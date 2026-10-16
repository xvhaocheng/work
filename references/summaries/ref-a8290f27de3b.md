# CounterPoint，ASPLOS 2026〔正式题名待整理〕

稳定ID：`ref-a8290f27de3b` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:39](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/docs/novelty-gate-20261006.md:47](../../iommu-platform/docs/novelty-gate-20261006.md)
- [iommu-platform/docs/shared-translation-gaps-20261006.md:14](../../iommu-platform/docs/shared-translation-gaps-20261006.md)
- [iommu-platform/out/independent-directions-20261015/methods/assessment.md:42](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:26](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:178](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/independent-research-directions-20261015.md:39](../../iommu-platform/docs/independent-research-directions-20261015.md)：

| [CounterPoint，ASPLOS2026](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf)，§2–4 | 以事件路径模型和带相关噪声的计数置信域检查模型，分析缺观测时错误模型不可辨识 | 检验目标要从模型一致性推进到具名控制的完整决策后果；也必须处理噪声，不能把计数接近当严格等价 |

[iommu-platform/docs/novelty-gate-20261006.md:47](../../iommu-platform/docs/novelty-gate-20261006.md)：

- [CounterPoint，ASPLOS 2026](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf)用硬件计数器与模型约束检验/修正微架构假设，案例涉及CPU地址翻译。B若只说“增加观测、找到计数和性能不一致”，不足以体现方法增量；CounterPoint本身也不是在线IOMMU配置选择器。

[iommu-platform/docs/shared-translation-gaps-20261006.md:14](../../iommu-platform/docs/shared-translation-gaps-20261006.md)：

| [CounterPoint](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf) | ASPLOS 2026；取得全文，关键段已核 | 已用带噪硬件计数器检验/修正微架构假设，案例就是Haswell MMU。第三候选需直接对照。 |

[iommu-platform/out/independent-directions-20261015/methods/assessment.md:42](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)：

| [CounterPoint，ASPLOS2026](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf) | §2–4把微操作路径的事件组映射为模型锥，结合相关噪声置信域做可行性反证/模型修订；已有缺计数器时错误模型不可检出的例子。txt:280、330、351 | 模型+多计数器+反例校核及“某观测不可缺”已有。它主要检验模型与计数一致性；D需证明对实际动作选择的价值，不能只输出原因分类。 |

[返回总目录](../README.md)
