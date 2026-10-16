# vIOMMU: Efficient IOMMU Emulation

稳定ID：`viommu2011` · 类型：`research_paper` · 主题：`io`

作者：Nadav Amit; Muli Ben-Yehuda; Dan Tsafrir; Assaf Schuster；年份：2011；出处：USENIX ATC 2011。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在KVM中向未修改guest暴露模拟IOMMU；hypervisor捕获IOVA映射并pin页；乐观拆除延迟约10ms复用IOVA；sidecore轮询模拟寄存器。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：guest未修改但一旦看到IOMMU会使用IOMMU。；hypervisor不能让guest直接编程物理IOMMU；物理IOMMU与模拟IOMMU正交。；inter-guest保护通过pin到物理unmap完成；intra-guest保护可由guest/host策略放松。；sidecore模拟需要特定硬件属性：同步寄存器写、单寄存器读写分离、宽松时序、显式更新内存数据结构。

限制：自身文件题头未含venue/year/DOI；venue/year来自fastsafe2024参考文献[6]交叉引用。；完整intra-guest protection在Linux上仍是open challenge。；不量化恶意对手的短漏洞窗口，只量化stale mapping窗口与数量。；sidecore多socket有QPI延迟，低功耗sleep仅适合长静默期，x86只能监控单cache line。；I/O page faults和多级IOMMU页表硬件支持只是未来工作，未实现。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[fastsafe2024](fastsafe2024.md)：fastsafe2024引用vIOMMU；本报告借该引用核对venue/year，但viommu2011自身题头未含venue。；[damn2018](damn2018.md)：两者都避免立即失效：vIOMMU乐观保留IOVA翻译，DAMN永久映射网络缓冲，但翻译实体和虚拟化层次不同。

原始来源：

- [来源1](https://www.mulix.org/pubs/iommu/viommu.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/viommu2011.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.pdf) · SHA-256 `1cf851f1f706aa3f58c33647d75a268e572e823ce106efc06ad01cf47c1b48c7`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:21115](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:70](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:177](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:279](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:331](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:404](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt:545](../../iommu-platform/out/shared-translation-literature-20261006/papers/viommu2011.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:281](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 viommu2011：PDF papers/viommu2011.pdf; text papers/viommu2011.txt; 报告 workers/G15_io_expansion.md; 证据卡 workers/G15_io_expansion-papers.json；reading=fulltext_worker_reviewed；status=published；year=2011；venue=USENIX ATC 2011；doi=未知；source=https://www.mulix.org/pubs/iommu/viommu.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:280](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **viommu2011** vIOMMU: Efficient IOMMU Emulation — Nadav Amit, Muli Ben-Yehuda, Dan Tsafrir, Assaf Schuster；USENIX ATC 2011；doi未知；source=https://www.mulix.org/pubs/iommu/viommu.pdf；topic=io；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:261](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2011 / published | <a id="viommu2011"></a>[vIOMMU: Efficient IOMMU Emulation](https://www.mulix.org/pubs/iommu/viommu.pdf) | viommu2011 | viommu2011 | core |

[返回总目录](../README.md)
