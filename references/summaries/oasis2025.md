# OASIS: Object-Aware Page Management for Multi-GPU Systems

稳定ID：`oasis2025` · 类型：`research_paper` · 主题：`multigpu`

作者：Yueqi Wang; Bingyao Li; Mohamed Tarek Ibn Ziad; Lieven Eeckhout; Jun Yang; Aamer Jaleel; Xulong Tang；年份：2025；出处：2025 IEEE International Symposium on High-Performance Computer Architecture (HPCA)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：在多GPU UVM中按object粒度追踪访问模式，动态选择on-touch migration、access counter-based migration或page duplication；Object Tracker编码Obj ID，O-Table记录策略，OP-Controller跨phase自校正。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：多GPU系统使用UVM/cudaMallocManaged，host CPU维护centralized page table。；object访问模式在同一phase内基本一致，可用单个page推断整个object策略。；指针高位unused bits可编码Obj ID，依赖TBI/LAM/UAI。；O-Table可小容量；OASIS-InMem可用shadow map替代。；默认on-touch，私有/共享可由physical address range和page fault推断。

限制：仅MGPUSim模拟，无真实系统实现；软件实现和Linux/UVM驱动优化留给future work。；Obj ID位数受限，O-Table默认最多24 entries；OASIS-InMem有shadow map内存/延迟开销。；shared-rw-mix/shared-write-only object无法到ideal。；2MB大页会把private object变shared，降低收益。；oversubscription下shared page evict后可能误判为private。；不优化地址翻译/PTW，不覆盖远端PTW。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：[barre2024](barre2024.md)：mechanism_comparison

原始来源：

- [来源1](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/oasis2025.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.pdf) · SHA-256 `4d6deb67f160eb07648af6fc563f58cf9d54cfb13700fd32143ff3b696933c6c`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:12858](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:14](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:53](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:306](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:391](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:488](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:497](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt:631](../../iommu-platform/out/shared-translation-literature-20261006/papers/oasis2025.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)：

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

[iommu-platform/docs/research-landscape-20261005.md:37](../../iommu-platform/docs/research-landscape-20261005.md)：

地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 L4（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。

[iommu-platform/docs/research-landscape-20261005.md:125](../../iommu-platform/docs/research-landscape-20261005.md)：

| [OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) | HPCA 2025 | object-aware page management，动态选择迁移策略 | MGPUSim；对象非均匀/2MB 页限制；“near-optimal”相对 hypothetical Ideal |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:17](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "| 6. 驻留迁移与观测 | UVM 驱逐/迁移、迁移后翻译失效 | IDYLL 定向/惰性失效；OASIS 对象粒度；ObservUVM 采样观测；HMM/SVA 接口 | IDYLL、OASIS、ObservUVM、CUDA stream allocator、Linux HMM/SVA、RISC-V IOMMU 队列 | 运行时看不到 HBM 访问；采样有代价；失效/迁移/数据一致性是不同层；多 GPU 结论仍需条件 |\n\n### 2.1 演变脉络\n\n第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。\n\n第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。\n\n第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 等则把动态分配、OS 协同、共享权限与远程间接访存放在一起讨论。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。"

[返回总目录](../README.md)
