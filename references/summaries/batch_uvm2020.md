# Batch-Aware Unified Memory Management in GPUs for Irregular Workloads

稳定ID：`batch_uvm2020` · 类型：`research_paper` · 主题：`lifecycle`

作者：Hyojong Kim; Jaewoong Sim; Prasun Gera; Ramyad Hadidi; Hyesoon Kim；年份：2020；出处：ASPLOS '20。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：Thread Oversubscription 增加 GPU page fault batch size；Unobtrusive Eviction 预取 eviction 并与 CPU-to-GPU migration 重叠。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：GPU runtime 批量处理 page fault；64KB page；20us GPU runtime fault handling；LRU 替换；NVIDIA PASCAL 驱动模型

限制：UE 未在真实 runtime 实现，仅模拟；TO 可能增加 premature eviction；主要面向 irregular graph workloads；不含多设备共享翻译

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：lifecycle主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://hparch.gatech.edu/papers/kim-asplos20.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/batch_uvm2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.pdf) · SHA-256 `dbf7455d45cd9b10781c964785c0e31e4b31031c2ae8afbb10cc44018d375f83`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:612](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt:16](../../iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt:267](../../iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt:535](../../iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt:695](../../iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:404](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:37](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:115](../../iommu-platform/docs/research-landscape-20261005.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:37](../../iommu-platform/docs/research-landscape-20261005.md)：

地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 L4（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。

[iommu-platform/docs/research-landscape-20261005.md:115](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) | ASPLOS 2020 | Thread oversubscription + unobtrusive eviction | 真机 Titan Xp 剖析 + MacSim；UE 未完整真机实现 |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:25](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 L4（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。"

[iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:35](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)：

地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 L4（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。

[返回总目录](../README.md)
