// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Test-only composition top: instantiates rv_iommu_tl_wrap with
// PolicyEnable=1 and NonblockEnable=1, exposing the policy controls and a
// flattened IOTLB research snapshot for fillmask observation.  No production
// RTL is modified.

`include "axi/typedef.svh"

module composition_tl_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter bit NonblockEnable = 1,
    parameter bit PolicyEnable   = 1
) (
    input  logic        clk_i,
    input  logic        rst_ni,

    // Scalar request fields
    input  logic [23:0] req_did,
    input  logic [63:0] req_iova,
    input  logic [5:0]  req_ttype,
    input  logic [19:0] req_pid,
    input  logic        req_pid_valid,
    input  logic        req_priv,
    input  logic        req_is_debug,
    input  logic        req_bank,
    input  logic        trans_req_valid_i,
    output logic        trans_req_ready_o,

    // Scalar response fields
    output logic        trans_resp_valid_o,
    input  logic        trans_resp_ready_i,
    output logic        resp_bank,
    output logic        resp_error,
    output logic        resp_ignore,
    output logic [55:0] resp_spaddr,
    output logic        resp_is_mrif,
    output logic [10:0] resp_mrif_nid,
    output logic [43:0] resp_mrif_nppn,
    output logic [47:0] resp_mrif_addr,
    output logic [2:0]  resp_range,
    output logic        resp_bypass,
    output logic [11:0] resp_fault_code,
    output logic        resp_w,
    output logic        resp_r,
    output logic        resp_x,
    output logic        resp_sum,
    output logic        resp_g,
    output logic        resp_u,
    output logic        resp_t2gpa,

    // Invalidation
    input  logic        inval_vma,
    input  logic        inval_gvma,
    input  logic        inval_av,
    input  logic        inval_gv,
    input  logic        inval_pscv,
    input  logic [28:0] inval_vpn,
    input  logic [15:0] inval_gscid,
    input  logic [19:0] inval_pscid,
    input  logic        iodir_inval_ddtc,
    input  logic        iodir_inval_pdtc,
    input  logic        iodir_dv,
    input  logic [23:0] iodir_did,
    input  logic [19:0] iodir_pid,

    // DDTW AXI read port (scalar)
    output logic        ddtw_ar_valid,
    output logic [63:0] ddtw_ar_addr,
    output logic [3:0]  ddtw_ar_id,
    output logic [7:0]  ddtw_ar_len,
    output logic [2:0]  ddtw_ar_size,
    output logic [1:0]  ddtw_ar_burst,
    input  logic        ddtw_ar_ready,
    output logic        ddtw_r_ready,
    input  logic        ddtw_r_valid,
    input  logic [63:0] ddtw_r_data,
    input  logic [3:0]  ddtw_r_id,
    input  logic [1:0]  ddtw_r_resp,
    input  logic        ddtw_r_last,

    // PTW AXI read port (scalar)
    output logic        ptw_ar_valid,
    output logic [63:0] ptw_ar_addr,
    output logic [3:0]  ptw_ar_id,
    output logic [7:0]  ptw_ar_len,
    output logic [2:0]  ptw_ar_size,
    output logic [1:0]  ptw_ar_burst,
    input  logic        ptw_ar_ready,
    output logic        ptw_r_ready,
    input  logic        ptw_r_valid,
    input  logic [63:0] ptw_r_data,
    input  logic [3:0]  ptw_r_id,
    input  logic [1:0]  ptw_r_resp,
    input  logic        ptw_r_last,

    // Control
    input  logic        nonblock_enable_i,
    output logic        nonblock_fire_o,
    output logic        nonblock_busy_o,
    output logic        fault_valid_o,

    // Policy composition controls (test-exposed)
    input  logic        policy_partition_i,
    input  logic        policy_clear_i,
    input  logic [23:0] policy_did_i,
    output logic        policy_quiet_o,

    // IOTLB research snapshot (test observation only; no functional effect)
    output logic        research_update_o,
    output logic [23:0] research_did_o,
    output logic [15:0] research_replace_mask_o,
    output logic [15:0] research_valid_q_o,
    output logic [15:0] research_valid_n_o
);

    // ------------------------------------------------------------------
    // Local types (field-by-field match to the RTL wrappers)
    // ------------------------------------------------------------------
    typedef logic [63:0] addr_t;
    typedef logic [55:0] paddr_t;

    typedef struct packed {
        logic [23:0] did;
        logic [63:0] iova;
        logic [5:0]  ttype;
        logic [19:0] pid;
        logic        pid_valid;
        logic        priv;
        logic        is_debug;
        logic        bank;
    } trans_req_data_t;

    typedef struct packed {
        logic        bank;
        logic        error;
        logic        ignore;
        paddr_t      spaddr;
        logic        is_mrif;
        rv_iommu::mrifc_content_t mrif_data;
        logic [2:0]  range;
        logic        bypass;
        rv_iommu::cause_t fault_code;
        logic        w;
        logic        r;
        logic        x;
        logic        sum;
        logic        g;
        logic        u;
        logic        t2gpa;
    } trans_resp_data_t;

    typedef struct packed {
        logic        error;
        logic        is_superpage;
        logic [1:0]  pbmt;
        logic [43:0] ppn;
    } dbg_resp_t;

    typedef struct packed {
        logic [5:0]  trans_type;
        rv_iommu::cause_t cause_code;
        logic [63:0] iova;
        logic [40:0] gpaddr;          // GPLEN39 = 41
        logic [23:0] did;
        logic        pv;
        logic [19:0] pid;
        logic        is_supervisor;
        logic        is_guest_pf;
        logic        is_implicit;
    } fault_data_t;

    // ------------------------------------------------------------------
    // AXI types
    // ------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(axi_aw_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_W_CHAN_T (axi_w_chan_t,  logic [63:0], logic [7:0], logic [0:0])
    `AXI_TYPEDEF_B_CHAN_T (axi_b_chan_t,  logic [3:0],  logic [0:0])
    `AXI_TYPEDEF_AR_CHAN_T(axi_ar_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_R_CHAN_T (axi_r_chan_t,  logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_REQ_T    (axi_req_t,     axi_aw_chan_t, axi_w_chan_t, axi_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (axi_resp_t,    axi_b_chan_t,  axi_r_chan_t)

    // ------------------------------------------------------------------
    // Configuration
    // ------------------------------------------------------------------
    localparam rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::rv_iommu_cfg_t'{
        NumOutstandingTrans : 1,
        WFifoDepth          : 1,
        MrifFifoDepth       : 1,
        FaultFifoDepth      : 1,
        NumIotlbEntries     : 16,
        NumDdtcEntries      : 8,
        NumPdtcEntries      : 1,
        NumMrifcEntries     : 1,
        InclPC              : 1'b0,
        InclAxiBC           : 1'b0,
        InclDbg             : 1'b0,
        InclATS             : 1'b0,
        MSITrans            : rv_iommu_cfg::MSI_DISABLED,
        IGS                 : rv_iommu_cfg::MSI_ONLY,
        NumIntVec           : 1,
        NumHpmCounters      : 0,
        PAddrWidth          : 56,
        AxiAddrWidth        : 64,
        AxiDataWidth        : 64,
        AxiIdWidth          : 4,
        AxiProgIdWidth      : 4,
        AxiUserWidth        : 1,
        AxiProIdWidth       : 1,
        AxiDevIdWidth       : 1,
        AxisDataWidth       : 64,
        AxisDestWidth       : 1,
        AxisUserWidth       : 1,
        AxisStrbWidth       : 8,
        AxisKeepWidth       : 8,
        AxisIdWidth         : 4,
        Freq                : 1
    };

    // ------------------------------------------------------------------
    // Request / response assembly
    // ------------------------------------------------------------------
    trans_req_data_t req_data;
    assign req_data.did       = req_did;
    assign req_data.iova      = req_iova;
    assign req_data.ttype     = req_ttype;
    assign req_data.pid       = req_pid;
    assign req_data.pid_valid = req_pid_valid;
    assign req_data.priv      = req_priv;
    assign req_data.is_debug  = req_is_debug;
    assign req_data.bank      = req_bank;

    trans_resp_data_t resp_data;
    assign resp_bank        = resp_data.bank;
    assign resp_error       = resp_data.error;
    assign resp_ignore      = resp_data.ignore;
    assign resp_spaddr      = resp_data.spaddr;
    assign resp_is_mrif     = resp_data.is_mrif;
    assign resp_mrif_nid    = resp_data.mrif_data.nid;
    assign resp_mrif_nppn   = resp_data.mrif_data.nppn;
    assign resp_mrif_addr   = resp_data.mrif_data.addr;
    assign resp_range       = resp_data.range;
    assign resp_bypass      = resp_data.bypass;
    assign resp_fault_code  = resp_data.fault_code;
    assign resp_w           = resp_data.w;
    assign resp_r           = resp_data.r;
    assign resp_x           = resp_data.x;
    assign resp_sum         = resp_data.sum;
    assign resp_g           = resp_data.g;
    assign resp_u           = resp_data.u;
    assign resp_t2gpa       = resp_data.t2gpa;

    // ------------------------------------------------------------------
    // Invalidation structs
    // ------------------------------------------------------------------
    rv_iommu::iotlb_inval_t iotinval_i;
    assign iotinval_i.inval_vma = inval_vma;
    assign iotinval_i.inval_gvma= inval_gvma;
    assign iotinval_i.av        = inval_av;
    assign iotinval_i.gv        = inval_gv;
    assign iotinval_i.pscv      = inval_pscv;
    assign iotinval_i.vpn       = inval_vpn;
    assign iotinval_i.gscid     = inval_gscid;
    assign iotinval_i.pscid     = inval_pscid;

    rv_iommu::xdtc_inval_t iodirinval_i;
    assign iodirinval_i.inval_ddtc = iodir_inval_ddtc;
    assign iodirinval_i.inval_pdtc = iodir_inval_pdtc;
    assign iodirinval_i.dv         = iodir_dv;
    assign iodirinval_i.did        = iodir_did;
    assign iodirinval_i.pid        = iodir_pid;

    // ------------------------------------------------------------------
    // DDTW AXI plumbing
    // ------------------------------------------------------------------
    axi_req_t  ddtw_axi_req;
    axi_resp_t ddtw_axi_resp;

    assign ddtw_ar_valid = ddtw_axi_req.ar_valid;
    assign ddtw_ar_addr  = ddtw_axi_req.ar.addr;
    assign ddtw_ar_id    = ddtw_axi_req.ar.id;
    assign ddtw_ar_len   = ddtw_axi_req.ar.len;
    assign ddtw_ar_size  = ddtw_axi_req.ar.size;
    assign ddtw_ar_burst = ddtw_axi_req.ar.burst;
    assign ddtw_r_ready  = ddtw_axi_req.r_ready;

    assign ddtw_axi_resp.ar_ready = ddtw_ar_ready;
    assign ddtw_axi_resp.r_valid  = ddtw_r_valid;
    assign ddtw_axi_resp.r.data   = ddtw_r_data;
    assign ddtw_axi_resp.r.id     = ddtw_r_id;
    assign ddtw_axi_resp.r.resp   = ddtw_r_resp;
    assign ddtw_axi_resp.r.last   = ddtw_r_last;
    assign ddtw_axi_resp.aw_ready = 1'b0;
    assign ddtw_axi_resp.w_ready  = 1'b0;
    assign ddtw_axi_resp.b_valid  = 1'b0;
    assign ddtw_axi_resp.b        = '0;

    // ------------------------------------------------------------------
    // PTW AXI plumbing
    // ------------------------------------------------------------------
    axi_req_t  ptw_axi_req;
    axi_resp_t ptw_axi_resp;

    assign ptw_ar_valid = ptw_axi_req.ar_valid;
    assign ptw_ar_addr  = ptw_axi_req.ar.addr;
    assign ptw_ar_id    = ptw_axi_req.ar.id;
    assign ptw_ar_len   = ptw_axi_req.ar.len;
    assign ptw_ar_size  = ptw_axi_req.ar.size;
    assign ptw_ar_burst = ptw_axi_req.ar.burst;
    assign ptw_r_ready  = ptw_axi_req.r_ready;

    assign ptw_axi_resp.ar_ready = ptw_ar_ready;
    assign ptw_axi_resp.r_valid  = ptw_r_valid;
    assign ptw_axi_resp.r.data   = ptw_r_data;
    assign ptw_axi_resp.r.id     = ptw_r_id;
    assign ptw_axi_resp.r.resp   = ptw_r_resp;
    assign ptw_axi_resp.r.last   = ptw_r_last;
    assign ptw_axi_resp.aw_ready = 1'b0;
    assign ptw_axi_resp.w_ready  = 1'b0;
    assign ptw_axi_resp.b_valid  = 1'b0;
    assign ptw_axi_resp.b        = '0;

    // ------------------------------------------------------------------
    // Configuration registers
    // ------------------------------------------------------------------
    iommu_reg2hw_capabilities_reg_t capabilities_i;
    always_comb begin
        capabilities_i            = '0;
        capabilities_i.sv39.q     = 1'b1;
        capabilities_i.pas.q      = 6'd56;
    end

    iommu_reg2hw_fctl_reg_t fctl_i;
    always_comb begin
        fctl_i = '0;
    end

    iommu_reg2hw_ddtp_reg_t ddtp_i;
    always_comb begin
        ddtp_i                = '0;
        ddtp_i.iommu_mode.q   = 4'b0010; // 1-level DDT
        ddtp_i.ppn.q          = 44'h10;  // 0x10000 >> 12
    end

    // ------------------------------------------------------------------
    // Static tie-offs
    // ------------------------------------------------------------------
    trans_req_data_t ats_trans_req_data_i;
    trans_req_data_t debug_req_data_i;
    assign ats_trans_req_data_i = '0;
    assign debug_req_data_i     = '0;

    logic ats_trans_req_valid_i;
    logic debug_req_valid_i;
    logic ats_trans_resp_ready_i;
    logic debug_resp_ready_i;
    logic ats_ddtc_valid_i;
    device_id_t ats_ddtc_did_i;

    assign ats_trans_req_valid_i = 1'b0;
    assign debug_req_valid_i     = 1'b0;
    assign ats_trans_resp_ready_i= 1'b1;
    assign debug_resp_ready_i    = 1'b1;
    assign ats_ddtc_valid_i      = 1'b0;
    assign ats_ddtc_did_i        = '0;

    // ------------------------------------------------------------------
    // DUT
    // ------------------------------------------------------------------
    trans_resp_data_t trans_resp_data_o;
    logic             trans_resp_valid_o_int;
    assign trans_resp_valid_o = trans_resp_valid_o_int;
    assign resp_data          = trans_resp_data_o;

    fault_data_t fault_data_o;
    logic        fault_valid_o_int;
    assign fault_valid_o = fault_valid_o_int;

    rv_iommu::research_t research_o_int;
    logic [15:0] nonblock_diag_unused;

    rv_iommu_tl_wrap #(
        .RVIOMMUCfg         (RVIOMMUCfg),
        .E3Enable           (1'b0),
        .NonblockEnable     (NonblockEnable),
        .PolicyEnable       (PolicyEnable),
        .addr_t             (addr_t),
        .paddr_t            (paddr_t),
        .dc_t               (rv_iommu::dc_base_t),
        .ddtc_up_t          (rv_iommu::ddtc_up_base_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .dbg_resp_t         (dbg_resp_t),
        .fault_data_t       (fault_data_t),
        .axi_req_t          (axi_req_t),
        .axi_resp_t         (axi_resp_t)
    ) i_dut (
        .clk_i,
        .rst_ni,
        .trans_req_data_i       (req_data),
        .trans_req_valid_i      (trans_req_valid_i),
        .trans_req_ready_o      (trans_req_ready_o),
        .ats_trans_req_data_i   (ats_trans_req_data_i),
        .ats_trans_req_valid_i  (ats_trans_req_valid_i),
        .ats_trans_req_ready_o  (),
        .debug_req_data_i       (debug_req_data_i),
        .debug_req_valid_i      (debug_req_valid_i),
        .debug_req_ready_o      (),
        .trans_resp_data_o      (trans_resp_data_o),
        .trans_resp_valid_o     (trans_resp_valid_o_int),
        .trans_resp_ready_i     (trans_resp_ready_i),
        .ats_trans_resp_data_o  (),
        .ats_trans_resp_valid_o (),
        .ats_trans_resp_ready_i (ats_trans_resp_ready_i),
        .ats_ddtc_valid_i       (ats_ddtc_valid_i),
        .ats_ddtc_did_i         (ats_ddtc_did_i),
        .ats_ddtc_ready_o       (),
        .ats_ddtc_tc_o          (),
        .debug_resp_data_o      (),
        .debug_resp_valid_o     (),
        .debug_resp_ready_i     (debug_resp_ready_i),
        .capabilities_i         (capabilities_i),
        .fctl_i                 (fctl_i),
        .ddtp_i                 (ddtp_i),
        .iotinval_i             (iotinval_i),
        .iodirinval_i           (iodirinval_i),
        .hpm_events_o           (),
        .ddtw_axi_req_o         (ddtw_axi_req),
        .ddtw_axi_resp_i        (ddtw_axi_resp),
        .pdtw_axi_req_o         (),
        .pdtw_axi_resp_i        ('0),
        .ptw_axi_req_o          (ptw_axi_req),
        .ptw_axi_resp_i         (ptw_axi_resp),
        .msiptw_axi_req_o       (),
        .msiptw_axi_resp_i      ('0),
        .fault_data_o           (fault_data_o),
        .fault_valid_o          (fault_valid_o_int),
        .fault_ready_i          (1'b1),
        .e3_enable_i            (1'b0),
        .e3_arm_i               (1'b0),
        .e3_active_i            (1'b0),
        .e3_waiting_i           (1'b0),
        .e3_delay128_i          (1'b0),
        .e3_o                   (),
        .research_o             (research_o_int),
        .nonblock_enable_i      (nonblock_enable_i),
        .nonblock_fire_o        (nonblock_fire_o),
        .nonblock_busy_o        (nonblock_busy_o),
        .nonblock_diag_o        (nonblock_diag_unused),
        .policy_partition_i     (policy_partition_i),
        .policy_clear_i         (policy_clear_i),
        .policy_did_i           (policy_did_i),
        .policy_quiet_o         (policy_quiet_o)
    );

    // Flatten the research snapshot into simple vectors for the C++ TB.
    // Bit positions match rv_iommu_iotlb research_o packing:
    //   entries[i][51]      = current valid
    //   entries_next[i][51] = next valid
    always_comb begin
        research_valid_q_o = '0;
        research_valid_n_o = '0;
        for (int unsigned i = 0; i < 16; i++) begin
            research_valid_q_o[i] = research_o_int.entries[i][51];
            research_valid_n_o[i] = research_o_int.entries_next[i][51];
        end
    end

    assign research_update_o       = research_o_int.update;
    assign research_did_o          = research_o_int.did;
    assign research_replace_mask_o = research_o_int.replace_mask;

endmodule
