# Bounded revision — A16 opt-in rows + `--job-count 64|4096`

Scope is exactly the two files. No new SV, no new adapter, no obj-dir change (`+job-count` and `+scenario` are runtime-only, like `+couple-depth`/`+v-gap-cycles`/`+policy`).

## Assumptions

1. **A16 page-table footprint fits the existing harness layout.** `setup_memory(16)` needs `2*a_pages = 32` L0A entries at `0x70000` (one 4 KiB PT holds 512 → fine), and one L1A leaf at `0x60800` (L1 index `(0x20000000>>21)&0x1FF == 256 == (0x60800-0x60000)/8`) covers the whole aggressor span `0x20000000..0x2001FFFF` (src 16 pages + dst 16 pages, still inside one 2 MiB block). Not executed here — Codex must confirm a real PASS.
2. **Budget model.** The historical `4_000_000` backstop covered the 64-job default (V plus A, both 64 jobs); `4_000_000 / 64 = 62_500` is kept as a per-job allowance and scaled linearly, plus `job_count * v_gap`, plus warm/drain margin. Per-job steady-state cost is assumed not to grow with run length. The watchdog remains the real stall detector.
3. **Warm-up is not scaled** and makes **no** board-warm256 equivalence claim; the long run covers only long continuous work plus the policy/gap knobs.
4. `+job-count` is strictly validated to `64|4096`; `--scenario` stays free-form (unknown names already yield `no scenario matched` + exit 2).
5. I did **not** execute anything; no new PASS is claimed.

---

## Diff — `tests/nonblock_coupled_test.cpp`

```diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -63,6 +63,24 @@
 // coupling-FIFO overflow CHECK (the f_up_r update), so a run can never
 // back-pressure at one depth and check another.
 //
+// Scenario selection (opt-in rows)
+// --------------------------------
+// The default plan is the historical 12 scenarios at 64 jobs each.  Four
+// additional opt-in rows carry the 16-page aggressor (mode1-a16-l1,
+// mode1-a16-l31, mode2-a16-l1, mode2-a16-l31).  They run ONLY when named with
+// +scenario=<name> (Python --scenario); with no +scenario they are skipped, so
+// the default run is unchanged.
+//
+// Job count (opt-in, +job-count=64|4096)
+// --------------------------------------
+// 64 is the historical default and keeps every existing log valid; 4096 is the
+// bounded long CONTINUOUS-work run.  The value is validated to exactly those two
+// boundaries and drives every V/A job loop.  The scenario budget is derived from
+// job_count and the V gap (scenario_budget()); the per-job watchdog is unchanged
+// and a planned V gap is never reported as a stall.  Warm-up is deliberately NOT
+// scaled with the job count and does NOT claim board-warm256 equivalence: this
+// run only covers long continuous work plus the policy/gap knobs.
+//
 // This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
 // it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no
 // ID remap.  It only reproduces the ordering/back-pressure shape of the board.
@@ -104,18 +104,51 @@
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
+//
+// Warm-up is deliberately NOT scaled with the job count.  The board warms 256
+// pages (warm256); this harness keeps its historical 16 rounds and does NOT
+// claim warm256 equivalence.  The long run only covers long CONTINUOUS work
+// plus the existing policy/gap knobs; it is not a warm-up port.
+static const uint32_t WARM_ROUNDS  = 16;
+
+// Job count is now selectable, but only between the two known values.  The
+// default 64 keeps every existing 12-scenario / 64-job log byte-comparable; 4096
+// is the bounded long continuous-work run.  Every V/A job loop uses this value
+// (Tb::job_count, set from +job-count=64|4096 in main()).
+static const uint32_t JOB_COUNT_DEFAULT = 64;
+
+// Last-progress watchdog: 4096 consecutive cycles with no upstream / completion
+// handshake is treated as a real stall.  A pending planned V inter-job gap is a
+// deliberate idle; run_scenario() widens the window by that bounded gap so a
+// known gap is never reported as a stall, while a real stall is still caught.
+static const long WATCHDOG_CYCLES = 4096L;
+// Largest accepted +v-gap-cycles.  Kept strictly inside the watchdog window so a
+// planned gap can never be mistaken for a stall even before the widening above.
+static const uint32_t V_GAP_MAX = 4000;
+static_assert(V_GAP_MAX < (uint32_t)WATCHDOG_CYCLES,
+              "planned V gap must stay inside the watchdog window");
+
+// Ultimate backstop only; the watchdog normally fires long before this.  It is
+// now DERIVED from the job count and the V gap instead of a fixed 4e6:
+//   * 4_000_000 was a safe backstop for the 64-job default (V plus A, both
+//     issuing 64 4 KiB jobs through one ordered R-return queue), i.e. 62_500
+//     cycles per job -- keep that per-job allowance and scale it linearly;
+//   * add the total planned V inter-job gap (job_count * v_gap_cycles);
+//   * add the warm-up budget plus a small drain/quiet margin.
+// Bounded: the 4096-job run gets about 272 M cycles, still finite.
+static const long SCENARIO_BUDGET_PER_JOB = 62500L;  // 64 * 62500 == 4_000_000
+static const long SCENARIO_BUDGET_WARM    = 400000L; // warm-up + drain margin
+static const long WARM_BUDGET             = 200000L;
+
+// Per-scenario backstop derived from the two run knobs.  It only has to be an
+// upper bound; the watchdog is what detects a real stall.
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
+    // Job count per DMA (V and A each issue this many 4 KiB jobs).  Default 64
+    // keeps the historical 12-scenario run; 4096 is the bounded long run.  Set
+    // from +job-count=64|4096 in main(); every V/A job loop uses this value.
+    uint32_t job_count   = JOB_COUNT_DEFAULT;
 
     // Upstream request-priority order for the two DMA clients (see +order= in
     // main()): 0 = V (dma[0]) first, 1 = A (dma[1]) first.
     // Defaults select the matching-source order (both V-first).
@@ -1127,8 +1127,9 @@
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
@@ -1145,48 +1145,60 @@
         for (uint32_t r = 0; r < WARM_ROUNDS; ++r) warm_round(a_pages);
 
+        // Derived bounded budget for THIS scenario: it scales with the job count
+        // and the planned V gap (see scenario_budget()).  The watchdog below is
+        // unchanged and still catches a real no-progress stall.
+        const long budget = scenario_budget(job_count, v_gap_cycles);
+
         uint32_t v_next = 0, a_next = 0;
         // Optional V-only inter-job gap.  After a V 4 KiB job completes, the
         // next V job is only started once v_gap_cycles complete ticks have
         // elapsed.  A is never gated here: the A start below is unchanged and
         // tick() keeps running, so A keeps progressing while V is idle.  The
         // gap is purely temporal and never touches any IOVA/PA or page stride.
         long v_ready_at = -1;
         start_job(0, v_next++);
         if (a_pages) start_job(1, a_next++);
 
-        while (v_next < JOB_COUNT || (a_pages && a_next < JOB_COUNT)) {
+        while (v_next < job_count || (a_pages && a_next < job_count)) {
             tick();
-            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
+            // A pending planned V gap is a deliberate idle: widen the progress
+            // window by that bounded gap so it is never reported as a stall,
+            // while a real stall (no handshake for the gap plus the full watchdog
+            // window) is still caught.
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
                 } else if (v_ready_at < 0) {
                     v_ready_at = g_cycle + (long)v_gap_cycles;
                 } else if (g_cycle >= v_ready_at) {
                     start_job(0, v_next++);
                     v_ready_at = -1;
                 }
             }
-            if (a_pages && dma[1].job_done && a_next < JOB_COUNT) start_job(1, a_next++);
-            if (g_cycle - scenario_start > SCENARIO_BUDGET) {
+            if (a_pages && dma[1].job_done && a_next < job_count) start_job(1, a_next++);
+            if (g_cycle - scenario_start > budget) {
                 dump_stall(std::string("scenario ") + name);
                 throw TestFailure("scenario budget exceeded");
             }
         }
         while (!dma[0].job_done || (a_pages && !dma[1].job_done)) {
             tick();
             if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                 dump_stall(std::string("scenario-drain ") + name);
                 throw TestFailure("watchdog: no handshake progress during drain");
             }
-            if (g_cycle - scenario_start > SCENARIO_BUDGET) {
+            if (g_cycle - scenario_start > budget) {
                 dump_stall(std::string("scenario-drain ") + name);
                 throw TestFailure("scenario drain budget exceeded");
             }
         }
@@ -1230,6 +1230,6 @@
         // event.
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
@@ -1251,15 +1251,21 @@
     Verilated::commandArgs(argc, argv);
 
-    // (runtime enable, aggressor pages, ptw latency)
-    const struct { const char* name; bool en; uint32_t a; uint32_t lat; } plan[] = {
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
+    // (runtime enable, aggressor pages, ptw latency, opt-in)
+    // The first 12 entries are the historical default plan.  The four a16 rows
+    // are opt-in: they only run when named with +scenario=<name>.
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
     };
 
     // optional single-scenario selection: +scenario=<name>
@@ -1289,22 +1289,25 @@
     std::string only;
     std::string order = "vv";
     std::string couple_depth_str = "3";
     std::string v_gap_str = "0";
     std::string policy_str = "0";
+    std::string job_count_str = "64";
     bool overlap = false;
     bool fullbuffer_permit = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
         if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
         const std::string okey = "+order=";
         if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
         const std::string ckey = "+couple-depth=";
         if (a.rfind(ckey, 0) == 0) couple_depth_str = a.substr(ckey.size());
         const std::string gkey = "+v-gap-cycles=";
         if (a.rfind(gkey, 0) == 0) v_gap_str = a.substr(gkey.size());
         const std::string pkey = "+policy=";
         if (a.rfind(pkey, 0) == 0) policy_str = a.substr(pkey.size());
+        const std::string jkey = "+job-count=";
+        if (a.rfind(jkey, 0) == 0) job_count_str = a.substr(jkey.size());
         if (a == "--overlap") overlap = true;
         if (a == "--fullbuffer-permit") fullbuffer_permit = true;
     }
@@ -1343,6 +1343,18 @@
                              "(expected exactly 0 or 1)\n", policy_str.c_str());
         return 2;
     }
+    // Strict job-count validation: exactly the two known values, same style as
+    // the depth/gap/policy knobs.  A free-form integer would let a typo silently
+    // become the workload length.
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
@@ -1370,27 +1370,35 @@
                 (int)policy_partition);
 
     int ran = 0, failed = 0;
     for (const auto& s : plan) {
-        if (!only.empty() && only != s.name) continue;
+        if (!only.empty()) {
+            // Explicit +scenario selects exactly that row (a16 rows included).
+            if (only != s.name) continue;
+        } else if (s.opt_in) {
+            // No +scenario: the historical 12 rows run; the a16 rows are opt-in
+            // and only run when named explicitly.
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
         } catch (const std::exception& e) {
             ++failed;
             std::fprintf(stderr, "nonblock_coupled_test: SCENARIO-FAIL %s: %s\n",
                          s.name, e.what());
         }
     }
 
     if (ran == 0) {
```

---

## Diff — `tests/test_nonblock_coupled.py`

```diff
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
+Long continuous work (4096 jobs per DMA) with the opt-in 16-page aggressor row.
+--scenario is required to select an a16 row; without it the default 12 rows run.
+Runtime flags only: same RTL, same obj directories:
+    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
+        --job-count 4096 --scenario mode2-a16-l31
 """
@@ -116,6 +116,14 @@
                          "+v-gap-cycles=<...> (default: 0 = historical "
                          "back-to-back issue).  The wait is counted in tick() and "
                          "never gates A and never changes any IOVA/PA or page "
                          "stride.")
+    ap.add_argument("--job-count", type=int, choices=(64, 4096), default=64,
+                    help="jobs per DMA, forwarded to the C++ harness as "
+                         "+job-count=<...> (default: 64).  64 keeps the "
+                         "historical 12-scenario run; 4096 is the bounded long "
+                         "continuous-work run.  Bounds are fixed (64 or 4096). "
+                         "Runtime flag only: it never changes the obj dir; the "
+                         "warm-up length is not scaled with it and does not "
+                         "claim board-warm256 equivalence.")
     ap.add_argument("--policy", action="store_true",
                     help="opt-in IOTLB replacement partition.  Elaborates the "
                          "test wrapper with -GPolicyEnable=1 (a compile "
@@ -127,7 +127,10 @@
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
 
@@ -204,10 +204,11 @@
         sys.stderr.write("verilator did not produce {}\n".format(exe))
         return 1
 
     run_args = [str(exe), "+order={}".format(opts.order),
-                "+couple-depth={}".format(opts.couple_depth)]
+                "+couple-depth={}".format(opts.couple_depth),
+                "+job-count={}".format(opts.job_count)]
     run_args.append("+v-gap-cycles={}".format(opts.v_gap_cycles))
     run_args.append("+policy={}".format(1 if opts.policy else 0))
     if opts.scenario:
         run_args.append("+scenario={}".format(opts.scenario))
     if opts.overlap:
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
+              opts.job_count, opts.couple_depth, opts.order, int(opts.overlap),
+              int(opts.fullbuffer_permit), opts.v_gap_cycles,
+              int(opts.policy)))
     run = subprocess.run(run_args)
     return run.returncode
```

---

## Precise commands (Codex to run / verify)

Default run — must still be exactly the 12 historical rows at 64 jobs:
```
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512
# expect: "nonblock_coupled_test: job_count=64", COUPLED-SUMMARY lines carry jobs=64
# expect: "nonblock_coupled_test: PASS (12 scenarios)"
```

A16 selection (one scenario each):
```
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 --scenario mode1-a16-l1
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 --scenario mode2-a16-l31
# expect: "PASS (1 scenarios)"; COUPLED-SUMMARY ... a_pages=16 ... jobs=64
```

Default must skip A16 (direct C++ check, no `+scenario`):
```
./out/nonblock-coupled/banked/board/overlap/obj/Vnonblock_banked_top
# expect: PASS (12 scenarios)  -- a16 rows NOT present
./out/nonblock-coupled/banked/board/overlap/obj/Vnonblock_banked_top +scenario=mode1-a16-l31
# expect: PASS (1 scenarios)
```

Long 4096-job run (board, no policy):
```
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 --job-count 4096
```

Long + policy + gap + fullbuffer + A16:
```
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --v-gap-cycles 1000 --policy --job-count 4096 --scenario mode2-a16-l1
```

Strict-bound rejections (both layers):
```
python3 tests/test_nonblock_coupled.py --job-count 100        # argparse rejects (returncode 2)
./out/nonblock-coupled/banked/board/overlap/obj/Vnonblock_banked_top +job-count=100
# expect: "invalid +job-count='100' (expected exactly 64 or 4096)" + returncode 2
```

## Acceptance checks

1. No `+scenario` ⇒ stdout `PASS (12 scenarios)` and each `COUPLED-SUMMARY` shows `jobs=64`; the new a16 rows never appear.
2. `+scenario=mode{1,2}-a16-l{1,31}` ⇒ exactly one scenario, `PASS (1 scenarios)`, `a_pages=16` in the header. (PASS not asserted here — must be observed.)
3. `+job-count=4096` (no `+scenario`) ⇒ `PASS (12 scenarios)`, `jobs=4096` in every header/SUMMARY, every scenario completes within the derived budget.
4. `+job-count` outside `{64,4096}` ⇒ exit 2, no scenario run.
5. Watchdog still fires on a genuine deadlock (e.g. temporarily force a stall) — a planned `+v-gap-cycles=4000` with `a_pages=0` must **not** trip it (widened `idle_limit`).

## Unresolved / watch-outs

- **A16 correctness not verified:** if an A16 scenario fails byte-compare, suspect the L1A/L0A mapping (`0x60800` / `0x70000`) or the page-table size, not the new control flow. The index arithmetic above argues it fits, but this is analysis, not execution.
- **Budget constant** `62_500 cycles/job` is inherited from the historical `4_000_000/64`. If long-run steady state is materially slower per job, raise `SCENARIO_BUDGET_PER_JOB`; the watchdog, not the budget, is the true stall detector.
- Warm-up stays at `WARM_ROUNDS=16` and is intentionally not a warm256 port; no claim of board warm equivalence is made.