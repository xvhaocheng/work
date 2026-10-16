# Revelator: Rapid Data Fetching via System-Software-Guided Hash-based Speculative Address Translation

稳定ID：`revelator2026v2` · 类型：`research_paper` · 主题：`foundations`

作者：Konstantinos Kanellopoulos; Konstantinos Sgouras; Harsh Songara; Andreas Kosmas Kakolyris; Vlad-Petru Nitu; Spiros Galanopoulos; Rahul Bera; Konstantina Koliogeorgi; Rakesh Kumar; Onur Mutlu；年份：2026；出处：ISCA 2026。

阅读等级：**既有worker全文提取；非root逐篇精读**。本轮是资料归档，不新增人读标记或复现实验结论。

问题：unknown（既有JSON卡没有独立问题字段；已存机制、假设和原文证据保留如下，不能由题名补写）。

机制／已存认识：OS层tiered hash-based allocation（数据页与末级PT帧）使VA→PA/VA→PTE可预测；L2 TLB miss时硬件speculation engine用同一CityHash重算候选PPN/PTE并提前取数；speculation degree filter按利用率与带宽控制推测度。

支持边界：原记录的全文证据提取；root只抽核部分关键原文，未逐篇重审或复现。

条件：hash-based allocation成功概率仅取决于内存利用率1-(1-p)^N；哈希输入含VPN、PID、tier seed，OS与硬件可重算；OS维护per-hash成功率计数器；使用secret per-process hash key；non-faulting推测取数；误推测可通过MSHR/日志清理

限制：arXiv预印本，未确认接收（感谢信提及MICRO 2025/ASPLOS 2026/ISCA 2026）；未涉及设备/IOMMU/SVA；多核/NUMA/虚拟化均仿真；cleanup、NUMA hint、扩展流程放在extended version；安全侧信道仅定性缓解；本地保存arXiv v2作者稿；正式出版元数据由IEEE核定。

与研究关系：本条用于对应主题的前作/语义边界；是否当前采用须回到总计划。

相邻资料：foundations主题近邻；历史收录不代表当前主线采纳。

原始来源：

- [来源1](https://arxiv.org/pdf/2508.02007v2)
- [来源2](https://arxiv.org/pdf/2508.02007v1)
- [来源3](https://doi.org/10.1109/ISCA66397.2026.00147)

本地原件（相对symlink，未复制论文）：

- [PDF](../papers/revelator2026v2.pdf) · canonical：[原路径](../../iommu-platform/out/shared-translation-literature-20261006/papers/revelator2026v2.pdf) · SHA-256 `ee7cd925ff1a319679126ee4e09559b2b887d2050f26e0030090f0b110b1aa27`

项目核查／引用入口（精确行保存在catalog；链接打开原文件）：

- [iommu-platform/out/shared-translation-literature-20261006/catalog.json:15091](../../iommu-platform/out/shared-translation-literature-20261006/catalog.json)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:326](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/out/research-landscape-20261005/source-manifest.json:500](../../iommu-platform/out/research-landscape-20261005/source-manifest.json)
- [iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/docs/research-landscape-20261005.md:72](../../iommu-platform/docs/research-landscape-20261005.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:28](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md:70](../../iommu-platform/out/innovation-council18-20261005/inputs/research-landscape-before-council.md)
- [iommu-platform/out/innovation-council18-20261005/round1/C1.md:16](../../iommu-platform/out/innovation-council18-20261005/round1/C1.md)

既有项目记录的上下文（历史判断，不能覆盖最新合同）：

[iommu-platform/docs/research-landscape-20261005.md:30](../../iommu-platform/docs/research-landscape-20261005.md)：

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

[iommu-platform/docs/research-landscape-20261005.md:72](../../iommu-platform/docs/research-landscape-20261005.md)：

| [Revelator v2](https://arxiv.org/pdf/2508.02007v2) | ISCA 2026；arXiv v2 2026-06-25 | OS tiered hash allocation + HW speculation engine，预测数据与最后一级 PTE | Linux 6.10.8 原型 + Virtuoso/Sniper + RTL 综合；OS 协同仍是条件 |

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:17](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"new": "| 6. 驻留迁移与观测 | UVM 驱逐/迁移、迁移后翻译失效 | IDYLL 定向/惰性失效；OASIS 对象粒度；ObservUVM 采样观测；HMM/SVA 接口 | IDYLL、OASIS、ObservUVM、CUDA stream allocator、Linux HMM/SVA、RISC-V IOMMU 队列 | 运行时看不到 HBM 访问；采样有代价；失效/迁移/数据一致性是不同层；多 GPU 结论仍需条件 |\n\n### 2.1 演变脉络\n\n第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。\n\n第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。\n\n第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 等则把动态分配、OS 协同、共享权限与远程间接访存放在一起讨论。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。"

[iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md:72](../../iommu-platform/out/research-landscape-20261005/workers/L6_map_edits.md)：

"old": "### 7.1 已具备较强全文/关键节证据的前沿\n\n- [HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)：HPCA 2026，wafer-scale GPU 分布式翻译；重要限制是排除 GPU-to-GPU migration，未评估 TLB shootdown，结果为仿真。\n- [Revelator v2](https://arxiv.org/pdf/2508.02007v2)：ISCA 2026，DOI 10.1109/ISCA66397.2026.00147；v2 标题为 System-Software-Guided；Linux 6.10.8 原型 + Sniper/Virtuoso + RTL 综合；OS 分配协同仍是前提，不照搬 v1 旧数字。\n- [ShareMMU](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)：MICRO 2026 camera-ready；Permission Tracker、定向 back-invalidation、权限下降局部化；仍等待 dirty write-back/flush acknowledgment，未消灭全部动态更新/排空成本；只作为前沿条件，不据此转安全主线。\n- [DX100](https://arxiv.org/pdf/2505.23073v1)：ISCA 2025，批量 indirection 重排；明确不覆盖 pointer chasing。\n- [MINISA](https://arxiv.org/pdf/2603.20623v1)：ISPASS 2026；本地 arXiv v1；控制面 ISA，与数据搬运正交。\n- [ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf)：ISCA 2026；真机 RTX 3090，采样观测。\n- [OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf)：HPCA 2025，对象感知多 GPU page management。\n- [Tiara](https://arxiv.org/pdf/2606.13708v1)：arXiv 2026 预印本，不是 NSDI 2022 同名负载均衡器；应用间接表≠硬件 OS 页表；Table 1 中 PagedAttention 最优批处理为 2 RTT，不能只拿朴素 160 RTT 讲必要性。",

[返回总目录](../README.md)
