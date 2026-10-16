// SPDX-License-Identifier: Solderpad-0.51
// Permission for one complete, aligned 4 KiB copy in a sufficiently large
// data buffer. Samples accepted frontend jobs and retired backend jobs only;
// it never changes either handshake. The register frontend serializes jobs.
// Unexpected overlap/retirement revokes this new permission until reset.
module idma_fullbuffer_certificate #(
    parameter int unsigned CapacityBytes = 0
) (
    input logic clk_i, rst_ni,
    input logic job_fire_i, job_done_i, normal_copy_i,
    input logic [63:0] length_i, src_i, dst_i,
    output logic permit_o
);
    logic pending_q, eligible_q, fault_q;
    logic eligible;
    assign eligible = CapacityBytes >= 4096 && normal_copy_i &&
                      length_i == 64'd4096 && src_i[11:0] == 0 && dst_i[11:0] == 0;
    assign permit_o = pending_q && eligible_q && !fault_q && normal_copy_i;

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            pending_q <= 1'b0;
            eligible_q <= 1'b0;
            fault_q <= 1'b0;
        end else begin
            case ({job_fire_i, job_done_i})
                2'b10: begin
                    if (pending_q) fault_q <= 1'b1;
                    pending_q <= 1'b1;
                    eligible_q <= eligible;
                end
                2'b01: begin
                    if (!pending_q) fault_q <= 1'b1;
                    pending_q <= 1'b0;
                    eligible_q <= 1'b0;
                end
                2'b11: begin
                    // Replace a completed sole job; an immediately completed
                    // job with no predecessor needs no read permission.
                    pending_q <= pending_q;
                    eligible_q <= pending_q && eligible;
                end
                default: ;
            endcase
        end
    end
endmodule
