# 多 DMA 输出选择容量与翻译需求管理

stage91 / 2026-10-09 / 交接稿：[mechanism-design-handoff-20261009-round04.md](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)

本轮只做机制设计、最近前作定向阅读与手算边界例；不做模型、RTL、Linux、构建或上板。主线 L-M 是条件性机制设计优先级，备选 L-W，取消只作组件与对照，不把三者写成三个已成立的独立创新。权威裁决取 [root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-result-acceptance.json)，机制卡取 [root-frontier-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-frontier-design.json)。

## 1. 给博一的问题陈述

一个 COPY 由三部分组成：源数据从真实地址读出；目的端不是固定地址，而是一个可互换的合法输出集合；最后把 64B 源真实写进集合中某个合法槽位。问题在于：如果源填充进 IOTLB 时按普通策略驱逐，可能把多个父任务同时还需要的那一个热槽赶掉，使原本一次就能完成的输出翻译被迫再做一次冷走表。也就是说，每个任务各自的命中数都 ≥1，但整体同时可分配的能力下降了。这正是本轮要处理的对象。

## 2. 主线 L-M

机制：源走表成功时，在填充点先算出插入该源结果后的实际替换掩码，只选那些不破坏已接纳、仍未绑定 COPY 父任务同时输出选择容量的普通 victim；若不存在这样的 victim，就走完整普通权限路径把该源 READ 请求经正常权限检查得到的地址翻译结果（PTE/翻译响应）直接返回原请求，并保留真实数据 READ，不占用共享 IOTLB 存储。源工作不能跳过，普通权限、AXI 契约与真实读写字节必须保留。

图模型：左顶点是已接纳未解决的输出义务；未绑定任务 j 连到当前 FREE 且被其描述符允许的槽位；已绑定未翻译任务 j 只连到自己选中的 COPY 槽位；右顶点只在其精确 4KiB 翻译真正常驻并被追踪时才存在。r(H) 为最大匹配大小。匹配与 Hall 是标准数学，不作为新定理声明。两任务判定：cV、cA、cU 分别是 V、A 及其并集的不重复邻居槽数；r=2 当且仅当 cV≥1 且 cA≥1 且 cU≥2。

决策/状态/成本三分：

- 决策点：源走表成功、形成 resolved next metadata 之后、真实替换发生之前。
- 状态：实际稳定 pool 上下文、每父允许槽位 bitmap、精确 FREE 或父拥有的绑定槽状态、真实 IOTLB fill/replace/invalidation 事件、被对应物理头接受的普通 WRITE 翻译响应。
- 成本：所有 round03 的父任务/池/所有权/索引/回调/payload/内核/completion 成本继续计。本轮命名新增 146bit 允许集与状态记录、15bit 三个精确不重复邻居计数、158bit 原请求结果续传，合计 319bit，向上取整 40B；如果 160bit resident 索引随本规则新引入，则 319+160=479bit，取整 60B，479 是包含 319 及其新增 160bit 的上位小计，319 与 479 不能再次相加。这些只是命名字段小计，不是净面积；不同契约与子集可能复用字段，不能相加成净优势。

## 3. {X,Y}/{Y} 例

V 接受 {X,Y}，A 只接受 {Y}，有两个不同的 FREE 热槽 {X,Y}。一次源填充驱逐 X 后，V 与 A 各自的本地命中都还是 1，但 cV=1、cA=1、cU=1，秩从 2 降到 1；A 必须用 Y；如果消费者要拿齐两个输出才释放槽位，两份结果须同时保留，V 就必须使用已变冷的 X。如果允许先消费 A、释放 Y 再给 V 使用，等待并复用 Y 也是必要强对照，要计入真实等待、消费与同步成本。精确数学：驱逐前 cV=2、cA=1、cU=2、r=2；驱逐后 r=1。所以每个父 h≥1 的局部计数会漏掉这次容量损失。因此重叠子集下必须做完整 FREE 图匹配准入，标量 free credit 不够，精确元数据与完整权限路径都要付费。

量化边界：若被移除的匹配造成一次未隐藏的目的走表成本 M，guard 与 forward 开销为 delta，则可能的端到端增益是 M−delta。按假设 M=20、delta=3 得到 17 个服务单位。注意这只是决策边界手算，不是合法完整原生轨迹或实测加速；任何隐藏走表、丢失的未来源复用、真实扫描/扇出/频率成本都要扣掉。

邻居情形：A 改成 {X,Y} 时驱逐 X 只能保秩 1，除非第三个热 Z 两边都允许；去掉 A 的未解决义务后一个热输出就够；源 S 反复使用而无用输出稀缺时，保输出选择可能带来额外源走表而变差；只有一个真实 FREE 槽或全冷时秩 0/1 无法创造容量，必须走普通冷回退；重复 cache 行不产生两个槽位；失效或所有权元数据不确定会作废保持声明。

## 4. BIND 与义务保持

分配槽位时，先在全部合法空闲槽中求一个能覆盖所有已准入任务的分配，再在这个约束下尽量使用热槽。不能从热槽匹配里随便取一条边。例如 V 可用 X 热槽或 Y 冷槽，A 只能用 X；先让 V 占 X 会让 A 无输出，合法分配应是 V→Y、A→X。所有其它池分配路径也必须保持这个可完成性约束。

BIND 时原子地消耗真实槽位与信用，把该任务的集合替换成它绑定的单元素集；普通输出绑定后直到 WRITE 翻译被稳定的物理请求头实际接收前，该单元素义务必须保持。在 FREE→COPY 时清除保护是错的：那一瞬间填充保留的源结果可能把选中的目标赶掉，重建被避免的那次走表。只有成功 WRITE 翻译被接受进稳定物理头，才退役该输出的翻译义务；所有权、payload 与 completion 继续到各自的真实 W/B 和消费者释放边界。索引、秩与所有权永远不授权权限：no-fill 路径必须跑与普通成功路径相同的 context、stage、privilege、RW、A/D/fault 与响应路由；已发表的 AXI 请求保持稳定并完整，不被租约、hint 或未接受的 VALID 推断为权限。

## 5. 备选 L-W

集合目标 WRITE 可以在选择具体叶子前先读公共 radix 前缀。在非叶子 PTE 响应之后、下一内存 AR 首次 VALID 之前，可以在已校验前缀内选一个新合法槽位，避免为未选候选去取叶子。合同：新的 PREPARE_WRITE_SET 父任务不可变，不是可变普通 AW，也不是普通固定 IOVA 翻译请求；每个已发出的 PTE AR 自首次 VALID 起不可变且必须完成，只有尚未发出的内部续传才能换索引。

手算例：三级走表每级 10，完整 X prepare 后 Y 带公共前缀付费 root+middle+leafX+leafY=40；集合前缀选择付费 root+middle+leafY=30 加 delta；取消 X 于叶子之前再配一条 PWC 与晚选择，同样是 30 加重启/查找 delta。这只是相对完整 X 的手数差。由于前缀缓存与取消/重放都已发表，且可中断 prepare+PWC 能在 PTE 数上匹配，就不宣称新的路径缓存算法；只有当目标导向续传在可达轨迹里、同等完整预算下消掉一项实际接口或路由成本，且最近前作未覆盖该集合操作时，专门架构才可能成为贡献。

## 6. 取消只作组件

跨地址的真实租约撤回可以在未发表 PTE AR 之前停掉可选孤儿后缀。Agent 的 19/51 与 22/54 只在它声明的、带共同已付费前缀的条件后缀排程下被接受，不是原始起点胜利；控制方可以从原始工作负载出发改动或重叠该前缀。裸取消、优先级、hot-first、匹配都不是贡献；已发表的可中断低优先级预取在 A 早到时恰好复现该早 A 情形，因此取消路径更适合作为根方向里公共前缀/晚叶选择的实现支撑，而不是另起一个算法名。

## 7. 最近前作与完整强对照

- 公共前缀缓存已被覆盖：[Translation Caching: Skip, Don't Walk，ISCA2010](https://cs.rice.edu/CS/Architecture/docs/barr-isca10.pdf)。
- 低优先级预取走表可被高优先级真实请求终止或冲刷：[US9542332B2](https://patents.google.com/patent/US9542332B2/en)。
- 未完成地址共享前缀的比较与取消/重放冗余走表已公开：[US11210232B2](https://patents.google.com/patent/US11210232B2/en)。
- no-fill/共享缓存 bypass 与请求保持缓冲已存在：[Dead Page and Dead Block Predictors，HPCA2021](https://www.csa.iisc.ac.in/~arkapravab/papers/hpca21_DOA.pdf)。
- 重装条目限时保护：[Dead on Arrival，2026 预印本](https://arxiv.org/pdf/2606.00486)，录用未验证。
- 时间性预取与 PTE 内嵌元数据：[Trail，2026年10月6日预印本](https://arxiv.org/pdf/2610.08483)。
- 共享 TLB 填充 bypass 与旁路缓存：[MASK，ASPLOS2018](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf)。

完整强对照包括：精确 set-aware pin+bypass；普通全 WRITE prepare 并保留结果；无条件源 bypass；源/目的角色 bypass；带正确缓冲成本的复用历史 dpPred 式预测器；MASK token/选择性准入；Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine (ICCD2018), DOI 10.1109/ICCD.2018.00052, [论文原稿](https://arxiv.org/pdf/1808.09751) 的预取与加锁；DAMN/io_uring/池回收；SDM overlap；普通分配器局部性；local k-choice guard（不相交池的更便宜消融）；更大普通 IOTLB 容量、合法 mask 重划分、hugepage 与页局部池布局。它们都要在相等完整预算下执行；两份小例与两路/P2 草图不构成对 16 项设计的证据，不允许强制 PLRU victim 或人为单子限制。

## 8. 机制新颖性、独有系统收益、可落地性

机制新颖性：只有 COPY 选择与真实所有权耦合的准入契约这一条仍是具体候选。Matching/Hall 定理、hot-first、源角色 bypass、死页预测、pinning、shadow cache、延迟放置、PWC 前缀复用、低优先级走表抢占/取消/重放、通用 output-pool API 单独都不新。若某个已发表主设计精确覆盖了这个 COPY 选择/所有权耦合操作，其覆盖动作就不能再单独声明；反过来，等价地实现整套新规则并不自动等于已发表覆盖，也不自动构成反证，要用真实可执行对照比较。

独有系统收益：现有手算例没有建立它。因为精确 set-aware pin+bypass 可以复现展示的局部动作与计数，all-prepare 也可能直接绕过这个边界，所以这些局部手例只能证明逐任务计数不足与一个具体新目标，不能证明存在独立更快的算法。

可落地性：受限的一源 no-fill 路径加现有 pool-parent 在工程上可信，但需要真实 permission/result 路由，以及 BIND 到 accepted 普通 WRITE 的义务交接；当前源码直接读过且尚未实现这些接口，见 [rv_iommu_tl_wrap.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_tl_wrap.sv) 与 [rv_iommu_iotlb.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv)、[rv_iommu_ptw.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv)。

## 9. 七组下游场景与推进/停止

- M1：重叠 {X,Y}/{Y} FREE 热例，采用消费者拿齐两个输出后再释放的真实共同持有场景；另测允许等待释放并复用 Y 的对照，从真实父/源/bind 历史比较本地计数、匹配分配、完整匹配容量准入与精确 pin；必须允许普通控制立刻合法早 BIND/提前 WRITE 准备，不能拖延对照；真实源完成、头容量背压或晚语义约束须从原始轨迹产生；若早绑定等低成本控制消除全部重叠，就停止单独 matching 准入收益。
- M2：加第三个有用热槽或去掉义务，确认保秩插入与未来真实源复用。
- M3：BIND→普通 WRITE 应答间隙、重复 cache 行、同边所有权/填充事件，检查不提前解除见证。
- M4：源复用与不复用，真实 16 项容量、允许掩码、resize/借用与等预算普通容量。
- W1：前缀响应后、叶子 AR 前槽位资格变化；比较完整 prepare、可中断 prepare+PWC、集合前缀续传。
- W2：叶子 AR 首次 VALID 后或共享前缀之外释放；要求已发表请求不变并走普通回退。
- C1：A 已在等待与 A 尚未到达时新备选成功；显示 2017 前作已能复现哪些取消动作，并计入未来 X 复用。

推进：合法完整轨迹加差异明确的有用动作/目标加可信已付费实现，就足以进入最小模型；只有 V/A/成本前沿在独立强对照下存活，才升 RTL。停止或重设计：如果源 prepare+普通分配/pin，或 cancel+PWC，在候选正区域内以相同或更低完整成本匹配，应停止其单独性能算法声明，只保留有支撑的集成/契约或重设计问题；如果没有真实共享上下文与所有权时序，先改工作负载或契约，再谈收益。

## 10. 执行与归属

本轮执行：0 模型运行、0 RTL 编辑、0 Linux 编辑、0 构建、0 上板、0 Claude 调用；数字均为手算或字段小计。Codex root 与两个关键设计席负责机制设计及核查；DeepSeek 起草中文，Codex root 审核保存。不作 venue 或分区承诺。
