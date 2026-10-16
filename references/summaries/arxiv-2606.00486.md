# Dead on Arrival，arXiv:2606.00486v2〔正式题名待整理〕

稳定ID：`arxiv-2606.00486` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/html/2606.00486v2)
- [来源2](https://arxiv.org/pdf/2606.00486)
- [来源3](https://arxiv.org/abs/2606.00486)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:42](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/docs/mechanism-design-handoff-20261009-round04.md:53](../../iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)
- [iommu-platform/docs/novelty-gate-20261006.md:46](../../iommu-platform/docs/novelty-gate-20261006.md)
- [iommu-platform/docs/shared-translation-gaps-20261006.md:18](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/out/independent-directions-20261015/methods/assessment.md:40](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)
- [iommu-platform/out/rcs1-completion-set-20261014/root/prior-and-closed-boundaries.md:11](../../iommu-platform/out/rcs1-completion-set-20261014/root/prior-and-closed-boundaries.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:24](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:176](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/independent-research-directions-20261015.md:42](../../iommu-platform/docs/independent-research-directions-20261015.md)：

| **[Dead on Arrival，2026 v2预印本](https://arxiv.org/html/2606.00486v2)，§5.1–5.4、§6–7.3** | 相近MPKI/重复walk比例下，访问共享及MSHR聚集结构导致保护收益不同；提出DEPOT，并展示与LatPC组合的正负效果 | “相似miss统计但收益不同，加入结构信息”“优化组合会退化”都已经非常接近。须先给基线同样可得的复用/扇出/突发信息，再具体验证跨owner的翻译—载荷时序对P/N/限流完整净效果的额外选择价值。仅换成IOMMU不算贡献 |

[iommu-platform/docs/novelty-gate-20261006.md:46](../../iommu-platform/docs/novelty-gate-20261006.md)：

- [Dead on Arrival v2，2026-06-02预印本](https://arxiv.org/html/2606.00486v2)已讨论相近miss统计下，页共享结构和同一次walk解除多少等待者会造成不同收益，并用驱逐历史保护条目。其“miss统计不足、需额外结构信息”的研究形态与我们的宽泛动机邻近。它不是源端DMA限速，所用GPU/MSHR机制也不能直接搬作本平台事实；当前按预印本处理，不称正式接收。

[iommu-platform/docs/shared-translation-gaps-20261006.md:18](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

| [Dead on Arrival](https://arxiv.org/abs/2606.00486) | 2026预印本，已取全文；摘要级核验 | 区分驱逐后重复遍历的不同负载情况，讨论多个warp共同等待造成的放大；可影响干扰/容量解释。页面写submitted IISWC，不写已接收。 |

[iommu-platform/docs/mechanism-design-handoff-20261009-round04.md:53](../../iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)：

- 重装条目限时保护：[Dead on Arrival，2026 预印本](https://arxiv.org/pdf/2606.00486)，录用未验证。

[返回总目录](../README.md)
