# D边界来源裁决：当前选策版本降优先级，保留研究资产

## Material Passport

2026-10-15，D-BOUNDARY-SOURCE-2。ARS-Codex experiment-agent/plan的有界问题发展、来源核查与机制比较；不是完整pipeline、统计验证或论文完成。共享研究Codex-only。两个源码席请求gpt-6.1-sol/high；root直接做机制比较、关键原文核查与裁决，后续文档请求Sol/medium；实际model/effort未知，未声称主会话切换。

**本轮把D“上一窗口有限观测→下一任务P/PN选择”的当前版本，从优先自动推进降为待新证据触发的候选。停止新增观测器、状态扫测与完整动作扩测；不自动提高I优先级或退回A，也暂不指定替代主线。** 这是基于累计来源与决定性证据的投入裁决，不是证明整个共享IOMMU观测问题无价值。完整记录见[稳定接受](root-result-acceptance.json)。

## 真实来源支持什么边界

| 来源/边界 | 可以接受的事实 | 不能据此接受的能力 |
| --- | --- | --- |
| 本机composition连续两次ioctl | 可为下一次实验选择mode；每次在全局mutex下configure，重新alloc/init/map/flush/warm，运行、drain/verify/capture，restore S/DID10后unmap/free | 同一持久应用池内，在两task之间用上一窗口观测切P/PN并继续原task |
| E3 pair两轮 | 同一调用内复用stream/mapping | 两轮之间任意输入P/PN；第二轮实际reseed/flush/warm，composition参数为NULL |
| SVA用户arena | 私有研究ABI接受用户虚拟内存；每次pin/建research domain/map/attach，完成后restore/unmap/release/unpin | 已实现的持久DMA binding或P/PN控制；用户可自行保留VA不等于保留设备映射 |
| 固定SPDK perf普通callback | 同一task/buffer完成后立即补交，保留slot寿命 | 单笔callback就是全owner quiet；QD=1的瞬间零在途也没有整体quiet/config承诺 |
| SPDK warm→measure | 保留task/buffer，清软件stats/histogram后持续供给 | 已停补交、联合排空、可安全切mode的自然应用phase |
| SPDK terminal drain | 停补交、处理已接受请求，callback释放task/payload，随后释放qpair；main等全部worker后退出 | 保pool的pause→config→resume；draining并非可恢复暂停 |

本机来源卡见[native findings](../native/findings.md)、[机器卡](../native/boundary-card.json)及[精确原文](../native/source-evidence.md)。外部来源固定SPDK v24.09 commit `19524ad45373e35d5fdc06f4e081444d87ceed80`，见[持久来源卡](../persistent/episode-source-card.md)、[原文与费用](../persistent/findings.md)、[原文摘录](../persistent/source-evidence.md)。这些是源码事实；未证明当前内核/BIT/设备或SPDK实际运行绑定，也未证明所有应用都没有所需边界。

## 配置会清掉什么，保留什么仍未知

root直接核生产连线：配置需要IOMMU生命周期、全部DMA job、observer在途与launch状态共同quiet，窗口非ARMED/ACTIVE；成功配置原子clear所有IOTLB valid和PLRU。相同mode重写也会clear并增加generation。旧pending义务必须先完成，不能作为下一quiet边界后的剩余积压；旧命中驻留和PLRU也不能直接保留使用。[硬件原文卡](hardware-boundary.md)逐项列出文件行号。

这不意味着DDR状态被reset。硬件quiet并未证明全部内存内部恢复成熟或状态冷启动；上轮私有WR+16/WR+34不是原生保证。已验证的完整COPY准备只使**选定两前史的有效DRAM状态**相同；未证明任何可能输入都相同，也未证明完整RTL/IOTLB世界等价。[前轮决定性结果](../../d-dram-action-20261015/root/decision-analysis.md)保持原样，不重跑。

## 初始化与每次准备：费用不能被计时窗口抹掉

composition的pool不是设备probe时建好、每task复用的公共对象，而是每次ioctl的局部stream。V1/A8明确包含4次DMA alloc、总请求73728B缓冲区及同量显式CPU初始化写、18页显式额外map与后续unmap；allocator内部额外映射/清零等仍未展开。每次还有两次配置（进入与restore）、warm反馈、HPM/flush、验证与释放。所有时间费用未知，不能填0。

SPDK的env/qpair/初始slot准备则主要发生在一次invocation，正常callback不重新分配；每IO仍有命令/offset/提交/完成工作。可选warm是整个invocation的连续运行统计阶段，count模式拒绝同时warm。它没有说明固定M0 warm256可以降低，也没有给出一个stock多episode恢复协议。

另一个实际信息时机是：本机configure早于stream_prepare，caller选择mode时尚未取得本次新分配PA/IOVA；composition结果有128word raw快照却没有通用pool/mapping句柄。若新实现合法地先取得布局再选择，普通控制同样应获得这些信息，不能把当前调用次序人为制造的信息差当新传感器价值。完整费用/最早信息表已进入两张来源卡。

## 比较后的最强保留版本

root独立比较了[两种正向设计](mechanism-comparison.md)：

1. **公开历史与布局预测。** 给普通程序全部合法可得任务、映射、复用/扇出/突发、提交/完成与已有计数，选择固定P/PN；正常重复来源成立时加入完整付费试选。可以比较把一次选择移到分配已知之后的普通控制位置，但须另核实现和费用。
2. **新增跨资源关系观测。** 在相同信息、动作和完整目标上，加入已结束窗口内带owner/请求身份的PTE等待与payload服务关系；若需要持久pool，明确新增admission pause、idle task、联合barrier与resume。普通规则也获得同一付费接口，分别裁决接口信息和选择策略。

第二种保留了原idea最有意义的部分：检验跨翻译—载荷反馈的关系是否真的改变完整决策。但本轮未取得跨合法边界仍有效的具体关系缺口、完整P/PN净效应反号或新增费用。保pool暂停本身是普通runtime适配；没有这些证据时不值得先实现大适配层或collector。第一种也未被证明足够，只是必须给予公平机会的可执行强对照，不能以“普通程序可想象做到”宣布D被论文覆盖。

## 为什么这次应降低投入优先级

这是合并证据后的有限判断：固定M0未给出需要新信息纠错的正负完整动作对；后续CS1付费菜单和MQSim有限覆写残留版本各已局部停止；独立DRAM源状态确实可持续，但经过所选完整COPY准备后该前史对收敛；现在再核自然应用phase与合法mode边界，现有两个来源并未提供目标持久控制能力。要继续当前版本，至少还需新runtime边界和未证实的预测关系，两者费用与收益都空缺。当前已有资产已经足以作出“先停止自动扩测”的决定，不需要再扫描状态或反复审查来推迟它。

未接受的结论是：整个D不可能、所有真实状态都会被warm洗去、ordinary O_host已经充分、全部前作覆盖、I或A因此更优。已有[最接近工作核查](../../independent-directions-20261015/methods/assessment.md)仍适用：DOA/CounterPoint/MISE/ASM/ObservUVM已提高宽泛观测选策主张的门槛；本轮不重复调研，不新增“首次”或覆盖主张。

## 下一步与保留条件

下一项是[RESEARCH-REPRIORITIZE-1](next-minimal-contract.json)：把这轮来源裁决带回已有六族候选，选择2–3个有具名实际问题的最强版本做一次有限投入重排。复用原文、关闭原因和工程资产，从正常任务尚未满足的具体需求提出正向机制，明确最接近工作、普通对照和一项能改变排名的最小证据；不默认A、I、D或按旧排名安排实现。来源未变不重复全面调研、旧实验或验收。

D若取得另一个明确正常边界，或有来源和冻结假设支持的新投影，同时给出跨边界关系、同信息强对照和全部费用，可以重审相应版本；不要求先做原生jointtrace才准所有模型，但也不能用任意隐状态替代来源。I仍not_ready的轻量预筛，不能因D缺证自动提升；A5/A2和其他独立工程成果完整保留。10月15—28日窗口不重算。MIG1792023314继续paused，WNS−0.391ns/598 setup失败/0新load，实际债务unknown。

本轮0构建、0模型/程序实验、0旧实验重跑、0设备/硬件操作、0生产Linux/RTL/ABI修改。只有定向读取、源码摘录/哈希核验、设计裁决与文档更新；不把静态来源核查写成运行验证。
