# 四 iDMA / RISC-V IOMMU 论文实验平台

这是从旧实验工程整理的独立、固定版本、仅构建 Linux 路径的平台。当前平台为 CVA6 + 四 iDMA + RISC-V IOMMU + Linux；迁移基线和历史修复保留在各组件内，不依赖旧目录。研究安排按[唯一总计划](../研究计划.md)执行；FPGA保留为实验平台，仿真按需辅助。

硬件最小身份：Genesys 2、CVA6 50 MHz、timer 25 MHz、4 DMA、IOTLB 16。DS50/50 与整批轮询保持。

2026-10-15入口同步：E3 v5及v6后续板测已完成，受控v6只支持局部A-miss→V-hit等待传播，见[受控E3结果](docs/e3-controlled-results-review.md)；旧“v5未上板”是2026-09-18早期快照。历史Mac上板不能推出当前Dell接板；构建通过不等于板卡功能、时序或机制证据，新FPGA性能主张须验对应时钟/时序。历史验证见 [docs/validation.md](docs/validation.md)。

## 当前入口

- 当前安排：[唯一总计划](../研究计划.md)、[研究导航](../研究导航.md)、[当前状态](../CURRENT_STATUS.md)；阶段背景见[HANDOFF](../HANDOFF.md)。D为当前主线、I轻量预筛，A5保留候选与局部工作；MIG1792023314继续暂停，入口同步不授权新实验。
- 平台使用、构建、交付：本文件、[构建说明](docs/build.md)。
- 历史 E3 v5 协议与有限矩阵：[E3_QUEUE_PAIR.md](E3_QUEUE_PAIR.md)。
- E3 原协议/字段：[E3_SERVICE.md](E3_SERVICE.md)；D0/D0 软件诊断字段：[E3_DIAGNOSTIC.md](E3_DIAGNOSTIC.md)。
- 历史代码地图（2026-09-18快照）：[docs/repository-map-2026-09-18.md](docs/repository-map-2026-09-18.md)。
- 验证历史：[docs/validation.md](docs/validation.md)；补丁来历：[docs/patches.md](docs/patches.md)。
- 通用双路实验入口：`bench/idma_experiment.c`；历史 E3 配对入口：`bench/idma_e3_pair.c`。
- 历史 E3 诊断/配对 ABI（不代表当前研究合同）：`--diagnostic 1` 使用 ioctl `0x0a`、D0/D0、schema `idma.e3_diag.v1`；`--diagnostic 2` 使用 ioctl `0x0b`、D0/D128、要求 v5 BIT、schema `idma.e3_queue_pair.v1`。其他接口以实际头文件和协议文档为准。

## 常用命令

在本目录运行；只有需要构建或验证时才执行对应命令：

```bash
make doctor
make check
make software PROFILE=migration-4dma-e3-queue JOBS=12
make hpm-test
make fpga PROFILE=migration-4dma-e3-queue
make bundle
```

- `PROFILE` 是 Makefile 变量，`make software` 和 `make fpga` 都会把它传给 `scripts/platform.py`。历史 E3 v5 使用 `migration-4dma-e3-queue`；配置不止两套，具体以 `configs/*.json` 和协议文档为准。
- `make doctor`：检查本机工具。
- `make check`：检查固定源码、工作树、各级指针和 lock 是否一致；会写 `out/manifests`，不是严格只读。
- `make software`：编译用户态程序，打包 rootfs，构建 Linux、DTB、lloader 和 OpenSBI payload。
- `make hpm-test`：用实际 HPM RTL 验证计数行为；这是定向仿真，不是完整 SoC 或板卡验收。
- `make fpga`：从固定提交导出独立构建树并继续综合/实现/bitstream；需要可用 Vivado 许可证。
- `make bundle`：导出平台和组件 Git bundle，用于离线恢复。
- 当前构建/恢复脚本不自动烧板、写 SD 卡、推送 GitHub 或升级依赖。日志默认只保存，有实际错误再读；不要为熟悉仓库重跑完整构建。

## 文件和输出

```text
cva6/                 固定 CVA6 子模块；内部包含 iDMA 和 IOMMU 子模块
linux/                固定 Linux 驱动及实验 ioctl
opensbi/              固定启动固件与 timer 修正
configs/              Linux defconfig、DTS 和各 PROFILE 硬件参数
bench/                用户态测试源码
tests/                主机测试、RTL 回归、板上 smoke 脚本
scripts/              构建、归档、离线恢复
sources.lock.json     完整组件版本及来源
provenance/           迁移前的本地修改副本，用于审查，不要再次应用
assets/rootfs-base.cpio  固定旧 rootfs 基础镜像
out/                  构建、测试和回传产物，不进普通 Git
```

主要软件输出：

- `out/linux/arch/riscv/boot/Image`
- `out/linux/platform.dtb`
- `out/rootfs.cpio`
- `out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin`

FPGA 输出示例（历史 v5配置）：

- `out/migration-4dma-e3-queue/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`
- 同目录 `.mcs`

已存在的导出目录通常拒绝覆盖，失败重试见 [构建说明](docs/build.md)。

## Git 与版本

- 顶层仓库管理脚本、配置、测试以及组件提交指针；CVA6、IOMMU、Linux、OpenSBI 保留各自来源历史。
- 修改 IOMMU 后，按 IOMMU → CVA6 子模块指针 → 顶层 CVA6 指针的顺序提交，并更新 `sources.lock.json`；`make check` 会检查工作树、各级指针和 lock 是否一致，并会写 `out/manifests`。
- 具体当前工作树和提交以 `git status`、`git rev-parse HEAD` 及 `sources.lock.json` 为准，不在文档中硬编码 HEAD。
- 这里没有个人远端；`upstream` 只表示来源，个人提交直接 `clone --recursive` 不保证能取到。
- 现有 bundle/恢复副本来自迁移阶段，不能假定包含最新实验修改；迁移新版前重新运行 `make bundle`。

## 验证范围与边界

- 主机测试、HPM 定向仿真和构建成功都不等于 FPGA 上板通过、完整时序签核或 IOMMU 干扰机制成立。
- 原始时钟定义、CDC 和外部端口约束审查仍未完成；正 WNS 不等于完整约束或 CDC 签核。历史多轮板上数据的下载哈希/SD 回读按要求未补做，期望哈希不能写成实测。
- E0/P2/HPM 仍有未签核边界，历史验证见 [docs/validation.md](docs/validation.md)。E3 v5因果条件修正属于历史阶段，后续受控v6证据仍限定局部等待传播；不能从已有数据推出 HOL、纯 PTW 代价或缓解方法有效。
- 历史批量/观测类实验的详细口径以对应文档为准：[E4_BATCH.md](E4_BATCH.md)、[E4_OBSERVER.md](E4_OBSERVER.md)、[E4_PATH.md](E4_PATH.md)、[E3_SERVICE.md](E3_SERVICE.md)、[E3_DIAGNOSTIC.md](E3_DIAGNOSTIC.md)、[E3_QUEUE_PAIR.md](E3_QUEUE_PAIR.md)。原始日志、数据和产物保留在 `out/`，不因整理而删。
- 板上验证入口和结果复核见 [docs/validation.md](docs/validation.md) 及对应 results-review 文档；脚本完成只表示命令成功，不自动证明冷/热状态、per-DID HPM 归属或实际并发压力。

上游许可证与版权文件保持在各组件内；基础 rootfs 沿用旧实验镜像，相关来源限制见 [rootfs 说明](assets/README.md)。

已结束的 method88 主机脚本与专用测试已移至 [归档区](../platform-archives/retired-code-20261005/)，不再留在当前 `scripts/`、`tests/`。归档保留原相对路径，复现时按 `manifest.json` 移回；共享 E2/串口/分析模块仍在当前工程，原始结果和交付包保留。本次整理的两个测试入口将生成代码写入 `out/`。
