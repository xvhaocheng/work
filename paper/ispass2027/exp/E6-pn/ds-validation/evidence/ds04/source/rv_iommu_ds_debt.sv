// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// Passive transport accounting for the optional PN configuration guard.
// These are transaction debts, not advertised credits. No output gates AXI.
module rv_iommu_ds_debt (
    input  logic clk_i,
    input  logic rst_ni,
    input  logic read_issue_i,
    input  logic read_last_i,
    input  logic write_issue_i,
    input  logic write_done_i,
    output logic quiet_o
);
    logic [7:0] read_q, read_n, write_q, write_n;
    logic bad_q, bad_n;

    always_comb begin
        read_n = read_q;
        write_n = write_q;
        bad_n = bad_q;
        // Simultaneous issue/retirement is a net zero, including at 0/255.
        unique case ({read_issue_i, read_last_i})
            2'b10: if (&read_q) bad_n = 1'b1;
                   else read_n = read_q + 8'd1;
            2'b01: if (read_q == '0) bad_n = 1'b1;
                   else read_n = read_q - 8'd1;
            default: ;
        endcase
        unique case ({write_issue_i, write_done_i})
            2'b10: if (&write_q) bad_n = 1'b1;
                   else write_n = write_q + 8'd1;
            2'b01: if (write_q == '0) bad_n = 1'b1;
                   else write_n = write_q - 8'd1;
            default: ;
        endcase
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            read_q <= '0;
            write_q <= '0;
            bad_q <= 1'b0;
        end else begin
            read_q <= read_n;
            write_q <= write_n;
            bad_q <= bad_n;
        end
    end
    // A range/protocol anomaly cannot be cleared by ARM, STOP or policy clear.
    assign quiet_o = !bad_q && (read_q == '0) && (write_q == '0);
endmodule
