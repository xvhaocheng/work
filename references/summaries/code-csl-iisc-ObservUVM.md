# csl-iisc/ObservUVM〔正式题名待整理〕

稳定ID：`code-csl-iisc-ObservUVM` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/csl-iisc/ObservUVM)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:40](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/out/independent-directions-20261015/methods/assessment.md:45](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/independent-research-directions-20261015.md:40](../../iommu-platform/docs/independent-research-directions-20261015.md)：

| [HELM，SC2025](https://doi.org/10.1145/3712285.3759812)，已取得的§2/§5.3/§6索引正文；[ObservUVM，ISCA2026](https://github.com/csl-iisc/ObservUVM)，§IV/VI | 可解释遥测指导策略；ObservUVM还已有付费采样、反馈选择及在线收益 | “更少观测→更好策略”宽表述已不新。需给出共享翻译反馈中具体缺失关系、获取时机及净选择收益。HELM全文未取得，不据部分正文宣布其全部边界 |

[iommu-platform/out/independent-directions-20261015/methods/assessment.md:45](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)：

| [ObservUVM，ISCA2026公开原件/实现入口](https://github.com/csl-iisc/ObservUVM) | §IV把样本页移到DRAM并复用access counter，付费取得关键HBM region观测；§VI的Tournament按回访fault归责并淘汰策略，已有反馈选择和负反馈预取。txt:368、524、566 | 采样+现有硬件+观测指导策略并非新。目标是显存超额订阅/evict/prefetch；D需要在共享翻译服务中给与该机制不同的具体信息/行动关系及净值。 |

[返回总目录](../README.md)
