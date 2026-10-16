# TRANS-FETCH: Driving Translation Prefetching with Deep Lookahead from L2 Data Prefetch Events

稳定ID：`transfetch2026` · 类型：`research_paper` · 主题：`foundations`

作者：Cansong Zhou; Xiaohang Wang; Peng Liu；年份：2026；出处：MICRO 2026。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：与 latpc2025（GPU TLB 预取与 MSHR 压缩）及 MICRO 2026 同会 Trail（temporal TLB prefetching）同属翻译预取线；题名表明用 L2 数据预取事件做深层 lookahead 来驱动翻译预取，但仅大会题录，未获摘要/全文，不能推断其预测结构或与硬件数据预取器的耦合方式。

支持边界：仅题录/摘要/索引片段，不得用于全文机制覆盖或收益裁决。

限制：仅官方题录，无摘要；MICRO 2026 未召开，按 accepted_upcoming；未猜 DOI。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.microarch.org/micro59/program/)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:19344](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/docs/composition-history-isolation-20261007.md:153](../../iommu-platform/docs/composition-history-isolation-20261007.md)
- [iommu-platform/docs/shared-translation-gaps-20261006.md:15](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/docs/shared-translation-gaps-20261006.md:16](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/docs/shared-translation-gaps-20261006.md:21](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/docs/translation-comparison-20261006.md:127](../../iommu-platform/docs/translation-comparison-20261006.md)
- [iommu-platform/out/flow-iommu-candidates-20261014/rendezvous/analysis.md:19](../../iommu-platform/out/flow-iommu-candidates-20261014/rendezvous/analysis.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:38](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/composition-history-isolation-20261007.md:153](../../iommu-platform/docs/composition-history-isolation-20261007.md)：

最新近邻：[NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs](https://www.microarch.org/micro59/program/)已列入MICRO 2026正式议程，本次另核到[作者公开artifact](https://github.com/redbird-arch/micro2026-nbwalker-artifact)。本地此前只有题录；这次也未获得论文全文，README不足以判定其与N的机制关系或性能。不能根据“non-blocking”题名声称我们的N相同或不同。另有同场次contention-aware translation offloading题目，仅作待核线索，未纳入已读机制对照。

[iommu-platform/docs/shared-translation-gaps-20261006.md:16](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

| [Make Every Batch Count: Fault Entry Merging for Efficient Batching in Unified Virtual Memory](https://www.microarch.org/micro59/program/) | MICRO 2026已接收；仅题录 | UVM批处理/故障条目合并近邻，需补机制对照。作者实验室页面误放SSD摘要，本轮未采用该摘要。 |

[iommu-platform/docs/shared-translation-gaps-20261006.md:15](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

| [SPMC: Speculative Multicast for GPU Address Translation Sharing](https://www.microarch.org/micro59/program/) | MICRO 2026已接收；仅官方题录 | 翻译共享的直接近邻，优先取全文；当前不能判断拓扑、共享范围或协议。 |

[iommu-platform/docs/shared-translation-gaps-20261006.md:21](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

MICRO 2026的上述接收状态按[会议官方程序](https://www.microarch.org/micro59/program/)核实；截至本记录日期会议尚未举行。这只是本轮优先补集，不宣称穷尽2026年全部相关工作。

[返回总目录](../README.md)
