Below are complete proposed files for the bounded non-block observer work. I could not execute anything, so nothing here is a claim of a passing run; every item is a proposal for Codex to build/verify.

---

## 1. `cva6/corev_apu/fpga/src/dma_nonblock_observer.sv`

```systemverilog
// SPDX-License-Identifier: Solderpad-0.51
//
// dma_nonblock_observer
//
// Passive, non-blocking (open, close] window observer. The module only samples
// the DMA monitor bus; no output feeds back into the DMA or AXI data paths, and
// the register bank is read-only from software.
//
// 16-word read-only bank, selected by the 8-bit index port:
//   0  magic  0x49444D414E424C01
//   1  version 1
//   2  Mode
//   3  flags  {.., frozen, active, armed}
//   4  seq    monotonic, incremented once per ARM
//   5  cycles active cycles counted inside (open, close]
//   6  fast DID10 read
//   7  fast DID10 write
//   8  fast DID11 read
//   9  fast DID11 write
//  10  fast other
//  11  normal DID10
//  12  normal DID11
//  13  normal other
//  14  all accepted (== sum of words 6..13)
//  15  sticky error bits
//
// rdata is zero for any other index. rst is synchronous and active high.
//
module dma_nonblock_observer #(
    parameter int unsigned Mode = 2
) (
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
    input  logic [5:0]  ttype
);
    localparam logic [63:0] NB_MAGIC   = 64'h49444D414E424C01;
    localparam logic [63:0] NB_VERSION = 64'd1;

    // DID / transaction-type encodings used by the IOMMU.
    localparam logic [23:0] DID_V = 24'd10;
    localparam logic [23:0] DID_A = 24'd11;
    localparam logic [5:0]  TT_R  = 6'b000010; // UNTRANSLATED_R
    localparam logic [5:0]  TT_W  = 6'b000011; // UNTRANSLATED_W

    // Error bit positions inside word 15.
    localparam int E_OPEN_NO_ARM  = 0;
    localparam int E_CLOSE_NO_ACT = 1;
    localparam int E_OVERLAP      = 2;
    localparam int E_OVERFLOW     = 3;
    localparam int E_FAST_NO_ACPT = 4;
    localparam int E_FAST_MODE1   = 5;
    localparam int E_FAST_BAD_TT  = 6;

    typedef enum logic [1:0] {S_OFF, S_ARMED, S_ACTIVE, S_FROZEN} state_e;

    state_e      state_q, state_d;
    logic [63:0] seq_q,   seq_d;
    logic [63:0] cnt_q [16];
    logic [63:0] cnt_d [16];
    logic [63:0] err_q,   err_d;

    // Event classification selects (module scope so always_comb stays simple).
    logic        ff_r10, ff_w10, ff_r11, ff_w11, ff_other;
    logic        nn_10, nn_11, nn_other;
    logic [3:0]  cat;

    always_comb begin
        ff_r10   = fast && (did == DID_V) && (ttype == TT_R);
        ff_w10   = fast && (did == DID_V) && (ttype == TT_W);
        ff_r11   = fast && (did == DID_A) && (ttype == TT_R);
        ff_w11   = fast && (did == DID_A) && (ttype == TT_W);
        ff_other = fast && !(ff_r10 || ff_w10 || ff_r11 || ff_w11);
        nn_10    = !fast && (did == DID_V);
        nn_11    = !fast && (did == DID_A);
        nn_other = !fast && (did != DID_V) && (did != DID_A);

        cat = 4'd0;
        if (accept) begin
            if (ff_r10)        cat = 4'd6;
            else if (ff_w10)   cat = 4'd7;
            else if (ff_r11)   cat = 4'd8;
            else if (ff_w11)   cat = 4'd9;
            else if (fast)     cat = 4'd10;
            else if (nn_10)    cat = 4'd11;
            else if (nn_11)    cat = 4'd12;
            else               cat = 4'd13;
        end

        state_d = state_q;
        seq_d   = seq_q;
        err_d   = err_q;
        for (int i = 0; i < 16; i++) cnt_d[i] = cnt_q[i];

        // ---- commands ----
        // ARM clears the window and arms. Invalid while active: set error and
        // preserve the in-flight window.
        if (arm) begin
            if (state_q == S_ACTIVE) begin
                err_d[E_OVERLAP] = 1'b1;
            end else begin
                seq_d   = seq_q + 64'd1;
                state_d = S_ARMED;
                err_d   = '0;
                for (int i = 0; i < 16; i++) cnt_d[i] = '0;
            end
        end

        // OPEN is only legal from ARMED. No open-edge event is counted.
        if (open) begin
            if (state_d == S_ARMED)       state_d = S_ACTIVE;
            else if (state_d == S_ACTIVE) err_d[E_OVERLAP]      = 1'b1; // open while active
            else                          err_d[E_OPEN_NO_ARM]  = 1'b1; // open without arm
        end

        // CLOSE is only legal while active. Freezes the window.
        if (close) begin
            if (state_d == S_ACTIVE) state_d = S_FROZEN;
            else                     err_d[E_CLOSE_NO_ACT] = 1'b1;
        end

        // ---- window accumulation (open, close] ----
        // Counting keys off the registered state, so the open-edge cycle is
        // excluded and the close-edge cycle (state still S_ACTIVE) is included.
        if (state_q == S_ACTIVE) begin
            if (cnt_d[5] == {64{1'b1}}) err_d[E_OVERFLOW] = 1'b1;
            else                        cnt_d[5] = cnt_d[5] + 64'd1;

            if (accept) begin
                if (cnt_d[14] == {64{1'b1}}) err_d[E_OVERFLOW] = 1'b1;
                else                         cnt_d[14] = cnt_d[14] + 64'd1;

                if (cnt_d[cat] == {64{1'b1}}) err_d[E_OVERFLOW] = 1'b1;
                else                          cnt_d[cat] = cnt_d[cat] + 64'd1;
            end
        end

        // ---- fast protocol errors (sticky), only while a window is open/armed ----
        if (state_q == S_ARMED || state_q == S_ACTIVE) begin
            if (fast && !accept)                               err_d[E_FAST_NO_ACPT] = 1'b1;
            if ((Mode == 1) && fast)                           err_d[E_FAST_MODE1]   = 1'b1;
            if (fast && !((ttype == TT_R) || (ttype == TT_W))) err_d[E_FAST_BAD_TT]  = 1'b1;
        end
    end

    always_comb begin
        rdata = 64'd0;
        case (index)
            8'd0:  rdata = NB_MAGIC;
            8'd1:  rdata = NB_VERSION;
            8'd2:  rdata = 64'(Mode);
            8'd3:  rdata = {61'd0, (state_q == S_FROZEN),
                                     (state_q == S_ACTIVE),
                                     (state_q == S_ARMED)};
            8'd4:  rdata = seq_q;
            8'd5:  rdata = cnt_q[5];
            8'd6:  rdata = cnt_q[6];
            8'd7:  rdata = cnt_q[7];
            8'd8:  rdata = cnt_q[8];
            8'd9:  rdata = cnt_q[9];
            8'd10: rdata = cnt_q[10];
            8'd11: rdata = cnt_q[11];
            8'd12: rdata = cnt_q[12];
            8'd13: rdata = cnt_q[13];
            8'd14: rdata = cnt_q[14];
            8'd15: rdata = err_q;
            default: rdata = 64'd0;
        endcase
    end

    always_ff @(posedge clk) begin
        if (rst) begin
            state_q <= S_OFF;
            seq_q   <= 64'd0;
            err_q   <= 64'd0;
            for (int i = 0; i < 16; i++) cnt_q[i] <= 64'd0;
        end else begin
            state_q <= state_d;
            seq_q   <= seq_d;
            err_q   <= err_d;
            for (int i = 0; i < 16; i++) cnt_q[i] <= cnt_d[i];
        end
    end
endmodule
```

---

## 2. `tests/nonblock_observer_top.sv`

```systemverilog
// SPDX-License-Identifier: Solderpad-0.51
//
// Verilator top for the non-block observer. Mode 2 by default; +define+NB_MODE1
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
    input  logic [5:0]  ttype
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
        .ttype  (ttype)
    );
endmodule
```

---

## 3. `tests/nonblock_observer_test.cpp`

```cpp
// SPDX-License-Identifier: Solderpad-0.51
//
// Verilator 4.038 harness for dma_nonblock_observer. Simple manual clock, no SV
// timing. Mode-agnostic: reads word 2 to learn the configured Mode.
//
#include <cstdint>
#include <cstdio>
#include "Vnonblock_observer_top.h"
#include "verilated.h"

static Vnonblock_observer_top *dut;
static int g_fail = 0;

static void tick() {
    dut->clk = 1; dut->eval();   // posedge captures inputs
    dut->clk = 0; dut->eval();
}

static void idle() {
    dut->arm = 0; dut->open = 0; dut->close = 0;
    dut->accept = 0; dut->fast = 0;
    dut->did = 0; dut->ttype = 0;
}

static uint64_t rd(int idx) {
    dut->index = (uint8_t)idx;
    dut->eval();
    return (uint64_t)dut->rdata;
}

static void check(const char *what, uint64_t got, uint64_t want) {
    if (got != want) {
        std::fprintf(stderr,
                     "FAIL %s: got %llu (0x%llx) want %llu (0x%llx)\n",
                     what,
                     (unsigned long long)got, (unsigned long long)got,
                     (unsigned long long)want, (unsigned long long)want);
        g_fail++;
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vnonblock_observer_top;

    idle();
    dut->rst = 0; dut->index = 0; dut->clk = 0; dut->eval();

    // ---- reset ----
    dut->rst = 1; idle(); tick();
    dut->rst = 0; idle(); dut->eval();
    check("magic",       rd(0),  0x49444D414E424C01ULL);
    check("version",     rd(1),  1);
    uint64_t mode = rd(2);
    if (mode != 1 && mode != 2) {
        std::fprintf(stderr, "FAIL mode=%llu\n", (unsigned long long)mode);
        g_fail++;
    }
    check("rst flags",  rd(3),  0);
    check("rst seq",    rd(4),  0);
    check("rst cycles", rd(5),  0);
    check("rst err",    rd(15), 0);

    // ---- ARM ----
    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("armed", rd(3), 0x1);
    check("seq1",  rd(4), 1);

    // ---- pre-open accepted event must be excluded (still ARMED) ----
    idle(); dut->accept = 1; dut->fast = 0; dut->did = 10; dut->ttype = 0x2; tick();
    idle(); dut->eval();
    check("pre-open excluded", rd(14), 0);

    // ---- open edge with a coincident accept: excluded, opens the window ----
    idle();
    dut->open = 1; dut->accept = 1; dut->fast = 0; dut->did = 10; dut->ttype = 0x2;
    tick();
    idle(); dut->eval();
    check("active",            rd(3),  0x2);
    check("open-edge excl 14", rd(14), 0);
    check("open-edge excl 11", rd(11), 0);

    // ---- active window ----
    // A: fast DID10 write
    idle(); dut->accept = 1; dut->fast = 1; dut->did = 10; dut->ttype = 0x3; tick();
    // B: normal DID11
    idle(); dut->accept = 1; dut->fast = 0; dut->did = 11; dut->ttype = 0x3; tick();
    // C: fast other (DID 12)
    idle(); dut->accept = 1; dut->fast = 1; dut->did = 12; dut->ttype = 0x2; tick();
    // D: idle cycle (cycles++ only)
    idle(); tick();
    // E: close edge with fast DID11 read, included
    idle();
    dut->accept = 1; dut->fast = 1; dut->did = 11; dut->ttype = 0x2; dut->close = 1;
    tick();
    idle(); dut->eval();

    check("frozen", rd(3),  0x4);
    check("w6",     rd(6),  0);
    check("w7",     rd(7),  1);
    check("w8",     rd(8),  1);
    check("w9",     rd(9),  0);
    check("w10",    rd(10), 1);
    check("w11",    rd(11), 0);
    check("w12",    rd(12), 1);
    check("w13",    rd(13), 0);
    check("w14",    rd(14), 4);
    check("cycles", rd(5),  5);

    // ---- conservation, overflow-safe ----
    uint64_t sum = 0, tmp = 0;
    int ovf = 0;
    for (int i = 6; i <= 13; i++) {
        if (__builtin_add_overflow(sum, rd(i), &tmp)) { ovf = 1; break; }
        sum = tmp;
    }
    check("conserve overflow", ovf, 0);
    check("conserve sum",      sum, rd(14));

    // ---- frozen read stability ----
    uint64_t a[16], b[16];
    for (int i = 0; i < 16; i++) a[i] = rd(i);
    for (int i = 0; i < 16; i++) b[i] = rd(i);
    for (int i = 0; i < 16; i++) check("stable", a[i], b[i]);

    // ---- ignored unknown index reads 0 ----
    check("unknown idx -> 0", rd(200), 0);

    // ---- new ARM resets seq++ and clears the window ----
    idle(); dut->arm = 1; tick();
    idle(); dut->eval();
    check("seq2",           rd(4),  2);
    check("armed2",         rd(3),  0x1);
    check("cycles cleared", rd(5),  0);
    check("errors cleared", rd(15), 0);

    // ---- invalid close (not active) -> bit1 ----
    idle(); dut->close = 1; tick();
    idle(); dut->eval();
    check("close-no-active", (rd(15) >> 1) & 1, 1);

    // ---- invalid fast without accept -> bit4 (+ Mode1 bit5) ----
    idle(); dut->fast = 1; dut->accept = 0; dut->did = 10; dut->ttype = 0x2; tick();
    idle(); dut->eval();
    check("fast-no-accept", (rd(15) >> 4) & 1, 1);
    if (mode == 1)
        check("mode1 fast err", (rd(15) >> 5) & 1, 1);
    else
        check("mode2 no mode1", (rd(15) >> 5) & 1, 0);

    delete dut;
    if (g_fail) {
        std::fprintf(stderr, "%d check(s) failed\n", g_fail);
        return 1;
    }
    std::printf("PASS (mode=%llu)\n", (unsigned long long)mode);
    return 0;
}
```

---

## 4. `tests/test_nonblock_observer.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Solderpad-0.51
"""Build and run the non-block observer Verilator test.

Builds the Mode 2 model (default) and, unless --no-mode1 is given, a Mode 1
model compiled with +define+NB_MODE1, then compiles and runs
tests/nonblock_observer_test.cpp against each. Exit status is non-zero on the
first failing model.
"""
from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
SRC = os.path.join(ROOT, "cva6", "corev_apu", "fpga", "src")
DUT = os.path.join(SRC, "dma_nonblock_observer.sv")
TOP = os.path.join(HERE, "nonblock_observer_top.sv")
CPP = os.path.join(HERE, "nonblock_observer_test.cpp")
TOPNAME = "nonblock_observer_top"


def build_and_run(verilator: str, mode: int, build_root: str) -> int:
    obj = os.path.join(build_root, f"obj_mode{mode}")
    os.makedirs(obj, exist_ok=True)
    cmd = [
        verilator,
        "--cc", "--exe", "--build",
        "-O2", "-Wno-fatal",
        "--top-module", TOPNAME,
        "-Mdir", obj,
        TOP, DUT, CPP,
    ]
    if mode == 1:
        cmd.append("+define+NB_MODE1")
    print(f"[build] mode={mode}: {' '.join(cmd)}", flush=True)
    subprocess.run(cmd, check=True, cwd=ROOT)
    exe = os.path.join(obj, "V" + TOPNAME)
    print(f"[run] {exe}", flush=True)
    return subprocess.run([exe], cwd=obj).returncode


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--verilator", default=shutil.which("verilator") or "verilator")
    ap.add_argument("--no-mode1", action="store_true")
    ap.add_argument("--build-root",
                    default=os.path.join(ROOT, "tests", "build_nonblock"))
    args = ap.parse_args()

    modes = [2] if args.no_mode1 else [2, 1]
    rc = 0
    for mode in modes:
        try:
            run_rc = build_and_run(args.verilator, mode, args.build_root)
        except subprocess.CalledProcessError as exc:
            print(f"build failed for mode {mode}: {exc}", file=sys.stderr)
            return 2
        if run_rc != 0 and rc == 0:
            rc = run_rc
    return rc


if __name__ == "__main__":
    sys.exit(main())
```

---

## 5. `bench/idma_nonblock_read.c`

```c
// SPDX-License-Identifier: GPL-2.0
/*
 * idma_nonblock_read -- fixed-address reader for the passive non-block
 * observer window bank.
 *
 * Reads only /dev/mem at the iDMA0 register block (0x50000000). It selects the
 * 16-word non-block bank through the index register at 0x70 and reads the data
 * register at 0x78. The observer must already be frozen (status low bits == 4)
 * before this helper runs: it never arms, opens, closes, or configures the DMA
 * or the observer. No address or count is taken from the command line.
 */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define IDMA0_PHYS   0x50000000ULL
#define MAP_SIZE     0x1000ULL

#define REG_STATUS   0x60u   /* observer status (read only here) */
#define REG_INDEX    0x70u   /* observer bank index             */
#define REG_DATA     0x78u   /* observer bank data              */

#define NB_WORDS     16
#define NB_MAGIC     0x49444D414E424C01ULL
#define NB_VERSION   1ULL

/*
 * Register-block index base of the non-block window bank.
 * Task specification: index = 512 + 0xD00 + i  (== 0xF00 + i).
 */
#define NB_INDEX_BASE (512ULL + 0xD00ULL)

#if defined(__riscv)
#define NB_BARRIER() __asm__ __volatile__("fence iorw, iorw" ::: "memory")
#else
#define NB_BARRIER() __sync_synchronize()
#endif

static volatile uint64_t *g_regs;

static inline void nb_write(uint32_t off, uint64_t val)
{
    NB_BARRIER();
    g_regs[off / 8u] = val;
    NB_BARRIER();
}

static inline uint64_t nb_read(uint32_t off)
{
    uint64_t val;

    NB_BARRIER();
    val = g_regs[off / 8u];
    NB_BARRIER();
    return val;
}

static void emit_json(const uint64_t w[NB_WORDS])
{
    unsigned i;

    printf("{\"schema\":\"idma-nonblock-window-v1\",");
    printf("\"mode\":%" PRIu64 ",", w[2]);
    printf("\"windowseq\":%" PRIu64 ",", w[4]);
    printf("\"cycles\":%" PRIu64 ",", w[5]);
    printf("\"fastcounts\":[");
    for (i = 6; i <= 10; i++)
        printf("%s%" PRIu64, i == 6 ? "" : ",", w[i]);
    printf("],\"normalcounts\":[");
    for (i = 11; i <= 13; i++)
        printf("%s%" PRIu64, i == 11 ? "" : ",", w[i]);
    printf("],\"words\":[");
    for (i = 0; i < NB_WORDS; i++)
        printf("%s%" PRIu64, i == 0 ? "" : ",", w[i]);
    printf("]}\n");
}

int main(void)
{
    uint64_t words[NB_WORDS];
    uint64_t orig_index, status, flags, sum, tmp;
    void *map;
    int fd, rc = 0, ovf;
    unsigned i;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "open /dev/mem: %s\n", strerror(errno));
        return 2;
    }

    map = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
               (off_t)IDMA0_PHYS);
    if (map == MAP_FAILED) {
        fprintf(stderr, "mmap 0x%llx: %s\n",
                (unsigned long long)IDMA0_PHYS, strerror(errno));
        close(fd);
        return 2;
    }
    g_regs = (volatile uint64_t *)map;

    orig_index = nb_read(REG_INDEX);
    status = nb_read(REG_STATUS);

    if ((status & 0x7ULL) != 0x4ULL) {
        fprintf(stderr,
                "observer not frozen (status=0x%016" PRIx64
                "): require low bits == 4\n", status);
        rc = 3;
        goto out_restore;
    }

    for (i = 0; i < NB_WORDS; i++) {
        nb_write(REG_INDEX, NB_INDEX_BASE + (uint64_t)i);
        words[i] = nb_read(REG_DATA);
    }

    flags = words[3];
    if (words[0] != NB_MAGIC) {
        fprintf(stderr, "bad magic: 0x%016" PRIx64 "\n", words[0]);
        rc = 4;
    }
    if (words[1] != NB_VERSION) {
        fprintf(stderr, "bad version: %" PRIu64 "\n", words[1]);
        rc = 4;
    }
    if (words[2] != 1 && words[2] != 2) {
        fprintf(stderr, "bad mode: %" PRIu64 "\n", words[2]);
        rc = 4;
    }
    if ((flags & 0x7ULL) != 0x4ULL) {
        fprintf(stderr,
                "flags not frozen/!active/!armed: 0x%016" PRIx64 "\n", flags);
        rc = 4;
    }
    if (words[15] != 0) {
        fprintf(stderr,
                "observer error bits set: 0x%016" PRIx64 "\n", words[15]);
        rc = 4;
    }

    sum = 0;
    ovf = 0;
    for (i = 6; i <= 13; i++) {
        if (__builtin_add_overflow(sum, words[i], &tmp)) {
            ovf = 1;
            break;
        }
        sum = tmp;
    }
    if (ovf) {
        fprintf(stderr, "counter sum overflows 64 bits\n");
        rc = 4;
    } else if (sum != words[14]) {
        fprintf(stderr,
                "conservation failed: sum(6..13)=%" PRIu64
                " != words[14]=%" PRIu64 "\n", sum, words[14]);
        rc = 4;
    }

    if (rc == 0)
        emit_json(words);

out_restore:
    nb_write(REG_INDEX, orig_index);
    munmap(map, MAP_SIZE);
    close(fd);
    return rc;
}
```

---

## Acceptance plan (for Codex to execute)

1. **Build + run the SV test (both modes)**
   `python3 tests/test_nonblock_observer.py` — must print `PASS (mode=2)` and `PASS (mode=1)` and exit 0. Uses Verilator 4.038, `--cc --exe --build`, manual clock, no SV timing.
2. **Checks encoded in the harness** (independent of my running it):
   - magic, version, reset flags/seq/cycles/errors all 0.
   - ARM → `flags==0x1`, `seq==1`.
   - pre-open accept excluded (`word14==0`); open-edge accept excluded (`word14==0`, `word11==0`).
   - active events land in words 7, 12, 10; close-edge event lands in word 8; `cycles==5`; `flags==0x4`.
   - conservation `sum(6..13) == word14` with `__builtin_add_overflow`.
   - frozen read stability (two full-bank reads equal).
   - unknown index → 0.
   - re-ARM → `seq==2`, counters/errors cleared.
   - close-without-active → error bit 1; fast-without-accept → error bit 4; Mode 1 sets bit 5, Mode 2 does not.
3. **Cross-compile the reader for RISC-V** (`bench/idma_nonblock_read.c`) to prove `fence iorw, iorw` compiles; on the host it must still compile via the `__sync_synchronize()` fallback (syntax check only, not a board test).
4. **On-board (Codex execution, not here):** confirm `/dev/mem` exists and the kernel has `CONFIG_DEVMEM=y` with `strict_devmem` unset; observer already returned/drained and frozen; then run the static helper and check `schema=idma-nonblock-window-v1`, `mode∈{1,2}`, conservation holds, exit 0.

## Assumptions / uncertainties to confirm

- **`rst` polarity/timing:** implemented as **synchronous active-high** `rst` (task only said "rst"). Internal consistency with the supplied testbench is guaranteed; confirm against the intended integration.
- **`open`/`close` are one-cycle pulses** (level signals re-evaluated each clock). The harness pulses them; real hardware must pulse them too.
- **Error bit map** (my assignment, since the task listed conditions without bit numbers): bit0 open-without-arm, bit1 close-without-active, bit2 arm/open overlap, bit3 overflow, bit4 fast-without-accept, bit5 Mode 1 fast, bit6 fast bad ttype.
- **Index base for the board reader is ambiguous.** The task text "512+0xD00+i" is implemented literally as `NB_INDEX_BASE == 0xF00`; if the hardware integration places the 16-word bank at a different register-block index (e.g. `0xD00`), change the single `#define`. This is the highest-risk item — please verify against the actual observer bank decode.
- **`fast` error gating:** fast protocol errors are only latched while `ARMED` or `ACTIVE`, to keep the frozen bank stable until the next ARM. The harness still triggers them while armed, so bit4/bit5 are exercised.
- Conservation invariant is structural (each accepted event increments exactly one category word and word14), so `sum(6..13) == word14` always holds unless a counter saturates — in which case bit3 is set and the reader will already reject via its error check.
- The Python runner needs `verilator` and GNU `make` on PATH; it does not build for the board.