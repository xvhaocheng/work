# A Case for Granularity Aware Page Migration

稳定ID：`granularity2018` · 类型：`research_paper` · 主题：`lifecycle`

作者：Jee Ho Ryoo; Lizy K. John; Arkaprava Basu；年份：2018；出处：ICS '18。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：动态选择 4KB/64KB/2MB 迁移粒度，以连续虚拟内存区间为单位，用 PTE access bits 推断空间局部性并在 move-in/move-out 调整。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：4KB base page；OS-directed migration；fast memory 400MB；slow PCM 模拟；AMD IBS 测 FM-hit

限制：PCM 为模拟；runtime 为 leading-load 模型投影；动态阈值需调参；不处理设备共享/权限；不依赖 huge pages

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/granularity2018.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.pdf) · SHA-256 `786cf6f813c43cad0bf667aa8b20d7f8bc411fcd6aeaf248611a8831f9a617c9`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:6586](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt:12](../../iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt:70](../../iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt:325](../../iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt:418](../../iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:402](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:444](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:445](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:445](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 granularity2018：PDF papers/granularity2018.pdf; text papers/granularity2018.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2018；venue=ICS '18；doi=10.1145/3205289.3208064；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:276](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 granularity2018：PDF papers/granularity2018.pdf; text papers/granularity2018.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2018；venue=ICS '18；doi=10.1145/3205289.3208064；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:444](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **granularity2018** A Case for Granularity Aware Page Migration — Jee Ho Ryoo, Lizy K. John, Arkaprava Basu；ICS '18；10.1145/3205289.3208064；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:275](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **granularity2018** A Case for Granularity Aware Page Migration — Jee Ho Ryoo, Lizy K. John, Arkaprava Basu；ICS '18；10.1145/3205289.3208064；source=https://www.csa.iisc.ac.in/~arkapravab/papers/ics_18_granularity_aware_migration.pdf；topic=lifecycle；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
