# NDPage: Efficient Address Translation for Near-Data Processing Architectures via Tailored Page Table

稳定ID：`ndpage2025` · 类型：`research_paper` · 主题：`accelerators`

作者：Qingcai Jiang; Buxin Tu; Hong An；年份：2025；出处：DATE 2025。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：NDP 中 PTE 访问 bypass L1 直接访主存，避免污染 cache；flatten 最后两级页表为 2MB flattened L2/L1 node，把 PTW 从 4 次 sequential 降为 3 次。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：NDP 只有 shallow L1，L2/L3 不存在；OS 标记 PTE 4KB 区域；PTE 64B aligned；硬件 walker 支持 flattened node bit；last two page table levels 近乎满；不限制 page mapping

限制：无真实 NDP 硬件，仅 Victima/Sniper 仿真；依赖 NDP cache 层级假设；无安全/多租户评估；无动态迁移/跨节点 remote PTW；仅 11 workloads

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[dipta2017](dipta2017.md)：NDPage 引用 DIPTA 为 PACT 2017，并指出其关联度限制可能导致 page conflict 性能退化。；[utopia2023](utopia2023.md)：NDPage 引用 Utopia 为 MICRO 2023。；[sparta2020](sparta2020.md)：两者都面向内存侧/近存翻译，但 NDPage 优化页表 cache 行为与层级，不做 partition 限制。

原始来源：

- [来源1](https://arxiv.org/pdf/2502.14220)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/ndpage2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.pdf) · SHA-256 `eb7daa273b4f5f245ca79741d661e8495ac436a9585f15fbb26e91c2cbbb1b3d`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:12099](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt:146](../../iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt:169](../../iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt:196](../../iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt:223](../../iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt:242](../../iommu-platform/out/shared-translation-literature-20261006/papers/ndpage2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:327](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:377](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:379](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 ndpage2025：PDF papers/ndpage2025.pdf; text papers/ndpage2025.txt; 报告 workers/G10_nearmemory.md; 证据卡 workers/G10_nearmemory-papers.json；reading=fulltext_worker_reviewed；status=published；year=2025；venue=DATE 2025；doi=未知；source=https://arxiv.org/pdf/2502.14220；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:377](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **ndpage2025** NDPage: Efficient Address Translation for Near-Data Processing Architectures via Tailored Page Table — Qingcai Jiang, Buxin Tu, Hong An；DATE 2025；doi未知；source=https://arxiv.org/pdf/2502.14220；topic=accelerators；status=published；relevance=core；reading=fulltext_worker_reviewed；fulltext=True；root_spotchecked=False；metadata_source=https://date25.date-conference.com/programme

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:327](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2025 / published | <a id="ndpage2025"></a>[NDPage: Efficient Address Translation for Near-Data Processing Architectures via Tailored Page Table](https://arxiv.org/pdf/2502.14220) | ndpage2025 | ndpage2025 | core |

[返回总目录](../README.md)
