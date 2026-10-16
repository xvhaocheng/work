# IOMMU 固定链翻译进度前沿：一次数学图运行的结论与停止点（2026-10-08）

## 结论先行

1. **本轮只做了一次数学图运行，覆盖 6450 个配置，与 root 的直接 max-gap 判别 mismatch = 0。**
2. 在受限模型内，**普通 checkpoint + 主动提前让路达到每请求 15 笔、两请求合计 30 笔的下界**。因此，**固定链上的调度停止作为独立创新候选**：相同原文字段的普通方法已经达到本合同最低 PTE 工作。
3. 这里 **q 是“最多连续服务 PTE 数”**，不是 cycle；**B 是“单个 suspended frame 可保存当前 checkpoint 的位数”**，不是面积、LUT 或总状态。二者都是模型量，不能直接外推为硬件周期或面积最优。
4. 受限命题是：在列明的普通保存 primitive 族里，**存在可完成策略当且仅当 q ≥ G(B)**，其中 G(B) 是相邻可保存 checkpoint 的最大间距。它只保证“存在一种完成策略”，**不保证任意公平策略都完成**；任意过早切换、每轮只服务一小段再丢进度，仍可 livelock。
5. **30 笔下界只在无 cross-request coalescing、无 PWC、每次读只推进当前请求、两个请求各固定 15 步时成立。** 若允许共享同一 PTE 读结果、合并读取或 cache，30 就不再是一般下界。
6. 关键例：
   - q = 3，B = 77，`semantic_chunks`：路径 `0→3→4→7→8→11→12→15`，每轮 `3/1/3/1/3/1/3`，**零重读**，双方共 30 笔。
   - q = 4，B = 36，`semantic_restart`：路径 `0→4→8→12→15`，每轮 `4/4/4/3`，**零重读**，双方共 30 笔。
   - q = 3，B = 36：无解；0→4 缺口无法跨越，搜索耗尽。
7. **q = 2 专用 80 bit 只是额外纸面普通构造**：路径 `0→1→3→5→7→9→11→13→15`，三位 tag + 最大 payload 77 bit。它**没有进入五族 graph 枚举**，也**没有证明全架构最小编码**；不能把 mixed 族 81 bit 当作 q = 2 的一般下界。
8. 五族的 B 范围到最小 q 表见 [`../out/translation-progress-frontier-20261008/codex-result-acceptance.md`](../out/translation-progress-frontier-20261008/codex-result-acceptance.md)。本文只列关键 3 例。

## 1. 模型与边界

固定合同：两个已存在的有限普通翻译请求 A/V，一条可切换 PTW，静态正常三层 Sv39 S × Sv39x4 G，全部 4 KiB 叶，DDT/PDT 已解析。一次 PTE 读只推进当前请求；不允许跨请求共享 PTE 结果或合并读取；无 PWC、无新增旁路。请求按 PTE 服务数计，不计 cycle、DDR 带宽和 R 延迟差异。

单请求正常链为：

`G1 G2 G3 S1 | G1 G2 G3 S2 | G1 G2 G3 S3 | G1 G2 G3(done)`

位置 p 是已完成的有效 PTE 数，不是免费寄存器。B 只计一个 suspended frame 中保存当前 checkpoint 的位宽；原请求/context C、active 完整执行寄存器、owner/valid、terminal 归属、在途状态等均另行双方同计。因此 B 不是总 LUT/面积，也不是任意 B-bit 编码的信息下界。

只要两个请求都未完成，当前请求可执行 `x ∈ [1, min(q, 15-p)]` 笔正常 PTE，然后交接；允许提前让路，不能强迫普通方法填满 q 浪费工作。另一个请求完成后，剩余请求可连续完成，因为不存在要等待的同伴。观察对象是两个有限请求的总 PTE 工作/完成可达性，不能扩成持续到达有界延迟。

## 2. 五族与一次运行

五族普通 primitive：

- `whole_restart`：非终态只回 0。
- `full_current`：当前 p 的 data_bits + 固定 5 bit generic phase tag，符合 §44 27-state 编码；空间不足可回 0。
- `semantic_restart`：保存 semantic 前缀；p<12 时 b=4*floor(p/4)，p∈[12,14] 时 b=12。
- `mixed_indexed`：当前 p 用 data_bits + 4 bit 固定 15 步链标签，也可保存 b 或回 0。
- `semantic_chunks`：普通 G 整段返回后保存 mapped S-PTE page，下一 turn 只读 S 后主动交接；只支持 checkpoint `{3,4,7,8,11,12}`，最大 payload 74 + 3 bit tag = 77 bit。

运行账本见 [`../out/translation-progress-frontier-20261008/execution-ledger.json`](../out/translation-progress-frontier-20261008/execution-ledger.json)：`math_model_runs = 1`，`exit 0`，`elapsed_seconds = 0.7781777381896973`，`stderr` 空；`DUT_builds = 0`，`DUT_runs = 0`，`production_RTL_changes = 0`，`Linux_changes = 0`，`BIT_builds = 0`，`JTAG_board_SD_actions = 0`。6450 = 5 族 × 86 预算 × 15 服务额度；每族 1290。所有数据是数学图，不是 DUT/FPGA 实验。

运行输出 [`../out/translation-progress-frontier-20261008/frontier_report.json`](../out/translation-progress-frontier-20261008/frontier_report.json) 与 root 直接 max-gap 判别一致，mismatch = 0；所有可完成配置 `min_work = 30`。无解配置确实耗尽 heap；成功配置早停，不把尚未定距节点称不可达。

## 3. 关键例

| 例子 | 保存族 | B | q | 路径 | 每轮 PTE 数 | 结果 |
|---|---:|---:|---:|---|---|---|
| q3 强普通点 | `semantic_chunks` | 77 | 3 | `0→3→4→7→8→11→12→15` | `3/1/3/1/3/1/3` | 零重读，双方 30 笔 |
| q4 restart 点 | `semantic_restart` | 36 | 4 | `0→4→8→12→15` | `4/4/4/3` | 零重读，双方 30 笔 |
| q3 不足点 | `semantic_restart` | 36 | 3 | 不能跨 0→4 | — | 无解，搜索耗尽 |

五族 B 范围→最小 q 表、原始路径与 cycle 说明见 [`../out/translation-progress-frontier-20261008/codex-result-acceptance.md`](../out/translation-progress-frontier-20261008/codex-result-acceptance.md) 与 [`../out/translation-progress-frontier-20261008/codex-proof-and-decision.md`](../out/translation-progress-frontier-20261008/codex-proof-and-decision.md)。

## 4. 原始文献对照

**Barr、Cox、Rixner，ISCA 2010，Translation Caching: Skip, Don’t Walk。** 官方作者 PDF：[作者论文 PDF](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)。其 §3.2/§3.3 支持 virtual-prefix partial translation、path entry 保留多个中间 PPN；§4.2 支持跨层共享槽、按 reload cost/level 保留；表 1/2 明确区分 tag 与 data 成本。它支持本报告 q2/q1 方向的 partial/path 普通能力，但**没有提供 77 bit 帧或 q3 暂停保证**。

**Bhargava、Serebrin、Spadini、Manne，ASPLOS 2008，Accelerating Two-Dimensional Page Walks for Virtualized Systems。** 正式 DOI：[正式 DOI](https://doi.org/10.1145/1346281.1346286)；本轮下载镜像：[论文 PDF](https://www.cs.columbia.edu/~cdall/candidacy/pdf/Bhargava2008.pdf)。其 §4.2 的 NTLB 以 guest PTE 的 GPA 为 key 缓存 G 子 walk 叶映射，命中后跳过完整 nested 子 walk。它支持本报告 q3 的普通保存能力，但**没有给出 77 bit 帧或 q3 持续暂停保证**。

**Hong、Kung，STOC 1981，I/O Complexity: The Red-Blue Pebble Game。** DOI：[正式 DOI](https://doi.org/10.1145/800076.802486)；镜像：[论文 PDF](https://perso.ens-lyon.fr/loris.marchal/docs-data-aware/hong_kung_red_blue_pebble_game_STOC81.pdf)。它提供固定 DAG、fast/slow store、hold/drop/recompute/I/O 的一般前史，但**不能仅凭它宣称完整 nested frontier 都已有**，也不能直接推出 phase-dependent bit/quantum/完成定理。

## 5. 下一步与停止线

固定链结论不能自动推广到未知 PTE 内容或持续到达。下一步只允许在同 live 到达/真实路径、同语义、同总资源/信息条件下，与 ordinary dynamic checkpoint / variable-size cache / pinned PWC 比较，判别：

> **真实共享翻译干扰里，复用和保留进度是否存在不可兼得约束。**

如果候选只是已有 checkpoint 点选择、提前 yield 或 phase 编码变化，不立新。未知路径或持续到达更难，**不自动产生创新**。若没有普通族达不到的成本点，就停止固定链作为独立创新。当前生产 PTW 没有外生任意时刻暂停接口；不能把它虚构为既有平台约束，也不能借此恢复新颖性。

主线仍是共享 IOMMU 性能干扰。本分支只收束为受限普通 checkpoint 可达性数学收尾，不要求 DUT/build/BIT/JTAG/板卡/SD，不要求重跑；production RTL/Linux/tests 未改。最新实际完整拷贝仍为 §43。

## 6. 出处与分工

- 本轮脚本：[`../out/translation-progress-frontier-20261008/frontier_model.py`](../out/translation-progress-frontier-20261008/frontier_model.py)
- 运行输出：[`../out/translation-progress-frontier-20261008/frontier_report.json`](../out/translation-progress-frontier-20261008/frontier_report.json)
- 执行账本：[`../out/translation-progress-frontier-20261008/execution-ledger.json`](../out/translation-progress-frontier-20261008/execution-ledger.json)
- 验收：[`../out/translation-progress-frontier-20261008/codex-result-acceptance.md`](../out/translation-progress-frontier-20261008/codex-result-acceptance.md)
- root 模型合同：[`../out/translation-progress-frontier-20261008/codex-model-contract.md`](../out/translation-progress-frontier-20261008/codex-model-contract.md)
- root 证明与裁决：[`../out/translation-progress-frontier-20261008/codex-proof-and-decision.md`](../out/translation-progress-frontier-20261008/codex-proof-and-decision.md)
- 两席 artifact：[`../out/translation-progress-frontier-20261008/map-proof-agent.md`](../out/translation-progress-frontier-20261008/map-proof-agent.md)、[`../out/translation-progress-frontier-20261008/rules-proof-agent.md`](../out/translation-progress-frontier-20261008/rules-proof-agent.md)

分工：DeepSeek 起草脚本和一次修订，并起草本报告；Codex root 设计合同/证明、亲读关键原文与代码、执行唯一 math run、实际改文档并最终审定；2 名 native 研究席只做关键证明/文献质疑，无运行；Claude 未调用。
