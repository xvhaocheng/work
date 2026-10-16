`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module idma_research_policy_top (
    input  logic clk_i, rst_ni,
    // Frontend register slave.
    input  logic valid_i, write_i,
    input  logic [7:0] addr_i,
    input  logic [63:0] wdata_i,
    input  logic [7:0] wstrb_i,
    input  logic ready_i, complete_i, backend_idle_i,
    input  logic batch_reverse_i,
    output logic [63:0] rdata_o,
    output logic ready_o, error_o,
    output logic request_o,
    output logic [63:0] length_o, src_o, dst_o,
    output logic [1:0] type_o,
    output logic batch_active_o,
    output logic issue_o, done_o,
    output logic [63:0] issue_id_o, done_id_o,
    // Policy register slave, full E index (0xE00..0xE5F).
    input  logic p_cfg_valid_i, p_cfg_write_i,
    input  logic [12:0] p_index_i,
    input  logic [63:0] p_wdata_i,
    output logic [63:0] p_rdata_o,
    // Policy observation inputs.
    input  logic p_quiet_i, p_supported_i,
    input  logic p_v_active_i, p_v_done_i,
    input  logic [2:0] p_ctl_state_i,
    input  logic [23:0] p_ctl_did_i,
    input  logic p_select_fire_i,
    input  logic [23:0] p_select_did_i,
    input  logic p_lookup_i, p_lookup_hit_i,
    input  logic [23:0] p_lookup_did_i,
    input  logic p_ptw_r_fire_i, p_response_error_i,
    // Policy outputs.
    output logic policy_enable_o, policy_allow_o, policy_abort_o
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

    assign req = {valid_i, write_i, addr_i, wdata_i, wstrb_i};

    idma_reg64_frontend #(
        .dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4),
        // Match the actual policy-controlled DMA1: observer registers off,
        // batch and launch ports on; launch handshake inputs remain idle.
        .ObserverPort(1'b0), .BatchPort(1'b1), .LaunchPort(1'b1),
        .ResearchPort(1'b1)
    ) u_frontend (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp),
        .burst_req_o(burst), .valid_o(request), .ready_i(ready_i),
        .backend_idle_i(backend_idle_i), .trans_complete_i(complete_i),
        .ttype_o(ttype), .wdata_o(wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o(batch_active_o),
        .policy_enable_i(policy_enable_o), .policy_allow_i(policy_allow_o),
        .policy_abort_i(policy_abort_o), .batch_reverse_i(batch_reverse_i),
        .obs_issue_o(issue_o), .obs_done_o(done_o), .obs_poll_o(),
        .obs_issue_id_o(issue_id_o), .obs_done_id_o(done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(), .obs_cfg_write_o(), .obs_cfg_addr_o(),
        .obs_cfg_wdata_o(), .obs_cfg_wstrb_o(), .obs_cfg_rdata_i('0),
        .launch_hold_i(1'b0), .launch_go_i(1'b0), .launch_cancel_i(1'b0),
        .launch_held_o(), .launch_active_o(), .launch_cancelled_o(), .launch_error_o()
    );

    dma_research_policy u_policy (
        .clk_i, .rst_ni,
        .cfg_valid_i(p_cfg_valid_i), .cfg_write_i(p_cfg_write_i),
        .index_i(p_index_i), .wdata_i(p_wdata_i), .rdata_o(p_rdata_o),
        .quiet_i(p_quiet_i), .supported_i(p_supported_i),
        .v_active_i(p_v_active_i), .a_issue_i(issue_o), .a_done_i(done_o),
        .v_done_i(p_v_done_i),
        .ctl_state_i(p_ctl_state_i), .ctl_did_i(p_ctl_did_i),
        .select_fire_i(p_select_fire_i), .select_did_i(p_select_did_i),
        .lookup_i(p_lookup_i), .lookup_hit_i(p_lookup_hit_i),
        .lookup_did_i(p_lookup_did_i),
        .ptw_r_fire_i(p_ptw_r_fire_i), .response_error_i(p_response_error_i),
        .policy_enable_o(policy_enable_o), .policy_allow_o(policy_allow_o),
        .policy_abort_o(policy_abort_o)
    );

    assign rdata_o = rsp.rdata;
    assign ready_o = rsp.ready;
    assign error_o = rsp.error;
    assign request_o = request;
    assign length_o = burst.length;
    assign src_o = burst.src_addr;
    assign dst_o = burst.dst_addr;
    assign type_o = ttype;
endmodule
