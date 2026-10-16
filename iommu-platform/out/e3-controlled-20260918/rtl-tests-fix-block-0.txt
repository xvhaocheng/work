`include "common_cells/registers.svh"
`include "idma/typedef.svh"

module idma_controlled_pair_top (
    input  logic clk_i, rst_ni,
    // CPU config for V frontend (port 0)
    input  logic v_cpu_valid_i, v_cpu_write_i,
    input  logic [31:0] v_cpu_addr_i,
    input  logic [63:0] v_cpu_wdata_i,
    input  logic [7:0] v_cpu_wstrb_i,
    output logic v_cpu_ready_o,
    output logic [63:0] v_cpu_rdata_o,
    output logic v_cpu_error_o,
    // CPU config for A frontend (port 1)
    input  logic a_cpu_valid_i, a_cpu_write_i,
    input  logic [31:0] a_cpu_addr_i,
    input  logic [63:0] a_cpu_wdata_i,
    input  logic [7:0] a_cpu_wstrb_i,
    output logic a_cpu_ready_o,
    output logic [63:0] a_cpu_rdata_o,
    output logic a_cpu_error_o,
    // Backend controls/outputs for V
    input  logic v_backend_ready_i,
    input  logic v_backend_idle_i,
    input  logic v_trans_complete_i,
    output logic v_valid_o,
    output logic [63:0] v_burst_src_addr_o,
    output logic [63:0] v_burst_dst_addr_o,
    output logic [63:0] v_burst_length_o,
    output logic [63:0] v_issue_id_o,
    output logic [63:0] v_done_id_o,
    output logic v_obs_issue_o,
    output logic v_obs_done_o,
    // Backend controls/outputs for A
    input  logic a_backend_ready_i,
    input  logic a_backend_idle_i,
    input  logic a_trans_complete_i,
    output logic a_valid_o,
    output logic [63:0] a_burst_src_addr_o,
    output logic [63:0] a_burst_dst_addr_o,
    output logic [63:0] a_burst_length_o,
    output logic [63:0] a_issue_id_o,
    output logic [63:0] a_done_id_o,
    output logic a_obs_issue_o,
    output logic a_obs_done_o,
    // Launcher status/select
    input  logic selected_i,
    input  logic select_write_i,
    input  logic [7:0] select_id_i,
    input  logic [23:0] select_did_i,
    input  logic [63:0] select_iova_i,
    output logic [3:0] launch_hold_o,
    output logic [3:0] launch_go_o,
    output logic [3:0] launch_cancel_o,
    output logic [3:0] frontend_held_o,
    output logic [3:0] frontend_active_o,
    output logic [3:0] frontend_cancelled_o,
    output logic [3:0] frontend_error_o,
    output logic [63:0] launcher_rdata_o,
    output logic launcher_valid_o,
    output logic launcher_write_o,
    output logic [7:0] launcher_addr_o
);
    typedef struct packed {
        logic        valid;
        logic        write;
        logic [31:0] addr;
        logic [63:0] wdata;
        logic [7:0]  wstrb;
    } dma_req_t;

    typedef struct packed {
        logic        ready;
        logic [63:0] rdata;
        logic        error;
    } dma_rsp_t;

    typedef logic [63:0] addr_t;
    typedef logic [3:0]  id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)

    dma_req_t v_req, a_req;
    dma_rsp_t v_rsp, a_rsp;
    burst_t v_burst_req, a_burst_req;

    logic v_obs_cfg_valid, v_obs_cfg_write;
    logic [7:0] v_obs_cfg_addr;
    logic [63:0] v_obs_cfg_wdata, v_obs_cfg_rdata;
    logic [7:0] v_obs_cfg_wstrb;
    logic a_obs_cfg_valid, a_obs_cfg_write;
    logic [7:0] a_obs_cfg_addr;
    logic [63:0] a_obs_cfg_wdata, a_obs_cfg_rdata;
    logic [7:0] a_obs_cfg_wstrb;

    logic v_launch_held, v_launch_active, v_launch_cancelled, v_launch_error;
    logic a_launch_held, a_launch_active, a_launch_cancelled, a_launch_error;
    logic [3:0] fe_held, fe_active, fe_cancelled, fe_error;
    logic [3:0] launcher_hold, launcher_go, launcher_cancel;
    logic [63:0] launcher_issue_id [4];
    logic [63:0] launcher_done_id [4];

    assign v_req.valid = v_cpu_valid_i;
    assign v_req.write = v_cpu_write_i;
    assign v_req.addr = v_cpu_addr_i;
    assign v_req.wdata = v_cpu_wdata_i;
    assign v_req.wstrb = v_cpu_wstrb_i;
    assign v_cpu_ready_o = v_rsp.ready;
    assign v_cpu_rdata_o = v_rsp.rdata;
    assign v_cpu_error_o = v_rsp.error;

    assign a_req.valid = a_cpu_valid_i;
    assign a_req.write = a_cpu_write_i;
    assign a_req.addr = a_cpu_addr_i;
    assign a_req.wdata = a_cpu_wdata_i;
    assign a_req.wstrb = a_cpu_wstrb_i;
    assign a_cpu_ready_o = a_rsp.ready;
    assign a_cpu_rdata_o = a_rsp.rdata;
    assign a_cpu_error_o = a_rsp.error;

    assign v_obs_cfg_rdata = v_obs_cfg_addr[7] ? launcher_rdata_o : 64'h0;
    assign a_obs_cfg_rdata = 64'h0;

    idma_reg64_frontend #(
        .dma_regs_req_t(dma_req_t),
        .dma_regs_rsp_t(dma_rsp_t),
        .burst_req_t(burst_t),
        .ObserverPort(1'b1),
        .BatchPort(1'b0),
        .LaunchPort(1'b1),
        .DEVICE_ID(24'd10),
        .PROCESS_ID(20'd20),
        .AxiIdWidth(4),
        .AxID(4'd0)
    ) i_v_frontend (
        .clk_i, .rst_ni,
        .obs_issue_o(v_obs_issue_o), .obs_done_o(v_obs_done_o), .obs_poll_o(),
        .batch_active_o(),
        .obs_issue_id_o(v_issue_id_o), .obs_done_id_o(v_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(v_obs_cfg_valid), .obs_cfg_write_o(v_obs_cfg_write),
        .obs_cfg_addr_o(v_obs_cfg_addr), .obs_cfg_wdata_o(v_obs_cfg_wdata),
        .obs_cfg_wstrb_o(v_obs_cfg_wstrb), .obs_cfg_rdata_i(v_obs_cfg_rdata),
        .dma_ctrl_req_i(v_req), .dma_ctrl_rsp_o(v_rsp),
        .burst_req_o(v_burst_req), .valid_o(v_valid_o),
        .ready_i(v_backend_ready_i), .backend_idle_i(v_backend_idle_i),
        .trans_complete_i(v_trans_complete_i),
        .ttype_o(), .wdata_o(), .rdata_i(64'h0), .rdata_valid_i(1'b0),
        .launch_hold_i(launcher_hold[0]), .launch_go_i(launcher_go[0]),
        .launch_cancel_i(launcher_cancel[0]),
        .launch_held_o(v_launch_held), .launch_active_o(v_launch_active),
        .launch_cancelled_o(v_launch_cancelled), .launch_error_o(v_launch_error)
    );

    idma_reg64_frontend #(
        .dma_regs_req_t(dma_req_t),
        .dma_regs_rsp_t(dma_rsp_t),
        .burst_req_t(burst_t),
        .ObserverPort(1'b0),
        .BatchPort(1'b0),
        .LaunchPort(1'b1),
        .DEVICE_ID(24'd11),
        .PROCESS_ID(20'd21),
        .AxiIdWidth(4),
        .AxID(4'd0)
    ) i_a_frontend (
        .clk_i, .rst_ni,
        .obs_issue_o(a_obs_issue_o), .obs_done_o(a_obs_done_o), .obs_poll_o(),
        .batch_active_o(),
        .obs_issue_id_o(a_issue_id_o), .obs_done_id_o(a_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(a_obs_cfg_valid), .obs_cfg_write_o(a_obs_cfg_write),
        .obs_cfg_addr_o(a_obs_cfg_addr), .obs_cfg_wdata_o(a_obs_cfg_wdata),
        .obs_cfg_wstrb_o(a_obs_cfg_wstrb), .obs_cfg_rdata_i(a_obs_cfg_rdata),
        .dma_ctrl_req_i(a_req), .dma_ctrl_rsp_o(a_rsp),
        .burst_req_o(a_burst_req), .valid_o(a_valid_o),
        .ready_i(a_backend_ready_i), .backend_idle_i(a_backend_idle_i),
        .trans_complete_i(a_trans_complete_i),
        .ttype_o(), .wdata_o(), .rdata_i(64'h0), .rdata_valid_i(1'b0),
        .launch_hold_i(launcher_hold[1]), .launch_go_i(launcher_go[1]),
        .launch_cancel_i(launcher_cancel[1]),
        .launch_held_o(a_launch_held), .launch_active_o(a_launch_active),
        .launch_cancelled_o(a_launch_cancelled), .launch_error_o(a_launch_error)
    );

    assign fe_held = {2'b0, a_launch_held, v_launch_held};
    assign fe_active = {2'b0, a_launch_active, v_launch_active};
    assign fe_cancelled = {2'b0, a_launch_cancelled, v_launch_cancelled};
    assign fe_error = {2'b0, a_launch_error, v_launch_error};
    assign frontend_held_o = fe_held;
    assign frontend_active_o = fe_active;
    assign frontend_cancelled_o = fe_cancelled;
    assign frontend_error_o = fe_error;

    assign launcher_issue_id[0] = v_issue_id_o;
    assign launcher_issue_id[1] = a_issue_id_o;
    assign launcher_issue_id[2] = '0;
    assign launcher_issue_id[3] = '0;
    assign launcher_done_id[0] = v_done_id_o;
    assign launcher_done_id[1] = a_done_id_o;
    assign launcher_done_id[2] = '0;
    assign launcher_done_id[3] = '0;

    assign launcher_valid_o = v_obs_cfg_valid && v_obs_cfg_addr[7];
    assign launcher_write_o = v_obs_cfg_write;
    assign launcher_addr_o = v_obs_cfg_addr;

    dma_controlled_launcher #(.NumPorts(4)) i_launcher (
        .clk_i, .rst_ni,
        .cfg_valid_i(v_obs_cfg_valid && v_obs_cfg_addr[7]),
        .cfg_write_i(v_obs_cfg_write),
        .cfg_addr_i(v_obs_cfg_addr),
        .cfg_wdata_i(v_obs_cfg_wdata),
        .cfg_wstrb_i(v_obs_cfg_wstrb),
        .cfg_rdata_o(launcher_rdata_o),
        .frontend_held_i(fe_held),
        .frontend_active_i(fe_active),
        .frontend_cancelled_i(fe_cancelled),
        .frontend_error_i(fe_error),
        .obs_issue_i({2'b0, a_obs_issue_o, v_obs_issue_o}),
        .obs_done_i({2'b0, a_obs_done_o, v_obs_done_o}),
        .obs_issue_id_i(launcher_issue_id),
        .obs_done_id_i(launcher_done_id),
        .selected_i(selected_i),
        .select_write_i(select_write_i),
        .select_id_i(select_id_i),
        .select_did_i(select_did_i),
        .select_iova_i(select_iova_i),
        .launch_hold_o(launcher_hold),
        .launch_go_o(launcher_go),
        .launch_cancel_o(launcher_cancel)
    );

    assign launch_hold_o = launcher_hold;
    assign launch_go_o = launcher_go;
    assign launch_cancel_o = launcher_cancel;

    assign v_burst_src_addr_o = v_burst_req.src_addr;
    assign v_burst_dst_addr_o = v_burst_req.dst_addr;
    assign v_burst_length_o = v_burst_req.length;
    assign a_burst_src_addr_o = a_burst_req.src_addr;
    assign a_burst_dst_addr_o = a_burst_req.dst_addr;
    assign a_burst_length_o = a_burst_req.length;
endmodule
