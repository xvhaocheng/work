# Linux 6.17 DMA API，Part Ic/Id〔正式题名待整理〕

稳定ID：`ref-ff3ad51d833c` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://docs.kernel.org/6.17/core-api/dma-api.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/innovation-council18-20261005/round2/C1.md:27](../../iommu-platform/out/innovation-council18-20261005/round2/C1.md)
- [iommu-platform/out/innovation-council18-20261005/round2/C1.md:126](../../iommu-platform/out/innovation-council18-20261005/round2/C1.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round2/C1.md:27](../../iommu-platform/out/innovation-council18-20261005/round2/C1.md)：

- 双射不自动解决 DMA mask、保留 IOVA 区间、同 PA 多重映射、连续段长度和 SG 描述符限制。Linux 官方文档明确设备地址受 mask 约束、`dma_map_sg` 可合并多个段；通用逐页置换可能破坏该机会。可以限定单页或保留连续段，但必须重新统计覆盖率，不能继续无条件声称构造成功。见 [Linux 6.17 DMA API，Part Ic/Id](https://docs.kernel.org/6.17/core-api/dma-api.html)。

[iommu-platform/out/innovation-council18-20261005/round2/C1.md:126](../../iommu-platform/out/innovation-council18-20261005/round2/C1.md)：

- [Linux 6.17 DMA API](https://docs.kernel.org/6.17/core-api/dma-api.html)：Part Ic/Id 的 mask、mapping failure、SG 合并与同步接口；用于核对 A1 的接口成本。

[返回总目录](../README.md)
