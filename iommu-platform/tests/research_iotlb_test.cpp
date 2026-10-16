// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded verification harness for the passive research taps of rv_iommu_iotlb.
//
// research_iotlb_top.sv instantiates the current RTL and the pre-change
// reference RTL side by side.  This testbench drives identical stimulus into
// both and compares the functional outputs lu_hit_o and lu_content_o on *every*
// evaluation.  In addition it checks the passive research_t tap view
// (entries / entries_next / plru / replace_mask / update / invalidate /
// supported) for the properties the taps must expose.
//
// This proves functional equivalence of lu_hit_o/lu_content_o against the prior
// revision for the exercised sequences only.  It says nothing about FPGA
// physical timing.

#include "Vresearch_iotlb_top.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>

static Vresearch_iotlb_top* top = 0;

static int         g_fail   = 0;
static long        g_checks = 0;
static std::string g_phase  = "boot";

// ------------------------------------------------------------------ helpers

static void fail(const std::string& msg) {
    ++g_fail;
    std::printf("FAIL [%s] %s\n", g_phase.c_str(), msg.c_str());
    if (g_fail > 50) {
        std::printf("FAIL [%s] more than 50 failures, aborting\n", g_phase.c_str());
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

// Every evaluation point compares the two functional output views.
static void eval_checked(const char* where) {
    top->eval();
    ++g_checks;
    if (top->a_lu_hit_o != top->r_lu_hit_o) {
        fail(std::string("lu_hit_o mismatch (") + where + "): actual=" +
             dec(static_cast<int>(top->a_lu_hit_o)) +
             " reference=" + dec(static_cast<int>(top->r_lu_hit_o)));
    }
    if (top->a_content_lo != top->r_content_lo || top->a_content_hi != top->r_content_hi) {
        fail(std::string("lu_content_o mismatch (") + where + "): actual=" +
             hex64(top->a_content_hi) + ":" + hex64(top->a_content_lo) +
             " reference=" + hex64(top->r_content_hi) + ":" + hex64(top->r_content_lo));
    }
}

static void settle(const char* where) { top->clk_i = 0; eval_checked(where); }

static void edge(const char* where) {
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
    uint32_t vpn;    // 29 bit
    uint32_t pscid;  // 20 bit
    uint32_t gscid;  // 16 bit
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
    t.vpn = vpn & 0x1FFFFFFFu;
    t.pscid = pscid & 0xFFFFFu;
    t.gscid = gscid & 0xFFFFu;
    t.en1s = en1s ? 1 : 0;
    t.en2s = en2s ? 1 : 0;
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

// All tags get a globally unique vpn2 so that lookups never alias and so that
// the vpn2[8:0] aliasing in vaddr_vpn2_match (RTL line 167) cannot alias
// either (values stay below 0x200).
static uint32_t g_vpn2 = 0x40u;
static uint32_t next_vpn2() { return g_vpn2++; }

static void drive_update(const Tag& t, const Content& c) {
    clear_lookup();
    top->update_update_i = 1;
    top->update_vpn_i    = t.vpn;
    top->update_pscid_i  = t.pscid;
    top->update_gscid_i  = t.gscid;
    // rv_iommu_iotlb latches en_1S_i / en_2S_i together with the update
    // (RTL lines 314-315), so they must accompany every update.
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

static void expect_hit(const Tag& t, const std::string& label) {
    drive_lookup(t);
    settle(label.c_str());
    if (!top->a_lu_hit_o) fail(label + ": expected a hit");
    edge(label.c_str());
}

static void expect_miss(const Tag& t, const std::string& label) {
    drive_lookup(t);
    settle(label.c_str());
    if (top->a_lu_hit_o) fail(label + ": expected a miss");
    edge(label.c_str());
}

// ------------------------------------------------------------- tap access

static void read_q(int idx, uint64_t& lo, uint64_t& hi) {
    top->tap_idx = static_cast<uint8_t>(idx & 0xF);
    top->eval();
    lo = top->tap_q_lo;
    hi = top->tap_q_hi;
}

static void read_n(int idx, uint64_t& lo, uint64_t& hi) {
    top->tap_idx = static_cast<uint8_t>(idx & 0xF);
    top->eval();
    lo = top->tap_n_lo;
    hi = top->tap_n_hi;
}

static int      lo_bit  (uint64_t lo, int b)   { return static_cast<int>((lo >> b) & 1u); }
static uint32_t lo_vpn  (uint64_t lo)          { return static_cast<uint32_t>(lo & 0x1FFFFFFFu); }
static uint32_t lo_pscid(uint64_t lo)          { return static_cast<uint32_t>((lo >> 29) & 0xFFFFFu); }
static int      lo_en1s (uint64_t lo)          { return lo_bit(lo, 49); }
static int      lo_en2s (uint64_t lo)          { return lo_bit(lo, 50); }
static int      lo_valid(uint64_t lo)          { return lo_bit(lo, 51); }
static int      lo_c1_1g(uint64_t lo)          { return lo_bit(lo, 52); }
static int      lo_c1_2m(uint64_t lo)          { return lo_bit(lo, 53); }
static int      lo_c2_1g(uint64_t lo)          { return lo_bit(lo, 54); }
static int      lo_c2_2m(uint64_t lo)          { return lo_bit(lo, 55); }
static int      lo_c1_g (uint64_t lo)          { return lo_bit(lo, 56); }
static uint32_t hi_gscid(uint64_t hi)          { return static_cast<uint32_t>(hi & 0xFFFFu); }

static bool is_onehot16(uint16_t m) {
    return m != 0u && ((m & static_cast<uint16_t>(m - 1u)) == 0u);
}

static int lowest_bit16(uint16_t m) {
    for (int i = 0; i < 16; i++) if (m & (1u << i)) return i;
    return -1;
}

// Tree PLRU victim implied by the raw plru read-out (RTL lines 348-364):
// victim index bits equal the tree bits along the path, MSB first.
static int victim_from_plru(uint32_t plru) {
    int b3 = (plru >> 0) & 1;
    int b2 = (plru >> (1 + b3)) & 1;
    int b1 = (plru >> (3 + ((b3 << 1) | b2))) & 1;
    int b0 = (plru >> (7 + ((b3 << 2) | (b2 << 1) | b1))) & 1;
    return (b3 << 3) | (b2 << 2) | (b1 << 1) | b0;
}

static Tag tag_from_tap(uint64_t lo, uint64_t hi) {
    return make_tag(lo_vpn(lo), lo_pscid(lo), hi_gscid(hi), lo_en1s(lo), lo_en2s(lo));
}

// ------------------------------------------------------------- tap checks

static void check_entry_bits(uint64_t lo, uint64_t hi, const Tag& t, const Content& c,
                             const std::string& label) {
    if (lo_vpn(lo) != (t.vpn & 0x1FFFFFFFu))
        fail(label + ": VPN[28:0] mismatch, low=" + hex64(lo));
    if (lo_pscid(lo) != (t.pscid & 0xFFFFFu))
        fail(label + ": PSCID[48:29] mismatch, low=" + hex64(lo));
    if (lo_en1s(lo) != (t.en1s ? 1 : 0))
        fail(label + ": en_1S[49] mismatch");
    if (lo_en2s(lo) != (t.en2s ? 1 : 0))
        fail(label + ": en_2S[50] mismatch");
    if (lo_valid(lo) != 1)
        fail(label + ": valid[51] is not set");
    if (lo_c1_1g(lo) != c.c1_1g) fail(label + ": content_1S.is_1G[52] mismatch");
    if (lo_c1_2m(lo) != c.c1_2m) fail(label + ": content_1S.is_2M[53] mismatch");
    if (lo_c2_1g(lo) != c.c2_1g) fail(label + ": content_2S.is_1G[54] mismatch");
    if (lo_c2_2m(lo) != c.c2_2m) fail(label + ": content_2S.is_2M[55] mismatch");
    if (lo_c1_g (lo) != c.c1_g)  fail(label + ": content_1S.g[56] mismatch");
    if (hi_gscid(hi) != (t.gscid & 0xFFFFu))
        fail(label + ": GSCID in high[15:0] mismatch, high=" + hex64(hi));
    if ((t.gscid & 0xFFFFu) != 0u && hi_gscid(hi) == 0u)
        fail(label + ": nonzero GSCID not visible in high[15:0]");
    if ((hi >> 16) != 0u)
        fail(label + ": high[63:16] must be zero, high=" + hex64(hi));
    if ((lo >> 57) != 0u)
        fail(label + ": low[63:57] must be zero, low=" + hex64(lo));
}

// "all 16 slot views, n == q when nothing is pending"
static void check_no_change_all(const char* where) {
    for (int i = 0; i < 16; i++) {
        uint64_t qlo, qhi, nlo, nhi;
        read_q(i, qlo, qhi);
        read_n(i, nlo, nhi);
        if (qlo != nlo || qhi != nhi) {
            fail(std::string("entries_next != entries for idle slot ") + dec(i) +
                 " (" + where + "): q=" + hex64(qhi) + ":" + hex64(qlo) +
                 " n=" + hex64(nhi) + ":" + hex64(nlo));
        }
    }
}

// ------------------------------------------------------------------ writes

// Writes one entry through the real replacement path and then accesses it so
// that the PLRU moves to another way (the tree only moves on a lookup hit,
// RTL line 338).  Returns the way the entry landed in.
static int write_and_touch(const Tag& t, const Content& c) {
    clear_inval();
    drive_update(t, c);
    settle("write pre-edge");

    if (!is_onehot16(top->tap_replace_mask)) {
        fail(std::string("write: replace_mask is not one-hot: 0x") + hex16(top->tap_replace_mask));
    }
    if (victim_from_plru(top->tap_plru) != lowest_bit16(top->tap_replace_mask)) {
        fail("write: replace_mask disagrees with the PLRU tree read-out");
    }
    int v = lowest_bit16(top->tap_replace_mask);

    uint64_t nlo, nhi;
    read_n(v, nlo, nhi);
    check_entry_bits(nlo, nhi, t, c, "write: next-state way " + dec(v));

    edge("write edge");

    uint64_t qlo, qhi;
    read_q(v, qlo, qhi);
    if (qlo != nlo || qhi != nhi) {
        fail("write: committed state != pre-edge next state for way " + dec(v));
    }
    check_entry_bits(qlo, qhi, t, c, "write: committed way " + dec(v));

    drive_lookup(t);
    settle("write touch");
    if (!top->a_lu_hit_o) {
        fail("write: freshly written way " + dec(v) + " does not hit, PLRU cannot advance");
    }
    edge("write touch edge");
    return v;
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
    if (!top->tap_invalidate) fail("tap_invalidate is low while IOTINVAL.VMA is asserted");
    if (top->tap_update)      fail("tap_update is high while IOTINVAL.VMA is asserted");
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
    if (!top->tap_invalidate) fail("tap_invalidate is low while IOTINVAL.GVMA is asserted");
    if (top->tap_update)      fail("tap_update is high while IOTINVAL.GVMA is asserted");
    edge("inval gvma edge");
    clear_inval();
    settle("post-inval gvma settle");
}

// --------------------------------------------------------------------- main

static Tag fill_tag(int k) {
    // 0..7  : first-stage only   (en_2S = 0)
    // 8..15 : two-stage          (en_2S = 1)
    uint32_t vpn2  = next_vpn2();
    uint32_t pscid = (k < 8) ? (0x100u + static_cast<uint32_t>(k))
                             : (0x200u + static_cast<uint32_t>(k));
    uint32_t gscid = 0x1000u + static_cast<uint32_t>(k);   // always nonzero
    int      en2s  = (k < 8) ? 0 : 1;
    return make_tag(mk_vpn(vpn2, 0x100u + static_cast<uint32_t>(k), 0x010u + static_cast<uint32_t>(k)),
                    pscid, gscid, 1, en2s);
}

static Content fill_content(int k) {
    uint64_t ppn = 0x1000u + static_cast<uint32_t>(k);
    int      g   = (k % 4 == 0) ? 1 : 0;                   // a few global mappings
    return make_content(ppn, 0, 0, g, ppn, 0, 0, 0);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    top = new Vresearch_iotlb_top;

    top->clk_i   = 0;
    top->rst_ni  = 0;
    top->tap_idx = 0;
    clear_inval();
    clear_update();
    clear_lookup();

    // ------------------------------------------------------------------ reset
    g_phase = "reset";
    for (int i = 0; i < 4; i++) edge("reset");
    top->rst_ni = 1;
    settle("post-reset");

    if (!top->tap_supported) {
        fail("tap_supported == 0: the instance is not configured with NumIotlbEntries == 16");
    }
    for (int i = 0; i < 16; i++) {
        uint64_t lo, hi;
        read_q(i, lo, hi);
        if (lo != 0 || hi != 0) {
            fail(std::string("slot ") + dec(i) + " not cleared by reset: " + hex64(hi) + ":" + hex64(lo));
        }
    }

    // --------------------------------------------- idle: all 16 views n == q
    g_phase = "idle-n-eq-q";
    clear_inval(); clear_update(); clear_lookup();
    settle("idle");
    check_no_change_all("after reset");

    // ------------------------------------------------------- cold lookups
    g_phase = "cold-miss";
    for (int k = 0; k < 24; k++) {
        Tag t = make_tag(mk_vpn(0x500u + static_cast<uint32_t>(k),
                                0x100u + static_cast<uint32_t>(k),
                                0x020u + static_cast<uint32_t>(k)),
                         0x10u + static_cast<uint32_t>(k),
                         0x20u + static_cast<uint32_t>(k), 1, k & 1);
        drive_lookup(t);
        settle("cold lookup");
        if (top->a_lu_hit_o || top->r_lu_hit_o)
            fail(std::string("cold lookup #") + dec(k) + " hit with an empty IOTLB");
        edge("cold lookup");
    }

    // -------------------------------------- capacity fill: all 16 ways valid
    Tag     slot_tag[16];
    Content slot_content[16];
    int     filled[16];
    for (int i = 0; i < 16; i++) filled[i] = 0;

    g_phase = "capacity-fill";
    for (int k = 0; k < 16; k++) {
        Tag     t = fill_tag(k);
        Content c = fill_content(k);

        clear_inval();
        drive_update(t, c);
        settle("fill pre-edge");

        if (!is_onehot16(top->tap_replace_mask)) {
            fail(std::string("fill #") + dec(k) + ": replace_mask not one-hot: 0x" +
                 hex16(top->tap_replace_mask));
        }
        if (victim_from_plru(top->tap_plru) != lowest_bit16(top->tap_replace_mask)) {
            fail(std::string("fill #") + dec(k) + ": replace_mask disagrees with the PLRU read-out");
        }
        int v = lowest_bit16(top->tap_replace_mask);
        if (filled[v]) {
            fail(std::string("fill #") + dec(k) + ": PLRU reselected way " + dec(v) +
                 " before all 16 ways were filled");
        }
        filled[v] = 1;

        // committed (old) q vs next (new) n for the selected way, before the edge
        uint64_t qlo, qhi, nlo, nhi;
        read_q(v, qlo, qhi);
        read_n(v, nlo, nhi);
        if (qlo == nlo && qhi == nhi)
            fail(std::string("fill #") + dec(k) + ": selected way has n == q before an update edge");
        check_entry_bits(nlo, nhi, t, c, "fill #" + dec(k) + " next-state way " + dec(v));

        // unrelated ways must keep n == q
        for (int i = 0; i < 16; i++) {
            if (i == v) continue;
            uint64_t q2lo, q2hi, n2lo, n2hi;
            read_q(i, q2lo, q2hi);
            read_n(i, n2lo, n2hi);
            if (q2lo != n2lo || q2hi != n2hi) {
                fail(std::string("fill #") + dec(k) + ": update changed the next state of way " + dec(i));
            }
        }

        edge("fill edge");

        read_q(v, qlo, qhi);
        if (qlo != nlo || qhi != nhi) {
            fail(std::string("fill #") + dec(k) + ": committed state != pre-edge next state");
        }
        check_entry_bits(qlo, qhi, t, c, "fill #" + dec(k) + " committed way " + dec(v));

        slot_tag[v]     = t;
        slot_content[v] = c;

        // touch the new entry so the PLRU advances to another way
        drive_lookup(t);
        settle("fill touch");
        if (!top->a_lu_hit_o) fail(std::string("fill #") + dec(k) + ": freshly filled way does not hit");
        edge("fill touch edge");
    }

    for (int i = 0; i < 16; i++) {
        uint64_t lo, hi;
        read_q(i, lo, hi);
        if (!lo_valid(lo)) fail(std::string("way ") + dec(i) + " is not valid after 16 fill+access rounds");
    }

    // -------------------------------------- capacity replacement ("A" fill)
    g_phase = "capacity-replace";
    {
        Tag     tagA  = make_tag(mk_vpn(next_vpn2(), 0x0AB, 0x0CD), 0x321, 0x654, 1, 1);
        Content contA = make_content(0x9876543ULL, 0, 0, 0, 0x2468ACEULL, 0, 0, 0);

        clear_inval();
        drive_update(tagA, contA);
        settle("A update pre-edge");
        if (!is_onehot16(top->tap_replace_mask)) fail("A: replace_mask not one-hot");
        int victimA = lowest_bit16(top->tap_replace_mask);
        Tag lost = slot_tag[victimA];

        uint64_t anlo, anhi;
        read_n(victimA, anlo, anhi);
        check_entry_bits(anlo, anhi, tagA, contA, "A next-state way " + dec(victimA));

        edge("A update edge");
        uint64_t aqlo, aqhi;
        read_q(victimA, aqlo, aqhi);
        if (aqlo != anlo || aqhi != anhi) fail("A: committed state != pre-edge next state");
        check_entry_bits(aqlo, aqhi, tagA, contA, "A committed way " + dec(victimA));

        expect_hit(tagA, "lookup A");
        expect_miss(lost, "lookup evicted tag");

        for (int i = 0; i < 16; i++) {
            if (i == victimA) continue;
            expect_hit(slot_tag[i], "lookup survivor way " + dec(i));
        }
    }

    // ------------------------------------------------------------ superpage
    g_phase = "superpage-1G-s1";
    {
        Tag     t = make_tag(mk_vpn(next_vpn2(), 0x000, 0x000), 0x111, 0x222, 1, 0);
        Content c = make_content(0x10000000000ULL, 1, 0, 0, 0, 0, 0, 0);
        write_and_touch(t, c);

        Tag p = t; p.vpn = mk_vpn(t.vpn >> 18, 0x1FF, 0x1FF);
        expect_hit(p, "1G stage-1: other vpn1/vpn0 still hit");

        Tag m = t; m.vpn = mk_vpn((t.vpn >> 18) + 1u, 0x000, 0x000);
        expect_miss(m, "1G stage-1: other vpn2 misses");
    }

    g_phase = "superpage-2M-s1";
    {
        Tag     t = make_tag(mk_vpn(next_vpn2(), 0x040, 0x000), 0x115, 0x226, 1, 0);
        Content c = make_content(0x2000, 0, 1, 0, 0, 0, 0, 0);
        write_and_touch(t, c);

        Tag p = t; p.vpn = mk_vpn(t.vpn >> 18, 0x040, 0x1FF);
        expect_hit(p, "2M stage-1: other vpn0 still hits");

        Tag m = t; m.vpn = mk_vpn(t.vpn >> 18, 0x041, 0x000);
        expect_miss(m, "2M stage-1: other vpn1 misses");
    }

    g_phase = "superpage-1G-2s";
    {
        Tag     t = make_tag(mk_vpn(next_vpn2(), 0x000, 0x000), 0x118, 0x228, 1, 1);
        Content c = make_content(0x3000, 1, 0, 0, 0x4000, 1, 0, 0);
        write_and_touch(t, c);

        Tag p = t; p.vpn = mk_vpn(t.vpn >> 18, 0x1AB, 0x1CD);
        expect_hit(p, "two-stage 1G: other vpn1/vpn0 still hit");

        Tag m = t; m.vpn = mk_vpn((t.vpn >> 18) + 1u, 0x000, 0x000);
        expect_miss(m, "two-stage 1G: other vpn2 misses");
    }

    g_phase = "mixed-stage-superpage";
    {
        // S1 1G + S2 4K => is_trans_1G / is_trans_2M both false, exact 4K match
        Tag     t = make_tag(mk_vpn(next_vpn2(), 0x020, 0x008), 0x119, 0x229, 1, 1);
        Content c = make_content(0x5000, 1, 0, 0, 0x6000, 0, 0, 0);
        write_and_touch(t, c);

        expect_hit(t, "mixed-stage: exact address hits");

        Tag p = t; p.vpn = mk_vpn(t.vpn >> 18, 0x020, 0x1FF);
        expect_miss(p, "mixed-stage: vpn0 change misses");
    }

    // --------------------------------------------------------------- global
    g_phase = "global-match";
    {
        Tag     tg = make_tag(mk_vpn(next_vpn2(), 0x011, 0x022), 0x300, 0x400, 1, 0);
        Content cg = make_content(0x7000, 0, 0, 1, 0, 0, 0, 0);   // S1 G = 1
        write_and_touch(tg, cg);

        Tag p = tg; p.pscid = 0x3FF;
        expect_hit(p, "global S1 mapping ignores PSCID when S1 is enabled");

        Tag     tn = make_tag(mk_vpn(next_vpn2(), 0x012, 0x023), 0x300, 0x400, 1, 0);
        Content cn = make_content(0x7100, 0, 0, 0, 0, 0, 0, 0);   // S1 G = 0
        write_and_touch(tn, cn);

        Tag pn = tn; pn.pscid = 0x3FE;
        expect_miss(pn, "non-global S1 mapping requires its PSCID");
    }

    // ---------------------------------------------------- context (GSCID/PS)
    g_phase = "context-gscid-pscid";
    {
        Tag     t = make_tag(mk_vpn(next_vpn2(), 0x013, 0x024), 0x3A0, 0x4A0, 1, 1);
        Content c = make_content(0x7200, 0, 0, 0, 0x7300, 0, 0, 0);
        write_and_touch(t, c);

        expect_hit(t, "two-stage: matching GSCID/PSCID hits");

        Tag pg = t; pg.gscid = 0x4A1;
        expect_miss(pg, "two-stage: GSCID mismatch misses");

        Tag pp = t; pp.pscid = 0x3A1;
        expect_miss(pp, "two-stage: non-global PSCID mismatch misses");

        Tag ps = t; ps.en2s = 0;
        expect_miss(ps, "two-stage: stage mismatch misses");

        // second-stage-only entry (en_1S = 0, en_2S = 1)
        Tag     t2 = make_tag(mk_vpn(next_vpn2(), 0x015, 0x026), 0x3C0, 0x4C0, 0, 1);
        Content c2 = make_content(0, 0, 0, 0, 0x7600, 0, 0, 0);
        write_and_touch(t2, c2);
        expect_hit(t2, "second-stage-only entry hits");
        Tag p2 = t2; p2.pscid = 0x3FF;
        expect_hit(p2, "second-stage-only entry ignores PSCID");

        // global two-stage entry
        Tag     t3 = make_tag(mk_vpn(next_vpn2(), 0x016, 0x027), 0x3B0, 0x4B0, 1, 1);
        Content c3 = make_content(0x7400, 0, 0, 1, 0x7500, 0, 0, 0);
        write_and_touch(t3, c3);

        Tag p3 = t3; p3.pscid = 0x3B1;
        expect_hit(p3, "global two-stage entry ignores PSCID");

        Tag p4 = t3; p4.gscid = 0x4B1;
        expect_miss(p4, "global two-stage entry still requires the GSCID");
    }

    // ------------------------------------------------------ IOTINVAL.VMA
    g_phase = "inval-vma";
    {
        Tag     h[4];
        Content hc[4];
        int     hs[4];
        Tag     ts;
        Content tsc;
        int     ss;

        // (GV,AV,PSCV) = 000 : every host (1S only) entry is invalidated
        for (int k = 0; k < 4; k++) {
            h[k]  = make_tag(mk_vpn(next_vpn2(), 0x060u + static_cast<uint32_t>(k),
                                                0x030u + static_cast<uint32_t>(k)),
                             0x200u + static_cast<uint32_t>(k / 2),
                             0x300u + static_cast<uint32_t>(k), 1, 0);
            hc[k] = make_content(0x8000u + static_cast<uint32_t>(k), 0, 0,
                                 (k == 0 || k == 3) ? 1 : 0, 0, 0, 0, 0);
            hs[k] = write_and_touch(h[k], hc[k]);
        }
        ts  = make_tag(mk_vpn(next_vpn2(), 0x061, 0x031), 0x2F0, 0x3F0, 1, 1);
        tsc = make_content(0x8100, 0, 0, 0, 0x8200, 0, 0, 0);
        ss  = write_and_touch(ts, tsc);

        apply_inval_vma(0, 0, 0, 0, 0, 0);
        for (int k = 0; k < 4; k++) {
            uint64_t lo, hi;
            read_q(hs[k], lo, hi);
            if (lo_valid(lo)) fail(std::string("VMA(000): host way ") + dec(k) + " not invalidated");
            expect_miss(h[k], "VMA(000): invalidated tag");
        }
        {
            uint64_t lo, hi;
            read_q(ss, lo, hi);
            if (!lo_valid(lo)) fail("VMA(000): two-stage entry wrongly invalidated");
            expect_hit(ts, "VMA(000): two-stage entry survives");
        }

        // (GV,AV,PSCV) = 001 : matching PSCID, G = 0 only
        for (int k = 0; k < 4; k++) {
            h[k]  = make_tag(mk_vpn(next_vpn2(), 0x070u + static_cast<uint32_t>(k),
                                                0x040u + static_cast<uint32_t>(k)),
                             0x200u + static_cast<uint32_t>(k / 2),
                             0x300u + static_cast<uint32_t>(k), 1, 0);
            hc[k] = make_content(0x9000u + static_cast<uint32_t>(k), 0, 0,
                                 (k == 0 || k == 3) ? 1 : 0, 0, 0, 0, 0);
            hs[k] = write_and_touch(h[k], hc[k]);
        }
        apply_inval_vma(0, 0, 1, 0, 0x200, 0);
        {
            uint64_t lo, hi;
            read_q(hs[1], lo, hi);
            if (lo_valid(lo)) fail("VMA(001): PSCID-matching non-global entry not invalidated");
            read_q(hs[0], lo, hi);
            if (!lo_valid(lo)) fail("VMA(001): global entry wrongly invalidated");
            read_q(hs[2], lo, hi);
            if (!lo_valid(lo)) fail("VMA(001): non-matching-PSCID entry wrongly invalidated");
            read_q(hs[3], lo, hi);
            if (!lo_valid(lo)) fail("VMA(001): global entry wrongly invalidated");
        }

        // (GV,AV,PSCV) = 010 : IOVA match, global included
        for (int k = 0; k < 4; k++) {
            h[k]  = make_tag(mk_vpn(next_vpn2(), 0x080u + static_cast<uint32_t>(k),
                                                0x050u + static_cast<uint32_t>(k)),
                             0x200u + static_cast<uint32_t>(k / 2),
                             0x300u + static_cast<uint32_t>(k), 1, 0);
            hc[k] = make_content(0xA000u + static_cast<uint32_t>(k), 0, 0,
                                 (k == 0 || k == 3) ? 1 : 0, 0, 0, 0, 0);
            hs[k] = write_and_touch(h[k], hc[k]);
        }
        apply_inval_vma(0, 1, 0, h[2].vpn, 0, 0);
        {
            uint64_t lo, hi;
            read_q(hs[2], lo, hi);
            if (lo_valid(lo)) fail("VMA(010): IOVA-matching entry not invalidated");
            read_q(hs[0], lo, hi);
            if (!lo_valid(lo)) fail("VMA(010): unrelated global entry wrongly invalidated");
            read_q(hs[1], lo, hi);
            if (!lo_valid(lo)) fail("VMA(010): unrelated entry wrongly invalidated");
            read_q(hs[3], lo, hi);
            if (!lo_valid(lo)) fail("VMA(010): unrelated global entry wrongly invalidated");
        }

        // (GV,AV,PSCV) = 011 : IOVA + PSCID match, G = 0 only
        for (int k = 0; k < 4; k++) {
            h[k]  = make_tag(mk_vpn(next_vpn2(), 0x090u + static_cast<uint32_t>(k),
                                                0x060u + static_cast<uint32_t>(k)),
                             0x200u + static_cast<uint32_t>(k / 2),
                             0x300u + static_cast<uint32_t>(k), 1, 0);
            hc[k] = make_content(0xB000u + static_cast<uint32_t>(k), 0, 0,
                                 (k == 0 || k == 3) ? 1 : 0, 0, 0, 0, 0);
            hs[k] = write_and_touch(h[k], hc[k]);
        }
        apply_inval_vma(0, 1, 1, h[1].vpn, 0x200, 0);
        {
            uint64_t lo, hi;
            read_q(hs[1], lo, hi);
            if (lo_valid(lo)) fail("VMA(011): IOVA+PSCID non-global entry not invalidated");
            read_q(hs[0], lo, hi);
            if (!lo_valid(lo)) fail("VMA(011): global entry wrongly invalidated");
            read_q(hs[2], lo, hi);
            if (!lo_valid(lo)) fail("VMA(011): unrelated entry wrongly invalidated");
        }
    }

    // ------------------------------------- IOTINVAL.VMA for two-stage sets
    g_phase = "inval-vma-2s";
    {
        Tag     s[4];
        Content sc[4];
        int     ss[4];
        Tag     host;
        Content hostc;
        int     hostslot;

        // s0: gscid 0x500, pscid 0x400, G=1
        // s1: gscid 0x500, pscid 0x400, G=0
        // s2: gscid 0x501, pscid 0x400, G=0
        // s3: gscid 0x500, pscid 0x401, G=0
        for (int k = 0; k < 4; k++) {
            uint32_t ps = (k == 3) ? 0x401u : 0x400u;
            uint32_t gs = (k == 2) ? 0x501u : 0x500u;
            int      gg = (k == 0) ? 1 : 0;
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0C0u + static_cast<uint32_t>(k),
                                               0x070u + static_cast<uint32_t>(k)),
                             ps, gs, 1, 1);
            sc[k] = make_content(0xC000u + static_cast<uint32_t>(k), 0, 0, gg,
                                 0xC100u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        host   = make_tag(mk_vpn(next_vpn2(), 0x0C8, 0x078), 0x400, 0x500, 1, 0);
        hostc  = make_content(0xC800, 0, 0, 0, 0, 0, 0, 0);
        hostslot = write_and_touch(host, hostc);

        // 100 : GSCID match, global included
        apply_inval_vma(1, 0, 0, 0, 0, 0x500);
        {
            uint64_t lo, hi;
            for (int k = 0; k < 4; k++) {
                read_q(ss[k], lo, hi);
                int expect_invalid = (k == 0 || k == 1 || k == 3) ? 1 : 0;
                if (expect_invalid && lo_valid(lo))
                    fail(std::string("VMA(100): way ") + dec(k) + " with matching GSCID not invalidated");
                if (!expect_invalid && !lo_valid(lo))
                    fail(std::string("VMA(100): way ") + dec(k) + " wrongly invalidated");
            }
            read_q(hostslot, lo, hi);
            if (!lo_valid(lo)) fail("VMA(100): first-stage-only entry wrongly invalidated");
        }

        // 101 : GSCID + PSCID, G = 0 only
        for (int k = 0; k < 4; k++) {
            uint32_t ps = (k == 3) ? 0x401u : 0x400u;
            uint32_t gs = (k == 2) ? 0x501u : 0x500u;
            int      gg = (k == 0) ? 1 : 0;
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0D0u + static_cast<uint32_t>(k),
                                               0x080u + static_cast<uint32_t>(k)),
                             ps, gs, 1, 1);
            sc[k] = make_content(0xD000u + static_cast<uint32_t>(k), 0, 0, gg,
                                 0xD100u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        apply_inval_vma(1, 0, 1, 0, 0x400, 0x500);
        {
            uint64_t lo, hi;
            for (int k = 0; k < 4; k++) {
                read_q(ss[k], lo, hi);
                int expect_invalid = (k == 1) ? 1 : 0;
                if (expect_invalid && lo_valid(lo))
                    fail(std::string("VMA(101): way ") + dec(k) + " not invalidated");
                if (!expect_invalid && !lo_valid(lo))
                    fail(std::string("VMA(101): way ") + dec(k) + " wrongly invalidated");
            }
        }

        // 110 : GSCID + IOVA, global included
        for (int k = 0; k < 4; k++) {
            uint32_t ps = (k == 3) ? 0x401u : 0x400u;
            uint32_t gs = (k == 2) ? 0x501u : 0x500u;
            int      gg = (k == 0) ? 1 : 0;
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0E0u + static_cast<uint32_t>(k),
                                               0x090u + static_cast<uint32_t>(k)),
                             ps, gs, 1, 1);
            sc[k] = make_content(0xE000u + static_cast<uint32_t>(k), 0, 0, gg,
                                 0xE100u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        apply_inval_vma(1, 1, 0, s[1].vpn, 0, 0x500);
        {
            uint64_t lo, hi;
            for (int k = 0; k < 4; k++) {
                read_q(ss[k], lo, hi);
                int expect_invalid = (k == 1) ? 1 : 0;
                if (expect_invalid && lo_valid(lo))
                    fail(std::string("VMA(110): way ") + dec(k) + " not invalidated");
                if (!expect_invalid && !lo_valid(lo))
                    fail(std::string("VMA(110): way ") + dec(k) + " wrongly invalidated");
            }
        }

        // 111 : GSCID + PSCID + IOVA, G = 0 only
        for (int k = 0; k < 4; k++) {
            uint32_t ps = (k == 3) ? 0x401u : 0x400u;
            uint32_t gs = (k == 2) ? 0x501u : 0x500u;
            int      gg = (k == 0) ? 1 : 0;
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0F0u + static_cast<uint32_t>(k),
                                               0x0A0u + static_cast<uint32_t>(k)),
                             ps, gs, 1, 1);
            sc[k] = make_content(0xF000u + static_cast<uint32_t>(k), 0, 0, gg,
                                 0xF100u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        apply_inval_vma(1, 1, 1, s[1].vpn, 0x400, 0x500);
        {
            uint64_t lo, hi;
            for (int k = 0; k < 4; k++) {
                read_q(ss[k], lo, hi);
                int expect_invalid = (k == 1) ? 1 : 0;
                if (expect_invalid && lo_valid(lo))
                    fail(std::string("VMA(111): way ") + dec(k) + " not invalidated");
                if (!expect_invalid && !lo_valid(lo))
                    fail(std::string("VMA(111): way ") + dec(k) + " wrongly invalidated");
            }
        }
    }

    // ------------------------------------------------------ IOTINVAL.GVMA
    g_phase = "inval-gvma";
    {
        Tag     s[4];
        Content sc[4];
        int     ss[4];
        Tag     host;
        Content hostc;
        int     hostslot;

        // (GV,AV) = 00 and 01 : every two-stage entry is invalidated
        for (int k = 0; k < 4; k++) {
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0A8u + static_cast<uint32_t>(k),
                                               0x0B0u + static_cast<uint32_t>(k)),
                             0x600u + static_cast<uint32_t>(k),
                             0x700u + static_cast<uint32_t>(k), 1, 1);
            sc[k] = make_content(0x11000u + static_cast<uint32_t>(k), 0, 0, (k == 0) ? 1 : 0,
                                 0x12000u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        host     = make_tag(mk_vpn(next_vpn2(), 0x0B8, 0x0B8), 0x700, 0x800, 1, 0);
        hostc    = make_content(0x13000, 0, 0, 0, 0, 0, 0, 0);
        hostslot = write_and_touch(host, hostc);

        apply_inval_gvma(0, 0, 0, 0);
        for (int k = 0; k < 4; k++) {
            uint64_t lo, hi;
            read_q(ss[k], lo, hi);
            if (lo_valid(lo)) fail(std::string("GVMA(00): two-stage way ") + dec(k) + " not invalidated");
        }
        {
            uint64_t lo, hi;
            read_q(hostslot, lo, hi);
            if (!lo_valid(lo)) fail("GVMA(00): first-stage-only entry wrongly invalidated");
        }

        // (GV,AV) = 10 : GSCID match
        for (int k = 0; k < 4; k++) {
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0C8u + static_cast<uint32_t>(k),
                                               0x0C0u + static_cast<uint32_t>(k)),
                             0x600u + static_cast<uint32_t>(k),
                             0x700u + static_cast<uint32_t>(k), 1, 1);
            sc[k] = make_content(0x14000u + static_cast<uint32_t>(k), 0, 0, 0,
                                 0x15000u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        apply_inval_gvma(1, 0, 0, 0x701);
        {
            uint64_t lo, hi;
            for (int k = 0; k < 4; k++) {
                read_q(ss[k], lo, hi);
                int expect_invalid = (k == 1) ? 1 : 0;
                if (expect_invalid && lo_valid(lo))
                    fail(std::string("GVMA(10): way ") + dec(k) + " with matching GSCID not invalidated");
                if (!expect_invalid && !lo_valid(lo))
                    fail(std::string("GVMA(10): way ") + dec(k) + " wrongly invalidated");
            }
        }

        // (GV,AV) = 11 : GSCID + GPA match (make_gppn of a 4K first-stage mapping)
        for (int k = 0; k < 4; k++) {
            uint32_t gs = (k == 2) ? 0x701u : 0x700u;   // way 2: different GSCID
            s[k]  = make_tag(mk_vpn(next_vpn2(), 0x0D8u + static_cast<uint32_t>(k),
                                               0x0D0u + static_cast<uint32_t>(k)),
                             0x680u + static_cast<uint32_t>(k),
                             gs, 1, 1);
            sc[k] = make_content(0x0ABCDE1uLL + ((k == 1) ? 1u : 0u), 0, 0, 0,
                                 0x16000u + static_cast<uint32_t>(k), 0, 0, 0);
            ss[k] = write_and_touch(s[k], sc[k]);
        }
        // (no post-hoc s[2].gscid assignment)
        apply_inval_gvma(1, 1, 0x0ABCDE1u, 0x700);
        {
            uint64_t lo, hi;
            read_q(ss[0], lo, hi);
            if (lo_valid(lo)) fail("GVMA(11): GPA+GSCID matching way 0 not invalidated");
            read_q(ss[1], lo, hi);
            if (!lo_valid(lo)) fail("GVMA(11): way 1 with a different GPA wrongly invalidated");
            read_q(ss[2], lo, hi);
            if (!lo_valid(lo)) fail("GVMA(11): way 2 with a different GSCID wrongly invalidated");
            read_q(ss[3], lo, hi);
            if (lo_valid(lo)) fail("GVMA(11): GPA+GSCID matching way 3 not invalidated");
        }
    }

    // ---------------------------------- invalidation takes precedence
    g_phase = "inval-over-update";
    {
        Tag     t  = make_tag(mk_vpn(next_vpn2(), 0x0A0, 0x050), 0x280, 0x380, 1, 0);
        Content c  = make_content(0xA000, 0, 0, 0, 0, 0, 0, 0);
        int     s  = write_and_touch(t, c);

        Tag     tu = make_tag(mk_vpn(next_vpn2(), 0x0A1, 0x051), 0x281, 0x381, 1, 0);
        Content cu = make_content(0xA100, 0, 0, 0, 0, 0, 0, 0);

        clear_lookup();
        top->inval_vma_i   = 1;                 // (GV,AV,PSCV) = 000
        top->inval_gvma_i  = 0;
        top->inval_gv_i    = 0;
        top->inval_av_i    = 0;
        top->inval_pscv_i  = 0;
        top->inval_vpn_i   = 0;
        top->inval_gscid_i = 0;
        top->inval_pscid_i = 0;

        top->update_update_i = 1;
        top->update_vpn_i    = tu.vpn;
        top->update_pscid_i  = tu.pscid;
        top->update_gscid_i  = tu.gscid;
        top->en_1S_i         = 1;
        top->en_2S_i         = 0;
        top->up_c1s_is_1G_i  = cu.c1_1g;
        top->up_c1s_is_2M_i  = cu.c1_2m;
        top->up_c1s_ppn_i    = cu.c1_ppn;
        top->up_c1s_g_i      = cu.c1_g;
        top->up_c1s_u_i      = cu.c1_u;
        top->up_c1s_x_i      = cu.c1_x;
        top->up_c1s_w_i      = cu.c1_w;
        top->up_c1s_r_i      = cu.c1_r;
        top->up_c1s_v_i      = cu.c1_v;
        top->up_c2s_is_1G_i  = cu.c2_1g;
        top->up_c2s_is_2M_i  = cu.c2_2m;
        top->up_c2s_ppn_i    = cu.c2_ppn;
        top->up_c2s_g_i      = cu.c2_g;
        top->up_c2s_u_i      = cu.c2_u;
        top->up_c2s_x_i      = cu.c2_x;
        top->up_c2s_w_i      = cu.c2_w;
        top->up_c2s_r_i      = cu.c2_r;
        top->up_c2s_v_i      = cu.c2_v;

        settle("inval+update settle");
        if (!top->tap_invalidate) fail("invalidation not reported while both inval and update are asserted");
        if (top->tap_update)      fail("tap_update must be deasserted while an invalidation is pending");

        edge("inval+update edge");

        {
            uint64_t lo, hi;
            read_q(s, lo, hi);
            if (lo_valid(lo)) fail("pending invalidation did not win over the update");
        }
        clear_inval();
        clear_update();
        expect_miss(tu, "update suppressed by invalidation");
    }

    // ------------------------------------- identical tag update + one-hot mask
    g_phase = "identical-tag-update";
    {
        // pick a currently valid way and re-issue an update with an identical tag
        int      slot = -1;
        uint64_t slot_lo = 0, slot_hi = 0;
        for (int i = 0; i < 16; i++) {
            uint64_t lo, hi;
            read_q(i, lo, hi);
            if (lo_valid(lo)) { slot = i; slot_lo = lo; slot_hi = hi; break; }
        }
        if (slot < 0) {
            fail("identical-tag update: no valid way available");
        } else {
            Tag     t = tag_from_tap(slot_lo, slot_hi);
            Content c = make_content(0x1111, 0, 0, lo_c1_g(slot_lo), 0x2222, 0, 0, 0);

            clear_inval();
            drive_update(t, c);
            settle("identical update pre-edge");
            if (!is_onehot16(top->tap_replace_mask)) {
                fail("identical-tag update: replace_mask is not one-hot");
            }
            int v = lowest_bit16(top->tap_replace_mask);
            edge("identical update edge");

            uint64_t lo, hi;
            read_q(v, lo, hi);
            if (!lo_valid(lo)) fail("identical-tag update: way was not written");

            int dup = 0;
            for (int i = 0; i < 16; i++) {
                uint64_t l2, h2;
                read_q(i, l2, h2);
                if (!lo_valid(l2)) continue;
                if (lo_vpn(l2) == (t.vpn & 0x1FFFFFFFu) &&
                    lo_pscid(l2) == (t.pscid & 0xFFFFFu) &&
                    hi_gscid(h2) == (t.gscid & 0xFFFFu) &&
                    lo_en1s(l2) == (t.en1s ? 1 : 0) &&
                    lo_en2s(l2) == (t.en2s ? 1 : 0)) {
                    ++dup;
                }
            }
            if (dup < 1 || dup > 2) {
                fail(std::string("identical-tag update: unexpected number of resident copies = ") + dec(dup));
            }
            expect_hit(t, "identical-tag update: tag still hits");
        }
    }

    // ----------------------------------------------- final idle / selected n->q
    g_phase = "final-idle";
    clear_inval(); clear_update(); clear_lookup();
    settle("final idle");
    check_no_change_all("final idle");

    g_phase = "selected-n-to-q";
    {
        Tag     t = make_tag(mk_vpn(next_vpn2(), 0x0B0, 0x060), 0x2A0, 0x3A0, 1, 0);
        Content c = make_content(0xB000, 0, 0, 1, 0, 0, 0, 0);

        clear_inval();
        drive_update(t, c);
        settle("selected n->q pre-edge");
        if (!is_onehot16(top->tap_replace_mask)) fail("selected n->q: replace_mask not one-hot");
        int v = lowest_bit16(top->tap_replace_mask);

        uint64_t nlo, nhi, qlo, qhi;
        read_n(v, nlo, nhi);
        read_q(v, qlo, qhi);
        if (qlo == nlo && qhi == nhi) fail("selected n->q: selected way already shows n == q before the edge");
        check_entry_bits(nlo, nhi, t, c, "selected n->q next state");

        edge("selected n->q edge");

        read_q(v, qlo, qhi);
        if (qlo != nlo || qhi != nhi) fail("selected way did not commit the pre-edge next state");
        check_entry_bits(qlo, qhi, t, c, "selected n->q committed");

        read_n(v, nlo, nhi);
        if (qlo != nlo || qhi != nhi) fail("selected way: entries_next != entries once quiescent");
    }

    // ------------------------------------------------------------------ report
    if (g_fail == 0) {
        std::printf("PASS: %ld functional output comparisons, 0 failures\n", g_checks);
        delete top;
        return 0;
    }
    std::printf("FAIL: %d failures across %ld functional output comparisons\n", g_fail, g_checks);
    delete top;
    return 1;
}
