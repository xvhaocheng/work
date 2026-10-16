# v5 S/N 板测结果复核（中文报告）

> 路径：`out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md`
> 身份：本文档由 DeepSeek 起草，Codex 已直接核对原始数据并执行实验与离线汇总。
> 样本规则：全样本保留，不剔除、不补跑、不自动重跑；fast=0 是合法科学观察，不产生 quality issue。

---

## 0. 复核对象与来源

| 类别 | 文件 / 证据 |
|---|---|
| v5 离线汇总 | [RESULTS.md](./RESULTS.md) |
| v5 汇总 JSON | [nonblock-pilot-analysis.json](./nonblock-pilot-analysis.json) |
| 原版旧基线 summary | [summary.json](../nonblock-pilot-20261004-original-03/summary.json) |
| S v5 summary | [summary.json](../nonblock-pilot-20261004-serial-v5-01/summary.json) |
| N v5 summary | [summary.json](../nonblock-pilot-20261004-hit-v5-01/summary.json) |
| S v5 boot | [boot.json](../nonblock-pilot-20261004-serial-v5-01/boot.json) |
| N v5 boot | [boot.json](../nonblock-pilot-20261004-hit-v5-01/boot.json) |
| S 构建验收 | [serial-v5-build-acceptance.json](../serial-v5-build-acceptance.json) |
| N 构建验收 | [hit-v5-build-acceptance.json](../hit-v5-build-acceptance.json) |
| v5 构建状态 | [v5-build-state.json](../v5-build-state.json) |
| v5 主机验收 | [v5-host-acceptance.json](../v5-host-acceptance.json) |
| v5 运行计划 | [nonblock-v5-qualified-read-plan-20261004.md](../../../docs/nonblock-v5-qualified-read-plan-20261004.md) |
| v4 结果复核 | [RESULTS_REVIEW.md](../analysis-v4-pilot/RESULTS_REVIEW.md) |

来源约束：S/N v5 使用不同 BIT，且为不同 fresh boot；原版 `original-03` 复用 06:07 完成的 15 项旧数据，v5 没有重新跑 original，本轮也不据此推断其当时启动方式。上述运行身份差异是本报告所有比较的前提。

---

## 1. 运行身份与数据完整性

### 1.1 三个 label

| label | 目录 | run_id | BIT | 数据状态 |
|---|---|---|---|---|
| original-serial | `out/n-prototype-20261004/nonblock-pilot-20261004-original-03` | `a72442e2e112f401` | `original-serial.bit` | complete，复用旧基线 |
| banked-serial | `out/n-prototype-20261004/nonblock-pilot-20261004-serial-v5-01` | `c76ccc3fac1ff68a` | `banked-serial-v5.bit` | complete |
| banked-hit | `out/n-prototype-20261004/nonblock-pilot-20261004-hit-v5-01` | `e7497b8802cb9e58` | `banked-hit-v5.bit` | complete |

### 1.2 新鲜启动与 Linux/SD

- S v5 boot：`d103c6f7-b74a-487b-adac-20b46e0ebbd2`，Linux `6.5.0-rc2-00093-gc0f5e13ea81f`，taint 0。
- N v5 boot：`c2bd10b5-26e7-4de6-ac4f-d3a04c14498c`，Linux `6.5.0-rc2-00093-gc0f5e13ea81f`，taint 0。
- 结论：S/N 各自 fresh boot，但使用不同 BIT；不能把 S/N 差异当作同一 BIT 上纯 runtime enable 因果效应。
- SD/Linux：本轮无需 SD 更新；按 v5 计划约束，Linux/SD 不变。

### 1.3 完整性

`nonblock-pilot-analysis.json` 显示：

| label | complete | failed | incomplete | not_run | total |
|---|---:|---:|---:|---:|---:|
| original-serial | 15 | 0 | 0 | 0 | 15 |
| banked-serial | 15 | 0 | 0 | 0 | 15 |
| banked-hit | 15 | 0 | 0 | 0 | 15 |

`quality_issues = []`，`complete = true`，分析脚本 exit 0。S/N runner exit 0，N 原始数据已按所给事实 SCP exit 0 回到 Dell。技术通道、数据、协议均未出现新的错误项。

---

## 2. 技术成功、机制激活、性能改善必须分开

### 2.1 技术成功

- 三 label：每 label 15/15 complete，failed 0，incomplete 0，not_run 0。
- `quality_issues = 0`，runner problems 空，taint_after = 0。
- S/N 两套 v5 构建：初次构建曾 exit 1，原因是 scope 与 FPGA source list 问题；经 `v5-shared-scope-fix.patch` 与 `v5-fpga-source-list-fix.patch` 后，`source_recovery_exit = 0`，两版最终构建验收记录为 timing/resource accepted。初次失败与修复过程应保留，不隐去。
- S 构建验收记录 `board_functional_acceptance = "15/15_completed_exit0"`。N 的 `hit-v5-build-acceptance.json` 现已记录 `board_functional_acceptance = "15/15_completed_exit0"`（`actual_acceptance_utc` 为构建验收时间，早于板测完成），N 的 `summary.json` 也已是 complete 且 finished_at 为 `2026-10-04T07:10:06Z`；早期交付时的 `pending` 是历史快照，不是本次 N 板测事实。

### 2.2 机制激活

机制激活看的是 candidate/probe/DC/leaf/fast accept 是否被真实观测到，而不是只看“搬运是否成功”。

- N `banked-hit`：
  - Solo、A1：candidate/probe/DC/leaf/fast accept 均为 0，只观测到 `runtime_enable`。
  - A8：candidate/probe/DC/leaf/fast accept 非零。
  - A16：candidate/probe/DC/leaf/fast accept 非零，并有少量 `leaf_lookup_miss`。
  - short-a8：fast accept 非零，但短组只做验收，不并入长组比较。
- S `banked-serial`：
  - Solo、A1：诊断 seen 为 0。
  - A8、A16：seen 出现 `base_s1_ptw_active`、`full_fast_window`、`candidate_in_window`，但 `probe_starts = 0`，fast accept 为 0。
- original-serial：没有本 v5 诊断 bank，fast 字段不适用；不能用旧 HPM 推断 N 机制。

### 2.3 性能改善

- 主机模型侧：v5 曾记录 A8 模型 `nonblock_fire`，但总周期未见改善；这不能替代板测。
- 板测侧：N/S 变化在 A8 为约 +4.2%，A16 为约 -2.7%，A1 约 0%，Solo 约 +0.1%，均为描述性中位差；不能宣称统计显著，也不能只凭 fast accept 计数宣称性能收益。
- S/原版差异同时包含分 bank、缓冲/layout 与启动条件/layout 差异，不能作纯因果外推。

---

## 3. 短组验收（只验收，不与长组混合）

短组只做验收，不参与两段对照，不与长组混合，不做显著性。A 吞吐为同一窗口内的 A 完成字节速率，不是独立总线吞吐。

| label | case | condition | cycles | V MiB/s | A jobs | A MiB/s |
|---|---|---|---:|---:|---:|---:|
| original-serial | short-solo | Solo | 44146 | 283.151 | 0 | 0 |
| original-serial | short-a1 | A1 | 83470 | 149.754 | 64 | 149.754 |
| original-serial | short-a8 | A8 | 113640 | 109.996 | 64 | 109.996 |
| banked-serial | short-solo | Solo | 44053 | 283.749 | 0 | 0 |
| banked-serial | short-a1 | A1 | 86709 | 144.160 | 64 | 144.160 |
| banked-serial | short-a8 | A8 | 140432 | 89.011 | 64 | 89.011 |
| banked-hit | short-solo | Solo | 44054 | 283.743 | 0 | 0 |
| banked-hit | short-a1 | A1 | 86662 | 144.239 | 64 | 144.239 |
| banked-hit | short-a8 | A8 | 135203 | 92.454 | 64 | 92.454 |

短组 N 的 short-a8 有 fast accept（fast V 96、fast A 2、合计 98，诊断计数 98/98/98/98/98/0/0/0/98）。该事实说明机制在短 A8 窗口可被真实触发，但短组不并入长组比较。

---

## 4. 长组同窗 V/A 吞吐中位数（每条件 3 次）

> 窗口中位取自 `raw_window[1]`；ns 与 MiB/s 是按 50 MHz 假设的推导量，不是直接测量值。V 吞吐与同窗口 A 吞吐共用同一窗口。A 速度只用同窗口 A 吞吐评价，不用不同窗口 A 完成数直接比较。
> 三 label 各自三轮技术上全部合格，因此发布中位/min/max。短组不混入。

### 4.1 V 吞吐中位数（MiB/s）

| condition | original 中位 | serial 中位 | hit 中位 |
|---|---:|---:|---:|
| Solo | 282.809 | 283.035 | 283.370 |
| A1 | 149.277 | 145.461 | 145.501 |
| A8 | 108.858 | 89.338 | 93.048 |
| A16 | 100.686 | 80.608 | 78.440 |

### 4.2 同窗口 A 吞吐中位数（MiB/s）

| condition | original 中位 | serial 中位 | hit 中位 | A jobs 中位 | A bytes 中位 |
|---|---:|---:|---:|---:|---:|
| Solo | 0 | 0 | 0 | 0 | 0 |
| A1 | 149.277 | 145.461 | 145.501 | 4096 | 16777216 |
| A8 | 108.858 | 89.338 | 93.048 | 4096 | 16777216 |
| A16 | 100.686 | 80.608 | 78.440 | 4096 | 16777216 |

A jobs/bytes 字段来源为 `raw_window[3]` 与 `w3 × 4096`，并与 `p4m.a_completed_jobs_window` 交叉核对。A 吞吐中位与 V 吞吐中位在上述 A 条件数值相等，是因为本轮二者在该窗口都完成 4096 份、每份 4096 B；相同单位本身不能推出相同速率。本报告不将其扩展为“窗口被 V 占满”或独立总线利用率结论。

---

## 5. 原版/S/N 相对变化与有限描述

### 5.1 V 吞吐相对变化

| condition | S/原版 Δ | S/原版 Δ% | N/S Δ | N/S Δ% |
|---|---:|---:|---:|---:|
| Solo | +0.226 | +0.1% | +0.334 | +0.1% |
| A1 | -3.816 | -2.6% | +0.040 | +0.0% |
| A8 | -19.520 | -17.9% | +3.710 | +4.2% |
| A16 | -20.078 | -19.9% | -2.168 | -2.7% |

补充 N/原版（描述值，非归因）：A8 约 -14.52%，A16 约 -22.09%；即 N 相对原版仍较低。A8 逐轮 V 吞吐为 104.263/92.987/93.048 MiB/s，三轮差异明显，全部保留；不能只写“平均 +4.2%”就称稳健。

### 5.2 同窗口 A 吞吐相对变化

在上述 A 条件下，同窗口 A 吞吐数值与 V 吞吐相同，因此相对变化表同上；Solo 条件下 A 完成数为 0，不构成 A 吞吐比较。

### 5.3 有限描述

- `S/原版` 同时改变分 bank、缓冲/layout 与启动条件：original 受限于旧 legacy3，S/N 为 512 缓冲；不能把 `original -> S` 的差值说成纯布局因果。
- `N/S` 是不同 BIT、不同 fresh boot 下的描述性变化；即使 fast count 在 A8/A16 非零，也不能把 N/S 中位差当作纯 enable 因果或统计显著结论。
- A8：N/S V 吞吐约 +4.2%，是描述性正值；S/原版约 -17.9%，N/原版约 -14.52%，不能与 N/S 直接抵消后归因“绕过有效”。A8 逐轮 104.263/92.987/93.048 MiB/s 差异明显，全部保留。
- A16：N/S 约 -2.7%，N 低于 S；N/原版约 -22.09%；不能宣称 N 在 A16 改善。
- A1、Solo：N/S 近似持平；Solo 的 +0.1% 不构成有意义收益声明。
- 不允许用 original 与 hit 的直接差值归因绕过效应；N/S 必须以 S 为中介，且在无更大样本和更严格对照前只能保留为描述性窗口周期差。

---

## 6. N 真实候选 / 探测 / DC / leaf / fast 触发计数

> 诊断 bank 是新增资格诊断计数，不是旧 IOMMU hit/miss/PTW；旧 HPM/miss/PTW 字段在 banked 结构下不是有效 metric，本报告不作为机制证据。
> fast 事件数是 accepted requests，不是 hit rate，也不是全部 miss 数。
> 诊断窗口语义为 `(open, close]`；跨窗口的部分计数合法，不强制 start/eval/leaf 之间跨边界守恒。

### 6.1 `banked-hit` 长组逐条件汇总（n=3）

| condition | candidate in window | probe_starts | dc_eval | dc_good | leaf_eval | leaf_good | leaf_lookup_miss | fast_accept | fast V 每轮 |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| Solo | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0/0/0 |
| A1 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0/0/0 |
| A8 | 3/3 | 18436 | 18436 | 18436 | 18436 | 18436 | 0 | 18436 | 6142/6144/6144 |
| A16 | 3/3 | 24582 | 24582 | 24582 | 24582 | 24579 | 3 | 24579 | 8193/8193/8193 |

补充：

- A8：fast A 每轮为 2，fast 合计每轮 6144/6146/6146；seen 位含 `base_s1_ptw_active, full_fast_window, candidate_in_window, probe_start, dc_eval, dc_good, leaf_eval, leaf_good, fast_accept, runtime_enable`；没有显式 `dc_msi_reject` 或 `leaf_msi_reject`。
- A16：fast A 为 0；fast 合计每轮 8193；seen 位含 `leaf_lookup_miss`；`leaf_good` 24579 低于 `leaf_eval` 24582，`leaf_lookup_miss` 为 3。
- Solo、A1：seen 并集只有 `runtime_enable`；fast 全 0，是这些条件下的科学未行使，不是技术失败。
- short-a8：fast accept 98，诊断计数 98/98/98/98/98/0/0/0/98；短组只验收，不并入长组。

### 6.2 `banked-serial` 对照

| condition | candidate in window | probe_starts | dc_eval | leaf_eval | fast_accept |
|---|---|---:|---:|---:|---:|
| Solo | 0/3 | 0 | 0 | 0 | 0 |
| A1 | 0/3 | 0 | 0 | 0 | 0 |
| A8 | 3/3 | 0 | 0 | 0 | 0 |
| A16 | 3/3 | 0 | 0 | 0 | 0 |

S 在 A8/A16 出现 `candidate_in_window` seen 位，但没有 probe_start、DC、leaf、fast accept。这说明 S 在该窗口曾出现过合格窗口谓词，但没有进入后续探测/服务路径；不能据此说 S 有 fast 命中。

### 6.3 解释限制

- `candidate_in_window` 只说明窗口内候选谓词成立；没有 probe start 时，不能推出有完整 probe，更不能推出 accept。
- DC good = 0 且没有显式 MSI reject 时，不能唯一归因 MSI；只有显式 reject 计数才支持该归因。
- 未出现 `candidate_in_window` 时，只能说明本轮在该 TL 接口/资格窗口未观测到合格候选，不能说明没有 cache 命中，也不能说明上游没有 V。
- `candidate_in_window` 已出现而 `probe_starts = 0` 时，只能说没有启动探测；不能由 probe=0 反推没有候选，本轮 S 在 A8/A16 就是反例。
- 诊断计数跨窗口采样，不假设 probe/DC/leaf 生命周期完整守恒。
- v5 资格限定在当前单任务、对齐 4 KiB、完整缓冲、DMA0/1 的冻结 workload 形状；不是通用 AXI 或多 job 无死锁证明。
- 证书内部 `fault_q` 是 sticky，且本轮未导出到板级诊断 ABI；不能从板级计数 0 反推 fault 状态。
- 已知 hit-under-miss / 完整缓冲原则不能被本轮声称新颖性；本轮只是工程基线验证与诊断。

---

## 7. 复核发现的数据问题（明确指出，不用漂亮结论覆盖）

1. **`v5-build-state.json` 的早期 stage 是历史快照，当前字段已更新**：早期交付快照写 `S15_complete_N15_running`、`hit_board = "running15"`；当前文件 stage 为 `v5_finished_analyzed_paused_by_user`，`hit_board = "15/15_complete_exit0"`，`serial_board = "15/15_complete_exit0"`，`analysis_complete = true`、`analysis_quality_issues = 0`。N `summary.json` 已 complete，finished_at 为 `2026-10-04T07:10:06Z`。因此早期 `running` 不是本轮最终事实。
2. **`hit-v5-build-acceptance.json` 的早期 pending 是历史快照，当前板测字段已更新**：当前文件写 `board_functional_acceptance = "15/15_completed_exit0"`（`actual_acceptance_utc` 为构建验收时间，早于板测完成）；没有 `board_acceptance` 字段。N runner 已 15/15 complete、exit 0、quality_issues 0。S 的 build-acceptance 也已写 `15/15_completed_exit0`，两者现已一致。
3. **初次构建失败必须保留在记录中**：S/N 两版初次 `initial_build_exit = 1`，`scope_recovery_exit = 1`；原因包括 scope 问题和 FPGA source list 漏项。后续用 `v5-shared-scope-fix.patch` 与 `v5-fpga-source-list-fix.patch` 后 `source_recovery_exit = 0`。不能只写“构建通过”而不提初次失败与修复路径。
4. **original-serial 是复用旧基线**：original-03 复用 06:07 完成的 15 项旧数据，v5 没有重新跑 original；本轮不据此推断其当时启动方式，也不能把 original 与 S/N 当作同一轮同条件三臂。
5. **S/N 不同 BIT、不同 fresh boot**：物理布局、面积、时序不同；S/N 只能作条件性描述比较，不能作纯 enable 因果，也不能作统计显著性。
6. **旧 HPM / miss / PTW 不作为证据**：`raw_observation` 中的旧 HPM 字段在本 banked 结构下不是有效 metric，不能用来补足候选/命中/PTW 结论。
7. **fast 全 0 不是质量问题**：Solo/A1 的 fast 全 0 是合法科学观察，不产生 quality issue，不剔除、不补跑。
8. **短组不能并入长组**：short-a8 有 fast accept，但短组只做验收；不能把短组正计数混入长组中位数或两段对照。

---

## 8. 本轮能说什么 / 不能说什么

### 8.1 能说

- v5 S/N 两套板测各自 15/15 complete，runner exit 0，quality_issues 0。
- N 在长组 A8/A16 真实观测到 candidate/probe/DC/leaf/fast accept；Solo/A1 没有。
- S 在长组 A8/A16 出现 `candidate_in_window` seen 位，但没有 probe/fast accept。
- 同窗 V/A 吞吐中位数已按每条件 3 次汇总；A8 N/S 约 +4.2%，A16 N/S 约 -2.7%，A1/Solo 近似持平，均为描述性。

### 8.2 不能说

- 不能说 N/S 达到统计显著。
- 不能说 original -> S 是纯布局因果；它同时含分 bank、缓冲/layout 与启动条件/layout 差异。
- 不能说 fast accept 是 hit rate 或全部 miss 数。
- 不能把 TL 接口/资格窗口未出现 `candidate_in_window` 外推为没有 cache 命中或上游没有 V。
- 不能用旧 HPM/miss/PTW 作为 N 机制证据。
- 不能从板级 fast=0 或诊断 0 反推 v5 证书 `fault_q` 状态。
- 不能把已知 hit-under-miss / 完整缓冲原则声称为本轮新颖性。
- 不能把 v0 的 implicit wiring defect 当作 A8 hang 的唯一已证根因；该根因未被唯一证明。
- 不能把 v4 无候选扩展到“无缓存命中”或“上游没有 V”；v4 的观察只限于其 TL 接口/资格窗口。

---

## 9. 结论

本轮 v5 S/N 板测从技术完成度看是完整的：S/N 各 15/15，quality issues 0。机制层面，N 在 A8/A16 首次给出真实非零 candidate/probe/DC/leaf/fast accept 计数，Solo/A1 没有；S 在 A8/A16 有 candidate 谓词但没有 probe/fast。性能层面，N/S 同窗中位差在 A8 为正、A16 为负、A1/Solo 近似持平；但 N/原版 A8 约 -14.52%、A16 约 -22.09%，N 相对原版仍较低。上述均为描述值，且受不同 BIT、不同 fresh boot、原版复用旧基线等限制，不能作因果或显著性结论。

数据问题也必须保留变更历史：`v5-build-state.json` 早期快照的 `N15_running`、`hit_board = running15`，以及 `hit-v5-build-acceptance.json` 早期交付的 `pending` 均为历史快照；Codex 已按实际返回把当前字段更新为 S15/N15 完成、`board_functional_acceptance = "15/15_completed_exit0"`、`analysis_complete = true`、`analysis_quality_issues = 0`。S/N 初次构建失败及修复补丁必须保留。所有样本保留，不剔除、不补跑、不自动重跑。

按用户要求，本轮分析完成后暂停；未启动下一轮。

---

