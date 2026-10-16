# pulp-platform/cva6-sdk/98611e6c4d14c308e930af2bfb0b703c361d4d05〔正式题名待整理〕

稳定ID：`code-pulp-platform-cva6-sdk-98611e6c4d14c308e930af2bfb0b703c361d4d05` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/Makefile#L155)
- [来源2](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/configs/buildroot64_defconfig#L22)
- [来源3](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/apps/addressability/addressability.c#L24)
- [来源4](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/Makefile#L7)
- [来源5](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/carfield.c#L89)
- [来源6](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/carfield.c#L174)
- [来源7](https://github.com/pulp-platform/cva6-sdk/tree/98611e6c4d14c308e930af2bfb0b703c361d4d05)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:19](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:21](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:24](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:20](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:22](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:23](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:9](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:24](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

| [sw/apps/addressability/addressability.c](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/apps/addressability/addressability.c#L24) | 24–42；完整文件45行 | 打开用户指定 device，mmap 4KiB SoC 寄存器，读 scratch/reset registers，munmap/close。没有计算 kernel、tile DMA、OpenMP target 或 SVA 映射。 |

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:23](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

| [同一 carfield.c](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/carfield.c#L174) | 174–207；完整文件456行 | 唯一 ioctl case `0x1` 用 `kmalloc` 与 `virt_to_phys` 返回连续物理地址；完整文件无 `iommu`/domain attach/IOVA map/unmap 调用。不能冒充论文所述 SVA driver。这里不作无关代码质量审查。 |

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:21](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

| [configs/buildroot64_defconfig](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/configs/buildroot64_defconfig#L22) | 22–29 | Linux 来源 stable/linux.git，固定 tag `v5.10.7`，应用本仓 `linux_patch/`。这与论文 TXT 138 的 patched Linux 6.9.0 不一致，不能静默当作论文软件版本。 |

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md:22](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-sdk-followup.md)：

| [carfield.c](https://github.com/pulp-platform/cva6-sdk/blob/98611e6c4d14c308e930af2bfb0b703c361d4d05/sw/drivers/carfield/carfield.c#L89) | 89–171 | `mmap` 选择 SoC/L2/cluster 的物理区域，设 noncached 后 `remap_pfn_range`。这是 CPU 映射硬件资源，不是给 accelerator 创建用户 buffer 的 IOVA mapping。 |

[返回总目录](../README.md)
