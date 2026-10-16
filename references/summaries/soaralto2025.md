# Tiered Memory Management Beyond Hotness

稳定ID：`soaralto2025` · 类型：`research_paper` · 主题：`lifecycle`

作者：Jinshu Liu; Hamid Hadian; Hanchen Xu; Huaicheng Li；年份：2025；出处：19th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’25)。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：AOL=Latency/MLP结合CPU stalls；Soar离线对象级性能贡献排序后静态放置；Alto按AOL动态调节页迁移强度。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：host PMU可读LLC stalls、offcore、cycles；AOL的K=f(AOL)可由微基准离线校准；对象按调用链分组可代表访问模式；Soar接受一次fast-tier profiling run；Alto可叠加到TPP/NBT/Nomad/Colloid

限制：Soar离线object-level，页面/对象内criticality可变时粒度不匹配；Alto阈值为离线经验值，带宽竞争下AOL膨胀需调阈值；部分workload负收益，5/182 cases；未覆盖IOMMU/SVA、远端PTW、多设备共享页表

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[memtis2023](memtis2023.md)：actual_citation；[nomad2024](nomad2024.md)：actual_citation；[pond2023](pond2023.md)：actual_citation

原始来源：

- [来源1](https://www.usenix.org/system/files/osdi25-liu.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/soaralto2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.pdf) · SHA-256 `e30fc068d4be593fbccc172adab544ccb9a3a8fce17a6793d2a8ef236b836ba4`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:16955](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt:29](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt:153](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt:290](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt:369](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt:563](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt:1052](../../iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt)
- [iommu-platform/docs/paper-driven-innovation-20261008.md:26](../../iommu-platform/docs/paper-driven-innovation-20261008.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/paper-driven-innovation-20261008.md:26](../../iommu-platform/docs/paper-driven-innovation-20261008.md)：

| [Soar/Alto，OSDI 2025](https://www.usenix.org/system/files/osdi25-liu.pdf)，§2/设计 | 分层 CPU 内存，访存关键性还受延迟与 MLP 影响 | miss 数/热度不等于端到端收益；不是 DMA 缓冲规则的直接前作 |

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:485](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 soaralto2025：PDF papers/soaralto2025.pdf; text papers/soaralto2025.txt; 报告 workers/G13_tiering.md; 证据卡 workers/G13_tiering-papers.json；reading=fulltext_worker_reviewed；status=published；year=2025；venue=19th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’25)；doi=未知；source=https://www.usenix.org/system/files/osdi25-liu.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:484](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **soaralto2025** Tiered Memory Management Beyond Hotness — Jinshu Liu, Hamid Hadian, Hanchen Xu, Huaicheng Li；19th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’25)；doi未知；source=https://www.usenix.org/system/files/osdi25-liu.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:420](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2025 / published | <a id="soaralto2025"></a>[Tiered Memory Management Beyond Hotness](https://www.usenix.org/system/files/osdi25-liu.pdf) | soaralto2025 | soaralto2025 | core |

[返回总目录](../README.md)
