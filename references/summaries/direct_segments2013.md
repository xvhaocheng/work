# Efficient Virtual Memory for Big Memory Servers

稳定ID：`direct_segments2013` · 类型：`research_paper` · 主题：`foundations`

作者：Arkaprava Basu; Jayneel Gandhi; Jichuan Chang; Mark D. Hill; Michael M. Swift；年份：2013；出处：ISCA'13 (Tel-Aviv, Israel)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：per-core BASE/LIMIT/OFFSET寄存器的direct segment：BASE≤V<LIMIT时物理地址=V+OFFSET，无TLB miss；其余地址走常规分页；OS提供primary region抽象并可与数据页迁移联动。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：big-memory工作负载早期分配、运行稳定；段内统一读写权限、物理连续；连续物理内存可预留或compaction获得；以4KB页模拟段硬件，TLB miss减少线性映射为cycle降低

限制：无真实段硬件执行，用4KB页模拟；不适合短时/动态执行环境；VM与kernel段为展望；不涉及设备/IOMMU/SVA

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/direct_segments2013.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/direct_segments2013.pdf) · SHA-256 `01443e0fd2ac227815599f8861ae895787bdc0d03e98eb5e550cfe66777e4ae3`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:4442](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:32](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/flow-iommu-candidate-review-20261014.md:44](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)
- [iommu-platform/docs/research-landscape-20261005.md:26](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:52](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:68](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md:24](../../iommu-platform/out/flow-iommu-candidates-20261014/lease/analysis.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:24](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:52](../../iommu-platform/docs/research-landscape-20261005.md)：

增加 TLB、IOTLB 或 walker 并不自动消除瓶颈。[Victima](https://arxiv.org/pdf/2310.04158) 说明大 TLB 的延迟/面积/功耗代价；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf) 说明 wafer-scale GPU 中，大量 GPM 的非本地翻译汇聚中央 IOMMU，中央 walker 数量和 mesh 多跳造成排队，因此提出分层缓存、重定向和预取。HDPAT 明确把 GPU-to-GPU 迁移排除在范围外，因此没有评估 TLB shootdown 影响；其结果是 MGPUSim 仿真，不能泛化成每个 IOMMU 的所有瓶颈。[SPARTA](https://arxiv.org/pdf/2001.07045) 则把部分翻译推向 memory-side partition，以面积/功耗和物理放置约束换覆盖；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 也需要连续物理内存、统一权限和 OS 修改。强解法已有，但扩展瓶颈会转移到互连、peer cache、一致性、功耗和失效成本。

[iommu-platform/docs/research-landscape-20261005.md:26](../../iommu-platform/docs/research-landscape-20261005.md)：

第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。

[iommu-platform/docs/flow-iommu-candidate-review-20261014.md:44](../../iommu-platform/docs/flow-iommu-candidate-review-20261014.md)：

C的连续映射与生命周期须分开比较。Direct Segments允许按基础页边界的连续映射；CoLT保留基础页、合并连续映射并明确属性/失效条件。简单generation或租约到期不能排空已经接受的DMA；回收仍需要禁止旧访问、失效、确认和互连排序。[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf)、[CoLT](https://users.ece.cmu.edu/~omutlu/pub/MICRO2012-proceedings.pdf)、[官方IOFENCE合同](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html)。

[iommu-platform/docs/research-landscape-20261005.md:68](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) | ISCA 2013 | direct segment/primary region 绕过 TLB | 真实服务器但用 4KB 页模拟 direct segment；强依赖连续物理内存 |

[返回总目录](../README.md)
