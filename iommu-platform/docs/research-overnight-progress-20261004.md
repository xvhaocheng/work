# 昨夜以来的 FPGA 实验进展（method88 公平对照与 N 原型线）

- 快照：2026-10-04，截至所提供 JSON/MD 字段；原文多为 UTC，本文给日本时间 JST=UTC+9。这里 v0～v5 是本轮硬件原型修订编号。
- 范围：历史说明 method88 公平对照与 N 原型线两条已测线；只汇总既有正式报告，不新增实验/代码/测试，不重新分析原始数据，不提出下一方案；执行与最终审阅归 Codex。
- 停止要求：method88 与当前 v5 S/N 板测、结果分析与文档更新均完成后停止；不自动启动下一轮，也不重启 method88。

## method88 路线：前半夜公平对照与结束（历史说明）

- 普通中文说明：原路线想解决的是“在固定 A8、V 的目标速度为 170 MiB/s 的条件下，按实际占用/忙碌周期来计费或调度，让重点观察的 V 路受到保护，同时尽量保留制造竞争的 A 路进度”。这里把该策略称为 method88 的原方法/实际占用计费（p5）；对照是 p1、p2、p3、p4、p6、fixedgap 等简单策略。
- 为什么要先匹配 V 的保护水平再比 A 保留速度：如果直接比 A 吞吐，某个基线可能用牺牲 V 来换 A，表面 A 更快但 V 保护没有对齐，比较不公平。固定计划因此要求：配对同轮同目标；V 两侧中位速率均 >= target 170；`|medianL-medianR|/mean(medians) <= 5%`；每 run E23/E20 不足比例的组均值差 <= 10 个百分点；每 run 有效完整窗口 > 0。只有通过这些保护匹配的对照，才在 p1/p2/p3/p4 中取 A 组均值最高者作“最强匹配简单基线”，再比较 p5 的 A 相对增益；5% A 增益只是工程参考，不是统计保证。
- 实际结果：method88 88/88 项完成（host_exit0）。四个启动×方向单元格对最强匹配简单基线均为负，跨方向判定 False：

  | 启动 | 方向 | 最强匹配简单基线 | p5 A 相对增益 | 保护匹配 | 通过 |
  | --- | --- | --- | ---: | --- | --- |
  | boot 1 | forward | p4 | -5.68952% | true | false |
  | boot 1 | reverse | p4 | -8.23689% | true | false |
  | boot 2 | forward | p1 | -5.15142% | true | false |
  | boot 2 | reverse | p2 | -4.43680% | true | false |

- 结论边界：按预定条件，固定 A8 / target=170 的实占计费主创新路线在本轮 88 项后结束，p5 转为已测试候选/基线；不继续为 p5 找新有利条件。不能由此说实际占用计费在任何条件都无用，也不能把 p5 对 p6 的反向复现写成跨方向普遍结论；这里只说明在上述冻结条件下没有显示稳定额外收益。
- 启动与结束时间：按交接给定，method88 于 02:42 JST 启动；本次所给正式报告没有可引用的完成钟点字段，因此不补造结束时刻，只记录 88/88 host_exit0 与其正式报告。
- 正式报告与离线分析：
  - [method88 固定 A8 / target=170 复核正式结果](../out/method88-execution-20261004/RESULTS.md)
  - [method88 离线分析 report.zh.txt](../out/method88-analysis-20261004-01/report.zh.txt)
- 转向 N 原型线：method88 结束后，不继续为 p5 挑条件，转向 N 原型线，验证在 A 查页表/未命中时，V 已缓存翻译的请求能否提前走候选/探测快速通路，以及相关计数是否真的非零。这是工程基线验证，不是把 method88 的四格结果当作 N 的因果关系。
- 证据边界：本段只汇总既有正式报告；这次文档补充只整理昨夜已实际运行完毕的 88 项正式记录，不追加板卡运行；E2/E3 是既有证据，不作为本轮新增实验。

## 先用大白话说明与术语

A 是制造竞争的 DMA，V 是重点观察的一路。S 是常规处理的对照版；N 尝试在 A 查页表时，提前处理另一方已经缓存好翻译结果的请求。我们一直在区分三件事：搬运是否正确，快速路径是否真正被触发，以及是否带来速度改善。4 KiB 缓冲能存下一份完整搬运数据，用来解除这组读写交错条件中的依赖限制。本轮不把这组依赖推理写成完整的板上死锁根因证明。

术语（第一次出现附中文）：BIT 是 FPGA 硬件配置；candidate 是已到快速通路入口且满足条件的请求；probe 是缓存探测。

为什么逐轮改（简短中文表）：

| 问题 | 本轮改动 | 结果 |
|---|---|---|
| 缓存结果齐全仍要排队 | 尝试先服务 | v5 证明路径能工作 |
| 代码和模型提示读入口依赖可能限制候选进入，v4 板测未见候选 | 在限定 4 KiB 单任务与完整缓冲下放行 DID10/11 读入口 | A8 略高于 S，A16 更低，且均低于原版，不能宣布改善成功 |

## 四点结论

1. **两条线分开计数，不能合并成重复试验**：method88 线为 88 项完成（88/88，host_exit0）；N 原型线为 original 15 + v3 S/N 30 + v4 S/N 30 + v5 S/N 30 = **105 项**。两条线样本不同，分别列 **88** 和 **105**；若说 **193**，那只是两条线通过完整功能清单的项目总数（88+105），不是 193 次重复试验，也不是 193 份机制收益。original 为复用基线，三 label 分析不重复累计；v0 零/不完整排除；v1/v2 模型未上板，不并入板测；部分/失败行不并入计数。主机模型/编译数字不是板测计数。**88 与 105 都只说明功能/数据/协议通过，不是加速证明。**
2. **v3 仅有 fast 接受计数为 0**，且 v3 没有提供 candidate/probe 这类候选及探测的板级诊断记录，不能把该 0 外推为“候选/探针未触发”。**v0** acceptcount 为 0 因已确认的静态 implicit wiring 缺陷而不具科学资格，不能据此断“快通路科学上未激活”；该缺陷也**未被证明是 A8 hang 的唯一原因**。**v4** 的 candidate=0 只在 TL 接口/资格窗口成立，不能推出无缓存命中或上游没有 V。
3. **v1 的 RR 模型 8/12 失败**，是有限 WAIT 环的回环观测；**不是板上 hang 根因已被证明**，交接标记 `not_proven`。v2 模型 depth512 通过，但 FPGA Place 失败，LUT 超限 exit1；模型通过不等于板测。
4. **v5 两套完整 FPGA recovery 构建均 exit0 并验收**：S WNS 0.177 / WHS 0.035 / LUT 174413 / FF 138384；N WNS 0.177 / WHS 0.038 / LUT 176476 / FF 141170。初次构建曾因 scope 与 FPGA source list 问题 exit1，经 `v5-shared-scope-fix.patch` 与 `v5-fpga-source-list-fix.patch` 后 `source_recovery_exit = 0`；既有约束下时序通过，`io_constraint_review_complete=false`，完整时钟/IO 复核待做。S 板测 15/15 complete、runner exit 0，finished_at 2026-10-04T06:55:56Z = JST 15:55:56；N 板测同样 15/15 complete、runner exit 0，finished_at 2026-10-04T07:10:06Z = JST 16:10:06。N 原始数据已回传 Dell，分析脚本 exit0、complete=True、quality_issues=0。**v5-build-state.json 与 hit-v5-build-acceptance.json 中早期交付的 running/pending 是历史快照；Codex 已按实际返回把当前字段更新为 S15/N15 完成、`board_functional_acceptance = "15/15_completed_exit0"`、`analysis_complete = true`、`analysis_quality_issues = 0`。**

## 阶段表（实际动作 / 数量 / 结果 / 层级）

| 阶段 | 实际动作 | 完成/结果 | 层级与备注 |
|---|---|---|---|
| original-03 | 原版对照在 06:07 完成 15 项；v5 比较时复用这些数据 | 15/15 completed，exit0，problems 空，taint0 | **板测/旧数据复用**；一次 Ctrl-C 解除启动脚本卡住，保留记录；本轮不据此推断其当时启动方式，也不重烧/不重跑（[summary.json](../out/n-prototype-20261004/nonblock-pilot-20261004-original-03/summary.json)） |
| v0 | 已上板尝试；serial-01/02 raw `completed=0 failed=0 status=stopped`（启动阶段停止，不是实验数据错误）；serial-03 无 summary | 交接/v1-work-state：短 Solo/A1 数据 OK，短 A8 hung，余 12 未跑 | **证据层级须分**：raw summary 与交接描述；acceptcount0 因确认静态 implicit wiring 缺陷，不作科学未激活结论；该缺陷非 A8 hang 唯一已证原因 |
| v1 | 读守卫；主机模型 + 两版 BIT 编译 | 编译/时序 exit0；banked RR 8/12 失败，legacy 12/12 通过；**未上板** | **模型/编译**；失败是有限 WAIT 环，不作板 hang 根因 |
| v2 | 整 4KiB buffer；主机模型 + 两版 BIT 编译 | 模型 depth512 12/12 通过；FPGA Place 失败，LUT S207069/N207226 > 203800，exit1 | **模型通过 ≠ 板测** |
| v3 | 局部 RAM；两版 BIT + 板测 | S 15/15、N 15/15，exit0，problems 空，taint0；**N fast 接受计数 0** | **板测**；数据/协议通过，快通路未观测到服务；v3 只有 fast 计数，没有 candidate/probe 证据 |
| v4 | flat MSI + 被动诊断；两版 BIT + 板测 | S 15/15、N 15/15；candidate/probe/DC/LEAF/fast 仍 0 | **板测 + 诊断**；消除一处已确认静态兼容限制，不等同加速；candidate=0 只在 TL 接口/资格窗口，不能推出无缓存命中/上游没有 V |
| v5 | 限定 4KiB job 资格 + DID10/11 读入口放行；主机测试；两版构建；S/N 板测 | 主机：certificate unit exit0，clean 重建 11/11，qualified/guarded RR 各 12/12；A8 模型 fire78，总周期未见改善。构建：初次 scope/source list 失败，修复后两版 recovery exit0 并验收。板测：S 15/15 complete、N 15/15 complete，runner exit0，quality_issues=0 | **主机/编译 vs 板测分离**。N 在 A8/A16 观测到 candidate/probe/DC/leaf/fast accept 非零；Solo/A1 为 0。S 在 A8/A16 可见 candidate_in_window 但 probe/fast 为 0。N/S 中位差仅描述性：A8 约 +4.2%，A16 约 -2.7%，A1/Solo 近似持平；N/原版 A8 约 -14.52%、A16 约 -22.09%，N 相对原版仍较低。不能作纯因果或显著性 |

## v5 长组同窗中位数摘要

> 窗口中位来自 `raw_window[1]`；ns 与 MiB/s 按 50 MHz 换算为推导量；下表速度单位均为 MiB/s。短组只验收，不混入长组。A/V 同窗速率数值相等，是因为本轮二者在该窗口都完成 4096 份、每份 4096 B；相同单位本身不能推出相同速率。

| condition | original V 中位 | S V 中位 | N V 中位 | A 中位说明 |
|---|---:|---:|---:|---|
| Solo | 282.809 | 283.035 | 283.370 | A jobs=0，A 吞吐=0 |
| A1 | 149.277 | 145.461 | 145.501 | A jobs=4096，A 吞吐同 V |
| A8 | 108.858 | 89.338 | 93.048 | A jobs=4096，A 吞吐同 V |
| A16 | 100.686 | 80.608 | 78.440 | A jobs=4096，A 吞吐同 V |

N/S 相对变化按描述性中位：Solo 约 +0.1%，A1 约 0.0%，A8 约 +4.2%，A16 约 -2.7%。N/原版仍较低：A8 约 -14.52%，A16 约 -22.09%，均为描述值而非归因。A8 逐轮 V 吞吐为 104.263/92.987/93.048 MiB/s，三轮差异明显，全部保留，不能只写“平均 +4.2%”就称稳健。S/原版差异同时含分 bank、缓冲/layout 与启动条件/layout 差异，不能作纯布局因果或统计显著结论。

## v5 N 真实候选 / 探测 / DC / leaf / fast 计数

- A8：candidate_in_window 3/3，probe_starts 18436，dc_eval 18436，dc_good 18436，leaf_eval 18436，leaf_good 18436，leaf_lookup_miss 0，fast_accept 18436；fast V 每轮 6142/6144/6144，fast A 每轮 2。
- A16：candidate_in_window 3/3，probe_starts 24582，dc_eval 24582，dc_good 24582，leaf_eval 24582，leaf_good 24579，leaf_lookup_miss 3，fast_accept 24579；fast V 每轮 8193/8193/8193，fast A 0。
- Solo/A1：candidate/probe/DC/leaf/fast accept 均为 0，只见 `runtime_enable`；这是这些条件下的科学未行使，不是技术失败。
- short-a8：fast accept 98，短组只验收，不并入长组。
- **fast 是 accepted requests，不是 hit rate，也不是全部 miss 数。新增诊断资格计数不是旧 IOMMU hit/miss/PTW；旧失效 HPM 不作为证据。**
- v5 资格限定在当前单任务、对齐 4 KiB、完整缓冲、DMA0/1 的冻结 workload；证书内部 `fault_q` sticky 未导出，不能从计数 0 反推故障状态。
- 已知 hit-under-miss / 完整缓冲原则不能被本轮声称新颖性；本轮只是工程基线验证与诊断。

## 冻结 15 项工作量（实际计划）

每次搬运 4 KiB。V 使用 DMA0，固定使用一组源/目的页；A 使用 DMA1，在 1、8 或 16 组源/目的页间循环，Solo 表示 V 单独运行。先做 Solo/A1/A8 三项短验收（预热16次、正式64次）；再按三轮固定顺序，每轮测 Solo/A1/A8/A16 四项（预热256次、正式4096次）。因此每版是 3+3×4=15 项。硬件计时窗口覆盖 V 的正式工作量；4 KiB 是每次搬运大小，不是计时窗口总量。

## 时间戳（可核查，JST）

- original-03 完成 2026-10-03T21:07:14Z → **06:07**
- v3 S 完成 02:30:08Z → **11:30**；v3 N 完成 02:42:37Z → **11:42**
- v4 S 完成 04:32:27Z → **13:32**；v4 N 完成 04:45:01Z → **13:45**
- v5 构建验收（2026-10-04）：S **15:46**；N **15:50:22**（以各 build-acceptance.json 实际时间为准）
- v5 板测完成（2026-10-04）：S finished_at 06:55:56Z → **15:55:56**；N finished_at 07:10:06Z → **16:10:06**
- N v5 boot：`c2bd10b5-26e7-4de6-ac4f-d3a04c14498c`，Linux `6.5.0-rc2-00093-gc0f5e13ea81f`；无需 SD 更新
- 口语“昨夜1点”仍无精确证据锚定，不补造；method88 按交接给定于 **02:42 JST 启动**，本次所给正式报告没有可引用的完成钟点，故不写完成时刻，只写 88/88 host_exit0；N 线时间戳均有 JSON 支撑。

## 来源与归属

- 研究与验证：[research-n-prototype-spec-20261004.md](./research-n-prototype-spec-20261004.md)、[research-literature-review-20261004.md](./research-literature-review-20261004.md)、[nonblock-v5-qualified-read-plan-20261004.md](history/plans/nonblock-v5-qualified-read-plan-20261004.md)、[CURRENT_STATUS.md](../../CURRENT_STATUS.md)、[HANDOFF.md](../../HANDOFF.md)、[analysis-v4-pilot/RESULTS_REVIEW.md](../out/n-prototype-20261004/analysis-v4-pilot/RESULTS_REVIEW.md)、[analysis-v5-pilot/RESULTS.md](../out/n-prototype-20261004/analysis-v5-pilot/RESULTS.md)、[analysis-v5-pilot/RESULTS_REVIEW.md](../out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md)
- 本稿由 DeepSeek 起草，Codex 已直接核对原始数据并执行实验与离线汇总，并修正实际状态、次数和计时口径；关键 RTL 与实际测试执行由 Codex 完成。N 采用已知原则做工程基线，目前不能以此主张新方法贡献；这不等于论文整体无创新。
- 本轮 v5 两版板测及分析完成后暂时停止，保留所有失败与未运行记录。

## 当前状态更新：S/N v5 均已完成

- S v5：15/15 completed，runner exit0，finished_at 2026-10-04T06:55:56Z = JST 15:55:56。
- N v5：15/15 completed，runner exit0，finished_at 2026-10-04T07:10:06Z = JST 16:10:06；N 原始数据已按所给事实 SCP exit0 回到 Dell。
- 离线分析：`analyze_nonblock_pilot --diagnostics` 实际 exit0，`complete=True`，`quality_issues=0`。
- 累计板测功能通过按两条线分列：method88 线 **88/88**（host_exit0）；N 原型线 original 15 + v3 30 + v4 30 + v5 30 = **105**。两线样本不同，合计若写 **193** 只是通过完整功能清单的项目总数（88+105），不是 193 次重复试验或机制收益。original 复用不重复计；v0 零/不完整排除；v1/v2 模型不并入板测。
- 机制层：N 在 A8/A16 首次给出真实非零 candidate/probe/DC/leaf/fast accept；Solo/A1 为 0；S 在 A8/A16 可见 candidate_in_window 但 probe/fast 为 0。
- 性能层：N/S 同窗中位差仅描述性，A8 约 +4.2%，A16 约 -2.7%，A1/Solo 近似持平；N/原版 A8 约 -14.52%、A16 约 -22.09%，N 相对原版仍较低。不能作纯因果或统计显著结论。
- 数据问题保留变更历史：`v5-build-state.json` 早期快照的 `S15_complete_N15_running`、`hit_board = "running15"`，以及 `hit-v5-build-acceptance.json` 早期交付的 `pending` 均为历史快照；Codex 已按实际返回更新当前字段：stage `v5_finished_analyzed_paused_by_user`、`hit_board = "15/15_complete_exit0"`、`board_functional_acceptance = "15/15_completed_exit0"`、`analysis_complete = true`、`analysis_quality_issues = 0`。N 实际 summary 已 complete。初次构建失败及修复补丁保留。
- 按用户要求，本轮分析完成后暂停；未启动下一轮。

