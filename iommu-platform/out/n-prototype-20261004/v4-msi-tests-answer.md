# Proposed v4 MSI-eligibility test suite

## What this is

Three files that build and run the **real** `rv_iommu_nonblock_hit` module directly (no DDTC/IOTLB/PTW/fabric RTL), configured `HasMsiContext=1`, `dc_t = rv_iommu::dc_ext_t`, and exercise the *chosen v4* MSI-eligibility function:

- **DC side** (`dcMSIsafe`): `mode==0` always safe; `mode==1` safe **only if** `req_q.iova[VLEN39-1:12]` is *not* in the MSI window; modes `2..15` never safe.
- **Leaf side** (`leafMSIsafe`): `mode==0` always safe; `mode==1` safe **only if** `content_1S.ppn[GPPNW39-1:0]` is *not* in the MSI window.

The window predicate is taken as `(x & ~mask) == (pattern & ~mask)` (compare on the `~mask` bits), which is exactly what reproduces the supplied data (`mask=0xff`, `pattern=0x28000` ⇒ PPN `0x280ff` **reject**, `0x28100` **allow**).

The harness is intentionally a **bounded eligibility test**: every DDTC hit / IOTLB hit / DC / leaf / DDTP input is driven as a scalar port, so nothing about the real caches or the fabric is proven. The new `diag_o[15:0]` port is connected but left unobserved.

---

## Assumptions (Codex must confirm against the actual v4 RTL)

1. v4 adds `output logic [15:0] diag_o` (passive). Left unused; the harness asserts only on `accept_o / touch_o / response`.
2. `dc_t = rv_iommu::dc_ext_t`, `HasMsiContext = 1'b1`, `ddtp_t = rv_iommu_reg_pkg::iommu_reg2hw_ddtp_reg_t` (`iommu_mode.q` = 4b, `ppn.q` = 44b, as in the existing `nonblock_tl_top.sv`).
3. Bit alignment: the **input** check compares `req_q.iova[38:12]` against the **low `VPNW39`(27) bits** of pattern/mask; the **leaf** check compares `content_1S.ppn[28:0]` against the **low `GPPNW39`(29) bits**. If v4 instead compares `pattern[38:12]` on the input side, only `MSI_INPUT_IOVA` (C++) needs changing — it is a single named constant.
4. `~mask`-based comparison as above (required to reproduce `0x280ff`→reject / `0x28100`→allow).
5. All harness ports are ≤64 bits so Verilator maps them to plain `CData/SData/QData` (no `VlWide`, no pointer casts).

---

## `tests/nonblock_msi_top.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Scalar harness around the real rv_iommu_nonblock_hit hit path configured
// with HasMsiContext=1 and the extended device context (rv_iommu::dc_ext_t).
//
// The DC, DDTP, IOTLB leaf and candidate request are driven from scalar ports
// so the bounded MSI-eligibility behaviour can be observed directly. This is
// NOT a cache / PTW / fabric proof: no DDTC, IOTLB or walker RTL is
// instantiated and every cache lookup/hit input is owned by the harness.

module nonblock_msi_top
  import rv_iommu::*;
  import rv_iommu_reg_pkg::*;
#(
    parameter int unsigned PAddrWidth = 56
) (
    input  logic                    clk_i,
    input  logic                    rst_ni,

    // Global control
    input  logic                    enable_i,
    input  logic                    window_i,
    input  logic                    invalidate_i,
    input  logic                    normal_fire_i,

    // Candidate request (scalar)
    input  logic [23:0]             cand_did,
    input  logic [63:0]             cand_iova,
    input  logic [5:0]              cand_ttype,
    input  logic [19:0]             cand_pid,
    input  logic                    cand_pid_valid,
    input  logic                    cand_priv,
    input  logic                    cand_is_debug,
    input  logic                    cand_bank,
    input  logic                    cand_valid_i,

    // DDTP
    input  logic [3:0]              ddtp_iommu_mode,
    input  logic [43:0]             ddtp_ppn,

    // Device context (rv_iommu::dc_ext_t)
    input  logic                    ddtc_hit_i,
    input  logic                    dc_tc_v,
    input  logic                    dc_tc_pdtv,
    input  logic                    dc_tc_sxl,
    input  logic                    dc_tc_t2gpa,
    input  logic                    dc_tc_en_ats,
    input  logic                    dc_tc_en_pri,
    input  logic [3:0]              dc_fsc_mode,
    input  logic [3:0]              dc_iohgatp_mode,
    input  logic [19:0]             dc_ta_pscid,
    input  logic [3:0]              dc_msiptp_mode,
    input  logic [43:0]             dc_msiptp_ppn,
    input  logic [51:0]             dc_msi_pattern,
    input  logic [51:0]             dc_msi_mask,

    // IOTLB leaf (rv_iommu::iotlb_content_t)
    input  logic                    iotlb_hit_i,
    input  logic                    leaf_v,
    input  logic                    leaf_u,
    input  logic                    leaf_is_1G,
    input  logic                    leaf_is_2M,
    input  logic                    leaf_r,
    input  logic                    leaf_w,
    input  logic                    leaf_x,
    input  logic                    leaf_g,
    input  logic [43:0]             leaf_ppn,

    // Response sink
    input  logic                    response_ready_i,

    // Observability
    output logic                    intercept_o,
    output logic                    accept_o,
    output logic                    busy_o,
    output logic                    ddtc_lookup_o,
    output logic                    ddtc_touch_o,
    output logic [23:0]             ddtc_did_o,
    output logic                    iotlb_lookup_o,
    output logic                    iotlb_touch_o,
    output logic [GPPNW39-1:0]      iotlb_vpn_o,
    output logic [19:0]             iotlb_pscid_o,
    output logic                    response_valid_o,
    output logic                    resp_bank,
    output logic [PAddrWidth-1:0]   resp_spaddr,
    output logic                    resp_r,
    output logic                    resp_w,
    output logic                    resp_x,
    output logic                    resp_u,
    output logic                    resp_g,
    // Passive diagnostic tap. Deliberately not interpreted here; pass/fail is
    // always asserted on accept / touch / response, never on this port.
    output logic [15:0]             diag_o
);

    // ----------------------------------------------------------------------
    // Local types (field-for-field compatible with the module's uses)
    // ----------------------------------------------------------------------
    typedef struct packed {
        logic [23:0] did;
        logic [63:0] iova;
        logic [5:0]  ttype;
        logic [19:0] pid;
        logic        pid_valid;
        logic        priv;
        logic        is_debug;
        logic        bank;
    } msi_req_t;

    typedef struct packed {
        logic                  bank;
        logic [PAddrWidth-1:0] spaddr;
        logic                  r;
        logic                  w;
        logic                  x;
        logic                  u;
        logic                  g;
    } msi_resp_t;

    // ----------------------------------------------------------------------
    // Candidate request
    // ----------------------------------------------------------------------
    msi_req_t cand_i;
    always_comb begin
        cand_i          = '0;
        cand_i.did       = cand_did;
        cand_i.iova      = cand_iova;
        cand_i.ttype     = cand_ttype;
        cand_i.pid       = cand_pid;
        cand_i.pid_valid = cand_pid_valid;
        cand_i.priv      = cand_priv;
        cand_i.is_debug  = cand_is_debug;
        cand_i.bank      = cand_bank;
    end

    // ----------------------------------------------------------------------
    // Extended device context
    // ----------------------------------------------------------------------
    dc_ext_t dc_i;
    always_comb begin
        dc_i                          = '0;
        dc_i.tc.v                     = dc_tc_v;
        dc_i.tc.pdtv                  = dc_tc_pdtv;
        dc_i.tc.sxl                   = dc_tc_sxl;
        dc_i.tc.t2gpa                 = dc_tc_t2gpa;
        dc_i.tc.en_ats                = dc_tc_en_ats;
        dc_i.tc.en_pri                = dc_tc_en_pri;
        dc_i.fsc.mode                 = dc_fsc_mode;
        dc_i.iohgatp.mode             = dc_iohgatp_mode;
        dc_i.ta.pscid                 = dc_ta_pscid;
        dc_i.msiptp.mode              = dc_msiptp_mode;
        dc_i.msiptp.ppn               = dc_msiptp_ppn;
        dc_i.msi_addr_pattern.pattern = dc_msi_pattern;
        dc_i.msi_addr_mask.mask       = dc_msi_mask;
    end

    // ----------------------------------------------------------------------
    // IOTLB leaf content
    // ----------------------------------------------------------------------
    iotlb_content_t leaf_i;
    always_comb begin
        leaf_i                        = '0;
        leaf_i.content_1S.v           = leaf_v;
        leaf_i.content_1S.u           = leaf_u;
        leaf_i.content_1S.is_1G       = leaf_is_1G;
        leaf_i.content_1S.is_2M       = leaf_is_2M;
        leaf_i.content_1S.r           = leaf_r;
        leaf_i.content_1S.w           = leaf_w;
        leaf_i.content_1S.x           = leaf_x;
        leaf_i.content_1S.g           = leaf_g;
        leaf_i.content_1S.ppn         = leaf_ppn;
    end

    // ----------------------------------------------------------------------
    // DDTP
    // ----------------------------------------------------------------------
    iommu_reg2hw_ddtp_reg_t ddtp_i;
    always_comb begin
        ddtp_i              = '0;
        ddtp_i.iommu_mode.q = ddtp_iommu_mode;
        ddtp_i.ppn.q        = ddtp_ppn;
    end

    // ----------------------------------------------------------------------
    // DUT
    // ----------------------------------------------------------------------
    msi_resp_t resp_o;

    rv_iommu_nonblock_hit #(
        .PAddrWidth    (PAddrWidth),
        .HasMsiContext (1'b1),
        .dc_t          (rv_iommu::dc_ext_t),
        .req_t         (msi_req_t),
        .resp_t        (msi_resp_t),
        .ddtp_t        (iommu_reg2hw_ddtp_reg_t)
    ) i_dut (
        .clk_i             (clk_i),
        .rst_ni            (rst_ni),
        .enable_i          (enable_i),
        .window_i          (window_i),
        .candidate_i       (cand_i),
        .candidate_valid_i (cand_valid_i),
        .normal_fire_i     (normal_fire_i),
        .invalidate_i      (invalidate_i),
        .ddtp_i            (ddtp_i),
        .intercept_o       (intercept_o),
        .accept_o          (accept_o),
        .busy_o            (busy_o),
        .ddtc_lookup_o     (ddtc_lookup_o),
        .ddtc_touch_o      (ddtc_touch_o),
        .ddtc_did_o        (ddtc_did_o),
        .ddtc_hit_i        (ddtc_hit_i),
        .ddtc_dc_i         (dc_i),
        .iotlb_lookup_o    (iotlb_lookup_o),
        .iotlb_touch_o     (iotlb_touch_o),
        .iotlb_vpn_o       (iotlb_vpn_o),
        .iotlb_pscid_o     (iotlb_pscid_o),
        .iotlb_hit_i       (iotlb_hit_i),
        .iotlb_content_i   (leaf_i),
        .response_o        (resp_o),
        .response_valid_o  (response_valid_o),
        .response_ready_i  (response_ready_i),
        .diag_o            (diag_o)
    );

    assign resp_bank   = resp_o.bank;
    assign resp_spaddr = resp_o.spaddr;
    assign resp_r      = resp_o.r;
    assign resp_w      = resp_o.w;
    assign resp_x      = resp_o.x;
    assign resp_u      = resp_o.u;
    assign resp_g      = resp_o.g;

endmodule
```

---

## `tests/nonblock_msi_test.cpp`

```cpp
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
```

---

## `tests/test_nonblock_msi.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""
Verilate and run the bounded nonblocking MSI-eligibility test.

Only the explicitly listed RTL closure is built: the real
rv_iommu_nonblock_hit module (HasMsiContext=1, dc_t=rv_iommu::dc_ext_t) plus
the RTL packages it imports. No DDTC / IOTLB / PTW / wrapper RTL is included;
the DC, IOTLB, DDTP and candidate inputs are driven directly by the harness.
Missing dependencies are reported with their exact expected paths.
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IOMMU = ROOT / "cva6" / "corev_apu" / "riscv-iommu"

TOP_MODULE = "nonblock_msi_top"
OUT = ROOT / "out" / "nonblock-msi"


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path):
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None:
            continue
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main() -> int:
    pkg = IOMMU / "packages"
    rtl = IOMMU / "rtl" / "translation_logic"
    cc = IOMMU / "vendor" / "common_cells" / "src"
    axi = IOMMU / "vendor" / "axi" / "src"

    # Package order matters: cf_math_pkg / axi_pkg and the RTL packages must be
    # processed before rv_iommu_pkg and before the harness that imports them.
    required = [
        cc / "cf_math_pkg.sv",
        axi / "axi_pkg.sv",
        pkg / "rv_iommu_cfg_pkg.sv",
        pkg / "rv_iommu_reg_pkg.sv",
        pkg / "rv_iommu_pkg.sv",
        rtl / "rv_iommu_nonblock_hit.sv",
        ROOT / "tests" / "nonblock_msi_top.sv",
        ROOT / "tests" / "nonblock_msi_test.cpp",
    ]

    sources = dedup([require(p) for p in required])

    include_dirs = [
        IOMMU / "vendor" / "axi" / "include",
        IOMMU / "vendor" / "common_cells" / "include",
    ]
    for d in include_dirs:
        if not d.exists():
            sys.exit("required include directory missing: {}".format(d))

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wall",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module",
        TOP_MODULE,
        "--Mdir",
        str(obj_dir),
    ]
    args += ["-I{}".format(d) for d in include_dirs]
    args += [str(p) for p in sources]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP_MODULE)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.exit("verilator did not produce {}".format(exe))

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
```

---

## Acceptance commands (do not run here)

```bash
# Build + run all cases; PASS line is only emitted after every case executed.
python3 tests/test_nonblock_msi.py
# expected: nonblock_msi_test: PASS pass_cases=8 reject_cases=10 accepts=8 ddtc_touch=8 iotlb_touch=8 responses=8

# Inspect the raw Verilator build log if it fails
tail -n 200 out/nonblock-msi/build.log
```

Expected behaviour against the guarded RTL:

- With the **v4 module** (mode0-or-mode1-with-MSI-exclusion + `diag_o`): all 18 cases run and the `PASS` line is printed with the counters above.
- With the **old module** (guard `msiptp.mode == 0` only, no `diag_o`): the build fails first on the missing `diag_o` port; with `diag_o` added but the old guard, the flat mode1 positive cases (mode1 R/W, boundary-allow, backpressure) abort with `run_pass: fast path did not accept`, exactly as intended.

---

## Coverage / limitations to record

- **Bounded eligibility only.** No DDTC, IOTLB, MRIFC, PDTW or PTW behaviour, no AXI, no fabric, no invalidation engine. Cache lookup/hit/content and DDTP are driven as constants by the harness; this proves nothing about the real caches.
- `diag_o` is *observed but not interpreted*. The suite deliberately asserts on `accept_o`, the two `touch_o`, `resp_*` and `response_valid_o` only; if Codex wants diag semantics asserted, the per-bit meaning must be supplied.
- The negative cases check “no accept, no touch”, not the path taken to reach the reject (no state output exists). The `REJECT → IDLE` return is only exercised via `normal_fire_i` / candidate deassertion as a harness convenience.
- **Bit-alignment assumption** (item 3 above) is the single most likely thing to need a one-line tweak (`MSI_INPUT_IOVA`); the leaf-side constants (`MSI_LEAF_PPN/_LO/_HI`) already reproduce the supplied `0x280ff`/`0x28100` data under the stated `~mask` comparison.
- Reserved-mode coverage is spot checks at `2` and `15`; modes `3..14` are not enumerated.
- No assertions/invariants (`+define+ASSERTS_OFF`); no timing/`--timing`; Verilator 4.038 flag set matches the existing runner.