// Copyright 2022 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Thomas Benz    <tbenz@iis.ee.ethz.ch>
// Author: Andreas Kuster <kustera@ethz.ch>
// Author: Paul Scheffler <paulsc@iis.ee.ethz.ch>
//
// Description: DMA core wrapper for the CVA6 integration

`include "axi/assign.svh"
`include "axi/typedef.svh"
`include "axi-iommu/assign.svh"
`include "axi-iommu/typedef.svh"
`include "idma/typedef.svh"
`include "register_interface/typedef.svh"

module dma_core_wrap #(
  parameter int unsigned              AxiAddrWidth     = 32'd0,
  parameter int unsigned              AxiDataWidth     = 32'd0,
  parameter int unsigned              AxiIdWidth       = 32'd0,
  parameter int unsigned              AxiUserWidth     = 32'd0,
  parameter int unsigned              AxiSlvIdWidth    = 32'd0,
  parameter int unsigned              NumAxInFlight    = 32'd0,
  parameter int unsigned              MemSysDepth      = 32'd0,
  parameter int unsigned              JobFifoDepth     = 32'd0,
  parameter int unsigned              BufferDepth      = 32'h3, // Default = 3
  parameter bit                       RAWCouplingAvail = 32'd0,
  parameter bit                       ObserverPort    = 1'b0,
  parameter bit BatchPort = 1'b0,
  parameter bit                       IsTwoD           = 32'd0,
  parameter type                      axi_mst_req_t    = logic,
  parameter type                      axi_mst_rsp_t    = logic,
  parameter type                      axi_slv_req_t    = logic,
  parameter type                      axi_slv_rsp_t    = logic,
  parameter logic [23:0]              DEVICE_ID        = 24'd1,
  parameter logic [19:0]              PROCESS_ID       = 20'd0,
  parameter logic [(AxiIdWidth-1):0]  AxID             = 0,
  parameter bit PnEnable = 1'b0,
  parameter bit PnTapEnable = 1'b0,
  parameter bit PnGapEnable = 1'b0
) (
  input  logic          clk_i,
  input  logic          rst_ni,
  input  logic          testmode_i,
    // Read-only event taps and optional observer register window.
    output logic obs_issue_o, obs_done_o, obs_poll_o,
  output logic batch_active_o,
    output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
    output logic obs_cfg_valid_o, obs_cfg_write_o,
    output logic [6:0] obs_cfg_addr_o,
    output logic [63:0] obs_cfg_wdata_o,
    output logic [7:0] obs_cfg_wstrb_o,
    input logic [63:0] obs_cfg_rdata_i,
  output axi_mst_req_t  axi_mst_req_o,
  input  axi_mst_rsp_t  axi_mst_rsp_i,
  input  axi_slv_req_t  axi_slv_req_i,
  output axi_slv_rsp_t  axi_slv_rsp_o,
  input logic pn_nonblock_enable_i = 1'b0,
  output logic pn_fullbuffer_permit_o, pn_backend_job_quiet_o,
  output logic pn_certificate_pending_o, pn_certificate_fault_o,
  output logic pn_stop_accepted_o, pn_frontend_quiet_o
);

  // local params
  localparam int unsigned DmaRegisterWidth = 32'd64;
  localparam int unsigned NumDim           = 32'd2;
  localparam int unsigned TFLenWidth       = AxiAddrWidth;

  typedef logic [AxiAddrWidth-1:0]     addr_t;
  typedef logic [AxiDataWidth-1:0]     data_t;
  typedef logic [(AxiDataWidth/8)-1:0] strb_t;
  typedef logic [AxiUserWidth-1:0]     axi_user_t;
  typedef logic [AxiIdWidth-1:0]       axi_id_t;
  typedef logic [AxiSlvIdWidth-1:0]    axi_slv_id_t;

  typedef logic [24-1:0]  iommu_sid_t;
  typedef logic           iommu_ssidv_t;
  typedef logic [20-1:0]  iommu_ssid_t;

  // iDMA struct definitions
  typedef logic [TFLenWidth-1:0]  tf_len_t;

  // iDMA request / response types
  `IDMA_TYPEDEF_FULL_REQ_T(idma_req_t, axi_slv_id_t, addr_t, tf_len_t)
  `IDMA_TYPEDEF_FULL_RSP_T(idma_rsp_t, addr_t)

  `REG_BUS_TYPEDEF_ALL(dma_regs, addr_t, data_t, strb_t)

  idma_req_t burst_req, burst_req_d;
  logic      be_valid, be_valid_d;
  logic      be_ready, be_ready_d;
  logic      be_trans_complete;
  idma_pkg::idma_busy_t idma_busy;

  idma_rsp_t idma_rsp;
  logic idma_rsp_valid;
  logic idma_rsp_ready;

  logic twod_trans_complete;
  logic twod_busy;

  dma_regs_req_t dma_regs_req;
  dma_regs_rsp_t dma_regs_rsp;

  logic [1:0] ttype;
  data_t wdata, rdata;
  logic rdata_valid;

  axi_to_reg #(
    .ADDR_WIDTH( AxiAddrWidth     ),
    .DATA_WIDTH( AxiDataWidth     ),
    .ID_WIDTH  ( AxiSlvIdWidth    ),
    .USER_WIDTH( AxiUserWidth     ),
    .axi_req_t ( axi_slv_req_t    ),
    .axi_rsp_t ( axi_slv_rsp_t    ),
    .reg_req_t ( dma_regs_req_t   ),
    .reg_rsp_t ( dma_regs_rsp_t   )
  ) i_axi_translate (
    .clk_i,
    .rst_ni,
    .testmode_i ( 1'b0          ),
    .axi_req_i  ( axi_slv_req_i ),
    .axi_rsp_o  ( axi_slv_rsp_o ),
    .reg_req_o  ( dma_regs_req  ),
    .reg_rsp_i  ( dma_regs_rsp  )
  );


  if (!IsTwoD) begin : gen_one_d
    /*
     * DMA Frontend
     */
    idma_reg64_frontend #(
      .ObserverPort ( ObserverPort ),
      .BatchPort ( BatchPort ),
      .PnTapEnable ( PnTapEnable ),
      .PnGapEnable ( PnGapEnable ),
      .dma_regs_req_t  ( dma_regs_req_t ),
      .dma_regs_rsp_t  ( dma_regs_rsp_t ),
      .burst_req_t     ( idma_req_t     ),
      .DEVICE_ID       ( DEVICE_ID      ),
      .PROCESS_ID      ( PROCESS_ID     ),
      .AxiIdWidth      ( AxiIdWidth     ),
      .AxID            ( AxID           )
    ) i_dma_frontend (
      .obs_issue_o, .obs_done_o, .obs_poll_o, .batch_active_o,
      .pn_stop_accepted_o, .pn_frontend_quiet_o,
      .obs_issue_id_o, .obs_done_id_o, .obs_poll_id_o,
      .obs_cfg_valid_o, .obs_cfg_write_o, .obs_cfg_addr_o,
      .obs_cfg_wdata_o, .obs_cfg_wstrb_o, .obs_cfg_rdata_i,
      .clk_i,
      .rst_ni,
      // AXI slave: control port
      .dma_ctrl_req_i   ( dma_regs_req      ),
      .dma_ctrl_rsp_o   ( dma_regs_rsp      ),
      // Backend control
      .burst_req_o      ( burst_req_d       ),
      .valid_o          ( be_valid_d        ),
      .ready_i          ( be_ready_d        ),
      .backend_idle_i   ( ~|idma_busy       ),
      .trans_complete_i ( be_trans_complete ),

      .ttype_o          ( ttype             ),
      .wdata_o          ( wdata             ),
      .rdata_i          ( rdata             ),
      .rdata_valid_i    ( rdata_valid       )
    );

    stream_fifo #(
        .FALL_THROUGH ( 1'b0         ),
        .DATA_WIDTH   ( AxiDataWidth ),
        .DEPTH        ( JobFifoDepth ),
        .T            ( idma_req_t   )
      ) i_stream_fifo_jobs_oned (
        .clk_i,
        .rst_ni,
        .testmode_i,
        .flush_i    ( 1'b0                ),
        .usage_o    ( /* NOT CONNECTED */ ),
        .data_i     ( burst_req_d         ),
        .valid_i    ( be_valid_d          ),
        .ready_o    ( be_ready_d          ),
        .data_o     ( burst_req           ),
        .valid_o    ( be_valid            ),
        .ready_i    ( be_ready            )
    );


    assign be_trans_complete = idma_rsp_valid;
    assign idma_rsp_ready    = 1'b1;

  end else begin : gen_two_d
    assign pn_stop_accepted_o = 1'b0;
    assign pn_frontend_quiet_o = 1'b0;
    assign batch_active_o = 1'b0;
    assign obs_issue_o = 1'b0;
    assign obs_done_o = 1'b0;
    assign obs_poll_o = 1'b0;
    assign obs_issue_id_o = '0;
    assign obs_done_id_o = '0;
    assign obs_poll_id_o = '0;
    assign obs_cfg_valid_o = 1'b0;
    assign obs_cfg_write_o = 1'b0;
    assign obs_cfg_addr_o = '0;
    assign obs_cfg_wdata_o = '0;
    assign obs_cfg_wstrb_o = '0;

    `IDMA_TYPEDEF_FULL_ND_REQ_T(idma_nd_req_t, idma_req_t, tf_len_t, tf_len_t)

    idma_nd_req_t idma_nd_req,       idma_nd_req_d;
    logic         idma_nd_req_valid, idma_nd_req_valid_d;
    logic         idma_nd_req_ready, idma_nd_req_ready_d;

    // 2D frontend
    idma_reg64_2d_frontend #(
      .dma_regs_req_t  ( dma_regs_req_t ),
      .dma_regs_rsp_t  ( dma_regs_rsp_t ),
      .burst_req_t     ( idma_req_t     ),
      .idma_nd_req_t   ( idma_nd_req_t  )

    ) i_dma_2d_frontend (
      .clk_i,
      .rst_ni,
      // AXI slave: control port
      .dma_ctrl_req_i   ( dma_regs_req             ),
      .dma_ctrl_rsp_o   ( dma_regs_rsp             ),
      // Backend control
      .idma_nd_req_o    ( idma_nd_req_d            ),
      .valid_o          ( idma_nd_req_valid_d      ),
      .ready_i          ( idma_nd_req_ready_d      ),
      .backend_idle_i   ( ~|idma_busy & !twod_busy ),
      .trans_complete_i ( twod_trans_complete      )
    );

    stream_fifo #(
        .FALL_THROUGH ( 1'b0          ),
        .DATA_WIDTH   ( AxiDataWidth  ),
        .DEPTH        ( JobFifoDepth  ),
        .T            ( idma_nd_req_t )
      ) i_stream_fifo_jobs_twod (
        .clk_i,
        .rst_ni,
        .testmode_i,
        .flush_i    ( 1'b0                ),
        .usage_o    ( /* NOT CONNECTED */ ),
        .data_i     ( idma_nd_req_d       ),
        .valid_i    ( idma_nd_req_valid_d ),
        .ready_o    ( idma_nd_req_ready_d ),
        .data_o     ( idma_nd_req         ),
        .valid_o    ( idma_nd_req_valid   ),
        .ready_i    ( idma_nd_req_ready   )
    );

    // Midend
    idma_nd_midend #(
      .NumDim        ( NumDim          ),
      .addr_t        ( addr_t          ),
      .idma_req_t    ( idma_req_t      ),
      .idma_rsp_t    ( idma_rsp_t      ),
      .idma_nd_req_t ( idma_nd_req_t   ),
      .RepWidths     ( {AxiAddrWidth, AxiAddrWidth} )
    ) i_idma_nd_midend (
      .clk_i,
      .rst_ni,
      .nd_req_i         ( idma_nd_req         ),
      .nd_req_valid_i   ( idma_nd_req_valid   ),
      .nd_req_ready_o   ( idma_nd_req_ready   ),
      .nd_rsp_o         ( /* NOT CONECTED */  ),
      .nd_rsp_valid_o   ( twod_trans_complete ),
      .nd_rsp_ready_i   ( 1'b1                ),
      .burst_req_o      ( burst_req           ),
      .burst_req_valid_o( be_valid            ),
      .burst_req_ready_i( be_ready            ),
      .burst_rsp_i      ( idma_rsp            ),
      .burst_rsp_valid_i( idma_rsp_valid      ),
      .burst_rsp_ready_o( idma_rsp_ready      ),
      .busy_o           ( twod_busy           )
    );
  end

  `AXI_TYPEDEF_AW_CHAN_EXT_T(axi_mst_aw_chan_t, addr_t, axi_id_t, axi_user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)
  `AXI_TYPEDEF_AR_CHAN_EXT_T(axi_mst_ar_chan_t, addr_t, axi_id_t, axi_user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)

  // MODE[0] is supplied by an external complete-allquiet guard. This mux
  // does not protect a busy mode change and never resets or flushes a branch.
  localparam int unsigned PnBufferDepth = 512;
  logic pn_select_n;
  idma_req_t pn_small_request, pn_large_request;
  idma_rsp_t pn_small_response, pn_large_response;
  idma_pkg::idma_busy_t pn_small_busy, pn_large_busy;
  axi_mst_req_t pn_small_axi_req, pn_large_axi_req;
  axi_mst_rsp_t pn_small_axi_rsp, pn_large_axi_rsp;
  logic pn_small_req_valid, pn_small_req_ready, pn_small_rsp_valid, pn_small_rsp_ready;
  logic pn_large_req_valid, pn_large_req_ready, pn_large_rsp_valid, pn_large_rsp_ready;
  logic [1:0] pn_small_ttype, pn_large_ttype;
  data_t pn_small_wdata, pn_large_wdata, pn_small_rdata, pn_large_rdata;
  logic pn_small_rdata_valid, pn_large_rdata_valid;
  logic pn_small_permit, pn_large_permit, pn_small_pending, pn_large_pending;
  logic pn_small_fault, pn_large_fault;
  logic pn_job_fire, pn_job_done;

  assign pn_select_n = PnEnable && !IsTwoD && pn_nonblock_enable_i;
  assign pn_small_request = pn_select_n ? '0 : burst_req;
  assign pn_large_request = pn_select_n ? burst_req : '0;
  assign pn_small_req_valid = !pn_select_n && be_valid;
  assign pn_large_req_valid = pn_select_n && be_valid;
  assign be_ready = pn_select_n ? pn_large_req_ready : pn_small_req_ready;
  assign idma_rsp = pn_select_n ? pn_large_response : pn_small_response;
  assign idma_rsp_valid = pn_select_n ? pn_large_rsp_valid : pn_small_rsp_valid;
  assign pn_small_rsp_ready = !pn_select_n && idma_rsp_ready;
  assign pn_large_rsp_ready = pn_select_n && idma_rsp_ready;
  assign idma_busy = pn_select_n ? pn_large_busy : pn_small_busy;
  assign pn_small_ttype = pn_select_n ? idma_pkg::DEFAULT : ttype;
  assign pn_large_ttype = pn_select_n ? ttype : idma_pkg::DEFAULT;
  assign pn_small_wdata = pn_select_n ? '0 : wdata;
  assign pn_large_wdata = pn_select_n ? wdata : '0;
  assign rdata = pn_select_n ? pn_large_rdata : pn_small_rdata;
  assign rdata_valid = pn_select_n ? pn_large_rdata_valid : pn_small_rdata_valid;
  assign axi_mst_req_o = pn_select_n ? pn_large_axi_req : pn_small_axi_req;
  assign pn_small_axi_rsp = pn_select_n ? '0 : axi_mst_rsp_i;
  assign pn_large_axi_rsp = pn_select_n ? axi_mst_rsp_i : '0;

  // Acceptance at the frontend->jobFIFO boundary, not the backend input.
  assign pn_job_fire = !IsTwoD && be_valid_d && be_ready_d;
  assign pn_job_done = !IsTwoD && idma_rsp_valid && idma_rsp_ready;
  assign pn_fullbuffer_permit_o = pn_select_n && pn_large_permit;
  assign pn_certificate_pending_o = pn_small_pending || pn_large_pending;
  assign pn_certificate_fault_o = pn_small_fault || pn_large_fault;
  // Backend/job domain only: no claim about frontend IDLE/launch/COUNT,
  // control AXI, system AXI stock, IOMMU/TL/PTW or observer quiet.
  assign pn_backend_job_quiet_o = !IsTwoD && !be_valid_d && !be_valid &&
      !(|pn_small_busy) && !(|pn_large_busy) &&
      !pn_small_rsp_valid && !pn_large_rsp_valid &&
      !pn_small_axi_req.ar_valid && !pn_small_axi_req.aw_valid && !pn_small_axi_req.w_valid &&
      !pn_large_axi_req.ar_valid && !pn_large_axi_req.aw_valid && !pn_large_axi_req.w_valid &&
      !axi_mst_rsp_i.r_valid && !axi_mst_rsp_i.b_valid && !pn_certificate_pending_o;

  idma_backend #(
    .DataWidth           ( AxiDataWidth                ),
    .AddrWidth           ( AxiAddrWidth                ),
    .UserWidth           ( AxiUserWidth                ),
    .AxiIdWidth          ( AxiIdWidth                  ),
    .NumAxInFlight       ( NumAxInFlight               ),
    .BufferDepth         ( BufferDepth                 ),
    .TFLenWidth          ( TFLenWidth                  ),
    .RAWCouplingAvail    ( RAWCouplingAvail            ),
    .MaskInvalidData     ( 1'b0                        ),
    .HardwareLegalizer   ( 1'b1                        ),
    .RejectZeroTransfers ( 1'b1                        ),
    .MemSysDepth         ( MemSysDepth                 ),
    .ErrorCap            ( idma_pkg::NO_ERROR_HANDLING ),
    .idma_req_t          ( idma_req_t                  ),
    .idma_rsp_t          ( idma_rsp_t                  ),
    .idma_eh_req_t       ( idma_pkg::idma_eh_req_t     ),
    .idma_busy_t         ( idma_pkg::idma_busy_t       ),
    .protocol_req_t      ( axi_mst_req_t               ),
    .protocol_rsp_t      ( axi_mst_rsp_t               ),
    .aw_chan_t           ( axi_mst_aw_chan_t           ),
    .ar_chan_t           ( axi_mst_ar_chan_t           )
  ) i_idma_backend (
    .clk_i,
    .rst_ni,
    .testmode_i,

    .idma_req_i    ( pn_small_request         ),
    .req_valid_i   ( pn_small_req_valid       ),
    .req_ready_o   ( pn_small_req_ready       ),

    .idma_rsp_o    ( pn_small_response        ),
    .rsp_valid_o   ( pn_small_rsp_valid       ),
    .rsp_ready_i   ( pn_small_rsp_ready       ),

    .idma_eh_req_i ( '0                ), // No error handling
    .eh_req_valid_i( 1'b1              ),
    .eh_req_ready_o( /*NOT CONNECTED*/ ),

    .protocol_req_o( pn_small_axi_req         ),
    .protocol_rsp_i( pn_small_axi_rsp         ),
    .busy_o        ( pn_small_busy            ),

    .ttype_i       ( pn_small_ttype           ),
    .wdata_i       ( pn_small_wdata           ),
    .rdata_o       ( pn_small_rdata           ),
    .rdata_valid_o ( pn_small_rdata_valid     )
  );

  if (PnEnable && !IsTwoD) begin : gen_pn_backend
    idma_backend #(
      .DataWidth           ( AxiDataWidth                ),
      .AddrWidth           ( AxiAddrWidth                ),
      .UserWidth           ( AxiUserWidth                ),
      .AxiIdWidth          ( AxiIdWidth                  ),
      .NumAxInFlight       ( NumAxInFlight               ),
      .BufferDepth         ( PnBufferDepth               ),
      .TFLenWidth          ( TFLenWidth                  ),
      .RAWCouplingAvail    ( RAWCouplingAvail            ),
      .MaskInvalidData     ( 1'b0                        ),
      .HardwareLegalizer   ( 1'b1                        ),
      .RejectZeroTransfers ( 1'b1                        ),
      .MemSysDepth         ( MemSysDepth                 ),
      .ErrorCap            ( idma_pkg::NO_ERROR_HANDLING ),
      .idma_req_t          ( idma_req_t                  ),
      .idma_rsp_t          ( idma_rsp_t                  ),
      .idma_eh_req_t       ( idma_pkg::idma_eh_req_t     ),
      .idma_busy_t         ( idma_pkg::idma_busy_t       ),
      .protocol_req_t      ( axi_mst_req_t               ),
      .protocol_rsp_t      ( axi_mst_rsp_t               ),
      .aw_chan_t           ( axi_mst_aw_chan_t           ),
      .ar_chan_t           ( axi_mst_ar_chan_t           )
    ) i_idma_backend_fullbuffer (
      .clk_i,
      .rst_ni,
      .testmode_i,

      .idma_req_i    ( pn_large_request         ),
      .req_valid_i   ( pn_large_req_valid       ),
      .req_ready_o   ( pn_large_req_ready       ),

      .idma_rsp_o    ( pn_large_response        ),
      .rsp_valid_o   ( pn_large_rsp_valid       ),
      .rsp_ready_i   ( pn_large_rsp_ready       ),

      .idma_eh_req_i ( '0                ), // No error handling
      .eh_req_valid_i( 1'b1              ),
      .eh_req_ready_o( /*NOT CONNECTED*/ ),

      .protocol_req_o( pn_large_axi_req         ),
      .protocol_rsp_i( pn_large_axi_rsp         ),
      .busy_o        ( pn_large_busy            ),

      .ttype_i       ( pn_large_ttype           ),
      .wdata_i       ( pn_large_wdata           ),
      .rdata_o       ( pn_large_rdata           ),
      .rdata_valid_o ( pn_large_rdata_valid     )
    );

    // Static capacities describe each actual branch, not an assumed Linux mode.
    idma_fullbuffer_certificate #(.CapacityBytes(BufferDepth * (AxiDataWidth / 8))) i_small_certificate (
      .clk_i, .rst_ni,
      .job_fire_i(pn_job_fire && !pn_select_n), .job_done_i(pn_job_done && !pn_select_n),
      .normal_copy_i(!pn_select_n && ttype == idma_pkg::DEFAULT),
      .length_i(64'(burst_req_d.length)), .src_i(64'(burst_req_d.src_addr)), .dst_i(64'(burst_req_d.dst_addr)),
      .permit_o(pn_small_permit), .pending_o(pn_small_pending), .fault_o(pn_small_fault)
    );
    idma_fullbuffer_certificate #(.CapacityBytes(PnBufferDepth * (AxiDataWidth / 8))) i_large_certificate (
      .clk_i, .rst_ni,
      .job_fire_i(pn_job_fire && pn_select_n), .job_done_i(pn_job_done && pn_select_n),
      .normal_copy_i(pn_select_n && ttype == idma_pkg::DEFAULT),
      .length_i(64'(burst_req_d.length)), .src_i(64'(burst_req_d.src_addr)), .dst_i(64'(burst_req_d.dst_addr)),
      .permit_o(pn_large_permit), .pending_o(pn_large_pending), .fault_o(pn_large_fault)
    );
  end else begin : gen_no_pn_backend
    assign pn_large_req_ready = 1'b0;
    assign pn_large_response = '0;
    assign pn_large_rsp_valid = 1'b0;
    assign pn_large_busy = '0;
    assign pn_large_axi_req = '0;
    assign pn_large_rdata = '0;
    assign pn_large_rdata_valid = 1'b0;
    assign pn_small_permit = 1'b0;
    assign pn_large_permit = 1'b0;
    assign pn_small_pending = 1'b0;
    assign pn_large_pending = 1'b0;
    assign pn_small_fault = 1'b0;
    assign pn_large_fault = 1'b0;
  end

endmodule : dma_core_wrap



module dma_core_wrap_intf #(
  parameter int unsigned               AXI_ADDR_WIDTH     = 32'd0,
  parameter int unsigned               AXI_DATA_WIDTH     = 32'd0,
  parameter int unsigned               AXI_USER_WIDTH     = 32'd0,
  parameter int unsigned               AXI_ID_WIDTH       = 32'd0,
  parameter int unsigned               AXI_SLV_ID_WIDTH   = 32'd0,
  parameter int unsigned               JOB_FIFO_DEPTH     = 32'd0,
  parameter int unsigned               NUM_AX_IN_FLIGHT   = 32'd0,
  parameter int unsigned               MEM_SYS_DEPTH      = 32'd0,
  parameter int unsigned               BUFFER_DEPTH       = 32'h3, // Default = 3
  parameter bit                        RAW_COUPLING_AVAIL =  1'b0,
  parameter bit                        OBSERVER_PORT    = 1'b0,
  parameter bit BATCH_PORT = 1'b0,
  parameter bit                        IS_TWO_D           =  1'b0,

  parameter logic [23:0]               DEVICE_ID          = 24'd1,
  parameter logic [19:0]               PROCESS_ID         = 20'd0,
  parameter logic [(AXI_ID_WIDTH-1):0] AxID               =     0,
  parameter bit PnEnable = 1'b0,
  parameter bit PnTapEnable = 1'b0,
  parameter bit PnGapEnable = 1'b0
) (
  input  logic         clk_i,
  input  logic         rst_ni,
  input  logic         testmode_i,
    // Read-only event taps and optional observer register window.
    output logic obs_issue_o, obs_done_o, obs_poll_o,
  output logic batch_active_o,
    output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
    output logic obs_cfg_valid_o, obs_cfg_write_o,
    output logic [6:0] obs_cfg_addr_o,
    output logic [63:0] obs_cfg_wdata_o,
    output logic [7:0] obs_cfg_wstrb_o,
    input logic [63:0] obs_cfg_rdata_i,
  AXI_BUS_EXT.Master   axi_master,
  AXI_BUS.Slave        axi_slave,
  input logic pn_nonblock_enable_i = 1'b0,
  output logic pn_fullbuffer_permit_o, pn_backend_job_quiet_o,
  output logic pn_certificate_pending_o, pn_certificate_fault_o,
  output logic pn_stop_accepted_o, pn_frontend_quiet_o
);

  typedef logic [AXI_ADDR_WIDTH-1:0]     addr_t;
  typedef logic [AXI_DATA_WIDTH-1:0]     data_t;
  typedef logic [(AXI_DATA_WIDTH/8)-1:0] strb_t;
  typedef logic [AXI_USER_WIDTH-1:0]     user_t;
  typedef logic [AXI_ID_WIDTH-1:0]       axi_id_t;
  typedef logic [AXI_SLV_ID_WIDTH-1:0]   axi_slv_id_t;

  /*** Define extended request structs ***/
  typedef logic [24-1:0]  iommu_sid_t;
  typedef logic           iommu_ssidv_t;
  typedef logic [20-1:0]  iommu_ssid_t;

  `AXI_TYPEDEF_EXT_ALL(axi_mst, addr_t, axi_id_t, data_t, strb_t, user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)
  axi_mst_req_t axi_mst_req;
  axi_mst_resp_t axi_mst_resp;
  `AXI_ASSIGN_FROM_REQ_EXT(axi_master, axi_mst_req)
  `AXI_ASSIGN_TO_RESP(axi_mst_resp, axi_master)

  `AXI_TYPEDEF_ALL(axi_slv, addr_t, axi_slv_id_t, data_t, strb_t, user_t)
  axi_slv_req_t axi_slv_req;
  axi_slv_resp_t axi_slv_resp;
  `AXI_ASSIGN_TO_REQ(axi_slv_req, axi_slave)
  `AXI_ASSIGN_FROM_RESP(axi_slave, axi_slv_resp)

  dma_core_wrap #(
    .AxiAddrWidth     ( AXI_ADDR_WIDTH     ),
    .AxiDataWidth     ( AXI_DATA_WIDTH     ),
    .AxiUserWidth     ( AXI_USER_WIDTH     ),
    .AxiIdWidth       ( AXI_ID_WIDTH       ),
    .AxiSlvIdWidth    ( AXI_SLV_ID_WIDTH   ),
    .JobFifoDepth     ( JOB_FIFO_DEPTH     ),
    .BufferDepth      ( BUFFER_DEPTH       ),
    .NumAxInFlight    ( NUM_AX_IN_FLIGHT   ),
    .MemSysDepth      ( MEM_SYS_DEPTH      ),
    .RAWCouplingAvail ( RAW_COUPLING_AVAIL ),
    .IsTwoD           ( IS_TWO_D           ),
    .ObserverPort     ( OBSERVER_PORT      ),
    .BatchPort        ( BATCH_PORT         ),
    .axi_mst_req_t    ( axi_mst_req_t      ),
    .axi_mst_rsp_t    ( axi_mst_resp_t     ),
    .axi_slv_req_t    ( axi_slv_req_t      ),
    .axi_slv_rsp_t    ( axi_slv_resp_t     ),
    .DEVICE_ID        ( DEVICE_ID          ),
    .PROCESS_ID       ( PROCESS_ID         ),
    .AxID             ( AxID               ),
    .PnEnable         ( PnEnable           ),
    .PnTapEnable      ( PnTapEnable        ),
    .PnGapEnable      ( PnGapEnable        )
  ) i_dma_core_wrap (
      .obs_issue_o, .obs_done_o, .obs_poll_o, .batch_active_o,
      .obs_issue_id_o, .obs_done_id_o, .obs_poll_id_o,
      .obs_cfg_valid_o, .obs_cfg_write_o, .obs_cfg_addr_o,
      .obs_cfg_wdata_o, .obs_cfg_wstrb_o, .obs_cfg_rdata_i,
    .clk_i,
    .rst_ni,
    .testmode_i,
    .pn_nonblock_enable_i, .pn_fullbuffer_permit_o, .pn_backend_job_quiet_o,
    .pn_certificate_pending_o, .pn_certificate_fault_o,
    .pn_stop_accepted_o, .pn_frontend_quiet_o,
    .axi_mst_req_o ( axi_mst_req  ),
    .axi_mst_rsp_i ( axi_mst_resp ),
    .axi_slv_req_i ( axi_slv_req  ),
    .axi_slv_rsp_o ( axi_slv_resp )
  );

endmodule : dma_core_wrap_intf
