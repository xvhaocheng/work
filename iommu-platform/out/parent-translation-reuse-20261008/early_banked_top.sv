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
    parameter bit NonblockEnable = 1,
    parameter bit BankedEnable = 1,
    parameter int unsigned NumOutstandingTrans = 4,
    parameter int unsigned WFifoDepth = 8,
    // Test-only elaboration switch for rv_iommu_tl_wrap.PolicyEnable.
    // 0 (default) reproduces the historical elaboration exactly; the Python
    // runner passes -GPolicyEnable=1 only for the opt-in --policy build.
    parameter bit PolicyEnable = 0,
    parameter bit CompletionGateEnable = 0,
    parameter int unsigned HeaderParkMode = 0,
    parameter int unsigned HeaderParkDepth = 2,
    parameter bit HeaderDirectEnable = 1,
    parameter int unsigned BankIdBit = 2
) (
    input  logic        clk_i,
    input  logic        rst_ni,
    input logic reuse_enable_i, reuse_prepare_i,
    input logic [1:0] reuse_parent_valid_i, reuse_parent_bank_i,
    input logic [1:0][63:0] reuse_parent_addr_i,
    input logic [1:0][23:0] reuse_parent_did_i,
    input logic [1:0][19:0] reuse_parent_pid_i,
    input logic [1:0] reuse_parent_pid_valid_i, reuse_parent_priv_i,
    input logic [1:0][31:0] reuse_parent_seq_i,

    // Test-only translation-policy input, forwarded verbatim to
    // rv_iommu_tl_wrap.policy_partition_i.  It has an effect only in a build
    // elaborated with PolicyEnable=1; 0 selects the full IOTLB replacement
    // mask (identical replacement behaviour to PolicyEnable=0).
    input  logic        policy_partition_i,

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
    output logic        in_flight_o,

    // ---------------- v5 full-buffer job permits (test input) ----------------
    // Bit b is the bank-b full-buffer admission permit.  2'b00 preserves the
    // legacy guarded behaviour; the coupled harness drives 2'b11 only under
    // the opt-in --fullbuffer-permit proxy.
    input  logic [1:0]  fullbuffer_jobs_i,

    input logic [1:0] late_comp_ar_admit_i,
    output logic [1:0] late_comp_ar_valid_o, late_comp_ar_fire_o,
    output logic [1:0][3:0] late_comp_ar_id_o,
    output logic [1:0][63:0] late_comp_ar_addr_o,
    output logic [1:0][7:0] late_comp_ar_len_o,
    output logic [1:0][2:0] late_comp_ar_size_o,


    // ---------------- test-only observation (no DUT behaviour change) -------
    // These ports only tap existing internal wires (trans_req_* / trans_resp_*
    // of the joint harness and the rv_iommu_tl_wrap e3_tl observer).  They do
    // not drive anything inside either DUT.
    output logic        dbg_tl_req_valid,
    output logic        dbg_tl_req_ready,
    output logic        dbg_tl_req_bank,
    output logic        dbg_tl_resp_valid,
    output logic        dbg_tl_resp_ready,
    output logic        dbg_tl_resp_bank,
    output logic        dbg_tl_ptw_active,
    output logic        dbg_tl_ptw_r_fire,
    output logic        dbg_tl_iotlb_lookup,
    output logic        dbg_tl_iotlb_hit
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
        c.NumOutstandingTrans = NumOutstandingTrans;
        c.WFifoDepth          = WFifoDepth;
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
    rv_iommu::e3_tl_t       tl_e3;
    always_comb begin
        iotinval_i   = '0;
        iodirinval_i = '0;
    end

    // Observation-only taps (see the port list).  Field names come from
    // rv_iommu_tl_wrap.sv:689-703; trans_req_* / trans_resp_* are the harness
    // wires declared above at :360-365.
    assign dbg_tl_req_valid    = memo_valid;
    assign dbg_tl_req_ready    = memo_ready;
    assign dbg_tl_req_bank     = memo_req.bank;
    assign dbg_tl_resp_valid   = memo_resp_valid;
    assign dbg_tl_resp_ready   = memo_resp_ready;
    assign dbg_tl_resp_bank    = memo_resp.bank;
    assign dbg_tl_ptw_active   = tl_e3.ptw_active;
    assign dbg_tl_ptw_r_fire   = tl_e3.ptw_r_fire;
    assign dbg_tl_iotlb_lookup = tl_e3.lookup;
    assign dbg_tl_iotlb_hit    = tl_e3.hit;

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
`ifndef TEST_COMPLETION_GATE
    initial if (CompletionGateEnable) $fatal(1, "completion gate needs TEST_COMPLETION_GATE build macro");
`endif
    // DUT : banked transaction controller
    // -------------------------------------------------------------------------
    if (BankedEnable) begin : gen_banked
`ifdef TEST_COMPLETION_GATE
    nonblock_completion_banked_ctl #(
        .CompletionGateEnable(CompletionGateEnable),
        .HeaderParkMode(HeaderParkMode), .HeaderParkDepth(HeaderParkDepth),
        .HeaderDirectEnable(HeaderDirectEnable),
`else
    rv_iommu_banked_ctl #(
`endif
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .BankIdBit          (BankIdBit),
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
        .fullbuffer_jobs_i  (fullbuffer_jobs_i),
`ifdef TEST_COMPLETION_GATE
        .completion_ar_admit_i(bank_completion_admit),
        .completion_stream_admit_i(late_comp_ar_admit_i),
`endif
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
    logic [1:0] bank_completion_admit;
    always_comb begin : map_completion_streams
        late_comp_ar_valid_o = '0; late_comp_ar_fire_o = '0;
        late_comp_ar_id_o = '0; late_comp_ar_addr_o = '0;
        late_comp_ar_len_o = '0; late_comp_ar_size_o = '0;
        bank_completion_admit = '0;
        for (int b = 0; b < 2; b++) begin
`ifdef TEST_COMPLETION_GATE
            if (HeaderParkMode != 0) begin
                for (int d = 0; d < 2; d++) begin
                    if (i_banked.header_candidate_valid[b][d]) begin
                        late_comp_ar_valid_o[d] = 1'b1;
                        late_comp_ar_fire_o[d] = i_banked.header_candidate_fire[b][d];
                        late_comp_ar_id_o[d] = i_banked.header_candidate[b][d].id;
                        late_comp_ar_addr_o[d] = i_banked.header_candidate[b][d].addr;
                        late_comp_ar_len_o[d] = i_banked.header_candidate[b][d].len;
                        late_comp_ar_size_o[d] = i_banked.header_candidate[b][d].size;
                    end
                end
            end else begin
`endif
            int d;
            d = int'(i_banked.bank_comp_req[b].ar.id[2]);
            bank_completion_admit[b] = late_comp_ar_admit_i[d];
            if (i_banked.bank_comp_req[b].ar_valid) begin
                late_comp_ar_valid_o[d] = 1'b1;
                late_comp_ar_fire_o[d] = i_banked.bank_comp_resp[b].ar_ready;
                late_comp_ar_id_o[d] = i_banked.bank_comp_req[b].ar.id;
                late_comp_ar_addr_o[d] = i_banked.bank_comp_req[b].ar.addr;
                late_comp_ar_len_o[d] = i_banked.bank_comp_req[b].ar.len;
                late_comp_ar_size_o[d] = i_banked.bank_comp_req[b].ar.size;
            end
`ifdef TEST_COMPLETION_GATE
            end
`endif
        end
    end

    end else begin : gen_legacy
    assign late_comp_ar_valid_o = '0;
    assign late_comp_ar_fire_o = '0;
    assign late_comp_ar_id_o = '0;
    assign late_comp_ar_addr_o = '0;
    assign late_comp_ar_len_o = '0;
    assign late_comp_ar_size_o = '0;
    rv_iommu_trans_ctl_wrap #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .addr_t             (addr_t),
        .paddr_t            (paddr_t),
        .trans_req_data_t   (trans_req_data_t),
        .trans_resp_data_t  (trans_resp_data_t),
        .fault_data_t       (fault_data_t),
        .axi_tr_aw_chan_t   (tr_aw_chan_t),
        .axi_tr_w_chan_t    (tr_w_chan_t),
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
    ) i_legacy (
        .clk_i,
        .rst_ni,
        .read_admit_i(1'b1),
        .fullbuffer_job_i(1'b0),
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
    end


    // -------------------------------------------------------------------------
    // DUT : actual translation logic
    // -------------------------------------------------------------------------
    trans_req_data_t memo_req;
    trans_resp_data_t memo_resp;
    logic memo_valid, memo_ready, memo_resp_valid, memo_resp_ready;
    parent_translation_reuse #(.req_t(trans_req_data_t), .resp_t(trans_resp_data_t)) i_reuse (
        .clk_i, .rst_ni, .enable_i(reuse_enable_i), .prepare_i(reuse_prepare_i),
        .flush_i((iotinval_i != '0) || (iodirinval_i != '0)),
        .parent_valid_i(reuse_parent_valid_i), .parent_bank_i(reuse_parent_bank_i), .parent_addr_i(reuse_parent_addr_i),
        .parent_did_i(reuse_parent_did_i), .parent_pid_i(reuse_parent_pid_i),
        .parent_pid_valid_i(reuse_parent_pid_valid_i), .parent_priv_i(reuse_parent_priv_i),
        .parent_seq_i(reuse_parent_seq_i),
        .s_req_i(trans_req_data), .s_valid_i(trans_req_valid), .s_ready_o(trans_req_ready),
        .s_resp_o(trans_resp_data), .s_resp_valid_o(trans_resp_valid), .s_resp_ready_i(trans_resp_ready),
        .m_req_o(memo_req), .m_valid_o(memo_valid), .m_ready_i(memo_ready),
        .m_resp_i(memo_resp), .m_resp_valid_i(memo_resp_valid), .m_resp_ready_o(memo_resp_ready)
    );

    rv_iommu_tl_wrap #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .NonblockEnable     (NonblockEnable),
        .PolicyEnable       (PolicyEnable),
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
        .trans_req_data_i       (memo_req),
        .trans_req_valid_i      (memo_valid),
        .trans_req_ready_o      (memo_ready),
        .ats_trans_req_data_i   (ats_trans_req_data_i),
        .ats_trans_req_valid_i  (ats_trans_req_valid_i),
        .ats_trans_req_ready_o  (),
        .debug_req_data_i       (debug_req_data_i),
        .debug_req_valid_i      (debug_req_valid_i),
        .debug_req_ready_o      (),
        .trans_resp_data_o      (memo_resp),
        .trans_resp_valid_o     (memo_resp_valid),
        .trans_resp_ready_i     (memo_resp_ready),
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
        .e3_o                   (tl_e3),
        .research_o             (),
        .nonblock_enable_i      (nonblock_enable_i),
        .nonblock_fire_o        (nonblock_fire_o),
        .nonblock_busy_o        (nonblock_busy_o),

        // Test-only policy plumbing.  policy_did_i is hard-wired to the
        // protected victim DID (V_DID == 10 in the C++ harness).  policy_clear_i
        // is driven from the existing reset, so the IOTLB can never be cleared
        // while a translation is in flight (there is no traffic during reset).
        .policy_partition_i     (policy_partition_i),
        .policy_clear_i         (!rst_ni),
        .policy_did_i           (24'd10),
        .policy_quiet_o         (),
        .nonblock_diag_o        ()
    );

endmodule
