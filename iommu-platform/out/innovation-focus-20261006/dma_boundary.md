以下只基于你提供的三篇论文文本、两份项目文档和依赖诊断做定向证据审查。未联网、未改代码、未做实验；项目文档只作为平台事实，不作为创新证据；模型日志不被当作实板根因。Codex 保留架构决策与验收。

## 0. 结论先行

**按你给出的判据，“有限缓冲 DMA 中翻译进度与数据进度耦合”目前不留下一个可独立立题的创新候选。**

理由不是“没人做过”，而是现有最强直接前作已经把最核心的解耦动作做了：socdma2018 的 hybrid IOMMU + retirement buffer 正是“TLB miss 时不占数据缓冲，改由 master 记录失败的 burst，等翻译完成后按原请求顺序重新发起”。它明确说自己提供“without additional buffers”的 DMA SVM 支持，并把“avoid stalling the entire DMA on a TLB miss while maintaining memory order guarantees”列为未来工作（`socdma2018.txt:84-100`, `:393-417`, `:518-521`, `:545-569`）。

剩下的“有限缓冲耦合”若只落到以下动作，均不合格：
- 写数据保留到 BRESP 或翻译完成后再释放：普通 AXI BRESP/skid/backpressure；
- 按 burst 记录 miss、等翻译后重发：socdma2018 retirement buffer；
- 把翻译返回与数据返回分 VC / 分 bank / 分 ID：通用 VC/ID/queue 隔离；
- 翻译先行、数据后发：通用 split transaction / prefetch；
- 限制 miss-pending 数据占用缓冲：通用 credit/缓冲管理。

NeuMMU 还明确警告过“把 DMA 数据/翻译请求发得不那么 bursty”会降低 MLP 和带宽利用（`neummu2019-author.txt:537-574`）；riscvsva2025 则已量化 RISC-V IOMMU 的 IOTLB miss/PTW 开销及 LLC 缓解（`riscvsva2025.txt:30-33`, `:224-230`, `:263-268`）。因此，若没有新的、非通用组合的决策，结论就是：**无创新；降级为规格/RTL 核查问题。**

---

## 1. 最近前作机制矩阵

| 维度 | socdma2018 / ICCD 2018 | NeuMMU / 2019 | riscvsva2025 / 2025 |
|---|---|---|---|
| 平台关系 | hybrid IOMMU + standard DMA；TLB 由 PMCA 上的软件完全管理 | NPU MMU；SPM-centric NPU；DMA 产生 translation burst | CVA6 + RISC-V IOMMU + Snitch DMA；Linux driver 建映射 |
| missing read/write | IOMMU 在 TLB miss 时存 metadata，回 error，drop transaction；DMA 记录哪些 burst miss，等 miss 处理完后 reissue；读写均按 burst 级处理。AXI burst 不跨页，因此每个 burst 在到达 IOMMU 的瞬间只需一个 TLB entry（`socdma2018.txt:196-200`, `:393-401`） | TLB miss 先查 PTS：PTS hit 则 merge 到 PRMB，等已有 translation 返回；PTS miss 则分配 PTW + PRMB。若所有 PTW 和 PRMB merge slot 都满，后续 translation request 被 block，直到 translation bandwidth 可用（`neummu2019-author.txt:588-618`） | 只测量：IOVA translation 在 IOTLB miss 时最多需三次顺序访存；DMA transfer 大于 page 时被拆成多个 burst，每个造成 IOTLB miss 的 burst 都可能降低有效带宽（`riscvsva2025.txt:30-33`, `:224-230`） |
| 错误响应/重发 | AXI error response 后 retirement buffer 把 burst 标 failed；DMA control unit 停止发新 burst，等所有 in-flight 完成，再把 reissuable failed burst 按原请求顺序重发，之后恢复常规操作（`socdma2018.txt:403-417`） | 提供文本不描述 AXI error/retry；它处理的是 translation 的 merge/backpressure，不是 DMA 数据 burst 的 error/retry | 提供文本不描述 error/retry；只说明 IOMMU 会带来 PTW 开销 |
| retirement buffer | 有。硬件 linked list，entry 存 external/internal address、length、AXI ID、DMA transfer ID、R/W、state、next index；state 含 free/in-flight/failed/peeked/reissuable；不是简单 FIFO，因为同 AXI ID 有序、不同 AXI ID 可乱序完成；只存 metadata，不存 data。64 B 可达 16 KiB data buffer 的 miss tolerance（`socdma2018.txt:403-417`, `:545-569`） | 无 retirement buffer；是 PRMB/PTS/TPreg，属 translation 侧结构 | 无 |
| 顺序/权限/invalidations | 同 AXI ID 的 burst 保序；不同 AXI ID 可任意顺序完成；重发按原请求顺序。vDMA 不再要求软件显式 lock TLB entries，但仍由软件 PTW 替换旧 TLB entry。提供文本没有展开 permission fault 或 IOTLB invalidation 协议（`socdma2018.txt:386-392`, `:412-417`） | 提供文本未描述 permission/invalidation/order 协议 | Linux driver 建 IOVA mapping；device directory entry；LLC bypass；coherency 用 self-invalidation/flush 处理，但不是 IOTLB invalidation 机制（`riscvsva2025.txt:140-146`, `:204-205`, `:269-279`） |
| 软件管理 TLB 条件 | TLB 完全由 PMCA 上一个 PE 管理的软件线程维护；miss metadata 入硬件 queue；多个 MHT 并行处理；PE 读 failing external address，加入 TLB，再写 handled VA；TLB entry 修改需互斥，同 set 用 atomic counter 决定 replacement order（`socdma2018.txt:201-209`, `:353-374`） | 不是 software-managed TLB；是 hardware TLB + parallel PTWs；跨 accelerator QoS 资源分配被列为 future work（`neummu2019-author.txt:715-719`） | 不是 software-managed TLB；是 RISC-V IOMMU 硬件 PTW；平台配置为 4 个 IOTLB entry、1 个 device directory entry、1 个 PTW、两个 AXI port（device transaction + PTW）（`riscvsva2025.txt:93-108`, `:121-125`） |
| 对本任务的约束 | 已直接覆盖“drop-and-retry / retirement metadata 解耦”；未来工作明确点名 out-of-order page table walking，以及“avoid stalling the entire DMA on a TLB miss while maintaining memory order guarantees”（`socdma2018.txt:518-521`） | 已说明降低 DMA 翻译/数据 burst 会损失 MLP/带宽；translation 侧已有 merge/backpressure，不能把“translation 满则 stall”当新发现（`neummu2019-author.txt:537-574`, `:588-618`） | 已给出最接近平台的 PTW 开销测量和 LLC 缓解；没有 finite-buffer DMA 的 translation/data coupling 机制。提供文本未涉及不等于无人做，仍需另查 IOMMU spec 和其他工作 |

补充边界（来自已记录审查，不是本轮新读原文）：TCWS/DAWS/CCWS/MASK 已覆盖页/局部性/翻译压力对源端发出的影响；mNPUsim 已覆盖 DMA burst、计算重叠和 DRAM/PTW/TLB 共享；DWS 已说明 walker 份额影响 walk 完成和 TLB 填充份额。见 `iommu-platform/docs/novelty-gate-20261006.md:33-40`, `:82-86`。因此，不能把“源端页感知”“翻译压力反馈”“共享资源竞争”重新包装成新意。

---

## 2. 最多两个候选命题、旧假设、最小反例与普通修复

### 候选 C1：写 burst 级 retirement 在“streaming 有限内部 FIFO”下失效

**新增约束**
DMA 的写数据来自前一级 read burst 填进有限内部 data FIFO，而不是软件可见、可重读的 SPM/source buffer。目标端写 burst 的 TLB miss 由硬件 IOMMU 以 fault/drop/error 方式返回，且该 burst 的 W data 已被 AXI 消费。此时要按 burst reissue，必须要么保留该 burst 的 W data，要么回读源端。

**旧假设为何失效**
socdma2018 的 retirement buffer 只存 metadata，不存 data；它假设 master 能“keep track of bursts that missed and reissue them”，并且 DMA 的数据源仍可重发。其硬件结论也是“retirement buffer stores just the metadata of each burst”，64 B 对 16 KiB data buffer（`socdma2018.txt:545-569`）。在 memory-to-memory streaming copy 中，源端数据可能只在 DMA 内部 FIFO 里；一旦写 burst 出错或漏翻译后 FIFO 指针前进，metadata 不足以 reissue。

**无需实验的最小反例**
- 4 KiB copy = 512 beats；一个 2 KiB burst = 256 beats（平台事实与诊断模型：`platform-feasibility-20261006.md:32`；`nonblock-dependency-diagnosis-20261004.md:59-61`）。
- 源 page 命中；目的 page 缺失。
- 第一个 2 KiB 写 burst 命中并 retire；第二个 2 KiB 写 burst miss。
- 如果第一个 burst 已释放 FIFO slot，第二个 burst 的 W data 又已被 AXI 消费，那么没有软件可见 source buffer 时，reissue 第二个 burst 无法复现其数据，除非回读源端或保留 2 KiB pending data。
- 这会直接减少可用 512-beat 缓冲，或把数据进度重新耦合到翻译进度。

**普通修复是否已足够**
大概率足够：标准 AXI 做法是 W data slot 到 BRESP OK 再释放，或在目标 translation 未决时 stall read side；再不行就 destination translation 先行、数据后发。这些是 BRESP-gated skid/backpressure/split transaction。即使写成“按翻译进度分配 pending-write 缓冲”，本质仍是 credit/缓冲管理。除非能给出一个非通用的新决策，否则不构成论文创新。

**裁定：淘汰为创新候选；保留为规格/RTL 核查问题。**

---

### 候选 C2：PTW 返回与 DMA 数据共享有序返回路径时的 head-of-line coupling

**新增约束**
硬件 IOMMU 的 PTW return 与 DMA data return 共享同一有序/分 bank 返回路径或同一有限 response queue；DMA 的有限 consumer FIFO 因目的写等翻译而满，导致本应解除该等待的 PTW return 被排在无法排空的数据之后。

**旧假设为何失效**
socdma2018 把 translation miss 放到软件/hardware queue，并 drop 掉缺失 transaction；它假设缺失 transaction 不继续占用 DMA 数据缓冲，PTW 返回与 DMA 数据队列解耦（`socdma2018.txt:87-100`, `:376-417`）。NeuMMU 的 PRMB/PTS 则只处理 translation 侧 merge/backpressure（`neummu2019-author.txt:588-618`），没有把 DMA data-buffer fullness 作为 PTW 返回的 HOL blocker。

**无需实验的最小反例**
你提供的依赖诊断已有一个模型级最小例：`coupled-banked-guard-vv.log:1-13` 中，sharedR head 是 A 的源读数据，A 的 cfifo 已满（3），目的完成 AW 仍未落地；同刻 `ptw_active=1 ptw_r_fire=0`。文档推断 PTW 返回被排在 A 源数据之后（`nonblock-dependency-diagnosis-20261004.md:83-104`）。这是一个“翻译进度等数据排空、数据排空等翻译/写完成”的耦合环。

**普通修复是否已足够**
诊断本身已经给出足够强的反面证据：
- 文档明确不能断言该 active PTW 属于 A，只能写“PTW 活跃且其返回被数据挡住”（`:104-106`）。
- guard2 已把模型 12/12 修过，并把机制定位为“一拍 AW 可见性竞争”（`:108-113`）。
- 板级 A8 根因在修复后板级重跑前未知（`:139-141`）。
- read admission guard 会减少读在写未完成前 bypass/前向的机会，模型通过不能直接解释为 N 收益；公平比较还要把 guard 作为 baseline 成本（`:153-163`）。

因此，C2 的普通修复是 read admission guard、AW 同周期可见性修正、分 VC/bank/ID 隔离。这些是 RTL 流控/死锁修复，不是新架构贡献。按你明确排除的“首个避免 PTW 死锁”类包装，C2 不能立题。

**裁定：淘汰。最多作为工程归因/流控 bug 线索；不得当实板根因或论文主线。**

---

## 3. 最强反驳与降级/淘汰建议

### 3.1 最强反驳

1. **socdma2018 已经做了“翻译进度与数据进度解耦”的核心形式。**  
   它不要求额外 data buffer，靠 lightweight hardware 让 DMA 容忍 TLB miss、记录 missing burst、按原顺序 reissue。其 retirement buffer 只存 metadata，64 B 对应 16 KiB data buffer 的 miss tolerance。任何“用 metadata/retirement 解耦翻译与数据”的宽泛说法都被它直接覆盖。

2. **它已经把最接近的下一步列为 future work。**  
   “avoid stalling the entire DMA on a TLB miss while maintaining memory order guarantees” 正是“有限缓冲下不让翻译进度卡住整个 DMA”的强表述。若我们的候选只是实现这个 future work，必须给出比 VC、credit、per-ID retirement、out-of-order queue 更具体的新机制。

3. **NeuMMU 已把“限流翻译突发”的代价讲清。**  
   “less bursty” DMA data/translation requests 会降低 MLP 和带宽利用，损害 memory-limited 应用。因此任何“翻译进度驱动数据限速”的候选必须正面击败这一前作观察，不能只说“能避免 buffer full”。

4. **riscvsva2025 已经占用“RISC-V IOMMU + CVA6 + Snitch/DMA 平台测量”位置。**  
   它测 IOTLB miss 开销、DMA burst 跨页拆分、LLC 对 PTW 的缓解。换平台、换 IOMMU、换 iDMA 不自动产生新意。

5. **依赖诊断的剩余证据是模型，不是实板根因。**  
   guard2 已修模型内确认的一拍 AW 可见性竞争；PTW 归属未证；板级 A8 根因未知。把 C2 包装成新机制，等于把工程流控 bug 或未确认模型环当创新。

6. **两个候选的动作都可归约为通用组合。**  
   C1 = write-data skid/BRESP-gated retirement + burst retry + 可能 translation-first split。  
   C2 = read admission/VC/bank/ID isolation + deadlock fix。  
   按你的判据，应明确写：**无创新。**

### 3.2 降级/淘汰决定建议

建议 Codex 采用以下裁定：

- **淘汰“有限缓冲 DMA 中翻译进度与数据进度耦合”作为独立创新候选。**
- 保留为**工程/规格核查问题**：真实 RISC-V IOMMU 的 device write TLB miss 响应语义，以及 iDMA 的写 FIFO 释放/重发条件。
- 不进入新控制器或新 RTL 机制实现；不重编 BIT；不把依赖模型通过当作 N 收益或板级根因。

### 3.3 仍可保留的一个“需核新的具体条件”

**唯一值得核的具体条件：**

> 在真实 RISC-V IOMMU + iDMA 路径中，当一个目的写 burst 因 IOTLB miss 被 fault/drop/error 返回时，iDMA 的 512-beat 写数据 FIFO 是否必须保留该 burst 的 W 数据才能做到 burst 级 retirement；还是说普通的 BRESP-gated hold / translation-first split / 分离 AXI ID 已足够？

这个条件需要查：
- RISC-V IOMMU spec 中 device transaction 遇 IOTLB miss 时是 stall、fault、还是 drop；W data 是否已被消费；BRESP 返回什么；
- iDMA 在 WLAST 还是 BRESP 释放 write data slot；是否支持按 burst 重发；AXI ID 是否允许乱序完成；
- PTW return 是否与 DMA data 共享同一 AXI ID/VC/bank/response queue。

**验收检查建议（无需实验）：**
1. 从 spec/RTL 明确写 TLB miss 的响应语义、W data 生命周期、BRESP 语义。
2. 从 iDMA RTL 明确写 FIFO 在何时释放、能否 replay 单个 write burst。
3. 从 IOMMU/互连 RTL 明确 PTW return 与 DMA data 的 ID/VC/bank/order 关系。
4. 若普通修复（BRESP-gated hold、translation-first split、分离 VC/ID、per-burst retirement）中任一项足够，停止该机制。
5. 只有当上述普通修复都不够，且能写出一个非通用、可证伪的新决策，才允许回到小范围概念验证；仍须与 socdma2018 retirement、NeuMMU PRMB/parallel PTW、LLC/PTW 加速、read admission guard 做强基线比较。

---

## 4. 阅读片段路径、物理行号/节号与短原文

### socdma2018

`iommu-platform/out/shared-translation-literature-20261006/papers/socdma2018.txt`

- `:20-24`：  
  “allowing DMA burst transfers to write SVM traditionally requires buffers to absorb transfers that miss in the TLB... stall all SVM accesses once they are full.”
- `:25-36`：  
  “supports parallel burst DMA transfers without additional buffers... lightweight hardware to a standard DMA engine to detect and react to TLB misses.”
- `:79-83`：  
  baseline hybrid “only supports DMA bursts that are guaranteed not to miss... and does not perform any prefetching.”
- `:87-100`：  
  “Hybrid IOMMUs... signal the TLB miss back to the master and drop the transaction... the DMA engine must be able to keep track of bursts that missed and reissue them when the miss has been handled.”
- `:201-209`：  
  “One PE of the PMCA manages the TLB... Upon a TLB miss, it reads the metadata... walks the page table... replaces an older TLB entry... notifies the PE... which then retries the memory access.”
- `:353-374`：  
  MHTs; “TLB misses (only metadata) were enqueued by the IOMMU in a hardware queue”; “atomic counter per TLB set”; mutual exclusion when modifying an entry.
- `:376-417`：  
  “The retirement buffer has three main interfaces...”; “When at least one burst has failed, the control unit stops issuing new bursts... reissues bursts as soon as they are reissuable... resumes regular operation.”
- `:393-401`：  
  “An AXI burst may not cross a page, so each burst requires exactly one TLB entry at the instant its request arrives at the IOMMU.”
- `:412-417`：  
  “The retirement buffer must keep the order in which bursts were issued... bursts with different AXI IDs in any order... not a simple FIFO queue.”
- `:518-521`：  
  “In the future, we plan to explore... avoid stalling the entire DMA on a TLB miss while maintaining memory order guarantees.”
- `:545-569`：  
  “The retirement buffer stores just the metadata of each burst... 64 B for the same TLB miss tolerance as the 16 KiB data buffer—a factor 256 less.”

### NeuMMU

`iommu-platform/out/novelty-gate-20261006/papers/neummu2019-author.txt`

- `:81-92`：  
  PRMB as “translation bandwidth filtering mechanism to reduce the number of distinct page-table walks concurrently in-flight.”
- `:127-142`：  
  contributions: PRMB, high-throughput parallel PTW, translation path register.
- `:537-574`：  
  “One might think that by having the DMA unit send data requests in a less bursty fashion... Unfortunately, such design decision will inevitably reduce memory-level parallelism and memory bandwidth utilization, significantly slowing down memory-limited applications like RNNs.”
- `:578-587`：  
  “When all the PTWs as well as all possible PRMB mergeable slots are full, any further translation requests are blocked until the translation bandwidth is available.”
- `:588-618`：  
  PTS hit -> merge into PRMB; PTS miss -> allocate PTW and PRMB; block when full.
- `:692-704`：  
  “SPM-centric NPUs should be designed for improving translation throughput first, and translation locality second.”
- `:715-719`：  
  “Studying efficient MMU resource allocation strategies across multiple accelerators for QoS is beyond our scope and we leave it as future work.”
- `:928-943`：  
  implementation overhead: PRMB/TPreg/PTS SRAM; negligible FPGA resource overhead.

### riscvsva2025

`iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt`

- `:30-33`：  
  “resolving IO virtual addresses can be particularly costly... up to three sequential memory accesses on IOTLB miss.”
- `:93-108`：  
  “configured with four IOTLB entries and one device directory entry... IOMMU connects to the main crossbar via two AXI ports, one for the device transactions and one for page table walk (PTW) transactions.”
- `:121-125`：  
  IOMMU located between SoC crossbar and Snitch cluster; LLC/SPM 128 KiB.
- `:204-205`：  
  Listing 1 “Self invalidation based coherency.”
- `:224-230`：  
  “When using IO virtual addresses, the latency of a single DMA transfer may increase by 300%... every burst causing IOTLB misses may reduce the effective memory bandwidth for the DMA-engine.”
- `:263-268`：  
  shared LLC for host and IOMMU PTW; IOMMU overhead lower than 2%.
- `:286-299`：  
  “using a shared LLC between the accelerator and host domains significantly improves the IOMMU performance by reducing the PTW time by 15× on average.”
- `:320-323`：  
  “we focus on the RISC-V IOMMU proposed in [7] coupled with a scratchpad-based programmable accelerator cluster with DMA engine.”
- `:356-370`：  
  conclusion: mitigate “without any changes to the IOMMU” by LLC; DMA bypass for bandwidth.

### 依赖诊断与平台事实

`iommu-platform/docs/nonblock-dependency-diagnosis-20261004.md`

- `:10-12`：  
  guard2 与 legacy 模型 12/12；模型结果不是真实 DDR/DMA 结论。
- `:46-49`：  
  跨 bank 读填满 DMA R→W FIFO，AW 不能进 demux，形成 circular wait；guard 延迟“选择”读，不撤回 AXI/TL VALID。
- `:83-104`：  
  `coupled-banked-guard-vv.log:1-13`：sharedR head 是 A 源读数据，A cfifo 满，目的 AW pending，`ptw_active=1`；文档推断 PTW 返回被数据挡住。
- `:104-106`：  
  不能断言 active PTW 属于 A；只写“PTW 活跃且其返回被数据挡住”。
- `:108-113`：  
  guard2 确认的是“一拍可见性竞争”；仍是模型结论。
- `:139-141`：  
  板级 A8 根因在修复后板级重跑前未知。
- `:153-163`：  
  guard 的模型通过不能直接解释为 N 收益；公平比较须把 guard 作为 baseline 成本。

`iommu-platform/docs/platform-feasibility-20261006.md`

- `:32`：  
  “512 是数据缓冲拍数。顶层... DMA0/1 的 BUFFER_DEPTH=512。”
- `:33`：  
  ADMIT 不发 VALID，收到 allow 且 backend idle 才进入 COUNT；当前 BIT role=0 未启用旧 policy。
- `:36`：  
  “实例化单个主 PTW；N 为特定窗口内另一 bank 的命中绕过... 并非多个主 PTW 并行。”

---

## 5. 最终建议

**建议裁定：无创新，淘汰为论文候选；降级为“RISC-V IOMMU 写 miss 响应语义 + iDMA 写 FIFO 重发条件”的规格/RTL 核查。**

若核查发现“写 burst miss 后 W data 必须保留，且普通 BRESP-gated hold / translation-first split / 分离 VC/ID 都不够”，那也只是**可能的小工程缺口**；要升级为贡献，必须再写出一条非通用、可证伪的新决策，并证明它超过 socdma2018 retirement、NeuMMU PRMB/parallel PTW、LLC/PTW 加速和 read admission guard。否则不要进入机制实现。
