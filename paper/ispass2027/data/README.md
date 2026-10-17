# ISPASS 2027 论文数据交付

本目录从历史原始 JSON 和已验收复核 JSON 确定性提取数据，没有从 Markdown 表格抄数，也没有修改、重建或重跑历史实验。当前交付是 FPGA 平台刻画论文的数据材料；失效干扰的行为 DDR 仿真另表保存。本文数据层不提出新机制，也不恢复旧实验或 MIG 工作。

## 文件与重建

本目录是论文选入数据，完整历史轮次与后续修正查[实验总账](../../../docs/experiment-ledger/README.md)。E3 v6保留历史数值，但必须同时使用[时序证据更正](../../../docs/research-history/evidence/daily-2026-10-16-resumed-2026-10-17/e3-timing-correction.md)，不能把已入表等同完成时序验证。

| 文件 | 用途 |
| --- | --- |
| [board_results.csv](board_results.csv) | 板测长表27544行：历史4096＋G1 828＋统一100点12480＋E1b新增100点10140；行数不是实验重复次数 |
| [invalidation_sim_results.csv](invalidation_sim_results.csv) | 28 个配置的独立失效仿真，980 行；不计入板测样本数 |
| [key_rows.json](key_rows.json) | 按论文证据主题组织的稳定 `row_id`、值和条件入口 |
| [source_manifest.json](source_manifest.json) | 本次读取的每个输入文件的相对路径、SHA256、长度及可用的历史哈希对照 |
| [validation.json](validation.json) | 实际提取、原始记录关键计算、逐行定位与输出哈希检查结果 |
| [extract_results.py](extract_results.py) | 标准库提取器；只写本目录生成文件，历史源只读 |
| [incremental_sources.json](incremental_sources.json) | 已安装真实G1与统一100点的显式 scalar locator 增量清单 |
| [板测导入说明](board-import.md) | 新 G1/正式 100 点导入、失败与跨 boot 计数规则 |
| [import_board_runs.py](import_board_runs.py) | 从指定原始 run 准备候选 scalar locator，并可显式安装 |
| [importer-validation.json](importer-validation.json) | 隔离 fixture 与主提取器接口的实际验证回执 |
| [实际 G1 导入包](board_imports/g1-20261017-03/summary.json) | 已安装9次实际测量的唯一恢复系列，完整失败/恢复来源链保留 |
| [统一100点导入包](board_imports/baseline100-20261017-01/summary.json) | 已安装ABA 25/50/25，100次接受、0失败、3个实际boot |
| [统一100点简表](baseline100-summary.md)／[关键行JSON](baseline100-summary.json) | 10个DS×条件的完整n=10结果及稳定row_id；DS50路径vr1入口 |
| [validation-notes.md](validation-notes.md) | 验证范围、描述性统计与有限样本限制 |

在工作区根目录执行：

```sh
python3 paper/ispass2027/data/extract_results.py
python3 paper/ispass2027/data/extract_results.py --check
```

第一条重建 CSV、关键行索引、来源清单和验证回执；第二条在内存重建并要求生成字节完全一致，随后核对已保存清单中的每个输入哈希。两条都不写历史目录，不运行历史复核脚本，不启动 RTL 或板测。本次初始验证覆盖 326 个源文件，其中 174 个另与历史保存哈希相符；6578 项源定位、关键计算与语义检查通过。后续增量会使计数变化，以 `validation.json` 为准。

## 长表口径

必需字段是 `row_id,experiment,condition,metric,statistic,value,unit,n,bitstream_id,kernel_id,raw_path,source_locator`。另保留 `evidence_level,value_status,identity_status,boot_id,sample_set`，用于区分板测/仿真、已观测值/推导值/缺失、身份核验状态、启动与采集段。

- `raw_path` 相对于 `/home/xvhaocheng/work`。`source_locator` 通常是 RFC 6901 JSON pointer；包含 `/` 的字典键按 `~1` 转义。路径可能指向已验收复核 JSON，而其原始记录和哈希也在 manifest 中。
- 少量直接从原始记录聚合的 E1 A 侧值，以及仿真的 M0 归一化比值，使用显式 JSON 表达式定位。`min/median/max` 列出每个输入的路径和 pointer；`divide` 分别列出分子、分母。没有不带来源的常量或隐式分母。
- `n` 始终是该行统计覆盖的重复试次数或配对数；`sample` 行通常为 1。E2 的 `target_count` 行以翻译目标为观察单位，每个试次观察源/目标两个翻译键，在 `condition` 明示 `observation_unit=translation_target;targets_per_trial=2`。`value` 保留原始分类目标计数，`n` 仍为重复试次数；两个目标不是两次独立重复。
- 各条件保留角色、DS、布局、启动、采集段及测量窗口。软件总耗时、HPM 非原子软件快照、硬件 V 窗口、启动阶段、DONE 到软件读取、A 全部尾部完成分别标注。不同窗口的值不能相减冒充未观测的 CPU 或纯 PTW 费用。
- `min/median/max` 是有限试次的描述性统计。P4M 的 `q1/q3/iqr` 沿用历史分析器的 Type 7 线性插值，并从逐试次值重新计算核对；它们不是置信区间。
- 路径 `*_mean_cycles` 是每个试次内 `sum/full` 后，在重复试次间再取表列统计量。既不是 p99，也不是整批时间占比；读写及多事务阶段有重叠，不能相加解释完整慢化。
- `value=unknown` 表示未观测，配合 `value_status=unknown/no_trigger`；不会留成易被解析器默补的空单元格或补成 0。Solo 没有 A：部分历史系列省略A计数，新增系列用not_applicable/unknown及有值n=0表示；均不把不存在的A计数补成0。

## 覆盖与正式样本边界

| 数据族 | 本次选入 | 主要来源 |
| --- | --- | --- |
| E1 | 144 条原记录中预定的最后 80 条主样本，2 角色 × 4 条件 × 10；16 条 E0 和 48 条敏感性保留在原始文件，不混入正式统计 | `mac-e1-stage-20260916-032650-run-rbo67pvd` 的 `records/assessment/server-review.json` |
| 自主 batch | main 60 条，2 角色 × Solo/A1/A8 × 10；ext 30 条只含 V0/A1 的 A16/A32/A64 × 10 | `mac-batch-main-20260916-112954-run-9sa69rhc` 与 `mac-batch-ext-20260916-114036-run-8pjv_djc` |
| DS0 | V0/A1，6 条件各 10 条，DS0/0；不与 DS50 合组 | `mac-batch-ds0-20260916-132332-run-ebgxhirr` |
| DS50 回切 | V0/A1，Solo/A1/A8 各 10 条，DS50/50；单独保留回切批次 | `mac-batch-ds50-20260916-135544-run-eawziww2` |
| path | 30 个正式试次，3 条件各 10；短验收不混入 | `mac-path-main-20260916-164633-run-vvguros8` |
| E3 v6 | 3 对短验收 + 8 对正式，22 轮；逐对与逐轮保存，不把不同前史或 D0/D0 合成 +128 对 | `e3-controlled-review-20260918/server-review.json` 及其原始 pairs、run 和 console |
| E2 | 全部 64 个已验收运行：boundary 30、prefix 30、short 4；正式组各 5 次、短例各 1 次 | `e2-results-review-20261003/accepted-summary.json`、`verified-extract.json` 和 64 个原始 `record.json` |
| P4M | 全 60 个有限运行；acq25、retained6、remaining29 独立保留；原在线 53 与离线接受 7 均保留 | `p4m-complete-review-20261002/summary.json`、`trials.json` 与 60 个原始 JSON |
| composition | clean 与历史隔离两侧，各按既有条件选入 18 个性能例；A8 含 S/N/P/PN，A1 只含 P/PN；不跨 boot 聚合 | `composition-isolated-20261007/comparison.json` 与两侧每例 `meta/record.json` |
| 真实G1 | 9个唯一实际试次，Solo/A1/A8各3次；旧解析失败与恢复链保留，未补采 | `paper/ispass2027/board/runs/g1-20261017-03` |
| 统一100点 | 实际ABA：DS50前25、DS0中50、DS50后25；5条件×2种DS×10次，3个boot，全部接受 | `paper/ispass2027/board/runs/baseline100-20261017-01` 三个原始子run；简表见上 |
| 失效仿真 | 24 个主配置 + 4 个 unmap-only 对照；每配置一次确定性执行 | `invalidation-interference-20261017` 的验证、selected-inputs 和 28 个原始 summary |

历史批次名相对 `iommu-platform/out/`；新板测列出工作区根目录相对路径。Source manifest 给出实际读取文件的完整相对路径。

## 论文取数的稳定入口

`key_rows.json` 是完整可导航入口，下面只列常用行名模式；每个最终数字仍可沿 CSV 回到源 JSON。

| 用途 | `row_id` 示例或模式 |
| --- | --- |
| E1 同轮软件慢化 | `e1.v0-A8.paired_slowdown.median`；角色交换将 `v0` 改 `v1` |
| E1 A1 零 miss 边界 | `e1.v0-A1.victim_iotlb_miss.median` 与 `e1.v0-A1.aggressor_iotlb_miss.median` |
| 自主供给后的硬件时间 | `batch_main.v0-A8.hardware_ms.median`、`batch_main.v0-A8.overlap_percent.median` |
| DS0 与回切 | `batch_ds0.v0-A8.hardware_ms.median`、`batch_ds50_retest.v0-A8.hardware_ms.median`；分母 A1 要取同一批次 |
| 共享路径 | `path.A8.v_r_1_mean_cycles.median`、`path.A8.v_w_1_mean_cycles.median`，与 A1 同名字段对照 |
| 最强时序匹配 E3 样例 | `e3v6.main-case6.delta_A_remaining.paired_difference` 与 `delta_V_remaining`；其余正式互补对是 case2/4/8，全部保留 |
| E2 W7/W8 布局边界 | `e2.boundary-w7-forward-n112.d8d_v_miss.median` 与 `e2.boundary-w8-forward-n112.d8d_v_miss.median`；reverse 分开 |
| P4M 各采集段 | `p4m.acq29_newboot-gap1000.v_mib_s.median`、`p4m.acq29_newboot-gap1000.a_window_mib_s.median`；另外两段前缀为 `acq25_origboot`、`acq6_retained` |
| P/PN 历史隔离后退化 | `composition.isolated_history-b1-a8.w_pct_change.paired_difference`，另有 b2/b3；`a_miss_diff` 同条件核数量相等 |
| 单独失效仿真 | `invalidation_sim.same-b4-m1-full-api.B_time_over_M0.normalized_ratio`；只在仿真表中 |

E3 热态未触发控制的 `delta_A_remaining/delta_V_remaining/delta_V_wait/delta_V_own` 写为 `unknown`，标记 `no_trigger`；触发计数 0、实际插入延迟 0、真实观测的整次硬件时间仍是有效诊断读数。不能把这些热态对当成经历了真实 +128 周期延迟的零效应控制。

P4M 配对百分比逐 block 保存。跨采集段或跨 power 的配对通过 `condition` 和 `sample_set=paired_cross_acq` 显式标出；本表不导入跨启动的 `all60` 聚合。单段中位数之比与同 block 配对比值的中位数不是同一统计量。A 同窗吞吐以实际共同窗口完成量和该窗口时间计算；`a_whole_completed` 含 V 后的尾部，不能用它除以 V 时间当 A 同窗吞吐。carry-in 在原始/复核记录中保留。

## 身份与推论限制

历史 `bitstream_id=expected_sha256:…` 是原始运行元数据中的预期或当时声明哈希；`declared_sha256:…` 是 E2 板端记录中的声明。它们与 `identity_status=loaded_hash_unknown` 一起使用，均不表示当时真实加载字节已独立核验。没有记录的 bit 哈希保持 `unknown`。不能把当前归档文件的 hash、当前分支 HEAD 或后来整理提交倒填为旧运行身份。

`kernel_id` 来自原始运行元数据、串口 Linux version、E2 板端汇总或 P4M params 的内核发布串；它不是对 SD 字节或完整运行源码的补认证。composition 两个真实 boot ID 分开；P4M 断电后的 boot 有实录，断电前两个采集段的连续启动依据沿用历史日志，完整 boot ID 缺失则写 `unknown_pre_powercycle`。

E1 是软件窗口现象，batch 是自主供给下的硬件压力，DS 汇合多类请求。E2 是特定布局的驻留—移除—首次再用因果链，E3 是独立 64B 受控配对的局部等待传播。它们不能合并成“自然 4KiB 慢化全部来自某个已证明的单一原因”。P4M 是固定 gap 的前台/后台取舍，composition 是该实现/参数下的组合退化，不推断通用最优或机制创新。

失效仿真中的 `T_A` 为最后 COPYDONE，`A_final_flush_end` 含最终 flush，`T_B` 为 B 全部完成；共同分母取匹配域、B 页数和固定工作量的 M0。`B_invalidation_attributed_misses` 来自 RTL 驻留键失效后首次 miss 归因，不等于把所有超过 M0 的 walk 都算成失效。`cq_entry_cycles` 是队列条目驻留面积，单位 `command_simulation_steps`，不是 CQ FSM 全部执行时间。DDR 为记录中的行为服务模型，Linux/CPU 未执行，不能换算成 FPGA 纳秒或 Linux 原生开销。

## 新 G1 与统一基线增量入口

新运行不因出现一个目录就自动进入论文表。将精确指向 JSON 数值的行描述追加到 `incremental_sources.json` 的 `rows` 后运行同一提取器。每个描述必须包含 CSV 全部字段，唯独省略 `value`：值由 `raw_path` 与 `source_locator` 读取，禁止手填。`raw_path` 仍相对工作区根目录，根数组的第一个元素写 `/0/…`。

`evidence_level` 必须为 `fpga_board`；需要 `row_id` 全局唯一，包含实际角色、DS、boot、采集段、测量窗口；未知身份如实填写。一个 JSON 可提供多行，不同条件不得预先混合。JSON 里无值应为 `null` 且 `value_status` 为 `unknown/no_trigger/not_applicable`。提取器会把新增原始 JSON 纳入来源哈希，并对行数值和稳定 ID 做同样检查。新基线是否有真实加载身份、时钟/时序和同负载资格，仍由该轮的原始 run manifest 与验收决定，导入本身不授予这些资格。

真实G1已安装：[导入包](board_imports/g1-20261017-03/summary.json)，9个唯一实际试次、每条件3次、828个标量行。旧解析失败与恢复链完整保留，未补采。

统一100点已独立安装：[导入包](board_imports/baseline100-20261017-01/summary.json)，100个原始试次全部接受，失败/未保留0、重复点0，3个实际boot。当前实际布局为ABA 25/50/25；DS0每条件10次来自中段单boot，DS50每条件10次显式合并前后两个boot，并保留两段各5次的独立统计。后续runner默认AB 50/50，每配置测完再切换；新布局不是对这100点采集历史的改写，不将每条件10次称为10次独立重启。

DS0的Solo/A1受害者HPM miss中位为0，A1双方为0；A8/A16/A64的受害者HPM miss中位为2，属于如实保留的合法差异，没有筛除这些样本。该指标来自非原子软件HPM快照，不能将这些条件写为“V零miss”，也不能只据此认定miss发生在硬件V计时窗口内。DS50完整10次的各条件V miss中位为0，不能把它推广到DS0或其他窗口。

[图表映射](../figure-rows.csv)的图2选择完整n=10硬件时间、吞吐和有值miss中位；图3保留历史path独立系列，另加入统一100点Solo/A1/A8的V读写各4阶段均值（每试次sum/full，再取10次中位），不重复选择前后各n=5的DS50分段。

## E1b DS0/25/50/100四点曲线

[本轮导入包](board_imports/ds-sweep-20261017-01/summary.json)只包含新增DS25/DS100共100次，
两个phase各50次、五条件各10次，无失败、重复或补采。`ds_sweep.complete=true`；
它独立于历史`formal100`，本包`formal100_complete=false`表示不含旧矩阵，不是本轮缺样。
新增10140个标量行，实板总计27544行；失效仿真980行保持独立。

[四档统计](ds-sweep-summary.md)及[逐试次与配对JSON](ds-sweep-summary.json)复用旧DS0/DS50共100次，
形成4×5×10共200个试次。硬件时间与吞吐逐试次计算，中位数、Q1、Q3及IQR宽度保留；
曲线按同DS、同block的`T_V(Ak)/T_V(A1)-1`取十配对的中位数与Q1—Q3。
DS50保留两个实际boot；所有配对均在各自同一boot内，不代表十次独立重启。
图2新增DS25/DS100完整n=10中位数，并映射四档曲线的160个独立硬件cycles操作数行。
作图入口与依赖见[analysis](../analysis/README.md)，[执行/时序/结束回执](../board/ds-sweep.md)另列。
