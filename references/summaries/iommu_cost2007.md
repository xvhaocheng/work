# The Price of Safety: Evaluating IOMMU Performance

稳定ID：`iommu_cost2007` · 类型：`research_paper` · 主题：`io`

作者：Muli Ben-Yehuda; Jimi Xenidis; Michal Ostrowski; Karl Rister; Alexis Bruemmer; Leendert Van Doorn；年份：2007；出处：2007 Linux Symposium, Volume One, June 27-30, 2007, Ottawa, Ontario, Canada。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在裸机与 Xen 下量化 Calgary/DART IOMMU 的吞吐和 CPU 成本；分析 TCE 表、IOTLB、map/unmap、flush、hypercall/锁的开销，并提出 deferred flush、DMA-API 预分配/批量、grant table 等优化。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：目标为 Calgary 与 DART 两类 TCE 架构 IOMMU；bare-metal 使用 Linux DMA-API；hypervisor 使用 Xen dom0/domU 与 hypervisor-aware IOMMU 层；netperf 测网络 IO，FFSB 测磁盘 IO；IOMMU on/off 对比；TCE 表/IO 地址空间可被多个共享该表的设备使用；IOTLB 行为受 cache size/associativity/replacement/invalidation 影响；DMA 地址不一定有 CPU 式时空局部性

限制：主要成本定位在软件 map/unmap、锁、hypercall/地址空间切换；未量化 IOTLB 硬件 miss 成本；不是多设备共享 IOMMU 干扰研究；不是现代 IOMMU/SVA/ATS 研究；老平台 Calgary/DART，不能直接外推到现代 CXL/ATS IOMMU；无 PASID、设备 ATC、共享虚拟地址、尾延迟、队列容量评估；部分优化（never free、预分配）牺牲保护或要求 guest 映射稳定

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[iotlb_bottleneck2010](iotlb_bottleneck2010.md)：2007 把主成本放在软件 map/unmap；2010 用 pseudo pass-through 隔离软件成本后转向硬件 IOTLB 解析瓶颈。二者是同一系统不同阶段的瓶颈。；[m2ndp2024](m2ndp2024.md)：2007 关注 CPU 侧软件映射成本；M2NDP 关注 ATS/ATC 与 CXL NDP 命令开销，问题对象不同。

原始来源：

- [来源1](https://www.kernel.org/doc/ols/2007/ols2007v1-pages-9-20.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/iommu_cost2007.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.pdf) · SHA-256 `8094a6e26246078cd7441b06c555e065b3a312c4f5780a246d67195914a5c921`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:8712](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:14](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:80](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:237](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:292](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:372](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:394](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt:426](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_cost2007.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:275](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 iommu_cost2007：PDF papers/iommu_cost2007.pdf; text papers/iommu_cost2007.txt; 报告 workers/G16_semantics.md; 证据卡 workers/G16_semantics-papers.json；reading=fulltext_worker_reviewed；status=published；year=2007；venue=2007 Linux Symposium, Volume One, June 27-30, 2007, Ottawa, Ontario, Canada；doi=未知；source=https://www.kernel.org/doc/ols/2007/ols2007v1-pages-9-20.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:274](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **iommu_cost2007** The Price of Safety: Evaluating IOMMU Performance — Muli Ben-Yehuda, Jimi Xenidis, Michal Ostrowski, Karl Rister, Alexis Bruemmer, Leendert Van Doorn；2007 Linux Symposium, Volume One, June 27-30, 2007, Ottawa, Ontario, Canada；doi未知；source=https://www.kernel.org/doc/ols/2007/ols2007v1-pages-9-20.pdf；topic=io；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:259](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2007 / published | <a id="iommu_cost2007"></a>[The Price of Safety: Evaluating IOMMU Performance](https://www.kernel.org/doc/ols/2007/ols2007v1-pages-9-20.pdf) | iommu_cost2007 | iommu_cost2007 | foundation |

[返回总目录](../README.md)
