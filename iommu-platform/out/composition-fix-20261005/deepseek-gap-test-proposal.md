# 结果

三处错误更正 + 三个测试文件的完整 unified diff + 运行命令 + 验收检查。**生产 RTL 不改**；本提案是**有界复现/回归**，不代表真实 SoC，也不代表完整 172 配置。

---

## A. 更正上一证据报告的三处错误

### A1 — F1：跨 bank 的 AW 不能假定可同时被 accept

`tests/nonblock_banked_top.sv` 里翻译请求进入 `rv_iommu_banked_ctl` 后经 `axi_demux_simple` 分流（源列表 `tests/test_nonblock_banked.py:42`）。AW 的接纳条件是 **`axi_demux_simple.sv:168-177`**：

```systemverilog
168: if (!aw_id_cnt_full && (w_open != {IdCounterWidth{1'b1}}) &&
...
175:   if (slv_req_i.aw_valid &&
176:         ((w_open == '0) || (w_select == slv_aw_select_i)) &&
177:         (!aw_select_occupied || (slv_aw_select_i == lookup_aw_select))) begin
```

- `w_select = (|w_open) ? w_select_q : slv_aw_select_i`（:252），且 `w_select_q` 在 `w_cnt_up` 时锁存（:251）。
- `w_open` 是"未关闭的 W burst"计数（`i_counter_open_w`, :236-249），只有 W 的 **last** 才 `w_cnt_down`（:435）。

**更正后的陈述**：只要 `w_open != 0`，**新 AW 必须与当前 `w_select_q` 同 bank**；第二个 bank 的 AW 只能在该 bank 的全部 W beat（含 last）排空、`w_open` 回 0 之后才可能被 accept。因此"两个 bank 的 AW 随便同时 accepted"不成立，F1 的任何复现/推导必须先满足 :175-177。C++ 侧的 `aw_order` / `w_owner`（`nonblock_coupled_test.cpp:211-212, 735-761`）正是这个语义：W 永远只从 `aw_order.front()` 出，"W owner is not at the AW head" 断言（:757-758）保证不跨 owner 交错。

### A2 — F2：`nonblock_hit.state_q` 与 `tl_wrap.state_q` 混淆

上一报告把 `rv_iommu_nonblock_hit` 的 `REJECT` 当成"主 walker 非 IDLE、`normal_gate` 关闭"的证据。

**更正后的陈述**：
- `normal_gate` 用的是 **`rv_iommu_tl_wrap.sv` 自己的 `state_q`**（:284-285；`state_q` 定义 :152，复位/更新 :961-964、:1616）：

```systemverilog
284: assign normal_gate = !NonblockEnable ||
285:                      (state_q == IDLE && !req_valid && !n_intercept);
286: assign trans_req_ready_o = n_intercept ? n_accept : (normal_trans_ready && normal_gate);
```

- nonblock 侧：`busy_o = state_q != IDLE && state_q != REJECT`（`rv_iommu_nonblock_hit.sv:49`），`intercept_o = start || busy_o`（:50）。所以 **处于 REJECT 时 `busy_o = 0`，且（无 `start` 时）`intercept_o = 0`**。
- 于是当主 FSM 回到 IDLE、`req_valid = 0` 时，`n_intercept = 0` → `normal_gate = 1` → `trans_req_ready_o` 可放行慢路径；REJECT 由 `normal_fire_i || !candidate_valid_i` 退出（:172-173）。

结论：**REJECT 不是"主 walker 非 IDLE"的证明**，也不阻塞慢路径；必须直接观察 `tl_wrap.state_q`（本 harness 只能通过 `dbg_tl_*`/`e3_*` 间接观察，:579-588、:775）。

### A3 — F3：`v_gap=1000` 的语义

**更正后的陈述**：`+v-gap-cycles=1000` 是**每个 V 的 4 KiB job 完成后、下一个 V job 发起前等待 1000 个完整 `tick()` 时钟周期**——不是源/目标 IOVA 的页距。每次 job 的 IOVA/PA 始终是 `V_SRC_IOVA=0x10000000` / `V_DST_IOVA=0x10001000` / `V_SRC_PA=0x80000000` / `V_DST_PA=0x80001000`（cpp:95-98），半段偏移固定 `+0x800`（cpp:438, 475）。它只作用于 DMA0(V)，**绝不施加到 A**。

> 已按你的说明记录：该卡死已在**新启动的 BIT 上单独用 PN / gap1000 / A8 / 长 4096 / warm 256 稳定重现**；不要再从旧 F1/F2/F3 推导任何生产 RTL 修改。

---

## B. 假设与设计决定（请 Codex 确认）

| # | 决定 | 说明 |
|---|---|---|
| B1 | 只改 `tests/nonblock_banked_top.sv`、`tests/nonblock_coupled_test.cpp`、`tests/test_nonblock_coupled.py` | 生产 RTL 零改动 |
| B2 | 顶层新增 parameter `PolicyEnable`，**默认 0**，转发给 `i_tl.PolicyEnable` | 默认 elaborate 与历史一致；`--policy` 才用 `-GPolicyEnable=1`。若你更想"顶层 TL 实例固定 PolicyEnable=1"，把 `.PolicyEnable (PolicyEnable)` 改成 `.PolicyEnable (1'b1)` 并把 python 的 `-GPolicyEnable` 去掉即可（一行） |
| B3 | `policy_did_i` 硬接 `24'd10` | 即受保护的 DID10（=V_DID，cpp:91） |
| B4 | `policy_clear_i = !rst_ni` | 用现有复位做安全初始化；复位期间无在途流量，等价于 IOTLB 复位清零（`rv_iommu_iotlb.sv:212-213, 387`）。**不新增 clear 脉冲接口** |
| B5 | plan 表**不改动** | 所给快照里是 `a1/a8`。若你树里已有 `a16` 行，本 diff 不触碰；若需要，见 D 节可选项 |
| B6 | watchdog 仍为 4096 | gap ≤ 4000 < 4096，即使 solo-V 也不会误触发；验收组合都带 A，A 持续握手 |
| B7 | `fullbuffer_jobs_i` 语义完全不动 | 不发明额外 buf 策略 |

---

## C. 完整 unified diff

### C1. `tests/nonblock_banked_top.sv`

```diff
--- a/tests/nonblock_banked_top.sv
+++ b/tests/nonblock_banked_top.sv
@@ -15,13 +15,23 @@
 #(
     parameter bit NonblockEnable = 1,
     parameter bit BankedEnable = 1,
     parameter int unsigned NumOutstandingTrans = 4,
-    parameter int unsigned WFifoDepth = 8
+    parameter int unsigned WFifoDepth = 8,
+    // Test-only elaboration switch for rv_iommu_tl_wrap.PolicyEnable.
+    // 0 (default) reproduces the historical elaboration exactly; the Python
+    // runner passes -GPolicyEnable=1 only for the opt-in --policy build.
+    parameter bit PolicyEnable = 0
 ) (
     input  logic        clk_i,
     input  logic        rst_ni,
 
+    // Test-only translation-policy input, forwarded verbatim to
+    // rv_iommu_tl_wrap.policy_partition_i.  It has an effect only in a build
+    // elaborated with PolicyEnable=1; 0 selects the full IOTLB replacement
+    // mask (identical replacement behaviour to PolicyEnable=0).
+    input  logic        policy_partition_i,
+
     // ---------------- upstream translation-request AXI : AW ------------------
     input  logic         tr_aw_valid,
     input  logic [3:0]   tr_aw_id,
     input  logic [63:0]  tr_aw_addr,
@@ -714,10 +724,11 @@
     rv_iommu_tl_wrap #(
         .RVIOMMUCfg         (Cfg),
         .E3Enable           (1'b0),
         .NonblockEnable     (NonblockEnable),
+        .PolicyEnable       (PolicyEnable),
         .addr_t             (addr_t),
         .paddr_t            (paddr_t),
         .dc_t               (rv_iommu::dc_ext_t),
         .ddtc_up_t          (rv_iommu::ddtc_up_ext_t),
         .trans_req_data_t   (trans_req_data_t),
@@ -774,9 +785,19 @@
         .e3_delay128_i          (1'b0),
         .e3_o                   (tl_e3),
         .research_o             (),
         .nonblock_enable_i      (nonblock_enable_i),
         .nonblock_fire_o        (nonblock_fire_o),
-        .nonblock_busy_o        (nonblock_busy_o)
+        .nonblock_busy_o        (nonblock_busy_o),
+
+        // Test-only policy plumbing.  policy_did_i is hard-wired to the
+        // protected victim DID (V_DID == 10 in the C++ harness).  policy_clear_i
+        // is driven from the existing reset, so the IOTLB can never be cleared
+        // while a translation is in flight (there is no traffic during reset).
+        .policy_partition_i     (policy_partition_i),
+        .policy_clear_i         (!rst_ni),
+        .policy_did_i           (24'd10),
+        .policy_quiet_o         (),
+        .nonblock_diag_o        ()
     );
 
 endmodule
```

### C2. `tests/nonblock_coupled_test.cpp`

```diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -256,11 +256,23 @@
     // Overlap mode: honor both halves of one 4 KiB job concurrently.
     bool overlap_mode = false;
 
     // v5 opt-in proxy: drive fullbuffer_jobs_i = 3u for the bounded 4 KiB
     // candidate.  Default false -> 0u (legacy guard).  With +couple-depth=512
     // the harness runs all 12 existing scenarios; each scenario has one 4 KiB
     // job per DMA, and the A-enabled scenarios therefore carry two jobs on the
     // two separate 512-deep coupling FIFOs.
     bool fullbuffer_permit = false;
 
+    // +v-gap-cycles=0|1000|4000: number of complete ticks the V victim waits
+    // after finishing one 4 KiB job before the next V job is started.  0 is the
+    // historical back-to-back issue.  A is never gated by this value: A's own
+    // start condition is unchanged and tick() keeps running during the wait.
+    // The wait is purely temporal; no IOVA/PA or page distance is changed.
+    uint32_t v_gap_cycles = 0;
+
+    // +policy=0|1: forwarded verbatim to the wrapper input policy_partition_i.
+    // Only observable in a build elaborated with PolicyEnable=1 (the Python
+    // --policy build); the default build forces the full replacement mask.
+    bool policy_partition = false;
+
     // Per-DMA selectors for the half whose AR / AW is currently on the wire,
     // and per-DMA pointers to the half of the next returning R / B / W beat.
     // They are all no-ops in serial mode (they stay 0 or 1).
@@ -573,7 +585,8 @@
     void tick() {
         top->nonblock_enable_i = nonblock_enable ? 1 : 0;
         top->fullbuffer_jobs_i = fullbuffer_permit ? 3u : 0u;
+        top->policy_partition_i = policy_partition ? 1 : 0;
 
         for (int d = 0; d < 2; ++d) dma_progress(d);
 
         // ---- drive all combinational inputs at clk low -----------------
@@ -1114,9 +1127,11 @@
     void run_scenario(const char* name, bool enable, uint32_t a_pages,
                       uint32_t ptw_lat) {
         std::printf("== coupled scenario %s (runtime enable=%d, a_pages=%u, "
-                    "ptw_latency=%u) ==\n", name, (int)enable, a_pages, ptw_lat);
+                    "ptw_latency=%u, v_gap_cycles=%u, policy_partition=%d) ==\n",
+                    name, (int)enable, a_pages, ptw_lat, v_gap_cycles,
+                    (int)policy_partition);
         setup_memory(a_pages);
         reset(enable);
         ptw_latency    = ptw_lat;
@@ -1132,17 +1147,33 @@
         uint32_t v_next = 0, a_next = 0;
+        // Optional V-only inter-job gap.  After a V 4 KiB job completes, the
+        // next V job is only started once v_gap_cycles complete ticks have
+        // elapsed.  A is never gated here: the A start below is unchanged and
+        // tick() keeps running, so A keeps progressing while V is idle.  The
+        // gap is purely temporal and never touches any IOVA/PA or page stride.
+        long v_ready_at = -1;
         start_job(0, v_next++);
         if (a_pages) start_job(1, a_next++);
 
         while (v_next < JOB_COUNT || (a_pages && a_next < JOB_COUNT)) {
             tick();
             if (g_cycle - last_progress_cycle > WATCHDOG_CYCLES) {
                 dump_stall(std::string("scenario ") + name);
                 throw TestFailure("watchdog: no handshake progress");
             }
-            if (dma[0].job_done && v_next < JOB_COUNT) start_job(0, v_next++);
+            if (dma[0].job_done && v_next < JOB_COUNT) {
+                if (v_gap_cycles == 0) {
+                    start_job(0, v_next++);
+                    v_ready_at = -1;
+                } else if (v_ready_at < 0) {
+                    v_ready_at = g_cycle + (long)v_gap_cycles;
+                } else if (g_cycle >= v_ready_at) {
+                    start_job(0, v_next++);
+                    v_ready_at = -1;
+                }
+            }
             if (a_pages && dma[1].job_done && a_next < JOB_COUNT) start_job(1, a_next++);
             if (g_cycle - scenario_start > SCENARIO_BUDGET) {
                 dump_stall(std::string("scenario ") + name);
                 throw TestFailure("scenario budget exceeded");
             }
         }
@@ -1199,12 +1230,14 @@
         std::printf("COUPLED-SUMMARY %s couple_depth=%u cycles=%ld comp_ar=%lu "
                     "ddtw_ar=%lu ptw_ar=%lu tl_accept=%lu tl_resp=%lu "
                     "iotlb_look=%lu iotlb_hit=%lu ptw_active=%lu r_beats=%lu b_up=%lu "
-                    "nonblock_fire=%lu nonblock_busy=%lu\n",
+                    "nonblock_fire=%lu nonblock_busy=%lu "
+                    "v_gap=%u policy=%d\n",
                     name, couple_depth, g_cycle - scenario_start,
                     (unsigned long)n_comp_ar, (unsigned long)n_ddtw_ar,
                     (unsigned long)n_ptw_ar, (unsigned long)n_tl_accept,
                     (unsigned long)n_tl_resp, (unsigned long)n_iotlb_lookup,
                     (unsigned long)n_iotlb_hit, (unsigned long)n_ptw_active,
                     (unsigned long)n_r_beats, (unsigned long)n_b_up,
                     (unsigned long)d_nonblock_fire,
-                    (unsigned long)d_nonblock_busy);
+                    (unsigned long)d_nonblock_busy,
+                    v_gap_cycles, (int)policy_partition);
@@ -1244,19 +1277,34 @@
     //        (owner 2) never changes either RR cursor.  This interleaves the
     //        two halves of both 4 KiB jobs on the shared upstream port as a
     //        legal AXI stream, e.g. V.h0 -> A.h0 -> V.h1 -> A.h1.
+    // optional V inter-job gap:           +v-gap-cycles=0|1000|4000 (default 0)
+    //   The V victim waits this many complete ticks between finishing one
+    //   4 KiB job and starting the next one.  The wait is counted by tick(),
+    //   so A keeps progressing; A is never gated by this value and no IOVA/PA
+    //   or page distance is changed.
+    // optional IOTLB partition policy:    +policy=0|1 (default 0)
+    //   Forwarded to the wrapper input policy_partition_i.  It is only
+    //   observable in a wrapper elaborated with -GPolicyEnable=1 (python
+    //   --policy), which is also the only reason the obj directory is split.
     std::string only;
     std::string order = "vv";
     std::string couple_depth_str = "3";
+    std::string v_gap_str = "0";
+    std::string policy_str = "0";
     bool overlap = false;
     bool fullbuffer_permit = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
         if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
         const std::string okey = "+order=";
         if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
         const std::string ckey = "+couple-depth=";
         if (a.rfind(ckey, 0) == 0) couple_depth_str = a.substr(ckey.size());
+        const std::string gkey = "+v-gap-cycles=";
+        if (a.rfind(gkey, 0) == 0) v_gap_str = a.substr(gkey.size());
+        const std::string pkey = "+policy=";
+        if (a.rfind(pkey, 0) == 0) policy_str = a.substr(pkey.size());
         if (a == "--overlap") overlap = true;
         if (a == "--fullbuffer-permit") fullbuffer_permit = true;
     }
@@ -1274,6 +1322,25 @@
     else {
         std::fprintf(stderr, "nonblock_coupled_test: invalid +couple-depth='%s' "
                              "(expected exactly 3 or 512)\n", couple_depth_str.c_str());
         return 2;
     }
+    // Strict gap/knob validation, same style as the depth check: exactly the
+    // known values, never a free-form integer.
+    uint32_t v_gap_cycles = 0;
+    if (v_gap_str == "0")         v_gap_cycles = 0;
+    else if (v_gap_str == "1000") v_gap_cycles = 1000;
+    else if (v_gap_str == "4000") v_gap_cycles = 4000;
+    else {
+        std::fprintf(stderr, "nonblock_coupled_test: invalid +v-gap-cycles='%s' "
+                             "(expected exactly 0, 1000 or 4000)\n", v_gap_str.c_str());
+        return 2;
+    }
+    bool policy_partition = false;
+    if (policy_str == "0")      policy_partition = false;
+    else if (policy_str == "1") policy_partition = true;
+    else {
+        std::fprintf(stderr, "nonblock_coupled_test: invalid +policy='%s' "
+                             "(expected exactly 0 or 1)\n", policy_str.c_str());
+        return 2;
+    }
     int order_ar = 0, order_aw = 0;   // 0 = V first, 1 = A first
@@ -1299,6 +1366,10 @@
     std::printf("nonblock_coupled_test: fullbuffer_permit=%d\n",
                 (int)fullbuffer_permit);
+    std::printf("nonblock_coupled_test: v_gap_cycles=%u\n", v_gap_cycles);
+    std::printf("nonblock_coupled_test: policy_partition=%d "
+                "(wrapper PolicyEnable is an elaboration flag; see --policy)\n",
+                (int)policy_partition);
 
     int ran = 0, failed = 0;
@@ -1306,9 +1377,11 @@
         try {
             Tb tb;
             tb.order_ar_prefer = order_ar;
             tb.order_aw_prefer = order_aw;
             tb.rr_order        = order_rr;
             tb.overlap_mode    = overlap;
             tb.couple_depth    = couple_depth;
             tb.fullbuffer_permit = fullbuffer_permit;
+            tb.v_gap_cycles      = v_gap_cycles;
+            tb.policy_partition  = policy_partition;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
```

### C3. `tests/test_nonblock_coupled.py`

```diff
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -15,7 +15,12 @@
 Legacy control (default coupling depth 3):
     python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr
 Legacy control, v2 depth:
     python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr --couple-depth 512
+
+Opt-in V inter-job gap and IOTLB partition policy (elaborated with
+PolicyEnable=1, built in a separate obj dir):
+    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
+        --fullbuffer-permit --v-gap-cycles 1000 --policy
 """
 
 import argparse
@@ -103,9 +108,25 @@
                          "the A-enabled scenarios exercise two jobs on the two "
                          "separate 512-deep coupling FIFOs. Default is 0 (legacy "
                          "guard).")
+    ap.add_argument("--v-gap-cycles", type=int, choices=(0, 1000, 4000), default=0,
+                    help="V inter-job gap: complete ticks the V victim waits "
+                         "between finishing one 4 KiB job and starting the next "
+                         "one, forwarded to the C++ harness as "
+                         "+v-gap-cycles=<...> (default: 0 = historical "
+                         "back-to-back issue).  The wait is counted in tick() and "
+                         "never gates A and never changes any IOVA/PA or page "
+                         "stride.")
+    ap.add_argument("--policy", action="store_true",
+                    help="opt-in IOTLB replacement partition.  Elaborates the "
+                         "test wrapper with -GPolicyEnable=1 (a compile "
+                         "capability change, hence a SEPARATE obj directory so "
+                         "the default artifact is never overwritten) and forwards "
+                         "+policy=1 to the C++ harness, which drives the wrapper "
+                         "input policy_partition_i=1.  Protected DID is 10. "
+                         "Requires the banked nonblocking build (not --legacy).")
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
     ap.add_argument("--scenario", default=None,
@@ -113,19 +134,30 @@
     if opts.fullbuffer_permit and opts.couple_depth != 512:
         ap.error("--fullbuffer-permit requires --couple-depth 512")
+    if opts.policy and opts.legacy:
+        ap.error("--policy requires the banked nonblocking build "
+                 "(--legacy elaborates NonblockEnable=0, which the "
+                 "PolicyEnable composition check rejects)")
 
     sibling = load_sibling()
     tb = ROOT / "tests" / "nonblock_coupled_test.cpp"
     sources = dedup([require(p) for p in sibling.SOURCES])
 
     OUT.mkdir(parents=True, exist_ok=True)
     # Default mode keeps its original obj dir so an already-passing build is
     # never overwritten.  Only the opt-in overlap mode uses a sub-directory.
     # --couple-depth is a RUNTIME flag only: same RTL, same Verilator source
     # list, no new SV adapter, so it never appears in the obj dir.  A depth-3
     # run and a depth-512 run of the same mode therefore reuse one binary.
     obj_root = (OUT
                 / ("legacy" if opts.legacy else "banked")
                 / ("reduced" if opts.reduced else "board"))
     if opts.overlap:
         obj_root = obj_root / "overlap"
+    if opts.policy:
+        # PolicyEnable is an ELABORATION-time capability of the test wrapper
+        # (-GPolicyEnable).  Only this opt-in build changes it, so it gets its
+        # own object directory and can never overwrite the default
+        # (PolicyEnable=0) artifact.  --v-gap-cycles is runtime-only and
+        # therefore never appears here.
+        obj_root = obj_root / "policy"
     obj_dir = obj_root / "obj"
     obj_dir.mkdir(parents=True, exist_ok=True)
@@ -150,7 +182,9 @@
         "-GBankedEnable={}".format(banked),
         "-GNonblockEnable={}".format(nonblock),
         "-GNumOutstandingTrans={}".format(num_ot),
         "-GWFifoDepth={}".format(w_fifo),
+        # Elaboration-time only; see --policy above.
+        "-GPolicyEnable={}".format(1 if opts.policy else 0),
     ]
     for inc in sibling.INCDIRS:
         args += ["-I{}".format(inc)]
@@ -173,8 +207,11 @@
     run_args = [str(exe), "+order={}".format(opts.order),
                 "+couple-depth={}".format(opts.couple_depth)]
+    run_args.append("+v-gap-cycles={}".format(opts.v_gap_cycles))
+    run_args.append("+policy={}".format(1 if opts.policy else 0))
     if opts.scenario:
         run_args.append("+scenario={}".format(opts.scenario))
     if opts.overlap:
         run_args.append("--overlap")
     if opts.fullbuffer_permit:
         run_args.append("--fullbuffer-permit")
@@ -181,5 +218,6 @@
     print("test_nonblock_coupled: couple_depth={} order={} overlap={} "
-          "fullbuffer_permit={}".format(
+          "fullbuffer_permit={} v_gap_cycles={} policy={}".format(
               opts.couple_depth, opts.order, int(opts.overlap),
-              int(opts.fullbuffer_permit)))
+              int(opts.fullbuffer_permit), opts.v_gap_cycles,
+              int(opts.policy)))
     run = subprocess.run(run_args)
```

---

## D. 运行命令

默认回归（gap 0 / policy off，历史场景，obj 目录不变）：

```bash
python3 tests/test_nonblock_coupled.py --overlap --order rr
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512
```

有界复现 / 回归（单场景，保留 failed state 的 STALL dump）：

```bash
# N（Nonblock=1），A8，gap1000，RR overlap，depth512 + fullbufferpermit
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --v-gap-cycles 1000 --scenario mode2-a8-l1

# 同上，gap4000
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --v-gap-cycles 4000 --scenario mode2-a8-l1

# PN（Nonblock=1 + policy partition），DID10 受保护
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --v-gap-cycles 1000 --policy --scenario mode2-a8-l1

# l31 变体与 a16（若你的 plan 表里已有 a16 行；否则见 D.1）
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512 \
    --fullbuffer-permit --v-gap-cycles 1000 --policy --scenario mode2-a8-l31
```

obj 目录布局（验证隔离）：

```
out/nonblock-coupled/banked/board/overlap/obj                 # 默认 artifact（不动）
out/nonblock-coupled/banked/board/overlap/policy/obj          # --policy 专用
```

#### D.1 可选的 a16 行（**本 diff 未包含**，按需自行加入 `main()` 的 `plan[]`）

```cpp
{ "mode2-a16-l1",    true,  16,  1 },
{ "mode2-a16-l31",   true,  16, 31 },
```

（`setup_memory` 已支持 `a_pages=16`：L0A 写入 `2*16=32` 个 PTE，`0x70000 + n*8` 到 `0x700F8`，均在已建表范围内；`src_page = 1+16` 仍唯一。）

---

## E. 验收检查

1. **默认不回归**：`--overlap --order rr`（depth3 与 depth512）12 个场景全 PASS；`COUPLED-SUMMARY` 除新追加的 `v_gap=0 policy=0` 外逐字段与旧日志一致；`grep 'v_gap=0 policy=0'` 命中。
2. **新旋钮只在显式打开时生效**：`+policy=1` 在 `-GPolicyEnable=0` 的二进制里被 C++ 驱动但 TL 内部 `policy_mask_i` 仍为 `'1`（`rv_iommu_tl_wrap.sv:669-672`），行为应逐字节等于 `+policy=0`。
3. **gap 不阻塞 A**：带 A 的场景在 gap 期间 `COUPLED-SUMMARY` 中 `comp_ar/ddtw_ar/ptw_ar/r_beats/b_up` 持续增长；`cycles` 增加约 `(JOB_COUNT-1)*gap`（64 job → 63*1000 ≈ 63000 额外周期量级）。
4. **正确性断言不变**：V/A 源页未被改写、目标页逐 8B 等于 `page_word`（cpp:1164-1179）；`completion AW does not match upstream AW`（:821-822）与 `upstream R beat changed while valid`（:693-697）保持。
5. **失败保留**：卡死时 `dump_stall` 打印 `sharedR.head / DMA0/1 / caw_q / exp_aw_q / TL dbg_*`，并以 `SCENARIO-FAIL ... SCENARIO-BUDGET/watchdog` 结束，进程返回 1。用 `--scenario` 单跑以保留干净日志。
6. **policy 隔离生效**：跑 `--policy` 后 `ls out/nonblock-coupled/banked/board/overlap/policy/obj/Vnonblock_banked_top` 存在，且不存在的默认 obj 时间戳未被更新（旧 artifact 未被覆盖）。
7. **参数校验**：直接跑二进制 `+v-gap-cycles=999` → 退出码 2；`+policy=2` → 退出码 2；`--policy --legacy` → `ap.error`。
8. **机制 guard 保留**：`n_comp_ar/n_r_beats/n_b_up/n_tl_accept/n_tl_resp/n_ddtw_ar+n_ptw_ar > 0`（cpp:1182-1187）；未启用场景 `d_nonblock_fire == 0`（:1195-1198）。

---

## F. 未决 / 需 Codex 确认

- **B2**：`PolicyEnable` 顶层参数默认 0 vs 顶层 TL 实例固定 1。当前提案取前者（默认 artifact 不变，仅 `--policy` 走 `out/.../policy/obj`）。
- **B4**：未新增 explicit clear 脉冲接口；`policy_clear_i` 绑 `!rst_ni`。如果你确实需要运行期"静默窗口内 clear"，需要额外顶层 input + C++ 在 `check_quiet` 之后驱动，仍在允许改动范围内。
- **B5/D.1**：所给快照 `plan[]` 只有 `a1/a8`；`a16` 是否已在你树中存在？若不存在且验收确实要 A16，请确认是否允许把两行加入 `plan[]`（会改变默认 sweep 的场景数）。
- **gap 与 watchdog**：gap ≤ 4000 时 `4096` watchdog 不会被误触发，但该结论依赖 `WATCHDOG_CYCLES = 4096`（cpp:115）。若后续把 gap 提高到 ≥4096，需要显式把"计划等待"从 watchdog 里排除（本提案未做）。
- 我无法执行/写文件；以上均为提案，未声称任何测试已通过。