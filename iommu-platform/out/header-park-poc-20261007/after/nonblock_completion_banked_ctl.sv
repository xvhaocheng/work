// Copyright © 2025 Manuel Rodríguez & Zero-Day Labs, Lda.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Licensed under the Solderpad Hardware License v 2.1 (the “License”);
// you may not use this file except in compliance with the License,
// or, at your option, the Apache License version 2.0.
// You may obtain a copy of the License at https://solderpad.org/SHL-2.1/.
// Unless required by applicable law or agreed to in writing, any work distributed
// under the License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Description: RISC-V IOMMU *banked* (two-lane) transaction controller wrapper.
//
//   This module wires two `rv_iommu_trans_ctl_wrap` instances with read admission
//   ("banks") behind an incoming AXI4 demultiplexer and in front of an outgoing
//   AXI4 multiplexer.  The bank of a transaction is selected by the immutable
//   ORIGINAL AXI ID bit `BankIdBit` (never by the device/stream id):
//
//     upstream  --axi_demux(2, sel=id[BankIdBit])--> bank0 / bank1
//               --axi_mux(2, high bank bit)--------> completion interface
//
//   Design notes / assumptions
//   --------------------------
//   * AXI IDs INSIDE each bank keep the ORIGINAL width.  The outgoing `axi_mux`
//     prepends exactly one bank bit as the MOST SIGNIFICANT ID bit.  That bit is
//     redundant with `id[BankIdBit]` (the incoming demux selected the bank with
//     that same bit), so it is stripped again for AW/AR.  For B/R the redundant
//     MSB is reconstructed from the ORIGINAL `id[BankIdBit]`, because the
//     external completion interface only ever carries ORIGINAL IDs.
//     ⇒ This assumes the external completion slave echoes AW/AR IDs unchanged.
//   * The two banks keep their own AW/AR/W FIFOs, their own AW-before-AR / W
//     gating and their own permission-error slave (each bank instantiates its
//     own `axi_err_slv`).  A wrapper admission guard delays new non-owner reads to avoid DMA
//     R-to-W dependency cycles. The legacy controller default is unchanged.
//   * MRIF is required to be disabled (MSI_DISABLED or MSI_BT_ONLY); the single
//     wrapper MRIF port is tied to zero.
//   * The legacy `rv_iommu::e3_ctl_t` observer is INCOMPATIBLE with two owners
//     (the old counters are not valid shared-owner counters).  `e3_o` therefore
//     returns ZERO except for `.quiet = !in_flight_o`.  NO legacy E3 observer
//     may consume any other field of this instance.
//   * The downstream translation engine must echo the `bank` field it received
//     in `trans_req_data_o` back in `trans_resp_data_i`.  At most one
//     translation is in flight per bank, which makes `bank` an unambiguous
//     lifecycle tag and guarantees the full bank-local header (retained at the
//     head of the bank AW/AR FIFO until COMPLETE) belongs to that tag.

`include "axi/typedef.svh"

// TEST ONLY snapshot. Default CompletionGateEnable=0 reproduces original.
// Enabling the gate disables ONLY translation-stage read admission. Every
// actual completion AR must acquire a packet-specific return disposition.
module nonblock_completion_banked_ctl #(
    parameter bit CompletionGateEnable = 1'b0,
    // Test PoC: 1 shared selectable headers + optional direct path;
    // 2 ordinary two source FIFOs, each HeaderParkDepth entries, no bypass.
    parameter int unsigned HeaderParkMode = 0,
    parameter int unsigned HeaderParkDepth = 2,
    parameter bit HeaderDirectEnable = 1'b1,
    /// RISC-V IOMMU configuration struct (global capacities, see RVIOMMUCfgBank)
    parameter rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::NullCfg,
    /// Legacy E3 observer enable. Kept for interface compatibility. NOT USED:
    /// the legacy E3 path is incompatible with two bank owners (see header).
    parameter bit E3Enable = 1'b0,

    /// ORIGINAL AXI ID bit used to select the bank. MUST be < RVIOMMUCfg.AxiIdWidth.
    parameter int BankIdBit = 0,

    /// Address data type
    parameter type addr_t = logic,
    /// Physical address data type
    parameter type paddr_t = logic,

    /// Translation request data type (must contain a `logic bank` field)
    parameter type trans_req_data_t = logic,
    /// Translation response data type (must contain a `logic bank` field)
    parameter type trans_resp_data_t = logic,
    /// Fault data type
    parameter type fault_data_t = logic,

    // Translation Request Interface data types
    parameter type axi_tr_aw_chan_t = logic,
    parameter type axi_tr_w_chan_t = logic,
    parameter type axi_tr_b_chan_t = logic,
    parameter type axi_tr_r_chan_t = logic,
    parameter type axi_tr_ar_chan_t = logic,
    parameter type axi_tr_req_t = logic,
    parameter type axi_tr_resp_t = logic,

    // Completion Interface data types (ORIGINAL ID width)
    parameter type axi_comp_aw_chan_t = logic,
    parameter type axi_comp_w_chan_t = logic,
    parameter type axi_comp_b_chan_t = logic,
    parameter type axi_comp_ar_chan_t = logic,
    parameter type axi_comp_r_chan_t = logic,
    parameter type axi_comp_req_t = logic,
    parameter type axi_comp_resp_t = logic,

    // Data Structures Interface data types
    parameter type axi_ds_req_t = logic,
    parameter type axi_ds_resp_t = logic
) (
    input  logic clk_i,
    input  logic rst_ni,

    input logic [1:0] fullbuffer_jobs_i,
    input logic [1:0] completion_ar_admit_i,
    input logic [1:0] completion_stream_admit_i,

    // AXI Translation Request Interface
    input  axi_tr_req_t    tr_axi_req_i,
    output axi_tr_resp_t   tr_axi_resp_o,

    // AXI Translation Completion Interface
    input  axi_comp_resp_t comp_axi_resp_i,
    output axi_comp_req_t  comp_axi_req_o,

    // MRIF Handler AXI IF (disabled / tied to zero)
    output axi_ds_req_t    mrif_axi_req_o,
    input  axi_ds_resp_t   mrif_axi_resp_i,

    // Translation engine request port
    output trans_req_data_t     trans_req_data_o,
    output logic                trans_req_valid_o,
    input  logic                trans_req_ready_i,

    // Translation engine response port
    input  trans_resp_data_t    trans_resp_data_i,
    input  logic                trans_resp_valid_i,
    output logic                trans_resp_ready_o,

    // MRIF Handler fault reporting port (disabled / tied to zero)
    output fault_data_t         mrif_fault_data_o,
    output logic                mrif_fault_valid_o,
    input  logic                mrif_fault_ready_i,

    output logic in_flight_o,
    output rv_iommu::e3_ctl_t e3_o
);

    //=========================================================================
    // Parameter validation
    //=========================================================================
    // Immediate, Verilator-safe fatal checks.  `BankIdBit < AxiIdWidth` is the
    // "unknown bank bit" assertion required by the architecture.
    initial begin : validate_params
        if (RVIOMMUCfg.AxiIdWidth < 1) begin
            $fatal(1, "rv_iommu_banked_ctl: AxiIdWidth must be >= 1");
        end
        if ((BankIdBit < 0) || (BankIdBit >= RVIOMMUCfg.AxiIdWidth)) begin
            $fatal(1, "rv_iommu_banked_ctl: BankIdBit (%0d) must be < AxiIdWidth (%0d)",
                   BankIdBit, RVIOMMUCfg.AxiIdWidth);
        end
        if (RVIOMMUCfg.MSITrans == rv_iommu_cfg::MSI_BT_MRIF) begin
            $fatal(1, "rv_iommu_banked_ctl: MRIF is unsupported");
        end
        if (RVIOMMUCfg.InclATS) begin
            $fatal(1, "rv_iommu_banked_ctl: InclATS must be disabled");
        end
        if ((RVIOMMUCfg.NumOutstandingTrans < 2) || ((RVIOMMUCfg.NumOutstandingTrans % 2) != 0)) begin
            $fatal(1, "rv_iommu_banked_ctl: NumOutstandingTrans must be even and >= 2");
        end
        if ((RVIOMMUCfg.WFifoDepth < 2) || ((RVIOMMUCfg.WFifoDepth % 2) != 0)) begin
            $fatal(1, "rv_iommu_banked_ctl: WFifoDepth must be even and >= 2");
        end
    end

    //=========================================================================
    // Per-bank configuration: NumOutstandingTrans and WFifoDepth are SPLIT,
    // everything else (caches, fault/MRIF FIFOs, widths, ...) is unchanged.
    // MSITrans is preserved (no MRIF); InclATS is forced low.
    //=========================================================================
    localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfgBank = rv_iommu_cfg::rv_iommu_cfg_t'{
        NumOutstandingTrans : RVIOMMUCfg.NumOutstandingTrans / 2,
        WFifoDepth          : RVIOMMUCfg.WFifoDepth / 2,
        MrifFifoDepth       : RVIOMMUCfg.MrifFifoDepth,
        FaultFifoDepth      : RVIOMMUCfg.FaultFifoDepth,
        NumIotlbEntries     : RVIOMMUCfg.NumIotlbEntries,
        NumDdtcEntries      : RVIOMMUCfg.NumDdtcEntries,
        NumPdtcEntries      : RVIOMMUCfg.NumPdtcEntries,
        NumMrifcEntries     : RVIOMMUCfg.NumMrifcEntries,
        InclPC              : RVIOMMUCfg.InclPC,
        InclAxiBC           : RVIOMMUCfg.InclAxiBC,
        InclDbg             : RVIOMMUCfg.InclDbg,
        InclATS             : 1'b0,
        MSITrans            : RVIOMMUCfg.MSITrans,
        IGS                 : RVIOMMUCfg.IGS,
        NumIntVec           : RVIOMMUCfg.NumIntVec,
        NumHpmCounters      : RVIOMMUCfg.NumHpmCounters,
        PAddrWidth          : RVIOMMUCfg.PAddrWidth,
        AxiAddrWidth        : RVIOMMUCfg.AxiAddrWidth,
        AxiDataWidth        : RVIOMMUCfg.AxiDataWidth,
        AxiIdWidth          : RVIOMMUCfg.AxiIdWidth,
        AxiProgIdWidth      : RVIOMMUCfg.AxiProgIdWidth,
        AxiUserWidth        : RVIOMMUCfg.AxiUserWidth,
        AxiProIdWidth       : RVIOMMUCfg.AxiProIdWidth,
        AxiDevIdWidth       : RVIOMMUCfg.AxiDevIdWidth,
        AxisDataWidth       : RVIOMMUCfg.AxisDataWidth,
        AxisDestWidth       : RVIOMMUCfg.AxisDestWidth,
        AxisUserWidth       : RVIOMMUCfg.AxisUserWidth,
        AxisStrbWidth       : RVIOMMUCfg.AxisStrbWidth,
        AxisKeepWidth       : RVIOMMUCfg.AxisKeepWidth,
        AxisIdWidth         : RVIOMMUCfg.AxisIdWidth,
        Freq                : RVIOMMUCfg.Freq
    };

    //=========================================================================
    // Internal (ORIGINAL_ID_WIDTH + 1) completion channel types for the mux
    // master port.  Only the ID field differs from the external completion
    // types; the bank bit is appended as the MSB by `axi_mux`/`axi_id_prepend`.
    //=========================================================================
    typedef logic [RVIOMMUCfg.AxiIdWidth:0]       comp_id_wide_t;
    typedef logic [RVIOMMUCfg.AxiAddrWidth-1:0]   comp_addr_t;
    typedef logic [RVIOMMUCfg.AxiDataWidth-1:0]   comp_data_t;
    typedef logic [RVIOMMUCfg.AxiDataWidth/8-1:0] comp_strb_t;
    typedef logic [RVIOMMUCfg.AxiUserWidth-1:0]   comp_user_t;

    `AXI_TYPEDEF_AW_CHAN_T(comp_aw_chan_wide_t, comp_addr_t, comp_id_wide_t, comp_user_t)
    `AXI_TYPEDEF_W_CHAN_T (comp_w_chan_t,        comp_data_t, comp_strb_t,    comp_user_t)
    `AXI_TYPEDEF_B_CHAN_T (comp_b_chan_wide_t,   comp_id_wide_t, comp_user_t)
    `AXI_TYPEDEF_AR_CHAN_T(comp_ar_chan_wide_t,  comp_addr_t, comp_id_wide_t, comp_user_t)
    `AXI_TYPEDEF_R_CHAN_T (comp_r_chan_wide_t,   comp_data_t, comp_id_wide_t, comp_user_t)
    `AXI_TYPEDEF_REQ_T (comp_req_wide_t,  comp_aw_chan_wide_t, comp_w_chan_t, comp_ar_chan_wide_t)
    `AXI_TYPEDEF_RESP_T(comp_resp_wide_t, comp_b_chan_wide_t, comp_r_chan_wide_t)

    //=========================================================================
    // Cross-bank wiring
    //=========================================================================
    axi_tr_req_t    [1:0] bank_tr_req;
    axi_tr_resp_t   [1:0] bank_tr_resp;

    axi_comp_req_t  [1:0] bank_comp_req;
    axi_comp_resp_t [1:0] bank_comp_resp;

    trans_req_data_t  [1:0] bank_trans_req_data;
    logic             [1:0] bank_trans_req_valid;
    logic             [1:0] bank_trans_req_ready;

    trans_resp_data_t [1:0] bank_trans_resp_data;
    logic             [1:0] bank_trans_resp_valid;
    logic             [1:0] bank_trans_resp_ready;

    logic             [1:0] bank_busy;

    //=========================================================================
    // Incoming bank select demux
    //   * select is the IMMUTABLE ORIGINAL AXI ID bit BankIdBit (AW / AR)
    //   * no extra spills, no ID modification (IDs stay ORIGINAL width)
    //   * MaxTrans = original NumOutstandingTrans*2  (documented tracker
    //     capacity for the two banks together), UniqueIds = 0
    //=========================================================================
    logic aw_sel;
    logic ar_sel;
    assign aw_sel = tr_axi_req_i.aw.id[BankIdBit];
    assign ar_sel = tr_axi_req_i.ar.id[BankIdBit];

    // The input demux keeps W ownership until LAST. A source read from the
    // other bank can otherwise fill its DMA's small R->W FIFO while its AW
    // cannot enter that demux. The shared memory R channel then cannot return
    // the current owner's source data or page-table reads: a circular wait.
    // Delay SELECTING such reads, rather than withdrawing an AXI/TL VALID.
    // Writes and already-selected transactions retain their original protocol.
    // A visible new AW also reserves admission before its input handshake.
    // W may legally finish before a stalled AW is accepted, hence the signed
    // balance (minimum -1 for that reserved head), rather than an unsigned
    // AW-minus-LAST counter that could underflow.
    logic signed [31:0] w_unfilled_q;
    logic w_owner_q, read_owner, restrict_reads;
    logic [1:0] bank_read_admit;
    assign restrict_reads = (w_unfilled_q > 0) || tr_axi_req_i.aw_valid;
    assign read_owner = (w_unfilled_q > 0) ? w_owner_q : aw_sel;
    // If a new AW enters this bank on this edge, its FIFO head only becomes
    // visible on the next cycle. Do not select an older queued AR one cycle
    // ahead of that AW. When the AW FIFO is full (no AW handshake), allow the
    // current owner to read and drain already-prepared writes instead.
    assign bank_read_admit[0] = ((w_unfilled_q > 0) ? !read_owner : !restrict_reads)
                                && !(aw_accept && !aw_sel);
    assign bank_read_admit[1] = ((w_unfilled_q > 0) ?  read_owner : !restrict_reads)
                                && !(aw_accept &&  aw_sel);
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            w_unfilled_q <= '0;
            w_owner_q <= 1'b0;
        end else begin
            w_unfilled_q <= w_unfilled_q + (aw_accept ? 32'sd1 : 32'sd0)
                                           - (w_last_accept ? 32'sd1 : 32'sd0);
            if (aw_accept && w_unfilled_q <= 0) w_owner_q <= aw_sel;
        end
    end

    axi_demux #(
        .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
        .AtopSupport    (1'b1),
        .aw_chan_t      (axi_tr_aw_chan_t),
        .w_chan_t       (axi_tr_w_chan_t),
        .b_chan_t       (axi_tr_b_chan_t),
        .ar_chan_t      (axi_tr_ar_chan_t),
        .r_chan_t       (axi_tr_r_chan_t),
        .axi_req_t      (axi_tr_req_t),
        .axi_resp_t     (axi_tr_resp_t),
        .NoMstPorts     (32'd2),
        .MaxTrans       (RVIOMMUCfg.NumOutstandingTrans * 2),
        .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
        .UniqueIds      (1'b0),
        .SpillAw        (1'b0),
        .SpillW         (1'b0),
        .SpillB         (1'b0),
        .SpillAr        (1'b0),
        .SpillR         (1'b0)
    ) i_in_axi_demux (
        .clk_i,
        .rst_ni,
        .test_i          (1'b0),
        .slv_req_i       (tr_axi_req_i),
        .slv_aw_select_i (aw_sel),
        .slv_ar_select_i (ar_sel),
        .slv_resp_o      (tr_axi_resp_o),
        .mst_reqs_o      (bank_tr_req),
        .mst_resps_i     (bank_tr_resp)
    );

    //=========================================================================
    // Two transaction controller banks with shared read admission
    //=========================================================================
    for (genvar b = 0; b < 2; b++) begin : gen_bank
        rv_iommu_trans_ctl_wrap #(
            .RVIOMMUCfg         (RVIOMMUCfgBank),
            .E3Enable           (1'b0),   // legacy E3 counters are per-owner only
            .ReadAdmissionEnable(!CompletionGateEnable),
            .FullBufferDID      (24'(10 + b)),
            .addr_t             (addr_t),
            .paddr_t            (paddr_t),
            .trans_req_data_t   (trans_req_data_t),
            .trans_resp_data_t  (trans_resp_data_t),
            .fault_data_t       (fault_data_t),
            .axi_tr_aw_chan_t   (axi_tr_aw_chan_t),
            .axi_tr_w_chan_t    (axi_tr_w_chan_t),
            .axi_tr_ar_chan_t   (axi_tr_ar_chan_t),
            .axi_tr_req_t       (axi_tr_req_t),
            .axi_tr_resp_t      (axi_tr_resp_t),
            .axi_comp_aw_chan_t (axi_comp_aw_chan_t),
            .axi_comp_w_chan_t  (axi_comp_w_chan_t),
            .axi_comp_b_chan_t  (axi_comp_b_chan_t),
            .axi_comp_ar_chan_t (axi_comp_ar_chan_t),
            .axi_comp_r_chan_t  (axi_comp_r_chan_t),
            .axi_comp_req_t     (axi_comp_req_t),
            .axi_comp_resp_t    (axi_comp_resp_t),
            .axi_ds_req_t       (axi_ds_req_t),
            .axi_ds_resp_t      (axi_ds_resp_t)
        ) i_bank (
            .clk_i,
            .rst_ni,
            .read_admit_i       (bank_read_admit[b]),
            .fullbuffer_job_i   (fullbuffer_jobs_i[b]),
            .tr_axi_req_i       (bank_tr_req[b]),
            .tr_axi_resp_o      (bank_tr_resp[b]),
            .comp_axi_resp_i    (bank_comp_resp[b]),
            .comp_axi_req_o     (bank_comp_req[b]),
            .mrif_axi_resp_i    (mrif_axi_resp_i),
            .trans_req_data_o   (bank_trans_req_data[b]),
            .trans_req_valid_o  (bank_trans_req_valid[b]),
            .trans_req_ready_i  (bank_trans_req_ready[b]),
            .trans_resp_data_i  (bank_trans_resp_data[b]),
            .trans_resp_valid_i (bank_trans_resp_valid[b]),
            .trans_resp_ready_o (bank_trans_resp_ready[b]),
            .mrif_fault_ready_i (mrif_fault_ready_i),
            .in_flight_o        (bank_busy[b])
        );
    end

    // MRIF is disabled: the single wrapper MRIF port is tied to zero.
    assign mrif_axi_req_o     = '0;
    assign mrif_fault_data_o  = '0;
    assign mrif_fault_valid_o = 1'b0;

    //=========================================================================
    // Translation request merge (round-robin, stable while valid)
    //   The bank-local request is copied and ONLY `.bank` is overwritten, so
    //   every other header field (iova/did/pid/ttype/...) is preserved bit-exact.
    //=========================================================================
    trans_req_data_t [1:0] bank_req_data;

    always_comb begin : bank_req_tag
        bank_req_data[0]       = bank_trans_req_data[0];
        bank_req_data[0].bank  = 1'b0;
        bank_req_data[1]       = bank_trans_req_data[1];
        bank_req_data[1].bank  = 1'b1;
    end

    stream_arbiter #(
        .DATA_T  (trans_req_data_t),
        .N_INP   (2),
        .ARBITER ("rr")
    ) i_trans_req_arb (
        .clk_i,
        .rst_ni,
        .inp_data_i  (bank_req_data),
        .inp_valid_i (bank_trans_req_valid),
        .inp_ready_o (bank_trans_req_ready),
        .oup_data_o  (trans_req_data_o),
        .oup_valid_o (trans_req_valid_o),
        .oup_ready_i (trans_req_ready_i)
    );

    //=========================================================================
    // Translation response routing by `.bank`
    //   Both banks see the same response payload (the existing controller
    //   ignores the bank field); only the matching bank sees `valid`.
    //   `trans_resp_ready_o` is the ready of that same bank.
    //=========================================================================
    always_comb begin : bank_resp_route
        bank_trans_resp_data[0] = trans_resp_data_i;
        bank_trans_resp_data[1] = trans_resp_data_i;
        bank_trans_resp_valid[0] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b0);
        bank_trans_resp_valid[1] = trans_resp_valid_i && (trans_resp_data_i.bank == 1'b1);
    end

    assign trans_resp_ready_o = trans_resp_data_i.bank ? bank_trans_resp_ready[1]
                                                       : bank_trans_resp_ready[0];

    //=========================================================================
    // Outgoing completion mux
    //   slave ports: ORIGINAL completion types / ORIGINAL ID width
    //   master port: ORIGINAL_ID_WIDTH + 1 (bank bit prepended as MSB)
    //=========================================================================
    axi_comp_req_t [1:0] admitted_comp_req;
    axi_comp_resp_t [1:0] admitted_comp_resp;
    axi_comp_ar_chan_t header_candidate [2][2];
    logic [1:0][1:0] header_candidate_valid, header_candidate_fire;
    logic header_busy;
    logic [31:0] hp_parked, hp_bypassed, hp_full_bypass, hp_highwater, hp_queued, hp_dispatched;
    if (HeaderParkMode == 0) begin : gen_original_completion
        always_comb begin
            admitted_comp_req = bank_comp_req;
            bank_comp_resp = admitted_comp_resp;
            header_candidate_valid = '0; header_candidate_fire = '0;
            for (int b = 0; b < 2; b++) begin
                for (int d = 0; d < 2; d++) header_candidate[b][d] = '0;
                if (CompletionGateEnable) begin
                    admitted_comp_req[b].ar_valid = bank_comp_req[b].ar_valid && completion_ar_admit_i[b];
                    bank_comp_resp[b].ar_ready = admitted_comp_resp[b].ar_ready && completion_ar_admit_i[b];
                end
            end
        end
        assign header_busy = 0;
        assign hp_parked = 0; assign hp_bypassed = 0; assign hp_full_bypass = 0;
        assign hp_highwater = 0; assign hp_queued = 0; assign hp_dispatched = 0;
    end else begin : gen_header_park
        localparam int Slots = HeaderParkMode == 2 ? 2 * HeaderParkDepth : HeaderParkDepth;
        axi_comp_ar_chan_t slot_header_q [2][Slots];
        logic slot_valid_q [2][Slots];
        logic [31:0] slot_seq_q [2][Slots];
        logic [31:0] accepted_q [2][2], sent_q [2][2];
        logic locked_q [2], locked_stream_q [2], rr_q [2];
        integer locked_slot_q [2];
        integer oldest [2][2], free_slot [2], chosen [2], chosen_slot [2], occupancy [2];
        logic dispatch [2], direct_selected [2], park_fire [2];
        logic [31:0] parked_q [2], bypass_q [2], full_bypass_q [2], highwater_q [2], dispatched_q [2];
        // Assertion history is test-only, not an additional architectural slot.
        logic held_q [2];
        axi_comp_ar_chan_t held_header_q [2];

        initial begin
            if (!CompletionGateEnable || HeaderParkMode > 2 || HeaderParkDepth < 1)
                $fatal(1, "header parking requires completion gate, mode1/2, depth>=1");
        end

        always_comb begin
            admitted_comp_req = bank_comp_req;
            bank_comp_resp = admitted_comp_resp;
            header_candidate_valid = '0; header_candidate_fire = '0;
            header_busy = 0;
            hp_queued = 0; hp_highwater = 0;
            hp_parked = parked_q[0] + parked_q[1];
            hp_bypassed = bypass_q[0] + bypass_q[1];
            hp_full_bypass = full_bypass_q[0] + full_bypass_q[1];
            hp_dispatched = dispatched_q[0] + dispatched_q[1];
            for (int b = 0; b < 2; b++) begin
                free_slot[b] = -1; occupancy[b] = 0;
                for (int d = 0; d < 2; d++) begin
                    oldest[b][d] = -1; header_candidate[b][d] = '0;
                    for (int n = 0; n < Slots; n++) begin
                        if (slot_valid_q[b][n] && int'(slot_header_q[b][n].id[2]) == d) begin
                            if (oldest[b][d] < 0 || slot_seq_q[b][n] < slot_seq_q[b][oldest[b][d]])
                                oldest[b][d] = n;
                        end
                    end
                    if (oldest[b][d] >= 0) begin
                        header_candidate[b][d] = slot_header_q[b][oldest[b][d]];
                        header_candidate_valid[b][d] = 1;
                    end else if (HeaderParkMode == 1 && HeaderDirectEnable &&
                        bank_comp_req[b].ar_valid && int'(bank_comp_req[b].ar.id[2]) == d) begin
                        // Never pass an earlier queued header with the same ID.
                        header_candidate[b][d] = bank_comp_req[b].ar;
                        header_candidate_valid[b][d] = 1;
                    end
                end
                for (int n = 0; n < Slots; n++) begin
                    occupancy[b] += int'(slot_valid_q[b][n]);
                    if (!slot_valid_q[b][n] && free_slot[b] < 0 &&
                        (HeaderParkMode == 1 || n / HeaderParkDepth == int'(bank_comp_req[b].ar.id[2])))
                        free_slot[b] = n;
                end
                chosen[b] = -1; chosen_slot[b] = -1;
                if (locked_q[b]) begin
                    chosen[b] = int'(locked_stream_q[b]);
                    chosen_slot[b] = locked_slot_q[b];
                end else begin
                    for (int k = 0; k < 2; k++) begin
                        int d;
                        d = (int'(rr_q[b]) + k) % 2;
                        if (chosen[b] < 0 && header_candidate_valid[b][d] && completion_stream_admit_i[d]) begin
                            chosen[b] = d; chosen_slot[b] = oldest[b][d];
                        end
                    end
                end
                admitted_comp_req[b].ar_valid = chosen[b] >= 0;
                admitted_comp_req[b].ar = '0;
                if (chosen[b] >= 0) admitted_comp_req[b].ar = header_candidate[b][chosen[b]];
                dispatch[b] = admitted_comp_req[b].ar_valid && admitted_comp_resp[b].ar_ready;
                direct_selected[b] = chosen[b] >= 0 && chosen_slot[b] < 0;
                // A selected direct header stays at the TC spill until dispatch.
                bank_comp_resp[b].ar_ready = direct_selected[b] ? admitted_comp_resp[b].ar_ready : free_slot[b] >= 0;
                park_fire[b] = bank_comp_req[b].ar_valid && bank_comp_resp[b].ar_ready && !direct_selected[b];
                if (dispatch[b]) header_candidate_fire[b][chosen[b]] = 1;
                hp_queued += 32'(occupancy[b]);
                if (highwater_q[b] > hp_highwater) hp_highwater = highwater_q[b];
                header_busy |= occupancy[b] != 0 || locked_q[b];
            end
        end

        always_ff @(posedge clk_i or negedge rst_ni) begin
            if (!rst_ni) begin
                for (int b = 0; b < 2; b++) begin
                    locked_q[b] <= 0; locked_stream_q[b] <= 0; locked_slot_q[b] <= -1;
                    rr_q[b] <= 0; held_q[b] <= 0; held_header_q[b] <= '0;
                    parked_q[b] <= 0; bypass_q[b] <= 0; full_bypass_q[b] <= 0;
                    highwater_q[b] <= 0; dispatched_q[b] <= 0;
                    for (int d = 0; d < 2; d++) begin accepted_q[b][d] <= 0; sent_q[b][d] <= 0; end
                    for (int n = 0; n < Slots; n++) begin
                        slot_valid_q[b][n] <= 0; slot_header_q[b][n] <= '0; slot_seq_q[b][n] <= 0;
                    end
                end
            end else begin
                for (int b = 0; b < 2; b++) begin
                    if (held_q[b] && (!admitted_comp_req[b].ar_valid || admitted_comp_req[b].ar != held_header_q[b]))
                        $fatal(1, "parked AR changed while VALID stalled");
                    held_q[b] <= admitted_comp_req[b].ar_valid && !admitted_comp_resp[b].ar_ready;
                    held_header_q[b] <= admitted_comp_req[b].ar;
                    if (admitted_comp_req[b].ar_valid && !admitted_comp_resp[b].ar_ready) begin
                        locked_q[b] <= 1; locked_stream_q[b] <= 1'(chosen[b]); locked_slot_q[b] <= chosen_slot[b];
                    end
                    if (dispatch[b]) begin
                        locked_q[b] <= 0; rr_q[b] <= !1'(chosen[b]);
                        dispatched_q[b] <= dispatched_q[b] + 1;
                        sent_q[b][chosen[b]] <= sent_q[b][chosen[b]] + 1;
                        if (chosen_slot[b] >= 0) begin
                            if (slot_seq_q[b][chosen_slot[b]] != sent_q[b][chosen[b]])
                                $fatal(1, "same-ID dispatch order violated");
                            slot_valid_q[b][chosen_slot[b]] <= 0;
                        end else begin
                            if (accepted_q[b][chosen[b]] != sent_q[b][chosen[b]])
                                $fatal(1, "direct header passed earlier same-ID request");
                            bypass_q[b] <= bypass_q[b] + 1;
                            if (occupancy[b] == Slots) full_bypass_q[b] <= full_bypass_q[b] + 1;
                        end
                    end
                    if (bank_comp_req[b].ar_valid && bank_comp_resp[b].ar_ready) begin
                        int d;
                        d = int'(bank_comp_req[b].ar.id[2]);
                        // This first PoC supports exactly original IDs0/4.
                        if (bank_comp_req[b].ar.id != RVIOMMUCfg.AxiIdWidth'(d * 4))
                            $fatal(1, "header PoC supports two constant DMA read IDs only");
                        accepted_q[b][d] <= accepted_q[b][d] + 1;
                        if (park_fire[b]) begin
                            slot_valid_q[b][free_slot[b]] <= 1;
                            slot_header_q[b][free_slot[b]] <= bank_comp_req[b].ar;
                            slot_seq_q[b][free_slot[b]] <= accepted_q[b][d];
                            parked_q[b] <= parked_q[b] + 1;
                        end
                    end
                    if (32'(occupancy[b] + int'(park_fire[b]) - int'(dispatch[b] && !direct_selected[b])) > highwater_q[b])
                        highwater_q[b] <= 32'(occupancy[b] + int'(park_fire[b]) - int'(dispatch[b] && !direct_selected[b]));
                end
            end
        end
    end

    comp_req_wide_t  mux_mst_req;
    comp_resp_wide_t mux_mst_resp;

    axi_mux #(
        .SlvAxiIDWidth (RVIOMMUCfg.AxiIdWidth),
        .slv_aw_chan_t (axi_comp_aw_chan_t),
        .mst_aw_chan_t (comp_aw_chan_wide_t),
        .w_chan_t      (axi_comp_w_chan_t),
        .slv_b_chan_t  (axi_comp_b_chan_t),
        .mst_b_chan_t  (comp_b_chan_wide_t),
        .slv_ar_chan_t (axi_comp_ar_chan_t),
        .mst_ar_chan_t (comp_ar_chan_wide_t),
        .slv_r_chan_t  (axi_comp_r_chan_t),
        .mst_r_chan_t  (comp_r_chan_wide_t),
        .slv_req_t     (axi_comp_req_t),
        .slv_resp_t    (axi_comp_resp_t),
        .mst_req_t     (comp_req_wide_t),
        .mst_resp_t    (comp_resp_wide_t),
        .NoSlvPorts    (32'd2),
        .MaxWTrans     (RVIOMMUCfg.NumOutstandingTrans),
        .FallThrough   (1'b0),
        .SpillAw       (1'b0),
        .SpillW        (1'b0),
        .SpillB        (1'b0),
        .SpillAr       (1'b0),
        .SpillR        (1'b0)
    ) i_out_axi_mux (
        .clk_i,
        .rst_ni,
        .test_i      (1'b0),
        .slv_reqs_i  (admitted_comp_req),
        .slv_resps_o (admitted_comp_resp),
        .mst_req_o   (mux_mst_req),
        .mst_resp_i  (mux_mst_resp)
    );

    //-------------------------------------------------------------------------
    // Master -> external request conversion.
    //   Explicit PER-FIELD conversion.  AW/AR drop the redundant MSB bank bit;
    //   every other header/user field is transferred field by field (no struct
    //   cast between different widths, no implicit truncation).
    //-------------------------------------------------------------------------
    always_comb begin : out_req_convert
        comp_axi_req_o = '0;

        comp_axi_req_o.aw_valid  = mux_mst_req.aw_valid;
        comp_axi_req_o.aw.id     = mux_mst_req.aw.id[RVIOMMUCfg.AxiIdWidth-1:0];
        comp_axi_req_o.aw.addr   = mux_mst_req.aw.addr;
        comp_axi_req_o.aw.len    = mux_mst_req.aw.len;
        comp_axi_req_o.aw.size   = mux_mst_req.aw.size;
        comp_axi_req_o.aw.burst  = mux_mst_req.aw.burst;
        comp_axi_req_o.aw.lock   = mux_mst_req.aw.lock;
        comp_axi_req_o.aw.cache  = mux_mst_req.aw.cache;
        comp_axi_req_o.aw.prot   = mux_mst_req.aw.prot;
        comp_axi_req_o.aw.qos    = mux_mst_req.aw.qos;
        comp_axi_req_o.aw.region = mux_mst_req.aw.region;
        comp_axi_req_o.aw.atop   = mux_mst_req.aw.atop;
        comp_axi_req_o.aw.user   = mux_mst_req.aw.user;

        comp_axi_req_o.w_valid   = mux_mst_req.w_valid;
        comp_axi_req_o.w.data    = mux_mst_req.w.data;
        comp_axi_req_o.w.strb    = mux_mst_req.w.strb;
        comp_axi_req_o.w.last    = mux_mst_req.w.last;
        comp_axi_req_o.w.user    = mux_mst_req.w.user;

        comp_axi_req_o.b_ready   = mux_mst_req.b_ready;

        comp_axi_req_o.ar_valid  = mux_mst_req.ar_valid;
        comp_axi_req_o.ar.id     = mux_mst_req.ar.id[RVIOMMUCfg.AxiIdWidth-1:0];
        comp_axi_req_o.ar.addr   = mux_mst_req.ar.addr;
        comp_axi_req_o.ar.len    = mux_mst_req.ar.len;
        comp_axi_req_o.ar.size   = mux_mst_req.ar.size;
        comp_axi_req_o.ar.burst  = mux_mst_req.ar.burst;
        comp_axi_req_o.ar.lock   = mux_mst_req.ar.lock;
        comp_axi_req_o.ar.cache  = mux_mst_req.ar.cache;
        comp_axi_req_o.ar.prot   = mux_mst_req.ar.prot;
        comp_axi_req_o.ar.qos    = mux_mst_req.ar.qos;
        comp_axi_req_o.ar.region = mux_mst_req.ar.region;
        comp_axi_req_o.ar.user   = mux_mst_req.ar.user;

        comp_axi_req_o.r_ready   = mux_mst_req.r_ready;
    end

    //-------------------------------------------------------------------------
    // External -> master response conversion.
    //   B/R reconstruct the redundant MSB bank bit from the ORIGINAL
    //   `id[BankIdBit]`; every other field is transferred field by field.
    //-------------------------------------------------------------------------
    always_comb begin : in_resp_convert
        mux_mst_resp = '0;

        mux_mst_resp.aw_ready = comp_axi_resp_i.aw_ready;
        mux_mst_resp.ar_ready = comp_axi_resp_i.ar_ready;
        mux_mst_resp.w_ready  = comp_axi_resp_i.w_ready;

        mux_mst_resp.b_valid  = comp_axi_resp_i.b_valid;
        mux_mst_resp.b.id     = {comp_axi_resp_i.b.id[BankIdBit], comp_axi_resp_i.b.id};
        mux_mst_resp.b.resp   = comp_axi_resp_i.b.resp;
        mux_mst_resp.b.user   = comp_axi_resp_i.b.user;

        mux_mst_resp.r_valid  = comp_axi_resp_i.r_valid;
        mux_mst_resp.r.id     = {comp_axi_resp_i.r.id[BankIdBit], comp_axi_resp_i.r.id};
        mux_mst_resp.r.data   = comp_axi_resp_i.r.data;
        mux_mst_resp.r.resp   = comp_axi_resp_i.r.resp;
        mux_mst_resp.r.last   = comp_axi_resp_i.r.last;
        mux_mst_resp.r.user   = comp_axi_resp_i.r.user;
    end

    //=========================================================================
    // in_flight_o
    //   Aggregates, over both banks:
    //     * AW accepted at the upstream input until the upstream B handshake
    //       (covers the whole write, including completion AW/AR accepted by the
    //       output and held downstream, i.e. "accepted output pressure")
    //     * AR accepted until the upstream R-LAST handshake
    //     * W-LAST bursts accepted at the upstream input until the upstream B
    //       handshake.  A write is counted by its LAST beat and is NOT tracked
    //       by downstream forwarding: a write rejected by a bank (translation
    //       error) is consumed internally by that bank's error slave and
    //       answered with an upstream B, but it is NEVER forwarded to the
    //       completion port.  A forward-based W counter would therefore never
    //       decrement for those beats and in_flight_o would stay stuck high
    //       forever (phantom non-quiet).
    //     * w_partial_q: set when a W burst has started (a non-LAST beat was
    //       accepted upstream) and cleared when its LAST beat is accepted;
    //       covers the window between the first beat of a burst and its LAST.
    //     * both bank controller states (bank in_flight_o = state != IDLE)
    //   32-bit counters, asserted bounded / no underflow.  Increment and
    //   decrement on the same cycle are netted, so they can never produce a
    //   false underflow or a spurious overflow report.
    //=========================================================================
    logic aw_accept, ar_accept, w_accept, w_nonlast_accept, w_last_accept,
          b_return, r_last_return;

    assign aw_accept        = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
    assign ar_accept        = tr_axi_req_i.ar_valid && tr_axi_resp_o.ar_ready;
    assign w_accept         = tr_axi_req_i.w_valid  && tr_axi_resp_o.w_ready;
    assign w_nonlast_accept = w_accept && !tr_axi_req_i.w.last;
    assign w_last_accept    = w_accept &&  tr_axi_req_i.w.last;
    assign b_return         = tr_axi_resp_o.b_valid && tr_axi_req_i.b_ready;
    assign r_last_return    = tr_axi_resp_o.r_valid && tr_axi_req_i.r_ready && tr_axi_resp_o.r.last;

    logic [31:0] inflight_aw_q, inflight_ar_q, inflight_w_q;
    logic        w_partial_q;

    always_ff @(posedge clk_i or negedge rst_ni) begin : inflight_counters
        if (!rst_ni) begin
            inflight_aw_q <= 32'd0;
            inflight_ar_q <= 32'd0;
            inflight_w_q  <= 32'd0;
            w_partial_q   <= 1'b0;
        end else begin
            inflight_aw_q <= inflight_aw_q
                           + (aw_accept     ? 32'd1 : 32'd0)
                           - (b_return      ? 32'd1 : 32'd0);
            inflight_ar_q <= inflight_ar_q
                           + (ar_accept     ? 32'd1 : 32'd0)
                           - (r_last_return ? 32'd1 : 32'd0);
            inflight_w_q  <= inflight_w_q
                           + (w_last_accept ? 32'd1 : 32'd0)
                           - (b_return      ? 32'd1 : 32'd0);
            w_partial_q   <= w_last_accept    ? 1'b0 :
                             w_nonlast_accept ? 1'b1 :
                             w_partial_q;
        end
    end

    assign in_flight_o = (inflight_aw_q != 32'd0) ||
                         (inflight_ar_q != 32'd0) ||
                         (inflight_w_q  != 32'd0) ||
                         w_partial_q ||
                         bank_busy[0] || bank_busy[1] || header_busy;

    // Bounded / no-underflow checks on the 32-bit in-flight counters.
    // A decrement is only a real underflow when there is no matching increment
    // in the same cycle; an increment only overflows when it is not cancelled
    // by a decrement in the same cycle.
    `ifndef SYNTHESIS
    always_ff @(posedge clk_i) begin : inflight_bounds
        if (rst_ni) begin
            if ((inflight_ar_q == 32'd0) && r_last_return && !ar_accept) begin
                $error("rv_iommu_banked_ctl: inflight_ar underflow");
            end
            if ((inflight_aw_q == 32'd0) && b_return && !aw_accept) begin
                $error("rv_iommu_banked_ctl: inflight_aw underflow");
            end
            if ((inflight_w_q == 32'd0) && b_return && !w_last_accept) begin
                $error("rv_iommu_banked_ctl: inflight_w underflow");
            end
            if ((inflight_aw_q == 32'hFFFF_FFFF) && aw_accept && !b_return) begin
                $error("rv_iommu_banked_ctl: inflight_aw counter overflow");
            end
            if ((inflight_ar_q == 32'hFFFF_FFFF) && ar_accept && !r_last_return) begin
                $error("rv_iommu_banked_ctl: inflight_ar counter overflow");
            end
            if ((inflight_w_q == 32'hFFFF_FFFF) && w_last_accept && !b_return) begin
                $error("rv_iommu_banked_ctl: inflight_w counter overflow");
            end
        end
    end
    `endif

    //=========================================================================
    // Legacy E3 observer: INCOMPATIBLE with two owners.
    // Only `.quiet` (== !in_flight_o) is meaningful.  All other fields are zero.
    // No legacy E3 observer may be attached to this instance.
    //=========================================================================
    always_comb begin : legacy_e3
        e3_o       = '0;
        e3_o.quiet = !in_flight_o;
    end

endmodule
