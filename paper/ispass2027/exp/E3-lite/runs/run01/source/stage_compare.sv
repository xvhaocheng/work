// Test-only wiring: actual candidate versus modules renamed from fixed 1d587.
module stage_compare #(
    parameter int Depth = 64,
    parameter bit HistEnable = 1
) (
    input logic clk_i, rst_ni, clear_i, run_i, active_i, open_i, close_i,
    input logic enter_i, leave_i,
    input logic [31:0] time_i,
    input logic path_arm_i, path_zero_i, path_abort_i,
    output logic [2:0] equal_o,
    output logic error_o, overflow_o, small_error_o, small_overflow_o,
    output logic [63:0] data_o [8],
    output logic [31:0] hist_o [32],
    output logic [2:0] small_hist_o [32],
    output logic [63:0] memory_o [Depth], gold_memory_o [Depth],
    output logic [6:0] count_o, head_o, tail_o,
    output logic bad_o, path_error_o,
    output logic [15:0] path_overflow_o,
    output logic [63:0] path_bin_o
);
    logic gold_error, gold_overflow, gold_small_error, gold_small_overflow;
    logic [63:0] gold_data [8], small_data [8], gold_small_data [8];
    logic [31:0] gold_hist [32];
    logic [2:0] gold_small_hist [32];
    dma_path_stage #(.Depth(Depth), .HistEnable(HistEnable)) candidate (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i, .open_i, .close_i,
        .enter_i, .leave_i, .time_i, .error_o, .data_o, .hist_o,
        .hist_overflow_o(overflow_o)
    );
    dma_path_stage_gold #(.Depth(Depth), .HistEnable(HistEnable)) reference_dut (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i, .open_i, .close_i,
        .enter_i, .leave_i, .time_i, .error_o(gold_error), .data_o(gold_data),
        .hist_o(gold_hist), .hist_overflow_o(gold_overflow)
    );
    // Narrow test-only counters force real saturation/overflow in finite time.
    dma_path_stage #(.Depth(Depth), .HistEnable(HistEnable), .TimeWidth(8),
                     .SumWidth(8), .CountWidth(3), .HistogramCountWidth(3)) small (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i, .open_i, .close_i,
        .enter_i, .leave_i, .time_i(time_i[7:0]), .error_o(small_error_o),
        .data_o(small_data), .hist_o(small_hist_o), .hist_overflow_o(small_overflow_o)
    );
    dma_path_stage_gold #(.Depth(Depth), .HistEnable(HistEnable), .TimeWidth(8),
                          .SumWidth(8), .CountWidth(3), .HistogramCountWidth(3)) small_gold (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i, .open_i, .close_i,
        .enter_i, .leave_i, .time_i(time_i[7:0]), .error_o(gold_small_error),
        .data_o(gold_small_data), .hist_o(gold_small_hist), .hist_overflow_o(gold_small_overflow)
    );
    logic [7:0] same_data, same_small_data;
    logic [31:0] same_hist, same_small_hist;
    for (genvar f=0; f<8; f++) begin
        assign same_data[f] = data_o[f] == gold_data[f];
        assign same_small_data[f] = small_data[f] == gold_small_data[f];
    end
    for (genvar b=0; b<32; b++) begin
        assign same_hist[b] = hist_o[b] == gold_hist[b];
        assign same_small_hist[b] = small_hist_o[b] == gold_small_hist[b];
    end
    assign equal_o[0] = (&same_data) && (&same_hist) && error_o == gold_error &&
        overflow_o == gold_overflow && candidate.count_q == reference_dut.count_q &&
        candidate.head_q == reference_dut.head_q && candidate.tail_q == reference_dut.tail_q;
    assign equal_o[1] = (&same_small_data) && (&same_small_hist) &&
        small_error_o == gold_small_error && small_overflow_o == gold_small_overflow &&
        small.count_q == small_gold.count_q && small.head_q == small_gold.head_q && small.tail_q == small_gold.tail_q;
    assign count_o = 7'(candidate.count_q);
    assign head_o = 7'(candidate.head_q);
    assign tail_o = 7'(candidate.tail_q);
    assign bad_o = candidate.bad;
    for (genvar word_index=0; word_index<Depth; word_index++) begin
        if (Depth == 64) begin
            assign memory_o[word_index] = 64'(candidate.gen_queue_ram.entries[word_index]);
        end else begin
            assign memory_o[word_index] = 64'({candidate.born_in_window[word_index], candidate.stamps[word_index]});
        end
        assign gold_memory_o[word_index] = 64'({reference_dut.born_in_window[word_index], reference_dut.stamps[word_index]});
    end
    logic [3:0] ids [2];
    assign ids[0] = 0;
    assign ids[1] = 0;
    logic [63:0] gold_path_bin;
    logic [15:0] gold_path_overflow;
    logic gold_path_error;
    dma_path_observer #(.PathQueueDepth(Depth), .HistEnable(HistEnable), .TimeWidth(8),
                        .HistogramCountWidth(3)) path_candidate (
        .clk_i, .rst_ni, .clear_i, .arm_i(path_arm_i), .active_i, .open_i, .close_i,
        .abort_i(path_abort_i), .victim_i(2'd0), .aggressor_i(2'd1), .has_a_i(1'b0), .idle_i(1'b1),
        .in_valid_i({4'b0, 3'b0, path_zero_i}), .in_ready_i(8'hff),
        .tr_valid_i({1'b0, path_zero_i}), .tr_ready_i(2'b11),
        .out_valid_i({1'b0, path_zero_i}), .out_ready_i(2'b11),
        .return_i({1'b0, path_zero_i}), .tr_id_i(ids), .out_id_i(ids), .return_id_i(ids),
        .index_i(7'd0), .hist_index_i(9'd0), .data_o(), .hist_data_o(path_bin_o),
        .hist_overflow_o(path_overflow_o), .error_o(path_error_o)
    );
    dma_path_observer_gold #(.PathQueueDepth(Depth), .HistEnable(HistEnable), .TimeWidth(8),
                             .HistogramCountWidth(3)) path_gold (
        .clk_i, .rst_ni, .clear_i, .arm_i(path_arm_i), .active_i, .open_i, .close_i,
        .abort_i(path_abort_i), .victim_i(2'd0), .aggressor_i(2'd1), .has_a_i(1'b0), .idle_i(1'b1),
        .in_valid_i({4'b0, 3'b0, path_zero_i}), .in_ready_i(8'hff),
        .tr_valid_i({1'b0, path_zero_i}), .tr_ready_i(2'b11),
        .out_valid_i({1'b0, path_zero_i}), .out_ready_i(2'b11),
        .return_i({1'b0, path_zero_i}), .tr_id_i(ids), .out_id_i(ids), .return_id_i(ids),
        .index_i(7'd0), .hist_index_i(9'd0), .data_o(), .hist_data_o(gold_path_bin),
        .hist_overflow_o(gold_path_overflow), .error_o(gold_path_error)
    );
    logic [127:0] same_path_data;
    logic [511:0] same_path_hist;
    for (genvar l=0; l<4; l++) for (genvar s=0; s<4; s++) begin
        for (genvar f=0; f<8; f++) assign same_path_data[l*32+s*8+f] =
            path_candidate.result[l][s][f] == path_gold.result[l][s][f];
        for (genvar b=0; b<32; b++) assign same_path_hist[l*128+s*32+b] =
            path_candidate.histogram[l][s][b] == path_gold.histogram[l][s][b];
    end
    assign equal_o[2] = (&same_path_data) && (&same_path_hist) &&
        path_error_o == gold_path_error && path_overflow_o == gold_path_overflow &&
        path_bin_o == gold_path_bin && path_candidate.run_q == path_gold.run_q &&
        path_candidate.time_q == path_gold.time_q;
endmodule
