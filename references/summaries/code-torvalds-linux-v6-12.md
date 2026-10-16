# torvalds/linux/v6.12〔正式题名待整理〕

稳定ID：`code-torvalds-linux-v6-12` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/torvalds/linux/blob/v6.12/drivers/iommu/iommu.c)
- [来源2](https://github.com/torvalds/linux/blob/v6.12/drivers/vfio/vfio_iommu_type1.c)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md:11](../../iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md:11](../../iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md)：

- Linux `v6.12` 两个原件只用来展示一个明确、可核查的VFIO type1 → IOMMU实现链，**不是该SPDK版本声明的唯一支持内核，也不是选定机器的已部署内核**。不把这段参考代码反推成Dell、NIC或SSD实况。[VFIO参考源码](https://github.com/torvalds/linux/blob/v6.12/drivers/vfio/vfio_iommu_type1.c)、[IOMMU参考源码](https://github.com/torvalds/linux/blob/v6.12/drivers/iommu/iommu.c)。

[返回总目录](../README.md)
