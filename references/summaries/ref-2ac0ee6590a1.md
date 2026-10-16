# 官方接口〔正式题名待整理〕

稳定ID：`ref-2ac0ee6590a1` · 类型：`technical_document` · 主题：`standards-software`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：项目已有引用/局部核查记录；不能据此视为全文精读或完成新颖性比较。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：standards-software资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://docs.nvidia.com/cuda/cuda-runtime-api/cuda_runtime_api/group__CUDART__MEMORY__POOLS.html)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/innovation-council18-20261005/round1/C3.md:162](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:162](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round1/C3.md:162](../../iommu-platform/out/innovation-council18-20261005/round1/C3.md)：

- **S9 CUDA Runtime API: Stream Ordered Memory Allocator**：[官方接口](https://docs.nvidia.com/cuda/cuda-runtime-api/cuda_runtime_api/group__CUDART__MEMORY__POOLS.html)。网页定向阅读 `cudaMemPoolGetAttribute/SetAttribute`、`cudaMemPoolTrimTo` 与 imported allocation 的完成约束；没有测试接口，也没有核验任意硬件/运行时组合都支持。页面检索当时显示 Runtime 13.4；这里只用接口能力建立强基线，部署版本仍需以目标系统为准。

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md:162](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C3.md)：

- **S9 CUDA Runtime API: Stream Ordered Memory Allocator**：[官方接口](https://docs.nvidia.com/cuda/cuda-runtime-api/cuda_runtime_api/group__CUDART__MEMORY__POOLS.html)。网页定向阅读 `cudaMemPoolGetAttribute/SetAttribute`、`cudaMemPoolTrimTo` 与 imported allocation 的完成约束；没有测试接口，也没有核验任意硬件/运行时组合都支持。页面检索当时显示 Runtime 13.4；这里只用接口能力建立强基线，部署版本仍需以目标系统为准。

[返回总目录](../README.md)
