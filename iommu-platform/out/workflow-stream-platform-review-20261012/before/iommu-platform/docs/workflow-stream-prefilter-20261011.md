# B：双缓冲流处理工作流预筛

日期：2026-10-11；作者与核证执行：Codex（本轮用户指定全 Codex，不调用 DeepSeek／Claude）。

**2026-10-12 stage99更新：** 官方HERO固定内核及六槽96KiB合同已取得并功能执行、独立验收；旧“缺kernel／块／slot事件”已补齐，剩余为同clock计算／实际存储端点和DMA预算。下文保留stage98原预筛与四槽泛化模板，不能直接套到新六槽应用，也不是本轮现状。[新报告与关口](workflow-stream-contract-20261012.md)／[stage98本文原样快照](../out/workflow-stream-contract-20261012/before/iommu-platform/docs/workflow-stream-prefilter-20261011.md)。

**建议：保留 B 的应用动机和一条可否定失效假设，延后进入性能模型。** 原论文支持规则分块搬入、计算、搬出和输入／输出各双缓冲；它没有给出本平台的应用计算时间、缓冲释放轨迹或普通方法的失败记录。可写 G0 失效卡，但当前证据不足以登记一次有解释力的 G0 性能判别。若另一工作流 A 已有完整入口，本轮只让 A 进入模型；若 A 也缺证据，就记录各自缺口，不能为保持席位制造 B 的动态阶段。

本结论是“缺最小工作流证据而延后”，不是 B 科学失败。旧 COPY 阴性只结束其对应分支；未测内容不能用作否定证据。证据索引见 [stream-prefilter.json](../out/research-execution-20261011/stream-prefilter.json)。本轮没有模型执行、RTL 修改、仿真、构建或板测。

## 1. 原文实际支持什么

原作者论文：[Scalable and Efficient Virtual Memory Sharing in Heterogeneous SoCs with TLB Prefetching and MMU-Aware DMA Engine](https://arxiv.org/pdf/1808.09751)。此次直接核对已有原文全文提取文件 [socdma2018.txt](../out/shared-translation-literature-20261006/papers/socdma2018.txt)，没有重新检索文献库。

| 事实 | 原文位置 | 可用范围 |
| --- | --- | --- |
| PMCA 用多通道 DMA 和双缓冲，在 L1 scratchpad 上把计算与搬运重叠 | 183–187 行 | 支持真实应用类动机；不是本平台已有 scratchpad 或计算模块的证据 |
| SP 按规则 stride 的 block 搬入／搬出；host 为输入输出分配一个 buffer；WT 在各 block 上执行可配置计算；输入和输出 block 各自双缓冲 | 437–450 行（§V-B） | 支持依赖模板；没有公布这里的 block 字节数、stride、具体计算代码、槽位复用时序或原始 trace |
| 评价平台为 Cortex-A9＋PMCA、每 cluster 256 KiB L1 SPM；DMA 至多 8 个 AXI burst 在途；两级 TLB 为 32／256 条 | 425–445 行 | 与当前 CVA6＋iDMA＋16 条 IOTLB 的资源和 miss 合同不同，不能照搬时延 |
| operational intensity 通过改变计算周期／数据量扫参数，意在刻画访问模式和架构组合 | 460–466 行；475–486 行 | 不同参数点属于不同评价配置；不能拼为一个程序中真实发生的 compute-heavy→memory-heavy 阶段 |
| helper thread 配置可以按 profiling 编译期选择，或在运行期变更；图中最后曲线为各配置整体最优 | 503–509 行；518–535 行 | 原文提出运行期调整可能性；没有给出 SP 同一次运行的阶段识别、切换轨迹、切换开销或在线控制验收 |
| PHT 利用 WT 的执行位置，按最小／最大距离窗口有条件预取；已考虑干扰造成的延迟 | 272–299 行 | “用程序进度指导翻译提前准备”已有直接前史，不能把这个宽泛概念当新贡献 |
| SP 中单加 MHT 对输入单流无效；PHT 可让多个页面进入 miss queue，再使第二 MHT 有用 | 487–517 行 | 指出有意义的翻译瓶颈／预取对照，但针对软件管理 hybrid IOMMU，不能证明本平台多 DMA 共享瓶颈的同因果 |

原文的 host 输入输出共用 buffer 提高 locality，因此也不能假定 SP 自然存在“互不复用的输入／输出页面”或必然有大量 output miss。将其改为独立 host buffer 是工作流变化，应给应用理由并重新核公平对照。

## 2. 最小依赖与缓冲寿命

以下是从双缓冲语义得到的**待应用代码／trace 核实模板**，不是声称已经复现原 benchmark。令块 j 的搬入为 I_j、计算为 C_j、搬出为 D_j，槽位 b=j mod 2；单个串行计算资源、各 block 不跨块别名为暂定条件。若应用有 halo、重叠 stride 或多个并行 WT，须由真实访问足迹修订，不能抹去额外依赖。

- 主存输入→I[b] 的搬入，必须等旧块 j−2 的计算不再读取 I[b]。采用 C_(j−2) 完成为保守释放点；如可更早释放，必须给出最后一次读取的真实证据。
- C_j 开始需要 I_j 真正搬入完成、上一个计算结束、O[b] 中旧输出 j−2 已真正搬出。前两个填充块和第一个排空块有单独初始化状态。
- C_j 结束产生 O[b] 的结果，D_j 才能从 O[b] 搬出；O[b] 直到 D_j 真实完成才可由 C_(j+2) 覆写。
- I_(j+1) 和 D_(j−1) 可以与 C_j 重叠，但输入输出是否也彼此并行，由实际 DMA 通道和资源决定。一个 V iDMA 交错两类搬运与两台 V iDMA 分别搬运是两种预算，不能混用。
- host 单 buffer 的 in-place 写回还要满足写回不会破坏未来 block 所读数据。对于 filter halo／重叠 block，先核 alias 和 read-before-write，再决定能否沿用原 host buffer。

用 s/d 表示开始／完成时刻，保守合法关系为：

```text
s(I_j) >= release(I[j mod 2], j-2)
s(C_j) >= max(d(I_j), d(C_(j-1)), d(D_(j-2)))
s(D_j) >= d(C_j)
```

首批缺失前驱按初始化“槽位可用”处理，不设成未来零时延 oracle。DMA 的完成必须覆盖真实源读、目的写及相应 B 响应；计算就绪和缓冲释放不是 NEXT_ID、AXI VALID 或内部空闲的同义词。外存映射须从首次相关搬运起保留至最后使用和必要排空结束，不能用计算结束代替映射寿命。

若输入／输出 block 同为 B 字节，应用 scratchpad 最少为 4B 字节（I0/I1/O0/O1），另计描述符、slot generation／所有权、计算状态和同步。当前每台 iDMA 的 512×8 B 私有搬运缓冲为 4 KiB；它吸收 COPY 载荷，不执行 C_j，不能计作上述四个应用槽位。

## 3. 计算时间、映射和目标尚缺什么

**计算时间。** 原文只给可配置 computation cycles，FIR／矩阵乘说明是强度例子（475–486 行），没有可移植到 CVA6 的逐块 C_j 时长。本轮不指定一个“恰好有利”的计算周期，也不制造不同强度串接。最小证据是一个明确 kernel、一个固定参数点的块大小／stride，以及 C_j 的开始、结束和槽位最后读／写事件。固定计算量也可能因真实搬运延迟产生余量变化，无须先假定动态 phase。

**映射与权限。** 现有测试用静态 Sv39 S1、4 KiB leaf，S2 字段为零；V 固定源／目的页、A 独立页环，leaf flags 为 0xDF（[nonblock_actual_dma_test.cpp](../tests/nonblock_actual_dma_test.cpp) 531–565 行）。这只是 COPY 映射，未实现 SP 的主存流和 scratchpad 身份。Linux 当前 stream_prepare 要求非 identity domain，分配强制物理连续源／目的 buffer，逐 PAGE_SIZE 映射，权限为 IOMMU_READ|IOMMU_WRITE（[idma.c](../linux/drivers/misc/idma.c) 270–289、354–398 行）；IOVA 起点 2 MiB 对齐不代表已有 2 MiB leaf（242–265 行）。

大页应列强简单对照：物理和 IOVA 均满足 leaf 对齐／连续性、所有覆盖字节权限与映射寿命一致、IOMMU 支持对应页大小，才可合并。DMA buffer allocation 已给物理连续性动机，不能通过随意拆页面禁止大页。若应用真正要求只读输入／只写输出、独立 owner 或不同释放时间，记录事实；原 benchmark 的单 host buffer／最大 locality 不能自动证明这种限制。权限放宽、额外映射空间、预先复制、配置／失效／重热都要计成本。当前 S2 关闭，不能移入嵌套映射理由。

**目标。** 固定真实输入、kernel、块数和输出正确性后，主指标为 T_stream=最后一个 D_j 的硬件完成−共同 t0，以及背景 A 固定有效工作量的 T_A。没有应用 deadline 就不造阈值；报告二者取舍和 T_return。附加记录逐块 compute 输入等待、output-slot 等待与实际 compute 周期，才能判断翻译等待是否传成计算气泡。不能拿 V COPY done 提前当完整应用完成。

若采用 N 个 block 且每 block 一次搬入和一次搬出，V 搬运事件为 2N，A 的工作量另固定。已有 M0 的每路 64 COPY 计数不可原封不动称为 B 的 64 个计算 block；可复用共同 t0、硬件 done、字节/B/quiet 守恒框架，须登记 B 的新事件合同。

## 4. 平台现在能控制什么

| 已核实接口 | 当前能力 | B 适配边界 |
| --- | --- | --- |
| software `program_single`＋NEXT_ID＋DONE | 可在前一任务真实完成后编程下一次 4 KiB COPY（test.cpp 1499–1527 行） | 可作为保守的单 DMA 搬运编排入口；现有代码没有 kernel 或四槽应用所有权；逐次配置时间计费 |
| BatchPort | 仅 DEFAULT、4096 B、源目的页对齐；每笔 COUNT 等 ready 后进入 WAIT，等 trans_complete 才进入下一 COUNT/GAP/ADMIT（frontend 153–159、519–598 行） | 自动地址同偏移步进、环绕；不能直接表达 main→I[b]、O[b]→main、compute-release 多前驱；JobFifoDepth=8 不代表现 batch 有 8 个活跃 block |
| GAP | 上一 COPY 完成后固定 countdown，首笔不延迟（frontend 560–585 行） | 不是 C_j；把 gap=compute cycles 会串行化 I/O 与计算，并漏掉释放约束 |
| A ADMIT | ResearchPort 可在尚未公开下一 COPY VALID 时等 backend_idle＋allow（frontend 588–598 行）；COUNT VALID 后需稳定（519–528 行） | 只能控制尚未发出的下一 A COPY，不能回收当前 A、改变 COPY 内 burst；本测试 top 的 enable=0／allow=1，未接入（top 385–390、444–449 行） |
| controlled launch | 源码支持先 HOLD 再 GO，COUNT 后不撤回（frontend 470–488、519–528、600–609 行） | 本测试 top hold/go 常量零；不等于已有每块 compute phase 握手 |
| 普通 DS 优先 | 已有 C++ 行为 DDR 服务干预 | 可作模型强简单对照，非已综合硬件；需要完整计入新 B 工作量窗口和尾段，不继承旧 V64 截止语义 |
| 容量保护 | composition P 固定保护 2 条，其余 14 条（composition-v1-spec 第 4–5 行） | 不可默认为 SP 多页流最优；条目工作集、可保留范围和共享服务路径须重新核 |
| 旧 p1–p5／p6 反馈 | 固定窗口反馈根据 V done bytes 调 A budget；p6 根据同反馈调 A-done 后 gap（dma_research_policy 79–84、340–344、375–424、596–601 行） | 它们已有反馈，不能称“纯静态”；目标尚是 DMA done bytes，而非计算完成或槽位依赖。接入本 top 及重新校准目标均要计适配 |

生产实例可按 profile 将 A policy 与 launch 端口接通（[ariane_peripherals_xilinx.sv](../cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv) 1274–1297 行），但当前只核接口，不声称当前 B 测试或 FPGA 已接入。该处还明确较大／多 job stream 不在当前私有缓冲依赖论证内（1264–1268 行）。不能为 B 先加新门控；G0 先核应用和普通不足，G1 才选信息／动作。

## 5. 可被否定的普通失效假设

**H_B1，待证实：** 在一个固定 kernel／参数、合法四槽复用的分块流中，A 的共享翻译服务等待恰落在下一输入可搬入或旧输出必须排空的有限提前窗口内；即使平均带宽／容量可容纳两者，普通静态容量保护、固定配额／服务顺序或既有窗口反馈仍留下可避免的 C_j 输入等待／output-slot 等待。因此，COPY 平均完成字节或平均 miss 不能充分反映应用的剩余重叠空间。

这里的“普通仍留下”是要检验的命题，不是因为没试就宣称做不到。它没有预定新门控，也没有把论文不同 compute 参数点改造成运行期变化。

**竞争解释 H_B0：** 合法大页、直接 ahead-of-time 翻译准备、普通 DS／V 服务优先、合理静态 quota 或既有反馈已经覆盖应用目标；或者 stall 来自单 DMA 供给、数据竞争、计算、别名／缓冲约束而非共享翻译。若如此，结束这条 H_B1 主张，保留真实边界。

**预定因果判别（只有取得第 7 节证据后执行）：** 同一固定输入和 kernel，比较 Solo、共享 A＋冻结强简单对照，再对有 stall 的配置只作一次既有翻译服务干预。保持 bytes、映射、buffer 数、计算工作量、供给合同和 A 完整结束；核某个 C_j 气泡的输入／输出 DMA 前驱、其翻译事件和干预后气泡是否消失。改进必须体现在 T_stream／T_A 的实际取舍，不用 miss 更少替代应用完成。

**相邻反例与否定条件：** 同 kernel 单实例无 A；或真实计算余量足以覆盖被观测翻译等待；或合法大页／保留条目下没有相关 miss。如果同样气泡仍存在，不能归因为该翻译等待。若普通 DS 优先或直接程序 phase 对照已覆盖所有已登记损失，则不能用本结果支持一个新规则。反例的参数取自真实工作流邻域，不事后选择获利强度；在实际参数均无缺口时停止，不扩成动态阶段搜索。

## 6. 强简单基线、前作和上界

| 类别 | 必须公平允许的办法 | 本轮状态／成本 |
| --- | --- | --- |
| 独立普通静态办法 | 相同合法大页、buffer、页表暖场和 ahead-of-time 准备；普通 DS 优先／V 服务优先；冻结选择规则的额度／quota | 不限制其合法提前准备。静态选法须在评价前冻结，不能每个轨迹事后选最优；所有准备、失效、重热、资源和 A 尾段计费 |
| 已有反馈 | p1–p5 或 p6，匹配合法事件和 A ADMIT 接入；窗口、target、初态和调参规则预登记 | 源码已有，B 未接入／验收。DMA done feedback 与 application compute feedback 的差别须消融；不能把强基线写成无反馈 |
| 直接程序信息的普通办法 | 使用真实 block ready／compute_done／slot-free，先服务合法 ready 的输入／排空输出，在剩余服务机会推进 A；同预算有限额度且保留全部旧在途债务 | 是新增信息的强简单对照，不能当 oracle。软件上报时延、slot generation、路由、调度和控制成本双方同计；只有同样信息下仍有具体不同决定才研究规则增量 |
| 准确前作 | MMU-aware DMA 的 compiler PHT＋WT progress＋timely translation setup；其 burst retry／retirement 合同另列 | 原文 272–299、371–412 行。错误重试 hybrid IOMMU 与本平台阻塞／返回合同不同，不能直接移植或把宽泛提前准备称新颖 |
| 离线上界 | 同 kernel／依赖／资源，去掉翻译 miss 但保留数据服务与同步；或知全未来 trace 的合法排程上界 | 只解释最大可回收损失，不作实现基线；oracle 提前准备／未来 compute_done 信息不免费。既有动态 trace 不能只减时间戳就称有效反事实 |

如果直接 phase 信息已经足够消除损失，能支持“应用信息有用”的结果，却不能把复制同规则的实现当独立新机制。如果还存在普通办法无法覆盖的可达差异，须在 G1 明确它多得到什么合法信息／动作、为何改变特定决定以及总代价；本轮没有裁定这种差异。

## 7. 进入模型之前的最小缺失证据

1. **一份实际应用合同：** 一个 kernel／一个固定参数点，输入尺寸、block／stride、真实 in-place 或独立 output、halo／alias、计算资源和四槽容量；提供应用代码或已有可核实 trace 的来源。动态 phase 不是必需条件；若要声称有动态 phase，另给同次程序切换证据。
2. **一个可重建的事件段：** block index＋slot generation；input issue／真实 done；compute start／done／最后读取；output issue／真实 done／slot free；外存映射权限与寿命。先给少量稳态块＋填充／排空即可，不能只给平均 cycles/B。
3. **可落实的移植边界：** scratchpad 是否真实存在或测试模型实现、一个／两个 V DMA 的取舍、A 预算、供给和 compute 开销、缓存／coherence／同步。当前两 DMA top 并未自动提供原 PMCA 的多通道和 SPM。
4. **普通办法的具体未覆盖事实：** 首先核大页／现有准备／服务优先能否直接满足目标；必要时登记一项有界因果判别。未经测试只能写未决，不能预告普通已失败。

达到 1–3 后可把 H_B1 升格为可执行 G0；4 获支持再进入 G1。如果取得证据所需适配超过一份小而可核验的工作流，先重估本轮预算，不先建整个 PMCA 或并行两条主模型。此次 B 交付保留应用模板、原文边界和失效卡素材，建议 root 延后 B 而非删除 B。
