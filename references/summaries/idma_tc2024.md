# A High-Performance, Energy-Efficient Modular DMA Engine Architecture

稳定ID：`idma_tc2024` · 类型：`research_paper` · 主题：`dma-dataflow`

作者：Benz et al.（完整作者待核）；年份：2024；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：模块化DMA具有流式小FIFO与可替换整次传输缓冲的设计空间；已核§II-C/p267。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：不由缓冲变化推出本项目IOMMU机制或性能创新。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：dma-dataflow近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/idma_tc2024.pdf) · canonical：[原路径](../../iommu-platform/out/paper-driven-innovation-20261008/papers/idma2024.pdf) · SHA-256 `9a96b35750a2ea463630fe9d4b9658f09d8a4556a05f9f1deb52d6c33a228bb8`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/composition-history-isolation-20261007.md:448](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/paper-driven-innovation-20261008.md:23](../../iommu-platform/docs/paper-driven-innovation-20261008.md)
- [iommu-platform/docs/research-literature-review-20261004.md:89](../../iommu-platform/docs/research-literature-review-20261004.md)
- [iommu-platform/out/completion-reserve-late-20261007/codex-review.md:50](../../iommu-platform/out/completion-reserve-late-20261007/codex-review.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/composition-history-isolation-20261007.md:448](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

活跃DMA payload：原8192B、两槽4144B、单槽2096B、无池48B（两3拍FIFO）。此数字排除共同IOMMU W FIFO、元数据、响应字段和控制，未综合面积/时序。iDMA已有小FIFO流式及可替换整缓冲，[原论文§II-C](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)；Arm DMA350完整响应容量和CTC消息依赖/接收信用均是强前作，不能凭变小/换位置宣布创新。完整设计、受限推演、表及原源。

[iommu-platform/docs/research-literature-review-20261004.md:89](../../iommu-platform/docs/research-literature-review-20261004.md)：

- Benz等，A High-Performance, Energy-Efficient Modular DMA Engine Architecture，IEEE TC73(1)，2024，doi10.1109/TC.2023.3329930。第267页说明某些系统/存储端点需要完整缓冲，可用SRAM存下整次传输；因此“增大数据缓冲”不是新创新。原文：https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf 。

[iommu-platform/docs/paper-driven-innovation-20261008.md:23](../../iommu-platform/docs/paper-driven-innovation-20261008.md)：

| [iDMA，IEEE TC 2024](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)，§II-C | 源/目的合法化分段、byte-stream transport、小 FIFO；必要时完整传输缓冲 | 读写解耦、直通、小 FIFO/大 SRAM 取舍已明确讨论 |

[iommu-platform/out/completion-reserve-late-20261007/codex-review.md:50](../../iommu-platform/out/completion-reserve-late-20261007/codex-review.md)：

[iDMA原论文 §II-C](https://spcl.inf.ethz.ch/Publications/.pdf/A_High-Performance_Energy-Efficient_Modular_DMA_Engine_Architecture.pdf)已有小FIFO流式数据通路及按系统替换为完整缓冲；[Arm DMA350 §4.3.4](https://documentation-service.arm.com/static/63ebf4d18e13d11599a936f2)使用完整响应空间的读准入；[CTC §3–4](https://www.cs.columbia.edu/~luca/research/ctc_NOCS09.pdf)已研究接收容量与消息依赖。它们支持相关原语有前史，不证明当前双写队列规则已被精确覆盖或新颖。

[返回总目录](../README.md)
