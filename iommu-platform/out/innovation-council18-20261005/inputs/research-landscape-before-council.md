# 共享 IOMMU、地址翻译与异构数据搬运：2026-10-05 研究地图

> 本文是截至 2026-10-05 的研究地图。覆盖 36 篇独立论文的 38 份全文版本与 4 份官方文档；由 DeepSeek 按研究线提取证据并起草，Codex 核验关键原文、正式版本与边界后审定。主地图（本文）与原始证据卡分开；关键来源边界见 [ROOT_REVIEW.md](../out/research-landscape-20261005/ROOT_REVIEW.md)，来源清单位于 [source-manifest.json](../out/research-landscape-20261005/source-manifest.json)，完整证据卡见 [L1](../out/research-landscape-20261005/workers/L1_translation.md)、[L2](../out/research-landscape-20261005/workers/L2_sharing.md)、[L3](../out/research-landscape-20261005/workers/L3_data_lifecycle.md)、[L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md)。阅读时可先看 §1–§3 的主干结论，再用 §5 索引和证据卡下钻。本文并非全领域系统综述，也未复现论文。

## 1. 直接结论

经典工作已经提供了缓存、大页、分区、调度、批处理和访存解耦等有效方法。前沿研究正在应对新的条件：更多设备共享资源、更大的内存与物理距离、更频繁的分配和迁移，以及软件难以观察的设备行为。

本次跨论文综合判断是：最值得追问的困难在于这些条件相遇时的代价。例如，把数据搬近计算可能增加映射失效；共享翻译可以减少重复工作，却需要管理不同设备的权限和进度；精细决策能减少浪费，但需要更多信息和元数据。已有方法各自解决了一部分问题，具体创新仍须落到有应用依据、能与强对手比较的命题上。

## 2. 六条研究线总表

| 研究线 | 经典问题 | 现在前沿 | 已有较强机制 | 受限条件与边界 |
|---|---|---|---|---|
| 1. 翻译覆盖/延迟 | radix 页表 TLB miss 后多级 walk、TLB reach 不足 | wafer-scale 集中 IOMMU 排队；OS/HW 协同 hash 推测翻译；大 TLB 与 cache-backed TLB | translation cache、walk cache、large page、direct segment、SPARTA、Victima、Revelator v2、HDPAT | TLB miss 不等于完整未缓存 walk；命中率、PTW 时延、应用吞吐、能耗不是同一指标；CPU/GPU/IOMMU/ATS 不能互推 |
| 2. 共享干扰与服务顺序 | LLC/TLB/PTW 共享时按需求竞争，不按收益分配 | 多租户 GPU walker stealing；共享 IOTLB/PTW 调度；混合关键性 IOMMU 干扰 | UCP、KPart、MASK、walk scheduling、walk stealing、IOTLB wall | 静态分区可能牺牲利用率，动态共享可能牺牲可预测性；本批工作未建立跨平台的统一保证 |
| 3. I/O 软件管理/映射生命周期 | map/unmap、IOVA 分配、IOTLB 失效高开销 | 生命周期接口与跨设备权限/失效语义扩展（RISC-V IOTINVAL/IODIR/IOFENCE、stream-ordered allocator 等）；rIOMMU/per-core deferred invalidation 是 2015 机制 | rIOMMU、Utilizing the IOMMU Scalably、IOTLB wall、BypassD、CUDA stream allocator | 合法语义不等于高性能；严格/延迟/权限跟踪代价不同；非 ATS 设备不是只能存物理地址 |
| 4. 间接访存/布局/搬运接口 | pointer chasing、维度不匹配、多 GPU 数据搬运与同步 | DX100 批量散读重排；Tiara 远程内存 ISA；NDS 多维布局；WIC warp 中断 | DAE、IMPICA、DX100、NDS、GPS、Tiara、WIC | 真串行地址依赖不同于可并行 scatter/gather；PIM/NIC 解链只是已见方案之一，不是唯一道路 |
| 5. GPU 动态分配 | UVM demand paging、碎片、KV cache 动态增长 | PagedAttention 与 vAttention 路线对撞；vTensor 解耦；GMLake 缝合 | PagedAttention、Batch-aware UVM、vAttention、vTensor、GMLake | 动态映射既非总是瓶颈，也非永远可隐藏；取决于可预测性、重叠、缓存池、粒度、驱动和阶段 |
| 6. 驻留迁移与观测 | UVM 驱逐/迁移、迁移后翻译失效 | IDYLL 定向/惰性失效；OASIS 对象粒度；ObservUVM 采样观测；HMM/SVA 接口 | IDYLL、OASIS、ObservUVM、CUDA stream allocator、Linux HMM/SVA、RISC-V IOMMU 队列 | 所研究 UVM 驱动缺少显存驻留访问的可见性；采样有代价；失效/迁移/数据一致性是不同层；多 GPU 结论仍需条件 |

### 2.1 演变脉络

第一阶段是经典缓存、覆盖与访存解耦：[DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) 把地址计算/访存与执行解耦，用队列隐藏延迟；[Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) 说明缓存部分翻译可跳过层级，TLB miss 不应默认等于多次未缓存 DRAM 访问；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 用连续物理区域绕过 TLB。已解决的是：在给定页表、缓存和直接段条件下，翻译覆盖与延迟有明确优化空间。新条件使原假设吃紧：GPU/IOMMU/ATS 把翻译推到多设备、多级共享结构，缓存覆盖率不再单独决定结果。

第二阶段是多加速器共享与 I/O 软件控制面：[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf)、[Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf)、[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 研究 GPU 多应用/多租户下共享 TLB 与 walker 的干扰，说明增加 TLB/walker 不自动消除竞争；[rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf)、[Utilizing the IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf)、[IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) 则把 I/O 路径的 IOVA 分配、失效批处理和 IOTLB miss 暴露为一阶成本。已解决的是：软件 IOVA 管理、ring 语义、批量/延迟失效、hugepage mapping 都有可测收益。新条件使原假设吃紧：10–40 Gbps 到 100–200 Gbps、单租户到多租户、性能目标到时间可预测性与不可信设备安全共享。

第三阶段是 2023–26 的动态对象、迁移、可观测、物理扩展与 OS 协同：[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 说明迁移后翻译失效会与 demand walk 争用；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 把决策粒度推向对象；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 把“运行时看不到 HBM 驻留访问”变成可观测性问题；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)、[Victima](https://arxiv.org/pdf/2310.04158)、[SPARTA](https://arxiv.org/pdf/2001.07045) 说明集中 IOMMU、大 TLB 与 memory-side translation 会把瓶颈转移到互连、面积/功耗和一致性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf)、[Revelator v2](https://arxiv.org/pdf/2508.02007v2) 分别通过系统虚拟内存支持 LLM 动态分配、通过 OS 分配协同帮助硬件预测地址；共享权限和远程间接访存则是 ShareMMU、Tiara 处理的不同问题。已解决的是：单向优化（更大缓存、更大页、更快失效、更多观测）都有明确适用条件。新条件使原假设吃紧：对象、页、区域、page-group 粒度并存，策略需要跨 OS/驱动/硬件接口协同，合法语义不自动等于低代价。这条脉络只描述本批材料的证据分布，不对所有领域作统一历史分期。

## 3. 六个仍在反复出现的瓶颈

以下六项是本次跨论文综合判断，不是任一论文的单独结论；每项至少给两个直接原始来源，并只在需要时保留证据卡导航。

### 卡点 1：粒度不一致
地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 在 PTE 粒度过滤和批量失效；[OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；[vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；[Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) 则在 batch 粒度权衡 fault 并发与过早驱逐。原证据卡见 [L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md)（IDYLL、OASIS、ObservUVM、vAttention、Batch-aware UVM），L2 只提供共享线背景。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换、观测和失效成本纳入设计。剩余约束：对象可能非均匀，2 MiB 页会改变私有/共享行为，采样依赖局部性，粒度切换本身有代价。

### 卡点 2：共享利用率与可预测性
共享资源上，静态容量分配能提供部分隔离但可能低利用；动态共享能提升吞吐但可能破坏可预测性。[MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) 显示多地址空间共享 GPU L2 TLB 和 page walker 时，translation 会成为一阶瓶颈，token/bypass/DRAM 调度有收益；[Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) 显示静态划分 walker 会 under-utilization，动态偷取提升多租户吞吐，而 DWS++ 为了改善公平，会牺牲部分总吞吐——这不是无条件定律；[KPart](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf) 在真实 Intel LLC 上显示 per-app way partition 粒度粗、associativity 低，聚类后才改善；其背景先行点也要保留：给后台更多缓存可降低后台内存带宽需求、改善 moses 前台尾延迟，因此现有 P/N 组合现象不自动新。原证据卡见 [L2](../out/research-landscape-20261005/workers/L2_sharing.md)。剩余约束：本次证据未确立跨平台通用隔离结论，不能写成“隔离与利用率不能同时最优”的形式化不可能定理。

### 卡点 3：动态放置与更新代价
迁移和动态映射的真正代价常落在翻译更新：page migration 改变映射后，旧 TLB/PTE 必须失效。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 的真实双 A100 剖析报告平均约 42% 执行时间耗在 page table invalidation；其零失效上界和提出方案收益来自 MGPUSim 仿真，因此应写成“真机问题剖析 + 模拟方案评估”，不能说整篇全实物，也不能说整篇无实物。[RISC-V IOMMU 队列文档](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) 已有 IOTINVAL、IODIR、IOFENCE、ATS.INVAL 等语义，但 PR/PW 只覆盖已被 IOMMU 处理的请求，尚未抵达的设备在途请求可能另需互连 fence；标准支持按上下文/地址失效，也允许简单实现扩大为全局失效，不能把实验 quiet 条件当作通用规范要求。强解法包括定向失效、批量/惰性 PTE 更新、目录缓存失效和按上下文/地址范围失效。剩余约束：合法语义不保证低代价，失效可能仍与 demand walk 争用资源。证据卡见 [L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md)。

### 卡点 4：缺信息，且测信息有成本
运行时经常缺关键信息。[ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) 指出 UVM driver 看不到 GPU 对 HBM-resident page 的访问，只能看 DRAM-resident page fault；NVIDIA access counters 原本只跟踪 GPU→DRAM over PCIe，不直接跟踪 HBM。它主动把选定 2 MiB 区域的一部分 64 KiB 采样页迁到主存，再借 PCIe 访问计数推断活跃性，不是计数器直接全量监控显存；依赖空间局部性，采样有代价，当前实测单 GPU，原文用 HBM 泛称 GPU 显存，不能据此说 RTX 3090 采用 HBM 芯片。[IDYLL](https://par.nsf.gov/servlets/purl/10492257) 也显示 driver 不知道哪些 GPU 持有有效翻译副本，基线向所有 GPU 广播失效。强解法已有：采样观测、对象跟踪、权限跟踪、per-context 失效；但“看到什么、何时看到、以什么代价看到”仍是独立问题。原证据卡见 [L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md)。

### 卡点 5：访存位置、依赖、布局与可编程性
间接访存必须分开两类：地址依赖远端返回数据的真串行链，和地址已可批量发出的 scatter/gather。[IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) 系统化 pointer chasing 的串行依赖；[DX100](https://arxiv.org/pdf/2505.23073v1) 明确不支持 pointer chasing，只做 array-based indirection；[Tiara](https://arxiv.org/pdf/2606.13708v1) Table 1 中 PagedAttention 的朴素 stop-and-wait 是 160 RTT，但最优批处理基线是 2 RTT，不能用 160 RTT 讲必要性；其应用间接表遍历不是硬件 OS 页表，已知地址的散读不能算不可并行的指针链。[NDS](https://par.nsf.gov/servlets/purl/10321761) 从维度不匹配处理 marshalling 与带宽利用，[GPS](https://harinimuthukrishnan.net/publication/gps/gps.pdf) 用 publish-subscribe 和弱内存模型合并写，[WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) 用 warp 级中断替代轮询。原证据卡见 [L3](../out/research-landscape-20261005/workers/L3_data_lifecycle.md)。本稿不把“只能 PIM/NIC”写成结论；只能写：对真串行地址依赖，已见 PIM/近 NIC/近内存解链方案，其他预测、helper thread、多链并行仍需逐案验证。

### 卡点 6：静态资源和物理扩展极限
增加 TLB、IOTLB 或 walker 并不自动消除瓶颈。[Victima](https://arxiv.org/pdf/2310.04158) 说明大 TLB 的延迟/面积/功耗代价；[HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf) 说明 wafer-scale GPU 中，大量 GPM 的非本地翻译汇聚中央 IOMMU，中央 walker 数量和 mesh 多跳造成排队，因此提出分层缓存、重定向和预取。HDPAT 明确把 GPU-to-GPU 迁移排除在范围外，因此没有评估 TLB shootdown 影响；其结果是 MGPUSim 仿真，不能泛化成每个 IOMMU 的所有瓶颈。[SPARTA](https://arxiv.org/pdf/2001.07045) 则把部分翻译推向 memory-side partition，以面积/功耗和物理放置约束换覆盖；[Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) 也需要连续物理内存、统一权限和 OS 修改。强解法已有，但扩展瓶颈会转移到互连、peer cache、一致性、功耗和失效成本。

## 4. 评估性质说明

本节只交代证据性质，不重复单篇限制。IDYLL 是真实双 A100 问题剖析 + MGPUSim 方案评估：42% invalidation 开销来自真机剖析，零失效上界和方案收益来自仿真，不能写成全实物或全模拟。ObservUVM 是单 GPU 真机原型（RTX 3090），采样观测依赖空间局部性，多 GPU 仅讨论。其余前沿工作多为原型、仿真或 RTL/分析模型：HDPAT/OASIS/ShareMMU 等以模拟为主，Revelator v2 为 Linux 6.10.8 原型 + Sniper/Virtuoso + RTL 综合，vAttention 为 ASPLOS 2025 正式版真机评估但 decode 仅与最优 PagedAttention 持平。各数字只在同论文、同平台、同基线和同负载内可比较，本稿不做跨论文排名；细节看 §5、§7.2 和各证据卡。

## 5. 代表文献索引（36 篇，按机制族分组；合并重复版本）

> 下列为正式论文短称与主来源链接；完整证据卡在原稿中。原稿中的过度推断已按 ROOT_REVIEW 收窄。每行是“机制族中的代表”，不是阅读数量统计。

### A. 翻译覆盖、缓存与推测翻译

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [DAE](https://people.eecs.berkeley.edu/~kubitron/courses/cs252-F00/handouts/papers/p231-smith.pdf) | ISCA 1982 | 解耦地址计算/访存与执行，用队列隐藏延迟 | 简化标量模型，非现代系统 |
| [Translation Caching](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf) | ISCA 2010 | 系统比较 page-table cache 与 translation cache，可跳过部分翻译层级 | trace-driven 模拟；不能外推 IOMMU/GPU/ATS |
| [Direct Segments](https://research.cs.wisc.edu/multifacet/papers/isca13_direct_segment.pdf) | ISCA 2013 | direct segment/primary region 绕过 TLB | 真实服务器但用 4KB 页模拟 direct segment；强依赖连续物理内存 |
| [SPARTA](https://arxiv.org/pdf/2001.07045) | arXiv 2020；正式会刊未确认 | accelerator-side tiny TLB + memory-side per-partition TLB，数据与 PTE 同 partition | Linux 原型 + CPI model；不猜会议 |
| [Victima](https://arxiv.org/pdf/2310.04158) | MICRO 2023；ETH 新闻页已核对 | 复用 L2 cache block 为 TLB entry cluster，cache-backed translation reach | Sniper+McPAT 模拟；[ETH 奖讯](https://ee.ethz.ch/news-and-events/d-itet-news-channel/2023/11/micro-distinguished-artifact-award-for-victima.html) |
| [HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf) | HPCA 2026 | wafer-scale GPU 分层缓存、重定向、预取，缓解中央 IOMMU 排队 | MGPUSim；排除 GPU-to-GPU migration/shootdown |
| [Revelator v2](https://arxiv.org/pdf/2508.02007v2) | ISCA 2026；arXiv v2 2026-06-25 | OS tiered hash allocation + HW speculation engine，预测数据与最后一级 PTE | Linux 6.10.8 原型 + Virtuoso/Sniper + RTL 综合；OS 协同仍是条件 |
| [Neighborhood-Aware Translation](https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf) | MICRO 2018 | 合并相邻翻译、提高 page walker 利用率 | GPU 模拟 |

### B. 共享结构、干扰与服务顺序

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [UCP](https://www.eecs.northwestern.edu/~rjoseph/eecs453/papers/quereshi-micro2006.pdf) | MICRO 2006 | utility-based cache partitioning，按收益而非需求分区 | 自研模拟器；不能直接套 IOTLB/设备共享 |
| [KPart](https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf) | HPCA 2018 | 真实 way-partitioning 上聚类 + speedup curve + DynaWay | 真实 Broadwell-D LLC；非 IOMMU，非 strict isolation |
| [MASK](https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf) | ASPLOS 2018 | GPU 多应用下 TLB fill、bypass、DRAM 调度 | GPGPU-Sim；非 CPU/SoC IOMMU/ATS |
| [Walk Scheduling](https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf) | ISCA 2018 | SIMT-aware PTW 服务顺序，优先进度关键 walk | gem5；非多租户安全方案 |
| [Walk Stealing](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf) | HPCA 2021 | 多租户 GPU 动态 page walk stealing，DWS++ 调吞吐/公平 | GPGPU-Sim；无真实 GPU、无安全隔离 |
| [Accelerator Translation](https://vast.cs.ucla.edu/sites/default/files/publications/hpca17.pdf) | HPCA 2017 | 每加速器小 TLB + 共享 L2 TLB + host MMU walk | PARADE/gem5；依赖正则加速器访问 |

### C. I/O 软件管理、映射生命周期与失效

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [rIOMMU](https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf) | ASPLOS 2015 | ring flat table，一 ring 一 IOTLB entry，burst 末失效 | 依赖 ring 顺序；部分模拟；非通用 IOMMU |
| [IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf) | ATC 2015 | scalable IOVA 分配、identity/kmalloc/magazines、per-core deferred invalidation | 真实 Linux/10GbE；该 2015 实现的严格失效受伸缩性限制，延迟失效则存在保护窗口 |
| [IOTLB wall](https://dejankostic.com/documents/publications/iotlb-peerj23.pdf) | PeerJ CS 2023 | 200 Gbps 下 IOTLB miss 数据路径瓶颈，hugepage mapping 缓解 | 真实 200Gbps Intel/AMD+Mellanox；hugepage 非 panacea |
| [BypassD](https://cgi.di.uoa.gr/~vkarakos/papers/asplos24_bypassd.pdf) | ASPLOS 2024 | 用户态共享 SSD，复用 IOMMU 做 VBA→LBA + 权限检查 | 真实软件/SSD 实验；硬件 IOMMU 改动用模型评估；ext4 without data journaling，不能写完全无日志 |
| [Bermuda](https://arxiv.org/pdf/2508.19670) | arXiv 2025；会议未核 | 混合关键性系统 IOMMU 干扰，反向工程 micro-TLB 与竞争 | 单板 ZCU104/Arm SMMUv2；跨架构相似性未验证 |
| [ShareMMU](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf) | MICRO 2026 camera-ready | Permission Tracker 验证 pre-translated access，支持不可信加速器共享 | gem5 PARADE；无真实硅/独立复现；非商用结论 |

### D. 间接访存、布局、搬运与同步接口

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [IMPICA](https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf) | ICCD 2016 | 3D 堆叠内存内 pointer chasing + address decoupling | gem5；仅适合可区域化链表等结构 |
| [NDS](https://par.nsf.gov/servlets/purl/10321761) | MICRO 2021 | 多维地址空间和 building blocks，缓解维度不匹配 | 作者报告自建 TLC SSD/控制器 + NVMeoF 实验平台，不应误改成纯模拟 |
| [GPS](https://harinimuthukrishnan.net/publication/gps/gps.pdf) | MICRO 2021 | 多 GPU publish-subscribe、本地副本、主动广播 | NVAS 仿真；sys-scoped 写仍要单副本 + TLB shootdown |
| [DX100](https://arxiv.org/pdf/2505.23073v1) | ISCA 2025 | 可编程数据访问加速器，重排/合并 bulk indirection | gem5+Ramulator2；明确不支持 pointer chasing |
| [Tiara](https://arxiv.org/pdf/2606.13708v1) | arXiv 2026；预印本 | NIC-side 可验证 ISA，折叠远程 indirection | FPGA 原型；PagedAttention 最优批处理基线 2 RTT，不是朴素 160 RTT |
| [WIC](https://www.usenix.org/system/files/atc25-zhang-jiajian.pdf) | USENIX ATC 2025 | warp 级中断替代轮询，隐藏 producer-consumer 同步 | RTX 4090+4×A800；UVM 256 fault 上限；并非所有场景都更好 |
| [MINISA](https://arxiv.org/pdf/2603.20623v1) | ISPASS 2026；本地 arXiv v1 | 可重配置推理加速器的最小控制 ISA | RTL/分析模型；主要优化控制/指令供给，不能直接外推为 IOMMU 性能收益；控制与数据面仍相互影响 |

### E. GPU 动态分配与 LLM 内存管理

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [Mosaic](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf) | MICRO 2017 | GPU 多页大小、CoCoA、原地合并/拆分 | GPGPU-Sim；依赖 en masse allocation |
| [PagedAttention](https://arxiv.org/pdf/2309.06180) | SOSP 2023 | KV cache 分块、块表、共享与 CoW | A100 实测；2–4× 不适用所有 GPU 负载，DNN 训练可能退化 |
| [Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) | ASPLOS 2020 | Thread oversubscription + unobtrusive eviction | 真机 Titan Xp 剖析 + MacSim；UE 未完整真机实现 |
| [vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) | ASPLOS 2025 | CUDA VMM 分离虚拟/物理，后台重叠、延迟回收、细粒度驱动支持 | 真机 A100/H100；decode 与最优 PagedAttention 持平，收益看上下文/阶段 |
| [vTensor/FlexInfer](https://arxiv.org/pdf/2407.15309v1) | arXiv 2024 | CPU 侧虚拟张量管理，解耦内存管理与 compute kernel | arXiv；LLM 服务框架 + VMM 接口；收益依赖特定 kernel，未承诺通用 |
| [GMLake](https://arxiv.org/pdf/2401.08156) | ASPLOS 2024 | VMS 缝合物理块，训练碎片整理 | 真机 8×A100；裸 VMM 慢 115× 是特定条件，不能说所有动态映射必须有缓存池 |

### F. 驻留迁移与观测

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [IDYLL](https://par.nsf.gov/servlets/purl/10492257) | MICRO 2023 | PTE directory 过滤不必要失效，惰性/批量失效 | 真机双 A100 剖析 42% invalidation 开销；零失效上界和方案收益来自 MGPUSim |
| [OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf) | HPCA 2025 | object-aware page management，动态选择迁移策略 | MGPUSim；对象非均匀/2MB 页限制；“near-optimal”相对 hypothetical Ideal |
| [ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf) | ISCA 2026 | 采样观测 HBM-resident 访问，用户态 policy | 真机 RTX 3090 单 GPU；采样依赖局部性，多 GPU 仅讨论 |

## 6. 官方规范与生产 API 索引

这些不是论文，但应作为“标准/生产 API 已有语义”的单独边界：

| 文档 | 关键语义 | 链接 |
|---|---|---|
| RISC-V IOMMU in-memory queues | IOTINVAL/IODIR/IOFENCE/ATS.INVAL；PR/PW 只覆盖已被 IOMMU 处理的请求，in-flight 设备请求可能另需互连 fence；允许简单实现扩大为全局失效 | [RISC-V IOMMU queues](https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html) |
| CUDA stream-ordered allocator | stream-ordered 分配/释放、pool、release threshold、event 依赖复用；不承诺零分配延迟 | [CUDA stream-ordered memory allocator](https://developer.nvidia.com/blog/using-cuda-stream-ordered-memory-allocator-part-1/) |
| Linux HMM | 页表镜像、mmu_interval_notifier、migrate_vma_*、跨设备 MMU 失效；不等于数据缓存一致性 | [Linux HMM](https://kernel.org/doc/html/latest/mm/hmm.html) |
| Linux SVA/ENQCMD | 同一虚拟地址、mmu_notifier 同步 device TLB、PASID/ATS/PRI 路径；与迁移/一致性分层 | [Linux SVA](https://kernel.org/doc/html/latest/arch/x86/sva.html) |

需要特别避免把“同一地址、映射一致性、数据缓存一致性、迁移”四件事混成一件。HMM/SVA 文档说明它们是不同语义层；也不能由此推出 CVA6 + RISC-V IOMMU + Linux/Genesys2 平台已实现 ATS/PRI/SVA 或 HMM 式迁移。

## 7. 2025/2026 前沿窗口与边界索引

### 7.1 前沿窗口：改变的假设与仍付出的代价

| 前沿 | 改变的假设 | 仍付出的代价 |
|---|---|---|
| HDPAT | wafer-scale 集中 IOMMU 可用分层缓存、重定向、预取缓解排队 | MGPUSim 仿真；排除 GPU-to-GPU migration/shootdown；不能泛化到所有 IOMMU |
| Revelator v2 | OS 可参与 tiered hash allocation，硬件推测数据与最后一级 PTE | OS 分配协同仍是前提；Linux 原型 + Sniper/Virtuoso + RTL 综合，不是完整真实处理器加速 |
| ShareMMU | 不可信加速器可共享翻译，Permission Tracker 验证 pre-translated access | 仍需 dirty write-back/flush acknowledgment；gem5 PARADE 仿真；无真实硅/独立复现 |
| DX100 | 批量 indirection 可重排/合并 | 明确不覆盖 pointer chasing；gem5 + Ramulator2 |
| MINISA | 可重配置推理加速器需要最小控制 ISA | 主要优化控制/指令供给，不能直接外推为 IOMMU 性能收益；控制与数据面仍相互影响 |
| ObservUVM | 采样观测可近似 access bits，policy 可放用户态 | 单 GPU 真机；依赖空间局部性；只观测 key region；多 GPU 仅讨论 |
| OASIS | 对象粒度可降低逐页策略学习开销 | MGPUSim；对象内模式需大体一致；2 MiB 页会挤压 private；相对 hypothetical Ideal |
| Tiara | 远程应用间接表可在 NIC 侧用可验证 ISA 折叠 | arXiv 2026 预印本；应用间接表≠硬件 OS 页表；最优批处理基线是 2 RTT，不是朴素 160 RTT |

表格只记录本批材料中的前沿条件，不重复硬件数字，也不把某个机制写成已交付通用方案。

### 7.2 边界分级索引

以下仅摘要/局部原文/作者页面已核，未纳入本稿“已全文模型阅读”计数，也不能据此作新颖性裁决：

| 文献 | 状态 | 说明与链接 |
|---|---|---|
| Pond | ASPLOS 2023 | 研究 CXL 内存池容量浪费与延迟权衡，用生产 trace + 仿真/延迟模拟；不是 2023 已实物部署 CXL 全栈。仅相邻分支。[官方 PDF](https://www.microsoft.com/en-us/research/wp-content/uploads/2022/10/2023_Pond_asplos23_official_asplos_version.pdf) |
| HELM | SC 2025 | 用 fault telemetry 构建 locality metrics；指标离线计算，不能当已交付通用在线调度器；真实 Titan V。仅可观测性近邻。[DOI](https://doi.org/10.1145/3712285.3759812) |
| CDFD | ISCA 2026 | 粗复制 + 细去重利用新 NVLink 条件与空闲显存；摘要中的低争用只属作者测量场景；作者 PDF 未全文审查。[DOI](https://doi.org/10.1109/ISCA66397.2026.00109) |
| SoftWalker | MICRO 2025 | SM 软件 walk warps 借计算资源处理翻译；完整限制/评估待读。[作者页](https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker/) |

## 8. 最容易误当创新的常规组件与强基线

下表只列复核创新时必须对照的普通方案/最近论文；不把来源中所有对手都宣告为当前 SOTA。若某问题尚无公认最强普通方案，写“至少需要比较”。

| 要解决的问题 | 必须比较的最强普通方案/最近论文 |
|---|---|
| 扩大 TLB/IOTLB 覆盖 | 单纯增大容量、大页、walk cache；对照 Victima、IOTLB wall，并检查是否只是把瓶颈移到 walker/互连 |
| 翻译调度与多租户共享 | 静态分区、统一共享、简单优先级；对照 MASK、Walk Scheduling、Walk Stealing，至少需要比较动态偷取/调度 |
| IOVA 分配与失效生命周期 | 严格失效、延迟批量失效、per-core flush；对照 rIOMMU、Utilizing the IOMMU Scalably、RISC-V IOMMU 队列语义 |
| 动态 GPU 内存与 KV cache | 连续预分配、demand paging、PagedAttention/vLLM；对照 vAttention、GMLake、Batch-aware UVM，至少需要比较可预测性与重叠条件 |
| 多 GPU/统一内存迁移与观测 | 广播失效、默认 UVM 驱逐/预取、按访问计数迁移；对照 IDYLL、OASIS、ObservUVM，核算采样和对象跟踪的成本 |
| 间接访存与布局搬运 | 软件预取/重排、PIM、近 NIC 解链；对照 IMPICA、DX100、Tiara、NDS、GPS、WIC，区分真串行链与可并行散读 |

比较应以目标应用条件、相同资源预算和相同正确性要求为前提，选择适用的强对手，解释增益来源、付出的代价及有效范围。新的研究也可以放宽先行工作依赖的条件；这需要证明新条件确有需求，并与该条件下可用的最佳办法比较。

## 9. 对本项目平台的适用范围

当前平台是 CVA6 + 四 iDMA + RISC-V IOMMU + Linux / Genesys2 FPGA。已证明的是平台功能验收 172/172，以及同 BIT 的 S/N/P/PN 干预和双方观测这一有限实验现象；这些是实验能力和证据资产，不等于论文创新，也不等于真实 GPU/CXL/PCIe ATS 或线上应用结论。PN 部分配置改善一方损害另一方，根因与普适性未定。IOTLB/PTW 的更多归因探针、map/unmap 性能测试等目前是候选测量用途，尚未执行；不能把它们写成已交付结果。平台可支持将翻译问题拆成 IOTLB/PTW 容量覆盖、walk 依赖时延、合并/调度、软件 IOVA 控制面、小 DMA 干扰、失效/同步语义分离等方向，但不能直接支持 GPU 多租户 walk stealing、200 Gbps IOTLB wall、CXL 全栈、不可信 ATS 加速器安全共享、wafer-scale 集中 IOMMU、真实多 GPU UVM 等结论。下一步应先挑少量真实应用条件，逐项对比现有论文假设和最强对手，再决定是否/怎样做实验；本轮只完成文献研究与记录，没有新增源码修改、测试、构建或上板。

## 10. 剩余范围与不确定项

本稿明确记录尚未覆盖或只做边界索引的主题：CXL 完整放置/一致性、编译器布局与稀疏格式、RDMA 注册缓存/撤销、更广实时 QoS/形式化界，以及 HELM/CDFD/SoftWalker/Pond 的全文审查；这些是后续专门深读范围，不能把未覆盖说成没有工作。题录不确定处也记录：SPARTA 仅 arXiv 2020，不猜会议；Bermuda 本地 arXiv 2025，会议未核则不写会议；Tiara 为 2026 预印本；MINISA 本地 arXiv v1，ISPASS 2026 已核但 v1 细节可能与终稿不同；ShareMMU 为 MICRO 2026 camera-ready，不代表商用或独立复现；vAttention 用 ASPLOS 2025 正式版，旧 2024 版仅作历史。实测、仿真、CPI/延迟模型、RTL 综合和离线分析必须分开写；仿真论文不能称为成熟部署，只能写“机制已有大量研究”或“标准/生产 API 已有”。
