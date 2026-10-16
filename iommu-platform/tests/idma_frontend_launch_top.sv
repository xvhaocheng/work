`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module idma_frontend_launch_top (
    input logic clk_i, rst_ni,
    input logic valid_i, write_i,
    input logic [7:0] addr_i,
    input logic [63:0] wdata_i,
    input logic ready_i, complete_i, idle_i,
    input logic hold_i, go_i, cancel_i,
    input logic [63:0] obs_cfg_rdata_i,
    output logic [63:0] rdata_o,
    output logic ready_o, error_o,
    output logic issue_o, done_o, poll_o,
    output logic [63:0] issue_id_o, done_id_o, poll_id_o,
    output logic launch_held_o, launch_active_o, launch_cancelled_o, launch_error_o,
    output logic cfg_valid_o, cfg_write_o,
    output logic [7:0] cfg_addr_o,
    output logic [63:0] cfg_wdata_o,
    output logic [7:0] cfg_wstrb_o,
    output logic request_o,
    output logic [63:0] length_o,
    output logic [1:0] type_o,
    output logic [63:0] src_o, dst_o
);
    typedef struct packed {logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;} req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)
    req_t req;
    rsp_t rsp;
    burst_t burst;
    logic request;
    logic [1:0] ttype;
    logic [63:0] wdata;
    assign req = {valid_i, write_i, addr_i, wdata_i, 8'hff};
    idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1'b1), .BatchPort(1'b1), .LaunchPort(1'b1)) dut (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp),
        .burst_req_o(burst), .valid_o(request), .ready_i,
        .backend_idle_i(idle_i), .trans_complete_i(complete_i),
        .ttype_o(ttype), .wdata_o(wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o(),
        .obs_issue_o(issue_o), .obs_done_o(done_o), .obs_poll_o(poll_o),
        .obs_issue_id_o(issue_id_o), .obs_done_id_o(done_id_o), .obs_poll_id_o(poll_id_o),
        .obs_cfg_valid_o(cfg_valid_o), .obs_cfg_write_o(cfg_write_o),
        .obs_cfg_addr_o(cfg_addr_o), .obs_cfg_wdata_o(cfg_wdata_o), .obs_cfg_wstrb_o(cfg_wstrb_o),
        .obs_cfg_rdata_i(obs_cfg_rdata_i),
        .launch_hold_i(hold_i), .launch_go_i(go_i), .launch_cancel_i(cancel_i),
        .launch_held_o, .launch_active_o, .launch_cancelled_o, .launch_error_o
    );
    assign rdata_o = rsp.rdata;
    assign ready_o = rsp.ready;
    assign error_o = rsp.error;
    assign request_o = request;
    assign length_o = burst.length;
    assign type_o = ttype;
    assign src_o = burst.src_addr;
    assign dst_o = burst.dst_addr;
endmodule
