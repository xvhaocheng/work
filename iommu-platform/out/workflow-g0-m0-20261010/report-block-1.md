# G0/M0 结果草稿（2026-10-10）

> 草稿状态：目标保存为 `docs/workflow-g0-m0-results-20261010.md`。root 已执行全部 10 个 case，均 rc0 且 checker pass；无需重新生成研究计划。图 `completion-comparison.svg/png` 与 `trace-statistics.json` 由 root 执行 [`scripts/analyze_full_batch_g0.py`](../scripts/analyze_full_batch_g0.py) 产生；本文不伪造 trace 统计结果或运行耗时。

## 结论

- 在固定 `full64-v1` 行为级 DDR 合同下，普通 DS priority 服务消除了本合同的 P/PN 差额，因此基于该差额支持的新 gate 分支到此结束。
- 研究主问题仍保留：这不是对所有普通策略、所有应用、N/S 差异或硬件实现的证明。
- DS 是 behavioral DDR model 的服务规则干预，不是硬件创新，也没有 FPGA 构建或板测。
- 下一步不自动扩矩阵、不自动重启旧候选；任何新轮次需单独论证 loss 或应用合同。

## 关键数值

数值来自 [`root-result-acceptance.json`](../out/workflow-g0-m0-20261010/root-result-acceptance.json) 与 [`results.json`](../out/workflow-g0-m0-20261010/results.json)。

| 组 | T_V | T_A | T_return | 说明 |
| --- | ---: | ---: | ---: | --- |
| A1 s/p/n/pn | 73539 | 72965 | 73544 | 四模式相同；A1 无 PTW walks，邻近负例 |
| A8 P | 86594 | 86082 | 86601 | FIFO 基线 |
| A8 PN | 90283 | 89771 | 90292 | PN V penalty 3689 cycles（+4.26%） |
| A8 P+DS | 76738 | 76164 | 76745 | 普通 DS priority |
| A8 PN+DS | 76738 | 76164 | 76745 | DS 后 P/PN 相等 |

DS V 相对 P 下降 11.38%，相对 PN 下降 15.00%。FIFO P/PN 的 `ptw_active_cycles` 为 16483/20172，DS 两者均为 13485；两个 PN 模式 walk beats 均为 93。

S/N 保留在图和 raw 中；N 有不同 miss/work counts，不能把 N 的差异偷移为同一因果。完整 10 行数据见 [`results-table.csv`](../out/workflow-g0-m0-20261010/results-table.csv)；图保留 A1 四模式、A8 六模式。

## M0 接受检查

[`m0-acceptance.json`](../out/workflow-g0-m0-20261010/m0-acceptance.json) 与 [`baseline-register.json`](../out/workflow-g0-m0-20261010/baseline-register.json) 记录：真实事件驱动、bytes/B 闭合、双顺序（A-first / V-first）、start_delay=128、CPU on/off、watchdog（4 个功能 case、失败不伪装成功）、旧 N 回归、14 个 real-record mutants 被拒、9 个 invalid args。实现仅为测试 C++/SV/runner/checker 与分析脚本；生产 RTL/Linux 未改，没有 FPGA 构建或板测。

## 工件

- [`results.json`](../out/workflow-g0-m0-20261010/results.json)
- [`m0-acceptance.json`](../out/workflow-g0-m0-20261010/m0-acceptance.json)
- [`root-result-acceptance.json`](../out/workflow-g0-m0-20261010/root-result-acceptance.json)
- [`baseline-register.json`](../out/workflow-g0-m0-20261010/baseline-register.json)
- [`workflow-failure-card.md`](../out/workflow-g0-m0-20261010/workflow-failure-card.md)
- [`completion-comparison.svg`](../out/workflow-g0-m0-20261010/completion-comparison.svg)
- [`completion-comparison.png`](../out/workflow-g0-m0-20261010/completion-comparison.png)
- [`trace-statistics.json`](../out/workflow-g0-m0-20261010/trace-statistics.json)
- [`results-table.csv`](../out/workflow-g0-m0-20261010/results-table.csv)
- [`trace-format-sample.csv`](../out/workflow-g0-m0-20261010/trace-format-sample.csv)
- [`scripts/analyze_full_batch_g0.py`](../scripts/analyze_full_batch_g0.py)

## 范围与限制

- fixed static mapping COPY microbenchmark，A1/A8，warm256，gap0，CPU disabled，现有 serial behavioral DDR。
- M0 限制：SYSTEM behavioral DDR model only；CPU on/off 测试不声称固定 CPU workload。
- DS priority 是 C++ 服务模型干预，不是 synthesized hardware 或已评估资源成本。
- N 与 S 有不同 miss/work counts；不由本 P/PN 干预解决，也不自动提升为新主题。
- 每个 case 为单次确定性执行；不把重复确定性当统计样本，不计算 p99。

## 下一步

- Archive this candidate rationale；保留已验证的 full-completion harness。
- 不基于该 P/PN 差额进入 G1 或扩矩阵。
- 不自动重启旧机制；后续轮次需单独论证 loss 或 application contract。
