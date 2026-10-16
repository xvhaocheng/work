# method88 固定 A8 t170 复核结果（草稿）

状态：基于 out/method88-analysis-20261004-01 的 88 项已完成离线分析整理。本文只复用已经校验过的原始记录，不新增板卡运行、不重试、不做新的 raw 校验或哈希回读。辅助统计脚本见 scripts/method88_aux.py。

## 0. 摘要

- 88 项全部可读：84 项 long + 4 项 short。两个启动分别报告，不合并。
- 第一次为复用既有启动（有既往 462 项活动），第二次为新启动，因此不是两次全新启动。
- n=3 轮/启动/方向只是工程重复，不是统计显著性证明。
- 公平规则沿用：配对同轮同目标，V 中位差 <=5%，双方达到 target 170，E23/E20 不足比例差 <=10 个百分点，且每 run complete_active_windows > 0；A 相对增益 5% 仅工程参考。
- 四个启动×方向单元格对最强匹配简单对照均为负；跨方向未通过。
- p5 对 p6 在反向方向两启动均复现窄幅反转：+34.68150% 与 +30.99451%；但这不是 actual cost 独有收益的证明。
- 措辞边界：不能说 actual cost 普遍无用，也不能说已经证明新机制。

## 1. 公平性规则与强基线结果

强基线取每个启动、每个方向 p1/p2/p3/p4 中满足保护匹配且 A 组均值最高者。

| 启动 | 方向 | 强基线 | p5 A 相对增益 | 保护匹配 | 通过 |
| --- | --- | --- | ---: | --- | --- |
| boot 1 | forward | p4 | -5.68952% | true | false |
| boot 1 | reverse | p4 | -8.23689% | true | false |
| boot 2 | forward | p1 | -5.15142% | true | false |
| boot 2 | reverse | p2 | -4.43680% | true | false |

四格均为负；跨方向判定为 false。按预定条件，固定 A8 实占主创新路线在本轮 88 后结束，p5 作为已测试候选/基线。

## 2. p5 对 fixedgap 与 p6

| 启动 | 方向 | 对照 | 保护匹配 | p5 A 相对增益 |
| --- | --- | --- | --- | ---: |
| boot 1 | forward | fixedgap | true | +24.42897% |
| boot 1 | reverse | fixedgap | false | +32.23674%（不宣称） |
| boot 2 | forward | fixedgap | true | +31.43434% |
| boot 2 | reverse | fixedgap | false | +29.65361%（不宣称） |
| boot 1 | forward | p6 | true | +0.02452%（低于工程阈值） |
| boot 1 | reverse | p6 | true | +34.68150% |
| boot 2 | forward | p6 | false | -3.62458%（不宣称） |
| boot 2 | reverse | p6 | true | +30.99451% |

反向方向两启动 p5 对 p6 的保护匹配均成立，p5 的 A 速率更高；这是窄幅反转复现，不是 actual cost 独有收益的证明。forward 方向一个低于阈值、一个未匹配，因此不能写成跨方向 universal 结论。

## 3. 限制

- 两个启动不是两次全新启动；boot 1 有既往 462 项活动。
- 每个启动/方向只有 n=3 轮，不能作统计显著性判断。
- 5% A 增益阈值是工程参考，不是统计保证。
- raw 0xE25 是 permission_closed_cycles，不是 ADMIT stall；0xE2E 包含 IDLE stale DID，不是真实占用。
- 本报告只整理既有 88 项记录，没有新增硬件机制或驱动/RTL 实现。

## 4. 条件性下一步提案

只有在需要继续追问时，才考虑以下单一条件性提案；本轮不实现驱动/RTL。

- 研究问题：当服务成本变化快于固定反馈窗口时，计数加反馈是否仍能保护 V？
- 候选干预：受控 A-only PTW response-delay pattern，同时保持计数、per-copy req/miss 不变。
- 约束：不保证 p5 胜出；不得在成功前重调参；策略参数必须与已冻结参数一致；必须使用公平强简单对照；数据冻结前写清预测与停止判据。
- 解释边界：这是人工诊断，不是真实负载证明。
- 运行时接口：新运行时探针很可能需要 Linux config+BIT，除非能够确认恰好已有接口；是否推进由 Codex 作 consequential decision。
- 本 call 不实现 driver/RTL。


