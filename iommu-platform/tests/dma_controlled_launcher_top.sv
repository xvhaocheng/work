module dma_controlled_launcher_top #(
    parameter int unsigned NumPorts = 4
) (
    input  logic clk_i, rst_ni,
    input  logic cfg_valid_i, cfg_write_i,
    input  logic [7:0] cfg_addr_i,
    input  logic [63:0] cfg_wdata_i,
    input  logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    input  logic [NumPorts-1:0] frontend_held_i,
    input  logic [NumPorts-1:0] frontend_active_i,
    input  logic [NumPorts-1:0] frontend_cancelled_i,
    input  logic [NumPorts-1:0] frontend_error_i,
    input  logic [NumPorts-1:0] obs_issue_i, obs_done_i,
    input  logic [NumPorts*64-1:0] obs_issue_id_i_flat,
    input  logic [NumPorts*64-1:0] obs_done_id_i_flat,
    input  logic selected_i, select_write_i,
    input  logic [7:0] select_id_i,
    input  logic [23:0] select_did_i,
    input  logic [63:0] select_iova_i,
    output logic [NumPorts-1:0] launch_hold_o,
    output logic [NumPorts-1:0] launch_go_o,
    output logic [NumPorts-1:0] launch_cancel_o
);
    logic [63:0] obs_issue_id_i [NumPorts];
    logic [63:0] obs_done_id_i [NumPorts];

    for (genvar p = 0; p < NumPorts; p++) begin : gen_unflatten
        assign obs_issue_id_i[p] = obs_issue_id_i_flat[p*64 +: 64];
        assign obs_done_id_i[p]  = obs_done_id_i_flat[p*64 +: 64];
    end

    dma_controlled_launcher #(.NumPorts(NumPorts)) i_dut (
        .clk_i, .rst_ni,
        .cfg_valid_i, .cfg_write_i, .cfg_addr_i, .cfg_wdata_i, .cfg_wstrb_i, .cfg_rdata_o,
        .frontend_held_i, .frontend_active_i, .frontend_cancelled_i, .frontend_error_i,
        .obs_issue_i, .obs_done_i, .obs_issue_id_i, .obs_done_id_i,
        .selected_i, .select_write_i, .select_id_i, .select_did_i, .select_iova_i,
        .launch_hold_o, .launch_go_o, .launch_cancel_o
    );
endmodule
