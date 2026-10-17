// Copyright 2022 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Michael Rogenmoser <michaero@ethz.ch>
//
// Description: DMA frontend module that includes 64bit config and status reg handling

module idma_reg64_frontend #(
    /// register_interface request type
    parameter type          dma_regs_req_t   = logic,
    /// register_interface response type
    parameter type          dma_regs_rsp_t   = logic,
    /// dma burst request type
    parameter type          burst_req_t      = logic,
    parameter bit ObserverPort = 1'b0,
    parameter bit BatchPort = 1'b0,
    /// Device ID
    parameter logic [23:0]  DEVICE_ID        = 24'd1,
    /// Process ID
    parameter logic [19:0]  PROCESS_ID       = 20'd1,
    /// AXI ID width
    parameter int unsigned  AxiIdWidth       = 32'd0,
    /// AXI AxID
    parameter logic [(AxiIdWidth-1):0] AxID  = 0
) (
    input  logic          clk_i,
    input  logic          rst_ni,
    // Read-only event taps and optional observer register window.
    output logic obs_issue_o, obs_done_o, obs_poll_o,
    output logic batch_active_o,
    output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
    output logic obs_cfg_valid_o, obs_cfg_write_o,
    output logic [6:0] obs_cfg_addr_o,
    output logic [63:0] obs_cfg_wdata_o,
    output logic [7:0] obs_cfg_wstrb_o,
    input logic [63:0] obs_cfg_rdata_i,
    /// register interface control slave
    input  dma_regs_req_t dma_ctrl_req_i,
    output dma_regs_rsp_t dma_ctrl_rsp_o,
    /// DMA backend signals
    output burst_req_t    burst_req_o,
    output logic          valid_o,
    input  logic          ready_i,
    input  logic          backend_idle_i,
    input  logic          trans_complete_i,

    output logic [1:0]    ttype_o,
    output logic [63:0]   wdata_o,
    input  logic [63:0]   rdata_i,
    input  logic          rdata_valid_i
);

    localparam int unsigned DmaRegisterWidth = 64;
    localparam int unsigned DmaRegisterBlockAw = 7;
    localparam logic [DmaRegisterBlockAw-1:0] ActiveCyclesOffset = 7'h50;

    typedef enum logic [1:0] { 
        IDLE    = 2'b00,
        COUNT   = 2'b01,
        WAIT    = 2'b10
    } idma_state_t;
    idma_state_t state_q, state_n;

    logic [63:0]    transf_counter_q, transf_counter_n;
    logic [63:0]    transf_offset_q, transf_offset_n;

    /*
     * Signal and register definitions
     */
    idma_reg64_frontend_reg_pkg::idma_reg64_frontend_reg2hw_t dma_reg2hw;
    idma_reg64_frontend_reg_pkg::idma_reg64_frontend_hw2reg_t dma_hw2reg;

    // transaction id
    logic [DmaRegisterWidth-1:0] next_id, done_id;
    logic [DmaRegisterWidth-1:0] active_cycles_q, active_cycles_n;
    logic issue;
    logic trans_complete;
    logic active_cycles_read;

    dma_regs_rsp_t dma_ctrl_rsp_tmp;
    dma_regs_req_t base_req;
    logic observer_access;
    logic batch_access, locked_write;
    logic [7:0] full_addr;
    logic batch_enable_q, batch_stop_q, batch_stopped_q, batch_error_q;
    logic [32:0] batch_target_q, batch_issued_q, batch_completed_q;
    logic [12:0] batch_pages_q;
    logic [63:0] batch_id_q;
    logic batch_cfg_ok, batch_task_issue, batch_task_done;
    assign full_addr = 8'(dma_ctrl_req_i.addr);
    assign batch_access = BatchPort && dma_ctrl_req_i.valid && full_addr[7];
    assign batch_active_o = BatchPort && batch_enable_q && state_q != IDLE;
    assign locked_write = batch_active_o && dma_ctrl_req_i.valid &&
                          dma_ctrl_req_i.write && !batch_access && !observer_access;
    assign batch_cfg_ok = !batch_error_q && batch_target_q != 0 && batch_target_q <= 33'h100000000 &&
                          batch_pages_q != 0 && batch_pages_q <= 4096 &&
                          dma_reg2hw.num_bytes.q == 4096 &&
                          dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT &&
                          dma_reg2hw.src_addr.q[11:0] == 0 && dma_reg2hw.dst_addr.q[11:0] == 0;
    assign batch_task_issue = batch_active_o && state_q == COUNT && ready_i;
    assign batch_task_done = batch_active_o && state_q == WAIT && trans_complete_i;

    // The high 128-byte bank is explicitly decoded; the old register file remains 7-bit.
    // Configuration is immutable while active. STOP drains the currently offered task.
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            batch_enable_q <= 0; batch_stop_q <= 0; batch_stopped_q <= 0;
            batch_error_q <= 0; batch_target_q <= 0; batch_pages_q <= 0;
            batch_issued_q <= 0; batch_completed_q <= 0; batch_id_q <= 0;
        end else if (BatchPort) begin
            if (batch_enable_q && issue) begin
                batch_issued_q <= 0; batch_completed_q <= 0;
                batch_stop_q <= 0; batch_stopped_q <= 0; batch_id_q <= next_id;
            end
            if (batch_task_issue) batch_issued_q <= batch_issued_q + 1'b1;
            if (batch_task_done) begin
                batch_completed_q <= batch_completed_q + 1'b1;
                if (batch_stop_q) batch_stopped_q <= 1;
            end
            if (locked_write) batch_error_q <= 1;
            if (batch_access && dma_ctrl_req_i.write) begin
                if (dma_ctrl_req_i.wstrb != 8'hff) batch_error_q <= 1;
                else if (full_addr == 8'h88 && dma_ctrl_req_i.wdata == 2)
                    batch_stop_q <= 1;
                else if (state_q != IDLE) batch_error_q <= 1;
                else case (full_addr)
                    8'h88: if (dma_ctrl_req_i.wdata <= 1) begin
                        batch_enable_q <= dma_ctrl_req_i.wdata[0];
                        batch_stop_q <= 0; batch_stopped_q <= 0;
                        if (dma_ctrl_req_i.wdata == 0) begin
                            batch_error_q <= 0;
                            batch_issued_q <= 0;
                            batch_completed_q <= 0;
                        end
                    end else batch_error_q <= 1;
                    8'h90: if (dma_ctrl_req_i.wdata > 0 && dma_ctrl_req_i.wdata <= 64'h100000000)
                        batch_target_q <= dma_ctrl_req_i.wdata[32:0];
                        else batch_error_q <= 1;
                    8'h98: if (dma_ctrl_req_i.wdata > 0 && dma_ctrl_req_i.wdata <= 4096)
                        batch_pages_q <= dma_ctrl_req_i.wdata[12:0];
                        else batch_error_q <= 1;
                    default: batch_error_q <= 1;
                endcase
            end
        end
    end

    assign observer_access = ObserverPort && !batch_access && dma_ctrl_req_i.valid &&
        (dma_ctrl_req_i.addr[6:0] == 7'h58 || dma_ctrl_req_i.addr[6:0] == 7'h60 ||
         dma_ctrl_req_i.addr[6:0] == 7'h68 || dma_ctrl_req_i.addr[6:0] == 7'h70 ||
         dma_ctrl_req_i.addr[6:0] == 7'h78);
    always_comb begin
        base_req = dma_ctrl_req_i;
        base_req.valid = dma_ctrl_req_i.valid && !observer_access && !batch_access && !locked_write;
    end
    assign obs_cfg_valid_o = observer_access;
    assign obs_cfg_write_o = dma_ctrl_req_i.write;
    assign obs_cfg_addr_o = dma_ctrl_req_i.addr[6:0];
    assign obs_cfg_wdata_o = dma_ctrl_req_i.wdata;
    assign obs_cfg_wstrb_o = dma_ctrl_req_i.wstrb;
    assign obs_issue_o = batch_enable_q ? batch_task_issue : issue;
    assign obs_issue_id_o = batch_enable_q ? 64'(batch_issued_q) + 1 : next_id;
    assign obs_done_o = batch_enable_q ? batch_task_done : trans_complete;
    // Value the functional ID generator will publish at this retirement edge.
    assign obs_done_id_o = batch_enable_q ? 64'(batch_completed_q) + 1 :
                           ((&done_id) ? 64'd2 : done_id + 64'd1);
    assign obs_poll_o = dma_ctrl_req_i.valid && !dma_ctrl_req_i.write &&
                       !batch_access && dma_ctrl_req_i.addr[6:0] == 7'h30 && dma_ctrl_rsp_o.ready;
    assign obs_poll_id_o = batch_enable_q ?
        ((state_q == IDLE && done_id == batch_id_q) ? 64'(batch_completed_q) : 64'd0) : dma_ctrl_rsp_o.rdata;

    /*
     * DMA registers
     */
    idma_reg64_frontend_reg_top #(
        .reg_req_t( dma_regs_req_t ),
        .reg_rsp_t( dma_regs_rsp_t )
    ) i_dma_conf_regs (
        .clk_i,
        .rst_ni,
        .reg_req_i ( base_req         ),
        .reg_rsp_o ( dma_ctrl_rsp_tmp ),
        .reg2hw    ( dma_reg2hw       ),
        .hw2reg    ( dma_hw2reg       ),
        .devmode_i ( 1'b0             ) // if 1, explicit error return for unmapped register access
    );

    always_comb begin

        /*** TRANSFER PARAMETERS ***/
        burst_req_o                     = '0;

        burst_req_o.length              = dma_reg2hw.num_bytes.q;
        burst_req_o.src_addr            = dma_reg2hw.src_addr.q;
        burst_req_o.dst_addr            = dma_reg2hw.dst_addr.q;

        ttype_o                         = dma_reg2hw.conf.ttype.q;
        wdata_o                         = dma_reg2hw.wdata.q;

            // Current backend only supports one ID
        burst_req_o.opt.axi_id             = AxID;
            // Device ID
        burst_req_o.opt.device_id          = DEVICE_ID;
            // Process ID
        burst_req_o.opt.pv                 = dma_reg2hw.conf.pv.q;
        burst_req_o.opt.process_id         = (dma_reg2hw.conf.pv.q) ? 
                                             (PROCESS_ID) : 
                                             ('0);
            // DMA only supports incremental burst
        burst_req_o.opt.src.burst          = axi_pkg::BURST_INCR;
            // this frontend currently does not support cache variations
        burst_req_o.opt.src.cache          = dma_reg2hw.conf.cache.q;
            // AXI4 does not support locked transactions, use atomics
        burst_req_o.opt.src.lock           = '0;
            // unpriviledged, secure, data access
        burst_req_o.opt.src.prot           = '0;
            // not participating in qos
        burst_req_o.opt.src.qos            = '0;
            // only one region
        burst_req_o.opt.src.region         = '0;
            // DMA only supports incremental burst
        burst_req_o.opt.dst.burst          = axi_pkg::BURST_INCR;
            // this frontend currently does not support cache variations
        burst_req_o.opt.dst.cache          = dma_reg2hw.conf.cache.q;
            // AXI4 does not support locked transactions, use atomics
        burst_req_o.opt.dst.lock           = '0;
            // unpriviledged, secure, data access
        burst_req_o.opt.dst.prot           = '0;
            // not participating in qos
        burst_req_o.opt.dst.qos            = '0;
            // only one region in system
        burst_req_o.opt.dst.region         = '0;
            // ensure coupled AW to avoid deadlocks
        burst_req_o.opt.beo.decouple_aw    = '0;
        burst_req_o.opt.beo.decouple_rw    = dma_reg2hw.conf.decouple.q;
            // this frontend currently only supports completely debursting
        burst_req_o.opt.beo.src_max_llen   = '0;
            // this frontend currently only supports completely debursting
        burst_req_o.opt.beo.dst_max_llen   = '0;
        burst_req_o.opt.beo.src_reduce_len = dma_reg2hw.conf.deburst.q;
        burst_req_o.opt.beo.dst_reduce_len = dma_reg2hw.conf.deburst.q;

            // serialization no longer supported
        // burst_req_o.serialize   = dma_reg2hw.conf.serialize.q;

        /*** TRANSFER CONTROL ***/
        // reset state
        valid_o              = '0;
        dma_hw2reg.next_id.d = '0;
        dma_hw2reg.done.d    = '0;
        dma_hw2reg.status.d  = ~backend_idle_i;
        active_cycles_read   = dma_ctrl_req_i.valid &&
                               !dma_ctrl_req_i.write &&
                               (dma_ctrl_req_i.addr[DmaRegisterBlockAw-1:0] == ActiveCyclesOffset);

        dma_ctrl_rsp_o = dma_ctrl_rsp_tmp;
        if (active_cycles_read) begin
            dma_ctrl_rsp_o.rdata = active_cycles_q;
            dma_ctrl_rsp_o.error = 1'b0;
            dma_ctrl_rsp_o.ready = 1'b1;
        end

        if (observer_access) begin
            dma_ctrl_rsp_o.rdata = obs_cfg_rdata_i;
            dma_ctrl_rsp_o.error = 1'b0;
            dma_ctrl_rsp_o.ready = 1'b1;
        end

        if (locked_write) begin
            dma_ctrl_rsp_o.ready = 1;
            dma_ctrl_rsp_o.error = 1;
        end
        if (batch_access) begin
            dma_ctrl_rsp_o.ready = 1;
            dma_ctrl_rsp_o.error = 0;
            dma_ctrl_rsp_o.rdata = 0;
            case (full_addr)
                8'h80: dma_ctrl_rsp_o.rdata = 64'h49444d4142544301;
                8'h88: dma_ctrl_rsp_o.rdata = {31'd0, batch_error_q, 28'd0,
                    (batch_completed_q != 0 && batch_completed_q == batch_target_q && !batch_active_o),
                    batch_stopped_q, batch_active_o, batch_enable_q};
                8'h90: dma_ctrl_rsp_o.rdata = 64'(batch_target_q);
                8'h98: dma_ctrl_rsp_o.rdata = 64'(batch_pages_q);
                8'ha0: dma_ctrl_rsp_o.rdata = 64'(batch_completed_q);
                8'ha8: dma_ctrl_rsp_o.rdata = 64'(batch_issued_q);
                default: dma_ctrl_rsp_o.error = 1;
            endcase
        end

        // use full width id from generator
        dma_hw2reg.done.d = done_id;

        dma_hw2reg.rdata.de = rdata_valid_i;
        dma_hw2reg.rdata.d  = rdata_i;

        issue           = 1'b0;
        trans_complete  = 1'b0;

        // Counter data
        transf_counter_n  = transf_counter_q;
        transf_offset_n   = transf_offset_q;
        active_cycles_n   = active_cycles_q + DmaRegisterWidth'(!backend_idle_i);
        state_n           = state_q;

        unique case (state_q)

            IDLE: begin
            
                // start transaction upon next_id read (and having a valid config)
                if (dma_reg2hw.next_id.re) begin
                    if (batch_enable_q && !batch_cfg_ok) begin
                        dma_ctrl_rsp_o.ready = 1;
                        dma_ctrl_rsp_o.error = 1;
                    end else if (dma_reg2hw.num_bytes.q != '0) begin
                        
                        issue = 1'b1;
                        dma_hw2reg.next_id.d = next_id;
                        dma_ctrl_rsp_o.ready = ready_i;

                        transf_counter_n = batch_enable_q ? 64'(batch_target_q) : dma_reg2hw.n_trans.q;
                        transf_offset_n  = '0;
                        state_n          = COUNT;
                        if (batch_enable_q && !ready_i) begin
                            issue = 0;
                            state_n = IDLE;
                        end
                    end
                end
            end

            COUNT: begin
            
                // Send request to FIFO
                valid_o              = 1'b1;

                burst_req_o.src_addr = dma_reg2hw.src_addr.q + transf_offset_q;
                burst_req_o.dst_addr = dma_reg2hw.dst_addr.q + transf_offset_q;

                if (ready_i) begin

                    state_n          = WAIT;
                    transf_counter_n = transf_counter_q - 1;
                    transf_offset_n  = transf_offset_q + 13'h1000;
                    if (batch_enable_q && transf_offset_n == (64'(batch_pages_q) << 12))
                        transf_offset_n = 0;
                end
            end

            WAIT: begin
                
                if (trans_complete_i) begin
                    if ((transf_counter_q == '0) || 
                            (!batch_enable_q && dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT) ||
                            (batch_enable_q && batch_stop_q)) begin
                        trans_complete = 1'b1;
                        state_n = IDLE;
                    end

                    else begin
                        state_n = COUNT;
                    end
                end
            end

            default: begin
                state_n = IDLE;
            end
        endcase
    end

    // transfer id generator
    idma_transfer_id_gen #(
        .IdWidth      ( DmaRegisterWidth  )
    ) i_idma_transfer_id_gen (
        .clk_i,
        .rst_ni,
        .issue_i      ( issue             ),
        .retire_i     ( trans_complete    ),
        .next_o       ( next_id           ),
        .completed_o  ( done_id           )
    );

    `FF(state_q,            state_n,            IDLE,   clk_i, rst_ni)
    `FF(transf_counter_q,   transf_counter_n,   '0,     clk_i, rst_ni)
    `FF(transf_offset_q,    transf_offset_n,    '0,     clk_i, rst_ni)
    `FF(active_cycles_q,    active_cycles_n,    '0,     clk_i, rst_ni)

endmodule : idma_reg64_frontend
