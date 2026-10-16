# MGG 官方摘要〔正式题名待整理〕

稳定ID：`ref-501aa6b4c0a4` · 类型：`research_record` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://www.usenix.org/conference/osdi23/presentation/wang-yuke)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/innovation-council18-20261005/round1/C6.md:50](../../iommu-platform/out/innovation-council18-20261005/round1/C6.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md:50](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C6.md:50](../../iommu-platform/out/innovation-council18-20261005/round1/C6.md)：

第一目标应用不是人为构造无限等待的 kernel，而是 WIC 已使用的 CPU–GPU 有限元/共轭梯度、跨 GPU stencil/halo 交换：消费者先做本地独立工作，再等待外部生产的数据；各分块完成时间不均。第二个待核对象是多 GPU GNN 的细粒度聚合，MGG 已说明其通信计算流水的重要性。[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) §3、§4；[MGG 官方摘要](https://www.usenix.org/conference/osdi23/presentation/wang-yuke)

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md:50](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C6.md)：

第一目标应用不是人为构造无限等待的 kernel，而是 WIC 已使用的 CPU–GPU 有限元/共轭梯度、跨 GPU stencil/halo 交换：消费者先做本地独立工作，再等待外部生产的数据；各分块完成时间不均。第二个待核对象是多 GPU GNN 的细粒度聚合，MGG 已说明其通信计算流水的重要性。[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) §3、§4；[MGG 官方摘要](https://www.usenix.org/conference/osdi23/presentation/wang-yuke)

[返回总目录](../README.md)
