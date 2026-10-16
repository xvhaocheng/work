# Arm官方PDF〔正式题名待整理〕

稳定ID：`ref-5649e50eaafd` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://documentation-service.arm.com/static/5e908aac8259fe2368e2b389)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md:130](../../iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md:130](../../iommu-platform/out/flow-iommu-candidates-20261014/fast-hit/analysis.md)：

| Arm CoreLink MMU-500 TRM DDI0517B | [Arm官方PDF](https://documentation-service.arm.com/static/5e908aac8259fe2368e2b389)，§2.4.1，2-16页 | TBU有fully associative可配置depth microTLB，缓存TCU返回PTW结果，先查microTLB再TCU。已存在本地命中/中心miss分层，因此把本地fast plane本身当独立创新不成立。本文读取到官方搜索索引该节内容；直接open PDF遇Internal Error，未据此延伸FIFO/credit实现细节 |

[返回总目录](../README.md)
