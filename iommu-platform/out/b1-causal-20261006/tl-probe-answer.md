Below is a bounded, test-only proposal. Production RTL is untouched. The patch uses only the three existing `composition_tl` test files and adds a `--history-probe` branch with output directory `out/b1-causal-20261006/tl-probe`. Default test behavior is preserved.

## Interface assumptions / uncertainties

1. **Verilator 4.038 XMR support**: `composition_tl_top.sv` reads internal signals from `i_dut.i_rv_iommu_iotlb.*`. If Verilator rejects non-public cross-module references, the root will need to expose those signals through the existing `rv_iommu_tl_wrap` `research_o` struct or mark the production signals `/*verilator public*/` (production RTL is frozen, so this is a root decision).
2. **Packed entry layout**: entry word extraction assumes `research_o.entries[i]` is 128 bits and the existing bit fields remain at `[28:0]`, `[48:29]`, `[51]`, `[49]`, `[50]`, `[52]`, `[53]`, `[54]`, `[55]`, `[56]`, `[79:64]`.
3. **A-page count**: 15 A pages (`A0..A14`) are warmed into 14 A-eligible ways. The proposal therefore does **not** hard-assert “all 15 A pages resident”; it records the actual residency state and compares prefix snapshots.
4. **Successor page**: the proposal is adaptive: it runs `PN` first, takes the actual evicted A page from `PN`’s first `A15` fill, then runs `P` and `PNlate` with that same successor IOVA. If the root wants fixed `A0`, this can be simplified later after evidence.
5. **No execution claim**: I cannot execute Verilator. The root must build/run and verify the actual victim ways and PLRU trees.

---

## 1. `tests/composition_tl_top.sv`

Add new passive output ports and assignments. No production RTL edits.

```diff
diff --git a/tests/composition_tl_top.sv b/tests/composition_tl_top.sv
--- a/tests/composition_tl_top.sv
+++ b/tests/composition_tl_top.sv
@@
-    output logic [15:0] research_valid_q_o,
-    output logic [15:0] research_valid_n_o
+    output logic [15:0] research_valid_q_o,
+    output logic [15:0] research_valid_n_o,
+
+    // History probe passive outputs (test observation only; no functional effect)
+    output logic [31:0]   history_plru_o,
+    output logic [2047:0] history_entries_o,
+    output logic          history_lookup_o,
+    output logic          history_touch_o,
+    output logic          history_lu_hit_o,
+    output logic [15:0]   history_lu_hitway_o,
+    output logic [28:0]   history_lu_vpn_o,
+    output logic [19:0]   history_lu_pscid_o,
+    output logic [28:0]   history_update_vpn_o,
+    output logic [19:0]   history_update_pscid_o,
+    output logic [15:0]   history_allocation_mask_o
 );
@@
     assign research_update_o       = research_o_int.update;
     assign research_did_o          = research_o_int.did;
     assign research_replace_mask_o = research_o_int.replace_mask;
+
+    // History probe passive flattening.  No functional effect.
+    always_comb begin
+        history_entries_o = '0;
+        for (int unsigned i = 0; i < 16; i++) begin
+            history_entries_o[128*i +: 128] = research_o_int.entries[i];
+        end
+    end
+
+    assign history_plru_o            = research_o_int.plru[31:0];
+    assign history_lookup_o          = i_dut.i_rv_iommu_iotlb.lookup_i;
+    assign history_touch_o           = i_dut.i_rv_iommu_iotlb.touch_i;
+    assign history_lu_hit_o          = i_dut.i_rv_iommu_iotlb.lu_hit_o;
+    assign history_lu_hitway_o       = i_dut.i_rv_iommu_iotlb.lu_hit;
+    assign history_lu_vpn_o          = i_dut.i_rv_iommu_iotlb.lu_vpn_i;
+    assign history_lu_pscid_o        = i_dut.i_rv_iommu_iotlb.lu_pscid_i;
+    assign history_update_vpn_o      = i_dut.i_rv_iommu_iotlb.update_i.vpn;
+    assign history_update_pscid_o    = i_dut.i_rv_iommu_iotlb.update_i.pscid;
+    assign history_allocation_mask_o = i_dut.i_rv_iommu_iotlb.allocation_mask;
 
 endmodule
```

---

## 2. `tests/composition_tl_test.cpp`

This adds history-probe helpers and `case_history_probe()`. Existing default cases are unchanged.

### 2.1 Includes and structs

```diff
diff --git a/tests/composition_tl_test.cpp b/tests/composition_tl_test.cpp
--- a/tests/composition_tl_test.cpp
+++ b/tests/composition_tl_test.cpp
@@
 #include <cstdint>
 #include <cstdio>
 #include <cstdlib>
+#include <algorithm>
+#include <array>
 #include <deque>
+#include <fstream>
 #include <map>
+#include <sstream>
 #include <string>
 #include <vector>
@@
 struct Fill {
     uint32_t did  = 0;
     uint16_t mask = 0;
 };
+
+struct EntrySnap {
+    uint32_t vpn   = 0;
+    uint32_t pscid = 0;
+    uint16_t gscid = 0;
+    bool     valid = false;
+    bool     en1   = false;
+    bool     en2   = false;
+    bool     is1g   = false;
+    bool     is2m   = false;
+    bool     c2s1g  = false;
+    bool     c2s2m  = false;
+    bool     g      = false;
+};
+
+struct RunResult {
+    std::string name;
+    bool n_enable = false;
+    bool partition = false;
+    bool late_v = false;
+    uint32_t prefix_plru = 0;
+    std::array<EntrySnap, 16> prefix_entries{};
+    int prefix_a_fill_count = 0;
+    bool prefix_all_a_resident = false;
+    uint32_t a15_pre_plru = 0;
+    uint32_t a15_post_plru = 0;
+    std::array<EntrySnap, 16> a15_pre_entries{};
+    std::array<EntrySnap, 16> a15_post_entries{};
+    uint16_t a15_fillmask = 0;
+    uint32_t a15_fill_did = 0;
+    int a15_fill_way = -1;
+    int a15_fill_count = 0;
+    uint32_t a15_update_vpn = 0;
+    uint32_t a15_update_pscid = 0;
+    uint32_t a15_evicted_vpn = 0;
+    uint32_t a15_evicted_pscid = 0;
+    int a15_evicted_a_index = -1;
+    bool a15_onehot_ok = false;
+    bool a15_writablemask_ok = false;
+    bool a15_committed_ok = false;
+    bool fast_accept_during_hold = false;
+    int fast_effective_touch_count = 0;
+    int fast_fill_during_hold = 0;
+    bool p_no_accept_during_hold = false;
+    uint32_t successor_iova = 0;
+    uint32_t successor_vpn = 0;
+    bool successor_resident_in_prefix = false;
+    bool successor_hit = false;
+    bool successor_saw_ptw = false;
+    int successor_fills = 0;
+    Resp successor_resp;
+    std::vector<uint16_t> a_fill_masks;
+};
+
+struct ProbeResult {
+    bool hit = false;
+    bool saw_ptw = false;
+    int fills = 0;
+    Resp resp;
+};
```

### 2.2 New `Tb` members

```diff
@@
     std::vector<Fill> fills;
 
     bool latched_req_fire = false;
+
+    bool history_mode = false;
+    std::string history_run;
+    int history_cycle = 0;
+    std::string history_phase;
+    std::ofstream history_events;
+    RunResult* current_result = nullptr;
+    bool a15_phase = false;
+    bool a15_fill_seen = false;
+    bool a15_post_captured = false;
```

### 2.3 Hook `tick()`

```diff
@@
     // -- one full clock cycle -------------------------------------------
     void tick() {
+        if (history_mode) history_cycle++;
         top->ddtw_ar_ready = ddtw_rd.pending ? 0 : 1;
         top->ptw_ar_ready  = ptw_rd.pending  ? 0 : 1;
         drive_ddtw_r();
         drive_ptw_r();
 
         top->eval();
+        if (history_mode) history_pre_edge();
 
         const bool     d_ar_fire = top->ddtw_ar_valid && top->ddtw_ar_ready;
@@
         top->clk_i = 1;
         top->eval();
+        if (history_mode) history_post_edge();
 
         d_ar_fire_last = d_ar_fire;
```

### 2.4 Insert history-probe helpers before `fast_prove_under_cold_a`

Insert after `drain_quiet()` and before `// Hold a bank-different cold A page...`.

```cpp
    // -- history probe helpers -------------------------------------------
    EntrySnap read_entry(int i) {
        EntrySnap e;
        uint32_t w0 = top->history_entries_o[4*i + 0];
        uint32_t w1 = top->history_entries_o[4*i + 1];
        uint32_t w2 = top->history_entries_o[4*i + 2];
        e.vpn   = w0 & 0x1FFFFFFF;
        e.pscid = ((w0 >> 29) & 0x7) | ((w1 & 0x1FFFF) << 3);
        e.valid = ((w1 >> 19) & 1) != 0;
        e.en1   = ((w1 >> 17) & 1) != 0;
        e.en2   = ((w1 >> 18) & 1) != 0;
        e.is1g  = ((w1 >> 20) & 1) != 0;
        e.is2m  = ((w1 >> 21) & 1) != 0;
        e.c2s1g = ((w1 >> 22) & 1) != 0;
        e.c2s2m = ((w1 >> 23) & 1) != 0;
        e.g     = ((w1 >> 24) & 1) != 0;
        e.gscid = (uint16_t)(w2 & 0xFFFF);
        return e;
    }

    std::array<EntrySnap, 16> read_all_entries() {
        std::array<EntrySnap, 16> a;
        for (int i = 0; i < 16; ++i) a[i] = read_entry(i);
        return a;
    }

    static int a_index_from_vpn(uint32_t vpn) {
        if (vpn < 0x20000 || vpn > 0x2000f) return -1;
        return (int)(vpn - 0x20000);
    }

    bool all_a_resident(const std::array<EntrySnap, 16>& es) {
        for (int n = 0; n < 15; ++n) {
            uint32_t vpn = 0x20000 + (uint32_t)n;
            bool found = false;
            for (int i = 0; i < 16; ++i) {
                if (es[i].valid && es[i].pscid == 11 && es[i].vpn == vpn) {
                    found = true;
                    break;
                }
            }
            if (!found) return false;
        }
        return true;
    }

    bool find_a_entry(const std::array<EntrySnap, 16>& es, uint32_t vpn) {
        for (int i = 0; i < 16; ++i) {
            if (es[i].valid && es[i].pscid == 11 && es[i].vpn == vpn) return true;
        }
        return false;
    }

    static bool entry_equal(const EntrySnap& a, const EntrySnap& b) {
        return a.vpn == b.vpn && a.pscid == b.pscid && a.gscid == b.gscid &&
               a.valid == b.valid && a.en1 == b.en1 && a.en2 == b.en2 &&
               a.is1g == b.is1g && a.is2m == b.is2m &&
               a.c2s1g == b.c2s1g && a.c2s2m == b.c2s2m && a.g == b.g;
    }

    static bool entries_equal(const std::array<EntrySnap, 16>& a,
                              const std::array<EntrySnap, 16>& b) {
        for (int i = 0; i < 16; ++i) if (!entry_equal(a[i], b[i])) return false;
        return true;
    }

    void log_event(const char* kind, bool effective_touch, bool fill) {
        if (!history_events.is_open()) return;
        history_events << "{\"run\":\"" << history_run << "\","
                       << "\"cycle\":" << history_cycle << ","
                       << "\"phase\":\"" << history_phase << "\","
                       << "\"kind\":\"" << kind << "\","
                       << "\"accepted_fast\":"
                       << ((top->nonblock_fire_o && top->trans_req_valid_i &&
                            top->trans_req_ready_o) ? "true" : "false") << ","
                       << "\"effective_touch\":" << (effective_touch ? "true" : "false") << ","
                       << "\"fill\":" << (fill ? "true" : "false") << ","
                       << "\"fill_mask\":" << top->research_replace_mask_o << ","
                       << "\"fill_did\":" << top->research_did_o << ","
                       << "\"lu_vpn\":" << top->history_lu_vpn_o << ","
                       << "\"lu_pscid\":" << top->history_lu_pscid_o << ","
                       << "\"update_vpn\":" << top->history_update_vpn_o << ","
                       << "\"update_pscid\":" << top->history_update_pscid_o << ","
                       << "\"plru\":" << top->history_plru_o << ","
                       << "\"evicted_vpn\":"
                       << (current_result ? current_result->a15_evicted_vpn : 0) << ","
                       << "\"evicted_pscid\":"
                       << (current_result ? current_result->a15_evicted_pscid : 0)
                       << "}\n";
    }

    void history_pre_edge() {
        if (!current_result) return;
        const bool effective_touch =
            top->history_lookup_o && top->history_lu_hit_o && top->history_touch_o;
        const bool fill = top->research_update_o;
        const uint32_t did = top->research_did_o;
        const uint16_t mask = top->research_replace_mask_o;

        if (history_phase == "a15_hold") {
            if (effective_touch) current_result->fast_effective_touch_count++;
            if (fill) current_result->fast_fill_during_hold++;
        }

        if (fill && did == 11) {
            int way = -1;
            if (__builtin_popcount((unsigned)mask) == 1)
                way = __builtin_ctz((unsigned)mask);
            if (way >= 0) current_result->a_fill_masks.push_back(mask);

            if (a15_phase && way >= 0) {
                current_result->a15_fill_count++;
                if (!a15_fill_seen) {
                    a15_fill_seen = true;
                    current_result->a15_fillmask = mask;
                    current_result->a15_fill_did = did;
                    current_result->a15_fill_way = way;
                    current_result->a15_pre_plru = top->history_plru_o;
                    current_result->a15_pre_entries = read_all_entries();
                    current_result->a15_update_vpn = top->history_update_vpn_o;
                    current_result->a15_update_pscid = top->history_update_pscid_o;
                    current_result->a15_onehot_ok =
                        (__builtin_popcount((unsigned)mask) == 1);
                    current_result->a15_writablemask_ok =
                        ((mask & ~top->history_allocation_mask_o) == 0);
                    const EntrySnap& pre = current_result->a15_pre_entries[way];
                    current_result->a15_evicted_vpn = pre.vpn;
                    current_result->a15_evicted_pscid = pre.pscid;
                    current_result->a15_evicted_a_index = a_index_from_vpn(pre.vpn);
                    log_event("a15_pre", effective_touch, fill);
                }
            }
        }

        if (effective_touch) {
            log_event("effective_touch", effective_touch, fill);
        }
    }

    void history_post_edge() {
        if (!current_result) return;
        if (a15_fill_seen && !a15_post_captured) {
            a15_post_captured = true;
            current_result->a15_post_plru = top->history_plru_o;
            current_result->a15_post_entries = read_all_entries();
            int way = current_result->a15_fill_way;
            if (way >= 0) {
                const EntrySnap& post = current_result->a15_post_entries[way];
                const uint32_t a15_vpn = 0x2000f000U >> 12;
                current_result->a15_committed_ok =
                    post.valid && post.vpn == a15_vpn && post.pscid == 11;
            }
            log_event("a15_post", false, false);
        }
    }

    ProbeResult probe_page(uint32_t did, uint64_t iova,
                           uint64_t exp_spaddr, uint8_t bank) {
        int before = count_fills(did);
        set_req(did, iova, 0x2, bank, true);
        if (!wait_accept()) FAIL_AT("probe_page: request not accepted");
        set_req(did, 0, 0, 0, false);
        bool saw_ptw = false;
        Resp r = pop_resp_track_ptw(saw_ptw);
        int after = count_fills(did);
        ProbeResult pr;
        pr.hit = !saw_ptw;
        pr.saw_ptw = saw_ptw;
        pr.fills = after - before;
        pr.resp = r;
        CHECK_AT(!r.error, "probe_page: error response");
        CHECK_AT(r.bank == bank, "probe_page: bank mismatch");
        CHECK_AT(r.spaddr == exp_spaddr, "probe_page: spaddr mismatch");
        return pr;
    }

    RunResult run_history_case(const std::string& name, bool n_enable,
                               bool partition, bool late_v,
                               bool use_own_successor,
                               uint32_t forced_successor_iova) {
        RunResult res;
        res.name = name;
        res.n_enable = n_enable;
        res.partition = partition;
        res.late_v = late_v;

        history_mode = true;
        history_run = name;
        current_result = &res;
        history_cycle = 0;
        history_phase = "reset";
        a15_phase = false;
        a15_fill_seen = false;
        a15_post_captured = false;

        reset(n_enable, partition, 10);

        history_phase = "prefix";
        warm_leaf(10, 0x10000000ULL, 0x80000000ULL, 0);
        warm_leaf(10, 0x10001000ULL, 0x80001000ULL, 0);
        for (int n = 0; n < 15; ++n) {
            uint64_t iova = 0x20000000ULL + (uint64_t)n * 0x1000ULL;
            uint64_t phys = 0x90000000ULL + (uint64_t)n * 0x1000ULL;
            warm_leaf(11, iova, phys, 1);
        }
        drain_quiet();

        res.prefix_plru = top->history_plru_o;
        res.prefix_entries = read_all_entries();
        res.prefix_a_fill_count = count_fills(11);
        res.prefix_all_a_resident = all_a_resident(res.prefix_entries);

        history_phase = "a15_hold";
        start_cold_a_and_hold(0x2000f000ULL, 1);
        a15_phase = true;

        const bool v_presented = !late_v;
        if (v_presented) {
            latched_req_fire = false;
            set_req(10, 0x10000000ULL, 0x2, 0, true);
        } else {
            set_req(10, 0, 0, 0, false);
        }

        bool v_accepted = false;
        for (int c = 0; c < 60; ++c) {
            tick();
            if (v_presented && req_fire && !v_accepted) {
                v_accepted = true;
                set_req(10, 0, 0, 0, false);
            }
        }
        res.fast_accept_during_hold = v_accepted;
        res.p_no_accept_during_hold = !v_accepted;

        history_phase = "a15_release";
        hold_ptw_r = false;

        if (v_presented) {
            if (v_accepted) {
                Resp r1 = pop_resp();
                Resp r2 = pop_resp();
                bool got_a = false, got_v = false;
                if (r1.bank == 0) {
                    got_v = true;
                    CHECK_AT(!r1.error && r1.spaddr == 0x80000000ULL,
                             "history: PNearly V response mismatch");
                } else if (r1.bank == 1) {
                    got_a = true;
                    CHECK_AT(!r1.error && r1.spaddr == 0x9000f000ULL,
                             "history: PNearly A15 response mismatch");
                } else FAIL_AT("history: unexpected response bank");
                if (r2.bank == 0) {
                    got_v = true;
                    CHECK_AT(!r2.error && r2.spaddr == 0x80000000ULL,
                             "history: PNearly V response mismatch");
                } else if (r2.bank == 1) {
                    got_a = true;
                    CHECK_AT(!r2.error && r2.spaddr == 0x9000f000ULL,
                             "history: PNearly A15 response mismatch");
                } else FAIL_AT("history: unexpected response bank");
                CHECK_AT(got_a && got_v, "history: PNearly missing response");
            } else {
                Resp ra = pop_resp();
                CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x9000f000ULL,
                         "history: P A15 response mismatch");
                if (!wait_accept()) FAIL_AT("history: P V not accepted after A15");
                set_req(10, 0, 0, 0, false);
                Resp rv = pop_resp();
                CHECK_AT(rv.bank == 0 && !rv.error && rv.spaddr == 0x80000000ULL,
                         "history: P V response mismatch");
            }
        } else {
            Resp ra = pop_resp();
            CHECK_AT(ra.bank == 1 && !ra.error && ra.spaddr == 0x9000f000ULL,
                     "history: PNlate A15 response mismatch");
            ProbeResult vpr = probe_page(10, 0x10000000ULL, 0x80000000ULL, 0);
            CHECK_AT(vpr.hit, "history: PNlate V did not hit");
            CHECK_AT(vpr.fills == 0, "history: PNlate V caused fill");
        }

        drain_quiet();

        CHECK_AT(a15_fill_seen, "history: A15 fill not observed");
        CHECK_AT(res.a15_fill_count == 1, "history: A15 fill count not one");
        CHECK_AT(res.a15_onehot_ok, "history: A15 fillmask not one-hot");
        CHECK_AT(res.a15_writablemask_ok,
                 "history: A15 fillmask outside allocation mask");
        CHECK_AT(res.a15_committed_ok, "history: A15 update not committed");
        CHECK_AT(res.a15_fill_did == 11, "history: A15 fill owner DID mismatch");
        CHECK_AT(res.a15_evicted_pscid == 11, "history: A15 victim not A (pscid)");
        CHECK_AT(res.a15_evicted_a_index >= 0,
                 "history: A15 victim VPN not an A page");

        history_phase = "successor";
        uint32_t succ_iova;
        if (use_own_successor) {
            CHECK_AT(res.a15_evicted_a_index >= 0,
                     "history: no evicted A page for successor");
            succ_iova = 0x20000000U +
                        (uint32_t)res.a15_evicted_a_index * 0x1000U;
        } else {
            succ_iova = forced_successor_iova;
        }
        res.successor_iova = succ_iova;
        res.successor_vpn = succ_iova >> 12;
        res.successor_resident_in_prefix =
            find_a_entry(res.prefix_entries, res.successor_vpn);
        CHECK_AT(res.successor_resident_in_prefix,
                 "history: successor not resident in prefix");
        uint32_t succ_phys = 0x90000000U +
            ((succ_iova >> 12) - 0x20000U) * 0x1000U;
        ProbeResult pr = probe_page(11, succ_iova, succ_phys, 1);
        res.successor_hit = pr.hit;
        res.successor_saw_ptw = pr.saw_ptw;
        res.successor_fills = pr.fills;
        res.successor_resp = pr.resp;

        drain_quiet();
        history_mode = false;
        current_result = nullptr;
        return res;
    }

    std::string run_json(const RunResult& r) {
        std::stringstream ss;
        ss << "    {\"name\":\"" << r.name << "\","
           << "\"N\":" << (r.n_enable ? "1" : "0") << ","
           << "\"partition\":" << (r.partition ? "1" : "0") << ","
           << "\"late_v\":" << (r.late_v ? "true" : "false") << ","
           << "\"prefix_plru\":" << r.prefix_plru << ","
           << "\"prefix_a_fill_count\":" << r.prefix_a_fill_count << ","
           << "\"prefix_all_a_resident\":"
           << (r.prefix_all_a_resident ? "true" : "false") << ","
           << "\"a15_pre_plru\":" << r.a15_pre_plru << ","
           << "\"a15_post_plru\":" << r.a15_post_plru << ","
           << "\"a15_fill_way\":" << r.a15_fill_way << ","
           << "\"a15_fillmask\":" << r.a15_fillmask << ","
           << "\"a15_fill_did\":" << r.a15_fill_did << ","
           << "\"a15_update_vpn\":" << r.a15_update_vpn << ","
           << "\"a15_update_pscid\":" << r.a15_update_pscid << ","
           << "\"a15_evicted_vpn\":" << r.a15_evicted_vpn << ","
           << "\"a15_evicted_pscid\":" << r.a15_evicted_pscid << ","
           << "\"a15_evicted_a_index\":" << r.a15_evicted_a_index << ","
           << "\"fast_accept_during_hold\":"
           << (r.fast_accept_during_hold ? "true" : "false") << ","
           << "\"fast_effective_touch_count\":" << r.fast_effective_touch_count << ","
           << "\"fast_fill_during_hold\":" << r.fast_fill_during_hold << ","
           << "\"successor_iova\":" << r.successor_iova << ","
           << "\"successor_vpn\":" << r.successor_vpn << ","
           << "\"successor_resident_in_prefix\":"
           << (r.successor_resident_in_prefix ? "true" : "false") << ","
           << "\"successor_hit\":" << (r.successor_hit ? "true" : "false") << ","
           << "\"successor_saw_ptw\":"
           << (r.successor_saw_ptw ? "true" : "false") << ","
           << "\"successor_fills\":" << r.successor_fills << ","
           << "\"a_fill_ways\":[";
        for (size_t i = 0; i < r.a_fill_masks.size(); ++i) {
            uint16_t m = r.a_fill_masks[i];
            int w = __builtin_popcount((unsigned)m) == 1
                        ? __builtin_ctz((unsigned)m) : -1;
            if (i) ss << ",";
            ss << w;
        }
        ss << "]}";
        return ss.str();
    }

    void write_summary(const RunResult& p, const RunResult& pn,
                       const RunResult& pnlate,
                       bool prefix_match, bool first_victim_diverges,
                       bool first_victim_equal_late,
                       bool prefill_tree_changed_pn,
                       bool prefill_tree_changed_p,
                       bool prefill_tree_changed_late) {
        (void)prefill_tree_changed_p;
        (void)prefill_tree_changed_late;
        std::ofstream out("summary.json");
        out << "{\n";
        out << "  \"suite\": \"b1-causal-20261006/tl-probe\",\n";
        out << "  \"scope\": \"legal TL requests, actual rv_iommu_tl_wrap, synthetic memory responder; no board/SoC/DDR claims\",\n";
        out << "  \"actual_RTL_checked\": true,\n";
        out << "  \"synthetic_memory_responder\": true,\n";
        out << "  \"platform_reachability_not_checked\": true,\n";
        out << "  \"board_attribution_not_checked\": true,\n";
        out << "  \"runs\": [\n";
        out << run_json(p) << ",\n";
        out << run_json(pn) << ",\n";
        out << run_json(pnlate) << "\n";
        out << "  ],\n";
        out << "  \"prefix_match\": " << (prefix_match ? "true" : "false") << ",\n";
        out << "  \"first_victim_diverges_P_vs_PN\": "
            << (first_victim_diverges ? "true" : "false") << ",\n";
        out << "  \"first_victim_equal_P_vs_PNlate\": "
            << (first_victim_equal_late ? "true" : "false") << ",\n";
        out << "  \"changed_prefill_tree_P_vs_PN\": "
            << (prefill_tree_changed_pn ? "true" : "false") << ",\n";
        out << "  \"changed_prefill_tree_P_vs_PNlate\": "
            << (pnlate.a15_pre_plru != p.a15_pre_plru ? "true" : "false") << ",\n";
        out << "  \"successor_hits\": {\"P\": "
            << (p.successor_hit ? "true" : "false")
            << ", \"PN\": " << (pn.successor_hit ? "true" : "false")
            << ", \"PNlate\": " << (pnlate.successor_hit ? "true" : "false") << "},\n";
        out << "  \"successor_fill_count\": {\"P\": " << p.successor_fills
            << ", \"PN\": " << pn.successor_fills
            << ", \"PNlate\": " << pnlate.successor_fills << "},\n";
        out << "  \"count\": 3,\n";
        out << "  \"result\": \"PASS\"\n";
        out << "}\n";
    }

    void case_history_probe() {
        history_events.open("history_events.jsonl");
        RunResult pn = run_history_case("PN", true, true, false, true, 0);
        uint32_t succ_iova = pn.successor_iova;
        RunResult p = run_history_case("P", false, true, false, false, succ_iova);
        RunResult pnlate = run_history_case("PNlate", true, true, true, false, succ_iova);
        history_events.close();

        bool prefix_match =
            entries_equal(p.prefix_entries, pn.prefix_entries) &&
            entries_equal(pn.prefix_entries, pnlate.prefix_entries) &&
            p.prefix_plru == pn.prefix_plru &&
            pn.prefix_plru == pnlate.prefix_plru;
        CHECK_AT(prefix_match, "history: prefix snapshots differ across runs");

        bool first_victim_diverges =
            (p.a15_fill_way != pn.a15_fill_way) ||
            (p.a15_evicted_vpn != pn.a15_evicted_vpn);
        CHECK_AT(first_victim_diverges,
                 "history: P and PN first A15 victim do not diverge");

        bool first_victim_equal_late =
            (p.a15_fill_way == pnlate.a15_fill_way) &&
            (p.a15_evicted_vpn == pnlate.a15_evicted_vpn);
        CHECK_AT(first_victim_equal_late,
                 "history: P and PNlate first A15 victim differ");

        bool prefill_tree_changed_pn = (pn.a15_pre_plru != p.prefix_plru);
        bool prefill_tree_changed_p = (p.a15_pre_plru != p.prefix_plru);
        bool prefill_tree_changed_late =
            (pnlate.a15_pre_plru != pnlate.prefix_plru);

        CHECK_AT(prefill_tree_changed_pn,
                 "history: PN pre-fill tree did not change before A15 fill");
        CHECK_AT(pn.a15_pre_plru != p.a15_pre_plru,
                 "history: PN pre-fill tree not different from P");
        CHECK_AT(pnlate.a15_pre_plru == p.a15_pre_plru,
                 "history: PNlate pre-fill tree differs from P");

        CHECK_AT(pn.fast_accept_during_hold,
                 "history: PN did not accept V during hold");
        CHECK_AT(pn.fast_effective_touch_count == 1,
                 "history: PN effective touch count != 1");
        CHECK_AT(pn.fast_fill_during_hold == 0,
                 "history: PN fill during hold");
        CHECK_AT(!p.fast_accept_during_hold,
                 "history: P accepted V during hold");
        CHECK_AT(p.fast_effective_touch_count == 0,
                 "history: P touched during hold");
        CHECK_AT(pnlate.fast_effective_touch_count == 0,
                 "history: PNlate touched during hold");

        CHECK_AT(p.successor_hit && !p.successor_saw_ptw &&
                 p.successor_fills == 0,
                 "history: P successor expected hit/no fill");
        CHECK_AT(!pn.successor_hit && pn.successor_saw_ptw &&
                 pn.successor_fills == 1,
                 "history: PN successor expected miss/1 fill");
        CHECK_AT(pnlate.successor_hit && !pnlate.successor_saw_ptw &&
                 pnlate.successor_fills == 0,
                 "history: PNlate successor expected hit/no fill");

        write_summary(p, pn, pnlate, prefix_match, first_victim_diverges,
                      first_victim_equal_late, prefill_tree_changed_pn,
                      prefill_tree_changed_p, prefill_tree_changed_late);
        std::printf("composition_tl_test: history probe PASS\n");
    }
```

### 2.5 Modify `main`

```diff
@@
 int main(int argc, char** argv) {
     Verilated::commandArgs(argc, argv);
+    bool history_probe = false;
+    for (int i = 1; i < argc; ++i) {
+        if (std::string(argv[i]) == "--history-probe") history_probe = true;
+    }
     Tb tb;
     tb.setup_memory();
 
+    if (history_probe) {
+        tb.case_history_probe();
+        return 0;
+    }
+
     tb.case_mode_gating("S",  false, false);
     tb.case_mode_gating("N",  true,  false);
     tb.case_mode_gating("P",  false, true);
     tb.case_mode_gating("PN", true,  true);
```

---

## 3. `tests/test_composition_tl.py`

Add `--history-probe` flag. Default output directory remains `out/composition-tl`.

```diff
diff --git a/tests/test_composition_tl.py b/tests/test_composition_tl.py
--- a/tests/test_composition_tl.py
+++ b/tests/test_composition_tl.py
@@
 def main() -> int:
+    history_probe = "--history-probe" in sys.argv[1:]
     pkg = IOMMU / "packages"
     rtl = IOMMU / "rtl" / "translation_logic"
     cc = IOMMU / "vendor" / "common_cells" / "src"
     axi = IOMMU / "vendor" / "axi" / "src"
@@
-    obj_dir = OUT / "obj"
+    out_root = ROOT / "out"
+    if history_probe:
+        out_root = out_root / "b1-causal-20261006" / "tl-probe"
+    else:
+        out_root = out_root / "composition-tl"
+    obj_dir = out_root / "obj"
     obj_dir.mkdir(parents=True, exist_ok=True)
@@
-    run = subprocess.run([str(exe)])
+    run_args = [str(exe)]
+    if history_probe:
+        run_args.append("--history-probe")
+    run = subprocess.run(run_args, cwd=str(out_root) if history_probe else None)
     return run.returncode
```

Note: the existing `OUT` constant is still used for the default path only if you keep it. If you prefer minimal diff, replace `OUT` with `out_root` as shown, or set `OUT = out_root` before `obj_dir`.

---

## Expected commands

Default regression (unchanged):

```bash
python3 tests/test_composition_tl.py
```

History-probe branch:

```bash
python3 tests/test_composition_tl.py --history-probe
```

Artifacts:

- `out/b1-causal-20261006/tl-probe/history_events.jsonl`
- `out/b1-causal-20261006/tl-probe/summary.json`

---

## Acceptance checks

1. Default `python3 tests/test_composition_tl.py` still builds and runs the original cases with no regression.
2. `python3 tests/test_composition_tl.py --history-probe` builds `composition_tl_top` and runs only `case_history_probe()`.
3. `summary.json` contains `scope`, three runs, `prefix_match`, `first_victim_diverges_P_vs_PN`, `first_victim_equal_P_vs_PNlate`, `changed_prefill_tree_P_vs_PN`, `successor_hits`, `successor_fill_count`, `count`, and `result`.
4. `history_events.jsonl` contains, at minimum:
   - `a15_pre` with `fill_mask`, `fill_did`, `update_vpn`, `update_pscid`, `plru`, `evicted_vpn`;
   - `a15_post` with post-fill PLRU;
   - `effective_touch` events for the fast `V0` accept and successor hits.
5. Hard checks:
   - `P` does not accept `V` during the 60-cycle hold.
   - `PN` accepts fast, has exactly one effective IOTLB touch during hold, and no fill during hold.
   - `PNlate` has no `V` input until after the `A15` response, then normal `V0` hits with no fill.
   - `A15` fill mask is one-hot, inside the A allocation mask, committed in the post-edge entry, and owner `DID == 11`.
   - The victim pre-entry is an A page (`pscid == 11`); its logical VPN is recorded.
   - `P` and `PNlate` first victim match; `P` and `PN` first victim differ.
   - `PN` pre-fill PLRU differs from `P`; `PNlate` pre-fill PLRU equals `P`.
   - Successor: `P` hit/no fill, `PN` miss/one fill, `PNlate` hit/no fill; all physical translations match `0x80000000`, `0x9000f000`, and the successor `0x900...` address.

---

## Unresolved / root-review items

- Verilator 4.038 may reject the `i_dut.i_rv_iommu_iotlb.*` hierarchical references. If so, the root must decide whether to expose those via the frozen production wrapper or adjust the test-only XMR strategy.
- The adaptive successor uses `PN`’s actual evicted A page. If the root prefers fixed `A0`, replace `use_own_successor`/`forced_successor_iova` with a constant `A0` and report the actual `PN` eviction.
- The summary is written only on successful completion in this proposal. If the root requires a `FAIL` summary on abort, the `CHECK_AT` calls in `case_history_probe` need to be converted to a non-aborting accumulator before writing `summary.json`.
