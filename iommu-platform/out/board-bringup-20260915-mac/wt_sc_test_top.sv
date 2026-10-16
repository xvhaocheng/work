module wt_sc_test_top (
    input logic clk, rst_n, req,
    input logic [2:0] offset, size,
    input logic aw_ready, w_ready, b_valid, exokay,
    input logic [3:0] b_id,
    output logic req_ack, aw_valid, w_valid, b_ready, atomic_valid,
    output logic [3:0] aw_id,
    output logic [63:0] result
);
    wt_cache_pkg::dcache_req_t data_req;
    wt_cache_pkg::dcache_rtrn_t data_rsp;
    ariane_axi::req_t axi_req;
    ariane_axi::resp_t axi_rsp;
    logic rsp_valid;
    always_comb begin
        data_req = '0;
        data_req.rtype = wt_cache_pkg::DCACHE_ATOMIC_REQ;
        data_req.size = size;
        data_req.paddr = 56'h80000000 + offset;
        data_req.data = 64'h0000000200000002;
        data_req.tid = 1;
        data_req.amo_op = ariane_pkg::AMO_SC;
        axi_rsp = '0;
        axi_rsp.aw_ready = aw_ready;
        axi_rsp.w_ready = w_ready;
        axi_rsp.b_valid = b_valid;
        axi_rsp.b.id = b_id;
        axi_rsp.b.resp = exokay ? axi_pkg::RESP_EXOKAY : axi_pkg::RESP_OKAY;
    end
    assign req_ack = req & data_ack;
    logic data_ack;
    assign aw_valid = axi_req.aw_valid;
    assign w_valid = axi_req.w_valid;
    assign b_ready = axi_req.b_ready;
    assign aw_id = axi_req.aw.id;
    assign atomic_valid = rsp_valid && data_rsp.rtype == wt_cache_pkg::DCACHE_ATOMIC_ACK;
    assign result = data_rsp.data[63:0];
    wt_axi_adapter #(.AxiAddrWidth(64), .AxiDataWidth(64), .AxiIdWidth(4)) dut (
        .clk_i(clk), .rst_ni(rst_n),
        .icache_data_req_i(1'b0), .icache_data_ack_o(), .icache_data_i('0),
        .icache_rtrn_vld_o(), .icache_rtrn_o(),
        .dcache_data_req_i(req), .dcache_data_ack_o(data_ack), .dcache_data_i(data_req),
        .dcache_rtrn_vld_o(rsp_valid), .dcache_rtrn_o(data_rsp),
        .axi_req_o(axi_req), .axi_resp_i(axi_rsp)
    );
endmodule
