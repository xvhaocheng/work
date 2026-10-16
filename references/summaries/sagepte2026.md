# SagePTE: Co-locating guest and host PTEs to accelerate virtual memory translation

稳定ID：`sagepte2026` · 类型：`research_paper` · 主题：`foundations`

作者：Amir Noohi; Mostafa Derispour; Rodrigo C. O. Rocha; Antonio Barbalace; Artemiy Margaritov；年份：2026；出处：MICRO 2026。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：机构摘要：仅在 guest 页表中就地影子 leaf PTE，按需把 guest/host 映射合入 128-bit 扩展项，复用 128-bit CAS 实现无 trap 的 leaf 同步，Alloy 验证隔离，ARM 上 4KB 平均 1.30×、THP 1.22×。与现有 nbwalker2026 同属 MICRO 2026 地址翻译工作，与 foundations 中 shared1993/synergistic2010/translation_ranger2019 的 TLB/翻译结构线相邻；聚焦嵌套翻译 leaf 开销，与设备 IOMMU 线不同。限制：仅机构题录摘要，作者手稿禁运至 2026-11-04，未获全文，机制细节未独立核读。

支持边界：仅题录/摘要/索引片段，不得用于全文机制覆盖或收益裁决。

限制：MICRO 2026 未召开，按 accepted_upcoming；无 DOI 记录，未猜。 机构记录说明全文embargo至2026-11-04；摘要级，未绕过限制。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.research.ed.ac.uk/en/publications/sagepte-co-locating-guest-and-host-ptes-to-accelerate-virtual-mem/)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:15703](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:41](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:114](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:117](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md:121](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md:121](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md)：

"source_url": "https://www.research.ed.ac.uk/en/publications/sagepte-co-locating-guest-and-host-ptes-to-accelerate-virtual-mem/",

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:114](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **sagepte2026** SagePTE: Co-locating guest and host PTEs to accelerate virtual memory translation — Amir Noohi, Mostafa Derispour, Rodrigo C. O. Rocha, Antonio Barbalace, Artemiy Margaritov；MICRO 2026；doi未知；source=https://www.research.ed.ac.uk/en/publications/sagepte-co-locating-guest-and-host-ptes-to-accelerate-virtual-mem/；topic=foundations；status=accepted_upcoming；relevance=foundation；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=University of Edinburgh Research Explorer 机构题录（摘要、Accepted/In press - 8 Jul 2026、ISBN、禁运）；MICRO 2026 官方程序（Session 2C Distinguished Papers I）

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:41](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2026 / accepted_upcoming | <a id="sagepte2026"></a>[SagePTE: Co-locating guest and host PTEs to accelerate virtual memory translation](https://www.research.ed.ac.uk/en/publications/sagepte-co-locating-guest-and-host-ptes-to-accelerate-virtual-mem/) | — | — | foundation |

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:117](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 sagepte2026：无本地文件；reading=abstract_or_indexed_excerpt；status=accepted_upcoming；year=2026；venue=MICRO 2026；doi=未知；source=https://www.research.ed.ac.uk/en/publications/sagepte-co-locating-guest-and-host-ptes-to-accelerate-virtual-mem/；root_spotchecked=False

[返回总目录](../README.md)
