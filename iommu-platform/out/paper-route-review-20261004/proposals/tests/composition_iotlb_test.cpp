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
        top->clk_i = 1; eval_checked("reset");
        top->clk_i = 0; eval_checked("reset");
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

    // The filled tag must still hit and must match the reference/base.
    drive_lookup(t);
    settle("tm hit");
    if (!top->a_lu_hit_o) fail("tm: the filled tag no longer hits");
    edge("tm hit");
}

// --------------------------------------------------------------------- main

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    top = new Vcomposition_iotlb_top;

    top->clk_i          = 0;
    top->rst_ni         = 0;
    top->tap_idx        = 0;
    top->policy_clear_i = 0;
    top->policy_mask_i  = 0xFFFF;
    clear_inval();
    clear_update();
    clear_lookup();

    g_phase   = "reset";
    g_unified = true;
    for (int i = 0; i < 4; i++) {
        top->clk_i = 1; eval_checked("reset");
        top->clk_i = 0; eval_checked("reset");
    }
    top->rst_ni = 1;
    settle("post-reset");

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
