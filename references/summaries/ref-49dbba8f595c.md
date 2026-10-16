# MISE〔正式题名待整理〕

稳定ID：`ref-49dbba8f595c` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://research.ece.cmu.edu/safari/pubs/mise-hpca13.pdf)
- [来源2](https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/paper-logic.md:395](../../iommu-platform/docs/paper-logic.md)
- [iommu-platform/docs/independent-research-directions-20261015.md:41](../../iommu-platform/docs/independent-research-directions-20261015.md)
- [iommu-platform/out/independent-directions-20261015/methods/assessment.md:44](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md:16](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)
- [iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:168](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/independent-research-directions-20261015.md:41](../../iommu-platform/docs/independent-research-directions-20261015.md)：

| [MISE，HPCA2013](https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf)，§3–4；[ASM，MICRO2015](https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf)，§3–4；[Coz，SOSP2015](https://arxiv.org/pdf/1608.03676)，§2 | 优先级干预估计独占服务、cache/memory共同干扰、因果剖析和优化价值都有前史 | 需在同预算适配中指出它们对翻译—载荷反馈的具体误判及改进。单次“增加PTW延迟”的敏感性不能直接当消除延迟的收益 |

[iommu-platform/docs/paper-logic.md:395](../../iommu-platform/docs/paper-logic.md)：

已经有 [IOMMU 按翻译请求使用信用额度的公开技术](https://patents.justia.com/patent/20250181518)；[AXI-REALM](https://arxiv.org/html/2501.10161v1)使用按时间窗口补充、按传输 beat 扣减的入口预算，并另外监控出口事务阶段时限；[MISE](https://research.ece.cmu.edu/safari/pubs/mise-hpca13.pdf)研究基于服务率的性能控制。它们不等于本候选全部实现，但足以排除“加令牌桶/反馈就是创新”。

[iommu-platform/out/independent-directions-20261015/methods/assessment.md:44](../../iommu-platform/out/independent-directions-20261015/methods/assessment.md)：

| [MISE，HPCA2013](https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf)及[ASM，MICRO2015](https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf) | MISE§3/4短期提升请求优先级估计alone服务率，另处理compute fraction；ASM§3/4再结合辅助tag估计缓存容量干扰，向QoS/公平控制供给slowdown。MISE已指出单请求等待可被并行隐藏。MISE txt:161、219、ASM txt:292 | 带干预的性能估计、联合cache/memory处理、分期在线控制已有。D须在具名适配中展示翻译反馈产生的具体估计/选择失效，而不是称一般性能归因全新。 |

[iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md:168](../../iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/revision.md)：

| mise2013 | MISE: Providing Performance Predictability and Improving Fairness in Shared Main Memory Systems | HPCA 2013 | 3 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 以服务速率和短时优先估计独跑性能；不等于已解决设备翻译归因 | https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf |

[返回总目录](../README.md)
