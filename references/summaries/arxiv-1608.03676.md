# Coz，SOSP2015〔正式题名待整理〕

稳定ID：`arxiv-1608.03676` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/pdf/1608.03676)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:41](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/out/independent-directions-20261015/methods/assessment.md:43](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:13](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:165](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/independent-research-directions-20261015.md:41](../../iommu-platform/docs/independent-research-directions-20261015.md)：

| [MISE，HPCA2013](https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf)，§3–4；[ASM，MICRO2015](https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf)，§3–4；[Coz，SOSP2015](https://arxiv.org/pdf/1608.03676)，§2 | 优先级干预估计独占服务、cache/memory共同干扰、因果剖析和优化价值都有前史 | 需在同预算适配中指出它们对翻译—载荷反馈的具体误判及改进。单次“增加PTW延迟”的敏感性不能直接当消除延迟的收益 |

[iommu-platform/out/independent-directions-20261015/methods/assessment.md:43](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)：

| [Coz，SOSP2015](https://arxiv.org/pdf/1608.03676) | §2有随机化虚拟speedup、progress points、基线及扰动费用校正，以测优化对吞吐/延迟的影响。txt:130 | 扰动找瓶颈和优化价值已有；E3只延迟一次PTW响应，不满足Coz暂停其他线程并扣时的虚拟加速等价，不能由加延迟敏感性直接推出消除延迟的收益。 |

[iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:165](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)：

| coz2015 | Coz: Finding Code that Counts with Causal Profiling | SOSP 2015 | 3 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 软件虚拟加速的因果剖析；不能宣称因果扰动本身是新方法 | https://arxiv.org/pdf/1608.03676 |

[iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:13](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)：

| Coz: Finding Code that Counts with Causal Profiling | SOSP 2015 | P0 | [来源](https://arxiv.org/pdf/1608.03676) · PDF · 文本 | root_key_sections_checked |

[返回总目录](../README.md)
