# 多设备共享地址翻译：经典、前沿与卡点（2026-10-06 深化版）

> 检索截止：2026-10-06。本专题共 123 项：104 项取得全文，共 105 个全文版本；19 项题录/摘要。增量 51 项新独立工作（43 项全文 + 8 项题录/摘要）；另有 2 项旧题录 CLAP、Translation Ranger 升级为全文。105 个全文版本均有本地证据卡；关键核查为有范围检查，不等于全篇逐字核验。3 份新增全文复用先前广域地图本地文件，其余为公开来源取得。时间箭头只表示机制对照，不表示引用或因果；本文不把 CPU 侧 TLB shootdown、NUMA 页表副本或 UVM 迁移直接当成 DMA/IOMMU 的完成保证。

入口：[完整123项索引](../out/shared-translation-literature-20261006/INDEX.md) · [CSV](../out/shared-translation-literature-20261006/catalog.csv) · [BibTeX](../out/shared-translation-literature-20261006/references.bib) · [增量来源与核查记录](../out/shared-translation-literature-20261006/expansion-01/README.md)。

## 结论与增量

共享翻译的基本思想已经成熟，但翻译结构、执行位置与并发调度、多设备/近存放置，以及映射更新生命周期和可观测性仍是活跃问题。本轮把性能线整理为七条可检验脉络：经典 IOMMU 成本、翻译表示与 OS 接口、执行并发调度、多 GPU 布局与迁移、近内存/CXL 解耦、更新生命周期正确性、可观测性与关键性。核心候选矛盾集中在共享净收益是否覆盖更新维护、翻译与计算资源竞争时的真实关键性、语义与粒度如何闭环；这些是候选问题，不声称全领域空白或新颖性成立。“本批没看到”只表示本批证据未覆盖。

## 七条脉络：机制变化、代价转移、评价条件

### 1. 经典 IOMMU 成本：从软件映射到硬件 IOTLB，再回到 miss 成本与更新

经典线先由 [The Price of Safety，2007](../out/shared-translation-literature-20261006/INDEX.md#iommu_cost2007) 把主要成本定位到 TCE `map/unmap`、锁、hypercall 与地址空间切换；[IOTLB Bottleneck，2010](../out/shared-translation-literature-20261006/INDEX.md#iotlb_bottleneck2010) 用 pseudo pass-through 固定恒等映射、消掉大部分软件映射，再用单 I/OAT 通道 DMA 微基准与 trace 驱动 IOTLB 模拟，把问题转到硬件 IOTLB 容量/冲突与 miss 服务成本。2011 年 [vIOMMU](../out/shared-translation-literature-20261006/INDEX.md#viommu2011) 把未修改 guest 的 IOMMU 模拟、乐观拆除和 sidecore 作为完成条件/模拟成本的优化；2018 年 [DAMN](../out/shared-translation-literature-20261006/INDEX.md#damn2018) 把保护边界从每 DMA `unmap` 上移到用户/内核边界，用永久映射与 OS 访问时复制换掉逐 DMA 失效；2024 年 [F&S](../out/shared-translation-literature-20261006/INDEX.md#fastsafe2024) 则明确不减少 IOTLB miss 数，而是降低每次 miss 成本、保留仍有效的 PTcache，并维持 strict 撤销。

代价变化：从 CPU 软件路径转到 IOTLB/PTcache 与失效队列，再转到“复制、保留与严格性”的语义折衷。评价方法从裸机/Xen 吞吐与 CPU 利用率，走到 DMA 微基准、trace 模拟、KVM 原型和现代实机尾延迟。条件很关键：2010 的单 I/OAT 微基准不是多设备共享 IOMMU 干扰的系统评估；DAMN 有适用范围限制；F&S 仍承认 strict 下 miss 不可避免。

### 2. 翻译表示与 OS 接口：从大页/连续到推测、哈希与可扩展翻译对象

经典问题是 radix 页表串行 walk、碎片化和 huge page 的收益/代价。2011 年 [SpecTLB](../out/shared-translation-literature-20261006/INDEX.md#spectlb2011) 用 reservation 对齐规律做 VA→PA 插值并并行验证，把代价转移到误预测、验证与 I/O 副作用防护；2016 年 [Ingens](../out/shared-translation-literature-20261006/INDEX.md#ingens2016) 把 contiguity 当资源，用 util/access bitvector、后台提升/降级与主动压缩，代价是 TLB invalidation、公平性与尾延迟治理；2019 年 [Translation Ranger](../out/shared-translation-literature-20261006/INDEX.md#translation_ranger2019) 做后分配碎片整理，代价是页面迁移、shootdown、单一 VMA 且权限一致的前提；2020 年 [CA paging + SpOT](../out/shared-translation-literature-20261006/INDEX.md#capaging2020) 把连续生成放到缺页分配路径，并用 PC 索引预测表推测嵌套翻译，代价是 best-effort、后台验证和安全缓解开销；2021 年 [Midgard](../out/shared-translation-literature-20261006/INDEX.md#midgard2021) 引入 VMA 粒度中间地址空间，把翻译拆成 V2M 与 M2P，代价是 OS 大改、VLB/MLB 硬件与 store rollback；2024 年 [DPT](../out/shared-translation-literature-20261006/INDEX.md#dpt2024) 把物理内存当无界哈希页表，用 SOA/CVA/CPD 解碰撞，代价是 CPD 迁移、PTE 页不可迁移、高利用率退化；2025 年 [EMT](../out/shared-translation-literature-20261006/INDEX.md#emt2025) 把 translation object/database/service 抽象给 MMU driver，代价是 OS 接入复杂度与 ECPT 的 kernel 指令开销。GPU/加速器侧的代表是 2024 年 [FS-HPT](../out/shared-translation-literature-20261006/INDEX.md#fshpt2024)，用固定大小 HPT、step table 与 remote PTE eviction。[Utopia](../out/shared-translation-literature-20261006/INDEX.md#utopia2023) 是 CPU 虚拟内存的混合限制/灵活映射机制；[NDPage](../out/shared-translation-literature-20261006/INDEX.md#ndpage2025) 面向近数据处理（NDP）的页表 cache 行为与层级；二者不能被归为 GPU 地址翻译机制。

评价条件多为真机 Linux、QEMU/KVM、Sniper、GPGPU-Sim 与面积估计；EMT 明确不面向 IOMMU DMA，Midgard、Memento、DPT、EMT 都不是设备 SVA 方案。

### 3. 执行并发与调度：从硬件 PTW 并行到软件 warp、idle CU 与 MSHR 扩容

经典线关心 PTW 并发、队列干扰与公平，已有 [MASK](../out/shared-translation-literature-20261006/INDEX.md#mask2018)（2018）、DWS（2021）、Trans-FW（2023）等先行工作。2024 年 [STAR](../out/shared-translation-literature-20261006/INDEX.md#star2024) 探索多实例 GPU 的 TLB 子项共享；2025 年 [SoftWalker](../out/shared-translation-literature-20261006/INDEX.md#softwalker2025) 把页表遍历搬到 SM 上的 PW Warp，用 In-TLB MSHR 把 L2 TLB entry 临时当 MSHR，并让 PW Warp 获得最高调度优先级；2026 年 [cuPTW](../out/shared-translation-literature-20261006/INDEX.md#cuptw2026) 在没有可发射 kernel wavefront 时发射翻译 wavefront，即利用 idle CU。版本边界：SoftWalker 采用最高调度优先级，cuPTW 在无 kernel wavefront 可发射时发射翻译 wavefront；二者在本轮所获版本中无相互 actual 引用，不能制造引用链。代价被转移到 SM/CU 空闲资源、调度优先级、MSHR/TLB 条目占用、常规负载 slowdown 与硬件支持成本。证据条件：SoftWalker 完整设计主要 Accel-sim，实机仅 A2000 微基准/profile；cuPTW 主评估为模拟；二者都不能简单宣称“无硬件改动软件方案”。[NB-Walker](../out/shared-translation-literature-20261006/INDEX.md#nbwalker2026) 与 CATO 仍是 MICRO 2026 题录级，不能推断协议细节或性能。评价要同时看平均延迟、尾延迟、regular workload slowdown 和资源机会成本。

### 4. 多 GPU 布局与迁移：chiplet 局部性、副本、迁移与加密同步

经典线从 TLB 聚合、远端 PTE 访问走到多 chiplet/晶圆规模。2025 年 [CLAP](../out/shared-translation-literature-20261006/INDEX.md#clap2025) 按 GPU 数据结构的 chiplet locality 选页大小、预组织连续物理帧并用 TLB coalescing 扩 reach；边界：CLAP 已经评估结合迁移的扩展，不能把“静态布局+动态迁移”说成空白；其默认仍避免迁移，跨 kernel 复用需选择性迁移。2025 年 [OASIS](../out/shared-translation-literature-20261006/INDEX.md#oasis2025) 从 page 粒度转到 object 粒度，动态选择 on-touch、access counter、duplication；代价是对象 ID 位宽、O-Table 容量、oversubscription 误判和不优化 PTW。2026 年 [HDPAT](../out/shared-translation-literature-20261006/INDEX.md#hdpat2026) 做晶圆内分层缓存、重定向与主动交付翻译，但明确排除 GPU 间页迁移，也未评估 shootdown；[WaferTrans](../out/shared-translation-literature-20261006/INDEX.md#wafertrans2026) 用分布式 PTE 存在性信息定位远端翻译，但 presence 信息不等于完整 PTE/权限一致性；[Clover](../out/shared-translation-literature-20261006/INDEX.md#clover2026) 关注晶圆级页表副本存储与访问距离，仍需全文核验一致性细节；[LAEGIS](../out/shared-translation-literature-20261006/INDEX.md#laegis2026) 是 GPU 机密计算下 UVM 迁移加密的性能近邻，研究性能而非新安全漏洞。

评价条件多为 GPGPU-Sim/MGPUSim、Verilog 面积、真机 profile 或模拟；跨论文数值不能直接排榜。

### 5. 近内存与 CXL 解耦：翻译下沉设备侧，但实体与维护成本各异

经典线把 VA→PA 或 node PA→FAM PA 下沉。2016 年 [IMPICA](../out/shared-translation-literature-20261006/INDEX.md#impica2016) 在 3D 堆叠内存逻辑层做 pointer chasing，用 region-based page table，但一致性靠只在 accelerator 内执行来简化；2017 年 [DIPTA](../out/shared-translation-literature-20261006/INDEX.md#dipta2017) 用 per-vault 倒排页表、限制关联度、page interleaving 与 way predictor，让翻译与取数并行；边界：DIPTA 为 PACT 2017。2020 年 [VBI](../out/shared-translation-literature-20261006/INDEX.md#vbi2020) 把分配与 VBI→physical 翻译交给 memory controller 的 MTL，但 multi-node 未评估；2021 年 [DeACT](../out/shared-translation-literature-20261006/INDEX.md#deact2021) 解耦 access control 与 node PA→FAM PA，官方已核为 HPCA 2021；2022 年 [Clio](../out/shared-translation-literature-20261006/INDEX.md#clio2022) 在 disaggregated memory node 用 FPGA fast path 做 VA→PA、权限与硬件缺页；2023 年 [DTL](../out/shared-translation-literature-20261006/INDEX.md#dtl2023) 在 CXL controller 做 HPA→DPA 与迁移以省 DRAM 功耗；2025 年 [NDPage](../out/shared-translation-literature-20261006/INDEX.md#ndpage2025) 处理 NDP 页表 cache 行为与层级，正式程序为 DATE 2025；2024 年 [M2NDP](../out/shared-translation-literature-20261006/INDEX.md#m2ndp2024) 已核为 MICRO 2024，用 CXL.mem 承载 NDP 命令、设备侧依赖 ATS/ATC 并用 DRAM-TLB 扩 reach；[Pond](../out/shared-translation-literature-20261006/INDEX.md#pond2023) 是 CXL 内存池与 VM 放置，不是 IOMMU/SVA 共享页表。

关键边界：DeACT、DTL、Pond、RDMA ODP/TeRM/ODRP 不是同一翻译实体；不能把资源池化、RNIC 页表缺页或 HPA→DPA 都混成“共享 IOMMU”。代价转移到 SRAM/DRAM 开销、低关联度 page fault、fabric/协议延迟、迁移失效与权限验证；评价包括 SST、gem5、FPGA 原型、真机功耗与模拟。

### 6. 更新生命周期正确性：shootdown、副本、回收上下文与在途请求

1989 年 [Mach shootdown](../out/shared-translation-literature-20261006/INDEX.md#shootdown1989) 给出软件维持TLB一致性的实现，并分析远端失效同步，并已量化队列溢出、IPI 与线性增长；2013 年 [RadixVM](../out/shared-translation-literature-20261006/INDEX.md#radvm2013) 用 per-core page table 与 targeted shootdown，但 IPI 在 80 核仍近 1 ms，per-core 页表内存可达 13×；2016 年 [Shoot4U](../out/shared-translation-literature-20261006/INDEX.md#shoot4u2016) 让 VMM 直接失效被抢占 vCPU 的 guest TLB；2020 年 [Ptlbmalloc2](../out/shared-translation-literature-20261006/INDEX.md#ptlbmalloc2020) 从应用分配器减少 arena imbalance 引发的 shootdown；2024 年 [Hydra](../out/shared-translation-literature-20261006/INDEX.md#hydra2024) 做 NUMA 页表按需副本，并用“无副本则无本地 TLB 条目”过滤 shootdown；边界：Hydra 是 page-table 级 NUMA sharer，不是每 PTE/TLB 精确目录；也不能把其 CPU 条件直接外推到 DMA 设备。2024 年 [FPR](../out/shared-translation-literature-20261006/INDEX.md#fpr2024) 按 recycling context 跳过 mmap 回收页的 shootdown，但有语义限制：可能保留 stale 访问，不能用于依赖 segfault 的程序，也不优化匿名 malloc/free。2026 年 [ShadowUpdate](../out/shared-translation-literature-20261006/INDEX.md#shadowupdate2026) 处理迁移失效时传播新映射与在途迁移请求，但仍是机构摘要级。代价转移到 IPI/中断、队列容量、页表副本内存、stale 窗口、ABA、后台回收与回退。评价条件：真机多核/多 socket、KVM、少数模拟；本轮没有任何全文把 CPU shootdown 完成语义证明为 DMA 翻译失效保证，因此不能把 CPU 的迁移机制直接当 DMA 保证。

### 7. 可观测性与关键性：从 hotness 到 per-page criticality 与设备侧观测

2023 年 [Memtis](../out/shared-translation-literature-20261006/INDEX.md#memtis2023) 用 PEBS 采样页/子页访问、动态 hot/warm/cold 与 skewness-aware split，但 PEBS 对 rarely/never accessed 页区分不足；2025 年 [Soar/Alto](../out/shared-translation-literature-20261006/INDEX.md#soaralto2025) 用 AOL=Latency/MLP 直接论证“热页不一定是性能关键页”，Soar 离线对象排序、Alto 在线调节迁移强度；2026 年 [PACT](../out/shared-translation-literature-20261006/INDEX.md#pact2026) 用 PEBS + CHA/TOR queue occupancy 分解 per-tier MLP，做 20ms 在线页级 criticality 与 eager demotion/adaptive promotion。边界：Soar/Alto 与 PACT 已经给出关键性命题，不能把“按关键性调度”当新颖性。2026 年 [ObservUVM](../out/shared-translation-literature-20261006/INDEX.md#observuvm2026) 把 PCIe access counters 反用于 GPU HBM 观测；[gpu_ext](../out/shared-translation-literature-20261006/INDEX.md#gpuext2025) 用 host/device 侧 eBPF hooks 暴露 GPU 内存与调度策略；[Bermuda](../out/shared-translation-literature-20261006/INDEX.md#bermuda2025) 实测 SMMUv2 共享翻译结构造成 DMA 干扰，但只是单平台特定条件。代价转移到采样开销、离线校准、阈值调参、策略反应滞后与设备侧可见性限制。评价要区分“有计数器”与“能及时支持决策”。

## 仍值得研究的矛盾：观察到的困难、强前作、缺失证据与停止条件

| 观察到的困难 | 最强先行工作及已做的事 | 创新还缺什么 | 停止条件（怎样真正否定必要性/收益） |
|---|---|---|---|
| 共享净收益是否覆盖更新维护：高更新率、多设备、失效扇出、确认等待与尾延迟同时出现时，共享节省的 miss 成本可能被更新维护吃掉。 | Trans-FW（远端转发已结合队列压力）、MGvm（已有动态映射/负载均衡）、CLAP（结合迁移评估 chiplet locality）、HDPAT（分层缓存与主动交付）、F&S（strict 下保留 PTcache 降低 miss 成本）、ShareMMU（权限约束共享与 back-invalidation）。 | 真实多设备负载中的在线更新率、确认等待、失效队列与净收益分解；ShadowUpdate 仍仅摘要，不能当全文审定。 | 若相同语义、资源预算下，强前作或简单本地/共享切换已达到同等效果，或真实轨迹中高复用与高更新几乎不共存，则该新课题缺乏必要性。 |
| 翻译占用计算资源时的真实关键性：翻译与计算竞争 SM/CU、MSHR、TLB、带宽时，抢占可能净损失，且应用阶段变化快。 | SoftWalker（PW Warp 最高优先级）、cuPTW（idle CU 发射翻译 wavefront）、Soar（离线对象级放置）/Alto（在线调节迁移）、PACT（在线页级 criticality）。 | 真实应用阶段变化下的净损失、在线估计时效、尾延迟与资源机会成本；不能只重复“按关键性调度”。 | 若固定优先级、空闲时发射或简单阈值已覆盖全部有效收益，或理想预知策略也没有额外进度收益，则停止复杂关键性机制。 |
| 语义与粒度闭环：多粒度共存时权限变化、共享写、迁移、失效的语义能否闭环。 | CLAP、OASIS、DPT/EMT、Utopia、NDPage、DeACT、DTL、Pond、RDMA ODP/TeRM/ODRP 已分别给出页大小选择、对象级策略、限制映射、哈希页表、近存翻译、CXL 池化等机制。 | 真实 SVA/PASID 多设备权限失效实验；不同翻译实体的身份、权限、失效与回收义务必须分别定义。 | 若真实应用的粒度冲突极少，或现有页拆分/对象策略在相同权限与回收约束下已消除损失，则不再发展新的粒度机制。 |
| 多 GPU/多 chiplet 页表副本与远端访问：动态迁移和高更新下副本维护成本可能超过收益。 | CLAP（chiplet locality、页大小与迁移）、MGvm（动态映射与负载均衡）、Clover（副本存储）、WaferTrans（远端 presence）、HDPAT（分层缓存与重定向）、Barre Chord（聚合映射与计算式翻译）。 | 副本一致性维护、跨 kernel 复用、动态迁移与高更新下副本成本；Clover 仍仅摘要，不能当全文。 | 若强前作的按需复制/动态放置已达到同预算下的收益上限，或理想放置消除远端遍历仍几乎不改善应用进度，则停止新增副本策略。 |
| 近内存/CXL 设备侧翻译的权限与协议：多个设备争用主机翻译服务，且各自ATC需要同步失效时的队列与尾延迟。 | DIPTA、DeACT、Clio、DTL、NDPage、M2NDP、SPARTA 已做近存页表、设备侧 VA→PA、HPA→DPA、ATS/ATC、DRAM-TLB 等机制或原型。 | 真实CXL/NDP设备的ATS路径、私有ATC失效与共享主机翻译服务的联合证据；不能把 Pond 的 VM 放置当 IOMMU SVA。 | 若真实设备中ATS/翻译不是主导成本，或现有ATC加标准失效批处理已满足同语义目标，则停止专门优化翻译；先补证据再谈新协议。 |
| 正确设备失效协议下慢设备/命令队列更新对其他设备进度的影响：不是缺少协议，而是慢 ACK、命令队列更新可能拖累其他设备进度，缺真实应用并发/更新频率/慢 ACK 证据。 | ShareMMU（设备侧失效与 back-invalidation 协议）、IDYLL（轻量 PTE invalidation）、F&S（strict IOTLB/PTcache 失效）、UNITD（硬件翻译一致性）。 | 真实应用中的并发强度、更新频率、慢 ACK 分布，以及慢设备对其他设备进度的因果测量。 | 若真实应用并发/更新频率下慢 ACK 不显著影响其他设备进度，或强前作/简单 baseline 已在相同协议下解决进度隔离，则停止。 |

## 四个开放研究问题（问题表述，不宣称论文创新成立）

1. 在真实多设备负载中，共享净收益何时覆盖更新维护成本？需要哪些可观测状态来预测切换？
2. 翻译与计算竞争同一资源时，真实关键性如何变化，在线估计能否及时避免净损失？
3. 多粒度共存时，权限、共享写、迁移和失效的语义闭环需要哪些真实设备/协议证据？
4. 哪些设备侧/IOMMU 侧观测能在真实负载中及时证伪一个共享/复制策略，而不是只给平均吞吐？

## 分阶段必读清单（20篇，分4组）

**第1组：经典能力边界（成熟区，先读）**

- [Mach shootdown（1989）](../out/shared-translation-literature-20261006/INDEX.md#shootdown1989)：理解远端失效完成、队列溢出与同步屏障。
- [The Price of Safety（2007）](../out/shared-translation-literature-20261006/INDEX.md#iommu_cost2007)：软件 map/unmap、锁与 hypercall 成本。
- [IOTLB Bottleneck（2010）](../out/shared-translation-literature-20261006/INDEX.md#iotlb_bottleneck2010)：用微基准隔离软件成本后观察 IOTLB 瓶颈。
- [Translation Ranger（2019）](../out/shared-translation-literature-20261006/INDEX.md#translation_ranger2019)：后分配 contiguity 与迁移税。
- [RadixVM（2013）](../out/shared-translation-literature-20261006/INDEX.md#radvm2013)：per-core 页表、targeted shootdown 与 IPI 上限。

**第2组：共享后的代价与更新（成熟区到前沿）**

- [Hydra（2024）](../out/shared-translation-literature-20261006/INDEX.md#hydra2024)：NUMA 页表 sharer 过滤 shootdown 及其边界。
- [Fast Page Recycling（2024预印本）](../out/shared-translation-literature-20261006/INDEX.md#fpr2024)：recycling context 跳过 shootdown 及其语义限制。
- [F&S（2024）](../out/shared-translation-literature-20261006/INDEX.md#fastsafe2024)：strict 安全下降低每次 IOTLB miss 成本。
- [ShareMMU（MICRO2026已接收）](../out/shared-translation-literature-20261006/INDEX.md#sharemmu2026)：不可信加速器间受权限约束的翻译共享与失效确认。
- [UNITD（2010）](../out/shared-translation-literature-20261006/INDEX.md#unitd2010)：把翻译一致性纳入 cache coherence 的硬件路径。

**第3组：多GPU与翻译结构（前沿）**

- [MGvm（2022）](../out/shared-translation-literature-20261006/INDEX.md#mgvm2022)：MCM GPU 的页表放置、动态映射与负载均衡。
- [Trans-FW（HPCA 2023）](../out/shared-translation-literature-20261006/INDEX.md#transfw2023)：远端转发、队列压力与多 GPU 页表遍历。
- [CLAP（2025）](../out/shared-translation-literature-20261006/INDEX.md#clap2025)：chiplet locality、页大小选择与结合迁移的边界。
- [DPT（2024）](../out/shared-translation-literature-20261006/INDEX.md#dpt2024)：把物理内存当无界哈希页表的翻译结构。
- [EMT（2025）](../out/shared-translation-literature-20261006/INDEX.md#emt2025)：MMU driver 抽象 translation object/database/service 的 OS 框架。

**第4组：执行并发、关键性与可观测（前沿，判断成熟区）**

- [SoftWalker（2025）](../out/shared-translation-literature-20261006/INDEX.md#softwalker2025)：PW Warp 最高优先级与 In-TLB MSHR。
- [cuPTW（2026）](../out/shared-translation-literature-20261006/INDEX.md#cuptw2026)：idle CU 发射翻译 wavefront 及其依赖。
- [Soar/Alto（2025）](../out/shared-translation-literature-20261006/INDEX.md#soaralto2025)：AOL 与 hotness 非关键性。
- [PACT（2026）](../out/shared-translation-literature-20261006/INDEX.md#pact2026)：在线页级 criticality 及其限制。
- [ObservUVM（2026）](../out/shared-translation-literature-20261006/INDEX.md#observuvm2026)：GPU页观测如何支持驱逐/预取决策（单GPU证据）。

## 需补全文重点与影响判断

- **ShadowUpdate**：影响“高更新率/迁移失效时共享是否仍净收益”的判断；目前仅机构摘要，不能论证协议细节或性能。
- **LATPC**：影响“GPU TLB 预取、MSHR 压缩与批量 PTW 是否已覆盖”的判断；仅机构摘要，不能只把 MSHR 当被忽略资源。
- **Clover**：影响“晶圆级页表副本存储量与一致性维护成本”的判断；摘要级，复制并非越多越好。
- **NBWalker**：影响“非阻塞 NUMA GPU PTW 是否已有直接前作”的判断；MICRO 2026 题录，未获全文。
- **CATO**：影响“争用感知翻译卸载”的判断；题录/摘要级，不能仅凭标题推断性能或协议。
- **CDFD**：影响“多 GPU 粗粒度复制后细粒度去重”的成熟度判断；metadata-only，不能论证协议细节/性能/成熟度。
- **LIBRA**：影响“多 GPU 页预取与迁移协调”的成熟度判断；metadata-only，作者 PDF 404，无摘要可核。
- **SagePTE**：影响“嵌套翻译 leaf 开销与 guest/host PTE 共置”的判断；MICRO 2026 接收尚待召开，机构页面标注全文公开时间为 2026-11-04，不能论证实现细节。另需注意 TRANS-FETCH、Maverick 也仅为 MICRO 2026 接收/题录级，LAEGIS 是性能近邻。

## 阅读证据分级、限制与分工

证据分级采用两条正交维度。材料可得性：全文、摘要、题录；评估方法：实机剖析、实机完整方案、模拟、分析、未评估。两条维度不互相替代，不能简单说“实机 > 模拟”；LAEGIS、Clio、DPT 等实际包含混合方法，应按具体结论标注其材料与评估条件。限制：未复现论文实验；本批未覆盖不等于全领域无人做；跨论文数字不可直接排榜；不把 CPU 迁移机制直接当 DMA 保证。分工：DeepSeek 完成分组全文证据提取与草拟，Codex 完成原始来源检索、关键核查与验收；未复现论文实验。