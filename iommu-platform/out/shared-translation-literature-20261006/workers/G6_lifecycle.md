# G6_lifecycle 多个设备共享地址翻译：证据阅读报告

> 阅读范围：用户提供的 13 篇全文。无联网、无执行、无写文件能力；以下只基于所给全文和题头/出版信息。未提供的 Pichai、cuPTW、Avatar、SP-IMPact、STAR 正式版/arXiv 对比等，不作为本组证据卡；其中 Pichai 只在本组若干参考文献中出现，用户已给 Rutgers 正式网页核对信息，但不把它列入本组 JSON。  
> 跨论文数字不直接排榜；每篇数字均写基线和条件。未覆盖 = 本组全文未覆盖，不能推成“全领域无人做”。

## A) 本组问题与机制演化

1. **2010–2011：把 TLB/翻译一致性从软件 shootdown 搬进硬件。**
   - UNITD 把 TLB 当作只读缓存纳入 cache coherence，用 PCAM 按 PTE 物理地址反向查找并失效 TLB 项，目标是消除软件 TLB shootdown。[unitd2010 L17-L29, L259-L268, L368-L379]
   - DiDi 用共享 inclusive 二级 TLB directory 记录哪个核持有翻译，配 PTLBI 非中断式远端 TLB invalidation，去掉 IPI 与 false-positive。[didi2011 L22-L48, L385-L401, L470-L502]
   - 这一阶段的共同问题：PTE 修改后，所有缓存了旧翻译的 TLB/MMU cache/nTLB 必须保持一致；硬件目录/CAM、共享目录、广播/多播与非中断失效是主要机制谱系。

2. **2017：从“更快 shootdown”转向“避免 shootdown”与虚拟化。**
   - SITE 给每个 TLB entry 加 expiration/lease，用 ETT 记录 PTE 最新过期时间；若 PTE 修改时最新 lease 已过期，则无需 shootdown。[site2017 L16-L22, L643-L653, L661-L672]
   - HATRIC 给 TLB/MMU cache/nTLB entry 加 co-tag，存“该翻译对应 PTE 的系统物理地址”，把翻译一致性 piggyback 到 cache coherence protocol 上，支持虚拟化 nested page table，避免 IPI/VM exit/invlpg 全刷。[hatric2017 L22-L30, L116-L123, L244-L256, L395-L431]
   - 问题焦点从“单机多核 TLB coherence”扩展到“虚拟化双层页表 + MMU cache/nTLB + 异构内存页迁移”。

3. **2018：软件 lazy shootdown 与迁移粒度。**
   - LATR 不改硬件，用 per-core Latr states 在 scheduler tick/context switch 时异步扫状态并本地失效 TLB；对 munmap/madvise/AutoNUMA 等 free/migration 操作可 lazy，对 mprotect/CoW/mremap 不可 lazy。[latr2018 L10-L39, L121-L129, L286-L316, L921-L933]
   - Granularity-aware page migration 指出 OS 迁移还有“迁移多少”这一维：把迁移粒度定义为连续虚拟内存区间，4KB/64KB/2MB，按 PTE access bits 推断空间局部性并动态调整。[granularity2018 L12-L28, L70-L78, L112-L122]

4. **2020：GPU UVM 生命周期与多 GPU 页迁移。**
   - Batch-Aware UVM 研究 GPU runtime 批量处理 page fault：批量处理造成串行化；Thread Oversubscription 增大 batch，Unobtrusive Eviction 把 eviction 从关键路径移开。[batch_uvm2020 L16-L36, L267-L275, L535-L551]
   - Griffin 面向 multi-GPU NUMA：Delayed First-Touch Migration 做页分布均衡，CPMS 批处理迁移，DPC 分类页，ACUD 选择性 drain CU，减少 TLB shootdown/pipeline flush。[griffin2020 L52-L66, L349-L373, L375-L399, L493-L512]

5. **2021：FPGA SVM 与 on-FPGA IOMMU。**
   - FPT migration 工作把 SVM physical page migration 做到 PCIe-attached FPGA：on-FPGA IOMMU 做地址翻译、触发 page fault；host driver 用 Linux HMM 迁移页、管理 TLB；支持 on-demand 与 user-managed migration。[fptmigration2021 L10-L26, L121-L137, L171-L192, L242-L250]

6. **2023：multi-GPU PTE invalidation 专门化。**
   - IDYLL 指出 UVM multi-GPU 中页面迁移引发的 PTE invalidation 与 demand TLB miss 争抢 page walk 资源；用 host-side in-PTE directory 记录哪些 GPU 持有有效 mapping，用每 GPU IRMB 批处理 invalidation，并 lazy 写回 PTE。[idyll2023 L27-L31, L474-L483, L502-L510, L563-L579]

7. **2024：IOMMU 被用于文件/共享设备路径；静态分析进入 UVM。**
   - BypassD 把 IOMMU 从“内存 IOVA->PA”扩展到“文件虚拟块地址 VBA->LBA”，内核 fmap 把 FTE attach 到进程页表，设备发 ATS 翻译请求，IOMMU 做权限检查；文件元数据仍由内核管，数据操作可从用户态直发设备。[bypassd2024 L31-L32, L263-L273, L460-L473]
   - SUV 用 LLVM 静态分析 CUDA kernel 和 host code，计算 access density、working set、iteration span，主动 pin/migrate/prefetch，access counters 只用于静态分析不友好的 pointer-chase/unknown。[suv2024 L16-L24, L240-L253, L531-L560]

8. **2026 前沿（文件为 2026，但正式题头未给）：observability 驱动 UVM 策略。**
   - ObservUVM 把 GPU 访问 DRAM 的 access counters 重用于“观测 HBM-resident 区域”：从 2MB 区域中抽样一个 64KB 页迁到 DRAM，统计 PCIe 访问，作为该区域是否被 GPU 活跃访问的证据；再用 userspace policy 做 eviction/prefetch。[observuvm2026 L73-L99, L373-L410, L445-L467]
   - 机制趋势：从“硬件一致性协议”走到“软件/驱动可编程策略 + 现有硬件计数器重利用 + 静态分析 + lazy/异步”。

**跨篇主线**：  
- **失效/一致性**：TLB shootdown -> hardware coherence -> co-tag/piggyback -> expiration/self-invalidation -> lazy software -> multi-GPU PTE invalidation batching。  
- **迁移生命周期**：Demand paging -> batching/eviction overlap -> multi-GPU migration classification -> granularity-aware migration -> compiler/static-analysis-guided UVM -> observability-guided eviction/prefetch。  
- **身份/权限匹配**：BypassD 是少数明确把 PASID、DevID、R/W、文件权限与 IOMMU 检查绑在一起的全文；多数 coherence 工作只保证翻译一致性，不处理“设备/进程是否有权访问该映射”。[bypassd2024 L307-L319, L460-L473, L562-L583]  
- **资源共享 vs 映射共享/SVA**：BypassD 共享的是 SSD 与文件，翻译对象是 VBA->LBA；Griffin/IDYLL 共享的是 multi-GPU 统一地址空间与远端页映射；FPT 共享 CPU/FPGA 虚拟地址空间；UNITD/DiDi/SITE/HATRIC/LATR 主要处理同系统翻译缓存一致性。不能把“共享页表/共享 VA”与“共享设备/资源隔离”混为一谈。

---

## B) 逐篇证据卡

### 1. bypassd2024
- **文件**：`iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt`
- **准确题名**：BypassD: Enabling fast userspace access to shared SSDs
- **作者**：Sujay Yadalam, Chloe Alverti, Vasileios Karakostas, Jayneel Gandhi, Michael Swift
- **年份/来源/DOI**：2024；ASPLOS ’24, Volume 1, April 27-May 1, 2024, La Jolla, CA, USA；DOI `10.1145/3617232.3624854`。证据：L13-L20, L54-L59。
- **机制**：IOMMU 扩展为可翻译 VBA->LBA，并做读写权限检查；内核 fmap() 把文件块 LBA 以 FTE 形式 attach 到进程页表；设备 NVMe 请求携带 VBA、操作、I/O size、队列 PASID，经 PCIe ATS 向 IOMMU 请求翻译；IOMMU walk 进程页表，比较 DevID 与 requester ID。[L31-L32, L263-L273, L304-L319, L460-L473]
- **复用粒度/身份前提**：文件级 mapping；FTE 以页表 radix-tree 粒度 attach；共享 file table entries 在多个进程间共享，R/W 最大权限预置，私有页表处再设权限。权限模型假设 kernel+hardware(device+IOMMU) 属于 TCB，用户进程含 UserLib 可恶意。[L411-L437, L562-L583]
- **资源共享 vs 映射共享/SVA**：共享 SSD 与文件，同时依赖 SVA 用 PASID walk 进程页表；但翻译对象不是普通内存 IOVA，而是文件 VBA->设备 LBA。这是“资源/文件共享”与“翻译共享”的结合。[L89-L97, L304-L319]
- **评价配置**：Intel Xeon Gold 5317，96GB，Intel Optane P5800X，Ubuntu 20.04 Linux 5.4；实现 Linux kernel/ext4/UserLib；IOMMU VBA 翻译开销用添加 delay 模拟，PCIe round-trip 约 345ns，VBA 翻译估 550ns；对比 sync/libaio/io_uring/SPDK/XRP/KVell。[L554-L559, L637-L662, L665-L668]
- **作者报告数字与基线**：4KB 访问延迟较标准 Linux kernel 降 42%；平均较 Linux kernel 好 25%；WiredTiger 提升约 20%；相对 XRP 在 storage-intensive 工作负载吞吐提升 9.6%。这些是各自基线/系统配置下数字。[L34-L39, L113-L118, L929-L939, L985-L987]
- **限制/未覆盖**：不支持 CoW、btrfs；只支持同步操作；不支持非阻塞写；不支持同一文件同时经 BypassD 与 kernel 接口访问；不支持跨 VM 文件共享；不做零拷贝；fmap 全文件映射，cold fmap 16GB 文件可达 21197.88us；设备端公平依赖 NVMe 队列 round-robin；IOMMU 硬件未商用，VBA 翻译为模拟。[L405-L406, L499-L508, L519-L539, L541-L559, L903-L906]
- **对可能创新点最强反证**：BypassD 已经把“IOMMU 翻译文件偏移、在 host 硬件做权限检查、共享设备安全直访”做成端到端设计；若创新点只是“用 IOMMU 检查 filesystem/LBA 权限”，该文是强反证。[L31-L32, L187-L201, L245-L248]
- **后续必追引文**：MonetaD [11]；DaxVM [6]；FlashMap [24]；SplitFS [32]；DevFS [33]；CrossFS [51]；Arrakis [49]；rIOMMU [42]；PCIe ATS [48]；SVA [41]；XRP [70]；KVell [38]。

### 2. idyll2023
- **文件**：`.../papers/idyll2023.txt`
- **准确题名**：IDYLL: Enhancing Page Translation in Multi-GPUs via Light Weight PTE Invalidations
- **作者**：Bingyao Li, Yanan Guo, Yueqi Wang, Aamer Jaleel, Jun Yang, Xulong Tang
- **年份/来源/DOI**：2023；MICRO ’23（56th Annual IEEE/ACM International Symposium on Microarchitecture），October 28-November 01, 2023, Toronto；DOI `10.1145/3613424.3614269`。证据：L41-L49。
- **机制**：In-PTE Directory 使用 host-side PTE 未用 bit 记录哪些 GPU 持有有效翻译，过滤 unnecessary invalidation；IRMB 每 GPU 硬件缓冲 invalidation 请求，按 VPN 高位归并、批量延迟写回 PTE；L2 TLB lookup 与 IRMB lookup 并行，IRMB hit 时绕过 page table walk，直接 far fault 或等待新 mapping。[L27-L31, L474-L483, L502-L510, L563-L579]
- **复用粒度/身份前提**：4KB 页；host-side PTE 访问位映射 GPU 访问信息；默认 4-GPU，11 个 unused bits；页面迁移策略保持 access counter-based migration；UVM driver 集中掌握所有 GPU 翻译。[L284-L295, L474-L491, L502-L510]
- **资源共享 vs 映射共享/SVA**：多 GPU 共享统一 UVM 地址空间与远端页映射；PTE invalidation 是为保证各 GPU 本地 page table 与迁移后映射一致。不是 IOMMU 设备 SVA 文章。
- **评价配置**：MGPUSim 多 GPU 模拟器，4-GPU baseline，每个 GPU 本地页表；另用 uvm-eval 在真实 2-GPU NVIDIA A100 上做 invalidation 开销 profiling。应用含 Hetero-Mark/AMDAPPSDK/SHOC/DNN-Mark。[L184-L204, L280-L307, L364-L370]
- **作者报告数字与基线**：相对 baseline（access counter-based migration）平均提升 69.9%；In-PTE only 27.3%，Lazy only 55.8%；zero-latency invalidation 可作理想对照。[L692-L714]
- **限制/未覆盖**：主要结果来自模拟；host-side unused bits 有限，GPU 数超过位数时 hash 产生 false positive；lazy invalidation 仍即时 TLB shootdown，只是 PTE 更新 lazy；迁移决策仍 reactive，不预测；大页 2MB 时增益降到 36.3%；DNN 推理增益 15.9%/12.0%。[L467-L485, L563-L579, L895-L908, L1042-L1074]
- **对可能创新点最强反证**：若创新点是“用 directory 过滤不必要 PTE invalidation + 批处理/惰性写回 + IRMB 旁路 page walk”，IDYLL 已覆盖 multi-GPU UVM 场景，并给出 69.9% 平均提升。[L27-L31, L692-L714]
- **后续必追引文**：Trans-FW [40]；Valkyrie [11]；MGPUSim [68]；uvm-eval [73]；GPS [47]；Pratheek page walk stealing [61]；MCM GPU VM [62]。

### 3. batch_uvm2020
- **文件**：`.../papers/batch_uvm2020.txt`
- **准确题名**：Batch-Aware Unified Memory Management in GPUs for Irregular Workloads
- **作者**：Hyojong Kim, Jaewoong Sim, Prasun Gera, Ramyad Hadidi, Hyesoon Kim
- **年份/来源/DOI**：2020；ASPLOS ’20, March 16–20, 2020, Lausanne, Switzerland；DOI `10.1145/3373376.3378529`。证据：L13-L19。
- **机制**：Thread Oversubscription 给每 SM 额外 inactive thread block，active block 因 page fault stall 时 context switch，增加同一 batch 的 page fault 数；Unobtrusive Eviction 在 batch 开始时预占一页 eviction，再让后续 eviction 与 CPU->GPU migration 双向重叠。[L24-L36, L307-L315, L444-L451, L535-L551]
- **复用粒度/身份前提**：GPU runtime 批量处理 page fault；模拟 64KB page size，GPU runtime fault handling 20us，PCIe 15.75GB/s；LRU page replacement；目标 irregular graph workloads。[L718-L742, L742-L743]
- **资源共享 vs 映射共享/SVA**：单 GPU UVM 内 HBM/DRAM 迁移与 eviction，不涉及多设备共享页表/SVA。
- **评价配置**：MacSim cycle-level simulator + 真实 NVIDIA Titan Xp profiling 佐证 batch processing 串行化；UE 因 NVIDIA runtime 闭源无法实机实现，只模拟。[L20-L21, L250-L266, L695-L698, L718-L730]
- **作者报告数字与基线**：TO+UE 相对 state-of-the-art page prefetching baseline 平均 2x，相对 ETC 1.79x；batch size 增 2.27x，batch 数减 51%，平均 batch processing time 减 27%。[L31-L36, L909-L911, L1004-L1012]
- **限制/未覆盖**：UE 未在真实 runtime 实现；TO 可能增加 premature eviction，BFS-TWC/SSSP-TWC 上观察到增加；ETC 的 PE 对 irregular 被禁用；context switch 开销被建模；仅 GPU UVM 生命周期，不含多设备共享翻译。[L467-L501, L1255-L1277, L1386-L1391]
- **对可能创新点最强反证**：若创新点是“通过增加并发行块数来摊薄 GPU page fault batch 成本”或“把 eviction 与 CPU->GPU migration 重叠”，该文已系统化并给出 2x 数量级结果。
- **后续必追引文**：Zheng et al. page prefetching [53]；ETC [29]；Virtual Thread [52]；MacSim [27]；GraphBIG [37]。

### 4. griffin2020
- **文件**：`.../papers/griffin2020.txt`
- **准确题名**：Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems
- **作者**：Trinayan Baruah, Yifan Sun, Ali Tolga Dinçer, Saiful A. Mojumder, José L. Abellán, Yash Ukidave, Ajay Joshi, Norman Rubin, John Kim, David Kaeli
- **年份/来源/DOI**：2020；2020 IEEE International Symposium on High Performance Computer Architecture (HPCA)；DOI 可由本组 idyll 引用给出 `10.1109/HPCA47549.2020.00055`；全文题头本身未列 DOI。[idyll2023 L1122-L1126]
- **机制**：DFTM 延迟 first-touch，按 GPU occupancy 决定是否迁移；CPMS 批处理 CPU-GPU/GPU-GPU 迁移，等待 N_PTW 个 page walk 完成再统一 flush/migrate；DPC 将页分 Mostly Dedicated/Shared/Streaming/Owner-Shifting/Out-of-interest，并决定谁迁到哪；ACUD 只 drain 与迁移页相关的在途访存，避免全 pipeline flush。[L349-L373, L375-L399, L401-L491, L493-L512]
- **复用粒度/身份前提**：4KB 页；4 个 AMD Radeon Instinct MI6 GPU，PCIe-v4；baseline first-touch 后 pin+DCA；DPC 需要 per-SE access counters 和 hyperparameters。[L519-L531, L534-L557, L401-L427]
- **资源共享 vs 映射共享/SVA**：multi-GPU 统一多 GPU 模型；共享/迁移的是页映射与数据位置，不是 IOMMU SVA 权限模型。
- **评价配置**：MGPUSim 1.4.1，4 AMD MI6，36 CU/GPU，PCIe-v4，4KB 页；扩展 Unified Memory 和 Demand-Paging；工作负载 30-64MB。[L536-L557, L549-L557, L577-L589]
- **作者报告数字与基线**：相对 NUMA multi-GPU baseline，geomean 1.37x，最高 2.9x（Matrix Transpose）；PageRank 反而 slowdown，因 sparse random 难利用迁移。[L713-L736]
- **限制/未覆盖**：模拟器评估，内存 footprint 小；DPC 迁移 reactive not predictive；PageRank 退化；hyperparameters 实验调优；未覆盖商业硬件实现；未讨论权限/隔离。
- **对可能创新点最强反证**：若创新点是“multi-GPU 页分类 + 批处理 flush + 选择性 CU drain”，Griffin 已覆盖并给出 1.37x geomean/2.9x peak。[L52-L66, L713-L724]
- **后续必追引文**：CARVE / Young et al. [10]；MGPUSim [12]；Pichai et al. [34]；Power et al. [35]；Zheng et al. [23]；DiDi [39]；UMH [8]；CODA [28]。

### 5. fptmigration2021
- **文件**：`.../papers/fptmigration2021.txt`
- **准确题名**：Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems
- **作者**：Torben Kalkhof, Andreas Koch
- **年份/来源/DOI**：全文题头未给出会议/期刊/年份；文件 stem 为 `fptmigration2021`，但不能据此当正式发表元数据。JSON 按无证据填 null/uncertain。DOI 无。
- **机制**：host CPU + PCIe-attached FPGA + on-device memory；on-FPGA IOMMU 做虚拟->物理翻译并触发 page fault；host device driver 用 Linux HMM API 迁移页到 device private pages，管理 IOMMU TLB；支持 on-demand page migration 与 user-managed migration；PCIe ATS/私有页表作为未来自主 PTW 方向。[L10-L26, L121-L137, L171-L192, L242-L250, L317-L333, L569-L579]
- **复用粒度/身份前提**：4KB 页；HMM API，device private struct page；L1 TLB 32 entry，L2 TLB 1024 entry，另 8-entry variable-length TLB 支持最多 4096 连续页；驱动用 PTW 判断页是否已在 device memory。[L278-L293, L317-L333]
- **资源共享 vs 映射共享/SVA**：CPU 与 FPGA 共享虚拟地址空间，SVM 下 accelerator 也用 virtual address；物理页迁移让数据靠近计算单元。这是 SVM+物理页迁移，不是多设备权限隔离。
- **评价配置**：VC709 FPGA，PCIe 3.0 x8，AMD Athlon X4 845，16GB host；运行 ArraySum、MemCopy、Sobel；比较 TaPaSCo baseline、ODPM、UMPM。[L272-L291, L335-L342]
- **作者报告数字与基线**：大 buffer 下 ODPM/UMPM 可接近甚至超过 TaPaSCo，取决于数据复用；UMPM 在 32KB 时约 1.1x TaPaSCo runtime；128 次 1MB copy 时 UMPM 比 TaPaSCo 快约 15%；大结构 TLB eviction 可导致 70% 更高 runtime。[L367-L383, L389-L400, L203-L205]
- **限制/未覆盖**：device-to-host back-migration 逐页，driver 需逐页检查在途访问；HMM 不支持 huge pages；TLB eviction 昂贵因为 device IOMMU 不能自主 PTW，必须问 driver；未来可用 PCIe ATS 或 device 私有页表；Sobel HLS core 因短 burst/低预取导致 SVM 慢一倍以上。[L529-L535, L546-L567, L569-L579]
- **对可能创新点最强反证**：若创新点是“PCIe FPGA 上 on-FPGA IOMMU + HMM 页迁移 + on-demand/UMPM”，该文已经做了硬件原型和实测量。
- **后续必追引文**：Vogel et al. configurable IOMMU [9]；Pass A Pointer [7]；Linux HMM [25]；Coyote [36]；CAPI [13]；OpenCAPI/CXL/CCIX [19-21]。

### 6. site2017
- **文件**：`.../papers/site2017.txt`
- **准确题名**：Avoiding TLB Shootdowns through Self-invalidating TLB Entries
- **作者**：Amro Awad, Arkaprava Basu, Sergey Blagodurov, Yan Solihin, Gabriel H. Loh
- **年份/来源/DOI**：2017；PACT 2017。site 全文题头未列会议；本组 latr2018 参考文献给出 “PACT, pages 273–287, Portland, OR, September 2017”。DOI 无。证据：latr2018 L960-L964。
- **机制**：每个 TLB entry 带 expiration/lease，过期即视为 invalid；OS 维护 ETT（Expiration Time Table）记录每个 PTE 最新过期时间；若 OS 改 PTE 时 ETT 显示 lease 已过期，则跳过 shootdown；硬件 PTW 计算 expiration，动态 lease 调整减少额外 page walk 和 shootdown。[L16-L22, L643-L653, L661-L672]
- **复用粒度/身份前提**：PTE 粒度；lease 单位用主存访问计数作为逻辑时钟；硬件 TLB entry 加 32-bit expiration；ETT 复用页表非叶节点。[L494-L520, L600-L620, L643-L653]
- **资源共享 vs 映射共享/SVA**：同系统 TLB 与页表一致性，面向异构内存 page migration，不涉及设备共享地址翻译。
- **评价配置**：PIN/trace-based simulation，8 核，L1/L2 TLB，DRAM+PCM 1:8，migration threshold 10，shootdown 20K/5K cycles。[L681-L737]
- **作者报告数字与基线**：动态 lease 相对无 self-invalidation baseline 平均执行时间降 45.5%，避免 65.2% shootdown；即使 shootdown latency 降到 10K cycles 仍平均降 34.2%。[L906-L915, L1007-L1015, L1482-L1483]
- **限制/未覆盖**：逻辑时钟 scalability 到多 socket 受限，文中提出 hierarchical clock 为 future work；timestamp overflow 需 flush TLB/ETT；短 lease 增加 page walk；RSBench 上动态算法不如最佳静态 lease；仿真评估。[L1568-L1610, L1640-L1656]
- **对可能创新点最强反证**：若创新点是“用过期/租约让 TLB entry 自失效从而避免 shootdown”，SITE 已系统化设计、ETT、硬件 PTW 修改和动态 lease。
- **后续必追引文**：UNITD [40]；DiDi [51]；Oskin & Loh [37]；异构内存管理 [7,38,42]；动态 self-invalidation 缓存工作 [27,43,45]。

### 7. hatric2017
- **文件**：`.../papers/hatric2017.txt`
- **准确题名**：Hardware Translation Coherence for Virtualized Systems
- **作者**：Zi Yan, Guilherme Cox, Ján Veselý, Abhishek Bhattacharjee
- **年份/来源/DOI**：本组全文为 arXiv:1701.07517v2 [cs.AR] 15 Feb 2017，题头无会议。正式版由本组 idyll 引用给出：ISCA 2017, pages 430–443，DOI `10.1145/3079856.3080211`。注意：本组提供的是 arXiv v2，不是正式版全文。证据：hatric2017 L6；idyll2023 L1269-L1272。
- **机制**：给 TLB/MMU cache/nTLB entry 加 co-tag，存该翻译对应 PTE 的 system physical address；co-tag 暴露给 cache coherence protocol，当 hypervisor 修改 nested page table，cache coherence 检测 PTE cache line 修改，按 co-tag 精确失效对应 translation structure；不靠 IPI/VM exit/invlpg 全刷。[L116-L123, L244-L256, L354-L363, L395-L431]
- **复用粒度/身份前提**：co-tag 使用 system physical address 的低位子集（2B 用 bits 19-3），按 cache line 粒度失效，因此可能 false invalidation；目录加 gPT/nPT 位识别 guest/nested page table；适用于 MESI/MOESI/MESIF。[L475-L479, L383-L393, L421-L431, L566-L570]
- **资源共享 vs 映射共享/SVA**：虚拟化下 guest page table + nested page table；共享的是翻译缓存一致性，不是设备共享资源。
- **评价配置**：修改 KVM 做 die-stacked DRAM paging；cycle-accurate simulation 32-CPU Haswell，2GB die-stacked+8GB off-chip，KVM 主评，Xen 初评。[L251-L256, L562-L565, L602-L617]
- **作者报告数字与基线**：KVM 上最高 30% 性能和 10% 节能，per-CPU area overhead 2%；Xen 上 canneal 21%、data caching 33% 提升。[L22-L30, L1040-L1044]
- **限制/未覆盖**：co-tag 粗粒度导致 spurious invalidation；目录 pseudo-specific；2B co-tag 是性能/能耗折中；部分 workload energy 反超 baseline 1-1.5%；不做 prefetch/update；Xen 结果只初评。[L475-L517, L994-L1014, L899-L907]
- **对可能创新点最强反证**：若创新点是“把翻译失效 piggyback 到 cache coherence、用 co-tag 支持虚拟化 TLB/MMU cache/nTLB”，HATRIC 已明确覆盖 UNITD 未覆盖的虚拟化和 MMU cache/nTLB。
- **后续必追引文**：UNITD [32]；DiDi [33]；KVM die-stacked paging [10]；Xen；coherence directory [61]。

### 8. didi2011
- **文件**：`.../papers/didi2011.txt`
- **准确题名**：DiDi: Mitigating The Performance Impact of TLB Shootdowns Using A Shared TLB Directory
- **作者**：Carlos Villavieja, Vasileios Karakostas, Lluis Vilanova, Yoav Etsion, Alex Ramirez, Avi Mendelson, Nacho Navarro, Adrián Cristal, Osman S. Unsal
- **年份/来源/DOI**：2011；2011 International Conference on Parallel Architectures and Compilation Techniques (PACT)；DOI `10.1109/PACT.2011.65`。证据：L1, L71-L73。
- **机制**：共享 inclusive 二级 TLB directory（DiDi）跟踪每个 translation 在哪些核的 private TLB 中；invalidation 只发给真正持有者，消除 false positive；每个核有 PTLBI buffer，DiDi 可无中断地让远端核失效 TLB entry，并在 LSQ 注入 memory barrier。[L385-L401, L470-L502]
- **复用粒度/身份前提**：per-core TLB + shared DiDi；ASID 标识地址空间；大页 invalidation 要 reset 所有覆盖的 DiDi bits；SMT 需映射 DiDi ASID 到执行上下文。[L405-L437, L448-L468]
- **资源共享 vs 映射共享/SVA**：同系统多核 TLB 一致性；不是设备共享。
- **评价配置**：真实 Intel Xeon E5640 / AMD Opteron 6128 测量 shootdown；TaskSim trace-based 模拟 DiDi；PIN tool 统计 false positive；32–256 核模型。[L188-L214, L215-L219]
- **作者报告数字与基线**：相对 Linux TLB shootdown，DiDi+PTLBI 把 shootdown 开销降低一个数量级；128 核时 TLB shootdown 可占 24% 机器周期，DiDi 后保持可接受。[L46-L48, L355-L357, L601-L605]
- **限制/未覆盖**：centralized DiDi 的 scalability 由后续工作质疑；ASID 位数有限导致 reuse/flush；inclusive 可能导致跨核 eviction；PTLBI 上限约 off-chip 往返；未覆盖设备共享 IOMMU。
- **对可能创新点最强反证**：若创新点是“共享 TLB directory 消除 false positive + 非中断远端 invalidation”，DiDi 是直接先例。
- **后续必追引文**：UNITD [23]；Bhattacharjee shared last-level TLB [8]；TaskSim [22]；Teller [26]；Rashid lazy TLB consistency [21]。

### 9. unitd2010
- **文件**：`.../papers/unitd2010.txt`
- **准确题名**：UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All
- **作者**：Bogdan F. Romanescu, Alvin R. Lebeck, Daniel J. Sorin, Anne Bracy
- **年份/来源/DOI**：2010；HPCA 2010, Bangalore, India, January 2010；用户给定作者主页核对 DOI `10.1109/HPCA.2010.5416643`。全文本身未印 DOI。证据：L1-L2。
- **机制**：把 TLB 纳入现有 cache coherence protocol，像只读 instruction cache 一样参与；只有 Shared/Invalid 两态，用 Valid bit；用 PCAM 按 PTE 物理地址反向查找 TLB entry 并失效；page table walker 在 fill TLB 时提供 last-level PTE 物理地址；本地 store 对 TLB self-snoop。[L17-L29, L259-L271, L325-L336, L368-L379, L414-L425]
- **复用粒度/身份前提**：PTE/cache block 粒度；x86 硬件管理 TLB；PCAM 与 TLB 同 entry 数；2-16 核仿真；支持 MOSI/MOESI、snooping/directory。[L302-L336, L468-L489]
- **资源共享 vs 映射共享/SVA**：同系统翻译一致性；UNITD 自称 virtualization 不影响 UNITD，但 HATRIC 明确声称 UNITD 不能支持虚拟化。这是本组内跨论文矛盾，应分别引用。[unitd2010 L476-L487; hatric2017 L602-L617]
- **评价配置**：真实 32-Xeon 系统测 shootdown latency；Simics+GEMS 仿真 UNITD，2-16 核；COW/unmap microbenchmark + Phoenix/PARSEC。[L186-L204, L501-L523]
- **作者报告数字与基线**：16 核上对频繁改页表 workload 最高 68% speedup；几乎与 ideal zero-latency TLB invalidation 相同；无 shootdown 应用无退化。[L26-L29, L605-L609, L679-L691]
- **限制/未覆盖**：PCAM 大容量时 area/power 可能不可行；未建模 interrupt 投递；HATRIC 认为不支持虚拟化/MMU cache/nTLB；虚拟化支持争议。
- **对可能创新点最强反证**：若创新点是“把 TLB coherence 统一进 cache coherence、用 CAM/PCAM 按 PTE 物理地址失效”，UNITD 是基础先例。
- **后续必追引文**：Teller [37]；Rosenburg [33]；Black [6]；Simics [25]；GEMS [27]；HATRIC 对其虚拟化批评 [hatric2017 L602-L617]。

### 10. latr2018
- **文件**：`.../papers/latr2018.txt`
- **准确题名**：Latr: Lazy Translation Coherence
- **作者**：Mohan Kumar, Steffen Maass, Sanidhya Kashyap, Ján Veselý, Zi Yan, Taesoo Kim, Abhishek Bhattacharjee, Tushar Krishna
- **年份/来源/DOI**：2018；ASPLOS’18, March 24–28, 2018, Williamsburg, VA, USA；DOI `10.1145/3173162.3173198`。证据：L77-L81。
- **机制**：软件 lazy TLB shootdown；per-core Latr states 是 lockless cyclic queue，记录 start/end、mm_struct、CPU bitmask、flags、active；发起核只写状态不发 IPI；各核在 scheduler tick/context switch 扫状态并本地失效 TLB；free 做 lazy reclamation，AutoNUMA 做 lazy page table unmap。[L286-L316, L397-L400, L417-L448, L530-L550]
- **复用粒度/身份前提**：x86 Linux 4.10；调度 tick 默认 1ms，延迟回收 2ms；每核 64 states，满了 fallback IPI；PCID 场景下 context switch 必须失效。[L309-L315, L473-L516, L921-L933]
- **资源共享 vs 映射共享/SVA**：同系统多核 TLB coherence；面向 free/migration，不是设备共享翻译。
- **评价配置**：2-socket 16-core E5-2630 v3 和 8-socket 120-core E7-8870 v2 实机；Linux 4.10 + 1012 LOC；对比 Linux 和 ABIS。[L479-L506, L530-L535]
- **作者报告数字与基线**：munmap() 延迟最多改善 70.8%（16 核）和 66.7%（120 核）；Apache 吞吐较 Linux 最高 +59.9%，较 ABIS +37.9%；PARSEC 平均 +1.5%，canneal 有一处 -1.7% overhead。[L33-L39, L585-L589, L721-L724, L769-L784]
- **限制/未覆盖**：只支持 free/migration；mprotect/CoW/mremap 不可 lazy；states 满时回退 IPI；不支持 transparent huge pages；改变 free 语义，影响 use-after-free 调试；tickless 内核需特殊处理。[L921-L933, L886-L902, L910-L919]
- **对可能创新点最强反证**：若创新点是“软件异步/lazy TLB shootdown，用调度 tick/context switch 批量失效，不要 IPI”，LATR 已系统实现并实机验证。
- **后续必追引文**：ABIS [2]；Barrelfish [10]；DiDi [62]；HATRIC [64]；UNITD [54]；AutoNUMA [20]；CAT [34]。

### 11. granularity2018
- **文件**：`.../papers/granularity2018.txt`
- **准确题名**：A Case for Granularity Aware Page Migration
- **作者**：Jee Ho Ryoo, Lizy K. John, Arkaprava Basu
- **年份/来源/DOI**：2018；ICS ’18: International Conference on Supercomputing, June 12–15, 2018, Beijing；DOI `10.1145/3205289.3208064`。证据：L30-L36。
- **机制**：把迁移粒度定义为连续虚拟内存区间，候选 4KB/64KB/2MB；用 PTE access bits 扫描虚存局部性；move-in 决定增大粒度，move-out 决定减小粒度；一次迁移只需一次 TLB shootdown，粒度越大越摊薄 overhead，但可能浪费 fast memory。[L70-L122, L325-L364, L441-L457]
- **复用粒度/身份前提**：4KB base page；不限 4KB/64KB/2MB，可为 4KB 倍数；OS-directed migration；fast memory 400MB，slow PCM 模拟；AMD IBS 测 FM-hit。[L112-L122, L418-L440]
- **资源共享 vs 映射共享/SVA**：异构内存 fast/slow 间页迁移；不是设备共享翻译。
- **评价配置**：AMD A10-6800K，Linux 3.16.36，NUMA emulation，DRAM-only 系统模拟 PCM；使用 leading-load 性能模型投影 runtime。[L418-L440, L538-L578]
- **作者报告数字与基线**：动态方案相对固定 4KB baseline 最高 36%、平均 11% 提升；在 200MB/800MB fast memory、1:8/1:2 latency 比值下结论稳健。[L27-L28, L112-L122, L921-L936]
- **限制/未覆盖**：PCM 是模拟；runtime 为投影而非全部实测；不依赖大页；动态阈值需要调参；迁移策略本身可替换，但只研究粒度维；未处理设备共享/权限。
- **对可能创新点最强反证**：若创新点是“迁移粒度应动态适配应用空间局部性，且以虚拟连续区间而非物理连续定义”，该文已是直接先例。
- **后续必追引文**：Oskin & Loh die-stacked DRAM [39]；Meswani access counters [35]；Linux batch TLB flush patches [29,30]；Amit ABIS [2]。

### 12. observuvm2026
- **文件**：`.../papers/observuvm2026.txt`
- **准确题名**：Observability-aided GPU Memory Oversubscription
- **作者**：Pratheek B, Khushit Shah, Arkaprava Basu
- **年份/来源/DOI**：全文题头未给正式年份/会议；文件 stem 为 `observuvm2026`，artifact 路径出现 `isca26ae`、`aeisca26.tar.gz`，参考文献含 2026 HPCA，但这些不构成正式发表元数据。JSON 按 `year: null, venue: null, publication_status: uncertain`。artifact DOI 为 `10.5281/zenodo.19428841`，不是论文 DOI。证据：L1023-L1092, L1256-L1258。
- **机制**：把 GPU 访问 DRAM 的 access counters 重用于 HBM observability；从 2MB HBM region 抽样一个 64KB 页迁到 DRAM，GPU 若访问该页则产生 access counter notification，代表该 region 仍活跃；用该信号驱动 eviction/prefetch 策略；ObservUVM 分离 mechanism 与 policy，policy 在 userspace，driver 通过 eBPF/tracepoints 通信。[L73-L99, L373-L410, L445-L467]
- **复用粒度/身份前提**：2MB region + 64KB sample；依赖 GPU 应用空间局部性；默认观察 100 个 region；access counter threshold 为 1；NVIDIA 3090 + UVM driver v525。[L89-L99, L399-L410, L445-L467, L730-L733]
- **资源共享 vs 映射共享/SVA**：单 GPU HBM/DRAM UVM；论文讨论可扩展到 multi-GPU、GH200 CPU-GPU，但当前评估单 GPU。[L930-L945, L977-L993]
- **评价配置**：NVIDIA 3090 24GB + AMD Ryzen 7950X，PCIe 4.0；修改开源 UVM v525，userspace C++，eBPF 通信；14 个应用，30%/50%/70% oversubscription。[L445-L467, L730-L733]
- **作者报告数字与基线**：TM++ 相对 UVM 平均 34%，最高 64%（SPM）；Tournament eviction 平均 20%，FDP 额外 9%，RGP 额外 5%；相对 ACBM 约 20%，相对 EarlyAdaptor 约 20%。[L734-L739, L900-L919]
- **限制/未覆盖**：单 GPU 评估；multi-GPU 需区分 PCIe-to-DRAM 与 peer-to-peer HBM；依赖空间局部性；access counters 只有约 256 个；sampling/通信 overhead 2.4%+1.8%；不同 page/region size 需适配；不提供硬件新机制。[L327-L332, L930-L945, L977-L1003]
- **对可能创新点最强反证**：若创新点是“复用现有 access counters 为 HBM 访问 observability，从而在 userspace 可编程 eviction/prefetch”，ObservUVM 已是直接先例，并给出 34% geomean。
- **后续必追引文**：Forest [39]；EarlyAdaptor [22]；Batch-Aware UVM [31/32]；IDYLL [34]；GRIT [60]；Trans-FW [35]；ACBM UVM driver [45]。

### 13. suv2024
- **文件**：`.../papers/suv2024.txt`
- **准确题名**：SUV: Static analysis guided Unified Virtual Memory
- **作者**：Pratheek B, Guilherme Cox, Jan Vesely, Arkaprava Basu
- **年份/来源/DOI**：2024；全文题头未给会议，但致谢出现 “MICRO 2024 conference”，artifact 路径含 `SUV-MICRO24`；据此可判 MICRO 2024。论文 DOI 未在全文给出；artifact DOI `10.5281/zenodo.13743918`。证据：L109-L110, L1131-L1133, L1163-L1165。
- **机制**：LLVM 两遍：SUV-Kern 分析 CUDA kernel memop、表达式树、循环/分支；SUV-Host 分析 host 分配、kernel launch、迭代变量并注入 metrics 代码；SUV-Mgmt 根据 access density、WSS、iteration span 做 pin/migrate/prefetch；高密度结构 pin 到 HBM，低密度 pin 到 DRAM；Temporal 结构只保留 working set，允许 on-demand migration；Iterative 结构在迭代边界软件预取；Unknown/pointer-chase 用 access counters。[L16-L24, L240-L253, L531-L560, L565-L583]
- **复用粒度/身份前提**：数据结构和部分结构粒度；需要 kernel 源码或 split closed-source library；静态分析需要可推断地址表达式；依赖 LLVM 15、NVIDIA 3090、UVM driver 525、CUDA 11.8。[L106-L110, L280-L301, L1077-L1114]
- **资源共享 vs 映射共享/SVA**：单 GPU UVM 内 HBM/DRAM 数据放置与迁移；不是设备共享翻译。
- **评价配置**：NVIDIA 3090 24GB + AMD Ryzen 7950X，PCIe 4.0；LLVM 15 + UVM driver 525；Polybench/Rodinia/Tango/HecBench 15 应用，15/30/50% oversubscription。[L565-L583, L1034-L1055]
- **作者报告数字与基线**：相对 UVM with TBP，SUV 降低执行时间 45–76%，摘要写 74% 平均；相对 SC 平均 40% 提升、PCIe traffic 少 81%；相对 AC 平均 43% 提升；zero oversubscription 下平均 20% 提升。[L25-L26, L111-L116, L1034-L1076, L1092-L1094]
- **限制/未覆盖**：静态分析对 pointer-chase/indirect/data-dependent 访问失效，只能 fallback access counters；closed-source library 需要额外拆分和 wrapper；静态分析可能高估 access density；仅单 GPU；需要重编译/driver 修改；不处理权限/多设备共享。[L452-L455, L1077-L1114, L119-125]
- **对可能创新点最强反证**：若创新点是“编译器静态分析驱动 UVM pin/migrate/prefetch，并用 access counters 混合处理未知访问”，SUV 已是直接先例，且给出 45–76% 量级。
- **后续必追引文**：SC [14]；TBP [11/12]；AC [8/13]；EarlyAdaptor [2]；ETC [7]；DeepUM [40]；Sentinel [41]；SwapAdvisor [42]；G10 [44]；Pichai [59]。

---

## C) 被证据支持的剩余瓶颈与不能断言的点

### C1. 动态映射/迁移/失效
**可支持：**
- 动态迁移必然引发翻译失效：Griffin、IDYLL、LATR、SITE、HATRIC、DiDi、UNITD 都把“映射变化后翻译缓存失效”当核心问题。[griffin2020 L219-L238; idyll2023 L27-L31; latr2018 L10-L39; site2017 L16-L22; hatric2017 L22-L30; didi2011 L22-L48; unitd2010 L17-L29]
- 动态迁移的 overhead 不只在 copy：TLB shootdown、page walk 争抢、pipeline flush、GPU VM exit、page fault batch 串行化、eviction 串行化都可主导。  
- 已有机制覆盖：批处理 invalidation（IDYLL IRMB、Griffin CPMS）、lazy invalidation（LATR、IDYLL）、self-invalidation（SITE）、hardware coherence（UNITD、HATRIC、DiDi）、选择性 drain（Griffin ACUD）、granularity-aware（granularity2018）。

**不能断言：**
- 不能断言“动态映射/迁移/失效在 2026 前已被完全解决”。本组显示的是特定系统、特定页大小、特定 workload/模拟条件下的缓解。
- 不能断言“远端 PTW 已普遍支持”：FPT 明确说其 device IOMMU 不能自主 PTW，TLB eviction 需问 driver，未来才考虑 PCIe ATS/私有页表。[fptmigration2021 L529-L535, L569-L579]

### C2. 远端 PTW
**可支持：**
- BypassD 让 IOMMU walk 进程页表翻译 VBA->LBA，并依赖 SVA/PASID 找页表。[bypassd2024 L304-L319]
- IDYLL 的瓶颈正是 invalidation 与 demand TLB miss 争抢 page walk queue/cache/threads。[idyll2023 L407-L416]
- FPT 的 FPGA device IOMMU 只能触发 fault，host driver 做 PTW，远端 TLB miss 代价高。[fptmigration2021 L242-L250, L529-L535]
- Griffin IOMMU 有 8 个 page table walker，CPMS 等待 N_PTW/page walk 完成以批处理迁移。[griffin2020 L351-L363, L527]

**不能断言：**
- 不能断言“所有设备/加速器都具有与 CPU 同等自主远端 PTW 能力”。本组仅 BypassD 提出 IOMMU 扩展翻译，FPT 明确未实现自主 PTW。

### C3. 队列与容量
**可支持：**
- BypassD：NVMe queue pair，PASID 绑定队列；fmap cold overhead 随文件大小增长；16GB cold fmap 21197.88us；warm fmap 259.94us。[bypassd2024 L263-L273, L903-L906]
- IDYLL：page walk queue 64 entries、IRMB 32 merged entries × 16 offsets；IRMB 容量变小会频繁 eviction 并降低收益。[idyll2023 L237-L238, L520-L594, L874-L885]
- Batch-Aware UVM：fault buffer 1024 entries，GPU runtime batch 串行化；batch size 增 2.27x，batch 数减 51%。[batch_uvm2020 L728-L729, L1129-L1131]
- ObservUVM：只有约 256 个 access counters；region 容量、采样数影响 GMM/SPM。[observuvm2026 L327-L332, L923-L975]
- SITE：ETT 内存开销，动态 lease 需调参；逻辑时钟 overflow 需 TLB flush。[site2017 L600-L620, L1575-L1583]

**不能断言：**
- 不能断言“容量问题已解决”。多数工作是缓解；容量与队列深度仍是设计约束。

### C4. 尾延迟
**可支持：**
- BypassD 在 motivation 中引用 MonetaD 权限数据 miss 可增 I/O latency 8x，并会带来 high tail latency；BypassD 自身未测 tail latency。[bypassd2024 L203-L206]
- Batch-Aware UVM 测 batch processing time 范围 223–553us，median 313us；GPU runtime fault handling 占 46.69%。这是 batch 延迟分布，不是端到端尾延迟。[batch_uvm2020 L263-L266]
- IDYLL 测 demand TLB miss latency、page migration waiting latency；页迁移等待延迟平均减 71%，但未做 p99/p99.9。[idyll2023 L818-L842]
- LATR 明确提到 synchronous TLB shootdown 贡献 datacenter tail latency；它测 munmap 平均延迟和 Apache 吞吐。[latr2018 L114-L117, L610-L628]
- ObservUVM 主要报告平均/几何平均 runtime、page faults、evictions，未见尾延迟专门指标。[observuvm2026 L734-L739]

**不能断言：**
- 不能断言“共享翻译的尾延迟已被这些论文充分表征”。本组大多报平均或聚合指标；BypassD 只把 MonetaD 的高尾延迟作为动机，未测自身尾延迟。

### C5. 权限/身份/共享隔离
**可支持：**
- BypassD 是唯一在本组中把 PASID、DevID、R/W、文件权限、IOMMU 检查、内核 revoke/fallback 串成完整机制的全文。[bypassd2024 L263-L273, L307-L319, L322-L349, L460-L473, L562-L583]
- DiDi 有 ASID，但目标是地址空间一致性，不是设备权限。[didi2011 L405-L437]
- HATRIC 的 co-tag、UNITD 的 PCAM、SITE 的 ETT 主要解决翻译一致性，不处理“哪个设备/进程有权访问哪个映射”。
- FPT 的 SVM 共享 CPU/FPGA 虚拟地址，但未展开多设备权限隔离。

**不能断言：**
- 不能断言“多设备共享地址翻译中的权限/身份匹配已被普遍解决”。本组只有 BypassD 明确做到 host 硬件权限检查；其他工作多是一致性问题。

### C6. 本组全文未覆盖 / 不能推为无人做
- 未覆盖：多设备 IOMMU 共享页表下的 per-device PASID 权限矩阵、远端 PTW 缓存一致性边界、SVA 与设备本地 TLB 的失效协议、跨厂商 IOMMU/ATS 商业支持、CXL/OpenCAPI 下 shared translation 的完整生命周期。
- 不能因本组未覆盖就说“无人做”：例如 Pichai、cuPTW、Avatar 等只在部分参考文献或用户提示中出现，未提供全文，不能据此做机制判断。Pichai 用户已给 Rutgers 正式网页核对 ASPLOS 2014 DOI `10.1145/2541940.2541942`，但本组未提供全文，故未列入 JSON 证据卡。SP-IMPact 只应作为系统干扰近邻，不是 IOMMU 专门缓解方案；本组未含其全文。

---

```json
{
  "papers": [
    {
      "id": "bypassd2024",
      "title": "BypassD: Enabling fast userspace access to shared SSDs",
      "authors": ["Sujay Yadalam", "Chloe Alverti", "Vasileios Karakostas", "Jayneel Gandhi", "Michael Swift"],
      "year": 2024,
      "venue": "ASPLOS '24, Volume 1",
      "doi": "10.1145/3617232.3624854",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement", "simulation"],
      "mechanism": "扩展 IOMMU 翻译文件 VBA 到设备 LBA，并用 PASID/FTE/DevID 做权限检查与共享 SSD 文件直访。",
      "assumptions": ["kernel 与硬件属于 TCB，用户进程和 UserLib 可恶意", "NVMe 设备", "ext4 且无 data journaling", "只支持同步操作", "文件全映射，不做 CoW/partial map"],
      "limitations": ["IOMMU 硬件未商用，VBA 翻译靠模拟", "cold fmap 大文件代价高", "不支持 CoW/btrfs/非阻塞写", "不支持 BypassD 与 kernel 接口并发访问", "不支持跨 VM 文件共享"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt", "location": "L31-L32", "claim": "BypassD 扩展 IOMMU 将文件偏移翻译为设备 LBA。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt", "location": "L263-L273", "claim": "fmap() 创建 FTE 并 attach 到进程页表，队列绑定 PASID。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt", "location": "L460-L473", "claim": "IOMMU 增加 FT bit、比较 DevID 与 requester ID，做 VBA 翻译和权限检查。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt", "location": "L659-L662", "claim": "PCIe/IOMMU 翻译开销通过 UserLib 添加 delay 模拟，最小 550ns。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/bypassd2024.txt", "location": "L903-L906", "claim": "cold fmap 开销表显示 16GB 文件达 21197.88us。"}
      ]
    },
    {
      "id": "idyll2023",
      "title": "IDYLL: Enhancing Page Translation in Multi-GPUs via Light Weight PTE Invalidations",
      "authors": ["Bingyao Li", "Yanan Guo", "Yueqi Wang", "Aamer Jaleel", "Jun Yang", "Xulong Tang"],
      "year": 2023,
      "venue": "MICRO '23",
      "doi": "10.1145/3613424.3614269",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "measurement"],
      "mechanism": "host-side in-PTE directory 记录 GPU 有效映射，IRMB 批处理 PTE invalidation 并 lazy 写回，减少与 demand TLB miss 的 page walk 争抢。",
      "assumptions": ["4KB 页", "默认 4-GPU", "host-side PTE 有 11 个 unused bits", "access counter-based migration 作为 baseline", "UVM driver 集中掌握所有 GPU 翻译"],
      "limitations": ["主要结果来自 MGPUSim 模拟", "unused bits/GPU 数增加时 hash false positive", "仍即时 TLB shootdown", "迁移 reactive not predictive", "大页下收益下降"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt", "location": "L27-L31", "claim": "IDYLL 用 in-PTE directory 减少 invalidation，用批处理降低 invalidation latency。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt", "location": "L502-L510", "claim": "利用 PTE unused bits 记录 GPU 访问信息。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt", "location": "L563-L579", "claim": "IRMB 缓冲 invalidation，L2 TLB 与 IRMB 并行查找。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/idyll2023.txt", "location": "L692-L714", "claim": "相对 baseline 平均 69.9% 提升。"}
      ]
    },
    {
      "id": "batch_uvm2020",
      "title": "Batch-Aware Unified Memory Management in GPUs for Irregular Workloads",
      "authors": ["Hyojong Kim", "Jaewoong Sim", "Prasun Gera", "Ramyad Hadidi", "Hyesoon Kim"],
      "year": 2020,
      "venue": "ASPLOS '20",
      "doi": "10.1145/3373376.3378529",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["simulation", "measurement"],
      "mechanism": "Thread Oversubscription 增加 GPU page fault batch size；Unobtrusive Eviction 预取 eviction 并与 CPU-to-GPU migration 重叠。",
      "assumptions": ["GPU runtime 批量处理 page fault", "64KB page", "20us GPU runtime fault handling", "LRU 替换", "NVIDIA PASCAL 驱动模型"],
      "limitations": ["UE 未在真实 runtime 实现，仅模拟", "TO 可能增加 premature eviction", "主要面向 irregular graph workloads", "不含多设备共享翻译"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt", "location": "L16-L36", "claim": "批量处理 page fault 引入串行化；TO+UE 平均 2x over state-of-the-art prefetching。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt", "location": "L267-L275", "claim": "增加 batch size 可摊薄 GPU runtime fault handling time。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt", "location": "L535-L551", "claim": "Unobtrusive Eviction 预取一页 eviction 后重叠双向迁移。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/batch_uvm2020.txt", "location": "L695-L698", "claim": "UE 因 NVIDIA runtime 闭源无法真实实现，改用模拟器。"}
      ]
    },
    {
      "id": "griffin2020",
      "title": "Griffin: Hardware-Software Support for Efficient Page Migration in Multi-GPU Systems",
      "authors": ["Trinayan Baruah", "Yifan Sun", "Ali Tolga Dinçer", "Saiful A. Mojumder", "José L. Abellán", "Yash Ukidave", "Ajay Joshi", "Norman Rubin", "John Kim", "David Kaeli"],
      "year": 2020,
      "venue": "2020 IEEE International Symposium on High Performance Computer Architecture (HPCA)",
      "doi": "10.1109/HPCA47549.2020.00055",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "DFTM 均衡页分布，CPMS 批处理迁移，DPC 分类页，ACUD 选择性 drain CU 以减少 pipeline flush。",
      "assumptions": ["4KB 页", "4 个 AMD MI6 GPU", "PCIe-v4", "baseline 为 first-touch pin + DCA", "DPC 需要 per-SE access counters 和超参数"],
      "limitations": ["MGPUSim 模拟，footprint 30-64MB", "PageRank 出现 slowdown", "迁移 reactive not predictive", "未覆盖权限隔离"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt", "location": "L52-L66", "claim": "Griffin 是 hardware-software 方案，最高 2.9x speedup。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt", "location": "L349-L373", "claim": "DFTM 延迟 first-touch migration 以均衡页分布。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt", "location": "L401-L491", "claim": "DPC 将页分为五类并决定是否/迁往何处。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/griffin2020.txt", "location": "L493-L512", "claim": "ACUD 选择性 drain CU，避免全 pipeline flush。"}
      ]
    },
    {
      "id": "fptmigration2021",
      "title": "Efficient Physical Page Migrations in Shared Virtual Memory Reconfigurable Computing Systems",
      "authors": ["Torben Kalkhof", "Andreas Koch"],
      "year": null,
      "venue": null,
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "core",
      "evaluation_type": ["hardware_prototype", "measurement"],
      "mechanism": "on-FPGA IOMMU 做 SVM 地址翻译和 page fault，host driver 用 Linux HMM 在 host/device memory 间迁移物理页，支持 ODPM 与 UMPM。",
      "assumptions": ["PCIe-attached FPGA VC709", "Linux HMM API", "4KB 页", "device private struct page", "L1/L2 TLB 与变长 TLB"],
      "limitations": ["全文题头无出版会议/年份", "HMM 不支持 huge pages", "device IOMMU 不能自主 PTW，TLB eviction 昂贵", "back-migration 逐页", "大 buffer scatter-gather DMA 降低效率"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt", "location": "L10-L26", "claim": "SVM 允许 CPU/FPGA 共享虚拟地址，并支持物理页迁移。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt", "location": "L121-L137", "claim": "on-FPGA IOMMU 做翻译并触发 page fault；driver 迁移页、管理 TLB。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt", "location": "L317-L333", "claim": "第三级 8-entry TLB 支持最多 4096 连续页映射。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt", "location": "L529-L535", "claim": "HMM 不支持 huge pages，TLB eviction 昂贵。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/fptmigration2021.txt", "location": "L569-L579", "claim": "未来可用 PCIe ATS 或 device 私有页表让 IOMMU 自主 PTW。"}
      ]
    },
    {
      "id": "site2017",
      "title": "Avoiding TLB Shootdowns through Self-invalidating TLB Entries",
      "authors": ["Amro Awad", "Arkaprava Basu", "Sergey Blagodurov", "Yan Solihin", "Gabriel H. Loh"],
      "year": 2017,
      "venue": "PACT 2017",
      "doi": null,
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation"],
      "mechanism": "TLB entry 带 expiration/lease，ETT 记录 PTE 最新过期时间；若 PTE 修改时 lease 已过期则免 shootdown。",
      "assumptions": ["PTE 粒度", "逻辑时钟用主存访问计数", "硬件 TLB entry 加 32-bit expiration", "ETT 复用页表非叶节点", "OS 修改 invalidation routine"],
      "limitations": ["逻辑时钟 scalability 受限", "timestamp overflow 需 flush TLB/ETT", "短 lease 增加 page walk", "RSBench 上动态算法不如最佳静态 lease", "仿真评估"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/site2017.txt", "location": "L16-L22", "claim": "SITE 用 expiration time 判断 TLB entry 是否仍需 shootdown。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/site2017.txt", "location": "L643-L653", "claim": "TLB entry 扩展 expiration，并在 lookup 时与当前逻辑时间比较。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/site2017.txt", "location": "L906-L915", "claim": "动态 lease 相对 baseline 平均降 45.5% 执行时间。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/site2017.txt", "location": "L1575-L1583", "claim": "timestamp overflow 时 flush TLB/ETT。"}
      ]
    },
    {
      "id": "hatric2017",
      "title": "Hardware Translation Coherence for Virtualized Systems",
      "authors": ["Zi Yan", "Guilherme Cox", "Ján Veselý", "Abhishek Bhattacharjee"],
      "year": 2017,
      "venue": "ISCA 2017",
      "doi": "10.1145/3079856.3080211",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["simulation", "measurement"],
      "mechanism": "给 TLB/MMU cache/nTLB 加 co-tag，存 PTE 的系统物理地址，把翻译一致性 piggyback 到 cache coherence，支持虚拟化 nested page table。",
      "assumptions": ["x86-64", "2B co-tag", "MESI/MOESI/MESIF", "translation structure 只读", "KVM 主评/Xen 初评"],
      "limitations": ["co-tag 粗粒度导致 false invalidation", "目录 pseudo-specific 造成 spurious coherence", "部分 workload energy 稍增", "未做 prefetch/update", "本组提供的全文是 arXiv v2"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt", "location": "L22-L30", "claim": "HATRIC 提供硬件 translation invalidation/coherence，KVM 最高 30%，Xen 最高 33%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt", "location": "L116-L123", "claim": "co-tag 存 PTE 的系统物理地址，使 hypervisor 无需 GVP 即可精确定位翻译结构。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt", "location": "L395-L431", "claim": "把 TLB/MMU cache/nTLB 作为只读结构集成进 cache coherence。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt", "location": "L602-L617", "claim": "HATRIC 可将 UNITD++ 扩展到 MMU cache/nTLB，并称 UNITD 不支持虚拟化。"}
      ]
    },
    {
      "id": "didi2011",
      "title": "DiDi: Mitigating The Performance Impact of TLB Shootdowns Using A Shared TLB Directory",
      "authors": ["Carlos Villavieja", "Vasileios Karakostas", "Lluis Vilanova", "Yoav Etsion", "Alex Ramirez", "Avi Mendelson", "Nacho Navarro", "Adrián Cristal", "Osman S. Unsal"],
      "year": 2011,
      "venue": "PACT 2011",
      "doi": "10.1109/PACT.2011.65",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement", "simulation"],
      "mechanism": "共享 inclusive 二级 TLB directory 跟踪翻译持有者，PTLBI 非中断式远端 invalidation，避免 IPI 和 false positive。",
      "assumptions": ["per-core TLB + shared DiDi", "inclusive directory", "ASID 标识地址空间", "PTLBI 可注入 LSQ memory barrier", "32-256 核模型"],
      "limitations": ["centralized DiDi 的 scalability 受质疑", "ASID 位数有限", "大页需 reset 对应 DiDi bits", "PTLBI 上限约 off-chip 往返", "不处理设备共享权限"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/didi2011.txt", "location": "L22-L48", "claim": "DiDi 用共享 TLB directory 和非中断远端 invalidation 把 shootdown 开销降一个数量级。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/didi2011.txt", "location": "L385-L401", "claim": "共享二级 TLB directory 跟踪每个 translation 在哪些核的 TLB 中。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/didi2011.txt", "location": "L470-L502", "claim": "PTLBI buffer 在 LSQ 注入 memory barrier 并失效远端 TLB entry。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/didi2011.txt", "location": "L527-L545", "claim": "Linux 常因 false positive 中断多个核，DiDi 只通知真正持有者。"}
      ]
    },
    {
      "id": "unitd2010",
      "title": "UNified Instruction/Translation/Data (UNITD) Coherence: One Protocol to Rule Them All",
      "authors": ["Bogdan F. Romanescu", "Alvin R. Lebeck", "Daniel J. Sorin", "Anne Bracy"],
      "year": 2010,
      "venue": "HPCA 2010",
      "doi": "10.1109/HPCA.2010.5416643",
      "publication_status": "published",
      "relevance": "foundation",
      "evaluation_type": ["simulation", "measurement"],
      "mechanism": "把 TLB 纳入 cache coherence，PCAM 按 PTE 物理地址反向查找并失效 TLB entry，消除软件 TLB shootdown。",
      "assumptions": ["x86 硬件管理 TLB", "PTE/cache block 粒度", "PCAM 与 TLB 同 entry 数", "MOSI/MOESI snooping/directory", "2-16 核仿真"],
      "limitations": ["大 PCAM 的 area/power 可能不可行", "未建模 interrupt 投递", "HATRIC 称 UNITD 不支持虚拟化/MMU cache/nTLB", "本组全文未印 DOI，DOI 由用户给定核对"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt", "location": "L17-L29", "claim": "UNITD 将 translation coherence 集成进 cache coherence，减少 TLB shootdown penalty 到近零。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt", "location": "L259-L268", "claim": "TLB 像只读 instruction cache 一样参与 coherence，只有 Shared/Invalid。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt", "location": "L368-L379", "claim": "PCAM 按 PTE 物理地址执行 coherence invalidation。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/unitd2010.txt", "location": "L605-L609", "claim": "16 核上最高 68% speedup。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/hatric2017.txt", "location": "L602-L617", "claim": "HATRIC 声称 UNITD 不支持虚拟化，且不覆盖 MMU cache/nTLB。"}
      ]
    },
    {
      "id": "latr2018",
      "title": "Latr: Lazy Translation Coherence",
      "authors": ["Mohan Kumar", "Steffen Maass", "Sanidhya Kashyap", "Ján Veselý", "Zi Yan", "Taesoo Kim", "Abhishek Bhattacharjee", "Tushar Krishna"],
      "year": 2018,
      "venue": "ASPLOS '18",
      "doi": "10.1145/3173162.3173198",
      "publication_status": "published",
      "relevance": "core",
      "evaluation_type": ["measurement"],
      "mechanism": "软件 lazy TLB shootdown：per-core Latr states 记录失效范围，调度 tick/context switch 时各核本地失效，避免 IPI 和 ACK 等待。",
      "assumptions": ["x86 Linux 4.10", "scheduler tick 默认 1ms", "延迟回收 2ms", "每核 64 states，满则 fallback IPI", "PCID 场景下 context switch 仍需失效"],
      "limitations": ["只支持 free/migration", "mprotect/CoW/mremap 不可 lazy", "states 满时回退 IPI", "不支持 transparent huge pages", "改变 free 语义，影响 use-after-free 调试"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/latr2018.txt", "location": "L10-L39", "claim": "Latr 异步 lazy TLB shootdown，Apache 较 Linux 最高 +59.9%，较 ABIS +37.9%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/latr2018.txt", "location": "L286-L316", "claim": "per-core Latr states 记录 shootdown 信息，调度 tick 时 sweep。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/latr2018.txt", "location": "L397-L400", "claim": "延迟两个 scheduler tick 确保所有核已失效后再复用页。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/latr2018.txt", "location": "L921-L933", "claim": "Latr 不适用于 mprotect/CoW/mremap，且不支持 THP。"}
      ]
    },
    {
      "id": "granularity2018",
      "title": "A Case for Granularity Aware Page Migration",
      "authors": ["Jee Ho Ryoo", "Lizy K. John", "Arkaprava Basu"],
      "year": 2018,
      "venue": "ICS '18",
      "doi": "10.1145/3205289.3208064",
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["measurement", "analysis"],
      "mechanism": "动态选择 4KB/64KB/2MB 迁移粒度，以连续虚拟内存区间为单位，用 PTE access bits 推断空间局部性并在 move-in/move-out 调整。",
      "assumptions": ["4KB base page", "OS-directed migration", "fast memory 400MB", "slow PCM 模拟", "AMD IBS 测 FM-hit"],
      "limitations": ["PCM 为模拟", "runtime 为 leading-load 模型投影", "动态阈值需调参", "不处理设备共享/权限", "不依赖 huge pages"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt", "location": "L12-L28", "claim": "迁移粒度是关键维度，动态方案最高 36%、平均 11% over 固定 4KB baseline。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt", "location": "L70-L78", "claim": "迁移粒度定义为一次迁移的连续虚拟内存区间。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt", "location": "L325-L364", "claim": "用 PTE access bits 扫描局部性，动态增大或减小粒度。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/granularity2018.txt", "location": "L418-L440", "claim": "AMD A10-6800K、Linux 3.16.36、NUMA emulation 模拟异构内存。"}
      ]
    },
    {
      "id": "observuvm2026",
      "title": "Observability-aided GPU Memory Oversubscription",
      "authors": ["Pratheek B", "Khushit Shah", "Arkaprava Basu"],
      "year": null,
      "venue": null,
      "doi": null,
      "publication_status": "uncertain",
      "relevance": "nearby",
      "evaluation_type": ["measurement"],
      "mechanism": "复用 access counters：抽样 HBM region 中 64KB 页迁到 DRAM，以 PCIe 访问计数观测区域活跃性，驱动 userspace eviction/prefetch 策略。",
      "assumptions": ["NVIDIA 3090 + UVM driver v525", "2MB region + 64KB sample", "GPU 应用有空间局部性", "access counter threshold 为 1", "默认观察 100 个 region"],
      "limitations": ["单 GPU 评估", "multi-GPU 需区分 PCIe 与 peer-to-peer", "依赖空间局部性", "约 256 个硬件 counters", "sampling/通信有 overhead"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt", "location": "L73-L99", "claim": "复用 access counters 对 HBM-resident region 抽样提供 observability。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt", "location": "L373-L410", "claim": "抽样页迁到 DRAM，GPU 访问触发 access counter notification。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt", "location": "L445-L467", "claim": "ObservUVM 修改 UVM driver，policy 放在 userspace，用 eBPF 通信。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt", "location": "L734-L739", "claim": "TM++ 相对 UVM 平均 34%，最高 64%。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt", "location": "L930-L945", "claim": "多 GPU 扩展仍面临区分 PCIe/peer-to-peer 访问的挑战。"}
      ]
    },
    {
      "id": "suv2024",
      "title": "SUV: Static analysis guided Unified Virtual Memory",
      "authors": ["Pratheek B", "Guilherme Cox", "Jan Vesely", "Arkaprava Basu"],
      "year": 2024,
      "venue": "MICRO 2024",
      "doi": null,
      "publication_status": "published",
      "relevance": "nearby",
      "evaluation_type": ["measurement"],
      "mechanism": "LLVM 静态分析 CUDA kernel/host，计算 access density、working set、iteration span；pin 高密度结构、保留 temporal working set、迭代边界预取，unknown 用 access counters。",
      "assumptions": ["需要 kernel 源码或 split closed-source library", "NVIDIA 3090 + UVM driver 525 + LLVM 15 + CUDA 11.8", "地址表达式可静态推断", "单 GPU UVM"],
      "limitations": ["pointer-chase/data-dependent 访问只能 fallback access counters", "closed-source library 需额外拆分", "可能高估 access density", "仅单 GPU", "需重编译和 driver 修改"],
      "evidence": [
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/suv2024.txt", "location": "L10-L26", "claim": "SUV 用静态分析推断 CUDA 访问语义，平均降执行时间 74% over UVM。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/suv2024.txt", "location": "L240-L253", "claim": "用 access density、working set、iteration span 做 proactive 管理，unknown 才用 access counters。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/suv2024.txt", "location": "L531-L560", "claim": "增强 UVM driver，新增 pin、access counter 控制、no-migrate region 等 API。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/suv2024.txt", "location": "L111-L116", "claim": "相对 UVM with TBP 降低 45–76%，相对 SC/AC 显著领先。"},
        {"file": "iommu-platform/out/shared-translation-literature-20261006/papers/suv2024.txt", "location": "L1077-L1114", "claim": "closed-source kernel 需要拆分 SUV-Host-app/SUV-Host-lib 才能工作。"}
      ]
    }
  ]
}
```