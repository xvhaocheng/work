以下为**独立前作证据与创新边界候选草案**，不是已立项创新，也不声称已验证。引用均为给定文件的本地行号，不编造页码；未获全文的项只作风险提示。当前 B 的最小 POC 已在 §16 给出：PN+B 相对 PN 略好，但加普通页表优先后 PN=79491、B=79486，仅差 5 cycle（`docs/composition-history-isolation-20261007.md:314-327`）。因此本草案不把 B 换名；下文的候选必须给出**B 没有的新决定**：完成释放依赖、可排空准备、源/目的配对窗口、有用 hit 预算。也不因此全球否定 PN 或 PTE phase。

## 1. 导航表：候选精确决定与边界

| 候选 | 精确决定 | 最接近前作证据（file:line/section） | 已覆盖思想 | 尚未覆盖但未确立的细节 | 最强普通 baseline | 最小价值判别 | 不可立题条件 |
|---|---|---|---|---|---|---|---|
| C1 完成释放依赖调度 | 在合法调度点（未声明 VALID 的物理 offer 准入；或接口允许的已接收返回重排）选择“其完成会释放阻塞另一请求的资源”的请求：owner tag、跨 bank AW 门控、W/read buffer credit、PTW 状态等。无释放候选则回退 PTE 优先/FIFO。不是 PTE 总优先或 SJF 长度。 | MASK `mask2018.txt:888-915`；PTW 调度 `walk_scheduling2018.txt:291-309,436-462`；DWS `walk_stealing2021.txt:355-382,468-553`；Criticality `criticality2013.txt:20-27,260-292`；AXI-REALM `axi-realm2025.txt:320-340,430-443,530-535`；ABU `abu2019.txt:384-427,819-848`；当前代码边界 `composition-history-isolation-20261007.md:170,197-198` | PTE 优先、数据/翻译协调、SJF、同指令批处理、walker stealing、等待线程关键性、full-W write buffer、budget/period、当前 AW 优先。 | 事务级“释放集”：DMA 读返回、写 B/W 完成、PTE 返回分别释放什么；跨 bank owner/W buffer/credit 的依赖；不是全局 PTE 优先。 | MASK Golden/Silver/Normal + 当前 AW 优先 + fullbuffer certificate + PTW-first + DWS；经典 DRAM 调度 PAR-BS/TCM/BLISS/SMS。 | 造出 H1 状态：PTE-first 与 release-first 选择不同；测 owner 等待、被堵 AW、V 完成时间、A 有效速度。若普通 AW 优先/fullbuffer 已作同选，或无合法选择点，停止。 | 只靠停 N、全局少发、严格 FIFO；需取消已 VALID 或抢占已接收 burst；MIG 不可重排且 admission-only 无状态；普通 baseline 同结果。 |
| C2 可排空准备队列 | 翻译完成后不直接 offer 物理 AR/AW；进入准备队列，只为“当前容量/owner 释放可满足、可排空”的请求发 offer；队头不可排空时跳过到下一可排空未 VALID 请求。已 VALID/已接收保持顺序。 | AXI-REALM write buffer `axi-realm2025.txt:320-340`、stage budget `:505-535`；ABU `abu2019.txt:384-427`；iDMA RAW coupler `composition-history-isolation-20261007.md:167,170`；trans_ctl fullbuffer `:198`；翻译后物理 AR/AW 等待 `:200`；MASK 数据协调 `mask2018.txt:888-915` | 写缓冲满 W 才转发、credit/budget、translation-aware 数据服务、fullbuffer certificate、读准入。 | 读侧容量+依赖的 prepare 队列；翻译状态与物理 offer 分离；队头跳过但不取消；不是 B 的逐 PTE 额度。 | 当前 fullbuffer certificate + AXI-REALM + ABU + iDMA RAW + B。 | 造 H2：q1 受容量/owner 限制，q2 可排空；比较 FIFO/PTE-first 与 skip。测队头阻塞周期、V 完成、A 速度、正确排空。无 q2 或普通 full buffer 已覆盖则停。 | 只是 B 换名；无后排可排空请求；只靠延迟 q1、总完成更慢；需取消 VALID。 |
| C3 源/目的页配对翻译小窗口 | 对 copy 作业把 src/dst 页作为一对；只准备完成当前对/小窗口所需翻译与读数据容量；不做无界 prefetch/cachepin；在不取消已 VALID 前提下优先补齐配对缺失侧。 | MMU-aware DMA：INDEX `out/shared-translation-literature-20261006/INDEX.md:319,353`，`docs/composition-history-isolation-20261007.md:210`；rIOMMU `INDEX.md:262`；fullbuffer certificate `composition-history-isolation-20261007.md:198`；AXI-REALM 碎片化 `axi-realm2025.txt:33-36,112-116` | MMU-aware DMA 的 TLB 预取、受控 TLB 管理、burst 失败重放；ring buffer；full buffer；fragmentation。 | 配对完成依赖 + 读容量申请；小窗口与真实可排空容量绑定；不是无界预取。 | MMU-aware DMA + fullbuffer certificate + AXI-REALM fragmentation + 当前 P/N。 | 同 hit/miss 下比较无界预取 vs 配对窗口；测 prepared-but-unusable 读、缓冲/credit、V/A。MMU-aware 或 fullbuffer 已同效果则停。 | 只适用对齐 full buffer；无配对信息；无容量耦合；真实负载本来就无界预取。 |
| C4 P/N 联合容量-服务预算按“有用 hit” | P ways 与 N fast 接受/服务预算按“有用 hit”更新：hit 避免主 PTW 等待，且其物理 offer 在当前可排空窗口可发出；不是全部 hit。只改未来 fill/预算，不撤 VALID。 | MASK TLB-Fill Tokens `mask2018.txt:828-846`；DWS TLB share `walk_stealing2021.txt:857-876`；GPU 分区+调度 `composition-history-isolation-20261007.md:148`；PLRU 隔离 `:9,30` | token、miss-rate 调整、walker share→TLB share、静态分区、局部 PLRU。 | hit 有用性连接到下游可排空性；P/N 联合容量/服务预算；不是全部 hit 统计。 | MASK tokens + DWS + 当前 PLRU 隔离 + all-hit 预算。 | 造两个 hit 数近似相同但有用性相反的状态；比较 all-hit vs useful-hit 预算。若普通 token/miss-rate 同选，或需偷看未来，停止。 | 无 blocked-but-hit 状态；有用性不可观测；只改计数器。 |

## 2. 两个合法硬件状态：候选与普通方法的不同选择

**状态 H1：返回/响应调度，合法选择点。**  
bank0 有未完成共享 owner（当前 `rv_iommu_banked_ctl.sv:246–278` 的单 owner/读准入，`composition-history-isolation-20261007.md:197`）。其释放条件在实现中被标记为某条已接收返回/响应，例如写事务的 B/W 完成或某数据读返回；bank1 有 pending AW，但受 `axi_demux_simple.sv:175–177` 跨 bank AW 门控（`:198`）。共享返回队列中另有一条已接收页表读返回，它只会推进 PTW，不释放 owner/信用。  
- 候选 C1：若返回路径合法支持跨 ID 重排，选择释放 owner 的那条返回/响应；普通 MASK/Priority/PTE-first 选择页表返回。  
- 合法性：不取消任何已 VALID 请求，不假设“AR 接收整 burst 后不可抢占”；若真实 MIG 不能重排已接收返回，则只保留“下一笔未声明 VALID 的 offer 准入”部分，不声称能在 MIG 内部重排。不偷看未来 PTE；只看请求角色、owner/credit 和已返回状态。

**状态 H2：未发 VALID 的 offer 准入。**  
准备队列中 q1、q2 翻译均已完成，尚未在共享互连声明 VALID。q1 是 256 拍读，但其所需返回容量/owner 释放当前不满足；q2 是短读或页表读，当前可排空。  
- 候选 C2：发 q2，q1 留在准备队列；FIFO/SJF/PTE-first 发 q1。  
- 合法性：只改变未 VALID 请求的 offer 顺序，不撤销、不打断、不取消；若 q1 已 VALID，候选不能取消，只能继续或等其完成。  
这两个状态说明：候选与普通方法的不同必须来自“释放依赖/可排空性”，而不是 PTE 总优先或长度排序。

## 3. 与强前作和普通组合的精确边界

- **MASK**：已有 PTE 优先、Golden/Silver/Normal、按并发 walk/受阻 warp 分配数据配额（`mask2018.txt:888-915`）；token 与 L2 bypass 也已覆盖（`:828-846`）。C1/C2 必须证明新增的是事务级 release/drainability，不是“翻译感知”换名。
- **PTW job batching / SJF**：`walk_scheduling2018.txt:291-309,436-462` 已按同一 SIMD 指令所需工作量调度并批处理。C1 若只按长度/优先级，就是旧思路。
- **DWS**：已有 walker 分区+stealing，且已提示 MASK+DWS 可能无显著叠加（`walk_stealing2021.txt:857-876`）。C4 不能只做 walker/TLB 份额。
- **Criticality / BIS**：已有按等待线程和同步瓶颈识别关键工作（`criticality2013.txt:20-27,260-292`）。C1 若只用“多少请求在等”，等价于关键性；必须落到 IOMMU/DMA 的资源释放。
- **AXI-REALM / ABU**：AXI-REALM 已有 burst fragmentation、period budget、write buffer 满 W 才转发、stage budget（`axi-realm2025.txt:320-340,430-443,530-535`）；ABU 已有 budget/period 和 response-time 分析（`abu2019.txt:384-427,819-848`）。C2 不能只做 budget/period 或写缓冲满 W。
- **经典 DRAM 调度**：PAR-BS、TCM、BLISS、Staged Memory Scheduling 已覆盖批处理、公平、并行度、异构服务顺序；本地仅见 MASK 参考文献（`mask2018.txt:1376-1378,1440-1442,1483-1485`），全文未在本批核读。它们是 C1/C2 的强普通 baseline，不是“已证明覆盖本平台”。
- **MMU-aware DMA**：给定 123 项索引确有 `socdma2018` 全文（`INDEX.md:319,353`），文档也承认其有预取、受控 TLB 管理与 burst 失败重放（`composition-history-isolation-20261007.md:210`）。C3 必须先核其配对/容量细节，不能因“没看到”称新。
- **RISC-V nonleaf cache**：规范已允许 nonleaf 页表项缓存（`composition-history-isolation-20261007.md:210,235`）。单独加小页表缓存是普通基线，不是 C4 的贡献。

## 4. 约束分层：P/PTW/DMA 不能混

- **P**：16-way IOTLB 2/14 分区、局部 PLRU、容量/miss 统计；隔离后 P/PN 的额外 miss 差消失，但 PN 仍慢约 12.7%（`composition-history-isolation-20261007.md:3,29-32,40`）。
- **N**：主 PTW 等待时另一 bank cached-hit 旁路；已有读写角色、accept/touch（`:199`）。N 的 fast 数不等于有效进度；§8 中 PN fast=62 但 DS 返回等待比 P 增加 5505（`:86-101`）。
- **PTW**：单 PTW不能中途抢占；PTE 返回后才判叶（`:200-201`）。PTE phase/B 只在特定状态有差异，不能全球推广。
- **DMA**：读数据 buffer、写 owner、W/AW 门控、fullbuffer certificate、RAW coupling 关闭（`:170,198`）。候选必须声明改的是“offer 准入”还是“返回调度”，不能把输入 owner 当下游 owner。
- **共享互连/DDR**：AXI4 不交错 W，合并写必须保持地址顺序（`:172`）；MIG 返回 FIFO/重排能力未证。C1/C2 不得假设可偷看未来或任意抢占。

## 5. 低成本概念验证建议

不求完整论文级证据即可做最小判别；但每个候选先做一个短条件，并保留 P/N/PN 同条件数据。  
- C1：在现有真实 DMA 模型给请求加“release_set/role”事件，比较 PTE-first 与 release-first；若接口不允许返回重排，只做未 VALID offer 准入。  
- C2：造 q1 容量不足、q2 可排空的两请求准备队列；比较 FIFO/PTE-first 与 skip。  
- C3：同 hit/miss 下比较无界预取与配对窗口；记录 prepared-but-unusable 读与 buffer/credit。  
- C4：造 hit 数近似相同、有用性相反的状态；比较 all-hit 与 useful-hit 预算。  
判定看数据正确、排空、合法握手，以及 V 完成时间、A 有效速度、资源/请求代价。若普通 baseline 同选、收益只来自停 N/少发、或需取消 VALID，停止该候选。这些差异若成立，可能只是 incremental contribution；不要求全新硬件原语，也不因未见同式就称创新。

## 6. Root 需新搜的 primary paper/official source（3–6 项优先）

1. **NB-Walker: Non-blocking Page Table Walker to Enhance Address Translation in NUMA GPUs**，MICRO 2026。关键词：non-blocking PTW、NUMA GPU、page-walk queue、MLP、MSHR。全文缺口：只有官方程序/artifact，未获论文全文；直接影响 C1/C4 是否已有非阻塞 walker 级 release/dependency 队列。
2. **Don’t Get Stuck in Traffic: A Case for Contention-Aware Translation Offloading**，MICRO 2026。关键词：contention-aware translation offloading、CXL tiered memory、PTW offload、hash leaf PTE。全文缺口：作者页 404，仅索引/题录；影响 C1/C4。
3. **SPMC: Speculative Multicast for GPU Address Translation Sharing**，MICRO 2026。关键词：translation sharing、multicast、TLB/IOMMU、speculative。全文缺口：仅官方题录；影响 C4 的共享容量/服务预算。
4. **Reducing Page Faults via Invalidation-Based Mapping Propagation in Multi-GPU Systems**（ShadowUpdate），ISCA 2026。关键词：invalidation-based mapping propagation、in-flight migration tracker、multi-GPU、page faults。全文缺口：仅机构摘要；影响 C1 的“完成释放/在途依赖”语义。
5. **经典 DRAM 调度全文缺口组**：PAR-BS *Parallelism-Aware Batch Scheduling: Enhancing both performance and fairness of shared DRAM systems*，ISCA 2008；TCM *Thread Cluster Memory Scheduling: Exploiting Differences in Memory Access Behavior*，MICRO 2010；BLISS *BLISS: Balancing Performance, Fairness and Complexity in Memory Access Scheduling*，IEEE TPDS 2016；Staged Memory Scheduling (SMS)，ISCA 2012。关键词：DRAM scheduling、batching、fairness、parallelism、heterogeneous batching。全文缺口：本批仅见间接引用（`mask2018.txt:1376-1378,1440-1442,1483-1485`）；需核它们对“完成依赖/可排空队列”的覆盖。
6. **Make Every Batch Count: Fault Entry Merging for Efficient Batching in Unified Virtual Memory**，MICRO 2026。关键词：UVM batching、fault entry merging、fault queue、queue blocking。全文缺口：仅题录；影响 C2 的批处理/队头阻塞近邻。

以上候选均未通过创新验收；不将“未搜到同式”当创新，也不将现有前作泛化为“所有都已有”。若 root 获 NB-Walker/CATO 等全文后发现普通方法已作同决定，应撤回相应候选；若发现小增量效果或适用边界，则保留为 incremental contribution。
