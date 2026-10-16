# Victima: Drastically Increasing Address Translation Reach by Leveraging Underutilized Cache Resources

稳定ID：`victima2023` · 类型：`research_paper` · 主题：`foundations`

作者：Konstantinos Kanellopoulos; Hong Chul Nam; F. Nisa Bostanci; Rahul Bera; Mohammad Sadrosadati; Rakesh Kumar; Davide Basilio Bartolini; Onur Mutlu；年份：2023；出处：MICRO 2023。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：把TLB条目存入L2 cache block(TLB block, 8 entries/64B)；PTW cost predictor决定是否存储；TLB-aware SRRIP优先保留TLB block；支持nested paging。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：L2 cache大量block零复用（平均92%）；PTW-CP(comparator, F1≈80.66%)预测足够准确；ASID/VMID位足以防别名；硬件支持TLB block tag匹配与失效（context switch/shootdown）

限制：仿真，无实机；PTW-CP为简化comparator；不涉及设备/IOMMU；tail latency与队列深度未系统展开；安全仅短节讨论

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2310.04158)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/victima2023.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/victima2023.pdf) · SHA-256 `acf092972da575fdba07aa71a0bef87d957d6580596220a194af3ffa383ce999`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:20911](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:342](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:52](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:70](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:28](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:50](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:68](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)：

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

[iommu-platform/docs/research-landscape-20261005.md:52](../../iommu-platform/docs/research-landscape-20261005.md)：

增加 TLB、IOTLB 或 walker 并不自动消除瓶颈。[Victima](https://arxiv.org/pdf/2310.04158) 说明大 TLB 的延迟/面积/功耗代价；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf) 说明 wafer-scale GPU 中，大量 GPM 的非本地翻译汇聚中央 IOMMU，中央 walker 数量和 mesh 多跳造成排队，因此提出分层缓存、重定向和预取。HDPAT 明确把 GPU-to-GPU 迁移排除在范围外，因此没有评估 TLB shootdown 影响；其结果是 MGPUSim 仿真，不能泛化成每个 IOMMU 的所有瓶颈。[SPARTA](https://arxiv.org/pdf/2001.07045) 则把部分翻译推向 memory-side partition，以面积/功耗和物理放置约束换覆盖；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 也需要连续物理内存、统一权限和 OS 修改。强解法已有，但扩展瓶颈会转移到互连、peer cache、一致性、功耗和失效成本。

[iommu-platform/docs/research-landscape-20261005.md:70](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Victima](https://arxiv.org/pdf/2310.04158) | MICRO 2023；ETH 新闻页已核对 | 复用 L2 cache block 为 TLB entry cluster，cache-backed translation reach | Sniper+McPAT 模拟；[ETH 奖讯](https://ee.ethz.ch/news-and-events/d-itet-news-channel/2023/11/micro-distinguished-artifact-award-for-victima.html) |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:17](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "| 6. 驻留迁移与观测 | UVM 驱逐/迁移、迁移后翻译失效 | IDYLL 定向/惰性失效；OASIS 对象粒度；ObservUVM 采样观测；HMM/SVA 接口 | IDYLL、OASIS、ObservUVM、CUDA stream allocator、Linux HMM/SVA、RISC-V IOMMU 队列 | 运行时看不到 HBM 访问；采样有代价；失效/迁移/数据一致性是不同层；多 GPU 结论仍需条件 |\n\n### 2.1 演变脉络\n\n第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。\n\n第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。\n\n第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 等则把动态分配、OS 协同、共享权限与远程间接访存放在一起讨论。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。"

[返回总目录](../README.md)
