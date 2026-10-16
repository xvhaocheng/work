# SpecTLB: A Mechanism for Speculative Address Translation

稳定ID：`spectlb2011` · 类型：`research_paper` · 主题：`foundations`

作者：Thomas W. Barr; Alan L. Cox; Scott Rixner；年份：2011；出处：ISCA '11: 38th Annual International Symposium on Computer Architecture。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：利用 reservation-based 内存分配器的对齐规律，对 TLB miss 的 4KB 页做 VA-PA 插值预测，并并行验证页表。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：OS 使用 reservation-based physical memory allocator；预测必须与底层 page table 验证；显式 marking 需要 OS/PTE 改动；heuristic 不修改 OS；I/O/不可缓存区域需要防止推测副作用

限制：无硬件原型，仅 simulation；显式 reservation marking 尚未进入任何 OS；heuristic 有 false positive，需验证；未覆盖设备 IOMMU/SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[revelator2026v2](revelator2026v2.md)：Revelator 参考文献 [88] 引用 SpecTLB。；[capaging2020](capaging2020.md)：CA paging 参考文献 [19] 引用 SpecTLB。；[emt2025](emt2025.md)：EMT 参考文献 [33] 引用 SpecTLB。

原始来源：

- [来源1](https://courses.physics.illinois.edu/ece598ms/fa2019/papers/paper32.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/spectlb2011.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.pdf) · SHA-256 `55ce2ef6e3a56ce1f2bd0ea3bb4b8565db3ba1d933d9f8ad213d16266eb82d4f`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:17720](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt:14](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt:249](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt:322](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt:350](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt:419](../../iommu-platform/out/shared-translation-literature-20261006/papers/spectlb2011.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:19](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:59](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 spectlb2011：PDF papers/spectlb2011.pdf; text papers/spectlb2011.txt; 报告 workers/G14_structure.md; 证据卡 workers/G14_structure-papers.json；reading=fulltext_worker_reviewed；status=published；year=2011；venue=ISCA '11: 38th Annual International Symposium on Computer Architecture；doi=未知；source=https://courses.physics.illinois.edu/ece598ms/fa2019/papers/paper32.pdf；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:58](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **spectlb2011** SpecTLB: A Mechanism for Speculative Address Translation — Thomas W. Barr, Alan L. Cox, Scott Rixner；ISCA '11: 38th Annual International Symposium on Computer Architecture；doi未知；source=https://courses.physics.illinois.edu/ece598ms/fa2019/papers/paper32.pdf；topic=foundations；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=未知

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:19](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2011 / published | <a id="spectlb2011"></a>[SpecTLB: A Mechanism for Speculative Address Translation](https://courses.physics.illinois.edu/ece598ms/fa2019/papers/paper32.pdf) | spectlb2011 | spectlb2011 | core |

[返回总目录](../README.md)
