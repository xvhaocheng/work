Below is a complete proposed patch set for the frozen interface you supplied.

Important assumptions/limits:
- I do not execute or synthesize here. All code is proposed for Codex/parent verification.
- The frontend/wrapper files are deliberately not edited. The top-level patch assumes the wrapper exposes the frozen `LaunchPort` parameter and `launch_*` ports exactly as specified.
- I did not touch FPGA build scripts because the supplied `cva6/corev_apu/fpga/Makefile` does not contain an explicit RTL source list. If your `run.tcl` uses an explicit source list, add `dma_controlled_launcher.sv` there; otherwise rely on the existing glob/list.
- Reset/clear semantics are implemented as described. `status_word` bits are documented in the new module header.
- The observer finite test is provided as new files plus a small standalone runner; no old test expectations were changed.

---

## 1. New file: `cva6/corev_apu/fpga/src/dma_controlled_launcher.sv`

```diff
diff --git a/cva6/corev_apu/fpga/src/dma_controlled_launcher.sv b/cva6/corev_apu/fpga/src/dma_controlled_launcher.sv
new file mode 100644
--- /dev/null
+++ b/cva6/corev_apu/fpga/src/dma_controlled_launcher.sv
@@ -0,0 +1,588 @@
+// SPDX-License-Identifier: Solderpad-0.51
+// Controlled launcher for the iDMA observer-port frontend.
+// This block does not gate AXI. It only observes E3 select, frontend status,
+// and backend issue/done, and drives hold/go/cancel to the controlled ports.
+//
+// Error bits [63:32] are sticky until a successful PREP or CLEAR while inactive.
+// bit  name
+//  0   bad_cfg_strobe          partial cfg_wstrb_i
+//  1   bad_command             unknown or unsupported command
+//  2   bad_prep                PREP not allowed in current state
+//  3   bad_go                  GO not allowed in current state
+//  4   abort_inactive          ABORT while not active/frozen
+//  5   frontend_error          frontend_error_i while owned
+//  6   unexpected_issue        obs_issue_i on unselected port
+//  7   unexpected_done         obs_done_i on unselected port
+//  8   done_without_issue      obs_done_i before obs_issue_i
+//  9   issue_id_mismatch       issue ID not fixed V=0/A=4
+// 10   done_id_mismatch        done ID not fixed V=0/A=4
+// 11   extra_issue             more than one issue on a lane
+// 12   extra_done              more than one done on a lane
+// 13   tick_overflow           64-bit tick counter saturated
+// 14   bad_index               c0 index outside 0..31
+// 15   bad_c8_write            write to read-only c8
+// 16   bad_b0_write            write to read-only b0
+// 17   select_mismatch         reserved for A-select diagnostics
+// 18   cancel_issue_race       reserved for abort/issue race diagnostics
+module dma_controlled_launcher #(
+    parameter int unsigned NumPorts = 4
+) (
+    input  logic clk_i, rst_ni,
+    // 8-bit MMIO cfg window: b0 magic, b8 command/status, c0 index, c8 data.
+    input  logic cfg_valid_i, cfg_write_i,
+    input  logic [7:0] cfg_addr_i,
+    input  logic [63:0] cfg_wdata_i,
+    input  logic [7:0] cfg_wstrb_i,
+    output logic [63:0] cfg_rdata_o,
+    // Controlled frontend status.
+    input  logic [NumPorts-1:0] frontend_held_i,
+    input  logic [NumPorts-1:0] frontend_active_i,
+    input  logic [NumPorts-1:0] frontend_cancelled_i,
+    input  logic [NumPorts-1:0] frontend_error_i,
+    // Actual backend accept/completion.
+    input  logic [NumPorts-1:0] obs_issue_i,
+    input  logic [NumPorts-1:0] obs_done_i,
+    input  logic [63:0] obs_issue_id_i [NumPorts],
+    input  logic [63:0] obs_done_id_i [NumPorts],
+    // E3 select tap.
+    input  logic selected_i,
+    input  logic select_write_i,
+    input  logic [7:0] select_id_i,
+    input  logic [23:0] select_did_i,
+    input  logic [63:0] select_iova_i,
+    // Frontend controls.
+    output logic [NumPorts-1:0] launch_hold_o,
+    output logic [NumPorts-1:0] launch_go_o,
+    output logic [NumPorts-1:0] launch_cancel_o
+);
+    localparam logic [63:0] MAGIC = 64'h49444d414c4e4301;
+    localparam int unsigned V_PORT = 0;
+    localparam int unsigned A_PORT = 1;
+    localparam logic [63:0] V_ID = 64'd0;
+    localparam logic [63:0] A_ID = 64'd4;
+    localparam logic [7:0] A_SEL_ID = 8'd4;
+    localparam logic [23:0] A_DID = 24'd11;
+    localparam logic [63:0] CMD_PREP = 64'h1;
+    localparam logic [63:0] CMD_GO = 64'h2;
+    localparam logic [63:0] CMD_ABORT = 64'h4;
+    localparam logic [63:0] CMD_CLEAR = 64'h8;
+    localparam logic [63:0] CMD_HAS_A = 64'h100;
+
+    localparam int ERR_BAD_STROBE = 0;
+    localparam int ERR_BAD_CMD = 1;
+    localparam int ERR_BAD_PREP = 2;
+    localparam int ERR_BAD_GO = 3;
+    localparam int ERR_ABORT = 4;
+    localparam int ERR_FRONTEND = 5;
+    localparam int ERR_UNEXP_ISSUE = 6;
+    localparam int ERR_UNEXP_DONE = 7;
+    localparam int ERR_DONE_NO_ISSUE = 8;
+    localparam int ERR_ISSUE_ID = 9;
+    localparam int ERR_DONE_ID = 10;
+    localparam int ERR_EXTRA_ISSUE = 11;
+    localparam int ERR_EXTRA_DONE = 12;
+    localparam int ERR_TICK_OVF = 13;
+    localparam int ERR_BAD_INDEX = 14;
+    localparam int ERR_BAD_C8_WRITE = 15;
+    localparam int ERR_BAD_B0_WRITE = 16;
+
+    typedef enum logic [1:0] {ST_OFF, ST_PREP, ST_RUNNING, ST_FROZEN} state_t;
+    state_t state_q, state_n;
+
+    logic [31:0] error_q, error_n;
+    logic [63:0] ticks_q, ticks_n;
+    logic [NumPorts-1:0] expected_mask_q, expected_mask_n;
+    logic [NumPorts-1:0] hold_mask_q, hold_mask_n;
+    logic [NumPorts-1:0] issued_mask_q, issued_mask_n;
+    logic [NumPorts-1:0] done_mask_q, done_mask_n;
+    logic [NumPorts-1:0] cancelled_mask_q, cancelled_mask_n;
+    logic [NumPorts-1:0] seen_held_q, seen_held_n;
+    logic aborted_q, aborted_n;
+    logic has_a_q, has_a_n;
+    logic [7:0] index_q, index_n;
+
+    logic [63:0] v_issue_count_q, v_issue_count_n;
+    logic [63:0] a_issue_count_q, a_issue_count_n;
+    logic [63:0] v_done_count_q, v_done_count_n;
+    logic [63:0] a_done_count_q, a_done_count_n;
+
+    logic [63:0] go_a_ts_q, go_a_ts_n;
+    logic [63:0] a_select_ts_q, a_select_ts_n;
+    logic [63:0] go_v_ts_q, go_v_ts_n;
+    logic [63:0] v_issue_ts_q, v_issue_ts_n;
+    logic [63:0] a_issue_ts_q, a_issue_ts_n;
+    logic [63:0] v_done_ts_q, v_done_ts_n;
+    logic [63:0] a_done_ts_q, a_done_ts_n;
+    logic go_a_ts_valid_q, go_a_ts_valid_n;
+    logic a_select_ts_valid_q, a_select_ts_valid_n;
+    logic go_v_ts_valid_q, go_v_ts_valid_n;
+    logic v_issue_ts_valid_q, v_issue_ts_valid_n;
+    logic a_issue_ts_valid_q, a_issue_ts_valid_n;
+    logic v_done_ts_valid_q, v_done_ts_valid_n;
+    logic a_done_ts_valid_q, a_done_ts_valid_n;
+
+    logic [63:0] v_issue_id_q, v_issue_id_n;
+    logic [63:0] a_issue_id_q, a_issue_id_n;
+    logic [63:0] v_done_id_q, v_done_id_n;
+    logic [63:0] a_done_id_q, a_done_id_n;
+    logic v_issue_id_valid_q, v_issue_id_valid_n;
+    logic a_issue_id_valid_q, a_issue_id_valid_n;
+    logic v_done_id_valid_q, v_done_id_valid_n;
+    logic a_done_id_valid_q, a_done_id_valid_n;
+
+    logic [7:0] selected_id_q, selected_id_n;
+    logic [23:0] selected_did_q, selected_did_n;
+    logic [63:0] selected_iova_q, selected_iova_n;
+    logic selected_write_q, selected_write_n;
+
+    logic [NumPorts-1:0] go_pulse_q, go_pulse_n;
+    logic a_go_sent_q, a_go_sent_n;
+    logic v_go_sent_q, v_go_sent_n;
+    logic a_select_seen_q, a_select_seen_n;
+
+    logic [63:0] status_word;
+
+    assign launch_hold_o = hold_mask_q;
+    assign launch_go_o = go_pulse_q;
+
+    always_comb begin : status_comb
+        status_word = '0;
+        status_word[63:32] = error_q;
+        status_word[15:12] = 4'(frontend_active_i);
+        status_word[11:8]  = 4'(frontend_held_i);
+        status_word[3] = aborted_q;
+        status_word[2] = (state_q == ST_FROZEN) && !aborted_q;
+        status_word[1] = (state_q == ST_RUNNING);
+        status_word[0] = (state_q == ST_PREP) || (state_q == ST_RUNNING);
+    end
+
+    always_comb begin : cancel_comb
+        launch_cancel_o = '0;
+        if (aborted_q && state_q == ST_RUNNING) begin
+            for (int p = 0; p < NumPorts; p++) begin
+                if (expected_mask_q[p] && hold_mask_q[p] && frontend_held_i[p] &&
+                    !obs_issue_i[p] && !cancelled_mask_q[p]) begin
+                    launch_cancel_o[p] = 1'b1;
+                end
+            end
+        end
+    end
+
+    always_comb begin : read_comb
+        cfg_rdata_o = '0;
+        case (cfg_addr_i)
+            8'hb0: cfg_rdata_o = MAGIC;
+            8'hb8: cfg_rdata_o = status_word;
+            8'hc0: cfg_rdata_o = {56'b0, index_q};
+            8'hc8: begin
+                case (index_q)
+                    8'd0:  cfg_rdata_o = MAGIC;
+                    8'd1:  cfg_rdata_o = {32'b0, error_q};
+                    8'd2:  cfg_rdata_o = status_word;
+                    8'd3:  cfg_rdata_o = ticks_q;
+                    8'd4:  cfg_rdata_o = go_a_ts_q;
+                    8'd5:  cfg_rdata_o = a_select_ts_q;
+                    8'd6:  cfg_rdata_o = go_v_ts_q;
+                    8'd7:  cfg_rdata_o = v_issue_ts_q;
+                    8'd8:  cfg_rdata_o = a_issue_ts_q;
+                    8'd9:  cfg_rdata_o = v_done_ts_q;
+                    8'd10: cfg_rdata_o = a_done_ts_q;
+                    8'd11: cfg_rdata_o = v_issue_id_q;
+                    8'd12: cfg_rdata_o = a_issue_id_q;
+                    8'd13: cfg_rdata_o = v_done_id_q;
+                    8'd14: cfg_rdata_o = a_done_id_q;
+                    8'd15: cfg_rdata_o = 64'(issued_mask_q);
+                    8'd16: cfg_rdata_o = 64'(done_mask_q);
+                    8'd17: cfg_rdata_o = 64'(cancelled_mask_q);
+                    8'd18: cfg_rdata_o = 64'(seen_held_q);
+                    8'd19: cfg_rdata_o = 64'(expected_mask_q);
+                    8'd20: cfg_rdata_o = 64'(selected_id_q);
+                    8'd21: cfg_rdata_o = 64'(selected_did_q);
+                    8'd22: cfg_rdata_o = selected_iova_q;
+                    8'd23: cfg_rdata_o = {63'b0, selected_write_q};
+                    8'd24: cfg_rdata_o = v_issue_count_q;
+                    8'd25: cfg_rdata_o = a_issue_count_q;
+                    8'd26: cfg_rdata_o = v_done_count_q;
+                    8'd27: cfg_rdata_o = a_done_count_q;
+                    8'd28: cfg_rdata_o = {63'b0, has_a_q};
+                    8'd29: cfg_rdata_o = 64'(frontend_held_i);
+                    8'd30: cfg_rdata_o = 64'(frontend_active_i);
+                    8'd31: cfg_rdata_o = '0;
+                    default: cfg_rdata_o = '0;
+                endcase
+            end
+            default: cfg_rdata_o = '0;
+        endcase
+    end
+
+    always_comb begin : main_comb
+        // defaults
+        state_n = state_q;
+        error_n = error_q;
+        ticks_n = ticks_q;
+        expected_mask_n = expected_mask_q;
+        hold_mask_n = hold_mask_q;
+        issued_mask_n = issued_mask_q;
+        done_mask_n = done_mask_q;
+        cancelled_mask_n = cancelled_mask_q;
+        seen_held_n = seen_held_q;
+        aborted_n = aborted_q;
+        has_a_n = has_a_q;
+        index_n = index_q;
+        v_issue_count_n = v_issue_count_q;
+        a_issue_count_n = a_issue_count_q;
+        v_done_count_n = v_done_count_q;
+        a_done_count_n = a_done_count_q;
+        go_a_ts_n = go_a_ts_q;
+        a_select_ts_n = a_select_ts_q;
+        go_v_ts_n = go_v_ts_q;
+        v_issue_ts_n = v_issue_ts_q;
+        a_issue_ts_n = a_issue_ts_q;
+        v_done_ts_n = v_done_ts_q;
+        a_done_ts_n = a_done_ts_q;
+        go_a_ts_valid_n = go_a_ts_valid_q;
+        a_select_ts_valid_n = a_select_ts_valid_q;
+        go_v_ts_valid_n = go_v_ts_valid_q;
+        v_issue_ts_valid_n = v_issue_ts_valid_q;
+        a_issue_ts_valid_n = a_issue_ts_valid_q;
+        v_done_ts_valid_n = v_done_ts_valid_q;
+        a_done_ts_valid_n = a_done_ts_valid_q;
+        v_issue_id_n = v_issue_id_q;
+        a_issue_id_n = a_issue_id_q;
+        v_done_id_n = v_done_id_q;
+        a_done_id_n = a_done_id_q;
+        v_issue_id_valid_n = v_issue_id_valid_q;
+        a_issue_id_valid_n = a_issue_id_valid_q;
+        v_done_id_valid_n = v_done_id_valid_q;
+        a_done_id_valid_n = a_done_id_valid_q;
+        selected_id_n = selected_id_q;
+        selected_did_n = selected_did_q;
+        selected_iova_n = selected_iova_q;
+        selected_write_n = selected_write_q;
+        go_pulse_n = '0;
+        a_go_sent_n = a_go_sent_q;
+        v_go_sent_n = v_go_sent_q;
+        a_select_seen_n = a_select_seen_q;
+
+        // Tick counter while owned. Saturates and sets sticky fault.
+        if (state_q == ST_PREP || state_q == ST_RUNNING) begin
+            if (ticks_q == {64{1'b1}}) begin
+                error_n[ERR_TICK_OVF] = 1'b1;
+            end else begin
+                ticks_n = ticks_q + 1'b1;
+            end
+        end
+
+        // Track held seen while owned.
+        if (state_q == ST_PREP || state_q == ST_RUNNING) begin
+            seen_held_n = seen_held_q | frontend_held_i;
+        end
+
+        // Record frontend error while owned.
+        if ((state_q == ST_PREP || state_q == ST_RUNNING) &&
+            |(frontend_error_i & expected_mask_q)) begin
+            error_n[ERR_FRONTEND] = 1'b1;
+        end
+
+        // Actual backend events.
+        if (state_q == ST_PREP || state_q == ST_RUNNING) begin
+            if (|(obs_issue_i & ~expected_mask_q)) error_n[ERR_UNEXP_ISSUE] = 1'b1;
+            if (|(obs_done_i & ~expected_mask_q)) error_n[ERR_UNEXP_DONE] = 1'b1;
+            for (int p = 0; p < NumPorts; p++) begin
+                if (obs_issue_i[p] && expected_mask_q[p]) begin
+                    if (issued_mask_q[p]) error_n[ERR_EXTRA_ISSUE] = 1'b1;
+                    if (p == V_PORT && obs_issue_id_i[p] != V_ID) error_n[ERR_ISSUE_ID] = 1'b1;
+                    if (p == A_PORT && obs_issue_id_i[p] != A_ID) error_n[ERR_ISSUE_ID] = 1'b1;
+                    issued_mask_n[p] = 1'b1;
+                    if (p == V_PORT) begin
+                        v_issue_count_n = v_issue_count_q + 1'b1;
+                        if (!v_issue_ts_valid_q) begin v_issue_ts_n = ticks_q; v_issue_ts_valid_n = 1'b1; end
+                        if (!v_issue_id_valid_q) begin v_issue_id_n = obs_issue_id_i[p]; v_issue_id_valid_n = 1'b1; end
+                    end else if (p == A_PORT) begin
+                        a_issue_count_n = a_issue_count_q + 1'b1;
+                        if (!a_issue_ts_valid_q) begin a_issue_ts_n = ticks_q; a_issue_ts_valid_n = 1'b1; end
+                        if (!a_issue_id_valid_q) begin a_issue_id_n = obs_issue_id_i[p]; a_issue_id_valid_n = 1'b1; end
+                    end
+                end
+                if (obs_done_i[p] && expected_mask_q[p]) begin
+                    if (!issued_mask_q[p]) error_n[ERR_DONE_NO_ISSUE] = 1'b1;
+                    if (done_mask_q[p]) error_n[ERR_EXTRA_DONE] = 1'b1;
+                    if (p == V_PORT && obs_done_id_i[p] != V_ID) error_n[ERR_DONE_ID] = 1'b1;
+                    if (p == A_PORT && obs_done_id_i[p] != A_ID) error_n[ERR_DONE_ID] = 1'b1;
+                    done_mask_n[p] = 1'b1;
+                    if (p == V_PORT) begin
+                        v_done_count_n = v_done_count_q + 1'b1;
+                        if (!v_done_ts_valid_q) begin v_done_ts_n = ticks_q; v_done_ts_valid_n = 1'b1; end
+                        if (!v_done_id_valid_q) begin v_done_id_n = obs_done_id_i[p]; v_done_id_valid_n = 1'b1; end
+                    end else if (p == A_PORT) begin
+                        a_done_count_n = a_done_count_q + 1'b1;
+                        if (!a_done_ts_valid_q) begin a_done_ts_n = ticks_q; a_done_ts_valid_n = 1'b1; end
+                        if (!a_done_id_valid_q) begin a_done_id_n = obs_done_id_i[p]; a_done_id_valid_n = 1'b1; end
+                    end
+                end
+            end
+        end
+
+        // Command writes.
+        if (cfg_valid_i && cfg_write_i) begin
+            if (cfg_wstrb_i != 8'hff) begin
+                error_n[ERR_BAD_STROBE] = 1'b1;
+            end else begin
+                case (cfg_addr_i)
+                    8'hb0: error_n[ERR_BAD_B0_WRITE] = 1'b1;
+                    8'hb8: begin
+                        if ((cfg_wdata_i & ~(CMD_HAS_A)) == CMD_PREP) begin
+                            if ((state_q == ST_OFF || (state_q == ST_FROZEN && !aborted_q)) &&
+                                frontend_active_i == '0 && frontend_held_i == '0) begin
+                                state_n = ST_PREP;
+                                aborted_n = 1'b0;
+                                error_n = '0;
+                                has_a_n = cfg_wdata_i[8];
+                                expected_mask_n = '0;
+                                expected_mask_n[V_PORT] = 1'b1;
+                                expected_mask_n[A_PORT] = cfg_wdata_i[8];
+                                hold_mask_n = expected_mask_n;
+                                issued_mask_n = '0;
+                                done_mask_n = '0;
+                                cancelled_mask_n = '0;
+                                seen_held_n = '0;
+                                ticks_n = '0;
+                                v_issue_count_n = '0;
+                                a_issue_count_n = '0;
+                                v_done_count_n = '0;
+                                a_done_count_n = '0;
+                                go_a_ts_n = '0; a_select_ts_n = '0; go_v_ts_n = '0;
+                                v_issue_ts_n = '0; a_issue_ts_n = '0; v_done_ts_n = '0; a_done_ts_n = '0;
+                                go_a_ts_valid_n = 1'b0; a_select_ts_valid_n = 1'b0; go_v_ts_valid_n = 1'b0;
+                                v_issue_ts_valid_n = 1'b0; a_issue_ts_valid_n = 1'b0;
+                                v_done_ts_valid_n = 1'b0; a_done_ts_valid_n = 1'b0;
+                                v_issue_id_n = '0; a_issue_id_n = '0; v_done_id_n = '0; a_done_id_n = '0;
+                                v_issue_id_valid_n = 1'b0; a_issue_id_valid_n = 1'b0;
+                                v_done_id_valid_n = 1'b0; a_done_id_valid_n = 1'b0;
+                                selected_id_n = '0; selected_did_n = '0; selected_iova_n = '0; selected_write_n = 1'b0;
+                                go_pulse_n = '0;
+                                a_go_sent_n = 1'b0; v_go_sent_n = 1'b0; a_select_seen_n = 1'b0;
+                            end else begin
+                                error_n[ERR_BAD_PREP] = 1'b1;
+                            end
+                        end else if (cfg_wdata_i == CMD_GO) begin
+                            if (state_q == ST_PREP && error_q == 0 &&
+                                (frontend_held_i & expected_mask_q) == expected_mask_q) begin
+                                state_n = ST_RUNNING;
+                                if (has_a_q) begin
+                                    go_pulse_n[A_PORT] = 1'b1;
+                                    a_go_sent_n = 1'b1;
+                                    if (!go_a_ts_valid_q) begin
+                                        go_a_ts_n = ticks_q + 1'b1;
+                                        go_a_ts_valid_n = 1'b1;
+                                    end
+                                end else begin
+                                    go_pulse_n[V_PORT] = 1'b1;
+                                    v_go_sent_n = 1'b1;
+                                    if (!go_v_ts_valid_q) begin
+                                        go_v_ts_n = ticks_q + 1'b1;
+                                        go_v_ts_valid_n = 1'b1;
+                                    end
+                                end
+                            end else begin
+                                error_n[ERR_BAD_GO] = 1'b1;
+                            end
+                        end else if (cfg_wdata_i == CMD_ABORT) begin
+                            if (state_q == ST_OFF) begin
+                                state_n = ST_FROZEN;
+                                aborted_n = 1'b1;
+                                hold_mask_n = '0;
+                            end else if (state_q == ST_PREP || state_q == ST_RUNNING) begin
+                                aborted_n = 1'b1;
+                                state_n = ST_RUNNING;
+                            end else begin
+                                error_n[ERR_ABORT] = 1'b1;
+                            end
+                        end else if (cfg_wdata_i == CMD_CLEAR) begin
+                            if (state_q == ST_OFF || state_q == ST_FROZEN) begin
+                                error_n = '0;
+                            end else begin
+                                error_n[ERR_BAD_CMD] = 1'b1;
+                            end
+                        end else begin
+                            error_n[ERR_BAD_CMD] = 1'b1;
+                        end
+                    end
+                    8'hc0: begin
+                        if (cfg_wdata_i < 64'd32) index_n = cfg_wdata_i[7:0];
+                        else error_n[ERR_BAD_INDEX] = 1'b1;
+                    end
+                    8'hc8: error_n[ERR_BAD_C8_WRITE] = 1'b1;
+                    default: error_n[ERR_BAD_CMD] = 1'b1;
+                endcase
+            end
+        end
+
+        // A select capture and V go, but ABORT wins same-edge.
+        if (!aborted_n && state_q == ST_RUNNING && has_a_q && !a_select_seen_q && !v_go_sent_q) begin
+            if (selected_i && !select_write_i && select_id_i == A_SEL_ID && select_did_i == A_DID &&
+                (obs_issue_i[A_PORT] || issued_mask_q[A_PORT])) begin
+                a_select_seen_n = 1'b1;
+                selected_id_n = select_id_i;
+                selected_did_n = select_did_i;
+                selected_iova_n = select_iova_i;
+                selected_write_n = select_write_i;
+                if (!a_select_ts_valid_q) begin
+                    a_select_ts_n = ticks_q;
+                    a_select_ts_valid_n = 1'b1;
+                end
+                go_pulse_n[V_PORT] = 1'b1;
+                v_go_sent_n = 1'b1;
+                if (!go_v_ts_valid_q) begin
+                    go_v_ts_n = ticks_q + 1'b1;
+                    go_v_ts_valid_n = 1'b1;
+                end
+            end
+        end
+
+        // Abort drain: cancel remaining HOLD and record cancellations.
+        if (aborted_q && state_q == ST_RUNNING) begin
+            for (int p = 0; p < NumPorts; p++) begin
+                if (expected_mask_q[p] && hold_mask_q[p]) begin
+                    if (obs_issue_i[p]) begin
+                        hold_mask_n[p] = 1'b0;
+                    end else if (frontend_cancelled_i[p]) begin
+                        cancelled_mask_n[p] = 1'b1;
+                        hold_mask_n[p] = 1'b0;
+                    end else if (seen_held_q[p] && !frontend_held_i[p] &&
+                                 !issued_mask_q[p] && !done_mask_q[p]) begin
+                        cancelled_mask_n[p] = 1'b1;
+                        hold_mask_n[p] = 1'b0;
+                    end
+                end
+            end
+        end
+
+        // Finish conditions, only after drain.
+        if (state_q == ST_RUNNING && !aborted_q) begin
+            if (issued_mask_q == expected_mask_q && done_mask_q == expected_mask_q &&
+                (frontend_held_i & expected_mask_q) == '0 &&
+                (frontend_active_i & expected_mask_q) == '0) begin
+                state_n = ST_FROZEN;
+                hold_mask_n = '0;
+            end
+        end
+        if (state_q == ST_RUNNING && aborted_q) begin
+            if ((frontend_held_i & expected_mask_q) == '0 &&
+                (frontend_active_i & expected_mask_q) == '0 &&
+                (issued_mask_q & ~done_mask_q) == '0 &&
+                ((expected_mask_q & ~issued_mask_q & ~cancelled_mask_q & seen_held_q) == '0)) begin
+                state_n = ST_FROZEN;
+                hold_mask_n = '0;
+            end
+        end
+    end
+
+    always_ff @(posedge clk_i or negedge rst_ni) begin
+        if (!rst_ni) begin
+            state_q <= ST_OFF;
+            error_q <= '0;
+            ticks_q <= '0;
+            expected_mask_q <= '0;
+            hold_mask_q <= '0;
+            issued_mask_q <= '0;
+            done_mask_q <= '0;
+            cancelled_mask_q <= '0;
+            seen_held_q <= '0;
+            aborted_q <= 1'b0;
+            has_a_q <= 1'b0;
+            index_q <= '0;
+            v_issue_count_q <= '0;
+            a_issue_count_q <= '0;
+            v_done_count_q <= '0;
+            a_done_count_q <= '0;
+            go_a_ts_q <= '0; a_select_ts_q <= '0; go_v_ts_q <= '0;
+            v_issue_ts_q <= '0; a_issue_ts_q <= '0; v_done_ts_q <= '0; a_done_ts_q <= '0;
+            go_a_ts_valid_q <= 1'b0; a_select_ts_valid_q <= 1'b0; go_v_ts_valid_q <= 1'b0;
+            v_issue_ts_valid_q <= 1'b0; a_issue_ts_valid_q <= 1'b0;
+            v_done_ts_valid_q <= 1'b0; a_done_ts_valid_q <= 1'b0;
+            v_issue_id_q <= '0; a_issue_id_q <= '0; v_done_id_q <= '0; a_done_id_q <= '0;
+            v_issue_id_valid_q <= 1'b0; a_issue_id_valid_q <= 1'b0;
+            v_done_id_valid_q <= 1'b0; a_done_id_valid_q <= 1'b0;
+            selected_id_q <= '0; selected_did_q <= '0; selected_iova_q <= '0; selected_write_q <= 1'b0;
+            go_pulse_q <= '0;
+            a_go_sent_q <= 1'b0; v_go_sent_q <= 1'b0; a_select_seen_q <= 1'b0;
+        end else begin
+            state_q <= state_n;
+            error_q <= error_n;
+            ticks_q <= ticks_n;
+            expected_mask_q <= expected_mask_n;
+            hold_mask_q <= hold_mask_n;
+            issued_mask_q <= issued_mask_n;
+            done_mask_q <= done_mask_n;
+            cancelled_mask_q <= cancelled_mask_n;
+            seen_held_q <= seen_held_n;
+            aborted_q <= aborted_n;
+            has_a_q <= has_a_n;
+            index_q <= index_n;
+            v_issue_count_q <= v_issue_count_n;
+            a_issue_count_q <= a_issue_count_n;
+            v_done_count_q <= v_done_count_n;
+            a_done_count_q <= a_done_count_n;
+            go_a_ts_q <= go_a_ts_n; a_select_ts_q <= a_select_ts_n; go_v_ts_q <= go_v_ts_n;
+            v_issue_ts_q <= v_issue_ts_n; a_issue_ts_q <= a_issue_ts_n;
+            v_done_ts_q <= v_done_ts_n; a_done_ts_q <= a_done_ts_n;
+            go_a_ts_valid_q <= go_a_ts_valid_n; a_select_ts_valid_q <= a_select_ts_valid_n;
+            go_v_ts_valid_q <= go_v_ts_valid_n;
+            v_issue_ts_valid_q <= v_issue_ts_valid_n; a_issue_ts_valid_q <= a_issue_ts_valid_n;
+            v_done_ts_valid_q <= v_done_ts_valid_n; a_done_ts_valid_q <= a_done_ts_valid_n;
+            v_issue_id_q <= v_issue_id_n; a_issue_id_q <= a_issue_id_n;
+            v_done_id_q <= v_done_id_n; a_done_id_q <= a_done_id_n;
+            v_issue_id_valid_q <= v_issue_id_valid_n; a_issue_id_valid_q <= a_issue_id_valid_n;
+            v_done_id_valid_q <= v_done_id_valid_n; a_done_id_valid_q <= a_done_id_valid_n;
+            selected_id_q <= selected_id_n; selected_did_q <= selected_did_n;
+            selected_iova_q <= selected_iova_n; selected_write_q <= selected_write_n;
+            go_pulse_q <= go_pulse_n;
+            a_go_sent_q <= a_go_sent_n; v_go_sent_q <= v_go_sent_n; a_select_seen_q <= a_select_seen_n;
+        end
+    end
+endmodule
```

---

## 2. Top integration: `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`

```diff
diff --git a/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv b/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv
--- a/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv
+++ b/cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv
@@ -910,8 +910,11 @@
     if (InclDMA) begin : gen_dma
         logic [NumDMADevices-1:0] obs_issue, obs_done, obs_poll, batch_active;
         logic [63:0] obs_issue_id [NumDMADevices], obs_done_id [NumDMADevices], obs_poll_id [NumDMADevices];
         logic [NumDMADevices-1:0] obs_cfg_valid, obs_cfg_write;
-        logic [6:0] obs_cfg_addr [NumDMADevices];
-        logic [63:0] obs_cfg_wdata [NumDMADevices], obs_cfg_rdata;
+        logic [7:0] obs_cfg_addr [NumDMADevices];
+        logic [63:0] obs_cfg_wdata [NumDMADevices], legacy_cfg_rdata, launch_cfg_rdata, obs_cfg_rdata;
         logic [7:0] obs_cfg_wstrb [NumDMADevices];
+        logic [NumDMADevices-1:0] launch_hold, launch_go, launch_cancel;
+        logic [NumDMADevices-1:0] launch_held, launch_active, launch_cancelled, launch_error;
         for (genvar o = 0; o < NumDMADevices; o++) begin : gen_dma_observation
             assign obs_arvalid[o] = axi_dma_tr_req[o].ar_valid;
             assign obs_awvalid[o] = axi_dma_tr_req[o].aw_valid;
@@ -932,11 +935,13 @@
-        dma_observer #(.NumPorts(NumDMADevices), .PathEnable(InclIOMMU),
-                       .E3Enable(InclIOMMU),
+        dma_observer #(.NumPorts(NumDMADevices), .PathEnable(InclIOMMU),
+                       .E3Enable(InclIOMMU), .ControlledEnable(InclIOMMU),
                        .AxiIdWidth(ariane_soc::IdWidth)) i_dma_observer (
             .clk_i, .rst_ni,
-            .cfg_valid_i(obs_cfg_valid[0]), .cfg_write_i(obs_cfg_write[0]),
-            .cfg_addr_i(obs_cfg_addr[0]), .cfg_wdata_i(obs_cfg_wdata[0]),
-            .cfg_wstrb_i(obs_cfg_wstrb[0]), .cfg_rdata_o(obs_cfg_rdata),
+            .cfg_valid_i(obs_cfg_valid[0] && !obs_cfg_addr[0][7]),
+            .cfg_write_i(obs_cfg_write[0]),
+            .cfg_addr_i(obs_cfg_addr[0][6:0]), .cfg_wdata_i(obs_cfg_wdata[0]),
+            .cfg_wstrb_i(obs_cfg_wstrb[0]), .cfg_rdata_o(legacy_cfg_rdata),
             .e3_data_i(e3_data), .e3_error_i(e3_error), .e3_control_o(e3_control), .e3_index_o(e3_index),
             .issue_i(obs_issue), .done_i(obs_done), .poll_i(obs_poll),
             .batch_active_i(batch_active),
@@ -955,6 +960,37 @@
             .return_i({axi_iommu_comp_resp.b_valid && axi_iommu_comp_req.b_ready,
                        axi_iommu_comp_resp.r_valid && axi_iommu_comp_resp.r.last && axi_iommu_comp_req.r_ready})
         );
+
+        assign obs_cfg_rdata = obs_cfg_addr[0][7] ? launch_cfg_rdata : legacy_cfg_rdata;
+        if (InclIOMMU) begin : gen_launcher
+            dma_controlled_launcher #(.NumPorts(NumDMADevices)) i_launcher (
+                .clk_i, .rst_ni,
+                .cfg_valid_i(obs_cfg_valid[0] && obs_cfg_addr[0][7]),
+                .cfg_write_i(obs_cfg_write[0]),
+                .cfg_addr_i(obs_cfg_addr[0]),
+                .cfg_wdata_i(obs_cfg_wdata[0]),
+                .cfg_wstrb_i(obs_cfg_wstrb[0]),
+                .cfg_rdata_o(launch_cfg_rdata),
+                .frontend_held_i(launch_held),
+                .frontend_active_i(launch_active),
+                .frontend_cancelled_i(launch_cancelled),
+                .frontend_error_i(launch_error),
+                .obs_issue_i(obs_issue), .obs_done_i(obs_done),
+                .obs_issue_id_i(obs_issue_id), .obs_done_id_i(obs_done_id),
+                .selected_i(e3_ctl.select_valid),
+                .select_write_i(e3_ctl.select_write),
+                .select_id_i(e3_ctl.select_id),
+                .select_did_i(e3_ctl.select_did),
+                .select_iova_i(e3_ctl.select_iova),
+                .launch_hold_o(launch_hold),
+                .launch_go_o(launch_go),
+                .launch_cancel_o(launch_cancel)
+            );
+        end else begin : gen_no_launcher
+            assign launch_hold = '0;
+            assign launch_go = '0;
+            assign launch_cancel = '0;
+            assign launch_cfg_rdata = '0;
+        end
 
@@ -1046,10 +1082,11 @@
                 .RAW_COUPLING_AVAIL ( 0                        ),
                 .IS_TWO_D           ( 0                        ),
                 .BATCH_PORT (1'b1),
                 .OBSERVER_PORT      ( i == 0                   ),
+                .LaunchPort         ( InclIOMMU && i < 2       ),
 
                 .DEVICE_ID          ( 24'(10 + i)              ),
                 .PROCESS_ID         ( 20'(20 + i)              ),
                 .AxID               ( '0                       )
             ) i_dma (
@@ -1064,6 +1101,13 @@
                 .obs_cfg_valid_o(obs_cfg_valid[i]), .obs_cfg_write_o(obs_cfg_write[i]),
                 .obs_cfg_addr_o(obs_cfg_addr[i]), .obs_cfg_wdata_o(obs_cfg_wdata[i]),
                 .obs_cfg_wstrb_o(obs_cfg_wstrb[i]), .obs_cfg_rdata_i(obs_cfg_rdata),
+                .launch_hold_i(launch_hold[i]),
+                .launch_go_i(launch_go[i]),
+                .launch_cancel_i(launch_cancel[i]),
+                .launch_held_o(launch_held[i]),
+                .launch_active_o(launch_active[i]),
+                .launch_cancelled_o(launch_cancelled[i]),
+                .launch_error_o(launch_error[i]),
                 .clk_i          ( clk_i              ),
                 .rst_ni         ( rst_ni             ),
                 .testmode_i     ( 1'b0               ),
```

---

## 3. Observer change: `cva6/corev_apu/fpga/src/dma_observer.sv`

```diff
diff --git a/cva6/corev_apu/fpga/src/dma_observer.sv b/cva6/corev_apu/fpga/src/dma_observer.sv
--- a/cva6/corev_apu/fpga/src/dma_observer.sv
+++ b/cva6/corev_apu/fpga/src/dma_observer.sv
@@ -5,6 +5,7 @@
     parameter int unsigned OutstandingWidth = 8,
     parameter bit PathEnable = 0,
     parameter bit E3Enable = 0,
+    parameter bit ControlledEnable = 0,
     parameter int AxiIdWidth = 4
 ) (
@@ -30,8 +31,9 @@
     output logic [7:0] e3_control_o, e3_index_o
 );
     // Low byte is interface version; a missing observer must never look valid.
-    localparam logic [63:0] Magic = E3Enable ? 64'h49444d414f425305 :
-        (PathEnable ? 64'h49444d414f425303 : 64'h49444d414f425302);
+    localparam logic [63:0] Magic = (ControlledEnable && E3Enable) ? 64'h49444d414f425306 :
+        (E3Enable ? 64'h49444d414f425305 :
+        (PathEnable ? 64'h49444d414f425303 : 64'h49444d414f425302));
     localparam int Count = 35;
@@ -47,6 +49,7 @@
     logic [1:0] victim_q, victim_n, aggressor_q, aggressor_n;
     logic has_a_q, has_a_n, batch_q, batch_n;
+    logic finite_q, finite_n;
     logic busy;
@@ -117,6 +120,7 @@
         target_n = target_q;
         index_n = index_q;
         victim_n = victim_q;
         aggressor_n = aggressor_q;
         has_a_n = has_a_q;
         batch_n = batch_q;
+        finite_n = finite_q;
         path_n = path_q;
@@ -173,7 +177,7 @@
         if (phase_q == ARMED || active) begin
             if (track_bad_n[victim_q] || (has_a_q && track_bad_n[aggressor_q]))
                 error_n[3] = 1'b1;
         end
-        if (batch_q && (active || phase_q == ARMED && issue_i[victim_q]) &&
+        if (!finite_q && batch_q && (active || phase_q == ARMED && issue_i[victim_q]) &&
             has_a_q && !batch_active_i[aggressor_q]) error_n[5] = 1'b1;
@@ -253,7 +257,7 @@
         if (cfg_valid_i && cfg_write_i) begin
             if (cfg_wstrb_i != 8'hff) error_n[0] = 1'b1;
             else case (cfg_addr_i)
@@ -268,11 +272,13 @@
                     } else if (cfg_wdata_i == 4) begin
                         if (busy || phase_q == ARMED) error_n[0] = 1'b1;
                         else begin phase_n = OFF; error_n = 0; path_clear = 1; path_n = 0; e3_n = 0; end
-                    } else if ((cfg_wdata_i & ~64'h000000001f030301) == 0 && cfg_wdata_i[0] &&
+                    } else if ((cfg_wdata_i & ~(64'h000000001f030301 | (ControlledEnable && E3Enable ? 64'h0000000020000000 : 64'h0))) == 0 && cfg_wdata_i[0] &&
+                                 (!cfg_wdata_i[29] || (ControlledEnable && E3Enable && cfg_wdata_i[26] && cfg_wdata_i[25] &&
+                                    cfg_wdata_i[9:8] == 0 && cfg_wdata_i[17:16] == 1)) &&
                                  (!cfg_wdata_i[28] || cfg_wdata_i[27]) &&
                                  (!cfg_wdata_i[27] || (E3Enable && cfg_wdata_i[26] && cfg_wdata_i[9:8] == 0 &&
                                     (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1))) &&
@@ -285,6 +291,7 @@
                         has_a_n = cfg_wdata_i[24];
                         batch_n = cfg_wdata_i[25];
+                        finite_n = cfg_wdata_i[29];
                         path_n = cfg_wdata_i[26];
                         path_arm = cfg_wdata_i[26];
                         e3_n = cfg_wdata_i[27];
@@ -310,6 +317,7 @@
             has_a_q <= 0;
             batch_q <= 0;
+            finite_q <= 0;
             path_q <= 0;
             e3_q <= 0;
@@ -326,6 +334,7 @@
             has_a_q <= has_a_n;
             batch_q <= batch_n;
+            finite_q <= finite_n;
             path_q <= path_n;
             e3_q <= e3_n;
```

---

## 4. Test wrapper: `tests/dma_controlled_launcher_top.sv`

```diff
diff --git a/tests/dma_controlled_launcher_top.sv b/tests/dma_controlled_launcher_top.sv
new file mode 100644
--- /dev/null
+++ b/tests/dma_controlled_launcher_top.sv
@@ -0,0 +1,55 @@
+module dma_controlled_launcher_top #(
+    parameter int unsigned NumPorts = 4
+) (
+    input  logic clk_i, rst_ni,
+    input  logic cfg_valid_i, cfg_write_i,
+    input  logic [7:0] cfg_addr_i,
+    input  logic [63:0] cfg_wdata_i,
+    input  logic [7:0] cfg_wstrb_i,
+    output logic [63:0] cfg_rdata_o,
+    input  logic [NumPorts-1:0] frontend_held_i,
+    input  logic [NumPorts-1:0] frontend_active_i,
+    input  logic [NumPorts-1:0] frontend_cancelled_i,
+    input  logic [NumPorts-1:0] frontend_error_i,
+    input  logic [NumPorts-1:0] obs_issue_i, obs_done_i,
+    input  logic [NumPorts*64-1:0] obs_issue_id_i_flat,
+    input  logic [NumPorts*64-1:0] obs_done_id_i_flat,
+    input  logic selected_i, select_write_i,
+    input  logic [7:0] select_id_i,
+    input  logic [23:0] select_did_i,
+    input  logic [63:0] select_iova_i,
+    output logic [NumPorts-1:0] launch_hold_o,
+    output logic [NumPorts-1:0] launch_go_o,
+    output logic [NumPorts-1:0] launch_cancel_o
+);
+    logic [63:0] obs_issue_id_i [NumPorts];
+    logic [63:0] obs_done_id_i [NumPorts];
+
+    for (genvar p = 0; p < NumPorts; p++) begin : gen_unflatten
+        assign obs_issue_id_i[p] = obs_issue_id_i_flat[p*64 +: 64];
+        assign obs_done_id_i[p]  = obs_done_id_i_flat[p*64 +: 64];
+    end
+
+    dma_controlled_launcher #(.NumPorts(NumPorts)) i_dut (
+        .clk_i, .rst_ni,
+        .cfg_valid_i, .cfg_write_i, .cfg_addr_i, .cfg_wdata_i, .cfg_wstrb_i, .cfg_rdata_o,
+        .frontend_held_i, .frontend_active_i, .frontend_cancelled_i, .frontend_error_i,
+        .obs_issue_i, .obs_done_i, .obs_issue_id_i, .obs_done_id_i,
+        .selected_i, .select_write_i, .select_id_i, .select_did_i, .select_iova_i,
+        .launch_hold_o, .launch_go_o, .launch_cancel_o
+    );
+endmodule
```

---

## 5. C++ test: `tests/dma_controlled_launcher_test.cpp`

```diff
diff --git a/tests/dma_controlled_launcher_test.cpp b/tests/dma_controlled_launcher_test.cpp
new file mode 100644
--- /dev/null
+++ b/tests/dma_controlled_launcher_test.cpp
@@ -0,0 +1,228 @@
+#include "Vdma_controlled_launcher_top.h"
+#include <verilated.h>
+#include <cassert>
+#include <cstdio>
+#include <stdexcept>
+
+static constexpr uint64_t MAGIC = 0x49444d414c4e4301ULL;
+
+struct Dut {
+    Vdma_controlled_launcher_top *d;
+
+    Dut() {
+        d = new Vdma_controlled_launcher_top;
+        reset();
+    }
+    ~Dut() { delete d; }
+
+    void tick() {
+        d->clk_i = 0; d->eval();
+        d->clk_i = 1; d->eval();
+        d->clk_i = 0; d->eval();
+    }
+    void reset() {
+        d->rst_ni = 0;
+        d->cfg_valid_i = 0; d->cfg_write_i = 0;
+        d->cfg_addr_i = 0; d->cfg_wdata_i = 0; d->cfg_wstrb_i = 0xff;
+        d->frontend_held_i = 0; d->frontend_active_i = 0;
+        d->frontend_cancelled_i = 0; d->frontend_error_i = 0;
+        d->obs_issue_i = 0; d->obs_done_i = 0;
+        d->obs_issue_id_i_flat = 0; d->obs_done_id_i_flat = 0;
+        d->selected_i = 0; d->select_write_i = 0;
+        d->select_id_i = 0; d->select_did_i = 0; d->select_iova_i = 0;
+        tick();
+        d->rst_ni = 1;
+        tick();
+    }
+    void write(uint8_t addr, uint64_t data, uint8_t strb = 0xff) {
+        d->cfg_valid_i = 1; d->cfg_write_i = 1;
+        d->cfg_addr_i = addr; d->cfg_wdata_i = data; d->cfg_wstrb_i = strb;
+        tick();
+        d->cfg_valid_i = 0; d->cfg_write_i = 0;
+        d->eval();
+    }
+    uint64_t read(uint8_t addr) {
+        d->cfg_valid_i = 0; d->cfg_write_i = 0;
+        d->cfg_addr_i = addr;
+        d->eval();
+        return d->cfg_rdata_o;
+    }
+    uint64_t read_index(uint8_t idx) {
+        write(0xc0, idx);
+        return read(0xc8);
+    }
+    void set_issue(int port, uint64_t id) {
+        d->obs_issue_i = (1u << port);
+        if (port == 0) d->obs_issue_id_i_flat = id;
+        else if (port == 1) d->obs_issue_id_i_flat = id << 64;
+        else if (port == 2) d->obs_issue_id_i_flat = id << 128;
+        else if (port == 3) d->obs_issue_id_i_flat = id << 192;
+    }
+    void set_done(int port, uint64_t id) {
+        d->obs_done_i = (1u << port);
+        if (port == 0) d->obs_done_id_i_flat = id;
+        else if (port == 1) d->obs_done_id_i_flat = id << 64;
+        else if (port == 2) d->obs_done_id_i_flat = id << 128;
+        else if (port == 3) d->obs_done_id_i_flat = id << 192;
+    }
+    void clear_events() {
+        d->obs_issue_i = 0; d->obs_done_i = 0;
+        d->obs_issue_id_i_flat = 0; d->obs_done_id_i_flat = 0;
+    }
+    uint64_t status() { return read(0xb8); }
+    bool error_any() { return (status() >> 32) != 0; }
+};
+
+static void test_magic(Dut &d) {
+    assert(d.read(0xb0) == MAGIC);
+}
+
+static void test_solo_prep_go_done(Dut &d) {
+    d.write(0xb8, 1);                    // PREP solo
+    assert((d.status() & 1) == 1);        // PREP
+    assert(d.d->launch_hold_o == 1);
+    d.d->frontend_held_i[0] = 1;
+    d.write(0xb8, 2);                    // GO solo
+    assert(d.d->launch_go_o == 1);
+    d.tick();
+    assert(d.d->launch_go_o == 0);
+    d.d->frontend_held_i[0] = 0;
+    d.d->frontend_active_i[0] = 1;
+    d.set_issue(0, 0); d.tick(); d.clear_events();
+    d.set_done(0, 0); d.tick(); d.clear_events();
+    d.d->frontend_active_i[0] = 0;
+    d.tick();
+    assert((d.status() & 4) == 4);        // FROZEN done
+    assert(d.read_index(24) == 1);        // V issue count
+    assert(d.read_index(26) == 1);        // V done count
+    assert(d.read_index(11) == 0);        // V issue ID
+    assert(d.read_index(13) == 0);        // V done ID
+}
+
+static void test_non_solo_exact_goA_then_goV(Dut &d) {
+    d.write(0xb8, 1 | 0x100);            // PREP with A
+    assert(d.read_index(19) == 3);        // expected mask V|A
+    assert(d.d->launch_hold_o == 3);
+    d.d->frontend_held_i[0] = 1;
+    d.d->frontend_held_i[1] = 1;
+    d.write(0xb8, 2);                    // GO
+    assert(d.d->launch_go_o == 2);        // A first only
+    d.tick();
+    assert(d.d->launch_go_o == 0);
+    d.d->frontend_held_i[1] = 0;
+    d.d->frontend_active_i[1] = 1;
+    // A actual issue.
+    d.set_issue(1, 4); d.tick(); d.clear_events();
+    // Valid A read select.
+    d.d->selected_i = 1; d.d->select_write_i = 0;
+    d.d->select_id_i = 4; d.d->select_did_i = 11; d.d->select_iova_i = 0x1000;
+    d.tick();
+    assert(d.d->launch_go_o == 1);        // V go on following cycle
+    d.d->selected_i = 0;
+    d.tick();
+    assert(d.d->launch_go_o == 0);
+}
+
+static void test_v_never_go_on_bad_a_select(Dut &d) {
+    d.write(0xb8, 1 | 0x100);
+    d.d->frontend_held_i[0] = 1;
+    d.d->frontend_held_i[1] = 1;
+    d.write(0xb8, 2);
+    d.tick();
+    d.d->frontend_held_i[1] = 0;
+    d.d->frontend_active_i[1] = 1;
+    d.set_issue(1, 4); d.tick(); d.clear_events();
+    // A write must not release V.
+    d.d->selected_i = 1; d.d->select_write_i = 1;
+    d.d->select_id_i = 4; d.d->select_did_i = 11;
+    d.tick();
+    assert(d.d->launch_go_o == 0);
+    // Wrong ID.
+    d.d->select_write_i = 0; d.d->select_id_i = 5; d.d->select_did_i = 11;
+    d.tick();
+    assert(d.d->launch_go_o == 0);
+    // Wrong DID.
+    d.d->select_id_i = 4; d.d->select_did_i = 12;
+    d.tick();
+    assert(d.d->launch_go_o == 0);
+    d.d->selected_i = 0;
+}
+
+static void test_duplicate_and_bad_writes(Dut &d) {
+    d.write(0xb8, 1);
+    d.write(0xb8, 1);                    // second PREP
+    assert(d.error_any());
+    d.write(0x60, 4);                     // unsupported forwarded write
+    assert(d.error_any());
+    d.write(0xb8, 2, 0x00);               // partial strobe
+    assert(d.error_any());
+    d.write(0xb0, 0);                     // write magic
+    assert(d.error_any());
+    d.write(0xc8, 0);                     // write data
+    assert(d.error_any());
+}
+
+static void test_abort_before_reserve(Dut &d) {
+    d.write(0xb8, 4);                    // ABORT in OFF
+    d.tick();
+    uint64_t s = d.status();
+    assert((s & 8) == 8);                 // aborted bit
+    assert((s & 4) == 4);                 // frozen done
+}
+
+static void test_abort_select_race(Dut &d) {
+    d.write(0xb8, 1 | 0x100);
+    d.d->frontend_held_i[0] = 1;
+    d.d->frontend_held_i[1] = 1;
+    d.write(0xb8, 2);
+    d.tick();
+    d.d->frontend_held_i[1] = 0;
+    d.d->frontend_active_i[1] = 1;
+    d.set_issue(1, 4);
+    // ABORT and valid A select in the same cycle. ABORT must win.
+    d.d->selected_i = 1; d.d->select_write_i = 0;
+    d.d->select_id_i = 4; d.d->select_did_i = 11;
+    d.d->cfg_valid_i = 1; d.d->cfg_write_i = 1;
+    d.d->cfg_addr_i = 0xb8; d.d->cfg_wdata_i = 4; d.d->cfg_wstrb_i = 0xff;
+    d.tick();
+    d.d->cfg_valid_i = 0; d.d->cfg_write_i = 0;
+    d.d->selected_i = 0;
+    assert(d.d->launch_go_o == 0);
+    // A issue must still drain.
+    d.clear_events();
+    d.set_done(1, 4); d.tick(); d.clear_events();
+    d.d->frontend_active_i[1] = 0;
+    d.tick();
+    assert((d.status() & 8) == 8);
+}
+
+int main(int argc, char **argv) {
+    Verilated::commandArgs(argc, argv);
+    test_magic(*(new Dut));
+    test_solo_prep_go_done(*(new Dut));
+    test_non_solo_exact_goA_then_goV(*(new Dut));
+    test_v_never_go_on_bad_a_select(*(new Dut));
+    test_duplicate_and_bad_writes(*(new Dut));
+    test_abort_before_reserve(*(new Dut));
+    test_abort_select_race(*(new Dut));
+    std::puts("PASS dma_controlled_launcher");
+    return 0;
+}
```

---

## 6. Python runner: `tests/test_dma_controlled_launcher.py`

```diff
diff --git a/tests/test_dma_controlled_launcher.py b/tests/test_dma_controlled_launcher.py
new file mode 100644
--- /dev/null
+++ b/tests/test_dma_controlled_launcher.py
@@ -0,0 +1,30 @@
+#!/usr/bin/env python3
+from pathlib import Path
+import subprocess
+
+root = Path(__file__).resolve().parents[1]
+out = root / 'out/dma-controlled-launcher'
+out.mkdir(parents=True, exist_ok=True)
+
+fpga = root / 'cva6/corev_apu/fpga/src'
+files = [
+    fpga / 'dma_controlled_launcher.sv',
+    root / 'tests/dma_controlled_launcher_top.sv',
+    root / 'tests/dma_controlled_launcher_test.cpp',
+]
+
+cmd = [
+    'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
+    '--top-module', 'dma_controlled_launcher_top',
+    '--Mdir', str(out),
+    *map(str, files),
+]
+
+log = out / 'build.log'
+with log.open('w') as f:
+    r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
+if r.returncode:
+    print(log.read_text())
+    raise SystemExit(r.returncode)
+
+subprocess.run([str(out / 'Vdma_controlled_launcher_top')], check=True)
```

---

## 7. Observer finite v6 test: `tests/dma_observer_finite_test.cpp`

```diff
diff --git a/tests/dma_observer_finite_test.cpp b/tests/dma_observer_finite_test.cpp
new file mode 100644
--- /dev/null
+++ b/tests/dma_observer_finite_test.cpp
@@ -0,0 +1,70 @@
+#include "Vdma_observer.h"
+#include <verilated.h>
+#include <cassert>
+#include <cstdio>
+
+struct B {
+    Vdma_observer d;
+    void tick() { d.clk_i = 0; d.eval(); d.clk_i = 1; d.eval(); }
+    B() { d.rst_ni = 0; tick(); d.rst_ni = 1; tick(); }
+    void write(unsigned a, unsigned long long v) {
+        d.cfg_valid_i = d.cfg_write_i = 1;
+        d.cfg_wstrb_i = 255;
+        d.cfg_addr_i = a;
+        d.cfg_wdata_i = v;
+        tick();
+        d.cfg_valid_i = d.cfg_write_i = 0;
+    }
+    unsigned long long read(unsigned a) {
+        d.cfg_addr_i = a;
+        d.eval();
+        return d.cfg_rdata_o;
+    }
+};
+
+int main(int argc, char **argv) {
+    Verilated::commandArgs(argc, argv);
+    B b;
+
+#ifdef CONTROLLED
+    assert(b.read(0x58) == 0x49444d414f425306ULL);
+
+    // target=1, V=0, A=1, batch=1, path=1, e3=1, finite=1
+    unsigned long long arm =
+        1 | (1 << 8) | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) | (1 << 29);
+    b.write(0x68, 1);
+    b.write(0x60, arm);
+    assert(!(b.read(0x60) >> 32));
+
+    // Start V issue.
+    b.d.issue_i = 1;
+    b.d.issue_id_i[0] = 1;
+    b.tick();
+    b.d.issue_i = 0;
+
+    // A can finish before V when finite is set. The legacy batch-active
+    // invariant must be skipped, so error bit 5 must stay zero.
+    b.d.issue_i = 2;
+    b.d.issue_id_i[1] = 2;
+    b.tick();
+    b.d.issue_i = 0;
+    b.d.done_i = 2;
+    b.d.done_id_i[1] = 2;
+    b.d.batch_active_i = 0;
+    b.tick();
+    b.d.done_i = 0;
+    assert(!(b.read(0x60) & (1ULL << 37)));
+
+    std::puts("PASS controlled finite v6 observer allows A finish before V");
+#else
+    assert(b.read(0x58) == 0x49444d414f425305ULL);
+
+    // Legacy: bit29 is not a valid ARM bit.
+    unsigned long long arm =
+        1 | (1 << 8) | (1 << 16) | (1 << 24) | (1 << 25) | (1 << 26) | (1 << 27) | (1 << 29);
+    b.write(0x68, 1);
+    b.write(0x60, arm);
+    assert((b.read(0x60) >> 32) != 0);
+
+    std::puts("PASS legacy v5 observer rejects finite flag");
+#endif
+    return 0;
+}
```

---

## 8. Observer finite runner: `tests/test_dma_observer_finite.py`

```diff
diff --git a/tests/test_dma_observer_finite.py b/tests/test_dma_observer_finite.py
new file mode 100644
--- /dev/null
+++ b/tests/test_dma_observer_finite.py
@@ -0,0 +1,35 @@
+#!/usr/bin/env python3
+from pathlib import Path
+import subprocess
+
+root = Path(__file__).resolve().parents[1]
+out = root / 'out/dma-observer-finite'
+out.mkdir(parents=True, exist_ok=True)
+
+fpga = root / 'cva6/corev_apu/fpga/src'
+base = [
+    fpga / 'dma_path_observer.sv',
+    fpga / 'dma_observer.sv',
+    root / 'tests/dma_observer_finite_test.cpp',
+]
+
+for controlled in (1, 0):
+    name = 'controlled' if controlled else 'legacy'
+    dest = out / name
+    dest.mkdir(exist_ok=True)
+    cmd = [
+        'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
+        '--top-module', 'dma_observer',
+        '--Mdir', str(dest),
+        '-GPathEnable=1', '-GE3Enable=1', f'-GControlledEnable={controlled}',
+        '-D' + ('CONTROLLED' if controlled else 'LEGACY'),
+        *map(str, base),
+    ]
+    log = dest / 'build.log'
+    with log.open('w') as f:
+        r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
+    if r.returncode:
+        print(log.read_text())
+        raise SystemExit(r.returncode)
+    subprocess.run([str(dest / 'Vdma_observer')], check=True)
```

---

## Suggested acceptance checks

1. Build/run `tests/test_dma_controlled_launcher.py`; expect `PASS dma_controlled_launcher`.
2. Build/run `tests/test_dma_observer_finite.py`; expect controlled v6 pass and legacy v5 rejection pass.
3. Re-run existing observer/iDMA tests to ensure no regressions, especially:
   - `tests/test_e3.py`
   - `tests/test_idma_batch_pair.py`
4. In synthesis/elaboration, verify:
   - `dma_controlled_launcher` is present in the FPGA source list used by `run.tcl`.
   - `dma_core_wrap_intf` accepts `LaunchPort` and the seven new `launch_*` ports.
   - `obs_cfg_addr_o` is 8 bits from the wrapper.
5. Check that only DMA0 config writes with `obs_cfg_addr[0][7] == 1` reach the launcher and only DMA0 legacy writes with `[7] == 0` reach `dma_observer`.
6. Check that PREP/GO/ABORT/CLEAR status and sticky error behavior match the documented bit meanings.

Open issues / assumptions for Codex:
- The exact frontend wrapper port names are assumed from the frozen interface. If the wrapper uses different names, only the top-level connection names need adjustment.
- If external debug visibility of the four frontend status vectors is required, add top-level outputs and wire them out; that is outside the files listed here.
- The finite observer test is intentionally focused. It does not attempt to reproduce the full existing E3 bank v2 checker; it only verifies that finite_q skips the one batch-active invariant and that legacy rejects bit29.