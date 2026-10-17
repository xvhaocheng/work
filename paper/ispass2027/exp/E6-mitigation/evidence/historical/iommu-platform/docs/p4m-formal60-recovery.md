# P4M formal60 恢复与 resume35 准备报告

## 1. 结论与状态

- 原始 formal60 计划已冻结：gap0/1000/2000 各 20，共 60；20 个交错 blocks；每 trial 4096 transfers / 256 warmup / 4 KiB；V0/A1 A8；仍 polling 完成，irq_pair 只表示触发临界区协议，不是 DMA 完成改中断。
- 原始在线运行：旧 runner 24 在线通过；第 25 次 formal-b09-g2000 因 startup_line_count 36->36 检查 exit5；后 35 未跑。原始 run.json 仍为 24 在线 / 25 attempted / 35 unrun / runner exit5，未改写。
- 第 25 次作为离线复核追认保留；不伪称 25 在线通过，不重跑/剔除，不抹掉原始 flags。
- 采集修复与 resume35 模式已由 Codex 应用并完成主机验证；resume35 的 35 次尚未运行，状态 prepared_not_run。Dell 未接板。
- 本文件由 DeepSeek 起草；Codex 已应用脚本并实际执行/接受主机与离线检查；DeepSeek 未运行实验、未改实现、未重新设计矩阵。

## 2. 原始 formal60 证据与故障定性

- 原始证据：out/mac-p4m-formal60-20260920-215406-run-cicggil4。14 份原始交付已回传并逐字节核对一致（Codex 机械核对全部 14 份原始交付 SHA 一致），含 25 个 JSON、完整 UART、板端目录压缩包 207 项、冻结 PLAN、实际 runner、版本来源、RESULTS.md、FAILURE_HANDOFF.md。
- 25 份 before-dmesg 错误文件皆空。
- 错误原文：`P4M: case=formal-b09-g2000 verify_failed=startup_line_count before=36 after=36 want=37`。
- 归档前快照 seq1..36，后快照 seq2..37，各 36 条：环形日志淘汰旧 seq1，完整新 seq37 存在。
- 定性：runner 记录计数假设失效；不能写 DMA 失败或新硬件根因。新 formal60 矩阵未改该检查函数。
- 修复方向：运行前记录 maxseq 并以唯一新 seq 定位，随后保持 startup/batch_time/JSON 关联和数据检查。

## 3. 采集修复与主机验证

- 修复脚本：tests/board-p4m-formal60.sh 已应用新 resume35 模式；交付复制为 out/p4m-formal60-recovery-20260920/delivery/platform-p4m-formal60。
- 实际主机验证（Codex 执行/接受）：
  - tests/test_board_p4m_start.py 41 项通过。
  - tests/test_p4m_formal60_recovery.py 16 项通过。
  - tests/test_p4m_formal60_resume.py 15 项通过，含真实 seq37 归档 BusyBox 预检查。
  - shell syntax 0。
  - 加强版 offline_revalidate.py 25/25，all_passed=true；global_metadata 通过；PLAN25 前缀通过；taint0；CLI exit0；capture complete；JSON success/errno0；data_errors/quarantined0。
  - 证据 tree 和原输入 hash 未变；Codex 机械核对全部 14 份原始交付 SHA 一致；25 份 before-dmesg 错误文件皆空。
- 上述检查记录：out/p4m-formal60-recovery-20260920/validation-final.json、validation-initial.json、audit.json、input-hashes.json、statistics.json。

## 4. resume35 运行范围与预检

- 只跑 flat 26..60：先 b09g0、b09g1000，然后 b10..20 原顺序。
- 35 分配：gap0 12、gap1000 12、gap2000 11；合并已有 8/8/9 可成 20/20/20。
- 必须用新单文件脚本；旧 rootfs 内 runner 未更新；不能直接用旧 collector（固定 60）冒充已适配。
- Preflight 要求：
  - 当前 Linux exact 6.5.0-rc2-00089-gefd9461d6459；
  - path_observe Y；
  - taint0；
  - 当前 dmesg maxseq37 完整唯一 startup+batchtime，且 a_begin_ns=3600085892000；
  - 原板端 /tmp/p4m-formal60-215406 存在，并保留第 25 次的 JSON、原始 dmesg 和 params.txt。
- 若发现重启、额外试验、seq 改变、证据缺失等不通过则停止人工复查；不能用改门限/重新跑 formal60 绕开。
- 板端继续运行 Linux，使用已有 00089/P4M v7，无需 SD 更新或重启或 BIT 载入；无内核/RTL 修改或构建。
- Dell 无接板，本次 35 尚未运行；报告状态 prepared_not_run。

## 5. Mac 操作员步骤（待执行，非已完成）

- 按既有传输方式将新单文件放到板端 /tmp/platform-p4m-formal60。
- 串口完整保存 stdout/stderr：
  - `sh /tmp/platform-p4m-formal60 resume35 /tmp/p4m-formal60-resume-01`
  - 新输出 dir，不能覆盖旧 dir；不用 tee 管道掩盖退出码；退出立即 `echo $?`。
- 完整保存 35 JSON/err/dmesg/argv/matrix/params/taint/resume35-preflight/串口和 exit。
- 完成后只读归档回 Dell 再合并审计。
- 脚本 35 完成不等于论文 60 正式已审结。
- 既有 Mac 采集器自动上传/参数适配未实现；不声称脚本已传到 Mac/FPGA。

## 6. 首 24 样本描述性分析边界

- 分析用首 24 个（8 完整 blocks）保持 balanced；详细读 statistics.json；不混入第 25 不完整 block 到主统计。
- V MiB/s 中位数：108.8275045 / 150.6503266 / 171.4545531（gap0/1000/2000）。
- 同硬件窗口 A MiB/s 中位数：108.8275045 / 75.3251633 / 57.0900898。
- A 整轮 miss/PTW 中位数：5140 / 3086 / 2401；V miss/PTW 为 0。
- 相对 gap0 配对 V 硬件周期比中位数：0.72242199 / 0.63492757；对应反比推算吞吐提升 38.42% / 57.50%。
- 同窗口 A 速率配对比中位数：0.69211626 / 0.52448158；对应下降 30.79% / 47.55%。
- 这些描述显示节流的 V/A 吞吐取舍；不宣布最终效果/显著性/最优 gap 或 IOMMU 机制根因。
- 整轮 HPM 不能当窗口计数。50 MHz 是既有假定时钟。
- 不要把各组 median 之比和 median 配对比完全混为一谈。

## 7. 总体计划与后续

- 只修复采集并完成原 60；不扩 gap、不加 Solo、不重新 12 验收。
- 之后全 60 每组 median/min/max/波动；20 block 同块配对；同时分段展示暂停前 8 完整块/后 11 完整块/b09 跨暂停块。
- 所有样本保留；暂停记录作协议偏离，不把两段当不间断；结果完成后再决定取舍。
- 未证明创新方法；旧预试不计 60。

## 8. 证据路径与交付内容

- 原始证据：[RESULTS.md](../out/mac-p4m-formal60-20260920-215406-run-cicggil4/RESULTS.md)、[PLAN.json](../out/mac-p4m-formal60-20260920-215406-run-cicggil4/PLAN.json)。
- 恢复证据：[离线审计](../out/p4m-formal60-recovery-20260920/audit.json)、[统计](../out/p4m-formal60-recovery-20260920/statistics.json)、[最终验证](../out/p4m-formal60-recovery-20260920/validation-final.json)、[初轮验证](../out/p4m-formal60-recovery-20260920/validation-initial.json)、[原始文件校验值](../out/p4m-formal60-recovery-20260920/input-hashes.json)。
- 交付说明：[README.md](../out/p4m-formal60-recovery-20260920/delivery/README.md)。
- 交付目录包含：PLAN-original.json、remaining-plan.json、audit.json、statistics.json、platform-p4m-formal60。
- 交付目录的 SHA256SUMS 记录最终文件校验值。
- 未验证边界：下载 BIT hash / SD readback / HPM 校准未验证；非本次阻塞。

## 9. 起草与执行归属

- DeepSeek：起草本报告和局部状态/交接/验证节草案。
- Codex：应用脚本、执行/接受主机与离线检查、机械核对 SHA、准备 resume35。
- 实际 resume35 板端运行：尚未执行；状态 prepared_not_run。
