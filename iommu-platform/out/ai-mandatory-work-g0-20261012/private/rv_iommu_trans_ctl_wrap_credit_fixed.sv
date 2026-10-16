// Copyright © 2025 Manuel Rodríguez & Zero-Day Labs, Lda.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

// Licensed under the Solderpad Hardware License v 2.1 (the “License”);
// you may not use this file except in compliance with the License,
// or, at your option, the Apache License version 2.0.
// You may obtain a copy of the License at https://solderpad.org/licenses/SHL-2.1/.
// Unless required by applicable law or agreed to in writing,
// any work distributed under the License is distributed on an “AS IS” BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
//
// Author: Manuel Rodríguez <manuel.cederog@gmail.com>
// Date: 20/01/2025
// Acknowledges: SSRC - Technology Innovation Institute (TII)
//
// Description: RISC-V IOMMU Transaction Controller Wrapper Module.

module rv_iommu_trans_ctl_wrap #(
    /// RISC-V IOMMU configuration struct
    parameter rv_iommu_cfg::rv_iommu_cfg_t RVIOMMUCfg = rv_iommu_cfg::NullCfg,
    parameter bit E3Enable = 1'b0,
    /// Banked DMA integration may hold new reads behind the upstream W owner.
    /// Legacy instances keep their original behavior when this is disabled.
    parameter bit ReadAdmissionEnable = 1'b0,

    /// Address data type
    parameter type addr_t = logic,
    /// Physical address data type
    parameter type paddr_t = logic,

    /// Translation request data type
    parameter type trans_req_data_t = logic,
    /// Translation response data type
    parameter type trans_resp_data_t = logic,
    /// Fault data type
    parameter type fault_data_t = logic,

    // Translation Request Interface data types
    parameter type axi_tr_aw_chan_t = logic,
    parameter type axi_tr_w_chan_t = logic,
    parameter type axi_tr_ar_chan_t = logic,
    parameter type axi_tr_req_t = logic,
    parameter type axi_tr_resp_t = logic,

    // Completion Interface data types
    parameter type axi_comp_aw_chan_t = logic,
    parameter type axi_comp_w_chan_t = logic,
    parameter type axi_comp_b_chan_t = logic,
    parameter type axi_comp_ar_chan_t = logic,
    parameter type axi_comp_r_chan_t = logic,
    parameter type axi_comp_req_t = logic,
    parameter type axi_comp_resp_t = logic,

    // Data Structures Interface data types
    parameter type axi_ds_req_t = logic,
    parameter type axi_ds_resp_t = logic,
    parameter logic [23:0] FullBufferDID = 24'd0
) (
    input  logic clk_i,
    input  logic rst_ni,
    input  logic read_admit_i,
    input  logic fullbuffer_job_i,

    // AXI Translation Request Interface
    input  axi_tr_req_t    tr_axi_req_i,
    output axi_tr_resp_t   tr_axi_resp_o,

    // AXI Translation Completion Interface
    input  axi_comp_resp_t comp_axi_resp_i,
    output axi_comp_req_t  comp_axi_req_o,

    // MRIF Handler AXI IF
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

    // MRIF Handler fault reporting port
    output fault_data_t         mrif_fault_data_o,
    output logic                mrif_fault_valid_o,
    input  logic                mrif_fault_ready_i,

    output logic in_flight_o,
    output rv_iommu::e3_ctl_t e3_o
);

    typedef enum logic [2:0] {
        IDLE        = 3'b000,
        REQ_TRANS   = 3'b001,
        AWAIT_RESP  = 3'b010,
        MRIF        = 3'b011,
        COMPLETE    = 3'b100
    } trans_ctl_state_t;
    trans_ctl_state_t state_q, state_n;

    assign in_flight_o = (state_q != IDLE);

    trans_req_data_t trans_req_data_q, trans_req_data_n;
    trans_resp_data_t trans_resp_data_q, trans_resp_data_n;

    axi_comp_req_t     axi_comp_req;
    axi_comp_resp_t    axi_comp_resp;

    //----------
    // AX FIFOs
    //----------
    axi_tr_aw_chan_t aw_data;
    logic aw_valid;
    logic aw_ready;
    logic tr_aw_ready;

    stream_fifo #(
        .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
        .T    (axi_tr_aw_chan_t)
    ) i_aw_stream_fifo (
        .clk_i      (clk_i),
        .rst_ni     (rst_ni),

        .flush_i    ('0),
        .testmode_i ('0),
        .usage_o    (  ),

        .data_i     (tr_axi_req_i.aw),
        .valid_i    (tr_axi_req_i.aw_valid),
        .ready_o    (tr_aw_ready),

        .data_o     (aw_data),
        .valid_o    (aw_valid),
        .ready_i    (aw_ready)
    );

    axi_tr_ar_chan_t ar_data;
    logic ar_valid;
    logic ar_ready;
    logic tr_ar_ready;

    stream_fifo #(
        .DEPTH(RVIOMMUCfg.NumOutstandingTrans),
        .T    (axi_tr_ar_chan_t)
    ) i_ar_stream_fifo (
        .clk_i      (clk_i),
        .rst_ni     (rst_ni),

        .flush_i    ('0),
        .testmode_i ('0),
        .usage_o    (  ),

        .data_i     (tr_axi_req_i.ar),
        .valid_i    (tr_axi_req_i.ar_valid),
        .ready_o    (tr_ar_ready),

        .data_o     (ar_data),
        .valid_o    (ar_valid),
        .ready_i    (ar_ready)
    );

    //--------
    // W FIFO
    //--------
    axi_tr_w_chan_t w_data;
    logic w_valid;
    logic w_ready;
    logic tr_w_ready;

    typedef logic [$clog2(RVIOMMUCfg.NumOutstandingTrans):0] w_cnt_t;
    w_cnt_t w_cnt_q;
    logic incr_w_cnt;
    logic decr_w_cnt;
    assign incr_w_cnt = (axi_comp_req.aw_valid & axi_comp_resp.aw_ready);
    assign decr_w_cnt = (w_valid & w_ready & w_data.last);
    logic w_cnt_ovf;

    logic [31:0] e3_waiting;
    logic e3_queue_error;
    if (E3Enable) begin : gen_e3_queue
        rv_iommu_e3_queue #(.Depth(RVIOMMUCfg.NumOutstandingTrans)) i_queue (
            .clk_i, .rst_ni,
            .ar_fire_i(tr_axi_req_i.ar_valid && tr_ar_ready && tr_axi_req_i.ar.id == '0),
            .ar_id_i(8'(tr_axi_req_i.ar.id)), .ar_did_i(tr_axi_req_i.ar.stream_id),
            .select_i(e3_o.select_valid && ar_data.id == '0), .select_write_i(e3_o.select_write),
            .select_id_i(e3_o.select_id), .select_did_i(e3_o.select_did),
            .waiting_o(e3_waiting), .error_o(e3_queue_error)
        );
    end else begin : gen_no_e3_queue
        assign e3_waiting = 0;
        assign e3_queue_error = 0;
    end
    always_comb begin
        e3_o = '0;
        e3_o.victim_waiting = e3_waiting;
        e3_o.queue_error = e3_queue_error;
        e3_o.state = state_q;
        e3_o.current_did = trans_req_data_q.did;
        e3_o.current_iova = 64'(trans_req_data_q.iova);
        e3_o.quiet = state_q == IDLE && !aw_valid && !ar_valid;
        e3_o.select_valid = state_q == IDLE &&
            ((aw_valid && !w_cnt_ovf) || ar_valid);
        e3_o.select_write = aw_valid && !w_cnt_ovf;
        e3_o.select_id = e3_o.select_write ? 8'(aw_data.id) : 8'(ar_data.id);
        e3_o.select_did = e3_o.select_write ? aw_data.stream_id : ar_data.stream_id;
        e3_o.select_iova = e3_o.select_write ? 64'(aw_data.addr) : 64'(ar_data.addr);
        e3_o.head_valid = ar_valid;
        e3_o.head_id = 8'(ar_data.id);
        e3_o.head_did = ar_data.stream_id;
        e3_o.head_iova = 64'(ar_data.addr);
        e3_o.request_fire = trans_req_valid_o && trans_req_ready_i;
        e3_o.response_fire = trans_resp_valid_i && trans_resp_ready_o;
        e3_o.response_error = trans_resp_data_i.error;
    end

    assign w_ready = (w_cnt_q != '0 || w_cnt_ovf) ? (axi_comp_resp.w_ready) : (1'b0);

    stream_fifo #(
        .DEPTH(RVIOMMUCfg.WFifoDepth),
        .T    (axi_tr_w_chan_t)
    ) i_w_stream_fifo (
        .clk_i      (clk_i),
        .rst_ni     (rst_ni),

        .flush_i    ('0),
        .testmode_i ('0),
        .usage_o    (  ),

        .data_i     (tr_axi_req_i.w),
        .valid_i    (tr_axi_req_i.w_valid),
        .ready_o    (tr_w_ready),

        .data_o     (w_data),
        .valid_o    (w_valid),
        .ready_i    (w_ready)
    );

    counter #(
        .WIDTH              ($clog2(RVIOMMUCfg.NumOutstandingTrans)+1),
        .STICKY_OVERFLOW    (1'b0)
    ) i_w_counter (
        .clk_i      (clk_i),
        .rst_ni     (rst_ni),
        .clear_i    (1'b0),
        .en_i       (incr_w_cnt ^ decr_w_cnt),
        .load_i     (1'b0),
        .down_i     (decr_w_cnt),
        .d_i        ('0),
        .q_o        (w_cnt_q),
        .overflow_o (w_cnt_ovf)
    );

    //-------------------------
    // AXI 4kiB Boundary Check
    //-------------------------
    logic               check_valid;
    addr_t              check_iova;
    axi_pkg::burst_t    check_burst;
    axi_pkg::len_t      check_length;
    axi_pkg::size_t     check_nbytes;

    logic is_read;
    assign is_read = ((trans_req_data_q.ttype == rv_iommu::UNTRANSLATED_RX) ||
                      (trans_req_data_q.ttype == rv_iommu::UNTRANSLATED_R ) ||
                      (trans_req_data_q.ttype == rv_iommu::TRANSLATED_R )   ||
                      (trans_req_data_q.ttype == rv_iommu::TRANSLATED_R )   );
    logic is_write;
    assign is_write = (trans_req_data_q.ttype == rv_iommu::UNTRANSLATED_W ||
                       trans_req_data_q.ttype == rv_iommu::TRANSLATED_W   );

    generate
    if (RVIOMMUCfg.InclAxiBC) begin : gen_axi_bc

        logic check_bc;

        always_comb begin

            check_bc = (state_q == REQ_TRANS);

            if (is_write) begin
                check_iova      = aw_data.addr;
                check_burst     = aw_data.burst;
                check_length    = aw_data.len;
                check_nbytes    = aw_data.size;
            end

            else begin
                check_iova      = ar_data.addr;
                check_burst     = ar_data.burst;
                check_length    = ar_data.len;
                check_nbytes    = ar_data.size;
            end
        end

        rv_iommu_axi_bc #(
            .addr_t             (addr_t)
        ) i_rv_iommu_axi_bc
        (
            .check_i            (check_bc),
            .addr_i             (check_iova),
            .burst_type_i       (check_burst),
            .burst_length_i     (check_length),
            .n_bytes_i          (check_nbytes),

            .allow_request_o    (check_valid)
        );
    end

    else begin : gen_axi_bc_disabled

        assign check_valid  = 1'b1;
        assign check_iova   = '0;
        assign check_burst  = '0;
        assign check_length = '0;
        assign check_nbytes = '0;
    end
    endgenerate

    //--------------------------------
    // AXI Completion Bus Connections
    //--------------------------------
    logic complete_read;
    logic complete_write;

    always_comb begin : completion_bus

        /* Request */
        axi_comp_req.aw.id      = aw_data.id;
        axi_comp_req.aw.addr    = addr_t'(trans_resp_data_q.spaddr);
        axi_comp_req.aw.len     = aw_data.len;
        axi_comp_req.aw.size    = aw_data.size;
        axi_comp_req.aw.burst   = aw_data.burst;
        axi_comp_req.aw.lock    = aw_data.lock;
        axi_comp_req.aw.cache   = aw_data.cache;
        axi_comp_req.aw.prot    = aw_data.prot;
        axi_comp_req.aw.qos     = aw_data.qos;
        axi_comp_req.aw.region  = aw_data.region;
        axi_comp_req.aw.atop    = aw_data.atop;
        axi_comp_req.aw.user    = aw_data.user;
        axi_comp_req.aw_valid   = complete_write;

        axi_comp_req.ar.id      = ar_data.id;
        axi_comp_req.ar.addr    = addr_t'(trans_resp_data_q.spaddr);
        axi_comp_req.ar.len     = ar_data.len;
        axi_comp_req.ar.size    = ar_data.size;
        axi_comp_req.ar.burst   = ar_data.burst;
        axi_comp_req.ar.lock    = ar_data.lock;
        axi_comp_req.ar.cache   = ar_data.cache;
        axi_comp_req.ar.prot    = ar_data.prot;
        axi_comp_req.ar.qos     = ar_data.qos;
        axi_comp_req.ar.region  = ar_data.region;
        axi_comp_req.ar.user    = ar_data.user;
        axi_comp_req.ar_valid   = complete_read;

        axi_comp_req.w          = w_data;
        axi_comp_req.w_valid    = w_valid & (w_cnt_q != '0 || w_cnt_ovf);
        axi_comp_req.r_ready    = tr_axi_req_i.r_ready;
        axi_comp_req.b_ready    = tr_axi_req_i.b_ready;

        /* Response */
        tr_axi_resp_o           = axi_comp_resp;
        tr_axi_resp_o.aw_ready  = tr_aw_ready;
        tr_axi_resp_o.ar_ready  = tr_ar_ready;
        tr_axi_resp_o.w_ready   = tr_w_ready;
    end

    //-----------------
    // Slave Selection
    //-----------------

    // MRIF Handler request struct
    typedef struct packed {
        trans_req_data_t            req_data;
        rv_iommu::mrifc_content_t   mrif_data;
        w_cnt_t                     mrif_w_cnt;
    } mrif_req_data_t;
    mrif_req_data_t mrif_req_data_d, mrif_req_data_q;
    logic mrif_req_valid_d, mrif_req_valid_q;
    logic mrif_req_ready_d, mrif_req_ready_q;

    // Error slave bus
    axi_comp_req_t   axi_err_req;
    axi_comp_resp_t  axi_err_resp;

    generate
    if (RVIOMMUCfg.MSITrans == rv_iommu_cfg::MSI_BT_MRIF) begin : gen_mrif_support

        // Bus to ignore (discard) MRIF transactions
        axi_comp_req_t    axi_ign_req;
        axi_comp_resp_t   axi_ign_resp;

        // Slave index
        logic [1:0] slv_idx;

        always_comb begin : slave_selection_mrif

            // Translation Error. Connect to Error Slave
            if (trans_resp_data_q.error) begin
                slv_idx = 2'b00;
            end

            // MRIF Transaction. Connect to Ignore Slave
            else if (trans_resp_data_q.is_mrif || trans_resp_data_q.ignore) begin
                slv_idx = 2'b10;
            end

            // Successful Normal Transaction. Connect to Completion Interface
            else begin
                slv_idx = 2'b01;
            end
        end

        // Ignore Slave
        rv_iommu_ign_slv #(
            .axi_req_t      (axi_comp_req_t),
            .axi_resp_t     (axi_comp_resp_t),
            .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
            .RespWidth      (RVIOMMUCfg.AxiDataWidth),
            .RespData       ('h0),
            .Resp           (axi_pkg::RESP_OKAY)
        ) i_rv_iommu_ign_slv (
            .clk_i          (clk_i),
            .rst_ni         (rst_ni),
            .ignore_req_i   (axi_ign_req),
            .ignore_resp_o  (axi_ign_resp)
        );

        // AXI Demux
        axi_demux #(
            .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
            .aw_chan_t      (axi_comp_aw_chan_t),
            .w_chan_t       (axi_comp_w_chan_t),
            .b_chan_t       (axi_comp_b_chan_t),
            .ar_chan_t      (axi_comp_ar_chan_t),
            .r_chan_t       (axi_comp_r_chan_t),
            .axi_req_t      (axi_comp_req_t),
            .axi_resp_t     (axi_comp_resp_t),

            .NoMstPorts     (32'd3),
            .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
            .SpillAw        (1'b1),
            .SpillW         (1'b1),
            .SpillB         (1'b1),
            .SpillAr        (1'b1),
            .SpillR         (1'b1)
        ) i_iommu_axi_demux_mrif (
            .clk_i          (clk_i),
            .rst_ni         (rst_ni),
            .test_i         (1'b0),
            .slv_aw_select_i(slv_idx),
            .slv_ar_select_i(slv_idx),
            .slv_req_i      (axi_comp_req),
            .slv_resp_o     (axi_comp_resp),
            /* { 2: ignore slave (MRIF), 1: comp IF, 0: error slave } */
            .mst_reqs_o     ({axi_ign_req, comp_axi_req_o, axi_err_req}),
            .mst_resps_i    ({axi_ign_resp, comp_axi_resp_i, axi_err_resp})
        );

        //----------------------------------------
        // Memory-Resident Interrupt File Handler
        //----------------------------------------
        assign mrif_req_data_d.req_data     = trans_req_data_q;
        assign mrif_req_data_d.mrif_data    = trans_resp_data_q.mrif_data;
        assign mrif_req_data_d.mrif_w_cnt   = w_cnt_q;

        // MRIF FIFO
        stream_fifo #(
            .DEPTH (RVIOMMUCfg.MrifFifoDepth),
            .T     (mrif_req_data_t)
        ) i_mrif_fifo (
            .clk_i      (clk_i),
            .rst_ni     (rst_ni),
            .flush_i    ('0),
            .testmode_i ('0),
            .usage_o    (  ),

            .data_i     (mrif_req_data_d),
            .valid_i    (mrif_req_valid_d),
            .ready_o    (mrif_req_ready_q),

            .data_o     (mrif_req_data_q),
            .valid_o    (mrif_req_valid_q),
            .ready_i    (mrif_req_ready_d)
        );

        rv_iommu_mrif_handler #(
            .RVIOMMUCfg         (RVIOMMUCfg),
            .addr_t             (addr_t),
            .paddr_t            (paddr_t),
            .mrif_req_data_t    (mrif_req_data_t),
            .w_chan_t           (axi_tr_w_chan_t),
            .w_cnt_t            (w_cnt_t),
            .fault_data_t       (fault_data_t),
            .axi_req_t          (axi_ds_req_t),
        .axi_resp_t             (axi_ds_resp_t)
        ) i_rv_iommu_mrif_handler (
            .clk_i              (clk_i),
            .rst_ni             (rst_ni),

            .req_data_i         (mrif_req_data_q),
            .req_valid_i        (mrif_req_valid_q),
            .req_ready_o        (mrif_req_ready_d),

            .w_data_i           (w_data),
            .w_valid_i          (w_valid),
            .w_ready_i          (w_ready),

            .fault_valid_o      (mrif_fault_valid_o),
            .fault_ready_i      (mrif_fault_ready_i),
            .fault_data_o       (mrif_fault_data_o),

            .mem_resp_i         (mrif_axi_resp_i),
            .mem_req_o          (mrif_axi_req_o)
        );
    end : gen_mrif_support

    // Do not generate transaction ignoring mechanism
    else begin : gen_mrif_support_disabled

        // Slave index
        logic slv_idx;

        always_comb begin : slave_selection_no_mrif

            // Translation Error. Connect to Error Slave
            if (trans_resp_data_q.error) begin
                slv_idx = 1'b0;
            end

            // Successful Normal Transaction. Connect to Completion Interface
            else begin
                slv_idx = 1'b1;
            end
        end

        // AXI demux
        axi_demux #(
            .AxiIdWidth     (RVIOMMUCfg.AxiIdWidth),
            .aw_chan_t      (axi_comp_aw_chan_t),
            .w_chan_t       (axi_comp_w_chan_t),
            .b_chan_t       (axi_comp_b_chan_t),
            .ar_chan_t      (axi_comp_ar_chan_t),
            .r_chan_t       (axi_comp_r_chan_t),
            .axi_req_t      (axi_comp_req_t),
            .axi_resp_t     (axi_comp_resp_t),

            .NoMstPorts     (32'd2),
            .AxiLookBits    (RVIOMMUCfg.AxiIdWidth),
            .SpillAw        (1'b1),
            .SpillW         (1'b1),
            .SpillB         (1'b1),
            .SpillAr        (1'b1),
            .SpillR         (1'b1)
        ) i_iommu_axi_demux_no_mrif (
            .clk_i          (clk_i),
            .rst_ni         (rst_ni),
            .test_i         (1'b0),
            .slv_aw_select_i(slv_idx),
            .slv_ar_select_i(slv_idx),
            .slv_req_i      (axi_comp_req),
            .slv_resp_o     (axi_comp_resp),
            /* { 1: comp IF, 0: error slave } */
            .mst_reqs_o     ({comp_axi_req_o, axi_err_req}),
            .mst_resps_i    ({comp_axi_resp_i, axi_err_resp})
        );

        assign mrif_req_data_d      = '0;
        assign mrif_req_ready_d     = 1'b0;
        assign mrif_req_data_q      = '0;
        assign mrif_req_valid_q     = 1'b0;
        assign mrif_req_ready_q     = 1'b0;
        assign mrif_axi_req_o       = '0;
        assign mrif_fault_valid_o   = 1'b0;
        assign mrif_fault_data_o    = '0;
    end : gen_mrif_support_disabled
    endgenerate

    // IOMMU Error Slave
    axi_err_slv #(
        .axi_req_t    (axi_comp_req_t),
        .axi_resp_t   (axi_comp_resp_t),
        .AxiIdWidth   (RVIOMMUCfg.AxiIdWidth),
        .RespWidth    (RVIOMMUCfg.AxiDataWidth),
        .Resp         (axi_pkg::RESP_SLVERR),
        .RespData     (64'hCA11AB1EBADCAB1E),
        .ATOPs        (1'b0),
        .MaxTrans     (1)
    ) i_iommu_axi_err_slv (
        .clk_i        (clk_i),
        .rst_ni       (rst_ni),
        .test_i       (1'b0),
        .slv_req_i    (axi_err_req),
        .slv_resp_o   (axi_err_resp)
    );

    //---------------------
    // Transaction Control
    //---------------------
    always_comb begin : transaction_ctl_comb

        // Default values
        trans_req_valid_o   = 1'b0;
        trans_req_data_o    = trans_req_data_q;
        trans_resp_ready_o  = 1'b0;

        state_n           = state_q;
        trans_req_data_n  = trans_req_data_q;
        trans_resp_data_n = trans_resp_data_q;

        aw_ready            = 1'b0;
        ar_ready            = 1'b0;
        complete_read       = 1'b0;
        complete_write      = 1'b0;
        mrif_req_valid_d    = 1'b0;

        unique case (state_q)

            // Monitor incoming requests
            // Forward AW before AR so read-to-write DMA can open the W drain
            // before source read data fills the write-data FIFO.
            IDLE: begin
                // Write request received
                // Stall writes if W FIFO is full
                if (aw_valid && !w_cnt_ovf) begin

                    // Tags
                    trans_req_data_n.iova       = aw_data.addr;
                    trans_req_data_n.did        = aw_data.stream_id;
                    trans_req_data_n.pid_valid  = aw_data.ss_id_valid;
                    trans_req_data_n.pid        = aw_data.substream_id;
                    trans_req_data_n.ttype      = aw_data.mmu_atst && !aw_data.mmu_valid ?
                                                  rv_iommu::TRANSLATED_W :
                                                  rv_iommu::UNTRANSLATED_W;
                    // AxPROT[0] indicates privileged transaction when set
                    trans_req_data_n.priv       = aw_data.prot[0];
                    trans_req_data_n.is_debug   = 1'b0;

                    state_n = REQ_TRANS;
                end

                // Read request received
                else if (ar_valid && (!ReadAdmissionEnable || read_admit_i ||
                         (fullbuffer_job_i && ar_data.stream_id == FullBufferDID))) begin
                    // Tags
                    trans_req_data_n.iova       = ar_data.addr;
                    trans_req_data_n.did        = ar_data.stream_id;
                    trans_req_data_n.pid_valid  = ar_data.ss_id_valid;
                    trans_req_data_n.pid        = ar_data.substream_id;
                    if(ar_data.mmu_atst && !ar_data.mmu_valid) begin
                       // ARPROT[2] indicates data access (r) when LOW, instruction access (rx) when HIGH
                       trans_req_data_n.ttype   = (ar_data.prot[2]) ?
                                                  (rv_iommu::TRANSLATED_RX) :
                                                  (rv_iommu::TRANSLATED_R);
                    end else begin
                       // ARPROT[2] indicates data access (r) when LOW, instruction access (rx) when HIGH
                       trans_req_data_n.ttype   = (ar_data.prot[2]) ?
                                                  (rv_iommu::UNTRANSLATED_RX) :
                                                  (rv_iommu::UNTRANSLATED_R);
                    end
                    // AxPROT[0] indicates privileged transaction when set
                    trans_req_data_n.priv       = ar_data.prot[0];
                    trans_req_data_n.is_debug   = 1'b0;

                    state_n = REQ_TRANS;
                end
            end

            // Perform BC check
            // Request translation
            REQ_TRANS: begin

                if (check_valid) begin

                    trans_req_valid_o = 1'b1;

                    if (trans_req_ready_i) begin
                        state_n     = AWAIT_RESP;
                    end
                end

                // Boundary violation
                else begin
                    state_n = COMPLETE;
                    trans_resp_data_n.error = 1'b1;
                end
            end

            // Await for a translation response
            AWAIT_RESP: begin

                trans_resp_ready_o = 1'b1;

                // Translation finished
                if (trans_resp_valid_i) begin
                    trans_resp_data_n   = trans_resp_data_i;
                    state_n             = (trans_resp_data_i.is_mrif &&
                                           !trans_resp_data_i.error) ? (MRIF) : (COMPLETE);
                end
            end

            // Trigger MRIF Handler
            MRIF: begin

                mrif_req_valid_d = 1'b1;
                if (mrif_req_ready_q) begin
                    state_n = COMPLETE;
                end
            end

            // Complete transaction
            // Select slave
            COMPLETE: begin

                complete_read = is_read;
                complete_write = is_write;

                if ((axi_comp_resp.ar_ready && is_read) ||
                    (axi_comp_resp.aw_ready && is_write)) begin
                    aw_ready    = is_write;
                    ar_ready    = is_read;
                    state_n     = IDLE;
                end
            end

            default: begin
                state_n = IDLE;
            end
        endcase
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin : transaction_ctl_seq
        if (!rst_ni) begin
            state_q             <= IDLE;
            trans_req_data_q    <= '0;
            trans_resp_data_q   <= '0;
        end

        else begin
            state_q             <= state_n;
            trans_req_data_q    <= trans_req_data_n;
            trans_resp_data_q   <= trans_resp_data_n;
        end
    end

endmodule
