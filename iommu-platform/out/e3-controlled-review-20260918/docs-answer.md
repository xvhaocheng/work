docs/e3-controlled-results-review.md
````
<!-- docs/e3-controlled-results-review.md -->
# E3 v6 受控运行：服务器复核结果（定稿）

**结论：复核通过（exit 0）。** 服务器上 `server_review_v6.py` 对本轮原始数据跑出的 `review-summary.json` 为 `status: passed`，9 项 checks 全部 `true`，`structural_errors`、`warnings`、`anomalies` 全为空：11 对 / 22 轮、418 条唯一 UART 观测行（期望也是 418）、114 条完全相同重复去重、11 份完整 CLI JSON 与 UART 对象逐字段一致、输入哈希前后未变。8 项 mutation 检查（1 项正常对照 + 7 项故障注入）全部符合预期。原始结果一律未改，全部不利结果一并保留。

分工说明：本轮测试由 **DeepSeek** 编写；**Codex** 负责审查、应用与执行，核对关键 RTL 与原始字段，并做最终科学决策。本轮没有重新综合、没有改 RTL / 驱动 / CLI、没有上板重跑。

> 早先那份失败草稿（`review-draft.json`）是**分析脚本自身**的解析与错误汇总缺陷造成的，**不是** FPGA 或板级运行失败。脚本缺陷已修正，修正后一次跑通。

## 1. 证据与完整性

| 输入 | 预期关系 | 结论 |
|---|---|---|
| `all-original-pairs.json` | 11 份 `idma.e3_controlled.v1` 配对文档 | 通过（pair checks） |
| `console.raw.log` | 22 个唯一 sequence、418 条唯一观测行、累积 dmesg（short 段 seq 1–6，main 段 seq 1–22） | 通过 |
| `console.readable.log` | 与 `console.raw.log` 只差 1 个 NUL 被替换为 `[NUL]` | 通过（`nul_bytes_in_raw = 1`） |
| `run.json` | 次级汇总，脚本独立重算后比对 | 通过（差异会作为结构性错误传播） |

- short 段与 main 段重复了 seq 1–6，所以 **114 条完全相同重复是预期的**，去重后剩 418 条。
- 输入哈希在复核前后一致；mutation 脚本也会在前后对真实输入取哈希，一旦变化就拒绝报成功。
- 所有输入只读打开，评审只在 run 目录之外写 1 个新文件。

## 2. 实际执行事实

- 11 对 / 22 轮，每对 2 轮，sequence 1–22。
- 14 轮触发（全部 cold），8 轮未触发（Solo、两组 hot 及其 D0/D128 伴跑轮）。
- 未触发轮不写任何零时间替代：identity / milestones / readings 保持 `null`，并给出明确的 `no_trigger` 理由。
- console 上打印的 11 份 CLI JSON 全部被解析，并与原始配对文档逐字段比对（256 个 `e3_words`、32 个 launcher word、prep 统计、逐轮统计与映射、uint64 保真），数量和顺序都对上才算通过。

## 3. 机制结论（范围受限）

> **在当前实现、当前配置、当前到达方式下，被延长的 A 侧 S1 PTW 响应会传导到一个已在等待的 V read，后者随后首次命中：A 的 miss 和 V 的 hit 被同一个串行控制器 / AR 队列排在了后面。**

精确措辞用“本实现的 A-miss 引起的 V-hit 阻塞，发生在共享的串行控制器 / AR 队列上”；**不**说“AXI 规范强制如此”。

支撑该结论的关键 RTL（Codex 直接核对）：

- `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_e3_delay.sv` 行 20–60：PTW-only 路径上只缓冲 1 个响应，每次 ARM 只捕获一次，128 计数只作用于被选中的那个响应。
- `rv_iommu_tl_wrap.sv` 行 590–616：要求 DID 11、S1 miss、无 S2/DDT/PDT/MSI，且 V 正在等待。
- `transaction_controller/rv_iommu_trans_ctl_wrap.sv` 行 140–157：AR FIFO 头只在 `ar_ready` 时前进；行 616–716：只有 IDLE 才做 select，AWAIT_RESP 保持当前请求，COMPLETE 要等输出地址被接收后才回 IDLE。同一个控制器的 AW 优先级把 V write 插在 A read 与 V read 之间。
- FPGA 侧 `dma_e3_observer.sv` 行 246–251：`e41`/`e73` 记的是**翻译响应**，不是数据返回（数据返回是 `e43`/`e75`，行 175–185）；行 254–273 记 trigger + V-pending + prefix。

观察到的常数 9 与“输出接收 +2”支持“串行服务式等待”这一解释，但**不能**证明所有下游各级自始至终都 ready。

## 4. 本轮观察到的数字（只作事实报告，不作通过门槛）

14 轮 cold 触发轮上：

- `e70 − e41 = 9`，**全部 14 轮**。
- `e42 − e41 = 2`，全部 14 轮。
- `e73 − e71 = 4`（V 自身翻译服务未被改变），全部 14 轮。
- `e41 − e4`：D0 轮为 271–274，D128 轮为 399–401。

这些只是**本轮观察到的规律**，只报告，绝不当作资格条件或通过条件，也没有定义任何容差；不是普适常数。

### 4.1 cold 互补对的 D128 − D0

| 配对（main case） | ΔA remaining | ΔV remaining | ΔV 完整排队 | ΔV 硬件周期 |
|---|---|---|---|---|
| 5（case 2） | +127 | +127 | +129 | +130 |
| 7（case 4） | +129 | +129 | +136 | +134 |
| 9（case 6） | +129 | +129 | +129 | +129 |
| 11（case 8） | +130 | +130 | +126 | +126 |

即 **4 对的 ΔA remaining 与 ΔV remaining 都是 {127, 129, 129, 130}**；完整排队是 {129, 136, 129, 126}。此处专门更正早先草稿把 V remaining 写成 126–136 的错误——126 属于排队列，不属于 V remaining 列。

D0/D0 基线（同对第二轮 − 第一轮）：main case 1 的 Vremaining = −2、总量 −8；main case 5 的 Vremaining = 0、总量 +1。

hot 对（short case 2、main case 3、main case 7）是**未触发的阴性对照**，没有真实的 128 周期暴露，**不是**等价暴露的对照，不能拿它们当“实际 +128”来比。

## 5. 可比性与有限样本

- 6 个正式 cold 对里：A 是 read，sequence 1、ID 4、DID 11；V 是 read，sequence 1、ID 0、DID 10、`first_hit = 1`；两者共享 IOVA 274875809792、`s1_level = 0`、`pte_pa = 2193147896`，且 prefix 向量（word 78–93）相同。每个触发轮里**恰好一个**插入请求：V **write**，sequence 1、IOVA 274875813888、`hit = 1`、未截断——尽管 sequence 号相同，它是与 V read 不同的请求。A 的 comp/appear/return 事件齐全（event bitmap 127）。
- 触发前耗时只报告、不设门槛：4 个 cold 互补对里有 3 对的 `e4 − e69` 与 `e4 − e39` 两轮不同；**main case 6 是这 4 对里唯一触发前耗时相等的**（131/131 与 135/135），是最强的时序匹配样例。其余 3 对**照常保留**，不因事后新加的门槛被丢弃——原实验的资格要求（E3_CONTROLLED §11）并不要求触发前耗时或全部隐藏状态完全相同。
- 限制：互补对 **N = 4**，单次启动，64 B 有限诊断。不做显著性检验，不编造容差，不把整个残差总量归因到单一原因。

## 6. 不主张的东西

不主张完整 E3 矩阵已完成；不主张普适 head-of-line blocking；不解释全部旧 A8 减速；不是缓解性能结果；不做统计推广；不主张新颖性；没有 p 值、没有容差。E1 证据不受影响；E2 未被证明，只是按需的证据驱动分支；E4 只完成一部分（不主张已排除全部数据 / 软件效应）。

## 7. 脚本缺陷与修正（简述）

失败草稿的根因在分析脚本，不在实验：

1. `OBS_RE` 缺 `re.MULTILINE`，跨整份多行 console 无法匹配 → 期望 418 条却只匹配到 **0 条 UART 行**。已修正，并显式要求 `[ \t]+` 分隔；dmesg dump 之外的碎片实时行继续忽略（只计数、不使用）。
2. `check_round` / `check_round_uart` / `expect_matrix_entry` 抛出的错误通常缺 `pair N ` 前缀，被过滤掉，导致 `execution_ok`、`pair.errors`、`findings.structural_errors` 出现**假通过**。现在检查某一对期间抛出的错误统一加一次前缀并直接传播，不做任何字符串匹配。
3. `check_round_uart` 原先只比 `cycles` 和 `e3_words[233]`；现在是全记录比对（256 个 `e3_words`、32 个 launcher word、prep 统计、逐轮统计与映射、uint64 保真），缺、多、冲突都判失败。
4. `observed_version_and_magic.ok` 原先硬编码 `True`，现在反映真实的版本 / magic 结果。
5. `run.json` 次级比对原先可以不一致却不影响退出码，现在作为结构性错误传播（被比对的计数器也是脚本独立算的）。
6. `pair_eligibility` 原先不暴露逐轮事件 / 命中 / 截断事实，且把触发前时间相等放进因果标签链；现在报告逐轮 event bitmap、first-hit、插入计数 / 截断与选择顺序，以及 identity / PTE / prefix / 插入顺序 / 插入重数匹配标志；触发前耗时相等只作**诊断**。

原有轮次检查里没有发现语义错误：`e3_words[233]` 对 V write stage-3 求和、`e3_words[17]+[21] = 1`、`e3_words[18]+[22] = 1`、launcher / observer 检查全部原样保留。

## 8. mutation 检查

`scripts/mutate_e3_controlled_review.py` 把 run 目录复制到临时目录，逐项注入故障，要求每一项都非零退出且至少命中一个结构性错误；未变异的正常对照必须 exit 0、结构性错误为 0。**共 8 项 = 1 项正常对照 + 7 项故障**，结果全部符合预期：

| 项 | 类型 | 结果 |
|---|---|---|
| control（未变异） | 正常对照 | exit 0，结构性错误 0 |
| `e3_word_magic` | 故障 | exit 1，命中 |
| `e3_word_final_hit` | 故障 | exit 1，命中 |
| `launcher_word` | 故障 | exit 1，命中 |
| `uart_time_field` | 故障 | exit 1，命中（6 个结构性错误） |
| `metadata_input` | 故障 | exit 1，命中 |
| `missing_uart_row` | 故障 | exit 1，命中（418 → 417，3 个结构性错误） |
| `json_uart_exact_match` | 故障 | exit 1，命中（1 个结构性错误） |

每项的 `first_error` 原文见 `review-summary.json` 的 `mutation_checks`。

## 9. 尚未验证 / 不冒充的项

来自 `review-summary.json` 的 `findings.unavailable`：

- `run.json` 的 `smoke_passed`、`hpm_calibrated` 为 `null`。
- bit 哈希未在主机上验证（`run.json` 的 `bit.hash_verified` 为 `false`）。
- `run.json` 未记录 software payload 的 sha256。
- SD 回读未验证（`run.json` 的 `sd_readback_verified` 为 `false`）。

补充：`run.json` 里的 `pending_upload` 是**上传前的快照**，当前这批证据已经在 Dell 目录下；在没有 upload-receipt 的情况下，本文不宣称 receipt 已被验证。下载哈希与 SD 回读未验证，原始实验室时钟的 CDC 边界不在本次复核范围内。旧的 build-status 文件保留为**构建时快照**，不代表本轮状态。

本轮没有重新编译、没有改 RTL，所以时序收敛、CDC 与外部 IO 约束仍是未复核项，成功跑板不能替代它们。

## 10. 复现

```
python3 out/e3-controlled-review-20260918/server_review_v6.py \
    <run_folder> <new_out.json> \
    --console <run_folder>/console.raw.log \
    --readable-console out/e3-controlled-review-20260918/console.readable.log
```

预期：exit 0；`findings.structural_errors == []`；`totals.unique_uart_rows == 418`；`totals.duplicate_rows_ignored == 114`；`totals.pairs == 11`；`totals.rounds == 22`；`checks.console_cli_json_match.ok == true`（11 份去重后文档）；`checks.pair_documents.ok == true`；`checks.observed_version_and_magic.ok == true`；每份配对文档与 console CLI JSON 完全一致。随后在临时副本上跑第 8 节的 mutation 检查。

原始数据保持只读，本任务不改 RTL / 驱动 / CLI，不重编译、不上板。
````

docs/e3-next-method-plan.md
````
<!-- docs/e3-next-method-plan.md -->
# E3 下一方法：实验卡（仅规划）

**结论：这是规划卡，不是结果。** 本任务不写 RTL、不改驱动 / CLI、不编译、不上板，原始数据只读。它规定**下一步**要测什么，并且不主张任何缓解效果。

## 0. 先纠正一个说法

英文草稿把标题写成 “Fixed conditions unchanged from v6”，这是**不准确**的。v6 是 **64 B 有限诊断**；下一阶段要回到**既有的连续共跑工作量**（连续 4 KiB、V 4096 jobs、warmup 256）。两者**不是同一个实验**：v6 只在机制层面支持结论，不能把它的固定条件直接当连续性能实验的前提。

## 1. 全局进度

| 线程 | 状态 | 位置 |
|---|---|---|
| E1 持续重叠 / DS 返回 / 路径证据 | 已完成，保留 | 本次不重开、不重测 |
| E2 | 未证明 | 按需的证据驱动分支；**不是**前置条件，也不能谎称已完成 |
| E3（受限范围） | 在当前实现与 64 B 有限设定下已支持机制 | 边界问题（到达相位、输出背压、泛化）保留为边界，不马上塞进更多调试 |
| E4 | 部分完成 | 不主张已排除全部数据 / 软件效应 |
| P4M | **当前优先** | 见下面的基线 / 测量契约 |
| P5 | P4M 之后 | 冻结规则后再做留出集评估 |

## 2. P4M 的目标

在**同一个硬件 V 窗口**（`V_issue → V_done`）里，测出 A 完成了多少**完整 copy**、边界上还有多少在途，再配上已有的 V 时间 / miss / wait 计数器——这样 V 变慢就能**画出来**（对 A 完成速率），而不是靠断言。

## 3. 测量契约（实现里必须写清）

- 窗口：`V_issue → V_done`，即现有硬件 V 窗口。HPM（`GO` 采样 → 两侧 drain 完）与 observer（`V_window`）语义不变。
- `A_completed_jobs_window`：**完成时间戳落在窗口内**的 A copy 数。
- `A_progress_window = A_completed_jobs_window × chunk`。它是**窗口内完成的完整 copy 数 × chunk**，**不是**窗口内的实际 AXI 字节数，也**绝不**当成总线吞吐。
- `A_inflight_at_issue` / `A_inflight_at_done`：在 `V_issue` / `V_done` 时刻已发出但未完成的 A 请求数。
- `A_started_before_window`：在 `V_issue` 之前发出、但在窗口内完成的 A copy。**单独报告并给出界**，不能悄悄算成窗口内启动的进展；含起点前开始任务的完成边界效应必须明确写出来。
- 禁止：把整轮 A 总量除以 V 窗口；把结果叫吞吐；把“没有迟返回标志”当作零时间。

## 4. A 描述符间固定间隔的基线实现

- 固定 gap **只在上一整个 copy 完成并 drain 之后**插入；由硬件定步，避免 CPU 轮询 / 调度重新引入定步。
- **绝不**卡住在途的 AXI 请求。
- `gap = 0` 必须与今天的基线行为逐位兼容。

## 5. 固定条件（回到既有连续共跑工作量）

DS 50/50、50 MHz、4 条 DMA 通道、IOTLB 16、V = DMA0/DID10/ID0、A = DMA1/DID11/ID4、**chunk 4 KiB、V 4096 jobs、warmup 256**；角色 Solo / A1 对照 / A8 主测；保持轮询方式。

- 这是**本次新版本自身**的连续基线，**不能**复用旧 V 周期基线直接拼出新版本结果；新版本必须先有**自己的 gap0 自对照**。
- 选择性 PTW 128 干预在连续性能基线里**关闭**，避免把人工放大当成自然收益；机制观察里可以保留。
- 比较时用**同 gap0 / 相近的 A 完成速率**去比 V，不是把“单纯牺牲 A 换 V 改善”当结论。

## 6. 分阶段计划：要求、验收、停止条件

| 阶段 | 要求 | 产出 | 验收 | 停止条件 |
|---|---|---|---|---|
| P4M-1 基线实现卡 | 共同 V 硬件窗口的 A 完成量 + 边界在途计数，加上 A 完整 copy 之后的硬件固定间隔 | RTL + 窗口化计数器 + 单元 / 模型测试 | 短正确性验收通过 | 窗口语义无法唯一定义时停下并报告 |
| P4M-2 gap0 验证 | 新版本自身的 gap0 自对照 | 对照记录 | **gap 0 与新版本连续基线一致** | 若 gap0 不能等价复现，停下 |
| P4M-3 Solo / A1 / A8 短验收 | 三种角色各跑一遍短验收 | 短验收记录 | 通过；未观察到 CPU 定步调度 | 若不能在不卡住在途请求的前提下硬件定步 gap，停下 |
| P4M-4 预试 | 选定少量 gap 点与重复数 | 预试短记 | 只定参数，不做正式结论 | gap 范围 / 重复数定不下来就不进入正式比较 |
| P4M-5 冻结正式取舍曲线 | 在冻结的少量 gap 点上做正式比较 | 测量表 + 曲线输入 | 两侧都报；A 进展与 V 变慢同时给 | **不做全网格**；不设任意 V / A 阈值 |
| P4M-6 决策 | 读曲线做取舍 | 短记 | 先画曲线再决策 | 若固定 gap 基线已经够用 → **停，不做解耦**；只有出现残余的在途 miss vs A 进展权衡时，才评估**一个**最小 hit / pending-miss 解耦设计（要覆盖排序、失效 / invalidation、fault、前向进展、面积开销） |
| P5 | 冻结规则后的留出集评估 | 留出集测量表 | 跑之前规则已冻结 | 只报告，不回头调参 |

## 7. 决策规则

- **先画曲线**：V 变慢 vs A 完成速率。用户没给目标函数之前，不硬要一个阈值。
- 两侧都报，A 进展必须和 V 变慢一起出现；只有 V 单侧数字不算实验结论。
- gap 范围与重复数要在正式比较**之前**冻结；当前接口**还没有** gap 实现，所以由预试确定，不能凭空假设。
- **gap 范围和重复数目前尚未确定，本卡不编造参数。**
- 不编造容差、不做 p 值、不从简单 gap 或通用 non-blocking 想法上主张新颖性。
- 新方法的具体架构**尚未选定**；只有在简单基线不够用时才评估 hit / pending-miss 解耦。不承诺“小改动”“创新”或“有效果”。
- 可保留一次独立启动的 v6 复现作为发表可复现性检查，但**不是**强制阻塞的下一步，也不做成重复诊断循环。

## 8. 不主张

完整 E3 矩阵完成；普适 head-of-line blocking；解释全部旧 A8 减速；缓解性能；统计推广；固定 gap 本身是贡献；第二次启动是发表必需。

## 9. 记账

真实验证之后，用**短前缀**更新已有的 plan / current-status / handoff，保留历史记录。成功报告必须引用真实复核 JSON，绝不编造验证次数或测试次数。原始数据只读，未授权任务内不改 RTL / 驱动 / CLI、不重编译、不上板。
````

handoff-section.md
````
<!-- handoff-section.md -->
**状态**：E3 v6 受控运行已完成上板并通过服务器复核。复核脚本 `server_review_v6.py` 在原始数据上只读运行，结果未改；本轮没有重新编译、没有改 RTL / 驱动 / CLI。此前的失败草稿只是分析脚本的解析与错误汇总缺陷（不是 FPGA 失败），缺陷已修正后再跑一次即通过；mutation 检查共 8 项（1 项正常对照 + 7 项故障）全部符合预期。英文草稿已定稿为两份中文文档：`iommu-platform/docs/e3-controlled-results-review.md`、`iommu-platform/docs/e3-next-method-plan.md`。

**结果**：复核 `status: passed`、exit 0，9 项 checks 全部 `true`，`structural_errors` / `warnings` / `anomalies` 全空；11 对 / 22 轮、418 条唯一 UART 观测行、114 条完全相同重复去重、11 份完整 CLI JSON 与 UART 对象逐字段一致、输入哈希未变。机制结论只限于当前实现 / 配置 / 到达方式，互补样本仅 4 对；cold 上观察到的 `e70−e41=9`、`e42−e41=2`、V 自身 4 周期只是本轮观察值，不是普适常数。原始数据与全部不利结果保留。

**下一步**：按 P4M 实验卡推进——先做“共同 V 硬件窗口的 A 完成量 + 边界在途计数 + A 完整 copy 之后硬件固定间隔”的基线实现卡，接着 gap0 自对照验证、Solo / A1 / A8 短验收，再预试选定少量 gap 点与重复数、冻结正式取舍曲线；gap 范围与重复数尚未确定，V / A 阈值未指定前只画曲线。主证据：原始 run `iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/`；服务器复核 `iommu-platform/out/e3-controlled-review-20260918/server-review.json`、`iommu-platform/out/e3-controlled-review-20260918/review-summary.json`；复核脚本 `iommu-platform/out/e3-controlled-review-20260918/server_review_v6.py`；实验卡 `iommu-platform/out/e3-controlled-review-20260918/e3-next-method-plan.md`。
````