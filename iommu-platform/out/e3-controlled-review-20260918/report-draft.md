# E3 v6 受控实验（00087，2026-09-18）事实结果复核草案

> 分工：本文件只整理事实与证据边界；科学解释与实验决策由 Codex 完成。脚本为本提案的一部分，尚未运行。原始终端日志未被改写（仅提供把唯一 NUL 换成 [NUL] 的可读副本）。

## 0. 输入与复核方法

- 结果目录：`out/mac-e3-controlled-20260918-164635-run-rk48bhte/`；原始配对 JSON：`all-original-pairs.json:1`（11 条 `idma.e3_controlled.v1`）；UART：`out/e3-controlled-review-20260918/console.readable.log`（原始 raw 仅 1 个 NUL）；次要证据：`E3_CONTROLLED_RESULTS.md:24-69`、`run.json:22-27`、`host-validation.json:8-12`。
- 本次未执行任何命令；下表数值为人工/交叉核对结果，需由 Codex 运行 `review_script` 独立重算后再引用。
- 字段口径：A 剩余服务 `e3[41]−e3[4]`；V 剩余等待 `e3[70]−e3[4]`；V 完整排队 `e3[70]−e3[69]`；V 自身翻译 `e3[73]−e3[71]`（`e3_controlled_fields.py:189-193`、`E3_CONTROLLED.md:296-302`）。

## 1. 执行层事实（execution）

- 11 对、22 轮：每对 `success=true`、`status=0`、`errno=0`、`rounds_attempted/completed=2/2`；每轮 `status=0`、`hpm_configured/measurement_started/measurement_complete=1`、`prep_complete=1`、`stage=8`、`flush_status=0`、`observer_magic=0x49444D414F425306`、`e3_words[0]=0x49444D4145330002`、`e3_words[1]=0`；V/A 各 `1 submitted/1 completed/64 B`，`data_errors=0`、`quarantined=0`、`s2_ptw=0`；V prime 恒为 1，hot 轮 A prime=1、cold 轮 A prime 全 0；probe 恒为 1/64 B 且 `iotlb_miss=s1_ptw=s2_ptw=0`；launcher magic/FROZEN/issued=done=expected/cancel=0、ID 与计数自洽、`goV=Aselect+1`、各时间戳不超 `ticks+1`；`taint=0`（begin/exit/final）、`final_exit=0`、`collection_failed=0`。
- UART：22 个 sequence × 19 行（3 batch + 16 path）= 418 行唯一观测；短阶段 114、主阶段 304（重复 dump 只计一次）；`console.readable.log` 的行区间见证据表。JSON↔UART 一致示例：seq7 `cycles=551=g3[?]`（`console.readable.log:917` 与 `rounds[0].controlled.v_hw_cycles=551`）、seq14 `546`（`:1050`）、seq19 `124`（`:1145`）、seq1 solo `v_only=98,neither=1`（`:481`，状态和=99=cycles）。
- 22 轮执行一览（`trigger`/`v_hw`/读数）：

| 阶段 | 用例 | seq | D | trigger | V 硬件周期 | A 剩余 | V 剩余 | V 排队 | V 自身 |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| short | 1 solo | 1 | 0 | 0 | 99 | – | – | – | – |
| short | 1 solo | 2 | 128 | 0 | 99 | – | – | – | – |
| short | 2 hot | 3 | 0 | 0 | 115 | – | – | – | – |
| short | 2 hot | 4 | 128 | 0 | 115 | – | – | – | – |
| short | 3 cold zero | 5 | 0 | 1 | 545 | 273 | 282 | 416 | 4 |
| short | 3 cold zero | 6 | 0 | 1 | 545 | 271 | 280 | 413 | 4 |
| main | 1 cold zero | 7 | 0 | 1 | 551 | 274 | 283 | 417 | 4 |
| main | 1 cold zero | 8 | 0 | 1 | 543 | 272 | 281 | 412 | 4 |
| main | 2 cold 互补 | 9 | 0 | 1 | 542 | 272 | 281 | 412 | 4 |
| main | 2 cold 互补 | 10 | 128 | 1 | 672 | 399 | 408 | 541 | 4 |
| main | 3 hot | 11 | 0 | 0 | 114 | – | – | – | – |
| main | 3 hot | 12 | 128 | 0 | 116 | – | – | – | – |
| main | 4 cold 互补 | 13 | 128 | 1 | 680 | 400 | 409 | 551 | 4 |
| main | 4 cold 互补 | 14 | 0 | 1 | 546 | 271 | 280 | 415 | 4 |
| main | 5 cold zero | 15 | 0 | 1 | 542 | 271 | 280 | 411 | 4 |
| main | 5 cold zero | 16 | 0 | 1 | 543 | 271 | 280 | 411 | 4 |
| main | 6 cold 互补 | 17 | 0 | 1 | 543 | 272 | 281 | 412 | 4 |
| main | 6 cold 互补 | 18 | 128 | 1 | 672 | 401 | 410 | 541 | 4 |
| main | 7 hot | 19 | 128 | 0 | 124 | – | – | – | – |
| main | 7 hot | 20 | 0 | 0 | 117 | – | – | – | – |
| main | 8 cold 互补 | 21 | 128 | 1 | 672 | 401 | 410 | 541 | 4 |
| main | 8 cold 互补 | 22 | 0 | 1 | 546 | 271 | 280 | 415 | 4 |

- 需先排查的采集异常（不构成执行失败，也暂不参与解释）：seq3 `victim_elapsed_ns=1095040`、seq19 `victim_elapsed_ns=1085200` 且 `done_to_read=52395`（`:1145`）、seq10 `snapshot_after_ns=1016640`、seq14 `snapshot_before_ns=1027280` 且 `e3[16]=54229`、launcher `ticks=60273`。

## 2. 因果可比性（causal eligibility，仅列事实与条件）

- 触发：cold 正式 12/12 轮触发；solo（seq1/2）与 hot（seq3/4/11/12/19/20）全部 `trigger=0`。hot 轮 A prime 为热命中，不产生“A 一级翻译首次缺失”，因此 hot 的 D128 不会注入（各轮 `e3[7]=0`）。
- 触发轮身份/前缀（`e3_words` 原始索引）：A 记录 `e3[32..45]` = seq1 / ID4 / DID11 / 读（write=0）/ IOVA 274875809792 / event=127 / 首次查询 miss（`e3[44]=0`）；V 记录 `e3[64..77]` = seq1 / ID0 / DID10 / 读 / 同一 IOVA / event=127 / 首次 hit（`e3[76]=1`）；S1 层级 `e3[8]=0`；PTE PA `e3[9]=2193147896`；插入 `e3[12]=1`、`e3[62]=0`、`e3[63]=1`，插入记录 `e3[46..53]` = seq1/ID0/DID10/IOVA 274875813888/write=1/hit=1。
- A 里程碑：所有触发轮 A/V 事件位均 127，A 的 comp/return 都在 V 窗口内；本次没有“A 返回落在窗口外”的轮次，脚本对缺席情形仍按 absent 保留（不做零值填充）。
- 触发前时间（`e3[4]−e3[69]` / `e3[4]−e3[39]`）：主对1 134/131、138/135；对2 131/133、135/137；对4 142/135、146/139；对5 131/131、135/135；对6 131/131、135/135；对8 131/135、135/139。只有对5、对6 两轮完全一致；记录字段相同不等于隐藏状态与到达时间相同（`E3_CONTROLLED_RESULTS.md:67` 同口径）。

## 3. 结构化读数与差值（不作因果结论）

D128−D0（互补 cold，按“D128 轮 − D0 轮”）：

| 主对 | ΔV 硬件周期 | ΔA 剩余 | ΔV 剩余 | ΔV 排队 | ΔV 自身 |
|---|---:|---:|---:|---:|---:|
| 2（seq9→10） | +130 | +127 | +127 | +129 | 0 |
| 4（seq14←13） | +134 | +129 | +129 | +136 | 0 |
| 6（seq17→18） | +129 | +129 | +129 | +129 | 0 |
| 8（seq22←21） | +126 | +130 | +130 | +126 | 0 |

D0/D0 基线（后一轮 − 前一轮）：

| 对 | ΔV 硬件周期 | ΔA 剩余 | ΔV 剩余 | ΔV 排队 | ΔV 自身 |
|---|---:|---:|---:|---:|---:|
| short 对3（seq5→6） | 0 | −2 | −2 | −3 | 0 |
| 主对1（seq7→8） | −8 | −2 | −2 | −5 | 0 |
| 主对5（seq15→16） | +1 | 0 | 0 | 0 | 0 |

无触发对（solo 对1、hot 对2/主对3/主对7）的 D128−D0 只有 V 硬件周期差（0、0、+2、+7），且 `e3[7]=0`，即未实际注入延迟；不能读作“注入了 128 周期但没有效果”。

## 4. 与既有计划/假设的关系

- 仍然有效：v5 的“整批轮转导致跨轮身份/前史不可比”在 v6 的记录字段层面被消除（同一映射生命周期、每轮 checked S1 失效、V prime/probe、固定 64 B、硬件在 A 被选中后放 V：`E3_CONTROLLED.md:38-53`）。本次 6 个 cold 正式对的目标身份、IOVA、S1 层级/PTE、插入记录在跨轮一致（`E3_CONTROLLED_RESULTS.md:65`；字段索引见上）。
- 需要更新：① “418 条唯一观测”不再是未测预期（`E3_CONTROLLED.md:275`），本次实测成立（`run.json:27`、`host-validation.json:8`，脚本重算应等于 22×19）。② hot 对不能充当“施加 128 周期”的阴性对照（它们根本不触发）。③ 4 个互补对中 3 对触发前时间不同，把这些完整排队差值全部归因于 D128 会越界；`E3_CONTROLLED_RESULTS.md:67-69` 已作此保留。④ D128−D0 增量为 +126…+136，不严格等于 128，中间请求（每轮 1 条 V 写）与前置时间差都可能参与（`E3_QUEUE_PAIR.md:107` 的同口径提醒）。
- 与计划第 3 步（`docs/next-mechanism-plan.md:54-66`）的关系：本次提供了“固定 V 请求 + 驻留翻译 + 只改 A PTW 响应”所需的受控触发与读数结构；但“只改这一笔响应”的排他性仍受前置时间差、同窗口中间请求以及 hot 无对照的限制。

## 5. 证据支持的下一步选项（供 Codex 选择，均不自动执行）

- A. 先跑 `review_script` 独立重算：418/22、22 轮读数、4 组 D128−D0、3 组 D0/D0、身份/PTE/前缀/插入/前置时间比较、launcher 与 UART 逐字段比对，并核对 `E3_CONTROLLED_RESULTS.md:24-69` 是否有数值出入。
- B. 若继续同流程采样（需 Codex 决定，且会改变有限矩阵规模）：把“两轮 `e3[4]−e3[69]`、`e3[4]−e3[39]` 必须相等”写成先验筛选条件，重复同一批互补 cold 对以获得可比子集；对不可比的轮次按 `E3_CONTROLLED.md:305` 保留并停止解释。
- C. 若要检验“只改一笔 A PTW 响应”的排他性，需要一个已确认可实现的新对照（例如让触发轮中间请求数为 0，或强制两轮中间请求身份/时序一致）：属 RTL/驱动改动，超出本次范围。
- D. 先定性第 1 节列出的 4 个采集异常（seq3/10/14/19）是否为宿主读取路径伪影，再决定这几轮能否进入差值表。
- E. 若要分离“A 服务被延长”与“V 正在等待”两个条件，可考虑新增“A cold 但 V 不在等待”的对照轮（新实验设计，需先写实验卡）。
- F. 论文侧：在当前证据下只能写“延迟增量集中在 A 剩余服务与 V 选中前等待、V 自身翻译不变、阴性/基线波动可辨”的机制表征候选；方法评估与吞吐结论按 `docs/paper-logic.md:134-143` 的分支继续等待机制证据。

## 6. 未完成/不可用检查

- BIT/payload SHA256 未在主机实测、SD 未回读、smoke/HPM 校准为空、上传回执未见（`run.json:7-9/19/21/22/39`）。
- 时序收敛、CDC、外部 IO 约束未审查（`E3_CONTROLLED.md:77`）。
- 统计：N=4 互补对，不做显著性声明；触发前时间不等价性未消除。
- 原始日志与可读副本的替换关系需 `--readable-console` 验证；若原始 raw 不可得，JSON↔UART 复核必须报 unavailable。