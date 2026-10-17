`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module gap_frontend_top (
    input logic clk_i, rst_ni, valid_i, write_i,
    input logic [7:0] addr_i, wstrb_i,
    input logic [63:0] wdata_i,
    input logic ready_i, complete_i,
    output logic [63:0] rdata_o,
    output logic ready_o, error_o, disabled_equal_o, zero_equal_o,
    output logic request_o, issue_o, done_o, batch_active_o, gap_active_o,
    output logic [31:0] gap_cycles_o, countdown_o,
    output logic [32:0] target_o,
    output logic [63:0] issue_id_o, done_id_o, length_o, src_o, dst_o,
    output logic [1:0] type_o
);
    typedef struct packed {logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;} req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)
    req_t req;
    rsp_t rsp[3];
    burst_t burst[3];
    logic [2:0] request, issue, done, poll, active, cfg_valid, cfg_write;
    logic [63:0] issue_id[3], done_id[3], poll_id[3], wdata[3], cfg_wdata[3];
    logic [7:0] cfg_wstrb[3];
    logic [6:0] cfg_addr[3];
    logic [1:0] ttype[3];
    assign req = {valid_i, write_i, addr_i, wdata_i, wstrb_i};
    for (genvar n=0; n<3; n++) begin : instances
        if (n==0) begin : reference
            idma_reg64_frontend_gold #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
                .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1), .BatchPort(1)) dut (
                .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp[n]),
                .burst_req_o(burst[n]), .valid_o(request[n]), .ready_i,
                .backend_idle_i(!request[n]), .trans_complete_i(complete_i),
                .ttype_o(ttype[n]), .wdata_o(wdata[n]), .rdata_i('0), .rdata_valid_i(1'b0),
                .batch_active_o(active[n]), .obs_issue_o(issue[n]), .obs_done_o(done[n]), .obs_poll_o(poll[n]),
                .obs_issue_id_o(issue_id[n]), .obs_done_id_o(done_id[n]), .obs_poll_id_o(poll_id[n]),
                .obs_cfg_valid_o(cfg_valid[n]), .obs_cfg_write_o(cfg_write[n]), .obs_cfg_addr_o(cfg_addr[n]),
                .obs_cfg_wdata_o(cfg_wdata[n]), .obs_cfg_wstrb_o(cfg_wstrb[n]), .obs_cfg_rdata_i(64'h49444d414f425304));
        end else begin : candidate
            idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
                .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1), .BatchPort(1),
                .BatchGapEnable(n==2)) dut (
                .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp[n]),
                .burst_req_o(burst[n]), .valid_o(request[n]), .ready_i,
                .backend_idle_i(!request[n]), .trans_complete_i(complete_i),
                .ttype_o(ttype[n]), .wdata_o(wdata[n]), .rdata_i('0), .rdata_valid_i(1'b0),
                .batch_active_o(active[n]), .obs_issue_o(issue[n]), .obs_done_o(done[n]), .obs_poll_o(poll[n]),
                .obs_issue_id_o(issue_id[n]), .obs_done_id_o(done_id[n]), .obs_poll_id_o(poll_id[n]),
                .obs_cfg_valid_o(cfg_valid[n]), .obs_cfg_write_o(cfg_write[n]), .obs_cfg_addr_o(cfg_addr[n]),
                .obs_cfg_wdata_o(cfg_wdata[n]), .obs_cfg_wstrb_o(cfg_wstrb[n]), .obs_cfg_rdata_i(64'h49444d414f425304),
                .batch_gap_active_o(), .batch_gap_cycles_o(), .batch_target_o());
        end
    end
    logic [1:0] equal;
    for (genvar n=1; n<3; n++) begin : comparisons
        assign equal[n-1] = rsp[n]==rsp[0] && burst[n]==burst[0] && request[n]==request[0] &&
            ttype[n]==ttype[0] && wdata[n]==wdata[0] && active[n]==active[0] &&
            issue[n]==issue[0] && done[n]==done[0] && poll[n]==poll[0] &&
            issue_id[n]==issue_id[0] && done_id[n]==done_id[0] && poll_id[n]==poll_id[0] &&
            cfg_valid[n]==cfg_valid[0] && cfg_write[n]==cfg_write[0] && cfg_addr[n]==cfg_addr[0] &&
            cfg_wdata[n]==cfg_wdata[0] && cfg_wstrb[n]==cfg_wstrb[0];
    end
    assign disabled_equal_o=equal[0];
    assign zero_equal_o=equal[1];
    assign rdata_o=rsp[2].rdata;
    assign ready_o=rsp[2].ready;
    assign error_o=rsp[2].error;
    assign request_o=request[2];
    assign issue_o=issue[2];
    assign done_o=done[2];
    assign batch_active_o=active[2];
    assign issue_id_o=issue_id[2];
    assign done_id_o=done_id[2];
    assign length_o=burst[2].length;
    assign src_o=burst[2].src_addr;
    assign dst_o=burst[2].dst_addr;
    assign type_o=ttype[2];
    // Verification-only probes into the real candidate's state/registers.
    assign gap_active_o=instances[2].candidate.dut.batch_gap_active_o;
    assign gap_cycles_o=instances[2].candidate.dut.batch_gap_cycles_o;
    assign target_o=instances[2].candidate.dut.batch_target_o;
    assign countdown_o=instances[2].candidate.dut.gap_countdown_q;
endmodule
