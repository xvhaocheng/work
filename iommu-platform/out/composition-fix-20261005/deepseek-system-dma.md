# Findings summary

## What I verified in the supplied sources

| Fact | Evidence |
|---|---|
| The two-DMA/TL integration harness is `tests/nonblock_actual_dma_top.sv`; it exposes **scalar** `comp_*`, `ddtw_*`, `ptw_*`, `cfg_*` plus 10 TL taps and 9 DMA taps | `tests/nonblock_actual_dma_top.sv:48-184` |
| The production DS merge is `rv_iommu_ds_if` (AR = 7-input `stream_arbiter`, R = `stream_demux` selected by RID: **0=PTW, 1=DDTW**, 2=PDTW, 3=CQ, 4=MSIPTW, 5=MRIF, 6=PQ) | `cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:81-93,226-263` (`{pq, mrif, msiptw, cq, pdtw, ddtw, ptw}` LSB-first ⇒ index 0 = `ptw`, 1 = `ddtw`) |
| Board DS delayer constants are `FixedDelayInput/Output = 50` | `cva6/corev_apu/fpga/src/ariane_xilinx.sv:165-166`, wired at `ariane_peripherals_xilinx.sv:1358-1380` |
| Board xbar is `axi_xbar_intf`, `NoSlvPorts=NrSlaves(4)`, 11 master ports, `MaxMstTrans/MaxSlvTrans=8`, `CUT_ALL_PORTS`, `PipelineStages=1`, `UniqueIds=0`, `FallThrough=0`; slave id 4b, master id 6b | `ariane_xilinx.sv:291-320` (master id width `AxiIdWidthSlaves = AxiIdWidthMaster + $clog2(NBSlave) = 4+2`) |
| `axi_mux` prepends `$clog2(NoSlvPorts)` bits and routes R by the same bits | `axi_mux.sv:67-68,135-183,372-373` |
| `axi_xbar` has a degenerate single-master path only at `NoMstPorts==1`; 2 ports keeps the full demux/multicut/err-slv structure | `axi_xbar_unmuxed.sv:106-229,232-271` |
| Runner adds `axi_atop_filter.sv`, `onehot_to_bin.sv` already (needed by the xbar/`ATOPs=1` and reg path) | `tests/test_nonblock_actual_dma.py:123-138` |
| `axi_delayer` uses `stream_delay`; `axi_xbar*` use `addr_decode`+`axi_multicut`+`axi_cut` — these modules are **not** in any current source list | `tests/test_nonblock_banked.py:21-65`, `tests/test_nonblock_actual_dma.py:91-166` |

## Design I implemented (proposals only — worker cannot compile)

* New top `tests/nonblock_actual_dma_system_top.sv` re-instantiates the **unchanged** `nonblock_actual_dma_top` and adds a test-only board-shaped fabric:
  `comp (slv3)`, real `rv_iommu_ds_if` (slv2) with live DDTW/PTW and 5 sources tied to 0, real `axi_delayer` 50/50, real `axi_xbar` 4→2, `mst0` = shared 6-bit-ID DDR, `mst1` = `axi_err_slv` dummy. CPU synthetic master on slv0 (id 0), debug idle slv1 tied to 0.
* CPU master: 64B (`len=7`), read-only, `addr=0x80000000`, single outstanding, interval 32, RREADY 16-ready/4-stall, never withdraws a stalled AR; gated by `dbg_dma_batch_active[0] | dbg_dma_batch_gap[0]`, exposed as `dbg_cpu_issued/completed/quiet/active`.
* C++ / runner: `#ifdef ACTUAL_DMA_SYSTEM` picks `Vnonblock_actual_dma_system_top.h`, one ordered shared DDR R queue (echo RID, burst by `beat/len`), one AW/W/B model, per-source attribution from the top 2 bits of the 6-bit ID; `--system` flag, `-DACTUAL_DMA_SYSTEM`, separate obj dir.

**Unproven / for Codex:** I cannot compile. The `unique case` in `rv_iommu_ds_if.sv:154-161` has a duplicated `3:` label (pre-existing) — Verilator emits a warning only; `-Wno-fatal` already present. Whether PTW always emits ≥1 accepted AR in every scenario is asserted by the guards but not proven by me.

---

# 1. New file — `tests/nonblock_actual_dma_system_top.sv`

```systemverilog
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
```

---

# 2. Edits to `tests/nonblock_actual_dma_test.cpp`

All edits are given as **explicit replacements** of the original line ranges in the context you supplied.

### R1 — replace lines **34–35** with

```cpp
#ifdef ACTUAL_DMA_SYSTEM
#include "Vnonblock_actual_dma_system_top.h"
using TopT = Vnonblock_actual_dma_system_top;
#else
#include "Vnonblock_actual_dma_top.h"
using TopT = Vnonblock_actual_dma_top;
#endif
#include "verilated.h"
```

### R2 — replace line **116**

```cpp
    Vnonblock_actual_dma_top* top;
```
with
```cpp
    TopT* top;
```

### R3 — replace line **121**

```cpp
    enum Port { P_COMP = 0, P_DDTW = 1, P_PTW = 2 };
```
with
```cpp
    enum Port { P_COMP = 0, P_DDTW = 1, P_PTW = 2, P_DDR = 3 };
```

### R4 — insert **after line 155** (`long v_watchdog_cycles = 200000L;`)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
    // Synthetic-CPU independent progress baseline (refreshed only by the CPU
    // lifecycle counters, never by DMA handshakes).
    long     cpu_progress_cycle  = 0;
    long     cpu_watchdog_cycles = 200000L;
    uint32_t last_cpu_issued     = 0;
    uint32_t last_cpu_completed  = 0;
#endif
```

### R5 — replace line **182**

```cpp
    Tb() : top(new Vnonblock_actual_dma_top) {}
```
with
```cpp
    Tb() : top(new TopT) {}
```

### R6 — wrap lines **264–311** (the existing `drive_comp()` + `drive_mem_r()`) in `#ifndef ACTUAL_DMA_SYSTEM`, and append a system-mode driver

Replace lines **264–311** with:

```cpp
#ifndef ACTUAL_DMA_SYSTEM
    void drive_comp() {
        top->comp_aw_ready = 1;
        top->comp_ar_ready = (mem_q.size() < 16) ? 1 : 0;
        top->comp_w_ready  = 1;
        if (!cb_q.empty()) {
            top->comp_b_valid = 1;
            top->comp_b_id    = cb_q.front();
            top->comp_b_resp  = 0;
        } else {
            top->comp_b_valid = 0;
            top->comp_b_id    = 0;
            top->comp_b_resp  = 0;
        }
    }

    void drive_mem_r() {
        top->comp_r_valid = 0;
        top->ddtw_r_valid = 0;
        top->ptw_r_valid  = 0;

        if (mem_q.empty()) return;
        MemRead& h = mem_q.front();
        if (h.latency > 0) { h.latency--; return; }

        switch (h.port) {
        case P_COMP:
            top->comp_r_valid = 1;
            top->comp_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->comp_r_id    = h.id;
            top->comp_r_resp  = 0;
            top->comp_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
        case P_DDTW:
            top->ddtw_r_valid = 1;
            top->ddtw_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->ddtw_r_id    = h.id;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
        case P_PTW:
            top->ptw_r_valid = 1;
            top->ptw_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
            top->ptw_r_id    = h.id;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = (h.beat == h.len) ? 1 : 0;
            break;
        default: break;
        }
    }
#else
    // ---- SYSTEM mode: ONE shared DDR port (comp + DS merged by the xbar) ----
    // The plain DDR model latency applies uniformly to every accepted DDR read;
    // the real 50/50 DS delayer adds latency on the DS path in RTL only.
    void drive_ddr() {
        top->ddr_aw_ready = 1;
        top->ddr_w_ready  = 1;
        top->ddr_ar_ready = (mem_q.size() < 16) ? 1 : 0;
        if (!cb_q.empty()) {
            top->ddr_b_valid = 1;
            top->ddr_b_id    = (uint8_t)cb_q.front();
            top->ddr_b_resp  = 0;
        } else {
            top->ddr_b_valid = 0;
            top->ddr_b_id    = 0;
            top->ddr_b_resp  = 0;
        }

        top->ddr_r_valid = 0;
        if (mem_q.empty()) return;
        MemRead& h = mem_q.front();
        if (h.latency > 0) { h.latency--; return; }

        top->ddr_r_valid = 1;
        top->ddr_r_data  = read64(h.addr + (uint64_t)h.beat * 8);
        top->ddr_r_id    = (uint8_t)h.id;   // 6-bit shared-DDR ID echoed verbatim
        top->ddr_r_resp  = 0;
        top->ddr_r_last  = (h.beat == h.len) ? 1 : 0;
    }
#endif
```

### R7 — add system-mode counters

Insert **after line 169** (`uint64_t n_ptw_active = 0, n_iotlb_lookup = 0, n_iotlb_hit = 0;`):

```cpp
#ifdef ACTUAL_DMA_SYSTEM
    uint64_t n_ddr_ar = 0, n_ddr_aw = 0, n_ddr_b = 0, n_ddr_r = 0;
    uint64_t n_slv_ar[4] = {0, 0, 0, 0};   // by top 2 bits of the 6-bit DDR ID
    uint64_t n_walk_ddtw = 0, n_walk_ptw = 0;
#endif
```

### R8 — replace the whole `tick()` body, lines **316–475**

```cpp
    void tick() {
        top->nonblock_enable_i  = nonblock_enable  ? 1 : 0;
        top->policy_partition_i = policy_partition ? 1 : 0;

        top->cfg_sel   = cfg_sel   ? 1 : 0;
        top->cfg_valid = cfg_valid ? 1 : 0;
        top->cfg_write = cfg_write ? 1 : 0;
        top->cfg_addr  = cfg_addr;
        top->cfg_wdata = cfg_wdata;
        top->cfg_wstrb = cfg_wstrb;

        top->clk_i = 0;
#ifdef ACTUAL_DMA_SYSTEM
        drive_ddr();
#else
        drive_comp();
        drive_mem_r();
        top->ddtw_ar_ready = 1;
        top->ptw_ar_ready  = 1;
#endif
        top->eval();

        // ================= pre-edge snapshot ==============================
        bool any_fire = false;

#ifdef ACTUAL_DMA_SYSTEM
        const bool     f_ddr_ar   = top->ddr_ar_valid && top->ddr_ar_ready;
        const bool     f_ddr_aw   = top->ddr_aw_valid && top->ddr_aw_ready;
        const bool     f_ddr_w    = top->ddr_w_valid  && top->ddr_w_ready;
        const bool     f_ddr_b    = top->ddr_b_valid  && top->ddr_b_ready;
        const bool     f_ddr_r    = top->ddr_r_valid  && top->ddr_r_ready;
        const uint64_t s_ddr_ar_a = top->ddr_ar_addr;
        const uint32_t s_ddr_ar_l = (uint32_t)top->ddr_ar_len;
        const uint32_t s_ddr_ar_i = (uint32_t)top->ddr_ar_id;
        const uint64_t s_ddr_aw_a = top->ddr_aw_addr;
        const uint32_t s_ddr_aw_l = (uint32_t)top->ddr_aw_len;
        const uint32_t s_ddr_aw_i = (uint32_t)top->ddr_aw_id;
        const uint64_t s_ddr_w_d  = top->ddr_w_data;
        const uint8_t  s_ddr_w_s  = (uint8_t)top->ddr_w_strb;
        const bool     s_ddr_w_l  = top->ddr_w_last;
        any_fire = f_ddr_ar || f_ddr_aw || f_ddr_w || f_ddr_b || f_ddr_r;
#else
        const bool     s_comp_ar_v = top->comp_ar_valid;
        const bool     s_comp_ar_r = top->comp_ar_ready;
        const uint32_t s_comp_ar_i = top->comp_ar_id;
        const uint64_t s_comp_ar_a = top->comp_ar_addr;
        const uint32_t s_comp_ar_l = (uint32_t)top->comp_ar_len;

        const bool     s_ddtw_ar_v = top->ddtw_ar_valid;
        const bool     s_ddtw_ar_r = top->ddtw_ar_ready;
        const uint64_t s_ddtw_ar_a = top->ddtw_ar_addr;
        const uint32_t s_ddtw_ar_l = (uint32_t)top->ddtw_ar_len;
        const uint32_t s_ddtw_ar_i = (uint32_t)top->ddtw_ar_id;

        const bool     s_ptw_ar_v = top->ptw_ar_valid;
        const bool     s_ptw_ar_r = top->ptw_ar_ready;
        const uint64_t s_ptw_ar_a = top->ptw_ar_addr;
        const uint32_t s_ptw_ar_l = (uint32_t)top->ptw_ar_len;
        const uint32_t s_ptw_ar_i = (uint32_t)top->ptw_ar_id;

        const bool     s_comp_aw_v = top->comp_aw_valid;
        const bool     s_comp_aw_r = top->comp_aw_ready;
        const uint32_t s_comp_aw_i = top->comp_aw_id;
        const uint64_t s_comp_aw_a = top->comp_aw_addr;
        const uint32_t s_comp_aw_l = (uint32_t)top->comp_aw_len;

        const bool     s_comp_w_v = top->comp_w_valid;
        const bool     s_comp_w_r = top->comp_w_ready;
        const uint64_t s_comp_w_d = top->comp_w_data;
        const uint8_t  s_comp_w_s = (uint8_t)top->comp_w_strb;
        const bool     s_comp_w_l = top->comp_w_last;

        const bool     s_comp_b_v = top->comp_b_valid;
        const bool     s_comp_b_r = top->comp_b_ready;

        const bool     s_comp_r_v = top->comp_r_valid;
        const bool     s_comp_r_r = top->comp_r_ready;
        const bool     s_ddtw_r_v = top->ddtw_r_valid;
        const bool     s_ddtw_r_r = top->ddtw_r_ready;
        const bool     s_ptw_r_v  = top->ptw_r_valid;
        const bool     s_ptw_r_r  = top->ptw_r_ready;

        const bool f_comp_ar = s_comp_ar_v && s_comp_ar_r;
        const bool f_ddtw_ar = s_ddtw_ar_v && s_ddtw_ar_r;
        const bool f_ptw_ar  = s_ptw_ar_v  && s_ptw_ar_r;
        const bool f_comp_aw = s_comp_aw_v && s_comp_aw_r;
        const bool f_comp_w  = s_comp_w_v  && s_comp_w_r;
        const bool f_comp_b  = s_comp_b_v  && s_comp_b_r;
        const bool f_comp_r  = s_comp_r_v  && s_comp_r_r;
        const bool f_ddtw_r  = s_ddtw_r_v  && s_ddtw_r_r;
        const bool f_ptw_r   = s_ptw_r_v   && s_ptw_r_r;

        any_fire = f_comp_ar || f_ddtw_ar || f_ptw_ar || f_comp_aw || f_comp_w ||
                   f_comp_b  || f_comp_r  || f_ddtw_r  || f_ptw_r;
#endif

        if (any_fire) last_progress_cycle = g_cycle;

        // V (DMA0 = bank0) independent completion progress.
        if (top->dbg_dma_done & 1) v_progress_cycle = g_cycle;

        if (top->dbg_fast_resp_valid && top->dbg_fast_resp_ready) {
            ++n_fast;
            ++n_fast_bank[top->dbg_fast_resp_bank & 1];
        }
        if (top->dbg_tl_req_valid && top->dbg_tl_req_ready) {
            ++n_tl_accept;
            ++n_req_bank[top->dbg_tl_req_bank & 1];
        }
        if (top->dbg_tl_resp_valid && top->dbg_tl_resp_ready) {
            ++n_tl_resp;
            ++n_resp_bank[top->dbg_tl_resp_bank & 1];
        }
        if (top->dbg_tl_ptw_active) ++n_ptw_active;
        if (top->dbg_tl_iotlb_lookup) {
            ++n_iotlb_lookup;
            if (top->dbg_tl_iotlb_hit) ++n_iotlb_hit;
        }

#ifdef ACTUAL_DMA_SYSTEM
        // Underlying walker counters: the real ds_if merge hides the external
        // dd/ptw ports, so walk evidence comes from the harness taps.
        if (top->dbg_ddtw_ar_fire) ++n_walk_ddtw;
        if (top->dbg_ptw_ar_fire)  ++n_walk_ptw;
        // Synthetic-CPU independent progress.
        if (top->dbg_cpu_issued != last_cpu_issued ||
            top->dbg_cpu_completed != last_cpu_completed) {
            cpu_progress_cycle = g_cycle;
            last_cpu_issued    = top->dbg_cpu_issued;
            last_cpu_completed = top->dbg_cpu_completed;
        }
#endif

        // ================= rising edge ====================================
        top->clk_i = 1;
        top->eval();

#ifdef ACTUAL_DMA_SYSTEM
        if (f_ddr_ar) {
            ++n_ddr_ar;
            ++n_slv_ar[(s_ddr_ar_i >> 4) & 3];
            MemRead r; r.port = P_DDR; r.addr = s_ddr_ar_a;
            r.len = (uint8_t)s_ddr_ar_l; r.id = (uint8_t)s_ddr_ar_i;
            r.beat = 0; r.latency = (int)ptw_latency;
            mem_q.push_back(r);
        }
        if (f_ddr_aw) {
            ++n_ddr_aw;
            CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;
            c.len = (uint8_t)s_ddr_aw_l; c.got = 0;
            caw_q.push_back(c);
        }
        if (f_ddr_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, s_ddr_w_d, s_ddr_w_s);
            c.got++;
            if (s_ddr_w_l) {
                CHECK(c.got == (uint32_t)c.len + 1, "completion W burst length mismatch");
                cb_q.push_back(c.id);
                caw_q.pop_front();
            }
        }
        if (f_ddr_b) {
            ++n_ddr_b;
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            cb_q.pop_front();
        }
        if (f_ddr_r) {
            ++n_ddr_r;
            MemRead& h = mem_q.front();
            CHECK(h.port == P_DDR, "R handshake on an unexpected port");
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }
#else
        if (f_comp_ar) {
            ++n_comp_ar;
            MemRead r; r.port = P_COMP; r.addr = s_comp_ar_a;
            r.len = (uint8_t)s_comp_ar_l; r.id = (uint8_t)s_comp_ar_i;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ddtw_ar) {
            ++n_ddtw_ar;
            MemRead r; r.port = P_DDTW; r.addr = s_ddtw_ar_a;
            r.len = (uint8_t)s_ddtw_ar_l; r.id = (uint8_t)s_ddtw_ar_i;
            r.beat = 0; r.latency = 1;
            mem_q.push_back(r);
        }
        if (f_ptw_ar) {
            ++n_ptw_ar;
            MemRead r; r.port = P_PTW; r.addr = s_ptw_ar_a;
            r.len = (uint8_t)s_ptw_ar_l; r.id = (uint8_t)s_ptw_ar_i;
            r.beat = 0; r.latency = (int)ptw_latency;
            mem_q.push_back(r);
        }
        if (f_comp_aw) {
            CawEnt c; c.id = s_comp_aw_i; c.addr = s_comp_aw_a;
            c.len = (uint8_t)s_comp_aw_l; c.got = 0;
            caw_q.push_back(c);
        }
        if (f_comp_w) {
            CHECK(!caw_q.empty(), "completion W without a pending completion AW");
            CawEnt& c = caw_q.front();
            CHECK(c.got < (uint32_t)c.len + 1, "too many completion W beats");
            write_beat(c.addr + (uint64_t)c.got * 8, s_comp_w_d, s_comp_w_s);
            c.got++;
            if (s_comp_w_l) {
                CHECK(c.got == (uint32_t)c.len + 1, "completion W burst length mismatch");
                cb_q.push_back(c.id);
                caw_q.pop_front();
            }
        }
        if (f_comp_b) {
            CHECK(!cb_q.empty(), "completion B handshake with empty cb_q");
            cb_q.pop_front();
        }
        if (f_comp_r || f_ddtw_r || f_ptw_r) {
            MemRead& h = mem_q.front();
            CHECK(h.port == (f_comp_r ? P_COMP : (f_ddtw_r ? P_DDTW : P_PTW)),
                  "R handshake on an unexpected port");
            if (h.beat == h.len) mem_q.pop_front();
            else                 h.beat++;
        }
#endif

        top->clk_i = 0;
        top->eval();
        ++g_cycle;
    }
```

### R9 — in `reset()`, after line **569** (`v_progress_cycle = g_cycle;`)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
        n_ddr_ar = n_ddr_aw = n_ddr_b = n_ddr_r = 0;
        n_slv_ar[0] = n_slv_ar[1] = n_slv_ar[2] = n_slv_ar[3] = 0;
        n_walk_ddtw = n_walk_ptw = 0;
        cpu_progress_cycle = g_cycle;
        last_cpu_issued    = 0;
        last_cpu_completed = 0;
#endif
```

### R10 — replace the `check_quiet()` quiet predicate, lines **586–588**

```cpp
            if (top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3) {
                quiet = true;
                break;
            }
```
with
```cpp
#ifdef ACTUAL_DMA_SYSTEM
            const bool cpu_q = top->dbg_cpu_quiet;
#else
            const bool cpu_q = true;
#endif
            if (top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3 && cpu_q) {
                quiet = true;
                break;
            }
```

### R11 — in `dump_stall()`, insert **after line 668** (end of the DMA1 fprintf)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
        std::fprintf(stderr,
            "  SYS cpu issued=%u completed=%u quiet=%d active=%d\n",
            (unsigned)top->dbg_cpu_issued, (unsigned)top->dbg_cpu_completed,
            (int)top->dbg_cpu_quiet, (int)top->dbg_cpu_active);
        std::fprintf(stderr,
            "  SYS ddr ar_v=%d ar_r=%d r_v=%d r_r=%d aw_v=%d aw_r=%d b_v=%d b_r=%d "
            "ds ar_v=%d ar_r=%d r_v=%d r_r=%d comp ar_f=%d aw_f=%d w_f=%d "
            "ddtw_f=%d ptw_f=%d\n",
            (int)top->dbg_ddr_ar_valid, (int)top->dbg_ddr_ar_ready,
            (int)top->dbg_ddr_r_valid,  (int)top->dbg_ddr_r_ready,
            (int)top->dbg_ddr_aw_valid, (int)top->dbg_ddr_aw_ready,
            (int)top->dbg_ddr_b_valid,  (int)top->dbg_ddr_b_ready,
            (int)top->dbg_ds_ar_valid,  (int)top->dbg_ds_ar_ready,
            (int)top->dbg_ds_r_valid,   (int)top->dbg_ds_r_ready,
            (int)top->dbg_comp_ar_fire, (int)top->dbg_comp_aw_fire,
            (int)top->dbg_comp_w_fire,  (int)top->dbg_ddtw_ar_fire,
            (int)top->dbg_ptw_ar_fire);
        std::fprintf(stderr,
            "  SYS ddr_ar=%lu ddr_aw=%lu ddr_r=%lu  slv_ar cpu=%lu dbg=%lu ds=%lu comp=%lu\n",
            (unsigned long)n_ddr_ar, (unsigned long)n_ddr_aw, (unsigned long)n_ddr_r,
            (unsigned long)n_slv_ar[0], (unsigned long)n_slv_ar[1],
            (unsigned long)n_slv_ar[2], (unsigned long)n_slv_ar[3]);
        if (!mem_q.empty()) {
            const MemRead& h = mem_q.front();
            std::fprintf(stderr,
                "  SYS ddrR.head slv=%u id=0x%02x addr=0x%016lx len=%u beat=%u lat=%d\n",
                (unsigned)((h.id >> 4) & 3), (unsigned)h.id,
                (unsigned long)h.addr, (unsigned)h.len, h.beat, h.latency);
        }
#endif
```

### R12 — in the V-completion loop of `run_scenario()`, insert after line **818** (`}` closing the V-progress check)

```cpp
#ifdef ACTUAL_DMA_SYSTEM
                if ((top->dbg_dma_batch_active & 1) &&
                    (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
                    dump_stall("scenario CPU progress");
                    throw TestFailure("watchdog: synthetic CPU made no progress");
                }
#endif
```

### R13 — replace the mechanism-guard block, lines **872–881**

```cpp
        // ---- 7. mechanism guards ---------------------------------------
        CHECK(n_comp_ar > 0, "no completion reads observed");
        CHECK(n_r_beats + n_comp_ar > 0, "no R traffic observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");
        // With nonblocking disabled the fast path must never fire.
        if (!enable) {
            CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                  "nonblocking fast path fired while nonblocking is disabled");
        }
```
with
```cpp
        // ---- 7. mechanism guards ---------------------------------------
#ifdef ACTUAL_DMA_SYSTEM
        CHECK(n_ddr_ar > 0, "no shared-DDR reads observed");
        CHECK(n_ddr_aw > 0, "no shared-DDR writes observed");
        CHECK(n_ddr_r  > 0, "no shared-DDR R beats observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        // Walker evidence comes from the underlying harness taps, not from the
        // (merged-away) external dd/ptw ports.
        CHECK((n_walk_ddtw + n_walk_ptw) > 0, "no walker reads observed");
        // Synthetic CPU must actually run and drain (no false PASS by skipping
        // the new branch).
        CHECK(top->dbg_cpu_completed > 0, "synthetic CPU completed no reads");
        CHECK(top->dbg_cpu_quiet, "synthetic CPU not quiet at terminal");
        // The idle debug port must never produce shared-DDR traffic.
        CHECK(n_slv_ar[1] == 0, "idle debug port produced shared-DDR reads");
        if (!enable) {
            CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                  "nonblocking fast path fired while nonblocking is disabled");
        }
        std::printf("SYSTEM walk ddtw=%lu ptw=%lu ddr_ar=%lu ddr_aw=%lu ddr_r=%lu "
                    "slv_ar cpu=%lu ds=%lu comp=%lu cpu_issued=%u cpu_completed=%u\n",
                    (unsigned long)n_walk_ddtw, (unsigned long)n_walk_ptw,
                    (unsigned long)n_ddr_ar, (unsigned long)n_ddr_aw,
                    (unsigned long)n_ddr_r, (unsigned long)n_slv_ar[0],
                    (unsigned long)n_slv_ar[2], (unsigned long)n_slv_ar[3],
                    (unsigned)top->dbg_cpu_issued, (unsigned)top->dbg_cpu_completed);
#else
        CHECK(n_comp_ar > 0, "no completion reads observed");
        CHECK(n_r_beats + n_comp_ar > 0, "no R traffic observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");
        if (!enable) {
            CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                  "nonblocking fast path fired while nonblocking is disabled");
        }
#endif
```

`main()` needs no modification: both builds accept the same `+key=value` plusargs. (The `--system` switch is a **build/runner** concern.)

---

# 3. Edits to `tests/test_nonblock_actual_dma.py`

### S1 — add `system_sources()` after `production_fifo_sources()` (insert after line **166**)

```python
def system_sources():
    """Extra production sources required by the SYSTEM-mode top:
    rv_iommu_ds_if + axi_delayer/stream_delay + axi_xbar family
    (axi_xbar -> axi_xbar_unmuxed -> axi_multicut -> axi_cut, addr_decode)
    and the stream_mux used by rv_iommu_ds_if.  axi_atop_filter /
    onehot_to_bin are already appended by idma_sources()."""
    axi = IOMMU / "vendor" / "axi" / "src"
    cc  = IOMMU / "vendor" / "common_cells" / "src"
    return [
        cc  / "stream_delay.sv",
        cc  / "addr_decode.sv",
        cc  / "stream_mux.sv",
        axi / "axi_cut.sv",
        axi / "axi_multicut.sv",
        axi / "axi_delayer.sv",
        axi / "axi_xbar_unmuxed.sv",
        axi / "axi_xbar.sv",
        IOMMU / "rtl" / "ext_interfaces" / "rv_iommu_ds_if.sv",
        ROOT / "tests" / "nonblock_actual_dma_system_top.sv",
    ]
```

### S2 — add the `--system` argument, after line **199**

```python
    ap.add_argument("--system", action="store_true",
                    help="build the shared-DDR system-integration top "
                         "(tests/nonblock_actual_dma_system_top.sv) with the "
                         "production rv_iommu_ds_if / axi_delayer(50/50) / "
                         "axi_xbar; compiles with -DACTUAL_DMA_SYSTEM and uses "
                         "the separate object directory out/nonblock-actual-dma/obj-system")
```

### S3 — replace lines **202–221** (source/objdir/args setup) with

```python
    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_actual_dma_test.cpp"

    if opts.system:
        top_name = "nonblock_actual_dma_system_top"
        obj_dir  = OUT / "obj-system"
        defines  = ["-DACTUAL_DMA_SYSTEM"]
        extra    = [require(p) for p in system_sources()]
    else:
        top_name = TOP
        obj_dir  = OUT / "obj"
        defines  = []
        extra    = [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")]

    sources = dedup(
        [require(ARIANE_PKG_SRC)]
        + [require(p) for p in sibling.SOURCES
           if p not in REPLACED_BY_PRODUCTION]
        + [require(p) for p in idma_sources()]
        + [require(p) for p in production_fifo_sources()]
        + extra
    )
    incdirs = dedup(list(sibling.INCDIRS) + idma_incdirs())

    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
    ] + defines + [
        "--top-module", top_name,
        "--Mdir", str(obj_dir),
    ]
```

### S4 — replace line **245** exe path

```python
    exe = obj_dir / "V{}".format(TOP)
```
with
```python
    exe = obj_dir / "V{}".format(top_name)
```

### S5 — replace the run-args/print block, lines **251–264**

```python
    run_args = [
        str(exe),
        "+mode={}".format(opts.mode),
        "+a-pages={}".format(opts.a_pages),
        "+v-gap={}".format(opts.v_gap),
        "+transfers={}".format(opts.transfers),
        "+warmup={}".format(opts.warmup),
        "+ptw-latency={}".format(opts.ptw_latency),
        "+a-target={}".format(opts.a_target),
    ]
    if opts.system:
        run_args += ["+system=1"]
    print("test_nonblock_actual_dma: system={} top={} mode={} a_pages={} v_gap={} "
          "transfers={} warmup={} ptw_latency={} a_target={}".format(
              int(opts.system), top_name, opts.mode, opts.a_pages, opts.v_gap,
              opts.transfers, opts.warmup, opts.ptw_latency, opts.a_target))
    run = subprocess.run(run_args)
    return run.returncode
```

The `+system=1` plusarg is informational only (compile-time `-DACTUAL_DMA_SYSTEM` is the real switch); Verilator's `commandArgs` ignores unknown plusargs, so the C++ needs no handler.

---

# 4. Source additions / dependencies (no duplicate definitions)

Added only in `--system` builds:

| File | Why | Already present? |
|---|---|---|
| `.../common_cells/src/stream_delay.sv` | `axi_delayer` | no |
| `.../common_cells/src/addr_decode.sv` | `axi_xbar_unmuxed` | no |
| `.../common_cells/src/stream_mux.sv` | `rv_iommu_ds_if` W channel | no |
| `.../axi/src/axi_cut.sv` | `axi_multicut` | no |
| `.../axi/src/axi_multicut.sv` | `axi_xbar_unmuxed` `PipelineStages` | no |
| `.../axi/src/axi_delayer.sv` | DS delay (50/50) | no |
| `.../axi/src/axi_xbar_unmuxed.sv` | `axi_xbar` | no |
| `.../axi/src/axi_xbar.sv` | system fabric | no |
| `.../riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv` | real DS merge | no |
| `tests/nonblock_actual_dma_system_top.sv` | new top | new |

`axi_err_slv`, `axi_demux`, `axi_demux_simple`, `axi_mux`, `axi_id_prepend`, `fifo_v3`, `stream_arbiter`, `rr_arb_tree`, `spill_register`, `axi_atop_filter`, `onehot_to_bin` are already in the existing list — **not** duplicated.

---

# 5. Assumptions, limitations, unresolved

**Assumptions**
1. The real `rv_iommu_ddtw` emits `ARID = 1` and the real `rv_iommu_ptw` emits `ARID = 0`, matching `rv_iommu_ds_if.sv:228-235` (`0 = PTW`, `1 = DDTW`). The wrapper does **not** rewrite IDs.
2. `nonblock_actual_dma_top.comp_*` / `ddtw_*` / `ptw_*` are the only external ports to absorb; `cfg_*`, the 10 TL taps and the 9 DMA taps are unchanged.
3. `--ptw-latency` is interpreted as the shared-DDR model read latency applied uniformly in system mode; the real DS delayer adds latency in RTL. **Flagged as an interpretation** — if the intended meaning is "DS reads only get this latency", change the `r.latency` assignment in `R8` to a per-source branch on `(id >> 4) == 2`.
4. `AXI_USER_WIDTH = 1`, `AxiIdWidthSlvPorts = 4`, master ID = 6. Matches the board (`ariane_peripherals_xilinx.sv:867`, `ariane_xilinx.sv:161`).
5. All test addresses < 4 GiB, so `addr_map[0]` (0..0x1_0000_0000 → DDR) covers everything; `mst1` is never hit.

**Limitations to be reported with every system-mode result** (also written into the SV header):
- no real CPU/caches/`axi_adapter`/dwidth-converters; the CPU master is a bounded read injector;
- only 2 of the 11 board master ports — no PLIC/CLINT/ROM/UART/SPI/GPIO/Ethernet/timer, no `dma_cfg axi_demux`;
- no MIG/DDR controller/clock-domain converter/512→64 width converter — DDR is a behavioural model;
- `cfg_*` still goes **directly** to the two `dma_core_wrap` slaves (bypasses the CPU crossbar and the production `dma_cfg axi_demux`);
- the xbar is fully connected (`Connectivity = '1`) whereas the board relies on the address rules.

**Unresolved / for Codex**
- Cannot compile. The `unique case` duplicate-`3:` label in `rv_iommu_ds_if.sv:158-159` is pre-existing and expects `-Wno-fatal`.
- Whether `n_walk_ptw > 0` in the `--warmup 256` cases: I kept the *combined* walker guard hard (as the original) and print `n_walk_ddtw`/`n_walk_ptw` individually, so a false FAIL is not introduced.
- Whether `axi_err_slv`'s parameter is `ATOPs` (typo for `ATOPs`) — I used the spelling seen in `ariane_peripherals_xilinx.sv:1325-1334` (`ATOPs` not passed there); the real module parameter is `ATOPs`. **Verify the exact casing** against `axi_err_slv.sv` before compiling.

**Suggested acceptance checks**
```
# S64 (must pass, both modes)
python3 tests/test_nonblock_actual_dma.py                          --mode s --transfers 64   --ptw-latency 31
python3 tests/test_nonblock_actual_dma.py --system                 --mode s --transfers 64   --ptw-latency 31
# N64
python3 tests/test_nonblock_actual_dma.py --system                 --mode n --transfers 64   --ptw-latency 31
# N4096 with the board DS delay in the loop
python3 tests/test_nonblock_actual_dma.py --system                 --mode n --transfers 4096 --ptw-latency 31 --warmup 256 --v-gap 1000
```
Checks: object dir `out/nonblock-actual-dma/obj-system`; log contains `SYSTEM walk ddtw=>0 ptw=>0`; `ddr_ar>0 && ddr_aw>0 && n_slv_ar[1]==0`; `cpu_completed>0`; terminal `PASS`; and on stall, the `SYS ddrR.head slv=… id=0x…` line names the blocking source.