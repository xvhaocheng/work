# Banked transaction-controller wrapper — proposed code

Everything below is a **proposal**. Nothing was executed, compiled or simulated. All file paths / module names in the dependency list are taken from the sources you supplied (Bender.yml, the two AXI files, the common_cells `stream_arbiter`, the IOMMU type-pkg) and are explicitly flagged where I could not confirm them from the supplied material.

---

## 1. New RTL — `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv`

```systemverilog
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
//   This module wires two UNMODIFIED `rv_iommu_trans_ctl_wrap` instances
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
//     own `axi_err_slv`).  No arbitration behaviour of the original controller
//     is modified.  The original `rv_iommu_trans_ctl_wrap.sv` is NOT touched.
//   * MRIF is required to be disabled (`MSITrans == MSI_DISABLED`); the single
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

module rv_iommu_banked_ctl #(
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
        if (RVIOMMUCfg.MSITrans != rv_iommu_cfg::MSI_DISABLED) begin
            $fatal(1, "rv_iommu_banked_ctl: MSITrans must be MSI_DISABLED");
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
    // MSITrans is forced MSI_DISABLED and InclATS is forced low.
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
        MSITrans            : rv_iommu_cfg::MSI_DISABLED,
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
    // Two unmodified transaction controller banks
    //=========================================================================
    for (genvar b = 0; b < 2; b++) begin : gen_bank
        rv_iommu_trans_ctl_wrap #(
            .RVIOMMUCfg         (RVIOMMUCfgBank),
            .E3Enable           (1'b0),   // legacy E3 counters are per-owner only
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
        .slv_reqs_i  (bank_comp_req),
        .slv_resps_o (bank_comp_resp),
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
    //     * W beats accepted at the upstream input until forwarded to the
    //       completion port (covers partial W arriving before its AW and W
    //       beats parked in the per-bank W FIFOs)
    //     * both bank controller states (bank in_flight_o = state != IDLE)
    //   32-bit saturating-free counters, asserted bounded / no underflow.
    //=========================================================================
    logic aw_accept, ar_accept, w_accept, b_return, r_last_return, w_forward;

    assign aw_accept     = tr_axi_req_i.aw_valid && tr_axi_resp_o.aw_ready;
    assign ar_accept     = tr_axi_req_i.ar_valid && tr_axi_resp_o.ar_ready;
    assign w_accept      = tr_axi_req_i.w_valid  && tr_axi_resp_o.w_ready;
    assign b_return      = tr_axi_resp_o.b_valid && tr_axi_req_i.b_ready;
    assign r_last_return = tr_axi_resp_o.r_valid && tr_axi_req_i.r_ready && tr_axi_resp_o.r.last;
    assign w_forward     = comp_axi_req_o.w_valid && comp_axi_resp_i.w_ready;

    logic [31:0] inflight_aw_q, inflight_ar_q, inflight_w_q;

    always_ff @(posedge clk_i or negedge rst_ni) begin : inflight_counters
        if (!rst_ni) begin
            inflight_aw_q <= 32'd0;
            inflight_ar_q <= 32'd0;
            inflight_w_q  <= 32'd0;
        end else begin
            inflight_aw_q <= inflight_aw_q
                           + (aw_accept    ? 32'd1 : 32'd0)
                           - (b_return     ? 32'd1 : 32'd0);
            inflight_ar_q <= inflight_ar_q
                           + (ar_accept    ? 32'd1 : 32'd0)
                           - (r_last_return ? 32'd1 : 32'd0);
            inflight_w_q  <= inflight_w_q
                           + (w_accept     ? 32'd1 : 32'd0)
                           - (w_forward    ? 32'd1 : 32'd0);
        end
    end

    assign in_flight_o = (inflight_aw_q != 32'd0) ||
                         (inflight_ar_q != 32'd0) ||
                         (inflight_w_q  != 32'd0) ||
                         bank_busy[0] || bank_busy[1];

    // Bounded / no-underflow checks on the 32-bit in-flight counters.
    always_ff @(posedge clk_i) begin : inflight_bounds
        if (rst_ni) begin
            if ((inflight_ar_q == 32'd0) && r_last_return) begin
                $error("rv_iommu_banked_ctl: inflight_ar underflow");
            end
            if ((inflight_aw_q == 32'd0) && b_return) begin
                $error("rv_iommu_banked_ctl: inflight_aw underflow");
            end
            if ((inflight_w_q == 32'd0) && w_forward) begin
                $error("rv_iommu_banked_ctl: inflight_w underflow");
            end
            if (inflight_aw_q == 32'hFFFF_FFFF) begin
                $error("rv_iommu_banked_ctl: inflight_aw counter overflow");
            end
            if (inflight_ar_q == 32'hFFFF_FFFF) begin
                $error("rv_iommu_banked_ctl: inflight_ar counter overflow");
            end
            if (inflight_w_q == 32'hFFFF_FFFF) begin
                $error("rv_iommu_banked_ctl: inflight_w counter overflow");
            end
        end
    end

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
```

---

## 2. Test top — `tests/rv_iommu_banked_ctl_tb_top.sv`

> This is an **actual-RTL wrapper-contract test only**. The "translation engine" below is an explicit stub that only echoes `.bank` and adds a constant to the IOVA. It is **not** a TL/PTW verification and must never be presented as one.

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Wrapper-contract test for rv_iommu_banked_ctl.
//
//   * The "translation engine" is an explicit TEST STUB for the wrapper
//     contract only: it echoes `.bank` and performs spaddr = iova + 0x1000.
//     It models NO page-table walk and NO two-stage translation.
//   * Stimulus, checking and the completion-side model all live here; the C++
//     harness only clocks, resets and reads the verdict.
//
// CHECK code map ($display "CHECK FAIL code=<n>"):
//    1..  9  S1 read bank1: comp AR id/addr, upstream R id/resp, tr log bank/iova/ttype/did
//   10      S1 drain quiet
//   20..29  S2 write bank0: comp AW id/addr, W beat order/payload/last, upstream B
//   30..39  S3 different-bank ordering while bank0 response held
//   40..49  S4 same-bank same-ID FIFO order
//   50..59  S5 permission error routing
//   60..79  S6 output backpressure stability + per-bank FIFO capacity
//   80..89  S7 final drain quiet / totals

`include "axi/typedef.svh"
`include "axi-iommu/typedef.svh"

module rv_iommu_banked_ctl_tb_top #(
    parameter int unsigned AXI_ID_W    = 8,
    parameter int unsigned AXI_ADDR_W  = 64,
    parameter int unsigned AXI_DATA_W  = 32,
    parameter int unsigned AXI_USER_W  = 2,
    parameter int unsigned BANK_ID_BIT = 3
) (
    input  logic        clk_i,
    input  logic        rst_ni,
    output logic        test_done_o,
    output logic [31:0] test_pass_cnt_o,
    output logic [31:0] test_fail_cnt_o
);

    //-------------------------------------------------------------------------
    // Configuration: original capacities are 4/4 → bank capacities 2/2
    //-------------------------------------------------------------------------
    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = '{
        NumOutstandingTrans : 32'd4,
        WFifoDepth          : 32'd4,
        MrifFifoDepth       : 32'd2,
        FaultFifoDepth      : 32'd2,
        NumIotlbEntries     : 32'd4,
        NumDdtcEntries      : 32'd4,
        NumPdtcEntries      : 32'd4,
        NumMrifcEntries     : 32'd4,
        InclPC              : 1'b0,
        InclAxiBC           : 1'b0,                       // keep stimulus addresses simple
        InclDbg             : 1'b0,
        InclATS             : 1'b0,
        MSITrans            : rv_iommu_cfg::MSI_DISABLED,
        IGS                 : rv_iommu_cfg::WSI_ONLY,
        NumIntVec           : 32'd1,
        NumHpmCounters      : 32'd0,
        PAddrWidth          : 32'd44,
        AxiAddrWidth        : AXI_ADDR_W,
        AxiDataWidth        : AXI_DATA_W,
        AxiIdWidth          : AXI_ID_W,
        AxiProgIdWidth      : 32'd8,
        AxiUserWidth        : AXI_USER_W,
        AxiProIdWidth       : 32'd4,
        AxiDevIdWidth       : 32'd4,
        AxisDataWidth       : 32'd32,
        AxisDestWidth       : 32'd4,
        AxisUserWidth       : 32'd2,
        AxisStrbWidth       : 32'd4,
        AxisKeepWidth       : 32'd4,
        AxisIdWidth         : 32'd8,
        Freq                : 32'd100
    };

    //-------------------------------------------------------------------------
    // Types
    //-------------------------------------------------------------------------
    typedef logic [AXI_ID_W-1:0]        tr_id_t;
    typedef logic [AXI_ADDR_W-1:0]      tr_addr_t;
    typedef logic [AXI_DATA_W-1:0]      tr_data_t;
    typedef logic [AXI_DATA_W/8-1:0]    tr_strb_t;
    typedef logic [AXI_USER_W-1:0]      tr_user_t;
    typedef logic [23:0]                tr_sid_t;
    typedef logic                       tr_ssidv_t;
    typedef logic [19:0]                tr_ssid_t;
    typedef logic [43:0]                paddr_t;

    `AXI_TYPEDEF_AW_CHAN_EXT_T(tr_aw_chan_t, tr_addr_t, tr_id_t, tr_user_t, tr_sid_t, tr_ssidv_t, tr_ssid_t)
    `AXI_TYPEDEF_AR_CHAN_EXT_T(tr_ar_chan_t, tr_addr_t, tr_id_t, tr_user_t, tr_sid_t, tr_ssidv_t, tr_ssid_t)
    `AXI_TYPEDEF_W_CHAN_T(tr_w_chan_t, tr_data_t, tr_strb_t, tr_user_t)
    `AXI_TYPEDEF_B_CHAN_T(tr_b_chan_t, tr_id_t, tr_user_t)
    `AXI_TYPEDEF_R_CHAN_T(tr_r_chan_t, tr_data_t, tr_id_t, tr_user_t)
    `AXI_TYPEDEF_REQ_EXT_T(tr_req_t, tr_aw_chan_t, tr_w_chan_t, tr_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(tr_resp_t, tr_b_chan_t, tr_r_chan_t)

    `AXI_TYPEDEF_ALL_CT(comp, comp_req_t, comp_resp_t, tr_addr_t, tr_id_t, tr_data_t, tr_strb_t, tr_user_t)
    `AXI_TYPEDEF_ALL_CT(ds,   ds_req_t,   ds_resp_t,   tr_addr_t, tr_id_t, tr_data_t, tr_strb_t, tr_user_t)

    typedef struct packed {
        logic             bank;
        logic [63:0]      iova;
        logic [23:0]      did;
        logic             pid_valid;
        logic [19:0]      pid;
        rv_iommu::ttype_t ttype;
        logic             priv;
        logic             is_debug;
    } trans_req_data_t;

    typedef struct packed {
        logic                     bank;
        logic [63:0]              spaddr;
        logic                     error;
        logic                     is_mrif;
        logic                     ignore;
        rv_iommu::mrifc_content_t mrif_data;
    } trans_resp_data_t;

    typedef struct packed {
        logic [63:0]      iotval2;
        logic [63:0]      iotval;
        logic [23:0]      did;
        rv_iommu::ttype_t ttyp;
        logic             priv;
        logic             pv;
        logic [19:0]      pid;
        rv_iommu::cause_t cause;
    } fault_data_t;

    //-------------------------------------------------------------------------
    // DUT
    //-------------------------------------------------------------------------
    axi_tr_req_t    tr_req;
    axi_tr_resp_t   tr_resp;
    axi_comp_req_t  comp_req;
    axi_comp_resp_t comp_resp;
    ds_req_t        mrif_req;
    ds_resp_t       mrif_resp;
    fault_data_t    mrif_fault_data;
    logic           mrif_fault_valid;
    logic           mrif_fault_ready;

    trans_req_data_t  dut_treq_data;
    logic             dut_treq_valid;
    logic             dut_treq_ready;
    trans_resp_data_t dut_tresp_data;
    logic             dut_tresp_valid;
    logic             dut_tresp_ready;
    logic             dut_in_flight;
    rv_iommu::e3_ctl_t e3;

    rv_iommu_banked_ctl #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .BankIdBit          (BANK_ID_BIT),
        .addr_t             (tr_addr_t),
        .paddr_t            (paddr_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .fault_data_t       (fault_data_t),
        .axi_tr_aw_chan_t   (tr_aw_chan_t),
        .axi_tr_w_chan_t    (tr_w_chan_t),
        .axi_tr_b_chan_t    (tr_b_chan_t),
        .axi_tr_r_chan_t    (tr_r_chan_t),
        .axi_tr_ar_chan_t   (tr_ar_chan_t),
        .axi_tr_req_t       (tr_req_t),
        .axi_tr_resp_t      (tr_resp_t),
        .axi_comp_aw_chan_t (comp_aw_chan_t),
        .axi_comp_w_chan_t  (comp_w_chan_t),
        .axi_comp_b_chan_t  (comp_b_chan_t),
        .axi_comp_ar_chan_t (comp_ar_chan_t),
        .axi_comp_r_chan_t  (comp_r_chan_t),
        .axi_comp_req_t     (comp_req_t),
        .axi_comp_resp_t    (comp_resp_t),
        .axi_ds_req_t       (ds_req_t),
        .axi_ds_resp_t      (ds_resp_t)
    ) i_dut (
        .clk_i,
        .rst_ni,
        .tr_axi_req_i       (tr_req),
        .tr_axi_resp_o      (tr_resp),
        .comp_axi_resp_i    (comp_resp),
        .comp_axi_req_o     (comp_req),
        .mrif_axi_req_o     (mrif_req),
        .mrif_axi_resp_i    (mrif_resp),
        .trans_req_data_o   (dut_treq_data),
        .trans_req_valid_o  (dut_treq_valid),
        .trans_req_ready_i  (dut_treq_ready),
        .trans_resp_data_i  (dut_tresp_data),
        .trans_resp_valid_i (dut_tresp_valid),
        .trans_resp_ready_o (dut_tresp_ready),
        .mrif_fault_data_o  (mrif_fault_data),
        .mrif_fault_valid_o (mrif_fault_valid),
        .mrif_fault_ready_i (mrif_fault_ready),
        .in_flight_o        (dut_in_flight),
        .e3_o               (e3)
    );

    //=========================================================================
    // Translation engine stub (CONTRACT MODEL ONLY - not TL/PTW)
    //=========================================================================
    logic [1:0]      eng_busy;
    logic [1:0]      eng_hold;
    logic [1:0]      eng_rvalid;
    logic [1:0]      eng_rready;
    logic [31:0]     eng_cnt   [2];
    int unsigned     eng_delay [2];
    trans_resp_data_t eng_resp [2];

    logic        eng_accept;
    logic [0:0]  eng_accept_bank;

    assign eng_accept      = dut_treq_valid && dut_treq_ready;
    assign eng_accept_bank = dut_treq_data.bank;
    assign dut_treq_ready  = dut_treq_data.bank ? !eng_busy[1] : !eng_busy[0];

    // response merge (bank0 priority; only one response is ever presented)
    assign dut_tresp_data  = eng_rvalid[0] ? eng_resp[0] : eng_resp[1];
    assign dut_tresp_valid = eng_rvalid[0] | eng_rvalid[1];
    assign eng_rready[0]   = dut_tresp_ready &&  eng_rvalid[0];
    assign eng_rready[1]   = dut_tresp_ready && !eng_rvalid[0];

    for (genvar b = 0; b < 2; b++) begin : gen_eng_bank
        localparam logic [0:0] THIS_BANK = (b == 0) ? 1'b0 : 1'b1;

        always_ff @(posedge clk_i or negedge rst_ni) begin
            if (!rst_ni) begin
                eng_busy[b]   <= 1'b0;
                eng_rvalid[b] <= 1'b0;
                eng_cnt[b]    <= '0;
                eng_resp[b]   <= '0;
            end else begin
                if (eng_accept && (eng_accept_bank == THIS_BANK)) begin
                    eng_busy[b]              <= 1'b1;
                    eng_rvalid[b]            <= 1'b0;
                    eng_cnt[b]               <= eng_delay[b];
                    eng_resp[b].bank         <= THIS_BANK;
                    eng_resp[b].spaddr       <= dut_treq_data.iova + 64'h1000;
                    eng_resp[b].error        <= (dut_treq_data.iova >= 64'h0000_0000_DEAD_0000) &&
                                                (dut_treq_data.iova <  64'h0000_0000_DEAD_1000);
                    eng_resp[b].is_mrif      <= 1'b0;
                    eng_resp[b].ignore       <= 1'b0;
                    eng_resp[b].mrif_data    <= '0;
                end else if (eng_busy[b]) begin
                    if (eng_cnt[b] != '0) begin
                        eng_cnt[b]    <= eng_cnt[b] - 1;
                        eng_rvalid[b] <= 1'b0;
                    end else if (!eng_hold[b]) begin
                        eng_rvalid[b] <= 1'b1;
                    end else begin
                        eng_rvalid[b] <= 1'b0;
                    end
                    if (eng_rvalid[b] && eng_rready[b]) begin
                        eng_rvalid[b] <= 1'b0;
                        eng_busy[b]   <= 1'b0;
                    end
                end
            end
        end
    end

    // translation request log
    logic [63:0] tr_log_iova  [0:31];
    logic [23:0] tr_log_did   [0:31];
    logic        tr_log_bank  [0:31];
    logic [5:0]  tr_log_ttype [0:31];
    int unsigned tr_log_cnt;

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            tr_log_cnt <= 0;
        end else if (eng_accept && (tr_log_cnt < 32)) begin
            tr_log_iova [tr_log_cnt] <= dut_treq_data.iova;
            tr_log_did  [tr_log_cnt] <= dut_treq_data.did;
            tr_log_bank [tr_log_cnt] <= dut_treq_data.bank;
            tr_log_ttype[tr_log_cnt] <= dut_treq_data.ttype;
            tr_log_cnt               <= tr_log_cnt + 1;
        end
    end

    //=========================================================================
    // Completion-side responder (test model of the completion slave)
    //=========================================================================
    logic comp_ar_stall, comp_aw_stall, comp_w_stall;

    // read side
    logic       c_r_pend, c_r_valid;
    logic [7:0] c_r_left, c_r_beat, c_r_id;
    comp_r_chan_t c_r_chan;

    // write side
    logic       c_b_pend, c_b_valid, c_w_done;
    logic [7:0] c_b_id;
    comp_b_chan_t c_b_chan;

    // logs
    logic [7:0]  c_ar_log_id   [0:31];
    logic [63:0] c_ar_log_addr [0:31];
    int unsigned c_ar_log_cnt;
    logic [7:0]  c_aw_log_id   [0:31];
    logic [63:0] c_aw_log_addr [0:31];
    int unsigned c_aw_log_cnt;
    logic [31:0] c_w_log_data  [0:31];
    logic        c_w_log_last  [0:31];
    int unsigned c_w_log_cnt;

    assign comp_resp.aw_ready = (!comp_aw_stall) && (!c_b_pend) && (!c_w_done);
    assign comp_resp.ar_ready = (!comp_ar_stall) && (!c_r_pend);
    assign comp_resp.w_ready  = !comp_w_stall;

    always_comb begin : comp_resp_drive
        comp_resp = '0;
        comp_resp.aw_ready = (!comp_aw_stall) && (!c_b_pend) && (!c_w_done);
        comp_resp.ar_ready = (!comp_ar_stall) && (!c_r_pend);
        comp_resp.w_ready  = !comp_w_stall;
        comp_resp.b_valid  = c_b_valid;
        comp_resp.b        = c_b_chan;
        comp_resp.r_valid  = c_r_valid;
        comp_resp.r        = c_r_chan;
    end

    always_comb begin : comp_b_chan_build
        c_b_chan      = '0;
        c_b_chan.id   = c_b_id;
        c_b_chan.resp = axi_pkg::RESP_OKAY;
        c_b_chan.user = '0;
    end

    always_comb begin : comp_r_chan_build
        c_r_chan       = '0;
        c_r_chan.id    = c_r_id;
        c_r_chan.data  = 32'hC0DE_0000 | {24'd0, c_r_beat};
        c_r_chan.resp  = axi_pkg::RESP_OKAY;
        c_r_chan.last  = (c_r_left <= 8'd1);
        c_r_chan.user  = '0;
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin : comp_read_fsm
        if (!rst_ni) begin
            c_r_pend  <= 1'b0;
            c_r_valid <= 1'b0;
            c_r_left  <= '0;
            c_r_beat  <= '0;
            c_r_id    <= '0;
        end else begin
            if (comp_req.ar_valid && comp_resp.ar_ready) begin
                c_r_pend <= 1'b1;
                c_r_left <= comp_req.ar.len + 8'd1;
                c_r_beat <= 8'd0;
                c_r_id   <= comp_req.ar.id;
            end
            if (!c_r_valid && c_r_pend) begin
                c_r_valid <= 1'b1;
            end
            if (c_r_valid && comp_req.r_ready) begin
                if (c_r_left <= 8'd1) begin
                    c_r_valid <= 1'b0;
                    c_r_pend  <= 1'b0;
                end else begin
                    c_r_left <= c_r_left - 8'd1;
                    c_r_beat <= c_r_beat + 8'd1;
                end
            end
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin : comp_write_fsm
        if (!rst_ni) begin
            c_b_pend  <= 1'b0;
            c_b_valid <= 1'b0;
            c_w_done  <= 1'b0;
            c_b_id    <= '0;
        end else begin
            if (comp_req.aw_valid && comp_resp.aw_ready) begin
                c_b_pend <= 1'b1;
                c_b_id   <= comp_req.aw.id;
                c_w_done <= 1'b0;
            end
            if (comp_req.w_valid && comp_resp.w_ready && comp_req.w.last) begin
                c_w_done <= 1'b1;
            end
            if (!c_b_valid && c_b_pend && c_w_done) begin
                c_b_valid <= 1'b1;
            end
            if (c_b_valid && comp_req.b_ready) begin
                c_b_valid <= 1'b0;
                c_b_pend  <= 1'b0;
                c_w_done  <= 1'b0;
            end
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin : comp_logs
        if (!rst_ni) begin
            c_ar_log_cnt <= 0;
            c_aw_log_cnt <= 0;
            c_w_log_cnt  <= 0;
        end else begin
            if (comp_req.ar_valid && comp_resp.ar_ready && (c_ar_log_cnt < 32)) begin
                c_ar_log_id  [c_ar_log_cnt] <= comp_req.ar.id;
                c_ar_log_addr[c_ar_log_cnt] <= comp_req.ar.addr;
                c_ar_log_cnt                <= c_ar_log_cnt + 1;
            end
            if (comp_req.aw_valid && comp_resp.aw_ready && (c_aw_log_cnt < 32)) begin
                c_aw_log_id  [c_aw_log_cnt] <= comp_req.aw.id;
                c_aw_log_addr[c_aw_log_cnt] <= comp_req.aw.addr;
                c_aw_log_cnt                <= c_aw_log_cnt + 1;
            end
            if (comp_req.w_valid && comp_resp.w_ready && (c_w_log_cnt < 32)) begin
                c_w_log_data[c_w_log_cnt] <= comp_req.w.data;
                c_w_log_last[c_w_log_cnt] <= comp_req.w.last;
                c_w_log_cnt               <= c_w_log_cnt + 1;
            end
        end
    end

    //=========================================================================
    // Upstream response logs (B / R returned to the requester)
    //=========================================================================
    logic [7:0]  up_r_log_id   [0:31];
    logic [1:0]  up_r_log_resp [0:31];
    int unsigned up_r_log_cnt;
    logic [7:0]  up_b_log_id   [0:31];
    int unsigned up_b_log_cnt;

    always_ff @(posedge clk_i or negedge rst_ni) begin : upstream_logs
        if (!rst_ni) begin
            up_r_log_cnt <= 0;
            up_b_log_cnt <= 0;
        end else begin
            if (tr_resp.r_valid && tr_req.r_ready && (up_r_log_cnt < 32)) begin
                up_r_log_id  [up_r_log_cnt] <= tr_resp.r.id;
                up_r_log_resp[up_r_log_cnt] <= tr_resp.r.resp;
                up_r_log_cnt                <= up_r_log_cnt + 1;
            end
            if (tr_resp.b_valid && tr_req.b_ready && (up_b_log_cnt < 32)) begin
                up_b_log_id[up_b_log_cnt] <= tr_resp.b.id;
                up_b_log_cnt              <= up_b_log_cnt + 1;
            end
        end
    end

    //=========================================================================
    // Handshake pulses (combinational, sampled at the active edge)
    //=========================================================================
    logic aw_fire, ar_fire, w_fire;
    assign aw_fire = tr_req.aw_valid && tr_resp.aw_ready;
    assign ar_fire = tr_req.ar_valid && tr_resp.ar_ready;
    assign w_fire  = tr_req.w_valid  && tr_resp.w_ready;

    //=========================================================================
    // Verdict
    //=========================================================================
    int unsigned n_pass, n_fail;
    logic        done;

    always_comb begin : verdict
        test_pass_cnt_o = n_pass;
        test_fail_cnt_o = n_fail;
        test_done_o     = done;
    end

    task automatic chk(input logic cond, input int unsigned code);
        begin
            if (cond) begin
                n_pass = n_pass + 1;
            end else begin
                n_fail = n_fail + 1;
                $display("[%0t] CHECK FAIL code=%0d", $time, code);
            end
        end
    endtask

    //-------------------------------------------------------------------------
    // AXI stimulus helpers (drive on negedge, sample handshake at posedge)
    //-------------------------------------------------------------------------
    task automatic issue_ar(input logic [7:0] id, input logic [63:0] addr, input logic [23:0] did);
        begin
            @(negedge clk_i);
            tr_req.ar           = '0;
            tr_req.ar.id        = id;
            tr_req.ar.addr      = addr;
            tr_req.ar.len       = 8'd0;
            tr_req.ar.size      = 3'd2;
            tr_req.ar.burst     = axi_pkg::BURST_INCR;
            tr_req.ar.prot      = 3'b000;
            tr_req.ar.stream_id = did;
            tr_req.ar_valid     = 1'b1;
            @(posedge clk_i);
            while (!ar_fire) @(posedge clk_i);
            @(negedge clk_i);
            tr_req.ar_valid = 1'b0;
        end
    endtask

    task automatic issue_aw(input logic [7:0] id, input logic [63:0] addr,
                            input logic [23:0] did, input logic [7:0] len);
        begin
            @(negedge clk_i);
            tr_req.aw           = '0;
            tr_req.aw.id        = id;
            tr_req.aw.addr      = addr;
            tr_req.aw.len       = len;
            tr_req.aw.size      = 3'd2;
            tr_req.aw.burst     = axi_pkg::BURST_INCR;
            tr_req.aw.prot      = 3'b000;
            tr_req.aw.stream_id = did;
            tr_req.aw_valid     = 1'b1;
            @(posedge clk_i);
            while (!aw_fire) @(posedge clk_i);
            @(negedge clk_i);
            tr_req.aw_valid = 1'b0;
        end
    endtask

    task automatic issue_w(input logic [31:0] data, input logic [3:0] strb, input logic last);
        begin
            @(negedge clk_i);
            tr_req.w       = '0;
            tr_req.w.data  = data;
            tr_req.w.strb  = strb;
            tr_req.w.last  = last;
            tr_req.w_valid = 1'b1;
            @(posedge clk_i);
            while (!w_fire) @(posedge clk_i);
            @(negedge clk_i);
            tr_req.w_valid = 1'b0;
        end
    endtask

    //=========================================================================
    // Test sequence
    //=========================================================================
    int unsigned ar_before;
    int unsigned r_before;
    logic [7:0]  snap_ar_id;
    logic [63:0] snap_ar_addr;
    int unsigned found;

    initial begin : main
        tr_req           = '0;
        tr_req.b_ready   = 1'b1;
        tr_req.r_ready   = 1'b1;
        tr_req.w         = '0;
        comp_ar_stall    = 1'b0;
        comp_aw_stall    = 1'b0;
        comp_w_stall     = 1'b0;
        eng_hold[0]      = 1'b0;
        eng_hold[1]      = 1'b0;
        eng_delay[0]     = 1;
        eng_delay[1]     = 1;
        mrif_resp        = '0;
        mrif_fault_ready = 1'b0;
        n_pass           = 0;
        n_fail           = 0;
        done             = 1'b0;

        while (!rst_ni) @(negedge clk_i);
        repeat (4) @(negedge clk_i);

        //----------------------------------------------------------------
        // S1 : read on bank 1 (id[3]=1), header / ID / address preservation
        //----------------------------------------------------------------
        issue_ar(8'h0B, 64'h0000_0000_1000_0000, 24'h00_0011);
        while (c_ar_log_cnt < 1) @(negedge clk_i);
        chk(c_ar_log_id[0]   == 8'h0B, 1);
        chk(c_ar_log_addr[0] == 64'h0000_0000_1000_1000, 2);
        while (up_r_log_cnt < 1) @(negedge clk_i);
        chk(up_r_log_id[0]   == 8'h0B, 3);
        chk(up_r_log_resp[0] == axi_pkg::RESP_OKAY, 4);
        chk(tr_log_cnt       == 1, 5);
        chk(tr_log_bank[0]   == 1'b1, 6);
        chk(tr_log_iova[0]   == 64'h0000_0000_1000_0000, 7);
        chk(tr_log_ttype[0]  == rv_iommu::UNTRANSLATED_R, 8);
        chk(tr_log_did[0]    == 24'h00_0011, 9);
        repeat (8) @(negedge clk_i);
        chk(dut_in_flight    == 1'b0, 10);

        //----------------------------------------------------------------
        // S2 : write burst on bank 0 (id[3]=0), AW then W beats with
        //      distinct payload, then B
        //----------------------------------------------------------------
        issue_aw(8'h01, 64'h0000_0000_2000_0000, 24'h00_0042, 8'd1);
        issue_w (32'hAAAA_0001, 4'hF, 1'b0);
        issue_w (32'hAAAA_0002, 4'hF, 1'b1);
        while (c_aw_log_cnt < 1) @(negedge clk_i);
        chk(c_aw_log_id[0]   == 8'h01, 20);
        chk(c_aw_log_addr[0] == 64'h0000_0000_2000_1000, 21);
        while (c_w_log_cnt < 2) @(negedge clk_i);
        chk(c_w_log_data[0]  == 32'hAAAA_0001, 22);
        chk(c_w_log_last[0]  == 1'b0, 23);
        chk(c_w_log_data[1]  == 32'hAAAA_0002, 24);
        chk(c_w_log_last[1]  == 1'b1, 25);
        while (up_b_log_cnt < 1) @(negedge clk_i);
        chk(up_b_log_id[0]   == 8'h01, 26);
        repeat (8) @(negedge clk_i);
        chk(dut_in_flight    == 1'b0, 27);

        ar_before = c_ar_log_cnt;

        //----------------------------------------------------------------
        // S3 : different-bank younger request accepted / translated /
        //      outgoing BEFORE the held slow response of bank 0
        //----------------------------------------------------------------
        eng_hold[0] = 1'b1;
        issue_ar(8'h02, 64'h0000_0000_3000_0000, 24'h60);
        while (tr_log_cnt < 1) @(negedge clk_i);             // bank0 translation accepted
        issue_ar(8'h0C, 64'h0000_0000_4000_0000, 24'h61);    // younger, bank 1
        while (c_ar_log_cnt < (ar_before + 1)) @(negedge clk_i);
        chk(c_ar_log_id[ar_before]   == 8'h0C, 30);          // bank1 left first
        chk(c_ar_log_addr[ar_before] == 64'h0000_0000_4000_1000, 31);
        chk(c_ar_log_cnt == (ar_before + 1), 32);            // bank0 still held
        eng_hold[0] = 1'b0;                                  // release slow response
        while (c_ar_log_cnt < (ar_before + 2)) @(negedge clk_i);
        chk(c_ar_log_id[ar_before+1]   == 8'h02, 33);
        chk(c_ar_log_addr[ar_before+1] == 64'h0000_0000_3000_1000, 34);
        while (up_r_log_cnt < 3) @(negedge clk_i);
        repeat (8) @(negedge clk_i);

        ar_before = c_ar_log_cnt;

        //----------------------------------------------------------------
        // S4 : same bank, same ID, FIFO order
        //----------------------------------------------------------------
        issue_ar(8'h03, 64'h0000_0000_5000_0000, 24'h70);
        issue_ar(8'h03, 64'h0000_0000_5000_1000, 24'h70);
        while (c_ar_log_cnt < (ar_before + 2)) @(negedge clk_i);
        chk(c_ar_log_id[ar_before]     == 8'h03, 40);
        chk(c_ar_log_addr[ar_before]   == 64'h0000_0000_5000_1000, 41);
        chk(c_ar_log_id[ar_before+1]   == 8'h03, 42);
        chk(c_ar_log_addr[ar_before+1] == 64'h0000_0000_5000_2000, 43);
        while (up_r_log_cnt < 5) @(negedge clk_i);
        repeat (8) @(negedge clk_i);

        //----------------------------------------------------------------
        // S5 : permission error routing (translation response error)
        //      completion interface must NOT see it, requester gets SLVERR
        //----------------------------------------------------------------
        ar_before = c_ar_log_cnt;
        r_before  = up_r_log_cnt;
        issue_ar(8'h0D, 64'h0000_0000_DEAD_0000, 24'h55);
        while (up_r_log_cnt <= r_before) @(negedge clk_i);
        chk(up_r_log_id[r_before]   == 8'h0D, 50);
        chk(up_r_log_resp[r_before] == axi_pkg::RESP_SLVERR, 51);
        chk(c_ar_log_cnt == ar_before, 52);
        repeat (8) @(negedge clk_i);

        //----------------------------------------------------------------
        // S6 : per-bank header capacity + output backpressure stability
        //      original total capacity 4 == 2 (bank0) + 2 (bank1)
        //----------------------------------------------------------------
        ar_before = c_ar_log_cnt;
        comp_ar_stall = 1'b1;                          // completion port stalls
        issue_ar(8'h04, 64'h0000_0000_6000_0000, 24'h80);   // bank0 #1
        issue_ar(8'h05, 64'h0000_0000_6000_1000, 24'h80);   // bank0 #2
        issue_ar(8'h1C, 64'h0000_0000_7000_0000, 24'h81);   // bank1 #1
        issue_ar(8'h1D, 64'h0000_0000_7000_1000, 24'h81);   // bank1 #2

        // a 5th header to bank0 must not be accepted (bank FIFO is full)
        @(negedge clk_i);
        tr_req.ar       = '0;
        tr_req.ar.id    = 8'h06;
        tr_req.ar.addr  = 64'h0000_0000_6000_2000;
        tr_req.ar.len   = 8'd0;
        tr_req.ar.size  = 3'd2;
        tr_req.ar.burst = axi_pkg::BURST_INCR;
        tr_req.ar_valid = 1'b1;
        repeat (6) begin
            @(posedge clk_i);
            chk(!ar_fire, 60);
        end
        @(negedge clk_i);
        tr_req.ar_valid = 1'b0;

        // backpressure stability of the completion request bus
        @(negedge clk_i);
        chk(comp_req.ar_valid == 1'b1, 61);
        snap_ar_id   = comp_req.ar.id;
        snap_ar_addr = comp_req.ar.addr;
        repeat (6) @(negedge clk_i);
        chk(comp_req.ar_valid == 1'b1, 62);
        chk((comp_req.ar.id == snap_ar_id) && (comp_req.ar.addr == snap_ar_addr), 63);

        comp_ar_stall = 1'b0;
        repeat (60) @(negedge clk_i);
        chk(c_ar_log_cnt == (ar_before + 4), 64);
        chk(dut_in_flight == 1'b0, 65);

        //----------------------------------------------------------------
        // S7 : final totals / drain quiet
        //----------------------------------------------------------------
        chk(c_ar_log_cnt == 9, 80);
        chk(c_aw_log_cnt == 1, 81);
        chk(up_r_log_cnt == 9, 82);
        chk(up_b_log_cnt == 1, 83);
        chk(dut_in_flight == 1'b0, 84);
        // legacy E3: only quiet is meaningful
        chk(e3.quiet == 1'b1, 85);

        repeat (4) @(negedge clk_i);
        done = 1'b1;
        $display("[%0t] rv_iommu_banked_ctl_tb_top: PASS=%0d FAIL=%0d",
                 $time, n_pass, n_fail);
    end

endmodule
```

---

## 3. C++ harness — `tests/rv_iommu_banked_ctl_test.cpp`

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Verilator harness for the banked transaction controller wrapper test.
// The stimulus/scoreboard live in rv_iommu_banked_ctl_tb_top.sv; this harness
// only drives the clock and reset and reports the verdict.

#include "Vrv_iommu_banked_ctl_tb_top.h"
#include "verilated.h"

#include <cstdio>

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    Vrv_iommu_banked_ctl_tb_top* top = new Vrv_iommu_banked_ctl_tb_top;

    top->clk_i  = 0;
    top->rst_ni = 0;

    const int MAX_CYCLES = 20000;
    int cycles = 0;

    for (int i = 0; i < MAX_CYCLES; i++) {
        top->clk_i = 1;
        top->eval();
        top->clk_i = 0;
        top->eval();

        if (i == 8) {
            top->rst_ni = 1;
        }
        cycles = i;
        if (top->test_done_o) {
            break;
        }
    }

    std::printf("rv_iommu_banked_ctl_test: cycles=%d pass=%u fail=%u done=%u\n",
                cycles,
                (unsigned)top->test_pass_cnt_o,
                (unsigned)top->test_fail_cnt_o,
                (unsigned)top->test_done_o);

    int rc = 0;
    if (!top->test_done_o) {
        std::printf("rv_iommu_banked_ctl_test: TIMEOUT\n");
        rc = 2;
    } else if (top->test_fail_cnt_o != 0) {
        rc = 1;
    }

    delete top;
    return rc;
}
```

---

## 4. Python runner — `tests/test_rv_iommu_banked_ctl.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the rv_iommu_banked_ctl wrapper-contract test.

This test exercises the *wrapper* contract of the banked transaction
controller (bank selection by the immutable ORIGINAL AXI ID bit, per-bank
ID/address/header preservation, AW/W ordering, B/R bank routing, completion
backpressure stability, per-bank/total header capacity and drain quiet).

The downstream "translation engine" is an explicit test stub inside
tests/rv_iommu_banked_ctl_tb_top.sv: it only echoes `.bank` and adds a fixed
offset to the IOVA.  This test therefore says NOTHING about TL / PTW / address
translation correctness.

Requires Verilator >= 4.038 (only 4.038-compatible constructs are used).
"""

import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
AXI    = IOMMU / "vendor" / "axi"
CC     = IOMMU / "vendor" / "common_cells"
OUT    = ROOT / "out" / "banked-ctl"

TOP_MODULE = "rv_iommu_banked_ctl_tb_top"


def tail(path: Path, lines: int = 160) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main() -> int:
    # ---- source list -------------------------------------------------------
    # NOTE: every path below is taken from the repo layout referenced by
    # cva6/corev_apu/riscv-iommu/Bender.yml (vendor/axi, vendor/common_cells,
    # include/).  Modules whose file name could not be confirmed from the
    # supplied sources are marked with "UNVERIFIED" in the comments.
    sources = dedup([
        # ---- packages (order matters) -------------------------------------
        AXI   / "src" / "axi_pkg.sv",
        IOMMU / "packages" / "rv_iommu_cfg_pkg.sv",
        IOMMU / "packages" / "rv_iommu_pkg.sv",

        # ---- common_cells -------------------------------------------------
        CC / "src" / "cf_math_pkg.sv",              # UNVERIFIED dependency of fifo_v3 / rr_arb_tree
        CC / "src" / "counter.sv",                  # used by rv_iommu_trans_ctl_wrap
        CC / "src" / "fifo_v3.sv",                  # used by axi_mux + stream_fifo
        CC / "src" / "spill_register.sv",           # used by axi_demux / axi_mux
        CC / "src" / "rr_arb_tree.sv",              # used by axi_mux / axi_demux_simple / stream_arbiter_flushable
        CC / "src" / "stream_fifo.sv",              # used by rv_iommu_trans_ctl_wrap
        CC / "src" / "stream_arbiter.sv",
        CC / "src" / "stream_arbiter_flushable.sv", # UNVERIFIED file name

        # ---- axi ----------------------------------------------------------
        AXI / "src" / "axi_id_prepend.sv",          # used by axi_mux
        AXI / "src" / "axi_demux_id_counters.sv",   # UNVERIFIED file name (used by axi_demux_simple)
        AXI / "src" / "axi_demux_simple.sv",
        AXI / "src" / "axi_demux.sv",
        AXI / "src" / "axi_mux.sv",
        AXI / "src" / "axi_err_slv.sv",

        # ---- riscv-iommu --------------------------------------------------
        IOMMU / "rtl" / "transaction_controller" / "rv_iommu_axi_bc.sv",
        IOMMU / "rtl" / "transaction_controller" / "rv_iommu_trans_ctl_wrap.sv",  # UNCHANGED
        IOMMU / "rtl" / "transaction_controller" / "rv_iommu_banked_ctl.sv",      # NEW

        # ---- testbench ----------------------------------------------------
        ROOT / "tests" / "rv_iommu_banked_ctl_tb_top.sv",
    ])
    tb = ROOT / "tests" / "rv_iommu_banked_ctl_test.cpp"

    include_dirs = [
        AXI   / "include",
        CC    / "include",
        IOMMU / "include",
    ]

    for path in sources + [tb]:
        if not path.exists():
            sys.exit("required file missing: {}".format(path))

    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = ["verilator", "--cc", "--exe", "--build",
            "-Wall", "-Wno-fatal",
            "-Wno-DECLFILENAME", "-Wno-UNUSED", "-Wno-UNDRIVEN",
            "-Wno-PINCONNECTEMPTY", "-Wno-VARHIDDEN",
            "--top-module", TOP_MODULE,
            "--Mdir", str(obj_dir)]
    for inc in include_dirs:
        args += ["-I" + str(inc)]
    args += [str(p) for p in sources]
    args += [str(tb)]

    build_log = OUT / "build.log"
    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        # Build log is only inspected on failure; never dumped otherwise.
        sys.stderr.write(tail(build_log))
        return 1

    exe = obj_dir / "V{}".format(TOP_MODULE)
    if not exe.exists():
        sys.stderr.write(tail(build_log))
        sys.exit("verilator did not produce {}".format(exe))

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
```

---

## 5. Summary of the implementation decisions

| Requirement | Where | How |
|---|---|---|
| Same params/ports as `trans_ctl_wrap` + `axi_tr_b_chan_t`, `axi_tr_r_chan_t`, `int BankIdBit` | `rv_iommu_banked_ctl` header | original parameter/port list kept verbatim, three additions |
| Two original instances, `NumOutstandingTrans`/`WFifoDepth` halved, even ≥ 2 | `RVIOMMUCfgBank` localparam + `validate_params` | named struct literal copies all other fields unchanged |
| MSI_DISABLED required, `InclATS` false, MRIF tied zero | `validate_params`, `RVIOMMUCfgBank`, `mrif_*` assigns | fatal check at elaboration; all `mrif_*` outputs zero |
| Incoming `axi_demux` selects on **original** `id[BankIdBit]`, not DID, no spills, `MaxTrans = NumOutstandingTrans*2`, `UniqueIds = false` | `i_in_axi_demux` | `aw_sel/ar_sel = tr_axi_req_i.{aw,ar}.id[BankIdBit]`; all `Spill* = 0` |
| IDs full original width in both banks | demux does not touch IDs; mux adds the bit only on its master side | — |
| Per-bank AW-before-AR/W gating and permission errors preserved | untouched bank instances (each has its own `axi_err_slv`) | — |
| Outgoing `axi_mux` 2 ports, internal ID width original+1, high bank bit | `i_out_axi_mux` with locally typedef'd wide completion types | `axi_id_prepend` inserts the MSB |
| Strip redundant high bit for AW/AR, reconstruct for B/R from external `id[BankIdBit]` | `out_req_convert` / `in_resp_convert` | field-by-field, **no cross-width struct cast** |
| `SpillAw`/`SpillAr = 0`, `MaxWTrans = NumOutstandingTrans` | `i_out_axi_mux` | — |
| Header/user fields preserved through demux and mux | by construction (only IDs are touched) | — |
| `.bank` overwritten only on the request copy; response routed by `.bank`; `trans_resp_ready_o` from the matching bank | `bank_req_tag`, `bank_resp_route` | `stream_arbiter` `"rr"`, data stable while valid |
| Existing controller ignores response `.bank` | untouched | — |
| `in_flight_o` aggregation (AW→B, AR→Rlast, W accept→forward, both controller states, incl. accepted output pressure) | `inflight_counters` + `in_flight_o` | 32-bit counters, `$error` bounded/no-underflow checks |
| `e3_o` zero except `.quiet = !in_flight_o` | `legacy_e3` | documented in header |
| BankIdBit < AxiIdWidth | `validate_params` `$fatal` | — |
| Old controller source unchanged | not touched | — |

---

## 6. Unresolved issues / risks (Codex must resolve)

1. **`axi_demux_simple` / `axi_demux_id_counters` semantics of `AxiLookBits`.** The original `trans_ctl_wrap` uses `AxiLookBits(RVIOMMUCfg.AxiIdWidth)` for its (2- or 3-port) internal demux; I mirrored that for the incoming 2-port demux. I could not confirm from the supplied files whether `AxiLookBits = AxiIdWidth` sizes a `2^AxiLookBits` lookup table (which would be area-significant) or is used differently. `AxiLookBits` was **not** specified in the task. Also unverified: whether `axi_demux_simple` has any constraint tying `MaxTrans` to `AxiLookBits`.
2. **File/module names not observable in the supplied material:** `axi_demux_simple.sv`, `axi_demux_id_counters.sv`, `axi_id_prepend.sv`, `axi_err_slv.sv`, `stream_arbiter_flushable.sv`, `fifo_v3.sv`, `spill_register.sv`, `rr_arb_tree.sv`, `cf_math_pkg.sv`. The Python runner flags them with `UNVERIFIED`; the runner exits early with a clear "required file missing" message if any path is wrong.
3. **Package dependencies of `rv_iommu_pkg.sv`.** The sibling test `tests/test_research_iotlb.py` compiles `rv_iommu_reg_pkg.sv` *before* `rv_iommu_pkg.sv`; the supplied excerpt of `rv_iommu_pkg.sv` does not reference it. If the real package pulls other packages, add them to the source list in the correct order.
4. **The translation engine must echo `.bank`.** The wrapper routes the response solely by `trans_resp_data_i.bank`. If the engine's response arrives with a stale/incorrect bank, `trans_resp_ready_o` deasserts and the transaction stalls (no mis-routing — fail-stop). This is an interface requirement on the TL.
5. **External completion slave must echo AW/AR IDs unchanged.** The B/R bank bit is reconstructed from `comp_axi_resp_i.{b,r}.id[BankIdBit]`. An interconnect that rewrites B/R IDs would break routing. This matches the task's stated contract but is worth an explicit comment in the completion-interface documentation.
6. **`axi_mux` with `Spill* = 0` is almost purely combinational.** It contains an AW-lock flop and the W select FIFO (`MaxWTrans` deep). `MaxWTrans = NumOutstandingTrans` was chosen as an upper bound on writes simultaneously inside the mux; if a different bound is wanted it is a one-line change.
7. **Assertion style.** Verilator 4.038's support for `assert` inside procedural code is version/lint-flag dependent, so the counter checks are written as immediate `$error` checks inside `always_ff` and the parameter checks as `$fatal` inside `initial`. If the flow wants real SVA, these convert 1:1 to `assert property` (and should then be wrapped so Verilator 4.038 does not have to elaborate them).
8. **Test coverage boundaries.** The test is a *wrapper-contract* test only. It does not exercise `InclAxiBC = 1` (the test sets it to 0), does not model TL/PTW, does not cover ATOP, does not cover `len > 1` reads, and does not cover `MrifFifoDepth`/fault paths. The completion-side responder serialises reads (`ar_ready = !read_pending`), which is sufficient for the exercised scenarios but is not a general AXI completion model.
9. **Testbench timing convention.** The stimulus drives on `negedge` and waits for handshakes at `posedge`; the check functions sample logs written by clocked logic. This is the standard race-free Verilator pattern, but if the local Verilator flow schedules coroutines differently, the handshake waits (`while (!ar_fire) @(posedge clk_i);`) are the first place to look.