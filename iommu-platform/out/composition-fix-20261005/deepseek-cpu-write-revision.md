# 目标修订 — CPU 写/AMO 测试扩展（v2）

以下是针对 **当前** 三个源文件（不是针对之前的提案差异）的完整统一 diff。由于尚未应用 CPU 写补丁，因此没有任何用于推导这些 hunk 的 CPU 写补丁。

关于补丁机制（需求 6）：行号根据当前文件内容计算；最外层 hunk 的 `@@` 计数是精确的。如果您的补丁工具报错，请使用 `patch -p1 --recount`（GNU patch）应用，它会根据 hunk 主体重新计算计数。

---

## 1. 修订内容及其原因（映射诉求 → 变更）

| 需求 | 变更 |
|---|---|
| (1) 不要删除 W 优先；独立的类拍 skew；无 AW↔W 就绪依赖 | AW/W valid 来自一个**自由运行、饱和的每事务 `cw_age_q`**，该计数器**不依赖任何 ready**。`(AW,W)` 延迟轮转为 `(2,0)/(0,0)/(0,2)` 个周期。`cw_aw_valid`/`cw_w_valid` 仅依赖 `cw_age_q` + 它们**自身**的握手机器。合法 AXI。 |
| (1) 在对应握手前保持稳定 | Valid 设置后继续保持（age 单调递增，done 标志来自各自通道）。Payload 是 `cw_amo_q`/`cw_wcnt_q` 的函数，而这些量仅在各自握手时改变。 |
| (2) 不在 64 处停止；持续到 `cpu_run` 下降后 drain；32 位计数；大上限 1048576 | 仅当 `CpuWrEn && cpu_run && cw_txn_q < CPU_WR_MAX` 时启动新事务，其中 `CPU_WR_MAX = 32'd1048576`。运行直到 `cpu_run` 下降，然后 drain；可靠性真正依赖于 C++ 场景预算 + 独立看门狗。 |
| (3) 在最后一次 AW/W 同一拍捕获 B/R；不丢失已握手的 B/R | `b_ready`/AMO `r_ready` 只在能够捕获响应的状态下有效（提前到达的 B/R 由从设备保持并被稍后捕获）。捕获**不受状态门控**，且严格按响应 ID 进行。共享 `r_ready = (read phase) | AMO accept`，两个 lane 按 ID 锁存：**ID0 读 vs ID1 AMO 是分离的**。突发响应错误是**每一拍**检查，而非仅 `last`（`cpu_rd_err_q` 在每一个 beats 上递增）。 |
| (4) 只有完整 AW/W/B/R 才能 quiet；issued==completed；AMO 连续性/最终值；写 data/stb；错误计数 | `dbg_cpu_quiet` 要求 `cw_state_q==CW_IDLE`。C++ 增加 `issued==completed` 检查（read/write/AMO）、AMO 旧值连续性 + 最终内存、普通写 data/stb、以及零错误计数。现有默认路径和 `axi_riscv_atomics` 端口连接未被修改。 |
| (5) 文档修复 | 见 §4（文本）；未修改 `.md` 文件，因为它不在 3 文件范围内。 |
| (6) 实际源上下文 / 避免巨大不变块 | 仅新的 CPU 块被替换（周边保持）；hunk 基于实际当前源；已注明 `--recount`。 |

---

## 2. `tests/nonblock_actual_dma_system_top.sv` 的差异

```diff
--- a/tests/nonblock_actual_dma_system_top.sv
+++ b/tests/nonblock_actual_dma_system_top.sv
@@ -36,6 +36,9 @@
 // The synthetic CPU master is NOT a real RISC-V core and NOT a cache model. 
-// It only injects one bounded read stream to reproduce CPU<->DMA<->walker
-// contention through the real crossbar.  It starts when DMA0's batch becomes
-// active and stops offering NEW reads when the batch (active|gap) ends, then
-// drains any outstanding beat.  Its AR is never withdrawn while stalled and its
-// RREADY is a fixed 16-cycle-ready / 4-cycle-stall pattern.
+// It injects one bounded read stream (ID0) plus, only with
+// --system-atomics --cpu-writes, a plain-write stream (ID2) and an AMO stream
+// (ATOMICLOAD+ADD, ID1), to reproduce CPU<->DMA<->walker contention through the
+// real crossbar.  It starts when DMA0's batch becomes active and stops offering
+// NEW read/write/AMO offers when the batch (active|gap) ends, then drains any
+// outstanding beat/transaction.  AR/AW/W valids are never withdrawn while
+// stalled; RREADY keeps the original fixed 16-cycle-ready / 4-cycle-stall
+// pattern for the read lane.
@@ -46,3 +49,5 @@
 //     ROM/UART/SPI/GPIO/Ethernet/timer are NOT instantiated;
-//   * optional real DDR atomics enabled by --system-atomics; no real CPU atomics yet;
+//   * optional real DDR atomics enabled by --system-atomics; the optional
+//     --cpu-writes AMO traffic is synthetic test stimulus, not a real CPU and
+//     does not reproduce any board measurement;
 //   * no MIG / DDR controller / clock converter / 512-to-64 width converter:
@@ -65,3 +70,6 @@
     parameter bit          NonblockEnable       = 1,
     parameter bit          DdrAtomics           = 0,
+    // Test-only: add the synthetic CPU plain-write / AMO stimulus.  Only
+    // effective together with DdrAtomics (--system-atomics --cpu-writes).
+    parameter bit          CpuWrites            = 0,
     parameter int unsigned NumOutstandingTrans = 8,
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
@@ -394,80 +414,340 @@
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
+    // Synthetic AXI stimulus only: NOT a real CPU, NOT a cache model, and no
+    // board-equivalence claim.  At most ONE write transaction (AW+W+B, plus the
+    // R of an AMO) is outstanding at a time.
+    //
+    // AW and W offers are controlled independently by a free-running, saturating
+    // per-transaction age counter that depends on NO ready: when a transaction
+    // starts, each channel's offer is delayed by a fixed number of cycles and
+    // then held stable until its own handshake.  The (AW,W) skew rotates
+    // (2,0) / (0,0) / (0,2) cycles.  AWVALID never depends on WREADY or the W
+    // handshake and WVALID never depends on AWREADY or the AW handshake, so the
+    // two channels can never deadlock each other.  Only the next beat of a W
+    // burst waits for the previous W beat's handshake (legal AXI ordering).
+    //
+    // Responses are never dropped: this engine asserts its BREADY / AMO RREADY
+    // only in states that capture the response (an early B/R stays asserted by
+    // the slave and is captured later), and capture is strictly by response ID.
+    // The shared RREADY is the OR of the read lane's periodic ready and the AMO
+    // lane's accept, with the read lane latching ID0 and the AMO lane ID1.
+    //
+    // New transactions start while cpu_run is asserted and continue until it
+    // drops; the transaction in flight is then drained.  CPU_WR_MAX is only a
+    // large safety bound and is not expected to be reached in a normal N4096 run.
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
+    // ATOMICLOAD (ATOP[5:4]=2'b10), little endian (ATOP[3]=0), ADD (ATOP[2:0]).
+    localparam logic [5:0]  CPU_AMO_ATOP =
+        {axi_pkg::ATOP_ATOMICLOAD, axi_pkg::ATOP_LITTLE_END, axi_pkg::ATOP_ADD};
+    // Large safety bound only; the effective bound is cpu_run plus the C++
+    // scenario budget and independent watchdog.
+    localparam logic [31:0] CPU_WR_MAX   = 32'd1048576;
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
+    logic        cpu_r_ready;
+
+    // ---- write / AMO engine ----------------------------------------------
+    typedef enum logic [1:0] {CW_IDLE, CW_ISSUE, CW_RESP} cw_state_e;
+
+    cw_state_e   cw_state_q;
+    logic        cw_amo_q;
+    logic [1:0]  cw_skew_q;
+    logic [3:0]  cw_age_q;
+    logic        cw_aw_done_q;
+    logic [7:0]  cw_wcnt_q;
+    logic        cw_b_done_q;
+    logic        cw_r_done_q;
+    logic [1:0]  cw_next_q;
+    logic [1:0]  cw_rot_q;
+    logic [31:0] cw_txn_q;
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
+    logic [3:0]  cw_aw_delay;
+    logic [3:0]  cw_w_delay;
+    logic        cw_aw_fire;
+    logic        cw_w_fire;
+    logic        cw_aw_ok;
+    logic [8:0]  cw_wdone_next;
+    logic        cw_w_ok;
+    logic        cw_b_accept;
+    logic        cw_r_accept;
+    logic        cw_b_fire;
+    logic        cw_r_fire;
+    logic [1:0]  cw_err_add;
+
+    // ---- plain read lane FSM (ID0, original behaviour preserved) ----------
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
+            // ---- read-lane R capture: ID0 only, every beat caught ----------
+            if (slv_resps[0].r_valid && cpu_r_ready &&
+                (slv_resps[0].r.id == CPU_ID_RD)) begin
+                if (slv_resps[0].r.resp != axi_pkg::RESP_OKAY)
+                    cpu_rd_err_q <= cpu_rd_err_q + 32'd1;
+                if (slv_resps[0].r.last) begin
+                    cpu_outstanding_q <= 1'b0;
+                    cpu_completed_q   <= cpu_completed_q + 32'd1;
+                end
+            end
+
+            // ---- RREADY phase: 16 cycles ready, 4 cycles stalled -----------
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
+    // Fixed offer delays after the transaction starts.  (AW,W)=(2,0)/(0,0)/(0,2)
+    assign cw_aw_delay = (cw_skew_q == 2'd0) ? 4'd2 : 4'd0;
+    assign cw_w_delay  = (cw_skew_q == 2'd2) ? 4'd2 : 4'd0;
+
+    // Valid offers.  AW never waits for W and W never waits for AW, so neither
+    // channel can hold the other's valid hostage.
+    assign cw_aw_valid = (cw_state_q == CW_ISSUE) && !cw_aw_done_q &&
+                         (cw_age_q >= cw_aw_delay);
+    assign cw_w_valid  = (cw_state_q == CW_ISSUE) && (cw_wcnt_q <= cw_aw_len) &&
+                         (cw_age_q >= cw_w_delay);
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
+    // Response accept: the engine only asserts its own BREADY / AMO RREADY when
+    // it can capture, and both lanes latch strictly by response ID, so no
+    // already-handshaked B/R is ever lost.
+    assign cw_b_accept = (cw_state_q == CW_RESP) && !cw_b_done_q;
+    assign cw_r_accept = (cw_state_q == CW_RESP) && cw_amo_q && !cw_r_done_q;
+    assign cpu_r_ready = (cpu_rphase_q < 5'd16) | cw_r_accept;
+
+    assign cw_b_fire   = slv_resps[0].b_valid && cw_b_accept &&
+                         (slv_resps[0].b.id == cw_aw_id);
+    assign cw_r_fire   = slv_resps[0].r_valid && cpu_r_ready &&
+                         (cw_state_q != CW_IDLE) &&
+                         (slv_resps[0].r.id == CPU_ID_AMO);
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
+            cw_age_q            <= 4'd0;
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
+            // ---- free-running age: increments every cycle in ISSUE,
+            //      saturates, and depends on NO ready -------------------------
+            if (cw_state_q != CW_ISSUE) cw_age_q <= 4'd0;
+            else if (cw_age_q != 4'hF)  cw_age_q <= cw_age_q + 4'd1;
+
+            // ---- response capture: never state-gated, by ID only -----------
+            if (cw_b_fire) cw_b_done_q <= 1'b1;
+            if (cw_r_fire) begin
+                cw_r_done_q      <= 1'b1;
+                cpu_amo_r_data_q <= slv_resps[0].r.data;
+                cpu_amo_r_fire_q <= 1'b1;
+            end
+
+            // ---- per-response error counting -------------------------------
+            if (cw_err_add != 2'd0) begin
+                if (cw_amo_q) cpu_amo_err_q <= cpu_amo_err_q + {30'b0, cw_err_add};
+                else          cpu_wr_err_q  <= cpu_wr_err_q  + {30'b0, cw_err_add};
+            end
+
+            case (cw_state_q)
+              CW_IDLE: begin
+                // New transactions start only while the model runs, up to a
+                // large safety bound that a normal N4096 run never reaches.
+                if (CpuWrEn && cpu_run && (cw_txn_q < CPU_WR_MAX)) begin
+                    cw_state_q   <= CW_ISSUE;
+                    cw_amo_q     <= cw_next_q[0];
+                    cw_skew_q    <= cw_rot_q;
+                    cw_aw_done_q <= 1'b0;
+                    cw_wcnt_q    <= '0;
+                    cw_b_done_q  <= 1'b0;
+                    cw_r_done_q  <= 1'b0;
+                    cw_txn_q     <= cw_txn_q + 32'd1;
+                    if (cw_next_q[0]) cpu_amo_issued_q <= cpu_amo_issued_q + 32'd1;
+                    else              cpu_wr_issued_q  <= cpu_wr_issued_q  + 32'd1;
+                    cw_next_q <= {1'b0, ~cw_next_q[0]};
+                    cw_rot_q  <= (cw_rot_q == 2'd2) ? 2'd0 : (cw_rot_q + 2'd1);
+                end
+              end
+
+              CW_ISSUE: begin
+                if (cw_aw_fire) cw_aw_done_q <= 1'b1;
+                if (cw_w_fire)  cw_wcnt_q    <= cw_wcnt_q + 8'd1;
+                if (cw_aw_ok && cw_w_ok) cw_state_q <= CW_RESP;
+              end
+
+              CW_RESP: begin
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
+        // ---- capture-only response ready ---------------------------------
+        cpu_req.b_ready  = cw_b_accept;
+        cpu_req.r_ready  = cpu_r_ready;
+    end
+    assign slv_reqs[0] = cpu_req;
@@ -844,4 +1124,16 @@
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
```

---

## 3. `tests/nonblock_actual_dma_test.cpp` 与 `tests/test_nonblock_actual_dma.py` 的差异

```diff
--- a/tests/nonblock_actual_dma_test.cpp
+++ b/tests/nonblock_actual_dma_test.cpp
@@ -123,4 +123,31 @@
 static uint64_t page_old(uint32_t page, uint32_t i) {
     return 0xD000000000000000ULL ^ ((uint64_t)(page + 1) << 40) ^ (uint64_t)i;
 }
+
+#ifdef ACTUAL_DMA_SYSTEM
+// ---------------------------------------------------------------------------
+// Synthetic-CPU write / AMO scoreboard constants.  These MUST match the
+// test-only RTL constants in tests/nonblock_actual_dma_system_top.sv
+// (CPU_WR_ADDR / CPU_AMO_ADDR / CPU_AMO_ADD / CPU_WR_DAT0 / CPU_WR_DAT1 /
+//  CPU_WR_STR0 / CPU_WR_STR1 / CPU_WR_INIT1 / CPU_AMO_INIT).
+// Both sides are test code.
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
 
 struct TestFailure : std::runtime_error {
@@ -215,3 +242,18 @@
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
+    // AMO old-value accumulation scoreboard (bounded: engine runs until
+    // cpu_run drops; the C++ counter is 64-bit and unbounded by design).
+    uint64_t cpu_amo_expected  = 0;
+    uint64_t n_cpu_amo_obs     = 0;
 #endif
@@ -324,3 +366,12 @@ @@
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
@@ -556,3 +607,23 @@
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
@@ -776,3 +847,10 @@
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
@@ -795,5 +873,5 @@
 #ifdef ACTUAL_DMA_SYSTEM
-            const bool cpu_q = top->dbg_cpu_quiet;
+            const bool cpu_q = top->dbg_cpu_quiet && top->dbg_cpu_wr_quiet;
 #else
             const bool cpu_q = true;
 #endif
@@ -804,5 +882,12 @@
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
         }
@@ -885,3 +970,13 @@
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
@@ -1261,4 +1356,9 @@
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
@@ -1327,10 +1427,56 @@
         // Synthetic CPU must actually run and drain (no false PASS by skipping
         // the new branch).
         CHECK(top->dbg_cpu_completed > 0, "synthetic CPU completed no reads");
+        CHECK(top->dbg_cpu_issued == top->dbg_cpu_completed,
+              "synthetic CPU issued/completed read mismatch at terminal");
         CHECK(top->dbg_cpu_quiet, "synthetic CPU not quiet at terminal");
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
+            // issued == completed: every offered AW/W/B/R really retired.
+            CHECK(top->dbg_cpu_wr_issued  == top->dbg_cpu_wr_completed,
+                  "synthetic CPU issued/completed write mismatch at terminal");
+            CHECK(top->dbg_cpu_amo_issued == top->dbg_cpu_amo_completed,
+                  "synthetic CPU issued/completed AMO mismatch at terminal");
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
@@ -1385,3 +1531,6 @@
     uint64_t    a_target = 1048576ULL;
+#ifdef ACTUAL_DMA_SYSTEM
+    bool        cpu_writes = false;
+#endif
 
     for (int i = 1; i < argc; ++i) {
@@ -1399,3 +1549,6 @@
         if (const char* v = val("ptw-latency")) ptw_lat = (uint32_t)std::strtoul(v, nullptr, 0);
         if (const char* v = val("a-target"))  a_target = std::strtoull(v, nullptr, 0);
+#ifdef ACTUAL_DMA_SYSTEM
+        if (const char* v = val("cpu-writes")) cpu_writes = (std::strtoul(v, nullptr, 0) != 0);
+#endif
     }
@@ -1432,3 +1585,6 @@
                 a_pages, v_gap, transfers, warmup, ptw_lat,
                 (unsigned long long)a_target);
     std::printf("NOTE: ptw latency is a plain model delay (DDL/DDR), NOT MIG-matched.\n");
+#ifdef ACTUAL_DMA_SYSTEM
+    std::printf("nonblock_actual_dma_test: cpu_writes=%d\n", (int)cpu_writes);
+#endif
 
     int rc = 0;
@@ -1442,3 +1598,6 @@
         tb.ptw_latency     = ptw_lat;
         tb.a_target        = a_target;
+#ifdef ACTUAL_DMA_SYSTEM
+        tb.cpu_writes      = cpu_writes;
+#endif
         tb.policy_partition= false;
```

```diff
--- a/tests/test_nonblock_actual_dma.py
+++ b/tests/test_nonblock_actual_dma.py
@@ -228,5 +228,12 @@
     ap.add_argument("--system-atomics", action="store_true",
                     help="system top with board production DDR atomics adapter; independent build")
+    ap.add_argument("--cpu-writes", action="store_true",
+                    help="--system-atomics build plus the optional synthetic "
+                         "CPU plain-write / AMO stimulus (CpuWrites=1); requires "
+                         "--system-atomics and uses the independent object "
+                         "directory out/nonblock-actual-dma/obj-system-atomics-cpu-writes")
     opts = ap.parse_args()
     if opts.system_atomics:
         opts.system = True
+    if opts.cpu_writes and not opts.system_atomics:
+        ap.error("--cpu-writes requires --system-atomics")
@@ -248,3 +255,6 @@
     if opts.system_atomics:
         obj_dir = OUT / "obj-system-atomics"
         defines += ["-GDdrAtomics=1"]
+    if opts.cpu_writes:
+        obj_dir = OUT / "obj-system-atomics-cpu-writes"
+        defines += ["-GCpuWrites=1"]
@@ -286,2 +296,3 @@
-    build_log = OUT / ("build-system-atomics.log" if opts.system_atomics else
-                       "build-system.log" if opts.system else "build.log")
+    build_log = OUT / ("build-system-atomics-cpu-writes.log" if opts.cpu_writes else
+                       "build-system-atomics.log" if opts.system_atomics else
+                       "build-system.log" if opts.system else "build.log")
@@ -310,6 +321,10 @@
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

## 4. 文档更正（文本，不修改 `.md` — 不在 3 文件范围内）

**替换** `out/composition-fix-20261005/deepseek-cpu-write-test.md` 修订版中的未解决问题 1（以及任何回显的“删除 `2'd0`”/“删除 W 优先”注释）：

> 1. **W-before-AW 与 pulp 互连。** W 优先（W 在 AW 之前）是**合法 AXI**，并被有意保留在轮转中；一个从 `AWVALID` 依赖 `WREADY`/W 握手推导出来的早期版本是**非法且会自我死锁**的，已被移除。当前的 AW/W offer 是独立设置时序的，精度为周期级，不使用反压。如果互连确实无法接受 AW 之前的 W，那是一个**真实发现**，应在 fabric 中调查——**不得**通过删除 W 优先 skew 来掩盖。这不是板级复现，未进行绝对时序声明，**也无任何板级测量已被复现**。
>
> 不要*为了从零验证基线行为而无条件重新运行整个通过的基线构建家族*；仅重新构建/运行新增的 CPU 写测试配置。

同时，将“关键设计决策 #2”中旧的“`cw_aw_allow` 等待所有 W 握手”措辞替换为上述固定延迟描述。

---

## 5. 假设与未解决问题（供 Codex 验证）

1. **`CPU_WR_MAX = 1048576` 的余量。** 我估计一个 N4096 运行在 `v_gap=1000, ptw_latency=31` 时会发出约 10^5–10^6 个 CPU 写/AMO 事务（批量时间 ÷ 每事务周期数），所以 1048576 边界*可能*在 `ptw_latency=100` 时被触及。如果 `dbg_cpu_wr_issued + dbg_cpu_amo_issued` 接近上限，请在保持 32 位宽的前提下提高 `CPU_WR_MAX`。这正是“不得在正常 N4096 内提前跑完”的担忧。
2. **从设备在 `b_ready`/`r_ready` 为低时保持 B/R** — 这是标准要求，也是此设计能够无丢失的原因。
3. **`axi_riscv_atomics` RMW 顺序** — 引擎接受 B/R 以任意顺序或同一周期到达，并且在 FSM 到达 `CW_RESP` 之前捕获 R（如果共享的 `cpu_r_ready` 恰好在该周期为高）。尚未执行验证。
4. **ID 回显** — 写引擎要求返回的 B ID == `cw_aw_id`（1 或 2），AMO R ID == 1。如果适配器/交叉开关改变 ID，会故意停顿并通过看门狗失败。
5. **未运行任何测试。** 我没有执行 Verilator；上述内容不主张任何 PASS/FAIL。

---

## 6. 验收命令（仅新增配置）

```bash
# 强制执行新的 CPU 写路径达到 N4096 的后半部分（第二个半部分必须保持写覆盖）
python3 tests/test_nonblock_actual_dma.py --system-atomics --cpu-writes --mode n --transfers 4096

# 预期产物：
#   out/nonblock-actual-dma/obj-system-atomics-cpu-writes/Vnonblock_actual_dma_system_top
#   out/nonblock-actual-dma/build-system-atomics-cpu-writes.log
# 预期新增输出（软件报告，非板级测量）：
#   SYSTEM cpu-wr wr_issued=<n> wr_completed=<n> amo_issued=<n> amo_completed=<n>
#               amo_obs=<n> rd_err=0 wr_err=0 amo_err=0 wr_mem=... amo_mem=...
#   nonblock_actual_dma_test: PASS
```

根据要求，这里不包含针对未修改基线构建的重新运行命令。在实际 N4096 运行中，`wr_issued == wr_completed`、`amo_issued == amo_completed` 以及 `dbg_cpu_issued == dbg_cpu_completed` 应全部成立，且 `wr_issued`/`amo_issued` 必须远低于 `CPU_WR_MAX`（即上限未被触及，因此后半部分仍保持写覆盖）。