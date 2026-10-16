## 全局证据边界

- 本轮只做比较与设计，不执行实验、不修改 RTL/Linux、不把作者结果写成我们的结果。
- 文中的“已解决”只表示该文献在其设定内解决；不等于解决跨设备 IOMMU、跨设备生产—消费依赖或在线最少观测问题。
- 未拿到全文的关键前作按“未排除覆盖风险”处理，不按“领域空白”处理。  
  依据：`iommu-platform/docs/shared-translation-gaps-20261006.md:41-48`。
- 不能用模型一致意见或既有板上功能实验宣称论文新意；同一请求轨迹在不同调度下不能强制固定，因为反馈会影响请求发出。  
  依据：`iommu-platform/docs/translation-opportunities-20261006.md:13-21,73`。

---

# 候选1：跨设备依赖感知翻译服务

## 1. 可证伪的窄问题

在一条可复现的真实多设备生产—消费流水线中，与 FIFO、按设备公平、SJF/同 SIMD 批处理、运行时已有优先级相比，向 IOMMU/PTW 调度暴露少量跨设备依赖或消费组信息，能否在计入元数据维护成本、请求发射反馈分叉和公平性约束后，降低端到端关键路径完成时间，而不是只降低平均翻译延迟？

反证形式：
- 完整依赖 oracle 相比普通优先级几乎没有额外端到端收益；
- 普通任务/关键性优先级已取得全部或几乎全部收益；
- 依赖信息维护成本或更新延迟抵消收益；
- 无法在真实流水线中找到翻译主导且本地设备状态缺少必要信息的状态。

## 2. 最强直接前作 + 通用基线

### 直接前作 1：`walk_scheduling2018` — Scheduling Page Table Walks for Irregular GPU Applications

本地：`iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt`。目录：`catalog.csv:123`。

- 已解决：证明 PTW 服务顺序本身影响 GPU 应用翻译开销；把同一 SIMD 指令产生的多个 PTW 看成一个 job，按预计工作量调度，并批处理同一 SIMD 指令请求。原文：“prioritizing translation requests from the instructions that require less work” 与 “batch page table walk requests from the same instruction” 分别见 `walk_scheduling2018.txt:27-29,405-407`。结果：非规则 GPU 应用平均 30%、最高 41% 加速，见 `walk_scheduling2018.txt:640-641,949-950`。
- 前提：GPU SIMT、wavefront、coalescer、GPU TLB 层次、IOMMU buffer、PTW 数量、PWC；请求可附加 instruction ID。实现和评价在 gem5 扩展中，见 `walk_scheduling2018.txt:532-541`。
- 未证明覆盖：没有跨设备生产—消费依赖；同一 SIMD 指令不等于同一消费组或同一端到端关键路径；没有 NIC/GPU/加速器流水线；没有证明翻译层需要新增依赖信息，而非运行时任务调度器已有。

### 直接前作 2：`socdma2018` — SoC DMA + TLB Prefetching + MMU-Aware DMA

本地：`papers/socdma2018.txt`。目录：`catalog.csv:99`。

- 已解决：混合 IOMMU 下用编译器生成 Prefetching Helper Threads 预填 TLB，用可变数量 Miss Handling Helper Threads 并行尽管理 TLB miss，用 DMA retirement buffer 支持缺 TLB 时 DMA burst 重发。原文：“compiler-generated Prefetching Helper Threads” 与 “only stalls the missing DMA engine” 见 `socdma2018.txt:29-31,594`。
- 前提：HeSoC、host CPU + PMCA、SPM、DMA、hybrid IOMMU 可丢事务并报告 miss、编译器能生成 PHT；平台为 Xilinx Zynq-7045 FPGA，见 `socdma2018.txt:431-434`。
- 未证明覆盖：不是跨设备生产—消费依赖；PHT 依据编译期信息和数据流，不是消费组或剩余关键工作；没有 NIC–GPU 共享 IOMMU；没有证明不同设备调度下请求轨迹可控或可固定。

### 直接前作 3：`accelerator_translation2017` — Supporting Address Translation for Accelerator-Centric Architectures

本地：`papers/accelerator_translation2017.txt`。目录：`catalog.csv:2`。

- 已解决：为定制加速器设计 private TLB、shared level-two TLB 和 host page walk；数据 tiling 导致相邻 tile 共享页时，shared TLB 过滤重复 walk。原文：“shared level-two TLB to serve private TLB misses” 见 `accelerator_translation2017.txt:30-32`。报告 75.8% page walk 减少、combined approach 达理想翻译 93.6%，见 `accelerator_translation2017.txt:34-35,724-727`。
- 前提：加速器 SPM、DMA、bulk transfer、tiling、regular 访问、host MMU 可接收 offload；模拟于 PARADE/gem5，见 `accelerator_translation2017.txt:266-273`。
- 未证明覆盖：依赖来自 tiling/页共享，不是运行时生产—消费依赖；没有多设备流水线；没有证明跨设备关键路径信息不能由运行时调度器廉价获得；host MMU 在更多 sharers 下可能瓶颈。

### 直接前作 4：`mask2018` — MASK: Multi-Application Concurrency

本地：`papers/mask2018.txt`。目录：`catalog.csv:61`。

- 已解决：多应用共享 GPU 时，用 TLB-Fill Tokens、Address-Translation-Aware L2 Bypass、Address-Space-Aware DRAM Scheduler 降低 TLB 争用、公平性和翻译—数据干扰。原文：“token-based technique to reduce TLB contention” 见 `mask2018.txt:55-56`。DRAM Golden/Silver/Normal 队列见 `mask2018.txt:894-915`。
- 前提：GPU spatial multiplexing、per-application 状态、shared L2 TLB、内存控制器优先级；模拟 Mosaic/GPGPU-Sim，工作负载为 CUDA SDK/Rodinia/Parboil 等，见 `mask2018.txt:944-961`。
- 未证明覆盖：应用级公平/争用，不是设备间生产—消费依赖；没有证明“下游等待更多”应优先的翻译请求；没有跨设备同步语义和失效路径。

### 通用基线 1：`bis2012` — Bottleneck Identification and Scheduling

本地：`gap-audit-02/papers/bis2012.txt`。目录：`catalog.csv` 无该行，来自 gap-audit-02。

- 已解决：用线程等待周期识别最关键瓶颈，并把瓶颈迁移到快核。原文：“highest thread waiting cycles” 见 `bis2012.txt:235-236,300-302`。
- 前提：软件/编译器标注 BottleneckCall/BottleneckWait/BottleneckReturn；ACMP 快核；硬件 BT/AIT/SB。
- 未证明覆盖：不处理翻译请求；不处理跨设备依赖；线程等待周期不等于 IOMMU/PTW 服务机会。

### 通用基线 2：`criticality2013` — Criticality Stacks

本地：`gap-audit-02/papers/criticality2013.txt`。

- 已解决：用“有用运行时间 + 同时等待线程数”计算线程关键性，并用于频率缩放。原文：“how many threads are concurrently waiting” 见 `criticality2013.txt:52-54`；动态频率缩放见 `criticality2013.txt:848-853`。
- 前提：插桩同步原语；能识别 spinning；硬件 criticality 计数器；模拟环境。
- 未证明覆盖：线程级关键性不是翻译请求级或设备间依赖级；没有 IOMMU；不能直接证明翻译层需要新增信息。

### 通用基线 3：`mise2013` / `asm2015` — 资源 slowdown 与 QoS/公平性

本地：`gap-audit-02/papers/mise2013.txt`、`gap-audit-02/papers/asm2015.txt`。

- 已解决：MISE 用 request-service-rate 估计应用 slowdown，并周期性给最高优先级测 alone rate，见 `mise2013.txt:18-20,80-82,251-256`；ASM 用 aggregate cache-access-rate 估计 slowdown，见 `asm2015.txt:21-24,251-254`。
- 前提：内存控制器计数器、epoch/quantum、ATS 等；不是 IOMMU 设备翻译。
- 未证明覆盖：不解决跨设备生产—消费依赖；slowdown 估计不等于翻译请求关键性；不能作为 IOMMU 完整等价解决证据。

## 3. 来自文献的具体应用入口

### 入口 A：非规则 GPU 应用 + gem5 GPU/IOMMU 模型

来源：`walk_scheduling2018`。应用：Xsbench、MVT、ATAX、NW、BICG、GESUMMV 等，见 `walk_scheduling2018.txt:493-506`。

- 真实证据强度：执行驱动 gem5 仿真，不是实物板测；硬件配置为 2GHz、8 CU、4 SIMD/CU、IOMMU 256 buffer、8 PTW，见 `walk_scheduling2018.txt:476-489`。
- 具体翻译/数据/同步路径：SIMD memory instruction → coalescer → GPU L1 TLB → shared L2 TLB → IOMMU TLB → IOMMU buffer → PTW → page table；同 SIMD 指令全部 walk 完成前指令不能前进，见 `walk_scheduling2018.txt:186-202,293-308`。
- 待验证事实：该路径能否扩展成 NIC/加速器/GPU 生产—消费流水线；是否存在“设备 A 后台搬运、设备 B 小消费组解锁大计算”的真实状态；依赖元数据是否超出 instruction ID。

### 入口 B：Zynq SoC 上 Pointer Chasing 与 Stream Processing

来源：`socdma2018`。平台：Xilinx Zynq-7045，dual-core ARM Cortex-A9 + PMCA，见 `socdma2018.txt:431-434`。Benchmark 见 `socdma2018.txt:419-428,466-474`。

- 真实证据强度：FPGA 实现/仿真平台，作者称测量实际实现而非纯模型，见 `socdma2018.txt:460-464`；但仍不是 NIC–GPU 生产系统。
- 具体翻译/数据/同步路径：Worker thread → shared L1 SPM/DMA → hybrid IOMMU TLB hit/miss → miss queue → MHT 软件 PTW → page table；DMA burst 缺 TLB 时进入 retirement buffer，由 PE 补页后重发，见 `socdma2018.txt:376-417`。
- 待验证事实：跨设备生产—消费依赖是否真实存在；是否共享同一 IOMMU 翻译结构；DMA 完成、页表更新、设备消费之间需要什么同步语义；依赖信息是否能由 OS/运行时以同成本提供。

补充：`gap-audit-03/README.md:9-14,20-22` 给出 VIO、BaM、GPUrdma、GPUDirect RDMA 文档线索，但这些在本轮给定材料中不足以证明“NIC–GPU 共享 IOMMU 翻译争用已存在”。VIO 的提交前缓冲区探查必须作为软件供给替代基线列入。

## 4. 尚未解决的约束

- 假设：多设备流水线中存在翻译主导状态，且本地设备/TLB/PTW 状态缺少必要依赖信息。当前只有候选设想，见 `translation-opportunities-20261006.md:11-13`。
- 原文限制：GPUDirect RDMA 官方文档要求相关路径 1:1 映射/直通；这不能推出所有架构都不能共享 IOMMU，但也不能用 NIC–GPU 直连证明共享 IOMMU 争用。见 `translation-opportunities-20261006.md:15`；`gap-audit-03/README.md:20`。
- 原文限制：必须分别核 GPU MMU、NIC 注册翻译、IOMMU、设备 ATS 缓存的位置和生命周期，不能把注册记录、BAR 映射、IOTLB、设备缓存混成同一种翻译副本。见 `gap-audit-03/README.md:21`。
- 原文限制：`walk_scheduling2018` 的“同 SIMD 指令”分组是局部完成组，不是跨设备消费组。
- 原文限制：`accelerator_translation2017` 的页共享来自 tiling/regularity；`socdma2018` 的预取来自编译器生成 PHT；都不是任意运行时依赖。
- 缺证据：真实多设备轨迹、依赖暴露成本、请求发射反馈分叉、公平性/防饥饿、失效确认、完成语义。
- 不得用“无插桩/不改 OS”等新限制规避前作；必须与 VIO 软件供给、运行时任务依赖调度、已有优先级同条件比较。

## 5. 最小验证草案

- 输入事件：选择一条真实多设备流水线阶段，其中一个设备产生大量后台搬运，另一个设备仅剩一个小消费组未齐备并解锁下游大段计算。不能人为排队当实际需求。
- 对照策略：FIFO；按设备公平；SJF/同 SIMD 批处理；运行时已有优先级；完整依赖 oracle。必须报告请求发射反馈导致的不同请求轨迹，不承诺固定同一轨迹。
- 固定资源/语义：IOMMU buffer、PTW 数、页大小、TLB/PWC、设备队列、内存顺序、失效和权限撤销语义保持一致；若使用插桩或 OS/运行时配合，所有对照同条件获得，且不作为新颖性。
- 观测：端到端完成时间；关键路径解锁时间；每设备翻译队列；请求发出时序；依赖元数据更新频率；公平性/饥饿；软件供给替代路径。
- 主要终点：端到端流水线完成或“下游大计算解锁”时间，而不是平均翻译延迟。
- 反证条件：oracle 无额外端到端收益；普通任务/关键性优先级已取得全部收益；依赖成本抵消收益；真实轨迹不存在翻译主导状态；收益只来自人为排队或权限放宽。
- 必要的新信息/维护成本：请求到消费组/剩余工作的映射；动态阶段更新；失效/回收；防饥饿 aging；跨设备同步；与 VIO 软件供给比较的成本。

## 6. 缺文献最多 2 项

1. CATS（ICS 2015）正式全文：当前只有题录/讲稿级材料，不能当作已精读对照。见 `shared-translation-gaps-20261006.md:29,35`。
2. VIO（OSDI 2025）全文/§3 设计说明：给定上下文中只有审计摘要和本地路径，缺失时不能排除软件供给已覆盖“提交前准备映射/页”的风险。见 `gap-audit-03/README.md:9,22,26`。

## 7. 建议取舍供 Codex 审定

候选1只有在真实多设备流水线中能证明“翻译主导 + 跨设备依赖信息不可由已有运行时/调度器廉价获得 + oracle 仍有额外端到端收益”时才值得作为独立课题。否则它应收缩为候选3的支撑场景或测量/方法贡献，而不是仅凭 BIS/Criticality 的抽象相似性、模型一致意见或既有板级功能实验宣称新意。最优先先做真实路径核查：GPU MMU、NIC 注册翻译、IOMMU、设备缓存、VIO 软件供给分别在哪一层；拿不到 CATS/VIO 全文时，保留未排除覆盖风险。

---

# 候选3：最少观测指导优化

## 1. 可证伪的窄问题

在限定在线观测集合下，是否存在两种真实翻译状态，其现有 TLB/IOTLB/PTW 统计相似但最优资源动作相反；若存在，使用最小补充观测能否使“动作后悔值/关键路径收益”优于仅用现有统计和简单模型，并且在计入观测成本、扰动成本和非平稳反馈后仍成立？

必须区分：
- 模型/离线真值知道所有事件；
- 在线策略只能看到采样计数器、有限归因状态或短暂有界扰动；
- 主要终点是动作后悔值或关键路径收益，不是分类准确率。

反证：现有计数器/简单模型已取得同样后悔值；观测和反应比应用阶段还慢；观测成本超过关键路径收益；真实状态不可区分；无法获得完整事件真值。

## 2. 最强直接前作 + 通用基线

### 直接前作 1：`counterpoint2026` — CounterPoint

本地：`gap-audit-02/papers/counterpoint2026.txt`。文档说明见 `shared-translation-gaps-20261006.md:12,23`。

- 已解决：用 HEC 模型约束、LDD、model cone、counter confidence regions 检验/修正微架构假设；案例是 Haswell MMU，发现 TLB prefetch、合并 PTW、abort PTW 等，见 `counterpoint2026.txt:26-31,139-146,781-814`。
- 前提：专家先验模型、HEC 采样、99% 置信、Haswell；不是在线控制器。
- 未证明覆盖：不是 IOMMU/跨设备翻译控制；特定 19 计数器噪声阈值不能泛化，见 `counterpoint2026.txt:83-84`；模型一致性检查不等于动作选择；不代表已解决最少观测控制问题。
- 文档明确：“CounterPoint ... 尚不是一个在线控制器”，见 `shared-translation-gaps-20261006.md:23`。

### 直接前作 2：`observuvm2026` — ObservUVM

本地：`papers/observuvm2026.txt`。目录：`catalog.csv:75`。

- 已解决：复用 PCIe access counters 提供对 HBM-resident page 的采样观测；用 sampled observability 改进 UVM eviction/prefetch；真实 NVIDIA 3090 上评估，见 `observuvm2026.txt:17-25,663-668`。原文：“repurpose existing hardware access counters” 见 `observuvm2026.txt:17-18`；“sampling a single page within a region provides sufficient observability” 见 `observuvm2026.txt:375-376`。
- 前提：NVIDIA UVM driver、access counters、2MB/64KB region/page、userspace 策略、eBPF 通信；观测是采样的。
- 未证明覆盖：目标是 HBM 超额订阅的 eviction/prefetch，不是 IOMMU/PTW 资源动作；没有动作后悔值/关键路径终点；采样充分性依赖空间局部性，不能直接搬到翻译控制。

### 直接前作 3：`mise2013` — MISE

本地：`gap-audit-02/papers/mise2013.txt`。

- 已解决：通过 request-service-rate 估计 slowdown；周期给最高优先级以获得 alone-request-service-rate；构建 QoS/Fair 策略，见 `mise2013.txt:18-20,80-82,251-256`。
- 前提：内存控制器计数器、epoch/interval、优先级调度；模拟 DDR3 + x86。
- 未证明覆盖：main memory 争用，不是设备 IOMMU 翻译；slowdown 估计不是动作后悔值；没有最少观测集合定义；不是 IOMMU 完整等价解决证据。

### 直接前作 4：`asm2015` — ASM

本地：`gap-audit-02/papers/asm2015.txt`。

- 已解决：用 aggregate cache access rate 估计 shared cache + main memory slowdown；不依赖 a priori 应用知识；用于 cache partitioning、memory bandwidth partitioning、soft slowdown guarantee，见 `asm2015.txt:21-24,130-137,1064-1090`。
- 前提：辅助 tag store、epoch/quantum、内存控制器优先级；模拟环境。
- 未证明覆盖：不是 IOMMU 翻译；不解决跨设备依赖；没有动作后悔值/关键路径收益；不能作为 IOMMU 完整等价解决证据。

### 通用基线 1：`bis2012`

- 已解决：等待周期识别瓶颈并加速，见 `bis2012.txt:17-19,235-236`。
- 前提：插桩 BottleneckCall/Wait；ACMP。
- 未证明覆盖：不针对翻译观测；不给出最少观测集合；不处理 IOMMU 噪声计数器。

### 通用基线 2：`criticality2013`

- 已解决：线程关键性度量和动态频率缩放，见 `criticality2013.txt:20-23,848-853`。
- 前提：同步插桩、硬件 criticality 计数器。
- 未证明覆盖：线程级，不是设备翻译请求级；不是最少观测；不处理 IOMMU。

### 通用基线 3：`coz2015` — Causal Profiling

本地：`gap-audit-02/papers/coz2015.txt`。

- 已解决：用 virtual speedup 预测优化某行代码的程序收益；能发现 contention 和优化机会，见 `coz2015.txt:20-29,103-113`。原文：“virtually speeding up code” 见 `coz2015.txt:20-27`。优化结果示例：Memcached 9%、SQLite 25%、PARSEC 最高 68%，见 `coz2015.txt:32-37`。
- 前提：采样、progress points、虚拟加速、Linux perf；有 17.6% 平均开销，见 `coz2015.txt:888-890`。
- 未证明覆盖：不是 IOMMU/设备翻译控制；不是最少观测；扰动本身会改变程序执行；不能作为 IOMMU 等价解决证据。

## 3. 来自文献的具体应用入口

### 入口 A：CounterPoint Haswell MMU + HEC 观测

来源：`counterpoint2026`。

- 真实证据强度：真实 Intel Haswell，约 20 million HEC samples，覆盖 GAPBS/SPEC2006/PARSEC/YCSB，见 `counterpoint2026.txt:740-753`。强于纯仿真，但分析离线、非在线控制。
- 具体翻译/数据/同步路径：load → STLB/TLB → PDE cache lookup → page walker → HEC 事件；含 TLB prefetch、walk merging、abort、PML4E cache 等，见 `counterpoint2026.txt:459-474,781-854`。
- 待验证事实：目标平台 IOMMU/设备是否有等价计数器；计数器语义、噪声和采样是否足以区分动作；能否从模型一致性推进到在线 action regret 最小化。

### 入口 B：ObservUVM 的 NVIDIA 3090 / UVM 驱动路径

来源：`observuvm2026`。

- 真实证据强度：真实 NVIDIA 3090 + AMD CPU + PCIe 4.0，14 个应用，修改 UVM driver + userspace policy，见 `observuvm2026.txt:663-668,1157-1274`。强于纯模型，但不是 IOMMU 翻译控制。
- 具体翻译/数据/同步路径：GPU access → HBM/DRAM page → page fault 或 access counter → UVM driver → userspace event loop → eviction/prefetch 决策 → migration/map/unmap，见 `observuvm2026.txt:130-160,468-490`。
- 待验证事实：同一采样思想能否用于 IOMMU/PTW 资源动作；观测是否反映翻译关键路径，而非仅 HBM hotness；观测/反应成本是否低于动作收益。

## 4. 尚未解决的约束

- 假设：相似现有 TLB 统计背后可能需要相反优化动作。当前只有候选假设，见 `translation-opportunities-20261006.md:39-41`。
- 原文限制：CounterPoint 的 19 计数器噪声阈值是 Haswell MMU 特例，不能泛化为所有硬件；且它是模型一致性检查，不是在线控制器。见 `counterpoint2026.txt:83-84`；`shared-translation-gaps-20261006.md:23`。
- 原文限制：Coz 是软件因果剖析，MISE/ASM 是 CPU 内存 slowdown，BIS/Criticality 是线程/瓶颈关键性；都不是 IOMMU 完全等价解决证据。见 `shared-translation-gaps-20261006.md:31`；`translation-opportunities-20261006.md:49`。
- 缺证据：IOMMU/设备翻译的完整事件真值；观测语义和噪声模型；动作后悔值定义；非平稳负载和跨资源反馈；扰动代价；观测/反应时延。
- 缺证据：真实应用中“相似统计、相反动作”的可判别对象；不是只交分类器或仪表盘。
- 不得用“无插桩/不改 OS”等新限制规避前作；必须与 VIO 软件供给、Coz 虚拟加速、MISE/ASM slowdown 估计同条件比较。

## 5. 最小验证草案

- 输入事件：真实负载中找一组状态，其现有 TLB/IOTLB/PTW 统计近似相同，但完整事件真值显示最优动作相反。候选状态包括：PTW 队列阻塞关键消费者 vs 翻译延迟被其他工作隐藏；映射更新/软件供给 vs 翻译服务。
- 对照策略：完整事件 oracle；现有计数器 + 简单阈值；现有计数器 + 简单模型；少量归因状态；短暂有界扰动；软件供给基线 VIO。所有策略必须计入观测和扰动成本。
- 固定资源/语义：IOMMU buffer、PTW 数、页大小、TLB/PWC、失效/权限语义、设备队列和完成条件一致；不放松权限撤销或过期翻译；不把“无插桩/不改 OS”当新颖性。
- 观测：动作选择；关键路径收益；相对 oracle 的动作后悔值；观测开销；扰动副作用；错误归因；阶段变化；公平性/饥饿。
- 主要终点：关键路径收益/动作后悔值 per decision 或 per resource，不是分类准确率。
- 反证条件：现有计数器/简单模型达到同样后悔值；观测+反应慢于应用阶段；观测成本超过收益；真实负载无可判别状态；无法获得完整真值；VIO 软件供给已解决该动作选择。
- 必要的新信息/维护成本：翻译动作到任务进度的因果信息；计数器语义和噪声；最小观测集维护；在线校准；扰动控制；非平稳反馈处理。

## 6. 缺文献最多 2 项

1. HELM 全文：当前仅出版社摘要/索引片段，不能把它当作不覆盖或已覆盖。见 `catalog.csv:44`；`shared-translation-gaps-20261006.md:41`。
2. VIO 全文/§3：当前给定上下文中只有审计摘要和路径，缺失时不能排除软件供给路径已覆盖“提交前准备映射/页”。见 `gap-audit-03/README.md:9,22,26`。

## 7. 建议取舍供 Codex 审定

候选3应优先定位为候选1的支撑或测量/方法贡献，而不是强行保留独立课题。保留条件只有一个：在真实翻译控制中证明“现有观测不可区分、补充观测可降低动作后悔值或提升关键路径收益、且观测/扰动成本计入后仍成立”。CounterPoint 的 Haswell/19 计数器噪声不能泛化；ObservUVM 是 HBM 观测而非 IOMMU 控制；Coz/BIS/MISE/ASM 都不是 IOMMU 完全等价解决证据。若只能提高分类准确率或做仪表盘，应停止独立立题；若能与候选1共享真实多设备流水线，则作为“观测最小化 + 动作收益”的支撑章节处理。未取得 HELM/VIO 全文前，必须保留未排除覆盖风险；不能用模型一致意见或既有板级功能实验宣称新意。