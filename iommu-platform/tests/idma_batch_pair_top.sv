`include "common_cells/registers.svh"
`include "idma/typedef.svh"
// Real frontends/registers/observer with deterministic synthetic DMA retirements.
module idma_batch_pair_top (
 input logic clk_i, rst_ni, valid_i, write_i, port_i,
 input logic [7:0] addr_i,
 input logic [63:0] wdata_i,
 output logic ready_o, error_o,
 output logic [63:0] rdata_o,
 output logic [1:0] running_o
);
 typedef struct packed {logic valid,write;logic[7:0]addr;logic[63:0]wdata;logic[7:0]wstrb;} req_t;
 typedef struct packed {logic ready,error;logic[63:0]rdata;} rsp_t;
 typedef logic[63:0] addr_t;
 typedef logic[3:0] id_t;
 `IDMA_TYPEDEF_FULL_REQ_T(burst_t,id_t,addr_t,addr_t)
 req_t req[2];rsp_t rsp[2];burst_t burst[2];
 logic[1:0] issue,done,poll,cfg_valid,cfg_write,request,complete;
 logic[63:0] issue_id[2],done_id[2],poll_id[2],cfg_wdata[2],cfg_rdata;
 logic[6:0] cfg_addr[2];logic[7:0] cfg_wstrb[2];
 for(genvar g=0;g<2;g++)begin
  logic[4:0] remaining;
  assign req[g]={valid_i&&(port_i==g),write_i,addr_i,wdata_i,8'hff};
  assign complete[g]=remaining==1;
  always_ff @(posedge clk_i or negedge rst_ni)begin
   if(!rst_ni) remaining<=0;
   else if(request[g]) remaining<=g==0?6:9;
   else if(remaining!=0) remaining<=remaining-1'b1;
  end
  idma_reg64_frontend #(.dma_regs_req_t(req_t),.dma_regs_rsp_t(rsp_t),.burst_req_t(burst_t),
    .AxiIdWidth(4),.ObserverPort(g==0),.BatchPort(1)) lane(
    .clk_i,.rst_ni,.dma_ctrl_req_i(req[g]),.dma_ctrl_rsp_o(rsp[g]),
    .burst_req_o(burst[g]),.valid_o(request[g]),.ready_i(1'b1),
    .backend_idle_i(remaining==0),.trans_complete_i(complete[g]),
    .ttype_o(),.wdata_o(),.rdata_i('0),.rdata_valid_i(1'b0),
    .batch_active_o(running_o[g]),.obs_issue_o(issue[g]),.obs_done_o(done[g]),.obs_poll_o(poll[g]),
    .obs_issue_id_o(issue_id[g]),.obs_done_id_o(done_id[g]),.obs_poll_id_o(poll_id[g]),
    .obs_cfg_valid_o(cfg_valid[g]),.obs_cfg_write_o(cfg_write[g]),.obs_cfg_addr_o(cfg_addr[g]),
    .obs_cfg_wdata_o(cfg_wdata[g]),.obs_cfg_wstrb_o(cfg_wstrb[g]),.obs_cfg_rdata_i(cfg_rdata));
 end
 assign ready_o=rsp[port_i].ready;
 assign error_o=rsp[port_i].error;
 assign rdata_o=rsp[port_i].rdata;
 // Private port address/data events track each synthetic backend task exactly.
 // Public ingress is unused here; real AXI mux is tested on the FPGA.
 dma_observer #(.NumPorts(2)) obs(.clk_i,.rst_ni,
  .cfg_valid_i(cfg_valid[0]),.cfg_write_i(cfg_write[0]),.cfg_addr_i(cfg_addr[0]),
  .cfg_wdata_i(cfg_wdata[0]),.cfg_wstrb_i(cfg_wstrb[0]),.cfg_rdata_o(cfg_rdata),
  .issue_i(issue),.done_i(done),.poll_i(poll),.batch_active_i(running_o),
  .issue_id_i(issue_id),.done_id_i(done_id),.poll_id_i(poll_id),
  .ar_valid_i(issue),.aw_valid_i(issue),.ar_ready_i(2'b11),.aw_ready_i(2'b11),
  .rlast_fire_i(complete),.b_fire_i(complete),.bad_id_i('0),
  .shared_ar_valid_i(1'b0),.shared_aw_valid_i(1'b0),.shared_ar_ready_i(1'b1),.shared_aw_ready_i(1'b1));
endmodule
