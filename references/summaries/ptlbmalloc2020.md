# Ptlbmalloc2: Reducing TLB Shootdowns with High Memory Efficiency

稳定ID：`ptlbmalloc2020` · 类型：`research_paper` · 主题：`lifecycle`

作者：Stijn Schildermans; Jianchen Shan; Xiaoning Ding; Kris Aerts；年份：2020；出处：2020 IEEE ISPA/BDCloud/SocialCom/SustainCom。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在 glibc 之上实现分配器库 ptlbmalloc2，用 global hysteresis 看应用全局内存状态决定 arena trim，减少 arena imbalance 触发的 mmap/munmap/madvise TLB shootdown。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：TLB shootdown 开销可由 CPU 数和 NUMA/虚拟化状态估算；应用内存变化可由全局 arena 累计状态评估；分配器可在 glibc 之上插入并管理 arena/trim 策略

限制：平均性能提升有限，多数场景 0-7%；global hysteresis 绑定 legacy 分配器设计，未集成进 glibc 本体导致锁竞争；只针对 arena imbalance 类 shootdown；不涉及设备 DMA、IOMMU、SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[shootdown1989](shootdown1989.md)：actual_citation；[latr2018](latr2018.md)：actual_citation；[shoot4u2016](shoot4u2016.md)：actual_citation；[radvm2013](radvm2013.md)：actual_citation；[fpr2024](fpr2024.md)：mechanism_comparison

原始来源：

- [来源1](https://web.njit.edu/~dingxn/papers/ispa20.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/ptlbmalloc2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.pdf) · SHA-256 `5ee324e96376eaed72bdcad9ba07fc665fe04eea271caaea42b561cad0c5b3df`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:14524](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt:19](../../iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt:155](../../iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt:231](../../iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt:299](../../iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt:675](../../iommu-platform/out/shared-translation-literature-20261006/papers/ptlbmalloc2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:406](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:452](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:453](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 ptlbmalloc2020：PDF papers/ptlbmalloc2020.pdf; text papers/ptlbmalloc2020.txt; 报告 workers/G12_update.md; 证据卡 workers/G12_update-papers.json；reading=fulltext_worker_reviewed；status=published；year=2020；venue=2020 IEEE ISPA/BDCloud/SocialCom/SustainCom；doi=10.1109/ISPA-BDCloud-SocialCom-SustainCom51426.2020.00036；source=https://web.njit.edu/~dingxn/papers/ispa20.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:452](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **ptlbmalloc2020** Ptlbmalloc2: Reducing TLB Shootdowns with High Memory Efficiency — Stijn Schildermans, Jianchen Shan, Xiaoning Ding, Kris Aerts；2020 IEEE ISPA/BDCloud/SocialCom/SustainCom；10.1109/ISPA-BDCloud-SocialCom-SustainCom51426.2020.00036；source=https://web.njit.edu/~dingxn/papers/ispa20.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:406](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2020 / published | <a id="ptlbmalloc2020"></a>[Ptlbmalloc2: Reducing TLB Shootdowns with High Memory Efficiency](https://web.njit.edu/~dingxn/papers/ispa20.pdf) | ptlbmalloc2020 | ptlbmalloc2020 | core |

[返回总目录](../README.md)
