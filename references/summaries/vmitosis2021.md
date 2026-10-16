# Fast Local Page-Tables for Virtualized NUMA Servers with vMitosis

稳定ID：`vmitosis2021` · 类型：`research_paper` · 主题：`foundations`

作者：Ashish Panwar; Reto Achermann; Arkaprava Basu; Abhishek Bhattacharjee; K. Gopinath; Jayneel Gandhi；年份：2021；出处：ASPLOS '21 (Virtual, USA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：对gPT与ePT做迁移与复制，使2D page walk本地化；Thin用迁移（随数据页迁移增量传播），Wide用复制（gPT每vCPU group一份本地副本，ePT每socket一份副本）；支持NUMA-visible/oblivious（NO-P/NO-F）。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：服务器NUMA拓扑可被guest或hypervisor利用/探测；page table可迁移或复制；gPT对hypervisor而言是普通guest数据页；ePT被hypervisor固定；AutoNUMA可用于驱动迁移

限制：特定4-socket Cascade Lake服务器；不涉及设备/IOMMU/SVA；shadow paging经验混合，最坏>5x退化；迁移/复制有开销，mprotect复制路径开销随region增大

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/vmitosis2021.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/vmitosis2021.pdf) · SHA-256 `9fdf5c37f77938a839b2fbcf78c988f69a6a044f56bab172e9d4ffa55d2da689`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:21331](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:29](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:78](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:79](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:24](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:57](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:58](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:79](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 vmitosis2021：PDF papers/vmitosis2021.pdf; text papers/vmitosis2021.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=ASPLOS '21 (Virtual, USA)；doi=10.1145/3445814.3446709；source=https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:58](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 vmitosis2021：PDF papers/vmitosis2021.pdf; text papers/vmitosis2021.txt; 报告 workers/G5_foundations.md; 证据卡 workers/G5_foundations-papers.json；reading=fulltext_worker_reviewed；status=published；year=2021；venue=ASPLOS '21 (Virtual, USA)；doi=10.1145/3445814.3446709；source=https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:78](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **vmitosis2021** Fast Local Page-Tables for Virtualized NUMA Servers with vMitosis — Ashish Panwar, Reto Achermann, Arkaprava Basu, Abhishek Bhattacharjee, K. Gopinath, Jayneel Gandhi；ASPLOS '21 (Virtual, USA)；10.1145/3445814.3446709；source=https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:57](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **vmitosis2021** Fast Local Page-Tables for Virtualized NUMA Servers with vMitosis — Ashish Panwar, Reto Achermann, Arkaprava Basu, Abhishek Bhattacharjee, K. Gopinath, Jayneel Gandhi；ASPLOS '21 (Virtual, USA)；10.1145/3445814.3446709；source=https://www.csa.iisc.ac.in/~arkapravab/papers/asplos21_vMitosis.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
