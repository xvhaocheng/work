# Memtis: Efficient Memory Tiering with Dynamic Page Classification and Page Size Determination

稳定ID：`memtis2023` · 类型：`research_paper` · 主题：`lifecycle`

作者：Taehyung Lee; Sumit Kumar Monga; Changwoo Min; Young Ik Eom；年份：2023；出处：ACM SIGOPS 29th Symposium on Operating Systems Principles (SOSP ’23)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用PEBS采样页/子页访问，指数分桶维护访问直方图，动态定hot/warm/cold阈值并后台迁移；按子页访问倾斜动态split huge page。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：Linux OS内存管理，v5.15.19；PEBS可采样LLC miss和store指令；huge page内4KB subpage可跟踪；fast tier/capacity tier可迁移页；单socket避免NUMA效应

限制：PEBS对rarely/never accessed页区分不足；huge page split涉及copy和TLB shootdown，开销高；CXL为emulated remote NUMA，不是真实CXL硬件；未覆盖IOMMU/SVA、多设备共享页表、远端PTW；单socket评估，未覆盖NUMA与多租户

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[pond2023](pond2023.md)：actual_citation

原始来源：

- [来源1](https://vtechworks.lib.vt.edu/bitstreams/579b2d19-7c70-44e9-b2c5-53c248a03bb6/download)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/memtis2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.pdf) · SHA-256 `1e0aef7ce83579c00cfa2dc72591c2b6f925803fca0aa052c36253250a51764c`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:11123](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt:132](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt:353](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt:533](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt:645](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt:932](../../iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:410](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:461](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 memtis2023：PDF papers/memtis2023.pdf; text papers/memtis2023.txt; 报告 workers/G13_tiering.md; 证据卡 workers/G13_tiering-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=ACM SIGOPS 29th Symposium on Operating Systems Principles (SOSP ’23)；doi=10.1145/3600006.3613167；source=https://vtechworks.lib.vt.edu/bitstreams/579b2d19-7c70-44e9-b2c5-53c248a03bb6/download；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:460](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **memtis2023** Memtis: Efficient Memory Tiering with Dynamic Page Classification and Page Size Determination — Taehyung Lee, Sumit Kumar Monga, Changwoo Min, Young Ik Eom；ACM SIGOPS 29th Symposium on Operating Systems Principles (SOSP ’23)；10.1145/3600006.3613167；source=https://vtechworks.lib.vt.edu/bitstreams/579b2d19-7c70-44e9-b2c5-53c248a03bb6/download；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:410](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2023 / published | <a id="memtis2023"></a>[Memtis: Efficient Memory Tiering with Dynamic Page Classification and Page Size Determination](https://vtechworks.lib.vt.edu/bitstreams/579b2d19-7c70-44e9-b2c5-53c248a03bb6/download) | memtis2023 | memtis2023 | core |

[返回总目录](../README.md)
