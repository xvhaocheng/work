# fullstats21 段分析草稿（只分析当前证据，不实现、不构建、不上板）

## 1. 范围与总判定
本报告只基于 out/research-fullstats-review-20261003/evidence.json 与随附 raw JSON/UART/README/params 分析当前 fullstats21 独立段。21/21 在线通过，failed/missing/not_run 为空（evidence.json:4-35）。这只说明交付 runner 在线判据通过，不表示方法 M 有效，也不表示已做独立离线语义验收或性能分析（README.md:3-7）。

## 2. main 条件 medians/ranges
table main_condition_summary 给出 18 个 main 用例按条件合并后的 V ms、V MiB/s、Acommon MiB/s 中位数与范围。口径：V ms=window_cycles/50000（50MHz）；V MiB/s 与 Acommon MiB/s 取 evidence.json 的 v_mib_per_s 与 a_common_mib_per_s。逐 run 数值来自 evidence.json:1563-1567、2046-2052、2546-2552、3046-3052、3546-3552、4046-4052、4546-4552、5046-5052、5546-5552、6046-6052、6546-6552、7046-7052、7529-7535、8029-8035、8529-8535、9029-9035、9529-9535、10012-10018。

主要读到：
- solo：V 约 282.8-283.2 MiB/s，V ms 约 56.5 ms。
- a1g0：V/Acommon 约 149.4-149.6 MiB/s。
- a8g0：V/Acommon 约 108.8-109.0 MiB/s。
- a16g0：V/Acommon 约 100.7-101.0 MiB/s，且 b01/b02 有 2 次 V lookup miss。
- a8g1000：V 约 150.7 MiB/s，Acommon 约 75.34-75.39 MiB/s。
- a8g2000：V 约 171.3-171.4 MiB/s，Acommon 约 57.07-57.09 MiB/s。

## 3. 全量分组覆盖
main 只观察到 group0（req=4,miss=0）、group1（req=4,miss=1）、group2（req=4,miss=2）；group3/4/5 均为 0。a8g0 三轮都是 [1536,0,2559,0,0,0]；a16g0 b01/b02 是 [0,1536,2559,0,0,0]，但 b03-a16g0 是 [768,0,3327,0,0,0]，说明同条件 run 间分组覆盖也有变化。a8g1000 三轮 [511,0,1536,0,0,0]；a8g2000 b01/b02 [170,0,1194,0,0,0]，b03 [170,0,1195,0,0,0]。a1g0 三轮 [4095,0,0,0,0,0]。可见 table main_group_counts_req_miss。

比例上：a8g0 的 g0 占 37.51%、g2 占 62.49%，总 miss/req=31.25%；a16g0 b01/b02 g1 占 37.51%、g2 占 62.49%，b03 g0 占 18.75%、g2 占 81.25%，总 miss/req=40.62%；a8g1000 g0 占 24.96%、g2 占 75.04%，总 miss/req=37.51%；a8g2000 g0 约 12.5%、g2 约 87.5%，总 miss/req=43.77%。group2 内部 miss/req 均为 2/4=50%。

## 4. same req=4,miss=2 的残余占用跨 gap 是否在全量中存活
存活。三轮 a8g0 的 group2 mean_main 约 1406.5-1409.6，a8g1000 约 1100.3-1102.2，a8g2000 约 840.6-841.2；同向差异约 307 cycles（g0 对 g1000）与约 567 cycles（g0 对 g2000）。range 大多不重叠或仅小段重叠。见 table same_req4_miss2_across_gaps 与 evidence.json:2704-2710、3704-3710、4204-4210、5204-5210、6204-6210、6704-6710、7687-7693、8687-8693、9187-9193。

限制：这仍是在不同 gap/outer 供给条件下观察，不能仅凭此断言纯 IOMMU 因果；它说明固定 16 抽样相位不能解释全部差异，但不证明实际占用计费有控制价值。

## 5. 固定 16 抽样相位偏差
全量 vs 同组固定 16 抽样的 mean 偏差通常小：group2 a8g0 约 +0.1% 到 +0.3%；a16g0 group2 约 +0.5% 到 +0.9%；a8g2000 group2 约 -0.1% 到 +0.45%；a1g0 与 a8g1000 的 group0 main 均值恒为 20，duration 偏差约 0.0%-0.26%。但采样覆盖强偏相位：a8g0/a16g0/a8g2000 采样只落在 group2，a8g1000 采样只落在 group0，a1g0 采样只落在 group0；未采样组没有 sampled_same_group 对照。sample range 也明显窄于 full range，例如 b01-a16g0 duration full max 2345 vs sampled max 2178；b01-a8g2000 main full max 1117 vs sampled max 916。见 tables.full_vs_fixed16_same_group 与 sample_phase_bias。

## 6. V hit/miss
绝大多数 main 的 common_window lookup_hit_v=16384、lookup_miss_v=0；只有 b01-a16g0 与 b02-a16g0 各为 hit=16382、miss=2，b03-a16g0 回到 miss=0。见 table v_lookup_hit_miss。V hit 占绝对多数不能推出 V 不受共享服务/出口等待影响；V miss 2 次也不能单独推断替换、PTW 阻塞或纯 IOMMU 因果。

## 7. short 分离
short-cost-solo/a8g0/a16g0 见 evidence.json:76-1058，v_done=64、warmup=8。short-a8g0 全量 group_counts=[24,0,39,0,0,0]，sum_req=252、sum_miss=78；short-a16g0=[12,0,51,0,0,0]，sum_req=252、sum_miss=102。short 不并入 main 中位数与结论。

## 8. 现有路线证据的位置
E2 W8/16 entry eviction then first miss 的局部支持、E3 局部受控共享服务阻塞证据、P4M60 tradeoff baseline 在 paper-logic.md:277-283、:312-314、:440-444 有记录。本段 fullstats21 不新增 E2/E3 因果结论，也不改写 P4M60；它只提供全量分组/占用观测。

## 9. 不能声称什么
- 不能从 group sums 推导总体中位数或分位数：evidence.json:552-556、1052-1057 与 docs/research-fullstats-v1.md:21、:47。
- 不能把组内搬运当个体独立或板级重复：docs/research-fullstats-v1.md:47；evidence.json:10961-10969。
- 不能宣称完整 DDT/PDT/PTW 类型覆盖：docs/research-fullstats-v1.md:19；evidence.json:11021。
- 不能宣称 M 收益或新颖性：paper-logic.md:376、:400-408、:440-444。

## 10. 警告与 provenance
UART 开头有大量零字节再出现 Hello World（uart-readable.txt:1），包含内核启动、IOMMU 初始化、IDMA 批次/路径日志，但 IDMA 行与内核时间戳交错且截断（例如 uart-readable.txt:284-285）。UART 中 hrtimer interrupt took 13207760 ns 见 uart-readable.txt:287；IOMMU no IRQ for page-request queue, disabling ATS/PRI 见 uart-readable.txt:218。taint 用例级为 before/after 0，但 params 明示 dmesg 只作 raw collection、不声明零 warning（params.txt:20-21）。README 说明本轮未新做下载哈希或 SD 回读（README.md:5）；DELL_RECEIPT 说明 rsync 内容无差异、取回前后 MD5 一致、板端归档 420651 字节、collection/report exit0 且 errors 空（DELL_RECEIPT.md:3），但没有独立离线语义验收或性能分析（DELL_RECEIPT.md:5）。最终 198 文件哈希验证仍待 Codex；此刻不能声称输入未变。
