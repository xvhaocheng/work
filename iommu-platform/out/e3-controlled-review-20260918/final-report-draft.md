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
