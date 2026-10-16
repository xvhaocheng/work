// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Private unit-only protocol fixture. No SYSTEM, CPU or IOMMU instance.
`include "axi/typedef.svh"

module stream_spm_protocol_top (
    input logic clk_i, rst_ni,
    input logic aw_valid_i,
    input logic [5:0] aw_id_i,
    input logic [63:0] aw_addr_i,
    input logic [7:0] aw_len_i,
    output logic aw_ready_o,
    input logic w_valid_i,
    input logic [63:0] w_data_i,
    input logic [7:0] w_strb_i,
    input logic w_last_i,
    output logic w_ready_o,
    input logic b_ready_i,
    output logic b_valid_o,
    output logic [5:0] b_id_o,
    output logic [1:0] b_resp_o,
    output logic b_user_o,
    input logic ar_valid_i,
    input logic [5:0] ar_id_i,
    input logic [63:0] ar_addr_i,
    input logic [7:0] ar_len_i,
    output logic ar_ready_o,
    input logic r_ready_i,
    output logic r_valid_o,
    output logic [5:0] r_id_o,
    output logic [63:0] r_data_o,
    output logic [1:0] r_resp_o,
    output logic r_last_o, r_user_o,
    output logic spm_busy_o, comp_busy_o, comp_cmd_ready_o, comp_done_o,
    output logic [31:0] comp_fault_o,
    output logic mac_valid_o,
    output logic mem_grant_valid_o, mem_grant_owner_o, mem_grant_we_o,
    output logic [63:0] mem_grant_addr_o, mem_grant_wdata_o,
    output logic [7:0] mem_grant_strb_o,
    output logic mem_rsp_valid_o, mem_rsp_owner_o, mem_rsp_we_o, mem_rsp_err_o,
    output logic [63:0] mem_rsp_addr_o, mem_rsp_rdata_o
);
    typedef logic [63:0] addr_t;
    typedef logic [63:0] data_t;
    typedef logic [5:0] id_t;
    typedef logic [7:0] strb_t;
    typedef logic [0:0] user_t;
    `AXI_TYPEDEF_AW_CHAN_T(aw_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_W_CHAN_T(w_t, data_t, strb_t, user_t)
    `AXI_TYPEDEF_B_CHAN_T(b_t, id_t, user_t)
    `AXI_TYPEDEF_AR_CHAN_T(ar_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T(r_t, data_t, id_t, user_t)
    `AXI_TYPEDEF_REQ_T(req_t, aw_t, w_t, ar_t)
    `AXI_TYPEDEF_RESP_T(resp_t, b_t, r_t)
    req_t req;
    resp_t resp;

    always_comb begin
        req = '0;
        req.aw_valid = aw_valid_i;
        req.aw.id = aw_id_i;
        req.aw.addr = aw_addr_i;
        req.aw.len = aw_len_i;
        req.aw.size = 3'd3;
        req.aw.burst = axi_pkg::BURST_INCR;
        req.w_valid = w_valid_i;
        req.w.data = w_data_i;
        req.w.strb = w_strb_i;
        req.w.last = w_last_i;
        req.b_ready = b_ready_i;
        req.ar_valid = ar_valid_i;
        req.ar.id = ar_id_i;
        req.ar.addr = ar_addr_i;
        req.ar.len = ar_len_i;
        req.ar.size = 3'd3;
        req.ar.burst = axi_pkg::BURST_INCR;
        req.r_ready = r_ready_i;
    end
    assign aw_ready_o = resp.aw_ready;
    assign w_ready_o = resp.w_ready;
    assign b_valid_o = resp.b_valid;
    assign b_id_o = resp.b.id;
    assign b_resp_o = resp.b.resp;
    assign b_user_o = resp.b.user;
    assign ar_ready_o = resp.ar_ready;
    assign r_valid_o = resp.r_valid;
    assign r_id_o = resp.r.id;
    assign r_data_o = resp.r.data;
    assign r_resp_o = resp.r.resp;
    assign r_last_o = resp.r.last;
    assign r_user_o = resp.r.user;

    // All commands disabled; this fixture tests the frozen slave endpoint.
    stream_spm_compute #(.axi_req_t(req_t), .axi_resp_t(resp_t)) i_endpoint (
        .clk_i, .rst_ni, .axi_req_i(req), .axi_resp_o(resp),
        .comp_cmd_valid_i(1'b0), .comp_aslot_i(1'b0), .comp_bslot_i(1'b0),
        .comp_cslot_i(1'b0), .spm_pause_i(1'b0),
        .comp_s_i(8'd0), .comp_t_i(8'd0),
        .comp_agen_i(32'd0), .comp_bgen_i(32'd0), .comp_cgen_i(32'd0),
        .comp_cmd_ready_o, .comp_busy_o, .comp_done_o, .comp_fault_o, .spm_busy_o,
        .mem_grant_valid_o, .mem_grant_owner_o, .mem_grant_we_o,
        .mem_grant_addr_o, .mem_grant_wdata_o, .mem_grant_strb_o,
        .mem_rsp_valid_o, .mem_rsp_owner_o, .mem_rsp_we_o, .mem_rsp_err_o,
        .mem_rsp_addr_o, .mem_rsp_rdata_o, .mac_valid_o
    );
endmodule : stream_spm_protocol_top
