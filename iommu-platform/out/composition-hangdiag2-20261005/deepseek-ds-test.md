# Bounded rv_iommu_ds_if write‑path regression (two files)

Below are the two complete files, the acceptance procedure, and the assumptions. Nothing here has been executed: the RTL fix is Codex's job; these are proposals to be built and run by the parent agent.

---

## File 1 — `tests/iommu_ds_if_write_tb.sv`

```systemverilog
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
// -----------------------------------------------------------------------------
// Bounded, self-checking Verilator (--binary --timing) testbench for the
// production cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv.
//
// Scope: EXTERNAL behaviour of the DS write path only (AW / W / B for the five
// write-capable producers CQ, FQ, MRIF handler, MSI IG, PQ).  The DUT is the
// unmodified production wrapper (or a saved baseline copy supplied through the
// runner's --ds-source option).  No RTL behaviour is re-implemented here.
//
// Scenario 1 (+scenario=1, default): FQ only.
//   One FQ burst, AWID = 1, AWLEN = 3 => exactly 4 W beats with WLAST.  The FQ
//   AW is held asserted until accepted, then the 4 W beats are driven.  The DS
//   sink accepts AW and W independently and answers with B carrying the same
//   ID.  This is the minimal reproducer of the reported symptom:
//   "accepted FQ AW but never any FQ W data" (hang).
//
// Scenario 2 (+scenario=2): all five writers active at once.
//   Distinct sentinel data, burst lengths 1..4, independent AW/W sub-FSMs,
//   DS AW/W/B backpressure, several bursts in flight concurrently.
//
// Scoreboard policy
// -----------------
//   * Expected W bursts are derived from the OBSERVED DS output AW handshakes
//     (AWID, AWLEN and a burst index encoded in AWADDR).  The DUT's internal
//     selection arithmetic is never mirrored.
//   * DS WREADY is held low until at least one DS AW handshake has been
//     observed.  This is the "hold sink WREADY until it has AW" option called
//     out in the task; it keeps the scoreboard online while the DUT may still
//     legitimately present W before AW (AW and W live in independent spill
//     registers).  No W beat is ever accepted without an outstanding expected
//     AW entry.
//   * Each producer drives its W sub-FSM only after its own AW for the same
//     burst has been accepted (this is the "matching accepted internal AW
//     ownership" property, and it also keeps the stimulus inside the wrapper's
//     supported contract: the wrapper routes W by accepted-AW order).  A
//     producer-side guard asserts this property explicitly.
//
// Exit status: $fatal(1) on any mismatch or on watchdog timeout; plain $finish
// (exit 0) only when every burst has been AW+W+B acknowledged and the error
// counter is zero.
// -----------------------------------------------------------------------------

`timescale 1ns/1ps

`include "axi/typedef.svh"

module iommu_ds_if_write_tb;

  // ---------------------------------------------------------------------------
  // AXI types - one consistent vendor set (axi/typedef.svh + axi_pkg).
  // ---------------------------------------------------------------------------
  localparam int unsigned AddrWidth = 64;
  localparam int unsigned DataWidth = 64;
  localparam int unsigned IdWidth   = 4;
  localparam int unsigned StrbWidth = DataWidth / 8;
  localparam int unsigned UserWidth = 1;

  typedef logic [AddrWidth-1:0] addr_t;
  typedef logic [DataWidth-1:0] data_t;
  typedef logic [IdWidth-1:0]   id_t;
  typedef logic [StrbWidth-1:0] strb_t;
  typedef logic [UserWidth-1:0] user_t;

  `AXI_TYPEDEF_ALL(axi, addr_t, id_t, data_t, strb_t, user_t)

  localparam int unsigned NumProd   = 5;
  localparam int unsigned QDepth    = 32;
  localparam int unsigned MaxCycles = 20000;

  // Producer index == AWID used by that producer.
  localparam int unsigned P_CQ   = 0;
  localparam int unsigned P_FQ   = 1;
  localparam int unsigned P_MRIF = 2;
  localparam int unsigned P_MSI  = 3;
  localparam int unsigned P_PQ   = 4;

  // ---------------------------------------------------------------------------
  // Clock / reset
  // ---------------------------------------------------------------------------
  logic clk_i;
  logic rst_n;

  initial begin
    clk_i = 1'b0;
    forever #5 clk_i = ~clk_i;
  end

  initial begin
    rst_n = 1'b0;
    repeat (10) @(posedge clk_i);
    rst_n = 1'b1;
  end

  // ---------------------------------------------------------------------------
  // Scenario selection and derived burst plan
  // ---------------------------------------------------------------------------
  int scenario;
  int total_bursts;
  int total_beats;

  function automatic int nburst_of(input int p);
    if (scenario == 1) begin
      return (p == int'(P_FQ)) ? 1 : 0;
    end
    case (p)
      int'(P_CQ)  : return 2;
      int'(P_FQ)  : return 2;
      int'(P_MRIF): return 1;
      int'(P_MSI) : return 2;
      int'(P_PQ)  : return 2;
      default     : return 0;
    endcase
  endfunction

  function automatic int beats_of(input int p, input int j);
    if (scenario == 1) begin
      return 4;                       // FQ burst, AWLEN = 3
    end
    case (p)
      int'(P_CQ)  : return (j == 0) ? 1 : 3;
      int'(P_FQ)  : return (j == 0) ? 2 : 4;
      int'(P_MRIF): return 1;
      int'(P_MSI) : return (j == 0) ? 3 : 2;
      int'(P_PQ)  : return (j == 0) ? 4 : 1;
      default     : return 1;
    endcase
  endfunction

  // AWADDR encodes marker | id | burst index, so the scoreboard can rebuild the
  // expected W data from the OBSERVED output AW (not from stimulus bookkeeping).
  function automatic logic [63:0] make_addr(input int p, input int j);
    return {32'h0, 8'h5A, 4'h0, 4'(p), 8'(j), 8'hA0};
  endfunction

  function automatic logic [63:0] exp_data(input int p, input int j, input int beat);
    return 64'hA500_0000_0000_0000 | (64'(p) << 32) | (64'(j) << 16) | 64'(beat);
  endfunction

  initial begin
    scenario = 1;
    if (!$value$plusargs("scenario=%d", scenario)) begin
      scenario = 1;
    end
    total_bursts = 0;
    total_beats  = 0;
    for (int p = 0; p < NumProd; p++) begin
      for (int j = 0; j < nburst_of(p); j++) begin
        total_bursts = total_bursts + 1;
        total_beats  = total_beats + beats_of(p, j);
      end
    end
    $display("[%0t] iommu_ds_if_write_tb: scenario=%0d total_bursts=%0d total_beats=%0d",
             $time, scenario, total_bursts, total_beats);
  end

  // ---------------------------------------------------------------------------
  // Producer-side signals (index == AWID)
  // ---------------------------------------------------------------------------
  axi_aw_chan_t p_aw       [NumProd];
  logic         p_aw_valid [NumProd];
  axi_w_chan_t  p_w        [NumProd];
  logic         p_w_valid  [NumProd];

  axi_req_t     prod_req   [NumProd];
  axi_resp_t    prod_resp  [NumProd];

  // Read-only producers (DDTW/PDTW/PTW/MSIPTW) tied off.  They never drive
  // AW/W and never receive a B response.
  axi_req_t     ro_req     [4];
  axi_resp_t    ro_resp    [4];

  always_comb begin
    for (int i = 0; i < NumProd; i++) begin
      prod_req[i]          = '0;
      prod_req[i].aw       = p_aw[i];
      prod_req[i].aw_valid = p_aw_valid[i];
      prod_req[i].w        = p_w[i];
      prod_req[i].w_valid  = p_w_valid[i];
      prod_req[i].b_ready  = 1'b1;
    end
  end

  always_comb begin
    for (int i = 0; i < 4; i++) begin
      ro_req[i] = '0;
    end
  end

  // ---------------------------------------------------------------------------
  // DUT: the unmodified production wrapper (or --ds-source baseline copy).
  // ---------------------------------------------------------------------------
  axi_req_t  ds_req;
  axi_resp_t ds_resp;

  rv_iommu_ds_if #(
    .aw_chan_t  (axi_aw_chan_t),
    .w_chan_t   (axi_w_chan_t),
    .b_chan_t   (axi_b_chan_t),
    .ar_chan_t  (axi_ar_chan_t),
    .r_chan_t   (axi_r_chan_t),
    .axi_req_t  (axi_req_t),
    .axi_resp_t (axi_resp_t)
  ) i_dut (
    .clk_i               (clk_i),
    .rst_ni              (rst_n),

    .ds_resp_i           (ds_resp),
    .ds_req_o            (ds_req),

    .ddtw_req_i          (ro_req[0]),
    .ddtw_resp_o         (ro_resp[0]),

    .pdtw_req_i          (ro_req[1]),
    .pdtw_resp_o         (ro_resp[1]),

    .ptw_req_i           (ro_req[2]),
    .ptw_resp_o          (ro_resp[2]),

    .msiptw_req_i        (ro_req[3]),
    .msiptw_resp_o       (ro_resp[3]),

    .mrif_handler_req_i  (prod_req[P_MRIF]),
    .mrif_handler_resp_o (prod_resp[P_MRIF]),

    .cq_req_i            (prod_req[P_CQ]),
    .cq_resp_o           (prod_resp[P_CQ]),

    .fq_req_i            (prod_req[P_FQ]),
    .fq_resp_o           (prod_resp[P_FQ]),

    .pq_req_i            (prod_req[P_PQ]),
    .pq_resp_o           (prod_resp[P_PQ]),

    .msi_ig_req_i        (prod_req[P_MSI]),
    .msi_ig_resp_o       (prod_resp[P_MSI])
  );

  // ---------------------------------------------------------------------------
  // Per-producer AW / W drivers.
  //
  // AW sub-FSM: issues AW(n), holds VALID + payload until AWREADY, then AW(n+1).
  // W  sub-FSM: issues the beats of W(n), holds VALID + payload until WREADY,
  //             then W(n+1).  W(n) is only offered once the AW(n) of the same
  //             producer has been accepted; this keeps every offered W burst
  //             backed by an entry already present in the DUT's accepted-AW
  //             order (so the test never relies on W with no owner present,
  //             which the wrapper does not route).
  // ---------------------------------------------------------------------------
  for (genvar p = 0; p < NumProd; p++) begin : g_prod
    logic [7:0] aw_done;    // AWs accepted by this producer
    logic       aw_busy;
    logic [7:0] w_done;     // W bursts completed by this producer
    logic [7:0] w_beat;
    logic       w_busy;

    always_ff @(posedge clk_i) begin
      axi_aw_chan_t aw_tmp;
      axi_w_chan_t  w_tmp;

      if (!rst_n) begin
        p_aw_valid[p] <= 1'b0;
        p_w_valid[p]  <= 1'b0;
        aw_done <= 8'd0;
        aw_busy <= 1'b0;
        w_done  <= 8'd0;
        w_beat  <= 8'd0;
        w_busy  <= 1'b0;
      end else begin
        // ---------------- AW sub-FSM ----------------
        if (!aw_busy) begin
          if (int'(aw_done) < nburst_of(int'(p))) begin
            aw_tmp        = '0;
            aw_tmp.id     = id_t'(p);
            aw_tmp.addr   = make_addr(int'(p), int'(aw_done));
            aw_tmp.len    = 8'(beats_of(int'(p), int'(aw_done)) - 1);
            aw_tmp.size   = 3'd3;
            aw_tmp.burst  = axi_pkg::BURST_INCR;
            p_aw[p]       <= aw_tmp;
            p_aw_valid[p] <= 1'b1;
            aw_busy       <= 1'b1;
          end
        end else begin
          if (prod_resp[p].aw_ready) begin
            p_aw_valid[p] <= 1'b0;
            aw_busy       <= 1'b0;
            aw_done       <= aw_done + 8'd1;
          end
        end

        // ---------------- W sub-FSM ----------------
        if (!w_busy) begin
          if ((int'(w_done) < nburst_of(int'(p))) && (int'(aw_done) > int'(w_done))) begin
            w_tmp        = '0;
            w_tmp.data   = exp_data(int'(p), int'(w_done), 0);
            w_tmp.strb   = {StrbWidth{1'b1}};
            w_tmp.last   = (beats_of(int'(p), int'(w_done)) == 1);
            p_w[p]       <= w_tmp;
            p_w_valid[p] <= 1'b1;
            w_beat       <= 8'd0;
            w_busy       <= 1'b1;
          end
        end else begin
          if (prod_resp[p].w_ready) begin
            if (int'(w_beat) == (beats_of(int'(p), int'(w_done)) - 1)) begin
              p_w_valid[p] <= 1'b0;
              w_busy       <= 1'b0;
              w_done       <= w_done + 8'd1;
              w_beat       <= 8'd0;
            end else begin
              w_tmp      = '0;
              w_tmp.data = exp_data(int'(p), int'(w_done), int'(w_beat) + 1);
              w_tmp.strb = {StrbWidth{1'b1}};
              w_tmp.last = ((int'(w_beat) + 1) == (beats_of(int'(p), int'(w_done)) - 1));
              p_w[p]     <= w_tmp;
              w_beat     <= w_beat + 8'd1;
            end
          end
        end
      end
    end
  end

  // ---------------------------------------------------------------------------
  // DS sink + scoreboard.  One clocked process owns all sink state; DUT
  // outputs are sampled with pre-edge values, sink ready/b_valid are registered.
  // ---------------------------------------------------------------------------
  logic        aw_ready_q = 1'b0;
  logic        w_ready_q  = 1'b0;
  logic        b_valid_q  = 1'b0;
  axi_b_chan_t b_data_q   = '0;

  always_comb begin
    ds_resp          = '0;
    ds_resp.aw_ready = aw_ready_q;
    ds_resp.w_ready  = w_ready_q;
    ds_resp.b_valid  = b_valid_q;
    ds_resp.b        = b_data_q;
  end

  logic [IdWidth-1:0] awq_id    [QDepth];
  logic [7:0]         awq_bidx  [QDepth];
  logic [7:0]         awq_beats [QDepth];
  logic [7:0]         awq_beat  [QDepth];
  integer             awq_head, awq_tail;

  logic [IdWidth-1:0] bq_id     [QDepth];
  integer             bq_head, bq_tail;

  integer errors;
  integer aw_hs;
  integer w_hs;
  integer w_bursts;
  integer b_sink_hs;

  integer aw_acc   [NumProd];   // producer-side AW handshakes
  integer w_burst  [NumProd];   // producer-side completed W bursts
  integer w_beat_c [NumProd];   // beat within current producer W burst
  integer b_acc    [NumProd];   // producer-side B handshakes

  logic [7:0] lfsr;
  integer     done_cnt;

  always @(posedge clk_i) begin
    logic [63:0] exp;
    integer      occ;
    integer      g;
    integer      sum_b;

    if (!rst_n) begin
      aw_ready_q <= 1'b0;
      w_ready_q  <= 1'b0;
      b_valid_q  <= 1'b0;
      b_data_q   <= '0;
      awq_head = 0; awq_tail = 0;
      bq_head  = 0; bq_tail  = 0;
      errors = 0; aw_hs = 0; w_hs = 0; w_bursts = 0; b_sink_hs = 0;
      lfsr = 8'h5A;
      done_cnt = 0;
      for (g = 0; g < NumProd; g = g + 1) begin
        aw_acc[g] = 0; w_burst[g] = 0; w_beat_c[g] = 0; b_acc[g] = 0;
      end
    end else begin
      lfsr = {lfsr[6:0], lfsr[7] ^ lfsr[5] ^ lfsr[4] ^ lfsr[3]};

      // -------- producer-side ownership / B-routing guard --------
      for (g = 0; g < NumProd; g = g + 1) begin
        if (p_aw_valid[g] && prod_resp[g].aw_ready) begin
          aw_acc[g] = aw_acc[g] + 1;
        end
        if (p_w_valid[g] && prod_resp[g].w_ready) begin
          if (w_beat_c[g] == 0) begin
            if (aw_acc[g] <= w_burst[g]) begin
              errors = errors + 1;
              $display("[%0t] ERROR: producer %0d W burst %0d handshake with no matching accepted AW (aw_acc=%0d)",
                       $time, g, w_burst[g], aw_acc[g]);
            end
          end
          if (w_beat_c[g] == (beats_of(g, (w_burst[g] < nburst_of(g)) ? w_burst[g] : 0) - 1)) begin
            w_burst[g] = w_burst[g] + 1;
            w_beat_c[g] = 0;
          end else begin
            w_beat_c[g] = w_beat_c[g] + 1;
          end
        end
        if (prod_resp[g].b_valid) begin
          if (prod_resp[g].b.id !== id_t'(g)) begin
            errors = errors + 1;
            $display("[%0t] ERROR: producer %0d received B with id %0d (mis-routed B)",
                     $time, g, prod_resp[g].b.id);
          end
          b_acc[g] = b_acc[g] + 1;
        end
      end

      // -------- DS output AW handshake --------
      if (aw_ready_q && ds_req.aw_valid) begin
        if (ds_req.aw.id[IdWidth-1:0] !== ds_req.aw.addr[19:16]) begin
          errors = errors + 1;
          $display("[%0t] ERROR: DS AW id=%0d but AWADDR encodes id=%0d",
                   $time, ds_req.aw.id, ds_req.aw.addr[19:16]);
        end
        if ((ds_req.aw.addr[31:24] !== 8'h5A) || (ds_req.aw.addr[7:0] !== 8'hA0)) begin
          errors = errors + 1;
          $display("[%0t] ERROR: DS AW marker mismatch AWADDR=%h", $time, ds_req.aw.addr);
        end
        if (ds_req.aw.len !== 8'(beats_of(ds_req.aw.id[IdWidth-1:0], ds_req.aw.addr[15:8]) - 1)) begin
          errors = errors + 1;
          $display("[%0t] ERROR: DS AW len=%0d unexpected for id=%0d burst=%0d",
                   $time, ds_req.aw.len, ds_req.aw.id, ds_req.aw.addr[15:8]);
        end
        awq_id[awq_tail]    = ds_req.aw.id[IdWidth-1:0];
        awq_bidx[awq_tail]  = ds_req.aw.addr[15:8];
        awq_beats[awq_tail] = ds_req.aw.len + 8'd1;
        awq_beat[awq_tail]  = 8'd0;
        awq_tail = (awq_tail + 1) % QDepth;
        aw_hs = aw_hs + 1;
        $display("[%0t] DS AW accepted: id=%0d burst=%0d len=%0d addr=%h",
                 $time, ds_req.aw.id, ds_req.aw.addr[15:8], ds_req.aw.len, ds_req.aw.addr);
      end

      // -------- DS output W handshake --------
      if (w_ready_q && ds_req.w_valid) begin
        if (awq_head == awq_tail) begin
          errors = errors + 1;
          $display("[%0t] ERROR: DS W beat accepted with no outstanding AW expected", $time);
        end else begin
          exp = exp_data(awq_id[awq_head], awq_bidx[awq_head], awq_beat[awq_head]);
          if (ds_req.w.data !== exp) begin
            errors = errors + 1;
            $display("[%0t] ERROR: DS W data got=%h exp=%h (id=%0d burst=%0d beat=%0d of %0d)",
                     $time, ds_req.w.data, exp, awq_id[awq_head], awq_bidx[awq_head],
                     awq_beat[awq_head], awq_beats[awq_head]);
          end
          if (ds_req.w.last !== (awq_beat[awq_head] == (awq_beats[awq_head] - 8'd1))) begin
            errors = errors + 1;
            $display("[%0t] ERROR: DS W last=%0d unexpected (id=%0d burst=%0d beat=%0d of %0d)",
                     $time, ds_req.w.last, awq_id[awq_head], awq_bidx[awq_head],
                     awq_beat[awq_head], awq_beats[awq_head]);
          end
          if (awq_beat[awq_head] == (awq_beats[awq_head] - 8'd1)) begin
            bq_id[bq_tail] = awq_id[awq_head];
            bq_tail = (bq_tail + 1) % QDepth;
            awq_head = (awq_head + 1) % QDepth;
            w_bursts = w_bursts + 1;
          end else begin
            awq_beat[awq_head] = awq_beat[awq_head] + 8'd1;
          end
          w_hs = w_hs + 1;
        end
      end

      // -------- DS B handshake --------
      if (b_valid_q && ds_req.b_ready) begin
        bq_head = (bq_head + 1) % QDepth;
        b_sink_hs = b_sink_hs + 1;
      end

      // -------- sink output registers --------
      occ = (awq_tail + QDepth - awq_head) % QDepth;

      // AW backpressure.  Keep the number of accepted-but-uncompleted AWs low
      // enough that the DUT's internal 8-entry accepted-AW order FIFO can never
      // overflow (output occupancy <= 4; two further AWs may still sit in the
      // DUT AW spill register).
      aw_ready_q <= (occ < 4) && lfsr[2];

      // WREADY only once at least one DS AW handshake has been observed
      // ("hold sink WREADY until it has AW").
      w_ready_q  <= (occ > 0) && lfsr[3];

      if (bq_head != bq_tail) begin
        b_valid_q     <= 1'b1;
        b_data_q.id   <= bq_id[bq_head];
        b_data_q.resp <= axi_pkg::RESP_OKAY;
        b_data_q.user <= '0;
      end else begin
        b_valid_q     <= 1'b0;
      end

      // -------- completion --------
      sum_b = 0;
      for (g = 0; g < NumProd; g = g + 1) begin
        sum_b = sum_b + b_acc[g];
      end

      if ((aw_hs >= total_bursts) && (w_bursts >= total_bursts) &&
          (b_sink_hs >= total_bursts) && (sum_b >= total_bursts)) begin
        done_cnt = done_cnt + 1;
      end else begin
        done_cnt = 0;
      end

      if (done_cnt == 4) begin
        if (sum_b != total_bursts) begin
          errors = errors + 1;
          $display("[%0t] ERROR: producer-side B count=%0d != total_bursts=%0d",
                   $time, sum_b, total_bursts);
        end
        if (errors == 0) begin
          $display("[%0t] SCENARIO %0d PASS: bursts=%0d aw_hs=%0d w_beats=%0d w_bursts=%0d b_sink=%0d b_prod=%0d",
                   $time, scenario, total_bursts, aw_hs, w_hs, w_bursts, b_sink_hs, sum_b);
          $finish;
        end else begin
          $display("[%0t] SCENARIO %0d FAIL: errors=%0d aw_hs=%0d w_beats=%0d w_bursts=%0d b_sink=%0d total_bursts=%0d",
                   $time, scenario, errors, aw_hs, w_hs, w_bursts, b_sink_hs, total_bursts);
          $fatal(1);
        end
      end
    end
  end

  // ---------------------------------------------------------------------------
  // Watchdog - bounded runtime, nonzero exit on timeout.
  // ---------------------------------------------------------------------------
  initial begin
    repeat (MaxCycles) @(posedge clk_i);
    $display("[%0t] SCENARIO %0d FAIL (watchdog): aw_hs=%0d w_beats=%0d w_bursts=%0d b_sink=%0d errors=%0d total_bursts=%0d",
             $time, scenario, aw_hs, w_hs, w_bursts, b_sink_hs, errors, total_bursts);
    $fatal(1);
  end

endmodule
```

---

## File 2 — `tests/test_iommu_ds_if_write.py`

```python
#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1
"""Verilate and run the bounded rv_iommu_ds_if write-path regression.

Builds the UNMODIFIED production
cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv together with
tests/iommu_ds_if_write_tb.sv using Verilator's --binary --timing flow (no
C++ boilerplate).

Vendor sources: exactly ONE tree, the one already used by
tests/test_nonblock_banked.py (the IOMMU vendor axi/ + common_cells/).  Only
the modules actually needed by rv_iommu_ds_if are compiled; the production
fifo_v3 / ariane_pkg dependency chain used by the iDMA test is NOT pulled in.

Usage
-----
    # production RTL (after fix)
    python3 tests/test_iommu_ds_if_write.py

    # saved "before" baseline copy, isolated output directory
    python3 tests/test_iommu_ds_if_write.py \\
        --ds-source /path/to/before.sv \\
        --out out/iommu-ds-if-write-before

Exit status is nonzero if the build fails, if any scenario reports a mismatch,
or if a scenario times out.
"""

import argparse
import os
import subprocess
import sys
from pathlib import Path

ROOT   = Path(__file__).resolve().parents[1]
IOMMU  = ROOT / "cva6" / "corev_apu" / "riscv-iommu"
VENDOR = IOMMU / "vendor"

DEFAULT_DS  = IOMMU / "rtl" / "ext_interfaces" / "rv_iommu_ds_if.sv"
DEFAULT_OUT = ROOT / "out" / "iommu-ds-if-write"
TB          = ROOT / "tests" / "iommu_ds_if_write_tb.sv"
TOP         = "iommu_ds_if_write_tb"

COMMON = VENDOR / "common_cells" / "src"
AXI    = VENDOR / "axi" / "src"

# Exactly the modules needed by rv_iommu_ds_if.sv and its dependencies.
SOURCES = [
    COMMON / "cf_math_pkg.sv",
    AXI    / "axi_pkg.sv",
    COMMON / "lzc.sv",
    COMMON / "fifo_v3.sv",
    COMMON / "spill_register_flushable.sv",
    COMMON / "spill_register.sv",
    COMMON / "rr_arb_tree.sv",
    COMMON / "stream_arbiter_flushable.sv",
    COMMON / "stream_arbiter.sv",
    COMMON / "stream_mux.sv",
    COMMON / "stream_demux.sv",
]

INCDIRS = [
    VENDOR / "axi" / "include",
    VENDOR / "common_cells" / "include",
]

# Disable common_cells / AXI assertions (same idea as test_nonblock_banked.py).
DEFINES = ["+define+ASSERTS_OFF", "+define+COMMON_CELLS_ASSERTS_OFF"]


def require(path: Path) -> Path:
    if not path.exists():
        sys.exit("required file missing: {}".format(path))
    return path


def tail(path: Path, lines: int = 240) -> str:
    try:
        text = path.read_text(errors="replace").splitlines()
    except OSError:
        return ""
    return "\n".join(text[-lines:]) + "\n"


def find_exe(obj_dir: Path, top: str):
    for base in (obj_dir, Path.cwd()):
        cand = base / ("V" + top)
        if cand.is_file() and os.access(str(cand), os.X_OK):
            return cand
    for p in sorted(obj_dir.rglob("V" + top)):
        if p.is_file() and os.access(str(p), os.X_OK):
            return p
    return None


def main() -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("--ds-source", type=Path, default=DEFAULT_DS,
                    help="rv_iommu_ds_if.sv under test (default: production RTL); "
                         "pass a saved before.sv to reproduce the baseline")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT,
                    help="output directory (obj dir + logs); use distinct "
                         "directories to isolate old vs new compiles")
    ap.add_argument("--scenarios", default="1,2",
                    help="comma-separated scenario list (default 1,2)")
    ap.add_argument("--build-only", action="store_true",
                    help="compile the model but do not run any scenario")
    opts = ap.parse_args()

    ds_source = require(opts.ds_source.resolve())
    tb        = require(TB)
    out_dir   = opts.out.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    obj_dir   = out_dir / "obj"
    obj_dir.mkdir(parents=True, exist_ok=True)

    scenarios = [s.strip() for s in opts.scenarios.split(",") if s.strip()]

    sources = [require(p) for p in SOURCES] + [ds_source, tb]

    build_log = out_dir / "build.log"
    args = [
        "verilator",
        "--binary",
        "--timing",
        "-Wno-fatal",
        "--top-module", TOP,
        "--Mdir", str(obj_dir),
    ] + DEFINES
    for inc in INCDIRS:
        args += ["-I{}".format(inc)]
    args += [str(p) for p in sources]

    print("test_iommu_ds_if_write: ds_source={}".format(ds_source))
    print("test_iommu_ds_if_write: out={}".format(out_dir))
    print("test_iommu_ds_if_write: build cmd: {} ...".format(" ".join(args[:8])))

    with build_log.open("w") as log:
        build = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
    if build.returncode != 0:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator build FAILED (rc={})\n".format(build.returncode))
        return 1

    exe = find_exe(obj_dir, TOP)
    if exe is None:
        sys.stderr.write(tail(build_log))
        sys.stderr.write("verilator did not produce an executable for {}\n".format(TOP))
        return 1
    print("test_iommu_ds_if_write: exe={}".format(exe))

    if opts.build_only:
        return 0

    results = []
    for sc in scenarios:
        run_log  = out_dir / "run-scenario-{}.log".format(sc)
        run_args = [str(exe), "+scenario={}".format(sc)]
        print("test_iommu_ds_if_write: running {}".format(" ".join(run_args)))
        with run_log.open("w") as log:
            run = subprocess.run(run_args, stdout=log, stderr=subprocess.STDOUT)
        sys.stdout.write(tail(run_log, 60))
        results.append((sc, run.returncode))
        if run.returncode != 0:
            print("test_iommu_ds_if_write: scenario {} FAILED (rc={})".format(sc, run.returncode))

    rc = 0
    print("test_iommu_ds_if_write: summary")
    for sc, r in results:
        status = "PASS" if r == 0 else "FAIL(rc={})".format(r)
        print("  scenario {} -> {}".format(sc, status))
        if r != 0:
            rc = 1
    return rc


if __name__ == "__main__":
    sys.exit(main())
```

---

## Acceptance guidance

The runner must be built/run by the parent agent; the results below are the *expected* observations, not executed facts.

### 1. Baseline (before) — must fail specifically on scenario 1

```bash
python3 tests/test_iommu_ds_if_write.py \
    --ds-source /path/to/before.sv \
    --out out/iommu-ds-if-write-before
```

Expected:
- scenario 1 log shows `DS AW accepted: id=1 burst=0 len=3 addr=...` (the FQ AW is accepted and the ACK/backpressure path works), then **no** `DS W data` line;
- the watchdog fires and prints counters with `w_beats=0 w_bursts=0`;
- the process exits nonzero, so the runner prints `scenario 1 -> FAIL(rc=1)` and returns 1.

This is the exact symptom "accepted FQ AW but no FQ W data".

### 2. After fix — must pass both scenarios

```bash
python3 tests/test_iommu_ds_if_write.py --out out/iommu-ds-if-write
```

Expected:
- scenario 1: `SCENARIO 1 PASS: bursts=1 aw_hs=1 w_beats=4 w_bursts=1 b_sink=1 b_prod=1`;
- scenario 2: `SCENARIO 2 PASS: bursts=9 aw_hs=9 w_beats=23 w_bursts=9 b_sink=9 b_prod=9`;
- runner returns 0.

### 3. Optional isolation / inspection

```bash
python3 tests/test_iommu_ds_if_write.py --build-only          # compile only
python3 tests/test_iommu_ds_if_write.py --scenarios 1         # FQ-only only
cat out/iommu-ds-if-write/build.log
cat out/iommu-ds-if-write/run-scenario-2.log
```

### What is asserted (external behaviour only)

- Scenario 1: FQ AW (AWID 1, AWLEN 3) accepted; exactly 4 W beats with sentinel data and `WLAST` only on the 4th beat; B returned with ID 1 to FQ; completion.
- Scenario 2: for every DS output AW handshake, the corresponding DS W burst carries the matching sentinel data for ID / burst index / beat, `WLAST` exactly on the last beat, and B is routed back to the producer matching the AWID. Producer-side guard verifies no producer W burst handshake happens without a matching accepted AW. Counts must satisfy `aw_hs == w_bursts == b_sink == b_prod == total_bursts`.
- No assertion inspects internal enums/registers; the scoreboard only uses the observed DS AW/W/B handshakes.

---

## Assumptions, scope and missing dependencies

1. **No RTL is written or modified.** The TB instantiates the production `rv_iommu_ds_if` (or a `--ds-source` copy). The known CQ=0/FQ=1/MRIF=2/MSI=3/PQ=4 AWID vs. `stream_mux` index-0=PQ ordering defect is *not* mirrored; the scoreboard is derived from observed output AW fields.
2. **One vendor tree only.** `axi_pkg`, `axi/typedef.svh`, `cf_math_pkg`, `lzc`, `fifo_v3`, `spill_register(_flushable)`, `rr_arb_tree`, `stream_arbiter(_flushable)`, `stream_mux`, `stream_demux` are taken from `cva6/corev_apu/riscv-iommu/vendor/{axi,common_cells}` — the same tree `tests/test_nonblock_banked.py` uses. The production FIFO / `ariane_pkg` chain is deliberately not pulled in.
3. **`--binary --timing`** is assumed available on the installed Verilator (it is the flow requested for this test). Assertion macros are disabled via `ASSERTS_OFF` **and** `COMMON_CELLS_ASSERTS_OFF` because `common_cells/assertions.svh` was not supplied here; if the guard name differs, the build log will show it and the define list is the single knob to extend.
4. **Outstanding-AW bound.** The sink never lets more than 4 accepted-but-uncompleted AWs accumulate (`occ < 4`), keeping the DUT's internal 8-entry accepted-AW order FIFO from overflowing. The ">8 AW pending credit saturation" corner from the task is therefore *not* exercised — it is only safe to add once the FIFO-overflow behaviour is separately confirmed.
5. **Early-W interpretation.** Producers present W independently of AW, but each producer offers W for burst *n* only after its own AW for burst *n* has been accepted. This keeps the stimulus inside the wrapper's supported contract (the wrapper routes W from an accepted-AW order FIFO, and its W mux default-selected input when the order is empty is not a defined owner for a fresh W burst). The output-side "W can arrive before AW" condition is still exercised: AW `aw_ready` backpressure in the sink delays the output AW handshake while W can already be presented, and the sink holds `WREADY` low until an AW handshake has been observed, which is the mitigation explicitly allowed by the task. If Codex wants genuine pre-AW producer W, that belongs with a decision on the empty-order behaviour and is flagged for Codex leadership.
6. **Sink B handling.** B is returned per completed burst using the ID of the just-completed AW entry, allowed to be buffered in a small B queue. Early B capture across multiple in-flight bursts is handled by that queue.
7. **Runtime bound.** Watchdog at 20000 cycles; anything that does not complete prints counters and exits nonzero. Scenario 1 (~tens of cycles), scenario 2 (~hundreds of cycles).
8. **Not executed / not verified here.** Builds and runs were not performed in this work item; the commands and expected outputs above are proposals for the parent agent to execute. The most likely first-order build risks to confirm are (a) exact `common_cells/assertions.svh` guard name, (b) Verilator `--binary` executable location (handled by `find_exe`), and (c) Verilator handling of the `unique case` duplicate label in the DUT (covered by `-Wno-fatal`, matching the existing `test_nonblock_actual_dma.py --system` build that already compiles this same file).