# G1_gpu 共享地址翻译文献证据报告

> 阅读范围：仅用户提供的 9 个全文文件（`gpummu2014 / mosaic2017 / mask2018 / walk_scheduling2018 / walk_stealing2021 / neighborhood2018 / valkyrie2020 / orchestrated2023 / star2024`）。
> 本报告不做联网检索，不把“本组未提供”推断为“领域没人做”。
> **未提供**：HDPAT、WaferTrans 全文在本批材料中不存在，因此无法提取其拓扑、迁移/失效假设；相关工作只能从本组论文的参考文献侧面看到名字（如 multi-GPU 的 IDYLL / Trans-FW / GRIT / Mitosis 仅在 star2024/orchestrated2023 的引文列表中出现，本组无全文）。
> 跨论文数字一律不横排比较；每条数字都标出其论文自定义基线。

---

## A. 时间线：问题与机制的演化

### A.0 全组速览

| 文件 | 准确题名（据题头） | 年份 | 出处（有证据者） | 评价形态 |
|---|---|---|---|---|
| gpummu2014 | Supporting x86-64 Address Translation for 100s of GPU Lanes | 2014（**来自同批论文参考文献**，本文件题头未标） | HPCA 2014（同上） | 全系统模拟（gem5-gpu） |
| mosaic2017 | Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes | 2017 | MICRO-50, 2017, DOI 10.1145/3123939.3123975 | 模拟（GPGPU-Sim+MAFIA） |
| mask2018 | MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency | 2018 | ASPLOS’18, DOI 10.1145/3173162.3173169 | 模拟 + 少量实机动机测量（K40/GTX1080） |
| walk_scheduling2018 | Scheduling Page Table Walks for Irregular GPU Applications | 2018 | ISCA 2018, pp.180–192, DOI 10.1109/ISCA.2018.00025 | 模拟（gem5 改造 + IOMMU 模型） |
| neighborhood2018 | Neighborhood-aware address translation for irregular GPU applications | 2018 | MICRO 2018, pp.352–363, DOI 10.1109/MICRO.2018.00036 | 模拟 + 面积/功耗分析（FabScalar） |
| valkyrie2020 | Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance | 2020 | PACT ’20, DOI 10.1145/3410463.3414639 | 模拟（MGPUSim）+ CACTI 面积/功耗 |
| walk_stealing2021 | Improving GPU Multi-tenancy with Page Walk Stealing | 2021 | HPCA 2021, pp.626–639, DOI 10.1109/HPCA51647.2021.00059 | 模拟（GPGPU-Sim/MASK 框架） |
| orchestrated2023 | Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs | **null**（题头/正文无年份；且引文含 HPCA 2024 条目，见下） | **null**（无出版信息） | 模拟（gem5-gpu） |
| star2024 | Improving Multi-Instance GPU Efficiency via Sub-Entry Sharing TLB Design | 2024 | 文件标注为 **arXiv:2404.18361v1, 29 Apr 2024**（preprint） | 实机动机测量（A100 MIG 共跑）+ 模拟（MGPUSim） |

**本组无任何 FPGA/ASIC 硬件原型**；“商用支持”只以厂商特性/白皮书形式被提及（UVA、HSA hUMA、CUDA streams、NVIDIA GRID、AMD FirePro/MxGPU、NVIDIA MPS、NVIDIA MIG）。

### A.1 阶段一（2014）：先把“GPU 侧 MMU 该长什么样、能不能兼容 CPU 页表”钉死

- 问题：GPU 需要每周期数百次翻译，且访存模式重吞吐不重局部性；当时的 GPU 虚拟内存与 CPU 不兼容，也没有公开评估。
- 机制：**per-CU 后置 coalescer 的私有 L1 TLB + 共享 32-way 高线程 PTW + 共享 page walk cache**，与 x86-64 页表完全兼容，4KB 页下平均 <2% 相对理想 MMU 的退化。
- 同时把“正确性”纳入：GPU 页错误（改 CPU IRET 微码 + GPU PF 寄存器）、GPU 参与 TLB shootdown（不能发起），并在 gem5-gpu 全系统 Linux 上跑通。

### A.2 阶段二（2017–2018 上半）：从“单地址空间”走向“多页大小 / 多地址空间”

- Mosaic（2017）把矛盾定位在**页大小折中**：地址翻译喜欢大页，demand paging 喜欢小页。解法是“基页传输 + 大页映射”，用连续性保持分配（CoCoA）+ 原地合并（In-Place Coalescer）+ 碎片压缩（CAC），合并不搬数据、不刷 TLB。
- MASK（2018）把矛盾定位在**多应用空间复用下的地址翻译干扰**：共享 L2 TLB 抖动、翻译请求与数据请求在 L2/DRAM 争用。给出 TLB-Fill Tokens、TLB bypass cache、翻译感知 L2 bypass、地址空间感知 DRAM 调度器，并第一次在本组内给出**内存保护**方案（per-core 页表根寄存器 + L2 TLB ASID）。

### A.3 阶段三（2018 下半）：把 IOMMU 的 walk 顺序与 walk 的空间局部性变成“一等公民”

- walk_scheduling2018：证明 **page walk 的服务顺序**本身能带来 30% 级差异（随机调度甚至更差），提出 SIMT-aware SJF + 同指令批处理。
- neighborhood2018：证明 **多个 walk 的 PTE 常常落在同一 64B cache line**（同一 neighborhood），可以在 IOMMU 里把并发 walk 的访存合并，减少 37% 页表访存、1.7× 加速。
- 二者正交、可叠加，且都把 PTW 摆在 CPU 侧 IOMMU（与 A.1 的“GPU 自建 MMU”形成路线分化）。

### A.4 阶段四（2020–2021）：把“共享资源”从被动争用变成显式管理与折中旋钮

- Valkyrie2020：把 L1-TLB 当作可互相借用的资源——LDT 记录页共享关系做定向预取，同 SE 内双向 ring 互探；1.95×。属于**单 GPU 内部的翻译复用**，不是多设备共享。
- walk_stealing2021：把共享 PTW 池做软分区 + 动态窃取（DWS/DWS++），并用 epoch/阈值把**吞吐 vs 公平**做成可调旋钮；指出单纯加 PTW 会加剧 L2 TLB 抖动。

### A.5 阶段五（2023–2024）：云多租户 / MIG 实例化后的“最后一级 TLB 仍是共享的”

- orchestrated2023：UVM 下按 thread block 量化翻译复用，做 TB 调度 + 基于 TBid 的 L1 TLB 分区 + 相邻 TB set 共享；12.5% 执行时间下降。
- star2024：指出 MIG 把计算/缓存/内存都分区了，**唯独 L3 TLB 仍然共享**；NVIDIA 的 L3 TLB 用 16 sub-entry 映射 1MB 对齐区间内 16 个 64KB 页，共享导致 sub-entry 在淘汰时大量未用；STAR 让一个条目的 sub-entry 被多个 base address 动态共享，A100 实机动机测量 + 模拟平均 30.2%。

### A.6 复用粒度 / 隔离前提的演化主线

- 翻译单元：4KB 页（2014、2018、2020、2021）→ 4KB 传输 + 2MB 映射（2017）→ 64KB 页 + 1MB 对齐 sub-entry（2024）。
- 共享对象：L2 TLB 条目（2014/2018 MASK）→ 页表 cache line（2018 neighborhood）→ L1-TLB 条目（2020）→ PTW 时间片（2021）→ L1 TLB set（2023）→ L3 TLB sub-entry（2024）。
- 隔离前提：**单地址空间 + CPU/GPU 一致性**（2014）→ **大页帧只属于单一保护域**（2017 软保证）→ **per-core 页表根 + ASID + 定向 flush**（2018）→ **tenantID 位**（2021）→ **MIG 实例硬隔离、但 L3 TLB 例外**（2024）。

### A.7 资源共享 / 映射共享 / SVA 的区分（本组总纲）

- **资源共享**（性能问题）：共享 L2/L3 TLB 容量与端口（mask2018、walk_stealing2021、star2024）、共享 PTW 池（walk_stealing2021）、共享 IOMMU walk 队列（walk_scheduling2018、neighborhood2018）、共享 L2/DRAM 与翻译请求争用（mask2018）。
- **映射共享**（同一翻译被多处复用）：同一应用多 CU 共享 L2 TLB（gpummu2014）；同 cache line 内多页 PTE 被一次 walk 带出（neighborhood2018）；同页表项被多个 L1-TLB 访问（valkyrie2020）；同一 TB 内部翻译复用（orchestrated2023）；同进程内 base address 共用 sub-entry（star2024）。
- **SVA（共享虚拟地址空间）**：gpummu2014 是 CPU–GPU 共享同一虚拟地址空间与页表的纲领性工作；walk_scheduling2018 / neighborhood2018 / walk_stealing2021 继承 HSA/SVM 前提，走 IOMMU + x86-64 页表；orchestrated2023 是 UVM 语义。**SVA ≠ 多设备共享同一份翻译**：本组只有“CPU 与单 GPU 共享页表”，没有多 GPU/多设备共享远端翻译的实测或设计。
- **安全/隔离**只有 mask2018（ASID + per-core 页表根 + 定向 flush）与 star2024（MIG 实例隔离，但 L3 TLB 例外）正面触碰；其余论文不做安全论证。

---

## B. 逐篇证据卡

### B1. gpummu2014

**元数据（题头 L1–L5）**
- 题名：Supporting x86-64 Address Translation for 100s of GPU Lanes
- 作者：Jason Power、Mark D. Hill、David A. Wood（University of Wisconsin–Madison）
- 年份/会议：**本文件题头无出版信息**。同批论文参考文献给出：mosaic2017 L1478–1479 “J. Power, M. D. Hill, and D. A. Wood, … in HPCA, 2014”；walk_scheduling2018 L1058–1061 “ser. HPCA ’14, 02 2014, pp. 568–578”；mask2018 L1480–1481 同。→ 记为 HPCA 2014（published），DOI 在全文与引文中均未见。
- 评价形态：**纯模拟**（gem5-gpu 全系统 + Linux），无实机、无 FPGA。

**机制（精确）**
- 三个数据驱动结论：①coalescer + scratchpad 是 TLB 带宽过滤器，平均把翻译速率降 6.8×（L20–22、L347–358）；②TLB miss 成簇，平均 60 个并发 walk、最大 >1000（L116–118、L399–410）；③post-coalescer TLB miss 率平均 29%、最高 67%（L23–24、L426–427）。
- 最终 Design 3：per-CU、位于 coalescer 之后的 L1 TLB；共享高线程 PTW（32 线程，L419–426）；共享 page walk cache（类 AMD，缓存非叶层，L499–514）。
- 关键配置（Table 1，L239–247；Table 2，L299–318）：16 CU @1.4GHz、32 lanes、L1 64KB、scratchpad 16KB、L2 1MB、DRAM 2GB DDR3 8 通道；Design 3 = 64 项 L1 TLB + 32-way PTW + 8KB PWC（共 16KB）；基线 = 理想 MMU（无限 TLB、1 周期 walk 命中）。
- 正确性：**首个在公开文献里正确实现 GPU 页错误与 TLB shootdown**（L519–527 “to our knowledge a first in public literature”）。minor fault 平均约 5000 周期（L576），按 TLB miss 一样 stall warp；major fault 建议抢占/checkpoint 重启（L582–588），未评估。shootdown：CPU 写 CR3 → 经 IPC 通知 GPU 刷全部 TLB；CPU 发起 shootdown → GPU 参与应答；**GPU 不能发起 shootdown**；CPU 核与 GPU kernel 一一对应（L563–581）。
- 备选设计：共享 L2 TLB 对 bfs/nw/sort 至少慢 2×（L626–632）；理想 PWC 只比 Design 3 快 1%（bfs 10%）（L614–628）；一前一后 TLB 预取 <1%（L653–658）；2MB 页除 gaussian 外 miss 降 >99%、miss 数降 >100×（L660–670），但为兼容性必须支持 4KB。

**复用粒度与前提**
- 粒度：4KB 页翻译；PWC 缓存非叶层。
- 前提：CPU/GPU 共享主内存与同一虚拟地址空间、CPU/GPU cache 保持一致（L137–139、L269–272）；页错误由 CPU 核上的 Linux 内核处理（需改 IRET 微码 + GPU PF 寄存器，L773–827）；GPU 页错误处理假设 GPU 进程仍在 CPU 核上运行，否则延迟更长。

**与“共享/SVA”的关系**
- SVA：本文就是 CPU–GPU 共享虚拟地址空间（“pointer-is-a-pointer”）的纲领性设计。
- 资源共享：同一应用多 CU 共享 L2、共享 PTW、共享 PWC（译：共享的是硬件资源，不是跨设备翻译）。
- 映射共享：同一应用的翻译在多个 CU 间复用（L2 TLB/PWC 命中）。

**评价配置与条件（不得与他文横比）**
- 基线：理想 MMU；结论“平均 <2%（L26–30、L134–138）”只对应该模拟设置与 Rodinia 子集 + database sort（缩小输入，L276–278）。

**限制 / 全文未覆盖**
- 全部为单地址空间、单应用集合的 GPU 负载，**完全没有多应用/多租户**（MASK 2018 才做）。
- 页错误与 shootdown 只做正确性实现，**没有定量性能评估**；major fault 方案未实现。
- 无 MIG/实例化、无动态映射迁移、无远端 PTW、无队列/尾延迟分析（只有并发 walk 数均值/最大）。
- 无实机测量、无商用硬件验证；“HSA hUMA 细节未公开也未评估”（L36–38）。

**对可能创新点的最强反证**
- 若创新点是“首次在 GPU 上做页错误处理 / TLB shootdown 参与”——本文 2014 已公开完成并声称公共文献首例（L519–527），反证极强。
- 若创新点是“GPU MMU 必须用奇异硬件”——本文证明常见 CPU MMU 思路组合即可（<2%），反证强。

**后续必追引文（全文内含）**
- Pichai/Hsu/Bhattacharjee ASPLOS 2014（4-port TLB + PTW 调度 + TLB-aware warp 调度，L742–743 邻近、L1497（））；Barr et al. ISCA 2010 translation caching；Bhattacharjee MICRO 2013 large-reach MMU caches；Vesely et al. ISPASS 2016（实机观察）；Zheng et al. HPCA 2016 paged memory for GPUs；Wong et al. ISPASS 2010（GPU 微架构反推 TLB）。

**证据（文件：`.../papers/gpummu2014.txt`）**
- L18–24：“(1) the coalescer and scratchpad memory are effective TLB bandwidth filters (reducing the translation rate by 6.8x on average), (2) TLB misses occur in bursts (60 concurrently on average), and (3) post-coalescer TLBs have high miss rates (29% average).”
- L26–30：“per-compute unit TLBs, a shared highly-threaded page table walker, and a shared page walk cache.”
- L134–138：“an average of less than 2% compared to an ideal MMU with an infinite sized TLB and minimal latency page walks”
- L519–527：“we correctly implement page faults and TLB shootdown in Linux 2.6.22 on top of gem5 full-system simulation, to our knowledge a first in public literature.”
- L565–581：“When the CR3 register is written on the CPU core that launched the GPU application, the GPU MMU is notified… The GPU cannot initiate a shootdown, only participate.”
- L665–666：“2MB pages reduce the TLB miss rate by more than 99%, resulting in more than 100 times fewer TLB misses.”

---

### B2. mosaic2017

**元数据（L1–L4、L28–L36、L61–L65）**
- 题名：Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes
- 作者：Rachata Ausavarungnirun、Joshua Landgraf、Vance Miller、Saugata Ghose、Jayneel Gandhi、Christopher J. Rossbach、Onur Mutlu
- 出处：**MICRO-50, October 14–18, 2017, Cambridge, MA, USA**；DOI **10.1145/3123939.3123975**；ACM ISBN 978-1-4503-4952-9 → published。
- 评价形态：**模拟**（MAFIA 框架 + GPGPU-Sim 3.2.2，L656–660）；PCIe 延迟是引用 NVIDIA GTX 1080 白皮书数据，不是自己实测（L294、L703–704）。

**机制（精确）**
- 核心观察：GPGPU 应用**en masse 分配**（一次分配一大批页），因此可以调整基页分配方式，使“虚拟连续 → 物理连续”，且大页帧只含单一保护域（L36–44、L127–136、L417–422）。
- 三个组件：①CoCoA（连续性保持分配，软保证单应用；free frame list + per-app free base page list，L485–499）；②In-Place Coalescer（分配后立刻合并，无需监视利用率、**无数据迁移、无需 TLB flush**，L503–536）；③CAC（大页内部碎片高时分裂 + 压缩，仅在同 DRAM 通道内迁移，L644–654、L390–393）。
- 页表结构改动：L3 PTE 加 large page bit，L4 PTE 加 disabled bit；设置 large page bit 后大页映射即可用，**不必等 L4 disabled bit 置位、不必刷 TLB**；分裂时必须刷 TLB 大页条目（L577–596、L632–635）。
- 数据通路：数据永远按基页（4KB）跨系统 I/O 总线传输，地址翻译用大页（2MB），因此不产生大页级别的超长 far-fault（L46–52、L281–324）。

**复用粒度与前提**
- 粒度：传输=4KB 基页；映射=2MB 大页帧；合并/分裂单位=大页帧。
- 前提：大页帧内基页必须 ①虚拟物理都连续 ②与虚拟大页对齐 ③同属一个虚拟地址空间/保护域（L398–412）；GPUSim 侧每 TLB 级分“基页表项集/大页表项集”两套（L251–255、L698–699）。
- 身份/权限：大页帧只允许来自同一应用（软保证），以避免合并/压缩违反内存保护（L446–451）。

**评价配置**
- 30 cores @1020MHz、GTO；L1 16KB；L1 TLB 128 基页/16 大页全相联；L2 2MB；L2 TLB 512 基页/256 大页；PTW 共享、最多 64 并发 walk；DRAM 3GB GDDR5 6 通道；235 个同构/异构多应用负载，工作集 10MB–2GB（L656–717）。
- 基线：GPU-MMU = 改自 Power et al.（用 512 项共享 L2 TLB 取代 PWC，因实测该选择平均好 14%，L286–294）。

**结果（严格带基线）**
- 相对 GPU-MMU：同构 +55.5%、异构 +29.7%（L180–182）；距 Ideal TLB（全命中）分别差 6.8%/15.4%（L20–22）。
- 214 个受 TLB reach 限制的负载上，L1/L2 miss 率降到 <1%（L1114–1116）。
- 纯 2MB 方案 vs 4KB：分配内存膨胀平均 40.2%、最坏 367%（L361–363）；带 demand paging 时 2MB 反而比 4KB 慢 92.5%（单应用）至 99.8%（5 应用）（L312–324）；2MB far-fault load-to-use 318µs vs 4KB 55µs（L306–307）。
- demand paging 开关对 weighted speedup 影响小（因为延迟只集中在 kernel 启动）（L1076–1084）。

**限制 / 全文未覆盖**
- 最坏情形建模：CAC 压缩被保守建模为“stall 整个 GPU 并刷流水线”（L704–707）。
- 作者承认**在评估负载中没观察到 CoCoA 软保证被破坏**，但长时运行可能引入碎片和内存膨胀（L1217–1223）。
- 单 GPU；**不涉及多租户共享 L3 TLB、多设备、远端翻译**；无实机/FPGA。
- 未覆盖动态页面迁移的失效协议（只有 TLB flush + 同通道迁移）；未覆盖页错误效率（far-fault 有讨论但只针对 2MB 情形）。

**对可能创新点的最强反证**
- “合并大页必然需要迁移数据 + 刷 TLB”——本文用 CoCoA 把反证做死（L532–533、L597–600）。
- “大页一定能提速”——本文用 demand paging 数字反驳（2MB 在带 paging 时严重变慢）。

**后续必追引文**
- Zheng et al. HPCA 2016（GPU paged memory，L1482–1483）；Agarwal et al. HPCA 2015（CC-NUMA GPU 带宽，L1341–1342）；Kwon et al. OSDI 2016 Ingens（L1389–1390）；Navarro et al. OSDI 2002（superpage，L1435–1436）；Cox & Bhattacharjee ASPLOS 2017 MIX TLB（L1391–1392）。

**证据（文件：`.../papers/mosaic2017.txt`）**
- L28–36：“R. Ausavarungnirun et al. Mosaic…In Proceedings of The 50th Annual IEEE/ACM International Symposium on Microarchitecture…(MICRO-50), 15 pages.”
- L36–44：“we observe that GPGPU applications present an opportunity to support multiple page sizes without costly data migration, as the applications perform most of their memory allocation en masse”
- L532–533：“Mosaic can perform coalescing in-place, i.e., base pages do not need to be migrated in order to be coalesced into a large page.”
- L597–600：“the lack of data migration and TLB flushes in Mosaic greatly reduces the time required for the coalescing operation…with respect to coalescing in existing MMUs.”
- L312–324：“for 4KB base pages, the demand paging overhead reduces performance…by an average of 40.0%…and 82.3%…with 2MB pages…92.5%…99.8%”
- L361–363：“the amount of allocated memory inflates by 40.2% on average, and up to 367% in the worst case, when we use 2MB pages”

---

### B3. mask2018

**元数据（L1–L4、L31–L36、L66–L70）**
- 题名：MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency
- 作者：Rachata Ausavarungnirun、Vance Miller、Joshua Landgraf、Saugata Ghose、Jayneel Gandhi、Adwait Jog、Christopher J. Rossbach、Onur Mutlu
- 出处：**ASPLOS’18, March 24–28, 2018, Williamsburg, VA, USA**；DOI **10.1145/3173162.3173169** → published。
- 评价形态：**模拟为主**（Mosaic 模拟器 = GPGPU-Sim 3.2.2 大改，L901–913、L944–951）+ **实机动机测量**（NVIDIA K40 与 GTX 1080 上测时间多路复用开销，L196–210）。

**机制（精确）**
- 定位：离散 GPU 的**多地址空间并发**；地址翻译是第一性性能问题（摘要 L11–19、L117–141）。
- 三个机制：①**TLB-Fill Tokens**：按 warp 发 token，只有带 token 的 warp 能填共享 L2 TLB；每 epoch（100K 周期）比较本 epoch 与上一 epoch 的 L2 TLB miss 率，>2% 加/减 token（L828–861）；②**TLB bypass cache**：32 项全相联，只有无 token 的 warp 能填；PTE 修改时全刷 TLB + bypass cache（L836–845）；③**Address-Translation-Aware L2 Bypass**：按页表层给请求打 3-bit page walk depth 标记，当某层地址翻译请求的 L2 命中率低于数据请求命中率时就 bypass 该层（L874–886）；④**Address-Space-Aware DRAM Scheduler**：Golden（翻译请求，小 FIFO）/Silver（被轮到的某应用数据请求）/Normal（其它应用）三队列，Golden > Silver > Normal（L895–915）；Silver 轮转配额由并发 walk 数与该 miss 阻塞 warp 数决定（L916–934）。
- 内存保护：per-core 页表根寄存器（类 CR3），页表根 cache 供 PTW 使用；改变页表根时保守排空在飞请求；L2 TLB 条目扩展 9-bit ASID；flush 只刷指定 core 的 L1 TLB 及 L2 TLB 中匹配 ASID 的条目（L688–710、L785–789、L1214–1216）。

**复用粒度与前提**
- 粒度：4KB 页翻译；页表层（4 级）粒度做 L2 bypass 决策。
- 前提：4 级页表；per-core 页表根寄存器；L2 TLB 512 项/16 路；L1 TLB 64 项 per core；PWC 16-way 8KB；PTW 共享最多 64 并发；6-bit 计数器只支持 ≤64 应用（L933–939 脚注）。
- 身份/权限匹配：ASID 唯一标识地址空间；flush 定向到 core + ASID；per-core 页表根保证不同空间用不同页表。

**评价配置**
- 30 cores、64 执行单元、1020MHz、64 线程/warp、GTO；L1 16KB；L2 2MB 16-way；DRAM GDDR5 1674MHz 8 通道；负载 27 个应用（CUDA SDK/Rodinia/Parboil/LULESH/SHOC），35 组双应用 → 0/1/2-HMR 三类（L917–1015）。
- 基线：Static（静态空间分区，模拟 GRID/FirePro）、PWCache、SharedTLB、Ideal。
- 指标：weighted speedup、max slowdown（不公平度）。

**结果（带基线）**
- 相对 SharedTLB（作者给出的最优基线）：weighted speedup +57.8%，IPC 吞吐 +43.4%，不公平度 -22.4%；距 Ideal 差 23.2%（L1130、L16–18）。
- Scalability（应用数 1→5，归一化到 Ideal）：SharedTLB 47.1/48.7/38.8/34.2/33.1% vs MASK 68.5/76.8/62.3/55.0/52.9%（L1126–1129）。
- 泛化：Fermi GTX480 上比 PWCache +46.9%、比 SharedTLB +29.1%；集成 GPU 配置下 +23.8%/+68.8%（L1150–1159）。
- 机制动机：单个 TLB miss 平均阻塞 >30 个 warp（64 warp/core）（L420–423）；翻译请求只占已用 DRAM 带宽 13.8%（最大带宽 2.4%），但平均 DRAM 延迟高于数据请求（FR-FCFS 偏向 row-buffer 命中）（L661–674）；4 级页表层 L2 命中率 99.8/98.8/68.7/1.0%（L646–655）。

**限制 / 全文未覆盖**
- **页错误与 TLB shootdown 明确留作 future work**（“Our design can be extended…We leave this as future work”，L889–893；shootdown 只引用他人技术 L894–899）。
- 计数器的多应用虚拟化（>64 应用）未做（L937–939）。
- 调度/分区用 oracle 静态核分区 + GTO，不是重点（L1005–1012）。
- 纯模拟；无 MIG/实例隔离；不涉及动态映射/迁移/远端 PTW/尾延迟（只有最大慢化/不公平度这一粗糙指标）。

**对可能创新点的最强反证**
- “多应用并发只靠加大 TLB 就行”——本文与 walk_stealing2021 都给出反证（本文 L1177–1182：8192 项时 MASK 与 SharedTLB 持平）；同时 L2 bypass/DRAM 调度说明“翻译请求应被视为独立流量类别”。
- “多应用下的 GPU 内存保护不需要新机制”——本文给出可落地保护方案（per-core 根 + ASID + 定向 flush），反证强。

**后续必追引文**
- Power et al. HPCA 2014；Pichai et al. ASPLOS 2014；Vesely et al. ISPASS 2016；Tang et al. PACT 2020 TLB compression；Yoon et al. ASPLOS 2018 virtual caching。

**证据（文件：`.../papers/mask2018.txt`）**
- L16–19：“MASK improves system throughput by 57.8%, improves IPC throughput by 43.4%, and reduces application-level unfairness by 22.4%.”
- L196–210：“We perform these experiments on real NVIDIA K40 [96, 97] and NVIDIA GTX 1080 [100] GPUs… time multiplexing between two processes increases the total execution time by 12%…to 10, the overhead…increases to 91%.”
- L661–674：“address translation requests consume only 13.8% of the total utilized DRAM bandwidth (2.4% of the maximum available bandwidth), their average DRAM latency is higher than that of data demand requests.”
- L785–789：“We extend each L2 TLB entry with an address space identifier (ASID). TLB flush operations target a single GPU core, flushing the core’s L1 TLB, and all entries in the L2 TLB that contain the matching address space identifier.”
- L889–893：“Address translation inevitably introduces page faults. Our design can be extended to use techniques from previous works…We leave this as future work.”
- L895–899：“Similarly, TLB shootdowns are required…Techniques to reduce TLB shootdown overhead…can be used with MASK.”

---

### B4. walk_scheduling2018（ISCA 2018）

**元数据（L1、L6–L9、L84–L85）**
- 题名：Scheduling Page Table Walks for Irregular GPU Applications
- 作者：Seunghee Shin、Guilherme Cox、Mark Oskin、Gabriel H. Loh、Yan Solihin、Abhishek Bhattacharjee、Arkaprava Basu（NCSU / Rutgers / AMD / IISc）
- 出处：**2018 ACM/IEEE 45th Annual ISCA**，DOI **10.1109/ISCA.2018.00025**，pp. 180–192 → published。
- 评价形态：**模拟**（execution-driven gem5，自建 GPU + IOMMU 地址翻译模型，L582–602）。

**机制（精确）**
- 观察 1：单个 SIMD 访存指令完成“全部翻译”所需访存数差异极大（1–16 vs 49–256 桶），适合“最短作业优先”（L291–309、L305–320）。
- 观察 2：同一 SIMD 指令的多条 walk 请求如果被别的指令插空，最后一条完成时间被推迟 2–3×（L376–399、L414–439）。
- 设计：SIMT-aware walk 调度器，两条规则——①优先调度“与最近一条已调度 walk 同一 SIMD 指令”的请求（时间批处理）；②否则调度 score 最低者（score = 该指令全部 walk 预计访存数，1–256）；每条 walk 请求附 **20-bit instruction ID**；IOMMU buffer 扩展该字段；含老化机制（阈值 2,000,000 个请求）（L414–465、L466–528）。
- 附带：PWC 加 2-bit 饱和计数器，避免替换掉有 pending 请求要用的条目（L529–544）。

**复用粒度与前提**
- 粒度：**一条 SIMD 指令所生成的全部 walk 请求**为一个调度单位；score 以“预计内存访问次数”计。
- 前提：IOMMU 走与 CPU 共享的 x86-64 四级页表（HSA/SVM，L178–202）；IOMMU 两级 TLB + 多 PTW（8–16）+ PWC 缓存前三级；walk 请求在 IOMMU buffer 排队，PTW 空闲时按 FCFS 取（L203–228）；score 在请求到达时估算，PWC 内容可能变化（L529–544）。

**评价配置**
- 2GHz、8 CUs、4 SIMD/CU、16 SIMD width、64 线程/wave；L1 32KB、L2 4MB；L1 TLB 32 项全相联、L2 TLB 512 项 16 路；IOMMU 256 buffer 项、8 PTW、IOMMU L1/L2 TLB 32/256 项、FCFS（L476–489）。
- 负载：6 个 irregular（XSB/MVT/ATX/NW/BIC/GEV）+ 6 个 regular（SSP/MIS/CLR/BCK/KMN/HOT）（L490–513、L640–665）。

**结果（带基线）**
- 相对 FCFS：irregular 应用平均 +30%（几何均值）、最高 +41%；regular 应用基本不变（L134、L640–665）。
- CU stall 周期平均 -23%（最高 -29%）（L665）。
- 首/末 walk 延迟差平均 -37%（L745–746）。
- walk 请求数（TLB miss 数）平均 -21%（最高 -30%）（L752–766）。
- 敏感性：L2 TLB 1024 项时 +25%；16 个 PTW 时 +8.4%；两者同时时 +5.3%（L773–795）；IOMMU buffer 128 项时 +13%，512 项时 +50%（L814–857）。

**限制 / 全文未覆盖**
- 纯模拟；内存足迹仅几百 MB；**明确不评大页**（仿真时间不可行，L828–830）。
- 不涉及多租户隔离、权限、ASID；不涉及跨设备失效；不涉及远端 PTW；不涉及尾延迟统计（只给平均/几何均值与少量最大）。
- score 是近似，PWC 动态变化会失准（L529–544）。

**对可能创新点的最强反证**
- “TLB/PTW 结构才是唯一战场，请求顺序无所谓”——本文用随机调度更差、SIMT-aware 更好（30%）直接反驳（L60–66、L242–279）。
- “FCFS 是自然的基线”也被反驳：随机调度可让应用慢 26%（L62–64）。

**后续必追引文**
- Vesely et al. ISPASS 2016（实机 GPU 翻译延迟）；Lowe-Power/Power et al. HPCA 2014（本文的 baseline 来源，L1058–1061）；Pichai et al. ASPLOS 2014（TLB-aware warp 调度）；Shin et al. MICRO 2018（同组 neighborhood 工作，后续）；Yoon et al. ASPLOS 2018（virtual caching）。

**证据（文件：`.../papers/walk_scheduling2018.txt`）**
- L16–38：“We discover that the order of servicing a GPU’s address translation requests (specifically, page table walks) plays a key role… We demonstrate that the reordering of translation requests… improves the performance of several irregular GPU applications by 30% on average.”
- L84–85：“2575-713X/18/$31.00 ©2018 IEEE 180 DOI 10.1109/ISCA.2018.00025”
- L203–228：“Upon a TLB miss, a page table walker walks an in-memory x86-64 page table… it typically selects a pending request from the IOMMU buffer in FCFS order.”
- L436–465：“each page walk request from the GPU is attached with an instruction ID (20 bits in our implementation)… assign an integer score (between 1 to 256…)”
- L640–665：“our scheduler speeds up irregular GPU applications by up to 41%, and by 30% on average (geometric mean)… there is little change in the performance of regular applications.”
- L828–830：“exorbitant simulation time prevents us from evaluating such large memory footprint.”

---

### B5. neighborhood2018（MICRO 2018）

**元数据（L1、L5–L9、L68–L69）**
- 题名：Neighborhood-aware address translation for irregular GPU applications
- 作者：Seunghee Shin、Michael LeBeane、Yan Solihin、Arkaprava Basu
- 出处：**2018 51st Annual IEEE/ACM MICRO**，DOI **10.1109/MICRO.2018.00036**，pp. 352–363 → published。
- 评价形态：**模拟 + 面积/功耗分析**（gem5 改造，FabScalar/CACTI 类分析，L582–594、L772–788）。

**机制（精确）**
- 定义：PTE 8B、cache line 64B → 同一 cache line 含 8 个 PTE，映射 8 个连续 4KB 页 → 叶级 neighborhood 是 32KB 对齐 VA 区；L2/L3/L4 级分别是 16MB/8GB/4TB 对齐区（L279–289、L340–346、L374–378）。
- 机制：PTW 每次读回一整条 cache line 后，扫描 IOMMU buffer，把所有落在同一 neighborhood 的 pending walk 一并完成（可跨级“部分合并”）；IOMMU buffer 条目加 **PL**（已合并到哪一级）、**PA**（下一级页表节点物理地址）、**CL**（合并位）；CL 置位的请求不启动 walk，避免重复访存（L383–460、L463–519）。
- 硬件：新增约 **1.5KB** 状态（含 PWST）；面积 0.048 mm²；每次读/写能量 0.041 nJ / 0.196 nJ；平均省 103.51 mW（L446–455、L777–791）。

**复用粒度与前提**
- 粒度：64B cache line（8 PTE）；叶级 32KB VA region，上级 16MB/8GB/4TB。
- 前提：PTW 按 cache line 粒度访存；x86-64 页表（并声称 ARM 类似，L291–293）；FCFS 基线；8 PTW、256 项 IOMMU buffer（L523–535）。

**评价配置**
- 2GHz、8 CUs、10 waves per SIMD-16、4 SIMD/CU、64 线程/wave；L1 32KB、L2 4MB；L1 TLB 32 项全相联、L2 TLB 512 项 16 路；IOMMU 256 buffer、8 PTW、32/256 IOMMU TLB、FCFS（L523–535）。
- 负载：MVT/ATX/NW/BIC/GEV（irregular）+ SSP/LUD/CLR/BCK/HOT（regular）（L536–579）。

**结果（带基线）**
- 相对无合并基线：页表访存 **-37%**；irregular 应用平均 **1.7×**、最高 **2.3×**（GEV）；regular 应用不退化（L42–43、L644–689）。
- 平均 page walk 延迟下降接近 **47%**，但作者明确说明“访存次数下降不一定让单条 walk 更快”（多 walker 利用率提高）（L673–689）。
- 敏感性：4→32 PTW 时仍有 ~1.3×（32 PTW）；IOMMU buffer 128/256/512 越大越好（L698–776）。
- 与“PTE 加数据 cache”替代方案比较：合并明显更优，因为一个 PTW 可服务多个请求、降低排队（L750–781）。

**限制 / 全文未覆盖**
- 纯模拟 + 分析；仅 5 个 irregular 应用。
- 假设没有软件/OS 修改（L786–787），因此**不涉及跨设备失效/权限**；不涉及多租户隔离；不涉及 TLB 容量问题；不涉及动态映射迁移。
- 与 walk_scheduling2018 同组、正交，作者自述可叠加（L793–812）。

**对可能创新点的最强反证**
- “页表访问必须一次一条、按请求独立”——本文用 cache-line 级合并直接反驳（L338–361）。
- “加一个 PTE cache 就够了”——本文用对比实验反驳（L750–781）。

**后续必追引文**
- Vesely et al. ISPASS 2016；Lowe-Power et al. HPCA 2014；Pichai et al. ASPLOS 2014；Shin et al. ISCA 2018（同组）；Yoon et al. ASPLOS 2018；Haria et al. ASPLOS 2018（identity mapping）。

**证据（文件：`.../papers/neighborhood2018.txt`）**
- L13–43：“many concurrent page walk requests… still fall on neighboring virtual page addresses… This simple scheme can reduce the number of accesses to the in-memory page table by 37% on average. This speeds up a set of GPU workloads by an average of 1.7×.”
- L68–69：“978-1-5386-6240-3/18/$31.00 ©2018 IEEE 352 DOI 10.1109/MICRO.2018.00036”
- L279–289：“Each page table entry in an x86-64 page table is 8-byte long; therefore, a typical 64-byte cache line contains eight PTEs… define such 32KB aligned virtual memory regions”
- L422–453：“The field PL contains the level of the page table up to which the given walk request has coalesced… a one-bit flag (CL)… In total, we add only around 1.5KB of additional state”
- L660–677：“full coalescing can speed up irregular workloads by around 1.7×, on average. The workload GEV speeds up by a significant 2.3×… page walk latency drops by close to 47%.”

---

### B6. valkyrie2020（PACT 2020）

**元数据（L1–L4、L70–L84）**
- 题名：Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance
- 作者：Trinayan Baruah、Yifan Sun、Saiful A. Mojumder、José L. Abellán、Yash Ukidave、Ajay Joshi、Norman Rubin、John Kim、David Kaeli
- 出处：**PACT ’20, October 3–7, 2020, Virtual Event, GA, USA**；DOI **10.1145/3410463.3414639** → published。
- 评价形态：**模拟 + 面积/功耗估算**（MGPUSim，AMD GCN3 建模；CACTI，L486–497、L691–694）。

**机制（精确）**
- 观察：GPU 内不同 L1-TLB 之间页共享度可高至 99%；约 60% 的 L1-TLB miss 能在另一个 L1-TLB 找到（同 SE 内约 33%–35%）（L150–171、L278–283）。
- 机制 A（预取）：**Locality Detection Table（LDT）**，100 项全相联，52-bit VPN 哈希成 18-bit tag，每项 64-bit mask 标记访问过该页的 L1-TLB；L2-TLB 侧预取器按 mask 把翻译推给 **NPT** 个伙伴 L1-TLB；NPT 每 100K 周期按 L1-TLB 命中率置信度（0–3）加减 4（L314–371）。
- 机制 B（互探）：同一 SE 内 16 个 L1-TLB 用**双向 ring**互联，64-bit 宽；miss 时同时发主请求（TTL=15，全环）与次请求（TTL=4，反向），先回的胜出；每 hop 1 周期；16 项缓冲；本地 CU 与邻居请求 round-robin 仲裁；当 L2-TLB 平均延迟超过 150 周期才启用 ring（L373–439、L461–483）。
- 组合：L1-TLB 拆成 **104 项 + 24 项预取 buffer**，避免预取挤掉邻居可能命中的条目（直接预取进 L1-TLB 会让邻居命中潜力从 52% 降到 8%，拆分后为 37%）（L461–483、L568–585）。

**复用粒度与前提**
- 粒度：一条页翻译（page table entry）；LDT 记录“哪些 L1-TLB 访问过该页”。
- 前提：AMD GCN 层级（4 SE × 16 CU = 64 CU）；L1-TLB 128 项全相联 1 周期；L2-TLB 32 set × 16 way、10 周期；IOMMU 8 PTW、150 周期；**4KB 页**；探测只限同 SE（L440–457、L486–499）。
- 身份/权限：作者明确说预测是否“此刻在某个 L1-TLB 中”不可行，所以放弃预测、改用互探（L405–421）。

**评价配置与结果（带基线）**
- 基线：无预取、无互探（L128-项 L1-TLB）。
- 预取单独 +1.45×；互探单独 +1.65×；合并 +1.95×（几何均值）；若不拆 L1-TLB 则只有 +1.55×（L573–586、L620–626）。
- L1-TLB 命中率提升：预取 4×、互探 3.624×、合并 4.60×（相对基线）（L620–626）。
- 负载相关：FLW 更受益于互探（1.63× vs 1.17×）；SPMV 更受益于预取（3.55× vs 1.8×）；KMeans 合并后从 2.6× 降到 2.4×（合并有轻微破坏性干扰）（L586–619）。
- 敏感性：64 项 L1-TLB 时合并 +1.71×；L2-TLB 1024/2048/4096/8192 项时 +1.82/1.27/1.18/1.06×（8192 项时与基线差距仅 6%）；大页 8KB/16KB/64KB/2MB 时为 1.92×/1.89×/1.61×/1×（L627–650）。
- 面积/功耗：额外 0.06 W（R9 Nano 175W TDP 的 0.37%）（L679–695）。

**限制 / 全文未覆盖**
- 单 GPU 内部；**不是多设备共享翻译**，也没有 CPU 侧/IOMMU 侧的翻译复用方案（作者明确把 IOMMU 调度留给 Shin et al.）。
- 纯模拟；预取与互探互相干扰；大页使收益消失；需要在 L1-TLB 内固定预留 buffer；不涉及页错误/迁移/失效/权限；LDT 是 per-GPU 单一结构（非多设备）。

**对可能创新点的最强反证**
- “L1-TLB 之间做共享/借用的正确做法是预测”——本文用实验说明预测不可行（“prediction mechanisms have difficulty predicting whether the shared page is present at a particular instant”），并转向探测（L414–421），反证强。
- “直接预取进 L1-TLB 就好”——本文用邻居命中潜力 52%→8% 反证（L461–483）。

**后续必追引文**
- Bhattacharjee & Martonosi ASPLOS 2010（CPU 侧 inter-core cooperative TLB prefetch）；Kandiraju & Sivasubramaniam ISCA 2002（distance prefetching）；Saulsbury et al. ISCA 2000（recency-based preloading）；Margaritov et al. MICRO 2019（prefetched address translation）；Ibrahim et al. PACT 2019（remote-core bandwidth）。

**证据（文件：`.../papers/valkyrie2020.txt`）**
- L57–58：“Valkyrie is able to achieve an average speedup of 1.95×, while adding modest hardware overhead.”
- L150–171：“there is a high degree of page sharing across the cores in a GPU… in many instances, whenever there is an L1-TLB miss, the same page table entry can also be found in another L1-TLB on the GPU (about 60% of the time).”
- L314–336：“we introduce the Locality Detection Table (LDT)… a finite-sized (100 entries in our implementation), fully associative table”
- L461–483：“the potential hit rate in neighboring L1-TLBs degrades to 8%… if we prefetch directly into the L1-TLBs… reserving 24 entries to serve as a separate prefetch buffer”
- L679–695：“Valkyrie introduces an additional 0.06 watts of power over the baseline. This constitutes a total of 0.37% of the 175W-TDP”

---

### B7. walk_stealing2021（HPCA 2021）

**元数据（L1、L6–L11、L87–L88）**
- 题名：Improving GPU Multi-tenancy with Page Walk Stealing
- 作者：B. Pratheek、Neha Jawalkar（同等贡献）、Arkaprava Basu（IISc）
- 出处：**2021 IEEE HPCA**，DOI **10.1109/HPCA51647.2021.00059**，pp. 626–639 → published。
- 评价形态：**模拟**（GPGPU-Sim，基于 MASK/MAFIA 框架，L222–226）。

**机制（精确）**
- 定位：多租户下，**共享 PTW 池的排队/交错**是独立且严重的瓶颈（不只是 L2 TLB 争用）；一个租户的 walk 会排在另一租户很多 walk 之后（L25–32、L355–382）。
- DWS：初始把 walker 均分给各租户；walker 空闲且其 owner 无 pending 时，才去偷别的租户的 walk；每个 walk 最多只等“一条别人的 walk”（L96–119、L436–461）。
- DWS++：允许在租户排队 walk 数差异超过阈值时也偷（DIFF_THRES），按 epoch（默认 200 walk）测各租户 walk 到达比例动态调整阈值；若刚偷过、或自身队列占用超过 QUEUE_THRES（默认 0.51），则禁止偷（L436–466、L543–553、L599–604）。
- 硬件：**tenantID**（2 租户 1 bit）；monolithic walk queue 均分为 per-walker 队列；EPOCH 计数器；FWA（每 walker 空闲槽位 + is_stolen 位）、TWM（tenantID→walker bitmap + PEND_WALKS + ENQ_EPOCH）、WTM（walker→tenantID）；默认配置（16 walker、2 租户、192 项队列）总状态 **192 bit**（L469–512）。

**复用粒度与前提**
- 粒度：一条 page walk 请求（工作窃取的单位）；walker 的软分区。
- 前提：租户数设计时固定（NVIDIA MPS 也限量）；tenantID 位数 =⌈log2 N⌉；TWM/WTM 可随租户增减更新（L565–603）；模拟以 2 租户为主，3–4 租户抽样（L196–209、L940–948）。
- 身份/权限匹配：每个翻译请求带 tenantID；per-walker 队列按 owner 归属；没有讨论 ASID/flush。

**评价配置**
- 30 SM、1137 MHz、GTO；L1 TLB 私有 32 项、12 MSHR；L2 TLB 共享 1024 项 16 路；**16 个 PTW、192 项 walk queue**；PWC 128 项；L2 2MB；DRAM 16 通道（L163–172）。
- 负载：MM/HS/RAY/FFT/LPS/JPEG/LIB/SRAD/3DS/BLK/QTC/SAD/GUPS（按 L2 TLB MPMI 分 Light/Medium/Heavy），组合成 45 个工作负载（L173–176、L210–226、L268–289）。

**结果（带基线）**
- DWS 相对 baseline：总 IPC（总吞吐）平均 **+37%**（对 32 个 VM 敏感负载 **+55%**）；weighted IPC **+15%**（L31–32、L127–128、L611–612）。
- DWS++ 吞吐 +34%、公平性优于 DWS（L661–678）。
- 与替代方案：**静态分区反而比 baseline 差**（“Thus, stealing is key”）；DWS 比 MASK 高 **29%**（原文说明这是在“更现实的基线（含 PWC、1024 项 L2 TLB、有限 walker）”下，MASK 原文的收益被高估）（L867–875）。
- 交错：baseline 下 HL 类平均每条 walk 要等 **42** 条别人的 walk，DWS 后降到 ~0.225（L696–733、L735–759）。
- 敏感性：增加 walker 或加大 TLB 后 DWS 收益下降但仍有意义；HH 类只有在 TLB 变大+walker 变多时才有明显收益（L895–938）。
- 大页：64KB 页下 DWS 仍有效（L956–966）。
- 扩展：3–4 租户抽样中 14 个负载里 10 个达到 1.25× 以上、最高 1.9×（L940–954）。

**限制 / 全文未覆盖**
- 纯模拟；DWS 可能损公平（DWS++ 以吞吐换公平）；HH 类资源完全受限时无解；**不涉及页错误/shootdown**；**不涉及跨设备共享翻译**（只讨论 GPU 内多租户）；不涉及远端 PTW；不涉及尾延迟指标（用平均/加权/吞吐）。

**对可能创新点的最强反证**
- “多租户下只要静态分区/加大 PTW 数就行”——本文用两张图反证：静态分区比 baseline 差；只加 PTW 会加剧 L2 TLB 抖动（L307–309、L1044–1048）。
- “MASK 已经解决多租户 TLB 问题”——本文在更现实基线下给出 29% 差距，说明仍有空间（L867–875）。

**后续必追引文**
- MASK（ASPLOS 2018）；Vesely et al. ISPASS 2016；Shin et al. ISCA 2018 / MICRO 2018；NVIDIA MPS、MIG 文档；Yoon et al. ASPLOS 2018；Cong et al. HPCA 2017。

**证据（文件：`.../papers/walk_stealing2021.txt`）**
- L25–32：“We show that shared page walkers are a key bottleneck under multi-tenancy… Over today’s design, we improve throughput by 37%, and weighted IPC by 15%, on average, over 45 workloads.”
- L87–88：“2378-203X/21/$31.00 ©2021 IEEE 626 DOI 10.1109/HPCA51647.2021.00059”
- L112–119：“DWS enables a page walker to steal a walk (work) from another walker owned by a different tenant. A walker can steal a walk only if no page walk request is pending from its owner tenant… A walk from a tenant may have to wait for at most one walk from another tenant”
- L391–405：“Since TLB entries are ultimately populated by results of walks, controlling the share of walkers among the tenants leads to similarly controlled sharing of L2 TLB entries.”
- L867–875：“DWS outperforms MASK by 29%, on average… Our baseline, unlike in [5], includes a page walk cache and larger L2 TLB (1024 vs. 512 entries). Importantly, authors simulated unlimited number of walkers.”
- L1044–1048：“static partitioning degrades performance over baseline. Thus, stealing is key.”

---

### B8. orchestrated2023（题头无出处信息）

**元数据（L1–L5）**
- 题名：Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs
- 作者：Bingyao Li、Yueqi Wang、Xulong Tang（University of Pittsburgh）
- 年份/出处：**题头与正文没有任何出版信息、DOI、年份**。⚠️ 关键不确定性：本文参考文献 [53] 引用 **2024 IEEE HPCA** 的 GRIT（`L1788–1791`），[29] 引用 MICRO 2023、[30] 引用 HPCA 2023 → 本文成文不早于 2024。因此用户给的 stem “orchestrated2023”与文件内证据**冲突**，不能据此断定年份或会议；JSON 中 year/venue/doi 记为 null，publication_status = uncertain。
- 评价形态：**模拟**（gem5-gpu，L218–220；配置“取自此前已发表工作”，L220–222）。

**机制（精确）**
- 观察：10 个 GPU 负载中，**翻译复用主要发生在 TB 内部，而不是 TB 之间**；且因 inter-TB 干扰，intra-TB 复用距离普遍超过 L1 TLB reach（64 项）（L21–23、L534–578）。
- 三个优化：①**thrashing-aware TB 调度**：TB 调度器维护 16 项表 `<TLBhits, TLBtotal>`，优先把新 TB 放到瞬时 L1 TLB miss 率低的 SM，资源不足则回退默认调度（不限制并行度）（L601–635）。②**基于 TBid 的 L1 TLB 分区**：用 TBid 索引 TLB set，条目存**完整 VPN**而非仅 tag；分区后同 SM 并发 TB 落在不同 set（L636–655、L648–650）。③**动态相邻 set 共享**：每 TB 1-bit sharing flag（共 16 bit），当某 set 过载而相邻 set 有空位则替位并把 flag 置 1，查找时按 flag 决定是否多查一个 set（L770–816）。
- 硬件开销：TB 状态表 136 字节（16×(4-bit SM id + 两个 32-bit 计数器)）（L817–830）。

**复用粒度与前提**
- 粒度：thread block（TB）粒度的翻译复用与 TLB set 归属；TLB 条目以整 VPN 寻址。
- 前提：TB 一旦调度到 SM **不可迁移**；每 SM 并发 TB 数由编译期资源决定（最多 16）；TBid 在 SM 内唯一且可重用；1-bit sharing flag 足够；假设相邻 TB 共享为主（L629–635、L673–689、L807–816）。
- 身份/权限：TBid 只是硬件标识，不承载地址空间/权限语义；未讨论 ASID 或跨进程隔离。

**评价配置与结果（带基线）**
- 16 SM @1400MHz、5 级流水；每 SM 48KB shared、64KB RF、最多 2048 线程；L1 16KB 4-way；L2 128KB/partition、共 1536KB；GTO + Round-Robin TB 调度；**L1 TLB 64 项 4-way 1 周期（SM 私有）、L2 TLB 512 项 16-way 10 周期**；**8 个共享 PTW、500 周期**（L87–100）。
- 负载：10 个 UVM 负载（bfs/color/mis/nw/pagerank/3dconv/atax/bicg/gemm/mvt），输入为最大可用规模（footprint 0.72GB–107.48GB）（L66–85、L208–217）。
- 结果：三招合计平均 **执行时间 -12.5%**；调度单独 -2.3%；**单纯分区反而 +14.3% 执行时间**；对 atax/bicg/nw/mvt 分区有效（inter-TB 干扰重）；与 TLB compression 结合再 +10.4%；2MB 大页下只剩 -2.13%（L786–798、L823–826、L946–951）。
- 作者自述：nw 的 L1 TLB 命中率提升没有转化为性能（compute-bound）（L813–816）。

**限制 / 全文未覆盖**
- **无任何年份/会议证据**；仅仿真；TB 不可迁移；每 SM 并发 TB 数固定；1-bit flag 不做计数/阈值（作者列为 future work）；只考虑相邻 TB set 共享；大页收益小；未涉及页错误/迁移/失效/权限隔离/跨设备。

**对可能创新点的最强反证**
- “加大 TLB 容量是出路”——本文与 MASK 一样给出反面证据：加大 TLB 提高命中率但不可扩展，且增加命中延迟与 die 面积（L208–214）。
- “单纯做 TLB 分区就能解决干扰”——本文给出反证：单纯分区让平均执行时间 +14.3%（L796–798）。

**后续必追引文**
- Tang et al. PACT 2020（TLB compression，本文的对比对象，L993–996）；Mosaic；MASK；Li et al. MICRO-54 2021（multi-GPU sharing & spilling aware TLB，L965–967）；Li et al. HPCA 2023 Trans-FW（L1749–1753）。

**证据（文件：`.../papers/orchestrated2023.txt`）**
- L1–5：“Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs / Bingyao Li, Yueqi Wang, Xulong Tang / Department of Computer Science, University of Pittsburgh”
- L21–23：“there exists substantial translation reuse within TBs rather than across the TBs. Moreover, the inter-TB interference significantly enlarges the intra-TB translation reuse distances.”
- L86–100：“L1: 64 entries, 4-way, 1-cycle lookup latency, SM private / L2: 512 entries, 16-way, 10-cycle lookup latency, SMs shared / Page table walk 8 shared page table walker, 500-cycle latency”
- L796–798：“simple partitioning of the TLB degrades the L1 TLB hit rates and performance of most benchmarks. The average (geomean) execution time increases by 14.3% with simply TLB partitioning”
- L1788–1791：“[53] Y. Wang, B. Li, A. Jaleel, J. Yang, and X. Tang, ‘GRIT…’ in 2024 IEEE International Symposium on High-Performance Computer Architecture, 2024, pp. 1080–1094.”（据此本文不早于 2024）

---

### B9. star2024（arXiv v1；与任何会议正式版是否同一篇未证明）

**元数据（L1–L5、L11）**
- 文件内题名：**Improving Multi-Instance GPU Efficiency via Sub-Entry Sharing TLB Design**
- 作者：Bingyao Li、Yueqi Wang、Tianyu Wang、Lieven Eeckhout、Jun Yang、Aamer Jaleel、Xulong Tang（University of Pittsburgh / Ghent University / NVIDIA）
- 出版信息：文件带 **arXiv:2404.18361v1 [cs.DC] 29 Apr 2024**（L11）→ 记录为 preprint；**DOI 无**。
- ⚠️ **不能凭名称把本 arXiv 版等同于任何 MICRO’24 正式版**：全文没有任何会议/DOI/版权行；“STAR” 这个名字在文中出现（L29、L140、L812），但版本对应关系在文件内无法证实。
- 评价形态：**实机动机测量（A100 MIG）+ 模拟**。

**机制（精确）**
- 问题定位：MIG 分区了 SM、缓存、内存，但**L3 TLB 仍被所有实例共享**（引自反向工程工作 [60]）（L18–19、L59–62、L179–185）。NVIDIA 新 GPU 的 L2/L3 TLB 条目含 **16 个 sub-entry**，一一映射 1MB 对齐区间内 16 个 64KB 页；淘汰时 16 个 sub-entry 一起清零，共享干扰导致 sub-entry 在淘汰时大量未用（L20–30、L247–261）。
- STAR：让一个 16-sub-entry 的 TLB 条目被**多个 base address 动态共享**。原 4-bit sub-entry index 拆成 n-bit sub-entry index + (4−n)-bit **AIB（Address Identify Bit）**；本设计 n=3，即 2 个 base address 各 8 个 sub-entry（L804–824）。
- 布局：**sequential layout**（连续占用 → 前半给 base1、后半给 base2，layout bit=‘01’）与 **stride layout**（交错占用 → 偶/奇 index 分配，predefine stride=1，layout bit=‘10’）；初始 layout bit=‘00’，即非共享（L835–861）。
- 共享时机：仅当该 set 内所有 way 都已被占用、新地址到来时，检查某条目 ①已用 sub-entry <8 且 ②只有 1 个 base address → 允许共享；多候选时优先同进程、否则选利用率最低者；都不满足才回退 LRU（L876–894）。
- 动态回退：当某 base 的 8 个 sub-entry 全满且新请求放不下时，条目**退回独占**，另一 base 的元数据与 sub-entry 被驱逐、layout bit 复位（L895–899、L931–970）。
- 硬件：每条目 +50 bit（2-bit layout + 16-bit AIB + 30-bit 第二 base VPB + 2-bit v/d）；每 sub-entry 加 1-bit comparator；**面积 +1.4%**（22nm，CACTI 估算）（L963–993）。
- 4-base 变体：3-bit layout indicator（‘000’ 非共享；第一位表 base 数；后两位表布局），共 3.6% 面积（L1556–1596）。

**复用粒度与前提**
- 粒度：**L3 TLB 条目内的 sub-entry**（每 sub-entry 对应一个 64KB 页，区间为 1MB 对齐）。
- 前提：L3 TLB 跨实例共享；L1/L2 TLB 随 GPC/TPC 分区；页大小 64KB（MIG 默认）；2-base 共享时每 base 8 sub-entry；同进程优先配对（L876–890）；实例大小 3g/2g/2g 为主（L231–246）。
- 身份/权限：共享仅发生在 base address 之间，**不跨实例/进程建立翻译共享**；作者把它与 static partitioning 组合（“within individual processes”，L1681–1685）。

**评价配置与结果（带基线）**
- 基线：**共跑的 MIG 基线**（每应用在自己的实例里、共享 L3 TLB）；归一化到“同实例大小但独占 L3 TLB”的单独运行（L45–46、L143–144、L302–304）。
- 实机动机测量：A100 上 MIG 3:2:2 / 3:3 共跑，出现最高 ~48%（W1）的性能下降（L45–47、L143–144、L460–464）。
- 仿真配置：SM 1GHz ×108；DRAM 5GB/slice；L1D 64KB 2-way；L2 2MB/slice 8-way；**L1 TLB 16 项 16-way（TPC 共享）、L2 TLB 128 项 8-way/16 sub-entry（GPC 共享）、L3 TLB 1024 项 8-way/16 sub-entry（GPU 共享，40 周期）**；8 PTW（GPC 共享，100 周期/级）；PWC 128 项（L225–246）。
- 结果：相对共跑基线平均 **+30.2%**（最高 +51.3%）；L3 TLB 命中率平均 +28.8%；sub-entry 平均利用率 +31.4%（L34–35、L1260–1289）。
- 4-base 共享：相对基线 +22.4%，但**比 2-base 低 7.8%**（地址冲突驱逐变多 + 最多 4 次顺序查找）（L1581–1596）。
- 对比：静态分区相对共享基线 **-7.9%**；STAR+静态分区 +14.2%；MASK+STAR 比单用 MASK +17.6%；8 sub-entry 的双 set/双 way 方案均劣于 STAR（L1615–1646、L1665–1685）。

**限制 / 全文未覆盖**
- **版本未定**：本文件是 arXiv v1，不能当作会议正式版内容。
- 仿真以 3 应用负载为主，4–6 应用只是抽样（L1597–1606）。
- 只共享同一实例/进程内部的 base address；不做跨实例翻译共享；不涉及页错误、迁移、失效传播、远端 PTW；不做安全/侧信道分析（虽引用“bootlegging”隔离论文）；对 L1/L2 TLB 沿用 MIG 现状。
- 依赖 [60] 的反向工程结论来描述 NVIDIA TLB 结构，作者自己没有独立复现 TLB 内部结构。

**对可能创新点的最强反证**
- “把 sub-entry 变成独立的普通 TLB 条目就能解决问题”——本文用 CACTI 估算反证：每 way 需 16 个 comparator，TLB 面积会变成基线的 **17.2×**（L777–802）。
- “共享得越多越好”——4-base 比 2-base 差 7.8% 反证（L1581–1596）。
- “静态给每个实例分 TLB way 就能消除干扰”——本文反证 -7.9%（L1667–1673）。

**后续必追引文**
- Zhang et al. CCS 2023 “Tunnels for Bootlegging: Fully Reverse-Engineering GPU TLBs…”（本文对 MIG L3 TLB 共享的关键依据，L1813–1815）；
- MASK（L1739–1743）；Mosaic（L1734–1738）；Pratheek et al. HPCA 2021（DWS，L1802–1804）；Shin et al. ISCA 2018（L1805–1808）；Li et al. MICRO 2021 / HPCA 2023 Trans-FW / IDYLL MICRO 2023 / GRIT HPCA 2024（multi-GPU 方向，本组无全文）。

**证据（文件：`.../papers/star2024.txt`）**
- L9–35：“NVIDIA’s Multi-Instance GPU (MIG) technology enables partitioning GPU computing power and memory into separate hardware instances… prior work identifies that MIG does not extend to partitioning the last-level TLB (i.e., L3 TLB)… STAR improves overall performance by an average of 30.2% across various multi-tenant workloads.”
- L11：“arXiv:2404.18361v1 [cs.DC] 29 Apr 2024”
- L20–23：“NVIDIA GPUs reorganized the TLB structure with 16 sub-entries in each L3 TLB entry that have a one-to-one mapping to the address translations for 16 pages of size 64 KB located within the same 1 MB aligned range.”
- L45–47：“we co-run representative GPU applications on an NVIDIA A100 GPU with MIG enabled… including (3g, 2g, 2g) and (3g, 3g)”
- L777–802：“such an expansion would result in a significant hardware cost… the TLB size is 17.2× of the baseline. This increase is impractical considering the constraints on GPU die size.”
- L990–993：“1.4% area overhead over the original L3 TLB assuming a 22 nm technology node.”
- L1581–1596：“4-base sharing improves performance by 22.4% over the baseline. However, it experiences a 7.8% performance reduction when compared to 2-base sharing.”

---

## C. 被证据支持的剩余瓶颈，与不能断言的点

### C.1 被证据支持（多篇、各自基线下）的剩余瓶颈

1. **共享 TLB 的容量/带宽/端口仍是第一性问题**（mask2018：多应用下 L2 TLB 抖动，8192 项才与基线持平；star2024：MIG 下 L3 TLB 是唯一未分区资源，sub-entry 淘汰时大量未用；valkyrie2020：L1-TLB 命中率可低至 99% miss、L2 94% miss 的场景）。这些都是论文内自报数字，基线各不相同，**不可横排**。
2. **page walk 的“顺序/批处理”本身是独立瓶颈**（walk_scheduling2018 30% 平均、41% 最高；neighborhood2018 37% 访存下降、1.7×）。两篇都属同组、正交，且都把 PTW 放在 CPU 侧 IOMMU。
3. **共享 PTW 池的排队/交错是独立于 TLB 的瓶颈**（walk_stealing2021：解除 PTW 争用比只解除 TLB 争用再带来 31% 吞吐 / 16% weighted IPC，baseline 为 1024 项 L2 TLB + PWC 的较现实配置）。
4. **翻译请求与数据请求在 L2/DRAM 的互相干扰**（mask2018：翻译只占已用带宽 13.8% 却延迟更高；需要独立队列/旁路）。
5. **地址翻译 vs demand paging 的页大小折中**（mosaic2017：单用大页在带 paging 时慢 92.5%–99.8%，并造成 40.2% 平均内存膨胀）。
6. **重复翻译/重复 walk 的浪费**（neighborhood2018 同一 cache line、valkyrie2020 跨 L1-TLB、orchestrated2023 跨 TB/TB 内复用距离、star2024 sub-entry 未用即淘汰）。
7. **扩容式解法被反复证伪**：mask2018（巨大 TLB 才追平）、walk_stealing2021（只加 PTW 会加剧 TLB 抖动）、orchestrated2023（单纯分区 +14.3%）、star2024（sub-entry 独立化 = 17.2× 面积）。

### C.2 不能断言 / 全文未覆盖（“没找到 ≠ 没人做”）

1. **动态映射/迁移/失效（invalidation）**：
   - 被覆盖的部分：mosaic2017 有合并/分裂时的 PTE 更新与“分裂才需 TLB flush”；gpummu2014 有 CR3 驱动的全 GPU TLB 刷与 GPU 参与的 shootdown（无性能量化）；mask2018 有 ASID 定向 flush 的**机制描述**（无评估）。
   - 未覆盖：跨设备/跨租户的失效传播协议、失效风暴、并发映射修改下的正确性证明；mask2018 明确把 page fault 与 shootdown 留作 future work。**因此不能断言“GPU 多租户/多设备环境下的动态重映射与失效已被解决”。**
2. **远端 PTW（remote page table walk）**：本组没有任何论文设计或评估“由另一设备/远端缓存/远端内存完成页表遍历”。walk_scheduling2018 只在相关工作中提到 Cong et al. 让 GPU 用 CPU PTW “在真实硬件上可能因长延迟不可行”（L890–894），这是对远端 PTW 的反向证据，不是实现。**不能据此推断远端 PTW 被实现或已被否决。**
3. **队列建模的深度**：gpummu2014（page walk queue 占用，最大 >1000）、walk_scheduling2018（IOMMU buffer 128/256/512 敏感性）、walk_stealing2021（per-walker 队列、FWA）、mask2018（Golden/Silver/Normal 队列）都有队列机制或敏感性，但都是**平均/聚合指标**。
4. **尾延迟（tail latency）**：本组**没有一篇论文以 p99/p99.9 等尾延迟为主指标**；最接近的是 gpummu2014 的最大并发 walk 数、mask2018 的最大慢化/不公平度、walk_stealing2021 的“一个租户被饿死”定性描述。**不能断言尾延迟已被覆盖或已被证明可忽略。**
5. **多设备（真·多设备共享同一翻译）**：本组全部是“1 CPU + 1 GPU（或多实例/多应用）”。没有多 GPU 共享远端翻译、CXL/PCIe P2P 共享页表、NVLink 远端地址翻译的实测。相关名称（IDYLL、Trans-FW、GRIT、Mitosis、multi-GPU）只出现在本组论文的参考文献里，**没有全文，不能当作证据**。
6. **安全/隔离属性**：mask2018 提供保护（ASID、per-core 根、定向 flush）但无攻击/侧信道分析；star2024 依赖 MIG 硬隔离却在 L3 TLB 上共享，没有安全评估。**不能断言共享 TLB 结构具备隔离/抗侧信道性质。**
7. **跨论文数字不可排榜**：例如“57.8%”（mask2018 vs SharedTLB 基线，模拟 27 应用/35 对）与“30.2%”（star2024 vs MIG 共跑基线，含 A100 实测动机）测量的东西、基线和硬件前提完全不同；同理“1.7×/1.95×/12.5%/37%”都只在各自基线内成立。**禁止直接排序或写“最优”。**
8. **HDPAT / WaferTrans**：本批材料未提供其全文，**无法提取拓扑、迁移或失效假设**；本报告对其只字不推断。

### C.3 对本组“可创新点”的最强反证汇总（跨篇）

- “GPU 页错误 + shootdown 无人做” → gpummu2014 已做（模拟全系统、称公共文献首例）。
- “合并大页必须迁移” → mosaic2017 已反证。
- “多租户/多应用只靠加大 TLB” → mask2018、walk_stealing2021、STAR 均反证。
- “page walk 顺序无所谓 / 只能改 TLB 结构” → walk_scheduling2018、neighborhood2018 均反证。
- “L1-TLB 之间用预测来共享” → valkyrie2020 反证（预测不可行，改探测）。
- “静态分区/增加 PTW 数就能解决多租户争用” → walk_stealing2021、orchestrated2023、star2024 均反证。
- “sub-entry 独立化成普通 TLB 条目” → star2024 反证（17.2× 面积）。
- “共享越多越好” → star2024 4-base vs 2-base 反证。

---

```json
{
  "papers": [
    {
      "id": "gpummu2014",
      "title": "Supporting x86-64 Address Translation for 100s of GPU Lanes",
      "authors": ["Jason Power", "Mark D. Hill", "David A. Wood"],
      "year": 2014,
      "venue": "HPCA 2014（本文件题头无出版信息；出处仅据同批论文参考文献：mosaic2017 L1478-1479、walk_scheduling2018 L1058-1061、mask2018 L1480-1481）",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "为 GPU 设计 x86-64 兼容 MMU：coalescer 之后的 per-CU 私有 L1 TLB + 共享 32-way 高线程 PTW + 共享 page walk cache；4KB 页下平均 <2% 相对理想 MMU 退化；并首次公开实现 GPU 页错误与 TLB shootdown 参与。",
      "assumptions": [
        "CPU 与 GPU 共享主内存、同一虚拟地址空间，且 CPU/GPU cache 保持一致（类 HSA/hUMA 前身）",
        "GPU 页错误复用 CPU 核上的 Linux 内核处理，需改 IRET 微码并加 GPU PF 寄存器",
        "CPU 核与 GPU kernel 一一对应；shootdown 只能由 CPU 发起，GPU 仅参与",
        "以 GPU-only Rodinia 子集 + database sort 作为未来异构应用 GPU 部分的代理，输入规模被缩小",
        "基线为理想 MMU（无限 TLB、1 周期 walk）"
      ],
      "limitations": [
        "仅单地址空间/单应用集合，完全没有多应用或多租户并发",
        "纯 gem5-gpu 全系统模拟，无实机测量、无 FPGA/ASIC 原型",
        "页错误与 TLB shootdown 只做正确性实现，未做性能量化；major page fault 依赖抢占/checkpoint，未评估",
        "不覆盖 MIG 实例化、动态映射迁移、远端 PTW、尾延迟"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt", "location": "L1-L5", "claim": "题名与作者：Supporting x86-64 Address Translation for 100s of GPU Lanes；Jason Power, Mark D. Hill, David A. Wood, University of Wisconsin–Madison；题头无年份/会议/DOI。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt", "location": "L18-L30", "claim": "三条发现（coalescer/scratchpad 使翻译速率降 6.8x；平均 60 个并发 walk；post-coalescer TLB miss 率平均 29%）与最终设计（per-CU TLB + 共享高线程 PTW + 共享 page walk cache），4KB 页平均 <2% 相对理想 MMU 退化。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt", "location": "L239-L247, L299-L318", "claim": "Table 1/2 配置：16 CU@1.4GHz、L1 64KB、L2 1MB、DRAM 2GB DDR3；Design3=64 项 L1 TLB+32-way PTW+8KB PWC（共 16KB）；基线=理想 MMU。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt", "location": "L519-L527", "claim": "在 gem5 全系统 Linux 上正确实现 GPU 页错误与 TLB shootdown，声称公共文献首例（'to our knowledge a first in public literature'）。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt", "location": "L565-L581", "claim": "shootdown 语义：CPU 写 CR3 触发 GPU TLB 刷新；GPU 只能参与、不能发起；CPU 核与 GPU kernel 一一对应。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpummu2014.txt", "location": "L660-L670", "claim": "2MB 大页除 gaussian 外使 TLB miss 降 >99%、miss 数降 >100×，但为兼容性仍必须支持 4KB。"}
      ]
    },
    {
      "id": "mosaic2017",
      "title": "Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes",
      "authors": ["Rachata Ausavarungnirun", "Joshua Landgraf", "Vance Miller", "Saugata Ghose", "Jayneel Gandhi", "Christopher J. Rossbach", "Onur Mutlu"],
      "year": 2017,
      "venue": "MICRO-50 (The 50th Annual IEEE/ACM International Symposium on Microarchitecture), Oct 14-18 2017, Cambridge, MA, USA",
      "doi": "10.1145/3123939.3123975",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["simulation"],
      "mechanism": "应用透明的多页大小 GPU 内存管理器：CoCoA 连续性保持分配（大页帧只属单一保护域）+ In-Place Coalescer（分配后原地合并，无数据迁移、无需 TLB flush）+ CAC（碎片高时分裂并在同通道内压缩迁移）；数据按 4KB 基页传输、地址翻译用 2MB 大页。",
      "assumptions": [
        "GPGPU 应用 en masse 分配（一次申请大块虚拟内存）",
        "大页帧内基页必须虚拟物理都连续、与虚拟大页对齐、同属一个保护域（软保证）",
        "页表改造：L3 PTE 加 large page bit，L4 PTE 加 disabled bit；large page bit 置位后大页映射即可用，不必等 L4 disabled bit 也不必刷 TLB",
        "仅当大页帧全满才合并；分裂时必须刷 TLB 大页条目",
        "GPGPU-Sim + MAFIA 模拟；PCIe 延迟取自 NVIDIA GTX 1080 白皮书数据"
      ],
      "limitations": [
        "纯模拟；CAC 用最坏情形建模（stall 所有 SM 并刷流水线）",
        "评估负载中未观察到 CoCoA 软保证被破坏，但长时运行可能引入碎片与内存膨胀（作者自述）",
        "单 GPU；不涉及多租户共享 L3 TLB、多设备共享翻译、远端 PTW",
        "压缩迁移被限制在同一 DRAM 通道内；in-DRAM bulk copy 为可选增强",
        "未覆盖页错误效率、失效传播协议"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt", "location": "L1-L4, L28-L36, L61-L65", "claim": "题名/作者/MICRO-50 2017 出版信息与 DOI 10.1145/3123939.3123975。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt", "location": "L36-L52, L148-L171", "claim": "核心观察（en masse 分配使虚拟连续可映射到物理连续）与三大组件 CoCoA / In-Place Coalescer / CAC。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt", "location": "L532-L536, L577-L600", "claim": "原地合并：不需迁移基页，不需要 TLB flush；L3 large page bit 与 L4 disabled bit 的设置顺序与语义。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt", "location": "L180-L182, L20-L22", "claim": "相对 GPU-MMU 基线，同构 +55.5%、异构 +29.7%；距 Ideal TLB 差 6.8%/15.4%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt", "location": "L306-L324, L361-L363", "claim": "2MB far-fault load-to-use 318µs vs 4KB 55µs；带 demand paging 时 2MB 反而慢 92.5%-99.8%；2MB 单独使用造成 40.2% 平均（最坏 367%）内存膨胀。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mosaic2017.txt", "location": "L656-L717", "claim": "模拟配置：30 cores@1020MHz、L1 TLB 128 基页/16 大页、L2 TLB 512 基页/256 大页、PTW 64 并发、235 个负载、工作集 10MB-2GB；CAC 最坏建模。"}
      ]
    },
    {
      "id": "mask2018",
      "title": "MASK: Redesigning the GPU Memory Hierarchy to Support Multi-Application Concurrency",
      "authors": ["Rachata Ausavarungnirun", "Vance Miller", "Joshua Landgraf", "Saugata Ghose", "Jayneel Gandhi", "Adwait Jog", "Christopher J. Rossbach", "Onur Mutlu"],
      "year": 2018,
      "venue": "ASPLOS 2018 (23rd ACM International Conference on Architectural Support for Programming Languages and Operating Systems), Williamsburg, VA, USA",
      "doi": "10.1145/3173162.3173169",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "measurement"],
      "mechanism": "多地址空间并发下的 GPU 内存层级重设计：TLB-Fill Tokens + 32 项 TLB bypass cache 抑制共享 L2 TLB 抖动；按页表层做翻译感知 L2 bypass；Address-Space-Aware DRAM 调度器（Golden/Silver/Normal 三队列优先翻译请求）；用 per-core 页表根寄存器 + L2 TLB 9-bit ASID + 定向 flush 提供内存保护。",
      "assumptions": [
        "4 级页表；per-core 页表根寄存器（类 CR3）与页表根 cache 供 PTW 使用",
        "L2 TLB 条目扩展 9-bit ASID；flush 只针对指定 core 的 L1 TLB 及匹配 ASID 的 L2 TLB 条目",
        "token epoch 100K 周期，miss 率变化 >2% 才调整 token；InitialTokens=80%，threshmax=500",
        "共享 PTW 最多 64 并发 walk；PWC 8KB 16-way；L1 TLB 64 项/core、L2 TLB 512 项",
        "6-bit 并发 walk 计数器，仅支持 ≤64 应用（作者列为未做）",
        "评价用 oracle 静态核分区 + GTO，调度非本文重点"
      ],
      "limitations": [
        "页错误与 TLB shootdown 明确留作 future work（仅引用他人技术）",
        "计数器虚拟化（>64 应用）未做",
        "纯模拟为主；实机测量仅用于时间多路复用开销的动机验证（K40/GTX 1080）",
        "不涉及 MIG/硬实例隔离、动态映射迁移、远端 PTW、尾延迟指标"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L1-L4, L31-L36, L66-L70", "claim": "题名/作者/ASPLOS'18 出版信息与 DOI 10.1145/3173162.3173169。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L16-L19", "claim": "相对 SharedTLB：系统吞吐 +57.8%、IPC 吞吐 +43.4%、不公平度 -22.4%；距理想 TLB 差 23.2%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L196-L210", "claim": "实机动机测量：NVIDIA K40 与 GTX 1080 上时间多路复用开销，2 进程 +12%，10 进程增至 91%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L661-L674", "claim": "翻译请求占已用 DRAM 带宽 13.8%（最大带宽 2.4%），但平均 DRAM 延迟高于数据请求，因 FR-FCFS 偏向 row-buffer 命中。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L688-L710, L785-L789", "claim": "内存保护：per-core 页表根寄存器、页表根 cache、L2 TLB ASID、只刷指定 core 与匹配 ASID 的条目。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L828-L861, L895-L915", "claim": "TLB-Fill Tokens 的 epoch/2% 调整规则、32 项全相联 bypass cache、Golden/Silver/Normal 三队列调度策略。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/mask2018.txt", "location": "L889-L899", "claim": "page fault 处理与 TLB shootdown 明确留作 future work，只引用他人技术。"}
      ]
    },
    {
      "id": "walk_scheduling2018",
      "title": "Scheduling Page Table Walks for Irregular GPU Applications",
      "authors": ["Seunghee Shin", "Guilherme Cox", "Mark Oskin", "Gabriel H. Loh", "Yan Solihin", "Abhishek Bhattacharjee", "Arkaprava Basu"],
      "year": 2018,
      "venue": "2018 ACM/IEEE 45th Annual International Symposium on Computer Architecture (ISCA), pp. 180-192",
      "doi": "10.1109/ISCA.2018.00025",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "IOMMU 内 SIMT-aware page walk 调度：优先服务同一条最近已调度 SIMD 指令的 walk（时间批处理），否则选 score 最低者（预计完成该指令全部翻译所需访存数，1-256）；每条 walk 请求携带 20-bit instruction ID；含防饿死老化机制与 PWC 2-bit 饱和计数器。",
      "assumptions": [
        "IOMMU 走与 CPU 共享的 x86-64 四级页表（HSA/SVM 前提）",
        "IOMMU 含两级 TLB、8 个 PTW、256 项 IOMMU buffer、PWC 缓存前三级；基线按 FCFS 取请求",
        "score 在请求到达时估算并用 PWC 状态近似，允许不精确",
        "老化阈值设为 200 万个请求",
        "PTW 以 cache line 粒度访存；负载为 OpenCL/HCC 不改动程序"
      ],
      "limitations": [
        "纯模拟（execution-driven gem5 + 自建 IOMMU 模型），无实机测量",
        "内存足迹仅几百 MB；明确因仿真时间不可行而未评估大页",
        "不涉及多租户隔离、ASID/权限、跨设备失效、远端 PTW、尾延迟",
        "score 为近似值，PWC 内容变化会使估算失准"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L1, L6-L10, L84-L85", "claim": "题名、作者与 ISCA 2018 版权/DOI 10.1109/ISCA.2018.00025。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L16-L38", "claim": "page walk 服务顺序决定翻译开销；提出 SJF 式优先级与同 SIMD 指令批处理，平均 +30%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L203-L228", "claim": "IOMMU 的 PTW 在 IOMMU buffer 中按 FCFS 取请求；IOMMU 走 in-memory x86-64 页表。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L436-L465", "claim": "硬件实现：每条请求 20-bit instruction ID、score 1-256、请求到达时扫描并重评分。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L640-L665", "claim": "结果：irregular 最高 +41%、平均 +30%（几何均值）；CU stall 周期平均 -23%；regular 应用基本无变化。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L745-L766, L814-L857", "claim": "首末 walk 延迟差 -37%；walk 请求数 -21%（最高 -30%）；IOMMU buffer 128 项时 +13%、512 项时 +50%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_scheduling2018.txt", "location": "L828-L838", "claim": "因仿真时间不可行而未评估大页；不规则应用大页收益有限。"}
      ]
    },
    {
      "id": "neighborhood2018",
      "title": "Neighborhood-aware address translation for irregular GPU applications",
      "authors": ["Seunghee Shin", "Michael LeBeane", "Yan Solihin", "Arkaprava Basu"],
      "year": 2018,
      "venue": "2018 51st Annual IEEE/ACM International Symposium on Microarchitecture (MICRO), pp. 352-363",
      "doi": "10.1109/MICRO.2018.00036",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "把 64B cache line 内 8 个 PTE 定义为一个 neighborhood（叶级 32KB、L2 16MB、L3 8GB、L4 4TB 对齐 VA 区）；PTW 读回 cache line 后扫描 IOMMU buffer，把同一 neighborhood 的所有 pending walk 立即一并完成（含跨级部分合并）；IOMMU buffer 条目加 PL/PA/CL 字段，CL 位阻止可被合并的 walk 启动。",
      "assumptions": [
        "PTE 8B、cache line 64B，故一个 neighborhood = 8 个连续 4KB 页（x86-64；声称 ARM 类似）",
        "PTW 以 cache line 粒度访存；FCFS 基线；8 个 PTW、256 项 IOMMU buffer",
        "不需要任何软件或 OS 修改",
        "附加硬件状态约 1.5KB（含 PWST）"
      ],
      "limitations": [
        "纯模拟 + 面积/功耗分析，仅 5 个 irregular 应用（MVT/ATX/NW/BIC/GEV）",
        "不涉及多租户隔离、权限、跨设备失效、动态映射迁移、远端 PTW",
        "以 x86-64 页表格式为前提；硬件面积/能量用工具估算而非流片",
        "访存数下降不必然使单条 walk 更快（作者自述）"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L1, L5-L9, L68-L69", "claim": "题名、作者与 MICRO 2018 版权/DOI 10.1109/MICRO.2018.00036。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L13-L43", "claim": "核心观察与结论：并发 walk 落在相邻页 → 同一 cache line；方案使页表访存 -37%，平均 1.7x 加速。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L279-L289, L340-L346, L374-L378", "claim": "neighborhood 定义与各级尺寸：叶级 32KB、L2 16MB、L3 8GB、L4 4TB 对齐 VA 区。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L422-L453", "claim": "IOMMU buffer 加 PL/PA/CL 字段 + PWST，总附加状态约 1.5KB；CL 位阻止可合并的 walk 启动。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L660-L677", "claim": "结果：平均 1.7x、GEV 2.3x；叶级与上级合并同等重要；平均 walk 延迟下降约 47%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L777-L791", "claim": "能量/面积：约 0.048mm²、每次读 0.041nJ/写 0.196nJ，平均省 103.51mW。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/neighborhood2018.txt", "location": "L786-L812", "claim": "不需要软件/OS 修改；与 page walk 调度（Shin et al. ISCA 2018）正交可叠加。"}
      ]
    },
    {
      "id": "valkyrie2020",
      "title": "Valkyrie: Leveraging Inter-TLB Locality to Enhance GPU Performance",
      "authors": ["Trinayan Baruah", "Yifan Sun", "Saiful A. Mojumder", "José L. Abellán", "Yash Ukidave", "Ajay Joshi", "Norman Rubin", "John Kim", "David Kaeli"],
      "year": 2020,
      "venue": "PACT 2020 (2020 International Conference on Parallel Architectures and Compilation Techniques), Virtual Event, GA, USA",
      "doi": "10.1145/3410463.3414639",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "利用 GPU 内 L1-TLB 间的页表项共享：LDT（100 项全相联、52-bit VPN 哈希为 18-bit tag、64-bit mask）记录哪些 L1-TLB 访问过该页，L2-TLB 侧预取器向 NPT 个伙伴 L1-TLB 预取；同一 SE 内 16 个 L1-TLB 通过双向 ring（主请求 TTL=15、次请求 TTL=4）互探，命中即返回；L1-TLB 拆为 104 项 + 24 项预取 buffer。",
      "assumptions": [
        "AMD GCN 层级：4 SE × 16 CU = 64 CU；L1-TLB 128 项全相联 1 周期；L2-TLB 32 set × 16 way 10 周期；IOMMU 8 PTW、150 周期",
        "4KB 页；LDT 全相联 100 项、18-bit 标签、FIFO 替换",
        "NPT 每 100K 周期按置信度（0-3）加减 4；ring 每 hop 1 周期，探测限同 SE",
        "当 L2-TLB 平均延迟超过 150 周期阈值才启用 ring 互探",
        "MGPUSim 建模，面积/功耗用 CACTI 估算"
      ],
      "limitations": [
        "单 GPU 内部机制，不是多设备共享翻译；不涉及 IOMMU/PTW 调度与跨设备失效",
        "预取与互探同时启用会互相干扰（KMeans 2.6x→2.4x）",
        "大页使收益消失（64KB 1.61x，2MB 1x）；L2-TLB 8192 项时与基线差距仅 6%",
        "需要预留 24 项 L1-TLB 作预取 buffer；未涉及页错误/迁移/权限"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L1-L4, L70-L84", "claim": "题名/作者与 PACT'20 版权/DOI 10.1145/3410463.3414639。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L57-L58, L128-L129", "claim": "平均 1.95x 加速；观察到 L1-TLB miss 率可高达 99%、L2 TLB 高达 93%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L150-L171", "claim": "L1-TLB 间页共享度可高达 99%；约 60% 的 L1-TLB miss 能在另一 L1-TLB 找到（同 SE 约 33-35%）。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L314-L349", "claim": "LDT 设计：100 项、18-bit 标签、64-bit mask、FIFO 替换、非关键路径。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L422-L439, L461-L483", "claim": "ring 互探 TTL=15/4；直接预取进 L1-TLB 使邻居命中潜力降到 8%，改为 104+24 拆分后回升到 37%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L573-L626", "claim": "结果：预取 1.45x、互探 1.65x、合并 1.95x；L1-TLB 命中率分别提升 4x/3.624x/4.60x。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/valkyrie2020.txt", "location": "L679-L695", "claim": "额外 0.06W，占 R9 Nano 175W TDP 的 0.37%。"}
      ]
    },
    {
      "id": "walk_stealing2021",
      "title": "Improving GPU Multi-tenancy with Page Walk Stealing",
      "authors": ["B. Pratheek", "Neha Jawalkar", "Arkaprava Basu"],
      "year": 2021,
      "venue": "2021 IEEE International Symposium on High-Performance Computer Architecture (HPCA), pp. 626-639",
      "doi": "10.1109/HPCA51647.2021.00059",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "多租户下对共享 PTW 池软分区 + 工作窃取：DWS 仅在 walker 空闲且其 owner 无 pending walk 时窃取他人 walk（每 walk 最多等一条别人的 walk）；DWS++ 在租户排队 walk 数差异超阈值（DIFF_THRES，按 epoch 内到达比例动态调）时也允许窃取，并用 QUEUE_THRES 与 is_stolen 位限制连续窃取。",
      "assumptions": [
        "租户数在设计时固定，但支持动态增删并重算分区（NVIDIA MPS 也限量）",
        "tenantID 2 租户 1 bit；默认 16 walker、2 租户、192 项队列，总状态 192 bit",
        "初始 walker 均分；epoch 默认 200 walk；QUEUE_THRES=0.51；DIFF_THRES 0.3-0.9",
        "共享 L2 TLB 1024 项 16 路、PWC 128 项、L1 TLB 32 项私有",
        "以 2 租户模拟为主，3-4 租户抽样验证"
      ],
      "limitations": [
        "纯模拟；不涉及页错误、shootdown、跨设备共享翻译、远端 PTW",
        "DWS 可能损公平（BLK.3DS 等）；DWS++ 以吞吐换公平",
        "HH 类完全资源受限时无解；TLB 严重抖动时收益有限",
        "用平均/吞吐/加权指标，未做尾延迟度量"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L1, L6-L11, L87-L88", "claim": "题名/作者与 HPCA 2021 版权/DOI 10.1109/HPCA51647.2021.00059。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L25-L32", "claim": "共享 page walker 是多租户关键瓶颈；DWS 平均吞吐 +37%、加权 IPC +15%（45 个负载）。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L96-L119", "claim": "DWS 规则：walker 仅在自身 owner 无 pending walk 时窃取；一条 walk 最多等一条他人 walk。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L163-L172", "claim": "基线配置：30 SM@1137MHz；L2 TLB 1024 项 16 路；16 walker、192 项 walk queue；PWC 128 项。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L469-L512", "claim": "硬件改动：tenantID、per-walker 队列、EPOCH 计数器、FWA/TWM/WTM，默认总状态 192 bit。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L696-L733", "claim": "交错量化：HL 类 baseline 平均每条 walk 等 42.291 条他人 walk，DWS 后降到 0.225。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/walk_stealing2021.txt", "location": "L867-L875, L1044-L1048", "claim": "DWS 比 MASK 高 29%（在更现实基线下）；静态分区比 baseline 差，说明窃取是关键。"}
      ]
    },
    {
      "id": "orchestrated2023",
      "title": "Orchestrated Scheduling and Partitioning for Improved Address Translation in GPUs",
      "authors": ["Bingyao Li", "Yueqi Wang", "Xulong Tang"],
      "year": null,
      "venue": null,
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "nearby",
      "evaluation_type": ["simulation"],
      "mechanism": "UVM 下按 thread block 做翻译复用感知的 TB 调度（优先放到瞬时 L1 TLB miss 率低的 SM）+ 基于硬件 TBid 的 L1 TLB 分区（用 TBid 索引 TLB set，条目存完整 VPN）+ 相邻 TB 动态 set 共享（每 TB 1-bit sharing flag）。",
      "assumptions": [
        "TB 一旦调度到 SM 不可迁移；每 SM 并发 TB 数编译期决定（最多 16）",
        "TBid 在 SM 内唯一且可重用于后续 TB；1-bit sharing flag 足以刻画共享",
        "翻译复用以 intra-TB 为主，inter-TB 共享主要发生在相邻 TB",
        "L1 TLB 64 项 4-way 1 周期、L2 TLB 512 项 16-way 10 周期、8 个共享 PTW 500 周期（配置取自此前已发表工作）",
        "gem5-gpu 仿真，10 个 UVM 负载，最大可用输入"
      ],
      "limitations": [
        "文件内没有任何年份/会议/DOI 信息；且引文包含 HPCA 2024 条目，与 stem '2023' 冲突，元数据无法定论",
        "纯模拟；单纯 TLB 分区反而使平均执行时间 +14.3%",
        "只考虑相邻 TB set 共享；1-bit flag 无计数/阈值（作者列为 future work）",
        "2MB 大页下收益仅 2.13%；不涉及页错误、权限隔离、跨设备共享翻译"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt", "location": "L1-L5", "claim": "题名与作者（Bingyao Li, Yueqi Wang, Xulong Tang, University of Pittsburgh）；题头无年份/会议/DOI。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt", "location": "L21-L23", "claim": "核心观察：翻译复用主要发生在 TB 内部而非 TB 之间；inter-TB 干扰放大 intra-TB 复用距离。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt", "location": "L86-L100", "claim": "基线配置：16 SM@1400MHz、L1 TLB 64 项 4-way、L2 TLB 512 项 16-way、8 个共享 PTW 500 周期。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt", "location": "L601-L655", "claim": "机制：thrashing-aware TB 调度（16 项 TLBhits/TLBtotal 表）与基于 TBid 的 TLB 分区，条目存整 VPN。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt", "location": "L786-L798, L823-L826, L946-L951", "claim": "结果：三招合计平均执行时间 -12.5%；调度单独 -2.3%；单纯分区 +14.3%；大页仅 -2.13%；与 TLB compression 结合再 +10.4%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/orchestrated2023.txt", "location": "L1788-L1791", "claim": "参考文献 [53] 引用 2024 IEEE HPCA 的 GRIT，说明本文成文不早于 2024，stem '2023' 与文中证据冲突。"}
      ]
    },
    {
      "id": "star2024",
      "title": "Improving Multi-Instance GPU Efficiency via Sub-Entry Sharing TLB Design",
      "authors": ["Bingyao Li", "Yueqi Wang", "Tianyu Wang", "Lieven Eeckhout", "Jun Yang", "Aamer Jaleel", "Xulong Tang"],
      "year": 2024,
      "venue": "arXiv preprint (arXiv:2404.18361v1 [cs.DC], 2024-04-29)；文件内无会议/DOI/版权信息",
      "doi": null,
      "publication_status": "preprint",
      "relevance": "core",
      "evaluation_type": ["measurement", "simulation"],
      "mechanism": "MIG 下 L3 TLB 仍跨实例共享；NVIDIA L3 TLB 每条目 16 个 sub-entry 对应 1MB 对齐区间内 16 个 64KB 页；STAR 允许一个条目被最多 2 个（或 4 个）base address 动态共享，按占用模式选择 sequential/stride 布局，用 layout bit 与 AIB 区分归属，并可在 shared/non-shared 间动态切换。",
      "assumptions": [
        "L3 TLB 跨 MIG 实例共享（依据反向工程工作 [60]）；L1/L2 TLB 随 TPC/GPC 分区",
        "L3 TLB 条目 16 个 sub-entry，每 sub-entry 对应 64KB 页、区间 1MB 对齐；MIG 默认页 64KB",
        "2-base 共享时每 base 8 个 sub-entry；多个 base 争夺同一 sub-entry index 时后来者替换先到者",
        "共享只在 base address 之间，同进程优先配对；不跨实例/进程建立翻译共享",
        "MGPUSim 改造后仿真；为保持持续竞争，早完成的负载会重启"
      ],
      "limitations": [
        "本文件是 arXiv v1，题名/内容是否等同于任何 MICRO 正式版在文件内无法证实，不得凭名称直接等同",
        "仿真以 3 应用负载为主，4-6 应用仅抽样；4-base 共享比 2-base 差 7.8%",
        "只共享 base address，不做跨实例翻译共享；不涉及页错误、迁移、失效传播、远端 PTW、安全/侧信道",
        "依赖 [60] 的反向工程结论描述 NVIDIA TLB 内部结构，作者未独立复现"
      ],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L1-L5, L11", "claim": "题名/作者/单位，以及 arXiv:2404.18361v1 [cs.DC] 29 Apr 2024 标记；无会议/DOI。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L9-L35", "claim": "MIG 未分区 L3 TLB；16 sub-entry 映射 1MB 区间内 16 个 64KB 页；STAR 平均提升 30.2%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L45-L47, L143-L144", "claim": "在真实 NVIDIA A100 上开启 MIG 共跑（3g,2g,2g 与 3g,3g）测动机性能下降。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L225-L246", "claim": "仿真配置：L1 TLB 16 项 16-way（TPC 共享）、L2 TLB 128 项 8-way/16 sub-entry（GPC 共享）、L3 TLB 1024 项 8-way/16 sub-entry（GPU 共享 40 周期）、8 PTW、PWC 128 项；页大小 64KB。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L804-L861", "claim": "STAR 格式：sub-entry index 拆分 + AIB；sequential（01）与 stride（10）布局；初始 layout bit=00 非共享。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L777-L802", "claim": "反证：把 sub-entry 独立成普通 TLB 条目会使 TLB 面积变为基线 17.2×，不可行。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L1260-L1289", "claim": "结果：平均 +30.2%（最高 51.3%）；L3 TLB 命中率平均 +28.8%；sub-entry 利用率 +31.4%；ST 在 W2 命中率 +52%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L1581-L1596", "claim": "4-base 共享相对基线 +22.4%，但比 2-base 低 7.8%（地址冲突驱逐增加、最多 4 次顺序查找）。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/star2024.txt", "location": "L1667-L1685", "claim": "静态分区相对共享基线 -7.9%；STAR+静态分区 +14.2%；MASK+STAR 比单用 MASK +17.6%。"}
      ]
    }
  ]
}
```

---

### 交付边界声明

1. 本报告只依据所列 9 个全文文件；**HDPAT / WaferTrans 未提供全文，未做任何拓扑、迁移、失效假设的提取或推断**。
2. 所有“年份/会议/DOI”均以文件内文或同批论文参考文献为准；`gpummu2014` 的会议/年份来自同批论文引文（已在 JSON 中标注），`orchestrated2023` 的年份/出处**无法确定**（其引文含 HPCA 2024），`star2024` 只能是 arXiv v1，**不得**按其题名与任何 MICRO 正式版直接等同。
3. 所有百分比/倍数都带各自基线与条件，**跨论文不可排榜**。
4. 本组没有任何 FPGA/硬件原型；实机测量只出现在 mask2018（K40/GTX1080 动机测量）与 star2024（A100 MIG 动机测量）。
5. “未在全文找到”只代表本组可支持范围到此为止，**不等于领域无人实现**。