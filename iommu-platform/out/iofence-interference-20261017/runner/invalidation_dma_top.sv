// Test-only system around exact F5 IOMMU and ba8cdd1 ordinary iDMA.
// All debug connections are passive hierarchical reads. No source module is edited.
`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"
module invalidation_dma_top (
    input logic clk_i,
    input logic rst_ni,
`ifndef SYNTHESIS
    input logic sim_fence_read_done_i,
    input logic sim_fence_write_done_i,
`endif

`ifndef SYNTHESIS
    // Simulation-only ORACLE: omniscient immediate removal, no software interface.
    input logic oracle_remove_i,
    input logic [19:0] oracle_pscid_i,
    input logic [26:0] oracle_vpn_i,
`endif

    input logic cfg_valid,
    input logic cfg_write,
    input logic [1:0] cfg_target,
    input logic [63:0] cfg_addr,
    input logic [63:0] cfg_wdata,
    input logic [7:0] cfg_wstrb,
    input logic [2:0] cfg_size,
    output logic cfg_ready,
    output logic cfg_error,
    output logic [63:0] cfg_rdata,
    output logic mem_aw_valid,
    input logic mem_aw_ready,
    output logic [5:0] mem_aw_id,
    output logic [63:0] mem_aw_addr,
    output logic [7:0] mem_aw_len,
    output logic [2:0] mem_aw_size,
    output logic [1:0] mem_aw_burst,
    output logic mem_aw_lock,
    output logic [3:0] mem_aw_cache,
    output logic [2:0] mem_aw_prot,
    output logic [3:0] mem_aw_qos,
    output logic [3:0] mem_aw_region,
    output logic [5:0] mem_aw_atop,
    output logic mem_aw_user,
    output logic mem_ar_valid,
    input logic mem_ar_ready,
    output logic [5:0] mem_ar_id,
    output logic [63:0] mem_ar_addr,
    output logic [7:0] mem_ar_len,
    output logic [2:0] mem_ar_size,
    output logic [1:0] mem_ar_burst,
    output logic mem_ar_lock,
    output logic [3:0] mem_ar_cache,
    output logic [2:0] mem_ar_prot,
    output logic [3:0] mem_ar_qos,
    output logic [3:0] mem_ar_region,
    output logic mem_ar_user,
    output logic mem_w_valid,
    input logic mem_w_ready,
    output logic [63:0] mem_w_data,
    output logic [7:0] mem_w_strb,
    output logic mem_w_last,
    output logic mem_w_user,
    input logic mem_b_valid,
    output logic mem_b_ready,
    input logic [5:0] mem_b_id,
    input logic [1:0] mem_b_resp,
    input logic mem_b_user,
    input logic mem_r_valid,
    output logic mem_r_ready,
    input logic [5:0] mem_r_id,
    input logic [63:0] mem_r_data,
    input logic [1:0] mem_r_resp,
    input logic mem_r_last,
    input logic mem_r_user,
    output logic dbg_tl_req_fire,
    output logic [23:0] dbg_tl_req_did,
    output logic [63:0] dbg_tl_req_iova,
    output logic dbg_lookup,
    output logic dbg_lookup_hit,
    output logic [26:0] dbg_lookup_vpn,
    output logic [19:0] dbg_lookup_pscid,
    output logic [23:0] dbg_lookup_did,
    output logic dbg_ptw_start,
    output logic [23:0] dbg_walk_did,
    output logic dbg_ptw_read_fire,
    output logic dbg_inval,
    output logic dbg_inval_av,
    output logic dbg_inval_pscv,
    output logic dbg_inval_gv,
    output logic [19:0] dbg_inval_pscid,
    output logic [26:0] dbg_inval_vpn,
    output logic [31:0] dbg_cq_head,
    output logic [31:0] dbg_cq_tail,
    output logic [31:0] dbg_cq_csr,
    output logic [2:0] dbg_cq_state,
    output logic [6:0] dbg_cq_opcode,
    output logic [2:0] dbg_cq_func3,
    output logic dbg_cq_decode,
    output logic dbg_cq_read_fire,
    output logic dbg_cq_write_b_fire,
    output logic [1:0] dbg_dma_issue,
    output logic [1:0] dbg_dma_done,
    output logic dbg_fault,
    output logic [11:0] dbg_fault_cause,
    output logic dbg_iommu_in_flight,
    output logic [1:0] dbg_dma_quiet,
    output logic [15:0] dbg_tags_valid,
    output logic [15:0] dbg_tags_en1,
    output logic [15:0] dbg_tags_en2,
    output logic [15:0] dbg_tags_global,
    output logic [431:0] dbg_tags_vpn,
    output logic [319:0] dbg_tags_pscid,
    output logic [2:0] dbg_tl_state,
    output logic [1:0] dbg_ptw_state,
    output logic dbg_ptw_init,
    output logic dbg_ptw_edge_trigger,
    // AXI mask bits: ARV,ARR,RV,RR,RL,AWV,AWR,WV,WR,BV,BR (bit0 first).
    output logic [2:0] dbg_cq_next_state,
    output logic [1:0] dbg_cq_wr_state,
    output logic dbg_cq_cmd_complete,
    output logic dbg_iofence_wait,
    output logic dbg_iofence_decode,
    output logic dbg_iofence_complete,
    output logic dbg_iofence_av,
    output logic dbg_iofence_pr,
    output logic dbg_iofence_pw,
    output logic dbg_tl_active,
    output logic [23:0] dbg_tl_active_did,
    output logic [63:0] dbg_tl_active_iova,
    output logic [3:0] dbg_tl_active_ttype,
    output logic [2:0] dbg_tl_next_state,
    output logic dbg_tl_resp_valid,
    output logic dbg_tl_resp_ready,
    output logic [10:0] dbg_cq_bus,
    output logic [10:0] dbg_ptw_bus,
    output logic [10:0] dbg_ddtw_bus,
    output logic [10:0] dbg_ds_bus,
    output logic [10:0] dbg_comp_bus,
    output logic [10:0] dbg_dma0_bus,
    output logic [10:0] dbg_dma1_bus,
    output logic [63:0] dbg_cq_ar_addr,
    output logic [63:0] dbg_ptw_ar_addr,
    output logic [63:0] dbg_ddtw_ar_addr,
    output logic [63:0] dbg_ds_ar_addr,
    output logic [63:0] dbg_comp_ar_addr,
    output logic [63:0] dbg_comp_aw_addr,
    output logic [3:0] dbg_ds_ar_id,
    output logic [3:0] dbg_ds_r_id,
    output logic [3:0] dbg_comp_ar_id,
    output logic [3:0] dbg_comp_aw_id,
    output logic [3:0] dbg_comp_r_id,
    output logic [3:0] dbg_comp_b_id,
    output logic dbg_iotlb_update,
    output logic dbg_iotlb_fill,
    output logic [26:0] dbg_iotlb_update_vpn,
    output logic [19:0] dbg_iotlb_update_pscid,
    output logic [15:0] dbg_iotlb_replace_en,
    output logic [15:0] dbg_iotlb_lu_hit_vec,
    output logic [14:0] dbg_iotlb_plru
);
  typedef logic [63:0] addr_t;
  typedef logic [63:0] data_t;
  typedef logic [7:0] strb_t;
  typedef logic [0:0] user_t;
  typedef logic [1:0] dma_id_t;
  typedef logic [3:0] iommu_id_t;
  typedef logic [5:0] cfg_id_t;
  typedef logic [5:0] mem_id_t;
  typedef logic [23:0] did_t;
  typedef logic ssidv_t;
  typedef logic [19:0] ssid_t;
  `AXI_TYPEDEF_EXT_ALL(tr2,addr_t,dma_id_t,data_t,strb_t,user_t,did_t,ssidv_t,ssid_t)
  `AXI_TYPEDEF_EXT_ALL(tr4,addr_t,iommu_id_t,data_t,strb_t,user_t,did_t,ssidv_t,ssid_t)
  `AXI_TYPEDEF_ALL(bus,addr_t,iommu_id_t,data_t,strb_t,user_t)
  `AXI_TYPEDEF_ALL(cfg,addr_t,cfg_id_t,data_t,strb_t,user_t)
  `AXI_TYPEDEF_ALL(mem,addr_t,mem_id_t,data_t,strb_t,user_t)
  tr2_req_t [3:0] dma_req;
  tr2_resp_t [3:0] dma_rsp;
  tr4_req_t tr_req;
  tr4_resp_t tr_rsp;
  cfg_req_t [2:0] cfg_req;
  cfg_resp_t [2:0] cfg_rsp;
  bus_req_t comp_req,ds_req;
  bus_resp_t comp_rsp,ds_rsp;
  assign dma_req[3:2]='0;

  for (genvar d=0;d<2;d++) begin: gen_dma
    dma_core_wrap #(
      .AxiAddrWidth(64),.AxiDataWidth(64),.AxiIdWidth(2),.AxiUserWidth(1),
      .AxiSlvIdWidth(6),.NumAxInFlight(8),.MemSysDepth(0),.JobFifoDepth(8),
      .BufferDepth(3),.RAWCouplingAvail(0),.IsTwoD(0),
      .axi_mst_req_t(tr2_req_t),.axi_mst_rsp_t(tr2_resp_t),
      .axi_slv_req_t(cfg_req_t),.axi_slv_rsp_t(cfg_resp_t),
      .DEVICE_ID(24'(10+d)),.PROCESS_ID(20'(20+d)),.AxID(2'd0)
    ) i_dma (
      .clk_i,.rst_ni,.testmode_i(1'b0),.axi_mst_req_o(dma_req[d]),
      .axi_mst_rsp_i(dma_rsp[d]),.axi_slv_req_i(cfg_req[d]),.axi_slv_rsp_o(cfg_rsp[d])
    );
    assign dbg_dma_issue[d]=i_dma.gen_one_d.i_dma_frontend.issue;
    assign dbg_dma_done[d]=i_dma.gen_one_d.i_dma_frontend.trans_complete;
    assign dbg_dma_quiet[d]=(i_dma.gen_one_d.i_dma_frontend.state_q==0) &&
      !(|i_dma.idma_busy) && !i_dma.be_valid_d && !i_dma.be_valid;
  end
  axi_mux #(
    .SlvAxiIDWidth(2),.NoSlvPorts(4),.MaxWTrans(8),
    .slv_aw_chan_t(tr2_aw_chan_t),.mst_aw_chan_t(tr4_aw_chan_t),.w_chan_t(tr2_w_chan_t),
    .slv_b_chan_t(tr2_b_chan_t),.mst_b_chan_t(tr4_b_chan_t),
    .slv_ar_chan_t(tr2_ar_chan_t),.mst_ar_chan_t(tr4_ar_chan_t),
    .slv_r_chan_t(tr2_r_chan_t),.mst_r_chan_t(tr4_r_chan_t),
    .slv_req_t(tr2_req_t),.slv_resp_t(tr2_resp_t),.mst_req_t(tr4_req_t),.mst_resp_t(tr4_resp_t)
  ) i_tr_mux(.clk_i,.rst_ni,.test_i(1'b0),.slv_reqs_i(dma_req),.slv_resps_o(dma_rsp),.mst_req_o(tr_req),.mst_resp_i(tr_rsp));

  function automatic rv_iommu_cfg::rv_iommu_cfg_t make_cfg();
    rv_iommu_cfg::rv_iommu_cfg_t c=rv_iommu_cfg::NullCfg;
    c.NumOutstandingTrans=8;c.WFifoDepth=32;c.MrifFifoDepth=1;c.FaultFifoDepth=1;
    c.NumIotlbEntries=16;c.NumDdtcEntries=8;c.NumPdtcEntries=1;c.NumMrifcEntries=1;
    c.InclPC=0;c.InclAxiBC=1;c.InclDbg=0;c.InclATS=0;
    c.MSITrans=rv_iommu_cfg::MSI_DISABLED;c.IGS=rv_iommu_cfg::WSI_ONLY;
    c.NumIntVec=1;c.NumHpmCounters=0;c.PAddrWidth=56;c.AxiAddrWidth=64;c.AxiDataWidth=64;
    c.AxiIdWidth=4;c.AxiProgIdWidth=6;c.AxiUserWidth=1;c.AxiProIdWidth=20;c.AxiDevIdWidth=24;
    c.AxisDataWidth=64;c.AxisDestWidth=1;c.AxisUserWidth=1;c.AxisStrbWidth=8;c.AxisKeepWidth=8;c.AxisIdWidth=1;c.Freq=1;
    return c;
  endfunction
  localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg=make_cfg();
  rv_iommu_top #(
    .RVIOMMUCfg(Cfg),
    .axi_tr_aw_chan_t(tr4_aw_chan_t),.axi_tr_w_chan_t(tr4_w_chan_t),.axi_tr_b_chan_t(tr4_b_chan_t),
    .axi_tr_ar_chan_t(tr4_ar_chan_t),.axi_tr_r_chan_t(tr4_r_chan_t),.axi_tr_req_t(tr4_req_t),.axi_tr_resp_t(tr4_resp_t),
    .axi_comp_aw_chan_t(bus_aw_chan_t),.axi_comp_w_chan_t(bus_w_chan_t),.axi_comp_b_chan_t(bus_b_chan_t),
    .axi_comp_ar_chan_t(bus_ar_chan_t),.axi_comp_r_chan_t(bus_r_chan_t),.axi_comp_req_t(bus_req_t),.axi_comp_resp_t(bus_resp_t),
    .axi_ds_aw_chan_t(bus_aw_chan_t),.axi_ds_w_chan_t(bus_w_chan_t),.axi_ds_b_chan_t(bus_b_chan_t),
    .axi_ds_ar_chan_t(bus_ar_chan_t),.axi_ds_r_chan_t(bus_r_chan_t),.axi_ds_req_t(bus_req_t),.axi_ds_resp_t(bus_resp_t),
    .axi_prog_aw_chan_t(cfg_aw_chan_t),.axi_prog_w_chan_t(cfg_w_chan_t),.axi_prog_b_chan_t(cfg_b_chan_t),
    .axi_prog_ar_chan_t(cfg_ar_chan_t),.axi_prog_r_chan_t(cfg_r_chan_t),.axi_prog_req_t(cfg_req_t),.axi_prog_resp_t(cfg_resp_t)
  ) i_iommu(.clk_i,.rst_ni,
`ifndef SYNTHESIS
    .oracle_remove_i,.oracle_pscid_i,.oracle_vpn_i,
    .sim_fence_read_done_i,.sim_fence_write_done_i,
`endif
    .tr_req_i(tr_req),.tr_resp_o(tr_rsp),
    .comp_resp_i(comp_rsp),.comp_req_o(comp_req),.ds_resp_i(ds_rsp),.ds_req_o(ds_req),
    .prog_req_i(cfg_req[2]),.prog_resp_o(cfg_rsp[2]),
    .ats_upstream_req_o(),.ats_upstream_resp_i(1'b0),.ats_downstream_req_i(1'b0),.ats_downstream_resp_o(),.wsi_wires_o());

  // Single outstanding host MMIO transaction; AW and W accept independently.
  typedef enum logic [2:0] {C_IDLE,C_SEND,C_B,C_AR,C_R,C_HOLD} cfg_state_t;
  cfg_state_t cfg_state;
  logic [1:0] target_q;
  logic [63:0] addr_q,wdata_q;
  logic [7:0] strb_q;
  logic [2:0] size_q;
  logic aw_done,w_done;
  cfg_req_t cfg_drive;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if(!rst_ni) begin
      cfg_state<=C_IDLE;target_q<=0;addr_q<=0;wdata_q<=0;strb_q<=0;size_q<=0;
      aw_done<=0;w_done<=0;cfg_error<=0;cfg_rdata<=0;
    end else begin
      case(cfg_state)
        C_IDLE: if(cfg_valid) begin
          target_q<=cfg_target;addr_q<=cfg_addr;wdata_q<=cfg_wdata;strb_q<=cfg_wstrb;size_q<=cfg_size;
          cfg_error<=cfg_target==3;cfg_rdata<=0;aw_done<=0;w_done<=0;
          cfg_state<=cfg_target==3?C_HOLD:(cfg_write?C_SEND:C_AR);
        end
        C_SEND: begin
          if(cfg_drive.aw_valid&&cfg_rsp[target_q].aw_ready)aw_done<=1;
          if(cfg_drive.w_valid&&cfg_rsp[target_q].w_ready)w_done<=1;
          if((aw_done||cfg_rsp[target_q].aw_ready)&&(w_done||cfg_rsp[target_q].w_ready))cfg_state<=C_B;
        end
        C_B: if(cfg_rsp[target_q].b_valid)begin cfg_error<=cfg_rsp[target_q].b.resp!=0;cfg_state<=C_HOLD;end
        C_AR: if(cfg_rsp[target_q].ar_ready)cfg_state<=C_R;
        C_R: if(cfg_rsp[target_q].r_valid)begin cfg_rdata<=cfg_rsp[target_q].r.data;cfg_error<=cfg_rsp[target_q].r.resp!=0;cfg_state<=C_HOLD;end
        C_HOLD: if(!cfg_valid)cfg_state<=C_IDLE;
        default:cfg_state<=C_IDLE;
      endcase
    end
  end
  always_comb begin
    cfg_drive='0;
    cfg_drive.aw.id=0;cfg_drive.aw.addr=addr_q;cfg_drive.aw.len=0;cfg_drive.aw.size=size_q;cfg_drive.aw.burst=axi_pkg::BURST_INCR;
    cfg_drive.w.data=wdata_q;cfg_drive.w.strb=strb_q;cfg_drive.w.last=1;
    cfg_drive.ar.id=0;cfg_drive.ar.addr=addr_q;cfg_drive.ar.len=0;cfg_drive.ar.size=size_q;cfg_drive.ar.burst=axi_pkg::BURST_INCR;
    cfg_drive.aw_valid=cfg_state==C_SEND&&!aw_done;cfg_drive.w_valid=cfg_state==C_SEND&&!w_done;
    cfg_drive.b_ready=cfg_state==C_B;cfg_drive.ar_valid=cfg_state==C_AR;cfg_drive.r_ready=cfg_state==C_R;
    cfg_req='0;
    if(target_q<3)cfg_req[target_q]=cfg_drive;
  end
  assign cfg_ready=cfg_state==C_HOLD;

  bus_req_t [3:0] fabric_req;
  bus_resp_t [3:0] fabric_rsp;
  mem_req_t [1:0] memory_req;
  mem_resp_t [1:0] memory_rsp;
  assign fabric_req[0]='0;assign fabric_req[1]='0;
  assign fabric_req[2]=comp_req;assign comp_rsp=fabric_rsp[2];
  assign fabric_req[3]=ds_req;assign ds_rsp=fabric_rsp[3];
  axi_pkg::xbar_rule_64_t [1:0] addr_map;
  assign addr_map[0]='{idx:32'd0,start_addr:64'h80000000,end_addr:64'hc0000000};
  assign addr_map[1]='{idx:32'd1,start_addr:64'h100000000,end_addr:64'h200000000};
  localparam axi_pkg::xbar_cfg_t XC='{
    NoSlvPorts:4,NoMstPorts:2,MaxMstTrans:8,MaxSlvTrans:8,FallThrough:1'b0,
    LatencyMode:axi_pkg::CUT_ALL_PORTS,PipelineStages:1,AxiIdWidthSlvPorts:4,AxiIdUsedSlvPorts:4,
    UniqueIds:1'b0,AxiAddrWidth:64,AxiDataWidth:64,NoAddrRules:2};
  axi_xbar #(.Cfg(XC),.ATOPs(1'b0),.Connectivity('1),
    .slv_aw_chan_t(bus_aw_chan_t),.mst_aw_chan_t(mem_aw_chan_t),.w_chan_t(bus_w_chan_t),
    .slv_b_chan_t(bus_b_chan_t),.mst_b_chan_t(mem_b_chan_t),
    .slv_ar_chan_t(bus_ar_chan_t),.mst_ar_chan_t(mem_ar_chan_t),
    .slv_r_chan_t(bus_r_chan_t),.mst_r_chan_t(mem_r_chan_t),
    .slv_req_t(bus_req_t),.slv_resp_t(bus_resp_t),.mst_req_t(mem_req_t),.mst_resp_t(mem_resp_t),
    .rule_t(axi_pkg::xbar_rule_64_t)
  ) i_xbar(.clk_i,.rst_ni,.test_i(1'b0),.slv_ports_req_i(fabric_req),.slv_ports_resp_o(fabric_rsp),
      .mst_ports_req_o(memory_req),.mst_ports_resp_i(memory_rsp),.addr_map_i(addr_map),
      .en_default_mst_port_i('0),.default_mst_port_i('0));
  axi_err_slv #(.AxiIdWidth(6),.axi_req_t(mem_req_t),.axi_resp_t(mem_resp_t),.ATOPs(1'b0))
    i_unused(.clk_i,.rst_ni,.test_i(1'b0),.slv_req_i(memory_req[1]),.slv_resp_o(memory_rsp[1]));
  assign mem_aw_valid=memory_req[0].aw_valid;
  assign mem_aw_id=memory_req[0].aw.id;
  assign mem_aw_addr=memory_req[0].aw.addr;
  assign mem_aw_len=memory_req[0].aw.len;
  assign mem_aw_size=memory_req[0].aw.size;
  assign mem_aw_burst=memory_req[0].aw.burst;
  assign mem_aw_lock=memory_req[0].aw.lock;
  assign mem_aw_cache=memory_req[0].aw.cache;
  assign mem_aw_prot=memory_req[0].aw.prot;
  assign mem_aw_qos=memory_req[0].aw.qos;
  assign mem_aw_region=memory_req[0].aw.region;
  assign mem_aw_atop=memory_req[0].aw.atop;
  assign mem_aw_user=memory_req[0].aw.user;
  assign mem_ar_valid=memory_req[0].ar_valid;
  assign mem_ar_id=memory_req[0].ar.id;
  assign mem_ar_addr=memory_req[0].ar.addr;
  assign mem_ar_len=memory_req[0].ar.len;
  assign mem_ar_size=memory_req[0].ar.size;
  assign mem_ar_burst=memory_req[0].ar.burst;
  assign mem_ar_lock=memory_req[0].ar.lock;
  assign mem_ar_cache=memory_req[0].ar.cache;
  assign mem_ar_prot=memory_req[0].ar.prot;
  assign mem_ar_qos=memory_req[0].ar.qos;
  assign mem_ar_region=memory_req[0].ar.region;
  assign mem_ar_user=memory_req[0].ar.user;
  assign mem_w_valid=memory_req[0].w_valid;
  assign mem_w_data=memory_req[0].w.data;
  assign mem_w_strb=memory_req[0].w.strb;
  assign mem_w_last=memory_req[0].w.last;
  assign mem_w_user=memory_req[0].w.user;
  assign mem_r_ready=memory_req[0].r_ready;
  assign mem_b_ready=memory_req[0].b_ready;
  always_comb begin
    memory_rsp[0]='0;
    memory_rsp[0].aw_ready=mem_aw_ready;
    memory_rsp[0].ar_ready=mem_ar_ready;
    memory_rsp[0].w_ready=mem_w_ready;
    memory_rsp[0].b_valid=mem_b_valid;
    memory_rsp[0].b.id=mem_b_id;
    memory_rsp[0].b.resp=mem_b_resp;
    memory_rsp[0].b.user=mem_b_user;
    memory_rsp[0].r_valid=mem_r_valid;
    memory_rsp[0].r.id=mem_r_id;
    memory_rsp[0].r.data=mem_r_data;
    memory_rsp[0].r.resp=mem_r_resp;
    memory_rsp[0].r.last=mem_r_last;
    memory_rsp[0].r.user=mem_r_user;
  end
`define TL i_iommu.gen_dc_base.i_rv_iommu_tl_wrap
`define PTW `TL.i_rv_iommu_ptw
`define IOTLB `TL.i_rv_iommu_iotlb
`define SW i_iommu.i_rv_iommu_sw_if_wrap
`define CQ `SW.i_rv_iommu_cq_handler
  logic lookup_seen;
  always_ff @(posedge clk_i or negedge rst_ni)begin
    if(!rst_ni)lookup_seen<=0;
    else if(!`TL.iotlb_lu)lookup_seen<=0;
    else lookup_seen<=1;
  end
  assign dbg_tl_req_fire=i_iommu.trans_req_valid&&i_iommu.trans_req_ready;
  assign dbg_tl_req_did=i_iommu.trans_req_data.did;
  assign dbg_tl_req_iova=i_iommu.trans_req_data.iova;
  assign dbg_lookup=`TL.iotlb_lu&&!lookup_seen;
  assign dbg_lookup_hit=`TL.iotlb_lu_hit;
  assign dbg_lookup_vpn=`TL.iotlb_lu_vpn[26:0];
  assign dbg_lookup_pscid=`TL.iotlb_lu_pscid;
  assign dbg_lookup_did=`TL.req_data.did;
  assign dbg_ptw_start=(`PTW.state_q==0)&&`PTW.init_ptw_i&&!`PTW.edge_trigger_q;
  assign dbg_walk_did=`TL.req_data.did;
  assign dbg_ptw_read_fire=i_iommu.ptw_axi_req.ar_valid&&i_iommu.ptw_axi_resp.ar_ready;
  assign dbg_tl_state=`TL.state_q;
  assign dbg_ptw_state=`PTW.state_q;
  assign dbg_ptw_init=`PTW.init_ptw_i;
  assign dbg_ptw_edge_trigger=`PTW.edge_trigger_q;
  assign dbg_inval=i_iommu.iotinval.inval_vma;
  assign dbg_inval_av=i_iommu.iotinval.av;
  assign dbg_inval_pscv=i_iommu.iotinval.pscv;
  assign dbg_inval_gv=i_iommu.iotinval.gv;
  assign dbg_inval_pscid=i_iommu.iotinval.pscid;
  assign dbg_inval_vpn=i_iommu.iotinval.vpn[26:0];
  assign dbg_cq_head=`SW.reg2hw.cqh.q;
  assign dbg_cq_tail=`SW.reg2hw.cqt.q;
  always_comb begin
    dbg_cq_csr='0;
    dbg_cq_csr[0]=`SW.reg2hw.cqcsr.cqen.q;dbg_cq_csr[1]=`SW.reg2hw.cqcsr.cie.q;
    dbg_cq_csr[8]=`SW.reg2hw.cqcsr.cqmf.q;dbg_cq_csr[9]=`SW.reg2hw.cqcsr.cmd_to.q;
    dbg_cq_csr[10]=`SW.reg2hw.cqcsr.cmd_ill.q;dbg_cq_csr[11]=`SW.reg2hw.cqcsr.fence_w_ip.q;
    dbg_cq_csr[16]=`SW.i_rv_iommu_regmap.cqcsr_cqon_qs;dbg_cq_csr[17]=`SW.i_rv_iommu_regmap.cqcsr_busy_qs;
  end
  assign dbg_cq_state=`CQ.state_q;
  assign dbg_cq_opcode=`CQ.cmd_q[6:0];assign dbg_cq_func3=`CQ.cmd_q[9:7];
  assign dbg_cq_decode=`CQ.state_q==3;
  assign dbg_cq_read_fire=i_iommu.cq_axi_req.ar_valid&&i_iommu.cq_axi_resp.ar_ready;
  assign dbg_cq_write_b_fire=i_iommu.cq_axi_req.b_ready&&i_iommu.cq_axi_resp.b_valid;
  assign dbg_fault=i_iommu.fault_valid||i_iommu.mrif_fault_valid||(|dbg_cq_csr[10:8]);
  assign dbg_fault_cause=i_iommu.fault_data.cause_code;
  assign dbg_iommu_in_flight=i_iommu.in_flight;
  for(genvar way=0;way<16;way++)begin:gen_tags
    assign dbg_tags_valid[way]=`IOTLB.tags_q[way].valid;
    assign dbg_tags_en1[way]=`IOTLB.tags_q[way].en_1S;
    assign dbg_tags_en2[way]=`IOTLB.tags_q[way].en_2S;
    assign dbg_tags_global[way]=`IOTLB.content_q[way].content_1S.g;
    assign dbg_tags_vpn[way*27+:27]={`IOTLB.tags_q[way].vpn2[8:0],`IOTLB.tags_q[way].vpn1,`IOTLB.tags_q[way].vpn0};
    assign dbg_tags_pscid[way*20+:20]=`IOTLB.tags_q[way].pscid;
  end
  // These signals only leave this independent simulation wrapper.
  assign dbg_cq_next_state=`CQ.state_n;
  assign dbg_cq_wr_state=`CQ.wr_state_q;
  assign dbg_cq_cmd_complete=((`CQ.state_q==3)||(`CQ.state_q==4)||(`CQ.state_q==6))&&(`CQ.state_n==0);
  assign dbg_iofence_wait=(`CQ.state_q==6);
  assign dbg_iofence_decode=(`CQ.state_q==3)&&(`CQ.cmd_q[6:0]==7'd2);
  assign dbg_iofence_complete=dbg_cq_cmd_complete&&(`CQ.cmd_q[6:0]==7'd2);
  assign dbg_iofence_av=`CQ.cmd_iofence.av;
  assign dbg_iofence_pr=`CQ.cmd_iofence.pr;
  assign dbg_iofence_pw=`CQ.cmd_iofence.pw;
  assign dbg_tl_active=`TL.req_valid;
  assign dbg_tl_active_did=`TL.req_data.did;
  assign dbg_tl_active_iova=`TL.req_data.iova;
  assign dbg_tl_active_ttype=`TL.req_data.ttype;
  assign dbg_tl_next_state=`TL.state_n;
  assign dbg_tl_resp_valid=i_iommu.trans_resp_valid;
  assign dbg_tl_resp_ready=i_iommu.trans_resp_ready;
`define OBS_AXI_MASK(req,rsp) {req.b_ready,rsp.b_valid,rsp.w_ready,req.w_valid,rsp.aw_ready,req.aw_valid,rsp.r.last,req.r_ready,rsp.r_valid,rsp.ar_ready,req.ar_valid}
  assign dbg_cq_bus=`OBS_AXI_MASK(i_iommu.cq_axi_req,i_iommu.cq_axi_resp);
  assign dbg_ptw_bus=`OBS_AXI_MASK(i_iommu.ptw_axi_req,i_iommu.ptw_axi_resp);
  assign dbg_ddtw_bus=`OBS_AXI_MASK(i_iommu.ddtw_axi_req,i_iommu.ddtw_axi_resp);
  assign dbg_ds_bus=`OBS_AXI_MASK(ds_req,ds_rsp);
  assign dbg_comp_bus=`OBS_AXI_MASK(comp_req,comp_rsp);
  assign dbg_dma0_bus=`OBS_AXI_MASK(dma_req[0],dma_rsp[0]);
  assign dbg_dma1_bus=`OBS_AXI_MASK(dma_req[1],dma_rsp[1]);
`undef OBS_AXI_MASK
  assign dbg_cq_ar_addr=i_iommu.cq_axi_req.ar.addr;
  assign dbg_ptw_ar_addr=i_iommu.ptw_axi_req.ar.addr;
  assign dbg_ddtw_ar_addr=i_iommu.ddtw_axi_req.ar.addr;
  assign dbg_ds_ar_addr=ds_req.ar.addr;
  assign dbg_comp_ar_addr=comp_req.ar.addr;
  assign dbg_comp_aw_addr=comp_req.aw.addr;
  assign dbg_ds_ar_id=ds_req.ar.id;
  assign dbg_ds_r_id=ds_rsp.r.id;
  assign dbg_comp_ar_id=comp_req.ar.id;
  assign dbg_comp_aw_id=comp_req.aw.id;
  assign dbg_comp_r_id=comp_rsp.r.id;
  assign dbg_comp_b_id=comp_rsp.b.id;
  assign dbg_iotlb_update=`IOTLB.update_i.update;
  assign dbg_iotlb_fill=`IOTLB.update_i.update&&!`IOTLB.inval_i.inval_vma&&!`IOTLB.inval_i.inval_gvma;
  assign dbg_iotlb_update_vpn=`IOTLB.update_i.vpn[26:0];
  assign dbg_iotlb_update_pscid=`IOTLB.update_i.pscid;
  assign dbg_iotlb_replace_en=`IOTLB.replace_en;
  assign dbg_iotlb_lu_hit_vec=`IOTLB.lu_hit;
  assign dbg_iotlb_plru=`IOTLB.plru_tree_q;
`undef TL
`undef PTW
`undef IOTLB
`undef SW
`undef CQ
endmodule
