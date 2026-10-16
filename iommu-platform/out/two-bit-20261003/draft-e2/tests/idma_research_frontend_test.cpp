// Directed Verilator test for ResearchPort batch admission and reverse order.
// Simulation only; no board claim and no execution claim.
#include "Vidma_research_frontend_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

static void ck(bool b, const char *s) {
    if (!b) throw std::runtime_error(s);
}

struct FE {
    Vidma_research_frontend_top d;
    uint64_t cycle = 0;
    bool valid = false, write = false;
    uint8_t addr = 0;
    uint64_t wdata = 0, wstrb = 0xff;
    bool ready = true;
    bool due_valid = false;
    uint64_t due = 0;
    bool policy_enable = false, policy_allow = false, policy_abort = false, batch_reverse = false;
    unsigned completions = 0;
    uint64_t last_done_id = 0;
    bool have_done = false;
    std::vector<uint64_t> srcs;

    void eval() {
        d.valid_i = valid;
        d.write_i = write;
        d.addr_i = addr;
        d.wdata_i = wdata;
        d.wstrb_i = wstrb;
        d.ready_i = ready;
        d.complete_i = due_valid && (cycle == due);
        d.backend_idle_i = !due_valid;
        d.policy_enable_i = policy_enable;
        d.policy_allow_i = policy_allow;
        d.policy_abort_i = policy_abort;
        d.batch_reverse_i = batch_reverse;
        d.eval();
    }

    void tick() {
        eval();
        if (d.rst_ni) {
            if (d.request_o && d.ready_i) {
                ck(!due_valid, "one outstanding task");
                srcs.push_back(d.src_o);
                due_valid = true;
                due = cycle + 3;
            }
        }
        if (d.rst_ni && d.complete_i) {
            ck(d.done_o, "batch done on completion");
            completions++;
            last_done_id = d.done_id_o;
            have_done = true;
            due_valid = false;
        }
        d.clk_i = 1; d.eval();
        cycle++;
        d.clk_i = 0; d.eval();
    }

    void reset() {
        d.rst_ni = 0;
        valid = false; write = false; addr = 0; wdata = 0; wstrb = 0xff;
        ready = true; due_valid = false; due = 0;
        policy_enable = false; policy_allow = false; policy_abort = false; batch_reverse = false;
        completions = 0; last_done_id = 0; have_done = false; srcs.clear();
        for (int i = 0; i < 3; i++) { eval(); d.clk_i = 1; d.eval(); d.clk_i = 0; d.eval(); }
        d.rst_ni = 1; eval(); d.clk_i = 1; d.eval(); d.clk_i = 0; d.eval();
    }

    FE() { reset(); }

    uint64_t access(unsigned a, bool wr = false, uint64_t val = 0, uint8_t strb = 0xff) {
        valid = true; write = wr; addr = (uint8_t)a; wdata = val; wstrb = strb;
        eval();
        ck(d.ready_o, "frontend MMIO ready");
        uint64_t r = d.rdata_o;
        tick();
        valid = false; wstrb = 0xff; eval();
        return r;
    }

    void configure(uint32_t target, unsigned p, bool reverse = false) {
        batch_reverse = reverse;
        access(0x00, true, 0x10000);
        access(0x08, true, 0x100000);
        access(0x10, true, 4096);
        access(0x18, true, 0);
        access(0x88, true, 0);
        access(0x90, true, target);
        access(0x98, true, p);
        access(0x88, true, 1);
    }

    void start() {
        uint64_t id = access(0x28);
        ck(id == 2, "one functional ID per batch");
    }

    void run_until(unsigned n, unsigned limit = 200000) {
        for (unsigned i = 0; i < limit && completions < n; i++) tick();
        ck(completions == n, "bounded completion count");
    }
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    try {
        unsigned cases = 0;

        // First copy: policy enable enters ADMIT and asserts no VALID until allow.
        {
            FE b; b.policy_enable = true; b.configure(4, 1, false); b.start();
            for (int i = 0; i < 5; i++) { b.tick(); ck(!b.d.request_o, "no VALID in ADMIT before allow"); }
            b.policy_allow = true;
            b.tick();
            ck(b.d.request_o, "VALID after allow");
            cases++;
        }

        // Per-copy ADMIT: after a completed copy, wait for allow again.
        {
            FE b; b.policy_enable = true; b.policy_allow = true; b.configure(2, 1, false); b.start();
            for (int i = 0; i < 100 && b.srcs.size() < 1; i++) b.tick();
            ck(b.srcs.size() == 1, "first copy offered");
            b.policy_allow = false;
            b.run_until(1);
            for (int i = 0; i < 4; i++) { b.tick(); ck(!b.d.request_o, "no second copy before allow"); }
            b.policy_allow = true;
            b.run_until(2);
            ck(b.srcs.size() == 2, "second copy offered after allow");
            cases++;
        }

        // COUNTVALID is held through a policy abort until backend ready.
        {
            FE b; b.policy_enable = true; b.policy_allow = true; b.configure(4, 1, false); b.start();
            for (int i = 0; i < 100 && !b.d.request_o; i++) b.tick();
            ck(b.d.request_o, "request pending in COUNT");
            uint64_t src0 = b.d.src_o;
            b.ready = false;
            b.policy_abort = true;
            for (int i = 0; i < 8; i++) {
                b.eval();
                ck(b.d.request_o, "abort cannot withdraw stalled VALID");
                ck(b.d.src_o == src0, "stalled payload stable");
                b.tick();
            }
            b.ready = true;
            b.tick();
            b.run_until(1);
            ck(b.completions == 1, "committed aborted copy completed once");
            b.policy_abort = false;
            for (int i = 0; i < 5; i++) { b.tick(); ck(!b.d.request_o, "abort retires after WAIT completion"); }
            cases++;
        }

        // STOP in ADMIT retires immediately and creates no obs_done.
        {
            FE b; b.policy_enable = true; b.policy_allow = false; b.configure(4, 1, false); b.start();
            b.tick();
            ck(!b.d.request_o, "ADMIT does not send VALID");
            b.access(0x88, true, 2);
            b.tick(); b.tick();
            ck(!b.d.batch_active_o, "STOP in ADMIT retires");
            ck(!b.have_done, "STOP in ADMIT no obs_done");
            cases++;
        }

        // Policy abort in ADMIT retires immediately and creates no obs_done.
        {
            FE b; b.policy_enable = true; b.policy_allow = false; b.configure(4, 1, false); b.start();
            b.policy_abort = true;
            b.tick(); b.tick();
            ck(!b.d.batch_active_o, "policy abort in ADMIT retires");
            ck(!b.have_done, "policy abort in ADMIT no obs_done");
            cases++;
        }

        // Reverse order is latched at batch issue, starts at pages-1, subtracts
        // 4 KiB and wraps.  target=8 and pages=4 exercises a full wrap.
        {
            FE b; b.policy_enable = true; b.policy_allow = true; b.configure(8, 4, true); b.start();
            b.run_until(8);
            ck(b.srcs.size() == 8, "eight reverse copies");
            const uint64_t base = 0x10000;
            for (int i = 0; i < 8; i++) {
                uint64_t expect = base + ((uint64_t)(3 - (i % 4)) << 12);
                ck(b.srcs[i] == expect, "reverse page order with wrap");
            }
            cases++;
        }

        std::cout << "PASS " << cases << " ResearchPort frontend scenarios\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "FAIL " << e.what() << "\n";
        return 1;
    }
}
