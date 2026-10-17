`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module p0_frontend_top (
    input logic clk_i, rst_ni, valid_i, write_i,
    input logic [7:0] addr_i, wstrb_i,
    input logic [63:0] wdata_i,
    input logic ready_i, complete_i,
    output logic [63:0] rdata_o,
    output logic ready_o, error_o, unchanged_o, issue_o, done_o, batch_active_o,
    output logic [63:0] issue_id_o, done_id_o, length_o, src_o, dst_o,
    output logic [1:0] type_o,
    output logic request_o
);
    typedef struct packed {
        logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;
    } req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)
    req_t req;
    rsp_t rsp, gold_rsp;
    burst_t burst, gold_burst;
    logic request, gold_request;
    logic [1:0] ttype, gold_ttype;
    logic [63:0] wdata, gold_wdata;
    assign req = {valid_i, write_i, addr_i, wdata_i, wstrb_i};
    idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1'b1), .BatchPort(1'b1)) dut (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp),
        .burst_req_o(burst), .valid_o(request), .ready_i,
        .backend_idle_i(!request), .trans_complete_i(complete_i),
        .ttype_o(ttype), .wdata_o(wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o, .obs_issue_o(issue_o), .obs_done_o(done_o), .obs_poll_o(),
        .obs_issue_id_o(issue_id_o), .obs_done_id_o(done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(), .obs_cfg_write_o(), .obs_cfg_addr_o(),
        .obs_cfg_wdata_o(), .obs_cfg_wstrb_o(), .obs_cfg_rdata_i(64'h49444d414f425303)
    );
    idma_reg64_frontend_gold #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1'b1), .BatchPort(1'b1)) gold (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(gold_rsp),
        .burst_req_o(gold_burst), .valid_o(gold_request), .ready_i,
        .backend_idle_i(!request), .trans_complete_i(complete_i),
        .ttype_o(gold_ttype), .wdata_o(gold_wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o(), .obs_issue_o(), .obs_done_o(), .obs_poll_o(),
        .obs_issue_id_o(), .obs_done_id_o(), .obs_poll_id_o(),
        .obs_cfg_valid_o(), .obs_cfg_write_o(), .obs_cfg_addr_o(),
        .obs_cfg_wdata_o(), .obs_cfg_wstrb_o(), .obs_cfg_rdata_i(64'h49444d414f425303)
    );
    assign rdata_o = rsp.rdata;
    assign ready_o = rsp.ready;
    assign error_o = rsp.error;
    assign request_o = request;
    assign length_o = burst.length;
    assign type_o = ttype;
    assign src_o = burst.src_addr;
    assign dst_o = burst.dst_addr;
    // Magic intentionally differs. All other legacy observables must match.
    assign unchanged_o = (burst == gold_burst) && (request == gold_request) &&
        (ttype == gold_ttype) && (wdata == gold_wdata) &&
        ((valid_i && addr_i == 8'h80) || rsp == gold_rsp);
endmodule
