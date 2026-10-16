# G4_accelerators 共享地址翻译证据阅读报告

## 0. 范围、方法与元数据核验说明

- 本报告只依据所给 8 篇全文，不联网、不执行、不写文件。所有行号均为所给 `content_with_line_numbers` 的 1-based 行号。
- 题头能核到的题目/作者/年份/会议优先从题头核；题头缺 venue/DOI 时，只在同一批全文的其他论文参考文献中找到明确引用时使用，并标明“来自他文引用证据”，不凭年份或常识猜正式发表。
- **本批全文中没有 STAR、SP-IMPact、cuPTW、Avatar 的全文或题头。** 因此无法核对 STAR 正式版与 arXiv 关系，也无法评价 SP-IMPact 是否“系统干扰近邻”、cuPTW/Avatar 对“只能改善 TLB”的突破及代价。只能在相关论文的测量/机制中间接看到“页表遍历路径、宿主干扰、软件 PTW、shared LLC”等近邻证据，不能等同于这些工作。
- 本报告严格区分：  
  - **资源共享**：共享 TLB/PT cache 等硬件结构；  
  - **映射共享 / SVA**：共享同一虚拟地址空间或复用同一 VA→PA 翻译；  
  - **数据零拷贝**：避免显式 copy；  
  - **隔离/权限**：每设备/每进程权限检查、device identification、TLBI/back-invalidation、A/D 位、数据一致性。  
  证据表明这四者不等价。

---

## A. 按时间解释本组问题与机制演化

### 2015 Border Control：先把“翻译”和“权限检查”解耦
Border Control 面向不可信第三方加速器：加速器可以有 TLB 和物理缓存，但每个跨越可信/不可信边界的物理地址请求都要检查页权限。它用每加速器 Protection Table + Border Control Cache，存 PPN 粒度 R/W 位；ATS 翻译完成时更新权限表；权限降级/TLB shootdown 时要求加速器写回脏数据并刷新缓存/表。  
关键演化点：它不试图共享翻译，而是允许“预翻译物理地址继续使用”，但通过权限检查保证沙箱。证据：`border2015.txt` L24-L35、L204-L241、L242-L266、L267-L338、L379-L418、L449-L475、L493-L507。

### 2016 SVM 实测：共享虚拟内存的真实延迟/并发/失效成本
在 AMD Kaveri APU 上实测 CPU-GPU shared virtual memory：GPU TLB miss 走 ATS 到 IOMMU，GPU TLB miss 延迟约 582ns，CPU 约 23ns，约 25×；IOMMU 约支持 16 个并发页表遍历，超过后排队使延迟急升；GPU page fault 3-80× CPU，吞吐在 32 并发后不扩展；TLB shootdown 约 4.2-4.4µs，且串行化可能成为未来瓶颈。  
关键演化点：把问题从“TLB 容量”扩展到页错误、shootdown、并发页表遍历、预取适用性。证据：`svm2016.txt` L10-L34、L193-L249、L251-L275、L376-L381、L502-L549、L638-L687、L764-L803、L828-L902。

### 2017 accelerator-centric translation：两级 TLB + 宿主核 MMU 页表遍历
面向定制加速器规则、批量、数据 tiling 的访存模式，提出私有小 TLB、共享 512-entry L2 TLB、把页表遍历卸载到宿主核 MMU。仿真中 IOMMU-only 仅 12.3% ideal，私有 TLB 23.3%，两级 TLB 51.8%，两级+宿主页表遍历 93.6% ideal，平均只差 6.4%。  
关键演化点：不重新定义 VM，而是在现有 IOMMU 框架上做 TLB 层次和页表遍历路径优化；共享 TLB 的复用前提是同一进程/地址空间及共同虚拟页。证据：`accelerator_translation2017.txt` L104-L115、L135-L142、L499-L517、L604-L626、L711-L731、L267-L328。

### 2018 DVM：干脆让大部分 VA==PA，绕过翻译
DVM 对堆分配做 identity mapping（VA==PA），用 region-level DAV 验证身份映射和权限，引入 Permission Entry 压缩页表、AVC 缓存所有页表级别，读访问可用 preload 与 DAV 重叠；失败回退常规翻译。报告在 graph accelerator 上 VM overhead 1.6%，比优化 VM 快 2.1×、动态能量少 3.9×。  
关键演化点：从“更快翻译”转向“多数访问不需要翻译”；代价是碎片化、CoW 首写、ASLR 熵、Meltdown/Spectre 风险。证据：`dvm2018.txt` L18-L35、L80-L93、L201-L285、L306-L315、L317-L357、L363-L408、L466-L524。

### 2018 embedded SoC DMA：混合 IOMMU + 编译器 TLB 预取 + MMU-aware DMA
面向嵌入式 PMCA：混合 IOMMU 由软件线程管理共享 TLB；编译器生成 Prefetching Helper Threads 提前填 TLB；多个 Miss Handling Helper Threads 并行处理 miss；DMA engine 增加 retirement buffer 记录未命中 burst 并重发，避免大数据缓冲。Zynq FPGA 实测，不规则访存最多 4×、规则访存最多 60% 提升，缓冲从 16KiB 量级降到 64B 量级。  
关键演化点：在嵌入式/NoC/SPM 场景，TLB miss 处理被软件线程化、可扩展化；前提是 DMA burst 不跨页、软件 PTW、编译器能提取 LDS 预取。证据：`socdma2018.txt` L25-L40、L110-L115、L192-L200、L201-L212、L237-L258、L259-L352、L353-L418、L419-L543、L545-L569。

### 2020 SPARTA：把翻译拆到内存侧分区 TLB
SPARTA 将翻译拆成加速器侧 tiny TLB 和内存侧共享 per-partition TLB/MMU。虚拟地址唯一决定内存分区；数据可映射到分区内任意位置；数据与 PTE 同分区，页表遍历局部化；翻译与数据读取重叠。支持 demand paging、CoW、碎片化，避免 identity mapping/set-associative VM 的极强映射限制。报告平均降低翻译开销 31.5×，性能 +57%，SPARTA-32 4KB 达 ideal 93.7%。  
关键演化点：从“TLB 层次”转向“物理地址空间分区 + 数据/PTE 共置”；用少量映射限制换近零翻译开销。证据：`sparta2020.txt` L20-L31、L186-L198、L220-L225、L383-L399、L443-L459、L505-L518、L526-L600、L590-L595、L609-L616、L618-L662、L954-L961。

### 2025 RISC-V IOMMU SVA 评测：LLC 把 PTW 成本压下去，但 DMA 要绕过 LLC
在开源 RISC-V Cheshire + Snitch 集群上集成 RISC-V IOMMU v1.0，评估 SVA。无 LLC 时 IOVA 翻译可占 gemm runtime 4.2%-17.6%，heat3d 高延迟下可达 81.3% overhead；给 host + IOMMU PTW 加 shared LLC 后，所有 kernel overhead <2%，PTW 时间降低 15×；但 DMA 必须绕过 LLC，主机需要 self-invalidate 保证一致性。  
关键演化点：SVA 的瓶颈常常在 PTW/LLC/一致性路径，而不只是 TLB 命中率；shared LLC 是有效缓解，但 DMA bypass 与自无效是配套代价。证据：`riscvsva2025.txt` L19-L53、L93-L113、L140-L166、L167-L195、L239-L268、L269-L281、L284-L302、L341-L373。

### 2026 ShareMMU：不可信加速器之间安全地共享 TLB
ShareMMU 在 IOMMU 内加 Permission Tracker：每个 Secure Vector 以 PPN 为标签，记录每加速器权限和映射到该 PPN 的 VPN；Permission Tracker inclusive 私有 TLB；每次预翻译访问都验证；PT eviction 时按 VPN/Access Mask 向私有 TLB 发 back-invalidation；AA-LRU 防止恶意加速器独占。权限更新用现有 TLB shootdown，降权时只清对应加速器 bit；back-invalidation 需要等待加速器写回脏行、flush 私有 TLB 并 ack。仿真相对 unsecured baseline 仅 1.66% overhead，ShareMMU-SP 2.82%。  
关键演化点：共享 TLB 的性能收益被安全化；安全共享需要权限匹配、device/accelerator 标识、back-invalidation 等待/排空和写回一致性，而不只是“共享翻译”。证据：`sharemmu2026.txt` L15-L35、L79-L125、L351-L394、L395-L415、L419-L436、L443-L467、L487-L535、L536-L645、L670-L738、L777-L825、L940-L979、L1001-L1003。

---

## B. 每篇证据卡

### B1. `accelerator_translation2017.txt`

**元数据**
- 文件：`iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt`
- 题头：L1-L2 “Supporting Address Translation for Accelerator-Centric Architectures”
- 作者：L3-L5 “Jason Cong, Zhenman Fang, Yuchen Hao, Glenn Reinman” / UCLA
- 题头无 venue/year/DOI。他文引用证据：`dvm2018.txt` L723-L727 给出 “2017 … IEEE International Symposium on High Performance Computer Architecture (HPCA) … https://doi.org/10.1109/HPCA.2017.19”；`riscvsva2025.txt` L339-L341 也给出 HPCA 2017, pp. 37-48。  
  → 可判 **published, HPCA 2017, DOI 10.1109/HPCA.2017.19**；作者以题头为准。

**精确机制**
- 基线 IOMMU-only 只有 12.3% ideal；私有 TLB 23.3%；两级 TLB 51.8%；两级+宿主页表遍历 93.6% ideal，差距 6.4%。L104-L115、L725-L731。
- 私有 TLB：16-32 entries，低延迟，non-blocking hit-under-miss，LRU。L135-L142、L399-L416、L417-L431。
- 共享 L2 TLB：512-entry，集中式，mostly-inclusive；跨加速器复用共同虚拟页翻译，减少 tiling 带来的重复页表遍历。L499-L517、L511-L517。
- 宿主页表遍历：把加速器 TLB miss 卸载到宿主核 MMU，利用 MMU cache/data cache；宿主 MMU 需区分加速器请求，需要单独端口/请求队列和 demux。L604-L626。
- 失效：accelerator private TLB 支持 TLB shootdown；shared TLB 在 mostly-inclusive 下也要检查失效。L526-L529。

**复用粒度及身份/权限匹配前提**
- 复用粒度：虚拟页→物理页翻译；共享 TLB entry 含 accelerator ID bits 和 host process/context ID。L606-L607。
- 前提：加速器工作在同一 application virtual address space / 同一进程；翻译请求带虚拟地址和 process ID 到对应宿主核。L607-L610。
- 权限匹配：本文主要依赖 IOMMU 作为正确性和安全基线，未设计不可信设备间权限匹配；共享 TLB 复用主要按同进程 + 共同虚拟页。L104-L108、L607-L610。

**资源共享 vs 映射共享/SVA**
- 资源共享：私有 TLB、共享 L2 TLB、宿主 MMU 资源。
- 映射共享/SVA：共享 TLB 复用同一 VA→PA；统一虚拟地址空间是目标。L124-L128。
- 不等价于数据零拷贝/取消隔离：统一虚拟地址减少 copy，但安全仍由 IOMMU 基线提供；本文未声称取消权限检查。L104-L108、L124-L128。

**评价配置**
- PARADE cycle-accurate full-system simulator，扩展 gem5 + HLS，CACTI 估面积。L267-L274。
- 8-issue OoO x86-64 @2GHz，32KB L1，2MB L2，per-core MMU；4KB page；IOMMU 32-entry IOTLB；2GB DDR3 4 channels；25 类加速器，每类 4 instances，最多 20 active。L317-L328、L329-L362。
- 理想翻译 = infinite TLB + zero page walk。L284-L286。

**限制/未覆盖**
- 纯仿真，无真实 FPGA/硅后。L267-L274。
- 正确性问题假定 page fault / TLB shootdown 对结果影响可忽略；只讨论实现方式，不量化失效等待/排空。L407-L416。
- 未讨论 A/D 位、每设备权限匹配、不可信加速器安全隔离。
- 大页、TLB-aware tiling、banked shared TLB、更多 sharers 留 future。L523-L525、L581、L867、L896-L899、L900-L908。
- 共享 TLB 的复用依赖同进程和共同虚拟页；未覆盖跨进程/跨权限域共享。

**对可能创新点的最强反证**
- 在本文仿真配置下，两级 TLB + 宿主页表遍历已经只剩 6.4% overhead，且私有 32-entry TLB 减少 30.4% page walk，512-entry shared TLB 消除 75.8% total page walk。任何“只改善 TLB”的新方案必须说明为何还能显著超过这个已接近 ideal 的仿真基线。L135-L142、L725-L731、L944-L949。
- 但反证受限于仿真、4KB page、同进程假设、规则访存；不能直接推广到不可信多租户、A/D 位、迁移尾延迟。

**后续必追引文**
- IOMMU 教程/AMD IOMMU：[22] Kegel et al. L1036-L1037。
- GPU MMU：[25] Pichai et al. L1043-L1045；[26] Power et al. L967-L968。
- shared last-level TLB：[40] Bhattacharjee et al. L1010-L1011。
- MMU cache：[27] Barr et al. L969-L970；[28] Bhargava et al. L971-L972。
- rIOMMU：[55] Malka et al. L1042-L1043。
- Border Control：[56] Olson et al. L1044-L1045。

---

### B2. `sparta2020.txt`

**元数据**
- 文件：`.../sparta2020.txt`
- 题头：L1-L2 “SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators”
- 作者：L3-L13 “JAVIER PICOREL, SEYED ALIREZA SANAEE KOHROUDI, ZI YAN, ABHISHEK BHATTACHARJEE, BABAK FALSAFI, DJORDJE JEVJDIC”
- L4 “arXiv:2001.07045v1 [cs.AR] 20 Jan 2020”。题头无正式会议/期刊/DOI。
  → **year 2020，venue null，DOI null，publication_status preprint**。不能凭内容推测正式发表。

**精确机制**
- 拆分翻译：加速器侧 tiny TLB 只覆盖 cache hierarchy；主存访问由 shared memory-side TLBs 翻译。L20-L31、L186-L198。
- 虚拟地址唯一决定 memory partition；物理页可映射到分区内任意位置（百万级选项）。L220-L225、L383-L399。
- 数据与 PTE 同分区，页表遍历局部化；翻译与数据读取重叠。L481-L487、L488-L495。
- 支持无 cache、virtual cache、physical cache 加速器；physical cache 下只需极小 TLB 覆盖 cache hit。L505-L518。
- OS：stock Linux 4.10，复用 NUMA membind 做 partitioning；inverted page table；每 PTE valid bit 由 CPU 和加速器原子访问。L526-L600、L590-L595。
- CoW：支持 fork/CoW，新页复制到同一 partition；DIPTA 不支持 CoW。L609-L616。

**复用粒度及身份/权限匹配前提**
- 复用粒度：物理页/partition-local translation；per-partition TLB 只保存本分区 frame。L443-L452。
- 前提：`MEM_PARTITION_INDEX_HASH()` 保证虚拟页唯一映射到某分区；共享物理页时需调整虚拟地址区域以匹配已有物理页的分区序列。L543-L589。
- 权限匹配：声称保留 fine-grained protection、demand paging、CoW、隔离；但未详述 memory-side TLB 中每设备权限位、device ID、权限匹配流程。L287-L291、L400-L409。

**资源共享 vs 映射共享/SVA**
- 资源共享：per-partition TLB 被所有访问该分区的线程/加速器共享。
- 映射共享/SVA：共享数据翻译不复制；统一虚拟内存目标。L393-L399。
- 不等于数据零拷贝/取消隔离：它保留 VM 保护、demand paging、CoW；但本文重点是翻译重叠，未把 SVA 等同于零拷贝。L279-L296、L400-L409。

**评价配置**
- 真实硬件测量：Intel Broadwell 1.5K-entry L2 TLB，perf 10 分钟窗口；Pin traces 128GB，1B instructions。L307-L315、L649-L654。
- Linux 4.10 原型，32 memory partitions/NUMA nodes，运行 RocksDB 16GB，>90% DRAM 使用无 page fault。L668-L671、L750-L757。
- CPI 模型估性能：accelerator 为简单 in-order core，包含 load/store 频率、cache/TLB 命中率、miss latency。L656-L662。
- 8-socket 128GB，4 channels/socket，4KB/2MB page，accelerator 16KB 4-way virtual cache，128-entry TLBs，baseline 有 perfect MMU caches。L954-L961。

**限制/未覆盖**
- 预印本；无正式发表证据。
- 无 memory-side TLB 硬件原型；全系统 TLB miss/page fault 仿真不现实，用 CPI 模型。L656-L662。
- 32-partition Linux 原型需多 1.5-2GB 内存达到同 page fault rate，作者称是 NUMA nodes overhead 而非 mapping 灵活性。L750-L757。
- 未详述 A/D 位、device ID spoofing、TLBI 等待/排空、实时迁移、远端 PTW（其目标就是避免远端 PTW）。
- thread contention：per-partition TLB 被多线程共享，miss rate 随 partition 增多下降，但未测尾延迟。L828-L864。

**对可能创新点的最强反证**
- SPARTA 报告平均降低翻译开销 31.5×、性能 +57%、SPARTA-32 4KB 达 ideal 93.7%、SPARTA-128 2MB 达 98%。如果新方案只是继续优化 TLB 命中率/页表遍历，必须解释为何还显著优于“内存侧分区 + 数据/PTE 共置 + 翻译数据重叠”。L243-L245、L966-L973。
- 但反证受限于 CPI 模型、预印本、无 memory-side TLB 硬件、权限匹配细节不足。

**后续必追引文**
- DIPTA / set-associative VM：[40] Picorel et al. L1163-L1164。
- Devirtualizing memory：[20] Haria et al. L1109-L1111。
- LegoOS：[45] Shan et al. L1173-L1175。
- Mitosis：[3] Achermann et al. L1068-L1069。
- Range Translations：[16] Gandhi et al. L1101-L1103。

---

### B3. `sharemmu2026.txt`

**元数据**
- 文件：`.../sharemmu2026.txt`
- 题头：L1-L2 “ShareMMU: Supporting Secure Address Translation Sharing among Untrusted Accelerators”
- 作者：L3-L11 “Faiz Alam, Mohamed Tarek Ibn Ziad, Yuanchao Xu, Abdulrahman Mahmoud, Greg Byrd, Aamer Jaleel”
- L1001-L1003 “We sincerely thank the anonymous reviewers from MICRO 2026…”  
  → 结合任务给定信息：**MICRO 2026 camera-ready，当前 2026-10-06 应标 accepted_upcoming**。venue 可用 “MICRO 2026”；DOI null。

**精确机制**
- Permission Tracker：IOMMU 内硬件 cache，每个 Secure Vector 以 PPN 为标签，记录每加速器权限、Access Mask、VPN、LRU、alias presence/counter。L351-L394。
- Permission Tracker inclusive 私有 TLB：private TLB 任一合法访问都能在 PT 找到；PT eviction 时向私有 TLB 发 back-invalidation。L395-L405。
- 访问控制：private TLB hit 时，加速器发物理地址给 IOMMU 验证；PT hit 且权限正确才允许，否则标记 malicious。L424-L432。
- private TLB miss + shared TLB hit：先查 PT，权限匹配则把共享翻译给 private TLB。L433-L437。
- 权限更新：用现有 TLB shootdown；降权时只清对应加速器 permission bit，Secure Vector 若其他加速器仍用则保留；PT eviction 时按 Access Mask/VPN 发 targeted back-invalidation。L419-L436、L443-L455。
- back-invalidation 瞬态：S_VALID → S_BCKInV → S_INFMem → S_INVALID；等待加速器写回脏 cache line、flush private TLB、发 flush ack；协议 deadlock-free。L487-L535、L560-L575。
- AA-LRU：victim score = LRU + Access Mask 重用位数，防止恶意加速器独占 PT。L457-L486。
- Alias：多 VA 映射同 PPN 时用 Alias Table，记录所有虚拟地址以做 back-invalidation。L519-L535。
- ShareMMU-SP：QARTA IDF 做 TLB set randomization + crypto-partition；PT 在 context switch 时 flush，由 shared TLB/Alias Table 重建。L536-L645。

**复用粒度及身份/权限匹配前提**
- 复用粒度：PPN 页面翻译；Secure Vector 以 PPN 为标签，记录每 accelerator 权限和映射到该 PPN 的 VPN。L122-L125、L351-L361。
- 身份/权限匹配：Access Mask 每加速器 1 bit；deviceID 用于 PT hit 更新和 back-invalidation。L357-L361、L469-L486。
- 前提：共享页表，叶节点存每加速器权限位；若私有页表，则 PT 权限更新类似 shared TLB 更新。L376-L394、L411-L414。
- 威胁模型：加速器/private TLB 不可信；shared TLB、IOMMU、OS、页表可信；要求 spatial safety（只访问特定物理地址）和 temporal safety（只限时间窗）。L311-L349、L110-L113。

**资源共享 vs 映射共享/SVA**
- 资源共享：shared TLB、Permission Tracker。
- 映射共享：多个加速器可复用同一已翻译 PPN，但必须通过 PT 权限匹配。
- 不等于数据零拷贝/取消隔离：ShareMMU 的核心就是不能在不可信加速器间无条件共享；它验证每个预翻译访问，支持权限降级、back-invalidation、写回/排空。L79-L125、L419-L436、L487-L535。

**评价配置**
- gem5 PARADE cycle-accurate full-system simulator。L726-L728。
- 8-issue OoO x86 @2GHz，2 processes，context switch 5ms；L1 32KB，L2 256KB/slice ×32；AFU n=4/8/16/32；private TLB 16-entry 2-way 1 cycle；shared TLB 1024-entry 8-way 3 cycles；PT 256-entry 16-way 5 cycles；PTWC 32-entry；4KB page；memory 120 cycles。L670-L708。
- 9 个 PARADE 加速器 benchmark，覆盖 Medical Imaging、PARSEC、Computer Vision、Computer Navigation。L709-L738。

**限制/未覆盖**
- 仿真，无真实硬件/FPGA。
- 威胁模型只覆盖 conflict/occupancy 类侧信道；TEE 扩展留 future。L593-L605。
- PT 容量/结合度有限；4-way 到 fully-assoc overhead 5.04%-8.05%，16-way sweet spot。L912-L930。
- context switch 会 flush PT，虽可由 shared TLB/Alias Table 重建，但 LPCIP/Denoise/Segmentation 有 4.1%/1.4%/1.3% overhead。L913-L926。
- 未显式讨论 A/D 位；未量化 device ID spoofing；未覆盖页面迁移真实尾延迟（只把 page migration 列为 shootdown 原因之一）。L443-L447。

**对可能创新点的最强反证**
- ShareMMU 相对 unsecured baseline 仅 1.66% slowdown，ShareMMU-SP 2.82%；而 Border Control 32.19%、CryptoMMU 17.21%。如果新方案要“安全共享 TLB”，必须解释为何能超过这个开销和安全概率。L777-L825、L940-L979。
- 反证受限于仿真、威胁模型、无硅后、A/D 位未覆盖；不能直接推广到真实多租户 SoC。

**后续必追引文**
- CryptoMMU：[12] Alam et al. L1055-L1062。
- Border Control：[36] Olson et al. L1084-L1086。
- SassCache：[37] Giner et al. L1087-L1090。
- Intel VT-d HPT：[85] Intel L1231-L1232。
- Secure TLBs：[86] Deng et al. L1233-L1236。
- TLB shootdown/ecoTlb 等：[70]-[74] L1168-L1187。

---

### B4. `dvm2018.txt`

**元数据**
- 文件：`.../dvm2018.txt`
- 题头：L1 “Devirtualizing Memory in Heterogeneous Systems”
- 作者：L2-L4 “Swapnil Haria, Mark D. Hill, Michael M. Swift”
- L44-L49 “In Proceedings of 2018 Architectural Support for Programming Languages and Operating Systems (ASPLOS’18) … https://doi.org/10.1145/3173162.3173194”
  → **published, ASPLOS’18, 2018, DOI 10.1145/3173162.3173194**。

**精确机制**
- Identity Mapping：堆分配尽量 VA==PA；多数访问无需页级翻译。L18-L27、L80-L93。
- DAV：验证 VA 是 identity-mapped 且进程有足够权限；若 PA!=VA，回退常规地址翻译。L178-L182。
- Permission Entry：新页表项格式，16 个 2-bit permission fields；L2PE 映射 2MB 的十六个 128KB 区域，L3PE 映射 1GB 的十六个 64MB 区域；减少页表大小。L225-L285。
- AVC：128-entry、4-way、64B block、1KB，缓存所有页表级别，替代 TLB 和 PWC。L278-L304。
- Preload on reads：预测 VA==PA 并并行 preload；DAV 成功则 preload 即实际读，失败则丢弃重试。L306-L315。
- OS：Linux 4.10，eager contiguous allocation，flexible address space，heap 改用 mmap，memory-mapped segments，CoW/fork 讨论。L317-L357、L363-L408。

**复用粒度及身份/权限匹配前提**
- 复用粒度：region-level permissions（PE 的 16 个区域），identity mapping 按页/区域。L249-L260。
- 权限匹配：DAV 检查进程是否持有该访问的有效权限；权限 2-bit：00 no, 01 read-only, 10 read-write, 11 read-execute。L201-L206。
- 前提：堆可 eager contiguous 分配；VA==PA 尽量成立；否则 fallback demand paging。L317-L357。

**资源共享 vs 映射共享/SVA**
- 不是 TLB 共享；是通过 identity mapping 减少翻译。
- 保留 VM 抽象和权限检查，不取消隔离。L219-L222、L363-L373。
- 不把 SVA 等同于零拷贝；它保留 conventional VM protection。L219-L222。

**评价配置**
- Linux 4.10 实现。L28、L332-L357。
- gem5 + Graphicionado graph accelerator，8 processing engines，32GB，DDR4 4 channels，CPU 3GHz，accelerator 1GHz，TLB 128-entry FA，PWC/AVC 128-entry 4-way。L414-L453。
- Graph workloads：BFS、PageRank、SSSP、Collaborative Filtering；多个图数据集。L455-L464。
- CPU cDVM：Intel Xeon E5-2430，96GB，硬件性能计数器 + BadgerTrap + 分析模型。L661-L684。

**限制/未覆盖**
- Identity mapping 需 eager contiguous allocation，加重碎片化；作者明确说本文不研究 fragmentation 影响。L79-L80、L335-L350。
- CoW 首写创建非 identity-mapped 副本；建议 fork 前分配共享结构，或 posix_spawn/vfork。L395-L408。
- 低内存回退 demand paging；ASLR 熵可能降低；Meltdown/Spectre 可能影响 preload；安全分析超出范围。L351-L357、L363-L393。
- 仅堆 identity map；code/stack 只部分 identity map。L319-L323、L597-L654。
- 未讨论 A/D 位、多设备权限匹配、TLBI 等待/排空、远端 PTW。

**对可能创新点的最强反证**
- DVM 在 graph accelerator 上 VM overhead 仅 1.6%，比 optimized conventional VM 快 2.1×、能量少 3.9×；cDVM 把 CPU VM overhead 从 29% 降到 5%。如果新方案仍在做 TLB/页表遍历，必须解释为何能超过“多数访问不翻译”的 1.6%。L28-L35、L466-L524、L673-L684。
- 反证受限于 identity mapping 的碎片化/CoW/ASLR 代价和 graph workload 特性。

**后续必追引文**
- Direct Segments：[5] Basu et al. L748-L751。
- RMM：[32] Karakostas et al. L792-L796。
- Border Control：[41] Olson et al. L764-L765。
- Cong et al. accelerator translation：[16] L723-L727。
- CoLT：[44] Pham et al. L777-L780。

---

### B5. `riscvsva2025.txt`

**元数据**
- 文件：`.../riscvsva2025.txt`
- 题头：L1-L3 “Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs”
- 作者：L4-L9 “Cyril Koenig, Enrico Zelioli, Luca Benini”
- L14 “arXiv:2502.17398v1 [cs.AR] 24 Feb 2025”。题头无 venue/DOI。
  → **year 2025，venue null，DOI null，publication_status preprint**。

**精确机制**
- 在开源 RISC-V Cheshire SoC + Snitch cluster 集成 RISC-V IOMMU v1.0。L34-L53、L93-L113。
- IOMMU 配置 4 IOTLB entries、1 device directory entry，缓存一个 (device, process) 的 root page table；IOTLB miss 可有 3 次顺序内存访问。L93-L103、L239-L247。
- SVA：主机创建 IOVA 映射，加速器通过 IOVA 访问；zero-copy offload。L140-L166、L167-L195。
- shared LLC 只服务 host 和 IOMMU PTW；DMA 绕过 LLC，主机 self-invalidate。L263-L281。
- self-invalidation 伪代码：flush L1/LLC，create mapping，flush L1，OpenMP target map，device kernel with LLC_BYPASS_OFFSET。L196-L205。

**复用粒度及身份/权限匹配前提**
- 复用粒度：IOVA page（4KB），device directory entry 按 (device, process)。L93-L103、L122-L124。
- 权限/身份：Linux IOMMU driver 把 accelerator attach 到 IOMMU domain，创建 IO-virtual-physical mapping；instruction cache 可用不同 device identifier 指向 bypassed device directory entry。L140-L165。
- 前提：内核 6.9 补丁支持 RISC-V IOMMU v1.0；OpenMP target offload；LLC 可配置为 cache/SRAM。L80-L90、L140-L148。

**资源共享 vs 映射共享/SVA**
- 资源共享：host 与 IOMMU PTW 共享 LLC。
- 映射共享/SVA：host 与 accelerator 共享 IOVA 空间，zero-copy offload。
- 不等于取消隔离：仍使用 IOMMU 翻译/保护；DMA 绕过 LLC 需 self-invalidation 保证一致性。L107-L108、L269-L281。

**评价配置**
- Xilinx Ultrascale+ VCU128 FPGA，Snitch 20MHz，host/IOMMU 50MHz；DRAM delayer 模拟高延迟。L109-L124。
- Cheshire 128KiB LLC/SPM，CVA6 64-bit，32KiB write-through D-cache；2GiB DDR；IOMMU 4 IOTLB。L120-L124。
- RajaPERF 子集：gemm 128×128、gesummv 512×512、heat3d 64³、axpy 32768、merge sort 65536；单精度、tiling、double buffering。L127-L135、L149-L166。
- DRAM 延迟 200/600/1000 cycles；比较 baseline、IOMMU、IOMMU+LLC。L185-L195。

**限制/未覆盖**
- FPGA emulation，非硅后；低时钟 + DRAM delayer，主存延迟被抬高以补偿。L109-L124。
- IOMMU 仅 4 IOTLB entries，无自定义 IOTLB coalescing/prefetching。L93-L103、L330-L338。
- 需要 shared LLC 才能把 overhead 降到 <2%；DMA 必须绕过 LLC，主机需 self-invalidate；host 干扰使 PTW 平均慢 20%。L248-L257、L263-L281。
- 未评估 TLB shootdown、页面迁移、A/D 位、device ID spoofing、远端 PTW 的失效路径。

**对可能创新点的最强反证**
- 无 LLC 时 IOMMU 翻译 overhead 可达 gemm 17.6%、heat3d 81.3%；加 shared LLC 后所有 kernel <2%，PTW 降低 15×。若新方案只优化 IOTLB，可能不如“让 IOMMU PTW 命中 shared LLC”直接。L34-L53、L239-L268、L284-L302。
- 反证受限于 FPGA 仿真、低时钟、4-entry IOTLB、单一 SoC 配置。

**后续必追引文**
- RISC-V IOMMU IP：[7][8] L334-L338。
- Cong et al. accelerator translation：[9] L339-L341。
- Neighborhood-aware GPU translation：[10] L343-L344。
- HERO：[21] L386-L391。
- Kurth et al. TLB prefetching/MMU-aware DMA：[22] L392-L394。
- Active Forwarding：[23] L395-L398。
- Price of Safety：[24] L399-L401。
- CoLT：[25] L402-L404。
- DAMN：[26] L405-L410。

---

### B6. `border2015.txt`

**元数据**
- 文件：`.../border2015.txt`
- 题头：L1 “Border Control: Sandboxing Accelerators”
- 作者：L3-L5 “Lena E. Olson, Jason Power, Mark D. Hill, David A. Wood”
- L54 “MICRO-48 December 05-09, 2015, Waikiki, HI, USA”；L59 “DOI: http://dx.doi.org/10.1145/2830772.2830819”
  → **published, MICRO-48 / 48th Annual IEEE/ACM International Symposium on Microarchitecture, 2015, DOI 10.1145/2830772.2830819**。

**精确机制**
- 在不可信加速器与可信系统边界检查每个物理地址请求的页权限；加速器可用 TLB 和物理缓存，但请求到内存前必须验证。L24-L35、L101-L107、L204-L241。
- Protection Table：每 active accelerator 的 flat table，2 bits/PPN（R/W），物理索引，初始化零权限；ATS 翻译时更新；保证合法请求的权限最新。L242-L266、L313-L321。
- BCC：缓存 Protection Table，按 PPN tag，subblock 多页/entry，128B block 对应 512 pages 权限；不要求硬件一致性。L306-L321、L341-L345。
- 操作：进程初始化、ATS 插入、加速器请求、mapping update、进程完成。L347-L418。
- Mapping update / permission downgrade：需写回脏块，再更新 Protection Table/BCC；若加速器忽略 flush，后续 writeback 会被权限错误拦住。L419-L437。
- 多进程加速器：权限取并集。L449-L458。
- 大页：插入大页翻译时更新覆盖的所有 4KB 权限表项。L493-L507。

**复用粒度及身份/权限匹配前提**
- 复用粒度：physical page number（PPN），2-bit R/W。L242-L266。
- 身份/权限匹配：每 active accelerator 一个 Protection Table；ATS 检查 address space ID 对应进程；不需要反向翻译，只需判断是否存在映射到该 PPN 且权限足够的虚拟页。L204-L241、L242-L266、L379-L384。
- 前提：ATS 提供的物理地址是合法翻译来源；若加速器发非 ATS 获得的物理地址，行为 undefined，只求安全。L322-L332。

**资源共享 vs 映射共享/SVA**
- 不是翻译共享；是“预翻译物理访问 + 权限检查”。
- 兼容 shared virtual memory，但不把 SVA 等同于取消隔离；核心是 sandbox。L9-L15、L97-L107。
- 不要求虚拟缓存；支持物理缓存和物理地址 TLB。L210-L225。

**评价配置**
- gem5-gpu simulator，MOESI，null directory，GPU 配置 highly threaded 8 cores / moderately threaded 1 core；Rodinia benchmarks。L536-L607。
- BCC 8KB，Protection Table 196KB，BCC latency 10 cycles，table latency 100 cycles；高并发 GPU 0.11 requests/cycle，BCC miss <0.1% for 1KB 512 pages/entry。L609-L686。
- Border Control-BCC overhead：highly threaded 0.15%，moderately threaded 0.84%。L673-L679。

**限制/未覆盖**
- 仿真，无真实硬件。
- 威胁模型排除加速器内部错误/侧信道、OS 给错权限；只保护跨边界物理访问。L135-L159、L227-L262。
- 不讨论 A/D 位；不讨论 device ID spoofing（依赖 ATS address space ID）。
- 不覆盖远端 PTW、页面迁移、TLBI 等待/排空，只覆盖 permission downgrade 写回与刷新。L713-L735。

**对可能创新点的最强反证**
- Border Control-BCC 在仿真中仅 0.15% overhead（highly threaded）、存储 0.006% physical memory/accelerator；任何“安全沙箱”方案必须解释为何能接近这个开销。L24-L35、L673-L686。
- 反证受限于仿真、威胁模型、无侧信道/TEE、无 A/D 位。

**后续必追引文**
- rIOMMU：[43] L804-L805。
- Hive：[45] L756-L758。
- Mondriaan：[31] L772-L773。
- CAPI：[12] L792-L794。
- ATS/IOMMU specs：[22]-[24] L822-L827。
- VIPS-M/FUSION：[38][10] L787-L790。

---

### B7. `svm2016.txt`

**元数据**
- 文件：`.../svm2016.txt`
- 题头：L1-L2 “Observations and Opportunities in Architecting Shared Virtual Memory for Heterogeneous Systems”
- 作者：L3-L7 “Jan Vesely, Arkaprava Basu, Mark Oskin, Gabriel H. Loh, Abhishek Bhattacharjee”
- 题头无 venue。`riscvsva2025.txt` L375-L378 引：“2016 IEEE International Symposium on Performance Analysis of Systems and Software (ISPASS), pp. 161–171, 2016”。  
  → **published, ISPASS 2016, year 2016, DOI null**（以他文引用为 venue 证据）。

**精确机制**
- AMD A10-7850K APU（Kaveri）上实测 CPU-GPU shared virtual memory。L157-L166。
- GPU TLB miss → ATS 请求经 PCIe-based 内部互连到 IOMMU；IOMMU 有 TLB hierarchy 和硬件页表遍历器；ATS 带 PASID；IOMMU 用 PASID→page table base 表。L111-L133。
- GPU page fault：IOMMU 找不到翻译 → ATS response 失败 → GPU 发 PPR；IOMMU 入内存队列并中断 CPU；OS driver 异步 work-queue 处理。L83-L110。
- TLB shootdown：driver 先命令 IOMMU 失效，再令 GPU 失效，IOMMU 收集 GPU TLB 完成通知。L111-L133。
- ATS 支持一次预取最多 8 个连续 4KB 页。L129-L133。

**复用粒度及身份/权限匹配前提**
- 复用粒度：PASID/process address space + page。
- 身份/权限：PASID 匹配 page table base；IOMMU 服务所有加速器。L122-L124。
- 前提：HSA runtime、KFD、C++AMP/OpenCL；共享虚拟寻址。L176-L179。

**资源共享 vs 映射共享/SVA**
- SVA / pointer-is-a-pointer；共享页表。L54-L62、L94-L101。
- 不是零拷贝自动等于无隔离；仍由 IOMMU 翻译和权限支持。L62-L70。
- 实测的是“共享虚拟内存代价”，不是新机制。

**评价配置**
- AMD A10-7850K APU，CPU max 3.7GHz，GPU 8 CUs 720MHz，DDR3 32GB 1600MHz，Linux 4.0 KFD/HSA。L167-L179。
- 6 个应用：B+ Tree Search、CoMD、miniAMR、miniFE、Graph500、XSBench；2 个微基准。L181-L191、L143-L166。
- 硬件性能计数器 + 自研 profiler + instrumented Linux IOMMU driver。L157-L166。

**关键测量**
- GPU TLB miss 582ns，CPU 23ns，约 25×。L193-L227。
- IOMMU 约 16 并发页表遍历，超过后排队，延迟显著跳升。L235-L249。
- BPT、CoMD、miniAMR 对 TLB miss 敏感；大页最多改善 11.9%/9.7%/4.6%。L251-L275。
- 差局部性下 slowdown 6-25×；1GB page 把 TLB miss 几乎消除，slowdown 从 6×→1.5×、26×→7×。L502-L549。
- 翻译预取对 XSBench-unsorted 可增 24% TLB miss、近 3× 性能退化；对 miniAMR 减半 TLB miss。L502-L549。
- GPU page fault 5-140µs，CPU ~1.7µs，3-80×慢；吞吐 32 并发后不扩展。L638-L687。
- TLB shootdown 4.2-4.4µs，single-entry 与 all-entry 相近；串行化可能成瓶颈。L764-L803、L828-L902。

**限制/未覆盖**
- 单一商用平台、第一代硬件；第二代 APU 仅初步。L266-L274、L874-L878。
- 只测平均延迟，不测尾延迟分布。
- 无新机制；不讨论 A/D 位、页面迁移、device ID spoofing、TEE。
- TLB shootdown 串行化是观察，不是解决方案。

**对可能创新点的最强反证**
- 真实硬件显示 GPU TLB miss 25× CPU、IOMMU 16 并发 PTW、page fault 3-80× CPU、shootdown 4.2-4.4µs 且串行。只改善 TLB 容量/命中率不能解决 page fault、shootdown、并发页表遍历瓶颈。L193-L249、L638-L687、L828-L902。
- 但该平台是 2016 Kaveri，不能直接代表 2026 硬件。

**后续必追引文**
- Pichai et al. GPU MMU：[21] L918-L925。
- Power et al. x86-64 GPU 地址翻译：[22] L926-L930。
- PCIe ATS：[17] L954-L956。
- HSA Foundation：[26] L947-L948。
- DiDi TLB shootdown：[25] L940-L946。

---

### B8. `socdma2018.txt`

**元数据**
- 文件：`.../socdma2018.txt`
- 题头：L1-L3 “Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine”
- 作者：L4-L8 “Andreas Kurth, Pirmin Vogel, Andrea Marongiu, Luca Benini”
- L9 “arXiv:1808.09751v1 [cs.AR] 29 Aug 2018”
- L65-L66 “Accepted for publication in Proceedings of the 36th IEEE International Conference on Computer Design (ICCD), October 7-10, 2018, Orlando, FL, USA”
  → **year 2018，venue ICCD 2018，DOI null；文本写 accepted for publication，本报告按 published/accepted 处理，严格说最终版页码/DOI未在全文出现。**

**精确机制**
- 面向 HeSoC：host multicore CPU + PMCA；PMCA 用 SPM 和 DMA。L42-L59、L203-L211。
- 混合 IOMMU：共享 TLB 由 PMCA 上的软件 PE 管理；TLB miss 时 IOMMU 入硬件队列、返回 error、丢弃 transaction；PE 读 miss metadata、走页表、替换 TLB、通知 PE 重试。L201-L212、L353-L357。
- 三个贡献：
  1. 编译器生成 Prefetching Helper Threads，提前填 TLB，针对 linked data structures。L84-L88、L259-L352。
  2. 可变数量并行 Miss Handling Helper Threads，保持 miss 处理延迟随并行工作负载扩展；MHT 共享状态、避免重复页表遍历、用 atomic counter 管 TLB set replacement。L89-L91、L353-L374。
  3. Hybrid-IOMMU-capable DMA engine：retirement buffer 记录 in-flight burst metadata，miss 后重发；AXI burst 不跨页，每 burst 需一个 TLB entry；buffer 存 metadata 而非数据。L92-L100、L376-L418。
- DMA retirement buffer：linked list in hardware，entry 含 external/internal address、length、AXI ID、DMA transfer ID、R/W、state、next；状态 free/in-flight/failed/peeked/reissuable。L376-L418。

**复用粒度及身份/权限匹配前提**
- 复用粒度：page（4KB），TLB entry 翻译 VA→PA。
- 身份/权限：混合 IOMMU 为 offloaded process 走页表；PE 管理 TLB。本文未展开每设备权限位/device ID/A/D。
- 前提：PMCA 与 host 共享 SVM；DMA burst 不跨页；TLB 共享；软件 PTW 由 PMCA PE 执行。L14-L40、L201-L212、L396-L401。

**资源共享 vs 映射共享/SVA**
- 资源共享：共享 TLB，多个 PE/cluster 共享。
- 映射共享/SVA：host 与 PMCA 共享虚拟内存。
- 不等于零拷贝/取消隔离：本文重点是 SVM 翻译开销和 DMA burst；未讨论不可信隔离。L14-L24。

**评价配置**
- Xilinx Zynq-7045 SoC，双核 ARM Cortex-A9 host + FPGA PMCA；host/PMCA 频率比 2133/500MHz，DDR3 533MHz。L431-L459。
- PMCA cluster：8KiB L1 I$、256KiB L1 data SPM、16 banks；DMA up to 8 AXI4 bursts in flight；L1 TLB 32-entry FA 1 cycle，L2 TLB 256-entry 8-way ≤6 cycles。L435-L448。
- 两个 benchmark：Pointer Chasing（图/不规则）、Stream Processing（规则流）；实际实现测量。L419-L475。
- 对比 SoA hybrid IOMMU [8] 和 ideal IOMMU。L460-L475。

**结果**
- PC：工作负载 1 cycles/B 时比 SoA 提升 4×；常见强度 <10 cycles/B 至少 50%。L478-L482。
- SP：移除 TLB entry locking，内存密集 kernel 最多 35% 提升；加 PHT 再 20-40%；整体 memory-intense 最多 60%，overhead 相对 ideal 低于 25%。L483-L543。
- DMA retirement buffer 64B vs 16KiB data buffer，2 个数量级更少内存。L545-L569。

**限制/未覆盖**
- Zynq FPGA emulation，不是优化后的硅后；作者说 FPGA 更像 emulator。L452-L458。
- 只两个基准，依赖编译器生成 PHT/MHT；PHT 生成算法不支持把单条访存流分给两个 PHT。L521-L524。
- 未讨论安全/不可信加速器、A/D 位、TLBI 等待/排空、页面迁移、远端 PTW。
- 软件 PTW 消耗 PMCA PE 资源；MHT/PHT 配置需 compile-time/runtime 调优。L510-L521。

**对可能创新点的最强反证**
- 在嵌入式 PMCA 场景，本文已用编译器 PHT + 多 MHT + MMU-aware DMA 把 irregular 最多提升 4×、regular 最多 60%，缓冲降低 2 个数量级。若新方案只做 TLB 预取或 miss 处理，必须解释为何还能显著超过这些实际测量。L95-L101、L478-L543、L545-L569。
- 反证受限于 FPGA emulation、两个基准、无安全模型。

**后续必追引文**
- Vesely et al. SVM observations：[9] L543-L544。
- Vogel et al. on-accelerator PTW：[8] L541-L542。
- Pichai et al. GPU MMU：[4] L531-L533。
- Power et al. x86-64 GPU translation：[15] L554-L555。
- ARM MMU-500：[5] L534-L535。
- PCIe ATS：[16] L556。
- Recency-based TLB preloading：[26] L574。
- TLB prefetching：[27] L575-L576。
- Shared LLC TLB/cooperative prefetchers：[28] L577-L578。

---

## C. 被证据支持的剩余瓶颈与不能断言的点

### C1. 被证据支持的剩余瓶颈

1. **TLB miss / 页表遍历延迟与并发上限**
   - `svm2016` 实测 GPU TLB miss 约 25× CPU；IOMMU 约 16 并发 PTW，超过后排队，延迟急升；GPU page fault 3-80× CPU，吞吐 32 并发后不扩展。L193-L249、L638-L687。
   - `accelerator_translation2017` 指出 IOMMU 页表遍历在队列下可 >1000 cycles，宿主页表遍历平均 58 cycles；共享 TLB 未命中 outstanding 数 4-instance 1.3、16-instance 4.9。L711-L728、L862-L867。
   - `riscvsva2025` 指出 IOTLB miss 需最多 3 次顺序内存访问；shared LLC 把 PTW 降低 15×，否则 gemm/heat3d overhead 显著。L239-L268、L284-L302。
   - `socdma2018` 说 TLB miss 处理由页表遍历内存延迟主导，单 MHT 成瓶颈，需多 MHT/PHT。L276-L299、L492-L521。

2. **失效 / 权限变更 / shootdown 等待与排空**
   - `sharemmu2026` 直接处理权限频繁变更：PCPKI 高；TLB shootdown + PT eviction back-invalidation；瞬态要求加速器写回脏行、flush private TLB、发 ack；协议 deadlock-free。L443-L467、L487-L535、L560-L575。
   - `border2015` 权限降级需写回脏块、更新 Protection Table/BCC；忽略 flush 会被后续 writeback 拦住。L419-L437、L713-L735。
   - `svm2016` TLB shootdown 4.2-4.4µs，single/all-entry 相近，串行化可能成未来瓶颈。L764-L803、L828-L902。
   - `accelerator_translation2017` 只假定 shootdown 支持，未量化等待/排空。L407-L416。
   - `riscvsva2025` 未测 shootdown；只做 self-invalidation 保 DMA bypass 一致性。L269-L281。

3. **队列 / 容量 / 公平性 / 尾延迟**
   - `sharemmu2026` Permission Tracker 容量有限、associativity 敏感；4-way 到 fully-assoc overhead 5.04%-8.05%，16-way sweet spot；context switch flush PT 有 4.1%/1.4%/1.3% overhead；NoC 额外 3.23%。L912-L938、L913-L926、L932-L938。
   - `accelerator_translation2017` 宿主 MMU 通常一次处理一个 page walk，可能成瓶颈；outstanding shared TLB miss 随实例数上升。L856-L867。
   - `sparta2020` per-partition TLB 被多线程共享，存在 bandwidth 和 interference 两个潜在问题；作者证明 partition 增多后 contention 下降，但未测 tail。L828-L864。
   - `border2015` BCC miss ratio / capacity 有分析；0.11 requests/cycle，BCC 不是瓶颈。L653-L686。
   - `riscvsva2025` IOMMU 仅 4 IOTLB entries；host 干扰使 PTW 平均慢 20%。L93-L103、L248-L257。
   - `svm2016` 页错误吞吐在 32 并发后不扩展。L638-L687。

4. **一致性 / 数据写回 / DMA 绕过缓存**
   - `riscvsva2025` shared LLC 只给 host/IOMMU PTW，DMA 必须 bypass LLC 且主机 self-invalidate；否则影响 DMA 带宽/一致性。L263-L281、L330-L338。
   - `sharemmu2026` back-invalidation 前必须等待加速器写回脏 cache line、flush private TLB、ack。L487-L535。
   - `border2015` permission downgrade 前必须写回脏块。L419-L437。
   - `svm2016` GPU page fault / shootdown 通过 IOMMU 与 OS 异步处理。L83-L133、L638-L687。

### C2. 不能断言的点

1. **A/D 位**
   - 所给 8 篇全文中没有一篇明确讨论 accessed/dirty 位、设备端 A/D 更新、A/D 位与 IOMMU 权限降级/迁移的交互。不能断言任何方案已处理 A/D 位。

2. **动态映射 / 页面迁移 / 热迁移**
   - `sharemmu2026` 把 page migration、memory compaction、CoW 列为 shootdown 原因，但没有测迁移延迟或迁移中翻译失效尾延迟。L443-L447。
   - `border2015` 只测 permission downgrade 频率，不测页面迁移。L713-L735。
   - `sparta2020` 支持 demand paging/CoW，但没有实时迁移评测。L400-L409、L609-L616。
   - `dvm2018` 明确 CoW 首写会破坏 identity mapping，建议 fork 前分配；不是完整迁移方案。L395-L408。
   - 不能断言这些论文覆盖了“设备正在使用翻译时后端迁移物理页”的场景。

3. **远端 PTW / NUMA 远端页表遍历**
   - `sparta2020` 通过数据/PTE 同分区避免远端 PTW；`riscvsva2025` 用 shared LLC 减少 PTW；`svm2016` 测 IOMMU PTW 但未区分远端 NUMA；`accelerator_translation2017` 把页表遍历卸载到宿主核 MMU。  
   - 不能断言这些方案覆盖了“远端 NUMA page table + device TLB 失效 + 迁移”的联合尾延迟。

4. **device identification / 权限匹配**
   - `border2015` 依赖 ATS address space ID 对应进程；`sharemmu2026` 用 Access Mask/deviceID 做每加速器权限，但威胁模型未显式评估 device ID spoofing；`riscvsva2025` 用 (device, process) directory entry；`accelerator_translation2017` 用 process ID。  
   - 不能断言已有论文覆盖了恶意设备伪造 device ID / PASID 的权限提升。

5. **TLBI 等待/排空尾延迟**
   - `sharemmu2026` 有 transient state 和 flush ack，是所给全文中对 back-invalidation 等待/排空最细的设计；但它只给平均/几何平均 overhead，没有 tail latency。
   - `svm2016` 测 shootdown 平均延迟，未测并发 shootdown 的尾延迟。
   - 不能断言已解决 TLBI 等待/排空尾延迟。

6. **SVA = 零拷贝 = 取消隔离**
   - 证据不支持。`riscvsva2025` 的 zero-copy offload 仍使用 IOMMU 翻译和 self-invalidation；`sharemmu2026`/`border2015` 在共享翻译或预翻译访问上仍做权限验证/沙箱；`sparta2020`/`dvm2018` 声称保留 VM 保护。  
   - 不能把 SVA 直接等同零拷贝或取消隔离。

7. **STAR / SP-IMPact / cuPTW / Avatar**
   - 所给 8 篇全文中没有这些论文的全文或题头。无法核对 STAR 正式版与 arXiv 关系；无法评价 SP-IMPact 是否只做系统干扰近邻；无法评价 cuPTW/Avatar 对“只能改善 TLB”的突破及代价。  
   - 只能从本批证据看到相关近邻：`svm2016` 的 IOMMU PTW 实测、`socdma2018` 的软件 PTW、`sparta2020` 的 memory-side PTW、`riscvsva2025` 的 shared LLC PTW 和 host interference。它们不能替代对 STAR/SP-IMPact/cuPTW/Avatar 的逐篇核对。

8. **跨论文排榜**
   - 各论文 baseline、仿真器/真实硬件、page size、workload、是否含安全、是否含 LLC 不同。不能把 0.15%、1.66%、1.6%、<2%、6.4% 等直接排成“谁最好”。本报告只报告各自基线和条件。

---

## JSON 输出

```json
{
  "papers": [
    {
      "id": "accelerator_translation2017",
      "title": "Supporting Address Translation for Accelerator-Centric Architectures",
      "authors": [
        "Jason Cong",
        "Zhenman Fang",
        "Yuchen Hao",
        "Glenn Reinman"
      ],
      "year": 2017,
      "venue": "IEEE International Symposium on High Performance Computer Architecture (HPCA)",
      "doi": "10.1109/HPCA.2017.19",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "simulation"
      ],
      "mechanism": "私有小TLB加共享L2 TLB，并将页表遍历卸载到宿主核MMU，以减少页表遍历次数和延迟。",
      "assumptions": [
        "加速器与宿主核处于同一应用虚拟地址空间/进程ID匹配",
        "TLB shootdown和page fault可支持",
        "使用4KB页进行主评估",
        "加速器访存规则、批量、适合数据tiling",
        "以IOMMU作为正确性和安全基线"
      ],
      "limitations": [
        "PARADE/gem5全系统仿真，无真实硬件",
        "未评估不可信加速器间权限隔离",
        "未讨论A/D位",
        "宿主页表遍历依赖宿主MMU端口和请求队列，可能成为瓶颈",
        "大页、TLB-aware tiling、banked shared TLB留作未来工作"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt",
          "location": "L1-L5",
          "claim": "题头给出题目和作者：Supporting Address Translation for Accelerator-Centric Architectures；Jason Cong, Zhenman Fang, Yuchen Hao, Glenn Reinman。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt",
          "location": "L104-L115",
          "claim": "IOMMU-only配置只达ideal地址翻译12.3%，需要私有TLB、共享TLB和更好页表遍历。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt",
          "location": "L135-L142",
          "claim": "16-32项私有TLB可降低30.4%页表遍历。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt",
          "location": "L499-L517",
          "claim": "512-entry共享L2 TLB采用mostly-inclusive策略。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt",
          "location": "L604-L626",
          "claim": "宿主页表遍历需要单独端口、请求队列和demux逻辑。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/accelerator_translation2017.txt",
          "location": "L725-L731",
          "claim": "三种设计分别达ideal的23.3%、51.8%、93.6%，组合方案差距6.4%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L723-L727",
          "claim": "他文引用给出HPCA 2017和DOI 10.1109/HPCA.2017.19。"
        }
      ]
    },
    {
      "id": "sparta2020",
      "title": "SPARTA: A Divide and Conquer Approach to Address Translation for Accelerators",
      "authors": [
        "Javier Picorel",
        "Seyed Alireza Sanaee Kohroudi",
        "Zi Yan",
        "Abhishek Bhattacharjee",
        "Babak Falsafi",
        "Djordje Jevdjic"
      ],
      "year": 2020,
      "venue": null,
      "doi": null,
      "publication_status": "preprint",
      "relevance": "core",
      "evaluation_type": [
        "measurement",
        "simulation",
        "analysis"
      ],
      "mechanism": "将翻译拆到加速器侧tiny TLB和内存侧共享per-partition TLB；虚拟地址决定内存分区，数据与PTE同分区，重叠翻译与数据读取。",
      "assumptions": [
        "虚拟地址唯一标识内存分区",
        "物理页可在分区内任意放置",
        "数据和PTE可共置于同一分区",
        "Linux NUMA/membind可复用于分区",
        "共享页表与细粒度保护可保留",
        "性能用CPI模型估计"
      ],
      "limitations": [
        "预印本，无正式发表证据",
        "无内存侧TLB硬件原型",
        "全系统TLB miss/page fault仿真不现实，使用CPI模型",
        "Linux 32-NUMA节点原型需额外1.5-2GB内存",
        "未详述每设备权限匹配、A/D位、实时迁移和TLBI排空"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt",
          "location": "L1-L13",
          "claim": "题头和作者；L4为arXiv:2001.07045v1。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt",
          "location": "L20-L31",
          "claim": "SPARTA拆分翻译，内存侧共享TLB，降低翻译开销并保留VM功能。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt",
          "location": "L220-L225",
          "claim": "虚拟地址识别分区，数据读取与翻译并行，数据和PTE同分区。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt",
          "location": "L590-L595",
          "claim": "inverted page table，每个PTE有valid bit，由CPU和加速器原子访问。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt",
          "location": "L656-L662",
          "claim": "使用真实硬件测量、trace-driven功能仿真、分析模型和Linux内核原型。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sparta2020.txt",
          "location": "L966-L973",
          "claim": "SPARTA-32达ideal 93.7%，SPARTA-128 2MB达98%。"
        }
      ]
    },
    {
      "id": "sharemmu2026",
      "title": "ShareMMU: Supporting Secure Address Translation Sharing among Untrusted Accelerators",
      "authors": [
        "Faiz Alam",
        "Mohamed Tarek Ibn Ziad",
        "Yuanchao Xu",
        "Abdulrahman Mahmoud",
        "Greg Byrd",
        "Aamer Jaleel"
      ],
      "year": 2026,
      "venue": "MICRO 2026",
      "doi": null,
      "publication_status": "accepted_upcoming",
      "relevance": "core",
      "evaluation_type": [
        "simulation"
      ],
      "mechanism": "IOMMU内Permission Tracker以PPN为标签记录每加速器权限和VPN，验证预翻译访问，支持共享TLB翻译重用、back-invalidation与AA-LRU。",
      "assumptions": [
        "共享页表，叶节点存每加速器权限",
        "共享TLB和IOMMU可信，加速器/private TLB不可信",
        "OS可信并设置合法权限",
        "Permission Tracker inclusive于私有TLB",
        "加速器会响应back-invalidation并写回脏行/flush私有TLB",
        "context switch间隔5ms，private TLB flush，shared TLB保留"
      ],
      "limitations": [
        "gem5 PARADE仿真，无真实硬件",
        "威胁模型不覆盖全部侧信道和TEE",
        "Permission Tracker容量和结合度有上界",
        "未显式讨论A/D位",
        "context switch flush Permission Tracker，PT延迟敏感",
        "别名和disjoint working set可能导致PT thrash"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt",
          "location": "L1-L11",
          "claim": "题头和作者。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt",
          "location": "L1001-L1003",
          "claim": "致谢MICRO 2026匿名审稿人。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt",
          "location": "L351-L394",
          "claim": "Permission Tracker和Secure Vector：PPN标签、每加速器权限、Access Mask、VPN、LRU、alias字段。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt",
          "location": "L419-L436",
          "claim": "权限更新用TLB shootdown，降权只清对应加速器bit，PT eviction发targeted back-invalidation。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt",
          "location": "L487-L535",
          "claim": "back-invalidation瞬态S_VALID/S_BCKInV/S_INFMem/S_INVALID，等待写回和flush ack，deadlock-free。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/sharemmu2026.txt",
          "location": "L777-L825",
          "claim": "ShareMMU平均slowdown 1.66%，ShareMMU-SP 2.82%，对比Border Control 32.19%、CryptoMMU 17.21%。"
        }
      ]
    },
    {
      "id": "dvm2018",
      "title": "Devirtualizing Memory in Heterogeneous Systems",
      "authors": [
        "Swapnil Haria",
        "Mark D. Hill",
        "Michael M. Swift"
      ],
      "year": 2018,
      "venue": "ASPLOS'18",
      "doi": "10.1145/3173162.3173194",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "simulation",
        "measurement",
        "analysis"
      ],
      "mechanism": "对堆分配做VA==PA身份映射，用DAV/PE/AVC验证身份映射和权限，读可preload与验证重叠，失败回退常规翻译。",
      "assumptions": [
        "堆可急切连续分配",
        "多数访问VA==PA",
        "PE可存16个区域权限",
        "AVC可替代TLB和PWC",
        "OS可支持灵活地址空间",
        "应用基本透明"
      ],
      "limitations": [
        "身份映射加重碎片化",
        "CoW首写破坏身份映射",
        "低内存回退demand paging",
        "ASLR熵可能降低，Meltdown/Spectre可能影响preload",
        "只对堆做身份映射",
        "安全分析超出本文范围"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L1-L4",
          "claim": "题头和作者。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L44-L49",
          "claim": "ASPLOS'18和DOI 10.1145/3173162.3173194。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L18-L35",
          "claim": "DVM用VA==PA身份映射和DAV，Linux 4.10实现，graph加速器VM开销1.6%，比优化VM快2.1X、能量少3.9X。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L249-L285",
          "claim": "Permission Entry有16个权限字段，L2PE映射2MB的16个128KB区域，L3PE映射1GB的16个64MB区域。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L395-L408",
          "claim": "CoW首写创建非身份映射副本；建议fork前分配共享结构。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/dvm2018.txt",
          "location": "L673-L684",
          "claim": "cDVM把CPU VM overhead从29%降到约5%。"
        }
      ]
    },
    {
      "id": "riscvsva2025",
      "title": "Evaluating IOMMU-Based Shared Virtual Addressing for RISC-V Embedded Heterogeneous SoCs",
      "authors": [
        "Cyril Koenig",
        "Enrico Zelioli",
        "Luca Benini"
      ],
      "year": 2025,
      "venue": null,
      "doi": null,
      "publication_status": "preprint",
      "relevance": "core",
      "evaluation_type": [
        "hardware_prototype",
        "measurement"
      ],
      "mechanism": "在开源RISC-V SoC集成IOMMU，硬件IOTLB/PTW支持SVA；shared LLC缓存host和IOMMU PTW，DMA绕过LLC并由主机self-invalidate。",
      "assumptions": [
        "RISC-V IOMMU v1.0",
        "设备目录项按(device, process)缓存根页表",
        "shared LLC只服务host和IOMMU PTW，DMA绕过",
        "Linux 6.9补丁和OpenMP offload",
        "FPGA emulation加DRAM delayer"
      ],
      "limitations": [
        "FPGA emulation，非硅后",
        "低时钟运行，DRAM延迟用delayer模拟",
        "IOMMU仅4个IOTLB项",
        "需要shared LLC才能把overhead降到2%以下",
        "DMA需绕过LLC，主机需self-invalidate",
        "未评估TLB shootdown、页面迁移、A/D位"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L1-L9",
          "claim": "题头和作者。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L14",
          "claim": "arXiv:2502.17398v1 [cs.AR] 24 Feb 2025。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L239-L247",
          "claim": "IOTLB miss需要三次额外内存访问，DMA transfer latency可增300%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L263-L281",
          "claim": "shared LLC缓存host和IOMMU PTW，DMA绕过LLC，主机self-invalidate。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L284-L302",
          "claim": "shared LLC把PTW时间降低15倍，平均PTW不超过200 cycles。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L341-L373",
          "claim": "结论：LLC使SVA overhead低于2%，DMA需绕过LLC。"
        }
      ]
    },
    {
      "id": "border2015",
      "title": "Border Control: Sandboxing Accelerators",
      "authors": [
        "Lena E. Olson",
        "Jason Power",
        "Mark D. Hill",
        "David A. Wood"
      ],
      "year": 2015,
      "venue": "MICRO-48",
      "doi": "10.1145/2830772.2830819",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "simulation"
      ],
      "mechanism": "在不可信加速器与内存边界检查每个物理地址请求的页权限；每加速器Protection Table和BCC缓存，翻译与权限检查解耦。",
      "assumptions": [
        "ATS提供的address space ID对应进程",
        "加速器物理请求首先来自ATS翻译",
        "页表权限可信",
        "多进程加速器权限取并集",
        "缓存不能为无写权限块提供数据",
        "权限降级需写回脏块"
      ],
      "limitations": [
        "gem5-gpu仿真，无真实硬件",
        "威胁模型排除加速器内部行为和侧信道",
        "未讨论A/D位",
        "未覆盖动态迁移、远端PTW、TLBI排空",
        "依赖ATS/IOMMU现有机制"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt",
          "location": "L1-L5",
          "claim": "题头和作者。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt",
          "location": "L54-L59",
          "claim": "MICRO-48 2015和DOI 10.1145/2830772.2830819。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt",
          "location": "L242-L266",
          "claim": "Protection Table每active accelerator、PPN粒度2-bit R/W，物理索引。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt",
          "location": "L306-L321",
          "claim": "BCC缓存Protection Table，128B block对应512页权限，显式管理不需要硬件一致性。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt",
          "location": "L673-L679",
          "claim": "Border Control-BCC在highly threaded GPU平均0.15%，moderately threaded 0.84%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/border2015.txt",
          "location": "L713-L735",
          "claim": "权限降级开销在常见context switch频率下约0.02%，高频率下仍小。"
        }
      ]
    },
    {
      "id": "svm2016",
      "title": "Observations and Opportunities in Architecting Shared Virtual Memory for Heterogeneous Systems",
      "authors": [
        "Jan Vesely",
        "Arkaprava Basu",
        "Mark Oskin",
        "Gabriel H. Loh",
        "Abhishek Bhattacharjee"
      ],
      "year": 2016,
      "venue": "2016 IEEE International Symposium on Performance Analysis of Systems and Software (ISPASS)",
      "doi": null,
      "publication_status": "published",
      "relevance": "foundation",
      "evaluation_type": [
        "measurement",
        "analysis"
      ],
      "mechanism": "在AMD Kaveri APU上实测CPU-GPU共享虚拟内存：GPU TLB miss经ATS到IOMMU，页错误经PPR队列，TLB shootdown经IOMMU，并量化延迟与并发。",
      "assumptions": [
        "AMD A10-7850K APU/Kaveri",
        "HSA/KFD/Linux 4.0",
        "PASID匹配页表基地址",
        "GPU使用ATS可预取最多8个连续4KB页",
        "应用数据驻留系统DRAM并使用SVA"
      ],
      "limitations": [
        "单一商用平台，第一代硬件",
        "第二代APU仅初步观察",
        "只测平均延迟，不测尾延迟分布",
        "无新机制，主要是表征",
        "未覆盖A/D位、页面迁移、device ID spoofing"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/svm2016.txt",
          "location": "L1-L7",
          "claim": "题头和作者。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt",
          "location": "L375-L378",
          "claim": "他文引用给出ISPASS 2016 venue。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/svm2016.txt",
          "location": "L193-L227",
          "claim": "GPU TLB miss约582ns，CPU约23ns，约25倍。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/svm2016.txt",
          "location": "L235-L249",
          "claim": "IOMMU约支持16个并发页表遍历，超过后排队使延迟急升。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/svm2016.txt",
          "location": "L638-L687",
          "claim": "GPU page fault 5-140µs，CPU约1.7µs，3-80倍慢；吞吐在32并发后不扩展。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/svm2016.txt",
          "location": "L764-L803",
          "claim": "GPU TLB shootdown平均4.2-4.4µs，single-entry与all-entry相近。"
        }
      ]
    },
    {
      "id": "socdma2018",
      "title": "Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine",
      "authors": [
        "Andreas Kurth",
        "Pirmin Vogel",
        "Andrea Marongiu",
        "Luca Benini"
      ],
      "year": 2018,
      "venue": "Proceedings of the 36th IEEE International Conference on Computer Design (ICCD)",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": [
        "hardware_prototype",
        "measurement"
      ],
      "mechanism": "混合IOMMU软件管理TLB；编译器生成PHT预取TLB项，并行MHT处理miss，DMA retirement buffer记录未命中burst并重发，无需大数据缓冲。",
      "assumptions": [
        "hybrid IOMMU，软件PTW",
        "PMCA共享TLB",
        "编译器可生成PHT/MHT",
        "DMA burst不跨页",
        "L1 SPM可共享MHT状态",
        "AXI4传输"
      ],
      "limitations": [
        "Zynq FPGA emulation，非优化硅后",
        "仅两个基准",
        "PHT生成算法不支持把单条访存流分给两个PHT",
        "未讨论安全/不可信隔离",
        "未评估TLBI等待/排空、页面迁移、A/D位",
        "软件PTW消耗PMCA PE资源"
      ],
      "evidence": [
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt",
          "location": "L1-L9",
          "claim": "题头、作者和arXiv:1808.09751v1。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt",
          "location": "L65-L66",
          "claim": "Accepted for publication in ICCD 2018。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt",
          "location": "L25-L40",
          "claim": "三个贡献：PHT预取、并行MHT、MMU-aware DMA无额外缓冲。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt",
          "location": "L376-L418",
          "claim": "DMA retirement buffer用硬件linked list记录burst metadata并重发。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt",
          "location": "L478-L543",
          "claim": "Pointer Chasing最多4倍提升，Stream Processing最多60%提升，相对ideal overhead低于25%。"
        },
        {
          "file": "iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt",
          "location": "L545-L569",
          "claim": "retirement buffer约64B，对比16KiB数据缓冲，少两个数量级。"
        }
      ]
    }
  ]
}
```