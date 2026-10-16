# Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes

稳定ID：`mosaic2017` · 类型：`research_paper` · 主题：`gpu`

作者：Rachata Ausavarungnirun; Joshua Landgraf; Vance Miller; Saugata Ghose; Jayneel Gandhi; Christopher J. Rossbach; Onur Mutlu；年份：2017；出处：MICRO-50 (The 50th Annual IEEE/ACM International Symposium on Microarchitecture), Oct 14-18 2017, Cambridge, MA, USA。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：应用透明的多页大小 GPU 内存管理器：CoCoA 连续性保持分配（大页帧只属单一保护域）+ In-Place Coalescer（分配后原地合并，无数据迁移、无需 TLB flush）+ CAC（碎片高时分裂并在同通道内压缩迁移）；数据按 4KB 基页传输、地址翻译用 2MB 大页。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：GPGPU 应用 en masse 分配（一次申请大块虚拟内存）；大页帧内基页必须虚拟物理都连续、与虚拟大页对齐、同属一个保护域（软保证）；页表改造：L3 PTE 加 large page bit，L4 PTE 加 disabled bit；large page bit 置位后大页映射即可用，不必等 L4 disabled bit 也不必刷 TLB；仅当大页帧全满才合并；分裂时必须刷 TLB 大页条目；GPGPU-Sim + MAFIA 模拟；PCIe 延迟取自 NVIDIA GTX 1080 白皮书数据

限制：纯模拟；CAC 用最坏情形建模（stall 所有 SM 并刷流水线）；评估负载中未观察到 CoCoA 软保证被破坏，但长时运行可能引入碎片与内存膨胀（作者自述）；单 GPU；不涉及多租户共享 L3 TLB、多设备共享翻译、远端 PTW；压缩迁移被限制在同一 DRAM 通道内；in-DRAM bulk copy 为可选增强；未覆盖页错误效率、失效传播协议

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：gpu主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf)
- [来源2](https://people.inf.ethz.ch/omutlu/pub/mosaic-application-transparent-multiple-page-sizes-for-GPUs_micro17.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/mosaic2017.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.pdf) · SHA-256 `0642eca394e04d5e19c8ad02c9ec405609daf045a2bd85e7668a30435f062716`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:11830](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt:36](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt:532](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt:180](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt:306](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt:656](../../iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:43](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:113](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Mosaic](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf) | MICRO 2017 | GPU 多页大小、CoCoA、原地合并/拆分 | GPGPU-Sim；依赖 en masse allocation |

[iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md:37](../../iommu-platform/out/mapping-organization-source-20261016/root/decision-analysis.md)：

| [Mosaic](https://people.inf.ethz.ch/omutlu/pub/mosaic-application-transparent-multiple-page-sizes-for-GPUs_micro17.pdf)，MICRO 2017，§4.2—4.4 | 保持连续性、单应用大页、合并与压缩；§4.2直接讨论先迁走其他应用页面再合并 | “搬走异域少数、保留多数”本身不新。未知路由后的 DMA 在途、pool归还和整页授权协议需另证，换网络场景本身不足 |

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:111](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

| [Mosaic](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf) | MICRO 2017 | GPU 多页大小、CoCoA、原地合并/拆分 | GPGPU-Sim；依赖 en masse allocation |

[iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md:107](../../iommu-platform/out/research-landscape-20261005/workers/L5_map_draft.md)：

| [Mosaic](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf) | MICRO 2017 | GPU 多页大小、CoCoA、原地合并/拆分 | GPGPU-Sim；依赖 en masse allocation |

[返回总目录](../README.md)
