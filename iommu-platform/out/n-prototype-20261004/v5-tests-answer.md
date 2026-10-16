Below are bounded v5 **test-only** proposals. No production RTL is proposed or modified. I have not executed anything; all results must be verified by Codex against the actual planned interfaces.

Key assumptions, stated once:

- The planned production module is `cva6/corev_apu/iDMA/src/idma_fullbuffer_certificate.sv` with exactly the scalar ports and parameter listed in the task.
- `rv_iommu_banked_ctl` gains `input logic [1:0] fullbuffer_jobs_i`; bit `b` is intended to drive the bank-`b` child `rv_iommu_trans_ctl_wrap.fullbuffer_job_i`.
- `rv_iommu_trans_ctl_wrap` gains `input logic fullbuffer_job_i` and parameter `FullBufferDID[23:0]`, with the banked wrapper intending `FullBufferDID = 10 + b`.
- Existing C++ harnesses instantiate `Vnonblock_banked_top`; they must drive the new input to `2'b00` by default. I patch `nonblock_coupled_test.cpp` here. If `tests/nonblock_banked_test.cpp` exists, the same one-line default must be added there.
- The certificate unit runner assumes the DUT is self-contained. If it imports `idma_pkg`, add that package to the runner source list before the DUT.

---

## 1. New actual certificate unit test

### 1.1 `tests/fullbuffer_certificate_test_top.sv`

````systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded scalar-I/O harness for the v5 idma_fullbuffer_certificate.
//
// Two real DUT instances are built in one top:
//   * CapacityBytes = 4096  (positive-capacity variant)
//   * CapacityBytes = 24    (always-no-grant variant)
//
// No clock generation, no `initial` stimulus, no behavioural models.
// All clocks, stimulus, and pre-edge checking live in
// tests/fullbuffer_certificate_test.cpp.

module fullbuffer_certificate_test_top #(
    parameter unsigned CapacityBytesLarge = 4096,
    parameter unsigned CapacityBytesSmall = 24
) (
    input  logic        clk_i,
    input  logic        rst_ni,

    input  logic        cap4096_job_fire_i,
    input  logic        cap4096_job_done_i,
    input  logic        cap4096_normal_copy_i,
    input  logic [63:0] cap4096_length_i,
    input  logic [63:0] cap4096_src_i,
    input  logic [63:0] cap4096_dst_i,
    output logic        cap4096_permit_o,

    input  logic        cap24_job_fire_i,
    input  logic        cap24_job_done_i,
    input  logic        cap24_normal_copy_i,
    input  logic [63:0] cap24_length_i,
    input  logic [63:0] cap24_src_i,
    input  logic [63:0] cap24_dst_i,
    output logic        cap24_permit_o
);

    idma_fullbuffer_certificate #(
        .CapacityBytes (CapacityBytesLarge)
    ) i_cap4096 (
        .clk_i,
        .rst_ni,
        .job_fire_i    (cap4096_job_fire_i),
        .job_done_i    (cap4096_job_done_i),
        .normal_copy_i (cap4096_normal_copy_i),
        .length_i      (cap4096_length_i),
        .src_i         (cap4096_src_i),
        .dst_i         (cap4096_dst_i),
        .permit_o      (cap4096_permit_o)
    );

    idma_fullbuffer_certificate #(
        .CapacityBytes (CapacityBytesSmall)
    ) i_cap24 (
        .clk_i,
        .rst_ni,
        .job_fire_i    (cap24_job_fire_i),
        .job_done_i    (cap24_job_done_i),
        .normal_copy_i (cap24_normal_copy_i),
        .length_i      (cap24_length_i),
        .src_i         (cap24_src_i),
        .dst_i         (cap24_dst_i),
        .permit_o      (cap24_permit_o)
    );

endmodule
````

### 1.2 `tests/fullbuffer_certificate_test.cpp`

````cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded v5 idma_fullbuffer_certificate unit test.
//
// Verilator 4.038 scalar ports, C++ pre-edge clocks.
// The harness owns the reference model: no internal DUT signal is inspected.
// The reference is deliberately fail-closed and checks the permission
// property "grant only for a sole qualified pending job".

#include "Vfullbuffer_certificate_test_top.h"
#include "verilated.h"

#include <cstdio>
#include <cstdint>
#include <string>

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
struct Ref {
    bool pending         = false;
    bool stored_eligible = false;
    bool sticky_fault    = false;
};

static bool eligible(bool normal_copy, uint64_t length,
                     uint64_t src, uint64_t dst, unsigned capacity) {
    return normal_copy &&
           (length == 4096ULL) &&
           ((src & 0xFFFULL) == 0ULL) &&
           ((dst & 0xFFFULL) == 0ULL) &&
           (capacity >= 4096u);
}

static bool ref_permit(const Ref& r, bool normal_copy_live) {
    return r.pending && r.stored_eligible && !r.sticky_fault && normal_copy_live;
}

static void ref_step(Ref& r,
                     bool fire, bool done, bool normal_copy,
                     uint64_t length, uint64_t src, uint64_t dst,
                     unsigned capacity) {
    const bool elig = eligible(normal_copy, length, src, dst, capacity);

    if (!r.pending) {
        if (fire && done) {
            // Immediate job completion: no pending, no grant.
            r.pending         = false;
            r.stored_eligible = false;
        } else if (fire) {
            r.pending         = true;
            r.stored_eligible = elig;
        } else if (done) {
            // Unexpected done with no pending.
            r.sticky_fault = true;
        }
    } else {
        if (fire && done) {
            // Atomic replacement of the current job eligibility.
            r.pending         = true;
            r.stored_eligible = elig;
        } else if (fire) {
            // Second fire without completion while pending.
            r.sticky_fault = true;
        } else if (done) {
            r.pending         = false;
            r.stored_eligible = false;
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
                 Ref& lr, Ref& sr, bool reset = false) {
    top->rst_ni = reset ? 0 : 1;
    top->clk_i  = 0;
    drive_large(ls);
    drive_small(ss);
    top->eval();

    if (reset) {
        CHECK(!permit_large(), "large permit asserted during reset");
        CHECK(!permit_small(), "small permit asserted during reset");
        lr = Ref();
        sr = Ref();
    } else {
        const bool lexp = ref_permit(lr, ls.normal);
        const bool sexp = ref_permit(sr, ss.normal);

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

        ref_step(lr, ls.fire, ls.done, ls.normal,
                 ls.length, ls.src, ls.dst, 4096u);
        ref_step(sr, ss.fire, ss.done, ss.normal,
                 ss.length, ss.src, ss.dst, 24u);
    }

    top->clk_i = 1;
    top->eval();
    top->clk_i = 0;
    top->eval();
    ++g_cycle;
}

static void reset_seq(Ref& lr, Ref& sr) {
    for (int i = 0; i < 4; ++i) tick(idle(), idle(), lr, sr, true);
    tick(idle(), idle(), lr, sr, false);
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------
static void test_reset_and_idle() {
    std::printf("== reset / no early permit ==\n");
    Ref lr, sr;
    reset_seq(lr, sr);

    for (int i = 0; i < 8; ++i) {
        tick(idle(), idle(), lr, sr);
        CHECK(!permit_large(), "large permit without fire");
        CHECK(!permit_small(), "small permit without fire");
    }
}

static void test_valid_4k_held_and_done() {
    std::printf("== valid 4K, held backpressure, completion clear ==\n");
    Ref lr, sr;
    reset_seq(lr, sr);

    tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
    CHECK(permit_large(), "large valid 4K not granted");
    CHECK(!permit_small(), "Capacity=24 granted valid 4K");

    for (int i = 0; i < 12; ++i) {
        tick(idle(), idle(), lr, sr);
        CHECK(permit_large(), "large permit not held while pending");
    }

    tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
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
        Ref lr, sr;
        reset_seq(lr, sr);
        tick(c.s, idle(), lr, sr);
        CHECK(!permit_large(), c.name);
        tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
    }
}

static void test_live_normal_copy() {
    std::printf("== live normal_copy gate ==\n");
    Ref lr, sr;
    reset_seq(lr, sr);

    tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
    CHECK(permit_large(), "large did not grant before live gate");

    tick(stim(false, false, false, 0, 0, 0), idle(), lr, sr);
    CHECK(!permit_large(), "live normal_copy=0 did not gate permit");

    tick(idle(), idle(), lr, sr);
    CHECK(permit_large(), "live normal_copy=1 did not restore permit");
}

static void test_simultaneous_replace_and_immediate() {
    std::printf("== simultaneous done+new fire ==\n");

    {
        Ref lr, sr;
        reset_seq(lr, sr);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(permit_large(), "initial valid fire");

        tick(stim(true, true, true, 4096, 0x3000, 0x4000), idle(), lr, sr);
        CHECK(permit_large(), "simultaneous done+new valid did not replace");

        tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
        CHECK(!permit_large(), "replacement permit survived done");
    }

    {
        Ref lr, sr;
        reset_seq(lr, sr);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);

        tick(stim(true, true, true, 2048, 0, 0), idle(), lr, sr);
        CHECK(!permit_large(), "simultaneous done+new invalid did not replace to no-grant");

        tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
        CHECK(!permit_large(), "invalid replacement left permit high");
    }

    {
        Ref lr, sr;
        reset_seq(lr, sr);

        tick(stim(true, true, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(!permit_large(), "immediate fire+done granted");

        tick(idle(), idle(), lr, sr);
        CHECK(!permit_large(), "immediate fire+done left pending");

        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(permit_large(), "valid fire after immediate completion did not grant");
    }
}

static void test_sticky_fault_and_reset() {
    std::printf("== sticky fault: done with no pending ==\n");
    {
        Ref lr, sr;
        reset_seq(lr, sr);

        tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
        CHECK(!permit_large(), "done with no pending granted");

        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(!permit_large(), "underflow sticky fault did not revoke");

        tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
        CHECK(!permit_large(), "underflow sticky fault cleared by done");

        reset_seq(lr, sr);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(permit_large(), "reset did not clear underflow sticky fault");
    }

    std::printf("== sticky fault: second fire while pending ==\n");
    {
        Ref lr, sr;
        reset_seq(lr, sr);

        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(permit_large(), "initial valid fire");

        tick(stim(true, false, true, 4096, 0x3000, 0x4000), idle(), lr, sr);
        CHECK(!permit_large(), "second fire while pending did not set sticky fault");

        tick(stim(false, true, true, 0, 0, 0), idle(), lr, sr);
        CHECK(!permit_large(), "second-fire sticky fault cleared by done");

        reset_seq(lr, sr);
        tick(stim(true, false, true, 4096, 0x1000, 0x2000), idle(), lr, sr);
        CHECK(permit_large(), "reset did not clear second-fire sticky fault");
    }
}

static void test_capacity_24_vs_4096() {
    std::printf("== CapacityBytes 24 vs 4096 ==\n");
    Ref lr, sr;
    reset_seq(lr, sr);

    Stim valid = stim(true, false, true, 4096, 0x1000, 0x2000);
    tick(valid, valid, lr, sr);

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
````

### 1.3 `tests/test_fullbuffer_certificate.py`

````python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the v5 idma_fullbuffer_certificate bounded unit test.

Builds tests/fullbuffer_certificate_test_top.sv with two real DUT instances:
CapacityBytes=4096 and CapacityBytes=24.  The C++ harness owns the clocks and
checks every permit pre-edge.

If the certificate RTL imports idma_pkg, add that package to `required`
before the certificate source and keep the package order before the DUT.
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IDMA = ROOT / "cva6" / "corev_apu" / "iDMA"
OUT  = ROOT / "out" / "fullbuffer-certificate"
TOP  = "fullbuffer_certificate_test_top"


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path):
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None or p in seen:
            continue
        seen.add(p)
        out.append(p)
    return out


def main() -> int:
    src = IDMA / "src"
    inc = IDMA / "include"

    required = [
        src / "idma_fullbuffer_certificate.sv",
        ROOT / "tests" / "fullbuffer_certificate_test_top.sv",
        ROOT / "tests" / "fullbuffer_certificate_test.cpp",
    ]

    sources = dedup([require(p) for p in required])

    include_dirs = [src]
    if inc.is_dir():
        include_dirs.append(inc)
    for d in include_dirs:
        if not d.exists():
            sys.exit("required include directory missing: {}".format(d))

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module",
        TOP,
        "--Mdir",
        str(obj_dir),
        "-GCapacityBytesLarge=4096",
        "-GCapacityBytesSmall=24",
    ]
    args += ["-I{}".format(d) for d in include_dirs]
    args += [str(p) for p in sources]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
````

---

## 2. `tests/nonblock_banked_top.sv` patch

Add the input, connect it to `rv_iommu_banked_ctl`, and explicitly tie the legacy direct `rv_iommu_trans_ctl_wrap` input to `1'b0`.

````diff
--- a/tests/nonblock_banked_top.sv
+++ b/tests/nonblock_banked_top.sv
@@ -148,6 +148,13 @@ module nonblock_banked_top
     input  logic        nonblock_enable_i,
     output logic        nonblock_fire_o,
     output logic        nonblock_busy_o,
     output logic        in_flight_o,
+
+    // ---------------- v5 full-buffer job permits (test input) ----------------
+    // Bit b is the bank-b full-buffer admission permit.  2'b00 preserves the
+    // legacy guarded behaviour; the coupled harness drives 2'b11 only under
+    // the opt-in --fullbuffer-permit proxy.
+    input  logic [1:0]  fullbuffer_jobs_i,
 
     // ---------------- test-only observation (no DUT behaviour change) -------
     // These ports only tap existing internal wires (trans_req_* / trans_resp_*
@@ -634,6 +641,7 @@ module nonblock_banked_top
     ) i_banked (
         .clk_i,
         .rst_ni,
+        .fullbuffer_jobs_i  (fullbuffer_jobs_i),
         .tr_axi_req_i       (tr_req),
         .tr_axi_resp_o      (tr_resp),
         .comp_axi_resp_i    (comp_resp),
@@ -678,6 +686,7 @@ module nonblock_banked_top
     ) i_legacy (
         .clk_i,
         .rst_ni,
         .read_admit_i(1'b1),
+        .fullbuffer_job_i(1'b0),
         .tr_axi_req_i       (tr_req),
         .tr_axi_resp_o      (tr_resp),
         .comp_axi_resp_i    (comp_resp),
````

If the new `rv_iommu_trans_ctl_wrap` parameter has no default, also add `.FullBufferDID(24'h0)` to the legacy parameter list.

---

## 3. `tests/banked_ctl_test_top.sv` patch

Add `fullbuffer_jobs_i` and connect it to the real banked controller.

````diff
--- a/tests/banked_ctl_test_top.sv
+++ b/tests/banked_ctl_test_top.sv
@@ -12,6 +12,10 @@ module banked_ctl_test_top (
     input  logic         clk_i,
     input  logic         rst_ni,
+
+    // v5 full-buffer job permits: bit b is the bank-b permit.
+    // Existing tests keep this at 2'b00.
+    input  logic [1:0]   fullbuffer_jobs_i,
 
     // ---------------- upstream translation-request AXI : AW ------------------
     input  logic         tr_aw_valid,
@@ -444,6 +448,7 @@ module banked_ctl_test_top (
     ) i_dut (
         .clk_i,
         .rst_ni,
+        .fullbuffer_jobs_i  (fullbuffer_jobs_i),
         .tr_axi_req_i       (tr_req),
         .tr_axi_resp_o      (tr_resp),
         .comp_axi_resp_i    (comp_resp),
````

---

## 4. `tests/banked_ctl_test.cpp` patch

Add the default-zero drive, reset it, and add the focused real-banked-controller admission test.

````diff
--- a/tests/banked_ctl_test.cpp
+++ b/tests/banked_ctl_test.cpp
@@ -122,6 +122,10 @@ struct Up {
 };
 static Up up;
 
+// v5 full-buffer admission proxy.  Existing tests leave this at 0 so the
+// legacy read guard is unchanged.
+static uint32_t g_fullbuffer_jobs = 0;
+
 // ============================================================================
 // Behavioural models
 // ============================================================================
@@ -276,6 +280,8 @@ static void drive_upstream() {
     dut->tr_b_ready = up.b_ready ? 1 : 0;
     dut->tr_r_ready = up.r_ready ? 1 : 0;
 
+    dut->fullbuffer_jobs_i = g_fullbuffer_jobs;
+
     // model driven
     dut->trans_req_ready   = sim.xl_accept ? 1 : 0;
     dut->trans_resp_valid  = sim.xl_resp_valid ? 1 : 0;
@@ -474,6 +480,7 @@ static void reset_all() {
     dut->rst_ni = 0;
     up  = Up();
     sim = Sim();
+    g_fullbuffer_jobs = 0;
     g_xlate.clear();
     g_exp_wr.clear();
     g_exp_rd.clear();
@@ -782,6 +789,101 @@ static void test_error_read_bank0() {
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
+// never checks the global quiet condition for this unfinished write.
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
+    g_fullbuffer_jobs = 2'b00;
+    drive_read(0x0, 0x4000, 0, 0xA);
+    run_cycles(40);
+    expect_eq(L_up_ar.size(), 1, "DID10 AR was not buffered upstream");
+    expect(!sim.xl[0].busy, "DID10 read was selected with fullbuffer permit=0");
+    expect_eq(L_cmp_ar.size(), 0, "DID10 reached completion with permit=0");
+
+    // --- permit bit 0 set: the queued qualified DID10 read is now selected
+    //     even though bank 1 is the unfilled W owner.
+    g_fullbuffer_jobs = 2'b01;
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
+    g_fullbuffer_jobs = 2'b01;
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
     Verilated::commandArgs(argc, argv);
@@ -797,6 +899,7 @@ int main(int argc, char **argv) {
     test_read_bank0_single();
     test_read_bank1_multi();
     test_error_read_bank0();
+    test_fullbuffer_read_admission();
 
     dut->final();
     delete dut;
````

Note: this test deliberately does not drive W beats for the A AW and does not simulate upstream AW bank reorder. It only requires the upstream demux to accept one legal AW, then checks that the queued READ selection is gated by the full-buffer admission override.

---

## 5. `tests/nonblock_coupled_test.cpp` patch

Add the opt-in proxy, default zero, and strict validation.

````diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -256,6 +256,11 @@ public:
     // Overlap mode: honor both halves of one 4 KiB job concurrently.
     bool overlap_mode = false;
 
+    // v5 opt-in proxy: drive fullbuffer_jobs_i = 2'b11 for the bounded 4 KiB
+    // solo candidate.  Default false -> 2'b00 (legacy guard).
+    bool fullbuffer_permit = false;
+
     // Per-DMA selectors for the half whose AR / AW is currently on the wire,
     // and per-DMA pointers to the half of the next returning R / B / W beat.
     // They are all no-ops in serial mode (they stay 0 or 1).
@@ -562,6 +567,7 @@ public:
     void tick() {
         top->nonblock_enable_i = nonblock_enable ? 1 : 0;
+        top->fullbuffer_jobs_i = fullbuffer_permit ? 2'b11 : 2'b00;
 
         for (int d = 0; d < 2; ++d) dma_progress(d);
 
@@ -1044,6 +1050,7 @@ public:
         nonblock_enable = enable;
         top->clk_i  = 0;
         top->rst_ni = 0;
+        top->fullbuffer_jobs_i = fullbuffer_permit ? 2'b11 : 2'b00;
 
         dma[0] = Dma(); dma[1] = Dma();
         aw_order.clear(); w_owner = -1;
@@ -1210,6 +1217,7 @@ int main(int argc, char** argv) {
     std::string only;
     std::string order = "vv";
     std::string couple_depth_str = "3";
     bool overlap = false;
+    bool fullbuffer_permit = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
@@ -1220,6 +1228,7 @@ int main(int argc, char** argv) {
         const std::string ckey = "+couple-depth=";
         if (a.rfind(ckey, 0) == 0) couple_depth_str = a.substr(ckey.size());
         if (a == "--overlap") overlap = true;
+        if (a == "--fullbuffer-permit") fullbuffer_permit = true;
     }
@@ -1251,6 +1260,21 @@ int main(int argc, char** argv) {
     std::printf("nonblock_coupled_test: overlap_mode=%d\n", (int)overlap);
     std::printf("nonblock_coupled_test: couple_depth=%u\n", couple_depth);
 
+    if (fullbuffer_permit) {
+        if (couple_depth != 512) {
+            std::fprintf(stderr, "nonblock_coupled_test: --fullbuffer-permit "
+                                 "requires +couple-depth=512\n");
+            return 2;
+        }
+        if (only != "mode2-solo-l1" && only != "mode2-solo-l31") {
+            std::fprintf(stderr, "nonblock_coupled_test: --fullbuffer-permit "
+                                 "requires a 4 KiB one-job scenario "
+                                 "(+scenario=mode2-solo-l1 or mode2-solo-l31)\n");
+            return 2;
+        }
+    }
+    std::printf("nonblock_coupled_test: fullbuffer_permit=%d\n",
+                (int)fullbuffer_permit);
+
     int ran = 0, failed = 0;
     for (const auto& s : plan) {
@@ -1264,6 +1288,7 @@ int main(int argc, char** argv) {
             tb.overlap_mode    = overlap;
             tb.couple_depth    = couple_depth;
+            tb.fullbuffer_permit = fullbuffer_permit;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
````

This preserves the default guarded behaviour and only drives `2'b11` when the opt-in flag is used with the bounded 4 KiB solo candidate.

---

## 6. `tests/test_nonblock_coupled.py` patch

Add the new CLI flag and require the known one-job 4 KiB scenario and depth 512.

````diff
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -88,6 +88,16 @@ def main() -> int:
     ap.add_argument("--couple-depth", type=int, choices=(3, 512), default=3,
                     help="per-DMA read->write coupling FIFO depth, forwarded to "
                          "the C++ harness as +couple-depth=<...> (default: 3). "
@@ -96,6 +106,13 @@ def main() -> int:
                          "harness back-pressures and checks at both depths.")
+    ap.add_argument("--fullbuffer-permit", action="store_true",
+                    help="drive the 2-bit full-buffer permit vector as 2'b11 "
+                         "during the bounded 4 KiB solo copy (proxy for the "
+                         "real iDMA full-buffer certificate). Requires "
+                         "--couple-depth 512 and --scenario mode2-solo-l1 or "
+                         "mode2-solo-l31. Default is 0 (legacy guard).")
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
@@ -102,6 +119,14 @@ def main() -> int:
     opts = ap.parse_args()
 
+    if opts.fullbuffer_permit:
+        if opts.couple_depth != 512:
+            ap.error("--fullbuffer-permit requires --couple-depth 512")
+        if opts.scenario not in ("mode2-solo-l1", "mode2-solo-l31"):
+            ap.error("--fullbuffer-permit requires a 4 KiB one-job scenario "
+                     "(--scenario mode2-solo-l1 or mode2-solo-l31)")
+
     sibling = load_sibling()
     tb = ROOT / "tests" / "nonblock_coupled_test.cpp"
@@ -164,8 +189,13 @@ def main() -> int:
     if opts.scenario:
         run_args.append("+scenario={}".format(opts.scenario))
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
````

The existing `COUPLED-SUMMARY` line remains unchanged and is the source of the available fast counts/probes. No new internal tap is invented.

---

## 7. Acceptance commands

Run from repository root. These are proposed commands; I have not executed them.

````bash
# 1) New certificate unit: CapacityBytes=4096 and 24 in one build.
python3 tests/test_fullbuffer_certificate.py

# 2) Banked controller focused test.  Use the existing banked_ctl runner name
#    in this repository; the added test function is test_fullbuffer_read_admission().
python3 tests/test_banked_ctl.py

# 3) Existing coupled defaults must remain byte-for-byte guarded.
python3 tests/test_nonblock_coupled.py --overlap --order rr

# 4) Guarded baseline for the bounded 4 KiB solo candidate.
python3 tests/test_nonblock_coupled.py --overlap --order rr \
    --couple-depth 512 --scenario mode2-solo-l1

# 5) Full-buffer permit proxy for the same bounded 4 KiB solo candidate.
python3 tests/test_nonblock_coupled.py --overlap --order rr \
    --couple-depth 512 --scenario mode2-solo-l1 --fullbuffer-permit

# Compare the two COUPLED-SUMMARY lines from (4) and (5).  This is the
# guarded-vs-permit admission-progress comparison; only the designed solo
# candidate is asserted positive, not all 12 scenarios.
````

---

## Unresolved / verify-before-merge

- The certificate runner assumes the DUT is self-contained. If `idma_fullbuffer_certificate.sv` imports `idma_pkg`, add `cva6/corev_apu/iDMA/src/idma_pkg.sv` to the runner source list before the DUT.
- The banked focused test assumes the planned production mapping `fullbuffer_jobs_i[b] -> child.fullbuffer_job_i` and `FullBufferDID = 10 + b`. If the actual mapping differs, the DID10/DID12 expectations must be adjusted.
- The banked focused test uses the existing legal AXI shape: one AW accepted, W intentionally not supplied, one queued READ selected/blocked. It does not simulate an illegal upstream AW bank reorder.
- `tests/nonblock_banked_test.cpp` was not supplied. If it instantiates `Vnonblock_banked_top`, add `top->fullbuffer_jobs_i = 0;` in its reset/drive path exactly as done in `nonblock_coupled_test.cpp`.
- No production RTL, cache, log, or board changes are proposed.