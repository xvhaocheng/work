# Reducing Page Faults via Invalidation-Based Mapping Propagation in Multi-GPU Systems

稳定ID：`shadowupdate2026` · 类型：`research_paper` · 主题：`lifecycle`

作者：Junsung Kim; Dongho Ha; Sungwoo Kim; Wonho Cho; Sungbin Kim; Yufei Ding; Won Woo Ro；年份：2026；出处：ISCA 2026。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：机构摘要明确失效时传播新映射、在途迁移tracker与拷贝完成后放行；作者PDF按钮无有效目标，未获全文。

支持边界：仅题录/摘要/索引片段，不得用于全文机制覆盖或收益裁决。

限制：原记录未单独列限制；不能据此认为没有限制。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/)
- [来源2](https://doi.org/10.1109/ISCA66397.2026.00110)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:15790](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/innovation-council18-20261005/PRIMARY_CHECKS.md:7](../../iommu-platform/out/innovation-council18-20261005/PRIMARY_CHECKS.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C2.md:33](../../iommu-platform/out/innovation-council18-20261005/round2/C2.md)
- [iommu-platform/docs/shared-translation-literature-20261006.md:65](../../iommu-platform/docs/shared-translation-literature-20261006.md)
- [iommu-platform/docs/translation-comparison-20261006.md:58](../../iommu-platform/docs/translation-comparison-20261006.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C3.md:141](../../iommu-platform/out/innovation-council18-20261005/round2/C3.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:424](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)
- [iommu-platform/out/shared-translation-literature-20261006/INDEX.md:492](../../iommu-platform/out/shared-translation-literature-20261006/INDEX.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/shared-translation-literature-20261006.md:65](../../iommu-platform/docs/shared-translation-literature-20261006.md)：

**3. 稳态查找做得快，不保证映射变化时整体仍快。** IDYLL、UNITD、HATRIC、LATR、ShadowUpdate已经分别处理不同更新问题；需要先理解它们的语义。HDPAT明确没有评估迁移与shootdown，WaferTrans的维护成本报告来自迁移不频繁的条件。因而值得查的是高更新率、异构完成时间与大规模共享同时出现时的代价，而不能直接提出“加一个版本号／目录／定向失效”作为新贡献。HDPAT全文 · [ShadowUpdate机构摘要](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/)

[iommu-platform/docs/translation-comparison-20261006.md:58](../../iommu-platform/docs/translation-comparison-20261006.md)：

| [ShadowUpdate，ISCA 2026，机构摘要](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/) | 利用已有失效广播传播新映射；跟踪在途迁移，页拷贝完成后再释放请求。 | “迁移时顺带传播映射、避免随后重新缺页”已经被提出。本轮仍未取得全文，不能确定其完整传播范围或开销模型。 |

[iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/shared-translation-literature-20261006.md:63](../../iommu-platform/out/shared-translation-literature-20261006/expansion-01/baseline/shared-translation-literature-20261006.md)：

**3. 稳态查找做得快，不保证映射变化时整体仍快。** IDYLL、UNITD、HATRIC、LATR、ShadowUpdate已经分别处理不同更新问题；需要先理解它们的语义。HDPAT明确没有评估迁移与shootdown，WaferTrans的维护成本报告来自迁移不频繁的条件。因而值得查的是高更新率、异构完成时间与大规模共享同时出现时的代价，而不能直接提出“加一个版本号／目录／定向失效”作为新贡献。HDPAT全文 · [ShadowUpdate机构摘要](https://yonsei.elsevierpure.com/en/publications/reducing-page-faults-via-invalidation-based-mapping-propagation-i/)

[iommu-platform/out/innovation-council18-20261005/round2/C2.md:33](../../iommu-platform/out/innovation-council18-20261005/round2/C2.md)：

共同新增近邻的证据边界：DATE 2025 SVA/LLC 结果依照 `/home/xvhaocheng/work/iommu-platform/out/innovation-council18-20261005/PRIMARY_CHECKS.md`，原文 [§III-A、§IV-C](https://arxiv.org/html/2502.17398v1)；本席本轮未额外核完整评估。ShadowUpdate 仅有该文件记录的[正式题录/摘要](https://doi.org/10.1109/ISCA66397.2026.00110)，不能据此淘汰任意回收/完成协议。更广的缓存闭环队列、实时界、LATR/EcoTLB 与 metastability 文献，本轮没有补齐原文；下面明确作为待核项，不假装已证明覆盖或空白。

[返回总目录](../README.md)
