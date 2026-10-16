# pulp-platform/hero/c8481a930a15a385d4f58f519bed83d2e75e2a73〔正式题名待整理〕

稳定ID：`code-pulp-platform-hero-c8481a930a15a385d4f58f519bed83d2e75e2a73` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/hardware/src/pulp_cluster_cfg_pkg.sv)
- [来源2](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/README.md)
- [来源3](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/mm-large/mm-large.c)
- [来源4](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/testset.cfg)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/docs/workflow-stream-platform-review-20261012.md:18](../../iommu-platform/docs/workflow-stream-platform-review-20261012.md)
- [iommu-platform/docs/workflow-stream-background-demand-20261012.md:11](../../iommu-platform/docs/workflow-stream-background-demand-20261012.md)
- [iommu-platform/docs/workflow-stream-contract-20261012.md:11](../../iommu-platform/docs/workflow-stream-contract-20261012.md)
- [iommu-platform/out/research-reappraisal-20261015/candidates/b-development.md:11](../../iommu-platform/out/research-reappraisal-20261015/candidates/b-development.md)
- [iommu-platform/docs/workflow-stream-background-demand-20261012.md:12](../../iommu-platform/docs/workflow-stream-background-demand-20261012.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/workflow-stream-contract-20261012.md:11](../../iommu-platform/docs/workflow-stream-contract-20261012.md)：

采用PULP官方HERO仓库固定提交`c8481a930a15a385d4f58f519bed83d2e75e2a73`的[mm-large.c](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/mm-large/mm-large.c)。原函数44—276行、默认参数284—305行、初始化320—326行及参考335—346行已直接核查。许可证和22份定向源码原样保留，来源清单给出网址、提交和哈希。

[iommu-platform/docs/workflow-stream-platform-review-20261012.md:18](../../iommu-platform/docs/workflow-stream-platform-review-20261012.md)：

固定HERO配置与端口来自[官方配置源码](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/hardware/src/pulp_cluster_cfg_pkg.sv)及bank实例。HEROv2另有实际多核FPGA实现，但其器件、核、存储、工具和资源预算须分别对应，不能按文献核数缩放R2时间。[HEROv2原论文](https://arxiv.org/pdf/2201.03861v1)

[iommu-platform/docs/workflow-stream-background-demand-20261012.md:11](../../iommu-platform/docs/workflow-stream-background-demand-20261012.md)：

- [官方应用说明](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/README.md)第4—17行提供各应用构建及Polybench线程、存储、DMA和offload选项，没有所选独立后台的到达/完成合同。计算线程并行不能直接当跨设备后台。

[iommu-platform/docs/workflow-stream-background-demand-20261012.md:12](../../iommu-platform/docs/workflow-stream-background-demand-20261012.md)：

- [官方测试入口](https://github.com/pulp-platform/hero/blob/c8481a930a15a385d4f58f519bed83d2e75e2a73/openmp-examples/testset.cfg)第5—15行登记四个子测试，没有描述共跑关系。未读取测试调度器，不断言测试一概串行或绝不并发；测试列表本身不足以提供应用轨迹。

[返回总目录](../README.md)
