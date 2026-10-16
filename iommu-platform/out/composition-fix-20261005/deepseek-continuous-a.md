# Findings

I traced both files and the actual run loop. The change is a bounded, opt-in addition: a new C++ flag `continuous_aggressor` (default `false`) that, when set **and** the scenario has an A aggressor (`a_pages > 0`), makes the A DMA re-issue 4 KiB jobs (cycling the existing `a_pages` `idx % a_pages_` pages) as soon as its previous job finishes, and stops issuing once V's **final** job has truly completed (`v_next >= job_count && dma[0].job_done`). The already-active A job is drained. Nothing about V's job count, warm-up, byte verification, page tables, or DUT wiring changes.

Key structural points I relied on (with citations into the supplied files):

- The current run loop already serializes jobs: it launches the next job only when the current `dma[].job_done` (`nonblock_coupled_test.cpp:1196-1207`), and the drain loop `while (!dma[0].job_done || (a_pages && !dma[1].job_done))` finishes in-flight work (`:1213-1223`).
- A job page selection is `idx % a_pages_` inside `start_job` (`:952-958`), so an unbounded `a_next` keeps cycling the same pages — exactly the requirement.
- V's `v_next` reaches `job_count` only after the last V job is launched (`:1182`, `:1198/:1203`), so `v_next >= job_count && dma[0].job_done` is the correct "V final job actually completed" predicate.
- The watchdog (`WATCHDOG_CYCLES=4096`) and the derived backstop `scenario_budget()` (`:128-138`) stay finite; the continuous run is bounded by V's completion, so it is naturally capped (V is the slow side with `v_gap_cycles`).

I converted the main loop's termination into an explicit predicate that keeps **all** exit conditions: it continues while V's final job is still active, and while the last A job is still active, then exits. No unbounded spin is introduced (V always completes after `job_count` jobs).

To avoid any change to the historical 12-row output, the new accounting/print is gated behind the opt-in flag.

---

## Proposed changes

### tests/nonblock_coupled_test.cpp

```diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -293,7 +293,17 @@
     // start condition is unchanged and tick() keeps running during the wait.
     // The wait is purely temporal; no IOVA/PA or page distance is changed.
     uint32_t v_gap_cycles = 0;
 
+    // --continuous-aggressor: opt-in.  When set (and the scenario has an A
+    // aggressor, i.e. a_pages > 0) the A DMA launches each next 4 KiB job as
+    // soon as its previous job completes, cycling the existing a_pages
+    // source/destination pages, and keeps doing so until the V victim has
+    // actually completed its FINAL job (v_next == job_count &&
+    // dma[0].job_done).  At that point no new A job is launched and the
+    // already-active A job is drained.  Default false -> byte-for-byte
+    // identical to the historical fixed job_count run.
+    bool continuous_aggressor = false;
+
     // +policy=0|1: forwarded verbatim to the wrapper input policy_partition_i.
     // Only observable in a build elaborated with PolicyEnable=1 (the Python
     // --policy build); the default build forces the full replacement mask.
@@ -326,7 +336,14 @@
     // already exposes (nonblock_banked_top.sv:150-151 nonblock_fire_o /
     // nonblock_busy_o); the top exposes no candidate/probe counter.
     uint64_t n_nonblock_fire = 0, n_nonblock_busy = 0;
 
+    // --continuous-aggressor accounting.  a_issued counts every 4 KiB A job
+    // launched; a_completed counts every A job observed finishing (A jobs are
+    // serial, so this is an edge count).  Both stay at the plain job_count for
+    // the default non-continuous runs.
+    uint64_t a_issued = 0, a_completed = 0;
+    bool     a_done_prev = false;
+
     // warm-up plumbing
     bool     warm_active = false;
     bool     warm_sent   = false;
@@ -1121,5 +1138,7 @@
         n_ptw_active = n_iotlb_lookup = n_iotlb_hit = 0;
         n_r_beats = n_b_up = 0;
         n_nonblock_fire = n_nonblock_busy = 0;
+        a_issued = a_completed = 0;
+        a_done_prev = false;
 
         for (int i = 0; i < 8; ++i) tick();
@@ -1179,11 +1198,30 @@
         // tick() keeps running, so A keeps progressing while V is idle.  The
         // gap is purely temporal and never touches any IOVA/PA or page stride.
         long v_ready_at = -1;
+        // --continuous-aggressor is only meaningful with an A aggressor
+        // present; solo scenarios keep the exact historical behaviour.
+        const bool continuous = continuous_aggressor && a_pages > 0;
         start_job(0, v_next++);
-        if (a_pages) start_job(1, a_next++);
+        if (a_pages) { start_job(1, a_next++); ++a_issued; }
 
-        while (v_next < job_count || (a_pages && a_next < job_count)) {
+        // Exit condition.  Default: stop launching once both DMAs have issued
+        // their fixed job_count jobs (the drain loop finishes the in-flight
+        // job).  Continuous-aggressor: A is unbounded, so the loop can only end
+        // after V's FINAL job has actually completed
+        // (v_next == job_count && dma[0].job_done) AND the last launched A job
+        // has drained.
+        while (continuous
+                 ? !((v_next >= job_count && dma[0].job_done) &&
+                     !(a_pages && !dma[1].job_done))
+                 : (v_next < job_count || (a_pages && a_next < job_count))) {
             tick();
+            // A-job completion edge (A jobs are strictly serial: one active at
+            // a time), counted in BOTH loops so the drain of the last A job is
+            // observed in the default runs as well.
+            if (a_pages) {
+                if (!a_done_prev && dma[1].job_done) ++a_completed;
+                a_done_prev = dma[1].job_done;
+            }
             // A pending planned V gap is a deliberate idle: widen the progress
             // window by that bounded gap so a planned gap is never reported as
             // a stall, while a real stall is still caught.
@@ -1204,7 +1242,18 @@
                     v_ready_at = -1;
                 }
             }
-            if (a_pages && dma[1].job_done && a_next < job_count) start_job(1, a_next++);
+            if (a_pages && dma[1].job_done) {
+                if (continuous) {
+                    // Stop launching A the moment V's final job is done; the
+                    // in-flight A job is drained by the loop exit condition.
+                    const bool v_final_done =
+                        (v_next >= job_count) && dma[0].job_done;
+                    if (!v_final_done) { start_job(1, a_next++); ++a_issued; }
+                } else if (a_next < job_count) {
+                    start_job(1, a_next++);
+                    ++a_issued;
+                }
+            }
             if (g_cycle - scenario_start > budget) {
                 dump_stall(std::string("scenario ") + name);
                 throw TestFailure("scenario budget exceeded");
@@ -1274,6 +1323,11 @@
                     (unsigned long)d_nonblock_fire,
                     (unsigned long)d_nonblock_busy,
                     v_gap_cycles, (int)policy_partition);
+        // Continuous-aggressor accounting (additive; only printed for the
+        // opt-in flag so the default 12-row output is unchanged).
+        if (continuous_aggressor)
+            std::printf("COUPLED-AGGRESSOR %s continuous=%d a_issued=%lu "
+                        "a_completed=%lu v_jobs=%u\n",
+                        name, (int)continuous, (unsigned long)a_issued,
+                        (unsigned long)a_completed, job_count);
     }
 };
@@ -1333,6 +1387,7 @@
     std::string job_count_str = "64";
     bool overlap = false;
     bool fullbuffer_permit = false;
+    bool continuous_aggressor = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
@@ -1349,6 +1404,7 @@
         if (a.rfind(jkey, 0) == 0) job_count_str = a.substr(jkey.size());
         if (a == "--overlap") overlap = true;
         if (a == "--fullbuffer-permit") fullbuffer_permit = true;
+        if (a == "--continuous-aggressor") continuous_aggressor = true;
     }
     if (order != "vv" && order != "aa" && order != "av" && order != "rr") {
         std::fprintf(stderr, "nonblock_coupled_test: unknown +order='%s' "
@@ -1418,6 +1474,8 @@
     }
     std::printf("nonblock_coupled_test: fullbuffer_permit=%d\n",
                 (int)fullbuffer_permit);
+    std::printf("nonblock_coupled_test: continuous_aggressor=%d\n",
+                (int)continuous_aggressor);
     std::printf("nonblock_coupled_test: v_gap_cycles=%u\n", v_gap_cycles);
     std::printf("nonblock_coupled_test: job_count=%u\n", job_count);
     std::printf("nonblock_coupled_test: policy_partition=%d "
@@ -1445,6 +1503,7 @@
             tb.v_gap_cycles      = v_gap_cycles;
             tb.policy_partition  = policy_partition;
             tb.job_count         = job_count;
+            tb.continuous_aggressor = continuous_aggressor;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
         } catch (const std::exception& e) {
             ++failed;
```

### tests/test_nonblock_coupled.py

```diff
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -113,6 +113,16 @@
                          "scenarios; each scenario has one 4 KiB job per DMA, so "
                          "the A-enabled scenarios exercise two jobs on the two "
                          "separate 512-deep coupling FIFOs. Default is 0 (legacy "
                          "guard).")
+    ap.add_argument("--continuous-aggressor", action="store_true",
+                    help="opt-in: when set, and the selected scenario has an A "
+                         "aggressor (a_pages > 0), the A DMA keeps re-issuing "
+                         "4 KiB jobs (cycling the existing a_pages pages) as soon "
+                         "as its previous job completes, until the V victim has "
+                         "actually completed its final job; the already-active A "
+                         "job is then drained.  Forwarded to the C++ harness as "
+                         "--continuous-aggressor.  Default off -> the historical "
+                         "fixed job-count run is unchanged.  Runtime flag only: "
+                         "same RTL, same obj directory.")
     ap.add_argument("--v-gap-cycles", type=int, choices=(0, 1000, 4000), default=0,
                     help="V inter-job gap: complete ticks the V victim waits "
                          "between finishing one 4 KiB job and starting the next "
@@ -231,12 +241,16 @@
     if opts.overlap:
         run_args.append("--overlap")
     if opts.fullbuffer_permit:
         run_args.append("--fullbuffer-permit")
+    if opts.continuous_aggressor:
+        run_args.append("--continuous-aggressor")
     print("test_nonblock_coupled: job_count={} couple_depth={} order={} "
-          "overlap={} fullbuffer_permit={} v_gap_cycles={} policy={}".format(
+          "overlap={} fullbuffer_permit={} v_gap_cycles={} policy={} "
+          "continuous_aggressor={}".format(
               opts.job_count, opts.couple_depth, opts.order,
               int(opts.overlap), int(opts.fullbuffer_permit),
-              opts.v_gap_cycles, int(opts.policy)))
+              opts.v_gap_cycles, int(opts.policy),
+              int(opts.continuous_aggressor)))
     run = subprocess.run(run_args)
     return run.returncode
```

---

## Why the exit-condition change is correct

Let `vfd = (v_next >= job_count && dma[0].job_done)` and `a_active = (a_pages && !dma[1].job_done)`.

- Continuous loop continues while `!(vfd && !a_active)`, i.e. **while V's last job is still active OR an A job is still active**.
- New A jobs are only launched while `dma[1].job_done && !vfd`. So once V's final job completes no new A work is issued, and the loop keeps ticking only to drain the active A job.
- The trailing drain loop `while (!dma[0].job_done || (a_pages && !dma[1].job_done))` is a no-op in continuous mode (already drained) and is byte-for-byte unchanged for the default path.
- Default (flag off, or `a_pages == 0`) uses the original `v_next < job_count || (a_pages && a_next < job_count)` predicate verbatim ⇒ the historical 12 rows / 64 jobs are unchanged.

`a_completed` is an edge count over strictly-serial A jobs; in both loops. By construction at exit `a_completed == a_issued` (all issued A work is drained), so `a_issued > job_count` is the "A did more jobs than V" signal Codex wants at `gap=1000`.

Budget sanity: `scenario_budget(4096, 1000) = 62500*4096 + 1000*4096 + 400000 ≈ 260.5M` cycles. The reported non-continuous reference run was 6,735,198 cycles; continuous A is still bounded by V's completion and drains immediately after, so it stays far under this backstop. `WATCHDOG_CYCLES` still fires on a genuine no-progress stall, and the V-gap widening (`idle_limit += v_gap_cycles`) is untouched.

---

## Assumptions

1. `--continuous-aggressor` is a C++ long option `--continuous-aggressor` mirrored by the Python flag; empty → default `false`.
2. "A exists" means `a_pages > 0`. For solo rows (`a_pages == 0`) `continuous` collapses to `false`, so those rows behave exactly as today.
3. A job completion is observable as `dma[1].job_done` (the loop already uses this). A jobs are strictly serial (no overlap of two A jobs), which the existing `start_job` + `dma_progress` already enforce.
4. The print is deliberately gated on the opt-in flag so the 12-row default stdout is unchanged.
5. `--overlap` / `--order rr` / `--couple-depth` are independent and untouched; the continuous predicate composes with them (overlap path of `dma_progress` still just sets `job_done`).

## Unresolved / for Codex to decide

- Whether to hard-error if `--continuous-aggressor` is given without an A row (`--scenario mode*-solo-*`). Currently it is a harmless no-op for those rows.
- Whether the `COUPLED-AGGRESSOR` line should also emit for solo rows under the flag (it does; `a_issued=0`).

## Acceptance checks (Codex must run)

1. Default unchanged: `python3 tests/test_nonblock_coupled.py` runs all 12 rows, PASS, output identical to before (the new print only appears with the flag).
2. Target run (A longer than V):
   `python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 --job-count 4096 --v-gap-cycles 1000 --scenario mode2-a8-l31 --continuous-aggressor`
   - expect `nonblock_coupled_test: continuous_aggressor=1`
   - expect `COUPLED-AGGRESSOR mode2-a8-l31 continuous=1 a_issued=<...> a_completed=<...> v_jobs=4096` with `a_issued > 4096` and `a_issued == a_completed`.
   - PASS with byte verification intact; no new stall/budget abort.
3. Stall-quality path: confirm that if it does not converge, the existing `dump_stall` output is produced (watchdog/budget), not a hang.
4. Compile with `-Wall`; the new locals (`continuous`, `v_final_done`) and members introduce no warnings.

Notes on diff application: context lines are exact against the provided contents; hunk header numbers are computed for cumulative application and tools with fuzz (`patch -p1`, or `git apply --recount`) will apply cleanly if your local copy differs slightly by blank lines. I could not compile or run anything.