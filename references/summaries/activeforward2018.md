# Active forwarding: Eliminate IOMMU address translation for accelerator-rich architectures

稳定ID：`activeforward2018` · 类型：`research_paper` · 主题：`accelerators`

作者：Hsueh-Chun Fu; Po-Han Wang; Chia-Lin Yang；年份：2018；出处：DAC 2018 (55th Design Automation Conference)。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：与现有 accelerators 线的关系：ontable2017 讨论加速器侧页表遍历，AF 则完全绕过 IOMMU 翻译，把加速器输入数据从 CPU cache 直接前送到 scratchpad；与 cryptommu2023 的第三方加速器访问控制/隔离属不同问题。机构摘要称相对 hostPageWalk 平均提升 8%、降低加速器功耗 22.1%，但未获全文、未复现，仅作机构摘要级记录。

支持边界：仅题录/摘要/索引片段，不得用于全文机制覆盖或收益裁决。

限制：机构题录与 dblp 元数据可核；仅机构摘要，未获 PDF，性能数字未独立验证。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:185](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:317](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:345](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:348](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md:15](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md:15](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/workers/Emeta.md)：

"source_url": "https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f",

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:345](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- **activeforward2018** Active forwarding: Eliminate IOMMU address translation for accelerator-rich architectures — Hsueh-Chun Fu, Po-Han Wang, Chia-Lin Yang；DAC 2018 (55th Design Automation Conference)；10.1145/3195970.3195984；source=https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f；topic=accelerators；status=published；relevance=core；reading=abstract_or_indexed_excerpt；fulltext=False；root_spotchecked=False；metadata_source=NTU Scholars institutional record (authors, venue, DOI and abstract) https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:348](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

- 版本 activeforward2018：无本地文件；reading=abstract_or_indexed_excerpt；status=published；year=2018；venue=DAC 2018 (55th Design Automation Conference)；doi=10.1145/3195970.3195984；source=https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f；root_spotchecked=False

[iommu-platform/out/shared-translation-literature-20261006/INDEX.md:317](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)：

| 2018 / published | <a id="activeforward2018"></a>[Active forwarding: Eliminate IOMMU address translation for accelerator-rich architectures](https://scholars.lib.ntu.edu.tw/entities/publication/5b7e8d80-0aaf-4d94-947b-8a966bb6567f) | — | — | core |

[返回总目录](../README.md)
