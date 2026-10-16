# To Cross, or Not to Cross Pages for Prefetching?

稳定ID：`cross2025` · 类型：`research_paper` · 主题：`foundations`

作者：Georgios Vavouliotis; Marti Torrents; Boris Grot; Kleovoulos Kalaitzidis; Leeor Peled; Marc Casas；年份：2025；出处：HPCA 2025。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：MOKA框架设计Page-Cross Filter：hashed perceptron(program features)+system features计数器(sTLB MPKI/Miss Rate)+vUB/pUB训练+epoch自适应阈值；实例化DRIPPER用于Berti/IPCP/BOP，决定是否发出page-cross预取。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：L1D使用VIPT，prefetcher可直接访问TLB层级；page-cross可能触发speculative page walk；有用性定义为在cache生命周期内至少一次命中；DRIPPER硬件开销约1.44KB/核

限制：仿真，无实机；不涉及设备/IOMMU/SVA；未覆盖page walk队列/tail latency；以4KB页为主，2MB仅作敏感度

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/cross2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/cross2025.pdf) · SHA-256 `1bb2ee7e8c9257c81d7400530163d835ab50a336258e0f2b4c5d9f1f1db28dbf`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:2960](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:36](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:94](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:95](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:27](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:63](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:64](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:95](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 cross2025：PDF papers/cross2025.pdf; text papers/cross2025.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2025；venue=HPCA 2025；doi=未知；source=https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:64](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 cross2025：PDF papers/cross2025.pdf; text papers/cross2025.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2025；venue=HPCA 2025；doi=未知；source=https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:94](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **cross2025** To Cross, or Not to Cross Pages for Prefetching? — Georgios Vavouliotis, Marti Torrents, Boris Grot, Kleovoulos Kalaitzidis, Leeor Peled, Marc Casas；HPCA 2025；doi未知；source=https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:63](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **cross2025** To Cross, or Not to Cross Pages for Prefetching? — Georgios Vavouliotis, Marti Torrents, Boris Grot, Kleovoulos Kalaitzidis, Leeor Peled, Marc Casas；HPCA 2025；doi未知；source=https://ease-lab.github.io/ease_website/pubs/CROSS_HPCA25.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
