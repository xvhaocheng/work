# 可行性/证据整理（只读草案，未执行任何代码、未复算、未构建、未上板）

所有数值均为**引用**或**手工核对示例**（我会明确标出哪几处），本文不宣称任何复算/测试运行结果。工具执行与最终裁决仍需 Codex。

---

## 0. 结论摘要（可行性判断）

1. **现有 172 只是功能/契约验收**，其自身文件明写 `performance_or_innovation_claim: false`、`full_board_IO_CDC_signoff: false`（`out/composition-ptwfix-20261005/board-acceptance.json:29-30`）。它**不能**证明性能收益、机制创新或跨 boot 稳定性。
2. **B（配置选择/适用边界）有一个"描述性 first result"雏形**：同一 boot、同一 BIT、同内核下，PN 相对 N 在 (A 页数, V gap) 格点上的符号翻转（`docs/performance-council-decision-20261005.md:23-28`）。但这是 **单 boot 内 3 段**，且多数单元的原始计数跨段完全相同，更像"1 个独立观测 + 计时抖动"，不是 n=3 独立重复。
3. **A（页面工作集感知的 DMA 提交控制）没有任何可直接支撑的已采数据**：冻结 ABI 里只有一个静态 ioctl 配置 + 固定 Vgap，不存在反馈/闭环提交变量（`docs/composition-v1-spec-20261004.md:44-45`；`docs/performance-council-decision-20261005.md:35`）。
4. **工作负载维度不足以刻画"页工作集"**：V 页数固定 1，A 页数只有 {0,1,8,16}，A gap 固定 0，chunk 固定 4096，无页身份/复用距离/访问序列控制（`scripts/composition_plan.py:34-35, 415-416`；spec:44）。
5. **资源数字全部不能当前使用**：89.80% 是**失败构建**的 Fully Placed 数字；99.45% slice / 80.31% LUT 是 **composition-trim** 的 Routed 数字；当前是 **composition-ptwfix**，其资源报告**不在本轮材料中**，且布线线数已从 273893 变为 276241，设计不同（`docs/composition-routing-cost-20261005.md:7,9,51`；`docs/composition-trim-20261005.md:39-41`；`docs/composition-v1-mac-20261004.md:10`）。

---

## 1. 事实表：172 验收「覆盖什么」

| 项 | 值 | 证据（file:line） |
|---|---|---|
| planned / attempted / completed / failed / unrun | 172 / 172 / 172 / 0 / 0 | `board-acceptance.json:3-7` |
| 分阶段 | short 16、noise 12、explore 144 | `board-acceptance.json:8-12` |
| 契约问题列表 | `[]` | `:13` |
| taint 全 0 | true | `:14` |
| V 及在场 A 数据错误全 0 | true | `:15` |
| solo（无 A）用例数 | 40 | `:16` |
| restore_status 全 0 | true | `:17` |
| measurement complete | true | `:18` |
| 新故障/内核告警 | `[]` | `:19` |
| **dmesg_prefix_preserved** | **false** | `:20` ← 需 Codex 解释该字段门控什么 |
| runner_exit | 0 | `:21` |
| boot_id | `2695d848-8589-491e-bf7f-1de7af5ea9cb`（**单一**） | `:22` |
| 成功 PN 诊断后同一 boot | true | `:23` |
| source 与已构建 DS/PTW 匹配 | true | `:24` |
| 完整板级 IO/CDC 签核 | **false** | `:29` |
| 性能或创新主张 | **false** | `:30` |

**计数自洽核对（手工，非代码）**：40 solo = short 4 模式 × a0 (4) + explore 3 block × 3 gap × 1 × 4 模式 (36) ✓；16+12+144=172 ✓；12 = 6 pair × 2 slot（`scripts/composition_plan.py:151-155`）✓。

### 1.1 172 验收「不能证明什么」

| 不能证明 | 证据 |
|---|---|
| 性能收益 / 机制创新 | `board-acceptance.json:30`；`docs/composition-v1-mac-20261004.md:81` |
| 跨 boot 稳定性 | spec:48「首批单boot，不冒充跨boot稳定性」 |
| 三次独立启动 | `docs/performance-council-decision-20261005.md:21`「不能当三次独立启动」 |
| 完整板级 IO/CDC 签核 | `board-acceptance.json:29`；`composition-v1-mac-20261004.md:10` |
| payload/SD 身份哈希 | `composition-v1-mac-20261004.md:177`（`fw_payload.bin` sha256 = null）；`docs/composition-handoff-20261005.md:15`（`written_not_readback_verified`） |
| A 提交控制 / B 选择规则本身 | `CURRENT_STATUS.md:3`「A提交控制、B配置选择目前仅为入口…不等于突破」 |
| 未见验证集 | `performance-council-decision-20261005.md:71`「当前172条均已看过，只能回溯分析，不能重命名为未见验证集」 |

---

## 2. 事实表：CSV 里有哪些指标、哪些能作 A/B first result

CSV：`out/paper-council-20261005/metrics.csv`（表头 1 行，数据 172 行，line 2–173）。

| CSV 列 | 语义 | raw 对应 | 可作 first result？ |
|---|---|---|---|
| `v_window_cycles` | (open, close] 窗口周期 | `window[1]`（`composition_plan.py:471`） | **B 可用**（配合 r_A）；**不能**当 V 延迟/吞吐 |
| `a_completed_jobs_window` | 同窗口 A 完成 job 数 | `window[3]`（`:472`） | 只能作分子；跨单元**不可直接比** |
| `v_actual_gap_cycles` | 实际 gap 占用 | `raw[24]`（`:473`） | 用于说明窗口含故意等待 |
| `raw_v_lookup/hit/miss` | V 首次 lookup | `raw[12..14]` | B 可用（机制侧描述） |
| `raw_a_lookup/hit/miss` | A 首次 lookup | `raw[15..17]` | B 可用（A 侧代价） |
| `raw_v_fast` / `raw_a_fast` | fast accept | `raw[21..22]` | B 可用 |
| `raw_*_fill` | 填充事件 | `raw[26..27]` | B 可用 |
| `raw_replace_pscid` | 被替换有效条目 PSCID≠新条目 PSCID 的事件数（**不是 DID 淘汰计数**） | `raw[29]`（spec:28） | 谨慎 |
| `raw_inval_cycles` | 规范失效周期数 | `raw[31]` | 本数据全 0 |

### 2.1 只在 `record.json`、**未进 CSV** 的指标（B 的可用增量）

`window flags[6] / sequence[7] / cycles[8] / errors[9] / ARM config[10] / ARM generation[11]`、`PLRU before/after[36,37]`、`V/A PSCID[38,39]`、`entries before[64-95] / after[96-127]`（`scripts/composition_plan.py:72-113`）。
→ 这些是"替换历史/条目状态"的直接证据，**不用上板、不用重跑**，纯离线从已归档 record 就能提取。

### 2.2 定义建议（离线可算；本轮不执行）

```
W_V   = v_window_cycles          = window[1]        # 含 V 每 chunk 的故意 gap
A_c   = a_completed_jobs_window  = window[3]
r_A   = A_c / W_V                                   # 同窗 A 有效完成率 [job/cycle]
ΔW_V(m2 vs m1) = W_V(m2)/W_V(m1) - 1
Δr_A(m2 vs m1) = r_A(m2)/r_A(m1) - 1
恒等式：A_c ≡ r_A · W_V            （必须成对报告 W_V 与 r_A）
可选补算：A 窗内 miss 率 = raw_a_miss/raw_a_lookup
          V fast 占比  = raw_v_fast/(raw_v_lookup + raw_v_fast)
```

**禁止用法（spec/plan 明确）**：
- 用 `W_V` 当 V 延迟或吞吐（spec:48「V窗口包括故意等待」）；
- 用 `W_V − v_actual_gap` 当服务延迟（spec:48；`composition_plan.py:475`「subtracting the nominal gap is not service latency」）；
- 用 `A_c` 跨单元直接比较（A 是连续 aggressor，窗口长度不同）。

### 2.3 手工核对示例（**未执行代码**，仅为定义可行性验证）

以 A8/g0、PN vs N 为例，三 block（CSV 行 39/41、86/88、137/135）：
- b1：W_V 9999550 vs 8611879 → +16.1%；r_A 4096/9999550 vs 4096/8611879 → −13.9%
- b2：9996821 vs 8613192 → +16.1%；→ −13.8%
- b3：10053347 vs 8642182 → +16.3%；→ −13.9%

与 `performance-council-decision-20261005.md:23-28` 公布的区间（+16.1%–16.3% / −13.8%–14.0%）**手工核对一致**。同法手核 A16/g0（−1.2% / +1.2%）与 A8/g4000（−2.9%–3.2% / −31%）亦一致。
→ 说明：文档那三行数字确实可由上述 `W_V / r_A` 定义从本 CSV 重现；**但本轮我没有运行任何脚本**，Codex 应把该定义固化成可复算脚本并存档。

### 2.4 结构发现（A/B 定义的关键前提）

- **V 的 lookup 计数 + fast 计数 = 4 × transfers**。例：行 39 `raw_v_lookup 8484 + raw_v_fast 7900 = 16384 = 4×4096`；行 41 `10240 + 6144 = 16384`；行 44 `8191 + 8193 = 16384`。
- **A 的 `raw_a_lookup/4 ≈ a_completed_jobs_window`**。例：行 66 `98284/4 = 24571` = `a_completed`；行 71 `73471/4 ≈ 18368`；行 70 `56162/4 ≈ 14041`。
- ⇒ **A 不是固定工作量，而是随窗口长度连续完成**。因此 `a_completed_jobs_window` 单列不可比，必须归一化为 `r_A`。
- ⚠️ **但 A 侧的 workload 语义（发多少个 A chunk、是否多轮、job 定义）不在冻结计划里**：`composition_plan.py` 只给 `a_pages`，从未定义 A 的 transfers 数。这是 A 题目的**定义级缺口**，需 Codex 从 driver/CLI 源码确认。

### 2.5 3 个 block 更接近"1 个观测 + 抖动"（手工核对）

| 单元 | b1 | b2 | b3 | 行号 |
|---|---|---|---|---|
| explore-bX-g0-a8-**mn** `raw_a_hit/miss` | 13308/3072 | 13308/3072 | 13308/3072 | 41/88/135 |
| explore-bX-g0-a8-**mpn** `raw_a_hit/miss` | 9802/6582 | 9802/6582 | 9802/6582 | 39/86/137 |
| explore-bX-g0-a8-**mp** `raw_a_hit/miss` | 11267/5117 | 11267/5117 | 11267/5117 | 38/89/136 |
| 对应 `v_window_cycles`（mn） | 8611879 | 8613192 | 8642182 | 41/88/135（±0.4%） |

**反例（计数确实变化的单元）**：
- `explore-bX-g0-a8-ms`：b1 V hit/miss = 16384/0，**b2 = 16382/2**，b3 = 16384/0（行 40/87/134）。
- g4000 的 A 计数跨 block 差异更大：`explore-bX-g4000-a8-mn` a_hit = 61074 / 61193 / 61069（行 71/118/169）。

⇒ 结论：**N/P/PN 在 g0/g1000 的计数器基本确定性重复，S 及 g4000 有偏差**；3 block 的"重复"主要是计时抖动，不能当 3 次独立测量。这条对 B 的"first result"强度是硬约束。

---

## 3. 单 boot 三 block vs 跨 boot / 功能 vs 性能 / V 窗口含 gap

| 疑问 | 事实 | 证据 |
|---|---|---|
| 单 boot 三 block | 172 formal01 只有一个 `boot_id`；explore 的 b1/b2/b3 是**同一 boot 内三段** | `board-acceptance.json:22`；`performance-council-decision-20261005.md:21` |
| 跨 boot | **无任何跨 boot 证据** | 同上；`CURRENT_STATUS.md:7` 亦明写「未新增实验」 |
| 历史多 boot 轮次 | net3（52/1/119）与 DS-only 轮次**明确未并入** | `composition-v1-mac-20261004.md:13` |
| 功能 vs 性能 | 172 = 契约校验（schema/ABI/configured/raw_captured/generation 递增/window frozen/lookup 守恒/gap 上下界/N 关闭无 fast/A 存在性/V issued-done==transfers） | `scripts/composition_plan.py:224-452`，特别 `:395-397,399-407,418-421,424-429` |
| 性能研究 | 未做 | `board-acceptance.json:30`；`composition-v1-mac-20261004.md:81` |
| V 窗口含 gap | `window = (open, close]`，open 边沿不计、close 边沿计；含 V 每 chunk 故意 gap | spec:40, 48；`composition_plan.py:475` |
| 量化 | g1000 全部为 `v_actual_gap = 4,095,000`（≈43% 窗口）；g4000 全部为 `16,380,000`（≈75–82%） | CSV 行 46–61、62–77 等 |
| 推论 | **跨 gap 的 `W_V` 不可比**；**同 gap 内 gap 恒定**，故同 gap 内 mode 间 ΔW_V 干净 | 手工核对：g1000 各行 v_actual_gap 恒为 4095000 |
| `r_A` 是"同窗有效完成率" | 非全程吞吐：A 在窗口外仍在跑 | `composition_plan.py:474`「A jobs completed inside the (open,close] window, not the whole round」 |

---

## 4. 工作负载维度是否足够页工作集 / 反馈控制

| 维度 | 取值 | 证据 |
|---|---|---|
| V 页数 | **固定 1**（`Vpages1`） | spec:44 |
| A 页数 | {0, 1, 8, **16**}（含 solo 0） | `composition_plan.py:34` |
| V gap | {0, 1000, 4000} | `composition_plan.py:35` |
| A gap | **固定 0**（validator 拒绝非 0） | spec:44；`composition_plan.py:415-416` |
| chunk | **固定 4096** | spec:44 |
| explore transfers / warmup | 4096 / 256（固定） | `composition_plan.py:43-44` |
| noise | 仅 **MODE_S**、A 页固定 8、6 pair×2 slot | `composition_plan.py:151-155` |
| short | transfers 16、warmup 16 | `composition_plan.py:37-38` |
| 反馈/闭环变量 | **不存在**：提交是"完成 → gap → 下一项"；mode 是每 ioctl 一次静态配置 | `performance-council-decision-20261005.md:35`；spec:44-45 |

**判断**：
- A 页数 {1,8,16} 只是一个**粗"页数"轴**，没有页身份、复用距离、访问序列、pin/冲突模式的显式控制；V 侧**完全没有工作集轴**。CSV 确实能看到工作集效应（A=1 时 `raw_a_miss=0`，行 34-37；A=8/16 时出现 3072–7680 miss，行 38-45），但这只能支撑"页数影响冲突 miss"的定性，**不足以支撑"页工作集感知的提交控制"**。
- 不存在任何反馈控制量（在途上限/批量/页预算的运行时调节）。**A 目前无可支撑 first result 的数据。**
- B 能拿到的是一个 **4 mode × 4 A页 × 3 gap = 48 格点、单 boot、每格点 3 段（≈1 独立观测）** 的粗边界网格。

---

## 5. 旧资源/时序证据的时效性

| 数字 | 来源/阶段 | 能否当"当前最终资源" | 证据 |
|---|---|---|---|
| LUT 183014/203800 = **89.80%**、Slice 50949/50950 | `out/migration-4dma-composition`，**Fully Placed**，对应**失败**的完整构建（31 overlap、WNS −3.268） | **不能** | `docs/composition-routing-cost-20261005.md:7,9`；`docs/composition-trim-20261005.md:7-11` |
| 旧 N v5 176476 LUT / 141170 FF | **Routed**，阶段不同 | **不能**用于拆增量 | `composition-routing-cost-20261005.md:11` |
| Synth 172284 → 152609（−11.42%） | **Synthesized** 同阶段对比 | **不能跨阶段外推** | `composition-routing-cost-20261005.md:47` |
| Routed LUT 163673/203800 = **80.31%**、Reg 133604、**occupied Slice 50670/50950 = 99.45%**、WNS +0.058 | `out/composition-trim-20261005/route-opt`（2026-10-05 03:06） | **已被 ptwfix 取代**，不能当当前数字 | `composition-trim-20261005.md:39-41`；`composition-routing-cost-20261005.md:51` |
| **当前** BIT：WNS +0.031 / WHS +0.019 / WPWS +0.062；**276241/276241** 布通 | `out/composition-ptwfix-20261005/route-opt`（2026-10-05 09:36 验收） | 这才是当前时序；**但 ptwfix 的 LUT/slice 资源报告不在本轮材料中** | `composition-v1-mac-20261004.md:9-10`；`board-acceptance.json:31` |

**要点**：trim 的 occupied Slice 已 99.45%（几乎满载），布线线数 273893；ptwfix 为 276241 线，**设计不同**。因此：
- 不能用 89.8% 判断余量；
- 也不能用 trim 的 99.45% 判断"新逻辑能不能放"；
- **必须取 ptwfix 自己的 routed utilization 报告**才能谈余量。旧 `placed` 报告的 Slice 近满本身也是强烈警告信号（`composition-routing-cost-20261005.md:9`）。

### 5.1 还应查的"最新构建/版本对应"文件

1. `iommu-platform/out/composition-ptwfix-20261005/acceptance.json` 及其 `route-opt/` 下的 routed utilization / timing report ← **取当前 LUT/FF/Slice 的唯一来源**（`composition-v1-mac-20261004.md:10`）
2. `iommu-platform/out/composition-ptwfix-20261005/targeted-test-acceptance.json`（7/0 定向测试，`composition-v1-mac-20261004.md:11`）
3. `iommu-platform/out/composition-ptwfix-20261005/mac-records/results/composition-ptwfix-20261005-formal01/summary.json`（`board-acceptance.json:25`）
4. `iommu-platform/sources.lock.json` + `git rev-parse HEAD` ← 版本对应（`CURRENT_STATUS.md:329`）
5. Mac 端 `tools/composition-v1-20261004/composition_plan.py` **vs** 仓库 `iommu-platform/scripts/composition_plan.py`（是否同一份；`composition-v1-mac-20261004.md:175-182`）
6. `iommu-platform/out/composition-trim-20261005/synthesis-resource-comparison.json`（阶段对比表，`composition-routing-cost-20261005.md:47`）

---

## 6. A / B 最低新增数据 与 受限外推

> 按用户要求，**以下只是缺口登记，不是本轮的重跑/编译/上板建议**；本轮不执行任何一项。

### 6.1 B（配置选择/适用边界）

| 层级 | 需要什么 | 现有是否已有 |
|---|---|---|
| 最低（**纯离线，零新采集**） | 从已归档 172 个 `record.json` 提取 `W_V / r_A / raw_* / PLRU 前后 / entries 前后`，产出 (mode × A页 × gap) 的 ΔW_V、Δr_A 表 + 符号边界图 | ✅ 数据已在 `formal01-complete.tar` |
| 升级为"规则"所需 | ≥2 个**独立 boot** 的同一格点重复 + 事前冻结的判据（不是重跑全套 172；只需 explore 子集） | ❌ 完全缺失；**本轮不建议执行** |
| 升级为"可迁移解释"所需 | 格点外的插值点（如 A 页数 2/4/12）、V 页数 >1、A gap ≠ 0 | ❌ 缺失 |

### 6.2 A（页面工作集感知的提交控制）

| 需要什么 | 现有 |
|---|---|
| 显式页工作集定义（页集合大小、复用距离、访问序列、pin 策略），且 V 侧也有工作集轴 | ❌ 只有 V=1、A∈{0,1,8,16} |
| 一个**提交控制变量**（在途上限 / 批量 / 页预算 / 反馈信号）与其 ABI | ❌ spec:44-45 只有静态 mode + Vgap |
| 闭环/反馈测量 | ❌ |
| A 侧 workload 语义的明确定义 | ❌ 冻结计划未定义 A 的 transfers |

### 6.3 受限外推（允许 / 不允许）

**允许**：
- 在本 boot、本 BIT、本内核、chunk=4096、Vpages=1、Agap=0、Apages∈{1,8,16}、gap∈{0,1000,4000} 的**已测格点**上，描述 PN 相对 N 的 (W_V, r_A) 差异。
- 把 3 段当"1 个观测 + 计时抖动的稳健性指示"。
- 用 `raw_*` 计数说明 A 侧代价方向（如 PN 下 `raw_a_miss` 由 3072 升到 6582，行 41 vs 39 —— 手工核对，三块一致）。

**不允许**：
- 跨 boot / 跨 BIT / 跨内核外推；
- 页大小、chunk、V 工作集、A gap 变化后的外推；
- "PN 更差"（A16/g0 方向相反）或"A8 的负向可推广"；
- 把 89.80% 或 99.45% 当当前资源，或据此保证新逻辑可放；
- 把 172 当未见验证集（`performance-council-decision-20261005.md:71`）；
- 把 B 的描述当"选择规则"、把 A 说成已有 first result；
- 任何发表保证。

---

## 7. 现有原始结果证据路径

| 内容 | 路径 |
|---|---|
| 派生指标表（172 行） | `iommu-platform/out/paper-council-20261005/metrics.csv:1-173` |
| 每用例原始记录 | `iommu-platform/out/composition-ptwfix-20261005/mac-records/results/composition-ptwfix-20261005-formal01/cases/<case-id>/record.json`（CSV `source` 列，每行都有） |
| 汇总 | `.../composition-ptwfix-20261005-formal01/summary.json`（`board-acceptance.json:25`） |
| 原始归档 | `mac-records/formal01-complete.tar`（`board-acceptance.json:26`） |
| 板级验收 | `iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json:1-32` |
| 冻结计划 + 校验器 + 窄窗指标 | `iommu-platform/scripts/composition_plan.py:34-35,143-178,224-452,454-479` |
| 冻结 ABI/窗口语义 | `iommu-platform/docs/composition-v1-spec-20261004.md:4,40,44-48` |
| 模式/指标叙述与三行结论 | `iommu-platform/docs/performance-council-decision-20261005.md:21-29,33,35,65,71` |
| 当前操作入口/内核/BIT | `iommu-platform/docs/composition-v1-mac-20261004.md:9-13,17,80-81` |
| 资源/阶段对比 | `iommu-platform/docs/composition-routing-cost-20261005.md:7-15,45-51`；`iommu-platform/docs/composition-trim-20261005.md:7-11,30-41` |
| 历史转交（失败构建） | `iommu-platform/docs/composition-handoff-20261005.md:7-17,39` |
| 总状态 | `CURRENT_STATUS.md:3,7,44,54,329,331` |

---

## 8. 需 Codex 直接核的清单（我无法执行）

1. **ptwfix route-opt 的 routed LUT/FF/Slice**：唯一能判断"新逻辑还能不能放"的数字，本轮材料缺失。
2. **A 侧 workload 语义**：A 到底发多少个 chunk、是否多轮、`a_completed_jobs_window` 的 job 定义 —— 决定 `r_A` 的解释力。我只用 `raw_a_lookup/4 ≈ completed` 做了手工一致性推断，需 driver/CLI 源码确认。
3. **`record.json` 的 raw[36..39]、raw[64..127]**（PLRU 前后、V/A PSCID、16 条 entry 前后）是否 172 份都存在且自洽 —— 这是 B 的机制侧证据，CSV 未携带。
4. **`dmesg_prefix_preserved=false`** 的含义与影响（`board-acceptance.json:20`）。
5. **Mac 部署的 `composition_plan.py` 与仓库版本是否逐字节一致**（`composition-v1-mac-20261004.md:175-182`），以及 `DELIVERY.json` 中 `fw_payload.bin` sha256=null 带来的身份松绑（`:177`）。
6. **`sources.lock.json` / git rev 与 ptwfix BIT 的对应**（`CURRENT_STATUS.md:329`）。
7. **文档三行数字的生成脚本**：目前只有结果 CSV 与文字，无固化脚本；建议 Codex 把 §2.2 定义落成一个只读离线工具并存档（本轮不执行）。
8. **g4000 单元跨 block 的 A 计数差异**（行 71/118/169 等）是否只是"窗口切点不同"，还是存在相位/触发差异 —— 影响 B 的稳健性结论。

---

## 9. 不确定 / 未决

- 我未执行任何代码；§2.3、§2.4、§2.5 的数值均为**手工核对示例**，不是复算。
- `a_completed_jobs_window` 在多处 > 4096（如 8191、12286、24571，行 51/76/66）说明 A 是连续流；但其与 V 的相位关系、以及是否会跨越 V 窗口边界，材料里无定义。
- 三题（A/B/其他）的**创新性仍未成立**：`performance-council-decision-20261005.md:69`（KPart 已联合考虑缓存分配与带宽争用）、`:71`（论文门槛）与 `CURRENT_STATUS.md:3` 均未确认创新。本草案**不提供发表可行性保证**。