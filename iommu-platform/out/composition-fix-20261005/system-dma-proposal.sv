// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_actual_dma_system_top.sv
//
// BOUNDED DIAGNOSTIC, TEST-ONLY SYSTEM-INTEGRATION WRAPPER -- NOT PRODUCTION RTL.
//
// This wrapper re-instantiates the UNCHANGED tests/nonblock_actual_dma_top.sv
// (real DMA0/DMA1 + real axi_mux + banked CTL <-> TL harness) and wraps it in a
// TEST-ONLY approximation of the board fabric built from PRODUCTION modules:
//
//   * real rv_iommu_ds_if   (DDTW + PTW live; PDTW/MSIPTW/MRIF/CQ/FQ/PQ/MSIIG
//     tied to zero).  The data-structure merge is the REAL production module --
//     it is NOT approximated with an axi_mux and ID rewriting.
//   * real axi_delayer with the board constants FixedDelayInput=50 /
//     FixedDelayOutput=50, between the DS interface and the crossbar.
//   * real axi_xbar with the board STRUCTURAL parameters:
//       MaxMstTrans=8, MaxSlvTrans=8, LatencyMode=CUT_ALL_PORTS,
//       PipelineStages=1, UniqueIds=0, FallThrough=0,
//       AxiIdWidthSlvPorts=4 -> master ID width 6.
//
// Ports of the crossbar (deliberately reduced vs the 11-port board fabric):
//     slv 0 : synthetic read-only CPU master   (TEST-ONLY, see below)
//     slv 1 : debug/idle master                (tied off)
//     slv 2 : IOMMU data structures -> delayer (rv_iommu_ds_if output)
//     slv 3 : IOMMU completion master          (from the inner harness)
//     mst 0 : shared DDR      -- rule covers 0x0 .. 0x1_0000_0000 (4 GiB)
//     mst 1 : dummy peripheral-- rule covers 0x1_0000_0000 .. 0x2_0000_0000,
//                               backed by axi_err_slv (never addressed).
// Two master ports are kept on purpose: axi_xbar_unmuxed has a degenerate
// single-master path that we do not want to exercise.
//
// EVERY test address used by tests/nonblock_actual_dma_test.cpp lives below
// 4 GiB (DDT 0x10000, walker tables 0x20000..0x70000, 0x80000000/0x90000000),
// so all real traffic is forced through mst0.
//
// The synthetic CPU master is NOT a real RISC-V core and NOT a cache model.
// It only injects one bounded read stream to reproduce CPU<->DMA<->walker
// contention through the real crossbar.  It starts when DMA0's batch becomes
// active and stops offering NEW reads when the batch (active|gap) ends, then
// drains any outstanding beat.  Its AR is never withdrawn while stalled and its
// RREADY is a fixed 16-cycle-ready / 4-cycle-stall pattern.
//
// DELIBERATE LIMITATIONS (must be reported with any result):
//   * no real CPU / caches / axi_adapter / dwidth converters;
//   * only 2 of the 11 board master ports; the peripheral fabric, PLIC/CLINT/
//     ROM/UART/SPI/GPIO/Ethernet/timer are NOT instantiated;
//   * no MIG / DDR controller / clock converter / 512-to-64 width converter:
//     DDR is a behavioural model in the C++ testbench;
//   * the cfg_* AXI slave selector still reaches the two dma_core_wrap slaves
//     DIRECTLY (it bypasses the CPU crossbar and the production dma_cfg
//     axi_demux).  This is unchanged behaviour, now an explicit limitation.
//
// Observation taps only: nothing here forces a register or bypasses a backend.

`include "axi/typedef.svh"
`include "axi/assign.svh"

module nonblock_actual_dma_system_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter bit          NonblockEnable       = 1,
    parameter int unsigned NumOutstandingTrans = 8,
    parameter int unsigned WFifoDepth          = 32,
    parameter bit          PolicyEnable        = 0,
    // Test-only: cycles between the synthetic CPU read offers.
    parameter int unsigned CpuIssueInterval    = 32,
    // Test-only: DS path delay (board constants).
    parameter int unsigned DsFixedDelayInput   = 50,
    parameter int unsigned DsFixedDelayOutput  = 50
) (
    input  logic        clk_i,
    input  logic        rst_ni,

    input  logic        nonblock_enable_i,
    input  logic        policy_partition_i,

    // ---------------- shared DDR master port (6-bit master-side ID) ---------
    output logic        ddr_aw_valid,
    output logic [5:0]  ddr_aw_id,
    output logic [63:0] ddr_aw_addr,
    output logic [7:0]  ddr_aw_len,
    output logic [2:0]  ddr_aw_size,
    output logic [1:0]  ddr_aw_burst,
    output logic [2:0]  ddr_aw_prot,
    output logic [0:0]  ddr_aw_user,
    output logic        ddr_w_valid,
    output logic [63:0] ddr_w_data,
    output logic [7:0]  ddr_w_strb,
    output logic        ddr_w_last,
    output logic [0:0]  ddr_w_user,
    output logic        ddr_b_ready,
    output logic        ddr_ar_valid,
    output logic [5:0]  ddr_ar_id,
    output logic [63:0] ddr_ar_addr,
    output logic [7:0]  ddr_ar_len,
    output logic [2:0]  ddr_ar_size,
    output logic [1:0]  ddr_ar_burst,
    output logic [2:0]  ddr_ar_prot,
    output logic [0:0]  ddr_ar_user,
    output logic        ddr_r_ready,
    input  logic        ddr_aw_ready,
    input  logic        ddr_w_ready,
    input  logic        ddr_b_valid,
    input  logic [5:0]  ddr_b_id,
    input  logic [1:0]  ddr_b_resp,
    input  logic        ddr_ar_ready,
    input  logic        ddr_r_valid,
    input  logic [5:0]  ddr_r_id,
    input  logic [63:0] ddr_r_data,
    input  logic [1:0]  ddr_r_resp,
    input  logic        ddr_r_last,

    // ---------------- test-only configuration AXI slave selector ------------
    input  logic        cfg_sel,
    input  logic        cfg_valid,
    input  logic        cfg_write,
    input  logic [63:0] cfg_addr,
    input  logic [63:0] cfg_wdata,
    input  logic [7:0]  cfg_wstrb,
    output logic        cfg_ready,
    output logic [63:0] cfg_rdata,
    output logic        cfg_error,

    // ---------------- re-exported harness observation taps ------------------
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
    output logic        dbg_nonblock_fire,
    output logic        dbg_nonblock_busy,
    output logic        dbg_in_flight,
    output logic        dbg_fast_resp_valid,
    output logic        dbg_fast_resp_ready,
    output logic        dbg_fast_resp_bank,
    output logic        dbg_slow_resp_valid,
    output logic [1:0]  dbg_bank_busy,
    output logic [2:0]  dbg_cfg_state,
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

    // ---------------- test-only system-fabric observation taps --------------
    output logic        dbg_comp_ar_fire,
    output logic        dbg_comp_aw_fire,
    output logic        dbg_comp_w_fire,
    output logic        dbg_ddtw_ar_fire,
    output logic        dbg_ptw_ar_fire,
    output logic        dbg_ds_ar_valid,
    output logic        dbg_ds_ar_ready,
    output logic        dbg_ds_r_valid,
    output logic        dbg_ds_r_ready,
    output logic        dbg_ddr_ar_valid,
    output logic        dbg_ddr_ar_ready,
    output logic        dbg_ddr_r_valid,
    output logic        dbg_ddr_r_ready,
    output logic        dbg_ddr_aw_valid,
    output logic        dbg_ddr_aw_ready,
    output logic        dbg_ddr_b_valid,
    output logic        dbg_ddr_b_ready,

    // ---------------- synthetic CPU lifecycle (read-only) -------------------
    output logic [31:0] dbg_cpu_issued,
    output logic [31:0] dbg_cpu_completed,
    output logic        dbg_cpu_quiet,
    output logic        dbg_cpu_active
);

    // ========================================================================
    // Fixed widths / crossbar shape
    // ========================================================================
    localparam int unsigned SysNumSlv = 4;
    localparam int unsigned SysNumMst = 2;
    localparam int unsigned SlvIdW    = 4;
    localparam int unsigned MstIdW    = SlvIdW + $clog2(SysNumSlv); // 6

    typedef logic [63:0]       addr_t;
    typedef logic [SlvIdW-1:0] slv_id_t;
    typedef logic [MstIdW-1:0] mst_id_t;
    typedef logic [63:0]       data_t;
    typedef logic [7:0]        strb_t;
    typedef logic [0:0]        user_t;

    `AXI_TYPEDEF_AW_CHAN_T(slv_aw_chan_t, addr_t, slv_id_t, user_t)
    `AXI_TYPEDEF_W_CHAN_T (w_chan_t,       data_t, strb_t,   user_t)
    `AXI_TYPEDEF_B_CHAN_T (slv_b_chan_t,   slv_id_t,         user_t)
    `AXI_TYPEDEF_AR_CHAN_T(slv_ar_chan_t,  addr_t, slv_id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T (slv_r_chan_t,   data_t, slv_id_t, user_t)
    `AXI_TYPEDEF_REQ_T    (slv_req_t,  slv_aw_chan_t, w_chan_t, slv_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (slv_resp_t, slv_b_chan_t,  slv_r_chan_t)

    `AXI_TYPEDEF_AW_CHAN_T(mst_aw_chan_t, addr_t, mst_id_t, user_t)
    `AXI_TYPEDEF_B_CHAN_T (mst_b_chan_t,  mst_id_t,         user_t)
    `AXI_TYPEDEF_AR_CHAN_T(mst_ar_chan_t, addr_t, mst_id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T (mst_r_chan_t,  data_t, mst_id_t, user_t)
    `AXI_TYPEDEF_REQ_T    (mst_req_t,  mst_aw_chan_t, w_chan_t, mst_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (mst_resp_t, mst_b_chan_t,  mst_r_chan_t)

    slv_req_t  [SysNumSlv-1:0] slv_reqs;
    slv_resp_t [SysNumSlv-1:0] slv_resps;
    mst_req_t  [SysNumMst-1:0] mst_reqs;
    mst_resp_t [SysNumMst-1:0] mst_resps;

    // ========================================================================
    // Inner, UNCHANGED harness scalar ports -> internal nets
    // ========================================================================
    logic        u_comp_aw_valid;
    logic [3:0]  u_comp_aw_id;
    logic [63:0] u_comp_aw_addr;
    logic [7:0]  u_comp_aw_len;
    logic [2:0]  u_comp_aw_size;
    logic [1:0]  u_comp_aw_burst;
    logic [2:0]  u_comp_aw_prot;
    logic [0:0]  u_comp_aw_user;

    logic        u_comp_ar_valid;
    logic [3:0]  u_comp_ar_id;
    logic [63:0] u_comp_ar_addr;
    logic [7:0]  u_comp_ar_len;
    logic [2:0]  u_comp_ar_size;
    logic [1:0]  u_comp_ar_burst;
    logic [2:0]  u_comp_ar_prot;
    logic [0:0]  u_comp_ar_user;

    logic        u_comp_w_valid;
    logic [63:0] u_comp_w_data;
    logic [7:0]  u_comp_w_strb;
    logic        u_comp_w_last;

    logic        u_comp_b_ready;
    logic        u_comp_r_ready;

    logic        u_ddtw_ar_valid;
    logic [63:0] u_ddtw_ar_addr;
    logic [3:0]  u_ddtw_ar_id;
    logic [7:0]  u_ddtw_ar_len;
    logic [2:0]  u_ddtw_ar_size;
    logic [1:0]  u_ddtw_ar_burst;
    logic        u_ddtw_r_ready;

    logic        u_ptw_ar_valid;
    logic [63:0] u_ptw_ar_addr;
    logic [3:0]  u_ptw_ar_id;
    logic [7:0]  u_ptw_ar_len;
    logic [2:0]  u_ptw_ar_size;
    logic [1:0]  u_ptw_ar_burst;
    logic        u_ptw_r_ready;

    // ========================================================================
    // DS merge (REAL production rv_iommu_ds_if) + delayer + crossbar plumbing
    // ========================================================================
    slv_req_t  ddtw_req,  ptw_req,  pdtw_req,  msiptw_req, mrif_req;
    slv_req_t  cq_req,    fq_req,   pq_req,    msi_ig_req;
    slv_resp_t ddtw_resp, ptw_resp, pdtw_resp, msiptw_resp, mrif_resp;
    slv_resp_t cq_resp,   fq_resp,  pq_resp,   msi_ig_resp;
    slv_req_t  ds_req,          ds_delayed_req;
    slv_resp_t ds_resp,         ds_delayed_resp;

    // ---- DDTW pack ----------------------------------------------------------
    always_comb begin : ddtw_pack
        ddtw_req = '0;
        ddtw_req.ar_valid = u_ddtw_ar_valid;
        ddtw_req.ar.id    = u_ddtw_ar_id;
        ddtw_req.ar.addr  = u_ddtw_ar_addr;
        ddtw_req.ar.len   = u_ddtw_ar_len;
        ddtw_req.ar.size  = u_ddtw_ar_size;
        ddtw_req.ar.burst = u_ddtw_ar_burst;
        ddtw_req.r_ready  = u_ddtw_r_ready;
    end

    // ---- PTW pack -----------------------------------------------------------
    always_comb begin : ptw_pack
        ptw_req = '0;
        ptw_req.ar_valid = u_ptw_ar_valid;
        ptw_req.ar.id    = u_ptw_ar_id;
        ptw_req.ar.addr  = u_ptw_ar_addr;
        ptw_req.ar.len   = u_ptw_ar_len;
        ptw_req.ar.size  = u_ptw_ar_size;
        ptw_req.ar.burst = u_ptw_ar_burst;
        ptw_req.r_ready  = u_ptw_r_ready;
    end

    assign pdtw_req   = '0;
    assign msiptw_req = '0;
    assign mrif_req   = '0;
    assign cq_req     = '0;
    assign fq_req     = '0;
    assign pq_req     = '0;
    assign msi_ig_req = '0;

    // ---- REAL production data-structure merge -------------------------------
    rv_iommu_ds_if #(
        .aw_chan_t  (slv_aw_chan_t),
        .w_chan_t   (w_chan_t),
        .b_chan_t   (slv_b_chan_t),
        .ar_chan_t  (slv_ar_chan_t),
        .r_chan_t   (slv_r_chan_t),
        .axi_req_t  (slv_req_t),
        .axi_resp_t (slv_resp_t)
    ) i_ds_if (
        .clk_i,
        .rst_ni,
        .ds_resp_i          (ds_resp),
        .ds_req_o           (ds_req),
        .ddtw_req_i         (ddtw_req),  .ddtw_resp_o (ddtw_resp),
        .pdtw_req_i         (pdtw_req),  .pdtw_resp_o (pdtw_resp),
        .ptw_req_i          (ptw_req),   .ptw_resp_o  (ptw_resp),
        .msiptw_req_i       (msiptw_req),.msiptw_resp_o(msiptw_resp),
        .mrif_handler_req_i (mrif_req),  .mrif_handler_resp_o(mrif_resp),
        .cq_req_i           (cq_req),    .cq_resp_o   (cq_resp),
        .fq_req_i           (fq_req),    .fq_resp_o   (fq_resp),
        .pq_req_i           (pq_req),    .pq_resp_o   (pq_resp),
        .msi_ig_req_i       (msi_ig_req),.msi_ig_resp_o(msi_ig_resp)
    );

    // ---- REAL production DS delayer (board constants 50/50) -----------------
    axi_delayer #(
        .aw_chan_t          (slv_aw_chan_t),
        .w_chan_t           (w_chan_t),
        .b_chan_t           (slv_b_chan_t),
        .ar_chan_t          (slv_ar_chan_t),
        .r_chan_t           (slv_r_chan_t),
        .req_t              (slv_req_t),
        .resp_t             (slv_resp_t),
        .StallRandomInput   (1'b0),
        .StallRandomOutput  (1'b0),
        .FixedDelayInput    (DsFixedDelayInput),
        .FixedDelayOutput   (DsFixedDelayOutput)
    ) i_iommu_ds_delayer (
        .clk_i,
        .rst_ni,
        .slv_req_i  (ds_req),
        .slv_resp_o (ds_resp),
        .mst_req_o  (ds_delayed_req),
        .mst_resp_i (ds_delayed_resp)
    );

    assign slv_reqs[2]     = ds_delayed_req;
    assign ds_delayed_resp = slv_resps[2];

    // ------------------------------------------------------------------------
    // Slave port 3 : completion master from the inner harness
    // ------------------------------------------------------------------------
    slv_req_t comp_req;
    always_comb begin : comp_pack
        comp_req = '0;
        comp_req.aw_valid = u_comp_aw_valid;
        comp_req.aw.id    = u_comp_aw_id;
        comp_req.aw.addr  = u_comp_aw_addr;
        comp_req.aw.len   = u_comp_aw_len;
        comp_req.aw.size  = u_comp_aw_size;
        comp_req.aw.burst = u_comp_aw_burst;
        comp_req.aw.prot  = u_comp_aw_prot;
        comp_req.aw.user  = u_comp_aw_user;
        comp_req.ar_valid = u_comp_ar_valid;
        comp_req.ar.id    = u_comp_ar_id;
        comp_req.ar.addr  = u_comp_ar_addr;
        comp_req.ar.len   = u_comp_ar_len;
        comp_req.ar.size  = u_comp_ar_size;
        comp_req.ar.burst = u_comp_ar_burst;
        comp_req.ar.prot  = u_comp_ar_prot;
        comp_req.ar.user  = u_comp_ar_user;
        comp_req.w_valid  = u_comp_w_valid;
        comp_req.w.data   = u_comp_w_data;
        comp_req.w.strb   = u_comp_w_strb;
        comp_req.w.last   = u_comp_w_last;
        comp_req.b_ready  = u_comp_b_ready;
        comp_req.r_ready  = u_comp_r_ready;
    end
    assign slv_reqs[3] = comp_req;

    // ------------------------------------------------------------------------
    // Slave port 1 : debug / idle master
    // ------------------------------------------------------------------------
    assign slv_reqs[1] = '0;

    // ========================================================================
    // Slave port 0 : synthetic read-only CPU master (TEST-ONLY)
    // ========================================================================
    logic        cpu_run;
    assign cpu_run = dbg_dma_batch_active[0] | dbg_dma_batch_gap[0];

    localparam logic [5:0] CPU_INTERVAL = 6'(CpuIssueInterval);

    logic        cpu_ar_valid_q;
    logic [3:0]  cpu_ar_id_q;
    logic [63:0] cpu_ar_addr_q;
    logic [7:0]  cpu_ar_len_q;
    logic [2:0]  cpu_ar_size_q;
    logic [1:0]  cpu_ar_burst_q;
    logic        cpu_outstanding_q;
    logic [31:0] cpu_issued_q;
    logic [31:0] cpu_completed_q;
    logic [5:0]  cpu_wait_q;
    logic [4:0]  cpu_rphase_q;

    slv_req_t    cpu_req;

    always_ff @(posedge clk_i or negedge rst_ni) begin : cpu_fsm
        if (!rst_ni) begin
            cpu_ar_valid_q    <= 1'b0;
            cpu_ar_id_q       <= '0;
            cpu_ar_addr_q     <= '0;
            cpu_ar_len_q      <= '0;
            cpu_ar_size_q     <= '0;
            cpu_ar_burst_q    <= '0;
            cpu_outstanding_q <= 1'b0;
            cpu_issued_q      <= '0;
            cpu_completed_q   <= '0;
            cpu_wait_q        <= CPU_INTERVAL;
            cpu_rphase_q      <= '0;
        end else begin
            // ---- AR offer: held stable until accepted, never withdrawn -----
            if (cpu_ar_valid_q) begin
                if (slv_resps[0].ar_ready) begin
                    cpu_ar_valid_q    <= 1'b0;
                    cpu_outstanding_q <= 1'b1;
                    cpu_issued_q      <= cpu_issued_q + 32'd1;
                end
            end else if (!cpu_outstanding_q && cpu_run && (cpu_wait_q == '0)) begin
                cpu_ar_valid_q <= 1'b1;
                cpu_ar_id_q    <= 4'd0;
                cpu_ar_addr_q  <= 64'h0000_0000_8000_0000; // V_SRC_PA
                cpu_ar_len_q   <= 8'd7;                    // 8 x 8 B = 64 B
                cpu_ar_size_q  <= 3'd3;
                cpu_ar_burst_q <= axi_pkg::BURST_INCR;
            end

            // ---- bounded inter-read interval -------------------------------
            if (cpu_ar_valid_q || cpu_outstanding_q) cpu_wait_q <= CPU_INTERVAL;
            else if (cpu_wait_q != '0)               cpu_wait_q <= cpu_wait_q - 6'd1;

            // ---- single outstanding read drain -----------------------------
            if (slv_resps[0].r_valid && cpu_req.r_ready && slv_resps[0].r.last) begin
                cpu_outstanding_q <= 1'b0;
                cpu_completed_q   <= cpu_completed_q + 32'd1;
            end

            // ---- RREADY shape: 16 cycles ready, 4 cycles stalled -----------
            if (cpu_rphase_q == 5'd19) cpu_rphase_q <= '0;
            else                       cpu_rphase_q <= cpu_rphase_q + 5'd1;
        end
    end

    always_comb begin : cpu_req_drive
        cpu_req = '0;
        cpu_req.ar_valid = cpu_ar_valid_q;
        cpu_req.ar.id    = cpu_ar_id_q;
        cpu_req.ar.addr  = cpu_ar_addr_q;
        cpu_req.ar.len   = cpu_ar_len_q;
        cpu_req.ar.size  = cpu_ar_size_q;
        cpu_req.ar.burst = cpu_ar_burst_q;
        cpu_req.ar.prot  = 3'b010;
        cpu_req.r_ready  = (cpu_rphase_q < 5'd16);
    end
    assign slv_reqs[0] = cpu_req;

    // ========================================================================
    // Crossbar address map (reduced; 2 of the 11 board master ports)
    // ========================================================================
    axi_pkg::xbar_rule_64_t [1:0] addr_map;
    always_comb begin : addr_map_setup
        addr_map[0] = '{idx: 32'd0,
                        start_addr: 64'h0000_0000_0000_0000,
                        end_addr:   64'h0000_0001_0000_0000}; // 0 .. 4 GiB -> DDR
        addr_map[1] = '{idx: 32'd1,
                        start_addr: 64'h0000_0001_0000_0000,
                        end_addr:   64'h0000_0002_0000_0000}; // dummy peripheral
    end

    localparam axi_pkg::xbar_cfg_t XbarCfg = '{
        NoSlvPorts:         32'd4,
        NoMstPorts:         32'd2,
        MaxMstTrans:        32'd8,
        MaxSlvTrans:        32'd8,
        FallThrough:        1'b0,
        LatencyMode:        axi_pkg::CUT_ALL_PORTS,
        PipelineStages:     32'd1,
        AxiIdWidthSlvPorts: 32'd4,
        AxiIdUsedSlvPorts:  32'd4,
        UniqueIds:          1'b0,
        AxiAddrWidth:       32'd64,
        AxiDataWidth:       32'd64,
        NoAddrRules:        32'd2
    };

    // ---- REAL production crossbar structure --------------------------------
    axi_xbar #(
        .Cfg           (XbarCfg),
        .ATOPs         (1'b1),
        .Connectivity  ('1),
        .slv_aw_chan_t (slv_aw_chan_t),
        .mst_aw_chan_t (mst_aw_chan_t),
        .w_chan_t      (w_chan_t),
        .slv_b_chan_t  (slv_b_chan_t),
        .mst_b_chan_t  (mst_b_chan_t),
        .slv_ar_chan_t (slv_ar_chan_t),
        .mst_ar_chan_t (mst_ar_chan_t),
        .slv_r_chan_t  (slv_r_chan_t),
        .mst_r_chan_t  (mst_r_chan_t),
        .slv_req_t     (slv_req_t),
        .slv_resp_t    (slv_resp_t),
        .mst_req_t     (mst_req_t),
        .mst_resp_t    (mst_resp_t),
        .rule_t        (axi_pkg::xbar_rule_64_t)
    ) i_xbar (
        .clk_i,
        .rst_ni,
        .test_i                 (1'b0),
        .slv_ports_req_i        (slv_reqs),
        .slv_ports_resp_o       (slv_resps),
        .mst_ports_req_o        (mst_reqs),
        .mst_ports_resp_i       (mst_resps),
        .addr_map_i             (addr_map),
        .en_default_mst_port_i  ('0),
        .default_mst_port_i     ('0)
    );

    // ========================================================================
    // Master port 0 : shared DDR
    // ========================================================================
    assign ddr_aw_valid = mst_reqs[0].aw_valid;
    assign ddr_aw_id    = mst_reqs[0].aw.id;
    assign ddr_aw_addr  = mst_reqs[0].aw.addr;
    assign ddr_aw_len   = mst_reqs[0].aw.len;
    assign ddr_aw_size  = mst_reqs[0].aw.size;
    assign ddr_aw_burst = mst_reqs[0].aw.burst;
    assign ddr_aw_prot  = mst_reqs[0].aw.prot;
    assign ddr_aw_user  = mst_reqs[0].aw.user;
    assign ddr_w_valid  = mst_reqs[0].w_valid;
    assign ddr_w_data   = mst_reqs[0].w.data;
    assign ddr_w_strb   = mst_reqs[0].w.strb;
    assign ddr_w_last   = mst_reqs[0].w.last;
    assign ddr_w_user   = mst_reqs[0].w.user;
    assign ddr_b_ready  = mst_reqs[0].b_ready;
    assign ddr_ar_valid = mst_reqs[0].ar_valid;
    assign ddr_ar_id    = mst_reqs[0].ar.id;
    assign ddr_ar_addr  = mst_reqs[0].ar.addr;
    assign ddr_ar_len   = mst_reqs[0].ar.len;
    assign ddr_ar_size  = mst_reqs[0].ar.size;
    assign ddr_ar_burst = mst_reqs[0].ar.burst;
    assign ddr_ar_prot  = mst_reqs[0].ar.prot;
    assign ddr_ar_user  = mst_reqs[0].ar.user;
    assign ddr_r_ready  = mst_reqs[0].r_ready;

    always_comb begin : ddr_resp_drive
        mst_resps[0] = '0;
        mst_resps[0].aw_ready = ddr_aw_ready;
        mst_resps[0].w_ready  = ddr_w_ready;
        mst_resps[0].b_valid  = ddr_b_valid;
        mst_resps[0].b.id     = ddr_b_id;
        mst_resps[0].b.resp   = ddr_b_resp;
        mst_resps[0].ar_ready = ddr_ar_ready;
        mst_resps[0].r_valid  = ddr_r_valid;
        mst_resps[0].r.id     = ddr_r_id;
        mst_resps[0].r.data   = ddr_r_data;
        mst_resps[0].r.resp   = ddr_r_resp;
        mst_resps[0].r.last   = ddr_r_last;
    end

    // ========================================================================
    // Master port 1 : dummy peripheral, never addressed by this test
    // ========================================================================
    axi_err_slv #(
        .AxiIdWidth (32'd6),
        .axi_req_t  (mst_req_t),
        .axi_resp_t (mst_resp_t),
        .Resp       (axi_pkg::RESP_DECERR),
        .ATOPs      (1'b1),
        .MaxTrans   (32'd4)
    ) i_dummy_err_slv (
        .clk_i,
        .rst_ni,
        .test_i     (1'b0),
        .slv_req_i  (mst_reqs[1]),
        .slv_resp_o (mst_resps[1])
    );

    // ========================================================================
    // The inner UNCHANGED harness
    // ========================================================================
    nonblock_actual_dma_top #(
        .NonblockEnable      (NonblockEnable),
        .NumOutstandingTrans (NumOutstandingTrans),
        .WFifoDepth          (WFifoDepth),
        .PolicyEnable        (PolicyEnable)
    ) i_dma (
        .clk_i,
        .rst_ni,
        .nonblock_enable_i  (nonblock_enable_i),
        .policy_partition_i (policy_partition_i),

        .comp_aw_valid (u_comp_aw_valid),
        .comp_aw_id    (u_comp_aw_id),
        .comp_aw_addr  (u_comp_aw_addr),
        .comp_aw_len   (u_comp_aw_len),
        .comp_aw_size  (u_comp_aw_size),
        .comp_aw_burst (u_comp_aw_burst),
        .comp_aw_prot  (u_comp_aw_prot),
        .comp_aw_user  (u_comp_aw_user),
        .comp_ar_valid (u_comp_ar_valid),
        .comp_ar_id    (u_comp_ar_id),
        .comp_ar_addr  (u_comp_ar_addr),
        .comp_ar_len   (u_comp_ar_len),
        .comp_ar_size  (u_comp_ar_size),
        .comp_ar_burst (u_comp_ar_burst),
        .comp_ar_prot  (u_comp_ar_prot),
        .comp_ar_user  (u_comp_ar_user),
        .comp_w_valid  (u_comp_w_valid),
        .comp_w_data   (u_comp_w_data),
        .comp_w_strb   (u_comp_w_strb),
        .comp_w_last   (u_comp_w_last),
        .comp_b_ready  (u_comp_b_ready),
        .comp_r_ready  (u_comp_r_ready),
        .comp_aw_ready (slv_resps[3].aw_ready),
        .comp_ar_ready (slv_resps[3].ar_ready),
        .comp_w_ready  (slv_resps[3].w_ready),
        .comp_b_valid  (slv_resps[3].b_valid),
        .comp_b_id     (slv_resps[3].b.id),
        .comp_b_resp   (slv_resps[3].b.resp),
        .comp_r_valid  (slv_resps[3].r_valid),
        .comp_r_id     (slv_resps[3].r.id),
        .comp_r_data   (slv_resps[3].r.data),
        .comp_r_resp   (slv_resps[3].r.resp),
        .comp_r_last   (slv_resps[3].r.last),

        .ddtw_ar_valid (u_ddtw_ar_valid),
        .ddtw_ar_addr  (u_ddtw_ar_addr),
        .ddtw_ar_id    (u_ddtw_ar_id),
        .ddtw_ar_len   (u_ddtw_ar_len),
        .ddtw_ar_size  (u_ddtw_ar_size),
        .ddtw_ar_burst (u_ddtw_ar_burst),
        .ddtw_ar_ready (ddtw_resp.ar_ready),
        .ddtw_r_ready  (u_ddtw_r_ready),
        .ddtw_r_valid  (ddtw_resp.r_valid),
        .ddtw_r_data   (ddtw_resp.r.data),
        .ddtw_r_id     (ddtw_resp.r.id),
        .ddtw_r_resp   (ddtw_resp.r.resp),
        .ddtw_r_last   (ddtw_resp.r.last),

        .ptw_ar_valid  (u_ptw_ar_valid),
        .ptw_ar_addr   (u_ptw_ar_addr),
        .ptw_ar_id     (u_ptw_ar_id),
        .ptw_ar_len    (u_ptw_ar_len),
        .ptw_ar_size   (u_ptw_ar_size),
        .ptw_ar_burst  (u_ptw_ar_burst),
        .ptw_ar_ready  (ptw_resp.ar_ready),
        .ptw_r_ready   (u_ptw_r_ready),
        .ptw_r_valid   (ptw_resp.r_valid),
        .ptw_r_data    (ptw_resp.r.data),
        .ptw_r_id      (ptw_resp.r.id),
        .ptw_r_resp    (ptw_resp.r.resp),
        .ptw_r_last    (ptw_resp.r.last),

        .dbg_tl_req_valid    (dbg_tl_req_valid),
        .dbg_tl_req_ready    (dbg_tl_req_ready),
        .dbg_tl_req_bank     (dbg_tl_req_bank),
        .dbg_tl_resp_valid   (dbg_tl_resp_valid),
        .dbg_tl_resp_ready   (dbg_tl_resp_ready),
        .dbg_tl_resp_bank    (dbg_tl_resp_bank),
        .dbg_tl_ptw_active   (dbg_tl_ptw_active),
        .dbg_tl_ptw_r_fire   (dbg_tl_ptw_r_fire),
        .dbg_tl_iotlb_lookup (dbg_tl_iotlb_lookup),
        .dbg_tl_iotlb_hit    (dbg_tl_iotlb_hit),

        .cfg_sel    (cfg_sel),
        .cfg_valid  (cfg_valid),
        .cfg_write  (cfg_write),
        .cfg_addr   (cfg_addr),
        .cfg_wdata  (cfg_wdata),
        .cfg_wstrb  (cfg_wstrb),
        .cfg_ready  (cfg_ready),
        .cfg_rdata  (cfg_rdata),
        .cfg_error  (cfg_error),

        .dbg_dma_issue        (dbg_dma_issue),
        .dbg_dma_done         (dbg_dma_done),
        .dbg_dma_poll         (dbg_dma_poll),
        .dbg_dma_batch_active (dbg_dma_batch_active),
        .dbg_dma_batch_gap    (dbg_dma_batch_gap),
        .dbg_dma_job_quiet    (dbg_dma_job_quiet),
        .dbg_dma_fullbuffer   (dbg_dma_fullbuffer),
        .dbg_dma_launch_active(dbg_dma_launch_active),
        .dbg_dma_launch_held  (dbg_dma_launch_held),
        .dbg_dma_launch_error (dbg_dma_launch_error),

        .dbg_nonblock_fire    (dbg_nonblock_fire),
        .dbg_nonblock_busy    (dbg_nonblock_busy),
        .dbg_in_flight        (dbg_in_flight),
        .dbg_fast_resp_valid  (dbg_fast_resp_valid),
        .dbg_fast_resp_ready  (dbg_fast_resp_ready),
        .dbg_fast_resp_bank   (dbg_fast_resp_bank),
        .dbg_slow_resp_valid  (dbg_slow_resp_valid),
        .dbg_bank_busy        (dbg_bank_busy),
        .dbg_cfg_state        (dbg_cfg_state)
    );

    // ========================================================================
    // Test-only observation taps
    // ========================================================================
    assign dbg_comp_ar_fire = u_comp_ar_valid & slv_resps[3].ar_ready;
    assign dbg_comp_aw_fire = u_comp_aw_valid & slv_resps[3].aw_ready;
    assign dbg_comp_w_fire  = u_comp_w_valid  & slv_resps[3].w_ready;
    assign dbg_ddtw_ar_fire = u_ddtw_ar_valid & ddtw_resp.ar_ready;
    assign dbg_ptw_ar_fire  = u_ptw_ar_valid  & ptw_resp.ar_ready;

    assign dbg_ds_ar_valid  = ds_delayed_req.ar_valid;
    assign dbg_ds_ar_ready  = ds_delayed_resp.ar_ready;
    assign dbg_ds_r_valid   = ds_delayed_resp.r_valid;
    assign dbg_ds_r_ready   = ds_delayed_req.r_ready;

    assign dbg_ddr_ar_valid = ddr_ar_valid;
    assign dbg_ddr_ar_ready = ddr_ar_ready;
    assign dbg_ddr_r_valid  = ddr_r_valid;
    assign dbg_ddr_r_ready  = ddr_r_ready;
    assign dbg_ddr_aw_valid = ddr_aw_valid;
    assign dbg_ddr_aw_ready = ddr_aw_ready;
    assign dbg_ddr_b_valid  = ddr_b_valid;
    assign dbg_ddr_b_ready  = ddr_b_ready;

    assign dbg_cpu_issued    = cpu_issued_q;
    assign dbg_cpu_completed = cpu_completed_q;
    assign dbg_cpu_active    = cpu_run;
    assign dbg_cpu_quiet     = ~cpu_ar_valid_q & ~cpu_outstanding_q & ~cpu_run;

endmodule
