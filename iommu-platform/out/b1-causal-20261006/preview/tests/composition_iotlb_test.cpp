// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded verification harness for the composition IOTLB allocation policy
// (profile migration-4dma-composition, nonblock_mode = 3).
//
// tests/composition_iotlb_top.sv instantiates the current rv_iommu_iotlb three
// times -- PolicyEnable=1 (actual), PolicyEnable=0 (base) and the extracted
// pre-change reference -- with identical stimulus.  This testbench drives
// directed and deterministic pseudo-random multi-state sequences and checks:
//
//   * on the full mask 0xffff with clear deasserted the actual instance is
//     bit-for-bit equivalent to both the policy-disabled instance and the
//     pre-change reference (lu_hit_o / lu_content_o) on every evaluation, and
//     its victim selection and PLRU state match the policy-disabled one;
//   * for restricted masks (0x0003, 0xfffc and single-way masks) the victim is
//     always exactly one entry that lies inside the programmed mask;
//   * two protected entries filled under mask 0x0003 survive 100 subsequent
//     mask 0xfffc fills and remain hit-able;
//   * policy_clear_i drops every valid bit and clears the PLRU tree, and wins
//     over a concurrent IOTLB update;
//   * IOTINVAL.VMA / IOTINVAL.GVMA keep the untouched content of surviving
//     entries and behave exactly like the reference;
//   * updates, lookup misses and invalidations add no touch side effects and do
//     not perturb the PLRU tree beyond a lookup hit.
//
// No Python model of the replacement policy is used: every temporal property is
// observed on the real RTL through the passive research tap.
//
// --history-probe (frozen 2026-10-06; out/b1-causal-20261006/iotlb-probe) runs
// only the four frozen H1 cases against the real RTL with TouchEnable = 1.  Its
// counterfactual is a test-side partition-local rule model ("shadow"); the
// i_base / i_reference outputs are ignored in that branch.  The default suite
// below never runs when the flag is present.

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
static bool        g_unified = true;   // mask==0xffff and clear deasserted
static bool        g_probe   = false;  // set by --history-probe (H1 probe only)
static int         g_edge_no = 0;      // case-local posedge counter for the probe event log

static uint32_t g_rng = 0x9e3779b9u;
static uint32_t rng() {
    g_rng ^= g_rng << 13;
    g_rng ^= g_rng >> 17;
    g_rng ^= g_rng << 5;
    return g_rng;
}

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

// ------------------------------------------------------------------ clocking

static void eval_checked(const std::string& where) {
    top->eval();
    ++g_checks;
    if (!g_unified) return;

    if (top->a_lu_hit_o != top->b_lu_hit_o ||
        top->a_content_lo != top->b_content_lo ||
        top->a_content_hi != top->b_content_hi) {
        fail("actual vs base functional mismatch (" + where + "): a=" +
             dec(static_cast<int>(top->a_lu_hit_o)) + ":" + hex64(top->a_content_hi) + ":" + hex64(top->a_content_lo) +
             " b=" + dec(static_cast<int>(top->b_lu_hit_o)) + ":" + hex64(top->b_content_hi) + ":" + hex64(top->b_content_lo));
    }
    if (top->a_lu_hit_o != top->r_lu_hit_o ||
        top->a_content_lo != top->r_content_lo ||
        top->a_content_hi != top->r_content_hi) {
        fail("actual vs reference functional mismatch (" + where + "): a=" +
             dec(static_cast<int>(top->a_lu_hit_o)) + ":" + hex64(top->a_content_hi) + ":" + hex64(top->a_content_lo) +
             " r=" + dec(static_cast<int>(top->r_lu_hit_o)) + ":" + hex64(top->r_content_hi) + ":" + hex64(top->r_content_lo));
    }
    if (top->a_replace_mask != top->b_replace_mask || top->a_plru != top->b_plru) {
        fail("actual vs base victim/PLRU mismatch (" + where + "): a_mask=0x" +
             hex16(top->a_replace_mask) + " b_mask=0x" + hex16(top->b_replace_mask) +
             " a_plru=0x" + hex16(top->a_plru) + " b_plru=0x" + hex16(top->b_plru));
    }
}

static void settle(const std::string& where) { top->clk_i = 0; eval_checked(where); }
static void edge(const std::string& where) {
    top->clk_i = 1; eval_checked(where);
    top->clk_i = 0; eval_checked(where);
    ++g_edge_no;   // case-local posedge counter; unused by the default suite
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
    top->touch_i    = 0;   // explicit: no stale touch is ever left asserted
    top->lu_vpn_i   = 0;
    top->lu_pscid_i = 0;
    top->lu_gscid_i = 0;
    top->en_1S_i    = 0;
    top->en_2S_i    = 0;
}

// ------------------------------------------------------------------- model

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

static uint32_t g_vpn2 = 0x40u;
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
    // rv_iommu_iotlb latches en_1S_i / en_2S_i together with the update.
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
    top->touch_i    = 0;   // a plain lookup never touches the tree
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

static void read_n(int idx, uint64_t& lo, uint64_t& hi) {
    top->tap_idx = static_cast<uint8_t>(idx & 0xF);
    top->eval();
    lo = top->a_n_lo;
    hi = top->a_n_hi;
}

static int      lo_valid(uint64_t lo) { return static_cast<int>((lo >> 51) & 1u); }

static bool is_onehot16(uint16_t m) {
    return m != 0u && ((m & static_cast<uint16_t>(m - 1u)) == 0u);
}

static int lowest_bit16(uint16_t m) {
    for (int i = 0; i < 16; i++) if (m & (1u << i)) return i;
    return -1;
}

static void snapshot_all(uint64_t out[16][2]) {
    for (int i = 0; i < 16; i++) read_q(i, out[i][0], out[i][1]);
}

// Invalidation may only clear the valid bit; every other visible bit of every
// entry (VPN, PSCID, stage, size, G, GSCID) must be preserved.
static void check_content_preserved(uint64_t before[16][2], const std::string& label) {
    for (int i = 0; i < 16; i++) {
        uint64_t lo, hi;
        read_q(i, lo, hi);
        uint64_t alo = lo & ~(1ULL << 51);
        uint64_t blo = before[i][0] & ~(1ULL << 51);
        if (alo != blo || hi != before[i][1]) {
            fail(label + ": entry " + dec(i) + " content changed by invalidation");
        }
    }
}

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

// Writes one entry through the real replacement path, then touches it so the
// PLRU advances.  Returns the way it landed in.
static int write_and_touch(const Tag& t, const Content& c) {
    clear_inval();
    drive_update(t, c);
    settle("w+t pre-edge");

    uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
    if (!is_onehot16(m)) fail("w+t: replace_mask not one-hot: 0x" + hex16(m));
    int v = lowest_bit16(m);

    uint64_t nlo, nhi;
    read_n(v, nlo, nhi);
    check_entry_bits(nlo, nhi, t, c, "w+t next way " + dec(v));

    edge("w+t edge");

    uint64_t qlo, qhi;
    read_q(v, qlo, qhi);
    check_entry_bits(qlo, qhi, t, c, "w+t committed way " + dec(v));

    drive_lookup(t);
    settle("w+t touch");
    if (!top->a_lu_hit_o) fail("w+t: freshly written way does not hit");
    edge("w+t touch edge");
    return v;
}

static void do_reset() {
    top->rst_ni = 0;
    top->policy_clear_i = 0;
    clear_inval(); clear_update(); clear_lookup();
    for (int i = 0; i < 4; i++) {
        edge("reset");   // counted by g_edge_no like every other posedge
    }
    top->rst_ni = 1;
    settle("post-reset");
}

// ------------------------------------------------------------ invalidations

static void apply_inval_vma(int gv, int av, int pscv, uint32_t vpn, uint32_t pscid, uint32_t gscid) {
    clear_update();
    clear_lookup();
    top->inval_vma_i   = 1;
    top->inval_gvma_i  = 0;
    top->inval_gv_i    = (gv   != 0);
    top->inval_av_i    = (av   != 0);
    top->inval_pscv_i  = (pscv != 0);
    top->inval_vpn_i   = vpn   & 0x1FFFFFFFu;
    top->inval_gscid_i = gscid & 0xFFFFu;
    top->inval_pscid_i = pscid & 0xFFFFFu;
    settle("inval vma settle");
    if (!top->a_invalidate) fail("tap invalidate is low while IOTINVAL.VMA is asserted");
    if (top->a_update)      fail("tap update is high while IOTINVAL.VMA is asserted");
    edge("inval vma edge");
    clear_inval();
    settle("post-inval vma settle");
}

static void apply_inval_gvma(int gv, int av, uint32_t vpn, uint32_t gscid) {
    clear_update();
    clear_lookup();
    top->inval_vma_i   = 0;
    top->inval_gvma_i  = 1;
    top->inval_gv_i    = (gv != 0);
    top->inval_av_i    = (av != 0);
    top->inval_pscv_i  = 0;
    top->inval_vpn_i   = vpn & 0x1FFFFFFFu;
    top->inval_gscid_i = gscid & 0xFFFFu;
    top->inval_pscid_i = 0;
    settle("inval gvma settle");
    if (!top->a_invalidate) fail("tap invalidate is low while IOTINVAL.GVMA is asserted");
    if (top->a_update)      fail("tap update is high while IOTINVAL.GVMA is asserted");
    edge("inval gvma edge");
    clear_inval();
    settle("post-inval gvma settle");
}

// ------------------------------------------------------------------- phases

static void phase_full_mask_fuzz() {
    g_phase   = "full-mask-fuzz";
    g_unified = true;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    do_reset();

    static Tag pool[256];
    int npool = 0;

    for (int it = 0; it < 5000; it++) {
        uint32_t r = rng();
        switch (r & 3u) {
        case 0u:
        case 1u: {
            Tag     t = random_tag();
            Content c = random_content();
            clear_inval();
            drive_update(t, c);
            settle("fuzz update");
            if (!is_onehot16(static_cast<uint16_t>(top->a_replace_mask)))
                fail("fuzz: replace_mask not one-hot: 0x" + hex16(top->a_replace_mask));
            edge("fuzz update");
            if (npool < 256) pool[npool++] = t;
            break;
        }
        case 2u: {
            Tag t;
            if (npool > 0 && (rng() & 1u)) t = pool[rng() % static_cast<uint32_t>(npool)];
            else                           t = random_tag();
            drive_lookup(t);
            settle("fuzz lookup");
            edge("fuzz lookup");
            break;
        }
        default: {
            uint32_t rr = rng();
            if (rr & 1u) {
                apply_inval_vma(static_cast<int>((rr >> 1) & 1u), static_cast<int>((rr >> 2) & 1u),
                                static_cast<int>((rr >> 3) & 1u),
                                rng() & 0x1FFFFFFFu, rng() & 0xFFFFFu, rng() & 0xFFFFu);
            } else {
                apply_inval_gvma(static_cast<int>((rr >> 1) & 1u), static_cast<int>((rr >> 2) & 1u),
                                 rng() & 0x1FFFFFFFu, rng() & 0xFFFFu);
            }
            break;
        }
        }
    }
}

static void mask_subset_case(uint16_t mask, const char* name) {
    g_phase   = std::string("mask-") + name;
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = mask;
    do_reset();

    for (int it = 0; it < 300; it++) {
        Tag     t = random_tag();
        Content c = random_content();
        drive_update(t, c);
        settle("mask update");
        uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
        if (!is_onehot16(m))
            fail(std::string(name) + ": replace_mask not one-hot: 0x" + hex16(m));
        if ((m & static_cast<uint16_t>(~mask)) != 0)
            fail(std::string(name) + ": victim 0x" + hex16(m) + " escapes mask 0x" + hex16(mask));
        edge("mask update");

        drive_lookup(t);
        settle("mask lookup");
        edge("mask lookup");
    }

    for (int i = 0; i < 16; i++) {
        uint64_t lo, hi;
        read_q(i, lo, hi);
        if (lo_valid(lo) && !(mask & (1u << i)))
            fail(std::string(name) + ": valid entry landed outside the mask in way " + dec(i));
    }
}

static void phase_mask_subsets() {
    mask_subset_case(0x0003u, "0003");
    mask_subset_case(0xFFFCu, "fffc");
    mask_subset_case(0x0001u, "0001");
    mask_subset_case(0x0002u, "0002");
    mask_subset_case(0x8000u, "8000");
}

static void phase_protected_retention() {
    g_phase   = "protected-retention";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0x0003;
    do_reset();

    Tag     prot[2];
    Content protc[2];
    int     protway[2];

    for (int p = 0; p < 2; p++) {
        prot[p]  = make_tag(next_vpn2(), 0x100u + static_cast<uint32_t>(p),
                            0x200u + static_cast<uint32_t>(p), 1, 0);
        protc[p] = make_content(0x1000u + static_cast<uint32_t>(p), 0, 0, 0, 0, 0, 0, 0);

        drive_update(prot[p], protc[p]);
        settle("prot fill");
        uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
        if (!is_onehot16(m)) fail("prot: replace_mask not one-hot: 0x" + hex16(m));
        if (m & static_cast<uint16_t>(~0x0003u)) fail("prot: victim outside the protected mask: 0x" + hex16(m));
        protway[p] = lowest_bit16(m);
        edge("prot fill");

        drive_lookup(prot[p]);
        settle("prot touch");
        if (!top->a_lu_hit_o) fail("prot: freshly filled protected tag does not hit");
        edge("prot touch");
    }
    if (protway[0] == protway[1]) fail("prot: both protected fills landed in the same way");

    top->policy_mask_i = 0xFFFC;
    for (int k = 0; k < 100; k++) {
        Tag     t = make_tag(next_vpn2(), 0x300u + static_cast<uint32_t>(k),
                             0x400u + static_cast<uint32_t>(k), 1, 0);
        Content c = make_content(0x2000u + static_cast<uint32_t>(k), 0, 0, 0, 0, 0, 0, 0);

        drive_update(t, c);
        settle("other fill");
        uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
        if (!is_onehot16(m)) fail("other: replace_mask not one-hot: 0x" + hex16(m));
        if (m & 0x0003u)     fail("other: fill evicted a protected way: 0x" + hex16(m));
        edge("other fill");

        if ((k % 10) == 0) {
            for (int p = 0; p < 2; p++) {
                uint64_t lo, hi;
                read_q(protway[p], lo, hi);
                if (!lo_valid(lo)) fail("other: protected way " + dec(protway[p]) + " became invalid");
                drive_lookup(prot[p]);
                settle("prot recheck");
                if (!top->a_lu_hit_o) fail("other: protected tag " + dec(p) + " no longer hits");
                edge("prot recheck");
            }
        }
    }

    for (int p = 0; p < 2; p++) {
        uint64_t lo, hi;
        read_q(protway[p], lo, hi);
        check_entry_bits(lo, hi, prot[p], protc[p], "protected final way " + dec(protway[p]));
        drive_lookup(prot[p]);
        settle("prot final lookup");
        if (!top->a_lu_hit_o) fail("prot: protected tag " + dec(p) + " lost after 100 fills");
        edge("prot final lookup");
    }
}

static void phase_policy_clear() {
    g_phase   = "policy-clear";
    g_unified = false;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    do_reset();

    for (int k = 0; k < 6; k++) {
        Tag     t = make_tag(next_vpn2(), 0x600u + static_cast<uint32_t>(k),
                             0x700u + static_cast<uint32_t>(k), 1, 0);
        Content c = make_content(0x3000u + static_cast<uint32_t>(k), 0, 0, 0, 0, 0, 0, 0);
        clear_inval();
        drive_update(t, c);
        settle("clear fill");
        edge("clear fill");
    }
    if (count_valid() == 0) fail("clear: no entry became valid before the clear");

    top->policy_clear_i = 1;
    clear_update(); clear_lookup(); clear_inval();
    settle("clear pre-edge");
    edge("clear edge");
    check_all_invalid("clear");
    if (top->a_plru != 0) fail("clear: PLRU tree was not cleared: 0x" + hex16(top->a_plru));
    if (!is_onehot16(static_cast<uint16_t>(top->a_replace_mask)))
        fail("clear: replace_mask not one-hot: 0x" + hex16(top->a_replace_mask));

    Tag     tu = make_tag(next_vpn2(), 0x650, 0x750, 1, 0);
    Content cu = make_content(0x3500, 0, 0, 0, 0, 0, 0, 0);
    top->policy_clear_i = 1;
    drive_update(tu, cu);
    settle("clear+update pre-edge");
    edge("clear+update edge");
    check_all_invalid("clear+update");
    drive_lookup(tu);
    settle("clear+update probe");
    if (top->a_lu_hit_o) fail("clear: concurrent update was applied despite clear");
    edge("clear+update probe");
    top->policy_clear_i = 0;

    drive_update(tu, cu);
    settle("post-clear update");
    edge("post-clear update");
    drive_lookup(tu);
    settle("post-clear probe");
    if (!top->a_lu_hit_o) fail("clear: allocation did not resume after clear deasserted");
    edge("post-clear probe");
}

static void phase_inval() {
    g_phase   = "inval-semantics";
    g_unified = true;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    do_reset();

    Tag     h[4];
    Content hc[4];
    int     hs[4];
    for (int k = 0; k < 4; k++) {
        h[k]  = make_tag(next_vpn2(), 0x200u + static_cast<uint32_t>(k / 2),
                         0x300u + static_cast<uint32_t>(k), 1, 0);
        hc[k] = make_content(0x8000u + static_cast<uint32_t>(k), 0, 0,
                             (k == 0 || k == 3) ? 1 : 0, 0, 0, 0, 0);
        hs[k] = write_and_touch(h[k], hc[k]);
    }
    Tag     s1   = make_tag(next_vpn2(), 0x210, 0x402, 1, 1);
    Content s1c  = make_content(0x8100, 0, 0, 0, 0x8200, 0, 0, 0);
    int     s1way = write_and_touch(s1, s1c);

    static uint64_t before[16][2];
    snapshot_all(before);

    // 001: 1S only, PSCID match, exclude global.
    apply_inval_vma(0, 0, 1, 0, 0x200, 0);
    check_content_preserved(before, "VMA(001)");
    {
        uint64_t lo, hi;
        read_q(hs[1], lo, hi);
        if (lo_valid(lo)) fail("VMA(001): non-global PSCID match was not invalidated");
        read_q(hs[0], lo, hi);
        if (!lo_valid(lo)) fail("VMA(001): global entry was wrongly invalidated");
        read_q(s1way, lo, hi);
        if (!lo_valid(lo)) fail("VMA(001): two-stage entry was wrongly invalidated");
    }

    // 100: 2S only, GSCID match, include global.
    snapshot_all(before);
    apply_inval_vma(1, 0, 0, 0, 0, 0x402);
    check_content_preserved(before, "VMA(100)");
    {
        uint64_t lo, hi;
        read_q(s1way, lo, hi);
        if (lo_valid(lo)) fail("VMA(100): matching-GSCID two-stage entry was not invalidated");
    }

    // GVMA 10: 2S, GSCID match.
    Tag     s2   = make_tag(next_vpn2(), 0x220, 0x405, 1, 1);
    Content s2c  = make_content(0x0ABCDE1u, 0, 0, 0, 0x8300, 0, 0, 0);
    int     s2way = write_and_touch(s2, s2c);
    snapshot_all(before);
    apply_inval_gvma(1, 0, 0, 0x405);
    check_content_preserved(before, "GVMA(10)");
    {
        uint64_t lo, hi;
        read_q(s2way, lo, hi);
        if (lo_valid(lo)) fail("GVMA(10): matching-GSCID entry was not invalidated");
    }
}

static void phase_touch_miss() {
    g_phase   = "touch-miss-side-effects";
    g_unified = true;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    do_reset();

    Tag     t  = make_tag(next_vpn2(), 0x500, 0x600, 1, 0);
    Content c  = make_content(0x1234, 0, 0, 0, 0, 0, 0, 0);
    clear_inval();
    drive_update(t, c);
    settle("tm fill");
    edge("tm fill");

    // An update must not move the PLRU tree.
    uint32_t plru0 = top->a_plru;
    Tag      t2    = make_tag(next_vpn2(), 0x501, 0x601, 1, 0);
    Content  c2    = make_content(0x1235, 0, 0, 0, 0, 0, 0, 0);
    drive_update(t2, c2);
    settle("tm update");
    if (top->a_plru != plru0) fail("tm: update changed the PLRU");
    edge("tm update");
    if (top->a_plru != plru0) fail("tm: update changed the PLRU (post-edge)");

    // A lookup miss must not move the PLRU tree.
    uint32_t plru1 = top->a_plru;
    Tag      miss  = make_tag(next_vpn2(), 0x5FF, 0x6FF, 1, 0);
    drive_lookup(miss);
    settle("tm miss");
    if (top->a_lu_hit_o)      fail("tm: unexpected hit on a cold tag");
    if (top->a_plru != plru1) fail("tm: a lookup miss changed the PLRU");
    edge("tm miss");
    if (top->a_plru != plru1) fail("tm: a lookup miss changed the PLRU (post-edge)");

    // An invalidation must not move the PLRU tree.
    uint32_t plru2 = top->a_plru;
    apply_inval_vma(1, 0, 0, 0, 0, t.gscid);
    if (top->a_plru != plru2) fail("tm: an invalidation changed the PLRU");

    // Updates, lookup misses and invalidations leave the PLRU tree untouched
    // (checked above and below), so the second fill re-used the victim way
    // chosen for the first fill and replaced t: the tag that survives in the
    // IOTLB is t2.  t2 is stage-1-only, and the gv=1 VMA invalidation above
    // does not apply to stage-1-only entries, so t2 is still present and
    // hit-able while the evicted t is not.
    drive_lookup(t);
    settle("tm evicted probe");
    if (top->a_lu_hit_o) fail("tm: the tag replaced by the second fill still hits");
    edge("tm evicted probe");

    drive_lookup(t2);
    settle("tm hit");
    if (!top->a_lu_hit_o) fail("tm: the surviving tag t2 no longer hits");
    edge("tm hit");
}

// ============================================================ H1 history probe
//
// Enabled only with --history-probe (elaborated with -GExplicitTouch=1).  The
// default suite above is untouched: nothing here runs without the flag.
//
// Object under test: the real rv_iommu_iotlb with TouchEnable = 1 (i_actual).
// Counterfactual: a small test-side rule model ("shadow") that reuses the RTL
// tree bit order and the victim walk but confines a touch to the partition
// that owns the touched way: a node flips direction only when the ownership
// mask has eligible leaves in BOTH of its subtrees.  The shadow is advanced
// only up to the first victim divergence; afterwards it answers exactly one
// logical-page query and is never fed further fills.

static FILE* g_events      = 0;
static FILE* g_summary     = 0;
static bool  g_shadow_lock = false;   // set at the first victim divergence

static const uint32_t PROBE_V_PSCID = 10;
static const uint32_t PROBE_A_PSCID = 11;
static const uint16_t PROBE_V_OWN   = 0x0003u;  // V partition owns ways 0-1
static const uint16_t PROBE_A_ALLOC = 0xFFFCu;  // formal A allocation mask
static const int      PROBE_X_ID    = 100;      // shadow page id of page x

// -------------------------------------------------------- test-side shadow

struct ShadowEntry {
    bool     valid;
    bool     en1s, en2s, c1_g;
    uint32_t vpn, pscid, gscid;
    int      page_id;
};

struct ShadowState {
    uint32_t    tree;   // bit n = PLRU node n, same order as plru_tree_q
    ShadowEntry e[16];
};

static ShadowState g_shadow;

// eligible[15+i] = leaf i; eligible[n] = eligible[2n+1] | eligible[2n+2].
static void sh_elig(uint16_t mask, bool e[31]) {
    for (int i = 0; i < 16; i++) e[15 + i] = ((mask >> i) & 1u) != 0u;
    for (int i = 14; i >= 0; i--) e[i] = e[2 * i + 1] || e[2 * i + 2];
}

static void sh_reset() {
    g_shadow.tree = 0;
    for (int i = 0; i < 16; i++) {
        g_shadow.e[i].valid   = false;
        g_shadow.e[i].en1s    = false;
        g_shadow.e[i].en2s    = false;
        g_shadow.e[i].c1_g    = false;
        g_shadow.e[i].vpn     = 0;
        g_shadow.e[i].pscid   = 0;
        g_shadow.e[i].gscid   = 0;
        g_shadow.e[i].page_id = -1;
    }
}

// Identical walk to rv_iommu_iotlb: node n = (2^lvl - 1) + (i >> shift).
static int sh_victim(uint16_t mask) {
    bool e[31];
    sh_elig(mask, e);
    for (int i = 0; i < 16; i++) {
        if (!((mask >> i) & 1u)) continue;
        bool en = true;
        for (int lvl = 0; lvl < 4; lvl++) {
            int  idx_base = (1 << lvl) - 1;
            int  shift    = 4 - lvl;
            int  n        = idx_base + (i >> shift);
            bool ni       = ((i >> (shift - 1)) & 1) != 0;
            bool cr       = e[2 * n + 2] &&
                            (!e[2 * n + 1] || (((g_shadow.tree >> n) & 1u) != 0u));
            en = en && (ni ? cr : !cr);
        }
        if (en) return i;
    }
    return -1;
}

// Partition-local touch: flip a node only when the ownership mask spans both
// of that node's subtrees; otherwise the previous direction is kept.
static void sh_touch(int leaf, uint16_t own) {
    bool o[31];
    sh_elig(own, o);
    for (int lvl = 0; lvl < 4; lvl++) {
        int idx_base = (1 << lvl) - 1;
        int shift    = 4 - lvl;
        int n        = idx_base + (leaf >> shift);
        if (!(o[2 * n + 1] && o[2 * n + 2])) continue;
        int bit = (~((leaf >> (shift - 1)) & 1)) & 1;
        g_shadow.tree = (g_shadow.tree & ~(1u << n)) | (static_cast<uint32_t>(bit) << n);
    }
}

static void sh_fill(const Tag& t, const Content& c, uint16_t mask, int page_id) {
    if (g_shadow_lock) fail("shadow: fill after the first victim divergence");
    int v = sh_victim(mask);
    if (v < 0) { fail("shadow: no victim for mask 0x" + hex16(mask)); return; }
    g_shadow.e[v].valid   = true;
    g_shadow.e[v].en1s    = (t.en1s != 0);
    g_shadow.e[v].en2s    = (t.en2s != 0);
    g_shadow.e[v].c1_g    = (c.c1_g != 0);
    g_shadow.e[v].vpn     = t.vpn;
    g_shadow.e[v].pscid   = t.pscid;
    g_shadow.e[v].gscid   = t.gscid;
    g_shadow.e[v].page_id = page_id;
}

// Every frozen page is a 4k, stage-1-only, non-global page, so the RTL
// 1G/2M sub-matching collapses to a full-VPN compare here.
static int sh_lookup(const Tag& t) {
    for (int i = 0; i < 16; i++) {
        const ShadowEntry& s = g_shadow.e[i];
        if (!s.valid) continue;
        if (s.en1s != (t.en1s != 0)) continue;
        if (s.en2s != (t.en2s != 0)) continue;
        if (!((!t.en1s) || (s.pscid == t.pscid) || s.c1_g)) continue;
        if (!((!t.en2s) || (s.gscid == t.gscid))) continue;
        if (s.vpn != t.vpn) continue;
        return s.page_id;
    }
    return -1;
}

// ------------------------------------------------------- probe JSONL events

static void json_str(FILE* f, const std::string& s) {
    std::fputc('"', f);
    for (std::size_t i = 0; i < s.size(); i++) {
        const char ch = s[i];
        if (ch == '"' || ch == '\\') std::fputc('\\', f);
        std::fputc(ch, f);
    }
    std::fputc('"', f);
}

static void json_opt(FILE* f, long long v) {
    if (v < 0) std::fputs("null", f);
    else       std::fprintf(f, "%lld", v);
}

static void json_bool(FILE* f, bool b) { std::fputs(b ? "true" : "false", f); }

struct ProbeEvent {
    std::string cse;
    int         edge;
    std::string phase;
    long long   lookup_vpn, lookup_pscid, lookup, touch, hit;
    long long   fill_vpn, fill_pscid, alloc_mask;
    long long   a_tree_before, a_tree_after;
    long long   s_tree_before, s_tree_after;
    long long   a_victim, s_victim;
    bool        first_divergence;
};

static ProbeEvent mk_event(const std::string& cse, const std::string& phase) {
    ProbeEvent e;
    e.cse   = cse;
    e.edge  = g_edge_no;
    e.phase = phase;
    e.lookup_vpn = e.lookup_pscid = e.lookup = e.touch = e.hit = -1;
    e.fill_vpn = e.fill_pscid = e.alloc_mask = -1;
    e.a_tree_before = e.a_tree_after = -1;
    e.s_tree_before = e.s_tree_after = -1;
    e.a_victim = e.s_victim = -1;
    e.first_divergence = false;
    return e;
}

static void emit(const ProbeEvent& e) {
    if (!g_events) return;
    std::fputs("{", g_events);
    std::fputs("\"case\": ", g_events);              json_str(g_events, e.cse);
    std::fprintf(g_events, ", \"edge\": %d", e.edge);
    std::fputs(", \"phase\": ", g_events);           json_str(g_events, e.phase);
    std::fputs(", \"lookupVPN\": ", g_events);       json_opt(g_events, e.lookup_vpn);
    std::fputs(", \"lookupPSCID\": ", g_events);     json_opt(g_events, e.lookup_pscid);
    std::fputs(", \"lookup\": ", g_events);          json_opt(g_events, e.lookup);
    std::fputs(", \"touch\": ", g_events);           json_opt(g_events, e.touch);
    std::fputs(", \"hit\": ", g_events);             json_opt(g_events, e.hit);
    std::fputs(", \"fillVPN\": ", g_events);         json_opt(g_events, e.fill_vpn);
    std::fputs(", \"fillPSCID\": ", g_events);       json_opt(g_events, e.fill_pscid);
    std::fputs(", \"allocation_mask\": ", g_events); json_opt(g_events, e.alloc_mask);
    std::fputs(", \"actual_tree_before\": ", g_events);
    json_opt(g_events, e.a_tree_before);
    std::fputs(", \"actual_tree_after\": ", g_events);
    json_opt(g_events, e.a_tree_after);
    std::fputs(", \"shadow_tree_before\": ", g_events);
    json_opt(g_events, e.s_tree_before);
    std::fputs(", \"shadow_tree_after\": ", g_events);
    json_opt(g_events, e.s_tree_after);
    std::fputs(", \"actual_victim\": ", g_events);
    json_opt(g_events, e.a_victim);
    std::fputs(", \"shadow_victim\": ", g_events);
    json_opt(g_events, e.s_victim);
    std::fputs(", \"first_divergence\": ", g_events);
    json_bool(g_events, e.first_divergence);
    std::fputs("}\n", g_events);
}

// -------------------- expected hit content (mirrors a_content_flat order)

static void flat_set_bit(uint64_t& lo, uint64_t& hi, int b, int v) {
    if (!(v & 1)) return;
    if (b < 64) lo |= (1ULL << b);
    else        hi |= (1ULL << (b - 64));
}

static void flat_set_field(uint64_t& lo, uint64_t& hi, int msb, int lsb, uint64_t v) {
    for (int b = lsb; b <= msb; b++)
        flat_set_bit(lo, hi, b, static_cast<int>((v >> (b - lsb)) & 1u));
}

// Same packing as the a_content_flat assign in composition_iotlb_top.sv:
//   {c1.is_1G, c1.is_2M, c1.ppn, c1.g, c1.u, c1.x, c1.w, c1.r, c1.v,
//    c2.is_1G, c2.is_2M, c2.ppn, c2.g, c2.u, c2.x, c2.w, c2.r, c2.v},
// MSB = bit 103.
static void expected_content_flat(const Content& c, uint64_t& lo, uint64_t& hi) {
    lo = 0;
    hi = 0;
    flat_set_field(lo, hi, 103, 102,
        (static_cast<uint64_t>(c.c1_1g & 1) << 1) | static_cast<uint64_t>(c.c1_2m & 1));
    flat_set_field(lo, hi, 101, 58, c.c1_ppn);
    flat_set_field(lo, hi, 57, 52,
        (static_cast<uint64_t>(c.c1_g & 1) << 5) | (static_cast<uint64_t>(c.c1_u & 1) << 4) |
        (static_cast<uint64_t>(c.c1_x & 1) << 3) | (static_cast<uint64_t>(c.c1_w & 1) << 2) |
        (static_cast<uint64_t>(c.c1_r & 1) << 1) |  static_cast<uint64_t>(c.c1_v & 1));
    flat_set_field(lo, hi, 51, 50,
        (static_cast<uint64_t>(c.c2_1g & 1) << 1) | static_cast<uint64_t>(c.c2_2m & 1));
    flat_set_field(lo, hi, 49, 6, c.c2_ppn);
    flat_set_field(lo, hi, 5, 0,
        (static_cast<uint64_t>(c.c2_g & 1) << 5) | (static_cast<uint64_t>(c.c2_u & 1) << 4) |
        (static_cast<uint64_t>(c.c2_x & 1) << 3) | (static_cast<uint64_t>(c.c2_w & 1) << 2) |
        (static_cast<uint64_t>(c.c2_r & 1) << 1) |  static_cast<uint64_t>(c.c2_v & 1));
}

static void check_hit_content(const std::string& cse, const Content& c,
                              const std::string& label) {
    uint64_t elo, ehi;
    expected_content_flat(c, elo, ehi);
    if (top->a_content_lo != elo || top->a_content_hi != ehi) {
        fail(cse + ": " + label + " hit content mismatch: actual hi=" +
             hex64(top->a_content_hi) + " lo=" + hex64(top->a_content_lo) +
             " expected hi=" + hex64(ehi) + " lo=" + hex64(elo));
    }
}

// ----------------------------------------------------------- probe stimulus

static Tag     g_probe_page_tag[16];
static Content g_probe_page_content[16];
static Tag     g_probe_x_tag;
static Content g_probe_x_content;

struct ProbeCaseResult {
    std::string cse;
    int         a_victim;
    int         s_victim;
    int         succ_actual;   // 1 hit / 0 miss / -1 not queried
    int         succ_shadow;   // 1 hit / 0 miss / -1 not queried
    std::string succ_page;
    bool        first_div;
};

static ProbeCaseResult g_probe_cases[4];
static int             g_probe_ncases = 0;

static int probe_actual_victim(const std::string& phase) {
    const uint16_t m = static_cast<uint16_t>(top->a_replace_mask);
    if (!is_onehot16(m)) fail(phase + ": replace_mask not one-hot: 0x" + hex16(m));
    const int v = lowest_bit16(m);
    if (v < 0) fail(phase + ": no eligible way inside the mask");
    return v;
}

// The only way any entry is primed: the normal update interface plus a
// single-way mask.  No hierarchical force is used anywhere in this file.
static void probe_build_prefix(const std::string& cse) {
    g_phase       = cse + ":prefix";
    g_unified     = false;
    g_shadow_lock = false;
    g_edge_no     = 0;   // case-local posedge counter

    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    do_reset();
    sh_reset();

    for (int k = 0; k < 16; k++) {
        g_probe_page_tag[k] = make_tag(mk_vpn(0x100u + static_cast<uint32_t>(k), 0, 0),
                                       (k < 2) ? PROBE_V_PSCID : PROBE_A_PSCID,
                                       0x300u + static_cast<uint32_t>(k), 1, 0);
        g_probe_page_content[k] = make_content(0x1000u + static_cast<uint32_t>(k), 0, 0, 0,
                                               0x2000u + static_cast<uint32_t>(k), 0, 0, 0);
    }

    for (int k = 0; k < 16; k++) {
        const uint16_t mask = static_cast<uint16_t>(1u << k);
        top->policy_mask_i = mask;
        clear_inval();
        drive_update(g_probe_page_tag[k], g_probe_page_content[k]);
        settle("prefix fill settle");

        const int av = probe_actual_victim(cse + ": priming slot " + dec(k));
        const int sv = sh_victim(mask);
        if (av != k) fail(cse + ": priming selected way " + dec(av) + " instead of way " + dec(k));
        if (sv != k) fail(cse + ": shadow priming selected way " + dec(sv));
        {
            uint64_t nlo, nhi;
            read_n(k, nlo, nhi);
            check_entry_bits(nlo, nhi, g_probe_page_tag[k], g_probe_page_content[k],
                             cse + ": prefix entries_next[" + dec(k) + "]");
        }

        ProbeEvent ev = mk_event(cse, "prime-slot-" + dec(k));
        ev.fill_vpn     = static_cast<long long>(g_probe_page_tag[k].vpn);
        ev.fill_pscid   = static_cast<long long>(g_probe_page_tag[k].pscid);
        ev.alloc_mask   = static_cast<long long>(mask);
        ev.a_tree_before = top->a_plru;
        ev.s_tree_before = g_shadow.tree;
        ev.a_victim      = av;
        ev.s_victim      = sv;
        edge("prefix fill edge");
        sh_fill(g_probe_page_tag[k], g_probe_page_content[k], mask, k);
        {
            uint64_t qlo, qhi;
            read_q(k, qlo, qhi);
            check_entry_bits(qlo, qhi, g_probe_page_tag[k], g_probe_page_content[k],
                             cse + ": prefix committed way " + dec(k));
        }
        ev.a_tree_after = top->a_plru;
        ev.s_tree_after = g_shadow.tree;
        emit(ev);
    }

    // Per-way identity and per-page hit content, with touch_i held at 0.
    for (int k = 0; k < 16; k++) {
        drive_lookup(g_probe_page_tag[k]);
        settle("prefix verify settle");
        if (!top->a_lu_hit_o) fail(cse + ": primed page " + dec(k) + " does not hit");
        check_hit_content(cse, g_probe_page_content[k], "primed page " + dec(k));
        const int sp = sh_lookup(g_probe_page_tag[k]);
        if (sp != k) fail(cse + ": shadow resolves primed page " + dec(k) + " to " + dec(sp));
        {
            uint64_t qlo, qhi;
            read_q(k, qlo, qhi);
            check_entry_bits(qlo, qhi, g_probe_page_tag[k], g_probe_page_content[k],
                             cse + ": prefix resident way " + dec(k));
        }

        ProbeEvent ev = mk_event(cse, "prime-verify-" + dec(k));
        ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[k].vpn);
        ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[k].pscid);
        ev.lookup       = 1;
        ev.touch        = 0;
        ev.hit          = top->a_lu_hit_o ? 1 : 0;
        ev.a_tree_before = top->a_plru;
        ev.s_tree_before = g_shadow.tree;
        edge("prefix verify edge");
        ev.a_tree_after = top->a_plru;
        ev.s_tree_after = g_shadow.tree;
        emit(ev);
    }

    if (top->a_plru != 0)
        fail(cse + ": prefix left a non-zero actual tree: 0x" + hex16(top->a_plru));
    if (g_shadow.tree != 0) fail(cse + ": prefix left a non-zero shadow tree");

    ProbeEvent ready = mk_event(cse, "prefix-ready");
    ready.a_tree_before = ready.a_tree_after = top->a_plru;
    ready.s_tree_before = ready.s_tree_after = g_shadow.tree;
    emit(ready);
}

// Shared prefix of H1-positive / H1-reverse-successor: x miss, V0 valid touch,
// one idle edge, then the A fill of page x under mask 0xfffc.
static void probe_to_first_divergence(const std::string& cse, ProbeCaseResult& r) {
    r.cse = cse;
    probe_build_prefix(cse);

    g_phase = cse + ":A-x-miss";
    drive_lookup(g_probe_x_tag);
    settle("x miss settle");
    if (top->a_lu_hit_o) fail(cse + ": new A page x unexpectedly hits");
    {
        ProbeEvent ev = mk_event(cse, "probe-x-lookup");
        ev.lookup_vpn   = static_cast<long long>(g_probe_x_tag.vpn);
        ev.lookup_pscid = static_cast<long long>(g_probe_x_tag.pscid);
        ev.lookup       = 1;
        ev.touch        = 0;
        ev.hit          = 0;
        ev.a_tree_before = top->a_plru;
        ev.s_tree_before = g_shadow.tree;
        edge("x miss edge");
        ev.a_tree_after = top->a_plru;
        ev.s_tree_after = g_shadow.tree;
        emit(ev);
    }
    if (top->a_plru != 0) fail(cse + ": an x lookup miss moved the actual tree");
    if (g_shadow.tree != 0) fail(cse + ": an x lookup miss moved the shadow tree");

    g_phase = cse + ":V0-touch";
    drive_lookup(g_probe_page_tag[0]);
    top->touch_i = 1;
    settle("V0 touch settle");
    if (!top->a_lu_hit_o) fail(cse + ": V way-0 page does not hit");
    check_hit_content(cse, g_probe_page_content[0], "V way-0");
    {
        ProbeEvent ev = mk_event(cse, "probe-V0-touch");
        ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[0].vpn);
        ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[0].pscid);
        ev.lookup       = 1;
        ev.touch        = 1;
        ev.hit          = 1;
        ev.a_tree_before = top->a_plru;
        ev.s_tree_before = g_shadow.tree;
        edge("V0 touch edge");
        ev.a_tree_after = top->a_plru;
        sh_touch(0, PROBE_V_OWN);
        ev.s_tree_after = g_shadow.tree;
        emit(ev);
    }
    // Real RTL: nodes 0, 1, 3 and 7 (root, 0-7, 0-3, 0-1) turn right.
    if (top->a_plru != 0x008Bu)
        fail(cse + ": post-touch actual tree is 0x" + hex16(top->a_plru) + ", expected 0x008b");
    // Shadow: only the 0-1 node flips (0x0003 owns both of its subtrees).
    if (g_shadow.tree != 0x0080u)
        fail(cse + ": post-touch shadow tree is 0x" + hex16(g_shadow.tree) + ", expected 0x0080");

    g_phase = cse + ":idle";
    clear_lookup();          // also forces touch_i = 0
    settle("idle settle");
    edge("idle edge");       // at least one full edge between the touch and the fill

    g_phase = cse + ":A-fill-x";
    top->policy_mask_i = PROBE_A_ALLOC;
    clear_inval();
    drive_update(g_probe_x_tag, g_probe_x_content);
    settle("fill x settle");
    const int av = probe_actual_victim(cse + ": fill x");
    const int sv = sh_victim(PROBE_A_ALLOC);
    ProbeEvent ev = mk_event(cse, "probe-fill-x");
    ev.fill_vpn      = static_cast<long long>(g_probe_x_tag.vpn);
    ev.fill_pscid    = static_cast<long long>(g_probe_x_tag.pscid);
    ev.alloc_mask    = static_cast<long long>(PROBE_A_ALLOC);
    ev.a_tree_before = top->a_plru;
    ev.s_tree_before = g_shadow.tree;
    ev.a_victim      = av;
    ev.s_victim      = sv;
    if (av != 8) fail(cse + ": actual victim is " + dec(av) + ", expected 8");
    if (sv != 2) fail(cse + ": shadow victim is " + dec(sv) + ", expected 2");
    if (av == sv) fail(cse + ": expected a first victim divergence at this fill");
    ev.first_divergence = (av != sv);
    {
        uint64_t nlo, nhi;
        read_n(av, nlo, nhi);
        check_entry_bits(nlo, nhi, g_probe_x_tag, g_probe_x_content, cse + ": x entries_next");
    }
    edge("fill x edge");
    {
        uint64_t qlo, qhi;
        read_q(av, qlo, qhi);
        check_entry_bits(qlo, qhi, g_probe_x_tag, g_probe_x_content, cse + ": x committed");
    }
    sh_fill(g_probe_x_tag, g_probe_x_content, PROBE_A_ALLOC, PROBE_X_ID);
    ev.a_tree_after = top->a_plru;
    ev.s_tree_after = g_shadow.tree;
    emit(ev);

    g_shadow_lock = true;   // the shadow stops tracking at the first divergence
    r.a_victim  = av;
    r.s_victim  = sv;
    r.first_div = true;
}

static void probe_case_positive() {
    const std::string cse = "H1-positive";
    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
    r.succ_actual = -1;
    r.succ_shadow = -1;
    probe_to_first_divergence(cse, r);

    g_phase = cse + ":successor-y";
    drive_lookup(g_probe_page_tag[8]);
    settle("y lookup settle");
    const int ah = top->a_lu_hit_o ? 1 : 0;
    const int sp = sh_lookup(g_probe_page_tag[8]);
    const int sh = (sp == 8) ? 1 : 0;
    if (ah != 0) fail(cse + ": successor page y still hits in the actual RTL");
    if (sh != 1) fail(cse + ": the shadow no longer resolves page y (page_id " + dec(sp) + ")");
    ProbeEvent ev = mk_event(cse, "successor-y");
    ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[8].vpn);
    ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[8].pscid);
    ev.lookup       = 1;
    ev.touch        = 0;
    ev.hit          = ah;
    ev.a_tree_before = top->a_plru;
    ev.s_tree_before = g_shadow.tree;
    edge("y lookup edge");
    ev.a_tree_after = top->a_plru;
    ev.s_tree_after = g_shadow.tree;
    emit(ev);

    r.succ_actual = ah;
    r.succ_shadow = sh;
    r.succ_page   = "way8-page-y";
}

static void probe_case_no_touch() {
    const std::string cse = "H1-no-touch";
    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
    r.succ_actual = -1;
    r.succ_shadow = -1;
    r.cse = cse;
    probe_build_prefix(cse);

    g_phase = cse + ":A-x-miss";
    drive_lookup(g_probe_x_tag);
    settle("x miss settle");
    if (top->a_lu_hit_o) fail(cse + ": new A page x unexpectedly hits");
    edge("x miss edge");

    g_phase = cse + ":V0-no-touch";
    drive_lookup(g_probe_page_tag[0]);   // drive_lookup() forces touch_i = 0
    settle("V0 no-touch settle");
    if (!top->a_lu_hit_o) fail(cse + ": V way-0 page does not hit");
    check_hit_content(cse, g_probe_page_content[0], "V way-0");
    {
        ProbeEvent ev = mk_event(cse, "probe-V0-no-touch");
        ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[0].vpn);
        ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[0].pscid);
        ev.lookup       = 1;
        ev.touch        = 0;
        ev.hit          = 1;
        ev.a_tree_before = top->a_plru;
        ev.s_tree_before = g_shadow.tree;
        edge("V0 no-touch edge");
        ev.a_tree_after = top->a_plru;
        ev.s_tree_after = g_shadow.tree;
        emit(ev);
    }
    if (top->a_plru != 0) fail(cse + ": touch_i=0 did not gate the actual touch");
    if (g_shadow.tree != 0) fail(cse + ": touch_i=0 moved the shadow tree");

    g_phase = cse + ":idle";
    clear_lookup();          // also forces touch_i = 0
    settle("idle settle");
    edge("idle edge");

    g_phase = cse + ":A-fill-x";
    top->policy_mask_i = PROBE_A_ALLOC;
    clear_inval();
    drive_update(g_probe_x_tag, g_probe_x_content);
    settle("fill x settle");
    const int av = probe_actual_victim(cse + ": fill x");
    const int sv = sh_victim(PROBE_A_ALLOC);
    if (av != 2) fail(cse + ": actual victim is " + dec(av) + ", expected 2");
    if (sv != 2) fail(cse + ": shadow victim is " + dec(sv) + ", expected 2");
    ProbeEvent ev = mk_event(cse, "probe-fill-x");
    ev.fill_vpn      = static_cast<long long>(g_probe_x_tag.vpn);
    ev.fill_pscid    = static_cast<long long>(g_probe_x_tag.pscid);
    ev.alloc_mask    = static_cast<long long>(PROBE_A_ALLOC);
    ev.a_tree_before = top->a_plru;
    ev.s_tree_before = g_shadow.tree;
    ev.a_victim      = av;
    ev.s_victim      = sv;
    ev.first_divergence = false;
    {
        uint64_t nlo, nhi;
        read_n(av, nlo, nhi);
        check_entry_bits(nlo, nhi, g_probe_x_tag, g_probe_x_content, cse + ": x entries_next");
    }
    edge("fill x edge");
    {
        uint64_t qlo, qhi;
        read_q(av, qlo, qhi);
        check_entry_bits(qlo, qhi, g_probe_x_tag, g_probe_x_content, cse + ": x committed");
    }
    sh_fill(g_probe_x_tag, g_probe_x_content, PROBE_A_ALLOC, PROBE_X_ID);
    ev.a_tree_after = top->a_plru;
    ev.s_tree_after = g_shadow.tree;
    emit(ev);

    r.a_victim  = av;
    r.s_victim  = sv;
    r.first_div = false;
}

static void probe_case_same_edge() {
    const std::string cse = "H1-same-edge";
    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
    r.succ_actual = -1;
    r.succ_shadow = -1;
    r.cse = cse;
    probe_build_prefix(cse);

    g_phase = cse + ":same-edge";
    // V way-0 hit with an explicit valid touch and the A fill of x, one edge.
    clear_inval();
    top->lookup_i        = 1;
    top->lu_vpn_i        = g_probe_page_tag[0].vpn;
    top->lu_pscid_i      = g_probe_page_tag[0].pscid;
    top->lu_gscid_i      = g_probe_page_tag[0].gscid;
    top->en_1S_i         = 1;   // shared by the lookup and the update latch
    top->en_2S_i         = 0;   // both pages are stage-1-only
    top->touch_i         = 1;
    top->policy_mask_i   = PROBE_A_ALLOC;
    top->update_update_i = 1;
    top->update_vpn_i    = g_probe_x_tag.vpn;
    top->update_pscid_i  = g_probe_x_tag.pscid;
    top->update_gscid_i  = g_probe_x_tag.gscid;
    top->up_c1s_is_1G_i  = g_probe_x_content.c1_1g;
    top->up_c1s_is_2M_i  = g_probe_x_content.c1_2m;
    top->up_c1s_ppn_i    = g_probe_x_content.c1_ppn;
    top->up_c1s_g_i      = g_probe_x_content.c1_g;
    top->up_c1s_u_i      = g_probe_x_content.c1_u;
    top->up_c1s_x_i      = g_probe_x_content.c1_x;
    top->up_c1s_w_i      = g_probe_x_content.c1_w;
    top->up_c1s_r_i      = g_probe_x_content.c1_r;
    top->up_c1s_v_i      = g_probe_x_content.c1_v;
    top->up_c2s_is_1G_i  = g_probe_x_content.c2_1g;
    top->up_c2s_is_2M_i  = g_probe_x_content.c2_2m;
    top->up_c2s_ppn_i    = g_probe_x_content.c2_ppn;
    top->up_c2s_g_i      = g_probe_x_content.c2_g;
    top->up_c2s_u_i      = g_probe_x_content.c2_u;
    top->up_c2s_x_i      = g_probe_x_content.c2_x;
    top->up_c2s_w_i      = g_probe_x_content.c2_w;
    top->up_c2s_r_i      = g_probe_x_content.c2_r;
    top->up_c2s_v_i      = g_probe_x_content.c2_v;
    settle("same-edge settle");
    if (!top->a_lu_hit_o) fail(cse + ": V way-0 page does not hit");
    check_hit_content(cse, g_probe_page_content[0], "V way-0");
    const int av = probe_actual_victim(cse + ": same-edge fill");
    const int sv = sh_victim(PROBE_A_ALLOC);
    if (av != 2) fail(cse + ": same-edge actual victim is " + dec(av) + ", expected 2 (old tree)");
    if (sv != 2) fail(cse + ": same-edge shadow victim is " + dec(sv) + ", expected 2 (old tree)");
    ProbeEvent ev = mk_event(cse, "probe-same-edge");
    ev.lookup_vpn    = static_cast<long long>(g_probe_page_tag[0].vpn);
    ev.lookup_pscid  = static_cast<long long>(g_probe_page_tag[0].pscid);
    ev.lookup        = 1;
    ev.touch         = 1;
    ev.hit           = 1;
    ev.fill_vpn      = static_cast<long long>(g_probe_x_tag.vpn);
    ev.fill_pscid    = static_cast<long long>(g_probe_x_tag.pscid);
    ev.alloc_mask    = static_cast<long long>(PROBE_A_ALLOC);
    ev.a_tree_before = top->a_plru;
    ev.s_tree_before = g_shadow.tree;
    ev.a_victim      = av;
    ev.s_victim      = sv;
    ev.first_divergence = false;
    {
        uint64_t nlo, nhi;
        read_n(av, nlo, nhi);
        check_entry_bits(nlo, nhi, g_probe_x_tag, g_probe_x_content, cse + ": x entries_next");
    }
    edge("same-edge edge");
    {
        uint64_t qlo, qhi;
        read_q(av, qlo, qhi);
        check_entry_bits(qlo, qhi, g_probe_x_tag, g_probe_x_content, cse + ": x committed");
    }
    sh_fill(g_probe_x_tag, g_probe_x_content, PROBE_A_ALLOC, PROBE_X_ID);
    sh_touch(0, PROBE_V_OWN);
    ev.a_tree_after = top->a_plru;
    ev.s_tree_after = g_shadow.tree;
    emit(ev);
    // This fill used the old tree; the post-edge trees are recorded only.
    if (top->a_plru != 0x008Bu)
        fail(cse + ": post-edge actual tree is 0x" + hex16(top->a_plru) + ", expected 0x008b");
    if (g_shadow.tree != 0x0080u)
        fail(cse + ": post-edge shadow tree is 0x" + hex16(g_shadow.tree) + ", expected 0x0080");

    r.a_victim  = av;
    r.s_victim  = sv;
    r.first_div = false;
}

static void probe_case_reverse_successor() {
    const std::string cse = "H1-reverse-successor";
    ProbeCaseResult& r = g_probe_cases[g_probe_ncases++];
    r.succ_actual = -1;
    r.succ_shadow = -1;
    probe_to_first_divergence(cse, r);

    g_phase = cse + ":successor-z";
    drive_lookup(g_probe_page_tag[2]);
    settle("z lookup settle");
    const int ah = top->a_lu_hit_o ? 1 : 0;
    const int sp = sh_lookup(g_probe_page_tag[2]);
    const int sh = (sp == 2) ? 1 : 0;
    if (ah != 1) fail(cse + ": successor page z does not hit in the actual RTL");
    check_hit_content(cse, g_probe_page_content[2], "successor page z");
    if (sh != 0) fail(cse + ": the shadow still resolves page z (page_id " + dec(sp) + ")");
    ProbeEvent ev = mk_event(cse, "successor-z");
    ev.lookup_vpn   = static_cast<long long>(g_probe_page_tag[2].vpn);
    ev.lookup_pscid = static_cast<long long>(g_probe_page_tag[2].pscid);
    ev.lookup       = 1;
    ev.touch        = 0;
    ev.hit          = ah;
    ev.a_tree_before = top->a_plru;
    ev.s_tree_before = g_shadow.tree;
    edge("z lookup edge");
    ev.a_tree_after = top->a_plru;
    ev.s_tree_after = g_shadow.tree;
    emit(ev);

    r.succ_actual = ah;
    r.succ_shadow = sh;
    r.succ_page   = "way2-page-z";
}

// ------------------------------------------------------------ probe summary

static void probe_write_summary() {
    if (!g_summary) return;
    std::fputs("{\n", g_summary);
    std::fputs("  \"suite\": \"b1-causal-20261006/iotlb-probe\",\n", g_summary);
    std::fputs("  \"actual_RTL_checked\": true,\n", g_summary);
    std::fputs("  \"counterfactual_model_only\": true,\n", g_summary);
    std::fputs("  \"synthetic_priming\": true,\n", g_summary);
    std::fputs("  \"platform_reachability_not_checked\": true,\n", g_summary);
    std::fputs("  \"board_attribution_not_checked\": true,\n", g_summary);
    std::fputs("  \"model_stops_at_first_divergence\": true,\n", g_summary);
    std::fputs("  \"cases\": [\n", g_summary);
    for (int i = 0; i < g_probe_ncases; i++) {
        const ProbeCaseResult& r = g_probe_cases[i];
        std::fputs("    {\"case\": ", g_summary);
        json_str(g_summary, r.cse);
        std::fprintf(g_summary,
                     ", \"actual_victim\": %d, \"shadow_victim\": %d, \"first_divergence\": %s",
                     r.a_victim, r.s_victim, r.first_div ? "true" : "false");
        std::fputs(", \"successor_page\": ", g_summary);
        json_str(g_summary, r.succ_page);
        std::fputs(", \"successor_queried\": ", g_summary);
        json_bool(g_summary, r.succ_actual >= 0);
        std::fputs(", \"successor_actual_hit\": ", g_summary);
        json_bool(g_summary, r.succ_actual == 1);
        std::fputs(", \"successor_shadow_hit\": ", g_summary);
        json_bool(g_summary, r.succ_shadow == 1);
        std::fputs("}", g_summary);
        if (i + 1 < g_probe_ncases) std::fputc(',', g_summary);
        std::fputc('\n', g_summary);
    }
    std::fputs("  ],\n", g_summary);
    std::fprintf(g_summary, "  \"checked_evaluations\": %ld,\n", g_checks);
    std::fprintf(g_summary, "  \"failures\": %d,\n", g_fail);
    std::fprintf(g_summary, "  \"result\": \"%s\"\n", (g_fail == 0) ? "PASS" : "FAIL");
    std::fputs("}\n", g_summary);
}

static int probe_main(const std::string& out_dir) {
    const std::string dir = out_dir.empty()
                                ? std::string("out/b1-causal-20261006/iotlb-probe")
                                : out_dir;
    const std::string ev_path  = dir + "/events.jsonl";
    const std::string sum_path = dir + "/summary.json";
    g_events  = std::fopen(ev_path.c_str(), "w");
    g_summary = std::fopen(sum_path.c_str(), "w");
    if (g_events == 0 || g_summary == 0) {
        std::printf("FAIL [history-probe]: cannot create %s or %s\n",
                    ev_path.c_str(), sum_path.c_str());
        return 1;
    }

    g_probe_x_tag     = make_tag(mk_vpn(0x80u, 0, 0), PROBE_A_PSCID, 0x500u, 1, 0);
    g_probe_x_content = make_content(0x3000u, 0, 0, 0, 0x4000u, 0, 0, 0);

    probe_case_positive();
    probe_case_no_touch();
    probe_case_same_edge();
    probe_case_reverse_successor();

    probe_write_summary();
    std::fclose(g_events);
    g_events = 0;
    std::fclose(g_summary);
    g_summary = 0;

    if (g_fail == 0) {
        std::printf("PASS [history-probe]: %ld checked evaluations, 0 failures\n", g_checks);
        return 0;
    }
    std::printf("FAIL [history-probe]: %d failures across %ld checked evaluations\n",
                g_fail, g_checks);
    return 1;
}

// --------------------------------------------------------------------- main

int main(int argc, char** argv) {
    std::string probe_out;
    for (int i = 1; i < argc; i++) {
        const std::string arg = (argv[i] != 0) ? argv[i] : "";
        if (arg == "--history-probe")                g_probe = true;
        else if (arg.rfind("--probe-out=", 0) == 0)  probe_out = arg.substr(12);
    }

    Verilated::commandArgs(argc, argv);
    top = new Vcomposition_iotlb_top;

    top->clk_i          = 0;
    top->rst_ni         = 0;
    top->tap_idx        = 0;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    top->touch_i        = 0;   // the probe must never see a stale touch
    clear_inval();
    clear_update();
    clear_lookup();

    g_phase   = "reset";
    g_unified = !g_probe;      // the probe never compares against base/reference
    for (int i = 0; i < 4; i++) {
        top->clk_i = 1; eval_checked("reset");
        top->clk_i = 0; eval_checked("reset");
    }
    top->rst_ni = 1;
    settle("post-reset");

    if (g_probe) return probe_main(probe_out);

    if (!top->a_supported) fail("actual instance does not report NumIotlbEntries == 16");
    if (!top->b_supported) fail("base instance does not report NumIotlbEntries == 16");
    check_all_invalid("reset");
    if (top->a_plru != 0) fail("reset: PLRU tree not cleared");

    phase_full_mask_fuzz();
    phase_mask_subsets();
    phase_protected_retention();
    phase_policy_clear();
    phase_inval();
    phase_touch_miss();

    if (g_fail == 0) {
        std::printf("PASS: %ld checked evaluations, 0 failures\n", g_checks);
        delete top;
        return 0;
    }
    std::printf("FAIL: %d failures across %ld checked evaluations\n", g_fail, g_checks);
    delete top;
    return 1;
}
