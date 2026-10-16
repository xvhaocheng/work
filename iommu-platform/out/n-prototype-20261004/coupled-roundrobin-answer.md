下面给出**仅改两个测试文件**的完整统一 git diff，并给出导航、假设、验收检查、和“是否与给定 `ariane_xilinx` 上游 fabric 一致”的判断。无 RTL 改动、无新 SV adapter、默认 `vv` 语义保留。

## 1. 设计要点

- `+order=rr` 新增：
  - AR 和 AW 各自维护独立轮询游标：`rr_ar_next`、`rr_aw_next`。
  - 每次**真实 DMA** 的 `f_up_ar` / `f_up_aw` 握手后，对应游标切到另一 DMA。
  - warm 专用 AR（owner 2）不更新任何游标。
  - 选择仍在 `ar_lock < 0` / `aw_lock < 0` 时进行；一旦锁定，`ar_lock`、`aw_lock`、`ar_half_sel`、`aw_half_sel` 和 VALID 载荷保持到握手。
  - 每 DMA 内部仍按 half 升序扫描（`h = 0..1`）。
- runner 增加 `--order {vv,aa,av,rr}`，默认 `vv`，统一追加 `+order=<...>` 给现有可执行文件。
- 新增测试建议：
  - RR overlap 一次跑 12 个 scenario：
    - `python3 tests/test_nonblock_coupled.py --overlap --order rr`
  - legacy 对照：
    - `python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr`
- 默认预算不变：`WATCHDOG_CYCLES=4096`、`SCENARIO_BUDGET=4000000`、`WARM_BUDGET=200000`；runner 默认仍是 board OT8/W32，`--reduced` 才切 OT4/W8。

## 2. 完整 unified git diff

```diff
diff --git a/tests/nonblock_coupled_test.cpp b/tests/nonblock_coupled_test.cpp
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -223,7 +223,14 @@
     // Upstream request-priority order for the two DMA clients (see +order= in
     // main()): 0 = V (dma[0]) first, 1 = A (dma[1]) first.
     // Defaults select the matching-source order (both V-first).
     int order_ar_prefer = 0;
     int order_aw_prefer = 0;
 
+    // +order=rr: independent AR/AW round-robin cursors.  They advance only on
+    // a real DMA AR/AW handshake (cur_*_d >= 0); a warm-up AR (owner 2) must
+    // never perturb them.
+    bool rr_order = false;
+    int  rr_ar_next = 0;            // next preferred DMA for AR (0=V, 1=A)
+    int  rr_aw_next = 0;            // next preferred DMA for AW (0=V, 1=A)
+
     // Overlap mode: honor both halves of one 4 KiB job concurrently.
@@ -363,25 +363,18 @@
         if (ar_lock < 0) {
             int ar_d = -1, ar_h = -1;
-            if (order_ar_prefer == 0) {
-                for (int d = 0; d < 2 && ar_d < 0; ++d) {
-                    if (!dma[d].used || dma[d].job_done) continue;
-                    for (int h = 0; h < 2; ++h) {
-                        if (dma[d].ar_want[h] && !dma[d].ar_done[h]) {
-                            ar_d = d; ar_h = h; break;
-                        }
-                    }
-                }
-            } else {
-                for (int d = 1; d >= 0 && ar_d < 0; --d) {
-                    if (!dma[d].used || dma[d].job_done) continue;
-                    for (int h = 0; h < 2; ++h) {
-                        if (dma[d].ar_want[h] && !dma[d].ar_done[h]) {
-                            ar_d = d; ar_h = h; break;
-                        }
-                    }
-                }
-            }
+            // rr_order starts at rr_ar_next; fixed modes start at
+            // order_ar_prefer.  The scan is always per-DMA ascending half.
+            for (int k = 0; k < 2 && ar_d < 0; ++k) {
+                int d = rr_order ? ((rr_ar_next + k) & 1)
+                                 : ((order_ar_prefer + k) & 1);
+                if (!dma[d].used || dma[d].job_done) continue;
+                for (int h = 0; h < 2; ++h) {
+                    if (dma[d].ar_want[h] && !dma[d].ar_done[h]) {
+                        ar_d = d; ar_h = h; break;
+                    }
+                }
+            }
             if (ar_d < 0 && warm_active && !warm_sent) { ar_d = 2; ar_h = 0; }
             ar_lock     = ar_d;
             ar_half_sel = ar_h;
         }
@@ -422,24 +422,17 @@
         if (aw_lock < 0) {
             int aw_d = -1, aw_h = -1;
-            if (order_aw_prefer == 0) {
-                for (int d = 0; d < 2 && aw_d < 0; ++d) {
-                    if (!dma[d].used || dma[d].job_done) continue;
-                    for (int h = 0; h < 2; ++h) {
-                        if (dma[d].aw_want[h] && !dma[d].aw_done[h]) {
-                            aw_d = d; aw_h = h; break;
-                        }
-                    }
-                }
-            } else {
-                for (int d = 1; d >= 0 && aw_d < 0; --d) {
-                    if (!dma[d].used || dma[d].job_done) continue;
-                    for (int h = 0; h < 2; ++h) {
-                        if (dma[d].aw_want[h] && !dma[d].aw_done[h]) {
-                            aw_d = d; aw_h = h; break;
-                        }
-                    }
-                }
-            }
+            // rr_order starts at rr_aw_next; fixed modes start at
+            // order_aw_prefer.  The scan is always per-DMA ascending half.
+            for (int k = 0; k < 2 && aw_d < 0; ++k) {
+                int d = rr_order ? ((rr_aw_next + k) & 1)
+                                 : ((order_aw_prefer + k) & 1);
+                if (!dma[d].used || dma[d].job_done) continue;
+                for (int h = 0; h < 2; ++h) {
+                    if (dma[d].aw_want[h] && !dma[d].aw_done[h]) {
+                        aw_d = d; aw_h = h; break;
+                    }
+                }
+            }
             aw_lock     = aw_d;
             aw_half_sel = aw_h;
         }
@@ -678,11 +678,14 @@
         if (f_up_ar) {
             up_ar.push_back({g_cycle, s_tr_ar_id, s_tr_ar_addr, s_tr_ar_len});
             if (cur_ar_d >= 0) {
                 Dma& m = dma[cur_ar_d];
                 m.ar_want[ar_half_sel] = false;
                 m.ar_done[ar_half_sel] = true;
+                // Advance AR RR only for a real DMA beat.  A warm-up AR has
+                // cur_ar_d == -1 and must leave both RR cursors untouched.
+                if (rr_order) rr_ar_next = 1 - cur_ar_d;
             } else if (ar_lock == 2) {
                 warm_sent = true;
             }
             ar_lock = -1;
         }
@@ -698,10 +698,11 @@
                 Dma& m = dma[cur_aw_d];
                 e.pa = m.dst_pa + (uint64_t)aw_half_sel * 0x800;
                 m.aw_want[aw_half_sel] = false;
                 m.aw_done[aw_half_sel] = true;
                 aw_order.push_back(cur_aw_d);
                 if (w_owner < 0) w_owner = aw_order.front();
+                if (rr_order) rr_aw_next = 1 - cur_aw_d;
             } else {
                 // No local model (a DMA AW always has cur_aw_d >= 0); fall back
                 // to the wire address so the expected queue stays well-formed.
                 e.pa = s_tr_aw_addr;
@@ -1040,4 +1040,5 @@
         ar_lock = -1; aw_lock = -1; cur_ar_d = -1; cur_aw_d = -1;
+        rr_ar_next = 0; rr_aw_next = 0;
         mem_q.clear(); caw_q.clear(); cb_q.clear(); exp_aw_q.clear();
         up_ar.clear(); up_aw.clear(); up_w.clear(); up_r.clear(); up_b.clear();
         cmp_ar.clear(); cmp_aw.clear(); cmp_w.clear();
@@ -1178,27 +1178,34 @@
     // optional single-scenario selection: +scenario=<name>
-    // optional priority-order selection: +order=vv|aa|av  (default vv)
+    // optional priority-order selection: +order=vv|aa|av|rr  (default vv)
     // optional overlap mode:              --overlap
     //
     //   vv : AR prefers V, AW prefers V   (matching-source order, default)
     //   aa : AR prefers A, AW prefers A
     //   av : AR prefers V, AW prefers A   (legacy adversarial order)
+    //   rr : AR/AW round-robin independently; after each real DMA AR/AW
+    //        handshake the preference flips to the other DMA.  A warm-up AR
+    //        (owner 2) never changes either RR cursor.  This interleaves the
+    //        two halves of both 4 KiB jobs on the shared upstream port as a
+    //        legal AXI stream, e.g. V.h0 -> A.h0 -> V.h1 -> A.h1.
     std::string only;
     std::string order = "vv";
     bool overlap = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
         if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
         const std::string okey = "+order=";
         if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
         if (a == "--overlap") overlap = true;
     }
-    if (order != "vv" && order != "aa" && order != "av") {
+    if (order != "vv" && order != "aa" && order != "av" && order != "rr") {
         std::fprintf(stderr, "nonblock_coupled_test: unknown +order='%s' "
-                             "(expected vv|aa|av)\n", order.c_str());
+                             "(expected vv|aa|av|rr)\n", order.c_str());
         return 2;
     }
     int order_ar = 0, order_aw = 0;   // 0 = V first, 1 = A first
+    bool order_rr = false;
     if (order == "aa")      { order_ar = 1; order_aw = 1; }
     else if (order == "av") { order_ar = 0; order_aw = 1; }  // legacy adversarial
+    else if (order == "rr") { order_rr = true; }
     // "vv" (default) leaves both at V-first.
@@ -1205,3 +1205,8 @@
-    std::printf("nonblock_coupled_test: +order=%s (AR %s-first, AW %s-first)\n",
-                order.c_str(), order_ar ? "A" : "V", order_aw ? "A" : "V");
+    if (order_rr) {
+        std::printf("nonblock_coupled_test: +order=rr "
+                    "(AR round-robin, AW round-robin)\n");
+    } else {
+        std::printf("nonblock_coupled_test: +order=%s (AR %s-first, AW %s-first)\n",
+                    order.c_str(), order_ar ? "A" : "V", order_aw ? "A" : "V");
+    }
     std::printf("nonblock_coupled_test: overlap_mode=%d\n", (int)overlap);
@@ -1214,5 +1214,6 @@
             Tb tb;
             tb.order_ar_prefer = order_ar;
             tb.order_aw_prefer = order_aw;
+            tb.rr_order        = order_rr;
             tb.overlap_mode    = overlap;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
diff --git a/tests/test_nonblock_coupled.py b/tests/test_nonblock_coupled.py
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -5,4 +5,9 @@
 Builds tests/nonblock_coupled_test.cpp against exactly the same RTL source
 list and include directories as tests/test_nonblock_banked.py.  No RTL file is
 copied or duplicated: the source list is imported from the sibling runner.
+
+RR overlap sweep (all 12 scenarios in one run):
+    python3 tests/test_nonblock_coupled.py --overlap --order rr
+Legacy control:
+    python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr
 """
@@ -61,6 +61,16 @@
     ap.add_argument("--overlap", action="store_true",
                     help="enable the two half-burst overlap stimulus mode "
                          "(same RTL, opt-in).  Builds in a SEPARATE obj "
                          "directory so the default PASS build is never "
                          "overwritten.  Forwarded to the C++ harness as "
-                         "--overlap.")
+                         "--overlap.  The RR sweep is "
+                         "--overlap --order rr; add --legacy for the "
+                         "legacy control run.")
+    ap.add_argument("--order", choices=("vv", "aa", "av", "rr"), default="vv",
+                    help="upstream request priority order, forwarded to the "
+                         "C++ harness as +order=<...> (default: vv).  "
+                         "'rr' selects the independent AR/AW round-robin "
+                         "stimulus that interleaves the two DMAs' halves on "
+                         "the shared upstream port, e.g. V.h0 -> A.h0 -> "
+                         "V.h1 -> A.h1; combine with --overlap to cover all "
+                         "12 scenarios with that legal AXI ordering.")
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
     ap.add_argument("--scenario", default=None,
@@ -128,6 +128,6 @@
-    run_args = [str(exe)]
+    run_args = [str(exe), "+order={}".format(opts.order)]
     if opts.scenario:
         run_args.append("+scenario={}".format(opts.scenario))
     if opts.overlap:
```

## 3. 这种轮询是否与给定 `ariane_xilinx` 上游 fabric 一致？

**不能从给定文件确认；应明确表述为：`rr` 是合法 AXI 交错刺激，不是完整 fabric 模型。**

依据：

- `ariane_xilinx.sv:291-305` 的 `AXI_XBAR_CFG` 只给出 `MaxMstTrans=8`、`MaxSlvTrans=8`、`FallThrough=0`、`LatencyMode=CUT_ALL_PORTS`、`PipelineStages=1`、`UniqueIds=0` 等；**没有显式说明对两个 bank/DMA 请求流采用 round-robin 还是固定优先级**。
- `ariane_xilinx.sv:307-320` 实例化的是板级 `axi_xbar_intf`，连接 `slave[]` / `master[]`。这是板级 AXI 互连，不是测试里那个内部 translation-request 端口仲裁器。
- `ariane_xilinx.sv:810-877` 的 `ariane_peripherals` 才把 `dma_cfg`、`iommu_comp`、`iommu_ds`、`iommu_prog` 接进去（`:848-851`）；真正内部 banked CTL / iDMA backend / TL 请求仲裁并不在本文件可见。
- 给定 top 里 `IOMMUNonblockMode = 0`（`:172`），且 IOMMU DS delay 为 50/50（`:165-166`）。测试 harness 自己也声明没有 XBAR、没有 `rv_iommu_ds_if` arbitration、没有 board `axi_delayer`、没有 ID remap（`nonblock_coupled_test.cpp:51-53`）。
- 因此：
  - 若真实上游对 V/A 两个 bank 请求做 round-robin，则 `rr` 更接近实际交错顺序。
  - 若不是 round-robin，`rr` 仍然是**合法 AXI 顺序**（例如 `V.h0 -> A.h0 -> V.h1 -> A.h1`），只是不保证等于给定 fabric 的实际固定优先级行为。
  - 不要把该测试当作完整 fabric 建模。

## 4. 假设与未决点

- 假设 `f_up_ar` / `f_up_aw` 是“真实 DMA 握手”的判据；warm owner 2 在 `cur_ar_d == -1` 路径，不更新游标。
- 假设 `ar_lock` / `aw_lock` 的锁定语义不变；本 patch 只改 `lock < 0` 时的选择起点。
- 假设 `rr` 模式下 `order_ar_prefer` / `order_aw_prefer` 保持 0，但实际分支由 `rr_order` 选择，因此不依赖这两个值。
- 未改 RTL、未新增 SV adapter、未改默认 `vv` 语义。
- 未改预算常量；默认最大预算保持 `WATCHDOG_CYCLES=4096`、`SCENARIO_BUDGET=4000000`、`WARM_BUDGET=200000`。

## 5. 建议验收检查

1. RR overlap 全量：
   - `python3 tests/test_nonblock_coupled.py --overlap --order rr`
   - 期望看到 `+order=rr (AR round-robin, AW round-robin)`、`overlap_mode=1`、`PASS (12 scenarios)`。
2. Legacy 对照：
   - `python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr`
   - 用于对照 legacy build；若 legacy 关闭 nonblock，则它不验证 nonblock 仲裁，只作为控制组。
3. 默认语义回归：
   - `python3 tests/test_nonblock_coupled.py --overlap`
   - 应仍走 `+order=vv`，行为与改动前默认一致。
4. 固定顺序回归：
   - `--overlap --order vv`、`--overlap --order aa`、`--overlap --order av` 仍应可运行。
5. 非法 order：
   - runner 的 argparse 会拒绝；直接对可执行文件传 `+order=xx` 应返回 2。

## 6. 实际未执行声明

**本回复没有运行任何命令，没有执行 `verilator`、没有执行 Python runner、没有跑仿真，也没有验证本 patch 能编译或 `rr` 能通过。** 你给出的 “overlap vv banked 12/12 和 legacy 12/12 实际已通过” 是上下文事实，我未复现。上述 diff 与验收检查均为提案，需由 Codex/执行环境实际构建和运行确认。