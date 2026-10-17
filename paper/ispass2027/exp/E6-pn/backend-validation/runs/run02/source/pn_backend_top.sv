// Test-only flattening around the actual full production wrapper and fixed gold.
`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"
module pn_backend_top #(parameter bit Enable = 1) (
  input logic clk_i, rst_ni, compare_i, nonblock_i,
  input logic ctrl_aw_valid_i, ctrl_w_valid_i, ctrl_b_ready_i, ctrl_ar_valid_i, ctrl_r_ready_i,
  input logic [63:0] ctrl_aw_addr_i, ctrl_w_data_i, ctrl_ar_addr_i,
  input logic [7:0] ctrl_w_strb_i,
  output logic ctrl_aw_ready_o, ctrl_w_ready_o, ctrl_b_valid_o, ctrl_ar_ready_o, ctrl_r_valid_o,
  output logic [63:0] ctrl_r_data_o,
  output logic [1:0] ctrl_b_resp_o, ctrl_r_resp_o,
  output logic equal_o, inactive_clean_o,
  output logic permit_o, quiet_o, certificate_pending_o, certificate_fault_o,
  output logic frontend_idle_o, job_fire_o, job_done_o, backend_req_fire_o,
  output logic [63:0] job_length_o, job_src_o, job_dst_o,
  output logic [1:0] ttype_o,
  output logic [7:0] busy_o,
  output logic obs_issue_o, obs_done_o, batch_active_o,
  output logic [63:0] obs_issue_id_o, obs_done_id_o,
  output logic [32:0] batch_issued_o, batch_completed_o,
  output logic ar_valid_o, aw_valid_o, w_valid_o, w_last_o, r_ready_o, b_ready_o,
  output logic [63:0] ar_addr_o, aw_addr_o, w_data_o,
  output logic [7:0] ar_len_o, aw_len_o, w_strb_o,
  output logic [2:0] ar_size_o, aw_size_o,
  output logic [1:0] ar_burst_o, aw_burst_o, ar_id_o, aw_id_o,
  output logic [23:0] ar_sid_o, aw_sid_o,
  input logic ar_ready_i, aw_ready_i, w_ready_i, r_valid_i, r_last_i, b_valid_i,
  input logic [63:0] r_data_i,
  input logic [1:0] r_id_i, r_resp_i, b_id_i, b_resp_i
);
  typedef logic [63:0] addr_t;
  typedef logic [63:0] data_t;
  typedef logic [7:0] strb_t;
  typedef logic [1:0] id_t;
  typedef logic user_t;
  typedef logic [23:0] sid_t;
  typedef logic ssidv_t;
  typedef logic [19:0] ssid_t;
  `AXI_TYPEDEF_EXT_ALL(mst,addr_t,id_t,data_t,strb_t,user_t,sid_t,ssidv_t,ssid_t)
  `AXI_TYPEDEF_ALL(slv,addr_t,id_t,data_t,strb_t,user_t)
  mst_req_t req, gold_req;
  mst_resp_t rsp;
  slv_req_t ctrl_req;
  slv_resp_t ctrl_rsp, gold_ctrl_rsp;
  logic gold_issue, gold_done, gold_poll, gold_batch, poll;
  logic [63:0] gold_issue_id, gold_done_id, gold_poll_id, poll_id;
  logic cfg_valid, cfg_write, gold_cfg_valid, gold_cfg_write;
  logic [6:0] cfg_addr, gold_cfg_addr;
  logic [63:0] cfg_data, gold_cfg_data;
  logic [7:0] cfg_strb, gold_cfg_strb;
  always_comb begin
    ctrl_req='0;
    ctrl_req.aw_valid=ctrl_aw_valid_i; ctrl_req.aw.addr=ctrl_aw_addr_i;
    ctrl_req.aw.size=3; ctrl_req.aw.burst=axi_pkg::BURST_INCR; ctrl_req.aw.id=1;
    ctrl_req.w_valid=ctrl_w_valid_i; ctrl_req.w.data=ctrl_w_data_i;
    ctrl_req.w.strb=ctrl_w_strb_i; ctrl_req.w.last=1;
    ctrl_req.b_ready=ctrl_b_ready_i;
    ctrl_req.ar_valid=ctrl_ar_valid_i; ctrl_req.ar.addr=ctrl_ar_addr_i;
    ctrl_req.ar.size=3; ctrl_req.ar.burst=axi_pkg::BURST_INCR; ctrl_req.ar.id=1;
    ctrl_req.r_ready=ctrl_r_ready_i;
    rsp='0;
    rsp.ar_ready=ar_ready_i; rsp.aw_ready=aw_ready_i; rsp.w_ready=w_ready_i;
    rsp.r_valid=r_valid_i; rsp.r.data=r_data_i; rsp.r.id=r_id_i;
    rsp.r.resp=axi_pkg::resp_t'(r_resp_i); rsp.r.last=r_last_i;
    rsp.b_valid=b_valid_i; rsp.b.id=b_id_i; rsp.b.resp=axi_pkg::resp_t'(b_resp_i);
  end
  dma_core_wrap #(.AxiAddrWidth(64),.AxiDataWidth(64),.AxiIdWidth(2),.AxiUserWidth(1),
    .AxiSlvIdWidth(2),.NumAxInFlight(8),.MemSysDepth(0),.JobFifoDepth(8),.BufferDepth(3),
    .RAWCouplingAvail(0),.ObserverPort(1),.BatchPort(1),.IsTwoD(0),
    .axi_mst_req_t(mst_req_t),.axi_mst_rsp_t(mst_resp_t),.axi_slv_req_t(slv_req_t),.axi_slv_rsp_t(slv_resp_t),
    .DEVICE_ID(24'd10),.PROCESS_ID(20'd20),.AxID(2'd0),.PnEnable(Enable)) candidate (
    .clk_i,.rst_ni,.testmode_i(1'b0),.obs_issue_o,.obs_done_o,.obs_poll_o(poll),.batch_active_o,
    .obs_issue_id_o,.obs_done_id_o,.obs_poll_id_o(poll_id),
    .obs_cfg_valid_o(cfg_valid),.obs_cfg_write_o(cfg_write),.obs_cfg_addr_o(cfg_addr),
    .obs_cfg_wdata_o(cfg_data),.obs_cfg_wstrb_o(cfg_strb),.obs_cfg_rdata_i(64'h12345678),
    .axi_mst_req_o(req),.axi_mst_rsp_i(rsp),.axi_slv_req_i(ctrl_req),.axi_slv_rsp_o(ctrl_rsp),
    .pn_nonblock_enable_i(nonblock_i),.pn_fullbuffer_permit_o(permit_o),.pn_backend_job_quiet_o(quiet_o),
    .pn_certificate_pending_o(certificate_pending_o),.pn_certificate_fault_o(certificate_fault_o)
  );
  // In functional N cases this unused reference is held in reset for the
  // whole case. No production backend receives a mode-dependent reset.
  dma_core_wrap_gold #(.AxiAddrWidth(64),.AxiDataWidth(64),.AxiIdWidth(2),.AxiUserWidth(1),
    .AxiSlvIdWidth(2),.NumAxInFlight(8),.MemSysDepth(0),.JobFifoDepth(8),.BufferDepth(3),
    .RAWCouplingAvail(0),.ObserverPort(1),.BatchPort(1),.IsTwoD(0),
    .axi_mst_req_t(mst_req_t),.axi_mst_rsp_t(mst_resp_t),.axi_slv_req_t(slv_req_t),.axi_slv_rsp_t(slv_resp_t),
    .DEVICE_ID(24'd10),.PROCESS_ID(20'd20),.AxID(2'd0)) reference_dut (
    .clk_i,.rst_ni(rst_ni && compare_i),.testmode_i(1'b0),
    .obs_issue_o(gold_issue),.obs_done_o(gold_done),.obs_poll_o(gold_poll),.batch_active_o(gold_batch),
    .obs_issue_id_o(gold_issue_id),.obs_done_id_o(gold_done_id),.obs_poll_id_o(gold_poll_id),
    .obs_cfg_valid_o(gold_cfg_valid),.obs_cfg_write_o(gold_cfg_write),.obs_cfg_addr_o(gold_cfg_addr),
    .obs_cfg_wdata_o(gold_cfg_data),.obs_cfg_wstrb_o(gold_cfg_strb),.obs_cfg_rdata_i(64'h12345678),
    .axi_mst_req_o(gold_req),.axi_mst_rsp_i(rsp),.axi_slv_req_i(ctrl_req),.axi_slv_rsp_o(gold_ctrl_rsp)
  );
  assign equal_o = ctrl_rsp==gold_ctrl_rsp && req==gold_req &&
    {obs_issue_o,obs_done_o,poll,batch_active_o,obs_issue_id_o,obs_done_id_o,poll_id,
     cfg_valid,cfg_write,cfg_addr,cfg_data,cfg_strb} ==
    {gold_issue,gold_done,gold_poll,gold_batch,gold_issue_id,gold_done_id,gold_poll_id,
     gold_cfg_valid,gold_cfg_write,gold_cfg_addr,gold_cfg_data,gold_cfg_strb} &&
    candidate.be_valid_d==reference_dut.be_valid_d && candidate.be_ready_d==reference_dut.be_ready_d &&
    candidate.be_valid==reference_dut.be_valid && candidate.be_ready==reference_dut.be_ready &&
    candidate.idma_rsp_valid==reference_dut.idma_rsp_valid && candidate.idma_busy==reference_dut.idma_busy;
  assign inactive_clean_o = candidate.pn_select_n ?
    (!candidate.pn_small_req_valid && !candidate.pn_small_rsp_ready && candidate.pn_small_request=='0 &&
     candidate.pn_small_axi_rsp=='0 && candidate.pn_small_ttype==idma_pkg::DEFAULT && candidate.pn_small_wdata=='0 &&
     !candidate.pn_small_rsp_valid && candidate.pn_small_busy=='0 &&
     !candidate.pn_small_axi_req.ar_valid && !candidate.pn_small_axi_req.aw_valid && !candidate.pn_small_axi_req.w_valid) :
    (!candidate.pn_large_req_valid && !candidate.pn_large_rsp_ready && candidate.pn_large_request=='0 &&
     candidate.pn_large_axi_rsp=='0 && candidate.pn_large_ttype==idma_pkg::DEFAULT && candidate.pn_large_wdata=='0 &&
     !candidate.pn_large_rsp_valid && candidate.pn_large_busy=='0 &&
     !candidate.pn_large_axi_req.ar_valid && !candidate.pn_large_axi_req.aw_valid && !candidate.pn_large_axi_req.w_valid);
  assign frontend_idle_o = candidate.gen_one_d.i_dma_frontend.state_q == 0;
  assign job_fire_o = candidate.pn_job_fire;
  assign job_done_o = candidate.pn_job_done;
  assign backend_req_fire_o = candidate.be_valid && candidate.be_ready;
  assign job_length_o=64'(candidate.burst_req_d.length);
  assign job_src_o=64'(candidate.burst_req_d.src_addr);
  assign job_dst_o=64'(candidate.burst_req_d.dst_addr);
  assign ttype_o=candidate.ttype;
  assign busy_o=candidate.idma_busy;
  assign batch_issued_o=candidate.gen_one_d.i_dma_frontend.batch_issued_q;
  assign batch_completed_o=candidate.gen_one_d.i_dma_frontend.batch_completed_q;
  assign ctrl_aw_ready_o=ctrl_rsp.aw_ready;
  assign ctrl_w_ready_o=ctrl_rsp.w_ready;
  assign ctrl_b_valid_o=ctrl_rsp.b_valid;
  assign ctrl_b_resp_o=ctrl_rsp.b.resp;
  assign ctrl_ar_ready_o=ctrl_rsp.ar_ready;
  assign ctrl_r_valid_o=ctrl_rsp.r_valid;
  assign ctrl_r_data_o=ctrl_rsp.r.data;
  assign ctrl_r_resp_o=ctrl_rsp.r.resp;
  assign ar_valid_o=req.ar_valid; assign ar_addr_o=req.ar.addr; assign ar_len_o=req.ar.len;
  assign ar_size_o=req.ar.size; assign ar_burst_o=req.ar.burst; assign ar_id_o=req.ar.id; assign ar_sid_o=req.ar.stream_id;
  assign aw_valid_o=req.aw_valid; assign aw_addr_o=req.aw.addr; assign aw_len_o=req.aw.len;
  assign aw_size_o=req.aw.size; assign aw_burst_o=req.aw.burst; assign aw_id_o=req.aw.id; assign aw_sid_o=req.aw.stream_id;
  assign w_valid_o=req.w_valid; assign w_data_o=req.w.data; assign w_strb_o=req.w.strb; assign w_last_o=req.w.last;
  assign r_ready_o=req.r_ready; assign b_ready_o=req.b_ready;
endmodule
