# Fast & Safe IO Memory Protection

稳定ID：`fastsafe2024` · 类型：`research_paper` · 主题：`io`

作者：Benny Rubin; Saksham Agarwal; Qizhe Cai; Rachit Agarwal；年份：2024；出处：ACM SIGOPS 30th Symposium on Operating Systems Principles (SOSP '24), November 4-6, 2024, Austin, TX, USA。

阅读等级：**局部已核（保留原有核读范围）**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：严格I/O撤销保护中的失效操作会破坏仍有效的中间页表缓存，提高后续miss成本。

机制／已存认识：利用 IOMMU 的 IO 页表 cache，连续 IOVA 分配、失效时保留 PTcache（仅页表回收时失效）、批量失效，保持 strict 安全。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：现代 IOMMU 有 PTcache-L1/L2/L3；IOTLB miss 成本取决于 PTcache miss。；strict 安全要求每个 IOVA 在 DMA 后立即 unmap 并失效 IOTLB；deferred 会留下 stale IOTLB 窗口。；Linux 非虚拟化下 IOVA 地址空间 per-device；Rx/Tx 共享同一 IOVA 空间和 PTcache。；Mellanox 多页 descriptor；networking 页表页回收极罕见；IOMMU invalidation queue 支持只失效 IOTLB 保留 PTcache。

限制：Linux v6.0.3；Mellanox 多页 descriptor；单页 descriptor 批量失效未评测。；依赖页表页回收罕见；回收时必须失效 PTcache。；不直接减少 IOTLB miss 数；某些场景与 IOMMU-off 有小差距。；尾延迟非完全等同：多数分位 1.17x，P99.99 约 1.42x。；无跨设备共享翻译、SVA、迁移、远端 PTW；DAMN 对比非独立复现。

与研究关系：说明严格保护与PWC保留可以同时研究，不能把F&S误写成lazy撤销或把严格保护自动排除。

相邻资料：io主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.qizhecai.com/papers/fands.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/fastsafe2024.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.pdf) · SHA-256 `42d11d59ba23b65f719578311c6cf9be7bbec9cf201e377fef124cd5449eab02`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:5391](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:39](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:23](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:64](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:164](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:251](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt:332](../../iommu-platform/out/shared-translation-literature-20261006/papers/fastsafe2024.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/out/innovation-council18-20261005/round2/C2.md:24](../../iommu-platform/out/innovation-council18-20261005/round2/C2.md)：

| S1 | [F&S，SOSP 2024](https://www.qizhecai.com/papers/fands.pdf)：R1 已读摘要、§1、§3、§4 关键段；R2 回读 §2.2、§3，尤其 283–319、466–559。全文在 `/home/xvhaocheng/work/iommu-platform/out/innovation-council18-20261005/sources/C2-fands2024.txt`。没有把全文取得视为逐图验收。 |

[iommu-platform/out/innovation-council18-20261005/round1/C2.md:19](../../iommu-platform/out/innovation-council18-20261005/round1/C2.md)：

| [F&S，SOSP 2024](https://www.qizhecai.com/papers/fands.pdf) | 摘要、§1、§3、§4.1–4.4关键段；全文已保存 | 连续 IOVA、保留不必失效的 PTcache、descriptor 内批量 unmap；研究严格保护下每次 miss 的成本。必须排除“只需改 Linux 分配就解决”的情形。 |

[iommu-platform/out/innovation-council18-20261005/round1-accepted/C2.md:19](../../iommu-platform/out/innovation-council18-20261005/round1-accepted/C2.md)：

| [F&S，SOSP 2024](https://www.qizhecai.com/papers/fands.pdf) | 摘要、§1、§3、§4.1–4.4关键段；全文已保存 | 连续 IOVA、保留不必失效的 PTcache、descriptor 内批量 unmap；研究严格保护下每次 miss 的成本。必须排除“只需改 Linux 分配就解决”的情形。 |

[iommu-platform/out/paper-council-20261005/round1/C2-literature.md:7](../../iommu-platform/out/paper-council-20261005/round1/C2-literature.md)：

- 作者原文：https://www.qizhecai.com/papers/fands.pdf

[返回总目录](../README.md)
