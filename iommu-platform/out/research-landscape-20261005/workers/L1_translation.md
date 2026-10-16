# 地址翻译：从经典页表/MMU 缓存到 2026 前沿的文献地图报告

已读 `MAP_BRIEF.md` 与 `source-manifest.json`。本报告严格按“先理解领域、不先选方案”的要求组织；不设计新 RTL，不把 P/N 当研究中心，不自动转向安全，不把未验证想法写成已确认缺口。本上下文只附了部分论文全文；未附全文的 manifest 条目只作题录/URL 占位，不凭题名猜机制。全文行号来自所附 `papers/*.txt` 提取文本。

---

## 0. 阅读范围、边界与证据等级

### 0.1 本次实际有全文并建证据卡的论文

| 论文 | 文件 | manifest 源 URL 位置 |
|---|---|---|
| Translation Caching: Skip, Don’t Walk (the Page Table), ISCA 2010 | `papers/translation_cache2010.txt` | `source-manifest.json:15` |
| Efficient Virtual Memory for Big Memory Servers, ISCA 2013 | `papers/direct_segments2013.txt` | `source-manifest.json:33` |
| Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes, MICRO 2017 | `papers/mosaic2017.txt` | `source-manifest.json:44` |
| Supporting Address Translation for Accelerator-Centric Architectures, HPCA 2017 | `papers/accelerator_translation2017.txt` | `source-manifest.json:88` |
| Neighborhood-Aware Address Translation for Irregular GPU Applications, MICRO 2018 | `papers/neighborhood2018.txt` | `source-manifest.json:300` |
| SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators, arXiv 2020；正式会刊未确认 | `papers/sparta2020.txt` | `source-manifest.json:183` |
| Victima: Drastically Increasing Address Translation Reach by Leveraging Underutilized Cache Resources, 2023/arXiv 2024 v3；venue 需按最终出版核对 | `papers/victima2023.txt` | `source-manifest.json:343` |
| HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs, HPCA 2026 | `papers/hdpat2026.txt` | `source-manifest.json:318` |
| Revelator: Rapid Data Fetching via OS-Guided Hash-based Speculative Address Translation, 本地 2025 arXiv v1；MAP_BRIEF 称 2026 ISCA 有 DOI，但本地 v1 评估细节不能当最终版逐字核对 | `papers/revelator2025preprint.txt` | `source-manifest.json:334` |

### 0.2 未附全文、只作题录占位的 manifest 条目

这些不能建机制证据卡；若后续要判断“是否真缺口”，必须补全文再读：

| id | manifest URL 行 |
|---|---|
| dae1982 | `source-manifest.json:5` |
| ucp2006 | `source-manifest.json:55` |
| kpart2018 | `source-manifest.json:66` |
| mask2018 | `source-manifest.json:77` |
| walk_scheduling2018 | `source-manifest.json:99` |
| walk_stealing2021 | `source-manifest.json:117` |
| riommu2015 | `source-manifest.json:128` |
| iommu_scalable2015 | `source-manifest.json:139` |
| iotlb_wall2023 | `source-manifest.json:150` |
| bermuda2025 | `source-manifest.json:161` |
| impica2016 | `source-manifest.json:172` |
| gps2021 | `source-manifest.json:194` |
| bypassd2024 | `source-manifest.json:205` |
| dx1002025 | `source-manifest.json:216` |
| vattention2024 | `source-manifest.json:234` |
| vtensor2024 | `source-manifest.json:245` |
| gmlake2024 | `source-manifest.json:256` |
| minisa2026 | `source-manifest.json:267` |
| sharemmu2026 | `source-manifest.json:278` |
| tiara2026 | `source-manifest.json:289` |
| wic2025 | `source-manifest.json:355` |
| nds2021 | `source-manifest.json:365` |

### 0.3 必须分开的四条轴

`MAP_BRIEF.md:17-20` 已给出边界，我在本报告中严格分开：

1. **容量/覆盖范围**：TLB 条目数、页大小、多页合并、cache-backed TLB、memory-side partition TLB。
2. **依赖访问时延**：TLB miss 后的 page walk 串行依赖、walk cache 命中、speculative fetch、page walk queueing。
3. **物理放置与操作系统约束**：连续物理内存、direct segment、primary region、hash-based allocation、partition、inverted page table、CoW/demand paging/fragmentation。
4. **并行与互连扩展成本**：多 page-table walker、共享 IOMMU、GPU/wafer 多 GPM、peer caching、redirection table、NoC/PCIe/远程内存往返。

并且：CPU MMU、GPU GMMU、SoC IOMMU、PCIe ATS、远程应用间接表不能互相替代；TLB miss 不等于完整未缓存 page walk；TLB 之外还有 walk caches、合并、并发、数据缓存与内存仲裁；命中率、PTW 时延、应用吞吐不是同一指标。`MAP_BRIEF.md:17-19`

---

## 1. 演变因果线：经典问题 → 方法族 → 旧假设何时吃紧 → 2024–2026 前沿

### 1.1 经典问题：radix page table 的 TLB miss 与多级 page walk

经典出发点不是“TLB 命中率”本身，而是：现代 x86-64 等架构用多级 radix tree 存页表；TLB miss 后，硬件 page walker 要按层串行取 PTE，简单情况下每层一次内存访问。Barr 等 ISCA 2010 明确指出，x86 页表从 80386 到 x86-64 从两级增至四级；TLB miss 后无 MMU cache 时 walk 需要每层一次内存访问，应用内存足迹增大时 TLB miss 影响可从 5–14% 逼近 50%。`translation_cache2010.txt:10-22, 40-62, 115-174, 846-857`

这一定义决定了后续所有方法族的基本对象：

- 减少 TLB miss 次数：增大 TLB、增大页、合并页、TLB 预取、共享 TLB。
- 降低 TLB miss 代价：MMU/page walk caches、translation caches、page walk 调度、speculation。
- 绕过 TLB/page walk：direct segment、identity mapping、redundant memory mappings、hash page table。
- 改变物理放置：连续物理内存、partition、hash-based allocation、memory-side translation。

### 1.2 方法族一：MMU/page walk caches，把“多级依赖”改成“跳过若干级”

Barr 2010 的核心贡献是把 MMU cache 设计空间讲清楚：page table cache（按物理地址索引）与 translation cache（按虚拟地址片段索引）；unified/split/path。关键结论是：translation cache 可存“部分翻译”，命中后可跳过一级或多级；在同样条目数下，translation cache 比 page table cache 覆盖更大地址范围；unified translation cache 配合 modified LRU 在 16GB 数据库 join 中几乎达到 3×n split cache 的命中率。`translation_cache2010.txt:10-22, 222-322, 366-386, 408-426, 682-724`

但它的评估是 trace-driven：AMD SimNow + FreeBSD 8.0、SPEC CPU2006 FP、SPECjbb2005、Sweep3d；模拟器存 tag 不存 data；不测 power，不测真实应用吞吐。`translation_cache2010.txt:388-435, 443-450` 因此它能支持“CPU radix 页表下 MMU cache 设计空间”的结论，不能直接推出 IOMMU、GPU GMMU、PCIe ATS 的行为。

### 1.3 方法族二：大内存服务器下 TLB reach 不足，direct segment 绕过页表

当内存从 GB 级走向几十 GB/多 TB，TLB reach 追不上内存足迹。Basu 等 ISCA 2013 观察到 big-memory 服务器负载（数据库、内存缓存、图分析）在 96GB 机器上 graph500 4KB 页时 51% 执行周期用于 TLB miss，2MB 页仍约 10%；但多数内存是启动时大块分配的匿名读写区，不 swap，也很少用细粒度保护。`direct_segments2013.txt:12-48, 228-247`

方法族：direct segment。用 per-core base/limit/offset 寄存器，把一段连续虚拟地址直接映射到连续物理内存，segment 内不可能 TLB miss；segment 外仍走普通分页。代价非常明确：需要连续物理内存、primary region 统一读写权限、放弃 swap/细粒度保护/稀疏分配，OS 要管理寄存器、物理内存预留/compaction、shootdown；原型还不支持动态 resize。`direct_segments2013.txt:26-40, 123-137, 151-154, 306-348, 398-407, 541-553`

它把“容量/覆盖”问题转化为“物理放置 + OS 约束”问题：一旦连续物理内存不足，direct segment 就无法覆盖全部负载；一旦负载需要 CoW/swap/细粒度保护，就不能简单放入 segment。

### 1.4 方法族三：GPU/加速器统一虚拟地址，多页大小与 IOMMU 成为中心

GPU 场景把问题从 CPU TLB 扩展到 GPU MMU/GMMU/IOMMU、demand paging、多应用并发。

Mosaic MICRO 2017 指出 GPU 上有两条相互拉扯的线：

- 地址翻译希望**大页**：增加 TLB reach，减少 TLB miss。
- demand paging 希望**小页**：减少 PCIe 传输量，降低 fault 等待。
- 合并/拆页若需迁移 base page，代价很高；GPGPU 常 en masse 分配，才给“分配时保持连续”的机会。`mosaic2017.txt:9-44, 127-136`

Mosaic 的机制是 CoCoA + In-Place Coalescer + Contiguity-Aware Compaction：用 base page 传输，按 contiguity-conserving 策略分配，在满足条件时原地合并成大页；碎片高时拆回 base page 并 compact。评估靠 GPGPU-Sim/MAFIA 模拟，235 个同构/异构多应用负载，对 GPU-MMU baseline 平均提升 55.5%/29.7%。`mosaic2017.txt:45-52, 151-171, 396-504, 656-716` 但它不是 CPU OS，也不是真实 GPU 硅验证。

Accelerator-Centric HPCA 2017 则面对 customized accelerator：统一虚拟地址需要硬件翻译，但 naive IOMMU-only 只有 ideal 的 12.3%；IOTLB 缺低延迟和页局部性，IOTLB miss 后 walk 很慢。作者提出 per-accelerator 小 private TLB（16–32 项）、shared L2 TLB（512 项）、把 page walk offload 到 host core MMU；模拟 PARADE/gem5，25 类加速器，结果组合方案达到 ideal 的 93.6%，平均 page walk latency 58 cycles。`accelerator_translation2017.txt:8-45, 64-143, 267-349, 594-665, 731-734` 其假设是加速器访问正则、host core 可服务 walk；16 个 accelerator instance 时 host MMU/互连/内存开始成为瓶颈。`accelerator_translation2017.txt:823-899`

### 1.5 方法族四：不规则 GPU page walk，从“单次 walk 快”转向“并发 walk 吞吐”

Neighborhood-Aware MICRO 2018 针对 irregular GPU：SIMT 下同一 wavefront 可能访问不同页，TLB miss 并发爆量，IOMMU 的 page walker 队列出现显著排队；商业硬件上不规则 GPU 负载可因地址翻译慢 3.7–4×。`neighborhood2018.txt:13-60, 289-303`

它的机制不是扩大 TLB，而是利用页表布局：x86-64 一个 64B cache line 含 8 个 8B PTE，因此一个 walker 取一个 PTE 时也隐含取了同 neighborhood 的其他 PTE；IOMMU 在 buffer 中扫描 pending walk，若落在同一 neighborhood，就一起服务。结果显示 full coalescing 平均减少 page table 访问 37%，不规则 GPU 负载加速 1.7×，page walk latency 下降约 47%，且只需约 1.5KB 状态。`neighborhood2018.txt:32-43, 71-123, 383-458, 602-687, 698-787`

关键限制：作者明确说，减少 page table 访问不一定让单次 walk 更快，因为多个 page table walker 可能并行；coalescing 更多是提升 walker 吞吐、减少排队。`neighborhood2018.txt:659-687` 这正是“TLB miss ≠ 完整未缓存 page walk；命中率、PTW 时延、吞吐不是同一指标”的实证。

### 1.6 方法族五：内存侧翻译与 cache-backed TLB，把容量/覆盖推向 TLB 以外

SPARTA arXiv 2020 面对 area/power 受限的 accelerator：大 TLB 面积/功耗不可行；centralized IOMMU 离 accelerator 和分布式内存都远，walk 延迟更高。它把翻译拆成 accelerator-side tiny TLB（只覆盖 cache 命中）和 memory-side per-partition TLB/MMU；虚拟地址唯一决定 partition，数据和 PTE 同 partition，从而把 translation 与 data fetch overlap。Linux 4.10 原型，inverted page table，使用 NUMA 分配路径；声称保留 demand paging/CoW，只对 mapping 加 partition 约束。`sparta2020.txt:14-31, 186-242, 383-409, 526-600` 评估用真实硬件 perf、Pin trace、CPI model，128GB 数据集；SPARTA-32 平均 1.57× speedup，128 partitions + 2MB 页可接近 ideal。`sparta2020.txt:618-654, 798-826, 954-973` 但它是 arXiv，venue 未确认；性能不是 full-system simulation；per-partition TLB 共享会引入线程竞争。`sparta2020.txt:828-864`

Victima 2023/2024 换一条路：不扩大 TLB、不引入大软件 TLB，而是把 L2 cache block 复用为 TLB entry cluster，并用 PTW cost predictor 只保留“贵翻译”的页，配合 TLB-aware SRRIP。它观察到现代数据密集负载的 L2 block 平均 92% 无复用，所以可借用 cache 容量。评估 Sniper，11 个数据密集负载；native 提升 7.4%，virtualized 提升 28.7%，PTW 减少 50%，translation reach 220MB/36×，面积/功耗开销 0.04%/0.08%。`victima2023.txt:9-39, 70-121, 491-505, 558-598, 1042-1220` 但它仍是模拟 + McPAT 面积功耗估计，不是真实 RISC-V IOMMU/GPU。

### 1.7 2024–2026 前沿：wafer-scale 集中式 IOMMU 瓶颈 vs OS 引导的推测翻译

HDPAT HPCA 2026 面对 wafer-scale GPU：数百 GPM 通过 mesh 互连，非本地翻译都要到中央 IOMMU；中央 IOMMU 只有约 16 个 page table walker，导致大量请求排队，SPMV 中 pre-queue delay 是最大项，近 700 个翻译请求等待；理想 IOMMU 可带来 5.45×/4.96× 加速。作者还观察到中心 GPM 因离 IOMMU 近、网络跳数少而执行更快，形成几何不平衡。`hdpat2026.txt:12-44, 147-158, 181-201, 260-316`

HDPAT 的机制是三层：

1. concentric caching + rotation：按离 IOMMU 距离分层，内层 GPM 给外层做 translation provider；轮换保证任意方向附近都有缓存 GPM。
2. redirection table：IOMMU 把最近翻译/预取的 VPN 重定向到已持有 PTE 的 GPM。
3. prefetching：walker 解析 VPN N 时预取 N+1..N+3，并按访问频次选择性分发。`hdpat2026.txt:31-44, 439-591, 592-659`

评估 MGPUSim，48 GPMs、32 CU/GPM、mesh，14 个 benchmark；相对 baseline 平均 1.57×，offload 42.1% 翻译；prefetch accuracy 65.55%；remote translation round-trip 降 41%；NoC 仅增 0.82%；redirection table 0.034mm²、0.16W。`hdpat2026.txt:592-763, 824-928, 936-945`

Revelator 本地 2025 arXiv v1 / 2026 ISCA 发表记录见 `MAP_BRIEF.md:34`，针对的是**单系统 CPU 地址翻译延迟**和**VA→PA 不可预测**：传统 OS 允许虚拟页映射任意物理帧，碎片、分配顺序、多租户使 VA→PA 基本不可预测；SpecTLB 依赖大页/连续 VA-PA，在碎片下 miss >95%，64-entry 也不低于 80%。`revelator2025preprint.txt:6-48, 194-300, 387-443`

Revelator 机制：OS 用 tiered hash-based allocation，尝试 `PPN_i = Hash_i(VPN)`，失败才回退普通分配；硬件 speculation engine 按同一 hash 生成多个候选 PA，并过滤带宽/碎片后提前 fetch 数据和最后一级 PTE；page table frame 也可 hash 分配，从而在第三级之前 speculative fetch 第四级 PTE，打破 PTW 串行依赖。Linux v6.13 原型；Virtuoso/Sniper 模拟；11 个数据密集 benchmark；native 27%、virtualized 20% speedup，超 SpecTLB 5%，能耗降 9%，面积/功耗 0.01%/0.02%。`revelator2025preprint.txt:24-47, 72-105, 476-600, 618-750, 812-927, 932-940`

---

## 2. 逐篇证据卡

> 每卡内容控制在约 200 英文词等量内；题录不确定点已标出。

### Card 1 — Translation Caching: Skip, Don’t Walk (the Page Table)

- **题录/URL**：ISCA 2010，Barr/Cox/Rixner，Rice University；`source-manifest.json:15`。
- **针对问题**：x86-64 radix tree 页表 TLB miss 后多级 page walk 的内存访问和时延；AMD/Intel MMU cache 设计空间未系统比较。`translation_cache2010.txt:10-22, 24-38`
- **核心机制**：把 MMU cache 分为 page table cache（物理地址索引）与 translation cache（虚拟地址片段索引），并分 unified/split/path；提出 UPTC/SPTC/STC/UTC/TPC；translation cache 可跳过一级或多级。`translation_cache2010.txt:185-322, 366-386`
- **假设/代价**：假定 radix tree；translation cache 需要 CAM 匹配和层级优先级；unified 需 replacement policy；path cache 对上层条目有效容量低于 3× split。`translation_cache2010.txt:302-326, 393-435, 682-724`
- **评估**：AMD SimNow + FreeBSD 8.0；SPEC CPU2006 FP、SPECjbb2005、Sweep3d；trace-driven 内存系统模拟，1MB L2，64-entry L1 TLB + 512-entry L2 TLB；无真实功耗测量。`translation_cache2010.txt:388-459, 491-511`
- **最关键强基线**：no MMU cache、UPTC、SPTC、STC、TPC 头对头；替代格式含 inverted page table、TSB。`translation_cache2010.txt:491-511, 728-814`
- **不能从论文推出**：不能推出其结论适用于 IOMMU/GPU/PCIe ATS；不能推出“hit rate 提升等于应用吞吐提升”。作者自己显示无 cache 时 DRAM 访问仅从 0.15 到 0.14，收益主要来自 L2 延迟差而非 DRAM 命中率。`translation_cache2010.txt:496-511`
- **原文定位**：§3–§7；Fig. 3–11；Table 2–6。

### Card 2 — Efficient Virtual Memory for Big Memory Servers

- **题录/URL**：ISCA 2013，Basu/Gandhi/Chang/Hill/Swift；`source-manifest.json:33`。
- **针对问题**：big-memory 服务器负载在 page-based VM 上 TLB miss 成本高，但很少用 swap/细粒度保护/稀疏分配。`direct_segments2013.txt:12-48`
- **核心机制**：direct segment + primary region；per-core base/limit/offset 寄存器，segment 内 `V -> V + offset`，无 TLB miss；segment 外仍分页。`direct_segments2013.txt:26-40, 123-137, 313-332`
- **假设/代价**：需要连续物理内存、primary region 统一读写权限；放弃 segment 内 swap/CoW/细粒度保护/稀疏分配；OS 要预留物理内存、compaction、shootdown；原型不支持动态 resize。`direct_segments2013.txt:151-154, 306-348, 398-407, 541-553`
- **评估**：真实 96GB x86-64 服务器，Linux 2.6.32 修改 + 硬件性能计数器 + fake page fault 估计；graph500、memcached、MySQL、NPB、GUPS；用 4KB 页模拟 direct segment，不是真实 direct-segment 硬件。TLB overhead 从最高 51.1% 降到 <0.5%；MySQL 只覆盖 92.4% D-TLB miss。`direct_segments2013.txt:81-116, 228-247, 605-734, 757-763`
- **不能从论文推出**：不能推出适用于 IOMMU/GPU/ATS；不能推出 direct segment 在碎片化/多租户生产系统中总能形成。作者自己列出碎片、短生命周期进程、稀疏分配等不适用场景。`direct_segments2013.txt:449-463, 500-507`
- **原文定位**：§2–§5；Table 1–5；Fig. 3–5。

### Card 3 — Mosaic: A GPU Memory Manager with Application-Transparent Support for Multiple Page Sizes

- **题录/URL**：MICRO 2017，Ausavarungnirun 等；`source-manifest.json:44`；题录提醒见 `MAP_BRIEF.md:28`。
- **针对问题**：GPU 地址翻译希望大页，demand paging 希望小页；已有 coalescing/splintering 需迁移数据。`mosaic2017.txt:9-44, 127-136`
- **核心机制**：CoCoA 按 application 分配连续 base page；In-Place Coalescer 在分配后原地合并大页，不迁移数据；CAC 在碎片高时 splinter 并 compact；L3/L4 PTE 上加 large page bit/disabled bit。`mosaic2017.txt:45-52, 151-171, 396-504, 558-596`
- **假设/代价**：依赖 GPGPU en masse allocation；需要 GPU runtime/硬件修改；soft guarantee 一个大页帧只属于一个 application；splinter 需 TLB flush；compaction 仍可能迁移数据。`mosaic2017.txt:127-136, 172-208, 434-453, 632-654`
- **评估**：GPGPU-Sim/MAFIA，235 个同构/异构多应用负载；baseline 是 Power et al. GPU-MMU，另设 Ideal TLB；平均提升 55.5%/29.7%，距 ideal 6.8%/15.4%。`mosaic2017.txt:172-208, 656-716, 719-784`
- **不能从论文推出**：不能推出适用于 CPU OS 或真实商用 GPU；不能推出“大页总是好”，论文明确显示 demand paging 下 2MB 页可慢 92.5%–99.8%。`mosaic2017.txt:308-324`
- **原文定位**：§3–§6；Fig. 1–16；Table 1–2。

### Card 4 — Supporting Address Translation for Accelerator-Centric Architectures

- **题录/URL**：HPCA 2017，Cong/Fang/Hao/Reinman，UCLA；`source-manifest.json:88`。
- **针对问题**：customized accelerator 统一虚拟地址需要地址翻译；IOMMU-only 只达 ideal 12.3%，IOTLB 缺低延迟/页局部性，IOTLB miss walk 慢。`accelerator_translation2017.txt:8-20, 64-113`
- **核心机制**：每 accelerator 小 private TLB（16–32 项）；shared 512-entry L2 TLB 捕获 data tiling 造成的同页共享；把 page walk offload 到 host core MMU，利用 MMU cache/data cache 预热；非阻塞 hit-under-miss，支持 shootdown。`accelerator_translation2017.txt:21-45, 134-143, 399-431, 485-509, 594-665`
- **假设/代价**：加速器访问正则、page divergence 小；host core 在加速器运行时 spinning、MMU 较空闲；shared TLB 需存 process/accelerator ID；host MMU 单 walk 可成为瓶颈。`accelerator_translation2017.txt:604-626, 823-879`
- **评估**：PARADE/gem5，25 类 accelerator，medical imaging、PARSEC、vision、navigation；4/16 instances；CACTI 面积；组合方案达 ideal 93.6%，平均 page walk latency 58 cycles，page walk 减少 75.8%/76.8%。`accelerator_translation2017.txt:267-349, 731-734, 890-899`
- **不能从论文推出**：不能推出适用于复杂 memory behavior 的 GPU；不能推出真实 SoC/IOMMU 商用行为。作者自己说 rIOMMU 不面向 complex accelerator。`accelerator_translation2017.txt:927-958`
- **原文定位**：§II–§V；Fig. 2–17；Table I–III。

### Card 5 — Neighborhood-Aware Address Translation for Irregular GPU Applications

- **题录/URL**：MICRO 2018，Shin/LeBeane/Solihin/Basu；`source-manifest.json:300`。
- **针对问题**：irregular GPU 访问导致 TLB miss 爆发、IOMMU page walk 排队；商业硬件上可慢 3.7–4×。`neighborhood2018.txt:13-60`
- **核心机制**：一个 64B cache line 含 8 个 PTE；page table walker 取 PTE 时，扫描 IOMMU buffer 中同一 neighborhood 的 pending walk，立即用同一 cache line 中的 PTE 服务；扩展到 L2/L3/L4 上层 neighborhood。增加 IOMMU buffer 的 PL/PA/CL 字段和 PWST。`neighborhood2018.txt:32-43, 71-123, 383-458`
- **假设/代价**：假定 x86-64 式页表、IOMMU buffer、walk 按 cache line；需约 1.5KB CAM；不减少翻译请求数量，但减少 page table 访问；效果依赖并发 walk 落在同 neighborhood。`neighborhood2018.txt:383-458, 523-600, 690-787`
- **评估**：gem5 异构 CPU+integrated GPU；Polybench/Rodinia/Pannotia；irregular MVT/ATX/NW/BIC/GEV；full coalescing 平均 1.7×，最高 2.3×；page table 访问 -37%，page walk latency -47%；与 PTE cache 对比，coalescing 更快。`neighborhood2018.txt:523-600, 602-687, 698-787`
- **不能从论文推出**：不能推出适用于 CPU MMU 或所有 IOMMU；作者明确说减少 page table 访问不一定加快单次 walk，因为多个 walker 并行。`neighborhood2018.txt:659-687`
- **原文定位**：§III–VI；Fig. 2–14；Table I–III。

### Card 6 — SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators

- **题录/URL**：arXiv:2001.07045v1，2020-01-20；正式会刊未确认，不猜会议；`source-manifest.json:183`，`MAP_BRIEF.md:27`。
- **针对问题**：area/power 受限 accelerator 不能上大 TLB；centralized IOMMU 离 accelerator 和分布式内存远；既有 contiguity/identity mapping 牺牲 VM 功能。`sparta2020.txt:14-31, 41-47, 298-335`
- **核心机制**：split translation；accelerator-side tiny TLB 只覆盖 cache；memory-side per-partition TLB/MMU；虚拟地址唯一决定 partition，数据和 PTE 同 partition，translation 与 data fetch overlap；partition 内存分配可在 Linux 中用 NUMA/membind 实现，inverted page table。`sparta2020.txt:20-31, 186-242, 383-409, 526-600`
- **假设/代价**：虚拟页必须映射到特定 partition，是对 mapping 的约束；per-partition TLB 被多线程共享，存在容量/干扰；需 OS 修改；支持 demand paging/CoW 是作者原型主张。`sparta2020.txt:220-242, 535-600, 828-864`
- **评估**：真实硬件 perf 测 TLB、Pin trace 128GB、CPI model、Linux 4.10 原型 32 partitions、RocksDB；对比 conventional 4KB/2MB、DIPTA；平均 translation overhead 降 31.5×，性能 +57%，memory-side TLB 约少 4× 条目。`sparta2020.txt:618-654, 669-671, 798-826, 954-973`
- **不能从论文推出**：不能推出完整系统性能已实测；作者用 CPI model，不是 full-system simulation。不能推出真实 GPU 可商用。`sparta2020.txt:656-662`
- **原文定位**：§2–§7；Table 1–2；Fig. 3–10。

### Card 7 — Victima: Drastically Increasing Address Translation Reach by Leveraging Underutilized Cache Resources

- **题录/URL**：arXiv:2310.04158v3，2024-01-05；venue 需按最终出版核对；`source-manifest.json:343`。
- **针对问题**：data-intensive workload 的 TLB reach 不足，PTW 频繁；大硬件 TLB 增加 latency/area/power；大 software-managed TLB 需连续内存和 OS 修改。`victima2023.txt:9-24, 57-96`
- **核心机制**：把 L2 cache block 复用为 TLB entry cluster；PTW-CP 预测 costly-to-translate 页；TLB-aware SRRIP 优先保留 TLB block；支持 native/virtualized，存 guest 和 nested TLB entry。`victima2023.txt:25-39, 100-121, 558-598`
- **假设/代价**：依赖 cache underutilization（92% L2 block 无复用）；修改 cache tag/元数据、ASID/VMID；需处理 context switch 和 shootdown 时 L2 中 TLB block 的 invalidate；预测错误会污染 cache？作者用 predictor 控制。`victima2023.txt:70-89, 491-505, 584-666, 980-1040`
- **评估**：Sniper，11 个 GraphBIG/GUPS/XSBench/DLRM/GenomicsBench；baseline 1.5K L2 TLB + radix；对比 POM-TLB、ideal shadow paging、opt 64K/128K L2 TLB；native +7.4%，virtualized +28.7%，PTW -50%，reach 220MB/36×，面积/功耗 0.04%/0.08%。`victima2023.txt:1042-1076, 1110-1220`
- **不能从论文推出**：不能推出真实芯片面积/功耗/吞吐；不能推出适用于 GPU/IOMMU。作者用 McPAT 和模拟。`victima2023.txt:1011-1039`
- **原文定位**：§3–§11；Fig. 1–29；Table 1–4。

### Card 8 — HDPAT: Hierarchical Distributed Page Address Translation for Wafer-Scale GPUs

- **题录/URL**：HPCA 2026，Xu/Li/Sun/Ren/Sun，William & Mary/UIUC；`source-manifest.json:318`，`MAP_BRIEF.md:33`。
- **针对问题**：wafer-scale GPU 大量 GPM 通过 mesh 连接，非本地翻译集中到中央 IOMMU；约 16 个 walker 造成 queueing；中心 GPM 因离 IOMMU 近而执行更快，几何不平衡。`hdpat2026.txt:12-44, 147-158, 181-201, 260-316`
- **核心机制**：concentric caching + rotation；redirection table 1024 entries；IOMMU walker 预取 next 4 PTE 并选择性分发到内/中层 GPM；Cuckoo filter 判断 PTE 可能位置；peer caching 绕过 IOMMU。`hdpat2026.txt:31-44, 439-591, 592-659`
- **假设/代价**：wafer-scale GPU 作为单一集成产品；driver 均匀划分 buffer/线程；GPM 本地页表只覆盖本 HBM；不做 GPU-to-GPU page migration，忽略 TLB shootdown；redirection table 不是 TLB，依赖集中 IOMMU 仍存在。`hdpat2026.txt:79-94, 131-146, 158-179, 917-928`
- **评估**：MGPUSim，48 GPMs、32 CU/GPM、mesh，14 个 Hetero-Mark/AMDAPPSDK/SHOC/DNNMark；baseline、Trans-FW、Valkyrie、Barre、simple distributed caching；平均 1.57×，offload 42.1%，prefetch accuracy 65.55%，round-trip -41%，NoC +0.82%，redirection table 0.034mm²/0.16W。`hdpat2026.txt:592-763, 824-928, 936-945`
- **不能从论文推出**：不能推出真实 wafer-scale GPU 结果；不能推出 PCIe ATS/SoC IOMMU 行为；不能推出 page migration 或 shootdown 成本可忽略，作者主动排除。`hdpat2026.txt:79-94`
- **原文定位**：§III–V；Fig. 1–22；Table I–II。

### Card 9 — Revelator: Rapid Data Fetching via OS-Guided Hash-based Speculative Address Translation

- **题录/URL**：本地 arXiv:2508.02007v1，2025-08-04；`MAP_BRIEF.md:34` 称 2026 ISCA 有 DOI，但本地 v1 评估细节不能当最终版；`source-manifest.json:334`。
- **针对问题**：VA→PA 在传统 OS 中不可预测；SpecTLB 依赖大页/连续 VA-PA，碎片下 miss 高；要隐藏 page walk 延迟。`revelator2025preprint.txt:6-48, 194-300, 387-443`
- **核心机制**：OS tiered hash-based allocation，`PPN_i = Hash_i(VPN)`，失败回退普通分配；硬件 speculation engine 按同一 hash 生成候选 PA，过滤碎片/带宽后提前 fetch 数据和最后一级 PTE；page table frame 也 hash 分配，speculatively fetch 第四级 PTE 以打破串行 walk。`revelator2025preprint.txt:24-47, 72-105, 476-600, 618-750`
- **假设/代价**：OS 必须成功用 hash 分配较多页；回退会降低收益；多候选 fetch 浪费带宽；虚拟化需 guest/hypervisor 合作；当前 nested speculation 留作 future work。`revelator2025preprint.txt:105-110, 517-548, 790-810, 1128-1131, 1259-1266`
- **评估**：Linux v6.13 原型 + Virtuoso/Sniper；GraphBIG/GUPS/XSBench/DLRM/GenomicsBench；native/virtualized；baseline radix、THP、SpecTLB、ECH、L2TLB-128K、POM-TLB；native +27%，virtualized +20%，超 SpecTLB 5%，能耗 -9%，面积/功耗 0.01%/0.02%。`revelator2025preprint.txt:99-120, 812-927, 932-940, 1230-1235, 1255-1265`
- **不能从论文推出**：不能推出生产系统行为；不能推出 hash 回退率高时仍同等有效；不能推出适用于 GPU/IOMMU/wafer。作者自己给出高 memory pressure 下收益降至 7%。`revelator2025preprint.txt:117-119`
- **原文定位**：§3–§8；Fig. 1–19；Table 1–2。

---

## 3. 跨论文对照：成熟机制、条件性解决、公开证据仍受限

### 3.1 成熟机制

1. **CPU radix 页表下的 MMU cache / translation cache**：Barr 2010 已系统比较 unified/split/path，说明 translation cache 能以更少 tag 覆盖更大地址范围，并给出 replacement policy 影响。`translation_cache2010.txt:303-322, 366-386, 682-724`
2. **大内存服务器上 direct segment 绕过 TLB**：Basu 2013 对特定负载（启动时大块匿名读写、不 swap、少保护）非常有效，但强依赖连续物理内存和 OS 支持。`direct_segments2013.txt:12-48, 398-407`
3. **GPU 多页大小与 demand paging 协同**：Mosaic 的 CoCoA/In-Place Coalescer 在模拟中证明可避免 coalescing 迁移，并提升 TLB reach。`mosaic2017.txt:45-52, 396-504`
4. **accelerator VM 的 private TLB + shared L2 TLB + host MMU page walk**：Cong 2017 在正则加速器负载上接近 ideal。`accelerator_translation2017.txt:21-45, 594-665`
5. **GPU/IOMMU page walk neighborhood coalescing**：Shin 2018 对 irregular GPU 的并发 walk 有明确收益，且与 PTE cache 对比显示 coalescing 优势。`neighborhood2018.txt:71-123, 698-787`
6. **cache-backed TLB reach**：Victima 对数据密集 CPU 负载的 PTW 减少和 native/virtualized 收益有模拟证据。`victima2023.txt:100-121, 1110-1220`

### 3.2 只在特定条件下解决

| 问题 | 条件 | 证据 |
|---|---|---|
| TLB reach 不足 | 只能在大页可形成、无严重碎片时靠大页/合并 | Mosaic 显示 demand paging 下 2MB 页可慢 92.5%–99.8%；SPARTA 指出 contiguity 依赖碎片 | `mosaic2017.txt:308-324`；`sparta2020.txt:344-357` |
| 连续物理内存/直段 | 需要 primary region、统一权限、连续物理块；不适合短生命周期/碎片/稀疏 | Basu 2013；SPARTA Table 1 | `direct_segments2013.txt:151-154, 449-463`；`sparta2020.txt:207-215` |
| GPU 多页大小 | 依赖 en masse allocation、GPGPU 工作负载、模拟 | Mosaic | `mosaic2017.txt:127-136, 172-208` |
| accelerator 翻译 | 依赖加速器访问正则、host core 可服务、shared TLB 命中 | Cong 2017 | `accelerator_translation2017.txt:604-626, 823-879` |
| 不规则 GPU walk 合并 | 依赖同 neighborhood 并发 walk、cache-line PTE 布局、IOMMU buffer | Shin 2018 | `neighborhood2018.txt:32-43, 659-687` |
| memory-side translation | 需要 OS partition、虚拟地址唯一决定 partition、per-partition TLB 可承受共享竞争 | SPARTA | `sparta2020.txt:186-242, 828-864` |
| cache 存 TLB | 依赖 cache underutilization、可预测 costly page、能处理 invalidation | Victima | `victima2023.txt:70-89, 491-505, 980-1040` |
| wafer-scale 分布式翻译 | 依赖 mesh、GPM peer cache、集中 IOMMU 仍存在、无 page migration/shootdown | HDPAT | `hdpat2026.txt:79-94, 439-591` |
| hash-based speculation | 依赖 hash 分配成功率；高压力/高带宽竞争下需 filter | Revelator | `revelator2025preprint.txt:105-110, 517-548, 1128-1131` |

### 3.3 公开证据仍受限，但不能叫全领域空白

- **wafer-scale GPU**：HDPAT 只有 MGPUSim 模拟，不做 page migration，忽略 TLB shootdown；不能外推真实硅。`hdpat2026.txt:79-94, 592-659`
- **Revelator**：本地 v1；真实 Linux 原型只验证分配策略，性能是 Virtuoso/Sniper；nested speculation 还是 future work。`revelator2025preprint.txt:99-110, 1259-1266`
- **SPARTA**：arXiv 2020，正式 venue 未确认；性能用 CPI model，不是 full-system。`sparta2020.txt:656-662`
- **Victima**：Sniper + McPAT；不是真实芯片。`victima2023.txt:1011-1039`
- **Mosaic / Cong / Shin**：均为模拟。`mosaic2017.txt:656-716`；`accelerator_translation2017.txt:267-349`；`neighborhood2018.txt:523-600`
- **direct segment**：真实硬件上是用 4KB 页模拟，不是 direct-segment 硬件。`direct_segments2013.txt:577-603`

这些只能说明“在这些评估条件下仍有开放问题”，不能自动说“全领域都没有解决”。特别是未附全文的 vAttention、GMLake、MINISA、ShareMMU、Tiara、BypassD、DX100 等不能凭题名判断。

---

## 4. 3–5 个真正的矛盾/瓶颈

每个瓶颈给至少两种相互补充或制约的原始证据，并区分：作者实测事实 / 作者解释 / 我的综合推断 / 未验证机会。

### 瓶颈 1：容量/覆盖范围 vs 低时延/低面积功耗

- **作者实测/模拟事实**：Victima 指出 TLB 每翻倍大小，访问延迟约增 1.4×；其自身表中 realistic 64K L2 TLB 只比 baseline 好 0.8%，而 optimistic 64K 好 4.0%。`victima2023.txt:40-42, 356-360`
- **作者实测/模拟事实**：Barr 2010 显示 translation cache 用更小 tag 达到更大覆盖，UTC+VI-LRU 在 16GB join 中 16 entries 达到 90% L3 命中，而 LRU 需 52 entries；无 cache 时每 walk 4 次内存层次访问，加 cache 降到 1.13。`translation_cache2010.txt:378-386, 496-511, 717-724`
- **相互制约**：Victima 用 cache 容量补 TLB 覆盖，但代价是 L2 数据容量/带宽和 invalidation 复杂度；Barr 用 translation cache 覆盖，但需要 replacement policy 保护上层条目。
- **我的综合推断**：容量扩展不是单一旋钮；必须同时问“新增条目放在哪、访问延迟多少、替换谁、失效谁”。
- **未验证机会**：在 CVA6/RISC-V IOMMU 上分别量化 IOTLB 容量、walk cache 容量、L2/L3 cache 占用对 page walk 延迟和系统吞吐的影响；不能直接套用 GPU/wafer 结论。

### 瓶颈 2：依赖式 page walk 时延 vs 并发/队列吞吐

- **作者实测/模拟事实**：Shin 2018 发现 full coalescing 减少 page table 访问 37%、page walk latency 下降 47%，但明确说“减少访问不一定加快单次 walk”，因为多个 walker 并行；收益可能来自 walker 利用率。`neighborhood2018.txt:659-687`
- **作者实测/模拟事实**：HDPAT 显示中央 IOMMU 只有约 16 个 walker，SPMV pre-queue delay 最大，近 700 个翻译请求排队；理想 IOMMU 可带来 5.45×/4.96×。`hdpat2026.txt:147-158, 181-201, 260-316`
- **相互制约**：增加 walker 可降 queueing，但带来 area/power，且 HDPAT 说“单纯加 walker 不可扩展”；coalescing 可减少访问但不打破单次 walk 的串行依赖。
- **作者解释**：Revelator 用 speculative PTE/data fetch 打破 PTW 串行依赖；`revelator2025preprint.txt:87-98` 说明它直接针对依赖链。
- **我的综合推断**：并发吞吐、单次 walk latency、应用吞吐是三件事；优化一个不等于优化另一个。
- **未验证机会**：为共享 IOMMU 建立“队列延迟 + walk 依赖 + coalescing 吞吐”联合模型；在 RISC-V IOMMU 上验证。

### 瓶颈 3：物理放置/OS 约束 vs 通用性与碎片化

- **作者实测/模拟事实**：direct segment 需要连续物理内存，compaction 10GB 约 3 秒，5 分钟进程约 1% 开销；原型不支持动态 resize。`direct_segments2013.txt:401-407, 541-553`
- **作者实测/模拟事实**：SPARTA 把虚拟地址限制到特定 partition，用 Linux NUMA/membind 实现 inverted page table；它声称可保留 demand paging/CoW，但 partition 本身是 mapping 约束。`sparta2020.txt:186-242, 535-600`
- **作者实测/事实**：Revelator 说明传统分配器中 VA→PA overlap <0.1%，碎片/多租户令预测困难；其方案要求 OS hash 分配成功，高压力下收益降到 7%。`revelator2025preprint.txt:387-443, 117-119`
- **相互制约**：越依赖物理放置预测/连续/partition，越提高翻译效率，但越受碎片、多租户、CoW、动态分配限制。
- **我的综合推断**：物理放置是地址翻译的隐藏控制面；不处理 OS 分配器，很难获得可泛化的翻译优化。
- **未验证机会**：在 Linux/RISC-V IOMMU 下测不同 fragmentation/multi-tenancy 对 direct segment/hash/partition 的影响；当前项目不要求。

### 瓶颈 4：并行与互连扩展成本：集中式 IOMMU/shared 结构的物理瓶颈

- **作者实测/模拟事实**：HDPAT 显示所有远程翻译请求汇聚中央 IOMMU，mesh 多跳增加延迟；offload 42.1% 后仍依赖 IOMMU；中心 GPM 比外围快。`hdpat2026.txt:181-201, 260-316, 704-713`
- **作者实测/模拟事实**：SPARTA 说 GPU 可集成多千条目 TLB，但 area/power 约束加速器不可行；memory-side TLB 共享会线程竞争，但 partition 后竞争影响下降。`sparta2020.txt:316-324, 828-864`
- **作者实测/模拟事实**：Cong 2017 16 个 accelerator instance 时 host MMU/互连/内存成为瓶颈，收益递减到 7.7% overhead。`accelerator_translation2017.txt:823-899`
- **相互制约**：分布式 peer caching/redirection 缓解集中 IOMMU，却增加 peer cache 容量、一致性、NoC 流量、失效成本。
- **我的综合推断**：扩展性瓶颈会从 IOMMU 转移到互连/peer cache/一致性，而不是消失。
- **未验证机会**：比较集中 IOMMU vs 分布式 peer IOMMU 在 FPGA 多 iDMA 上的队列和 NoC 成本；不能直接做 wafer 结论。

### 瓶颈 5：指标分离——命中率、PTW 时延、应用吞吐、能耗不是同一指标

- **作者实测/模拟事实**：Barr 2010 中 MMU cache 把每 walk 内存层次访问从 4 降到 1.13，但 DRAM 只从 0.15 降到 0.14；收益主要来自 L2 延迟差。`translation_cache2010.txt:496-511`
- **作者实测/模拟事实**：Shin 2018 中 page table 访问降 37%，page walk latency 降 47%，但作者强调单次 walk 不一定更快。`neighborhood2018.txt:659-687`
- **作者实测/模拟事实**：Victima 用 PTW 数量、L2 TLB miss latency、端到端 speedup 多个指标；Revelator 也分开报告 memory access latency、L2 TLB MPKI、address translation latency、speedup。`victima2023.txt:1109-1220`；`revelator2025preprint.txt:932-940`
- **我的综合推断**：若只报 TLB hit rate，可能掩盖 page walk 串行依赖、queueing、cache pollution、带宽浪费。
- **未验证机会**：定义并测量“IOMMU 队列延迟 / 单次 walk 延迟 / 应用吞吐 / 能耗”的分层指标，再谈优化。

---

## 5. 最容易误判为创新的普通方案、强对手、仍需补齐的分支

### 5.1 容易误判为创新的普通方案

1. **单纯扩大 TLB / 加 L2/L3 TLB**：Victima 已显示 realistic 大 TLB 因延迟/面积/功耗收益小；HDPAT 已显示单纯加 walker 不可扩展。`victima2023.txt:356-360`；`hdpat2026.txt:147-158`
2. **单纯使用大页**：Mosaic 显示 demand paging 下大页可严重变慢；Revelator 显示大页/连续性在碎片下不可靠。`mosaic2017.txt:308-324`；`revelator2025preprint.txt:20-23`
3. **简单 next-N prefetch**：HDPAT、Revelator、Shin 都做预取/合并，但分别在 IOMMU、OS 放置、IOMMU buffer 层面解决约束；只加“预取 N 页”没有处理带宽、碎片、失效。
4. **简单 shared TLB / central IOMMU**：HDPAT 专门证明 central IOMMU 在 wafer 规模是瓶颈；Cong 2017 也显示 host MMU 可能瓶颈。`hdpat2026.txt:181-201`；`accelerator_translation2017.txt:823-879`
5. **把 page invalidation/permission update 等价为“全局停机”**：`MAP_BRIEF.md:20` 明确禁止；合法语义与代价必须分开，非 ATS 设备也不是只能存物理地址，逐页 in-flight 计数也不是唯一合法办法。
6. **把 TLB miss 等同于完整未缓存 page walk**：`MAP_BRIEF.md:18` 明确；Barr、Shin、Victima 都显示 walk cache、合并、并发、数据缓存、内存仲裁在起作用。

### 5.2 强对手

- **Victima**：cache-backed translation reach；强在低 area/power + native/virtualized。
- **SPARTA**：memory-side partition/co-location；强在 accelerator area/power 与大规模内存。
- **HDPAT**：wafer-scale 分布式 translation；强在多 GPM、mesh、IOMMU queueing。
- **Revelator**：OS-guided hash speculation；强在 fragmentation-resilient、直接打破 PTW 依赖。
- **Mosaic**：GPU 多页大小 + demand paging；强在同时处理 TLB reach 和 PCIe fault。
- **Cong 2017**：accelerator private/shared TLB + host MMU walk；强在正则加速器和低硬件复杂度。
- **Shin 2018**：IOMMU page walk coalescing；强在 irregular GPU、成本约 1.5KB。
- 其他只见题录的强对手：POM-TLB、DIPTA、ECH、Trans-FW、Valkyrie、Barre 等，需补全文再评估。`victima2023.txt`、`revelator2025preprint.txt`、`hdpat2026.txt` 的 related work 已给出这些名字。

### 5.3 仍需补齐的经典或前沿分支

- **经典分支**：DAE 1982、TLB/MMU cache 早期设计、page table organization（inverted/hashed/TSB/guarded page table）、invalidation/permission update 语义。未附全文，不能建卡。`source-manifest.json:5` 等。
- **IOMMU 专属分支**：rIOMMU、Utilizing IOMMU Scalably、IOTLB wall、共享 IOMMU queueing、PCIe ATS 与 non-ATS 设备语义。未附全文。
- **GPU/GMMU 分支**：walk scheduling、walk stealing、MASK、vAttention、MINISA、ShareMMU 等。未附全文；题录提醒见 `MAP_BRIEF.md:25-32`。
- **异构/动态内存/软硬件接口**：GMLake、vTensor、Tiara、NDS、WIC 等。未附全文；Tiara 题录提醒见 `MAP_BRIEF.md:35`。
- **评估方法分支**：如何把 hit rate、PTW latency、queueing、应用吞吐、能耗分开测量；现有论文多用模拟、trace、CPI model，真实生产 IOMMU/ATS 证据不足。

### 5.4 最有必要精读的 3–5 篇与阅读顺序

1. **Translation Caching: Skip, Don’t Walk (the Page Table), ISCA 2010**  
   经典 MMU cache 设计空间；先建立“page walk cache vs translation cache；覆盖 vs tag/替换”的语言。`translation_cache2010.txt`

2. **Efficient Virtual Memory for Big Memory Servers, ISCA 2013**  
   理解 TLB reach 不足时如何转向 physical placement/OS 约束；direct segment 的代价边界非常清楚。`direct_segments2013.txt`

3. **Mosaic, MICRO 2017 + Supporting Address Translation for Accelerator-Centric Architectures, HPCA 2017**  
   前者看 GPU 多页大小/demand paging 的 tradeoff；后者看 accelerator + IOMMU + host MMU 的翻译结构。`mosaic2017.txt`；`accelerator_translation2017.txt`

4. **Victima, 2023/2024 + SPARTA, arXiv 2020**  
   两者都把翻译推向 TLB 以外：Victima 用 cache，SPARTA 用 memory partition；对照“容量/覆盖”和“物理放置/OS 约束”。`victima2023.txt`；`sparta2020.txt`

5. **HDPAT, HPCA 2026 + Revelator, 2025 arXiv v1 / 2026 ISCA**  
   2026 前沿两端：HDPAT 解决 wafer-scale 集中 IOMMU/互连扩展；Revelator 解决单系统 VA→PA 不可预测和 PTW 依赖时延。`hdpat2026.txt`；`revelator2025preprint.txt`

---

## 6. HDPAT 与 Revelator 各自解决的对象：明确对比

| 维度 | HDPAT 2026 | Revelator 2025/2026 |
|---|---|---|
| 主要对象 | wafer-scale GPU 中**集中式 IOMMU 瓶颈 + mesh 多跳 + 多 GPM 远程翻译** | 单系统 CPU 中**VA→PA 不可预测 + TLB miss 后 PTW 串行依赖** |
| 问题层面 | 并行/互连扩展成本；translation request 汇聚、IOMMU queueing、几何不平衡 | 依赖访问时延；物理放置/OS 分配器；page walk 依赖链 |
| 核心手段 | concentric caching、rotation、redirection table、4-PTE prefetch、peer caching、Cuckoo filter | OS tiered hash-based allocation、硬件 speculation engine、候选 PA/PTE 提前 fetch、degree filter |
| 容量/覆盖 | 把翻译缓存/PTE 分散到 GPM peer caches，扩大系统级翻译覆盖 | 不主要扩大 TLB 容量；用 hash 让预测成为可能，减少对 TLB 覆盖的依赖 |
| 物理放置/OS | 不改变 OS 分配器；依赖 driver 均匀划分，不做 GPU-to-GPU page migration | 直接改变 OS 物理页分配；要求 hash 分配成功，失败回退 |
| 并行/互连 | 核心议题：减少到中央 IOMMU 的远程翻译和跳数 | 不是多 GPM/wafer 议题；关注单请求延迟隐藏 |
| 评估 | MGPUSim，48 GPMs，14 benchmarks，baseline/Trans-FW/Valkyrie/Barre | Linux v6.13 原型 + Virtuoso/Sniper，11 benchmarks，radix/THP/SpecTLB/ECH/POM-TLB |
| 不能互换 | 不能直接推出 CPU OS 分配器行为 | 不能直接推出 wafer-scale/多 GPM/集中 IOMMU 行为 |
| 互补关系 | HDPAT 解决“翻译请求太多、太远、中央排队” | Revelator 解决“翻译本身串行、VA→PA 不可预测、数据 fetch 被翻译挡住” |

**我的综合推断**：二者不是同一问题的竞争方案。HDPAT 是**系统级分布式翻译调度**，Revelator 是**单系统内存访问路径上的 OS-HW 协同推测**。若要把 Revelator 的思路迁移到 HDPAT 场景，需要先问 OS/hypervisor 是否能对 GPM/HBM 物理页做 hash 放置、guest/host 两级翻译如何合作；若要把 HDPAT 的 peer caching 迁移到单 CPU IOMMU，需要先问是否有多个可缓存 PTE 的 peer 代理和 mesh/NoC 结构。两者都未提供可直接迁移到 RISC-V IOMMU + CVA6 + 四 iDMA 的证据。

---

## 7. 对本项目平台（CVA6 + 四 iDMA + RISC-V IOMMU + Linux / Genesys2）的适用范围

`MAP_BRIEF.md:37-40` 已限定：已通过 172/172 功能验收，有同 BIT 的 S/N/P/PN 干预和双方观测；不等于论文创新、实际 GPU/CXL/PCIe ATS 或线上应用结论；Pn 部分配置改善一方损害另一方，根因与可泛化性未确立。

### 7.1 本线证据能支持什么

- 支持把地址翻译问题拆成：TLB/IOTLB 容量/覆盖、page walk 依赖时延、walk cache/合并、物理放置/OS 约束、共享 IOMMU 队列与互连成本。`translation_cache2010.txt`、`direct_segments2013.txt`、`neighborhood2018.txt`、`victima2023.txt`
- 支持承认“TLB miss 不等于完整未缓存 page walk”，以及 walk caches、合并、并发、数据缓存、内存仲裁会影响结果。`MAP_BRIEF.md:18`；`neighborhood2018.txt:659-687`
- 支持在 RISC-V IOMMU/Linux 平台关注 invalidation/permission update 的合法语义与代价分离，不假设所有 IOMMU 必须全局停机。`MAP_BRIEF.md:20`
- 支持把“共享 IOMMU 的队列/并行 page table walker”作为设计问题：HDPAT、Cong 2017、Shin 2018 都说明了集中 walker/IOMMU 队列会成为瓶颈。`hdpat2026.txt:147-158`；`accelerator_translation2017.txt:823-879`；`neighborhood2018.txt:289-303`
- 支持说明 direct segment/primary region 类方案的强条件：连续物理内存、统一权限、OS 修改、碎片敏感。`direct_segments2013.txt:151-154, 398-407`

### 7.2 本线证据不能支持什么

- 不能支持 GPU GMMU、wafer-scale GPU、PCIe ATS、CXL/远程内存、线上租户、商用 RISC-V IOMMU 的结论。
- 不能支持“HDPAT/Revelator/Victima 的收益可在 CVA6 + FPGA 上复现”。
- 不能把 S/N/P/PN 的功能验收解释为论文创新或通用根因。
- 不能从这些论文推出本平台的 iDMA/IOMMU 配置一定需要 direct segment、hash allocation、peer caching 或 cache-backed TLB；这些是可选机制族，不是已确认缺口。
- 不能以“加自适应/端到端/智能”代替具体机制；必须回到容量、时延、放置、并行/互连四条轴分别测量。

---

## 8. 建议的下一步阅读顺序（给 Codex 验证用）

1. 先核 `translation_cache2010` 与 `direct_segments2013`：建立 page walk cache / translation cache / direct segment 的经典边界。
2. 再核 `mosaic2017`、`accelerator_translation2017`、`neighborhood2018`：建立 GPU/accelerator/IOMMU 三条翻译路径的差异。
3. 再核 `sparta2020`、`victima2023`：建立 memory-side partition 与 cache-backed TLB 两条“TLB 之外”的覆盖方案。
4. 最后核 `hdpat2026`、`revelator2025preprint`：建立 2026 前沿的“分布式翻译调度”与“OS-HW 推测翻译”两端。
5. 补读未附全文的关键分支：`walk_stealing2021`、`iotlb_wall2023`、`vattention2024`、`sharemmu2026`、`minisa2026`、`tiara2026` 等；在补全文前不要判断其是否已解决某缺口。

以上报告只使用所附全文与 `MAP_BRIEF.md`/`source-manifest.json` 的题录信息；未宣称运行实验、复现论文或修改文件。