# A4：AXPY的具体控制动作与完整交付推导

2026-10-15。作者与接受者：root Codex；固定DMA回执链由独占Sol/ultra席补核，root直接复核关键源。仅研究判断，不应用方案、不编译、不仿真、不上板。复用A3来源、方法和前作，不重跑其8项符号检查。

## 1. 决定与范围

**这条AXPY有真实设备consumer和合法可改变的动作；对该动作，现有公开完成ID已经足够，不需要新增Linux、IOMMU或RTL接口。** 最有根据的普通版本是最后tile只等后发输入RY的一个ID，同时保留末尾全部排空。它可能放宽计算等待，但实际提前窗口、完整净收益和共享多DMA价值均未证明。

这不是“普通方案已满足某个时间目标”的证书：实际目标期限、服务界与费用尚缺。它是“普通接口足以表达这一安全候选动作”的证据。A作为候选保留，本轮不恢复其跨方向默认优先级。

## 2. 完整输入与任务事实

- 固定业务：已有AXPY `n=32768, alpha=1, do_map=1, noise=0`；输入按现有main的确定性公式生成，两个既有cold/hot episode分别恢复原值，不扫压力、相位或随机种子。每次kernel为64个512-FP32 tile，每个输入/输出job为2048B；128个输入、64个输出，总搬运393216B。原main两次kernel共384个job/786432B，映射在它们之前建立、最终才撤销。它们是静态工作量，不是容量、时钟或延迟。
- 源：Hero-tools `47339e040c43ab4d44d9a870bc307251084a1ba4` 的A2隔离修补AXPY；不是未修改上游原件。Carfield `dbb4ad3380e6e81fa7eac3955e2340752f29dab0`；Bender锁定 **pulp-platform/snitch_cluster** `269830af872fa943d8b079470120d42d62e1cb69`。AXPY SHA256 `0738c631e9b08685c1ac0f6d1feb0a1ce3f38b17f7805bc29a6ac1b54df12307`。
- 原资源：X[2]/Y[2]四个2048B L1槽，共8192B；core8唯一DMA publisher，核0–7读X、读写Y做FP32 FMA。保留原始host x/y，所有比较均可保留原buffer；校验用reference/expected单独记仪器成本。
- 底层：仅一个DMA，FPCluster端口经IOMMU，其余master旁路。不是当前CVA6四iDMA原生任务，也没有真实第二publisher。AXPY的显式IOVA路径不能称为已验SVA/shared-mm。
- 真实链：host准备/映射/cache交接→target→输入DMA写L1→8核FMA→输出DMA写host y→最后wait-all/fence/barrier→dispatcher DONE→host cache acquire→独立结果消费→unmap/失效完成/unpin/free。Hero排空、失效完成和cache三个后端仍缺实现，默认拒绝pin；本轮没有把它们补成已运行链。

原AXPY发布顺序为：初始RX0、RY0；每轮先wait-all与barrier，再同时进行下一tile的RX/RY与本tile计算；计算结束barrier后才发布WY。本轮只补决定控制能力的DMA回执原件，应用/owner/lifecycle沿用[A3 source合同](../../a3-consumer-position-rules-20261015/source/source-contract.json)。

## 3. 已闭合的承诺与完成合同

| 事件/能力 | 固定来源给出的含义 | 不能据此推出什么 |
| --- | --- | --- |
| 源调用/首次offer | core8执行自定义指令；FE等待下游ready和响应空间 | offer不是接受，调用不是固定周期 |
| descriptor接受/返回ID | twod FIFO valid&ready入队，next_id递增；返回原next_id，核心见到返回还可延后 | 不代表载荷完成；接受后无cancel/whole-job重排API |
| 原生AXI产生 | FIFO和单job reshaper产生AR/AW；native AXI ID均0 | 软件transaction ID不是可供IOMMU随机选择的AXI job tag |
| completed_id | 最后写分段B退休后累加；同ID有序B下为有序完成前缀，值为最后完成ID加1 | 不检查R/B错误，不证明成功、cache可见、失效完成或可free |
| dma_wait(id) | 在新鲜ID、有限模差窗口内等该ID及其全部前缀 | 不能单独越过更早job；无需也不能免费读取内部翻译完成 |
| dma_wait_all | 等next_id==completed_id | 比当前消费者所需输入集合更宽，含更晚发布的旧输出 |

关键原文：FE 169–173、268–299、320–324；twod 41–104；reshaper 239–310；mover 321–369；runtime 429–458。详见[逐条来源及15份哈希](../source/receipt.json)、[推导与原文定位](../source/receipt.md)。root直接核了FE、twod/backend、reshaper、mover、runtime与AXPY相应调用；没有用未实例化的另一ID发生器代替实际FE。

实际资源分级：twod FIFO 3 descriptors，backend FIFO 3个1D job，AR/AW/R/W FIFO各深3，data BufferDepth=3，last-AW标志FIFO深6，FE last-1D FIFO深16。单位与阶段不同，不能相加成总接受容量或从深度推出真实pending窗口。

两项窄范围勘误原样登记、不改旧锁文件：旧表的Snitch仓URL应为`snitch_cluster`；runtime把serialize/deserialize编码进bit2，而固定FE只收cfg[1:0]，twod转出的serialize恒0。**不能用cfg0注释作串行/无死锁保证。** backend允许跨job在途，源码有该模式风险注释；本轮未观测死锁，也未证明所有原生协议/错误行为。

## 4. 具体动作与owner证明

### O-T1：最后tile按一个输入ID放行

1. 原来所有非最后tile仍wait-all。倒数第二轮发布RX_last、RY_last，保存后发RY的返回ID；一个逻辑32位值，4B。不得假定编译后只多4B物理成本。
2. 最后轮用`dma_wait(id_y_last)`替代该轮wait-all，保留全核barrier与必要编译器/硬件顺序。RX比RY早接受，累计完成前缀已覆盖RX，无需另等RX。
3. 最后轮没有未来预取。最后计算的Y-bank与WY_prev读取的bank不同，不覆盖旧输出。底部全核barrier后才发布WY_last。
4. 末尾wait-all、fence、barrier、DONE、cache、独立consumer、unmap/失效/unpin/free全保留；两组Y一直活到相关访问真正完成。单tile时不读取未生成的预取ID，沿原路径。

固定默认任务给出可核的ID关系。设本episode初始next_id为b，且无其他issuer插入：RX63=b+188、RY63=b+189、WY62=b+190，WY63=b+191。最后循环入口已接受WY62；若completed=b+190、next=b+191，RY wait通过而busy仍为1。最终必须completed=b+192。**这是允许的计数状态，不是已经测到的时间窗口**；重复运行不假设计数重置为0。仅192个新鲜job，仍须检查实际XLEN/有符号差/回绕，不用古老ID充当永久许可。

runtime wait helper本身没有memory clobber；现有barrier带memory clobber/FPU同步。将来实现仍需核固定编译器产生的顺序及端点可见性，必要的本地fence及其代价双方一致计入，不借此声称本轮已编译验证。

### O-G：给普通方案更完整的机会

不把普通对照锁死在最后tile。一般循环允许以下公开ID固件：

```text
初始发布RX0、RY0，保留input_y_id
每轮t：
  core8 wait(input_y_id)；所有核进入计算起始barrier
  核0–7计算当前X/Y
  core8若有下一tile：
    发布RX[t+1]到旧X-bank
    若t>0：wait(previous_output_id)   # 旧Y-bank仍可能被WY[t-1]读取
    发布RY[t+1]到旧Y-bank，更新input_y_id
  所有核结束计算barrier
  core8发布WY[t]，更新previous_output_id
尾部保留完整wait-all/fence/barrier/lifecycle
```

旧X计算在上一轮结束barrier后已结束，可早发布下一RX；旧Y要等其旧输出读完，保守公开完成ID足以回收。逻辑上两个32位值/8B即可表达，实际寄存器/溢出/指令/等待费用未验。输入ID变量在本轮wait后可复用，不人为限制可复用字段或额外正常标量；全部比较开放相同元数据预算。RX可以在Y guard前发布是合法选择，不强迫两个输入一起阻塞。该普通版本可能损失预取重叠，不能凭owner正确就宣称更快；serialize0下的新交错还须验liveness。**本轮未应用任何伪码变体。**

中间轮机械替换所有wait-all但不加Y guard，不是合法强对照：它可能把后继RY写入仍被旧WY读取的同一槽。停止范围仅此错误改法，不排除O-G等普通程序。

## 5. 控制位置按动作判别

| 决策位置 | 尚可改变的具体动作 | 太晚/无权改变的具体事项 | 最终消费者后果 |
| --- | --- | --- | --- |
| core8调用之前 | 合法RX/RY顺序、是否预取、是否延后输出/下一输入；公开ID/DONE节流 | 尚未完成FMA的WY当前数据还不存在 | 重排需计失去的计算/预取重叠，不免费提前WY |
| descriptor接受之后 | 固件仍可缩小“等哪些已发布job才能计算”的范围 | 不能撤回或重排已入FIFO的whole descriptor | O-T1仍可能提前另一bank计算；所以“已接受”不等于所有控制都无效 |
| IOMMU入口/DS | 仅对真实同时可见、独立且未承诺的请求头节流/优先；给DS-first合法机会 | 不能恢复未生成的最后WY或隐藏的whole-job选择，不能随意改已offer头 | 可加快旧输出/翻译，但不能单靠服务优先删除固件wait-all依赖；尚无全策略无效证明 |
| walker/内存 | 有对应可修改选择能力时，MASK/DWS等方案可服务翻译/数据 | 不能免费赋予本固定端点多walker、随机队列或任意B重排 | 必须重算完整输出/consumer和实现费用，不由翻译提前直接判胜 |
| host DONE之后 | cache交接、结果消费、释放 | 不得在真实排空/可见/失效完成前free | O-T1不改变该安全要求，不把最后目标改成compute-only |

特别地，当前用途输出优先不是在循环顶部“选择现成WY”：当前WY须等计算结束。普通实现可以取消本轮预取，在结束barrier后先发布WY，再发布下一RX/RY；它保留原槽但付出失去预取与计算重叠的费用。它也可把旧输出节流到公开完成，或保留原buffer并使用已有独立页表/cache配置。所有这些合法普通选项都在比较范围内。

独立IOVA、同址shadow、适用大页/LLC页表缓存须按Hero真实端点和支持能力计费；当前板PV0/DID10/11 helper不能免费移植到Hero PV1/DID0。映射选择可以消除walk压力，但不会自动删去程序wait-all或Y回收依赖。CPU原y上FMA也是合法整体功能方案，源码无CPU预算/后继设备位置要求排除它；原输入直接writev不能完成AXPY变换。host只有整次offload/DONE接口，不能把每tile设备选择冒充host原生控制。

## 6. 完整完成推导：不把B有序偷换成串行服务

输入是任务/程序、上述API摘要、四槽owner、实际前缀、允许动作和真实服务合同；输出分别为owner合法性、可表达动作、局部放行条件、完整交付时间范围/UNKNOWN。若要称某位置“足够”，还需具名可执行策略的完整上界满足目标；若称“必须更早”，需该晚位置**全部允许策略**的下界超过目标，再给付费早方案上界。本例没有这些时间证书。

对同一最后轮入口，设r为两个输入的成功可读时间，o为旧输出退休时间，h0/h1为各等待观察与起始barrier代价，k0/k1为最后计算到新输出**首次offer**的时间。局部关系为：

```text
O0的最后输出offer：p0 = max(r,o) + h0 + k0
O-T1的最后输出offer：p1 = r + h1 + k1
```

只有原服务不被新增计算改变时，r/o才能直接共享。一般情况下使用各自轨迹的rπ、oπ、kπ；提前计算会竞争TCDM bank，改变旧输出、翻译/DDR/cache与供给。prefix中保存ID的费用也可能改变随后时序，不能默认为共同前缀免费。

令 `G_Q^π(p)` 为：从该实际剩余前缀Q，在p首次offer最后输出后，经真实FIFO背压、AR/R/AW/W/B、旧/新输出全部退休与设备尾排空，到dispatcher DONE的时间。它必须由实际资源服务产生，不是自由指定的延时。host继续执行cache acquire、独立consumer、revoke/失效完成/unpin/free，完整时间为：

```text
Tπ = 实际准备/映射/供给及前缀时间π + G_Q^π(pπ)
     + host可见性交接/消费/映射撤销及资源归还时间π
```

应同时输出计算完成、两笔输出B、设备DONE、host消费和资源归还的向量。单DMA没有第二offload；本次“双方”是全部旧/新输出义务与consumer，不虚构第二DMA任务完成。

原main的完整调用含cold/hot两次offload：共同准备/映射只在真实发生处计一次，每次输入恢复、cache供给、target、完整DMA和独立consumer均计入，最后一次之后再计全部撤销/释放。上述尾部公式可以逐次组合，但不能把单次尾部改善直接称为整个main收益，或把第二次运行/最终释放删掉。参考生成/诊断成本单独报告仪器与业务范围，CPU整体对照获得同样的输入/结果要求。

**不能直接套 `max(旧B, compute结束)+固定新DMA时延`。** 固定serialize=0，后一个job可以在旧B退休前产生/服务请求；同ID B顺序只限制退休顺序。把全部新输出工作强制串行到旧B后，会误算损失重叠和收益。这正是本轮对A3纯源语义补充的服务边界。

实际预测算法是：保留上述依赖/owner边；服务节点按真实服务器和可用缓冲给界，保留已offer/已接受义务与合法流水；将完整信息同样交给标准DAG/状态/AXI分析。得到候选完整上界U1和普通基线下界L0才可事前承诺净收益，判据 `U1 < L0`；若仅有均值/样本，则只承诺其对应统计比较，不能叫WCET。当前服务、并发损失及全部费用未给界，**完整时间输出UNKNOWN，数值为null**。本轮不实现/执行时序后端。

## 7. 冻结的是可推翻的研究决定合同，不是四个时间胜例

| 预测/边界 | 事前条件 | 失败判据与结论范围 |
| --- | --- | --- |
| P1 原生普通动作存在 | 固定API/有序成功完成、RY输入可读、旧WY未退休、不同bank、尾排空保留 | 真实ID不具前缀语义、错误/可见性不成立或owner被破坏，则撤销该版本安全主张；实际从无窗口只否定本任务重叠机会 |
| P2 局部提前可能传到完整交付 | p1早于p0，真实G与全费用导出的U1<L0 | 完整consumer/释放不在预测界内，或观察到净收益符号相反，则服务/费用判定失败；compute提前不算通过 |
| N1 相邻无等待边界 | 自然运行中最后输入可读时旧WY已退休，其他服务/费用相同 | 若仍归因“隐藏旧输出等待”报告收益则因果解释失败；新增开销可能使其更慢，不造延迟制造该案 |
| N2 普通已覆盖该动作 | O-T1/O-G享有相同信息/四槽/元数据，公开ID本来可用 | 把同一完成前缀另包新接口而不增能力，只能作为候选等价实现，不能主张其独有性能动作 |

这些是条件命题和失败条件，**不是已经冻结/执行的1正1反2时间留出**。n=32768默认任务和动作已固定，但自然pending、服务和成本尚未绑定；不把n=512或任意压力旋钮改名为留出。没有实际收益需求时，不为凑正例继续软件维护或扩大矩阵。

## 8. 最近前作与方法增量

复用已核原文：MASK §5.2–5.4已联合翻译与数据服务，DWS §V–VI已分配/借用walker；RTAS2026 §V–VI已有FIFO外调度、合法分块与完整响应分析，TC2023 §2/§3.7允许job内任意请求时序但有读写独立/流水不满假设。不得称这些工作不支持依赖到达，也不以普通完成ID、max-plus/事件图或Y回收本身报新意。精确原文入口见[既有近邻核查](../../research-novelty-assessment-20261015/analysis-priors.md)。

本轮的方法深化是**按具体动作区分承诺边界**：已接受的WY阻止源端whole-job重排，却不阻止固件释放另一个bank的consumer；软件job前缀又不等于AXI job tag或serialized service。这个源→动作→owner→服务摘要可以避免错误地选择“新增早通知硬件”。但它当前是一个手工源码实例；给同信息的标准分析也能表达，尚不构成独立新算法或论文主贡献。

可保留的发展版本：把这类端点摘要做成范围明确、可复用的抽取/检查规则，判断何时已有公开ID足够、何时真实共享翻译反馈与owner使所有具名晚控制不足，并在未见任务事前给部署预测。只有导出有用的新决定、标准同信息对照不能用相同工作量直接替代其抽取价值，且真实服务/完整成本验证通过，才接受方法贡献。单AXPY、SVA实现、两个策略比较和更多日志不能代替该证据。

## 9. 最少后续证据与停止范围

本轮决定保留AXPY为普通控制校准任务；不新增Linux/RTL、不继续编译、不上板，不宣布完整时间收益。下一次若安排该任务，仅先补下列必要事实，再决定一小段后端或测量：

1. 固定硬件真实身份及serialize错配的处理选择、成功/ordering/liveness与cache/drain/失效后端；不能把本轮静态源替代实际硬件验收。
2. 自然默认运行最后两tile的RX/RY/WY接受ID、completed计数、关键B/错误、barrier/compute与最终DONE；需同一时基或已界定跨时基。先判断窗口是否实际存在，不通过扫相位寻找它。
3. 若窗口存在，最小模型只含最后两tile的真实FIFO/AR/R/AW/W/B/TCDM服务和既有尾部，保留serialize0合法流水；服务取自对应配置，不能任意填时延。ID保存、poll/fence、失去的重叠和host全生命周期均计费。
4. 若研究共享IOMMU主张，先有具名的真实第二publisher/consumer，或已有应用给出的CPU/设备位置约束。此缺口不能由向AXPY添加合成aggressor或禁止CPU来填补。本轮不广找新任务，跨方向独立比较仍另行保留。

O-T1能表达本次动作，故停止“该动作必须新增早期意图/完成接口”的版本；现有资料不足以停止A、普通ID策略本身或真实共享翻译研究。无真实第二publisher和服务界，就不继续修外围软件来寻找论文胜例。三包文件基线、A2构建、A3符号结果、旧结束实验及MIG暂停全部保持。
