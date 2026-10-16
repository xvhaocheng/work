# An Open-Source Research Platform for Heterogeneous Systems on Chip

稳定ID：`kurth_thesis2022` · 类型：`thesis` · 主题：`dma-dataflow`

作者：Andreas Kurth；年份：2022；出处：unknown。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：见项目核查记录中明确的问题和适用合同。

机制／已存认识：已存章节讨论写命令/数据锁步避免下游死锁，以及混合IOMMU下DMA恢复/重放和元数据退休缓冲。

支持边界：仅支持已列核读章节/公开补丁；没有本项目复现证据。

限制：论文系统与当前硬件PTW不同；不能把搜索索引近期日期当出版年份或直接移植保证。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：dma-dataflow近邻；用于限定对应版本的主张，不自动选择论文主线。

原始来源：

- [来源1](https://www.research-collection.ethz.ch/server/api/core/bitstreams/d99ddf72-4aab-48fa-8e43-9b0b403bc353/content)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/research-literature-review-20261004.md:90](../../iommu-platform/docs/research-literature-review-20261004.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-literature-review-20261004.md:90](../../iommu-platform/docs/research-literature-review-20261004.md)：

- Kurth，An Open-Source Research Platform for Heterogeneous Systems on Chip，ETH博士论文，2022（不能把搜索索引近期日期当发表年份）。网络demux章节PDF64页说明写命令/数据锁步用于避免下游死锁；本仓库axi_demux_simple确实在W未完成时阻止切换AW银行。第5章介绍混合IOMMU下DMA恢复/重放及元数据退休缓冲，环境与当前硬件PTW不同，不能直接移植或称新贡献。原文：https://www.research-collection.ethz.ch/server/api/core/bitstreams/d99ddf72-4aab-48fa-8e43-9b0b403bc353/content 。

[返回总目录](../README.md)
