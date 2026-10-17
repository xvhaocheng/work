// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Verification only: real candidate instances and the byte-frozen 85b gold.
// Hierarchical reads below never feed back into a production DUT.
module p_iotlb_top (
    input logic clk_i, rst_ni,
    input logic policy_clear_i,
    input logic [15:0] policy_mask_i,
    input logic update_i, lookup_i,
    input logic [28:0] update_vpn_i, lookup_vpn_i, inval_vpn_i,
    input logic [19:0] pscid_i, inval_pscid_i,
    input logic [15:0] gscid_i, inval_gscid_i,
    input logic [43:0] ppn_i,
    input logic [7:0] flags_i,
    input logic en1_i, en2_i,
    input logic inval_vma_i, inval_gvma_i, inval_av_i, inval_gv_i, inval_pscv_i,
    output logic off_equal_o, full_equal_o, p_equal_o,
    output logic p_hit_o, gold_hit_o,
    output logic [14:0] p_plru_o, p_plru_next_o, shared_plru_o,
    output logic [15:0] p_victim_o, shared_victim_o, p_valid_o, p_valid_next_o
);
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{default:'0, NumIotlbEntries:16,
        MSITrans:rv_iommu_cfg::MSI_DISABLED, IGS:rv_iommu_cfg::MSI_ONLY};
    rv_iommu::iotlb_up_t up;
    rv_iommu::iotlb_inval_t inval;
    rv_iommu::iotlb_content_t p_content, off_content, full_content, gold_content;
    logic off_hit, full_hit;
    always_comb begin
        up = '0;
        up.update = update_i; up.vpn = update_vpn_i;
        up.pscid = pscid_i; up.gscid = gscid_i;
        up.content.content_1S.ppn = ppn_i;
        up.content.content_1S.is_1G = flags_i[7];
        up.content.content_1S.is_2M = flags_i[6];
        up.content.content_1S.g = flags_i[5];
        up.content.content_1S.u = flags_i[4];
        up.content.content_1S.x = flags_i[3];
        up.content.content_1S.w = flags_i[2];
        up.content.content_1S.r = flags_i[1];
        up.content.content_1S.v = flags_i[0];
        up.content.content_2S = up.content.content_1S;
        inval = '0;
        inval.inval_vma = inval_vma_i; inval.inval_gvma = inval_gvma_i;
        inval.av = inval_av_i; inval.gv = inval_gv_i; inval.pscv = inval_pscv_i;
        inval.vpn = inval_vpn_i; inval.pscid = inval_pscid_i; inval.gscid = inval_gscid_i;
    end
    `define COMMON .clk_i, .rst_ni, .inval_i(inval), .update_i(up), .lookup_i,
    `define LOOKUP .lu_vpn_i(lookup_vpn_i), .lu_pscid_i(pscid_i), .lu_gscid_i(gscid_i), .en_1S_i(en1_i), .en_2S_i(en2_i)
    rv_iommu_iotlb #(.RVIOMMUCfg(Cfg), .PolicyEnable(1), .PartitionLocalHistory(1)) p (
        `COMMON `LOOKUP, .policy_clear_i, .policy_mask_i,
        .lu_hit_o(p_hit_o), .lu_content_o(p_content));
    rv_iommu_iotlb #(.RVIOMMUCfg(Cfg), .PolicyEnable(0), .PartitionLocalHistory(1)) off (
        `COMMON `LOOKUP, .policy_clear_i, .policy_mask_i,
        .lu_hit_o(off_hit), .lu_content_o(off_content));
    rv_iommu_iotlb #(.RVIOMMUCfg(Cfg), .PolicyEnable(1), .PartitionLocalHistory(1)) full (
        `COMMON `LOOKUP, .policy_clear_i(1'b0), .policy_mask_i(16'hffff),
        .lu_hit_o(full_hit), .lu_content_o(full_content));
    rv_iommu_iotlb #(.RVIOMMUCfg(Cfg), .PolicyEnable(1), .PartitionLocalHistory(0)) shared (
        `COMMON `LOOKUP, .policy_clear_i, .policy_mask_i,
        .lu_hit_o(), .lu_content_o());
    rv_iommu_iotlb_gold #(.RVIOMMUCfg(Cfg)) gold (
        `COMMON `LOOKUP, .lu_hit_o(gold_hit_o), .lu_content_o(gold_content));
    `undef COMMON
    `undef LOOKUP
    assign off_equal_o = off_hit == gold_hit_o && off_content == gold_content &&
        off.tags_q == gold.tags_q && off.tags_n == gold.tags_n &&
        off.content_q == gold.content_q && off.content_n == gold.content_n &&
        off.plru_tree_q == gold.plru_tree_q && off.plru_tree_n == gold.plru_tree_n &&
        off.replace_en == gold.replace_en;
    assign full_equal_o = full_hit == gold_hit_o && full_content == gold_content &&
        full.tags_q == gold.tags_q && full.tags_n == gold.tags_n &&
        full.content_q == gold.content_q && full.content_n == gold.content_n &&
        full.plru_tree_q == gold.plru_tree_q && full.plru_tree_n == gold.plru_tree_n &&
        full.replace_en == gold.replace_en;
    assign p_equal_o = p_hit_o == gold_hit_o && p_content == gold_content &&
        p.tags_q == gold.tags_q && p.tags_n == gold.tags_n &&
        p.content_q == gold.content_q && p.content_n == gold.content_n &&
        p.plru_tree_q == gold.plru_tree_q && p.plru_tree_n == gold.plru_tree_n &&
        p.replace_en == gold.replace_en;
    assign p_plru_o = p.plru_tree_q;
    assign p_plru_next_o = p.plru_tree_n;
    assign shared_plru_o = shared.plru_tree_q;
    assign p_victim_o = p.replace_en;
    assign shared_victim_o = shared.replace_en;
    for (genvar i=0; i<16; i++) begin : gen_valid_taps
        assign p_valid_o[i] = p.tags_q[i].valid;
        assign p_valid_next_o[i] = p.tags_n[i].valid;
    end
endmodule
