# A3：真实消费者与控制位置规则

2026年10月15日。**A已推进到一条具体的普通方案与可检查规则：先按真实consumer缩小等待范围，再检查缓冲回收会不会被破坏。** DATE关联AXPY有真实L1计算消费者；现成runtime已提供按transaction ID等待，不能继续只给wait-all/串行对照。最小候选仅改最后tile入口，保留末尾全部排空。

本轮交付的是源码合同、方法规则和符号逻辑检查。没有证明实际重叠窗口、共享多DMA竞争或完整时间收益；四个时间案例仍未冻结。原MIG暂停，生产代码/FPGA/板端及已结束实验均未动。

## 1. 真实任务带来的具体进展

精确来源复用A2固定HeroSDK及已编译隔离修补。AXPY每tile为512个FP32元素，四个2048B L1槽X[2]/Y[2]，core8唯一DMA publisher，核0–7读X并原地修改Y。原输入不能直接writev交付计算结果；当前kernel确实要求数据在L1。**这不排除CPU在原y计算同一算子**，源程序本来就用CPU FMA生成独立expected，也没有CPU预算或后继设备消费者来禁止这一方案。[源码合同与逐行证据](../out/a3-consumer-position-rules-20261015/source/findings.md)

默认32768元素对应64tiles、128笔输入/64笔输出，四槽合计8192B；这些是静态工作量，不是DMA接受容量或周期测量。校验用的三份reference/expected不是所有部署都必须支付的业务buffer，比较时把业务成本和验证仪器分开。

还存在一个明确平台缺口：固定Snitch源码限制至多一个DMA，Carfield只把FPCluster端口经IOMMU，其余master旁路。因此这条应用有真实设备消费者，却**还不是当前四iDMA共享IOMMU的竞争任务**。不能把两个平台的现成源码拼成已运行链。当前Hero的三个硬件gate缺实现、默认拒绝pin，原生功能状态继续继承A2。[完整source合同](../out/a3-consumer-position-rules-20261015/source/source-contract.json)

## 2. 新发现的普通能力与最小版本

固定runtime的`__dma_start_1d_wideptr_base`返回32位transaction ID，`dma_wait(tid)`轮询completed-ID，`dma_wait_all()`轮询busy。AXPY所有issue都丢弃返回ID，只用wait-all。源码位置是runtime:238–286/429–458、axpy_snitch:79–98/135，root已直接复核。

第t+1轮wait-all同时等待下一输入及WY[t]写回，虽然K[t+1]只使用另一奇偶bank。可先发展这个有界普通版本：

- 保存倒数第二轮未来预取返回的两个最后输入ID。
- 仅最后tile入口等待这两个ID，然后保留原all-core barrier并计算。
- 此时`rest=0`，没有后继prefetch覆盖旧Y槽；旧WY仍可持有旧bank，当前计算使用另一bank。
- 最后输出、末wait-all/fence/barrier、host cache获取、consumer和全部释放原样保留。只有一tile时维持原方案。

该方案只需保存两个ID，逻辑上8B元数据，不增四槽、不增加小传输许可或新反馈接口；寄存器/栈、poll/fence及供给成本待实际编译/验证。**未应用或编译这份新变体，也未声称原生完成ID已有足够物理保证。** helper可用是事实；固定RTL的累计完成、ID回绕/错误、最终R/W/B含义仍需定向核查，实际同时pending窗口未观测。

一般中间tile不能机械替换：RY[t+2]会写回与WY[t]相同的Y-bank，可能覆盖仍被DMA读取的结果。因此中间版本要另加Y槽reuse guard；最后tile利用不存在下一次覆盖，形成可审查的最小普通版本。源码所有权推导由root提出、source席独立核原件，详见[规则设计§3/§7](../out/a3-consumer-position-rules-20261015/method/design.md)。

即使这个版本提前计算，也不自动提前完整任务。DMA读取与计算可能竞争TCDM bank，输出、共享DDR、翻译/cache状态和最终host/释放仍有费用；不能用只看compute ready的图宣布完整收益。

## 3. 方法细化为四类证书

| 规则 | 可检查依据 | 能给的结论 |
| --- | --- | --- |
| **W 等待范围/回收** | 实际consumer输入、公开完成闭包、冲突访问、后继overwrite及最终排空 | 现成API有合法放行机会；若只核源码，不升级为deadline满足 |
| **L 必经义务下界** | 每个允许实现/策略共有的消费者、已承诺及回收边，有来源的服务下界 | 只有更早已计费方案U与晚位置全策略下界L满足U≤D<L，且完整目标通过，才说必须前移 |
| **S 普通可行见证** | 一个具名在线普通方案在全部允许环境下满足完整向量及释放约束 | 推荐普通方案，不要求证明它全局最优；缺服务时仅给源级能力结论 |
| **N 完整无影响** | cut时间相同及全部后继相关状态等价，含cache/TLB、owner、在途R/W/B和服务 | 才可说完整目标不变；只看到另一个等待未结束，只能说局部屏蔽 |

[具体输入、算法、证明与失败规则](../out/a3-consumer-position-rules-20261015/method/design.md)有两项相对A1必须显式加严的条件：

1. 下界覆盖的**允许实现族**须包括CPU/原buffer/普通页表等。若CPU绕过DMA，“DMA必经”就只对设备实现成立；不允许用一个实现的路径证明全局必要。
2. 从某条baseline轨迹抽到的旧债务只给prefix之后的条件结论。要从共同就绪时证明所有晚策略均失败，须证明该prefix不可避免或覆盖其他prefix，不能排除合法普通节流。

这些上下界/标准图逻辑本身不是新算法；候选增量是能重复从共享翻译接口及consumer抽取约束、给出有用的部署决定。

## 4. 本轮实际验证与同信息标准比较

执行一次[符号规则检查](../out/a3-consumer-position-rules-20261015/validation/check_rules.py)，[8项通过](../out/a3-consumer-position-rules-20261015/validation/rule-probes.json)：最后tile抽象机会、旧输出已退时差异消失、中间tile覆盖错误、单tile无额外等待、累计ID闭包、最终输出仍须排空、CPU实现破坏全局必经DMA等。

参考方法得到完全相同的生产者、owner、公开完成前缀和目标，直接展开依赖/资源约束；在四个完成前缀状态上与规则判断一致。**没有展示求解算法的独有结果，也没有运行TC2023公式、LP求解器或完整时序工具。** 这项检查只回答规则是否与同信息标准建模一致，不回答论文新意或时间准确性。

无时钟/任意服务时延、无payload运行、无实际AXPY episode/总线/RTL/内核/编译/FPGA/板操作，未回放旧四案八次或A2检查。输入源SHA和实际命令留在[执行账本](../out/a3-consumer-position-rules-20261015/validation/execution-ledger.json)。8项符号检查不是四个时间案例或新留出通过。

## 5. 新意边界和下一决定

RTAS2026已覆盖FIFO外调度、合法分块和完整任务分析；TC2023允许任务内任意到达，但另有读写独立、pipeline不满等假设；MASK/DWS已有联合服务/局部交错能力。沿用[用户的创新分析](research-novelty-assessment-20261015.md)及[原文定位](../out/research-novelty-assessment-20261015/analysis-priors.md)，没有重复广查文献或重新签发阅读状态。

本轮增量是：真实consumer及四槽回收推导出“最后tile可优先试已有ID、一般中间tile需另保护”的具体工程决定；同时把全策略下界的实现族和prefix量词要求具体化。**当前仍缺自动/可复用抽取的实证、未见任务、真实多端点和完整预测，不能据此接受论文主贡献。** 同信息标准参考得到相同答案，求解独有性不主张。

下一批按两个有界问题推进：

1. 先沿固定DMA frontend核completed-ID的接受/退休/回绕及编译器/硬件ordering，实施并编译最后tile普通变体作为强对照；原四槽、最后排空与完整业务工作不变。没有对应硬件gate和身份时不运行native或性能。
2. 单独绑定一个真实第二publisher/consumer与同一IOMMU，而非人为给当前CPU DDR staging设“必须DMA”。CPU实现若仍合法则保留；若要用CPU预算/设备位置限定，须从应用取得实际要求。

只有上述真实合同、合法有用窗口、有限服务及全成本闭合后，才冻结一正一邻反两时间留出。没有新目标或硬件安排时，不自动补Hero gate、改FPGA、扫压力/相位或扩工作负载矩阵。

方法：受限源码合同研究＋形式条件推导，使用已有程序/运行时/RTL作次级原始数据；验证标准是来源可追溯、owner安全、量词正确及同信息参考一致。局限为单AXPY、人工摘要、ID物理语义/真实窗口/多端点/成本未闭合。由Codex辅助完成，root承担实质机制、关键原件、执行与裁决；一名Sol独占席核source，未用DeepSeek/Claude。两周10月15—28日及历史用时保留，论文未完成。
