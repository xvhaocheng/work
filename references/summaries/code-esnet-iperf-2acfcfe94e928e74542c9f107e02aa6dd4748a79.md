# esnet/iperf/2acfcfe94e928e74542c9f107e02aa6dd4748a79〔正式题名待整理〕

稳定ID：`code-esnet-iperf-2acfcfe94e928e74542c9f107e02aa6dd4748a79` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/esnet/iperf/blob/2acfcfe94e928e74542c9f107e02aa6dd4748a79/src/iperf3.1)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/real-dma-scenario-screen-20261012.md:22](../../iommu-platform/docs/real-dma-scenario-screen-20261012.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/real-dma-scenario-screen-20261012.md:22](../../iommu-platform/docs/real-dma-scenario-screen-20261012.md)：

[SPDK实际完成代码](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/app/spdk_nvme_perf/perf.c)／[系统配置](https://github.com/spdk/spdk/blob/19524ad45373e35d5fdc06f4e081444d87ceed80/doc/system_configuration.md)／[iperf3官方手册](https://github.com/esnet/iperf/blob/2acfcfe94e928e74542c9f107e02aa6dd4748a79/src/iperf3.1)。VFIO group给隔离/权限粒度，示例是I/OAT多通道，不证明NIC/SSD共用翻译缓存或walker；UIO的off/passthrough配置不能当翻译干扰证据。

[返回总目录](../README.md)
