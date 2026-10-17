`include "common_cells/registers.svh"
`include "idma/typedef.svh"
// Actual frontend/gold; backend readiness/completion are unit inputs only.
module frontend_tap_top (
    input logic clk_i, rst_ni, valid_i, write_i, ready_i, complete_i, backend_idle_i,
    input logic [7:0] addr_i, wstrb_i,
    input logic [63:0] wdata_i,
    output logic gold_match_o, default_zero_o, stop_o, quiet_o,
    output logic [63:0] rdata_o, src_o, dst_o, issue_id_o, done_id_o,
    output logic ready_o, error_o, offer_o, issue_o, done_o, active_o, raw_launch_o, retire_o,
    output logic [1:0] state_o, next_state_o,
    output logic [31:0] gap_remaining_o,
    output logic [32:0] issued_o, completed_o
);
    typedef struct packed {logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;} req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef struct packed {
        logic issue, done, poll, active;
        logic [63:0] issue_id, done_id, poll_id;
        logic cfg_valid, cfg_write;
        logic [6:0] cfg_addr;
        logic [63:0] cfg_wdata;
        logic [7:0] cfg_wstrb;
    } events_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)
    req_t req;
    rsp_t rsp[3]; burst_t burst[3]; events_t events[3];
    logic offer[3]; logic [1:0] ttype[3]; logic [63:0] special[3];
    logic stop_off, quiet_off;
    assign req = {valid_i, write_i, addr_i, wdata_i, wstrb_i};
`define PORTS(I) \
    .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp[I]), \
    .burst_req_o(burst[I]), .valid_o(offer[I]), .ready_i, .backend_idle_i, .trans_complete_i(complete_i), \
    .ttype_o(ttype[I]), .wdata_o(special[I]), .rdata_i(64'h123456789abcdef0), .rdata_valid_i(1'b0), \
    .batch_active_o(events[I].active), .obs_issue_o(events[I].issue), .obs_done_o(events[I].done), .obs_poll_o(events[I].poll), \
    .obs_issue_id_o(events[I].issue_id), .obs_done_id_o(events[I].done_id), .obs_poll_id_o(events[I].poll_id), \
    .obs_cfg_valid_o(events[I].cfg_valid), .obs_cfg_write_o(events[I].cfg_write), .obs_cfg_addr_o(events[I].cfg_addr), \
    .obs_cfg_wdata_o(events[I].cfg_wdata), .obs_cfg_wstrb_o(events[I].cfg_wstrb), .obs_cfg_rdata_i(64'h49444d414f425303)
    idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t), .burst_req_t(burst_t),
        .AxiIdWidth(4), .ObserverPort(1), .BatchPort(1), .PnGapEnable(1), .PnTapEnable(1)) dut (
        `PORTS(0), .pn_stop_accepted_o(stop_o), .pn_frontend_quiet_o(quiet_o));
    idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t), .burst_req_t(burst_t),
        .AxiIdWidth(4), .ObserverPort(1), .BatchPort(1), .PnGapEnable(1)) default_dut (
        `PORTS(1), .pn_stop_accepted_o(stop_off), .pn_frontend_quiet_o(quiet_off));
    idma_reg64_frontend_gold #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t), .burst_req_t(burst_t),
        .AxiIdWidth(4), .ObserverPort(1), .BatchPort(1), .PnGapEnable(1)) gold (`PORTS(2));
`undef PORTS
    assign gold_match_o = rsp[0] == rsp[2] && rsp[1] == rsp[2] &&
        burst[0] == burst[2] && burst[1] == burst[2] &&
        events[0] == events[2] && events[1] == events[2] &&
        offer[0] == offer[2] && offer[1] == offer[2] &&
        ttype[0] == ttype[2] && ttype[1] == ttype[2] && special[0] == special[2] && special[1] == special[2] &&
        dut.state_q == gold.state_q && default_dut.state_q == gold.state_q &&
        dut.state_n == gold.state_n && default_dut.state_n == gold.state_n &&
        dut.batch_issued_q == gold.batch_issued_q && default_dut.batch_issued_q == gold.batch_issued_q &&
        dut.batch_completed_q == gold.batch_completed_q && default_dut.batch_completed_q == gold.batch_completed_q &&
        dut.batch_error_q == gold.batch_error_q && default_dut.batch_error_q == gold.batch_error_q &&
        dut.batch_stop_q == gold.batch_stop_q && default_dut.batch_stop_q == gold.batch_stop_q &&
        dut.gap_remaining_q == gold.gap_remaining_q && default_dut.gap_remaining_q == gold.gap_remaining_q;
    assign default_zero_o = !(stop_off || quiet_off);
    assign rdata_o = rsp[0].rdata; assign ready_o = rsp[0].ready; assign error_o = rsp[0].error;
    assign src_o = burst[0].src_addr; assign dst_o = burst[0].dst_addr;
    assign offer_o = offer[0]; assign issue_o = events[0].issue; assign done_o = events[0].done;
    assign active_o = events[0].active; assign issue_id_o = events[0].issue_id; assign done_id_o = events[0].done_id;
    assign state_o = dut.state_q; assign next_state_o = dut.state_n;
    assign gap_remaining_o = dut.gap_remaining_q;
    assign raw_launch_o = dut.issue; assign retire_o = dut.trans_complete;
    assign issued_o = dut.batch_issued_q; assign completed_o = dut.batch_completed_q;
endmodule
