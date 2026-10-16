# N 非阻塞工程 pilot 离线汇总（RESULTS.md）

- 生成时间：2026-10-04T07:14:13Z
- 分析脚本：`scripts/analyze_nonblock_pilot.py`（离线汇总工具；可用性见本报告验收项）
- 汇总 JSON：`nonblock-pilot-analysis.json`
- 数据状态：**完整**（脚本退出码 = 0）
- 时钟换算假设：50 MHz（1 cycle = 20.0 ns，仅用于时间/吞吐推导）
- 真实 V 固定工作量：short = 262144 B，long = 16777216 B（= transfers × 4096）
- 诊断 bank：已读取（`--diagnostics`）

> 本文件由脚本按固定模板生成，是描述性汇总，不是实验结论声明；
> 不含显著性统计，不含 novelty 判断，不对未运行/失败项做推测或补值，
> 不修改任何原始文件。fast 事件数不是命中率，也不是全部 miss 数。

## 1. 数据来源与运行身份

| label | 目录 | 存在 | run_id | BIT | device | summary.status | STOPPED |
|---|---|---|---|---|---|---|---|
| original-serial | `out/n-prototype-20261004/nonblock-pilot-20261004-original-03` | 是 | a72442e2e112f401 | `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/nonblock-pilot-20261004/original-serial.bit` | /dev/idma0 | complete | 无 |
| banked-serial | `out/n-prototype-20261004/nonblock-pilot-20261004-serial-v5-01` | 是 | c76ccc3fac1ff68a | `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/nonblock-pilot-20261004/banked-serial-v5.bit` | /dev/idma0 | complete | 无 |
| banked-hit | `out/n-prototype-20261004/nonblock-pilot-20261004-hit-v5-01` | 是 | e7497b8802cb9e58 | `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/nonblock-pilot-20261004/banked-hit-v5.bit` | /dev/idma0 | complete | 无 |

> 来源约束：original-serial 来源受限于旧 legacy3，而 S/N 为 512 缓冲；S/原版差异同时包含结构与缓冲存储差异，不得作因果外推。

## 2. 完整度矩阵（每 label 冻结 15 例，三 label 共 45 例）

| label | 完成 | 失败 | 不完整 | 未运行 | 合计 |
|---|---|---|---|---|---|
| original-serial | 15 | 0 | 0 | 0 | 15 |
| banked-serial | 15 | 0 | 0 | 0 | 15 |
| banked-hit | 15 | 0 | 0 | 0 | 15 |

| order | case | group | condition | round | state | cycles | A jobs |
|---|---|---|---|---|---|---|---|
| 1 | original-serial / short-solo | short | Solo | — | complete | 44146 | 0 |
| 2 | original-serial / short-a1 | short | A1 | — | complete | 83470 | 64 |
| 3 | original-serial / short-a8 | short | A8 | — | complete | 113640 | 64 |
| 4 | original-serial / long-r1-solo | long | Solo | 1 | complete | 2828343 | 0 |
| 5 | original-serial / long-r1-a1 | long | A1 | 1 | complete | 5359179 | 4096 |
| 6 | original-serial / long-r1-a8 | long | A8 | 1 | complete | 7349054 | 4096 |
| 7 | original-serial / long-r1-a16 | long | A16 | 1 | complete | 7945520 | 4096 |
| 8 | original-serial / long-r2-a16 | long | A16 | 2 | complete | 7946297 | 4096 |
| 9 | original-serial / long-r2-a8 | long | A8 | 2 | complete | 7347789 | 4096 |
| 10 | original-serial / long-r2-a1 | long | A1 | 2 | complete | 5360287 | 4096 |
| 11 | original-serial / long-r2-solo | long | Solo | 2 | complete | 2828763 | 0 |
| 12 | original-serial / long-r3-solo | long | Solo | 3 | complete | 2832793 | 0 |
| 13 | original-serial / long-r3-a1 | long | A1 | 3 | complete | 5358650 | 4096 |
| 14 | original-serial / long-r3-a8 | long | A8 | 3 | complete | 7353256 | 4096 |
| 15 | original-serial / long-r3-a16 | long | A16 | 3 | complete | 7942861 | 4096 |
| 1 | banked-serial / short-solo | short | Solo | — | complete | 44053 | 0 |
| 2 | banked-serial / short-a1 | short | A1 | — | complete | 86709 | 64 |
| 3 | banked-serial / short-a8 | short | A8 | — | complete | 140432 | 64 |
| 4 | banked-serial / long-r1-solo | long | Solo | 1 | complete | 2826957 | 0 |
| 5 | banked-serial / long-r1-a1 | long | A1 | 1 | complete | 5495261 | 4096 |
| 6 | banked-serial / long-r1-a8 | long | A8 | 1 | complete | 8951217 | 4096 |
| 7 | banked-serial / long-r1-a16 | long | A16 | 1 | complete | 9929407 | 4096 |
| 8 | banked-serial / long-r2-a16 | long | A16 | 2 | complete | 9924601 | 4096 |
| 9 | banked-serial / long-r2-a8 | long | A8 | 2 | complete | 8954802 | 4096 |
| 10 | banked-serial / long-r2-a1 | long | A1 | 2 | complete | 5499765 | 4096 |
| 11 | banked-serial / long-r2-solo | long | Solo | 2 | complete | 2825050 | 0 |
| 12 | banked-serial / long-r3-solo | long | Solo | 3 | complete | 2826503 | 0 |
| 13 | banked-serial / long-r3-a1 | long | A1 | 3 | complete | 5501111 | 4096 |
| 14 | banked-serial / long-r3-a8 | long | A8 | 3 | complete | 8959431 | 4096 |
| 15 | banked-serial / long-r3-a16 | long | A16 | 3 | complete | 9916294 | 4096 |
| 1 | banked-hit / short-solo | short | Solo | — | complete | 44054 | 0 |
| 2 | banked-hit / short-a1 | short | A1 | — | complete | 86662 | 64 |
| 3 | banked-hit / short-a8 | short | A8 | — | complete | 135203 | 64 |
| 4 | banked-hit / long-r1-solo | long | Solo | 1 | complete | 2826372 | 0 |
| 5 | banked-hit / long-r1-a1 | long | A1 | 1 | complete | 5499331 | 4096 |
| 6 | banked-hit / long-r1-a8 | long | A8 | 1 | complete | 7672922 | 4096 |
| 7 | banked-hit / long-r1-a16 | long | A16 | 1 | complete | 10198914 | 4096 |
| 8 | banked-hit / long-r2-a16 | long | A16 | 2 | complete | 10214171 | 4096 |
| 9 | banked-hit / long-r2-a8 | long | A8 | 2 | complete | 8603325 | 4096 |
| 10 | banked-hit / long-r2-a1 | long | A1 | 2 | complete | 5495410 | 4096 |
| 11 | banked-hit / long-r2-solo | long | Solo | 2 | complete | 2823084 | 0 |
| 12 | banked-hit / long-r3-solo | long | Solo | 3 | complete | 2823168 | 0 |
| 13 | banked-hit / long-r3-a1 | long | A1 | 3 | complete | 5498262 | 4096 |
| 14 | banked-hit / long-r3-a8 | long | A8 | 3 | complete | 8597740 | 4096 |
| 15 | banked-hit / long-r3-a16 | long | A16 | 3 | complete | 10195105 | 4096 |

## 3. 质量问题清单（不剔除、不修补、不重跑）

无。

> fast 计数全 0 **不是**质量问题：它只表示本轮未观测到真实 accept。

### 3.2 runner 记录的 case problems（原样保留）

无。

## 4. 短组验收（3 例，只做验收，不与长组混合）

> 短组只做验收（3 例），不与长组混合，不参与两段对照比较，不做显著性。A 吞吐为同一窗口内的 A 完成字节速率，不是总线吞吐。

| label | case | condition | state | qualified | cycles | ns | V MiB/s | A jobs | A B | A MiB/s | A 字段来源 | raw_window 一致 | helper 一致 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | short-a1 | A1 | complete | 是 | 83470 | 1669400.0 | 149.75 | 64 | 262144 | 149.75 | record.p4m.raw_window[3] | 是 | 不适用 |
| original-serial | short-a8 | A8 | complete | 是 | 113640 | 2272800.0 | 110.00 | 64 | 262144 | 110.00 | record.p4m.raw_window[3] | 是 | 不适用 |
| original-serial | short-solo | Solo | complete | 是 | 44146 | 882920.0 | 283.15 | 0 | 0 | 0.00 | record.p4m.raw_window[3] | 是 | 不适用 |
| banked-serial | short-a1 | A1 | complete | 是 | 86709 | 1734180.0 | 144.16 | 64 | 262144 | 144.16 | record.p4m.raw_window[3] | 是 | 是 |
| banked-serial | short-a8 | A8 | complete | 是 | 140432 | 2808640.0 | 89.01 | 64 | 262144 | 89.01 | record.p4m.raw_window[3] | 是 | 是 |
| banked-serial | short-solo | Solo | complete | 是 | 44053 | 881060.0 | 283.75 | 0 | 0 | 0.00 | record.p4m.raw_window[3] | 是 | 是 |
| banked-hit | short-a1 | A1 | complete | 是 | 86662 | 1733240.0 | 144.24 | 64 | 262144 | 144.24 | record.p4m.raw_window[3] | 是 | 是 |
| banked-hit | short-a8 | A8 | complete | 是 | 135203 | 2704060.0 | 92.45 | 64 | 262144 | 92.45 | record.p4m.raw_window[3] | 是 | 是 |
| banked-hit | short-solo | Solo | complete | 是 | 44054 | 881080.0 | 283.74 | 0 | 0 | 0.00 | record.p4m.raw_window[3] | 是 | 是 |

## 5. 长组：每条件每次值 + 中位/min/max

> long 组每条件给出三轮每次原值 + 中位/min/max（描述性，不做显著性）。第 2 轮顺序为反向，请按轮次看。仅当同一条件三个 label 各自三轮全部技术合格时才发布 median/min/max；否则只保留逐轮原值并标为仅诊断。V 吞吐与同窗口 A 吞吐分列，不用不同窗口的 A 完成数比较 A 速度。fast 全 0 与 activation 边界并列见第 7 节（fast 全 0 不是技术失败）。

### 5.1 条件 Solo

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 2828343 | 2828763 | 2832793 | 2828763 | 2828343 | 2832793 | 3 |
| banked-serial | 是 | 2826957 | 2825050 | 2826503 | 2826503 | 2825050 | 2826957 | 3 |
| banked-hit | 是 | 2826372 | 2823084 | 2823168 | 2823168 | 2823084 | 2826372 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 282.851 | 282.809 | 282.407 | 282.809 | 282.407 | 282.851 | 3 |
| banked-serial | 是 | 282.99 | 283.181 | 283.035 | 283.035 | 282.99 | 283.181 | 3 |
| banked-hit | 是 | 283.048 | 283.378 | 283.37 | 283.37 | 283.048 | 283.378 | 3 |

**a_jobs** — 同窗口 A 完成数 raw_window[3]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| banked-serial | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| banked-hit | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |

**a_bytes** — 同窗口 A 字节（w3×4096）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| banked-serial | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| banked-hit | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |

**a_mib_s** — 同窗口 A 吞吐 MiB/s

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| banked-serial | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |
| banked-hit | 是 | 0 | 0 | 0 | 0 | 0 | 0 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

### 5.2 条件 A1

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 5359179 | 5360287 | 5358650 | 5359179 | 5358650 | 5360287 | 3 |
| banked-serial | 是 | 5495261 | 5499765 | 5501111 | 5499765 | 5495261 | 5501111 | 3 |
| banked-hit | 是 | 5499331 | 5495410 | 5498262 | 5498262 | 5495410 | 5499331 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 149.277 | 149.246 | 149.291 | 149.277 | 149.246 | 149.291 | 3 |
| banked-serial | 是 | 145.58 | 145.461 | 145.425 | 145.461 | 145.425 | 145.58 | 3 |
| banked-hit | 是 | 145.472 | 145.576 | 145.501 | 145.501 | 145.472 | 145.576 | 3 |

**a_jobs** — 同窗口 A 完成数 raw_window[3]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |
| banked-serial | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |
| banked-hit | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |

**a_bytes** — 同窗口 A 字节（w3×4096）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |
| banked-serial | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |
| banked-hit | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |

**a_mib_s** — 同窗口 A 吞吐 MiB/s

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 149.277 | 149.246 | 149.291 | 149.277 | 149.246 | 149.291 | 3 |
| banked-serial | 是 | 145.58 | 145.461 | 145.425 | 145.461 | 145.425 | 145.58 | 3 |
| banked-hit | 是 | 145.472 | 145.576 | 145.501 | 145.501 | 145.472 | 145.576 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

### 5.3 条件 A8

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 7349054 | 7347789 | 7353256 | 7349054 | 7347789 | 7353256 | 3 |
| banked-serial | 是 | 8951217 | 8954802 | 8959431 | 8954802 | 8951217 | 8959431 | 3 |
| banked-hit | 是 | 7672922 | 8603325 | 8597740 | 8597740 | 7672922 | 8603325 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 108.858 | 108.876 | 108.795 | 108.858 | 108.795 | 108.876 | 3 |
| banked-serial | 是 | 89.373 | 89.338 | 89.291 | 89.338 | 89.291 | 89.373 | 3 |
| banked-hit | 是 | 104.263 | 92.987 | 93.048 | 93.048 | 92.987 | 104.263 | 3 |

**a_jobs** — 同窗口 A 完成数 raw_window[3]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |
| banked-serial | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |
| banked-hit | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |

**a_bytes** — 同窗口 A 字节（w3×4096）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |
| banked-serial | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |
| banked-hit | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |

**a_mib_s** — 同窗口 A 吞吐 MiB/s

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 108.858 | 108.876 | 108.795 | 108.858 | 108.795 | 108.876 | 3 |
| banked-serial | 是 | 89.373 | 89.338 | 89.291 | 89.338 | 89.291 | 89.373 | 3 |
| banked-hit | 是 | 104.263 | 92.987 | 93.048 | 93.048 | 92.987 | 104.263 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

### 5.4 条件 A16

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 7945520 | 7946297 | 7942861 | 7945520 | 7942861 | 7946297 | 3 |
| banked-serial | 是 | 9929407 | 9924601 | 9916294 | 9924601 | 9916294 | 9929407 | 3 |
| banked-hit | 是 | 10198914 | 10214171 | 10195105 | 10198914 | 10195105 | 10214171 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 100.686 | 100.676 | 100.719 | 100.686 | 100.676 | 100.719 | 3 |
| banked-serial | 是 | 80.569 | 80.608 | 80.675 | 80.608 | 80.569 | 80.675 | 3 |
| banked-hit | 是 | 78.44 | 78.323 | 78.469 | 78.44 | 78.323 | 78.469 | 3 |

**a_jobs** — 同窗口 A 完成数 raw_window[3]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |
| banked-serial | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |
| banked-hit | 是 | 4096 | 4096 | 4096 | 4096 | 4096 | 4096 | 3 |

**a_bytes** — 同窗口 A 字节（w3×4096）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |
| banked-serial | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |
| banked-hit | 是 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 16777216 | 3 |

**a_mib_s** — 同窗口 A 吞吐 MiB/s

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 100.686 | 100.676 | 100.719 | 100.686 | 100.676 | 100.719 | 3 |
| banked-serial | 是 | 80.569 | 80.608 | 80.675 | 80.608 | 80.569 | 80.675 | 3 |
| banked-hit | 是 | 78.44 | 78.323 | 78.469 | 78.44 | 78.323 | 78.469 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

## 6. 两段对照（仅 long 组，弱因果表述）

- `S/原版结构+缓冲变化`：`banked-serial − original-serial（结构+缓冲差异；来源受限于旧 legacy3 vs S/N 512 缓冲，不是纯布局因果）：original -> bankedSerial`
- `N/S变化`：`banked-hit − banked-serial（同布局不同 enable 的描述性变化；本轮 fast 全 0 时不构成绕过证据）：bankedSerial -> bankedHit`
- **不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；N/S变化必须以 banked-serial 为中介，且在本轮 fast 全 0 时只能作描述性窗口周期差保留。**
- V 吞吐与同窗口 A 吞吐均以 raw_window[1] 为窗口、50 MHz 换算（推导量）；A 速度只用同窗口 A 吞吐评价，不用不同窗口 A 完成数直接比较。跨 boot / 布局+缓冲差异不得作因果外推。
- 发布规则：同一比较条件在 original-serial/banked-serial/banked-hit 三个 label 各自三轮（共 3×3 次）技术上全部合格时才发布；否则只列 reasons，不发布数值。
- 仅描述性中位/min/max，不做显著性检验。

### 6.1 V 吞吐（MiB/s）收益 / 代价

| condition | published | orig 中位 | serial 中位 | hit 中位 | S/原版结构+缓冲变化 Δ | S/原版结构+缓冲变化 Δ% | N/S变化 Δ | N/S变化 Δ% |
|---|---|---|---|---|---|---|---|---|
| Solo | 是 | 282.809 | 283.035 | 283.37 | 0.226 | +0.1% | 0.334 | +0.1% |
| A1 | 是 | 149.277 | 145.461 | 145.501 | -3.816 | -2.6% | 0.04 | +0.0% |
| A8 | 是 | 108.858 | 89.338 | 93.048 | -19.52 | -17.9% | 3.71 | +4.2% |
| A16 | 是 | 100.686 | 80.608 | 78.44 | -20.078 | -19.9% | -2.168 | -2.7% |

- 条件 Solo：all_fast_zero = 是；any_fast_nonzero = 否
- 条件 A1：all_fast_zero = 是；any_fast_nonzero = 否
- 条件 A8：all_fast_zero = 否；any_fast_nonzero = 是
- 条件 A16：all_fast_zero = 否；any_fast_nonzero = 是

### 6.2 同窗口 A 吞吐（MiB/s）收益 / 代价

| condition | published | orig 中位 | serial 中位 | hit 中位 | S/原版结构+缓冲变化 Δ | S/原版结构+缓冲变化 Δ% | N/S变化 Δ | N/S变化 Δ% |
|---|---|---|---|---|---|---|---|---|
| Solo | 是 | 0 | 0 | 0 | 0 | — | 0 | — |
| A1 | 是 | 149.277 | 145.461 | 145.501 | -3.816 | -2.6% | 0.04 | +0.0% |
| A8 | 是 | 108.858 | 89.338 | 93.048 | -19.52 | -17.9% | 3.71 | +4.2% |
| A16 | 是 | 100.686 | 80.608 | 78.44 | -20.078 | -19.9% | -2.168 | -2.7% |

- 条件 Solo：all_fast_zero = 是；any_fast_nonzero = 否
- 条件 A1：all_fast_zero = 是；any_fast_nonzero = 否
- 条件 A8：all_fast_zero = 否；any_fast_nonzero = 是
- 条件 A16：all_fast_zero = 否；any_fast_nonzero = 是

## 7. 诊断 bank（可选 `--diagnostics`；只读，不反馈 DMA/AXI）

> fast V/A/other 计数来自 core words 6..10（V=w6+w7, A=w8+w9, other=w10）；seen 为 sticky 16 位掩码；阶段计数来自第二 bank words 7..14。fast 事件数不是命中率，也不是全部 miss 数；fast 全 0 = 科学上未行使，不是技术失败。

- seen bit0 `base_s1_ptw_active`：宽口径 base S1 PTW 活动；可能含 implicit/debug，不等于 fast 候选
- seen bit1 `full_fast_window`：完整 fast 窗口谓词
- seen bit2 `candidate_in_window`：窗口内候选有效
- seen bit3 `probe_start`：probe start
- seen bit4 `dc_eval`：DC 评估
- seen bit5 `dc_good`：DC 通过稳定阶段
- seen bit6 `leaf_eval`：LEAF 评估
- seen bit7 `leaf_good`：LEAF 合格
- seen bit8 `dc_msi_reject`：DC MSI 拒绝（DDTC 命中）
- seen bit9 `leaf_msi_reject`：LEAF MSI 拒绝（IOTLB 命中）
- seen bit10 `leaf_lookup_miss`：LEAF 查找未命中
- seen bit11 `fast_accept`：fast accept
- seen bit12 `dc_lookup_miss`：DC 查找未命中
- seen bit13 `dc_other_reject`：DC 其他拒绝
- seen bit14 `leaf_other_reject`：LEAF 其他拒绝
- seen bit15 `runtime_enable`：运行时 enable

解释许可与禁止（固定）

- seen bit0 是宽口径 base S1 PTW 活动，可能包含 implicit/debug 触发；bit1 才是完整 fast 窗口谓词，bit2 才是窗口内候选有效。
- 未出现 bit2（candidate in window）或 probe start = 0 时，只能说明本轮未观测到合格窗口候选，不能说明没有 cache 命中。
- DC good 计数为 0 且没有显式 MSI reject 时，不能唯一归因 MSI；只有显式 reject 计数才支持该归因。
- fast 计数 > 0 表示本轮真实 accept，不是命中率，也不是 novelty；fast 全 0 只表示机制未被触发（科学上未行使），不是技术失败。
- 诊断采样跨窗口时 start/eval/leaf 计数之间无需守恒，不假设生命周期完整。
- 旧 HPM / miss / PTW 字段在 banked 结构下不是有效 metric，绝不作为机制证据。
- S 来源受限于旧 legacy3 vs S/N 512 缓冲差异：跨 boot 与布局+缓冲差异不得作因果外推。

### 7.1 逐 case fast 计数（来自 core words 6..10）

| label | case | rnd | fast V | fast A | fast other | fast Σ | all0 |
|---|---|---|---|---|---|---|---|
| original-serial | short-solo | — | — | — | — | — | — |
| original-serial | short-a1 | — | — | — | — | — | — |
| original-serial | short-a8 | — | — | — | — | — | — |
| original-serial | long-r1-solo | 1 | — | — | — | — | — |
| original-serial | long-r1-a1 | 1 | — | — | — | — | — |
| original-serial | long-r1-a8 | 1 | — | — | — | — | — |
| original-serial | long-r1-a16 | 1 | — | — | — | — | — |
| original-serial | long-r2-a16 | 2 | — | — | — | — | — |
| original-serial | long-r2-a8 | 2 | — | — | — | — | — |
| original-serial | long-r2-a1 | 2 | — | — | — | — | — |
| original-serial | long-r2-solo | 2 | — | — | — | — | — |
| original-serial | long-r3-solo | 3 | — | — | — | — | — |
| original-serial | long-r3-a1 | 3 | — | — | — | — | — |
| original-serial | long-r3-a8 | 3 | — | — | — | — | — |
| original-serial | long-r3-a16 | 3 | — | — | — | — | — |
| banked-serial | short-solo | — | 0 | 0 | 0 | 0 | 是 |
| banked-serial | short-a1 | — | 0 | 0 | 0 | 0 | 是 |
| banked-serial | short-a8 | — | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r1-solo | 1 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r1-a1 | 1 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r1-a8 | 1 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r1-a16 | 1 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r2-a16 | 2 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r2-a8 | 2 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r2-a1 | 2 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r2-solo | 2 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r3-solo | 3 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r3-a1 | 3 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r3-a8 | 3 | 0 | 0 | 0 | 0 | 是 |
| banked-serial | long-r3-a16 | 3 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | short-solo | — | 0 | 0 | 0 | 0 | 是 |
| banked-hit | short-a1 | — | 0 | 0 | 0 | 0 | 是 |
| banked-hit | short-a8 | — | 96 | 2 | 0 | 98 | 否 |
| banked-hit | long-r1-solo | 1 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | long-r1-a1 | 1 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | long-r1-a8 | 1 | 6142 | 2 | 0 | 6144 | 否 |
| banked-hit | long-r1-a16 | 1 | 8193 | 0 | 0 | 8193 | 否 |
| banked-hit | long-r2-a16 | 2 | 8193 | 0 | 0 | 8193 | 否 |
| banked-hit | long-r2-a8 | 2 | 6144 | 2 | 0 | 6146 | 否 |
| banked-hit | long-r2-a1 | 2 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | long-r2-solo | 2 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | long-r3-solo | 3 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | long-r3-a1 | 3 | 0 | 0 | 0 | 0 | 是 |
| banked-hit | long-r3-a8 | 3 | 6144 | 2 | 0 | 6146 | 否 |
| banked-hit | long-r3-a16 | 3 | 8193 | 0 | 0 | 8193 | 否 |

### 7.2 逐 case 诊断 seen 位与阶段计数（第二 bank words 6..15）

| label | case | seen | 阶段计数（probe/dc_eval/dc_good/leaf_eval/leaf_good/dc_msi_rej/leaf_msi_rej/leaf_miss/fast_acc） | diag_ok |
|---|---|---|---|---|
| original-serial | short-solo | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | short-a1 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | short-a8 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r1-solo | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r1-a1 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r1-a8 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r1-a16 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r2-a16 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r2-a8 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r2-a1 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r2-solo | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r3-solo | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r3-a1 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r3-a8 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| original-serial | long-r3-a16 | — | —/—/—/—/—/—/—/—/— | 不适用 |
| banked-serial | short-solo | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | short-a1 | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | short-a8 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r1-solo | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r1-a1 | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r1-a8 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r1-a16 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r2-a16 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r2-a8 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r2-a1 | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r2-solo | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r3-solo | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r3-a1 | 0x0000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r3-a8 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-serial | long-r3-a16 | 0x0007 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | short-solo | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | short-a1 | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | short-a8 | 0x88FF | 98/98/98/98/98/0/0/0/98 | 是 |
| banked-hit | long-r1-solo | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | long-r1-a1 | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | long-r1-a8 | 0x88FF | 6144/6144/6144/6144/6144/0/0/0/6144 | 是 |
| banked-hit | long-r1-a16 | 0x8CFF | 8194/8194/8194/8194/8193/0/0/1/8193 | 是 |
| banked-hit | long-r2-a16 | 0x8CFF | 8194/8194/8194/8194/8193/0/0/1/8193 | 是 |
| banked-hit | long-r2-a8 | 0x88FF | 6146/6146/6146/6146/6146/0/0/0/6146 | 是 |
| banked-hit | long-r2-a1 | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | long-r2-solo | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | long-r3-solo | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | long-r3-a1 | 0x8000 | 0/0/0/0/0/0/0/0/0 | 是 |
| banked-hit | long-r3-a8 | 0x88FF | 6146/6146/6146/6146/6146/0/0/0/6146 | 是 |
| banked-hit | long-r3-a16 | 0x8CFF | 8194/8194/8194/8194/8193/0/0/1/8193 | 是 |

### 7.3 长组每条件 fast/activation 汇总（Solo/A1/A8/A16，n=3）

#### 条件 Solo（all_fast_zero = 是）

| label | fast V r1/r2/r3 | fast A r1/r2/r3 | fast Σ r1/r2/r3 | fast_acc r1/r2/r3 | all0 | seen 并集 | seen bits |
|---|---|---|---|---|---|---|---|
| original-serial | —/—/— | —/—/— | —/—/— | —/—/— | 否 | — | — |
| banked-serial | 0/0/0 | 0/0/0 | 0/0/0 | 0/0/0 | 是 | 0x0000 | — |
| banked-hit | 0/0/0 | 0/0/0 | 0/0/0 | 0/0/0 | 是 | 0x8000 | runtime_enable |

| label | base_s1_ptw | full_window | candidate | runtime_enable | probe_starts | dc_eval | dc_good | leaf_eval | leaf_good | dc_msi_rej | leaf_msi_rej | leaf_miss | fast_acc |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | 0/3 | 0/3 | 0/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-serial | 0/3 | 0/3 | 0/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-hit | 0/3 | 0/3 | 0/3 | 3/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

#### 条件 A1（all_fast_zero = 是）

| label | fast V r1/r2/r3 | fast A r1/r2/r3 | fast Σ r1/r2/r3 | fast_acc r1/r2/r3 | all0 | seen 并集 | seen bits |
|---|---|---|---|---|---|---|---|
| original-serial | —/—/— | —/—/— | —/—/— | —/—/— | 否 | — | — |
| banked-serial | 0/0/0 | 0/0/0 | 0/0/0 | 0/0/0 | 是 | 0x0000 | — |
| banked-hit | 0/0/0 | 0/0/0 | 0/0/0 | 0/0/0 | 是 | 0x8000 | runtime_enable |

| label | base_s1_ptw | full_window | candidate | runtime_enable | probe_starts | dc_eval | dc_good | leaf_eval | leaf_good | dc_msi_rej | leaf_msi_rej | leaf_miss | fast_acc |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | 0/3 | 0/3 | 0/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-serial | 0/3 | 0/3 | 0/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-hit | 0/3 | 0/3 | 0/3 | 3/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

#### 条件 A8（all_fast_zero = 否）

| label | fast V r1/r2/r3 | fast A r1/r2/r3 | fast Σ r1/r2/r3 | fast_acc r1/r2/r3 | all0 | seen 并集 | seen bits |
|---|---|---|---|---|---|---|---|
| original-serial | —/—/— | —/—/— | —/—/— | —/—/— | 否 | — | — |
| banked-serial | 0/0/0 | 0/0/0 | 0/0/0 | 0/0/0 | 是 | 0x0007 | base_s1_ptw_active,full_fast_window,candidate_in_window |
| banked-hit | 6142/6144/6144 | 2/2/2 | 6144/6146/6146 | 6144/6146/6146 | 否 | 0x88FF | base_s1_ptw_active,full_fast_window,candidate_in_window,probe_start,dc_eval,dc_good,leaf_eval,leaf_good,fast_accept,runtime_enable |

| label | base_s1_ptw | full_window | candidate | runtime_enable | probe_starts | dc_eval | dc_good | leaf_eval | leaf_good | dc_msi_rej | leaf_msi_rej | leaf_miss | fast_acc |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | 0/3 | 0/3 | 0/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-serial | 3/3 | 3/3 | 3/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-hit | 3/3 | 3/3 | 3/3 | 3/3 | 18436 | 18436 | 18436 | 18436 | 18436 | 0 | 0 | 0 | 18436 |

#### 条件 A16（all_fast_zero = 否）

| label | fast V r1/r2/r3 | fast A r1/r2/r3 | fast Σ r1/r2/r3 | fast_acc r1/r2/r3 | all0 | seen 并集 | seen bits |
|---|---|---|---|---|---|---|---|
| original-serial | —/—/— | —/—/— | —/—/— | —/—/— | 否 | — | — |
| banked-serial | 0/0/0 | 0/0/0 | 0/0/0 | 0/0/0 | 是 | 0x0007 | base_s1_ptw_active,full_fast_window,candidate_in_window |
| banked-hit | 8193/8193/8193 | 0/0/0 | 8193/8193/8193 | 8193/8193/8193 | 否 | 0x8CFF | base_s1_ptw_active,full_fast_window,candidate_in_window,probe_start,dc_eval,dc_good,leaf_eval,leaf_good,leaf_lookup_miss,fast_accept,runtime_enable |

| label | base_s1_ptw | full_window | candidate | runtime_enable | probe_starts | dc_eval | dc_good | leaf_eval | leaf_good | dc_msi_rej | leaf_msi_rej | leaf_miss | fast_acc |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | 0/3 | 0/3 | 0/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-serial | 3/3 | 3/3 | 3/3 | 0/3 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| banked-hit | 3/3 | 3/3 | 3/3 | 3/3 | 24582 | 24582 | 24582 | 24582 | 24579 | 0 | 0 | 3 | 24579 |

> 计数窗口 (open, close]，跨窗口的部分 probe 合法；start/eval/leaf 之间不假设完整生命周期关系。seen bit0 是宽口径 base S1 PTW，可能含 implicit/debug。若未见 bit2（candidate）或 probe 为 0，只能说本轮未观测到合格窗口候选，不能说明没有 cache 命中。DC good=0 且无显式 MSI reject 时不能唯一归因 MSI。fast>0 只表示本轮真实 accept，不是命中率，也不是 novelty；fast 全 0 是科学上未行使，不是技术失败。

## 8. 原始字段证据

### 8.1 p4m.raw_window 逐 case（16 字，原样，未做任何解释性替换）

| label | case | w0 | w1 | w2 | w3 | w4 | w5 | w6 | w7 | w8 | w9 | w10 | w11 | w12 | w13 | w14 | w15 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | short-solo | 5279429606098881793 | 44146 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 64 | 64 | 2 |
| original-serial | short-a1 | 5279429606098881793 | 83470 | 64 | 64 | 1 | 1 | 1 | 0 | 49 | 113 | 49 | 112 | 0 | 64 | 64 | 7 |
| original-serial | short-a8 | 5279429606098881793 | 113640 | 64 | 64 | 1 | 1 | 1 | 0 | 6 | 70 | 6 | 69 | 0 | 64 | 64 | 7 |
| original-serial | long-r1-solo | 5279429606098881793 | 2828343 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| original-serial | long-r1-a1 | 5279429606098881793 | 5359179 | 4096 | 4096 | 1 | 1 | 1 | 0 | 51 | 4147 | 51 | 4146 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r1-a8 | 5279429606098881793 | 7349054 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r1-a16 | 5279429606098881793 | 7945520 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r2-a16 | 5279429606098881793 | 7946297 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r2-a8 | 5279429606098881793 | 7347789 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r2-a1 | 5279429606098881793 | 5360287 | 4096 | 4096 | 1 | 1 | 1 | 0 | 52 | 4148 | 52 | 4147 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r2-solo | 5279429606098881793 | 2828763 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| original-serial | long-r3-solo | 5279429606098881793 | 2832793 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| original-serial | long-r3-a1 | 5279429606098881793 | 5358650 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r3-a8 | 5279429606098881793 | 7353256 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| original-serial | long-r3-a16 | 5279429606098881793 | 7942861 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | short-solo | 5279429606098881793 | 44053 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 64 | 64 | 2 |
| banked-serial | short-a1 | 5279429606098881793 | 86709 | 64 | 64 | 1 | 1 | 1 | 0 | 50 | 114 | 50 | 113 | 0 | 64 | 64 | 7 |
| banked-serial | short-a8 | 5279429606098881793 | 140432 | 64 | 64 | 1 | 1 | 1 | 0 | 6 | 70 | 6 | 69 | 0 | 64 | 64 | 7 |
| banked-serial | long-r1-solo | 5279429606098881793 | 2826957 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-serial | long-r1-a1 | 5279429606098881793 | 5495261 | 4096 | 4096 | 1 | 1 | 1 | 0 | 61 | 4157 | 61 | 4156 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r1-a8 | 5279429606098881793 | 8951217 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r1-a16 | 5279429606098881793 | 9929407 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-a16 | 5279429606098881793 | 9924601 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-a8 | 5279429606098881793 | 8954802 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-a1 | 5279429606098881793 | 5499765 | 4096 | 4096 | 1 | 1 | 1 | 0 | 64 | 4160 | 64 | 4159 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-solo | 5279429606098881793 | 2825050 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-serial | long-r3-solo | 5279429606098881793 | 2826503 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-serial | long-r3-a1 | 5279429606098881793 | 5501111 | 4096 | 4096 | 1 | 1 | 1 | 0 | 62 | 4158 | 62 | 4157 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r3-a8 | 5279429606098881793 | 8959431 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r3-a16 | 5279429606098881793 | 9916294 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | short-solo | 5279429606098881793 | 44054 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 64 | 64 | 2 |
| banked-hit | short-a1 | 5279429606098881793 | 86662 | 64 | 64 | 1 | 1 | 1 | 0 | 49 | 113 | 49 | 112 | 0 | 64 | 64 | 7 |
| banked-hit | short-a8 | 5279429606098881793 | 135203 | 64 | 64 | 1 | 1 | 1 | 0 | 6 | 70 | 6 | 69 | 0 | 64 | 64 | 7 |
| banked-hit | long-r1-solo | 5279429606098881793 | 2826372 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-hit | long-r1-a1 | 5279429606098881793 | 5499331 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r1-a8 | 5279429606098881793 | 7672922 | 4096 | 4096 | 1 | 1 | 1 | 0 | 13 | 4109 | 13 | 4108 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r1-a16 | 5279429606098881793 | 10198914 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-a16 | 5279429606098881793 | 10214171 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-a8 | 5279429606098881793 | 8603325 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-a1 | 5279429606098881793 | 5495410 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-solo | 5279429606098881793 | 2823084 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-hit | long-r3-solo | 5279429606098881793 | 2823168 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-hit | long-r3-a1 | 5279429606098881793 | 5498262 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r3-a8 | 5279429606098881793 | 8597740 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r3-a16 | 5279429606098881793 | 10195105 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |

字段定位：

- 由 `scripts/mac_nonblock_pilot.py` 的 `validate_record()` 钉死：`w0` = bank magic，`w1` = 窗口周期数（1 字 = 1 cycle，== `raw_observation[0]`），`w13` = V issued，`w14` = V done（后三者应等于该 case 的 transfers）。
- 由 `bench/idma_experiment.c:487` 确认：`w3` = 窗口内 A 完成数（`a_completed_jobs_window`），与 `p4m.a_completed_jobs_window` 交叉核对；同窗口 A 字节 = `w3 × 4096`。
- 其余 `w2`、`w4..w12`、`w15` 本脚本只原样保存，不做语义解释。

### 8.2 A 窗口内完成数的取值来源（逐 case）

| label | case | A jobs | 字段来源 | 状态 |
|---|---|---|---|---|
| original-serial | long-r1-solo | 0 | record.p4m.raw_window[3] | complete |
| original-serial | long-r1-a1 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r1-a8 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r1-a16 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r2-a16 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r2-a8 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r2-a1 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r2-solo | 0 | record.p4m.raw_window[3] | complete |
| original-serial | long-r3-solo | 0 | record.p4m.raw_window[3] | complete |
| original-serial | long-r3-a1 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r3-a8 | 4096 | record.p4m.raw_window[3] | complete |
| original-serial | long-r3-a16 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r1-solo | 0 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r1-a1 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r1-a8 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r1-a16 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r2-a16 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r2-a8 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r2-a1 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r2-solo | 0 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r3-solo | 0 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r3-a1 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r3-a8 | 4096 | record.p4m.raw_window[3] | complete |
| banked-serial | long-r3-a16 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r1-solo | 0 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r1-a1 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r1-a8 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r1-a16 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r2-a16 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r2-a8 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r2-a1 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r2-solo | 0 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r3-solo | 0 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r3-a1 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r3-a8 | 4096 | record.p4m.raw_window[3] | complete |
| banked-hit | long-r3-a16 | 4096 | record.p4m.raw_window[3] | complete |

### 8.3 helper 窗口摘要（完整 16 字见 JSON `cases[*].helper.words`）

| label | case | mode | seq=words[4] | cycles | flags | errors | Σfast | Σnormal | accepted | 来源 |
|---|---|---|---|---|---|---|---|---|---|---|
| banked-serial | short-solo | 1 | 1 | 44053 | 4 | 0 | 0 | 256 | 256 | helper.json |
| banked-serial | short-a1 | 1 | 2 | 86709 | 4 | 0 | 0 | 512 | 512 | helper.json |
| banked-serial | short-a8 | 1 | 3 | 140432 | 4 | 0 | 0 | 510 | 510 | helper.json |
| banked-serial | long-r1-solo | 1 | 4 | 2826957 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-serial | long-r1-a1 | 1 | 5 | 5495261 | 4 | 0 | 0 | 32768 | 32768 | helper.json |
| banked-serial | long-r1-a8 | 1 | 6 | 8951217 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r1-a16 | 1 | 7 | 9929407 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r2-a16 | 1 | 8 | 9924601 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r2-a8 | 1 | 9 | 8954802 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r2-a1 | 1 | 10 | 5499765 | 4 | 0 | 0 | 32768 | 32768 | helper.json |
| banked-serial | long-r2-solo | 1 | 11 | 2825050 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-serial | long-r3-solo | 1 | 12 | 2826503 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-serial | long-r3-a1 | 1 | 13 | 5501111 | 4 | 0 | 0 | 32768 | 32768 | helper.json |
| banked-serial | long-r3-a8 | 1 | 14 | 8959431 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r3-a16 | 1 | 15 | 9916294 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | short-solo | 2 | 1 | 44054 | 4 | 0 | 0 | 256 | 256 | helper.json |
| banked-hit | short-a1 | 2 | 2 | 86662 | 4 | 0 | 0 | 512 | 512 | helper.json |
| banked-hit | short-a8 | 2 | 3 | 135203 | 4 | 0 | 98 | 412 | 510 | helper.json |
| banked-hit | long-r1-solo | 2 | 4 | 2826372 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-hit | long-r1-a1 | 2 | 5 | 5499331 | 4 | 0 | 0 | 32768 | 32768 | helper.json |
| banked-hit | long-r1-a8 | 2 | 6 | 7672922 | 4 | 0 | 6144 | 26624 | 32768 | helper.json |
| banked-hit | long-r1-a16 | 2 | 7 | 10198914 | 4 | 0 | 8193 | 24573 | 32766 | helper.json |
| banked-hit | long-r2-a16 | 2 | 8 | 10214171 | 4 | 0 | 8193 | 24573 | 32766 | helper.json |
| banked-hit | long-r2-a8 | 2 | 9 | 8603325 | 4 | 0 | 6146 | 26620 | 32766 | helper.json |
| banked-hit | long-r2-a1 | 2 | 10 | 5495410 | 4 | 0 | 0 | 32768 | 32768 | helper.json |
| banked-hit | long-r2-solo | 2 | 11 | 2823084 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-hit | long-r3-solo | 2 | 12 | 2823168 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-hit | long-r3-a1 | 2 | 13 | 5498262 | 4 | 0 | 0 | 32768 | 32768 | helper.json |
| banked-hit | long-r3-a8 | 2 | 14 | 8597740 | 4 | 0 | 6146 | 26620 | 32766 | helper.json |
| banked-hit | long-r3-a16 | 2 | 15 | 10195105 | 4 | 0 | 8193 | 24573 | 32766 | helper.json |

### 8.4 p4m.raw_observation

`raw_observation` 是旧 HPM 观测数组（本脚本只记录长度，值在 JSON 里原样保留）：

| label | case | len(raw_observation) |
|---|---|---|
| original-serial | short-solo | 35 |
| original-serial | short-a1 | 35 |
| original-serial | short-a8 | 35 |
| original-serial | long-r1-solo | 35 |
| original-serial | long-r1-a1 | 35 |
| original-serial | long-r1-a8 | 35 |
| original-serial | long-r1-a16 | 35 |
| original-serial | long-r2-a16 | 35 |
| original-serial | long-r2-a8 | 35 |
| original-serial | long-r2-a1 | 35 |
| original-serial | long-r2-solo | 35 |
| original-serial | long-r3-solo | 35 |
| original-serial | long-r3-a1 | 35 |
| original-serial | long-r3-a8 | 35 |
| original-serial | long-r3-a16 | 35 |
| banked-serial | short-solo | 35 |
| banked-serial | short-a1 | 35 |
| banked-serial | short-a8 | 35 |
| banked-serial | long-r1-solo | 35 |
| banked-serial | long-r1-a1 | 35 |
| banked-serial | long-r1-a8 | 35 |
| banked-serial | long-r1-a16 | 35 |
| banked-serial | long-r2-a16 | 35 |
| banked-serial | long-r2-a8 | 35 |
| banked-serial | long-r2-a1 | 35 |
| banked-serial | long-r2-solo | 35 |
| banked-serial | long-r3-solo | 35 |
| banked-serial | long-r3-a1 | 35 |
| banked-serial | long-r3-a8 | 35 |
| banked-serial | long-r3-a16 | 35 |
| banked-hit | short-solo | 35 |
| banked-hit | short-a1 | 35 |
| banked-hit | short-a8 | 35 |
| banked-hit | long-r1-solo | 35 |
| banked-hit | long-r1-a1 | 35 |
| banked-hit | long-r1-a8 | 35 |
| banked-hit | long-r1-a16 | 35 |
| banked-hit | long-r2-a16 | 35 |
| banked-hit | long-r2-a8 | 35 |
| banked-hit | long-r2-a1 | 35 |
| banked-hit | long-r2-solo | 35 |
| banked-hit | long-r3-solo | 35 |
| banked-hit | long-r3-a1 | 35 |
| banked-hit | long-r3-a8 | 35 |
| banked-hit | long-r3-a16 | 35 |

> 旧 HPM / COST / E3 / miss / PTW 数据 **不作为 N 机制证据，也不作为有效 metric**；banked 结构下 `victim/aggressor.iotlb_miss/s1_ptw/s2_ptw` 一律不得使用。helper 的快速路径事件数 **不是命中率，也不是全部 miss 数**。

## 9. 不确定性、未运行项与未做的事

- `bench/idma_experiment.c:487` 明确窗口内 A 完成数 = `raw_window[3]`；本脚本据此设 `RAW_WINDOW_A_JOBS_INDEX = 3`，并与 `record.p4m.a_completed_jobs_window` 交叉核对，绝不使用整轮 A 计数。
- 窗口周期数取自 `raw_window[1]`，单位按 1 字 = 1 cycle 处理；ns 与 MiB/s 是以 50 MHz 为假设的推导量，不是直接测量值。V 吞吐与同窗口 A 吞吐共用这一窗口换算。
- 真实 V 工作量固定为 transfers × 4096 B；同窗口 A 工作量 = w3 × 4096 B；脚本按此计算，不重新测数据量。
- A 速度只用同一窗口的 A 吞吐评价；不用不同窗口的 A 完成数直接比较 A 速度。
- 短组只用做验收，不与长组混合；长组每条件三轮值 + 中位/min/max 均为描述性，不做显著性检验，不能据此外推。
- 失败 / 残缺数据逐项保留 raw 并分类，只作诊断；同一比较条件三轮技术上未全部合格时，不发布该条件的性能汇总与两段对照，也不把残缺数值与有效样本混合，更不剔除不利但合法的合格样本。fast=0 是合法观察，不产生 issue、不剔除。
- 每个 label 只对应一次运行、一次新鲜启动；跨 label 的差异同时包含运行间波动，本脚本不做方差分析。S 来源受限于旧 legacy3 与 S/N 512 缓冲差异，不得声称因果。
- banked-hit 相对 banked-serial 的差值只是描述性 `N/S变化`；original-serial 与 banked-hit 的直接差值不用于绕过归因。本轮 fast 计数全 0 时，`N/S变化` 不构成绕过效果证据。
- helper 窗口语义为 (open, close]；序号一律取 `words[4]`（reader emit_json 的 `windowseq` 亦等于 `words[4]`，仅在存在时做交叉核对）；脚本只校验 ABI 冻结位、计数守恒、与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号严格递增。
- `--diagnostics` 打开时，第二 16 字诊断 bank 从 `helper.json` 的 `diagnostic` 对象读取（`diag-window.json` 仅在存在时交叉核对）；关闭时行为与旧版一致。
- 诊断 bank 的字段语义由 RTL/runner 契约决定；本脚本只做只读交叉核对，不实现硬件。
- 只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；因此计划外的额外 case 目录不会被发现。
- 技术资格（qualified）已汇合该 case 的全部质量 issue 与该 label 的 plan/boot/flash/caps/summary 必要质量问题；合法偏低或波动样本与 fast=0 不产生 issue，仍保留并参与汇总，但任一真实 issue 都会阻止两段对照发布。
- 不做哈希校验、不从设备读回、不重跑、不修改任何原始文件。
- 旧 HPM / COST / E3 / miss / PTW 数据不是本设计的机制证据；helper 快速路径事件数不是命中率，也不是全部 miss 数；不宣称 novelty。

未运行 / 失败 / 不完整的用例已逐条列在第 2 节与第 3 节，不做补值、不做剔除、不自动重跑。

