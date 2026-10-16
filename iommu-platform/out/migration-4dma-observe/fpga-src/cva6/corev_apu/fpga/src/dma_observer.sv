// SPDX-License-Identifier: Solderpad-0.51
// Passive E4 observer. No output controls the DMA or AXI data paths.
module dma_observer #(
    parameter int unsigned NumPorts = 4,
    parameter int unsigned CounterWidth = 64,
    parameter int unsigned OutstandingWidth = 8
) (
    input logic clk_i, rst_ni,
    input logic cfg_valid_i, cfg_write_i,
    input logic [6:0] cfg_addr_i,
    input logic [63:0] cfg_wdata_i,
    input logic [7:0] cfg_wstrb_i,
    output logic [63:0] cfg_rdata_o,
    input logic [NumPorts-1:0] issue_i, done_i, poll_i,
    input logic [63:0] issue_id_i [NumPorts],
    input logic [63:0] done_id_i [NumPorts],
    input logic [63:0] poll_id_i [NumPorts],
    input logic [NumPorts-1:0] ar_valid_i, aw_valid_i, ar_ready_i, aw_ready_i,
    input logic [NumPorts-1:0] rlast_fire_i, b_fire_i, bad_id_i,
    input logic shared_ar_valid_i, shared_aw_valid_i,
    input logic shared_ar_ready_i, shared_aw_ready_i
);
    // Low byte is interface version; a missing observer must never look valid.
    localparam logic [63:0] Magic = 64'h49444d414f425301; // IDMAOBS, v1
    localparam int Count = 34;
    typedef enum logic [2:0] {OFF, ARMED, HARDWARE, POLL_WAIT, REFILL, FROZEN} phase_t;
    phase_t phase_q, phase_n;
    logic [CounterWidth-1:0] count_q [Count], count_n [Count];
    logic [CounterWidth-1:0] segment_q, segment_n;
    logic [OutstandingWidth-1:0] reads_q [NumPorts], reads_n [NumPorts];
    logic [OutstandingWidth-1:0] writes_q [NumPorts], writes_n [NumPorts];
    logic [NumPorts-1:0] track_bad_q, track_bad_n;
    logic [31:0] error_q, error_n;
    logic [63:0] target_q, target_n, expected_q, expected_n;
    logic [5:0] index_q, index_n;
    logic [1:0] victim_q, victim_n, aggressor_q, aggressor_n;
    logic has_a_q, has_a_n;
    logic active, v_pending, a_pending;

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
                cfg_rdata_o[1] = active;
                cfg_rdata_o[2] = phase_q == FROZEN;
                cfg_rdata_o[63:32] = error_q;
            end
            7'h68: cfg_rdata_o = target_q;
            7'h70: cfg_rdata_o = 64'(index_q);
            7'h78: if (int'(index_q) < Count) cfg_rdata_o = 64'(count_q[index_q]);
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
                        phase_n = POLL_WAIT;
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
                7'h68: if (!active && phase_q != ARMED) target_n = cfg_wdata_i;
                         else error_n[0] = 1'b1;
                7'h70: if (cfg_wdata_i < 64'(Count)) index_n = cfg_wdata_i[5:0];
                         else error_n[0] = 1'b1;
                7'h60: begin
                    if (cfg_wdata_i == 2) begin // Abort observation, never DMA.
                        if (active || phase_q == ARMED) begin
                            phase_n = FROZEN;
                            error_n[4] = 1'b1;
                        end
                    end else if (cfg_wdata_i == 4) begin
                        if (active || phase_q == ARMED) error_n[0] = 1'b1;
                        else begin phase_n = OFF; error_n = 0; end
                    end else if ((cfg_wdata_i & ~64'h0000000001030301) == 0 && cfg_wdata_i[0] &&
                                 !active && phase_q != ARMED && target_q != 0 && target_q <= 64'h100000000 &&
                                 int'(cfg_wdata_i[9:8]) < NumPorts && int'(cfg_wdata_i[17:16]) < NumPorts &&
                                 (!cfg_wdata_i[24] || cfg_wdata_i[9:8] != cfg_wdata_i[17:16])) begin
                        phase_n = ARMED;
                        error_n = 0;
                        victim_n = cfg_wdata_i[9:8];
                        aggressor_n = cfg_wdata_i[17:16];
                        has_a_n = cfg_wdata_i[24];
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
            track_bad_q <= track_bad_n;
            for (int i = 0; i < Count; i++) count_q[i] <= count_n[i];
            for (int i = 0; i < NumPorts; i++) begin reads_q[i] <= reads_n[i]; writes_q[i] <= writes_n[i]; end
        end
    end
endmodule
