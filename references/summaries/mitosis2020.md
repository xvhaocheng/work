# Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines

稳定ID：`mitosis2020` · 类型：`research_paper` · 主题：`foundations`

作者：Reto Achermann; Ashish Panwar; Abhishek Bhattacharjee; Timothy Roscoe; Jayneel Gandhi；年份：2020；出处：ASPLOS '20。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：OS透明复制和迁移页表到多个NUMA socket；PV-Ops扩展页表分配/释放/写；循环链表定位副本；context switch按socket选择本地replica；accessed/dirty bits在副本间OR

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：多socket NUMA大内存机器；页表可复制且应用无感；Linux x86-64 PV-Ops可用；页表更新在critical section且TLB flush IPI保证一致性

限制：CPU NUMA OS层，不是GPU/IOMMU硬件翻译；无硬件TLB/PTW改动；虚拟化/nested paging留future work；mprotect开销>3x；页表更新低效；不覆盖GPU多设备SVA、远端PTW、队列/尾延迟

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf)
- [来源2](https://doi.org/10.1145/3373376.3378468)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/mitosis2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.pdf) · SHA-256 `9e9928282ee64ca25ce21670176d4f4bc4bf73b1630661de7f5de26e6d30e121`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:11672](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt:5](../../iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt:16](../../iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt:631](../../iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt:1718](../../iommu-platform/out/shared-translation-literature-20261006/papers/mitosis2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md:225](../../iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:27](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:74](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md:225](../../iommu-platform/out/shared-translation-literature-20261006/workers/G7_supplement.md)：

- DOI：`https://doi.org/10.1145/3373376.3378468`【mitosis2020.txt:L78】。

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:75](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 mitosis2020：PDF papers/mitosis2020.pdf; text papers/mitosis2020.txt; 报告 workers/G7_supplement.md; 证据卡 workers/G7_supplement-papers.json；reading=fulltext_worker_reviewed；status=published；year=2020；venue=ASPLOS '20；doi=10.1145/3373376.3378468；source=https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:56](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 mitosis2020：PDF papers/mitosis2020.pdf; text papers/mitosis2020.txt; 报告 workers/G7_supplement.md; 证据卡 workers/G7_supplement-papers.json；reading=fulltext_worker_reviewed；status=published；year=2020；venue=ASPLOS '20；doi=10.1145/3373376.3378468；source=https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:74](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **mitosis2020** Mitosis: Transparently Self-Replicating Page-Tables for Large-Memory Machines — Reto Achermann, Ashish Panwar, Abhishek Bhattacharjee, Timothy Roscoe, Jayneel Gandhi；ASPLOS '20；10.1145/3373376.3378468；source=https://apanwariisc.github.io/publications/asplos-2020-mitosis/mitosis-asplos20.pdf；topic=foundations；status=published；relevance=nearby；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
