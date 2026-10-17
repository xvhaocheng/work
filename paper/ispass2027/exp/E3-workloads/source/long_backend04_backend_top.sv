// Test-only flattened ports around the unmodified, fixed-source iDMA backend.
`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"
`include "idma/typedef.svh"
module backend_top (
  input logic clk_i, rst_ni,
  input logic req_valid_i, rsp_ready_i,
  input logic [63:0] length_i, src_i, dst_i,
  output logic req_ready_o, rsp_valid_o, rsp_error_o, rsp_last_o,
  output logic [7:0] busy_o,
  output logic ar_valid_o, aw_valid_o, w_valid_o, w_last_o, r_ready_o, b_ready_o,
  output logic [63:0] ar_addr_o, aw_addr_o, w_data_o,
  output logic [7:0] ar_len_o, aw_len_o, w_strb_o,
  output logic [2:0] ar_size_o, aw_size_o,
  output logic [1:0] ar_burst_o, aw_burst_o, ar_id_o, aw_id_o,
  output logic [23:0] ar_sid_o, aw_sid_o,
  input logic ar_ready_i, aw_ready_i, w_ready_i, r_valid_i, r_last_i, b_valid_i,
  input logic [63:0] r_data_i,
  input logic [1:0] r_id_i, r_resp_i, b_id_i, b_resp_i,
  output logic path4_r_error_o,path4_w_error_o,path16_r_error_o,path16_w_error_o,
  output logic [63:0] path16_r_full_o,path16_w_full_o
);
  typedef logic [63:0] addr_t;
  typedef logic [63:0] data_t;
  typedef logic [7:0] strb_t;
  typedef logic [1:0] id_t;
  typedef logic user_t;
  typedef logic [23:0] sid_t;
  typedef logic ssidv_t;
  typedef logic [19:0] ssid_t;
  `AXI_TYPEDEF_EXT_ALL(axi, addr_t, id_t, data_t, strb_t, user_t, sid_t, ssidv_t, ssid_t)
  `IDMA_TYPEDEF_FULL_REQ_T(idma_req_t, id_t, addr_t, addr_t)
  `IDMA_TYPEDEF_FULL_RSP_T(idma_rsp_t, addr_t)
  idma_req_t req;
  idma_rsp_t rsp;
  idma_pkg::idma_busy_t busy;
  axi_req_t axi_req;
  axi_resp_t axi_rsp;
  always_comb begin
    req = '0;
    req.length = length_i;
    req.src_addr = src_i;
    req.dst_addr = dst_i;
    req.opt.device_id = 24'd10;
    req.opt.process_id = 20'd20;
    req.opt.src.burst = axi_pkg::BURST_INCR;
    req.opt.dst.burst = axi_pkg::BURST_INCR;
    req.opt.last = 1'b1;
    axi_rsp = '0;
    axi_rsp.ar_ready = ar_ready_i;
    axi_rsp.aw_ready = aw_ready_i;
    axi_rsp.w_ready = w_ready_i;
    axi_rsp.r_valid = r_valid_i;
    axi_rsp.r.data = r_data_i;
    axi_rsp.r.id = r_id_i;
    axi_rsp.r.resp = axi_pkg::resp_t'(r_resp_i);
    axi_rsp.r.last = r_last_i;
    axi_rsp.b_valid = b_valid_i;
    axi_rsp.b.id = b_id_i;
    axi_rsp.b.resp = axi_pkg::resp_t'(b_resp_i);
  end
  assign rsp_error_o = rsp.error;
  assign rsp_last_o = rsp.last;
  assign busy_o = busy;
  assign ar_valid_o = axi_req.ar_valid;
  assign ar_addr_o = axi_req.ar.addr;
  assign ar_len_o = axi_req.ar.len;
  assign ar_size_o = axi_req.ar.size;
  assign ar_burst_o = axi_req.ar.burst;
  assign ar_id_o = axi_req.ar.id;
  assign ar_sid_o = axi_req.ar.stream_id;
  assign aw_valid_o = axi_req.aw_valid;
  assign aw_addr_o = axi_req.aw.addr;
  assign aw_len_o = axi_req.aw.len;
  assign aw_size_o = axi_req.aw.size;
  assign aw_burst_o = axi_req.aw.burst;
  assign aw_id_o = axi_req.aw.id;
  assign aw_sid_o = axi_req.aw.stream_id;
  assign w_valid_o = axi_req.w_valid;
  assign w_data_o = axi_req.w.data;
  assign w_strb_o = axi_req.w.strb;
  assign w_last_o = axi_req.w.last;
  assign r_ready_o = axi_req.r_ready;
  assign b_ready_o = axi_req.b_ready;
  idma_backend #(
    .DataWidth(64), .AddrWidth(64), .UserWidth(1), .AxiIdWidth(2),
    .NumAxInFlight(8), .BufferDepth(3), .TFLenWidth(64), .MemSysDepth(0),
    .RAWCouplingAvail(0), .MaskInvalidData(0), .HardwareLegalizer(1),
    .RejectZeroTransfers(1), .ErrorCap(idma_pkg::NO_ERROR_HANDLING),
    .idma_req_t(idma_req_t), .idma_rsp_t(idma_rsp_t),
    .idma_eh_req_t(idma_pkg::idma_eh_req_t), .idma_busy_t(idma_pkg::idma_busy_t),
    .protocol_req_t(axi_req_t), .protocol_rsp_t(axi_resp_t),
    .aw_chan_t(axi_aw_chan_t), .ar_chan_t(axi_ar_chan_t)
  ) dut (
    .clk_i, .rst_ni, .testmode_i(1'b0), .idma_req_i(req), .req_valid_i,
    .req_ready_o, .idma_rsp_o(rsp), .rsp_valid_o, .rsp_ready_i,
    .idma_eh_req_i('0), .eh_req_valid_i(1'b0), .eh_req_ready_o(),
    .protocol_req_o(axi_req), .protocol_rsp_i(axi_rsp), .busy_o(busy),
    .ttype_i(idma_pkg::DEFAULT), .wdata_i('0), .rdata_o(), .rdata_valid_o()
  );

  logic [31:0] monitor_time_q;
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) monitor_time_q <= 0; else monitor_time_q <= monitor_time_q+1;
  logic [63:0] p4r [8];
  dma_path_stage #(.Depth(4)) i_p4r (
    .clk_i,.rst_ni,.clear_i(1'b0),.run_i(1'b1),.active_i(1'b1),.open_i(1'b0),.close_i(1'b0),
    .enter_i(ar_valid_o && ar_ready_i),.leave_i(r_valid_i && r_ready_o && r_last_i),.time_i(monitor_time_q),
    .error_o(path4_r_error_o),.data_o(p4r));
  logic [63:0] p4w [8];
  dma_path_stage #(.Depth(4)) i_p4w (
    .clk_i,.rst_ni,.clear_i(1'b0),.run_i(1'b1),.active_i(1'b1),.open_i(1'b0),.close_i(1'b0),
    .enter_i(aw_valid_o && aw_ready_i),.leave_i(b_valid_i && b_ready_o),.time_i(monitor_time_q),
    .error_o(path4_w_error_o),.data_o(p4w));
  logic [63:0] p16r [8];
  dma_path_stage #(.Depth(16)) i_p16r (
    .clk_i,.rst_ni,.clear_i(1'b0),.run_i(1'b1),.active_i(1'b1),.open_i(1'b0),.close_i(1'b0),
    .enter_i(ar_valid_o && ar_ready_i),.leave_i(r_valid_i && r_ready_o && r_last_i),.time_i(monitor_time_q),
    .error_o(path16_r_error_o),.data_o(p16r));
  assign path16_r_full_o=p16r[1];
  logic [63:0] p16w [8];
  dma_path_stage #(.Depth(16)) i_p16w (
    .clk_i,.rst_ni,.clear_i(1'b0),.run_i(1'b1),.active_i(1'b1),.open_i(1'b0),.close_i(1'b0),
    .enter_i(aw_valid_o && aw_ready_i),.leave_i(b_valid_i && b_ready_o),.time_i(monitor_time_q),
    .error_o(path16_w_error_o),.data_o(p16w));
  assign path16_w_full_o=p16w[1];
endmodule
