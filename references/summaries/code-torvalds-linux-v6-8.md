# torvalds/linux/v6.8〔正式题名待整理〕

稳定ID：`code-torvalds-linux-v6-8` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://raw.githubusercontent.com/torvalds/linux/v6.8/Documentation/ABI/testing/sysfs-kernel-iommu_groups)
- [来源2](https://raw.githubusercontent.com/torvalds/linux/v6.8/drivers/iommu/intel/dmar.c)
- [来源3](https://raw.githubusercontent.com/torvalds/linux/v6.8/drivers/iommu/iommu.c)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/gpu-ai-deployment-gate-20261012.md:23](../../iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/gpu-ai-deployment-gate-20261012.md:23](../../iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)：

根直接核Linux v6.8参考源码：设备probe后关联iommu_device，Intel的dmar名字对应DRHD所分配的硬件实例；不是仅凭隔离组推断共享。DMA-FQ是翻译域加延迟批量失效，不能当作严格逐次撤销的F&S配置。上游参考源码不冒称Ubuntu6.8.0-60的逐字二进制来源。[域类型ABI](https://raw.githubusercontent.com/torvalds/linux/v6.8/Documentation/ABI/testing/sysfs-kernel-iommu_groups)、[设备关联源码](https://raw.githubusercontent.com/torvalds/linux/v6.8/drivers/iommu/iommu.c)、[Intel实例源码](https://raw.githubusercontent.com/torvalds/linux/v6.8/drivers/iommu/intel/dmar.c)。

[返回总目录](../README.md)
