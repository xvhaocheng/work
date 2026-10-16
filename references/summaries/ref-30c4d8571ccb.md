# AMAC 原文 §3〔正式题名待整理〕

稳定ID：`ref-30c4d8571ccb` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://www.vldb.org/pvldb/vol9/p252-kocberber.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/innovation-council18-20261005/round1/C4.md:16](../../iommu-platform/out/innovation-council18-20261005/round1/C4.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C4.md:16](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C4.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C4.md:16](../../iommu-platform/out/innovation-council18-20261005/round1/C4.md)：

**一个实质性补充：地图还低估了软件对手。** Milk 已有延迟访问、协程式 collection/distribution/delivery、TLB 与缓存容量约束、DRAM bank 排布，以及需要时保留串行次序；AMAC 已经按 lookup 保存独立状态，适应可变链长和早退。不能把软件对手写成“朴素按原顺序逐个访问”。[Milk 原文 §3、§5.2](https://commit.csail.mit.edu/papers/2016/kiriansky16pact-milk.pdf)、[AMAC 原文 §3](https://www.vldb.org/pvldb/vol9/p252-kocberber.pdf)。本轮补充阅读范围见来源记录。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C4.md:16](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C4.md)：

**一个实质性补充：地图还低估了软件对手。** Milk 已有延迟访问、协程式 collection/distribution/delivery、TLB 与缓存容量约束、DRAM bank 排布，以及需要时保留串行次序；AMAC 已经按 lookup 保存独立状态，适应可变链长和早退。不能把软件对手写成“朴素按原顺序逐个访问”。[Milk 原文 §3、§5.2](https://commit.csail.mit.edu/papers/2016/kiriansky16pact-milk.pdf)、[AMAC 原文 §3](https://www.vldb.org/pvldb/vol9/p252-kocberber.pdf)。本轮补充阅读范围见来源记录。

[返回总目录](../README.md)
