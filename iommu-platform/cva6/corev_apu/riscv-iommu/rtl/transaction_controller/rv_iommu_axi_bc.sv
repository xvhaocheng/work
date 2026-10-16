// Copyright © 2025 Manuel Rodríguez & Zero-Day Labs, Lda.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

// Licensed under the Solderpad Hardware License v 2.1 (the “License”); 
// you may not use this file except in compliance with the License, 
// or, at your option, the Apache License version 2.0. 
// You may obtain a copy of the License at https://solderpad.org/licenses/SHL-2.1/.
// Unless required by applicable law or agreed to in writing, 
// any work distributed under the License is distributed on an “AS IS” BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
//
// Author: Manuel Rodríguez <manuel.cederog@gmail.com>
// Date: 20/01/2025
// Acknowledges: SSRC - Technology Innovation Institute (TII)
//
// Description: AXI 4-kiB Boundary Checker module for RISC-V IOMMU: 
//              Checks whether an AXI transaction crosses a 4-kiB address boundary, 
//              which is illegal in AXI4 transactions.

module rv_iommu_axi_bc #(
    /// Address data type
    parameter type addr_t = logic
) (
    // AxVALID
    input  logic                check_i,
    // AxADDR
    input  addr_t addr_i,
    // AxBURST
    input  axi_pkg::burst_t     burst_type_i,
    // AxLEN
    input  axi_pkg::len_t       burst_length_i,
    // AxSIZE
    input  axi_pkg::size_t      n_bytes_i,

    // To indicate valid requests or boundary violations
    output logic                allow_request_o
);

    // Keep the page offset plus the largest AXI burst (256 * 128 bytes)
    // without overflow. AxSIZE is log2(bytes per beat), including size zero.
    logic [16:0] beat_bytes, burst_bytes, page_offset, aligned_offset;
    logic [16:0] fixed_faddr, incr_faddr;
    assign beat_bytes = 17'd1 << n_bytes_i;
    assign burst_bytes = (17'(burst_length_i) + 17'd1) << n_bytes_i;
    assign page_offset = {5'b0, addr_i[11:0]};
    // An unaligned first beat accesses only the remaining bytes of its
    // aligned beat window; subsequent INCR beats start at aligned addresses.
    assign aligned_offset = page_offset & ~(beat_bytes - 17'd1);
    // Compare the last included byte, not the address after the transfer.
    assign fixed_faddr = aligned_offset + beat_bytes - 17'd1;
    assign incr_faddr = aligned_offset + burst_bytes - 17'd1;

    logic [2:0] log2_len;
    logic [16:0] wrap_boundary;
    logic [16:0] wrap_faddr;
    always_comb begin : wrap_bursts
        
        // For wrap bursts, N of transfers must be {2, 4, 8, 16}
        // So, ARLEN must be {1, 3, 7, 15}
        unique case (burst_length_i)
            8'd1:    log2_len = 3'b001;
            8'd3:    log2_len = 3'b010;
            8'd7:    log2_len = 3'b011;
            8'd15:   log2_len = 3'b100;
            default: log2_len = 3'b111;  // invalid
        endcase

        // The lowest address within a wrapping burst
        // Wrap_Boundary = (INT(Start_Address / (Burst_Length x Number_Bytes))) x (Burst_Length x Number_Bytes)
        // Legal WRAP lengths make burst_bytes a power of two. Use a wide
        // mask so adding two 3-bit exponents cannot wrap at 8.
        wrap_boundary   = page_offset & ~(burst_bytes - 17'd1);

        // Last included byte of the complete wrapping window.
        wrap_faddr      = wrap_boundary + burst_bytes - 17'd1;
    end

    always_comb begin : boundary_check

        allow_request_o = 1'b0;

        unique case (burst_type_i)

            // FIXED repeats the same (possibly partial) aligned beat window.
            axi_pkg::BURST_FIXED: begin
                if (fixed_faddr < 17'd4096) begin
                    allow_request_o = check_i;
                end
            end

            // WRAP requires a legal length and a beat-aligned start address.
            axi_pkg::BURST_WRAP: begin
                if (!(&log2_len) && page_offset == aligned_offset &&
                    wrap_faddr < 17'd4096) begin
                    allow_request_o  = check_i;     // Allow transaction
                end
            end

            // INCR's complete aligned container must stay within this page.
            axi_pkg::BURST_INCR: begin
                if (incr_faddr < 17'd4096) begin
                    allow_request_o  = check_i;
                end
            end

            default:;
        endcase
    end
    
endmodule
