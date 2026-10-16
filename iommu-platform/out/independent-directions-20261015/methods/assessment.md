# 两个方法型候选的独立比较：A/A3与决策观测

2026-10-15；Codex独占关键判断席，沿用户指定Sol/ultra。已读最新AGENTS/CURRENT_STATUS，取消A默认优先。本报告只比较两种方法型方向；不替root评选容量/放行/共享服务机制，也不重复其他席的准备/生命周期设计。没有新实验、仿真、构建、goal、外部模型、生产或全局文档修改。

**本席建议：在这两个方向中，优先发展D的“面向实际控制选择的有限观测”窄版；A/A3保留为分析资产和普通强对照，不再凭现有框架优先。** D不是已经通过的新贡献：优先的理由是它提出了一个可以被明确推翻的信息与动作问题，并直接面对共享翻译研究中经常选错控制的风险；不是因为已有E2/E3样本多，或诊断平台容易包装成论文。D若不能证明有限观测确实改变有用动作，便降为机制论文的支撑工具。两个候选目前都没有被接受为独立论文贡献；这个事实不妨碍选择更值得优先判别的新问题。

## 1. 两个候选的最强版本及各自真正要增加什么

| 项目 | A/A3：源合同与控制位置 | D：有限观测与实际动作选择 |
|---|---|---|
| 真实问题 | 软件完成API、DMA/翻译义务、buffer复用和consumer不一致，容易在失效的控制位置投入工程 | 相似miss/周期统计可能来自不同等待组织，因而选错P、N、PN或限流；更多计数不一定帮助决策 |
| 最强合理版本 | 从受限且具名的真实API/接口自动或可重复抽取约束，给出有用的普通方案/位置/必要能力及完整后果证书 | 在固定位置与已存在的动作集合中，识别哪些观测缺失会造成实际选择错误；以少量可获得信息/有界校准减少完整决策损失 |
| 要新增的认识 | 哪类源/协议关系能由可复用规则推出不同部署决定，而不是人工补全标准图 | 什么状态在现有观测下不可区分但需要不同动作；哪项额外联合时序信息能及时区分、何时取不到/不值得取 |
| 不能当增量 | W/L/S/N命名、上下界不等式、VALID锁、完整计时、普通wait-ID或DAG本身 | 单个平台、更多日志、替换/排队现象、延迟注入、原因标签更准、泛称可解释计数器指导策略 |
| 与另一方向的接口 | 给定源合同与服务，分析位置/能力；可能完全离线 | 给定固定位置和合法动作，利用运行中或下一同类任务前已得到的有限数据选动作；不证明最早必要控制位置 |

这种区分允许共享底层证据，但不把D改名成A，也不要求D先做通用C/RTL语义抽取器。

## 2. A/A3当前的实质与最近强对照

A3最实在的产物是：真实AXPY的四个2048B L1槽、已有transaction-ID API和下一次覆盖关系，共同指出“最后tile可先试输入ID等待，中间tile机械替换不安全”。这是有用的源级工程发现，不是没有价值。其源、真实窗口和原生完成ID仍有未核项；固定硬件只有一个DMA publisher，并非当前四iDMA共享竞争任务。[当前事实](/home/xvhaocheng/work/iommu-platform/docs/a3-consumer-position-rules-20261015.md:9)。

决定性反对证据是原验收明确记录：8项符号检查通过，**同信息标准参考在全部4个抽象完成前缀给同答案**；没有数值下界、完整时间、自动抽取器、未见任务预测或独立论文贡献。[root原件:5](/home/xvhaocheng/work/iommu-platform/out/a3-consumer-position-rules-20261015/root-result-acceptance.json:5)、[未接受项:20](/home/xvhaocheng/work/iommu-platform/out/a3-consumer-position-rules-20261015/root-result-acceptance.json:20)。W是依赖闭包+冲突检查；L是全部实现/策略共有约束的标准下界；S是普通可行见证；N要求完整相关状态等价。其正确性要求必要，但不能靠重新命名形成新算法。[规则原件:38](/home/xvhaocheng/work/iommu-platform/out/a3-consumer-position-rules-20261015/method/design.md:38)、[同信息比较:121](/home/xvhaocheng/work/iommu-platform/out/a3-consumer-position-rules-20261015/method/design.md:121)。

已核前作不重读不变原文，直接复用[原文评审](/home/xvhaocheng/work/iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:9)：

- TC2023已允许job内任意到达、在途/carry-in和层次响应分析；不能把依赖请求或完整任务泛称新意。其读写独立、不满pipeline等条件确有限制，修正这些条件可以有用，但增加输入不自动成为方法贡献。
- ICRT已有可随机访问的事务队列与组合调度，REALM已有外置分段/预算、先收W后发AW及阶段监测。不能把它们的能力免费放到本平台，也不能仅以“现接口缺功能”宣布创新。
- [RTAS2026原文](https://retis.santannapisa.it/~giorgio/paps/2026/Aromolo-RTAS26.pdf)§V-A/B/C、§VI已具体做FIFO外调度、合法DNN分块、预算保护及WCET任务分析。A若只主张这组宽构件，已被覆盖。A的逐PTE/owner反馈与跨位置抽取合同尚未被这篇完整覆盖，但当前A3也没有交出可复用抽取和不同部署判定的证据。[准确覆盖范围](/home/xvhaocheng/work/iommu-platform/out/research-novelty-assessment-20261015/analysis-priors.md:37)。

**A值得继续的条件**不是必须发明新求解器：至少用同一抽取规则处理一个未参与规则设计的真实任务，事前发现普通建模直接应用没有给出的有用部署决定，并可审查其抽取正确性、适用边界与全部代价。需要和同信息普通依赖分析、人工建模成本、现有runtime能力公平比较。若只是最后tile两ID的工程优化加标准图说明，应保留为普通对照/工程成果，不单列方法论文主贡献。

## 3. D最强前作：已做什么、留下的准确问题

下表是本轮定向核决定部分的原始方法；不是按标题或摘要判覆盖。读到的范围与没读到的范围分开。

| 原始方法 | 已有实质及决定原文 | D不能重复／仍可检验的差异 |
|---|---|---|
| [Bermuda，RTCSA2025](https://arxiv.org/pdf/2508.19670v1) | §III/IV用ZCU104商用SMMU、PMU+ILA、共享TBU的独立FPGA/FPD DMA、不同页压力、SMMU on/off和小payload控制分析干扰。[txt:129](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:129)、[393](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:393)、[411](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/bermuda2025.txt:411) | 设备共享IOMMU会干扰、缓存压力微基准和开关对照均已有。其interf_tcu是压力配置，不是内部全部原因独立份额的证明；未给有限观测改变具体P/N/限流选择的完整决策合同。 |
| [Dead on Arrival，arXiv:2606.00486v2](https://arxiv.org/html/2606.00486v2)；决定性最近前作 | §4.1/4.2为Accel-Sim实验；§5.2/5.3以相近重走比例/MPKI、收益悬殊的atax/bicg说明VPN共享和MSHR死槽突发的区分力；§6为Bloom/PDS/保护计时器；§7.3组合LatPC出现正负效果。[方法:355](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/methods/dead-on-arrival-v2.txt:355)、[判别:616](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/methods/dead-on-arrival-v2.txt:616)、[机制:684](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/methods/dead-on-arrival-v2.txt:684)、[组合:806](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/methods/dead-on-arrival-v2.txt:806) | “普通统计近似→结构观测→不同优化收益”以及“两个优化组合可能退化”均已有。§5声称可作runtime判别，但§6实际保护规则不以突发分类开关；不能将它说成已实现本文全部付费动作选择，也不能仅换成IOMMU重述其模式。 |
| [RISC-V SVA，DATE2025](https://arxiv.org/pdf/2502.17398v1) | §III/IV以FIFO型AXI delayer可配置延迟B/R，并用真实kernel比较CPU、copy/map、offload和device；按阶段分解。[txt:114](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:114)、[147](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/riscvsva2025.txt:147) | 可控延迟与完整任务breakdown不是新意；这里延迟整个内存端，也不能直接当逐owner/PTW干预。 |
| [CounterPoint，ASPLOS2026](https://cs.stanford.edu/~trippel/pubs/lindsay_ASPLOS26.pdf) | §2–4把微操作路径的事件组映射为模型锥，结合相关噪声置信域做可行性反证/模型修订；已有缺计数器时错误模型不可检出的例子。[txt:280](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/counterpoint2026.txt:280)、[330](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/counterpoint2026.txt:330)、[351](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/counterpoint2026.txt:351) | 模型+多计数器+反例校核及“某观测不可缺”已有。它主要检验模型与计数一致性；D需证明对实际动作选择的价值，不能只输出原因分类。 |
| [Coz，SOSP2015](https://arxiv.org/pdf/1608.03676) | §2有随机化虚拟speedup、progress points、基线及扰动费用校正，以测优化对吞吐/延迟的影响。[txt:130](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/coz2015.txt:130) | 扰动找瓶颈和优化价值已有；E3只延迟一次PTW响应，不满足Coz暂停其他线程并扣时的虚拟加速等价，不能由加延迟敏感性直接推出消除延迟的收益。 |
| [MISE，HPCA2013](https://www.istc-cc.cmu.edu/publications/papers/2013/mise-hpca13.pdf)及[ASM，MICRO2015](https://users.ece.cmu.edu/~omutlu/pub/application-slowdown-model_micro15.pdf) | MISE§3/4短期提升请求优先级估计alone服务率，另处理compute fraction；ASM§3/4再结合辅助tag估计缓存容量干扰，向QoS/公平控制供给slowdown。MISE已指出单请求等待可被并行隐藏。[MISE txt:161](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/mise2013.txt:161)、[219](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/mise2013.txt:219)、[ASM txt:292](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/papers/asm2015.txt:292) | 带干预的性能估计、联合cache/memory处理、分期在线控制已有。D须在具名适配中展示翻译反馈产生的具体估计/选择失效，而不是称一般性能归因全新。 |
| [ObservUVM，ISCA2026公开原件/实现入口](https://github.com/csl-iisc/ObservUVM) | §IV把样本页移到DRAM并复用access counter，付费取得关键HBM region观测；§VI的Tournament按回访fault归责并淘汰策略，已有反馈选择和负反馈预取。[txt:368](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:368)、[524](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:524)、[566](/home/xvhaocheng/work/iommu-platform/out/shared-translation-literature-20261006/papers/observuvm2026.txt:566) | 采样+现有硬件+观测指导策略并非新。目标是显存超额订阅/evict/prefetch；D需要在共享翻译服务中给与该机制不同的具体信息/行动关系及净值。 |
| [HELM，SC2025](https://doi.org/10.1145/3712285.3759812) | 本次出版社搜索索引返回§2、指标定义、§5.3及§6正文：每allocation fault重复/span/density；MIM用其选择placement/migration。指标跨完整runtime聚合，正文明确MIM为offline整应用策略。 | “可解释遥测→策略”已有。在线有限窗口、观测时机与完整切换费用仍是可比较问题，但不能凭HELM是offline就宣布创新；ObservUVM/MISE已有在线强对照。直接DOI/full获取均403，未取得整篇原件，故只核以上返回段，不对其全部能力或实验边界下覆盖结论。 |

本轮没有取得HELM作者artifact、重做CounterPoint统计或复证Coz/MISE/ASM数学。已有[10月6日候选③](/home/xvhaocheng/work/iommu-platform/docs/translation-comparison-20261006.md:85)已提出“相似观测需要不同动作”；本报告是在它上面具体发展，不冒称今天首次提出。IISWC2024的address-translation scaling论文仅检得题名/方法摘要，本轮未深读，不能用其摘要判候选被覆盖。

**DOA收紧了D的立题边界**：简单增加复用/扇出/突发指标已经不够。D必须在给予这些可用信息的强对照下，识别跨PTW与payload的具体信息缺口，改变具名P/PN/限流的完整动作选择，并说明信息取得的净值；单列新终点、换平台或只画新的结构指标相关图，都不足以支持这版方法贡献。这里提出的是待验证推进，不是宣称DOA没有全部共享资源分析能力。

## 4. D的具体可推翻版本：不要解释全部原因，先纠正一个实际决策

### 4.1 固定研究合同

先固定**一个实际位置**和合法普通动作集合，例如现有P、PN及一个冻结gap；N可作为第四项，但不强行扩大菜单。目标为在后台完整任务/进度约束和同资源预算内缩短V的完整完成；使用双方、consumer、资源归还与quiet的完整向量，不能以hit率或某个请求快几拍代替。若动作只能idle时配置，就只对下一个有真实重复来源的任务选择，不能假设运行中无代价切换。

基础观测`O0`预先限定为当前可读的per-DID lookup/hit/miss、walk工作量、完成字节/任务数、pending和各自总周期等；**还要给基线同样可取得的复用、请求合并/扇出与占用突发信息**，按真实事件/噪声及成本定义，不能为了制造不可分只保留弱边际计数。DOA所说的既有GPU occupancy计数不能当成本平台已有；若取得需新增硬件，两方按同一费用计。普通基线包括`O0`的简单阈值、已有固定P/PN/gap、DOA式结构判别、同预算MISE/ASM式估计和观察反馈选择；若任务可重复且模式稳定，还必须计入“完整任务逐个试选再使用经验最好者”的具名普通程序。

### 4.2 核心可检验假设

**H-D：在同一冻结合同、动作和目标下，存在两种源绑定的合法共享翻译状态/阶段，给予上述强O0后在预设测量分辨率内仍不可分，但P→PN对完整目标的净效应符号相反；新增一个小的跨资源联合时间关系观测后，可以在动作发生前或下一同类任务前区分，并以较低总成本降低错误选择的损失。** 不同任务也可研究，但必须采用相同合同定义和完整起点/终点；不能借两套不兼容测量制造反转。

新增观测的第一版只试两类候选量，不先造大collector：

1. 已结束窗口里，V请求等待与另一owner的活跃PTW在同一服务链上的联合时间关系；若随后才确认V为hit，只能在该结果已可见后回溯统计，不能给早先决策偷用未来的hit真值。
2. 同一已结束窗口里，某owner的PTE处于pending，而共享服务出口正在返回另一owner payload的联合时间关系。

这些是**有身份的共现/次序信号，不是已证明的因果份额**。两项总周期不能相加成“可收回损失”；pending不自动证明HOL，payload返回也不证明它是全部PTE等待的原因。须直接核该位置的队列/owner定义，并用实际完整动作反事实验证选取价值。若O0本来已有同等信息，不能重命名成新增观测。

这项发展有一个具体认识目标：**在已知复用/扇出/突发后，PTE与载荷跨owner的联合时序是否仍决定保护、共享放行或限流的净选择？** 若回答为是，再给出一个小观测何时足够、何时须付费校准或明确弃权的可用条件。这仍是假设，不是当前数据已经证明。即使成立，也只说明冻结观测菜单和任务族中的必要信息；不称全局最小观测集，也不把通用分类器/价值阈值当作新算法。

### 4.3 干预与误差：区别诊断和控制价值

对有限新增观测仍不能分开的状态，候选方法可以请求一次有预算的校准，优先用已有P/PN/gap合法切换而不是假设新MC特权。干预规模、相同起点/映射/任务、保留旧义务、counterbalance和复位/排空费用必须冻结。当前E3的单次PTW延迟只能作为局部敏感性资产，不直接变成这种完整校准。

报告选择错误应以**所选完整策略与同条件可实现完整策略的目标差距**度量，并列后台/释放约束违约；完整记录、观测器面积/时序、采样开销、校准、重播、warmup、切换及适用的摊销全部计入。离线全信息oracle只作界/真值入口，不能冒充已实现普通控制。若动作改变后继请求流/缓存状态，比较完整闭环轨迹，不能逐时刻把局部best拼成一个不存在的策略。

具体可否决点：O0已足够，尤其DOA式结构信号已消除差异；新信号到得太晚；两状态没有真实来源；普通全任务试选/反馈程序同成本达到相同结果；新信号只能提高原因分类；collector或校准成本吞掉收益。任一成立就停止对应D版本的独立策略/方法主张，保留诊断资产；不扩大为整个共享翻译问题无价值。最强审稿质疑是：你是否只把DOA的结构指标故事和MISE/ObservUVM的估计/选择套路移植到新平台？必须以强O0仍出错、具名新增观测纠错、未见状态预测与全成本结果回答，不能以文献未提同名指标回答。

### 4.4 若主贡献落在可复用基准/反例方法

不必强行发明新controller。可以把“同观测、不同完整动作后果”的反例作为基准单位：每个实例给出真实任务来源、共同起点/完成合同、已有观测的固定投影、可执行动作矩阵、采样/干预费用及至少一个独立未见变体；用它揭示并复现具名估计或选择方法的失效条件，再验证小观测/校准能否消除失效。反例成立须是同合同完整动作结果，不是两条旧日志各取一段。这样的贡献取决于发现有意义且可迁移的跨资源决策边界和可重复结果；一般“相同输入不能判不同状态”的证明、通用因果框架和日志格式本身均是普通构造。若最终只整理现有E2/E3/PN，仍只是资产归档。

## 5. 与已有资产的关系：不能把旧局部结果拼成新全因果

历史已证E2特定布局替换→首次miss；E3单启动64B人工延迟的局部等待传播；PN旧实板有约12.7%组合退化；M0另一行为DDR合同里普通服务规则消掉3689周期差额。[历史裁决:34](/home/xvhaocheng/work/iommu-platform/docs/research-panorama-audit-20261011.md:34)、[模型/板测边界:60](/home/xvhaocheng/work/iommu-platform/docs/research-panorama-audit-20261011.md:60)。

这四件事共同说明**诊断问题值得问**，不能共同证明H-D：不是同一任务、同一配置或同一完整状态；E3的第一个候选关系已有局部仪器雏形，第二个关系只在行为模型完整轨迹有依据。没有在同一原生合同里证明两者同时可取得、O0确实混淆、或用它选择P/PN比普通程序更好。旧板端根因和自然损失仍不能由模型替代。

固定gap的取舍、method88冻结强对照负结果及M0完整双方完成可以直接复用，避免只保V局部窗口而隐藏A代价。原MIG collector时序失败/paused是工程边界，不是D的科研负结果，也不自动授权恢复构建。诊断新信号若需要同样昂贵的全历史collector，D优先理由可能失效，须比较更简单已有计数/短轨迹办法。

## 6. 为什么本席优先D，以及下一项有界判别

**仍优先D的窄版，因为它把未解决问题落在“已有强观测之后，哪项跨资源信息还改变哪个现有动作”，可被一对具名相反状态和完整决策后果直接推翻；无需先接受A的通用源抽取任务。** DOA加上CounterPoint/HELM/ObservUVM/MISE/ASM使宽泛诊断故事失去独立性，却没有替本平台证明收紧后的H-D。优先的是先查这一决定问题，绝非现有证据成熟度更高或论文贡献已成立。通用工具能表达这一假设，不是已发表覆盖；相反，仅换到IOMMU场景也不是贡献。

A3当前最强真实发现则由现成API和标准依赖规则直接得到，尚未展示抽取/部署方法对未见任务的独立价值。若root将主要精力用于共享IOMMU性能研究，我不建议为了保留A主线而把大量投入转为新的通用程序分析工具工程。A可以帮助检查D的合法性与完整目标，但不是D获选的前提。

**建议的下一项独占交付**：先在不新增硬件前，形成一页“H-D相反状态对”：真实任务与共同目标、固定位置/动作、O0准确现有来源与噪声、两个状态为何被O0混淆、拟加联合观测的实际信号入口、不同动作后果的可检验预测、完整费用清单及简单试选对照。可以先用有来源的小模型证明可区分原理，再要求原生任务支持；不得把人工改相位所得模型例子记为真实状态。此门通过才冻结新实验。若源核表明无法形成有意义状态对，应让有明确需求的其他机制方向领先，不因本报告选D的有界判别题就把D设为长期默认。

独立贡献的接受点是：**有限共享翻译观测缺口＋取得信息的具体办法＋在冻结未见状态中改善完整动作选择并覆盖费用**，或有可复用价值的诊断不可辨识边界；不是要求现在已经有性能实证才允许立题，也不是把“有机会”写成已证创新。主会话仍须和其他机制/生命周期方向平等比较后决定总排名。

## 7. 未核与证据身份

- 本次复用TC/ICRT/REALM/RTAS2026既有原文核查，不重复读不变全文；新读A3验收/规则，以及Bermuda、DATE2025、CounterPoint、Coz、MISE、ASM和ObservUVM的决定方法段。方法段阅读不等于其实现/全部定理重验。
- DOA新核§4–7.3并保留上述局部原文入口。作者在[原件:57](/home/xvhaocheng/work/iommu-platform/out/independent-directions-20261015/methods/dead-on-arrival-v2.txt:57)写明IEEE投稿，本报告只按预印本论方法，不称正式发表。未复现实验；原文的零退化措辞与§7.1小幅负数并不一致，未将其当成保证，也未采用内部不一致的huge-page容量数字来推论。
- HELM此前只有出版社元数据；本次检索实际返回了所列关键正文段。可复核短摘录为“**As an offline tool, MIM treats applications as one large kernel**”（§6，13英文词）。检索原始入口为上述DOI；直接网页两次403，未下载全文/运行artifact，也未改旧文献库的阅读状态。本文对HELM的判断限这几个段落。
- 本轮未做PDF页锚结构验收；引用本地抽取txt的精确行和原文章节。没有用页码检查未通过冒充通过，没有新性能/硬件数据。
- 新联合观测的可达性、物理成本、窗口内状态混淆和收益符号全部是待检验命题；未证信息来自同一真实任务时不拼接E2/E3/PN，未声称所有前作缺失同一方法。
