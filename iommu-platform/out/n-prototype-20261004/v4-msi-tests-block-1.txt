// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded eligibility test of the real rv_iommu_nonblock_hit hit path with
// HasMsiContext=1 and dc_t = rv_iommu::dc_ext_t. The harness drives the DC,
// DDTP, IOTLB-leaf and candidate ports directly, so it verifies only the
// module's own accept / address / touch / response behaviour -- it is not a
// cache, PTW or fabric proof.

#include "Vnonblock_msi_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

// ---------------------------------------------------------------------------
// Failure helpers: always print a concrete reason before aborting.
// ---------------------------------------------------------------------------
#define FAIL_AT(msg)                                                          \
    do {                                                                      \
        std::fprintf(stderr, "nonblock_msi_test: FAIL: %s (%s:%d)\n", (msg),  \
                     __FILE__, __LINE__);                                     \
        std::abort();                                                         \
    } while (0)

#define CHECK_AT(cond, msg)                                                   \
    do {                                                                      \
        if (!(cond)) FAIL_AT(msg);                                            \
    } while (0)

// ---------------------------------------------------------------------------
// Shared image / constants
// ---------------------------------------------------------------------------
// Live bring-up DC image (DID10/11): msiptp.mode=1, MSI mask 0xff, MSI pattern
// 0x28000. The MSI window is the set of values whose ~mask bits equal the
// pattern's ~mask bits: low 27 bits on the input VPN side, low 29 bits on the
// cached leaf PPN side. This is exactly what makes leaf PPN 0x280ff reject and
// 0x28100 allow.
static const uint64_t MSI_PATTERN    = 0x28000ULL;
static const uint64_t MSI_MASK       = 0x000FFULL;

static const uint64_t ORD_IOVA       = 0x10000000ULL; // not in the MSI window
static const uint64_t ORD_LEAF_PPN   = 0x80000ULL;    // not in the MSI window

static const uint64_t MSI_INPUT_IOVA = 0x28000000ULL; // iova[38:12] = 0x28000
static const uint64_t MSI_LEAF_PPN   = 0x28000ULL;    // in the MSI window
static const uint64_t MSI_LEAF_LO    = 0x280FFULL;    // last in-window PPN
static const uint64_t MSI_LEAF_HI    = 0x28100ULL;    // first out-of-window PPN

static const uint8_t TT_R = 0x2; // UNTRANSLATED_R
static const uint8_t TT_W = 0x3; // UNTRANSLATED_W

struct Resp {
    uint64_t spaddr = 0;
    int      bank   = 0;
    uint8_t  r = 0, w = 0, x = 0, u = 0, g = 0;
};

struct Snap {
    bool     accept       = false;
    bool     ddtc_touch   = false;
    bool     iotlb_touch  = false;
    bool     rvalid       = false;
    bool     busy         = false;
    bool     ddtc_lookup  = false;
    bool     iotlb_lookup = false;
    int      bank         = 0;
    uint64_t spaddr       = 0;
    uint8_t  r = 0, w = 0, x = 0, u = 0, g = 0;
    uint32_t did   = 0;
    uint32_t vpn   = 0;
    uint32_t pscid = 0;
    uint32_t diag  = 0; // observed, never asserted on
};

class Tb {
public:
    Vnonblock_msi_top* top;

    // ---- drive state (applied every tick) ----
    uint8_t  enable = 0, window = 1, invalidate = 0, normal_fire = 0;
    uint32_t cand_did = 0;
    uint64_t cand_iova = 0;
    uint32_t cand_ttype = 0;
    uint32_t cand_pid = 0;
    uint8_t  cand_pid_valid = 0, cand_priv = 0, cand_is_debug = 0;
    uint8_t  cand_bank = 0, cand_valid = 0;
    uint8_t  ddtp_iommu_mode = 2;
    uint64_t ddtp_ppn = 0;
    uint8_t  ddtc_hit = 1;
    uint8_t  dc_tc_v = 1, dc_tc_pdtv = 0, dc_tc_sxl = 0;
    uint8_t  dc_tc_t2gpa = 0, dc_tc_en_ats = 0, dc_tc_en_pri = 0;
    uint8_t  dc_fsc_mode = 8, dc_iohgatp_mode = 0;
    uint32_t dc_ta_pscid = 0;
    uint8_t  dc_msiptp_mode = 0;
    uint64_t dc_msiptp_ppn = 0;
    uint64_t dc_msi_pattern = MSI_PATTERN;
    uint64_t dc_msi_mask = MSI_MASK;
    uint8_t  iotlb_hit = 1;
    uint8_t  leaf_v = 1, leaf_u = 1, leaf_is_1G = 0, leaf_is_2M = 0;
    uint8_t  leaf_r = 1, leaf_w = 1, leaf_x = 0, leaf_g = 0;
    uint64_t leaf_ppn = ORD_LEAF_PPN;
    uint8_t  response_ready = 1;

    // ---- cumulative observations (only what actually executed) ----
    int accepts = 0, ddtc_touches = 0, iotlb_touches = 0, responses = 0;
    int pass_cases = 0, reject_cases = 0;

    std::vector<Resp> resp_log;

    Tb() : top(new Vnonblock_msi_top) {}
    ~Tb() { top->final(); delete top; }

    // ------------------------------------------------------------------
    void apply() {
        top->enable_i      = enable;
        top->window_i      = window;
        top->invalidate_i  = invalidate;
        top->normal_fire_i = normal_fire;

        top->cand_did       = cand_did;
        top->cand_iova      = cand_iova;
        top->cand_ttype     = cand_ttype;
        top->cand_pid       = cand_pid;
        top->cand_pid_valid = cand_pid_valid;
        top->cand_priv      = cand_priv;
        top->cand_is_debug  = cand_is_debug;
        top->cand_bank      = cand_bank;
        top->cand_valid_i   = cand_valid;

        top->ddtp_iommu_mode = ddtp_iommu_mode;
        top->ddtp_ppn        = ddtp_ppn;

        top->ddtc_hit_i      = ddtc_hit;
        top->dc_tc_v         = dc_tc_v;
        top->dc_tc_pdtv      = dc_tc_pdtv;
        top->dc_tc_sxl       = dc_tc_sxl;
        top->dc_tc_t2gpa     = dc_tc_t2gpa;
        top->dc_tc_en_ats    = dc_tc_en_ats;
        top->dc_tc_en_pri    = dc_tc_en_pri;
        top->dc_fsc_mode     = dc_fsc_mode;
        top->dc_iohgatp_mode = dc_iohgatp_mode;
        top->dc_ta_pscid     = dc_ta_pscid;
        top->dc_msiptp_mode  = dc_msiptp_mode;
        top->dc_msiptp_ppn   = dc_msiptp_ppn;
        top->dc_msi_pattern  = dc_msi_pattern;
        top->dc_msi_mask     = dc_msi_mask;

        top->iotlb_hit_i = iotlb_hit;
        top->leaf_v      = leaf_v;
        top->leaf_u      = leaf_u;
        top->leaf_is_1G  = leaf_is_1G;
        top->leaf_is_2M  = leaf_is_2M;
        top->leaf_r      = leaf_r;
        top->leaf_w      = leaf_w;
        top->leaf_x      = leaf_x;
        top->leaf_g      = leaf_g;
        top->leaf_ppn    = leaf_ppn;

        top->response_ready_i = response_ready;
    }

    // One full cycle. Inputs are settled with clk low, every output is
    // snapshotted BEFORE the posedge, and the model is advanced from that
    // pre-edge snapshot only (no shared pre/post-edge mixing).
    Snap tick() {
        apply();
        top->clk_i = 0;
        top->eval();

        Snap s;
        s.accept       = top->accept_o;
        s.ddtc_touch   = top->ddtc_touch_o;
        s.iotlb_touch  = top->iotlb_touch_o;
        s.rvalid       = top->response_valid_o;
        s.busy         = top->busy_o;
        s.ddtc_lookup  = top->ddtc_lookup_o;
        s.iotlb_lookup = top->iotlb_lookup_o;
        s.bank         = top->resp_bank;
        s.spaddr       = top->resp_spaddr;
        s.r = top->resp_r;
        s.w = top->resp_w;
        s.x = top->resp_x;
        s.u = top->resp_u;
        s.g = top->resp_g;
        s.did   = top->ddtc_did_o;
        s.vpn   = top->iotlb_vpn_o;
        s.pscid = top->iotlb_pscid_o;
        s.diag  = top->diag_o;

        // Touch is the sole accept consequence: never assert one without the
        // other.
        CHECK_AT(s.ddtc_touch  == s.accept, "ddtc_touch_o must equal accept_o");
        CHECK_AT(s.iotlb_touch == s.accept, "iotlb_touch_o must equal accept_o");

        if (s.accept) {
            accepts++;
            if (s.ddtc_touch)  ddtc_touches++;
            if (s.iotlb_touch) iotlb_touches++;
        }
        if (s.rvalid && response_ready) {
            Resp r;
            r.spaddr = s.spaddr;
            r.bank   = s.bank;
            r.r = s.r; r.w = s.w; r.x = s.x; r.u = s.u; r.g = s.g;
            resp_log.push_back(r);
            responses++;
        }

        top->clk_i = 1;
        top->eval();
        top->clk_i = 0;
        top->eval();
        return s;
    }

    // ------------------------------------------------------------------
    void reset() {
        enable = 0; window = 1; invalidate = 0; normal_fire = 0;
        cand_did = 0; cand_iova = 0; cand_ttype = 0; cand_pid = 0;
        cand_pid_valid = 0; cand_priv = 0; cand_is_debug = 0;
        cand_bank = 0; cand_valid = 0;
        ddtp_iommu_mode = 2; ddtp_ppn = 0x10;
        ddtc_hit = 1;
        dc_tc_v = 1; dc_tc_pdtv = 0; dc_tc_sxl = 0;
        dc_tc_t2gpa = 0; dc_tc_en_ats = 0; dc_tc_en_pri = 0;
        dc_fsc_mode = 8; dc_iohgatp_mode = 0; dc_ta_pscid = 0;
        dc_msiptp_mode = 0; dc_msiptp_ppn = 0;
        dc_msi_pattern = MSI_PATTERN; dc_msi_mask = MSI_MASK;
        iotlb_hit = 1;
        leaf_v = 1; leaf_u = 1; leaf_is_1G = 0; leaf_is_2M = 0;
        leaf_r = 1; leaf_w = 1; leaf_x = 0; leaf_g = 0;
        leaf_ppn = ORD_LEAF_PPN;
        response_ready = 1;

        top->clk_i  = 0;
        top->rst_ni = 0;
        apply();
        top->eval();
        for (int i = 0; i < 4; ++i) {
            top->clk_i = 1; top->eval();
            top->clk_i = 0; top->eval();
        }
        top->rst_ni = 1;
        apply();
        top->clk_i = 1; top->eval();
        top->clk_i = 0; top->eval();

        resp_log.clear();
    }

    // Baseline config for a would-be fast hit.
    void set_pass_config(uint8_t msi_mode, uint64_t iova, uint8_t ttype,
                         uint64_t leaf_ppn_v) {
        enable = 1; window = 1; invalidate = 0; normal_fire = 0;
        response_ready = 1;
        cand_did = 10; cand_iova = iova; cand_ttype = ttype;
        cand_pid = 0; cand_pid_valid = 0; cand_priv = 0;
        cand_is_debug = 0; cand_bank = 0; cand_valid = 0;
        ddtp_iommu_mode = 2; ddtp_ppn = 0x10;
        ddtc_hit = 1;
        dc_tc_v = 1; dc_tc_pdtv = 0; dc_tc_sxl = 0;
        dc_tc_t2gpa = 0; dc_tc_en_ats = 0; dc_tc_en_pri = 0;
        dc_fsc_mode = 8; dc_iohgatp_mode = 0; dc_ta_pscid = 0x0A;
        dc_msiptp_mode = msi_mode; dc_msiptp_ppn = 0x1234;
        dc_msi_pattern = MSI_PATTERN; dc_msi_mask = MSI_MASK;
        iotlb_hit = 1;
        leaf_v = 1; leaf_u = 1; leaf_is_1G = 0; leaf_is_2M = 0;
        leaf_r = 1; leaf_w = 1; leaf_x = 0; leaf_g = 0;
        leaf_ppn = leaf_ppn_v;
    }

    // Expect a fast accept, exactly one of each touch, then the response.
    void run_pass(uint64_t exp_spaddr) {
        const int acc0 = accepts;
        const int dt0  = ddtc_touches;
        const int it0  = iotlb_touches;

        cand_valid = 1;
        bool accepted = false;
        for (int i = 0; i < 64 && !accepted; ++i) {
            Snap s = tick();
            if (s.accept) accepted = true;
        }
        CHECK_AT(accepted, "run_pass: fast path did not accept");
        CHECK_AT(accepts == acc0 + 1, "run_pass: accept count != 1");
        CHECK_AT(ddtc_touches == dt0 + 1, "run_pass: DDTC touch count != 1");
        CHECK_AT(iotlb_touches == it0 + 1, "run_pass: IOTLB touch count != 1");
        cand_valid = 0;

        Resp r;
        bool got = false;
        for (int i = 0; i < 64 && !got; ++i) {
            tick();
            if (!resp_log.empty()) {
                r = resp_log.front();
                resp_log.erase(resp_log.begin());
                got = true;
            }
        }
        CHECK_AT(got, "run_pass: no response");
        CHECK_AT(r.spaddr == exp_spaddr, "run_pass: response spaddr mismatch");
        CHECK_AT(r.bank == 0, "run_pass: response bank mismatch");
        CHECK_AT(resp_log.empty(), "run_pass: duplicate response");
        pass_cases++;
    }

    // Expect the fast path to refuse; no accept and no cache touch at all.
    void run_reject() {
        const int acc0 = accepts;
        const int dt0  = ddtc_touches;
        const int it0  = iotlb_touches;

        cand_valid = 1;
        for (int i = 0; i < 16; ++i) tick();

        CHECK_AT(accepts == acc0, "run_reject: unexpected accept");
        CHECK_AT(ddtc_touches == dt0, "run_reject: unexpected DDTC touch");
        CHECK_AT(iotlb_touches == it0, "run_reject: unexpected IOTLB touch");

        normal_fire = 1; tick(); normal_fire = 0;
        cand_valid = 0; tick();
        reject_cases++;
    }

    // Mutate one DC field after the DC has been captured and before the LEAF
    // accept; the cached DC must no longer match.
    void run_dc_change(int which) {
        const int acc0 = accepts;
        const int dt0  = ddtc_touches;
        const int it0  = iotlb_touches;

        cand_valid = 1;
        tick(); // IDLE -> DC
        tick(); // DC  -> LEAF, dc_q captured

        if (which == 0)      dc_msi_mask    = 0x1FFULL;
        else if (which == 1) dc_msi_pattern = 0x38000ULL;
        else                 dc_msiptp_ppn  = 0x2345ULL;

        for (int i = 0; i < 8; ++i) tick();

        CHECK_AT(accepts == acc0, "dc-change: unexpected accept");
        CHECK_AT(ddtc_touches == dt0, "dc-change: unexpected DDTC touch");
        CHECK_AT(iotlb_touches == it0, "dc-change: unexpected IOTLB touch");

        normal_fire = 1; tick(); normal_fire = 0;
        cand_valid = 0; tick();
        reject_cases++;
    }

    // Hold the response sink low: payload must stay stable and touches fire
    // exactly once.
    void run_backpressure(uint64_t exp_spaddr) {
        const int acc0 = accepts;
        const int dt0  = ddtc_touches;
        const int it0  = iotlb_touches;

        response_ready = 0;
        cand_valid = 1;
        bool accepted = false;
        for (int i = 0; i < 64 && !accepted; ++i) {
            Snap s = tick();
            if (s.accept) accepted = true;
        }
        CHECK_AT(accepted, "backpressure: fast path did not accept");
        cand_valid = 0;

        bool     have = false;
        uint64_t sp   = 0;
        int      bank = -1;
        int      held = 0;
        for (int i = 0; i < 64; ++i) {
            Snap s = tick();
            if (s.rvalid) {
                if (!have) {
                    have = true; sp = s.spaddr; bank = s.bank;
                } else {
                    CHECK_AT(s.spaddr == sp && s.bank == bank,
                             "backpressure: response payload changed while stalled");
                }
                held++;
            } else if (have) {
                FAIL_AT("backpressure: response_valid dropped while stalled");
            }
        }
        CHECK_AT(have, "backpressure: no response presented");
        CHECK_AT(held > 16, "backpressure: response was not actually held");
        CHECK_AT(sp == exp_spaddr && bank == 0, "backpressure: stalled payload mismatch");
        CHECK_AT(accepts == acc0 + 1, "backpressure: accept count != 1");
        CHECK_AT(ddtc_touches == dt0 + 1, "backpressure: DDTC touch count != 1");
        CHECK_AT(iotlb_touches == it0 + 1, "backpressure: IOTLB touch count != 1");
        CHECK_AT(resp_log.empty(), "backpressure: unexpected early response");

        response_ready = 1;
        tick();
        CHECK_AT(resp_log.size() == 1, "backpressure: response not drained exactly once");
        Resp r = resp_log.front();
        resp_log.erase(resp_log.begin());
        CHECK_AT(r.spaddr == exp_spaddr && r.bank == 0,
                 "backpressure: drained payload mismatch");

        Snap s2 = tick();
        CHECK_AT(!s2.rvalid, "backpressure: response_valid still high after drain");
        CHECK_AT(resp_log.empty(), "backpressure: extra response");
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Tb tb;

    // ---- mode0 read / write pass ----
    tb.reset();
    tb.set_pass_config(0, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_pass(0x80000000ULL);

    tb.reset();
    tb.set_pass_config(0, ORD_IOVA, TT_W, ORD_LEAF_PPN);
    tb.run_pass(0x80000000ULL);

    // ---- mode0 stays unconditional over the MSI window ----
    tb.reset();
    tb.set_pass_config(0, MSI_INPUT_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_pass(0x80000000ULL);

    tb.reset();
    tb.set_pass_config(0, ORD_IOVA, TT_R, MSI_LEAF_PPN);
    tb.run_pass(0x28000000ULL);

    // ---- mode1 flat, ordinary input + ordinary leaf, read / write pass ----
    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_pass(0x80000000ULL);

    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_W, ORD_LEAF_PPN);
    tb.run_pass(0x80000000ULL);

    // ---- mode1 flat, input in MSI range, ordinary leaf: reject, no touch ----
    tb.reset();
    tb.set_pass_config(1, MSI_INPUT_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_reject();

    tb.reset();
    tb.set_pass_config(1, MSI_INPUT_IOVA, TT_W, ORD_LEAF_PPN);
    tb.run_reject();

    // ---- mode1 flat, ordinary input, leaf PPN in MSI range: reject on hit ----
    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, MSI_LEAF_PPN);
    tb.run_reject();

    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_W, MSI_LEAF_PPN);
    tb.run_reject();

    // ---- mask boundary: 0x280ff reject, 0x28100 allow ----
    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, MSI_LEAF_LO);
    tb.run_reject();

    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, MSI_LEAF_HI);
    tb.run_pass(0x28100000ULL);

    // ---- reserved modes 2 / 15 reject ----
    tb.reset();
    tb.set_pass_config(2, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_reject();

    tb.reset();
    tb.set_pass_config(15, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_reject();

    // ---- DC change after capture, before LEAF accept: reject ----
    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_dc_change(0); // mask change

    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_dc_change(1); // pattern change

    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_dc_change(2); // plain DC field change

    // ---- held response backpressure ----
    tb.reset();
    tb.set_pass_config(1, ORD_IOVA, TT_R, ORD_LEAF_PPN);
    tb.run_backpressure(0x80000000ULL);

    std::printf("nonblock_msi_test: PASS pass_cases=%d reject_cases=%d "
                "accepts=%d ddtc_touch=%d iotlb_touch=%d responses=%d\n",
                tb.pass_cases, tb.reject_cases, tb.accepts, tb.ddtc_touches,
                tb.iotlb_touches, tb.responses);
    return 0;
}
