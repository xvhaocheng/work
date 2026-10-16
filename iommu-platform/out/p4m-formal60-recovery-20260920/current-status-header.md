# 论文平台当前状态（精简版）

## 2026-09-20 P4M formal60：24 通过，第 25 次日志计数检查停止（未完成）

证据目录：`/home/xvhaocheng/work/iommu-platform/out/mac-p4m-formal60-20260920-215406-run-cicggil4`。14 份原始证据已回传并逐字节核对一致，含 25 个 JSON、完整 UART、板端目录压缩包（207 项）、冻结 PLAN、实际 runner、版本来源、RESULTS.md 与 FAILURE_HANDOFF.md。故障原因由 Codex 核定，DeepSeek 起草交接；没有另跑独立验收。

计划 gap0/1000/2000 各 20，共 60，4096 transfers / warmup256 / 4KiB / V0-A1，20 块交错。复用同次启动的 Linux00089 与 P4M v7 BIT，无重启/重载。实际 24 次通过（每 gap 8），第 25 次 formal-b09-g2000 CLI exit0、JSON success=true、双方 data_errors/quarantined0、taint0，但 runner exit5；35 次未运行，无自动重跑。

错误原文：`P4M: case=formal-b09-g2000 verify_failed=startup_line_count before=36 after=36 want=37`。归档前快照 seq1..36，后快照 seq2..37，各36条：旧seq1被 dmesg 环形缓冲覆盖，新seq37存在。继承的记录检查要求总数+1，因而停止；并非发现 DMA 内容校验错误。新 formal60 矩阵未改该检查函数。保留第25样本但不计通过；RESULTS.md gap2000 n9含此样本，不能当成9个合格结果，也不能宣称完成60次。

下一步候选仅修脚本按新 seq 定位、保留其余检查，再由用户决定剩余计划；本轮未修 runner、未补跑，不需要因这处脚本缺陷重建 BIT/Linux。FPGA Linux 保持运行，主机串口采集已关闭。旧 gap500 反例及所有历史证据保留。下面旧记录里的“尚未上板/正式参数未冻结”是历史状态，不覆盖本条。

## 2026-09-20 P4M 启动控制软件：软件构建通过，板上未跑

本轮完成 P4M 启动控制软件（驱动 batch start、测试、runner、finalizer）与配套文档：DeepSeek 起草，Codex 复核并执行。`make software PROFILE=migration-4dma-p4m JOBS=12` 退出 0（2026-09-20 12:12–12:13 UTC）。

- 版本：内核 `6.5.0-rc2-00089-gefd9461d6459`，Linux `efd9461d6459049f59c9ea5466f3703e45413bb3`，顶层 `a08426f46720b646ae0fa052a0a27136a347b69f`，CVA6 `e822bc9d`、IOMMU `137f5e5`；新内核是独立软件基线，不与 `00088` 混算。
- 硬件不变：沿用既有 BIT `7af1ccbc…`（未重建 FPGA），新 payload `fc8e8a73…`；交付目录 `out/p4m-start-control-20260920/delivery`，产物哈希、rootfs 内的新脚本及复用 BIT 来源均已核验。
- 主机检查 6 套件全 0：batch-start 10 场景、scheduler、controlled、CLI、rootfs、board-p4m-start runner 41 例（含主机编译 CLI 与 BusyBox ash/awk）；均为主机检查，非板级证据。
- 板上未跑（board_tested=false）：无功能/性能结论。板卡命令 `platform-p4m-start all /tmp/p4m-start-01`；UART 与日志由 Mac 侧捕获，Dell 未接板。
- 协议、字段契约、runner 门限与判据见 [P4M 启动控制协议](iommu-platform/P4M_START_CONTROL.md)，验证见 [validation.md](iommu-platform/docs/validation.md)，产物见 [out](iommu-platform/out/p4m-start-control-20260920/)。
- 下一步：4 短试次 + 8 比较试次（4 对）均只作描述，正式参数未冻结。

