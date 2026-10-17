`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"
module wrapper_top (
 input logic clk_i,rst_ni,cfg_valid_i,cfg_write_i,lane_i,
 input logic[7:0] addr_i,wstrb_i,
 input logic[63:0] wdata_i,
 output logic ready_o,error_o,
 output logic[63:0] rdata_o,
 output logic[31:0] gap0_o,gap1_o,
 output logic[32:0] target0_o,target1_o
);
 typedef logic[63:0] addr_t;typedef logic[63:0] data_t;
 typedef logic[7:0] strb_t;typedef logic[3:0] id_t;typedef logic user_t;
 typedef logic[23:0] did_t;typedef logic[19:0] pid_t;
 `AXI_TYPEDEF_ALL(cfg,addr_t,id_t,data_t,strb_t,user_t)
 `AXI_TYPEDEF_EXT_ALL(master,addr_t,id_t,data_t,strb_t,user_t,did_t,logic,pid_t)
 cfg_req_t cfg_req[2];cfg_resp_t cfg_rsp[2];
 master_req_t master_req[2];master_resp_t master_rsp;
 logic aw_q,w_q,ar_q;
 logic[3:0] issue,done,poll,active,gap;
 logic[63:0] issue_id[4],done_id[4],poll_id[4],obs_wdata[2],obs_data;
 logic[31:0] gap_cycles[4];logic[32:0] target[4];
 logic[1:0] obs_valid,obs_write;logic[6:0] obs_addr[2];logic[7:0] obs_strb[2];
 always_comb begin
  master_rsp='0;
  for(int i=0;i<2;i++)begin
   cfg_req[i]='0;
   if(lane_i==i && cfg_valid_i)begin
    cfg_req[i].aw.addr=64'(addr_i);cfg_req[i].aw.size=3;cfg_req[i].aw.burst=1;
    cfg_req[i].w.data=wdata_i;cfg_req[i].w.strb=wstrb_i;cfg_req[i].w.last=1;
    cfg_req[i].aw_valid=cfg_write_i&&!aw_q;cfg_req[i].w_valid=cfg_write_i&&!w_q;cfg_req[i].b_ready=cfg_write_i;
    cfg_req[i].ar.addr=64'(addr_i);cfg_req[i].ar.size=3;cfg_req[i].ar.burst=1;
    cfg_req[i].ar_valid=!cfg_write_i&&!ar_q;cfg_req[i].r_ready=!cfg_write_i;
   end
  end
  ready_o=cfg_valid_i&&(cfg_write_i?cfg_rsp[lane_i].b_valid:cfg_rsp[lane_i].r_valid);
  error_o=cfg_write_i?cfg_rsp[lane_i].b.resp[1]:cfg_rsp[lane_i].r.resp[1];
  rdata_o=cfg_rsp[lane_i].r.data;
 end
 always_ff @(posedge clk_i or negedge rst_ni)begin
  if(!rst_ni)begin aw_q<=0;w_q<=0;ar_q<=0;end
  else if(!cfg_valid_i||ready_o)begin aw_q<=0;w_q<=0;ar_q<=0;end
  else begin
   if(cfg_req[lane_i].aw_valid&&cfg_rsp[lane_i].aw_ready)aw_q<=1;
   if(cfg_req[lane_i].w_valid&&cfg_rsp[lane_i].w_ready)w_q<=1;
   if(cfg_req[lane_i].ar_valid&&cfg_rsp[lane_i].ar_ready)ar_q<=1;
  end
 end
 for(genvar i=0;i<2;i++)begin : lanes
  dma_core_wrap #(.AxiAddrWidth(64),.AxiDataWidth(64),.AxiIdWidth(4),.AxiUserWidth(1),.AxiSlvIdWidth(4),
   .NumAxInFlight(8),.JobFifoDepth(8),.BufferDepth(3),.ObserverPort(i==0),.BatchPort(1),.BatchGapEnable(1),
   .DEVICE_ID(24'(10+i)),.PROCESS_ID(20'(20+i)),.axi_mst_req_t(master_req_t),.axi_mst_rsp_t(master_resp_t),
   .axi_slv_req_t(cfg_req_t),.axi_slv_rsp_t(cfg_resp_t)) dut (
   .clk_i,.rst_ni,.testmode_i(1'b0),.axi_mst_req_o(master_req[i]),.axi_mst_rsp_i(master_rsp),
   .axi_slv_req_i(cfg_req[i]),.axi_slv_rsp_o(cfg_rsp[i]),
   .obs_issue_o(issue[i]),.obs_done_o(done[i]),.obs_poll_o(poll[i]),.batch_active_o(active[i]),
   .batch_gap_active_o(gap[i]),.batch_gap_cycles_o(gap_cycles[i]),.batch_target_o(target[i]),
   .obs_issue_id_o(issue_id[i]),.obs_done_id_o(done_id[i]),.obs_poll_id_o(poll_id[i]),
   .obs_cfg_valid_o(obs_valid[i]),.obs_cfg_write_o(obs_write[i]),.obs_cfg_addr_o(obs_addr[i]),
   .obs_cfg_wdata_o(obs_wdata[i]),.obs_cfg_wstrb_o(obs_strb[i]),.obs_cfg_rdata_i(obs_data));
 end
 assign issue[3:2]=0;assign done[3:2]=0;assign poll[3:2]=0;assign active[3:2]=0;assign gap[3:2]=0;
 for(genvar i=2;i<4;i++)begin
  assign issue_id[i]=0;assign done_id[i]=0;assign poll_id[i]=0;assign gap_cycles[i]=0;assign target[i]=0;
 end
 assign gap0_o=gap_cycles[0];assign gap1_o=gap_cycles[1];assign target0_o=target[0];assign target1_o=target[1];
 dma_observer #(.GapEnable(1),.PathEnable(1),.HistEnable(1)) observer (
  .clk_i,.rst_ni,.cfg_valid_i(obs_valid[0]),.cfg_write_i(obs_write[0]),.cfg_addr_i(obs_addr[0]),
  .cfg_wdata_i(obs_wdata[0]),.cfg_wstrb_i(obs_strb[0]),.cfg_rdata_o(obs_data),
  .issue_i(issue),.done_i(done),.poll_i(poll),.batch_active_i(active),.batch_gap_active_i(gap),
  .batch_gap_cycles_i(gap_cycles),.batch_target_i(target),.issue_id_i(issue_id),.done_id_i(done_id),.poll_id_i(poll_id),
  .ar_valid_i('0),.aw_valid_i('0),.ar_ready_i('0),.aw_ready_i('0),.rlast_fire_i('0),.b_fire_i('0),.bad_id_i('0),
  .shared_ar_valid_i(1'b0),.shared_aw_valid_i(1'b0),.shared_ar_ready_i(1'b0),.shared_aw_ready_i(1'b0),
  .tr_id_i('{4'd0,4'd0}),.out_id_i('{4'd0,4'd0}),.return_id_i('{4'd0,4'd0}),.out_valid_i('0),.out_ready_i('0),.return_i('0));
endmodule
