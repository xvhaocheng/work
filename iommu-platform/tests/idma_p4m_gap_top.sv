`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module idma_p4m_gap_top (
    input logic clk_i, rst_ni,
    // Production frontend control.
    input logic prod_valid_i, prod_write_i,
    input logic [7:0] prod_addr_i,
    input logic [63:0] prod_wdata_i,
    input logic [7:0] prod_wstrb_i,
    input logic prod_ready_i, prod_complete_i,
    output logic [63:0] prod_rdata_o,
    output logic prod_ready_o, prod_error_o,
    output logic prod_request_o,
    output logic [63:0] prod_length_o,
    output logic [1:0] prod_type_o,
    output logic [63:0] prod_src_o, prod_dst_o,
    output logic prod_issue_o, prod_done_o,
    output logic [63:0] prod_issue_id_o, prod_done_id_o,
    output logic prod_batch_active_o,
    output logic prod_cfg_valid_o, prod_cfg_write_o,
    output logic [7:0] prod_cfg_addr_o,
    output logic [63:0] prod_cfg_wdata_o,
    output logic [7:0] prod_cfg_wstrb_o,
    // Controlled-launch handshake, driven with identical values into both
    // frontends so the cycle-for-cycle comparison stays valid.
    input logic prod_launch_hold_i, prod_launch_go_i, prod_launch_cancel_i,
    output logic prod_launch_active_o, prod_launch_held_o, prod_launch_error_o,
    // Renamed prechange golden frontend control.
    input logic gold_valid_i, gold_write_i,
    input logic [7:0] gold_addr_i,
    input logic [63:0] gold_wdata_i,
    input logic gold_ready_i, gold_complete_i,
    output logic [63:0] gold_rdata_o,
    output logic gold_ready_o, gold_error_o,
    output logic gold_request_o,
    output logic [63:0] gold_length_o,
    output logic [1:0] gold_type_o,
    output logic [63:0] gold_src_o, gold_dst_o,
    input logic gold_launch_hold_i, gold_launch_go_i, gold_launch_cancel_i,
    // Golden batch/observer event taps for the cycle-for-cycle comparison.
    output logic gold_batch_active_o,
    output logic gold_issue_o, gold_done_o,
    output logic [63:0] gold_issue_id_o, gold_done_id_o
);
    typedef struct packed {logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;} req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)

    req_t prod_req, gold_req;
    rsp_t prod_rsp, gold_rsp;
    burst_t prod_burst, gold_burst;
    logic prod_request, gold_request;
    logic [1:0] prod_ttype, gold_ttype;
    logic [63:0] prod_wdata, gold_wdata;

    assign prod_req = {prod_valid_i, prod_write_i, prod_addr_i, prod_wdata_i, prod_wstrb_i};
    idma_reg64_frontend #(
        .dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4),
        .ObserverPort(1'b1), .BatchPort(1'b1), .LaunchPort(1'b1)
    ) dut (
        .clk_i, .rst_ni, .dma_ctrl_req_i(prod_req), .dma_ctrl_rsp_o(prod_rsp),
        .burst_req_o(prod_burst), .valid_o(prod_request), .ready_i(prod_ready_i),
        .backend_idle_i(!prod_request), .trans_complete_i(prod_complete_i),
        .ttype_o(prod_ttype), .wdata_o(prod_wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o(prod_batch_active_o),
        .obs_issue_o(prod_issue_o), .obs_done_o(prod_done_o), .obs_poll_o(),
        .obs_issue_id_o(prod_issue_id_o), .obs_done_id_o(prod_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(prod_cfg_valid_o), .obs_cfg_write_o(prod_cfg_write_o),
        .obs_cfg_addr_o(prod_cfg_addr_o), .obs_cfg_wdata_o(prod_cfg_wdata_o),
        .obs_cfg_wstrb_o(prod_cfg_wstrb_o), .obs_cfg_rdata_i(64'h49444d414f425301),
        .launch_hold_i(prod_launch_hold_i), .launch_go_i(prod_launch_go_i),
        .launch_cancel_i(prod_launch_cancel_i),
        .launch_held_o(prod_launch_held_o), .launch_active_o(prod_launch_active_o),
        .launch_cancelled_o(), .launch_error_o(prod_launch_error_o),
        .policy_enable_i(1'b0), .policy_allow_i(1'b0),
        .policy_abort_i(1'b0), .batch_reverse_i(1'b0)
    );

    assign gold_req = {gold_valid_i, gold_write_i, gold_addr_i, gold_wdata_i, 8'hff};
    idma_reg64_frontend_before #(
        .dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4),
        .ObserverPort(1'b1), .BatchPort(1'b1), .LaunchPort(1'b1)
    ) gold (
        .clk_i, .rst_ni, .dma_ctrl_req_i(gold_req), .dma_ctrl_rsp_o(gold_rsp),
        .burst_req_o(gold_burst), .valid_o(gold_request), .ready_i(gold_ready_i),
        .backend_idle_i(!gold_request), .trans_complete_i(gold_complete_i),
        .ttype_o(gold_ttype), .wdata_o(gold_wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o(gold_batch_active_o),
        .obs_issue_o(gold_issue_o), .obs_done_o(gold_done_o), .obs_poll_o(),
        .obs_issue_id_o(gold_issue_id_o), .obs_done_id_o(gold_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(), .obs_cfg_write_o(), .obs_cfg_addr_o(), .obs_cfg_wdata_o(),
        .obs_cfg_wstrb_o(), .obs_cfg_rdata_i(64'h49444d414f425301),
        .launch_hold_i(gold_launch_hold_i), .launch_go_i(gold_launch_go_i),
        .launch_cancel_i(gold_launch_cancel_i),
        .launch_held_o(), .launch_active_o(), .launch_cancelled_o(), .launch_error_o()
    );

    assign prod_rdata_o = prod_rsp.rdata;
    assign prod_ready_o = prod_rsp.ready;
    assign prod_error_o = prod_rsp.error;
    assign prod_request_o = prod_request;
    assign prod_length_o = prod_burst.length;
    assign prod_type_o = prod_ttype;
    assign prod_src_o = prod_burst.src_addr;
    assign prod_dst_o = prod_burst.dst_addr;

    assign gold_rdata_o = gold_rsp.rdata;
    assign gold_ready_o = gold_rsp.ready;
    assign gold_error_o = gold_rsp.error;
    assign gold_request_o = gold_request;
    assign gold_length_o = gold_burst.length;
    assign gold_type_o = gold_ttype;
    assign gold_src_o = gold_burst.src_addr;
    assign gold_dst_o = gold_burst.dst_addr;
endmodule
