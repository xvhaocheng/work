# PFM / Dual-View Memory〔正式题名待整理〕

稳定ID：`arxiv-2608.06989` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/abs/2608.06989)
- [来源2](https://arxiv.org/pdf/2608.06989)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/shared-translation-gaps-20261006.md:13](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/docs/translation-opportunities-20261006.md:61](../../iommu-platform/docs/translation-opportunities-20261006.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:23](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:175](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/translation-opportunities-20261006.md:61](../../iommu-platform/docs/translation-opportunities-20261006.md)：

补充对照：[PFM / Dual-View Memory](https://arxiv.org/abs/2608.06989)（MICRO 2026已接收）已有一份物理数据面向NPU/PIM的不同逻辑视图；原文§IV-C的ARU做PA到硬件地址重映射，§VIII明确保留基本VA→PA转换，并依赖离线映射、有限MDT与重排缓冲。因此宽泛的“双视图”主张需要撤回；IOMMU页表表示共存及其更新成本是否仍有独立问题，需要精确比较，不能仅因层级不同声称新意。另补RMM与Twizzler；Direct Segments与DVM在原123项中已有全文，不是新增遗漏。

[iommu-platform/docs/shared-translation-gaps-20261006.md:13](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

| [PFM / Dual-View Memory](https://arxiv.org/abs/2608.06989) | MICRO 2026已接收，官方程序确认；取得作者稿，关键段已核 | 同一物理数据向NPU/PIM提供不同逻辑视图。第四候选不能把“双视图”本身当创新。 |

[iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:175](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)：

| pfm2026 | Rethinking Unified Memory for NPU-PIM Systems: Dual-View Memory for Dynamic Inference of LLM | MICRO 2026 accepted_upcoming; arXiv 2608.06989 | 4 | P0 | not_in_123_catalog | root_key_sections_checked | 已核 §V-C/§VIII：ARU 是 PA 到设备硬件地址；保持 VA→PA，需离线映射与 relayout buffer；否定宽泛双视图新意，不等于全覆盖 IOMMU 翻译副本 | https://arxiv.org/pdf/2608.06989 |

[iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:23](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)：

| Rethinking Unified Memory for NPU-PIM Systems: Dual-View Memory for Dynamic Inference of LLM | MICRO 2026 accepted_upcoming; arXiv 2608.06989 | P0 | [来源](https://arxiv.org/pdf/2608.06989) · PDF · 文本 | root_key_sections_checked |

[返回总目录](../README.md)
