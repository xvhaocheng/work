# G5_foundations 共享地址翻译经典与前沿证据报告

> 阅读范围：本组 10 篇全文（`icc2010, slltlb2011, nocstar2018, translation_cache2010, direct_segments2013, victima2023, revelator2026v2, vmitosis2021, contiguitas2023, cross2025`）。
> 元数据核对仅依据所给全文题头/出版信息；全文未给出者标 null，不据文件名或引用年份反推。
> 重要边界：本组绝大多数是**CPU 核/核间 TLB 与页表机制**，不是 IOMMU/SVA 设备侧实现。它们可作**思想前史与机制谱系**，不能据此断言商用 IOMMU 已同样实现。

---

## A. 本组问题与机制演化的时间解释

本组共同的问题背景是：**地址翻译开销随内存容量与并发度增长，而单个私有 TLB/单层页表结构无法同时兼顾命中率、访问延迟、容量与失效成本**。机制演化大致分三段。

### A1. 2010–2013：单核与 CMP 内的 TLB/页表加速
- **translation_cache2010** 先把问题定位为“TLB miss 后的 page walk 太贵”，系统提出 MMU cache 设计空间（page table cache vs translation cache；unified/split/path），并给出统一 translation cache + VI-LRU 优于 AMD Page Walk Cache 与 Intel Paging-Structure Cache 的结论（L1–58, L302–436）。这是**“缓存部分翻译以跳过若干层页表”**的思想来源。
- **icc2010** 把视角从单核拉到 CMP：发现多个核在同一 1M 指令窗口内会对相同虚拟页、相同 context 产生 TLB miss（Inter-Core Shared, ICS）或可预测的跨核 stride（ICPS），提出 Leader-Follower（把 leader 的翻译推给 follower 的 Prefetch Buffer）与 Distance-based Cross-Core（共享 Distance Table 记录 distance pair 并 pull 预取）两类 ICC 预取器（L19–32, L116–138, L210–376）。核心洞见是**跨代理的翻译冗余可被复用**。
- **slltlb2011** 换一条路：不再预取，而是直接把 last-level TLB 做成**多核共享的 SLL TLB**（私有 L1 TLB + 中心共享 L2 TLB，mostly-inclusive，shootdown-aware），用共享结构消除复制、并按运行需求动态分配条目（L17–34, L161–253）。
- **direct_segments2013** 则指出 big-memory 工作负载几乎不用分页的丰富语义（swap、Cow、细粒度保护），因此用 **direct segment（BASE/LIMIT/OFFSET）** 把一段连续虚拟地址直接映到连续物理地址，彻底消除该区 TLB miss，其余仍走分页（L12–58, L124–137, L306–357）。

这一段的关键词是：**跳过页表层数、复用跨核翻译、把翻译结构共享化、对大块内存去 TLB 化**。

### A2. 2018–2021：共享翻译结构的可扩展性与虚拟化/NUMA 下的放置
- **nocstar2018** 指出上一步共享 TLB 的致命短板是**访问延迟**：monolithic shared TLB 因 SRAM 变大 + 互连远而变慢，甚至比私有 L2 TLB 更差。它把 SLL TLB 切成分布在各核附近的 slice，并用**latchless、单周期、电路交换的专用互连**（NOCSTAR）把远端 slice 访问拉回近单周期（L20–34, L484–648, L735–788）。同时给出 TLB shootdown 的“invalidation leader”设计以抑制失效风暴（L790–813）。
- **vmitosis2021** 把问题搬到虚拟化 NUMA：guest page table (gPT) 与 extended page table (ePT) 物理放置不当会导致 2D page walk 远端化，最坏 3.1× 慢；方案是**迁移 + 复制**两套 page table，并区分 NUMA-visible / NUMA-oblivious 两种 VM 配置（L10–30, L67–135, L336–505）。

这一段的关键词是：**共享结构的物理布局与访问路径决定成败；翻译数据的“位置”（NUMA/切片/远端）与“容量”同等重要**。

### A3. 2023–2026：翻译 reach、物理 contiguity、推测与跨界预取
- **victima2023** 复用**未充分利用的 L2 cache block** 存 TLB 条目簇（8 entries/64B），用 PTW cost predictor 决定“值得缓存翻译”的页，配合 TLB-aware SRRIP（L25–52, L112–122, L585–676）。它同时明确反对“无脑加大 TLB”：更大 TLB 访问延迟、面积、功耗上升，realistic 64K L2 TLB 仅 ~0.8% 提升（L40–46, L359）。
- **contiguitas2023** 从数据中心生产视角指出：真正卡点是**物理内存 contiguity**，而根因是散布的 unmovable 分配（网络占 73%）；它把 movable/unmovable 分两区并动态调边界，再用 LLC 硬件扩展让 unmovable 页“在使用中”透明迁移，并把 shootdown 从同步全核确认降级为**每核本地、lazy 失效**（L19–45, L129–133, L188–256, L516–712）。它显式提到 **IOMMU + IOTLB、NIC 私有 TLB、以及通过内存队列的 invalidation 请求**（L545–553, L188–194），是本组最接近“设备侧共享翻译”的一篇。
- **cross2025** 聚焦 L1D 预取的**跨页边界**问题：静态“永远允许/永远禁止”page-cross 都不优，且 SOTA L1D 预取器 page-cross 准确率仅约 50%；提出 MOKA 框架与 DRIPPER 过滤器（hashed perceptron + system features + 自适应阈值）动态决定是否发出 page-cross 预取（L13–52, L113–355, L357–647）。
- **revelator2026v2** 把“可预测性”做进 OS：**tiered hash-based allocation** 让 VA→PA 与 VA→PTE 映射可被硬件用同一 hash 重算，L2 TLB miss 时 speculation engine 生成候选 PA 并**在页表遍历完成前**发起推测取数，配 speculation degree filter 控制带宽（L28–57, L347–570, L628–659）。它还讨论虚拟化（horizontal/diagonal speculation）、NUMA（Bloom-filter hint）与安全（secret per-process hash key、误推测清理）（L664–846）。

这一段的关键词是：**从“命中率/容量”转向“可预测性 + 物理 contiguity + 推测 + 失效降级”**，并第一次较明确地把 IOMMU/IOTLB、设备 TLB、lazy invalidation 纳入讨论语境。

### A4. 共享的三种含义（必须区分）
- **资源共享（resource sharing）**：硬件结构被多代理共用。例：slltlb2011 的共享 SLL TLB、nocstar2018 的分布式共享 slice、victima2023 的 cache block 复用、contiguitas2023 的 LLC 双映射元数据表。
- **映射共享（mapping/translation sharing）**：同一 translation（VP/PP/context/protection/page size 全同）被多代理复用。例：icc2010 的 ICS miss、slltlb2011 的 inter-core sharing、revelator 的可预测 VA→PA。
- **SVA（Shared Virtual Address / Shared Virtual Memory）**：CPU 与设备共享同一虚拟地址空间，设备用同一页表翻译。本组**没有一篇完整实现或评测 SVA**；contiguitas2023 仅提及 IOMMU/IOTLB/NIC TLB 作为迁移/失效对象（L188–194, L545–553）。这是本组“经典”与“设备共享翻译前沿”之间最大的证据缺口。

---

## B. 逐篇证据卡

### B1. icc2010
- **文件/定位**：`papers/icc2010.txt` L1–2（题目）、L3（作者）、L66（ASPLOS’10 出版行）、L67（ACM 版权行）。
- **精确机制**：两类 Inter-Core Cooperative (ICC) TLB 预取器。
  - Leader-Follower：leader 核 TLB+PB miss 后 walk page table，把已获翻译 push 到所有 follower 的 per-core Prefetch Buffer（PB），PB 与 TLB 并行查找；带 confidence counters 抑制过度预取；shootdown 会同时使 PB 失效（L210–320, L301–304）。
  - Distance-based Cross-Core：共享 Distance Table (DT) 记录跨核的 distance-pair，pull 式预取（L322–376）。
- **复用粒度与身份/权限匹配前提**：复用粒度是**单条翻译条目（virtual page + physical page + context ID + protection + page size 全同）**；匹配窗口为 1M 指令；跨核 stride 还需 context ID 与 page size 一致（L116–138）。
- **资源共享 vs 映射共享/SVA**：属于**映射共享驱动**——核间共享的是翻译内容，硬件结构（TLB/PB/DT）本身仍是私有/prefetch 侧；不涉及 SVA。
- **评价配置**：GEMS + Simics，4–16 核 UltraSPARC III Cu，PARSEC（9 个），1 billion instructions，<5% cold miss（L377–482, L463–467）。
- **限制/未覆盖**：纯仿真；无实机；不涉及设备/IOMMU；未评估安全；superpaging 实际未被系统使用（L458–462）；多程序集仅给少量适配说明（L1055–1071）。
- **最强反证**：即便有 confidence，bad prefetch 平均仍有 21%（无 confidence 为 38%）；盲目 push 翻译会驱逐有用 PB 条目（L644–660）。**“共享/推送翻译必有益”不成立**。
- **后续必追引文**：Kandiraju & Sivasubramaniam, ISCA 2002 [14]；Bhattacharjee & Martonosi, PACT 2009 [2]；Saulsbury et al., ISCA 2000 [19]。

### B2. slltlb2011
- **文件/定位**：`papers/slltlb2011.txt` L1（题目）、L3–6（作者/单位）。**题头未含会议/年份/DOI，故 venue/year 记 null（不据文件stem或引用年份反推）**。
- **精确机制**：私有 L1 TLB + **共享 last-level (SLL) L2 TLB**；mostly-inclusive（允许两级独立替换，>97% L1 条目在 SLL 中）；条目含完整 context/process ID；shootdown-aware（失效需查所有层）；可加简单 stride 预取（page±1,2,3）（L161–275, L183–191, L246–253）。
- **复用粒度与身份/权限匹配前提**：复用粒度是 translation entry；条目带完整 context/process ID；mostly-inclusive 前提下失效需检查所有层（L160–191）。
- **资源共享 vs 映射共享/SVA**：**资源共享 + 映射共享并重**——SLL TLB 是共享硬件资源，同时利用 inter-core translation sharing；不涉及 SVA。
- **评价配置**：GEMS/Simics，4 核/16 核 UltraSPARC III Cu，PARSEC（9）+ SPEC CPU2006 multiprogrammed mixes；并行用 timing 模拟，多程序仅 functional 模拟（L277–436）。
- **限制/未覆盖**：仿真；无实机；无 IOMMU/设备；未讨论安全；cactusADM 因 TLB 复用极差反而轻微降级（L988–1074）。
- **最强反证**：对高 ICPS 的 Blackscholes，SLL TLB 不如 ICC prefetcher；对部分应用 SLL 会带来额外访问延迟成本（L502–505, L767–804）。**共享结构并非普遍优于预取/私有结构**。
- **后续必追引文**：Barr et al., ISCA 2010 [2]；Bhattacharjee & Martonosi, PACT 2009 [3]；Bhattacharjee & Martonosi, ASPLOS 2010 [4]；Srikantaiah & Kandemir, MICRO 2010 [21 引]。

### B3. nocstar2018
- **文件/定位**：`papers/nocstar2018.txt` L1（MICRO 2018）、L6–11（题目/作者）、L68–69（ISBN/DOI 10.1109/MICRO.2018.00030）。
- **精确机制**：把 SLL TLB 做成**分布式 TLB slice**（每核附近、slice ≤ 当前私有 L2 TLB 大小），用 **latchless、单周期、电路交换** 的专用 side-band NOC（NOCSTAR）连接 L1 TLB 与远端 slice；XY 路由、per-link arbiter、路径先 acquire 再单周期穿越；TLB shootdown 用 **invalidation leader** 减少风暴（L484–648, L790–813）。
- **复用粒度与身份/权限匹配前提**：条目含 valid/translation/context ID；mostly-inclusive；LRU + modulo indexing；shootdown 需按 VA/ASID 精确失效，或由 leader 转发（L496–500, L790–813）。
- **资源共享 vs 映射共享/SVA**：**资源共享**为主（分布共享 slice + 专用互连）；利用翻译去复制。不涉及 SVA。
- **评价配置**：in-house cycle-accurate（Simics 基础）模拟 Haswell 16–64 核；PARSEC + CloudSuite；同时做 **TSMC 28nm/2GHz 的 RTL 综合与 place-and-route** 评估互连延迟/面积/功耗（L649–651, L815–833, L1201–1264）。
- **限制/未覆盖**：仿真 + 综合级硬件，非流片；未涉及设备/IOMMU；未评估安全；hypertreading/PTW latency 作为敏感度，但队列/tail latency 未系统展开。
- **最强反证**：monolithic shared TLB 在 25-cycle 访问延迟下反而比私有 L2 TLB 差 10–15%；即使零互连延迟（16-cycle）也几乎无收益（L302–311, L422–1239）。**“共享更大结构”不自动获益，延迟是决定项**。
- **后续必追引文**：Bhattacharjee et al., HPCA 2011 [34]；Krishna et al., SMART HPCA 2013 [48]；Kim et al., Flattened Butterfly ISCA 2007 [47]；Kumar et al., LATR ASPLOS 2018 [35]。

### B4. translation_cache2010
- **文件/定位**：`papers/translation_cache2010.txt` L1–4（题目/作者）、L80（ISCA’10）、L81（ACM 版权）。
- **精确机制**：MMU cache 设计空间两维（indexing: physical vs virtual；partitioning: unified/split/path），提出 UPTC/SPTC/UTC/STC/TPC 五设计；核心结论是 **translation cache（虚拟地址索引、可跳过若干页表层）** 优于 page table cache（物理地址索引、需逐层）；最优为 **unified translation-cache + 变插入点 LRU（VI-LRU）**（L185–300, L302–436, L640–725）。
- **复用粒度与身份/权限匹配前提**：条目按虚拟地址前缀索引（L4/L3/L2 index），允许匹配最长前缀以跳层；不依赖进程 context 在挑选阶段，但依赖页表一致性（软件失效维护）与 radix 结构（L185–300, L337–347）。
- **资源共享 vs 映射共享/SVA**：既非资源也非跨代理映射共享，而是**单代理内的分层翻译复用**；是后续所有“翻译缓存/页表缓存”的谱系源头。
- **评价配置**：AMD SimNow trace + 自建 memory system simulator，64-entry L1 TLB / 512-entry L2 TLB，L2 data cache 1MB；SPEC CFP2006、SPECjbb2005、Sweep3d；合成 16GB hash join（L437–467）。
- **限制/未覆盖**：trace-based 仿真；不含 instruction/page table loads；未涉及多核共享/IOMMU/设备/SVA；未讨论安全。
- **最强反证**：unified cache 对 large app 会让低复用条目驱逐高复用条目；random replacement 使 unified 退化最严重（UPTC 63%、UTC 49%）。需 level-aware/VI-LRU 才能成立（L640–725）。**“统一缓存”本身会引入层级冲突**。
- **后续必追引文**：Bhargava et al., ASPLOS 2008 [9]；Jacob & Mudge, ASPLOS 1998 [15]；Talluri & Hill, ASPLOS 1994 [23]。

### B5. direct_segments2013
- **文件/定位**：`papers/direct_segments2013.txt` L1–2（题目/作者）、L74（ISCA’13）、L76（ACM 版权）。
- **精确机制**：**direct segment**：per-core BASE/LIMIT/OFFSET 寄存器，若 `BASE ≤ V < LIMIT` 则物理地址 = V + OFFSET，无 TLB miss；其余地址仍走常规分页；OS 提供 primary region 抽象，可与数据页迁移联动，可动态收缩/增长（L124–137, L306–357）。
- **复用粒度与身份/权限匹配前提**：**整段连续虚拟地址范围**——粒度是 segment 级，而非单条 translation；要求范围内统一读写权限、物理连续、不可换出；BASE/LIMIT 以 4KB 边界对齐（L129–147, L321–332）。
- **资源共享 vs 映射共享/SVA**：**避开翻译**而非共享翻译；段内不存在并发映射概念。不涉及 SVA。
- **评价配置**：真实 96GB Sandy Bridge 双路；硬件性能计数器测 TLB miss cycle；内核改动把 TLB miss 变假 page fault 以统计 miss 落在 primary region 的比例；用 4KB 页**模拟** direct-segment 硬件（非真实段硬件执行）（L81–96, L533–604）。
- **限制/未覆盖**：用 4KB 页模拟段硬件，性能是线性外推；不适合动态/短时执行环境；VM 与 kernel 段为展望；未涉及 IOMMU/设备；未讨论安全。
- **最强反证**：大页并非普适。**1GB 页反而使 NPB:CG 的 D-TLB miss 成本从 1.2% 升到 7.1%**，因 1GB 只有 4 个条目；且 direct segment 只对“超过 99% 内存为匿名读写”的工作负载有效（L206–221, L253–268）。**“去分页/大页”不能一概而论**。
- **后续必追引文**：Barr et al., ISCA 2011 (SpecTLB) [3]；Barr et al., ISCA 2010 [4]；Pham et al., CoLT MICRO 2012 [36]。

### B6. victima2023
- **文件/定位**：`papers/victima2023.txt` L1–5（题目/作者）、L12（arXiv:2310.04158v3, 5 Jan 2024）、L1426（“MICRO 2023 reviewers”）。
- **精确机制**：把 TLB 条目放进 **L2 cache block**（TLB block，8×8B PTEs）；L2 miss 或 L2 TLB eviction 时，若 PTW cost predictor (PTW-CP) 判定该页“未来翻译昂贵”，就把含 PTEs 的 block 改造成 TLB block；TLB-aware SRRIP 让 TLB block 更难被驱逐；支持 nested paging（同时存 conventional/nested TLB block）（L112–122, L550–676, L683–776）。
- **复用粒度与身份/权限匹配前提**：每个 TLB block 覆盖 8 个连续虚拟页；硬件 tag 中含 **ASID/VMID** 防别名；page size 信息保留；context switch / shootdown 需使 L2 中 TLB block 失效（L585–676, L836–870, L980–1055）。
- **资源共享 vs 映射共享/SVA**：**资源共享**（复用 cache 存储 + 新增缓存类型）；间接受益于页面局部性，不是跨代理映射共享；不涉及 SVA。
- **评价配置**：扩展版 Sniper 模拟器；11 个 data-intensive workload（GraphBIG/GUPS/XSBench/DLRM/GenomicsBench），4KB+2MB，500M 指令；native 与 virtualized（nested paging）两套（L1042–1094）。
- **限制/未覆盖**：仿真；PTW-CP 用简化 comparator；未涉及 IOMMU/多设备；未覆盖 tail latency、队列深度；安全仅短节讨论（L836–870）。
- **最强反证**：**现实大 TLB 基本不划算**——64K L2 TLB 在 CACTI 延迟下仅 0.8% 提升；大 L3 TLB 仅 2.9%；Victima 依赖“92% L2 block 零复用”的假设（L40–46, L288–494）。若缓存被充分利用，收益假设不成立。
- **后续必追引文**：Ryoo et al., POM-TLB ISCA 2017 [17]；Bhattacharjee et al., HPCA 2011 [14]；Bharadwaj et al., MICRO 2018 [15]；Carlson et al., Sniper SC 2011 [42]。

### B7. revelator2026v2
- **文件/定位**：`papers/revelator2026v2.txt` L1–8（题目/作者）、L14（arXiv:2508.02007v2, 25 Jun 2026）、L1362（感谢 MICRO 2025 / ASPLOS 2026 / ISCA 2026 reviewers）。
- **精确机制**：OS 层 **tiered hash-based allocation**：对每个 VPN 依次用 `PPNi = CityHash(VPN, PID, seedi)` 选空闲物理帧，成功概率 `1-(1-p)^N`；对 last-level PT frame 用 `Hash(VPN>>9)` 单次哈希放帧；硬件 speculation engine 在 L2 TLB miss 时用同哈希重算候选 PA/PTE 并**在翻译完成前**触发推测取数；speculation degree filter 按内存利用率与带宽动态决定发几条候选（L347–570, L628–659, L960–967）。
- **复用粒度与身份/权限匹配前提**：页级粒度（4KB）；哈希输入含 **PID** 以确保跨进程不同 VPN 映射不同候选；PT 帧按 512 VPN 共享；需要 secret per-process hash key（安全），并依赖 OS 维护 per-hash 成功率计数器（L521–570, L836–846）。
- **资源共享 vs 映射共享/SVA**：本质是**把可预测性做进映射**（映射层设计）以支持推测；不是共享硬件资源；虚拟化部分涉及 guest/host 两级翻译（horizontal/diagonal speculation），但仍是 CPU 侧，不是设备 SVA（L737–800）。
- **评价配置**：Virtuoso/Sniper 仿真；单核 11 个 translation-intensive workload；多核 30 个 Google trace mix；8 节点/64 核 NUMA；Linux 6.10.8 原型；Chisel/Yosys 综合（area 0.02%, power 0.03%）（L834–967, L1360–1370）。
- **限制/未覆盖**：**preprint，未确认接收**；hash 侧信道仅定性缓解；未涉及设备/IOMMU；多核/NUMA/virtualized 均为仿真；大量细节（cleanup、NUMA hint、extended flow）推给 extended version（L114, L1362）。
- **最强反证**：静态策略/静态阈值不足（L628–659）；**speculation degree 过大在 80% 利用率下反而 4% slowdown**（L1195）；large-page/contiguity 方案在 fragmentation 下失效（L197–328）。**“多做推测”不是单调收益**。
- **后续必追引文**：Barr et al., SpecTLB ISCA 2011 [88]；Alverti et al., SpOT ISCA 2020 [21]；Skarlatos et al., ECH ASPLOS 2020 [19]；Gosakan et al., Mosaic Pages ASPLOS 2023 [106]；Kanellopoulos et al., Victima MICRO 2023 [18]。

### B8. vmitosis2021
- **文件/定位**：`papers/vmitosis2021.txt` L1–9（题目/作者）、L46–49（ASPLOS’21 / DOI 10.1145/3445814.3446709）。
- **精确机制**：对 **gPT（guest page table）与 ePT（extended page table）** 做迁移与复制，使 2D page walk 尽量本地化。Thin 工作负载用迁移（随数据页迁移增量迁移 gPT/ePT，叶到根传播），Wide 工作负载用复制（gPT 每 vCPU group 一份本地副本；ePT 每 socket 一份副本 + eager 同步 + TLB flush）；支持 NUMA-visible / NUMA-oblivious（para-virtualized NO-P、fully-virtualized NO-F）（L10–30, L336–505）。
- **复用粒度与身份/权限匹配前提**：page-table page 级；迁移以 **PTE 指向的 NUMA socket 计数** 为判据；复制要求 per-vCPU 的 gPT 副本与 per-socket 的 ePT 副本；shootdown/一致性靠 eager 更新 + flush，access/dirty bits 通过 OR 合并（L336–424）。
- **资源共享 vs 映射共享/SVA**：**副本资源 + 映射本地化**；不是设备 SVA。它处理的是 CPU 侧两级翻译的 NUMA 放置。
- **评价配置**：真实 4×24×2 Xeon Gold 6252（Cascade Lake, 1.5TiB, 4 socket）；Linux/KVM 原型；工作负载 Memcached/XSBench/Graph500/Redis/GUPS/BTree/Canneal；Thin 最坏 1.8–3.1×，Wide 1.06–1.6×（NUMA-visible）/1.16–1.4×（NUMA-oblivious）；artifact 有 Zenodo DOI 10.5281/zenodo.4321310（L27–30, L122–131, L526–541, L1563）。
- **限制/未覆盖**：特定 4-socket 服务器；未涉及设备/IOMMU/SVA；迁移/复制有开销（mprotect 复制路径在高尺寸 region 上开销更高）；shadow paging 经验混合，最坏 >5× 退化（L1198–1258）。
- **最强反证**：远端 page-table 可致最多 **3.1× slowdown**；且在 4 socket 上平均约 75% 的 gPT 访问本就是远端的（L19, L1122–1131）。说明**翻译结构的物理放置是真实、可量化的瓶颈**，不能只谈容量。
- **后续必追引文**：Achermann et al., Mitosis ASPLOS 2020 [1]；Gandhi et al., Agile Paging ISCA 2016 [30]；Ryoo et al., POM-TLB ISCA 2017 [65]；Skarlatos et al., ECH ASPLOS 2020 [67]。

### B9. contiguitas2023
- **文件/定位**：`papers/contiguitas2023.txt` L1–17（题目/作者）、L57（ISCA’23）、L60–61（DOI 10.1145/3579371.3589079）。
- **精确机制**：
  1. OS 侧把 movable 与 unmovable 分配放入两个连续区，动态按 pressure（扩展 PSI）调整边界，unmovable 内按生命周期做“远离边界”放置；
  2. **Contiguitas-HW** 在 LLC 内做源/目的双映射，用 `Ptr` 标记已拷贝进度，按 `addr offset < Ptr` 决定读源还是目的；支持 noncacheable 与 cacheable 两种访问模型；分片 LLC 下跨 slice 拷贝用 slice-selection hash；
  3. **失效降级**：不再同步 IPI 全核确认，而是每核本地、lazy 失效；NIC/IOMMU 的失效请求通过内存队列提交（L134–144, L188–256, L516–712, L892–910）。
- **复用粒度与身份/权限匹配前提**：页级迁移（4KB）；I/O 相关 unmovable 页（网络缓冲、IOMMU 相关）占 73%；硬件需 OS 提供源/目的 PPN；metadata table 16 项 FA；30μs 窗口内完成本地失效（L369–373, L516–553, L918–935）。
- **资源共享 vs 映射共享/SVA**：**LLC 资源共享 + 迁移期间的映射双活**；**明确提及 IOMMU/IOTLB、NIC 私有 TLB**，以及设备失效队列，是本组最接近“设备侧共享地址翻译失效语义”的一篇，但**并未实现或评测 SVA 地址空间共享本身**。
- **评价配置**：Meta 生产环境（Web/Cache A/Cache B + CI，真实流量）+ QEMU+SST+DRAMSim3 full-system 模拟（8 核 64GB）；生产结果 2–9%（部分碎片）/7–18%（高碎片）；1GB 页带来额外 7.5%；Contiguitas-HW 仅 0.014% core 面积（L726–742, L769–910, L918–943）。
- **限制/未覆盖**：Contiguitas-HW 仅模拟验证；unmovable 区内仍有 22% 内部碎片；未给出设备侧队列深度/tail latency 的量化；未给出严格隔离 vs lazy 失效的安全语义讨论；未实现设备共享地址空间（SVA）。
- **最强反证**：**fragmentation 与服务器 uptime 几乎不相关**（Pearson 0.00286；短 uptime 也只有 0.16），且在启动后 1 小时内即可高度碎片化（L318–326）。**不能假设“重启/新机就有大页/contiguity”**；同时硬件 IOTLB 失效语义在本组仅被定性描述。
- **后续必追引文**：Peleg et al., “Utilizing the IOMMU Scalably”, ATC 2015 [80]；Kumar et al., LATR ASPLOS 2018 [63]；Romanescu et al., UNITD HPCA 2010 [90]；Yan et al., Translation Ranger ISCA 2019 [114]；AMD/Intel IOMMU 规范 [5][55]。

### B10. cross2025
- **文件/定位**：`papers/cross2025.txt` L1（HPCA 2025）、L5–10（题目/作者）。
- **精确机制**：**MOKA 框架**设计 Page-Cross Filter：hashed perceptron predictors（program features）+ saturating counters for system features（sTLB MPKI、sTLB Miss Rate）+ vUB/pUB 训练 + epoch 自适应阈值；实例化 **DRIPPER**，对 Berti/IPCP/BOP 决定是否发出 page-cross prefetch；被允许时经 TLB 层级，必要时触发 speculative page walk（L13–52, L357–647）。
- **复用粒度与身份/权限匹配前提**：L1D 预取请求级（跨页边界判定）；program feature 用 VA/PC/delta；system feature 用 sTLB MPKI/Miss Rate；无身份/权限匹配机制（不涉及 ASID/context 的直接比较）（L519–524, L648–722）。
- **资源共享 vs 映射共享/SVA**：属**单核 L1D 预取与 TLB 交互**；不共享翻译结构，也不涉及 SVA。
- **评价配置**：ChampSim，1 核/8 核，5-level radix、x86 page walker、split PSC；396 workload（218 seen / 178 unseen），300 个 8 核 mix；DRIPPER 硬件 1.44KB（L723–753, L666–671）。
- **限制/未覆盖**：仿真；未涉及设备/IOMMU/SVA；未覆盖 page table walk 队列/tail latency；以 4KB 为主（2MB 作为敏感度）。
- **最强反证**：静态“永远允许”与“永远禁止”都非普遍最优；SOTA L1D 预取器 page-cross 平均仅约 50% 有用；PPF（SOTA 过滤）转用于 page-cross 反而劣于 Discard PGC（L194–329, L883–893）。**“允许跨界/复用翻译”必须动态过滤**。
- **后续必追引文**：Vavouliotis et al., Page Size Aware Cache Prefetching MICRO 2022 [89]；Navarro-Torres et al., Berti MICRO 2022 [60]；Michaud, BOP HPCA 2016 [57]；Bhatia et al., PPF ISCA 2019 [20]。

---

## C. 被证据支持的剩余瓶颈与不能断言的点

### C1. 被本组证据支持的瓶颈
1. **访问延迟 vs 命中率的根本权衡**：共享/更大 TLB 提高命中率，但 SRAM 与互连延迟同步上升；monolithic shared TLB 在真实延迟下可比私有更差（nocstar2018 L302–311, L422–1239）；realistic 64K TLB 提升仅约 0.8%（victima2023 L359）。**共享结构不自动获益**。
2. **物理内存 contiguity / 大页可用性是硬约束**：23% 服务器连一个 2MB 页都凑不出，1GB 动态分配“实际上不可能”；碎片与 uptime 弱相关（contiguitas2023 L277–329, L318–326）。这一约束直接限制 contiguity 类与 large-page 类翻译优化。
3. **失效（invalidation）成本随代理数线性增长**：经典 shootdown 需同步全核确认，随 victim TLB 数线性变差；lazy / 本地失效 + 双活映射是已证据支持的方向（contiguitas2023 L188–256, L906–910）。但**严格隔离 vs lazy 失效的安全语义**在本组未被正式分析——仅为设计取舍，不能当作已解决。
4. **翻译结构的物理放置（NUMA / slice / 远端）是独立瓶颈**：vmitosis2021 证明远端 gPT/ePT 可达 3.1× 慢，且 4-socket 上约 75% gPT 访问本就远端（L19, L1122–1131）。
5. **推测/预取的准确率与副作用**：跨核翻译盲推（icc2010 bad prefetch 21–38%）、跨页预取（cross2025 page-cross 有用性约 50%）、过度推测（revelator at 80% util 4% slowdown）均表明**“共享/推测”需按收益与带宽动态开关**。
6. **容量不是唯一或首要因素**：大 TLB、大 cache-TLB、更大 reach 的收益被延迟/面积/功耗抵消（victima2023 L40–46；nocstar2018 L422–1239）。

### C2. 不能断言的（本组全文可支持范围）
- **设备侧 SVA 未被本组实现或评测**：除 contiguitas2023 提及 IOMMU/IOTLB/NIC TLB 与失效队列外，其余均为 CPU 核间 TLB / 页表机制。**不能说“商用 IOMMU 已实现上述共享翻译机制”**。
- **IOTLB 吞吐墙的归因范围未给定**：本组没有给出内核/设备/工作负载维度上 IOTLB 吞吐瓶颈的系统量化。**不能把吞吐问题都归因给“容量不足”**；延迟、队列深度、失效风暴、带宽都可能是因素，本组只支持“延迟与失效成本是重要项”。
- **动态映射 / 迁移 / 失效 / 远端 PTW / 队列 / 容量 / 尾延迟**：本组各自覆盖其中一部分（contiguitas 的迁移与失效、vmitosis 的迁移/复制、nocstar 的队列/contention、revelator 的推测与清理），但**没有一篇同时给出多设备共享地址翻译下的完整 tail latency / 队列深度 / 严格隔离语义**。这些是“本组未覆盖”，**不能据此推断“全领域无人做”**。
- **安全语义**：victima2023 与 revelator2026v2 都做了有限的侧信道讨论（victima L836–870；revelator L836–846），但**严格隔离、lazy 失效、共享翻译结构下的安全保证**没有在本组被形式化。不能把任一设计读作“已满足严格隔离”。
- **元数据**：`slltlb2011` 所给全文题头未含会议/年份/DOI，故记 null；`revelator2026v2` 为 arXiv 预印本，感谢信提及多个会议审稿但未见接收结论，故记 preprint。其余各篇的 venue/年份/DOI 均以上述题头证据为准。

---

```json
{
  "papers": [
    {
      "id": "icc2010",
      "title": "Inter-Core Cooperative TLB Prefetchers for Chip Multiprocessors",
      "authors": ["Abhishek Bhattacharjee", "Margaret Martonosi"],
      "year": 2010,
      "venue": "ASPLOS'10 (Pittsburgh, PA, USA)",
      "doi": null,
      "publication_status": "published",
      "relevance": "foundation",
      "evaluation_type": ["simulation"],
      "mechanism": "面向CMP的跨核协作TLB预取：Leader-Follower把leader核的翻译push到follower的Prefetch Buffer；Distance-based Cross-Core用共享Distance Table记录distance-pair并pull预取。",
      "assumptions": [
        "多个核在1M指令窗口内对相同虚拟页/上下文产生TLB miss（ICS）或可预测stride（ICPS）",
        "PB与TLB可并行查找",
        "shootdown会同时使PB中匹配条目失效",
        "采用non-faulting prefetch"
      ],
      "limitations": [
        "仅GEMS/Simics仿真，无实机",
        "不涉及设备/IOMMU共享地址翻译",
        "未评估安全语义",
        "superpaging在实际系统中未被使用",
        "多程序集适配仅作简述"
      ],
      "evidence": [
        {"file": "papers/icc2010.txt", "location": "L1-L3", "claim": "题名与作者：Inter-Core Cooperative TLB Prefetchers for Chip Multiprocessors, Bhattacharjee & Martonosi"},
        {"file": "papers/icc2010.txt", "location": "L66-L67", "claim": "ASPLOS'10, March 13-17, 2010, Pittsburgh；ACM 978-1-60558-839-1/10/03"},
        {"file": "papers/icc2010.txt", "location": "L116-L138", "claim": "ICS/ICPS定义：身份匹配要求virtual page, physical page, context ID, protection, page size一致"},
        {"file": "papers/icc2010.txt", "location": "L210-L320", "claim": "Leader-Follower算法与confidence estimation"},
        {"file": "papers/icc2010.txt", "location": "L301-L304", "claim": "shootdown-aware：失效消息同时使PB失效"},
        {"file": "papers/icc2010.txt", "location": "L463-L467", "claim": "1 billion instructions模拟，cold miss<5%"},
        {"file": "papers/icc2010.txt", "location": "L644-L660", "claim": "bad prefetch在无confidence时平均38%，2-bit confidence降到21%"}
      ]
    },
    {
      "id": "slltlb2011",
      "title": "Shared Last-Level TLBs for Chip Multiprocessors",
      "authors": ["Abhishek Bhattacharjee", "Daniel Lustig", "Margaret Martonosi"],
      "year": null,
      "venue": null,
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "foundation",
      "evaluation_type": ["simulation"],
      "mechanism": "私有L1 TLB + 中心共享SLL L2 TLB；mostly-inclusive、shootdown-aware；可选简单stride预取(±1,2,3页)。",
      "assumptions": [
        "L1与SLL mostly-inclusive（>97% L1条目在SLL中）",
        "SLL TLB为blocking",
        "shootdown需检查所有层级",
        "条目存储完整context/process ID"
      ],
      "limitations": [
        "题头未含出版信息，venue/year无法确认",
        "仿真，无实机",
        "不涉及设备/IOMMU/SVA",
        "未评估安全",
        "对部分应用（cactusADM）反而轻微降级"
      ],
      "evidence": [
        {"file": "papers/slltlb2011.txt", "location": "L1-L6", "claim": "题名与作者（全文题头未含会议/年份/DOI）"},
        {"file": "papers/slltlb2011.txt", "location": "L17-L34", "claim": "首个提出并评估SLL TLB；消除7-79% miss"},
        {"file": "papers/slltlb2011.txt", "location": "L160-L191", "claim": "SLL TLB条目含完整context/process ID；mostly-inclusive；两级独立替换"},
        {"file": "papers/slltlb2011.txt", "location": "L192-L245", "claim": "shootdown-aware与集中放置，22-cycle roundtrip"},
        {"file": "papers/slltlb2011.txt", "location": "L256-L275", "claim": "可选择加入stride预取(±1,2,3页)"},
        {"file": "papers/slltlb2011.txt", "location": "L988-L1074", "claim": "cactusADM因TLB复用极差反而轻微降级"},
        {"file": "papers/slltlb2011.txt", "location": "L502-L505", "claim": "Blackscholes（高ICPS）下SLL不如ICC prefetcher"}
      ]
    },
    {
      "id": "nocstar2018",
      "title": "Scalable Distributed Last-Level TLBs Using Low-Latency Interconnects",
      "authors": ["Srikant Bharadwaj", "Guilherme Cox", "Tushar Krishna", "Abhishek Bhattacharjee"],
      "year": 2018,
      "venue": "MICRO 2018",
      "doi": "10.1109/MICRO.2018.00030",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["simulation", "hardware_prototype", "analysis"],
      "mechanism": "分布式SLL TLB slice + latchless、单周期、电路交换专用互连(NOCSTAR)；XY路由、per-link arbiter、路径先acquire后穿越；TLB shootdown使用invalidation leader。",
      "assumptions": [
        "L1/L2 TLB mostly-inclusive，LRU+modulo indexing",
        "每slice有请求队列；路径可在单周期setup并穿越",
        "XY路由与静态优先级仲裁",
        "shootdown先到invalidation leader再转发"
      ],
      "limitations": [
        "仿真+28nm RTL综合，非流片",
        "不涉及设备/IOMMU/SVA",
        "未评估安全",
        "队列/tail latency仅在contention/storm microbenchmark中局部涉及"
      ],
      "evidence": [
        {"file": "papers/nocstar2018.txt", "location": "L1-L11", "claim": "题名/作者；MICRO 2018"},
        {"file": "papers/nocstar2018.txt", "location": "L68-L69", "claim": "DOI 10.1109/MICRO.2018.00030"},
        {"file": "papers/nocstar2018.txt", "location": "L484-L500", "claim": "TLB组织：每核附近分布式slice，≤当前私有L2 TLB"},
        {"file": "papers/nocstar2018.txt", "location": "L502-L648", "claim": "latchless switch + fine-grained circuit switching；路径setup/grant"},
        {"file": "papers/nocstar2018.txt", "location": "L790-L813", "claim": "shootdown用invalidation leader减少风暴"},
        {"file": "papers/nocstar2018.txt", "location": "L302-L311", "claim": "monolithic shared TLB在25-cycle访问延迟下比私有L2 TLB差10-15%"},
        {"file": "papers/nocstar2018.txt", "location": "L649-L651", "claim": "TSMC 28nm 2GHz place-and-route"},
        {"file": "papers/nocstar2018.txt", "location": "L1201-L1264", "claim": "invalidation方式与PTW策略的敏感度研究"}
      ]
    },
    {
      "id": "translation_cache2010",
      "title": "Translation Caching: Skip, Don't Walk (the Page Table)",
      "authors": ["Thomas W. Barr", "Alan L. Cox", "Scott Rixner"],
      "year": 2010,
      "venue": "ISCA'10 (Saint-Malo, France)",
      "doi": null,
      "publication_status": "published",
      "relevance": "foundation",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "MMU cache设计空间（page table cache vs translation cache；unified/split/path）；提出UPTC/SPTC/UTC/STC/TPC；最优为unified translation cache + 变插入点LRU(VI-LRU)。",
      "assumptions": [
        "x86-64四级radix page table",
        "trace不含instruction/page table loads；4KB页",
        "L2 data cache 1MB；TLB配置近似Opteron",
        "页表一致性由软件维护"
      ],
      "limitations": [
        "trace-driven模拟，无实机",
        "未涉及多核共享/IOMMU/设备/SVA",
        "未讨论安全"
      ],
      "evidence": [
        {"file": "papers/translation_cache2010.txt", "location": "L1-L4", "claim": "题名与作者"},
        {"file": "papers/translation_cache2010.txt", "location": "L80-L81", "claim": "ISCA'10, June 19-23, 2010, Saint-Malo；ACM 978-1-4503-0053-7/10/06"},
        {"file": "papers/translation_cache2010.txt", "location": "L185-L300", "claim": "page table cache与translation cache设计区分；五种结构"},
        {"file": "papers/translation_cache2010.txt", "location": "L302-L436", "claim": "indexing/partitioning/coverage/complexity比较"},
        {"file": "papers/translation_cache2010.txt", "location": "L437-L455", "claim": "trace方法与模拟器"},
        {"file": "papers/translation_cache2010.txt", "location": "L640-L725", "claim": "VI-LRU；统一cache对large app的冲突与缓解"}
      ]
    },
    {
      "id": "direct_segments2013",
      "title": "Efficient Virtual Memory for Big Memory Servers",
      "authors": ["Arkaprava Basu", "Jayneel Gandhi", "Jichuan Chang", "Mark D. Hill", "Michael M. Swift"],
      "year": 2013,
      "venue": "ISCA'13 (Tel-Aviv, Israel)",
      "doi": null,
      "publication_status": "published",
      "relevance": "foundation",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "per-core BASE/LIMIT/OFFSET寄存器的direct segment：BASE≤V<LIMIT时物理地址=V+OFFSET，无TLB miss；其余地址走常规分页；OS提供primary region抽象并可与数据页迁移联动。",
      "assumptions": [
        "big-memory工作负载早期分配、运行稳定",
        "段内统一读写权限、物理连续",
        "连续物理内存可预留或compaction获得",
        "以4KB页模拟段硬件，TLB miss减少线性映射为cycle降低"
      ],
      "limitations": [
        "无真实段硬件执行，用4KB页模拟",
        "不适合短时/动态执行环境",
        "VM与kernel段为展望",
        "不涉及设备/IOMMU/SVA"
      ],
      "evidence": [
        {"file": "papers/direct_segments2013.txt", "location": "L1-L2", "claim": "题名与作者"},
        {"file": "papers/direct_segments2013.txt", "location": "L74-L76", "claim": "ISCA'13 Tel-Aviv；ACM 978-1-4503-2079-5/13/06"},
        {"file": "papers/direct_segments2013.txt", "location": "L124-L137", "claim": "direct segment定义：BASE/LIMIT/OFFSET、无TLB miss；段外走分页"},
        {"file": "papers/direct_segments2013.txt", "location": "L306-L357", "claim": "硬件支持：段硬件与并行D-TLB译码"},
        {"file": "papers/direct_segments2013.txt", "location": "L206-L221", "claim": "99%以上内存为读写权限的动态匿名区"},
        {"file": "papers/direct_segments2013.txt", "location": "L253-L268", "claim": "1GB页反而使NPB:CG的D-TLB miss成本从1.2%升到7.1%"},
        {"file": "papers/direct_segments2013.txt", "location": "L533-L604", "claim": "用4KB页模拟direct segment的评估方法"}
      ]
    },
    {
      "id": "victima2023",
      "title": "Victima: Drastically Increasing Address Translation Reach by Leveraging Underutilized Cache Resources",
      "authors": ["Konstantinos Kanellopoulos", "Hong Chul Nam", "F. Nisa Bostanci", "Rahul Bera", "Mohammad Sadrosadati", "Rakesh Kumar", "Davide Basilio Bartolini", "Onur Mutlu"],
      "year": 2023,
      "venue": "MICRO 2023",
      "doi": null,
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["simulation", "analysis"],
      "mechanism": "把TLB条目存入L2 cache block(TLB block, 8 entries/64B)；PTW cost predictor决定是否存储；TLB-aware SRRIP优先保留TLB block；支持nested paging。",
      "assumptions": [
        "L2 cache大量block零复用（平均92%）",
        "PTW-CP(comparator, F1≈80.66%)预测足够准确",
        "ASID/VMID位足以防别名",
        "硬件支持TLB block tag匹配与失效（context switch/shootdown）"
      ],
      "limitations": [
        "仿真，无实机",
        "PTW-CP为简化comparator",
        "不涉及设备/IOMMU",
        "tail latency与队列深度未系统展开",
        "安全仅短节讨论"
      ],
      "evidence": [
        {"file": "papers/victima2023.txt", "location": "L1-L5", "claim": "题名与作者"},
        {"file": "papers/victima2023.txt", "location": "L12", "claim": "arXiv:2310.04158v3 [cs.AR] 5 Jan 2024"},
        {"file": "papers/victima2023.txt", "location": "L1426", "claim": "感谢MICRO 2023审稿人，表明发表于MICRO 2023"},
        {"file": "papers/victima2023.txt", "location": "L112-L122", "claim": "关键机制：TLB block + PTW-CP + TLB-aware SRRIP"},
        {"file": "papers/victima2023.txt", "location": "L585-L676", "claim": "TLB block结构与ASID/VMID；cache block metadata修改"},
        {"file": "papers/victima2023.txt", "location": "L683-L776", "claim": "PTW-CP设计：PTW频率/成本计数器 + comparator"},
        {"file": "papers/victima2023.txt", "location": "L836-L870", "claim": "context switch与shootdown下的TLB block失效"},
        {"file": "papers/victima2023.txt", "location": "L359", "claim": "realistic 64K L2 TLB仅带来约0.8%额外性能"}
      ]
    },
    {
      "id": "revelator2026v2",
      "title": "Revelator: Rapid Data Fetching via System-Software-Guided Hash-based Speculative Address Translation",
      "authors": ["Konstantinos Kanellopoulos", "Konstantinos Sgouras", "Harsh Songara", "Andreas Kosmas Kakolyris", "Vlad-Petru Nitu", "Spiros Galanopoulos", "Rahul Bera", "Konstantina Koliogeorgi", "Rakesh Kumar", "Onur Mutlu"],
      "year": 2026,
      "venue": null,
      "doi": null,
      "publication_status": "preprint",
      "relevance": "nearby",
      "evaluation_type": ["simulation", "measurement", "analysis"],
      "mechanism": "OS层tiered hash-based allocation（数据页与末级PT帧）使VA→PA/VA→PTE可预测；L2 TLB miss时硬件speculation engine用同一CityHash重算候选PPN/PTE并提前取数；speculation degree filter按利用率与带宽控制推测度。",
      "assumptions": [
        "hash-based allocation成功概率仅取决于内存利用率1-(1-p)^N",
        "哈希输入含VPN、PID、tier seed，OS与硬件可重算",
        "OS维护per-hash成功率计数器；使用secret per-process hash key",
        "non-faulting推测取数；误推测可通过MSHR/日志清理"
      ],
      "limitations": [
        "arXiv预印本，未确认接收（感谢信提及MICRO 2025/ASPLOS 2026/ISCA 2026）",
        "未涉及设备/IOMMU/SVA",
        "多核/NUMA/虚拟化均仿真",
        "cleanup、NUMA hint、扩展流程放在extended version",
        "安全侧信道仅定性缓解"
      ],
      "evidence": [
        {"file": "papers/revelator2026v2.txt", "location": "L1-L8", "claim": "题名与作者"},
        {"file": "papers/revelator2026v2.txt", "location": "L14", "claim": "arXiv:2508.02007v2 [cs.AR] 25 Jun 2026"},
        {"file": "papers/revelator2026v2.txt", "location": "L28-L57", "claim": "摘要：hash-based speculative translation + speculation degree filter"},
        {"file": "papers/revelator2026v2.txt", "location": "L347-L495", "claim": "motivation：contiguity与large-page方案在fragmentation下失效"},
        {"file": "papers/revelator2026v2.txt", "location": "L521-L570", "claim": "tiered hash-based allocation：PPNi=CityHash(VPN,PID,seedi)；1-(1-p)^N概率"},
        {"file": "papers/revelator2026v2.txt", "location": "L583-L620", "claim": "末级PT frame用Hash(VPN>>9)单次分配"},
        {"file": "papers/revelator2026v2.txt", "location": "L628-L659", "claim": "speculation degree filter：利用率与带宽监视+过滤"},
        {"file": "papers/revelator2026v2.txt", "location": "L836-L846", "claim": "安全：secret per-process hash key + 误推测清理"},
        {"file": "papers/revelator2026v2.txt", "location": "L1362", "claim": "感谢MICRO 2025、ASPLOS 2026、ISCA 2026审稿人"}
      ]
    },
    {
      "id": "vmitosis2021",
      "title": "Fast Local Page-Tables for Virtualized NUMA Servers with vMitosis",
      "authors": ["Ashish Panwar", "Reto Achermann", "Arkaprava Basu", "Abhishek Bhattacharjee", "K. Gopinath", "Jayneel Gandhi"],
      "year": 2021,
      "venue": "ASPLOS '21 (Virtual, USA)",
      "doi": "10.1145/3445814.3446709",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "对gPT与ePT做迁移与复制，使2D page walk本地化；Thin用迁移（随数据页迁移增量传播），Wide用复制（gPT每vCPU group一份本地副本，ePT每socket一份副本）；支持NUMA-visible/oblivious（NO-P/NO-F）。",
      "assumptions": [
        "服务器NUMA拓扑可被guest或hypervisor利用/探测",
        "page table可迁移或复制",
        "gPT对hypervisor而言是普通guest数据页；ePT被hypervisor固定",
        "AutoNUMA可用于驱动迁移"
      ],
      "limitations": [
        "特定4-socket Cascade Lake服务器",
        "不涉及设备/IOMMU/SVA",
        "shadow paging经验混合，最坏>5x退化",
        "迁移/复制有开销，mprotect复制路径开销随region增大"
      ],
      "evidence": [
        {"file": "papers/vmitosis2021.txt", "location": "L1-L9", "claim": "题名与作者"},
        {"file": "papers/vmitosis2021.txt", "location": "L46-L49", "claim": "ASPLOS'21；DOI 10.1145/3445814.3446709"},
        {"file": "papers/vmitosis2021.txt", "location": "L19", "claim": "page table放置不当最多3.1x slowdown"},
        {"file": "papers/vmitosis2021.txt", "location": "L67-L135", "claim": "2D page table放置分析；<10% walk为Local-Local"},
        {"file": "papers/vmitosis2021.txt", "location": "L336-L411", "claim": "迁移设计：增量迁移、叶到根传播"},
        {"file": "papers/vmitosis2021.txt", "location": "L413-L505", "claim": "复制设计：gPT per-vCPU副本、ePT per-socket副本与eager同步"},
        {"file": "papers/vmitosis2021.txt", "location": "L67-L79", "claim": "表1：Migration/Replication在Linux/KVM、Mitosis、vMitosis的对比"},
        {"file": "papers/vmitosis2021.txt", "location": "L1236-L1246", "claim": "复制内存开销：每层副本约0.4%，四副本约1.2%"}
      ]
    },
    {
      "id": "contiguitas2023",
      "title": "Contiguitas: The Pursuit of Physical Memory Contiguity in Datacenters",
      "authors": ["Kaiyang Zhao", "Kaiwen Xue", "Ziqi Wang", "Dan Schatzberg", "Leon Yang", "Antonis Manousis", "Johannes Weiner", "Rik van Riel", "Bikash Sharma", "Chunqiang Tang", "Dimitrios Skarlatos"],
      "year": 2023,
      "venue": "ISCA '23 (Orlando, FL, USA)",
      "doi": "10.1145/3579371.3589079",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["measurement", "simulation"],
      "mechanism": "OS把movable与unmovable分配放入两个连续区并动态按pressure调边界；LLC硬件扩展(Contiguitas-HW)对源/目的页做双映射，Ptr标记拷贝进度，并在迁移期间允许双活；shootdown由同步IPI降级为每核本地lazy失效；IOMMU/NIC的失效通过内存队列提交。",
      "assumptions": [
        "网络等I/O产生unmovable页（占73%）",
        "IOMMU有local TLB、NIC有cache-coherent TLB，并可经内存队列接收失效请求",
        "本地失效可在30us窗口内完成；元数据表16项FA",
        "存在足够连续物理内存用于初始unmovable区"
      ],
      "limitations": [
        "Contiguitas-HW仅全系统仿真验证",
        "unmovable区内部仍有22%碎片",
        "未实现/评测设备共享地址空间(SVA)",
        "未给出设备侧队列深度/tail latency量化",
        "未形式化lazy失效 vs 严格隔离的安全语义"
      ],
      "evidence": [
        {"file": "papers/contiguitas2023.txt", "location": "L1-L17", "claim": "题名与作者"},
        {"file": "papers/contiguitas2023.txt", "location": "L57-L61", "claim": "ISCA'23, June 17-21, 2023, Orlando；DOI 10.1145/3579371.3589079"},
        {"file": "papers/contiguitas2023.txt", "location": "L129-L133", "claim": "网络分配占unmovable页的73%"},
        {"file": "papers/contiguitas2023.txt", "location": "L188-L256", "claim": "页迁移与TLB shootdown流程与成本"},
        {"file": "papers/contiguitas2023.txt", "location": "L318-L326", "claim": "fragmentation与uptime弱相关（Pearson 0.00286）"},
        {"file": "papers/contiguitas2023.txt", "location": "L516-L553", "claim": "Contiguitas-HW概述；目标平台含IOMMU local TLB与NIC TLB，失效经内存队列"},
        {"file": "papers/contiguitas2023.txt", "location": "L558-L712", "claim": "硬件操作：双映射、Ptr、noncacheable vs cacheable访问、分片LLC"},
        {"file": "papers/contiguitas2023.txt", "location": "L804-L813", "claim": "生产性能提升2-18%；Web用1GB页额外7.5%"},
        {"file": "papers/contiguitas2023.txt", "location": "L906-L910", "claim": "经典shootdown阻塞时间随核心数线性增加；Contiguitas-HW成本恒为一次本地TLB失效"}
      ]
    },
    {
      "id": "cross2025",
      "title": "To Cross, or Not to Cross Pages for Prefetching?",
      "authors": ["Georgios Vavouliotis", "Marti Torrents", "Boris Grot", "Kleovoulos Kalaitzidis", "Leeor Peled", "Marc Casas"],
      "year": 2025,
      "venue": "HPCA 2025",
      "doi": null,
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["simulation"],
      "mechanism": "MOKA框架设计Page-Cross Filter：hashed perceptron(program features)+system features计数器(sTLB MPKI/Miss Rate)+vUB/pUB训练+epoch自适应阈值；实例化DRIPPER用于Berti/IPCP/BOP，决定是否发出page-cross预取。",
      "assumptions": [
        "L1D使用VIPT，prefetcher可直接访问TLB层级",
        "page-cross可能触发speculative page walk",
        "有用性定义为在cache生命周期内至少一次命中",
        "DRIPPER硬件开销约1.44KB/核"
      ],
      "limitations": [
        "仿真，无实机",
        "不涉及设备/IOMMU/SVA",
        "未覆盖page walk队列/tail latency",
        "以4KB页为主，2MB仅作敏感度"
      ],
      "evidence": [
        {"file": "papers/cross2025.txt", "location": "L1-L10", "claim": "HPCA 2025；题名与作者"},
        {"file": "papers/cross2025.txt", "location": "L13-L52", "claim": "摘要：page-cross预取静态策略均非普遍最优"},
        {"file": "papers/cross2025.txt", "location": "L194-L329", "claim": "Permit PGC与Discard PGC在不同负载上互有胜负"},
        {"file": "papers/cross2025.txt", "location": "L318", "claim": "SOTA L1D预取器page-cross平均约50%有用"},
        {"file": "papers/cross2025.txt", "location": "L357-L417", "claim": "MOKA设计概览：perceptron+system features+自适应阈值"},
        {"file": "papers/cross2025.txt", "location": "L540-L647", "claim": "自适应阈值方案与极值检测"},
        {"file": "papers/cross2025.txt", "location": "L666-L671", "claim": "DRIPPER硬件开销1.44KB/核"},
        {"file": "papers/cross2025.txt", "location": "L883-L893", "claim": "PPF转用于page-cross反而不如Discard PGC"}
      ]
    }
  ]
}
```