// SPDX-License-Identifier: Solderpad-0.51
//
// Structural top linking the production dma_observer (V9 profile:
// P4MEnable/ResearchEnable = 1, PathEnable/E3/Controlled = 0) to the real
// dma_composition_control block, behind the same absolute observer index bank
// that ariane_peripherals_xilinx.sv builds for IOMMUNonblockMode == 3.
//
// The legacy diagnostic observer and the old E2/COST recorder are intentionally
// NOT instantiated: for mode 3 the observer mux selects the composition
// relative bank D20..D9F and returns zero for every other relative index
// (the old research/observer bank reads 0).
//
// Only interface pins are exposed; no DUT internal register is read directly.

module composition_observer_top (
    input  logic         clk_i,
    input  logic         rst_ni,

    // Observer configuration interface
    input  logic         cfg_valid_i,
    input  logic         cfg_write_i,
    input  logic [6:0]   cfg_addr_i,
    input  logic [63:0]  cfg_wdata_i,
    input  logic [7:0]   cfg_wstrb_i,
    output logic [63:0]  cfg_rdata_o,

    // DMA job and batch events
    input  logic [3:0]   issue_i,
    input  logic [3:0]   done_i,
    input  logic [3:0]   poll_i,
    input  logic [3:0]   batch_active_i,
    input  logic [63:0]  issue_id_i [4],
    input  logic [63:0]  done_id_i  [4],
    input  logic [63:0]  poll_id_i  [4],

    // Private DMA AXI observation
    input  logic [3:0]   ar_valid_i,
    input  logic [3:0]   aw_valid_i,
    input  logic [3:0]   ar_ready_i,
    input  logic [3:0]   aw_ready_i,
    input  logic [3:0]   rlast_fire_i,
    input  logic [3:0]   b_fire_i,
    input  logic [3:0]   bad_id_i,

    // Shared IOMMU TR / completion AXI observation
    input  logic         shared_ar_valid_i,
    input  logic         shared_aw_valid_i,
    input  logic         shared_ar_ready_i,
    input  logic         shared_aw_ready_i,
    input  logic [3:0]   tr_id_i     [2],
    input  logic [3:0]   out_id_i    [2],
    input  logic [3:0]   return_id_i [2],
    input  logic [1:0]   out_valid_i,
    input  logic [1:0]   out_ready_i,
    input  logic [1:0]   return_i,

    // E3 sideband (present but unused for the composition profile)
    input  logic [63:0]  e3_data_i,
    input  logic         e3_error_i,

    // Composition observation inputs mirrored from the IOMMU research struct
    input  logic         lookup_i,
    input  logic         lookup_hit_i,
    input  logic         normal_i,
    input  logic         stage1_i,
    input  logic         stage2_i,
    input  logic [23:0]  lookup_did_i,
    input  logic [19:0]  lookup_pscid_i,
    input  logic         accept_i,
    input  logic         fast_i,
    input  logic [23:0]  fast_did_i,
    input  logic [1:0]   gap_active_i,
    input  logic         update_i,
    input  logic         invalidate_i,
    input  logic [23:0]  update_did_i,
    input  logic [15:0]  replace_mask_i,
    input  logic [31:0]  plru_i,
    // Deterministic entry patterns so the snapshot words can be verified
    // without exposing wide array ports to the C++ model.
    input  logic [63:0]  entries_seed_i,
    input  logic [63:0]  entries_next_seed_i,

    // Quiet gate and support inputs that production combines for the block
    input  logic         composition_iommu_quiet_i,
    input  logic         supported_i,
    input  logic [3:0]   job_quiet_i,
    input  logic [3:0]   launch_active_i,
    input  logic [3:0]   launch_held_i,

    // Composition control outputs (production wiring into rv_iommu_top)
    output logic [1:0]   composition_mode_o,
    output logic [23:0]  composition_did_o,
    output logic         composition_clear_o
);
    logic [63:0] research_data;
    logic [12:0] research_index;
    logic        research_write;
    logic [63:0] research_wdata;
    logic        research_quiet;
    logic        research_arm;
    logic        research_open;
    logic        research_close;

    logic        composition_access;
    logic [12:0] composition_index;
    logic [63:0] composition_data;
    logic        composition_quiet;

    logic [7:0]  e3_control;
    logic [7:0]  e3_index;

    logic [127:0] entries_pat [16];
    logic [127:0] entries_next_pat [16];

    always_comb begin
        for (int i = 0; i < 16; i++) begin
            entries_pat[i]      = { entries_seed_i      + 64'(i), 64'h1000_0000_0000_0000 + 64'(i) };
            entries_next_pat[i] = { entries_next_seed_i + 64'(i), 64'h2000_0000_0000_0000 + 64'(i) };
        end
    end

    // New-profile data mux, matching ariane_peripherals_xilinx.sv for
    // IOMMUNonblockMode == 3.  The legacy observer, policy and nonblock banks are
    // not built in this profile, so every non-composition index reads zero.
    assign composition_access = (research_index >= 13'hD20) && (research_index <= 13'hD9F);
    assign composition_index  = research_index - 13'hD20;
    assign research_data      = composition_access ? composition_data : 64'd0;
    assign composition_quiet  = composition_iommu_quiet_i && research_quiet &&
                                (&job_quiet_i) && !(|issue_i) &&
                                !(|launch_active_i) && !(|launch_held_i);

    dma_observer #(
        .NumPorts        (4),
        .PathEnable      (1'b0),
        .E3Enable        (1'b0),
        .ControlledEnable(1'b0),
        .P4MEnable       (1'b1),
        .ResearchEnable  (1'b1),
        .AxiIdWidth      (4)
    ) i_observer (
        .clk_i,
        .rst_ni,
        .cfg_valid_i,
        .cfg_write_i,
        .cfg_addr_i,
        .cfg_wdata_i,
        .cfg_wstrb_i,
        .cfg_rdata_o,
        .issue_i,
        .done_i,
        .poll_i,
        .batch_active_i,
        .issue_id_i,
        .done_id_i,
        .poll_id_i,
        .ar_valid_i,
        .aw_valid_i,
        .ar_ready_i,
        .aw_ready_i,
        .rlast_fire_i,
        .b_fire_i,
        .bad_id_i,
        .shared_ar_valid_i,
        .shared_aw_valid_i,
        .shared_ar_ready_i,
        .shared_aw_ready_i,
        .tr_id_i,
        .out_id_i,
        .return_id_i,
        .out_valid_i,
        .out_ready_i,
        .return_i,
        .e3_data_i,
        .e3_error_i,
        .e3_control_o(e3_control),
        .e3_index_o(e3_index),
        .research_data_i(research_data),
        .research_index_o(research_index),
        .research_write_o(research_write),
        .research_wdata_o(research_wdata),
        .research_quiet_o(research_quiet),
        .research_arm_o(research_arm),
        .research_open_o(research_open),
        .research_close_o(research_close)
    );

    dma_composition_control i_composition (
        .clk_i,
        .rst_ni,
        .cfg_write_i(research_write && composition_access),
        .index_i(composition_index[6:0]),
        .wdata_i(research_wdata),
        .rdata_o(composition_data),
        .quiet_i(composition_quiet),
        .supported_i,
        .arm_i(research_arm),
        .open_i(research_open),
        .close_i(research_close),
        .mode_o(composition_mode_o),
        .protected_did_o(composition_did_o),
        .clear_o(composition_clear_o),
        .lookup_i,
        .lookup_hit_i,
        .normal_i,
        .stage1_i,
        .stage2_i,
        .lookup_did_i,
        .lookup_pscid_i,
        .accept_i,
        .fast_i,
        .fast_did_i,
        .gap_active_i,
        .update_i,
        .invalidate_i,
        .update_did_i,
        .replace_mask_i,
        .entries_i(entries_pat),
        .entries_next_i(entries_next_pat),
        .plru_i
    );
endmodule
