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
