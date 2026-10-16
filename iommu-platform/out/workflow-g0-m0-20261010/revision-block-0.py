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
