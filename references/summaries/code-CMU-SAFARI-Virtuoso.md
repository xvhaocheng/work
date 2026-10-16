# CMU-SAFARI/Virtuoso〔正式题名待整理〕

稳定ID：`code-CMU-SAFARI-Virtuoso` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/CMU-SAFARI/Virtuoso)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/shared-translation-gaps-20261006.md:19](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md:120](../../iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-translation-gaps-20261006.md:19](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

| [Virtuoso](https://github.com/CMU-SAFARI/Virtuoso) | ASPLOS 2025，已取作者稿；方法细节待读 | 需要把虚拟内存的软件管理成本带入评估；这是方法/工具补项，不是已运行的实验。 |

[iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md:120](../../iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)：

| PTW／数据物理放置、bank 着色 | Mitosis、MGvm、CLAP 已显式安排 PT／data 布局；[Phoenix 作者稿](https://arxiv.org/abs/2502.10923)还联合 scheduler、PT placement 与带宽 QoS；[Virtuoso 官方代码](https://github.com/CMU-SAFARI/Virtuoso)及 ASPLOS2025讲稿已显式纳入 PT／data DRAM 干扰。普通 per-stream arena／PT pool 着色／复制尚未被排除，所以不能把“把页表搬到空闲 bank”立新。现有 FIFO DDR 模型也没有实际 bank/row 调度，直接沿用旧 DUT 不能验该主张。 |

[返回总目录](../README.md)
