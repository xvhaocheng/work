# spdk/dpdk/8d8db71763ccc7f092e3d66dbc8b32deaaf69fb4〔正式题名待整理〕

稳定ID：`code-spdk-dpdk-8d8db71763ccc7f092e3d66dbc8b32deaaf69fb4` · 类型：`source_code` · 主题：`software-artifacts`

作者：unknown；年份：unknown；出处：unknown。

阅读等级：**仅题录/摘要/提及或核读范围待整理**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：项目记录中用于何种问题，见逐条来源上下文。

机制／已存认识：本轮未独立抽取方法；仅保留项目记录已明确写出的内容，不由题名补结论。

支持边界：公开组件及固定版本源码引用；不是论文，也不代表本轮运行过artifact。

限制：作者/年份/完整题名未归一时显式unknown；正文范围和作者结果未在本轮重新核验。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：software-artifacts资料；是否当前采用须回到总计划和本轮接受记录。

原始来源：

- [来源1](https://github.com/spdk/dpdk/blob/8d8db71763ccc7f092e3d66dbc8b32deaaf69fb4/lib/eal/linux/eal_vfio.c#L802)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md:33](../../iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md:33](../../iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md)：

固定DPDK `eal_vfio.c:802–918`：设备所属VFIO group加入container，primary process在首个active group选择IOMMU type；default container的既有memseg由`dma_map_func`映射，之后注册内存事件回调。`1399–1477`的type1链遍历memseg，提交包含`vaddr/iova/len`及READ/WRITE权限的`VFIO_IOMMU_MAP_DMA`；`606–657`在后续ALLOC/FREE按memseg粒度map/unmap。IOVA=VA时同样需要映射，不表示绕过翻译。No-IOMMU是代码中的另一个类型，不能因使用VFIO就自动认定translated。[固定DPDK源码](https://github.com/spdk/dpdk/blob/8d8db71763ccc7f092e3d66dbc8b32deaaf69fb4/lib/eal/linux/eal_vfio.c#L802)。

[返回总目录](../README.md)
