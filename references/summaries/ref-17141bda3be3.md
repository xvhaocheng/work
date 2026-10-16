# Taming Non-blocking Caches，RTAS2016〔正式题名待整理〕

稳定ID：`ref-17141bda3be3` · 类型：`research_paper` · 主题：`supplementary-research`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：supplementary-research资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://ittc.ku.edu/~heechul/papers/taming-rtas2016-camera.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/independent-research-directions-20261015.md:106](../../iommu-platform/docs/independent-research-directions-20261015.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/independent-research-directions-20261015.md:106](../../iommu-platform/docs/independent-research-directions-20261015.md)：

[Taming Non-blocking Caches，RTAS2016](https://ittc.ku.edu/~heechul/papers/taming-rtas2016-camera.pdf)§II/§V已说明容量分区仍会被共享MSHR耗尽阻塞，并用OS控制在途额度；[MASK，ASPLOS2018](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)§5.2–5.4已有TLB准入、PTE缓存旁路和页表/数据内存调度。root重核这两处关键设计，故“容量与并发共同保护”“页表优先”不能再宽称新。

[返回总目录](../README.md)
