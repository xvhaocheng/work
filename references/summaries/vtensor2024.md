# vTensor: Flexible Virtual Tensor Management for Efficient LLM Serving

稳定ID：`vtensor2024` · 类型：`research_paper` · 主题：`data-lifecycle`

作者：unknown；年份：2024；出处：arXiv 2024。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：paged Attention 把内存管理与 compute kernel 绑死：只能跑 CUDA core（在 MQA 上被压到 3.6 TFLOP/s），且要独占预分配内存（`vtensor2024.txt:33-36, 270-350`）。

机制／已存认识：vTensor 抽象；把内存管理与计算解耦；CPU 侧 vTensor Manager（VTP/VTO/VTS）；radix tree 做 prefix match；2MB chunk（`vtensor2024.txt:38-55, 375-401, 448-458`）。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。；不能把 kernel 级加速（3.92×）等同端到端加速（1.86×），也不能把与 vAttention 同名机制混同——vTensor 是 CPU 侧主动管理，vAttention 是系统 demand paging。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://arxiv.org/pdf/2407.15309v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/vtensor2024.pdf) · canonical：[原路径](../../iommu-platform/out/research-landscape-20261005/papers/vtensor2024.pdf) · SHA-256 `662c6f0cf45b646a95ccd499845d1afc399d6ffd8e2b5baa5a30dedaa97114bf`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:244](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:117](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:115](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:111](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)
- [iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:68](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)
- [iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:69](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-proposed.md:110](../../iommu-platform/out/research-landscape-20261005/workers/map-proposed.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-revised.md:115](../../iommu-platform/out/research-landscape-20261005/workers/map-revised.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:117](../../iommu-platform/docs/research-landscape-20261005.md)：

| [vTensor/FlexInfer](https://arxiv.org/pdf/2407.15309v1) | arXiv 2024 | CPU 侧虚拟张量管理，解耦内存管理与 compute kernel | arXiv；LLM 服务框架 + VMM 接口；收益依赖特定 kernel，未承诺通用 |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:69](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "| [vTensor/FlexInfer](https://arxiv.org/pdf/2407.15309v1) | arXiv 2024 | CPU 侧虚拟张量管理，解耦内存管理与 compute kernel | arXiv；LLM 服务框架 + VMM 接口；收益依赖特定 kernel，未承诺通用 |"

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:115](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

| [vTensor/FlexInfer](https://arxiv.org/pdf/2407.15309v1) | arXiv 2024 | CPU 侧虚拟张量管理，解耦内存管理与 compute kernel | arXiv；LLM 服务框架 + VMM 接口；收益依赖特定 kernel，未承诺通用 |

[iommu-platform/out/research-landscape-20261005/workers/map-revised.md:115](../../iommu-platform/out/research-landscape-20261005/workers/map-revised.md)：

| [vTensor/FlexInfer](https://arxiv.org/pdf/2407.15309v1) | arXiv 2024 | CPU 侧虚拟张量管理，解耦内存管理与 compute kernel | arXiv；LLM 服务框架 + VMM 接口；收益依赖特定 kernel，未承诺通用 |

[返回总目录](../README.md)
