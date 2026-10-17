// Finite test interface around the actual production stage and path modules.
module stage_top (
    input logic clk_i, rst_ni, clear_i, run_i, active_i, open_i, close_i,
    input logic enter_i, leave_i,
    input logic [31:0] time_i,
    input logic path_arm_i, path_zero_i, path_abort_i,
    output logic error_o, overflow_o,
    output logic [63:0] data_o [8],
    output logic [31:0] hist_o [32],
    output logic [2:0] small_hist_o [32],
    output logic small_overflow_o, path_error_o,
    output logic [15:0] path_overflow_o,
    output logic [63:0] path_bin_o
);
    dma_path_stage #(.HistEnable(1)) i_stage (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i, .open_i, .close_i,
        .enter_i, .leave_i, .time_i, .error_o, .data_o,
        .hist_o, .hist_overflow_o(overflow_o)
    );
    dma_path_stage #(.HistEnable(1), .HistogramCountWidth(3)) i_small (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i, .open_i, .close_i,
        .enter_i, .leave_i, .time_i, .error_o(), .data_o(),
        .hist_o(small_hist_o), .hist_overflow_o(small_overflow_o)
    );
    logic [3:0] ids [2];
    assign ids[0] = 0;
    assign ids[1] = 0;
    dma_path_observer #(.HistEnable(1), .HistogramCountWidth(3), .TimeWidth(8)) i_path (
        .clk_i, .rst_ni, .clear_i, .arm_i(path_arm_i), .active_i,
        .open_i, .close_i, .abort_i(path_abort_i), .victim_i(2'd0), .aggressor_i(2'd1),
        .has_a_i(1'b0), .idle_i(1'b1),
        .in_valid_i({4'b0, 3'b0, path_zero_i}), .in_ready_i(8'hff),
        .tr_valid_i({1'b0, path_zero_i}), .tr_ready_i(2'b11),
        .out_valid_i({1'b0, path_zero_i}), .out_ready_i(2'b11),
        .return_i({1'b0, path_zero_i}), .tr_id_i(ids), .out_id_i(ids), .return_id_i(ids),
        .index_i(7'd0), .hist_index_i(9'd0), .data_o(), .hist_data_o(path_bin_o),
        .hist_overflow_o(path_overflow_o), .error_o(path_error_o)
    );
endmodule
