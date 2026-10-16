# Codex 独立研究席：两套规则推演与强普通反证

2026-10-08；作者及推演为 Codex 子 agent `innovation_rules_40`。本稿为研究筛选，不是实现、仿真、面积或新颖性验收。未调用 DeepSeek 或 Claude，未运行 DUT/build，未改生产代码。根 agent 负责汇总更新 HANDOFF/CURRENT_STATUS。

## 结论

**这次两套具体微架构规则都不能按当前证据立为独立机制。** 一致写序可由普通序号队列表达；细粒度预算加排空义务与工作史已有规则重合，又受到普通成对预约和 AXI-REALM 等前史约束。不应为取得收益人为添加本平台没有实现的 LTI 完成寿命约束。

保留一个较明确的分析切口：**读写 burst 的错位分段可能让一次预算关闭后的排空范围贯穿整个 copy，有限单 burst 长度本身不提供有限 copy 排空界。** 下文给出受限条件下的推导、反例与可证伪检查；共同分段、普通源子片和额外尾部缓冲都可能达到相同保证。因此若做论文，它首先是容量/分段/隔离响应的性质问题，而不是已经证明的新调度。

当前 §39 最强普通源物理读旁路，在同原窗口4/父槽4/48B FIFO/CONF1/各32KiB时均值18975，后置18824，旧5927差距剩151。不能用这个151或原23.95%支持下面任一新颖性声明。`CURRENT_STATUS.md:3–9`是现有实验事实。

## 1. 已直接核的实际接口

| 关键源 | 本次直接读到的边界 | 对设计的限制 |
|---|---|---|
| `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_trans_ctl_wrap.sv:693–725` | AWAIT_RESP 一直 ready 消费译址；COMPLETE 在物理 AR/AW 握手后回 IDLE。 | 不能把 TC COMPLETE 写成等 DMA RLAST/W retirement 的长期结果锁。 |
| `cva6/corev_apu/riscv-iommu/rtl/transaction_controller/rv_iommu_banked_ctl.sv:246–277` | 当前 W owner 与 AW 计数限制 source AR 选择；代码注释明确错误来源读可能堵住共享 R，连同 PTW。 | 当前有真实 R→W→共享返回依赖；不是 PTE/payload 共享一个 byte-credit 池。 |
| `cva6/corev_apu/riscv-iommu/rtl/ext_interfaces/rv_iommu_ds_if.sv:77–108` | DDTW/PDTW/PTW/CQ/PQ/MSIPTW/MRIF 七路 AR 经独立仲裁及 spill 发往 DS。 | 页表读取和命令读取确有共同 DS 入口，数据 comp 是另一发起端口；最终共享 DDR 不意味着跨 ID 必须 FIFO。 |
| `cva6/corev_apu/fpga/src/dma_research_policy.sv:284–345` | 支持译址次数、miss、PTW beat、TC REQ_TRANS/AWAIT_RESP 周期预算；COMPLETE 不收费，周期末无条件 refill 避免空预算停住。 | 既有的是反馈和计费机制；给不同费用起名字不构成新创新。 |
| `cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:588–596`；`cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv:1274–1289` | ResearchPort 的 ADMIT 在 backend idle 后才 offer 下一 copy。生产当前主要对 A job 供给门控。 | 下文细粒度限流是拟议功能；不能称现生产预算已经触发内部排空问题。 |
| `cva6/corev_apu/iDMA/src/idma_legalizer.sv:137–221`、`:450–461` | src/dst 的 burst/page 边界由各自地址和配置计算；decouple_rw 时独立推进，共同配置时受共同最小边界约束。 | 可以研究真实可配置的错位 R/W 分段；共同分段必须作为强普通基线。 |
| `out/prepared-source-bypass-20261008/multi_actual_dma_top.sv:549–575`、`:867–943` | 普通已经可以缓存准备结果，物理源 AR 真绕 TC，单全局 child route 固定至 RLAST。 | 不能用“有 ready 结果仍堵在 TC”作为新方案的基础反例。 |

根已核的生产 ATS0、CQ PR/PW drain TODO、Linux 普通 IOFENCE PR/PW0等边界，本席接收为协作证据，没有重复全部阅读；最终以根直接源核报告为准。

## 2. 方案 A：细粒度关闭预算时只准入排空闭包

### 2.1 拟议规则的完整接口

目标场景：一个周期性小 copy 对时延敏感，另1–3个 DMA 做长时间 bulk copy。普通 job gate 可控制下一 job，却不能在已启动长 job 内产生细粒度响应。试图移到 source/AW 子片准入处时，关闭所有新事务可能把已经承诺的 W/R 留在公共资源里。

输入：每 DMA 的 copy 标识、实际 source/destination byte 区间；各物理 source child 的首次 VALID/AR handshake/RLAST；各虚拟与物理 AW/WLAST；真实 TC/PTW 服务事件；周期预算 refill。关联来源必须作为明确生产接口实现，不能沿用 test 直接窥探 iDMA 私有字段后宣称透明 AXI 功能。

状态：每 DMA 一个 signed 正常余额；已经不可撤销的 source/W 区间集合 C；既有 metadata header 引用；每活动 child 的 bytes-remaining；预算关闭标记；最多覆盖这些不可撤销请求的 continuation 标记。C 从已握手或已首拍 VALID 的不可撤销事务开始，禁止把未来整 job 默认为 C。统计状态和功能状态分列。

输出：新的 source child/AW/TL offer 选择；continuation 服务标签；正常服务/排空服务记账标签。只在首次 VALID 之前作决定，已 offer 的 VALID/地址/len 保持。

事件与准入：

1. 正常余额满足条件时，允许普通已准备物理 AR 旁路和 ready AW。
2. 关闭预算后，不再新增与 C 无关的 copy 区间。若排空已承诺目的 W 需要 source 数据，则允许覆盖其 byte 区间的最小 source child 及其必要译址；若排空已接收 source 数据需要目的 W，则允许其对应的最小目的 AW 及译址。由此继续扩展 C。
3. 排空服务依然记到原始 DMA 余额，不能借用受害者预算或悄悄宽免收费。正常余额只由 refill 增加；服务 debt 不因为 read beat 返回自动消失。
4. C 中事务真正完成并且没有其数据尾部/目的义务时才移除；错误返回也须走有定义的 R/W 排空处理。不能把 TC COMPLETE 当作 C 完成。

这是具体规则，不是“遇堵加优先级”。但是规则的主体已被内部和普通方法覆盖：`docs/composition-history-isolation-20261007.md:252/258`已有旧债务保留和已承诺写所需排空读豁免；`:567`已有成对 slot 预约与 ready 队列；`:585–587`已有明确 byte 前缀、预约尾部和跨 copy 保留。**因为相同关键决定已探索，而且普通每片成对预约可以先保留同样闭包，本规则不能仅凭这一接口立独立机制。**

### 2.2 最小反例：单 burst 界不足以界定排空范围

受限模型：线性 byte-preserving copy；source 与 destination 都按各自2KiB边界分段；无错误/失效；目的流保持每 DMA 顺序；要求预算关闭后最终让已不可撤销的物理 R/W 都完成；每 DMA 可停放的 payload B 小于1KiB；不允许修改已 offer 的 ARLEN/AWLEN。这个模型不假定 MIG 按 AR 次序返回，也不引用 LTI。

源区间为 `R0=[0,2048), R1=[2048,4096), ...`；目的首段为 `W0=[0,1024)`，随后 `W1=[1024,3072), W2=[3072,5120), ...`。这是 src0/dst3072、2KiB源分段的自然几何。

若 W0 已不可撤销，要完成它须取得 R0 的前1KiB。要让 R0 也完成，还会得到后1KiB；B不足以全部停放，须推进 W1。完成 W1 又须 R1 的前1KiB，而完成 R1 又留下新的1KiB尾部，递归继续。简单 interval-overlap 图的连通分量可能覆盖整个 job。

**因此在该受限模型下，maxburst=2KiB不推出关闭预算后的排空工作≤一个或两个burst；排空量可能随 job 长度增长。** 这不等于当前 §39 必然存在该问题：§39的普通物理源 child 已切为1KiB，与本例目的边界兼容，因此恰好排除了这项前提。也不等于任意 implementation 都必须全 job 排空：若允许1KiB tail storage、不同通路独立停车、重新分片或不同 isolation 定义，界会改变。

### 2.3 可望形成的受限命题

令目的完整 burst 的候选停止点为 q；`r(q)` 是覆盖 q 的最后一个已完整取得的物理源 burst 的末端。在只有单 DMA、只在目的 burst 结束停车、没有额外 outstanding 越过停止点的受限实现中：

`safe_stop(q) => r(q) - q <= B`。

因为复制的 prefix q 已写出，剩下的 source suffix 必须存在某个能在预算关闭后独立停放的真实 payload 存储里。若 B=0，停止点必须同时是 source 和 destination 物理 burst 的结束点。这个必要条件若要升级成充分条件，仍须分别证明协议 route、upstream logical RLAST、错误和各共享返回通路的停车可行性；仅 byte 算式不是通用进度证明。

引入最大隔离响应量 H 后，可以把目的停止点作为有向图节点：若相邻停止点差≤H，边可走；现有 source 分段的尾部≤B则成本0，否则在该目的点新增 source physical cut，成本1。求最小成本到 job 末端，得到一个“为满足H需要至少多少额外源fragment”的受限分段问题。**这是一个可推演的分析/最优化问题，不是已证明新的在线RTL规则。** 它的全部输入可由 descriptor 和普通合法化器提前得到，普通静态 splitter可以执行完全相同的 cut；所以不能把最优 cuts 放在后译址位置后再称独有机制。

### 2.4 Strongest ordinary 与同信息同成本判别

必须保留四种强普通：CONF0共同分段；descriptor-derived source/dst边界切分；完整R尾部预约并精确直通；每子片独立译址+ready队列+普通budget/debt/refund。均给相同copy关联信息、原请求窗口、公共FIFO、normal完整permission/context/error字段。

如果静态普通 splitter 按相同 q 输出、用相同 parent/RLAST恢复 metadata，就有逐事件复现方案A的保证与请求数。这会否定“新微架构”的主张。可能留下的论文贡献仅是经前作核清的锐界/最小分片命题及其低成本算法；不是仅换计费位置。

### 2.5 实验可证伪和实现边界

不以当前32KiB固定配置的151周期为目标。最小新检查应扫作业长度、src/dst offset、读写legalizer decouple、物理child最大长度、FIFO B和预算关闭时刻，同时保留正常physical旁路。检查关闭后的实际source/dest字节与译址服务数，源/目的/预算守恒、真实quiet、错误处理、所有held VALID稳定。

若未分片且B<1KiB，在上述错位几何里关闭后的排空字节保持常数而不随job长度增长，应寻找哪个真实停车通路打断了模型；命题必须收窄或淘汰，不能把观测解释成新机制正确。若普通共同分段/边界切分达到同样保证和metadata成本，也停止新机制推荐。

最初只允许 out 抽象模型/规格，本席未执行。生产实现边界至少包括 frontend descriptor关联、legalizer/source父恢复、TC准入、两侧W序、budget observer；这是跨模块时序与进度问题，不能让普通实现 worker自主改所有模块后再验收。不能用请求量界声称硬时延界；还须存在DDR/互连/PTW有限服务界及饥饿防止假设。

## 3. 方案 B：在提交前使多个 W 序列一致

目的：在任何 source FIFO 填入错误owner之前，避免虚拟 W 序列与物理 W 序列把不同 DMA 排到头部。翻译准备仍可乱序，数据仍走普通物理旁路。

输入：每虚拟 AW 的不可撤销接收事件及 byte区间；每个准备好的目的译址；物理AW可提交条件；两侧W完成；原owner/ID和同源顺序约束。状态：每 bank 的单调序号、待提交ready位和head序号，header复用现队列。输出：只将满足队列head序号的ready AW首次offer到物理通路，读服务与已有准确owner门一致；已offer地址保持。

最小反例：虚拟序列 `[V0,A0]`，翻译完成顺序 `[A0,V0]`；朴素ready-first把物理序列做成 `[A0,V0]`，共享R和小FIFO可能把彼此所需源数据堵住。规则B把prepared A0停在metadata队列，等V0 ready后按虚拟序提交。

**该反例只是ready-first弱策略的反例。** strongest ordinary为每bank原AW FIFO+prepared位，在物理提交时保持原顺序；它和规则B拥有相同输入、header、seq存储和输出。跨bank独立队列仍并行，不必将四DMA全局串行。普通pipeline已知的reorder buffer或order-preserving queue均可以直接表达，因此不能称独立机制。

可证明性质仅为“物理AW次序为虚拟AW次序在该bank上的保序投影”；这不自动证明端到端无死锁，因为跨bank R返回、W路由、错误以及某头译址无法完成都尚需假设。普通序号FIFO有同一性质、同成本。真实bulk+小copy负载在长miss头部可能性能更差。若新规则完全等普通prepared FIFO，比较只确认工程基线，不继续扩DUT。

## 4. 本次主来源核查与前史门槛

本席通过浏览直接打开并核关键段；没有宣称通读所有全文。以下小段支持范围各自明确：

1. [AXI-REALM，IEEE TC 2025，作者预印本](https://arxiv.org/pdf/2501.10161)，§III-A，尤原文p4：period-based预算、逐beat扣费、budget耗尽后reduce outstanding并隔离；§III-B要求互连进度及RR。该工作已经覆盖预算+分片+在线调参，不能将这些组件本身称新。它不直接证明本平台错位copy闭包性质。
2. [Arm CCI-500官方TRM](https://documentation-service.arm.com/static/5e7b6a2616d2907d59402da0)，§2.4.7 p2-41：读写合并OT限额具有为防deadlock保留tracker的最小值。限流同时保留逃生容量已有明确硬件前史；并非本平台源/目的关联闭包的同式证明。
3. [Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine](https://arxiv.org/html/1808.09751v1)，§IV-C：已有逐burst metadata退休/重试，同AXI ID保持顺序，不必整transfer长期锁所有TLB；§IV-B还已有同页walk合并。普通每子片译址/重试/减少整job译址锁不能作为新增机制。
4. [Scheduling Page Table Walks for Irregular GPU Applications，ISCA2018，作者PDF](https://www.csa.iisc.ac.in/~arkapravab/papers/GPU_page_walk_scheduler_ISCA_18.pdf)，§III–IV：已有按一条SIMD指令的整体翻译工作量SJF和同指令batch。按“完成一组义务还需多少翻译”来调度不能仅凭分组语义改为copy就立新。
5. [MASK，ASPLOS2018，作者PDF](https://ghose.cs.illinois.edu/papers/18asplos_mask.pdf)，§5.2–5.4：已有TLB fill token、页表/data不同服务和受阻应用data quota。普通criticality、stage-budget、动态限流均有强邻近。
6. [Physically-Aware Preemptive Virtual Channels for Deadlock-Free AXI Networks-on-Chip，2026作者预印本](https://arxiv.org/abs/2607.01430)：已有由endpoint R/W依赖引起的环、读写traffic class separation和共享宽链路的轻量VC方案。不能将“发现R/W环或加独立VC”本身称创新。

## 5. 给根 agent 的接受/停止建议

不升级A/B为生产方案，不重跑§39、不磨151；§40先完成真实寿命边界的否决。若需要一个仍可独立推演的窄问题，选“错位burst闭包的容量—最少分片—隔离响应界”，先核已有D/G/H、DMA350完整预约、MMU-aware DMA/AXI-REALM，再只做受限命题与普通等价构造。

如果证明新锐界且前作没有覆盖，并以真实iDMA/四DMA验证该界在资源预算中的价值，可以把分析及达界实现作为独立贡献；因为当前只有条件性推演且普通同信息能实现相同cuts，不能把A/B称独立调度创新。没有论文等级或录用保证。
