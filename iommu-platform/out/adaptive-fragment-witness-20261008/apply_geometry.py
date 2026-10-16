#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Build out/adaptive-fragment-witness-20261008/geometry_test.cpp from the
archived out/e2-interface-poc-20261007/before/nonblock_actual_dma_test.cpp.

Every anchor is asserted to occur exactly once, so a stale/edited source
fails loudly instead of silently producing a partial patch.
Reads only the archived copy; writes only the new out/ experiment copy.
"""
import os
import sys

SRC = "out/e2-interface-poc-20261007/before/nonblock_actual_dma_test.cpp"
DST = "out/adaptive-fragment-witness-20261008/geometry_test.cpp"


def rep(src: str, old: str, new: str) -> str:
    n = src.count(old)
    assert n == 1, f"anchor not unique ({n} occurrences):\n---\n{old}\n---"
    return src.replace(old, new, 1)


# ---------------------------------------------------------------------------
# (1) new class members: geo-smoke observation state
# ---------------------------------------------------------------------------
A1 = r'''    // Debug-only reset baselines.  DONE waits use the NEXT_ID captured by
    // trigger(), never these values.
    uint64_t v_done_last = 0, a_done_last = 0;'''

B1 = r'''    // ---------------- geo-smoke single-DMA geometry witness ---------------
    // Armed by +geo-smoke=1.  Records the ACTUAL shared-DDR AR/AW/R/W
    // geometry of one manual 2048-byte V copy.  The warm/batch counters stay
    // untouched because geo-smoke never runs those phases.
    bool     geo_smoke = false;
    uint32_t geo_conf  = 0;
    uint64_t geo_src_ar      = 0;   // source-data AR handshakes
    uint64_t geo_dst_aw      = 0;   // destination AW handshakes
    uint64_t geo_src_r_beats = 0;   // source-data R beats
    uint64_t geo_dst_w_beats = 0;   // destination W beats
    uint64_t geo_beat_n      = 0;   // running source R beat index (1-based)
    uint64_t geo_beat129_cycle = 0;
    bool     geo_have_beat129  = false;
    uint32_t geo_r_seg         = 0; // distinct source R segments observed
    static const uint32_t GEO_MAX_SEG = 8;
    uint64_t geo_ar_cycle[GEO_MAX_SEG] = {0};
    uint64_t geo_ar_addr [GEO_MAX_SEG] = {0};
    uint32_t geo_ar_len  [GEO_MAX_SEG] = {0};
    uint64_t geo_aw_cycle[GEO_MAX_SEG] = {0};
    uint64_t geo_aw_addr [GEO_MAX_SEG] = {0};
    uint32_t geo_aw_len  [GEO_MAX_SEG] = {0};
    uint64_t geo_r_first [GEO_MAX_SEG] = {0};
    uint64_t geo_r_last  [GEO_MAX_SEG] = {0};

    // Debug-only reset baselines.  DONE waits use the NEXT_ID captured by
    // trigger(), never these values.
    uint64_t v_done_last = 0, a_done_last = 0;'''

# ---------------------------------------------------------------------------
# (2) source-data AR capture (comp port, addr inside V_SRC_PA page)
# ---------------------------------------------------------------------------
A2 = r'''        if (f_ddr_ar) {
            ++n_ddr_ar;
            ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
            MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;'''

B2 = r'''        if (f_ddr_ar) {
            ++n_ddr_ar;
            ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
            if (geo_smoke && ((s_ddr_ar_i >> 4) & 3) == 2 &&
                s_ddr_ar_a >= V_SRC_PA && s_ddr_ar_a < (V_SRC_PA + 0x1000ULL)) {
                if (geo_src_ar < GEO_MAX_SEG) {
                    geo_ar_cycle[geo_src_ar] = (uint64_t)g_cycle;
                    geo_ar_addr[geo_src_ar]  = s_ddr_ar_a;
                    geo_ar_len[geo_src_ar]   = (uint32_t)s_ddr_ar_l;
                }
                ++geo_src_ar;
            }
            MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;'''

# ---------------------------------------------------------------------------
# (3) destination AW capture (comp port, addr inside the geo destination range)
# ---------------------------------------------------------------------------
A3 = r'''        if (f_ddr_aw) {
            ++n_ddr_aw;
            CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;'''

B3 = r'''        if (f_ddr_aw) {
            ++n_ddr_aw;
            if (geo_smoke && ((s_ddr_aw_i >> 4) & 3) == 2 &&
                s_ddr_aw_a >= (V_DST_PA + 3072ULL) &&
                s_ddr_aw_a <  (V_DST_PA + 3072ULL + 4096ULL)) {
                if (geo_dst_aw < GEO_MAX_SEG) {
                    geo_aw_cycle[geo_dst_aw] = (uint64_t)g_cycle;
                    geo_aw_addr[geo_dst_aw]  = s_ddr_aw_a;
                    geo_aw_len[geo_dst_aw]   = (uint32_t)s_ddr_aw_l;
                }
                ++geo_dst_aw;
            }
            CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;'''

# ---------------------------------------------------------------------------
# (4) destination W beat count (only when the head AW is a geo destination AW)
# ---------------------------------------------------------------------------
A4 = r'''        if (f_ddr_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();'''

B4 = r'''        if (f_ddr_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            if (geo_smoke && ((caw_q.front().id >> 4) & 3) == 2 &&
                caw_q.front().addr >= (V_DST_PA + 3072ULL))
                ++geo_dst_w_beats;
            CawEnt& c = caw_q.front();'''

# ---------------------------------------------------------------------------
# (5) source-data R beat capture (head of mem_q BEFORE the pop)
# ---------------------------------------------------------------------------
A5 = r'''        if (f_ddr_r) {
            ++n_ddr_r;
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");'''

B5 = r'''        if (f_ddr_r) {
            ++n_ddr_r;
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");
            if (geo_smoke && ((h.id >> 4) & 3) == 2 &&
                h.addr >= V_SRC_PA && h.addr < (V_SRC_PA + 0x1000ULL)) {
                ++geo_src_r_beats;
                ++geo_beat_n;
                if (geo_beat_n == 129 && !geo_have_beat129) {
                    geo_beat129_cycle = (uint64_t)g_cycle;
                    geo_have_beat129  = true;
                }
                if (h.beat == 0) {
                    if (geo_r_seg < GEO_MAX_SEG) geo_r_first[geo_r_seg] = (uint64_t)g_cycle;
                }
                if (h.beat == h.len) {
                    if (geo_r_seg < GEO_MAX_SEG) geo_r_last[geo_r_seg] = (uint64_t)g_cycle;
                    ++geo_r_seg;
                }
            }'''

# ---------------------------------------------------------------------------
# (6) append run_geo_smoke() + report_geo_smoke() before the class close
# ---------------------------------------------------------------------------
A6 = r'''                    v_gap_cycles, ptw_latency);
    }
};'''

B6 = r'''                    v_gap_cycles, ptw_latency);
    }

    // ---------------------------------------------------------------------
    // geo-smoke: one manual 2048-byte V copy used as a single-DMA geometry
    // witness.  Same setup/reset/config/trigger/quiet order as run_scenario()
    // but without the warm, batch, CPU and gap phases.
    // ---------------------------------------------------------------------
    void run_geo_smoke(bool enable) {
        std::printf("== geo-smoke scenario (geo_conf=%u, enable=%d) ==\n",
                    geo_conf, (int)enable);

        setup_memory(a_pages);
        // Keep the owner-only measurement phase OFF and the policy gate OFF
        // so this is the plain Mode0 path; geo-smoke is a cold single copy.
        measurement_phase = false;
        policy_partition  = false;
        reset(enable);
        scenario_start      = g_cycle;
        last_progress_cycle = g_cycle;
        v_progress_cycle    = g_cycle;

        // Fresh geometry record (reset() ticks are discarded).
        geo_src_ar = geo_dst_aw = geo_src_r_beats = geo_dst_w_beats = 0;
        geo_beat_n = 0;
        geo_beat129_cycle = 0;
        geo_have_beat129  = false;
        geo_r_seg = 0;
        for (uint32_t i = 0; i < GEO_MAX_SEG; ++i) {
            geo_ar_cycle[i] = geo_ar_addr[i] = 0; geo_ar_len[i] = 0;
            geo_aw_cycle[i] = geo_aw_addr[i] = 0; geo_aw_len[i] = 0;
            geo_r_first[i]  = geo_r_last[i]  = 0;
        }

        // One manual copy: CONF = geo_conf (bit0 = decouple_rw),
        // SRC = V_SRC_IOVA, DST = V_DST_IOVA + 3072, NB = 2048.
        cfg_wr(SEL_V, REG_CONF, (uint64_t)(geo_conf & 1u));
        cfg_wr(SEL_V, REG_SRC,  V_SRC_IOVA);
        cfg_wr(SEL_V, REG_DST,  V_DST_IOVA + 3072ULL);
        cfg_wr(SEL_V, REG_NB,   2048ULL);

        const uint64_t id = trigger(SEL_V);
        wait_done(SEL_V, id, "geo-smoke");
        check_quiet("geo-smoke");

        // Emit the observed geometry FIRST: even if a later CHECK fails, the
        // single-line record is still available for analysis.
        report_geo_smoke();

        // ---- exact byte check of the whole 2048 B copy -------------------
        for (uint32_t i = 0; i < 256; ++i) {
            CHECK(read64(V_SRC_PA + (uint64_t)i * 8) == page_word(0, i),
                  "geo-smoke: source page was modified");
            CHECK(read64(V_DST_PA + 3072ULL + (uint64_t)i * 8) == page_word(0, i),
                  "geo-smoke: destination bytes do not match the copied source");
        }
        // ---- adjacent UNWRITTEN bytes must retain their pre-write value --
        for (uint32_t i = 0; i < 384; ++i) {
            CHECK(read64(V_DST_PA + (uint64_t)i * 8) == page_old(0, i),
                  "geo-smoke: unwritten head of the V destination page changed");
        }
        for (uint32_t i = 128; i < 512; ++i) {
            CHECK(read64(V_COLD_DST_PA + (uint64_t)i * 8) == page_old(0, i),
                  "geo-smoke: unwritten tail of the cold destination page changed");
        }

        // ---- geometry acceptance (depends on geo_conf) -------------------
        CHECK(geo_src_r_beats == 256, "geo-smoke: source R beats != 256");
        CHECK(geo_dst_w_beats == 256, "geo-smoke: destination W beats != 256");
        CHECK(geo_dst_aw == 2, "geo-smoke: destination AW count != 2");
        CHECK(geo_have_beat129, "geo-smoke: global 129th source R beat not seen");
        if (geo_conf == 0) {
            CHECK(geo_src_ar == 2, "geo-smoke(CONF0): source AR count != 2");
            if (geo_src_ar == 2) {
                CHECK(geo_ar_len[0] == 127 && geo_ar_len[1] == 127,
                      "geo-smoke(CONF0): source ARs are not two 1024-byte bursts");
            }
        } else {
            CHECK(geo_src_ar == 1, "geo-smoke(CONF1): source AR count != 1");
            if (geo_src_ar >= 1) {
                CHECK(geo_ar_len[0] == 255,
                      "geo-smoke(CONF1): source AR is not one 2048-byte burst");
            }
        }
    }

    // ---------------------------------------------------------------------
    // geo-smoke JSON line.  Every field is a raw observation of the manual
    // single-DMA copy, not a warm/batch counter.
    // ---------------------------------------------------------------------
    void report_geo_smoke() {
        std::string s;
        char hdr[1024];
        std::snprintf(hdr, sizeof(hdr),
            "GEO-SMOKE-JSON {\"schema\":\"geo-smoke-v1\","
            "\"geo_conf\":%u,\"nb_bytes\":%u,"
            "\"src_iova\":%llu,\"dst_iova\":%llu,"
            "\"src_pa\":%llu,\"dst_pa\":%llu,"
            "\"src_ar_count\":%llu,\"dst_aw_count\":%llu,"
            "\"src_r_beats\":%llu,\"dst_w_beats\":%llu,",
            geo_conf, 2048u,
            (unsigned long long)V_SRC_IOVA,
            (unsigned long long)(V_DST_IOVA + 3072ULL),
            (unsigned long long)V_SRC_PA,
            (unsigned long long)(V_DST_PA + 3072ULL),
            (unsigned long long)geo_src_ar,
            (unsigned long long)geo_dst_aw,
            (unsigned long long)geo_src_r_beats,
            (unsigned long long)geo_dst_w_beats);
        s += hdr;

        s += "\"src_ar\":[";
        for (uint32_t i = 0; i < geo_src_ar && i < GEO_MAX_SEG; ++i) {
            char b[128];
            std::snprintf(b, sizeof(b), "%s{\"c\":%llu,\"a\":%llu,\"len\":%u}",
                (i ? "," : ""),
                (unsigned long long)geo_ar_cycle[i],
                (unsigned long long)geo_ar_addr[i],
                (unsigned)geo_ar_len[i]);
            s += b;
        }
        s += "],\"dst_aw\":[";
        for (uint32_t i = 0; i < geo_dst_aw && i < GEO_MAX_SEG; ++i) {
            char b[128];
            std::snprintf(b, sizeof(b), "%s{\"c\":%llu,\"a\":%llu,\"len\":%u}",
                (i ? "," : ""),
                (unsigned long long)geo_aw_cycle[i],
                (unsigned long long)geo_aw_addr[i],
                (unsigned)geo_aw_len[i]);
            s += b;
        }
        s += "],\"src_r\":[";
        for (uint32_t i = 0; i < geo_r_seg && i < GEO_MAX_SEG; ++i) {
            char b[128];
            std::snprintf(b, sizeof(b), "%s{\"first\":%llu,\"last\":%llu}",
                (i ? "," : ""),
                (unsigned long long)geo_r_first[i],
                (unsigned long long)geo_r_last[i]);
            s += b;
        }
        s += "]";
        {
            char b[160];
            std::snprintf(b, sizeof(b),
                ",\"beat129_cycle\":%lld,\"beat129_seen\":%d}\n",
                geo_have_beat129 ? (long long)geo_beat129_cycle : -1LL,
                (int)geo_have_beat129);
            s += b;
        }
        std::printf("%s", s.c_str());
    }
};'''

# ---------------------------------------------------------------------------
# (7a) flag locals in main()
# ---------------------------------------------------------------------------
A7A = r'''    bool        v_cold_dst = false;
    bool        owner_only = false;'''

B7A = r'''    bool        v_cold_dst = false;
    bool        owner_only = false;
    bool        geo_smoke  = false;
    uint32_t    geo_conf   = 0;'''

# ---------------------------------------------------------------------------
# (7b) flag parsing in main()
# ---------------------------------------------------------------------------
A7B = r'''        if (const char* v = val("owner-only")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +owner-only='%s' (expected 0|1)\n", v);
                return 2;
            }
            owner_only = (parsed != 0);
        }'''

B7B = r'''        if (const char* v = val("owner-only")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +owner-only='%s' (expected 0|1)\n", v);
                return 2;
            }
            owner_only = (parsed != 0);
        }
        if (const char* v = val("geo-smoke")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +geo-smoke='%s' (expected 0|1)\n", v);
                return 2;
            }
            geo_smoke = (parsed != 0);
        }
        if (const char* v = val("geo-conf")) {
            char* end = nullptr;
            const unsigned long parsed = std::strtoul(v, &end, 0);
            if (end == v || *end != '\0' || parsed > 1) {
                std::fprintf(stderr,
                    "nonblock_actual_dma_test: invalid +geo-conf='%s' (expected 0|1)\n", v);
                return 2;
            }
            geo_conf = (uint32_t)parsed;
        }'''

# ---------------------------------------------------------------------------
# (8) branch in main() to run_geo_smoke() when armed
# ---------------------------------------------------------------------------
A8 = r'''        tb.policy_partition= policy;
        tb.run_scenario(mode.c_str(), enable);'''

B8 = r'''        tb.policy_partition= policy;
        if (geo_smoke) {
            tb.geo_smoke = true;
            tb.geo_conf  = geo_conf;
            tb.run_geo_smoke(enable);
        } else {
            tb.run_scenario(mode.c_str(), enable);
        }'''


def main() -> int:
    with open(SRC, "r", encoding="utf-8") as f:
        s = f.read()

    for i, (old, new) in enumerate(
            [(A1, B1), (A2, B2), (A3, B3), (A4, B4), (A5, B5),
             (A6, B6), (A7A, B7A), (A7B, B7B), (A8, B8)], start=1):
        s = rep(s, old, new)
        print(f"  patch {i}: ok")

    assert SRC != DST
    os.makedirs(os.path.dirname(DST), exist_ok=True)
    with open(DST, "w", encoding="utf-8") as f:
        f.write(s)
    print("wrote", DST)
    return 0


if __name__ == "__main__":
    sys.exit(main())
