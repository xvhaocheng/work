# AXI-ICRT: Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs

稳定ID：`axi_icrt2023` · 类型：`research_paper` · 主题：`axi-realtime`

作者：Jiang et al.（完整作者待核）；年份：2023；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：已核§4–6利用可选择事务队列及互连改造支持组合调度和服务界。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：B路径被视为可忽略；不能免费假定固定FIFO接口拥有同样选择能力，也不能声称其硬件已集成本平台。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：axi-realtime近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/axi_icrt2023.pdf) · canonical：[原路径](../../iommu-platform/out/research-reappraisal-20261015/analysis/adjacent/axi-icrt.pdf) · SHA-256 `adfc570ef617b51ab12908e26e30dcbbbedd5a1693073f04a4fdadb0084a1cfb`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-novelty-assessment-20261015.md:43](../../iommu-platform/docs/research-novelty-assessment-20261015.md)
- [iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md:12](../../iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md)
- [iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:20](../../iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md)
- [iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md:15](../../iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-novelty-assessment-20261015.md:43](../../iommu-platform/docs/research-novelty-assessment-20261015.md)：

| **AXI-ICRT，§4–6；AXI-REALM，§III，核至v2** | ICRT用可选择队列和互连改造支持组合调度，分析依赖事务服务界；其B路径视为可忽略。REALM已有分段、region预算、有限缓冲、收齐写数据后再发AW及阶段监测 | 固定接口上不能免费使用可随机选择队列；但这些是具名可付费方案。A需证明何时它们足够、何时某个旧义务或消费者依赖使目标仍不能达到。分开AW/W/B或增加consumer终点本身不是创新。[ICRT](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)；[REALM v2](https://arxiv.org/html/2501.10161v2) |

[iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md:15](../../iommu-platform/out/research-reappraisal-20261015/analysis/adjacent-assessment.md)：

Zhe Jiang、Kecheng Yang、Nathan Fisher、Ian Gray、Neil Audsley、Zheng Dong，[AXI-ICRT Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)。本地保存作者稿与文本；本轮不从URL文件名推断出版年份。直接核设计目标、§4架构与§6调度分析，未对完整实现及全部评估作重新验收。

[iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md:12](../../iommu-platform/out/control-position-method-20261015/literature/analysis-delta.md)：

|ICRT|Jiang 等，*AXI-ICRT: Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs*；[作者PDF](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)；同一 adjacent 目录 `axi-icrt.{pdf,txt}`。本次定向读§4.1–4.2、§6/§6.1及Table1；未重读全篇或验作者实现。|“each job consists of a batch of consecutive transactions”（§6，PDF7，txt L847–850）|

[iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:20](../../iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md)：

[Jiang等，AXI-ICRT: Towards a Real-Time AXI-Interconnect for Highly Integrated SoCs，作者原件](https://userweb.cs.txstate.edu/~k_y47/webpage/pubs/tc23c.pdf)。

[返回总目录](../README.md)
