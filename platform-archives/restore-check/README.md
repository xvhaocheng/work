# 四 iDMA / RISC-V IOMMU 论文实验平台

这是从旧实验工程整理出的 **独立、固定版本、仅构建 Linux 路径** 的平台。保留上游历史和个人修复，不依赖旧目录。软件及 `migration-4dma` 的 BIT/MCS 已构建成功，HPM 定向测试与离线恢复已通过；尚未上板验收，遗留时钟约束警告见验证记录。

## 常用命令

在本目录运行：

```bash
make doctor
make check
make software JOBS=12
make hpm-test
make fpga PROFILE=migration-4dma
```

- `make software`：编译当前 benchmark，打包 rootfs，构建 Linux、DTB、lloader 和 OpenSBI payload。
- `make hpm-test`：用实际 HPM RTL 验证计数行为，并确认旧 RTL 会被同一测试检出错误。
- `make fpga`：从固定提交导出独立构建树，生成 IP、综合、实现、bitstream。需要可用的 Vivado 许可证。
- `make bundle`：导出平台和所有组件的 Git bundle，可离线恢复。

没有自动烧板、写 SD 卡、推送 GitHub 或升级依赖的步骤。

## 两个 FPGA 配置

| PROFILE | DMA | IOTLB | DS 输入／输出延迟 | 用途 |
|---|---:|---:|---:|---|
| `migration-4dma`（默认） | 4 | 16 | 50 / 50 cycles | 与旧工程进行迁移对照 |
| `baseline-4dma` | 4 | 16 | 0 / 0 cycles | 无人为延迟的候选性能基线 |

CPU 50 MHz，timer timebase 25 MHz，DID 10～13。两种配置都需要新 bitstream；软件镜像可共用。当前脚本只支持四路硬件，单路 smoke 使用四路硬件中的一个 DMA；不能只改 JSON 的 DMA 数量就认为已完成拓扑与设备树修改。

配置集中在 `configs/`。FPGA 参数只改导出的构建树，不改组件工作树。每个 PROFILE 的 FPGA 输出目录首次创建后会保留；失败重试方式见 [构建说明](docs/build.md)。

## 文件和输出

```text
cva6/                 固定 CVA6 子模块；内部包含 iDMA 和 IOMMU 子模块
linux/                固定 Linux 驱动及 benchmark ioctl
opensbi/              固定启动固件与 timer 修正
configs/              Linux defconfig、DTS、两套硬件参数
bench/                用户态测试源码
tests/                HPM RTL 回归、板上 smoke 脚本
scripts/              构建、归档、离线恢复
sources.lock.json     完整的 21 个组件版本及来源
provenance/           迁移前的本地修改副本（用于审查，不要再次应用）
assets/rootfs-base.cpio  固定的旧 rootfs 基础镜像
out/                  所有新构建和测试产物，不进普通 Git
```

主要软件输出：

- `out/linux/arch/riscv/boot/Image`
- `out/linux/platform.dtb`
- `out/rootfs.cpio`
- `out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin`

FPGA 输出：`out/<PROFILE>/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`，同目录另有 `.mcs` 文件。

## Git 怎么用

总仓库管理自己的脚本、配置、测试，以及组件提交指针。CVA6、IOMMU、Linux、OpenSBI 各自保留完整来源历史；工作分支叫 `platform`。

修改 IOMMU 后，依次提交 IOMMU → CVA6 的子模块指针 → 顶层的 CVA6 指针，并更新版本清单。`make check` 会检查工作树、每层指针和 lock 是否一致。这里没有创建远端个人 fork；`upstream` 只表示来源，不能把未上传的个人提交误认为上游可以下载。

**当前完整恢复请使用 bundle。** `.gitmodules` 中保留上游来源 URL，但个人提交不在上游，直接 `clone --recursive` 不保证成功。

```bash
make bundle
python3 out/source-bundles/restore.py out/source-bundles /一个尚不存在的新目录/iommu-platform
```

组件仓库没有旧目录的 Git alternates，也没有依赖旧目录的 origin。

## 验证范围

本地软件构建和 HPM 定向仿真不等于 FPGA 已经上板通过。请看 [验证记录](docs/validation.md) 与 [补丁说明](docs/patches.md)。板上运行 `/usr/bin/platform-smoke` 可执行四路 Solo、大传输和双路测试；需要保存串口日志并检查所有 `errors=0`。

上游许可证与版权文件保持在各组件内；基础 rootfs 沿用旧实验镜像，相关来源限制见 [rootfs 说明](assets/README.md)。
