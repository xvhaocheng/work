# Utilizing the IOMMU Scalably

稳定ID：`iommu_scalable2015` · 类型：`research_paper` · 主题：`io`

作者：Omer Peleg; Adam Morrison; Benjamin Serebrin; Dan Tsafrir；年份：2015；出处：USENIX ATC '15 (USENIX Annual Technical Conference)。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：识别 IOVA 分配与 IOTLB 失效锁瓶颈；提出 dynamic identity mapping、IOVA-kmalloc、magazine 每核缓存和每核延迟失效队列。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：Intel x86 IOMMU 4 级 radix 页表；per-device 页表；IOVA 48-bit，PA 46-bit。；高并发动态 DMA 映射；每个设备有 IOVA allocator 锁；IOTLB 失效队列有锁。；同一 IOVA range 不并发 map/unmap；IOVA range 不可部分 unmap。；dynamic identity mapping 假设 buffer 物理连续；IOVA-kmalloc 可用 GFP_DMA 限制低内存区避免冲突。

限制：仅 Linux/Intel x86、10Gb/s、单 NIC；未覆盖 100/200Gbps、多设备共享翻译、跨设备 SVA。；strict IOTLB invalidation 在其设计下 16 核约 1/6 无 IOMMU TPS，不 scalable。；IOVA-kmalloc 页表内存可无界增长；page table memory 管理是 future work。；dynamic identity mapping 不比 IOVA 分配方案更快，且更复杂。；未覆盖迁移、远端 PTW、多租户、失效尾延迟分布。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：io主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/iommu_scalable2015.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.pdf) · SHA-256 `2fe319d9f089d6db3d02bf49172b86f0656798ada5c36ff7bb597c1c56d10df1`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:8916](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt:1](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt:3](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt:1456](../../iommu-platform/out/shared-translation-literature-20261006/papers/iotlb_wall2023.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt:18](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt:100](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt:293](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt)
- [iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt:305](../../iommu-platform/out/shared-translation-literature-20261006/papers/iommu_scalable2015.txt)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:28](../../iommu-platform/docs/research-landscape-20261005.md)：

第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。

[iommu-platform/docs/research-landscape-20261005.md:91](../../iommu-platform/docs/research-landscape-20261005.md)：

| [IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf) | ATC 2015 | scalable IOVA 分配、identity/kmalloc/magazines、per-core deferred invalidation | 真实 Linux/10GbE；该 2015 实现的严格失效受伸缩性限制，延迟失效则存在保护窗口 |

[iommu-platform/docs/independent-research-directions-20261015.md:77](../../iommu-platform/docs/independent-research-directions-20261015.md)：

- [Utilizing the IOMMU Scalably，ATC2015](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)§3.2/§5已有per-core flush队列、锁外等待及同步前保留资源。新问题须增加对live任务和回收的共同可预测结果。

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:17](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "| 6. 驻留迁移与观测 | UVM 驱逐/迁移、迁移后翻译失效 | IDYLL 定向/惰性失效；OASIS 对象粒度；ObservUVM 采样观测；HMM/SVA 接口 | IDYLL、OASIS、ObservUVM、CUDA stream allocator、Linux HMM/SVA、RISC-V IOMMU 队列 | 运行时看不到 HBM 访问；采样有代价；失效/迁移/数据一致性是不同层；多 GPU 结论仍需条件 |\n\n### 2.1 演变脉络\n\n第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。\n\n第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。\n\n第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 等则把动态分配、OS 协同、共享权限与远程间接访存放在一起讨论。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。"

[返回总目录](../README.md)
