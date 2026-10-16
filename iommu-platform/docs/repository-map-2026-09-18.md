# 当前仓库接手地图（2026-09-18）

结论：当前开发平台是 CVA6 + 四 iDMA + IOMMU + Linux，研究多 DMA 共享 IOMMU 的地址翻译干扰。E3 排队定位成立，配对因果未成立：E3 v5 已实现并通过主机验证及软件/FPGA 完整构建，但尚未上板；旧版 E3 在 Mac 上完成过 00085 D0/D0 板上诊断，预热前史仍不同，旧约 8 ms 轮次差未稳定复现。不能宣布 HOL、纯 PTW 代价或方法有效性。

模块路径均相对平台根 `iommu-platform/`。这份地图只做当前模块路径导航，不记录源码行号，也不代替 `git status`/`sources.lock.json` 的版本核对。各阶段工作见 [HANDOFF.md](../../HANDOFF.md)，当前状态见 [CURRENT_STATUS.md](../../CURRENT_STATUS.md)。

## 模块入口

| 工作 | 入口及关系 |
|---|---|
| FPGA 集成 | `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv`、`cva6/corev_apu/fpga/src/ariane_xilinx.sv`：DMA/IOMMU/观测器接线；iDMA 源码由 CVA6 跟踪 |
| IOMMU 触发输入 | `cva6/corev_apu/riscv-iommu/rtl/rv_iommu_top.sv`：V 等待的触发识别入口；旧逻辑只看 AR FIFO 队头，v5 改为跟踪已接收未选中的 V 读 |
| 共享控制器 | `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv`：选中/请求/响应事件、AR 队头、COMPLETE 握手后弹出事务 |
| 选择性延迟与观测 | `cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_e3_delay.sv`；`cva6/corev_apu/fpga/src/dma_observer.sv`、`cva6/corev_apu/fpga/src/dma_path_observer.sv`、`cva6/corev_apu/fpga/src/dma_e3_observer.sv`、`cva6/corev_apu/fpga/src/dma_e3_write_observer.sv` |
| 内核实验 | `linux/drivers/misc/idma.c`：批量/配对/诊断等实验主体；具体模式以当前协议文档和实际头文件为准 |
| 用户/内核 ABI | `linux/include/uapi/linux/idma.h`：E3 字段和 ioctl 定义。当前 ABI：`--diagnostic 1` → ioctl `0x0a`、D0/D0、schema `idma.e3_diag.v1`；`--diagnostic 2` → ioctl `0x0b`、D0/D128、要求 v5 BIT、schema `idma.e3_queue_pair.v1`。字段以 [E3_QUEUE_PAIR.md](../E3_QUEUE_PAIR.md)、[E3_DIAGNOSTIC.md](../E3_DIAGNOSTIC.md)、[E3_SERVICE.md](../E3_SERVICE.md) 为准 |
| 用户态 | `bench/idma_experiment.c` 通用实验；`bench/idma_e3_pair.c` 配对/诊断 CLI，输出 schema 以 [E3_QUEUE_PAIR.md](../E3_QUEUE_PAIR.md)、[E3_DIAGNOSTIC.md](../E3_DIAGNOSTIC.md) 为准 |
| 构建和交付 | `Makefile` → `scripts/platform.py`；`configs/migration-4dma-e3-queue.json`；当前 E3 v5 交付 [交付说明](../out/e3-followup-20260918/delivery/README.md) |
| 验证入口 | `tests/test_e3.py`、`tests/test_e3_cli.py`、`tests/test_idma_scheduler.py`、`tests/test_dma_path.py`；验证范围见 [E3_SERVICE.md](../E3_SERVICE.md) 和 [validation.md](validation.md)，不能当作完整系统仿真 |

## 版本与工作树

- 不在此文档硬编码当前 HEAD；先用 `git --no-optional-locks status --short`、`git rev-parse HEAD` 和 `sources.lock.json` 对照实际状态。
- 组件修改按实际依赖更新父 gitlink 和 lock：IOMMU → CVA6 → 顶层。`make check` 会写 `out/manifests`，不是严格只读命令。
- 现有 FPGA 导出目录通常拒绝覆盖；不要为熟悉仓库重跑完整构建。
- 历史脏文档、原始日志、数据和旧产物保留；旧日期记录本身不是错误，应归档解释，不要抹成当前状态。不要按目录名推断删除或裁剪组件。

## 阅读优先级

- 当前状态速览：[CURRENT_STATUS.md](../../CURRENT_STATUS.md)；各阶段工作与证据：[HANDOFF.md](../../HANDOFF.md)。
- 当前 E3 v5：[E3_QUEUE_PAIR.md](../E3_QUEUE_PAIR.md)。
- E3 字段/旧协议：[E3_SERVICE.md](../E3_SERVICE.md)、[E3_DIAGNOSTIC.md](../E3_DIAGNOSTIC.md)。
- 结果复核：[e3-diag-results-review.md](e3-diag-results-review.md)、[e3-results-review.md](e3-results-review.md)、[validation.md](validation.md)。
- 论文逻辑：[paper-logic.md](paper-logic.md)。
- 时钟/CDC/IO 审查仍未完成；旧 bundle/恢复副本不能假定包含最新实验提交；历史工程和恢复副本不作为开发目录。
