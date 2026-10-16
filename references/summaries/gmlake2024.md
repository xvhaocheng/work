# GMLake: Efficient and Transparent GPU Memory Defragmentation for Large-scale DNN Training with Virtual Memory Stitching

稳定ID：`gmlake2024` · 类型：`research_paper` · 主题：`data-lifecycle`

作者：unknown；年份：2024；出处：ASPLOS 2024。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：PyTorch caching allocator 在 recomputation/offloading/LoRA/分布式训练下碎片高达 30%（`gmlake2024.txt:24-113`）。

机制／已存认识：VMS（Virtual Memory Stitching）；pBlock 物理原语池 + sBlock 缝合池；用 VMM API 把多个非连续物理块映射到一个虚拟地址（`gmlake2024.txt:105-116, 364-466`）；BestFit 四状态（`gmlake2024.txt:469-522`）。

支持边界：既有已存全文与逐篇笔记；本轮不重审评价。

限制：未复现论文实验；仅存全文不等于全文精读。；这是训练场景，不能直接外推到推理；"训练有周期性所以缝合可摊销"这一前提不适用于推理。（与 vAttention 互补：vAttention 用重叠，GMLake 用周期性。）

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：数据搬运、资源组织与动态内存近邻，历史收录不代表当前采用。

原始来源：

- [来源1](https://arxiv.org/pdf/2401.08156)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/gmlake2024.pdf) · canonical：[原路径](../../iommu-platform/out/research-landscape-20261005/papers/gmlake2024.pdf) · SHA-256 `99903eef4b04b25eadd60061e5c724505b803fc16adf7d666e1203e712097eb8`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/research-landscape-20261005/source-manifest.json:255](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:118](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:116](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/round1/C5.md:20](../../iommu-platform/out/innovation-council18-20261005/round1/C5.md)
- [iommu-platform/out/innovation-council18-20261005/round1-accepted/C5.md:20](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C5.md)
- [iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:112](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-proposed.md:111](../../iommu-platform/out/research-landscape-20261005/workers/map-proposed.md)
- [iommu-platform/out/research-landscape-20261005/workers/map-revised.md:116](../../iommu-platform/out/research-landscape-20261005/workers/map-revised.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:118](../../iommu-platform/docs/research-landscape-20261005.md)：

| [GMLake](https://arxiv.org/pdf/2401.08156) | ASPLOS 2024 | VMS 缝合物理块，训练碎片整理 | 真机 8×A100；裸 VMM 慢 115× 是特定条件，不能说所有动态映射必须有缓存池 |

[iommu-platform/out/innovation-council18-20261005/round1/C5.md:20](../../iommu-platform/out/innovation-council18-20261005/round1/C5.md)：

| G：[GMLake，提供的 2024 预印本](https://arxiv.org/pdf/2401.08156) | gmlake2024.txt，317–382、424–448、495–592、617–652、954–977 | 拼接复用已有物理块，释放首先更新状态。映射成本高，重复分配模式收敛后才能摊薄；极端不规则分配并未被无条件解决。文中 peak active/peak reserved 的碎片指标不能当成每个时刻的实际碎片率。 |

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C5.md:20](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C5.md)：

| G：[GMLake，提供的 2024 预印本](https://arxiv.org/pdf/2401.08156) | gmlake2024.txt，317–382、424–448、495–592、617–652、954–977 | 拼接复用已有物理块，释放首先更新状态。映射成本高，重复分配模式收敛后才能摊薄；极端不规则分配并未被无条件解决。文中 peak active/peak reserved 的碎片指标不能当成每个时刻的实际碎片率。 |

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:116](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

| [GMLake](https://arxiv.org/pdf/2401.08156) | ASPLOS 2024 | VMS 缝合物理块，训练碎片整理 | 真机 8×A100；裸 VMM 慢 115× 是特定条件，不能说所有动态映射必须有缓存池 |

[返回总目录](../README.md)
