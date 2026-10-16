I reviewed the supplied sources and the prior draft against all eight review points. Below are the complete final contents of the six allowed files (no diffs). I have not executed anything; every behavioral claim is derived from the supplied RTL and the generated `reg_pkg`, and must be re-verified by Codex.

## Brief changes made

1. **Launcher forwarding reachable** — `obs_cfg_valid_o = observer_access || launcher_access`; the launcher window write/read is now actually observed by the external block. A dedicated launcher-window test samples `cfg_valid_o/cfg_write_o/cfg_addr_o/cfg_wdata_o` while `valid_i` is asserted.
2. **Correct register map** — `CONF` is written at `0x18` (was 0x20 = STATUS) and `configure_controlled` writes `0` to CONF. Invalid-config test writes a real writable field bit (`cache[0]`, PERMIT byte0 writable) at `0x18`, not a reserved/ignored bit. `N_TRANS (0x48)` is deliberately not used (DEFAULT 64B path ignores it).
3. **`ready_i` actually driven** — removed the `d.ready_i = !stall` override in `eval()`; tests now drive `ready_i` directly, so `ready_i = 0` is genuinely exercised (reservation still returns ready, COUNT stalls).
4. **Pulse sampling fixed** — `access2` now copies `rdata`, `error`, `ready`, `launch_error`, `launch_cancelled`, `issue`, `done`, IDs and the cfg forward bus into `Rsp` *while `valid_i` is high*, before the accept edge and before deassert.
5. **DONE override removed** — dropped `dma_hw2reg.done.d = launch_id_q`; `dma_hw2reg.done.d` keeps the raw generator `done_id`. `obs_done_id_o` reports the reserved ID (matches "done_id matching saved").
6. **`launch_cfg_ok` per-field** — compares each `reg_pkg` CONF `q` field (ttype==DEFAULT, pv/cache/decouple/deburst/serialize==0) plus 64B/aligned/!batch/idle. `launch_active_o` is documented as an all-`HOLD/COUNT/WAIT` preflight, distinct from `launch_ctrl_q`.
7. **Test coverage added** — DONE after cancel retirement, stale go/cancel in IDLE harmless, batch writes rejected while HOLD without poisoning `batch_error_q`, repeated NEXT_ID error, invalid config consumes no ID. Historical gold instance untouched (only DUT tie-offs).
8. **Architecture unchanged** — HOLD precedes VALID, `go`→COUNT with persistent VALID until backend `ready_i`, reserved ID, cancel only in HOLD (pulse), exact 8-bit launcher decode `>=0xb0`.

---

## cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv

```systemverilog
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
    parameter bit LaunchPort = 1'b0,
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
    // Optional controlled-launch handshake (active only when LaunchPort).
    input  logic launch_hold_i, launch_go_i, launch_cancel_i,
    output logic launch_held_o, launch_active_o, launch_cancelled_o, launch_error_o,
    output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
    output logic obs_cfg_valid_o, obs_cfg_write_o,
    output logic [7:0] obs_cfg_addr_o,
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
    localparam logic [7:0] ActiveCyclesOffset = 8'h50;
    localparam logic [7:0] NextIdOffset       = 8'h28;
    // Launcher register forwarding lives in the upper bank, above the batch window.
    localparam logic [7:0] LauncherBase       = 8'hb0;

    typedef enum logic [1:0] {
        IDLE    = 2'b00,
        COUNT   = 2'b01,
        WAIT    = 2'b10,
        HOLD    = 2'b11
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
    logic id_retire;
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

    // Controlled-launch state
    logic launcher_access;
    logic launch_busy;
    logic launch_cfg_ok;
    logic launch_next_id_read;
    logic launch_base_write;
    logic launch_prepare_err;
    logic launch_locked_write;
    logic launch_ctrl_q, launch_ctrl_n;
    logic [DmaRegisterWidth-1:0] launch_id_q, launch_id_n;

    assign full_addr = 8'(dma_ctrl_req_i.addr);
    // Launcher window is carved out before the batch high-bank decode so 0xb0..0xff
    // never aliases the batch bank.
    assign launcher_access = LaunchPort && ObserverPort && dma_ctrl_req_i.valid && (full_addr >= LauncherBase);
    assign batch_access = BatchPort && dma_ctrl_req_i.valid && full_addr[7] && !launcher_access;
    assign batch_active_o = BatchPort && batch_enable_q && state_q != IDLE;
    assign locked_write = batch_active_o && dma_ctrl_req_i.valid &&
                          dma_ctrl_req_i.write && !batch_access && !observer_access && !launcher_access;
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
            // While a controlled launch job is active the batch bank is read-only:
            // the write is rejected (AXI error) but must not poison batch_error_q.
            if (batch_access && dma_ctrl_req_i.write && !launch_busy) begin
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

    // Legacy observer window decoded on the exact 8-bit address (no high alias).
    assign observer_access = ObserverPort && !batch_access && !launcher_access && dma_ctrl_req_i.valid &&
        (full_addr == 8'h58 || full_addr == 8'h60 ||
         full_addr == 8'h68 || full_addr == 8'h70 || full_addr == 8'h78);

    always_comb begin
        base_req = dma_ctrl_req_i;
        base_req.valid = dma_ctrl_req_i.valid && !observer_access && !launcher_access && !batch_access &&
                         !locked_write && !launch_locked_write && !(launch_busy && launch_next_id_read);
    end

    // Launcher writes never reach the register file, so the window is forwarded to
    // the external register block through the observer config bus. Without the
    // launcher_access term the external block would never see launcher traffic.
    assign obs_cfg_valid_o = observer_access || launcher_access;
    assign obs_cfg_write_o = dma_ctrl_req_i.write;
    assign obs_cfg_addr_o  = full_addr;
    assign obs_cfg_wdata_o = dma_ctrl_req_i.wdata;
    assign obs_cfg_wstrb_o = dma_ctrl_req_i.wstrb;

    // Controlled-launch helpers
    assign launch_busy = LaunchPort && launch_ctrl_q;
    assign launch_next_id_read = LaunchPort && dma_ctrl_req_i.valid && !dma_ctrl_req_i.write &&
                                 (full_addr == NextIdOffset);
    assign launch_base_write = LaunchPort && dma_ctrl_req_i.valid && dma_ctrl_req_i.write &&
                               !batch_access && !observer_access && !launcher_access;
    assign launch_locked_write = launch_busy && launch_base_write;

    // Exactly 64-byte, DEFAULT, aligned, batch-disabled, idle-backend transfer with
    // an all-zero configuration. CONF is compared per reg_pkg field to avoid packed
    // aggregate comparison ambiguities.
    assign launch_cfg_ok = (dma_reg2hw.conf.ttype.q     == idma_pkg::DEFAULT) &&
                           (dma_reg2hw.conf.pv.q         == 1'b0) &&
                           (dma_reg2hw.conf.cache.q      == 4'b0) &&
                           (dma_reg2hw.conf.decouple.q   == 1'b0) &&
                           (dma_reg2hw.conf.deburst.q    == 1'b0) &&
                           (dma_reg2hw.conf.serialize.q  == 1'b0) &&
                           (dma_reg2hw.num_bytes.q       == 64'd64) &&
                           (dma_reg2hw.src_addr.q[5:0]   == '0) &&
                           (dma_reg2hw.dst_addr.q[5:0]   == '0) &&
                           !batch_enable_q &&
                           backend_idle_i;

    assign launch_prepare_err = LaunchPort && state_q == IDLE && launch_hold_i && launch_next_id_read &&
                                (launch_cancel_i || !launch_cfg_ok);

    assign launch_error_o = LaunchPort && (launch_prepare_err ||
        (launch_busy && launch_next_id_read) ||
        launch_locked_write ||
        (launch_busy && batch_access && dma_ctrl_req_i.write));

    assign launch_cancelled_o = LaunchPort && state_q == HOLD && launch_cancel_i;
    assign launch_held_o      = LaunchPort && state_q == HOLD;
    // launch_active_o tracks ANY active transfer (HOLD/COUNT/WAIT), including an
    // ordinary job, so it is a preflight "frontend busy" indicator. It is
    // intentionally wider than launch_ctrl_q (the controlled job only); use
    // launch_held_o / launch_busy to distinguish the controlled job.
    assign launch_active_o    = LaunchPort && (state_q == HOLD || state_q == COUNT || state_q == WAIT);

    assign obs_issue_o = batch_enable_q ? batch_task_issue :
                         (LaunchPort && (launch_ctrl_q || launch_hold_i)) ?
                            (launch_ctrl_q && state_q == COUNT && ready_i) : issue;
    assign obs_issue_id_o = batch_enable_q ? 64'(batch_issued_q) + 1 :
                            (LaunchPort && (launch_ctrl_q || launch_hold_i)) ?
                               (launch_ctrl_q ? launch_id_q : next_id) : next_id;
    assign obs_done_o = batch_enable_q ? batch_task_done : trans_complete;
    // For a controlled job report the reserved ID; otherwise keep the raw generator
    // behaviour (previous observer semantics).
    assign obs_done_id_o = batch_enable_q ? 64'(batch_completed_q) + 1 :
                           ((LaunchPort && launch_ctrl_q && trans_complete) ? launch_id_q :
                            ((&done_id) ? 64'd2 : done_id + 64'd1));
    assign obs_poll_o = dma_ctrl_req_i.valid && !dma_ctrl_req_i.write &&
                       !batch_access && !observer_access && !launcher_access &&
                       full_addr == 8'h30 && dma_ctrl_rsp_o.ready;
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
                               (full_addr == ActiveCyclesOffset);

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

        if (launcher_access) begin
            dma_ctrl_rsp_o.rdata = obs_cfg_rdata_i;
            dma_ctrl_rsp_o.error = 1'b0;
            dma_ctrl_rsp_o.ready = 1'b1;
        end

        if (locked_write) begin
            dma_ctrl_rsp_o.ready = 1;
            dma_ctrl_rsp_o.error = 1;
        end

        if (launch_locked_write) begin
            dma_ctrl_rsp_o.ready = 1;
            dma_ctrl_rsp_o.error = 1;
            dma_ctrl_rsp_o.rdata = '0;
        end

        if (launch_busy && launch_next_id_read) begin
            dma_ctrl_rsp_o.ready = 1;
            dma_ctrl_rsp_o.error = 1;
            dma_ctrl_rsp_o.rdata = '0;
        end

        if (batch_access) begin
            dma_ctrl_rsp_o.ready = 1;
            dma_ctrl_rsp_o.error = 0;
            dma_ctrl_rsp_o.rdata = 0;
            if (LaunchPort && launch_busy && dma_ctrl_req_i.write) begin
                // Controlled job active: batch bank is read-only.
                dma_ctrl_rsp_o.error = 1;
            end else begin
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
        end

        // DONE keeps the raw ID generator value (no launch-specific override).
        dma_hw2reg.done.d = done_id;

        dma_hw2reg.rdata.de = rdata_valid_i;
        dma_hw2reg.rdata.d  = rdata_i;

        issue           = 1'b0;
        trans_complete  = 1'b0;
        id_retire       = 1'b0;

        launch_ctrl_n   = launch_ctrl_q;
        launch_id_n     = launch_id_q;

        // Counter data
        transf_counter_n  = transf_counter_q;
        transf_offset_n   = transf_offset_q;
        active_cycles_n   = active_cycles_q + DmaRegisterWidth'(!backend_idle_i);
        state_n           = state_q;

        unique case (state_q)

            IDLE: begin

                if (LaunchPort && launch_hold_i && launch_next_id_read) begin
                    // Controlled preparation: reserve an ID and enter HOLD, or
                    // reject (invalid config / simultaneous cancel) without
                    // allocating an ID.
                    if (launch_cancel_i || !launch_cfg_ok) begin
                        dma_ctrl_rsp_o.ready = 1;
                        dma_ctrl_rsp_o.error = 1;
                    end else begin
                        issue               = 1'b1;
                        dma_hw2reg.next_id.d = next_id;
                        dma_ctrl_rsp_o.ready = 1;
                        dma_ctrl_rsp_o.error = 0;
                        launch_id_n         = next_id;
                        launch_ctrl_n       = 1'b1;
                        transf_counter_n    = dma_reg2hw.n_trans.q;
                        transf_offset_n     = '0;
                        state_n             = HOLD;
                    end
                end else if (dma_reg2hw.next_id.re) begin
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

                // Send request to FIFO. VALID stays asserted (and addresses stable)
                // regardless of later cancel/hold/go until the backend is ready.
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
                    if (launch_ctrl_q) begin
                        // Controlled job completion.
                        trans_complete = 1'b1;
                        id_retire      = 1'b1;
                        launch_ctrl_n  = 1'b0;
                        launch_id_n    = '0;
                        state_n        = IDLE;
                    end else if ((transf_counter_q == '0) ||
                            (!batch_enable_q && dma_reg2hw.conf.ttype.q == idma_pkg::DEFAULT) ||
                            (batch_enable_q && batch_stop_q)) begin
                        trans_complete = 1'b1;
                        id_retire      = 1'b1;
                        state_n = IDLE;
                    end

                    else begin
                        state_n = COUNT;
                    end
                end
            end

            HOLD: begin
                // No backend VALID here. Cancel retires the reserved ID without an
                // obs_issue/obs_done event; go starts the transfer.
                if (launch_cancel_i) begin
                    id_retire     = 1'b1;
                    launch_ctrl_n = 1'b0;
                    launch_id_n   = '0;
                    state_n       = IDLE;
                end else if (launch_go_i) begin
                    state_n       = COUNT;
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
        .retire_i     ( id_retire         ),
        .next_o       ( next_id           ),
        .completed_o  ( done_id           )
    );

    `FF(state_q,            state_n,            IDLE,   clk_i, rst_ni)
    `FF(transf_counter_q,   transf_counter_n,   '0,     clk_i, rst_ni)
    `FF(transf_offset_q,    transf_offset_n,    '0,     clk_i, rst_ni)
    `FF(active_cycles_q,    active_cycles_n,    '0,     clk_i, rst_ni)
    `FF(launch_ctrl_q,      launch_ctrl_n,      1'b0,   clk_i, rst_ni)
    `FF(launch_id_q,        launch_id_n,        '0,     clk_i, rst_ni)

endmodule : idma_reg64_frontend
```

---

## cva6/corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv

```systemverilog
// Copyright 2022 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Thomas Benz    <tbenz@iis.ee.ethz.ch>
// Author: Andreas Kuster <kustera@ethz.ch>
// Author: Paul Scheffler <paulsc@iis.ee.ethz.ch>
//
// Description: DMA core wrapper for the CVA6 integration

`include "axi/assign.svh"
`include "axi/typedef.svh"
`include "axi-iommu/assign.svh"
`include "axi-iommu/typedef.svh"
`include "idma/typedef.svh"
`include "register_interface/typedef.svh"

module dma_core_wrap #(
  parameter int unsigned              AxiAddrWidth     = 32'd0,
  parameter int unsigned              AxiDataWidth     = 32'd0,
  parameter int unsigned              AxiIdWidth       = 32'd0,
  parameter int unsigned              AxiUserWidth     = 32'd0,
  parameter int unsigned              AxiSlvIdWidth    = 32'd0,
  parameter int unsigned              NumAxInFlight    = 32'd0,
  parameter int unsigned              MemSysDepth      = 32'd0,
  parameter int unsigned              JobFifoDepth     = 32'd0,
  parameter int unsigned              BufferDepth      = 32'h3, // Default = 3
  parameter bit                       RAWCouplingAvail = 32'd0,
  parameter bit                       ObserverPort    = 1'b0,
  parameter bit BatchPort = 1'b0,
  parameter bit LaunchPort = 1'b0,
  parameter bit                       IsTwoD           = 32'd0,
  parameter type                      axi_mst_req_t    = logic,
  parameter type                      axi_mst_rsp_t    = logic,
  parameter type                      axi_slv_req_t    = logic,
  parameter type                      axi_slv_rsp_t    = logic,
  parameter logic [23:0]              DEVICE_ID        = 24'd1,
  parameter logic [19:0]              PROCESS_ID       = 20'd0,
  parameter logic [(AxiIdWidth-1):0]  AxID             = 0
) (
  input  logic          clk_i,
  input  logic          rst_ni,
  input  logic          testmode_i,
    // Read-only event taps and optional observer register window.
    output logic obs_issue_o, obs_done_o, obs_poll_o,
  output logic batch_active_o,
  input  logic launch_hold_i, launch_go_i, launch_cancel_i,
  output logic launch_held_o, launch_active_o, launch_cancelled_o, launch_error_o,
    output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
    output logic obs_cfg_valid_o, obs_cfg_write_o,
    output logic [7:0] obs_cfg_addr_o,
    output logic [63:0] obs_cfg_wdata_o,
    output logic [7:0] obs_cfg_wstrb_o,
    input logic [63:0] obs_cfg_rdata_i,
  output axi_mst_req_t  axi_mst_req_o,
  input  axi_mst_rsp_t  axi_mst_rsp_i,
  input  axi_slv_req_t  axi_slv_req_i,
  output axi_slv_rsp_t  axi_slv_rsp_o
);

  // local params
  localparam int unsigned DmaRegisterWidth = 32'd64;
  localparam int unsigned NumDim           = 32'd2;
  localparam int unsigned TFLenWidth       = AxiAddrWidth;

  typedef logic [AxiAddrWidth-1:0]     addr_t;
  typedef logic [AxiDataWidth-1:0]     data_t;
  typedef logic [(AxiDataWidth/8)-1:0] strb_t;
  typedef logic [AxiUserWidth-1:0]     axi_user_t;
  typedef logic [AxiIdWidth-1:0]       axi_id_t;
  typedef logic [AxiSlvIdWidth-1:0]    axi_slv_id_t;

  typedef logic [24-1:0]  iommu_sid_t;
  typedef logic           iommu_ssidv_t;
  typedef logic [20-1:0]  iommu_ssid_t;

  // iDMA struct definitions
  typedef logic [TFLenWidth-1:0]  tf_len_t;

  // iDMA request / response types
  `IDMA_TYPEDEF_FULL_REQ_T(idma_req_t, axi_slv_id_t, addr_t, tf_len_t)
  `IDMA_TYPEDEF_FULL_RSP_T(idma_rsp_t, addr_t)

  `REG_BUS_TYPEDEF_ALL(dma_regs, addr_t, data_t, strb_t)

  idma_req_t burst_req, burst_req_d;
  logic      be_valid, be_valid_d;
  logic      be_ready, be_ready_d;
  logic      be_trans_complete;
  idma_pkg::idma_busy_t idma_busy;

  idma_rsp_t idma_rsp;
  logic idma_rsp_valid;
  logic idma_rsp_ready;

  logic twod_trans_complete;
  logic twod_busy;

  dma_regs_req_t dma_regs_req;
  dma_regs_rsp_t dma_regs_rsp;

  logic [1:0] ttype;
  data_t wdata, rdata;
  logic rdata_valid;

  axi_to_reg #(
    .ADDR_WIDTH( AxiAddrWidth     ),
    .DATA_WIDTH( AxiDataWidth     ),
    .ID_WIDTH  ( AxiSlvIdWidth    ),
    .USER_WIDTH( AxiUserWidth     ),
    .axi_req_t ( axi_slv_req_t    ),
    .axi_rsp_t ( axi_slv_rsp_t    ),
    .reg_req_t ( dma_regs_req_t   ),
    .reg_rsp_t ( dma_regs_rsp_t   )
  ) i_axi_translate (
    .clk_i,
    .rst_ni,
    .testmode_i ( 1'b0          ),
    .axi_req_i  ( axi_slv_req_i ),
    .axi_rsp_o  ( axi_slv_rsp_o ),
    .reg_req_o  ( dma_regs_req  ),
    .reg_rsp_i  ( dma_regs_rsp  )
  );


  if (!IsTwoD) begin : gen_one_d
    /*
     * DMA Frontend
     */
    idma_reg64_frontend #(
      .ObserverPort ( ObserverPort ),
      .BatchPort ( BatchPort ),
      .LaunchPort ( LaunchPort ),
      .dma_regs_req_t  ( dma_regs_req_t ),
      .dma_regs_rsp_t  ( dma_regs_rsp_t ),
      .burst_req_t     ( idma_req_t     ),
      .DEVICE_ID       ( DEVICE_ID      ),
      .PROCESS_ID      ( PROCESS_ID     ),
      .AxiIdWidth      ( AxiIdWidth     ),
      .AxID            ( AxID           )
    ) i_dma_frontend (
      .obs_issue_o, .obs_done_o, .obs_poll_o, .batch_active_o,
      .launch_hold_i, .launch_go_i, .launch_cancel_i,
      .launch_held_o, .launch_active_o, .launch_cancelled_o, .launch_error_o,
      .obs_issue_id_o, .obs_done_id_o, .obs_poll_id_o,
      .obs_cfg_valid_o, .obs_cfg_write_o, .obs_cfg_addr_o,
      .obs_cfg_wdata_o, .obs_cfg_wstrb_o, .obs_cfg_rdata_i,
      .clk_i,
      .rst_ni,
      // AXI slave: control port
      .dma_ctrl_req_i   ( dma_regs_req      ),
      .dma_ctrl_rsp_o   ( dma_regs_rsp      ),
      // Backend control
      .burst_req_o      ( burst_req_d       ),
      .valid_o          ( be_valid_d        ),
      .ready_i          ( be_ready_d        ),
      .backend_idle_i   ( ~|idma_busy       ),
      .trans_complete_i ( be_trans_complete ),

      .ttype_o          ( ttype             ),
      .wdata_o          ( wdata             ),
      .rdata_i          ( rdata             ),
      .rdata_valid_i    ( rdata_valid       )
    );

    stream_fifo #(
        .FALL_THROUGH ( 1'b0         ),
        .DATA_WIDTH   ( AxiDataWidth ),
        .DEPTH        ( JobFifoDepth ),
        .T            ( idma_req_t   )
      ) i_stream_fifo_jobs_oned (
        .clk_i,
        .rst_ni,
        .testmode_i,
        .flush_i    ( 1'b0                ),
        .usage_o    ( /* NOT CONNECTED */ ),
        .data_i     ( burst_req_d         ),
        .valid_i    ( be_valid_d          ),
        .ready_o    ( be_ready_d          ),
        .data_o     ( burst_req           ),
        .valid_o    ( be_valid            ),
        .ready_i    ( be_ready            )
    );


    assign be_trans_complete = idma_rsp_valid;
    assign idma_rsp_ready    = 1'b1;

  end else begin : gen_two_d
    assign batch_active_o = 1'b0;
    assign obs_issue_o = 1'b0;
    assign obs_done_o = 1'b0;
    assign obs_poll_o = 1'b0;
    assign obs_issue_id_o = '0;
    assign obs_done_id_o = '0;
    assign obs_poll_id_o = '0;
    assign obs_cfg_valid_o = 1'b0;
    assign obs_cfg_write_o = 1'b0;
    assign obs_cfg_addr_o = '0;
    assign obs_cfg_wdata_o = '0;
    assign obs_cfg_wstrb_o = '0;
    assign launch_held_o = 1'b0;
    assign launch_active_o = 1'b0;
    assign launch_cancelled_o = 1'b0;
    assign launch_error_o = 1'b0;

    `IDMA_TYPEDEF_FULL_ND_REQ_T(idma_nd_req_t, idma_req_t, tf_len_t, tf_len_t)

    idma_nd_req_t idma_nd_req,       idma_nd_req_d;
    logic         idma_nd_req_valid, idma_nd_req_valid_d;
    logic         idma_nd_req_ready, idma_nd_req_ready_d;

    // 2D frontend
    idma_reg64_2d_frontend #(
      .dma_regs_req_t  ( dma_regs_req_t ),
      .dma_regs_rsp_t  ( dma_regs_rsp_t ),
      .burst_req_t     ( idma_req_t     ),
      .idma_nd_req_t   ( idma_nd_req_t  )

    ) i_dma_2d_frontend (
      .clk_i,
      .rst_ni,
      // AXI slave: control port
      .dma_ctrl_req_i   ( dma_regs_req             ),
      .dma_ctrl_rsp_o   ( dma_regs_rsp             ),
      // Backend control
      .idma_nd_req_o    ( idma_nd_req_d            ),
      .valid_o          ( idma_nd_req_valid_d      ),
      .ready_i          ( idma_nd_req_ready_d      ),
      .backend_idle_i   ( ~|idma_busy & !twod_busy ),
      .trans_complete_i ( twod_trans_complete      )
    );

    stream_fifo #(
        .FALL_THROUGH ( 1'b0          ),
        .DATA_WIDTH   ( AxiDataWidth  ),
        .DEPTH        ( JobFifoDepth  ),
        .T            ( idma_nd_req_t )
      ) i_stream_fifo_jobs_twod (
        .clk_i,
        .rst_ni,
        .testmode_i,
        .flush_i    ( 1'b0                ),
        .usage_o    ( /* NOT CONNECTED */ ),
        .data_i     ( idma_nd_req_d       ),
        .valid_i    ( idma_nd_req_valid_d ),
        .ready_o    ( idma_nd_req_ready_d ),
        .data_o     ( idma_nd_req         ),
        .valid_o    ( idma_nd_req_valid   ),
        .ready_i    ( idma_nd_req_ready   )
    );

    // Midend
    idma_nd_midend #(
      .NumDim        ( NumDim          ),
      .addr_t        ( addr_t          ),
      .idma_req_t    ( idma_req_t      ),
      .idma_rsp_t    ( idma_rsp_t      ),
      .idma_nd_req_t ( idma_nd_req_t   ),
      .RepWidths     ( {AxiAddrWidth, AxiAddrWidth} )
    ) i_idma_nd_midend (
      .clk_i,
      .rst_ni,
      .nd_req_i         ( idma_nd_req         ),
      .nd_req_valid_i   ( idma_nd_req_valid   ),
      .nd_req_ready_o   ( idma_nd_req_ready   ),
      .nd_rsp_o         ( /* NOT CONECTED */  ),
      .nd_rsp_valid_o   ( twod_trans_complete ),
      .nd_rsp_ready_i   ( 1'b1                ),
      .burst_req_o      ( burst_req           ),
      .burst_req_valid_o( be_valid            ),
      .burst_req_ready_i( be_ready            ),
      .burst_rsp_i      ( idma_rsp            ),
      .burst_rsp_valid_i( idma_rsp_valid      ),
      .burst_rsp_ready_o( idma_rsp_ready      ),
      .busy_o           ( twod_busy           )
    );
  end

  `AXI_TYPEDEF_AW_CHAN_EXT_T(axi_mst_aw_chan_t, addr_t, axi_id_t, axi_user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)
  `AXI_TYPEDEF_AR_CHAN_EXT_T(axi_mst_ar_chan_t, addr_t, axi_id_t, axi_user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)

  idma_backend #(
    .DataWidth           ( AxiDataWidth                ),
    .AddrWidth           ( AxiAddrWidth                ),
    .UserWidth           ( AxiUserWidth                ),
    .AxiIdWidth          ( AxiIdWidth                  ),
    .NumAxInFlight       ( NumAxInFlight               ),
    .BufferDepth         ( BufferDepth                 ),
    .TFLenWidth          ( TFLenWidth                  ),
    .RAWCouplingAvail    ( RAWCouplingAvail            ),
    .MaskInvalidData     ( 1'b0                        ),
    .HardwareLegalizer   ( 1'b1                        ),
    .RejectZeroTransfers ( 1'b1                        ),
    .MemSysDepth         ( MemSysDepth                 ),
    .ErrorCap            ( idma_pkg::NO_ERROR_HANDLING ),
    .idma_req_t          ( idma_req_t                  ),
    .idma_rsp_t          ( idma_rsp_t                  ),
    .idma_eh_req_t       ( idma_pkg::idma_eh_req_t     ),
    .idma_busy_t         ( idma_pkg::idma_busy_t       ),
    .protocol_req_t      ( axi_mst_req_t               ),
    .protocol_rsp_t      ( axi_mst_rsp_t               ),
    .aw_chan_t           ( axi_mst_aw_chan_t           ),
    .ar_chan_t           ( axi_mst_ar_chan_t           )
  ) i_idma_backend (
    .clk_i,
    .rst_ni,
    .testmode_i,

    .idma_req_i    ( burst_req         ),
    .req_valid_i   ( be_valid          ),
    .req_ready_o   ( be_ready          ),

    .idma_rsp_o    ( idma_rsp          ),
    .rsp_valid_o   ( idma_rsp_valid    ),
    .rsp_ready_i   ( idma_rsp_ready    ),

    .idma_eh_req_i ( '0                ), // No error handling
    .eh_req_valid_i( 1'b1              ),
    .eh_req_ready_o( /*NOT CONNECTED*/ ),

    .protocol_req_o( axi_mst_req_o     ),
    .protocol_rsp_i( axi_mst_rsp_i     ),
    .busy_o        ( idma_busy         ),

    .ttype_i       ( ttype             ),
    .wdata_i       ( wdata             ),
    .rdata_o       ( rdata             ),
    .rdata_valid_o ( rdata_valid       )
  );

endmodule : dma_core_wrap



module dma_core_wrap_intf #(
  parameter int unsigned               AXI_ADDR_WIDTH     = 32'd0,
  parameter int unsigned               AXI_DATA_WIDTH     = 32'd0,
  parameter int unsigned               AXI_USER_WIDTH     = 32'd0,
  parameter int unsigned               AXI_ID_WIDTH       = 32'd0,
  parameter int unsigned               AXI_SLV_ID_WIDTH   = 32'd0,
  parameter int unsigned               JOB_FIFO_DEPTH     = 32'd0,
  parameter int unsigned               NUM_AX_IN_FLIGHT   = 32'd0,
  parameter int unsigned               MEM_SYS_DEPTH      = 32'd0,
  parameter int unsigned               BUFFER_DEPTH       = 32'h3, // Default = 3
  parameter bit                        RAW_COUPLING_AVAIL =  1'b0,
  parameter bit                        OBSERVER_PORT    = 1'b0,
  parameter bit BATCH_PORT = 1'b0,
  parameter bit LAUNCH_PORT = 1'b0,
  parameter bit                        IS_TWO_D           =  1'b0,

  parameter logic [23:0]               DEVICE_ID          = 24'd1,
  parameter logic [19:0]               PROCESS_ID         = 20'd0,
  parameter logic [(AXI_ID_WIDTH-1):0] AxID               =     0
) (
  input  logic         clk_i,
  input  logic         rst_ni,
  input  logic         testmode_i,
    // Read-only event taps and optional observer register window.
    output logic obs_issue_o, obs_done_o, obs_poll_o,
  output logic batch_active_o,
  input  logic launch_hold_i, launch_go_i, launch_cancel_i,
  output logic launch_held_o, launch_active_o, launch_cancelled_o, launch_error_o,
    output logic [63:0] obs_issue_id_o, obs_done_id_o, obs_poll_id_o,
    output logic obs_cfg_valid_o, obs_cfg_write_o,
    output logic [7:0] obs_cfg_addr_o,
    output logic [63:0] obs_cfg_wdata_o,
    output logic [7:0] obs_cfg_wstrb_o,
    input logic [63:0] obs_cfg_rdata_i,
  AXI_BUS_EXT.Master   axi_master,
  AXI_BUS.Slave        axi_slave
);

  typedef logic [AXI_ADDR_WIDTH-1:0]     addr_t;
  typedef logic [AXI_DATA_WIDTH-1:0]     data_t;
  typedef logic [(AXI_DATA_WIDTH/8)-1:0] strb_t;
  typedef logic [AXI_USER_WIDTH-1:0]     user_t;
  typedef logic [AXI_ID_WIDTH-1:0]       axi_id_t;
  typedef logic [AXI_SLV_ID_WIDTH-1:0]   axi_slv_id_t;

  /*** Define extended request structs ***/
  typedef logic [24-1:0]  iommu_sid_t;
  typedef logic           iommu_ssidv_t;
  typedef logic [20-1:0]  iommu_ssid_t;

  `AXI_TYPEDEF_EXT_ALL(axi_mst, addr_t, axi_id_t, data_t, strb_t, user_t, iommu_sid_t, iommu_ssidv_t, iommu_ssid_t)
  axi_mst_req_t axi_mst_req;
  axi_mst_resp_t axi_mst_resp;
  `AXI_ASSIGN_FROM_REQ_EXT(axi_master, axi_mst_req)
  `AXI_ASSIGN_TO_RESP(axi_mst_resp, axi_master)

  `AXI_TYPEDEF_ALL(axi_slv, addr_t, axi_slv_id_t, data_t, strb_t, user_t)
  axi_slv_req_t axi_slv_req;
  axi_slv_resp_t axi_slv_resp;
  `AXI_ASSIGN_TO_REQ(axi_slv_req, axi_slave)
  `AXI_ASSIGN_FROM_RESP(axi_slave, axi_slv_resp)

  dma_core_wrap #(
    .AxiAddrWidth     ( AXI_ADDR_WIDTH     ),
    .AxiDataWidth     ( AXI_DATA_WIDTH     ),
    .AxiUserWidth     ( AXI_USER_WIDTH     ),
    .AxiIdWidth       ( AXI_ID_WIDTH       ),
    .AxiSlvIdWidth    ( AXI_SLV_ID_WIDTH   ),
    .JobFifoDepth     ( JOB_FIFO_DEPTH     ),
    .BufferDepth      ( BUFFER_DEPTH       ),
    .NumAxInFlight    ( NUM_AX_IN_FLIGHT   ),
    .MemSysDepth      ( MEM_SYS_DEPTH      ),
    .RAWCouplingAvail ( RAW_COUPLING_AVAIL ),
    .IsTwoD           ( IS_TWO_D           ),
    .ObserverPort     ( OBSERVER_PORT      ),
    .BatchPort        ( BATCH_PORT         ),
    .LaunchPort       ( LAUNCH_PORT        ),
    .axi_mst_req_t    ( axi_mst_req_t      ),
    .axi_mst_rsp_t    ( axi_mst_resp_t     ),
    .axi_slv_req_t    ( axi_slv_req_t      ),
    .axi_slv_rsp_t    ( axi_slv_resp_t     ),
    .DEVICE_ID        ( DEVICE_ID          ),
    .PROCESS_ID       ( PROCESS_ID         ),
    .AxID             ( AxID               )
  ) i_dma_core_wrap (
      .obs_issue_o, .obs_done_o, .obs_poll_o, .batch_active_o,
      .launch_hold_i, .launch_go_i, .launch_cancel_i,
      .launch_held_o, .launch_active_o, .launch_cancelled_o, .launch_error_o,
      .obs_issue_id_o, .obs_done_id_o, .obs_poll_id_o,
      .obs_cfg_valid_o, .obs_cfg_write_o, .obs_cfg_addr_o,
      .obs_cfg_wdata_o, .obs_cfg_wstrb_o, .obs_cfg_rdata_i,
    .clk_i,
    .rst_ni,
    .testmode_i,
    .axi_mst_req_o ( axi_mst_req  ),
    .axi_mst_rsp_i ( axi_mst_resp ),
    .axi_slv_req_i ( axi_slv_req  ),
    .axi_slv_rsp_o ( axi_slv_resp )
  );

endmodule : dma_core_wrap_intf
```

---

## tests/idma_frontend_observer_top.sv

```systemverilog
`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module idma_frontend_observer_top (
    input logic clk_i, rst_ni,
    input logic valid_i, write_i,
    input logic [7:0] addr_i,
    input logic [63:0] wdata_i,
    input logic ready_i, complete_i,
    output logic [63:0] rdata_o,
    output logic ready_o, error_o, unchanged_o,
    output logic issue_o, done_o, poll_o, cfg_valid_o,
    output logic [63:0] issue_id_o, done_id_o, poll_id_o,
    output logic batch_active_o,
    output logic [63:0] length_o,
    output logic [1:0] type_o,
    output logic request_o,
    output logic [63:0] src_o, dst_o
);
    typedef struct packed {logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;} req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)
    req_t req;
    rsp_t rsp, gold_rsp;
    burst_t burst, gold_burst;
    logic request, gold_request;
    logic [1:0] ttype, gold_ttype;
    logic [63:0] wdata, gold_wdata;
    assign req = {valid_i, write_i, addr_i, wdata_i, 8'hff};
    idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1'b1), .BatchPort(1'b1)) dut (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp),
        .burst_req_o(burst), .valid_o(request), .ready_i,
        .backend_idle_i(!request), .trans_complete_i(complete_i),
        .ttype_o(ttype), .wdata_o(wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o, .obs_issue_o(issue_o), .obs_done_o(done_o), .obs_poll_o(poll_o),
        .obs_issue_id_o(issue_id_o), .obs_done_id_o(done_id_o), .obs_poll_id_o(poll_id_o),
        .obs_cfg_valid_o(cfg_valid_o), .obs_cfg_write_o(), .obs_cfg_addr_o(),
        .obs_cfg_wdata_o(), .obs_cfg_wstrb_o(), .obs_cfg_rdata_i(64'h49444d414f425301),
        .launch_hold_i(1'b0), .launch_go_i(1'b0), .launch_cancel_i(1'b0),
        .launch_held_o(), .launch_active_o(), .launch_cancelled_o(), .launch_error_o()
    );
    idma_reg64_frontend_before #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4)) gold (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(gold_rsp),
        .burst_req_o(gold_burst), .valid_o(gold_request), .ready_i,
        .backend_idle_i(!request), .trans_complete_i(complete_i),
        .ttype_o(gold_ttype), .wdata_o(gold_wdata), .rdata_i('0), .rdata_valid_i(1'b0)
    );
    assign rdata_o = rsp.rdata;
    assign ready_o = rsp.ready;
    assign error_o = rsp.error;
    assign request_o = request;
    assign length_o = burst.length;
    assign type_o = ttype;
    assign src_o = burst.src_addr;
    assign dst_o = burst.dst_addr;
    assign unchanged_o = (burst == gold_burst) && (request == gold_request) &&
        (ttype == gold_ttype) && (wdata == gold_wdata) &&
        (cfg_valid_o || (rsp == gold_rsp));
endmodule
```

---

## tests/idma_frontend_launch_top.sv

```systemverilog
`include "common_cells/registers.svh"
`include "idma/typedef.svh"
module idma_frontend_launch_top (
    input logic clk_i, rst_ni,
    input logic valid_i, write_i,
    input logic [7:0] addr_i,
    input logic [63:0] wdata_i,
    input logic ready_i, complete_i, idle_i,
    input logic hold_i, go_i, cancel_i,
    input logic [63:0] obs_cfg_rdata_i,
    output logic [63:0] rdata_o,
    output logic ready_o, error_o,
    output logic issue_o, done_o, poll_o,
    output logic [63:0] issue_id_o, done_id_o, poll_id_o,
    output logic launch_held_o, launch_active_o, launch_cancelled_o, launch_error_o,
    output logic cfg_valid_o, cfg_write_o,
    output logic [7:0] cfg_addr_o,
    output logic [63:0] cfg_wdata_o,
    output logic [7:0] cfg_wstrb_o,
    output logic request_o,
    output logic [63:0] length_o,
    output logic [1:0] type_o,
    output logic [63:0] src_o, dst_o
);
    typedef struct packed {logic valid, write; logic [7:0] addr; logic [63:0] wdata; logic [7:0] wstrb;} req_t;
    typedef struct packed {logic ready, error; logic [63:0] rdata;} rsp_t;
    typedef logic [63:0] addr_t;
    typedef logic [3:0] id_t;
    `IDMA_TYPEDEF_FULL_REQ_T(burst_t, id_t, addr_t, addr_t)
    req_t req;
    rsp_t rsp;
    burst_t burst;
    logic request;
    logic [1:0] ttype;
    logic [63:0] wdata;
    assign req = {valid_i, write_i, addr_i, wdata_i, 8'hff};
    idma_reg64_frontend #(.dma_regs_req_t(req_t), .dma_regs_rsp_t(rsp_t),
        .burst_req_t(burst_t), .AxiIdWidth(4), .ObserverPort(1'b1), .BatchPort(1'b1), .LaunchPort(1'b1)) dut (
        .clk_i, .rst_ni, .dma_ctrl_req_i(req), .dma_ctrl_rsp_o(rsp),
        .burst_req_o(burst), .valid_o(request), .ready_i,
        .backend_idle_i(idle_i), .trans_complete_i(complete_i),
        .ttype_o(ttype), .wdata_o(wdata), .rdata_i('0), .rdata_valid_i(1'b0),
        .batch_active_o(),
        .obs_issue_o(issue_o), .obs_done_o(done_o), .obs_poll_o(poll_o),
        .obs_issue_id_o(issue_id_o), .obs_done_id_o(done_id_o), .obs_poll_id_o(poll_id_o),
        .obs_cfg_valid_o(cfg_valid_o), .obs_cfg_write_o(cfg_write_o),
        .obs_cfg_addr_o(cfg_addr_o), .obs_cfg_wdata_o(cfg_wdata_o), .obs_cfg_wstrb_o(cfg_wstrb_o),
        .obs_cfg_rdata_i(obs_cfg_rdata_i),
        .launch_hold_i(hold_i), .launch_go_i(go_i), .launch_cancel_i(cancel_i),
        .launch_held_o, .launch_active_o, .launch_cancelled_o, .launch_error_o
    );
    assign rdata_o = rsp.rdata;
    assign ready_o = rsp.ready;
    assign error_o = rsp.error;
    assign request_o = request;
    assign length_o = burst.length;
    assign type_o = ttype;
    assign src_o = burst.src_addr;
    assign dst_o = burst.dst_addr;
endmodule
```

---

## tests/idma_frontend_launch_test.cpp

```cpp
#include "Vidma_frontend_launch_top.h"
#include "verilated.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>

static void ck(bool b, const char* s) { if (!b) throw std::runtime_error(s); }

struct Bench {
    Vidma_frontend_launch_top d;
    uint64_t cycle = 0;

    // eval() never touches ready_i: tests drive it directly, so ready_i == 0 is
    // genuinely exercised.
    void eval() {
        d.clk_i = 0;
        d.eval();
    }
    void tick() {
        eval();
        d.clk_i = 1;
        d.eval();
        cycle++;
        eval();
    }

    Bench() {
        d.rst_ni = 0;
        d.valid_i = 0;
        d.write_i = 0;
        d.addr_i  = 0;
        d.wdata_i = 0;
        d.ready_i = 1;
        d.complete_i = 0;
        d.idle_i = 1;
        d.hold_i = 0;
        d.go_i = 0;
        d.cancel_i = 0;
        d.obs_cfg_rdata_i = 0;
        tick();
        d.rst_ni = 1;
        tick();
    }

    struct Rsp {
        uint64_t rdata;
        bool error, ready;
        bool launch_error, launch_cancelled, launch_held, launch_active;
        bool issue, done, cfg_valid, cfg_write;
        uint64_t issue_id, done_id, cfg_wdata;
        uint8_t cfg_addr;
    };

    // Copy every combinational output while the request is still asserted, so
    // single-cycle pulses (launch_error, cfg_valid, issue, done) are captured.
    Rsp sample() {
        Rsp r;
        r.rdata            = d.rdata_o;
        r.error            = d.error_o;
        r.ready            = d.ready_o;
        r.launch_error     = d.launch_error_o;
        r.launch_cancelled = d.launch_cancelled_o;
        r.launch_held      = d.launch_held_o;
        r.launch_active    = d.launch_active_o;
        r.issue            = d.issue_o;
        r.done             = d.done_o;
        r.issue_id         = d.issue_id_o;
        r.done_id          = d.done_id_o;
        r.cfg_valid        = d.cfg_valid_o;
        r.cfg_write        = d.cfg_write_o;
        r.cfg_addr         = (uint8_t)d.cfg_addr_o;
        r.cfg_wdata        = d.cfg_wdata_o;
        return r;
    }

    Rsp access2(unsigned addr, bool wr = false, uint64_t val = 0) {
        d.addr_i  = addr;
        d.write_i = wr;
        d.wdata_i = val;
        d.valid_i = 1;
        eval();
        Rsp r = sample();
        tick();
        d.valid_i = 0;
        d.write_i = 0;
        eval();
        return r;
    }

    uint64_t access(unsigned addr, bool wr = false, uint64_t val = 0) {
        return access2(addr, wr, val).rdata;
    }

    // CONF is at 0x18 (STATUS is 0x20 and must not be used as CONF). N_TRANS at
    // 0x48 is not needed for a single DEFAULT 64-byte transfer.
    void configure_controlled(uint64_t src = 0x10000, uint64_t dst = 0x100000,
                              uint64_t bytes = 64, uint64_t conf = 0) {
        access(0x00, true, src);
        access(0x08, true, dst);
        access(0x10, true, bytes);
        access(0x18, true, conf);
    }
};

// Deterministic ID a fresh instance reserves for the first controlled job.
static uint64_t fresh_first_id() {
    Bench b;
    b.configure_controlled();
    b.d.idle_i = 1;
    b.d.hold_i = 1;
    auto r = b.access2(0x28);
    b.d.cancel_i = 1; b.tick(); b.d.cancel_i = 0; b.d.hold_i = 0; b.eval();
    return r.rdata;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    try {
        // 1) Reserve with backend ready=0: one ID, no issue, HOLD stable under stall.
        {
            Bench b;
            b.configure_controlled(0x10000, 0x100000);
            b.d.idle_i = 1;
            b.d.ready_i = 0;      // backend not ready, must not block reservation
            b.d.hold_i = 1;
            auto r = b.access2(0x28);
            ck(r.ready && !r.error, "reserve ready/ok");
            uint64_t reserved = r.rdata;
            ck(!r.issue, "no issue at reservation");
            ck(!r.done, "no done at reservation");
            ck(!r.launch_error, "no launch error at reservation");
            ck(b.d.launch_held_o && b.d.launch_active_o, "held+active after reserve");

            for (int i = 0; i < 100; i++) {
                b.eval();
                ck(!b.d.request_o, "no VALID in HOLD");
                b.tick();
            }
            ck(!b.d.issue_o && !b.d.done_o, "no issue/done in HOLD");

            // locked base write while held
            auto w = b.access2(0x00, true, 0xDEAD0000);
            ck(w.ready && w.error, "base write error while held");
            ck(w.launch_error, "launch_error pulse on locked base write");
            ck(b.d.src_o == 0x10000, "src unchanged by locked write");

            // repeated NEXT_ID while held
            auto r2 = b.access2(0x28);
            ck(r2.ready && r2.error, "second NEXT_ID error");
            ck(r2.launch_error, "launch_error pulse on repeated NEXT_ID");
            ck(!r2.issue, "no issue on repeated NEXT_ID");

            // batch write rejected while held, not poisoning batch_error_q
            auto bw = b.access2(0x88, true, 1);
            ck(bw.ready && bw.error, "batch write rejected while held");
            ck(bw.launch_error, "launch_error on batch write while held");
            auto bs = b.access2(0x88, false);
            ck(((bs.rdata >> 32) & 1) == 0, "batch_error_q not poisoned");

            // go -> COUNT
            b.d.hold_i = 0;
            b.d.go_i = 1;
            b.tick();
            b.d.go_i = 0;
            b.eval();
            ck(b.d.request_o, "VALID after go");
            ck(b.d.src_o == 0x10000 && b.d.dst_o == 0x100000, "addresses stable");
            ck(b.d.launch_active_o, "active in COUNT");
            ck(!b.d.issue_o, "no actual issue while backend not ready");

            for (int i = 0; i < 10; i++) {
                b.eval();
                ck(b.d.request_o, "VALID stable under backend stall");
                ck(b.d.src_o == 0x10000, "addr stable under stall");
                b.tick();
            }
            ck(!b.d.issue_o, "no actual issue under stall");

            // cancel in COUNT must not withdraw VALID
            b.d.cancel_i = 1;
            b.eval();
            ck(b.d.request_o, "cancel in COUNT does not withdraw VALID");
            ck(!b.d.launch_cancelled_o, "no cancelled pulse outside HOLD");
            b.tick();
            b.d.cancel_i = 0;
            b.eval();
            ck(b.d.request_o, "VALID still asserted after COUNT cancel attempt");

            // actual issue when backend ready
            b.d.ready_i = 1;
            b.eval();
            ck(b.d.issue_o && b.d.issue_id_o == reserved, "actual issue with reserved ID");
            b.tick();
            b.d.ready_i = 0;
            b.eval();
            ck(!b.d.request_o, "no VALID in WAIT");

            // completion uses reserved ID
            b.d.complete_i = 1;
            b.eval();
            ck(b.d.done_o && b.d.done_id_o == reserved, "done with reserved ID");
            b.tick();
            b.d.complete_i = 0;
            b.eval();
            ck(!b.d.done_o, "done once");
            ck(!b.d.launch_active_o, "inactive after completion");
        }

        // 2) cancel in HOLD retires ID with no issue/done; next job increments &
        //    completion still reports the newly reserved ID.
        {
            Bench b;
            b.configure_controlled(0x20000, 0x200000);
            b.d.idle_i = 1;
            b.d.ready_i = 0;
            b.d.hold_i = 1;
            auto r3 = b.access2(0x28);
            ck(r3.ready && !r3.error, "reserve ready");
            uint64_t id3 = r3.rdata;
            ck(!r3.issue, "no issue at reserve");
            ck(b.d.launch_held_o, "held");
            ck(!b.d.launch_cancelled_o, "no cancel before");

            b.d.cancel_i = 1;
            b.eval();
            ck(b.d.launch_cancelled_o, "cancel pulse in HOLD");
            ck(!b.d.issue_o && !b.d.done_o, "no issue/done on cancel");
            b.tick();
            b.d.cancel_i = 0;
            b.d.hold_i = 0;
            b.eval();
            ck(!b.d.launch_held_o && !b.d.launch_active_o, "idle after cancel");
            ck(!b.d.launch_cancelled_o, "cancel pulse ended");

            // reserved ID retired: next reservation increments
            b.d.hold_i = 1;
            auto r4 = b.access2(0x28);
            ck(r4.ready && !r4.error, "reserve ready after cancel");
            uint64_t id4 = r4.rdata;
            ck(id4 == id3 + 1, "ID increments after cancel retirement");

            // complete that second job; DONE must use the reserved ID
            b.d.hold_i = 0;
            b.d.ready_i = 0;
            b.d.go_i = 1;
            b.tick();
            b.d.go_i = 0;
            b.eval();
            ck(b.d.request_o, "VALID after go");
            b.d.ready_i = 1;
            b.eval();
            ck(b.d.issue_o && b.d.issue_id_o == id4, "issue reserved ID");
            b.tick();
            b.d.ready_i = 0;
            b.eval();
            b.d.complete_i = 1;
            b.eval();
            ck(b.d.done_o && b.d.done_id_o == id4, "DONE reserved ID after cancel retirement");
            b.tick();
            b.d.complete_i = 0;
            b.eval();
            ck(!b.d.launch_active_o, "idle after completion");
        }

        // 3) simultaneous prepare+cancel rejects without consuming an ID.
        {
            Bench b;
            b.configure_controlled(0x30000, 0x300000);
            b.d.idle_i = 1;
            b.d.ready_i = 0;
            b.d.hold_i = 1;
            b.d.cancel_i = 1;
            b.d.valid_i = 1; b.d.write_i = 0; b.d.addr_i = 0x28;
            b.eval();
            ck(b.d.ready_o && b.d.error_o, "simultaneous cancel rejects");
            ck(b.d.launch_error_o, "launch_error pulse");
            ck(!b.d.issue_o, "no issue");
            b.tick();
            b.d.valid_i = 0; b.d.cancel_i = 0;
            b.eval();

            uint64_t ref = fresh_first_id();
            b.d.hold_i = 1;
            auto r5 = b.access2(0x28);
            ck(r5.ready && !r5.error, "reserve after rejected prep");
            ck(r5.rdata == ref, "rejected prep did not consume an ID");
            b.d.cancel_i = 1; b.tick(); b.d.cancel_i = 0; b.d.hold_i = 0; b.eval();
        }

        // 4) invalid preparations reject and consume no ID.
        {
            Bench b;
            b.configure_controlled(0x40000, 0x400000);
            b.d.idle_i = 1;
            b.d.ready_i = 0;
            uint64_t ref = fresh_first_id();

            auto expect_reject = [&](const char* msg) {
                b.d.hold_i = 1;
                b.d.valid_i = 1; b.d.write_i = 0; b.d.addr_i = 0x28;
                b.eval();
                ck(b.d.ready_o && b.d.error_o, msg);
                ck(b.d.launch_error_o, "launch_error on invalid prep");
                ck(!b.d.issue_o, "no issue on invalid prep");
                b.tick();
                b.d.valid_i = 0; b.d.hold_i = 0;
                b.eval();
            };

            b.access(0x10, true, 128);
            expect_reject("invalid num_bytes reject");
            b.access(0x10, true, 64);

            // real writable CONF field (cache[0]); 0x18 is CONF, not STATUS
            b.access(0x18, true, 1);
            expect_reject("invalid CONF reject");
            b.access(0x18, true, 0);

            b.access(0x00, true, 0x40001);
            expect_reject("invalid src alignment reject");
            b.access(0x00, true, 0x40000);

            b.access(0x08, true, 0x400001);
            expect_reject("invalid dst alignment reject");
            b.access(0x08, true, 0x400000);

            b.d.idle_i = 0;
            expect_reject("backend busy reject");
            b.d.idle_i = 1;

            b.access(0x88, true, 1);
            expect_reject("batch enabled reject");
            b.access(0x88, true, 0);

            b.d.hold_i = 1;
            auto r6 = b.access2(0x28);
            ck(r6.ready && !r6.error, "valid reserve after rejects");
            ck(r6.rdata == ref, "invalid preps consumed no ID");
            b.d.cancel_i = 1; b.tick(); b.d.cancel_i = 0; b.d.hold_i = 0; b.eval();
        }

        // 5) launcher window exact 8-bit decode, no aliasing, forwarding observable.
        {
            Bench b;
            b.configure_controlled(0x50000, 0x500000);
            b.d.idle_i = 1;
            b.d.ready_i = 0;
            b.d.obs_cfg_rdata_i = 0xDEADBEEFCAFEBABEULL;

            auto lr = b.access2(0xd0, false);
            ck(lr.ready && !lr.error, "launcher read ready");
            ck(lr.rdata == 0xDEADBEEFCAFEBABEULL, "launcher read forwarded");
            ck(lr.cfg_valid, "launcher read cfg_valid sampled while valid");
            ck(!lr.cfg_write, "launcher read not write");
            ck(lr.cfg_addr == 0xd0, "launcher read addr exact");

            auto lw = b.access2(0xd0, true, 0x1234);
            ck(lw.ready && !lw.error, "launcher write accepted");
            ck(lw.cfg_valid && lw.cfg_write, "launcher write forwarded (cfg_valid/cfg_write)");
            ck(lw.cfg_addr == 0xd0, "launcher write addr");
            ck(lw.cfg_wdata == 0x1234, "launcher write data");

            auto lw2 = b.access2(0xd8, true, 0x55);
            ck(lw2.cfg_valid && lw2.cfg_addr == 0xd8, "0xd8 distinct from 0x58");

            auto o1 = b.access2(0x58, false);
            ck(o1.cfg_valid && o1.cfg_addr == 0x58, "0x58 legacy observer");

            auto ar = b.access2(0x50, false);
            ck(!ar.cfg_valid, "0x50 active-cycles not launcher");
            auto br = b.access2(0x80, false);
            ck(br.rdata == 0x49444d4142544301ULL, "batch magic intact");
            ck(!br.cfg_valid, "batch not launcher");
            auto sr = b.access2(0x00, false);
            ck(sr.rdata == 0x50000, "base SRC read intact");
            ck(!sr.cfg_valid, "base not launcher");
        }

        // 6) stale go/cancel in IDLE are harmless.
        {
            Bench b;
            b.configure_controlled(0x70000, 0x700000);
            b.d.idle_i = 1;
            b.d.ready_i = 0;
            b.d.go_i = 1; b.d.cancel_i = 1;
            b.eval();
            ck(!b.d.launch_cancelled_o, "no cancelled pulse in IDLE");
            ck(!b.d.launch_held_o, "not held in IDLE");
            ck(!b.d.launch_active_o, "not active in IDLE");
            b.tick();
            b.d.go_i = 0; b.d.cancel_i = 0;
            b.eval();
            ck(!b.d.request_o, "no VALID from stale go");

            uint64_t ref = fresh_first_id();
            b.d.hold_i = 1;
            auto r7 = b.access2(0x28);
            ck(r7.ready && !r7.error && r7.rdata == ref, "stale go/cancel did not affect reservation");
            b.d.cancel_i = 1; b.tick(); b.d.cancel_i = 0; b.d.hold_i = 0; b.eval();
        }

        // 7) normal 64B transfer (hold=0) is unchanged.
        {
            Bench b;
            b.configure_controlled(0x60000, 0x600000);
            b.d.idle_i = 1;
            b.d.ready_i = 1;
            b.d.hold_i = 0;
            b.d.valid_i = 1; b.d.write_i = 0; b.d.addr_i = 0x28;
            b.eval();
            ck(b.d.ready_o, "normal NEXT_ID ready");
            ck(b.d.issue_o, "normal obs issue at NEXT_ID");
            ck(!b.d.error_o, "normal no error");
            b.tick();
            b.d.valid_i = 0;
            b.eval();
            ck(b.d.request_o, "normal VALID after NEXT_ID");
            ck(b.d.src_o == 0x60000, "normal src stable");
            b.tick();
            b.d.ready_i = 0;
            b.eval();
            ck(!b.d.request_o, "normal no VALID in WAIT");
            b.d.complete_i = 1;
            b.eval();
            ck(b.d.done_o, "normal done");
            b.tick();
            b.d.complete_i = 0;
            b.eval();
        }

        std::cout << "PASS launch frontend scenarios\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL " << e.what() << "\n";
        return 1;
    }
}
```

---

## tests/test_idma_frontend_launch.py

```python
#!/usr/bin/env python3
"""Compile and run the launch-port frontend test against the real generated
register file and frontend. Compile log is saved and only printed on failure."""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
cva6 = root / "cva6"
out = root / "out/e3-controlled-20260918/frontend-launch"
out.mkdir(parents=True, exist_ok=True)

prim = cva6 / "corev_apu/register_interface/vendor/lowrisc_opentitan/src"
files = [
    cva6 / "vendor/pulp-platform/common_cells/include/common_cells/registers.svh",
    cva6 / "vendor/pulp-platform/axi/src/axi_pkg.sv",
    cva6 / "corev_apu/iDMA/src/idma_pkg.sv",
    cva6 / "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv",
    prim / "prim_subreg_arb.sv",
    prim / "prim_subreg.sv",
    prim / "prim_subreg_ext.sv",
    cva6 / "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv",
    cva6 / "corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv",
    cva6 / "corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv",
    root / "tests/idma_frontend_launch_top.sv",
    root / "tests/idma_frontend_launch_test.cpp",
]

cmd = [
    "verilator", "--cc", "--exe", "--build", "-Wno-fatal",
    "--top-module", "idma_frontend_launch_top",
    "--Mdir", str(out),
    "-I" + str(cva6 / "vendor/pulp-platform/common_cells/include"),
    "-I" + str(cva6 / "corev_apu/iDMA/src/include"),
] + list(map(str, files))

log_path = out / "build.log"
with log_path.open("w") as log:
    res = subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT)
if res.returncode != 0:
    print(log_path.read_text())
    raise SystemExit(res.returncode)

res = subprocess.run([str(out / "Vidma_frontend_launch_top")])
if res.returncode != 0:
    raise SystemExit(res.returncode)
```

---

## Assumptions, uncertainties, unresolved issues

- **`idma_pkg::DEFAULT` == 2'b00 assumed for the "all-zero CONF" rule.** `launch_cfg_ok` compares `dma_reg2hw.conf` per generated `reg_pkg` field (`ttype==DEFAULT`, `pv/cache/decouple/deburst/serialize==0`). If `DEFAULT != 0`, "CONF exactly 0" in the task would conflict with the pre-existing `batch_cfg_ok` that also compares `ttype.q == idma_pkg::DEFAULT`; the per-field form is the safe choice and is what is proposed.
- **CONF writability**: `reg_pkg` `PERMIT[3] = 4'b0001`, i.e. only byte 0 (cache[3:0], ttype[1:0]) is software-writable. The invalid-CONF test writes `1` at `0x18`, which lands on `cache[0]` (a real writable field), deliberately not a reserved bit. If Codex finds the actual generated reg_top masks cache, switch the invalid value to `0x10` (ttype=1) — that bit is in the same writable byte.
- **Reserved-ID numeric values are not hard-coded.** Because `idma_transfer_id_gen.sv` was not supplied, the test captures the first reservation ID and checks relative behavior (`id4 == id3+1`, and equality against a fresh-instance reference for the "no ID consumed" checks). This avoids asserting an unknown reset value.
- **`dma_hw2reg.done.d` readback** now equals the raw generator `done_id`; the observer tap `obs_done_id_o` reports the reserved ID on controlled completion (per the original spec "done_id matching saved"). These two differ by design; Codex should confirm this matches expectations.
- **Launcher window semantics** (`full_addr >= 0xb0`) are only active when `LaunchPort && ObserverPort`; unknown launcher addresses are forwarded with `error=0`, and the external launcher block defines the error/status. Launcher reads/writes remain permitted while a controlled job is active.
- **Python runner file list/paths** (OpenTitan prim location, iDMA include dir) are carried over from the prior draft; they were not independently verified. Codex should reconcile the Compile `prim`/include paths with the repository's existing frontend test runner before relying on it.
- **`launch_active_o` intentionally asserts for any active transfer** (ordinary COUNT/WAIT included); this is documented in the RTL comment and is distinct from the controlled-job flag `launch_ctrl_q`.

## Suggested acceptance checks (for Codex to run)

1. `python3 tests/test_idma_frontend_launch.py` — expect `PASS launch frontend scenarios` and a saved `out/e3-controlled-20260918/frontend-launch/build.log`.
2. Existing normal/observer and batch frontend runners (unchanged sources) still pass, confirming the historical gold comparison in `idma_frontend_observer_top.sv` still holds with `LaunchPort=0`.
3. Elaborate `dma_core_wrap`/`dma_core_wrap_intf` for both `IS_TWO_D=0/1` and `LAUNCH_PORT=0/1` to confirm no port-width mismatches on `obs_cfg_addr_o[7:0]`.