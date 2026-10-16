#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
make_natural_experiment.py

Deterministic generator for the out/natural-write-order-20261008/ experiment.

Creates ONLY inside /home/xvhaocheng/work/iommu-platform/out/natural-write-order-20261008/:
    system_top.sv                (copy of tests/nonblock_actual_dma_system_top.sv + read-only tr_* taps)
    natural_test.cpp             (copy of tests/nonblock_actual_dma_test.cpp + +natural=1 driver)
    build-natural-command.json   (copy of the §23 samebank-private command, natural parameters)
    build-natural.sh             (runs the command above)

It NEVER writes to tests/ or to any production RTL.  Every anchor must occur
EXACTLY ONCE in the current source text; otherwise the script raises and writes
nothing.  No file is read-modify-written in place.
"""

import json
import os
import sys

ROOT   = "/home/xvhaocheng/work/iommu-platform"
OUTDIR = os.path.join(ROOT, "out", "natural-write-order-20261008")

SRC_TOP = os.path.join(ROOT, "tests", "nonblock_actual_dma_system_top.sv")
SRC_CPP = os.path.join(ROOT, "tests", "nonblock_actual_dma_test.cpp")
SRC_CMD = os.path.join(ROOT, "out", "write-order-boundary-20261007",
                       "build-samebank-private-command.json")

DST_TOP = os.path.join(OUTDIR, "system_top.sv")
DST_CPP = os.path.join(OUTDIR, "natural_test.cpp")
DST_CMD = os.path.join(OUTDIR, "build-natural-command.json")
DST_SH  = os.path.join(OUTDIR, "build-natural.sh")


def read(path):
    with open(path, "r", encoding="utf-8") as fh:
        return fh.read()


def write(path, text):
    real_out = os.path.realpath(OUTDIR)
    real_dst = os.path.realpath(path)
    if os.path.commonpath([real_out, real_dst]) != real_out:
        raise AssertionError("refusing to write outside %s: %s" % (OUTDIR, path))
    with open(path, "w", encoding="utf-8") as fh:
        fh.write(text)
    print("wrote %-24s %7d bytes" % (os.path.basename(path), len(text)))


def patch(text, anchor, replacement, tag):
    """Exact anchored replacement; the anchor must occur exactly once."""
    n = text.count(anchor)
    if n != 1:
        raise AssertionError(
            "anchor %r occurs %d times (expected exactly 1); nothing written" % (tag, n))
    return text.replace(anchor, replacement, 1)


# ===========================================================================
# 1. system_top.sv: read-only translation-request taps
# ===========================================================================
TOP_ANCHOR_PORTS = r"""    output logic [8:0]  dbg_ra_down_attrs
);"""

TOP_REPL_PORTS = r"""    output logic [8:0]  dbg_ra_down_attrs,

    // ---------------- test-only natural write-order taps (read-only) --------
    // Direct, purely observational wires to the inner harness translation
    // request port.  No register, no feedback, no change to any existing
    // module name or port; the DUT cannot see these signals.
    //   nat_vaw_* : virtual completion AW offered at the IOMMU request port
    //   nat_var_* : virtual source AR offered at the IOMMU request port
    //   nat_vw_fire / nat_vw_last : virtual W beat handshake / WLAST level
    output logic        nat_vaw_valid,
    output logic        nat_vaw_ready,
    output logic [3:0]  nat_vaw_id,
    output logic [63:0] nat_vaw_addr,
    output logic [7:0]  nat_vaw_len,
    output logic        nat_var_valid,
    output logic        nat_var_ready,
    output logic [3:0]  nat_var_id,
    output logic [63:0] nat_var_addr,
    output logic [7:0]  nat_var_len,
    output logic        nat_vw_fire,
    output logic        nat_vw_last
);"""

TOP_ANCHOR_ASSIGN = r"""    assign dbg_cpu_amo_r_data    = cpu_amo_r_data_q;

endmodule"""

TOP_REPL_ASSIGN = r"""    assign dbg_cpu_amo_r_data    = cpu_amo_r_data_q;

    // ========================================================================
    // Test-only natural write-order taps.  Pure combinatorial reads of the
    // inner harness translation-request nets (same access class as the
    // existing dbg_tr_aw_stall / dbg_iotlb_fill taps above); they drive
    // nothing and add no state.
    // ========================================================================
    assign nat_vaw_valid = i_dma.tr_aw_valid;
    assign nat_vaw_ready = i_dma.tr_aw_ready;
    assign nat_vaw_id    = i_dma.tr_aw_id;
    assign nat_vaw_addr  = i_dma.tr_aw_addr;
    assign nat_vaw_len   = i_dma.tr_aw_len;
    assign nat_var_valid = i_dma.tr_ar_valid;
    assign nat_var_ready = i_dma.tr_ar_ready;
    assign nat_var_id    = i_dma.tr_ar_id;
    assign nat_var_addr  = i_dma.tr_ar_addr;
    assign nat_var_len   = i_dma.tr_ar_len;
    assign nat_vw_fire   = i_dma.tr_w_valid && i_dma.tr_w_ready;
    assign nat_vw_last   = i_dma.tr_w_last;

endmodule"""


# ===========================================================================
# 2. natural_test.cpp
# ===========================================================================
CPP_ANCHOR_GEOM = r"""static const uint32_t PAGE_BEATS = 512;   // 4 KiB / 8 B
static const uint32_t PAGE_BYTES = 4096;"""

CPP_REPL_GEOM = r"""static const uint32_t PAGE_BEATS = 512;   // 4 KiB / 8 B
static const uint32_t PAGE_BYTES = 4096;

// ---------------------------------------------------------------------------
// NATURAL write-order experiment geometry (test-only; this file only).
//
// Two REAL DMAs, one single-task copy each, CONF = 1 (decouple_rw), source
// offset 0 and destination offset 3072, in four disjoint physical regions:
//   V (DMA0): src IOVA 0x1000_0000 -> PA 0x8000_0000, 8 pages (32 KiB)
//             dst IOVA 0x1001_0000 -> PA 0x8100_0000, 9 pages (+ 3072 offset)
//   A (DMA1): src IOVA 0x2000_0000 -> PA 0x9000_0000, 8 pages
//             dst IOVA 0x2000_8000 -> PA 0x9010_0000, 9 pages (+ 3072 offset)
// These constants only build the memory image, classify addresses and drive
// the acceptance checks; they never drive the DUT.
// ---------------------------------------------------------------------------
static const uint64_t NAT_V_SRC_IOVA      = 0x10000000ULL;
static const uint64_t NAT_V_SRC_PA        = 0x80000000ULL;
static const uint64_t NAT_V_DST_PAGE_IOVA = 0x10010000ULL;
static const uint64_t NAT_V_DST_PAGE_PA   = 0x81000000ULL;
static const uint64_t NAT_A_SRC_IOVA      = 0x20000000ULL;
static const uint64_t NAT_A_SRC_PA        = 0x90000000ULL;
static const uint64_t NAT_A_DST_PAGE_IOVA = 0x20008000ULL;   // A_IOVA_BASE + 8*4K
static const uint64_t NAT_A_DST_PAGE_PA   = 0x90100000ULL;   // A_DST_PA_BASE
static const uint32_t NAT_MAX_PAGES       = 8;               // 32 KiB / DMA
static const uint32_t NAT_DST_PAGES       = 9;               // 9 dst pages
static const uint32_t NAT_DST_OFF         = 3072;            // dst page offset

// Region classification for the trace.  The virtual (IOVA) and physical (PA)
// windows of this experiment never overlap, so one classifier per side is
// enough.  Nothing here is derived from observed traffic.
enum NatRegion {
    NR_VSRC = 0, NR_VDST = 1, NR_ASRC = 2, NR_ADST = 3, NR_OTHER = 4, NR_N = 5
};

static inline const char* nat_region_name(int r) {
    switch (r) {
    case NR_VSRC: return "vsrc";
    case NR_VDST: return "vdst";
    case NR_ASRC: return "asrc";
    case NR_ADST: return "adst";
    default:      return "other";
    }
}
static inline int nat_pregion(uint64_t pa) {
    if (pa >= 0x80000000ULL && pa < 0x80100000ULL) return NR_VSRC;
    if (pa >= 0x81000000ULL && pa < 0x81100000ULL) return NR_VDST;
    if (pa >= 0x90000000ULL && pa < 0x90100000ULL) return NR_ASRC;
    if (pa >= 0x90100000ULL && pa < 0x90200000ULL) return NR_ADST;
    return NR_OTHER;
}
static inline int nat_vregion(uint64_t va) {
    if (va >= 0x10000000ULL && va < 0x10008000ULL) return NR_VSRC;
    if (va >= 0x10010000ULL && va < 0x10019000ULL) return NR_VDST;
    if (va >= 0x20000000ULL && va < 0x20008000ULL) return NR_ASRC;
    if (va >= 0x20008000ULL && va < 0x20012000ULL) return NR_ADST;
    return NR_OTHER;
}"""

CPP_ANCHOR_DTOR = r"""    ~Tb() {
#ifdef ACTUAL_DMA_SYSTEM
        if (event_trace) std::fclose(event_trace);
#endif
        top->final(); delete top;
    }"""

CPP_REPL_DTOR = r"""    ~Tb() {
#ifdef ACTUAL_DMA_SYSTEM
        if (event_trace) std::fclose(event_trace);
        if (nat_trace) { std::fflush(nat_trace); std::fclose(nat_trace); nat_trace = nullptr; }
#endif
        top->final(); delete top;
    }"""

CPP_ANCHOR_MEMBERS = r"""    // Debug-only reset baselines.  DONE waits use the NEXT_ID captured by
    // trigger(), never these values.
    uint64_t v_done_last = 0, a_done_last = 0;"""

CPP_REPL_MEMBERS = r"""    // Debug-only reset baselines.  DONE waits use the NEXT_ID captured by
    // trigger(), never these values.
    uint64_t v_done_last = 0, a_done_last = 0;

    // ---------------- natural write-order experiment (test-only) ----------
    // Pure observation/bookkeeping for the +natural=1 run.  Inert while
    // natural_enable is false, so every ordinary scenario is unchanged.
    bool        natural_enable = false;
    std::string natural_first  = "v";
    uint64_t    natural_bytes  = 2048;
    std::string nat_trace_path;
    FILE*       nat_trace      = nullptr;

    struct NatVawEnt {
        uint32_t id     = 0;
        uint64_t addr   = 0;
        uint32_t len    = 0;
        uint32_t got    = 0;
        int      region = NR_OTHER;
    };
    // Virtual AW handshakes observed on the translation-request port, in the
    // order they really happened.  A virtual W beat is attached only to the
    // head of this queue and only WLAST closes the head; the queue is never
    // fed from anything but a real nat_vaw handshake and never invented.
    std::deque<NatVawEnt> nat_vaw_q;

    uint64_t n_nat_vaw = 0, n_nat_var = 0;
    uint64_t n_nat_vw_beats = 0, n_nat_vwlast = 0;
    uint64_t n_nat_vw_unattr = 0, n_nat_vw_mismatch = 0;
    uint64_t n_nat_par = 0, n_nat_paw = 0, n_nat_pw = 0, n_nat_pr = 0;
    uint64_t nat_par_bursts[NR_N] = {0, 0, 0, 0, 0};
    uint64_t nat_paw_bursts[NR_N] = {0, 0, 0, 0, 0};
    uint64_t nat_pr_beats[NR_N]   = {0, 0, 0, 0, 0};
    uint64_t nat_pw_beats[NR_N]   = {0, 0, 0, 0, 0};
    uint64_t nat_rstall_cycles = 0, nat_rstall_episodes = 0;
    uint64_t nat_rstall_region[NR_N] = {0, 0, 0, 0, 0};
    bool     nat_r_stall_prev = false;
    long     nat_r_stall_begin = 0;
    int      nat_r_stall_region = NR_OTHER;
    uint64_t nat_r_stall_id = 0, nat_r_stall_addr = 0, nat_r_stall_len = 0;

    // Saved pre-copy payload snapshots; index 0 = V (DMA0), 1 = A (DMA1).
    std::vector<uint64_t> nat_src_saved[2];
    std::vector<uint64_t> nat_dst_saved[2];"""

CPP_ANCHOR_TICKHEAD = r"""    void tick() {
        top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;"""

CPP_REPL_TICKHEAD = r"""#ifdef ACTUAL_DMA_SYSTEM
    // ---------------------------------------------------------------------
    // Natural write-order trace emitter (test-only).
    // CSV columns: cycle,event,region,id,addr,len,extra
    // Flushed only for the key events so a watchdog abort still leaves the
    // full on-disk scene (the Tb destructor flushes/closes on exception too).
    // ---------------------------------------------------------------------
    void nat_emit(const char* event, int region, uint64_t id, uint64_t addr,
                  uint64_t len, uint64_t extra, bool flush) {
        if (!nat_trace) return;
        std::fprintf(nat_trace, "%ld,%s,%s,%llu,0x%016llx,%llu,%llu\n",
                     g_cycle, event, nat_region_name(region),
                     (unsigned long long)id, (unsigned long long)addr,
                     (unsigned long long)len, (unsigned long long)extra);
        if (flush) std::fflush(nat_trace);
    }
#endif

    void nat_clear_run_state() {
        nat_vaw_q.clear();
        n_nat_vaw = n_nat_var = 0;
        n_nat_vw_beats = n_nat_vwlast = 0;
        n_nat_vw_unattr = n_nat_vw_mismatch = 0;
        n_nat_par = n_nat_paw = n_nat_pw = n_nat_pr = 0;
        for (int i = 0; i < NR_N; ++i) {
            nat_par_bursts[i] = nat_paw_bursts[i] = 0;
            nat_pr_beats[i]   = nat_pw_beats[i]   = 0;
            nat_rstall_region[i] = 0;
        }
        nat_rstall_cycles = nat_rstall_episodes = 0;
        nat_r_stall_prev = false;
        nat_r_stall_begin = 0;
        nat_r_stall_region = NR_OTHER;
        nat_r_stall_id = nat_r_stall_addr = nat_r_stall_len = 0;
    }

    // ---------------------------------------------------------------------
    // Memory image for the natural geometry: distinct page tables, payload
    // and destination pre-fill for the two DMAs.  Rebuilt from scratch so no
    // other scenario's content leaks into the byte checks.
    // ---------------------------------------------------------------------
    void setup_memory_natural() {
        mem.clear();
        a_pages = NAT_MAX_PAGES;

        // ---- extended 1-level DDT (same layout as the coupled harness) ----
        write64(0x10280, 0x0000000000000001ULL);
        write64(0x10288, 0x0000000000000000ULL);
        write64(0x10290, 0x000000000000A000ULL);
        write64(0x10298, 0x8000000000000020ULL);   // Sv39, V root 0x20000
        write64(0x102A0, 0x0ULL); write64(0x102A8, 0x0ULL);
        write64(0x102B0, 0x0ULL); write64(0x102B8, 0x0ULL);

        write64(0x102C0, 0x0000000000000001ULL);
        write64(0x102C8, 0x0000000000000000ULL);
        write64(0x102D0, 0x000000000000B000ULL);
        write64(0x102D8, 0x8000000000000030ULL);   // Sv39, A root 0x30000
        write64(0x102E0, 0x0ULL); write64(0x102E8, 0x0ULL);
        write64(0x102F0, 0x0ULL); write64(0x102F8, 0x0ULL);

        // ---- V: root 0x20000 -> L1V 0x40000 -> L0V 0x50000 ----------------
        write64(0x20000, 0x0000000000010001ULL);
        write64(0x40400, 0x0000000000014001ULL);
        for (uint32_t i = 0; i < NAT_MAX_PAGES; ++i)        // index 0..7  : source
            write64(0x50000 + (uint64_t)i * 8,
                    (((NAT_V_SRC_PA >> 12) + (uint64_t)i) << 10) | 0xDFULL);
        for (uint32_t i = 0; i < NAT_DST_PAGES; ++i)        // index 16..24: dest
            write64(0x50000 + (uint64_t)(16 + i) * 8,
                    (((NAT_V_DST_PAGE_PA >> 12) + (uint64_t)i) << 10) | 0xDFULL);

        // ---- A: root 0x30000 -> L1A 0x60000 -> L0A 0x70000 ----------------
        write64(0x30000, 0x0000000000018001ULL);
        write64(0x60800, 0x000000000001C001ULL);
        for (uint32_t i = 0; i < NAT_MAX_PAGES; ++i)        // index 0..7  : source
            write64(0x70000 + (uint64_t)i * 8,
                    (((NAT_A_SRC_PA >> 12) + (uint64_t)i) << 10) | 0xDFULL);
        for (uint32_t i = 0; i < NAT_DST_PAGES; ++i)        // index 8..16 : dest
            write64(0x70000 + (uint64_t)(8 + i) * 8,        // 9th PTE @ 0x70080
                    (((NAT_A_DST_PAGE_PA >> 12) + (uint64_t)i) << 10) | 0xDFULL);

        // ---- payload: every source page a page_word, every destination page
        //      a distinguishable page_old (V domain 0, A domain 16) ---------
        for (uint32_t p = 0; p < NAT_MAX_PAGES; ++p)
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                write64(NAT_V_SRC_PA + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_word(p, i));
                write64(NAT_A_SRC_PA + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_word(16 + p, i));
            }
        for (uint32_t p = 0; p < NAT_DST_PAGES; ++p)
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                write64(NAT_V_DST_PAGE_PA + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_old(p, i));
                write64(NAT_A_DST_PAGE_PA + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_old(16 + p, i));
            }

        // ---- snapshot the pre-copy payload (used by the terminal checks) --
        const size_t src_words = (size_t)NAT_MAX_PAGES * PAGE_BEATS;
        const size_t dst_words = (size_t)NAT_DST_PAGES * PAGE_BEATS;
        for (int d = 0; d < 2; ++d) {
            nat_src_saved[d].assign(src_words, 0);
            nat_dst_saved[d].assign(dst_words, 0);
        }
        for (size_t k = 0; k < src_words; ++k) {
            nat_src_saved[0][k] = read64(NAT_V_SRC_PA + (uint64_t)k * 8);
            nat_src_saved[1][k] = read64(NAT_A_SRC_PA + (uint64_t)k * 8);
        }
        for (size_t k = 0; k < dst_words; ++k) {
            nat_dst_saved[0][k] = read64(NAT_V_DST_PAGE_PA + (uint64_t)k * 8);
            nat_dst_saved[1][k] = read64(NAT_A_DST_PAGE_PA + (uint64_t)k * 8);
        }
    }

    // ---------------------------------------------------------------------
    // +natural=1: one single-task copy per real DMA, both configured before
    // any trigger, triggered in +natural-first order, NEXT-reserved DONE
    // matching only (never a stale DONE), then drain-to-quiet and bytes.
    // ---------------------------------------------------------------------
    void run_natural() {
        const uint32_t beats   = (uint32_t)(natural_bytes / 8);
        const bool     first_v = (natural_first == "v");

        std::printf("== natural write-order run: bytes=%llu beats_per_dma=%u "
                    "first=%s conf=1 src_off=0 dst_off=%u ==\n",
                    (unsigned long long)natural_bytes, (unsigned)beats,
                    natural_first.c_str(), (unsigned)NAT_DST_OFF);

        nat_clear_run_state();
        setup_memory_natural();

        // Ordinary phase: no owner-only gate, no window fixture, no batch and
        // no warm-up.  reset() also re-baselines the debug DONE values, which
        // are never used for matching here.
        measurement_phase = false;
        reset(false);                     // +mode=s semantics: nonblock off
        // +mode=s: force the ordinary phase so the main-level +policy knob can
        // never make this natural run contradict its own reset(false).
        policy_partition = false;
        owner_only = false;
        scenario_start      = g_cycle;
        last_progress_cycle = g_cycle;
        v_progress_cycle    = g_cycle;

        // ---- program BOTH real DMAs completely before any trigger --------
        // CONF = 1 -> decouple_rw; source offset 0, destination offset 3072.
        cfg_wr(SEL_V, REG_CONF, 1ULL);
        cfg_wr(SEL_V, REG_SRC,  NAT_V_SRC_IOVA);
        cfg_wr(SEL_V, REG_DST,  NAT_V_DST_PAGE_IOVA + (uint64_t)NAT_DST_OFF);
        cfg_wr(SEL_V, REG_NB,   (uint64_t)natural_bytes);

        cfg_wr(SEL_A, REG_CONF, 1ULL);
        cfg_wr(SEL_A, REG_SRC,  NAT_A_SRC_IOVA);
        cfg_wr(SEL_A, REG_DST,  NAT_A_DST_PAGE_IOVA + (uint64_t)NAT_DST_OFF);
        cfg_wr(SEL_A, REG_NB,   (uint64_t)natural_bytes);

        // ---- NEXT_ID triggers, in the requested order ---------------------
        uint64_t v_id = 0, a_id = 0;
        if (first_v) {
            v_id = trigger(SEL_V);
            a_id = trigger(SEL_A);
        } else {
            a_id = trigger(SEL_A);
            v_id = trigger(SEL_V);
        }

        // ---- DONE waits match ONLY the reserved NEXT ids ------------------
        if (first_v) {
            wait_done(SEL_V, v_id, "natural V done");
            wait_done(SEL_A, a_id, "natural A done");
        } else {
            wait_done(SEL_A, a_id, "natural A done");
            wait_done(SEL_V, v_id, "natural V done");
        }

        check_quiet("natural");

        nat_verify_and_report(beats);
    }

    // ---------------------------------------------------------------------
    // Terminal acceptance: exact bytes, per-DMA physical data geometry,
    // virtual AW / WLAST queue drained, R-stall totals reported.  No claim
    // about any particular AW ordering or stall existence is made here.
    // ---------------------------------------------------------------------
    void nat_verify_and_report(uint32_t beats) {
        // ---- (1) exact byte geometry -------------------------------------
        for (uint32_t p = 0; p < NAT_MAX_PAGES; ++p)
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                const size_t k = (size_t)p * PAGE_BEATS + i;
                CHECK(read64(NAT_V_SRC_PA + (uint64_t)k * 8) == nat_src_saved[0][k],
                      "natural: V source bytes were modified by the copy");
                CHECK(read64(NAT_A_SRC_PA + (uint64_t)k * 8) == nat_src_saved[1][k],
                      "natural: A source bytes were modified by the copy");
            }

        const uint32_t off_beats = NAT_DST_OFF / 8;              // 384
        const uint32_t dst_beats = NAT_DST_PAGES * PAGE_BEATS;   // 4608
        for (uint32_t k = 0; k < dst_beats; ++k) {
            const bool copied = (k >= off_beats) && (k < off_beats + beats);
            const uint64_t want_v = copied ? nat_src_saved[0][k - off_beats]
                                           : nat_dst_saved[0][k];
            const uint64_t want_a = copied ? nat_src_saved[1][k - off_beats]
                                           : nat_dst_saved[1][k];
            CHECK(read64(NAT_V_DST_PAGE_PA + (uint64_t)k * 8) == want_v,
                  "natural: V destination bytes differ from the copied source "
                  "(untouched head/tail must keep their pre-copy value)");
            CHECK(read64(NAT_A_DST_PAGE_PA + (uint64_t)k * 8) == want_a,
                  "natural: A destination bytes differ from the copied source "
                  "(untouched head/tail must keep their pre-copy value)");
        }

        // ---- (2) per-DMA physical read / write data geometry --------------
        const uint64_t want = (uint64_t)beats;
        CHECK(nat_pr_beats[NR_VSRC] == want,
              "natural: V physical source read beats != natural_bytes/8");
        CHECK(nat_pr_beats[NR_ASRC] == want,
              "natural: A physical source read beats != natural_bytes/8");
        CHECK(nat_pw_beats[NR_VDST] == want,
              "natural: V physical destination write beats != natural_bytes/8");
        CHECK(nat_pw_beats[NR_ADST] == want,
              "natural: A physical destination write beats != natural_bytes/8");
        CHECK(nat_pr_beats[NR_VDST] == 0 && nat_pr_beats[NR_ADST] == 0 &&
              nat_pw_beats[NR_VSRC] == 0 && nat_pw_beats[NR_ASRC] == 0,
              "natural: DDR read/write regions crossed over");

        // ---- (3) virtual AW / WLAST bookkeeping ---------------------------
        // The virtual-W observer pairs a W beat only with the head of the queue
        // of virtual AW handshakes it really saw.  A legal W-before-AW (a W beat
        // that precedes its own AW on this port) cannot be paired by this
        // observer, so the failures below are reported as OBSERVER limitations,
        // never as a DUT protocol verdict.
        CHECK(n_nat_var > 0 && n_nat_vaw > 0,
              "natural: no virtual read/write request was observed");
        CHECK(n_nat_vw_beats == 2ULL * want,
              "natural: virtual W beats != both copies' destination beats; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vwlast == n_nat_vaw,
              "natural: virtual WLAST count != virtual AW count; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vw_unattr == 0,
              "natural: unattributed virtual W beat observed; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(n_nat_vw_mismatch == 0,
              "natural: paired virtual W length mismatch; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");
        CHECK(nat_vaw_q.empty(),
              "natural: virtual AW / WLAST pairing queue did not drain; "
              "observer cannot pair legal W-before-AW; not a DUT verdict");

        // ---- (4) summary (no ordering and no stall claim) -----------------
        std::printf("NATURAL-SUMMARY bytes=%llu first=%s vaw=%llu var=%llu "
                    "vw_beats=%llu vwlast=%llu v_src_r=%llu v_dst_w=%llu "
                    "a_src_r=%llu a_dst_w=%llu rstall_cycles=%llu "
                    "rstall_episodes=%llu vw_unattributed=%llu "
                    "vw_len_mismatch=%llu\n",
                    (unsigned long long)natural_bytes, natural_first.c_str(),
                    (unsigned long long)n_nat_vaw, (unsigned long long)n_nat_var,
                    (unsigned long long)n_nat_vw_beats,
                    (unsigned long long)n_nat_vwlast,
                    (unsigned long long)nat_pr_beats[NR_VSRC],
                    (unsigned long long)nat_pw_beats[NR_VDST],
                    (unsigned long long)nat_pr_beats[NR_ASRC],
                    (unsigned long long)nat_pw_beats[NR_ADST],
                    (unsigned long long)nat_rstall_cycles,
                    (unsigned long long)nat_rstall_episodes,
                    (unsigned long long)n_nat_vw_unattr,
                    (unsigned long long)n_nat_vw_mismatch);

        nat_report_json(beats);
    }

    // Only reached after every CHECK above passed; the completed/verified
    // flags are therefore literal for THIS run and never printed otherwise.
    void nat_report_json(uint32_t beats) {
        std::printf("NATURAL-JSON {"
            "\"schema\":\"natural-write-order-v1\","
            "\"completed\":true,\"verified\":true,"
            "\"natural\":1,\"natural_bytes\":%llu,\"natural_first\":\"%s\","
            "\"conf\":1,\"src_offset\":0,\"dst_offset\":%u,"
            "\"vaw\":%llu,\"var\":%llu,\"vw_beats\":%llu,\"vwlast\":%llu,"
            "\"vaw_queue_remaining\":%llu,"
            "\"vw_unattributed\":%llu,\"vw_len_mismatch\":%llu,"
            "\"v_src_r_beats\":%llu,\"v_dst_w_beats\":%llu,"
            "\"a_src_r_beats\":%llu,\"a_dst_w_beats\":%llu,"
            "\"want_beats_per_dma\":%llu,"
            "\"v_src_ar_bursts\":%llu,\"v_dst_aw_bursts\":%llu,"
            "\"a_src_ar_bursts\":%llu,\"a_dst_aw_bursts\":%llu,"
            "\"par_total\":%llu,\"paw_total\":%llu,"
            "\"pw_total\":%llu,\"pr_total\":%llu,"
            "\"rstall_cycles\":%llu,\"rstall_episodes\":%llu,"
            "\"rstall_vsrc\":%llu,\"rstall_vdst\":%llu,"
            "\"rstall_asrc\":%llu,\"rstall_adst\":%llu,\"rstall_other\":%llu,"
            "\"trace\":\"%s\"}\n",
            (unsigned long long)natural_bytes, natural_first.c_str(),
            (unsigned)NAT_DST_OFF,
            (unsigned long long)n_nat_vaw, (unsigned long long)n_nat_var,
            (unsigned long long)n_nat_vw_beats, (unsigned long long)n_nat_vwlast,
            (unsigned long long)nat_vaw_q.size(),
            (unsigned long long)n_nat_vw_unattr,
            (unsigned long long)n_nat_vw_mismatch,
            (unsigned long long)nat_pr_beats[NR_VSRC],
            (unsigned long long)nat_pw_beats[NR_VDST],
            (unsigned long long)nat_pr_beats[NR_ASRC],
            (unsigned long long)nat_pw_beats[NR_ADST],
            (unsigned long long)beats,
            (unsigned long long)nat_par_bursts[NR_VSRC],
            (unsigned long long)nat_paw_bursts[NR_VDST],
            (unsigned long long)nat_par_bursts[NR_ASRC],
            (unsigned long long)nat_paw_bursts[NR_ADST],
            (unsigned long long)n_nat_par, (unsigned long long)n_nat_paw,
            (unsigned long long)n_nat_pw, (unsigned long long)n_nat_pr,
            (unsigned long long)nat_rstall_cycles,
            (unsigned long long)nat_rstall_episodes,
            (unsigned long long)nat_rstall_region[NR_VSRC],
            (unsigned long long)nat_rstall_region[NR_VDST],
            (unsigned long long)nat_rstall_region[NR_ASRC],
            (unsigned long long)nat_rstall_region[NR_ADST],
            (unsigned long long)nat_rstall_region[NR_OTHER],
            nat_trace_path.c_str());
    }

    void tick() {
        top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;"""

CPP_ANCHOR_SAMPLE = r"""        any_fire = f_ddr_ar || f_ddr_aw || f_ddr_w || f_ddr_b || f_ddr_r;"""

CPP_REPL_SAMPLE = r"""        any_fire = f_ddr_ar || f_ddr_aw || f_ddr_w || f_ddr_b || f_ddr_r;

        // ================= natural write-order sample (PRE-EDGE) ==========
        // Same sample position as the existing diagnostics: before the rising
        // edge and before any harness queue is mutated, so caw_q.front() and
        // mem_q.front() are still the head that this cycle's W/R handshake
        // belongs to.  Observation only: it writes the +natural-trace file and
        // the natural counters and drives nothing.  Inert unless +natural=1.
        if (natural_enable) {
            // ---- virtual (translation-request port) handshakes -----------
            if (top->nat_vaw_valid && top->nat_vaw_ready) {
                NatVawEnt e;
                e.id     = (uint32_t)top->nat_vaw_id;
                e.addr   = (uint64_t)top->nat_vaw_addr;
                e.len    = (uint32_t)top->nat_vaw_len;
                e.got    = 0;
                e.region = nat_vregion(e.addr);
                ++n_nat_vaw;
                nat_emit("vaw", e.region, e.id, e.addr, e.len, n_nat_vaw, true);
                nat_vaw_q.push_back(e);
            }
            if (top->nat_var_valid && top->nat_var_ready) {
                ++n_nat_var;
                nat_emit("var", nat_vregion(top->nat_var_addr), top->nat_var_id,
                         top->nat_var_addr, top->nat_var_len, n_nat_var, true);
            }
            if (top->nat_vw_fire) {
                const bool last = (top->nat_vw_last != 0);
                ++n_nat_vw_beats;
                if (last) ++n_nat_vwlast;
                if (nat_vaw_q.empty()) {
                    // A real virtual W beat with no observed virtual AW to
                    // attach it to: reported, never invented.
                    ++n_nat_vw_unattr;
                    nat_emit(last ? "vwlast" : "vw", NR_OTHER, 0xFFFFFFFFULL,
                             0, 0, n_nat_vw_beats, last);
                } else {
                    NatVawEnt& h = nat_vaw_q.front();
                    const uint64_t baddr = h.addr + (uint64_t)h.got * 8ULL;
                    nat_emit(last ? "vwlast" : "vw", h.region, h.id, baddr,
                             h.len, h.got, last);
                    ++h.got;
                    if (last) {
                        if (h.got != h.len + 1u) ++n_nat_vw_mismatch;
                        nat_vaw_q.pop_front();
                    }
                }
            }

            // ---- physical shared-DDR data traffic ------------------------
            // Only the real completion master (crossbar slave port 2, i.e.
            // (id>>4)&3 == 2) carries DMA payload; DS/PTW walks, the idle
            // debug port and the (silent) synthetic CPU are excluded.
            if (f_ddr_ar && (((s_ddr_ar_i >> 4) & 3u) == 2u)) {
                const int reg = nat_pregion(s_ddr_ar_a);
                ++n_nat_par;
                ++nat_par_bursts[reg];
                nat_emit("par", reg, s_ddr_ar_i, s_ddr_ar_a, s_ddr_ar_l,
                         n_nat_par, true);
            }
            if (f_ddr_aw && (((s_ddr_aw_i >> 4) & 3u) == 2u)) {
                const int reg = nat_pregion(s_ddr_aw_a);
                ++n_nat_paw;
                ++nat_paw_bursts[reg];
                nat_emit("paw", reg, s_ddr_aw_i, s_ddr_aw_a, s_ddr_aw_l,
                         n_nat_paw, true);
            }
            if (f_ddr_w && !caw_q.empty()) {
                const CawEnt& c = caw_q.front();   // BEFORE mutate / pop
                if (((c.id >> 4) & 3u) == 2u) {
                    const int reg = nat_pregion(c.addr);
                    const uint64_t baddr = c.addr + (uint64_t)c.got * 8ULL;
                    ++n_nat_pw;
                    ++nat_pw_beats[reg];
                    nat_emit(s_ddr_w_l ? "pwlast" : "pw", reg, c.id, baddr,
                             c.len, c.got, s_ddr_w_l);
                }
            }
            if (f_ddr_r && !mem_q.empty()) {
                const MemRead& h = mem_q.front();  // BEFORE pop
                if (((h.id >> 4) & 3u) == 2u) {
                    const int reg = nat_pregion(h.addr);
                    ++n_nat_pr;
                    ++nat_pr_beats[reg];
                    if (h.beat == 0)
                        nat_emit("rfirst", reg, h.id,
                                 h.addr + (uint64_t)h.beat * 8ULL, h.len, 0, true);
                    if (h.beat == h.len)
                        nat_emit("rlast", reg, h.id,
                                 h.addr + (uint64_t)h.beat * 8ULL, h.len,
                                 (uint64_t)(g_cycle - h.accepted_cycle), true);
                }
            }

            // ---- DDR read back-pressure episodes (RVALID && !RREADY) -----
            // The DDR model's RVALID hold and service rule are untouched; only
            // the episode boundaries and the head attribution are recorded.
            const bool r_stall = top->ddr_r_valid && !top->ddr_r_ready;
            if (r_stall) {
                ++nat_rstall_cycles;
                if (!nat_r_stall_prev) {
                    nat_r_stall_begin  = g_cycle;
                    nat_r_stall_id     = 0;
                    nat_r_stall_addr   = 0;
                    nat_r_stall_len    = 0;
                    nat_r_stall_region = NR_OTHER;
                    if (!mem_q.empty()) {
                        const MemRead& h = mem_q.front();
                        nat_r_stall_id   = (uint64_t)h.id;
                        nat_r_stall_addr = h.addr;
                        nat_r_stall_len  = (uint64_t)h.len;
                        if (((h.id >> 4) & 3u) == 2u)
                            nat_r_stall_region = nat_pregion(h.addr);
                    }
                    ++nat_rstall_episodes;
                    nat_emit("rblock_begin", nat_r_stall_region, nat_r_stall_id,
                             nat_r_stall_addr, nat_r_stall_len, 0, true);
                }
                ++nat_rstall_region[nat_r_stall_region];
            } else if (nat_r_stall_prev) {
                nat_emit("rblock_end", nat_r_stall_region, nat_r_stall_id,
                         nat_r_stall_addr, nat_r_stall_len,
                         (uint64_t)(g_cycle - nat_r_stall_begin), true);
            }
            nat_r_stall_prev = r_stall;
        }"""

CPP_ANCHOR_MAINDECL = r"""    std::string trace_path;
    std::string read_admit = "none";"""

CPP_REPL_MAINDECL = r"""    std::string trace_path;
    std::string read_admit = "none";
    // ---- natural write-order experiment flags (test-only) ----------------
    bool        natural = false;
    uint64_t    natural_bytes = 2048;
    std::string natural_first = "v";
    std::string natural_trace;"""

CPP_ANCHOR_ARGPARSE = r"""        if (const char* v = val("cpu-writes")) cpu_writes = (std::strtoul(v, nullptr, 0) != 0);"""

CPP_REPL_ARGPARSE = r"""        if (const char* v = val("cpu-writes")) cpu_writes = (std::strtoul(v, nullptr, 0) != 0);
        if (const char* v = val("natural")) {
            char* end = nullptr;
            const unsigned long nv = std::strtoul(v, &end, 10);
            if (end == v || *end != '\0' || nv > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +natural must be exactly 0 or 1\n");
                return 2;
            }
            natural = (nv != 0);
        }
        if (const char* v = val("natural-bytes")) {
            char* end = nullptr;
            natural_bytes = std::strtoull(v, &end, 10);
            if (end == v || *end != '\0') {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: +natural-bytes must be a complete "
                    "decimal number (2048|32768, no trailing characters)\n");
                return 2;
            }
        }
        if (const char* v = val("natural-first")) natural_first = v;
        if (const char* v = val("natural-trace")) natural_trace = v;"""

CPP_ANCHOR_VALID = r"""    if (read_period == 0) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +read-period must be > 0\n");
        return 2;
    }"""

CPP_REPL_VALID = r"""    if (read_period == 0) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +read-period must be > 0\n");
        return 2;
    }
    if (natural) {
        // +natural=1 drives only the ordinary single-task +mode=s scenario.
        // Refuse any combination with the optional request-shaping knobs
        // instead of silently running something else.
        if (mode != "s") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 requires +mode=s (got "
                "\"%s\")\n", mode.c_str());
            return 2;
        }
        if (read_admit != "none") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 cannot be combined with "
                "+read-admit=%s\n", read_admit.c_str());
            return 2;
        }
        if (cpu_writes) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 cannot be combined with "
                "+cpu-writes\n");
            return 2;
        }
        if (!(natural_bytes == 2048 || natural_bytes == 32768)) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural-bytes must be 2048|32768\n");
            return 2;
        }
        if (natural_first != "v" && natural_first != "a") {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural-first must be v|a\n");
            return 2;
        }
        if (natural_trace.empty()) {
            std::fprintf(stderr,
                "nonblock_actual_dma_test: +natural=1 needs +natural-trace=<path>\n");
            return 2;
        }
    }"""

CPP_ANCHOR_RUN = r"""        tb.policy_partition= policy;
        tb.run_scenario(mode.c_str(), enable);"""

CPP_REPL_RUN = r"""        tb.policy_partition= policy;
#ifdef ACTUAL_DMA_SYSTEM
        if (natural) {
            tb.natural_enable = true;
            tb.natural_first  = natural_first;
            tb.natural_bytes  = natural_bytes;
            tb.nat_trace_path = natural_trace;
            tb.nat_trace      = std::fopen(natural_trace.c_str(), "w");
            CHECK(tb.nat_trace, "cannot open +natural-trace output file");
            std::fprintf(tb.nat_trace, "cycle,event,region,id,addr,len,extra\n");
            std::fflush(tb.nat_trace);
            std::printf("nonblock_actual_dma_test: natural=1 bytes=%llu first=%s "
                        "trace=%s\n",
                        (unsigned long long)natural_bytes, natural_first.c_str(),
                        natural_trace.c_str());
            tb.run_natural();
        } else
#endif
        tb.run_scenario(mode.c_str(), enable);"""

CPP_ANCHOR_CATCH = r"""    } catch (const std::exception& e) {
        std::fprintf(stderr, "nonblock_actual_dma_test: SCENARIO-FAIL %s: %s\n",
                     mode.c_str(), e.what());
        rc = 1;
    }"""

CPP_REPL_CATCH = r"""    } catch (const std::exception& e) {
        std::fprintf(stderr, "nonblock_actual_dma_test: SCENARIO-FAIL %s: %s\n",
                     mode.c_str(), e.what());
#ifdef ACTUAL_DMA_SYSTEM
        if (natural) {
            // Explicit NON-success record: no PASS is claimed and no counter
            // is invented for a run that did not reach its terminal checks.
            std::printf("NATURAL-JSON {\"schema\":\"natural-write-order-v1\","
                        "\"natural\":1,\"natural_bytes\":%llu,\"natural_first\":\"%s\","
                        "\"completed\":false,\"verified\":false}\n",
                        (unsigned long long)natural_bytes, natural_first.c_str());
        }
#endif
        rc = 1;
    }"""


# ===========================================================================
# 3. build command (reuse of the §23 source list, natural parameters)
# ===========================================================================
OLD_MDIR = ("/home/xvhaocheng/work/iommu-platform/out/write-order-boundary-20261007"
            "/obj-samebank-private")
NEW_MDIR = os.path.join(OUTDIR, "obj-natural")
OLD_TOP = "/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_system_top.sv"
OLD_CPP = "/home/xvhaocheng/work/iommu-platform/tests/nonblock_actual_dma_test.cpp"


def build_command():
    cmd = json.loads(read(SRC_CMD))
    if not isinstance(cmd, list):
        raise AssertionError("%s is not a JSON list" % SRC_CMD)

    def swap(old, new, tag):
        n = cmd.count(old)
        if n != 1:
            raise AssertionError("command entry %r (%s) occurs %d times, expected 1"
                                 % (old, tag, n))
        cmd[cmd.index(old)] = new

    for need in ("-GDmaBufferDepth=512", "-GReverseReadOrderEnable=1",
                 "-GPolicyEnable=1", "-GSharedPoolMode=0", "-GPoolSlots=1",
                 "-GSameBankDmasEnable=1", "+define+TEST_COMPLETION_GATE",
                 "-DACTUAL_DMA_SYSTEM", "--top-module",
                 "nonblock_actual_dma_system_top"):
        if need not in cmd:
            raise AssertionError("required command entry missing: %r" % need)

    swap("-GDmaBufferDepth=512", "-GDmaBufferDepth=3", "DmaBufferDepth")
    swap("-GReverseReadOrderEnable=1", "-GReverseReadOrderEnable=0",
         "ReverseReadOrderEnable")

    i = cmd.index("-GSameBankDmasEnable=1")
    cmd[i + 1:i + 1] = ["-GInterleaveAwEnable=0", "-GSourceReadGateEnable=0",
                        "-GFrontierEnable=0", "-GHeaderParkMode=0"]

    swap(OLD_MDIR, NEW_MDIR, "Mdir")
    swap(OLD_TOP, DST_TOP, "system_top source")
    swap(OLD_CPP, DST_CPP, "test source")
    return cmd


# ===========================================================================
# main
# ===========================================================================
def main():
    os.makedirs(OUTDIR, exist_ok=True)

    top = read(SRC_TOP)
    top = patch(top, TOP_ANCHOR_PORTS, TOP_REPL_PORTS, "system_top ports")
    top = patch(top, TOP_ANCHOR_ASSIGN, TOP_REPL_ASSIGN, "system_top assigns")
    # nat_vaw_valid occurs exactly twice in the emitted top: once as the
    # output port declaration and once as the assign lhs.  The assign rhs is
    # "i_dma.tr_aw_valid", a different string that is not counted here.
    if top.count("nat_vaw_valid") != 2:      # port decl + assign lhs
        raise AssertionError("nat_vaw_valid count %d != 2" % top.count("nat_vaw_valid"))
    if top.count("i_dma.tr_aw_len") != 1:
        raise AssertionError("i_dma.tr_aw_len count != 1")

    cpp = read(SRC_CPP)
    for anchor, repl, tag in (
            (CPP_ANCHOR_GEOM,      CPP_REPL_GEOM,      "geometry constants"),
            (CPP_ANCHOR_DTOR,      CPP_REPL_DTOR,      "destructor"),
            (CPP_ANCHOR_MEMBERS,   CPP_REPL_MEMBERS,   "natural members"),
            (CPP_ANCHOR_TICKHEAD,  CPP_REPL_TICKHEAD,  "natural methods + tick head"),
            (CPP_ANCHOR_SAMPLE,    CPP_REPL_SAMPLE,    "tick pre-edge sample"),
            (CPP_ANCHOR_MAINDECL,  CPP_REPL_MAINDECL,  "main declarations"),
            (CPP_ANCHOR_ARGPARSE,  CPP_REPL_ARGPARSE,  "argument parsing"),
            (CPP_ANCHOR_VALID,     CPP_REPL_VALID,     "flag validation"),
            (CPP_ANCHOR_RUN,       CPP_REPL_RUN,       "run dispatch"),
            (CPP_ANCHOR_CATCH,     CPP_REPL_CATCH,     "failure JSON")):
        cpp = patch(cpp, anchor, repl, tag)
    if cpp.count("void run_natural()") != 1:
        raise AssertionError("run_natural not emitted exactly once")
    if cpp.count("NATURAL-JSON") != 2:
        raise AssertionError("NATURAL-JSON count %d != 2" % cpp.count("NATURAL-JSON"))
    if cpp.count("tb.run_scenario(mode.c_str(), enable);") != 1:
        raise AssertionError("run_scenario dispatch not preserved exactly once")

    cmd = build_command()

    write(DST_TOP, top if top.endswith("\n") else top + "\n")
    write(DST_CPP, cpp if cpp.endswith("\n") else cpp + "\n")
    write(DST_CMD, json.dumps(cmd, indent=2) + "\n")
    write(DST_SH, """#!/bin/sh
# Builds the natural write-order experiment with the command stored next to it.
set -eu
cd /home/xvhaocheng/work/iommu-platform
python3 - <<'PY'
import json, subprocess, sys
argv = json.load(open("out/natural-write-order-20261008/build-natural-command.json"))
print("+ " + " ".join(argv), file=sys.stderr)
sys.exit(subprocess.call(argv))
PY
""")

    print("")
    print("anchors applied (all count==1):")
    print("  system_top.sv    : ports, assigns")
    print("  natural_test.cpp : geometry, destructor, members, methods, tick sample,")
    print("                     main-decl, arg-parse, validation, dispatch, catch")
    print("")
    print("next steps, from /home/xvhaocheng/work/iommu-platform:")
    print("  sh out/natural-write-order-20261008/build-natural.sh")
    print("  out/natural-write-order-20261008/obj-natural/Vnonblock_actual_dma_system_top \\")
    print("      +mode=s +natural=1 +natural-bytes=32768 +natural-first=a \\")
    print("      +natural-trace=out/natural-write-order-20261008/natural-a-32768.csv")


if __name__ == "__main__":
    main()
