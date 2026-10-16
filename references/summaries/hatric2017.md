# Hardware Translation Coherence for Virtualized Systems

稳定ID：`hatric2017` · 类型：`research_paper` · 主题：`lifecycle`

作者：Zi Yan; Guilherme Cox; Ján Veselý; Abhishek Bhattacharjee；年份：2017；出处：ISCA 2017。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：给 TLB/MMU cache/nTLB 加 co-tag，存 PTE 的系统物理地址，把翻译一致性 piggyback 到 cache coherence，支持虚拟化 nested page table。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：x86-64；2B co-tag；MESI/MOESI/MESIF；translation structure 只读；KVM 主评/Xen 初评

限制：co-tag 粗粒度导致 false invalidation；目录 pseudo-specific 造成 spurious coherence；部分 workload energy 稍增；未做 prefetch/update；本组提供的全文是 arXiv v2

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/1701.07517)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/hatric2017.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.pdf) · SHA-256 `c27f91a63a8ecff9bebb7c54943e07d18ebbe8f906a48ccd97c52a806fdb1494`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:7330](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt:22](../../iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt:116](../../iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt:395](../../iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt:602](../../iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:401](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:442](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:443](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:443](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 hatric2017：PDF papers/hatric2017.pdf; text papers/hatric2017.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2017；venue=ISCA 2017；doi=10.1145/3079856.3080211；source=https://arxiv.org/pdf/1701.07517；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:274](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- 版本 hatric2017：PDF papers/hatric2017.pdf; text papers/hatric2017.txt; 报告 workers/G6_lifecycle.md; 证据卡 workers/G6_lifecycle-papers.json；reading=fulltext_worker_reviewed；status=published；year=2017；venue=ISCA 2017；doi=10.1145/3079856.3080211；source=https://arxiv.org/pdf/1701.07517；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:442](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **hatric2017** Hardware Translation Coherence for Virtualized Systems — Zi Yan, Guilherme Cox, Ján Veselý, Abhishek Bhattacharjee；ISCA 2017；10.1145/3079856.3080211；source=https://arxiv.org/pdf/1701.07517；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md:273](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/INDEX.md)：

- **hatric2017** Hardware Translation Coherence for Virtualized Systems — Zi Yan, Guilherme Cox, Ján Veselý, Abhishek Bhattacharjee；ISCA 2017；10.1145/3079856.3080211；source=https://arxiv.org/pdf/1701.07517；topic=lifecycle；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[返回总目录](../README.md)
