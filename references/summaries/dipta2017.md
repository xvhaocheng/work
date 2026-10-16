# Near-Memory Address Translation

稳定ID：`dipta2017` · 类型：`research_paper` · 主题：`accelerators`

作者：Javier Picorel; Djordje Jevdjic; Babak Falsafi；年份：2017；出处：PACT 2017。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：近存 MPU 用 per-vault 分布式倒排页表，限制 virtual-to-physical 关联度到 direct/4-way，用 page interleaving 与 way predictor 让翻译和数据取回并行。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：工作集几乎全驻内存；虚拟页常连续映射到物理页；MPU 不能运行 OS，缺页交 CPU handler；不暴露 shared libraries/kernel，因此不处理 synonym；memory oversubscription 不在范围；多程序下可能因 limited associativity 产生竞争

限制：无硬件原型，仅 trace-driven + full-system simulation；不支持 synonym/CoW 未展开；memory oversubscription 未评估；多程序关联度竞争未充分解决；安全仅 ASID/page flags 层面

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[sparta2020](sparta2020.md)：SPARTA 将 DIPTA 引用为 PACT 2017。；[sparta2020](sparta2020.md)：SPARTA 批评 DIPTA 的 SRAM/DRAM 开销、低关联度 page fault、CoW/synonym 限制。

原始来源：

- [来源1](https://arxiv.org/pdf/1612.00445)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/dipta2017.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.pdf) · SHA-256 `8a0f80dd8e624e3523fe741b015f0662eca2c083b83645a1448e9bcf80391592`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:4277](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt:26](../../iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt:215](../../iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt:369](../../iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt:493](../../iommu-platform/out/shared-translation-literature-20261006/papers/dipta2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:315](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:341](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:342](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:342](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 dipta2017：PDF papers/dipta2017.pdf; text papers/dipta2017.txt; 报告 workers/G10_nearmemory.md; 证据卡 workers/G10_nearmemory-papers.json；reading=fulltext_worker_reviewed；status=published；year=2017；venue=PACT 2017；doi=未知；source=https://arxiv.org/pdf/1612.00445；root_spotchecked=True

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:341](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **dipta2017** Near-Memory Address Translation — Javier Picorel, Djordje Jevdjic, Babak Falsafi；PACT 2017；doi未知；source=https://arxiv.org/pdf/1612.00445；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:315](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2017 / published | <a id="dipta2017"></a>[Near-Memory Address Translation](https://arxiv.org/pdf/1612.00445) | dipta2017 | dipta2017 | core |

[返回总目录](../README.md)
