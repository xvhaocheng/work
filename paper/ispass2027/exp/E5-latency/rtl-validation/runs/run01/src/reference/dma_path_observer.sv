// SPDX-License-Identifier: Solderpad-0.51
// Passive, ordered stage scoreboard. All times use the existing observer clock.
module dma_path_stage_reference #(
    parameter int Depth = 4,
    parameter int TimeWidth = 32,
    parameter int SumWidth = 48,
    parameter int CountWidth = 32
) (
    input logic clk_i, rst_ni, clear_i, run_i, active_i, open_i, close_i,
    input logic enter_i, leave_i,
    input logic [TimeWidth-1:0] time_i,
    output logic error_o,
    output logic [63:0] data_o [8]
);
    localparam int PtrWidth = $clog2(Depth);
    logic [TimeWidth-1:0] stamps [Depth];
    logic born_in_window [Depth];
    logic [PtrWidth-1:0] head_q, tail_q;
    logic [PtrWidth:0] count_q, count_next;
    logic [CountWidth-1:0] entered_q, full_q, carry_q;
    logic [SumWidth-1:0] sum_q, area_q;
    logic [TimeWidth-1:0] max_q, age;
    logic [PtrWidth:0] opening_q, closing_q;
    logic matched, whole, bad;
    logic [SumWidth:0] sum_next, area_next;

    always_comb begin
        count_next = count_q;
        matched = leave_i && (count_q != 0 || enter_i);
        bad = (leave_i && !matched) || (enter_i && count_q == Depth && !leave_i);
        if (enter_i && !leave_i) count_next = count_q + 1'b1;
        if (leave_i && !enter_i) count_next = count_q - 1'b1;
        age = count_q == 0 ? '0 : time_i - stamps[head_q];
        whole = count_q == 0 ? active_i : born_in_window[head_q];
        sum_next = {1'b0, sum_q} + (SumWidth+1)'(age);
        area_next = {1'b0, area_q} + (SumWidth+1)'(count_q);
        data_o[0] = 64'(entered_q);
        data_o[1] = 64'(full_q);
        data_o[2] = 64'(carry_q);
        data_o[3] = 64'(sum_q);
        data_o[4] = 64'(max_q);
        data_o[5] = 64'(opening_q);
        data_o[6] = 64'(closing_q);
        data_o[7] = 64'(area_q);
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            head_q <= 0; tail_q <= 0; count_q <= 0; error_o <= 0;
            entered_q <= 0; full_q <= 0; carry_q <= 0;
            sum_q <= 0; max_q <= 0; area_q <= 0;
            opening_q <= 0; closing_q <= 0;
        end else if (clear_i) begin
            head_q <= 0; tail_q <= 0; count_q <= 0; error_o <= 0;
            entered_q <= 0; full_q <= 0; carry_q <= 0;
            sum_q <= 0; max_q <= 0; area_q <= 0;
            opening_q <= 0; closing_q <= 0;
        end else if (run_i) begin
            if (bad) error_o <= 1;
            if (!bad) begin
                count_q <= count_next;
                if (enter_i) begin
                    stamps[tail_q] <= time_i;
                    born_in_window[tail_q] <= active_i;
                    tail_q <= tail_q + 1'b1;
                end
                if (leave_i) head_q <= head_q + 1'b1;
                if (open_i) opening_q <= count_next;
                if (close_i) closing_q <= count_next;
                if (active_i) begin
                    area_q <= area_next[SumWidth-1:0];
                    if (area_next[SumWidth]) error_o <= 1;
                    if (enter_i) begin
                        entered_q <= entered_q + 1'b1;
                        if (&entered_q) error_o <= 1;
                    end
                    if (matched) begin
                        if (whole) begin
                            full_q <= full_q + 1'b1;
                            sum_q <= sum_next[SumWidth-1:0];
                            if (age > max_q) max_q <= age;
                            if (&full_q || sum_next[SumWidth]) error_o <= 1;
                        end else begin
                            carry_q <= carry_q + 1'b1;
                            if (&carry_q) error_o <= 1;
                        end
                    end
                end
            end
        end
    end
endmodule

module dma_path_observer_reference #(
    parameter int NumPorts = 4,
    parameter int AxiIdWidth = 4,
    parameter int TimeWidth = 32,
    parameter int SumWidth = 48,
    parameter int CountWidth = 32
) (
    input logic clk_i, rst_ni, clear_i, arm_i, active_i, open_i, close_i, abort_i,
    input logic [1:0] victim_i, aggressor_i,
    input logic has_a_i, idle_i,
    input logic [1:0][NumPorts-1:0] in_valid_i, in_ready_i,
    input logic [1:0] tr_valid_i, tr_ready_i, out_valid_i, out_ready_i, return_i,
    input logic [AxiIdWidth-1:0] tr_id_i [2], out_id_i [2], return_id_i [2],
    input logic [6:0] index_i,
    output logic [63:0] data_o,
    output logic error_o
);
    localparam int PrivateIdWidth = AxiIdWidth - $clog2(NumPorts);
    logic run_q, bad_q;
    logic [TimeWidth-1:0] time_q;
    logic [3:0] in_hold_q;
    logic [1:0] out_hold_q;
    logic [AxiIdWidth-1:0] out_held_id_q [2];
    logic [3:0][4:0] event_chain;
    logic [15:0] stage_error;
    logic [63:0] result [4][4][8];

    assign data_o = result[index_i[6:5]][index_i[4:3]][index_i[2:0]];
    assign error_o = bad_q || |stage_error;
    for (genvar lane = 0; lane < 4; lane++) begin : gen_lane
        localparam int Role = lane / 2;
        localparam int Channel = lane % 2; // read, write
        logic [1:0] port;
        logic [AxiIdWidth-1:0] id;
        logic enabled;
        assign port = Role == 0 ? victim_i : aggressor_i;
        assign id = AxiIdWidth'(port) << PrivateIdWidth;
        assign enabled = Role == 0 || has_a_i;
        assign event_chain[lane][0] = enabled && in_valid_i[Channel][port] && !in_hold_q[lane];
        assign event_chain[lane][1] = enabled && tr_valid_i[Channel] && tr_ready_i[Channel] && tr_id_i[Channel] == id;
        assign event_chain[lane][2] = enabled && out_valid_i[Channel] && !out_hold_q[Channel] && out_id_i[Channel] == id;
        assign event_chain[lane][3] = enabled && out_valid_i[Channel] && out_ready_i[Channel] && out_id_i[Channel] == id;
        assign event_chain[lane][4] = enabled && return_i[Channel] && return_id_i[Channel] == id;
        for (genvar stage = 0; stage < 4; stage++) begin : gen_stage
            dma_path_stage_reference #(.TimeWidth(TimeWidth), .SumWidth(SumWidth), .CountWidth(CountWidth)) i_stage (
                .clk_i, .rst_ni, .clear_i(clear_i || arm_i), .run_i(run_q),
                .active_i, .open_i, .close_i, .time_i(time_q),
                .enter_i(event_chain[lane][stage]), .leave_i(event_chain[lane][stage+1]),
                .error_o(stage_error[lane*4+stage]), .data_o(result[lane][stage])
            );
        end
        always_ff @(posedge clk_i or negedge rst_ni) begin
            if (!rst_ni) in_hold_q[lane] <= 0;
            else if (clear_i || arm_i) in_hold_q[lane] <= 0;
            else if (run_q) in_hold_q[lane] <= enabled && in_valid_i[Channel][port] && !in_ready_i[Channel][port];
        end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            run_q <= 0; bad_q <= 0; time_q <= 0; out_hold_q <= 0;
            for (int ch = 0; ch < 2; ch++) out_held_id_q[ch] <= 0;
        end else if (clear_i || arm_i) begin
            run_q <= arm_i;
            bad_q <= arm_i && !idle_i;
            time_q <= 0;
            out_hold_q <= 0;
        end else if (run_q) begin
            time_q <= time_q + 1'b1;
            // The entire armed session must fit in 2^32 clocks (85.9s at 50MHz).
            if (&time_q) bad_q <= 1;
            if (close_i || abort_i) run_q <= 0;
            for (int lane = 0; lane < 4; lane++) begin
                if (in_hold_q[lane] && !in_valid_i[lane%2][lane < 2 ? victim_i : aggressor_i]) bad_q <= 1;
            end
            for (int ch = 0; ch < 2; ch++) begin
                if (out_hold_q[ch] && (!out_valid_i[ch] || out_id_i[ch] != out_held_id_q[ch])) bad_q <= 1;
                out_hold_q[ch] <= out_valid_i[ch] && !out_ready_i[ch];
                out_held_id_q[ch] <= out_id_i[ch];
                // This platform emits private ID=0; high bits identify the DMA.
                if ((tr_valid_i[ch] && |tr_id_i[ch][PrivateIdWidth-1:0]) ||
                    (out_valid_i[ch] && |out_id_i[ch][PrivateIdWidth-1:0]) ||
                    (return_i[ch] && |return_id_i[ch][PrivateIdWidth-1:0])) bad_q <= 1;
            end
        end
    end
endmodule
