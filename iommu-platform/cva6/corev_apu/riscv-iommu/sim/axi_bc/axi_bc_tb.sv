// Independent Verilator wrapper for rv_iommu_axi_bc.
//
// * AXI address is 64 bit wide (the DUT is instantiated with addr_t = logic [63:0]).
// * The wrapped DUT is purely combinational: the C++ driver only drives the
//   inputs and calls eval().  There is no clock and there are no timing
//   constructs, so the test is built with --cc --exe --build only.
//
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

`default_nettype none

module axi_bc_tb (
    // AxVALID / gate of the checker
    input  logic        check_i,
    // AxADDR
    input  logic [63:0] addr_i,
    // AxBURST
    input  logic [1:0]  burst_type_i,
    // AxLEN
    input  logic [7:0]  burst_length_i,
    // AxSIZE
    input  logic [2:0]  n_bytes_i,
    // 1 = request allowed, 0 = request rejected
    output logic        allow_request_o
);

    rv_iommu_axi_bc #(
        .addr_t ( logic [63:0] )
    ) i_axi_bc (
        .check_i         ( check_i         ),
        .addr_i          ( addr_i          ),
        .burst_type_i    ( burst_type_i    ),
        .burst_length_i  ( burst_length_i  ),
        .n_bytes_i       ( n_bytes_i       ),
        .allow_request_o ( allow_request_o )
    );

endmodule

`default_nettype wire
