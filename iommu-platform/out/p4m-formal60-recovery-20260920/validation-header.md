# 验证记录（2026-09-08）

## 2026-09-20 P4M formal60：24 通过，第 25 次日志计数检查停止（未完成）

证据目录：`/home/xvhaocheng/work/iommu-platform/out/mac-p4m-formal60-20260920-215406-run-cicggil4`。14 份原始证据已回传并逐字节核对一致，含 25 个 JSON、完整 UART、板端目录压缩包（207 项）、冻结 PLAN、实际 runner、版本来源、RESULTS.md 与 FAILURE_HANDOFF.md。故障原因由 Codex 核定，DeepSeek 起草交接；没有另跑独立验收。

计划 gap0/1000/2000 各 20，共 60，4096 transfers / warmup256 / 4KiB / V0-A1，20 块交错。复用同次启动的 Linux00089 与 P4M v7 BIT，无重启/重载。实际 24 次通过（每 gap 8），第 25 次 formal-b09-g2000 CLI exit0、JSON success=true、双方 data_errors/quarantined0、taint0，但 runner exit5；35 次未运行，无自动重跑。

错误原文：`P4M: case=formal-b09-g2000 verify_failed=startup_line_count before=36 after=36 want=37`。归档前快照 seq1..36，后快照 seq2..37，各36条：旧seq1被 dmesg 环形缓冲覆盖，新seq37存在。继承的记录检查要求总数+1，因而停止；并非发现 DMA 内容校验错误。新 formal60 矩阵未改该检查函数。保留第25样本但不计通过；RESULTS.md gap2000 n9含此样本，不能当成9个合格结果，也不能宣称完成60次。

下一步候选仅修脚本按新 seq 定位、保留其余检查，再由用户决定剩余计划；本轮未修 runner、未补跑，不需要因这处脚本缺陷重建 BIT/Linux。FPGA Linux 保持运行，主机串口采集已关闭。旧 gap500 反例及所有历史证据保留。下面旧记录里的“尚未上板/正式参数未冻结”是历史状态，不覆盖本条。

## 2026-09-20 P4M start-control 00089：12/12 板上完成并回传

