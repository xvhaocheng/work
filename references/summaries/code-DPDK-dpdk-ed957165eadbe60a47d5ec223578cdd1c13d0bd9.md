# DPDK/dpdk/ed957165eadbe60a47d5ec223578cdd1c13d0bd9〔正式题名待整理〕

稳定ID：`code-DPDK-dpdk-ed957165eadbe60a47d5ec223578cdd1c13d0bd9` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/DPDK/dpdk/blob/ed957165eadbe60a47d5ec223578cdd1c13d0bd9/examples/vhost/main.c#L1118)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/stage117-r3-astra-20261013.md:7](../../iommu-platform/docs/stage117-r3-astra-20261013.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/stage117-r3-astra-20261013.md:7](../../iommu-platform/docs/stage117-r3-astra-20261013.md)：

冻结 DPDK v25.11，commit `ed957165eadbe60a47d5ec223578cdd1c13d0bd9`。唯一任务是 `examples/vhost/main.c` 的 `vm2vm=1` 本地单播分支：客体 A 的 TX 包按目的 MAC 送到客体 B 的 RX 队列。不是上一轮 dmafwd，不把文档的 PVP 回环测试或规定复制次数作为有用输出。[源码 main.c](https://github.com/DPDK/dpdk/blob/ed957165eadbe60a47d5ec223578cdd1c13d0bd9/examples/vhost/main.c#L1118)。

[返回总目录](../README.md)
