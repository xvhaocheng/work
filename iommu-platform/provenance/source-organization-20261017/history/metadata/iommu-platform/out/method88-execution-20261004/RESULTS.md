# method88 固定 A8 / target=170 复核正式结果

状态：正式报告。88/88 项已完成（host_exit0），原始记录已回传；本次只整理已冻结的 88 项与既有辅助提取，不新增板卡运行、不重试、不做新的 raw 校验或哈希回读、不做独立解包验收。本轮 88 实测已完成；无 Linux/RTL 新实现构建。

配套路径

- Dell 原始输出：`out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/`
- 离线分析：`out/method88-analysis-20261004-01/`（`report.zh.txt`）
- 辅助占用统计：`out/method88-aux-20261004-01/`
- 正式报告：`out/method88-execution-20261004/RESULTS.md`（本文件）
- Mac 侧归档：`results/experiments/20261004-method88-ssh-01/`
- 文献核对：`docs/research-literature-review-20261004.md`
- E2 复核证据：`out/e2-results-review-20261003/RESULTS.md`

## 0. 摘要

- 88 项全部可读：84 项 long + 4 项 short；失败 0、缺失 0、停止 []（`out/method88-analysis-20261004-01/report.zh.txt:12-14`）。冻结计划校验通过，`plan_sha256=6ea6c4158a19c324359ece6e1458adfa450472cfb3b09168d13550c383f783dc`，`seed=1380792376`（同文 `:9`）。
- 两个启动分别报告，不合并：boot 1 身份 `93406a68-fc8a-4040-9dc6-ae4e4a670d36`（复用既有启动，有既往 462 项活动），boot 2 身份 `236b6316-cca3-49be-b52b-48f3963fa515`（新启动）；各 44 项通过（同文 `:17-19`）。内核 `6.5.0-rc2-00093-gc0f5e13ea81f`（FPGA UART 实读，Mac `results/serial/20261004-live-confirm-01/uart.log`）。
- 公平规则沿用：配对同轮同目标；V 两侧中位速率均 >= target 170；`|medianL-medianR|/mean(medians) <= 5%`；每 run E23/E20 不足比例的组均值差 <= 10 个百分点；每 run 有效完整窗口 > 0；A 增益 5% 仅工程参考（计划 `docs/research-method-next-20261004.md:30`；分析报告 `:156-163`）。
- 四个启动×方向单元格对最强匹配简单对照均为负，跨方向判定 False。按预定条件，固定 A8 实占主创新路线在本轮 88 后结束；p5 作为已测试候选/基线。不继续为 p5 找新有利条件。
- 反向方向 p5 对 p6 在两个启动均保护匹配且 p5 A 速率更高（+34.68150% / +30.99451%）。这是反向条件下的复现，不是窄幅效应，也不构成 实际占用计费 独有收益的证明。
- E2 已完成 64，不是尚无条目替换直接证据。Codex 已实际验收：Linux `00093` / role1 新版 64 = 短 4、边界 30、前缀 30，64 clean / exit0；128 target 中 62 个替换后 firstmiss、66 保持 hit，无未决。W7 正反各 5 次保留两 target，W8 正反各 5 次均替换；128 target 非 128 独立试验。证据：`out/e2-results-review-20261003/RESULTS.md`。该证据只限有限布局/实现范围，不能推持续吞吐归因。新 88 不是重做 E2/E3。
- UART 已 live 返回 uname `00093` + bootid second，确认 second boot，Linux 不需 restart；serial closed。

## 1. 强基线与四格结果

强基线取每个启动、每个方向 p1/p2/p3/p4 中满足对 p5 保护匹配且 A 组均值最高者（计划 `docs/research-method-next-20261004.md:32`）。

| 启动 | 方向 | 强基线 | p5 A 相对增益 | 保护匹配 | 通过 |
| --- | --- | --- | ---: | --- | --- |
| boot 1 | forward | p4 | -5.68952% | true | false |
| boot 1 | reverse | p4 | -8.23689% | true | false |
| boot 2 | forward | p1 | -5.15142% | true | false |
| boot 2 | reverse | p2 | -4.43680% | true | false |

证据：`out/method88-analysis-20261004-01/report.zh.txt:22-27`（boot1 方向 0，增益 -0.056895203598005734）、`:30-37`（boot1 方向 1，-0.08236889321441186）、`:38-45`（boot2 方向 0，-0.05151423801872654）、`:46-53`（boot2 方向 1，-0.044368047254335415）。

方向判定与跨方向：方向 0 与方向 1 两个启动均 False；跨方向 False（`report.zh.txt:56-58`）。按计划 `docs/research-method-next-20261004.md:35-36`，必须两方向皆成立才谈跨方向；不存在匹配可行对照时结论为证据不足，不是成功。

## 2. p5 对 fixedgap 与 p6

| 启动 | 方向 | 对照 | 保护匹配 | p5 A 相对增益 | 处理 |
| --- | --- | --- | --- | ---: | --- |
| boot 1 | forward | fixedgap | true | +24.42897% | 参照 |
| boot 1 | reverse | fixedgap | false | +32.23674% | 保留、不宣称 |
| boot 2 | forward | fixedgap | true | +31.43434% | 参照 |
| boot 2 | reverse | fixedgap | false | +29.65361% | 保留、不宣称 |
| boot 1 | forward | p6 | true | +0.02452% | 低于工程阈值 |
| boot 1 | reverse | p6 | true | +34.68150% | 反向条件复现 |
| boot 2 | forward | p6 | false | -3.62458% | 保留、不宣称 |
| boot 2 | reverse | p6 | true | +30.99451% | 反向条件复现 |

证据：`report.zh.txt:23-29,31-36,39-44,47-52`。

读法与边界：

- 反向方向两个启动 p5 对 p6 均保护匹配且 p5 A 速率更高，可称**反向条件下的复现**；这不是窄幅效应，也不是 实际占用计费 独有收益的证明。
- forward 方向一格低于 5% 工程阈值（boot1 +0.02452%）、一格保护未匹配（boot2 -3.62458%），因此不存在跨方向 普遍 结论。
- fixedgap 只作参照；其中两格保护未匹配，一律保留披露，不作收益声明（`report.zh.txt:160`）。

## 3. 辅助占用统计（既有 84 个 long 的提取）

辅助目录：`out/method88-aux-20261004-01/`。辅助提取范围：84 个 long，n=3 每启动/方向，两个不同启动；boot1 为既有启动含旧 462 历史，boot2 为新启动。以下为已实际提取的量；本次未提供该目录逐行文本，故不给行级锚点，只引用既有观测定义来源。

- COMPLETE / (MAIN + COMPLETE)：全部 84 个 long run 的最小值 0.380313%、最大值 0.851637%。
- 每 copy COMPLETE 均值：全部 84 个 long run 均为 4 cycles。
- 数据边界：每 copy miss 不恒定，不能把该均值写成每 copy miss 固定。
- pooled fullstats（要求分组 `req==4 && miss<=4`，`dma_research_observer.sv:1540-1569,369-393,1694`）：
  - miss = 0：33583
  - miss = 1：45420
  - miss = 2：62968
  - miss = 3、miss = 4 及其他：0

必守边界：

- **不能说每 copy miss 固定**：pooled 分布本身说明每 copy miss 有分布；策略改变 PLRU/cache mix。
- **0xE2E 不能作真实服务分母**：`ctl_did_i==DID_A` 无状态过滤，`e3_o.current_did` 来自 `trans_req_data_q.did`，而该寄存器只在 IDLE 接受新 AW/AR 时重载，因此 IDLE 可保留上一请求的 stale DID；E2E 含 IDLE 与全部状态（`dma_research_policy.sv:58,199,294,451`；`rv_iommu_trans_ctl_wrap.sv:196,625-626,642-643,728-740`；`dma_research_observer.sv:432-433,974-1001,1517-1535`）。
- **不存在漏 DDT/PDT/PTW 计费主张**：p5 在每个 `ctl_did==DID_A` 且状态为 `REQ_TRANS`/`AWAIT_RESP` 的周期计费；`REQ_TRANS` 发出翻译请求、`AWAIT_RESP` 等 `trans_resp_valid_i`，整段内部翻译响应（DDT/PDT/PTW）在计费区间内；唯一被排除的非 IDLE 状态是 COMPLETE，而 IDLE 不是 busy（`rv_iommu_trans_ctl_wrap.sv:90-96,195-196,667-701,728-740`；`dma_research_policy.sv:290-293,328`）。
- **COMPLETE 侧可另行观测**：COMPLETE-owned busy cycle 可由 `cost_comp_q[1]`/`cur_a_comp` 单独得到，不应混入 E2E（`dma_research_observer.sv:1528-1535,1041-1060`）。

## 4. 限制

- 两个启动不是两次全新启动；boot 1 有既往 462 项活动（`report.zh.txt:19`）。
- 每启动/方向只有 n=3 轮，只是工程重复，不能作统计显著性判断（`report.zh.txt:158`）。
- 5% A 增益阈值是工程参考，不是统计保证（计划 `docs/research-method-next-20261004.md:30`）。
- raw 0xE25 是 `permission_closed_cycles`，不是 ADMIT stall，且不得作分母；0xE2E 含 IDLE stale DID，不是真实占用（`report.zh.txt:161`）。
- 人工 delay 只作诊断，不代表真实应用（计划 `docs/research-method-next-20261004.md:58`）。
- 未匹配保护对照、缺失与失败全部保留，不自动重跑（`report.zh.txt:12,160`）。
- 本报告只整理既有 88 项记录；没有新增硬件机制或驱动/RTL 实现，没有新构建/SD 更新。
- UART uname `00093` + bootid second 已 live 确认；Linux 不需 restart；serial closed。

## 5. E2/E3 与新增证据边界

- E2：最新 E2 已完成 64。Linux `00093`/role1 新版 64 = 短 4、边界 30、前缀 30，64 clean/exit0；128 target 中 62 个替换后 firstmiss、66 保持 hit，无未决。W7 正反各 5 次保留两 target，W8 正反各 5 次均替换；128 target 非 128 独立试验。证据：`out/e2-results-review-20261003/RESULTS.md`。该证据明确限有限布局/实现范围，不能推持续吞吐归因。
- E3：保留有限受控阻塞证据；人工 128 周期只作诊断口径，不外推为自然 4KiB 任务损失。
- method88 不扩展、也不重做 E2/E3；其唯一判定对象是固定 A8 主创新路线是否存在稳定额外收益。

## 6. 结论与路线终止

- 固定 A8 / target=170 的实占计费（p5）相对最强匹配简单对照在四个启动×方向单元格全负，跨方向不成立。按预定条件结束该主创新路线，p5 转为已测试候选/基线（计划 `docs/research-method-next-20261004.md:52-53`）。
- p5 vs p6 的反向复现不足以支撑 实际占用计费 独有价值主张；不得把反向复现写成跨方向 普遍 结论。
- 不宣称 实际占用计费 普遍无用，也不宣称已证明新机制。新方法尚未成立。

## 7. 条件性下一步（本轮不实现）

- 唯一优先研究问题与终止判据见 `docs/research-literature-review-20261004.md` 末节与本次交接的 `next_proposal`。
- 不继续为了 p5 赢而挑条件；新唯一推荐方向保持机制边界/合理 N 强基线。
- 但现在没有 N/P 实现和足够 novelty 差异，不能称已选好新方法或创新成立；候选先对照规格。
- 明确可执行下一步：先做定向关键 RTL 可行性与标准 N 接口/资源/失效/顺序规格；若仅常规优化无增量，不编大工程。读现有源码不是多余 hash 回读。用户授权继续实现也不授权做科学无价值工程。
- 首选一项标准方法强基线/设计取舍对照实验：同一 BIT、同一仪器、同一工作集、同一完整 copy 作动点，比较 S（现有串行）/ N（合理有限非阻塞）/ P（同每类总容量静态预留），报告 V 端到端完成时间与短时未达目标分布、A 在同一 V 硬件持续时间内的有效完成字节、以及 LUT/FF/BRAM/Fmax。
- 明示：即使 N 或 P 有效，常规优化也不自动构成创新；只有相同 V 保护下保留更多 A 进度或同效果下更低实际成本的可解释差异，并在未调参条件下复现，才谈方法候选。
- 本轮不实现驱动/RTL，不新增构建/SD 更新。
