// SPDX-License-Identifier: Solderpad-0.51
//
// Structural top for the composition-control Verilator test.
// No SV timing constructs.  The flat packed entry ports are retained for
// Verilator public access and converted internally to the unpacked arrays used
// by the production control block.
//
module composition_control_top (
    input  logic               clk_i,
    input  logic               rst_ni,
    input  logic               cfg_write_i,
    input  logic [6:0]         index_i,
    input  logic [63:0]        wdata_i,
    output logic [63:0]        rdata_o,
    input  logic               quiet_i,
    input  logic               supported_i,
    input  logic               arm_i,
    input  logic               open_i,
    input  logic               close_i,
    output logic [1:0]         mode_o,
    output logic [23:0]        protected_did_o,
    output logic               clear_o,
    input  logic               lookup_i,
    input  logic               lookup_hit_i,
    input  logic               normal_i,
    input  logic               stage1_i,
    input  logic               stage2_i,
    input  logic [23:0]        lookup_did_i,
    input  logic [19:0]        lookup_pscid_i,
    input  logic               accept_i,
    input  logic               fast_i,
    input  logic [23:0]        fast_did_i,
    input  logic [1:0]         gap_active_i,
    input  logic               update_i,
    input  logic               invalidate_i,
    input  logic [23:0]        update_did_i,
    input  logic [15:0]        replace_mask_i,
    input  logic [15:0][127:0] entries_i,
    input  logic [15:0][127:0] entries_next_i,
    input  logic [31:0]        plru_i
);
    logic [127:0] entries_unpacked_i [16];
    logic [127:0] entries_unpacked_next_i [16];

    always_comb begin
        for (int i = 0; i < 16; i++) begin
            entries_unpacked_i[i] = entries_i[i];
            entries_unpacked_next_i[i] = entries_next_i[i];
        end
    end

    dma_composition_control dut (
        .clk_i(clk_i),
        .rst_ni(rst_ni),
        .cfg_write_i(cfg_write_i),
        .index_i(index_i),
        .wdata_i(wdata_i),
        .rdata_o(rdata_o),
        .quiet_i(quiet_i),
        .supported_i(supported_i),
        .arm_i(arm_i),
        .open_i(open_i),
        .close_i(close_i),
        .mode_o(mode_o),
        .protected_did_o(protected_did_o),
        .clear_o(clear_o),
        .lookup_i(lookup_i),
        .lookup_hit_i(lookup_hit_i),
        .normal_i(normal_i),
        .stage1_i(stage1_i),
        .stage2_i(stage2_i),
        .lookup_did_i(lookup_did_i),
        .lookup_pscid_i(lookup_pscid_i),
        .accept_i(accept_i),
        .fast_i(fast_i),
        .fast_did_i(fast_did_i),
        .gap_active_i(gap_active_i),
        .update_i(update_i),
        .invalidate_i(invalidate_i),
        .update_did_i(update_did_i),
        .replace_mask_i(replace_mask_i),
        .entries_i(entries_unpacked_i),
        .entries_next_i(entries_unpacked_next_i),
        .plru_i(plru_i)
    );
endmodule