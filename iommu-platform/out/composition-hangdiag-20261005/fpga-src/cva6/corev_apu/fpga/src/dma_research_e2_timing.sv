// SPDX-License-Identifier: Solderpad-0.51
//
// E2 probe timing block for the 2026-10-03 two-BIT implementation contract.
//
// This module is passive: none of its outputs drive DMA, IOTLB, translation,
// command queues, or interrupts.  It only records the V/DID10 interval after a
// legally accepted PROBE command.  The observer instantiates it read-only.
//
// Register meaning in the observer local index window (D80..D8F):
//   D80 magic 0x49444d4145325431 (IDMAE2T1)
//   D81 version 1
//   D82 error
//   D83 flags[2:0] = issue_seen, select_seen, done_seen
//   D84 V issue epoch after PROBE
//   D85 first V select epoch
//   D86 V done epoch
//   D87 done - issue cycles
//   D88 first_select - issue cycles
//   D89 V REQ_TRANS/AWAIT_RESP occupancy cycles
//   D8A V COMPLETE cycles
//   D8B V select count
//   D8C V hit count
//   D8D V miss count
//   D8E..D8F reserved (read zero)
//
// Timing contract implemented here:
//   * phase_i gates the block.  Only the observer E2_PROBE phase (3) measures:
//     issue/done/select/lookup events, repeat detection and every error bit that
//     belongs to the armed/active/done interval.  IDLE/ARM_WARM/ARM_A/READY
//     traffic is ignored completely (no counts, no flags, no error).
//   * In FREEZE (4) and ABORT (5) every field, error_o included, is held bit
//     for bit until the legal CLEAR.  Later ordinary V issue/done/select/lookup
//     or fault activity cannot pollute a frozen result.
//   * The occupancy interval is (issue, done]: the issue edge itself is not an
//     occupancy cycle, the done edge is.  D87/D88 are epoch differences and
//     therefore also follow (issue, done].  Select/lookup events on the issue
//     edge are still recorded and counted.
//   * A backwards or wrapping epoch never silently wraps the unsigned
//     difference: error bit 6 is set and the affected cycle field is stored as
//     zero when done < issue or first_select < issue.
//
// Error bits:
//   [0] counter overflow
//   [1] duplicate V issue
//   [2] V done without issue
//   [3] duplicate V done
//   [4] response_error during active interval
//   [5] unsupported during armed/active interval
//   [6] epoch went backwards (done < issue or first_select < issue)
//   [7] duplicate/illegal probe_accept (old data preserved; not flagged while
//       FREEZE/ABORT holds the block)
//
// Clearing is accepted only from the already-legal observer CLEAR path.  clear_i
// has priority over every other input and releases a frozen result.
module dma_research_e2_timing #(
    parameter int unsigned CounterWidth = 64
) (
    input  logic          clk_i,
    input  logic          rst_ni,

    input  logic          clear_i,
    input  logic          probe_accept_i,
    input  logic [2:0]    phase_i,
    input  logic [63:0]   epoch_i,

    input  logic          issue_v_i,
    input  logic          done_v_i,
    input  logic          select_v_i,
    input  logic          ctl_v_i,
    input  logic [2:0]    ctl_state_i,
    input  logic          lookup_v_i,
    input  logic          lookup_hit_i,
    input  logic          response_error_i,
    input  logic          unsupported_i,

    output logic [15:0]   error_o,
    output logic [63:0]   flags_o,
    output logic [63:0]   issue_epoch_o,
    output logic [63:0]   first_select_epoch_o,
    output logic [63:0]   done_epoch_o,
    output logic [63:0]   done_issue_cycles_o,
    output logic [63:0]   first_select_issue_cycles_o,
    output logic [63:0]   v_main_cycles_o,
    output logic [63:0]   v_comp_cycles_o,
    output logic [63:0]   v_select_count_o,
    output logic [63:0]   v_hit_count_o,
    output logic [63:0]   v_miss_count_o
);

    localparam logic [2:0] ST_REQ_TRANS  = 3'd1;
    localparam logic [2:0] ST_AWAIT_RESP = 3'd2;
    localparam logic [2:0] ST_COMPLETE   = 3'd4;

    localparam logic [2:0] T_IDLE   = 3'd0;
    localparam logic [2:0] T_ARMED  = 3'd1;
    localparam logic [2:0] T_ACTIVE = 3'd2;
    localparam logic [2:0] T_DONE   = 3'd3;

    // observer E2 phases (phase_i); only E2_PROBE measures
    localparam logic [2:0] E2_PROBE  = 3'd3;
    localparam logic [2:0] E2_FREEZE = 3'd4;
    localparam logic [2:0] E2_ABORT  = 3'd5;

    logic [2:0]  state_q, state_n;
    logic [15:0] err_q, err_n;
    logic [63:0] flags_q, flags_n;

    logic [63:0] issue_epoch_q, issue_epoch_n;
    logic [63:0] first_select_epoch_q, first_select_epoch_n;
    logic [63:0] done_epoch_q, done_epoch_n;
    logic [63:0] done_issue_q, done_issue_n;
    logic [63:0] first_select_issue_q, first_select_issue_n;

    logic [CounterWidth-1:0] v_main_q, v_main_n;
    logic [CounterWidth-1:0] v_comp_q, v_comp_n;
    logic [CounterWidth-1:0] v_sel_q,  v_sel_n;
    logic [CounterWidth-1:0] v_hit_q,  v_hit_n;
    logic [CounterWidth-1:0] v_miss_q, v_miss_n;

    // phase_i gating: only E2_PROBE measures, FREEZE/ABORT hold everything.
    logic phase_window;      // phase_i == E2_PROBE
    logic phase_frozen;      // phase_i == E2_FREEZE || phase_i == E2_ABORT

    // Interval (issue, done]: the issue edge is inside the measurement window
    // for select/lookup, but it is not an occupancy cycle.
    logic issue_edge;
    logic interval_open;     // issue edge or already past it
    logic main_comp_active;  // strictly after the issue edge
    logic [63:0] issue_epoch_now;

    assign phase_window = (phase_i == E2_PROBE);
    assign phase_frozen = (phase_i == E2_FREEZE) || (phase_i == E2_ABORT);

    assign issue_edge       = (state_q == T_ARMED) && issue_v_i;
    assign interval_open    = issue_edge || (state_q == T_ACTIVE);
    assign main_comp_active = (state_q == T_ACTIVE);

    assign issue_epoch_now = issue_edge ? epoch_i : issue_epoch_q;

    always_comb begin
        state_n = state_q;
        err_n   = err_q;
        flags_n = flags_q;

        issue_epoch_n = issue_epoch_q;
        first_select_epoch_n = first_select_epoch_q;
        done_epoch_n = done_epoch_q;
        done_issue_n = done_issue_q;
        first_select_issue_n = first_select_issue_q;

        v_main_n = v_main_q;
        v_comp_n = v_comp_q;
        v_sel_n  = v_sel_q;
        v_hit_n  = v_hit_q;
        v_miss_n = v_miss_q;

        if (clear_i) begin
            state_n = T_IDLE;
            err_n   = 16'd0;
            flags_n = 64'd0;
            issue_epoch_n = 64'd0;
            first_select_epoch_n = 64'd0;
            done_epoch_n = 64'd0;
            done_issue_n = 64'd0;
            first_select_issue_n = 64'd0;
            v_main_n = '0;
            v_comp_n = '0;
            v_sel_n  = '0;
            v_hit_n  = '0;
            v_miss_n = '0;
        end else if (phase_frozen) begin
            // FREEZE (4) / ABORT (5): hold every recorded field bit for bit,
            // error_o included, until the legal CLEAR arrives.  Ordinary V
            // issue/done/select/lookup or fault activity belongs to a later,
            // unrelated interval and must not pollute a frozen result.
        end else begin
            // A duplicate / illegal PROBE acceptance is flagged but never
            // overwrites data already recorded for the session.
            if (probe_accept_i) begin
                if (state_q == T_IDLE) state_n = T_ARMED;
                else                   err_n[7] = 1'b1;
            end

            // Only the observer E2_PROBE phase measures.  ARM_WARM/ARM_A/
            // READY/IDLE traffic is ignored (no counts, no flags, no error).
            if (phase_window) begin
                if (unsupported_i && ((state_q == T_ARMED) || (state_q == T_ACTIVE)))
                    err_n[5] = 1'b1;
                if (response_error_i && interval_open)
                    err_n[4] = 1'b1;

                if (issue_v_i) begin
                    if (state_q == T_ARMED) begin
                        state_n = T_ACTIVE;
                        issue_epoch_n = epoch_i;
                        flags_n[0] = 1'b1;
                    end else if ((state_q == T_ACTIVE) || (state_q == T_DONE)) begin
                        err_n[1] = 1'b1;
                    end
                end

                if (done_v_i) begin
                    if (interval_open) begin
                        state_n = T_DONE;
                        done_epoch_n = epoch_i;
                        flags_n[2] = 1'b1;
                        if (epoch_i < issue_epoch_now) begin
                            // Epoch moved backwards: never store a wrapped
                            // unsigned difference.
                            err_n[6] = 1'b1;
                            done_issue_n = 64'd0;
                        end else begin
                            done_issue_n = epoch_i - issue_epoch_now;
                        end
                    end else if (state_q == T_DONE) begin
                        err_n[3] = 1'b1;
                    end else if (state_q == T_ARMED) begin
                        err_n[2] = 1'b1;
                        state_n = T_DONE;
                        done_epoch_n = epoch_i;
                        flags_n[2] = 1'b1;
                    end
                end

                // Occupancy covers (issue, done] only: the issue edge itself is
                // not an occupancy cycle, the done edge is.
                if (main_comp_active) begin
                    if (ctl_v_i && ((ctl_state_i == ST_REQ_TRANS) || (ctl_state_i == ST_AWAIT_RESP))) begin
                        if (&v_main_q) err_n[0] = 1'b1;
                        v_main_n = v_main_q + 1'b1;
                    end
                    if (ctl_v_i && (ctl_state_i == ST_COMPLETE)) begin
                        if (&v_comp_q) err_n[0] = 1'b1;
                        v_comp_n = v_comp_q + 1'b1;
                    end
                end

                // Select/lookup events on the issue edge are still recorded.
                if (interval_open) begin
                    if (select_v_i) begin
                        if (&v_sel_q) err_n[0] = 1'b1;
                        v_sel_n = v_sel_q + 1'b1;
                        if (!flags_q[1]) begin
                            first_select_epoch_n = epoch_i;
                            flags_n[1] = 1'b1;
                            if (epoch_i < issue_epoch_now) begin
                                err_n[6] = 1'b1;
                                first_select_issue_n = 64'd0;
                            end else begin
                                first_select_issue_n = epoch_i - issue_epoch_now;
                            end
                        end
                    end
                    if (lookup_v_i) begin
                        if (lookup_hit_i) begin
                            if (&v_hit_q) err_n[0] = 1'b1;
                            v_hit_n = v_hit_q + 1'b1;
                        end else begin
                            if (&v_miss_q) err_n[0] = 1'b1;
                            v_miss_n = v_miss_q + 1'b1;
                        end
                    end
                end
            end  // phase_window: only E2_PROBE measures
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            state_q <= T_IDLE;
            err_q   <= 16'd0;
            flags_q <= 64'd0;
            issue_epoch_q <= 64'd0;
            first_select_epoch_q <= 64'd0;
            done_epoch_q <= 64'd0;
            done_issue_q <= 64'd0;
            first_select_issue_q <= 64'd0;
            v_main_q <= '0;
            v_comp_q <= '0;
            v_sel_q  <= '0;
            v_hit_q  <= '0;
            v_miss_q <= '0;
        end else begin
            state_q <= state_n;
            err_q   <= err_n;
            flags_q <= flags_n;
            issue_epoch_q <= issue_epoch_n;
            first_select_epoch_q <= first_select_epoch_n;
            done_epoch_q <= done_epoch_n;
            done_issue_q <= done_issue_n;
            first_select_issue_q <= first_select_issue_n;
            v_main_q <= v_main_n;
            v_comp_q <= v_comp_n;
            v_sel_q  <= v_sel_n;
            v_hit_q  <= v_hit_n;
            v_miss_q <= v_miss_n;
        end
    end

    assign error_o = err_q;
    assign flags_o = flags_q;
    assign issue_epoch_o = issue_epoch_q;
    assign first_select_epoch_o = first_select_epoch_q;
    assign done_epoch_o = done_epoch_q;
    assign done_issue_cycles_o = done_issue_q;
    assign first_select_issue_cycles_o = first_select_issue_q;
    assign v_main_cycles_o = 64'(v_main_q);
    assign v_comp_cycles_o = 64'(v_comp_q);
    assign v_select_count_o = 64'(v_sel_q);
    assign v_hit_count_o = 64'(v_hit_q);
    assign v_miss_count_o = 64'(v_miss_q);

endmodule
