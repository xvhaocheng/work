# RESULTS — 00089 启动控制 12 例（p4m-start-control-20260920）

**证据缩写**（行号对应起草时快照）

- **R** = `results/experiments/20260920-211940-run-46nvmwgi/records.json`
- **O** = `results/experiments/20260920-211940-run-46nvmwgi/observations-unique.log`
- **J** = `results/experiments/20260920-211940-run-46nvmwgi/run.json`
- **D** = `artifacts/software/p4m-start-control-20260920/delivery/P4M_START_CONTROL.md`

范围：12 例 = 4 短（64 transfers / warmup 8）+ 8 长（4096 transfers / warmup 256，4096 B chunk），长例顺序 `B P P B B P P B`，顺序与门控见 J:34、D:11-13、O:122（`short_block complete; starting comparison block`）。日志记录 `cases_done=12 total=12`、`final_taint=0`、`collection_failed=0`、`final_exit=0`、`P4M_START_HOST_EXIT=0`（O:370-372），run 记录 `cases_completed=12`、`board_exit_code=0`、`final_taint=0`（J:24-27）。以上为采集日志/记录内容，非本报告自行执行。

## 1. 4 短试次（V 硬件窗口 = `p4m` 块）

| seq | 例 | 模式 | V 硬窗口cycles | V 硬窗口ns | V完成MiB | V硬件MiB/s | A→V ns | irq_span ns | 开窗A ID | V miss/S1 | A整轮HPM miss/S1 | data_err | 证据 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | start-s1-solo | P4M solo | 44015 | 880300 | 0.25 | 283.9940929228672 | —(has_a=0) | 270920 | — | 0/0 | —(无A) | 0 | R:93,167-168,175,183；O:26 |
| 2 | start-s2-a1-gap0 | P4M pair A1g0 | 84388 | 1687760 | 0.25 | 148.12532587571692 | 778800 | 1387240 | 63 | 0/0 | 0/0 | 0 | R:303,392-393,400；O:57 |
| 3 | start-s3-a8-gap0 | P4M pair A8g0 | 113764 | 2275280 | 0.25 | 109.87658661791076 | 164120 | 315680 | 6 | 0/0 | 100/100 | 0 | R:528,617-618,625；O:87 |
| 4 | start-s4-a8-gap1000 | P4M pair A8g1000 | 83001 | 1660020 | 0.25 | 150.6005951735521 | 107120 | 278280 | 3 | 0/0 | 62/62 | 0 | R:753,842-843,850；O:117 |

短例 A 窗口进度：s2 64/64 job（R:412-414），s4 32/32（R:862-864）；s1 无 A（R:187-190）。同配置 A8 两例 s3 与 s4 的 A整轮HPM miss 为 100/100 与 62/62，提交量 1343 与 62（R:541,766-771），仅记录，未做解读。

## 2. 8 长试次

| seq | 例 | 模式 | V 硬窗口cycles | V 硬窗口ns | V完成MiB | V硬件MiB/s | A→V ns | irq_span ns | 开窗A ID | V miss/S1 | A整轮HPM miss/S1 | data_err | 证据 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 5 | start-c1-b8 | B | 7352667 | 147053340 | 16 | 108.80405708568061 | 163760 | 316360 | — | 0/0 | 5140/5140 | 0 | R:975,992；O:130,149 |
| 6 | start-c2-p8g0 | P | 7348356 | 146967120 | 16 | 108.86788827324098 | 162960 | 315320 | 6 | 0/0 | 5140/5140 | 0 | R:1109,1198,1206；O:179 |
| 7 | start-c3-p8g0 | P | 7349736 | 146994720 | 16 | 108.84744703755347 | 163720 | 315920 | 6 | 0/0 | 5140/5140 | 0 | R:1334,1423,1431；O:209 |
| 8 | start-c4-b8 | B | 7351947 | 147038940 | 16 | 108.81471261966388 | 164400 | 316720 | — | 0/0 | 5140/5140 | 0 | R:1556,1573；O:221,240 |
| 9 | start-c5-b8 | B | 7352171 | 147043420 | 16 | 108.81139734100309 | 163960 | 316040 | — | 0/0 | 5140/5140 | 0 | R:1687,1704；O:252,271 |
| 10 | start-c6-p8g0 | P | 7352138 | 147042760 | 16 | 108.81188573990315 | 165280 | 317040 | 6 | 0/0 | 5140/5140 | 0 | R:1821,1910,1918；O:301 |
| 11 | start-c7-p8g0 | P | 7346334 | 146926680 | 16 | 108.89785299715477 | 165000 | 316800 | 6 | 0/0 | 5140/5140 | 0 | R:2046,2135,2143；O:331 |
| 12 | start-c8-b8 | B | 7347151 | 146943020 | 16 | 108.88574360320075 | 163840 | 315880 | — | 0/0 | 5140/5140 | 0 | R:2268,2285；O:343,362 |

列口径：**本表只以硬件 cycles 为准**（P 取 `p4m.cycles`，如 R:1198；B 取 `idma_batch_obs part=time` 的 cycles，如 O:130；两种记录类型不同）。`V完成MiB` 为 `victim.completed_bytes`（短 262144 / 长 16777216，R:92、R:974）。`V硬件MiB/s` = `V完成MiB / (硬窗口ns/1e9)`（20 ns/cycle 固定假设下）。`A整轮HPM` 指 `result.aggressor` 的整轮统计，非窗口内统计。

## 3. 4 对 P/B 比值（长例，仅硬件 cycles）

| 对 | B | P | P/B（硬件cycles） | 差（P−B, cycles） |
|---|---|---|---|---|
| p1 | c1 (seq5) | c2 (seq6) | 0.9994136821373796 | −4311 |
| p2 | c4 (seq8) | c3 (seq7) | 0.9996992633379974 | −2211 |
| p3 | c5 (seq9) | c6 (seq10) | 0.9999955115298597 | −33 |
| p4 | c8 (seq12) | c7 (seq11) | 0.9998888004343452 | −817 |

- **范围**：0.9994136821373796–0.9999955115298597；中位 ≈0.9997940318861713。
- **绝对差**：−33 … −4311 cycles（P 相对 B）。20 ns/cycle 固定假设下约 0.66 µs – 86.22 µs，属描述量。n=4，仅作描述，不做等价性或显著性判定，不设阈值（D:47）。

**条件摘要（长例硬件 cycles）**
- B cycles：min 7347151 / median 7352059 / max 7352667。
- P cycles：min 7346334 / median 7349046 / max 7352138。
- 每试次 V 硬件速率中位：B 108.81305498033348 MiB/s；P 108.85766765539722 MiB/s。

## 4. 启动跨度

- **8 长（独立统计）**：`a_to_v_ns` 162960–165280 ns（跨度 2320 ns，O:179/209/240/271/301/331/362，最小 c2、最大 c6）；`irq_span_ns` 315320–317040 ns（跨度 1720 ns，同上）。分布紧密，但无阈值结论。
- **短（不含 A1 离群）**：`a_to_v_ns` 107120（s4，O:117）与 164120（s3，O:87）；`irq_span_ns` 278280（s4）与 315680（s3）。
- **短 A1 离群**：s2 `a_to_v_ns=778800`、`irq_span_ns=1387240`（O:57），约为 s3 的 4.7×/4.4×；该例 dmesg 内含 `hrtimer: interrupt took 13155120 ns`（O:37）。仅记录该并存现象，不作 IRQ 因果或根因结论。

## 5. 域边界与语义限制

- **硬件开窗在飞 ID ≠ 启动软件 a_id**：4 个长 P 例的开窗在飞 ID 全为 6（R:1206/1431/1918/2143），短例为 63/6/3（R:400/625/850）；启动软件 `a_id` 为长 285→2084 递增（O:152/182/212/243/274/304/334/365）、短 10/19/28（O:29/60/90/120）。两者属不同域，**不做减法或对齐比较**。
- **HPM/软件括号 ≠ 硬件窗口**：`victim_elapsed_ns`、`snapshot_before/after_ns` 为软件括号且非原子（R:1006,1011），与 `p4m.cycles`/`hardware_window_ns` 不可互为替代（D:29-30）。窗口定义为 `(open,close] cycles×20 ns`，50 MHz 固定假设，`calibration=unverified`（R:192-193,197,207）。本报告不使用 `active_cycles` 作为窗口量。
- **B（legacy batch，`idma.batch.v1`）无 p4m 块**：无硬件 A 开窗 ID、无 A 窗口进度，A 侧仅有整轮 `result.aggressor`（R:987-1002；semantics R:1008-1010）；其重叠需看匹配 `idma_batch_obs` 的 AXI 记录（O:131 等）。
- 全部 12 例：V `iotlb_miss=0`、`s1_ptw=0`、`data_errors=0`、`quarantined=0`；P4M 例 `observer_status=4`、`closed=true`、`conservation=true`、`task_error_mask=0`（如 R:179,184-186,410-411,1210,1217）。`strict_preopening_inflight` 仅 P4M 例适用：Solo（seq1，R:185）为 **false**；含 A 的 7 个 P4M 例（seq2/3/4 R:410/635/860；seq6/7/10/11 R:1216/1441/1928/2153）为 **true**。B 例（batch schema）无该字段。

## 6. 候选参数（建议，交由 Main Codex / 用户决定，不展开）

- `a-gap-cycles` 候选：**0 / 1000 / 2000**；每条件重复次数需用户决定，本报告不预设、不冻结顺序。沿用 4096 transfers / 256 warmup / chunk 4096（R:206,881）。
- 本次仅 gap1000 作为短试工程检查（R:205；O:92-121），gap2000 尚未出现于本轮记录。

## 7. 缺口与不确定性

- `bit.sha256=null`、`hash_verified=false`（J:7-8）；`software_sha256=null`（J:17）；`smoke_passed`/`linux_boot_passed`/`hpm_calibrated` 均为 null（J:19-21）；`status="12_cases_completed_collecting"` 为**采集期元数据快照**（J:22）。
- 原始 UART 与板上完整目录已归档，**非缺失**；本轮无需额外主机测试或 hash 复核。
- `observed_kernel` 与 `expected_kernel` 同为 `6.5.0-rc2-00089-gefd9461d6459`（J:23,29）；`expected_payload_sha256=fc8e8a73…`（J:30）未在本证据集内复核。
- 本报告由 **DeepSeek 起草，Codex 执行并复核**；报告中的新数值（硬件 cycles、V 硬件速率、条件摘要）以 Codex 复核为准，起草方未自行执行任何命令或测试。

## 8. 归档与执行补充

- 本轮仅执行一次 `platform-p4m-start all /tmp/p4m-start-01`；没有重跑或追加正式实验，全部参与 DMA 的 data_errors/quarantined 为0。
- 完整串口为 `console.raw.log`；`board-evidence.tar.gz.b64` 是板上 `/tmp/p4m-start-01` 全目录的 tar 归档，经 Mac 压缩并作 base64 文本保存，含104个文件（12份原始JSON、stderr、每例before/after dmesg、startup/batch_time及运行参数）。可用 `base64 -D -i board-evidence.tar.gz.b64 | tar -tzf -` 查看目录，提取时用独立新目录。
- 板上 BusyBox tar 不支持 `-z`，首次归档命令退出1且未生成压缩包；随后只读打包为普通tar并成功取回（exit0）。这是采集兼容性问题，不是DMA试次失败；两次归档操作原始记录均保留，没有重复实验。
- BIT加载前串口乱码也原样保留。重新加载明确复用的P4M v7 BIT后正常启动00089；其JTAG状态和输出另附。
- 下载哈希及SD回读按用户偏好未做；本轮保留交付runner内置检查，没有新增独立主机验收或HPM校准。短A1的hrtimer警告不隐瞒、不据此推断中断根因。
