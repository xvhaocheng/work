# IOTLB-SC: An Accelerator-Independent Leakage Source in Modern Cloud Systems

稳定ID：`iotlb_sc2023` · 类型：`research_paper` · 主题：`security`

作者：Tiemann et al.（完整作者待核）；年份：2023；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：共享IOTLB外设间侧信道研究；已核§7含set/way分区讨论。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：安全泄漏与本项目性能/完整资源成本问题不同，跨设备驱逐或分区概念本身已有。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：security近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://arxiv.org/pdf/2202.11623)
- [来源2](https://doi.org/10.1145/3579856.3582838)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/paper-scenarios-mitigation.md:165](../../iommu-platform/docs/paper-scenarios-mitigation.md)
- [iommu-platform/out/paper-council-20261005/round2/ALL.md:246](../../iommu-platform/out/paper-council-20261005/round2/ALL.md)
- [iommu-platform/out/paper-council-20261005/round2/C2-literature.md:34](../../iommu-platform/out/paper-council-20261005/round2/C2-literature.md)
- [iommu-platform/out/paper-council-20261005/round2/C2.md:93](../../iommu-platform/out/paper-council-20261005/round2/C2.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-scenarios-mitigation.md:165](../../iommu-platform/docs/paper-scenarios-mitigation.md)：

| Tiemann 等，ASIA CCS 2023，*IOTLB-SC: An Accelerator-Independent Leakage Source in Modern Cloud Systems*：[作者 PDF](https://arxiv.org/pdf/2202.11623)、[正式 DOI](https://doi.org/10.1145/3579856.3582838) | 研究共享 IOTLB 的外设间侧信道；第 7 节已讨论 set/way 分区。不能把跨设备驱逐或分区概念当原创；安全泄漏研究与本文拟做的性能/代价评价需区分。 |

[iommu-platform/out/paper-council-20261005/round2/ALL.md:246](../../iommu-platform/out/paper-council-20261005/round2/ALL.md)：

**侧信道。** [IOTLB-SC 原文](https://arxiv.org/pdf/2202.11623) 已有不同虚拟机中 GPU 与 FPGA 的 peripheral-to-peripheral IOTLB 时序侧信道，攻击由用户态设备侧 DMA 和计时实现。因此泛称“发现 IOMMU 设备间时序信道”已无新颖性。A1 的“容量分区后仍存在 PTW/控制器通道”不因这一原文就自动被精确否定，但必须建立新的残余资源、威胁能力或防护边界。

[iommu-platform/out/paper-council-20261005/round2/C2.md:93](../../iommu-platform/out/paper-council-20261005/round2/C2.md)：

**侧信道。** [IOTLB-SC 原文](https://arxiv.org/pdf/2202.11623) 已有不同虚拟机中 GPU 与 FPGA 的 peripheral-to-peripheral IOTLB 时序侧信道，攻击由用户态设备侧 DMA 和计时实现。因此泛称“发现 IOMMU 设备间时序信道”已无新颖性。A1 的“容量分区后仍存在 PTW/控制器通道”不因这一原文就自动被精确否定，但必须建立新的残余资源、威胁能力或防护边界。

[iommu-platform/out/paper-council-20261005/round2/C2-literature.md:34](../../iommu-platform/out/paper-council-20261005/round2/C2-literature.md)：

- [作者 arXiv 原文](https://arxiv.org/pdf/2202.11623)，14 页。

[返回总目录](../README.md)
