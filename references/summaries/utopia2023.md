# Utopia: Fast and Efficient Address Translation via Hybrid Restrictive & Flexible Virtual-to-Physical Address Mappings

稳定ID：`utopia2023` · 类型：`research_paper` · 主题：`foundations`

作者：Konstantinos Kanellopoulos; Rahul Bera; Kosta Stojiljkovic; Nisa Bostanci; Can Firtina; Rachata Ausavarungnirun; Rakesh Kumar; Nastaran Hajinazar; Mohammad Sadrosadati; Nandita Vijaykumar; Onur Mutlu；年份：2023；出处：MICRO 2023。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：混合 RestSeg 与 FlexSeg：RestSeg 用 set-associative hash mapping、TAR/SF 和 RestSegWalk 实现低延迟翻译；FlexSeg 保留 conventional fully flexible mapping。OS 负责 RestSeg 分配与页迁移。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：OS 扩展支持 RestSeg/FlexSeg 创建、分配、迁移；RestSeg boot time 创建并需连续物理内存；页面可被识别为 costly-to-translate；RestSeg 有容量与 set 空位；迁移需 TLB/TAR/SF shootdown 和 cache flush；FlexSeg 保留 sharing/swap

限制：无硬件原型，仅 Sniper 仿真；virtualized environments 留 future work；RestSeg 1MB 时无收益；迁移与 TAR/SF cache 维护有成本；无安全/GPU/I/O 评估

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[dipta2017](dipta2017.md)：Utopia 引用 DIPTA 为 PACT 2017，并作为 hash-based restrictive mapping 前作。；[vbi2020](vbi2020.md)：Utopia 引用 VBI 为 ISCA 2020，并称其正交。；[sparta2020](sparta2020.md)：两者都用限制映射减少翻译元数据，但 SPARTA 限制到 partition，Utopia 限制到 RestSeg 而保留 FlexSeg。

原始来源：

- [来源1](https://arxiv.org/pdf/2211.12205v4)
- [来源2](https://arxiv.org/abs/2211.12205)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/utopia2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.pdf) · SHA-256 `34f7be2991b5d6ebb01c6019f2c4c1454159847396e6779d2da60247316e0ae3`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:20120](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt:27](../../iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt:652](../../iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt:731](../../iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt:913](../../iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt:443](../../iommu-platform/out/shared-translation-literature-20261006/papers/utopia2023.txt)
- [iommu-platform/out/innovation-council18-20261005/round1/C1.md:36](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:36](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C1.md:36](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)：

| [Utopia](https://arxiv.org/abs/2211.12205)、[Mosaic Pages](https://www.cs.unc.edu/~porter/pubs/mosaic.pdf)，目前摘要/片段 | 混合限制/自由映射；哈希候选压缩小页翻译 | 不是再次提出“受限快路径 + 自由 fallback”，也不是再次提出多候选哈希。 |

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md:36](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C1.md)：

| [Utopia](https://arxiv.org/abs/2211.12205)、[Mosaic Pages](https://www.cs.unc.edu/~porter/pubs/mosaic.pdf)，目前摘要/片段 | 混合限制/自由映射；哈希候选压缩小页翻译 | 不是再次提出“受限快路径 + 自由 fallback”，也不是再次提出多候选哈希。 |

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:86](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **utopia2023** Utopia: Fast and Efficient Address Translation via Hybrid Restrictive & Flexible Virtual-to-Physical Address Mappings — Konstantinos Kanellopoulos, Rahul Bera, Kosta Stojiljkovic, Nisa Bostanci, Can Firtina, Rachata Ausavarungnirun, Rakesh Kumar, Nastaran Hajinazar, Mohammad Sadrosadati, Nandita Vijaykumar, Onur Mutlu；MICRO 2023；doi未知；source=https://arxiv.org/pdf/2211.12205v4；topic=foundations；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=True；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:87](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 utopia2023：PDF papers/utopia2023.pdf; text papers/utopia2023.txt; 报告 workers/G10_nearmemory.md; 证据卡 workers/G10_nearmemory-papers.json；reading=fulltext_worker_reviewed；status=published；year=2023；venue=MICRO 2023；doi=未知；source=https://arxiv.org/pdf/2211.12205v4；root_spotchecked=True

[返回总目录](../README.md)
