// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// Bounded scalar-I/O harness for the v5 idma_fullbuffer_certificate.
//
// Two real DUT instances are built in one top:
//   * CapacityBytes = 4096  (positive-capacity variant)
//   * CapacityBytes = 24    (always-no-grant variant)
//
// No clock generation, no `initial` stimulus, no behavioural models.
// All clocks, stimulus, and pre-edge checking live in
// tests/fullbuffer_certificate_test.cpp.

module fullbuffer_certificate_test_top #(
    parameter unsigned CapacityBytesLarge = 4096,
    parameter unsigned CapacityBytesSmall = 24
) (
    input  logic        clk_i,
    input  logic        rst_ni,

    input  logic        cap4096_job_fire_i,
    input  logic        cap4096_job_done_i,
    input  logic        cap4096_normal_copy_i,
    input  logic [63:0] cap4096_length_i,
    input  logic [63:0] cap4096_src_i,
    input  logic [63:0] cap4096_dst_i,
    output logic        cap4096_permit_o,

    input  logic        cap24_job_fire_i,
    input  logic        cap24_job_done_i,
    input  logic        cap24_normal_copy_i,
    input  logic [63:0] cap24_length_i,
    input  logic [63:0] cap24_src_i,
    input  logic [63:0] cap24_dst_i,
    output logic        cap24_permit_o
);

    idma_fullbuffer_certificate #(
        .CapacityBytes (CapacityBytesLarge)
    ) i_cap4096 (
        .clk_i,
        .rst_ni,
        .job_fire_i    (cap4096_job_fire_i),
        .job_done_i    (cap4096_job_done_i),
        .normal_copy_i (cap4096_normal_copy_i),
        .length_i      (cap4096_length_i),
        .src_i         (cap4096_src_i),
        .dst_i         (cap4096_dst_i),
        .permit_o      (cap4096_permit_o)
    );

    idma_fullbuffer_certificate #(
        .CapacityBytes (CapacityBytesSmall)
    ) i_cap24 (
        .clk_i,
        .rst_ni,
        .job_fire_i    (cap24_job_fire_i),
        .job_done_i    (cap24_job_done_i),
        .normal_copy_i (cap24_normal_copy_i),
        .length_i      (cap24_length_i),
        .src_i         (cap24_src_i),
        .dst_i         (cap24_dst_i),
        .permit_o      (cap24_permit_o)
    );

endmodule
