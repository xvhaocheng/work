// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_actual_dma_system_top.sv
//
// BOUNDED DIAGNOSTIC, TEST-ONLY SYSTEM-INTEGRATION WRAPPER -- NOT PRODUCTION RTL.
//
// This wrapper re-instantiates tests/nonblock_actual_dma_top.sv
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
// Ports of the crossbar (deliberately reduced vs the 12-port board fabric):
//     slv 0 : synthetic read-only CPU master   (TEST-ONLY, see below)
//     slv 1 : debug/idle master                (tied off)
//     slv 3 : IOMMU data structures -> delayer (rv_iommu_ds_if output)
//     slv 2 : IOMMU completion master          (from the inner harness)
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
// It injects one bounded read stream (ID0) plus, only with
// --system-atomics --cpu-writes, a plain-write stream (ID2) and an AMO stream
// (ATOMICLOAD+ADD, ID1), to reproduce CPU<->DMA<->walker contention through the
// real crossbar.  It starts when DMA0's batch becomes active and stops offering
// NEW read/write/AMO offers when the batch (active|gap) ends, then drains any
// outstanding beat/transaction.  AR/AW/W valids are never withdrawn while
// stalled; RREADY keeps the original fixed 16-cycle-ready / 4-cycle-stall
// pattern for the read lane.
//
// DELIBERATE LIMITATIONS (must be reported with any result):
//   * no real CPU / caches / axi_adapter / dwidth converters;
//   * only 2 of the 12 board master ports; the peripheral fabric, PLIC/CLINT/
//     ROM/UART/SPI/GPIO/Ethernet/timer are NOT instantiated;
//   * optional real DDR atomics enabled by --system-atomics; the optional
//     --cpu-writes AMO traffic is synthetic test stimulus, not a real CPU and
//     does not reproduce any board measurement;
//   * no MIG / DDR controller / clock converter / 512-to-64 width converter:
//     DDR is a behavioural model in the C++ testbench;
//   * the cfg_* AXI slave selector still reaches the two dma_core_wrap slaves
//     DIRECTLY (it bypasses the CPU crossbar and the production dma_cfg
//     axi_demux).  This is unchanged behaviour, now an explicit limitation.
//
// Default admission mode remains passthrough. Optional test-only admission
// modes add a completion-AR proxy; no production backend is replaced.

`include "axi/typedef.svh"
`include "axi/assign.svh"

module nonblock_actual_dma_system_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter int unsigned ParentDepth = 8,
    parameter bit          NonblockEnable       = 1,
    parameter bit          DdrAtomics           = 0,
    // Test-only: add the synthetic CPU plain-write / AMO stimulus.  Only
    // effective together with DdrAtomics (--system-atomics --cpu-writes).
    parameter bit          CpuWrites            = 0,
    parameter int unsigned NumOutstandingTrans = 8,
    parameter int unsigned WFifoDepth          = 32,
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
    parameter int unsigned PoolSlots           = 1,
    // Test-only: cycles between the synthetic CPU read offers.
    parameter int unsigned CpuIssueInterval    = 32,
    // Test-only: DS path delay (board constants).
    parameter int unsigned DsFixedDelayInput   = 50,
    parameter int unsigned DsFixedDelayOutput  = 50
) (
    input  logic        clk_i,
    input  logic        rst_ni,
    input logic reuse_enable_i, reuse_prepare_i,
    input logic [1:0] prepare_order_i, physical_bypass_i,
    input logic [15:0] shared_read_quantum_i,
    input logic shared_write_keep_q_i,
    input logic shared_source_cutover_i,
    input logic shared_write_prefetch_i,
    input logic shared_write_progress_i,

    // §42 runtime diagnostic inputs; old obligations are never cancelled.
    input logic pause_a_i, pause_reads_a_i,
    output logic dbg_shared_detached,
    output logic [63:0] dbg_shared_cut_bytes,
    output logic dbg_shared_cutover,
    output logic [63:0] dbg_shared_cutover_vcut_bytes, dbg_shared_cutover_oldend_bytes,
    output logic dbg_shared_write_prefetch_ready,
    output logic dbg_shared_write_progress_active,
    output logic [31:0] dbg_shared_parents,
    output logic [31:0] dbg_pause_a_offers, dbg_pause_a_wlast,
        dbg_pause_a_bpending, dbg_pause_a_children, dbg_pause_a_parents,
    output logic [63:0] dbg_pause_a_end, dbg_pause_a_rbytes,
        dbg_pause_a_wbytes, dbg_pause_a_childbytes,
        dbg_pause_v_rbytes, dbg_pause_v_wbytes,
    output logic dbg_pause_a_held, dbg_pause_a_read_busy,
        dbg_pause_a_prepare_busy, dbg_pause_a_quiet,


    input  logic        nonblock_enable_i,
    input  logic        policy_partition_i,
    input  logic        owner_only_i,

    // Test-only read-admission proxy controls (SYSTEM build only).
    input  logic [2:0]  read_admit_mode_i,
    input  logic [31:0] read_budget_i,
    input  logic [31:0] read_period_i,

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

    output logic        dbg_comp_ar_fire,
    output logic        dbg_comp_aw_fire,
    output logic        dbg_comp_w_fire,
    output logic        dbg_ddtw_ar_fire,
    output logic        dbg_ptw_ar_fire,
    // Simulation-only probes. No production port or synthesized observer.
    output logic        dbg_tr_aw_stall,
    output logic        dbg_tr_aw_bank,
    // Raw, possibly overlapping causes: cross-bank W, capacity, ID mapping,
    // selected-bank not ready. C++ also reports a priority classification.
    output logic [3:0]  dbg_aw_wait_flags,
    output logic        dbg_iotlb_fill,
    output logic        dbg_main_bank,
    output logic        dbg_policy_elaborated,
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
    output logic        dbg_cpu_active,

    // ---------------- synthetic CPU write / AMO lifecycle (optional) --------
    output logic [31:0] dbg_cpu_wr_issued,
    output logic [31:0] dbg_cpu_wr_completed,
    output logic [31:0] dbg_cpu_amo_issued,
    output logic [31:0] dbg_cpu_amo_completed,
    output logic [31:0] dbg_cpu_rd_err,
    output logic [31:0] dbg_cpu_wr_err,
    output logic [31:0] dbg_cpu_amo_err,
    output logic        dbg_cpu_wr_quiet,
    output logic        dbg_cpu_amo_r_fire,
    output logic [63:0] dbg_cpu_amo_r_data,

    // ---------------- test-only read-admission proxy observation ----------
    output logic        dbg_ra_new_captured,
    output logic        dbg_ra_first_offer,
    output logic        dbg_ra_ar_fire,
    output logic        dbg_ra_gate_ingress,
    output logic        dbg_ra_gate_budget,
    output logic        dbg_ra_exempt,
    output logic        dbg_ra_oversize_escape,
    output logic        dbg_ra_fastgated,
    output logic        dbg_ra_phase_reset,
    output logic [31:0] dbg_ra_total_debt,
    output logic        dbg_ra_buffer_empty,
    output logic [31:0] dbg_ra_capture_count,
    output logic [31:0] dbg_ra_offer_handshake_count,
    output logic [31:0] dbg_ra_tag_fault, dbg_ra_tag_queued,
    output logic        dbg_ra_capture_fast,
    output logic        dbg_ra_capture_bank,
    output logic [3:0]  dbg_ra_capture_id,
    output logic [63:0] dbg_ra_capture_addr,
    output logic [7:0]  dbg_ra_capture_len,
    output logic        dbg_ra_up_valid,
    output logic        dbg_ra_up_ready,
    output logic [3:0]  dbg_ra_up_id,
    output logic [63:0] dbg_ra_up_addr,
    output logic [7:0]  dbg_ra_up_len,
    output logic        dbg_ra_up_bank,
    output logic [8:0]  dbg_ra_up_attrs,
    output logic        dbg_ra_down_valid,
    output logic        dbg_ra_down_ready,
    output logic [3:0]  dbg_ra_down_id,
    output logic [63:0] dbg_ra_down_addr,
    output logic [7:0]  dbg_ra_down_len,
    output logic [8:0]  dbg_ra_down_attrs,

    // ---------------- test-only natural write-order taps (read-only) --------
    // Direct, purely observational wires to the inner harness translation
    // request port.  No register, no feedback, no change to any existing
    // module name or port; the DUT cannot see these signals.
    //   nat_vaw_* : virtual completion AW offered at the IOMMU request port
    //   nat_var_* : virtual source AR offered at the IOMMU request port
    //   nat_vw_fire / nat_vw_last : virtual W beat handshake / WLAST level
    output logic        nat_vaw_valid,
    output logic        nat_vaw_ready,
    output logic [3:0]  nat_vaw_id,
    output logic [63:0] nat_vaw_addr,
    output logic [7:0]  nat_vaw_len,
    output logic        nat_var_valid,
    output logic        nat_var_ready,
    output logic [3:0]  nat_var_id,
    output logic [63:0] nat_var_addr,
    output logic [7:0]  nat_var_len,
    output logic        nat_vw_fire,
    output logic        nat_vw_last,
    output logic [31:0] nat_w_count0,
    output logic [31:0] nat_w_count1,
    output logic [1:0]  nat_w_overflow
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

    logic        u_comp_ar_ready;
    slv_ar_chan_t u_comp_ar_chan;
    logic        proxy_ar_valid;
    logic        proxy_ar_ready;
    slv_ar_chan_t proxy_ar_chan;
    logic fragment_ar_valid, fragment_ar_ready, fragment_r_ready;
    logic fragment_r_valid;
    slv_ar_chan_t fragment_ar_chan;
    slv_r_chan_t fragment_r_chan;
    logic [31:0] fragment_parents, fragment_children, fragment_merged, fragment_lasts;

    logic [1:0][31:0] comp_aw_pending_q;
    logic        comp_aw_fire, comp_b_fire;
    logic        comp_aw_bank, comp_b_bank;

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

    assign slv_reqs[3]     = ds_delayed_req;
    assign ds_delayed_resp = slv_resps[3];

    // ------------------------------------------------------------------------
    // Slave port 2 : completion master from the inner harness
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
        comp_req.aw.cache = i_dma.i_sys.comp_req.aw.cache;
        comp_req.aw.lock = i_dma.i_sys.comp_req.aw.lock;
        comp_req.aw.qos = i_dma.i_sys.comp_req.aw.qos;
        comp_req.aw.region = i_dma.i_sys.comp_req.aw.region;
        comp_req.ar_valid = proxy_ar_valid;
        comp_req.ar = proxy_ar_chan;
        comp_req.w_valid  = u_comp_w_valid;
        comp_req.w.data   = u_comp_w_data;
        comp_req.w.strb   = u_comp_w_strb;
        comp_req.w.last   = u_comp_w_last;
        comp_req.b_ready  = u_comp_b_ready;
        comp_req.r_ready  = fragment_r_ready;
    end
    always_comb begin : comp_ar_in_pack
        u_comp_ar_chan = i_dma.i_sys.comp_req.ar;
        if (i_dma.direct_ar_valid) begin
            u_comp_ar_chan.id = i_dma.pre_ar.id | 4'd1;
            u_comp_ar_chan.addr = i_dma.pre_ar.addr;
            u_comp_ar_chan.len = i_dma.pre_ar.len;
            u_comp_ar_chan.size = i_dma.pre_ar.size;
            u_comp_ar_chan.burst = i_dma.pre_ar.burst;
            u_comp_ar_chan.lock = i_dma.pre_ar.lock;
            u_comp_ar_chan.cache = i_dma.pre_ar.cache;
            u_comp_ar_chan.prot = i_dma.pre_ar.prot;
            u_comp_ar_chan.qos = i_dma.pre_ar.qos;
            u_comp_ar_chan.region = i_dma.pre_ar.region;
            u_comp_ar_chan.user = i_dma.pre_ar.user;
        end
    end

    assign slv_reqs[2] = comp_req;

    // ------------------------------------------------------------------------
    // Slave port 1 : debug / idle master
    // ------------------------------------------------------------------------
    assign slv_reqs[1] = '0;

    // ========================================================================
    // Slave port 0 : synthetic CPU master (TEST-ONLY)
    //   ID0 : plain read stream (original behaviour, unchanged)
    //   ID1 : ATOMICLOAD + ADD 64-bit (optional; CpuWrites & DdrAtomics)
    //   ID2 : plain 2-beat write burst (optional; CpuWrites & DdrAtomics)
    //
    // Synthetic AXI stimulus only: NOT a real CPU, NOT a cache model, and no
    // board-equivalence claim.  At most ONE write transaction (AW+W+B, plus the
    // R of an AMO) is outstanding at a time.
    //
    // AW and W offers are controlled independently by a free-running, saturating
    // per-transaction age counter that depends on NO ready: when a transaction
    // starts, each channel's offer is delayed by a fixed number of cycles and
    // then held stable until its own handshake.  The (AW,W) skew rotates
    // (2,0) / (0,0) / (0,2) cycles.  AWVALID never depends on WREADY or the W
    // handshake and WVALID never depends on AWREADY or the AW handshake, so the
    // two channels can never deadlock each other.  Only the next beat of a W
    // burst waits for the previous W beat's handshake (legal AXI ordering).
    //
    // Responses are never dropped: this engine asserts its BREADY / AMO RREADY
    // only in states that capture the response (an early B/R stays asserted by
    // the slave and is captured later), and capture is strictly by response ID.
    // The shared RREADY is the OR of the read lane's periodic ready and the AMO
    // lane's accept, with the read lane latching ID0 and the AMO lane ID1.
    //
    // New transactions start while cpu_run is asserted and continue until it
    // drops; the transaction in flight is then drained.  CPU_WR_MAX is only a
    // large safety bound and is not expected to be reached in a normal N4096 run.
    // ========================================================================
    localparam bit CpuWrEn = CpuWrites && DdrAtomics;

    localparam logic [3:0]  CPU_ID_RD    = 4'd0;
    localparam logic [3:0]  CPU_ID_AMO   = 4'd1;
    localparam logic [3:0]  CPU_ID_WR    = 4'd2;

    localparam logic [7:0]  CPU_WR_LEN   = 8'd1;            // 2 x 8 B burst
    localparam logic [63:0] CPU_WR_ADDR  = 64'h0000_0000_8100_0000;
    localparam logic [63:0] CPU_AMO_ADDR = 64'h0000_0000_8100_1000;
    localparam logic [63:0] CPU_AMO_ADD  = 64'h0000_0000_1234_5678;
    localparam logic [63:0] CPU_WR_DAT0  = 64'h1122_3344_5566_7788;
    localparam logic [63:0] CPU_WR_DAT1  = 64'hAABB_CCDD_EEFF_0011;
    localparam logic [7:0]  CPU_WR_STR0  = 8'hFF;
    localparam logic [7:0]  CPU_WR_STR1  = 8'h0F;
    // ATOMICLOAD (ATOP[5:4]=2'b10), little endian (ATOP[3]=0), ADD (ATOP[2:0]).
    localparam logic [5:0]  CPU_AMO_ATOP =
        {axi_pkg::ATOP_ATOMICLOAD, axi_pkg::ATOP_LITTLE_END, axi_pkg::ATOP_ADD};
    // Large safety bound only; the effective bound is cpu_run plus the C++
    // scenario budget and independent watchdog.
    localparam logic [31:0] CPU_WR_MAX   = 32'd1048576;

    logic        cpu_run;
    assign cpu_run = dbg_dma_batch_active[0] | dbg_dma_batch_gap[0];

    localparam logic [5:0] CPU_INTERVAL = 6'(CpuIssueInterval);

    slv_req_t    cpu_req;

    // ---- plain read lane (ID0) -------------------------------------------
    logic        cpu_ar_valid_q;
    logic [3:0]  cpu_ar_id_q;
    logic [63:0] cpu_ar_addr_q;
    logic [7:0]  cpu_ar_len_q;
    logic [2:0]  cpu_ar_size_q;
    logic [1:0]  cpu_ar_burst_q;
    logic        cpu_outstanding_q;
    logic [31:0] cpu_issued_q;
    logic [31:0] cpu_completed_q;
    logic [31:0] cpu_rd_err_q;
    logic [5:0]  cpu_wait_q;
    logic [4:0]  cpu_rphase_q;
    logic        cpu_r_ready;

    // ---- write / AMO engine ----------------------------------------------
    typedef enum logic [1:0] {CW_IDLE, CW_ISSUE, CW_RESP} cw_state_e;

    cw_state_e   cw_state_q;
    logic        cw_amo_q;
    logic [1:0]  cw_skew_q;
    logic [3:0]  cw_age_q;
    logic        cw_aw_done_q;
    logic [7:0]  cw_wcnt_q;
    logic        cw_b_done_q;
    logic        cw_r_done_q;
    logic [1:0]  cw_next_q;
    logic [1:0]  cw_rot_q;
    logic [31:0] cw_txn_q;
    logic [31:0] cpu_wr_issued_q;
    logic [31:0] cpu_wr_completed_q;
    logic [31:0] cpu_amo_issued_q;
    logic [31:0] cpu_amo_completed_q;
    logic [31:0] cpu_wr_err_q;
    logic [31:0] cpu_amo_err_q;
    logic [63:0] cpu_amo_r_data_q;
    logic        cpu_amo_r_fire_q;

    logic        cw_aw_valid;
    logic [3:0]  cw_aw_id;
    logic [63:0] cw_aw_addr;
    logic [7:0]  cw_aw_len;
    logic [5:0]  cw_aw_atop;
    logic        cw_w_valid;
    logic [63:0] cw_w_data;
    logic [7:0]  cw_w_strb;
    logic        cw_w_last;
    logic [3:0]  cw_aw_delay;
    logic [3:0]  cw_w_delay;
    logic        cw_aw_fire;
    logic        cw_w_fire;
    logic        cw_aw_ok;
    logic [8:0]  cw_wdone_next;
    logic        cw_w_ok;
    logic        cw_b_accept;
    logic        cw_r_accept;
    logic        cw_b_fire;
    logic        cw_r_fire;
    logic [1:0]  cw_err_add;

    // ---- plain read lane FSM (ID0, original behaviour preserved) ----------
    always_ff @(posedge clk_i or negedge rst_ni) begin : cpu_read_fsm
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
            cpu_rd_err_q      <= '0;
            cpu_wait_q        <= CPU_INTERVAL;
            cpu_rphase_q      <= '0;
        end else begin
            // ---- AR offer: held stable until accepted, never withdrawn ----
            if (cpu_ar_valid_q) begin
                if (slv_resps[0].ar_ready) begin
                    cpu_ar_valid_q    <= 1'b0;
                    cpu_outstanding_q <= 1'b1;
                    cpu_issued_q      <= cpu_issued_q + 32'd1;
                end
            end else if (!cpu_outstanding_q && cpu_run && (cpu_wait_q == '0)) begin
                cpu_ar_valid_q <= 1'b1;
                cpu_ar_id_q    <= CPU_ID_RD;
                cpu_ar_addr_q  <= 64'h0000_0000_8000_0000; // V_SRC_PA
                cpu_ar_len_q   <= 8'd7;                    // 8 x 8 B = 64 B
                cpu_ar_size_q  <= 3'd3;
                cpu_ar_burst_q <= axi_pkg::BURST_INCR;
            end

            // ---- bounded inter-read interval ------------------------------
            if (cpu_ar_valid_q || cpu_outstanding_q) cpu_wait_q <= CPU_INTERVAL;
            else if (cpu_wait_q != '0)               cpu_wait_q <= cpu_wait_q - 6'd1;

            // ---- read-lane R capture: ID0 only, every beat caught ----------
            if (slv_resps[0].r_valid && cpu_r_ready &&
                (slv_resps[0].r.id == CPU_ID_RD)) begin
                if (slv_resps[0].r.resp != axi_pkg::RESP_OKAY)
                    cpu_rd_err_q <= cpu_rd_err_q + 32'd1;
                if (slv_resps[0].r.last) begin
                    cpu_outstanding_q <= 1'b0;
                    cpu_completed_q   <= cpu_completed_q + 32'd1;
                end
            end

            // ---- RREADY phase: 16 cycles ready, 4 cycles stalled -----------
            if (cpu_rphase_q == 5'd19) cpu_rphase_q <= '0;
            else                       cpu_rphase_q <= cpu_rphase_q + 5'd1;
        end
    end

    // ---- write / AMO transaction descriptor ------------------------------
    assign cw_aw_len  = cw_amo_q ? 8'd0 : CPU_WR_LEN;
    assign cw_aw_id   = cw_amo_q ? CPU_ID_AMO : CPU_ID_WR;
    assign cw_aw_addr = cw_amo_q ? CPU_AMO_ADDR : CPU_WR_ADDR;
    assign cw_aw_atop = cw_amo_q ? CPU_AMO_ATOP : 6'b000000;

    // Fixed offer delays after the transaction starts.  (AW,W)=(2,0)/(0,0)/(0,2)
    assign cw_aw_delay = (cw_skew_q == 2'd0) ? 4'd2 : 4'd0;
    assign cw_w_delay  = (cw_skew_q == 2'd2) ? 4'd2 : 4'd0;

    // Valid offers.  AW never waits for W and W never waits for AW, so neither
    // channel can hold the other's valid hostage.
    assign cw_aw_valid = (cw_state_q == CW_ISSUE) && !cw_aw_done_q &&
                         (cw_age_q >= cw_aw_delay);
    assign cw_w_valid  = (cw_state_q == CW_ISSUE) && (cw_wcnt_q <= cw_aw_len) &&
                         (cw_age_q >= cw_w_delay);
    assign cw_w_last   = (cw_wcnt_q == cw_aw_len);

    assign cw_aw_fire  = cw_aw_valid && slv_resps[0].aw_ready;
    assign cw_w_fire   = cw_w_valid  && slv_resps[0].w_ready;

    assign cw_aw_ok      = cw_aw_done_q || cw_aw_fire;
    assign cw_wdone_next = {1'b0, cw_wcnt_q} + (cw_w_fire ? 9'd1 : 9'd0);
    assign cw_w_ok       = cw_wdone_next > {1'b0, cw_aw_len};

    always_comb begin : cw_w_payload
        if (cw_amo_q) begin
            cw_w_data = CPU_AMO_ADD;
            cw_w_strb = 8'hFF;
        end else if (cw_wcnt_q == 8'd0) begin
            cw_w_data = CPU_WR_DAT0;
            cw_w_strb = CPU_WR_STR0;
        end else begin
            cw_w_data = CPU_WR_DAT1;
            cw_w_strb = CPU_WR_STR1;
        end
    end

    // Response accept: the engine only asserts its own BREADY / AMO RREADY when
    // it can capture, and both lanes latch strictly by response ID, so no
    // already-handshaked B/R is ever lost.
    assign cw_b_accept = (cw_state_q == CW_RESP) && !cw_b_done_q;
    assign cw_r_accept = (cw_state_q == CW_RESP) && cw_amo_q && !cw_r_done_q;
    assign cpu_r_ready = (cpu_rphase_q < 5'd16) | cw_r_accept;

    assign cw_b_fire   = slv_resps[0].b_valid && cw_b_accept &&
                         (slv_resps[0].b.id == cw_aw_id);
    assign cw_r_fire   = slv_resps[0].r_valid && cpu_r_ready &&
                         (cw_state_q != CW_IDLE) &&
                         (slv_resps[0].r.id == CPU_ID_AMO);

    always_comb begin : cw_err_cnt
        cw_err_add = 2'd0;
        if (cw_b_fire && (slv_resps[0].b.resp != axi_pkg::RESP_OKAY))
            cw_err_add = cw_err_add + 2'd1;
        if (cw_r_fire && (slv_resps[0].r.resp != axi_pkg::RESP_OKAY))
            cw_err_add = cw_err_add + 2'd1;
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin : cpu_write_fsm
        if (!rst_ni) begin
            cw_state_q          <= CW_IDLE;
            cw_amo_q            <= 1'b0;
            cw_skew_q           <= 2'd0;
            cw_age_q            <= 4'd0;
            cw_aw_done_q        <= 1'b0;
            cw_wcnt_q           <= '0;
            cw_b_done_q         <= 1'b0;
            cw_r_done_q         <= 1'b0;
            cw_next_q           <= 2'd0;
            cw_rot_q            <= 2'd0;
            cw_txn_q            <= '0;
            cpu_wr_issued_q     <= '0;
            cpu_wr_completed_q  <= '0;
            cpu_amo_issued_q    <= '0;
            cpu_amo_completed_q <= '0;
            cpu_wr_err_q        <= '0;
            cpu_amo_err_q       <= '0;
            cpu_amo_r_data_q    <= '0;
            cpu_amo_r_fire_q    <= 1'b0;
        end else begin
            cpu_amo_r_fire_q <= 1'b0;

            // ---- free-running age: increments every cycle in ISSUE,
            //      saturates, and depends on NO ready -------------------------
            if (cw_state_q != CW_ISSUE) cw_age_q <= 4'd0;
            else if (cw_age_q != 4'hF)  cw_age_q <= cw_age_q + 4'd1;

            // ---- response capture: never state-gated, by ID only -----------
            if (cw_b_fire) cw_b_done_q <= 1'b1;
            if (cw_r_fire) begin
                cw_r_done_q      <= 1'b1;
                cpu_amo_r_data_q <= slv_resps[0].r.data;
                cpu_amo_r_fire_q <= 1'b1;
            end

            // ---- per-response error counting -------------------------------
            if (cw_err_add != 2'd0) begin
                if (cw_amo_q) cpu_amo_err_q <= cpu_amo_err_q + {30'b0, cw_err_add};
                else          cpu_wr_err_q  <= cpu_wr_err_q  + {30'b0, cw_err_add};
            end

            case (cw_state_q)
              CW_IDLE: begin
                // New transactions start only while the model runs, up to a
                // large safety bound that a normal N4096 run never reaches.
                if (CpuWrEn && cpu_run && (cw_txn_q < CPU_WR_MAX)) begin
                    cw_state_q   <= CW_ISSUE;
                    cw_amo_q     <= cw_next_q[0];
                    cw_skew_q    <= cw_rot_q;
                    cw_aw_done_q <= 1'b0;
                    cw_wcnt_q    <= '0;
                    cw_b_done_q  <= 1'b0;
                    cw_r_done_q  <= 1'b0;
                    cw_txn_q     <= cw_txn_q + 32'd1;
                    if (cw_next_q[0]) cpu_amo_issued_q <= cpu_amo_issued_q + 32'd1;
                    else              cpu_wr_issued_q  <= cpu_wr_issued_q  + 32'd1;
                    cw_next_q <= {1'b0, ~cw_next_q[0]};
                    cw_rot_q  <= (cw_rot_q == 2'd2) ? 2'd0 : (cw_rot_q + 2'd1);
                end
              end

              CW_ISSUE: begin
                if (cw_aw_fire) cw_aw_done_q <= 1'b1;
                if (cw_w_fire)  cw_wcnt_q    <= cw_wcnt_q + 8'd1;
                if (cw_aw_ok && cw_w_ok) cw_state_q <= CW_RESP;
              end

              CW_RESP: begin
                if ((cw_b_done_q || cw_b_fire) &&
                    (cw_amo_q ? (cw_r_done_q || cw_r_fire) : 1'b1)) begin
                    if (cw_amo_q) cpu_amo_completed_q <= cpu_amo_completed_q + 32'd1;
                    else          cpu_wr_completed_q  <= cpu_wr_completed_q  + 32'd1;
                    cw_state_q <= CW_IDLE;
                end
              end

              default: cw_state_q <= CW_IDLE;
            endcase
        end
    end

    always_comb begin : cpu_req_drive
        cpu_req = '0;
        // ---- plain read (ID0) --------------------------------------------
        cpu_req.ar_valid = cpu_ar_valid_q;
        cpu_req.ar.id    = cpu_ar_id_q;
        cpu_req.ar.addr  = cpu_ar_addr_q;
        cpu_req.ar.len   = cpu_ar_len_q;
        cpu_req.ar.size  = cpu_ar_size_q;
        cpu_req.ar.burst = cpu_ar_burst_q;
        cpu_req.ar.prot  = 3'b010;
        // ---- plain write (ID2) / AMO (ID1) -------------------------------
        cpu_req.aw_valid = cw_aw_valid;
        cpu_req.aw.id    = cw_aw_id;
        cpu_req.aw.addr  = cw_aw_addr;
        cpu_req.aw.len   = cw_aw_len;
        cpu_req.aw.size  = 3'd3;
        cpu_req.aw.burst = axi_pkg::BURST_INCR;
        cpu_req.aw.atop  = cw_aw_atop;
        cpu_req.aw.prot  = 3'b010;
        cpu_req.w_valid  = cw_w_valid;
        cpu_req.w.data   = cw_w_data;
        cpu_req.w.strb   = cw_w_strb;
        cpu_req.w.last   = cw_w_last;
        // ---- capture-only response ready ---------------------------------
        cpu_req.b_ready  = cw_b_accept;
        cpu_req.r_ready  = cpu_r_ready;
    end
    assign slv_reqs[0] = cpu_req;

    // ========================================================================
    // Crossbar address map (reduced; 2 of the 12 board master ports)
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
    // Optional production DDR atomics adapter, exactly the board widths/limit.
    // This is a diagnostic model extension, not a production RTL change.
    mst_req_t ddr_req;
    mst_resp_t ddr_resp;
    if (DdrAtomics) begin : gen_ddr_atomics
        axi_riscv_atomics #(
            .AXI_ADDR_WIDTH(64), .AXI_DATA_WIDTH(64), .AXI_ID_WIDTH(6),
            .AXI_USER_WIDTH(1), .AXI_MAX_WRITE_TXNS(1), .RISCV_WORD_WIDTH(64)
        ) i_atomics (
            .clk_i, .rst_ni,
            .slv_aw_addr_i (mst_reqs[0].aw.addr),
            .slv_aw_prot_i (mst_reqs[0].aw.prot),
            .slv_aw_region_i (mst_reqs[0].aw.region),
            .slv_aw_atop_i (mst_reqs[0].aw.atop),
            .slv_aw_len_i (mst_reqs[0].aw.len),
            .slv_aw_size_i (mst_reqs[0].aw.size),
            .slv_aw_burst_i (mst_reqs[0].aw.burst),
            .slv_aw_lock_i (mst_reqs[0].aw.lock),
            .slv_aw_cache_i (mst_reqs[0].aw.cache),
            .slv_aw_qos_i (mst_reqs[0].aw.qos),
            .slv_aw_id_i (mst_reqs[0].aw.id),
            .slv_aw_user_i (mst_reqs[0].aw.user),
            .slv_aw_ready_o (mst_resps[0].aw_ready),
            .slv_aw_valid_i (mst_reqs[0].aw_valid),
            .slv_ar_addr_i (mst_reqs[0].ar.addr),
            .slv_ar_prot_i (mst_reqs[0].ar.prot),
            .slv_ar_region_i (mst_reqs[0].ar.region),
            .slv_ar_len_i (mst_reqs[0].ar.len),
            .slv_ar_size_i (mst_reqs[0].ar.size),
            .slv_ar_burst_i (mst_reqs[0].ar.burst),
            .slv_ar_lock_i (mst_reqs[0].ar.lock),
            .slv_ar_cache_i (mst_reqs[0].ar.cache),
            .slv_ar_qos_i (mst_reqs[0].ar.qos),
            .slv_ar_id_i (mst_reqs[0].ar.id),
            .slv_ar_user_i (mst_reqs[0].ar.user),
            .slv_ar_ready_o (mst_resps[0].ar_ready),
            .slv_ar_valid_i (mst_reqs[0].ar_valid),
            .slv_w_data_i (mst_reqs[0].w.data),
            .slv_w_strb_i (mst_reqs[0].w.strb),
            .slv_w_user_i (mst_reqs[0].w.user),
            .slv_w_last_i (mst_reqs[0].w.last),
            .slv_w_ready_o (mst_resps[0].w_ready),
            .slv_w_valid_i (mst_reqs[0].w_valid),
            .slv_r_data_o (mst_resps[0].r.data),
            .slv_r_resp_o (mst_resps[0].r.resp),
            .slv_r_last_o (mst_resps[0].r.last),
            .slv_r_id_o (mst_resps[0].r.id),
            .slv_r_user_o (mst_resps[0].r.user),
            .slv_r_ready_i (mst_reqs[0].r_ready),
            .slv_r_valid_o (mst_resps[0].r_valid),
            .slv_b_resp_o (mst_resps[0].b.resp),
            .slv_b_id_o (mst_resps[0].b.id),
            .slv_b_user_o (mst_resps[0].b.user),
            .slv_b_ready_i (mst_reqs[0].b_ready),
            .slv_b_valid_o (mst_resps[0].b_valid),
            .mst_aw_addr_o (ddr_req.aw.addr),
            .mst_aw_prot_o (ddr_req.aw.prot),
            .mst_aw_region_o (ddr_req.aw.region),
            .mst_aw_atop_o (ddr_req.aw.atop),
            .mst_aw_len_o (ddr_req.aw.len),
            .mst_aw_size_o (ddr_req.aw.size),
            .mst_aw_burst_o (ddr_req.aw.burst),
            .mst_aw_lock_o (ddr_req.aw.lock),
            .mst_aw_cache_o (ddr_req.aw.cache),
            .mst_aw_qos_o (ddr_req.aw.qos),
            .mst_aw_id_o (ddr_req.aw.id),
            .mst_aw_user_o (ddr_req.aw.user),
            .mst_aw_ready_i (ddr_resp.aw_ready),
            .mst_aw_valid_o (ddr_req.aw_valid),
            .mst_ar_addr_o (ddr_req.ar.addr),
            .mst_ar_prot_o (ddr_req.ar.prot),
            .mst_ar_region_o (ddr_req.ar.region),
            .mst_ar_len_o (ddr_req.ar.len),
            .mst_ar_size_o (ddr_req.ar.size),
            .mst_ar_burst_o (ddr_req.ar.burst),
            .mst_ar_lock_o (ddr_req.ar.lock),
            .mst_ar_cache_o (ddr_req.ar.cache),
            .mst_ar_qos_o (ddr_req.ar.qos),
            .mst_ar_id_o (ddr_req.ar.id),
            .mst_ar_user_o (ddr_req.ar.user),
            .mst_ar_ready_i (ddr_resp.ar_ready),
            .mst_ar_valid_o (ddr_req.ar_valid),
            .mst_w_data_o (ddr_req.w.data),
            .mst_w_strb_o (ddr_req.w.strb),
            .mst_w_user_o (ddr_req.w.user),
            .mst_w_last_o (ddr_req.w.last),
            .mst_w_ready_i (ddr_resp.w_ready),
            .mst_w_valid_o (ddr_req.w_valid),
            .mst_r_data_i (ddr_resp.r.data),
            .mst_r_resp_i (ddr_resp.r.resp),
            .mst_r_last_i (ddr_resp.r.last),
            .mst_r_id_i (ddr_resp.r.id),
            .mst_r_user_i (ddr_resp.r.user),
            .mst_r_ready_o (ddr_req.r_ready),
            .mst_r_valid_i (ddr_resp.r_valid),
            .mst_b_resp_i (ddr_resp.b.resp),
            .mst_b_id_i (ddr_resp.b.id),
            .mst_b_user_i (ddr_resp.b.user),
            .mst_b_ready_o (ddr_req.b_ready),
            .mst_b_valid_i (ddr_resp.b_valid)
        );
    end else begin : gen_ddr_bypass
        assign ddr_req = mst_reqs[0];
        assign mst_resps[0] = ddr_resp;
    end

    assign ddr_aw_valid = ddr_req.aw_valid;
    assign ddr_aw_id    = ddr_req.aw.id;
    assign ddr_aw_addr  = ddr_req.aw.addr;
    assign ddr_aw_len   = ddr_req.aw.len;
    assign ddr_aw_size  = ddr_req.aw.size;
    assign ddr_aw_burst = ddr_req.aw.burst;
    assign ddr_aw_prot  = ddr_req.aw.prot;
    assign ddr_aw_user  = ddr_req.aw.user;
    assign ddr_w_valid  = ddr_req.w_valid;
    assign ddr_w_data   = ddr_req.w.data;
    assign ddr_w_strb   = ddr_req.w.strb;
    assign ddr_w_last   = ddr_req.w.last;
    assign ddr_w_user   = ddr_req.w.user;
    assign ddr_b_ready  = ddr_req.b_ready;
    assign ddr_ar_valid = ddr_req.ar_valid;
    assign ddr_ar_id    = ddr_req.ar.id;
    assign ddr_ar_addr  = ddr_req.ar.addr;
    assign ddr_ar_len   = ddr_req.ar.len;
    assign ddr_ar_size  = ddr_req.ar.size;
    assign ddr_ar_burst = ddr_req.ar.burst;
    assign ddr_ar_prot  = ddr_req.ar.prot;
    assign ddr_ar_user  = ddr_req.ar.user;
    assign ddr_r_ready  = ddr_req.r_ready;

    always_comb begin : ddr_resp_drive
        ddr_resp = '0;
        ddr_resp.aw_ready = ddr_aw_ready;
        ddr_resp.w_ready  = ddr_w_ready;
        ddr_resp.b_valid  = ddr_b_valid;
        ddr_resp.b.id     = ddr_b_id;
        ddr_resp.b.resp   = ddr_b_resp;
        ddr_resp.ar_ready = ddr_ar_ready;
        ddr_resp.r_valid  = ddr_r_valid;
        ddr_resp.r.id     = ddr_r_id;
        ddr_resp.r.data   = ddr_r_data;
        ddr_resp.r.resp   = ddr_r_resp;
        ddr_resp.r.last   = ddr_r_last;
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
    // The inner actual-DMA harness (owner-only switch is test-only).
    // ========================================================================
    nonblock_actual_dma_top #(
        .NonblockEnable      (NonblockEnable),
        .NumOutstandingTrans (NumOutstandingTrans),
        .WFifoDepth          (WFifoDepth),
        .PolicyEnable        (PolicyEnable),
        .DmaBufferDepth      (DmaBufferDepth),
        .SharedPoolMode      (SharedPoolMode),
        .InterleaveAwEnable(InterleaveAwEnable),
        .InterleaveAwDelay(InterleaveAwDelay),
        .SameBankDmasEnable(SameBankDmasEnable),
        .ReverseReadOrderEnable(ReverseReadOrderEnable),
        .SourceReadGateEnable(SourceReadGateEnable),
        .FrontierEnable(FrontierEnable),
        .FrontierScanEnable(FrontierScanEnable),
        .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
        .HeaderDirectEnable(HeaderDirectEnable),
        .PoolSlots           (PoolSlots)
    ) i_dma (
        .pause_a_i, .pause_reads_a_i,
        .dbg_shared_detached, .dbg_shared_cut_bytes, .dbg_shared_parents,
        .dbg_shared_cutover, .dbg_shared_cutover_vcut_bytes, .dbg_shared_cutover_oldend_bytes,
        .dbg_shared_write_prefetch_ready,
        .dbg_shared_write_progress_active,
        .dbg_pause_a_offers, .dbg_pause_a_wlast, .dbg_pause_a_bpending,
        .dbg_pause_a_children, .dbg_pause_a_parents, .dbg_pause_a_end,
        .dbg_pause_a_rbytes, .dbg_pause_a_wbytes, .dbg_pause_a_childbytes,
        .dbg_pause_v_rbytes, .dbg_pause_v_wbytes,
        .dbg_pause_a_held, .dbg_pause_a_read_busy,
        .dbg_pause_a_prepare_busy, .dbg_pause_a_quiet,
        .reuse_enable_i(reuse_enable_i), .reuse_prepare_i(reuse_prepare_i), .prepare_order_i, .physical_bypass_i, .shared_read_quantum_i, .shared_write_keep_q_i, .shared_source_cutover_i, .shared_write_prefetch_i, .shared_write_progress_i,
        .clk_i,
        .rst_ni,
        .nonblock_enable_i  (nonblock_enable_i),
        .policy_partition_i (policy_partition_i),
        .owner_only_i       (owner_only_i),

        .dbg_e2_mode (dbg_e2_mode),
        .dbg_e2_slots (dbg_e2_slots),
        .dbg_e2_dma_depth (dbg_e2_dma_depth),
        .dbg_e2_occupancy (dbg_e2_occupancy),
        .dbg_e2_reserved (dbg_e2_reserved),
        .dbg_e2_busy (dbg_e2_busy),
        .dbg_e2_fault (dbg_e2_fault),
        .dbg_e2_grant_pool (dbg_e2_grant_pool),
        .dbg_e2_grant_stream (dbg_e2_grant_stream),
        .dbg_e2_received (dbg_e2_received),
        .dbg_e2_replayed (dbg_e2_replayed),
        .dbg_e2_highwater (dbg_e2_highwater),
        .dbg_e2_grant_future (dbg_e2_grant_future),
        .dbg_e2_aw_fixture_stall(dbg_e2_aw_fixture_stall),
        .dbg_e2_tc_read_wait(dbg_e2_tc_read_wait), .dbg_e2_tc_aw_pending(dbg_e2_tc_aw_pending),
        .dbg_e2_tc_state(dbg_e2_tc_state),
        .dbg_e2_tc_spill(dbg_e2_tc_spill), .dbg_e2_tc_ar_head(dbg_e2_tc_ar_head),
        .dbg_e2_tc_queue_ids(dbg_e2_tc_queue_ids),
        .dbg_hp_parked(dbg_hp_parked), .dbg_hp_bypassed(dbg_hp_bypassed),
        .dbg_hp_full_bypass(dbg_hp_full_bypass), .dbg_hp_highwater(dbg_hp_highwater),
        .dbg_hp_queued(dbg_hp_queued), .dbg_hp_dispatched(dbg_hp_dispatched),
        .dbg_e2_ar_fixture_stall(dbg_e2_ar_fixture_stall),
        .dbg_frontier_grant(),
        .dbg_frontier_wait(),
        .dbg_e2_meta_accepted (dbg_e2_meta_accepted),
        .dbg_e2_meta_early (dbg_e2_meta_early),
        .dbg_e2_meta_highwater (dbg_e2_meta_highwater),
        .dbg_e2_queued (dbg_e2_queued),


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
        .comp_aw_ready (slv_resps[2].aw_ready),
        .comp_ar_ready (u_comp_ar_ready),
        .comp_w_ready  (slv_resps[2].w_ready),
        .comp_b_valid  (slv_resps[2].b_valid),
        .comp_b_id     (slv_resps[2].b.id),
        .comp_b_resp   (slv_resps[2].b.resp),
        .comp_r_valid  (fragment_r_valid),
        .comp_r_id     (fragment_r_chan.id),
        .comp_r_data   (fragment_r_chan.data),
        .comp_r_resp   (fragment_r_chan.resp),
        .comp_r_last   (fragment_r_chan.last),

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
    // Test-only completion-AR ingress proxy.  AW/W/R/B are untouched.
    // ========================================================================

    assign fragment_ar_valid = u_comp_ar_valid;
    assign u_comp_ar_ready = fragment_ar_ready;
    assign fragment_ar_chan = u_comp_ar_chan;
    assign fragment_r_valid = slv_resps[2].r_valid;
    assign fragment_r_chan = slv_resps[2].r;
    assign fragment_r_ready = u_comp_r_ready;
    assign fragment_parents = i_dma.pre_parent_count;
    assign fragment_children = i_dma.pre_child_count;
    assign fragment_merged = i_dma.pre_merged;
    assign fragment_lasts = i_dma.pre_lasts;

    nonblock_read_admission #(
        .BankIdBit (SameBankDmasEnable ? 3 : 2),
        .IdWidth   (4),
        .AddrWidth (64),
        .LenWidth  (8),
        .ReadTagQueueEnable(SharedPoolMode >= 4),
        .ReadTagPerStreamEnable(HeaderParkMode != 0),
        .ar_chan_t (slv_ar_chan_t)
    ) i_read_admission (
        .clk_i,
        .rst_ni,
        .mode_i   (3'd0),
        .budget_i (read_budget_i),
        .period_i (read_period_i),

        .s_ar_valid (fragment_ar_valid),
        .s_ar_ready (fragment_ar_ready),
        .s_ar_chan  (fragment_ar_chan),
        .m_ar_valid (proxy_ar_valid),
        .m_ar_ready (slv_resps[2].ar_ready),
        .m_ar_chan  (proxy_ar_chan),

        .ptw_active_i   (dbg_tl_ptw_active),
        .ptw_r_fire_i   (dbg_tl_ptw_r_fire),
        .main_bank_i    (dbg_main_bank),
        .fast_resp_valid_i (dbg_fast_resp_valid),
        .fast_resp_ready_i (dbg_fast_resp_ready),
        .fast_resp_bank_i  (dbg_fast_resp_bank),
        .bank_read_stream_i ({
            i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.ar_data.id[2],
            i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.ar_data.id[2]
        }),
        .bank_is_read_i ({
            i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.is_read,
            i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.is_read
        }),
        .bank_trans_resp_valid_i (i_dma.i_sys.gen_banked.i_banked.bank_trans_resp_valid),
        .bank_trans_resp_ready_i (i_dma.i_sys.gen_banked.i_banked.bank_trans_resp_ready),
        .bank_trans_resp_error_i ({
            i_dma.i_sys.gen_banked.i_banked.bank_trans_resp_data[1].error,
            i_dma.i_sys.gen_banked.i_banked.bank_trans_resp_data[0].error}),

        .ddr_ar_valid_i (ddr_ar_valid),
        .ddr_ar_ready_i (ddr_ar_ready),
        .ddr_ar_id_i    (ddr_ar_id),

        .aw_pending_i   (comp_aw_pending_q),
        .comp_aw_valid_i (u_comp_aw_valid),
        .comp_aw_bank_i  (comp_aw_bank),
        .comp_r_fire_i   (slv_resps[2].r_valid && u_comp_r_ready),

        .dbg_new_captured (dbg_ra_new_captured),
        .dbg_first_offer (dbg_ra_first_offer),
        .dbg_ar_fire (dbg_ra_ar_fire),
        .dbg_gate_ingress (dbg_ra_gate_ingress),
        .dbg_gate_budget (dbg_ra_gate_budget),
        .dbg_exempt (dbg_ra_exempt),
        .dbg_oversize_escape (dbg_ra_oversize_escape),
        .dbg_fastgated (dbg_ra_fastgated),
        .dbg_phase_reset (dbg_ra_phase_reset),
        .dbg_total_debt (dbg_ra_total_debt),
        .dbg_buffer_empty (dbg_ra_buffer_empty),
        .dbg_capture_count (dbg_ra_capture_count),
        .dbg_offer_handshake_count (dbg_ra_offer_handshake_count),
        .dbg_capture_fast (dbg_ra_capture_fast),
        .dbg_capture_bank (dbg_ra_capture_bank),
        .dbg_capture_id (dbg_ra_capture_id),
        .dbg_capture_addr (dbg_ra_capture_addr),
        .dbg_capture_len (dbg_ra_capture_len),
        .dbg_tag_fault(dbg_ra_tag_fault), .dbg_tag_queued(dbg_ra_tag_queued)
    );

    // AW/B exemption tracking: physical comp AW/B handshakes, per bank.
    assign comp_aw_fire = u_comp_aw_valid && slv_resps[2].aw_ready;
    assign comp_b_fire  = slv_resps[2].b_valid && u_comp_b_ready;
    assign comp_aw_bank = u_comp_aw_id[SameBankDmasEnable ? 3 : 2];
    assign comp_b_bank  = slv_resps[2].b.id[SameBankDmasEnable ? 3 : 2];

    always_ff @(posedge clk_i or negedge rst_ni) begin : comp_aw_pending_fsm
        if (!rst_ni) begin
            comp_aw_pending_q <= '0;
        end else begin
            if (comp_aw_fire && (comp_aw_bank == 1'b0)) begin
                if (comp_b_fire && (comp_b_bank == 1'b0))
                    comp_aw_pending_q[0] <= comp_aw_pending_q[0];
                else
                    comp_aw_pending_q[0] <= comp_aw_pending_q[0] + 32'd1;
            end else if (comp_b_fire && (comp_b_bank == 1'b0)) begin
                if (comp_aw_pending_q[0] != 32'd0)
                    comp_aw_pending_q[0] <= comp_aw_pending_q[0] - 32'd1;
            end

            if (comp_aw_fire && (comp_aw_bank == 1'b1)) begin
                if (comp_b_fire && (comp_b_bank == 1'b1))
                    comp_aw_pending_q[1] <= comp_aw_pending_q[1];
                else
                    comp_aw_pending_q[1] <= comp_aw_pending_q[1] + 32'd1;
            end else if (comp_b_fire && (comp_b_bank == 1'b1)) begin
                if (comp_aw_pending_q[1] != 32'd0)
                    comp_aw_pending_q[1] <= comp_aw_pending_q[1] - 32'd1;
            end
        end
    end

    // ========================================================================
    // Test-only observation taps
    // ========================================================================
    assign dbg_comp_ar_fire = u_comp_ar_valid & u_comp_ar_ready;
    assign dbg_comp_aw_fire = u_comp_aw_valid & slv_resps[2].aw_ready;
    assign dbg_comp_w_fire  = u_comp_w_valid  & slv_resps[2].w_ready;
    assign dbg_ddtw_ar_fire = u_ddtw_ar_valid & ddtw_resp.ar_ready;
    assign dbg_ptw_ar_fire  = u_ptw_ar_valid  & ptw_resp.ar_ready;

    assign dbg_ra_up_valid   = u_comp_ar_valid;
    assign dbg_ra_up_ready   = u_comp_ar_ready;
    assign dbg_ra_up_id      = u_comp_ar_id;
    assign dbg_ra_up_addr    = u_comp_ar_addr;
    assign dbg_ra_up_len     = u_comp_ar_len;
    assign dbg_ra_up_bank    = u_comp_ar_id[SameBankDmasEnable ? 3 : 2];
    assign dbg_ra_up_attrs   = {u_comp_ar_size, u_comp_ar_burst, u_comp_ar_prot, u_comp_ar_user};

    assign dbg_ra_down_valid = proxy_ar_valid;
    assign dbg_frontier_grant = fragment_parents;
    assign dbg_frontier_wait = fragment_children;
    assign dbg_ra_down_ready = slv_resps[2].ar_ready;
    assign dbg_ra_down_id    = proxy_ar_chan.id;
    assign dbg_ra_down_addr  = proxy_ar_chan.addr;
    assign dbg_ra_down_len   = proxy_ar_chan.len;
    assign dbg_ra_down_attrs = {proxy_ar_chan.size, proxy_ar_chan.burst, proxy_ar_chan.prot, proxy_ar_chan.user};

    assign dbg_tr_aw_stall = i_dma.tr_aw_valid && !i_dma.tr_aw_ready;
    assign dbg_tr_aw_bank = i_dma.tr_aw_id[2];
    assign dbg_iotlb_fill = i_dma.i_sys.i_tl.iotlb_update.update;
    assign dbg_main_bank = i_dma.i_sys.i_tl.req_data.bank;
    assign dbg_policy_elaborated = PolicyEnable;
    `define PN_DEMUX i_dma.i_sys.gen_banked.i_banked.i_in_axi_demux.i_demux_simple.genblk1
    assign dbg_aw_wait_flags[0] = dbg_tr_aw_stall && !`PN_DEMUX.lock_aw_valid_q &&
        (`PN_DEMUX.w_open != '0) &&
        (`PN_DEMUX.w_select != i_dma.i_sys.gen_banked.i_banked.aw_sel);
    assign dbg_aw_wait_flags[1] = dbg_tr_aw_stall && !`PN_DEMUX.lock_aw_valid_q &&
        (`PN_DEMUX.aw_id_cnt_full || (&`PN_DEMUX.w_open) ||
         (`PN_DEMUX.ar_id_cnt_full && i_dma.i_sys.tr_req.aw.atop[axi_pkg::ATOP_R_RESP]));
    assign dbg_aw_wait_flags[2] = dbg_tr_aw_stall && !`PN_DEMUX.lock_aw_valid_q &&
        `PN_DEMUX.aw_select_occupied &&
        (`PN_DEMUX.lookup_aw_select != i_dma.i_sys.gen_banked.i_banked.aw_sel);
    assign dbg_aw_wait_flags[3] = dbg_tr_aw_stall &&
        `PN_DEMUX.aw_valid && !`PN_DEMUX.aw_ready;
    `undef PN_DEMUX

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
    assign dbg_cpu_quiet     = ~cpu_ar_valid_q & ~cpu_outstanding_q & ~cpu_run &
                               (cw_state_q == CW_IDLE);

    assign dbg_cpu_wr_issued     = cpu_wr_issued_q;
    assign dbg_cpu_wr_completed  = cpu_wr_completed_q;
    assign dbg_cpu_amo_issued    = cpu_amo_issued_q;
    assign dbg_cpu_amo_completed = cpu_amo_completed_q;
    assign dbg_cpu_rd_err        = cpu_rd_err_q;
    assign dbg_cpu_wr_err        = cpu_wr_err_q;
    assign dbg_cpu_amo_err       = cpu_amo_err_q;
    assign dbg_cpu_wr_quiet      = (cw_state_q == CW_IDLE);
    assign dbg_cpu_amo_r_fire    = cpu_amo_r_fire_q;
    assign dbg_cpu_amo_r_data    = cpu_amo_r_data_q;

    // ========================================================================
    // Test-only natural write-order taps.  Pure combinatorial reads of the
    // inner harness translation-request nets (same access class as the
    // existing dbg_tr_aw_stall / dbg_iotlb_fill taps above); they drive
    // nothing and add no state.
    // ========================================================================
    assign nat_vaw_valid = i_dma.tr_aw_valid;
    assign nat_vaw_ready = i_dma.tr_aw_ready;
    assign nat_vaw_id    = i_dma.tr_aw_id;
    assign nat_vaw_addr  = i_dma.tr_aw_addr;
    assign nat_vaw_len   = i_dma.tr_aw_len;
    assign nat_var_valid = i_dma.tr_ar_valid;
    assign nat_var_ready = i_dma.tr_ar_ready;
    assign nat_var_id    = i_dma.tr_ar_id;
    assign nat_var_addr  = i_dma.tr_ar_addr;
    assign nat_var_len   = i_dma.tr_ar_len;
    assign nat_vw_fire   = i_dma.tr_w_valid && i_dma.tr_w_ready;
    assign nat_vw_last   = i_dma.tr_w_last;
    // Codex: expose existing bank counters for the independently recorded C0
    // hypothesis. Observation only; no admission or counter state is changed.
    assign nat_w_count0 = 32'(i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.w_cnt_q);
    assign nat_w_count1 = 32'(i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.w_cnt_q);
    assign nat_w_overflow = {
        i_dma.i_sys.gen_banked.i_banked.gen_bank[1].i_bank.w_cnt_ovf,
        i_dma.i_sys.gen_banked.i_banked.gen_bank[0].i_bank.w_cnt_ovf
    };

endmodule
