# pulp-platform/carfield/dbb4ad3380e6e81fa7eac3955e2340752f29dab0〔正式题名待整理〕

稳定ID：`code-pulp-platform-carfield-dbb4ad3380e6e81fa7eac3955e2340752f29dab0` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/.gitmodules)
- [来源2](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/Bender.yml#L28)
- [来源3](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/docs/um/sw.md#L214)
- [来源4](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv#L469)
- [来源5](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/boot/carfield_soc.dtsi#L154)
- [来源6](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/sw.mk#L182)
- [来源7](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/omptarget.c#L24)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:31](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:25](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:29](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:26](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:27](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:30](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)
- [iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:28](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:26](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)：

| [hw/cheshire_wrap.sv](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/hw/cheshire_wrap.sv#L469) | 469–529 | FPCluster master 不走其他 master 的 bypass；其事务接到 `riscv_iommu`，completion 回原 FPCluster 下游，DS/PTW 使用独立 IOMMU master。AW/AR stream_id=0、ss_id_valid=1、substream_id=0。这是实际硬件插入；具体 driver 配置是否使一次任务走 translated mode 仍需软件原件。 |

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:28](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)：

| [sw/tests/bare-metal/snitchd/omptarget.c](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/tests/bare-metal/snitchd/omptarget.c#L24) | 24–74 | mailbox 收 offloadFn/Args，执行 `offloadFn(offloadArgs)`，barrier 后 DONE。只提供 generic offload dispatch，没有 axpy/gemm 计算、DMA tile、IOVA mapping 或 SVA 模式开关。 |

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:27](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)：

| [sw/boot/carfield_soc.dtsi](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/sw/boot/carfield_soc.dtsi#L154) | 154–166 | `spatz-cluster@51000000` 具有 `iommus = <&riscv_iommu>`；IOMMU 节点地址 0x2000a000。设备树有绑定，但不能单独证明 domain attach/map/unmap 实际成功。 |

[iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md:31](../../iommu-platform/out/control-position-method-20261015/cases/enabled-sva-source-gap.md)：

| [.gitmodules](https://github.com/pulp-platform/carfield/blob/dbb4ad3380e6e81fa7eac3955e2340752f29dab0/.gitmodules) / sw git tree | 1–3 / `sw/deps/cva6-sdk` | 唯一 gitlink 指向 `pulp-platform/cva6-sdk` 的 `98611e6c4d14c308e930af2bfb0b703c361d4d05`。其内容未在本次有界链继续展开；不能说依赖中也没有 driver 或应用。 |

[返回总目录](../README.md)
