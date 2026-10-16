# P4M formal60 recovery — 初步事实 / 统计表 / 恢复条件

**状态**: draft（DeepSeek 起草，Codex 核定）。只读后处理；未重跑、未删除、未重标任何原始样本。本文不含显著性、等价性或创新性结论；不声称任何独立验收。

**run**: `20260920-215406-run-cicggil4`（label `p4m-formal-gap60`）

## 0. Run metadata / provenance

- status = `stopped_runner_verification_exit5_evidence_collected`；runner_exit = 5；host_exit = 5（run.json:22-24）
- cases_done = 24；cases_attempted = 25；cases_not_run = 35（run.json:25-27）
- failed_case = `formal-b09-g2000`；failure = `verify_failed=startup_line_count before=36 after=36 want=37`（run.json:28-29）
- confirmed_stop_cause = dmesg 环形缓冲覆盖（run.json:30）
- kernel = `6.5.0-rc2-00089-gefd9461d6459`；boot_reused_from = `results/experiments/20260920-211940-run-46nvmwgi/run.json`；rebooted = false；bit_reloaded = false（run.json:36-40）
- initial_taint = 0；final_taint = 0（run.json:34,40）
- linux_after_run = `Keep running; no shutdown or reboot`（run.json:43）；`independent_host_acceptance` = not run（run.json:44）
- BIT provenance gap：run.json 的 `bit.sha256 = null`、`hash_verified = false`（run.json:7-8）。本轮未构建 BIT（沿用既有 `7af1ccbc…`），此处仅记录该未校验事实，不推断影响。
- **不自动提交**：恢复/补跑/上传均需显式授权；本文件与统计脚本均不触发任何板上动作。

## 1. 停止事实（记录，不推断根因）

- 停在 formal-b09-g2000（第 25 个）的 runner 校验：`P4M: case=formal-b09-g2000 verify_failed=startup_line_count before=36 after=36 want=37`（FAILURE_HANDOFF.md:10）。
- 证据：`formal-b09-g2000.dmesg.before` = seq 1..36（36 条）；`.dmesg.raw` = seq 2..37（36 条）；旧 seq=1 被环形缓冲覆盖、新 seq=37 存在（FAILURE_HANDOFF.md:17-20）。该校验逻辑原样继承 00089 runner（FAILURE_HANDOFF.md:21）。
- Codex 核定：脚本“条数应递增（after = before + 1）”的断言与环形缓冲覆盖冲突；属采集脚本簿记/断言缺陷，**无**指向 DMA 数据错误的证据（FAILURE_HANDOFF.md:32）。
- **警告（不得越界推断）**：`pr_warn_once` 意味着同类内核告警只打印一次；“此后无该告警”**不能**推出“此后无 IRQ 事件”，也**不**用于推因。本轮不下 IRQ 根因结论。

## 2. 主统计（只有前 24 个，平衡 8 block；三条件各 8）

转换口径：V MiB/s = `v_rate_bps / 2^20` = 16 MiB / `window_s`；A 窗口 MiB/s = `a_window_rate_bps / 2^20` = `a_window_bytes / window_s`（**不是**总线吞吐）。

### 2.1 每条件 min / median / max

| condition | n | V hw cycles | V MiB/s | A 窗口 bytes | A 窗口 MiB/s | A 整程 HPM active cycles | V 整程 miss/s1/s2 | A 整程 miss / s1 / s2 |
|---|---|---|---|---|---|---|---|---|
| gap0 | 8 | 7345648 / 7351083 / 7355699 | 108.760 / 108.83 / 108.909 | 16777216（8/8 恒定） | 108.760 / 108.83 / 108.909 | 7323033 / 8123428.5 / 8259559 | 0 / 0 / 0（8/8） | 5140 / 5140 / 0（8/8 恒定） |
| gap1000 | 8 | 5307434 / 5310310.5 / 5312316 | 150.561 / 150.64 / 150.746 | 8388608（8/8 恒定） | 75.297 / 75.33 / 75.367 | 3220671 / 3230042.5 / 3242381 | 0 / 0 / 0（8/8） | 3084 / 3086 / 3088；s2 = 0 |
| gap2000 | 8 | 4664494 / 4665959.5 / 4668331 | 171.373 / 171.46 / 171.514 | 5582848 / 5586944 / 5586944 | 57.067 / 57.09 / 57.103 | 1914376 / 1916341.5 / 1922014 | 0 / 0 / 0（8/8） | 2398 / 2401 / 2406；s2 = 0 |

注：gap2000 的 v_cycles / active 中位为 n=8 主集；`summary.json` 的 4666106 / 1916220 是含第 25 个的 n=9 值（summary.json:304,354）。三条件 V 整程 miss/s1_ptw/s2_ptw 恒为 0/0/0。仅描述，不剔除任何点。

### 2.2 同 block 配对比值（gap/gap0）

| condition | blocks | V cycles ratio min / median / max | A 窗口 rate ratio min / median / max |
|---|---|---|---|
| gap1000 | 8 | 0.7215724 / 0.7224220 / 0.7230040 | 0.6915591 / 0.6921163 / 0.6929312 |
| gap2000 | 8 | 0.6341333 / 0.6349276 / 0.6353056 | 0.5241695 / 0.5244816 / 0.5249667 |

来源：summary.json:562-607；逐 block 明细 summary.json:423-551。第 25 个无同 block gap0 基线，配对为空（summary.json:552-559）。

### 2.3 数值取舍（同 block 配对中位比，不是非配对中位数之比）

| condition | V cycles 中位比 | V 速率提升 | A 窗口速率中位比 | A 窗口速率损失 |
|---|---|---|---|---|
| gap1000 | 0.72242 | +38.4% | 0.69212 | −30.8% |
| gap2000 | 0.63493 | +57.5% | 0.52448 | −47.6% |

V 速率提升 = 1/ratio − 1；A 损失 = 1 − ratio。两侧同 block 配对、两侧都报。仅描述；无显著性/等价/创新结论。

## 3. 第 25 个（provisional，单独）

`formal-b09-g2000`：v_cycles=4668176，window_ns=93363520，V MiB/s≈171.37，a_window_bytes=5591040，A 窗口 MiB/s≈57.11，a_whole_active_cycles=1911411，A 整程 iotlb_miss=2396 / s1_ptw=2396 / s2_ptw=0，V 整程 0/0/0；flags `no_case_end, startup_missing`（trial_summary.json:2625-2627）；CLI success=true、result status=0、measurement_complete=1（records.json:5524-5626）；batch_time seq=37（trial_summary.json:2608-2619）。

- 计数口径：**24 通过 + 1 未通过**；gap2000 现有 n=9（含第 25），其中 case_end=0 者 8、有标记者 1。**不得视作 9 个通过**（RESULTS.md:38；FAILURE_HANDOFF.md:38-39）。
- 第 25 的 `startup_domain` 全为 null（trial_summary.json:2590-2606）；原始 seq=37 存在于 `.dmesg.raw`，仅缺 runner 规范化标记（FAILURE_HANDOFF.md:25）。

## 4. startup span（全部样本报告，不排除；range 条件）

| condition | a_to_v_ns min/median/max | irq_span_ns min/median/max | 备注 |
|---|---|---|---|
| gap0 | 163040 / 164260 / 165440 | 315520 / 315840 / 317240 | 8/8 |
| gap1000 | 107560 / 108840 / 141160 | 257760 / 279280 / 280680 | b08 a_to_v=141160 偏高 |
| gap2000 | 87040 / 87940 / 88760 | 231480 / 232580 / 234240 | 8/8 |

来源：trial_summary.json startup_domain（b08-g1000 见 :2289）。观察（描述，不排除）：三条件的 startup 跨度**系统性不同**（gap 越大，a_to_v / irq_span 越小），属启动相位/预窗口历史的潜在混杂。

## 5. 段 / 时间混杂（pause）

- 边界：第 25 后停止。
- **b09 跨暂停**：b09-g2000 在暂停前（第 25），b09-g0 / b09-g1000 在恢复后（第 26、27）。b09 全程 flagged，**不丢、不并入 primary**。
- 段 A（暂停前）：blocks 1–8 = 24（full）。
- 段 B（恢复后）：blocks 10–20 = 11 个 full block（33 trial）+ b09 余下 2。
- 全部 20 block 均列出/报告；段间差异（若恢复后有数据）只作描述，不作因果。

## 6. 连续性 / 异质性（仅实际存在的）

- 段 A 内 V cycles 比值与 A 速率比值极紧：gap1000 V cycles ratio CV≈0.00058、gap2000≈0.00062（summary.json:572,596）。仅 3 个 gap 水平，**未见非单调**，不外推。
- 结构性差异（描述，不排除）：
  - gap0 8/8 `strict_preopening_inflight=true`、`a_started_before_window_bytes=4096`；
  - gap2000 8/8 `strict_preopening_inflight=false`、`a_started_before_window_bytes=0`；
  - gap1000 除 b08 外为 true/4096；b08-g1000 为 false/0（trial_summary.json:2252-2273），且其 a_to_v=141160 偏高，但该 block 比值仍在带内。
- gap0 的 A 整程 active_cycles 跨度大（min 7323033 由 b07-g0 拉低，CV≈0.0385，summary.json:91-99）；**不剔除**，如实列出。

## 7. 恢复条件（尚未授权；顺序固定）

1. **先停止 & 修脚本**：将启动标记定位从“总条数 + 1”改为按新出现的 seq 定位（如认 seq=37），无需新 BIT、无需重启 Linux（FAILURE_HANDOFF.md:43）。修脚本由 fixworker 负责。
2. **离线审计**：独立复核第 25 的 qualification。合格声明必须**显式**为 “original 24 live passed + 1 offline accepted”。**不得**重跑 / 丢弃 / 重标任何原始行；不得把离线接受说成 live 通过。
3. **审计通过后**，才按冻结 PLAN 准备 35 个 ONLY（positions 26..60）：同 `00089` + observer v7；**不改 CPU/hw、不重启、不换 BIT、不新增 gap500/Solo/A1、不做新功耗标定/内核或 BIT 构建**。
4. 审计失败 / 证据缺失：**conditional — 不 resume**。

### 7.1 冻结剩余位置 26..60（原 PLAN 行，不改顺序）

| pos | trial | block | gap |
|---|---|---|---|
| 26 | formal-b09-g0 | 9 | 0 |
| 27 | formal-b09-g1000 | 9 | 1000 |
| 28 | formal-b10-g0 | 10 | 0 |
| 29 | formal-b10-g2000 | 10 | 2000 |
| 30 | formal-b10-g1000 | 10 | 1000 |
| 31 | formal-b11-g2000 | 11 | 2000 |
| 32 | formal-b11-g1000 | 11 | 1000 |
| 33 | formal-b11-g0 | 11 | 0 |
| 34 | formal-b12-g1000 | 12 | 1000 |
| 35 | formal-b12-g0 | 12 | 0 |
| 36 | formal-b12-g2000 | 12 | 2000 |
| 37 | formal-b13-g0 | 13 | 0 |
| 38 | formal-b13-g1000 | 13 | 1000 |
| 39 | formal-b13-g2000 | 13 | 2000 |
| 40 | formal-b14-g1000 | 14 | 1000 |
| 41 | formal-b14-g2000 | 14 | 2000 |
| 42 | formal-b14-g0 | 14 | 0 |
| 43 | formal-b15-g2000 | 15 | 2000 |
| 44 | formal-b15-g0 | 15 | 0 |
| 45 | formal-b15-g1000 | 15 | 1000 |
| 46 | formal-b16-g0 | 16 | 0 |
| 47 | formal-b16-g2000 | 16 | 2000 |
| 48 | formal-b16-g1000 | 16 | 1000 |
| 49 | formal-b17-g2000 | 17 | 2000 |
| 50 | formal-b17-g1000 | 17 | 1000 |
| 51 | formal-b17-g0 | 17 | 0 |
| 52 | formal-b18-g1000 | 18 | 1000 |
| 53 | formal-b18-g0 | 18 | 0 |
| 54 | formal-b18-g2000 | 18 | 2000 |
| 55 | formal-b19-g0 | 19 | 0 |
| 56 | formal-b19-g1000 | 19 | 1000 |
| 57 | formal-b19-g2000 | 19 | 2000 |
| 58 | formal-b20-g2000 | 20 | 2000 |
| 59 | formal-b20-g1000 | 20 | 1000 |
| 60 | formal-b20-g0 | 20 | 0 |

逻辑计数（若第 25 离线接受）：现有 8 + 8 + (8 live + 1 offline) → gap0 8 / gap1000 8 / gap2000 9；剩余 35 的 gap 分布 = gap0 12 / gap1000 12 / gap2000 11。**该 12/12/11 只在审计通过后成立**；未通过前不冻结“新 20”，也不声称 60 完成。

## 8. 旁路约束记录

- 板仍在运行（run.json:43）；**Dell 无板**，不向其他人发消息，不触发自动提交。
- 本轮不新增 gap500 / Solo / A1，不做新功耗标定，不构建新内核/BIT。

## 9. 证据一致性（无需重做重型分析）

- `summary.json` 与 `RESULTS.md` 对 primary 24 的每条件 min/median/max 一致：gap0 v_cycles 中位 7351083（summary.json:46 与 RESULTS.md:15）；gap1000 5310310.5（summary.json:175 与 RESULTS.md:16）；gap2000（含第 25，n=9）4666106（summary.json:304 与 RESULTS.md:17）。配对比值表亦一致（summary.json:423-551 与 RESULTS.md:53-54）。
- 因此本报告只给一致性与补充口径，不重算原始日志、不做重型后处理。

## 10. 未决 / 待 Codex 核定

- 离线审计对第 25 的 qualification 结论（通过 / 缺失证据）。
- 恢复运行时是否记录“暂停前/恢复后”边界标记（segment boundary），以支撑段 A / 段 B 的描述性对照。
- BIT `sha256` 未校验这一点如何在后续 run metadata 中收敛。
