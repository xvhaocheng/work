// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded verification-only wrapper. It modifies no RTL: it instantiates the
// current rv_iommu_iotlb next to rv_iommu_iotlb_reference (only the module
// declaration of the prior revision renamed, extracted at run time by
// tests/test_research_iotlb.py) with the *same* configuration and the *same*
// inputs, and exposes
//   * the functional outputs of both instances, and
//   * the passive research_t tap view of the current instance
// on plain scalar ports so a C++ testbench can compare them.
//
// The two functional outputs (lu_hit_o, lu_content_o) are the only outputs the
// equivalence claim covers. The tap ports are passive read-outs.

`ifndef RESEARCH_IOTLB_TOP_SV
`define RESEARCH_IOTLB_TOP_SV

module research_iotlb_top (
    input  logic                              clk_i,
    input  logic                              rst_ni,

    // ---- IOTINVAL command fields (rv_iommu::iotlb_inval_t) ----
    input  logic                              inval_vma_i,
    input  logic                              inval_gvma_i,
    input  logic                              inval_av_i,
    input  logic                              inval_gv_i,
    input  logic                              inval_pscv_i,
    input  logic [rv_iommu::GPPNW39-1:0]      inval_vpn_i,
    input  logic [15:0]                       inval_gscid_i,
    input  logic [19:0]                       inval_pscid_i,

    // ---- IOTLB update fields (rv_iommu::iotlb_up_t) ----
    input  logic                              update_update_i,
    input  logic [rv_iommu::GPPNW39-1:0]      update_vpn_i,
    input  logic [19:0]                       update_pscid_i,
    input  logic [15:0]                       update_gscid_i,
    input  logic                              up_c1s_is_1G_i,
    input  logic                              up_c1s_is_2M_i,
    input  logic [43:0]                       up_c1s_ppn_i,
    input  logic                              up_c1s_g_i,
    input  logic                              up_c1s_u_i,
    input  logic                              up_c1s_x_i,
    input  logic                              up_c1s_w_i,
    input  logic                              up_c1s_r_i,
    input  logic                              up_c1s_v_i,
    input  logic                              up_c2s_is_1G_i,
    input  logic                              up_c2s_is_2M_i,
    input  logic [43:0]                       up_c2s_ppn_i,
    input  logic                              up_c2s_g_i,
    input  logic                              up_c2s_u_i,
    input  logic                              up_c2s_x_i,
    input  logic                              up_c2s_w_i,
    input  logic                              up_c2s_r_i,
    input  logic                              up_c2s_v_i,

    // ---- Lookup fields ----
    input  logic                              lookup_i,
    input  logic [rv_iommu::GPPNW39-1:0]      lu_vpn_i,
    input  logic [19:0]                       lu_pscid_i,
    input  logic [15:0]                       lu_gscid_i,
    input  logic                              en_1S_i,
    input  logic                              en_2S_i,

    // ---- functional outputs : current RTL ----
    output logic                              a_lu_hit_o,
    output logic [63:0]                       a_content_lo,
    output logic [63:0]                       a_content_hi,

    // ---- functional outputs : reference revision ----
    output logic                              r_lu_hit_o,
    output logic [63:0]                       r_content_lo,
    output logic [63:0]                       r_content_hi,

    // ---- passive research tap view (current RTL only) ----
    input  logic [3:0]                        tap_idx,
    output logic [63:0]                       tap_q_lo,
    output logic [63:0]                       tap_q_hi,
    output logic [63:0]                       tap_n_lo,
    output logic [63:0]                       tap_n_hi,
    output logic [31:0]                       tap_plru,
    output logic [15:0]                       tap_replace_mask,
    output logic                              tap_update,
    output logic                              tap_invalidate,
    output logic                              tap_supported
);

    // 16 IOTLB entries.  rv_iommu_iotlb only consumes NumIotlbEntries from the
    // configuration struct, so the remaining fields may stay at their zero
    // default.  Both instances below receive the very same value.
    localparam rv_iommu_cfg::rv_iommu_cfg_t IOTLB_CFG = '{default: '0, NumIotlbEntries: 16};

    rv_iommu::iotlb_inval_t   inval;
    rv_iommu::iotlb_up_t      update;

    logic                     a_lu_hit, r_lu_hit;
    rv_iommu::iotlb_content_t a_content, r_content;
    logic [103:0]             a_content_flat, r_content_flat;
    rv_iommu::research_t      research;

    // Scalar ports -> rv_iommu structs (all members are written; no default
    // assignment pattern is needed).
    always_comb begin : struct_build
        inval.inval_vma  = inval_vma_i;
        inval.inval_gvma = inval_gvma_i;
        inval.av         = inval_av_i;
        inval.gv         = inval_gv_i;
        inval.pscv       = inval_pscv_i;
        inval.vpn        = inval_vpn_i;
        inval.gscid      = inval_gscid_i;
        inval.pscid      = inval_pscid_i;

        update.update                    = update_update_i;
        update.vpn                       = update_vpn_i;
        update.pscid                     = update_pscid_i;
        update.gscid                     = update_gscid_i;
        update.content.content_1S.is_1G  = up_c1s_is_1G_i;
        update.content.content_1S.is_2M  = up_c1s_is_2M_i;
        update.content.content_1S.ppn    = up_c1s_ppn_i;
        update.content.content_1S.g      = up_c1s_g_i;
        update.content.content_1S.u      = up_c1s_u_i;
        update.content.content_1S.x      = up_c1s_x_i;
        update.content.content_1S.w      = up_c1s_w_i;
        update.content.content_1S.r      = up_c1s_r_i;
        update.content.content_1S.v      = up_c1s_v_i;
        update.content.content_2S.is_1G  = up_c2s_is_1G_i;
        update.content.content_2S.is_2M  = up_c2s_is_2M_i;
        update.content.content_2S.ppn    = up_c2s_ppn_i;
        update.content.content_2S.g      = up_c2s_g_i;
        update.content.content_2S.u      = up_c2s_u_i;
        update.content.content_2S.x      = up_c2s_x_i;
        update.content.content_2S.w      = up_c2s_w_i;
        update.content.content_2S.r      = up_c2s_r_i;
        update.content.content_2S.v      = up_c2s_v_i;
    end

    rv_iommu_iotlb #(
        .RVIOMMUCfg   (IOTLB_CFG)
    ) i_actual (
        .clk_i        (clk_i),
        .rst_ni       (rst_ni),
        .inval_i      (inval),
        .update_i     (update),
        .lookup_i     (lookup_i),
        .lu_vpn_i     (lu_vpn_i),
        .lu_pscid_i   (lu_pscid_i),
        .lu_gscid_i   (lu_gscid_i),
        .en_1S_i      (en_1S_i),
        .en_2S_i      (en_2S_i),
        .lu_hit_o     (a_lu_hit),
        .lu_content_o (a_content),
        .research_o   (research)
    );

    // Prior revision: identical functional ports, no research tap.
    rv_iommu_iotlb_reference #(
        .RVIOMMUCfg   (IOTLB_CFG)
    ) i_reference (
        .clk_i        (clk_i),
        .rst_ni       (rst_ni),
        .inval_i      (inval),
        .update_i     (update),
        .lookup_i     (lookup_i),
        .lu_vpn_i     (lu_vpn_i),
        .lu_pscid_i   (lu_pscid_i),
        .lu_gscid_i   (lu_gscid_i),
        .en_1S_i      (en_1S_i),
        .en_2S_i      (en_2S_i),
        .lu_hit_o     (r_lu_hit),
        .lu_content_o (r_content)
    );

    // iotlb_content_t is a packed struct; flatten it explicitly (field order
    // follows the packed declaration in rv_iommu_pkg.sv) so that the C++
    // testbench can compare the two views bit for bit.
    assign a_content_flat = {
        a_content.content_1S.is_1G, a_content.content_1S.is_2M, a_content.content_1S.ppn,
        a_content.content_1S.g,     a_content.content_1S.u,     a_content.content_1S.x,
        a_content.content_1S.w,     a_content.content_1S.r,     a_content.content_1S.v,
        a_content.content_2S.is_1G, a_content.content_2S.is_2M, a_content.content_2S.ppn,
        a_content.content_2S.g,     a_content.content_2S.u,     a_content.content_2S.x,
        a_content.content_2S.w,     a_content.content_2S.r,     a_content.content_2S.v
    };

    assign r_content_flat = {
        r_content.content_1S.is_1G, r_content.content_1S.is_2M, r_content.content_1S.ppn,
        r_content.content_1S.g,     r_content.content_1S.u,     r_content.content_1S.x,
        r_content.content_1S.w,     r_content.content_1S.r,     r_content.content_1S.v,
        r_content.content_2S.is_1G, r_content.content_2S.is_2M, r_content.content_2S.ppn,
        r_content.content_2S.g,     r_content.content_2S.u,     r_content.content_2S.x,
        r_content.content_2S.w,     r_content.content_2S.r,     r_content.content_2S.v
    };

    assign a_lu_hit_o   = a_lu_hit;
    assign r_lu_hit_o   = r_lu_hit;
    assign a_content_lo = a_content_flat[63:0];
    assign a_content_hi = {24'h0, a_content_flat[103:64]};
    assign r_content_lo = r_content_flat[63:0];
    assign r_content_hi = {24'h0, r_content_flat[103:64]};

    // Passive tap, index-selected so that all 16 slot views are reachable.
    assign tap_q_lo         = research.entries[tap_idx][63:0];
    assign tap_q_hi         = research.entries[tap_idx][127:64];
    assign tap_n_lo         = research.entries_next[tap_idx][63:0];
    assign tap_n_hi         = research.entries_next[tap_idx][127:64];
    assign tap_plru         = research.plru;
    assign tap_replace_mask = research.replace_mask;
    assign tap_update       = research.update;
    assign tap_invalidate   = research.invalidate;
    assign tap_supported    = research.supported;

endmodule

`endif // RESEARCH_IOTLB_TOP_SV
