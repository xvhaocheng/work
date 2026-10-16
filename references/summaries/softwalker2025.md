# SoftWalker: Supporting Software Page Table Walk for Irregular GPU Applications

稳定ID：`softwalker2025` · 类型：`research_paper` · 主题：`gpu`

作者：Sungbin Jang; Junhyeok Park; Yongho Lee; Osang Kwon; Donghyun Kim; Juyoung Seok; Seokin Hong；年份：2025；出处：58th IEEE/ACM International Symposium on Microarchitecture (MICRO ’25)。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：用SM上的专用软件PW Warp执行页表遍历，以GPU空闲周期和TLP扩展page walk并发；In-TLB MSHR把L2 TLB entry临时当MSHR，缓解MSHR饱和；hybrid保留硬件PTW给regular workload。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：不规则GPU应用存在大量stall周期，可被PW Warp利用。；页表遍历涉及特权映射，需要专用PW Warp与普通warp隔离。；driver可预加载PW code并管理PW Warp。；L2 TLB MSHR容量是关键瓶颈之一，可用TLB entry扩容。；regular workload可通过hybrid保留硬件PTW避免 slowdown。

限制：完整设计主要在Accel-sim中评估；实机仅做A2000微基准/profile，不是完整设计实机测量。；纯SoftWalker会让部分regular workload slowdown，需要hybrid。；未在MCM/多chiplet远端PTW场景下评估；只在related work提到MGvm/CLAP。；安全讨论限于PW Warp隔离和未初始化寄存器，未覆盖完整权限/SVA/多租户。；报告平均latency/speedup，未给尾延迟。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[cuptw2026](cuptw2026.md)：mechanism_comparison；[mgvm2022](mgvm2022.md)：mechanism_comparison

原始来源：

- [来源1](https://zoon17.github.io/pdfs/SoftWalker_micro25.pdf)
- [来源2](https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker/)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/softwalker2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.pdf) · SHA-256 `dd448e7efb746d87b71223e520327707847132d78fb17adf3d9ceb8c400c177e`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:17324](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:21](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:102](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:191](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:266](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:608](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:783](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt:1337](../../iommu-platform/out/shared-translation-literature-20261006/papers/softwalker2025.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:167](../../iommu-platform/docs/research-landscape-20261005.md)：

| SoftWalker | MICRO 2025 | SM 软件 walk warps 借计算资源处理翻译；完整限制/评估待读。[作者页](https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker/) |

[iommu-platform/docs/paper-driven-innovation-20261008.md:25](../../iommu-platform/docs/paper-driven-innovation-20261008.md)：

| [SoftWalker，MICRO 2025](https://zoon17.github.io/pdfs/SoftWalker_micro25.pdf)，机制部分 | GPU 软件 walker、翻译专用存储及调度支持 | 复用闲置计算/缓存资源、增加翻译并行度已有 |

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:165](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

| SoftWalker | MICRO 2025 | SM 软件 walk warps 借计算资源处理翻译；完整限制/评估待读。[作者页](https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker/) |

[iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:157](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)：

| SoftWalker | MICRO 2025 | SM 软件 walk warps 借计算资源处理翻译；完整限制/评估待读。[作者页](https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker/) |

[返回总目录](../README.md)
