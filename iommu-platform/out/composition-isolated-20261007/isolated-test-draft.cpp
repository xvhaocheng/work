// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Isolated partition-local-history suite for the composition IOTLB allocation
// policy (rv_iommu_iotlb PartitionLocalHistory = 1).
//
// Built by:
//     python3 tests/test_composition_iotlb.py --partition-local
// which elaborates tests/composition_iotlb_top.sv with
//     -GExplicitTouch=1 -GPartitionLocalHistory=1
// into out/composition-isolated-20261007/iotlb-test.  The default regression
// (out/composition-iotlb) and the frozen H1 history probe
// (out/b1-causal-20261006/iotlb-probe) keep their own trees and are untouched.
//
// Every temporal property is observed on the real RTL through the passive
// research tap (a_plru, a_replace_mask, a_q_*).  rtl_victim() mirrors the RTL
// replace_en walk and is cross-checked against the real a_replace_mask /
// b_replace_mask on every settle, so nothing is proven by a pure model.
//
//   * full mask 0xffff with explicit touch: i_actual (isolated) is bit-for-bit
//     equal to i_base and i_reference on lu_hit_o / lu_content_o and equal to
//     i_base on victim selection and PLRU state (equivalent stimulus: touch_i is
//     asserted on every lookup because i_base runs TouchEnable = 0);
//   * after legal 0x0003 / 0xfffc fills and touches, a V hit does not move the
//     next A victim, and an A hit does not move the next V victim;
//   * touch_i = 0 does not update the isolated tree;
//   * a same-edge touch+fill uses the old tree for the victim;
//   * policy_clear_i and reset drop all valid bits and clear the tree, and a
//     clear wins over a concurrent update;
//   * every fill is one-hot and inside the programmed mask;
//   * a frozen H1 counterexample: the shared touch steers the A victim to a
//     different way, the isolated touch does not.

#include "Vcomposition_iotlb_top.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

static Vcomposition_iotlb_top* top = 0;

static int         g_fail    = 0;
static long        g_checks  = 0;
static std::string g_phase   = "boot";
static bool        g_unified = false;  // full mask: a == b == r on every eval

static uint32_t g_rng = 0x2545F491u;
static uint32_t rng() {
    g_rng ^= g_rng << 13;
    g_rng ^= g_rng >> 17;
    g_rng ^= g_rng << 5;
    return g_rng;
}

static const uint16_t FULL_MASK = 0xffffu;
static const uint16_t V_MASK    = 0x0003u;   // isolated V partition (ways 0-1)
static const uint16_t A_MASK    = 0xfffcu;   // isolated A partition (ways 2-15)

// ------------------------------------------------------------------ helpers

static void fail(const std::string& msg) {
    ++g_fail;
    std::printf("FAIL [%s] %s\n", g_phase.c_str(), msg.c_str());
    if (g_fail > 60) {
        std::printf("FAIL [%s] more than 60 failures, aborting\n", g_phase.c_str());
        std::exit(1);
    }
}

static std::string dec(long long v) {
    char b[32];
    std::snprintf(b, sizeof(b), "%lld", v);
    return std::string(b);
}

static std::string hex16(uint32_t v) {
    char b[32];
    std::snprintf(b, sizeof(b), "%04llx", static_cast<unsigned long long>(v & 0xFFFFu));
    return std::string(b);
}

static std::string hex64(uint64_t v) {
    char b[32];
    std::snprintf(b, sizeof(b), "%016llx", static_cast<unsigned long long>(v));
    return std::string(b);
}

static bool is_onehot16(uint16_t m) {
    return m != 0u && ((m & static_cast<uint16_t>(m - 1u)) == 0u);
}

static int lowest_bit16(uint16_t m) {
    for (int i = 0; i < 16; i++) if (m & (1u << i)) return i;
    return -1;
}

// Mirror of the RTL replace_en walk (rv_iommu_iotlb.sv plru_replacement):
//   eligible[15+i] = mask bit i;  eligible[n] = eligible[2n+1] | eligible[2n+2]
//   choose_right = eligible[right] && (!eligible[left] || plru bit n)
// Cross-checked against the real a_replace_mask / b_replace_mask on every eval.
static int rtl_victim(uint32_t plru, uint16_t mask) {
    bool elig[31];
    for (int i = 0; i < 16; i++) elig[15 + i] = ((mask >> i) & 1u) != 0u;
    for (int i = 14; i >= 0; i--) elig[i] = elig[2 * i + 1] || elig[2 * i + 2];

    for (int i = 0; i < 16; i++) {
        if (!((mask >> i) & 1u)) continue;
        bool en = true;
        for (int lvl = 0; lvl < 4; lvl++) {
            int  idx_base = (1 << lvl) - 1;
            int  shift    = 4 - lvl;
            int  n        = idx_base + (i >> shift);
            bool ni       = ((i >> (shift - 1)) & 1) != 0;
            bool cr       = elig[2 * n + 2] &&
                            (!elig[2 * n + 1] || (((plru >> n) & 1u) != 0u));
            en = en && (ni ? cr : !cr);
        }
        if (en) return i;
    }
    return -1;
}

// ------------------------------------------------------------------ clocking

static void eval_checked(const std::string& where) {
    top->eval();
    ++g_checks;

    if (top->rst_ni) {
        // Victim-walk self check against the real RTL.  a allocates under the
        // programmed policy mask; b (PolicyEnable=0) always allocates full.
        uint16_t am = static_cast<uint16_t>(top->policy_mask_i);
        if (am != 0u) {
            int      wv = rtl_victim(top->a_plru, am);
            uint16_t m  = static_cast<uint16_t>(top->a_replace_mask);
            if (!is_onehot16(m) || lowest_bit16(m) != wv)
                fail("walk/a mismatch (" + where + "): walk=" + dec(wv) +
                     " mask=0x" + hex16(m) + " plru=0x" + hex16(top->a_plru));
        }
        int      wb = rtl_victim(top->b_plru, FULL_MASK);
        uint16_t mb = static_cast<uint16_t>(top->b_replace_mask);
        if (!is_onehot16(mb) || lowest_bit16(mb) != wb)
            fail("walk/b mismatch (" + where + "): walk=" + dec(wb) +
                 " mask=0x" + hex16(mb) + " plru=0x" + hex16(top->b_plru));
    }

    if (!g_unified) return;

    if (top->a_lu_hit_o != top->b_lu_hit_o ||
        top->a_content_lo != top->b_content_lo ||
        top->a_content_hi != top->b_content_hi) {
        fail("full-mask actual vs base functional mismatch (" + where + "): a=" +
             dec(static_cast<int>(top->a_lu_hit_o)) + ":" +
             hex64(top->a_content_hi) + ":" + hex64(top->a_content_lo) +
             " b=" + dec(static_cast<int>(top->b_lu_hit_o)) + ":" +
             hex64(top->b_content_hi) + ":" + hex64(top->b_content_lo));
    }
    if (top->a_lu_hit_o != top->r_lu_hit_o ||
        top->a_content_lo != top->r_content_lo ||
        top->a_content_hi != top->r_content_hi) {
        fail("full-mask actual vs reference functional mismatch (" + where + ")");
    }
    if (top->a_replace_mask != top->b_replace_mask || top->a_plru != top->b_plru) {
        fail("full-mask actual vs base victim/PLRU mismatch (" + where + "): a_mask=0x" +
             hex16(top->a_replace_mask) + " b_mask=0x" + hex16(top->b_replace_mask) +
             " a_plru=0x" + hex16(top->a_plru) + " b_plru=0x" + hex16(top->b_plru));
    }
}

static void settle(const std::string& where) { top->clk_i = 0; eval_checked(where); }
static void edge(const std::string& where) {
    top->clk_i = 1; eval_checked(where);
    top->clk_i = 0; eval_checked(where);
}

// ---------------------------------------------------------------- stimulus

static void clear_inval() {
    top->inval_vma_i   = 0;
    top->inval_gvma_i  = 0;
    top->inval_av_i    = 0;
    top->inval_gv_i    = 0;
    top->inval_pscv_i  = 0;
    top->inval_vpn_i   = 0;
    top->inval_gscid_i = 0;
    top->inval_pscid_i = 0;
}

static void clear_update() {
    top->update_update_i = 0;
    top->update_vpn_i    = 0;
    top->update_pscid_i  = 0;
    top->update_gscid_i  = 0;
    top->up_c1s_is_1G_i  = 0;
    top->up_c1s_is_2M_i  = 0;
    top->up_c1s_ppn_i    = 0;
    top->up_c1s_g_i      = 0;
    top->up_c1s_u_i      = 0;
    top->up_c1s_x_i      = 0;
    top->up_c1s_w_i      = 0;
    top->up_c1s_r_i      = 0;
    top->up_c1s_v_i      = 0;
    top->up_c2s_is_1G_i  = 0;
    top->up_c2s_is_2M_i  = 0;
    top->up_c2s_ppn_i    = 0;
    top->up_c2s_g_i      = 0;
    top->up_c2s_u_i      = 0;
    top->up_c2s_x_i      = 0;
    top->up_c2s_w_i      = 0;
    top->up_c2s_r_i      = 0;
    top->up_c2s_v_i      = 0;
}

static void clear_lookup() {
    top->lookup_i   = 0;
    top->touch_i    = 0;
    top->lu_vpn_i   = 0;
    top->lu_pscid_i = 0;
    top->lu_gscid_i = 0;
    top->en_1S_i    = 0;
    top->en_2S_i    = 0;
}

struct Tag {
    uint32_t vpn;
    uint32_t pscid;
    uint32_t gscid;
    int      en1s;
    int      en2s;
};

struct Content {
    int      c1_1g, c1_2m;
    uint64_t c1_ppn;
    int      c1_g, c1_u, c1_x, c1_w, c1_r, c1_v;
    int      c2_1g, c2_2m;
    uint64_t c2_ppn;
    int      c2_g, c2_u, c2_x, c2_w, c2_r, c2_v;
};

static Tag make_tag(uint32_t vpn, uint32_t pscid, uint32_t gscid, int en1s, int en2s) {
    Tag t;
    t.vpn   = vpn   & 0x1FFFFFFFu;
    t.pscid = pscid & 0xFFFFFu;
    t.gscid = gscid & 0xFFFFu;
    t.en1s  = en1s ? 1 : 0;
    t.en2s  = en2s ? 1 : 0;
    return t;
}

static Content make_content(uint64_t c1_ppn, int c1_1g, int c1_2m, int c1_g,
                            uint64_t c2_ppn, int c2_1g, int c2_2m, int c2_g) {
    Content c;
    c.c1_1g = c1_1g; c.c1_2m = c1_2m; c.c1_ppn = c1_ppn & 0xFFFFFFFFFFFULL; c.c1_g = c1_g;
    c.c1_u = 0; c.c1_x = 0; c.c1_w = 1; c.c1_r = 1; c.c1_v = 1;
    c.c2_1g = c2_1g; c.c2_2m = c2_2m; c.c2_ppn = c2_ppn & 0xFFFFFFFFFFFULL; c.c2_g = c2_g;
    c.c2_u = 0; c.c2_x = 0; c.c2_w = 1; c.c2_r = 1; c.c2_v = 1;
    return c;
}

static uint32_t mk_vpn(uint32_t vpn2, uint32_t vpn1, uint32_t vpn0) {
    return ((vpn2 & 0x7FFu) << 18) | ((vpn1 & 0x1FFu) << 9) | (vpn0 & 0x1FFu);
}

static uint32_t g_vpn2 = 0x100u;
static uint32_t next_vpn2() { return g_vpn2++; }

static Tag random_tag() {
    uint32_t vpn   = rng() & 0x1FFFFFFFu;
    uint32_t pscid = rng() & 0xFFFFFu;
    uint32_t gscid = rng() & 0xFFFFu;
    int      en1   = (rng() & 1u) ? 1 : 0;
    int      en2   = (rng() & 1u) ? 1 : 0;
    if (!en1 && !en2) en1 = 1;
    return make_tag(vpn, pscid, gscid, en1, en2);
}

static Content random_content() {
    Content c;
    c.c1_1g = 0; c.c1_2m = 0; c.c1_ppn = rng() & 0xFFFFFFFFFFFULL;
    c.c1_g = (rng() & 1u) ? 1 : 0; c.c1_u = 0; c.c1_x = 0; c.c1_w = 1; c.c1_r = 1; c.c1_v = 1;
    c.c2_1g = 0; c.c2_2m = 0; c.c2_ppn = rng() & 0xFFFFFFFFFFFULL;
    c.c2_g = (rng() & 1u) ? 1 : 0; c.c2_u = 0; c.c2_x = 0; c.c2_w = 1; c.c2_r = 1; c.c2_v = 1;
    return c;
}

static void drive_update(const Tag& t, const Content& c) {
    clear_lookup();
    top->update_update_i = 1;
    top->update_vpn_i    = t.vpn;
    top->update_pscid_i  = t.pscid;
    top->update_gscid_i  = t.gscid;
    top->en_1S_i         = (t.en1s != 0);
    top->en_2S_i         = (t.en2s != 0);
    top->up_c1s_is_1G_i  = c.c1_1g;
    top->up_c1s_is_2M_i  = c.c1_2m;
    top->up_c1s_ppn_i    = c.c1_ppn;
    top->up_c1s_g_i      = c.c1_g;
    top->up_c1s_u_i      = c.c1_u;
    top->up_c1s_x_i      = c.c1_x;
    top->up_c1s_w_i      = c.c1_w;
    top->up_c1s_r_i      = c.c1_r;
    top->up_c1s_v_i      = c.c1_v;
    top->up_c2s_is_1G_i  = c.c2_1g;
    top->up_c2s_is_2M_i  = c.c2_2m;
    top->up_c2s_ppn_i    = c.c2_ppn;
    top->up_c2s_g_i      = c.c2_g;
    top->up_c2s_u_i      = c.c2_u;
    top->up_c2s_x_i      = c.c2_x;
    top->up_c2s_w_i      = c.c2_w;
    top->up_c2s_r_i      = c.c2_r;
    top->up_c2s_v_i      = c.c2_v;
}

static void drive_lookup(const Tag& t) {
    clear_inval();
    clear_update();
    top->lookup_i   = 1;
    top->touch_i    = 0;
    top->lu_vpn_i   = t.vpn;
    top->lu_pscid_i = t.pscid;
    top->lu_gscid_i = t.gscid;
    top->en_1S_i    = (t.en1s != 0);
    top->en_2S_i    = (t.en2s != 0);
}

// ------------------------------------------------------------- tap access

static void read_q(int idx, uint64_t& lo, uint64_t& hi) {
    top->tap_idx = static_cast<uint8_t>(idx & 0xF);
    top->eval();
    lo = top->a_q_lo;
    hi = top->a_q_hi;
}

static int lo_valid(uint64_t lo) { return static_cast<int>((lo >> 51) & 1u); }

static int count_valid() {
    int n = 0;
    for (int i = 0; i < 16; i++) {
        uint64_t lo, hi;
        read_q(i, lo, hi);
        if (lo_valid(lo)) ++n;
    }
    return n;
}

static void check_all_invalid(const std::string& label) {
    for (int i = 0; i < 16; i++) {
        uint64_t lo, hi;
        read_q(i, lo, hi);
        if (lo_valid(lo)) fail(label + ": way " + dec(i) + " is still valid");
    }
}

static void check_entry_bits(uint64_t lo, uint64_t hi, const Tag& t, const Content& c,
                             const std::string& label) {
    if ((lo & 0x1FFFFFFFu) != (t.vpn & 0x1FFFFFFFu))          fail(label + ": VPN mismatch");
    if (((lo >> 29) & 0xFFFFFu) != (t.pscid & 0xFFFFFu))      fail(label + ": PSCID mismatch");
    if (((lo >> 49) & 1u) != static_cast<uint32_t>(t.en1s))   fail(label + ": en_1S mismatch");
    if (((lo >> 50) & 1u) != static_cast<uint32_t>(t.en2s))   fail(label + ": en_2S mismatch");
    if (((lo >> 51) & 1u) != 1u)                              fail(label + ": valid not set");
    if (((lo >> 52) & 1u) != static_cast<uint32_t>(c.c1_1g))  fail(label + ": c1.is_1G mismatch");
    if (((lo >> 53) & 1u) != static_cast<uint32_t>(c.c1_2m))  fail(label + ": c1.is_2M mismatch");
    if (((lo >> 54) & 1u) != static_cast<uint32_t>(c.c2_1g))  fail(label + ": c2.is_1G mismatch");
    if (((lo >> 55) & 1u) != static_cast<uint32_t>(c.c2_2m))  fail(label + ": c2.is_2M mismatch");
    if (((lo >> 56) & 1u) != static_cast<uint32_t>(c.c1_g))   fail(label + ": c1.g mismatch");
    if ((hi & 0xFFFFu) != (t.gscid & 0xFFFFu))                fail(label + ": GSCID mismatch");
}

// ------------------------------------------------------- directed commands

static void do_reset() {
    top->rst_ni = 0;
    top->policy_clear_i = 0;
    clear_inval(); clear_update(); clear_lookup();
    for (int i = 0; i < 4; i++) edge("reset");
    top->rst_ni = 1;
    settle("post-reset");
}

// One legal fill under `mask`, committed through a posedge.  Checks one-hot,
// in-mask placement and the walk, then verifies the committed entry bits.
static int fill(uint16_t mask, const Tag& t, const Content& c, const std::string& label) {
    top->policy_mask_i = mask;
    clear_inval();
    drive_update(t, c);
    settle(label + " settle");
    uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
    if (!is_onehot16(m)) fail(label + ": replace_mask not one-hot: 0x" + hex16(m));
    if ((m & static_cast<uint16_t>(~mask)) != 0)
        fail(label + ": victim 0x" + hex16(m) + " escapes mask 0x" + hex16(mask));
    int v = lowest_bit16(m);
    edge(label + " edge");
    uint64_t lo, hi;
    read_q(v, lo, hi);
    check_entry_bits(lo, hi, t, c, label + " way " + dec(v));
    return v;
}

// A live lookup hit with an explicit valid touch, under `mask`.
static void touch_hit(uint16_t mask, const Tag& t, const std::string& label) {
    top->policy_mask_i = mask;
    drive_lookup(t);
    top->touch_i = 1;
    settle(label + " settle");
    if (!top->a_lu_hit_o) fail(label + ": lookup does not hit");
    edge(label + " edge");
}

// Reads the current isolated victim for `mask` without committing an update.
static int probe_victim(uint16_t mask, const std::string& label) {
    top->policy_mask_i = mask;
    Tag     t = make_tag(0x1FFFFu, 0x7FFu, 0x1FFu, 1, 0);   // never committed
    Content c = make_content(0, 0, 0, 0, 0, 0, 0, 0);
    clear_inval();
    drive_update(t, c);
    settle(label + " probe");
    uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
    if (!is_onehot16(m)) fail(label + ": replace_mask not one-hot: 0x" + hex16(m));
    if ((m & static_cast<uint16_t>(~mask)) != 0)
        fail(label + ": victim 0x" + hex16(m) + " escapes mask 0x" + hex16(mask));
    int v = lowest_bit16(m);
    clear_update();
    settle(label + " restore");
    return v;
}

// ------------------------------------------------------------------- phases

static void phase_reset() {
    g_phase   = "reset";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = FULL_MASK;
    do_reset();
    check_all_invalid("reset");
    if (top->a_plru != 0) fail("reset: a_plru not cleared: 0x" + hex16(top->a_plru));
    if (top->b_plru != 0) fail("reset: b_plru not cleared: 0x" + hex16(top->b_plru));
}

// Full mask: the isolated instance must reproduce i_base / i_reference exactly.
// Equivalent stimulus: touch_i is asserted on every lookup because i_base runs
// TouchEnable = 0 (every hit is a touch there).
static void phase_fullmask() {
    g_phase   = "fullmask-equivalent";
    g_unified = true;
    top->policy_clear_i = 0;
    top->policy_mask_i  = FULL_MASK;
    do_reset();

    static Tag pool[128];
    int np = 0;

    for (int it = 0; it < 3000; it++) {
        uint32_t r = rng();
        if (r & 1u) {
            Tag     t = random_tag();
            Content c = random_content();
            top->policy_mask_i = FULL_MASK;
            clear_inval();
            drive_update(t, c);
            settle("fm fill");
            edge("fm fill");
            if (np < 128) pool[np++] = t;
        } else {
            Tag t = (np > 0 && (rng() & 1u)) ? pool[rng() % static_cast<uint32_t>(np)]
                                             : random_tag();
            top->policy_mask_i = FULL_MASK;
            drive_lookup(t);
            top->touch_i = 1;   // equivalent to the base TouchEnable = 0
            settle("fm lookup");
            edge("fm lookup");
        }
        if (g_fail) return;
    }
}

// Legal 0x0003 / 0xfffc fills and touches, then V/A cross-partition invariance.
static void phase_isolation_legal() {
    g_phase   = "isolation-legal";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = A_MASK;
    do_reset();

    // Build an A state with legal fills + A touches (no V activity yet, so the
    // isolated and base A-affecting tree nodes are identical).
    static const int NA = 6;
    Tag     at[NA];
    Content ac[NA];
    for (int k = 0; k < NA; k++) {
        at[k] = make_tag(next_vpn2(), 0x0A00u + static_cast<uint32_t>(k),
                         0x0B00u + static_cast<uint32_t>(k), 1, 0);
        ac[k] = make_content(0x5000u + static_cast<uint32_t>(k), 0, 0, 0, 0, 0, 0, 0);
        fill(A_MASK, at[k], ac[k], "iso A fill " + dec(k));
        touch_hit(A_MASK, at[k], "iso A touch " + dec(k));
    }
    if (count_valid() == 0) fail("iso: no valid entry after the A build");

    int vA_before = probe_victim(A_MASK, "iso A victim before");
    uint32_t a_tree_before = top->a_plru;

    // One legal V fill + V touch under mask 0x0003.
    Tag     vt = make_tag(next_vpn2(), 0x0C00u, 0x0D00u, 1, 0);
    Content vc = make_content(0x6000u, 0, 0, 0, 0, 0, 0, 0);
    int vw = fill(V_MASK, vt, vc, "iso V fill");
    if (vw != 0 && vw != 1) fail("iso: V fill landed in way " + dec(vw) + ", outside 0-1");
    touch_hit(V_MASK, vt, "iso V touch");

    uint32_t a_delta = top->a_plru ^ a_tree_before;
    if (a_delta != 0x0080u)
        fail("iso: V touch flipped 0x" + hex16(a_delta) + ", expected only node 7 (0x0080)");

    int vA_after = probe_victim(A_MASK, "iso A victim after");
    if (vA_after != vA_before)
        fail("iso: a V hit moved the A victim: " + dec(vA_before) + " -> " + dec(vA_after));

    // Reverse direction: an A touch must not move the next V victim.
    int vV_before = probe_victim(V_MASK, "iso V victim before");
    touch_hit(A_MASK, at[NA - 1], "iso reverse A touch");
    int vV_after = probe_victim(V_MASK, "iso V victim after");
    if (vV_after != vV_before)
        fail("iso: an A hit moved the V victim: " + dec(vV_before) + " -> " + dec(vV_after));

    // The V tree bit is the only node a V touch may write.
    if ((top->a_plru & 0x0080u) == 0u)
        fail("iso: node 7 is clear after a V touch: 0x" + hex16(top->a_plru));
}

// touch_i = 0 must not update the isolated tree.
static void phase_touch0() {
    g_phase   = "touch0-no-update";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = A_MASK;
    do_reset();

    Tag     t = make_tag(next_vpn2(), 0x1100u, 0x1200u, 1, 0);
    Content c = make_content(0x8000u, 0, 0, 0, 0, 0, 0, 0);
    fill(A_MASK, t, c, "t0 fill");
    touch_hit(A_MASK, t, "t0 touch");

    uint32_t p0 = top->a_plru;
    int      v0 = probe_victim(A_MASK, "t0 victim before");

    top->policy_mask_i = A_MASK;
    drive_lookup(t);            // touch_i = 0
    settle("t0 lookup");
    if (!top->a_lu_hit_o) fail("t0: resident A page does not hit");
    if (top->a_plru != p0) fail("t0: touch_i=0 changed the isolated tree");
    edge("t0 lookup");
    if (top->a_plru != p0) fail("t0: touch_i=0 changed the isolated tree (post-edge)");

    int v1 = probe_victim(A_MASK, "t0 victim after");
    if (v1 != v0) fail("t0: touch_i=0 moved the A victim: " + dec(v0) + " -> " + dec(v1));
}

// A same-edge touch + fill must select the victim from the old tree.
static void phase_same_edge() {
    g_phase   = "same-edge-old-tree";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = A_MASK;
    do_reset();

    Tag     t = make_tag(next_vpn2(), 0x1300u, 0x1400u, 1, 0);
    Content c = make_content(0x9000u, 0, 0, 0, 0, 0, 0, 0);
    fill(A_MASK, t, c, "se fill");
    touch_hit(A_MASK, t, "se touch");

    Tag     t2 = make_tag(next_vpn2(), 0x1500u, 0x1600u, 1, 0);
    Content c2 = make_content(0x9100u, 0, 0, 0, 0, 0, 0, 0);

    uint32_t p0 = top->a_plru;
    int      v0 = probe_victim(A_MASK, "se victim");

    // Same-cycle touch (resident t) and fill (t2).
    top->policy_mask_i = A_MASK;
    drive_lookup(t);        // clears update/inval, sets lookup, touch_i = 0
    top->touch_i         = 1;
    top->update_update_i = 1;
    top->update_vpn_i    = t2.vpn;
    top->update_pscid_i  = t2.pscid;
    top->update_gscid_i  = t2.gscid;
    top->up_c1s_is_1G_i  = c2.c1_1g;  top->up_c1s_is_2M_i = c2.c1_2m;
    top->up_c1s_ppn_i    = c2.c1_ppn; top->up_c1s_g_i     = c2.c1_g;
    top->up_c1s_u_i      = c2.c1_u;   top->up_c1s_x_i     = c2.c1_x;
    top->up_c1s_w_i      = c2.c1_w;   top->up_c1s_r_i     = c2.c1_r;
    top->up_c1s_v_i      = c2.c1_v;
    top->up_c2s_is_1G_i  = c2.c2_1g;  top->up_c2s_is_2M_i = c2.c2_2m;
    top->up_c2s_ppn_i    = c2.c2_ppn; top->up_c2s_g_i     = c2.c2_g;
    top->up_c2s_u_i      = c2.c2_u;   top->up_c2s_x_i     = c2.c2_x;
    top->up_c2s_w_i      = c2.c2_w;   top->up_c2s_r_i     = c2.c2_r;
    top->up_c2s_v_i      = c2.c2_v;

    settle("se same-edge");
    if (top->a_plru != p0) fail("se: the tree changed before the edge");
    if (!top->a_lu_hit_o) fail("se: the resident page does not hit in the same edge");
    {
        uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
        if (!is_onehot16(m) || lowest_bit16(m) != v0)
            fail("se: same-edge fill used mask 0x" + hex16(m) + " instead of the old-tree victim " +
                 dec(v0));
    }
    edge("se same-edge");
    if (top->a_plru == p0) fail("se: the same-edge touch did not update the tree");
    {
        uint64_t lo, hi;
        read_q(v0, lo, hi);
        check_entry_bits(lo, hi, t2, c2, "se t2 committed");
    }
}

static void phase_policy_clear() {
    g_phase   = "policy-clear";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = A_MASK;
    do_reset();

    for (int k = 0; k < 4; k++) {
        Tag     t = make_tag(next_vpn2(), 0x1700u + static_cast<uint32_t>(k),
                             0x1800u + static_cast<uint32_t>(k), 1, 0);
        Content c = make_content(0xA000u + static_cast<uint32_t>(k), 0, 0, 0, 0, 0, 0, 0);
        fill(A_MASK, t, c, "clr fill " + dec(k));
    }
    if (count_valid() == 0) fail("clr: no valid entry before the clear");

    top->policy_clear_i = 1;
    clear_update(); clear_lookup(); clear_inval();
    settle("clr pre-edge");
    edge("clr edge");
    check_all_invalid("clr");
    if (top->a_plru != 0) fail("clr: tree not cleared: 0x" + hex16(top->a_plru));

    // Concurrent update + clear: the clear wins.
    Tag     tu = make_tag(next_vpn2(), 0x1900u, 0x1A00u, 1, 0);
    Content cu = make_content(0xB000u, 0, 0, 0, 0, 0, 0, 0);
    top->policy_clear_i = 1;
    top->policy_mask_i  = A_MASK;
    drive_update(tu, cu);
    settle("clr+upd");
    edge("clr+upd");
    check_all_invalid("clr+upd");
    top->policy_clear_i = 0;

    // Allocation resumes after the clear is released.
    fill(A_MASK, tu, cu, "clr resume");
    touch_hit(A_MASK, tu, "clr resume touch");
}

// Frozen H1 counterexample.  Synthetic priming (16 single-way fills) is used
// ONLY to fix a known start state; it is not claimed to be board-reachable.
// The single-way masks mean i_actual and i_base prime the very same ways, so the
// shared-tree counterfactual is clean.
static void phase_h1_counterexample() {
    g_phase   = "h1-counterexample";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = FULL_MASK;
    do_reset();

    Tag     pt[16];
    Content pc[16];
    for (int k = 0; k < 16; k++) {
        pt[k] = make_tag(next_vpn2(), 0x00E0u + static_cast<uint32_t>(k),
                         0x00F0u + static_cast<uint32_t>(k), 1, 0);
        pc[k] = make_content(0x7000u + static_cast<uint32_t>(k), 0, 0, 0, 0, 0, 0, 0);
        top->policy_mask_i = static_cast<uint16_t>(1u << k);
        clear_inval();
        drive_update(pt[k], pc[k]);
        settle("h1 prime " + dec(k));
        int w = lowest_bit16(static_cast<uint16_t>(top->a_replace_mask));
        if (w != k) fail("h1 prime: way " + dec(k) + " chose " + dec(w));
        edge("h1 prime " + dec(k));
    }
    if (top->a_plru != 0) fail("h1: priming moved the isolated tree: 0x" + hex16(top->a_plru));
    if (top->b_plru != 0) fail("h1: priming moved the shared tree: 0x" + hex16(top->b_plru));

    int vA_base = probe_victim(A_MASK, "h1 A victim baseline");
    if (vA_base != 2) fail("h1: baseline A victim is " + dec(vA_base) + ", expected 2");

    uint32_t a0 = top->a_plru;
    uint32_t b0 = top->b_plru;

    // V way-0 touch with the policy mask naming the V partition.
    touch_hit(V_MASK, pt[0], "h1 V touch");

    uint32_t a_delta = top->a_plru ^ a0;
    uint32_t b_delta = top->b_plru ^ b0;
    if (top->a_plru != 0x0080u)
        fail("h1: isolated tree is 0x" + hex16(top->a_plru) + ", expected 0x0080");
    if (top->b_plru != 0x008bu)
        fail("h1: shared tree is 0x" + hex16(top->b_plru) + ", expected 0x008b");
    if (a_delta != 0x0080u)
        fail("h1: isolated V-touch delta 0x" + hex16(a_delta) + ", expected 0x0080");
    if (b_delta != 0x008Bu)
        fail("h1: shared V-touch delta 0x" + hex16(b_delta) + ", expected 0x008b");

    int vA_iso = probe_victim(A_MASK, "h1 A victim after V touch");
    if (vA_iso != vA_base)
        fail("h1: V touch moved the isolated A victim: " + dec(vA_base) + " -> " + dec(vA_iso));

    int vA_shared = rtl_victim(top->b_plru, A_MASK);
    if (vA_shared != 8)
        fail("h1: shared-policy A victim is " + dec(vA_shared) + ", expected 8");
    if (vA_shared == vA_iso)
        fail("h1: counterexample failed to separate the isolated and shared victims");
}

// --------------------------------------------------------------------- main

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        const std::string arg = (argv[i] != 0) ? argv[i] : "";
        if (arg == "--partition-local") { /* accepted for parity with the runner */ }
    }

    Verilated::commandArgs(argc, argv);
    top = new Vcomposition_iotlb_top;

    top->clk_i          = 0;
    top->rst_ni         = 0;
    top->tap_idx        = 0;
    top->policy_clear_i = 0;
    top->policy_mask_i  = FULL_MASK;
    top->touch_i        = 0;
    clear_inval();
    clear_update();
    clear_lookup();

    g_phase   = "reset";
    g_unified = false;
    for (int i = 0; i < 4; i++) {
        top->clk_i = 1; eval_checked("reset");
        top->clk_i = 0; eval_checked("reset");
    }
    top->rst_ni = 1;
    settle("post-reset");

    if (!top->a_supported) fail("actual instance does not report NumIotlbEntries == 16");

    phase_reset();
    phase_fullmask();
    phase_isolation_legal();
    phase_touch0();
    phase_same_edge();
    phase_policy_clear();
    phase_h1_counterexample();

    if (g_fail == 0) {
        std::printf("PASS [partition-local]: %ld checked evaluations, 0 failures\n", g_checks);
        delete top;
        return 0;
    }
    std::printf("FAIL [partition-local]: %d failures across %ld checked evaluations\n",
                g_fail, g_checks);
    delete top;
    return 1;
}
