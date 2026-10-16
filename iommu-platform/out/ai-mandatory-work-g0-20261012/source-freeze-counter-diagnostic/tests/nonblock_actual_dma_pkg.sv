// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
//
// tests/nonblock_actual_dma_pkg.sv
//
// TEST-ONLY ariane_pkg stub for tests/test_nonblock_actual_dma.py.
//
// The production cva6/vendor/pulp-platform/common_cells fifo_v3.sv selects
// its FPGA RAM storage with
//
//     localparam bit UseFpgaRam = ariane_pkg::FPGA_EN || FORCE_FPGA_RAM;
//
// Pulling in the real CVA6 ariane_pkg would drag in the whole SoC
// configuration.  The production global value is FPGA_EN == 0, so this stub
// makes that value explicit: every generic queue (FORCE_FPGA_RAM == 0) stays
// on the non-RAM storage branch, while the real 512 x 8 bit DMA buffer FIFO
// (FORCE_FPGA_RAM == 1) still opts into AsyncDpRam.

package ariane_pkg;
    localparam bit FPGA_EN = 1'b0;
endpackage : ariane_pkg
