# 固定 SDK / Snitch 来源后继核查

2026-10-15。**已继续核完已知 SDK 链；当前固定 SDK 确有 Carfield driver，但取得的是物理连续内存与寄存器映射接口，应用树只有 addressability 测试，不能绑定论文的 Linux 6.9 / SVA / FP32 双缓冲工作负载。** 最小缺口已经缩为“论文实际实验软件的固定版本和那条 SVA 应用/driver/runtime 链”，不再停在“SDK 未展开”。

本文件追加在原 `enabled-sva-source-gap.md/.json` 之后，保留原件及其当时的 unknown。未改 HERO 合同、冻结输入或执行、旧验收、production；未构建、仿真或板测，也未搜索别家项目。全部由 Codex 核原件，无 DeepSeek/Claude 调用。

## 本次固定范围与读取情况

- SDK：Carfield gitlink 的 [pulp-platform/cva6-sdk@98611e6c4d14c308e930af2bfb0b703c361d4d05](https://github.com/pulp-platform/cva6-sdk/tree/98611e6c4d14c308e930af2bfb0b703c361d4d05)。
- Snitch：Carfield `Bender.yml:28` 的 [pulp-platform/snitch_cluster@269830af872fa943d8b079470120d42d62e1cb69](https://github.com/pulp-platform/snitch_cluster/tree/269830af872fa943d8b079470120d42d62e1cb69)。只读 sw 树入口与一条最接近的 axpy 源链，不遍历其 submodule。
- SDK root git-tree 请求返回 409；contents root/ref、固定 raw 文件可读。`sw/apps` contents 返回 404、明确 tree SHA 请求返回 409。**没有据此声称 SDK 为空或不可取得**：最后用一次受 8MiB 上限约束的固定 SDK archive 读取目录名。实际 archive 只有 **64129B、73 项**，SHA256 `733827bf0aa27ef2c61a5f0246083fdf3aa1fe8b93eb12584d75f89d918fdb65`；`sw/apps` 只有 addressability 的 Makefile 和 C 文件。没有读取 rootfs 凭据文件内容，未展开 submodule 或落大仓。

所有请求、返回状态、原件内容或目录名、SHA256 在 `enabled-sva-sdk-followup.json`；API 错误作为读取通道的事实保留，最终结论使用已成功取得的源码和 archive 目录。

## SDK 原件直接给出的链

| 原件 | 位置 | 事实与边界 |
|---|---|---|
| [SDK Makefile](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/Makefile#L155) | 155–168 | `images` 依赖 `rootfs/root/carfield.ko`；模块从 `sw/drivers/carfield` 构建。因此是可导航实际 driver 入口。 |
| [sw/drivers/carfield/Makefile](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/Makefile#L7) | 7–18 | 默认 kernel 为 `build/linux-v5.10.7`，模块是 `carfield.o`。 |
| [configs/buildroot64_defconfig](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/configs/buildroot64_defconfig#L22) | 22–29 | Linux 来源 stable/linux.git，固定 tag `v5.10.7`，应用本仓 `linux_patch/`。这与论文 TXT 138 的 patched Linux 6.9.0 不一致，不能静默当作论文软件版本。 |
| [carfield.c](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/carfield.c#L89) | 89–171 | `mmap` 选择 SoC/L2/cluster 的物理区域，设 noncached 后 `remap_pfn_range`。这是 CPU 映射硬件资源，不是给 accelerator 创建用户 buffer 的 IOVA mapping。 |
| [同一 carfield.c](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/carfield.c#L174) | 174–207；完整文件456行 | 唯一 ioctl case `0x1` 用 `kmalloc` 与 `virt_to_phys` 返回连续物理地址；完整文件无 `iommu`/domain attach/IOVA map/unmap 调用。不能冒充论文所述 SVA driver。这里不作无关代码质量审查。 |
| [sw/apps/addressability/addressability.c](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/apps/addressability/addressability.c#L24) | 24–42；完整文件45行 | 打开用户指定 device，mmap 4KiB SoC 寄存器，读 scratch/reset registers，munmap/close。没有计算 kernel、tile DMA、OpenMP target 或 SVA 映射。 |

因此不是“未找到 driver”；而是**已经取得这一固定版本的 driver，并查明它不是所需的 SVA 映射接口**。不是“未能读 apps”；而是 archive 已闭合该目录内容，固定版本只提供 addressability 应用。

## Snitch 固定依赖的应用入口

`snitch_cluster` 固定 sw subtree `cf6056b8e4f997d117c73209a72bd60db11ccc6e` 的递归目录返回 `truncated=false`，共340项。含 `sw/blas/axpy`、`sw/blas/gemm` 和其他裸机应用/`snRuntime`；没有因目录截断漏掉已列条目。

只核了一条最接近的 [sw/blas/axpy/src/main.c](https://github.com/pulp-platform/snitch_cluster/blob/269830af872fa943d8b079470120d42d62e1cb69/sw/blas/axpy/src/main.c#L11)：

- 11–25 行使用 `double`、静态 `data.h` 数据符号和 TCDM 指针；
- 27–35 行由 DM core 启动 x、y 两笔读取，`wait_all` 后 barrier；
- 37–53 行计算后 barrier，再写出 z、`wait_all`、barrier；
- [axpy.h](https://github.com/pulp-platform/snitch_cluster/blob/269830af872fa943d8b079470120d42d62e1cb69/sw/blas/axpy/src/axpy.h#L7) 7–39 行明确 double 运算/`fmadd.d`。

这给出一条真实 DMA/compute 程序，但没有 Linux 用户映射、实际 SVA 启用或论文的 FP32 tiled double-buffer 绑定。不能仅凭名字同为 axpy 认定就是 DATE 应用；也没有把整个 Snitch 依赖所有程序一并判为无 SVA。本次不继续展开其他 kernel 或 runtime 来拼一条未经版本证明的“等价论文实现”。

## 新输入后的最小缺口与裁决

已补的输入：SDK driver 原件、模块构建入口、Linux 配置、实际应用目录和 addressability 原件；Snitch 的固定 sw/app 入口及具体 axpy DMA 代码。原报告中“SDK 未展开”的 unknown 被本次结果替代，其历史原件不改写。

仍缺的最小输入是作者 **DATE 2025 实验软件的准确 ref**，至少同时包含：

1. Linux 6.9 / RISC-V IOMMU domain attach、用户页 pin/map/unmap 与 ioctl 的实际 driver；
2. 调用该接口的用户态库/OpenMP runtime，以及论文 axpy 或 gemm 的固定 FP32 tile/DMA 程序；
3. SVA 模式开关、输入、DID/PID/domain、IOVA+bypass offset、flush/输出可见性、consumer/free 的一条完整调用链。

如果这些位于已知仓库的另一分支/提交，只需准确位置，不需要再开展跨项目地图。当前 pinned SDK 的 Linux tag 和 ioctl 不满足这条绑定，因此不能用“可编译 carfield.ko”代替准入；本次 **不接受真实 SVA 完整时延验证，也不自动追加实验**。冻结 source-contract 的语义结果继续按其原范围记录。

没有证明作者软件永久缺失、没有否定论文实验；已证明的是：**公开 Carfield 该 gitlink 所固定的 SDK 版本不能立即提供本轮需要的 SVA 应用/driver 链。**
