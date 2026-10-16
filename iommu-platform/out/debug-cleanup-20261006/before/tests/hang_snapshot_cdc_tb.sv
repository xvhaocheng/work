// SPDX-License-Identifier: Solderpad-0.51
// Asynchronous-clock contract test: sample on request, hold while shifting,
// reject an unacknowledged capture when the SoC clock is stopped, then recover.
module hang_snapshot_cdc_tb;
    timeunit 1ns;
    timeprecision 1ps;
    bit clk = 0, scan_clk = 0, run_soc = 1;
    bit rst_n = 0, scan_reset = 1;
    bit capture = 0, shift = 0, update = 0, tdi = 0;
    wire tdo;
    logic [1023:0] sample_data = '0;
    logic [1087:0] frame, frame_again, unused;
    bit ignored;
    always #10 clk = run_soc ? ~clk : 0;
    always #503 scan_clk = ~scan_clk;
    hang_snapshot_cdc dut (
        .clk_i(clk), .rst_ni(rst_n), .sample_i(sample_data),
        .scan_clk_i(scan_clk), .scan_reset_i(scan_reset), .select_i(1'b1),
        .capture_i(capture), .shift_i(shift), .update_i(update),
        .tdi_i(tdi), .tdo_o(tdo)
    );
    task automatic edge_scan(input bit c, s, u, d, output bit q);
        @(negedge scan_clk);
        capture = c; shift = s; update = u; tdi = d;
        #1; q = tdo;
        @(posedge scan_clk); #1;
    endtask
    task automatic idle_scan;
        repeat (16) edge_scan(0, 0, 0, 0, ignored);
    endtask
    task automatic dr(input logic [1087:0] command, output logic [1087:0] response);
        bit q;
        edge_scan(1, 0, 0, 0, q);
        for (int i = 0; i < 1088; i++) begin
            edge_scan(0, 1, 0, command[i], q);
            response[i] = q;
        end
        edge_scan(0, 0, 1, 0, q);
        edge_scan(0, 0, 0, 0, q);
    endtask
    task automatic valid_frame(input logic [1087:0] f, input logic [1023:0] expected);
        if (f[31:0] != 32'h48414e47 || f[39:32] != 1 ||
            !f[40] || !f[41] || f[42] != f[43]) $fatal(1, "invalid header");
        if (f[1087:64] != expected) $fatal(1, "snapshot torn or sampled at capture");
    endtask
    initial begin
        #77; rst_n = 1;
        @(negedge scan_clk); scan_reset = 0;
        dr('0, frame);
        if (frame[40]) $fatal(1, "valid without request");
        sample_data = {32{32'h12345678}};
        dr(1088'd1, unused);
        idle_scan();
        sample_data = {32{32'h87654321}};
        dr('0, frame);
        valid_frame(frame, {32{32'h12345678}});
        dr('0, frame_again);
        if (frame_again != frame) $fatal(1, "read changed held snapshot");
        dr(1088'd1, unused);
        idle_scan();
        dr('0, frame_again);
        valid_frame(frame_again, {32{32'h87654321}});
        if (frame_again[63:44] != frame[63:44] + 20'd1) $fatal(1, "sequence did not advance");
        @(negedge clk); run_soc = 0;
        sample_data = {32{32'h55aa33cc}};
        dr(1088'd1, unused);
        idle_scan();
        dr('0, frame);
        if (frame[40] || frame[41] || frame[1087:64] != 0) $fatal(1, "stopped clock accepted stale sample");
        run_soc = 1;
        idle_scan();
        dr('0, frame);
        valid_frame(frame, {32{32'h55aa33cc}});
        $display("hang_snapshot_cdc_tb: PASS (hold, repeated reads, stopped clock, recovery)");
        $finish;
    end
    initial begin
        #20000000; $fatal(1, "bounded test timeout");
    end
endmodule
