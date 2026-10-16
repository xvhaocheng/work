# Findings and what the replacement script changes

Scope note: all 7 defects live in the **C++ harness** (`tests/nonblock_actual_dma_test.cpp`) and the **runner** (`tests/test_nonblock_actual_dma.py`). `tests/nonblock_actual_dma_system_top.sv` needs **no edit** for this list (the `dbg_cpu_wr_quiet` tap is already `(cw_state_q == CW_IDLE)`, i.e. true with `cpu_enable=0`), so the script below leaves it untouched and says so.

| # | Defect | Where it is fixed |
|---|---|---|
| 1 | `(fd && count<target)` / `(fi && count<target)` saturate and swallow extra pulses; no single‑active / ordering / done‑before‑issue rule | Edit **C1** (per‑copy recorder) + **C14** (report invariants) |
| 2 | `full_aw_n` never incremented → AW/B conservation always fails; byte classification only looks at slave2, no physical payload range, no RESP/ID cross‑check | Edits **C2–C5** (AW/W/R/B rising edges), **C13** (`full_reg_cross_check`), **C14** (`full_err==0`, `aw==b`) |
| 3 | `full_measure_begin` hard‑opens `win_open` at `t0` and `drive_ddr` reuses it for DSprio; legacy trace shares the file with the full trace | Edits **C6** (DS condition), **C7** (legacy trace off in full), **C8** (arm window, don't open it) |
| 4 | `total_budget` uses `a_pages*16`; one watchdog; delay loops can skip un‑started lanes unboundedly | Edits **C9** (budget), **C10/C11** (state), **C12** (pure watchdog fn + `run_full_batch`), **C14** (`scenario_budget` field) |
| 5 | `cpu_writes` terminal asserts `completed>0` even when `cpu_enable=0`; full mode silently forces the profile | Edits **C15** (paired zero checks), **C18** (reject unsupported profile) |
| 6 | Runner silently overwrites explicit `--transfers/--warmup/...`; C++ plusargs use `strtoul` (accepts negatives/garbage) | Edits **C16, C17, C18** (C++) and **P1** (runner) |
| 7 | `report_full` order, no `scenario_budget`, no readback note, watchdog not unit‑testable | Edits **C12** (pure fn), **C13** (readback cross‑check), **C14** (validate‑before‑print) + verifier script |

I found **no** CPU assertion inside `warm_phase()` (warm runs batch‑OFF, so `cpu_run` is low) and the CPU watchdog is gated on `dbg_dma_batch_active & 1` — so `cpu_enable=0` is safe there. That is why fix 5 only touches the terminal block.

---

## Code block 1 — `apply_full_batch_fixes.py`

Precise, idempotent replacement script. Run from the platform directory (the directory that contains `out/`). `ROOT` is fixed.

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""
apply_full_batch_fixes.py

Precise, idempotent replacement of the full-batch contract fixes in the
PROPOSED harness (out/workflow-g0-m0-20261010/proposed/tests).  The real
tests/ tree is NOT touched.  Only the changed hunks are carried here; nothing
unchanged is re-emitted.

Run from the directory that contains out/ :
    python3 apply_full_batch_fixes.py [--dry-run] [--check]
"""

import argparse
import sys
from pathlib import Path

ROOT = Path("out/workflow-g0-m0-20261010/proposed")

EDITS = {}


# ===========================================================================
# tests/nonblock_actual_dma_test.cpp
# ===========================================================================
EDITS["tests/nonblock_actual_dma_test.cpp"] = []

# ---- C16: <cerrno> for the strict plusarg parsers --------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""#include <cstdio>
#include <cstdint>""",
    r"""#include <cerrno>
#include <cstdio>
#include <cstdint>""",
))

# ---- C12a: pure full-batch watchdog decision function ----------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""#define CHECK(cond, msg) do { if (!(cond)) fail((msg), __FILE__, __LINE__); } while (0)""",
    r"""#define CHECK(cond, msg) do { if (!(cond)) fail((msg), __FILE__, __LINE__); } while (0)

#ifdef ACTUAL_DMA_SYSTEM
// Pure full-batch watchdog decision: it reads NO DUT / harness state, so it
// can be unit-tested directly with cycle numbers.  Before the channel has
// issued anything it gets the larger `supply_wait` allowance; afterwards it
// gets the steady-state `gap`.  Returns true when the channel must be
// declared stalled.
static bool full_watchdog_fault(long now, long last_progress, bool issued,
                                long supply_wait, long gap) {
    const long elapsed = now - last_progress;
    return issued ? (elapsed > gap) : (elapsed > supply_wait);
}
#endif""",
))

# ---- C10a: two separate watchdog bounds + per-lane started flag ------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    // Full-batch per-channel progress watchdog (cycles).
    long     full_watchdog_cycles = 200000L;""",
    r"""    // Full-batch per-channel progress watchdog (cycles).  Two DISTINCT
    // bounds are kept: the initial allowance before the lane has issued
    // anything, and the steady-state no-progress gap after the first issue.
    long     full_watchdog_cycles    = 200000L;
    long     full_supply_wait_cycles = 200000L;""",
))

EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    long     full_last_progress[2] = {0, 0};
    uint32_t full_issue_n[2]   = {0, 0};""",
    r"""    long     full_last_progress[2] = {0, 0};
    bool     full_started[2]   = {false, false};
    uint32_t full_issue_n[2]   = {0, 0};""",
))

# ---- C9: budget cap constant ----------------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    static const long WATCHDOG_CYCLES = 200000L;   // ~10x a 4 KiB copy + walk
    static const long CFG_TIMEOUT     = 100000L;""",
    r"""    static const long WATCHDOG_CYCLES = 200000L;   // ~10x a 4 KiB copy + walk
    static const long CFG_TIMEOUT     = 100000L;
    // Per-delay cap used when SIZING the scenario budget.  A larger requested
    // launch delay is accepted by the harness but cannot inflate the budget
    // without bound.
    static const long BUDGET_DELAY_CAP = 2000000L;""",
))

# ---- C7: disable the legacy (window-relative) trace in full mode -----------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    void trace_event(const char* event, unsigned tag, uint64_t addr, uint64_t value) {
        if (event_trace && win_open)
            std::fprintf(event_trace, "%ld,%s,%u,%llu,%llu\n",""",
    r"""    void trace_event(const char* event, unsigned tag, uint64_t addr, uint64_t value) {
        // The legacy trace is a WINDOW-relative timeline.  In full-batch mode
        // the full trace block emits a t0-relative timeline into the same
        // file, so the legacy trace is disabled there to keep exactly one time
        // base per file.
        if (event_trace && win_open && !full_era)
            std::fprintf(event_trace, "%ld,%s,%u,%llu,%llu\n",""",
))

# ---- C6: DS-priority window for full vs legacy -----------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        // Each request retains its original latency/beat state after rotation.
        if (ds_priority && win_open && !ddr_r_held &&
            ((mem_q.front().id >> 4) & 3) != 3) {""",
    r"""        // Each request retains its original latency/beat state after rotation.
        // The priority WINDOW is the legacy first-V-issue window in the legacy
        // path and the full-era V-completion count in full-batch mode
        // (v_transfers == 64 in the approved full configuration).  The held-R
        // guard applies to BOTH: a beat currently held MUST NOT be displaced.
        const bool ds_window = full_batch
            ? (full_era && full_done_n[0] < (uint32_t)v_transfers)
            : win_open;
        if (ds_priority && !ddr_r_held && ds_window &&
            ((mem_q.front().id >> 4) & 3) != 3) {""",
))

# ---- C1: every ISSUE/DONE pulse counts; single-active; ordering ------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""            for (int c = 0; c < 2; ++c) {
                const uint32_t bit    = 1u << c;
                const uint32_t target = (c == 0) ? v_transfers : (uint32_t)a_target;
                if ((fd & bit) && full_done_n[c] < target) {
                    full_done_cyc[c].push_back(g_cycle);
                    ++full_done_n[c];
                    full_final_done[c]      = g_cycle;
                    full_last_progress[c]   = g_cycle;
                }
                if ((fi & bit) && full_issue_n[c] < target) {
                    const long ready = (full_issue_n[c] == 0)
                        ? full_t0
                        : (full_done_cyc[c].empty() ? full_t0
                                                    : full_done_cyc[c].back());
                    full_ready_cyc[c].push_back(ready);
                    full_issue_cyc[c].push_back(g_cycle);
                    ++full_issue_n[c];
                    if (!full_issue_seen[c]) {
                        full_issue_seen[c]  = true;
                        full_first_issue[c] = g_cycle;
                    }
                    full_last_progress[c] = g_cycle;
                }
            }""",
    r"""            for (int c = 0; c < 2; ++c) {
                const uint32_t bit    = 1u << c;
                const uint32_t target = (c == 0) ? v_transfers : (uint32_t)a_target;
                // DONE is processed BEFORE ISSUE so a back-to-back same-cycle
                // done->issue keeps ready[j] == done[j-1].
                if (fd & bit) {
                    // Every pulse counts.  An extra pulse is a HARD failure:
                    // saturation would silently hide a duplicate DONE.
                    CHECK(full_done_n[c] < target,
                          "full-batch DONE pulse beyond the channel target");
                    CHECK(full_done_n[c] < full_issue_n[c],
                          "full-batch DONE pulse without a matching ISSUE");
                    full_done_cyc[c].push_back(g_cycle);
                    ++full_done_n[c];
                    full_final_done[c]      = g_cycle;
                    full_last_progress[c]   = g_cycle;
                }
                if (fi & bit) {
                    CHECK(full_issue_n[c] < target,
                          "full-batch ISSUE pulse beyond the channel target");
                    // Strict single-active: no new job may start while the
                    // previous job of the same channel is still outstanding.
                    CHECK(full_issue_n[c] == full_done_n[c],
                          "full-batch channel issued a second job before DONE");
                    const long ready = (full_issue_n[c] == 0)
                        ? full_t0
                        : full_done_cyc[c].back();
                    CHECK(ready <= g_cycle,
                          "full-batch per-copy ready time is after its issue");
                    full_ready_cyc[c].push_back(ready);
                    full_issue_cyc[c].push_back(g_cycle);
                    ++full_issue_n[c];
                    if (!full_issue_seen[c]) {
                        full_issue_seen[c]  = true;
                        full_first_issue[c] = g_cycle;
                    }
                    full_last_progress[c] = g_cycle;
                }
            }""",
))

# ---- C2: AW -- increment full_aw_n, classify by slave2 + dst range ---------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        if (f_ddr_aw) {
            ++n_ddr_aw;
            CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;
            c.len = (uint8_t)s_ddr_aw_l; c.got = 0;
            caw_q.push_back(c);
        }""",
    r"""        if (f_ddr_aw) {
            ++n_ddr_aw;
            if (full_era) {
                const unsigned slv = (s_ddr_aw_i >> 4) & 3u;
                const unsigned chn = (unsigned)((s_ddr_aw_i >> 2) & 1u);
                if (slv == 2u) {
                    // Completion write payload.  Classification needs BOTH the
                    // completion slave port / bank bit AND a legal 4 KiB
                    // physical payload window (the V measurement destination
                    // page or an A ring destination page).  A synthetic-CPU
                    // write to the SAME V address arrives on slave port 0 and
                    // is therefore excluded.
                    const uint64_t a0 = s_ddr_aw_a;
                    const uint64_t a1 = a0 + ((uint64_t)s_ddr_aw_l + 1ULL) * 8ULL - 1ULL;
                    CHECK((a0 >> 12) == (a1 >> 12),
                          "full-batch AW payload crosses a 4 KiB page");
                    const bool in_v = (a0 >= V_DST_PA_LO && a1 < V_DST_PA_HI);
                    const bool in_a = (a0 >= A_DST_PA_BASE &&
                                       a1 <  A_DST_PA_BASE + (uint64_t)a_pages * 0x1000ULL);
                    CHECK(in_v || in_a,
                          "full-batch AW payload outside the completion payload map");
                    ++full_aw_n[chn];
                }
            }
            CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;
            c.len = (uint8_t)s_ddr_aw_l; c.got = 0;
            caw_q.push_back(c);
        }""",
))

# ---- C4: B -- id must match the queued AW; resp recorded -------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        if (f_ddr_b) {
            ++n_ddr_b;
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            // full-batch: B response conservation (slave port 2 only).
            if (full_era && ((((uint32_t)(uint8_t)cb_q.front()) >> 4) & 3u) == 2u) {
                const unsigned ch =
                    (unsigned)((((uint32_t)(uint8_t)cb_q.front()) >> 2) & 1u);
                ++full_b_n[ch];
                if (top->ddr_b_resp != 0) ++full_err[ch];
            }
            cb_q.pop_front();
        }""",
    r"""        if (f_ddr_b) {
            ++n_ddr_b;
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            const uint32_t bq_id = (uint32_t)(uint8_t)cb_q.front();
            // The driven B must echo the queued completion AW id.
            CHECK((uint32_t)top->ddr_b_id == bq_id,
                  "DDR B id does not match the queued completion AW id");
            // full-batch: B response conservation (slave port 2 only).
            if (full_era && ((bq_id >> 4) & 3u) == 2u) {
                const unsigned chn = (unsigned)((bq_id >> 2) & 1u);
                ++full_b_n[chn];
                if (top->ddr_b_resp != 0) ++full_err[chn];
            }
            cb_q.pop_front();
        }""",
))

# ---- C5: R -- head==driven id/last; src range; resp recorded ---------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        if (f_ddr_r) {
            ++n_ddr_r;
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");
            // full-batch: accepted-AR -> R byte conservation (slave port 2).
            if (full_era && ((((uint8_t)h.id) >> 4) & 3u) == 2u) {
                const unsigned ch = (unsigned)((((uint8_t)h.id) >> 2) & 1u);
                ++full_r_n[ch];
                full_r_return_bytes[ch] += 8ULL;   // one 64-bit R beat
            }
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }""",
    r"""        if (f_ddr_r) {
            ++n_ddr_r;
            CHECK(!mem_q.empty(), "R handshake with an empty DDR read queue");
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");
            // The R payload driven this cycle must match the queue head: the
            // id is echoed verbatim and LAST marks the burst tail.
            CHECK((uint32_t)top->ddr_r_id == (uint32_t)h.id &&
                  ((h.beat == h.len) == (top->ddr_r_last != 0)),
                  "DDR R id/LAST does not match the queued request head");
            // full-batch: accepted-AR -> R byte conservation (slave port 2).
            if (full_era && ((((uint8_t)h.id) >> 4) & 3u) == 2u) {
                const unsigned chn = (unsigned)((((uint8_t)h.id) >> 2) & 1u);
                // Payload range: only the completion source pages may count.
                const uint64_t a1 = h.addr + ((uint64_t)h.len + 1ULL) * 8ULL - 1ULL;
                CHECK((h.addr >> 12) == (a1 >> 12),
                      "full-batch AR payload crosses a 4 KiB page");
                const bool in_v = (h.addr >= V_SRC_PA &&
                                   a1 <  V_SRC_PA + (uint64_t)PAGE_BYTES);
                const bool in_a = (h.addr >= A_SRC_PA_BASE &&
                                   a1 <  A_SRC_PA_BASE + (uint64_t)a_pages * 0x1000ULL);
                CHECK(in_v || in_a,
                      "full-batch AR payload outside the completion source map");
                ++full_r_n[chn];
                full_r_return_bytes[chn] += 8ULL;   // one 64-bit R beat
                if (top->ddr_r_resp != 0) ++full_err[chn];
            }
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }""",
))

# ---- C11a: reset() clears the new state ------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        full_last_progress[0] = full_last_progress[1] = g_cycle;
        full_issue_n[0] = full_issue_n[1] = 0;
        full_done_n[0]  = full_done_n[1]  = 0;
        full_issue_seen[0] = full_issue_seen[1] = false;""",
    r"""        full_last_progress[0] = full_last_progress[1] = g_cycle;
        full_started[0] = full_started[1] = false;
        full_issue_n[0] = full_issue_n[1] = 0;
        full_done_n[0]  = full_done_n[1]  = 0;
        full_issue_seen[0] = full_issue_seen[1] = false;""",
))

# ---- C9b: total_budget for full mode --------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    long total_budget() const {
        long per_job = per_job_cycles();
        // The warm phase is concurrent, but it still moves up to `warmup` V
        // jobs plus a comparable number of A refills, so the original
        // two-jobs-per-warmup bound is kept.
        long warm    = (long)warmup * 2L * per_job;
        long victim  = (long)v_transfers * (per_job + (long)v_gap_cycles);
        long aggr    = (long)a_pages * 16L * per_job;
        return 200000L + warm + victim + aggr;
    }""",
    r"""    long total_budget() const {
        long per_job = per_job_cycles();
        // The warm phase is concurrent, but it still moves up to `warmup` V
        // jobs plus a comparable number of A refills, so the original
        // two-jobs-per-warmup bound is kept.
        long warm    = (long)warmup * 2L * per_job;
        const long wrap = 200000L;   // setup + drain-to-quiet + readback
        if (full_batch) {
            // Full-batch budget: warm + V(batch target) + A(batch target) +
            // the three BOUNDED launch delays + wrap-up.  The A term is
            // bounded by a_target, NOT by a_pages * 16: the ring page count is
            // not a work multiplier in full mode.
            long victim = (long)v_transfers * (per_job + (long)v_gap_cycles);
            long aggr   = (long)a_target * per_job;
            long d1 = std::min<long>((long)start_delay,   BUDGET_DELAY_CAP);
            long d2 = std::min<long>((long)v_start_delay, BUDGET_DELAY_CAP);
            long d3 = std::min<long>((long)a_start_delay, BUDGET_DELAY_CAP);
            return wrap + warm + victim + aggr + d1 + d2 + d3;
        }
        long victim  = (long)v_transfers * (per_job + (long)v_gap_cycles);
        long aggr    = (long)a_pages * 16L * per_job;
        return wrap + warm + victim + aggr;
    }""",
))

# ---- C11b: full_measure_begin resets full_started --------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        for (int c = 0; c < 2; ++c) {
            full_last_progress[c] = g_cycle;
            full_issue_n[c]       = 0;
            full_done_n[c]        = 0;
            full_issue_seen[c]    = false;""",
    r"""        full_supply_wait_cycles = full_watchdog_cycles;
        for (int c = 0; c < 2; ++c) {
            full_last_progress[c] = g_cycle;
            full_started[c]       = false;
            full_issue_n[c]       = 0;
            full_done_n[c]        = 0;
            full_issue_seen[c]    = false;""",
))

# ---- C8: arm the pre-existing window instead of opening it at t0 ----------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        full_trace_ptw_active = false;
        // Open the model-only DS-priority window at the common t0.  The
        // full-batch statistics never read win_open.
        win_open       = true;
        win_seen       = true;
        win_armed      = false;
        win_open_cycle = g_cycle;
        fg_raw_open = (uint32_t)top->dbg_frontier_grant;
        fw_raw_open = (uint32_t)top->dbg_frontier_wait;
    }""",
    r"""        full_trace_ptw_active = false;
        // Arm the PRE-EXISTING measurement window exactly as the legacy path
        // does: it opens at the first hardware V issue and closes at the final
        // V done.  The full-batch statistics never read win_open, and the DS
        // priority uses the full-era completion counter instead, so this
        // window only feeds the shared observation plumbing.
        win_open        = false;
        win_seen        = false;
        win_armed       = true;
        win_open_cycle  = 0;
        win_close_cycle = 0;
        fg_raw_open = (uint32_t)top->dbg_frontier_grant;
        fw_raw_open = (uint32_t)top->dbg_frontier_wait;
    }""",
))

# ---- C12b: full_channel_watchdog + started-lane policing -------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    void full_channel_watchdog(int ch, const char* tag) {
        const uint32_t target = (ch == 0) ? v_transfers : (uint32_t)a_target;
        if (full_done_n[ch] >= target) return;
        if (g_cycle - full_last_progress[ch] > full_watchdog_cycles) {
            dump_stall(std::string("full watchdog ") + tag);
            throw TestFailure("watchdog: full-batch channel made no progress");
        }
    }""",
    r"""    void full_channel_watchdog(int ch, const char* tag) {
        const uint32_t target = (ch == 0) ? v_transfers : (uint32_t)a_target;
        if (full_done_n[ch] >= target) return;   // completed lane: auto-closed
        if (!full_started[ch])         return;   // lane not launched yet
        if (full_watchdog_fault(g_cycle, full_last_progress[ch],
                                full_issue_seen[ch],
                                full_supply_wait_cycles, full_watchdog_cycles)) {
            dump_stall(std::string("full watchdog ") + tag);
            throw TestFailure("watchdog: full-batch channel made no progress");
        }
    }

    // Police every lane that has already been launched.  Used inside the
    // bounded launch-delay loops so a long delay cannot hide a stalled lane.
    void full_check_started_lanes() {
        if (full_started[0]) full_channel_watchdog(0, "V");
        if (full_started[1]) full_channel_watchdog(1, "A");
    }""",
))

# ---- C12c: run_full_batch --------------------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        full_measure_begin();

        // Billed common delay after t0 and before the first NEXT_ID.
        for (uint32_t i = 0; i < start_delay; ++i) tick();

        // launch-order av/va selects which channel's NEXT_ID comes first; the
        // per-channel delays are bounded and purely acceptance.
        if (launch_order == "va") {
            for (uint32_t i = 0; i < v_start_delay; ++i) tick();
            start_batch(SEL_V);
            for (uint32_t i = 0; i < a_start_delay; ++i) tick();
            start_batch(SEL_A);
        } else {
            for (uint32_t i = 0; i < a_start_delay; ++i) tick();
            start_batch(SEL_A);
            for (uint32_t i = 0; i < v_start_delay; ++i) tick();
            start_batch(SEL_V);
        }

        bool v_done = false, a_done = false;
        bool err, complete, stopped, active, enable_b;
        for (;;) {
            tick();
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall("full scenario handshake");
                throw TestFailure("watchdog: no handshake progress in full-batch");
            }
            if (g_cycle - scenario_start > scenario_budget) {
                dump_stall("full scenario budget");
                throw TestFailure("scenario budget exhausted in full-batch");
            }
            if (!v_done) {
                full_channel_watchdog(0, "V");
                read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
                if (complete) v_done = true;
            }
            if (!a_done) {
                full_channel_watchdog(1, "A");
                read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
                if (complete) a_done = true;
            }""",
    r"""        full_measure_begin();

        // Billed common delay after t0 and before the first NEXT_ID.  t0 is
        // NOT moved by any delay; the delay is only billed into the budget.
        for (uint32_t i = 0; i < start_delay; ++i) {
            tick(); full_check_started_lanes();
        }

        // launch-order av/va selects which channel's NEXT_ID comes first; the
        // per-channel delays are bounded and purely acceptance.  Every delay
        // loop still polices every lane that already started, so a long delay
        // cannot skip the watchdog of a started lane.
        if (launch_order == "va") {
            for (uint32_t i = 0; i < v_start_delay; ++i) {
                tick(); full_check_started_lanes();
            }
            start_batch(SEL_V); full_started[0] = true; full_last_progress[0] = g_cycle;
            for (uint32_t i = 0; i < a_start_delay; ++i) {
                tick(); full_check_started_lanes();
            }
            start_batch(SEL_A); full_started[1] = true; full_last_progress[1] = g_cycle;
        } else {
            for (uint32_t i = 0; i < a_start_delay; ++i) {
                tick(); full_check_started_lanes();
            }
            start_batch(SEL_A); full_started[1] = true; full_last_progress[1] = g_cycle;
            for (uint32_t i = 0; i < v_start_delay; ++i) {
                tick(); full_check_started_lanes();
            }
            start_batch(SEL_V); full_started[0] = true; full_last_progress[0] = g_cycle;
        }

        bool v_done = false, a_done = false;
        bool err, complete, stopped, active, enable_b;
        for (;;) {
            tick();
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall("full scenario handshake");
                throw TestFailure("watchdog: no handshake progress in full-batch");
            }
            if (g_cycle - scenario_start > scenario_budget) {
                dump_stall("full scenario budget");
                throw TestFailure("scenario budget exhausted in full-batch");
            }
            if (!v_done) {
                full_channel_watchdog(0, "V");
                read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
                CHECK(!err, "full-batch V batch_ctl error during the run");
                if (complete) v_done = true;
            }
            if (!a_done) {
                full_channel_watchdog(1, "A");
                read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
                CHECK(!err, "full-batch A batch_ctl error during the run");
                if (complete) a_done = true;
            }""",
))

# ---- C13: full_reg_cross_check --------------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    void full_reg_cross_check() {
        bool err, complete, stopped, active, enable_b;
        read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
        full_reg_complete[0] = complete; full_reg_stopped[0] = stopped;
        full_reg_active[0]   = active;   full_reg_enable[0]  = enable_b;
        full_reg_issued[0]   = cfg_rd(SEL_V, REG_B_ISSUED);
        full_reg_done[0]     = cfg_rd(SEL_V, REG_B_DONE);
        read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
        full_reg_complete[1] = complete; full_reg_stopped[1] = stopped;
        full_reg_active[1]   = active;   full_reg_enable[1]  = enable_b;
        full_reg_issued[1]   = cfg_rd(SEL_A, REG_B_ISSUED);
        full_reg_done[1]     = cfg_rd(SEL_A, REG_B_DONE);

        CHECK(full_reg_issued[0] == (uint64_t)v_transfers &&
              full_reg_done[0]   == (uint64_t)v_transfers,
              "full-batch V REG_B_ISSUED/DONE do not match transfers");
        CHECK(full_reg_issued[1] == a_target && full_reg_done[1] == a_target,
              "full-batch A REG_B_ISSUED/DONE do not match a_target");
        CHECK(full_reg_complete[0] && full_reg_complete[1],
              "full-batch batch_ctl did not report complete on both channels");
        CHECK(!full_reg_stopped[0] && !full_reg_stopped[1],
              "full-batch reported STOP on a measured channel");
        CHECK(full_issue_n[0] == v_transfers && full_done_n[0] == v_transfers,
              "full-batch V hardware issue/done pulses != transfers");
        CHECK(full_issue_n[1] == (uint32_t)a_target &&
              full_done_n[1] == (uint32_t)a_target,
              "full-batch A hardware issue/done pulses != a_target");
    }""",
    r"""    void full_reg_cross_check() {
        // Register readback is a CROSS-CHECK only.  It happens AFTER the
        // protocol quiet moment was captured (check_quiet) and its idle
        // transaction overhead is therefore EXCLUDED from every reported time.
        bool err, complete, stopped, active, enable_b;
        read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
        CHECK(!err, "full-batch V batch_ctl reported an error at readback");
        full_reg_complete[0] = complete; full_reg_stopped[0] = stopped;
        full_reg_active[0]   = active;   full_reg_enable[0]  = enable_b;
        full_reg_issued[0]   = cfg_rd(SEL_V, REG_B_ISSUED);
        full_reg_done[0]     = cfg_rd(SEL_V, REG_B_DONE);
        read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
        CHECK(!err, "full-batch A batch_ctl reported an error at readback");
        full_reg_complete[1] = complete; full_reg_stopped[1] = stopped;
        full_reg_active[1]   = active;   full_reg_enable[1]  = enable_b;
        full_reg_issued[1]   = cfg_rd(SEL_A, REG_B_ISSUED);
        full_reg_done[1]     = cfg_rd(SEL_A, REG_B_DONE);

        CHECK(full_reg_issued[0] == (uint64_t)v_transfers &&
              full_reg_done[0]   == (uint64_t)v_transfers,
              "full-batch V REG_B_ISSUED/DONE do not match transfers");
        CHECK(full_reg_issued[1] == a_target && full_reg_done[1] == a_target,
              "full-batch A REG_B_ISSUED/DONE do not match a_target");
        CHECK(full_reg_complete[0] && full_reg_complete[1],
              "full-batch batch_ctl did not report complete on both channels");
        CHECK(!full_reg_stopped[0] && !full_reg_stopped[1],
              "full-batch reported STOP on a measured channel");
        CHECK(!full_reg_active[0] && !full_reg_active[1],
              "full-batch batch_ctl still active after natural completion");
        CHECK(full_reg_enable[0] && full_reg_enable[1],
              "full-batch batch_ctl enable bit was cleared before readback");
        CHECK(full_issue_n[0] == v_transfers && full_done_n[0] == v_transfers,
              "full-batch V hardware issue/done pulses != transfers");
        CHECK(full_issue_n[1] == (uint32_t)a_target &&
              full_done_n[1] == (uint32_t)a_target,
              "full-batch A hardware issue/done pulses != a_target");
    }""",
))

# ---- C14a: report_full header -- validate everything before printing ------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    void report_full(const char* name, bool enable) {
        // ---- per-copy invariants (verified before printing) -----------
        for (int c = 0; c < 2; ++c) {
            CHECK(!full_ready_cyc[c].empty(),
                  "full-batch per-copy readiness array is empty");
            CHECK(full_ready_cyc[c][0] == full_t0,
                  "full-batch ready[0] does not equal t0");
            const size_t n = full_issue_cyc[c].size();
            CHECK(full_ready_cyc[c].size() == n,
                  "full-batch ready/issue array length mismatch");
            for (size_t j = 1; j < n; ++j)
                CHECK(full_ready_cyc[c][j] == full_done_cyc[c][j - 1],
                      "full-batch ready[j] != done[j-1]");
        }
        // ---- payload conservation at the shared DDR port --------------
        for (int c = 0; c < 2; ++c) {
            CHECK(full_r_return_bytes[c] == full_rd_bytes[c],
                  "full-batch read AR->R byte conservation failed");
            CHECK(full_b_n[c] == full_aw_n[c],
                  "full-batch AW->B response conservation failed");
            CHECK(full_rd_bytes[c] == 262144ULL,
                  "full-batch read payload bytes != 262144");
            CHECK(full_wr_bytes[c] == 262144ULL,
                  "full-batch write payload bytes != 262144");
        }""",
    r"""    void report_full(const char* name, bool enable) {
        // ---- per-copy invariants (ALL validated before anything prints) --
        for (int c = 0; c < 2; ++c) {
            const uint32_t target = (c == 0) ? v_transfers : (uint32_t)a_target;
            CHECK(full_issue_n[c] == target && full_done_n[c] == target,
                  "full-batch hardware issue/done counts != channel target");
            CHECK(!full_ready_cyc[c].empty(),
                  "full-batch per-copy readiness array is empty");
            CHECK(full_ready_cyc[c].size() == (size_t)target,
                  "full-batch per-copy array length != channel target");
            CHECK(full_issue_cyc[c].size() == full_ready_cyc[c].size() &&
                  full_done_cyc[c].size()  == full_ready_cyc[c].size(),
                  "full-batch ready/issue/done array length mismatch");
            CHECK(full_ready_cyc[c][0] == full_t0,
                  "full-batch ready[0] does not equal t0");
            const size_t n = full_issue_cyc[c].size();
            for (size_t j = 0; j < n; ++j)
                CHECK(full_ready_cyc[c][j] <= full_issue_cyc[c][j] &&
                      full_issue_cyc[c][j] <= full_done_cyc[c][j],
                      "full-batch ready/issue/done order violated");
            for (size_t j = 1; j < n; ++j) {
                CHECK(full_ready_cyc[c][j] == full_done_cyc[c][j - 1],
                      "full-batch ready[j] != done[j-1]");
                CHECK(full_issue_cyc[c][j] >= full_done_cyc[c][j - 1],
                      "full-batch issue[j] precedes done[j-1] (single-active)");
                CHECK(full_issue_cyc[c][j] > full_issue_cyc[c][j - 1],
                      "full-batch issue times are not increasing");
                CHECK(full_done_cyc[c][j] > full_done_cyc[c][j - 1],
                      "full-batch done times are not increasing");
            }
            CHECK(full_done_n[c] <= full_issue_n[c],
                  "full-batch done pulses exceed issue pulses");
            CHECK(full_final_done[c] == full_done_cyc[c].back(),
                  "full-batch final-done marker != last done cycle");
        }
        // ---- payload / response conservation at the shared DDR port ---
        for (int c = 0; c < 2; ++c) {
            CHECK(full_ar_n[c] > 0 && full_aw_n[c] > 0,
                  "full-batch completion channel observed no AR/AW traffic");
            CHECK(full_r_return_bytes[c] == full_rd_bytes[c],
                  "full-batch read AR->R byte conservation failed");
            CHECK(full_b_n[c] == full_aw_n[c],
                  "full-batch AW->B response conservation failed");
            CHECK(full_rd_bytes[c] == 262144ULL,
                  "full-batch read payload bytes != 262144");
            CHECK(full_wr_bytes[c] == 262144ULL,
                  "full-batch write payload bytes != 262144");
            CHECK(full_wr_bytes[c] == full_rd_bytes[c],
                  "full-batch read/write payload bytes differ");
            CHECK(full_err[c] == 0,
                  "full-batch completion channel observed an error response");
        }""",
))

# ---- C14b: T_return / readback comment ------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        const long t_v    = full_final_done[0] - full_t0;
        const long t_a    = full_final_done[1] - full_t0;
        const long t_data = std::max(t_v, t_a);
        const long t_ret  = full_quiet_cycle - full_t0;""",
    r"""        // Every reported time is derived ONLY from pre-edge hardware pulses.
        // T_return is the cycle at which the protocol was quiet (all TL/DMA
        // and CPU lanes idle).  The later register readback used only as a
        // cross-check is NOT included, so its overhead is excluded.
        const long t_v    = full_final_done[0] - full_t0;
        const long t_a    = full_final_done[1] - full_t0;
        const long t_data = std::max(t_v, t_a);
        const long t_ret  = full_quiet_cycle - full_t0;""",
))

# ---- C14c: scenario_budget in the summary ---------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r'''            "\"full_watchdog\":%ld,"
            "\"t0\":%ld,\"T_V\":%ld,\"T_A\":%ld,\"T_data\":%ld,\"T_return\":%ld,"''',
    r'''            "\"full_watchdog\":%ld,\"scenario_budget\":%ld,"
            "\"t0\":%ld,\"T_V\":%ld,\"T_A\":%ld,\"T_data\":%ld,\"T_return\":%ld,"''',
))

EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""            start_delay, v_start_delay, a_start_delay, full_watchdog_cycles,
            full_t0, t_v, t_a, t_data, t_ret,""",
    r"""            start_delay, v_start_delay, a_start_delay, full_watchdog_cycles,
            scenario_budget,
            full_t0, t_v, t_a, t_data, t_ret,""",
))

# ---- C15: cpu_writes terminal block (paired down when cpu_enable=0) -------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        if (cpu_writes) {
            CHECK(top->dbg_cpu_wr_completed  > 0,
                  "synthetic CPU completed no plain writes");
            CHECK(top->dbg_cpu_amo_completed > 0,
                  "synthetic CPU completed no AMOs");
            // issued == completed: every offered AW/W/B/R really retired.
            CHECK(top->dbg_cpu_wr_issued  == top->dbg_cpu_wr_completed,
                  "synthetic CPU issued/completed write mismatch at terminal");
            CHECK(top->dbg_cpu_amo_issued == top->dbg_cpu_amo_completed,
                  "synthetic CPU issued/completed AMO mismatch at terminal");
            CHECK(top->dbg_cpu_rd_err  == 0,
                  "synthetic CPU read received a non-OKAY response");
            CHECK(top->dbg_cpu_wr_err  == 0,
                  "synthetic CPU write received a non-OKAY response");
            CHECK(top->dbg_cpu_amo_err == 0,
                  "synthetic CPU AMO received a non-OKAY response");
            CHECK(n_cpu_amo_obs == (uint64_t)top->dbg_cpu_amo_completed,
                  "AMO R observations do not match completed AMOs");
            // Plain write data / strobe actually landed in DDR.
            CHECK(read64(CPU_WR_ADDR) == CPU_WR_DAT0,
                  "synthetic CPU plain write beat 0 data was not stored");
            CHECK(read64(CPU_WR_ADDR + 8) ==
                      cpu_apply_strb(CPU_WR_INIT1, CPU_WR_DAT1, CPU_WR_STR1),
                  "synthetic CPU plain write beat 1 data/strb was not honoured");
            // AMO accumulation reached the expected final DDR value.
            CHECK(read64(CPU_AMO_ADDR) ==
                      CPU_AMO_INIT +
                      (uint64_t)top->dbg_cpu_amo_completed * CPU_AMO_ADD,
                  "synthetic CPU AMO final DDR value does not match the addend sum");""",
    r"""        if (cpu_writes) {
            if (cpu_enable) {
                CHECK(top->dbg_cpu_wr_completed  > 0,
                      "synthetic CPU completed no plain writes");
                CHECK(top->dbg_cpu_amo_completed > 0,
                      "synthetic CPU completed no AMOs");
                // issued == completed: every offered AW/W/B/R really retired.
                CHECK(top->dbg_cpu_wr_issued  == top->dbg_cpu_wr_completed,
                      "synthetic CPU issued/completed write mismatch at terminal");
                CHECK(top->dbg_cpu_amo_issued == top->dbg_cpu_amo_completed,
                      "synthetic CPU issued/completed AMO mismatch at terminal");
                CHECK(top->dbg_cpu_rd_err  == 0,
                      "synthetic CPU read received a non-OKAY response");
                CHECK(top->dbg_cpu_wr_err  == 0,
                      "synthetic CPU write received a non-OKAY response");
                CHECK(top->dbg_cpu_amo_err == 0,
                      "synthetic CPU AMO received a non-OKAY response");
                CHECK(n_cpu_amo_obs == (uint64_t)top->dbg_cpu_amo_completed,
                      "AMO R observations do not match completed AMOs");
                // Plain write data / strobe actually landed in DDR.
                CHECK(read64(CPU_WR_ADDR) == CPU_WR_DAT0,
                      "synthetic CPU plain write beat 0 data was not stored");
                CHECK(read64(CPU_WR_ADDR + 8) ==
                          cpu_apply_strb(CPU_WR_INIT1, CPU_WR_DAT1, CPU_WR_STR1),
                      "synthetic CPU plain write beat 1 data/strb was not honoured");
                // AMO accumulation reached the expected final DDR value.
                CHECK(read64(CPU_AMO_ADDR) ==
                          CPU_AMO_INIT +
                          (uint64_t)top->dbg_cpu_amo_completed * CPU_AMO_ADD,
                      "synthetic CPU AMO final DDR value does not match the addend sum");
            } else {
                // Runtime-disabled synthetic CPU: the write/AMO lane is
                // required to be PAIREDLY silent -- nothing issued, nothing
                // completed, no response error, and quiet.  The "completed > 0"
                // assertions above only hold when the CPU actually runs.
                CHECK(top->dbg_cpu_wr_issued     == 0 &&
                      top->dbg_cpu_wr_completed  == 0 &&
                      top->dbg_cpu_amo_issued    == 0 &&
                      top->dbg_cpu_amo_completed == 0,
                      "synthetic CPU write/AMO issued traffic while disabled");
                CHECK(top->dbg_cpu_rd_err  == 0 &&
                      top->dbg_cpu_wr_err  == 0 &&
                      top->dbg_cpu_amo_err == 0,
                      "synthetic CPU reported a response error while disabled");
                CHECK(top->dbg_cpu_wr_quiet,
                      "synthetic CPU write lane not quiet at terminal");
                CHECK(n_cpu_amo_obs == 0,
                      "AMO R observed while the synthetic CPU was disabled");
            }""",
))

# ---- C16b: strict parsers before main -------------------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);""",
    r"""// ---------------------------------------------------------------------------
// Strict parsers for the full-batch control plusargs.  They reject empty,
// sign-wrapped, overflowing and trailing-garbage values, so a typo cannot
// silently become 0/1 or an unbounded delay.  The pre-existing a-pages /
// v-gap / transfers / warmup / ptw-latency / a-target parsers are unchanged.
// ---------------------------------------------------------------------------
static bool parse_gate01(const char* key, const char* v, bool& out) {
    if (v == nullptr || *v == '\0') {
        std::fprintf(stderr, "nonblock_actual_dma_test: +%s requires 0|1\n", key);
        return false;
    }
    errno = 0;
    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(v, &end, 0);
    if (errno != 0 || end == v || *end != '\0' || parsed > 1ULL) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +%s must be 0|1 (got '%s')\n",
                     key, v);
        return false;
    }
    out = (parsed != 0);
    return true;
}

static bool parse_ulong(const char* key, const char* v, unsigned long long& out,
                        unsigned long long maxv) {
    if (v == nullptr || *v == '\0') {
        std::fprintf(stderr, "nonblock_actual_dma_test: +%s requires a value\n", key);
        return false;
    }
    errno = 0;
    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(v, &end, 0);
    if (errno != 0 || end == v || *end != '\0' || parsed > maxv) {
        std::fprintf(stderr,
                     "nonblock_actual_dma_test: invalid +%s='%s' "
                     "(unsigned, <= %llu)\n", key, v, maxv);
        return false;
    }
    out = parsed;
    return true;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);""",
))

# ---- C17: strict parsing of the new numeric plusargs ----------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        if (const char* v = val("full-batch"))
            full_batch = (std::strtoul(v, nullptr, 0) != 0);
        if (const char* v = val("cpu-enable")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +cpu-enable='%s' (expected 0|1)\n", v);
                return 2;
            }
            cpu_enable = (parsed != 0);
        }
        if (const char* v = val("start-delay"))
            start_delay = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("v-start-delay"))
            v_start_delay = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("a-start-delay"))
            a_start_delay = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("full-watchdog"))
            full_watchdog = std::strtol(v, nullptr, 0);""",
    r"""        if (const char* v = val("full-batch")) {
            if (!parse_gate01("full-batch", v, full_batch)) return 2;
        }
        if (const char* v = val("cpu-enable")) {
            if (!parse_gate01("cpu-enable", v, cpu_enable)) return 2;
        }
        if (const char* v = val("start-delay")) {
            unsigned long long p = 0;
            if (!parse_ulong("start-delay", v, p, 0xFFFFFFFFULL)) return 2;
            start_delay = (uint32_t)p;
        }
        if (const char* v = val("v-start-delay")) {
            unsigned long long p = 0;
            if (!parse_ulong("v-start-delay", v, p, 0xFFFFFFFFULL)) return 2;
            v_start_delay = (uint32_t)p;
        }
        if (const char* v = val("a-start-delay")) {
            unsigned long long p = 0;
            if (!parse_ulong("a-start-delay", v, p, 0xFFFFFFFFULL)) return 2;
            a_start_delay = (uint32_t)p;
        }
        if (const char* v = val("full-watchdog")) {
            unsigned long long p = 0;
            if (!parse_ulong("full-watchdog", v, p, 0x7FFFFFFFFFFFFFFFULL) || p == 0) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +full-watchdog must be > 0\n");
                return 2;
            }
            full_watchdog = (long)p;
        }""",
))

EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""        if (const char* v = val("cpu-writes")) cpu_writes = (std::strtoul(v, nullptr, 0) != 0);
        if (const char* v = val("ds-priority")) ds_priority = (std::strtoul(v, nullptr, 0) != 0);""",
    r"""        if (const char* v = val("cpu-writes")) {
            if (!parse_gate01("cpu-writes", v, cpu_writes)) return 2;
        }
        if (const char* v = val("ds-priority")) {
            if (!parse_gate01("ds-priority", v, ds_priority)) return 2;
        }""",
))

# ---- C18: full mode rejects an unsupported profile -------------------------
EDITS["tests/nonblock_actual_dma_test.cpp"].append((
    r"""    if (full_batch) {
        if (!(transfers == 64 && a_target == 64ULL && v_gap == 0 &&
              (a_pages == 1 || a_pages == 8) && warmup == 256 && ptw_lat == 31)) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +full-batch requires transfers=64 "
                "a-target=64 v-gap=0 a-pages=1|8 warmup=256 ptw-latency=31\n");
            return 2;
        }
    }
#endif""",
    r"""    if (full_batch) {
        if (!(transfers == 64 && a_target == 64ULL && v_gap == 0 &&
              (a_pages == 1 || a_pages == 8) && warmup == 256 && ptw_lat == 31)) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +full-batch requires transfers=64 "
                "a-target=64 v-gap=0 a-pages=1|8 warmup=256 ptw-latency=31\n");
            return 2;
        }
        // Full-batch does not support the completion-AR proxy profiles.  Fail
        // loudly instead of silently ignoring the request.
        if (read_admit != "none") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +full-batch does not support "
                "+read-admit (got '%s')\n", read_admit.c_str());
            return 2;
        }
    }
#endif""",
))

# ===========================================================================
# tests/test_nonblock_actual_dma.py  (runner)
# ===========================================================================
EDITS["tests/test_nonblock_actual_dma.py"] = []

# ---- P1: None defaults so explicit conflicts can be rejected ---------------
EDITS["tests/test_nonblock_actual_dma.py"].append((
    r"""    ap.add_argument("--v-gap", type=int, default=1000,
                    help="V inter-job gap cycles inside the V batch")
    ap.add_argument("--transfers", type=int, choices=(64, 1024, 4096), default=64,
                    help="V victim 4 KiB jobs")
    ap.add_argument("--warmup", type=int, default=256,
                    help="batch-OFF V transfers issued during warm-up")
    ap.add_argument("--ptw-latency", type=int, choices=(31, 100), default=31,
                    help="model PTW read latency (DDL/DDR model only; NOT MIG)")
    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=1048576,
                    help="A autonomous batch target (default 1048576)")
    ap.add_argument("--full-batch", action="store_true",
                    help="independent full-batch double-natural-completion run "
                         "(SYSTEM only; forces transfers=a-target=64, v-gap=0, "
                         "warmup=256, ptw-latency=31; a-pages must be 1 or 8)")""",
    r"""    ap.add_argument("--v-gap", type=int, default=None,
                    help="V inter-job gap cycles inside the V batch "
                         "(legacy default 1000; full-batch requires 0)")
    ap.add_argument("--transfers", type=int, choices=(64, 1024, 4096), default=None,
                    help="V victim 4 KiB jobs (legacy default 64; full-batch "
                         "requires 64)")
    ap.add_argument("--warmup", type=int, default=None,
                    help="batch-OFF V transfers issued during warm-up "
                         "(legacy default 256; full-batch requires 256)")
    ap.add_argument("--ptw-latency", type=int, choices=(31, 100), default=None,
                    help="model PTW read latency (DDL/DDR model only; NOT MIG) "
                         "(legacy default 31; full-batch requires 31)")
    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=None,
                    help="A autonomous batch target (legacy default 1048576; "
                         "full-batch requires 64)")
    ap.add_argument("--full-batch", action="store_true",
                    help="independent full-batch double-natural-completion run "
                         "(SYSTEM only; requires transfers=a-target=64, v-gap=0, "
                         "warmup=256, ptw-latency=31; a-pages must be 1 or 8). "
                         "An explicitly conflicting value is REJECTED, never "
                         "silently overwritten")""",
))

EDITS["tests/test_nonblock_actual_dma.py"].append((
    r"""    ap.add_argument("--start-delay", type=int, default=0,
                    help="billed cycles between the common t0 and the first NEXT_ID")
    ap.add_argument("--v-start-delay", type=int, default=0,
                    help="bounded cycles before V's NEXT_ID")
    ap.add_argument("--a-start-delay", type=int, default=0,
                    help="bounded cycles before A's NEXT_ID")
    ap.add_argument("--launch-order", choices=("av", "va"), default="av",
                    help="which channel's NEXT_ID is issued first (default av)")
    ap.add_argument("--full-watchdog", type=int, default=200000,
                    help="full-batch per-channel progress watchdog cycles "
                         "(default 200000; a small value is for negative tests)")""",
    r"""    ap.add_argument("--start-delay", type=int, default=None,
                    help="billed cycles between the common t0 and the first "
                         "NEXT_ID (full-batch only)")
    ap.add_argument("--v-start-delay", type=int, default=None,
                    help="bounded cycles before V's NEXT_ID (full-batch only)")
    ap.add_argument("--a-start-delay", type=int, default=None,
                    help="bounded cycles before A's NEXT_ID (full-batch only)")
    ap.add_argument("--launch-order", choices=("av", "va"), default=None,
                    help="which channel's NEXT_ID is issued first (full-batch "
                         "only; default av)")
    ap.add_argument("--full-watchdog", type=int, default=None,
                    help="full-batch per-channel progress watchdog cycles "
                         "(full-batch only; default 200000, a small value is "
                         "for negative tests)")""",
))

# ---- P2: explicit-conflict validation + per-mode defaults -----------------
EDITS["tests/test_nonblock_actual_dma.py"].append((
    r"""    if opts.full_batch:
        if not opts.system:
            ap.error("--full-batch requires --system")
        if opts.a_pages not in (1, 8):
            ap.error("--full-batch requires --a-pages 1 or 8")
        # Force the approved experiment configuration.
        opts.transfers    = 64
        opts.a_target     = 64
        opts.v_gap        = 0
        opts.warmup       = 256
        opts.ptw_latency  = 31
    if not opts.system:""",
    r"""    if opts.full_batch:
        if not opts.system:
            ap.error("--full-batch requires --system")
        if opts.a_pages not in (1, 8):
            ap.error("--full-batch requires --a-pages 1 or 8")
        # An EXPLICIT conflicting value is rejected, never silently
        # overwritten.  Unset (None) knobs take the approved full-batch value.
        if opts.transfers is not None and opts.transfers != 64:
            ap.error("--full-batch requires --transfers 64 (got {})".format(opts.transfers))
        if opts.a_target is not None and opts.a_target != 64:
            ap.error("--full-batch requires --a-target 64 (got {})".format(opts.a_target))
        if opts.v_gap is not None and opts.v_gap != 0:
            ap.error("--full-batch requires --v-gap 0 (got {})".format(opts.v_gap))
        if opts.warmup is not None and opts.warmup != 256:
            ap.error("--full-batch requires --warmup 256 (got {})".format(opts.warmup))
        if opts.ptw_latency is not None and opts.ptw_latency != 31:
            ap.error("--full-batch requires --ptw-latency 31 (got {})".format(opts.ptw_latency))
        if opts.read_admit is not None and opts.read_admit != "none":
            ap.error("--full-batch does not support --read-admit")
        opts.transfers    = 64
        opts.a_target     = 64
        opts.v_gap        = 0
        opts.warmup       = 256
        opts.ptw_latency  = 31
        opts.start_delay   = 0      if opts.start_delay   is None else opts.start_delay
        opts.v_start_delay = 0      if opts.v_start_delay is None else opts.v_start_delay
        opts.a_start_delay = 0      if opts.a_start_delay is None else opts.a_start_delay
        opts.launch_order  = "av"   if opts.launch_order  is None else opts.launch_order
        opts.full_watchdog = 200000 if opts.full_watchdog is None else opts.full_watchdog
    else:
        # Legacy defaults.  Full-batch-only knobs must be left at their
        # defaults; an explicit non-default value is REJECTED rather than
        # silently ignored.
        opts.transfers   = 64      if opts.transfers   is None else opts.transfers
        opts.a_target    = 1048576 if opts.a_target    is None else opts.a_target
        opts.v_gap       = 1000    if opts.v_gap       is None else opts.v_gap
        opts.warmup      = 256     if opts.warmup      is None else opts.warmup
        opts.ptw_latency = 31      if opts.ptw_latency is None else opts.ptw_latency
        if (opts.start_delay is not None or opts.v_start_delay is not None or
                opts.a_start_delay is not None or opts.launch_order is not None or
                opts.full_watchdog is not None):
            ap.error("--start-delay/--v-start-delay/--a-start-delay/"
                     "--launch-order/--full-watchdog require --full-batch")
        opts.start_delay   = 0
        opts.v_start_delay = 0
        opts.a_start_delay = 0
        opts.launch_order  = "av"
        opts.full_watchdog = 200000

    if not opts.system:""",
))

# Explicitly unchanged (documented, not edited here):
EDITS["tests/nonblock_actual_dma_system_top.sv"] = []


def apply_edits(dry_run: bool, check: bool) -> int:
    if not ROOT.is_dir():
        sys.stderr.write("ROOT not found: {} (run from the platform directory)\n"
                         .format(ROOT))
        return 2
    rc = 0
    for rel, hunks in EDITS.items():
        path = ROOT / rel
        if not path.is_file():
            sys.stderr.write("missing proposed file: {}\n".format(path))
            rc = 2
            continue
        if not hunks:
            print("UNCHANGED  {} (no edits for this fix set)".format(rel))
            continue
        text = path.read_text()
        changed = 0
        for n, (old, new) in enumerate(hunks, 1):
            if old in text:
                if check:
                    print("WOULD-CHANGE {} hunk {}".format(rel, n))
                text = text.replace(old, new, 1)
                changed += 1
            elif new in text:
                print("ALREADY-APPLIED {} hunk {}".format(rel, n))
            else:
                sys.stderr.write("NO-MATCH {} hunk {}\n".format(rel, n))
                rc = 2
        if changed and not (dry_run or check):
            path.write_text(text)
        print("{}  {} hunks applied{}{}".format(
            rel, changed,
            " (dry-run)" if dry_run else "",
            " (check)" if check else ""))
    return rc


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--dry-run", action="store_true",
                    help="report matches without writing")
    ap.add_argument("--check", action="store_true",
                    help="report matches without writing (alias of --dry-run)")
    opts = ap.parse_args()
    return apply_edits(opts.dry_run, opts.check)


if __name__ == "__main__":
    sys.exit(main())
```

---

## Code block 2 — `tests/test_full_batch_contract.py`

Independent acceptance checker. It only *validates* a record; it never emits a passing record as a performance result, and its `--selftest` proves that hand-edited records are rejected.

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Independent acceptance checker for the full-batch contract.

CONTRACT CHECKER ONLY -- NOT A BENCHMARK.  It never produces, derives or
endorses a performance number.  It decides whether ONE
ACTUAL-DMA-FULL-BATCH-JSON record (optionally cross-checked against the
matching +event-trace CSV) is internally consistent.  A record that was
hand-edited or synthesised must fail here, and a record is only usable as
evidence if it was emitted by a real run AND is accepted by this tool.

Checked, independently of the C++ harness bookkeeping:
  * per-channel counts:  issue == done == target
  * ordering:            ready[0] == t0; ready[j] == done[j-1];
                         ready <= issue <= done; issue and done strictly
                         increasing; issue[j] >= done[j-1] (single active)
  * payload conservation: r_beats*8 == r_bytes == rd_bytes == target*4096,
                         wr_bytes == target*4096, aw == b, err == 0
  * registers:           reg_issued == reg_done == target, batch_complete,
                         not batch_stopped
  * quiet == 1, T_V >= 0, T_A >= 0, T_data == max(T_V, T_A), T_return >= T_data
  * scenario_budget present and positive
  * (optional) the event trace's full_dma_issue / full_dma_done pulse counts
    match the JSON issue/done counts bit for bit.

Usage
-----
    python3 tests/test_full_batch_contract.py <log-with-full-json> \
        [--trace <event-trace.csv>] [--target 64]

    # negative-test plan against a REAL successful record:
    python3 tests/test_full_batch_contract.py --selftest <real-success.log>
"""

import argparse
import copy
import json
import sys
from pathlib import Path

SCHEMA      = "actual-dma-full-batch-v1"
PAGE_BYTES  = 4096
FULL_TARGET = 64
CHANNEL_IDS = ("V", "A")


# ---------------------------------------------------------------------------
# parsing
# ---------------------------------------------------------------------------
def load_records(text):
    """Return every ACTUAL-DMA-FULL-BATCH-JSON record in `text`."""
    recs = []
    for line in text.splitlines():
        line = line.strip()
        if line.startswith("ACTUAL-DMA-FULL-BATCH-JSON "):
            recs.append(json.loads(line.split(" ", 1)[1]))
    return recs


def load_first_record(path):
    recs = load_records(Path(path).read_text(errors="replace"))
    if not recs:
        raise SystemExit("no ACTUAL-DMA-FULL-BATCH-JSON record in {}".format(path))
    return recs[0]


def count_trace_pulses(path):
    """Count full_dma_issue / full_dma_done tag bits from the CSV trace."""
    v_iss = a_iss = v_done = a_done = 0
    for line in Path(path).read_text(errors="replace").splitlines():
        parts = line.rstrip("\n").split(",")
        if len(parts) != 5:
            continue
        _cyc, ev, tag, _addr, _val = parts
        try:
            mask = int(tag)
        except ValueError:
            continue
        if ev == "full_dma_issue":
            if mask & 1:
                v_iss += 1
            if mask & 2:
                a_iss += 1
        elif ev == "full_dma_done":
            if mask & 1:
                v_done += 1
            if mask & 2:
                a_done += 1
    return v_iss, a_iss, v_done, a_done


# ---------------------------------------------------------------------------
# checks
# ---------------------------------------------------------------------------
def _need(cond, msg, fails):
    if not cond:
        fails.append(msg)


def check_channel(chan, target, fails):
    tag = chan.get("id", "?")
    ready = chan.get("ready")
    issue = chan.get("issue_cyc")
    done = chan.get("done_cyc")

    _need(chan.get("issue") == target,
          "{}: issue {} != {}".format(tag, chan.get("issue"), target), fails)
    _need(chan.get("done") == target,
          "{}: done {} != {}".format(tag, chan.get("done"), target), fails)

    if not (isinstance(ready, list) and isinstance(issue, list) and
            isinstance(done, list)):
        fails.append("{}: ready/issue_cyc/done_cyc must be arrays".format(tag))
        return

    _need(len(ready) == target,
          "{}: ready array len {} != {}".format(tag, len(ready), target), fails)
    _need(len(issue) == target,
          "{}: issue array len {} != {}".format(tag, len(issue), target), fails)
    _need(len(done) == target,
          "{}: done array len {} != {}".format(tag, len(done), target), fails)
    if not (len(ready) == len(issue) == len(done) == target):
        return

    _need(ready[0] == issue[0] or ready[0] >= 0,
          "{}: ready[0] must be a real cycle".format(tag), fails)
    for j in range(target):
        _need(ready[j] <= issue[j] <= done[j],
              "{}: order ready[{}]<=issue[{}]<=done[{}] violated".format(
                  tag, j, j, j), fails)
    for j in range(1, target):
        _need(ready[j] == done[j - 1],
              "{}: ready[{}] {} != done[{}] {}".format(
                  tag, j, ready[j], j - 1, done[j - 1]), fails)
        _need(issue[j] >= done[j - 1],
              "{}: issue[{}] precedes done[{}] (single-active)".format(
                  tag, j, j - 1), fails)
        _need(issue[j] > issue[j - 1],
              "{}: issue times not strictly increasing at {}".format(tag, j), fails)
        _need(done[j] > done[j - 1],
              "{}: done times not strictly increasing at {}".format(tag, j), fails)

    _need(chan.get("r_beats", 0) * 8 == chan.get("r_bytes"),
          "{}: r_beats*8 != r_bytes".format(tag), fails)
    _need(chan.get("rd_bytes") == target * PAGE_BYTES,
          "{}: rd_bytes {} != {}".format(tag, chan.get("rd_bytes"),
                                         target * PAGE_BYTES), fails)
    _need(chan.get("wr_bytes") == target * PAGE_BYTES,
          "{}: wr_bytes {} != {}".format(tag, chan.get("wr_bytes"),
                                         target * PAGE_BYTES), fails)
    _need(chan.get("r_bytes") == chan.get("rd_bytes"),
          "{}: r_bytes != rd_bytes".format(tag), fails)
    _need(chan.get("b") == chan.get("aw"),
          "{}: b {} != aw {}".format(tag, chan.get("b"), chan.get("aw")), fails)
    _need(chan.get("ar", 0) > 0 and chan.get("aw", 0) > 0,
          "{}: no AR/AW traffic observed".format(tag), fails)
    _need(chan.get("err") == 0,
          "{}: err {} != 0".format(tag, chan.get("err")), fails)
    _need(chan.get("reg_issued") == target,
          "{}: reg_issued {} != {}".format(tag, chan.get("reg_issued"), target), fails)
    _need(chan.get("reg_done") == target,
          "{}: reg_done {} != {}".format(tag, chan.get("reg_done"), target), fails)
    _need(chan.get("batch_complete") == 1,
          "{}: batch not complete".format(tag), fails)
    _need(chan.get("batch_stopped") == 0,
          "{}: batch reported STOP".format(tag), fails)


def check_record(rec, target=FULL_TARGET):
    """Return a (possibly empty) list of contract violations."""
    fails = []
    _need(rec.get("schema") == SCHEMA,
          "unexpected schema {!r}".format(rec.get("schema")), fails)
    _need(rec.get("quiet") == 1, "quiet flag is not 1", fails)

    chans = rec.get("channels")
    if not (isinstance(chans, list) and len(chans) == 2):
        fails.append("expected exactly two channels")
        return fails
    by_id = {}
    for c in chans:
        by_id[c.get("id")] = c
    if set(by_id) != set(CHANNEL_IDS):
        fails.append("channel ids are not V/A")
        return fails

    check_channel(by_id["V"], target, fails)
    check_channel(by_id["A"], target, fails)

    _need(rec.get("T_V", -1) >= 0, "T_V is negative", fails)
    _need(rec.get("T_A", -1) >= 0, "T_A is negative", fails)
    _need(rec.get("T_data") == max(rec.get("T_V", -1), rec.get("T_A", -1)),
          "T_data != max(T_V,T_A)", fails)
    _need(rec.get("T_return", -1) >= rec.get("T_data", -1),
          "T_return < T_data", fails)

    sb = rec.get("scenario_budget")
    _need(isinstance(sb, (int, float)) and sb > 0,
          "scenario_budget missing or <= 0", fails)
    return fails


def check_trace(rec, trace_path, target=FULL_TARGET, fails=None):
    fails = [] if fails is None else fails
    v_iss, a_iss, v_done, a_done = count_trace_pulses(trace_path)
    by_id = {c.get("id"): c for c in rec.get("channels", [])}
    if "V" in by_id:
        _need(v_iss == target,
              "trace: V issue pulses {} != {}".format(v_iss, target), fails)
        _need(v_done == target,
              "trace: V done pulses {} != {}".format(v_done, target), fails)
        _need(v_iss == by_id["V"].get("issue") and
              v_done == by_id["V"].get("done"),
              "trace: V pulse counts disagree with the JSON record", fails)
    if "A" in by_id:
        _need(a_iss == target,
              "trace: A issue pulses {} != {}".format(a_iss, target), fails)
        _need(a_done == target,
              "trace: A done pulses {} != {}".format(a_done, target), fails)
        _need(a_iss == by_id["A"].get("issue") and
              a_done == by_id["A"].get("done"),
              "trace: A pulse counts disagree with the JSON record", fails)
    return fails


# ---------------------------------------------------------------------------
# negative-test plan: mutate a REAL successful record; every mutant must FAIL
# ---------------------------------------------------------------------------
def _mutations(rec):
    cases = []

    def add(name, mut):
        m = copy.deepcopy(rec)
        mut(m)
        cases.append((name, m))

    add("drop_last_V_done",
        lambda r: r["channels"][0]["done_cyc"].pop())
    add("add_extra_A_done",
        lambda r: r["channels"][1]["done_cyc"].append(
            r["channels"][1]["done_cyc"][-1] + 1))
    add("duplicate_V_done_timestamp",
        lambda r: r["channels"][0]["done_cyc"].__setitem__(
            3, r["channels"][0]["done_cyc"][2]))
    add("break_V_ready_alignment",
        lambda r: r["channels"][0]["ready"].__setitem__(
            2, r["channels"][0]["ready"][2] + 1))
    add("swap_V_issue_order",
        lambda r: r["channels"][0]["issue_cyc"].__setitem__(
            4, r["channels"][0]["issue_cyc"][3] - 1))
    add("bump_V_rd_bytes",
        lambda r: r["channels"][0].__setitem__(
            "rd_bytes", r["channels"][0]["rd_bytes"] + 8))
    add("bump_A_wr_bytes",
        lambda r: r["channels"][1].__setitem__(
            "wr_bytes", r["channels"][1]["wr_bytes"] + 64))
    add("break_V_aw_b_conservation",
        lambda r: r["channels"][0].__setitem__("b", r["channels"][0]["b"] - 1))
    add("set_A_error",
        lambda r: r["channels"][1].__setitem__("err", 1))
    add("clear_quiet",
        lambda r: r.__setitem__("quiet", 0))
    add("zero_scenario_budget",
        lambda r: r.__setitem__("scenario_budget", 0))
    add("shrink_reg_done",
        lambda r: r["channels"][0].__setitem__(
            "reg_done", r["channels"][0]["reg_done"] - 1))
    return cases


def selftest(path, target=FULL_TARGET):
    rec = load_first_record(path)
    base = check_record(rec, target)
    if base:
        print("REFERENCE RECORD IS ALREADY INVALID:")
        for f in base:
            print("  " + f)
        return 1
    print("reference record accepted ({} channels, target {})".format(
        len(rec.get("channels", [])), target))
    bad = 0
    for name, mutant in _mutations(rec):
        fails = check_record(mutant, target)
        if fails:
            print("REJECT  {}".format(name))
        else:
            print("ACCEPTED(!) {}".format(name))
            bad += 1
    if bad:
        print("{} mutant(s) NOT rejected -- checker is too weak".format(bad))
        return 1
    print("all {} mutants rejected".format(len(_mutations(rec))))
    return 0


# ---------------------------------------------------------------------------
def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("log", nargs="?", default=None,
                    help="file containing an ACTUAL-DMA-FULL-BATCH-JSON record")
    ap.add_argument("--trace", default=None,
                    help="optional +event-trace CSV to cross-check pulse counts")
    ap.add_argument("--target", type=int, default=FULL_TARGET,
                    help="per-channel job target (default 64)")
    ap.add_argument("--selftest", metavar="REAL_LOG", default=None,
                    help="load a REAL successful record and require every "
                         "mutation of it to be rejected")
    opts = ap.parse_args(argv)

    if opts.selftest:
        return selftest(opts.selftest, opts.target)

    if not opts.log:
        ap.error("provide a log path or --selftest REAL_LOG")
    rec = load_first_record(opts.log)
    fails = check_record(rec, opts.target)
    if opts.trace:
        fails = check_trace(rec, opts.trace, opts.target, fails)

    if fails:
        print("FULL-BATCH CONTRACT: REJECT")
        for f in fails:
            print("  " + f)
        return 1
    print("FULL-BATCH CONTRACT: ACCEPT "
          "(counts/order/ready/bytes/B/quiet/errors all consistent)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

---

## Assumptions

1. **Single-active is literal.** Edit C1 requires `full_issue_n == full_done_n` at each new ISSUE (exactly one job in flight per channel). If the DUT legitimately pipelines two jobs, relax to `full_issue_n[c] - full_done_n[c] <= 1`. This is directly requested by item 1.
2. **`batch_ctl` enable stays 1 after a natural completion.** Edit C13 `CHECK(full_reg_enable[...])` and `CHECK(!full_reg_active[...])` follow item 2's "inactive/enabled state check". If the RTL auto-clears `enable` on completion, drop only that one `CHECK` (keep `!active`).
3. **DS priority keeps the held-R guard (`!ddr_r_held`).** Item 3's literal expression is `ds_priority && (full_batch ? (full_era && full_done_n[0]<64) : win_open)`. I kept `!ddr_r_held` because item 3 also says "禁止 held 修改" — a beat held from the previous tick must not be displaced. Removing it would break AXI payload stability. Flagged for root review.
4. **`v_transfers` == 64 in full mode** (the runner and C++ both pin this), so `full_done_n[0] < v_transfers` is the requested `full_done_n[0] < 64`.
5. **Payload maps:** read payload must be in `V_SRC_PA` (0x8000_0000, one page) or an A ring source page (`A_SRC_PA_BASE .. +a_pages*4K`); write payload must be in `[V_DST_PA, V_COLD_DST_PA+4K)` or an A ring destination page. `V_DST_PA_LO/HI` are new file-scope constants derived from existing ones.
6. **SV untouched** — no item in this list requires an RTL wrapper change.
7. Old parsers (`a-pages`, `v-gap`, `transfers`, `warmup`, `ptw-latency`, `a-target`, `read-*`) are intentionally left alone per item 6.

## Unresolved / needs root verification

- The `enable==1` readback check (assumption 2).
- The single-active strictness (assumption 1).
- The `!ddr_r_held` retention choice (assumption 3).
- `full_wr_bytes == 262144ULL` assumes full strobes on every completion W beat (512×8 B per job).

## Suggested acceptance checks

1. `python3 apply_full_batch_fixes.py --dry-run` — must report every hunk as `WOULD-CHANGE` and exit 0 (no `NO-MATCH`).
2. Rebuild SYSTEM full-batch: `python3 tests/test_nonblock_actual_dma.py --system --full-batch --cpu-enable 1` → PASS, `ACTUAL-DMA-FULL-BATCH-JSON` contains `"scenario_budget":` and `full_err` 0.
3. `python3 tests/test_nonblock_actual_dma.py --system --full-batch --cpu-enable 0` → PASS (paired zero assertions).
4. Conflict rejection: `--system --full-batch --transfers 4096` → argparse error; `--transfers 64 --launch-order va` (no `--full-batch`) → argparse error; `+full-batch=2` / `+start-delay=-1` / `+full-watchdog=abc` → C++ exit 2.
5. Verifier: `python3 tests/test_full_batch_contract.py <success.log> --trace <trace.csv>` → ACCEPT; `--selftest <success.log>` → all mutants REJECT.
6. Targeted watchdog unit test: call `full_watchdog_fault(1000, 0, false, 500, 200)` → true; `(200, 0, false, 500, 200)` → false; `(300, 200, true, 500, 200)` → true; `(250, 200, true, 500, 200)` → false.