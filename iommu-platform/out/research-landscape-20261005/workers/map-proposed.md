# 共享 IOMMU、地址翻译与异构数据搬运：2026-10-05 研究地图

> 本稿是 `docs/research-landscape-20261005.md` 的草案。证据基础为 [ROOT_REVIEW.md](../out/research-landscape-20261005/ROOT_REVIEW.md)、[source-manifest.json](../out/research-landscape-20261005/source-manifest.json)、四份 worker 原稿，以及本任务实际附上的 Revelator 2026 v2 全文提取。四份 worker 是原稿，不是权威结论；本稿采用 ROOT_REVIEW 的纠错与边界。除 Revelator v2 所附全文外，本次没有重新逐篇阅读全部原始 PDF，因此不声称对所有原文做过逐字复核。论文细节行号以 worker 原稿为准，完整证据卡见 [L1](../out/research-landscape-20261005/workers/L1_translation.md)、[L2](../out/research-landscape-20261005/workers/L2_sharing.md)、[L3](../out/research-landscape-20261005/workers/L3_data_lifecycle.md)、[L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md)。

## 1. 直接结论

地址翻译与共享 IOMMU 并不是一个“缺一个机制”的空白领域。主干问题已有大量有效方法：translation/page-walk cache、direct segment、多页支持、GPU/IOMMU page-walk 合并与调度、IOVA 分配与失效批处理、hugepage-aware mapping、cache-backed TLB、memory-side translation、GPU UVM 迁移与观测、权限跟踪、hash-based speculative translation 等。真正的困难出现在规模、粒度、共享、动态变化和跨层信息条件同时出现时：翻译覆盖与面积/时延冲突；共享结构和多租户使干扰、公平与可预测性分离；动态放置与迁移会触发翻译更新和失效；运行时常常看不到设备侧真实访问；间接访存的“真串行地址依赖”和“可批量散读”必须分开；单芯片物理扩展最终会撞上互连、容量、功耗和一致性限制。这是本次跨论文综合判断，不是无条件“不可能三角”。本次证据不能支持“全领域没有通用结论”，也不能把某个论文未讨论的内容自动称为全领域空白。

## 2. 六条研究线总表

| 研究线 | 经典问题 | 现在前沿 | 已有较强机制 | 受限条件与边界 |
|---|---|---|---|---|
| 1. 翻译覆盖/延迟 | radix 页表 TLB miss 后多级 walk、TLB reach 不足 | wafer-scale 集中 IOMMU 排队；OS/HW 协同 hash 推测翻译；大 TLB 与 cache-backed TLB | translation cache、walk cache、large page、direct segment、SPARTA、Victima、Revelator v2、HDPAT | TLB miss 不等于完整未缓存 walk；命中率、PTW 时延、应用吞吐、能耗不是同一指标；CPU/GPU/IOMMU/ATS 不能互推 |
| 2. 共享干扰与服务顺序 | LLC/TLB/PTW 共享时按需求竞争，不按收益分配 | 多租户 GPU walker stealing；共享 IOTLB/PTW 调度；混合关键性 IOMMU 干扰 | UCP、KPart、MASK、walk scheduling、walk stealing、IOTLB wall | 静态分区可能牺牲利用率，动态共享可能牺牲可预测性；没有形式化“隔离与利用率不能同时最优”定理 |
| 3. I/O 软件管理/映射生命周期 | map/unmap、IOVA 分配、IOTLB 失效高开销 | ring-aware IOMMU、per-core deferred invalidation、VBA 复用、stream-ordered allocator、RISC-V IOTINVAL/IODIR/IOFENCE | rIOMMU、Utilizing the IOMMU Scalably、IOTLB wall、BypassD、CUDA stream allocator | 合法语义不等于高性能；严格/延迟/权限跟踪代价不同；非 ATS 设备不是只能存物理地址 |
| 4. 间接访存/布局/搬运接口 | pointer chasing、维度不匹配、多 GPU 数据搬运与同步 | DX100 批量散读重排；Tiara 远程内存 ISA；NDS 多维布局；WIC warp 中断 | DAE、IMPICA、DX100、NDS、GPS、Tiara、WIC | 真串行地址依赖不同于可并行 scatter/gather；PIM/NIC 解链只是已见方案之一，不是唯一道路 |
| 5. GPU 动态分配 | UVM demand paging、碎片、KV cache 动态增长 | PagedAttention 与 vAttention 路线对撞；vTensor 解耦；GMLake 缝合 | PagedAttention、Batch-aware UVM、vAttention、vTensor、GMLake | 动态映射既非总是瓶颈，也非永远可隐藏；取决于可预测性、重叠、缓存池、粒度、驱动和阶段 |
| 6. 驻留迁移与观测 | UVM 驱逐/迁移、迁移后翻译失效 | IDYLL 定向/惰性失效；OASIS 对象粒度；ObservUVM 采样观测；HMM/SVA 接口 | IDYLL、OASIS、ObservUVM、CUDA stream allocator、Linux HMM/SVA、RISC-V IOMMU 队列 | 运行时看不到 HBM 访问；采样有代价；失效/迁移/数据一致性是不同层；多 GPU 结论仍需条件 |

## 3. 全局五到六个卡点

### 卡点 1：粒度不一致
地址翻译天然是页/PTE 粒度，内存策略可能是对象、区域、batch 或虚拟张量粒度。IDYLL 在 PTE 粒度过滤和批量失效；OASIS 用对象粒度降低逐页学习开销，但对象内页面模式必须大体一致，2 MiB 页会把 private 挤成 shared；ObservUVM 用 2 MiB 区域采样再借 64 KiB 采样页推断活跃性；vAttention 保留虚拟连续但物理使用 64 KiB 到 2 MiB 的 page-group；Batch-aware UVM 则在 batch 粒度权衡 fault 并发与过早驱逐。原始证据见 [L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md) 对 IDYLL、OASIS、ObservUVM、vAttention 的卡片，以及 [L3](../out/research-landscape-20261005/workers/L3_data_lifecycle.md) 对 Batch-aware UVM/vAttention 的卡片。强解法不是“统一所有粒度”，而是承认不同粒度各有适用条件，并把切换成本纳入设计。

### 卡点 2：共享利用率与可预测性
共享资源上，静态容量分配能提供部分隔离但可能低利用；动态共享能提升吞吐但可能破坏可预测性。MASK 显示多地址空间共享 GPU L2 TLB 和 page walker 时，translation 会成为一阶瓶颈，token/bypass/DRAM 调度有收益；walk stealing 显示静态划分 walker 会 under-utilization，动态偷取提升多租户吞吐，但 DWS++ 需以公平换吞吐；KPart 在真实 Intel LLC 上显示 per-app way partition 粒度粗、associativity 低，聚类后才改善。原始证据见 [L2](../out/research-landscape-20261005/workers/L2_sharing.md)。这里只能写成：本次证据未确立跨平台通用隔离结论，不能写成“隔离与利用率不能同时最优”的形式化不可能定理。

### 卡点 3：动态放置与更新代价
迁移和动态映射的真正代价常落在翻译更新：page migration 改变映射后，旧 TLB/PTE 必须失效。IDYLL 的真实双 A100 剖析报告平均约 42% 执行时间耗在 page table invalidation；其零失效上界和提出方案收益来自 MGPUSim 仿真，因此应写成“真机问题剖析 + 模拟方案评估”，不能说整篇全实物，也不能说整篇无实物。RISC-V IOMMU 已有 IOTINVAL、IODIR、IOFENCE、ATS.INVAL 等语义，但官方队列文档说明 PR/PW 只覆盖已被 IOMMU 处理的请求，尚未抵达的设备在途请求可能另需互连 fence；标准支持按上下文/地址失效，也允许简单实现扩大为全局失效，不能把实验 quiet 条件当作通用规范要求。证据见 [L4](../out/research-landscape-20261005/workers/L4_migration_observability_recovered.md) 和 RISC-V 官方文档。

### 卡点 4：缺信息，且测信息有成本
运行时经常缺关键信息。ObservUVM 指出 UVM driver 看不到 GPU 对 HBM-resident page 的访问，只能看 DRAM-resident page fault；NVIDIA access counters 原本只跟踪 GPU→DRAM over PCIe，不直接跟踪 HBM。它主动把选定 2 MiB 区域的一部分 64 KiB 采样页迁到主存，再借 PCIe 访问计数推断活跃性，不是计数器直接全量监控显存；依赖空间局部性，采样有代价，当前实测单 GPU，原文用 HBM 泛称 GPU 显存，不能据此说 RTX 3090 采用 HBM 芯片。IDYLL 也显示 driver 不知道哪些 GPU 持有有效翻译副本，基线向所有 GPU 广播失效。强解法已有：采样观测、对象跟踪、权限跟踪、per-context 失效；但“看到什么、何时看到、以什么代价看到”仍是独立问题。

### 卡点 5：访存位置、依赖、布局与可编程性
间接访存必须分开两类：地址依赖远端返回数据的真串行链，和地址已可批量发出的 scatter/gather。IMPICA 系统化 pointer chasing 的串行依赖；DX100 明确不支持 pointer chasing，只做 array-based indirection；Tiara Table 1 中 PagedAttention 的朴素 stop-and-wait 是 160 RTT，但最优批处理基线是 2 RTT，不能用 160 RTT 讲必要性；其应用间接表遍历不是硬件 OS 页表，已知地址的散读不能算不可并行的指针链。NDS 从维度不匹配处理 marshalling 与带宽利用，GPS 用 publish-subscribe 和弱内存模型合并写，WIC 用 warp 级中断替代轮询。原始证据见 [L3](../out/research-landscape-20261005/workers/L3_data_lifecycle.md)。本稿不把“只能 PIM/NIC”写成结论；只能写：对真串行地址依赖，已见 PIM/近 NIC/近内存解链方案，其他预测、helper thread、多链并行仍需逐案验证。

### 卡点 6：静态资源和物理扩展极限
增加 TLB、IOTLB 或 walker 并不自动消除瓶颈。Victima 说明大 TLB 的延迟/面积/功耗代价；HDPAT 说明 wafer-scale GPU 中，数百 GPM 的非本地翻译汇聚中央 IOMMU，中央 walker 数量和 mesh 多跳造成排队，因此提出分层缓存、重定向和预取。但 HDPAT 明确把 GPU-to-GPU 迁移排除在范围外，因此没有评估 TLB shootdown 影响；其结果是 MGPUSim 仿真，不能泛化成每个 IOMMU 的所有瓶颈。SPARTA 则把部分翻译推向 memory-side partition，以面积/功耗和物理放置约束换覆盖。direct segment 也需要连续物理内存、统一权限和 OS 修改。强解法已有，但扩展瓶颈会转移到互连、peer cache、一致性、功耗和失效成本。

## 4. 关键论文的限制与评估性质

| 文献 | 关键限制 | 实测/仿真区分 |
|---|---|---|
| HDPAT | 研究 wafer-scale 集中请求与多跳延迟；排除 GPU-to-GPU 迁移，未评估 TLB shootdown；不能泛化成所有 IOMMU | MGPUSim 仿真，非真实 wafer-scale 硅 |
| MASK | 面向 GPU spatial multiplexing 和共享 L2 TLB/page walker；不是 CPU/SoC IOMMU，不是 ATS 设备侧 TLB | GPGPU-Sim/Mosaic 模拟 |
| IDYLL | 多 GPU UVM 迁移触发 PTE invalidation；大页收益下降；不能当真实多 GPU 硬件全量结论 | 真机双 A100 剖析平均 42% invalidation 开销；零失效上界和方案收益来自 MGPUSim |
| ObservUVM | 采样观测依赖空间局部性；只观测 key region；单 GPU；多 GPU 仅讨论；不能把 RTX 3090 写成 HBM 芯片 | 真机 RTX 3090 + Ryzen 7950X |
| ShareMMU | Permission Tracker 对私有 TLB 包含，驱逐需定向 back-invalidation；权限下降可只清受影响设备权限，但仍等待 dirty write-back/flush acknowledgment；没有消灭全部动态更新/排空成本 | MICRO 2026 camera-ready，gem5 PARADE 仿真；非商用、非独立复现 |
| vAttention | 需改驱动支持细粒度；映射可与 LLM 计算重叠；decode 与强 PagedAttention 可持平，收益看上下文/阶段；不承诺所有动态场景都有瓶颈或都被解决 | ASPLOS 2025 正式版；真机 1–2×A100/H100 与在线/离线评估 |
| Revelator v2 | OS 必须参与 hash-based allocation；支持 THP/NUMA 不取消 OS 分配协同要求；不是完整真实处理器加速已实现 | Linux 6.10.8 原型验证分配；性能用 Virtuoso/Sniper 模拟；RTL 综合估面积/功耗 |

## 5. 代表文献索引（约 36 篇，按机制族分组；合并重复版本）

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
| [IOMMU Scalably](https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf) | ATC 2015 | scalable IOVA 分配、identity/kmalloc/magazines、per-core deferred invalidation | 真实 Linux/10GbE；strict invalidation 不 scalable，有安全窗口 |
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
| [MINISA](https://arxiv.org/pdf/2603.20623v1) | ISPASS 2026；本地 arXiv v1 | 可重配置推理加速器的最小控制 ISA | RTL/分析模型；与数据搬运/IOMMU 正交 |

### E. GPU 动态分配与 LLM 内存管理

| 短称 | 年份/venue | 做了什么 | 条件/评估性质 |
|---|---|---|---|
| [Mosaic](https://research.ece.cmu.edu/safari/pubs/mosaic-micro17.pdf) | MICRO 2017 | GPU 多页大小、CoCoA、原地合并/拆分 | GPGPU-Sim；依赖 en masse allocation |
| [PagedAttention](https://arxiv.org/pdf/2309.06180) | SOSP 2023 | KV cache 分块、块表、共享与 CoW | A100 实测；2–4× 不适用所有 GPU 负载，DNN 训练可能退化 |
| [Batch-aware UVM](https://hparch.gatech.edu/papers/kim-asplos20.pdf) | ASPLOS 2020 | Thread oversubscription + unobtrusive eviction | 真机 Titan Xp 剖析 + MacSim；UE 未完整真机实现 |
| [vAttention](https://apanwariisc.github.io/publications/asplos-2025-vattention/vattention-asplos25.pdf) | ASPLOS 2025 | CUDA VMM 分离虚拟/物理，后台重叠、延迟回收、细粒度驱动支持 | 真机 A100/H100；decode 与最优 PagedAttention 持平，收益看上下文/阶段 |
| [vTensor/FlexInfer](https://arxiv.org/pdf/2407.15309v1) | arXiv 2024 | CPU 侧虚拟张量管理，解耦内存管理与 compute kernel | arXiv；与 vAttention 机制不同 |
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

### 7.1 已具备较强全文/关键节证据的前沿

- [HDPAT](https://sarchlab.org/hdpat_hpca_2026.pdf)：HPCA 2026，wafer-scale GPU 分布式翻译；重要限制是排除 GPU-to-GPU migration，未评估 TLB shootdown，结果为仿真。
- [Revelator v2](https://arxiv.org/pdf/2508.02007v2)：ISCA 2026，DOI 10.1109/ISCA66397.2026.00147；v2 标题为 System-Software-Guided；Linux 6.10.8 原型 + Sniper/Virtuoso + RTL 综合；OS 分配协同仍是前提，不照搬 v1 旧数字。
- [ShareMMU](https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf)：MICRO 2026 camera-ready；Permission Tracker、定向 back-invalidation、权限下降局部化；仍等待 dirty write-back/flush acknowledgment，未消灭全部动态更新/排空成本；只作为前沿条件，不据此转安全主线。
- [DX100](https://arxiv.org/pdf/2505.23073v1)：ISCA 2025，批量 indirection 重排；明确不覆盖 pointer chasing。
- [MINISA](https://arxiv.org/pdf/2603.20623v1)：ISPASS 2026；本地 arXiv v1；控制面 ISA，与数据搬运正交。
- [ObservUVM](https://www.csa.iisc.ac.in/~arkapravab/papers/ISCA26_ObservUVM.pdf)：ISCA 2026；真机 RTX 3090，采样观测。
- [OASIS](https://yueqiwang42.github.io/assets/pdf/papers/OASIS_HPCA25.pdf)：HPCA 2025，对象感知多 GPU page management。
- [Tiara](https://arxiv.org/pdf/2606.13708v1)：arXiv 2026 预印本，不是 NSDI 2022 同名负载均衡器；应用间接表≠硬件 OS 页表；Table 1 中 PagedAttention 最优批处理为 2 RTT，不能只拿朴素 160 RTT 讲必要性。

### 7.2 边界分级索引

以下仅摘要/局部原文/作者页面已核，未纳入本稿“已全文模型阅读”计数，也不能据此作新颖性裁决：

| 文献 | 状态 | 说明与链接 |
|---|---|---|
| Pond | ASPLOS 2023 | 研究 CXL 内存池容量浪费与延迟权衡，用生产 trace + 仿真/延迟模拟；不是 2023 已实物部署 CXL 全栈。仅相邻分支。[官方 PDF](https://www.microsoft.com/en-us/research/wp-content/uploads/2022/10/2023_Pond_asplos23_official_asplos_version.pdf) |
| HELM | SC 2025 | 用 fault telemetry 构建 locality metrics；指标离线计算，不能当已交付通用在线调度器；真实 Titan V。仅可观测性近邻。[DOI](https://doi.org/10.1145/3712285.3759812) |
| CDFD | ISCA 2026 | 粗复制 + 细去重利用新 NVLink 条件与空闲显存；摘要中的低争用只属作者测量场景；作者 PDF 未全文审查。[DOI](https://doi.org/10.1109/ISCA66397.2026.00109) |
| SoftWalker | MICRO 2025 | SM 软件 walk warps 借计算资源处理翻译；完整限制/评估待读。[作者页](https://skku-compasslab.github.io/compasslab/publications/250715-micro-softwalker/) |

## 8. 最容易误当创新的常规组件与强基线

- 单纯扩大 TLB/IOTLB：Victima 已显示大 TLB 有延迟、面积、功耗代价；walk stealing/walk scheduling 显示调度和共享干扰是独立瓶颈。
- 单纯大页：IOTLB wall 显示 2 MiB mapping 可缓解，但作者明确不是 panacea；Mosaic 显示 demand paging 下大页可严重变慢。
- 单纯 next-N prefetch：HDPAT、Revelator、Shin 等都在预取/合并上做了约束处理；只加“预取 N 页”没有处理带宽、碎片、失效。
- 简单 shared TLB/central IOMMU：HDPAT 专门显示 wafer-scale 集中 IOMMU 瓶颈；Cong 2017 也显示 host MMU 可成瓶颈。
- 简单 deferred invalidate：rIOMMU 和 IOMMU Scalably 已系统分析；关键不是“延迟”，而是语义、安全窗口、可扩展性和可测代价。
- 简单静态配额：UCP/KPart/walk stealing 都显示静态等分/静态分区有隔离优点但利用率/关联度差。
- 把零拷贝、epoch、动态映射口号当缺口：BypassD 的关键是硬件权限检查和文件元数据在内核，不是零拷贝；GPS 用作用域和订阅/广播，Tiara 用注册期静态验证；vAttention 与 GMLake 说明动态映射取决于可预测性、重叠、缓存池、粒度和驱动实现。
- “加自适应/智能/端到端”但没有目标函数：本批论文的自适应都有明确目标，如 miss utility、speedup、total IPC、fairness、PTW work、IOTLB miss/MiB、迁移等待等。
- 强基线必须对照：SpecTLB/SpOT/ECH/DMT/ASAP/Mosaic Pages/POM-TLB/L2 TLB-64K；PagedAttention/vLLM/FlashInfer/FlashAttention paged；KPart/Vantage/NoPart；MASK/SharedTLB/static partition；rIOMMU/strict/defer/no-IOMMU；Border Control/CryptoMMU；GPS/RPC/NDS 软件版；DX100 的 DMP 间接预取器；Tiara 的 RPC/RedN/PRISM；WIC 的 GPS/EffShare/FinePack 等。

## 9. 对本项目平台的适用范围

当前平台是 CVA6 + 四 iDMA + RISC-V IOMMU + Linux / Genesys2 FPGA。它已通过 172/172 功能验收，有同 BIT 的 S/N/P/PN 干预和双方观测。这些是实验能力和证据资产，不等于论文创新，也不等于实际 GPU/CXL/PCIe ATS 或线上应用结论。Pn 部分配置改善一方损害另一方，根因与可泛化性未确立。

本平台能支持的：把翻译问题拆成 IOTLB/PTW 容量覆盖、walk 依赖时延、合并/调度、软件 IOVA 控制面、小 DMA 干扰、失效/同步语义分离；可借鉴 bermuda 式微基准、BypassD 式翻译命中/miss 成本分离、iommu_scalable 式控制面分类，但必须按平台条件重新定义。它不能直接支持 GPU 多租户 walk stealing、200 Gbps IOTLB wall、CXL 全栈、不可信 ATS 加速器安全共享、wafer-scale 集中 IOMMU、真实多 GPU UVM 等结论。当前不设计新 RTL，不把 P/N 当研究中心，不自动转安全，不以现有 FPGA 适配度筛掉大问题。下一步应挑少量真实应用条件，逐项对比现有论文假设和最强对手，之后再决定是否/怎样做实验；本轮没有源码/测试/构建/上板，也没有新编译或新实验已完成。

## 10. 剩余范围与不确定项

本稿未覆盖或只做边界索引：CXL 完整放置/一致性；编译器布局与稀疏格式；RDMA 注册缓存/撤销；更广实时 QoS/形式化界；HELM/CDFD/SoftWalker/Pond 的全文审查；部分正式版与预印本差异。不能把未覆盖说成没有工作。题录不确定处：SPARTA 仅 arXiv 2020，不猜会议；Bermuda 本地 arXiv 2025，会议未核则不写会议；Tiara 为 2026 预印本，不能说已正式会议发表；MINISA 本地 arXiv v1，ISPASS 2026 已核但 v1 细节可能与终稿不同；ShareMMU 为 MICRO 2026 camera-ready，不代表商用或独立复现；vAttention 用 ASPLOS 2025 正式版，旧 2024 版仅作历史。特别提醒：仿真论文不能称为成熟部署，只能写“机制已有大量研究”或“标准/生产 API 已有”；实测、仿真、CPI/延迟模型、RTL 综合和离线分析必须分开写。
