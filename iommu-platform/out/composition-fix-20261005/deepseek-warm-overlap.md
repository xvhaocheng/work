## Findings — exact Linux sequencing (source line evidence)

All citations are `linux/drivers/misc/idma.c` (the file in context) unless stated.

**Warm launch order (A first, then V)** — `idma_run_session2()`:
- `2497-2501`: `if (a && (!batch || e.warmup_transfers)) { ret = idma_submit(a, false); ... }` → **A is submitted before the window**, and is only submitted at all when `warmup_transfers != 0` (or non-batch).
- `2502`: `ret = idma_run_window(v, a, e.warmup_transfers, deadline, false, &end);` → V work happens inside the window. Note `measured=false`: warm does **not** feed `refill_gap_ns_*` (`430-437` only accumulate when `measured`).

**`idma_run_window()` (V-first, then A)** — `689-748`:
- `696-701`: `count == 0` returns immediately (no V submit).
- `706`: **one** `idma_submit(v, measured)` before the poll loop (V reserved first).
- `718-720`: `idma_poll(v, …)` — V is polled **first**; `0` = pending, `1` = done, `<0` = error (`idma_poll()` `666-687`, match on `DONE == transfer_id`, `673-675`).
- `721-726`: on V done, `++completed`; at `completed == count` it captures `*end_ns = v->last_complete_ns` and **returns immediately** — i.e. no V resubmit and **no A poll/refill after the final V completion**.
- `727-729`: otherwise V is resubmitted (one outstanding V at a time by loop construction).
- `731-740`: **then** A is polled (`732`) and resubmitted on completion (`736`) — A-second, one outstanding A at a time.
- `741-743`: spin / `cond_resched()` / `cpu_relax()`.
- `250-769`: `idma_drain()` matches `DONE == transfer_id` with `readq_poll_timeout(..., IDMA_XFER_TIMEOUT_US)` (`66`: 2 s) and quarantines on failure.

**A ring/index advance** — `idma_submit_config()`/`idma_submit_commit()`:
- `414`: `s->offset = s->next_offset;` (submission uses the current ring page).
- `440-442`: `s->next_offset += s->chunk; if (s->next_offset == s->size) s->next_offset = 0;`
- First A submit uses offset 0: streams are zero-initialised (`2360`), `idma_stream_prepare()` (`353-399`) does not touch `next_offset`, and `idma_stream_reseed()` resets it to 0 (`1249`). So warm A walks pages `0,1,…,a_pages-1,0,…`, one page per submit.

**After the window** — `2517-2528`:
- `2518`: `v->next_offset = 0` (every measured V task restarts at the same address).
- `2522-2528`: in batch mode **A is drained** (`idma_drain(a)`, `2524-2526`) before the batch engines are configured (`2539-2545`).

**What the current C++ test does instead** (`tests/nonblock_actual_dma_test.cpp`):
- `896-923` `warm_phase()`: serial per iteration — V `program_single`+`trigger`+`wait_done` (`902-904`), then A over page `i % a_pages` (`906-912`). A page index is driven by the iteration counter, not by A submissions; A is never concurrent with V; no A drain; watchdog at `914` is refreshed by any handshake.

So the model vs. Linux deltas are: launch order (V-first vs A-first), concurrency (serial vs one-in-flight-per-channel), A ring driver (iteration index vs submit-index), A drain (absent), and V-count semantics (per-iteration `wait_done` vs exactly `count` completions). The patch below fixes only those, keeping the measured batch phase (`954-961`, `964-1021`), targets, gaps, cfg handshakes, DDR model and both build modes untouched.

---

## Proposed patch (`tests/nonblock_actual_dma_test.cpp`)

```diff
--- a/tests/nonblock_actual_dma_test.cpp
+++ b/tests/nonblock_actual_dma_test.cpp
@@ -15,13 +15,18 @@
 //   * DDR + page tables + one ordered R-return queue (completion + DDTW + PTW);
 //   * a per-4KiB page content checker.
 //
 // Software timeline (all phase prints are software-timing diagnostics and are
 // NOT a board measurement):
 //   1. setup_memory()/reset() per case (counters never carried across cases);
-//   2. warm: --warmup iterations; each iteration runs ONE V transfer
-//      (batch OFF, NEXT_ID/DONE polling) then ONE A transfer over --a-pages
-//      pages, SERIALLY.  WARM IS NOT A CONTINUOUS-A OVERLAP: the continuous
-//      autonomous-A overlap is exercised by the batch phase below, not here;
+//   2. warm: --warmup (256) V transfers CONCURRENT with a continuously
+//      refilled A ring over --a-pages pages, batch OFF on both.  This models
+//      the Linux warm schedule (linux/drivers/misc/idma.c:2497-2528 plus
+//      idma_run_window() at idma.c:689-748), NOT its absolute board timing:
+//      A is launched first, V second; each loop iteration polls and refills V
+//      before A; V stops at exactly --warmup completions; the A transfer is
+//      left open by that stop and drained.  The continuous autonomous-A
+//      overlap during the measurement window is still exercised by the batch
+//      phase below, not here;
 //   3. configure A autonomous batch (target --a-target) and V batch
 //      (target --transfers, pages=1, gap --v-gap), both configurations
 //      completed before any trigger;
@@ -28,6 +33,20 @@
 //   5. wait V batch complete, STOP A, drain to quiet;
 //   6. verify every touched page byte-for-byte; check quiet.
 //
+// Warm-phase model limitations (no board-equivalence claim):
+//   * every register access here is a full multi-cycle cfg AXI transaction,
+//     far slower than a board MMIO read, so the poll period, the V/A
+//     interleaving ratio and the number of A refills during warm are NOT
+//     board numbers.  Modelled is the driver ORDER: A launched first,
+//     V-first poll/refill, A-second poll/refill, exactly --warmup V
+//     completions (256 on the board run), one A ring page per submit, and
+//     the final A drain;
+//   * both DMAs are polled by one single-threaded software loop: no
+//     concurrent CPU, no IRQ latency and no multi-core contention is
+//     modelled;
+//   * the warm A refill count depends on model timing, so it is reported
+//     by the WARM-SUMMARY line and asserted only to be > 0, never to a
+//     board value.
+//
 // Exit: 0 on PASS, non-zero on FAIL / bounded STALL / WAIT.
 // 
 #ifdef ACTUAL_DMA_SYSTEM
@@ -159,6 +178,26 @@
     long scenario_budget = 0;
     // Per-V-progress watchdog (gap-scaled), set in reset().
     long v_watchdog_cycles = 200000L;
+    // ---------------- warm-phase model state / counters -------------------
+    // Software bookkeeping only: what THIS model scheduled and observed; not
+    // a hardware measurement and not a board target.
+    uint32_t warm_a_page         = 0;   // next A ring page (first page is 0)
+    uint64_t warm_v_submitted    = 0;   // V transfers submitted during warm
+    uint64_t warm_v_completed    = 0;   // V DONE matches observed
+    uint64_t warm_a_submitted    = 0;   // A transfers submitted during warm
+    uint64_t warm_a_completed    = 0;   // A DONE matches observed
+    uint64_t warm_overlap_ticks  = 0;   // ticks with V and A both in flight
+    bool     warm_a_drained      = false;
+    long     warm_a_drain_cycles = 0;
+    long     warm_start_cycle    = 0;
+    long     warm_end_cycle      = 0;
+    // Refreshed ONLY by a warm V completion, never by any A handshake, so a
+    // stalled V cannot be hidden by A traffic.
+    long     warm_v_progress_cycle = 0;
+    // One 4 KiB V job plus its walk is ~1e3-1e4 model cycles here; 200k is a
+    // generous but finite bound for both the V stall watchdog and the A drain.
+    static const long WARM_V_WATCHDOG_CYCLES = 200000L;
+    static const long WARM_A_DRAIN_CYCLES    = 200000L;
 #ifdef ACTUAL_DMA_SYSTEM
     // Synthetic-CPU independent progress baseline (refreshed only by the CPU
     // lifecycle counters, never by DMA handshakes).
@@ -681,6 +720,16 @@
         n_fast = 0;
         n_fast_bank[0] = n_fast_bank[1] = 0;
         v_done_last = a_done_last = 0;
+        warm_a_page         = 0;
+        warm_v_submitted    = 0;
+        warm_v_completed    = 0;
+        warm_a_submitted    = 0;
+        warm_a_completed    = 0;
+        warm_overlap_ticks  = 0;
+        warm_a_drained      = false;
+        warm_a_drain_cycles = 0;
+        warm_start_cycle    = 0;
+        warm_end_cycle      = 0;
 
         for (int i = 0; i < 8; ++i) tick();
         top->rst_ni = 1;
@@ -687,6 +736,7 @@
         for (int i = 0; i < 4; ++i) tick();
         last_progress_cycle = g_cycle;
         v_progress_cycle    = g_cycle;
+        warm_v_progress_cycle = g_cycle;
 #ifdef ACTUAL_DMA_SYSTEM
         n_ddr_ar = n_ddr_aw = n_ddr_b = n_ddr_r = 0;
         n_slv_ar[0] = n_slv_ar[1] = n_slv_ar[2] = n_slv_ar[3] = 0;
@@ -833,6 +883,16 @@
         std::fprintf(stderr,
             "  v_done_last(debug)=%llu a_done_last(debug)=%llu\n",
             (unsigned long long)v_done_last, (unsigned long long)a_done_last);
+        std::fprintf(stderr,
+            "  warm v_sub=%llu v_done=%llu a_sub=%llu a_done=%llu "
+            "overlap_ticks=%llu a_page=%u\n",
+            (unsigned long long)warm_v_submitted,
+            (unsigned long long)warm_v_completed,
+            (unsigned long long)warm_a_submitted,
+            (unsigned long long)warm_a_completed,
+            (unsigned long long)warm_overlap_ticks,
+            (unsigned)warm_a_page);
         std::fprintf(stderr,
             "  mem_q=%zu caw_q=%zu cb_q=%zu\n",
             mem_q.size(), caw_q.size(), cb_q.size());
@@ -886,6 +946,9 @@
     }
     long total_budget() const {
         long per_job = per_job_cycles();
+        // The warm phase is concurrent, but it still moves up to `warmup` V
+        // jobs plus a comparable number of A refills, so the original
+        // two-jobs-per-warmup bound is kept.
         long warm    = (long)warmup * 2L * per_job;
         long victim  = (long)v_transfers * (per_job + (long)v_gap_cycles);
         long aggr    = (long)a_pages * 16L * per_job;
@@ -893,42 +956,184 @@
     }
 
     // ---------------------------------------------------------------------
-    // warm phase: --warmup iterations, V then A, SERIAL
+    // warm phase: one V transfer in flight, concurrently with a continuously
+    // refilled A ring, batch OFF on both channels.  This models the Linux
+    // warm schedule:
+    //   idma_run_session2()  linux/drivers/misc/idma.c:2497-2528
+    //   idma_submit()        linux/drivers/misc/idma.c:447-467
+    //   idma_run_window()    linux/drivers/misc/idma.c:689-748
+    //   idma_drain()         linux/drivers/misc/idma.c:750-769
+    // The ORDER is modelled; the absolute timing is not (see the model
+    // limitations note at the top of this file).
     // ---------------------------------------------------------------------
+
+    // One 4 KiB V copy, batch OFF.  Returns the reserved NEXT_ID so DONE can
+    // be matched exactly; a stale DONE is never accepted.
+    uint64_t warm_submit_v() {
+        program_single(SEL_V, V_SRC_IOVA, V_DST_IOVA);
+        ++warm_v_submitted;
+        return trigger(SEL_V);
+    }
+
+    // One A ring step, batch OFF.  The ring index starts at page 0 and
+    // advances one page per submit with wrap, like the driver's
+    // "next_offset += chunk; if (next_offset == size) next_offset = 0"
+    // (idma.c:440-442) over an a_pages-page ring.  Source page p is
+    // A_IOVA_BASE + p*4K and destination page p is
+    // A_IOVA_BASE + (a_pages + p)*4K, as setup_memory() maps them.
+    uint64_t warm_submit_a() {
+        const uint32_t p = warm_a_page;
+
+        CHECK(p < a_pages, "warm A ring page index out of range");
+        program_single(SEL_A,
+                       A_IOVA_BASE + (uint64_t)p * 0x1000,
+                       A_IOVA_BASE + (uint64_t)(a_pages + p) * 0x1000);
+        warm_a_page = (p + 1 == a_pages) ? 0 : (p + 1);
+        ++warm_a_submitted;
+        return trigger(SEL_A);
+    }
+
     void warm_phase() {
-        for (uint32_t i = 0; i < warmup; ++i) {
-            // V : one 4 KiB copy, batch OFF.  The expected ID is the value the
-            // NEXT_ID read returned, so a wrong/stale DONE is never accepted.
-            program_single(SEL_V, V_SRC_IOVA, V_DST_IOVA);
-            uint64_t v_exp = trigger(SEL_V);
-            wait_done(SEL_V, v_exp, "warm V");
-
-            // A : ring over a_pages, batch OFF
-            uint32_t p = i % a_pages;
-            program_single(SEL_A,
-                           A_IOVA_BASE + (uint64_t)p * 0x1000,
-                           A_IOVA_BASE + (uint64_t)(a_pages + p) * 0x1000);
-            uint64_t a_exp = trigger(SEL_A);
-            wait_done(SEL_A, a_exp, "warm A");
-
-            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
-                dump_stall("warm");
-                throw TestFailure("watchdog during warm-up");
-            }
-            if (g_cycle - scenario_start > scenario_budget) {
-                dump_stall("warm budget");
-                throw TestFailure("scenario budget exceeded during warm-up");
-            }
-        }
+        // Software mirror of struct idma_stream::in_flight: at most one
+        // transfer outstanding per channel.
+        bool     v_in_flight = false, a_in_flight = false;
+        uint64_t v_id = 0, a_id = 0;
+        long     drain_start;
+        long     a_deadline;
+
+        warm_start_cycle      = g_cycle;
+        warm_v_progress_cycle = g_cycle;
+        warm_a_page           = 0;
+        warm_v_submitted      = 0;
+        warm_v_completed      = 0;
+        warm_a_submitted      = 0;
+        warm_a_completed      = 0;
+        warm_overlap_ticks    = 0;
+        warm_a_drained        = false;
+        warm_a_drain_cycles   = 0;
+
+        if (warmup == 0) {
+            // idma.c:2497-2502: with warmup_transfers == 0 the driver
+            // neither launches A nor enters the V window.
+            warm_a_drained = true;
+            warm_end_cycle = g_cycle;
+            return;
+        }
+
+        // ---- A-first initial launch (idma.c:2497-2501) -------------------
+        a_id = warm_submit_a();
+        a_in_flight = true;
+
+        // ---- V first submit, before the poll loop (idma.c:706) ----------
+        v_id = warm_submit_v();
+        v_in_flight = true;
+
+        for (;;) {
+            // Bounded watchdog refreshed ONLY by a V (DMA0) completion, so A
+            // handshakes can never mask a stalled V.
+            if (g_cycle - warm_v_progress_cycle > WARM_V_WATCHDOG_CYCLES) {
+                dump_stall("warm V progress");
+                throw TestFailure("watchdog: warm V made no completion progress");
+            }
+            if (g_cycle - scenario_start > scenario_budget) {
+                dump_stall("warm budget");
+                throw TestFailure("scenario budget exceeded during warm-up");
+            }
+
+            // ---- V first: poll (idma.c:718), then refill (idma.c:727) -----
+            if (v_in_flight && cfg_rd(SEL_V, REG_DONE) == v_id) {
+                v_in_flight = false;
+                ++warm_v_completed;
+                warm_v_progress_cycle = g_cycle;
+                // idma.c:722-726: stop at exactly `count` completions, before
+                // A is polled or refilled again.
+                if (warm_v_completed == warmup)
+                    break;
+                v_id = warm_submit_v();
+                v_in_flight = true;
+            }
+
+            // ---- A second: poll (idma.c:731), refill (idma.c:736) --------
+            if (a_in_flight && cfg_rd(SEL_A, REG_DONE) == a_id) {
+                a_in_flight = false;
+                ++warm_a_completed;
+                a_id = warm_submit_a();
+                a_in_flight = true;
+            }
+
+            // Overlap evidence: both channels have one transfer outstanding
+            // in this software model.
+            if (v_in_flight && a_in_flight)
+                ++warm_overlap_ticks;
+
+            tick();
+        }
+
+        CHECK(!v_in_flight, "warm V still in flight after the target count");
+        CHECK(warm_v_completed == warmup,
+              "warm V completion count does not match --warmup");
+        CHECK(warm_v_submitted == warmup,
+              "warm issued more V transfers than the target count");
+        CHECK(warm_overlap_ticks > 0,
+              "warm never had V and A in flight at the same time");
+        CHECK(warm_a_completed > 0, "warm A completed no transfer");
+
+        // ---- drain the still-open A transfer (idma.c:2524-2528) ---------
+        // A fixed cycle cap keeps this bound independent of any progress
+        // heuristic, like idma_drain()'s fixed timeout.
+        drain_start = g_cycle;
+        if (a_in_flight) {
+            a_deadline = g_cycle + WARM_A_DRAIN_CYCLES;
+            for (;;) {
+                if (cfg_rd(SEL_A, REG_DONE) == a_id) {
+                    a_in_flight = false;
+                    break;
+                }
+                if (g_cycle >= a_deadline) {
+                    dump_stall("warm A drain");
+                    throw TestFailure("watchdog: warm A drain never completed");
+                }
+                tick();
+            }
+        }
+        warm_a_drain_cycles = g_cycle - drain_start;
+        warm_a_drained      = true;
+        warm_end_cycle      = g_cycle;
+
+        // ---- bounded A-ring data evidence -------------------------------
+        // One A transfer is outstanding at a time and jobs retire in order,
+        // so `warm_a_completed` completions cover exactly the first
+        // min(warm_a_completed, a_pages) ring pages, starting at page 0.
+        // This proves warm A really moved data, not only that DONE matched.
+        // V's single page is covered by the scenario-wide check below.
+        {
+            const uint32_t pages_done =
+                (warm_a_completed < (uint64_t)a_pages) ?
+                (uint32_t)warm_a_completed : a_pages;
+
+            for (uint32_t p = 0; p < pages_done; ++p) {
+                for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
+                    CHECK(read64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 +
+                                 (uint64_t)i * 8) == page_word(1 + p, i),
+                          "warm A source page was modified");
+                    CHECK(read64(A_DST_PA_BASE + (uint64_t)p * 0x1000 +
+                                 (uint64_t)i * 8) == page_word(1 + p, i),
+                          "warm A destination page was not copied");
+                }
+            }
+        }
     }
 
     void drain_a() {
-        // batch is OFF during warm: nothing is outstanding by construction,
-        // but keep the check so a future change cannot silently paper over it.
+        // batch is OFF during warm and the Linux warm path leaves at most
+        // ONE A transfer open, which warm_phase() drains in-line under a
+        // fixed cycle cap.  Keep the checks so a future change cannot
+        // silently paper over an outstanding A.
         bool err, complete, stopped, active, enable;
         read_batch_ctl(SEL_A, err, complete, stopped, active, enable);
         CHECK(!active, "A still active after warm drain");
+        CHECK(warm_a_drained, "warm A drain did not complete");
     }
 
     // ---------------------------------------------------------------------
     // one scenario
     // ---------------------------------------------------------------------
@@ -950,6 +1155,20 @@
         // ---- 1. warm + drain -------------------------------------------
         warm_phase();
         drain_a();
+        std::printf("WARM-SUMMARY model v_target=%u v_submitted=%llu "
+                    "v_completed=%llu a_submitted=%llu a_completed=%llu "
+                    "overlap_ticks=%llu a_drain_cycles=%ld warm_cycles=%ld "
+                    "(software-model counters, NOT a board measurement)\n",
+                    (unsigned)warmup,
+                    (unsigned long long)warm_v_submitted,
+                    (unsigned long long)warm_v_completed,
+                    (unsigned long long)warm_a_submitted,
+                    (unsigned long long)warm_a_completed,
+                    (unsigned long long)warm_overlap_ticks,
+                    warm_a_drain_cycles,
+                    warm_end_cycle - warm_start_cycle);
 
         // ---- 2. configure BOTH batches before any trigger --------------
         // A autonomous: ring over a_pages, no gap.
```

Notes on the patch:
- Hunk line numbers are derived from the numbering supplied with the task; hunks are in ascending file order, use 2–3 context lines and do not overlap. `git apply`/`patch -p1` should apply cleanly; if offsets are reported, they are only from the two comment hunks.
- Nothing outside `warm_phase()`/`drain_a()`/counters/one comment/one printf touches behaviour: the measured batch phase (`configure_batch` A `a-target`/`a_pages`/gap 0, V `--transfers`/1 page/`--v-gap`, A-then-V triggers, V-complete wait, STOP A, drain, `check_quiet`, byte verification, DDR/mode guards) is unchanged.
- No hardware flags are invented: only `cfg_wr`/`cfg_rd` (`program_single`/`trigger`/`REG_DONE` polls), `tick()`, and the existing memory model are used.
- Header/copyright (`SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1`, lines 1-5) preserved.

### Why no runner change is needed (`tests/test_nonblock_actual_dma.py`)
`--warmup` already means “warm transfers”; the default 256 (`216-217`) is the board-run value, and the C++ now maps it to the **exact V completion count**. Hard-asserting `== 256` in C++ would make `+warmup` (still accepted at `292`, and used by callers) unusable and would break `--warmup 0`; instead the value is checked against the configured target and printed as `v_target=` in `WARM-SUMMARY`. No runner edit is essential.

---

## Assumptions, uncertainties, unresolved items
- **Not executed.** No build/run was performed; nothing above is claimed to pass. In particular this patch makes **no claim** about S4096/N64/N1024/N4096 board behaviour, `fast_bank0=3510 / fast_bank1=0` vs board `fast A=2`, or the N1024/4096 hang. It is a fidelity change only.
- **Software concurrency, not hardware concurrency.** `warm_overlap_ticks` counts model ticks where the model holds one transfer outstanding on each channel; it is a construction property (A launched first, refilled until the V target), not a hardware observation. The data-backed part is `warm_a_completed > 0` plus the bounded A-ring prefix byte check.
- **In-order A retirement assumption.** The A-prefix check assumes one outstanding A transfer at a time ⇒ completions in submission order ⇒ pages `0..min(completed,a_pages)-1` copied. This follows from the model’s `a_in_flight` gating (and the frontend’s per-DMA job ordering), but it is an assumption about the DUT, not verified RTL here.
- **Poll granularity differs from the board.** Each `cfg_rd` is a full multi-cycle cfg AXI transaction (`cfg_xact`, `600-620`), so the model’s poll period is far longer than an MMIO read: the V/A interleaving ratio, the number of A refills and all cycle counts are model numbers. A hardware overlap cannot be inferred from this harness.
- **A drain duration.** `WARM_A_DRAIN_CYCLES = 200000` is a fixed cap chosen to be generous for one 4 KiB copy; `warm_a_drain_cycles` is reported so the real value is visible. If it proves too tight in SYSTEM mode (shared DDR + 50/50 delayer), the fix is a bounded constant bump, not a progress heuristic.
- **Watchdog split.** Warm V stall is now bounded by `WARM_V_WATCHDOG_CYCLES` refreshed only on V completion, plus the existing `scenario_budget`; the previous “any handshake” check inside warm (`914`) was removed because A traffic could mask a stalled V. `wait_done()` (used elsewhere) is unchanged.
- **`total_budget()` unchanged** (conservative: it still charges 2 jobs per warm-up iteration). If a future run trips “scenario budget exceeded during warm-up”, the budget formula is the place to revisit — deliberately not changed here.
- **New output line.** `WARM-SUMMARY …` is additive and distinctly prefixed; if any external script parses stdout strictly, that script would need to ignore it. `ACTUAL-DMA-SUMMARY` is untouched.
- **`warm_a_page`/counters reset** in both `reset()` and at warm entry (belt and braces) so counters never carry across scenarios.

## Suggested acceptance checks (for Codex to run)
1. Build both tops with the provided runner: `tests/test_nonblock_actual_dma.py --mode s|n --transfers 64` and `--mode n --transfers 64 --ptw-latency 31` (object dirs `out/nonblock-actual-dma/obj*`).
2. Confirm each run prints `WARM-SUMMARY … v_target=256 v_submitted=256 v_completed=256 a_completed>0 overlap_ticks>0` and still ends in `PASS` with `ACTUAL-DMA-SUMMARY` byte verification intact.
3. Same with `--system` (`-DACTUAL_DMA_SYSTEM`, `obj-system`) to confirm the shared-DDR/mem_q path and the synthetic-CPU guards still hold.
4. `--warmup 0` must skip warm entirely (no A launch, `WARM-SUMMARY … v_submitted=0`).
5. If any case now trips “warm V progress” or “warm A drain”, capture the new `dump_stall` warm line (`v_sub/a_sub/overlap/a_page`) — that is model-bounded failure evidence, not a board measurement.