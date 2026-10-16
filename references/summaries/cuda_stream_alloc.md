# CUDA stream-ordered memory allocator〔正式题名待整理〕

稳定ID：`cuda_stream_alloc` · 类型：`technical_document` · 主题：`data-lifecycle`

作者：unknown；年份：unknown；出处：Official technical blog。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：既有广域地图的逐篇证据卡，问题范围以该记录为准。

机制／已存认识：复用既有报告，见下面source_context；未根据短题名补写机制。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://developer.nvidia.com/blog/using-cuda-stream-ordered-memory-allocator-part-1/)

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:467](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:135](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:133](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:129](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-proposed.md:128](../../iommu-platform/out/research-landscape-20261005/workers/map-proposed.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-revised.md:133](../../iommu-platform/out/research-landscape-20261005/workers/map-revised.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:135](../../iommu-platform/docs/research-landscape-20261005.md)：

| CUDA stream-ordered allocator | stream-ordered 分配/释放、pool、release threshold、event 依赖复用；不承诺零分配延迟 | [CUDA stream-ordered memory allocator](https://developer.nvidia.com/blog/using-cuda-stream-ordered-memory-allocator-part-1/) |

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:133](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

| CUDA stream-ordered allocator | stream-ordered 分配/释放、pool、release threshold、event 依赖复用；不承诺零分配延迟 | [CUDA stream-ordered memory allocator](https://developer.nvidia.com/blog/using-cuda-stream-ordered-memory-allocator-part-1/) |

[iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:129](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)：

| CUDA stream-ordered allocator | stream-ordered 分配/释放、pool、release threshold、event 依赖复用；不承诺零分配延迟 | [CUDA stream-ordered memory allocator](https://developer.nvidia.com/blog/using-cuda-stream-ordered-memory-allocator-part-1/) |

[iommu-platform/out/research-landscape-20261005/workers/map-proposed.md:128](../../iommu-platform/out/research-landscape-20261005/workers/map-proposed.md)：

| CUDA stream-ordered allocator | stream-ordered 分配/释放、pool、release threshold、event 依赖复用；不承诺零分配延迟 | [CUDA stream-ordered memory allocator](https://developer.nvidia.com/blog/using-cuda-stream-ordered-memory-allocator-part-1/) |

[返回总目录](../README.md)
