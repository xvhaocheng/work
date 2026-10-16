# P4M pilot-21 结果（7 条件 × 3 block，4096 transfers，warmup 256）

## 0. 结论摘要

- 本轮为 **21 trial = 7 条件 × 3 个交错 block**，所有 trial 均为 `--chunk 4096 --transfers 4096 --warmup 256 --timeout-ms 60000`，V=`/dev/idma0`，A=DMA1（A1 为 1 页，A8 为 8 页），平台固定 50 MHz 假设（`records.json:192-193` 等）。
- 所有 21 条 builtin 记录 `success=true`、`error=null`、`errno=0`、`result.status=0`；P4M 运行 `observer_status=4`。未见内置失败。
- B8 为 legacy batch，**没有 P4M JSON bank**；其硬件窗口周期必须取 `observations-unique.log` 中 `idma_batch_obs seq=7/17/23 part=time` 的 `cycles`（`observations-unique.log:140,401,558`），不能用软件窗口时间或 `victim_elapsed_ns`。
- 条件中位数（16 MiB 除以硬件窗口 derived）：S `283.508 MiB/s`、A1g0 `149.346`、B8 `103.284`、A8g0 `103.243`、A8g500 `98.075`、A8g1000 `145.379`、A8g2000 `171.394`。
- A 窗口完成 jobs 随 gap 增大下降：A8g0 `4096`、A8g500 `4095`、A8g1000 `2048`、A8g2000 `1365`（中位数）。A 整轮 miss/S1PTW 也大致随 gap 增大下降，但 A8g500 的 miss 高于 A8g0，且 V 更慢，呈现非单调。
- A8g0 第三次样本 `seq24`/`t20` 明显不同：V cycles `7248503`（前两次约 `7.75M`），derived `110.368 MiB/s`，victim `iotlb_miss=2/s1_ptw=2`，A 整轮 miss `5141`，A submitted `5998`。按任务要求 **保留、不丢弃、不重跑、不无证据诊断原因**。
- 元数据以当前 run.json 和 upload-receipt.json 为准；DeepSeek 收到的是运行中快照，Codex 已更新为21次完成并回传。下载哈希/SD回读未核验；本轮未做独立主机验收或新HPM校准。

---

## 1. 数据来源与口径

### 1.1 运行与证据

- 运行目录：`results/experiments/20260920-202441-run-9_mfua8v/`
- builtin 记录：`records.json`（21 条）
- UART 去重证据：`observations-unique.log`（含历史短测 `seq1..4`；本 pilot 为 `seq5..25`）
- runner 与冻结矩阵：`scripts/linux_p4m_pilot21.sh:22-24,55-57,138-150`
- 运行元数据：`run.json`
- 基线语义说明：`artifacts/software/p4m-baseline-20260920/delivery/P4M_BASELINE.md`

内核与 BIT 观测：
- `observations-unique.log:1-2` 显示 `Linux 6.5.0-rc2-00088-g9f8da9e7a6a0`，与预期 kernel 一致。
- `run.json` 记录 `observed_linux_version=6.5.0-rc2-00088-g9f8da9e7a6a0`。
- `P4M_BASELINE.md:3` 说明配套 observer v7 / Linux `00088`。
- 元数据以当前 run.json 和 upload-receipt.json 为准；DeepSeek 收到的是运行中快照，Codex 已更新为21次完成并回传。下载哈希/SD回读未核验；本轮未做独立主机验收或新HPM校准。

### 1.2 窗口区分（关键，不能混用）

必须区分以下不同窗口：

1. **P4M 硬件窗口 `(open, close]`**
   - P4M 运行取 `p4m.cycles`。
   - 语义见 `P4M_BASELINE.md:18-22`：开边界事件不计入 issue/done 汇总，关边界事件计入汇总；V issued/done 包含首个被测任务；A done 可以为 0 仍为合法窗口。
   - P4M bank 字段 448..463 的解释见 `P4M_BASELINE.md:29-48`。
2. **B8 legacy batch 硬件窗口**
   - B8 无 P4M bank；取 `idma_batch_obs ... part=time ... cycles=...`。
   - 不能用 `result.victim_elapsed_ns` 或软件时间替代；batch semantics 明确 `victim_elapsed_ns` 包含 A/V launch 与 final polling（`records.json:555-562` 等）。
3. **软件/端到端窗口**
   - `victim_elapsed_ns`、`drain_ns`、`done_to_read` 属于软件窗口、launch、final polling、poll tail，不是同一个 P4M 硬件窗口。
4. **HPM 软件快照窗口**
   - `P4M_BASELINE.md:59`：HPM 软件快照窗口单独标记，不能直接当成 P4M 硬件窗口。
   - batch semantics 也写明 `counter_window` 是 HPM brackets software window and is not atomic（如 `records.json:700-701` 等）。
5. **A 整轮统计 vs A 窗口统计**
   - A 整轮：`result.aggressor`，包含启动与最终 drain，不能当作 V 窗口吞吐（`P4M_BASELINE.md:58`；`records.json:558-559` 等）。
   - A 窗口：`p4m.a_completed_jobs_window`、`p4m.a_progress_window_bytes`、`p4m.a_completed_bytes_per_second`，仅表示硬件窗口内完成的 A copy 数乘 chunk，不是 AXI/总线字节数（`P4M_BASELINE.md:56-57`）。

### 1.3 指标定义

- `V cycles`：P4M 行取 `p4m.cycles`；B8 行取 `idma_batch_obs part=time cycles`。
- `V hardware ms = cycles * 20 ns = cycles / 50000 ms`。
- `V derived MiB/s = 16 MiB / (cycles * 20 ns) = 800000000 / cycles MiB/s`。
- `A 窗口 jobs`：`p4m.a_completed_jobs_window`。
- `A 窗口速率 MiB/s`：`p4m.a_completed_bytes_per_second / 1048576`。
- `A 整轮 miss/S1PTW`：`result.aggressor.iotlb_miss` / `result.aggressor.s1_ptw`；与 A 窗口完全分离。
- `V miss/S1PTW`：`result.victim.iotlb_miss` / `result.victim.s1_ptw`；与 A 整轮、A 窗口都分离。
- `done_to_read`：来自 `idma_batch_obs` 的 end-to-end poll tail 字段，不是硬件窗口。

Trial 与 observer sequence 映射已由 `trial_begin` 标记和 P4M `sequence` 字段核对一致：
- t01..t07 → P4M/observer seq 5..11（B8 为 seq7）
- t08..t14 → seq 12..18（B8 为 seq17）
- t15..t21 → seq 19..25（B8 为 seq23）

---

## 2. 7 条件 n=3 汇总表

说明：
- `V cycles`、`V hardware ms`、`V derived MiB/s` 均为各条件 3 个样本的中位数，方括号为 `[min,max]`。
- B8 无 P4M bank，因此 A window 列为 `N/A`；其 A 整列为 legacy batch 的 `result.aggressor` 整轮统计。
- S 无 A，`a_completed_jobs_window=0` 合法，`has_a=false`；不视为失败。
- 所有 P4M 行的 `task_error_mask=0`、`conservation=true`、`closed=true`、`observer_status=4`。
- P4M flags：bit0=`hasA`，bit1=`closed`，bit2=`strict pre-opening initial carry`（`P4M_BASELINE.md:48`）。

| 条件 | V cycles 中位数 [min,max] | V hardware ms 中位数 | V derived MiB/s 中位数 | A 窗口完成 jobs / 速率 MiB/s 中位数 | A 整轮 miss/S1PTW 中位数 [range] | V miss/S1PTW 中位数 [range] | P4M flags / 备注 |
|---|---:|---:|---:|---:|---:|---:|---|
| S | 2821786 [2821580, 2822099] | 56.436 | 283.508 | 0 / 0 | N/A（无 A） | 0 / 0 [0,0] | flags=2（无 hasA，closed） |
| A1g0 | 5356688 [5346832, 5368703] | 107.134 | 149.346 | 4096 / 149.346 | 0 / 0 [0,0] | 0 / 0 [0,0] | flags=7（hasA+closed+strict） |
| B8 | 7745642 [7744503, 7745970] | 154.913 | 103.284 | N/A（无 P4M bank） | 6160 / 6160 [6160,6160] | 0 / 0 [0,0] | 无 P4M flags；batch status=0 |
| A8g0 | 7748692 [7248503, 7751680] | 154.974 | 103.243 | 4096 / 103.243 | 6160 / 6160 [5141,6160] | 0 / 0 [0,2] | flags=7；第 3 样本 seq24 明显不同 |
| A8g500 | 8157023 [8156880, 8157735] | 163.140 | 98.075 | 4095 / 98.051 | 7184 / 7184 [7184,7184] | 0 / 0 [0,0] | flags=3（hasA+closed，无 strict） |
| A8g1000 | 5502872 [5497730, 5506360] | 110.057 | 145.379 | 2048 / 72.689 | 3600 / 3600 [3592,3600] | 0 / 0 [0,0] | flags=7 |
| A8g2000 | 4667603 [4667331, 4668975] | 93.352 | 171.394 | 1365 / 57.101 | 2406 / 2406 [2402,2406] | 0 / 0 [0,0] | flags=3 |

补充说明：
- A8g500 的 A 窗口速率为 `a_completed_bytes_per_second / 1048576`，因为窗口内完成 jobs=4095，对应字节数为 `4095*4096`，不是 16 MiB 全量；不要把它和 V derived（按 16 MiB 算）混为一谈。
- A8g1000 的 A 窗口 jobs=2048，A 窗口速率约为 V derived 的一半，符合窗口内 A 进度减少。
- A8g2000 的 A 窗口 jobs 中位数 1365（三次为 1365、1365、1364）。
- A8g0 的 V miss/S1PTW range 为 `[0,2]`，因为第三次样本 seq24 为 2/2；不能默认所有 V miss/PTW 都是 0。

---

## 3. 21 trial 全表

说明：
- `cycles`：P4M 行取 `p4m.cycles`；B8 行取 `idma_batch_obs part=time cycles`。
- `Vrate`：`800000000 / cycles MiB/s`。
- `A 窗口 jobs / 速率`：仅 P4M 行有；B8 为 `N/A`；S 为 `0 / 0`。
- `A 整轮 miss/S1PTW`：`result.aggressor`；S 无 A，为 `N/A`；B8 有整轮 A 统计。
- `V miss/S1PTW`：`result.victim`。
- `done_to_read`：来自 `observations-unique.log` 对应 `idma_batch_obs` 行。
- `builtin`：P4M 行给出 `status=0; flags=...`；B8 行给出 `status=0; 无P4M bank`。所有行 `success=true`、`error=null`、`errno=0`。

| # | block | condition | observer seq | cycles | Vrate MiB/s | A 窗口 jobs / 速率 MiB/s | A 整轮 miss/S1PTW | V miss/S1PTW | done_to_read | builtin |
|---:|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| t01 | B1 | S | 5 | 2822099 | 283.477 | 0 / 0 | N/A（无 A） | 0/0 | 675 | status=0; flags=2 |
| t02 | B1 | A1g0 | 6 | 5368703 | 149.012 | 4096 / 149.012 | 0/0 | 0/0 | 1365 | status=0; flags=7 |
| t03 | B1 | B8 | 7 | 7745642 | 103.284 | N/A | 6160/6160 | 0/0 | 952443 | status=0; 无P4M bank |
| t04 | B1 | A8g0 | 8 | 7751680 | 103.203 | 4096 / 103.203 | 6160/6160 | 0/0 | 932416 | status=0; flags=7 |
| t05 | B1 | A8g500 | 9 | 8157735 | 98.066 | 4095 / 98.042 | 7184/7184 | 0/0 | 28108 | status=0; flags=3 |
| t06 | B1 | A8g1000 | 10 | 5497730 | 145.515 | 2048 / 72.757 | 3592/3592 | 0/0 | 3980 | status=0; flags=7 |
| t07 | B1 | A8g2000 | 11 | 4667603 | 171.394 | 1365 / 57.117 | 2406/2406 | 0/0 | 45162 | status=0; flags=3 |
| t08 | B2 | S | 12 | 2821580 | 283.529 | 0 / 0 | N/A（无 A） | 0/0 | 1285 | status=0; flags=2 |
| t09 | B2 | A8g2000 | 13 | 4668975 | 171.344 | 1365 / 57.101 | 2406/2406 | 0/0 | 31733 | status=0; flags=3 |
| t10 | B2 | A8g1000 | 14 | 5502872 | 145.379 | 2048 / 72.689 | 3600/3600 | 0/0 | 14075 | status=0; flags=7 |
| t11 | B2 | A8g500 | 15 | 8156880 | 98.077 | 4095 / 98.053 | 7184/7184 | 0/0 | 67253 | status=0; flags=3 |
| t12 | B2 | A8g0 | 16 | 7748692 | 103.243 | 4096 / 103.243 | 6160/6160 | 0/0 | 26767 | status=0; flags=7 |
| t13 | B2 | B8 | 17 | 7744503 | 103.299 | N/A | 6160/6160 | 0/0 | 960499 | status=0; 无P4M bank |
| t14 | B2 | A1g0 | 18 | 5356688 | 149.346 | 4096 / 149.346 | 0/0 | 0/0 | 6203 | status=0; flags=7 |
| t15 | B3 | S | 19 | 2821786 | 283.508 | 0 / 0 | N/A（无 A） | 0/0 | 2949 | status=0; flags=2 |
| t16 | B3 | A8g1000 | 20 | 5506360 | 145.287 | 2048 / 72.643 | 3600/3600 | 0/0 | 31968 | status=0; flags=7 |
| t17 | B3 | A8g500 | 21 | 8157023 | 98.075 | 4095 / 98.051 | 7184/7184 | 0/0 | 66005 | status=0; flags=3 |
| t18 | B3 | A8g2000 | 22 | 4667331 | 171.404 | 1364 / 57.079 | 2402/2402 | 0/0 | 18302 | status=0; flags=3 |
| t19 | B3 | B8 | 23 | 7745970 | 103.280 | N/A | 6160/6160 | 0/0 | 9503 | status=0; 无P4M bank |
| t20 | B3 | A8g0 | 24 | 7248503 | 110.368 | 4096 / 110.368 | 5141/5141 | 2/2 | 295647 | status=0; flags=7 |
| t21 | B3 | A1g0 | 25 | 5346832 | 149.621 | 4096 / 149.621 | 0/0 | 0/0 | 32325 | status=0; flags=7 |

关键逐行证据引用：
- builtin 主记录：`records.json:1-4394`。
- observer seq 与 done_to_read：`observations-unique.log:86,113,140,166,192,218,244,270,296,322,348,374,401,427,453,479,505,531,558,584,610`。
- T20 A8g0 r3 异常字段：victim `iotlb_miss=2`、`s1_ptw=2`（`records.json:4038-4040`），A 整轮 `iotlb_miss=5141`、`s1_ptw=5141`、`submitted=5998`（`records.json:4050-4055`），cycles `7248503`（`records.json:4126`），done_to_read `295647`（`observations-unique.log:584`）。
- t10 drain 异常：`drain_ns=739480`（`records.json:1987`），明显高于其它有 A trial 的约 `72–75k` 量级；done_to_read `14075`（`observations-unique.log:322`）。

---

## 4. 配对 A8g0 vs legacy B8（同 block，硬件窗口 cycles 比值）

按任务要求，必须同 block 配对，不能把所有 A8g0 与所有 B8 混在一起算平均。

| block | B8 trial / seq | B8 cycles | A8g0 trial / seq | A8g0 cycles | 比值 A8g0/B8 |
|---|---|---:|---|---:|---:|
| B1 | t03 / seq7 | 7745642 | t04 / seq8 | 7751680 | 1.00078 |
| B2 | t13 / seq17 | 7744503 | t12 / seq16 | 7748692 | 1.00054 |
| B3 | t19 / seq23 | 7745970 | t20 / seq24 | 7248503 | 0.93578 |

- 三个配对比值中位数：`1.00054`。
- 范围：`[0.93578, 1.00078]`。
- 解释注意：B1/B2 中 A8g0 与 B8 的硬件窗口 cycles 非常接近（比值约 `1.0005–1.0008`）；B3 的 A8g0 为异常样本，导致比值降至 `0.93578`。该异常不能通过“丢弃/重跑”消除，也不能仅凭 n=3 诊断原因。

---

## 5. 条件变异与 gap tradeoff 观察

### 5.1 条件间变异

- S（无 A）最快：V cycles 中位数 `2821786`，derived `283.508 MiB/s`；A 窗口进度为 0，合法。
- A1g0：V cycles 中位数 `5356688`，derived `149.346 MiB/s`；相对 S 约慢 `1.90x`。A 整轮 miss/S1PTW 为 `0/0`，A 窗口 jobs 全部 `4096`。
- A8g0：V cycles 中位数 `7748692`，derived `103.243 MiB/s`；相对 S 约慢 `2.75x`。A 整轮 miss/S1PTW 中位数 `6160/6160`，A 窗口 jobs `4096`（但第三次样本 seq24 异常，使范围变宽）。
- A8g500：V cycles 中位数 `8157023`，derived `98.075 MiB/s`；在三个 block 中均比同 block A8g0 更慢（cycles 更大），且 A 整轮 miss/S1PTW `7184/7184` 高于 A8g0 的 `6160/6160`；A 窗口 jobs `4095`。
- A8g1000：V cycles 中位数 `5502872`，derived `145.379 MiB/s`；明显快于 A8g0/g500，接近 A1g0。A 窗口 jobs `2048`，A 整轮 miss/S1PTW `3600/3600`。
- A8g2000：V cycles 中位数 `4667603`，derived `171.394 MiB/s`；除 S 外最快。A 窗口 jobs 中位数 `1365`，A 整轮 miss/S1PTW `2406/2406`。
- B8（legacy batch）：硬件窗口 cycles 中位数 `7745642`，derived `103.284 MiB/s`；与 B1/B2 的 A8g0 很接近，但 B8 无 P4M bank，A 窗口指标不可得。

### 5.2 gap tradeoff

从 gap0 → gap500 → gap1000 → gap2000 看：

- V 变快方向（cycles 降低）：A8g0 `~7.75M` → A8g1000 `~5.50M` → A8g2000 `~4.67M`。
- 但 A 窗口完成 jobs 下降：`4096` → `2048` → `1365`。
- A 整轮 miss/S1PTW 中位数也下降：`6160/6160` → `3600/3600` → `2406/2406`。
- A8g500 不完全遵循“gap 越大 V 越快”的简单单调：其 V cycles 中位数 `8157023`，高于 A8g0 的 `7748692`；A 整轮 miss/S1PTW `7184/7184` 高于 A8g0。因此本轮观察更像“gap500 可能恶化 V，并伴随更高 A 整轮 miss”，但 n=3 不能做因果结论。
- 所有这些都是描述性观察；没有预注册阈值，不做正式显著性/等效性检验，不把 n=3 的差异写成正式结论。

### 5.3 边界与窗口截断

- A8g500：`a_issue_window=4096`，`a_done_window=4095`，`a_completed_jobs_window=4095`，`a_started_before_window_completed=0`，flags=3（无 strict carry）。见 `records.json:975-994, 2308-2327, 3547-3566` 等。
- A8g1000：`a_issue_window=2048`，`a_done_window=2048`，`a_completed_jobs_window=2048`，`a_started_before_window_completed=1`，flags=7。见 `records.json:1200-1219, 2083-2102, 3322-3341` 等。
- A8g2000：部分样本 `a_completed_jobs_window=1365`，第三次为 `1364`，flags=3。见 `records.json:1443-1445, 1876-1878, 3790-3792`。
- 这些边界截断意味着 A 窗口 jobs 不是“应完成总量”，而是硬件窗口内完成量；不能用 A 窗口 jobs 反推 A 整轮提交/完成。

---

## 6. 异常、内置状态与证据缺口

### 6.1 内置状态

- 所有 21 条 builtin 记录：`success=true`、`error=null`、`errno=0`、`result.status=0`。
- 所有 P4M 行：`observer_status=4`、`task_error_mask=0`、`conservation=true`、`closed=true`。
  - 例：S flags=2（`records.json:182,1646,3110`）；A8g500 flags=3（`records.json:988,2321,3560`）；A8g1000/A8g0/A1g0 flags=7（如 `records.json:407,763,1213` 等）。
- B8 三条：`schema=idma.batch.v1`，无 `p4m` 字段；builtin `status=0`。
  - `records.json:435-564`（B8 r1）、`records.json:2574-2703`（B8 r2）、`records.json:3813-3942`（B8 r3）。
  - B8 硬件 cycles：`observations-unique.log:140,401,558`。

未见内置失败；但“内置成功”不等于独立主机验收或 BIT hash 校验完成。

### 6.2 异常样本

- **A8g0 第三次样本 t20 / seq24**：
  - V cycles `7248503`，明显低于同条件前两次 `7751680`、`7748692`；derived 升至 `110.368 MiB/s`。
  - victim `iotlb_miss=2`、`s1_ptw=2`（`records.json:4038-4040`）。
  - A 整轮 `iotlb_miss=5141`、`s1_ptw=5141`，submitted/completed `5998`（`records.json:4050-4055`）。
  - done_to_read `295647`（`observations-unique.log:584`）。
  - 按任务要求：保留为内置检查成功的原始样本（可比性仍待调查），不丢弃、不重跑、不无证据诊断原因。
- **A8g1000 第二次样本 t10 / seq14**：
  - `drain_ns=739480`（`records.json:1987`），远高于其它有 A trial 的约 `72–75k` 量级；属于软件/poll tail 异常证据，不能直接归因到 P4M 硬件窗口。
  - done_to_read `14075`（`observations-unique.log:322`），本身不异常，但与 drain 异常同时出现，需作为后续复核线索。
- **B8 r3 整轮 A submitted/completed=4142**：
  - 低于 B8 r1/r2 的 `5507/5517`；A 整轮 miss/S1PTW 仍为 `6160/6160`。见 `records.json:3915-3922`。
  - 不能仅凭该差异推断原因；需要更多样本或独立观测。
- **历史 hrtimer 行**：
  - `observations-unique.log:28` 有 `hrtimer: interrupt took 16200800 ns`，位于历史短测 `seq1..4` 区域（在 seq2 之前），不在本 pilot `seq5..25` 内。不能直接归因到 pilot 结果，但可作为平台噪声背景保留。

### 6.3 证据缺口

- `run.json`：BIT `sha256=null`、`hash_verified=false`、给出 `expected_sha256` 但未验证。
- `run.json`：`software_sha256=null`。
- `run.json`：本轮未记录新增独立 HPM 校准；不据此断言历史上从未校准。
- 元数据以当前 run.json 和 upload-receipt.json 为准；DeepSeek 收到的是运行中快照，Codex 已更新为21次完成并回传。下载哈希/SD回读未核验；本轮未做独立主机验收或新HPM校准。
- `run.json`：`independent_host_acceptance="not run per user preference; runner syntax only checked"`；因此不能声称独立主机验证已完成。
- 原始 UART 1,450,098 字节由 Codex 完整保存并回传 Dell。DeepSeek 分析使用去重日志，Codex 直接从原始 UART 提取 JSON 并检查结束标记。
- 50 MHz 与 `cycles*20ns` 是固定平台假设，不是每次板级校准结果（`P4M_BASELINE.md:55`）。
- `P4M_BASELINE.md:65` 明确 gap 点与重复数尚未冻结，由预试决定；本报告不编造正式参数。

---

## 7. 下一阶段参数建议（仅建议，不运行、不扩展）

以下只是从本轮 pilot 观察出发的参数优先级建议，不构成本轮执行动作，也不新增验收脚本或测试。

### 7.1 值得考虑进入下一阶段正式参数

1. **S / A1g0 / A8g0 自对照**
   - S 作为 solo 基线，A1g0 作为单页 A 对照，A8g0 作为 8 页 A 基线；必须使用同版本 v7 自身的 gap0 自对照（`P4M_BASELINE.md:63`）。
2. **A8 gap1000 与 gap2000**
   - 这两个点在本轮显示明显的 V/A tradeoff：V 窗口变短、A 窗口 jobs 下降；适合作为下一阶段正式 gap 点的候选。
   - 但需预注册分析方法和重复数，不能直接沿用 n=3 做正式结论。
3. **legacy B8 对照**
   - B8 可作为“无 P4M bank 的 legacy batch”对照；但其硬件窗口只能取 `idma_batch_obs part=time cycles`，且 A 窗口指标不可得。
4. **gap500**
   - 若目标是选择吞吐取舍，gap1000/2000更值得优先考虑；若目标是完整刻画响应曲线，应保留gap500这个非单调点，不能只展示有利条件。本轮三个 block 中 A8g500 的 V 均比同 block A8g0 更慢，A 整轮 miss 更高，且与 gap0/gap1000 之间非单调。
   - 若必须覆盖 gap500，需增加重复并预注册分析，不能把本轮 n=3 直接升级为正式结论。

### 7.2 Caveats

- n=3 只能做描述性比较，不做正式显著性、等效性或阈值判定。
- A8g0 seq24 异常样本必须保留；下一阶段正式分析需预先规定异常样本处理规则，不能事后无证据丢弃/重跑。
- B8 无 P4M A 窗口；不能把 B8 整轮 A 统计当成 A 窗口统计。
- `victim_elapsed_ns`、`drain_ns`、`done_to_read` 是软件/poll tail；正式分析必须继续与 P4M 硬件窗口分离。
- 按用户偏好，本轮未做下载哈希、SD回读、独立主机验收或新增HPM校准；应如实记录这些边界，不将其改写为通过，也不擅自补跑。
- 原始 UART 已保存并回传；后续调查可直接使用，不需要为获得日志而补跑。

---

## 8. 执行归档与限制

21 次运行全部 exit=0、data_errors/quarantined=0、V4096/4096、最终 taint=0，collection_failed=0；没有重跑。原始证据已回传 `/home/xvhaocheng/work/iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v`，见 upload-receipt.json。正式实验未启动。



- DeepSeek 基于 records.json、去重观测、运行中元数据、runner 和语义说明起草报告；Codex 审阅、复算关键数字、执行并回传实验。DeepSeek 本身未执行命令。
- DeepSeek 未读取原始 UART，只分析其去重副本；Codex 已保存和回传原始 UART，并复核提取记录、关键硬件窗口与结束标记。
- 未声称独立主机验证完成；`run.json` 明确其未运行。
- 未声称 BIT hash 已验证；`run.json` 显示未验证。
- 以上所有数值均为对给定文件的事后计算与近似舍入；若与原始工具输出存在末位差异，以原始 `records.json` 和 `observations-unique.log` 为准。
