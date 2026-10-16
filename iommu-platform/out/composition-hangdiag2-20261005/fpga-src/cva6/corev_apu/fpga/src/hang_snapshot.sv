// SPDX-License-Identifier: Solderpad-0.51
// Read-only, request/acknowledge snapshot. No output enters the functional SoC.
// A USER4 DR write with bit 0 set requests one sample; a zero write only reads.
// The sample is held until the next acknowledged request. Consumers must reject
// frames without VALID and READY. Data crosses only after two ACK sync stages.
module hang_snapshot_cdc #(
    parameter logic [7:0] FrameVersion = 8'd1
) (
    input logic clk_i, rst_ni,
    input logic [1023:0] sample_i,
    input logic scan_clk_i, scan_reset_i, select_i,
    input logic capture_i, shift_i, update_i, tdi_i,
    output logic tdo_o
);
    timeunit 1ns;
    timeprecision 1ps;
    logic request_q;
    (* ASYNC_REG = "TRUE" *) logic request_meta_q, request_sync_q;
    logic acknowledge_q, valid_q;
    logic [19:0] sequence_q;
    logic [1023:0] snapshot_q;
    (* ASYNC_REG = "TRUE" *) logic acknowledge_meta_q, acknowledge_sync_q;
    logic [1087:0] shift_q;

    always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
            request_meta_q <= 0;
            request_sync_q <= 0;
            acknowledge_q <= 0;
            valid_q <= 0;
            sequence_q <= 0;
            snapshot_q <= '0;
        end else begin
            request_meta_q <= request_q;
            request_sync_q <= request_meta_q;
            if (request_sync_q != acknowledge_q) begin
                snapshot_q <= sample_i;
                sequence_q <= sequence_q + 1'b1;
                valid_q <= 1;
                acknowledge_q <= request_sync_q;
            end
        end
    end

    // BSCANE2 RESET is the TAP reset, independent of the CPU and its bus.
    always_ff @(posedge scan_clk_i or posedge scan_reset_i) begin
        if (scan_reset_i) begin
            request_q <= 0;
            acknowledge_meta_q <= 0;
            acknowledge_sync_q <= 0;
            shift_q <= '0;
        end else begin
            acknowledge_meta_q <= acknowledge_q;
            acknowledge_sync_q <= acknowledge_meta_q;
            if (select_i && update_i && shift_q[0] &&
                acknowledge_sync_q == request_q)
                request_q <= ~request_q;
            if (select_i && capture_i) begin
                if (acknowledge_sync_q == request_q)
                    shift_q <= {snapshot_q, sequence_q, acknowledge_sync_q,
                        request_q, 1'b1, valid_q, FrameVersion, 32'h48414e47};
                else
                    shift_q <= {1024'b0, 20'b0, acknowledge_sync_q,
                        request_q, 1'b0, 1'b0, FrameVersion, 32'h48414e47};
            end else if (select_i && shift_i)
                shift_q <= {tdi_i, shift_q[1087:1]};
        end
    end
    assign tdo_o = shift_q[0];
endmodule

`ifdef IOMMU_HANG_DIAG
module hang_snapshot #(
    parameter logic [7:0] FrameVersion = 8'd1
) (
    input logic clk_i, rst_ni,
    input logic [1023:0] sample_i
);
    timeunit 1ns;
    timeprecision 1ps;
    wire capture, shift, update, select, scan_reset, scan_tck, scan_clk, tdi, tdo;
    BSCANE2 #(.JTAG_CHAIN(4)) i_bscan (
        .CAPTURE(capture), .DRCK(), .RESET(scan_reset), .RUNTEST(),
        .SEL(select), .SHIFT(shift), .TCK(scan_tck), .TDI(tdi), .TMS(),
        .UPDATE(update), .TDO(tdo)
    );
    BUFG i_scan_clock (.I(scan_tck), .O(scan_clk));
    hang_snapshot_cdc #(.FrameVersion(FrameVersion)) i_cdc (
        .clk_i, .rst_ni, .sample_i, .scan_clk_i(scan_clk),
        .scan_reset_i(scan_reset), .select_i(select), .capture_i(capture),
        .shift_i(shift), .update_i(update), .tdi_i(tdi), .tdo_o(tdo)
    );
endmodule
`endif
