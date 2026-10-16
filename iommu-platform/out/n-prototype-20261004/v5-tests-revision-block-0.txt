// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded v5 idma_fullbuffer_certificate unit test.
//
// Verilator 4.038 scalar ports, C++ pre-edge clocks.
// The harness owns the reference model: no internal DUT signal is inspected.
// The reference is deliberately fail-closed and checks the permission
// property "grant only for a sole qualified accepted job".
//
// The reference model stores accepted jobs in an independent deque.  It does
// not mirror the DUT pending_q / eligible_q / fault_q registers.  It retires
// the front job on done, appends a job on fire, and poisons permission if it
// observes an unexpected retirement or an overlap (a second accepted job while
// one is still live).  Permission is derived only from the single live job's
// raw geometry plus the live normal_copy_i input, and capacity.

#include "Vfullbuffer_certificate_test_top.h"
#include "verilated.h"

#include <cstdio>
#include <cstdint>
#include <deque>

static long g_cycle = 0;
static int  g_fail  = 0;
static Vfullbuffer_certificate_test_top* top = nullptr;

double sc_time_stamp() { return static_cast<double>(g_cycle); }

static void fail(const char* msg, const char* file, int line) {
    std::printf("[cycle %ld] FAIL: %s (%s:%d)\n", g_cycle, msg, file, line);
    ++g_fail;
}
#define CHECK(cond, msg) do { if (!(cond)) fail((msg), __FILE__, __LINE__); } while (0)

// ---------------------------------------------------------------------------
// Independent reference model
// ---------------------------------------------------------------------------
struct Job {
    uint64_t length = 0;
    uint64_t src    = 0;
    uint64_t dst    = 0;
    bool     normal = false;
};

struct Model {
    std::deque<Job> jobs;
    bool            poison = false;
};

static bool job_eligible(const Job& j, unsigned capacity) {
    return j.normal &&
           (j.length == 4096ULL) &&
           ((j.src & 0xFFFULL) == 0ULL) &&
           ((j.dst & 0xFFFULL) == 0ULL) &&
           (capacity >= 4096u);
}

static bool ref_permit(const Model& m, bool normal_copy_live, unsigned capacity) {
    if (m.poison) return false;
    if (m.jobs.size() != 1u) return false;
    const Job& j = m.jobs.front();
    return normal_copy_live && job_eligible(j, capacity);
}

static void ref_step(Model& m,
                     bool fire, bool done, bool normal,
                     uint64_t length, uint64_t src, uint64_t dst) {
    Job j;
    j.length = length;
    j.src    = src;
    j.dst    = dst;
    j.normal = normal;

    if (fire && done) {
        // Simultaneous completion + replacement.  If a job was live, the DUT
        // retires it and keeps one live successor.  If none was live, the DUT
        // treats this as an immediately-completed job and keeps no live job.
        if (!m.jobs.empty()) {
            m.jobs.pop_front();
            m.jobs.push_back(j);
        }
    } else if (fire) {
        if (!m.jobs.empty()) {
            // Overlap: a second accepted job while one is live.  Permission is
            // revoked until reset.
            m.poison = true;
        }
        m.jobs.push_back(j);
    } else if (done) {
        if (m.jobs.empty()) {
            // Unexpected retirement with no accepted live job.
            m.poison = true;
        } else {
            m.jobs.pop_front();
        }
    }
}

// ---------------------------------------------------------------------------
// Stimulus
// ---------------------------------------------------------------------------
struct Stim {
    bool     fire   = false;
    bool     done   = false;
    bool     normal = true;
    uint64_t length = 4096;
    uint64_t src    = 0;
    uint64_t dst    = 0;
};

static Stim stim(bool fire, bool done, bool normal,
                 uint64_t length, uint64_t src, uint64_t dst) {
    Stim s;
    s.fire = fire; s.done = done; s.normal = normal;
    s.length = length; s.src = src; s.dst = dst;
    return s;
}

static Stim idle() { return stim(false, false, true, 0, 0, 0); }

static void drive_large(const Stim& s) {
    top->cap4096_job_fire_i    = s.fire   ? 1 : 0;
    top->cap4096_job_done_i    = s.done   ? 1 : 0;
    top->cap4096_normal_copy_i = s.normal ? 1 : 0;
    top->cap4096_length_i      = s.length;
    top->cap4096_src_i         = s.src;
    top->cap4096_dst_i         = s.dst;
}

static void drive_small(const Stim& s) {
    top->cap24_job_fire_i    = s.fire   ? 1 : 0;
    top->cap24_job_done_i    = s.done   ? 1 : 0;
    top->cap24_normal_copy_i = s.normal ? 1 : 0;
    top->cap24_length_i      = s.length;
    top->cap24_src_i         = s.src;
    top->cap24_dst_i         = s.dst;
}

static bool permit_large() { return top->cap4096_permit_o != 0; }
static bool permit_small() { return top->cap24_permit_o != 0; }

static void tick(const Stim& ls, const Stim& ss,
                 Model& lm, Model& sm, bool reset = false) {
    top->rst_ni = reset ? 0 : 1;
    top->clk_i  = 0;
    drive_large(ls);
    drive_small(ss);
    top->eval();

    if (reset) {
        CHECK(!permit_large(), "large permit asserted during reset");
        CHECK(!permit_small(), "small permit asserted during reset");
        lm = Model();
        sm = Model();
    } else {
        const bool lexp = ref_permit(lm, ls.normal, 4096u);
        const bool sexp = ref_permit(sm, ss.normal, 24u);

        if (permit_large() != lexp) {
            std::printf("[cycle %ld] large permit mismatch: dut=%d ref=%d\n",
                        g_cycle, (int)permit_large(), (int)lexp);
            ++g_fail;
        }
        if (permit_small() != sexp) {
            std::printf("[cycle %ld] small permit mismatch: dut=%d ref=%d\n",
                        g_cycle, (int)permit_small(), (int)sexp);
            ++g_fail;
        }

        ref_step(lm, ls.fire, ls.done, ls.normal,
                 ls.length, ls.src, ls.dst);
        ref_step(sm, ss.fire, ss.done, ss.normal,
                 ss.length, ss.src, ss.dst);
    }

    top->clk_i = 1;
    top->eval();
    top->clk_i = 0;
    top->eval();
    ++g_cycle;
}

static void reset_seq(Model& lm, Model& sm) {
    for (int i = 0; i < 4; ++i) tick(idle(), idle(), lm, sm, true);
    tick(idle(), idle(), lm, sm, false);
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------
static void test_reset_and_idle() {
    std::printf("== reset / no early permit ==\n");
    Model lm, sm;
    reset_seq(lm, sm);

    for (int i = 0; i < 8; ++i) {
        tick(idle(), idle(), lm, sm);
        CHECK(!permit_large(), "large permit without fire");
        CHECK(!permit_small(), "small permit without fire");
    }
}

static void test_valid_4k_held_and_done() {
    std::printf("== valid 4K, held backpressure, completion clear ==\n");
    Model lm, sm;
    reset_seq(lm, sm);

    tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
    CHECK(permit_large(), "large valid 4K not granted");
    CHECK(!permit_small(), "Capacity=24 granted valid 4K");

    for (int i = 0; i < 12; ++i) {
        tick(idle(), idle(), lm, sm);
        CHECK(permit_large(), "large permit not held while pending");
    }

    tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
    CHECK(!permit_large(), "large permit not cleared by done");
    CHECK(!permit_small(), "small permit changed by done");
}

static void test_non4k_and_unaligned() {
    std::printf("== non-4K and unaligned ==\n");

    struct Case { const char* name; Stim s; };
    const Case cases[] = {
        { "length 2048",    stim(true, false, true,  2048, 0x1000, 0x2000) },
        { "src unaligned",  stim(true, false, true,  4096, 0x1001, 0x2000) },
        { "dst unaligned",  stim(true, false, true,  4096, 0x1000, 0x2001) },
        { "normal_copy=0",  stim(true, false, false, 4096, 0x1000, 0x2000) },
    };

    for (const auto& c : cases) {
        Model lm, sm;
        reset_seq(lm, sm);
        tick(c.s, idle(), lm, sm);
        CHECK(!permit_large(), c.name);
        tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
    }
}

static void test_live_normal_copy() {
    std::printf("== live normal_copy gate ==\n");
    Model lm, sm;
    reset_seq(lm, sm);

    tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
    CHECK(permit_large(), "large did not grant before live gate");

    tick(stim(false, false, false, 0, 0, 0), idle(), lm, sm);
    CHECK(!permit_large(), "live normal_copy=0 did not gate permit");

    tick(idle(), idle(), lm, sm);
    CHECK(permit_large(), "live normal_copy=1 did not restore permit");
}

static void test_simultaneous_replace_and_immediate() {
    std::printf("== simultaneous done+new fire ==\n");

    {
        Model lm, sm;
        reset_seq(lm, sm);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(permit_large(), "initial valid fire");

        tick(stim(true, true, true, 4096, 0x3000, 0x4000), idle(), lm, sm);
        CHECK(permit_large(), "simultaneous done+new valid did not replace");

        tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
        CHECK(!permit_large(), "replacement permit survived done");
    }

    {
        Model lm, sm;
        reset_seq(lm, sm);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);

        tick(stim(true, true, true, 2048, 0, 0), idle(), lm, sm);
        CHECK(!permit_large(), "simultaneous done+new invalid did not replace to no-grant");

        tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
        CHECK(!permit_large(), "invalid replacement left permit high");
    }

    {
        Model lm, sm;
        reset_seq(lm, sm);

        tick(stim(true, true, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(!permit_large(), "immediate fire+done granted");

        tick(idle(), idle(), lm, sm);
        CHECK(!permit_large(), "immediate fire+done left pending");

        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(permit_large(), "valid fire after immediate completion did not grant");
    }
}

static void test_sticky_fault_and_reset() {
    std::printf("== sticky fault: done with no pending ==\n");
    {
        Model lm, sm;
        reset_seq(lm, sm);

        tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
        CHECK(!permit_large(), "done with no pending granted");

        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(!permit_large(), "underflow sticky fault did not revoke");

        tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
        CHECK(!permit_large(), "underflow sticky fault cleared by done");

        reset_seq(lm, sm);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(permit_large(), "reset did not clear underflow sticky fault");
    }

    std::printf("== sticky fault: second fire while pending ==\n");
    {
        Model lm, sm;
        reset_seq(lm, sm);

        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(permit_large(), "initial valid fire");

        tick(stim(true, false, true, 4096, 0x3000, 0x4000), idle(), lm, sm);
        CHECK(!permit_large(), "second fire while pending did not set sticky fault");

        tick(stim(false, true, true, 0, 0, 0), idle(), lm, sm);
        CHECK(!permit_large(), "second-fire sticky fault cleared by done");

        reset_seq(lm, sm);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lm, sm);
        CHECK(permit_large(), "reset did not clear second-fire sticky fault");
    }
}

static void test_capacity_24_vs_4096() {
    std::printf("== CapacityBytes 24 vs 4096 ==\n");
    Model lm, sm;
    reset_seq(lm, sm);

    Stim valid = stim(true, false, true, 4096, 0x1000, 0x2000);
    tick(valid, valid, lm, sm);

    CHECK(permit_large(), "Capacity=4096 did not grant valid 4K");
    CHECK(!permit_small(), "Capacity=24 granted valid 4K");
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    top = new Vfullbuffer_certificate_test_top;

    test_reset_and_idle();
    test_valid_4k_held_and_done();
    test_non4k_and_unaligned();
    test_live_normal_copy();
    test_simultaneous_replace_and_immediate();
    test_sticky_fault_and_reset();
    test_capacity_24_vs_4096();

    top->final();
    delete top;

    if (g_fail) {
        std::printf("FAILED: %d check(s)\n", g_fail);
        return 1;
    }
    std::printf("PASS\n");
    return 0;
}
