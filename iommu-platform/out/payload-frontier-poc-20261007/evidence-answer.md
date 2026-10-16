以下仅基于你提供的 8 条日志内容做证据整理、算术和中文记录草案；我没有执行仿真，PASS 是日志中已有字段。目录前缀统一为 `out/payload-frontier-poc-20261007/`。

## 1. 日志分组与口径

- 早期校准（无夹具；按任务给定先于夹具/ScanEnable 编译代次）：
  - `out/payload-frontier-poc-20261007/strict-normal-pn.log`
  - `out/payload-frontier-poc-20261007/frontier-normal-pn.log`
- 最后模型（有限 64 cycle 写尾反压；strict / full-prefix / ordinary-virtual 的 P 与 PN）：
  - `out/payload-frontier-poc-20261007/strict-tail-pn.log`
  - `out/payload-frontier-poc-20261007/frontier-tail-pn.log`
  - `out/payload-frontier-poc-20261007/ordinary-virtual-pn.log`
  - `out/payload-frontier-poc-20261007/strict-tail-p.log`
  - `out/payload-frontier-poc-20261007/frontier-tail-p.log`
  - `out/payload-frontier-poc-20261007/ordinary-virtual-p.log`

统一口径：
- `W_cycles` 取 `ACTUAL-DMA-WINDOW-JSON` 第 11 行。
- 窗口 V/A 工作量取第 11 行 `v_completed` / `a_completed`；场景设置为 `v_transfers=64, a_pages=8, v_gap=0`（第 2、7 行）。
- `FAST` 窗口值取第 11 行 `fast/fast_bank0/fast_bank1`；累计 summary 取第 15 行 `fast/fast_bank0/fast_bank1`。
- frontier 字段：无写尾夹具的 normal 日志在第 21 行；写尾夹具日志在第 22 行。
- 写 stall/payload：第 11 行 `ddr_w_stall`、`ddr_w`、`comp_w_fire`；写尾终端在第 21 行。
- `*_raw` 与 `E2`、`ACTUAL-DMA-SUMMARY` 累计量包含 warmup；只有 `*_window` 是 `(open,close]` 窗口内。

## 2. 紧凑结果表

表内 `log:line` 省略目录前缀 `out/payload-frontier-poc-20261007/`。

| 运行 | mode | W_cycles 出处 | W_cycles | V/A窗口 | FAST窗口；累计summary | frontier grant raw/window | frontier wait raw/window | ddr_w_stall | ddr_w / comp_w_fire | WRITE-TAIL pause/holds/hold_cycles |
|---|---|---:|---:|---:|---|---:|---:|---:|---:|---:|
| strict-normal-pn（早期校准） | pn | `strict-normal-pn.log:11` | 92374 | 64/64 | 62(62/0)；327(326/1) | 0/0 | 0/0 | 0 | 65536/65536 | n/a，无写尾终端 |
| frontier-normal-pn（早期校准） | pn | `frontier-normal-pn.log:11` | 92374 | 64/64 | 62(62/0)；327(326/1) | 0/0 | 0/0 | 0 | 65536/65536 | n/a，无写尾终端 |
| strict-tail-pn（最后模型） | pn | `strict-tail-pn.log:11` | 102686 | 64/64 | 62(62/0)；327(326/1) | 0/0 | 33725/6816 | 16384 | 65536/65536 | 64/1284/82176 |
| frontier-tail-pn（最后模型） | pn | `frontier-tail-pn.log:11` | 96267 | 64/64 | 62(62/0)；327(326/1) | 475/96 | 475/96 | 16384 | 65536/65536 | 64/1284/82176 |
| ordinary-virtual-pn（最后模型） | pn | `ordinary-virtual-pn.log:11` | 96267 | 64/64 | 62(62/0)；327(326/1) | 475/96 | 475/96 | 16384 | 65536/65536 | 64/1284/82176 |
| strict-tail-p（最后模型） | p | `strict-tail-p.log:11` | 101731 | 64/64 | 0(0/0)；0 | 0/0 | 33501/6592 | 16384 | 65536/65536 | 64/1284/82176 |
| frontier-tail-p（最后模型） | p | `frontier-tail-p.log:11` | 95692 | 64/64 | 0(0/0)；0 | 475/96 | 475/96 | 16384 | 65536/65536 | 64/1284/82176 |
| ordinary-virtual-p（最后模型） | p | `ordinary-virtual-p.log:11` | 95692 | 64/64 | 0(0/0)；0 | 475/96 | 475/96 | 16384 | 65536/65536 | 64/1284/82176 |

补充：
- 6 条最后模型写尾夹具的 `ddr_w_stall=16384`，且 `16384 = 256 × 64`；`hold_cycles=82176 = 1284 × 64`，`pause_cycles=64`。出处：各 tail 日志第 11、21 行。
- 2 条早期校准无 `WRITE-TAIL-TERMINAL-JSON`，其 `ddr_w_stall=0`，`AW-FIXTURE-TERMINAL-JSON stall_cycles=0`、`AR-FIXTURE-TERMINAL-JSON stall_cycles=0`。出处：`strict-normal-pn.log:11,17,18`；`frontier-normal-pn.log:11,17,18`。
- 所给内容中，`strict-normal-pn.log` 与 `frontier-normal-pn.log` 关键字段相同；`ordinary-virtual-pn.log` 与 `frontier-tail-pn.log` 关键字段相同；`ordinary-virtual-p.log` 与 `frontier-tail-p.log` 关键字段相同。

## 3. 精确计算

### 3.1 strict -> full-prefix，PN

- strict-tail-pn：`W_cycles=102686`，出处 `out/payload-frontier-poc-20261007/strict-tail-pn.log:11`。
- frontier-tail-pn：`W_cycles=96267`，出处 `out/payload-frontier-poc-20261007/frontier-tail-pn.log:11`。
- `ΔW = 102686 - 96267 = 6419 cycles`。
- W 降幅：`6419 / 102686 = 0.06251096 ≈ 6.2511%`。
- 若以 W 比吞吐：`102686 / 96267 = 1.066679 ≈ +6.6679%`。
- frontier 窗口等待：`wait_window 6816 -> 96`；`grant_window 0 -> 96`；`wait_raw 33725 -> 475`；`grant_raw 0 -> 475`。出处：`strict-tail-pn.log:22`、`frontier-tail-pn.log:22`。
- 窗口 V/A 均为 64/64，场景 `v_transfers=64, a_pages=8, v_gap=0`。出处：两日志第 11、2、7 行。

### 3.2 strict -> full-prefix，P

- strict-tail-p：`W_cycles=101731`，出处 `out/payload-frontier-poc-20261007/strict-tail-p.log:11`。
- frontier-tail-p：`W_cycles=95692`，出处 `out/payload-frontier-poc-20261007/frontier-tail-p.log:11`。
- `ΔW = 101731 - 95692 = 6039 cycles`。
- W 降幅：`6039 / 101731 = 0.0593624 ≈ 5.9362%`。
- 若以 W 比吞吐：`101731 / 95692 = 1.0631087 ≈ +6.3109%`。
- frontier 窗口等待：`wait_window 6592 -> 96`；`grant_window 0 -> 96`；`wait_raw 33501 -> 475`；`grant_raw 0 -> 475`。出处：`strict-tail-p.log:22`、`frontier-tail-p.log:22`。
- 窗口 V/A 均为 64/64，场景 `v_transfers=64, a_pages=8, v_gap=0`。出处：两日志第 11、2、7 行。

### 3.3 最强普通 P/PN 比较

这里把 `ordinary-virtual-pn` 与 `ordinary-virtual-p` 作为普通虚拟路径 P/PN 比较；它们分别与对应 full-prefix 的 `frontier-tail-pn`、`frontier-tail-p` 关键字段相同。

- ordinary-virtual-pn：`W_cycles=96267`，出处 `out/payload-frontier-poc-20261007/ordinary-virtual-pn.log:11`。
- ordinary-virtual-p：`W_cycles=95692`，出处 `out/payload-frontier-poc-20261007/ordinary-virtual-p.log:11`。
- `ΔW = 96267 - 95692 = 575 cycles`，PN 比 P 多 575 cycles。
- PN 相对 P 的 W 比值：`96267 / 95692 = 1.0060088 ≈ 1.0060`。
- 即在本矩阵内，P 比 PN 约高 `0.6009%` 的 W 比吞吐；PN 的 W 比 P 约长 `0.6009%`。
- PN 窗口 `FAST=62 (62/0)`，累计 `fast=327 (326/1)`；P 窗口 `FAST=0`，累计 `fast=0`。出处：`ordinary-virtual-pn.log:11,15`；`ordinary-virtual-p.log:11,15`。
- PN 窗口 `iotlb_lookup=453, iotlb_hit=421, iotlb_miss=32`，P 窗口 `iotlb_lookup=515, iotlb_hit=483, iotlb_miss=32`。出处：各自第 11 行。
- 累计 summary：PN `iotlb_look=2241, iotlb_hit=2066, ptw_active=80252`；P `iotlb_look=2568, iotlb_hit=2391, ptw_active=99469`。出处：各自第 15 行。
- 结论仅限本矩阵：ordinary-virtual 与对应 full-prefix 结果相同，不能写成全局定理或新论文。

### 3.4 固定 V/A 可用 W 比吞吐，但动态 CPU poll 读数改变

- 所有 8 条日志的 V/A 逻辑工作量相同：`transfers=64, a_pages=8, v_gap=0`，窗口 `v_completed=64, a_completed=64`。出处：各日志第 2、7、11 行。
- 因此在同一窗口定义 `hardware_first_V_issue_to_final_V_done_(open,close]` 下，`W_other / W_base` 可作为本夹具内相对吞吐比较。
- 但动态 CPU poll 读数并不相同，不能称所有请求完全相同：
  - `cpu_issued/cpu_completed`：`284`（`strict-normal-pn.log:14`、`frontier-normal-pn.log:14`）、`381`（`strict-tail-pn.log:14`）、`349`（`frontier-tail-pn.log:14`、`ordinary-virtual-pn.log:14`）、`373`（`strict-tail-p.log:14`）、`342`（`frontier-tail-p.log:14`、`ordinary-virtual-p.log:14`）。
  - `aw_class[0]`/`aw_raw[0]`：`90711`、`91039`、`85511`、`90308`、`84938` 等，出处第 10 行。
  - 窗口 `ddr_ar_slv0_cpu`：`284`、`381`、`348`、`373`、`341` 等，出处第 11 行。
- 因此 W 比值是“固定 V/A 工作量、受控 DDR 模型夹具内”的相对吞吐读数，不是逐请求完全相同的证明。

## 4. 关键数字的日志路径:行引用

### 4.1 W_cycles

- `out/payload-frontier-poc-20261007/strict-normal-pn.log:11`：92374
- `out/payload-frontier-poc-20261007/frontier-normal-pn.log:11`：92374
- `out/payload-frontier-poc-20261007/strict-tail-pn.log:11`：102686
- `out/payload-frontier-poc-20261007/frontier-tail-pn.log:11`：96267
- `out/payload-frontier-poc-20261007/ordinary-virtual-pn.log:11`：96267
- `out/payload-frontier-poc-20261007/strict-tail-p.log:11`：101731
- `out/payload-frontier-poc-20261007/frontier-tail-p.log:11`：95692
- `out/payload-frontier-poc-20261007/ordinary-virtual-p.log:11`：95692

### 4.2 V/A 工作量

- 窗口 `v_completed=64, a_completed=64`：所有 8 条第 11 行。
- 场景 `transfers=64, a_pages=8, v_gap=0, warmup=256, ptw_latency=31, a_target=1048576`：所有 8 条第 2、7 行。
- 注意第 8 行 `WARM-SUMMARY` 的 `v_submitted=256, v_completed=256, a_submitted=257, a_completed=257` 是含 warmup 的软件模型计数；窗口内以第 11 行为准。

### 4.3 FAST

- PN 窗口：`fast=62, fast_bank0=62, fast_bank1=0`，出处所有 PN 日志第 11 行。
- PN 累计 summary：`fast=327, fast_bank0=326, fast_bank1=1`，出处所有 PN 日志第 15 行。
- P 窗口：`fast=0, fast_bank0=0, fast_bank1=0`，出处所有 P 日志第 11 行。
- P 累计 summary：`fast=0`，出处所有 P 日志第 15 行。

### 4.4 frontier grant/window wait

- normal（无写尾夹具）：
  - `strict-normal-pn.log:21`：`frontier_grant_raw=0, frontier_grant_window=0, frontier_wait_raw=0, frontier_wait_window=0`
  - `frontier-normal-pn.log:21`：同上
- tail（写尾夹具）：
  - `strict-tail-pn.log:22`：grant `0/0`，wait `33725/6816`
  - `frontier-tail-pn.log:22`：grant `475/96`，wait `475/96`
  - `ordinary-virtual-pn.log:22`：grant `475/96`，wait `475/96`
  - `strict-tail-p.log:22`：grant `0/0`，wait `33501/6592`
  - `frontier-tail-p.log:22`：grant `475/96`，wait `475/96`
  - `ordinary-virtual-p.log:22`：grant `475/96`，wait `475/96`

### 4.5 写 stall/payload

- 所有 8 条第 11 行：`ddr_w=65536`、`comp_w_fire=65536`、`ddr_aw=256`、`ddr_b=256`。
- 6 条 tail 第 11 行：`ddr_w_stall=16384`。
- 2 条 normal 第 11 行：`ddr_w_stall=0`。
- 6 条 tail 第 21 行：`WRITE-TAIL-TERMINAL-JSON {"pause_cycles":64,"holds":1284,"hold_cycles":82176}`。
- 2 条 normal 无 `WRITE-TAIL-TERMINAL-JSON`；其写夹具相关为 `AW-FIXTURE-TERMINAL-JSON stall_cycles=0`、`AR-FIXTURE-TERMINAL-JSON stall_cycles=0`（第 17、18 行）。

### 4.6 PASS 与终端排空证据

- normal 2 条：`nonblock_actual_dma_test: PASS` 在第 22 行。
- tail 6 条：`nonblock_actual_dma_test: PASS` 在第 23 行。
- 终端排空证据（8 条共有结构）：
  - `READ-ADMIT-TERMINAL-JSON`：`captures=1284, down_ar_fires=1284, buffer_empty=1, debt=0`。出处：所有日志第 13 行。
  - `READ-TAG-TERMINAL-JSON`：`queue_enable=1, fault=0, queued=0`。出处：所有日志第 16 行。
  - `AW-FIXTURE-TERMINAL-JSON`：`stall_cycles=0`；`AR-FIXTURE-TERMINAL-JSON`：`stall_cycles=0`。出处：所有日志第 17、18 行。
  - `HEADER-PARK-TERMINAL-JSON`：`parked=0, bypassed=0, full_bypass=0, highwater=0, queued=0, dispatched=0`。出处：所有日志第 19 行。
  - `E2-TERMINAL-JSON`：`occupancy_beats=0, pending_reads=0, grants_pool=0, meta_queued=0, highwater_beats=0`。出处：所有日志第 20 行。
- tail 额外写尾证据：
  - `WRITE-TAIL-TERMINAL-JSON`：`pause_cycles=64, holds=1284, hold_cycles=82176`。出处：6 条 tail 第 21 行。
  - 该终端没有显式 `pending=0` 字段；可用的排空/PASS 证据是第 23 行 PASS，加上第 13、16、19、20 行的 buffer/queue/occupancy/pending 归零。

### 4.7 `*_raw` 含 warmup

- `FRONTIER-TERMINAL-JSON` 同时给出 `*_raw` 与 `*_window`，并注明 `window_scope="pre_edge_(open,close]"`。例如 `strict-tail-pn.log:22` 的 `wait_raw=33725` 对应 `wait_window=6816`；`frontier-tail-pn.log:22` 的 `wait_raw=475` 对应 `wait_window=96`。
- `E2-TERMINAL-JSON` 明确 `meta_scope` 与 `scope` 为 `"cumulative_including_warm_not_window"`。出处：所有日志第 20 行。
- `ACTUAL-DMA-SUMMARY` 的 `cycles` 是含 warmup 的累计值，例如 `strict-tail-pn.log:15` 为 `508812`，而窗口 `W_cycles` 为 `102686`；`WARM-SUMMARY` 的 `warm_cycles` 另列，例如 `strict-tail-pn.log:8` 为 `404967`。
- 因此比较窗口行为应使用 `W_cycles` 与 `*_window`；`*_raw`、`E2`、`ACTUAL-DMA-SUMMARY` 累计量不能直接当作窗口内独立请求数。

### 4.8 `windowwait` 语义

- `frontier_wait_window` 不是“独立请求数”。按本阶段字段口径，它是窗口内每个周期对符合额外准入条件的源进行计数后求和，同一源可在多个周期贡献，因此是“周期 × 源”的累加量。
- 因此 `6816`、`6592`、`96` 等不能读作 6816/6592/96 个独立请求。出处：`strict-tail-pn.log:22`、`strict-tail-p.log:22`、`frontier-tail-pn.log:22`、`frontier-tail-p.log:22` 等。

## 5. 可直接纳入阶段记录的中文短草案

本阶段整理 8 条仿真日志：其中 `strict-normal-pn.log`、`frontier-normal-pn.log` 为无写尾夹具的早期校准，按任务给定先于夹具/ScanEnable 编译代次，不纳入最后模型对比；其余 6 条为最后模型，覆盖有限 64 cycle 写尾反压下的 strict、full-prefix、ordinary-virtual 三条路径，且 P/PN 各一条。

所有日志第 3 行注明 PTW latency 是普通 DDL/DDR 模型延迟，`NOT MIG-matched`；第 8 行注明 warm summary 是 `software-model counters, NOT a board measurement`。因此本阶段反压结论应表述为“受控 DDR 模型夹具内的仿真结果”，不是 MIG/板端已复现。

固定 V/A 工作量下，各日志窗口均为 `v_completed=64, a_completed=64`，场景 `transfers=64, a_pages=8, v_gap=0`，窗口定义为 `hardware_first_V_issue_to_final_V_done_(open,close]`。因此可用 `W_cycles` 比值做本夹具内的相对吞吐比较。PN 下 strict `W=102686` 到 full-prefix `W=96267`，减少 `6419 cycles`，W 降幅约 `6.2511%`，W 比吞吐约 `+6.6679%`；P 下 strict `W=101731` 到 full-prefix `W=95692`，减少 `6039 cycles`，W 降幅约 `5.9362%`，W 比吞吐约 `+6.3109%`。frontier 窗口等待从 strict 的 `wait_window=6816`（PN）/`6592`（P）降到 `96`，`grant_window` 从 `0` 变为 `96`。

普通虚拟路径中，`ordinary-virtual-pn` 的 `W=96267`，`ordinary-virtual-p` 的 `W=95692`；PN 比 P 多 `575 cycles`，即在本矩阵内 P 约高 `0.6009%` 的 W 比吞吐。`ordinary-virtual-pn` 与 `frontier-tail-pn`、`ordinary-virtual-p` 与 `frontier-tail-p` 的关键窗口/终端字段相同，说明“普通虚拟写队头 + prepared AW”与“扫描全前缀”在本矩阵下结果相同；该结论仅限本矩阵，不能写成全局定理或新论文。

记录时需注意：`*_raw`、`E2`、`ACTUAL-DMA-SUMMARY` 包含 warmup，只有 `*_window` 是窗口内值；`frontier_wait_window` 是每周期符合额外准入条件的源个数之和，不是独立请求数。所有 8 条日志均有 `nonblock_actual_dma_test: PASS`；6 条最后模型另有 `READ-ADMIT-TERMINAL` 的 `buffer_empty=1, debt=0`、`E2-TERMINAL` 的 `occupancy_beats=0, pending_reads=0`、`HEADER-PARK` 的 `queued=0` 以及 `WRITE-TAIL-TERMINAL` 的 `pause_cycles=64, holds=1284, hold_cycles=82176` 作为终端排空/PASS 证据。构建计数、真实源文件 hash 与关键代码审定由 Codex 后续单独保存。
