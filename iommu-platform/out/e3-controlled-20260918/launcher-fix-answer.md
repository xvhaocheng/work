Below are the corrected complete files. I did not run any tests. The frontend/wrapper files are intentionally not edited; the new real integration test targets the frozen updated frontend interface and will only compile after that worker lands.

Assumptions:
- `cva6/Makefile` line 272 already globs `cva6/corev_apu/fpga/src/*.sv`, so no build-file change is needed.
- The frontend worker will expose exactly `LaunchPort`/`LAUNCH_PORT` and `launch_hold_i`, `launch_go_i`, `launch_cancel_i`, `launch_held_o`, `launch_active_o`, `launch_cancelled_o`, `launch_error_o`.
- `idma_reg64_frontend` will widen `obs_cfg_addr_o` to `[7:0]` and forward `full_addr >= 0xb0` through the existing observer config signals when `LaunchPort && ObserverPort`.
- The launcher `NumPorts=4`; ports 0=V, 1=A, 2/3 unused.
- Actual reservation IDs are arbitrary non-zero `NEXT_ID` values; only the AXI select ID is fixed 4, DID 11.
- No tests were executed; all verification claims are proposed acceptance checks only.

---

## `cva6/corev_apu/fpga/src/dma_controlled_launcher.sv`

```systemverilog
// SPDX-License-Identifier: Solderpad-0.51
// Controlled launcher for the iDMA observer-port frontend.
// This block does not gate AXI. It only observes E3 select, frontend status,
// and backend issue/done, and drives hold/go/cancel to the controlled ports.
//
// Error bits [63:32] are sticky until a successful PREP or CLEAR while inactive.
// bit  name
//  0   bad_cfg_strobe          partial cfg_wstrb_i
//  1   bad_command             unknown or unsupported command
//  2   bad_prep                PREP not allowed in current state
//  3   bad_go                  GO not allowed in current state
//  4   abort_inactive          ABORT while not active/frozen
//  5   frontend_error          frontend_error_i while owned
//  6   unexpected_issue        obs_issue_i on unselected port or in PREP
//  7   unexpected_done         obs_done_i on unselected port or in PREP
//  8   done_without_issue      obs_done_i before obs_issue_i
//  9   issue_id_zero           actual issue ID is zero
// 10   done_id_mismatch        done ID differs from captured issue ID
// 11   extra_issue             more than one issue on a lane
// 12   extra_done              more than one done on a lane
// 13   tick_overflow           64-bit tick counter saturated
// 14   bad_index               c0 index outside 0..31
// 15   bad_c8_write            write to read-only c8
// 16   bad_b0_write            write to read-only b0
// 17   cancel_issue_race       frontend_cancelled and obs_issue same edge
module dma_controlled_launcher #(
    parameter int unsigned NumPorts = 4
) (
    input  logic clk_i, rst_ni,
    // 8-bit MMIO cfg window: b0 magic, b8 command/status, c0 index, c8 data.
    input  logic cfg_valid_i, cfg_write_i,
    input  logic [7:0] cfg_addr_i,
    input  logic [63:0] cfg_wdata_i,
    input  logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    // Controlled frontend status.
    input  logic [NumPorts-1:0] frontend_held_i,
    input  logic [NumPorts-1:0] frontend_active_i,
    input  logic [NumPorts-1:0] frontend_cancelled_i,
    input  logic [NumPorts-1:0] frontend_error_i,
    // Actual backend accept/completion.
    input  logic [NumPorts-1:0] obs_issue_i,
    input  logic [NumPorts-1:0] obs_done_i,
    input  logic [63:0] obs_issue_id_i [NumPorts],
    input  logic [63:0] obs_done_id_i [NumPorts],
    // E3 select tap.  Only select_id is AXI-visible; obs IDs are NEXT_ID.
    input  logic selected_i,
    input  logic select_write_i,
    input  logic [7:0] select_id_i,
    input  logic [23:0] select_did_i,
    input  logic [63:0] select_iova_i,
    // Frontend controls.
    output logic [NumPorts-1:0] launch_hold_o,
    output logic [NumPorts-1:0] launch_go_o,
    output logic [NumPorts-1:0] launch_cancel_o
);
    localparam logic [63:0] MAGIC = 64'h49444d414c4e4301;
    localparam int unsigned V_PORT = 0;
    localparam int unsigned A_PORT = 1;
    localparam logic [7:0] A_SEL_ID = 8'd4;
    localparam logic [23:0] A_DID = 24'd11;
    localparam logic [63:0] CMD_PREP = 64'h1;
    localparam logic [63:0] CMD_GO = 64'h2;
    localparam logic [63:0] CMD_ABORT = 64'h4;
    localparam logic [63:0] CMD_CLEAR = 64'h8;
    localparam logic [63:0] CMD_HAS_A = 64'h100;

    localparam int ERR_BAD_STROBE = 0;
    localparam int ERR_BAD_CMD = 1;
    localparam int ERR_BAD_PREP = 2;
    localparam int ERR_BAD_GO = 3;
    localparam int ERR_ABORT = 4;
    localparam int ERR_FRONTEND = 5;
    localparam int ERR_UNEXP_ISSUE = 6;
    localparam int ERR_UNEXP_DONE = 7;
    localparam int ERR_DONE_NO_ISSUE = 8;
    localparam int ERR_ISSUE_ID = 9;
    localparam int ERR_DONE_ID = 10;
    localparam int ERR_EXTRA_ISSUE = 11;
    localparam int ERR_EXTRA_DONE = 12;
    localparam int ERR_TICK_OVF = 13;
    localparam int ERR_BAD_INDEX = 14;
    localparam int ERR_BAD_C8_WRITE = 15;
    localparam int ERR_BAD_B0_WRITE = 16;
    localparam int ERR_CANCEL_ISSUE_RACE = 17;

    typedef enum logic [1:0] {ST_OFF, ST_PREP, ST_RUNNING, ST_FROZEN} state_t;
    state_t state_q, state_n;

    logic [31:0] error_q, error_n;
    logic [63:0] ticks_q, ticks_n;
    logic [NumPorts-1:0] expected_mask_q, expected_mask_n;
    logic [NumPorts-1:0] hold_mask_q, hold_mask_n;
    logic [NumPorts-1:0] issued_mask_q, issued_mask_n;
    logic [NumPorts-1:0] done_mask_q, done_mask_n;
    logic [NumPorts-1:0] cancelled_mask_q, cancelled_mask_n;
    logic [NumPorts-1:0] seen_held_q, seen_held_n;
    logic aborted_q, aborted_n;
    logic has_a_q, has_a_n;
    logic [7:0] index_q, index_n;

    logic [63:0] issue_id_q [NumPorts], issue_id_n [NumPorts];
    logic [63:0] done_id_q [NumPorts], done_id_n [NumPorts];
    logic issue_id_valid_q [NumPorts], issue_id_valid_n [NumPorts];
    logic done_id_valid_q [NumPorts], done_id_valid_n [NumPorts];
    logic [63:0] issue_count_q [NumPorts], issue_count_n [NumPorts];
    logic [63:0] done_count_q [NumPorts], done_count_n [NumPorts];
    logic [63:0] issue_ts_q [NumPorts], issue_ts_n [NumPorts];
    logic [63:0] done_ts_q [NumPorts], done_ts_n [NumPorts];
    logic issue_ts_valid_q [NumPorts], issue_ts_valid_n [NumPorts];
    logic done_ts_valid_q [NumPorts], done_ts_valid_n [NumPorts];

    logic [63:0] go_a_ts_q, go_a_ts_n;
    logic [63:0] a_select_ts_q, a_select_ts_n;
    logic [63:0] go_v_ts_q, go_v_ts_n;
    logic go_a_ts_valid_q, go_a_ts_valid_n;
    logic a_select_ts_valid_q, a_select_ts_valid_n;
    logic go_v_ts_valid_q, go_v_ts_valid_n;

    logic [7:0] selected_id_q, selected_id_n;
    logic [23:0] selected_did_q, selected_did_n;
    logic [63:0] selected_iova_q, selected_iova_n;
    logic selected_write_q, selected_write_n;

    logic [NumPorts-1:0] go_pulse_q, go_pulse_n;
    logic a_go_sent_q, a_go_sent_n;
    logic v_go_sent_q, v_go_sent_n;
    logic a_select_seen_q, a_select_seen_n;

    logic [63:0] status_word;
    logic new_abort_cmd, abort_eff;

    assign new_abort_cmd = cfg_valid_i && cfg_write_i && (cfg_wstrb_i == 8'hff) &&
                           (cfg_addr_i == 8'hb8) && (cfg_wdata_i == CMD_ABORT);
    assign abort_eff = aborted_q || new_abort_cmd;

    assign launch_hold_o = hold_mask_q;
    assign launch_go_o = abort_eff ? '0 : go_pulse_q;

    always_comb begin : status_comb
        status_word = '0;
        status_word[63:32] = error_q;
        status_word[15:12] = 4'(frontend_active_i);
        status_word[11:8]  = 4'(frontend_held_i);
        status_word[3] = aborted_q;
        status_word[2] = (state_q == ST_FROZEN);
        status_word[1] = (state_q == ST_RUNNING);
        status_word[0] = (state_q == ST_PREP);
    end

    always_comb begin : cancel_comb
        launch_cancel_o = '0;
        if ((state_q == ST_PREP || state_q == ST_RUNNING) && abort_eff) begin
            for (int p = 0; p < NumPorts; p++) begin
                if (expected_mask_q[p] && hold_mask_q[p] && frontend_held_i[p] &&
                    !issued_mask_q[p] && !cancelled_mask_q[p]) begin
                    launch_cancel_o[p] = 1'b1;
                end
            end
        end
    end

    always_comb begin : read_comb
        cfg_rdata_o = '0;
        case (cfg_addr_i)
            8'hb0: cfg_rdata_o = MAGIC;
            8'hb8: cfg_rdata_o = status_word;
            8'hc0: cfg_rdata_o = {56'b0, index_q};
            8'hc8: begin
                case (index_q)
                    8'd0:  cfg_rdata_o = MAGIC;
                    8'd1:  cfg_rdata_o = {32'b0, error_q};
                    8'd2:  cfg_rdata_o = status_word;
                    8'd3:  cfg_rdata_o = ticks_q;
                    8'd4:  cfg_rdata_o = go_a_ts_q;
                    8'd5:  cfg_rdata_o = a_select_ts_q;
                    8'd6:  cfg_rdata_o = go_v_ts_q;
                    8'd7:  cfg_rdata_o = issue_ts_q[V_PORT];
                    8'd8:  cfg_rdata_o = issue_ts_q[A_PORT];
                    8'd9:  cfg_rdata_o = done_ts_q[V_PORT];
                    8'd10: cfg_rdata_o = done_ts_q[A_PORT];
                    8'd11: cfg_rdata_o = issue_id_q[V_PORT];
                    8'd12: cfg_rdata_o = issue_id_q[A_PORT];
                    8'd13: cfg_rdata_o = done_id_q[V_PORT];
                    8'd14: cfg_rdata_o = done_id_q[A_PORT];
                    8'd15: cfg_rdata_o = 64'(issued_mask_q);
                    8'd16: cfg_rdata_o = 64'(done_mask_q);
                    8'd17: cfg_rdata_o = 64'(cancelled_mask_q);
                    8'd18: cfg_rdata_o = 64'(seen_held_q);
                    8'd19: cfg_rdata_o = 64'(expected_mask_q);
                    8'd20: cfg_rdata_o = 64'(selected_id_q);
                    8'd21: cfg_rdata_o = 64'(selected_did_q);
                    8'd22: cfg_rdata_o = selected_iova_q;
                    8'd23: cfg_rdata_o = {63'b0, selected_write_q};
                    8'd24: cfg_rdata_o = issue_count_q[V_PORT];
                    8'd25: cfg_rdata_o = issue_count_q[A_PORT];
                    8'd26: cfg_rdata_o = done_count_q[V_PORT];
                    8'd27: cfg_rdata_o = done_count_q[A_PORT];
                    8'd28: cfg_rdata_o = {63'b0, has_a_q};
                    8'd29: cfg_rdata_o = 64'(frontend_held_i);
                    8'd30: cfg_rdata_o = 64'(frontend_active_i);
                    8'd31: cfg_rdata_o = '0;
                    default: cfg_rdata_o = '0;
                endcase
            end
            default: cfg_rdata_o = '0;
        endcase
    end

    always_comb begin : main_comb
        state_n = state_q;
        error_n = error_q;
        ticks_n = ticks_q;
        expected_mask_n = expected_mask_q;
        hold_mask_n = hold_mask_q;
        issued_mask_n = issued_mask_q;
        done_mask_n = done_mask_q;
        cancelled_mask_n = cancelled_mask_q;
        seen_held_n = seen_held_q;
        aborted_n = aborted_q;
        has_a_n = has_a_q;
        index_n = index_q;
        go_a_ts_n = go_a_ts_q;
        a_select_ts_n = a_select_ts_q;
        go_v_ts_n = go_v_ts_q;
        go_a_ts_valid_n = go_a_ts_valid_q;
        a_select_ts_valid_n = a_select_ts_valid_q;
        go_v_ts_valid_n = go_v_ts_valid_q;
        selected_id_n = selected_id_q;
        selected_did_n = selected_did_q;
        selected_iova_n = selected_iova_q;
        selected_write_n = selected_write_q;
        go_pulse_n = '0;
        a_go_sent_n = a_go_sent_q;
        v_go_sent_n = v_go_sent_q;
        a_select_seen_n = a_select_seen_q;

        for (int p = 0; p < NumPorts; p++) begin
            issue_id_n[p] = issue_id_q[p];
            done_id_n[p] = done_id_q[p];
            issue_id_valid_n[p] = issue_id_valid_q[p];
            done_id_valid_n[p] = done_id_valid_q[p];
            issue_count_n[p] = issue_count_q[p];
            done_count_n[p] = done_count_q[p];
            issue_ts_n[p] = issue_ts_q[p];
            done_ts_n[p] = done_ts_q[p];
            issue_ts_valid_n[p] = issue_ts_valid_q[p];
            done_ts_valid_n[p] = done_ts_valid_q[p];
        end

        // Tick counter while owned. Saturates and sets sticky fault.
        if (state_q == ST_PREP || state_q == ST_RUNNING) begin
            if (ticks_q == {64{1'b1}}) begin
                error_n[ERR_TICK_OVF] = 1'b1;
            end else begin
                ticks_n = ticks_q + 1'b1;
            end
        end

        // Track held seen while owned.
        if (state_q == ST_PREP || state_q == ST_RUNNING) begin
            seen_held_n = seen_held_q | frontend_held_i;
        end

        // Record frontend error while owned.
        if ((state_q == ST_PREP || state_q == ST_RUNNING) &&
            |(frontend_error_i & expected_mask_q)) begin
            error_n[ERR_FRONTEND] = 1'b1;
        end

        // Actual backend events.  IDs are arbitrary non-zero NEXT_ID values.
        if (state_q == ST_PREP || state_q == ST_RUNNING) begin
            if (|(obs_issue_i & ~expected_mask_q)) error_n[ERR_UNEXP_ISSUE] = 1'b1;
            if (|(obs_done_i & ~expected_mask_q)) error_n[ERR_UNEXP_DONE] = 1'b1;
            for (int p = 0; p < NumPorts; p++) begin
                if (obs_issue_i[p] && expected_mask_q[p]) begin
                    if (state_q == ST_PREP) error_n[ERR_UNEXP_ISSUE] = 1'b1;
                    if (issued_mask_q[p]) error_n[ERR_EXTRA_ISSUE] = 1'b1;
                    if (obs_issue_id_i[p] == 64'd0) error_n[ERR_ISSUE_ID] = 1'b1;
                    if (!issue_id_valid_q[p]) begin
                        issue_id_n[p] = obs_issue_id_i[p];
                        issue_id_valid_n[p] = 1'b1;
                    end
                    issued_mask_n[p] = 1'b1;
                    hold_mask_n[p] = 1'b0;
                    issue_count_n[p] = issue_count_q[p] + 1'b1;
                    if (!issue_ts_valid_q[p]) begin
                        issue_ts_n[p] = ticks_q;
                        issue_ts_valid_n[p] = 1'b1;
                    end
                end
                if (obs_done_i[p] && expected_mask_q[p]) begin
                    if (state_q == ST_PREP) error_n[ERR_UNEXP_DONE] = 1'b1;
                    if (!issued_mask_q[p] && !obs_issue_i[p]) error_n[ERR_DONE_NO_ISSUE] = 1'b1;
                    if (done_mask_q[p]) error_n[ERR_EXTRA_DONE] = 1'b1;
                    if (issue_id_valid_q[p] && obs_done_id_i[p] != issue_id_q[p]) begin
                        error_n[ERR_DONE_ID] = 1'b1;
                    end
                    done_mask_n[p] = 1'b1;
                    done_count_n[p] = done_count_q[p] + 1'b1;
                    if (!done_id_valid_q[p]) begin
                        done_id_n[p] = obs_done_id_i[p];
                        done_id_valid_n[p] = 1'b1;
                    end
                    if (!done_ts_valid_q[p]) begin
                        done_ts_n[p] = ticks_q;
                        done_ts_valid_n[p] = 1'b1;
                    end
                end
                if (obs_issue_i[p] && frontend_cancelled_i[p]) begin
                    error_n[ERR_CANCEL_ISSUE_RACE] = 1'b1;
                end
            end
        end

        // Command writes.
        if (cfg_valid_i && cfg_write_i) begin
            if (cfg_wstrb_i != 8'hff) begin
                error_n[ERR_BAD_STROBE] = 1'b1;
            end else begin
                case (cfg_addr_i)
                    8'hb0: error_n[ERR_BAD_B0_WRITE] = 1'b1;
                    8'hb8: begin
                        if ((cfg_wdata_i & ~CMD_HAS_A) == CMD_PREP) begin
                            if ((state_q == ST_OFF || state_q == ST_FROZEN) &&
                                frontend_active_i == '0 && frontend_held_i == '0) begin
                                state_n = ST_PREP;
                                aborted_n = 1'b0;
                                error_n = '0;
                                has_a_n = cfg_wdata_i[8];
                                expected_mask_n = '0;
                                expected_mask_n[V_PORT] = 1'b1;
                                expected_mask_n[A_PORT] = cfg_wdata_i[8];
                                hold_mask_n = expected_mask_n;
                                issued_mask_n = '0;
                                done_mask_n = '0;
                                cancelled_mask_n = '0;
                                seen_held_n = '0;
                                ticks_n = '0;
                                go_a_ts_n = '0; a_select_ts_n = '0; go_v_ts_n = '0;
                                go_a_ts_valid_n = 1'b0; a_select_ts_valid_n = 1'b0; go_v_ts_valid_n = 1'b0;
                                selected_id_n = '0; selected_did_n = '0; selected_iova_n = '0; selected_write_n = 1'b0;
                                go_pulse_n = '0;
                                a_go_sent_n = 1'b0; v_go_sent_n = 1'b0; a_select_seen_n = 1'b0;
                                for (int p = 0; p < NumPorts; p++) begin
                                    issue_id_n[p] = '0; done_id_n[p] = '0;
                                    issue_id_valid_n[p] = 1'b0; done_id_valid_n[p] = 1'b0;
                                    issue_count_n[p] = '0; done_count_n[p] = '0;
                                    issue_ts_n[p] = '0; done_ts_n[p] = '0;
                                    issue_ts_valid_n[p] = 1'b0; done_ts_valid_n[p] = 1'b0;
                                end
                            end else begin
                                error_n[ERR_BAD_PREP] = 1'b1;
                            end
                        end else if (cfg_wdata_i == CMD_GO) begin
                            if (state_q == ST_PREP && error_n == '0 &&
                                (frontend_held_i & expected_mask_q) == expected_mask_q) begin
                                state_n = ST_RUNNING;
                                if (has_a_q) begin
                                    go_pulse_n[A_PORT] = 1'b1;
                                    a_go_sent_n = 1'b1;
                                    if (!go_a_ts_valid_q) begin
                                        go_a_ts_n = ticks_q + 1'b1;
                                        go_a_ts_valid_n = 1'b1;
                                    end
                                end else begin
                                    go_pulse_n[V_PORT] = 1'b1;
                                    v_go_sent_n = 1'b1;
                                    if (!go_v_ts_valid_q) begin
                                        go_v_ts_n = ticks_q + 1'b1;
                                        go_v_ts_valid_n = 1'b1;
                                    end
                                end
                            end else begin
                                error_n[ERR_BAD_GO] = 1'b1;
                            end
                        end else if (cfg_wdata_i == CMD_ABORT) begin
                            if (state_q == ST_OFF) begin
                                state_n = ST_FROZEN;
                                aborted_n = 1'b1;
                                hold_mask_n = '0;
                            end else if (state_q == ST_PREP || state_q == ST_RUNNING) begin
                                aborted_n = 1'b1;
                                state_n = ST_RUNNING;
                            end else begin
                                error_n[ERR_ABORT] = 1'b1;
                            end
                        end else if (cfg_wdata_i == CMD_CLEAR) begin
                            if (state_q == ST_OFF || state_q == ST_FROZEN) begin
                                state_n = ST_OFF;
                                aborted_n = 1'b0;
                                error_n = '0;
                                expected_mask_n = '0;
                                hold_mask_n = '0;
                                issued_mask_n = '0;
                                done_mask_n = '0;
                                cancelled_mask_n = '0;
                                seen_held_n = '0;
                                ticks_n = '0;
                                go_a_ts_n = '0; a_select_ts_n = '0; go_v_ts_n = '0;
                                go_a_ts_valid_n = 1'b0; a_select_ts_valid_n = 1'b0; go_v_ts_valid_n = 1'b0;
                                selected_id_n = '0; selected_did_n = '0; selected_iova_n = '0; selected_write_n = 1'b0;
                                go_pulse_n = '0;
                                a_go_sent_n = 1'b0; v_go_sent_n = 1'b0; a_select_seen_n = 1'b0;
                                for (int p = 0; p < NumPorts; p++) begin
                                    issue_id_n[p] = '0; done_id_n[p] = '0;
                                    issue_id_valid_n[p] = 1'b0; done_id_valid_n[p] = 1'b0;
                                    issue_count_n[p] = '0; done_count_n[p] = '0;
                                    issue_ts_n[p] = '0; done_ts_n[p] = '0;
                                    issue_ts_valid_n[p] = 1'b0; done_ts_valid_n[p] = 1'b0;
                                end
                            end else begin
                                error_n[ERR_BAD_CMD] = 1'b1;
                            end
                        end else begin
                            error_n[ERR_BAD_CMD] = 1'b1;
                        end
                    end
                    8'hc0: begin
                        if (cfg_wdata_i < 64'd32) index_n = cfg_wdata_i[7:0];
                        else error_n[ERR_BAD_INDEX] = 1'b1;
                    end
                    8'hc8: error_n[ERR_BAD_C8_WRITE] = 1'b1;
                    default: error_n[ERR_BAD_CMD] = 1'b1;
                endcase
            end
        end

        // A select capture and V go, but ABORT wins same-edge.
        if (!abort_eff && state_q == ST_RUNNING && has_a_q && !a_select_seen_q && !v_go_sent_q) begin
            if (selected_i && !select_write_i && select_id_i == A_SEL_ID && select_did_i == A_DID &&
                (obs_issue_i[A_PORT] || issued_mask_q[A_PORT])) begin
                a_select_seen_n = 1'b1;
                selected_id_n = select_id_i;
                selected_did_n = select_did_i;
                selected_iova_n = select_iova_i;
                selected_write_n = select_write_i;
                if (!a_select_ts_valid_q) begin
                    a_select_ts_n = ticks_q;
                    a_select_ts_valid_n = 1'b1;
                end
                go_pulse_n[V_PORT] = 1'b1;
                v_go_sent_n = 1'b1;
                if (!go_v_ts_valid_q) begin
                    go_v_ts_n = ticks_q + 1'b1;
                    go_v_ts_valid_n = 1'b1;
                end
            end
        end

        // Abort drain: record only actual frontend_cancelled pulses.
        if ((state_q == ST_PREP || state_q == ST_RUNNING) && abort_eff) begin
            for (int p = 0; p < NumPorts; p++) begin
                if (expected_mask_q[p] && hold_mask_q[p] && frontend_cancelled_i[p]) begin
                    if (obs_issue_i[p]) begin
                        error_n[ERR_CANCEL_ISSUE_RACE] = 1'b1;
                    end else if (!cancelled_mask_q[p]) begin
                        cancelled_mask_n[p] = 1'b1;
                        hold_mask_n[p] = 1'b0;
                    end
                end
            end
        end

        // Finish conditions, only after drain.
        if (state_q == ST_RUNNING && !aborted_q) begin
            if (issued_mask_q == expected_mask_q && done_mask_q == expected_mask_q &&
                (frontend_held_i & expected_mask_q) == '0 &&
                (frontend_active_i & expected_mask_q) == '0) begin
                state_n = ST_FROZEN;
                hold_mask_n = '0;
            end
        end
        if (state_q == ST_RUNNING && aborted_q) begin
            if ((frontend_held_i & expected_mask_q) == '0 &&
                (frontend_active_i & expected_mask_q) == '0 &&
                (issued_mask_q & ~done_mask_q) == '0 &&
                ((expected_mask_q & ~issued_mask_q & ~cancelled_mask_q & seen_held_q) == '0)) begin
                state_n = ST_FROZEN;
                hold_mask_n = '0;
            end
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            state_q <= ST_OFF;
            error_q <= '0;
            ticks_q <= '0;
            expected_mask_q <= '0;
            hold_mask_q <= '0;
            issued_mask_q <= '0;
            done_mask_q <= '0;
            cancelled_mask_q <= '0;
            seen_held_q <= '0;
            aborted_q <= 1'b0;
            has_a_q <= 1'b0;
            index_q <= '0;
            go_a_ts_q <= '0; a_select_ts_q <= '0; go_v_ts_q <= '0;
            go_a_ts_valid_q <= 1'b0; a_select_ts_valid_q <= 1'b0; go_v_ts_valid_q <= 1'b0;
            selected_id_q <= '0; selected_did_q <= '0; selected_iova_q <= '0; selected_write_q <= 1'b0;
            go_pulse_q <= '0;
            a_go_sent_q <= 1'b0; v_go_sent_q <= 1'b0; a_select_seen_q <= 1'b0;
            for (int p = 0; p < NumPorts; p++) begin
                issue_id_q[p] <= '0; done_id_q[p] <= '0;
                issue_id_valid_q[p] <= 1'b0; done_id_valid_q[p] <= 1'b0;
                issue_count_q[p] <= '0; done_count_q[p] <= '0;
                issue_ts_q[p] <= '0; done_ts_q[p] <= '0;
                issue_ts_valid_q[p] <= 1'b0; done_ts_valid_q[p] <= 1'b0;
            end
        end else begin
            state_q <= state_n;
            error_q <= error_n;
            ticks_q <= ticks_n;
            expected_mask_q <= expected_mask_n;
            hold_mask_q <= hold_mask_n;
            issued_mask_q <= issued_mask_n;
            done_mask_q <= done_mask_n;
            cancelled_mask_q <= cancelled_mask_n;
            seen_held_q <= seen_held_n;
            aborted_q <= aborted_n;
            has_a_q <= has_a_n;
            index_q <= index_n;
            go_a_ts_q <= go_a_ts_n; a_select_ts_q <= a_select_ts_n; go_v_ts_q <= go_v_ts_n;
            go_a_ts_valid_q <= go_a_ts_valid_n; a_select_ts_valid_q <= a_select_ts_valid_n;
            go_v_ts_valid_q <= go_v_ts_valid_n;
            selected_id_q <= selected_id_n; selected_did_q <= selected_did_n;
            selected_iova_q <= selected_iova_n; selected_write_q <= selected_write_n;
            go_pulse_q <= go_pulse_n;
            a_go_sent_q <= a_go_sent_n; v_go_sent_q <= v_go_sent_n; a_select_seen_q <= a_select_seen_n;
            for (int p = 0; p < NumPorts; p++) begin
                issue_id_q[p] <= issue_id_n[p]; done_id_q[p] <= done_id_n[p];
                issue_id_valid_q[p] <= issue_id_valid_n[p]; done_id_valid_q[p] <= done_id_valid_n[p];
                issue_count_q[p] <= issue_count_n[p]; done_count_q[p] <= done_count_n[p];
                issue_ts_q[p] <= issue_ts_n[p]; done_ts_q[p] <= done_ts_n[p];
                issue_ts_valid_q[p] <= issue_ts_valid_n[p]; done_ts_valid_q[p] <= done_ts_valid_n[p];
            end
        end
    end
endmodule
```

---

## `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`

```systemverilog
// Copyright 2018 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// Xilinx Peripherals

`include "axi/assign.svh"
`include "axi/typedef.svh"
`include "axi-iommu/assign.svh"
`include "axi-iommu/typedef.svh"
`include "register_interface/assign.svh"
`include "register_interface/typedef.svh"

module ariane_peripherals #(
    parameter int AxiAddrWidth = -1,
    parameter int AxiDataWidth = -1,
    parameter int AxiIdWidth   = -1,
    parameter int AxiUserWidth =  1,
    parameter bit InclUART     =  1,
    parameter bit InclSPI      =  0,
    parameter bit InclEthernet =  0,
    parameter bit InclGPIO     =  0,
    parameter bit InclTimer    =  1,
    parameter bit InclDMA      =  0,
    parameter bit InclIOMMU    =  0,
    parameter int unsigned IOMMUDSFixedDelayInput  = 0,
    parameter int unsigned IOMMUDSFixedDelayOutput = 0
) (
    input  logic       clk_i           , // Clock
    input  logic       clk_200MHz_i    ,
    input  logic       rst_ni          , // Asynchronous reset active low
    AXI_BUS.Slave      plic            ,
    AXI_BUS.Slave      uart            ,
    AXI_BUS.Slave      spi             ,
    AXI_BUS.Slave      gpio            ,
    AXI_BUS.Slave      ethernet        ,
    AXI_BUS.Slave      timer           ,
    AXI_BUS.Slave      dma_cfg         , // DMA Engine configuration IF     (XBAR  => DMA  )
    AXI_BUS.Master     iommu_comp      , // IOMMU Completion IF             (IOMMU => XBAR )
    AXI_BUS.Master     iommu_ds        , // IOMMU Data Structures IF        (IOMMU => XBAR )
    AXI_BUS.Slave      iommu_prog      , // IOMMU Programming IF            (XBAR  => IOMMU)
    output logic [1:0] irq_o           ,
    // UART
    input  logic       rx_i            ,
    output logic       tx_o            ,
    // Ethernet
    input  logic       eth_clk_i       ,
    input  wire        eth_rxck        ,
    input  wire        eth_rxctl       ,
    input  wire [3:0]  eth_rxd         ,
    output wire        eth_txck        ,
    output wire        eth_txctl       ,
    output wire [3:0]  eth_txd         ,
    output wire        eth_rst_n       ,
    input  logic       phy_tx_clk_i    , // 125 MHz Clock
    // MDIO Interface
    inout  wire        eth_mdio        ,
    output logic       eth_mdc         ,
    // SPI
    output logic       spi_clk_o       ,
    output logic       spi_mosi        ,
    input  logic       spi_miso        ,
    output logic       spi_ss          ,
    // SD Card
    input  logic       sd_clk_i        ,
    output logic [7:0] leds_o          ,
    input  logic [7:0] dip_switches_i
);

    // ---------------
    // 1. PLIC
    // ---------------
    logic [ariane_soc::NumSources-1:0] irq_sources;

    // Unused interrupt sources
    assign irq_sources[(ariane_soc::NumSources-1):(ariane_soc::RVIOMMUCfg.NumIntVec+7)] = '0;

    REG_BUS #(
        .ADDR_WIDTH ( 32 ),
        .DATA_WIDTH ( 32 )
    ) reg_bus (clk_i);

    logic         plic_penable;
    logic         plic_pwrite;
    logic [31:0]  plic_paddr;
    logic         plic_psel;
    logic [31:0]  plic_pwdata;
    logic [31:0]  plic_prdata;
    logic         plic_pready;
    logic         plic_pslverr;

    axi2apb_64_32 #(
        .AXI4_ADDRESS_WIDTH ( AxiAddrWidth  ),
        .AXI4_RDATA_WIDTH   ( AxiDataWidth  ),
        .AXI4_WDATA_WIDTH   ( AxiDataWidth  ),
        .AXI4_ID_WIDTH      ( AxiIdWidth    ),
        .AXI4_USER_WIDTH    ( AxiUserWidth  ),
        .BUFF_DEPTH_SLAVE   ( 2             ),
        .APB_ADDR_WIDTH     ( 32            )
    ) i_axi2apb_64_32_plic (
        .ACLK      ( clk_i          ),
        .ARESETn   ( rst_ni         ),
        .test_en_i ( 1'b0           ),
        .AWID_i    ( plic.aw_id     ),
        .AWADDR_i  ( plic.aw_addr   ),
        .AWLEN_i   ( plic.aw_len    ),
        .AWSIZE_i  ( plic.aw_size   ),
        .AWBURST_i ( plic.aw_burst  ),
        .AWLOCK_i  ( plic.aw_lock   ),
        .AWCACHE_i ( plic.aw_cache  ),
        .AWPROT_i  ( plic.aw_prot   ),
        .AWREGION_i( plic.aw_region ),
        .AWUSER_i  ( plic.aw_user   ),
        .AWQOS_i   ( plic.aw_qos    ),
        .AWVALID_i ( plic.aw_valid  ),
        .AWREADY_o ( plic.aw_ready  ),
        .WDATA_i   ( plic.w_data    ),
        .WSTRB_i   ( plic.w_strb    ),
        .WLAST_i   ( plic.w_last    ),
        .WUSER_i   ( plic.w_user    ),
        .WVALID_i  ( plic.w_valid   ),
        .WREADY_o  ( plic.w_ready   ),
        .BID_o     ( plic.b_id      ),
        .BRESP_o   ( plic.b_resp    ),
        .BVALID_o  ( plic.b_valid   ),
        .BUSER_o   ( plic.b_user    ),
        .BREADY_i  ( plic.b_ready   ),
        .ARID_i    ( plic.ar_id     ),
        .ARADDR_i  ( plic.ar_addr   ),
        .ARLEN_i   ( plic.ar_len    ),
        .ARSIZE_i  ( plic.ar_size   ),
        .ARBURST_i ( plic.ar_burst  ),
        .ARLOCK_i  ( plic.ar_lock   ),
        .ARCACHE_i ( plic.ar_cache  ),
        .ARPROT_i  ( plic.ar_prot   ),
        .ARREGION_i( plic.ar_region ),
        .ARUSER_i  ( plic.ar_user   ),
        .ARQOS_i   ( plic.ar_qos    ),
        .ARVALID_i ( plic.ar_valid  ),
        .ARREADY_o ( plic.ar_ready  ),
        .RID_o     ( plic.r_id      ),
        .RDATA_o   ( plic.r_data    ),
        .RRESP_o   ( plic.r_resp    ),
        .RLAST_o   ( plic.r_last    ),
        .RUSER_o   ( plic.r_user    ),
        .RVALID_o  ( plic.r_valid   ),
        .RREADY_i  ( plic.r_ready   ),
        .PENABLE   ( plic_penable   ),
        .PWRITE    ( plic_pwrite    ),
        .PADDR     ( plic_paddr     ),
        .PSEL      ( plic_psel      ),
        .PWDATA    ( plic_pwdata    ),
        .PRDATA    ( plic_prdata    ),
        .PREADY    ( plic_pready    ),
        .PSLVERR   ( plic_pslverr   )
    );

    apb_to_reg i_apb_to_reg (
        .clk_i     ( clk_i        ),
        .rst_ni    ( rst_ni       ),
        .penable_i ( plic_penable ),
        .pwrite_i  ( plic_pwrite  ),
        .paddr_i   ( plic_paddr   ),
        .psel_i    ( plic_psel    ),
        .pwdata_i  ( plic_pwdata  ),
        .prdata_o  ( plic_prdata  ),
        .pready_o  ( plic_pready  ),
        .pslverr_o ( plic_pslverr ),
        .reg_o     ( reg_bus      )
    );

    // define reg type according to REG_BUS above
    `REG_BUS_TYPEDEF_ALL(plic, logic[31:0], logic[31:0], logic[3:0])
    plic_req_t plic_req;
    plic_rsp_t plic_rsp;

    // assign REG_BUS.out to (req_t, rsp_t) pair
    `REG_BUS_ASSIGN_TO_REQ(plic_req, reg_bus)
    `REG_BUS_ASSIGN_FROM_RSP(reg_bus, plic_rsp)

    plic_top #(
      .N_SOURCE    ( ariane_soc::NumSources  ),
      .N_TARGET    ( ariane_soc::NumTargets  ),
      .MAX_PRIO    ( ariane_soc::MaxPriority ),
      .reg_req_t   ( plic_req_t              ),
      .reg_rsp_t   ( plic_rsp_t              )
    ) i_plic (
      .clk_i,
      .rst_ni,
      .req_i         ( plic_req    ),
      .resp_o        ( plic_rsp    ),
      .le_i          ( '0          ), // 0:level 1:edge
      .irq_sources_i ( irq_sources ),
      .eip_targets_o ( irq_o       )
    );

    // ---------------
    // 2. UART
    // ---------------
    logic         uart_penable;
    logic         uart_pwrite;
    logic [31:0]  uart_paddr;
    logic         uart_psel;
    logic [31:0]  uart_pwdata;
    logic [31:0]  uart_prdata;
    logic         uart_pready;
    logic         uart_pslverr;

    axi2apb_64_32 #(
        .AXI4_ADDRESS_WIDTH ( AxiAddrWidth ),
        .AXI4_RDATA_WIDTH   ( AxiDataWidth ),
        .AXI4_WDATA_WIDTH   ( AxiDataWidth ),
        .AXI4_ID_WIDTH      ( AxiIdWidth   ),
        .AXI4_USER_WIDTH    ( AxiUserWidth ),
        .BUFF_DEPTH_SLAVE   ( 2            ),
        .APB_ADDR_WIDTH     ( 32           )
    ) i_axi2apb_64_32_uart (
        .ACLK      ( clk_i          ),
        .ARESETn   ( rst_ni         ),
        .test_en_i ( 1'b0           ),
        .AWID_i    ( uart.aw_id     ),
        .AWADDR_i  ( uart.aw_addr   ),
        .AWLEN_i   ( uart.aw_len    ),
        .AWSIZE_i  ( uart.aw_size   ),
        .AWBURST_i ( uart.aw_burst  ),
        .AWLOCK_i  ( uart.aw_lock   ),
        .AWCACHE_i ( uart.aw_cache  ),
        .AWPROT_i  ( uart.aw_prot   ),
        .AWREGION_i( uart.aw_region ),
        .AWUSER_i  ( uart.aw_user   ),
        .AWQOS_i   ( uart.aw_qos    ),
        .AWVALID_i ( uart.aw_valid  ),
        .AWREADY_o ( uart.aw_ready  ),
        .WDATA_i   ( uart.w_data    ),
        .WSTRB_i   ( uart.w_strb    ),
        .WLAST_i   ( uart.w_last    ),
        .WUSER_i   ( uart.w_user    ),
        .WVALID_i  ( uart.w_valid   ),
        .WREADY_o  ( uart.w_ready   ),
        .BID_o     ( uart.b_id      ),
        .BRESP_o   ( uart.b_resp    ),
        .BVALID_o  ( uart.b_valid   ),
        .BUSER_o   ( uart.b_user    ),
        .BREADY_i  ( uart.b_ready   ),
        .ARID_i    ( uart.ar_id     ),
        .ARADDR_i  ( uart.ar_addr   ),
        .ARLEN_i   ( uart.ar_len    ),
        .ARSIZE_i  ( uart.ar_size   ),
        .ARBURST_i ( uart.ar_burst  ),
        .ARLOCK_i  ( uart.ar_lock   ),
        .ARCACHE_i ( uart.ar_cache  ),
        .ARPROT_i  ( uart.ar_prot   ),
        .ARREGION_i( uart.ar_region ),
        .ARUSER_i  ( uart.ar_user   ),
        .ARQOS_i   ( uart.ar_qos    ),
        .ARVALID_i ( uart.ar_valid  ),
        .ARREADY_o ( uart.ar_ready  ),
        .RID_o     ( uart.r_id      ),
        .RDATA_o   ( uart.r_data    ),
        .RRESP_o   ( uart.r_resp    ),
        .RLAST_o   ( uart.r_last    ),
        .RUSER_o   ( uart.r_user    ),
        .RVALID_o  ( uart.r_valid   ),
        .RREADY_i  ( uart.r_ready   ),
        .PENABLE   ( uart_penable   ),
        .PWRITE    ( uart_pwrite    ),
        .PADDR     ( uart_paddr     ),
        .PSEL      ( uart_psel      ),
        .PWDATA    ( uart_pwdata    ),
        .PRDATA    ( uart_prdata    ),
        .PREADY    ( uart_pready    ),
        .PSLVERR   ( uart_pslverr   )
    );

    if (InclUART) begin : gen_uart
        apb_uart i_apb_uart (
            .CLK     ( clk_i           ),
            .RSTN    ( rst_ni          ),
            .PSEL    ( uart_psel       ),
            .PENABLE ( uart_penable    ),
            .PWRITE  ( uart_pwrite     ),
            .PADDR   ( uart_paddr[4:2] ),
            .PWDATA  ( uart_pwdata     ),
            .PRDATA  ( uart_prdata     ),
            .PREADY  ( uart_pready     ),
            .PSLVERR ( uart_pslverr    ),
            .INT     ( irq_sources[0]  ),
            .OUT1N   (                 ), // keep open
            .OUT2N   (                 ), // keep open
            .RTSN    (                 ), // no flow control
            .DTRN    (                 ), // no flow control
            .CTSN    ( 1'b0            ),
            .DSRN    ( 1'b0            ),
            .DCDN    ( 1'b0            ),
            .RIN     ( 1'b0            ),
            .SIN     ( rx_i            ),
            .SOUT    ( tx_o            )
        );
    end else begin
        /* pragma translate_off */
        `ifndef VERILATOR
        mock_uart i_mock_uart (
            .clk_i     ( clk_i        ),
            .rst_ni    ( rst_ni       ),
            .penable_i ( uart_penable ),
            .pwrite_i  ( uart_pwrite  ),
            .paddr_i   ( uart_paddr   ),
            .psel_i    ( uart_psel    ),
            .pwdata_i  ( uart_pwdata  ),
            .prdata_o  ( uart_prdata  ),
            .pready_o  ( uart_pready  ),
            .pslverr_o ( uart_pslverr )
        );
        `endif
        /* pragma translate_on */
    end

    // ---------------
    // 3. SPI
    // ---------------
    assign spi.b_user = 1'b0;
    assign spi.r_user = 1'b0;

    if (InclSPI) begin : gen_spi
        logic [31:0] s_axi_spi_awaddr;
        logic [7:0]  s_axi_spi_awlen;
        logic [2:0]  s_axi_spi_awsize;
        logic [1:0]  s_axi_spi_awburst;
        logic [0:0]  s_axi_spi_awlock;
        logic [3:0]  s_axi_spi_awcache;
        logic [2:0]  s_axi_spi_awprot;
        logic [3:0]  s_axi_spi_awregion;
        logic [3:0]  s_axi_spi_awqos;
        logic        s_axi_spi_awvalid;
        logic        s_axi_spi_awready;
        logic [31:0] s_axi_spi_wdata;
        logic [3:0]  s_axi_spi_wstrb;
        logic        s_axi_spi_wlast;
        logic        s_axi_spi_wvalid;
        logic        s_axi_spi_wready;
        logic [1:0]  s_axi_spi_bresp;
        logic        s_axi_spi_bvalid;
        logic        s_axi_spi_bready;
        logic [31:0] s_axi_spi_araddr;
        logic [7:0]  s_axi_spi_arlen;
        logic [2:0]  s_axi_spi_arsize;
        logic [1:0]  s_axi_spi_arburst;
        logic [0:0]  s_axi_spi_arlock;
        logic [3:0]  s_axi_spi_arcache;
        logic [2:0]  s_axi_spi_arprot;
        logic [3:0]  s_axi_spi_arregion;
        logic [3:0]  s_axi_spi_arqos;
        logic        s_axi_spi_arvalid;
        logic        s_axi_spi_arready;
        logic [31:0] s_axi_spi_rdata;
        logic [1:0]  s_axi_spi_rresp;
        logic        s_axi_spi_rlast;
        logic        s_axi_spi_rvalid;
        logic        s_axi_spi_rready;

        xlnx_axi_dwidth_converter i_xlnx_axi_dwidth_converter_spi (
            .s_axi_aclk     ( clk_i              ),
            .s_axi_aresetn  ( rst_ni             ),

            .s_axi_awid     ( spi.aw_id          ),
            .s_axi_awaddr   ( spi.aw_addr[31:0]  ),
            .s_axi_awlen    ( spi.aw_len         ),
            .s_axi_awsize   ( spi.aw_size        ),
            .s_axi_awburst  ( spi.aw_burst       ),
            .s_axi_awlock   ( spi.aw_lock        ),
            .s_axi_awcache  ( spi.aw_cache       ),
            .s_axi_awprot   ( spi.aw_prot        ),
            .s_axi_awregion ( spi.aw_region      ),
            .s_axi_awqos    ( spi.aw_qos         ),
            .s_axi_awvalid  ( spi.aw_valid       ),
            .s_axi_awready  ( spi.aw_ready       ),
            .s_axi_wdata    ( spi.w_data         ),
            .s_axi_wstrb    ( spi.w_strb         ),
            .s_axi_wlast    ( spi.w_last         ),
            .s_axi_wvalid   ( spi.w_valid        ),
            .s_axi_wready   ( spi.w_ready        ),
            .s_axi_bid      ( spi.b_id           ),
            .s_axi_bresp    ( spi.b_resp         ),
            .s_axi_bvalid   ( spi.b_valid        ),
            .s_axi_bready   ( spi.b_ready        ),
            .s_axi_arid     ( spi.ar_id          ),
            .s_axi_araddr   ( spi.ar_addr[31:0]  ),
            .s_axi_arlen    ( spi.ar_len         ),
            .s_axi_arsize   ( spi.ar_size        ),
            .s_axi_arburst  ( spi.ar_burst       ),
            .s_axi_arlock   ( spi.ar_lock        ),
            .s_axi_arcache  ( spi.ar_cache       ),
            .s_axi_arprot   ( spi.ar_prot        ),
            .s_axi_arregion ( spi.ar_region      ),
            .s_axi_arqos    ( spi.ar_qos         ),
            .s_axi_arvalid  ( spi.ar_valid       ),
            .s_axi_arready  ( spi.ar_ready       ),
            .s_axi_rid      ( spi.r_id           ),
            .s_axi_rdata    ( spi.r_data         ),
            .s_axi_rresp    ( spi.r_resp         ),
            .s_axi_rlast    ( spi.r_last         ),
            .s_axi_rvalid   ( spi.r_valid        ),
            .s_axi_rready   ( spi.r_ready        ),

            .m_axi_awaddr   ( s_axi_spi_awaddr   ),
            .m_axi_awlen    ( s_axi_spi_awlen    ),
            .m_axi_awsize   ( s_axi_spi_awsize   ),
            .m_axi_awburst  ( s_axi_spi_awburst  ),
            .m_axi_awlock   ( s_axi_spi_awlock   ),
            .m_axi_awcache  ( s_axi_spi_awcache  ),
            .m_axi_awprot   ( s_axi_spi_awprot   ),
            .m_axi_awregion ( s_axi_spi_awregion ),
            .m_axi_awqos    ( s_axi_spi_awqos    ),
            .m_axi_awvalid  ( s_axi_spi_awvalid  ),
            .m_axi_awready  ( s_axi_spi_awready  ),
            .m_axi_wdata    ( s_axi_spi_wdata    ),
            .m_axi_wstrb    ( s_axi_spi_wstrb    ),
            .m_axi_wlast    ( s_axi_spi_wlast    ),
            .m_axi_wvalid   ( s_axi_spi_wvalid   ),
            .m_axi_wready   ( s_axi_spi_wready   ),
            .m_axi_bresp    ( s_axi_spi_bresp    ),
            .m_axi_bvalid   ( s_axi_spi_bvalid   ),
            .m_axi_bready   ( s_axi_spi_bready   ),
            .m_axi_araddr   ( s_axi_spi_araddr   ),
            .m_axi_arlen    ( s_axi_spi_arlen    ),
            .m_axi_arsize   ( s_axi_spi_arsize   ),
            .m_axi_arburst  ( s_axi_spi_arburst  ),
            .m_axi_arlock   ( s_axi_spi_arlock   ),
            .m_axi_arcache  ( s_axi_spi_arcache  ),
            .m_axi_arprot   ( s_axi_spi_arprot   ),
            .m_axi_arregion ( s_axi_spi_arregion ),
            .m_axi_arqos    ( s_axi_spi_arqos    ),
            .m_axi_arvalid  ( s_axi_spi_arvalid  ),
            .m_axi_arready  ( s_axi_spi_arready  ),
            .m_axi_rdata    ( s_axi_spi_rdata    ),
            .m_axi_rresp    ( s_axi_spi_rresp    ),
            .m_axi_rlast    ( s_axi_spi_rlast    ),
            .m_axi_rvalid   ( s_axi_spi_rvalid   ),
            .m_axi_rready   ( s_axi_spi_rready   )
        );

        xlnx_axi_quad_spi i_xlnx_axi_quad_spi (
            .ext_spi_clk    ( clk_i                  ),
            .s_axi4_aclk    ( clk_i                  ),
            .s_axi4_aresetn ( rst_ni                 ),
            .s_axi4_awaddr  ( s_axi_spi_awaddr[23:0] ),
            .s_axi4_awlen   ( s_axi_spi_awlen        ),
            .s_axi4_awsize  ( s_axi_spi_awsize       ),
            .s_axi4_awburst ( s_axi_spi_awburst      ),
            .s_axi4_awlock  ( s_axi_spi_awlock       ),
            .s_axi4_awcache ( s_axi_spi_awcache      ),
            .s_axi4_awprot  ( s_axi_spi_awprot       ),
            .s_axi4_awvalid ( s_axi_spi_awvalid      ),
            .s_axi4_awready ( s_axi_spi_awready      ),
            .s_axi4_wdata   ( s_axi_spi_wdata        ),
            .s_axi4_wstrb   ( s_axi_spi_wstrb        ),
            .s_axi4_wlast   ( s_axi_spi_wlast        ),
            .s_axi4_wvalid  ( s_axi_spi_wvalid       ),
            .s_axi4_wready  ( s_axi_spi_wready       ),
            .s_axi4_bresp   ( s_axi_spi_bresp        ),
            .s_axi4_bvalid  ( s_axi_spi_bvalid       ),
            .s_axi4_bready  ( s_axi_spi_bready       ),
            .s_axi4_araddr  ( s_axi_spi_araddr[23:0] ),
            .s_axi4_arlen   ( s_axi_spi_arlen        ),
            .s_axi4_arsize  ( s_axi_spi_arsize       ),
            .s_axi4_arburst ( s_axi_spi_arburst      ),
            .s_axi4_arlock  ( s_axi_spi_arlock       ),
            .s_axi4_arcache ( s_axi_spi_arcache      ),
            .s_axi4_arprot  ( s_axi_spi_arprot       ),
            .s_axi4_arvalid ( s_axi_spi_arvalid      ),
            .s_axi4_arready ( s_axi_spi_arready      ),
            .s_axi4_rdata   ( s_axi_spi_rdata        ),
            .s_axi4_rresp   ( s_axi_spi_rresp        ),
            .s_axi4_rlast   ( s_axi_spi_rlast        ),
            .s_axi4_rvalid  ( s_axi_spi_rvalid       ),
            .s_axi4_rready  ( s_axi_spi_rready       ),
            .io0_i          ( '0                     ),
            .io0_o          ( spi_mosi               ),
            .io0_t          (                        ),
            .io1_i          ( spi_miso               ),
            .io1_o          (                        ),
            .io1_t          (                        ),
            .ss_i           ( '0                     ),
            .ss_o           ( spi_ss                 ),
            .ss_t           (                        ),
            .sck_o          ( spi_clk_o              ),
            .sck_i          ( '0                     ),
            .sck_t          (                        ),
            .ip2intc_irpt   ( irq_sources[1]         )
        );
    end else begin
        assign spi_clk_o = 1'b0;
        assign spi_mosi = 1'b0;
        assign spi_ss = 1'b0;

        // assign irq_sources [1] = 1'b0;
        assign spi.aw_ready = 1'b1;
        assign spi.ar_ready = 1'b1;
        assign spi.w_ready = 1'b1;

        assign spi.b_valid = spi.aw_valid;
        assign spi.b_id = spi.aw_id;
        assign spi.b_resp = axi_pkg::RESP_SLVERR;
        assign spi.b_user = '0;

        assign spi.r_valid = spi.ar_valid;
        assign spi.r_resp = axi_pkg::RESP_SLVERR;
        assign spi.r_data = 'hdeadbeef;
        assign spi.r_last = 1'b1;
    end


    // ---------------
    // 4. Ethernet
    // ---------------

    if (InclEthernet) begin : gen_ethernet

    logic                    clk_200_int, clk_rgmii, clk_rgmii_quad;
    logic                    eth_en, eth_we, eth_int_n, eth_pme_n, eth_mdio_i, eth_mdio_o, eth_mdio_oe;
    logic [AxiAddrWidth-1:0] eth_addr;
    logic [AxiDataWidth-1:0] eth_wrdata, eth_rdata;
    logic [AxiDataWidth/8-1:0] eth_be;

    axi2mem #(
        .AXI_ID_WIDTH   ( AxiIdWidth       ),
        .AXI_ADDR_WIDTH ( AxiAddrWidth     ),
        .AXI_DATA_WIDTH ( AxiDataWidth     ),
        .AXI_USER_WIDTH ( AxiUserWidth     )
    ) i_axi2rom (
        .clk_i  ( clk_i                   ),
        .rst_ni ( rst_ni                  ),
        .slave  ( ethernet                ),
        .req_o  ( eth_en                  ),
        .we_o   ( eth_we                  ),
        .addr_o ( eth_addr                ),
        .be_o   ( eth_be                  ),
        .data_o ( eth_wrdata              ),
        .data_i ( eth_rdata               )
    );

    framing_top eth_rgmii (
       .msoc_clk(clk_i),
       .core_lsu_addr(eth_addr[14:0]),
       .core_lsu_wdata(eth_wrdata),
       .core_lsu_be(eth_be),
       .ce_d(eth_en),
       .we_d(eth_en & eth_we),
       .framing_sel(eth_en),
       .framing_rdata(eth_rdata),
       .rst_int(!rst_ni),
       .clk_int(phy_tx_clk_i), // 125 MHz in-phase
       .clk90_int(eth_clk_i),    // 125 MHz quadrature
       .clk_200_int(clk_200MHz_i),
       /*
        * Ethernet: 1000BASE-T RGMII
        */
       .phy_rx_clk(eth_rxck),
       .phy_rxd(eth_rxd),
       .phy_rx_ctl(eth_rxctl),
       .phy_tx_clk(eth_txck),
       .phy_txd(eth_txd),
       .phy_tx_ctl(eth_txctl),
       .phy_reset_n(eth_rst_n),
       .phy_int_n(eth_int_n),
       .phy_pme_n(eth_pme_n),
       .phy_mdc(eth_mdc),
       .phy_mdio_i(eth_mdio_i),
       .phy_mdio_o(eth_mdio_o),
       .phy_mdio_oe(eth_mdio_oe),
       .eth_irq(irq_sources[2])
    );

       IOBUF #(
          .DRIVE(12), // Specify the output drive strength
          .IBUF_LOW_PWR("TRUE"),  // Low Power - "TRUE", High Performance = "FALSE"
          .IOSTANDARD("DEFAULT"), // Specify the I/O standard
          .SLEW("SLOW") // Specify the output slew rate
       ) IOBUF_inst (
          .O(eth_mdio_i),     // Buffer output
          .IO(eth_mdio),   // Buffer inout port (connect directly to top-level port)
          .I(eth_mdio_o),     // Buffer input
          .T(~eth_mdio_oe)      // 3-state enable input, high=input, low=output
       );

    end else begin
        assign irq_sources [2] = 1'b0;
        assign ethernet.aw_ready = 1'b1;
        assign ethernet.ar_ready = 1'b1;
        assign ethernet.w_ready = 1'b1;

        assign ethernet.b_valid = ethernet.aw_valid;
        assign ethernet.b_id = ethernet.aw_id;
        assign ethernet.b_resp = axi_pkg::RESP_SLVERR;
        assign ethernet.b_user = '0;

        assign ethernet.r_valid = ethernet.ar_valid;
        assign ethernet.r_resp = axi_pkg::RESP_SLVERR;
        assign ethernet.r_data = 'hdeadbeef;
        assign ethernet.r_last = 1'b1;
    end

    // 5. GPIO
    assign gpio.b_user = 1'b0;
    assign gpio.r_user = 1'b0;

    if (InclGPIO) begin : gen_gpio

        logic [31:0] s_axi_gpio_awaddr;
        logic [7:0]  s_axi_gpio_awlen;
        logic [2:0]  s_axi_gpio_awsize;
        logic [1:0]  s_axi_gpio_awburst;
        logic [3:0]  s_axi_gpio_awcache;
        logic        s_axi_gpio_awvalid;
        logic        s_axi_gpio_awready;
        logic [31:0] s_axi_gpio_wdata;
        logic [3:0]  s_axi_gpio_wstrb;
        logic        s_axi_gpio_wvalid;
        logic        s_axi_gpio_wready;
        logic [1:0]  s_axi_gpio_bresp;
        logic        s_axi_gpio_bvalid;
        logic        s_axi_gpio_bready;
        logic [31:0] s_axi_gpio_araddr;
        logic [7:0]  s_axi_gpio_arlen;
        logic [2:0]  s_axi_gpio_arsize;
        logic [1:0]  s_axi_gpio_arburst;
        logic        s_axi_gpio_arvalid;
        logic        s_axi_gpio_arready;
        logic [31:0] s_axi_gpio_rdata;
        logic [1:0]  s_axi_gpio_rresp;
        logic        s_axi_gpio_rlast;
        logic        s_axi_gpio_rvalid;
        logic        s_axi_gpio_rready;

        // system-bus is 64-bit, convert down to 32 bit
        xlnx_axi_dwidth_converter i_xlnx_axi_dwidth_converter_gpio (
            .s_axi_aclk     ( clk_i              ),
            .s_axi_aresetn  ( rst_ni             ),
            .s_axi_awid     ( gpio.aw_id         ),
            .s_axi_awaddr   ( gpio.aw_addr[31:0] ),
            .s_axi_awlen    ( gpio.aw_len        ),
            .s_axi_awsize   ( gpio.aw_size       ),
            .s_axi_awburst  ( gpio.aw_burst      ),
            .s_axi_awlock   ( gpio.aw_lock       ),
            .s_axi_awcache  ( gpio.aw_cache      ),
            .s_axi_awprot   ( gpio.aw_prot       ),
            .s_axi_awregion ( gpio.aw_region     ),
            .s_axi_awqos    ( gpio.aw_qos        ),
            .s_axi_awvalid  ( gpio.aw_valid      ),
            .s_axi_awready  ( gpio.aw_ready      ),
            .s_axi_wdata    ( gpio.w_data        ),
            .s_axi_wstrb    ( gpio.w_strb        ),
            .s_axi_wlast    ( gpio.w_last        ),
            .s_axi_wvalid   ( gpio.w_valid       ),
            .s_axi_wready   ( gpio.w_ready       ),
            .s_axi_bid      ( gpio.b_id          ),
            .s_axi_bresp    ( gpio.b_resp        ),
            .s_axi_bvalid   ( gpio.b_valid       ),
            .s_axi_bready   ( gpio.b_ready       ),
            .s_axi_arid     ( gpio.ar_id         ),
            .s_axi_araddr   ( gpio.ar_addr[31:0] ),
            .s_axi_arlen    ( gpio.ar_len        ),
            .s_axi_arsize   ( gpio.ar_size       ),
            .s_axi_arburst  ( gpio.ar_burst      ),
            .s_axi_arlock   ( gpio.ar_lock       ),
            .s_axi_arcache  ( gpio.ar_cache      ),
            .s_axi_arprot   ( gpio.ar_prot       ),
            .s_axi_arregion ( gpio.ar_region     ),
            .s_axi_arqos    ( gpio.ar_qos        ),
            .s_axi_arvalid  ( gpio.ar_valid      ),
            .s_axi_arready  ( gpio.ar_ready      ),
            .s_axi_rid      ( gpio.r_id          ),
            .s_axi_rdata    ( gpio.r_data        ),
            .s_axi_rresp    ( gpio.r_resp        ),
            .s_axi_rlast    ( gpio.r_last        ),
            .s_axi_rvalid   ( gpio.r_valid       ),
            .s_axi_rready   ( gpio.r_ready       ),

            .m_axi_awaddr   ( s_axi_gpio_awaddr  ),
            .m_axi_awlen    ( s_axi_gpio_awlen   ),
            .m_axi_awsize   ( s_axi_gpio_awsize  ),
            .m_axi_awburst  ( s_axi_gpio_awburst ),
            .m_axi_awlock   (                    ),
            .m_axi_awcache  ( s_axi_gpio_awcache ),
            .m_axi_awprot   (                    ),
            .m_axi_awregion (                    ),
            .m_axi_awqos    (                    ),
            .m_axi_awvalid  ( s_axi_gpio_awvalid ),
            .m_axi_awready  ( s_axi_gpio_awready ),
            .m_axi_wdata    ( s_axi_gpio_wdata   ),
            .m_axi_wstrb    ( s_axi_gpio_wstrb   ),
            .m_axi_wlast    (                    ),
            .m_axi_wvalid   ( s_axi_gpio_wvalid  ),
            .m_axi_wready   ( s_axi_gpio_wready  ),
            .m_axi_bresp    ( s_axi_gpio_bresp   ),
            .m_axi_bvalid   ( s_axi_gpio_bvalid  ),
            .m_axi_bready   ( s_axi_gpio_bready  ),
            .m_axi_araddr   ( s_axi_gpio_araddr  ),
            .m_axi_arlen    ( s_axi_gpio_arlen   ),
            .m_axi_arsize   ( s_axi_gpio_arsize  ),
            .m_axi_arburst  ( s_axi_gpio_arburst ),
            .m_axi_arlock   (                    ),
            .m_axi_arcache  ( s_axi_gpio_arcache ),
            .m_axi_arprot   (                    ),
            .m_axi_arregion (                    ),
            .m_axi_arqos    (                    ),
            .m_axi_arvalid  ( s_axi_gpio_arvalid ),
            .m_axi_arready  ( s_axi_gpio_arready ),
            .m_axi_rdata    ( s_axi_gpio_rdata   ),
            .m_axi_rresp    ( s_axi_gpio_rresp   ),
            .m_axi_rlast    ( s_axi_gpio_rlast   ),
            .m_axi_rvalid   ( s_axi_gpio_rvalid  ),
            .m_axi_rready   ( s_axi_gpio_rready  )
        );

        xlnx_axi_gpio i_xlnx_axi_gpio (
            .s_axi_aclk    ( clk_i                  ),
            .s_axi_aresetn ( rst_ni                 ),
            .s_axi_awaddr  ( s_axi_gpio_awaddr[8:0] ),
            .s_axi_awvalid ( s_axi_gpio_awvalid     ),
            .s_axi_awready ( s_axi_gpio_awready     ),
            .s_axi_wdata   ( s_axi_gpio_wdata       ),
            .s_axi_wstrb   ( s_axi_gpio_wstrb       ),
            .s_axi_wvalid  ( s_axi_gpio_wvalid      ),
            .s_axi_wready  ( s_axi_gpio_wready      ),
            .s_axi_bresp   ( s_axi_gpio_bresp       ),
            .s_axi_bvalid  ( s_axi_gpio_bvalid      ),
            .s_axi_bready  ( s_axi_gpio_bready      ),
            .s_axi_araddr  ( s_axi_gpio_araddr[8:0] ),
            .s_axi_arvalid ( s_axi_gpio_arvalid     ),
            .s_axi_arready ( s_axi_gpio_arready     ),
            .s_axi_rdata   ( s_axi_gpio_rdata       ),
            .s_axi_rresp   ( s_axi_gpio_rresp       ),
            .s_axi_rvalid  ( s_axi_gpio_rvalid      ),
            .s_axi_rready  ( s_axi_gpio_rready      ),
            .gpio_io_i     ( '0                     ),
            .gpio_io_o     ( leds_o                 ),
            .gpio_io_t     (                        ),
            .gpio2_io_i    ( dip_switches_i         )
        );

        assign s_axi_gpio_rlast = 1'b1;

    end

    // 6. Timer
    if (InclTimer) begin : gen_timer
        logic         timer_penable;
        logic         timer_pwrite;
        logic [31:0]  timer_paddr;
        logic         timer_psel;
        logic [31:0]  timer_pwdata;
        logic [31:0]  timer_prdata;
        logic         timer_pready;
        logic         timer_pslverr;

        axi2apb_64_32 #(
            .AXI4_ADDRESS_WIDTH ( AxiAddrWidth ),
            .AXI4_RDATA_WIDTH   ( AxiDataWidth ),
            .AXI4_WDATA_WIDTH   ( AxiDataWidth ),
            .AXI4_ID_WIDTH      ( AxiIdWidth   ),
            .AXI4_USER_WIDTH    ( AxiUserWidth ),
            .BUFF_DEPTH_SLAVE   ( 2            ),
            .APB_ADDR_WIDTH     ( 32           )
        ) i_axi2apb_64_32_timer (
            .ACLK      ( clk_i           ),
            .ARESETn   ( rst_ni          ),
            .test_en_i ( 1'b0            ),
            .AWID_i    ( timer.aw_id     ),
            .AWADDR_i  ( timer.aw_addr   ),
            .AWLEN_i   ( timer.aw_len    ),
            .AWSIZE_i  ( timer.aw_size   ),
            .AWBURST_i ( timer.aw_burst  ),
            .AWLOCK_i  ( timer.aw_lock   ),
            .AWCACHE_i ( timer.aw_cache  ),
            .AWPROT_i  ( timer.aw_prot   ),
            .AWREGION_i( timer.aw_region ),
            .AWUSER_i  ( timer.aw_user   ),
            .AWQOS_i   ( timer.aw_qos    ),
            .AWVALID_i ( timer.aw_valid  ),
            .AWREADY_o ( timer.aw_ready  ),
            .WDATA_i   ( timer.w_data    ),
            .WSTRB_i   ( timer.w_strb    ),
            .WLAST_i   ( timer.w_last    ),
            .WUSER_i   ( timer.w_user    ),
            .WVALID_i  ( timer.w_valid   ),
            .WREADY_o  ( timer.w_ready   ),
            .BID_o     ( timer.b_id      ),
            .BRESP_o   ( timer.b_resp    ),
            .BVALID_o  ( timer.b_valid   ),
            .BUSER_o   ( timer.b_user    ),
            .BREADY_i  ( timer.b_ready   ),
            .ARID_i    ( timer.ar_id     ),
            .ARADDR_i  ( timer.ar_addr   ),
            .ARLEN_i   ( timer.ar_len    ),
            .ARSIZE_i  ( timer.ar_size   ),
            .ARBURST_i ( timer.ar_burst  ),
            .ARLOCK_i  ( timer.ar_lock   ),
            .ARCACHE_i ( timer.ar_cache  ),
            .ARPROT_i  ( timer.ar_prot   ),
            .ARREGION_i( timer.ar_region ),
            .ARUSER_i  ( timer.ar_user   ),
            .ARQOS_i   ( timer.ar_qos    ),
            .ARVALID_i ( timer.ar_valid  ),
            .ARREADY_o ( timer.ar_ready  ),
            .RID_o     ( timer.r_id      ),
            .RDATA_o   ( timer.r_data    ),
            .RRESP_o   ( timer.r_resp    ),
            .RLAST_o   ( timer.r_last    ),
            .RUSER_o   ( timer.r_user    ),
            .RVALID_o  ( timer.r_valid   ),
            .RREADY_i  ( timer.r_ready   ),
            .PENABLE   ( timer_penable   ),
            .PWRITE    ( timer_pwrite    ),
            .PADDR     ( timer_paddr     ),
            .PSEL      ( timer_psel      ),
            .PWDATA    ( timer_pwdata    ),
            .PRDATA    ( timer_prdata    ),
            .PREADY    ( timer_pready    ),
            .PSLVERR   ( timer_pslverr   )
        );

        apb_timer #(
                .APB_ADDR_WIDTH ( 32 ),
                .TIMER_CNT      ( 2  )
        ) i_timer (
            .HCLK    ( clk_i            ),
            .HRESETn ( rst_ni           ),
            .PSEL    ( timer_psel       ),
            .PENABLE ( timer_penable    ),
            .PWRITE  ( timer_pwrite     ),
            .PADDR   ( timer_paddr      ),
            .PWDATA  ( timer_pwdata     ),
            .PRDATA  ( timer_prdata     ),
            .PREADY  ( timer_pready     ),
            .PSLVERR ( timer_pslverr    ),
            .irq_o   ( irq_sources[6:3] )
        );
    end

    //-------------------------------------
    // Direct Memory Access Engine & IOMMU
    //-------------------------------------
    /*
        Two peripheral master interfaces (XBAR slave ports)
        - IOMMU Data Structures IF for implicit accesses
        - IOMMU Completion IF to forward allowed requests
        Two peripheral slave interfaces (XBAR master ports)
        - IOMMU Programming IF for IOMMU configuration
        - iDMA Programming IF for iDMA configuration
    */

    typedef logic [AxiAddrWidth-1:0]                iommu_addr_t;
    typedef logic [AxiDataWidth-1:0]                iommu_data_t;
    typedef logic [(AxiDataWidth/8)-1:0]            iommu_strb_t;
    typedef logic [AxiUserWidth-1:0]                iommu_user_t;
    typedef logic [ariane_soc::IdWidth-1:0]         iommu_id_t;
    typedef logic [ariane_soc::IdWidthSlave-1:0]    iommu_slv_id_t;

    localparam int unsigned NumDMADevices = 4;
    localparam int unsigned DmaAxiIdWidth =
        ariane_soc::IdWidth - $clog2(NumDMADevices);

    // AXI Bus between DMA devices and IOMMU TR IF
    // Extended with AXI support for untranslated transactions
    `AXI_TYPEDEF_EXT_ALL(axi_tr, iommu_addr_t, iommu_id_t,
                            iommu_data_t, iommu_strb_t, iommu_user_t,
                            rv_iommu::device_id_t, logic, rv_iommu::process_id_t)
    axi_tr_req_t axi_iommu_tr_req;
    axi_tr_resp_t axi_iommu_tr_resp;
    `AXI_TYPEDEF_ALL(axi_comp, iommu_addr_t, iommu_id_t,
                            iommu_data_t, iommu_strb_t, iommu_user_t)
    axi_comp_req_t  axi_iommu_comp_req;
    axi_comp_resp_t axi_iommu_comp_resp;
    rv_iommu::e3_ctl_t e3_ctl;
    rv_iommu::e3_tl_t e3_tl;
    logic [7:0] e3_control, e3_index;
    logic [63:0] e3_data;
    logic e3_error;
    if (!InclDMA) begin : gen_no_e3_control
        assign e3_control = 0;
        assign e3_index = 0;
    end

    typedef logic [DmaAxiIdWidth-1:0] dma_iommu_id_t;
    `AXI_TYPEDEF_EXT_ALL(axi_dma_tr, iommu_addr_t, dma_iommu_id_t,
                            iommu_data_t, iommu_strb_t, iommu_user_t,
                            rv_iommu::device_id_t, logic, rv_iommu::process_id_t)
    axi_dma_tr_req_t  [NumDMADevices-1:0] axi_dma_tr_req;
    axi_dma_tr_resp_t [NumDMADevices-1:0] axi_dma_tr_resp;

    // AXI Bus between System Interconnect and IOMMU Programming IF
    `AXI_TYPEDEF_ALL(axi_prog, iommu_addr_t, iommu_slv_id_t,
                     iommu_data_t, iommu_strb_t, iommu_user_t)
    axi_prog_req_t axi_iommu_prog_req;
    axi_prog_resp_t axi_iommu_prog_resp;
    `AXI_ASSIGN_TO_REQ(axi_iommu_prog_req, iommu_prog)
    `AXI_ASSIGN_FROM_RESP(iommu_prog, axi_iommu_prog_resp)

    //------------
    // DMA Engine
    //------------
    if (InclDMA) begin : gen_dma
        logic [NumDMADevices-1:0] obs_issue, obs_done, obs_poll, batch_active;
        logic [63:0] obs_issue_id [NumDMADevices], obs_done_id [NumDMADevices], obs_poll_id [NumDMADevices];
        logic [NumDMADevices-1:0] obs_cfg_valid, obs_cfg_write;
        logic [7:0] obs_cfg_addr [NumDMADevices];
        logic [63:0] obs_cfg_wdata [NumDMADevices], legacy_cfg_rdata, launch_cfg_rdata, obs_cfg_rdata;
        logic [7:0] obs_cfg_wstrb [NumDMADevices];
        logic [NumDMADevices-1:0] launch_hold, launch_go, launch_cancel;
        logic [NumDMADevices-1:0] launch_held, launch_active, launch_cancelled, launch_error;
        logic [NumDMADevices-1:0] obs_arvalid, obs_awvalid, obs_arready, obs_awready;
        logic [NumDMADevices-1:0] obs_rlast, obs_b, obs_bad_id;
        for (genvar o = 0; o < NumDMADevices; o++) begin : gen_dma_observation
            assign obs_arvalid[o] = axi_dma_tr_req[o].ar_valid;
            assign obs_awvalid[o] = axi_dma_tr_req[o].aw_valid;
            assign obs_arready[o] = axi_dma_tr_resp[o].ar_ready;
            assign obs_awready[o] = axi_dma_tr_resp[o].aw_ready;
            assign obs_rlast[o] = axi_dma_tr_resp[o].r_valid && axi_dma_tr_resp[o].r.last && axi_dma_tr_req[o].r_ready;
            assign obs_b[o] = axi_dma_tr_resp[o].b_valid && axi_dma_tr_req[o].b_ready;
            // Each current iDMA emits AxID=0. Monitor its private mux-side ID.
            assign obs_bad_id[o] = (obs_arvalid[o] && |axi_dma_tr_req[o].ar.id) ||
                (obs_awvalid[o] && |axi_dma_tr_req[o].aw.id) ||
                (axi_dma_tr_resp[o].r_valid && |axi_dma_tr_resp[o].r.id) ||
                (axi_dma_tr_resp[o].b_valid && |axi_dma_tr_resp[o].b.id);
        end
        dma_observer #(.NumPorts(NumDMADevices), .PathEnable(InclIOMMU),
                       .E3Enable(InclIOMMU), .ControlledEnable(InclIOMMU),
                       .AxiIdWidth(ariane_soc::IdWidth)) i_dma_observer (
            .clk_i, .rst_ni,
            .cfg_valid_i(obs_cfg_valid[0] && !obs_cfg_addr[0][7]),
            .cfg_write_i(obs_cfg_write[0]),
            .cfg_addr_i(obs_cfg_addr[0][6:0]), .cfg_wdata_i(obs_cfg_wdata[0]),
            .cfg_wstrb_i(obs_cfg_wstrb[0]), .cfg_rdata_o(legacy_cfg_rdata),
            .e3_data_i(e3_data), .e3_error_i(e3_error), .e3_control_o(e3_control), .e3_index_o(e3_index),
            .issue_i(obs_issue), .done_i(obs_done), .poll_i(obs_poll),
            .batch_active_i(batch_active),
            .issue_id_i(obs_issue_id), .done_id_i(obs_done_id), .poll_id_i(obs_poll_id),
            .ar_valid_i(obs_arvalid), .aw_valid_i(obs_awvalid),
            .ar_ready_i(obs_arready), .aw_ready_i(obs_awready),
            .rlast_fire_i(obs_rlast), .b_fire_i(obs_b), .bad_id_i(obs_bad_id),
            .shared_ar_valid_i(axi_iommu_tr_req.ar_valid),
            .shared_aw_valid_i(axi_iommu_tr_req.aw_valid),
            .shared_ar_ready_i(axi_iommu_tr_resp.ar_ready),
            .shared_aw_ready_i(axi_iommu_tr_resp.aw_ready),
            .tr_id_i('{axi_iommu_tr_req.ar.id, axi_iommu_tr_req.aw.id}),
            .out_id_i('{axi_iommu_comp_req.ar.id, axi_iommu_comp_req.aw.id}),
            .return_id_i('{axi_iommu_comp_resp.r.id, axi_iommu_comp_resp.b.id}),
            .out_valid_i({axi_iommu_comp_req.aw_valid, axi_iommu_comp_req.ar_valid}),
            .out_ready_i({axi_iommu_comp_resp.aw_ready, axi_iommu_comp_resp.ar_ready}),
            .return_i({axi_iommu_comp_resp.b_valid && axi_iommu_comp_req.b_ready,
                       axi_iommu_comp_resp.r_valid && axi_iommu_comp_resp.r.last && axi_iommu_comp_req.r_ready})
        );

        assign obs_cfg_rdata = obs_cfg_addr[0][7] ? launch_cfg_rdata : legacy_cfg_rdata;
        if (InclIOMMU) begin : gen_launcher
            dma_controlled_launcher #(.NumPorts(NumDMADevices)) i_launcher (
                .clk_i, .rst_ni,
                .cfg_valid_i(obs_cfg_valid[0] && obs_cfg_addr[0][7]),
                .cfg_write_i(obs_cfg_write[0]),
                .cfg_addr_i(obs_cfg_addr[0]),
                .cfg_wdata_i(obs_cfg_wdata[0]),
                .cfg_wstrb_i(obs_cfg_wstrb[0]),
                .cfg_rdata_o(launch_cfg_rdata),
                .frontend_held_i(launch_held),
                .frontend_active_i(launch_active),
                .frontend_cancelled_i(launch_cancelled),
                .frontend_error_i(launch_error),
                .obs_issue_i(obs_issue), .obs_done_i(obs_done),
                .obs_issue_id_i(obs_issue_id), .obs_done_id_i(obs_done_id),
                .selected_i(e3_ctl.select_valid),
                .select_write_i(e3_ctl.select_write),
                .select_id_i(e3_ctl.select_id),
                .select_did_i(e3_ctl.select_did),
                .select_iova_i(e3_ctl.select_iova),
                .launch_hold_o(launch_hold),
                .launch_go_o(launch_go),
                .launch_cancel_o(launch_cancel)
            );
        end else begin : gen_no_launcher
            assign launch_hold = '0;
            assign launch_go = '0;
            assign launch_cancel = '0;
            assign launch_cfg_rdata = '0;
        end

        dma_e3_observer i_e3_observer (
            .clk_i,.rst_ni,.arm_i(e3_control[1]),.clear_i(e3_control[2]),
            .active_i(e3_control[3]),.open_i(e3_control[4]),.close_i(e3_control[5]),
            .abort_i(e3_control[6]),.delay128_i(e3_control[7]),
            .ctl_i(e3_ctl),.tl_i(e3_tl),.index_i(e3_index),.data_o(e3_data),.error_o(e3_error),
            .tr_fire_i({axi_iommu_tr_req.aw_valid && axi_iommu_tr_resp.aw_ready,
                        axi_iommu_tr_req.ar_valid && axi_iommu_tr_resp.ar_ready}),
            .tr_id_i('{axi_iommu_tr_req.ar.id,axi_iommu_tr_req.aw.id}),
            .tr_address_i('{64'(axi_iommu_tr_req.ar.addr),64'(axi_iommu_tr_req.aw.addr)}),
            .out_valid_i({axi_iommu_comp_req.aw_valid,axi_iommu_comp_req.ar_valid}),
            .out_ready_i({axi_iommu_comp_resp.aw_ready,axi_iommu_comp_resp.ar_ready}),
            .out_id_i('{axi_iommu_comp_req.ar.id,axi_iommu_comp_req.aw.id}),
            .return_i({axi_iommu_comp_resp.b_valid && axi_iommu_comp_req.b_ready,
                       axi_iommu_comp_resp.r_valid && axi_iommu_comp_resp.r.last && axi_iommu_comp_req.r_ready}),
            .return_id_i('{axi_iommu_comp_resp.r.id,axi_iommu_comp_resp.b.id}),
            .wlast_i(axi_iommu_comp_req.w_valid && axi_iommu_comp_req.w.last && axi_iommu_comp_resp.w_ready)
        );

        AXI_BUS #(
            .AXI_ADDR_WIDTH ( AxiAddrWidth             ),
            .AXI_DATA_WIDTH ( AxiDataWidth             ),
            .AXI_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
            .AXI_USER_WIDTH ( AxiUserWidth             )
        ) dma_cfg_bus [NumDMADevices-1:0]();

        axi_demux_intf #(
            .AXI_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
            .ATOP_SUPPORT   ( 1'b0                     ),
            .AXI_ADDR_WIDTH ( AxiAddrWidth             ),
            .AXI_DATA_WIDTH ( AxiDataWidth             ),
            .AXI_USER_WIDTH ( AxiUserWidth             ),
            .NO_MST_PORTS   ( NumDMADevices            ),
            .MAX_TRANS      ( 8                        )
        ) i_dma_cfg_demux (
            .clk_i,
            .rst_ni,
            .test_i           ( 1'b0                ),
            .slv_aw_select_i  ( dma_cfg.aw_addr[13:12] ),
            .slv_ar_select_i  ( dma_cfg.ar_addr[13:12] ),
            .slv              ( dma_cfg             ),
            .mst              ( dma_cfg_bus         )
        );

        // AXI Bus between iDMAs and IOMMU TR IF
        // Extended with IOMMU AXI signals
        AXI_BUS_EXT #(
            .AXI_ADDR_WIDTH ( AxiAddrWidth          ),
            .AXI_DATA_WIDTH ( AxiDataWidth          ),
            .AXI_ID_WIDTH   ( DmaAxiIdWidth         ),
            .AXI_USER_WIDTH ( AxiUserWidth          ),
            .AXI_SID_WIDTH  ( rv_iommu::DevIdWidth  ),
            .AXI_SSID_WIDTH ( rv_iommu::ProcIdWidth )
        ) idma_axi_master [NumDMADevices-1:0]();

        for (genvar i = 0; i < NumDMADevices; i++) begin : gen_idma_axi_assign
            `AXI_ASSIGN_TO_REQ_EXT(axi_dma_tr_req[i], idma_axi_master[i])
            `AXI_ASSIGN_FROM_RESP(idma_axi_master[i], axi_dma_tr_resp[i])
        end

        axi_mux #(
            .SlvAxiIDWidth ( DmaAxiIdWidth        ),
            .slv_aw_chan_t ( axi_dma_tr_aw_chan_t ),
            .mst_aw_chan_t ( axi_tr_aw_chan_t     ),
            .w_chan_t      ( axi_dma_tr_w_chan_t  ),
            .slv_b_chan_t  ( axi_dma_tr_b_chan_t  ),
            .mst_b_chan_t  ( axi_tr_b_chan_t      ),
            .slv_ar_chan_t ( axi_dma_tr_ar_chan_t ),
            .mst_ar_chan_t ( axi_tr_ar_chan_t     ),
            .slv_r_chan_t  ( axi_dma_tr_r_chan_t  ),
            .mst_r_chan_t  ( axi_tr_r_chan_t      ),
            .slv_req_t     ( axi_dma_tr_req_t     ),
            .slv_resp_t    ( axi_dma_tr_resp_t    ),
            .mst_req_t     ( axi_tr_req_t         ),
            .mst_resp_t    ( axi_tr_resp_t        ),
            .NoSlvPorts    ( NumDMADevices        ),
            .MaxWTrans     ( 8                    )
        ) i_dma_iommu_tr_mux (
            .clk_i,
            .rst_ni,
            .test_i      ( 1'b0              ),
            .slv_reqs_i  ( axi_dma_tr_req    ),
            .slv_resps_o ( axi_dma_tr_resp   ),
            .mst_req_o   ( axi_iommu_tr_req  ),
            .mst_resp_i  ( axi_iommu_tr_resp )
        );

        for (genvar i = 0; i < NumDMADevices; i++) begin : gen_dma_inst
            dma_core_wrap_intf #(
                .AXI_ADDR_WIDTH     ( AxiAddrWidth             ),
                .AXI_DATA_WIDTH     ( AxiDataWidth             ),
                .AXI_ID_WIDTH       ( DmaAxiIdWidth            ),
                .AXI_SLV_ID_WIDTH   ( ariane_soc::IdWidthSlave ),
                .AXI_USER_WIDTH     ( AxiUserWidth             ),
                .JOB_FIFO_DEPTH     ( 8                        ),
                .NUM_AX_IN_FLIGHT   ( 8                        ),
                .MEM_SYS_DEPTH      ( 0                        ),
                .RAW_COUPLING_AVAIL ( 0                        ),
                .IS_TWO_D           ( 0                        ),
                .BATCH_PORT (1'b1),
                .OBSERVER_PORT      ( i == 0                   ),
                .LAUNCH_PORT        ( InclIOMMU && i < 2       ),

                .DEVICE_ID          ( 24'(10 + i)              ),
                .PROCESS_ID         ( 20'(20 + i)              ),
                .AxID               ( '0                       )
            ) i_dma (
                .obs_issue_o(obs_issue[i]), .obs_done_o(obs_done[i]), .obs_poll_o(obs_poll[i]),
                .batch_active_o(batch_active[i]),
                .obs_issue_id_o(obs_issue_id[i]), .obs_done_id_o(obs_done_id[i]), .obs_poll_id_o(obs_poll_id[i]),
                .obs_cfg_valid_o(obs_cfg_valid[i]), .obs_cfg_write_o(obs_cfg_write[i]),
                .obs_cfg_addr_o(obs_cfg_addr[i]), .obs_cfg_wdata_o(obs_cfg_wdata[i]),
                .obs_cfg_wstrb_o(obs_cfg_wstrb[i]), .obs_cfg_rdata_i(obs_cfg_rdata),
                .launch_hold_i(launch_hold[i]),
                .launch_go_i(launch_go[i]),
                .launch_cancel_i(launch_cancel[i]),
                .launch_held_o(launch_held[i]),
                .launch_active_o(launch_active[i]),
                .launch_cancelled_o(launch_cancelled[i]),
                .launch_error_o(launch_error[i]),
                .clk_i          ( clk_i              ),
                .rst_ni         ( rst_ni             ),
                .testmode_i     ( 1'b0               ),
                // slave port
                .axi_slave      ( dma_cfg_bus[i]     ),
                // master port
                .axi_master     ( idma_axi_master[i] )
            );
        end
    end

	//--------------
    // No DMA Engine
    //--------------
    //
    // When no DMA engine is included, TR AXI Bus request xVALID/xREADY wires are set to zero
    // AXI transactions directed to the DMA config port are responded with error.
    else begin : gen_dma_disabled

		// AXI Bus between System Interconnect (Mst) and iDMA Configuration Port (Slv)
		ariane_axi_soc::req_slv_t axi_dma_cfg_req;
		ariane_axi_soc::resp_slv_t axi_dma_cfg_resp;
		`AXI_ASSIGN_TO_REQ(axi_dma_cfg_req, dma_cfg)
		`AXI_ASSIGN_FROM_RESP(dma_cfg, axi_dma_cfg_resp)

        axi_err_slv #(
			.AxiIdWidth ( ariane_soc::IdWidthSlave   ),
			.axi_req_t  ( ariane_axi_soc::req_slv_t  ),
			.axi_resp_t ( ariane_axi_soc::resp_slv_t )
        ) i_idma_err_slv (
			.clk_i      ( clk_i    				),
			.rst_ni     ( rst_ni   				),
			.slv_req_i  ( axi_dma_cfg_req       ),
			.slv_resp_o ( axi_dma_cfg_resp      ),
			.test_i     ( 1'b0     				)
        );

		// Set TR IF request wires to a known state
		assign axi_iommu_tr_req.ar_valid = 1'b0;
        assign axi_iommu_tr_req.aw_valid = 1'b0;
        assign axi_iommu_tr_req.w_valid  = 1'b0;
        assign axi_iommu_tr_req.b_ready  = 1'b1;
        assign axi_iommu_tr_req.r_ready  = 1'b1;
    end

    //--------------------------------------------
    // RISC-V Input/Output Memory Management Unit
    //--------------------------------------------
    if (InclIOMMU) begin : gen_iommu

        // AXI Bus between IOMMU Memory IF and System Interconnect
        `AXI_TYPEDEF_ALL(axi_ds, iommu_addr_t, iommu_id_t,
                            iommu_data_t, iommu_strb_t, iommu_user_t)
        axi_ds_req_t  axi_iommu_ds_req;
        axi_ds_resp_t axi_iommu_ds_resp;
        axi_ds_req_t  axi_iommu_ds_delayed_req;
        axi_ds_resp_t axi_iommu_ds_delayed_resp;

        axi_delayer #(
            .aw_chan_t          ( axi_ds_aw_chan_t          ),
            .w_chan_t           ( axi_ds_w_chan_t           ),
            .b_chan_t           ( axi_ds_b_chan_t           ),
            .ar_chan_t          ( axi_ds_ar_chan_t          ),
            .r_chan_t           ( axi_ds_r_chan_t           ),
            .req_t              ( axi_ds_req_t              ),
            .resp_t             ( axi_ds_resp_t             ),
            .StallRandomInput   ( 1'b0                      ),
            .StallRandomOutput  ( 1'b0                      ),
            .FixedDelayInput    ( IOMMUDSFixedDelayInput    ),
            .FixedDelayOutput   ( IOMMUDSFixedDelayOutput   )
        ) i_iommu_ds_delayer (
            .clk_i      ( clk_i                     ),
            .rst_ni     ( rst_ni                    ),
            .slv_req_i  ( axi_iommu_ds_req          ),
            .slv_resp_o ( axi_iommu_ds_resp         ),
            .mst_req_o  ( axi_iommu_ds_delayed_req  ),
            .mst_resp_i ( axi_iommu_ds_delayed_resp )
        );

        `AXI_ASSIGN_FROM_REQ(iommu_ds, axi_iommu_ds_delayed_req)
        `AXI_ASSIGN_TO_RESP(axi_iommu_ds_delayed_resp, iommu_ds)

        // AXI Bus between IOMMU Completion IF and System Interconnect
        `AXI_ASSIGN_FROM_REQ(iommu_comp, axi_iommu_comp_req)
        `AXI_ASSIGN_TO_RESP(axi_iommu_comp_resp, iommu_comp)

        //--------------
        // RISC-V IOMMU
        //--------------
        rv_iommu_top #(
            .RVIOMMUCfg   (ariane_soc::RVIOMMUCfg),
            .E3Enable(InclDMA),

            .axi_tr_aw_chan_t       (axi_tr_aw_chan_t),
            .axi_tr_w_chan_t        (axi_tr_w_chan_t),
            .axi_tr_b_chan_t        (axi_tr_b_chan_t),
            .axi_tr_ar_chan_t       (axi_tr_ar_chan_t),
            .axi_tr_r_chan_t        (axi_tr_r_chan_t),
            .axi_tr_req_t           (axi_tr_req_t),
            .axi_tr_resp_t          (axi_tr_resp_t),

            .axi_comp_aw_chan_t     (axi_comp_aw_chan_t),
            .axi_comp_w_chan_t      (axi_comp_w_chan_t),
            .axi_comp_b_chan_t      (axi_comp_b_chan_t),
            .axi_comp_ar_chan_t     (axi_comp_ar_chan_t),
            .axi_comp_r_chan_t      (axi_comp_r_chan_t),
            .axi_comp_req_t         (axi_comp_req_t),
            .axi_comp_resp_t        (axi_comp_resp_t),

            .axi_ds_aw_chan_t       (axi_ds_aw_chan_t),
            .axi_ds_w_chan_t        (axi_ds_w_chan_t),
            .axi_ds_b_chan_t        (axi_ds_b_chan_t),
            .axi_ds_ar_chan_t       (axi_ds_ar_chan_t),
            .axi_ds_r_chan_t        (axi_ds_r_chan_t),
            .axi_ds_req_t           (axi_ds_req_t),
            .axi_ds_resp_t          (axi_ds_resp_t),

            .axi_prog_aw_chan_t     (axi_prog_aw_chan_t),
            .axi_prog_w_chan_t      (axi_prog_w_chan_t),
            .axi_prog_b_chan_t      (axi_prog_b_chan_t),
            .axi_prog_ar_chan_t     (axi_prog_ar_chan_t),
            .axi_prog_r_chan_t      (axi_prog_r_chan_t),
            .axi_prog_req_t         (axi_prog_req_t),
            .axi_prog_resp_t        (axi_prog_resp_t)
        ) i_riscv_iommu (
            .clk_i        (clk_i),
            .rst_ni       (rst_ni),
            .tr_req_i     (axi_iommu_tr_req),
            .tr_resp_o    (axi_iommu_tr_resp),
            .comp_resp_i  (axi_iommu_comp_resp),
            .comp_req_o   (axi_iommu_comp_req),
            .ds_resp_i    (axi_iommu_ds_resp),
            .ds_req_o     (axi_iommu_ds_req),
            .prog_req_i   (axi_iommu_prog_req),
            .prog_resp_o  (axi_iommu_prog_resp),
            .wsi_wires_o  (irq_sources[(ariane_soc::RVIOMMUCfg.NumIntVec-1)+7:7]),
            .e3_enable_i(e3_control[0]), .e3_arm_i(e3_control[1]),
            .e3_active_i(e3_control[3]), .e3_delay128_i(e3_control[7]),
            .e3_ctl_o(e3_ctl), .e3_tl_o(e3_tl)
        );

    //----------
    // No IOMMU:
    //----------
    //
    //	When the IOMMU is not included, translation requests are bypassed directly to the XBAR.
    //	AXI transactions performed to the IOMMU programmming IF are responded with error.
    //	All memory IF request xVALID/xREADY wires are set to zero.
    end else begin : gen_iommu_disabled

        axi_err_slv #(
            .AxiIdWidth ( ariane_soc::IdWidthSlave   ),
            .axi_req_t  ( ariane_axi_soc::req_slv_t  ),
            .axi_resp_t ( ariane_axi_soc::resp_slv_t )
        ) i_iommu_err_slv (
            .clk_i      ( clk_i               ),
            .rst_ni     ( rst_ni              ),
            .test_i     ( 1'b0                ),
            .slv_req_i  ( axi_iommu_prog_req  ),
            .slv_resp_o ( axi_iommu_prog_resp )
        );

        // Connect directly the device to the System Interconnect
        // TR IF req => Comp IF req
		`AXI_ASSIGN_FROM_REQ(iommu_comp, axi_iommu_tr_req)

        // Comp IF resp => TR IF resp
		`AXI_ASSIGN_TO_RESP(axi_iommu_tr_resp, iommu_comp)

		// Set memory IF request xVALID/xREADY wires to a known state
        assign iommu_ds.aw_valid  = 1'b0;
        assign iommu_ds.w_valid   = 1'b0;
        assign iommu_ds.b_ready   = 1'b1;
        assign iommu_ds.ar_valid  = 1'b0;
        assign iommu_ds.r_ready   = 1'b1;

        assign irq_sources[(ariane_soc::RVIOMMUCfg.NumIntVec-1)+7:7] = '0;

    end

endmodule
```

---

## `cva6/corev_apu/fpga/src/dma_observer.sv`

```systemverilog
// SPDX-License-Identifier: Solderpad-0.51
// Passive E4 observer. No output controls the DMA or AXI data paths.
module dma_observer #(
    parameter int unsigned NumPorts = 4,
    parameter int unsigned CounterWidth = 64,
    parameter int unsigned OutstandingWidth = 8,
    parameter bit PathEnable = 0,
    parameter bit E3Enable = 0,
    parameter bit ControlledEnable = 0,
    parameter int AxiIdWidth = 4
) (
    input logic clk_i, rst_ni,
    input logic cfg_valid_i, cfg_write_i,
    input logic [6:0] cfg_addr_i,
    input logic [63:0] cfg_wdata_i,
    input logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    input logic [NumPorts-1:0] issue_i, done_i, poll_i,
    input logic [NumPorts-1:0] batch_active_i,
    input logic [63:0] issue_id_i [NumPorts],
    input logic [63:0] done_id_i [NumPorts],
    input logic [63:0] poll_id_i [NumPorts],
    input logic [NumPorts-1:0] ar_valid_i, aw_valid_i, ar_ready_i, aw_ready_i,
    input logic [NumPorts-1:0] rlast_fire_i, b_fire_i, bad_id_i,
    input logic shared_ar_valid_i, shared_aw_valid_i,
    input logic shared_ar_ready_i, shared_aw_ready_i,
    input logic [AxiIdWidth-1:0] tr_id_i [2], out_id_i [2], return_id_i [2],
    input logic [1:0] out_valid_i, out_ready_i, return_i,
    input logic [63:0] e3_data_i,
    input logic e3_error_i,
    output logic [7:0] e3_control_o, e3_index_o
);
    // Low byte is interface version; a missing observer must never look valid.
    localparam logic [63:0] Magic = (ControlledEnable && E3Enable) ? 64'h49444d414f425306 :
        (E3Enable ? 64'h49444d414f425305 :
        (PathEnable ? 64'h49444d414f425303 : 64'h49444d414f425302));
    localparam int Count = 35;
    typedef enum logic [2:0] {OFF, ARMED, HARDWARE, POLL_WAIT, REFILL, FROZEN, BATCH_READ} phase_t;
    phase_t phase_q, phase_n;
    logic [CounterWidth-1:0] count_q [Count], count_n [Count];
    logic [CounterWidth-1:0] segment_q, segment_n;
    logic [OutstandingWidth-1:0] reads_q [NumPorts], reads_n [NumPorts];
    logic [OutstandingWidth-1:0] writes_q [NumPorts], writes_n [NumPorts];
    logic [NumPorts-1:0] track_bad_q, track_bad_n;
    logic [31:0] error_q, error_n;
    logic [63:0] target_q, target_n, expected_q, expected_n;
    logic [8:0] index_q, index_n;
    logic [1:0] victim_q, victim_n, aggressor_q, aggressor_n;
    logic has_a_q, has_a_n, batch_q, batch_n;
    logic finite_q, finite_n;
    logic busy;
    assign busy = active || phase_q == BATCH_READ;
    logic active, v_pending, a_pending;
    logic path_q, path_n, path_arm, path_clear, path_error, path_idle;
    logic [63:0] path_data;
    logic e3_q, e3_n, delay_q, delay_n, e3_arm;
    assign e3_index_o = 8'(index_q - 9'd192);
    assign e3_control_o = {delay_n, e3_q && phase_n == FROZEN && phase_q != FROZEN,
        e3_q && active && phase_n == BATCH_READ, e3_q && phase_q == ARMED && issue_i[victim_q],
        e3_q && active, path_clear, e3_arm, e3_q || e3_arm};
    assign path_idle = !(|reads_q[victim_n] || |writes_q[victim_n] ||
        ar_valid_i[victim_n] || aw_valid_i[victim_n] ||
        (has_a_n && (|reads_q[aggressor_n] || |writes_q[aggressor_n] ||
        ar_valid_i[aggressor_n] || aw_valid_i[aggressor_n])));
    if (PathEnable) begin : gen_path
        dma_path_observer #(.NumPorts(NumPorts), .AxiIdWidth(AxiIdWidth)) i_path (
            .clk_i, .rst_ni, .clear_i(path_clear), .arm_i(path_arm),
            .active_i(active), .open_i(phase_q == ARMED && issue_i[victim_q]),
            .close_i(active && phase_n == BATCH_READ),
            .abort_i(phase_n == FROZEN),
            .victim_i(victim_n), .aggressor_i(aggressor_n), .has_a_i(has_a_n), .idle_i(path_idle),
            .in_valid_i({aw_valid_i, ar_valid_i}), .in_ready_i({aw_ready_i, ar_ready_i}),
            .tr_valid_i({shared_aw_valid_i, shared_ar_valid_i}),
            .tr_ready_i({shared_aw_ready_i, shared_ar_ready_i}),
            .tr_id_i, .out_valid_i, .out_ready_i, .out_id_i, .return_i, .return_id_i,
            .index_i(7'(index_q - 8'd64)), .data_o(path_data), .error_o(path_error)
        );
    end else begin : gen_no_path
        assign path_data = 0;
        assign path_error = 0;
    end

    assign active = phase_q == HARDWARE || phase_q == POLL_WAIT || phase_q == REFILL;
    assign v_pending = |reads_q[victim_q] || |writes_q[victim_q] ||
                       ar_valid_i[victim_q] || aw_valid_i[victim_q];
    assign a_pending = has_a_q && (|reads_q[aggressor_q] || |writes_q[aggressor_q] ||
                                  ar_valid_i[aggressor_q] || aw_valid_i[aggressor_q]);

    // Counter overflow is sticky; values are never silently treated as valid.
    task automatic bump(input int slot, input logic enable);
        if (enable) begin
            count_n[slot] = count_q[slot] + 1'b1;
            if (&count_q[slot]) error_n[2] = 1'b1;
        end
    endtask

    always_comb begin
        cfg_rdata_o = '0;
        case (cfg_addr_i)
            7'h58: cfg_rdata_o = Magic;
            7'h60: begin
                cfg_rdata_o[0] = phase_q == ARMED;
                cfg_rdata_o[1] = busy;
                cfg_rdata_o[2] = phase_q == FROZEN;
                cfg_rdata_o[63:32] = error_q;
                cfg_rdata_o[38] = error_q[6] || (path_q && path_error);
                cfg_rdata_o[39] = error_q[7] || (e3_q && e3_error_i);
            end
            7'h68: cfg_rdata_o = target_q;
            7'h70: cfg_rdata_o = 64'(index_q);
            7'h78: if (int'(index_q) < Count) cfg_rdata_o = 64'(count_q[index_q]);
                   else if (PathEnable && index_q >= 64 && index_q < 192) cfg_rdata_o = path_data;
                   else if (E3Enable && index_q >= 192 && index_q < 448) cfg_rdata_o = e3_data_i;
            default: ;
        endcase
    end

    always_comb begin
        phase_n = phase_q;
        for (int i = 0; i < Count; i++) count_n[i] = count_q[i];
        segment_n = segment_q;
        error_n = error_q;
        expected_n = expected_q;
        target_n = target_q;
        index_n = index_q;
        victim_n = victim_q;
        aggressor_n = aggressor_q;
        has_a_n = has_a_q;
        batch_n = batch_q;
        finite_n = finite_q;
        path_n = path_q;
        path_arm = 0;
        path_clear = 0;
        e3_n = e3_q;
        delay_n = delay_q;
        e3_arm = 0;
        track_bad_n = track_bad_q;
        // Track continuously, including warmup and drain, never reset on ARM.
        for (int i = 0; i < NumPorts; i++) begin
            reads_n[i] = reads_q[i];
            writes_n[i] = writes_q[i];
            case ({ar_valid_i[i] && ar_ready_i[i], rlast_fire_i[i]})
                2'b10: begin
                    reads_n[i] = reads_q[i] + 1'b1;
                    if (&reads_q[i]) track_bad_n[i] = 1'b1;
                end
                2'b01: begin
                    reads_n[i] = reads_q[i] - 1'b1;
                    if (reads_q[i] == 0) track_bad_n[i] = 1'b1;
                end
                default: ; // Simultaneous acceptance/return has zero net change.
            endcase
            case ({aw_valid_i[i] && aw_ready_i[i], b_fire_i[i]})
                2'b10: begin
                    writes_n[i] = writes_q[i] + 1'b1;
                    if (&writes_q[i]) track_bad_n[i] = 1'b1;
                end
                2'b01: begin
                    writes_n[i] = writes_q[i] - 1'b1;
                    if (writes_q[i] == 0) track_bad_n[i] = 1'b1;
                end
                default: ;
            endcase
            if (bad_id_i[i]) track_bad_n[i] = 1'b1;
        end

        if (phase_q == ARMED && issue_i[victim_q]) begin
            phase_n = HARDWARE;
            expected_n = issue_id_i[victim_q];
            count_n[7] = 1;
            segment_n = 0;
            count_n[30] = CounterWidth'((CounterWidth'(reads_n[victim_q]) << 16) | CounterWidth'(writes_n[victim_q]));
            count_n[31] = has_a_q ? CounterWidth'((CounterWidth'(reads_n[aggressor_q]) << 16) | CounterWidth'(writes_n[aggressor_q])) : '0;
            if (issue_id_i[victim_q] == 0) error_n[1] = 1'b1;
        end
        if (phase_q == ARMED || active) begin
            if (track_bad_n[victim_q] || (has_a_q && track_bad_n[aggressor_q]))
                error_n[3] = 1'b1;
        end
        if (!finite_q && batch_q && (active || phase_q == ARMED && issue_i[victim_q]) &&
            has_a_q && !batch_active_i[aggressor_q]) error_n[5] = 1'b1;
        if (phase_q == BATCH_READ) begin
            bump(34, 1'b1);
            if (issue_i[victim_q] || done_i[victim_q]) error_n[1] = 1'b1;
            if (poll_i[victim_q] && poll_id_i[victim_q] == expected_q) begin
                count_n[9] = 1;
                phase_n = FROZEN;
            end
        end
        if (active) begin
            bump(0, 1'b1);
            segment_n = segment_q + 1'b1;
            if (&segment_q) error_n[2] = 1'b1;
            bump(10 + int'({a_pending, v_pending}), 1'b1);
            for (int role = 0; role < 2; role++) begin
                if (role == 0 || has_a_q) begin
                    bump(14 + role*6, ar_valid_i[role == 0 ? victim_q : aggressor_q] && ar_ready_i[role == 0 ? victim_q : aggressor_q]);
                    bump(15 + role*6, aw_valid_i[role == 0 ? victim_q : aggressor_q] && aw_ready_i[role == 0 ? victim_q : aggressor_q]);
                    bump(16 + role*6, rlast_fire_i[role == 0 ? victim_q : aggressor_q]);
                    bump(17 + role*6, b_fire_i[role == 0 ? victim_q : aggressor_q]);
                    bump(18 + role*6, ar_valid_i[role == 0 ? victim_q : aggressor_q] && !ar_ready_i[role == 0 ? victim_q : aggressor_q]);
                    bump(19 + role*6, aw_valid_i[role == 0 ? victim_q : aggressor_q] && !aw_ready_i[role == 0 ? victim_q : aggressor_q]);
                end
            end
            bump(26, shared_ar_valid_i && shared_ar_ready_i);
            bump(27, shared_aw_valid_i && shared_aw_ready_i);
            bump(28, shared_ar_valid_i && !shared_ar_ready_i);
            bump(29, shared_aw_valid_i && !shared_aw_ready_i);
            case (phase_q)
                HARDWARE: begin
                    bump(1, 1'b1);
                    if (issue_i[victim_q]) error_n[1] = 1'b1;
                    if (done_i[victim_q]) begin
                        if (done_id_i[victim_q] != expected_q) error_n[1] = 1'b1;
                        bump(8, 1'b1);
                        if (segment_n > count_q[4]) count_n[4] = segment_n;
                        segment_n = 0;
                        if (batch_q) begin
                            if (64'(count_n[8]) == target_q) begin
                                phase_n = BATCH_READ;
                                count_n[32] = CounterWidth'((CounterWidth'(reads_n[victim_q]) << 16) | CounterWidth'(writes_n[victim_q]));
                                count_n[33] = has_a_q ? CounterWidth'((CounterWidth'(reads_n[aggressor_q]) << 16) | CounterWidth'(writes_n[aggressor_q])) : '0;
                            end else phase_n = REFILL;
                        end else phase_n = POLL_WAIT;
                    end
                end
                POLL_WAIT: begin
                    bump(2, 1'b1);
                    if (issue_i[victim_q] || done_i[victim_q]) error_n[1] = 1'b1;
                    if (poll_i[victim_q] && poll_id_i[victim_q] == expected_q) begin
                        bump(9, 1'b1);
                        if (segment_n > count_q[5]) count_n[5] = segment_n;
                        segment_n = 0;
                        if (64'(count_n[9]) == target_q) begin
                            phase_n = FROZEN;
                            count_n[32] = CounterWidth'((CounterWidth'(reads_n[victim_q]) << 16) | CounterWidth'(writes_n[victim_q]));
                            count_n[33] = has_a_q ? CounterWidth'((CounterWidth'(reads_n[aggressor_q]) << 16) | CounterWidth'(writes_n[aggressor_q])) : '0;
                        end else phase_n = REFILL;
                    end
                end
                REFILL: begin
                    bump(3, 1'b1);
                    if (done_i[victim_q]) error_n[1] = 1'b1;
                    if (issue_i[victim_q]) begin
                        if (issue_id_i[victim_q] == 0 || issue_id_i[victim_q] == expected_q) error_n[1] = 1'b1;
                        expected_n = issue_id_i[victim_q];
                        bump(7, 1'b1);
                        if (segment_n > count_q[6]) count_n[6] = segment_n;
                        segment_n = 0;
                        phase_n = HARDWARE;
                    end
                end
                default: ;
            endcase
        end

        if (cfg_valid_i && cfg_write_i) begin
            if (cfg_wstrb_i != 8'hff) error_n[0] = 1'b1;
            else case (cfg_addr_i)
                7'h68: if (!busy && phase_q != ARMED) target_n = cfg_wdata_i;
                         else error_n[0] = 1'b1;
                7'h70: if (cfg_wdata_i < 64'(Count) ||
                          (PathEnable && cfg_wdata_i >= 64 && cfg_wdata_i < 192) ||
                          (E3Enable && cfg_wdata_i >= 192 && cfg_wdata_i < 448)) index_n = cfg_wdata_i[8:0];
                         else error_n[0] = 1'b1;
                7'h60: begin
                    if (cfg_wdata_i == 2) begin // Abort observation, never DMA.
                        if (busy || phase_q == ARMED) begin
                            phase_n = FROZEN;
                            error_n[4] = 1'b1;
                        end
                    end else if (cfg_wdata_i == 4) begin
                        if (busy || phase_q == ARMED) error_n[0] = 1'b1;
                        else begin phase_n = OFF; error_n = 0; path_clear = 1; path_n = 0; e3_n = 0; finite_n = 0; end
                    end else if ((cfg_wdata_i & ~(64'h000000001f030301 | (ControlledEnable && E3Enable ? 64'h0000000020000000 : 64'h0))) == 0 && cfg_wdata_i[0] &&
                                 (!cfg_wdata_i[29] || (ControlledEnable && E3Enable && cfg_wdata_i[27] && cfg_wdata_i[26] && cfg_wdata_i[25] &&
                                    target_q == 64'd1 && cfg_wdata_i[9:8] == 0 &&
                                    (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1))) &&
                                 (!cfg_wdata_i[28] || cfg_wdata_i[27]) &&
                                 (!cfg_wdata_i[27] || (E3Enable && cfg_wdata_i[26] && cfg_wdata_i[9:8] == 0 &&
                                    (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1))) &&
                                 (!cfg_wdata_i[26] || (PathEnable && cfg_wdata_i[25])) &&
                                 !busy && phase_q != ARMED && target_q != 0 && target_q <= 64'h100000000 &&
                                 int'(cfg_wdata_i[9:8]) < NumPorts && int'(cfg_wdata_i[17:16]) < NumPorts &&
                                 (!cfg_wdata_i[24] || cfg_wdata_i[9:8] != cfg_wdata_i[17:16])) begin
                        phase_n = ARMED;
                        error_n = 0;
                        victim_n = cfg_wdata_i[9:8];
                        aggressor_n = cfg_wdata_i[17:16];
                        has_a_n = cfg_wdata_i[24];
                        batch_n = cfg_wdata_i[25];
                        finite_n = cfg_wdata_i[29];
                        path_n = cfg_wdata_i[26];
                        path_arm = cfg_wdata_i[26];
                        e3_n = cfg_wdata_i[27];
                        e3_arm = cfg_wdata_i[27];
                        delay_n = cfg_wdata_i[28];
                        segment_n = 0;
                        for (int i = 0; i < Count; i++) count_n[i] = 0;
                    end else error_n[0] = 1'b1;
                end
                default: error_n[0] = 1'b1;
            endcase
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            phase_q <= OFF;
            error_q <= 0;
            target_q <= 0;
            expected_q <= 0;
            segment_q <= 0;
            index_q <= 0;
            victim_q <= 0;
            aggressor_q <= 0;
            has_a_q <= 0;
            batch_q <= 0;
            finite_q <= 0;
            path_q <= 0;
            e3_q <= 0;
            delay_q <= 0;
            track_bad_q <= 0;
            for (int i = 0; i < Count; i++) count_q[i] <= 0;
            for (int i = 0; i < NumPorts; i++) begin reads_q[i] <= 0; writes_q[i] <= 0; end
        end else begin
            phase_q <= phase_n;
            error_q <= error_n;
            target_q <= target_n;
            expected_q <= expected_n;
            segment_q <= segment_n;
            index_q <= index_n;
            victim_q <= victim_n;
            aggressor_q <= aggressor_n;
            has_a_q <= has_a_n;
            batch_q <= batch_n;
            finite_q <= finite_n;
            path_q <= path_n;
            e3_q <= e3_n;
            delay_q <= delay_n;
            track_bad_q <= track_bad_n;
            for (int i = 0; i < Count; i++) count_q[i] <= count_n[i];
            for (int i = 0; i < NumPorts; i++) begin reads_q[i] <= reads_n[i]; writes_q[i] <= writes_n[i]; end
        end
    end
endmodule
```

---

## `tests/dma_controlled_launcher_top.sv`

```systemverilog
module dma_controlled_launcher_top #(
    parameter int unsigned NumPorts = 4
) (
    input  logic clk_i, rst_ni,
    input  logic cfg_valid_i, cfg_write_i,
    input  logic [7:0] cfg_addr_i,
    input  logic [63:0] cfg_wdata_i,
    input  logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    input  logic [NumPorts-1:0] frontend_held_i,
    input  logic [NumPorts-1:0] frontend_active_i,
    input  logic [NumPorts-1:0] frontend_cancelled_i,
    input  logic [NumPorts-1:0] frontend_error_i,
    input  logic [NumPorts-1:0] obs_issue_i, obs_done_i,
    input  logic [NumPorts*64-1:0] obs_issue_id_i_flat,
    input  logic [NumPorts*64-1:0] obs_done_id_i_flat,
    input  logic selected_i, select_write_i,
    input  logic [7:0] select_id_i,
    input  logic [23:0] select_did_i,
    input  logic [63:0] select_iova_i,
    output logic [NumPorts-1:0] launch_hold_o,
    output logic [NumPorts-1:0] launch_go_o,
    output logic [NumPorts-1:0] launch_cancel_o
);
    logic [63:0] obs_issue_id_i [NumPorts];
    logic [63:0] obs_done_id_i [NumPorts];

    for (genvar p = 0; p < NumPorts; p++) begin : gen_unflatten
        assign obs_issue_id_i[p] = obs_issue_id_i_flat[p*64 +: 64];
        assign obs_done_id_i[p]  = obs_done_id_i_flat[p*64 +: 64];
    end

    dma_controlled_launcher #(.NumPorts(NumPorts)) i_dut (
        .clk_i, .rst_ni,
        .cfg_valid_i, .cfg_write_i, .cfg_addr_i, .cfg_wdata_i, .cfg_wstrb_i, .cfg_rdata_o,
        .frontend_held_i, .frontend_active_i, .frontend_cancelled_i, .frontend_error_i,
        .obs_issue_i, .obs_done_i, .obs_issue_id_i, .obs_done_id_i,
        .selected_i, .select_write_i, .select_id_i, .select_did_i, .select_iova_i,
        .launch_hold_o, .launch_go_o, .launch_cancel_o
    );
endmodule
```

---

## `tests/dma_controlled_launcher_test.cpp`

```cpp
#include "Vdma_controlled_launcher_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <cstdint>

static constexpr uint64_t MAGIC = 0x49444d414c4e4301ULL;

struct Dut {
    Vdma_controlled_launcher_top *d;

    Dut() {
        d = new Vdma_controlled_launcher_top;
        reset();
    }
    ~Dut() { delete d; }

    void tick() {
        d->clk_i = 0; d->eval();
        d->clk_i = 1; d->eval();
        d->clk_i = 0; d->eval();
    }
    void reset() {
        d->rst_ni = 0;
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->cfg_addr_i = 0; d->cfg_wdata_i = 0; d->cfg_wstrb_i = 0xff;
        d->frontend_held_i = 0; d->frontend_active_i = 0;
        d->frontend_cancelled_i = 0; d->frontend_error_i = 0;
        d->obs_issue_i = 0; d->obs_done_i = 0;
        for (int i = 0; i < 8; i++) {
            d->obs_issue_id_i_flat[i] = 0;
            d->obs_done_id_i_flat[i] = 0;
        }
        d->selected_i = 0; d->select_write_i = 0;
        d->select_id_i = 0; d->select_did_i = 0; d->select_iova_i = 0;
        tick();
        d->rst_ni = 1;
        tick();
    }
    void write(uint8_t addr, uint64_t data, uint8_t strb = 0xff) {
        d->cfg_valid_i = 1; d->cfg_write_i = 1;
        d->cfg_addr_i = addr; d->cfg_wdata_i = data; d->cfg_wstrb_i = strb;
        tick();
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->eval();
    }
    void write_no_tick(uint8_t addr, uint64_t data, uint8_t strb = 0xff) {
        d->cfg_valid_i = 1; d->cfg_write_i = 1;
        d->cfg_addr_i = addr; d->cfg_wdata_i = data; d->cfg_wstrb_i = strb;
        d->eval();
    }
    void end_write() {
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->eval();
    }
    uint64_t read(uint8_t addr) {
        d->cfg_valid_i = 0; d->cfg_write_i = 0;
        d->cfg_addr_i = addr;
        d->eval();
        return d->cfg_rdata_o;
    }
    uint64_t read_index(uint8_t idx) {
        write(0xc0, idx);
        return read(0xc8);
    }
    void set_issue_id(int port, uint64_t id) {
        d->obs_issue_id_i_flat[port*2] = (uint32_t)id;
        d->obs_issue_id_i_flat[port*2+1] = (uint32_t)(id >> 32);
    }
    void set_done_id(int port, uint64_t id) {
        d->obs_done_id_i_flat[port*2] = (uint32_t)id;
        d->obs_done_id_i_flat[port*2+1] = (uint32_t)(id >> 32);
    }
    void issue(int port, uint64_t id) {
        d->obs_issue_i = (1u << port);
        set_issue_id(port, id);
    }
    void done(int port, uint64_t id) {
        d->obs_done_i = (1u << port);
        set_done_id(port, id);
    }
    void clear_events() {
        d->obs_issue_i = 0; d->obs_done_i = 0;
        for (int i = 0; i < 8; i++) {
            d->obs_issue_id_i_flat[i] = 0;
            d->obs_done_id_i_flat[i] = 0;
        }
    }
    void set_held(int port, bool v) {
        if (v) d->frontend_held_i = d->frontend_held_i | (1u << port);
        else d->frontend_held_i = d->frontend_held_i & ~(1u << port);
    }
    void set_active(int port, bool v) {
        if (v) d->frontend_active_i = d->frontend_active_i | (1u << port);
        else d->frontend_active_i = d->frontend_active_i & ~(1u << port);
    }
    uint64_t status() { return read(0xb8); }
    bool error_any() { return (status() >> 32) != 0; }
};

static void test_magic(Dut &d) {
    assert(d.read(0xb0) == MAGIC);
}

static void test_solo_prep_go_done(Dut &d) {
    d.write(0xb8, 1); // PREP solo
    assert((d.status() & 1) == 1); // PREP
    assert(d.d->launch_hold_o == 1);
    d.set_held(0, true);
    d.write(0xb8, 2); // GO
    assert(d.d->launch_go_o == 1);
    d.tick();
    assert(d.d->launch_go_o == 0);
    d.set_held(0, false);
    d.set_active(0, true);
    d.issue(0, 91);
    d.tick();
    d.clear_events();
    d.done(0, 91);
    d.tick();
    d.clear_events();
    d.set_active(0, false);
    d.tick();
    assert((d.status() & 4) == 4); // FROZEN
    assert((d.status() & 8) == 0); // not aborted
    assert(d.read_index(24) == 1); // V issue count
    assert(d.read_index(26) == 1); // V done count
    assert(d.read_index(11) == 91);
    assert(d.read_index(13) == 91);
}

static void test_non_solo_exact_goA_then_goV(Dut &d) {
    d.write(0xb8, 1 | 0x100); // PREP with A
    assert(d.read_index(19) == 3); // expected mask V|A
    assert(d.d->launch_hold_o == 3);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2); // GO
    assert(d.d->launch_go_o == 2); // A first only
    d.tick();
    assert(d.d->launch_go_o == 0);
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    d.tick();
    d.clear_events();
    // Valid A read select.
    d.d->selected_i = 1; d.d->select_write_i = 0;
    d.d->select_id_i = 4; d.d->select_did_i = 11; d.d->select_iova_i = 0x1000;
    d.tick();
    assert(d.d->launch_go_o == 1); // V go on following cycle
    d.d->selected_i = 0;
    d.tick();
    assert(d.d->launch_go_o == 0);
}

static void test_v_never_go_on_bad_a_select(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2);
    d.tick();
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    d.tick();
    d.clear_events();
    // A write must not release V.
    d.d->selected_i = 1; d.d->select_write_i = 1;
    d.d->select_id_i = 4; d.d->select_did_i = 11;
    d.tick();
    assert(d.d->launch_go_o == 0);
    // Wrong ID.
    d.d->select_write_i = 0; d.d->select_id_i = 5; d.d->select_did_i = 11;
    d.tick();
    assert(d.d->launch_go_o == 0);
    // Wrong DID.
    d.d->select_id_i = 4; d.d->select_did_i = 12;
    d.tick();
    assert(d.d->launch_go_o == 0);
    d.d->selected_i = 0;
}

static void test_duplicate_and_bad_writes(Dut &d) {
    d.write(0xb8, 1);
    d.write(0xb8, 1); // second PREP
    assert(d.error_any());
    d.write(0x60, 4); // unsupported forwarded write
    assert(d.error_any());
    d.write(0xb8, 2, 0x00); // partial strobe
    assert(d.error_any());
    d.write(0xb0, 0); // write magic
    assert(d.error_any());
    d.write(0xc8, 0); // write data
    assert(d.error_any());
}

static void test_abort_before_reserve(Dut &d) {
    d.write(0xb8, 4); // ABORT in OFF
    d.tick();
    uint64_t s = d.status();
    assert((s & 8) == 8); // aborted bit
    assert((s & 4) == 4); // frozen
    assert((s & 1) == 0); // not PREP
}

static void test_go_then_immediate_abort(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2); // GO, registers go pulse
    assert(d.d->launch_go_o == 2);
    // Same edge MMIO ABORT while registered go is high.
    d.write_no_tick(0xb8, 4, 0xff);
    assert(d.d->launch_go_o == 0);
    assert((d.d->launch_cancel_o & 0x3) == 0x3);
    d.tick();
    d.end_write();
    // Frontend acknowledges cancel for both held ports.
    d.d->frontend_cancelled_i = 0x3;
    d.tick();
    d.d->frontend_cancelled_i = 0;
    d.set_held(0, false);
    d.set_held(1, false);
    d.tick();
    assert((d.status() & 8) == 8);
    assert(d.read_index(17) == 3);
    assert(d.read_index(15) == 0);
}

static void test_count_backpressure_abort_drains(Dut &d) {
    d.write(0xb8, 1); // PREP solo
    d.set_held(0, true);
    d.write(0xb8, 2); // GO
    d.tick();
    // Frontend left HOLD with COUNT backpressure.
    d.set_held(0, false);
    d.set_active(0, true);
    d.tick();
    // ABORT while active COUNT.
    d.write(0xb8, 4);
    assert((d.d->launch_cancel_o & 1) == 0);
    d.tick();
    d.tick();
    assert((d.status() & 4) == 0); // not frozen
    // Actual issue and done drain.
    d.issue(0, 103);
    d.tick();
    d.clear_events();
    d.done(0, 103);
    d.tick();
    d.clear_events();
    d.set_active(0, false);
    d.tick();
    assert((d.status() & 8) == 8);
    assert((d.status() & 4) == 4);
    assert(d.read_index(15) == 1);
    assert(d.read_index(16) == 1);
    assert(d.read_index(17) == 0);
}

static void test_partial_reserve_abort(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true); // only V reserves
    d.write(0xb8, 4); // ABORT while only V held
    assert((d.d->launch_cancel_o & 1) == 1);
    assert((d.d->launch_cancel_o & 2) == 0);
    d.tick();
    d.d->frontend_cancelled_i = 1;
    d.tick();
    d.d->frontend_cancelled_i = 0;
    d.set_held(0, false);
    d.tick();
    assert((d.status() & 8) == 8);
    assert(d.read_index(17) == 1);
}

static void test_new_prep_after_aborted_terminal(Dut &d) {
    d.write(0xb8, 4); // abort in OFF -> frozen aborted
    d.tick();
    assert((d.status() & 8) == 8);
    d.write(0xb8, 1); // new PREP in FROZEN
    assert((d.status() & 1) == 1);
    assert((d.status() & 8) == 0);
    assert(d.read_index(1) == 0);
}

static void test_both_done_and_late_a(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2);
    d.tick();
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    d.tick();
    d.clear_events();
    d.d->selected_i = 1; d.d->select_write_i = 0;
    d.d->select_id_i = 4; d.d->select_did_i = 11;
    d.tick();
    d.d->selected_i = 0;
    d.tick();
    // V issue and done first.
    d.set_held(0, false);
    d.set_active(0, true);
    d.issue(0, 3);
    d.tick();
    d.clear_events();
    d.done(0, 3);
    d.tick();
    d.clear_events();
    d.set_active(0, false);
    d.tick();
    assert((d.status() & 4) == 0); // not frozen, A still pending
    // A done late.
    d.done(1, 2);
    d.tick();
    d.clear_events();
    d.set_active(1, false);
    d.tick();
    assert((d.status() & 4) == 4);
}

static void test_abort_select_race(Dut &d) {
    d.write(0xb8, 1 | 0x100);
    d.set_held(0, true);
    d.set_held(1, true);
    d.write(0xb8, 2);
    d.tick();
    d.set_held(1, false);
    d.set_active(1, true);
    d.issue(1, 2);
    // ABORT and valid A select same cycle. ABORT must win.
    d.d->selected_i = 1; d.d->select_write_i = 0;
    d.d->select_id_i = 4; d.d->select_did_i = 11;
    d.write_no_tick(0xb8, 4, 0xff);
    assert(d.d->launch_go_o == 0);
    d.tick();
    d.end_write();
    d.d->selected_i = 0;
    d.clear_events();
    // A issue must still drain.
    d.done(1, 2);
    d.tick();
    d.clear_events();
    d.set_active(1, false);
    d.tick();
    assert((d.status() & 8) == 8);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    test_magic(*(new Dut));
    test_solo_prep_go_done(*(new Dut));
    test_non_solo_exact_goA_then_goV(*(new Dut));
    test_v_never_go_on_bad_a_select(*(new Dut));
    test_duplicate_and_bad_writes(*(new Dut));
    test_abort_before_reserve(*(new Dut));
    test_go_then_immediate_abort(*(new Dut));
    test_count_backpressure_abort_drains(*(new Dut));
    test_partial_reserve_abort(*(new Dut));
    test_new_prep_after_aborted_terminal(*(new Dut));
    test_both_done_and_late_a(*(new Dut));
    test_abort_select_race(*(new Dut));
    std::puts("PASS dma_controlled_launcher");
    return 0;
}
```

---

## `tests/test_dma_controlled_launcher.py`

```python
#!/usr/bin/env python3
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/e3-controlled-20260918/dma-controlled-launcher'
out.mkdir(parents=True, exist_ok=True)

fpga = root / 'cva6/corev_apu/fpga/src'
files = [
    fpga / 'dma_controlled_launcher.sv',
    root / 'tests/dma_controlled_launcher_top.sv',
    root / 'tests/dma_controlled_launcher_test.cpp',
]

cmd = [
    'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
    '--top-module', 'dma_controlled_launcher_top',
    '--Mdir', str(out),
    *map(str, files),
]

log = out / 'build.log'
with log.open('w') as f:
    r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
if r.returncode:
    print(log.read_text())
    raise SystemExit(r.returncode)

subprocess.run([str(out / 'Vdma_controlled_launcher_top')], check=True)
```

---

## `tests/dma_observer_finite_test.cpp`

```cpp
#include "Vdma_observer.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>

struct B {
    Vdma_observer d;
    void tick() { d.clk_i = 0; d.eval(); d.clk_i = 1; d.eval(); d.clk_i = 0; d.eval(); }
    B() { d.rst_ni = 0; tick(); d.rst_ni = 1; tick(); }
    void write(unsigned a, unsigned long long v) {
        d.cfg_valid_i = d.cfg_write_i = 1;
        d.cfg_wstrb_i = 0xff;
        d.cfg_addr_i = a;
        d.cfg_wdata_i = v;
        tick();
        d.cfg_valid_i = d.cfg_write_i = 0;
        d.eval();
    }
    unsigned long long read(unsigned a) {
        d.cfg_addr_i = a;
        d.eval();
        return d.cfg_rdata_o;
    }
};

static void arm_common(B &b, bool finite, unsigned target = 1) {
    b.write(0x68, target);
    unsigned long long cmd = 1 | (1 << 8) | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27);
    if (finite) cmd |= (1ULL << 29);
    b.write(0x60, cmd);
}

#ifdef CONTROLLED
static void test_controlled_finite_allows_a_before_v() {
    B b;
    assert(b.read(0x58) == 0x49444d414f425306ULL);
    arm_common(b, true, 1);
    assert(!(b.read(0x60) >> 32));
    // V issue, port 0.
    b.d.issue_i = 1u << 0;
    b.d.issue_id_i[0] = 1;
    b.tick();
    b.d.issue_i = 0;
    // A issue and done before V, port 1.
    b.d.issue_i = 1u << 1;
    b.d.issue_id_i[1] = 2;
    b.tick();
    b.d.issue_i = 0;
    b.d.done_i = 1u << 1;
    b.d.done_id_i[1] = 2;
    b.d.batch_active_i = 0;
    b.tick();
    b.d.done_i = 0;
    assert(!(b.read(0x60) & (1ULL << 37))); // error bit 5
}

static void test_controlled_rejects_finite_without_e3() {
    B b;
    b.write(0x68, 1);
    unsigned long long cmd = 1 | (1 << 8) | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1ULL << 29);
    b.write(0x60, cmd);
    assert((b.read(0x60) >> 32) != 0);
}

static void test_controlled_rejects_finite_target_gt1() {
    B b;
    b.write(0x68, 2);
    unsigned long long cmd = 1 | (1 << 8) | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) | (1ULL << 29);
    b.write(0x60, cmd);
    assert((b.read(0x60) >> 32) != 0);
}
#endif

#ifndef CONTROLLED
static void test_legacy_finite_flag_rejected_and_continuous_a_error() {
    B b;
    assert(b.read(0x58) == 0x49444d414f425305ULL);
    b.write(0x68, 1);
    unsigned long long bad = 1 | (1 << 8) | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) | (1ULL << 29);
    b.write(0x60, bad);
    assert((b.read(0x60) >> 32) != 0);
    // Same A-before-V without finite must set batch-active error bit5.
    B c;
    arm_common(c, false, 1);
    assert(!(c.read(0x60) >> 32));
    c.d.issue_i = 1u << 0;
    c.d.issue_id_i[0] = 1;
    c.tick();
    c.d.issue_i = 0;
    c.d.issue_i = 1u << 1;
    c.d.issue_id_i[1] = 2;
    c.tick();
    c.d.issue_i = 0;
    c.d.done_i = 1u << 1;
    c.d.done_id_i[1] = 2;
    c.d.batch_active_i = 0;
    c.tick();
    c.d.done_i = 0;
    assert(c.read(0x60) & (1ULL << 37));
}
#endif

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
#ifdef CONTROLLED
    test_controlled_finite_allows_a_before_v();
    test_controlled_rejects_finite_without_e3();
    test_controlled_rejects_finite_target_gt1();
    std::puts("PASS controlled v6 observer finite/legacy checks");
#else
    test_legacy_finite_flag_rejected_and_continuous_a_error();
    std::puts("PASS legacy v5 observer continuous A rejection");
#endif
    return 0;
}
```

---

## `tests/test_dma_observer_finite.py`

```python
#!/usr/bin/env python3
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/e3-controlled-20260918/dma-observer-finite'
out.mkdir(parents=True, exist_ok=True)

fpga = root / 'cva6/corev_apu/fpga/src'
base = [
    fpga / 'dma_path_observer.sv',
    fpga / 'dma_observer.sv',
    root / 'tests/dma_observer_finite_test.cpp',
]

for controlled in (1, 0):
    name = 'controlled' if controlled else 'legacy'
    dest = out / name
    dest.mkdir(exist_ok=True)
    cmd = [
        'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
        '--top-module', 'dma_observer',
        '--Mdir', str(dest),
        '-GPathEnable=1', '-GE3Enable=1', f'-GControlledEnable={controlled}',
        '-CFLAGS', '-D' + ('CONTROLLED' if controlled else 'LEGACY'),
        *map(str, base),
    ]
    log = dest / 'build.log'
    with log.open('w') as f:
        r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
    if r.returncode:
        print(log.read_text())
        raise SystemExit(r.returncode)
    subprocess.run([str(dest / 'Vdma_observer')], check=True)
```

---

## `tests/idma_controlled_pair_top.sv`

```systemverilog
module idma_controlled_pair_top (
    input  logic clk_i, rst_ni,
    // CPU config for V frontend (port 0)
    input  logic v_cpu_valid_i, v_cpu_write_i,
    input  logic [31:0] v_cpu_addr_i,
    input  logic [63:0] v_cpu_wdata_i,
    input  logic [7:0] v_cpu_wstrb_i,
    output logic v_cpu_ready_o,
    output logic [63:0] v_cpu_rdata_o,
    output logic v_cpu_error_o,
    // CPU config for A frontend (port 1)
    input  logic a_cpu_valid_i, a_cpu_write_i,
    input  logic [31:0] a_cpu_addr_i,
    input  logic [63:0] a_cpu_wdata_i,
    input  logic [7:0] a_cpu_wstrb_i,
    output logic a_cpu_ready_o,
    output logic [63:0] a_cpu_rdata_o,
    output logic a_cpu_error_o,
    // Backend controls/outputs for V
    input  logic v_backend_ready_i,
    input  logic v_backend_idle_i,
    input  logic v_trans_complete_i,
    output logic v_valid_o,
    output logic [63:0] v_burst_src_addr_o,
    output logic [63:0] v_burst_dst_addr_o,
    output logic [63:0] v_burst_length_o,
    output logic [63:0] v_issue_id_o,
    output logic [63:0] v_done_id_o,
    output logic v_obs_issue_o,
    output logic v_obs_done_o,
    // Backend controls/outputs for A
    input  logic a_backend_ready_i,
    input  logic a_backend_idle_i,
    input  logic a_trans_complete_i,
    output logic a_valid_o,
    output logic [63:0] a_burst_src_addr_o,
    output logic [63:0] a_burst_dst_addr_o,
    output logic [63:0] a_burst_length_o,
    output logic [63:0] a_issue_id_o,
    output logic [63:0] a_done_id_o,
    output logic a_obs_issue_o,
    output logic a_obs_done_o,
    // Launcher status/select
    input  logic selected_i,
    input  logic select_write_i,
    input  logic [7:0] select_id_i,
    input  logic [23:0] select_did_i,
    input  logic [63:0] select_iova_i,
    output logic [3:0] launch_hold_o,
    output logic [3:0] launch_go_o,
    output logic [3:0] launch_cancel_o,
    output logic [3:0] frontend_held_o,
    output logic [3:0] frontend_active_o,
    output logic [3:0] frontend_cancelled_o,
    output logic [3:0] frontend_error_o,
    output logic [63:0] launcher_rdata_o,
    output logic launcher_valid_o,
    output logic launcher_write_o,
    output logic [7:0] launcher_addr_o
);
    typedef struct packed {
        logic        valid;
        logic        write;
        logic [31:0] addr;
        logic [63:0] wdata;
        logic [7:0]  wstrb;
    } dma_req_t;

    typedef struct packed {
        logic        ready;
        logic [63:0] rdata;
        logic        error;
    } dma_rsp_t;

    typedef struct packed {
        logic [63:0] length;
        logic [63:0] src_addr;
        logic [63:0] dst_addr;
        struct packed {
            logic [3:0] axi_id;
            logic [23:0] device_id;
            logic pv;
            logic [19:0] process_id;
            struct packed {
                logic [1:0] burst;
                logic [3:0] cache;
                logic lock;
                logic [2:0] prot;
                logic [3:0] qos;
                logic [3:0] region;
            } src;
            struct packed {
                logic [1:0] burst;
                logic [3:0] cache;
                logic lock;
                logic [2:0] prot;
                logic [3:0] qos;
                logic [3:0] region;
            } dst;
            struct packed {
                logic decouple_aw;
                logic decouple_rw;
                logic [7:0] src_max_llen;
                logic [7:0] dst_max_llen;
                logic src_reduce_len;
                logic dst_reduce_len;
            } beo;
        } opt;
    } test_burst_req_t;

    dma_req_t v_req, a_req;
    dma_rsp_t v_rsp, a_rsp;
    test_burst_req_t v_burst_req, a_burst_req;

    logic v_obs_cfg_valid, v_obs_cfg_write;
    logic [7:0] v_obs_cfg_addr;
    logic [63:0] v_obs_cfg_wdata, v_obs_cfg_rdata;
    logic [7:0] v_obs_cfg_wstrb;
    logic a_obs_cfg_valid, a_obs_cfg_write;
    logic [7:0] a_obs_cfg_addr;
    logic [63:0] a_obs_cfg_wdata, a_obs_cfg_rdata;
    logic [7:0] a_obs_cfg_wstrb;

    logic v_launch_held, v_launch_active, v_launch_cancelled, v_launch_error;
    logic a_launch_held, a_launch_active, a_launch_cancelled, a_launch_error;
    logic [3:0] fe_held, fe_active, fe_cancelled, fe_error;
    logic [3:0] launcher_hold, launcher_go, launcher_cancel;
    logic [63:0] launcher_issue_id [4];
    logic [63:0] launcher_done_id [4];

    assign v_req.valid = v_cpu_valid_i;
    assign v_req.write = v_cpu_write_i;
    assign v_req.addr = v_cpu_addr_i;
    assign v_req.wdata = v_cpu_wdata_i;
    assign v_req.wstrb = v_cpu_wstrb_i;
    assign v_cpu_ready_o = v_rsp.ready;
    assign v_cpu_rdata_o = v_rsp.rdata;
    assign v_cpu_error_o = v_rsp.error;

    assign a_req.valid = a_cpu_valid_i;
    assign a_req.write = a_cpu_write_i;
    assign a_req.addr = a_cpu_addr_i;
    assign a_req.wdata = a_cpu_wdata_i;
    assign a_req.wstrb = a_cpu_wstrb_i;
    assign a_cpu_ready_o = a_rsp.ready;
    assign a_cpu_rdata_o = a_rsp.rdata;
    assign a_cpu_error_o = a_rsp.error;

    assign v_obs_cfg_rdata = v_obs_cfg_addr[7] ? launcher_rdata_o : 64'h0;
    assign a_obs_cfg_rdata = 64'h0;

    idma_reg64_frontend #(
        .dma_regs_req_t(dma_req_t),
        .dma_regs_rsp_t(dma_rsp_t),
        .ObserverPort(1'b1),
        .BatchPort(1'b0),
        .LaunchPort(1'b1),
        .DEVICE_ID(24'd10),
        .PROCESS_ID(20'd20),
        .AxiIdWidth(4),
        .AxID('0)
    ) i_v_frontend (
        .clk_i, .rst_ni,
        .obs_issue_o(v_obs_issue_o), .obs_done_o(v_obs_done_o), .obs_poll_o(),
        .batch_active_o(),
        .obs_issue_id_o(v_issue_id_o), .obs_done_id_o(v_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(v_obs_cfg_valid), .obs_cfg_write_o(v_obs_cfg_write),
        .obs_cfg_addr_o(v_obs_cfg_addr), .obs_cfg_wdata_o(v_obs_cfg_wdata),
        .obs_cfg_wstrb_o(v_obs_cfg_wstrb), .obs_cfg_rdata_i(v_obs_cfg_rdata),
        .dma_ctrl_req_i(v_req), .dma_ctrl_rsp_o(v_rsp),
        .burst_req_o(v_burst_req), .valid_o(v_valid_o),
        .ready_i(v_backend_ready_i), .backend_idle_i(v_backend_idle_i),
        .trans_complete_i(v_trans_complete_i),
        .ttype_o(), .wdata_o(), .rdata_i(64'h0), .rdata_valid_i(1'b0),
        .launch_hold_i(launcher_hold[0]), .launch_go_i(launcher_go[0]),
        .launch_cancel_i(launcher_cancel[0]),
        .launch_held_o(v_launch_held), .launch_active_o(v_launch_active),
        .launch_cancelled_o(v_launch_cancelled), .launch_error_o(v_launch_error)
    );

    idma_reg64_frontend #(
        .dma_regs_req_t(dma_req_t),
        .dma_regs_rsp_t(dma_rsp_t),
        .ObserverPort(1'b0),
        .BatchPort(1'b0),
        .LaunchPort(1'b1),
        .DEVICE_ID(24'd11),
        .PROCESS_ID(20'd21),
        .AxiIdWidth(4),
        .AxID(4'd1)
    ) i_a_frontend (
        .clk_i, .rst_ni,
        .obs_issue_o(a_obs_issue_o), .obs_done_o(a_obs_done_o), .obs_poll_o(),
        .batch_active_o(),
        .obs_issue_id_o(a_issue_id_o), .obs_done_id_o(a_done_id_o), .obs_poll_id_o(),
        .obs_cfg_valid_o(a_obs_cfg_valid), .obs_cfg_write_o(a_obs_cfg_write),
        .obs_cfg_addr_o(a_obs_cfg_addr), .obs_cfg_wdata_o(a_obs_cfg_wdata),
        .obs_cfg_wstrb_o(a_obs_cfg_wstrb), .obs_cfg_rdata_i(a_obs_cfg_rdata),
        .dma_ctrl_req_i(a_req), .dma_ctrl_rsp_o(a_rsp),
        .burst_req_o(a_burst_req), .valid_o(a_valid_o),
        .ready_i(a_backend_ready_i), .backend_idle_i(a_backend_idle_i),
        .trans_complete_i(a_trans_complete_i),
        .ttype_o(), .wdata_o(), .rdata_i(64'h0), .rdata_valid_i(1'b0),
        .launch_hold_i(launcher_hold[1]), .launch_go_i(launcher_go[1]),
        .launch_cancel_i(launcher_cancel[1]),
        .launch_held_o(a_launch_held), .launch_active_o(a_launch_active),
        .launch_cancelled_o(a_launch_cancelled), .launch_error_o(a_launch_error)
    );

    assign fe_held = {2'b0, a_launch_held, v_launch_held};
    assign fe_active = {2'b0, a_launch_active, v_launch_active};
    assign fe_cancelled = {2'b0, a_launch_cancelled, v_launch_cancelled};
    assign fe_error = {2'b0, a_launch_error, v_launch_error};
    assign frontend_held_o = fe_held;
    assign frontend_active_o = fe_active;
    assign frontend_cancelled_o = fe_cancelled;
    assign frontend_error_o = fe_error;

    assign launcher_issue_id[0] = v_issue_id_o;
    assign launcher_issue_id[1] = a_issue_id_o;
    assign launcher_issue_id[2] = '0;
    assign launcher_issue_id[3] = '0;
    assign launcher_done_id[0] = v_done_id_o;
    assign launcher_done_id[1] = a_done_id_o;
    assign launcher_done_id[2] = '0;
    assign launcher_done_id[3] = '0;

    assign launcher_valid_o = v_obs_cfg_valid && v_obs_cfg_addr[7];
    assign launcher_write_o = v_obs_cfg_write;
    assign launcher_addr_o = v_obs_cfg_addr;

    dma_controlled_launcher #(.NumPorts(4)) i_launcher (
        .clk_i, .rst_ni,
        .cfg_valid_i(v_obs_cfg_valid && v_obs_cfg_addr[7]),
        .cfg_write_i(v_obs_cfg_write),
        .cfg_addr_i(v_obs_cfg_addr),
        .cfg_wdata_i(v_obs_cfg_wdata),
        .cfg_wstrb_i(v_obs_cfg_wstrb),
        .cfg_rdata_o(launcher_rdata_o),
        .frontend_held_i(fe_held),
        .frontend_active_i(fe_active),
        .frontend_cancelled_i(fe_cancelled),
        .frontend_error_i(fe_error),
        .obs_issue_i({2'b0, a_obs_issue_o, v_obs_issue_o}),
        .obs_done_i({2'b0, a_obs_done_o, v_obs_done_o}),
        .obs_issue_id_i(launcher_issue_id),
        .obs_done_id_i(launcher_done_id),
        .selected_i(selected_i),
        .select_write_i(select_write_i),
        .select_id_i(select_id_i),
        .select_did_i(select_did_i),
        .select_iova_i(select_iova_i),
        .launch_hold_o(launcher_hold),
        .launch_go_o(launcher_go),
        .launch_cancel_o(launcher_cancel)
    );

    assign launch_hold_o = launcher_hold;
    assign launch_go_o = launcher_go;
    assign launch_cancel_o = launcher_cancel;

    assign v_burst_src_addr_o = v_burst_req.src_addr;
    assign v_burst_dst_addr_o = v_burst_req.dst_addr;
    assign v_burst_length_o = v_burst_req.length;
    assign a_burst_src_addr_o = a_burst_req.src_addr;
    assign a_burst_dst_addr_o = a_burst_req.dst_addr;
    assign a_burst_length_o = a_burst_req.length;
endmodule
```

---

## `tests/idma_controlled_pair_test.cpp`

```cpp
#include "Vidma_controlled_pair_top.h"
#include <verilated.h>
#include <cassert>
#include <cstdio>
#include <cstdint>

struct Tb {
    Vidma_controlled_pair_top *d;

    Tb() { d = new Vidma_controlled_pair_top; reset(); }
    ~Tb() { delete d; }

    void tick() {
        d->clk_i = 0; d->eval();
        d->clk_i = 1; d->eval();
        d->clk_i = 0; d->eval();
    }
    void reset() {
        d->rst_ni = 0;
        d->v_cpu_valid_i = 0; d->v_cpu_write_i = 0; d->v_cpu_addr_i = 0; d->v_cpu_wdata_i = 0; d->v_cpu_wstrb_i = 0xff;
        d->a_cpu_valid_i = 0; d->a_cpu_write_i = 0; d->a_cpu_addr_i = 0; d->a_cpu_wdata_i = 0; d->a_cpu_wstrb_i = 0xff;
        d->v_backend_ready_i = 0; d->v_backend_idle_i = 1; d->v_trans_complete_i = 0;
        d->a_backend_ready_i = 0; d->a_backend_idle_i = 1; d->a_trans_complete_i = 0;
        d->selected_i = 0; d->select_write_i = 0; d->select_id_i = 0; d->select_did_i = 0; d->select_iova_i = 0;
        tick();
        d->rst_ni = 1;
        tick();
    }
    void cpu_write(bool is_a, uint32_t addr, uint64_t data, uint8_t strb = 0xff) {
        if (is_a) {
            d->a_cpu_valid_i = 1; d->a_cpu_write_i = 1;
            d->a_cpu_addr_i = addr; d->a_cpu_wdata_i = data; d->a_cpu_wstrb_i = strb;
            while (!d->a_cpu_ready_o) tick();
            tick();
            d->a_cpu_valid_i = 0; d->a_cpu_write_i = 0;
        } else {
            d->v_cpu_valid_i = 1; d->v_cpu_write_i = 1;
            d->v_cpu_addr_i = addr; d->v_cpu_wdata_i = data; d->v_cpu_wstrb_i = strb;
            while (!d->v_cpu_ready_o) tick();
            tick();
            d->v_cpu_valid_i = 0; d->v_cpu_write_i = 0;
        }
        d->eval();
    }
    uint64_t cpu_read(bool is_a, uint32_t addr) {
        if (is_a) {
            d->a_cpu_valid_i = 1; d->a_cpu_write_i = 0; d->a_cpu_addr_i = addr;
            while (!d->a_cpu_ready_o) tick();
            uint64_t v = d->a_cpu_rdata_o;
            tick();
            d->a_cpu_valid_i = 0;
            d->eval();
            return v;
        } else {
            d->v_cpu_valid_i = 1; d->v_cpu_write_i = 0; d->v_cpu_addr_i = addr;
            while (!d->v_cpu_ready_o) tick();
            uint64_t v = d->v_cpu_rdata_o;
            tick();
            d->v_cpu_valid_i = 0;
            d->eval();
            return v;
        }
    }
    void launcher_write(uint8_t addr, uint64_t data) { cpu_write(false, addr, data); }
    uint64_t launcher_read(uint8_t addr) { return cpu_read(false, addr); }
    void configure(bool is_a, uint64_t src, uint64_t dst) {
        cpu_write(is_a, 0x00, src);
        cpu_write(is_a, 0x08, dst);
        cpu_write(is_a, 0x10, 64);
        cpu_write(is_a, 0x18, 0);
    }
    uint64_t reserve(bool is_a) { return cpu_read(is_a, 0x28); }
};

static void test_basic_pair_flow() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    uint64_t v_id = t.reserve(false);
    uint64_t a_id = t.reserve(true);
    assert(v_id != 0 && a_id != 0 && v_id != a_id);
    assert(t.d->frontend_held_o == 3);
    t.launcher_write(0xb8, 2); // GO -> A first
    assert(t.d->launch_go_o == 2);
    t.tick();
    assert(t.d->launch_go_o == 0);
    // A offers valid and is accepted.
    t.d->a_backend_ready_i = 1;
    while (!t.d->a_valid_o) t.tick();
    t.tick();
    t.d->a_backend_ready_i = 0;
    // Valid A read select after actual A issue.
    t.d->selected_i = 1; t.d->select_write_i = 0;
    t.d->select_id_i = 4; t.d->select_did_i = 11; t.d->select_iova_i = 0x3000;
    t.tick();
    assert(t.d->launch_go_o == 1);
    t.d->selected_i = 0;
    // V offers valid and is accepted.
    t.d->v_backend_ready_i = 1;
    while (!t.d->v_valid_o) t.tick();
    t.tick();
    t.d->v_backend_ready_i = 0;
    // Complete both.
    t.d->a_trans_complete_i = 1; t.tick(); t.d->a_trans_complete_i = 0;
    t.d->v_trans_complete_i = 1; t.tick(); t.d->v_trans_complete_i = 0;
    t.tick();
    assert((t.launcher_read(0xb8) & 4) == 4);
    assert(t.launcher_read(0xc0) == 0);
    t.launcher_write(0xc0, 11);
    assert(t.launcher_read(0xc8) == v_id);
    t.launcher_write(0xc0, 12);
    assert(t.launcher_read(0xc8) == a_id);
}

static void test_go_then_immediate_abort() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    t.reserve(true);
    t.launcher_write(0xb8, 2);
    assert(t.d->launch_go_o == 2);
    // Abort on the same edge before frontend samples registered go.
    t.d->v_cpu_valid_i = 1; t.d->v_cpu_write_i = 1;
    t.d->v_cpu_addr_i = 0xb8; t.d->v_cpu_wdata_i = 4; t.d->v_cpu_wstrb_i = 0xff;
    t.d->eval();
    assert(t.d->launch_go_o == 0);
    assert(t.d->launch_cancel_o == 3);
    t.tick();
    t.d->v_cpu_valid_i = 0; t.d->v_cpu_write_i = 0;
    t.tick();
    assert(t.d->v_valid_o == 0 && t.d->a_valid_o == 0);
    assert((t.launcher_read(0xb8) & 8) == 8);
}

static void test_count_backpressure_abort() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.launcher_write(0xb8, 1);
    t.reserve(false);
    t.launcher_write(0xb8, 2);
    t.tick();
    t.d->v_backend_ready_i = 0;
    while (!t.d->v_valid_o) t.tick();
    t.launcher_write(0xb8, 4);
    assert((t.d->launch_cancel_o & 1) == 0);
    t.tick();
    assert(t.d->v_valid_o == 1);
    t.d->v_backend_ready_i = 1;
    t.tick();
    t.d->v_backend_ready_i = 0;
    t.d->v_trans_complete_i = 1; t.tick(); t.d->v_trans_complete_i = 0;
    t.tick();
    assert((t.launcher_read(0xb8) & 8) == 8);
    assert((t.launcher_read(0xb8) & 4) == 4);
}

static void test_partial_reserve_abort() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    assert(t.d->frontend_held_o == 1);
    t.launcher_write(0xb8, 4);
    t.tick();
    assert((t.launcher_read(0xb8) & 8) == 8);
}

static void test_new_prep_after_aborted_terminal() {
    Tb t;
    t.launcher_write(0xb8, 4);
    t.tick();
    assert((t.launcher_read(0xb8) & 8) == 8);
    t.launcher_write(0xb8, 1);
    assert((t.launcher_read(0xb8) & 1) == 1);
    assert((t.launcher_read(0xb8) & 8) == 0);
}

static void test_v_release_only_valid_a_select() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    t.reserve(true);
    t.launcher_write(0xb8, 2);
    t.tick();
    t.d->a_backend_ready_i = 1;
    while (!t.d->a_valid_o) t.tick();
    t.tick();
    t.d->a_backend_ready_i = 0;
    // Invalid selects.
    t.d->selected_i = 1; t.d->select_write_i = 1; t.d->select_id_i = 4; t.d->select_did_i = 11;
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.d->select_write_i = 0; t.d->select_id_i = 5;
    t.tick();
    assert(t.d->launch_go_o == 0);
    t.d->select_id_i = 4; t.d->select_did_i = 12;
    t.tick();
    assert(t.d->launch_go_o == 0);
    // Valid select.
    t.d->select_did_i = 11;
    t.tick();
    assert(t.d->launch_go_o == 1);
}

static void test_both_done_late_a() {
    Tb t;
    t.configure(false, 0x1000, 0x2000);
    t.configure(true, 0x3000, 0x4000);
    t.launcher_write(0xb8, 1 | 0x100);
    t.reserve(false);
    t.reserve(true);
    t.launcher_write(0xb8, 2);
    t.tick();
    t.d->a_backend_ready_i = 1;
    while (!t.d->a_valid_o) t.tick();
    t.tick();
    t.d->a_backend_ready_i = 0;
    t.d->selected_i = 1; t.d->select_write_i = 0;
    t.d->select_id_i = 4; t.d->select_did_i = 11;
    t.tick();
    t.d->selected_i = 0;
    t.d->v_backend_ready_i = 1;
    while (!t.d->v_valid_o) t.tick();
    t.tick();
    t.d->v_backend_ready_i = 0;
    // V done first.
    t.d->v_trans_complete_i = 1; t.tick(); t.d->v_trans_complete_i = 0;
    t.tick();
    assert((t.launcher_read(0xb8) & 4) == 0);
    // A done late.
    t.d->a_trans_complete_i = 1; t.tick(); t.d->a_trans_complete_i = 0;
    t.tick();
    assert((t.launcher_read(0xb8) & 4) == 4);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    test_basic_pair_flow();
    test_go_then_immediate_abort();
    test_count_backpressure_abort();
    test_partial_reserve_abort();
    test_new_prep_after_aborted_terminal();
    test_v_release_only_valid_a_select();
    test_both_done_late_a();
    std::puts("PASS idma_controlled_pair");
    return 0;
}
```

---

## `tests/test_idma_controlled_pair.py`

```python
#!/usr/bin/env python3
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
cva6 = root / 'cva6'
out = root / 'out/e3-controlled-20260918/frontend-controlled-pair'
out.mkdir(parents=True, exist_ok=True)

prim = cva6 / 'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
files = [
    cva6 / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
    cva6 / 'vendor/pulp-platform/axi/src/axi_pkg.sv',
    cva6 / 'corev_apu/iDMA/src/idma_pkg.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
    prim / 'prim_subreg_arb.sv',
    prim / 'prim_subreg.sv',
    prim / 'prim_subreg_ext.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv',
    cva6 / 'corev_apu/fpga/src/dma_controlled_launcher.sv',
    root / 'tests/idma_controlled_pair_top.sv',
    root / 'tests/idma_controlled_pair_test.cpp',
]

cmd = [
    'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
    '-I' + str(cva6 / 'vendor/pulp-platform/common_cells/include'),
    '-I' + str(cva6 / 'corev_apu/iDMA/src/include'),
    '--top-module', 'idma_controlled_pair_top',
    '--Mdir', str(out),
    *map(str, files),
]

log = out / 'build.log'
with log.open('w') as f:
    r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
if r.returncode:
    print(log.read_text())
    raise SystemExit(r.returncode)

subprocess.run([str(out / 'Vidma_controlled_pair_top')], check=True)
```

---

## Proposed acceptance checks

1. `python3 tests/test_dma_controlled_launcher.py` builds and runs the standalone launcher test. Expected: `PASS dma_controlled_launcher`.
2. `python3 tests/test_dma_observer_finite.py` builds and runs controlled v6 and legacy v5 observer tests. Expected: both pass, and the controlled test checks finite allows A-before-V while legacy rejects finite and still reports the continuous-A error.
3. `python3 tests/test_idma_controlled_pair.py` builds two production `idma_reg64_frontend` instances plus the production launcher. Expected: `PASS idma_controlled_pair`. This test is intentionally written against the frozen updated frontend interface and will fail to compile until the frontend worker lands the `LaunchPort`/8-bit bridge changes.
4. Confirm the FPGA Makefile line 272 glob picks up `dma_controlled_launcher.sv`; no explicit build-list edit was made.
5. Re-run existing observer/iDMA tests, especially the existing batch/observer regressions, to confirm no unrelated behavior changed.
6. In elaboration, confirm only DMA0 legacy writes with `obs_cfg_addr[0][7]==0` reach `dma_observer`, and only DMA0 launcher writes with `[7]==1` reach `dma_controlled_launcher`.

Unresolved/assumption notes:
- The integration top uses local test-only `dma_req_t`, `dma_rsp_t`, and `test_burst_req_t` types so the test can compile against the production frontend. If the wrapper supplies named package types, those typedefs can be replaced by the package types without changing the test intent.
- The frontend files remain unchanged as requested; the real integration test cannot compile until the separate frontend worker updates `idma_reg64_frontend.sv` and the wrapper to the frozen interface.