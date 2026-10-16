# 共享资源、IOMMU 性能与软件管理：文献地图报告

## 0. 阅读范围、边界与标注法

**本报告只把附带的 `papers/*.txt` 全文提取当作证据。**未附带文本的论文（例如 `dae1982`、`translation_cache2010`、`direct_segments2013`、`mosaic2017`、`accelerator_translation2017`、`impica2016`、`sparta2020`、`gps2021`、`bypassd2024`、`dx1002025`、`vattention2024`、`vtensor2024`、`gmlake2024`、`minisa2026`、`tiara2026`、`neighborhood2018`、`hdpat2026`、`revelator2025preprint`、`victima2023`、`wic2025`、`nds2021` 等）不在本报告证据卡内；只在“仍需补齐分支”中作为未审条目提及，不当作已验证证据。

**分层边界**（MAP_BRIEF 要求）：
- **静态容量**：LLC/IOTLB/PTW 队列的 entry 数、way 数、associativity。
- **动态映射控制面**：IOVA 分配、页表更新、IOTLB invalidation、TLB shootdown、back-invalidation。
- **数据路径**：DMA 事务、IOTLB lookup、PTW、L2/DRAM 请求调度。
- **布局变换与同步成本**：hugepage、ring/flat table、identity mapping、cacheline flush、memory barrier、per-core flush queue。
- **TLB miss ≠ 完整未缓存 page walk**：MASK 明确建模 page walk cache、并发 page walk、MSHR、L2/DRAM 队列（`papers/mask2018.txt:239-254, 415-434`）；walk_scheduling 明确 PWC 命中可把 walk 从 4 次访存降为 1–3 次（`papers/walk_scheduling2018.txt:230-235, 291-309`）；walk_stealing 也建模 PWC（`papers/walk_stealing2021.txt:166-175`）；IOTLB wall 区分最后一级 IOTLB miss 与下一级 IOMMU entry miss（`papers/iotlb_wall2023.txt:215-225, 487-499`）。因此 **IOTLB miss rate、PTW 时延、应用吞吐、加权 IPC、尾延迟不是同一指标**。
- **标准语义 vs 未验证软件实现假设**：例如 rIOMMU 的 “one IOTLB entry per ring” 依赖 ring 顺序（`papers/riommu2015.txt:106-134`）；iommu_scalable 依赖 Linux x86 的 48-bit IOVA/46-bit PA、kmalloc、GFP_DMA 等（`papers/iommu_scalable2015.txt:426-432, 494-523`）；KPart 的带宽 derating 假设各应用 MLP 相同（`papers/kpart2018.txt:457-468`）；bermuda 的 micro-TLB 深度 64 是对 ZCU104 MMU-500 的反向工程，跨架构相似性只是假设（`papers/bermuda2025.txt:229-260, 642-653`）。

**题录提醒采用 MAP_BRIEF 已核对版本**：`walk_stealing2021` 实际题名 *Improving GPU Multi-tenancy with Page Walk Stealing*, HPCA 2021；IOTLB wall 为 PeerJ Computer Science 2023；ShareMMU 为 MICRO 2026 camera-ready；bermuda 为 arXiv 2025 v1，venue 不猜。所有 URL 标注在证据卡中，取自 `source-manifest.json`。

---

## 1. 这条线的演变：经典问题 → 方法族 → 旧假设为何吃紧 → 2024–2026 前沿

### 1.1 经典起点：共享缓存“按需求”分配不等于按收益分配
- UCP 2006 把问题定义为：多核共享 LLC 上，LRU 按 demand 隐式分区，但需求高不等于收益高；应按 utility（给定 cache 大小能减少多少 miss）分区（`papers/ucp2006.txt:10-31, 98-118`）。
- 方法族：UMON（Auxiliary Tag Directory + hit counters）利用 LRU stack property 估计 miss curve；DSS 只采样 32 sets；Lookahead 算法做 way 粒度分配；替换策略加 way-partition bit（`papers/ucp2006.txt:268-276, 338-370, 378-405, 406-429`）。
- 旧假设：LRU stack property、miss 减少与 CPI 改善相关、分区按 way 粒度、全局 uniform partition 可接受、每 5M cycles 重分区。代价：无法响应细粒度 phase，牺牲 per-set 控制（`papers/ucp2006.txt:489-499`）。

### 1.2 从模拟到真实系统：way-partitioning 的粒度/associativity 陷阱
- KPart 2018 指出：商用 Intel Broadwell-D 已支持 way-partitioning，但 UCP 式“每应用一个 partition”在 8 核 12-way LLC 上只改善 1.7%，还伤害 30% mix；原因是 partition 数受 way 数限制、粒度粗、小 partition associativity 低（`papers/kpart2018.txt:11-36, 49-58, 314-323, 690-700`）。
- 方法族：先聚类应用，再在 cluster 间分区；用 miss curve 距离做 hierarchical clustering；用 speedup curve 而非 miss curve 做分配；估计带宽 contention；DynaWay 用 way-partitioning 做在线 profiling（`papers/kpart2018.txt:303-338, 390-426, 439-470, 504-533`）。
- 旧假设吃紧：真实系统没有 UMON 硬件，离线 profile 不能适应 phase；DynaWay 的 profiling 长度随核数线性增长（`papers/kpart2018.txt:70-93, 528-534`）。

### 1.3 共享地址翻译结构：从单应用 GPU 到多应用/多租户
- MASK 2018：GPU 从单应用设计转向 spatial multiplexing 后，共享 L2 TLB 竞争使一次 TLB miss 可 stall 多个 warp，address translation 成为一阶瓶颈（`papers/mask2018.txt:11-57, 120-141`）。机制族：TLB-Fill Tokens、TLB bypass cache、address-translation-aware L2 bypass、address-space-aware DRAM scheduler（`papers/mask2018.txt:828-939`）。
- walk_scheduling 2018：同一 GPU/IOMMU 系统中，**page table walk 的服务顺序**本身决定性能；irregular GPU app 的一条 SIMD memory instruction 可产生 0–256 次访存用于翻译，不同 instruction 的 “work” 差异极大（`papers/walk_scheduling2018.txt:16-38, 87-106, 291-309`）。机制：SIMT-aware PTW scheduler，按 instruction 批量、优先短 job、aging 防饿死（`papers/walk_scheduling2018.txt:414-489`）。
- walk_stealing 2021：把问题推进到 **GPU multi-tenancy**。共享 L2 TLB 和共享 page walker 是瓶颈；静态划分 walker 会 under-utilization，动态 page walk stealing（DWS）在 owner 无 pending 时偷 walk；DWS++ 用队列差值和 walk 生成率调吞吐/公平（`papers/walk_stealing2021.txt:14-32, 90-151, 389-466`）。

### 1.4 软件 IOVA 管理：从“把 IOMMU 当 MMU 复制”到按 workload 重构
- rIOMMU 2015：高带宽 NIC/PCIe SSD 使用 ring buffer，IOVA 短命且顺序可预测；把 hierarchical page table 换成 per-ring flat table，一个 ring 只占一个 IOTLB entry，只有 burst 结束才显式 invalidate（`papers/riommu2015.txt:11-35, 118-134, 395-419`）。
- iommu_scalable 2015：高并发 map/unmap 下，IOVA allocator 全局锁和 IOTLB invalidation lock 是瓶颈；提出 dynamic identity mapping、IOVA-kmalloc、per-core magazines、per-core deferred invalidation（`papers/iommu_scalable2015.txt:18-48, 100-124, 207-213, 293-308, 415-482, 574-605`）。
- 旧假设吃紧：10–40 Gbps 到 100–200 Gbps；单核无法维持高吞吐；IOTLB invalidation 等待不可接受；strict invalidation 与 scalability 冲突（`papers/iommu_scalable2015.txt:785-819`）。

### 1.5 数据路径视角：IOTLB wall
- IOTLB wall 2023：在 200 Gbps Linux 接收路径上，IOMMU 可造成最高 20% 吞吐下降，且不是 CPU-bound；IOTLB miss 在超过某个 offered rate 后突然增加，形成 “IOTLB wall”（`papers/iotlb_wall2023.txt:11-25, 62-99, 474-550`）。
- 关键变量：MTU、LRO/TSO、buffer organization、packet drop/buffer shuffling、NIC driver、PCIe 版本、IOTLB 实现；软件可用 hugepage-aware allocator + 2 MiB IOMMU mapping 恢复吞吐（`papers/iotlb_wall2023.txt:774-789, 915-930`）。但作者明确说 hugepage 不是 panacea，链路速率再升需要更大 hugepage 或 IOMMU 架构变化（`papers/iotlb_wall2023.txt:1007-1013`）。

### 1.6 混合关键性与不可信加速器：从“性能/公平”转到“时间可预测性 + 安全共享”
- bermuda 2025：混合关键性系统中，IOMMU 作为 shared translation resource 被忽视；在 Xilinx UltraScale+ ZCU104 / Arm SMMUv2 MMU-500 上，IOMMU 竞争主要影响小 payload DMA，最大延迟 1.79×；作者假设跨架构相似，但明确未验证（`papers/bermuda2025.txt:10-44, 642-653`）。
- ShareMMU 2026：多加速器系统中，shared TLB 能大幅提升性能，但不可信加速器可能忽略 shootdown、使用 stale translation；ShareMMU 用 Permission Tracker 验证每个 pre-translated access，并给出 ShareMMU-SP 抗 shared-TLB 侧信道变体（`papers/sharemmu2026.txt:15-38, 80-125, 311-348, 531-605`）。

**规模/应用变化使旧假设吃紧的因果链**：
- CPU LLC：从少量应用到大量 co-run，way-partitioning 粒度/associativity 不足 → KPart。
- GPU：从单应用到 multi-application / multi-tenant，共享 TLB 和共享 walker 从不变量变成竞争点 → MASK → walk scheduling → walk stealing。
- I/O：从 10 Gbps 到 200/400 Gbps，IOVA/invalidation CPU 开销与 IOTLB miss 数据路径瓶颈同时放大 → rIOMMU / scalable IOMMU / IOTLB wall。
- 系统形态：从单租户到混合关键性、第三方不可信加速器，性能隔离不够，还要时间可预测性与安全共享 → bermuda / ShareMMU。

---

## 2. 证据卡（10 篇）

### 卡 1：Utility-Based Cache Partitioning (UCP)
- **题录/状态/URL**：*Utility-Based Cache Partitioning: A Low-Overhead, High-Performance, Runtime Mechanism to Partition Shared Caches*，MICRO 2006；本地提取确认（`papers/ucp2006.txt:1-6, 90-91`）。URL：`https://www.eecs.northwestern.edu/~rjoseph/eecs453/papers/quereshi-micro2006.pdf`（`source-manifest.json:53-63`）。
- **针对问题**：多核共享 LLC 的 demand-based LRU 隐式分区，需求高不等于收益高。
- **核心机制**：UMON（ATD + hit counters）利用 LRU stack property 估计所有 way 数的 miss；DSS 采样 32 sets；Lookahead/greedy 分区；tag bit 实现 way partitioning（`papers/ucp2006.txt:100-118, 236-276, 338-370, 378-429`）。
- **适用假设与代价**：假设 LRU stack property、miss 减少与 CPI 改善相关、按 way 粒度全局 uniform 分区、5M cycles 重分区；代价是无法响应细粒度 phase，牺牲 per-set 控制；1920B/UMON（<0.2% L2 面积）（`papers/ucp2006.txt:489-499, 946-957`）。
- **真实评估平台/负载/强基线**：自研 alpha ISA 模拟器；SPEC CPU2000，20 个双核 multiprogrammed mix；基线 LRU、Half-and-Half、in-cache monitoring。相对 LRU 平均 weighted speedup +10.96%（最高 23%），IPCsum +16.8%，fairness +11%；in-cache 方案仅 +4%（`papers/ucp2006.txt:430-446, 453-503, 604-837`）。
- **不能推出的结论**：不能把 UCP 直接搬到 IOMMU IOTLB 或 DMA 共享；没有真实硬件、没有 multithreaded、没有安全/隔离语义；DSS 是静态采样；不能声称它解决了混合关键性或不可信设备。
- **原文定位**：`papers/ucp2006.txt:10-31, 100-118, 236-276, 378-405, 489-499, 946-957`。

### 卡 2：KPart
- **题录/状态/URL**：*KPart: A Hybrid Cache Partitioning-Sharing Technique for Commodity Multicores*，HPCA 2018（`papers/kpart2018.txt:1-8`）。URL：`https://people.csail.mit.edu/sanchez/papers/2018.kpart.hpca.pdf`（`source-manifest.json:64-74`）。
- **针对问题**：商用 way-partitioning 粒度粗、partition 数少、小 partition associativity 低，使 per-application UCP 在真实系统失效。
- **核心机制**：hierarchical clustering（Whirlpool miss curve 距离）；在 cluster 间用 speedup curve 做 Lookahead 分区；带宽 contention derating；Kauto 自动选 cluster 数；DynaWay 用 way-partitioning 做在线 profiling（`papers/kpart2018.txt:303-338, 390-426, 439-470, 476-533`）。
- **适用假设与代价**：需要 way-partitioning 硬件和详细 profiling；cluster 内不做 strict isolation；带宽 derating 假设各应用 MLP 相同（作者称实际 prefetcher throttle 使其近似，但未证明普适）；DynaWay 的 profiling 长度随核数线性增长；未解决数百核（`papers/kpart2018.txt:457-468, 528-534, 103-105`）。
- **真实评估平台/负载/强基线**：8 核 Intel Broadwell-D（12MB 12-way LLC，Intel CAT）；18 个 SPEC CPU2006/PBBS，30 个 8-application mix；基线 NoPart、NoClust（per-app way partition）、UCP、Vantage。KPart 平均 +24%（最高 79%），NoClust 仅 +1.7% 且伤 30% mix；模拟中 Vantage 平均 +22%，KPart Kbest +20%，K=2/3 +18%（`papers/kpart2018.txt:70-93, 643-647, 690-700, 1416-1435`）。
- **不能推出的结论**：KPart 不是 IOMMU 方案；不是 strict isolation；不能外推到 GPU/SoC IOMMU；Vantage 是模拟对手；不能把 cluster 内共享当作安全隔离。
- **原文定位**：`papers/kpart2018.txt:11-36, 49-58, 248-264, 303-338, 439-470, 476-484, 643-647, 690-700, 1416-1435`。

### 卡 3：MASK
- **题录/状态/URL**：*MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency*，ASPLOS 2018（`papers/mask2018.txt:1-4, 33-36`）。URL：`https://research.ece.cmu.edu/safari/pubs/MASK-address-space-aware-gpu-memory-hierarchy_asplos18.pdf`（`source-manifest.json:75-85`）。
- **针对问题**：GPU spatial multiplexing 下多地址空间共享 L2 TLB、page walker、L2/DRAM，TLB miss 可 stall 多个 warp；address translation 成为一阶瓶颈（`papers/mask2018.txt:11-57, 120-141`）。
- **核心机制**：(1) TLB-Fill Tokens 限流 shared L2 TLB fill，加 32-entry TLB bypass cache；(2) Address-Translation-Aware L2 Bypass，按 page table level hit rate 决定 bypass；(3) Address-Space-Aware DRAM Scheduler，Golden/Silver/Normal 三队列优先翻译请求；(4) ASID 做 memory protection（`papers/mask2018.txt:828-939, 944-974`）。
- **适用假设与代价**：离散 GPU、多地址空间、4-level page table、共享 L2 TLB 与 page walker；PTE 修改需 flush TLB/TLB bypass cache；page fault 和 TLB shootdown 留给 future work；token 机制需 counters；DRAM scheduler 增加队列存储；不处理不可信加速器安全模型（`papers/mask2018.txt:785-790, 888-894, 1214-1247`）。
- **真实评估平台/负载/强基线**：Mosaic/GPGPU-Sim 3.2.2；27 个 CUDA SDK/Rodinia/Parboil/LULESH/SHOC，35 对应用；基线 Static、PWCache、SharedTLB、Ideal。相对 SharedTLB +57.8% weighted speedup、+43.4% IPC、unfairness -22.4%，距 Ideal 23.2%；8192-entry shared L2 TLB 时与 SharedTLB 持平（`papers/mask2018.txt:944-974, 1109-1137, 1177-1184`）。
- **不能推出的结论**：不是 CPU/SoC IOMMU；不是 ATS/PCIe 设备侧 TLB；没有真实 GPU 实验；没有安全隔离证明；不能把“TLB miss 多”直接等同“完整 page walk 多”。
- **原文定位**：`papers/mask2018.txt:11-57, 124-141, 235-254, 828-939, 944-974, 1109-1137, 1177-1184`。

### 卡 4：Scheduling Page Table Walks for Irregular GPU Applications
- **题录/状态/URL**：ISCA 2018（`papers/walk_scheduling2018.txt:1-10, 84-85`）。URL：`https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf`（`source-manifest.json:97-114`）。
- **针对问题**：irregular GPU app 的 divergent memory access 产生大量 translation request；FCFS 服务 PTW 会阻碍 SIMT wavefront/progress；随机调度可慢 26%（`papers/walk_scheduling2018.txt:16-38, 240-279`）。
- **核心机制**：SIMT-aware PTW scheduler 在 IOMMU 内；给每个 walk request 附 instruction ID（20 bits）；score 估计该 instruction 全部 walk 所需访存数；优先低 score，并批量同一 instruction；aging 防饿死（`papers/walk_scheduling2018.txt:414-489`）。
- **适用假设与代价**：GPU 使用 IOMMU PTW；请求可携带 instruction ID；score 在 arrival 时计算，PWC 内容可能变导致不准确；扫描 pending 请求会增加非临界路径延迟；依赖 PWC 2-bit saturating counters 减少 stale；aging 阈值 2M requests（`papers/walk_scheduling2018.txt:507-543`）。
- **真实评估平台/负载/强基线**：gem5 扩展 CPU+integrated GPU；Polybench/Rodinia/Pannotia/XSBench，6 个 irregular + 6 个 regular；基线 FCFS、random。irregular 平均 +30%（最高 41%），CU stall -23%，latency gap -37%，PTW 请求 -21%；regular 基本无影响；灵敏度：1024 L2 TLB + 8 walkers 时 +25%，512 TLB + 16 walkers 时 +8.4%，1024+16 时 +5.3%；IOMMU buffer 128 → +13%，512 → +50%（`papers/walk_scheduling2018.txt:637-665, 671-681, 736-756, 772-795, 841-856`）。
- **不能推出的结论**：不是多租户方案；不是 CPU/SoC IOMMU；没有真实 GPU；没有安全隔离；不能将 PTW 调度收益外推到 IOTLB 容量或 IOVA 管理。
- **原文定位**：`papers/walk_scheduling2018.txt:16-38, 203-229, 291-309, 414-489, 637-665, 671-681, 772-795`。

### 卡 5：Improving GPU Multi-tenancy with Page Walk Stealing
- **题录/状态/URL**：HPCA 2021，实际题名 *Improving GPU Multi-tenancy with Page Walk Stealing*（`papers/walk_stealing2021.txt:1-11`）。URL：`https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DWS.pdf`（`source-manifest.json:115-125`）。
- **针对问题**：GPU spatial multi-tenancy 下，共享 L2 TLB 和共享 page walker 的 interleaving 造成不可控干扰；静态划分 walker 会 under-utilization；增加 TLB/walker 不能消除 interference（`papers/walk_stealing2021.txt:14-32, 90-151, 316-339`）。
- **核心机制**：DWS 把 walker 分给 tenant，但 owner 无 pending 时允许 steal；DWS++ 用 PEND_WALKS、ENQ_EPOCH、DIFF_THRES、QUEUE_THRES、is_stolen、epoch 控制偷取激进度；硬件状态约 192 bits（`papers/walk_stealing2021.txt:506-512, 514-563, 567-586`）。
- **适用假设与代价**：spatial multi-tenancy、共享 walker、固定最大 tenant 数；DWS 可能伤公平，DWS++ 以吞吐换公平；不提供安全隔离；模拟环境；HH workload 若 TLB/walker 本身不足，DWS 帮助有限（`papers/walk_stealing2021.txt:129-151, 567-586, 777-784, 877-915`）。
- **真实评估平台/负载/强基线**：GPGPU-Sim 增强 MASK；30 SMs、1024-entry L2 TLB、16 walkers、192-entry queue；45 个 workload；基线 Baseline、S-TLB、S-(TLB+PTW)、MASK、static partition。DWS 平均 total IPC +37%（32 个 VM-intensive workload +55%），weighted IPC +15%；DWS++ +34%；DWS 比 MASK 高 29%；static partition 比 baseline 差；2048 TLB+32 PTW 仍比 S-(TLB+PTW) 慢约 8%（`papers/walk_stealing2021.txt:222-226, 316-339, 611-669, 844-855, 877-888`）。
- **不能推出的结论**：不是 CPU/SoC IOMMU；没有真实 GPU；不提供安全/隔离；不能把“增加 TLB/PTW 无用”推广为所有架构；不能声称静态分区在所有 domain 都无效。
- **原文定位**：`papers/walk_stealing2021.txt:14-32, 90-151, 316-339, 389-405, 506-512, 611-669, 844-855, 877-915`。

### 卡 6：rIOMMU
- **题录/状态/URL**：*rIOMMU: Efficient IOMMU for I/O Devices that Employ Ring Buffers*，ASPLOS 2015（`papers/riommu2015.txt:1-6, 58-60`）。URL：`https://www.mulix.org/pubs/iommu/riommu-asplos-2015.pdf`（`source-manifest.json:126-136`）。
- **针对问题**：IOMMU 保护使高带宽 NIC/PCIe SSD 吞吐下降可达 10×；strict invalidation 昂贵，deferred 牺牲安全窗口（`papers/riommu2015.txt:11-35, 286-347`）。
- **核心机制**：per-ring flat page table；IOVA 是 flat table index；一个 ring 最多一个 IOTLB entry，每次新翻译隐式替换旧翻译；只在 burst 结束显式 invalidate；rIOTLB 预取下一个 rPTE；rDEVICE/rRING/rPTE/rIOVA 数据结构（`papers/riommu2015.txt:395-419, 421-463, 527-542`）。
- **适用假设与代价**：依赖 ring 语义和驱动按顺序 map/unmap；适用于 NIC/NVMe 等 ring 设备；不适用于 SATA AHCI、RDMA 或随机访问；I/O page fault 通常不发生；需要驱动修改；out-of-order access 只能退化为从 DRAM 取 translation；N≥L 否则 overflow（`papers/riommu2015.txt:553-605`）。
- **真实评估平台/负载/强基线**：Mellanox 40 Gbps / Broadcom 10 GbE；Netperf stream/RR、Apache 1KB/1MB、Memcached；基线 Linux strict、strict+、defer、defer+、no-IOMMU。相对 strict/strict+ 吞吐 1.00–7.56×；相对 no-IOMMU 0.77–1.00×；CPU 0.36–1.00×；RR 场景提升较小（`papers/riommu2015.txt:624-654, 790-803`）。
- **不能推出的结论**：不是通用 IOMMU；不是真实硬件实现（rIOMMU 部分模拟）；没有安全攻击模型；不能推广到 CPU MMU 或 GPU GMMU；不能声称所有非 ATS 设备只能存物理地址。
- **原文定位**：`papers/riommu2015.txt:11-35, 118-134, 395-419, 421-463, 527-542, 553-605, 624-654, 790-803`。

### 卡 7：Utilizing the IOMMU Scalably
- **题录/状态/URL**：*Utilizing the IOMMU Scalably*，source-manifest 标为 ATC 2015；本地提取未含会刊页眉，题录按 manifest 使用（`source-manifest.json:137-147`）。URL：`https://www.cs.tau.ac.il/~mad/publications/atc2015-iommu.pdf`。
- **针对问题**：高并发 map/unmap 下 IOMMU 管理 subsystem 不 scalable；IOVA allocator 全局锁和 IOTLB invalidation lock 成为瓶颈；16 核 netperf RR 吞吐停滞（`papers/iommu_scalable2015.txt:18-48, 100-124, 207-213`）。
- **核心机制**：三种 scalable IOVA assignment：dynamic identity mapping、IOVA-kmalloc、per-core magazines；scalable IOTLB invalidation：per-core deferred flush queue，取代 global flush queue（`papers/iommu_scalable2015.txt:415-482, 540-552, 574-605`）。
- **适用假设与代价**：Linux/Intel x86；高吞吐 NIC；identity mapping 依赖 buffer 物理连续；48-bit IOVA/46-bit PA 空余位区分权限；>64GB 需 GFP_DMA 避免 collision；dynamic identity 要 atomic PTE refcount 和 alias 处理，page table 内存可能增长；IOVA-kmalloc 最简单但 page table blowup；magazines 多 1.2× page table 内存；deferred invalidation 有安全窗口；strict invalidation 与 scalability 不兼容（`papers/iommu_scalable2015.txt:253-268, 293-308, 388-408, 426-432, 494-523, 785-819`）。
- **真实评估平台/负载/强基线**：Linux 3.17.2，16 核 Xeon E5-2630 v3，Broadcom 10 Gb/s NIC；270 个 netperf TCP RR、memcached；基线 stock Linux、EiovaR。设计达 no-IOMMU 的 88.5%–100%；16 核 RR 为 no-IOMMU 的 93%–94%，EiovaR 仅 23%；strict invalidation 约 1/6 TPS；IOVA-kmalloc 性能最好但 PT 内存近似线性增长（`papers/iommu_scalable2015.txt:697-704, 742-783, 785-819`）。
- **不能推出的结论**：不是硬件 IOMMU 翻译优化；不是设备侧 ATS；不提供安全隔离；不能直接用于 RISC-V IOMMU，除非确认其 IOVA 语义、invalidation queue、page table 格式和 Linux driver 支持相同假设。
- **原文定位**：`papers/iommu_scalable2015.txt:18-48, 100-124, 207-213, 253-268, 293-308, 388-408, 415-482, 540-552, 574-605, 697-704, 785-819`。

### 卡 8：Overcoming the IOTLB wall for multi-100-Gbps Linux-based networking
- **题录/状态/URL**：PeerJ Computer Science 2023，DOI 10.7717/peerj-cs.1385；Submitted 2022-11-04，Accepted 2023-04-17，Published 2023-05-16（`papers/iotlb_wall2023.txt:1-9, 35-37`）。URL：`https://dejankostic.com/documents/publications/iotlb-peerj23.pdf`（`source-manifest.json:148-158`）。
- **针对问题**：200 Gbps 下 IOTLB miss 造成吞吐下降（最高 20%），且不是 CPU-bound；传统工作聚焦 IOVA/invalidation CPU 开销，忽视数据路径 IOTLB miss（`papers/iotlb_wall2023.txt:11-25, 83-105, 336-350`）。
- **核心机制**：先 characterize IOTLB misses/MiB；发现减少少量 miss 即可把 wall 推向更高数据率；提出 HPA hugepage-aware allocator，用 2 MiB IOMMU mapping；LRO/TSO 减少 translation；buffer recycling 减少 shuffling（`papers/iotlb_wall2023.txt:120-127, 774-789, 915-930`）。
- **适用假设与代价**：Linux + Intel IceLake/AMD EPYC + NVIDIA/Mellanox NIC；IOTLB 硬件不可改；hugepage 有 memory stranding/fragmentation 风险；HPA 起初只处理 RX；leaky driver 会耗尽 pool；大 mapping 有安全顾虑；buffer shuffling 不能完全消除；不保证消除所有 miss（`papers/iotlb_wall2023.txt:792-845, 886-912, 1007-1022`）。
- **真实评估平台/负载/强基线**：200 Gbps iPerf TCP、Memcached、DPDK FastClick；基线 no-IOMMU、IOMMU 4KB、LRO/TSO 开关。2 MiB mapping 基本恢复吞吐；misses/MiB 从约 420 降到约 360 可显著推迟 drop；2 MiB 减少 miss 可达约 10×；每 4KB page allocation 成本相近（`papers/iotlb_wall2023.txt:474-550, 915-930, 946-957`）。
- **不能推出的结论**：不是新 IOMMU 硬件；不是多租户隔离；不是 GPU/SoC IOMMU；不能推广到所有速率；ATS 只提到 Intel IceLake 仅 1 GiB entry caching via ATS（`papers/iotlb_wall2023.txt:176-181`）；不能声称 hugepage 是普遍安全方案。
- **原文定位**：`papers/iotlb_wall2023.txt:11-25, 83-105, 176-181, 202-230, 336-350, 474-550, 774-789, 915-930, 1007-1022`。

### 卡 9：Beyond the Bermuda Triangle of Contention: IOMMU Interference in Mixed Criticality Systems
- **题录/状态/URL**：arXiv:2508.19670v1 [cs.DC]，2025-08-27；本地文本未确认正式会刊，不猜会议（`papers/bermuda2025.txt:12`）。URL：`https://arxiv.org/pdf/2508.19670`（`source-manifest.json:159-169`）。
- **针对问题**：混合关键性系统中 IOMMU 作为共享翻译结构，其 contention 对 timing predictability 的影响被忽视；此前多聚焦 LLC、bus、main memory（“Bermuda Triangle”）和 IOMMU 侧信道（`papers/bermuda2025.txt:10-44, 48-120`）。
- **核心机制**：在 Xilinx UltraScale+ ZCU104（Arm SMMUv2 / CoreLink MMU-500）上，用 FPGA DMA 微基准 + FPD-DMA 干扰 + Bao hypervisor + SMMU PMU + ILA，反向工程 micro-TLB 深度，测 TBU/TCU 竞争（`papers/bermuda2025.txt:169-176, 227-260, 398-420`）。
- **适用假设与代价**：单板 ZCU104；decentralized TBU/IOTLB；micro-TLB 64 entries；macro-TLB 深度按 SMMUv2 spec 2000；4KB page；16B payload；不使用 superpages；作者假设跨架构相似但明确未验证；没有评估 mitigation（`papers/bermuda2025.txt:229-260, 435-446, 642-653`）。
- **真实评估平台/负载/强基线**：真实 ZCU104；SMMU disabled/enabled；solo/interf_tbu/interf_tcu；100/150/300 MHz；基线 solo、SMMU disabled。SMMU enabled 下干扰使平均执行时间约 1.47×，最大 1.79×；16B payload 吞吐下降约 32%；1KB 约 10%；4KB 收敛；大 payload 影响被内存延迟稀释（`papers/bermuda2025.txt:499-558, 570-590, 614-633`）。
- **不能推出的结论**：不能从单板宣布所有 IOMMU 都没有隔离/可预测方案；不能推广到 RISC-V IOMMU、Intel VT-d、AMD-Vi 或 GPU GMMU；不能证明跨架构假设；不能把平均延迟和最大延迟混为一谈；未评估 mitigation。
- **原文定位**：`papers/bermuda2025.txt:10-44, 83-120, 169-176, 227-260, 398-420, 499-558, 570-590, 614-633, 642-653`。

### 卡 10：ShareMMU
- **题录/状态/URL**：*ShareMMU: Supporting Secure Address Translation Sharing among Untrusted Accelerators*，MICRO 2026 camera-ready；本地文本致谢匿名 MICRO 2026 reviewers（`papers/sharemmu2026.txt:1-11, 1001-1003`）。URL：`https://d1qx31qr3h6wln.cloudfront.net/publications/ShareMMU_MICRO26_Camera_Ready.pdf`（`source-manifest.json:276-286`）。**状态**：作者 camera-ready；商用/独立复现未确认。
- **针对问题**：多加速器系统共享 TLB 能提升性能，但不可信加速器可能忽略 TLB shootdown、使用 stale translation 或伪造物理地址；shared TLB 还暴露侧信道；permission change 频繁（`papers/sharemmu2026.txt:15-38, 80-125, 249-276, 311-348`）。
- **核心机制**：Permission Tracker（Secure Vector：PPN + Access Mask + VPN + AA-LRU + Alias Table）；inclusive of private TLBs；eviction 时 back-invalidate；验证每个 pre-translated access；ShareMMU-SP 用 QARTA IDF 做 crypto-randomization + crypto-partition 保护 shared TLB，context switch 时 flush Permission Tracker（`papers/sharemmu2026.txt:368-415, 443-467, 531-605, 632-668`）。
- **适用假设与代价**：shared page table；private TLB 不可信；shared TLB、IOMMU、OS 可信；ATS/pre-translated address；加速器数量有上限；Permission Tracker 256-entry 16-way ≈3.25KB SRAM；context switch 开销最高 4.1%（LPCIP），几何平均 0.82%；NoC 额外 traffic 3.23%；侧信道概率模型；TEE-aware 设计留 future work（`papers/sharemmu2026.txt:326-348, 368-415, 594-605, 632-668, 947-972`）。
- **真实评估平台/负载/强基线**：gem5 PARADE；8-issue x86 OoO 2GHz，2 进程 5ms context switch；16 accelerators；PARADE 九个 workload；基线 unsecured ATS、Border Control、CryptoMMU。ShareMMU 平均 slowdown 1.66%，ShareMMU-SP 2.82%；Border Control 32.19%，CryptoMMU 17.21%；随 accelerator 数 4→32，ShareMMU 约 2.03%→2.50%，ShareMMU-SP 3.17%→2.93%（`papers/sharemmu2026.txt:670-708, 725-740, 776-799`）。
- **不能推出的结论**：没有真实硅验证；没有独立复现；不是商用结论；安全概率分析依赖模型；不能推广到所有 ATS/IOMMU 实现；没有完整 TEE 设计；不能据此说所有 shared TLB 都不安全或所有加速器共享都需硬件 Permission Tracker。
- **原文定位**：`papers/sharemmu2026.txt:15-38, 80-125, 114-125, 249-276, 311-348, 368-415, 443-467, 531-605, 632-708, 725-740, 776-799, 947-972`。

---

## 3. 跨论文对照：哪些问题已有成熟机制、哪些只在特定条件下解决、哪些证据仍受限

| 主题 | 已有成熟机制 | 只在特定条件下解决 | 公开证据仍受限 | 迁移到别处需要什么 |
|---|---|---|---|---|
| CPU LLC 容量分区 | UCP utility monitor + way partitioning；KPart clustering + DynaWay（`ucp2006.txt:100-118, 378-429`；`kpart2018.txt:303-338, 504-533`） | 需 way-partitionable LLC、miss curve/profiling、Intel CAT 类硬件；KPart 无 strict isolation（`kpart2018.txt:101-105`） | UCP 是模拟；KPart 真实但只覆盖 LLC，不覆盖 IOMMU/IOTLB；Vantage 是模拟对手 | 迁移到 IOMMU 需要 per-device miss curve、可分区 IOTLB/PTW 或可编程替换策略；DMA 与 CPU cache 语义不同 |
| GPU/IOMMU PTW 调度 | walk_scheduling 的 SIMT-aware scheduler；MASK 的 TLB token/bypass/DRAM 调度（`walk_scheduling2018.txt:414-489`；`mask2018.txt:828-939`） | 需 GPU SIMT、共享 L2 TLB/PTW、IOMMU buffer、可区分 instruction/page walk 请求；收益随 TLB/walker 增大而变小（`walk_scheduling2018.txt:772-795`） | 全部为模拟（gem5/Mosaic/GPGPU-Sim）；无真实 GPU 多应用实验；不涉及安全 | 迁移到 RISC-V/SoC IOMMU 需确认是否有 multiple PTW、是否可带 requester/stream 标识、是否有足够并发请求可调度 |
| 多租户 walker 共享/stealing | DWS/DWS++ 动态 walk stealing；S-(TLB+PTW) 作为 headroom 上界（`walk_stealing2021.txt:90-151, 611-669`） | 需 spatial multi-tenancy、共享 walker、固定 tenant 数；DWS 可伤公平，DWS++ 需调参；HH 资源不足时帮助有限（`walk_stealing2021.txt:777-784, 877-915`） | 模拟；无真实 GPU；无安全隔离；未覆盖 CPU/SoC IOMMU | 需确认 RISC-V IOMMU 是否共享 PTW、是否支持 per-tenant 队列/优先级、是否有可信 tenant 标识 |
| 软件 IOVA / invalidation 管理 | rIOMMU ring flat table + one-entry-per-ring；iommu_scalable identity/kmalloc/magazines/per-core deferred invalidation（`riommu2015.txt:395-419, 527-542`；`iommu_scalable2015.txt:415-482, 574-605`） | 需 ring 顺序或 Linux x86/identity mapping；strict invalidation 与 scalability 冲突；deferred 有安全窗口；kmalloc 可能 PT blowup（`iommu_scalable2015.txt:785-819`） | rIOMMU 部分模拟；iommu_scalable 是真实 Linux 但只覆盖 10 GbE/特定 workload；不是设备侧 ATS；不是安全方案 | 迁移到 RISC-V IOMMU/Linux 需核对 IOVA 格式、invalidation queue、页表内存回收、driver API；不能假设 one-entry-per-ring 合法 |
| IOTLB wall / 数据路径 | hugepage IOMMU mapping + HPA；LRO/TSO；buffer recycling（`iotlb_wall2023.txt:774-789, 915-930`） | 需 200 Gbps NIC、Linux、可控 hugepage 内存、RX 路径；hugepage 不是 panacea；leaky driver 会耗尽 pool（`iotlb_wall2023.txt:1007-1022`） | 真实 200 Gbps 实验但平台是 Intel/AMD + Mellanox；不是多租户；不是 GPU/SoC；不是硬件 IOMMU 修改 | 低速率平台只能验证小 payload 翻译开销/缓冲复用原则，不能声称复现 IOTLB wall；需 200Gbps 级 link 或等价 microbenchmark |
| 混合关键性 IOMMU 干扰 | bermuda 的 microbenchmark + PMU/ILA 反向工程；提出 QoS/stream-ID、page coloring 作为未来方向（`bermuda2025.txt:398-420, 642-677`） | 单板 ZCU104/Arm SMMUv2；主要影响小 payload；跨架构相似是假设；mitigation 未评估 | 单一真实板；无多平台；无 RISC-V/Intel/AMD 对比；无形式化 WCET 分析 | 迁移到 RISC-V IOMMU 需先确认 IOTLB/TBU 组织、PMU 可观测事件、是否可做 DMA 干扰实验；不能从单板宣布全领域无隔离 |
| 不可信加速器安全共享 | ShareMMU Permission Tracker + back-invalidation；Border Control、CryptoMMU 作为对手（`sharemmu2026.txt:368-415, 443-467`） | 需 ATS/pre-translated address、untrusted private TLB、shared page table、bounded accelerators；ShareMMU-SP 针对 shared TLB 侧信道（`sharemmu2026.txt:311-348, 632-668`） | 模拟；无真实硅；无独立复现；安全分析依赖概率模型；TEE 留 future work | 当前 CVA6/iDMA/RISC-V 平台若无第三方不可信 ATS 加速器，不能验证 ShareMMU 威胁模型；需先确认 ATS/PASID、device-side IOTLB 和 shootdown 语义 |

**联系与区别**：
- **联系**：UCP/KPart、MASK/walk_scheduling/walk_stealing、rIOMMU/iommu_scalable、iotlb_wall、bermuda/ShareMMU 都在处理“共享结构上的竞争/分配/同步”，都区分了容量、策略和控制面；都显示“加资源”不等于“消除干扰”。
- **区别**：
  - CPU LLC 分区（UCP/KPart）是 **容量+置换策略**；IOMMU page walk 调度/stealing 是 **请求排序/队列调度**；软件 IOVA 管理是 **控制面与同步**；IOTLB wall 是 **数据路径 cache miss**；bermuda 是 **时间可预测性**；ShareMMU 是 **安全访问控制与共享翻译**。
  - UCP/KPart 的 utility/miss curve 语义不能直接套到 IOTLB；IOTLB 没有“每个设备一条完整 LRU stack”这种天然模型，且 DMA 请求不可重启（`riommu2015.txt:188-199`）。
  - GPU GMMU 的 SIMT/wavefront 语义不能直接套到 CPU MMU 或 SoC IOMMU；walk_scheduling/walk_stealing 的收益依赖 SIMT 批量进度。
  - SoC IOMMU（Arm SMMUv2）的 TBU/TCU 分布式 cache 结构与 Intel VT-d/AMD-Vi 集中式 IOTLB 不同（`bermuda2025.txt:145-167`），不能互相替代结论。
  - PCIe ATS 让设备侧缓存 translation（`iotlb_wall2023.txt:176-181`；`sharemmu2026.txt:80-103`），非 ATS 设备仍需每 DMA 经 IOMMU 翻译；不能把 ShareMMU 的 ATS 假设套到非 ATS 设备。
  - remote application indirect table / 应用层地址表遍历不同于硬件 OS 页表；Tiara 等未在本批文本中，不能混入本线结论。

---

## 4. 真正的矛盾/瓶颈（3–5 个，每个至少两种原始证据）

### 矛盾 1：静态容量分区 vs 动态共享——隔离、利用率、关联度不能同时最优
- **证据 A（UCP）**：Half-and-Half 静态等分有性能隔离优势，但不能响应需求变化；UCP 动态按 utility 分区平均 +11%，但每 5M cycles 才调整一次，对 fine-grained phase 反而不如 LRU，且全局 uniform partition 牺牲 per-set 控制（`papers/ucp2006.txt:492-499, 946-955`）。
- **证据 B（KPart）**：真实 Broadwell-D 上，per-application way partition（NoClust）平均只 +1.7%，伤害 30% mix；原因是每 partition 至少 1 way、小 partition associativity 低；KPart 通过 clustering 让多个应用共享 partition，平均 +24%，但 cluster 内无 strict isolation（`papers/kpart2018.txt:11-36, 49-58, 314-323, 690-700, 101-105`）。
- **证据 C（walk_stealing）**：静态 partition walker 会导致 under-utilization 和 load imbalance；DWS 在 owner 无 pending 时偷 walk，平均 +37% total IPC；但 DWS 可伤公平，DWS++ 需要牺牲部分吞吐（`papers/walk_stealing2021.txt:316-339, 389-405, 611-669, 878-888`）。
- **作者实测事实**：UCP/KPart 是各自平台上的 simulation/real-system 测量；walk_stealing 是模拟。
- **作者解释**：静态分区忽略了应用/租户的动态需求和收益差异；粗粒度 way partition 降低 associativity；静态 walker 分区降低利用率。
- **我的综合推断**：在 IOMMU 场景中，“隔离”至少分三层：容量隔离（IOTLB/PTW entry 配额）、时序隔离（请求优先级/队列）、故障/安全隔离（权限和 translation 验证）。静态容量配额外加动态共享策略是常见组合，但不能把“某个静态方案在某平台伤性能”推广为“所有共享方案都没有隔离”。
- **未验证机会**：跨层联合控制（LLC partition + IOTLB partition + walker scheduling + permission tracker）在真实 RISC-V/SoC IOMMU 上如何权衡；缺少形式化隔离保证。

### 矛盾 2：增加 TLB/PTW 容量不消除干扰，调度/分区可改善但有上限
- **证据 A（walk_stealing）**：把 TLB 翻倍到 2048-entry、PTW 翻倍到 32，仍比 S-(TLB+PTW)（同样资源但不共享）慢约 8%；作者称 interference 是 key performance limiter；增加 PTW 可能 thrash L2 TLB，增加 TLB 增加面积/能耗/lookup latency（`papers/walk_stealing2021.txt:316-339, 877-888`）。
- **证据 B（walk_scheduling）**：SIMT-aware scheduler 在 512 L2 TLB+8 PTW 下平均 +30%；换成 1024 L2 TLB+8 PTW 时 +25%；512 L2 TLB+16 PTW 时 +8.4%；1024+16 时仅 +5.3%。资源越足，调度收益越小，但仍存在（`papers/walk_scheduling2018.txt:772-795`）。
- **证据 C（MASK）**：MASK 在 8192-entry shared L2 TLB 时与 SharedTLB 持平，说明 capacity 足够可消除部分竞争；但在较小 TLB 下 token/bypass/DRAM 调度带来 57.8% speedup，说明瓶颈不只是容量（`papers/mask2018.txt:1109-1137, 1177-1184`）。
- **作者实测事实**：以上均为模拟；MASK/walk_stealing 使用不同 simulator 和 baseline。
- **作者解释**：共享结构上的 interleaving 和 uncontrolled contention 是独立于容量的性能限制。
- **我的综合推断**：IOTLB miss 数与 PTW 数不能等同；容量、并发 walker 数、PWC 命中、请求排序、L2/DRAM 优先级共同决定结果。增加容量可能把瓶颈从 TLB 移到内存仲裁或调度。
- **未验证机会**：真实 GPU/IOMMU 上同时控制 TLB 容量、PTW 数、PWC 大小和调度策略的联合设计；需要区分 hit rate、PTW latency、throughput。

### 矛盾 3：invalidation/permission update 的合法语义与代价必须分开
- **证据 A（rIOMMU）**：ring 语义下 IOVA 顺序可预测，flat table + one IOTLB entry per ring 使每次新翻译隐式替换旧翻译，只在 burst 结束显式 invalidate；这不等同于所有 IOMMU 都需全局停机，也不表示可以随意省略 invalidation（`papers/riommu2015.txt:127-134, 527-542`）。
- **证据 B（iommu_scalable）**：Linux deferred invalidation 每 10ms 或 250 个请求批量全局 flush，保持 correctness 但制造安全窗口；strict invalidation 每个 unmap 等待完成，16 核时只有约 1/6 TPS，作者称 strict invalidation 与 scalable implementation 不兼容（`papers/iommu_scalable2015.txt:293-308, 785-819`）。
- **证据 C（ShareMMU）**：Permission Tracker inclusive of private TLBs；permission update 用现有 TLB shootdown + targeted back-invalidation；不可信加速器若忽略 shootdown，其 stale translation 不在 Permission Tracker 中，会被拒绝；Secure Vector 可只清某 accelerator 的 permission bit 而不 flush 整个 entry（`papers/sharemmu2026.txt:419-455, 531-575`）。
- **作者实测事实**：rIOMMU 的 invalidation 成本来自实测 Linux strict/defer 模式；iommu_scalable 是真实 Linux 测量；ShareMMU 是模拟。
- **作者解释**：invalidation 成本可通过批处理、per-core queue、ring 语义、back-invalidation 摊销；安全窗口是 trade-off。
- **我的综合推断**：合法实现至少有三类：立即/严格 invalidation、延迟/批量 invalidation、基于权限跟踪的按需验证。逐页 in-flight 计数、全局停机、非 ATS 只能物理地址都不是标准必然。
- **未验证机会**：RISC-V IOMMU 的 invalidation 队列与 ATS/PASID 语义组合；如何在保持安全窗口可界定前提下做 scalable invalidation。

### 矛盾 4：软件管理能大幅恢复性能，但目标（性能/安全/多租户）不同，边界不同
- **证据 A（iommu_scalable）**：纯软件重新设计 IOVA allocation 和 IOTLB invalidation，在 16 核 netperf RR 上达 no-IOMMU 的 93%–94%，而 EiovaR 只有 23%；但 strict invalidation 仍不 scalable，kmalloc 有 PT blowup（`papers/iommu_scalable2015.txt:41-48, 697-704, 742-783, 785-819`）。
- **证据 B（iotlb_wall）**：数据路径 IOTLB wall 可通过软件 HPA + 2 MiB mapping 恢复最多 20% 吞吐下降；但作者明确说 hugepage 不是 panacea，链路更快需要更大 hugepage 或 IOMMU 架构变化（`papers/iotlb_wall2023.txt:20-25, 915-930, 1007-1013`）。
- **证据 C（ShareMMU）**：为了不可信加速器安全共享 translation，ShareMMU 引入硬件 Permission Tracker；纯软件 Border Control 因 protection table 同步开销平均慢 32.19%，CryptoMMU 慢 17.21%，而 ShareMMU 1.66%（`papers/sharemmu2026.txt:80-103, 249-276, 776-799`）。
- **证据 D（rIOMMU）**：rIOMMU 是硬件结构重设计（flat table、one IOTLB entry per ring），但评估用软件模拟 rIOMMU 的 invalidation 成本；说明性能修复可能落在硬件/软件分界的不同侧（`papers/riommu2015.txt:118-134, 624-654`）。
- **作者实测事实**：iommu_scalable 真实系统；iotlb_wall 真实 200Gbps；ShareMMU 模拟；rIOMMU 混合。
- **作者解释**：性能瓶颈在控制面（IOVA/invalidation）和数据路径（IOTLB miss）之间移动；安全目标需要额外验证结构。
- **我的综合推断**：不能把“软件能修 IOTLB wall”外推为“软件能实现不可信加速器安全共享”；也不能把“硬件 Permission Tracker 低开销”外推为所有平台需要它。目标函数不同。
- **未验证机会**：软件/硬件协同的 IOMMU 管理框架，同时满足性能、安全窗口可控和多租户隔离；在 RISC-V IOMMU 上验证。

### 矛盾 5：单板/模拟证据不能宣布全领域“没有隔离方案”
- **证据 A（bermuda）**：在单块 Xilinx UltraScale+ ZCU104 / Arm SMMUv2 上，IOMMU 竞争使小 payload 最大延迟 1.79×；作者明确假设跨架构相似但“further research is needed to confirm this”，且 mitigation 未评估（`papers/bermuda2025.txt:40-44, 642-653, 654-677`）。
- **证据 B（walk_stealing）**：GPGPU-Sim 模拟显示 DWS 可 +37% total IPC；但模拟 baseline、无真实 GPU、无安全模型（`papers/walk_stealing2021.txt:222-226, 611-669, 940-954`）。
- **证据 C（ShareMMU）**：gem5 PARADE 模拟显示 ShareMMU 1.66% slowdown，但无真实硅、无独立复现；作者称是 MICRO 2026 camera-ready，不代表商用（`papers/sharemmu2026.txt:15-38, 776-799, 999-1008`）。
- **证据 D（KPart）**：真实 Intel Broadwell-D CPU LLC 上 KPart 有效，但这是 CPU cache，不是 IOMMU；不能据此说 IOMMU 隔离已解决（`papers/kpart2018.txt:70-72, 307-323`）。
- **作者实测事实**：bermuda 是真实板；KPart 是真实机器；walk_stealing/ShareMMU 是模拟；UCP 是模拟。
- **作者解释**：各工作只声称其评估范围内的结论。
- **我的综合推断**：领域现状应表述为：在特定平台/特定负载/特定共享结构上，已证明 contention 和性能损失，并已有若干机制族；但不存在一个已验证的跨架构、跨设备、跨安全域的通用隔离结论。不能从单板观察宣布全领域没有隔离方案。
- **未验证机会**：多架构对照（RISC-V IOMMU、Arm SMMUv3、Intel VT-d、AMD-Vi、GPU GMMU、PCIe ATS）；真实硬件上的 walk stealing / permission tracker；形式化 timing/security 保证。

---

## 5. 最容易误判为创新的普通方案、强对手、仍需补齐的经典或前沿分支

### 5.1 容易误判为创新的普通方案
- **静态 way partition / Half-and-Half**：UCP 和 KPart 都显示静态等分或 per-app 分区有隔离优点但利用率/关联度差；walk_stealing 显示静态 walker 分区 under-utilization。普通方案若只做静态配额，必须证明比 KPart/DWS 类动态共享更好。
- **简单 deferred invalidate**：rIOMMU 和 iommu_scalable 已系统分析；仅“延迟失效”不新，关键是是否保持语义、安全窗口是否可控、是否 scalable。
- **简单 hugepage**：iotlb_wall 显示 2 MiB mapping 可恢复吞吐，但作者明确不是 panacea；bermuda 还指出 superpage 会妨碍 page coloring；因此“用 hugepage 解决 IOTLB”不是充分创新。
- **简单增加 TLB/PTW 容量**：walk_stealing 和 walk_scheduling 都显示干扰/调度是独立瓶颈。
- **简单 random/FCFS 调度 PTW**：walk_scheduling 显示 random 可慢 26%，FCFS 不是最优；但提出新调度需与这些 baseline 比较。
- **“加自适应/智能/端到端”**：本批论文中，自适应/聚类/调参都有明确目标函数（miss utility、speedup、total IPC、fairness、PTW work、IOTLB miss/MiB）。没有目标函数和可测约束的“智能”不算证据。

### 5.2 强对手
- **CPU LLC**：UCP Lookahead、KPart Kbest/Kauto、Vantage（fine-grained partitioning）、NoPart/NoClust、LRU、Half-and-Half（`ucp2006.txt:378-429`；`kpart2018.txt:1416-1435`）。
- **GPU/IOMMU**：MASK、SharedTLB、PWCache、Ideal、FCFS、random、S-(TLB+PTW)、static partition（`mask2018.txt:944-974`；`walk_scheduling2018.txt:240-279`；`walk_stealing2021.txt:316-339, 844-855`）。
- **I/O 软件管理**：Linux strict/strict+、defer/defer+、EiovaR、stock Linux、no-IOMMU（`riommu2015.txt:790-803`；`iommu_scalable2015.txt:697-704`）。
- **IOTLB 数据路径**：no-IOMMU、IOMMU 4KB、LRO/TSO、DPDK FastClick（`iotlb_wall2023.txt:474-550, 774-789, 1131-1159`）。
- **安全共享**：unsecured ATS、Border Control、CryptoMMU（`sharemmu2026.txt:80-103, 776-799`）。
- **混合关键性**：SMMU disabled、solo、interf_tbu、interf_tcu；以及未来 QoS/stream-ID、page coloring（`bermuda2025.txt:499-558, 642-677`）。

### 5.3 仍需补齐的经典/前沿分支
- **RISC-V IOMMU 实证**：bermuda 引用 RISC-V IOMMU open-source implementation（`bermuda2025.txt:150-152`），但没有实验；ShareMMU/walk_stealing 不是 RISC-V。当前平台是 RISC-V IOMMU，必须补 IOTLB/TBU 结构、PMU、invalidation 队列、ATS/PASID 能力。
- **PCIe ATS / PASID / device-side TLB shootdown**：iotlb_wall 提到 ATS 仅 IceLake 1 GiB entry caching（`iotlb_wall2023.txt:176-181`）；ShareMMU 假设 ATS pre-translation，但没有真实 ATS 行为验证。非 ATS 设备与 ATS 设备的结论必须分开。
- **CXL / PCIe 5.0 / 多加速器 SoC 的 IOMMU 干扰与侧信道**：bermuda 在 future work 提到 CXL/PCIe 5.0（`bermuda2025.txt:700-706`），但未实验。
- **Nested translation / 两阶段 IOMMU / 虚拟化**：iommu_scalable 提到 nested IOMMU 但聚焦 intra-OS（`iommu_scalable2015.txt:157-176`）；ShareMMU 不做 nested。两阶段翻译、虚拟 IOMMU、coIOMMU 等未在本批文本中。
- **I/O page fault 与不可重启 DMA**：rIOMMU 明确指出 DMA 通常不可重启，I/O page fault 标准化但未广泛支持（`riommu2015.txt:188-199`）；MASK 把 page fault 留 future（`mask2018.txt:888-894`）。
- **形式化内存一致性/页表更新可见性**：rIOMMU 和 iommu_scalable 都涉及 memory barrier/cacheline flush（`riommu2015.txt:516-522`；`iommu_scalable2015.txt:328-334`），但没有完整形式化模型。
- **多租户 + 安全 + 时间可预测性的联合设计**：ShareMMU 做安全共享，bermuda 做时序干扰，walk_stealing 做多租户性能；三者未联合评估。
- **真实硬件 walk stealing / PTW scheduling**：目前是模拟；需真实 GPU 或 RISC-V IOMMU 验证。
- **大 IOTLB mapping 的安全语义**：iotlb_wall 指出 hugepage mapping 扩大设备可访问域（`iotlb_wall2023.txt:1014-1022`），与 ShareMMU 的细粒度权限跟踪存在张力。

---

## 6. 对当前平台（CVA6 + 四 iDMA + RISC-V IOMMU + Linux/Genesys2 FPGA）的适用范围

**该平台能支持的**：
- **软件 IOVA 管理开销与控制面研究**：如果 Linux RISC-V IOMMU driver 提供 map/unmap、IOVA allocator、invalidation queue，可对照 iommu_scalable 的瓶颈分类（IOVA allocation、IOTLB invalidation、page table management）做小规模测量；但必须核实其 API 和语义。
- **小 DMA 事务的翻译开销/干扰**：iDMA 可产生小 payload、连续/非连续访问；可借鉴 bermuda 的微基准方法，测量 IOMMU enabled/disabled、单设备/多设备干扰下的 DMA latency。但这是小规模 FPGA 平台，不能复现 200 Gbps IOTLB wall，也不能直接外推 SMMUv2 TBU/TCU 结论。
- **ring-like 批量 invalidation 的可行性验证**：如果 iDMA 驱动和 RISC-V IOMMU 支持类似 per-ring/flat table 或批量 invalidation，可验证 rIOMMU 思想的一部分；但 RISC-V IOMMU 是否支持 one IOTLB entry per ring、是否允许 flat table 替代 hierarchical table，必须先查规范，不能假设。
- **共享资源干预实验**：同 BIT 的 S/N/P/PN 干预可作为“静态配额/策略切换”的工程验证；但 Pn 配置一方改善一方受损，根因和泛化性未确立，不能当作论文级 IOMMU 隔离结论。
- **功能验收**：172/172 功能验收可证明平台功能正确性，但不能证明 IOMMU 性能、隔离、安全或论文创新。

**该平台不能直接支持的**：
- **GPU 多租户 walk stealing / SIMT-aware PTW scheduling**：没有 GPU SIMT wavefront、没有共享 GPU L2 TLB；walk_scheduling/walk_stealing 的负载和调度语义不匹配。
- **200 Gbps IOTLB wall**：没有 100/200 Gbps NIC 和对应 PCIe 带宽，不能复现 iotlb_wall 的 wall 位置和 hugepage 恢复效果。
- **不可信第三方加速器 ATS 安全共享**：平台没有 PCIe ATS/PASID、device-side TLB、untrusted accelerator 模型；ShareMMU 的威胁模型不能直接验证。
- **混合关键性 hypervisor**：若平台运行 Linux 而非 Bao/静态分区 hypervisor，bermuda 的 MCS 干扰实验需要重新搭建；即使搭建，单板也不能推广。
- **CPU LLC UCP/KPart**：CVA6 是否有可 way-partition 的 shared LLC、是否支持 Intel CAT 类接口未知；不能把 KPart 直接作为平台方案。
- **“全领域无隔离方案”**：绝不可由本平台干涉观察推出。

**结论**：当前平台最适合验证“软件/控制面 + 小规模 DMA 数据路径”的 IOMMU 工程行为，以及作为 RISC-V IOMMU 的早期实证平台；不适合直接验证 GPU 多租户、200Gbps IOTLB wall、ATS 不可信加速器安全共享。任何从该平台到其他领域的迁移，都必须先满足对应论文的硬件/负载/语义条件。

---

## 7. 最必要精读的 3–5 篇与阅读顺序

按“先语义、后机制、再边界”的顺序：

1. **UCP 2006**（`papers/ucp2006.txt`）：先把 cache utility、miss curve、way partitioning、DSS、Lookahead 的经典语义读透；这是后续 KPart 和所有“utility 式共享资源分配”的起点。
2. **KPart 2018**（`papers/kpart2018.txt`）：看真实商用 way-partitioning 如何打破 UCP 假设，以及 clustering + DynaWay 如何补洞；重点读 NoClust 伤害 30% mix、Kauto、Vantage 对比。
3. **walk_scheduling 2018 + walk_stealing 2021**（`papers/walk_scheduling2018.txt`，`papers/walk_stealing2021.txt`）：先读单应用 irregular GPU 的 PTW 调度，再读多租户 walk stealing；两者合起来覆盖“排序 vs 分区 vs 偷取”的完整图谱。注意都是模拟。
4. **iommu_scalable 2015 + rIOMMU 2015**（`papers/iommu_scalable2015.txt`，`papers/riommu2015.txt`）：前者覆盖通用 Linux IOVA/invalidation scalability，后者覆盖 ring 特定 flat table + one-entry-per-ring；对照读，理解软件控制面 vs 硬件/设备语义的边界。
5. **iotlb_wall 2023**（`papers/iotlb_wall2023.txt`）：数据路径 IOTLB wall 的真实 200Gbps 证据，理解 hugepage、LRO/TSO、buffer recycling 的适用条件和“不是 panacea”。
6. **bermuda 2025 + ShareMMU 2026**（`papers/bermuda2025.txt`，`papers/sharemmu2026.txt`）：作为前沿边界阅读，分别代表“混合关键性时序干扰”和“不可信加速器安全共享”；注意 bermuda 单板、ShareMMU 模拟且 camera-ready，不等于商用/独立复现。

若要压缩到 5 组，可将第 3 组、第 4 组、第 6 组各视为一组“成对精读”。这样顺序仍是：UCP → KPart → GPU PTW 调度/偷取 → 软件 IOVA/invalidation → IOTLB wall → MCS/安全共享。

---

## 8. 最终要点

- **不要混层**：CPU MMU、GPU GMMU、SoC IOMMU、PCIe ATS、远程应用间接表不是同一东西；UCP/KPart 的 LLC 语义不能直接套 IOTLB；GPU SIMT walk scheduling 不能直接套 RISC-V IOMMU。
- **不要混指标**：TLB hit rate、IOTLB miss/MiB、PTW 时延、DMA latency、吞吐、加权 IPC、公平性、尾延迟、WCET 各自独立；TLB miss 不等于完整 page walk。
- **不要混语义与代价**：invalidation/permission update 的合法语义（必须保持 correctness/security）与代价（等待、flush、锁、shootdown）分开；合法实现至少有严格、延迟批量、权限跟踪三类。
- **不要用单板/模拟宣布全领域结论**：bermuda 单板、walk_stealing/sharemmu 模拟、KPart 真实但 CPU LLC、iotlb_wall 真实但 200Gbps NIC；没有跨架构统一结论。
- **已有成熟机制**：CPU LLC utility/clustering、GPU TLB token/bypass、PTW 调度/stealing、ring flat table、scalable IOVA/invalidation、hugepage IOTLB mapping、Permission Tracker。
- **仍受限**：真实硬件 walk stealing、RISC-V IOMMU IOTLB/invalidation 实证、ATS/PASID 安全语义、CXL/PCIe5 干扰与侧信道、混合关键性形式化 WCET、软件/硬件联合隔离保证。
- **当前平台**：可做小规模软件 IOVA/控制面和小 DMA 干扰实验；不能直接验证 GPU 多租户、200Gbps IOTLB wall、不可信 ATS 加速器安全共享，也不能从 172/172 功能验收推出论文创新或全领域隔离结论。