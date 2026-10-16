// Copyright 2018 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Original package author: Florian Zaruba <zarubaf@iis.ee.ethz.ch>
// Private test-only extraction/adaptation by Codex, 2026-10-12.
// Compile instead of tests/nonblock_actual_dma_pkg.sv and whole-core packages.
// The fu_op enum and sext32 below are verbatim production extracts. XLEN=64,
// TRANS_ID_BITS=3, BITMANIP=0, FPGA_EN=0 are the frozen research-model choices.
// ariane_pkg.sv SHA256: 9dcd36676645b702c404d4479b59fbb5380e9217ffc308677adcc53347757e3e
// riscv_pkg.sv SHA256: 712ad870ef2342123cfb61f8622f3dc8ff0637f73104408e4bc28d568217216b

package riscv;
    localparam int unsigned XLEN = 64;
    typedef logic [XLEN-1:0] xlen_t;
endpackage : riscv

package ariane_pkg;
    localparam int unsigned TRANS_ID_BITS = 3;
    localparam bit BITMANIP = 1'b0;
    localparam bit FPGA_EN = 1'b0;

    typedef enum logic [7:0] { // basic ALU op
                               ADD, SUB, ADDW, SUBW,
                               // logic operations
                               XORL, ORL, ANDL,
                               // shifts
                               SRA, SRL, SLL, SRLW, SLLW, SRAW,
                               // comparisons
                               LTS, LTU, GES, GEU, EQ, NE,
                               // jumps
                               JALR, BRANCH,
                               // set lower than operations
                               SLTS, SLTU,
                               // CSR functions
                               MRET, SRET, DRET, ECALL, WFI, FENCE, FENCE_I, SFENCE_VMA, HFENCE_VVMA, HFENCE_GVMA, CSR_WRITE, CSR_READ, CSR_SET, CSR_CLEAR,
                               // LSU functions
                               LD, SD, LW, LWU, SW, LH, LHU, SH, LB, SB, LBU,
                               // Hypervisor Virtual-Machine Load and Store Instructions
                               HLV_B, HLV_BU, HLV_H, HLV_HU, HLVX_HU, HLV_W, HLVX_WU, HSV_B, HSV_H, HSV_W, HLV_WU, HLV_D, HSV_D,
                               // Atomic Memory Operations
                               AMO_LRW, AMO_LRD, AMO_SCW, AMO_SCD,
                               AMO_SWAPW, AMO_ADDW, AMO_ANDW, AMO_ORW, AMO_XORW, AMO_MAXW, AMO_MAXWU, AMO_MINW, AMO_MINWU,
                               AMO_SWAPD, AMO_ADDD, AMO_ANDD, AMO_ORD, AMO_XORD, AMO_MAXD, AMO_MAXDU, AMO_MIND, AMO_MINDU,
                               // Multiplications
                               MUL, MULH, MULHU, MULHSU, MULW,
                               // Divisions
                               DIV, DIVU, DIVW, DIVUW, REM, REMU, REMW, REMUW,
                               // Floating-Point Load and Store Instructions
                               FLD, FLW, FLH, FLB, FSD, FSW, FSH, FSB,
                               // Floating-Point Computational Instructions
                               FADD, FSUB, FMUL, FDIV, FMIN_MAX, FSQRT, FMADD, FMSUB, FNMSUB, FNMADD,
                               // Floating-Point Conversion and Move Instructions
                               FCVT_F2I, FCVT_I2F, FCVT_F2F, FSGNJ, FMV_F2X, FMV_X2F,
                               // Floating-Point Compare Instructions
                               FCMP,
                               // Floating-Point Classify Instruction
                               FCLASS,
                               // Vectorial Floating-Point Instructions that don't directly map onto the scalar ones
                               VFMIN, VFMAX, VFSGNJ, VFSGNJN, VFSGNJX, VFEQ, VFNE, VFLT, VFGE, VFLE, VFGT, VFCPKAB_S, VFCPKCD_S, VFCPKAB_D, VFCPKCD_D,
                               // Offload Instructions to be directed into cv_x_if
                               OFFLOAD,
                               // Or-Combine and REV8
                               ORCB, REV8,
                               // Bitwise Rotation
                               ROL, ROLW, ROR, RORI, RORIW, RORW,
                               // Sign and Zero Extend
                               SEXTB, SEXTH, ZEXTH,
                               // Count population
                               CPOP, CPOPW,
                               // Count Leading/Training Zeros
                               CLZ, CLZW, CTZ, CTZW,
                               // Carry less multiplication Op's
                               CLMUL, CLMULH, CLMULR,
                               // Single bit instructions Op's
                               BCLR, BCLRI, BEXT, BEXTI, BINV, BINVI, BSET, BSETI,
                               // Integer minimum/maximum
                               MAX, MAXU, MIN, MINU,
                               // Shift with Add Unsigned Word and Unsigned Word Op's (Bitmanip)
                               SH1ADDUW, SH2ADDUW, SH3ADDUW, ADDUW, SLLIUW,
                               // Shift with Add (Bitmanip)
                               SH1ADD, SH2ADD, SH3ADD,
                               // Bitmanip Logical with negate op (Bitmanip)
                               ANDN, ORN, XNOR
                             } fu_op;

    function automatic riscv::xlen_t sext32 (logic [31:0] operand);
        return {{riscv::XLEN-32{operand[31]}}, operand[31:0]};
    endfunction
endpackage : ariane_pkg
