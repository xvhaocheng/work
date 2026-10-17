// Finite host wrapper around the actual complete peripheral DMA/IOMMU cone.
// Board I/O peripherals are disabled; no quiet/certificate/translation stand-in.
`include "axi/typedef.svh"
`include "axi/assign.svh"
module pn_peripheral_top #(parameter bit Enable=1, History=1) (
  input logic clk_i, rst_ni, cfg_iommu_i,
  input logic ctrl_aw_valid_i, ctrl_w_valid_i, ctrl_b_ready_i, ctrl_ar_valid_i, ctrl_r_ready_i,
  input logic [63:0] ctrl_aw_addr_i, ctrl_w_data_i, ctrl_ar_addr_i,
  input logic [7:0] ctrl_w_strb_i,
  output logic ctrl_aw_ready_o, ctrl_w_ready_o, ctrl_b_valid_o, ctrl_ar_ready_o, ctrl_r_valid_o,
  output logic [63:0] ctrl_r_data_o,
  output logic [1:0] ctrl_b_resp_o, ctrl_r_resp_o,
  output logic [5:0] ctrl_b_id_o, ctrl_r_id_o,
  output logic mem_ar_valid_o[2], mem_aw_valid_o[2], mem_w_valid_o[2], mem_w_last_o[2],
  output logic mem_r_ready_o[2], mem_b_ready_o[2],
  output logic [63:0] mem_ar_addr_o[2], mem_aw_addr_o[2], mem_w_data_o[2],
  output logic [7:0] mem_ar_len_o[2], mem_aw_len_o[2], mem_w_strb_o[2],
  output logic [2:0] mem_ar_size_o[2], mem_aw_size_o[2],
  output logic [1:0] mem_ar_burst_o[2], mem_aw_burst_o[2],
  output logic [3:0] mem_ar_id_o[2], mem_aw_id_o[2],
  input logic mem_ar_ready_i[2], mem_aw_ready_i[2], mem_w_ready_i[2],
  input logic mem_r_valid_i[2], mem_r_last_i[2], mem_b_valid_i[2],
  input logic [63:0] mem_r_data_i[2],
  input logic [3:0] mem_r_id_i[2], mem_b_id_i[2],
  input logic [1:0] mem_r_resp_i[2], mem_b_resp_i[2],
  output logic [2:0] mode_word_o,
  output logic policy_clear_o, dataflow_quiet_o, iommu_quiet_o, capture_busy_o,
  output logic [3:0] permit_o, certificate_pending_o, certificate_fault_o,
  output logic [3:0] frontend_quiet_o, backend_quiet_o, issue_o, done_o, batch_active_o,
  output logic arm_o, arm_has_a_o, obs_active_o,
  output logic n_fire_o,
  output logic [23:0] n_did_o
);
  typedef logic [63:0] addr_t;
  typedef logic [63:0] data_t;
  typedef logic [7:0] strb_t;
  typedef logic [5:0] ctrl_id_t;
  typedef logic [3:0] mem_id_t;
  typedef logic user_t;
  `AXI_TYPEDEF_ALL(ctrl,addr_t,ctrl_id_t,data_t,strb_t,user_t)
  `AXI_TYPEDEF_ALL(mem,addr_t,mem_id_t,data_t,strb_t,user_t)
  ctrl_req_t ctrl_req, dma_req, prog_req, unused_req;
  ctrl_resp_t ctrl_rsp, dma_rsp, prog_rsp;
  mem_req_t mem_req[2];
  mem_resp_t mem_rsp[2];
  AXI_BUS #(.AXI_ADDR_WIDTH(64),.AXI_DATA_WIDTH(64),.AXI_ID_WIDTH(6),.AXI_USER_WIDTH(1))
    dma(),prog(),plic(),uart(),spi(),gpio(),ethernet(),timer();
  AXI_BUS #(.AXI_ADDR_WIDTH(64),.AXI_DATA_WIDTH(64),.AXI_ID_WIDTH(4),.AXI_USER_WIDTH(1))
    comp(),ds();
  always_comb begin
    ctrl_req='0;
    ctrl_req.aw_valid=ctrl_aw_valid_i;ctrl_req.aw.addr=ctrl_aw_addr_i;
    ctrl_req.aw.size=3;ctrl_req.aw.burst=axi_pkg::BURST_INCR;ctrl_req.aw.id=6'h2d;
    ctrl_req.w_valid=ctrl_w_valid_i;ctrl_req.w.data=ctrl_w_data_i;
    ctrl_req.w.strb=ctrl_w_strb_i;ctrl_req.w.last=1;
    ctrl_req.b_ready=ctrl_b_ready_i;
    ctrl_req.ar_valid=ctrl_ar_valid_i;ctrl_req.ar.addr=ctrl_ar_addr_i;
    ctrl_req.ar.size=3;ctrl_req.ar.burst=axi_pkg::BURST_INCR;ctrl_req.ar.id=6'h35;
    ctrl_req.r_ready=ctrl_r_ready_i;
    unused_req='0;unused_req.r_ready=1;unused_req.b_ready=1;
    dma_req=cfg_iommu_i ? unused_req : ctrl_req;
    prog_req=cfg_iommu_i ? ctrl_req : unused_req;
    ctrl_rsp=cfg_iommu_i ? prog_rsp : dma_rsp;
  end
  `AXI_ASSIGN_FROM_REQ(dma,dma_req)
  `AXI_ASSIGN_TO_RESP(dma_rsp,dma)
  `AXI_ASSIGN_FROM_REQ(prog,prog_req)
  `AXI_ASSIGN_TO_RESP(prog_rsp,prog)
  `AXI_ASSIGN_FROM_REQ(plic,unused_req)
  `AXI_ASSIGN_FROM_REQ(uart,unused_req)
  `AXI_ASSIGN_FROM_REQ(spi,unused_req)
  `AXI_ASSIGN_FROM_REQ(gpio,unused_req)
  `AXI_ASSIGN_FROM_REQ(ethernet,unused_req)
  `AXI_ASSIGN_FROM_REQ(timer,unused_req)
  `AXI_ASSIGN_TO_REQ(mem_req[0],comp)
  `AXI_ASSIGN_FROM_RESP(comp,mem_rsp[0])
  `AXI_ASSIGN_TO_REQ(mem_req[1],ds)
  `AXI_ASSIGN_FROM_RESP(ds,mem_rsp[1])
  for(genvar i=0;i<2;i++)begin
    always_comb begin
      mem_rsp[i]='0;
      mem_rsp[i].ar_ready=mem_ar_ready_i[i];mem_rsp[i].aw_ready=mem_aw_ready_i[i];
      mem_rsp[i].w_ready=mem_w_ready_i[i];mem_rsp[i].r_valid=mem_r_valid_i[i];
      mem_rsp[i].r.data=mem_r_data_i[i];mem_rsp[i].r.id=mem_r_id_i[i];
      mem_rsp[i].r.resp=axi_pkg::resp_t'(mem_r_resp_i[i]);mem_rsp[i].r.last=mem_r_last_i[i];
      mem_rsp[i].b_valid=mem_b_valid_i[i];mem_rsp[i].b.id=mem_b_id_i[i];
      mem_rsp[i].b.resp=axi_pkg::resp_t'(mem_b_resp_i[i]);
    end
    assign mem_ar_valid_o[i]=mem_req[i].ar_valid;
    assign mem_aw_valid_o[i]=mem_req[i].aw_valid;
    assign mem_w_valid_o[i]=mem_req[i].w_valid;
    assign mem_ar_addr_o[i]=mem_req[i].ar.addr;assign mem_aw_addr_o[i]=mem_req[i].aw.addr;
    assign mem_ar_len_o[i]=mem_req[i].ar.len;assign mem_aw_len_o[i]=mem_req[i].aw.len;
    assign mem_ar_size_o[i]=mem_req[i].ar.size;assign mem_aw_size_o[i]=mem_req[i].aw.size;
    assign mem_ar_burst_o[i]=mem_req[i].ar.burst;assign mem_aw_burst_o[i]=mem_req[i].aw.burst;
    assign mem_ar_id_o[i]=mem_req[i].ar.id;assign mem_aw_id_o[i]=mem_req[i].aw.id;
    assign mem_w_data_o[i]=mem_req[i].w.data;assign mem_w_strb_o[i]=mem_req[i].w.strb;
    assign mem_w_last_o[i]=mem_req[i].w.last;
    assign mem_r_ready_o[i]=mem_req[i].r_ready;assign mem_b_ready_o[i]=mem_req[i].b_ready;
  end
  wire eth_txck,eth_txctl,eth_rst_n,eth_mdio,eth_mdc;
  wire [3:0] eth_txd;
  wire [1:0] irq;
  wire tx,spi_clk,spi_mosi,spi_ss;
  wire [7:0] leds;
  ariane_peripherals #(.AxiAddrWidth(64),.AxiDataWidth(64),.AxiIdWidth(6),.AxiUserWidth(1),
    .InclUART(0),.InclSPI(0),.InclEthernet(0),.InclGPIO(0),.InclTimer(0),
    .InclDMA(1),.InclIOMMU(1),.PnEnable(Enable),.HistoryControlEnable(History)) dut (
    .clk_i,.clk_200MHz_i(clk_i),.rst_ni,.plic,.uart,.spi,.gpio,.ethernet,.timer,
    .dma_cfg(dma),.iommu_comp(comp),.iommu_ds(ds),.iommu_prog(prog),.irq_o(irq),
    .rx_i(1'b1),.tx_o(tx),.eth_clk_i(clk_i),.eth_rxck(1'b0),.eth_rxctl(1'b0),.eth_rxd(4'b0),
    .eth_txck,.eth_txctl,.eth_txd,.eth_rst_n,.phy_tx_clk_i(clk_i),.eth_mdio,.eth_mdc,
    .spi_clk_o(spi_clk),.spi_mosi,.spi_miso(1'b0),.spi_ss,.sd_clk_i(clk_i),
    .leds_o(leds),.dip_switches_i(8'b0)
  );
  assign ctrl_aw_ready_o=ctrl_rsp.aw_ready;assign ctrl_w_ready_o=ctrl_rsp.w_ready;
  assign ctrl_b_valid_o=ctrl_rsp.b_valid;assign ctrl_ar_ready_o=ctrl_rsp.ar_ready;
  assign ctrl_r_valid_o=ctrl_rsp.r_valid;assign ctrl_r_data_o=ctrl_rsp.r.data;
  assign ctrl_b_resp_o=ctrl_rsp.b.resp;assign ctrl_r_resp_o=ctrl_rsp.r.resp;
  assign ctrl_b_id_o=ctrl_rsp.b.id;assign ctrl_r_id_o=ctrl_rsp.r.id;
  // Test-only visibility; production uses explicit ports, never hierarchy.
  assign mode_word_o=dut.pn_mode_word;assign policy_clear_o=dut.pn_policy_clear;
  assign dataflow_quiet_o=dut.pn_dataflow_quiet;assign iommu_quiet_o=dut.pn_iommu_quiet;
  assign capture_busy_o=dut.pn_capture_busy;
  assign permit_o=dut.pn_fullbuffer_permit;assign certificate_pending_o=dut.pn_certificate_pending;
  assign certificate_fault_o=dut.pn_certificate_fault;
  assign frontend_quiet_o=dut.pn_frontend_quiet;assign backend_quiet_o=dut.pn_backend_job_quiet;
  assign issue_o=dut.gen_dma.obs_issue;assign done_o=dut.gen_dma.obs_done;
  assign batch_active_o=dut.gen_dma.batch_active;
  assign n_fire_o=dut.pn_n_fire;assign n_did_o=dut.pn_n_did;
  assign arm_o=dut.gen_dma.i_dma_observer.pn_arm_accepted;
  assign arm_has_a_o=dut.gen_dma.i_dma_observer.has_a_n;
  assign obs_active_o=dut.gen_dma.i_dma_observer.active;
endmodule
