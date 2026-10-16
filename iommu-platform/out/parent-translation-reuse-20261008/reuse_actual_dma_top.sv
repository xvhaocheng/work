// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_actual_dma_top.sv
//
// BOUNDED DIAGNOSTIC WRAPPER -- NOT PRODUCTION RTL.
//
// Structure
// ---------
//   * re-instantiates the existing scalar-I/O joint harness
//     tests/nonblock_banked_top.sv UNCHANGED and re-exports its complete
//     comp / ddtw / ptw scalarDDR ports and its 10 dbg taps verbatim;
//   * instantiates TWO real dma_core_wrap (non-intf, "type" flavour):
//       DMA0 = V : DEVICE_ID 10, PROCESS_ID 20, ObserverPort=1, ResearchPort=0
//       DMA1 = A : DEVICE_ID 11, PROCESS_ID 21, ObserverPort=0, ResearchPort=1
//     exactly as ariane_peripherals_xilinx.sv:1253-1306 does for i==0/1,
//     with the real board parameters:
//       JobFifoDepth 8, BufferDepth 512, NumAxInFlight 8, MemSysDepth 0,
//       RAWCouplingAvail 0, IsTwoD 0, BatchPort 1, LaunchPort 1, AxID 0;
//   * muxes the two real translation-request masters with the real 4-input
//     axi_mux used on the FPGA (2 live ports + 2 tied to zero) so the AxID
//     carries the bank index exactly as the board does
//     (SlvAxiIDWidth=2, NoSlvPorts=4 => master id width 4).
//     nonblock_banked_top.BankIdBit is already 2 (line 628) and is untouched;
//   * default fullbuffer_jobs_i uses the REAL private-capacity certificate;
//     optional SharedPoolMode substitutes a per-response RTL reservation;
//   * exposes one test-only configuration AXI slave selector that drives
//     EXACTLY ONE dma_core_wrap axi_slv port at a time.  No frontend register
//     is ever forced and the real backend is never bypassed: the FSM performs a
//     real AW/W/B or AR/R AXI transaction on the selected slave.
//
// Observation-only taps do not drive anything.

`include "axi/typedef.svh"
`include "axi/assign.svh"
`include "axi-iommu/typedef.svh"

module nonblock_actual_dma_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter bit          NonblockEnable       = 1,
    parameter int unsigned NumOutstandingTrans = 8,
    parameter int unsigned WFifoDepth          = 32,
    // Test-only elaboration switch, forwarded verbatim to the harness.
    parameter bit          PolicyEnable        = 0,
    parameter int unsigned DmaBufferDepth      = 512,
    parameter int unsigned SharedPoolMode      = 0,
    parameter bit InterleaveAwEnable = 0,
    parameter int unsigned InterleaveAwDelay = 64,
    parameter bit SameBankDmasEnable = 0,
    parameter bit ReverseReadOrderEnable = 0,
    parameter bit SourceReadGateEnable = 0,
    parameter bit FrontierEnable = 0,
    parameter bit FrontierScanEnable = 1,
    parameter int unsigned HeaderParkMode = 0,
    parameter int unsigned HeaderParkDepth = 2,
    parameter bit HeaderDirectEnable = 1,
    parameter int unsigned PoolSlots           = 1
) (
    input  logic clk_i,
    input  logic rst_ni,
    input logic reuse_enable_i,

    // ---- runtime mode inputs (forwarded to the harness) --------------------
    input  logic nonblock_enable_i,
    input  logic policy_partition_i,
    // Test-only ordinary baseline: suppress non-owner full-buffer permission.
    // The actual DMA buffer capacity and certificate observation stay intact.
    input  logic owner_only_i,

    // ---------------- completion AXI  (re-export of nonblock_banked_top) ----
    output logic         comp_aw_valid,
    output logic [3:0]   comp_aw_id,
    output logic [63:0]  comp_aw_addr,
    output logic [7:0]   comp_aw_len,
    output logic [2:0]   comp_aw_size,
    output logic [1:0]   comp_aw_burst,
    output logic [2:0]   comp_aw_prot,
    output logic [0:0]   comp_aw_user,

    output logic         comp_ar_valid,
    output logic [3:0]   comp_ar_id,
    output logic [63:0]  comp_ar_addr,
    output logic [7:0]   comp_ar_len,
    output logic [2:0]   comp_ar_size,
    output logic [1:0]   comp_ar_burst,
    output logic [2:0]   comp_ar_prot,
    output logic [0:0]   comp_ar_user,

    output logic         comp_w_valid,
    output logic [63:0]  comp_w_data,
    output logic [7:0]   comp_w_strb,
    output logic         comp_w_last,

    output logic         comp_b_ready,
    output logic         comp_r_ready,

    input  logic         comp_aw_ready,
    input  logic         comp_ar_ready,
    input  logic         comp_w_ready,
    input  logic         comp_b_valid,
    input  logic [3:0]   comp_b_id,
    input  logic [1:0]   comp_b_resp,
    input  logic         comp_r_valid,
    input  logic [3:0]   comp_r_id,
    input  logic [63:0]  comp_r_data,
    input  logic [1:0]   comp_r_resp,
    input  logic         comp_r_last,

    // ---------------- DDTW AXI read port -----------------------------------
    output logic         ddtw_ar_valid,
    output logic [63:0]  ddtw_ar_addr,
    output logic [3:0]   ddtw_ar_id,
    output logic [7:0]   ddtw_ar_len,
    output logic [2:0]   ddtw_ar_size,
    output logic [1:0]   ddtw_ar_burst,
    input  logic         ddtw_ar_ready,
    output logic         ddtw_r_ready,
    input  logic         ddtw_r_valid,
    input  logic [63:0]  ddtw_r_data,
    input  logic [3:0]   ddtw_r_id,
    input  logic [1:0]   ddtw_r_resp,
    input  logic         ddtw_r_last,

    // ---------------- PTW AXI read port -------------------------------------
    output logic         ptw_ar_valid,
    output logic [63:0]  ptw_ar_addr,
    output logic [3:0]   ptw_ar_id,
    output logic [7:0]   ptw_ar_len,
    output logic [2:0]   ptw_ar_size,
    output logic [1:0]   ptw_ar_burst,
    input  logic         ptw_ar_ready,
    output logic         ptw_r_ready,
    input  logic         ptw_r_valid,
    input  logic [63:0]  ptw_r_data,
    input  logic [3:0]   ptw_r_id,
    input  logic [1:0]   ptw_r_resp,
    input  logic         ptw_r_last,

    // ---------------- TL observation (re-export of nonblock_banked_top) -----
    output logic        dbg_tl_req_valid,
    output logic        dbg_tl_req_ready,
    output logic        dbg_tl_req_bank,
    output logic        dbg_tl_resp_valid,
    output logic        dbg_tl_resp_ready,
    output logic        dbg_tl_resp_bank,
    output logic        dbg_tl_ptw_active,
    output logic        dbg_tl_ptw_r_fire,
    output logic        dbg_tl_iotlb_lookup,
    output logic        dbg_tl_iotlb_hit,

    // ---------------- test-only configuration AXI slave selector ------------
    // cfg_sel: 0 = DMA0 (V), 1 = DMA1 (A).  cfg_* is a single-beat AXI4 access
    // on the selected dma_core_wrap axi_slv port.  Inputs must be held stable
    // until cfg_ready is seen.
    input  logic         cfg_sel,
    input  logic         cfg_valid,
    input  logic         cfg_write,
    input  logic [63:0]  cfg_addr,
    input  logic [63:0]  cfg_wdata,
    input  logic [7:0]   cfg_wstrb,
    output logic         cfg_ready,
    output logic [63:0]  cfg_rdata,
    output logic         cfg_error,

    // ---------------- DMA observer taps (observation only, no feedback) -----
    output logic [1:0]  dbg_dma_issue,
    output logic [1:0]  dbg_dma_done,
    output logic [1:0]  dbg_dma_poll,
    output logic [1:0]  dbg_dma_batch_active,
    output logic [1:0]  dbg_dma_batch_gap,
    output logic [1:0]  dbg_dma_job_quiet,
    output logic [1:0]  dbg_dma_fullbuffer,
    output logic [1:0]  dbg_dma_launch_active,
    output logic [1:0]  dbg_dma_launch_held,
    output logic [1:0]  dbg_dma_launch_error,

    // ---------------- nonblocking lifecycle taps (read-only) ----------------
    // Re-exported from nonblock_banked_top's control/trace outputs so the S/N
    // (nonblocking disabled/enabled) paths are observable rather than open.
    output logic        dbg_nonblock_fire,
    output logic        dbg_nonblock_busy,
    output logic        dbg_in_flight,

    // ---------------- fast/main + 2-bank hierarchical taps (read-only) ------
    // Real instance names (for lookup):
    //   i_sys             : nonblock_banked_top          (tests/nonblock_banked_top.sv)
    //   i_sys.gen_banked.i_banked : rv_iommu_banked_ctl  (bank_busy[1:0])
    //   i_sys.i_tl        : rv_iommu_tl_wrap             (fast_resp_*/slow_resp_valid)
    //   i_sys.i_tl.gen_nonblock_hit.i_hit : rv_iommu_nonblock_hit
    // These are pure observations; nothing here drives the DUT.
    output logic        dbg_fast_resp_valid,
    output logic        dbg_fast_resp_ready,
    output logic        dbg_fast_resp_bank,
    output logic        dbg_slow_resp_valid,
    output logic [1:0]  dbg_bank_busy,
    // Real registered cfg-FSM state (never a hard-wired 0).
    output logic [31:0] dbg_e2_mode,
    output logic [31:0] dbg_e2_slots,
    output logic [31:0] dbg_e2_dma_depth,
    output logic [31:0] dbg_e2_occupancy,
    output logic [31:0] dbg_e2_reserved,
    output logic [31:0] dbg_e2_busy,
    output logic [31:0] dbg_e2_fault,
    output logic [31:0] dbg_e2_grant_pool,
    output logic [31:0] dbg_e2_grant_stream,
    output logic [31:0] dbg_e2_received,
    output logic [31:0] dbg_e2_replayed,
    output logic [31:0] dbg_e2_highwater,
    output logic [31:0] dbg_e2_grant_future,
    output logic [31:0] dbg_e2_aw_fixture_stall, dbg_e2_tc_read_wait, dbg_e2_tc_aw_pending, dbg_e2_tc_state,
    output logic [31:0] dbg_e2_tc_spill, dbg_e2_tc_ar_head,
    output logic [31:0] dbg_e2_tc_queue_ids,
    output logic [31:0] dbg_hp_parked, dbg_hp_bypassed, dbg_hp_full_bypass,
    output logic [31:0] dbg_hp_highwater, dbg_hp_queued, dbg_hp_dispatched,
    output logic [31:0] dbg_e2_ar_fixture_stall,
    output logic [31:0] dbg_frontier_grant,
    output logic [31:0] dbg_frontier_wait,
    output logic [31:0] dbg_e2_meta_accepted,
    output logic [31:0] dbg_e2_meta_early,
    output logic [31:0] dbg_e2_meta_highwater,
    output logic [31:0] dbg_e2_queued,
    output logic [2:0]  dbg_cfg_state
);

    // ------------------------------------------------------------------------
    // Fixed widths
    // ------------------------------------------------------------------------
    localparam int unsigned AxiAddrWidth   = 64;
    localparam int unsigned AxiDataWidth   = 64;
    localparam int unsigned AxiUserWidth   = 1;
    // DmaAxiIdWidth on the FPGA = ariane_soc::IdWidth - $clog2(4) = 4 - 2 = 2
    localparam int unsigned DmaAxiIdWidth  = 2;
    localparam int unsigned MuxAxiIdWidth  = 4;   // 2 + $clog2(4)
    localparam int unsigned CfgBusIDWidth  = 6;   // ariane_soc::IdWidthSlave

    typedef logic [AxiAddrWidth-1:0]      addr_t;
    typedef logic [AxiDataWidth-1:0]      data_t;
    typedef logic [AxiDataWidth/8-1:0]    strb_t;
    typedef logic [AxiUserWidth-1:0]      user_t;
    typedef logic [DmaAxiIdWidth-1:0]     dma_id_t;
    typedef logic [MuxAxiIdWidth-1:0]     mux_id_t;
    typedef logic [CfgBusIDWidth-1:0]     cfg_id_t;
    typedef logic [23:0]                  did_t;
    typedef logic                         ssidv_t;
    typedef logic [19:0]                  subid_t;

    // ------------------------------------------------------------------------
    // DMA translation-request bus types (id width 2, as the FPGA DmaAxiIdWidth)
    // ------------------------------------------------------------------------
    `AXI_TYPEDEF_EXT_ALL(axi_tr2, addr_t, dma_id_t, data_t, strb_t, user_t,
                         did_t, ssidv_t, subid_t)
    // ------------------------------------------------------------------------
    // Mux master bus types (id width 4 after axi_id_prepend)
    // ------------------------------------------------------------------------
    `AXI_TYPEDEF_EXT_ALL(axi_tr4, addr_t, mux_id_t, data_t, strb_t, user_t,
                         did_t, ssidv_t, subid_t)

    // ------------------------------------------------------------------------
    // Configuration AXI slave bus types (id width 6 == ariane_soc::IdWidthSlave)
    // ------------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(cfg_aw_chan_t, addr_t, cfg_id_t, user_t)
    `AXI_TYPEDEF_W_CHAN_T (cfg_w_chan_t,  data_t, strb_t, user_t)
    `AXI_TYPEDEF_B_CHAN_T (cfg_b_chan_t,  cfg_id_t, user_t)
    `AXI_TYPEDEF_AR_CHAN_T(cfg_ar_chan_t, addr_t, cfg_id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T (cfg_r_chan_t,  data_t, cfg_id_t, user_t)
    `AXI_TYPEDEF_REQ_T    (cfg_req_t,  cfg_aw_chan_t, cfg_w_chan_t, cfg_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (cfg_resp_t, cfg_b_chan_t,  cfg_r_chan_t)

    // ------------------------------------------------------------------------
    // Mux plumbing
    // ------------------------------------------------------------------------
    axi_tr2_req_t  [3:0] slv_reqs;
    axi_tr2_resp_t [3:0] slv_resps;
    axi_tr4_req_t        mux_mst_req;
    axi_tr4_resp_t       mux_mst_resp;

    // ------------------------------------------------------------------------
    // Harness translation-request ports (individual bits, unpacked below)
    // ------------------------------------------------------------------------
    logic        tr_aw_valid;
    logic [3:0]  tr_aw_id;
    logic [63:0] tr_aw_addr;
    logic [7:0]  tr_aw_len;
    logic [2:0]  tr_aw_size;
    logic [1:0]  tr_aw_burst;
    logic [2:0]  tr_aw_prot;
    logic [0:0]  tr_aw_user;
    logic [23:0] tr_aw_did;
    logic        tr_aw_ssid_valid;
    logic [19:0] tr_aw_subid;
    logic        tr_aw_mmu_atst;
    logic        tr_aw_mmu_valid;

    logic        tr_ar_valid;
    logic [3:0]  tr_ar_id;
    logic [63:0] tr_ar_addr;
    logic [7:0]  tr_ar_len;
    logic [2:0]  tr_ar_size;
    logic [1:0]  tr_ar_burst;
    logic [2:0]  tr_ar_prot;
    logic [0:0]  tr_ar_user;
    logic [23:0] tr_ar_did;
    logic        tr_ar_ssid_valid;
    logic [19:0] tr_ar_subid;
    logic        tr_ar_mmu_atst;
    logic        tr_ar_mmu_valid;

    logic        tr_w_valid;
    logic [63:0] tr_w_data;
    logic [7:0]  tr_w_strb;
    logic        tr_w_last;
    logic [0:0]  tr_w_user;

    logic        tr_b_ready;
    logic        tr_r_ready;

    logic        tr_aw_ready;
    logic        tr_ar_ready;
    logic        tr_w_ready;
    logic        tr_b_valid;
    logic [3:0]  tr_b_id;
    logic [1:0]  tr_b_resp;
    logic        tr_r_valid;
    logic [3:0]  tr_r_id;
    logic [63:0] tr_r_data;
    logic [1:0]  tr_r_resp;
    logic        tr_r_last;

    // ------------------------------------------------------------------------
    // Real DMA masters
    // ------------------------------------------------------------------------
    axi_tr2_req_t  dma0_mst_req, dma1_mst_req;
    axi_tr2_resp_t dma0_mst_rsp, dma1_mst_rsp;

    // Config slave plumbing: exactly one of cfg_req[0]/cfg_req[1] is live.
    cfg_req_t  [1:0] cfg_req;
    cfg_resp_t [1:0] cfg_rsp;

    // DMA observer taps
    logic        d0_issue, d0_done, d0_poll, d0_batch_active, d0_batch_gap;
    logic        d0_job_quiet, d0_fullbuffer, d0_launch_active, d0_launch_held, d0_launch_err;
    logic        d1_issue, d1_done, d1_poll, d1_batch_active, d1_batch_gap;
    logic        d1_job_quiet, d1_fullbuffer, d1_launch_active, d1_launch_held, d1_launch_err;

    logic [1:0]  fullbuffer_jobs_o;   // 2-bit harness permit, certificate-driven

    // ========================================================================
    // The two real frontend+backend wrappers
    // ========================================================================
    dma_core_wrap #(
        .AxiAddrWidth     ( AxiAddrWidth    ),
        .AxiDataWidth     ( AxiDataWidth    ),
        .AxiIdWidth       ( DmaAxiIdWidth   ),
        .AxiUserWidth     ( AxiUserWidth    ),
        .AxiSlvIdWidth    ( CfgBusIDWidth   ),
        .NumAxInFlight    ( 8               ),
        .MemSysDepth      ( 0               ),
        .JobFifoDepth     ( 8               ),
        .BufferDepth      ( DmaBufferDepth  ),
        .RAWCouplingAvail ( 1'b0            ),
        .ObserverPort     ( 1'b1            ),   // only V
        .BatchPort        ( 1'b1            ),
        .LaunchPort       ( 1'b1            ),
        .ResearchPort     ( 1'b0            ),   // only A
        .IsTwoD           ( 1'b0            ),
        .axi_mst_req_t    ( axi_tr2_req_t   ),
        .axi_mst_rsp_t    ( axi_tr2_resp_t  ),
        .axi_slv_req_t    ( cfg_req_t       ),
        .axi_slv_rsp_t    ( cfg_resp_t      ),
        .DEVICE_ID        ( 24'd10          ),
        .PROCESS_ID       ( 20'd20          ),
        .AxID             ( 2'd0            )
    ) i_dma0 (
        .clk_i, .rst_ni,
        .testmode_i     ( 1'b0              ),
        .obs_issue_o    ( d0_issue          ),
        .obs_done_o     ( d0_done           ),
        .obs_poll_o     ( d0_poll           ),
        .batch_active_o ( d0_batch_active   ),
        .batch_gap_active_o ( d0_batch_gap  ),
        .job_quiet_o    ( d0_job_quiet      ),
        .fullbuffer_job_o ( d0_fullbuffer   ),
        .policy_enable_i( 1'b0              ),
        .policy_allow_i ( 1'b1              ),
        .policy_abort_i ( 1'b0              ),
        .batch_reverse_i( 1'b0              ),
        .launch_hold_i  ( 1'b0              ),
        .launch_go_i    ( 1'b0              ),
        .launch_cancel_i( 1'b0              ),
        .launch_held_o  ( d0_launch_held    ),
        .launch_active_o( d0_launch_active  ),
        .launch_cancelled_o ( /* open */    ),
        .launch_error_o ( d0_launch_err     ),
        .obs_issue_id_o ( /* open */        ),
        .obs_done_id_o  ( /* open */        ),
        .obs_poll_id_o  ( /* open */        ),
        .obs_cfg_valid_o( /* open */        ),
        .obs_cfg_write_o( /* open */        ),
        .obs_cfg_addr_o ( /* open */        ),
        .obs_cfg_wdata_o( /* open */        ),
        .obs_cfg_wstrb_o( /* open */        ),
        .obs_cfg_rdata_i( 64'd0             ),
        .axi_mst_req_o  ( dma0_mst_req      ),
        .axi_mst_rsp_i  ( dma0_mst_rsp      ),
        .axi_slv_req_i  ( cfg_req[0]        ),
        .axi_slv_rsp_o  ( cfg_rsp[0]        )
    );

    dma_core_wrap #(
        .AxiAddrWidth     ( AxiAddrWidth    ),
        .AxiDataWidth     ( AxiDataWidth    ),
        .AxiIdWidth       ( DmaAxiIdWidth   ),
        .AxiUserWidth     ( AxiUserWidth    ),
        .AxiSlvIdWidth    ( CfgBusIDWidth   ),
        .NumAxInFlight    ( 8               ),
        .MemSysDepth      ( 0               ),
        .JobFifoDepth     ( 8               ),
        .BufferDepth      ( DmaBufferDepth  ),
        .RAWCouplingAvail ( 1'b0            ),
        .ObserverPort     ( 1'b0            ),   // only V
        .BatchPort        ( 1'b1            ),
        .LaunchPort       ( 1'b1            ),
        .ResearchPort     ( 1'b1            ),   // only A
        .IsTwoD           ( 1'b0            ),
        .axi_mst_req_t    ( axi_tr2_req_t   ),
        .axi_mst_rsp_t    ( axi_tr2_resp_t  ),
        .axi_slv_req_t    ( cfg_req_t       ),
        .axi_slv_rsp_t    ( cfg_resp_t      ),
        .DEVICE_ID        ( 24'd11          ),
        .PROCESS_ID       ( 20'd21          ),
        .AxID             ( 2'd0            )
    ) i_dma1 (
        .clk_i, .rst_ni,
        .testmode_i     ( 1'b0              ),
        .obs_issue_o    ( d1_issue          ),
        .obs_done_o     ( d1_done           ),
        .obs_poll_o     ( d1_poll           ),
        .batch_active_o ( d1_batch_active   ),
        .batch_gap_active_o ( d1_batch_gap  ),
        .job_quiet_o    ( d1_job_quiet      ),
        .fullbuffer_job_o ( d1_fullbuffer   ),
        .policy_enable_i( 1'b0              ),
        .policy_allow_i ( 1'b1              ),
        .policy_abort_i ( 1'b0              ),
        .batch_reverse_i( 1'b0              ),
        .launch_hold_i  ( 1'b0              ),
        .launch_go_i    ( 1'b0              ),
        .launch_cancel_i( 1'b0              ),
        .launch_held_o  ( d1_launch_held    ),
        .launch_active_o( d1_launch_active  ),
        .launch_cancelled_o ( /* open */    ),
        .launch_error_o ( d1_launch_err     ),
        .obs_issue_id_o ( /* open */        ),
        .obs_done_id_o  ( /* open */        ),
        .obs_poll_id_o  ( /* open */        ),
        .obs_cfg_valid_o( /* open */        ),
        .obs_cfg_write_o( /* open */        ),
        .obs_cfg_addr_o ( /* open */        ),
        .obs_cfg_wdata_o( /* open */        ),
        .obs_cfg_wstrb_o( /* open */        ),
        .obs_cfg_rdata_i( 64'd0             ),
        .axi_mst_req_o  ( dma1_mst_req      ),
        .axi_mst_rsp_i  ( dma1_mst_rsp      ),
        .axi_slv_req_i  ( cfg_req[1]        ),
        .axi_slv_rsp_o  ( cfg_rsp[1]        )
    );

    // ========================================================================
    // Real 4-input axi_mux : 2 live DMA ports + 2 tied off
    // ========================================================================
    axi_tr2_req_t [1:0] pool_s_req, pool_m_req;
    axi_tr2_resp_t [1:0] pool_s_rsp, pool_m_rsp;
    logic [1:0] pool_permission;
    logic [1:0] late_comp_ar_admit, late_comp_ar_valid, late_comp_ar_fire;
    logic physical_w_stream;
    logic [7:0] physical_stream_order_q;
    logic [2:0] physical_stream_rd_q, physical_stream_wr_q;
    // Independently track DMA identity at the real completion AW first offer.
    // A shared bank mux port alone no longer identifies its W source stream.
    assign physical_w_stream = SameBankDmasEnable ?
        physical_stream_order_q[physical_stream_rd_q] :
        i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_data[0];
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            physical_stream_order_q <= '0;
            physical_stream_rd_q <= '0;
            physical_stream_wr_q <= '0;
        end else begin
            if (i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push) begin
                physical_stream_order_q[physical_stream_wr_q] <=
                    i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.mst_aw_chan.id[2];
                physical_stream_wr_q <= physical_stream_wr_q + 1'b1;
            end
            if (comp_w_valid && comp_w_ready && comp_w_last)
                physical_stream_rd_q <= physical_stream_rd_q + 1'b1;
        end
    end
    initial if (SameBankDmasEnable && SharedPoolMode > 0 && SharedPoolMode < 4)
        $fatal(1, "same-bank DMA experiment requires the completion gate model");
    logic [1:0][3:0] late_comp_ar_id;
    logic [1:0][63:0] late_comp_ar_addr;
    logic [1:0][7:0] late_comp_ar_len;
    logic [1:0][2:0] late_comp_ar_size;
    logic [1:0] harness_capacity_permission;
    // Shared stimulus shim applies identical legal AR timing to the ordinary
    // private-buffer baseline and the completion-admission candidate.
    logic ar_fixture_active_q, ar_fixture_released_q, ar_fixture_wait_q;
    logic [31:0] ar_fixture_age_q;
    axi_tr2_ar_chan_t ar_fixture_held_q;
    logic ar_fixture_block;
    assign ar_fixture_block = ReverseReadOrderEnable && ar_fixture_active_q &&
        ar_fixture_age_q < 64 && !ar_fixture_released_q;
    always_comb begin
        pool_s_req = {dma1_mst_req, dma0_mst_req};
        pool_s_req[1].ar_valid = dma1_mst_req.ar_valid && !ar_fixture_block;
        dma0_mst_rsp = pool_s_rsp[0];
        dma1_mst_rsp = pool_s_rsp[1];
        dma1_mst_rsp.ar_ready = pool_s_rsp[1].ar_ready && !ar_fixture_block;
    end
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            ar_fixture_active_q <= 0; ar_fixture_released_q <= 0;
            ar_fixture_wait_q <= 0; ar_fixture_age_q <= 0;
            ar_fixture_held_q <= '0; dbg_e2_ar_fixture_stall <= 0;
        end else begin
            if (ar_fixture_active_q) ar_fixture_age_q <= ar_fixture_age_q + 1;
            dbg_e2_ar_fixture_stall <= dbg_e2_ar_fixture_stall +
                32'(ar_fixture_block && dma1_mst_req.ar_valid);
            if (pool_s_req[1].ar_valid) ar_fixture_released_q <= 1;
            if (ReverseReadOrderEnable && ar_fixture_wait_q &&
                (!pool_s_req[1].ar_valid || pool_s_req[1].ar != ar_fixture_held_q))
                $fatal(1, "read-order fixture withdrew or changed stalled AR");
            ar_fixture_wait_q <= pool_s_req[1].ar_valid && !pool_s_rsp[1].ar_ready;
            ar_fixture_held_q <= pool_s_req[1].ar;
            if (i_dma1.idma_rsp_valid && i_dma1.idma_rsp_ready) ar_fixture_active_q <= 0;
            if (i_dma1.be_valid_d && i_dma1.be_ready_d) begin
                ar_fixture_active_q <= 1; ar_fixture_released_q <= 0;
                ar_fixture_age_q <= 0;
            end
        end
    end
    logic [1:0] pre_flow_ready;
    logic pre_owner, pre_parent_valid, pre_parent_ready;
    logic pre_ar_valid, pre_r_ready, pre_r_valid;
    axi_tr4_ar_chan_t pre_parent_ar, pre_ar;
    axi_tr4_r_chan_t pre_raw_r, pre_r;
    logic [31:0] pre_parent_count, pre_child_count, pre_merged, pre_lasts;
    always_comb begin
        pre_owner = !(pool_m_req[0].ar_valid && pre_flow_ready[0]);
        pre_parent_valid = pool_m_req[pre_owner].ar_valid && pre_flow_ready[pre_owner];
        pre_parent_ar = axi_tr4_ar_chan_t'(pool_m_req[pre_owner].ar);
        pre_parent_ar.id = {1'b0, pre_owner, pool_m_req[pre_owner].ar.id};
        pool_m_rsp = {slv_resps[1], slv_resps[0]};
        pool_m_rsp[0].ar_ready = pre_parent_valid && pre_parent_ready && !pre_owner;
        pool_m_rsp[1].ar_ready = pre_parent_valid && pre_parent_ready && pre_owner;
    end
    always_comb begin
        slv_reqs[0] = pool_m_req[0]; slv_reqs[1] = pool_m_req[1];
        slv_reqs[0].ar_valid = 0; slv_reqs[1].ar_valid = 0;
    end
    assign slv_reqs[2] = '0;
    assign slv_reqs[3] = '0;
    assign dbg_e2_mode = SharedPoolMode;
    assign dbg_e2_slots = (SharedPoolMode == 0 || SharedPoolMode == 7) ? 0 : PoolSlots;
    assign dbg_e2_dma_depth = DmaBufferDepth;
    // Production fullbuffer certificates continue to report real private
    // capacity. The extra test permission is per reserved response, not job.
    assign harness_capacity_permission = SharedPoolMode == 0 ?
        (fullbuffer_jobs_o & {2{!owner_only_i}}) : pool_permission;
    if (SharedPoolMode == 0) begin : gen_no_pool
        assign pool_m_req = pool_s_req;
        assign pool_s_rsp = pool_m_rsp;
        assign pool_permission = '0;
        assign dbg_e2_occupancy = 0;
        assign dbg_e2_reserved = 0;
        assign dbg_e2_busy = 0;
        assign dbg_e2_fault = 0;
        assign dbg_e2_grant_pool = 0;
        assign dbg_e2_grant_stream = 0;
        assign dbg_e2_received = 0;
        assign dbg_e2_replayed = 0;
        assign dbg_e2_highwater = 0;
        assign late_comp_ar_admit = '0;
        assign dbg_e2_aw_fixture_stall = 0;
        assign dbg_frontier_grant = 0;
        assign dbg_frontier_wait = 0;
        assign dbg_e2_grant_future = 0;
        assign dbg_e2_meta_accepted = 0;
        assign dbg_e2_meta_early = 0;
        assign dbg_e2_meta_highwater = 0;
        assign dbg_e2_queued = 0;
    end else if (SharedPoolMode < 4) begin : gen_pool
        assign late_comp_ar_admit = '0;
        assign dbg_e2_aw_fixture_stall = 0;
        assign dbg_frontier_grant = 0;
        assign dbg_frontier_wait = 0;
        assign dbg_e2_grant_future = 0;
        assign dbg_e2_meta_accepted = 0;
        assign dbg_e2_meta_early = 0;
        assign dbg_e2_meta_highwater = 0;
        assign dbg_e2_queued = 0;

        nonblock_completion_pool #(
            .Mode(SharedPoolMode), .Slots(PoolSlots),
            .req_t(axi_tr2_req_t), .rsp_t(axi_tr2_resp_t),
            .ar_t(axi_tr2_ar_chan_t), .r_t(axi_tr2_r_chan_t)
        ) i_pool (
            .clk_i, .rst_ni,
            .s_req_i(pool_s_req), .s_rsp_o(pool_s_rsp),
            .m_req_o(pool_m_req), .m_rsp_i(pool_m_rsp),
            .job_fire_i({i_dma1.be_valid_d && i_dma1.be_ready_d,
                         i_dma0.be_valid_d && i_dma0.be_ready_d}),
            .job_done_i({i_dma1.idma_rsp_valid && i_dma1.idma_rsp_ready,
                         i_dma0.idma_rsp_valid && i_dma0.idma_rsp_ready}),
            .job_normal_i({i_dma1.ttype == idma_pkg::DEFAULT,
                           i_dma0.ttype == idma_pkg::DEFAULT}),
            .job_src_i({64'(i_dma1.burst_req_d.src_addr), 64'(i_dma0.burst_req_d.src_addr)}),
            .job_dst_i({64'(i_dma1.burst_req_d.dst_addr), 64'(i_dma0.burst_req_d.dst_addr)}),
            .job_length_i({64'(i_dma1.burst_req_d.length), 64'(i_dma0.burst_req_d.length)}),
            .aw_offer_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
            .aw_bank_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]), .aw_addr_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
            .aw_len_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len), .aw_size_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
            .v_whead_valid_i(!i_dma_iommu_tr_mux.gen_mux.w_fifo_empty),
            .v_whead_bank_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0]),
            .v_wlast_i(tr_w_valid && tr_w_ready && tr_w_last),
            .p_aw_fire_i(comp_aw_valid && comp_aw_ready), .p_aw_bank_i(comp_aw_id[2]),
            .p_whead_valid_i(!i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_empty),
            .p_whead_bank_i(i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_data[0]),
            .p_wlast_i(comp_w_valid && comp_w_ready && comp_w_last),
            .reserved_permission_o(pool_permission),
            .occupancy_o(dbg_e2_occupancy), .reserved_o(dbg_e2_reserved),
            .busy_o(dbg_e2_busy), .fault_o(dbg_e2_fault),
            .grant_pool_o(dbg_e2_grant_pool), .grant_stream_o(dbg_e2_grant_stream),
            .received_o(dbg_e2_received), .replayed_o(dbg_e2_replayed),
            .highwater_o(dbg_e2_highwater)
        );
    end else begin : gen_late_pool
        assign pool_permission = '0;
        nonblock_completion_pool_late #(
            .Mode(SharedPoolMode), .Slots(PoolSlots),
            .InterleaveAwEnable(InterleaveAwEnable),
            .InterleaveAwDelay(InterleaveAwDelay),
            .SourceReadGateEnable(SourceReadGateEnable),
            .FrontierEnable(FrontierEnable),
            .FrontierScanEnable(FrontierScanEnable),
            .req_t(axi_tr2_req_t), .rsp_t(axi_tr2_resp_t),
            .ar_t(axi_tr2_ar_chan_t), .r_t(axi_tr2_r_chan_t)
        ) i_pool (
            .clk_i, .rst_ni,
            .s_req_i(pool_s_req), .s_rsp_o(pool_s_rsp),
            .m_req_o(pool_m_req), .m_rsp_i(pool_m_rsp),
            .job_fire_i({i_dma1.be_valid_d && i_dma1.be_ready_d,
                         i_dma0.be_valid_d && i_dma0.be_ready_d}),
            .job_done_i({i_dma1.idma_rsp_valid && i_dma1.idma_rsp_ready,
                         i_dma0.idma_rsp_valid && i_dma0.idma_rsp_ready}),
            .job_normal_i({i_dma1.ttype == idma_pkg::DEFAULT,
                           i_dma0.ttype == idma_pkg::DEFAULT}),
            .job_src_i({64'(i_dma1.burst_req_d.src_addr), 64'(i_dma0.burst_req_d.src_addr)}),
            .job_dst_i({64'(i_dma1.burst_req_d.dst_addr), 64'(i_dma0.burst_req_d.dst_addr)}),
            .job_length_i({64'(i_dma1.burst_req_d.length), 64'(i_dma0.burst_req_d.length)}),
            .aw_offer_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
            .aw_bank_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]), .aw_addr_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
            .aw_len_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len), .aw_size_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
            .v_whead_valid_i(!i_dma_iommu_tr_mux.gen_mux.w_fifo_empty),
            .v_whead_bank_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_data[0]),
            .v_wlast_i(tr_w_valid && tr_w_ready && tr_w_last),
            .p_aw_fire_i(comp_aw_valid && comp_aw_ready), .p_aw_bank_i(comp_aw_id[2]),
            .p_whead_valid_i(!i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_empty),
            .p_whead_bank_i(physical_w_stream),
            .p_wlast_i(comp_w_valid && comp_w_ready && comp_w_last),
            .p_aw_offer_i(i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.w_fifo_push),
            .p_aw_offer_bank_i(i_sys.gen_banked.i_banked.i_out_axi_mux.gen_mux.mst_aw_chan.id[2]),
            .completion_ar_valid_i(late_comp_ar_valid),
            .completion_ar_fire_i(late_comp_ar_fire),
            .completion_ar_id_i(late_comp_ar_id),
            .completion_ar_addr_i(late_comp_ar_addr),
            .completion_ar_len_i(late_comp_ar_len),
            .completion_ar_size_i(late_comp_ar_size),
            .completion_ar_admit_o(late_comp_ar_admit),
            .occupancy_o(dbg_e2_occupancy), .reserved_o(dbg_e2_reserved),
            .busy_o(dbg_e2_busy), .fault_o(dbg_e2_fault),
            .grant_pool_o(dbg_e2_grant_pool), .grant_stream_o(dbg_e2_grant_stream),
            .received_o(dbg_e2_received), .replayed_o(dbg_e2_replayed),
            .highwater_o(dbg_e2_highwater),
            .grant_future_o(dbg_e2_grant_future), .aw_fixture_stall_o(dbg_e2_aw_fixture_stall),
            .grant_frontier_o(dbg_frontier_grant),
            .frontier_wait_o(dbg_frontier_wait),
            .meta_accepted_o(dbg_e2_meta_accepted), .meta_early_o(dbg_e2_meta_early),
            .meta_highwater_o(dbg_e2_meta_highwater), .queued_o(dbg_e2_queued)
        );
    end
    // slv_resps[2]/[3] intentionally open (tied-off slaves never response).

    axi_mux #(
        .SlvAxiIDWidth ( DmaAxiIdWidth      ),
        .slv_aw_chan_t ( axi_tr2_aw_chan_t  ),
        .mst_aw_chan_t ( axi_tr4_aw_chan_t  ),
        .w_chan_t      ( axi_tr2_w_chan_t   ),
        .slv_b_chan_t  ( axi_tr2_b_chan_t   ),
        .mst_b_chan_t  ( axi_tr4_b_chan_t   ),
        .slv_ar_chan_t ( axi_tr2_ar_chan_t  ),
        .mst_ar_chan_t ( axi_tr4_ar_chan_t  ),
        .slv_r_chan_t  ( axi_tr2_r_chan_t   ),
        .mst_r_chan_t  ( axi_tr4_r_chan_t   ),
        .slv_req_t     ( axi_tr2_req_t      ),
        .slv_resp_t    ( axi_tr2_resp_t     ),
        .mst_req_t     ( axi_tr4_req_t      ),
        .mst_resp_t    ( axi_tr4_resp_t     ),
        .NoSlvPorts    ( 4                  ),
        .MaxWTrans     ( 8                  )
    ) i_dma_iommu_tr_mux (
        .clk_i,
        .rst_ni,
        .test_i      ( 1'b0             ),
        .slv_reqs_i  ( slv_reqs         ),
        .slv_resps_o ( slv_resps        ),
        .mst_req_o   ( mux_mst_req      ),
        .mst_resp_i  ( mux_mst_resp     )
    );

    // ========================================================================
    // Unpack mux master request -> harness request bits (field by field).
    // All stream/user/error-capable fields are preserved.
    // ========================================================================
    always_comb begin : unpack_mux_mst
        tr_aw_valid      = mux_mst_req.aw_valid;
        tr_aw_id         = mux_mst_req.aw.id;
        tr_aw_addr       = mux_mst_req.aw.addr;
        tr_aw_len        = mux_mst_req.aw.len;
        tr_aw_size       = mux_mst_req.aw.size;
        tr_aw_burst      = mux_mst_req.aw.burst;
        tr_aw_prot       = mux_mst_req.aw.prot;
        tr_aw_user       = mux_mst_req.aw.user;
        tr_aw_did        = mux_mst_req.aw.stream_id;
        tr_aw_ssid_valid = mux_mst_req.aw.ss_id_valid;
        tr_aw_subid      = mux_mst_req.aw.substream_id;
        tr_aw_mmu_atst   = mux_mst_req.aw.mmu_atst;
        tr_aw_mmu_valid  = mux_mst_req.aw.mmu_valid;

        tr_ar_valid      = pre_ar_valid;
        tr_ar_id         = pre_ar.id;
        tr_ar_addr       = pre_ar.addr;
        tr_ar_len        = pre_ar.len;
        tr_ar_size       = pre_ar.size;
        tr_ar_burst      = pre_ar.burst;
        tr_ar_prot       = pre_ar.prot;
        tr_ar_user       = pre_ar.user;
        tr_ar_did        = pre_ar.stream_id;
        tr_ar_ssid_valid = pre_ar.ss_id_valid;
        tr_ar_subid      = pre_ar.substream_id;
        tr_ar_mmu_atst   = pre_ar.mmu_atst;
        tr_ar_mmu_valid  = pre_ar.mmu_valid;

        tr_w_valid       = mux_mst_req.w_valid;
        tr_w_data        = mux_mst_req.w.data;
        tr_w_strb        = mux_mst_req.w.strb;
        tr_w_last        = mux_mst_req.w.last;
        tr_w_user        = mux_mst_req.w.user;

        tr_b_ready       = mux_mst_req.b_ready;
        tr_r_ready       = pre_r_ready;
    end

    // Harness response bits -> mux master response
    always_comb begin : pack_mux_mst_resp
        mux_mst_resp = '0;
        mux_mst_resp.aw_ready = tr_aw_ready;
        mux_mst_resp.ar_ready = tr_ar_ready;
        mux_mst_resp.w_ready  = tr_w_ready;
        mux_mst_resp.b_valid  = tr_b_valid;
        mux_mst_resp.b.id     = tr_b_id;
        mux_mst_resp.b.resp   = tr_b_resp;
        mux_mst_resp.b.user   = '0;
        mux_mst_resp.r_valid  = pre_r_valid;
        mux_mst_resp.r.id     = pre_r.id;
        mux_mst_resp.r.data   = pre_r.data;
        mux_mst_resp.r.resp   = pre_r.resp;
        mux_mst_resp.r.last   = pre_r.last;
        mux_mst_resp.r.user   = '0;
    end

    // ========================================================================
    // Certificate-driven full-buffer permit (NOT a static 2'b11).
    // Bit b == bank b full-buffer permit; bank0 = V = DMA0, bank1 = A = DMA1.
    // ========================================================================
    assign fullbuffer_jobs_o = { d1_fullbuffer, d0_fullbuffer };
`ifdef TEST_COMPLETION_GATE
    assign dbg_hp_parked = i_sys.gen_banked.i_banked.hp_parked;
    assign dbg_hp_bypassed = i_sys.gen_banked.i_banked.hp_bypassed;
    assign dbg_hp_full_bypass = i_sys.gen_banked.i_banked.hp_full_bypass;
    assign dbg_hp_highwater = i_sys.gen_banked.i_banked.hp_highwater;
    assign dbg_hp_queued = i_sys.gen_banked.i_banked.hp_queued;
    assign dbg_hp_dispatched = i_sys.gen_banked.i_banked.hp_dispatched;
`else
    assign dbg_hp_parked = 0; assign dbg_hp_bypassed = 0; assign dbg_hp_full_bypass = 0;
    assign dbg_hp_highwater = 0; assign dbg_hp_queued = 0; assign dbg_hp_dispatched = 0;
`endif

    // ========================================================================

    always_comb begin
        pre_raw_r = '0;
        pre_raw_r.id = tr_r_id; pre_raw_r.data = tr_r_data;
        pre_raw_r.resp = tr_r_resp; pre_raw_r.last = tr_r_last;
    end
    pretl_owner_fragment #(.ParentDepth(1), .ar_t(axi_tr4_ar_chan_t), .r_t(axi_tr4_r_chan_t)) i_pre (
        .clk_i, .rst_ni, .adaptive_i(1'b0), .flow_ready_o(pre_flow_ready),
        .s_ar_valid_i(pre_parent_valid), .s_ar_ready_o(pre_parent_ready), .s_ar_i(pre_parent_ar),
        .m_ar_valid_o(pre_ar_valid), .m_ar_ready_i(tr_ar_ready), .m_ar_o(pre_ar),
        .m_r_valid_i(tr_r_valid), .m_r_ready_o(pre_r_ready), .m_r_i(pre_raw_r),
        .s_r_valid_o(pre_r_valid), .s_r_ready_i(mux_mst_req.r_ready), .s_r_o(pre_r),
        .job_fire_i({i_dma1.be_valid_d && i_dma1.be_ready_d, i_dma0.be_valid_d && i_dma0.be_ready_d}),
        .job_done_i({i_dma1.idma_rsp_valid && i_dma1.idma_rsp_ready, i_dma0.idma_rsp_valid && i_dma0.idma_rsp_ready}),
        .job_normal_i({i_dma1.ttype == idma_pkg::DEFAULT, i_dma0.ttype == idma_pkg::DEFAULT}),
        .job_src_i({64'(i_dma1.burst_req_d.src_addr), 64'(i_dma0.burst_req_d.src_addr)}),
        .job_dst_i({64'(i_dma1.burst_req_d.dst_addr), 64'(i_dma0.burst_req_d.dst_addr)}),
        .job_length_i({64'(i_dma1.burst_req_d.length), 64'(i_dma0.burst_req_d.length)}),
        .v_ar_fire_i(1'b0), .v_ar_owner_i(1'b0), .v_ar_addr_i(64'd0), .v_ar_len_i(8'd0),
        .v_aw_offer_i(i_dma_iommu_tr_mux.gen_mux.w_fifo_push),
        .v_aw_owner_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.id[2]),
        .v_aw_addr_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.addr),
        .v_aw_len_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.len),
        .v_aw_size_i(i_dma_iommu_tr_mux.gen_mux.mst_aw_chan.size),
        .v_w_fire_i(tr_w_valid && tr_w_ready), .v_wlast_i(tr_w_last),
        .p_aw_fire_i(comp_aw_valid && comp_aw_ready), .p_aw_owner_i(comp_aw_id[2]),
        .p_w_fire_i(comp_w_valid && comp_w_ready), .p_wlast_i(comp_w_last),
        .parent_count_o(pre_parent_count), .child_count_o(pre_child_count),
        .merged_count_o(pre_merged), .parent_last_count_o(pre_lasts)
    );
    initial $display("PRETL-DECL ar_bits=%0d r_bits=%0d", $bits(axi_tr4_ar_chan_t), $bits(axi_tr4_r_chan_t));

    logic [1:0] reuse_parent_valid, reuse_parent_pid_valid, reuse_parent_priv;
    logic [1:0][63:0] reuse_parent_addr;
    logic [1:0][23:0] reuse_parent_did;
    logic [1:0][19:0] reuse_parent_pid;
    logic [1:0][31:0] reuse_parent_seq;
    for (genvar f=0; f<2; f++) begin : gen_reuse_parent
        assign reuse_parent_valid[f] = i_pre.active_q[f] && i_pre.pa_q[f] > i_pre.done_q[f] &&
            i_pre.parent_q[f][0].len == 255 && i_pre.parent_q[f][0].size == 3 &&
            i_pre.parent_q[f][0].burst == 1 && !i_pre.parent_q[f][0].lock &&
            i_pre.parent_q[f][0].addr[11:0] <= 2048;
        assign reuse_parent_addr[f] = i_pre.parent_q[f][0].addr;
        assign reuse_parent_did[f] = i_pre.parent_q[f][0].stream_id;
        assign reuse_parent_pid[f] = i_pre.parent_q[f][0].substream_id;
        assign reuse_parent_pid_valid[f] = i_pre.parent_q[f][0].ss_id_valid;
        assign reuse_parent_priv[f] = i_pre.parent_q[f][0].prot[0];
        assign reuse_parent_seq[f] = i_pre.done_q[f];
    end

    // The unchanged joint harness (real banked CTL <-> real TL)
    // ========================================================================
    nonblock_banked_top #(
        .NonblockEnable      ( NonblockEnable      ),
        .BankedEnable        ( 1'b1                ),
        .NumOutstandingTrans ( NumOutstandingTrans ),
        .WFifoDepth          ( WFifoDepth          ),
        .PolicyEnable        ( PolicyEnable        ),
        .CompletionGateEnable( SharedPoolMode >= 4 ),
        .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
        .HeaderDirectEnable(HeaderDirectEnable),
        .BankIdBit(SameBankDmasEnable ? 3 : 2)
    ) i_sys (
        .reuse_enable_i(reuse_enable_i),
        .reuse_parent_valid_i(reuse_parent_valid), .reuse_parent_addr_i(reuse_parent_addr),
        .reuse_parent_did_i(reuse_parent_did), .reuse_parent_pid_i(reuse_parent_pid),
        .reuse_parent_pid_valid_i(reuse_parent_pid_valid), .reuse_parent_priv_i(reuse_parent_priv),
        .reuse_parent_seq_i(reuse_parent_seq),
        .clk_i                 ( clk_i               ),
        .rst_ni                ( rst_ni              ),
        .policy_partition_i    ( policy_partition_i  ),

        .tr_aw_valid           ( tr_aw_valid         ),
        .tr_aw_id              ( tr_aw_id            ),
        .tr_aw_addr            ( tr_aw_addr          ),
        .tr_aw_len             ( tr_aw_len           ),
        .tr_aw_size            ( tr_aw_size          ),
        .tr_aw_burst           ( tr_aw_burst         ),
        .tr_aw_prot            ( tr_aw_prot          ),
        .tr_aw_user            ( tr_aw_user          ),
        .tr_aw_did             ( tr_aw_did           ),
        .tr_aw_ssid_valid      ( tr_aw_ssid_valid    ),
        .tr_aw_subid           ( tr_aw_subid         ),
        .tr_aw_mmu_atst        ( tr_aw_mmu_atst      ),
        .tr_aw_mmu_valid       ( tr_aw_mmu_valid     ),

        .tr_ar_valid           ( tr_ar_valid         ),
        .tr_ar_id              ( tr_ar_id            ),
        .tr_ar_addr            ( tr_ar_addr          ),
        .tr_ar_len             ( tr_ar_len           ),
        .tr_ar_size            ( tr_ar_size          ),
        .tr_ar_burst           ( tr_ar_burst         ),
        .tr_ar_prot            ( tr_ar_prot          ),
        .tr_ar_user            ( tr_ar_user          ),
        .tr_ar_did             ( tr_ar_did           ),
        .tr_ar_ssid_valid      ( tr_ar_ssid_valid    ),
        .tr_ar_subid           ( tr_ar_subid         ),
        .tr_ar_mmu_atst        ( tr_ar_mmu_atst      ),
        .tr_ar_mmu_valid       ( tr_ar_mmu_valid     ),

        .tr_w_valid            ( tr_w_valid          ),
        .tr_w_data             ( tr_w_data           ),
        .tr_w_strb             ( tr_w_strb           ),
        .tr_w_last             ( tr_w_last           ),
        .tr_w_user             ( tr_w_user           ),

        .tr_b_ready            ( tr_b_ready          ),
        .tr_r_ready            ( tr_r_ready          ),

        .tr_aw_ready           ( tr_aw_ready         ),
        .tr_ar_ready           ( tr_ar_ready         ),
        .tr_w_ready            ( tr_w_ready          ),
        .tr_b_valid            ( tr_b_valid          ),
        .tr_b_id               ( tr_b_id             ),
        .tr_b_resp             ( tr_b_resp           ),
        .tr_r_valid            ( tr_r_valid          ),
        .tr_r_id               ( tr_r_id             ),
        .tr_r_data             ( tr_r_data           ),
        .tr_r_resp             ( tr_r_resp           ),
        .tr_r_last             ( tr_r_last           ),

        .comp_aw_valid         ( comp_aw_valid       ),
        .comp_aw_id            ( comp_aw_id          ),
        .comp_aw_addr          ( comp_aw_addr        ),
        .comp_aw_len           ( comp_aw_len         ),
        .comp_aw_size          ( comp_aw_size        ),
        .comp_aw_burst         ( comp_aw_burst       ),
        .comp_aw_prot          ( comp_aw_prot        ),
        .comp_aw_user          ( comp_aw_user        ),
        .comp_ar_valid         ( comp_ar_valid       ),
        .comp_ar_id            ( comp_ar_id          ),
        .comp_ar_addr          ( comp_ar_addr        ),
        .comp_ar_len           ( comp_ar_len         ),
        .comp_ar_size          ( comp_ar_size        ),
        .comp_ar_burst         ( comp_ar_burst       ),
        .comp_ar_prot          ( comp_ar_prot        ),
        .comp_ar_user          ( comp_ar_user        ),
        .comp_w_valid          ( comp_w_valid        ),
        .comp_w_data           ( comp_w_data         ),
        .comp_w_strb           ( comp_w_strb         ),
        .comp_w_last           ( comp_w_last         ),
        .comp_b_ready          ( comp_b_ready        ),
        .comp_r_ready          ( comp_r_ready        ),
        .comp_aw_ready         ( comp_aw_ready       ),
        .comp_ar_ready         ( comp_ar_ready       ),
        .comp_w_ready          ( comp_w_ready        ),
        .comp_b_valid          ( comp_b_valid        ),
        .comp_b_id             ( comp_b_id           ),
        .comp_b_resp           ( comp_b_resp         ),
        .comp_r_valid          ( comp_r_valid        ),
        .comp_r_id             ( comp_r_id           ),
        .comp_r_data           ( comp_r_data         ),
        .comp_r_resp           ( comp_r_resp         ),
        .comp_r_last           ( comp_r_last         ),

        .ddtw_ar_valid         ( ddtw_ar_valid       ),
        .ddtw_ar_addr          ( ddtw_ar_addr        ),
        .ddtw_ar_id            ( ddtw_ar_id          ),
        .ddtw_ar_len           ( ddtw_ar_len         ),
        .ddtw_ar_size          ( ddtw_ar_size        ),
        .ddtw_ar_burst         ( ddtw_ar_burst       ),
        .ddtw_ar_ready         ( ddtw_ar_ready       ),
        .ddtw_r_ready          ( ddtw_r_ready        ),
        .ddtw_r_valid          ( ddtw_r_valid        ),
        .ddtw_r_data           ( ddtw_r_data         ),
        .ddtw_r_id             ( ddtw_r_id           ),
        .ddtw_r_resp           ( ddtw_r_resp         ),
        .ddtw_r_last           ( ddtw_r_last         ),

        .ptw_ar_valid          ( ptw_ar_valid        ),
        .ptw_ar_addr           ( ptw_ar_addr         ),
        .ptw_ar_id             ( ptw_ar_id           ),
        .ptw_ar_len            ( ptw_ar_len          ),
        .ptw_ar_size           ( ptw_ar_size         ),
        .ptw_ar_burst          ( ptw_ar_burst        ),
        .ptw_ar_ready          ( ptw_ar_ready        ),
        .ptw_r_ready           ( ptw_r_ready         ),
        .ptw_r_valid           ( ptw_r_valid         ),
        .ptw_r_data            ( ptw_r_data          ),
        .ptw_r_id              ( ptw_r_id            ),
        .ptw_r_resp            ( ptw_r_resp          ),
        .ptw_r_last            ( ptw_r_last          ),

        .nonblock_enable_i     ( nonblock_enable_i   ),
        .nonblock_fire_o       ( dbg_nonblock_fire   ),
        .nonblock_busy_o       ( dbg_nonblock_busy   ),
        .in_flight_o           ( dbg_in_flight       ),

        .fullbuffer_jobs_i     ( harness_capacity_permission ),
        .late_comp_ar_admit_i  (late_comp_ar_admit),
        .late_comp_ar_valid_o  (late_comp_ar_valid), .late_comp_ar_fire_o(late_comp_ar_fire),
        .late_comp_ar_id_o     (late_comp_ar_id), .late_comp_ar_addr_o(late_comp_ar_addr),
        .late_comp_ar_len_o    (late_comp_ar_len), .late_comp_ar_size_o(late_comp_ar_size),

        .dbg_tl_req_valid      ( dbg_tl_req_valid    ),
        .dbg_tl_req_ready      ( dbg_tl_req_ready    ),
        .dbg_tl_req_bank       ( dbg_tl_req_bank     ),
        .dbg_tl_resp_valid     ( dbg_tl_resp_valid   ),
        .dbg_tl_resp_ready     ( dbg_tl_resp_ready   ),
        .dbg_tl_resp_bank      ( dbg_tl_resp_bank    ),
        .dbg_tl_ptw_active     ( dbg_tl_ptw_active   ),
        .dbg_tl_ptw_r_fire     ( dbg_tl_ptw_r_fire   ),
        .dbg_tl_iotlb_lookup   ( dbg_tl_iotlb_lookup ),
        .dbg_tl_iotlb_hit      ( dbg_tl_iotlb_hit    )
    );

    // ========================================================================
    // Test-only configuration AXI master FSM.
    //
    // Turns ONE external cfg request into ONE real AW/W/B or AR/R transaction
    // on the selected dma_core_wrap axi_slv port.  The external master must
    // hold cfg_valid/cfg_addr/cfg_wdata/cfg_wstrb/cfg_write/cfg_sel stable until
    // cfg_ready is observed.  Nothing is forced into the frontend registers.
    // ========================================================================
    typedef enum logic [2:0] {
        C_IDLE, C_AW, C_W, C_B, C_AR, C_R, C_HOLD
    } cfg_state_e;

    cfg_state_e  cfg_state_q;
    logic [63:0] cfg_addr_q, cfg_wdata_q, cfg_rdata_q;
    logic [7:0]  cfg_wstrb_q;
    logic        cfg_sel_q, cfg_write_q, cfg_err_q;
    logic        cfg_aw_done_q, cfg_w_done_q;
    // Selected-slave request body.  Declared at module scope: a block-item
    // declaration may not follow statements inside an always_comb begin-block.
    cfg_req_t    cfg_drive_req;

    always_ff @(posedge clk_i or negedge rst_ni) begin : cfg_fsm
        if (!rst_ni) begin
            cfg_state_q <= C_IDLE;
            cfg_addr_q  <= '0;
            cfg_wdata_q <= '0;
            cfg_wstrb_q <= '0;
            cfg_sel_q   <= 1'b0;
            cfg_write_q <= 1'b0;
            cfg_rdata_q <= '0;
            cfg_err_q   <= 1'b0;
            cfg_aw_done_q <= 1'b0;
            cfg_w_done_q  <= 1'b0;
        end else begin
            unique case (cfg_state_q)
                C_IDLE: begin
                    if (cfg_valid) begin
                        cfg_addr_q  <= cfg_addr;
                        cfg_wdata_q <= cfg_wdata;
                        cfg_wstrb_q <= cfg_wstrb;
                        cfg_sel_q   <= cfg_sel;
                        cfg_write_q <= cfg_write;
                        cfg_err_q   <= 1'b0;
                        cfg_aw_done_q <= 1'b0;
                        cfg_w_done_q  <= 1'b0;
                        cfg_state_q <= cfg_write ? C_AW : C_AR;
                    end
                end
                C_AW: begin
                    // Offer AW and W independently: the register converter
                    // may withhold AWREADY until WVALID is present.
                    if (cfg_drive_req.aw_valid && cfg_rsp[cfg_sel_q].aw_ready)
                        cfg_aw_done_q <= 1'b1;
                    if (cfg_drive_req.w_valid && cfg_rsp[cfg_sel_q].w_ready)
                        cfg_w_done_q <= 1'b1;
                    if ((cfg_aw_done_q || cfg_rsp[cfg_sel_q].aw_ready) &&
                        (cfg_w_done_q || cfg_rsp[cfg_sel_q].w_ready))
                        cfg_state_q <= C_B;
                end
                C_B: begin
                    if (cfg_rsp[cfg_sel_q].b_valid) begin
                        cfg_err_q   <= (cfg_rsp[cfg_sel_q].b.resp != axi_pkg::RESP_OKAY);
                        cfg_rdata_q <= '0;
                        cfg_state_q <= C_HOLD;
                    end
                end
                C_AR: begin
                    if (cfg_rsp[cfg_sel_q].ar_ready) cfg_state_q <= C_R;
                end
                C_R: begin
                    if (cfg_rsp[cfg_sel_q].r_valid) begin
                        cfg_rdata_q <= cfg_rsp[cfg_sel_q].r.data;
                        cfg_err_q   <= (cfg_rsp[cfg_sel_q].r.resp != axi_pkg::RESP_OKAY);
                        cfg_state_q <= C_HOLD;
                    end
                end
                C_HOLD: begin
                    // Hold ready until the external master drops cfg_valid so a
                    // held request can never be re-issued.
                    if (!cfg_valid) cfg_state_q <= C_IDLE;
                end
                default: cfg_state_q <= C_IDLE;
            endcase
        end
    end

    // Drive the selected slave port; the other port stays completely idle.
    always_comb begin : cfg_drive
        cfg_drive_req = '0;
        cfg_drive_req.aw.id    = '0;
        cfg_drive_req.aw.addr  = cfg_addr_q;
        cfg_drive_req.aw.len   = 8'd0;                 // single beat
        cfg_drive_req.aw.size  = 3'd3;                 // 8 bytes
        cfg_drive_req.aw.burst = axi_pkg::BURST_INCR;
        cfg_drive_req.aw.user  = '0;
        cfg_drive_req.w.data   = cfg_wdata_q;
        cfg_drive_req.w.strb   = cfg_wstrb_q;
        cfg_drive_req.w.last   = 1'b1;
        cfg_drive_req.w.user   = '0;
        cfg_drive_req.ar.id    = '0;
        cfg_drive_req.ar.addr  = cfg_addr_q;
        cfg_drive_req.ar.len   = 8'd0;
        cfg_drive_req.ar.size  = 3'd3;
        cfg_drive_req.ar.burst = axi_pkg::BURST_INCR;
        cfg_drive_req.ar.user  = '0;
        // Response-ready defaults are LOW.  A response may only be consumed in
        // the state that expects it, so B/R can never be accepted early/lost.
        cfg_drive_req.b_ready  = 1'b0;
        cfg_drive_req.r_ready  = 1'b0;

        unique case (cfg_state_q)
            // Hold each channel until its own handshake, without waiting for
            // the other channel or duplicating an already accepted request.
            C_AW: begin
                     cfg_drive_req.aw_valid = !cfg_aw_done_q;
                     cfg_drive_req.w_valid  = !cfg_w_done_q;
            end
            C_B:     cfg_drive_req.b_ready  = 1'b1;
            C_AR:    cfg_drive_req.ar_valid = 1'b1;
            C_R:     cfg_drive_req.r_ready  = 1'b1;
            default: ;
        endcase

        cfg_req[0] = '0;
        cfg_req[1] = '0;
        if (cfg_sel_q) cfg_req[1] = cfg_drive_req;
        else           cfg_req[0] = cfg_drive_req;
    end

    assign cfg_ready = (cfg_state_q == C_HOLD);
    assign cfg_rdata = cfg_rdata_q;
    assign cfg_error = cfg_err_q;

    // ========================================================================
    // Observation-only DMA taps
    // ========================================================================
    assign dbg_dma_issue        = { d1_issue,         d0_issue         };
    assign dbg_dma_done         = { d1_done,          d0_done          };
    assign dbg_dma_poll         = { d1_poll,          d0_poll          };
    assign dbg_dma_batch_active = { d1_batch_active,  d0_batch_active  };
    assign dbg_dma_batch_gap    = { d1_batch_gap,     d0_batch_gap     };
    assign dbg_dma_job_quiet    = { d1_job_quiet,     d0_job_quiet     };
    assign dbg_dma_fullbuffer   = { d1_fullbuffer,    d0_fullbuffer    };
    assign dbg_dma_launch_active= { d1_launch_active, d0_launch_active };
    assign dbg_dma_launch_held  = { d1_launch_held,   d0_launch_held   };
    assign dbg_dma_launch_error = { d1_launch_err,    d0_launch_err    };

    // ========================================================================
    // Read-only fast/main + 2-bank lifecycle taps.
    // ========================================================================
    assign dbg_fast_resp_valid = i_sys.i_tl.fast_resp_valid;
    assign dbg_fast_resp_ready = i_sys.i_tl.fast_resp_ready;
    assign dbg_fast_resp_bank  = i_sys.i_tl.fast_resp_data.bank;
    assign dbg_slow_resp_valid = i_sys.i_tl.slow_resp_valid;
    assign dbg_e2_tc_read_wait = {30'b0,
        i_sys.gen_banked.i_banked.bank_comp_req[1].ar_valid && !i_sys.gen_banked.i_banked.bank_comp_resp[1].ar_ready,
        i_sys.gen_banked.i_banked.bank_comp_req[0].ar_valid && !i_sys.gen_banked.i_banked.bank_comp_resp[0].ar_ready};
    assign dbg_e2_tc_aw_pending = {30'b0, i_sys.gen_banked.i_banked.gen_bank[1].i_bank.aw_valid,
        i_sys.gen_banked.i_banked.gen_bank[0].i_bank.aw_valid};
    assign dbg_e2_tc_state = {26'b0, i_sys.gen_banked.i_banked.gen_bank[1].i_bank.state_q,
                             i_sys.gen_banked.i_banked.gen_bank[0].i_bank.state_q};
    assign dbg_e2_tc_ar_head = {24'b0, i_sys.gen_banked.i_banked.bank_comp_req[1].ar.id,
                              i_sys.gen_banked.i_banked.bank_comp_req[0].ar.id};
    for (genvar b = 0; b < 2; b++) begin : gen_spill_observation
        assign dbg_e2_tc_spill[b*2] = i_sys.gen_banked.i_banked.gen_bank[b].i_bank.
            gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif.i_ar_spill_reg.
            spill_register_flushable_i.gen_spill_reg.a_full_q;
        assign dbg_e2_tc_spill[b*2+1] = i_sys.gen_banked.i_banked.gen_bank[b].i_bank.
            gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif.i_ar_spill_reg.
            spill_register_flushable_i.gen_spill_reg.b_full_q;
        assign dbg_e2_tc_queue_ids[b*12+:4] = i_sys.gen_banked.i_banked.gen_bank[b].i_bank.
            gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif.i_ar_spill_reg.
            spill_register_flushable_i.gen_spill_reg.b_data_q.id;
        assign dbg_e2_tc_queue_ids[b*12+4+:4] = i_sys.gen_banked.i_banked.gen_bank[b].i_bank.
            gen_mrif_support_disabled.i_iommu_axi_demux_no_mrif.i_ar_spill_reg.
            spill_register_flushable_i.gen_spill_reg.a_data_q.id;
        assign dbg_e2_tc_queue_ids[b*12+8+:4] = i_sys.gen_banked.i_banked.gen_bank[b].i_bank.ar_data.id;
    end
    assign dbg_e2_tc_spill[31:4] = '0;
    assign dbg_e2_tc_queue_ids[31:24] = '0;
    assign dbg_bank_busy       = i_sys.gen_banked.i_banked.bank_busy;
    assign dbg_cfg_state       = cfg_state_q;

endmodule
