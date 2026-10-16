以下为只读 instrumentation 的单一完整 unified diff。假定 Codex 的 frontier 补丁已经在 `tests/nonblock_completion_pool_late.sv` 中提供 `logic [1:0] can_frontier`（以及 `FrontierEnable`）；本 diff **不**实现 predicate、不修改 grant/admission/stream_safe。若没有 Codex 的 predicate，本 diff 会因 `can_frontier` 未声明而无法编译，这是已知未决依赖。

测量窗口边界与快照点：
- 窗口打开：`tests/nonblock_actual_dma_test.cpp:810-817`，第一个硬件 V issue 前沿。
- 窗口关闭：`tests/nonblock_actual_dma_test.cpp:971-975`，第 `v_transfers` 个硬件 V done 前沿。
- 窗口计数语义：`tests/nonblock_actual_dma_test.cpp:804-819`，计 `(open, close]` 前沿周期。
- `frontier_wait` window delta 快照：start 在 open 后第一个被计数周期取 `dbg_frontier_wait`，即 posedge open 之后、posedge open+1 之前；end 在 close 周期 posedge 之后立即取。因此 delta 覆盖 `(open, close]` 对应 posedge 区间。raw terminal 为累计值，包含 warm-up，不是窗口值。
- 不断言 `frontier_wait > 0`；合法 0 不报错。

WLAST 计数复位证据：
- `tests/nonblock_completion_pool_late.sv:278-279`：reset 时 `vw_done_q/pw_done_q` 清零。
- `tests/nonblock_completion_pool_late.sv:344-356`：`job_fire_i` 时清零 `vw_done_q/pw_done_q`。
- `tests/nonblock_completion_pool_late.sv:340-343`：`job_done_i` 只清 `job_active_q` 并检查 metadata/armed，**不**清 WLAST 计数。
- `tests/nonblock_actual_dma_top.sv:591-592`：`job_done_i` 来自 DMA `idma_rsp_valid && idma_rsp_ready`。因此下一 `job_fire_i` 可以在外部 completion B 被观察前复位旧 virtual/physical WLAST 计数；pool 本身不跟踪 B。

```diff
diff --git a/tests/nonblock_completion_pool_late.sv b/tests/nonblock_completion_pool_late.sv
--- a/tests/nonblock_completion_pool_late.sv
+++ b/tests/nonblock_completion_pool_late.sv
@@ -35,7 +35,8 @@
     output logic [1:0] completion_ar_admit_o,
     output logic [31:0] occupancy_o, reserved_o, busy_o, fault_o,
     output logic [31:0] grant_pool_o, grant_stream_o,
     output logic [31:0] received_o, replayed_o, highwater_o,
     output logic [31:0] meta_accepted_o, meta_early_o, meta_highwater_o, queued_o,
-    output logic [31:0] grant_future_o, aw_fixture_stall_o
+    output logic [31:0] grant_future_o, aw_fixture_stall_o,
+    output logic [31:0] frontier_wait_o
 );
@@ -260,7 +260,8 @@
             meta_early_o <= 0;
             meta_highwater_o <= 0;
             grant_future_o <= 0;
             aw_fixture_stall_o <= 0;
+            frontier_wait_o <= 0;
             aw_released_q <= 0; aw_wait_q <= 0;
             source_read_release_q <= 0;
             v_rd_q <= 0; v_wr_q <= 0; p_rd_q <= 0; p_wr_q <= 0;
@@ -294,7 +294,12 @@
             aw_fixture_stall_o <= aw_fixture_stall_o +
                 32'(aw_fixture_block[0] && s_req_i[0].aw_valid) +
                 32'(aw_fixture_block[1] && s_req_i[1].aw_valid);
             grant_future_o <= grant_future_o + 32'(grant_future[0]) + 32'(grant_future[1]);
+            frontier_wait_o <= frontier_wait_o +
+                32'(can_frontier[0] && !can_stream[0] && !can_future[0] &&
+                     completion_ar_valid_i[0] && req_seq_q[0] > launch_seq_q[0] && !armed_q[0]) +
+                32'(can_frontier[1] && !can_stream[1] && !can_future[1] &&
+                     completion_ar_valid_i[1] && req_seq_q[1] > launch_seq_q[1] && !armed_q[1]);
             if (Mode == 6 || Mode == 7) begin
                 v_count_q <= v_count_q + 4'(aw_offer_i) - 4'(v_wlast_i);
                 p_count_q <= p_count_q + 4'(p_aw_offer_i) - 4'(p_wlast_i);
diff --git a/tests/nonblock_actual_dma_top.sv b/tests/nonblock_actual_dma_top.sv
--- a/tests/nonblock_actual_dma_top.sv
+++ b/tests/nonblock_actual_dma_top.sv
@@ -208,5 +208,6 @@
     output logic [31:0] dbg_e2_highwater,
     output logic [31:0] dbg_e2_grant_future,
+    output logic [31:0] dbg_frontier_wait,
     output logic [31:0] dbg_e2_aw_fixture_stall, dbg_e2_tc_read_wait, dbg_e2_tc_aw_pending, dbg_e2_tc_state,
     output logic [31:0] dbg_e2_tc_spill, dbg_e2_tc_ar_head,
     output logic [31:0] dbg_e2_tc_queue_ids,
@@ -565,7 +565,8 @@
         assign late_comp_ar_admit = '0;
         assign dbg_e2_aw_fixture_stall = 0;
         assign dbg_e2_grant_future = 0;
+        assign dbg_frontier_wait = 0;
         assign dbg_e2_meta_accepted = 0;
         assign dbg_e2_meta_early = 0;
         assign dbg_e2_meta_highwater = 0;
         assign dbg_e2_queued = 0;
@@ -573,7 +573,8 @@
         assign late_comp_ar_admit = '0;
         assign dbg_e2_aw_fixture_stall = 0;
         assign dbg_e2_grant_future = 0;
+        assign dbg_frontier_wait = 0;
         assign dbg_e2_meta_accepted = 0;
         assign dbg_e2_meta_early = 0;
         assign dbg_e2_meta_highwater = 0;
         assign dbg_e2_queued = 0;
@@ -661,6 +661,7 @@
             .grant_future_o(dbg_e2_grant_future), .aw_fixture_stall_o(dbg_e2_aw_fixture_stall),
             .meta_accepted_o(dbg_e2_meta_accepted), .meta_early_o(dbg_e2_meta_early),
-            .meta_highwater_o(dbg_e2_meta_highwater), .queued_o(dbg_e2_queued)
+            .meta_highwater_o(dbg_e2_meta_highwater), .queued_o(dbg_e2_queued),
+            .frontier_wait_o(dbg_frontier_wait)
         );
     end
     // slv_resps[2]/[3] intentionally open (tied-off slaves never response).
diff --git a/tests/nonblock_actual_dma_system_top.sv b/tests/nonblock_actual_dma_system_top.sv
--- a/tests/nonblock_actual_dma_system_top.sv
+++ b/tests/nonblock_actual_dma_system_top.sv
@@ -198,6 +198,7 @@
     output logic [31:0] dbg_e2_highwater,
 
     output logic [31:0] dbg_e2_grant_future,
+    output logic [31:0] dbg_frontier_wait,
     output logic [31:0] dbg_e2_aw_fixture_stall, dbg_e2_tc_read_wait, dbg_e2_tc_aw_pending, dbg_e2_tc_state,
     output logic [31:0] dbg_e2_tc_spill, dbg_e2_tc_ar_head,
     output logic [31:0] dbg_e2_tc_queue_ids,
@@ -1124,7 +1124,8 @@
         .dbg_e2_replayed (dbg_e2_replayed),
         .dbg_e2_highwater (dbg_e2_highwater),
         .dbg_e2_grant_future (dbg_e2_grant_future),
+        .dbg_frontier_wait (dbg_frontier_wait),
         .dbg_e2_aw_fixture_stall(dbg_e2_aw_fixture_stall),
         .dbg_e2_tc_read_wait(dbg_e2_tc_read_wait), .dbg_e2_tc_aw_pending(dbg_e2_tc_aw_pending),
         .dbg_e2_tc_state(dbg_e2_tc_state),
diff --git a/tests/nonblock_actual_dma_test.cpp b/tests/nonblock_actual_dma_test.cpp
--- a/tests/nonblock_actual_dma_test.cpp
+++ b/tests/nonblock_actual_dma_test.cpp
@@ -362,4 +362,8 @@
     uint64_t w_fast_bank[2]  = {0, 0};
     uint64_t w_iotlb_lookup  = 0, w_iotlb_hit = 0;
     uint64_t w_ptw_active    = 0, w_ptw_r_fire = 0;
+    uint64_t w_frontier_wait_start = 0;
+    uint64_t w_frontier_wait_end   = 0;
+    bool     w_frontier_wait_start_seen = false;
+    bool     win_closing = false;
 #ifdef ACTUAL_DMA_SYSTEM
     uint64_t w_ddr_ar = 0, w_ddr_aw = 0, w_ddr_w = 0, w_ddr_r = 0, w_ddr_b = 0;
@@ -1235,7 +1235,11 @@
         w_iotlb_lookup  = 0;
         w_iotlb_hit     = 0;
         w_ptw_active    = 0;
         w_ptw_r_fire    = 0;
+        w_frontier_wait_start = 0;
+        w_frontier_wait_end = 0;
+        w_frontier_wait_start_seen = false;
+        win_closing = false;
         v_done_last = a_done_last = 0;
         warm_a_page         = 0;
         warm_v_submitted    = 0;
@@ -809,11 +809,12 @@
         // includes A's first issue even when it precedes the window opening.
         const uint32_t measure_issue = (uint32_t)top->dbg_dma_issue & 3u;
         const uint32_t measure_done = (uint32_t)top->dbg_dma_done & 3u;
         const bool opening = win_armed && !win_seen && (measure_issue & 1u);
         if (opening) {
             win_open = true;
             win_seen = true;
             win_open_cycle = g_cycle;
+            w_frontier_wait_start_seen = false;
         }
         if (win_open && !opening) {
             ++w_cycles;
@@ -818,7 +818,11 @@
         if (win_open && !opening) {
             ++w_cycles;
 
+            if (!w_frontier_wait_start_seen) {
+                w_frontier_wait_start = (uint64_t)top->dbg_frontier_wait;
+                w_frontier_wait_start_seen = true;
+            }
+
 #ifdef ACTUAL_DMA_SYSTEM
             // Read-admission proxy: accumulate the PRE-EDGE event pulses
             // directly.  No open/close cumulative differences are used.
             if (top->dbg_ra_new_captured)      ++w_ra_captures;
@@ -971,6 +971,7 @@
             if (w_v_done == v_transfers) {
                 win_open = false;
                 win_armed = false;
                 win_close_cycle = g_cycle;
+                win_closing = true;
             }
         }
@@ -1113,6 +1113,10 @@
 #endif
 
+        if (win_closing) {
+            w_frontier_wait_end = (uint64_t)top->dbg_frontier_wait;
+            win_closing = false;
+        }
+
         top->clk_i = 0;
         top->eval();
         ++g_cycle;
@@ -2284,5 +2284,23 @@
                     (unsigned)e2_meta_accepted, (unsigned)e2_meta_early,
                     (unsigned)e2_meta_highwater, (unsigned)e2_queued);
 
+        const uint64_t frontier_wait_raw = (uint64_t)top->dbg_frontier_wait;
+        const uint64_t frontier_wait_window =
+            (w_frontier_wait_end >= w_frontier_wait_start) ?
+            (w_frontier_wait_end - w_frontier_wait_start) : 0;
+        std::printf("FRONTIER-WAIT-TERMINAL-JSON {"
+                    "\"schema\":\"frontier-wait-terminal-v1\","
+                    "\"scenario\":\"%s\","
+                    "\"raw_frontier_wait\":%llu,"
+                    "\"window_frontier_wait\":%llu,"
+                    "\"window_start_snapshot\":%llu,"
+                    "\"window_end_snapshot\":%llu,"
+                    "\"window_definition\":\"hardware_first_V_issue_to_final_V_done_(open,close]_pre-edge_cycles\","
+                    "\"scope\":\"raw_cumulative_including_warm_not_window; window_delta_is_window_only\"}\n",
+                    name,
+                    (unsigned long long)frontier_wait_raw,
+                    (unsigned long long)frontier_wait_window,
+                    (unsigned long long)w_frontier_wait_start,
+                    (unsigned long long)w_frontier_wait_end);
+
     }
 };
```

未包含 dump_stall 的额外诊断行；若 Codex 需要，可只在 `dump_stall()` 加只读打印，不影响本 diff。未执行任何测试，也不声称测试通过。
