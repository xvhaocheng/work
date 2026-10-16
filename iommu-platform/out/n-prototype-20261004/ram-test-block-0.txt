// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_fifo_ram_top.sv
//
// Test-only harness for the production FIFO FPGA-RAM storage equivalence test.
//
// This file contains NO production logic.  It only instantiates the real
// production modules that are under test:
//
//   DUT    idma_stream_fifo  Depth = 512, type_t = logic [7:0]
//          -> the (to be implemented) depth/type rule must make this instance
//             use the FPGA RAM storage branch (AsyncDpRam).
//   REF    stream_fifo       DEPTH = 512, T = logic [7:0], FORCE_FPGA_RAM = 0
//          -> historical register/distributed-RAM storage branch.
//   AB     stream_fifo       DEPTH = 512, T = logic [7:0], FORCE_FPGA_RAM = 1
//          -> same module as REF, ONLY the storage flag differs: direct A/B
//             anchor for the flag itself.
//   LANE3  idma_stream_fifo  Depth = 3,  type_t = logic [7:0]
//          -> lane guard: the opt-in rule is depth+type based, so a depth-3
//             lane must NOT be switched to RAM storage.
//
// ariane_pkg is TEST-ONLY here.  The production tree keeps
// ariane_pkg::FPGA_EN == 0, but pulling in the full CVA6 package would drag
// in the whole SoC configuration; this stub makes the value explicit and
// keeps the FORCE_FPGA_RAM = 0 reference a genuine non-RAM reference.
package ariane_pkg;
    localparam bit FPGA_EN = 1'b0;
endpackage : ariane_pkg


module nonblock_fifo_ram_top (
    input  logic        clk_i,
    input  logic        rst_ni,        // async, active low: resets ALL FIFOs
    input  logic        testmode_i,    // unused by the FIFO RTL, kept for fidelity

    // ---- shared 512 deep, 8 bit stream for DUT / REF / AB ---------------
    input  logic        flush_i,       // synchronous flush for DUT / REF / AB
    input  logic [7:0]  data_i,
    input  logic        valid_i,

    // DUT (production idma_stream_fifo, Depth 512)
    input  logic        dut_ready_i,   // downstream pop side
    output logic [7:0]  dut_data_o,
    output logic        dut_valid_o,
    output logic        dut_ready_o,   // upstream "not full"
    output logic [8:0]  dut_usage_o,

    // REF (production stream_fifo, Depth 512, FORCE_FPGA_RAM = 0)
    input  logic        ref_ready_i,
    output logic [7:0]  ref_data_o,
    output logic        ref_valid_o,
    output logic        ref_ready_o,
    output logic [8:0]  ref_usage_o,

    // AB (production stream_fifo, Depth 512, FORCE_FPGA_RAM = 1)
    input  logic        ab_ready_i,
    output logic [7:0]  ab_data_o,
    output logic        ab_valid_o,
    output logic        ab_ready_o,
    output logic [8:0]  ab_usage_o,

    // ---- independent depth-3 lane guard --------------------------------
    input  logic        lane3_flush_i,
    input  logic [7:0]  lane3_data_i,
    input  logic        lane3_valid_i,
    input  logic        lane3_ready_i,
    output logic [7:0]  lane3_data_o,
    output logic        lane3_valid_o,
    output logic        lane3_ready_o,
    output logic [1:0]  lane3_usage_o
);

    // -----------------------------------------------------------------
    // DUT: production iDMA stream FIFO, 512 x 8 bit.
    // -----------------------------------------------------------------
    idma_stream_fifo #(
        .Depth  ( 32'd512     ),
        .type_t ( logic [7:0] )
    ) i_dut_stream_fifo (
        .clk_i      ( clk_i       ),
        .rst_ni     ( rst_ni      ),
        .flush_i    ( flush_i     ),
        .testmode_i ( testmode_i  ),
        .usage_o    ( dut_usage_o ),
        .data_i     ( data_i      ),
        .valid_i    ( valid_i     ),
        .ready_o    ( dut_ready_o ),
        .data_o     ( dut_data_o  ),
        .valid_o    ( dut_valid_o ),
        .ready_i    ( dut_ready_i )
    );

    // -----------------------------------------------------------------
    // REF: exactly the same production FIFO module, RAM storage OFF.
    // -----------------------------------------------------------------
    stream_fifo #(
        .DEPTH          ( 32'd512     ),
        .T              ( logic [7:0] ),
        .FORCE_FPGA_RAM ( 1'b0        )
    ) i_ref_stream_fifo (
        .clk_i      ( clk_i       ),
        .rst_ni     ( rst_ni      ),
        .flush_i    ( flush_i     ),
        .testmode_i ( testmode_i  ),
        .usage_o    ( ref_usage_o ),
        .data_i     ( data_i      ),
        .valid_i    ( valid_i     ),
        .ready_o    ( ref_ready_o ),
        .data_o     ( ref_data_o  ),
        .valid_o    ( ref_valid_o ),
        .ready_i    ( ref_ready_i )
    );

    // -----------------------------------------------------------------
    // AB: direct flag A/B anchor - identical module, RAM storage ON.
    // -----------------------------------------------------------------
    stream_fifo #(
        .DEPTH          ( 32'd512     ),
        .T              ( logic [7:0] ),
        .FORCE_FPGA_RAM ( 1'b1        )
    ) i_ab_stream_fifo (
        .clk_i      ( clk_i      ),
        .rst_ni     ( rst_ni     ),
        .flush_i    ( flush_i    ),
        .testmode_i ( testmode_i ),
        .usage_o    ( ab_usage_o ),
        .data_i     ( data_i     ),
        .valid_i    ( valid_i    ),
        .ready_o    ( ab_ready_o ),
        .data_o     ( ab_data_o  ),
        .valid_o    ( ab_valid_o ),
        .ready_i    ( ab_ready_i )
    );

    // -----------------------------------------------------------------
    // LANE3: depth-3 lane guard (must stay on the non-RAM branch).
    // -----------------------------------------------------------------
    idma_stream_fifo #(
        .Depth  ( 32'd3       ),
        .type_t ( logic [7:0] )
    ) i_lane3_stream_fifo (
        .clk_i      ( clk_i         ),
        .rst_ni     ( rst_ni        ),
        .flush_i    ( lane3_flush_i ),
        .testmode_i ( testmode_i    ),
        .usage_o    ( lane3_usage_o ),
        .data_i     ( lane3_data_i  ),
        .valid_i    ( lane3_valid_i ),
        .ready_o    ( lane3_ready_o ),
        .data_o     ( lane3_data_o  ),
        .valid_o    ( lane3_valid_o ),
        .ready_i    ( lane3_ready_i )
    );

endmodule : nonblock_fifo_ram_top
