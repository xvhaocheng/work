# SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators

稳定ID：`sparta2020` · 类型：`preprint` · 主题：`accelerators`

作者：Javier Picorel; Seyed Alireza Sanaee Kohroudi; Zi Yan; Abhishek Bhattacharjee; Babak Falsafi; Djordje Jevdjic；年份：2020；出处：unknown。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：将翻译拆到加速器侧tiny TLB和内存侧共享per-partition TLB；虚拟地址决定内存分区，数据与PTE同分区，重叠翻译与数据读取。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：虚拟地址唯一标识内存分区；物理页可在分区内任意放置；数据和PTE可共置于同一分区；Linux NUMA/membind可复用于分区；共享页表与细粒度保护可保留；性能用CPI模型估计

限制：预印本，无正式发表证据；无内存侧TLB硬件原型；全系统TLB miss/page fault仿真不现实，使用CPI模型；Linux 32-NUMA节点原型需额外1.5-2GB内存；未详述每设备权限匹配、A/D位、实时迁移和TLBI排空

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：accelerators主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2001.07045)
- [来源2](https://arxiv.org/html/2001.07045v1)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/sparta2020.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.pdf) · SHA-256 `26f228d4afff7f9dcf2883b68e01ee6096f30b3e6b1db0524dba76b123656c0e`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:17539](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt:20](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt:220](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt:590](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt:656](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt:966](../../iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:182](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)：

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

[iommu-platform/docs/research-landscape-20261005.md:52](../../iommu-platform/docs/research-landscape-20261005.md)：

增加 TLB、IOTLB 或 walker 并不自动消除瓶颈。[Victima](https://arxiv.org/pdf/2310.04158) 说明大 TLB 的延迟/面积/功耗代价；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf) 说明 wafer-scale GPU 中，大量 GPM 的非本地翻译汇聚中央 IOMMU，中央 walker 数量和 mesh 多跳造成排队，因此提出分层缓存、重定向和预取。HDPAT 明确把 GPU-to-GPU 迁移排除在范围外，因此没有评估 TLB shootdown 影响；其结果是 MGPUSim 仿真，不能泛化成每个 IOMMU 的所有瓶颈。[SPARTA](https://arxiv.org/pdf/2001.07045) 则把部分翻译推向 memory-side partition，以面积/功耗和物理放置约束换覆盖；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 也需要连续物理内存、统一权限和 OS 修改。强解法已有，但扩展瓶颈会转移到互连、peer cache、一致性、功耗和失效成本。

[iommu-platform/docs/research-landscape-20261005.md:69](../../iommu-platform/docs/research-landscape-20261005.md)：

| [SPARTA](https://arxiv.org/pdf/2001.07045) | arXiv 2020；正式会刊未确认 | accelerator-side tiny TLB + memory-side per-partition TLB，数据与 PTE 同 partition | Linux 原型 + CPI model；不猜会议 |

[iommu-platform/docs/performance-council-decision-20261005.md:100](../../iommu-platform/docs/performance-council-decision-20261005.md)：

- [SPARTA](https://arxiv.org/html/2001.07045v1)，设计部分：内存侧翻译和依靠虚拟地址确定内存分区已有研究；挪动翻译位置本身不是创新。

[返回总目录](../README.md)
