// SPDX-License-Identifier: Solderpad-0.51
// Passive count of accepted V reads that have not been selected for service.
// Selection removes a request here even though the AX FIFO retains its head
// until COMPLETE. No signal from this module changes normal arbitration.
module rv_iommu_e3_queue #(
    parameter int unsigned Depth = 4
) (
    input logic clk_i, rst_ni,
    input logic ar_fire_i,
    input logic [7:0] ar_id_i,
    input logic [23:0] ar_did_i,
    input logic select_i, select_write_i,
    input logic [7:0] select_id_i,
    input logic [23:0] select_did_i,
    output logic [31:0] waiting_o,
    output logic error_o
);
    localparam int unsigned Width = $clog2(Depth + 1) > 0 ? $clog2(Depth + 1) : 1;
    logic [Width-1:0] count_q;
    logic push, pop;
    assign push = ar_fire_i && ar_id_i == 0 && ar_did_i == 24'd10;
    assign pop = select_i && !select_write_i && select_id_i == 0 && select_did_i == 24'd10;
    assign waiting_o = 32'(count_q);
    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            count_q <= 0;
            error_o <= 0;
        end else begin
            case ({push, pop})
                2'b10: if (count_q < Depth) count_q <= count_q + 1'b1;
                       else error_o <= 1;
                2'b01: if (count_q != 0) count_q <= count_q - 1'b1;
                       else error_o <= 1;
                2'b11: if (count_q == 0) error_o <= 1; // Registered, non-fall-through FIFO.
                default: ;
            endcase
        end
    end
endmodule
