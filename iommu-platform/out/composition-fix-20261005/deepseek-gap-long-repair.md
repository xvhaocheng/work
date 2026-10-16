## What was wrong with the prior patch

The reference patch's *content* was mostly right, but its hunk anchors did not line up with the supplied file (e.g. its first hunk claimed `@@ -63,6 @@` while its first context line is actually file line 63's *predecessor* region, and hunk `@@ -104,18 +104,51 @@` claimed 33 added lines where only 23 were present). `--recount` fixes counts, not context anchoring, so the apply failed at the constants block. Below are regenerated patches: every context line was transcribed from the exact supplied file text, start lines and counts are internally consistent, and no line outside the two named files is touched.

Note on blank context lines: they are a **single space character** (`" "`). If any transport strips that trailing space, re-add it before applying.

---

## Patch 1 — `tests/nonblock_coupled_test.cpp`

```diff
diff --git a/tests/nonblock_coupled_test.cpp b/tests/nonblock_coupled_test.cpp
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -63,6 +63,21 @@
 // coupling-FIFO overflow CHECK (the f_up_r update), so a run can never
 // back-pressure at one depth and check another.
 //
+// Scenario selection (opt-in rows)
+// --------------------------------
+// The default plan is the historical 12 rows at 64 jobs each.  Four opt-in rows
+// carry the 16-page aggressor (mode1-a16-l1, mode1-a16-l31, mode2-a16-l1,
+// mode2-a16-l31); they run ONLY when named with +scenario=<name> (Python
+// --scenario).  With no +scenario they are skipped and the default run is
+// unchanged.
+//
+// Job count (opt-in, +job-count=64|4096)
+// --------------------------------------
+// 64 is the historical default and keeps every existing log valid; 4096 is the
+// bounded long continuous-work run.  Both are validated exactly.  Warm-up is
+// deliberately NOT scaled with the job count: the board warm256 is 256
+// transfers, not 256 pages, and this harness makes no board-warm256 claim.
+//
 // This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
 // it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no
 // ID remap.  It only reproduces the ordering/back-pressure shape of the board.
@@ -104,18 +104,38 @@
 static const uint32_t PAGE_BEATS   = 512;   // 4 KiB / 8 B
 static const uint32_t BURST_BEATS  = 256;   // 2 KiB / 8 B
 static const uint32_t BURST_LEN    = 255;   // -> 256 beats
 // The per-DMA read->write coupling-FIFO depth is no longer a file constant:
 // it is Tb::couple_depth (default 3), set from +couple-depth=3|512 in main().
-static const uint32_t WARM_ROUNDS  = 16;
-static const uint32_t JOB_COUNT    = 64;
-
-// Last-progress watchdog: 4096 consecutive cycles with no upstream / completion
-// handshake is treated as a real stall (a known stall no longer hides inside a
-// 4-million-cycle budget).
-static const long WATCHDOG_CYCLES = 4096L;
-// Ultimate backstop only; the watchdog normally fires long before this.
-static const long SCENARIO_BUDGET = 4000000L;
-static const long WARM_BUDGET     = 200000L;
+static const uint32_t WARM_ROUNDS  = 16;   // not scaled with the job count
+
+// Job count per DMA.  64 is the historical default (every existing log); 4096
+// is the bounded long continuous-work run.  Set from +job-count=64|4096.
+static const uint32_t JOB_COUNT_DEFAULT = 64;
+
+// Last-progress watchdog: 4096 consecutive cycles with no upstream / completion
+// handshake is treated as a real stall.
+static const long WATCHDOG_CYCLES = 4096L;
+// Largest accepted +v-gap-cycles, strictly inside the watchdog window so a
+// planned V inter-job gap is never mistaken for a stall.
+static const uint32_t V_GAP_MAX = 4000;
+static_assert(V_GAP_MAX < (uint32_t)WATCHDOG_CYCLES,
+              "planned V gap must stay inside the watchdog window");
+
+// Ultimate backstop only; the watchdog normally fires long before this.  Now
+// DERIVED from the job count and the V gap: keep the historical per-job
+// allowance (4_000_000 / 64 == 62_500 cycles per job), add the total planned V
+// gap, and add a fixed warm-up + drain margin.  Finite for both job counts.
+static const long SCENARIO_BUDGET_PER_JOB = 62500L;
+static const long SCENARIO_BUDGET_WARM    = 400000L;
+static const long WARM_BUDGET             = 200000L;
+
+// Per-scenario backstop derived from the two run knobs (upper bound only; the
+// per-job watchdog is what detects a real stall).
+static long scenario_budget(uint32_t job_count, uint32_t v_gap_cycles) {
+    return SCENARIO_BUDGET_PER_JOB * (long)job_count
+         + (long)v_gap_cycles * (long)job_count
+         + SCENARIO_BUDGET_WARM;
+}
 
 // ---------------------------------------------------------------------------
 // identifiers / addresses / sizes
@@ -239,7 +239,11 @@
     long scenario_start  = 0;
     uint32_t ptw_latency = 1;
     uint32_t a_pages_    = 0;
+    // Job count per DMA (V and A each issue this many 4 KiB jobs).  64 keeps the
+    // historical 12-row run; 4096 is the bounded long run.  Set from
+    // +job-count=64|4096 in main(); every V/A job loop uses this value.
+    uint32_t job_count = JOB_COUNT_DEFAULT;
 
     // Upstream request-priority order for the two DMA clients (see +order= in
     // main()): 0 = V (dma[0]) first, 1 = A (dma[1]) first.
     // Defaults select the matching-source order (both V-first).
@@ -1127,9 +1127,10 @@
     void run_scenario(const char* name, bool enable, uint32_t a_pages,
                       uint32_t ptw_lat) {
         std::printf("== coupled scenario %s (runtime enable=%d, a_pages=%u, "
-                    "ptw_latency=%u, v_gap_cycles=%u, policy_partition=%d) ==\n",
-                    name, (int)enable, a_pages, ptw_lat, v_gap_cycles,
-                    (int)policy_partition);
+                    "ptw_latency=%u, jobs=%u, v_gap_cycles=%u, "
+                    "policy_partition=%d) ==\n",
+                    name, (int)enable, a_pages, ptw_lat, job_count,
+                    v_gap_cycles, (int)policy_partition);
         setup_memory(a_pages);
         reset(enable);
         ptw_latency    = ptw_lat;
@@ -1145,3 +1145,6 @@
         for (uint32_t r = 0; r < WARM_ROUNDS; ++r) warm_round(a_pages);
 
+        // Bounded per-scenario budget; scales with the job count and V gap.
+        const long budget = scenario_budget(job_count, v_gap_cycles);
+
         uint32_t v_next = 0, a_next = 0;
@@ -1155,12 +1155,17 @@
         if (a_pages) start_job(1, a_next++);
 
-        while (v_next < JOB_COUNT || (a_pages && a_next < JOB_COUNT)) {
+        while (v_next < job_count || (a_pages && a_next < job_count)) {
             tick();
-            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
+            // A pending planned V gap is a deliberate idle: widen the progress
+            // window by that bounded gap so a planned gap is never reported as
+            // a stall, while a real stall is still caught.
+            long idle_limit = WATCHDOG_CYCLES;
+            if (v_ready_at >= 0) idle_limit += (long)v_gap_cycles;
+            if (g_cycle - last_progress_cycle > idle_limit) {
                 dump_stall(std::string("scenario ") + name);
                 throw TestFailure("watchdog: no handshake progress");
             }
-            if (dma[0].job_done && v_next < JOB_COUNT) {
+            if (dma[0].job_done && v_next < job_count) {
                 if (v_gap_cycles == 0) {
                     start_job(0, v_next++);
                     v_ready_at = -1;
@@ -1173,6 +1173,6 @@
             }
-            if (a_pages && dma[1].job_done && a_next < JOB_COUNT) start_job(1, a_next++);
-            if (g_cycle - scenario_start > SCENARIO_BUDGET) {
+            if (a_pages && dma[1].job_done && a_next < job_count) start_job(1, a_next++);
+            if (g_cycle - scenario_start > budget) {
                 dump_stall(std::string("scenario ") + name);
                 throw TestFailure("scenario budget exceeded");
             }
@@ -1184,6 +1184,6 @@
                 throw TestFailure("watchdog: no handshake progress during drain");
             }
-            if (g_cycle - scenario_start > SCENARIO_BUDGET) {
+            if (g_cycle - scenario_start > budget) {
                 dump_stall(std::string("scenario-drain ") + name);
                 throw TestFailure("scenario drain budget exceeded");
             }
@@ -1229,9 +1229,9 @@
         }
-        std::printf("COUPLED-SUMMARY %s couple_depth=%u cycles=%ld comp_ar=%lu "
+        std::printf("COUPLED-SUMMARY %s couple_depth=%u jobs=%u cycles=%ld comp_ar=%lu "
                     "ddtw_ar=%lu ptw_ar=%lu tl_accept=%lu tl_resp=%lu "
                     "iotlb_look=%lu iotlb_hit=%lu ptw_active=%lu r_beats=%lu b_up=%lu "
                     "nonblock_fire=%lu nonblock_busy=%lu "
                     "v_gap=%u policy=%d\n",
-                    name, couple_depth, g_cycle - scenario_start,
+                    name, couple_depth, job_count, g_cycle - scenario_start,
                     (unsigned long)n_comp_ar, (unsigned long)n_ddtw_ar,
                     (unsigned long)n_ptw_ar, (unsigned long)n_tl_accept,
@@ -1250,18 +1250,24 @@
 
     // (runtime enable, aggressor pages, ptw latency)
     const struct { const char* name; bool en; uint32_t a; uint32_t lat; } plan[] = {
-        { "mode1-solo-l1",  false, 0,  1 },
-        { "mode1-solo-l31", false, 0,  31 },
-        { "mode1-a1-l1",    false, 1,  1 },
-        { "mode1-a1-l31",   false, 1,  31 },
-        { "mode1-a8-l1",    false, 8,  1 },
-        { "mode1-a8-l31",   false, 8,  31 },
-        { "mode2-solo-l1",  true,  0,  1 },
-        { "mode2-solo-l31", true,  0,  31 },
-        { "mode2-a1-l1",    true,  1,  1 },
-        { "mode2-a1-l31",   true,  1,  31 },
-        { "mode2-a8-l1",    true,  8,  1 },
-        { "mode2-a8-l31",   true,  8,  31 },
-    };
+    // (runtime enable, aggressor pages, ptw latency, opt-in)
+    // The first 12 rows are the historical default plan; the four a16 rows run
+    // only when named with +scenario=<name>.
+    const struct { const char* name; bool en; uint32_t a; uint32_t lat; bool opt_in; } plan[] = {
+        { "mode1-solo-l1",  false, 0,  1,  false },
+        { "mode1-solo-l31", false, 0,  31, false },
+        { "mode1-a1-l1",    false, 1,  1,  false },
+        { "mode1-a1-l31",   false, 1,  31, false },
+        { "mode1-a8-l1",    false, 8,  1,  false },
+        { "mode1-a8-l31",   false, 8,  31, false },
+        { "mode1-a16-l1",   false, 16, 1,  true  },
+        { "mode1-a16-l31",  false, 16, 31, true  },
+        { "mode2-solo-l1",  true,  0,  1,  false },
+        { "mode2-solo-l31", true,  0,  31, false },
+        { "mode2-a1-l1",    true,  1,  1,  false },
+        { "mode2-a1-l31",   true,  1,  31, false },
+        { "mode2-a8-l1",    true,  8,  1,  false },
+        { "mode2-a8-l31",   true,  8,  31, false },
+        { "mode2-a16-l1",   true,  16, 1,  true  },
+        { "mode2-a16-l31",  true,  16, 31, true  },
+    };
 
     // optional single-scenario selection: +scenario=<name>
@@ -1291,6 +1291,7 @@
     std::string couple_depth_str = "3";
     std::string v_gap_str = "0";
     std::string policy_str = "0";
+    std::string job_count_str = "64";
     bool overlap = false;
     bool fullbuffer_permit = false;
     for (int i = 1; i < argc; ++i) {
@@ -1306,5 +1306,7 @@
         const std::string pkey = "+policy=";
         if (a.rfind(pkey, 0) == 0) policy_str = a.substr(pkey.size());
+        const std::string jkey = "+job-count=";
+        if (a.rfind(jkey, 0) == 0) job_count_str = a.substr(jkey.size());
         if (a == "--overlap") overlap = true;
         if (a == "--fullbuffer-permit") fullbuffer_permit = true;
     }
@@ -1344,6 +1344,17 @@
                              policy_str.c_str());
         return 2;
     }
+    // Strict job-count validation: exactly the two known values (same style as
+    // the depth/gap/policy knobs).
+    uint32_t job_count = 0;
+    if (job_count_str == "64")        job_count = 64;
+    else if (job_count_str == "4096") job_count = 4096;
+    else {
+        std::fprintf(stderr, "nonblock_coupled_test: invalid +job-count='%s' "
+                             "(expected exactly 64 or 4096)\n",
+                             job_count_str.c_str());
+        return 2;
+    }
     int order_ar = 0, order_aw = 0;   // 0 = V first, 1 = A first
     bool order_rr = false;
     if (order == "aa")      { order_ar = 1; order_aw = 1; }
@@ -1366,6 +1366,7 @@
     std::printf("nonblock_coupled_test: fullbuffer_permit=%d\n",
                 (int)fullbuffer_permit);
     std::printf("nonblock_coupled_test: v_gap_cycles=%u\n", v_gap_cycles);
+    std::printf("nonblock_coupled_test: job_count=%u\n", job_count);
     std::printf("nonblock_coupled_test: policy_partition=%d "
                 "(wrapper PolicyEnable is an elaboration flag; see --policy)\n",
                 (int)policy_partition);
@@ -1374,13 +1374,20 @@
     for (const auto& s : plan) {
-        if (!only.empty() && only != s.name) continue;
+        if (!only.empty()) {
+            // Explicit +scenario selects exactly that row (a16 rows included).
+            if (only != s.name) continue;
+        } else if (s.opt_in) {
+            // No +scenario: the historical 12 rows run; the a16 rows are opt-in.
+            continue;
+        }
         ++ran;
         try {
             Tb tb;
             tb.order_ar_prefer = order_ar;
             tb.order_aw_prefer = order_aw;
             tb.rr_order        = order_rr;
             tb.overlap_mode    = overlap;
             tb.couple_depth    = couple_depth;
             tb.fullbuffer_permit = fullbuffer_permit;
             tb.v_gap_cycles      = v_gap_cycles;
             tb.policy_partition  = policy_partition;
+            tb.job_count         = job_count;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
```

---

## Patch 2 — `tests/test_nonblock_coupled.py`

```diff
diff --git a/tests/test_nonblock_coupled.py b/tests/test_nonblock_coupled.py
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -17,8 +17,14 @@
 Legacy control, v2 depth:
     python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr --couple-depth 512
 
 Opt-in V inter-job gap and IOTLB partition policy (elaborated with
 PolicyEnable=1, built in a separate obj dir):
     python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
         --fullbuffer-permit --v-gap-cycles 1000 --policy
+
+Long continuous work (4096 jobs per DMA) with an opt-in 16-page aggressor row.
+--scenario is required to select an a16 row; without it the default 12 rows run.
+Runtime flags only: same RTL, same obj directories:
+    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
+        --job-count 4096 --scenario mode2-a16-l31
 """
@@ -116,6 +116,14 @@
                          "back-to-back issue).  The wait is counted in tick() and "
                          "never gates A and never changes any IOVA/PA or page "
                          "stride.")
+    ap.add_argument("--job-count", type=int, choices=(64, 4096), default=64,
+                    help="jobs per DMA, forwarded to the C++ harness as "
+                         "+job-count=<...> (default: 64).  64 keeps the "
+                         "historical 12-scenario run; 4096 is the bounded long "
+                         "continuous-work run.  Bounds are fixed (64 or 4096). "
+                         "Runtime flag only: it never changes the obj dir; the "
+                         "warm-up length is not scaled with it and makes no "
+                         "board-warm256 claim.")
     ap.add_argument("--policy", action="store_true",
                     help="opt-in IOTLB replacement partition.  Elaborates the "
                          "test wrapper with -GPolicyEnable=1 (a compile "
@@ -127,6 +127,9 @@
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
     ap.add_argument("--scenario", default=None,
-                    help="run only the named scenario "
-                         "(forwarded to the C++ harness as +scenario=)")
+                    help="run only the named scenario, forwarded to the C++ "
+                         "harness as +scenario=.  Without it the historical 12 "
+                         "rows run; the opt-in 16-page aggressor rows "
+                         "(mode1-a16-l1, mode1-a16-l31, mode2-a16-l1, "
+                         "mode2-a16-l31) run ONLY when named here.")
     opts = ap.parse_args()
@@ -207,4 +207,5 @@
     run_args = [str(exe), "+order={}".format(opts.order),
-                "+couple-depth={}".format(opts.couple_depth)]
+                "+couple-depth={}".format(opts.couple_depth),
+                "+job-count={}".format(opts.job_count)]
     run_args.append("+v-gap-cycles={}".format(opts.v_gap_cycles))
     run_args.append("+policy={}".format(1 if opts.policy else 0))
@@ -215,9 +215,9 @@
     if opts.fullbuffer_permit:
         run_args.append("--fullbuffer-permit")
-    print("test_nonblock_coupled: couple_depth={} order={} overlap={} "
-          "fullbuffer_permit={} v_gap_cycles={} policy={}".format(
-              opts.couple_depth, opts.order, int(opts.overlap),
-              int(opts.fullbuffer_permit), opts.v_gap_cycles,
-              int(opts.policy)))
+    print("test_nonblock_coupled: job_count={} couple_depth={} order={} "
+          "overlap={} fullbuffer_permit={} v_gap_cycles={} policy={}".format(
+              opts.job_count, opts.couple_depth, opts.order,
+              int(opts.overlap), int(opts.fullbuffer_permit),
+              opts.v_gap_cycles, int(opts.policy)))
     run = subprocess.run(run_args)
     return run.returncode
```

---

## Semantics preserved / changed

Preserved (no behavior change with default flags):
- Default plan still runs the same 12 rows in the same order, 64 jobs each, `couple_depth` default 3, gap 0, policy 0, `fullbuffer_jobs_i` 0 unless `--fullbuffer-permit`, overlap off. The extra `opt_in` field only gates rows when `+scenario` is absent.
- `WATCHDOG_CYCLES` stays 4096 (used by `warm_page`, `check_quiet`, both scenario loops); `WARM_ROUNDS` stays 16; `WARM_BUDGET` stays 200000.
- `SCENARIO_BUDGET` 4 000 000 is exactly reproduced for the default run: `62500*64 + 0 + 400000 = 4 400 000` (slightly *looser* than before, so no previously passing 64-job run can now trip the budget; the watchdog is unchanged and is the real stall detector).

Changed (opt-in only):
- `+job-count=64|4096` (Python `--job-count`, `choices=(64, 4096)`), strictly validated; `Tb::job_count` drives every V/A job loop and is echoed in the scenario header and `COUPLED-SUMMARY`.
- Budget scales: `budget = 62500*job_count + v_gap_cycles*job_count + 400000` (finite: 4096 jobs → 256 M, 4096 jobs + gap 4000 → ≈272.8 M).
- V-gap idle is never reported as a stall: the progress window is widened by `v_gap_cycles` only while `v_ready_at >= 0`. `static_assert(V_GAP_MAX < WATCHDOG_CYCLES)` documents that a bare gap already fits.
- a16 rows exist (opt-in, `--scenario mode1-a16-l1 | mode1-a16-l31 | mode2-a16-l1 | mode2-a16-l31`), 16 aggressor pages per direction.

## Assumptions / things to check

1. Blank context lines in both patches are a single space; verify before applying.
2. Apply from the repo root (`git apply --check --recount --unidiff-zero <file>`); paths are `a/tests/... b/tests/...`.
3. a16 feasibility by inspection: with `a_pages=16`, `setup_memory` writes `2*16 = 32` L0A entries at `0x70000..0x700F8` (no overlap with DDT `0x10000`, roots `0x20000/0x30000`, L1V `0x40000`, L0V `0x50000`, L1A `0x60000`), and uses `page_word(1+p)` for p ≤ 15 (no collision with V page 0). Not executed here.
4. Warm-up is deliberately *not* scaled and makes no board-warm256 claim; `WARM_ROUNDS` stays 16 because on the board warm256 is 256 *transfers*, not 256 pages.
5. Runtime: 4096 jobs × 2 DMAs × 4 KiB is long; expect the scaled budget to be exercised, not the watchdog.

## Suggested acceptance (Codex executes)

```
git apply --check --recount --unidiff-zero <cpp.patch>
git apply --check --recount --unidiff-zero <py.patch>
python3 -m py_compile tests/test_nonblock_coupled.py
# default behavior unchanged (12 rows, 64 jobs)
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512
# opt-in a16 row
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --scenario mode2-a16-l31
# long run + gap (V_GAP/IOVA/PA untouched), N-mode2-a8-l31
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --scenario mode2-a8-l31 --v-gap-cycles 1000 --job-count 4096
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --scenario mode2-a8-l31 --v-gap-cycles 4000 --job-count 4096
# strict knob rejection
python3 tests/test_nonblock_coupled.py --job-count 100   # argparse error
./Vnonblock_banked_top +job-count=7                      # exit 2
```

No tests were executed by me; the patches above are proposals to be applied and verified by the parent.