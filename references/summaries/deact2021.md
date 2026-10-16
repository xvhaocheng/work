# DeACT: Architecture-Aware Virtual Memory Support for Fabric Attached Memory Systems

稳定ID：`deact2021` · 类型：`research_paper` · 主题：`accelerators`

作者：Vamsee Reddy Kommareddy; Clayton Hughes; Simon David Hammond; Amro Awad；年份：2021；出处：HPCA 2021。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：FAM 系统中把 access control metadata 与 node PA→FAM PA 翻译解耦；translation 可在 node local DRAM 做 unverified cache，ACM 由 off-node STU 缓存和验证。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：node OS 管理假想 flat node physical memory；memory 与 fabric 可信；compute node 同一时间单用户；不允许同节点 co-locating 资源；shared FAM pages 用 1GB 粒度 bitmap 授权；DeACT 只应用于最后一级 PTE

限制：无硬件原型，仅 SST simulation；不覆盖 timing/covert/physical attacks；page migration invalidation 只讨论未完整实测；DeACT 对部分 benchmark 退化或不改善；shared page 1GB 粒度有存储/共享 trade-off；Sandia官方题录核定；全文为SAND2020-14055C作者稿，不能以报告号2020替代会议年份。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[sparta2020](sparta2020.md)：两者都把部分翻译移到 memory side，但 DeACT 面向 FAM node PA→FAM PA 并强调 access control；SPARTA 面向 accelerator VA→PA 且保留 per-page protection。

原始来源：

- [来源1](https://www.osti.gov/servlets/purl/1837344)
- [来源2](https://www.sandia.gov/research/publications/details/deact-architecture-aware-virtual-memory-support-for-fabric-attached-memory-2021-02-01/)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/deact2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.pdf) · SHA-256 `6fc3ae381170370f632fb92416351329aac52621d5b1ef49878059b768e16da7`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:3926](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt:14](../../iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt:101](../../iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt:312](../../iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt:430](../../iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt:906](../../iommu-platform/out/shared-translation-literature-20261006/papers/deact2021.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:321](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:357](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:357](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **deact2021** DeACT: Architecture-Aware Virtual Memory Support for Fabric Attached Memory Systems — Vamsee Reddy Kommareddy, Clayton Hughes, Simon David Hammond, Amro Awad；HPCA 2021；10.1109/HPCA51647.2021.00046；source=https://www.osti.gov/servlets/purl/1837344；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://www.sandia.gov/research/publications/details/deact-architecture-aware-virtual-memory-support-for-fabric-attached-memory-2021-02-01/

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:359](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 deact2021：PDF papers/deact2021.pdf; text papers/deact2021.txt; 报告 workers/G10_nearmemory.md; 证据卡 workers/G10_nearmemory-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=HPCA 2021；doi=10.1109/HPCA51647.2021.00046；source=https://www.osti.gov/servlets/purl/1837344；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:321](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2021 / published | <a id="deact2021"></a>[DeACT: Architecture-Aware Virtual Memory Support for Fabric Attached Memory Systems](https://www.osti.gov/servlets/purl/1837344) | deact2021 | deact2021 | core |

[返回总目录](../README.md)
