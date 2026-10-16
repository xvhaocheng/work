// SPDX-License-Identifier: Solderpad-0.51
//
// Structural top for the non-block observer. Mode 2 by default; +define+NB_MODE1
// selects the Mode 1 variant. No SV timing constructs.
//
`ifdef NB_MODE1
`define NB_MODE_VAL 1
`else
`define NB_MODE_VAL 2
`endif

module nonblock_observer_top (
    input  logic        clk,
    input  logic        rst,
    input  logic        arm,
    input  logic        open,
    input  logic        close,
    input  logic [7:0]  index,
    output logic [63:0] rdata,
    input  logic        accept,
    input  logic        fast,
    input  logic [23:0] did,
    input  logic [5:0]  ttype,
    input  logic [15:0] diag_events
);
    dma_nonblock_observer #(.Mode(`NB_MODE_VAL)) dut (
        .clk    (clk),
        .rst    (rst),
        .arm    (arm),
        .open   (open),
        .close  (close),
        .index  (index),
        .rdata  (rdata),
        .accept (accept),
        .fast   (fast),
        .did    (did),
        .ttype  (ttype),
        .diag_events(diag_events)
    );
endmodule
