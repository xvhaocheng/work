# Scalable and Effective Page-table and TLB management on NUMA Systems

稳定ID：`hydra2024` · 类型：`research_paper` · 主题：`lifecycle`

作者：Bin Gao; Qingxuan Kang; Hao-Wei Tee; Kyle Timothy Ng Chu; Alireza Sanaee; Djordje Jevdjic；年份：2024；出处：2024 USENIX Annual Technical Conference (ATC '24)。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：Hydra 对 NUMA 页表做 lazily、partial、on-demand per-PTE 复制：VMA 有 owner，page-table 有 sharer circular list；只更新 sharer list 中的副本，并利用“无副本则无本地 TLB 条目”不变量过滤 TLB shootdown。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：每个 VMA 有一个 owner socket，owner 必持有有效 PTE；page-table sharer list 可精确维护，且能定位至少一个副本；没有某 socket 的 page-table 副本，则该 socket 不可能有对应 PTE 的 TLB 条目；首次访问缺 PTE 的远端 PTW 惩罚可由后续本地访问摊薄

限制：THP 非优先，未实现；虚拟化 nested page table 留未来工作；intra-socket shootdown 未解决；极端共享时 Hydra 收敛到 Mitosis；首次访问仍有远端 PTW，需要 prefetch 缓解；不涉及设备 DMA、IOMMU、SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[mitosis2020](mitosis2020.md)：actual_citation；[radvm2013](radvm2013.md)：actual_citation；[latr2018](latr2018.md)：actual_citation；[fpr2024](fpr2024.md)：mechanism_comparison

原始来源：

- [来源1](https://www.usenix.org/system/files/atc24-gao-bin-scalable.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/hydra2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.pdf) · SHA-256 `3130e48b7d21e63383423efd62411f46a97adb6e0c02fbf303ba4461d02b0526`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:7750](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:23](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:32](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:260](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:386](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:498](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:545](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt:791](../../iommu-platform/out/shared-translation-literature-20261006/papers/hydra2024.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:471](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 hydra2024：PDF papers/hydra2024.pdf; text papers/hydra2024.txt; 报告 workers/G12_update.md; 证据卡 workers/G12_update-papers.json；reading=fulltext_worker_reviewed；status=published；year=2024；venue=2024 USENIX Annual Technical Conference (ATC '24)；doi=未知；source=https://www.usenix.org/system/files/atc24-gao-bin-scalable.pdf；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:470](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **hydra2024** Scalable and Effective Page-table and TLB management on NUMA Systems — Bin Gao, Qingxuan Kang, Hao-Wei Tee, Kyle Timothy Ng Chu, Alireza Sanaee, Djordje Jevdjic；2024 USENIX Annual Technical Conference (ATC '24)；doi未知；source=https://www.usenix.org/system/files/atc24-gao-bin-scalable.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:415](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2024 / published | <a id="hydra2024"></a>[Scalable and Effective Page-table and TLB management on NUMA Systems](https://www.usenix.org/system/files/atc24-gao-bin-scalable.pdf) | hydra2024 | hydra2024 | core |

[返回总目录](../README.md)
