## 1. 交付内容

只改了两个测试文件 + spec 一个文档，统一 diff 见 §4。其他两文档的追加段以**草案文本**给出（§5），不直接 diff。

改动要点：

| 位置 | 改动 |
|---|---|
| `tests/nonblock_coupled_test.cpp` | 删除文件级 `COUPLE_DEPTH = 3`；新增 `Tb::couple_depth`（默认 3）；新增 `+couple-depth=3\|512` 严格校验（字符串精确匹配，非法值 `return 2`）；反压点与溢出 CHECK 共用该值；summary 打印 depth |
| `tests/test_nonblock_coupled.py` | 新增 `--couple-depth`（`type=int, choices=(3,512), default=3`），转发为 `+couple-depth=`；明确 obj 目录**不**随深度变化（同一 C++ 支持两深度，不新增 SV adapter）；`SOURCES`/`INCDIRS`/锁定与计数逻辑未动 |
| `docs/research-n-prototype-spec-20261004.md` | 顶部状态更新（v2 未仿真/未编译/未上板）；§5 容量条补 v2 每路数据核算；v1 段删除"总槽/数据深度不增加"这一陈旧断言；新增 `## v2更新：4 KiB 整缓冲（方案B）` |

未触碰：任何 `.sv`、source list、`BankedEnable/NonblockEnable/NumOutstandingTrans/WFifoDepth` 绑定、obj 目录选择、已有的 `--legacy/--overlap/--order/--scenario` 语义、默认深度 3。

## 2. 假设与约束

- `couple_depth` 是**运行时**参数，双方在同一 Tb 生命周期内不变；`start_job`/`check_quiet`/锁定/计数逻辑未改。
- 严格校验只在 C++ 侧做字符串精确比较（`"3"` / `"512"`），runner 侧用 argparse `choices` 双保险；`+couple-depth=0512`、`+couple-depth=4` 均返回 2。
- depth 512 下每路最多 256(R)×2 = 512 拍，CHECK 为 `<=`，因此 well-formed job 不会触发溢出（语义解释，未仿真验证）。
- 我没有运行任何命令；下文的验收命令是**待执行**的，不是结果。

## 3. 关键证据（file:line）

- `cva6/corev_apu/iDMA/src/systems/cva6/dma_core_wrap.sv:27` `BufferDepth = 32'h3, // Default = 3`；`:293` `.BufferDepth ( BufferDepth )`；`:349` `BUFFER_DEPTH = 32'h3, // Default = 3`；`:411` `.BufferDepth ( BUFFER_DEPTH )` —— 24 B/路 = 3×8 B 的来源。
- `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:146-150` `batch_cfg_ok` 要求 `dma_reg2hw.num_bytes.q == 4096`；`:151` `batch_task_issue = batch_active_o && state_q == COUNT && ready_i`；`:152` `batch_task_done = batch_active_o && state_q == WAIT && trans_complete_i` —— 工作量边界：只覆盖"正好 4096 B、一次一个 job、等 `trans_complete_i`"。
- `tests/nonblock_coupled_test.cpp`：`:92`（旧常量）、`:190`（注释引用）、`:531`（反压）、`:744`（溢出 CHECK）、`:1144-1152`（summary）、`:1188-1203`（arg 解析）、`:1224-1229`（Tb 赋值）。
- `tests/test_nonblock_coupled.py`：`:91`（`sibling.SOURCES`）、`:96-102`（obj 目录）、`:143-148`（run_args）。
- `scripts/mac_nonblock_pilot.py:103` `DEFAULT_CHUNK = 4096`；`:141-144` short/long 计划 —— 原版 15 项均为 3 拍缓冲下的数据。
- `docs/nonblock-dependency-diagnosis-20261004.md:83-106`（§3.2 A8 循环）、`:139-141`（§4.3 板级 A8 根因未知）。

## 4. 统一 diff

```diff
--- a/tests/nonblock_coupled_test.cpp
+++ b/tests/nonblock_coupled_test.cpp
@@ -23,7 +23,7 @@
 //   * the memory model keeps comp_r_valid / ddtw_r_valid / ptw_r_valid STABLE
 //     for the head beat regardless of downstream FIFO room.  Upstream R
 //     acceptance (tr_r_ready) is throttled by the bank of the RETURNED tr_r_id
-//     and the 3-beat coupling-FIFO room, evaluated after settling clk-low.
+//     and the coupling-FIFO room (Tb::couple_depth, 3 by default), evaluated after settling clk-low.
 //   * completion B is accepted exactly on sampled comp_b_valid && comp_b_ready
 //     and popped from cb_q.
 //   * all handshake payloads are snapshotted BEFORE the rising edge.
@@ -39,7 +39,7 @@
 // and RAWCouplingAvail = 0 (see cva6/corev_apu/iDMA/src/idma_backend.sv:472-473,
 // :554-557, :590-608, :612-625, :837-858).  The dependencies kept by the
 // harness in this mode are:
-//   * the same 3-entry R->W coupling FIFO shared by both halves of the DMA;
+//   * the same per-DMA R->W coupling FIFO (Tb::couple_depth) shared by both halves;
 //   * W beats strictly matched to the currently-issued AW;
 //   * both AWs of a job emitted in AW-issue order;
 //   * both B responses before the job is reported done / the next job issued;
@@ -48,6 +48,21 @@
 // The default mode is byte-for-byte identical to the pre-existing serial run,
 // so the existing PASS logs stay valid.
 //
+// Coupling-FIFO depth (opt-in, +couple-depth=3|512)
+// ------------------------------------------------
+// The per-DMA read->write coupling FIFO models the board RAWCouplingAvail = 0
+// buffer.  Depth 3 is the historical v1 default and is what every existing log
+// in this repository was produced with, so it is never changed implicitly.
+// Depth 512 is the v2 "full 4 KiB job" configuration: both 2 KiB halves of one
+// 4 KiB copy (2 x 256 R beats) fit in the lane buffer, so a returned R beat is
+// not held off just because the other half still has W beats to drain.  The
+// depth is chosen solely by this C++ flag, forwarded by
+// tests/test_nonblock_coupled.py --couple-depth; the RTL, the Verilator source
+// list and the obj directory are unchanged.  The SAME value feeds both the
+// upstream tr_r_ready back-pressure (settle_upstream_r_ready) and the
+// coupling-FIFO overflow CHECK (the f_up_r update), so a run can never
+// back-pressure at one depth and check another.
+//
 // This is a test harness model.  It is NOT DDR and NOT the real iDMA backend:
 // it has no XBAR, no rv_iommu_ds_if arbitration, no board axi_delayer and no
 // ID remap.  It only reproduces the ordering/back-pressure shape of the board.
@@ -89,7 +104,8 @@
 static const uint32_t PAGE_BEATS   = 512;   // 4 KiB / 8 B
 static const uint32_t BURST_BEATS  = 256;   // 2 KiB / 8 B
 static const uint32_t BURST_LEN    = 255;   // -> 256 beats
-static const uint32_t COUPLE_DEPTH = 3;     // per-DMA read->write FIFO
+// The per-DMA read->write coupling-FIFO depth is no longer a file constant:
+// it is Tb::couple_depth (default 3), set from +couple-depth=3|512 in main().
 static const uint32_t WARM_ROUNDS  = 16;
 static const uint32_t JOB_COUNT    = 64;
 
@@ -187,7 +203,7 @@
         uint32_t wx     [2] = {0, 0};   // W beats emitted this half
         bool     b_done [2] = {false, false};
 
-        std::deque<Beat> cfifo;     // read->write coupling FIFO (<= COUPLE_DEPTH)
+        std::deque<Beat> cfifo;     // read->write coupling FIFO (<= Tb::couple_depth)
                                     // shared across both halves of one DMA
     };
     Dma dma[2];                     // [0]=V (bank0), [1]=A (bank1)
@@ -214,7 +230,11 @@
     std::vector<RecR>  up_r;
     std::vector<RecB>  up_b;
 
     bool nonblock_enable = true;
+    // Per-DMA read->write coupling-FIFO depth.  3 = the v1 default and the
+    // value every existing log was produced with; 512 = the v2 full 4 KiB job
+    // buffered per lane.  Set from +couple-depth=3|512 in main().
+    uint32_t couple_depth = 3;
     long g_cycle         = 0;
     long scenario_start  = 0;
     uint32_t ptw_latency = 1;
@@ -522,13 +542,13 @@
 
     // ---------------------------------------------------------------------
     // upstream R acceptance: throttle tr_r_ready from the RETURNED id and
-    // the 3-beat coupling-FIFO room.  Called after the first clk-low eval;
+    // the coupling-FIFO room (Tb::couple_depth).  Called after the first clk-low eval;
     // re-evaluates so the combinational chain settles before sampling.
     // ---------------------------------------------------------------------
     void settle_upstream_r_ready() {
         if (top->tr_r_valid) {
             uint32_t bank = (top->tr_r_id >> BANK_BIT) & 1u;
-            top->tr_r_ready = (dma[bank].cfifo.size() < COUPLE_DEPTH) ? 1 : 0;
+            top->tr_r_ready = (dma[bank].cfifo.size() < couple_depth) ? 1 : 0;
         } else {
             top->tr_r_ready = 1;
         }
@@ -741,7 +761,7 @@
                 Beat b; b.data = s_tr_r_data; b.strb = 0xFF;
                 b.last = s_tr_r_last; b.id = s_tr_r_id;
                 m.cfifo.push_back(b);
-                CHECK(m.cfifo.size() <= COUPLE_DEPTH, "coupling FIFO overflowed");
+                CHECK(m.cfifo.size() <= couple_depth, "coupling FIFO overflowed");
                 int h = r_half_cur[bank];
                 CHECK(h < 2, "R half pointer overflow");
                 m.rx[h]++;
@@ -1141,15 +1161,15 @@
         CHECK(n_tl_resp > 0, "no translation response returned by the TL");
         CHECK(n_ddtw_ar + n_ptw_ar > 0, "no walker reads observed");
 
-        std::printf("COUPLED-SUMMARY %s cycles=%ld comp_ar=%lu ddtw_ar=%lu ptw_ar=%lu "
-                    "tl_accept=%lu tl_resp=%lu iotlb_look=%lu iotlb_hit=%lu "
-                    "ptw_active=%lu r_beats=%lu b_up=%lu\n",
-                    name, g_cycle - scenario_start,
+        std::printf("COUPLED-SUMMARY %s couple_depth=%u cycles=%ld comp_ar=%lu "
+                    "ddtw_ar=%lu ptw_ar=%lu tl_accept=%lu tl_resp=%lu "
+                    "iotlb_look=%lu iotlb_hit=%lu ptw_active=%lu r_beats=%lu b_up=%lu\n",
+                    name, couple_depth, g_cycle - scenario_start,
                     (unsigned long)n_comp_ar, (unsigned long)n_ddtw_ar,
                     (unsigned long)n_ptw_ar, (unsigned long)n_tl_accept,
                     (unsigned long)n_tl_resp, (unsigned long)n_iotlb_lookup,
                     (unsigned long)n_iotlb_hit, (unsigned long)n_ptw_active,
                     (unsigned long)n_r_beats, (unsigned long)n_b_up);
     }
 };
 
@@ -1176,6 +1196,7 @@
     // optional single-scenario selection: +scenario=<name>
     // optional priority-order selection: +order=vv|aa|av|rr  (default vv)
     // optional overlap mode:              --overlap
+    // optional coupling depth:            +couple-depth=3|512 (default 3)
     //
     //   vv : AR prefers V, AW prefers V   (matching-source order, default)
     //   aa : AR prefers A, AW prefers A
@@ -1187,20 +1208,33 @@
     //        legal AXI stream, e.g. V.h0 -> A.h0 -> V.h1 -> A.h1.
     std::string only;
     std::string order = "vv";
+    std::string couple_depth_str = "3";
     bool overlap = false;
     for (int i = 1; i < argc; ++i) {
         std::string a = argv[i];
         const std::string skey = "+scenario=";
         if (a.rfind(skey, 0) == 0) only = a.substr(skey.size());
         const std::string okey = "+order=";
         if (a.rfind(okey, 0) == 0) order = a.substr(okey.size());
+        const std::string ckey = "+couple-depth=";
+        if (a.rfind(ckey, 0) == 0) couple_depth_str = a.substr(ckey.size());
         if (a == "--overlap") overlap = true;
     }
     if (order != "vv" && order != "aa" && order != "av" && order != "rr") {
         std::fprintf(stderr, "nonblock_coupled_test: unknown +order='%s' "
                              "(expected vv|aa|av|rr)\n", order.c_str());
         return 2;
     }
+    // Strict depth validation: exactly the two known values.  A free-form
+    // integer would let a typo silently become the coupling-FIFO bound, and the
+    // overflow CHECK would then happily agree with the typo.
+    uint32_t couple_depth = 0;
+    if (couple_depth_str == "3")        couple_depth = 3;
+    else if (couple_depth_str == "512") couple_depth = 512;
+    else {
+        std::fprintf(stderr, "nonblock_coupled_test: invalid +couple-depth='%s' "
+                             "(expected exactly 3 or 512)\n", couple_depth_str.c_str());
+        return 2;
+    }
     int order_ar = 0, order_aw = 0;   // 0 = V first, 1 = A first
     bool order_rr = false;
     if (order == "aa")      { order_ar = 1; order_aw = 1; }
@@ -1215,6 +1249,7 @@
         std::printf("nonblock_coupled_test: +order=%s (AR %s-first, AW %s-first)\n",
                     order.c_str(), order_ar ? "A" : "V", order_aw ? "A" : "V");
     }
     std::printf("nonblock_coupled_test: overlap_mode=%d\n", (int)overlap);
+    std::printf("nonblock_coupled_test: couple_depth=%u\n", couple_depth);
 
     int ran = 0, failed = 0;
     for (const auto& s : plan) {
@@ -1226,6 +1261,7 @@
             tb.order_aw_prefer = order_aw;
             tb.rr_order        = order_rr;
             tb.overlap_mode    = overlap;
+            tb.couple_depth    = couple_depth;
             tb.run_scenario(s.name, s.en, s.a, s.lat);
         } catch (const std::exception& e) {
             ++failed;
--- a/tests/test_nonblock_coupled.py
+++ b/tests/test_nonblock_coupled.py
@@ -6,10 +6,16 @@
 list and include directories as tests/test_nonblock_banked.py.  No RTL file is
 copied or duplicated: the source list is imported from the sibling runner.
 
 RR overlap sweep (all 12 scenarios in one run):
     python3 tests/test_nonblock_coupled.py --overlap --order rr
-Legacy control:
-    python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr
+
+v2 full-4KiB coupling buffer (same RTL, runtime flag only; see --couple-depth):
+    python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512
+
+Legacy control (default coupling depth 3):
+    python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr
+Legacy control, v2 depth:
+    python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr --couple-depth 512
 """
 
 import argparse
@@ -79,6 +85,14 @@
                          "the shared upstream port, e.g. V.h0 -> A.h0 -> "
                          "V.h1 -> A.h1; combine with --overlap to cover all "
                          "12 scenarios with that legal AXI ordering.")
+    ap.add_argument("--couple-depth", type=int, choices=(3, 512), default=3,
+                    help="per-DMA read->write coupling FIFO depth, forwarded to "
+                         "the C++ harness as +couple-depth=<...> (default: 3). "
+                         "3 is the historical v1 3-beat coupling depth every "
+                         "existing log was produced with; 512 is the v2 full "
+                         "4 KiB job (both 2 KiB halves) buffered per DMA lane. "
+                         "Runtime flag only: the RTL source list and the obj "
+                         "directory are deliberately unchanged, because the same "
+                         "harness back-pressures and checks at both depths.")
     ap.add_argument("--reduced", action="store_true",
                     help="reduced OT4/W8 config instead of the board OT8/W32 config")
     ap.add_argument("--scenario", default=None,
@@ -93,6 +107,9 @@
     OUT.mkdir(parents=True, exist_ok=True)
     # Default mode keeps its original obj dir so an already-passing build is
     # never overwritten.  Only the opt-in overlap mode uses a sub-directory.
+    # --couple-depth is a RUNTIME flag only: same RTL, same Verilator source
+    # list, no new SV adapter, so it never appears in the obj dir.  A depth-3
+    # run and a depth-512 run of the same mode therefore reuse one binary.
     obj_root = (OUT
                 / ("legacy" if opts.legacy else "banked")
                 / ("reduced" if opts.reduced else "board"))
@@ -140,11 +157,14 @@
         sys.stderr.write("verilator did not produce {}\n".format(exe))
         return 1
 
-    run_args = [str(exe), "+order={}".format(opts.order)]
+    run_args = [str(exe), "+order={}".format(opts.order),
+                "+couple-depth={}".format(opts.couple_depth)]
     if opts.scenario:
         run_args.append("+scenario={}".format(opts.scenario))
     if opts.overlap:
         run_args.append("--overlap")
+    print("test_nonblock_coupled: couple_depth={} order={} overlap={}".format(
+        opts.couple_depth, opts.order, int(opts.overlap)))
     run = subprocess.run(run_args)
     return run.returncode
 
--- a/docs/research-n-prototype-spec-20261004.md
+++ b/docs/research-n-prototype-spec-20261004.md
@@ -1,6 +1,6 @@
 # 研究规格：跨上下文 N 原型（未就绪实现）
 
-本文件记录下一阶段的研究约束，尚不是实现就绪的规格。本轮不改 RTL/驱动、不编译、不板测；文末接受条件均为"将来须证明"，不是"已通过"。本稿只做对上一版 N 提案的有限必要纠正，依据 Codex 已直接核对的 critical 源码（out/method88-literature-20261004/codex-n-critical-review.md），不重复完整 source 索引。
+本文件记录下一阶段的研究约束，尚不是实现就绪的规格。状态（2026-10-04）：v2 架构方向已定（见文末 §v2更新：DMA0/1 每路 4 KiB 整缓冲，只改 FPGA 集成参数，不改驱动/Linux/静态 reader/P4M ABI），但 v2 尚未仿真、尚未编译、尚未上板；v1 只有有限 C++ 模型与"两个 BIT 构建中"的证据。文末接受条件均为"将来须证明"，不是"已通过"。本稿只做对上一版 N 提案的有限必要纠正，依据 Codex 已直接核对的 critical 源码（out/method88-literature-20261004/codex-n-critical-review.md），不重复完整 source 索引。
 
 ## 1. 目标与第一阶段边界
 
@@ -55,7 +55,7 @@
 ## 5. 容量、输出、失效与 fault 的一致约束
 
-- 容量：保持 16 项 IOTLB 和原 DDT/PDT 缓存、一个 PTW，不复制 IOMMU。逐类列出请求槽、查表记录、结果槽和端口数量；仅保持参数名不变不等于总容量相同，移出原 FIFO 的请求仍须计入容量。N/P 比较必须保持每类总容量与仲裁/替换规则相同。
+- 容量：保持 16 项 IOTLB 和原 DDT/PDT 缓存、一个 PTW，不复制 IOMMU。逐类列出请求槽、查表记录、结果槽和端口数量；仅保持参数名不变不等于总容量相同，移出原 FIFO 的请求仍须计入容量。N/P 比较必须保持每类总容量与仲裁/替换规则相同。v2 已改变每路数据缓冲：DMA0/1 由 3 拍（24 B）/路 变为 512 拍（4096 B）/路，DMA2/3 仍为 3 拍（24 B）/路，两侧数据容量净增 8144 B（见文末 §v2更新）。逐类核算必须按 4096 B/路（DMA0/1）重做，不能再引用"数据深度不增加"。
 - 输出：仅一路 trans_resp_data_o（rv_iommu_tl_wrap.sv:71-75、:1446-1449）；两完成同周期不可表示，需仲裁，不能假装多端口。
 - 失效：probe/V 必须尊重 IOTLB 失效与更新（rv_iommu_iotlb.sv:205-303、:306-320）；必须定义与 inval 同周期、已取得结果尚未交付及在途查表的顺序。未证明能合法交付时，暂停命中通道并按原顺序处理。软件 quiet 不能替代内部顺序和失效代约束。
 - fault：错误路径与 axi_err_slv 对主/次请求都必须完整（rv_iommu_tl_wrap.sv:1398-1425；rv_iommu_trans_ctl_wrap.sv:577-592）。权限失配返回 fault，不返回 hit。
@@ -110,6 +110,37 @@
 
 ## v1更新：共享写数据所有权下的读接纳（2026-10-04）
 
-v0“AW/W关联已保持”不足以排除读写依赖循环。S/N两版都增加相同读接纳规则：另一银行的W尚未收到LAST时，暂缓选择新读；新AW进入FIFO的同一拍也暂缓选择读，下一拍由已有AW优先规则处理。已经发出的AXI/TL VALID不撤回；缓存与查表器仍各一份，总槽/数据深度不增加。新增有符号AW减WLAST余额和银行所有者属于控制成本，不能称零开销。
+v0“AW/W关联已保持”不足以排除读写依赖循环。S/N两版都增加相同读接纳规则：另一银行的W尚未收到LAST时，暂缓选择新读；新AW进入FIFO的同一拍也暂缓选择读，下一拍由已有AW优先规则处理。已经发出的AXI/TL VALID不撤回；缓存与查表器仍各一份，AW/AR 槽总数不变；但"数据缓冲深度不增加"只对 v1 成立，v2 已把 DMA0/1 每路耦合缓冲从 24 B 提高到 4096 B（见 §v2更新）。新增有符号AW减WLAST余额和银行所有者属于控制成本，不能称零开销。
 
 有限共享返回模型36场景通过，原控制器对照12场景通过，旧联合命中/权限/背压测试通过。模型不是实际DMA后端或DDR；同一4KiB内两段请求更大范围的重叠、完整平台活性仍需板测。此规则可能减少命中绕过机会，不预设N有吞吐收益或创新。两个v1 BIT正在编译。详见docs/nonblock-dependency-diagnosis-20261004.md。
+
+## v2更新：4 KiB 整缓冲（方案B，2026-10-04，未仿真/未编译/未上板）
+
+背景：v1 的读接纳守卫（见上一节）只延迟"读的选择"，并不增加耦合缓冲容量；板级 A8 停滞的精确根因仍未确认（docs/nonblock-dependency-diagnosis-20261004.md:139-141）。v2 因此选择"整缓冲"方案 B：把 DMA0/1 的每路读→写耦合缓冲从 3 拍提高到 512 拍，使一个 4 KiB job 的两半（2 × 256 个 8 B 拍）都能被缓冲，而不是引入更一般的读写 credit 规则。S（mode1，双银行）与 N（mode2，双银行+有限命中绕过）同为 512；DMA2/3 与原版 mode0 仍为 3 拍。
+
+集成点（由 Codex 修改，不在本稿内给 diff）：`ariane_peripherals_*.sv` 的 genDMA 生成块把 `BUFFER_DEPTH` 绑为 `(IOMMUNonblockMode != 0 && i < 2) ? 512 : 3`：S/N 两个 nonblock 实例的 DMA0/1 为 512，DMA2/3 与原版 mode0 仍为 3。DMA 侧该参数透传路径已核对：`dma_core_wrap` 的 `BufferDepth`（cva6/corev_apu/iDMA/src/systems/cva6/dma_core_wrap.sv:27，默认 3）→ `dma_core_wrap_intf` 的 `BUFFER_DEPTH`（同文件:349，默认 3）→ `idma_backend`（同文件:293、:411）。
+
+### 容量核算（数据字节，非面积）
+
+- DMA0/1：每路耦合缓冲 3 拍 × 8 B = 24 B → 512 拍 × 8 B = 4096 B。
+- 两路合计：48 B → 8192 B，数据容量净增 8144 B。
+- DMA2/3：仍 3 拍 × 8 B = 24 B/路，不增加。
+- 随动元数据存储：`MetaFifoDepth` 11 → 520（随 BufferDepth 缩放）。该数值取自 Codex 的集成方案，本稿未核对 idma_backend.sv 中 MetaFifoDepth 的推导，合入前需复核。
+- 本稿不断言综合面积、FF 或 LUT/RAM 结果；这些必须在真实综合后测量，上面只给数据字节数。
+- v1 的读接纳守卫不删除；IOTLB/DDTC/PTW/AXI 槽/W FIFO 预算不变。
+
+### 该方案能/不能支撑的主张
+
+- 能：在有限 C++ 模型里，把耦合缓冲从 3 拍到 512 拍后，"共享 R 返回队头已满、另一路 W 无法推进"那类循环（docs/nonblock-dependency-diagnosis-20261004.md:83-106）不再由 3 拍容量触发。
+- 不能外推：batch 路径只启动正好 4096 B 的 job——`idma_reg64_frontend.sv:146-150` 要求 `num_bytes == 4096`，`:151` 的 `batch_task_issue` 在 COUNT 且 ready 时发出，`:152` 的 `batch_task_done` 必须等 WAIT 且 `trans_complete_i`，完成后才进入下一个 job。因此本方案只覆盖"整 4 KiB、一次一个 job"的形状，不能据此断言任意 job、descriptor、多 job 或一般 AXI 情况无死锁。
+- 不能外推（区分度）：原版 15 项板级数据是在 3 拍缓冲下取得的（scripts/mac_nonblock_pilot.py:103 `DEFAULT_CHUNK = 4096`；:141-144 的 short/long 计划），只能描述"银行布局 + 必要缓冲合并开销"，不能用来分离 N 的绕过增量。只有 S 与 N 同为 512 的公平对照，才可能把绕过增量从缓冲容量本身的影响中分离出来。
+- 仍不主张 novelty：N 仍是前人已知 hit-under-miss 的基线（见上文"实现更新：两银行生命周期"段末），静态银行划分已改变资源分配，不能改名为额外保护方法。
+
+### 测试侧状态
+
+- C++ 有限模型与 runner 支持 `+couple-depth=3|512`（tests/nonblock_coupled_test.cpp）与 `--couple-depth`（tests/test_nonblock_coupled.py，choices 3/512，默认 3）。同一个可执行文件覆盖两个深度：RTL 源列表与 obj 目录不变，不新增 SV adapter。
+- 反压（tr_r_ready）与耦合 FIFO 溢出检查共用同一深度值，避免"按一个深度反压、按另一个深度检查"。
+- 尚未执行：本稿不声称 v2 的任何仿真、编译、综合或板级结果。验收命令（尚未运行）：
+  - `python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512`
+  - 对照：`python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr`（默认深度 3）
+- 默认深度 3 的既有失败日志保留为基线证据，不重跑、不删除：out/n-prototype-20261004/coupled-banked-vv.log（8/12 失败）、out/n-prototype-20261004/coupled-banked-guard-vv.log（4/12 失败）。
```

## 5. 另两文档的追加段草案（不直接 diff）

追加到 `docs/nonblock-pilot-plan-20261004.md` 末尾：

```markdown
## 2026-10-04 v2（4 KiB 整缓冲）当前进展

- 已定方向：不引入一般读写 credit 规则，改为把 DMA0/1 每路读→写耦合缓冲从 3 拍（24 B）提高到 512 拍（4096 B）；DMA2/3 与原版 mode0 仍 3 拍。S（mode1）与 N（mode2）同为 512，只有这样才能把绕过增量从缓冲容量本身的影响中分离出来。
- FPGA 侧只改 `ariane_peripherals_*.sv` genDMA 的 BUFFER_DEPTH 绑定（`(IOMMUNonblockMode != 0 && i < 2) ? 512 : 3`），只需 BIT；Linux 00093、静态 reader 与 P4M ABI 不变。
- 本 pilot 的 15 项原版数据是 3 拍缓冲下取得的，只能描述"银行布局 + 必要缓冲合并开销"，不能用于分离 N 的绕过增量。
- v2 尚未仿真、尚未编译、尚未上板；v1 的两个 BIT 仍在编译。原版 15 项已完成不重跑；默认深度 3 的既有失败日志作为基线保留，不删除。
- 本 pilot 的固件、命令行与 15 项计划本身不变；`--reader` 要求与 mode 映射不变。
```

追加到 `docs/nonblock-dependency-diagnosis-20261004.md` 末尾：

```markdown
## 附：v2（4 KiB 整缓冲）与本文档结论的关系（草案，未执行）

- 本文档 §3 的循环是 3 拍耦合缓冲下的有限模型结论；v2 把 DMA0/1 每路缓冲改为 512 拍（4096 B/路；两路合计净增 8144 B；DMA2/3 仍 24 B/路），用于检验"容量不足"这一必要条件。
- 这不改变 §4.3：板级 A8 根因只有在修复后的板级重跑后才可能确认；v2 尚未仿真/编译/上板，本节不新增根因结论。
- 模型侧新增 `+couple-depth=3|512`（同一可执行文件、同一 RTL 源列表、不新增 SV adapter）。验收命令（尚未运行）：`python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512`；对照 `python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr`（默认 3）。
- 默认深度 3 的既有失败日志（coupled-banked-vv.log 8/12、coupled-banked-guard-vv.log 4/12）保留，不重跑、不删除。
```

## 6. 验收（尚未执行）

```bash
# v2 深度（S/N 同 512 的模型侧验证；同一 obj 目录，无新 SV adapter）
python3 tests/test_nonblock_coupled.py --overlap --order rr --couple-depth 512

# 对照：原控制器 + 默认深度 3
python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr

# 可选：legacy 也跑 v2 深度，确认 legacy 侧不受深度影响
python3 tests/test_nonblock_coupled.py --legacy --overlap --order rr --couple-depth 512
```

验收观察点：stdout 应出现 `nonblock_coupled_test: couple_depth=512`、每个场景 `COUPLED-SUMMARY <name> couple_depth=512 ...`，并以 `PASS (12 scenarios)` 结束；若因 cfifo 相关 STALL 失败，dump 中 `cfifo=` 可用于区分"入队/出队"与"深度"两类原因。默认深度 3 的既有失败日志作为基线保留，不重跑。

## 7. 不确定项 / 需 Codex 复核

1. `MetaFifoDepth 11 → 520` 我没有源文件可核对（`idma_backend.sv` 未提供），spec 段落已标注"合取前需复核"。
2. `ariane_peripherals_*.sv` 的 genDMA 行号我无法给出，spec 只写了文件名与表达式，未编行号。
3. depth 512 下 cfifo 上界为 512 的推理（每半 256 R 拍 × 2）未经仿真；`<=` 边界在真实 stimulus 下是否会触及未验证。
4. C++ 端用字符串精确匹配做"严格校验"，runner 端用 argparse `choices`；两者语义一致但实现不共享，若将来增加第三档深度需同时改两处。