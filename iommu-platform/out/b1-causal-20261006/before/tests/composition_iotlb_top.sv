// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded verification-only wrapper for the composition IOTLB allocation
// policy (profile migration-4dma-composition, nonblock_mode = 3).  It modifies
// no RTL.  Three instances of rv_iommu_iotlb receive the very same stimulus:
//
//   * i_actual    current rv_iommu_iotlb, PolicyEnable = 1   (under test)
//   * i_base      current rv_iommu_iotlb, PolicyEnable = 0   (in-revision LRU)
//   * i_reference extracted pre-change revision, module identifier renamed at
//                 run time by tests/test_composition_iotlb.py
//
// The functional outputs lu_hit_o / lu_content_o of all three instances are
// compared on every evaluation.  The passive research_t taps of the two current
// instances are exposed on scalar ports so the C++ testbench can observe victim
// selection, PLRU state and the actual entry arrays.

`ifndef COMPOSITION_IOTLB_TOP_SV
`define COMPOSITION_IOTLB_TOP_SV

module composition_iotlb_top (
    input  logic                          clk_i,
    input  logic                          rst_ni,

    input  logic                          policy_clear_i,
    input  logic [15:0]                   policy_mask_i,

    input  logic                          inval_vma_i,
    input  logic                          inval_gvma_i,
    input  logic                          inval_av_i,
    input  logic                          inval_gv_i,
    input  logic                          inval_pscv_i,
    input  logic [rv_iommu::GPPNW39-1:0]  inval_vpn_i,
    input  logic [15:0]                   inval_gscid_i,
    input  logic [19:0]                   inval_pscid_i,

    input  logic                          update_update_i,
    input  logic [rv_iommu::GPPNW39-1:0]  update_vpn_i,
    input  logic [19:0]                   update_pscid_i,
    input  logic [15:0]                   update_gscid_i,
    input  logic                          up_c1s_is_1G_i,
    input  logic                          up_c1s_is_2M_i,
    input  logic [43:0]                   up_c1s_ppn_i,
    input  logic                          up_c1s_g_i,
    input  logic                          up_c1s_u_i,
    input  logic                          up_c1s_x_i,
    input  logic                          up_c1s_w_i,
    input  logic                          up_c1s_r_i,
    input  logic                          up_c1s_v_i,
    input  logic                          up_c2s_is_1G_i,
    input  logic                          up_c2s_is_2M_i,
    input  logic [43:0]                   up_c2s_ppn_i,
    input  logic                          up_c2s_g_i,
    input  logic                          up_c2s_u_i,
    input  logic                          up_c2s_x_i,
    input  logic                          up_c2s_w_i,
    input  logic                          up_c2s_r_i,
    input  logic                          up_c2s_v_i,

    input  logic                          lookup_i,
    input  logic [rv_iommu::GPPNW39-1:0]  lu_vpn_i,
    input  logic [19:0]                   lu_pscid_i,
    input  logic [15:0]                   lu_gscid_i,
    input  logic                          en_1S_i,
    input  logic                          en_2S_i,

    output logic                          a_lu_hit_o,
    output logic [63:0]                   a_content_lo,
    output logic [63:0]                   a_content_hi,

    output logic                          b_lu_hit_o,
    output logic [63:0]                   b_content_lo,
    output logic [63:0]                   b_content_hi,

    output logic                          r_lu_hit_o,
    output logic [63:0]                   r_content_lo,
    output logic [63:0]                   r_content_hi,

    input  logic [3:0]                    tap_idx,
    output logic [15:0]                   a_replace_mask,
    output logic [31:0]                   a_plru,
    output logic [63:0]                   a_q_lo,
    output logic [63:0]                   a_q_hi,
    output logic [63:0]                   a_n_lo,
    output logic [63:0]                   a_n_hi,
    output logic                          a_update,
    output logic                          a_invalidate,
    output logic                          a_supported,
    output logic [15:0]                   b_replace_mask,
    output logic [31:0]                   b_plru,
    output logic                          b_supported
);

    // 16 IOTLB entries; only NumIotlbEntries is consumed by rv_iommu_iotlb.
    localparam rv_iommu_cfg::rv_iommu_cfg_t IOTLB_CFG = '{default: '0, NumIotlbEntries: 16};

    rv_iommu::iotlb_inval_t   inval;
    rv_iommu::iotlb_up_t      update;

    logic                     a_lu_hit, b_lu_hit, r_lu_hit;
    rv_iommu::iotlb_content_t a_content, b_content, r_content;
    logic [103:0]             a_content_flat, b_content_flat, r_content_flat;
    rv_iommu::research_t      a_research, b_research;

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

    // Composition under test.  TouchEnable = 0 keeps the pre-existing
    // touch-on-hit behaviour of the reference so the full-mask comparison is
    // bit-exact; touch_i is tied low.
    rv_iommu_iotlb #(
        .RVIOMMUCfg   (IOTLB_CFG),
        .TouchEnable  (1'b0),
        .PolicyEnable (1'b1)
    ) i_actual (
        .clk_i          (clk_i),
        .rst_ni         (rst_ni),
        .inval_i        (inval),
        .policy_clear_i (policy_clear_i),
        .policy_mask_i  (policy_mask_i),
        .update_i       (update),
        .lookup_i       (lookup_i),
        .touch_i        (1'b0),
        .lu_vpn_i       (lu_vpn_i),
        .lu_pscid_i     (lu_pscid_i),
        .lu_gscid_i     (lu_gscid_i),
        .en_1S_i        (en_1S_i),
        .en_2S_i        (en_2S_i),
        .lu_hit_o       (a_lu_hit),
        .lu_content_o   (a_content),
        .research_o     (a_research)
    );

    // Same revision, policy disabled: the in-tree LRU baseline.
    rv_iommu_iotlb #(
        .RVIOMMUCfg   (IOTLB_CFG),
        .TouchEnable  (1'b0),
        .PolicyEnable (1'b0)
    ) i_base (
        .clk_i          (clk_i),
        .rst_ni         (rst_ni),
        .inval_i        (inval),
        .policy_clear_i (1'b0),
        .policy_mask_i  ('1),
        .update_i       (update),
        .lookup_i       (lookup_i),
        .touch_i        (1'b0),
        .lu_vpn_i       (lu_vpn_i),
        .lu_pscid_i     (lu_pscid_i),
        .lu_gscid_i     (lu_gscid_i),
        .en_1S_i        (en_1S_i),
        .en_2S_i        (en_2S_i),
        .lu_hit_o       (b_lu_hit),
        .lu_content_o   (b_content),
        .research_o     (b_research)
    );

    // Pre-change revision; only the module declaration was renamed.  Extra
    // output ports (if any) are intentionally left unconnected.
    rv_iommu_iotlb_reference #(
        .RVIOMMUCfg (IOTLB_CFG)
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

    // iotlb_content_t is packed; flatten explicitly (field order follows the
    // packed declaration) so the C++ testbench compares bit for bit.
    assign a_content_flat = {
        a_content.content_1S.is_1G, a_content.content_1S.is_2M, a_content.content_1S.ppn,
        a_content.content_1S.g,     a_content.content_1S.u,     a_content.content_1S.x,
        a_content.content_1S.w,     a_content.content_1S.r,     a_content.content_1S.v,
        a_content.content_2S.is_1G, a_content.content_2S.is_2M, a_content.content_2S.ppn,
        a_content.content_2S.g,     a_content.content_2S.u,     a_content.content_2S.x,
        a_content.content_2S.w,     a_content.content_2S.r,     a_content.content_2S.v
    };

    assign b_content_flat = {
        b_content.content_1S.is_1G, b_content.content_1S.is_2M, b_content.content_1S.ppn,
        b_content.content_1S.g,     b_content.content_1S.u,     b_content.content_1S.x,
        b_content.content_1S.w,     b_content.content_1S.r,     b_content.content_1S.v,
        b_content.content_2S.is_1G, b_content.content_2S.is_2M, b_content.content_2S.ppn,
        b_content.content_2S.g,     b_content.content_2S.u,     b_content.content_2S.x,
        b_content.content_2S.w,     b_content.content_2S.r,     b_content.content_2S.v
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
    assign a_content_lo = a_content_flat[63:0];
    assign a_content_hi = {24'h0, a_content_flat[103:64]};
    assign b_lu_hit_o   = b_lu_hit;
    assign b_content_lo = b_content_flat[63:0];
    assign b_content_hi = {24'h0, b_content_flat[103:64]};
    assign r_lu_hit_o   = r_lu_hit;
    assign r_content_lo = r_content_flat[63:0];
    assign r_content_hi = {24'h0, r_content_flat[103:64]};

    // Passive tap of the current actual instance, index-selected for all 16 ways.
    assign a_replace_mask = a_research.replace_mask;
    assign a_plru         = a_research.plru;
    assign a_q_lo         = a_research.entries[tap_idx][63:0];
    assign a_q_hi         = a_research.entries[tap_idx][127:64];
    assign a_n_lo         = a_research.entries_next[tap_idx][63:0];
    assign a_n_hi         = a_research.entries_next[tap_idx][127:64];
    assign a_update       = a_research.update;
    assign a_invalidate   = a_research.invalidate;
    assign a_supported    = a_research.supported;

    assign b_replace_mask = b_research.replace_mask;
    assign b_plru         = b_research.plru;
    assign b_supported    = b_research.supported;

endmodule

`endif // COMPOSITION_IOTLB_TOP_SV
