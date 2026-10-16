# G13_tiering 证据阅读报告：多设备共享地址翻译、分层页放置与“关键性”前沿

## 0. 导航摘要（不超过一页）

本组全文可支持的主线不是“IOMMU/SVA 共享页表”本身，而是**分层/池化内存里数据放置、观测、迁移、页大小与动态映射**；它们与多设备共享地址翻译相邻，但不能直接等同。全文里唯一直接提到 ATS/PRI、vIOMMU 的是 Pond，但 Pond 明确说 ATS/PRI 设备尚不广泛、vIOMMU 需要 guest OS 改动且有开销，因此 Pond 自己走的是 CXL池 + hypervisor页表 + zNUMA，不是 IOMMU SVA 复用方案（`pond2023.txt` L1192-L1219）。因此：**应用使用 VM、GPU UVM、CXL pooling，不等于 IOMMU/SVA 复用；共享物理资源、共享映射、共享缓存/计数器是不同层。**

本组最硬的可支持结论：

1. **“热度 ≠ 性能关键性”已被 Soar/Alto 和 PACT 正面论证。** Soar/Alto 用 AOL 说明热页放快层可能反而变慢（`soaralto2025.txt` L153-L194）；PACT 用 PAC 说明同频页可差 65× stall（`pact2026.txt` L301-L309，L286-L299）。
2. **页迁移是否要复制数据，取决于干净/脏、是否多映射、是否 huge page、是否有 shadow copy。** Nomad 的 TPM 先复制、后查脏、脏则 abort；干净 shadow 可直接 remap 降级；多映射页禁用 TPM（`nomad2024.txt` L386-L401，L448-L485，L487-L528，L530-L555）。
3. **页大小与 TLB/地址翻译成本是中心变量。** Memtis 发现 huge page 内部子页热度倾斜，动态 split；但 split 昂贵、涉及 copy 和 TLB shootdown（`memtis2023.txt` L77-L90，L390-L397，L522-L527）。vAttention 用 CUDA VMM 把虚拟/物理分配解耦，并改驱动支持 64KB 页；它报告 64KB 页对 attention kernel 无明显 TLB thrash（`vattention2025.txt` L1180-L1223）。
4. **观测是当前卡点，不是附属功能。** Memtis 用 PEBS 但有“冷页采样不足”限制（`memtis2023.txt` L932-L939）；Soar/Alto 的 AOL 需要离线校准且带宽竞争下 AOL 膨胀（`soaralto2025.txt` L1119-L1117，L1052-L1113）；PACT 用 CHA/TOR 做 per-tier MLP（`pact2026.txt` L334-L463）；ObservUVM 把 PCIe access counters 反用于 HBM 观测（`observuvm2026.txt` L73-L120，L322-L332，L373-L410）。
5. **在线性、页粒度、是否依赖离线 profiling，是 Soar/Alto 与 PACT 的核心分歧。** Soar 是离线 object profiling + 静态 allocation；Alto 是运行时 AOL 调节迁移。PACT 声称首个 online、page-granular、criticality-first 系统（`pact2026.txt` L70-L72，L927-L953）。但 PACT 自己承认在部分 workload 上 Soar 的离线洞察更好（`pact2026.txt` L927-L953），不能反推 PACT 全面胜出。

本组对照矩阵：

| 论文 | 优化段 | 信息何处产生 | 何时必须完成 | 维护成本谁承担 |
|---|---|---|---|---|
| Memtis 2023 | 页放置 + page size | PEBS采样、page/histogram、subpage metadata | 后台500ms周期迁移；split在后台 | OS kernel background threads |
| Soar 2025 | 初始对象放置 | 离线fast-tier profiling、PEBS、AOL、对象调用链 | 分配前/分配时 | 应用 allocator / profiling run |
| Alto 2025 | 迁移强度调节 | perf counters、AOL、1s采样 | 运行中1s级 | kernel ~30 LOC + user tool |
| Nomad 2024 | 迁移机制 + 非独占shadow | page fault、PTE dirty bit、shadow XArray | 访问时/淘汰时 | kernel driver，TLB shootdown |
| Pond 2023 | CXL池切片 + VM放置 | ML模型、PMU、access bits、生产trace | VM调度时/QoS异常时 | cloud control plane、hypervisor、EMC |
| gpu_ext 2025 | GPU内存/调度策略 | eBPF hooks、跨层maps、设备侧事件 | page fault/eviction/warp hook时 | driver extension + admin/eBPF |
| vAttention 2025 | KV cache虚拟/物理解耦 | CUDA VMM APIs、step API | 每迭代前，后台重叠 | serving framework + NVIDIA driver patch |
| PACT 2026 | 页级criticality + eager demotion/adaptive promotion | PEBS + TOR queue occupancy + PAC | 20ms在线采样，后台迁移 | kernel/perf + 2线程 |
| ObservUVM 2026（旧对比） | GPU HBM eviction/prefetch | 反用PCIe access counters、采样页 | 访问/淘汰时 | modified UVM driver + userspace/eBPF |

---

## A. 时间与机制演化解释

### 2023：分层放置开始系统化；同时出现 CXL池化与“VM ≠ IOMMU复用”
- **Memtis** 主张 tiered memory 的核心是 access distribution-based placement + skewness-aware page size。它用 PEBS 采样子页访问，维护 page access histogram 和 emulated base page histogram，动态定 hot/warm/cold 阈值，后台迁移，huge page 低利用时 split（`memtis2023.txt` L90-L126，L353-L397，L440-L492）。
- **Pond** 从云生产环境出发，把问题定义成 memory stranding 和 DRAM成本，提出 CXL pool + zNUMA + ML预测。它用 hypervisor页表和 CXL.mem load/store，不走 page fault；明确把 ATS/PRI、vIOMMU 列为替代/未广泛可用路径（`pond2023.txt` L99-L134，L181-L210，L724-L758，L1192-L1219）。这给出一个关键边界：**资源池化可以共享物理内存，但不等于多设备共享同一地址翻译结构。**

### 2024：非独占分层与事务式迁移，暴露 TLB/多映射/脏页条件
- **Nomad** 反问 exclusive tiering 是否最优：当 fast tier 压力大时，page migration 本身可能拖垮性能。它提出 non-exclusive tiering + transactional page migration（TPM）+ page shadowing：复制但不先 unmap；copy后查 dirty，脏则 abort；干净则 remap 并留 shadow。降级若 shadow 干净可直接 remap，不复制（`nomad2024.txt` L48-L81，L126-L164，L386-L401，L487-L528）。
- 限制也很明确：多映射页会需要多 TLB shootdown，Nomad 对多映射页关闭 TPM、退回同步迁移；它还把未来“全局工作集估计/迁移节流”列为未来工作（`nomad2024.txt` L530-L555，L1096-L1108）。

### 2025：从“热度”转向“性能关键性”；GPU侧开始 OS 可编程与虚拟/物理解耦
- **Soar/Alto** 直接反驳 hotness：热页不一定 critical，MLP 会掩蔽慢层延迟；AOL = Latency/MLP，Soar 做离线对象排序，Alto 做运行时迁移调节。AOL 需要离线校准 K=f(AOL) 的 a,b，带宽竞争下 AOL 膨胀，需要调阈值（`soaralto2025.txt` L153-L194，L264-L363，L369-L499，L544-L608，L1052-L1117）。
- **gpu_ext** 转向 GPU 驱动/设备作为可编程 OS 子系统：host driver hooks + device-side eBPF + SIMT-aware verifier。它不直接解决多设备共享地址翻译，但把 GPU MMU、page fault、eviction、prefetch 变成策略点（`gpuext2025.txt` L29-L38，L69-L84，L263-L327，L340-L435）。
- **vAttention** 反对 PagedAttention 把 KV cache 虚拟布局改成非连续；用 CUDA VMM 保留虚拟连续、延迟分配物理页，并改 NVIDIA 开源驱动支持 64KB page-group。它报告 64KB 页对 attention kernel 无明显 TLB thrashing（`vattention2025.txt` L99-L115，L408-L437，L659-L741，L1180-L1223）。

### 2026：PACT 把“页级在线关键性”作为一等原则；ObservUVM 把 GPU HBM 观测作为一等机制
- **PACT** 认为 Soar/Alto 仍 coarse object 或 reactive hint；它提出 PAC = 页级 stall 贡献，使用 per-tier MLP（CHA/TOR queue occupancy）+ PEBS + 20ms 窗口，在线驱动 eager demotion + adaptive promotion（`pact2026.txt` L70-L72，L137-L176，L334-L463，L471-L541，L545-L670）。
- **ObservUVM（旧对比）** 则揭示 GPU UVM 的另一卡点：驱动看不到 HBM-resident 页访问，只能 LRM 淘汰；它把 PCIe access counters 反用为采样观测，分离 policy/mechanism，在用户态探索 eviction/prefetch（`observuvm2026.txt` L13-L27，L73-L120，L217-L262，L373-L410，L446-L529）。它本身没有直接声称 IOMMU/SVA，但清楚说明“观测到访问”是淘汰/预取决策的前提。

---

## B. 每篇证据卡

### B1. memtis2023

**题录核对**  
- 标题：Memtis: Efficient Memory Tiering with Dynamic Page Classification and Page Size Determination  
- 作者：Taehyung Lee, Sumit Kumar Monga, Changwoo Min, Young Ik Eom  
- 年份：2023  
- 会议：ACM SIGOPS 29th Symposium on Operating Systems Principles (SOSP ’23)  
- DOI：10.1145/3600006.3613167  
- 证据：`memtis2023.txt` L1-L18。

**最强前作 / 关键改变**  
最强前作包括 TPP、HeMem、Nimble、AutoNUMA、AutoTiering、Tiering-0.8；Memtis 的表格明确对比这些系统的 access tracking、placement metric、threshold、critical-path migration、page size（`memtis2023.txt` L132-L152）。关键改变：不用静态阈值，转向 access distribution-based hot set；不用“每页固定4KB/固定huge page”，转向 skewness-aware split/merge；全程后台迁移（`memtis2023.txt` L90-L126，L353-L397）。

**精确机制**  
- PEBS 采样 LLC load miss 和 retired store；动态调采样间隔，目标 CPU <3%；维护 page access histogram + emulated base page histogram（`memtis2023.txt` L411-L427，L429-L435）。
- 用 hotness factor EMA、指数分桶、cooling/减半、hot/warm/cold 阈值动态适配；hot set 尽量贴近 fast tier 容量（`memtis2023.txt` L374-L397，L440-L492）。
- huge page split：比较 eHR/rHR，估算 split benefit；按 skewness 选 top-N；后台 split，将 hot base page 迁到 fast tier，释放全零 subpage；merge 仅在全部 base pages hot 时（`memtis2023.txt` L533-L570，L576-L599）。

**复用粒度、身份/权限匹配前提**  
- 跟踪粒度：4KB subpage（huge page 内 512 个 subpage）；管理粒度：base page / huge page 两类。  
- 身份匹配前提：需要 Linux `compound_page`、PTE page frame padding、per-page metadata；依赖进程/页所属关系，不是跨设备身份。  
- 证据：`memtis2023.txt` L111-L126，L606-L632。

**资源共享 vs 映射共享/SVA**  
- 共享的是 memory tier 资源（DRAM + NVM/CXL）；映射仍是 OS page table/THP，没有跨设备共享页表。  
- 全文未覆盖 IOMMU/SVA；不能从 Memtis 推出多设备共享地址翻译方案。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：THP 默认，低 huge page utilization 或 hot subpage 高度倾斜时 split；split 涉及 data copy + TLB shootdown，所以保守（`memtis2023.txt` L522-L527）。  
- 数据放置：hotness distribution，不靠静态阈值；warm set 减少迁移（`memtis2023.txt` L461-L521，L830-L838）。  
- 观测：PEBS，不能精确识别 rarely/never accessed pages（`memtis2023.txt` L932-L939）。  
- 动态映射：hot/warm/cold 动态迁移，后台 500ms 唤醒（`memtis2023.txt` L493-L506）。

**评价配置**  
- 双路 Intel Xeon Gold 5218R，单 socket 评估避免 NUMA；DRAM + Optane DCPMM，以及 emulated CXL（remote NUMA 降频，177ns）（`memtis2023.txt` L645-L662）。  
- 8 个 memory-intensive 应用；fast:capacity = 1:2, 1:8, 1:16，另 2:1；与 AutoNUMA/AutoTiering/Tiering-0.8/TPP/Nimble/HeMem 比（`memtis2023.txt` L667-L676，L752-L774）。

**限制 / 未覆盖**  
- PEBS 对未被采样的冷页不敏感；可能需要 page table scanning 补充，但又引入开销（`memtis2023.txt` L932-L939）。  
- 未真实 CXL 硬件，CXL 为 emulation；单 socket 避免 NUMA。  
- 未覆盖多设备共享页表、IOMMU/SVA、远端 PTW、多租户隔离。

**对可能创新点最强反证**  
- 如果创新点是“动态 hotness + huge page split”，Memtis 已是强前作；必须证明在 PEBS 采样不足、CXL真实设备、尾延迟或跨设备场景下新增价值。  
- 如果创新点是“hotness 等同于性能”，Memtis 仍以 hotness 为主，Soar/Alto/PACT later 才直接反证。

**后续必追引文**  
TPP ASPLOS’23；HeMem SOSP’21；TMTS ASPLOS’23；Nimble ASPLOS’19；Thermostat ASPLOS’17；PRISM PACT’20；HawkEye ASPLOS’19（`memtis2023.txt` L1174-L1222，L906-L939）。

**证据定位**  
- `memtis2023.txt` L1-L18：题录、DOI。  
- `memtis2023.txt` L132-L152：与 prior work 对比表。  
- `memtis2023.txt` L353-L397：PEBS + histogram + split overview。  
- `memtis2023.txt` L533-L570：split benefit/skewness。  
- `memtis2023.txt` L645-L662：硬件与 benchmark。  
- `memtis2023.txt` L932-L939：限制。

---

### B2. soaralto2025

**题录核对**  
- 标题：Tiered Memory Management Beyond Hotness  
- 作者：Jinshu Liu, Hamid Hadian, Hanchen Xu, Huaicheng Li  
- 年份：2025  
- 会议：19th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’25)  
- DOI：null（全文未给 DOI）  
- 证据：`soaralto2025.txt` L1-L10，L83。

**最强前作 / 关键改变**  
最强前作：TPP、Nomad、NBT、Colloid；背景还列 X-Mem、HeMem、Memtis 等（`soaralto2025.txt` L100-L104，L1119-L1248）。关键改变：指出 hotness 不是 performance-criticality 的可靠代理；提出 AOL = Latency/MLP（结合 CPU stalls），Soar 做离线对象排序，Alto 做迁移调节；称可无缝叠加到 TPP/Nomad/NBT/Colloid（`soaralto2025.txt` L29-L51，L153-L166，L544-L608）。

**精确机制**  
- AOL 估计：`P = sLLC/c`，`AOL = Latency/MLP = A1/A3`，`K = f(AOL) = 1/(a+b/AOL)`，最终 `S = P×K`。使用四个 counters：A1, A3, c, sLLC（`soaralto2025.txt` L290-L363，Table 1 L241-L247）。  
- Soar：对象跟踪（malloc/free/mmap/munmap）、PEBS、AOL performance flow；合并 object flow、memory access flow、performance flow，给 object score；按 unit score 放 top-N 到 fast tier；short-lived objects 回退 first-touch（`soaralto2025.txt` L369-L431，L435-L499，L503-L526）。  
- Alto：AOL 低则限制/禁用 promotion，AOL 高则全速，中间用 stepwise F(l) 部分迁移；默认 AOLlow=40, AOLhigh=100（`soaralto2025.txt` L563-L608）。

**复用粒度、身份/权限匹配前提**  
- Soar 粒度：object type（按 call chain 分组），不是页；Alto 粒度：页 promotion rate。  
- 前提：Soar 需要一次 fast-tier profiling run；对象生命周期、vaddr、size、调用链匹配；Alto 需要 perf 访问 counters，依赖 Linux/CPU PMU。  
- 证据：`soaralto2025.txt` L410-L421，L435-L499，L563-L608。

**资源共享 vs 映射共享/SVA**  
- 共享的是 fast/slow tier 容量资源；映射仍是 OS/NUMA 页机制。  
- 全文未覆盖 IOMMU/SVA；不能把 object ranking 或 AOL 当作设备共享地址翻译方案。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：Soar 是 object-level，Alto 调节页 promotion；没有像 Memtis/PACT 那样定义 split policy。  
- 数据放置：Soar 静态对象放置，目标 near-optimal initial placement；Alto 动态迁移调节。  
- 观测：AOL 用 host PMU；Soar 额外 PEBS + object tracking；Alto 1s 采样。  
- 动态映射：Alto 过滤不必要迁移；Soar 静态后基本避免运行时迁移，但依赖 profiling。

**评价配置**  
- SKX/NUMA：CloudLab dual-socket Intel Skylake，10-core/socket，96GB DDR4/socket；emulated CXL by uncore freq / disabling cores，fast/slow latency 90/190ns，BW 49/17GB/s（`soaralto2025.txt` L617-L662）。  
- SPR/CXL：Sapphire Rapids，32-core/socket，192GB DDR5，ASIC-based 128GB CXL expander；latency 114/271ns，BW 218/26GB/s（`soaralto2025.txt` L661-L667）。  
- Workloads: graph, ML, caching, HPC；8 threads；RSS 8–35GB；slow-tier ratio 10–90%（`soaralto2025.txt` L681-L690）。

**限制 / 未覆盖**  
- Soar 需要离线 profiling；假设 object 内 uniform access distribution；未覆盖 heterogeneous access pattern object（`soaralto2025.txt` L528-L542）。  
- Alto 在带宽竞争下收益下降；要调 AOL thresholds；AOL 会因 queuing delay 膨胀（`soaralto2025.txt` L100-L150，L1060-L1117）。  
- 未覆盖 IOMMU/SVA、远端 PTW、多设备共享页表。

**对可能创新点最强反证**  
- 如果创新点是“hotness 不等于 performance”，Soar/Alto 已是强前作。  
- 如果创新点是“online page-granular criticality”，PACT 明确说 Soar 是 offline、object-level，且 PACT 自己 online、page-level（`pact2026.txt` L70-L72，L137-L166）。  
- 如果创新点是“只做迁移调节”，Alto 已能叠加到 TPP/Nomad/NBT/Colloid，且 kernel 侧约 30 LOC 改动（`soaralto2025.txt` L609-L620）。

**后续必追引文**  
X-Mem；TMO；TPP；Nomad；NBT；Colloid；HeMem；Memtis；M5；Chrono；Telescope；ARMS（`soaralto2025.txt` L1217-L1248，L1205-L1232，L1231-L1248，L1263-L1270）。

**证据定位**  
- `soaralto2025.txt` L29-L51：AOL + Soar/Alto abstract。  
- `soaralto2025.txt` L153-L194：hotness vs performance microbenchmark。  
- `soaralto2025.txt` L290-L363：AOL数学与counters。  
- `soaralto2025.txt` L369-L499：Soar profiling/scoring。  
- `soaralto2025.txt` L563-L608：Alto算法。  
- `soaralto2025.txt` L1052-L1117：带宽竞争下限制。

---

### B3. nomad2024

**题录核对**  
- 标题：Nomad: Non-Exclusive Memory Tiering via Transactional Page Migration  
- 作者：Lingfeng Xiang, Zhen Lin, Weishu Deng, Hui Lu, Jia Rao, Yifan Yuan, Ren Wang  
- 年份：2024  
- 会议：18th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’24)  
- DOI：null（全文未给 DOI）  
- 证据：`nomad2024.txt` L1-L13，L22-L25。

**最强前作 / 关键改变**  
最强前作：TPP、Memtis、Nimble、TMTS、AutoTiering；它批评 exclusive tiering 在 fast tier 压力下产生 thrashing，批评同步 page migration 在关键路径（`nomad2024.txt` L110-L152，L317-L329）。关键改变：non-exclusive tiering + TPM + shadow page；把 migration 做成异步，降级可用 remap 而非 copy。

**精确机制**  
- TPM：不清除映射就复制页；复制后检查 dirty；若脏则 abort 并稍后重试；若干净则 remap 到 fast tier，旧页变 shadow copy（`nomad2024.txt` L126-L164，L386-L401）。  
- 两队列：PCQ + migration pending queue；kpromote 后台线程做 transaction migration；原则上每次成功迁移只需一个 page fault（`nomad2024.txt` L448-L485）。  
- Shadow：XArray 索引 fast tier物理地址到 slow tier shadow物理地址；master page 设 read-only，用 shadow r/w bit 记录原读写权限；写触发 shadow page fault 并丢弃 shadow（`nomad2024.txt` L487-L528）。  
- Shadow 回收：kswapd 优先回收 shadow pages；分配失败时释放 shadow；目标释放10倍请求页或全部 shadow（`nomad2024.txt` L507-L528）。

**复用粒度、身份/权限匹配前提**  
- 粒度：64KB page 迁移，2MB region in LRU；shadow copy 仅对从 slow tier promoted 的页。  
- 前提：页表 PTE dirty bit、active/accessed bit、XArray、struct page shadow flag；依赖 Linux LRU/kswapd。  
- 证据：`nomad2024.txt` L229-L240，L448-L485，L487-L528。

**资源共享 vs 映射共享/SVA**  
- 共享的是 fast/slow tier 容量和 shadow copy 资源；映射仍是 OS PTE remap。  
- 全文未覆盖 IOMMU/SVA；TLB shootdown 是本地 CPU IPI，不是设备地址翻译共享。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：64KB page，2MB region list；没有多设备页表。  
- 数据放置：非独占，热页可同时有 fast master + slow shadow；不过度频繁迁移。  
- 观测：依赖 page fault stream、LRU、active/accessed bits；PEBS 被批评为粗且不响应。  
- 动态映射：TPM 异步迁移；干净 shadow 可 remap 降级；脏则丢弃 shadow。

**评价配置**  
- 四个平台：Platform A/B FPGA-based CXL（Agilex 7）+ Sapphire Rapids；Platform C Optane PM；Platform D AMD Genoa + Micron pre-market CXL（`nomad2024.txt` L530-L589）。  
- 与 TPP、Memtis-Default、Memtis-QuickCool 比；Linux v5.13-rc6 / v5.15.19；microbenchmark + Redis/PageRank/Liblinear（`nomad2024.txt` L617-L636，L908-L923）。

**限制 / 未覆盖**  
- 两轮 TLB shootdown；多映射页需多次 shootdown，Nomad 对多映射页禁用 TPM、退回同步迁移（`nomad2024.txt` L530-L555）。  
- 依赖未来 lightweight TLB shootdown 改善（ARM/未来AMD/Intel）。  
- 全局工作集估计、迁移节流列未来工作；write 操作因 shadow fault 有额外开销（`nomad2024.txt` L1096-L1108，L818-L823）。  
- 未覆盖 IOMMU/SVA、远端 PTW、多设备共享页表。

**对可能创新点最强反证**  
- 如果创新点是“迁移必须独占且同步”，Nomad 反证。  
- 如果创新点是“异步迁移不需要 shadow”，Nomad 说明 shadow 是快降级关键。  
- 如果创新点是“多设备页迁移没有 TLB 条件”，Nomad 直接给出多映射页禁用 TPM 的反证。

**后续必追引文**  
TPP；Memtis；TMTS；Nimble；Concurrent page migration for mobile systems with OS-managed hybrid memory；Demystifying CXL memory；Ganguly access counters；EarlyAdaptor（`nomad2024.txt` L110? references L1114-L1272）。

**证据定位**  
- `nomad2024.txt` L48-L81：non-exclusive / TPM abstract。  
- `nomad2024.txt` L126-L164：TPM 与 shadow 总览。  
- `nomad2024.txt` L386-L401：TPM commit/abort。  
- `nomad2024.txt` L487-L528：shadow management。  
- `nomad2024.txt` L530-L555：TLB 限制。  
- `nomad2024.txt` L568-L589：四平台。

---

### B4. pond2023

**题录核对**  
- 标题：Pond: CXL-Based Memory Pooling Systems for Cloud Platforms  
- 作者：Huaicheng Li, Daniel S. Berger, Lisa Hsu, Daniel Ernst, Pantea Zardoshti, Stanko Novakovic, Monish Shah, Samir Rajadnya, Scott Lee, Ishwar Agarwal, Mark D. Hill, Marcus Fontoura, Ricardo Bianchini  
- 年份：2023  
- 会议：ASPLOS ’23, March 25–29, 2023, Vancouver, BC, Canada, Volume 2  
- DOI：10.1145/3575693.3578835  
- 证据：`pond2023.txt` L1-L42，L63-L66。

**最强前作 / 关键改变**  
最强前作：memory disaggregation / pooling：ThymesisFlow、Clio、Infiniswap、Leap、AIFM；以及 TPP/Nimble/OS far memory（`pond2023.txt` L1192-L1219，L1251-L1274）。关键改变：不是只做 pooling，而是 full-stack cloud pool：CXL池 + zNUMA + ML预测 latency sensitivity/untouched memory + QoS monitor + EMC slice ownership。

**精确机制**  
- 分析 Azure 100 clusters 的 stranding/untouched memory；小池 8–16 sockets 足够；CXL 加 70–90ns，rack-scale 加 >180ns；pool 越大收益递减（`pond2023.txt` L99-L134）。  
- CXL.mem 使用 load/store，不涉及 page fault 或 DMA；CXL memory 用 hypervisor page tables + MMU 虚拟化（`pond2023.txt` L174-L189）。  
- EMC：外部 memory controller ASIC，多 CXL ports，HDM decoder；1GB slice，owner 至多一个 host，permission table 检查 requestor/owner；非法访问 fatal memory error（`pond2023.txt` L463-L486）。  
- zNUMA：加 memory block 无 CPU entry；guest OS 优先本地，zNUMA 用于 untouched memory（`pond2023.txt` L724-L734）。  
- QoS：PMU/TMA + access bit scans；ML latency-insensitivity + untouched memory；overprediction 用 QoS monitor 触发 live migration 到 all-local（`pond2023.txt` L816-L868，L907-L922）。

**复用粒度、身份/权限匹配前提**  
- 池化粒度：1GB memory slice；至多一个 host 拥有；host id 权限表。  
- VM 侧：zNUMA node；hypervisor page tables；静态 preallocation/pinning，compat with virtualization accelerators。  
- 身份匹配：EMC permission 要求 requestor = slice owner；不是 IOMMU PASID/device ID 匹配。  
- 证据：`pond2023.txt` L181-L189，L463-L486，L724-L734。

**资源共享 vs 映射共享/SVA**  
- 明确是**资源共享**：pool memory slices 在 host 间移动；hosts 有 separate coherency domains，ownership model，非 shared coherent address space（`pond2023.txt` L463-L470）。  
- **不是映射共享/SVA**：不用 ATS/PRI；ATS/PRI 设备尚不广泛，vIOMMU 需 guest OS 改动且开销（`pond2023.txt` L1192-L1219）。  
- 这直接支持“应用使用 VM ≠ IOMMU复用方案”：Pond 的 VM 使用 hypervisor 页表和 zNUMA，CXL memory 像 CPUless NUMA；不是设备共享一个地址翻译表。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：CXL slice 1GB；zNUMA memory block；无 page fault；offlining 1GB 10–100 ms/GB，onlining 微秒/GB（`pond2023.txt` L596-L627）。  
- 数据放置：ML预测哪些 VM latency-insensitive、untouched memory 多少；pool DRAM 或 local DRAM。  
- 观测：core PMU/TMA，1s 采样，1ms；hypervisor page table access bit 每30分钟扫10s；guest-committed memory counter（`pond2023.txt` L935-L941，L816-L868）。  
- 动态映射：live migration / reconfiguration，50ms/GB 把 pool memory 拷回 local；不是设备页表动态共享。

**评价配置**  
- 生产服务器 emulation layer；部分 lab；158 workloads；simulation with 100 production clusters/75 days；emulated CXL 182%/222% latency；Intel/AMD 生产服务器（`pond2023.txt` L925-L1035，L1000-L1028）。  
- 并非真实 CXL 硬件；CXL 尚需一年广泛部署；用 emulation layer 证明 VM workflow/zNUMA/guest performance（`pond2023.txt` L110-L121）。

**限制 / 未覆盖**  
- 依赖预测模型；误预测需要 QoS monitor 缓解，但只能缓解约1% misprediction（`pond2023.txt` L1120-L1214）。  
- 静态 preallocation；ATS/PRI 不广泛；vIOMMU 需 guest 改动；不追求 shared coherence。  
- 未覆盖 IOMMU/SVA、远端 PTW、设备共享页表、多设备原子/PASID 语义。  
- 未评估真实 CXL 硬件；EMC/PM 是设计，不是大规模部署结果。

**对可能创新点最强反证**  
- 如果创新点是“池化内存需要 page fault/迁移才能被 VM 使用”，Pond 反证：CXL.mem load/store 无需 page fault/DMA。  
- 如果创新点是“VM 使用共享内存就是 IOMMU 复用”，Pond 明确不是：ownership + hypervisor页表 + zNUMA，ATS/PRI/vIOMMU 另路未采用。  
- 如果创新点是“pool 越大越好”，Pond 反证：8–16 sockets 足够，大池 latency 和 cost 更差。

**后续必追引文**  
TPP；Nimble；Software-Defined Far Memory；Infiniswap；Leap；AIFM；ThymesisFlow；Clio；LegoOS；PCIe ATS；vIOMMU；coIOMMU；Demystifying CXL Memory（`pond2023.txt` L1286-L1401）。

**证据定位**  
- `pond2023.txt` L99-L134：四项 insight。  
- `pond2023.txt` L174-L189：CXL.mem、hypervisor page tables、no page fault/DMA。  
- `pond2023.txt` L463-L486：EMC slice/ownership/permission。  
- `pond2023.txt` L724-L734：zNUMA。  
- `pond2023.txt` L1192-L1219：ATS/PRI/vIOMMU 替代与限制。  
- `pond2023.txt` L1251-L1274：结论。

---

### B5. gpuext2025

**题录核对**  
- 标题：gpu_ext: Extensible OS Policies for GPUs via eBPF  
- 作者：Yusheng Zheng, Tong Yu, Yiwei Yang, Minghui Jiang, Xiangyu Gao, Jianchang Su, Yanpeng Hu, Wenan Mao, Wei Zhang, Dan Williams, Andi Quinn  
- 年份：2025  
- 来源：arXiv:2512.12615v2 [cs.OS], 20 Dec 2025  
- DOI：null  
- 状态：preprint  
- 证据：`gpuext2025.txt` L1-L7。

**最强前作 / 关键改变**  
最强前作：driver-level kernel policies（TimeGraph、Gdev、GPREEMPT、LithOS、GCAPS、kernel interception）、user-space runtimes（Paella、Pie、XSched、KTransformers）、GPU profiling（NVBit、Neutrino）、CPU eBPF（sched_ext、cache_ext）、prior eGPU。关键改变：host driver hooks + device-side eBPF + SIMT-aware verifier，把 GPU driver/device 当成可编程 OS 子系统（`gpuext2025.txt` L186-L228，L229-L258，L263-L327）。

**精确机制**  
- 三个 eBPF prog types：`BPF_PROG_TYPE_GPU_MEM`、`BPF_PROG_TYPE_GPU_SCHED`、`BPF_PROG_TYPE_GPU_DEV`；memory hooks: activate/access/evict_prepare/prefetch；scheduling: host queue task_init/task_destroy + device block enter/exit/probe/retprobe（`gpuext2025.txt` L263-L327）。  
- SIMT-aware verifier：warp-uniform branch/loop/map keys；禁止 GPU-wide barriers、global sync、non-uniform atomics；warp leader 执行 policy 一次并 broadcast（`gpuext2025.txt` L340-L408，L442-L480）。  
- 跨层 maps：host DRAM / GPU global / shared memory 分层 shard，snapshot-based eventual consistency；GPU kernel completion 边界合并（`gpuext2025.txt` L411-L435，L483-L503）。

**复用粒度、身份/权限匹配前提**  
- 内存区域：2MB physical UVM chunks / VA blocks， finer 4KB pages；prefetch page 粒度，eviction 目前 2MB block。  
- 身份/权限：eBPF TCB 信任管理员加载；policy 不写 native device code；host 复用 Linux verifier + BTF/kfunc。  
- 证据：`gpuext2025.txt` L274-L289，L340-L349，L463-L477。

**资源共享 vs 映射共享/SVA**  
- 共享的是 GPU memory/UVM 资源与 policy state；跨层 maps 是状态共享，不是共享地址翻译。  
- 提到 Linux HMM/migrate_vma，AMD ROCm XNACK；不涉及 IOMMU SVA。  
- 证据：`gpuext2025.txt` L940-L945。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：eviction 2MB block，prefetch page；region 可 2MB/VA/4KB。  
- 数据放置：prefetch/eviction policies；adaptive memory prefetching、LFU eviction。  
- 观测：device-side eBPF observability，warp-level；工具 kernelretsnoop/threadhist/launchlate；对比 NVBit 3–14% vs 85–87% overhead（`gpuext2025.txt` L889-L916）。  
- 动态映射：GPU kernel instrumentation，无需重启应用；UVM page fault/prefetch 策略可改。

**评价配置**  
- Server A: Intel Core Ultra 9 285K, 128GB DDR5, RTX 5090；Server B: dual Intel Gold 6138, 256GB, P40；PyTorch 2.9.0, vLLM 0.11.0, llama.cpp 7101, Faiss 1.13.0（`gpuext2025.txt` L471-L481）。  
- Workloads: LLM inference, GNN training, vector search, multi-tenant。  
- 结果：吞吐最高 4.8×，尾延迟最高降 2×；instrument-only <0.2% overhead（`gpuext2025.txt` L36-L38，L924-L925）。

**限制 / 未覆盖**  
- 当前实现 NVIDIA；driver bypass Linux standard interfaces，portability future work（`gpuext2025.txt` L940-L945）。  
- No comparison policy-level vs eGPU/Neutrino read-only observability（`gpuext2025.txt` L918-L921）。  
- 未覆盖 IOMMU/SVA、跨设备共享页表、远端 PTW。

**对可能创新点最强反证**  
- 如果创新点是“host-side eBPF 足够控制 GPU”，gpu_ext 反证：host eBPF 看不到 device 内部 warp/scheduling，需 device-side hooks。  
- 如果创新点是“设备端策略不需 verifier”，gpu_ext 反证：SIMT mismatch、死锁/无界循环会 halt GPU，需 SIMT-aware verifier。

**后续必追引文**  
eGPU prior；sched_ext；cache_ext；XRP；GPREEMPT；LithOS；Paella；Pie；XSched；NVIDIA open GPU kernel modules；HMM/migrate_vma；NVBit；Neutrino（`gpuext2025.txt` L972-L1197）。

**证据定位**  
- `gpuext2025.txt` L29-L38：abstract 结果。  
- `gpuext2025.txt` L69-L84：三个挑战。  
- `gpuext2025.txt` L263-L327：policy interfaces。  
- `gpuext2025.txt` L340-L435：verifier + maps。  
- `gpuext2025.txt` L442-L503：实现/maps。  
- `gpuext2025.txt` L924-L925：overhead。

---

### B6. vattention2025

**题录核对**  
- 标题：vAttention: Dynamic Memory Management for Serving LLMs without PagedAttention  
- 作者：Ramya Prabhu, Ajay Nayak, Jayashree Mohan, Ramchandran Ramjee, Ashish Panwar  
- 年份：2025  
- 会议：ASPLOS ’25, March 30–April 3, 2025, Rotterdam, Netherlands, Volume 1  
- DOI：10.1145/3669940.3707256  
- 证据：`vattention2025.txt` L1-L9，L43-L60。

**最强前作 / 关键改变**  
最强前作：vLLM PagedAttention；FlashAttention-2/3、FlashInfer、TensorRT-LLM、GMLake。关键改变：指出 PagedAttention 为动态物理分配牺牲虚拟连续性，导致 kernel 改写、框架冗余、CPU/GPU 关键路径开销；用 CUDA VMM 解耦虚拟/物理分配，保留虚拟连续，并改驱动支持 64KB page-group（`vattention2025.txt` L11-L37，L95-L115，L1255-L1272）。

**精确机制**  
- 预保留虚拟空间：每层 K/V 各一个 virtual tensor，大小 B × L × H × D × P；虚拟空间充足，物理页运行时映射（`vattention2025.txt` L408-L437，L452-L513）。  
- CUDA VMM APIs：`cuMemAddressReserve`、`cuMemCreate`、`cuMemMap`、`cuMemSetAccess`；vAttention 自建 `vMemReserve/vMemCreate/vMemMap/vMemRelease/vMemFree` 支持 64/128/256KB page-group；CUDA 原生只支持 2MB 页（`vattention2025.txt` L472-L482）。  
- `step` API 每迭代确保 KV cache sub-tensor 被物理页 backing；失败则 preempt；`reqId` 定位 batch 中 sub-tensor；deferred reclamation + eager allocation；backgroud thread 重叠分配（`vattention2025.txt` L522-L544，L572-L617，L659-L683）。

**复用粒度、身份/权限匹配前提**  
- 粒度：page-group 64KB/128KB/256KB，或 2MB；block size 是 token 数 per page-group（Table 8）。  
- 前提：每个 request `reqId`，连续 batching 通过 `cache_batch_idx` 映射 Q 与 KV cache；依赖 FlashAttention/FlashInfer 对 stride/block-table 的支持。  
- 证据：`vattention2025.txt` L560-L570，L633-L656，L1164-L1175。

**资源共享 vs 映射共享/SVA**  
- 共享的是 GPU physical memory / KV cache 物理页；映射是 CUDA VMM 的 GPU 虚拟地址到物理页。  
- 不是多设备共享页表/SVA；未覆盖 IOMMU、PASID、ATS。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：64KB 页减少 fragmentation，且作者未发现 TLB thrashing；2MB 页在动态 trace 下 batch size 更低（`vattention2025.txt` L1180-L1223）。  
- 数据放置：物理页按 request 子区域动态映射；deferred reclamation 复用 physical pages；eager allocation 预映。  
- 观测：不需要 Block-Table；KV cache 虚拟连续，flash kernels 直接用。  
- 动态映射：每 decode iteration 最多一个新 page-group per virtual tensor；prefill 需按 prompt token 数映射。

**评价配置**  
- Yi-6B 1×A100，Llama-3-8B/Yi-34B 2×A100；H100 用于 FA3 portability；vLLM v0.2.7；FlashAttention-2 v2.5.9，FlashInfer v0.4.0（`vattention2025.txt` L754-L758，L786-L801）。  
- Workloads：arXiv-Summarization offline/online，OpenChat dynamic trace；512 requests长上下文等。

**限制 / 未覆盖**  
- 需要修改 NVIDIA 开源驱动支持小页；或用 tensor slicing，但 FlashInfer 早期不支持 stride；未覆盖所有 GPU/厂商。  
- 未系统量化 TLB shootdown/远端 PTW；只报告 64KB 对 attention kernel 无明显负面影响。  
- 未覆盖多设备共享页表/IOMMU SVA。

**对可能创新点最强反证**  
- 如果创新点是“动态 KV cache 必须 PagedAttention”，vAttention 反证：CUDA VMM 可保留虚拟连续，支持 unmodified kernels。  
- 如果创新点是“64KB 页一定 TLB thrash”，vAttention 反证：LLM attention 下未观察到。  
- 如果创新点是“虚拟/物理解耦只适用于训练”，GMLake 已有训练方向，vAttention 面向 inference KV cache（`vattention2025.txt` L1255-L1272）。

**后续必追引文**  
PagedAttention/vLLM；GMLake；FlashAttention-2/3；FlashInfer；Orca；Sarathi；Splitwise；CUDA VMM docs；Ingens/HawkEye（`vattention2025.txt` L1299-L1477）。

**证据定位**  
- `vattention2025.txt` L11-L37：abstract。  
- `vattention2025.txt` L95-L115：PagedAttention pitfalls。  
- `vattention2025.txt` L408-L437：observation/design。  
- `vattention2025.txt` L472-L482：CUDA VMM APIs and page sizes。  
- `vattention2025.txt` L659-L683：latency hiding/deferred reclamation。  
- `vattention2025.txt` L1180-L1223：page size/TLB。

---

### B7. pact2026

**题录核对**  
- 标题：PACT: A Criticality-First Design for Tiered Memory  
- 作者：Hamid Hadian, Jinshu Liu, Hanchen Xu, Hansen Idden, Huaicheng Li  
- 年份：2026  
- 会议：Proceedings of the 31st ACM International Conference on Architectural Support for Programming Languages and Operating Systems, Volume 2 (ASPLOS ’26), March 22–26, 2026, Pittsburgh, PA, USA  
- DOI：10.1145/3779212.3790198  
- 证据：`pact2026.txt` L1-L42，L57-L60。

**最强前作 / 关键改变**  
最强前作：Soar/Alto、Memtis、Colloid、Nomad、TPP、NBT；PACT 明确说 SoarAlto 最接近，但 Soar 离线、object-level，AOL 系统级 MLP，不能运行时区分 memory tier；PACT 做 online、page-granular、criticality-first（`pact2026.txt` L137-L176，L689-L692）。关键改变：PAC = per-page access criticality，利用 CHA/TOR queue occupancy 分解 per-tier MLP，直接归因 CPU stall 到页；eager demotion + adaptive promotion。

**精确机制**  
- PAC 建模：`LLC-stalls = k × LLC-misses / MLP`；per-tier MLP `MLP = T1/T2`，T1=TOR_OCCUPANCY，T2=TOR_OCCUPANCY_COUNTER0；用 PEBS 采样 page vaddr/access count；每20ms窗口把 stalls 按 access frequency 分摊给页；`PAC[p] = α·PAC[p] + Sp`（`pact2026.txt` L313-L463，Algorithm 1 L400-L446）。  
- Promotion/demotion：维护 priority bins；eager demotion 提前 demote 冷页给 high-PAC 让空间；adaptive promotion 用 Reservoir sampling + Freedman-Diaconis bin width + scaling optimization，无需全局排序/静态阈值（`pact2026.txt` L545-L603，L608-L670）。  
- 实现：Linux 5.15 + TPP modules；禁用 NUMA hint fault scanning；PEBS records 裁剪，5MB buffer；扩展 perf 直接算 PAC；shared memory buffer；25 bytes/per 4KB page = 0.6% memory overhead；两个线程（PEBS processing + migrations）（`pact2026.txt` L672-L683）。

**复用粒度、身份/权限匹配前提**  
- 跟踪粒度：4KB page PAC；huge page 下若选中的 4KB 页属于 2MB THP，则迁移整个 2MB huge page 以 amortize migration。  
- 前提：Intel CHA/TOR counters；PEBS；page 访问采样率；proportional attribution 假设 20ms 内 MLP 稳定；AMD 需用 IBS + Little’s Law 近似，无 TOR。  
- 证据：`pact2026.txt` L440-L469，L487-L541，L781-L789。

**资源共享 vs 映射共享/SVA**  
- 共享的是 DRAM/CXL tier 资源；映射/迁移通过 `move_pages()`，仍是 OS page table。  
- 未覆盖 IOMMU/SVA、设备共享页表；TOR 是 CPU-to-memory queue，不是 IOMMU queue。

**页大小 / 数据放置 / 观测 / 动态映射**  
- 页大小：4KB PAC 精度，但 THP migration 为 2MB；PACT 对 4KB/THP 都评估。  
- 数据放置：criticality-first，eager demotion + adaptive promotion；badly performing frequency policy 被比较，PACT 同迁移数下提高 18%。  
- 观测：20ms，PEBS rate 默认1/400；采样 period 可到百 ms；默认 α=1.0 不 cooling，因为 PAC 高度 skewed；cooling 多无益。  
- 动态映射：在线更新 PAC，动态 bin width；带宽竞争下仍有效。

**评价配置**  
- CloudLab dual-socket Intel Skylake，10-core Xeon 2.2GHz，96GB DDR4/socket，local 90ns/52GB/s，cross-socket 140ns/32GB/s，emulated CXL 190ns/2.1×（`pact2026.txt` L707-L717）。  
- 13 workloads，graph/GPT-2/Redis/Silo/SPEC；8 threads；RSS 6.6–40GB；带宽 6–45GB/s；fast:slow 8:1 到1:8；4KB vs THP；与 Soar/Alto/Memtis/Colloid/Nomad/TPP/NBT/NoTier 比（`pact2026.txt` L719-L745，L689-L692）。  
- 结果：最高 61% over 第二强；平均 gap 4.1%，最大 11.8%；迁移最多少 50×（`pact2026.txt` L25-L27，L72-L78，L801-L801）。

**限制 / 未覆盖**  
- proportional attribution 在多租户/异构访问混合同一 tier 时可能稀释 latency-sensitive pages；作者称是 observability gap，未来工作（`pact2026.txt` L509-L541）。  
- 默认只采样 slow-tier load，demotion 依赖 Linux (M)LRU；不是全策略都由 PAC 驱动。  
- PACT 在部分 workload 输给 Soar/Colloid/NBT/Memtis；它承认 Soar 的离线 profiling 在 603.bwaves、bc-urand、sssp-kron 更好（`pact2026.txt` L915-L926，L927-L953）。  
- 未覆盖 IOMMU/SVA、远端 PTW、多设备共享页表、TLB shootdown 成本细节。

**对可能创新点最强反证**  
- 如果创新点是“hotness 足够”，PACT + Soar/Alto 反证。  
- 如果创新点是“online page-granular criticality 没人做”，PACT 是直接前作。  
- 如果创新点是“AOL 可以不经修改直接用于 page 级在线放置”，PACT 反证：AOL 是系统级/离线校准，PACT 用 per-tier TOR MLP 和 online PAC。  
- 但若声称“PACT 全面优于 Soar”，PACT 自己承认 Soar 在部分 workload 更好，不能过度。

**后续必追引文**  
Soar/Alto OSDI’25；Memtis SOSP’23；Colloid SOSP’24；Nomad OSDI’24；TPP ASPLOS’23；NBT；TMO ASPLOS’22；M5 ASPLOS’25；Chrono EuroSys’25；Telescope ATC’24；ARMS；NeoMem MICRO’57；CXL CHMU；TPEBS（`pact2026.txt` L1136-L1273）。

**证据定位**  
- `pact2026.txt` L10-L42：abstract/venue/DOI。  
- `pact2026.txt` L137-L176：PACT vs SoTA。  
- `pact2026.txt` L334-L463：counters/per-tier stalls/MLP/PAC。  
- `pact2026.txt` L471-L541：sampling。  
- `pact2026.txt` L545-L670：migration policies/adaptive binning。  
- `pact2026.txt` L927-L953：PACT vs Soar/Alto。  
- `pact2026.txt` L1083-L1118：带宽竞争/结论。

---

## C. 被证据支持的剩余瓶颈与不能断言的点

### C1. 被证据支持的瓶颈

1. **HBM/慢层访问观测不足。**  
   - Memtis：PEBS 无法有效区分 rarely/never accessed pages（`memtis2023.txt` L932-L939）。  
   - Soar/Alto：AOL 系统级 MLP，离线校准 a,b；带宽竞争会使 AOL 膨胀，收益下降（`soaralto2025.txt` L340-L363，L1052-L1117）。  
   - PACT：CPU stalls 是 per-core coarse，缺 per-page/per-tier；用 CHA/TOR 补（`pact2026.txt` L79-L84，L334-L388）。  
   - ObservUVM：UVM 驱动对 HBM-resident 访问盲；access counters 原生只跟踪 PCIe 到 DRAM，反用为采样观测（`observuvm2026.txt` L13-L27，L73-L120，L322-L332）。

2. **页迁移的数据复制条件复杂。**
   - Nomad：clean -> remap shadow / copy commit；dirty -> abort；多映射 -> 关闭 TPM；shadow 可降级到 remap（`nomad2024.txt` L386-L401，L448-L528，L530-L555）。  
   - Memtis：split 涉及数据 copy + TLB shootdown，所以必须估算 benefit（`memtis2023.txt` L390-L397，L522-L527）。  
   - PACT：4KB criticality 但 THP 迁移整个 2MB page；copy/migration 成本用 coarse migration 摊薄（`pact2026.txt` L781-L789）。  
   - Pond：CXL load/store 无 page fault/DMA；但 offlining 切片慢，release 异步；live migration 50ms/GB copy（`pond2023.txt` L596-L627，L735-L758）。  
   - vAttention：虚拟连续、物理页动态 map；不复制数据来实现动态分配；deferred reclamation 复用页（`vattention2025.txt` L408-L437，L659-L683）。

3. **页大小 / TLB / 地址翻译成本不可回避。**
   - Memtis huge page split 与 TLB shootdown 成本。  
   - vAttention 64KB 页 vs 2MB 页，报告无明显 TLB thrash；2MB 页限制 batch size（`vattention2025.txt` L1180-L1223）。  
   - PACT 4KB fine-grained criticality + 2MB migration，页粒度与迁移粒度不一致（`pact2026.txt` L781-L789）。  
   - Nomad 两轮 TLB shootdown；多映射页禁用 TPM（`nomad2024.txt` L530-L555）。  
   - ObservUVM page/region 2MB/64KB，没有报告 TLB shootdown 次数，但采样页会往返迁移。

4. **队列/容量/权限是共享资源管理的真实约束。**
   - Pond EMC 1GB slice 权限表，requestor=owner，不允许访问则 fatal memory error；host 不 interleave across EMCs；pool fragmentation 要避免（`pond2023.txt` L463-L486，L596-L627）。  
   - PACT TOR queue occupancy 是 per-tier MLP 的观测点（`pact2026.txt` L334-L388，L440-L463）。  
   - gpu_ext cross-layer maps 需要分层 shard 和 relaxed consistency；CPU map access via PCIe 比 GPU-side 慢 6000×（`gpuext2025.txt` L411-L435，L885-L916）。  
   - ObservUVM 只有约256 access counters，无法跟踪 TB 级 DRAM；改用于少量 HBM-region 采样观测（`observuvm2026.txt` L73-L120，L322-L332）。

5. **尾延迟和带宽竞争下性能退化。**
   - Soar/Alto under bandwidth contention：收益下降，甚至 Alto+Colloid 有 -3% 情况；AOL thresholds 需调（`soaralto2025.txt` L997-L1050，L1083-L1117）。  
   - Pond：PDM 1–5%，TP 98%；CXL latency 高时更多 workload >25% slowdown；需 QoS monitor（`pond2023.txt` L432-L477，L1080-L1219）。  
   - PACT：带宽饱和下仍比 Colloid/Memtis 少迁移，但 slowdown 会变化；敏感性分析显示 PEBS rate/spacing 影响（`pact2026.txt` L1083-L1118，L989-L1032）。  
   - vAttention：在线场景下 median latency 降，但极端长上下文仍存在 queuing；评测在接近 capacity 时进行（`vattention2025.txt` L906-L917，L918-L1027）。  
   - gpuext：尾延迟降 2×，但具体依赖 workload；多租户互相改善（`gpuext2025.txt` L36-L38，L854-L864）。

### C2. 不能断言的点

- **不能断言本组论文支持 IOMMU/SVA 共享页表。** 它们分别用 OS page table、NUMA hint fault、GPU UVM 页表、CUDA VMM、CXL.mem hypervisor页表。除了 Pond 提到 ATS/PRI/vIOMMU 作为替代但未采用，其余没有直接做 IOMMU/SVA。  
- **不能断言“远端 PTW”瓶颈。** 全文没有直接测量 remote page-table walk 的 IOMMU/CPU 设备共享场景。PACT 测的是 CPU CHA/TOR 到 DRAM/CXL；Nomad 测 TLB shootdown；vAttention 测 GPU VMM。都不能等同远端 PTW。  
- **不能断言“多设备共享地址翻译没人做”。** 本组未覆盖不是不存在；“未找到”只能说明这些全文内无直接证据，不能推全领域空白。  
- **不能直接跨论文排榜。** 各论文 baseline、硬件、kernel 版本、tier 比例、workload 不同。例如 PACT 的 “61% over 第二强” 是 PACT 自己的 13 workloads/7 ratios；Memtis 的 “33.6%” 是 Memtis 自己的 8 apps/3 configs；不能把数字并列当统一排名。  
- **不能把应用使用 VM 当作 IOMMU 复用方案。** Pond 的 VM 用 hypervisor页表/zNUMA/EMC ownership；没有把设备 IOMMU 与 CPU MMU 共享同一页表。  
- **不能把 GPU UVM/access counter 当作 IOMMU ATS/PRI。** ObservUVM 反用 PCIe access counters；gpu_ext 用 HMM/migrate_vma；vAttention 用 CUDA VMM；都不是 PCIe ATS/PASID 设备共享页表。  
- **不能把 FPGA 原型当方向正确性证明。** Nomad 只有 Platform A/B 是 FPGA-based CXL，且 latency 更高；Pond 明确 ASIC-based 更接近 industry expectation，FPGA prototype 报告更高 latency。不能从 Nomad 的 FPGA 平台推“FPGA 方向正确”。

### C3. 页迁移复制条件小结

| 条件 | 是否复制 | 证据 |
|---|---|---|
| Nomad TPM clean page | 先复制，脏检查通过则 remap；旧页 shadow | `nomad2024.txt` L386-L401 |
| Nomad TPM dirty during copy | abort，丢弃 copy，重试 | `nomad2024.txt` L386-L401 |
| Nomad demotion + clean shadow | 只 remap PTE，不复制 | `nomad2024.txt` L487-L507 |
| Nomad multi-mapped page | 禁用 TPM，退回同步迁移 | `nomad2024.txt` L530-L555 |
| Memtis huge page split | 复制 hot subpage + TLB shootdown | `memtis2023.txt` L390-L397，L522-L527 |
| Pond CXL pool access | load/store，无 page fault/DMA；slice释放慢，live correction 要 copy | `pond2023.txt` L174-L189，L596-L627，L735-L758 |
| vAttention KV cache | 动态 map 物理页；deferred reclamation复用；kernel append 单 tensor copy vs paged block copy | `vattention2025.txt` L408-L437，L659-L683，L810-L826 |
| PACT | `move_pages()` 迁移；4KB选中但THP整页迁移 | `pact2026.txt` L781-L789 |
| ObservUVM | 采样页 migrate DRAM<->HBM；eviction/prefetch region/page | `observuvm2026.txt` L373-L410，L621-L668 |

---

## D. Soar/Alto vs PACT 深入比较

### D1. 贡献差异

| 维度 | Soar | Alto | PACT |
|---|---|---|---|
| 核心问题 | 初始对象放置 | 迁移强度调节 | 页级在线 criticality |
| 粒度 | object type by call chain | page promotion rate | 4KB PAC，THP迁移 |
| 信息 | offline profiling + PEBS + AOL | runtime AOL (perf 1s) | online PEBS + TOR per-tier MLP + PAC |
| 在线性 | 离线一次 | 在线调节 | 在线，20ms |
| 策略 | top-N object 放 fast tier | 低AOL限迁移，高AOL全速 | eager demotion + adaptive promotion |
| 重要前提 | object内 access 均匀、可 profiling | 阈值校准、PMU | CHA/TOR counters、MLP phase stability |
| 关键证据 | `soaralto2025.txt` L369-L499 | `soaralto2025.txt` L563-L608 | `pact2026.txt` L334-L463，L545-L670 |

### D2. 线上关键性度量代价

- **Soar/Alto AOL**
  - 4 counters: A1, A3, c, sLLC（`soaralto2025.txt` L290-L363）。
  - AOL 预测模型 `K = 1/(a+b/AOL)`，a,b 硬件相关但 workload-independent，可离线用微基准校准；用户不需重复大 profiling（`soaralto2025.txt` L341-L363）。
  - Soar profiling：PEBS sample rate 3000 够用；profile 一次 fast-tier run；object tracking through LD_PRELOAD/backtrace；开销低但 object granularity（`soaralto2025.txt` L385-L421）。
  - Alto：perf counters 每1s采样；kernel ~30 LOC；stepwise调节 promotion（`soaralto2025.txt` L563-L608，L609-L620）。
  - 代价风险：带宽竞争下 AOL inflation，threshold 需调；这是论文自己列的 future work（`soaralto2025.txt` L100-L150，L1060-L1117）。

- **PACT PAC**
  - 20ms窗口，PEBS 默认 1/400；4 counters（Table 1：PEBS slow-tier LLC miss，TOR_OCCUPANCY，TOR_OCCUPANCY_COUNTER0 等），perf subsystem 扩展直接算 PAC；shared memory buffer；2线程（`pact2026.txt` L334-L334，L400-L446，L672-L683）。
  - 内存开销 25 bytes/per 4KB tracked page = 0.6%；采样 rate/period 敏感性：PEBS 800→4000 时 slowdown 23%→30%；period 10ms→1000ms 时 promotion 0.8M→1.7M，slowdown 20%→27%（`pact2026.txt` L989-L1032）。
  - 在线优势：可适应 phase shift；不依赖离线 object profiling；但 proportional attribution 在多租户/混合同 tier 异构访问下可能稀释（`pact2026.txt` L509-L541）。
  - 与 Soar/Alto 对比：PACT 说 Alto 用 system-wide MLP，PAC 用 per-tier TOR MLP；Soar 离线 object，PACT online page（`pact2026.txt` L927-L953）。PACT 在 8/12 workload 胜 Alto，平均 7.6%；对 Soar 在 6 workload 平均只多 3.3% slowdown，但在 603.bwaves/bc-urand/sssp-kron 输给 Soar 最多 48%（`pact2026.txt` L927-L953）。

### D3. 页表和 TLB 相关成本

- Soar/Alto 叠加的 NBT/Nomad/Colloid 多依赖 **NUMA hinting faults + VMA scanning + PAGE_NONE**；这会产生 minor page fault 和页表操作（`soaralto2025.txt` L587-L598）。
- PACT 禁用 NUMA hint fault scanning，改用 TOR + PEBS + `move_pages`；但 4KB page criticality 选中 THP 时迁移整个 2MB huge page，以摊薄迁移成本（`pact2026.txt` L672-L683，L781-L789）。全文没有报告 PACT 的 TLB shootdown 次数或远端 PTW 成本；因此不能声称 PACT 已解决 TLB 级开销，只能说它用 coarse migration 来降低迁移次数。
- 相邻证据：
  - Memtis：huge page split 涉及 TLB shootdown，必须保守（`memtis2023.txt` L522-L527）。
  - Nomad：TPM 需要两轮 TLB shootdown，多映射页禁用 TPM（`nomad2024.txt` L530-L555）。
  - vAttention：64KB 页没有造成明显 attention TLB thrash；2MB 页反而限制 batch size（`vattention2025.txt` L1180-L1223）。
  - ObservUVM：用 2MB region/64KB page 采样，未直接量化 TLB shootdown。

### D4. 谁承担维护成本

- Soar：应用/operator 承担 profiling run 和对象排序部署；allocator 承担放置。
- Alto：kernel + user-level tool；每1s调控，约30 LOC kernel 改动，但需要 perf 和阈值维护。
- PACT：kernel/perf 扩展 + 两个后台线程 + 25B/page 元数据；维护 PAC hash、bin width、eager demotion。  
- 对比：PACT 降低“人工阈值调参”成本，但增加在线采样与内核/驱动改动成本；Soar 降低运行时开销，但增加离线 profiling 与静态放置不适应性；Alto 只调节迁移，不解决“迁哪些页的 criticality 来源”。

---

## E. 本组对常见创新说法的反证 / 边界

- “热页放快层一定加速”：被 Soar/Alto 微基准反证（Hot-on-DRAM 52.4% of All-on-DRAM；Cold-on-DRAM 比 ideal hotness 高34%），`soaralto2025.txt` L191-L194。PACT 同样用 PAC 反证（`pact2026.txt` L301-L309）。
- “频率就是性能关键性”：被 PACT 反证（同频页 stall 差 65×），`pact2026.txt` L286-L309。
- “动态 KV cache 必须 PagedAttention”：被 vAttention 反证（CUDA VMM 可保留虚拟连续，支持 unmodified kernels），`vattention2025.txt` L99-L115，L1255-L1272。
- “VM 使用池化内存就是共享地址翻译”：被 Pond 反证（ownership + hypervisor page table + zNUMA；ATS/PRI、vIOMMU 未采用），`pond2023.txt` L1192-L1219。
- “异步迁移不需要 shadow”：被 Nomad 反证（干净 shadow 可 remap 降级，脏则丢弃/重试），`nomad2024.txt` L487-L528。
- “GPU host eBPF 足以控制设备策略”：被 gpu_ext 反证（device-side scheduling/memory events 对 host 不可见；需 device-side eBPF + SIMT verifier），`gpuext2025.txt` L69-L84，L212-L228。
- **不能反证为“领域无人做”的：** IOMMU SVA、ATS/PRI、PASID、远端 PTW、多设备共享页表。本组全文没有直接评估；Pond 只在替代方案中提及 ATS/PRI/vIOMMU。未找到不等于不存在。

---

## F. 对照矩阵（优化段 / 信息何处产生 / 何时必须完成 / 维护成本）

| 论文 | 优化段 | 信息何处产生 | 何时必须完成 | 维护成本 |
|---|---|---|---|---|
| memtis2023 | 页放置 + page size | PEBS、histogram、subpage metadata | 后台周期；split后台 | kernel threads；PEBS overhead <3% |
| soaralto2025 Soar | 初始对象放置 | 离线 profiling、PEBS、AOL、调用链 | 分配前/时 | profiling run + allocator |
| soaralto2025 Alto | 迁移强度 | perf counters、AOL、1s | 运行中1s | kernel ~30 LOC + user tool |
| nomad2024 | 迁移机制 + shadow | page fault、PTE dirty、XArray | 访问/淘汰时 | kernel driver + TLB shootdown |
| pond2023 | CXL池切片 + VM放置 | ML、PMU、access bits、trace | VM调度/QoS异常 | control plane + hypervisor + EMC |
| gpuext2025 | GPU内存/调度策略 | eBPF hooks、跨层maps | hook时 | driver extension + eBPF admin |
| vattention2025 | KV cache虚拟/物理 | CUDA VMM、step API | 每迭代前，重叠 | serving framework + driver patch |
| pact2026 | 页级criticality + 迁移 | PEBS + TOR + PAC | 20ms在线 | kernel/perf + 2线程 + 0.6%内存 |
| observuvm2026（旧对比） | GPU HBM eviction/prefetch | 反用PCIe access counters | 访问/淘汰时 | modified UVM driver + userspace/eBPF |

---

## G. 最值得追踪的引文（按主题）

- **热点/分层放置**：TPP ASPLOS’23；HeMem SOSP’21；Memtis SOSP’23；Nomad OSDI’24；Nimble ASPLOS’19；TMTS ASPLOS’23；Thermostat ASPLOS’17。
- **关键性/MLP/在线度量**：Soar/Alto OSDI’25；PACT ASPLOS’26；TMO ASPLOS’22；ARMS；Chrono EuroSys’25；Telescope ATC’24。
- **CXL/池化/VM/IOMMU-adjacent**：Pond ASPLOS’23；Demystifying CXL Memory MICRO’56；CXL 3.0/CHMU；PCIe ATS；vIOMMU；coIOMMU。
- **GPU UVM/可编程**：ObservUVM；gpu_ext；eGPU；Forest ISCA’25；EarlyAdaptor ISPASS’23；HMM/migrate_vma；NVBit；Neutrino。
- **LLM KV cache/虚拟内存**：vAttention ASPLOS’25；PagedAttention/vLLM SOSP’23；FlashAttention-2/3；FlashInfer；GMLake ASPLOS’24。

---

```json
{
  "papers": [
    {
      "id": "memtis2023",
      "title": "Memtis: Efficient Memory Tiering with Dynamic Page Classification and Page Size Determination",
      "authors": [
        "Taehyung Lee",
        "Sumit Kumar Monga",
        "Changwoo Min",
        "Young Ik Eom"
      ],
      "year": 2023,
      "venue": "ACM SIGOPS 29th Symposium on Operating Systems Principles (SOSP ’23)",
      "doi": "10.1145/3600006.3613167",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "measurement",
        "hardware_prototype",
        "analysis"
      ],
      "mechanism": "用PEBS采样页/子页访问，指数分桶维护访问直方图，动态定hot/warm/cold阈值并后台迁移；按子页访问倾斜动态split huge page。",
      "assumptions": [
        "Linux OS内存管理，v5.15.19",
        "PEBS可采样LLC miss和store指令",
        "huge page内4KB subpage可跟踪",
        "fast tier/capacity tier可迁移页",
        "单socket避免NUMA效应"
      ],
      "limitations": [
        "PEBS对rarely/never accessed页区分不足",
        "huge page split涉及copy和TLB shootdown，开销高",
        "CXL为emulated remote NUMA，不是真实CXL硬件",
        "未覆盖IOMMU/SVA、多设备共享页表、远端PTW",
        "单socket评估，未覆盖NUMA与多租户"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt",
          "location": "L1-L18",
          "claim": "题录：标题、作者、SOSP ’23、DOI。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt",
          "location": "L132-L152",
          "claim": "与AutoNUMA/AutoTiering/Tiering-0.8/TPP/HotBox/Nimble/MULTI-CLOCK/TMTS/HeMem对比表。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt",
          "location": "L353-L397",
          "claim": "PEBS采样、两类histogram、动态hot set分类、skewness-aware page size设计。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt",
          "location": "L533-L570",
          "claim": "split benefit估计：eHR/rHR，skewness，top-N split候选。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt",
          "location": "L645-L662",
          "claim": "硬件：Intel Xeon Gold 5218R、DRAM+Optane DCPMM、emulated CXL；单socket。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/memtis2023.txt",
          "location": "L932-L939",
          "claim": "限制：PEBS对rarely/never accessed页不准；page table scanning补充有开销。"
        }
      ],
      "relations": [
        {
          "type": "actual_citation",
          "target": "pond2023",
          "source_section": "Memtis参考文献 [42] L1099-L1103"
        }
      ]
    },
    {
      "id": "soaralto2025",
      "title": "Tiered Memory Management Beyond Hotness",
      "authors": [
        "Jinshu Liu",
        "Hamid Hadian",
        "Hanchen Xu",
        "Huaicheng Li"
      ],
      "year": 2025,
      "venue": "19th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’25)",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "measurement",
        "hardware_prototype",
        "analysis"
      ],
      "mechanism": "AOL=Latency/MLP结合CPU stalls；Soar离线对象级性能贡献排序后静态放置；Alto按AOL动态调节页迁移强度。",
      "assumptions": [
        "host PMU可读LLC stalls、offcore、cycles",
        "AOL的K=f(AOL)可由微基准离线校准",
        "对象按调用链分组可代表访问模式",
        "Soar接受一次fast-tier profiling run",
        "Alto可叠加到TPP/NBT/Nomad/Colloid"
      ],
      "limitations": [
        "Soar离线object-level，页面/对象内criticality可变时粒度不匹配",
        "Alto阈值为离线经验值，带宽竞争下AOL膨胀需调阈值",
        "部分workload负收益，5/182 cases",
        "未覆盖IOMMU/SVA、远端PTW、多设备共享页表"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt",
          "location": "L29-L51",
          "claim": "AOL、Soar、Alto摘要与最高12.4×、少数退化≤3%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt",
          "location": "L153-L194",
          "claim": "hotness vs performance微基准；热页放fast tier可降性能。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt",
          "location": "L290-L363",
          "claim": "AOL数学、四个counters、K=f(AOL)、预测模型。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt",
          "location": "L369-L499",
          "claim": "Soar object profiling/ranking/scoring。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt",
          "location": "L563-L608",
          "claim": "Alto AOL阈值与stepwise迁移调节。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/soaralto2025.txt",
          "location": "L1052-L1117",
          "claim": "带宽竞争下收益下降，需调AOL阈值。"
        }
      ],
      "relations": [
        {
          "type": "actual_citation",
          "target": "memtis2023",
          "source_section": "Soar/Alto参考文献 [19] L1217-L1221"
        },
        {
          "type": "actual_citation",
          "target": "nomad2024",
          "source_section": "Soar/Alto参考文献 [22] L1243-L1248"
        },
        {
          "type": "actual_citation",
          "target": "pond2023",
          "source_section": "Soar/Alto参考文献 [7] L1128-L1133"
        }
      ]
    },
    {
      "id": "nomad2024",
      "title": "Nomad: Non-Exclusive Memory Tiering via Transactional Page Migration",
      "authors": [
        "Lingfeng Xiang",
        "Zhen Lin",
        "Weishu Deng",
        "Hui Lu",
        "Jia Rao",
        "Yifan Yuan",
        "Ren Wang"
      ],
      "year": 2024,
      "venue": "18th USENIX Symposium on Operating Systems Design and Implementation (OSDI ’24)",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "measurement",
        "hardware_prototype",
        "analysis"
      ],
      "mechanism": "非独占分层：页可从slow promoted到fast且保留slow shadow；TPM先复制不unmap，脏则abort，干净则remap；干净shadow降级直接remap。",
      "assumptions": [
        "Linux kernel v5.13-rc6/v5.15.19",
        "页有dirty/accessed bit和PTE",
        "fast/slow tier可映射到GPU/CPU页表",
        "shadow copy只对promoted页",
        "多映射页可退回同步迁移"
      ],
      "limitations": [
        "TPM需两轮TLB shootdown；多映射页需多次shootdown",
        "多映射页禁用TPM，退回同步迁移",
        "shadow页有空间开销，需回收避免OOM",
        "write场景因shadow fault有额外开销",
        "未覆盖IOMMU/SVA、远端PTW、多设备共享页表"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt",
          "location": "L48-L81",
          "claim": "非独占分层与TPM abstract，up to 6× over TPP。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt",
          "location": "L126-L164",
          "claim": "TPM设计：复制不unmap，脏则abort，干净则remap并留shadow。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt",
          "location": "L386-L401",
          "claim": "TPM workflow与三步骤迁移对比。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt",
          "location": "L448-L485",
          "claim": "两队列PCQ+migration pending queue，kpromote后台迁移。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt",
          "location": "L487-L528",
          "claim": "shadow page管理、XArray、read-only、shadow r/w bit、回收。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/nomad2024.txt",
          "location": "L530-L555",
          "claim": "TLB shootdown限制；多映射页禁用TPM。"
        }
      ],
      "relations": [
        {
          "type": "actual_citation",
          "target": "memtis2023",
          "source_section": "Nomad参考文献 [37] L1207-L1211"
        },
        {
          "type": "actual_citation",
          "target": "pond2023",
          "source_section": "Nomad参考文献 [38] L1213-L1222"
        }
      ]
    },
    {
      "id": "pond2023",
      "title": "Pond: CXL-Based Memory Pooling Systems for Cloud Platforms",
      "authors": [
        "Huaicheng Li",
        "Daniel S. Berger",
        "Lisa Hsu",
        "Daniel Ernst",
        "Pantea Zardoshti",
        "Stanko Novakovic",
        "Monish Shah",
        "Samir Rajadnya",
        "Scott Lee",
        "Ishwar Agarwal",
        "Mark D. Hill",
        "Marcus Fontoura",
        "Ricardo Bianchini"
      ],
      "year": 2023,
      "venue": "ASPLOS ’23, March 25–29, 2023, Vancouver, BC, Canada, Volume 2",
      "doi": "10.1145/3575693.3578835",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": [
        "simulation",
        "measurement",
        "analysis"
      ],
      "mechanism": "CXL池 + EMC 1GB slice ownership + zNUMA + ML预测latency-insensitive/untouched memory + QoS monitor；资源池化不等于共享地址翻译。",
      "assumptions": [
        "CXL.mem load/store，无page fault/DMA",
        "VM静态preallocation兼容虚拟化加速",
        "EMC permission table按host id/slice owner",
        "zNUMA映射为无核NUMA节点",
        "ML预测可接受误预测并由QoS缓解"
      ],
      "limitations": [
        "依赖ML预测和QoS monitor；误预测只能缓解部分",
        "ATS/PRI设备尚不广泛；vIOMMU需guest改动且有开销",
        "hosts有separate coherency domains，池内存显式移动，不是共享一致映射",
        "slice offlining慢，需异步release",
        "未真实CXL硬件，使用emulation layer；未覆盖IOMMU/SVA、远端PTW"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pond2023.txt",
          "location": "L99-L134",
          "claim": "四项insight：pool size 8-16 sockets、latency sensitivity、untouched memory、ML预测。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pond2023.txt",
          "location": "L174-L189",
          "claim": "CXL.mem无page fault/DMA；CXL memory用hypervisor page tables/MMU虚拟化。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pond2023.txt",
          "location": "L463-L486",
          "claim": "EMC 1GB slice ownership、permission table、requestor/owner匹配、非法访问fatal。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pond2023.txt",
          "location": "L724-L734",
          "claim": "zNUMA：memory block无CPU entry，guest优先本地。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pond2023.txt",
          "location": "L1192-L1219",
          "claim": "ATS/PRI不广泛；vIOMMU需guest改动；说明Pond不是IOMMU/SVA方案。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pond2023.txt",
          "location": "L1251-L1274",
          "claim": "结论：pool size 16 sockets、latency 222%下节省DRAM 7%。"
        }
      ],
      "relations": [
        {
          "type": "mechanism_comparison",
          "target": "observuvm2026",
          "source_section": "报告 C.2/C.3：VM池化与GPU UVM观测均非IOMMU SVA"
        }
      ]
    },
    {
      "id": "gpuext2025",
      "title": "gpu_ext: Extensible OS Policies for GPUs via eBPF",
      "authors": [
        "Yusheng Zheng",
        "Tong Yu",
        "Yiwei Yang",
        "Minghui Jiang",
        "Xiangyu Gao",
        "Jianchang Su",
        "Yanpeng Hu",
        "Wenan Mao",
        "Wei Zhang",
        "Dan Williams",
        "Andi Quinn"
      ],
      "year": 2025,
      "venue": "arXiv preprint arXiv:2512.12615v2 [cs.OS]",
      "doi": null,
      "publication_status": "preprint",
      "relevance": "nearby",
      "evaluation_type": [
        "measurement",
        "hardware_prototype",
        "analysis"
      ],
      "mechanism": "在GPU driver和device侧引入eBPF hooks与SIMT-aware verifier，策略可运行时更新；跨层maps共享策略状态，用于GPU内存放置、预取、调度、观测。",
      "assumptions": [
        "NVIDIA open GPU kernel modules",
        "administrator加载策略，eBPF verifier信任模型",
        "host driver hooks + device-side PTX trampolines",
        "warp-uniform control flow，禁止GPU-wide barrier/global sync/non-uniform atomic",
        "跨层map采用eventual consistency"
      ],
      "limitations": [
        "当前实现仅NVIDIA；Linux标准接口被vendor driver绕过",
        "device-side策略需SIMT verifier和JIT，复杂度高",
        "未覆盖IOMMU/SVA、多设备共享页表、远端PTW",
        "设备侧policy性能与可移植性仍受vendor runtime限制"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt",
          "location": "L29-L38",
          "claim": "abstract：GPU driver/device可编程OS子系统，吞吐最高4.8×、尾延迟最高2×。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt",
          "location": "L69-L84",
          "claim": "host-only eBPF不足，需设备侧hooks与kernel内策略逻辑。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt",
          "location": "L263-L327",
          "claim": "三类BPF program type、memory与scheduling接口。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt",
          "location": "L340-L435",
          "claim": "SIMT-aware verification与hierarchical cross-layer maps。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt",
          "location": "L924-L925",
          "claim": "hooks enabled but no policy attached，overhead <0.2%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/gpuext2025.txt",
          "location": "L940-L945",
          "claim": "portability：NVIDIA当前实现，HMM/migrate_vma、ROCm/Level Zero未来。"
        }
      ],
      "relations": [
        {
          "type": "mechanism_comparison",
          "target": "observuvm2026",
          "source_section": "报告 C.1/C.2：GPU UVM观测与可编程策略对比"
        }
      ]
    },
    {
      "id": "vattention2025",
      "title": "vAttention: Dynamic Memory Management for Serving LLMs without PagedAttention",
      "authors": [
        "Ramya Prabhu",
        "Ajay Nayak",
        "Jayashree Mohan",
        "Ramchandran Ramjee",
        "Ashish Panwar"
      ],
      "year": 2025,
      "venue": "ASPLOS ’25, March 30–April 3, 2025, Rotterdam, Netherlands, Volume 1",
      "doi": "10.1145/3669940.3707256",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": [
        "measurement",
        "hardware_prototype",
        "analysis"
      ],
      "mechanism": "用CUDA VMM解耦虚拟/物理分配，KV cache保留虚拟连续、物理页按需映射；改NVIDIA开源驱动支持64KB page-group，并用重叠分配/延迟回收隐藏CUDA VMM延迟。",
      "assumptions": [
        "CUDA VMM APIs可用",
        "NVIDIA GPU/驱动可修改开源UVM部分",
        "FlashAttention/FlashInfer等kernel支持非分页连续/stride访问",
        "虚拟地址空间充足",
        "KV cache按request子区域管理"
      ],
      "limitations": [
        "需要修改NVIDIA驱动以支持小页；portability受vendor限制",
        "未系统量化TLB shootdown/远端PTW，只报告64KB无显著TLB thrash",
        "未覆盖IOMMU/SVA、多设备共享页表",
        "在线serving仍需preemption等策略，swap KV cache到CPU留future work"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/vattention2025.txt",
          "location": "L11-L37",
          "claim": "abstract：PagedAttention问题与vAttention收益。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/vattention2025.txt",
          "location": "L95-L115",
          "claim": "PagedAttention三pitfalls：kernel改写、框架冗余、CPU/GPU关键路径开销。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/vattention2025.txt",
          "location": "L408-L437",
          "claim": "Observation1/2：KV cache需求可预测、分配带宽低；虚拟连续+物理按需。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/vattention2025.txt",
          "location": "L472-L482",
          "claim": "CUDA VMM APIs vs vAttention APIs；CUDA原生只支持2MB页。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/vattention2025.txt",
          "location": "L659-L683",
          "claim": "重叠分配、deferred reclamation、eager allocation、page-group回收。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/vattention2025.txt",
          "location": "L1180-L1223",
          "claim": "64KB页无明显TLB thrash；2MB页限制batch size。"
        }
      ],
      "relations": [
        {
          "type": "mechanism_comparison",
          "target": "pond2023",
          "source_section": "报告 C.3：动态虚拟/物理解耦 vs CXL池化"
        }
      ]
    },
    {
      "id": "pact2026",
      "title": "PACT: A Criticality-First Design for Tiered Memory",
      "authors": [
        "Hamid Hadian",
        "Jinshu Liu",
        "Hanchen Xu",
        "Hansen Idden",
        "Huaicheng Li"
      ],
      "year": 2026,
      "venue": "Proceedings of the 31st ACM International Conference on Architectural Support for Programming Languages and Operating Systems, Volume 2 (ASPLOS ’26)",
      "doi": "10.1145/3779212.3790198",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "measurement",
        "hardware_prototype",
        "analysis"
      ],
      "mechanism": "PAC=页级访问对CPU stall的贡献；用PEBS+CHA/TOR queue occupancy分解per-tier MLP，20ms在线更新；eager demotion+adaptive promotion按PAC迁移页。",
      "assumptions": [
        "Intel CHA/TOR counters可用；AMD需IBS+Little's Law近似",
        "20ms窗口内MLP稳定，可按访问频率比例分摊stall",
        "PEBS默认1/400足够",
        "PAC分布高度skew，需动态binning",
        "Linux 5.15 + TPP模块，禁用NUMA hint fault scanning"
      ],
      "limitations": [
        "proportional attribution在同tier异构/多租户下可能稀释latency-sensitive页",
        "默认只采slow-tier load，demotion依赖Linux (M)LRU",
        "4KB PAC选中THP时迁移整个2MB页；未量化TLB shootdown",
        "部分workload输给Soar/Colloid/NBT/Memtis；Soar离线profiling在部分场景更好",
        "未覆盖IOMMU/SVA、远端PTW、多设备共享页表"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pact2026.txt",
          "location": "L10-L42",
          "claim": "abstract：PACT/PAC、最高61% over 第二强、最高50×少迁移；ASPLOS ’26/DOI。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pact2026.txt",
          "location": "L137-L176",
          "claim": "PACT vs Soar/Alto：AOL coarse object/offline，PAC online page-granular。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pact2026.txt",
          "location": "L334-L463",
          "claim": "Table 1 counters、per-tier stalls、TOR-MLP、PAC sampling。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pact2026.txt",
          "location": "L471-L541",
          "claim": "PAC sampling周期、冷却、采样限制、proportional attribution限制。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pact2026.txt",
          "location": "L545-L670",
          "claim": "eager demotion、adaptive promotion、Reservoir/Freedman-Diaconis adaptive binning。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/pact2026.txt",
          "location": "L927-L953",
          "claim": "PACT vs Soar/Alto：PACT胜8/12，但Soar在部分workload更好。"
        }
      ],
      "relations": [
        {
          "type": "actual_citation",
          "target": "soaralto2025",
          "source_section": "PACT参考文献 [34] L1207-L1210"
        },
        {
          "type": "actual_citation",
          "target": "memtis2023",
          "source_section": "PACT参考文献 [29] L1179-L1182"
        },
        {
          "type": "actual_citation",
          "target": "nomad2024",
          "source_section": "PACT参考文献 [53] L1228-L1231"
        },
        {
          "type": "actual_citation",
          "target": "pond2023",
          "source_section": "PACT参考文献 [30] L1184-L1188"
        }
      ]
    }
  ]
}
```