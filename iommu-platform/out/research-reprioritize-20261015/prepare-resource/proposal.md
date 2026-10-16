# B1/Q：两项正向设计及有限投入判断

RESEARCH-REPRIORITIZE-1，prepare-resource 独占席。2026-10-15 启动，2026-10-16 交付。Codex-only；按 root 指派请求 Astra/xhigh，实际模型与推理强度无独立运行回执，均记 unknown。采用 ARS-Codex deep-research 的有界问题与方法设计，不启动全 pipeline。

**本席推荐进入新主线比较的机制数为 0。** 已认真发展两种不同方案：B1-D 把未来翻译结果作为可交接的一次性描述符资源；Q-S 把提前准备作为可在合法边界暂停的低优先工作。前者仍值得保留为片上接口/实现研究，但本次提出的“复用现有描述符即可少一份状态”在真实 HERO 入口上不成立，且没有留下明确的完整成本改进；后者的主要动作可由已核 checkpoint、partial restart 与保留服务表达。本判断不否定 B1/Q 整个思想族，也不把同新增接口上的普通实现等同于已发表架构覆盖。

这是一项投入建议，不是 root 的全局裁决。I 的 not_ready、A5/A2、10月15—28日窗口、M0 warm256/64/64/gap0 guard、原生/FPGA/MIG 暂停均保持。

## 1. 新设计所依据的真实需求与硬边界

### 1.1 HERO 确实有提前准备窗口，但没有现成未来描述符

`mm-large.c:164–228` 的 B 条带循环在计算 B(t) 时已能算出 B(t+2) 的 host 地址；B(t) 所在 SPM 槽要等本次 `omp for` 的隐式 barrier 后才能覆写。host 页权限与 SPM 数据占用是不同对象，纯翻译不侵犯 SPM owner。这支持合法提前准备，不支持实测残余等待或跨保护域新授权。[S1]

本轮进一步核到两个具体事实：

- 默认 `height=256`，`stripe_height` 由 128 逐次减半直到 `stripe_height*256*4 < 32768`，故条带为 `16*256*4=16384 B`。若使用 4 KiB 页，一条带在页对齐时涉及 4 页，否则 5 页；这只是源码算术，实际 host 对齐、页粒度和映射未知，不能以 4 KiB 假定排除可行大页。[S1:284–310]
- `hero_dma_job_t` 只有一个 `uint32_t id`；异步 helper 写源/目的/长度寄存器，然后读 `tf_id` 启动并返回 ID。应用只实际提交 t+1，未来 t+2 只有可计算地址，没有已经分配的多页翻译结果容器。[S2:72–74; S3:58–85; S1:171–179]

旧 ID 等待完成后可以合法回收，不能强迫普通基线一直留着。可是 32 bit ID 至多可改作 handle；若要在 release 前保持整条带翻译，它并没有免费容纳 4/5 个 PPN、权限、粒度、context 与有效代。也可只准备一个有限前缀，双方均允许这样做，但不能仍宣称整条带已就绪。handle 指向的结果表、分配器和读写口仍需计费。硬件后端已经退休的字段也可尝试复用，但双方都可使用，且本轮没有其可用容量/端口的具体实现证据。

三个 PE 分别提交 A/B/C 的源码支持软件提交并发；它不证明三个独立 DMA publisher 或当前 Dell 上的共同 IOMMU 竞争。不能把六个 SPM 槽全部累计成同时必须驻留的翻译集合；已消费数据的翻译可以提前回收，必须按实际后继用途记账。

### 1.2 当前平台的普通入口与真实接收容量

平台配置有 16 项 IOTLB、`InclDbg=1`、`InclATS=0`、56 bit PA。[S4] debug controller 用 IOVA/DID/PID/RW 发纯翻译，返回 PPN/fault/PBMT/粒度，响应 ready 恒为 1；因此 O-DBG 是必须比较的付费普通预热前门。[S5] 它与 normal 共享翻译服务；输入处 normal 优先，debug 不经过 payload AR/AW，不能算独立免费 walker。[S6:170–213,1544–1567]

正常 TC 在 `AWAIT_RESP` 恒拉高 `trans_resp_ready_o`，把结果保存到 TC 后才等物理 AR/AW 的内部交接。[S7:674–725] 所以本机正常 payload 背压不会把一个已接受正常请求的共享 TL 结果钉到数据完成。不能以这个不存在的瓶颈为 Q 分配额外 response slots。B1 若增加未来结果保持，新增的是新的结果 owner/容量问题。

PTW 的 PTE 请求为单拍读；`MEM_ACCESS` 发布 ARVALID 后等 ARREADY，随后 `PROC_PTE` 接收 R，再检查 PTE/总线错误。[S8:274–280,384–405,607–644] 已 first-offer 的 AR、已握手的 PTE 及其结果归属不能因优先级变化消失；下一 PTE 首次 VALID 之前才是本轮安全暂停点。

## 2. 设计尝试一：B1-D，一次性未来描述符结果交接

### 正常问题、具体能力与实现选择

问题是：future 地址已知且 SPM 仍忙时，普通预热完成的翻译可能在真正消费前丢失；用长时间 IOTLB pin 又可能挤占其他有用工作。尝试把准备结果直接绑定到一个未来请求，存放在源端描述符所属的有限结果池，待 payload 合法后交给 TC；结果不必一直占共享 IOTLB，且不授权提前写 SPM。此处“可能丢失/挤占”是待检验残余，不是已经观测的 HERO 性能事实。

最强版本不是一个新 ready 分数，而是一个明确的一次性交接接口：

1. `register(span, context, requested_permissions, source, job_generation)` 登记不可变 future span；同时预约 K 个 pending/ready/fault 结果额度中足够的额度。额度不足就拒绝或等待，不可先接受无处返回的 walk。
2. 翻译按真实 page boundary 产生许可结果。每个结果有完整 key、PPN/粒度/属性、所请求权限是否获准、context/失效代和所有者；失败也是有 owner 的终态，不可把 debug 返回 PPN 直接当通行证。
3. `payload_release` 来自真实计算/consumer 依赖，只说明应用现在允许提交。它不冒充 AR/AW 的握手或完整任务成功。
4. `adopt` 在准确原请求及范围匹配、权限有效且真实接收能力存在时，把结果交给原生物理地址 owner。需要改变地址/许可的门放在第一次 VALID 之前；held VALID 后不可换头。旧结果在最后一份身份/许可被接收之前不能回收。
5. 最后一次翻译用途真正交接后，可释放该结果；未来消费者尚未生成、原 accepted parent 尚有未发字节或还有其他合法用途时不可提前清账。R/W/B、consumer、缓存可见和 buffer 归还分别退休。
6. 正式失效优先使未采纳结果不可用；已经承诺的事务按正式协议完成/清债。首个性能投影可以固定静态映射，但必须说明其范围，不能声称已解决异步撤销。取消前与 first-VALID 后的处理不同。

不把应用整个 DAG 放进 IOMMU；原应用负责 SPM release，接口只保留准许交接的证据和完整结果归属。K 是总 pending/ready/fault 容量，不是只数 ready 项，更不是保证完成时间。

### 最近前作与三个具体普通对照

- ICCD2018 §IV-A 已在 worker 的进度窗口内纯翻译预取；§IV-C 还用逐 burst 重放缩短整 DMA 的长期锁页。故“SPM 不能覆写仍可先译”“只保留到最后用途”不能宽称新。[P1]
- RISC-V ATS 已分离 translation request/completion 与后续 translated request，并有 ATC 失效/响应语义。此项复用 10月15日原规范核读，不声称本席重读 PCI-SIG 全文；它没有证明本片上实现成本相同。[R2]
- O-DBG：同 future 地址与时间窗口，用当前 debug 串行预热，之后仍走 native lookup；计 CPU/MMIO/串行仲裁/污染/重走。DATE2025 的 PTE LLC 也必须进入允许配置，因为它在原平台显著削弱翻译残余，但其百分比不是本机数据。[P3]
- O-PIN：在同一付费登记口与 K 预算上只 pin 尚有翻译用途的页，最后用途即 unpin；允许给完成推进让路与合法回收，不能罚它整任务长 pin。
- **O-PAQ-D**：具名普通有限队列程序。`free -> pending -> ready|fault -> handed_off -> free`；把 descriptor 中已消费 ID 字段回收作 handle，结果放同样 K 项表，逐项匹配 context/epoch/range，遵守同一 release 和握手条件。这是 B1-D 的普通实现候选，不是一个需要打败的虚构全知程序；本轮仅给出可实施状态机与配对，未编程/运行，也不声称完整异常协议等价已验证。

还必须允许合适的片上 ATS/ATC 适配、正常翻译后的物理缓存、合法大页/映射驻留，以及增加 payload 槽这个不同面积点；都计真实费用，不向 ATS 基线硬塞整套 PCIe PHY。

### 一个条件正例与一个相邻反例：均未运行

**正例投影 B+**：只使用 HERO 实际 A/B/C 条带与同一合法映射，不添加压力流。取某 B(t+2) 的合法窗口 `[a,r]`，证明（a）同预算 O-DBG/PTE cache 后，某个需要的 leaf 在 r 时确实不在可用路径上；（b）其重翻译阻挡完整矩阵结果，而非仅阻挡一个计数；（c）B1-D 早先结果仍有效且 K 容量可达。若晚到的普通服务为 L，准备/保持能隐藏的局部上限是 `min(L,r-a)`，不是总任务收益。O-PIN 和 O-PAQ-D 也可获得此隐藏，B1-D 的研究增量必须另由部署/完整成本给出。

**相邻反例 B−**：保持同一程序、条带和真实资源，只取本来就没有中间相关 eviction、PTE 已热或计算足够遮蔽翻译的正常条带。O-DBG 已满足完整目标，B1-D 只增加登记/存储/采纳成本。若只能另添 publisher、扩工作集或制造时相才得到 B+，应另立有来源投影，不把它回写为原 HERO 正例。

一个同信息固定的 O-PAQ-D/O-PIN 实现贯穿正反条件；不能每条件拼 oracle。完整终点包括所有矩阵数据、最终 C stripe、等待、cache handoff、consumer 与释放。[S1:238–273] 本轮没有周期值、收益数或自然 eviction 观测。

### 为什么本版没有升位

本轮试图给出的具体成本点是“无需额外 ATC/结果表，原 future 描述符已有字段足够”。直接源码推翻了这项理由：future span 尚未提交，现成对象只是 32 bit job ID；若在 4 KiB 合同下把默认整条带翻译保持到 release，需要 4/5 页结果，而只保持前缀也必须有真实的结果归属。可以加结果池，也可借现有后端存储，但必须交付真实容量/端口/归属图，不能把空闲字段想象成可任意共享多端口 SRAM。普通 O-PAQ-D 可以使用相同回收，因此“原位”词汇不提供独立逻辑节省。

**保留的条件贡献**：如果一个实际片上实现相对 O-DBG/可行 PTE cache/local ATC 给出更有用的能力或同完整目标下更佳部署成本，B1 仍可是一项接口/系统贡献；无需每个协议构件首次出现。**当前不能采用的主张**：B1-D 已有独有调度收益，或已证明去掉一份结果表。不同声明分别裁决，不能因 O-PAQ-D 可实现它就宣称整个接口已发表、架构无价值。

最小下一证据是**一条 future stripe 的结果驻留与物理存储对照卡**：a/r 的源事件、各页真实粒度/key/最后用途、允许 PTE cache/预热后的残余、K 总额度，以及 candidate/O-PAQ-D/O-PIN 各字段在哪里保存、哪个端口写读、何时回收。可以先给 source-bound 条件投影；无需先有 native joint trace。但单纯再写同协议功能模型不会决定这项成本主张。

## 3. 设计尝试二：Q-S，提前准备的可暂停服务与有界归还

### 机制与不同于 B1-D 的目标

B1-D 保留完成的翻译，Q-S 则限制**尚未完成的准备工作占用 walker 多久**。真实问题是纯预热可能在 demand 到达前刚获得共享服务；入口优先级不能撤回已承诺的服务。试图用一个可暂停的低优先准备上下文，让 demand 在当前已承诺 PTE 的尾部之后前进，随后继续原准备，而非让它从根反复重走。

拟议规则：只在没有 demand eligible 时开始 prepare；demand 到达就标记让路；当前已 first-offer 的 PTE AR 和随后 R/错误必须完整退休，保存下一 PTE 所需状态后才切换。正常/错误终态如果已发布结果，按原 owner 交接，不把终态当可丢弃中间进度。只设有限 J 个 suspended frame；满时要明确拒绝/放弃尚未承诺 prepare 或采用受限 restart，不能偷偷新增 walker。持续到达时给 preparation 固定可执行服务份额或有限延期合同，否则它可以饥饿；该份额也会增加 demand 等待。

保证严格写作：在 memory 本身有已给定有限服务界的条件下，**一次让路额外阻塞至多是当前已承诺 PTE 的剩余服务加切换开销**。没有 AR/R 服务界，就只能给 PTE 事务边界而非 cycles；全体 demand 的总等待还包括既有同类排队与公平服务，不能被这一个 residual 代替。生产 PTW 当前没有该暂停接口。

### 已核前作与具体普通实现

1. DWS §V 的 owned walker 在 owner 无请求时借给别人，owner 后来请求至多等一个 foreign **whole walk**；这是直接原文，不可偷换为一个 PTE。它已经提供保留/借用概念。[P4:111–118,369–378]
2. 逐 PTE tracking/token/再仲裁在此前 US8140781B2 核读中已有；本轮沿用该已核边界，未重新读专利。Rocket 2022 的 request destination、parent/child PTE、level、地址重建与 S/G 返回直接原文已本轮重核，不能把嵌套 phase 分解或删除冗余字段再立新。[R4; S9]
3. **O-CKPT**：同 J、同原请求/context、同 terminal/owner/在途账本，demand 到达后等当前 PTE 真正退休，按既有字段存 frame，按原优先/公平选择恢复。它已经能给 Q-S 上述单次让路性质。
4. **O-GR**：保留已经完成的 S 前缀，只重起当前 G 子 walk；在最大三层稳定正常无写侧效合同中，一次暂停最多重读 3 个已经读过的 G PTE。必须给一个能完成 G+S 的连续片段或明确暂停次数，否则反复重读根可以无进展。已有 q/B 数学结果只覆盖固定两请求，不推广为持续到达定理。[R4:81–96; R5]
5. 给 O-CKPT/O-GR 同预算 partial/path PWC、合法 prefix 共享、动态 cache 分配与 field recycling；不拿完整 raw snapshot 对 whole restart 作为唯一基线。MASK 的 TLB token、PTE bypass、页表/数据内存优先分别作为适用 ordinary 部件，不拼成免费全知组合。[P2]

### 条件正反例、费用与判定

**Q+，未运行**：真实 future span 的 prepare 已开始，demand 在某个未到叶子的 PTE 返回后到达；后续尚有至少两级合法工作，且 demand 确是完整 consumer 的关键路径。Q-S 相对“等 whole prepare 结束”的入口策略可让 demand 早走。但 O-CKPT 做同一件事；这不是新机制相对强基线的胜例。

**Q−，未运行**：同一到达位置，当前 PTE 实为叶子，或 memory 的当前不可撤事务已占主要残余；切换没有后续工作可避，反增加保存/恢复成本。另一个安全邻反是 frame 满或无限 demand：前者不能无处归属，后者没有保留服务就无法承诺 prepare 完成。

全成本包含 J 个 frame 的全部 context/phase/owner/return/error、active frame、在途 PTE 地址/ID/归属、保存恢复口、暂停仲裁、PWC tag/data/端口、被重读 PTE、terminal/fault sink、失效与取消、准备侧与 demand 侧完整工作。82/36 bit 等旧数字只是一类成功路径 payload 账，不是总面积或本版新最小编码。

**本版停止到哪里**：停止“可在 PTE 边界暂停准备、优先 demand、随后继续，所以有独立 Q 新意”的主张。接受的普通具体实现已具备这一动作；本次没有找到一种已握手义务使 Q-S 合法前进而 O-CKPT 在同接口/容量下必须停。正常 TC 恒 ready 还排除了一个本机容易误造的共享结果滞留理由。没有证明持续到达/动态 PTE/PWC 的全族性能最优，也没有宣布整个 Q 无价值。若要重开，应给一个新服务/存储成本点及可验证闭合义务，不重新扫描固定 15-step 模型。

## 4. root 追加的 C2-P1 短评：完整服务界是否值得单独保留

**有精确适用性问题，当前没有足以升位的理论贡献。** TC2023 §2.1 允许 job 内任意到达，故 PTE 返回才生成后继请求本身不超出它的到达包络；其明确限制是 R/W 独立、互连 pipeline 不满，以及有界 PS 服务常数。Lemma10 的流水代价依赖 pipeline 不满，不能把未知反馈等待塞入 compute C 来绕过该前提。[P5:103–130,185–226,590–610,657–671]

值得保留的窄问题是：**给定正常 DMA 的 source-R→destination-W 依赖、有限返回/在途额度和共享单 PTW，能否在同一可执行调节规则下，给出既安全又足够紧的完整 consumer 服务界，从而判断是否需要额外返回容量？** 这是 C2-P1 的分析投影，不是请求实现新 scheduler。没有实际应用期限时，不得虚构 deadline；可以先给有界完成/阻塞条件，cycles 保持条件化。

最强同信息 ordinary 对照是一个明确的事件递推：每个 PTE R、TL 交接、payload AR/R/AW/W/B、consumer 和 credit release 为节点；依赖/已承诺顺序/同ID约束为边；有限槽用“第 k 个申请不得早于所对应旧 owner 释放”的资源边；给每服务区间同样的有界时延。固定服务选择时用 `F(v)=max(F(pred(v)),resource_available(v))+service(v)`；选择/命中/动态到达改变图时，用同合同有限状态探索，而非把一张事后固定 DAG 冒充闭环上界。普通模型须得到与候选相同来源信息，允许合法字段和 credit 回收。

最小决定性证据是一个**来自当前合法源码链、会使 TC 的特定流水前提失效的正常有限返回阻塞片段**，配一条仅改变真实返回能力的相邻可排空片段，再说明新分析如何用可复用摘要给 ordinary recurrence 没有直接提供的更有用容量/服务决定。源码有限 buffer 不等于已证明它会满；TL normal response 不会被 payload 钉住，因此不能拿那个旧假设做见证。当前没有该完整片段、明确紧度改善或应用服务目标，本席不因 B/Q 性能设计未升位而强选理论。AXI-ICRT/REALM 与 2026 NoC 读写循环近邻已在旧审查中存在，后续若选择这条才定向核完整方法。

## 5. 共同成本与接受边界

| 费用/义务 | 本轮可接受内容 | 未知项 |
|---|---|---|
| 应用供给 | HERO 源码地址与依赖；默认条带算术 | 当前实际映射/对齐、真实周期、共同硬件 publisher |
| 登记/结果 | B1-D 的 K 个全部状态；O-PAQ-D 同接口/回收 | 实际字宽、结果池/端口布局、MMIO/CPU |
| PTW/缓存 | 单 PTW、16 IOTLB；O-DBG 争用；CKPT/GR 对照 | 新接口时序、PTE/cache/DRAM 闭环成本 |
| 许可/owner | first VALID、实际握手、完整范围/权限/epoch、真实结果 owner | 一般异步失效/错误/取消原生实现 |
| 数据/完成 | R/W/B/consumer/cache/最后释放分别记账 | 完整任务净收益、背景进度、能耗 |
| 硬件 | 不把字段数当面积 | LUT/FF/BRAM/Fmax/物理能耗均 null |

可核查 novelty 论证：ICCD2018 已有进度限定的纯翻译准备；ATS 已有结果与后续访问分离、保存和失效；DWS 与已有 continuation 提供有限资源保留/借用及合法暂停的基础。HERO 的正常准备窗口和当前 debug 前门支持一个具体片上部署问题。**如果**实际设计能在准确普通预热/PTE cache/local ATC 之后解决一种源级可达残余，并在同完整完成目标下减少明确资源、软件或服务成本，新增贡献可以落在该接口/集成及其验证认识；**如果**实现只是同状态的 PA 队列或同字段 checkpoint，不能把重新命名作为新算法。本轮已核的是来源、协议边界和两项设计化简；剩余成本点与完整结果均未证，不作全领域绝对新颖性结论。

本轮只定向读源、手工设计并生成四份研究文件；0 新模型/源程序实验、0 旧实验重跑、0 构建、0 native DMA/硬件操作、0 生产修改、0 联网、0 DeepSeek/Claude。未改 canonical 文档或全局排序。完整定位、来源级别与 hash 见 `evidence-map.md`、`source-manifest.json`；机器候选卡见 `candidate-card.json`。
