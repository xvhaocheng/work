# Quest 原文，§2.2、§3.4–3.5、§4.3〔正式题名待整理〕

稳定ID：`arxiv-2406.10774` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://arxiv.org/html/2406.10774v1)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/innovation-council18-20261006.md:70](../../iommu-platform/docs/innovation-council18-20261006.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C5.md:26](../../iommu-platform/out/innovation-council18-20261005/round2/C5.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/innovation-council18-20261006.md:70](../../iommu-platform/docs/innovation-council18-20261006.md)：

- **KV粒度并非无人研究。** Quest§3.5核算选择元数据和选中页字节；ShadowKV§4/§5.3联合处理选择与CPU回取，并比较chunk大小对精度、batch和命中率的影响。根定向复核，因此A5必须提出这些分析之外的精确差异。[Quest](https://arxiv.org/html/2406.10774v1) · [ShadowKV](https://arxiv.org/html/2410.21465v1)

[iommu-platform/out/innovation-council18-20261005/round2/C5.md:26](../../iommu-platform/out/innovation-council18-20261005/round2/C5.md)：

- **Quest** 已用页内 Key 的最小/最大值估计页重要性，选择页做注意力；§3.5 已显式核算元数据与选中页的数据量，§4.3 比较相关算子成本。它保留完整 KV，不应被误称为已经解决 CPU 分层驻留。[Quest 原文，§2.2、§3.4–3.5、§4.3](https://arxiv.org/html/2406.10774v1)

[返回总目录](../README.md)
