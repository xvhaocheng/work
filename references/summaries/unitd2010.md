# UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All

稳定ID：`unitd2010` · 类型：`research_paper` · 主题：`lifecycle`

作者：Bogdan F. Romanescu; Alvin R. Lebeck; Daniel J. Sorin; Anne Bracy；年份：2010；出处：HPCA 2010。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：把 TLB 纳入 cache coherence，PCAM 按 PTE 物理地址反向查找并失效 TLB entry，消除软件 TLB shootdown。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：x86 硬件管理 TLB；PTE/cache block 粒度；PCAM 与 TLB 同 entry 数；MOSI/MOESI snooping/directory；2-16 核仿真

限制：大 PCAM 的 area/power 可能不可行；未建模 interrupt 投递；HATRIC 称 UNITD 不支持虚拟化/MMU cache/nTLB；本组全文未印 DOI，DOI 由用户给定核对

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/unitd2010.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.pdf) · SHA-256 `a71e1f585e1ccbf26fbbf077aa3405e424f6c6e8a4bcfd05b76fa01c09a2db7f`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:19960](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt:17](../../iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt:259](../../iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt:368](../../iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt:605](../../iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt:602](../../iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:395](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:430](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:431](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 unitd2010：PDF papers/unitd2010.pdf; text papers/unitd2010.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2010；venue=HPCA 2010；doi=10.1109/HPCA.2010.5416643；source=https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:268](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 unitd2010：PDF papers/unitd2010.pdf; text papers/unitd2010.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2010；venue=HPCA 2010；doi=10.1109/HPCA.2010.5416643；source=https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:430](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **unitd2010** UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All — Bogdan F. Romanescu, Alvin R. Lebeck, Daniel J. Sorin, Anne Bracy；HPCA 2010；10.1109/HPCA.2010.5416643；source=https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf；topic=lifecycle；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:267](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **unitd2010** UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All — Bogdan F. Romanescu, Alvin R. Lebeck, Daniel J. Sorin, Anne Bracy；HPCA 2010；10.1109/HPCA.2010.5416643；source=https://people.ee.duke.edu/~sorin/papers/hpca10_unitd.pdf；topic=lifecycle；status=published；relevance=foundation；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
