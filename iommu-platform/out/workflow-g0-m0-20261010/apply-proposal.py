#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#
# apply_full_batch.py
#
# Precise, idempotency-checked replacements for the approved full-batch work:
#   tests/nonblock_actual_dma_test.cpp
#   tests/nonblock_actual_dma_system_top.sv
#   tests/test_nonblock_actual_dma.py
#
# Every replacement asserts that its "old" fragment occurs EXACTLY ONCE in the
# current file text before it is applied, so a wrong file / already-patched file
# aborts instead of corrupting the tree. No production RTL is touched.
#
# Run from anywhere inside the repo:
#   python3 apply_full_batch.py
#
from pathlib import Path

# --------------------------------------------------------------------------
def find_root() -> Path:
    here = Path(__file__).resolve().parent
    for cand in [here, *here.parents]:
        if (cand / "tests" / "nonblock_actual_dma_test.cpp").is_file():
            return cand
    raise SystemExit("repo root not found (tests/nonblock_actual_dma_test.cpp)")

ROOT = find_root()

def sub(text: str, old: str, new: str, label: str) -> str:
    n = text.count(old)
    if n != 1:
        raise SystemExit(
            "ANCHOR NOT UNIQUE ({} hits) for [{}]; refusing to edit".format(n, label))
    return text.replace(old, new, 1)

def read(p: Path) -> str:
    return p.read_text()

def write(p: Path, t: str) -> None:
    p.write_text(t)

# ==========================================================================
# File 1: tests/nonblock_actual_dma_test.cpp
# ==========================================================================
def patch_cpp() -> None:
    p = ROOT / "tests" / "nonblock_actual_dma_test.cpp"
    t = read(p)

    # ---- (C1) main-scope option variables --------------------------------
    t = sub(
        t,
        r"""    bool        v_cold_dst = false;
    bool        owner_only = false;
""",
        r"""    bool        v_cold_dst = false;
    bool        owner_only = false;
    // ---- full-batch independent mode (default OFF) ----------------------
    bool        full_batch    = false;
    bool        cpu_enable    = true;
    uint32_t    start_delay   = 0;
    uint32_t    v_start_delay = 0;
    uint32_t    a_start_delay = 0;
    std::string launch_order  = "av";
    long        full_watchdog = 200000L;
""",
        "C1 main option vars")

    # ---- (C2) Tb full-batch run-time knobs -------------------------------
    t = sub(
        t,
        r"""    uint32_t warmup            = 256;
    uint64_t a_target          = 1048576ULL;   // or 0x100000000ULL
""",
        r"""    uint32_t warmup            = 256;
    uint64_t a_target          = 1048576ULL;   // or 0x100000000ULL
    // ---- full-batch independent mode (SYSTEM; default OFF) --------------
    bool     full_batch    = false;
    // Runtime CPU enable.  Drives cpu_enable_i on the SYSTEM top only; the
    // non-SYSTEM top has no such port and ignores it.
    bool     cpu_enable    = true;
    // Billed common delay after the shared t0 and before the first NEXT_ID.
    uint32_t start_delay   = 0;
    // Bounded per-channel delays before the corresponding NEXT_ID.
    uint32_t v_start_delay = 0;
    uint32_t a_start_delay = 0;
    // "av" | "va": which channel's NEXT_ID is issued first.
    std::string launch_order = "av";
    // Full-batch per-channel progress watchdog (cycles).
    long     full_watchdog_cycles = 200000L;
""",
        "C2 Tb full-batch knobs")

    # ---- (C3) Tb full-batch measurement state ----------------------------
    t = sub(
        t,
        r"""    // Debug-only reset baselines.  DONE waits use the NEXT_ID captured by
    // trigger(), never these values.
    uint64_t v_done_last = 0, a_done_last = 0;
""",
        r"""    // ---------------------------------------------------------------------
    // FULL-BATCH independent mode state (SYSTEM-only consumers; declared
    // unconditionally so the non-SYSTEM build still compiles).  Default OFF.
    // Every count below is accumulated from PRE-EDGE hardware samples.
    // ---------------------------------------------------------------------
    bool     full_era          = false;
    long     full_t0           = 0;
    long     full_quiet_cycle  = 0;
    long     full_last_progress[2] = {0, 0};
    uint32_t full_issue_n[2]   = {0, 0};
    uint32_t full_done_n[2]    = {0, 0};
    bool     full_issue_seen[2]= {false, false};
    long     full_first_issue[2] = {0, 0};
    long     full_final_done[2]  = {0, 0};
    std::vector<long> full_ready_cyc[2];
    std::vector<long> full_issue_cyc[2];
    std::vector<long> full_done_cyc[2];
    uint64_t full_ar_n[2] = {0, 0};
    uint64_t full_aw_n[2] = {0, 0};
    uint64_t full_rd_bytes[2] = {0, 0};
    uint64_t full_wr_bytes[2] = {0, 0};
    uint64_t full_r_n[2] = {0, 0};
    uint64_t full_r_return_bytes[2] = {0, 0};
    uint64_t full_b_n[2] = {0, 0};
    uint64_t full_err[2] = {0, 0};
    uint64_t full_reg_issued[2] = {0, 0};
    uint64_t full_reg_done[2]   = {0, 0};
    bool     full_reg_complete[2] = {false, false};
    bool     full_reg_stopped[2]  = {false, false};
    bool     full_reg_active[2]   = {false, false};
    bool     full_reg_enable[2]   = {false, false};
    // full-era raw classifiers (G0 explanation only; raw, not de-biased)
    uint64_t full_tl_accept = 0, full_tl_resp = 0;
    uint64_t full_iotlb_lookup = 0, full_iotlb_hit = 0;
    uint64_t full_ptw_active = 0, full_ptw_r_fire = 0;
    uint64_t full_fast = 0;
    uint64_t full_comp_ar = 0, full_comp_aw = 0, full_comp_w = 0;
    uint64_t full_ddtw_ar = 0, full_ptw_ar = 0;
    bool     full_trace_ptw_active = false;

    // Debug-only reset baselines.  DONE waits use the NEXT_ID captured by
    // trigger(), never these values.
    uint64_t v_done_last = 0, a_done_last = 0;
""",
        "C3 full-batch state")

    # ---- (C4) tick(): drive the runtime cpu_enable_i ---------------------
    t = sub(
        t,
        r"""#ifdef ACTUAL_DMA_SYSTEM
        top->read_admit_mode_i =
            (read_admit_measure_only && !measurement_phase) ? 1 : read_admit_mode_val;
        top->read_budget_i     = read_budget;
        top->read_period_i     = read_period;
#endif
""",
        r"""#ifdef ACTUAL_DMA_SYSTEM
        // Runtime CPU enable: a real input, not a CpuIssueInterval==0 fake.
        top->cpu_enable_i   = cpu_enable ? 1 : 0;
        top->read_admit_mode_i =
            (read_admit_measure_only && !measurement_phase) ? 1 : read_admit_mode_val;
        top->read_budget_i     = read_budget;
        top->read_period_i     = read_period;
#endif
""",
        "C4 drive cpu_enable_i")

    # ---- (C5) tick(): full-era per-copy recording + counters + full trace -
    t = sub(
        t,
        r"""        if (top->dbg_tl_ptw_active) ++n_ptw_active;
        if (top->dbg_tl_iotlb_lookup) {
            ++n_iotlb_lookup;
            if (top->dbg_tl_iotlb_hit) ++n_iotlb_hit;
        }
""",
        r"""        if (top->dbg_tl_ptw_active) ++n_ptw_active;
        if (top->dbg_tl_iotlb_lookup) {
            ++n_iotlb_lookup;
            if (top->dbg_tl_iotlb_hit) ++n_iotlb_hit;
        }

#ifdef ACTUAL_DMA_SYSTEM
        // ---- full-batch per-copy recording + raw era counters -----------
        // All values are PRE-EDGE samples.  DONE is processed before ISSUE so
        // a back-to-back same-cycle done->issue keeps ready[j] == done[j-1].
        if (full_era) {
            const uint32_t fi = (uint32_t)top->dbg_dma_issue & 3u;
            const uint32_t fd = (uint32_t)top->dbg_dma_done  & 3u;
            for (int c = 0; c < 2; ++c) {
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
            }
            if (top->dbg_tl_req_valid && top->dbg_tl_req_ready) ++full_tl_accept;
            if (top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) ++full_tl_resp;
            if (top->dbg_tl_ptw_active) ++full_ptw_active;
            if (top->dbg_tl_ptw_r_fire) ++full_ptw_r_fire;
            if (top->dbg_tl_iotlb_lookup) {
                ++full_iotlb_lookup;
                if (top->dbg_tl_iotlb_hit) ++full_iotlb_hit;
            }
            if (top->dbg_fast_resp_valid && top->dbg_fast_resp_ready) ++full_fast;
            if (top->dbg_comp_ar_fire) ++full_comp_ar;
            if (top->dbg_comp_aw_fire) ++full_comp_aw;
            if (top->dbg_comp_w_fire)  ++full_comp_w;
            if (top->dbg_ddtw_ar_fire) ++full_ddtw_ar;
            if (top->dbg_ptw_ar_fire)  ++full_ptw_ar;
        }

        // ---- full-batch trace (independent of win_open) ----------------
        // Event-driven only: real hardware issue/done pulses plus the existing
        // payload / PTW / DDR diagnostics over the WHOLE t0..quiet era, so the
        // full path is visible even when no legacy measurement window is open.
        if (full_era && event_trace) {
            const uint32_t fissue = (uint32_t)top->dbg_dma_issue & 3u;
            const uint32_t fdone  = (uint32_t)top->dbg_dma_done  & 3u;
            if (fissue)
                std::fprintf(event_trace, "%ld,full_dma_issue,%u,0,0\n",
                             g_cycle - full_t0, fissue);
            if (fdone)
                std::fprintf(event_trace, "%ld,full_dma_done,%u,0,0\n",
                             g_cycle - full_t0, fdone);
            if (f_ddr_ar)
                std::fprintf(event_trace, "%ld,full_ddr_ar,%u,%llu,%u\n",
                             g_cycle - full_t0, s_ddr_ar_i,
                             (unsigned long long)s_ddr_ar_a, s_ddr_ar_l);
            if (f_ddr_aw)
                std::fprintf(event_trace, "%ld,full_ddr_aw,%u,%llu,%u\n",
                             g_cycle - full_t0, s_ddr_aw_i,
                             (unsigned long long)s_ddr_aw_a, s_ddr_aw_l);
            if (f_ddr_w)
                std::fprintf(event_trace, "%ld,full_ddr_w,%u,%llu,%u\n",
                             g_cycle - full_t0, (unsigned)s_ddr_w_l,
                             (unsigned long long)s_ddr_w_d, (unsigned)s_ddr_w_s);
            if (f_ddr_b)
                std::fprintf(event_trace, "%ld,full_ddr_b,%u,0,0\n",
                             g_cycle - full_t0, (unsigned)top->ddr_b_id);
            if (f_ddr_r && !mem_q.empty()) {
                const MemRead& r = mem_q.front();
                std::fprintf(event_trace, "%ld,full_ddr_r,%u,%llu,%u\n",
                             g_cycle - full_t0, (unsigned)r.id,
                             (unsigned long long)r.addr, (unsigned)r.beat);
            }
            if (top->dbg_tl_ptw_active != full_trace_ptw_active) {
                std::fprintf(event_trace, "%ld,full_ptw_%s,%u,0,0\n",
                             g_cycle - full_t0,
                             top->dbg_tl_ptw_active ? "start" : "end",
                             (unsigned)top->dbg_main_bank);
                full_trace_ptw_active = top->dbg_tl_ptw_active;
            }
            if (top->dbg_iotlb_fill)
                std::fprintf(event_trace, "%ld,full_fill,%u,0,0\n",
                             g_cycle - full_t0, (unsigned)top->dbg_main_bank);
            if (top->dbg_ptw_ar_fire)
                std::fprintf(event_trace, "%ld,full_ptw_ar,%u,0,0\n",
                             g_cycle - full_t0, (unsigned)top->dbg_main_bank);
            if (top->dbg_tl_ptw_r_fire)
                std::fprintf(event_trace, "%ld,full_ptw_r,%u,0,0\n",
                             g_cycle - full_t0, (unsigned)top->dbg_main_bank);
            if (top->dbg_fast_resp_valid && top->dbg_fast_resp_ready)
                std::fprintf(event_trace, "%ld,full_fast,%u,0,0\n",
                             g_cycle - full_t0, (unsigned)top->dbg_fast_resp_bank);
        }
#endif
""",
        "C5 full-era recording + trace")

    # ---- (C6) rise edge: comp AR payload bytes ---------------------------
    t = sub(
        t,
        r"""        if (f_ddr_ar) {
            ++n_ddr_ar;
            ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
            MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;
""",
        r"""        if (f_ddr_ar) {
            ++n_ddr_ar;
            ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
            // full-batch: real payload read bytes for the completion master
            // (slave port 2).  DS walk (3) and CPU (0) are excluded.
            if (full_era && (((s_ddr_ar_i >> 4) & 3u) == 2u)) {
                const unsigned ch = (unsigned)((s_ddr_ar_i >> 2) & 1u);
                ++full_ar_n[ch];
                full_rd_bytes[ch] += ((uint64_t)s_ddr_ar_l + 1ULL) * 8ULL;
            }
            MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;
""",
        "C6 comp AR bytes")

    # ---- (C7) rise edge: comp W strobe bytes -----------------------------
    t = sub(
        t,
        r"""        if (f_ddr_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);
""",
        r"""        if (f_ddr_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            // full-batch: strobe-weighted write payload bytes, attributed by
            // the pending AW id (slave port 2 only).
            if (full_era && (((c.id >> 4) & 3u) == 2u)) {
                const unsigned ch = (unsigned)((c.id >> 2) & 1u);
                unsigned bits = 0;
                for (int i = 0; i < 8; ++i)
                    if (s_ddr_w_s & (uint8_t)(1u << i)) ++bits;
                full_wr_bytes[ch] += bits;
            }
            write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);
""",
        "C7 comp W bytes")

    # ---- (C8) rise edge: comp B responses --------------------------------
    t = sub(
        t,
        r"""        if (f_ddr_b) {
            ++n_ddr_b;
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            cb_q.pop_front();
        }
""",
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
        }
""",
        "C8 comp B conservation")

    # ---- (C9) rise edge: comp R return beats -----------------------------
    t = sub(
        t,
        r"""        if (f_ddr_r) {
            ++n_ddr_r;
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }
""",
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
        }
""",
        "C9 comp R conservation")

    # ---- (C10) reset(): clear full-batch accumulation --------------------
    t = sub(
        t,
        r"""        trace_ptw_active = false;
        trace_aw_flags = 0;
""",
        r"""        trace_ptw_active = false;
        trace_aw_flags = 0;
        // Full-batch state is (re)initialised by full_measure_begin(); clear
        // the accumulation flags so repeated scenarios are well defined.
        full_era = false;
        full_t0 = 0;
        full_quiet_cycle = 0;
        full_last_progress[0] = full_last_progress[1] = g_cycle;
        full_issue_n[0] = full_issue_n[1] = 0;
        full_done_n[0]  = full_done_n[1]  = 0;
        full_issue_seen[0] = full_issue_seen[1] = false;
        full_first_issue[0] = full_first_issue[1] = 0;
        full_final_done[0]  = full_final_done[1]  = 0;
        full_ready_cyc[0].clear(); full_ready_cyc[1].clear();
        full_issue_cyc[0].clear(); full_issue_cyc[1].clear();
        full_done_cyc[0].clear();  full_done_cyc[1].clear();
        full_ar_n[0] = full_ar_n[1] = 0;
        full_aw_n[0] = full_aw_n[1] = 0;
        full_rd_bytes[0] = full_rd_bytes[1] = 0;
        full_wr_bytes[0] = full_wr_bytes[1] = 0;
        full_r_n[0] = full_r_n[1] = 0;
        full_r_return_bytes[0] = full_r_return_bytes[1] = 0;
        full_b_n[0] = full_b_n[1] = 0;
        full_err[0] = full_err[1] = 0;
        full_reg_issued[0] = full_reg_issued[1] = 0;
        full_reg_done[0]   = full_reg_done[1]   = 0;
        full_reg_complete[0] = full_reg_complete[1] = false;
        full_reg_stopped[0]  = full_reg_stopped[1]  = false;
        full_tl_accept = full_tl_resp = 0;
        full_iotlb_lookup = full_iotlb_hit = 0;
        full_ptw_active = full_ptw_r_fire = 0;
        full_fast = 0;
        full_comp_ar = full_comp_aw = full_comp_w = 0;
        full_ddtw_ar = full_ptw_ar = 0;
        full_trace_ptw_active = false;
""",
        "C10 reset clears full state")

    # ---- (C11) run_scenario(): branch legacy vs full ---------------------
    t = sub(
        t,
        r"""        // ---- 3. A NEXT_ID then V NEXT_ID -------------------------------
        win_armed = true;
        start_batch(SEL_A);
        start_batch(SEL_V);

        // ---- 4. wait for V batch completion ----------------------------
        {
            long t = 0;
            bool err, complete, stopped, active, enable_b;
            for (;;) {
                tick();
                if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                    dump_stall("scenario V");
                    throw TestFailure("watchdog: no handshake progress while V runs");
                }
                // V completion progress is tracked independently of A's
                // handshakes, so a stalled V is detected even while A runs.
                if (g_cycle - v_progress_cycle > v_watchdog_cycles) {
                    dump_stall("scenario V progress");
                    throw TestFailure("watchdog: V made no completion progress");
                }
#ifdef ACTUAL_DMA_SYSTEM
                if ((top->dbg_dma_batch_active & 1) &&
                    (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
                    dump_stall("scenario CPU progress");
                    throw TestFailure("watchdog: synthetic CPU made no progress");
                }
                if (cpu_writes && !top->dbg_cpu_wr_quiet &&
                    (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
                    dump_stall("scenario CPU write progress");
                    throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
                }
#endif
                read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
                if (complete) break;
                if (g_cycle - scenario_start > scenario_budget) {
                    dump_stall("scenario V budget");
                    throw TestFailure("scenario budget exceeded while V runs");
                }
                ++t;
            }
            (void)t;
        }

        // tick() already froze the window at the final hardware V done.
        // Print before drain; final PASS still requires all data checks.
        report_window(name, enable);

        // ---- 5. STOP A and drain ---------------------------------------
        stop_batch(SEL_A);
        {
            bool err, complete, stopped, active, enable_b;
            long t = 0;
            for (;;) {
                tick();
                if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                    dump_stall("scenario A drain");
                    throw TestFailure("watchdog: no handshake progress while A drains");
                }
                read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
                if (!active) break;
                if (g_cycle - scenario_start > scenario_budget) {
                    dump_stall("scenario A drain budget");
                    throw TestFailure("scenario budget exceeded while A drains");
                }
                ++t;
            }
            (void)t;
        }

        check_quiet(name);
""",
        r"""        // ---- 3. launches + run to terminal -----------------------------
        // Full-batch (independent, default OFF) shares the configure phase
        // above.  Its common t0 is captured inside run_full_batch() after
        // BOTH batches are configured and before the first NEXT_ID.
        if (full_batch) {
            run_full_batch(name, enable);
        } else {
            win_armed = true;
            start_batch(SEL_A);
            start_batch(SEL_V);

            // ---- 4. wait for V batch completion ------------------------
            {
                long t = 0;
                bool err, complete, stopped, active, enable_b;
                for (;;) {
                    tick();
                    if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                        dump_stall("scenario V");
                        throw TestFailure("watchdog: no handshake progress while V runs");
                    }
                    // V completion progress is tracked independently of A's
                    // handshakes, so a stalled V is detected even while A runs.
                    if (g_cycle - v_progress_cycle > v_watchdog_cycles) {
                        dump_stall("scenario V progress");
                        throw TestFailure("watchdog: V made no completion progress");
                    }
#ifdef ACTUAL_DMA_SYSTEM
                    if (cpu_enable && (top->dbg_dma_batch_active & 1) &&
                        (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
                        dump_stall("scenario CPU progress");
                        throw TestFailure("watchdog: synthetic CPU made no progress");
                    }
                    if (cpu_enable && cpu_writes && !top->dbg_cpu_wr_quiet &&
                        (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
                        dump_stall("scenario CPU write progress");
                        throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
                    }
#endif
                    read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
                    if (complete) break;
                    if (g_cycle - scenario_start > scenario_budget) {
                        dump_stall("scenario V budget");
                        throw TestFailure("scenario budget exceeded while V runs");
                    }
                    ++t;
                }
                (void)t;
            }

            // tick() already froze the window at the final hardware V done.
            // Print before drain; final PASS still requires all data checks.
            report_window(name, enable);

            // ---- 5. STOP A and drain (LEGACY measurement path only) ----
            stop_batch(SEL_A);
            {
                bool err, complete, stopped, active, enable_b;
                long t = 0;
                for (;;) {
                    tick();
                    if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                        dump_stall("scenario A drain");
                        throw TestFailure("watchdog: no handshake progress while A drains");
                    }
                    read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
                    if (!active) break;
                    if (g_cycle - scenario_start > scenario_budget) {
                        dump_stall("scenario A drain budget");
                        throw TestFailure("scenario budget exceeded while A drains");
                    }
                    ++t;
                }
                (void)t;
            }

            check_quiet(name);
        }
""",
        "C11 run_scenario branch")

    # ---- (C12) terminal: pairwise CPU assertions -------------------------
    t = sub(
        t,
        r"""        // Synthetic CPU must actually run and drain (no false PASS by skipping
        // the new branch).
        CHECK(top->dbg_cpu_completed > 0, "synthetic CPU completed no reads");
        CHECK(top->dbg_cpu_issued == top->dbg_cpu_completed,
              "synthetic CPU issued/completed read mismatch at terminal");
        CHECK(top->dbg_cpu_quiet, "synthetic CPU not quiet at terminal");
""",
        r"""        // Synthetic CPU assertions are PAIRED: a runtime-disabled CPU is
        // required to have issued nothing and to be quiet, never to have
        // completed reads.
        if (cpu_enable) {
            CHECK(top->dbg_cpu_completed > 0, "synthetic CPU completed no reads");
            CHECK(top->dbg_cpu_issued == top->dbg_cpu_completed,
                  "synthetic CPU issued/completed read mismatch at terminal");
            CHECK(top->dbg_cpu_quiet, "synthetic CPU not quiet at terminal");
        } else {
            CHECK(top->dbg_cpu_issued == 0 && top->dbg_cpu_completed == 0,
                  "synthetic CPU issued traffic while runtime-disabled");
            CHECK(top->dbg_cpu_quiet, "synthetic CPU not quiet at terminal");
        }
""",
        "C12 pairwise CPU terminal checks")

    # ---- (C13) check_quiet(): CPU write watchdog only while enabled ------
    t = sub(
        t,
        r"""#ifdef ACTUAL_DMA_SYSTEM
            if (cpu_writes && !top->dbg_cpu_wr_quiet &&
                (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
                dump_stall(std::string("quiet cpu-wr ") + tag);
                throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
            }
#endif
""",
        r"""#ifdef ACTUAL_DMA_SYSTEM
            if (cpu_enable && cpu_writes && !top->dbg_cpu_wr_quiet &&
                (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
                dump_stall(std::string("quiet cpu-wr ") + tag);
                throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
            }
#endif
""",
        "C13 check_quiet CPU-wr watchdog")

    # ---- (C14) main(): parse the new plusargs ----------------------------
    t = sub(
        t,
        r"""        if (const char* v = val("owner-only")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +owner-only='%s' (expected 0|1)\n", v);
                return 2;
            }
            owner_only = (parsed != 0);
        }
""",
        r"""        if (const char* v = val("owner-only")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +owner-only='%s' (expected 0|1)\n", v);
                return 2;
            }
            owner_only = (parsed != 0);
        }
        if (const char* v = val("full-batch"))
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
            full_watchdog = std::strtol(v, nullptr, 0);
        if (const char* v = val("launch-order")) launch_order = v;
""",
        "C14 parse new plusargs")

    # ---- (C15) main(): strict validation ---------------------------------
    t = sub(
        t,
        r"""    if (!(ptw_lat == 31 || ptw_lat == 100)) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +ptw-latency must be 31|100\n");
        return 2;
    }
""",
        r"""    if (!(ptw_lat == 31 || ptw_lat == 100)) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +ptw-latency must be 31|100\n");
        return 2;
    }
    if (launch_order != "av" && launch_order != "va") {
        std::fprintf(stderr,
            "nonblock_actual_dma_test: invalid +launch-order='%s' (expected av|va)\n",
            launch_order.c_str());
        return 2;
    }
    if (full_watchdog <= 0) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +full-watchdog must be > 0\n");
        return 2;
    }
#ifndef ACTUAL_DMA_SYSTEM
    if (full_batch) {
        std::fprintf(stderr,
            "nonblock_actual_dma_test: +full-batch requires the SYSTEM build (--system)\n");
        return 2;
    }
#endif
#ifdef ACTUAL_DMA_SYSTEM
    if (full_batch) {
        if (!(transfers == 64 && a_target == 64ULL && v_gap == 0 &&
              (a_pages == 1 || a_pages == 8) && warmup == 256 && ptw_lat == 31)) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +full-batch requires transfers=64 "
                "a-target=64 v-gap=0 a-pages=1|8 warmup=256 ptw-latency=31\n");
            return 2;
        }
    }
#endif
""",
        "C15 validate new options")

    # ---- (C16) main(): config print --------------------------------------
    t = sub(
        t,
        r"""    std::printf("nonblock_actual_dma_test: cpu_writes=%d\n", (int)cpu_writes);
    std::printf("nonblock_actual_dma_test: read_admit=%s budget=%u period=%u\n",
                read_admit.c_str(), read_budget, read_period);
""",
        r"""    std::printf("nonblock_actual_dma_test: cpu_writes=%d\n", (int)cpu_writes);
    std::printf("nonblock_actual_dma_test: read_admit=%s budget=%u period=%u\n",
                read_admit.c_str(), read_budget, read_period);
    std::printf("nonblock_actual_dma_test: full_batch=%d cpu_enable=%d launch_order=%s "
                "start_delay=%u v_start_delay=%u a_start_delay=%u full_watchdog=%ld\n",
                (int)full_batch, (int)cpu_enable, launch_order.c_str(),
                start_delay, v_start_delay, a_start_delay, full_watchdog);
""",
        "C16 config print")

    # ---- (C17) main(): tb wiring ----------------------------------------
    t = sub(
        t,
        r"""#ifdef ACTUAL_DMA_SYSTEM
        tb.cpu_writes      = cpu_writes;
        tb.ds_priority     = ds_priority;
""",
        r"""        tb.full_batch          = full_batch;
        tb.cpu_enable          = cpu_enable;
        tb.start_delay         = start_delay;
        tb.v_start_delay       = v_start_delay;
        tb.a_start_delay       = a_start_delay;
        tb.launch_order        = launch_order;
        tb.full_watchdog_cycles= full_watchdog;
#ifdef ACTUAL_DMA_SYSTEM
        tb.cpu_writes      = cpu_writes;
        tb.ds_priority     = ds_priority;
""",
        "C17 tb wiring")

    # ---- (C18) class tail: report_full call + new methods ----------------
    t = sub(
        t,
        r"""                    (unsigned)fw_raw, (unsigned)fw_win);
    }
};
""",
        r"""                    (unsigned)fw_raw, (unsigned)fw_win);

#ifdef ACTUAL_DMA_SYSTEM
        // The full-batch summary is printed ONLY after every terminal check
        // above has passed.
        if (full_batch) report_full(name, enable);
#endif
    }

#ifdef ACTUAL_DMA_SYSTEM
    // ---------------------------------------------------------------------
    // FULL-BATCH independent mode (SYSTEM only).  Default OFF; every legacy
    // scenario above is byte-for-byte unchanged.
    //
    // Epoch: t0 is captured after BOTH batches are configured and before the
    // first NEXT_ID.  All per-channel statistics run t0 -> final quiet and
    // never read win_open; win_open is set at t0 ONLY for the pre-existing
    // "ds_priority && win_open" model intervention.
    // ---------------------------------------------------------------------
    void full_measure_begin() {
        full_era         = true;
        full_t0          = g_cycle;
        full_quiet_cycle = 0;
        for (int c = 0; c < 2; ++c) {
            full_last_progress[c] = g_cycle;
            full_issue_n[c]       = 0;
            full_done_n[c]        = 0;
            full_issue_seen[c]    = false;
            full_first_issue[c]   = 0;
            full_final_done[c]    = 0;
            full_ready_cyc[c].clear();
            full_issue_cyc[c].clear();
            full_done_cyc[c].clear();
            full_ar_n[c] = full_aw_n[c] = 0;
            full_rd_bytes[c] = full_wr_bytes[c] = 0;
            full_r_n[c] = full_r_return_bytes[c] = 0;
            full_b_n[c] = full_err[c] = 0;
            full_reg_issued[c] = full_reg_done[c] = 0;
            full_reg_complete[c] = full_reg_stopped[c] = false;
            full_reg_active[c] = full_reg_enable[c] = false;
        }
        full_tl_accept = full_tl_resp = 0;
        full_iotlb_lookup = full_iotlb_hit = 0;
        full_ptw_active = full_ptw_r_fire = 0;
        full_fast = 0;
        full_comp_ar = full_comp_aw = full_comp_w = 0;
        full_ddtw_ar = full_ptw_ar = 0;
        full_trace_ptw_active = false;
        // Open the model-only DS-priority window at the common t0.  The
        // full-batch statistics never read win_open.
        win_open       = true;
        win_seen       = true;
        win_armed      = false;
        win_open_cycle = g_cycle;
        fg_raw_open = (uint32_t)top->dbg_frontier_grant;
        fw_raw_open = (uint32_t)top->dbg_frontier_wait;
    }

    void full_measure_end() { full_era = false; }

    // Independent per-channel progress watchdog.  Closed as soon as the
    // channel has produced all of its DONE pulses, so it fires even when the
    // OTHER channel or the synthetic CPU is still active.
    void full_channel_watchdog(int ch, const char* tag) {
        const uint32_t target = (ch == 0) ? v_transfers : (uint32_t)a_target;
        if (full_done_n[ch] >= target) return;
        if (g_cycle - full_last_progress[ch] > full_watchdog_cycles) {
            dump_stall(std::string("full watchdog ") + tag);
            throw TestFailure("watchdog: full-batch channel made no progress");
        }
    }

    void run_full_batch(const char* name, bool enable) {
        (void)name;
        (void)enable;
        full_measure_begin();

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
            }
            if (cpu_enable && (top->dbg_dma_batch_active & 1) &&
                (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
                dump_stall("full scenario CPU progress");
                throw TestFailure("watchdog: synthetic CPU made no progress");
            }
            if (cpu_enable && cpu_writes && !top->dbg_cpu_wr_quiet &&
                (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
                dump_stall("full scenario CPU write progress");
                throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
            }
            if (v_done && a_done) break;
        }

        // Both channels completed NATURALLY: STOP is never used on the
        // measured path in full mode.
        check_quiet(name);
        full_quiet_cycle = g_cycle;
        full_measure_end();
        full_reg_cross_check();
    }

    // Read-back cross-check.  Times in the summary come ONLY from the
    // pre-edge hardware pulses; these registers/flags are counts only.
    void full_reg_cross_check() {
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
    }

    void report_full(const char* name, bool enable) {
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
        }

        const long t_v    = full_final_done[0] - full_t0;
        const long t_a    = full_final_done[1] - full_t0;
        const long t_data = std::max(t_v, t_a);
        const long t_ret  = full_quiet_cycle - full_t0;

        auto arr = [](const std::vector<long>& v) -> std::string {
            std::string s = "[";
            for (size_t i = 0; i < v.size(); ++i) {
                if (i) s += ",";
                s += std::to_string(v[i]);
            }
            s += "]";
            return s;
        };

        std::printf("ACTUAL-DMA-FULL-BATCH-JSON {"
            "\"schema\":\"actual-dma-full-batch-v1\","
            "\"scenario\":\"%s\",\"enable\":%d,"
            "\"cpu_enable\":%d,\"ds_priority\":%d,\"launch_order\":\"%s\","
            "\"a_pages\":%u,\"v_gap\":%u,\"transfers\":%u,\"warmup\":%u,"
            "\"ptw_latency\":%u,\"a_target\":%llu,"
            "\"start_delay\":%u,\"v_start_delay\":%u,\"a_start_delay\":%u,"
            "\"full_watchdog\":%ld,"
            "\"t0\":%ld,\"T_V\":%ld,\"T_A\":%ld,\"T_data\":%ld,\"T_return\":%ld,"
            "\"quiet\":1,"
            "\"channels\":["
            "{\"id\":\"V\",\"bank\":0,\"issue\":%u,\"done\":%u,"
            "\"first_issue\":%ld,\"final_done\":%ld,"
            "\"ready\":%s,\"issue_cyc\":%s,\"done_cyc\":%s,"
            "\"ar\":%llu,\"aw\":%llu,\"rd_bytes\":%llu,\"wr_bytes\":%llu,"
            "\"r_beats\":%llu,\"r_bytes\":%llu,\"b\":%llu,\"err\":%llu,"
            "\"reg_issued\":%llu,\"reg_done\":%llu,"
            "\"batch_complete\":%d,\"batch_stopped\":%d},"
            "{\"id\":\"A\",\"bank\":1,\"issue\":%u,\"done\":%u,"
            "\"first_issue\":%ld,\"final_done\":%ld,"
            "\"ready\":%s,\"issue_cyc\":%s,\"done_cyc\":%s,"
            "\"ar\":%llu,\"aw\":%llu,\"rd_bytes\":%llu,\"wr_bytes\":%llu,"
            "\"r_beats\":%llu,\"r_bytes\":%llu,\"b\":%llu,\"err\":%llu,"
            "\"reg_issued\":%llu,\"reg_done\":%llu,"
            "\"batch_complete\":%d,\"batch_stopped\":%d}"
            "],"
            "\"raw\":{\"tl_accept\":%llu,\"tl_resp\":%llu,"
            "\"iotlb_lookup\":%llu,\"iotlb_hit\":%llu,"
            "\"ptw_active_cycles\":%llu,\"ptw_r_fire\":%llu,\"fast\":%llu,"
            "\"comp_ar\":%llu,\"comp_aw\":%llu,\"comp_w\":%llu,"
            "\"ddtw_ar\":%llu,\"ptw_ar\":%llu},"
            "\"attribution\":\"raw pre-edge hardware counters; comp-port payload only; "
            "no oracle\"}\n",
            name, (int)enable,
            (int)cpu_enable, (int)ds_priority, launch_order.c_str(),
            a_pages, v_gap_cycles, v_transfers, warmup, ptw_latency,
            (unsigned long long)a_target,
            start_delay, v_start_delay, a_start_delay, full_watchdog_cycles,
            full_t0, t_v, t_a, t_data, t_ret,
            (unsigned)full_issue_n[0], (unsigned)full_done_n[0],
            full_first_issue[0], full_final_done[0],
            arr(full_ready_cyc[0]).c_str(), arr(full_issue_cyc[0]).c_str(),
            arr(full_done_cyc[0]).c_str(),
            (unsigned long long)full_ar_n[0], (unsigned long long)full_aw_n[0],
            (unsigned long long)full_rd_bytes[0], (unsigned long long)full_wr_bytes[0],
            (unsigned long long)full_r_n[0], (unsigned long long)full_r_return_bytes[0],
            (unsigned long long)full_b_n[0], (unsigned long long)full_err[0],
            (unsigned long long)full_reg_issued[0], (unsigned long long)full_reg_done[0],
            (int)full_reg_complete[0], (int)full_reg_stopped[0],
            (unsigned)full_issue_n[1], (unsigned)full_done_n[1],
            full_first_issue[1], full_final_done[1],
            arr(full_ready_cyc[1]).c_str(), arr(full_issue_cyc[1]).c_str(),
            arr(full_done_cyc[1]).c_str(),
            (unsigned long long)full_ar_n[1], (unsigned long long)full_aw_n[1],
            (unsigned long long)full_rd_bytes[1], (unsigned long long)full_wr_bytes[1],
            (unsigned long long)full_r_n[1], (unsigned long long)full_r_return_bytes[1],
            (unsigned long long)full_b_n[1], (unsigned long long)full_err[1],
            (unsigned long long)full_reg_issued[1], (unsigned long long)full_reg_done[1],
            (int)full_reg_complete[1], (int)full_reg_stopped[1],
            (unsigned long long)full_tl_accept, (unsigned long long)full_tl_resp,
            (unsigned long long)full_iotlb_lookup, (unsigned long long)full_iotlb_hit,
            (unsigned long long)full_ptw_active, (unsigned long long)full_ptw_r_fire,
            (unsigned long long)full_fast,
            (unsigned long long)full_comp_ar, (unsigned long long)full_comp_aw,
            (unsigned long long)full_comp_w,
            (unsigned long long)full_ddtw_ar, (unsigned long long)full_ptw_ar);
    }
#else
    void run_full_batch(const char*, bool) {
        throw TestFailure("full-batch mode requires the SYSTEM build (--system)");
    }
#endif
};
""",
        "C18 report_full call + new methods")

    write(p, t)
    print("patched:", p)

# ==========================================================================
# File 2: tests/nonblock_actual_dma_system_top.sv
# ==========================================================================
def patch_sv() -> None:
    p = ROOT / "tests" / "nonblock_actual_dma_system_top.sv"
    t = read(p)

    # ---- (S1) new runtime cpu_enable_i port ------------------------------
    t = sub(
        t,
        r"""    input  logic        nonblock_enable_i,
    input  logic        policy_partition_i,
    input  logic        owner_only_i,
""",
        r"""    input  logic        nonblock_enable_i,
    input  logic        policy_partition_i,
    input  logic        owner_only_i,
    // Runtime CPU enable.  When low, NO synthetic CPU source (read lane or
    // write/AMO engine) starts a new offer.  This is a real runtime input,
    // not a CpuIssueInterval==0 workaround.
    input  logic        cpu_enable_i,
""",
        "S1 cpu_enable_i port")

    # ---- (S2) gate cpu_run ----------------------------------------------
    t = sub(
        t,
        r"""    logic        cpu_run;
    assign cpu_run = dbg_dma_batch_active[0] | dbg_dma_batch_gap[0];
""",
        r"""    logic        cpu_run;
    // Runtime CPU enable gates BOTH synthetic CPU lanes (read + write/AMO),
    // because each lane starts new work only while cpu_run is high.
    assign cpu_run = cpu_enable_i & (dbg_dma_batch_active[0] | dbg_dma_batch_gap[0]);
""",
        "S2 cpu_run gate")

    write(p, t)
    print("patched:", p)

# ==========================================================================
# File 3: tests/test_nonblock_actual_dma.py
# ==========================================================================
def patch_runner() -> None:
    p = ROOT / "tests" / "test_nonblock_actual_dma.py"
    t = read(p)

    # ---- (R1) new CLI options -------------------------------------------
    t = sub(
        t,
        r"""    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=1048576,
                    help="A autonomous batch target (default 1048576)")
""",
        r"""    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=1048576,
                    help="A autonomous batch target (default 1048576)")
    ap.add_argument("--full-batch", action="store_true",
                    help="independent full-batch double-natural-completion run "
                         "(SYSTEM only; forces transfers=a-target=64, v-gap=0, "
                         "warmup=256, ptw-latency=31; a-pages must be 1 or 8)")
    ap.add_argument("--cpu-enable", type=int, choices=(0, 1), default=1,
                    help="drive the runtime cpu_enable_i input (SYSTEM only, "
                         "default 1)")
    ap.add_argument("--ds-priority", action="store_true",
                    help="SYSTEM-only model-only DS read priority inside the "
                         "t0-based window (default off for the CPU-off experiment)")
    ap.add_argument("--event-trace", default=None,
                    help="path for the CSV event trace (+event-trace, SYSTEM only)")
    ap.add_argument("--start-delay", type=int, default=0,
                    help="billed cycles between the common t0 and the first NEXT_ID")
    ap.add_argument("--v-start-delay", type=int, default=0,
                    help="bounded cycles before V's NEXT_ID")
    ap.add_argument("--a-start-delay", type=int, default=0,
                    help="bounded cycles before A's NEXT_ID")
    ap.add_argument("--launch-order", choices=("av", "va"), default="av",
                    help="which channel's NEXT_ID is issued first (default av)")
    ap.add_argument("--full-watchdog", type=int, default=200000,
                    help="full-batch per-channel progress watchdog cycles "
                         "(default 200000; a small value is for negative tests)")
""",
        "R1 runner options")

    # ---- (R2) validation -------------------------------------------------
    t = sub(
        t,
        r"""    if opts.cpu_writes and not opts.system_atomics:
        ap.error("--cpu-writes requires --system-atomics")
""",
        r"""    if opts.cpu_writes and not opts.system_atomics:
        ap.error("--cpu-writes requires --system-atomics")
    if opts.start_delay < 0 or opts.v_start_delay < 0 or opts.a_start_delay < 0:
        ap.error("--start-delay/--v-start-delay/--a-start-delay must be >= 0")
    if opts.full_watchdog <= 0:
        ap.error("--full-watchdog must be > 0")
    if opts.full_batch:
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
    if not opts.system:
        if opts.cpu_enable != 1:
            ap.error("--cpu-enable requires --system")
        if (opts.ds_priority or opts.event_trace is not None or
                opts.start_delay or opts.v_start_delay or opts.a_start_delay or
                opts.launch_order != "av" or opts.full_watchdog != 200000):
            ap.error("--ds-priority/--event-trace/--start-delay/--v-start-delay/"
                     "--a-start-delay/--launch-order/--full-watchdog require --system")
""",
        "R2 runner validation")

    # ---- (R3) forward plusargs ------------------------------------------
    t = sub(
        t,
        r"""    if opts.system:
        run_args += ["+system=1"]
    if opts.cpu_writes:
        run_args += ["+cpu-writes=1"]
""",
        r"""    if opts.system:
        run_args += ["+system=1"]
    if opts.cpu_writes:
        run_args += ["+cpu-writes=1"]
    run_args += ["+cpu-enable={}".format(opts.cpu_enable)]
    run_args += ["+start-delay={}".format(opts.start_delay)]
    run_args += ["+v-start-delay={}".format(opts.v_start_delay)]
    run_args += ["+a-start-delay={}".format(opts.a_start_delay)]
    run_args += ["+launch-order={}".format(opts.launch_order)]
    run_args += ["+full-watchdog={}".format(opts.full_watchdog)]
    if opts.full_batch:
        run_args += ["+full-batch=1"]
    if opts.ds_priority:
        run_args += ["+ds-priority=1"]
    if opts.event_trace:
        run_args += ["+event-trace={}".format(opts.event_trace)]
""",
        "R3 runner plusargs")

    # ---- (R4) summary print ---------------------------------------------
    t = sub(
        t,
        r"""    print("test_nonblock_actual_dma: system={} cpu_writes={} top={} mode={} "
          "a_pages={} v_gap={} transfers={} warmup={} ptw_latency={} a_target={}".format(
              int(opts.system), int(opts.cpu_writes), top_name, opts.mode,
              opts.a_pages, opts.v_gap, opts.transfers, opts.warmup,
              opts.ptw_latency, opts.a_target))
""",
        r"""    print("test_nonblock_actual_dma: system={} cpu_writes={} top={} mode={} "
          "a_pages={} v_gap={} transfers={} warmup={} ptw_latency={} a_target={}".format(
              int(opts.system), int(opts.cpu_writes), top_name, opts.mode,
              opts.a_pages, opts.v_gap, opts.transfers, opts.warmup,
              opts.ptw_latency, opts.a_target))
    print("test_nonblock_actual_dma: full_batch={} cpu_enable={} ds_priority={} "
          "launch_order={} start_delay={} v_start_delay={} a_start_delay={} "
          "full_watchdog={} event_trace={}".format(
              int(opts.full_batch), int(opts.cpu_enable), int(opts.ds_priority),
              opts.launch_order, opts.start_delay, opts.v_start_delay,
              opts.a_start_delay, opts.full_watchdog, opts.event_trace))
""",
        "R4 runner print")

    write(p, t)
    print("patched:", p)

# ==========================================================================
if __name__ == "__main__":
    patch_cpp()
    patch_sv()
    patch_runner()
    print("done: 3 files patched (nothing else touched)")
