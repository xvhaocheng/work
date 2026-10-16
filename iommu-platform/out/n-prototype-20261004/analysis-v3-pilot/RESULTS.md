# N 非阻塞工程 pilot 离线汇总（RESULTS.md）

- 生成时间：2026-10-04T02:47:31Z
- 分析脚本：`scripts/analyze_nonblock_pilot.py`（离线汇总工具；可用性见本报告验收项）
- 汇总 JSON：`nonblock-pilot-analysis.json`
- 数据状态：**完整**（脚本退出码 = 0）
- 时钟换算假设：50 MHz（1 cycle = 20.0 ns，仅用于时间/吞吐推导）
- 真实 V 固定工作量：short = 262144 B，long = 16777216 B（= transfers × 4096）

> 本文件由脚本按固定模板生成，是描述性汇总，不是实验结论声明；
> 不含显著性统计，不含 novelty 判断，不对未运行/失败项做推测或补值，
> 不修改任何原始文件。

## 1. 数据来源与运行身份

| label | 目录 | 存在 | run_id | BIT | device | summary.status | STOPPED |
|---|---|---|---|---|---|---|---|
| original-serial | `out/n-prototype-20261004/nonblock-pilot-20261004-original-03` | 是 | a72442e2e112f401 | `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/nonblock-pilot-20261004/original-serial.bit` | /dev/idma0 | complete | 无 |
| banked-serial | `out/n-prototype-20261004/nonblock-pilot-20261004-serial-v3-01` | 是 | 3ae491ffaa3df550 | `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/nonblock-pilot-20261004/banked-serial-v3.bit` | /dev/idma0 | complete | 无 |
| banked-hit | `out/n-prototype-20261004/nonblock-pilot-20261004-hit-v3-01` | 是 | cc371e91173ce50a | `/Users/jojo/Desktop/在mac链接pfga(codex)/tools/nonblock-pilot-20261004/banked-hit-v3.bit` | /dev/idma0 | complete | 无 |

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
| 1 | banked-serial / short-solo | short | Solo | — | complete | 44038 | 0 |
| 2 | banked-serial / short-a1 | short | A1 | — | complete | 87276 | 64 |
| 3 | banked-serial / short-a8 | short | A8 | — | complete | 119158 | 64 |
| 4 | banked-serial / long-r1-solo | long | Solo | 1 | complete | 2820368 | 0 |
| 5 | banked-serial / long-r1-a1 | long | A1 | 1 | complete | 5581750 | 4096 |
| 6 | banked-serial / long-r1-a8 | long | A8 | 1 | complete | 7666937 | 4096 |
| 7 | banked-serial / long-r1-a16 | long | A16 | 1 | complete | 8275137 | 4096 |
| 8 | banked-serial / long-r2-a16 | long | A16 | 2 | complete | 8272332 | 4096 |
| 9 | banked-serial / long-r2-a8 | long | A8 | 2 | complete | 7669765 | 4096 |
| 10 | banked-serial / long-r2-a1 | long | A1 | 2 | complete | 5584177 | 4096 |
| 11 | banked-serial / long-r2-solo | long | Solo | 2 | complete | 2825110 | 0 |
| 12 | banked-serial / long-r3-solo | long | Solo | 3 | complete | 2825919 | 0 |
| 13 | banked-serial / long-r3-a1 | long | A1 | 3 | complete | 5585686 | 4096 |
| 14 | banked-serial / long-r3-a8 | long | A8 | 3 | complete | 7667658 | 4096 |
| 15 | banked-serial / long-r3-a16 | long | A16 | 3 | complete | 8274495 | 4096 |
| 1 | banked-hit / short-solo | short | Solo | — | complete | 44064 | 0 |
| 2 | banked-hit / short-a1 | short | A1 | — | complete | 87929 | 64 |
| 3 | banked-hit / short-a8 | short | A8 | — | complete | 119030 | 64 |
| 4 | banked-hit / long-r1-solo | long | Solo | 1 | complete | 2831598 | 0 |
| 5 | banked-hit / long-r1-a1 | long | A1 | 1 | complete | 5586215 | 4096 |
| 6 | banked-hit / long-r1-a8 | long | A8 | 1 | complete | 7667627 | 4096 |
| 7 | banked-hit / long-r1-a16 | long | A16 | 1 | complete | 8277440 | 4096 |
| 8 | banked-hit / long-r2-a16 | long | A16 | 2 | complete | 8272399 | 4096 |
| 9 | banked-hit / long-r2-a8 | long | A8 | 2 | complete | 7669858 | 4096 |
| 10 | banked-hit / long-r2-a1 | long | A1 | 2 | complete | 5584794 | 4096 |
| 11 | banked-hit / long-r2-solo | long | Solo | 2 | complete | 2825984 | 0 |
| 12 | banked-hit / long-r3-solo | long | Solo | 3 | complete | 2825890 | 0 |
| 13 | banked-hit / long-r3-a1 | long | A1 | 3 | complete | 5579282 | 4096 |
| 14 | banked-hit / long-r3-a8 | long | A8 | 3 | complete | 7667630 | 4096 |
| 15 | banked-hit / long-r3-a16 | long | A16 | 3 | complete | 8275122 | 4096 |

## 3. 质量问题清单（不剔除、不修补、不重跑）

无。

### 3.2 runner 记录的 case problems（原样保留）

无。

## 4. 短组验收（3 例，只做验收，不与长组混合）

> 短组只做验收（3 例），不与长组混合，不参与布局/绕过效应比较，不做显著性。A 吞吐为同一窗口内的 A 完成字节速率，不是总线吞吐。

| label | case | condition | state | qualified | cycles | ns | V MiB/s | A jobs | A B | A MiB/s | A 字段来源 | raw_window 一致 | helper 一致 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| original-serial | short-a1 | A1 | complete | 是 | 83470 | 1669400.0 | 149.75 | 64 | 262144 | 149.75 | record.p4m.raw_window[3] | 是 | 不适用 |
| original-serial | short-a8 | A8 | complete | 是 | 113640 | 2272800.0 | 110.00 | 64 | 262144 | 110.00 | record.p4m.raw_window[3] | 是 | 不适用 |
| original-serial | short-solo | Solo | complete | 是 | 44146 | 882920.0 | 283.15 | 0 | 0 | 0.00 | record.p4m.raw_window[3] | 是 | 不适用 |
| banked-serial | short-a1 | A1 | complete | 是 | 87276 | 1745520.0 | 143.22 | 64 | 262144 | 143.22 | record.p4m.raw_window[3] | 是 | 是 |
| banked-serial | short-a8 | A8 | complete | 是 | 119158 | 2383160.0 | 104.90 | 64 | 262144 | 104.90 | record.p4m.raw_window[3] | 是 | 是 |
| banked-serial | short-solo | Solo | complete | 是 | 44038 | 880760.0 | 283.85 | 0 | 0 | 0.00 | record.p4m.raw_window[3] | 是 | 是 |
| banked-hit | short-a1 | A1 | complete | 是 | 87929 | 1758580.0 | 142.16 | 64 | 262144 | 142.16 | record.p4m.raw_window[3] | 是 | 是 |
| banked-hit | short-a8 | A8 | complete | 是 | 119030 | 2380600.0 | 105.02 | 64 | 262144 | 105.02 | record.p4m.raw_window[3] | 是 | 是 |
| banked-hit | short-solo | Solo | complete | 是 | 44064 | 881280.0 | 283.68 | 0 | 0 | 0.00 | record.p4m.raw_window[3] | 是 | 是 |

## 5. 长组：每条件每次值 + 中位/min/max

> long 组每条件给出三轮每次原值 + 中位/min/max（描述性，不做显著性）。第 2 轮顺序为反向，请按轮次看。仅当同一条件三个 label 各自三轮全部技术合格时才发布 median/min/max；否则只保留逐轮原值并标为仅诊断。V 吞吐与同窗口 A 吞吐分列，不用不同窗口的 A 完成数比较 A 速度。

### 5.1 条件 Solo

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 2828343 | 2828763 | 2832793 | 2828763 | 2828343 | 2832793 | 3 |
| banked-serial | 是 | 2820368 | 2825110 | 2825919 | 2825110 | 2820368 | 2825919 | 3 |
| banked-hit | 是 | 2831598 | 2825984 | 2825890 | 2825984 | 2825890 | 2831598 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 282.851 | 282.809 | 282.407 | 282.809 | 282.407 | 282.851 | 3 |
| banked-serial | 是 | 283.651 | 283.175 | 283.094 | 283.175 | 283.094 | 283.651 | 3 |
| banked-hit | 是 | 282.526 | 283.087 | 283.097 | 283.087 | 282.526 | 283.097 | 3 |

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
| banked-serial | 是 | 5581750 | 5584177 | 5585686 | 5584177 | 5581750 | 5585686 | 3 |
| banked-hit | 是 | 5586215 | 5584794 | 5579282 | 5584794 | 5579282 | 5586215 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 149.277 | 149.246 | 149.291 | 149.277 | 149.246 | 149.291 | 3 |
| banked-serial | 是 | 143.324 | 143.262 | 143.223 | 143.262 | 143.223 | 143.324 | 3 |
| banked-hit | 是 | 143.21 | 143.246 | 143.388 | 143.246 | 143.21 | 143.388 | 3 |

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
| banked-serial | 是 | 143.324 | 143.262 | 143.223 | 143.262 | 143.223 | 143.324 | 3 |
| banked-hit | 是 | 143.21 | 143.246 | 143.388 | 143.246 | 143.21 | 143.388 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

### 5.3 条件 A8

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 7349054 | 7347789 | 7353256 | 7349054 | 7347789 | 7353256 | 3 |
| banked-serial | 是 | 7666937 | 7669765 | 7667658 | 7667658 | 7666937 | 7669765 | 3 |
| banked-hit | 是 | 7667627 | 7669858 | 7667630 | 7667630 | 7667627 | 7669858 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 108.858 | 108.876 | 108.795 | 108.858 | 108.795 | 108.876 | 3 |
| banked-serial | 是 | 104.344 | 104.306 | 104.334 | 104.334 | 104.306 | 104.344 | 3 |
| banked-hit | 是 | 104.335 | 104.304 | 104.335 | 104.335 | 104.304 | 104.335 | 3 |

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
| banked-serial | 是 | 104.344 | 104.306 | 104.334 | 104.334 | 104.306 | 104.344 | 3 |
| banked-hit | 是 | 104.335 | 104.304 | 104.335 | 104.335 | 104.304 | 104.335 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

### 5.4 条件 A16

**cycles** — 窗口周期数 raw_window[1]

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 7945520 | 7946297 | 7942861 | 7945520 | 7942861 | 7946297 | 3 |
| banked-serial | 是 | 8275137 | 8272332 | 8274495 | 8274495 | 8272332 | 8275137 | 3 |
| banked-hit | 是 | 8277440 | 8272399 | 8275122 | 8275122 | 8272399 | 8277440 | 3 |

**v_mib_s** — V 吞吐 MiB/s（transfers×4096 / 窗口）

| label | qualified | r1 | r2 | r3 | 中位 | min | max | n |
|---|---|---|---|---|---|---|---|---|
| original-serial | 是 | 100.686 | 100.676 | 100.719 | 100.686 | 100.676 | 100.719 | 3 |
| banked-serial | 是 | 96.675 | 96.708 | 96.683 | 96.683 | 96.675 | 96.708 | 3 |
| banked-hit | 是 | 96.648 | 96.707 | 96.675 | 96.675 | 96.648 | 96.707 | 3 |

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
| banked-serial | 是 | 96.675 | 96.708 | 96.683 | 96.683 | 96.675 | 96.708 | 3 |
| banked-hit | 是 | 96.648 | 96.707 | 96.675 | 96.675 | 96.648 | 96.707 | 3 |

> 条件整体合格 = 是；不合格时该条件 median/min/max 为空。

## 6. 布局效应与绕过效应分解（仅 long 组）

- 布局效应：`banked-serial − original-serial（同串行行为、不同银行布局）：original -> bankedSerial`
- 绕过效应：`banked-hit − banked-serial（同布局，仅运行时使能绕过）：bankedSerial -> bankedHit`
- **不允许用 original-serial 与 banked-hit 的直接差值归因绕过效应；绕过效应必须以 banked-serial 为中介。**
- V 吞吐与同窗口 A 吞吐均以 raw_window[1] 为窗口、50 MHz 换算（推导量）；A 速度只用同窗口 A 吞吐评价，不用不同窗口 A 完成数直接比较。
- 发布规则：同一比较条件在 original-serial/banked-serial/banked-hit 三个 label 各自三轮（共 3×3 次）技术上全部合格时才发布；否则只列 reasons，不发布数值。
- 仅描述性中位/min/max，不做显著性检验。

### 6.1 V 吞吐（MiB/s）收益 / 代价

| condition | published | orig 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |
|---|---|---|---|---|---|---|---|---|
| Solo | 是 | 282.809 | 283.175 | 283.087 | 0.366 | +0.1% | -0.088 | -0.0% |
| A1 | 是 | 149.277 | 143.262 | 143.246 | -6.015 | -4.0% | -0.016 | -0.0% |
| A8 | 是 | 108.858 | 104.334 | 104.335 | -4.523 | -4.2% | 0 | +0.0% |
| A16 | 是 | 100.686 | 96.683 | 96.675 | -4.003 | -4.0% | -0.007 | -0.0% |


### 6.2 同窗口 A 吞吐（MiB/s）收益 / 代价

| condition | published | orig 中位 | serial 中位 | hit 中位 | 布局 Δ | 布局 Δ% | 绕过 Δ | 绕过 Δ% |
|---|---|---|---|---|---|---|---|---|
| Solo | 是 | 0 | 0 | 0 | 0 | — | 0 | — |
| A1 | 是 | 149.277 | 143.262 | 143.246 | -6.015 | -4.0% | -0.016 | -0.0% |
| A8 | 是 | 108.858 | 104.334 | 104.335 | -4.523 | -4.2% | 0 | +0.0% |
| A16 | 是 | 100.686 | 96.683 | 96.675 | -4.003 | -4.0% | -0.007 | -0.0% |


## 7. 原始字段证据

### 7.1 p4m.raw_window 逐 case（16 字，原样，未做任何解释性替换）

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
| banked-serial | short-solo | 5279429606098881793 | 44038 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 64 | 64 | 2 |
| banked-serial | short-a1 | 5279429606098881793 | 87276 | 64 | 64 | 1 | 1 | 1 | 0 | 61 | 125 | 61 | 124 | 0 | 64 | 64 | 7 |
| banked-serial | short-a8 | 5279429606098881793 | 119158 | 64 | 64 | 1 | 1 | 1 | 0 | 6 | 70 | 6 | 69 | 0 | 64 | 64 | 7 |
| banked-serial | long-r1-solo | 5279429606098881793 | 2820368 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-serial | long-r1-a1 | 5279429606098881793 | 5581750 | 4096 | 4096 | 1 | 1 | 1 | 0 | 51 | 4147 | 51 | 4146 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r1-a8 | 5279429606098881793 | 7666937 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r1-a16 | 5279429606098881793 | 8275137 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-a16 | 5279429606098881793 | 8272332 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-a8 | 5279429606098881793 | 7669765 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-a1 | 5279429606098881793 | 5584177 | 4096 | 4096 | 1 | 1 | 1 | 0 | 49 | 4145 | 49 | 4144 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r2-solo | 5279429606098881793 | 2825110 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-serial | long-r3-solo | 5279429606098881793 | 2825919 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-serial | long-r3-a1 | 5279429606098881793 | 5585686 | 4096 | 4096 | 1 | 1 | 1 | 0 | 61 | 4157 | 61 | 4156 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r3-a8 | 5279429606098881793 | 7667658 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-serial | long-r3-a16 | 5279429606098881793 | 8274495 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | short-solo | 5279429606098881793 | 44064 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 64 | 64 | 2 |
| banked-hit | short-a1 | 5279429606098881793 | 87929 | 64 | 64 | 1 | 1 | 1 | 0 | 49 | 113 | 49 | 112 | 0 | 64 | 64 | 7 |
| banked-hit | short-a8 | 5279429606098881793 | 119030 | 64 | 64 | 1 | 1 | 1 | 0 | 6 | 70 | 6 | 69 | 0 | 64 | 64 | 7 |
| banked-hit | long-r1-solo | 5279429606098881793 | 2831598 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-hit | long-r1-a1 | 5279429606098881793 | 5586215 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r1-a8 | 5279429606098881793 | 7667627 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r1-a16 | 5279429606098881793 | 8277440 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-a16 | 5279429606098881793 | 8272399 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-a8 | 5279429606098881793 | 7669858 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-a1 | 5279429606098881793 | 5584794 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r2-solo | 5279429606098881793 | 2825984 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-hit | long-r3-solo | 5279429606098881793 | 2825890 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 4096 | 4096 | 2 |
| banked-hit | long-r3-a1 | 5279429606098881793 | 5579282 | 4096 | 4096 | 1 | 1 | 1 | 0 | 50 | 4146 | 50 | 4145 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r3-a8 | 5279429606098881793 | 7667630 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |
| banked-hit | long-r3-a16 | 5279429606098881793 | 8275122 | 4096 | 4096 | 1 | 1 | 1 | 0 | 6 | 4102 | 6 | 4101 | 0 | 4096 | 4096 | 7 |

字段定位：

- 由 `scripts/mac_nonblock_pilot.py` 的 `validate_record()` 钉死：`w0` = bank magic，`w1` = 窗口周期数（1 字 = 1 cycle，== `raw_observation[0]`），`w13` = V issued，`w14` = V done（后三者应等于该 case 的 transfers）。
- 由 `bench/idma_experiment.c:487` 确认：`w3` = 窗口内 A 完成数（`a_completed_jobs_window`），与 `p4m.a_completed_jobs_window` 交叉核对；同窗口 A 字节 = `w3 × 4096`。
- 其余 `w2`、`w4..w12`、`w15` 本脚本只原样保存，不做语义解释。

### 7.2 A 窗口内完成数的取值来源（逐 case）

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

### 7.3 helper 窗口摘要（完整 16 字见 JSON `cases[*].helper.words`）

| label | case | mode | seq=words[4] | cycles | flags | errors | Σfast | Σnormal | accepted | 来源 |
|---|---|---|---|---|---|---|---|---|---|---|
| banked-serial | short-solo | 1 | 1 | 44038 | 4 | 0 | 0 | 256 | 256 | helper.json |
| banked-serial | short-a1 | 1 | 2 | 87276 | 4 | 0 | 0 | 511 | 511 | helper.json |
| banked-serial | short-a8 | 1 | 3 | 119158 | 4 | 0 | 0 | 510 | 510 | helper.json |
| banked-serial | long-r1-solo | 1 | 4 | 2820368 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-serial | long-r1-a1 | 1 | 5 | 5581750 | 4 | 0 | 0 | 32767 | 32767 | helper.json |
| banked-serial | long-r1-a8 | 1 | 6 | 7666937 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r1-a16 | 1 | 7 | 8275137 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r2-a16 | 1 | 8 | 8272332 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r2-a8 | 1 | 9 | 7669765 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r2-a1 | 1 | 10 | 5584177 | 4 | 0 | 0 | 32767 | 32767 | helper.json |
| banked-serial | long-r2-solo | 1 | 11 | 2825110 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-serial | long-r3-solo | 1 | 12 | 2825919 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-serial | long-r3-a1 | 1 | 13 | 5585686 | 4 | 0 | 0 | 32767 | 32767 | helper.json |
| banked-serial | long-r3-a8 | 1 | 14 | 7667658 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-serial | long-r3-a16 | 1 | 15 | 8274495 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | short-solo | 2 | 1 | 44064 | 4 | 0 | 0 | 256 | 256 | helper.json |
| banked-hit | short-a1 | 2 | 2 | 87929 | 4 | 0 | 0 | 511 | 511 | helper.json |
| banked-hit | short-a8 | 2 | 3 | 119030 | 4 | 0 | 0 | 510 | 510 | helper.json |
| banked-hit | long-r1-solo | 2 | 4 | 2831598 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-hit | long-r1-a1 | 2 | 5 | 5586215 | 4 | 0 | 0 | 32767 | 32767 | helper.json |
| banked-hit | long-r1-a8 | 2 | 6 | 7667627 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | long-r1-a16 | 2 | 7 | 8277440 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | long-r2-a16 | 2 | 8 | 8272399 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | long-r2-a8 | 2 | 9 | 7669858 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | long-r2-a1 | 2 | 10 | 5584794 | 4 | 0 | 0 | 32767 | 32767 | helper.json |
| banked-hit | long-r2-solo | 2 | 11 | 2825984 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-hit | long-r3-solo | 2 | 12 | 2825890 | 4 | 0 | 0 | 16384 | 16384 | helper.json |
| banked-hit | long-r3-a1 | 2 | 13 | 5579282 | 4 | 0 | 0 | 32767 | 32767 | helper.json |
| banked-hit | long-r3-a8 | 2 | 14 | 7667630 | 4 | 0 | 0 | 32766 | 32766 | helper.json |
| banked-hit | long-r3-a16 | 2 | 15 | 8275122 | 4 | 0 | 0 | 32766 | 32766 | helper.json |

### 7.4 p4m.raw_observation

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

> 旧 HPM / COST / E3 数据 **不作为 N 机制证据**；helper 的快速路径事件数
> **不是命中率，也不是全部 miss 数**。

## 8. 不确定性、未运行项与未做的事

- `bench/idma_experiment.c:487` 明确窗口内 A 完成数 = `raw_window[3]`；本脚本据此设 `RAW_WINDOW_A_JOBS_INDEX = 3`，并与 `record.p4m.a_completed_jobs_window` 交叉核对，绝不使用整轮 A 计数。
- 窗口周期数取自 `raw_window[1]`，单位按 1 字 = 1 cycle 处理；ns 与 MiB/s 是以 50 MHz 为假设的推导量，不是直接测量值。V 吞吐与同窗口 A 吞吐共用这一窗口换算。
- 真实 V 工作量固定为 transfers × 4096 B；同窗口 A 工作量 = w3 × 4096 B；脚本按此计算，不重新测数据量。
- A 速度只用同一窗口的 A 吞吐评价；不用不同窗口的 A 完成数直接比较 A 速度。
- 短组只用做验收，不与长组混合；长组每条件三轮值 + 中位/min/max 均为描述性，不做显著性检验，不能据此外推。
- 失败 / 残缺数据逐项保留 raw 并分类，只作诊断；同一比较条件三轮技术上未全部合格时，不发布该条件的性能汇总与两段效应，也不把残缺数值与有效样本混合，更不剔除不利但合法的合格样本。
- 每个 label 只对应一次运行、一次新鲜启动；跨 label 的差异同时包含运行间波动，本脚本不做方差分析。
- banked-hit 相对 banked-serial 的差值才是绕过效应；original-serial 与 banked-hit 的直接差值不用于绕过归因。
- helper 窗口语义为 (open, close]；序号一律取 `words[4]`（reader emit_json 的 `windowseq` 亦等于 `words[4]`，仅在存在时做交叉核对）；脚本只校验 ABI 冻结位、计数守恒、与 P4M 相同的 cycles、Mode1 快速项全 0、Mode2 可大于 0、序号严格递增。
- 只按冻结计划的 15 个 case id 打开确切路径，未枚举目录；因此计划外的额外 case 目录不会被发现。
- 技术资格（qualified）已汇合该 case 的全部质量 issue 与该 label 的 plan/boot/flash/caps/summary 必要质量问题；合法偏低或波动样本不产生 issue，仍保留并参与汇总，但任一真实 issue 都会阻止效应发布。
- 不做哈希校验、不从设备读回、不重跑、不修改任何原始文件。
- 旧 HPM / COST / E3 数据不是本设计的机制证据；helper 快速路径事件数不是命中率，也不是全部 miss 数。

未运行 / 失败 / 不完整的用例已逐条列在第 2 节与第 3 节，不做补值、不做剔除、不自动重跑。

