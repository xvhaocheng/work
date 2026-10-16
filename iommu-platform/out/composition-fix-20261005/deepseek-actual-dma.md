# Findings summary

This is a large, architecture-heavy integration. I can deliver a **complete first draft of the three requested files** plus a precise dependency/limitation/acceptance list, but I **cannot compile, elaborate or run** anything here, and several integration facts (the exact `axi-iommu` EXT‑channel macro field set, the `axi_mux` version's ports, and the exact internal signal names of `rv_iommu_tl_wrap` / `rv_iommu_banked_ctl`) are **not present in the supplied context**. Those points are called out explicitly as `VERIFY‑P0` below; Codex must confirm them before trusting a green build.

Key facts I relied on, with citations:
- Joint harness instantiation surface (`tr_*`, `comp_*`, `ddtw_*`, `ptw_*`, `dbg_*`, `nonblock_enable_i`, `fullbuffer_jobs_i`, `policy_partition_i`): `tests/nonblock_banked_top.sv:35‑184, 589‑598`.
- `BankIdBit = 2` already inside the harness: `tests/nonblock_banked_top.sv:628`.
- Real wrapper parameter/port names: `cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:18‑64`, `:426‑465`.
- FPGA topology (`axi_mux` with 4 ports, `DmaAxiIdWidth = IdWidth−clog2(4) = 2`): `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:870‑873, 1225‑1250, 1253‑1306`.
- Certificate driving `fullbuffer_job_o`: `dma_core_wrap.sv:173‑184`; consumed at `ariane_peripherals_xilinx.sv:1463`.
- Frontend 64‑bit register map and batch semantics: `idma_reg64_frontend_reg_pkg.sv:110‑119`; batch/decode/status: `idma_reg64_frontend.sv:135‑217, 424‑446, 468‑617`.
- Existing memory/page‑table model to be reused: `tests/nonblock_coupled_test.cpp:360‑437` (memory), `:574‑621` (R return), `:626‑910` (tick/queues).
- Runner pattern to reuse: `tests/test_nonblock_banked.py:21‑70, 98‑137`; sibling loader `tests/test_nonblock_coupled.py:44‑51`.

The reminder `NOT verified / NOT run` applies to everything below.

---

BEGIN_FILE tests/nonblock_actual_dma_top.sv
```systemverilog
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
//   * fullbuffer_jobs_i of the harness is driven from the REAL certificates
//     (dma_core_wrap fullbuffer_job_o), never from a static 2'b11;
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
    parameter bit          PolicyEnable        = 0
) (
    input  logic clk_i,
    input  logic rst_ni,

    // ---- runtime mode inputs (forwarded to the harness) --------------------
    input  logic nonblock_enable_i,
    input  logic policy_partition_i,

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
    output logic [1:0]  dbg_dma_launch_error
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

    logic        fullbuffer_jobs_o;   // 2-bit harness permit, certificate-driven

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
        .BufferDepth      ( 512             ),
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
        .BufferDepth      ( 512             ),
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
    assign slv_reqs[0] = dma0_mst_req;
    assign slv_reqs[1] = dma1_mst_req;
    assign slv_reqs[2] = '0;
    assign slv_reqs[3] = '0;
    assign dma0_mst_rsp = slv_resps[0];
    assign dma1_mst_rsp = slv_resps[1];
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

        tr_ar_valid      = mux_mst_req.ar_valid;
        tr_ar_id         = mux_mst_req.ar.id;
        tr_ar_addr       = mux_mst_req.ar.addr;
        tr_ar_len        = mux_mst_req.ar.len;
        tr_ar_size       = mux_mst_req.ar.size;
        tr_ar_burst      = mux_mst_req.ar.burst;
        tr_ar_prot       = mux_mst_req.ar.prot;
        tr_ar_user       = mux_mst_req.ar.user;
        tr_ar_did        = mux_mst_req.ar.stream_id;
        tr_ar_ssid_valid = mux_mst_req.ar.ss_id_valid;
        tr_ar_subid      = mux_mst_req.ar.substream_id;
        tr_ar_mmu_atst   = mux_mst_req.ar.mmu_atst;
        tr_ar_mmu_valid  = mux_mst_req.ar.mmu_valid;

        tr_w_valid       = mux_mst_req.w_valid;
        tr_w_data        = mux_mst_req.w.data;
        tr_w_strb        = mux_mst_req.w.strb;
        tr_w_last        = mux_mst_req.w.last;
        tr_w_user        = mux_mst_req.w.user;

        tr_b_ready       = mux_mst_req.b_ready;
        tr_r_ready       = mux_mst_req.r_ready;
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
        mux_mst_resp.r_valid  = tr_r_valid;
        mux_mst_resp.r.id     = tr_r_id;
        mux_mst_resp.r.data   = tr_r_data;
        mux_mst_resp.r.resp   = tr_r_resp;
        mux_mst_resp.r.last   = tr_r_last;
        mux_mst_resp.r.user   = '0;
    end

    // ========================================================================
    // Certificate-driven full-buffer permit (NOT a static 2'b11).
    // Bit b == bank b full-buffer permit; bank0 = V = DMA0, bank1 = A = DMA1.
    // ========================================================================
    assign fullbuffer_jobs_o = { d1_fullbuffer, d0_fullbuffer };

    // ========================================================================
    // The unchanged joint harness (real banked CTL <-> real TL)
    // ========================================================================
    nonblock_banked_top #(
        .NonblockEnable      ( NonblockEnable      ),
        .BankedEnable        ( 1'b1                ),
        .NumOutstandingTrans ( NumOutstandingTrans ),
        .WFifoDepth          ( WFifoDepth          ),
        .PolicyEnable        ( PolicyEnable        )
    ) i_sys (
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
        .nonblock_fire_o       ( /* open */          ),
        .nonblock_busy_o       ( /* open */          ),
        .in_flight_o           ( /* open */          ),

        .fullbuffer_jobs_i     ( fullbuffer_jobs_o   ),

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
                        cfg_state_q <= cfg_write ? C_AW : C_AR;
                    end
                end
                C_AW: begin
                    // AW is accepted the cycle after aw_ready.
                    if (cfg_rsp[cfg_sel_q].aw_ready) cfg_state_q <= C_W;
                end
                C_W: begin
                    if (cfg_rsp[cfg_sel_q].w_ready) cfg_state_q <= C_B;
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
        cfg_req[0] = '0;
        cfg_req[1] = '0;

        automatic cfg_req_t r = '0;
        r.aw.id    = '0;
        r.aw.addr  = cfg_addr_q;
        r.aw.len   = 8'd0;                 // single beat
        r.aw.size  = 3'd3;                 // 8 bytes
        r.aw.burst = axi_pkg::BURST_INCR;
        r.aw.user  = '0;
        r.w.data   = cfg_wdata_q;
        r.w.strb   = cfg_wstrb_q;
        r.w.last   = 1'b1;
        r.w.user   = '0;
        r.ar.id    = '0;
        r.ar.addr  = cfg_addr_q;
        r.ar.len   = 8'd0;
        r.ar.size  = 3'd3;
        r.ar.burst = axi_pkg::BURST_INCR;
        r.ar.user  = '0;
        r.b_ready  = 1'b1;
        r.r_ready  = 1'b1;

        unique case (cfg_state_q)
            C_AW:    r.aw_valid = 1'b1;
            C_W:     r.aw_valid = 1'b1;    // keep AW asserted (already accepted) is harmless
                     r.w_valid  = 1'b1;
            C_B:     r.b_ready  = 1'b1;
            C_AR:    r.ar_valid = 1'b1;
            C_R:     r.r_ready  = 1'b1;
            default: ;
        endcase

        if (cfg_sel_q) cfg_req[1] = r;
        else           cfg_req[0] = r;
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

endmodule
```
END_FILE

---

BEGIN_FILE tests/nonblock_actual_dma_test.cpp
```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_actual_dma_test.cpp
//
// Bounded DIAGNOSTIC software around tests/nonblock_actual_dma_top.sv.
//
// What is REAL here (no C++ fakes):
//   * AW / AR / W on the IOMMU translation-request port are produced by TWO
//     real dma_core_wrap (frontend + legalizer + R->AW coupler + datapath);
//   * the frontend register file is reached through a REAL AXI slave access
//     generated by the test-only register-master FSM in the SV wrapper: the
//     C++ only issues cfg_valid/cfg_write/cfg_addr/cfg_wdata/cfg_wstrb.
//
// What is MODELED (identical to tests/nonblock_coupled_test.cpp):
//   * DDR + page tables + one ordered R-return queue (completion + DDTW + PTW);
//   * a per-4KiB page content checker.
//
// Software timeline (all phase prints are software-timing diagnostics and are
// NOT a board measurement):
//   1. setup_memory()/reset() per case (counters never carried across cases);
//   2. warm: --warmup V transfers (batch OFF, NEXT_ID/DONE polling) with a
//      continuous A loop over --a-pages pages, then drain A;
//   3. configure A autonomous batch (target --a-target) and V batch
//      (target --transfers, pages=1, gap --v-gap), both configurations
//      completed before any trigger;
//   4. A NEXT_ID trigger, then V NEXT_ID trigger;
//   5. wait V batch complete, STOP A, drain to quiet;
//   6. verify every touched page byte-for-byte; check quiet.
//
// Exit: 0 on PASS, non-zero on FAIL / bounded STALL / WAIT.

#include "Vnonblock_actual_dma_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// identifiers / addresses / sizes (identical to the coupled harness)
// ---------------------------------------------------------------------------
static const uint32_t V_DID     = 10;
static const uint32_t A_DID     = 11;

static const uint64_t V_SRC_IOVA = 0x10000000ULL;
static const uint64_t V_DST_IOVA = 0x10001000ULL;
static const uint64_t V_SRC_PA   = 0x80000000ULL;
static const uint64_t V_DST_PA   = 0x80001000ULL;

static const uint64_t A_IOVA_BASE   = 0x20000000ULL;
static const uint64_t A_SRC_PA_BASE = 0x90000000ULL;
static const uint64_t A_DST_PA_BASE = 0x90100000ULL;

static const uint32_t PAGE_BEATS = 512;   // 4 KiB / 8 B
static const uint32_t PAGE_BYTES = 4096;

// ---------------------------------------------------------------------------
// 64-bit frontend register offsets (idma_reg64_frontend_reg_pkg.sv:110-119)
// and the BatchPort window (idma_reg64_frontend.sv:192-214, 424-446)
// ---------------------------------------------------------------------------
static const uint64_t REG_SRC    = 0x00;
static const uint64_t REG_DST    = 0x08;
static const uint64_t REG_NB     = 0x10;
static const uint64_t REG_CONF   = 0x18;
static const uint64_t REG_STATUS = 0x20;
static const uint64_t REG_NEXT   = 0x28;
static const uint64_t REG_DONE   = 0x30;

static const uint64_t REG_B_CTL    = 0x88;   // enable / stop / status (R+W)
static const uint64_t REG_B_TARGET = 0x90;   // number of 4 KiB jobs
static const uint64_t REG_B_PAGES  = 0x98;   // ring page count
static const uint64_t REG_B_DONE   = 0xa0;   // completed jobs
static const uint64_t REG_B_ISSUED = 0xa8;   // issued jobs
static const uint64_t REG_B_GAP    = 0xd0;   // inter-job gap cycles

// cfg_sel values
static const bool SEL_V = false;   // DMA0
static const bool SEL_A = true;    // DMA1

// ---------------------------------------------------------------------------
// deterministic, injective payload words (identical to the coupled harness)
// ---------------------------------------------------------------------------
static uint64_t make_test_word(uint64_t word_index) {
    return (word_index * 0x9E3779B97F4A7C15ULL) ^ 0xA5C3E10D2B4F9678ULL;
}
static uint64_t page_word(uint32_t page, uint32_t i) {
    return make_test_word(((uint64_t)page << 9) | (uint64_t)i);
}
static uint64_t page_old(uint32_t page, uint32_t i) {
    return 0xD000000000000000ULL ^ ((uint64_t)(page + 1) << 40) ^ (uint64_t)i;
}

struct TestFailure : std::runtime_error {
    explicit TestFailure(const std::string& m) : std::runtime_error(m) {}
};
static void fail(const char* msg, const char* file, int line) {
    char buf[512];
    std::snprintf(buf, sizeof(buf), "%s (%s:%d)", msg, file, line);
    throw TestFailure(buf);
}
#define CHECK(cond, msg) do { if (!(cond)) fail((msg), __FILE__, __LINE__); } while (0)

// ---------------------------------------------------------------------------
class Tb {
public:
    Vnonblock_actual_dma_top* top;

    // ---------------- shared memory -------------------------------------
    std::map<uint64_t, uint8_t> mem;

    enum Port { P_COMP = 0, P_DDTW = 1, P_PTW = 2 };
    struct MemRead {
        Port     port    = P_COMP;
        uint64_t addr    = 0;
        uint8_t  len     = 0;
        uint8_t  id      = 0;
        uint32_t beat    = 0;
        int      latency = 1;
    };
    std::deque<MemRead> mem_q;                 // SINGLE ordered R-return queue

    struct CawEnt { uint32_t id = 0; uint64_t addr = 0; uint8_t len = 0; uint32_t got = 0; };
    std::deque<CawEnt>   caw_q;                // completion AWs being written
    std::deque<uint32_t> cb_q;                 // completion B ids ready

    // ---------------- run-time knobs ------------------------------------
    bool     nonblock_enable   = true;
    bool     policy_partition  = false;
    uint32_t ptw_latency       = 1;
    uint32_t a_pages           = 8;
    uint32_t v_gap_cycles      = 1000;
    uint32_t v_transfers       = 64;
    uint32_t warmup            = 256;
    uint64_t a_target          = 1048576ULL;   // or 0x100000000ULL

    long g_cycle        = 0;
    long scenario_start = 0;
    long last_progress_cycle = 0;

    // ---------------- config driver state -------------------------------
    bool     cfg_valid  = false;
    bool     cfg_write  = false;
    bool     cfg_sel    = false;
    uint64_t cfg_addr   = 0;
    uint64_t cfg_wdata  = 0;
    uint8_t  cfg_wstrb  = 0;

    // ---------------- observation counters ------------------------------
    uint64_t n_comp_ar = 0, n_ddtw_ar = 0, n_ptw_ar = 0;
    uint64_t n_r_beats = 0, n_b_up = 0;
    uint64_t n_tl_accept = 0, n_tl_resp = 0;
    uint64_t n_ptw_active = 0, n_iotlb_lookup = 0, n_iotlb_hit = 0;
    uint64_t n_req_bank[2] = {0, 0};
    uint64_t n_resp_bank[2] = {0, 0};

    // per-DMA last DONE value for edge detection
    uint64_t v_done_last = 0, a_done_last = 0;

    static const long WATCHDOG_CYCLES = 200000L;   // ~10x a 4 KiB copy + walk
    static const long CFG_TIMEOUT     = 100000L;

    Tb() : top(new Vnonblock_actual_dma_top) {}
    ~Tb() { top->final(); delete top; }

    // ---------------------------------------------------------------------
    // memory helpers (identical to coupled harness)
    // ---------------------------------------------------------------------
    void write64(uint64_t addr, uint64_t data) {
        for (int i = 0; i < 8; ++i)
            mem[addr + i] = (uint8_t)((data >> (8 * i)) & 0xFF);
    }
    uint64_t read64(uint64_t addr) {
        uint64_t data = 0;
        for (int i = 0; i < 8; ++i) {
            auto it = mem.find(addr + i);
            if (it == mem.end()) fail("unmapped read", __FILE__, __LINE__);
            data |= (uint64_t)it->second << (8 * i);
        }
        return data;
    }
    void write_beat(uint64_t addr, uint64_t data, uint8_t strb) {
        for (int i = 0; i < 8; ++i)
            if (strb & (1 << i))
                mem[addr + i] = (uint8_t)((data >> (8 * i)) & 0xFF);
    }

    // ---------------------------------------------------------------------
    // devices / page tables (identical to coupled harness + A ring)
    // ---------------------------------------------------------------------
    void setup_memory(uint32_t ap) {
        mem.clear();
        a_pages = ap;

        // Extended 1-level DDT at 0x10000, stride 64. DID10 -> 0x10280, DID11 -> 0x102C0.
        write64(0x10280, 0x0000000000000001ULL);
        write64(0x10288, 0x0000000000000000ULL);
        write64(0x10290, 0x000000000000A000ULL);
        write64(0x10298, 0x8000000000000020ULL); // Sv39, V root 0x20000
        write64(0x102A0, 0x0ULL); write64(0x102A8, 0x0ULL);
        write64(0x102B0, 0x0ULL); write64(0x102B8, 0x0ULL);

        write64(0x102C0, 0x0000000000000001ULL);
        write64(0x102C8, 0x0000000000000000ULL);
        write64(0x102D0, 0x000000000000B000ULL);
        write64(0x102D8, 0x8000000000000030ULL); // Sv39, A root 0x30000
        write64(0x102E0, 0x0ULL); write64(0x102E8, 0x0ULL);
        write64(0x102F0, 0x0ULL); write64(0x102F8, 0x0ULL);

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
        write64(0x20000, 0x0000000000010001ULL);
        write64(0x40400, 0x0000000000014001ULL);
        write64(0x50000, 0x00000000200000DFULL); // Vsrc -> 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // Vdst -> 0x80001000

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
        write64(0x30000, 0x0000000000018001ULL);
        write64(0x60800, 0x000000000001C001ULL);
        for (uint32_t n = 0; n < 2 * a_pages; ++n) {
            uint64_t pa = (n < a_pages)
                        ? (A_SRC_PA_BASE + (uint64_t)n * 0x1000)
                        : (A_DST_PA_BASE + (uint64_t)(n - a_pages) * 0x1000);
            write64(0x70000 + (uint64_t)n * 8, ((pa >> 12) << 10) | 0xDFULL);
        }

        // V source content + unique destination pre-fill.
        for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
            write64(V_SRC_PA + (uint64_t)i * 8, page_word(0, i));
            write64(V_DST_PA + (uint64_t)i * 8, page_old(0, i));
        }
        // A source content + unique destination pre-fill.
        for (uint32_t p = 0; p < a_pages; ++p) {
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                write64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_word(1 + p, i));
                write64(A_DST_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                        page_old(1 + p, i));
            }
        }
    }

    // ---------------------------------------------------------------------
    // memory R-return drive (ordered, head-of-line) -- no Rready hardwire
    // ---------------------------------------------------------------------
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
        }
    }

    // ---------------------------------------------------------------------
    // one cycle
    // ---------------------------------------------------------------------
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
        drive_comp();
        drive_mem_r();
        top->ddtw_ar_ready = 1;
        top->ptw_ar_ready  = 1;
        top->eval();

        // ---- snapshot handshakes BEFORE the rising edge -----------------
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

        if (f_comp_ar || f_ddtw_ar || f_ptw_ar || f_comp_aw || f_comp_w ||
            f_comp_b || f_comp_r || f_ddtw_r || f_ptw_r) {
            last_progress_cycle = g_cycle;
        }

        // ---- TL observation counters + per-bank counts ------------------
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

        // ---- rising edge -------------------------------------------------
        top->clk_i = 1;
        top->eval();

        // ---- queue / state updates from snapshots ------------------------
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

        top->clk_i = 0;
        top->eval();
        ++g_cycle;
    }

    // ---------------------------------------------------------------------
    // configuration transactions (blocking; interleaved with tick())
    // ---------------------------------------------------------------------
    uint64_t cfg_xact(bool sel, bool write, uint64_t addr, uint64_t wdata, uint8_t wstrb) {
        cfg_sel = sel; cfg_write = write; cfg_addr = addr;
        cfg_wdata = wdata; cfg_wstrb = wstrb; cfg_valid = true;

        long t = 0;
        for (; t < CFG_TIMEOUT; ++t) {
            tick();
            if (top->cfg_ready) break;
        }
        if (!top->cfg_ready) {
            cfg_valid = false;
            dump_stall("cfg-timeout");
            throw TestFailure("configuration transaction never completed");
        }
        uint64_t rd = top->cfg_rdata;
        bool     er = top->cfg_error;
        cfg_valid = false;
        tick();                      // let the SV FSM return to IDLE
        if (er) fail("configuration transaction returned an error", __FILE__, __LINE__);
        return rd;
    }

    void cfg_wr(bool sel, uint64_t addr, uint64_t data) {
        cfg_xact(sel, true, addr, data, 0xFF);
    }
    uint64_t cfg_rd(bool sel, uint64_t addr) {
        return cfg_xact(sel, false, addr, 0, 0x00);
    }

    // program ONE batch-off 64-bit copy
    void program_single(bool sel, uint64_t src, uint64_t dst) {
        cfg_wr(sel, REG_CONF, 0x0);
        cfg_wr(sel, REG_SRC,  src);
        cfg_wr(sel, REG_DST,  dst);
        cfg_wr(sel, REG_NB,   (uint64_t)PAGE_BYTES);
    }

    // read NEXT_ID (0x28) to issue the programmed transfer
    void trigger(bool sel) {
        (void)cfg_rd(sel, REG_NEXT);
    }

    // poll DONE (0x30) until it changes from `prev`
    void wait_done(bool sel, uint64_t& prev, const char* where) {
        long t = 0;
        for (; t < WATCHDOG_CYCLES; ++t) {
            tick();
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall(where);
                throw TestFailure("watchdog: no handshake progress while waiting DONE");
            }
            uint64_t d = cfg_rd(sel, REG_DONE);
            if (d != prev) { prev = d; return; }
        }
        dump_stall(where);
        throw TestFailure("DONE never changed");
    }

    // ---------------------------------------------------------------------
    // reset / quiet
    // ---------------------------------------------------------------------
    void reset(bool enable) {
        nonblock_enable = enable;
        top->clk_i  = 0;
        top->rst_ni = 0;

        cfg_valid = false; cfg_write = false; cfg_sel = false;
        cfg_addr = 0; cfg_wdata = 0; cfg_wstrb = 0;

        mem_q.clear(); caw_q.clear(); cb_q.clear();
        n_comp_ar = n_ddtw_ar = n_ptw_ar = 0;
        n_r_beats = n_b_up = 0;
        n_tl_accept = n_tl_resp = 0;
        n_ptw_active = n_iotlb_lookup = n_iotlb_hit = 0;
        n_req_bank[0] = n_req_bank[1] = 0;
        n_resp_bank[0] = n_resp_bank[1] = 0;
        v_done_last = a_done_last = 0;

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        for (int i = 0; i < 4; ++i) tick();
        last_progress_cycle = g_cycle;

        // Baseline DONE values so the first wait_done() sees an edge.
        v_done_last = cfg_rd(SEL_V, REG_DONE);
        a_done_last = cfg_rd(SEL_A, REG_DONE);
    }

    void check_quiet(const char* tag) {
        for (int i = 0; i < 64; ++i) {
            tick();
            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall(std::string("quiet ") + tag);
                throw TestFailure("watchdog during drain-to-quiet");
            }
        }
        CHECK(mem_q.empty() && caw_q.empty() && cb_q.empty(),
              "memory/AW queues not empty after drain");
        (void)tag;
    }

    // ---------------------------------------------------------------------
    // stall dump (software-time diagnostic; no scientific claim)
    // ---------------------------------------------------------------------
    void dump_stall(const std::string& where) {
        std::fprintf(stderr,
            "nonblock_actual_dma_test: STALL at %s cycle=%ld scenario_cycles=%ld "
            "since_progress=%ld\n",
            where.c_str(), g_cycle, g_cycle - scenario_start,
            g_cycle - last_progress_cycle);

        if (!mem_q.empty()) {
            const MemRead& h = mem_q.front();
            std::fprintf(stderr,
                "  sharedR.head port=%d id=%u addr=0x%016lx len=%u beat=%u latency=%d\n",
                (int)h.port, (unsigned)h.id, (unsigned long)h.addr,
                (unsigned)h.len, h.beat, h.latency);
        } else {
            std::fprintf(stderr, "  sharedR.head (empty)\n");
        }
        if (!caw_q.empty()) {
            std::fprintf(stderr, "  caw_q.head id=%u addr=0x%016lx len=%u got=%u\n",
                         (unsigned)caw_q.front().id,
                         (unsigned long)caw_q.front().addr,
                         (unsigned)caw_q.front().len, caw_q.front().got);
        }
        std::fprintf(stderr,
            "  cfg_state_q=%d cfg_sel=%d cfg_valid=%d\n",
            (int)0, (int)cfg_sel, (int)cfg_valid);
        std::fprintf(stderr,
            "  TL req_v=%d req_r=%d req_bank=%d resp_v=%d resp_r=%d resp_bank=%d "
            "ptw_active=%d ptw_r_fire=%d iotlb_look=%d iotlb_hit=%d\n",
            (int)top->dbg_tl_req_valid, (int)top->dbg_tl_req_ready,
            (int)top->dbg_tl_req_bank,  (int)top->dbg_tl_resp_valid,
            (int)top->dbg_tl_resp_ready,(int)top->dbg_tl_resp_bank,
            (int)top->dbg_tl_ptw_active,(int)top->dbg_tl_ptw_r_fire,
            (int)top->dbg_tl_iotlb_lookup,(int)top->dbg_tl_iotlb_hit);
        std::fprintf(stderr,
            "  DMA issue=%d done=%d poll=%d batch_active=%d batch_gap=%d "
            "job_quiet=%d fullbuffer=%d launch_active=%d launch_held=%d launch_err=%d\n",
            (int)(top->dbg_dma_issue & 1), (int)(top->dbg_dma_done & 1),
            (int)(top->dbg_dma_poll & 1),
            (int)(top->dbg_dma_batch_active & 1), (int)(top->dbg_dma_batch_gap & 1),
            (int)(top->dbg_dma_job_quiet & 1), (int)(top->dbg_dma_fullbuffer & 1),
            (int)(top->dbg_dma_launch_active & 1), (int)(top->dbg_dma_launch_held & 1),
            (int)(top->dbg_dma_launch_error & 1));
        std::fprintf(stderr,
            "  mem_q=%zu caw_q=%zu cb_q=%zu\n",
            mem_q.size(), caw_q.size(), cb_q.size());
    }

    // ---------------------------------------------------------------------
    // batch helpers
    // ---------------------------------------------------------------------
    // returns {error, complete, stopped, active, enable}
    void read_batch_ctl(bool sel, bool& err, bool& complete, bool& stopped,
                        bool& active, bool& enable) {
        uint64_t v = cfg_rd(sel, REG_B_CTL);
        err      = (v >> 32) & 1;
        complete = (v >> 3) & 1;
        stopped  = (v >> 2) & 1;
        active   = (v >> 1) & 1;
        enable   = (v >> 0) & 1;
    }

    void configure_batch(bool sel, uint64_t src, uint64_t dst,
                         uint64_t target, uint32_t pages, uint32_t gap) {
        cfg_wr(sel, REG_CONF, 0x0);
        cfg_wr(sel, REG_SRC,  src);
        cfg_wr(sel, REG_DST,  dst);
        cfg_wr(sel, REG_NB,   (uint64_t)PAGE_BYTES);
        cfg_wr(sel, REG_B_PAGES,  (uint64_t)pages);
        cfg_wr(sel, REG_B_TARGET, target);
        cfg_wr(sel, REG_B_GAP,    (uint64_t)gap);
        cfg_wr(sel, REG_B_CTL,    1ULL);            // enable
        // sanity: enable bit set, no error
        bool err, complete, stopped, active, enable;
        read_batch_ctl(sel, err, complete, stopped, active, enable);
        CHECK(!err && enable, "batch enable failed");
    }

    void start_batch(bool sel) {
        (void)cfg_rd(sel, REG_NEXT);                // NEXT_ID read triggers
    }

    void stop_batch(bool sel) {
        cfg_wr(sel, REG_B_CTL, 2ULL);               // STOP drains current job
    }

    // ---------------------------------------------------------------------
    // warm phase: --warmup V transfers (batch OFF) + continuous A ring
    // ---------------------------------------------------------------------
    void warm_phase() {
        for (uint32_t i = 0; i < warmup; ++i) {
            // V : one 4 KiB copy, batch OFF
            program_single(SEL_V, V_SRC_IOVA, V_DST_IOVA);
            trigger(SEL_V);
            wait_done(SEL_V, v_done_last, "warm V");

            // A : ring over a_pages, batch OFF
            uint32_t p = i % a_pages;
            program_single(SEL_A,
                           A_IOVA_BASE + (uint64_t)p * 0x1000,
                           A_IOVA_BASE + (uint64_t)(a_pages + p) * 0x1000);
            trigger(SEL_A);
            wait_done(SEL_A, a_done_last, "warm A");

            if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                dump_stall("warm");
                throw TestFailure("watchdog during warm-up");
            }
        }
    }

    void drain_a() {
        // batch is OFF during warm: nothing is outstanding by construction,
        // but keep the check so a future change cannot silently paper over it.
        bool err, complete, stopped, active, enable;
        read_batch_ctl(SEL_A, err, complete, stopped, active, enable);
        CHECK(!active, "A still active after warm drain");
    }

    // ---------------------------------------------------------------------
    // one scenario
    // ---------------------------------------------------------------------
    void run_scenario(const char* name, bool enable) {
        std::printf("== actual-dma scenario %s (runtime enable=%d, a_pages=%u, "
                    "ptw_latency=%u, transfers=%u, v_gap=%u, warmup=%u, "
                    "a_target=%llu, policy=%d) ==\n",
                    name, (int)enable, a_pages, ptw_latency, v_transfers,
                    v_gap_cycles, warmup, (unsigned long long)a_target,
                    (int)policy_partition);

        setup_memory(a_pages);
        reset(enable);
        scenario_start      = g_cycle;
        last_progress_cycle = g_cycle;

        // ---- 1. warm + drain -------------------------------------------
        warm_phase();
        drain_a();

        // ---- 2. configure BOTH batches before any trigger --------------
        // A autonomous: ring over a_pages, no gap.
        configure_batch(SEL_A, A_IOVA_BASE,
                        A_IOVA_BASE + (uint64_t)a_pages * 0x1000,
                        a_target, a_pages, 0);
        // V victim: single page, --transfers jobs, --v-gap gap.
        configure_batch(SEL_V, V_SRC_IOVA, V_DST_IOVA,
                        (uint64_t)v_transfers, 1, v_gap_cycles);

        // ---- 3. A NEXT_ID then V NEXT_ID -------------------------------
        start_batch(SEL_A);
        start_batch(SEL_V);

        // ---- 4. wait for V batch completion ----------------------------
        {
            long t = 0;
            bool err, complete, stopped, active, enable_b;
            for (;;) {
                tick();
                if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                    dump_stall("scenario V");
                    throw TestFailure("watchdog: no handshake progress while V runs");
                }
                read_batch_ctl(SEL_V, err, complete, stopped, active, enable_b);
                if (complete) break;
                if (g_cycle - scenario_start > (long)WATCHDOG_CYCLES * 200L) {
                    dump_stall("scenario V budget");
                    throw TestFailure("scenario V budget exceeded");
                }
                ++t;
            }
            (void)t;
        }

        // ---- 5. STOP A and drain ---------------------------------------
        stop_batch(SEL_A);
        {
            bool err, complete, stopped, active, enable_b;
            long t = 0;
            for (;;) {
                tick();
                if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                    dump_stall("scenario A drain");
                    throw TestFailure("watchdog: no handshake progress while A drains");
                }
                read_batch_ctl(SEL_A, err, complete, stopped, active, enable_b);
                if (!active) break;
                if (++t > 200L * WATCHDOG_CYCLES) {
                    dump_stall("scenario A drain budget");
                    throw TestFailure("A drain budget exceeded");
                }
            }
        }

        check_quiet(name);

        // ---- 6. exact byte verification of every touched page ----------
        for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
            CHECK(read64(V_SRC_PA + (uint64_t)i * 8) == page_word(0, i),
                  "V source page was modified");
            CHECK(read64(V_DST_PA + (uint64_t)i * 8) == page_word(0, i),
                  "V destination bytes do not match the copied source");
        }
        for (uint32_t p = 0; p < a_pages; ++p) {
            for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                CHECK(read64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8)
                      == page_word(1 + p, i),
                      "A source page was modified");
                CHECK(read64(A_DST_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8)
                      == page_word(1 + p, i),
                      "A destination bytes do not match the copied source");
            }
        }

        // ---- 7. mechanism guards ---------------------------------------
        CHECK(n_comp_ar > 0, "no completion reads observed");
        CHECK(n_r_beats + n_comp_ar > 0, "no R traffic observed");
        CHECK(n_tl_accept > 0, "no translation accepted by the TL");
        CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");

        std::printf("ACTUAL-DMA-SUMMARY %s enable=%d a_pages=%u transfers=%u "
                    "cycles=%ld comp_ar=%lu ddtw_ar=%lu ptw_ar=%lu "
                    "tl_accept=%lu tl_resp=%lu iotlb_look=%lu iotlb_hit=%lu "
                    "ptw_active=%lu req_bank0=%lu req_bank1=%lu "
                    "resp_bank0=%lu resp_bank1=%lu v_gap=%u ptw_lat=%u\n",
                    name, (int)enable, a_pages, v_transfers,
                    g_cycle - scenario_start,
                    (unsigned long)n_comp_ar, (unsigned long)n_ddtw_ar,
                    (unsigned long)n_ptw_ar, (unsigned long)n_tl_accept,
                    (unsigned long)n_tl_resp, (unsigned long)n_iotlb_lookup,
                    (unsigned long)n_iotlb_hit, (unsigned long)n_ptw_active,
                    (unsigned long)n_req_bank[0], (unsigned long)n_req_bank[1],
                    (unsigned long)n_resp_bank[0], (unsigned long)n_resp_bank[1],
                    v_gap_cycles, ptw_latency);
    }
};

// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::string mode     = "s";
    uint32_t    a_pages  = 8;
    uint32_t    v_gap    = 1000;
    uint32_t    transfers= 64;
    uint32_t    warmup   = 256;
    uint32_t    ptw_lat  = 31;
    uint64_t    a_target = 1048576ULL;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto val = [&](const char* k) -> const char* {
            const std::string key = std::string("+") + k + "=";
            if (a.rfind(key, 0) == 0) return a.c_str() + key.size();
            return nullptr;
        };
        if (const char* v = val("mode"))      mode = v;
        if (const char* v = val("a-pages"))   a_pages = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("v-gap"))     v_gap = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("transfers")) transfers = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("warmup"))    warmup = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("ptw-latency")) ptw_lat = (uint32_t)std::strtoul(v, nullptr, 0);
        if (const char* v = val("a-target"))  a_target = std::strtoull(v, nullptr, 0);
    }

    // ---- strict validation ------------------------------------------------
    if (mode != "s" && mode != "n") {
        std::fprintf(stderr, "nonblock_actual_dma_test: invalid +mode='%s' (expected s|n)\n",
                     mode.c_str());
        return 2;
    }
    if (a_pages == 0) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +a-pages must be > 0\n");
        return 2;
    }
    if (!(transfers == 64 || transfers == 1024 || transfers == 4096)) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +transfers must be 64|1024|4096\n");
        return 2;
    }
    if (!(ptw_lat == 31 || ptw_lat == 100)) {
        std::fprintf(stderr, "nonblock_actual_dma_test: +ptw-latency must be 31|100\n");
        return 2;
    }

    // The harness compiles --mode into runtime nonblock_enable_i.
    //   s -> nonblock enable asserted, n -> nonblock enable de-asserted.
    // This S/N mapping is an ASSUMPTION to be confirmed by Codex.
    const bool enable = (mode == "s");

    std::printf("nonblock_actual_dma_test: mode=%s (nonblock_enable=%d)\n",
                mode.c_str(), (int)enable);
    std::printf("nonblock_actual_dma_test: a_pages=%u v_gap=%u transfers=%u warmup=%u "
                "ptw_latency=%u a_target=%llu\n",
                a_pages, v_gap, transfers, warmup, ptw_lat,
                (unsigned long long)a_target);
    std::printf("NOTE: ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched.\n");

    int rc = 0;
    try {
        Tb tb;
        tb.a_pages         = a_pages;
        tb.v_gap_cycles    = v_gap;
        tb.v_transfers     = transfers;
        tb.warmup          = warmup;
        tb.ptw_latency     = ptw_lat;
        tb.a_target        = a_target;
        tb.policy_partition= false;
        tb.run_scenario(enable ? "s" : "n", enable);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "nonblock_actual_dma_test: SCENARIO-FAIL %s: %s\n",
                     (enable ? "s" : "n"), e.what());
        rc = 1;
    }

    if (rc == 0) std::printf("nonblock_actual_dma_test: PASS\n");
    return rc;
}
```
END_FILE

---

BEGIN_FILE tests/test_nonblock_actual_dma.py
```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the bounded actual-DMA diagnostic test.

Builds a plain C++ (Verilator --cc --exe --build, no --timing) testbench
around tests/nonblock_actual_dma_top.sv.  That wrapper re-instantiates the
UNCHANGED tests/nonblock_banked_top.sv and drives its translation-request
port from TWO real dma_core_wrap instances through the real 4-input axi_mux.

The RTL source list / include dirs are imported from the sibling
tests/test_nonblock_banked.py so no file is duplicated.  The common_cells /
axi packages come from EXACTLY ONE vendor tree (the one that runner already
uses); the iDMA and register_interface files are appended, never a second
copy of common_cells/axi.

No Vivado, no hardware.  The object directory is independent:
out/nonblock-actual-dma/obj.

Examples
--------
    python3 tests/test_nonblock_actual_dma.py --mode s --transfers 4096
    python3 tests/test_nonblock_actual_dma.py --mode n --transfers 64 --ptw-latency 31
"""

import argparse
import importlib.util
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CVA6 = ROOT / "cva6"
IOMMU = CVA6 / "corev_apu" / "riscv-iommu"
IDMA = CVA6 / "corev_apu" / "iDMA"
REGI = CVA6 / "corev_apu" / "register_interface"

OUT = ROOT / "out" / "nonblock-actual-dma"
TOP = "nonblock_actual_dma_top"


def load_sibling():
    path = Path(__file__).resolve().parent / "test_nonblock_banked.py"
    if not path.is_file():
        sys.exit("sibling runner missing: {}".format(path))
    spec = importlib.util.spec_from_file_location("nonblock_banked_runner", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None or p in seen:
            continue
        seen.add(p)
        out.append(p)
    return out


def tail(path: Path, lines: int = 240) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


# ---------------------------------------------------------------------------
# Ordered iDMA source list.
#
# Order matters: packages first (idma_pkg, then the Auto-generated register
# packages), then the level-ordered RTL taken from iDMA/Bender.yml:20-98, then
# the register_interface primitives used by the generated reg_top files.
# ---------------------------------------------------------------------------
def idma_sources():
    prim = REGI / "vendor" / "lowrisc_opentitan" / "src"
    return [
        # --- packages (ordered imports) ---
        IDMA / "src" / "idma_pkg.sv",
        IDMA / "src" / "frontends" / "register_64bit" / "idma_reg64_frontend_reg_pkg.sv",
        # --- level 0 ---
        IDMA / "src" / "frontends" / "idma_transfer_id_gen.sv",
        IDMA / "src" / "idma_stream_fifo.sv",
        IDMA / "src" / "idma_fullbuffer_certificate.sv",
        # --- level 1 ---
        IDMA / "src" / "idma_buffer.sv",
        IDMA / "src" / "idma_error_handler.sv",
        IDMA / "src" / "idma_channel_coupler.sv",
        # --- level 2 ---
        IDMA / "src" / "idma_axi_transport_layer.sv",
        IDMA / "src" / "idma_axi_lite_transport_layer.sv",
        IDMA / "src" / "idma_obi_transport_layer.sv",
        IDMA / "src" / "idma_legalizer.sv",
        # --- level 3 ---
        IDMA / "src" / "idma_backend.sv",
        # --- 64-bit register frontend ---
        IDMA / "src" / "frontends" / "register_64bit" / "idma_reg64_frontend_reg_top.sv",
        IDMA / "src" / "frontends" / "register_64bit" / "idma_reg64_frontend.sv",
        # --- cva6 system wrapper (non-intf dma_core_wrap) ---
        IDMA / "src" / "systems" / "cva6_reg" / "dma_core_wrap.sv",
        # --- register_interface primitives needed by the generated reg_top ---
        prim / "prim_subreg_arb.sv",
        prim / "prim_subreg.sv",
        prim / "prim_subreg_ext.sv",
        REGI / "src" / "axi_to_reg.sv",
        REGI / "src" / "axi_lite_to_reg.sv",
    ]


def idma_incdirs():
    return [
        IDMA / "src" / "include",
        REGI / "vendor" / "lowrisc_opentitan" / "src",
    ]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--mode", choices=("s", "n"), default="s",
                    help="runtime IOMMU mode: s -> nonblock_enable_i=1, "
                         "n -> nonblock_enable_i=0 (mapping is an assumption; "
                         "see the C++ header comment)")
    ap.add_argument("--a-pages", type=int, default=8,
                    help="number of 4 KiB aggressor pages in the A ring")
    ap.add_argument("--v-gap", type=int, default=1000,
                    help="V inter-job gap cycles inside the V batch")
    ap.add_argument("--transfers", type=int, choices=(64, 1024, 4096), default=64,
                    help="V victim 4 KiB jobs")
    ap.add_argument("--warmup", type=int, default=256,
                    help="batch-OFF V transfers issued during warm-up")
    ap.add_argument("--ptw-latency", type=int, choices=(31, 100), default=31,
                    help="model PTW read latency (DDL/DDR model only; NOT MIG)")
    ap.add_argument("--a-target", type=lambda s: int(s, 0), default=1048576,
                    help="A autonomous batch target (default 1048576)")
    opts = ap.parse_args()

    sibling = load_sibling()
    tb = ROOT / "tests" / "nonblock_actual_dma_test.cpp"

    # common_cells/axi come from the sibling's tree ONLY (no duplicate vendor).
    sources = dedup(
        [require(p) for p in sibling.SOURCES]
        + [require(p) for p in idma_sources()]
        + [require(ROOT / "tests" / "nonblock_actual_dma_top.sv")]
    )
    incdirs = dedup(list(sibling.INCDIRS) + idma_incdirs())

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module", TOP,
        "--Mdir", str(obj_dir),
    ]
    for inc in incdirs:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce {}\n".format(exe))
        return 1

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
    print("test_nonblock_actual_dma: mode={} a_pages={} v_gap={} transfers={} "
          "warmup={} ptw_latency={} a_target={}".format(
              opts.mode, opts.a_pages, opts.v_gap, opts.transfers,
              opts.warmup, opts.ptw_latency, opts.a_target))
    run = subprocess.run(run_args)
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
```
END_FILE

---

# Interface / dependency notes

**Wrapper ports** (`nonblock_actual_dma_top.sv`)
- `comp_*` (33 signals), `ddtw_*` (13), `ptw_*` (13), `dbg_tl_*` (10): byte-for-byte the `nonblock_banked_top` port shapes — see `tests/nonblock_banked_top.sv:88‑156, 170‑184`.
- `nonblock_enable_i`, `policy_partition_i`: forwarded unchanged to the harness (`:32, :159`).
- `cfg_sel` (1), `cfg_valid/cfg_write`, `cfg_addr[63:0]`, `cfg_wdata[63:0]`, `cfg_wstrb[7:0]`, `cfg_ready/cfg_error/cfg_rdata[63:0]` — single-beat AXI4 on `CfgBusIDWidth = 6` (`ariane_soc::IdWidthSlave = 4 + clog2(4) = 6`, `ariane_soc_pkg.sv:31‑32`).
- `dbg_dma_*`: 2-bit observation vectors (bit0 = V/DMA0, bit1 = A/DMA1).

**Dependencies (paths)**
- `cva6/corev_apu/riscv-iommu/vendor/{common_cells,axi}` — the single vendor pair, imported via the sibling runner.
- `cva6/corev_apu/iDMA/src/**` — real DMA (Bender.yml:20‑98 order) + `frontends/register_64bit/**` + `systems/cva6_reg/dma_core_wrap.sv`.
- `cva6/corev_apu/register_interface/src/{axi_to_reg,axi_lite_to_reg}.sv` and `.../vendor/lowrisc_opentitan/src/prim_subreg*.sv`.
- `cva6/corev_apu/iDMA/src/include` (idma `guard.svh`/`typedef.svh`) and the prim src dir as include dirs.

---

# Limitations / assumptions (do not over-claim)

1. **`VERIFY-P0` (types):** the wrapper assumes `AXI_TYPEDEF_EXT_ALL(name, addr, id, data, strb, user, sid, ssidv, ssid)` exists in `axi-iommu/typedef.svh` and yields channels with `stream_id/ss_id_valid/substream_id/mmu_atst/mmu_valid`. This is what `ariane_peripherals_xilinx.sv:876, 907` and `dma_core_wrap.sv:303‑304` use, but the macro body is **not in context** — confirm the field names used in `unpack_mux_mst`.
2. **`VERIFY-P0` (mux accept EXT):** `axi_mux` must accept EXT slv/mst channel types (as `ariane_peripherals_xilinx.sv:1225‑1250` does). If the local mux version rejects extra fields, switch to a thin shim type.
3. **S/N mapping is an assumption** (`--mode s -> nonblock_enable_i=1`). The task text does not pin the exact bit; confirm with Codex.
4. **No internal TL fast/main FSM tap is exposed.** `nonblock_banked_top` ties `nonblock_diag_o` off (`:800`) and does not surface it; the wrapper therefore only re-exports the existing 10 TL `dbg_*` taps plus the DMA observer vectors. A true TL fast/main FSM dump needs a **read-only hierarchical tap** inside `rv_iommu_tl_wrap`, whose signal names are not in the supplied context (`VERIFY-P1`).
5. **PTW latency is a model delay**, not MIG-matched (statement appears on stdout).
6. **Warm is NEXT_ID/DONE batch-OFF polling**, an approximation of Linux driver behaviour, not bit-exact; no board-match claim is made.
7. **Bank `aw/ar/wfifo` internal counters are not exposed** (no ports on the harness / DUT) — only per-bank TL accept/response counts are collected (`n_req_bank/n_resp_bank`).
8. This is a **diagnostic**, not a production change: no RTL/Linux file is modified.

---

# Suggested acceptance commands (Codex to run; I have not run them)

```bash
# Smoke build + short bounded run (board-like OT8/W32 defaults from sibling)
python3 tests/test_nonblock_actual_dma.py --mode n --transfers 64 --ptw-latency 31

# S-mode long victim
python3 tests/test_nonblock_actual_dma.py --mode s --transfers 4096 --v-gap 1000 --ptw-latency 31

# DDR-model latency variant
python3 tests/test_nonblock_actual_dma.py --mode s --transfers 1024 --ptw-latency 100

# artefacts
ls -l out/nonblock-actual-dma/obj/Vnonblock_actual_dma_top
tail -n 50 out/nonblock-actual-dma/build.log
```

Acceptance = `build.log` clean, `Vnonblock_actual_dma_top` produced, exit code `0` and final line `nonblock_actual_dma_test: PASS`.

---

# Unresolved issues for Codex

- `VERIFY-P0`: macro/type compatibility (points 1–2) — likely the first compile blocker.
- `VERIFY-P0`: `axi_mux` `w_chan_t` packing when slv id width ≠ mst id width (the W channel has no id, so this normally works, but confirm with the vendored mux).
- `VERIFY-P1`: decide whether to add read-only hierarchical taps for the TL fast/main FSM and the bank `aw/ar/wfifo` state; this requires the `rv_iommu_tl_wrap.sv` / `rv_iommu_banked_ctl.sv` sources.
- `VERIFY-P1`: confirm the S/N meaning and whether `policy_partition_i` should also switch with `--mode`.
- `VERIFY-P1`: the `a_target = 0x100000000` upper-bound path (`idma_reg64_frontend.sv:203`) — confirm the C++ default target (`1048576`) is the intended "driver MAX" or switch to the true max.
- The batch `complete` condition requires `batch_completed_q == batch_target_q` (`idma_reg64_frontend.sv:434‑436`); with V `pages=1` this is `--transfers` jobs — confirm that is the wanted "V done" definition.