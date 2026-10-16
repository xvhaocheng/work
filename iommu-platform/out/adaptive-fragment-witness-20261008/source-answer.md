# 导航概要

| 问题 | 结论（证据强度） | 关键 file:line |
|---|---|---|
| Q1 几何与 AW 次序 | **提供的文件不足以判定“第二目的 W 的有效译址/匹配 prepared‑AW 出现在第一 R 后 1024 返回之前”**。能从 legalizer+TC 推出的只有：几何=(1×R2048, 2×W1024)、TC 每 bank 串行、物理 AW 地址=译后 spaddr。iDMA manager/R→AW coupler/虚拟 AW 发射次序缺失 | `idma_legalizer.sv:82-83,209-214,236-268,452-466`；`rv_iommu_trans_ctl_wrap.sv:603-733,628,646,716-727,333` |
| Q2 仅改 C++ 运行时可否验证 | **不可以**。CONF、copy len、dst 偏移均为硬编码；batch 只能表达 N×4KiB job；E2 pool 结构上只接受 4096 对齐、长度 4096 的 job；无独立 manual 场景 | `nonblock_actual_dma_test.cpp:100-101,1224-1229,1713-1735,2086-2116`；`nonblock_completion_pool_late.sv:402-405`；`nonblock_actual_dma_system_top.sv:80,91` |
| Q3 几何 smoke | 配置/内存**接口足够**，可给 C++‑only patch（relink，不改生产 SV）。观测次序需在 `tick()` 内加 4 行（已含在 patch）。前提：CONF bit0=decouple_rw 待核；E2 必须 Mode 0 | 见 §3 patch |
| Q4 G vs greedy | 形式化：`credit=0` 或 `p≥Lmax` 时 G≡greedy；`credit>0 且 0<p<Lmax` 时 G 严格更保守（可区分但只读更少）。当前 W‑head 直通（`can_drain`）已实现 greedy 可达集，G/H 无独有决定 | `translation-lane-...md:50,52`；`nonblock_completion_pool_late.sv:94-106,108-124,146-169` |

**未实现/未运行/创新未成立**：本文只做源码取证与草案，未跑任何仿真，未改任何 RTL，不主张 G/H 已实现或创新成立。

---

# Q1：几何与“第二目 W 的译址/prepared‑AW”时序

## 1.1 能从提供文件确定的部分

### (a) legalizer 几何 = 1×R2048 + 2×W1024

- PageSize/PageAddrWidth 定义：`idma_legalizer.sv:82-85`
  ```
  PageSize = (MaxBeatsPerBurst*StrbWidth > 4096) ? 4096 : MaxBeatsPerBurst*StrbWidth
  ```
  AXI `MaxBeatsPerBurst=256`（`:75`）。无论 `DataWidth=64`（PageSize=2048）还是 `DataWidth=512`（PageSize=4096），下面结论都成立。
- 读页内剩余：`r_num_bytes_to_pb = r_page_size - r_page_offset`（`:163`）；写侧同理 `:193`。
- decouple_rw 时源/目的各自用本侧页内剩余：`r_num_bytes_possible = decouple_rw ? r_num_bytes_to_pb : c_num_bytes_to_pb`（`:209-210`），`w_...`（`:213-214`）。
- 切分逻辑：长度 > possible 则切一段并推进地址，否则一段收尾（读 `:236-249`，写 `:255-268`）。
- 请求装配：读 AR 的 len 由 `(r_num_bytes + r_addr_offset - 1) >> OffsetWidth`（`:336`）；写 AW 同理（`:358`）。
- 流控：decouple_rw 时 `r_tf_ena = r_ready_i & !flush_i`，`w_tf_ena = w_ready_i & !flush_i`（`:453-458`），即源读与目的写各自推进。

代入：源 offset=0、len=2048 ⇒ 单段（256 beats，len=255）。目的 offset=3072、len=2048 ⇒ 第一段到 2048/4096 边界=1024B（128 beats，len=127），第二段 1024B。**与用户给的 “W1024+1024” 一致**（PageSize=2048 时 3072 mod 2048=1024，剩 1024；PageSize=4096 时 4096−3072=1024。两者都得 0x1000 边界）。

### (b) TC 每 bank 串行、AW 优先、物理 AW=译后 spaddr

- 状态机 IDLE/REQ_TRANS/AWAIT_RESP/COMPLETE：`rv_iommu_trans_ctl_wrap.sv:96-102`。
- 每 bank 一个 AW FIFO（深度 `NumOutstandingTrans`）`:121-139`；AR FIFO `:146-164`；W FIFO `:222-240`。
- IDLE 内 **AW 先于 AR** 判定：`if (aw_valid && !w_cnt_ovf)`（`:628`）在 `else if (ar_valid && ...)`（`:646`）之前。
- 完成时物理 AW/AR 的地址取译后 `spaddr`：`axi_comp_req.aw.addr = addr_t'(trans_resp_data_q.spaddr)`（`:333`），AR 同 `:347`。
- COMPLETE 发出物理 AW，握手后回 IDLE：`:716-727`。
- 结论：**同一 bank 内，第二目 W 的“有效译址”（=TC 物理 AW）只可能在该 bank TC 回到 IDLE 且第二目 AW 成为 FIFO head 之后才出现**；而该 bank 的源读 AR 也走同一 FSM。因此 TC 层是串行瓶颈。

### (c) 生产 banked ctl 的读准入

- bank 选择=原始 AXI ID bit `BankIdBit`：`rv_iommu_banked_ctl.sv:243-244`。
- 读准入保护（W 未填满期间限制对侧读）：`:246-278`。
- 生产配置强制 `ReadAdmissionEnable(1'b1)`：`rv_iommu_banked_ctl.sv:318`；测试变体为 `!CompletionGateEnable`：`nonblock_completion_banked_ctl.sv:329`。

## 1.2 无法从提供文件判定的核心点

问题真正依赖的是 **iDMA 内部 manager / R→AW coupler 是否在第一 R 的后半返回前就发出第二目 W 的虚拟 AW**，以及 **CONF bit0 到底是 `decouple_rw` 还是 `decouple_aw`**。这两者都不在提供的文件中：

- 测试注释明说 DMAs = “frontend + legalizer + R→AW coupler + datapath”（`nonblock_actual_dma_test.cpp:8-9`），但 coupler/manager/frontend/datapath 源文件均未提供。
- 测试只写 `REG_CONF=0x0`，没有 CONF 位域解码（`:101,1225,1715`）。位域定义在注释指向的 `idma_reg64_frontend_reg_pkg.sv:110-119`（`:95`），未提供。
- 内层 harness `tests/nonblock_actual_dma_top.sv` 未提供，无法确认 banked ctl / pool 的实际接线（`nonblock_actual_dma_system_top.sv:1096-1253` 只实例化它）。
- 翻译引擎本体（TC 下游）未提供。
- 文档本身也只做“测量到 DS 首 R 均值/最大值、A PTW-active”等，未给 AW 发射次序（`translation-lane-...md:24`）。

**诚实结论**：不能判定。能给的边界是：
1. legalizer 证明几何固定为 1×R2048 + 2×W1024；
2. TC 串行 ⇒ 第二目 W 的物理 AW（“有效译址”）必然晚于第一目 W 的物理 AW 完成；
3. 但“早于第一 R 后半返回”取决于 iDMA 是否在数据到齐前就发射 W 的虚拟 AW（即 `decouple_aw` 语义 / coupler 是否要求读数据）。这必须看 iDMA 源码。

## 1.3 精确认列的下一批文件（按需精确，不猜）

在 `nonblock_actual_dma_top.sv` 实例化处确认确切路径后提供以下（文件名以实例化的模块名为准）：

1. `.../iDMA/src/idma_reg64_frontend_reg_pkg.sv` — **CONF bit0 语义**（引用于 `nonblock_actual_dma_test.cpp:95`）。
2. `.../iDMA/src/idma_reg64_frontend.sv` — 寄存器文件与 batch 窗口（引用于 `:96`）。
3. iDMA manager / midend：`idma_manager.sv`、`idma_midend.sv`（或 distributed midend）。
4. R→AW coupler 与数据通路：`idma_backend_rw_axi.sv`（含 R→AW 依赖）/ `idma_r_data_path.sv` / `idma_w_data_path.sv`。
5. 测试内层 harness：`tests/nonblock_actual_dma_top.sv`（确定 DMA↔banked_ctl↔pool 接线与 `SEL_V` 路由）。
6. TC 下游翻译引擎（`rv_iommu_trans_eng_wrap.sv` / `rv_iommu_ptw.sv` 类）以确认翻译时延/次序。
7. （若存在）测试用 `dma_core_wrap.sv`（含 frontend+legalizer+coupler+datapath 的包装）。

---

# Q2：现 test cpp 能否仅改运行时/映射验证该形状

**结论：不能仅靠运行时配置/映射。** 现有 cpp 把关键量全部硬编码，且没有独立的 manual 单拷贝场景。

| 需求 | 现状 | 证据 |
|---|---|---|
| CONF | `REG_CONF=0x18`（`nonblock_actual_dma_test.cpp:101`），只写 `0x0` | `:1225`（`program_single`）、`:1715`（`configure_batch`） |
| copy len | `REG_NB=0x10`（`:100`），写死 `PAGE_BYTES=4096`（`:92`） | `:1228,1718` |
| dst 映射 | `V_DST_IOVA=0x10001000→V_DST_PA=0x80001000`、`V_COLD_DST_IOVA=0x10002000→V_COLD_DST_PA=0x80002000`，**offset=0（页对齐）**；PTE 在 `0x50008/0x50010` | `:81-85,476-477` |
| 校验 | 整 4KiB 页逐字节校验 | `:2086-2116`（V+A）、`:2092-2106`（cold-dst） |
| E2 pool gate | `SharedPoolMode`（`system_top:80`）、`PoolSlots`（`:91`）为**编译期**参数，cpp 无运行时旋钮；pool 结构上只接受 `job_length==4096 && [11:0]==0` 的 job | pool `:402-405` |
| batch 接口 | `configure_batch/start_batch/stop_batch`（`:1713-1735`），job 单位是“4KiB job”，**无法表达 2048 字节单拷贝** | `:1713-1735` |
| manual 接口 | `program_single`（`:1224-1229`）+ `trigger`（`:1233-1235`）+ `wait_done`（`:1238-1255`），但只被 warm 路径调用 | 同左 |

**要点**：
- 2048B/offset3072 的 dst 会跨 IOMMU 4KiB PTE 边界（`0x10001C00+0x800=0x10002400`），需要两个 PTE（现有 `V_DST`/`V_COLD_DST` 恰好覆盖，物理页连续，可复用）。
- 但 **E2 pool（Mode≥4）与几何不自洽**：pool 对 `job_fire` 检查 `job_length_i!=4096 || src[11:0]!=0 || dst[11:0]!=0` 即置 `fault_o[0]`（`nonblock_completion_pool_late.sv:402-405`）。此形状必须 **E2 Mode 0**，且 Mode 无法运行时切换。
- 因此验证该形状 = 需要 **C++ 源码改动（加 CONF/len/dst-off 旋钮 + 独立 manual 场景）并 relink**，不改生产 SV。这**不是**“仅改运行时配置”。

---

# Q3：几何 smoke 方案（非新机制） + C++ patch

## 3.1 方案

- 同一次手动拷贝：`src=V_SRC_IOVA(0x10000000)`、`dst=V_DST_IOVA+3072(0x10001C00)`、`len=2048`，两次运行 `CONF=0` 与 `CONF=1`（默认 `mode=s`，nonblock off；`policy=0`；无 pool；`DmaBufferDepth=512` beats ≥ 2048B ⇒ full‑buffer 模型，无流控压力）。
- 走 **manual 路径**（`program_single` 系列），batch 关闭 ⇒ 合成 CPU `cpu_run=0` 静默 ⇒ 满足 silence 检查。
- 保留整段字节校验、响应/静默检查、`E2-fault` 检查（tick 中无条件）。
- 观测：记录两个物理 AW 的 cycle 与第一 R 的 beat‑0 / beat‑128（后 1024 起点）cycle，输出 `GEO-SMOKE-JSON`，用于**判读**（不是自动判定“创新”）。

## 3.2 接口充分性判断

配置接口（`cfg_wr`）+ 内存模型（`write64/read64`）+ 整场景计数器（`n_ddr_aw/n_ddr_r/...` 在 `tick()` 上升沿**窗口外**累加，`:1086-1124`）**足够**。次序观测原 `event_trace` 被 `win_open` 门控（`:420-425`），manual 不打开窗口，故需在 `tick()` 加轻量记录。**因此可给完整小 patch**（仅 C++，relink）。

**前提/告警（须核）**：
- CONF bit0 = `decouple_rw` 未证实（需 `idma_reg64_frontend_reg_pkg.sv`）；若 bit0 是 `decouple_aw`，本 smoke 语义需改。
- 现有 system binary 必须用 `SharedPoolMode=0` 构建（无运行时旋钮）。
- `nonblock_actual_dma_top.sv` 是否把 `SEL_V` 接到真实 DMA 且 banked ctl 正常，无法从提供文件确认。

## 3.3 建议 patch（`nonblock_actual_dma_test.cpp`，unified diff）

```diff
@@ class Tb { public:
     uint64_t a_target          = 1048576ULL;   // or 0x100000000ULL
+
+    // ---- geometry-smoke knobs (manual single copy; no pool) -----------
+    bool     geo_smoke    = false;
+    uint32_t geo_conf     = 0;
+    uint32_t geo_len      = PAGE_BYTES;
+    uint64_t geo_dst_off  = 0;
+    uint32_t geo_aw_n     = 0;
+    long     geo_aw_cycle[8] = {0,0,0,0,0,0,0,0};
+    long     geo_r_start  = 0;
+    long     geo_r_mid    = 0;
 
     long g_cycle        = 0;
@@ void reset(bool enable) {
         w_ds_latency_sum = w_ds_latency_n = w_ds_latency_max = 0;
         ddr_r_held = false;
+        geo_aw_n = 0; geo_r_start = 0; geo_r_mid = 0;
+        for (auto& c : geo_aw_cycle) c = 0;
         write_tail_holds = write_tail_hold_cycles = 0;
@@ void program_single(bool sel, uint64_t src, uint64_t dst) {
     // program ONE batch-off 64-bit copy
     void program_single(bool sel, uint64_t src, uint64_t dst) {
         cfg_wr(sel, REG_CONF, 0x0);
         cfg_wr(sel, REG_SRC,  src);
         cfg_wr(sel, REG_DST,  dst);
         cfg_wr(sel, REG_NB,   (uint64_t)PAGE_BYTES);
     }
+
+    // Same manual copy, but with explicit CONF and byte length.
+    void program_single_ex(bool sel, uint64_t src, uint64_t dst,
+                           uint64_t conf, uint64_t len) {
+        cfg_wr(sel, REG_CONF, conf);
+        cfg_wr(sel, REG_SRC,  src);
+        cfg_wr(sel, REG_DST,  dst);
+        cfg_wr(sel, REG_NB,   len);
+    }
+
+    // ---------------------------------------------------------------------
+    // Geometry smoke: one manual 2048 B copy, src 0x10000000 -> dst
+    // 0x10001000 + geo_dst_off (== 3072 => crosses the 4 KiB PTE boundary).
+    // Reuses the existing V_SRC/V_DST/V_COLD_DST mappings. E2 must be Mode 0.
+    // ---------------------------------------------------------------------
+    void run_geometry_smoke() {
+        std::printf("== geometry-smoke conf=%u len=%u dst_off=%llu ==\n",
+                    geo_conf, geo_len, (unsigned long long)geo_dst_off);
+        setup_memory(a_pages);
+        measurement_phase = false;
+        reset(false);                      // nonblock off
+        scenario_start      = g_cycle;
+        last_progress_cycle = g_cycle;
+        v_progress_cycle    = g_cycle;
+
+        const uint64_t src = V_SRC_IOVA;
+        const uint64_t dst = V_DST_IOVA + geo_dst_off;
+        program_single_ex(SEL_V, src, dst, (uint64_t)geo_conf, (uint64_t)geo_len);
+        const uint64_t id = trigger(SEL_V);
+        wait_done(SEL_V, id, "geometry-smoke");
+        check_quiet("geometry-smoke");
+
+        const uint64_t words = (uint64_t)geo_len / 8;
+        for (uint64_t j = 0; j < words; ++j) {
+            CHECK(read64(V_SRC_PA + j * 8) == page_word(0, (uint32_t)j),
+                  "geometry-smoke source page changed");
+            const uint64_t off = geo_dst_off + j * 8;
+            const uint64_t dpa = (off < 4096) ? (V_DST_PA + off)
+                                              : (V_COLD_DST_PA + (off - 4096));
+            CHECK(read64(dpa) == page_word(0, (uint32_t)j),
+                  "geometry-smoke destination bytes do not match the source");
+        }
+        CHECK(n_ddr_aw == 2, "geometry-smoke expected exactly two write bursts");
+        std::printf(
+            "GEO-SMOKE-JSON {\"conf\":%u,\"len\":%u,\"dst_off\":%llu,"
+            "\"ddr_ar\":%lu,\"ddr_aw\":%lu,\"ddr_w\":%lu,\"ddr_r\":%lu,"
+            "\"aw0_cycle\":%ld,\"aw1_cycle\":%ld,\"r_start\":%ld,\"r_mid\":%ld}\n",
+            geo_conf, geo_len, (unsigned long long)geo_dst_off,
+            (unsigned long)n_ddr_ar, (unsigned long)n_ddr_aw,
+            (unsigned long)n_ddr_w, (unsigned long)n_ddr_r,
+            geo_aw_cycle[0], geo_aw_cycle[1], geo_r_start, geo_r_mid);
+        std::printf("geometry-smoke: PASS\n");
+    }
@@ rising edge
         if (f_ddr_aw) {
             ++n_ddr_aw;
+            if (geo_smoke) { if (geo_aw_n < 8) geo_aw_cycle[geo_aw_n] = g_cycle; ++geo_aw_n; }
             CawEnt c; c.id = s_ddr_aw_i; c.addr = s_ddr_aw_a;
             c.len = (uint8_t)s_ddr_aw_l; c.got = 0;
             caw_q.push_back(c);
         }
@@ rising edge
         if (f_ddr_r) {
             ++n_ddr_r;
             MemRead& h = mem_q.front();
             CHECK(h.port == P_DDR, "R handshake on an unexpected port");
+            if (geo_smoke) {
+                if (h.len == 255 && h.beat == 0   && geo_r_start == 0) geo_r_start = g_cycle;
+                if (h.len == 255 && h.beat == 128 && geo_r_mid   == 0) geo_r_mid   = g_cycle;
+            }
             if (h.beat == h.len) mem_q.pop_front();
             else                 h.beat++;
         }
@@ int main(int argc, char** argv) {
     bool        owner_only = false;
+    bool        geo_smoke   = false;
+    uint32_t    geo_conf    = 0;
+    uint32_t    geo_len     = 2048;
+    uint64_t    geo_dst_off = 3072;
 #ifdef ACTUAL_DMA_SYSTEM
@@ for (int i = 1; i < argc; ++i) {
         if (const char* v = val("owner-only")) { /* ... unchanged ... */ }
+        if (const char* v = val("geo-smoke"))   geo_smoke   = (std::strtoul(v, nullptr, 0) != 0);
+        if (const char* v = val("geo-conf"))    geo_conf    = (uint32_t)std::strtoul(v, nullptr, 0);
+        if (const char* v = val("geo-len"))     geo_len     = (uint32_t)std::strtoul(v, nullptr, 0);
+        if (const char* v = val("geo-dst-off")) geo_dst_off = std::strtoull(v, nullptr, 0);
@@ strict validation
     if (!(ptw_lat == 31 || ptw_lat == 100)) {
         std::fprintf(stderr, "nonblock_actual_dma_test: +ptw-latency must be 31|100\n");
         return 2;
     }
+    if (geo_smoke && (geo_conf > 1 || geo_len == 0 || (geo_len % 8) != 0)) {
+        std::fprintf(stderr,
+            "nonblock_actual_dma_test: geo-smoke needs conf<=1 and len multiple of 8\n");
+        return 2;
+    }
@@ try {
         tb.owner_only      = owner_only;
+        tb.geo_conf        = geo_conf;
+        tb.geo_len         = geo_len;
+        tb.geo_dst_off     = geo_dst_off;
@@ try {
         tb.policy_partition= policy;
-        tb.run_scenario(mode.c_str(), enable);
+        if (geo_smoke) {
+            tb.geo_smoke = true;
+            tb.run_geometry_smoke();
+        } else {
+            tb.run_scenario(mode.c_str(), enable);
+        }
```

**注**：`read64` 在 `0x80001C00..0x80002400` 全部落在 `setup_memory` 已初始化的 `V_DST`/`V_COLD_DST` 页内（`:490-494`），无需新增页表或内存。**未新增共享 PTE/payload 池、未加 ideal oracle、未加写延迟、未造弱基线。**

---

# Q4：G 与“准确 greedy”无法区分的条件，及 G/H 是否被当前 W‑head 直通涵盖

## 4.1 符号与规则（按文档 `translation-lane-...md:43-52`）

- `Lmax`：合法源读上限（legalizer 段边界，`idma_legalizer.sv:82-83`）。
- `p`：目的译址 + 虚拟/物理写顺序 + 独立写服务共同保证**可连续消费**的长度（文档明示“TLB 命中或瞬时 WREADY 不够”，`:45`）。
- `C` = `free_credit`（可回收 payload 字节）。
- **G**：在虚拟 AR 首次 VALID 前选择并冻结 `L`，满足 `max(0,L−p) ≤ C`（`:46`）；且策略为 `p≥Lmax ⇒ L=Lmax`，`0<p<Lmax ⇒ 先切在 p 处`（`:47`），`p=0 ⇒ 安全整返回预约回退`（`:47`）。
- **greedy**：`L_g = min(Lmax, p + C)`（文档自述“普通最长可行 L=min(Lmax,p+free_credit)”，`:50`）。

## 4.2 不可区分的条件（命题）

**命题 1**：`L_G = L_g` 当且仅当 `C=0` 或 `p ≥ Lmax`。
- 证明：若 `p≥Lmax`，则 `L_G=Lmax=L_g`；若 `C=0` 且 `p<Lmax`，`L_G=p=min(Lmax,p)=L_g`；若 `0<p<Lmax` 且 `C>0`，`L_G=p < min(Lmax,p+C)=L_g`，不相等。
- 这正是文档 `:50` 的“当 credit=0 或 p 已覆盖 Lmax 时…一致”。

**命题 2**：`0<p<Lmax` 且 `C>0` 时 G 严格更保守（`L_G<L_g`），即“决定不同”（文档 `:50`），但**只体现在读得更少**。

**命题 3（被当前 W‑head 直通涵盖）**：pool PoC 的 `can_drain`（`nonblock_completion_pool_late.sv:94-106`）正是“当前虚拟/物理 W‑head 匹配 + 对应 AW 已 prepared”的直通判据；`frontier_drain`（`:146-169`）是其“完整前缀”变体，`future_drain`（`:108-124`）是后继变体。既然 `p` 本身就取自当前 W‑head 的**可 drain 前缀**，则 greedy 的 `L_g=min(Lmax,p+C)` 已在该直通的可达集内；G 的决定集 `⊆` greedy，因此 **G 不产生直通无法达到的独有决定**——在 `C=0`/`p≥Lmax` 时二者同一，在 `0<p<Lmax,C>0` 时 G 只是子集。故“G（及 H）可能被普通当前 W‑head 直通涵盖”。

**命题 4（H）**：文档 `:52` 述 H = 仅当存在具体跨 copy 依赖时，为另一 copy 当前 W 义务保留最小进展资源。若该受限/对齐场景中当前 W‑owner 已满足 `p>0`（即“普通切短 + 直通”已能进展），则 H 需要的最小保护量为 0、且无独有决定 ⇒ H 与直通等价。这也是文档自述“H 是未证候选”。

## 4.3 明确不主张

- 未实现、未运行、未证明 G/H 有非零收益或创新成立。
- 以上命题 3/4 依赖：(i) 对 G 策略文本的读法（“先切在 p 处”）——正式的 G 草案 `codex-new-rule.md` 未提供；(ii) `p` 的精确定义（必须排除 TLB 命中/瞬时 WREADY）；(iii) “准确 greedy”与 G 使用同一 `p` 与同一信用语义。缺任一则该等价论断不成立。

---

# 验收命令（草案，需按实际构建系统校正）

```bash
# 1) 仅重编 C++（不改生产 SV）。以现有 runner tests/test_nonblock_actual_dma.py
#    重链 Vnonblock_actual_dma_system_top（该 runner 已在源码注释中被引用：
#    nonblock_actual_dma_test.cpp:2523）。确保构建 SharedPoolMode=0（E2 Mode 0）。
#    ——若现有 system binary 非 Mode 0，则本 smoke 前置不成立，须先重建。

# 2) CONF=0 运行（manual，无 batch，无 pool）：
<build>/Vnonblock_actual_dma_system_top +mode=s +warmup=0 \
    +geo-smoke=1 +geo-conf=0 +geo-len=2048 +geo-dst-off=3072 \
  | tee geo-conf0.log

# 3) CONF=1 运行：
<build>/Vnonblock_actual_dma_system_top +mode=s +warmup=0 \
    +geo-smoke=1 +geo-conf=1 +geo-len=2048 +geo-dst-off=3072 \
  | tee geo-conf1.log

# 通过条件（建议断言）：
#  - 退出码 0；日志含 "geometry-smoke: PASS"；
#  - GEO-SMOKE-JSON .ddr_aw == 2（两段写）；
#  - 无 "SCENARIO-FAIL" / "not quiet" / "E2 pool fault"；
#  - aw1_cycle 与 r_mid 比较（仅报告，不作自动“创新”判定）。
```

---

# 假设、未决与最小所需源

1. **CONF bit0=decouple_rw 未证实** → 需 `idma_reg64_frontend_reg_pkg.sv`（引用于 `nonblock_actual_dma_test.cpp:95`）。若 bit0 实为 `decouple_aw`，Q3 的两次运行语义需改写。
2. **E2 Mode 0 前提**：pool 结构上只支持 4096 对齐、长度 4096 的 job（`nonblock_completion_pool_late.sv:402-405`），且 Mode 为编译期参数（`nonblock_actual_dma_system_top.sv:80,91`）。须确认现有 system binary 的 `SharedPoolMode`。
3. **Q1 完整跟踪缺失源**（见 §1.3）：iDMA `manager/midend`、R→AW coupler/backend/data path、`nonblock_actual_dma_top.sv`、翻译引擎、以及（若存在）`dma_core_wrap.sv`。
4. **未运行任何仿真/未改任何 RTL**；patch 未经编译验证，`GEO-SMOKE-JSON` 字段名与格式为草案。
5. 未新增共享 PTE/payload 池、未加 ideal oracle、未加写延迟、未造人造弱基线（遵守约束）。
