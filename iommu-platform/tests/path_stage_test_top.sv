module path_stage_test_top (
    input logic clk_i, rst_ni, clear_i, run_i, active_i, enter_i, leave_i,
    input logic [7:0] time_i,
    output logic error_o,
    output logic [63:0] data_o [8]
);
    dma_path_stage #(.TimeWidth(8), .SumWidth(9), .CountWidth(3)) i_stage (
        .clk_i, .rst_ni, .clear_i, .run_i, .active_i,
        .open_i(1'b0), .close_i(1'b0), .enter_i, .leave_i, .time_i,
        .error_o, .data_o
    );
endmodule
