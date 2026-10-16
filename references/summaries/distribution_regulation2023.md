# Memory Latency Distribution-Driven Regulation for Temporal Isolation in MPSoCs

稳定ID：`distribution_regulation2023` · 类型：`research_paper` · 主题：`resource-management`

作者：Saeed et al.（完整作者待核）；年份：2023；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：用互连PMU观测请求时延CDF，并通过调节强制一阶随机占优；已有COTS MPSoC实现报告。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：项目仅核摘要/算法片段，未逐页精读；不涉及共享IOMMU多DMA服务。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：resource-management近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://cs-people.bu.edu/rmancuso/files/papers/DistributionRegulation_ECRTS23.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:18](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:18](../../iommu-platform/docs/research-literature-review-20261004.md)：

| Memory Latency Distribution-Driven Regulation for Temporal Isolation in MPSoCs（Saeed 等；2023；ECRTS 2023，非 RTSS2022） | https://cs-people.bu.edu/rmancuso/files/papers/DistributionRegulation_ECRTS23.pdf | `out/method88-literature-20261004/latency-primary.json`（作者 PDF 摘要/算法片段；本次未逐页全文核对）；DOI 10.4230/LIPIcs.ECRTS.2023.4 | 提出 distribution-driven regulation；用 interconnect-level PMU 观察 per-request memory latency CDF；CDF 反馈触发调节以强制一阶随机占优；COTS Xilinx Ultrascale+ MPSoC 上 full-stack 实现；摘要称 NRT 吞吐约 2.2× 于 DRAM bandwidth management 类方法 | 不涉及 IOMMU/共享翻译服务；不是共享 IOMMU 多 DMA 流保护；未逐页核对本课题接口 | 重叠：latency CDF 反馈限流已存在；仅把平均吞吐反馈改成 CDF 不自动新颖。区别：DRAM 带宽调节 vs IOMMU 共享翻译服务，须具体规则证据 |

[返回总目录](../README.md)
