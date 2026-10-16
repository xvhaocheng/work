# 有界缺口审计：123项专题CSV × 旧广域地图 × 四候选

## 0. 证据边界与三态定义（先立规矩）

我无法联网，因此下列一切"某工作是否存在"的判断只有三种合法表述，全文严格分用：

- **A 态｜当前目录确实未收**：在 `catalog.csv`（123 行，L2–L124）中检索不到该条目。**不等于该工作不存在**，只等于"本专题库未收"。
- **B 态｜旧广域地图已有而专题未并入**：在 `research-landscape-20261005.md` 中出现，但 `catalog.csv` 中无对应行。这不是"没有论文"，是**内部合并缺口**。
- **C 态｜已收录但缺全文**：`catalog.csv` 有行但 `fulltext_available=false`，只能作摘要/题录级证据。

所有靠记忆写出的题名/作者/年/venue 一律标 **〔待外部核验〕**；不确定字段写 `null`；不造 URL。凡我在给定文件中找到的直接事实，给文件与行号。

---

## 1. 导航摘要

### 1.1 三态清点（均为给定文件事实）

| 类别 | 数量 | 明细 |
|---|---|---|
| 目录总行 | 123 | `catalog.csv` L2–L124 |
| **C 态：已收录但缺全文** | **19** | `activeforward2018`(L3)、`cato2026`(L11)、`cdfd2026`(L12)、`clover2026`(L15)、`cryptommu2023`(L20)、`ctl2026`(L21)、`helm2025`(L44)、`lamp2025`(L55)、`latpc2025`(L56)、`libra2026`(L59)、`maverick2026`(L62)、`nbwalker2026`(L69)、`ontable2017`(L77)、`sagepte2026`(L89)、`shadowupdate2026`(L90)、`shared1993`(L91)、`snakebyte2023`(L97)、`synergistic2010`(L108)、`transfetch2026`(L110) |
| **B 态：旧地图有、目录未并入（论文）** | **12** | DAE、UCP、KPart、NDS、GPS、DX100、Tiara、WIC、MINISA、PagedAttention、vTensor/FlexInfer、GMLake（均见 `research-landscape-20261005.md` §2.1/§5/§7.2，catalog 无对应行） |
| **B 态：旧地图有、目录未并入（正式规范/文档）** | **4** | RISC-V IOMMU in-memory queues(L134)、CUDA stream-ordered allocator(L135)、Linux HMM(L136)、**Linux SVA/ENQCMD(L137)** |

**注意 B 态第 4 项**：旧地图 §6 已经把 Linux SVA/**ENQCMD** 规范列为"生产 API 已有语义"，而 123 项目录里**一条规范条目都没有**。这正是候选 4（ECMDQ/队列实现）必须补齐的前置。

### 1.2 结构性发现（直接决定四候选能否声称新意）

**发现 1 — 候选 1、3 在目录里是"IOMMU 内部自证"，缺全部通用方法论对手。**
`translation-opportunities-20261006.md:15` 自己写明"还必须补齐**一般任务图调度、关键性内存调度和异构流水线研究**，不能只在 IOMMU 论文中检索"；`:56` 又说"关键性、SIMT 指令完成组和并发调度都有经典/近年工作"。但 `catalog.csv` 全文检索**没有任何一条**通用任务调度 / 关键性理论 / DAG 运行时 / 因果剖析条目——最接近的只有 `walk_scheduling2018`(L123)、`walk_stealing2021`(L124)、`mask2018`(L61)、`pact2026`(L79)、`soaralto2025`(L98)、`observuvm2026`(L75)。**结论：候选 1 与候选 3 当前的新意主张，是在"对手池被人为限定在 IOMMU/翻译圈"的条件下成立的。**

**发现 2 — 候选 2 缺"副本生命周期"的三大经典支柱：租约、缓存准入、DSM 自适应复制/一致性。**
`translation-opportunities-20261006.md:25` 明确要求"还要查**自适应复制、缓存准入与一致性协议的经典近邻**"；`evidence.md:25-26` 把该候选的停止条件压在"真实更新率与净收益分解"上。但目录内与"谁持有副本、何时回收"相关的只有 `clover2026`(L15, 摘要)、`hydra2024`(L45)、`idyll2023`(L47)、`sharemmu2026`(L92)、`transfw2023`(L111)、`mgvm2022`(L65)——**全部是 2018 年后的系统机制，没有一条是"租约/准入/一致性"的一般理论**。

**发现 3 — 候选 4 的目录状态最干净也最危险。**
`translation-opportunities-20261006.md:53` 已把 2024 年 ECMDQ 内核讨论当作**反证线索**记入（"不能将问题发现或'多队列'包装成首次贡献"），`:54` 又用 RISC-V IOMMU 规范否掉"乱序执行"新意。但 `catalog.csv` 中 **ECMDQ、iommufd、QEMU riscv-iommu、Linux RISC-V IOMMU 驱动全部不存在**；旧地图 §6 只有 ENQCMD 一条规范链接。**后果：候选 4 目前只有一个 LKML 链接级别线索，没有任何可比较的实现侧原文。**

**发现 4 — 已收录但缺全文的 19 条中，有 6 条直接压在候选 2/候选 1 的判据上**：`shadowupdate2026`（候选 2 的净收益结论唯一支撑，`evidence.md:24` 已声明不能当全文）、`clover2026`、`latpc2025`、`helm2025`、`snakebyte2023`、`synergistic2010`。这 6 条若不取全文，候选 2 的"必须超过的前作"清单是**不完整**的。

**发现 5 — 输入内部存在两处**假阴性**（给定文件事实，值得回修）：**
- `evidence.md:51` 称"**MMU-awareDMA** 在给定文件未见全文/题录，不能比较"——但 `catalog.csv:99` 的 `socdma2018`（"…with TLB Prefetching and **MMU-Aware DMA Engine**"）**已收录且 `fulltext_available=true`**。
- `evidence.md:51` 又把"**rIOMMU 全文**"列为必补读——但 `catalog.csv:87` 的 `riommu2015` **已收录且 `fulltext_available=true`**。
- `translation-opportunities-20261006.md:63` 自己也承认"MMU-aware DMA 在本轮 worker 输入中缺席，不代表 123 项库没有该文"。——**这三处说明：候选 1/候选 4 的部分"缺失证据"结论，源于检索路径而非文献缺口，应在补外之前先修内。**

### 1.3 分配原则

- **P0（直接可能否定候选）**：不读就会把已被做过的事写成新意。
- **P1（成熟方法，必须作为强基线对照）**：读了通常不会否定候选，但会改变"增益来源"的表述。
- **P2（拓宽/盲点）**：线索级或方向级，不宣称结论。

---

## 2. 总表（24 项）

| ID | 题名 | 年/venue | 优先 | 对应候选步骤 | 目录状态 |
|---|---|---|---|---|---|
| G01 | Coz: Finding Code that Counts with Causal Profiling | SOSP 2015〔待核〕 | P0 | 候选3（`translation-opportunities:29-39`） | A 态 |
| G02 | Application Slowdown Model (ASM) | null〔待核〕 | P0 | 候选3 + 候选1 | A 态 |
| G03 | Bottleneck Identification and Scheduling in Multithreaded Applications (BIS) | ASPLOS 2012〔待核〕 | P0 | 候选1 + 候选3 | A 态 |
| G04 | Criticality Stacks: Identifying Critical Threads in Parallel Programs using Synchronization Behavior | ISCA 2013〔待核〕 | P0 | 候选1 | A 态 |
| G05 | PAR-BS: Parallelism-Aware Batch Scheduling | ISCA 2008〔待核〕 | P0 | 候选1 + 降级表 L56-57 | A 态 |
| G06 | TimeGraph: GPU Scheduling for Real-Time Multi-Tasking Environments | USENIX ATC 2011〔待核〕 | P0 | 候选1 | A 态 |
| G07 | MemGuard（+MemPol） | RTAS 2013 / RTAS 2015〔待核〕 | P0 | 候选1 + 候选2 | A 态 |
| G08 | MIND: In-Network Memory Management for Disaggregated Data Centers | SOSP 2021〔待核〕 | P0 | 候选2 + 候选4 | A 态 |
| G09 | ECMDQ：内核 ATS 失效命令队列补丁系列（正式技术资料） | 2024 LKML 讨论已见〔待核〕 | P0 | 候选4 + 降级表 L53 | A 态（仅 `:53` 线索） |
| G10 | ATLAS: A Scalable and High-Performance Scheduling Algorithm for Multiple Memory Controllers | HPCA 2010〔待核〕 | P1 | 候选1 | A 态 |
| G11 | MISE（内存干扰与 slowdown 估计） | null〔待核〕 | P1 | 候选3 | A 态 |
| G12 | Baymax: QoS Awareness and Increased Utilization for Non-Preemptive Accelerators | ASPLOS 2016〔待核〕 | P1 | 候选1 | A 态 |
| G13 | vCUDA / GPU 虚拟化调度族 | null〔待核〕 | P1 | 候选1 | A 态 |
| G14 | CATS：criticality-aware task scheduling runtime | null〔待核〕 | P1 | 候选1 | A 态 |
| G15 | StarPU（+ PaRSEC / Legion）：异构 DAG 任务运行时 | 2011 / 2013〔待核〕 | P1 | 候选1 | A 态 |
| G16 | Lazy Release Consistency / Munin / TreadMarks：自适应复制与一致性 | 1992 / 1991 / 1994〔待核〕 | P1 | 候选2 | A 态 |
| G17 | ARC / TinyLFU / Hyperbolic caching：缓存准入与替换 | 2003 / 2017 / 2017〔待核〕 | P1 | 候选2 | A 态 |
| G18 | CPI²（Eyerman）+ Bubble-Up：干扰归因与 slowdown 估计 | 2009 / 2011〔待核〕 | P1 | 候选3 | A 态 |
| G19 | iommufd / QEMU riscv-iommu / Linux RISC-V IOMMU 驱动：开源队列实现 | null（正式文档） | P1 | 候选4 | A 态 |
| G20 | CHERI 能力式对象地址表示（+Intel MPX） | S&P 2015〔待核〕 | P2 | 候选4 | A 态 |
| G21 | Dead on Arrival 2026 | null | P2 | 未知，可能 P0 | 线索级（`translation-opportunities:~` 未列正式条目） |
| G22 | GPUDirect RDMA / NVSHMEM / GPUDirect Storage（官方技术资料族） | null | P2 | 候选1 | A 态 |
| G23 | TPP: Transparent Page Placement for CXL-Enabled Tiered-Memory 等 tiered 强基线 | ASPLOS 2023〔待核〕 | P2 | 候选3（反方基线） | A 态 |
| G24 | 近年 IOMMU/UVM/NDP 扩查检索位（不凭记忆伪造题名） | null | P2 | 全部 | A 态 |

---

## 3. 逐项明细

> 说明：我无权访问这些原文，**不虚构章节号**。"需核段落"一律写成"原文必须回答的问题"，供取全文后定向定位。

### P0

**G01｜Coz: Finding Code that Counts with Causal Profiling** — Curtsinger, Berger；SOSP 2015〔待外部核验〕
- **直接冲突的候选步骤**：候选 3（`translation-opportunities-20261006.md:29-39`）。候选 3 的可发表主张是"用少量归因状态/有界扰动，判别'多给翻译资源是否真的推进任务'"。Coz 已把"反事实扰动→因果归因"做成**通用方法论**（virtual speedup）。
- **需核要点**：(a) virtual speedup 的定义与施加方式；(b) 实验成本与所需重复次数；(c) 非平稳负载与扰动偏差的处理；(d) 是否要求"扰动可被注入到被测量资源本身"。
- **为何 123 项不够**：目录内涵盖"观测"的只有 `observuvm2026`(L75)、`pact2026`(L79)、`soaralto2025`(L98)、`iotlb_bottleneck2010`(L52)，全部是**系统内观测机制**，无一条通用因果剖析方法论。`evidence.md:40-42` 自己也写"必须给出可判别的因果测试或扰动设计"——这句话的对手在目录外。

**G02｜Application Slowdown Model (ASM)** — 作者/年/venue：`null`〔待外部核验；记忆疑似 MICRO 2013 段，**不据此写定**〕
- **直接冲突的候选步骤**：候选 3（慢的是谁、慢多少）与候选 1（"等待越多的消费者未必关键"）。
- **需核要点**：(a) slowdown 的参考基准定义（独跑？公平共享？）；(b) 所需输入（PMU/间隔采样/插桩）与是否满足"少量观测"；(c) 误差界与开销；(d) 是否处理**外部设备/IO 侧**发起者。
- **为何 123 项不够**：目录内 slowdown 语义只有 `pact2026`（页级 PAC）与 `soaralto2025`（AOL=Latency/MLP），见 `evidence.md:39`；两者都是 tiered-memory 内的 criticality，不是通用 slowdown 估计。若 ASM 类已能在现有计数器下估计干扰，候选 3 的"还缺什么信息"命题必须重写。

**G03｜Bottleneck Identification and Scheduling in Multithreaded Applications (BIS)** — Joao, Suleman, Mutlu, Patt 等；ASPLOS 2012〔待外部核验〕
- **直接冲突的候选步骤**：候选 1（`translation-opportunities:9-17`）——"仅按发起设备/请求数量/本地等待时间分配会选错请求"。BIS 的核心就是"识别**被拖慢**的线程并优先放行"，与该取舍**同构**。
- **需核要点**：(a) 瓶颈判据（同步阻塞？加速比曲线？临界区预测？）；(b) 判据所需信息是否本地可获得——若只需本地信息，则候选 1 的"跨设备依赖提供现有接口无法获得的必要信息"这一条被直接削弱；(c) 是否覆盖生产者—消费者跨设备。
- **为何 123 项不够**：同类只有 `walk_scheduling2018`（同 SIMD 指令批处理）、`walk_stealing2021`（多租户窃取）、`mask2018`。`translation-opportunities:56` 已自认"关键性…有经典/近年工作"——但那条工作不在库里。

**G04｜Criticality Stacks** — Du Bois, Eyerman, Eeckhout；ISCA 2013〔待外部核验〕
- **直接冲突的候选步骤**：候选 1 的机制探索（`:13`"由运行时标记消费组、依赖关系或近似剩余工作"）。若"关键"可由**同步行为**预测，则候选 1 必须证明其运行时标记提供了超出同步行为的信息。
- **需核要点**：关键性定义、所需插桩（编译器/运行时）、预测误差、是否依赖可静态获知的同步结构。
- **为何 123 项不够**：目录内无任何关键性理论条目；`pact2026` 是页级在线归因、`soaralto2025` 是 AOL 代理，都不回答"关键性从何而来"。

**G05｜PAR-BS: Parallelism-Aware Batch Scheduling** — Mutlu, Moscibroda；ISCA 2008〔待外部核验〕
- **直接冲突的候选步骤**：候选 1 的批处理/同组优先，以及降级表 `translation-opportunities:56-57`（"单独改 batch…不拆成多个表面不同的'创新'"）。PAR-BS 已证明"分批 + 并行度感知"可同时改善吞吐与**防饥饿公平性**。
- **需核要点**：(a) batch 形成规则与释放条件；(b) 防饥饿机制（是否用 per-thread 进度指标）；(c) 与 per-application slowdown 的关系。
- **为何 123 项不够**：目录内 batch 近邻只有 `batch_uvm2020`(L6, UVM batch 语义)、`riommu2015`(L87, ring/burst 末失效)、`walk_scheduling2018`(同指令批处理)——**没有一条是请求调度层的 batching+公平性经典**。

**G06｜TimeGraph: GPU Scheduling for Real-Time Multi-Tasking Environments** — Kato 等；USENIX ATC 2011〔待外部核验〕
- **直接冲突的候选步骤**：候选 1（异构/多设备流水线中的优先级与依赖服务）。
- **需核要点**：优先级如何从依赖/死线导出、如何处理**非抢占**执行体、调度决策实现位置（内核/驱动/用户态）、是否覆盖多设备协同。
- **为何 123 项不够**：目录内 GPU 侧调度只有 `orchestrated2023`(L78, 地址翻译的调度与分区) 与 `mask2018`；无通用 GPU 任务调度/QoS 条目。

**G07｜MemGuard（RTAS 2013）/ MemPol（RTAS 2015）** — Yun 等；〔待外部核验〕
- **直接冲突的候选步骤**：候选 1（为关键消费者保留资源）与候选 2（副本准入 = 条目/带宽预算）。
- **需核要点**：预留与回收机制、是否作用于设备侧发起者、利用率代价、超发时的仲裁。
- **为何 123 项不够**：目录内 I/O 侧只有 `iommu_scalable2015`(L51, magazine/延迟失效) 与 `iotlb_wall2023`(L53, hugepage 缓解)；**无带宽预留/准入控制条目**，候选 2 若落到"预算分配"将无对手可比。

**G08｜MIND: In-Network Memory Management for Disaggregated Data Centers** — SOSP 2021〔待外部核验〕
- **直接冲突的候选步骤**：候选 2（谁持有副本、何时传播/回收）与候选 4（对象身份与表示）。
- **需核要点**：所有权/一致性协议、失效路径与完成语义、对 disaggregated 拓扑的假设、是否需要可编程交换。
- **为何 123 项不够**：目录内近邻 `clio2022`(L14)、`deact2021`(L25)、`dtl2023`(L30)、`m2ndp2024`(L60) 都是**翻译/内存结构或 NDP 执行**，不是"副本所有权协议"；MIND 未收录，候选 2 的"设备—IOMMU 协作控制副本传播"因此缺少最近的结构性对手。

**G09｜ECMDQ + 队列语义（正式技术资料）** — Intel 内核补丁系列；2024 LKML 讨论已作为线索存在于 `translation-opportunities-20261006.md:53`〔待外部核验〕
- **直接冲突的候选步骤**：候选 4 与降级表 `:53`。文档已自认"ECMDQ 内核讨论已经明确提到 ATS 队头阻塞，以及让不相关失效并行推进"，因此**"多队列/独立队列解决队头阻塞"不能作为新意**；候选 4 必须在 ECMDQ 之外另找约束。
- **需核要点**：(a) ECMDQ 的命令队列归属（per-device/per-domain？）与完成/fence 范围；(b) 与 RISC-V 侧 IOTINVAL/IODIR/IOFENCE.C（旧地图 L134）的对应关系；(c) 是否处理"无关设备被慢 ACK 拖住"。
- **为何 123 项不够**：`catalog.csv` 中**没有任何队列实现条目**；旧地图 §6（L137）只有 Linux SVA/**ENQCMD** 规范链接且**未并入目录**。候选 4 目前只有链接级线索，没有可比原文。

### P1

**G10｜ATLAS: A Scalable and High-Performance Scheduling Algorithm for Multiple Memory Controllers** — HPCA 2010〔待外部核验〕
- 冲突：候选 1 的"多资源/多队列下如何分配"。核：per-bank/controller 粒度调度、是否按线程并行度而非公平性优化、批量与优先级的耦合。
- 123 项不够：`iommu_scalable2015` 处理的是软件 IOVA 分配，不是控制器/通道级调度。

**G11｜MISE（内存干扰与 slowdown 估计）** — 作者/年/venue `null`〔待外部核验，我对题名归属信心不足，勿引用〕
- 冲突：候选 3。核：估计所需的观测集合、是否需要离线训练、误差与在线可用性。
- 123 项不够：见 G02。

**G12｜Baymax: QoS Awareness and Increased Utilization for Non-Preemptive Accelerators in Warehouse Scale Computers** — ASPLOS 2016〔待外部核验〕
- 冲突：候选 1 的"加速器是非抢占的、完成顺序决定消费者开工"。核：QoS 度量、非抢占下的排队与优先级、与集群调度的接口。
- 123 项不够：目录内无加速器 QoS 条目（`sharemmu2026`、`cryptommu2023`、`border2015` 都是安全/隔离向）。

**G13｜vCUDA / GPU 虚拟化调度族** — 作者/年/venue `null`〔待外部核验〕
- 冲突：候选 1（虚拟化层如何暴露/隐藏设备依赖）。核：API 拦截点、是否能看到生产者—消费者关系。
- 123 项不够：目录内 GPU 虚拟化/多租户只有 `mask2018`、`star2024`、`walk_stealing2021`，均为硬件层资源竞争，非调度运行时。

**G14｜CATS：criticality-aware task scheduling runtime** — `null`〔待外部核验，题名归属不确定〕
- 冲突：候选 1 的机制探索。核：关键性如何在线推导、运行时开销、是否需要应用配合。
- 123 项不够：同 G04。

**G15｜StarPU / PaRSEC / Legion：异构 DAG 任务运行时** — 2011 / 2013 / 2012〔待外部核验〕
- 冲突：`translation-opportunities:15` 明确点名"**一般任务图调度**"必须补齐。候选 1 的"消费组 + 依赖 + 近似剩余工作"与 DAG 运行时的 ready-set/critical-path 概念直接相邻。
- 核：依赖表达方式、就绪判定、跨设备数据搬运与依赖的关系、是否已有"按关键路径优先搬运"。
- 123 项不够：目录内 `orchestrated2023`、`walk_scheduling2018` 都在 GPU 翻译内部，无 DAG 运行时条目。

**G16｜Lazy Release Consistency / Munin / TreadMarks** — 1992 / 1991 / 1994〔待外部核验〕
- 冲突：候选 2 的"自适应复制 + 一致性 + 何时传播"。这三者构成"多副本存活期与失效传播"的经典理论层。
- 核：复制粒度与准入、释放一致性下失效的传播范围、写共享导致的复制退化。
- 123 项不够：目录内 `unitd2010`(L114)、`hatric2017`(L42)、`latr2018`(L57)、`site2017`(L95) 都是 CPU 翻译/缓存一致性，没有 DSM 复制理论条目。

**G17｜ARC / TinyLFU / Hyperbolic caching** — 2003 / 2017 / 2017〔待外部核验〕
- 冲突：`translation-opportunities:25` 点名的"**缓存准入**"。候选 2 若要决定"是否把翻译副本留在某设备"，必须证明超出通用准入策略的部分。
- 核：准入判据、是否需要历史频率、扫描抗性、开销。
- 123 项不够：目录内无替换/准入策略条目（`victima2023` 是 TLB reach 借 L2，不是准入）。

**G18｜CPI²（Eyerman 等，2009）+ Bubble-Up（Mars 等，2011）**〔待外部核验〕
- 冲突：候选 3 的"相似 miss 率、相反动作"必须靠归因模型或干扰曲线来判别。
- 核：归因所需计数、是否假设同构核、对内存受限段的区分能力、对短窗口的可用性。
- 123 项不够：`evidence.md:39` 只列了 `iotlb_bottleneck2010` 的成本隔离微基准，属 IOMMU 内部，不是通用归因。

**G19｜iommufd / QEMU riscv-iommu / Linux RISC-V IOMMU 驱动（开源队列实现）** — 正式文档/代码，`null` 元数据
- 冲突：候选 4 要求"现有开源队列实现"。必须核实：命令队列的分发与提交、失效命令的作用域、完成与 fence 的软件可见语义。
- 123 项不够：目录内 I/O 软件面只有 `viommu2011`(L120)、`riommu2015`、`iommu_scalable2015`、`fastsafe2024`(L33)，全部早于 ECMDQ 世代；无实现侧对照。

### P2

**G20｜CHERI（能力式对象地址表示，IEEE S&P 2015）与 Intel MPX**〔待外部核验〕
- 冲突：候选 4 的"同一底层对象、不同设备不同翻译表示"。核：对象粒度寻址如何避免页级间接、权限如何随表示携带、是否已被否证（MPX 被弃用是重要反证）。
- 123 项不够：目录内对象表示只有 `vbi2020`、`utopia2023`、`oasis2025`、`dpt2024`，全部是页/区域映射层。

**G21｜Dead on Arrival 2026** — 题名/作者/venue 全 `null`
- **仅"已有搜索线索但非正式收录"**。在给定四个文件中，我**找不到**它的正式条目：`catalog.csv` 无行，`research-landscape-20261005.md` 无，`translation-opportunities-20261006.md` 无，`evidence.md` 无。
- 处理建议：先外部核验其 venue 与主题；**若属 IOMMU/UVM/多 GPU 翻译，应直接升级为 P0**（因为它可能是候选 1–4 的直接近邻且当前完全不可见）。

**G22｜GPUDirect RDMA / NVSHMEM / GPUDirect Storage（官方技术资料族）** — `null` 元数据
- 冲突：候选 1 的"CPU–GPU–NIC 流水线"。核：生产者—消费者同步原语（stream/event/信号量）、是否暴露完成顺序给主机侧调度。
- 123 项不够：`research-landscape-20261005.md:190` 明示"RDMA 注册缓存/撤销"属**尚未覆盖**范围。

**G23｜TPP: Transparent Page Placement for CXL-Enabled Tiered-Memory（ASPLOS 2023）等 tiered 强基线**〔待外部核验〕
- 冲突：候选 3 的**反方基线**——若"热度"已能被通用 tiering 策略做好，则"关键性"的增量收益空间需要重新测量。
- 123 项不够：目录内有 `memtis2023`、`nomad2024`、`soaralto2025`、`pact2026`，但没有以"热度 vs 关键性"对照为核心的经典基线集。

**G24｜近年 IOMMU / UVM / NDP 扩查检索位** — `null`
- 我**没有可靠题名**，因此不伪造。建议按会场页直接扫：ISCA/MICRO/ASPLOS/OSDI/ATC/FAST 2024–2026 的 IOMMU、UVM、CXL 翻译、NDP 翻译分场；以及 MICRO 2026 已知存在但目录未收的同会场条目（`catalog.csv:110` 的 `transfetch2026` 注记已提到同会还有 "Trail（temporal TLB prefetching）" 未收）。
- 为何 123 项不够：`research-landscape-20261005.md:190` 自认 CXL 完整放置/一致性、RDMA 注册缓存、更广实时 QoS/形式化界**未覆盖**。

---

## 4. 最优先取全文的 8 项

按"不取会直接写错新意"排序：

1. **G01 Coz（SOSP 2015）** —— 决定候选 3 能否以"提出因果归因"为贡献。
2. **G02 Application Slowdown Model** —— 决定候选 3 的"还缺什么信息"是否成立；也决定候选 1 的目标函数是否已被定义过。
3. **G03 BIS（ASPLOS 2012）** —— 决定候选 1 的"关键性调度"是否只是把本地瓶颈识别搬到 IOMMU。
4. **G04 Criticality Stacks（ISCA 2013）** —— 决定"关键性"从何来这一问是否已被回答。
5. **G06 TimeGraph（ATC 2011）** —— 决定"给翻译请求加优先级"是否已有异构调度框架层对手。
6. **G07 MemGuard（RTAS 2013）** —— 决定候选 2 的"副本准入"是否只是带宽预留的改名。
7. **G08 MIND（SOSP 2021）** —— 决定候选 2/候选 4 的"副本所有权"是否已有更彻底的结构性方案。
8. **G09 ECMDQ + 队列语义** —— 决定候选 4 的问题定义是否已在 2024 年被公开提出（`:53` 的线索只到链接级，不足以定案）。

**次级（若预算允许，紧随其后）**：G05 PAR-BS（防饥饿/批处理）、G19 开源队列实现、G15 StarPU/PaRSEC。

---

## 5. 状态核查：Direct Segments / RMM / Devirtualizing Memory

| 工作 | 目录状态 | 事实来源 |
|---|---|---|
| **Direct Segments**（Efficient Virtual Memory for Big Memory Servers） | **已在目录且全文可读**（`direct_segments2013`） | `catalog.csv:28`，`fulltext_available=true`；旧地图亦有（`research-landscape-20261005.md:26, 68`） |
| **Devirtualizing Memory（DVM）** | **已在目录且全文可读**（`dvm2018`） | `catalog.csv:31`，`fulltext_available=true`；旧地图 §0 注记（`:5`）说本轮已"重点补核 DVM" |
| **RMM** | **给定输入中检索不到**（既不在 123 项目录，也不在旧地图 §5/§6）。**不能据此断言该工作不存在**；"RMM" 可能指多篇不同工作，需外部消歧后再定 | `catalog.csv` 全表、`research-landscape-20261005.md` 全文均无匹配 |

**结论**：候选 4 的"必须超过的前作"清单里，Direct Segments 与 DVM **不是缺口**，是已有全文对手——`translation-opportunities-20261006.md:47` 已正确把 VBI/Utopia/EMT/DeACT 列为威胁，但**未同时点出 Direct Segments 与 DVM 已是本库全文对手**，措辞上容易让人误以为二者是新发现。RMM 需外部核验，暂列 P2 检索位。

---

## 6. B 态清单：旧地图已有、123 项未并入（必须先补齐，再谈新意）

这些**不需要取全文**（旧地图标注其中多数已有全文版本），而是需要**并入 123 项目录**，否则候选 1–4 的对手池有洞：

| 未并入条目 | 旧地图位置 | 与哪个候选相关 |
|---|---|---|
| **WIC**（warp 级中断替代轮询，隐藏 producer-consumer 同步） | §5 D 表 `:106` | **候选 1**（消费者完成/开工）——最相关 |
| **GPS**（多 GPU publish-subscribe，sys-scoped 写仍要单副本 + TLB shootdown） | §5 D 表 `:103`，§3 卡点 5 `:49` | **候选 1 + 候选 2** |
| **Tiara**（NIC 侧可验证 ISA 折叠远程 indirection） | §5 D 表 `:105`，§7.1 `:154` | 候选 1（注意基线是 2 RTT 不是 160 RTT） |
| **DX100**（可编程批量 indirection，明确不支持 pointer chasing） | §5 D 表 `:104` | 候选 1 |
| **NDS** | §5 D 表 `:102` | 候选 1（布局/搬运） |
| **UCP**（utility-based cache partitioning，按收益而非需求分配） | §5 B 表 `:79`；§3 卡点 2 `:40` | **候选 1、候选 3**（"按收益而非需求"= 关键性调度的祖先） |
| **KPart** | §5 B 表 `:80`；§3 卡点 2 `:40` | 候选 1（分区粒度与可预测性） |
| **DAE** | §5 A 表 `:66` | 候选 1（解耦地址计算与执行） |
| **MINISA** | §5 D 表 `:107` | 候选 4（控制 ISA，弱相关） |
| **PagedAttention / vTensor / GMLake** | §5 E 表 `:114, 117, 118` | 候选 2（动态分配/碎片/复用时机） |
| **Linux SVA/ENQCMD 规范** | §6 `:137` | **候选 4**（ECMDQ 的直接前身语义） |
| **RISC-V IOMMU queues / Linux HMM / CUDA stream-ordered allocator** | §6 `:134-136` | 候选 2、候选 4 |

**特别提示**：`research-landscape-20261005.md:79` 已经写了 UCP 是"utility-based cache partitioning，**按收益而非需求**分区"——这句话和候选 1（"仅按请求数量/本地等待分配会选错"）是**同一个命题的十五年前版本**。UCP 未并入 123 项，是当前最容易被外部审稿人直接指出的洞。

---

## 7. 不确定项与我未做的事

1. **所有非给定文件事实均标〔待外部核验〕**；G02/G11/G13/G14/G21/G22 的关键字段为 `null`，因为我的记忆不足以支撑作者/年/venue，**宁缺不造**。
2. **未伪造任何 URL**；仅引用给定文件中的 `catalog.csv`、`research-landscape-20261005.md`、`translation-opportunities-20261006.md`、`evidence.md`、`metadata-only.json` 的行号，以及 `:53` 中已被文档自身记录的那条 2024 LKML 链接（作为"文档已有线索"的事实，而非我提供的来源）。
3. **未执行任何命令、未修改任何文件、未运行测试**；本文全部为阅读与对照结果。
4. **G24 为方向性检索位**，不给具体题名——因为凭记忆列出"可能漏掉的近年 IOMMU/UVM/NDP 论文"极易编造，不符合本次审计的证据标准。
5. **架构判断留给 Codex**：本文只做"缺口定位 + 冲突点标定"，不判定候选 1–4 是否成立；尤其 G21（Dead on Arrival 2026）与 G24 的会场扩查结果，可能整体改写 P0 名单。