# 交付：3 篇全文证据卡 + 中文补充说明草稿

**执行声明（不造假）**：本次调用**未执行任何命令、未联网、未读写任何文件、未运行测试**。以下"已下载 16 篇 PDF / 已核 20 项"均为 Codex 侧给定事实，本文件只做转录与引用。`docs/shared-translation-gaps-20261006.md` 是**尚未落盘的草案**，是否保存、如何保存由 Codex 决定。全部 worker 上一轮原答复（`gap-audit-02/gap.md`）**原样保留**，本文件是它的补充与勘误，不是替换；本文件不宣布任何候选已被否定，也不宣布停止研究。

---

## 第一部分：3 篇全文的证据卡

> **行号规则（必读）**：以下行号一律指**本轮交给我的抽取文本文件的实际行号**，**不是**论文版面行号，**也不是** `gap.md` 报告行号。`pfm2026.txt` 与 `counterpoint2026.txt` 是两栏 PDF 的线性化结果，图注/表行会与正文交错，因此**承载证据的是"章节 + 原文短句"，行号只作定位锚点，建议 Codex 用 PDF 再锚定一次**。

### 卡 A｜PFM（`iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/pfm2026.txt`）

| 字段 | 内容 |
|---|---|
| 题名 | "Rethinking Unified Memory for NPU–PIM Systems: Dual-View Memory for Dynamic Inference of LLM"（L1–2） |
| 版本 | 抽取文本显示 `arXiv:2608.06989v1 [cs.AR] 7 Aug 2026`（L15）。**该 PDF 正文不含"已被 MICRO 2026 接收"的表述**；`verified-gaps.json:115` 的 venue 字段维持 Codex 已核事实，本卡不新增接受结论 |
| 核心机制 | 解耦物理数据布局与访问者可见逻辑视图；ARU（Address Remapping Unit）+ FAS（Flexible Access Scheduler）；MDT（Mapping Descriptor Table）（L33–45；L98–119） |

**证据点 1｜PFM 改的是 PA→HA，不是 VA→PA（关键限制）**
- 章节：§IV-A-2 "PFM Interface for Memory Allocation and Access"
- 行号：L443–445
- 原句："By solving a multi-objective optimization problem, PFM derives the optimal **PA-to-HA (Physical Address to Hardware Address)** mapping. This mapping is stored in the PFM controller's Mapping Descriptor Table (MDT)"
- 章节：§IV-C "Dual-view Addressing & Scheduling"
- 行号：L540–543
- 原句："memory requests first undergo standard **VA-to-PA translation** according to the page table. Then, it uses the MDT and the request origin (NPU or PIM) to **resolve hardware addresses**"
- 章节：§VIII Discussion, "Compatibility"
- 行号：L1242–L1252（右栏，与 Table III 行交错）
- 原句："PFM leverages the native 2 MB superpages already supported by modern GPUs… our design **does not introduce modifications to the fundamental virtual-to-physical address translation**"

**证据点 2｜PFM 明确依赖离线 profiling 与有限 MDT（关键限制）**
- 章节：Fig.1 说明 / §IV-A-2
- 行号：L101–L105、L1274–L1276
- 原句："**Using offline profiling**, PFM formulates physical-address-to-hardware-address mapping as an optimization problem"；"**Limitations** PFM relies on **offline profiling and a finite number of MDT entries** to store mapping templates… PFM may require **re-profiling** or a larger MDT to maintain mapping coverage"

**证据点 3｜求解开销与面积（限制/成本）**
- 章节：§IV-B-4 "Mapping Optimization"
- 行号：L721–L724
- 原句："the feasible mapping space is roughly 10^15 candidates, and the COIN-OR/PuLP solver takes **up to 30 minutes per tensor**. This search is performed offline"
- 章节：§VII-E "Area Overhead"
- 行号：L1276–L1298
- 原句："the total area is **2.82 mm²**… ARU/FAS adds **4.95%** over [the memory-interface] region… 0.34% of the full A100 die"

**证据点 4｜relayout buffer 确实存在（上一轮任务描述中的该点成立）**
- 章节：§IV-C-2 "Flexible Access Scheduler (FAS)"
- 行号：L801–L805
- 原句："To mitigate fragmentation from non-contiguous accesses, a **relayout buffer** aggregates scattered segments across channels and reconstructs them into logical tensor order before delivery to the NPU"
- 另见 §VII-E：`A 64 KB relayout buffer supports multiple burst transfers for data reorganization.`（L1288）

**限制（不可越界的推论）**
1. PFM 与候选 4 在**层次上不重叠**：PFM 保 VA→PA 页表，只在 PA→HA 层做"按访问者的硬件地址视图"。因此它**不能被引用为**"同底层对象不同翻译表示已被做过"的直接证据，也**不能被引用为**"候选 4 已被否定"。
2. PFM 是单机 NPU–PIM 内的内存控制器改造，**无 IOMMU/ATS、无失效命令、无跨设备权限撤销**语义。
3. 用的是 HBM-PIM 仿真栈 + A100 类算力 + GPGPU-Sim/Ramulator（§VII-A，L1105–L1140），**不是真硬件测量**。
4. PFM 参考文献自带一批近邻（NeuPIMs L1353–1357、AttAcc L1368–1372、IANUS L1394–1398、FACIL L1399–1403、UM-PIM L1443–1447、PagedAttention L1312–1316、Vespa L1366–1367）。**本轮输入不含 `catalog.csv`，我无法核这些是否已在 123 项内**——列为待核，不得写成"新缺口"。

---

### 卡 B｜CounterPoint（`.../gap-audit-02/papers/counterpoint2026.txt`）

| 字段 | 内容 |
|---|---|
| 题名 | "CounterPoint: Using Hardware Event Counters to Refute and Refine Microarchitectural Assumptions"（L1–2） |
| 作者/单位 | Nick Lindsay（Yale）、Caroline Trippel（Stanford）、Anurag Khandelwal（Yale）、Abhishek Bhattacharjee（Yale）（L3–11） |
| venue | **ASPLOS '26**，已由抽取文本自证：L47–58 "Proceedings of the 31st ACM International Conference on Architectural Support for Programming Languages and Operating Systems, Volume 2 (ASPLOS '26), March 22–26, 2026, Pittsburgh, PA, USA"；DOI 10.1145/3779212.3790145（L15、L58） |

**证据点 1｜它做的是"模型 vs 计数器"一致性检验，不是在线控制**
- 章节：Abstract / §1
- 行号：L20–25、L149–L155
- 原句："We introduce CounterPoint, a framework that **tests user-specified microarchitectural models… for consistency with performance counter data**. When mismatches occur, CounterPoint pinpoints plausible microarchitectural features that could explain them"；"we invent CounterPoint—a framework that helps experts **reconcile HEC data with their mental models** of the microarchitecture"

**证据点 2｜计数器噪声是量级化的硬约束（对候选 3 最有用）**
- 章节：Fig.1(c) 说明 / §1
- 行号：L82–84、L137–146
- 原句："it becomes **impossible to determine whether the model constraint is violated with 99% confidence once 19 HECs are active**"；"modern architectures allow recording thousands of logical HECs, but these are **multiplexed onto… typically just 4 to 8 at a time**… the growing number of HECs makes it nearly impossible to determine whether a representative model constraint is truly violated"

**证据点 3｜Haswell MMU 的未文档化行为（可作为候选 1/3 的问题前提素材）**
- 章节：Abstract / §7.1
- 行号：L26–31、L781–L824、L841–L859
- 原句："These include a **load–store queue-side TLB prefetcher**, **merging page table walkers**, **abortable page table walks**, and more"；"page table walk merging reduces the number of distinct walks by **nearly half**"；"the **PDE cache is queried before** outstanding walks to the same virtual page are merged"；"**aborted walks** can still consume MMU and memory hierarchy resources, effectively imposing a hidden performance tax"
- 另一条重要反例：同一 MMU 的朴素模型约束 `load.pde$_miss ≤ load.causes_walk` **在真实测量下不成立**（L86–98）

**证据点 4｜作者自己声明的适用边界（禁止越界引用）**
- 章节：§9 Conclusion & Future Work
- 行号：L911–L927
- 原句："our current study **evaluates the benefit of CounterPoint on CPUs**; exploring the utility of CounterPoint to **hardware accelerators** would broaden its applicability."；"this paper presents a **first proof-of-concept** study… Substantial work remains to extend it into a robust, system-wide modeling framework, including support for **multiple cores, multiple sockets, hyperthreading, kernel-level activity**"
- 成本：平均 **213 s/model**、单点可行性 **~200 ms**、约束推导**指数增长**（L872–L904）

**证据点 5｜顺带坐实两条外部事实（对我们的勘误有用）**
- `counterpoint2026.txt:1020–1023` 参考文献 [15] = Basu, Gandhi, Chang, Hill, Swift, "Efficient virtual memory for big memory servers" (2013) → 即**已收录的 Direct Segments**，说明"它是已收录全文对手"再次被印证。
- `counterpoint2026.txt:1052–1056` 参考文献 [58] = Kanellopoulos et al., "Virtuoso…"，页码 1400–1421，DOI 10.1145/3676641.3716027 → 旁证 `verified-gaps.json` 的 virtuoso2025 = ASPLOS 2025。
- `counterpoint2026.txt:925–931` 相关工作中点名 Binoculars 等用 HEC 刻画 page-walker 争用的前作，"CounterPoint offers a **more general-purpose approach**"。

**限制（不可越界的推论）**
1. CounterPoint **不是**在线最优控制的证据，**不是**调度策略，**不是**对"最少观测"命题的正面或反面结论——它是**方法论基线**：任何声称"靠少量计数器就能判别"的方案，必须回答"在多少计数器并入后仍可 99% 置信地判别"。
2. 它只覆盖 CPU、native 执行、data-side activity（§7 开头 L707–709），**不覆盖加速器/设备侧发起者**。
3. 其结论是"**likely** undocumented"（Abstract L272–275），非 RTL 确认；作者自述"full confirmation would require proprietary RTL"。

---

### 卡 C｜Criticality Stacks（`.../gap-audit-02/papers/criticality2013.txt`）

| 字段 | 内容 |
|---|---|
| 题名 | "Criticality Stacks: Identifying Critical Threads in Parallel Programs using Synchronization Behavior"（L1–3） |
| 作者 | **4 人**：Kristof Du Bois、Stijn Eyerman、**Jennifer B. Sartor**、Lieven Eeckhout（L5） |
| 单位 | ELIS Department, Ghent University, Belgium（L6–7） |
| venue | **ISCA'13, Tel-Aviv, Israel**；Copyright 2013 ACM（L62–63） |

**证据点 1｜关键性 = "在做有用工作的时间份额" + "有多少线程在等它"（与候选 1/3 同构）**
- 章节：Abstract / §2
- 行号：L20–28、L215–231
- 原句："we propose a new metric for assessing thread criticality, which **combines both how much time a thread is performing useful work and how many co-running threads are waiting**. We show how thread criticality can be calculated **online with modest hardware additions and with low overhead**."
- 公式：`C_j = Σ_i (t_i / r_i)`（仅对 running 线程累加）；并满足"**所有线程 criticality 之和 = 总执行时间**"（L234–236）

**证据点 2｜"等得最多/最慢的"不等于"最关键的"（这是对候选 1/3 最直接的一击）**
- 章节：§2 例子
- 行号：L215–L231
- 原句："This might seem counter-intuitive because it has the **smallest total running time**… Accelerating thread 3 would reduce the execution time of the critical section… however, thread 0 would still reach the barrier much later… Speeding up thread 0 on the other hand results in a much larger speedup"
- 该现象**在 2013 年就被在线硬件度量**，不是新观察。

**证据点 3｜BIS 与"cache-miss 关键性预测器"是两篇不同前作（勘误依据）**
- 章节：§1 "Comparison to prior work" / §9.3 / 参考文献
- 行号：L123–L137、L1210–L1220、L1227–L1231、L1286–L1290
- 原句："Bottleneck Identification and Scheduling (**BIS**) [15] tries to identify parallel bottlenecks, and migrates threads executing these bottlenecks to a big core… we instead find the thread(s) most critical to overall performance. **A bottleneck could be on the critical path for one thread, but not for others.**"
- 参考文献 [15] 全文可核："J. Joao, M. Suleman, O. Mutlu, and Y. Patt. **Bottleneck identification and scheduling in multithreaded applications**. In **ASPLOS**, pages **223–234**, Mar. **2012**."（L1286–L1290）
- 另一条 [3]：Bhattacharjee & Martonosi, **ISCA 2009**，"Thread criticality predictors…"（L1227–L1231，L1211–L1220 有对比）→ **"2009 那篇"另有其人，不是 Criticality Stacks 本身**

**证据点 4｜动态算法与效果（可用于判断"在线关键性"已被做到什么程度）**
- 章节：§7 / Algorithm 1
- 行号：L799–L825（Algorithm 1）、L828（时间片 10 ms）、L841–L859（α=1.2、β=0.8）、L983–L986（结果）
- 原句："our dynamic scheme achieves an average of **4.6% speedup** in comparison with **BIS's 2.4%** for 8 threads. For 16 threads, we speed up on average by **4.2%**, almost doubling BIS's improvement of **2.7%**"

**限制（不可越界的推论）**
1. 度量**必须先插桩同步原语**才能工作："we instrument all **Pthread and OpenMP** primitives that involve spinning"（L254–L258）；结论自述"can profile every **(instrumented)** stall event"（L1202–L1205）。**纯硬件路径会漏检/迟检**（L218–233 明确讨论 software vs hardware spin detection 的取舍）。
2. 评测域是 **SPLASH-2 / PARSEC / Rodinia 的 8/16 线程共享内存程序**，gem5 全系统仿真（L238–L239、L287、L304–L322）。
3. 它处理的是**线程级同步等待**，不是**设备侧发起的翻译请求**；两者共享"关键性"抽象，但**不是等价证据**。
4. 这篇全文的参考文献本身就是一条检索种子链，123 项目录里没有：`[3] Bhattacharjee/Martonosi ISCA2009`、`[9] Chen/Stenström SC2012 critical lock analysis`、`[10] Eyerman speedup stacks ISPASS2012`、`[12] Fields et al. ISCA2001 critical-path prediction`、`[14] Hollingsworth 1996 online critical path profiling`、`[27] Saidi et al. ISCA2009`、`[28] Suleman et al. ASPLOS2009 big-core critical section`、`[31] Tune et al. HPCA2001`——**未核它们是否在 123 项内**（本轮无 `catalog.csv`），只作下一步种子，不写成缺口。

---

## 第二部分：中文补充说明草稿

**建议路径**：`iommu-platform/docs/shared-translation-gaps-20261006.md`
**状态**：草案，**尚未写入磁盘**。

---

# 共享地址翻译：缺口补充与勘误（2026-10-06）

> 本文件是 `out/shared-translation-literature-20261006/gap-audit-02/gap.md`（上一轮 worker 原答复，**全文保留、不改动、不替换**）的**补充与勘误**。本文件只做四件事：登记本轮真正到手并读到的全文；精确转录 Codex 已核的 20 项题录；修正上一轮的四候选结构错误与若干归属错误；给出证据深度分层与下一步必读清单。
>
> **边界**：未执行任何命令、未联网、未修改任何文件、未运行测试。**"已下载 16 篇 PDF" ≠ "已精读"**；本轮只有 3 篇进入本 writer 的输入。主目录 **123 项本轮不重建**，仍以上一轮 `catalog.csv`（L2–L124）为准，本文件不重复其清单。

## 1. 本轮补了什么

1. **题录级**：`out/shared-translation-literature-20261006/gap-audit-02/verified-gaps.json`（L1–L223）确认 **20 项**，含题名、venue、可下载 `source_url`、候选归属、优先级、审计注记。这是**题录级**证据，不是精读级。
2. **全文级（仅 3 篇，且是节选可复核）**：`papers/pfm2026.txt`、`papers/counterpoint2026.txt`、`papers/criticality2013.txt`。三篇均给出了**文件实际行号 + 章节 + 原句 + 限制**（见第三部分证据卡）。
3. **新题录但无全文**：`cats2015`(ICS 2015)、`trail2026`、`spmc2026`、`faultmerge2026`（后三者 MICRO 2026 accepted_upcoming，`read_depth=metadata_only`）。**未取全文，不推断性能/拓扑/协议。**
4. **结构勘误（最重要）**：上一轮把"命令队列审计/ECMDQ"误列为第 4 候选，见 §4-E1～E3。
5. **下载≠精读**：16 篇 PDF 已落入 `gap-audit-02` 独立目录，本轮**不声明**其中任何一篇已被精读；只有上述 3 篇进入可引用状态。

## 2. Codex 已核的 20 项（精确转录自 `verified-gaps.json`，不添加未核实文献）

| ID | 题名 | venue（已核） | candidate | 优先级 | coverage | read_depth | 审计注记 | source_url |
|---|---|---|---|---|---|---|---|---|
| coz2015 | Coz: Finding Code that Counts with Causal Profiling | SOSP 2015 | 3 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 软件虚拟加速的因果剖析；不能宣称因果扰动本身是新方法 | https://arxiv.org/pdf/1608.03676 |
| bis2012 | Bottleneck Identification and Scheduling in Multithreaded Applications | ASPLOS 2012 | 1 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 按线程等待识别/加速同步瓶颈；跨设备应用是待证明差异 | https://people.inf.ethz.ch/omutlu/pub/bottleneck-identification-and-scheduling_asplos12.pdf |
| asm2015 | The Application Slowdown Model: Quantifying and Controlling the Impact of Inter-Application Interference at Shared Caches and Main Memory | **MICRO 2015** | 3 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 缓存与主存干扰/slowdown 估计；核最少观测与新信息 | https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf |
| mise2013 | MISE: Providing Performance Predictability and Improving Fairness in Shared Main Memory Systems | HPCA 2013 | 3 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 以服务速率和短时优先估计独跑性能；不等于已解决设备翻译归因 | https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf |
| parbs2008 | Parallelism-Aware Batch Scheduling: Enhancing both Performance and Fairness of Shared DRAM Systems | ISCA 2008 | 1 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 批次公平性与保留线程内存并行度；成熟调度基线 | https://people.inf.ethz.ch/omutlu/pub/parbs_isca08.pdf |
| rmm2015 | Redundant Memory Mappings for Fast Access to Large Memories | ISCA 2015 | 4 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 范围映射与标准分页共存；多表示本身已有先例 | https://pages.cs.wisc.edu/~swift/papers/isca15-rmm.pdf |
| twizzler2020 | Twizzler: a Data-Centric OS for Non-Volatile Memory | USENIX ATC 2020 | 4 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 对象身份、持久指针与局部视图；不同于任意设备 PTE 协议 | https://par.nsf.gov/servlets/purl/10222952 |
| mind2021 | MIND: In-Network Memory Management for Disaggregated Data Centers | SOSP 2021 | 2 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 网络内存管理与缓存一致性；数据副本与翻译副本须区分 | https://arxiv.org/abs/2107.00164 |
| tinylfu2015 | TinyLFU: A Highly Efficient Cache Admission Policy | 2015 author preprint; formal version to verify | 2 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 通用缓存准入强基线；当前保存 2015 作者预印本，非声称仅有预印本 | https://arxiv.org/pdf/1512.00727 |
| leases1989 | Leases: An Efficient Fault-Tolerant Mechanism for Distributed File Cache Consistency | SOSP 1989 | 2 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 副本有效期/租约经典；不得直接替代 IOMMU 撤权语义 | https://www.cs.wm.edu/~liqun/teaching/cs780_17s/papers/leases.pdf |
| pfm2026 | Rethinking Unified Memory for NPU-PIM Systems: Dual-View Memory for Dynamic Inference of LLM | MICRO 2026 accepted_upcoming; arXiv 2608.06989 | 4 | P0 | not_in_123_catalog | root_key_sections_checked | 已核 §V-C/§VIII：ARU 是 PA 到设备硬件地址；保持 VA→PA，需离线映射与 relayout buffer；否定宽泛双视图新意，不等于全覆盖 IOMMU 翻译副本 | https://arxiv.org/pdf/2608.06989 |
| deadentry2026 | Dead on Arrival: Characterizing and Protecting Against Dead-Entry TLB Misses in GPU Microarchitectures | 2026 preprint; **submitted is not accepted** | 1 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | GPU 死条目/重复遍历与 burst amplification；arXiv 标 submitted IISWC，**不能写 accepted** | https://arxiv.org/pdf/2606.00486 |
| virtuoso2025 | Virtuoso: Enabling Fast and Accurate Virtual Memory Research via an Imitation-based Operating System Simulation Methodology | ASPLOS 2025 | 3 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 模拟方法与 OS 成本建模；工具适用性待核，不是已复现 | https://arxiv.org/pdf/2403.04635 |
| counterpoint2026 | CounterPoint: Using Hardware Event Counters to Refute and Refine Microarchitectural Assumptions | ASPLOS 2026 | 3 | P0 | not_in_123_catalog | root_key_sections_checked | 计数器噪声与微架构模型反驳；Haswell MMU 案例，不等于已有在线最优控制 | https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf |
| criticality2013 | Criticality Stacks: Identifying Critical Threads in Parallel Programs using Synchronization Behavior | ISCA 2013 | 1 | P0 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending（**已落后，见 §4-E5**） | 同步行为推导线程关键性；不是 2009 论文 | https://users.elis.ugent.be/~leeckhou/papers/isca13.pdf |
| atlas2010 | ATLAS: A Scalable and High-Performance Scheduling Algorithm for Multiple Memory Controllers | HPCA 2010（**与 source_url 文件名冲突，见 §4-E13**） | 1 | P1 | not_in_123_catalog | primary_metadata_or_abstract_checked_full_review_pending | 多控制器 least-attained-service 调度基线 | https://www.cs.cmu.edu/~dongsuh/paper/HPCA16_ATLAS.pdf |
| cats2015 | Criticality-Aware Dynamic Task Scheduling for Heterogeneous Architectures | ICS 2015 | 1 | P0 | not_in_123_catalog | metadata_plus_related_author_talk | 作者机构已核 venue 和作者，UPC 相关 talk 摘要阐释运行时关键路径；**未取得完整 ICS 论文，勿把 talk 当正式全文** | https://www.bsc.es/mateo-valero/71-conferences ；次源 https://upcommons.upc.edu/bitstreams/11354708-3c36-4fc4-bbe5-e0e4fdc8c9ba/download |
| trail2026 | Trail: Effective, Low-Cost and Scalable Temporal TLB Prefetching via Page-Table-Embedded Deltas | MICRO 2026 accepted_upcoming | 1 | P1 | not_in_123_catalog | metadata_only | 官方程序 + 作者主页确认；有 Virtuoso 实现链接，**本文全文未取，不推断性能** | https://www.microarch.org/micro59/program/ |
| spmc2026 | SPMC: Speculative Multicast for GPU Address Translation Sharing | MICRO 2026 accepted_upcoming | 2 | P0 | not_in_123_catalog | metadata_only | 官方程序题录；直接共享翻译近邻，**未取全文，不凭题名断言拓扑/协议** | https://www.microarch.org/micro59/program/ |
| faultmerge2026 | Make Every Batch Count: Fault Entry Merging for Efficient Batching in Unified Virtual Memory | MICRO 2026 accepted_upcoming | 1 | P0 | not_in_123_catalog | metadata_only | 官方程序与作者实验室确认题录；**作者网页摘要错误地谈 SSD，未用其摘要；全文未取** | https://www.microarch.org/micro59/program/ |

**统计（由上表直接可数）**：P0 = 12、P1 = 8；候选分布 1/2/3/4 = 8/4/5/3；`coverage` 全部为 `not_in_123_catalog`；`read_depth` 为 `metadata_only` 3 项、`metadata_plus_related_author_talk` 1 项、`root_key_sections_checked` 2 项、其余 14 项为元数据/摘要级。

## 3. 这 20 项为何影响上一轮四候选

**先立等价性边界（重要）**：BIS / Criticality Stacks / MISE / ASM / Coz 属于**通用共享内存与并行运行时**的强前作或基线，**不与"跨设备地址翻译"完全等价**。正确用法是：它们负责收窄"哪些抽象已被做过"，不负责证明"IOMMU 侧的问题已被解决"。

- **候选 1（让翻译服务识别跨设备计算依赖）** — 相关条目：bis2012、parbs2008、criticality2013（已读）、atlas2010、cats2015、trail2026、faultmerge2026、deadentry2026。已读的 Criticality Stacks 把"忙的/等得最多的"与"最关键的"显式分开，并证明 2013 年即可在 CPU 上在线算出线程关键性（平均 +4.6%）。**收窄后的可辩护点**：不在"关键性调度"这个抽象，而在"设备侧发起者、无应用插桩可用时如何获得等价信息"。
- **候选 2（按映射生命周期决定共享范围）** — 相关条目：mind2021（副本所有权/一致性协议）、tinylfu2015（准入策略）、leases1989（副本有效期）、spmc2026（题录级近邻，**不断言**）。这三条正好覆盖"谁持有副本／何时准入／何时过期"，正是 `translation-opportunities-20261006.md:25` 自己要求补的"自适应复制、缓存准入与一致性协议的经典近邻"。
- **候选 3（找出指导翻译优化所必需的最少观测）** — 相关条目：coz2015、asm2015、mise2013、virtuoso2025、counterpoint2026（已读）。已读的 CounterPoint 给出硬约束：噪声随并发计数器数量增长，19 个 HEC 之后 99% 置信度下已无法判别违反（`counterpoint2026.txt:82-84`）；且它自己做的是**离线模型检验**，并在 future work 中明说未覆盖加速器、多核/多 socket、内核态（`counterpoint2026.txt:911-927`）。**因此**：它**不是**"最佳在线控制已被证明"的证据，也**不是**对候选 3 的否定；它是候选 3 必须回答的**方法论下限**。
- **候选 4（同一底层对象，允许不同设备使用不同的翻译表示）** — 相关条目：rmm2015（P0，新消歧）、twizzler2020、pfm2026（已读）。PFM 的限制必须写进候选 4 的对手清单：PFM 保留标准 VA→PA 页表翻译，只重做 PA→HA；依赖离线 profiling 与 32 项 MDT；带 64 KB relayout buffer。**结论**：PFM 是"同一 PA 空间下按访问者给出不同硬件地址视图"的强前作，但**不在 VA→PA 层**与候选 4 竞争；候选 4 若在设备侧表示/身份层主张新意，必须**显式划清与 PFM 的层次**。

## 4. 对上一轮 `gap.md` 的勘误（逐条）

- **E1｜候选 4 ≠ ECMDQ/队列审计（结构性错误，作废）**。正式候选 4 始终是"同一底层对象，允许不同设备使用不同的翻译表示"（`docs/translation-opportunities-20261006.md:41-47`）。ECMDQ 只出现在**降级表** `:53`，作为"多命令队列"宽泛想法的反证线索。上一轮 `gap.md:26`（"这正是候选 4（ECMDQ/队列实现）必须补齐的前置"）与 `:37`、`:131-134`（G09）把它升格为候选 4 项，**作废**。
- **E2｜删除"ENQCMD 是 ECMDQ 的直接前身语义"**。`gap.md:251`、`:134` 的该说法**无据**。旧广域地图 §6（`research-landscape-20261005.md:137`）的 ENQCMD 行可作为"生产 API 已有语义"的规范层证据保留，但不得用于支撑候选 4 或任何 ECMDQ 叙事。
- **E3｜删除上一轮对 ECMDQ 的 Intel 归属**。`gap.md:66`（G09）、`:131` 写"Intel 内核补丁系列"，**未经证实**，且该题**不在**本轮 20 项已核表内。只保留 `translation-opportunities-20261006.md:53` 自带的 2024 LKML 链接级线索；不写作者/公司/版本。
- **E4｜ASM 正式 venue = MICRO 2015**（`verified-gaps.json:27`）。删除 `gap.md:59,96` 的"null〔待核〕；记忆疑似 MICRO 2013"。
- **E5｜Criticality Stacks = ISCA 2013，作者 4 人**（Du Bois、Eyerman、**Sartor**、Eeckhout，Ghent University），见 `papers/criticality2013.txt:5-7,62-63`。`gap.md:61,106` 作者名单不全、venue 待核标记可去。`verified-gaps.json:163` 的"不是 2009 论文"应明确指认为 **Bhattacharjee & Martonosi, ISCA 2009**（`criticality2013.txt:1227-1231` 参考文献 [3]）；即"2009 那篇"另有其人。另：`verified-gaps.json:165` 的 `read_depth` 落后于本轮实际到手全文，建议升级字段（字段名由 Codex 定）。
- **E6｜DVM / Direct Segments / rIOMMU / MMU-aware DMA 不是新缺口**：`dvm2018`、`direct_segments2013`、`riommu2015`、`socdma2018` 均已有 123 项全文（`gap.md:42-44,227-228` 已部分修正，此处固化）。新增侧证：CounterPoint 参考文献 [15] 即 Direct Segments（`papers/counterpoint2026.txt:1020-1023`）。
- **E7｜RMM 是真实新条目，不是"检索不到"**：`verified-gaps.json:58-67` 给出 `rmm2015`（Redundant Memory Mappings for Fast Access to Large Memories, ISCA 2015）+ 可下载 URL。`gap.md:229` 的"给定输入中检索不到、可能指多篇不同工作"**作废**：消歧为这一篇，归候选 4，P0。
- **E8｜UCP / KPart / GPS / WIC / DAE / NDS / Tiara / DX100 / MINISA / PagedAttention / vTensor / GMLake 不是新遗漏**：它们**已在旧广域地图**（`research-landscape-20261005.md` §5/§7 各表），属"内部并入目录"的 TODO，**不要重复下载、不要写成新缺口**。`gap.md:235-254` 的 B 态表可保留为并入清单，但语气降为"已知未并入"。
- **E9｜19 项"已收录但缺全文"清单未变化**（`gap.md:22`），本轮不增不减。优先级见 §6。
- **E10｜本轮新题录缺口 ≠ 已收缺全文**：`cats2015`、`trail2026`、`spmc2026`、`faultmerge2026` 是"123 项目录里没有"（coverage=not_in_123_catalog）；与那 19 项"目录有行但无全文"是**两类问题**，报告里不得混表。
- **E11｜Dead on Arrival 落定**：`verified-gaps.json:124-133` 给出题名与 arXiv 2606.00486；venue 只能写 **submitted IISWC，不得写 accepted**。`gap.md:188-190`（"题名/作者/venue 全 null"）**作废**。
- **E12｜这些仍是〔待核〕，不要升级也不要消失**：`gap.md` 中的 G06 TimeGraph、G07 MemGuard、G12 Baymax、G13 vCUDA、G16 LRC/Munin/TreadMarks、G17 ARC/TinyLFU/Hyperbolic、G18 CPI²/Bubble-Up、G20 CHERI、G22 GPUDirect 族、G23 TPP、G24 扩查位，**均不在本轮 20 项已核表内**；统一标注"未进入本轮 20 项，维持上一轮〔待核〕状态"，不得写成已核事实（TinyLFU 是唯一例外：它以 `tinylfu2015` 形式进入了本轮表）。
- **E13｜`atlas2010` 内部字段冲突（提请 Codex 裁定）**：`verified-gaps.json:170` venue 写 HPCA 2010，但 `:171` 的 `source_url` 文件名是 `HPCA16_ATLAS.pdf`。本文件**不擅自二选一**。
- **E14｜PFM 的会场字段不可由本轮 PDF 自证**：抽取文本只显示 `arXiv:2608.06989v1`（`papers/pfm2026.txt:15`）。维持 Codex 已核事实，不新增"已被接收"的结论。

## 5. 证据深度分层（避免"下载=精读"的滑移）

| 层 | 含义 | 条目 |
|---|---|---|
| **D0** | 本轮到手全文并给出可复核行号/章节/原句 | pfm2026、counterpoint2026、criticality2013（3） |
| **D1** | Codex 侧已核关键章节，但全文未进入本 writer 输入 | 无（与 D0 同源） |
| **D2** | 仅题录（metadata_only），任何机制/性能均不可推断 | trail2026、spmc2026、faultmerge2026（3） |
| **D3** | 题录 + 相关作者讲稿（**讲稿≠正式全文**） | cats2015（1） |
| **D4** | 元数据/摘要级已核 | 其余 13 项 |
| **D5** | 123 项**已收录但缺全文**（C 态，上一轮 19 项，未变化） | `gap.md:22` 所列 19 条 |
| **D6** | 旧广域地图**已有**、123 项未并入（内部并入问题，**非新缺口**） | UCP、KPart、GPS、WIC、DAE、NDS、Tiara、DX100、MINISA、PagedAttention/vTensor/GMLake、RISC-V IOMMU queues、Linux HMM、CUDA stream-ordered allocator、Linux SVA/ENQCMD |

## 6. 下一步必读

**A. 本轮 20 项表内、尚未到手全文的 8 项（按"不读就会把已做过的事写成新意"排序）**

1. **coz2015（P0, SOSP 2015，候选 3）** — 决定候选 3 能否以"提出因果归因"为贡献。
2. **bis2012（P0, ASPLOS 2012，候选 1）** — 决定"关键性调度"是否只是把本地瓶颈识别搬到 I/O 侧。（venue 已由 `criticality2013.txt:1286-1290` 旁证）
3. **asm2015（P0, MICRO 2015，候选 3 + 1）** — 决定候选 3 的"还缺什么信息"与候选 1 的目标函数是否已被定义过。
4. **mise2013（P0, HPCA 2013，候选 3）** — 决定"以服务速率估计独跑性能"是否已足够。
5. **rmm2015（P0, ISCA 2015，候选 4）** — 候选 4 的**新**直接对手（范围映射与标准分页共存）。
6. **mind2021（P1, SOSP 2021，候选 2 + 4）** — 决定"副本所有权协议"是否已有更彻底的结构性方案。
7. **parbs2008（P1, ISCA 2008，候选 1）** — 批处理 + 防饥饿公平性的成熟基线。
8. **deadentry2026（P0, submitted IISWC，候选 1）** — GPU 死条目/重复遍历；venue 只能写 submitted。

次批（预算允许时紧随）：twizzler2020、tinylfu2015、leases1989、virtuoso2025、atlas2010、cats2015、trail2026、spmc2026、faultmerge2026。

**B. 已收录但缺全文的优先 6 项（与上表是不同轨道，不可混算）**：`shadowupdate2026`、`clover2026`、`latpc2025`、`nbwalker2026`、`cato2026`、`helm2025`。其中 `shadowupdate2026` 是候选 2 净收益结论的唯一支撑（`evidence.md:24` 已声明不能当全文）；`clover2026`、`latpc2025` 同压候选 2 判据。这 6 条不取全文，候选 2 的"必须超过的前作"清单就是不完整的。

**C. 免费种子（来自本轮已到手全文的参考文献，成本低、命中率高；是否已在 123 项内本轮未核）**
- `criticality2013.txt`：[3] Bhattacharjee/Martonosi ISCA 2009；[9] Chen/Stenström SC 2012；[10] Eyerman et al. ISPASS 2012；[12] Fields et al. ISCA 2001；[14] Hollingsworth 1996；[27] Saidi et al. ISCA 2009；[28] Suleman et al. ASPLOS 2009；[31] Tune et al. HPCA 2001。
- `counterpoint2026.txt`：[41] Ertmer et al. 2025（Reverse-Engineering the Address Translation Caches）；[63] Lindsay & Bhattacharjee IISWC 2024；[121] Binoculars；[12] Translation caching: skip, don't walk。
- `pfm2026.txt`：[17] NeuPIMs、[41] AttAcc、[48] IANUS、[49] FACIL、[62] UM-PIM、[25] PagedAttention。

## 7. 未做的事与不确定项

1. **未执行任何命令、未联网、未修改文件、未运行测试**；本文件为草案，落盘由 Codex 决定。
2. 所有行号均为**抽取文本文件行号**；`pfm2026.txt` / `counterpoint2026.txt` 为两栏线性化结果，行号需 Codex 用 PDF 再锚定一次（章节名 + 原句才是承载证据）。
3. 其余 **17 项未到手全文者，一律只沿用 `verified-gaps.json` 与 Codex 给定已核事实**，本文件不伪造精读，不推断性能、拓扑、协议、误差。
4. `gap.md` 的 §4"最优先取全文 8 项"应以下列方式**修订而非删除**：删去其中的 G09（ECMDQ，理由见 §4-E1～E3）；G04（Criticality Stacks）与新增的 CounterPoint 已到手，转为 D0；空位由 rmm2015 与 deadentry2026 补入（见 §6-A）。
5. `verified-gaps.json` 有三处待 Codex 裁定，本文件不擅自定案：`atlas2010` 的 venue/URL 冲突（E13）；`pfm2026` 的会场字段无法由本轮 PDF 自证（E14）；`criticality2013` 的 `read_depth` 落后于实际（E5）。
6. **本文件不宣布任何候选已被否定，也不宣布研究停止**；BIS/Criticality/MISE/ASM/Coz 是强前作或基线，不与跨设备翻译完全等价。

---

## 附录：给 Codex 的复核清单（验收要点）

1. 用 PDF 重新锚定 `pfm2026.txt` 与 `counterpoint2026.txt` 的 3 处行号区间（PFM Compatibility L1242–L1252、PFM Limitations L1268–L1276、CounterPoint Future Work L911–L927）。
2. 确认 `criticality2013.txt:1286-1290`（BIS = Joao/Suleman/Mutlu/Patt, ASPLOS 2012, pp.223–234）可作为 bis2012 的**独立旁证**。
3. 裁定 `atlas2010` 的 venue（HPCA 2010 vs URL 名 HPCA16）。
4. 确认 `rmm2015` 与 `direct_segments2013` 是两篇不同工作（后者已在 123 项内且全文可读）。
5. 确认 `docs/shared-translation-gaps-20261006.md` 落盘后，`gap-audit-02/gap.md` **保持原样不动**（保留 worker 原答复）。
6. 若采纳 §6-A 的 8 项清单，同步修订 `gap.md` §4 的旧 8 项列表（并注明原列表被修订而非被抹除）。