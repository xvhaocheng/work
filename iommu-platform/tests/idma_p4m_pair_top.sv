`include "common_cells/registers.svh"
`include "idma/typedef.svh"
// Real two-lane P4M frontend/registers/observer integration harness.
//
// V = batch lane 0 (observer victim, 6-cycle synthetic backend)
// A = batch lane 1 (observer aggressor, 9-cycle synthetic backend)
//
// The synthetic backends prove wiring and the contract's edge accounting only.
// They are not an AXI timing model and must never be used to claim datapath or
// performance behaviour.
module idma_p4m_pair_top (
 input logic clk_i, rst_ni, valid_i, write_i, port_i,
 input logic [7:0] addr_i,
 input logic [63:0] wdata_i,
 output logic ready_o, error_o,
 output logic [63:0] rdata_o,
 output logic [1:0] running_o,
 // Raw per-lane event taps for the C++ independent event oracle.  They are the
 // frontend event taps, never the observer's internal counters.
 output logic [1:0] issue_o, done_o,
 output logic [63:0] issue_id_o [2],
 output logic [63:0] done_id_o [2]
);
 typedef struct packed {logic valid,write;logic[7:0]addr;logic[63:0]wdata;logic[7:0]wstrb;} req_t;
 typedef struct packed {logic ready,error;logic[63:0]rdata;} rsp_t;
 typedef logic[63:0] addr_t;
 typedef logic[3:0] id_t;
 `IDMA_TYPEDEF_FULL_REQ_T(burst_t,id_t,addr_t,addr_t)
 req_t req[2];rsp_t rsp[2];burst_t burst[2];
 logic[1:0] issue,done,poll,cfg_valid,cfg_write,request,complete;
 logic[63:0] issue_id[2],done_id[2],poll_id[2],cfg_wdata[2],cfg_rdata;
 // Keep the full 8-bit frontend control address locally; only the observer's
 // low 7 bits are its documented config window.
 logic[7:0] cfg_addr[2];logic[7:0] cfg_wstrb[2];
 for(genvar g=0;g<2;g++)begin : gen_lane
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
    // LaunchPort is disabled here, so tie the controlled-launch inputs
    // explicitly to zero instead of leaving them floating.
    .launch_hold_i(1'b0),.launch_go_i(1'b0),.launch_cancel_i(1'b0),
    .launch_held_o(),.launch_active_o(),.launch_cancelled_o(),.launch_error_o(),
    .ttype_o(),.wdata_o(),.rdata_i('0),.rdata_valid_i(1'b0),
    .batch_active_o(running_o[g]),.obs_issue_o(issue[g]),.obs_done_o(done[g]),.obs_poll_o(poll[g]),
    .obs_issue_id_o(issue_id[g]),.obs_done_id_o(done_id[g]),.obs_poll_id_o(poll_id[g]),
    .obs_cfg_valid_o(cfg_valid[g]),.obs_cfg_write_o(cfg_write[g]),.obs_cfg_addr_o(cfg_addr[g]),
    .obs_cfg_wdata_o(cfg_wdata[g]),.obs_cfg_wstrb_o(cfg_wstrb[g]),.obs_cfg_rdata_i(cfg_rdata));
 end
 assign ready_o=rsp[port_i].ready;
 assign error_o=rsp[port_i].error;
 assign rdata_o=rsp[port_i].rdata;
 assign issue_o=issue;
 assign done_o=done;
 assign issue_id_o[0]=issue_id[0];
 assign issue_id_o[1]=issue_id[1];
 assign done_id_o[0]=done_id[0];
 assign done_id_o[1]=done_id[1];
 // The gap slot 0xd0 and its read-only capability 0xd8 are decoded and
 // serviced inside the BatchPort frontend, so batch accesses are never
 // forwarded to the observer config bus.  Only the legacy observer window
 // reaches the observer, and only through lane 0's low-7-bit address.
 dma_observer #(.NumPorts(2),.P4MEnable(1),.PathEnable(0)) obs(.clk_i,.rst_ni,
  .cfg_valid_i(cfg_valid[0]),.cfg_write_i(cfg_write[0]),.cfg_addr_i(cfg_addr[0][6:0]),
  .cfg_wdata_i(cfg_wdata[0]),.cfg_wstrb_i(cfg_wstrb[0]),.cfg_rdata_o(cfg_rdata),
  .issue_i(issue),.done_i(done),.poll_i(poll),.batch_active_i(running_o),
  .issue_id_i(issue_id),.done_id_i(done_id),.poll_id_i(poll_id),
  .ar_valid_i(issue),.aw_valid_i(issue),.ar_ready_i(2'b11),.aw_ready_i(2'b11),
  .rlast_fire_i(complete),.b_fire_i(complete),.bad_id_i('0),
  .shared_ar_valid_i(1'b0),.shared_aw_valid_i(1'b0),.shared_ar_ready_i(1'b1),.shared_aw_ready_i(1'b1));
endmodule
