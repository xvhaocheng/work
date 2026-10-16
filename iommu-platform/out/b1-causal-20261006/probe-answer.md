I implemented the frozen H1 specification strictly inside the three existing test files. Below is one machine‑applicable unified diff, then the acceptance commands, then assumptions/risks. **None of this was executed here** — every assertion below is a proposal for the parent to run.

### Design notes (so the diff is auditable)

* **Wrapper** (`composition_iotlb_top.sv`) gains `parameter bit ExplicitTouch = 0` and `input logic touch_i`. Only `i_actual` uses them (`.TouchEnable(ExplicitTouch)`, `.touch_i(touch_i)`). `i_base` and `i_reference` are untouched, so with the default `ExplicitTouch=0` the instance is bit‑identical to today (`TouchEnable=0` ⇒ `touch_i` is dead in the RTL expression `lu_hit[i] && (!TouchEnable || touch_i)`).
* **Python** gains `--history-probe` (and `--probe-out=`). Probe branch: independent `out = out/b1-causal-20261006/iotlb-probe`, `-GExplicitTouch=1` added to the unchanged `--cc --exe --build` invocation, and `--history-probe --probe-out=<dir>` passed to the same executable. Default path is byte‑for‑byte the old behaviour. The pre‑change reference is still extracted/elaborated because the wrapper always instantiates it; **its outputs are ignored** in the probe branch (`g_unified=false` disables every a‑vs‑b/a‑vs‑r comparison).
* **C++**: reuses the existing `Tag/Content/make_tag/make_content/drive_update/drive_lookup/read_q/read_n/check_entry_bits/is_onehot16/lowest_bit16/do_reset` helpers. Adds a test‑side rule model (`sh_victim` = identical tree walk to the RTL; `sh_touch` = flip a node only when the owning partition mask spans **both** subtrees), 16‑way synthetic priming through `policy_mask_i = 1<<k` + normal update (no force anywhere), the four frozen cases, JSONL events and a summary. The shadow is advanced **only up to the first victim divergence** (`g_shadow_lock`), then answers exactly one logical‑page query.
* Expected values asserted: post‑touch actual tree `0x008b` (nodes 0,1,3,7) vs shadow `0x0080` (node 7 only); fill victims **8 / 2**; touch‑0 victims **2 / 2**; same‑edge victims **2 / 2** with post‑edge trees `0x008b / 0x0080`; successors `y = miss/hit`, `z = hit/miss`. Pure bit‑level identity, no tolerance, no “weak success”.

```diff
diff --git a/tests/composition_iotlb_top.sv b/tests/composition_iotlb_top.sv
--- a/tests/composition_iotlb_top.sv
+++ b/tests/composition_iotlb_top.sv
@@ -17,7 +17,13 @@
 `ifndef COMPOSITION_IOTLB_TOP_SV
 `define COMPOSITION_IOTLB_TOP_SV
 
-module composition_iotlb_top (
+module composition_iotlb_top #(
+    // ExplicitTouch = 0 (default) keeps i_actual at TouchEnable = 0 with touch_i
+    // ignored, bit-exact against i_base and the pre-change reference.
+    // -GExplicitTouch=1 selects the H1 history probe, where i_actual runs
+    // TouchEnable = 1 and touch_i is a live, explicit input.
+    parameter bit ExplicitTouch = 0
+) (
     input  logic                          clk_i,
     input  logic                          rst_ni,
 
     input  logic                          policy_clear_i,
@@ -56,7 +62,8 @@
     input  logic                          up_c2s_r_i,
     input  logic                          up_c2s_v_i,
 
     input  logic                          lookup_i,
+    input  logic                          touch_i,
     input  logic [rv_iommu::GPPNW39-1:0]  lu_vpn_i,
     input  logic [19:0]                   lu_pscid_i,
     input  logic [15:0]                   lu_gscid_i,
@@ -137,8 +144,10 @@
 
-    // Composition under test.  TouchEnable = 0 keeps the pre-existing
-    // touch-on-hit behaviour of the reference so the full-mask comparison is
-    // bit-exact; touch_i is tied low.
+    // Composition under test.  With the default ExplicitTouch = 0 this is the
+    // pre-existing configuration: TouchEnable = 0, touch_i ignored and the
+    // functional outputs bit-exact against i_base / i_reference on every
+    // evaluation.  With -GExplicitTouch=1 the instance runs TouchEnable = 1 and
+    // touch_i becomes a live input used only by the H1 history probe.
     rv_iommu_iotlb #(
         .RVIOMMUCfg   (IOTLB_CFG),
-        .TouchEnable  (1'b0),
+        .TouchEnable  (ExplicitTouch),
         .PolicyEnable (1'b1)
@@ -150,7 +157,7 @@
         .policy_clear_i (policy_clear_i),
         .policy_mask_i  (policy_mask_i),
         .update_i       (update),
         .lookup_i       (lookup_i),
-        .touch_i        (1'b0),
+        .touch_i        (touch_i),
         .lu_vpn_i       (lu_vpn_i),
         .lu_pscid_i     (lu_pscid_i),
diff --git a/tests/composition_iotlb_test.cpp b/tests/composition_iotlb_test.cpp
--- a/tests/composition_iotlb_test.cpp
+++ b/tests/composition_iotlb_test.cpp
@@ -24,6 +24,12 @@
 //     not perturb the PLRU tree beyond a lookup hit.
 //
 // No Python model of the replacement policy is used: every temporal property is
 // observed on the real RTL through the passive research tap.
+//
+// --history-probe (frozen 2026-10-06; out/b1-causal-20261006/iotlb-probe) runs
+// only the four frozen H1 cases against the real RTL with TouchEnable = 1.  Its
+// counterfactual is a test-side partition-local rule model ("shadow"); the
+// i_base / i_reference outputs are ignored in that branch.  The default suite
+// below never runs when the flag is present.
 
 #include "Vcomposition_iotlb_top.h"
@@ -39,8 +45,10 @@
 static int         g_fail    = 0;
 static long        g_checks  = 0;
 static std::string g_phase   = "boot";
 static bool        g_unified = true;   // mask==0xffff and clear deasserted
+static bool        g_probe   = false;  // set by --history-probe (H1 probe only)
+static int         g_edge_no = 0;      // posedge counter for the probe event log
 
 static uint32_t g_rng = 0x9e3779b9u;
 static uint32_t rng() {
     g_rng ^= g_rng << 13;
@@ -109,7 +117,8 @@
 static void settle(const std::string& where) { top->clk_i = 0; eval_checked(where); }
 static void edge(const std::string& where) {
     top->clk_i = 1; eval_checked(where);
     top->clk_i = 0; eval_checked(where);
+    ++g_edge_no;   // probe event counter; unused by the default suite
 }
 
 // ---------------------------------------------------------------- stimulus
@@ -152,8 +161,9 @@
 
 static void clear_lookup() {
     top->lookup_i   = 0;
+    top->touch_i    = 0;   // explicit: no stale touch is ever left asserted
     top->lu_vpn_i   = 0;
     top->lu_pscid_i = 0;
     top->lu_gscid_i = 0;
     top->en_1S_i    = 0;
@@ -748,6 +758,779 @@
     if (!top->a_lu_hit_o) fail("tm: the surviving tag t2 no longer hits");
     edge("tm hit");
 }
 
 // --------------------------------------------------------------------- main
 
+
+// ============================================================ H1 history probe
+//
+// Enabled only with --history-probe (elaborated with -GExplicitTouch=1).  The
+// default suite above is untouched: nothing here runs without the flag.
+//
+// Object under test: the real rv_iommu_iotlb with TouchEnable = 1 (i_actual).
+// Counterfactual: a small test-side rule model ("shadow") that reuses the RTL
+// tree bit order and the victim walk but confines a touch to the partition
+// that owns the touched way: a node flips direction only when the ownership
+// mask has eligible leaves in BOTH of its subtrees.  The shadow is advanced
+// only up to the first victim divergence; afterwards it answers exactly one
+// logical-page query and is never fed further fills.
+
+static FILE* g_events      = 0;
+static FILE* g_summary     = 0;
+static bool  g_shadow_lock = false;   // set at the first victim divergence
+
+static const uint32_t PROBE_V_PSCID = 10;
+static const uint32_t PROBE_A_PSCID = 11;
+static const uint16_t PROBE_V_OWN   = 0x0003u;  // V partition owns ways 0-1
+static const uint16_t PROBE_A_ALLOC = 0xFFFCu;  // formal A allocation mask
+static const int      PROBE_X_ID    = 100;      // shadow page id of page x
+
+// -------------------------------------------------------- test-side shadow
+
+struct ShadowEntry {
+    bool     valid;
+    bool     en1s, en2s, c1_g;
+    uint32_t vpn, pscid, gscid;
+    int      page_id;
+};
+
+struct ShadowState {
+    uint32_t    tree;   // bit n = PLRU node n, same order as plru_tree_q
+    ShadowEntry e[16];
+};
+
+static ShadowState g_shadow;
+
+// eligible[15+i] = leaf i; eligible[n] = eligible[2n+1] | eligible[2n+2].
+static void sh_elig(uint16_t mask, bool e[31]) {
+    for (int i = 0; i < 16; i++) e[15 + i] = ((mask >> i) & 1u) != 0u;
+    for (int i = 14; i >= 0; i--) e[i] = e[2 * i + 1] || e[2 * i + 2];
+}
+
+static void sh_reset() {
+    g_shadow.tree = 0;
+    for (int i = 0; i < 16; i++) {
+        g_shadow.e[i].valid   = false;
+        g_shadow.e[i].en1s    = false;
+        g_shadow.e[i].en2s    = false;
+        g_shadow.e[i].c1_g    = false;
+        g_shadow.e[i].vpn     = 0;
+        g_shadow.e[i].pscid   = 0;
+        g_shadow.e[i].gscid   = 0;
+        g_shadow.e[i].page_id = -1;
+    }
+}
+
+// Identical walk to rv_iommu_iotlb: node n = (2^lvl - 1) + (i >> shift).
+static int sh_victim(uint16_t mask) {
+    bool e[31];
+    sh_elig(mask, e);
+    for (int i = 0; i < 16; i++) {
+        if (!((mask >> i) & 1u)) continue;
+        bool en = true;
+        for (int lvl = 0; lvl < 4; lvl++) {
+            int  idx_base = (1 << lvl) - 1;
+            int  shift    = 4 - lvl;
+            int  n        = idx_base + (i >> shift);
+            bool ni       = ((i >> (shift - 1)) & 1) != 0;
+            bool cr       = e[2 * n + 2] &&
+                            (!e[2 * n + 1] || (((g_shadow.tree >> n) & 1u) != 0u));
+            en = en && (ni ? cr : !cr);
+        }
+        if (en) return i;
+    }
+    return -1;
+}
+
+// Partition-local touch: flip a node only when the ownership mask spans both
+// of that node's subtrees; otherwise the previous direction is kept.
+static void sh_touch(int leaf, uint16_t own) {
+    bool o[31];
+    sh_elig(own, o);
+    for (int lvl = 0; lvl < 4; lvl++) {
+        int idx_base = (1 << lvl) - 1;
+        int shift    = 4 - lvl;
+        int n        = idx_base + (leaf >> shift);
+        if (!(o[2 * n + 1] && o[2 * n + 2])) continue;
+        int bit = (~((leaf >> (shift - 1)) & 1)) & 1;
+        g_shadow.tree = (g_shadow.tree & ~(1u << n)) | (static_cast<uint32_t>(bit) << n);
+    }
+}
+
+static void sh_fill(const Tag& t, const Content& c, uint16_t mask, int page_id) {
+    if (g_shadow_lock) fail("shadow: fill after the first victim divergence");
+    int v = sh_victim(mask);
+    if (v < 0) { fail("shadow: no victim for mask 0x" + hex16(mask)); return; }
+    g_shadow.e[v].valid   = true;
+    g_shadow.e[v].en1s    = (t.en1s != 0);
+    g_shadow.e[v].en2s    = (t.en2s != 0);
+    g_shadow.e[v].c1_g    = (c.c1_g != 0);
+    g_shadow.e[v].vpn     = t.vpn;
+    g_shadow.e[v].pscid   = t.pscid;
+    g_shadow.e[v].gscid   = t.gscid;
+    g_shadow.e[v].page_id = page_id;
+}
+
+// Every frozen page is a 4k, stage-1-only, non-global page, so the RTL
+// 1G/2M sub-matching collapses to a full-VPN compare here.
+static int sh_lookup(const Tag& t) {
+    for (int i = 0; i < 16; i++) {
+        const ShadowEntry& s = g_shadow.e[i];
+        if (!s.valid) continue;
+        if (s.en1s != (t.en1s != 0)) continue;
+        if (s.en2s != (t.en2s != 0)) continue;
+        if (!((!t.en1s) || (s.pscid == t.pscid) || s.c1_g)) continue;
+        if (!((!t.en2s) || (s.gscid == t.gscid))) continue;
+        if (s.vpn != t.vpn) continue;
+        return s.page_id;
+    }
+    return -1;
+}
+
+// ------------------------------------------------------- probe JSONL events
+
+static void json_str(FILE* f, const std::string& s) {
+    std::fputc('"', f);
+    for (std::size_t i = 0; i < s.size(); i++) {
+        const char ch = s[i];
+        if (ch == '"' || ch == '\\') std::fputc('\\', f);
+        std::fputc(ch, f);
+    }
+    std::fputc('"', f);
+}
+
+static void json_opt(FILE* f, long long v) {
+    if (v < 0) std::fputs("null", f);
+    else       std::fprintf(f, "%lld", v);
+}
+
+static void json_bool(FILE* f, bool b) { std::fputs(b ? "true" : "false", f); }
+
+struct ProbeEvent {
+    std::string cse;
+    int         edge;
+    std::string phase;
+    long long   lookup_vpn, lookup_pscid, lookup, touch, hit;
+    long long   fill_vpn, fill_pscid, alloc_mask;
+    long long   a_tree_before, a_tree_after;
+    long long   s_tree_before, s_tree_after;
+    long long   a_victim, s_victim;
+    bool        first_divergence;
+};
+
+static ProbeEvent mk_event(const std::string& cse, const std::string& phase) {
+    ProbeEvent e;
+    e.cse   = cse;
+    e.edge  = g_edge_no;
+    e.phase = phase;
+    e.lookup_vpn = e.lookup_pscid = e.lookup = e.touch = e.hit = -1;
+    e.fill_vpn = e.fill_pscid = e.alloc_mask = -1;
+    e.a_tree_before = e.a_tree_after = -1;
+    e.s_tree_before = e.s_tree_after = -1;
+    e.a_victim = e.s_victim = -1;
+    e.first_divergence = false;
+    return e;
+}
+
+static void emit(const ProbeEvent& e) {
+    if (!g_events) return;
+    std::fputs("{", g_events);
+    std::fputs("\"case\": ", g_events);              json_str(g_events, e.cse);
+    std::fprintf(g_events, ", \"edge\": %d", e.edge);
+    std::fputs(", \"phase\": ", g_events);           json_str(g_events, e.phase);
+    std::fputs(", \"lookupVPN\": ", g_events);       json_opt(g_events, e.lookup_vpn);
+    std::fputs(", \"lookupPSCID\": ", g_events);     json_opt(g_events, e.lookup_pscid);
+    std::fputs(", \"lookup\": ", g_events);          json_opt(g_events, e.lookup);
+    std::fputs(", \"touch\": ", g_events);           json_opt(g_events, e.touch);
+    std::fputs(", \"hit\": ", g_events);             json_opt(g_events, e.hit);
+    std::fputs(", \"fillVPN\": ", g_events);         json_opt(g_events, e.fill_vpn);
+    std::fputs(", \"fillPSCID\": ", g_events);       json_opt(g_events, e.fill_pscid);
+    std::fputs(", \"allocation_mask\": ", g_events); json_opt(g_events, e.alloc_mask);
+    std::fputs(", \"actual_tree_before\": ", g_events);
+    json_opt(g_events, e.a_tree_before);
+    std::fputs(", \"actual_tree_after\": ", g_events);
+    json_opt(g_events, e.a_tree_after);
+    std::fputs(", \"shadow_tree_before\": ", g_events);
+    json_opt(g_events, e.s_tree_before);
+    std::fputs(", \"shadow_tree_after\": ", g_events);
+    json_opt(g_events, e.s_tree_after);
+    std::fputs(", \"actual_victim\": ", g_events);
+    json_opt(g_events, e.a_victim);
+    std::fputs(", \"shadow_victim\": ", g_events);
+    json_opt(g_events, e.s_victim);
+    std::fputs(", \"first_divergence\": ", g_events);
+    json_bool(g_events, e.first_divergence);
+    std::fputs("}\n", g_events);
+}
+
+// -------------------- expected hit content (mirrors a_content_flat order)
+
+static void flat_set_bit(uint64_t& lo, uint64_t& hi, int b, int v) {
+    if (!(v & 1)) return;
+    if (b < 64) lo |= (1ULL << b);
+    else        hi |= (1ULL << (b - 64));
+}
+
+static void flat_set_field(uint64_t& lo, uint64_t& hi, int msb, int lsb, uint64_t v) {
+    for (int b = lsb; b <= msb; b++)
+        flat_set_bit(lo, hi, b, static_cast<int>((v >> (b - lsb)) & 1u));
+}
+
+// Same packing as the a_content_flat assign in composition_iotlb_top.sv:
+//   {c1.is_1G, c1.is_2M, c1.ppn, c1.g, c1.u, c1.x, c1.w, c1.r, c1.v,
+//    c2.is_1G, c2.is_2M, c2.ppn, c2.g, c2.u, c2.x, c2.w, c2.r, c2.v},
+// MSB = bit 103.
+static void expected_content_flat(const Content& c, uint64_t& lo, uint64_t& hi) {
+    lo = 0;
+    hi = 0;
+    flat_set_field(lo, hi, 103, 102,
+        (static_cast<uint64_t>(c.c1_1g & 1) << 1) | static_cast<uint64_t>(c.c1_2m & 1));
+    flat_set_field(lo, hi, 101, 58, c.c1_ppn);
+    flat_set_field(lo, hi, 57, 52,
+        (static_cast<uint64_t>(c.c1_g & 1) << 5) | (static_cast<uint64_t>(c.c1_u & 1) << 4) |
+        (static_cast<uint64_t>(c.c1_x & 1) << 3) | (static_cast<uint64_t>(c.c1_w & 1) << 2) |
+        (static_cast<uint64_t>(c.c1_r & 1) << 1) |  static_cast<uint64_t>(c.c1_v & 1));
+    flat_set_field(lo, hi, 51, 50,
+        (static_cast<uint64_t>(c.c2_1g & 1) << 1) | static_cast<uint64_t>(c.c2_2m & 1));
+    flat_set_field(lo, hi, 49, 6, c.c2_ppn);
+    flat_set_field(lo, hi, 5, 0,
+        (static_cast<uint64_t>(c.c2_g & 1) << 5) | (static_cast<uint64_t>(c.c2_u & 1) << 4) |
+        (static_cast<uint64_t>(c.c2_x & 1) << 3) | (static_cast<uint64_t>(c.c2_w & 1) << 2) |
+        (static_cast<uint64_t>(c.c2_r & 1) << 1) |  static_cast<uint64_t>(c.c2_v & 1));
+}
+
+static void check_hit_content(const std::string& cse, const Content& c,
+                              const std::string& label) {
+    uint64_t elo, ehi;
+    expected_content_flat(c, elo, ehi);
+    if (top->a_content_lo != elo || top->a_content_hi != ehi) {
+        fail(cse + ": " + label + " hit content mismatch: actual hi=" +
+             hex64(top->a_content_hi) + " lo=" + hex64(top->a_content_lo) +
+             " expected hi=" + hex64(ehi) + " lo=" + hex64(elo));
+    }
+}
+
+// ----------------------------------------------------------- probe stimulus
+
+static Tag     g_probe_page_tag[16];
+static Content g_probe_page_content[16];
+static Tag     g_probe_x_tag;
+static Content g_probe_x_content;
+
+struct ProbeCaseResult {
+    std::string cse;
+    int         a_victim;
+    int         s_victim;
+    int         succ_actual;   // 1 hit / 0 miss / -1 not queried
+    int         succ_shadow;
+    std::string succ_page;
+    bool        first_div;
+};
+
+static ProbeCaseResult g_probe_cases[4];
+static int             g_probe_ncases = 0;
+
+static int probe_actual_victim(const std::string& phase) {
+    const uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
+    if (!is_onehot16(m)) fail(phase + ": replace_mask not one-hot: 0x" + hex16(m));
+    const int v = lowest_bit16(m);
+    if (v < 0) fail(phase + ": no eligible way inside the mask");
+    return v;
+}
+
+// The only way any entry is primed: the normal update interface plus a
+// single-way mask.  No hierarchical force is used anywhere in this file.
+static void probe_build_prefix(const std::string& cse) {
+    g_phase       = cse + ":prefix";
+    g_unified     = false;
+    g_shadow_lock = false;
+
+    top->policy_clear_i = 0;
+    top->policy_mask_i  = 0xFFFF;
+    do_reset();
+    sh_reset();
+
+    for (int k = 0; k < 16; k++) {
+        g_probe_page_tag[k] = make_tag(mk_vpn(0x100u + static_cast<uint32_t>(k), 0, 0),
+                                       (k < 2) ? PROBE_V_PSCID : PROBE_A_PSCID,
+                                       0x300u + static_cast<uint32_t>(k), 1, 0);
+        g_probe_page_content[k] = make_content(0x1000u + static_cast<uint32_t>(k), 0, 0, 0,
+                                               0x2000u + static_cast<uint32_t>(k), 0, 0, 0);
+    }
+
+    for (int k = 0; k < 16; k++) {
+        const uint16_t mask = static_cast<uint16_t>(1u << k);
+        top->policy_mask_i = mask;
+        clear_inval();
+        drive_update(g_probe_page_tag[k], g_probe_page_content[k]);
+        settle("prefix fill settle");
+
+        const int av = probe_actual_victim(cse + ": priming slot " + dec(k));
+        const int sv = sh_victim(mask);
+        if (av != k) fail(cse + ": priming selected way " + dec(av) + " instead of way " + dec(k));
+        if (sv != k) fail(cse + ": shadow priming selected way " + dec(sv));
+        {
+            uint64_t nlo, nhi;
+            read_n(k, nlo, nhi);
+            check_entry_bits(nlo, nhi, g_probe_page_tag[k], g_probe_page_content[k],
+                             cse + ": prefix entries_next[" + dec(k) + "]");
+        }
+
+        ProbeEvent ev = mk_event(cse, "prime-slot-" + dec(k));
+        ev.fill_vpn     = static_cast<long long>(g_probe_page_tag[k].vpn);
+        ev.fill_pscid   = static_cast<long long>(g_probe_page_tag[k].pscid);
+        ev.alloc_mask   = static_cast<long long>(mask);
+        ev.a_tree_before = top->a_plru;
+        ev.s_tree_before = g_shadow.tree;
+        ev.a_victim      = av;
+        ev.s_victim      = sv;
+        edge("prefix fill edge");
+        sh_fill(g_probe_page_tag[k], g_probe_page_content[k], mask, k);
+        {
+            uint64_t qlo, qhi;
+            read_q(k, qlo, qhi);
+            check_entry_bits(qlo, qhi, g_probe_page_tag[k], g_probe_page_content[k],
+                             cse + ": prefix committed way " + dec(k));
+        }
+        ev.a_tree_after = top->a_plru;
+        ev.s_tree_after = g_shadow.tree;
+        emit(ev);
+    }
+
+    // Per-way identity and per-page hit content, with touch_i held at 0.
+    for (int k = 0; k < 16; k++) {
+        drive_lookup(g_probe_page_tag[k]);
+        settle("prefix verify settle");
+        if (!top->a_lu_hit_o) fail(cse + ": primed page " + dec(k) + " does not hit");
+        check_hit_content(cse, g_probe_page_content[k], "primed page " + dec(k));
+        const int sp = sh_lookup(g_probe_page_tag[k]);
+        if (sp != k) fail(cse + ": shadow resolves primed page " + dec(k) + " to " + dec(sp));
+        {
+            uint64_t qlo, qhi;
+            read_q(k, qlo, qhi);
+            check_entry_bits(qlo, qhi, g_probe_page_tag[k], g_probe_page_content[k],
+                             cse + ": prefix resident way " + dec(k));
+        }
+
+        ProbeEvent ev = mk_event(cse, "prime-verify-" + dec(k));
+        ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[k].vpn);
+        ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[k].pscid);
+        ev.lookup       = 1;
+        ev.touch        = 0;
+        ev.hit          = top->a_lu_hit_o ? 1 : 0;
+        ev.a_tree_before = top->a_plru;
+        ev.s_tree_before = g_shadow.tree;
+        edge("prefix verify edge");
+        ev.a_tree_after = top->a_plru;
+        ev.s_tree_after = g_shadow.tree;
+        emit(ev);
+    }
+
+    if (top->a_plru != 0)
+        fail(cse + ": prefix left a non-zero actual tree: 0x" + hex16(top->a_plru));
+    if (g_shadow.tree != 0) fail(cse + ": prefix left a non-zero shadow tree");
+
+    ProbeEvent ready = mk_event(cse, "prefix-ready");
+    ready.a_tree_before = ready.a_tree_after = top->a_plru;
+    ready.s_tree_before = ready.s_tree_after = g_shadow.tree;
+    emit(ready);
+}
+
+// Shared prefix of H1-positive / H1-reverse-successor: x miss, V0 valid touch,
+// one idle edge, then the A fill of page x under mask 0xfffc.
+static void probe_to_first_divergence(const std::string& cse, ProbeCaseResult& r) {
+    r.cse = cse;
+    probe_build_prefix(cse);
+
+    g_phase = cse + ":A-x-miss";
+    drive_lookup(g_probe_x_tag);
+    settle("x miss settle");
+    if (top->a_lu_hit_o) fail(cse + ": new A page x unexpectedly hits");
+    {
+        ProbeEvent ev = mk_event(cse, "probe-x-lookup");
+        ev.lookup_vpn   = static_cast<long long>(g_probe_x_tag.vpn);
+        ev.lookup_pscid = static_cast<long long>(g_probe_x_tag.pscid);
+        ev.lookup       = 1;
+        ev.touch        = 0;
+        ev.hit          = 0;
+        ev.a_tree_before = top->a_plru;
+        ev.s_tree_before = g_shadow.tree;
+        edge("x miss edge");
+        ev.a_tree_after = top->a_plru;
+        ev.s_tree_after = g_shadow.tree;
+        emit(ev);
+    }
+    if (top->a_plru != 0) fail(cse + ": an x lookup miss moved the actual tree");
+    if (g_shadow.tree != 0) fail(cse + ": an x lookup miss moved the shadow tree");
+
+    g_phase = cse + ":V0-touch";
+    drive_lookup(g_probe_page_tag[0]);
+    top->touch_i = 1;
+    settle("V0 touch settle");
+    if (!top->a_lu_hit_o) fail(cse + ": V way-0 page does not hit");
+    check_hit_content(cse, g_probe_page_content[0], "V way-0");
+    {
+        ProbeEvent ev = mk_event(cse, "probe-V0-touch");
+        ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[0].vpn);
+        ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[0].pscid);
+        ev.lookup       = 1;
+        ev.touch        = 1;
+        ev.hit          = 1;
+        ev.a_tree_before = top->a_plru;
+        ev.s_tree_before = g_shadow.tree;
+        edge("V0 touch edge");
+        ev.a_tree_after = top->a_plru;
+        sh_touch(0, PROBE_V_OWN);
+        ev.s_tree_after = g_shadow.tree;
+        emit(ev);
+    }
+    // Real RTL: nodes 0, 1, 3 and 7 (root, 0-7, 0-3, 0-1) turn right.
+    if (top->a_plru != 0x008Bu)
+        fail(cse + ": post-touch actual tree is 0x" + hex16(top->a_plru) + ", expected 0x008b");
+    // Shadow: only the 0-1 node flips (0x0003 owns both of its subtrees).
+    if (g_shadow.tree != 0x0080u)
+        fail(cse + ": post-touch shadow tree is 0x" + hex16(g_shadow.tree) + ", expected 0x0080");
+
+    g_phase = cse + ":idle";
+    clear_lookup();
+    settle("idle settle");
+    edge("idle edge");   // at least one full edge between the touch and the fill
+
+    g_phase = cse + ":A-fill-x";
+    top->policy_mask_i = PROBE_A_ALLOC;
+    clear_inval();
+    drive_update(g_probe_x_tag, g_probe_x_content);
+    settle("fill x settle");
+    const int av = probe_actual_victim(cse + ": fill x");
+    const int sv = sh_victim(PROBE_A_ALLOC);
+    ProbeEvent ev = mk_event(cse, "probe-fill-x");
+    ev.fill_vpn      = static_cast<long long>(g_probe_x_tag.vpn);
+    ev.fill_pscid    = static_cast<long long>(g_probe_x_tag.pscid);
+    ev.alloc_mask    = static_cast<long long>(PROBE_A_ALLOC);
+    ev.a_tree_before = top->a_plru;
+    ev.s_tree_before = g_shadow.tree;
+    ev.a_victim      = av;
+    ev.s_victim      = sv;
+    if (av != 8) fail(cse + ": actual victim is " + dec(av) + ", expected 8");
+    if (sv != 2) fail(cse + ": shadow victim is " + dec(sv) + ", expected 2");
+    if (av == sv) fail(cse + ": expected a first victim divergence at this fill");
+    ev.first_divergence = (av != sv);
+    {
+        uint64_t nlo, nhi;
+        read_n(av, nlo, nhi);
+        check_entry_bits(nlo, nhi, g_probe_x_tag, g_probe_x_content, cse + ": x entries_next");
+    }
+    edge("fill x edge");
+    {
+        uint64_t qlo, qhi;
+        read_q(av, qlo, qhi);
+        check_entry_bits(qlo, qhi, g_probe_x_tag, g_probe_x_content, cse + ": x committed");
+    }
+    sh_fill(g_probe_x_tag, g_probe_x_content, PROBE_A_ALLOC, PROBE_X_ID);
+    ev.a_tree_after = top->a_plru;
+    ev.s_tree_after = g_shadow.tree;
+    emit(ev);
+
+    g_shadow_lock = true;   // the shadow stops tracking at the first divergence
+    r.a_victim  = av;
+    r.s_victim  = sv;
+    r.first_div = true;
+}
+
+static void probe_case_positive() {
+    const std::string cse = "H1-positive";
+    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
+    probe_to_first_divergence(cse, r);
+
+    g_phase = cse + ":successor-y";
+    drive_lookup(g_probe_page_tag[8]);
+    settle("y lookup settle");
+    const int ah = top->a_lu_hit_o ? 1 : 0;
+    const int sp = sh_lookup(g_probe_page_tag[8]);
+    const int sh = (sp == 8) ? 1 : 0;
+    if (ah != 0) fail(cse + ": successor page y still hits in the actual RTL");
+    if (sh != 1) fail(cse + ": the shadow no longer resolves page y (page_id " + dec(sp) + ")");
+    ProbeEvent ev = mk_event(cse, "successor-y");
+    ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[8].vpn);
+    ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[8].pscid);
+    ev.lookup       = 1;
+    ev.touch        = 0;
+    ev.hit          = ah;
+    ev.a_tree_before = top->a_plru;
+    ev.s_tree_before = g_shadow.tree;
+    edge("y lookup edge");
+    ev.a_tree_after = top->a_plru;
+    ev.s_tree_after = g_shadow.tree;
+    emit(ev);
+
+    r.succ_actual = ah;
+    r.succ_shadow = sh;
+    r.succ_page   = "way8-page-y";
+}
+
+static void probe_case_no_touch() {
+    const std::string cse = "H1-no-touch";
+    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
+    r.cse = cse;
+    probe_build_prefix(cse);
+
+    g_phase = cse + ":A-x-miss";
+    drive_lookup(g_probe_x_tag);
+    settle("x miss settle");
+    if (top->a_lu_hit_o) fail(cse + ": new A page x unexpectedly hits");
+    edge("x miss edge");
+
+    g_phase = cse + ":V0-no-touch";
+    drive_lookup(g_probe_page_tag[0]);   // clear_lookup() leaves touch_i at 0
+    settle("V0 no-touch settle");
+    if (!top->a_lu_hit_o) fail(cse + ": V way-0 page does not hit");
+    check_hit_content(cse, g_probe_page_content[0], "V way-0");
+    {
+        ProbeEvent ev = mk_event(cse, "probe-V0-no-touch");
+        ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[0].vpn);
+        ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[0].pscid);
+        ev.lookup       = 1;
+        ev.touch        = 0;
+        ev.hit          = 1;
+        ev.a_tree_before = top->a_plru;
+        ev.s_tree_before = g_shadow.tree;
+        edge("V0 no-touch edge");
+        ev.a_tree_after = top->a_plru;
+        ev.s_tree_after = g_shadow.tree;
+        emit(ev);
+    }
+    if (top->a_plru != 0) fail(cse + ": touch_i=0 did not gate the actual touch");
+    if (g_shadow.tree != 0) fail(cse + ": touch_i=0 moved the shadow tree");
+
+    g_phase = cse + ":idle";
+    clear_lookup();
+    settle("idle settle");
+    edge("idle edge");
+
+    g_phase = cse + ":A-fill-x";
+    top->policy_mask_i = PROBE_A_ALLOC;
+    clear_inval();
+    drive_update(g_probe_x_tag, g_probe_x_content);
+    settle("fill x settle");
+    const int av = probe_actual_victim(cse + ": fill x");
+    const int sv = sh_victim(PROBE_A_ALLOC);
+    if (av != 2) fail(cse + ": actual victim is " + dec(av) + ", expected 2");
+    if (sv != 2) fail(cse + ": shadow victim is " + dec(sv) + ", expected 2");
+    ProbeEvent ev = mk_event(cse, "probe-fill-x");
+    ev.fill_vpn      = static_cast<long long>(g_probe_x_tag.vpn);
+    ev.fill_pscid    = static_cast<long long>(g_probe_x_tag.pscid);
+    ev.alloc_mask    = static_cast<long long>(PROBE_A_ALLOC);
+    ev.a_tree_before = top->a_plru;
+    ev.s_tree_before = g_shadow.tree;
+    ev.a_victim      = av;
+    ev.s_victim      = sv;
+    ev.first_divergence = false;
+    {
+        uint64_t nlo, nhi;
+        read_n(av, nlo, nhi);
+        check_entry_bits(nlo, nhi, g_probe_x_tag, g_probe_x_content, cse + ": x entries_next");
+    }
+    edge("fill x edge");
+    {
+        uint64_t qlo, qhi;
+        read_q(av, qlo, qhi);
+        check_entry_bits(qlo, qhi, g_probe_x_tag, g_probe_x_content, cse + ": x committed");
+    }
+    sh_fill(g_probe_x_tag, g_probe_x_content, PROBE_A_ALLOC, PROBE_X_ID);
+    ev.a_tree_after = top->a_plru;
+    ev.s_tree_after = g_shadow.tree;
+    emit(ev);
+
+    r.a_victim  = av;
+    r.s_victim  = sv;
+    r.first_div = false;
+}
+
+static void probe_case_same_edge() {
+    const std::string cse = "H1-same-edge";
+    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
+    r.cse = cse;
+    probe_build_prefix(cse);
+
+    g_phase = cse + ":same-edge";
+    // V way-0 hit with an explicit valid touch and the A fill of x, one edge.
+    clear_inval();
+    top->lookup_i        = 1;
+    top->lu_vpn_i        = g_probe_page_tag[0].vpn;
+    top->lu_pscid_i      = g_probe_page_tag[0].pscid;
+    top->lu_gscid_i      = g_probe_page_tag[0].gscid;
+    top->en_1S_i         = 1;   // shared by the lookup and the update latch
+    top->en_2S_i         = 0;   // both pages are stage-1-only
+    top->touch_i         = 1;
+    top->policy_mask_i   = PROBE_A_ALLOC;
+    top->update_update_i = 1;
+    top->update_vpn_i    = g_probe_x_tag.vpn;
+    top->update_pscid_i  = g_probe_x_tag.pscid;
+    top->update_gscid_i  = g_probe_x_tag.gscid;
+    top->up_c1s_is_1G_i  = g_probe_x_content.c1_1g;
+    top->up_c1s_is_2M_i  = g_probe_x_content.c1_2m;
+    top->up_c1s_ppn_i    = g_probe_x_content.c1_ppn;
+    top->up_c1s_g_i      = g_probe_x_content.c1_g;
+    top->up_c1s_u_i      = g_probe_x_content.c1_u;
+    top->up_c1s_x_i      = g_probe_x_content.c1_x;
+    top->up_c1s_w_i      = g_probe_x_content.c1_w;
+    top->up_c1s_r_i      = g_probe_x_content.c1_r;
+    top->up_c1s_v_i      = g_probe_x_content.c1_v;
+    top->up_c2s_is_1G_i  = g_probe_x_content.c2_1g;
+    top->up_c2s_is_2M_i  = g_probe_x_content.c2_2m;
+    top->up_c2s_ppn_i    = g_probe_x_content.c2_ppn;
+    top->up_c2s_g_i      = g_probe_x_content.c2_g;
+    top->up_c2s_u_i      = g_probe_x_content.c2_u;
+    top->up_c2s_x_i      = g_probe_x_content.c2_x;
+    top->up_c2s_w_i      = g_probe_x_content.c2_w;
+    top->up_c2s_r_i      = g_probe_x_content.c2_r;
+    top->up_c2s_v_i      = g_probe_x_content.c2_v;
+    settle("same-edge settle");
+    if (!top->a_lu_hit_o) fail(cse + ": V way-0 page does not hit");
+    check_hit_content(cse, g_probe_page_content[0], "V way-0");
+    const int av = probe_actual_victim(cse + ": same-edge fill");
+    const int sv = sh_victim(PROBE_A_ALLOC);
+    if (av != 2) fail(cse + ": same-edge actual victim is " + dec(av) + ", expected 2 (old tree)");
+    if (sv != 2) fail(cse + ": same-edge shadow victim is " + dec(sv) + ", expected 2 (old tree)");
+    ProbeEvent ev = mk_event(cse, "probe-same-edge");
+    ev.lookup_vpn    = static_cast<long long>(g_probe_page_tag[0].vpn);
+    ev.lookup_pscid  = static_cast<long long>(g_probe_page_tag[0].pscid);
+    ev.lookup        = 1;
+    ev.touch         = 1;
+    ev.hit           = 1;
+    ev.fill_vpn      = static_cast<long long>(g_probe_x_tag.vpn);
+    ev.fill_pscid    = static_cast<long long>(g_probe_x_tag.pscid);
+    ev.alloc_mask    = static_cast<long long>(PROBE_A_ALLOC);
+    ev.a_tree_before = top->a_plru;
+    ev.s_tree_before = g_shadow.tree;
+    ev.a_victim      = av;
+    ev.s_victim      = sv;
+    ev.first_divergence = false;
+    {
+        uint64_t nlo, nhi;
+        read_n(av, nlo, nhi);
+        check_entry_bits(nlo, nhi, g_probe_x_tag, g_probe_x_content, cse + ": x entries_next");
+    }
+    edge("same-edge edge");
+    {
+        uint64_t qlo, qhi;
+        read_q(av, qlo, qhi);
+        check_entry_bits(qlo, qhi, g_probe_x_tag, g_probe_x_content, cse + ": x committed");
+    }
+    sh_fill(g_probe_x_tag, g_probe_x_content, PROBE_A_ALLOC, PROBE_X_ID);
+    sh_touch(0, PROBE_V_OWN);
+    ev.a_tree_after = top->a_plru;
+    ev.s_tree_after = g_shadow.tree;
+    emit(ev);
+    // This fill used the old tree; the post-edge trees are recorded only.
+    if (top->a_plru != 0x008Bu)
+        fail(cse + ": post-edge actual tree is 0x" + hex16(top->a_plru) + ", expected 0x008b");
+    if (g_shadow.tree != 0x0080u)
+        fail(cse + ": post-edge shadow tree is 0x" + hex16(g_shadow.tree) + ", expected 0x0080");
+
+    r.a_victim  = av;
+    r.s_victim  = sv;
+    r.first_div = false;
+}
+
+static void probe_case_reverse_successor() {
+    const std::string cse = "H1-reverse-successor";
+    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
+    probe_to_first_divergence(cse, r);
+
+    g_phase = cse + ":successor-z";
+    drive_lookup(g_probe_page_tag[2]);
+    settle("z lookup settle");
+    const int ah = top->a_lu_hit_o ? 1 : 0;
+    const int sp = sh_lookup(g_probe_page_tag[2]);
+    const int sh = (sp == 2) ? 1 : 0;
+    if (ah != 1) fail(cse + ": successor page z does not hit in the actual RTL");
+    check_hit_content(cse, g_probe_page_content[2], "successor page z");
+    if (sh != 0) fail(cse + ": the shadow still resolves page z (page_id " + dec(sp) + ")");
+    ProbeEvent ev = mk_event(cse, "successor-z");
+    ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[2].vpn);
+    ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[2].pscid);
+    ev.lookup       = 1;
+    ev.touch        = 0;
+    ev.hit          = ah;
+    ev.a_tree_before = top->a_plru;
+    ev.s_tree_before = g_shadow.tree;
+    edge("z lookup edge");
+    ev.a_tree_after = top->a_plru;
+    ev.s_tree_after = g_shadow.tree;
+    emit(ev);
+
+    r.succ_actual = ah;
+    r.succ_shadow = sh;
+    r.succ_page   = "way2-page-z";
+}
+
+// ------------------------------------------------------------ probe summary
+
+static void probe_write_summary() {
+    if (!g_summary) return;
+    std::fputs("{\n", g_summary);
+    std::fputs("  \"suite\": \"b1-causal-20261006/iotlb-probe\",\n", g_summary);
+    std::fputs("  \"actual_RTL_checked\": true,\n", g_summary);
+    std::fputs("  \"counterfactual_model_only\": true,\n", g_summary);
+    std::fputs("  \"synthetic_priming\": true,\n", g_summary);
+    std::fputs("  \"platform_reachability_not_checked\": true,\n", g_summary);
+    std::fputs("  \"board_attribution_not_checked\": true,\n", g_summary);
+    std::fputs("  \"model_stops_at_first_divergence\": true,\n", g_summary);
+    std::fputs("  \"cases\": [\n", g_summary);
+    for (int i = 0; i < g_probe_ncases; i++) {
+        const ProbeCaseResult& r = g_probe_cases[i];
+        std::fputs("    {\"case\": ", g_summary);
+        json_str(g_summary, r.cse);
+        std::fprintf(g_summary,
+                     ", \"actual_victim\": %d, \"shadow_victim\": %d, \"first_divergence\": %s",
+                     r.a_victim, r.s_victim, r.first_div ? "true" : "false");
+        std::fputs(", \"successor_page\": ", g_summary);
+        json_str(g_summary, r.succ_page);
+        std::fputs(", \"successor_queried\": ", g_summary);
+        json_bool(g_summary, r.succ_actual >= 0);
+        std::fputs(", \"successor_actual_hit\": ", g_summary);
+        json_bool(g_summary, r.succ_actual == 1);
+        std::fputs(", \"successor_shadow_hit\": ", g_summary);
+        json_bool(g_summary, r.succ_shadow == 1);
+        std::fputs("}", g_summary);
+        if (i + 1 < g_probe_ncases) std::fputc(',', g_summary);
+        std::fputc('\n', g_summary);
+    }
+    std::fputs("  ],\n", g_summary);
+    std::fprintf(g_summary, "  \"checked_evaluations\": %ld,\n", g_checks);
+    std::fprintf(g_summary, "  \"failures\": %d,\n", g_fail);
+    std::fprintf(g_summary, "  \"result\": \"%s\"\n", (g_fail == 0) ? "PASS" : "FAIL");
+    std::fputs("}\n", g_summary);
+}
+
+static int probe_main(const std::string& out_dir) {
+    const std::string dir = out_dir.empty()
+                                ? std::string("out/b1-causal-20261006/iotlb-probe")
+                                : out_dir;
+    const std::string ev_path  = dir + "/events.jsonl";
+    const std::string sum_path = dir + "/summary.json";
+    g_events  = std::fopen(ev_path.c_str(), "w");
+    g_summary = std::fopen(sum_path.c_str(), "w");
+    if (g_events == 0 || g_summary == 0) {
+        std::printf("FAIL [history-probe]: cannot create %s or %s\n",
+                    ev_path.c_str(), sum_path.c_str());
+        return 1;
+    }
+
+    g_probe_x_tag     = make_tag(mk_vpn(0x400u, 0, 0), PROBE_A_PSCID, 0x500u, 1, 0);
+    g_probe_x_content = make_content(0x3000u, 0, 0, 0, 0x4000u, 0, 0, 0);
+
+    probe_case_positive();
+    probe_case_no_touch();
+    probe_case_same_edge();
+    probe_case_reverse_successor();
+
+    probe_write_summary();
+    std::fclose(g_events);
+    g_events = 0;
+    std::fclose(g_summary);
+    g_summary = 0;
+
+    if (g_fail == 0) {
+        std::printf("PASS [history-probe]: %ld checked evaluations, 0 failures\n", g_checks);
+        return 0;
+    }
+    std::printf("FAIL [history-probe]: %d failures across %ld checked evaluations\n",
+                g_fail, g_checks);
+    return 1;
+}
@@ -752,26 +1535,36 @@
 // --------------------------------------------------------------------- main
 
 int main(int argc, char** argv) {
+    std::string probe_out;
+    for (int i = 1; i < argc; i++) {
+        const std::string arg = (argv[i] != 0) ? argv[i] : "";
+        if (arg == "--history-probe")                g_probe = true;
+        else if (arg.rfind("--probe-out=", 0) == 0)  probe_out = arg.substr(12);
+    }
+
     Verilated::commandArgs(argc, argv);
     top = new Vcomposition_iotlb_top;
 
     top->clk_i          = 0;
     top->rst_ni         = 0;
     top->tap_idx        = 0;
     top->policy_clear_i = 0;
     top->policy_mask_i  = 0xFFFF;
+    top->touch_i        = 0;   // the probe must never see a stale touch
     clear_inval();
     clear_update();
     clear_lookup();
 
     g_phase   = "reset";
-    g_unified = true;
+    g_unified = !g_probe;      // the probe never compares against base/reference
     for (int i = 0; i < 4; i++) {
         top->clk_i = 1; eval_checked("reset");
         top->clk_i = 0; eval_checked("reset");
     }
     top->rst_ni = 1;
     settle("post-reset");
 
+    if (g_probe) return probe_main(probe_out);
+
     if (!top->a_supported) fail("actual instance does not report NumIotlbEntries == 16");
     if (!top->b_supported) fail("base instance does not report NumIotlbEntries == 16");
     check_all_invalid("reset");
diff --git a/tests/test_composition_iotlb.py b/tests/test_composition_iotlb.py
--- a/tests/test_composition_iotlb.py
+++ b/tests/test_composition_iotlb.py
@@ -26,11 +26,16 @@
+import argparse
 import re
 import subprocess
 import sys
 from pathlib import Path
 
 ROOT = Path(__file__).resolve().parents[1]
 IOMMU = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
 OUT = ROOT / "out" / "composition-iotlb"
+
+# Frozen H1 history probe (2026-10-06): independent output tree.  The default
+# tree above is never written by the probe branch and vice versa.
+HISTORY_PROBE_OUT = ROOT / "out" / "b1-causal-20261006" / "iotlb-probe"
 
 REF_SHA = "137f5e5fb1f2cc65a1346359226b16247c3f8445"
 REF_REL = "rtl/translation_logic/rv_iommu_iotlb.sv"
@@ -50,6 +55,11 @@
-def extract_reference() -> Path:
-    """git-show the prior revision and rename only the module declaration."""
-    rtl_dir = OUT / "rtl"
+def extract_reference(out: Path) -> Path:
+    """git-show the prior revision and rename only the module declaration.
+
+    The extracted module is required for elaboration in both branches because
+    composition_iotlb_top always instantiates rv_iommu_iotlb_reference; the
+    history-probe branch simply ignores its outputs.
+    """
+    rtl_dir = out / "rtl"
     rtl_dir.mkdir(parents=True, exist_ok=True)
     ref_file = rtl_dir / "rv_iommu_iotlb_reference.sv"
 
@@ -83,9 +93,20 @@
             out.append(p)
     return out
 
-def main() -> int:
-    ref_file = extract_reference()
-    cfg_pkg = IOMMU / "packages" / "rv_iommu_cfg_pkg.sv"
+def main(argv=None) -> int:
+    ap = argparse.ArgumentParser(description="composition IOTLB allocation test")
+    ap.add_argument("--history-probe", action="store_true",
+                    help="run the frozen H1 history probe instead of the default suite")
+    ap.add_argument("--probe-out", default=None,
+                    help="probe output dir (default: out/b1-causal-20261006/iotlb-probe)")
+    cli = ap.parse_args(argv)
+
+    out = Path(cli.probe_out) if cli.probe_out else (HISTORY_PROBE_OUT if cli.history_probe else OUT)
+    verilator_extra = ["-GExplicitTouch=1"] if cli.history_probe else []
+    run_extra = ["--history-probe", "--probe-out={}".format(out)] if cli.history_probe else []
+
+    ref_file = extract_reference(out)
+    cfg_pkg = IOMMU / "packages" / "rv_iommu_cfg_pkg.sv"
 
     sources = dedup([
         IOMMU / "packages" / "rv_iommu_reg_pkg.sv",   # must precede rv_iommu_pkg
@@ -101,19 +122,20 @@
         if not path.exists():
             sys.exit("required file missing: {}".format(path))
 
-    obj_dir = OUT / "obj"
+    obj_dir = out / "obj"
     obj_dir.mkdir(parents=True, exist_ok=True)
 
     args = ["verilator", "--cc", "--exe", "--build",
             "-Wall", "-Wno-fatal",
             "--top-module", TOP_MODULE,
             "--Mdir", str(obj_dir)]
+    args += verilator_extra
     args += [str(p) for p in sources]
     args += [str(tb)]
 
-    build_log = OUT / "build.log"
+    build_log = out / "build.log"
     with build_log.open("w") as log:
         build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
     if build.returncode != 0:
         # The build log is only inspected on failure; never dumped otherwise.
         sys.stderr.write(tail(build_log))
@@ -124,7 +146,7 @@
         sys.stderr.write(tail(build_log))
         sys.exit("verilator did not produce {}".format(exe))
 
-    run = subprocess.run([str(exe)])
+    run = subprocess.run([str(exe)] + run_extra)
     return run.returncode
 
 
```

### Acceptance commands

```bash
# 1) default suite must be byte-for-byte unchanged (long fuzz + mask + inval + touch phases)
python3 tests/test_composition_iotlb.py ; echo "default rc=$?"

# 2) frozen H1 history probe (Verilator 4.038, --cc --exe --build, no --binary)
python3 tests/test_composition_iotlb.py --history-probe ; echo "probe rc=$?"
# PASS requires rc==0 and "PASS [history-probe]" on stdout.

# 3) artefacts produced by step 2
jq .  out/b1-causal-20261006/iotlb-probe/summary.json
jq -c 'select(.first_divergence==true)' out/b1-causal-20261006/iotlb-probe/events.jsonl
jq -c 'select(.case=="H1-positive" and .phase=="successor-y")' out/b1-causal-20261006/iotlb-probe/events.jsonl
jq -c 'select(.case=="H1-same-edge" and .phase=="probe-same-edge")' out/b1-causal-20261006/iotlb-probe/events.jsonl

# 4) default tree must not have been touched by step 2
ls -la out/composition-iotlb/   # mtimes unchanged since step 1

# 5) optional explicit probe output directory
python3 tests/test_composition_iotlb.py --history-probe --probe-out=out/b1-causal-20261006/iotlb-probe
```

### Expected probe outcome (to be confirmed by the run, not by this proposal)

| case | actual victim | shadow victim | first divergence | successor query |
|---|---|---|---|---|
| `H1-positive` | 8 | 2 | yes (fill‑x edge) | page y = **miss / hit** |
| `H1-no-touch` | 2 | 2 | no | not queried |
| `H1-same-edge` | 2 (old tree) | 2 (old tree) | no (post‑edge trees `0x008b` / `0x0080`) | not queried |
| `H1-reverse-successor` | 8 | 2 | yes (fill‑x edge) | page z = **hit / miss** |

### Assumptions / unresolved risks (parent should verify)

1. **Big hunk line count.** The C++ probe insertion is one contiguous block; its hunk header is `@@ -748,6 +758,779 @@` (773 added lines, 6 context lines). If any line is dropped when pasting, `git apply` will fail loudly — prefer `git apply --check` first, and fall back to `patch -p1` if needed.
2. **`-GExplicitTouch=1`** relies on Verilator 4.038 top‑level parameter override for a `bit` parameter; that is the mechanism the R5 spec assumed. If an older tool rejects `-G`, the fallback is to add a second wrapper compile, but that was not requested.
3. **Reference module still compiled.** The wrapper unconditionally instantiates `rv_iommu_iotlb_reference`, so the probe branch still performs the `git show` of `137f5e5…`. Per C1 R5 this is only used for elaboration; all its outputs are ignored because `g_unified=false` in probe mode. If you want the probe to be *fully* independent of git history, a follow‑up would need a top‑level generate‑if around the reference instance — deliberately **not** done here (interface change beyond the approved batch).
4. **Content packing assumption.** `expected_content_flat()` mirrors the field order written in `composition_iotlb_top.sv`'s `a_content_flat` assign (`c1.is_1G, is_2M, ppn, g,u,x,w,r,v` then `c2.*`). If `iotlb_content_t`'s packed declaration differs, the hit‑content assertions will misfire — check `rv_iommu_pkg.sv` once before trusting a content failure.
5. **`successor_queried=false`** is reported for `H1-no-touch` / `H1-same-edge`; `successor_actual_hit=false` in those rows is meaningless and must be read together with `successor_queried`.
6. **Scope limits baked into `summary.json`** (deliberately true): `counterfactual_model_only`, `synthetic_priming`, `platform_reachability_not_checked`, `board_attribution_not_checked`. Passing these four cases licenses only “a local component‑level dependency exists under this input”; it does **not** establish that a real fast‑accepted translation produces this touch, nor any whole‑run/PN performance claim.
7. **Not run.** No build, no simulation and no assertion above has been executed in this session; there is no test evidence. Architecture‑level decisions (e.g. whether to drop the reference instance entirely, or whether this probe is the right next artefact) remain with Codex.
