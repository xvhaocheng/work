# SP-IMPact: A Framework for Static Partitioning Interference Mitigation and Performance Analysis

稳定ID：`spimpact2025` · 类型：`research_paper` · 主题：`io`

作者：Diogo Costa; Gonçalo Moreira; Afonso Oliveira; José Martins; Sandro Pinto；年份：2025；出处：NG-RES 2025 (OASIcs)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：静态分区hypervisor下的干扰分析框架：生成guest、cache coloring和memory bandwidth reservation配置，自动部署与PMU采集，分析LLC和system bus竞争

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：静态分区hypervisor/Bao；ARM Cortex-A53与PMU可用；cache coloring与MBR可作为缓解手段；VM间spatial isolation已由hypervisor提供

限制：不是IOMMU专门缓解方案；当前评估主要覆盖LLC与bus，IOMMU/interrupt controller仅被指出为缺口；评估未包含MBR配置；仅3个MiBench基准；无GPU、无地址翻译、无page walk；未给队列/尾延迟/多设备SVA分析

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：io主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2501.16245)
- [来源2](https://arxiv.org/html/2501.16245v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/spimpact2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.pdf) · SHA-256 `d1aa0590d95b2f7ee63700565b75b12a26b077d8373aaefa1754609de94fb626`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:17908](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt:38](../../iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt:110](../../iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt:384](../../iommu-platform/out/shared-translation-literature-20261006/papers/spimpact2025.txt)
- [iommu-platform/docs/paper-route-five-round-review-20261004.md:90](../../iommu-platform/docs/paper-route-five-round-review-20261004.md)
- [iommu-platform/out/composition-v1-delivery-20261004/RESEARCH_REVIEW.md:90](../../iommu-platform/out/composition-v1-delivery-20261004/RESEARCH_REVIEW.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:272](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-route-five-round-review-20261004.md:90](../../iommu-platform/docs/paper-route-five-round-review-20261004.md)：

- [SP-IMPact作者全文](https://arxiv.org/html/2501.16245v1)：已有cache coloring/内存限流配置及交互分析框架。其展示评价明确未纳入MBR，不能说已完整实测本课题的所有交互。它削弱“交互分析本身新颖”的说法，不证明当前主方案已经被否证。

[iommu-platform/out/composition-v1-delivery-20261004/RESEARCH_REVIEW.md:90](../../iommu-platform/out/composition-v1-delivery-20261004/RESEARCH_REVIEW.md)：

- [SP-IMPact作者全文](https://arxiv.org/html/2501.16245v1)：已有cache coloring/内存限流配置及交互分析框架。其展示评价明确未纳入MBR，不能说已完整实测本课题的所有交互。它削弱“交互分析本身新颖”的说法，不证明当前主方案已经被否证。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:305](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 spimpact2025：PDF papers/spimpact2025.pdf; text papers/spimpact2025.txt; 报告 workers/G7_supplement.md; 证据卡 workers/G7_supplement-papers.json；reading=fulltext_worker_reviewed；status=published；year=2025；venue=NG-RES 2025 (OASIcs)；doi=10.4230/OASIcs.NG-RES.2025.5；source=https://arxiv.org/pdf/2501.16245；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:203](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 spimpact2025：PDF papers/spimpact2025.pdf; text papers/spimpact2025.txt; 报告 workers/G7_supplement.md; 证据卡 workers/G7_supplement-papers.json；reading=fulltext_worker_reviewed；status=published；year=2025；venue=NG-RES 2025 (OASIcs)；doi=10.4230/OASIcs.NG-RES.2025.2；source=https://arxiv.org/pdf/2501.16245；root_spotchecked=False

[返回总目录](../README.md)
