# RadixVM: Scalable address spaces for multithreaded applications

稳定ID：`radvm2013` · 类型：`research_paper` · 主题：`lifecycle`

作者：Austin T. Clements; M. Frans Kaashoek; Nickolai Zeldovich；年份：2013；出处：EuroSys 2013 (Prague, Czech Republic)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：Radix tree 存 per-page mapping metadata，Refcache 可扩展引用计数，per-core page tables 精确追踪哪些核缓存了映射并定向发送 TLB shootdown。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：非重叠 mmap/munmap/pagefault 应并行；重叠区域可串行；x86 不报告 TLB 内容时，可用 per-core page table 的 fault 记录推断哪些核可能缓存映射；Refcache 延迟检测 zero 引用可接受，两 epoch 后确认回收

限制：只保证非重叠区域完美扩展，重叠操作串行；未实现 radix tree collapsing，未支持 swapping；per-core page table 内存开销可达 13×；APIC IPI 协议在 80 核近 1 ms，targeted shootdown 仍受 IPI 限制；不涉及设备 DMA、IOMMU、SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[shootdown1989](shootdown1989.md)：actual_citation；[hydra2024](hydra2024.md)：mechanism_comparison

原始来源：

- [来源1](https://pdos.csail.mit.edu/papers/radixvm:eurosys13.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/radvm2013.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.pdf) · SHA-256 `9b199a877714cb09b177271e8c4a35d7f3720cd95afdc1696cca1bf622fafa4c`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:14710](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt:8](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt:164](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt:403](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt:678](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt:785](../../iommu-platform/out/shared-translation-literature-20261006/papers/radvm2013.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:397](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:435](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 radvm2013：PDF papers/radvm2013.pdf; text papers/radvm2013.txt; 报告 workers/G12_update.md; 证据卡 workers/G12_update-papers.json；reading=fulltext_worker_reviewed；status=published；year=2013；venue=EuroSys 2013 (Prague, Czech Republic)；doi=未知；source=https://pdos.csail.mit.edu/papers/radixvm:eurosys13.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:434](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **radvm2013** RadixVM: Scalable address spaces for multithreaded applications — Austin T. Clements, M. Frans Kaashoek, Nickolai Zeldovich；EuroSys 2013 (Prague, Czech Republic)；doi未知；source=https://pdos.csail.mit.edu/papers/radixvm:eurosys13.pdf；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:397](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2013 / published | <a id="radvm2013"></a>[RadixVM: Scalable address spaces for multithreaded applications](https://pdos.csail.mit.edu/papers/radixvm:eurosys13.pdf) | radvm2013 | radvm2013 | core |

[返回总目录](../README.md)
