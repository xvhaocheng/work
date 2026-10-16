# P4M pilot-21 结果复核与启动协议控制计划

**状态一句话**：P4M-4 pilot-21 已完成并在服务器复核，21/21 功能通过；P4M-2 legacy `--batch` 与 P4M gap0 显式比较已执行，但受启动相位/预窗口历史混淆，结论未定。先做共享启动协议控制，再进入正式 P4M-5；本轮只做分析、文档和计划，不改代码、不跑板、不构建。完整 21 条表见 [RESULTS.md](../out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/RESULTS.md)，本文不复抄大表。原始记录：[records.json](../out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/records.json)、[observations-unique.log](../out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/observations-unique.log)、[console.raw.log](../out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/console.raw.log)、[run.json](../out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/run.json)、[upload-receipt.json](../out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/upload-receipt.json)。

## 1. 功能边界与数据口径

- 21/21：success=true、result.status=0、V issued/done=4096、data_errors/quarantined=0、final taint=0、collection_failed=0。
- B8 无 P4M JSON bank，硬件窗口只能取 `idma_batch_obs ... part=time ... cycles`；A 窗口统计不可得。P4M 窗口仍按 `(open, close]` 语义；HPM 软件快照窗口、`victim_elapsed_ns`/`drain_ns`/`done_to_read` 不能当成 P4M 硬件窗口。见 [P4M_BASELINE.md](../P4M_BASELINE.md)。
- 本批未完成下载哈希/SD 回读/独立主机验收/HPM 校准；run.json 明确 independent_host_acceptance 未运行，不应改写为通过。

## 2. 关键复核：第三 P4M gap0 的“更快”不是单纯全局 ID 或 poll tail

本轮直接抽查 records.json 索引 3/11/19（seq8/16/24）及原始 console seq24 `part=time` 行；raw console 在 9187/9826/10478 行重复给出同一 cycles=7248503。

RTL/驱动证据：

- RTL frontend 行 147/151/175：batch issued/completed 按 batch/control 复位；行 265/271：observer taskID = batch issued/completed + 1。
- observer 行 305–324：在 first V issue 采样 opening A inflight ID。
- driver `idma.c:658–667`：先 `idma_submit(A)` 再 `idma_submit(V)`；`idma_submit` 393–433 只做 MMIO 配置与 NEXT_ID trigger，没有 pair-wide bounded start protection。
- 结论：opening A inflight ID 前两样本=4（开窗时已有 3 个 A 完成），第三样本=1448（开窗时已有 1447 个 A 完成），相差 1444 个 A copy；但三者窗口 A done 都是 4096，公共窗口末端计数正确冻结。这支持“预窗口 A 进度/启动历史不同”，不是单纯全局 ID 编号或窗口后 poll tail 造成的假象。
- 残余算术（ns）：whole elapsed − hw time − done_to_read：
  - seq8: 174088640 − 155033600 − 18648320 = 406720
  - seq16: 155917760 − 154973840 − 535340 = 408580
  - seq24: 171325240 − 144970060 − 5912940 = 20442240
  该残余只能标为“硬件窗口与 poll tail 之外的其他开销”，不是精确 trigger interval；第三样本残余显著更大，说明启动/预窗口历史确有不同。CPU 中断/抢占是怀疑方向，但不是已证根因；不能声称全部 ~6.42% 周期变化已解释，也不能声称容量机制已被证明。

## 3. P4M-2 配对观察与边界

- 第三对 A8g0/legacy B8 硬件窗口 cycles 比值约 0.93578（约 −6.42227%），前两对约 1.00078/1.00054（约 +0.07795%/+0.05409%）；n=3，描述性。
- gap500 三个 block 均比同 block gap0 更慢，保留为非单调反例。
- gap1000/gap2000 有 V/A tradeoff 迹象，但 n=3 不能写成正式 winner；0/500/1000/2000 继续保留为候选，不冻结正式实验参数，也不声称 500/1000/2000 是最终选定点。
- 相位/启动历史差异是已证实的观测；IOTLB 状态/容量假设仍是假设，未被本批单独证实或排除。不要在文档里把两者混成同一绝对结论。

## 4. 下一步：先做共享启动协议控制，再进正式 P4M-5

- 整体方法论文方向保留。P4M-4 pilot 完成；P4M-2 比较已做但 inconclusive；新优先级是先做启动协议控制，先于正式 P4M-5。
- 不把“未改启动流程的盲 ABBA”作为唯一下一步：它不能移除本批已观察到的启动/预窗口混淆，且 legacy batch 无 opening ID bank，单靠 ABBA 无法直接对照开窗时 A 进度。
- 待评估的最小设计（本轮未实现、未上板）：legacy batch 与 P4M 共享一个软件 driver batch start 修复——A/V 先配置，随后只用短、非睡眠临界区罩住两个硬件 trigger 操作，以约束 CPU 中断/抢占缝隙；trigger 后立即恢复 IRQ/抢占，绝不跨 DMA 执行、poll、drain、alloc、logging 持锁。保持既有 ordering/barriers、cleanup/error ownership、单 batch request 契约、BIT、测量窗口、HPM 语义、连续硬件 gap 与 polling。同时记录 A/V 软件 trigger 时间戳与持续时间，窗口后记录；明确标注为软件 trigger 间隔，不是 cycle-exact 硬件 issue 间隔。只做软件构建，不需要 FPGA 重建；低层改动需 Codex 关键源码/时序复核与适当主机验证，本轮不做实现或自动测试。
- 新软件验证后计划：先 4 次短 sanity，再做 8 次平衡比较 trial（B/P/P/B 重复两轮；4096 jobs、256 warmup、4 KiB、gap0）。新 8 次与旧 21 次分开，不替换旧异常；新版本用自己的 baseline；B batch 没有 A window/opening ID，需 BOTH modes 新 trigger timing，P4M 才用 bank opening ID。不要人为要求 opening ID=4，也不宣称 n=4 等价；比较 paired ratios 与完整分布，先调查系统性残余再进正式。

## 5. 本轮边界与分工

- 本轮仅分析、计划、文档；无 C 代码修改、无板卡运行、无构建、无新增 validator/regression；已有原始报告不改。
- 分工：DeepSeek 提供证据草案；Codex 做关键源码/原始记录抽查、决策与执行复核。
