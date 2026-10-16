## 1) Short summary and limitations

**What this patch does.** It turns the archived C++ harness `out/e2-interface-poc-20261007/before/nonblock_actual_dma_test.cpp` into a new experiment copy `out/adaptive-fragment-witness-20261008/geometry_test.cpp` that adds exactly one scenario (`+geo-smoke=1`) plus one knob (`+geo-conf=0|1`). The scenario reuses the harness's own setup/reset/config/trigger/wait/quiet order, but skips warm, batch, gap, CPU and policy so the single V copy runs on the plain Mode0 path.

For that one manual 2048-byte copy (`SRC=V_SRC_IOVA`, `DST=V_DST_IOVA+3072`, `NB=2048`, `CONF=geo_conf`) it:

* programs the copy manually and waits on the reserved `NEXT_ID` through `DONE`;
* verifies the full 2048 B destination against the source words;
* verifies the adjacent **unwritten** bytes of both touched pages are still `page_old` (no over-write past the DMA footprint);
* counts only **comp-port** (`(id>>4)&3 == 2`) DDR traffic whose address sits in the `V_SRC_PA` / geo-destination windows — no DS (`prefix 3`), no CPU (`prefix 0`), no page-table walks;
* records every source-data AR (cycle/addr/len), every destination AW (cycle/addr/len), each source-R first/last beat cycle and the global 129th source-R beat cycle;
* prints one line `GEO-SMOKE-JSON {...}`.

**Limitations (please keep these attached to the result).**

* This is **not** an implementation of candidate “I”. It only records whether the real timing space “read length locked while W2 not yet prepared” exists for `CONF0` (2×1024 coupled read) vs `CONF1` (1×2048 decoupled read).
* No performance, area or board-level claim. The 512-beat buffer used here is the ordinary buffer; this is not a resource-saving experiment.
* The root independent I design lives in `out/adaptive-fragment-witness-20261008/codex-rule-i.md`; the existing transparent splitter is prior art and no split/reassembly innovation is claimed.
* The W2 timing itself is deliberately **not asserted** — the observation may eliminate the candidate. The script asserts only the byte/geometry facts stated in the request (256 R beats, 256 W beats, two destination 1024-byte writes, CONF0 = 2 source reads, CONF1 = 1 source read).
* All error paths (CFG error, `wait_done` failure, `check_quiet` failure, byte mismatch, queue/AW/W/B sanity checks) remain hard `CHECK`/throw failures — nothing is swallowed.

## 2) Python replacement script

Save as `out/adaptive-fragment-witness-20261008/make_geometry_test.py` and run from the repo root. It only reads the archived original and writes the new experiment copy; it never touches `tests/` or any production file.

```python
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
```

Run it once from the repo root:

```
python3 out/adaptive-fragment-witness-20261008/make_geometry_test.py
```

Any non-unique anchor raises `AssertionError` and writes nothing (the write happens only after all nine replacements succeed).

## 3) Exact compile / run suggestion (do not execute here)

The existing generated model `out/write-order-boundary-20261007/obj-samebank-private` already carries all required parameters (`PolicyEnable1 / DmaBufferDepth512 / SharedPoolMode0 / PoolSlots1 / SameBankDmasEnable1 / ReverseReadOrderEnable1 / TEST_COMPLETION_GATE`, `-CFLAGS -DACTUAL_DMA_SYSTEM`). Reuse that exact command line, changing only `-Mdir` and the trailing `.cpp`:

* `-Mdir out/adaptive-fragment-witness-20261008/obj`
* replace `.../tests/nonblock_actual_dma_test.cpp` with `out/adaptive-fragment-witness-20261008/geometry_test.cpp`

Everything else (all `-G*`, `--top-module nonblock_actual_dma_system_top`, the full SV file list and include dirs) stays byte-identical to `out/write-order-boundary-20261007/build-samebank-private-command.json`.

Then:

```
./out/adaptive-fragment-witness-20261008/obj/Vnonblock_actual_dma_system_top \
    +mode=n +geo-smoke=1 +geo-conf=0 +warmup=0 +a-pages=1

./out/adaptive-fragment-witness-20261008/obj/Vnonblock_actual_dma_system_top \
    +mode=n +geo-smoke=1 +geo-conf=1 +warmup=0 +a-pages=1
```

`+mode=n` keeps `nonblock_enable=1, policy=0`; `run_geo_smoke()` additionally forces `policy_partition=false`, so both runs take the plain Mode0 path. `+warmup=0 +a-pages=1` only shrink the finite scenario budget — geo-smoke itself never runs warm/batch/gap. Each run must print exactly one `GEO-SMOKE-JSON {...}` line and must exit with code 0; if a byte/geometry check fails it exits non-zero but still prints the JSON first.

## 4) Original source evidence (paths and line numbers)

Harness under modification
* `out/e2-interface-poc-20261007/before/nonblock_actual_dma_test.cpp`
  * V addresses/permissions: `80–85`
  * `setup_memory()` page tables + page fills: `436–496`
  * block-reset / quiet: `1179–1291`, `1293–1329`
  * manual copy + trigger + DONE: `1143–1148`, `1152–1154`, `1157–1174`
  * pre-edge DDR snapshot (`s_ddr_ar_*`, `s_ddr_aw_*`): `676–691`
  * rising-edge DDR AR/AW/W/B/R bookkeeping (push/pop of `mem_q`/`caw_q`): `1003–1043`
  * `run_scenario()` reference order: `1855–2134`
  * main flag parse / mode→enable,poli: `2141–2212`, `2259–2260`, `2312–2313`
* Top-level header for the chosen model: `out/write-order-boundary-20261007/obj-samebank-private/Vnonblock_actual_dma_system_top.h`
  * `ddr_ar_addr/id/len/valid/ready` `89–92`, `102`; `ddr_aw_addr/id/len` `234`, `78–79`; `ddr_w_*` `84–86`, `98`, `235`; `ddr_r_*` `96`, `103–106`, `237`; `ddr_aw_addr` etc. `234–237`; `dbg_dma_job_quiet` `118`; `dbg_in_flight` `125`
* Build is `-DACTUAL_DMA_SYSTEM` with the parameter set in `out/write-order-boundary-20261007/build-samebank-private-command.json` (rows `10–22`).

Mechanism facts the smoke relies on
* CONF bit0 → `decouple_rw`: `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:366–367`
  (`burst_req_o.opt.beo.decouple_aw = '0;` and `burst_req_o.opt.beo.decouple_rw = dma_reg2hw.conf.decouple.q;`).
* With `RAWCouplingAvail=0` the backend routes AW through an independent fall-through register, no R-first gating: `cva6/corev_apu/iDMA/src/idma_backend.sv:837–858` (`gen_r_aw_bypass`), AR likewise `idma_backend.sv:612–625`.
* `CONF` is written through the same `REG_CONF = 0x18` path used by `program_single()`; verified against `nonblock_actual_dma_test.cpp:98–104`, `1143–1148`.
* I-design framing and the “transparent splitter is prior art, no split-innovation claim”: `out/adaptive-fragment-witness-20261008/codex-rule-i.md`.

Assumptions / open items
* The record assumes the comp port is the only path that reads `V_SRC_PA` and writes the geo destination range; DS/CPU/page-table addresses are ignored by construction. If the model interleaves the walker onto the same comp ID inside `V_SRC_PA`, that would be a real anomaly and the `src_r`/`src_ar` counters would show it.
* The checks “CONF0 → 2 source AR, CONF1 → 1 source AR; both → 2 destination AW” encode the geometry you asked me to check; if the fabric splits differently, the JSON is still printed and the failing CHECK identifies the divergence.
