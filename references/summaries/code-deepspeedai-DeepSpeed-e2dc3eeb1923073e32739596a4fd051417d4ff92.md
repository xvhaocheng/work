# deepspeedai/DeepSpeed/e2dc3eeb1923073e32739596a4fd051417d4ff92〔正式题名待整理〕

稳定ID：`code-deepspeedai-DeepSpeed-e2dc3eeb1923073e32739596a4fd051417d4ff92` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://raw.githubusercontent.com/deepspeedai/DeepSpeed/e2dc3eeb1923073e32739596a4fd051417d4ff92/docs/assets/files/SC21-ZeRO-Infinity.pdf)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md:52](../../iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md:52](../../iommu-platform/out/stage114-c113-native-evidence-20261013/workers/published-boundary/report.md)：

固定DeepSpeed sha下下载的官方 [SC21-ZeRO-Infinity.pdf](https://raw.githubusercontent.com/deepspeedai/DeepSpeed/e2dc3eeb1923073e32739596a4fd051417d4ff92/docs/assets/files/SC21-ZeRO-Infinity.pdf) 是23页报告slides。第3页图再次显示单节点0.5／1.5／28TB，第11页是overlap、DeepNVMe和pinned层示意，第14–18页是模型规模／吞吐／scaling／特性图。它没有native run JSON、launcher、路径、rank map、IOMMU配置或原始完成日志。文件元数据creation为2023-10-19 UTC；这只说明归档PDF的导出元数据，既不能推出SC21实验发生在2023，也不能证明运行代码版本。固定asset所在版本不等于实验执行版本。

[返回总目录](../README.md)
