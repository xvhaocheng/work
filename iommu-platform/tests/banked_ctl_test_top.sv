// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Thin, purely-structural scalar-I/O harness around rv_iommu_banked_ctl.
//
// No clock generation, no event controls, no `initial` stimulus and no
// behavioural models: it only instantiates the DUT and breaks every packed
// struct port out into flat scalars (and back).  All stimulus and checking
// lives in tests/banked_ctl_test.cpp.

`include "axi/typedef.svh"

module banked_ctl_test_top (
    input  logic         clk_i,
    input  logic         rst_ni,

    // v5 full-buffer job permits: bit b is the bank-b permit.
    // Existing tests keep this at 2'b00.
    input  logic [1:0]   fullbuffer_jobs_i,

    // ---------------- upstream translation-request AXI : AW ------------------
    input  logic         tr_aw_valid,
    input  logic [3:0]   tr_aw_id,
    input  logic [63:0]  tr_aw_addr,
    input  logic [7:0]   tr_aw_len,
    input  logic [2:0]   tr_aw_size,
    input  logic [1:0]   tr_aw_burst,
    input  logic [2:0]   tr_aw_prot,
    input  logic [0:0]   tr_aw_user,
    input  logic [23:0]  tr_aw_did,          // device id / stream_id (24 b)
    input  logic         tr_aw_ssid_valid,
    input  logic [19:0]  tr_aw_subid,        // substream / process id (20 b)
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

    // ---------------- upstream translation-request AXI : responses ----------
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

    // ---------------- completion AXI : requests (DUT master) ---------------
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

    // ---------------- completion AXI : responses (external slave) ----------
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

    // ---------------- translation-engine request (DUT output) ---------------
    output logic         trans_req_valid,
    output logic         trans_req_bank,
    output logic [63:0]  trans_req_iova,
    output logic [23:0]  trans_req_did,
    output logic         trans_req_pid_valid,
    output logic [19:0]  trans_req_pid,
    output logic [5:0]   trans_req_ttype,
    output logic         trans_req_priv,
    output logic         trans_req_is_debug,
    input  logic         trans_req_ready,

    // ---------------- translation-engine response (DUT input) ---------------
    input  logic         trans_resp_valid,
    input  logic         trans_resp_bank,
    input  logic         trans_resp_error,
    input  logic [63:0]  trans_resp_spaddr,
    output logic         trans_resp_ready,

    output logic         in_flight_o,
    output logic         e3_quiet
);

    //--------------------------------------------------------------------------
    // Fixed test configuration
    //--------------------------------------------------------------------------
    localparam int unsigned AxiIdWidth   = 4;
    localparam int unsigned AxiAddrWidth = 64;
    localparam int unsigned AxiDataWidth = 64;
    localparam int unsigned AxiUserWidth = 1;

    typedef logic [AxiIdWidth-1:0]     id_t;
    typedef logic [AxiAddrWidth-1:0]   addr_t;
    typedef logic [AxiDataWidth-1:0]   data_t;
    typedef logic [AxiDataWidth/8-1:0] strb_t;
    typedef logic [AxiUserWidth-1:0]   user_t;
    typedef logic [23:0]               did_t;    // device/stream id
    typedef logic [19:0]               subid_t;  // substream/process id

    //--------------------------------------------------------------------------
    // Translation-request channel types (AXI + IOMMU extensions).  `id` stays
    // the MSB of every channel so that axi_id_prepend's ID strip/re-add works.
    //--------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------
    // Completion channel types (plain AXI, ORIGINAL ID width)
    //--------------------------------------------------------------------------
    `AXI_TYPEDEF_AW_CHAN_T(comp_aw_chan_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_W_CHAN_T (comp_w_chan_t,  data_t, strb_t, user_t)
    `AXI_TYPEDEF_B_CHAN_T (comp_b_chan_t,  id_t, user_t)
    `AXI_TYPEDEF_AR_CHAN_T(comp_ar_chan_t, addr_t, id_t, user_t)
    `AXI_TYPEDEF_R_CHAN_T (comp_r_chan_t,  data_t, id_t, user_t)
    `AXI_TYPEDEF_REQ_T (axi_comp_req_t,  comp_aw_chan_t, comp_w_chan_t, comp_ar_chan_t)
    `AXI_TYPEDEF_RESP_T(axi_comp_resp_t, comp_b_chan_t,  comp_r_chan_t)

    //--------------------------------------------------------------------------
    // Translation engine data types.  Widths match the real IOMMU translation
    // request/response: ttype 6 b, DID 24 b, PID 20 b.
    //--------------------------------------------------------------------------
    typedef struct packed {
        logic [5:0]  ttype;
        logic        priv;
        logic        is_debug;
        logic        pid_valid;
        subid_t      pid;
        did_t        did;
        addr_t       iova;
        logic        bank;
    } trans_req_data_t;

    typedef struct packed {
        logic  error;
        logic  is_mrif;
        logic  ignore;
        addr_t spaddr;
        logic  bank;
    } trans_resp_data_t;

    typedef logic fault_data_t;
    typedef logic axi_ds_req_t;
    typedef logic axi_ds_resp_t;

    //--------------------------------------------------------------------------
    // Fixed configuration: ID 4, addr 64, data 64, user 1, 4 original slots,
    // W depth 8, bank bit 2.
    //
    // Built by copying NullCfg and overriding only the fields the test cares
    // about.  `'{default: NullCfg, ...}` would apply the whole 808-bit struct
    // to *every* field (WIDTH); copying keeps each field's package width and
    // its package default (AxisDataWidth, ... are NOT zero in general).
    //--------------------------------------------------------------------------
    function automatic rv_iommu_cfg::rv_iommu_cfg_t test_cfg();
        rv_iommu_cfg::rv_iommu_cfg_t c;
        c                     = rv_iommu_cfg::NullCfg;
        c.NumOutstandingTrans = 4;
        c.WFifoDepth          = 8;
        c.InclAxiBC           = 1'b0;
        c.MSITrans            = rv_iommu_cfg::MSI_DISABLED;
        c.AxiIdWidth          = 4;
        c.AxiAddrWidth        = 64;
        c.AxiDataWidth        = 64;
        c.AxiUserWidth        = 1;
        c.PAddrWidth          = 64;
        return c;
    endfunction

    localparam rv_iommu_cfg::rv_iommu_cfg_t Cfg = test_cfg();

    axi_tr_req_t    tr_req;
    axi_tr_resp_t   tr_resp;
    axi_comp_req_t  comp_req;
    axi_comp_resp_t comp_resp;

    trans_req_data_t  trans_req_data;
    trans_resp_data_t trans_resp_data;

    rv_iommu::e3_ctl_t e3_o;

    //--------------------------------------------------------------------------
    // Pack the flat upstream inputs into the request struct
    //--------------------------------------------------------------------------
    always_comb begin : pack_tr_req
        tr_req          = '0;

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

    //--------------------------------------------------------------------------
    // Unpack the upstream responses
    //--------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------
    // Completion master expansion
    //--------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------
    // Completion slave packing
    //--------------------------------------------------------------------------
    always_comb begin : pack_comp_resp
        comp_resp          = '0;
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

    //--------------------------------------------------------------------------
    // Translation engine response unpack / request expansion
    //--------------------------------------------------------------------------
    assign trans_resp_data.bank    = trans_resp_bank;
    assign trans_resp_data.error   = trans_resp_error;
    assign trans_resp_data.spaddr  = trans_resp_spaddr;
    assign trans_resp_data.is_mrif = 1'b0;
    assign trans_resp_data.ignore  = 1'b0;

    assign trans_req_bank      = trans_req_data.bank;
    assign trans_req_iova      = trans_req_data.iova;
    assign trans_req_did       = trans_req_data.did;
    assign trans_req_pid_valid = trans_req_data.pid_valid;
    assign trans_req_pid       = trans_req_data.pid;
    assign trans_req_ttype     = trans_req_data.ttype;
    assign trans_req_priv      = trans_req_data.priv;
    assign trans_req_is_debug  = trans_req_data.is_debug;

    assign e3_quiet = e3_o.quiet;

    //--------------------------------------------------------------------------
    // DUT
    //--------------------------------------------------------------------------
    rv_iommu_banked_ctl #(
        .RVIOMMUCfg         (Cfg),
        .E3Enable           (1'b0),
        .BankIdBit          (2),
        .addr_t             (addr_t),
        .paddr_t            (addr_t),
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
    ) i_dut (
        .clk_i,
        .rst_ni,
        .fullbuffer_jobs_i  (fullbuffer_jobs_i),
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
        .e3_o               (e3_o)
    );

endmodule
