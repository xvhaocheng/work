# Bounded CPU write / AMO test extension — proposed patch

## Navigation summary

**Files touched (3, exactly as scoped):**

| File | Nature of change |
|---|---|
| `tests/nonblock_actual_dma_system_top.sv` | Adds `CpuWrites` parameter, a bounded synthetic CPU write/AMO engine on slave port 0 (ID1 = AMO, ID2 = plain write), new observation taps, and folds the new channels into `dbg_cpu_quiet`. No change to `DdrAtomics` wiring, `rv_iommu_ds_if`, `axi_delayer`, `axi_xbar`, or the inner harness. |
| `tests/nonblock_actual_dma_test.cpp` | Adds `+cpu-writes` handling, an AMO old‑value continuity scoreboard, a write/AMO progress watchdog, and DDR final‑value / plain‑write data‑strb checks. Everything new is under `#ifdef ACTUAL_DMA_SYSTEM`; the non‑system build is untouched. |
| `tests/test_nonblock_actual_dma.py` | Adds `--cpu-writes` (requires `--system-atomics`), an independent object dir `out/nonblock-actual-dma/obj-system-atomics-cpu-writes`, `-GCpuWrites=1`, `+cpu-writes=1`, and a separate build log. |

**Key design decisions (bounded, test‑only):**
1. **One write-capable transaction outstanding at a time.** A single engine alternates plain write / AMO, so no W-interleaving across transactions is ever generated.
2. **AW and W are driven from independent valids that are held until accepted**; the per-transaction skew rotates `W-first → simultaneous → AW-first` (`cw_rot_q`). The W beats of one burst never reorder.
3. **The AMO R is demultiplexed by ID** and is never allowed to complete/clear the plain read lane.
4. **Bounded transaction count** (`CPU_WR_TOTAL = 64` write+AMO transactions per scenario) so the scoreboard stays bounded.
5. Addresses `0x8100_0000` (write) / `0x8100_1000` (AMO) are disjoint from V (`0x8000_0000`), A (`0x9000_0000+`), DDT (`0x1_0000`) and every page table; the existing end-of-scenario byte checks still prove V/A/tables were not overwritten.

**No claim is made that this is a real CPU or a board reproduction** — the engine is synthetic AXI stimulus only.

---

## Interface assumptions / dependencies

- `axi_pkg` is imported (`import axi_pkg::*;`) so `axi_pkg::ATOP_ATOMICLOAD` (2'b10), `axi_pkg::ATOP_LITTLE_END` (1'b0), `axi_pkg::ATOP_ADD` (3'b000) build the AMO ATOP `6'b100000`.
- `slv_aw_chan_t` (from `AXI_TYPEDEF_AW_CHAN_T`) includes an `atop` field — already used by the `axi_riscv_atomics` instance at line 552.
- The crossbar appends the slave index as the **high 2 bits** of the 6-bit master ID (already relied on by `n_slv_ar[(id>>4)&3]`), so the ID seen back at slave port 0 is the original 4-bit ID (`0` read, `1` AMO, `2` write).
- The atomics adapter performs the AMO as an internal read-modify-write on the DDR master port; the C++ DDR model therefore observes an internal AR and an internal AW/W for each AMO.
- **Risk (flagged, not resolved here):** rotating in a true W-before-AW skew stresses whether the pulp `axi_xbar`/`axi_mux` accept W before AW is accepted. If the fabric requires AW first, the W-first transaction stalls; the new C++ write/AMO watchdog turns that into a bounded FAIL rather than a hang. If that turns out to be the case, the fix is a one-liner in the skew rotation (drop skew `2'd0`).

---

## Patch

```diff
--- a/tests/nonblock_actual_dma_system_top.sv
+++ b/tests/nonblock_actual_dma_system_top.sv
@@ -36,6 +36,10 @@
 // The synthetic CPU master is NOT a real RISC-V core and NOT a cache model.
-// It only injects one bounded read stream to reproduce CPU<->DMA<->walker
-// contention through the real crossbar.  It starts when DMA0's batch becomes
-// active and stops offering NEW reads when the batch (active|gap) ends, then
-// drains any outstanding beat.  Its AR is never withdrawn while stalled and its
-// RREADY is a fixed 16-cycle-ready / 4-cycle-stall pattern.
+// It injects one bounded read stream (ID0) plus, only with
+// --system-atomics --cpu-writes, a bounded plain-write stream (ID2) and a
+// bounded AMO (ATOMICLOAD+ADD, ID1) stream, to reproduce CPU<->DMA<->walker
+// contention through the real crossbar.  It starts when DMA0's batch becomes
+// active and stops offering NEW reads/writes when the batch (active|gap) ends,
+// then drains any outstanding beat/transaction.  Its AR/AW/W valids are never
+// withdrawn while stalled and its RREADY is a fixed 16-cycle-ready /
+// 4-cycle-stall pattern.
@@ -43,8 +47,9 @@
 // DELIBERATE LIMITATIONS (must be reported with any result):
 //   * no real CPU / caches / axi_adapter / dwidth converters;
 //   * only 2 of the 12 board master ports; the peripheral fabric, PLIC/CLINT/
 //     ROM/UART/SPI/GPIO/Ethernet/timer are NOT instantiated;
-//   * optional real DDR atomics enabled by --system-atomics; no real CPU atomics yet;
+//   * optional real DDR atomics enabled by --system-atomics; the optional
+//     --cpu-writes AMO traffic is synthetic test stimulus, not a real CPU;
 //   * no MIG / DDR controller / clock converter / 512-to-64 width converter:
 //     DDR is a behavioural model in the C++ testbench;
 //   * the cfg_* AXI slave selector still reaches the two dma_core_wrap slaves
@@ -64,9 +69,12 @@
 #(
     parameter bit          NonblockEnable       = 1,
     parameter bit          DdrAtomics           = 0,
+    // Test-only: add synthetic CPU plain-write / AMO traffic.  Only effective
+    // together with DdrAtomics (i.e. --system-atomics --cpu-writes).
+    parameter bit          CpuWrites            = 0,
     parameter int unsigned NumOutstandingTrans = 8,
     parameter int unsigned WFifoDepth          = 32,
     parameter bit          PolicyEnable        = 0,
     // Test-only: cycles between the synthetic CPU read offers.
     parameter int unsigned CpuIssueInterval    = 32,
@@ -179,6 +187,18 @@
     // ---------------- synthetic CPU lifecycle (read-only) -------------------
     output logic [31:0] dbg_cpu_issued,
     output logic [31:0] dbg_cpu_completed,
     output logic        dbg_cpu_quiet,
-    output logic        dbg_cpu_active
+    output logic        dbg_cpu_active,
+
+    // ---------------- synthetic CPU write / AMO lifecycle (optional) --------
+    output logic [31:0] dbg_cpu_wr_issued,
+    output logic [31:0] dbg_cpu_wr_completed,
+    output logic [31:0] dbg_cpu_amo_issued,
+    output logic [31:0] dbg_cpu_amo_completed,
+    output logic [31:0] dbg_cpu_rd_err,
+    output logic [31:0] dbg_cpu_wr_err,
+    output logic [31:0] dbg_cpu_amo_err,
+    output logic        dbg_cpu_wr_quiet,
+    output logic        dbg_cpu_amo_r_fire,
+    output logic [63:0] dbg_cpu_amo_r_data
 );
@@ -394,80 +394,312 @@
-    // ========================================================================
-    // Slave port 0 : synthetic read-only CPU master (TEST-ONLY)
-    // ========================================================================
-    logic        cpu_run;
-    assign cpu_run = dbg_dma_batch_active[0] | dbg_dma_batch_gap[0];
-
-    localparam logic [5:0] CPU_INTERVAL = 6'(CpuIssueInterval);
-
-    logic        cpu_ar_valid_q;
-    logic [3:0]  cpu_ar_id_q;
-    logic [63:0] cpu_ar_addr_q;
-    logic [7:0]  cpu_ar_len_q;
-    logic [2:0]  cpu_ar_size_q;
-    logic [1:0]  cpu_ar_burst_q;
-    logic        cpu_outstanding_q;
-    logic [31:0] cpu_issued_q;
-    logic [31:0] cpu_completed_q;
-    logic [5:0]  cpu_wait_q;
-    logic [4:0]  cpu_rphase_q;
-
-    slv_req_t    cpu_req;
-
-    always_ff @(posedge clk_i or negedge rst_ni) begin : cpu_fsm
-        if (!rst_ni) begin
-            cpu_ar_valid_q    <= 1'b0;
-            cpu_ar_id_q       <= '0;
-            cpu_ar_addr_q     <= '0;
-            cpu_ar_len_q      <= '0;
-            cpu_ar_size_q     <= '0;
-            cpu_ar_burst_q    <= '0;
-            cpu_outstanding_q <= 1'b0;
-            cpu_issued_q      <= '0;
-            cpu_completed_q   <= '0;
-            cpu_wait_q        <= CPU_INTERVAL;
-            cpu_rphase_q      <= '0;
-        end else begin
-            // ---- AR offer: held stable until accepted, never withdrawn -----
-            if (cpu_ar_valid_q) begin
-                if (slv_resps[0].ar_ready) begin
-                    cpu_ar_valid_q    <= 1'b0;
-                    cpu_outstanding_q <= 1'b1;
-                    cpu_issued_q      <= cpu_issued_q + 32'd1;
-                end
-            end else if (!cpu_outstanding_q && cpu_run && (cpu_wait_q == '0)) begin
-                cpu_ar_valid_q <= 1'b1;
-                cpu_ar_id_q    <= 4'd0;
-                cpu_ar_addr_q  <= 64'h0000_0000_8000_0000; // V_SRC_PA
-                cpu_ar_len_q   <= 8'd7;                    // 8 x 8 B = 64 B
-                cpu_ar_size_q  <= 3'd3;
-                cpu_ar_burst_q <= axi_pkg::BURST_INCR;
-            end
-
-            // ---- bounded inter-read interval -------------------------------
-            if (cpu_ar_valid_q || cpu_outstanding_q) cpu_wait_q <= CPU_INTERVAL;
-            else if (cpu_wait_q != '0)               cpu_wait_q <= cpu_wait_q - 6'd1;
-
-            // ---- single outstanding read drain -----------------------------
-            if (slv_resps[0].r_valid && cpu_req.r_ready && slv_resps[0].r.last) begin
-                cpu_outstanding_q <= 1'b0;
-                cpu_completed_q   <= cpu_completed_q + 32'd1;
-            end
-
-            // ---- RREADY shape: 16 cycles ready, 4 cycles stalled -----------
-            if (cpu_rphase_q == 5'd19) cpu_rphase_q <= '0;
-            else                       cpu_rphase_q <= cpu_rphase_q + 5'd1;
-        end
-    end
-
-    always_comb begin : cpu_req_drive
-        cpu_req = '0;
-        cpu_req.ar_valid = cpu_ar_valid_q;
-        cpu_req.ar.id    = cpu_ar_id_q;
-        cpu_req.ar.addr  = cpu_ar_addr_q;
-        cpu_req.ar.len   = cpu_ar_len_q;
-        cpu_req.ar.size  = cpu_ar_size_q;
-        cpu_req.ar.burst = cpu_ar_burst_q;
-        cpu_req.ar.prot  = 3'b010;
-        cpu_req.r_ready  = (cpu_rphase_q < 5'd16);
-    end
-    assign slv_reqs[0] = cpu_req;
+    // ========================================================================
+    // Slave port 0 : synthetic CPU master (TEST-ONLY)
+    //   ID0 : plain read stream (original behaviour, unchanged)
+    //   ID1 : ATOMICLOAD + ADD 64-bit (optional; CpuWrites & DdrAtomics)
+    //   ID2 : plain 2-beat write burst (optional; CpuWrites & DdrAtomics)
+    //
+    // This is a synthetic AXI stimulus source.  It is NOT a real CPU, NOT a
+    // cache model, and makes no board-equivalence claim.  At most ONE write
+    // transaction (AW+W+B, plus the R of the AMO) is outstanding at a time.
+    // AW and W are handshaked fully independently and both valids are held
+    // stable until accepted; the per-transaction skew is rotated through
+    // W-first / simultaneous / AW-first.  The W beats of one burst always
+    // stay in order.  New transactions are only started while cpu_run is
+    // asserted; an already-started transaction is always drained.
+    // ========================================================================
+    localparam bit CpuWrEn = CpuWrites && DdrAtomics;
+
+    localparam logic [3:0]  CPU_ID_RD    = 4'd0;
+    localparam logic [3:0]  CPU_ID_AMO   = 4'd1;
+    localparam logic [3:0]  CPU_ID_WR    = 4'd2;
+
+    localparam logic [7:0]  CPU_WR_LEN   = 8'd1;            // 2 x 8 B burst
+    localparam logic [63:0] CPU_WR_ADDR  = 64'h0000_0000_8100_0000;
+    localparam logic [63:0] CPU_AMO_ADDR = 64'h0000_0000_8100_1000;
+    localparam logic [63:0] CPU_AMO_ADD  = 64'h0000_0000_1234_5678;
+    localparam logic [63:0] CPU_WR_DAT0  = 64'h1122_3344_5566_7788;
+    localparam logic [63:0] CPU_WR_DAT1  = 64'hAABB_CCDD_EEFF_0011;
+    localparam logic [7:0]  CPU_WR_STR0  = 8'hFF;
+    localparam logic [7:0]  CPU_WR_STR1  = 8'h0F;
+    // ATOMICLOAD (ATOP[5:4] = 2'b10), little endian (ATOP[3] = 0), ADD (ATOP[2:0]).
+    localparam logic [5:0]  CPU_AMO_ATOP =
+        {axi_pkg::ATOP_ATOMICLOAD, axi_pkg::ATOP_LITTLE_END, axi_pkg::ATOP_ADD};
+    // Bounded number of CPU write/AMO transactions issued per scenario.
+    localparam logic [7:0]  CPU_WR_TOTAL = 8'd64;
+
+    logic        cpu_run;
+    assign cpu_run = dbg_dma_batch_active[0] | dbg_dma_batch_gap[0];
+
+    localparam logic [5:0] CPU_INTERVAL = 6'(CpuIssueInterval);
+
+    slv_req_t    cpu_req;
+
+    // ---- plain read lane (ID0) -------------------------------------------
+    logic        cpu_ar_valid_q;
+    logic [3:0]  cpu_ar_id_q;
+    logic [63:0] cpu_ar_addr_q;
+    logic [7:0]  cpu_ar_len_q;
+    logic [2:0]  cpu_ar_size_q;
+    logic [1:0]  cpu_ar_burst_q;
+    logic        cpu_outstanding_q;
+    logic [31:0] cpu_issued_q;
+    logic [31:0] cpu_completed_q;
+    logic [31:0] cpu_rd_err_q;
+    logic [5:0]  cpu_wait_q;
+    logic [4:0]  cpu_rphase_q;
+
+    // ---- write / AMO engine ----------------------------------------------
+    typedef enum logic [1:0] {CW_IDLE, CW_ISSUE, CW_RESP} cw_state_e;
+
+    cw_state_e   cw_state_q;
+    logic        cw_amo_q;
+    logic [1:0]  cw_skew_q;
+    logic        cw_aw_done_q;
+    logic [7:0]  cw_wcnt_q;
+    logic        cw_b_done_q;
+    logic        cw_r_done_q;
+    logic [1:0]  cw_next_q;
+    logic [1:0]  cw_rot_q;
+    logic [7:0]  cw_txn_q;
+    logic [31:0] cpu_wr_issued_q;
+    logic [31:0] cpu_wr_completed_q;
+    logic [31:0] cpu_amo_issued_q;
+    logic [31:0] cpu_amo_completed_q;
+    logic [31:0] cpu_wr_err_q;
+    logic [31:0] cpu_amo_err_q;
+    logic [63:0] cpu_amo_r_data_q;
+    logic        cpu_amo_r_fire_q;
+
+    logic        cw_aw_valid;
+    logic [3:0]  cw_aw_id;
+    logic [63:0] cw_aw_addr;
+    logic [7:0]  cw_aw_len;
+    logic [5:0]  cw_aw_atop;
+    logic        cw_w_valid;
+    logic [63:0] cw_w_data;
+    logic [7:0]  cw_w_strb;
+    logic        cw_w_last;
+    logic        cw_aw_allow;
+    logic        cw_w_allow;
+    logic        cw_aw_fire;
+    logic        cw_w_fire;
+    logic        cw_aw_ok;
+    logic [8:0]  cw_wdone_next;
+    logic        cw_w_ok;
+    logic        cw_b_fire;
+    logic        cw_r_fire;
+    logic [1:0]  cw_err_add;
+
+    // ---- plain read lane FSM (ID0) ---------------------------------------
+    always_ff @(posedge clk_i or negedge rst_ni) begin : cpu_read_fsm
+        if (!rst_ni) begin
+            cpu_ar_valid_q    <= 1'b0;
+            cpu_ar_id_q       <= '0;
+            cpu_ar_addr_q     <= '0;
+            cpu_ar_len_q      <= '0;
+            cpu_ar_size_q     <= '0;
+            cpu_ar_burst_q    <= '0;
+            cpu_outstanding_q <= 1'b0;
+            cpu_issued_q      <= '0;
+            cpu_completed_q   <= '0;
+            cpu_rd_err_q      <= '0;
+            cpu_wait_q        <= CPU_INTERVAL;
+            cpu_rphase_q      <= '0;
+        end else begin
+            // ---- AR offer: held stable until accepted, never withdrawn ----
+            if (cpu_ar_valid_q) begin
+                if (slv_resps[0].ar_ready) begin
+                    cpu_ar_valid_q    <= 1'b0;
+                    cpu_outstanding_q <= 1'b1;
+                    cpu_issued_q      <= cpu_issued_q + 32'd1;
+                end
+            end else if (!cpu_outstanding_q && cpu_run && (cpu_wait_q == '0)) begin
+                cpu_ar_valid_q <= 1'b1;
+                cpu_ar_id_q    <= CPU_ID_RD;
+                cpu_ar_addr_q  <= 64'h0000_0000_8000_0000; // V_SRC_PA
+                cpu_ar_len_q   <= 8'd7;                    // 8 x 8 B = 64 B
+                cpu_ar_size_q  <= 3'd3;
+                cpu_ar_burst_q <= axi_pkg::BURST_INCR;
+            end
+
+            // ---- bounded inter-read interval ------------------------------
+            if (cpu_ar_valid_q || cpu_outstanding_q) cpu_wait_q <= CPU_INTERVAL;
+            else if (cpu_wait_q != '0)               cpu_wait_q <= cpu_wait_q - 6'd1;
+
+            // ---- single outstanding read drain ----------------------------
+            // Only an ID0 R may complete the plain read: the AMO returns on
+            // ID1 and must never be accounted as a plain read.
+            if (slv_resps[0].r_valid && cpu_req.r_ready &&
+                (slv_resps[0].r.id == CPU_ID_RD) && slv_resps[0].r.last) begin
+                cpu_outstanding_q <= 1'b0;
+                cpu_completed_q   <= cpu_completed_q + 32'd1;
+                if (slv_resps[0].r.resp != axi_pkg::RESP_OKAY)
+                    cpu_rd_err_q <= cpu_rd_err_q + 32'd1;
+            end
+
+            // ---- RREADY shape: 16 cycles ready, 4 cycles stalled ----------
+            if (cpu_rphase_q == 5'd19) cpu_rphase_q <= '0;
+            else                       cpu_rphase_q <= cpu_rphase_q + 5'd1;
+        end
+    end
+
+    // ---- write / AMO transaction descriptor ------------------------------
+    assign cw_aw_len  = cw_amo_q ? 8'd0 : CPU_WR_LEN;
+    assign cw_aw_id   = cw_amo_q ? CPU_ID_AMO : CPU_ID_WR;
+    assign cw_aw_addr = cw_amo_q ? CPU_AMO_ADDR : CPU_WR_ADDR;
+    assign cw_aw_atop = cw_amo_q ? CPU_AMO_ATOP : 6'b000000;
+
+    // W-first holds AW back until the whole W burst has been accepted;
+    // AW-first holds W back until AW has been accepted; simultaneous
+    // presents both at once.  In every mode both valids are held until
+    // accepted.
+    assign cw_w_allow  = (cw_skew_q != 2'd2) || cw_aw_done_q;
+    assign cw_aw_allow = (cw_skew_q != 2'd0) || (cw_wcnt_q == (cw_aw_len + 8'd1));
+
+    assign cw_aw_valid = (cw_state_q == CW_ISSUE) && !cw_aw_done_q && cw_aw_allow;
+    assign cw_w_valid  = (cw_state_q == CW_ISSUE) && (cw_wcnt_q <= cw_aw_len) &&
+                         cw_w_allow;
+    assign cw_w_last   = (cw_wcnt_q == cw_aw_len);
+
+    assign cw_aw_fire  = cw_aw_valid && slv_resps[0].aw_ready;
+    assign cw_w_fire   = cw_w_valid  && slv_resps[0].w_ready;
+
+    assign cw_aw_ok      = cw_aw_done_q || cw_aw_fire;
+    assign cw_wdone_next = {1'b0, cw_wcnt_q} + (cw_w_fire ? 9'd1 : 9'd0);
+    assign cw_w_ok       = cw_wdone_next > {1'b0, cw_aw_len};
+
+    always_comb begin : cw_w_payload
+        if (cw_amo_q) begin
+            cw_w_data = CPU_AMO_ADD;
+            cw_w_strb = 8'hFF;
+        end else if (cw_wcnt_q == 8'd0) begin
+            cw_w_data = CPU_WR_DAT0;
+            cw_w_strb = CPU_WR_STR0;
+        end else begin
+            cw_w_data = CPU_WR_DAT1;
+            cw_w_strb = CPU_WR_STR1;
+        end
+    end
+
+    // B / R demux for the write engine.  An AMO is complete only when BOTH a
+    // B and an R have been observed; a plain write needs only the B.
+    assign cw_b_fire = (cw_state_q == CW_RESP) && slv_resps[0].b_valid &&
+                       (slv_resps[0].b.id == cw_aw_id);
+    assign cw_r_fire = (cw_state_q == CW_RESP) && slv_resps[0].r_valid &&
+                       cpu_req.r_ready && (slv_resps[0].r.id == CPU_ID_AMO);
+
+    always_comb begin : cw_err_cnt
+        cw_err_add = 2'd0;
+        if (cw_b_fire && (slv_resps[0].b.resp != axi_pkg::RESP_OKAY))
+            cw_err_add = cw_err_add + 2'd1;
+        if (cw_r_fire && (slv_resps[0].r.resp != axi_pkg::RESP_OKAY))
+            cw_err_add = cw_err_add + 2'd1;
+    end
+
+    always_ff @(posedge clk_i or negedge rst_ni) begin : cpu_write_fsm
+        if (!rst_ni) begin
+            cw_state_q          <= CW_IDLE;
+            cw_amo_q            <= 1'b0;
+            cw_skew_q           <= 2'd0;
+            cw_aw_done_q        <= 1'b0;
+            cw_wcnt_q           <= '0;
+            cw_b_done_q         <= 1'b0;
+            cw_r_done_q         <= 1'b0;
+            cw_next_q           <= 2'd0;
+            cw_rot_q            <= 2'd0;
+            cw_txn_q            <= '0;
+            cpu_wr_issued_q     <= '0;
+            cpu_wr_completed_q  <= '0;
+            cpu_amo_issued_q    <= '0;
+            cpu_amo_completed_q <= '0;
+            cpu_wr_err_q        <= '0;
+            cpu_amo_err_q       <= '0;
+            cpu_amo_r_data_q    <= '0;
+            cpu_amo_r_fire_q    <= 1'b0;
+        end else begin
+            cpu_amo_r_fire_q <= 1'b0;
+
+            case (cw_state_q)
+              CW_IDLE: begin
+                // New transactions are only ever started while the synthetic
+                // CPU model runs, and only up to the bounded count.
+                if (CpuWrEn && cpu_run && (cw_txn_q < CPU_WR_TOTAL)) begin
+                    cw_state_q   <= CW_ISSUE;
+                    cw_amo_q     <= cw_next_q[0];
+                    cw_skew_q    <= cw_rot_q;
+                    cw_aw_done_q <= 1'b0;
+                    cw_wcnt_q    <= '0;
+                    cw_b_done_q  <= 1'b0;
+                    cw_r_done_q  <= 1'b0;
+                    cw_txn_q     <= cw_txn_q + 8'd1;
+                    if (cw_next_q[0]) cpu_amo_issued_q <= cpu_amo_issued_q + 32'd1;
+                    else              cpu_wr_issued_q  <= cpu_wr_issued_q  + 32'd1;
+                    cw_next_q <= {1'b0, ~cw_next_q[0]};
+                    cw_rot_q  <= (cw_rot_q == 2'd2) ? 2'd0 : (cw_rot_q + 2'd1);
+                end
+              end
+
+              CW_ISSUE: begin
+                if (cw_aw_ok && cw_w_ok) begin
+                    cw_state_q   <= CW_RESP;
+                    cw_aw_done_q <= cw_aw_ok;
+                    cw_wcnt_q    <= cw_wdone_next[7:0];
+                end else begin
+                    if (cw_aw_fire) cw_aw_done_q <= 1'b1;
+                    if (cw_w_fire)  cw_wcnt_q    <= cw_wcnt_q + 8'd1;
+                end
+              end
+
+              CW_RESP: begin
+                if (cw_b_fire) cw_b_done_q <= 1'b1;
+                if (cw_r_fire) begin
+                    cw_r_done_q      <= 1'b1;
+                    cpu_amo_r_data_q <= slv_resps[0].r.data;
+                    cpu_amo_r_fire_q <= 1'b1;
+                end
+
+                if (cw_err_add != 2'd0) begin
+                    if (cw_amo_q) cpu_amo_err_q <= cpu_amo_err_q + {30'b0, cw_err_add};
+                    else          cpu_wr_err_q  <= cpu_wr_err_q  + {30'b0, cw_err_add};
+                end
+
+                if ((cw_b_done_q || cw_b_fire) &&
+                    (cw_amo_q ? (cw_r_done_q || cw_r_fire) : 1'b1)) begin
+                    if (cw_amo_q) cpu_amo_completed_q <= cpu_amo_completed_q + 32'd1;
+                    else          cpu_wr_completed_q  <= cpu_wr_completed_q  + 32'd1;
+                    cw_state_q <= CW_IDLE;
+                end
+              end
+
+              default: cw_state_q <= CW_IDLE;
+            endcase
+        end
+    end
+
+    always_comb begin : cpu_req_drive
+        cpu_req = '0;
+        // ---- plain read (ID0) --------------------------------------------
+        cpu_req.ar_valid = cpu_ar_valid_q;
+        cpu_req.ar.id    = cpu_ar_id_q;
+        cpu_req.ar.addr  = cpu_ar_addr_q;
+        cpu_req.ar.len   = cpu_ar_len_q;
+        cpu_req.ar.size  = cpu_ar_size_q;
+        cpu_req.ar.burst = cpu_ar_burst_q;
+        cpu_req.ar.prot  = 3'b010;
+        // ---- plain write (ID2) / AMO (ID1) -------------------------------
+        cpu_req.aw_valid = cw_aw_valid;
+        cpu_req.aw.id    = cw_aw_id;
+        cpu_req.aw.addr  = cw_aw_addr;
+        cpu_req.aw.len   = cw_aw_len;
+        cpu_req.aw.size  = 3'd3;
+        cpu_req.aw.burst = axi_pkg::BURST_INCR;
+        cpu_req.aw.atop  = cw_aw_atop;
+        cpu_req.aw.prot  = 3'b010;
+        cpu_req.w_valid  = cw_w_valid;
+        cpu_req.w.data   = cw_w_data;
+        cpu_req.w.strb   = cw_w_strb;
+        cpu_req.w.last   = cw_w_last;
+        cpu_req.b_ready  = 1'b1;
+        // ---- RREADY shape: 16 cycles ready, 4 cycles stalled -------------
+        cpu_req.r_ready  = (cpu_rphase_q < 5'd16);
+    end
+    assign slv_reqs[0] = cpu_req;
@@ -844,6 +844,18 @@
     assign dbg_cpu_issued    = cpu_issued_q;
     assign dbg_cpu_completed = cpu_completed_q;
     assign dbg_cpu_active    = cpu_run;
-    assign dbg_cpu_quiet     = ~cpu_ar_valid_q & ~cpu_outstanding_q & ~cpu_run;
+    assign dbg_cpu_quiet     = ~cpu_ar_valid_q & ~cpu_outstanding_q & ~cpu_run &
+                               (cw_state_q == CW_IDLE);
+
+    assign dbg_cpu_wr_issued     = cpu_wr_issued_q;
+    assign dbg_cpu_wr_completed  = cpu_wr_completed_q;
+    assign dbg_cpu_amo_issued    = cpu_amo_issued_q;
+    assign dbg_cpu_amo_completed = cpu_amo_completed_q;
+    assign dbg_cpu_rd_err        = cpu_rd_err_q;
+    assign dbg_cpu_wr_err        = cpu_wr_err_q;
+    assign dbg_cpu_amo_err       = cpu_amo_err_q;
+    assign dbg_cpu_wr_quiet      = (cw_state_q == CW_IDLE);
+    assign dbg_cpu_amo_r_fire    = cpu_amo_r_fire_q;
+    assign dbg_cpu_amo_r_data    = cpu_amo_r_data_q;
 
 endmodule

--- a/tests/nonblock_actual_dma_test.cpp
+++ b/tests/nonblock_actual_dma_test.cpp
@@ -123,8 +123,33 @@
 static uint64_t page_old(uint32_t page, uint32_t i) {
     return 0xD000000000000000ULL ^ ((uint64_t)(page + 1) << 40) ^ (uint64_t)i;
 }
 
+#ifdef ACTUAL_DMA_SYSTEM
+// ---------------------------------------------------------------------------
+// Synthetic-CPU write / AMO scoreboard constants.  These MUST match the
+// test-only RTL constants in tests/nonblock_actual_dma_system_top.sv
+// (CPU_WR_ADDR / CPU_AMO_ADDR / CPU_AMO_ADD / CPU_WR_DAT0 / CPU_WR_DAT1 /
+//  CPU_WR_STR0 / CPU_WR_STR1).  Both sides are test code.
+// ---------------------------------------------------------------------------
+static const uint64_t CPU_WR_ADDR  = 0x81000000ULL;
+static const uint64_t CPU_AMO_ADDR = 0x81001000ULL;
+static const uint64_t CPU_AMO_ADD  = 0x0000000012345678ULL;
+static const uint64_t CPU_WR_DAT0  = 0x1122334455667788ULL;
+static const uint64_t CPU_WR_DAT1  = 0xAABBCCDDEEFF0011ULL;
+static const uint8_t  CPU_WR_STR0  = 0xFF;
+static const uint8_t  CPU_WR_STR1  = 0x0F;
+static const uint64_t CPU_WR_INIT1 = 0xE000000000000001ULL;
+static const uint64_t CPU_AMO_INIT = 0xD000000000000000ULL;
+
+static uint64_t cpu_apply_strb(uint64_t cur, uint64_t data, uint8_t strb) {
+    uint64_t mask = 0;
+    for (int i = 0; i < 8; ++i)
+        if (strb & (uint8_t)(1u << i)) mask |= (0xFFULL << (8 * i));
+    return (cur & ~mask) | (data & mask);
+}
+#endif
+
 struct TestFailure : std::runtime_error {
     explicit TestFailure(const std::string& m) : std::runtime_error(m) {}
 };
@@ -210,8 +235,22 @@
 #ifdef ACTUAL_DMA_SYSTEM
     // Synthetic-CPU independent progress baseline (refreshed only by the CPU
     // lifecycle counters, never by DMA handshakes).
     long     cpu_progress_cycle  = 0;
     long     cpu_watchdog_cycles = 200000L;
     uint32_t last_cpu_issued     = 0;
     uint32_t last_cpu_completed  = 0;
+    // Synthetic-CPU write/AMO scoreboard.  `cpu_writes` mirrors the run-time
+    // +cpu-writes flag; the counters below are refreshed ONLY by the new
+    // write/AMO lifecycle taps, so a stalled write path cannot hide behind
+    // read progress (and vice versa).
+    bool     cpu_writes             = false;
+    long     cpu_wr_progress_cycle  = 0;
+    long     cpu_wr_watchdog_cycles = 200000L;
+    uint32_t last_cpu_wr_issued     = 0;
+    uint32_t last_cpu_wr_completed  = 0;
+    uint32_t last_cpu_amo_issued    = 0;
+    uint32_t last_cpu_amo_completed = 0;
+    // AMO old-value accumulation scoreboard (bounded: CPU_WR_TOTAL/2 AMOs).
+    uint64_t cpu_amo_expected  = 0;
+    uint64_t n_cpu_amo_obs     = 0;
 #endif
@@ -318,10 +357,19 @@
         for (uint32_t p = 0; p < a_pages; ++p) {
             for (uint32_t i = 0; i < PAGE_BEATS; ++i) {
                 write64(A_SRC_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                         page_word(1 + p, i));
                 write64(A_DST_PA_BASE + (uint64_t)p * 0x1000 + (uint64_t)i * 8,
                         page_old(1 + p, i));
             }
         }
+
+#ifdef ACTUAL_DMA_SYSTEM
+        // Synthetic-CPU write / AMO scratch: distinct from V (0x8000_0000),
+        // A (0x9000_0000+), the DDT (0x1_0000) and every page table.
+        write64(CPU_WR_ADDR + 0, 0xE000000000000000ULL);
+        write64(CPU_WR_ADDR + 8, CPU_WR_INIT1);
+        for (uint32_t i = 0; i < 8; ++i)
+            write64(CPU_AMO_ADDR + (uint64_t)i * 8, CPU_AMO_INIT + (uint64_t)i);
+#endif
     }
 
@@ -546,13 +594,31 @@
 #ifdef ACTUAL_DMA_SYSTEM
         // Underlying walker counters: the real ds_if merge hides the external
         // dd/ptw ports, so walk evidence comes from the harness taps.
         if (top->dbg_ddtw_ar_fire) ++n_walk_ddtw;
         if (top->dbg_ptw_ar_fire)  ++n_walk_ptw;
         // Synthetic-CPU independent progress.
         if (top->dbg_cpu_issued != last_cpu_issued ||
             top->dbg_cpu_completed != last_cpu_completed) {
             cpu_progress_cycle = g_cycle;
             last_cpu_issued    = top->dbg_cpu_issued;
             last_cpu_completed = top->dbg_cpu_completed;
         }
+        // Synthetic-CPU write / AMO independent progress.  Refreshed ONLY by
+        // the new write/AMO lifecycle taps, so a stalled write path cannot be
+        // hidden by read progress.
+        if (top->dbg_cpu_wr_issued != last_cpu_wr_issued ||
+            top->dbg_cpu_wr_completed != last_cpu_wr_completed ||
+            top->dbg_cpu_amo_issued != last_cpu_amo_issued ||
+            top->dbg_cpu_amo_completed != last_cpu_amo_completed) {
+            cpu_wr_progress_cycle  = g_cycle;
+            last_cpu_wr_issued     = top->dbg_cpu_wr_issued;
+            last_cpu_wr_completed  = top->dbg_cpu_wr_completed;
+            last_cpu_amo_issued    = top->dbg_cpu_amo_issued;
+            last_cpu_amo_completed = top->dbg_cpu_amo_completed;
+        }
+        // AMO old-value accumulation continuity scoreboard.
+        if (top->dbg_cpu_amo_r_fire) {
+            CHECK(top->dbg_cpu_amo_r_data == cpu_amo_expected,
+                  "AMO returned old value broke accumulation continuity");
+            cpu_amo_expected += CPU_AMO_ADD;
+            ++n_cpu_amo_obs;
+        }
 #endif
@@ -771,8 +837,15 @@
 #ifdef ACTUAL_DMA_SYSTEM
         n_ddr_ar = n_ddr_aw = n_ddr_b = n_ddr_r = 0;
         n_slv_ar[0] = n_slv_ar[1] = n_slv_ar[2] = n_slv_ar[3] = 0;
         n_walk_ddtw = n_walk_ptw = 0;
         cpu_progress_cycle = g_cycle;
         last_cpu_issued    = 0;
         last_cpu_completed = 0;
+        cpu_wr_progress_cycle  = g_cycle;
+        last_cpu_wr_issued     = 0;
+        last_cpu_wr_completed  = 0;
+        last_cpu_amo_issued    = 0;
+        last_cpu_amo_completed = 0;
+        cpu_amo_expected       = CPU_AMO_INIT;
+        n_cpu_amo_obs          = 0;
 #endif
@@ -795,13 +868,19 @@
 #ifdef ACTUAL_DMA_SYSTEM
-            const bool cpu_q = top->dbg_cpu_quiet;
+            const bool cpu_q = top->dbg_cpu_quiet && top->dbg_cpu_wr_quiet;
 #else
             const bool cpu_q = true;
 #endif
             if (top->dbg_in_flight == 0 && top->dbg_dma_job_quiet == 3 && cpu_q) {
                 quiet = true;
                 break;
             }
             if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                 dump_stall(std::string("quiet ") + tag);
                 throw TestFailure("watchdog during drain-to-quiet");
             }
+#ifdef ACTUAL_DMA_SYSTEM
+            if (cpu_writes && !top->dbg_cpu_wr_quiet &&
+                (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
+                dump_stall(std::string("quiet cpu-wr ") + tag);
+                throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
+            }
+#endif
@@ -883,5 +962,14 @@
 #ifdef ACTUAL_DMA_SYSTEM
         std::fprintf(stderr,
             "  SYS cpu issued=%u completed=%u quiet=%d active=%d\n",
             (unsigned)top->dbg_cpu_issued, (unsigned)top->dbg_cpu_completed,
             (int)top->dbg_cpu_quiet, (int)top->dbg_cpu_active);
+        std::fprintf(stderr,
+            "  SYS cpu-wr issued=%u completed=%u amo_issued=%u amo_completed=%u "
+            "quiet=%d amo_r_fire=%d amo_r_data=0x%016llx rd_err=%u wr_err=%u amo_err=%u\n",
+            (unsigned)top->dbg_cpu_wr_issued, (unsigned)top->dbg_cpu_wr_completed,
+            (unsigned)top->dbg_cpu_amo_issued, (unsigned)top->dbg_cpu_amo_completed,
+            (int)top->dbg_cpu_wr_quiet, (int)top->dbg_cpu_amo_r_fire,
+            (unsigned long long)top->dbg_cpu_amo_r_data,
+            (unsigned)top->dbg_cpu_rd_err, (unsigned)top->dbg_cpu_wr_err,
+            (unsigned)top->dbg_cpu_amo_err);
@@ -1258,7 +1346,12 @@
 #ifdef ACTUAL_DMA_SYSTEM
                 if ((top->dbg_dma_batch_active & 1) &&
                     (g_cycle - cpu_progress_cycle > cpu_watchdog_cycles)) {
                     dump_stall("scenario CPU progress");
                     throw TestFailure("watchdog: synthetic CPU made no progress");
                 }
+                if (cpu_writes && !top->dbg_cpu_wr_quiet &&
+                    (g_cycle - cpu_wr_progress_cycle > cpu_wr_watchdog_cycles)) {
+                    dump_stall("scenario CPU write progress");
+                    throw TestFailure("watchdog: synthetic CPU write/AMO made no progress");
+                }
 #endif
@@ -1331,13 +1424,48 @@
         // The idle debug port must never produce shared-DDR traffic.
         CHECK(n_slv_ar[1] == 0, "idle debug port produced shared-DDR reads");
         if (!enable) {
             CHECK(n_fast == 0 && n_fast_bank[0] == 0 && n_fast_bank[1] == 0,
                   "nonblocking fast path fired while nonblocking is disabled");
         }
+        // ---- optional synthetic CPU write / AMO evidence -------------------
+        if (cpu_writes) {
+            CHECK(top->dbg_cpu_wr_completed  > 0,
+                  "synthetic CPU completed no plain writes");
+            CHECK(top->dbg_cpu_amo_completed > 0,
+                  "synthetic CPU completed no AMOs");
+            CHECK(top->dbg_cpu_rd_err  == 0,
+                  "synthetic CPU read received a non-OKAY response");
+            CHECK(top->dbg_cpu_wr_err  == 0,
+                  "synthetic CPU write received a non-OKAY response");
+            CHECK(top->dbg_cpu_amo_err == 0,
+                  "synthetic CPU AMO received a non-OKAY response");
+            CHECK(n_cpu_amo_obs == (uint64_t)top->dbg_cpu_amo_completed,
+                  "AMO R observations do not match completed AMOs");
+            // Plain write data / strobe actually landed in DDR.
+            CHECK(read64(CPU_WR_ADDR) == CPU_WR_DAT0,
+                  "synthetic CPU plain write beat 0 data was not stored");
+            CHECK(read64(CPU_WR_ADDR + 8) ==
+                      cpu_apply_strb(CPU_WR_INIT1, CPU_WR_DAT1, CPU_WR_STR1),
+                  "synthetic CPU plain write beat 1 data/strb was not honoured");
+            // AMO accumulation reached the expected final DDR value.
+            CHECK(read64(CPU_AMO_ADDR) ==
+                      CPU_AMO_INIT +
+                      (uint64_t)top->dbg_cpu_amo_completed * CPU_AMO_ADD,
+                  "synthetic CPU AMO final DDR value does not match the addend sum");
+            std::printf("SYSTEM cpu-wr wr_issued=%u wr_completed=%u amo_issued=%u "
+                        "amo_completed=%u amo_obs=%llu rd_err=%u wr_err=%u amo_err=%u "
+                        "wr_mem=0x%016llx amo_mem=0x%016llx\n",
+                        (unsigned)top->dbg_cpu_wr_issued,
+                        (unsigned)top->dbg_cpu_wr_completed,
+                        (unsigned)top->dbg_cpu_amo_issued,
+                        (unsigned)top->dbg_cpu_amo_completed,
+                        (unsigned long long)n_cpu_amo_obs,
+                        (unsigned)top->dbg_cpu_rd_err,
+                        (unsigned)top->dbg_cpu_wr_err,
+                        (unsigned)top->dbg_cpu_amo_err,
+                        (unsigned long long)read64(CPU_WR_ADDR),
+                        (unsigned long long)read64(CPU_AMO_ADDR));
+        }
         std::printf("SYSTEM walk ddtw=%lu ptw=%lu ddr_ar=%lu ddr_aw=%lu ddr_r=%lu "
                     "slv_ar cpu=%lu ds=%lu comp=%lu cpu_issued=%u cpu_completed=%u\n",
                     (unsigned long)n_walk_ddtw, (unsigned long)n_walk_ptw,
                     (unsigned long)n_ddr_ar, (unsigned long)n_ddr_aw,
                     (unsigned long)n_ddr_r, (unsigned long)n_slv_ar[0],
                     (unsigned long)n_slv_ar[3], (unsigned long)n_slv_ar[2],
                     (unsigned)top->dbg_cpu_issued, (unsigned)top->dbg_cpu_completed);
@@ -1379,23 +1507,29 @@
     std::string mode     = "s";
     uint32_t    a_pages  = 8;
     uint32_t    v_gap    = 1000;
     uint32_t    transfers= 64;
     uint32_t    warmup   = 256;
     uint32_t    ptw_lat  = 31;
     uint64_t    a_target = 1048576ULL;
+#ifdef ACTUAL_DMA_SYSTEM
+    bool        cpu_writes = false;
+#endif
 
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         auto val = [&](const char* k) -> const char* {
             const std::string key = std::string("+") + k + "=";
             if (a.rfind(key, 0) == 0) return a.c_str() + key.size();
             return nullptr;
         };
         if (const char* v = val("mode"))      mode = v;
         if (const char* v = val("a-pages"))   a_pages = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("v-gap"))     v_gap = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("transfers")) transfers = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("warmup"))    warmup = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("ptw-latency")) ptw_lat = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("a-target"))  a_target = std::strtoull(v, nullptr, 0);
+#ifdef ACTUAL_DMA_SYSTEM
+        if (const char* v = val("cpu-writes")) cpu_writes = (std::strtoul(v, nullptr, 0) != 0);
+#endif
     }
@@ -1427,19 +1561,25 @@
     std::printf("nonblock_actual_dma_test: mode=%s (nonblock_enable=%d)\n",
                 mode.c_str(), (int)enable);
     std::printf("nonblock_actual_dma_test: a_pages=%u v_gap=%u transfers=%u warmup=%u "
                 "ptw_latency=%u a_target=%llu\n",
                 a_pages, v_gap, transfers, warmup, ptw_lat,
                 (unsigned long long)a_target);
     std::printf("NOTE: ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched.\n");
+#ifdef ACTUAL_DMA_SYSTEM
+    std::printf("nonblock_actual_dma_test: cpu_writes=%d\n", (int)cpu_writes);
+#endif
 
     int rc = 0;
     try {
         Tb tb;
         tb.a_pages         = a_pages;
         tb.v_gap_cycles    = v_gap;
         tb.v_transfers     = transfers;
         tb.warmup          = warmup;
         tb.ptw_latency     = ptw_lat;
         tb.a_target        = a_target;
+#ifdef ACTUAL_DMA_SYSTEM
+        tb.cpu_writes      = cpu_writes;
+#endif
         tb.policy_partition= false;
         tb.run_scenario(enable ? "n" : "s", enable);

--- a/tests/test_nonblock_actual_dma.py
+++ b/tests/test_nonblock_actual_dma.py
@@ -228,7 +228,14 @@
     ap.add_argument("--system-atomics", action="store_true",
                     help="system top with board production DDR atomics adapter; independent build")
+    ap.add_argument("--cpu-writes", action="store_true",
+                    help="--system-atomics build plus the optional synthetic CPU "
+                         "plain-write / AMO traffic (CpuWrites=1); requires "
+                         "--system-atomics and uses the independent object "
+                         "directory out/nonblock-actual-dma/obj-system-atomics-cpu-writes")
     opts = ap.parse_args()
     if opts.system_atomics:
         opts.system = True
+    if opts.cpu_writes and not opts.system_atomics:
+        ap.error("--cpu-writes requires --system-atomics")
 
     sibling = load_sibling()
@@ -248,8 +255,11 @@
     if opts.system_atomics:
         obj_dir = OUT / "obj-system-atomics"
         defines += ["-GDdrAtomics=1"]
+    if opts.cpu_writes:
+        obj_dir = OUT / "obj-system-atomics-cpu-writes"
+        defines += ["-GCpuWrites=1"]
     atomics = CVA6 / "corev_apu" / "src" / "axi_riscv_atomics" / "src"
     if opts.system:
         extra += [require(atomics / name) for name in (
             "axi_res_tbl.sv", "axi_riscv_lrsc.sv", "axi_riscv_amos_alu.sv",
             "axi_riscv_amos.sv", "axi_riscv_atomics.sv")]
@@ -286,2 +296,3 @@
-    build_log = OUT / ("build-system-atomics.log" if opts.system_atomics else
-                       "build-system.log" if opts.system else "build.log")
+    build_log = OUT / ("build-system-atomics-cpu-writes.log" if opts.cpu_writes else
+                       "build-system-atomics.log" if opts.system_atomics else
+                       "build-system.log" if opts.system else "build.log")
@@ -310,6 +321,11 @@
     if opts.system:
         run_args += ["+system=1"]
-    print("test_nonblock_actual_dma: system={} top={} mode={} a_pages={} v_gap={} "
-          "transfers={} warmup={} ptw_latency={} a_target={}".format(
-              int(opts.system), top_name, opts.mode, opts.a_pages, opts.v_gap,
-              opts.transfers, opts.warmup, opts.ptw_latency, opts.a_target))
+    if opts.cpu_writes:
+        run_args += ["+cpu-writes=1"]
+    print("test_nonblock_actual_dma: system={} cpu_writes={} top={} mode={} "
+          "a_pages={} v_gap={} transfers={} warmup={} ptw_latency={} a_target={}".format(
+              int(opts.system), int(opts.cpu_writes), top_name, opts.mode,
+              opts.a_pages, opts.v_gap, opts.transfers, opts.warmup,
+              opts.ptw_latency, opts.a_target))
```

---

## What each new check actually proves

| Check (C++) | Proves |
|---|---|
| `dbg_cpu_wr_completed > 0` | Plain-write channel (ID2, AW+W→B) really completed inside the batch window. |
| `read64(CPU_WR_ADDR) == CPU_WR_DAT0` | Beat 0 (`strb 0xFF`) reached the DDR model through xbar + atomics pass-through. |
| `read64(CPU_WR_ADDR+8) == apply_strb(INIT1, DAT1, STR1)` | Beat 1 **and its `0x0F` strobe** were honoured (bytes 4..7 keep the init fill). |
| `dbg_cpu_amo_completed > 0`, no `amo_err` | AMO (ID1, `atop=ATOP_ATOMICLOAD|LITTLE_END|ADD`, `len=0`, `size=3`) completed with both B **and** R. |
| `dbg_cpu_amo_r_data == cpu_amo_expected` on every `dbg_cpu_amo_r_fire` | The returned **old value** follows the exact addend accumulation `INIT, INIT+A, INIT+2A …`. |
| `read64(CPU_AMO_ADDR) == INIT + completed*A` | DDR **final value** equals the addend sum (RTL RMW arithmetic is correct). |
| `n_cpu_amo_obs == dbg_cpu_amo_completed` | The AMO R was counted exactly once per AMO and was **not** folded into the read lane. |
| `dbg_cpu_rd_err/wr_err/amo_err == 0` | No `SLVERR`/`DECERR` on any new or existing CPU channel. |
| `dbg_cpu_quiet && dbg_cpu_wr_quiet` at terminal | The quiet condition now includes AW/W/B/R of ID1/ID2. |
| Watchdogs (`cpu_progress_cycle`, `cpu_wr_progress_cycle`) | Independent, bounded progress for read vs. write/AMO; a stuck write cannot hide behind read traffic or vice versa. |

---

## Acceptance commands (to be run by Codex)

```bash
# 1. Legacy non-system build must still compile and pass (default behaviour unchanged)
python3 tests/test_nonblock_actual_dma.py --mode s --transfers 64
python3 tests/test_nonblock_actual_dma.py --mode n --transfers 64

# 2. Existing system build, no atomics: unchanged
python3 tests/test_nonblock_actual_dma.py --system --mode n --transfers 64

# 3. Existing system-atomics build: unchanged (obj-system-atomics)
python3 tests/test_nonblock_actual_dma.py --system-atomics --mode n --transfers 64

# 4. NEW: system-atomics + CPU writes/AMO (separate obj dir)
python3 tests/test_nonblock_actual_dma.py --system-atomics --cpu-writes --mode n --transfers 64

# Expected new artifacts:
#   out/nonblock-actual-dma/obj-system-atomics-cpu-writes/Vnonblock_actual_dma_system_top
#   out/nonblock-actual-dma/build-system-atomics-cpu-writes.log
# Expected new terminal evidence:
#   SYSTEM cpu-wr wr_issued=32 wr_completed=32 amo_issued=32 amo_completed=32
#               amo_obs=32 rd_err=0 wr_err=0 amo_err=0 wr_mem=... amo_mem=...
#   nonblock_actual_dma_test: PASS
```

---

## Unresolved issues / things Codex must verify

1. **W-before-AW skew vs. the pulp `axi_xbar`.** The rotation emits a genuine W-first transaction. If `axi_xbar_unmuxed`/`axi_mux` require AW to be accepted before any W beat, the W-first transaction stalls. The new C++ write watchdog converts that into a bounded FAIL rather than a hang. If it fires, drop `2'd0` from `cw_rot_q`'s rotation (one line).
2. **`axi_riscv_atomics` RMW sequencing.** Assumed: it buffers W, issues its own AR, waits R, issues its own AW/W, then returns B and R to the slave port in either order. The engine accepts B and R in any order, but the assumption about *when* the adapter needs W relative to AW has not been executed here.
3. **B/R ID echo.** `cw_b_fire` requires `slv_resps[0].b.id == cw_aw_id` (1 or 2) and `cw_r_fire` requires ID 1. If the adapter/xbar alters the returned ID, the FSM stalls and the watchdog fires — this is deliberate, to make an ID bug visible rather than silent.
4. **No test has been run.** I cannot execute Verilator; nothing above asserts observed PASS/fail behaviour.