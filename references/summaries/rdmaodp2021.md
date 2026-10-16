# Pitfalls of InfiniBand with On-Demand Paging

稳定ID：`rdmaodp2021` · 类型：`research_paper` · 主题：`io`

作者：Takuya Fukuoka; Shigeyuki Sato; Kenjiro Taura；年份：2021；出处：IEEE ISPASS 2021。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：实验揭示InfiniBand ODP在重传/超时下的packet damming和packet flood；根因是RC timeout、RNR NAK和RNIC页状态更新失败。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：InfiniBand RC、ODP、RNR NAK和重传机制按规范/硬件实现工作。；QP参数CACK、CRetry、minimal RNR NAK delay决定timeout和重传行为。；RNIC translation table、页状态和QP状态是ODP数据路径的关键状态。；client-side ODP与server-side ODP行为不同；client-side stateful导致packet flood更严重。

限制：不提出新ODP实现或硬件修复，只做实验分析和lessons learned。；部分集群无sudo/ibdump，只能推测packet damming/flood发生。；packet damming被vendor称源自ConnectX-4页缺页处理方法并在后续型号消失，但长timeout仍存在；packet flood在最新卡仍存在。；未覆盖IOMMU/IOTLB、远端PTW、SVA、通用队列模型和多租户生产尾延迟。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[term2024](term2024.md)：TeRM在参考文献中引用该文，并用其说明ODP缺页开销并非可忽略。；[odrp2025](odrp2025.md)：ODRP不使用ODP缺页，而用RDMA WR链在远端做交换页管理；二者都涉及RNIC但翻译/管理实体不同。

原始来源：

- [来源1](https://tkygtr6.github.io/pub/ISPASS21_paper.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/rdmaodp2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.pdf) · SHA-256 `17d3b34e5e89f71dd708e7c0eeff15db51312e9add3ec7d0604923713bd3fa0b`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:14889](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:9](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:122](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:189](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:263](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:408](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:479](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt:616](../../iommu-platform/out/shared-translation-literature-20261006/papers/rdmaodp2021.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:293](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 rdmaodp2021：PDF papers/rdmaodp2021.pdf; text papers/rdmaodp2021.txt; 报告 workers/G15_io_expansion.md; 证据卡 workers/G15_io_expansion-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=IEEE ISPASS 2021；doi=未知；source=https://tkygtr6.github.io/pub/ISPASS21_paper.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:292](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **rdmaodp2021** Pitfalls of InfiniBand with On-Demand Paging — Takuya Fukuoka, Shigeyuki Sato, Kenjiro Taura；IEEE ISPASS 2021；doi未知；source=https://tkygtr6.github.io/pub/ISPASS21_paper.pdf；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:266](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2021 / published | <a id="rdmaodp2021"></a>[Pitfalls of InfiniBand with On-Demand Paging](https://tkygtr6.github.io/pub/ISPASS21_paper.pdf) | rdmaodp2021 | rdmaodp2021 | core |

[返回总目录](../README.md)
