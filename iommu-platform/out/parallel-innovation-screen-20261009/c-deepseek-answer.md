以下是对**已提供文件**的阅读归纳，不新增浏览、执行或写文件，也不把任何模型/推演称为真实应用 trace。提供材料里没有可核的 `BIS` 定义；为避免发明规则，我把它单列为未知。  

---

## 0. 结论先行（可导航）

1. **C4-P1 当前准确边界**：它不是“已发现一个新的搬运抽象”，而是“翻译页集合与真实消费组开工条件共同约束批次”的**条件保留问题/否证对象**，证据不足，不进入 RTL。  
   证据：`out/innovation-council18-20261005/round2/C4.md:13`、`:74`、`:113`；`docs/innovation-council18-20261006.md:50`。  
2. **地图中没有核到至少一个真实应用同时满足四个条件**：consumer 可提前开工、翻译是关键路径、跨页重排有空间、普通页池/大页不能轻易消除。多个文件明确写“尚未证实/尚未找到/应用真实性最弱”。因此状态应写 **未知，而非“无”**。  
   证据：`docs/innovation-council18-20261006.md:50`、`:92`；`out/innovation-council18-20261005/round2/C4.md:13`、`:97`、`:101`；`A4.md:7`、`:72–86`；`D4.md:3`、`:7–10`、`:20`。  
3. **已有普通等价/强对手**：remaining-use/refcount 的 OR/refcount 在模型内与普通 interval/refcount 逐项同 decision；Depth4 现有窗口扫描可挡洞；Milk/Propagation Blocking 已被 DX100 相关工作列为软件批处理+重排的先行；consumer 组信息可能普通运行时已有。  
   证据：`docs/translation-sharing-window-20261008.md:37–43`；`out/research-landscape-20261005/papers/dx1002025.txt:1308–1316`；`out/innovation-council18-20261005/round2/C4.md:76`。  
4. **简单对照需要的合同**：同一 job/context/window/完成信息、同一资源预算、同一正确性/生命周期；普通对照允许 translate-first、hit-first/RR、bounded lookahead、任务感知保留；反事实应报 T00/T10/T01/T11，不能把两个独立加速百分比相加。  
   证据：`docs/translation-sharing-window-20261008.md:49`；`out/innovation-council18-20261005/round2/C4.md:76`、`:82`、`:88–95`。  
5. **下一证据不是再找关键词相似，而是**：真实应用阶段的地址产生、实际映射、真实消费依赖图；然后给可行大页/池化、Milk式软件分组、小 tile、DX100式重排的同时间线成本。  
   证据：`docs/innovation-council18-20261006.md:92`；`out/innovation-council18-20261005/round2/C4.md:97`、`:99`、`:101`、`:111`。  

---

## 1. 最强现有机制逐项：输入信息 / 调度动作 / 评价目标 / 原文证据与已读范围

> 说明：Milk、小 batch 软件流水、BIS 在提供材料中没有完整原文；下列凡标“二手/未读原文”者，不能当作已全文核过。DX100 与 SIMT Walk Scheduling 有较完整抽取文本。

### 1.1 Milk

| 项 | 内容 |
|---|---|
| 输入信息 | DX100 相关工作二手描述：程序员标注/编译器可见的整批随机 indirect accesses；cache-fitting batch；中间数据结构。C4/A4/D4 二手笔记补充：TLB reach、cache 容量约束、协程式 collection/distribution/delivery、有序模式。 |
| 调度动作 | 把随机 indirect accesses 收集成能 fit cache 的批次，再重排成较顺序的 DRAM 访问；容量感知临时容器；协程式 collection/distribution/delivery。 |
| 评价目标 | 改善 indirect memory/DRAM 效率；代价是计时/空间开销、中间结构污染 cache、牺牲 temporal locality；支持较广应用而非只 PageRank。 |
| 原文证据 | `out/research-landscape-20261005/papers/dx1002025.txt:1308–1316`（DX100 §7 Related Work 对 Milk/Propagation Blocking 的描述）；C4 二手：`out/innovation-council18-20261005/round2/C4.md:36`、`:109`；A4：`A4.md:20`、`:91`；D4：`D4.md:9`、`:13`。 |
| 已读范围 | **未读 Milk 原文**。提供材料只给出 DX100 相关工作二手描述和 C4 source notes 的转引；`C4-source-notes.md` 未在本上下文中提供。不能编造 Milk 的精确 ISA/接口/实验数字。 |
| 下一条需浏览证据 | Milk 原文 §3、§5.2；`out/innovation-council18-20261005/sources/C4-source-notes.md`（未提供）；重点核“是否已用同样 task graph/consumer readiness 信息”和“中间结构成本”。 |

### 1.2 DX100

| 项 | 内容 |
|---|---|
| 输入信息 | bulk index tile，最大 16K indices 在取回后对 DX100 可见；虚拟字地址经 Interface 翻译；映射到 DRAM 坐标 CH/RA/BG/BA/RO/CO/WO。DX100 假设 stream/indirect 区域通过 huge pages 映射，只用 256-entry TLB，并在 application lifetime 一次性传 PTE。 |
| 调度动作 | Indirect Access unit 用 Row Table 按 DRAM row 重排；Word Table 按 column 串 linked-list 合并 redundant accesses；Request Generator 跨 channel/bank group interleave；compiler 做 tiling、hoist indirect loads、插 wait API。 |
| 评价目标 | 提高 row-buffer hit、bandwidth utilization、MLP/request buffer occupancy；2.6× over multicore、2.0× over DMP；带宽 3.9×、row-buffer hit 2.7×；tile size 1K→32K 时 1.7×→2.9×；面积/功耗。**明确不支持 pointer chasing**，只做 array-based indirection。 |
| 原文证据 | `out/research-landscape-20261005/papers/dx1002025.txt:71–77`（16K window/重排合并交错）；`:256–271`（8 条 ISA/scratchpad tile）；`:273–281`（Address Decoder 用 Interface 翻译）；`:369–395`（Range Fuser/ALU）；`:432–452`（huge pages + 256 TLB + PTE 一次装载）；`:477–495`（不支持 pointer chasing，小 frontier 回退 baseline）；`:501–511`（compiler/legality）；`:522–580`（eval methodology）；`:719–745`（带宽/RBH/request buffer）；`:1076–1093`（prefetcher 对比）；`:1292–1343`（memory reorder、Milk/Propagation Blocking 相关工作）。 |
| 已读范围 | 提供的是 DX100 抽取文本，可核上述行；不是外部新检索。DX100 假设翻译开销已被 huge pages/256 TLB 掩盖，因此它本身不是“翻译关键路径”的证据。 |
| 下一条需浏览证据 | DX100 artifact 公开程序是否包含公开原始索引流；`https://github.com/arkhadem/DX100.git`（`dx1002025.txt:1718`）；以及真实应用实际分配/映射是否类似 huge pages。C4 已警告：DX100 artifact 提供程序不等于提供公开原始索引流。`C4.md:97` |

### 1.3 SIMT Walk Scheduling（`SIMTWalkScheduling`）

| 项 | 内容 |
|---|---|
| 输入信息 | 每个 page walk request 带 instruction ID（实现 20 bits）；PWC lookup 给出该 request 约需 1–4 次 memory access 的估计；IOMMU buffer 中 pending walk requests；最近已调度 request 的 instruction ID。 |
| 调度动作 | 若 page walker ready，优先选与“最近已调度 walk 同一 SIMD instruction”的请求，以 batch 同一 instruction 的 walk；若无，则选 estimated least work（最低 score）的 instruction，tie 时 oldest；加 aging 防 starvation（阈值 2M requests）。PWC 加 2-bit saturating counter 减少 score 失真。 |
| 评价目标 | 降低 irregular GPU app 的 translation overhead；over FCFS 平均 30%、最高 41%；stall cycles 降 23%；first/last walk latency gap 降 37%；page walk 数降 21%；更大 TLB/更多 walker 时收益降到 25%/8.4%/5.3%；IOMMU buffer 128/512 时收益 13%/50%。 |
| 原文证据 | `out/research-landscape-20261005/papers/walk_scheduling2018.txt:16–38`（abstract/SIMT-aware）；`:103–105`（优先 less work）；`:122–130`（batch 同 instruction）；`:216–235`（IOMMU/walker/PWC）；`:291–309`（SJF + batch key ideas）；`:395–408`（interleaving）；`:414–489`（design/score/指令 ID）；`:508–528`（scan 不在 critical path、aging、PWC counter）；`:640–665`（speedup/stall）；`:736–767`（latency gap/page walk 降）；`:798–839`（why not large pages）。 |
| 已读范围 | 提供的是该论文抽取文本，可核上述行。它是 **page walk 调度**，不是 scatter/gather 数据重排，也不是 consumer-first。SJF/batch 本身是普通调度概念；具体 instruction-ID 调度器是论文提案。 |
| 下一条需浏览证据 | 与普通 SJF、FCFS、random、单纯更大 TLB/walker 的同资源对照已在该文；若 C4 要用它，还需核它是否与真实 consumer 开工时间联合，而不仅是 walk 完成顺序。 |

### 1.4 小 batch 软件流水

| 项 | 内容 |
|---|---|
| 输入信息 | batch/tile indices、实际 mapping、buffer ready、完成/退休状态；C4/A4/D4 认为普通软件可获得同一 job/context/window/完成信息。 |
| 调度动作 | 把大 gather 切成 cache-fitting 小批；按虚拟页/页组排序；double buffer；让翻译/预取与数据搬运重叠；bounded lookahead；page barrier/lag cap；有限 prefetch。 |
| 评价目标 | whole-copy/stage 时间、中间结构开销、cache pollution、翻译 miss、带宽；但 C4 明确指出同信息简单策略必须一起收费。 |
| 原文证据 | DX100 compiler tiling/hoist/wait：`dx1002025.txt:454–476`、`:501–511`；tile size 敏感性：`:1096–1107`；C4 把 “DX100 小 tile/双缓冲＋AMAC” 列为最强普通组合之一：`out/innovation-council18-20261005/round2/C4.md:80`、`:99`；A4 也列小 tile+双缓冲为强对手：`A4.md:57–58`；D4 列普通组合：`D4.md:9–10`；`docs/translation-sharing-window-20261008.md:49`。 |
| 已读范围 | 这不是单篇命名论文，而是从 DX100/compiler、Milk/Propagation Blocking 相关工作、C4/A4/D4 对手列表中抽出的普通策略组合。提供材料没有给出该组合的完整统一实现评测。 |
| 下一条需浏览证据 | 在真实 trace 上实现同信息 T00 基线：页分组+小 tile+双缓冲+普通 translate-first/bounded lookahead，并计中间 buffer、通知、cache/LLC 代价。 |

### 1.5 remaining-use / refcount

| 项 | 内容 |
|---|---|
| 输入信息 | 注册 job `J`、每 direction 连续 frontier、可靠结束状态；共享 entry `e`；`compatible_context`；`entry_page_interval`。模型里是两份 32KiB 拷贝共享输入和翻译上下文，每流每页 D/S/D/S 四次逻辑译址请求。 |
| 调度动作 | 构造 `Remaining(J,dir)`；对共享 entry 计算 `need(e,J,dir)=compatible_context && overlap(entry_page_interval, Remaining)`；再做 OR 或 refcount。 |
| 评价目标 | 逻辑翻译 miss 数；比较 LRU、ordinary remaining-use、private dest+shared src、offline oracle；模型 9 组 trace/capacity、4 普通策略、36 policy records。`ordinary≥oracle≥24`；E8/gap4 LRU30→ordinary remaining-use24。 |
| 原文证据 | `docs/translation-sharing-window-20261008.md:13–31`（矩阵/限制）；`:33–39`（停止把“登记 job 剩余用途取交/OR 再 dead-first”作为独立算法；Depth4 扫描挡洞；short-parent hole）；`:41–43`（普通等价：boolean 与普通 interval/refcount 完全相同）。 |
| 已读范围 | 这是 **一次有限逻辑请求/缓存模型**，不是 DUT、不是硬件实验；`PTE_reads=3×misses` 仅是 Sv39 无 PWC 假设，不是实测 PTE counts/cycle。不能称真实 copy 达到 24。 |
| 下一条需浏览证据 | 在现有 prepared parent/window＋virtual/physical W obligation 上构造真实可达 trace，区分 TL-frontier 控制与普通 page/translate-first controls；若无差异即停。`translation-sharing-window:53` |

### 1.6 BIS

| 项 | 内容 |
|---|---|
| 输入信息 | **未知**。 |
| 调度动作 | **未知**。 |
| 评价目标 | **未知**。 |
| 原文证据 | 在提供的 `AGENTS.md`、`docs/innovation-council18-20261006.md`、`docs/research-landscape-20261005.md`、`docs/translation-sharing-window-20261008.md`、C4/A4/D4 R2、L3、DX100、Walk Scheduling 文本中，**均未出现可核的 BIS 展开或来源**。 |
| 已读范围 | 不能列输入/动作/目标，也不能核其普通复现；若 BIS 是某个历史方案缩写，需要给出来源文件与行号。 |
| 下一条需浏览证据 | 请 Codex 明确 BIS 全称/文件；否则本席不替 Codex 发明规则。 |

### 1.7 consumer-first

| 项 | 内容 |
|---|---|
| 输入信息 | 真实算法依赖形成的消费组 `R_g`；最后一个必要输入到达时间；后续全批 barrier/kernel 启动/stream 顺序/消费计算资源是否不足；地址产生时刻；实际映射。 |
| 调度动作 | 消费者在合法必要输入到达后提前开工，不等整批完成；代价是可能破坏页复用/重排窗口。 |
| 评价目标 | group ready 时间、stage/end-to-end 时间、reuse/miss、通知成本；必须在翻译瓶颈独立确认后才谈联合。 |
| 原文证据 | `out/innovation-council18-20261005/round2/C4.md:13`、`:78`、`:90–95`、`:101`；A4 认为消费组早开工本质是任务图/数据流调度，属于旧题：`A4.md:57–60`；D4 最强版本：`D4.md:3`、`:7–10`、`:20`；`docs/innovation-council18-20261006.md:92`。 |
| 已读范围 | 提供材料中没有一篇 consumer-first 原始论文；它是 C4 候选接口/测量问题。不能把“有消费组”自动当作新信息。C4 明确：普通运行时可能已有同样任务图，必须让软件对手使用同样信息。 |
| 下一条需浏览证据 | 真实应用阶段：记录间接数据走 CPU/GPU 本地访存/DMA、默认 arena 和映射、实际消费者是否独立；只有算子名不进入 trace。`C4.md:97` |

---

## 2. 地图中是否至少一个真实应用同时具有四条件？

**结论：未核到；状态是“未知”，不是“无”。**  
四个条件必须分别成立，C4 明确“两个门槛分别成立才有联合议题”。`out/innovation-council18-20261005/round2/C4.md:78`

| 条件 | 什么算成立 | 提供材料中的核查状态 | 证据 | 缺口/下一证据 |
|---|---|---|---|---|
| consumer 可提前开工 | 消费组来自真实算法依赖；最后一个必要输入到达后可合法启动真实工作；后续无全批 barrier/kernel/stream/消费资源不足 | **未核到真实应用**。C4 说尚未找到满足全部条件的实际应用阶段；AMR/粒子只是首选但不预设满足。 | `C4.md:13`、`:97`；`innovation-council18:50`、`:92`；`A4.md:7`；`D4.md:3` | 真实依赖图/最后一个必要输入到达时刻 |
| 翻译关键路径 | 在适用强基线下独立确认翻译仍影响完整阶段 | **未核到**。必须“在适用强基线下独立确认”；A4 要求暖页表+LLC+页排序之后计算。 | `C4.md:78`；`A4.md:72–80`；`D4.md:8` | 实际映射、TLB/PTW/LLC 条件、页面覆盖 |
| 跨页重排空间 | scatter/gather 地址可并行发出，重排/合并有收益；不是真串行 pointer chasing | 机制存在：DX100 明确 array-based indirection 可重排，pointer chasing 不支持；但真实应用是否同时有翻译瓶颈未核。 | `dx1002025.txt:479–495`；`L3_data_lifecycle.md:196–210`；`research-landscape:20`、`:49` | 真实索引流、跨页比例、复用距离 |
| 无可行普通页池/大页轻易消除 | 默认 arena/大页/池化/pack/软件分组不能消掉剩余翻译成本 | **未知**。C4 要求改写成“稳定映射使剩余翻译成本无实际影响”才淘汰；D4 警告默认 arena+大页可能隐藏。 | `C4.md:78`；`A4.md:72–86`；`D4.md:8`；`innovation-council18:92` | 实际默认分配粒度、huge page/pool 配置 |

**候选真实应用线索，但均未核成“同时满足”**：  
- AMR/粒子应用阶段：C4 首选但不预设。`C4.md:97`  
- PagedAttention block-table gather：D4 作为最低成本首判候选之一，但 Tiara 已说明 PagedAttention 最优批处理基线是 2 RTT，不是朴素 160 RTT。`D4.md:3`、`:20`；`L3_data_lifecycle.md:150–157`；`research-landscape:105`  
- DX100 demo 程序：不等于公开原始索引流。`C4.md:97`

因此不能写成“地图中不存在真实应用”，只能说：**在已提供材料中未核到；需要真实 trace 才能判。**

---

## 3. 简单对照需要的成本与同信息合同；哪些历史主张已普通复现

### 3.1 同信息合同

| 合同项 | 要求 | 证据 |
|---|---|---|
| 信息同权 | 候选与普通对手使用同一 job/context/window/完成信息；普通运行时可能已有同样任务图；软件对手必须使用同样信息并支付传递、调度、通知成本。 | `C4.md:76`；`translation-sharing-window:49` |
| 合法信息 | 地址产生时刻、真实组依赖、别名/写权限、元数据提取和 fan-out 必须收费；不能用事后全 trace/未来 top-k。 | `C4.md:88–90` |
| 普通可行动作 | 允许普通 translate-first scheduler、hit-first/RR、bounded lookahead、任务感知保留；普通 page barrier/lag cap＋用途保留/有限 prefetch。 | `translation-sharing-window:49` |
| 反事实 | 报 T00 调优基线、T10 仅理想化翻译、T01 仅理想化组完成通知、T11 两者同时理想化；不能把两个独立加速百分比相加；去除通知开销不能让索引提前生成/数据提前返回/凭空增加消费者资源。 | `C4.md:82` |
| 停止 | 若普通组合同决策、同完整成本/性能，不立独立机制；若仅页排序、普通小任务或已有事件接口即可达到相同结果，C4 机制主张终止。 | `translation-sharing-window:51`；`C4.md:76`、`:101` |

### 3.2 必须收费的成本账本

| 项目 | 必须记录/收费 | 防止的错误归因 | 证据 |
|---|---|---|---|
| 信息与合法性 | 索引产生时刻、真实组依赖、别名/写权限、元数据提取和 fan-out | 把事后全 trace、免费任务图、未来 top-k 交给候选 | `C4.md:90` |
| 存储与翻译 | 实际页大小/覆盖、TLB 组织及共享、页表/LLC 命中、miss 合并、映射维护 | 小页等于慢；稳定映射等于零翻译；少 PTE 流量等于无关键路径 | `C4.md:91` |
| 状态预算 | 索引/行/页表、分桶、完成位、双缓冲、shadow copy，统一按字节统计 | 把更多 SRAM 或更多在途当接口收益 | `C4.md:92` |
| 数据路径 | 有用/实际读写字节、源去重、fan-out、pack、缓存维护、链路与排队 | 只数源读取，漏掉输出、复制和数据服务争用 | `C4.md:93` |
| 完成与消费 | 通知、CPU 提交、kernel/任务启动、同步、消费资源、阶段边界 | 提早拿到数据但没有真实提早完成工作 | `C4.md:94` |
| 正确性/生命周期 | 读阶段稳定映射；阶段间按既有完成与失效协议释放/复用 | 将局部 drain、无 ATS 或普通完成误写成新的安全保证 | `C4.md:95` |

### 3.3 提供材料中已可称“普通等价/普通强对手”的历史主张

> 严格说，以下多是“提供材料中的普通等价结论/二手强对手”，不是本轮新复现实验。

1. **remaining-use/refcount 的取交/OR**：模型内普通 interval/refcount 逐项同 decision，boolean 完全相同；不能把“注册 job 剩余用途取交/OR”本身当独立机制创新。  
   证据：`docs/translation-sharing-window-20261008.md:41–43`。  
2. **Depth4 窗口扫描挡洞**：现有普通窗口组合 scan 能挡洞，无需另存 hole bitmap；但若要求 short parent 在 R 前推进，需要额外成功证书或扩大合法结果资格，两方同计。  
   证据：`docs/translation-sharing-window-20261008.md:37–39`。  
3. **Milk/Propagation Blocking 软件批量+重排**：DX100 相关工作已把“先收集成 cache-fitting batch，再重排为 sequential DRAM”列为先行软件方案，并指出中间结构 timing/space/cache pollution 代价。  
   证据：`out/research-landscape-20261005/papers/dx1002025.txt:1308–1316`。  
4. **页分组/软件排序是直接对手**：A4 已撤回“DX100 首次让整 tile 在翻译前可见”，因为 Milk 式软件早已看见整批并按 TLB reach 分组。  
   证据：`A4.md:91`；`C4.md:28–30`。  
5. **SIMT walk 调度中的 SJF/batch**：SJF 与 batching 是普通调度概念；论文贡献是把它们用于 page walk，并加 instruction ID/score。具体 scheduler 不是普通基线，但“调度顺序重要”已被该文建立。  
   证据：`walk_scheduling2018.txt:287–309`、`:414–489`。  
6. **consumer 组信息不是自动新信息**：普通运行时可能已有同样任务图；若仅页排序、普通小任务或已有事件接口即可达到相同结果，机制主张终止。  
   证据：`C4.md:76`。  
7. **DX100 tile size 敏感性**：1K→32K tile 时 1.7×→2.9×，说明小批/大批有普通工程 tradeoff；但这不等于 C4 新意。  
   证据：`dx1002025.txt:1096–1107`。  

**尚未普通复现/未解决的**：真实应用上四条件同时成立、同信息 T00/T10/T01/T11 的完整阶段比较、consumer-first 与翻译瓶颈的联合收益、普通组合是否达到候选。  
证据：`docs/translation-sharing-window-20261008.md:7`、`:15`；`C4.md:103`；`A4.md:68`；`D4.md:20`。

---

## 4. 证据导航：file:line + 原文页节 + 网址 + 下一条需浏览证据

### 4.1 内部文档

| 文件:行 | 页/节 | 关键内容 | 下一条需浏览证据 |
|---|---|---|---|
| `docs/innovation-council18-20261006.md:50` | §3 26 主张去留表 C4-P1 | C4-P1 条件保留；四个应用条件尚未同时证实；需对抗 Milk、DX100、普通流水 | 真实应用 trace + 同信息普通对照 |
| `docs/innovation-council18-20261006.md:92` | §6 三份具体问题证据卡 | 搬运“完成一批”是否晚于消费者真正所需；要求真实阶段地址产生、实际映射、消费依赖图；给大页/池化、Milk式分组、小tile、DX100式重排成本；同时间线联合反事实 | 采集/取得真实阶段 trace |
| `out/innovation-council18-20261005/round2/C4.md:13` | §1 裁决表 C4-P1 | 收窄后保留为否证对象；不进入 RTL；尚未找到满足全部条件的实际应用阶段 | 真实阶段门槛 |
| `out/innovation-council18-20261005/round2/C4.md:74–84` | §6 回应 A4/D4 | 消费组信息不是自动新信息；两个门槛分别成立；T00/T10/T01/T11；门槛只是筛选建议 | 同信息合同与反事实实现 |
| `out/innovation-council18-20261005/round2/C4.md:88–95` | §7 共同成本账本 | 信息/合法、存储翻译、状态预算、数据路径、完成消费、正确性生命周期 | 逐项收费表 |
| `out/innovation-council18-20261005/round2/C4.md:97–101` | §7 先做资料/接口检查；停止顺序 | 首选 AMR/粒子但不预设；DX100 artifact 不等于公开索引流；停止顺序①–⑤ | 真实应用路径与默认 arena |
| `out/innovation-council18-20261005/round2/C4.md:109–113` | §8 来源边界/决定待核 | Milk/MMU-aware DMA/AMAC 原文与已读范围在 C4 source notes；待核顺序；最终修订 | 读 C4 source notes、Milk 全文、Neighborhood-Aware Translation、Terminus/Phloem/MAPLE、MMU-aware DMA |
| `out/innovation-council18-20261005/round2/A4.md:7` | 裁决表 C4-P1 | 只作测量问题，不立机制；可能被软件分组、小tile+双缓冲吸收；应用门槛未过 | 真实应用门槛 |
| `out/innovation-council18-20261005/round2/A4.md:57–60` | §(2) 改进 C4-P1 | 消费组早开工与翻译分组拆成两个独立门槛；前者是任务图/数据流调度旧题；15% 门槛须在暖页表+LLC+页排序后算 | 真实 trace 统计 |
| `out/innovation-council18-20261005/round2/A4.md:72–86` | §(3) 首个判别、停止 | 应用门槛：若 gather 稳定落在大页/池，A4/C4/D4-P1 一并停；离线统计不同页/PTE 线/数据线、LRU、页排序 K 曲线 | DX100 artifact 索引流可取得性 |
| `out/innovation-council18-20261005/round2/D4.md:3` | 裁决表 C4-P1 | 收窄保留为否证对象；若 arena/大页或软件分组+小tile近上界则致命；最低首判 AMR/PagedAttention trace | 真实 trace + 默认 arena+大页 |
| `out/innovation-council18-20261005/round2/D4.md:7–10` | C4-P1 分析 | 最强形态是翻译页集合+真实消费组开工条件共同约束批次；应用真实性最弱；Milk/MMU-aware DMA/AMAC 是强对手；最强普通组合 | Milk 全文、AMAC、MMU-aware DMA 全文 |
| `out/innovation-council18-20261005/round2/D4.md:20` | 首个判别 | 零硬件 offline：DX100 artifact 12 索引流＋AMReX/PagedAttention trace；阈值杀 A4/C4 | 公开索引流是否可得 |
| `docs/research-landscape-20261005.md:20` | §2 六条研究线总表 间接访存 | 真串行地址依赖不同于可并行 scatter/gather | 真实应用分类 |
| `docs/research-landscape-20261005.md:49` | §3 卡点5 | 间接访存分真串行链与可并行散读；DX100 不支持 pointer chasing；Tiara PagedAttention 160†/2‡ | Tiara Table 1/§2.1/§2.2 |
| `docs/research-landscape-20261005.md:92` | §5 C I/O 软件管理 | IOTLB wall：hugepage mapping 缓解 | 实际大页配置 |
| `docs/research-landscape-20261005.md:104` | §5 D 间接访存 | DX100 行：明确不覆盖 pointer chasing；gem5+Ramulator2 | DX100 artifact |
| `docs/research-landscape-20261005.md:105` | §5 D 间接访存 | Tiara 行：预印本；PagedAttention 最优批处理 2 RTT | Tiara 全文 |
| `docs/research-landscape-20261005.md:180` | §8 强基线 | 间接访存/布局搬运必须对照软件预取/重排、PIM、近 NIC；区分真串行链与可并行散读 | 具体强对手 |
| `docs/translation-sharing-window-20261008.md:7` | §0 边界 | 一次有限逻辑模型；1 run、9 trace/capacity、36 records；无 DUT/生产 RTL/Linux/tests/BIT/JTAG/上板 | 真实执行证据 |
| `docs/translation-sharing-window-20261008.md:13–31` | §2 实际未命中矩阵 | 模型矩阵；ordinary≥oracle≥24；PTE_reads=3×misses 仅 Sv39 无 PWC；registered-use 只用两登记 stream 固定规则 | prepared parent/window 真实 trace |
| `docs/translation-sharing-window-20261008.md:33–39` | §3 事件、空洞与普通窗口 | 停止把“登记 job 剩余用途取交/OR 再 dead-first”当独立算法；Depth4 扫描挡洞；short-parent hole | DUT 运行该握手例 |
| `docs/translation-sharing-window-20261008.md:41–43` | §4 剩余用途的普通等价 | 普通 Remaining/need OR/refcount 与普通 interval/refcount 完全相同；不能把该 boolean 当独立机制 | 真实 trace 是否仍有差异 |
| `docs/translation-sharing-window-20261008.md:45–53` | §5 下一候选与停止条件 | 下一候选：译址 materialization 前沿约束共享源超前；允许普通 translate-first、hit-first/RR、bounded lookahead、任务感知保留；同信息同成本则停；单一下一动作构造真实可达 trace | 真实可达 trace |
| `out/research-landscape-20261005/workers/L3_data_lifecycle.md:106–114` | 卡 6 DX100 | DX100 机制/边界/评估 | DX100 原文 |
| `out/research-landscape-20261005/workers/L3_data_lifecycle.md:196–210` | §3.4 关键分界 | gather/scatter 可并行；pointer chase 不可；multi-level translation/conditional multi-host 不可 | Tiara/IMPICA/DX100 原文 |
| `out/research-landscape-20261005/workers/L3_data_lifecycle.md:252–257` | §5.2/5.3 强对手与缺环 | DX100 的 DMP、GPS、BypassD、vAttention、Tiara、WIC 强对手；动态内存可预知度等缺环 | 相关全文 |

### 4.2 原始论文/网址

| 对象 | 本地/原文页节 | 网址 | 已核内容 | 下一条需浏览证据 |
|---|---|---|---|---|
| DX100 | `out/research-landscape-20261005/papers/dx1002025.txt`；§1、§2.1、§3.2、§3.4、§3.6、§4.2 Limitations、§5、§6、§7 Related Work；行见 1.2 | `https://arxiv.org/pdf/2505.23073v1` | 16K 可见窗口；Row/Word Table 重排合并；huge pages+256 TLB；不支持 pointer chasing；2.6×/2.0×/3.9×/2.7×；tile size 敏感性；Milk/Propagation Blocking 二手描述 | DX100 artifact 公开索引流是否可得；真实应用是否 huge pages 映射 |
| SIMT Walk Scheduling | `out/research-landscape-20261005/papers/walk_scheduling2018.txt`；Abstract、§III、§IV、§V、§VI Discussion；行见 1.3 | `https://guilhermecox.github.io/dw/sshin-gcox-isca18.pdf` | instruction ID+score；batch 同 instruction；SJF；aging；30%/41%；stall 23%；latency gap 37%；page walk 21%；灵敏度 | 与普通 SJF/batch 的同资源对照已在该文；C4 需另核 consumer 开工联合 |
| Tiara | 未提供全文；仅 L3 二手卡；Table 1/§2.1/§2.2 待读 | `https://arxiv.org/pdf/2606.13708v1` | PagedAttention 160†/2‡；embedding table lookup 可并行 scatter-gather；pointer chase/multi-level translation 不同；FPGA 周期模型 | 读 Table 1、§2.1、§2.2、评估性质 |
| Milk | 未提供全文；仅 DX100 §7 二手描述；C4/A4/D4 转引 | 提供材料未给 URL | DX100 对 Milk 的描述：cache-fitting batch、重排、中间结构开销；C4 笔记称 Milk §3/§5.2 有容量感知容器/协程 | 读 Milk 原文 §3、§5.2；读 `out/innovation-council18-20261005/sources/C4-source-notes.md`（未提供） |
| Neighborhood-Aware Translation | 未提供全文；仅 C4/A4 提及 | `https://www.csa.iisc.ac.in/~arkapravab/papers/micro2018_neighborhood.pdf` | 合并相邻翻译、提高 page walker 利用率；地图标为 GPU 模拟 | 读全文，精确覆盖翻译合并/页排序 |
| ASPLOS 2014 TLB-aware wavefront | `docs/translation-sharing-window-20261008.md:57` | `https://courses.grainger.illinois.edu/ece598ms/fa2021/papers/paper233.pdf` | TLB miss/victim tags/hit 的 LRU 深度控制 warp 调度；按共享 PTE 历史重组线程 | 读 §7.2/8.2，核是否已覆盖“以翻译信息调 producer” |
| Locality Descriptor 2018 | `docs/translation-sharing-window-20261008.md:59` | `https://people.inf.ethz.ch/omutlu/pub/LocalityDescriptor-Cross-Layer-GPU-Data-Locality-Abstraction_isca18.pdf` | 按已知共享范围协调任务、缓存保护和预取距离 | 读全文，核是否覆盖多 DMA 精确前沿合同 |
| Orchestrated DAC2023 | `docs/translation-sharing-window-20261008.md:59` | `https://libingyao.github.io/assets/files/DAC2023.pdf` | 按翻译压力放置新 TB，并做分区/共享 | 读全文，核是否覆盖联合调度 |
| IMPICA | 未提供全文；L3 二手卡 | `https://pdl.cmu.edu/PDL-FTP/associated/16iccd_impica.pdf` | pointer chasing 串行依赖、PIM 解链、RPT | 若需区分真串行链与 scatter/gather，读 §1/§3.1/§7.3 |
| RISC-V IOMMU queues | 地图引用 | `https://docs.riscv.org/reference/iommu/v20260222/iommu_in_memory_queues.html` | IOTINVAL/IODIR/IOFENCE/ATS.INVAL；PR/PW 覆盖边界 | 仅在正确性/失效合同需要时读 |

---

## 5. 可导航摘要与接受检查

### 5.1 给 Codex 的短结论

- **C4-P1 保留的是问题，不是机制**：翻译页集合×真实消费组开工条件共同约束批次；四条件未同时证实；不进入 RTL。  
- **真实应用状态**：未核到同时满足四条件的应用；写未知。候选 AMR/粒子、PagedAttention block-table 都只是待核，不是实际 trace。  
- **最强普通对手**：Milk式软件批量/容量感知容器、DX100式重排、小 tile+双缓冲、页排序、普通 translate-first/bounded lookahead、consumer 组普通任务图调度。  
- **已普通等价**：remaining-use/refcount OR 同 interval/refcount；Depth4 扫描挡洞；Milk/Propagation Blocking 批量重排。  
- **BIS**：提供材料无定义，不能列。  
- **下一步唯一动作**：真实阶段地址产生、实际映射、消费依赖图；同时间线 T00/T10/T01/T11；无差异则停。  

### 5.2 Codex 可直接核对的接受检查

1. 核 `docs/innovation-council18-20261006.md:50`、`:92` 是否确实写“四个应用条件尚未同时证实”。
2. 核 `out/innovation-council18-20261005/round2/C4.md:13` 是否确实写“不进入 RTL”“尚未找到满足全部条件的实际应用阶段”。
3. 核 `out/innovation-council18-20261005/round2/C4.md:76–84` 是否确实写“同信息”“两个门槛”“T00/T10/T01/T11”。
4. 核 `out/innovation-council18-20261005/round2/C4.md:88–95` 成本账本六项。
5. 核 `out/innovation-council18-20261005/round2/C4.md:97–101` 停止顺序：无真实可提前消费组停 C4 接口；同信息简单策略达到候选停新机制；只有人为碎片/单缓冲/未校准 FPGA 降级条件测量。
6. 核 `out/research-landscape-20261005/papers/dx1002025.txt:432–452` huge pages+256 TLB；`:479–495` 不支持 pointer chasing。
7. 核 `out/research-landscape-20261005/papers/walk_scheduling2018.txt:291–309` SJF；`:414–489` instruction ID/score/batch。
8. 核 `docs/translation-sharing-window-20261008.md:41–43` 普通 interval/refcount 同 decision；`:37–39` Depth4 扫描挡洞；`:53` 单一下一动作构造真实可达 trace。
9. 检查 BIS：在提供文件中无展开；若仓库其他文件有，需 Codex 补行号。
10. 缺文件依赖：`out/innovation-council18-20261005/sources/C4-source-notes.md`、`PRIMARY_CHECKS.md`、Milk 原文、Tiara 原文、MMU-aware DMA、AMAC、Neighborhood-Aware Translation 全文未在本次上下文中提供；不能凭题名判决。

### 5.3 不足以免除/足以淘汰的界线

- **不足以免除**：只凭“consumer-first”“页排序”“lookahead”等关键词相似；只凭一个摘要、一个非目标配置的 2% 开销、一个 LLC 改善算例。  
  依据：`docs/innovation-council18-20261006.md:83`。  
- **足以淘汰当前版本**：  
  ① 找不到有实际成本且可获取关键证据的目标应用阶段；  
  ② 有效大页/池化/pack/软件分组已消除完整阶段空间；  
  ③ C4 没有真实可提前消费组；  
  ④ 同信息普通策略达到候选；  
  ⑤ 只有人为碎片、单个缓冲点或未校准 FPGA 现象。  
  依据：`out/innovation-council18-20261005/round2/C4.md:101`。  

---

## 6. 不确定性与未决项

- 本轮未浏览、未执行、未写文件；所有结论限于提供文本与其中转引。
- Milk、小 batch 软件流水、BIS、consumer-first 没有完整原文；BIS 在提供内容中无定义。
- `PRIMARY_CHECKS.md`、`C4-source-notes.md`、Milk/Tiara/AMAC/MMU-aware DMA/Neighborhood-Aware Translation 全文未提供。
- `docs/translation-sharing-window-20261008.md` 只是一次有限逻辑模型，不是 DUT；不能把模型 24 miss、36 policy records 或 DP 小比较写成真实应用或硬件结果。
- DX100 是 gem5+Ramulator2 评估；SIMT Walk Scheduling 是 gem5 评估；Tiara 主要是周期/分析模型加有限实物点。不能跨论文排名或外推成本平台结论。