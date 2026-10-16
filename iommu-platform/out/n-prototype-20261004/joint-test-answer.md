Below are the complete proposed files for the bounded `rv_iommu_banked_ctl` + `rv_iommu_tl_wrap` integration test. No existing testhelper changes are required. I have not executed the build or run; the parent must verify with Verilator.

## `tests/nonblock_banked_top.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Structural scalar-I/O harness around the joint
//   rv_iommu_banked_ctl  <->  rv_iommu_tl_wrap
// system.  No clock generation, no `initial` stimulus, no behavioural models.
// All stimulus/checking lives in tests/nonblock_banked_test.cpp.

`include "axi/typedef.svh"

module nonblock_banked_top
  import rv_iommu_reg_pkg::*;
  import rv_iommu::*;
  import rv_iommu_cfg::*;
  import axi_pkg::*;
#(
    parameter bit NonblockEnable = 1
) (
    input  logic        clk_i,
    input  logic        rst_ni,

    // ---------------- upstream translation-request AXI : AW ------------------
    input  logic         tr_aw_valid,
    input  logic [3:0]   tr_aw_id,
    input  logic [63:0]  tr_aw_addr,
    input  logic [7:0]   tr_aw_len,
    input  logic [2:0]   tr_aw_size,
    input  logic [1:0]   tr_aw_burst,
    input  logic [2:0]   tr_aw_prot,
    input  logic [0:0]   tr_aw_user,
    input  logic [23:0]  tr_aw_did,
    input  logic         tr_aw_ssid_valid,
    input  logic [19:0]  tr_aw_subid,
    input  logic         tr_aw_mmu_atst,
    input  logic         tr_aw_mmu_valid,

    // ---------------- upstream translation-request AXI : AR ------------------
    input  logic         tr_ar_valid,
    input  logic [3:0]   tr_ar_id,
    input  logic [63:0]  tr_ar_addr,
    input  logic [7:0]   tr_ar_len,
    input  logic [2:0]   tr_ar_size,
    input  logic [1:0]   tr_ar_burst,
    input  logic [2:0]   tr_ar_prot,
    input  logic [0:0]   tr_ar_user,
    input  logic [23:0]  tr_ar_did,
    input  logic         tr_ar_ssid_valid,
    input  logic [19:0]  tr_ar_subid,
    input  logic         tr_ar_mmu_atst,
    input  logic         tr_ar_mmu_valid,

    // ---------------- upstream translation-request AXI : W -------------------
    input  logic         tr_w_valid,
    input  logic [63:0]  tr_w_data,
    input  logic [7:0]   tr_w_strb,
    input  logic         tr_w_last,
    input  logic [0:0]   tr_w_user,

    // ---------------- upstream translation-request AXI : B/R ready ----------
    input  logic         tr_b_ready,
    input  logic         tr_r_ready,

    // ---------------- upstream translation-request AXI : responses -----------
    output logic         tr_aw_ready,
    output logic         tr_ar_ready,
    output logic         tr_w_ready,
    output logic         tr_b_valid,
    output logic [3:0]   tr_b_id,
    output logic [1:0]   tr_b_resp,
    output logic         tr_r_valid,
    output logic [3:0]   tr_r_id,
    output logic [63:0]  tr_r_data,
    output logic [1:0]   tr_r_resp,
    output logic         tr_r_last,

    // ---------------- completion AXI : requests ------------------------------
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

    // ---------------- completion AXI : responses -----------------------------
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

    // ---------------- DDTW AXI read port -------------------------------------
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

    // ---------------- PTW AXI read port --------------------------------------
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

    // ---------------- control/trace ------------------------------------------
    input  logic        nonblock_enable_i,
    output logic        nonblock_fire_o,
    output logic        nonblock_busy_o,
    output logic        in_flight_o
);

    // -------------------------------------------------------------------------
    // Fixed widths
    // -------------------------------------------------------------------------
    localparam int unsigned AxiIdWidth   = 4;
    localparam int unsigned AxiAddrWidth = 64;
    localparam int unsigned AxiDataWidth = 64;
    localparam int unsigned AxiUserWidth = 1;

    typedef logic [AxiIdWidth-1:0]     id_t;
    typedef logic [AxiAddrWidth-1:0]   addr_t;
    typedef logic [AxiDataWidth-1:0]   data_t;
    typedef logic [AxiDataWidth/8-1:0] strb_t;
    typedef logic [AxiUserWidth-1:0]   user_t;
    typedef logic [23:0]               did_t;
    typedef logic [19:0]               subid_t;
    typedef logic [55:0]               paddr_t;

    // -------------------------------------------------------------------------
    // Upstream translation-request AXI types
    // -------------------------------------------------------------------------
    typedef struct packed {
        id_t              id;
        addr_t            addr;
        axi_pkg::len_t    len;
        axi_pkg::size_t   size;
        axi_pkg::burst_t  burst;
        logic             lock;
        axi_pkg::cache_t  cache;
        axi_pkg::prot_t   prot;
        axi_pkg::qos_t    qos;
        axi_pkg::region_t region;
        axi_pkg::atop_t   atop;
        user_t            user;
        did_t             stream_id;
        logic             ss_id_valid;
        subid_t           substream_id;
        logic             mmu_atst;
        logic             mmu_valid;
    } tr_aw_chan_t;

    typedef struct packed {
        id_t              id;
        addr_t            addr;
        axi_pkg::len_t    len;
        axi_pkg::size_t   size;
        axi_pkg::burst_t  burst;
        logic             lock;
        axi_pkg::cache_t  cache;
        axi_pkg::prot_t   prot;
        axi_pkg::qos_t    qos;
        axi_pkg::region_t region;
        user_t            user;
        did_t             stream_id;
        logic             ss_id_valid;
        subid_t           substream_id;
        logic             mmu_atst;
        logic             mmu_valid;
    } tr_ar_chan_t;

    typedef struct packed {
        data_t data;
        strb_t strb;
        logic  last;
        user_t user;
    } tr_w_chan_t;

    typedef struct packed {
        id_t            id;
        axi_pkg::resp_t resp;
        user_t          user;
    } tr_b_chan_t;

    typedef struct packed {
        id_t            id;
        data_t          data;
        axi_pkg::resp_t resp;
        logic           last;
        user_t          user;
    } tr_r_chan_t;

    `AXI_TYPEDEF_REQ_T (axi_tr_req_t,  tr_aw_chan_t, tr_w_chan_t, tr_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(axi_tr_resp_t, tr_b_chan_t,  tr_r_chan_t)

    // -------------------------------------------------------------------------
    // Completion AXI types (ORIGINAL ID width)
    // -------------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(comp_aw_chan_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_W_CHAN_T (comp_w_chan_t,  data_t, strb_t, user_t)
    `AXI_TYPEDEF_B_CHAN_T (comp_b_chan_t,  id_t, user_t)
    `AXI_TYPEDEF_AR_CHAN_T(comp_ar_chan_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T (comp_r_chan_t,  data_t, id_t, user_t)
    `AXI_TYPEDEF_REQ_T (axi_comp_req_t,  comp_aw_chan_t, comp_w_chan_t, comp_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(axi_comp_resp_t, comp_b_chan_t,  comp_r_chan_t)

    // -------------------------------------------------------------------------
    // Translation-engine types (match rv_iommu_tl_wrap / rv_iommu_banked_ctl)
    // -------------------------------------------------------------------------
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
        logic [40:0] gpaddr;
        logic [23:0] did;
        logic        pv;
        logic [19:0] pid;
        logic        is_supervisor;
        logic        is_guest_pf;
        logic        is_implicit;
    } fault_data_t;

    typedef logic axi_ds_req_t;
    typedef logic axi_ds_resp_t;

    // -------------------------------------------------------------------------
    // DDTW / PTW AXI read types
    // -------------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(axi_aw_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_W_CHAN_T (axi_w_chan_t,  logic [63:0], logic [7:0], logic [0:0])
    `AXI_TYPEDEF_B_CHAN_T (axi_b_chan_t,  logic [3:0],  logic [0:0])
    `AXI_TYPEDEF_AR_CHAN_T(axi_ar_chan_t, logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_R_CHAN_T (axi_r_chan_t,  logic [63:0], logic [3:0], logic [0:0])
    `AXI_TYPEDEF_REQ_T    (axi_req_t,     axi_aw_chan_t, axi_w_chan_t, axi_ar_chan_t)
    `AXI_TYPEDEF_RESP_T   (axi_resp_t,    axi_b_chan_t,  axi_r_chan_t)

    // -------------------------------------------------------------------------
    // Configuration
    // -------------------------------------------------------------------------
    function automatic rv_iommu_cfg::rv_iommu_cfg_t test_cfg();
        rv_iommu_cfg::rv_iommu_cfg_t c;
        c = rv_iommu_cfg::NullCfg;
        c.NumOutstandingTrans = 4;
        c.WFifoDepth          = 8;
        c.MrifFifoDepth       = 1;
        c.FaultFifoDepth      = 1;
        c.NumIotlbEntries     = 16;
        c.NumDdtcEntries      = 8;
        c.NumPdtcEntries      = 1;
        c.NumMrifcEntries     = 1;
        c.InclPC              = 1'b0;
        c.InclAxiBC           = 1'b0;
        c.InclDbg             = 1'b0;
        c.InclATS             = 1'b0;
        c.MSITrans            = rv_iommu_cfg::MSI_BT_ONLY;
        c.IGS                 = rv_iommu_cfg::MSI_ONLY;
        c.NumIntVec           = 1;
        c.NumHpmCounters      = 0;
        c.PAddrWidth          = 56;
        c.AxiAddrWidth        = 64;
        c.AxiDataWidth        = 64;
        c.AxiIdWidth          = 4;
        c.AxiProgIdWidth      = 4;
        c.AxiUserWidth        = 1;
        c.AxiProIdWidth       = 1;
        c.AxiDevIdWidth       = 1;
        c.AxisDataWidth       = 64;
        c.AxisDestWidth       = 1;
        c.AxisUserWidth       = 1;
        c.AxisStrbWidth       = 8;
        c.AxisKeepWidth       = 8;
        c.AxisIdWidth         = 4;
        c.Freq                = 1;
        return c;
    endfunction

    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = test_cfg();

    axi_tr_req_t    tr_req;
    axi_tr_resp_t   tr_resp;
    axi_comp_req_t  comp_req;
    axi_comp_resp_t comp_resp;

    trans_req_data_t  trans_req_data;
    logic             trans_req_valid;
    logic             trans_req_ready;
    trans_resp_data_t trans_resp_data;
    logic             trans_resp_valid;
    logic             trans_resp_ready;

    axi_req_t  ddtw_axi_req;
    axi_resp_t ddtw_axi_resp;
    axi_req_t  ptw_axi_req;
    axi_resp_t ptw_axi_resp;

    // -------------------------------------------------------------------------
    // Pack upstream request
    // -------------------------------------------------------------------------
    always_comb begin : pack_tr_req
        tr_req = '0;

        tr_req.aw_valid = tr_aw_valid;
        tr_req.aw.id    = tr_aw_id;
        tr_req.aw.addr  = tr_aw_addr;
        tr_req.aw.len   = tr_aw_len;
        tr_req.aw.size  = tr_aw_size;
        tr_req.aw.burst = tr_aw_burst;
        tr_req.aw.prot  = tr_aw_prot;
        tr_req.aw.user  = tr_aw_user;
        tr_req.aw.stream_id    = tr_aw_did;
        tr_req.aw.ss_id_valid  = tr_aw_ssid_valid;
        tr_req.aw.substream_id = tr_aw_subid;
        tr_req.aw.mmu_atst     = tr_aw_mmu_atst;
        tr_req.aw.mmu_valid    = tr_aw_mmu_valid;

        tr_req.ar_valid = tr_ar_valid;
        tr_req.ar.id    = tr_ar_id;
        tr_req.ar.addr  = tr_ar_addr;
        tr_req.ar.len   = tr_ar_len;
        tr_req.ar.size  = tr_ar_size;
        tr_req.ar.burst = tr_ar_burst;
        tr_req.ar.prot  = tr_ar_prot;
        tr_req.ar.user  = tr_ar_user;
        tr_req.ar.stream_id    = tr_ar_did;
        tr_req.ar.ss_id_valid  = tr_ar_ssid_valid;
        tr_req.ar.substream_id = tr_ar_subid;
        tr_req.ar.mmu_atst     = tr_ar_mmu_atst;
        tr_req.ar.mmu_valid    = tr_ar_mmu_valid;

        tr_req.w_valid = tr_w_valid;
        tr_req.w.data  = tr_w_data;
        tr_req.w.strb  = tr_w_strb;
        tr_req.w.last  = tr_w_last;
        tr_req.w.user  = tr_w_user;

        tr_req.b_ready = tr_b_ready;
        tr_req.r_ready = tr_r_ready;
    end

    // -------------------------------------------------------------------------
    // Unpack upstream responses
    // -------------------------------------------------------------------------
    assign tr_aw_ready = tr_resp.aw_ready;
    assign tr_ar_ready = tr_resp.ar_ready;
    assign tr_w_ready  = tr_resp.w_ready;
    assign tr_b_valid  = tr_resp.b_valid;
    assign tr_b_id     = tr_resp.b.id;
    assign tr_b_resp   = tr_resp.b.resp;
    assign tr_r_valid  = tr_resp.r_valid;
    assign tr_r_id     = tr_resp.r.id;
    assign tr_r_data   = tr_resp.r.data;
    assign tr_r_resp   = tr_resp.r.resp;
    assign tr_r_last   = tr_resp.r.last;

    // -------------------------------------------------------------------------
    // Completion master expansion
    // -------------------------------------------------------------------------
    assign comp_aw_valid = comp_req.aw_valid;
    assign comp_aw_id    = comp_req.aw.id;
    assign comp_aw_addr  = comp_req.aw.addr;
    assign comp_aw_len   = comp_req.aw.len;
    assign comp_aw_size  = comp_req.aw.size;
    assign comp_aw_burst = comp_req.aw.burst;
    assign comp_aw_prot  = comp_req.aw.prot;
    assign comp_aw_user  = comp_req.aw.user;

    assign comp_ar_valid = comp_req.ar_valid;
    assign comp_ar_id    = comp_req.ar.id;
    assign comp_ar_addr  = comp_req.ar.addr;
    assign comp_ar_len   = comp_req.ar.len;
    assign comp_ar_size  = comp_req.ar.size;
    assign comp_ar_burst = comp_req.ar.burst;
    assign comp_ar_prot  = comp_req.ar.prot;
    assign comp_ar_user  = comp_req.ar.user;

    assign comp_w_valid  = comp_req.w_valid;
    assign comp_w_data   = comp_req.w.data;
    assign comp_w_strb   = comp_req.w.strb;
    assign comp_w_last   = comp_req.w.last;

    assign comp_b_ready  = comp_req.b_ready;
    assign comp_r_ready  = comp_req.r_ready;

    // -------------------------------------------------------------------------
    // Completion slave packing
    // -------------------------------------------------------------------------
    always_comb begin : pack_comp_resp
        comp_resp = '0;
        comp_resp.aw_ready = comp_aw_ready;
        comp_resp.ar_ready = comp_ar_ready;
        comp_resp.w_ready  = comp_w_ready;
        comp_resp.b_valid  = comp_b_valid;
        comp_resp.b.id     = comp_b_id;
        comp_resp.b.resp   = comp_b_resp;
        comp_resp.r_valid  = comp_r_valid;
        comp_resp.r.id     = comp_r_id;
        comp_resp.r.data   = comp_r_data;
        comp_resp.r.resp   = comp_r_resp;
        comp_resp.r.last   = comp_r_last;
    end

    // -------------------------------------------------------------------------
    // DDTW scalar plumbing
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // PTW scalar plumbing
    // -------------------------------------------------------------------------
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

    // -------------------------------------------------------------------------
    // Configuration registers (1-level DDT at 0x10000, Sv39)
    // -------------------------------------------------------------------------
    iommu_reg2hw_capabilities_reg_t capabilities_i;
    always_comb begin
        capabilities_i = '0;
        capabilities_i.sv39.q = 1'b1;
        capabilities_i.pas.q  = 6'd56;
    end

    iommu_reg2hw_fctl_reg_t fctl_i;
    always_comb begin
        fctl_i = '0;
    end

    iommu_reg2hw_ddtp_reg_t ddtp_i;
    always_comb begin
        ddtp_i = '0;
        ddtp_i.iommu_mode.q = 4'b0010; // 1-level DDT
        ddtp_i.ppn.q        = 44'h10;  // 0x10000 >> 12
    end

    rv_iommu::iotlb_inval_t iotinval_i;
    rv_iommu::xdtc_inval_t  iodirinval_i;
    always_comb begin
        iotinval_i   = '0;
        iodirinval_i = '0;
    end

    // -------------------------------------------------------------------------
    // ATS / debug tie-offs
    // -------------------------------------------------------------------------
    trans_req_data_t ats_trans_req_data_i;
    trans_req_data_t debug_req_data_i;
    logic ats_trans_req_valid_i;
    logic debug_req_valid_i;
    logic ats_trans_resp_ready_i;
    logic debug_resp_ready_i;
    logic ats_ddtc_valid_i;
    device_id_t ats_ddtc_did_i;

    assign ats_trans_req_data_i  = '0;
    assign debug_req_data_i      = '0;
    assign ats_trans_req_valid_i = 1'b0;
    assign debug_req_valid_i     = 1'b0;
    assign ats_trans_resp_ready_i= 1'b1;
    assign debug_resp_ready_i    = 1'b1;
    assign ats_ddtc_valid_i      = 1'b0;
    assign ats_ddtc_did_i        = '0;

    // -------------------------------------------------------------------------
    // DUT : banked transaction controller
    // -------------------------------------------------------------------------
    rv_iommu_banked_ctl #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .BankIdBit          (2),
        .addr_t             (addr_t),
        .paddr_t            (paddr_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .fault_data_t       (fault_data_t),
        .axi_tr_aw_chan_t   (tr_aw_chan_t),
        .axi_tr_w_chan_t    (tr_w_chan_t),
        .axi_tr_b_chan_t    (tr_b_chan_t),
        .axi_tr_r_chan_t    (tr_r_chan_t),
        .axi_tr_ar_chan_t   (tr_ar_chan_t),
        .axi_tr_req_t       (axi_tr_req_t),
        .axi_tr_resp_t      (axi_tr_resp_t),
        .axi_comp_aw_chan_t (comp_aw_chan_t),
        .axi_comp_w_chan_t  (comp_w_chan_t),
        .axi_comp_b_chan_t  (comp_b_chan_t),
        .axi_comp_ar_chan_t (comp_ar_chan_t),
        .axi_comp_r_chan_t  (comp_r_chan_t),
        .axi_comp_req_t     (axi_comp_req_t),
        .axi_comp_resp_t    (axi_comp_resp_t),
        .axi_ds_req_t       (axi_ds_req_t),
        .axi_ds_resp_t      (axi_ds_resp_t)
    ) i_banked (
        .clk_i,
        .rst_ni,
        .tr_axi_req_i       (tr_req),
        .tr_axi_resp_o      (tr_resp),
        .comp_axi_resp_i    (comp_resp),
        .comp_axi_req_o     (comp_req),
        .mrif_axi_req_o     (),
        .mrif_axi_resp_i    ('0),
        .trans_req_data_o   (trans_req_data),
        .trans_req_valid_o  (trans_req_valid),
        .trans_req_ready_i  (trans_req_ready),
        .trans_resp_data_i  (trans_resp_data),
        .trans_resp_valid_i (trans_resp_valid),
        .trans_resp_ready_o (trans_resp_ready),
        .mrif_fault_data_o  (),
        .mrif_fault_valid_o (),
        .mrif_fault_ready_i (1'b0),
        .in_flight_o        (in_flight_o),
        .e3_o               ()
    );

    // -------------------------------------------------------------------------
    // DUT : actual translation logic
    // -------------------------------------------------------------------------
    rv_iommu_tl_wrap #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .NonblockEnable     (NonblockEnable),
        .addr_t             (addr_t),
        .paddr_t            (paddr_t),
        .dc_t               (rv_iommu::dc_ext_t),
        .ddtc_up_t          (rv_iommu::ddtc_up_ext_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .dbg_resp_t         (dbg_resp_t),
        .fault_data_t       (fault_data_t),
        .axi_req_t          (axi_req_t),
        .axi_resp_t         (axi_resp_t)
    ) i_tl (
        .clk_i,
        .rst_ni,
        .trans_req_data_i       (trans_req_data),
        .trans_req_valid_i      (trans_req_valid),
        .trans_req_ready_o      (trans_req_ready),
        .ats_trans_req_data_i   (ats_trans_req_data_i),
        .ats_trans_req_valid_i  (ats_trans_req_valid_i),
        .ats_trans_req_ready_o  (),
        .debug_req_data_i       (debug_req_data_i),
        .debug_req_valid_i      (debug_req_valid_i),
        .debug_req_ready_o      (),
        .trans_resp_data_o      (trans_resp_data),
        .trans_resp_valid_o     (trans_resp_valid),
        .trans_resp_ready_i     (trans_resp_ready),
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
        .fault_data_o           (),
        .fault_valid_o          (),
        .fault_ready_i          (1'b1),
        .e3_enable_i            (1'b0),
        .e3_arm_i               (1'b0),
        .e3_active_i            (1'b0),
        .e3_waiting_i           (1'b0),
        .e3_delay128_i          (1'b0),
        .e3_o                   (),
        .research_o             (),
        .nonblock_enable_i      (nonblock_enable_i),
        .nonblock_fire_o        (nonblock_fire_o),
        .nonblock_busy_o        (nonblock_busy_o)
    );

endmodule
```

## `tests/nonblock_banked_test.cpp`

```cpp
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
#include "Vnonblock_banked_top.h"
#include "verilated.h"

double sc_time_stamp() { return 0; }

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <map>
#include <vector>

#define FAIL_AT(msg)                                                            \
    do {                                                                        \
        std::fprintf(stderr, "nonblock_banked_test: FAIL: %s (%s:%d)\n",        \
                     (msg), __FILE__, __LINE__);                                \
        std::abort();                                                           \
    } while (0)

#define CHECK_AT(cond, msg)                                                     \
    do {                                                                        \
        if (!(cond)) FAIL_AT(msg);                                              \
    } while (0)

struct Beat {
    uint64_t data;
    uint8_t  strb;
    bool     last;
};

static const uint32_t V_ID      = 0;
static const uint32_t A_ID      = 4;
static const uint32_t V_DID     = 10;
static const uint32_t A_DID     = 11;
static const uint64_t VSRC_IOVA = 0x10000000ULL;
static const uint64_t VDST_IOVA = 0x10001000ULL;
static const uint64_t VRO_IOVA  = 0x10002000ULL;
static const uint64_t A_IOVA    = 0x20000000ULL;
static const uint64_t VSRC_PA   = 0x80000000ULL;
static const uint64_t VDST_PA   = 0x80001000ULL;
static const uint64_t A_PA      = 0x90000000ULL;

class Tb {
public:
    Vnonblock_banked_top* top;
    std::map<uint64_t, uint8_t> mem;

    struct AxiReadState {
        bool     pending = false;
        uint64_t addr    = 0;
        uint8_t  len     = 0;
        uint8_t  size    = 0;
        uint8_t  id      = 0;
        int      beat    = 0;
    };
    AxiReadState ddtw_rd, ptw_rd;
    bool hold_ptw_r       = false;
    bool a_ptw_hold_armed = false;

    struct Up {
        bool     aw_valid = false;
        uint32_t aw_id = 0; uint64_t aw_addr = 0;
        uint32_t aw_len = 0, aw_size = 3, aw_burst = 1, aw_prot = 0, aw_user = 0;
        uint32_t aw_did = 0; bool aw_ssid = false; uint32_t aw_subid = 0;
        bool     aw_atst = false, aw_mvalid = false;

        bool     ar_valid = false;
        uint32_t ar_id = 0; uint64_t ar_addr = 0;
        uint32_t ar_len = 0, ar_size = 3, ar_burst = 1, ar_prot = 0, ar_user = 0;
        uint32_t ar_did = 0; bool ar_ssid = false; uint32_t ar_subid = 0;
        bool     ar_atst = false, ar_mvalid = false;

        std::deque<Beat> wq;
        std::deque<std::pair<uint32_t,int> > w_attrib;
        bool b_ready = true, r_ready = true;
    } up;

    struct CawEnt {
        uint32_t id = 0;
        uint64_t addr = 0;
        uint32_t len = 0;
        std::vector<Beat> exp;
        bool     have_exp = false;
        int      got = 0;
    };
    std::deque<CawEnt> caw_q;
    std::deque<std::pair<uint32_t,uint32_t> > b_q;
    std::deque<Beat> r_q;

    bool comp_aw_ready = true, comp_ar_ready = true, comp_w_ready = true;
    bool comp_b_valid = false; uint32_t comp_b_id = 0, comp_b_resp = 0;
    bool comp_r_valid = false; uint32_t comp_r_id = 0;
    uint64_t comp_r_data = 0; uint32_t comp_r_resp = 0; bool comp_r_last = false;

    bool nonblock_enable = true;
    long g_cycle = 0;

    struct UpAwRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
    struct UpArRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
    struct UpWRec  { long cyc; uint32_t id; uint64_t data; uint8_t strb; bool last; };
    struct UpBRec  { long cyc; uint32_t id; uint32_t resp; };
    struct UpRRec  { long cyc; uint32_t id; uint64_t data; bool last; uint32_t resp; };
    struct CmpAwRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
    struct CmpArRec { long cyc; uint32_t id; uint64_t addr; uint32_t len; };
    struct CmpWRec  { long cyc; uint64_t data; uint8_t strb; bool last; };

    std::vector<UpAwRec>  L_up_aw;
    std::vector<UpArRec>  L_up_ar;
    std::vector<UpWRec>   L_up_w;
    std::vector<UpBRec>   L_up_b;
    std::vector<UpRRec>   L_up_r;
    std::vector<CmpAwRec> L_cmp_aw;
    std::vector<CmpArRec> L_cmp_ar;
    std::vector<CmpWRec>  L_cmp_w;
    std::vector<long>     L_nonblock_fire_cycles;

    struct PendingWrite {
        uint64_t paddr;
        uint32_t len;
        std::vector<Beat> beats;
    };
    std::deque<PendingWrite> g_pending_writes;

    Tb() : top(new Vnonblock_banked_top) {}
    ~Tb() { top->final(); delete top; }

    void write64(uint64_t addr, uint64_t data) {
        for (int i = 0; i < 8; ++i)
            mem[addr + i] = (uint8_t)((data >> (8 * i)) & 0xFF);
    }

    uint64_t read64(uint64_t addr) {
        uint64_t data = 0;
        for (int i = 0; i < 8; ++i) {
            auto it = mem.find(addr + i);
            if (it == mem.end()) {
                std::fprintf(stderr,
                             "nonblock_banked_test: unmapped read @0x%016lx\n",
                             (unsigned long)(addr + i));
                std::abort();
            }
            data |= (uint64_t)it->second << (8 * i);
        }
        return data;
    }

    void write_beat(uint64_t addr, uint64_t data, uint8_t strb) {
        for (int i = 0; i < 8; ++i)
            if (strb & (1 << i))
                mem[addr + i] = (uint8_t)((data >> (8 * i)) & 0xFF);
    }

    void setup_memory() {
        // Extended DDT, 1-level, base 0x10000, stride 64.
        // DID10 -> 0x10280, DID11 -> 0x102C0.
        // Word order ascending: tc, iohgatp, ta, fsc, msiptp, mask, pattern, reserved.
        write64(0x10280, 0x0000000000000001ULL);
        write64(0x10288, 0x0000000000000000ULL);
        write64(0x10290, 0x000000000000A000ULL); // pscid=10
        write64(0x10298, 0x8000000000000020ULL); // Sv39, root 0x20
        write64(0x102A0, 0x0ULL);
        write64(0x102A8, 0x0ULL);
        write64(0x102B0, 0x0ULL);
        write64(0x102B8, 0x0ULL);

        write64(0x102C0, 0x0000000000000001ULL);
        write64(0x102C8, 0x0000000000000000ULL);
        write64(0x102D0, 0x000000000000B000ULL); // pscid=11
        write64(0x102D8, 0x8000000000000030ULL); // Sv39, root 0x30
        write64(0x102E0, 0x0ULL);
        write64(0x102E8, 0x0ULL);
        write64(0x102F0, 0x0ULL);
        write64(0x102F8, 0x0ULL);

        // V page tables: root 0x20000, L1V 0x40000, L0V 0x50000.
        write64(0x20000, 0x0000000000010001ULL);
        write64(0x40400, 0x0000000000014001ULL);
        write64(0x50000, 0x00000000200000DFULL); // Vsrc -> 0x80000000
        write64(0x50008, 0x00000000200004DFULL); // Vdst -> 0x80001000
        write64(0x50010, 0x00000000200000D3ULL); // Vro  -> 0x80000000, w=0
        write64(0x50018, 0x00000000200008DFULL); // Vcold -> 0x80002000

        // A page tables: root 0x30000, L1A 0x60000, L0A 0x70000.
        write64(0x30000, 0x0000000000018001ULL);
        write64(0x60800, 0x000000000001C001ULL);
        write64(0x70000, 0x00000000240000DFULL); // Asrc -> 0x90000000
        write64(0x70008, 0x00000000240004DFULL); // Adst -> 0x90001000

        // Default completion memory.
        for (int i = 0; i < 512; ++i)
            write64(0x80000000 + (uint64_t)i * 8,
                    0x1000000000000000ULL + (uint64_t)i);
        for (int i = 0; i < 512; ++i)
            write64(0x80001000 + (uint64_t)i * 8,
                    0x2000000000000000ULL + (uint64_t)i);
        for (int i = 0; i < 512; ++i)
            write64(0x90000000 + (uint64_t)i * 8,
                    0x3000000000000000ULL + (uint64_t)i);
    }

    void drive_upstream() {
        top->tr_aw_valid = up.aw_valid ? 1 : 0;
        top->tr_aw_id    = up.aw_id;
        top->tr_aw_addr  = up.aw_addr;
        top->tr_aw_len   = up.aw_len;
        top->tr_aw_size  = up.aw_size;
        top->tr_aw_burst = up.aw_burst;
        top->tr_aw_prot  = up.aw_prot;
        top->tr_aw_user  = up.aw_user;
        top->tr_aw_did   = up.aw_did;
        top->tr_aw_ssid_valid = up.aw_ssid ? 1 : 0;
        top->tr_aw_subid = up.aw_subid;
        top->tr_aw_mmu_atst  = up.aw_atst ? 1 : 0;
        top->tr_aw_mmu_valid = up.aw_mvalid ? 1 : 0;

        top->tr_ar_valid = up.ar_valid ? 1 : 0;
        top->tr_ar_id    = up.ar_id;
        top->tr_ar_addr  = up.ar_addr;
        top->tr_ar_len   = up.ar_len;
        top->tr_ar_size  = up.ar_size;
        top->tr_ar_burst = up.ar_burst;
        top->tr_ar_prot  = up.ar_prot;
        top->tr_ar_user  = up.ar_user;
        top->tr_ar_did   = up.ar_did;
        top->tr_ar_ssid_valid = up.ar_ssid ? 1 : 0;
        top->tr_ar_subid = up.ar_subid;
        top->tr_ar_mmu_atst  = up.ar_atst ? 1 : 0;
        top->tr_ar_mmu_valid = up.ar_mvalid ? 1 : 0;

        if (!up.wq.empty()) {
            top->tr_w_valid = 1;
            top->tr_w_data  = up.wq.front().data;
            top->tr_w_strb  = up.wq.front().strb;
            top->tr_w_last  = up.wq.front().last ? 1 : 0;
        } else {
            top->tr_w_valid = 0;
            top->tr_w_data  = 0;
            top->tr_w_strb  = 0;
            top->tr_w_last  = 0;
        }
        top->tr_w_user = 0;

        top->tr_b_ready = up.b_ready ? 1 : 0;
        top->tr_r_ready = up.r_ready ? 1 : 0;
    }

    void drive_comp() {
        top->comp_aw_ready = comp_aw_ready ? 1 : 0;
        top->comp_ar_ready = comp_ar_ready ? 1 : 0;
        top->comp_w_ready  = comp_w_ready  ? 1 : 0;
        top->comp_b_valid  = comp_b_valid  ? 1 : 0;
        top->comp_b_id     = comp_b_id;
        top->comp_b_resp   = comp_b_resp;
        top->comp_r_valid  = comp_r_valid  ? 1 : 0;
        top->comp_r_id     = comp_r_id;
        top->comp_r_data   = comp_r_data;
        top->comp_r_resp   = comp_r_resp;
        top->comp_r_last   = comp_r_last ? 1 : 0;
    }

    void drive_ddtw_r() {
        if (ddtw_rd.pending) {
            top->ddtw_r_valid = 1;
            top->ddtw_r_data  =
                read64(ddtw_rd.addr + (uint64_t)ddtw_rd.beat *
                                           (1ULL << ddtw_rd.size));
            top->ddtw_r_id    = ddtw_rd.id;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = (ddtw_rd.beat == ddtw_rd.len) ? 1 : 0;
        } else {
            top->ddtw_r_valid = 0;
            top->ddtw_r_data  = 0;
            top->ddtw_r_id    = 0;
            top->ddtw_r_resp  = 0;
            top->ddtw_r_last  = 0;
        }
    }

    void drive_ptw_r() {
        if (ptw_rd.pending && !hold_ptw_r) {
            top->ptw_r_valid = 1;
            top->ptw_r_data  =
                read64(ptw_rd.addr + (uint64_t)ptw_rd.beat *
                                          (1ULL << ptw_rd.size));
            top->ptw_r_id    = ptw_rd.id;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = (ptw_rd.beat == ptw_rd.len) ? 1 : 0;
        } else {
            top->ptw_r_valid = 0;
            top->ptw_r_data  = 0;
            top->ptw_r_id    = 0;
            top->ptw_r_resp  = 0;
            top->ptw_r_last  = 0;
        }
    }

    void tick() {
        top->nonblock_enable_i = nonblock_enable ? 1 : 0;
        drive_upstream();
        drive_comp();
        top->ddtw_ar_ready = ddtw_rd.pending ? 0 : 1;
        top->ptw_ar_ready  = ptw_rd.pending  ? 0 : 1;
        drive_ddtw_r();
        drive_ptw_r();

        top->eval();

        bool d_ar_fire = top->ddtw_ar_valid && top->ddtw_ar_ready;
        uint64_t d_ar_addr = top->ddtw_ar_addr;
        uint8_t d_ar_len   = top->ddtw_ar_len;
        uint8_t d_ar_size  = top->ddtw_ar_size;
        uint8_t d_ar_id    = top->ddtw_ar_id;
        bool d_r_fire = ddtw_rd.pending && top->ddtw_r_valid && top->ddtw_r_ready;

        bool p_ar_fire = top->ptw_ar_valid && top->ptw_ar_ready;
        uint64_t p_ar_addr = top->ptw_ar_addr;
        uint8_t p_ar_len   = top->ptw_ar_len;
        uint8_t p_ar_size  = top->ptw_ar_size;
        uint8_t p_ar_id    = top->ptw_ar_id;
        bool p_r_fire = ptw_rd.pending && top->ptw_r_valid && top->ptw_r_ready;

        bool up_aw_fire = up.aw_valid && top->tr_aw_ready;
        bool up_ar_fire = up.ar_valid && top->tr_ar_ready;
        bool up_w_fire  = !up.wq.empty() && top->tr_w_ready;
        bool up_b_fire  = top->tr_b_valid && up.b_ready;
        bool up_r_fire  = top->tr_r_valid && up.r_ready;

        bool cmp_aw_fire = top->comp_aw_valid && top->comp_aw_ready;
        bool cmp_ar_fire = top->comp_ar_valid && top->comp_ar_ready;
        bool cmp_w_fire  = top->comp_w_valid && top->comp_w_ready;
        bool cmp_b_fire  = comp_b_valid && top->comp_b_ready;
        bool cmp_r_fire  = comp_r_valid && top->comp_r_ready;

        if (top->nonblock_fire_o)
            L_nonblock_fire_cycles.push_back(g_cycle);

        if (up_aw_fire) {
            L_up_aw.push_back({g_cycle, up.aw_id, up.aw_addr, up.aw_len});
            up.w_attrib.push_back(std::make_pair(up.aw_id, (int)up.aw_len + 1));
            up.aw_valid = false;
        }
        if (up_ar_fire) {
            L_up_ar.push_back({g_cycle, up.ar_id, up.ar_addr, up.ar_len});
            up.ar_valid = false;
        }
        if (up_w_fire) {
            uint32_t wid = up.w_attrib.empty() ? 0u : up.w_attrib.front().first;
            L_up_w.push_back({g_cycle, wid, up.wq.front().data,
                              up.wq.front().strb, up.wq.front().last});
            if (!up.w_attrib.empty()) {
                if (--up.w_attrib.front().second <= 0)
                    up.w_attrib.pop_front();
            }
            up.wq.pop_front();
        }
        if (up_b_fire)
            L_up_b.push_back({g_cycle, top->tr_b_id, (uint32_t)top->tr_b_resp});
        if (up_r_fire)
            L_up_r.push_back({g_cycle, top->tr_r_id, top->tr_r_data,
                              (bool)top->tr_r_last, (uint32_t)top->tr_r_resp});

        if (cmp_aw_fire) {
            L_cmp_aw.push_back({g_cycle, top->comp_aw_id,
                                top->comp_aw_addr, top->comp_aw_len});
            bool found = false;
            for (auto it = g_pending_writes.begin();
                 it != g_pending_writes.end(); ++it) {
                if (it->paddr == top->comp_aw_addr &&
                    it->len == top->comp_aw_len) {
                    CawEnt c;
                    c.id       = top->comp_aw_id;
                    c.addr     = top->comp_aw_addr;
                    c.len      = top->comp_aw_len;
                    c.exp      = it->beats;
                    c.have_exp = true;
                    c.got      = 0;
                    caw_q.push_back(c);
                    g_pending_writes.erase(it);
                    found = true;
                    break;
                }
            }
            if (!found) FAIL_AT("comp AW with unexpected addr/len");
        }

        if (cmp_ar_fire) {
            L_cmp_ar.push_back({g_cycle, top->comp_ar_id,
                                top->comp_ar_addr, top->comp_ar_len});
            for (uint32_t i = 0; i <= (uint32_t)top->comp_ar_len; ++i) {
                Beat rb;
                rb.data = read64(top->comp_ar_addr + (uint64_t)i * 8);
                rb.strb = 0xFF;
                rb.last = (i == (uint32_t)top->comp_ar_len);
                r_q.push_back(rb);
            }
        }

        if (cmp_w_fire) {
            L_cmp_w.push_back({g_cycle, top->comp_w_data,
                               (uint8_t)top->comp_w_strb,
                               (bool)top->comp_w_last});
            int match = -1;
            for (size_t i = 0; i < caw_q.size(); ++i) {
                CawEnt& c = caw_q[i];
                if (!c.have_exp || c.got >= (int)c.exp.size()) continue;
                if (c.exp[c.got].data == top->comp_w_data &&
                    c.exp[c.got].strb == (uint8_t)top->comp_w_strb &&
                    c.exp[c.got].last == (bool)top->comp_w_last) {
                    match = (int)i;
                    break;
                }
            }
            if (match < 0) FAIL_AT("comp W beat matches no outstanding AW");
            CawEnt& c = caw_q[(size_t)match];
            CHECK_AT(c.exp[c.got].data == top->comp_w_data,
                     "comp W data mismatch");
            CHECK_AT(c.exp[c.got].strb == (uint8_t)top->comp_w_strb,
                     "comp W strb mismatch");
            CHECK_AT(c.exp[c.got].last == (bool)top->comp_w_last,
                     "comp W last mismatch");
            write_beat(c.addr + (uint64_t)c.got * 8,
                       top->comp_w_data, (uint8_t)top->comp_w_strb);
            c.got++;
            if (top->comp_w_last) {
                CHECK_AT(c.got == (int)c.len + 1,
                         "comp W burst length != len+1");
                b_q.push_back(std::make_pair(c.id, 0u));
                caw_q.erase(caw_q.begin() + match);
            }
        }

        if (cmp_b_fire) { b_q.pop_front(); comp_b_valid = false; }
        if (cmp_r_fire) { r_q.pop_front(); comp_r_valid = false; }

        top->clk_i = 1;
        top->eval();

        if (d_ar_fire) {
            if (ddtw_rd.pending) FAIL_AT("DDTW AR while pending");
            ddtw_rd.pending = true;
            ddtw_rd.addr    = d_ar_addr;
            ddtw_rd.len     = d_ar_len;
            ddtw_rd.size    = d_ar_size;
            ddtw_rd.id      = d_ar_id;
            ddtw_rd.beat    = 0;
        }
        if (d_r_fire) {
            if (ddtw_rd.beat == ddtw_rd.len) ddtw_rd.pending = false;
            else                             ddtw_rd.beat++;
        }

        if (p_ar_fire) {
            if (ptw_rd.pending) FAIL_AT("PTW AR while pending");
            ptw_rd.pending = true;
            ptw_rd.addr    = p_ar_addr;
            ptw_rd.len     = p_ar_len;
            ptw_rd.size    = p_ar_size;
            ptw_rd.id      = p_ar_id;
            ptw_rd.beat    = 0;
            if (a_ptw_hold_armed && p_ar_addr >= 0x30000ULL &&
                p_ar_addr < 0x80000ULL) {
                hold_ptw_r       = true;
                a_ptw_hold_armed = false;
            }
        }
        if (p_r_fire) {
            if (ptw_rd.beat == ptw_rd.len) ptw_rd.pending = false;
            else                           ptw_rd.beat++;
        }

        if (!comp_b_valid && !b_q.empty()) {
            comp_b_valid = true;
            comp_b_id    = b_q.front().first;
            comp_b_resp  = b_q.front().second;
        }
        if (!comp_r_valid && !r_q.empty()) {
            comp_r_valid = true;
            comp_r_id    = r_q.front().id;
            comp_r_data  = r_q.front().data;
            comp_r_resp  = 0;
            comp_r_last  = r_q.front().last;
        }

        top->clk_i = 0;
        top->eval();
        ++g_cycle;
    }

    void reset(bool enable) {
        nonblock_enable = enable;
        top->clk_i  = 0;
        top->rst_ni = 0;

        up = Up();
        caw_q.clear(); b_q.clear(); r_q.clear();
        ddtw_rd = AxiReadState();
        ptw_rd  = AxiReadState();
        hold_ptw_r = false;
        a_ptw_hold_armed = false;
        comp_aw_ready = true; comp_ar_ready = true; comp_w_ready = true;
        comp_b_valid = false; comp_r_valid = false;
        L_up_aw.clear(); L_up_ar.clear(); L_up_w.clear();
        L_up_b.clear(); L_up_r.clear();
        L_cmp_aw.clear(); L_cmp_ar.clear(); L_cmp_w.clear();
        L_nonblock_fire_cycles.clear();
        g_pending_writes.clear();

        for (int i = 0; i < 8; ++i) tick();
        top->rst_ni = 1;
        for (int i = 0; i < 2; ++i) tick();
    }

    bool wait_for_up_ar(size_t prev, int maxc = 50000) {
        for (int i = 0; i < maxc; ++i) {
            tick();
            if (L_up_ar.size() > prev) return true;
        }
        return false;
    }

    bool wait_for_up_r(size_t prev, int maxc = 50000) {
        for (int i = 0; i < maxc; ++i) {
            tick();
            if (L_up_r.size() > prev) return true;
        }
        return false;
    }

    bool wait_for_up_b(size_t prev, int maxc = 50000) {
        for (int i = 0; i < maxc; ++i) {
            tick();
            if (L_up_b.size() > prev) return true;
        }
        return false;
    }

    bool wait_for_ptw_hold(int maxc = 50000) {
        for (int i = 0; i < maxc; ++i) {
            tick();
            if (hold_ptw_r) return true;
        }
        return false;
    }

    void start_read(uint32_t id, uint64_t iova, uint32_t len, uint32_t did) {
        CHECK_AT(!up.ar_valid, "start_read: previous AR still valid");
        up.ar_valid = true;
        up.ar_id    = id;
        up.ar_addr  = iova;
        up.ar_len   = len;
        up.ar_size  = 3;
        up.ar_burst = 1;
        up.ar_prot  = 0;
        up.ar_did   = did;
    }

    void start_write(uint32_t id, uint64_t iova, uint32_t len, uint32_t did,
                     const std::vector<Beat>& beats) {
        CHECK_AT(!up.aw_valid, "start_write: previous AW still valid");
        CHECK_AT(up.wq.empty(), "start_write: previous W queue not empty");
        up.aw_valid = true;
        up.aw_id    = id;
        up.aw_addr  = iova;
        up.aw_len   = len;
        up.aw_size  = 3;
        up.aw_burst = 1;
        up.aw_prot  = 0;
        up.aw_did   = did;
        up.wq.clear();
        for (size_t i = 0; i < beats.size(); ++i)
            up.wq.push_back(beats[i]);
    }

    void check_quiet() {
        for (int i = 0; i < 20; ++i) tick();
        CHECK_AT(!top->in_flight_o, "in_flight_o not low after quiet drain");
    }

    void warm_leaf(uint64_t iova, uint64_t exp_pa, uint32_t did, uint32_t id) {
        (void)exp_pa;
        size_t prev_r = L_up_r.size();
        start_read(id, iova, 0, did);
        if (!wait_for_up_r(prev_r)) FAIL_AT("warm_leaf: no upstream R");
        UpRRec r = L_up_r.back();
        CHECK_AT(r.id == id, "warm_leaf: R id mismatch");
        CHECK_AT(r.resp == 0, "warm_leaf: R resp not OKAY");
        CHECK_AT(r.last, "warm_leaf: R last not set");
    }

    void warm_v() {
        warm_leaf(VSRC_IOVA, VSRC_PA, V_DID, V_ID);
        warm_leaf(VDST_IOVA, VDST_PA, V_DID, V_ID);
        warm_leaf(VRO_IOVA,  VSRC_PA, V_DID, V_ID);
    }

    void case_fast_hit_during_held_a() {
        std::printf("== fast V hit while A held ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        a_ptw_hold_armed = true;
        hold_ptw_r       = false;
        size_t prev_a_ar = L_up_ar.size();
        start_read(A_ID, A_IOVA, 0, A_DID);
        if (!wait_for_up_ar(prev_a_ar)) FAIL_AT("A AR not accepted");
        if (!wait_for_ptw_hold()) FAIL_AT("A PTW not held");
        CHECK_AT(top->in_flight_o, "in_flight_o low while A held");

        size_t prev_v_r = L_up_r.size();
        size_t prev_fire = L_nonblock_fire_cycles.size();
        start_read(V_ID, VSRC_IOVA, 0, V_DID);
        if (!wait_for_up_r(prev_v_r))
            FAIL_AT("V cached read did not complete while A held");
        CHECK_AT(L_up_r.size() == prev_v_r + 1,
                 "A completed while held");
        CHECK_AT(top->in_flight_o, "in_flight_o low while A still held");
        CHECK_AT(L_nonblock_fire_cycles.size() > prev_fire,
                 "V cached read did not trigger nonblock_fire");
        UpRRec vr = L_up_r.back();
        CHECK_AT(vr.id == V_ID, "V R id mismatch");
        CHECK_AT(vr.resp == 0, "V R resp not OKAY");
        CHECK_AT(vr.last, "V R last not set");

        hold_ptw_r = false;
        size_t prev_a_r = L_up_r.size();
        if (!wait_for_up_r(prev_a_r)) FAIL_AT("A did not complete after release");
        CHECK_AT(L_up_r.size() == prev_a_r + 1, "extra R after A release");
        UpRRec ar = L_up_r.back();
        CHECK_AT(ar.id == A_ID, "A R id mismatch");
        CHECK_AT(ar.resp == 0, "A R resp not OKAY");
        CHECK_AT(ar.last, "A R last not set");
        check_quiet();
    }

    void case_nonblock_disabled_blocks() {
        std::printf("== nonblock disabled blocks V until A released ==\n");
        mem.clear(); setup_memory();
        reset(false);
        warm_v();

        a_ptw_hold_armed = true; hold_ptw_r = false;
        size_t prev_a_ar = L_up_ar.size();
        start_read(A_ID, A_IOVA, 0, A_DID);
        if (!wait_for_up_ar(prev_a_ar)) FAIL_AT("A AR not accepted");
        if (!wait_for_ptw_hold()) FAIL_AT("A PTW not held");

        size_t prev_v_r  = L_up_r.size();
        size_t prev_fire = L_nonblock_fire_cycles.size();
        start_read(V_ID, VSRC_IOVA, 0, V_DID);
        for (int i = 0; i < 200; ++i) tick();
        CHECK_AT(L_up_r.size() == prev_v_r,
                 "V completed while nonblock disabled and A held");
        CHECK_AT(L_nonblock_fire_cycles.size() == prev_fire,
                 "nonblock_fire occurred while disabled");

        hold_ptw_r = false;
        size_t prev_a_r = L_up_r.size();
        if (!wait_for_up_r(prev_a_r)) FAIL_AT("A did not complete after release");
        size_t prev_v_r2 = L_up_r.size();
        if (!wait_for_up_r(prev_v_r2))
            FAIL_AT("V did not complete after A released");
        CHECK_AT(L_nonblock_fire_cycles.size() == prev_fire,
                 "nonblock_fire occurred after release with disabled");
        check_quiet();
    }

    void case_write_128beats() {
        std::printf("== legal V write hit, 128 beats ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        uint32_t len = 127;
        std::vector<Beat> beats;
        for (uint32_t i = 0; i <= len; ++i) {
            Beat b;
            b.data = ((uint64_t)0x1111 << 48) | ((uint64_t)i << 8) | (i & 0xFF);
            b.strb = (uint8_t)(0xA5 ^ (i & 0xFF));
            b.last = (i == len);
            beats.push_back(b);
        }
        g_pending_writes.push_back({VDST_PA, len, beats});
        size_t prev_b = L_up_b.size();
        start_write(V_ID, VDST_IOVA, len, V_DID, beats);
        if (!wait_for_up_b(prev_b)) FAIL_AT("V write did not complete");
        CHECK_AT(L_up_b.back().id == V_ID, "V write B id mismatch");
        CHECK_AT(L_up_b.back().resp == 0, "V write B resp not OKAY");
        CHECK_AT(L_cmp_aw.size() >= 1, "no completion AW");
        CHECK_AT(L_cmp_aw.back().id == V_ID, "comp AW id mismatch");
        CHECK_AT(L_cmp_aw.back().addr == VDST_PA, "comp AW addr mismatch");
        CHECK_AT(L_cmp_aw.back().len == len, "comp AW len mismatch");
        CHECK_AT(L_cmp_w.size() >= (size_t)(len + 1),
                 "not enough comp W beats");
        for (uint32_t i = 0; i <= len; ++i) {
            uint64_t got = read64(VDST_PA + (uint64_t)i * 8);
            for (int byte = 0; byte < 8; ++byte) {
                if (beats[i].strb & (1 << byte)) {
                    uint8_t exp_byte = (beats[i].data >> (8 * byte)) & 0xFF;
                    uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
                    CHECK_AT(exp_byte == got_byte,
                             "memory byte mismatch after write");
                }
            }
        }
        check_quiet();
    }

    void case_4kib_read_split() {
        std::printf("== 4KiB read split into 2x256 beats ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        for (uint32_t i = 0; i < 512; ++i) {
            write64(VSRC_PA + (uint64_t)i * 8,
                    ((uint64_t)0xABCD << 48) | i);
        }

        size_t prev_r = L_up_r.size();
        start_read(V_ID, VSRC_IOVA, 255, V_DID);
        if (!wait_for_up_r(prev_r))
            FAIL_AT("first 4KiB read half did not complete");
        prev_r = L_up_r.size();
        start_read(V_ID, VSRC_IOVA + 0x800, 255, V_DID);
        if (!wait_for_up_r(prev_r))
            FAIL_AT("second 4KiB read half did not complete");

        CHECK_AT(L_up_r.size() >= 512, "not enough upstream R beats");
        size_t start = L_up_r.size() - 512;
        for (uint32_t i = 0; i < 512; ++i) {
            uint64_t exp = ((uint64_t)0xABCD << 48) | i;
            CHECK_AT(L_up_r[start + i].data == exp,
                     "4KiB read data mismatch");
            bool exp_last = (i == 255 || i == 511);
            CHECK_AT(L_up_r[start + i].last == exp_last,
                     "4KiB read last mismatch");
            CHECK_AT(L_up_r[start + i].id == V_ID,
                     "4KiB read id mismatch");
        }
        check_quiet();
    }

    void case_4kib_write_split() {
        std::printf("== 4KiB write split into 2x256 beats ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        uint32_t len = 255;
        std::vector<Beat> beats1, beats2;
        for (uint32_t i = 0; i <= len; ++i) {
            Beat b;
            b.data = ((uint64_t)0x2222 << 48) | i;
            b.strb = (uint8_t)(0x5A ^ (i & 0xFF));
            b.last = (i == len);
            beats1.push_back(b);
        }
        for (uint32_t i = 0; i <= len; ++i) {
            Beat b;
            b.data = ((uint64_t)0x3333 << 48) | (i + 256);
            b.strb = (uint8_t)(0xA5 ^ (i & 0xFF));
            b.last = (i == len);
            beats2.push_back(b);
        }
        g_pending_writes.push_back({VDST_PA, len, beats1});
        g_pending_writes.push_back({VDST_PA + 0x800, len, beats2});

        size_t prev_b = L_up_b.size();
        start_write(V_ID, VDST_IOVA, len, V_DID, beats1);
        if (!wait_for_up_b(prev_b))
            FAIL_AT("first 4KiB write half did not complete");
        prev_b = L_up_b.size();
        start_write(V_ID, VDST_IOVA + 0x800, len, V_DID, beats2);
        if (!wait_for_up_b(prev_b))
            FAIL_AT("second 4KiB write half did not complete");

        CHECK_AT(L_cmp_aw.size() >= 2,
                 "not enough completion AW for 4KiB write");
        CHECK_AT(L_cmp_w.size() >= 512,
                 "not enough completion W for 4KiB write");
        for (uint32_t i = 0; i <= len; ++i) {
            uint64_t got = read64(VDST_PA + (uint64_t)i * 8);
            for (int byte = 0; byte < 8; ++byte) {
                if (beats1[i].strb & (1 << byte)) {
                    uint8_t exp_byte =
                        (beats1[i].data >> (8 * byte)) & 0xFF;
                    uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
                    CHECK_AT(exp_byte == got_byte,
                             "memory byte mismatch 4KiB first half");
                }
            }
        }
        for (uint32_t i = 0; i <= len; ++i) {
            uint64_t got = read64(VDST_PA + 0x800 + (uint64_t)i * 8);
            for (int byte = 0; byte < 8; ++byte) {
                if (beats2[i].strb & (1 << byte)) {
                    uint8_t exp_byte =
                        (beats2[i].data >> (8 * byte)) & 0xFF;
                    uint8_t got_byte = (got >> (8 * byte)) & 0xFF;
                    CHECK_AT(exp_byte == got_byte,
                             "memory byte mismatch 4KiB second half");
                }
            }
        }
        check_quiet();
    }

    void case_readonly_write_fault() {
        std::printf("== read-only V write fault ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        size_t prev_b  = L_up_b.size();
        size_t prev_aw = L_cmp_aw.size();
        size_t prev_w  = L_cmp_w.size();
        std::vector<Beat> beats;
        Beat b; b.data = 0xDEADBEEF; b.strb = 0xFF; b.last = true;
        beats.push_back(b);
        start_write(V_ID, VRO_IOVA, 0, V_DID, beats);
        if (!wait_for_up_b(prev_b))
            FAIL_AT("readonly write did not return B");
        CHECK_AT(L_up_b.back().id == V_ID, "readonly fault B id mismatch");
        CHECK_AT(L_up_b.back().resp == 2,
                 "readonly fault B resp != SLVERR");
        CHECK_AT(L_cmp_aw.size() == prev_aw,
                 "readonly write reached completion AW");
        CHECK_AT(L_cmp_w.size() == prev_w,
                 "readonly write reached completion W");
        check_quiet();
    }

    void case_completion_backpressure() {
        std::printf("== completion AW backpressure ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        comp_aw_ready = false;
        std::vector<Beat> beats;
        Beat b; b.data = 0x1234; b.strb = 0xFF; b.last = true;
        beats.push_back(b);
        g_pending_writes.push_back({VDST_PA, 0, beats});
        start_write(V_ID, VDST_IOVA, 0, V_DID, beats);
        int seen = 0;
        uint64_t addr0 = 0;
        for (int i = 0; i < 50; ++i) {
            tick();
            if (top->comp_aw_valid) {
                if (seen == 0) addr0 = top->comp_aw_addr;
                else CHECK_AT(top->comp_aw_addr == addr0,
                              "comp AW addr changed while stalled");
                ++seen;
            }
        }
        CHECK_AT(seen > 0, "comp AW never asserted while stalled");
        comp_aw_ready = true;
        size_t prev_b = L_up_b.size();
        if (!wait_for_up_b(prev_b))
            FAIL_AT("write did not complete after backpressure release");
        CHECK_AT(L_up_b.back().id == V_ID, "backpressure B id mismatch");
        check_quiet();
    }

    void case_reverse_av_arrival() {
        std::printf("== reverse A/V arrival, V while A held ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        a_ptw_hold_armed = true; hold_ptw_r = false;
        size_t prev_a_ar = L_up_ar.size();
        start_read(A_ID, A_IOVA, 0, A_DID);
        if (!wait_for_up_ar(prev_a_ar)) FAIL_AT("A AR not accepted");
        if (!wait_for_ptw_hold()) FAIL_AT("A PTW not held");

        size_t prev_fire = L_nonblock_fire_cycles.size();
        size_t prev_v_b  = L_up_b.size();
        std::vector<Beat> beats;
        Beat b; b.data = 0x5555; b.strb = 0xFF; b.last = true;
        beats.push_back(b);
        g_pending_writes.push_back({VDST_PA, 0, beats});
        start_write(V_ID, VDST_IOVA, 0, V_DID, beats);
        for (int i = 0; i < 100; ++i) tick();
        CHECK_AT(L_nonblock_fire_cycles.size() > prev_fire,
                 "V write did not trigger nonblock_fire while A held");

        hold_ptw_r = false;
        size_t prev_a_r = L_up_r.size();
        if (!wait_for_up_r(prev_a_r))
            FAIL_AT("A did not complete after release");

        for (int i = 0; i < 50000 && L_up_b.size() == prev_v_b; ++i)
            tick();
        CHECK_AT(L_up_b.size() > prev_v_b,
                 "V write did not complete eventually");
        CHECK_AT(L_up_b.back().id == V_ID, "V write B id mismatch");
        check_quiet();
    }

    void case_same_bank_ordering() {
        std::printf("== same-bank read ordering ==\n");
        mem.clear(); setup_memory();
        reset(true);
        warm_v();

        size_t prev_r = L_up_r.size();
        start_read(V_ID, VSRC_IOVA, 0, V_DID);
        if (!wait_for_up_r(prev_r))
            FAIL_AT("first same-bank read did not complete");
        prev_r = L_up_r.size();
        start_read(V_ID, VDST_IOVA, 0, V_DID);
        if (!wait_for_up_r(prev_r))
            FAIL_AT("second same-bank read did not complete");

        CHECK_AT(L_cmp_ar.size() >= 2, "not enough completion AR");
        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 2].addr == VSRC_PA,
                 "first AR addr mismatch");
        CHECK_AT(L_cmp_ar[L_cmp_ar.size() - 1].addr == VDST_PA,
                 "second AR addr mismatch");
        check_quiet();
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    Tb tb;
    tb.case_fast_hit_during_held_a();
    tb.case_nonblock_disabled_blocks();
    tb.case_write_128beats();
    tb.case_4kib_read_split();
    tb.case_4kib_write_split();
    tb.case_readonly_write_fault();
    tb.case_completion_backpressure();
    tb.case_reverse_av_arrival();
    tb.case_same_bank_ordering();

    std::printf("nonblock_banked_test: PASS\n");
    return 0;
}
```

## `tests/test_nonblock_banked.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the banked nonblocking integration test.

Builds a plain C++ (Verilator --cc --exe --build, no --timing) testbench
around tests/nonblock_banked_top.sv, which instantiates the real
rv_iommu_banked_ctl connected to the real rv_iommu_tl_wrap.
"""

import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
VENDOR = IOMMU / "vendor"
OUT    = ROOT / "out" / "nonblock-banked"

TOP = "nonblock_banked_top"

SOURCES = [
    # packages
    VENDOR / "common_cells" / "src" / "cf_math_pkg.sv",
    VENDOR / "axi" / "src" / "axi_pkg.sv",
    IOMMU / "packages" / "rv_iommu_cfg_pkg.sv",
    IOMMU / "packages" / "rv_iommu_reg_pkg.sv",
    IOMMU / "packages" / "rv_iommu_pkg.sv",

    # common_cells
    VENDOR / "common_cells" / "src" / "lzc.sv",
    VENDOR / "common_cells" / "src" / "counter.sv",
    VENDOR / "common_cells" / "src" / "delta_counter.sv",
    VENDOR / "common_cells" / "src" / "fifo_v3.sv",
    VENDOR / "common_cells" / "src" / "spill_register_flushable.sv",
    VENDOR / "common_cells" / "src" / "spill_register.sv",
    VENDOR / "common_cells" / "src" / "rr_arb_tree.sv",
    VENDOR / "common_cells" / "src" / "stream_fifo.sv",
    VENDOR / "common_cells" / "src" / "stream_arbiter.sv",
    VENDOR / "common_cells" / "src" / "stream_arbiter_flushable.sv",

    # AXI
    VENDOR / "axi" / "src" / "axi_demux_simple.sv",
    VENDOR / "axi" / "src" / "axi_demux.sv",
    VENDOR / "axi" / "src" / "axi_mux.sv",
    VENDOR / "axi" / "src" / "axi_id_prepend.sv",
    VENDOR / "axi" / "src" / "axi_err_slv.sv",

    # banked transaction controller
    IOMMU / "rtl" / "transaction_controller" / "rv_iommu_trans_ctl_wrap.sv",
    IOMMU / "rtl" / "transaction_controller" / "rv_iommu_banked_ctl.sv",

    # translation logic
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_ddtc.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_ddtw.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_iotlb.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_ptw.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_e3_delay.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_nonblock_hit.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_tl_wrap.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_msiptw.sv",
    IOMMU / "rtl" / "translation_logic" / "rv_iommu_mrifc.sv",

    # harness
    ROOT / "tests" / "nonblock_banked_top.sv",
]

INCDIRS = [
    VENDOR / "axi" / "include",
    VENDOR / "common_cells" / "include",
]


def tail(path: Path, lines: int = 200) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def require(path: Path):
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def dedup(paths):
    seen, out = set(), []
    for p in paths:
        if p is None:
            continue
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def main() -> int:
    tb = ROOT / "tests" / "nonblock_banked_test.cpp"
    sources = dedup([require(p) for p in SOURCES])

    OUT.mkdir(parents=True, exist_ok=True)
    obj_dir = OUT / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    args = [
        "verilator",
        "--cc",
        "--exe",
        "--build",
        "-Wno-fatal",
        "+define+ASSERTS_OFF",
        "--top-module",
        TOP,
        "--Mdir",
        str(obj_dir),
    ]
    for inc in INCDIRS:
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

    run = subprocess.run([str(exe)])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
```

## Acceptance coverage / unresolved limits

Covered by the proposed test:

- Actual `rv_iommu_banked_ctl` connected to actual `rv_iommu_tl_wrap`.
- `NonblockEnable=1` at elaboration and runtime `nonblock_enable_i` toggling.
- `MSI_BT_ONLY`, extended `dc_ext_t` / `ddtc_up_ext_t`, 8-word extended DC layout.
- 1-level DDT at `0x10000`, stride 64, DID10/DID11 contexts.
- Bank select via original AXI ID bit 2; V=ID0/DID10, A=ID4/DID11.
- Capacities: total `NumOutstandingTrans=4`, `WFifoDepth=8`, `NumIotlbEntries=16`, `NumDdtcEntries=8`.
- Warm Vsrc/Vdst; cold A read; A first PTW response held.
- V cached read accepted/completed while A held when nonblock enabled.
- Nonblock disabled cannot translate/complete V until A released.
- A release drains exactly once with correct ID/address/data.
- Legal V write hit with 128-beat AXI burst and 4KiB read/write split into 2x256-beat bursts.
- Full payload/strb/last checking for completion writes; memory readback verification.
- Read-only V write returns SLVERR original ID and no external completion AW/W.
- Completion AW backpressure stability while stalled.
- Same-bank read ordering via completion AR address order.
- `in_flight_o` quiet drain after 20 idle cycles.
- C++-only memory/slave model; no forcing of caches; no HPM fake.
- Verilator 4.038 no timing coroutines; `sc_time_stamp()` returns 0.
- `ASSERTS_OFF` for vendor SVA; DUT immediate `$error`/`$fatal` remain enabled.
- Runner saves build log and only surfaces it on failure.

Unresolved / assumptions to verify:

- Extended DC word order is assumed to be ascending `tc, iohgatp, ta, fsc, msiptp, msi_addr_mask, msi_addr_pattern, reserved`, derived from `dc_ext_t` packed order and existing base-DC tests. If `rv_iommu_ddtw` uses a different extended layout, only `setup_memory()` needs adjustment.
- The source list is based on the existing `banked_ctl_test.py` and `nonblock_tl_test.py` unions plus `rv_iommu_msiptw.sv` and `rv_iommu_mrifc.sv`. If `rv_iommu_trans_ctl_wrap.sv` actually requires `rv_iommu_trans_ctl.sv`, that file must be added to `SOURCES`.
- The runner assumes `rv_iommu_msiptw.sv` and `rv_iommu_mrifc.sv` exist at the listed paths and that their own closure is already covered by the union above.
- Build/run has not been executed here. The parent should run `python3 tests/test_nonblock_banked.py` and inspect `out/nonblock-banked/build.log` only on failure.
- If the actual `rv_iommu_ddtw` issues a different burst length for extended DC, the C++ read model already supports arbitrary `len`, but the memory image may need more/fewer words.
- The test does not model or force caches, HPM, or MRIF, consistent with the requested scope.