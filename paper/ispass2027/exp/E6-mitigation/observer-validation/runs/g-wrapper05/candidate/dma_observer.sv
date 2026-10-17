// SPDX-License-Identifier: Solderpad-0.51
// Passive E4 observer. No output controls the DMA or AXI data paths.
module dma_observer #(
    parameter int unsigned NumPorts = 4,
    parameter int unsigned CounterWidth = 64,
    parameter int unsigned OutstandingWidth = 8,
    parameter bit PathEnable = 0,
    parameter bit HistEnable = 0,
    parameter bit GapEnable = 0,
    parameter int HistogramCountWidth = 32,
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
    input logic [NumPorts-1:0] batch_gap_active_i,
    input logic [31:0] batch_gap_cycles_i [NumPorts],
    input logic [32:0] batch_target_i [NumPorts],
    input logic [63:0] issue_id_i [NumPorts],
    input logic [63:0] done_id_i [NumPorts],
    input logic [63:0] poll_id_i [NumPorts],
    input logic [NumPorts-1:0] ar_valid_i, aw_valid_i, ar_ready_i, aw_ready_i,
    input logic [NumPorts-1:0] rlast_fire_i, b_fire_i, bad_id_i,
    input logic shared_ar_valid_i, shared_aw_valid_i,
    input logic shared_ar_ready_i, shared_aw_ready_i,
    input logic [AxiIdWidth-1:0] tr_id_i [2], out_id_i [2], return_id_i [2],
    input logic [1:0] out_valid_i, out_ready_i, return_i
);
    // Low byte is interface version; a missing observer must never look valid.
    localparam logic [63:0] Magic = PathEnable && HistEnable ? 64'h49444d414f425304 :
                                  PathEnable ? 64'h49444d414f425303 : 64'h49444d414f425302;
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
    logic [9:0] index_q, index_n;
    logic [1:0] victim_q, victim_n, aggressor_q, aggressor_n;
    logic has_a_q, has_a_n, batch_q, batch_n;
    logic busy;
    assign busy = active || phase_q == BATCH_READ;
    logic active, v_pending, a_pending;
    logic path_q, path_n, path_arm, path_clear, path_error, path_idle;
    logic [63:0] path_data;
    logic [63:0] hist_data, hist_info;
    logic [15:0] hist_overflow;
    logic g_arm, g_clear, g_abort, g_busy, g_ready, g_config_ok, g_a_complete;
    logic [9:0] g_reject, g_error;
    logic [63:0] g_data;
    logic g_quiet;
    assign g_quiet = !(|batch_active_i || |issue_i || |done_i || |ar_valid_i || |aw_valid_i ||
        |track_bad_q || |rlast_fire_i || |b_fire_i) &&
        reads_q[0] == 0 && writes_q[0] == 0 && reads_q[1] == 0 && writes_q[1] == 0;
    assign g_config_ok = cfg_wdata_i[25] && cfg_wdata_i[26] &&
        cfg_wdata_i[9:8] == 0 && (!cfg_wdata_i[24] || cfg_wdata_i[17:16] == 1) &&
        target_q == 4096 && batch_target_i[0] == 4096 && batch_gap_cycles_i[0] == 0 &&
        (!cfg_wdata_i[24] || batch_target_i[1] == 4096);
    if (GapEnable) begin : gen_gap
        dma_gap_observer #(.NumPorts(NumPorts)) i_gap (
            .clk_i, .rst_ni, .arm_i(g_arm), .clear_i(g_clear), .abort_i(g_abort),
            .has_a_i(cfg_wdata_i[24]), .quiet_i(g_quiet), .reject_i(g_reject),
            .issue_i, .done_i, .issue_id_i, .done_id_i, .batch_active_i,
            .gap_active_i(batch_gap_active_i), .gap_cycles_i(batch_gap_cycles_i), .target_i(batch_target_i),
            .open_i(phase_q == ARMED && issue_i[victim_q]),
            .close_i(active && phase_n == BATCH_READ), .window_cycles_i(64'(count_n[0])),
            .index_i(5'(index_q - 10'd768)), .data_o(g_data),
            .busy_o(g_busy), .ready_o(g_ready), .a_complete_o(g_a_complete), .error_o(g_error)
        );
    end else begin : gen_no_gap
        assign g_data = 0;
        assign g_busy = 0;
        assign g_ready = 1;
        assign g_a_complete = 0;
        assign g_error = 0;
    end
    assign hist_info = {16'd0, hist_overflow, 8'd32, 8'(HistogramCountWidth), 8'd32, 8'd1};
    assign path_idle = !(|reads_q[victim_n] || |writes_q[victim_n] ||
        ar_valid_i[victim_n] || aw_valid_i[victim_n] ||
        (has_a_n && (|reads_q[aggressor_n] || |writes_q[aggressor_n] ||
        ar_valid_i[aggressor_n] || aw_valid_i[aggressor_n])));
    if (PathEnable) begin : gen_path
        dma_path_observer #(.NumPorts(NumPorts), .AxiIdWidth(AxiIdWidth),
                            .HistEnable(HistEnable), .HistogramCountWidth(HistogramCountWidth)) i_path (
            .clk_i, .rst_ni, .clear_i(path_clear), .arm_i(path_arm),
            .active_i(active), .open_i(phase_q == ARMED && issue_i[victim_q]),
            .close_i(active && phase_n == BATCH_READ),
            .abort_i(phase_n == FROZEN),
            .victim_i(victim_n), .aggressor_i(aggressor_n), .has_a_i(has_a_n), .idle_i(path_idle),
            .in_valid_i({aw_valid_i, ar_valid_i}), .in_ready_i({aw_ready_i, ar_ready_i}),
            .tr_valid_i({shared_aw_valid_i, shared_ar_valid_i}),
            .tr_ready_i({shared_aw_ready_i, shared_ar_ready_i}),
            .tr_id_i, .out_valid_i, .out_ready_i, .out_id_i, .return_i, .return_id_i,
            .index_i(7'(index_q - 10'd64)), .data_o(path_data), .error_o(path_error),
            .hist_index_i(9'(index_q - 10'd256)), .hist_data_o(hist_data), .hist_overflow_o(hist_overflow)
        );
    end else begin : gen_no_path
        assign path_data = 0;
        assign path_error = 0;
        assign hist_data = 0;
        assign hist_overflow = 0;
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
                cfg_rdata_o[38] = error_q[6] || (path_q && path_error) ||
                    (GapEnable && (|g_error || (phase_q == FROZEN && g_busy)));
            end
            7'h68: cfg_rdata_o = target_q;
            7'h70: cfg_rdata_o = 64'(index_q);
            7'h78: if (int'(index_q) < Count) cfg_rdata_o = 64'(count_q[index_q]);
                   else if (PathEnable && index_q >= 64 && index_q < 192) cfg_rdata_o = path_data;
                   else if (PathEnable && HistEnable && index_q == 192) cfg_rdata_o = hist_info;
                   else if (PathEnable && HistEnable && index_q >= 256 && index_q < 768) cfg_rdata_o = hist_data;
                   else if (GapEnable && index_q >= 768 && index_q < 800) cfg_rdata_o = g_data;
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
        path_n = path_q;
        path_arm = 0;
        path_clear = 0;
        track_bad_n = track_bad_q;
        g_arm = 0;
        g_clear = 0;
        g_abort = 0;
        g_reject = 0;
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
        if (batch_q && (active || phase_q == ARMED && issue_i[victim_q]) &&
            has_a_q && !batch_active_i[aggressor_q] &&
            !(GapEnable && g_a_complete)) error_n[5] = 1'b1;
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
            if (cfg_wstrb_i != 8'hff) begin error_n[0] = 1'b1; g_reject[8] = GapEnable; end
            else case (cfg_addr_i)
                7'h68: if (!busy && phase_q != ARMED && !(GapEnable && g_busy)) target_n = cfg_wdata_i;
                         else begin error_n[0] = 1'b1; g_reject[8] = GapEnable; end
                7'h70: if (cfg_wdata_i < 64'(Count) ||
                          (PathEnable && cfg_wdata_i >= 64 && cfg_wdata_i < 192) ||
                          (PathEnable && HistEnable && (cfg_wdata_i == 192 ||
                           (cfg_wdata_i >= 256 && cfg_wdata_i < 768))) ||
                          (GapEnable && cfg_wdata_i >= 768 && cfg_wdata_i < 800)) index_n = cfg_wdata_i[9:0];
                         else begin error_n[0] = 1'b1; g_reject[8] = GapEnable; end
                7'h60: begin
                    if (cfg_wdata_i == 2) begin // Abort observation, never DMA.
                        if (busy || phase_q == ARMED || (GapEnable && g_busy)) begin
                            phase_n = FROZEN;
                            error_n[4] = 1'b1;
                            g_abort = GapEnable;
                        end
                    end else if (cfg_wdata_i == 4) begin
                        if (busy || phase_q == ARMED || (GapEnable && !g_ready)) begin
                            error_n[0] = 1'b1; g_reject[8] = GapEnable;
                        end else begin phase_n = OFF; error_n = 0; path_clear = 1; path_n = 0; g_clear = GapEnable; end
                    end else if ((cfg_wdata_i & ~64'h0000000007030301) == 0 && cfg_wdata_i[0] &&
                                 (!cfg_wdata_i[26] || (PathEnable && cfg_wdata_i[25])) &&
                                 !busy && phase_q != ARMED && target_q != 0 && target_q <= 64'h100000000 &&
                                 int'(cfg_wdata_i[9:8]) < NumPorts && int'(cfg_wdata_i[17:16]) < NumPorts &&
                                 (!cfg_wdata_i[24] || cfg_wdata_i[9:8] != cfg_wdata_i[17:16]) &&
                                 (!GapEnable || (g_ready && g_config_ok))) begin
                        phase_n = ARMED;
                        error_n = 0;
                        victim_n = cfg_wdata_i[9:8];
                        aggressor_n = cfg_wdata_i[17:16];
                        has_a_n = cfg_wdata_i[24];
                        batch_n = cfg_wdata_i[25];
                        path_n = cfg_wdata_i[26];
                        path_arm = cfg_wdata_i[26];
                        g_arm = GapEnable;
                        segment_n = 0;
                        for (int i = 0; i < Count; i++) count_n[i] = 0;
                    end else begin
                        error_n[0] = 1'b1;
                        if (GapEnable) begin
                            if (cfg_wdata_i[0] && (cfg_wdata_i & ~64'h0000000007030301) == 0 &&
                                g_ready && !busy && phase_q != ARMED && !g_config_ok)
                                g_reject[0] = 1;
                            else g_reject[8] = 1;
                        end
                    end
                end
                default: begin error_n[0] = 1'b1; g_reject[8] = GapEnable; end
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
            path_q <= 0;
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
            path_q <= path_n;
            track_bad_q <= track_bad_n;
            for (int i = 0; i < Count; i++) count_q[i] <= count_n[i];
            for (int i = 0; i < NumPorts; i++) begin reads_q[i] <= reads_n[i]; writes_q[i] <= writes_n[i]; end
        end
    end
endmodule

// G's bounded task/window accounting. It observes events only; no DMA/AXI control.
// Continuous lane pairing survives ARM/clear/abort. Measured IDs restart at 1
// after ARM, independently of arbitrary warmup/nonbatch functional IDs.
module dma_gap_observer #(parameter int unsigned NumPorts = 4) (
    input logic clk_i, rst_ni, arm_i, clear_i, abort_i, has_a_i, quiet_i,
    input logic [9:0] reject_i,
    input logic [NumPorts-1:0] issue_i, done_i, batch_active_i, gap_active_i,
    input logic [63:0] issue_id_i [NumPorts], done_id_i [NumPorts],
    input logic [31:0] gap_cycles_i [NumPorts],
    input logic [32:0] target_i [NumPorts],
    input logic open_i, close_i,
    input logic [63:0] window_cycles_i,
    input logic [4:0] index_i,
    output logic [63:0] data_o,
    output logic busy_o, ready_o, a_complete_o,
    output logic [9:0] error_o
);
    logic [NumPorts-1:0] live_q, live_n, live_bad_q, live_bad_n;
    logic [63:0] live_id_q [NumPorts], live_id_n [NumPorts];
    logic owned_q, owned_n, abort_q, abort_n, has_a_q, has_a_n;
    logic window_q, window_n, closed_q, closed_n;
    logic [1:0] started_q, started_n, finished_q, finished_n;
    logic [9:0] error_q, error_n;
    logic [63:0] tick_q, tick_n;
    logic [63:0] words_q [32], words_n [32];
    logic carry_q, carry_n;
    logic both_done, lanes_idle;

    assign both_done = finished_q[0] && (!has_a_q || finished_q[1]);
    assign busy_o = owned_q && !abort_q && !both_done;
    assign lanes_idle = !live_q[0] && !live_bad_q[0] &&
        (!has_a_i || (!live_q[1] && !live_bad_q[1]));
    // ARM can select A; clear must also retain the previously selected A.
    assign ready_o = !busy_o && quiet_i && lanes_idle &&
        (!has_a_q || (!live_q[1] && !live_bad_q[1]));
    assign a_complete_o = owned_q && !abort_q && has_a_q && finished_q[1] &&
        words_q[11] == 4096 && words_q[12] == 4096 && !live_q[1] &&
        !live_bad_q[1] && error_q == 0 && !issue_i[1] && !done_i[1];
    assign error_o = error_q;
    always_comb begin
        data_o = words_q[index_i];
        case (index_i)
            0: data_o = 64'h49444d4150344d02;
            1, 31: data_o = 0;
            2: data_o = {58'd0, has_a_q, finished_q[1], finished_q[0], closed_q,
                         window_q, owned_q && !abort_q && !window_q && !closed_q};
            3: data_o = {54'd0, error_q};
            default: ;
        endcase
    end

    task automatic bump(input int slot);
        if (&words_q[slot]) error_n[4] = 1'b1;
        else words_n[slot] = words_q[slot] + 1'b1;
    endtask

    always_comb begin
        live_n = live_q; live_bad_n = live_bad_q;
        for (int i=0; i<NumPorts; i++) begin
            live_id_n[i] = live_id_q[i];
            // Always consume actual events, even on abort/clear/ARM edges.
            if (done_i[i]) begin
                if (!live_q[i] || done_id_i[i] == 0 || done_id_i[i] != live_id_q[i])
                    live_bad_n[i] = 1;
                live_n[i] = 0; live_id_n[i] = 0;
            end
            if (issue_i[i]) begin
                if (live_n[i] || issue_id_i[i] == 0) live_bad_n[i] = 1;
                live_n[i] = 1; live_id_n[i] = issue_id_i[i];
            end
        end
        owned_n=owned_q; abort_n=abort_q; has_a_n=has_a_q;
        window_n=window_q; closed_n=closed_q;
        started_n=started_q; finished_n=finished_q;
        error_n=error_q | reject_i; tick_n=tick_q; carry_n=carry_q;
        for (int i=0; i<32; i++) words_n[i]=words_q[i];

        if (owned_q && !abort_q) begin
            if (!both_done) begin
                if (&tick_q) error_n[4]=1;
                else tick_n=tick_q+1'b1;
            end
            for (int role=0; role<2; role++) begin
                if (role==0 || has_a_q) begin
                    if (live_bad_n[role]) begin
                        if (issue_i[role]) error_n[1]=1;
                        if (done_i[role]) error_n[2]=1;
                    end
                    if (!finished_q[role] &&
                        (64'(target_i[role]) != words_q[4+role] ||
                         64'(gap_cycles_i[role]) != words_q[6+role])) error_n[9]=1;
                    if (started_q[role] && !finished_q[role] && !batch_active_i[role]) error_n[3]=1;
                    if (finished_q[role] && (issue_i[role] || done_i[role])) error_n[3]=1;
                    if (!finished_q[role]) begin
                        if (started_q[role]) words_n[10+3*role]=tick_n-words_q[27+2*role];
                        if (gap_active_i[role] && started_q[role]) bump(14+role);
                        if (issue_i[role]) begin
                            if (issue_id_i[role] != words_q[8+3*role]+1) error_n[1]=1;
                            if (words_q[8+3*role] >= words_q[4+role]) error_n[3]=1;
                            bump(8+3*role);
                            if (!started_q[role]) begin
                                started_n[role]=1;
                                words_n[27+2*role]=tick_n;
                                words_n[10+3*role]=0;
                            end
                        end
                        if (done_i[role]) begin
                            if (words_q[9+3*role] >= words_q[4+role]) error_n[3]=1;
                            bump(9+3*role);
                            if (words_n[9+3*role] == words_q[4+role]) begin
                                finished_n[role]=1;
                                words_n[28+2*role]=tick_n;
                            end
                        end
                    end
                end
            end

            if (window_q) begin
                bump(16);
                if (has_a_q && issue_i[1]) bump(17);
                if (has_a_q && done_i[1]) begin
                    bump(18);
                    if (words_q[18]==0) words_n[25]=done_id_i[1];
                    words_n[26]=done_id_i[1];
                    if (carry_q && done_id_i[1]==words_q[23]) words_n[21]=1;
                end
            end
            if (open_i) begin
                if (window_q || closed_q || !issue_i[0] || words_q[8] != 0) error_n[5]=1;
                window_n=1;
                words_n[19]=has_a_q ? 64'(live_n[1]) : 0;
                words_n[22]=has_a_q ? 64'(issue_i[1]) : 0;
                words_n[23]=has_a_q && live_n[1] ? live_id_n[1] : 0;
                carry_n=has_a_q && live_n[1] && !issue_i[1];
            end
            if (close_i) begin
                if (!window_q || closed_q || !done_i[0] || words_n[9] != 4096 ||
                    words_n[16] != window_cycles_i) error_n[5]=1;
                window_n=0; closed_n=1;
                words_n[20]=has_a_q ? 64'(live_n[1]) : 0;
                words_n[24]=has_a_q && live_n[1] ? live_id_n[1] : 0;
                if (words_n[19]+words_n[17] != words_n[18]+words_n[20] ||
                    words_n[19]>1 || words_n[20]>1 ||
                    (words_n[21]!=0 && (!carry_q || words_n[23]==0))) error_n[6]=1;
            end
        end

        // Measurement-control priority does NOT supersede the continuous tracker.
        // Abort excludes its edge from measured sums, freezes invalid prior data,
        // while live_n still consumes same-edge issue/done and later drain events.
        if (abort_i) begin
            for (int i=0; i<32; i++) words_n[i]=words_q[i];
            started_n=started_q; finished_n=finished_q; tick_n=tick_q;
            window_n=0; abort_n=1; error_n=error_q | reject_i | 10'h080;
        end else if (clear_i || arm_i) begin
            for (int i=0; i<32; i++) words_n[i]=0;
            started_n=0; finished_n=0; tick_n=0; error_n=0;
            window_n=0; closed_n=0; carry_n=0; abort_n=0;
            owned_n=arm_i; has_a_n=arm_i && has_a_i;
            if (arm_i) begin
                words_n[4]=64'(target_i[0]); words_n[6]=64'(gap_cycles_i[0]);
                if (has_a_i) begin words_n[5]=64'(target_i[1]); words_n[7]=64'(gap_cycles_i[1]); end
            end
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            live_q<=0; live_bad_q<=0; owned_q<=0; abort_q<=0; has_a_q<=0;
            window_q<=0; closed_q<=0; started_q<=0; finished_q<=0;
            error_q<=0; tick_q<=0; carry_q<=0;
            for (int i=0; i<NumPorts; i++) live_id_q[i]<=0;
            for (int i=0; i<32; i++) words_q[i]<=0;
        end else begin
            live_q<=live_n; live_bad_q<=live_bad_n; owned_q<=owned_n; abort_q<=abort_n; has_a_q<=has_a_n;
            window_q<=window_n; closed_q<=closed_n; started_q<=started_n; finished_q<=finished_n;
            error_q<=error_n; tick_q<=tick_n; carry_q<=carry_n;
            for (int i=0; i<NumPorts; i++) live_id_q[i]<=live_id_n[i];
            for (int i=0; i<32; i++) words_q[i]<=words_n[i];
        end
    end
endmodule
