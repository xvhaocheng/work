# 方法结果复核报告（2026-10-04）


## 0. 科学结论、状态与证据入口

- 科学结论：方法 462 项已全部离线通过；加上既有 E2 64 项，共 526 项已完成离线验收。p5 在公平匹配下胜固定 gap，但没有稳定优于简单计数对照的证据；单一启动上的反向效果存在阶段/顺序混杂，不能归因方向。
- 当前状态：E2 局部缓存替换与 E3 服务阻塞证据保留；机制分支本阶段收尾，方法优势仍是未解决关卡；暂停把 p5 当已确认主创新，不自动加入缓存分区硬件。
- 证据入口：逐行明细见 [summary-final/summary.zh.md](summary-final/summary.zh.md)，机器统计见 [accepted-evidence.json](accepted-evidence.json)，下一步计划见 [../../docs/research-method-next-20261004.md](../../docs/research-method-next-20261004.md)，既有 E2 复核见 [../e2-results-review-20261003/RESULTS.md](../e2-results-review-20261003/RESULTS.md)。
- 原始 README 的采集限制已被用户当前授权覆盖，且保留原文不改。

## 1. 环境与完整性

| 项目 | 值 | 证据 |
|---|---|---|
| Linux | `00093-gc0f5e13ea81f`，role2 | Codex 已核查；本轮非新板测重跑 |
| Boot | 单一 boot `93406a68-fc8a-4040-9dc6-ae4e4a670d36` | Codex 已核查 |
| 收集结束 | 2026-10-04 00:50:01 JST | Codex 已核查 |
| Summary cases | 462 期望，462 ok，0 failed，0 missing | [机器证据](accepted-evidence.json) |
| Case records | 462 条，unique ids 462 | [机器证据](accepted-evidence.json) |
| 回传文件完整性 | before 10041，after_existing 10041，changed_or_removed 空，added 空，unchanged true | [机器证据](accepted-evidence.json) |
| stderr | 非空数 0 | [机器证据](accepted-evidence.json) |
| 内核 warning 匹配 | 仅 `m-short-unlimited-a8`：`hrtimer: interrupt took 13930720 ns` | [机器证据](accepted-evidence.json) |
| exit/taint/uname | exit0、taint0、uname 已独立核查 | Codex 已实际执行核查；本报告仅引用 accepted evidence 口径 |

说明：taint0 不等于“无 warning”。本数据保留唯一 hrtimer WARN，且不把它写成无 warning。

## 2. 阶段计数与 V 共同窗口

462 项按阶段分开：短 9、参考 12、校准 89、正式 192、边界 40、反向 120。证据见 [机器证据](accepted-evidence.json)。

| 阶段 | 运行数 | V 共同窗口 miss=0 | V 共同窗口 miss=2 | 备注 |
|---|---:|---:|---:|---|
| boundary | 40 | 40 | 0 | [机器证据](accepted-evidence.json) |
| calibration | 89 | 86 | 3 | [机器证据](accepted-evidence.json) |
| formal | 192 | 180 | 12 | [机器证据](accepted-evidence.json) |
| holdout | 120 | 114 | 6 | [机器证据](accepted-evidence.json) |
| reference | 12 | 9 | 3 | [机器证据](accepted-evidence.json) |
| short | 9 | 9 | 0 | [机器证据](accepted-evidence.json) |
| 合计 | 462 | 438 | 24 | 24/462 miss=2，其余 438 miss=0 |

逐行明细见 [summary-final/summary.zh.md](summary-final/summary.zh.md)。全部数据保留，不因 miss=2 或唯一 WARN 删除。

## 3. 方法、公平规则与指标边界

方法名称按当前冻结实现：`fixedgap`、`p1` 字节、`p2` 请求、`p3` miss、`p4` 事件加权、`p5` 实占、`p6` 仅 V 反馈、`p7` stale actual。设计定义见 `docs/research-two-bit-v2.md:29`。

公平比较规则固定如下：

- V 两侧各组中位速率均需 >= 目标。
- |medianL - medianR| / mean(medians) <= 5%。
- 每 run E23/E20 不足比例的组均值差 <= 10 个百分点，且每 run 有效完整窗口 > 0。
- A 百分比 =（组均值方法 - 组均值基线）/ 组均值基线，不是表中中位数之比。
- CI 基于配对 run 差值的重采样，仅用于描述，不作为显著性判断。
- 5% 为工程参考阈值，不是统计保证。
- 旧校准 unmatched 含 fixedgap120、p1/p2/p6t150、p7 全目标等，仅作信息保留；formal 重新匹配，不根据校准删除数据。
- 462 项按短 9、参考 12、校准 89、正式 192、边界 40、反向 120 分开报告。

指标边界：

- E25 许可关闭发生在 armed 期间，没有 frontend wait 信号，不是实际 stall，不能除以 C19 当实际阻塞。
- E2E 只按 `ctl_did==A`，不筛状态，不是实际 cost。`p5` 正式 t170 r3 的 E2E 为 1765750，其他约 14M，差异保留，不据此诊断 bug，该字段不进收益/匹配。
- RTL 口径（`dma_research_policy.sv`）：E2E=归属标识 A 的全部周期，不筛状态；p5 实际计费只对应 E32 记录的 REQ_TRANS/AWAIT_RESP 周期。不能用 E2E 差异诊断 bug。
- E39 armed 与 C19 共同窗口口径不同。
- V 完整活动窗不足比例不是硬延迟 SLA；完成粒度为 4KiB 离散完成，deadband 为 4096。
- 无旧 BIT 对照，故未测零开销。
- holdout 未参与调参，但不是独立重启；它是同启动工程重复，方向与阶段仍混杂。

## 4. 关键公平正负例

以下只列关键例。完整逐行见 [summary-final/summary.zh.md](summary-final/summary.zh.md)。

| 阶段/顺序 | 目标 | 对照 | 保护匹配 | p5 A 增益 | 区间口径 | 说明 |
|---|---:|---|---|---:|---|---|
| formal（正向阶段） | 150 | fixedgap | 是 | +15.289% | 区间全正 | 公平匹配正例；[机器证据](accepted-evidence.json) |
| formal（正向阶段） | 170 | fixedgap | 是 | +24.211% | 区间全正 | 公平匹配正例；[机器证据](accepted-evidence.json) |
| formal（正向阶段） | 170 | p1 字节 | 是 | -6.464% | 区间全负 | 匹配反例；[机器证据](accepted-evidence.json) |
| formal（正向阶段） | 170 | p2 请求 | 是 | -6.110% | 区间全负 | 匹配反例；[机器证据](accepted-evidence.json) |
| formal（正向阶段） | 170 | p6 仅V反馈 | 是 | -3.791% | 区间跨0 | 与反向 +40.672% 形成方向/阶段混杂；[机器证据](accepted-evidence.json) |
| holdout（反向阶段） | 170 | p6 仅V反馈 | 是 | +40.672% | 区间全正 | 反转复现问题，非简单对照主问题；[机器证据](accepted-evidence.json) |
| holdout（反向阶段） | 120 | p4 事件加权 | 是 | +12.878% | 区间全正 | 正例保留；[机器证据](accepted-evidence.json) |
| holdout（反向阶段） | 150 | p4 事件加权 | 是 | +5.247% | 区间跨0 | 正例保留但区间跨 0；[机器证据](accepted-evidence.json) |
| holdout（反向阶段） | 170 | p1 字节 | 是 | +0.963% | 区间跨0 | 未达 5%；[机器证据](accepted-evidence.json) |
| holdout（反向阶段） | 170 | p2 请求 | 是 | +2.697% | 区间跨0 | 未达 5%；[机器证据](accepted-evidence.json) |

表中 formal 为正向阶段，holdout 为反向阶段；区间口径为配对 run 的单位差值 MiB/s 区间（非百分比），仅描述，不作显著性断言。

其他需要注意的正结果：p5 相对 p3/p6/p7 的原始正差值（formal p3 t120 +6.704%、formal p6 t120 +17.758%、formal p6 t150 +23.080%、formal p7 多目标大正增益、反向 p3 t120 +8.807% 等）均因保护不匹配，不能作为公平收益；CI 跨 0 仅单独报告，不取消工程 5% 门槛。反向 fixedgap 在 120/150/170 三个目标均未满足公平匹配，因此不能说复现正式 fixedgap 收益。

## 5. 边界与参考

- 边界 p5 vs fixedgap：A1 V 中位 149.581，A16 V 中位 147.257，均未达 150。A1 差距虽小，也不事后放宽阈值。证据：[机器证据](accepted-evidence.json)，组值见 [机器证据](accepted-evidence.json)。
- 当前同版参考：Solo 283.099、A1 149.507、A8 108.904、A16 100.930 MiB/s。证据：[机器证据](accepted-evidence.json)。
- 边界与参考不用于宣称创新成立，只用于界定当前版本能力和边界。

## 6. 结论

1. 功能全部成功不等于创新成立。462/462 summary ok、完整性无变化、stderr 为空、exit0/taint0 均已核查，但结果本身不支持把“功能成功”写成“创新已成立”。
2. p5 在正式 t150/t170 对 fixedgap 有公平匹配下的 A 均值增益 +15.289%/+24.211%；但正式 t170 对简单字节 p1、请求 p2 为 -6.464%/-6.110%，是匹配反例。p5 未获普遍优势。
3. 反向 p5 相对 p6（目标170） 为 +40.672% 且匹配，但正式为 -3.791%；方向与采集阶段混杂，不能归因方向。反向 p5 相对 p4（目标120） +12.878%、t150 +5.247%（CI 跨 0）作为正例保留，不能遗漏。
4. 更准的成本测量不等于控制更好。同外层反馈可补偿稳定负载上简单代理的误差，这是待验证解释，不是本数据证明的根因。
5. p5 未获普遍优势，不等于该信息在所有未来工作负载无价值，也不等于论文没价值或创新已成立。整体路线：E2 局部缓存替换与 E3 服务阻塞证据保留，机制分支本阶段收尾；方法优势仍是未解决关卡，暂停把 p5 当已确认主创新，不自动加入缓存分区硬件。
6. 下一步 88 项是看过旧数据后的复核，不是全新盲测；主张和筛选规则必须提前冻结，并按启动分别报告。

DeepSeek 起草证据整理、统计程序和文档；Codex 核对关键来源、实际执行验证并作最终决策。
