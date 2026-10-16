Below are the bounded test-only corrections. I have not executed anything. The certificate SV tops and unit Python runner are unchanged, as requested; only the certificate C++ reference model is replaced and the four C++/Python files get focused edits.

Key corrections relative to the previous draft:

- Certificate C++ reference no longer mirrors the DUT registers. It uses an independent accepted-job deque with raw `length/src/dst/normal` metadata plus a poison flag.
- All C++ permit drives use ordinary C++ values: `0u`, `1u`, `3u`. No `2'b00`, `2'b01`, `2'b11` remains in C++.
- `--fullbuffer-permit` is no longer restricted to the two solo scenarios. It requires only `--couple-depth 512`, and with no `--scenario` it runs all 12 existing scenarios.
- `tests/nonblock_banked_test.cpp` explicitly drives `fullbuffer_jobs_i = 0` in its reset/drive paths.
- Banked focused admission test qualifies DID10 only; DID12 stays blocked under the held A write. No invalid AW reorder is attempted. Existing logger/property asserts are retained.

---

## 1. Complete corrected certificate C++

Path: `tests/fullbuffer_certificate_test.cpp`

```cpp
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
```

---

## 2. Exact focused diffs

### 2.1 `tests/banked_ctl_test.cpp`

```diff
@@ -123,6 +123,10 @@
 static Up up;
 
+// v5 full-buffer admission proxy.  Existing tests leave this at 0 so the
+// legacy read guard is unchanged.
+static uint32_t g_fullbuffer_jobs = 0;
+
 // ============================================================================
 // Behavioural models
 // ============================================================================
@@ -277,6 +281,8 @@
     dut->tr_b_ready = up.b_ready ? 1 : 0;
     dut->tr_r_ready = up.r_ready ? 1 : 0;
 
+    dut->fullbuffer_jobs_i = (uint8_t)(g_fullbuffer_jobs & 0x3u);
+
     // model driven
     dut->trans_req_ready   = sim.xl_accept ? 1 : 0;
     dut->trans_resp_valid  = sim.xl_resp_valid ? 1 : 0;
@@ -476,6 +482,7 @@
     dut->rst_ni = 0;
     up  = Up();
     sim = Sim();
+    g_fullbuffer_jobs = 0;
     g_xlate.clear();
     g_exp_wr.clear();
     g_exp_rd.clear();
@@ -782,6 +789,105 @@
     expect(!dut->in_flight_o, "in_flight_o stuck after error read");
 }
 
+// ============================================================================
+// v5 full-buffer READ admission:
+//   * bank 1 has an accepted A AW with no W beats, so w_unfilled_q > 0 and
+//     bank 1 is the W owner;
+//   * a queued bank-0 READ with DID10 (the bank-0 FullBufferDID) is admitted
+//     only when fullbuffer_jobs_i[0] is set;
+//   * a queued bank-0 READ with DID12 remains blocked even with permit bit 0
+//     set, because its DID does not match the bank's FullBufferDID.
+//
+// The A write is intentionally left unfinished: the bank-1 translation
+// response is held, so w_unfilled_q remains > 0.  The test is bounded and
+// never checks the global quiet condition for this unfinished write.  No
+// upstream AW reorder is attempted: exactly one legal AW is accepted.
+// ============================================================================
+static void test_fullbuffer_read_admission() {
+    std::printf("== v5 full-buffer READ admission vs W-owner guard ==\n");
+    reset_all();
+
+    // Hold bank-1 translation response so the A write stays in flight.
+    sim.xl_hold[1] = true;
+
+    // A (bank 1) AW accepted, no W beats: w_unfilled_q becomes 1 and the
+    // current W owner is bank 1.
+    add_xlate(0x2000, 0x50000000ull, false, 0xB);
+    std::vector<WBeat> no_w;
+    drive_write(0x4, 0x2000, 0, 0xB, no_w);   // AXI id bit 2 = 1 -> bank 1
+    run_cycles(20);
+    expect_eq(L_up_aw.size(), 1, "A AW was not accepted");
+    expect(sim.xl[1].busy, "bank-1 A translation was not selected / held");
+
+    // Expected DID10 and DID12 reads (both bank 0: AXI id bit 2 == 0).
+    expect_read(0x0, 0x4000, 0xCAFE1000ull, 0, false, 0xA);
+    expect_read(0x0, 0x5000, 0xCAFE2000ull, 0, false, 0xC);
+
+    // --- permit bit 0 clear: the DID10 read is buffered upstream but must not
+    //     be selected by the bank-0 controller.
+    g_fullbuffer_jobs = 0u;
+    drive_read(0x0, 0x4000, 0, 0xA);
+    run_cycles(40);
+    expect_eq(L_up_ar.size(), 1, "DID10 AR was not buffered upstream");
+    expect(!sim.xl[0].busy, "DID10 read was selected with fullbuffer permit=0");
+    expect_eq(L_cmp_ar.size(), 0, "DID10 reached completion with permit=0");
+
+    // --- permit bit 0 set: the queued qualified DID10 read is now selected
+    //     even though bank 1 is the unfilled W owner.
+    g_fullbuffer_jobs = 1u;
+    for (int i = 0; i < 160 && L_cmp_ar.size() == 0; ++i) sim_cycle();
+    expect_eq(L_cmp_ar.size(), 1, "DID10 completion AR count");
+    if (!L_cmp_ar.empty()) {
+        expect_eq(L_cmp_ar[0].addr, 0xCAFE1000ull, "DID10 comp AR addr");
+    }
+    for (int i = 0; i < 80; ++i) sim_cycle();
+    expect_eq(L_up_r.size(), 1, "DID10 upstream R count");
+
+    // --- same bank, non-matching DID12: permit bit 0 is still set, but the
+    //     original W-owner guard must keep it unselected.
+    g_fullbuffer_jobs = 1u;
+    drive_read(0x0, 0x5000, 0, 0xC);
+    run_cycles(80);
+    expect_eq(L_up_ar.size(), 2, "DID12 AR was not buffered upstream");
+    expect_eq(L_cmp_ar.size(), 1, "DID12 read was selected despite non-matching FullBufferDID");
+
+    // Keep the A write unfinished; this test is bounded and never checks quiet.
+}
+
 // ============================================================================
 int main(int argc, char **argv) {
@@ -799,6 +905,7 @@
     test_read_bank0_single();
     test_read_bank1_multi();
     test_error_read_bank0();
+    test_fullbuffer_read_admission();
 
     dut->final();
     delete dut;
```

---

### 2.2 `tests/nonblock_banked_test.cpp`

```diff
@@ -210,6 +210,7 @@
     void drive_upstream() {
+        top->fullbuffer_jobs_i = 0;
         top->tr_aw_valid = up.aw_valid ? 1 : 0;
         top->tr_aw_id    = up.aw_id;
         top->tr_aw_addr  = up.aw_addr;
@@ -499,6 +500,7 @@
     void reset(bool enable) {
         nonblock_enable = enable;
         top->clk_i  = 0;
         top->rst_ni = 0;
+        top->fullbuffer_jobs_i = 0;
 
         up = Up();
         caw_q.clear(); b_q.clear(); r_q.clear(); comp_w_q.clear();
```

---

### 2.3 `tests/nonblock_coupled_test.cpp`

```diff
@@ -256,6 +256,15 @@
     // Overlap mode: honor both halves of one 4 KiB job concurrently.
     bool overlap_mode = false;
 
+    // v5 opt-in proxy: drive fullbuffer_jobs_i = 3u for the bounded 4 KiB
+    // candidate.  Default false -> 0u (legacy guard).  With +couple-depth=512
+    // the harness runs all 12 existing scenarios; each scenario has one 4 KiB
+    // job per DMA, and the A-enabled scenarios therefore carry two jobs on the
+    // two separate 512-deep coupling FIFOs.
+    bool fullbuffer_permit = false;
+
     // Per-DMA selectors for the half whose AR / AW is currently on the wire,
     // and per-DMA pointers to the half of the next returning R / B / W beat.
     // They are all no-ops in serial mode (they stay 0 or 1).
@@ -561,6 +570,7 @@
     void tick() {
         top->nonblock_enable_i = nonblock_enable ? 1 : 0;
+        top->fullbuffer_jobs_i = fullbuffer_permit ? 3u : 0u;
 
         for (int d = 0; d < 2; ++d) dma_progress(d);
 
@@ -1044,6 +1054,7 @@
     void reset(bool enable) {
         nonblock_enable = enable;
         top->clk_i  = 0;
         top->rst_ni = 0;
+        top->fullbuffer_jobs_i = fullbuffer_permit ? 3u : 0u;
 
         dma[0] = Dma(); dma[1] = Dma();
         aw_order.clear(); w_owner = -1;
@@ -1210,6 +1221,7 @@
     std::string order = "vv";
     std::string couple_depth_str = "3";
     bool overlap = false;
+    bool fullbuffer_permit = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
@@ -1220,6 +1232,7 @@
         if (a.rfind(ckey, 0) == 0) couple_depth_str = a.substr(ckey.size());
         if (a == "--overlap") overlap = true;
+        if (a == "--fullbuffer-permit") fullbuffer_permit = true;
     }
@@ -1252,6 +1265,13 @@
     std::printf("nonblock_coupled_test: overlap_mode=%d\n", (int)overlap);
     std::printf("nonblock_coupled_test: couple_depth=%u\n", couple_depth);
+    if (fullbuffer_permit && couple_depth != 512) {
+        std::fprintf(stderr, "nonblock_coupled_test: --fullbuffer-permit "
+                             "requires +couple-depth=512\n");
+        return 2;
+    }
+    std::printf("nonblock_coupled_test: fullbuffer_permit=%d\n",
+                (int)fullbuffer_permit);
 
     int ran = 0, failed = 0;
     for (const auto& s : plan) {
@@ -1264,6 +1284,7 @@
             tb.overlap_mode    = overlap;
             tb.couple_depth    = couple_depth;
+            tb.fullbuffer_permit = fullbuffer_permit;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
         } catch (const std::exception& e) {
```

---

### 2.4 `tests/test_nonblock_coupled.py`

```diff
@@ -97,6 +97,17 @@
                          "harness back-pressures and checks at both depths.")
+    ap.add_argument("--fullbuffer-permit", action="store_true",
+                    help="drive the 2-bit full-buffer permit vector as 3 during "
+                         "the bounded 4 KiB copy (proxy for the real iDMA "
+                         "full-buffer certificate). Requires --couple-depth 512. "
+                         "With no --scenario this runs all 12 existing RR "
+                         "scenarios; each scenario has one 4 KiB job per DMA, so "
+                         "the A-enabled scenarios exercise two jobs on the two "
+                         "separate 512-deep coupling FIFOs. Default is 0 (legacy "
+                         "guard).")
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
@@ -102,6 +113,9 @@
     opts = ap.parse_args()
 
+    if opts.fullbuffer_permit and opts.couple_depth != 512:
+        ap.error("--fullbuffer-permit requires --couple-depth 512")
+
     sibling = load_sibling()
     tb = ROOT / "tests" / "nonblock_coupled_test.cpp"
     sources = dedup([require(p) for p in sibling.SOURCES])
@@ -164,8 +178,14 @@
     if opts.overlap:
         run_args.append("--overlap")
-    print("test_nonblock_coupled: couple_depth={} order={} overlap={}".format(
-        opts.couple_depth, opts.order, int(opts.overlap)))
+    if opts.fullbuffer_permit:
+        run_args.append("--fullbuffer-permit")
+    print("test_nonblock_coupled: couple_depth={} order={} overlap={} "
+          "fullbuffer_permit={}".format(
+              opts.couple_depth, opts.order, int(opts.overlap),
+              int(opts.fullbuffer_permit)))
     run = subprocess.run(run_args)
     return run.returncode
```

---

## 3. Acceptance commands to run at repository root

These are proposed commands. I have not run them.

```bash
# Certificate unit: CapacityBytes=4096 and 24 in one build.
python3 tests/test_fullbuffer_certificate.py

# Banked controller focused test, including the DID10/DID12 admission case.
python3 tests/test_banked_ctl.py

# Direct banked legacy guard: fullbuffer_jobs_i is explicitly driven to 0.
python3 tests/test_nonblock_banked.py

# Guarded RR all 12 Mode1/Mode2 scenarios (real TL + shared R-return queue).
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512

# Qualified RR all 12 Mode1/Mode2 scenarios with the full-buffer permit proxy.
# There is intentionally no --scenario restriction.
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 --fullbuffer-permit
```

The existing `COUPLED-SUMMARY` line already prints the observed fast/probe counters from the existing taps: `comp_ar`, `ddtw_ar`, `ptw_ar`, `tl_accept`, `tl_resp`, `iotlb_look`, `iotlb_hit`, `ptw_active`, `r_beats`, `b_up`. No new internal tap is invented, and no new assertion forces those counters positive in all 12 scenarios.

---

## 4. Unresolved / verify-before-merge

- The certificate module supplied is self-contained and does not import `idma_pkg`; the unit Python runner therefore does not need a package addition.
- The banked focused test assumes the production mapping `fullbuffer_jobs_i[b] -> child.fullbuffer_job_i` and `FullBufferDID = 10 + b`. If the actual mapping differs, the DID10/DID12 expectations must be adjusted.
- The banked focused test uses one legal A AW and does not attempt an upstream AW bank reorder. DID12 remains blocked under the held A write.
- The coupled permit proxy is a bounded 4 KiB buffer/harness proxy. It is not actual frontend integration and is not a universal deadlock proof.
- I have not executed anything; all pass/fail status must be verified by Codex against the actual integrated RTL and runner scripts.