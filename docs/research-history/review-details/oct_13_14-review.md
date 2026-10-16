# 2026-10-13—14 历史审阅单元

本单元119个分块、37个来源均已逐块显示并阅读全文。覆盖根CURRENT_STATUS、HANDOFF中分配的10/13—14完整段落、validation相应段落、33份docs来源（含具名来源/机制/实验报告与计划片段）。覆盖依据是 `ledger-oct_13_14.json`；HANDOFF、validation与计划的其余部分由其他单元负责，本单元不声称它们单文件全文。重复关系：HANDOFF与validation大段同步重复，报告给出独立设计/执行细节；保留重复记录有助于核修订，不应把119块当119项研究。

另外通过已授权 `dell` SSH阅读全文6个out文件：RCS根裁决、32行结果表，BR1根裁决与后继合同，AW run-result和六案G0 README。默认沙箱网络拒绝后，经只读授权通道成功；未改网络/SSH/文件配置，未运行科学测试。该新增读取是派生数值表及执行回执抽核，未独立复算事件或原始字节，不代表重新验证实验。

## 10月13日：应用来源的实质筛选，不是只有CO1

stage111的普通R107恢复已留实板验收（本单元读HANDOFF/validation当日条目）：实际boot370a9dd6…/S0，4×4MiB backing，8真实2MiB leaf；priorout和restore各1769472B/432tasks，总864硬件task、144batch。恢复1728hit/0miss/0walk，冷换出8miss/8walk。24worker-layer/12joint是全恢复后CPU字节核验记账，不是原生attention/collective执行。原pre-NEXT EBUSY为软件idle判定错误，最小修复及一次实板pilot完成，不能解释为硬件平台不可并发。

stage112核vLLM TP2层依赖与24次RowParallel归并，活动前缀每rank12288B。GraphAllReduce持久buffer与旧backing slack可容纳两前缀，未增加leaf。归档的是当前FPGA共享翻译缺口投影：partial输出/算术consumer/真实requester/共享路径没有成立，且源事实未排除稳定映射。不是collective科学阴性。

stage113—115选DeepSpeed v0.16.4 ZeRO-Infinity NVMe tiled CPU-Adam：区别真实p/m/v更新、app pinned池与NVMe按request map/unmap。两AIO句柄不等于两PCI requester，pin不等于持续IOVA。源码推导稳定期AIO `6ΣL_i+8ΣQ_i`，完整成功bytes和拓扑仍缺。stage114原论文0.5T单节点需求约p/m/v 6TB>名义CPU1.5TB，排除该规模全CPU驻留；不能排除固定tile/pool、大页、persistent map、单设备。固定官方小测试只得到256MiB预期AIO合同，测试源码未运行；sub_group_size100并不拆weight成100元素tile。stage115官方NVMe模板无关联run，SC22原日志实际CPU offload，Discussion7447是参数NVMe/optimizer CPU，不能拼成NVMe CPU-Adam实跑。取包goal先active→blocked→观察paused，stage116条件封存；状态改变不计研究贡献。

stage116把来源/接口与成本数值获取拆开，避免要求外部包先含所有本研究实验；仍不准把NVMe改固定DDR copy后宣称保留原按request mapping因果。

stage117第一来源Holoscan 2.7.0双目RoCE：持续MR、GPU数值consumer成立，actual translated共同路径未证。RDMA输入原无host源读，而DEFAULT DDR copy增加翻译源读；现WRITE只有64bit寄存器字，无图像stream payload接口。stage118给needs_named_adaptation，图像投影条件归档，未实施stage119。

stage117-r2 DPDK dmafwd：独占可写RX mbuf可原地改MAC并TX，未证明必须独立copy，静态拒绝基线，不是测得零copy更快。stage117-r3改选同源vm2vm：B预发布buffer/cookie，返回A指针不满足B原地址消费，所以必要搬运合同成立；CPU直接A→B和同owner单DMA是合法强对照，不强制host staging两次copy。

其后同一个三包60/65/1514B、载荷1639B/含header1675B、四span60/65/503/1011B逐关推进：

1. 三结果：packet/descriptor/lifetime软件适配；Dell i9-14900K主机CPU一次生命周期29830ns（交付5128、observer24344、间隙358）；单DMA RTL配置至quiet1227周期，12lookup/11hit。主机ns不能与RTL周期算速度比。旧“功能满足所以性能关闭”后来明确勘误。
2. 用户修关口：一次有界普通性能比较本身就是G0，无需预先证明残余再重复。四案single/parallel排空1161/1601模型周期，真实重叠1333；single完成向量更早且少一引擎。440差额混合冷DDT、供给、数据与翻译，未归因。
3. 逐包软件glue静态关：fence.i等待CPU写buffer/MSHR，不等待全部DMA，不能由全cache flush推全DMA必须静止；跨group16B缓存行闭包无交集，同包header/payload共享行仍需绑定。公开接口缺packet join/acquire/publish/consumer/return。
4. 协议适配：17定向协议测试、同binary single/parallel两例；peer-active消费有见证，但MODEL_UNCACHED_ASSUMPTION，不能称真实cache安全。
5. 真实WT cache端口：3微测试＋2集成例；缺fence会返旧字、正确fence返新字，MSHR/写退休闭合。包1取得与used发布时peer活跃，实际consumer时peer已完成；当时未观察消费重叠，不是全域不可能。
6. 六案G0：真实缓存端口CPU/单DMA/双DMA×即时/整批，均一个新binary各一次。used向量CPU即时[114,392,2051]、单DMA整批[1455,1455,1455]；双DMA即时相较单DMA即时首包早71，后两包晚109，metadata工作与多context流量不同。普通即时控制有真实consumer/peer重叠。完整CPU指令/ALU/软件/应用/FPGA成本null，没有共享翻译损失完整因果证据，未进G1。六案不是原生CVA6 memcpy或原生DPDK实测。

## 10月13—14日：从应用优先改为机制先行的具体失败链

用户随后允许机制→有界合成microbenchmark→GPU/AI真实共享路径→FPGA。不再把实际工作流包作为所有机制探索统一前提，也未恢复旧关闭收益。R1事件证书当前安全动作被Mode7 O(1)和普通current头+准确目的AW表达；R2原序写许可提前准备先保留，再核全生命周期与37成本类别，具名WRITE成功/TL fault范围与普通O-B5S配对1329bit+C_common，独有逻辑动作/成本关闭。1329是字段合计，不是总面积。重要修正：TC内部AW handoff不等外部AW接受，W可在firstVALID锁后、READY前前进；长burst保证应以256>16+2=18和firstVALID锁表述。

F1是NormalTL firstVALID前的两顺序反馈预测。原118bit头缺fullID/LEN/SIZE/RB debt，observer/history/selector/CDC皆付费。对称R/W tau例既无真实独占server/source证据，也被固定O-turn覆盖，停止该例；不关闭F1全族。R/PTE返回cut在生产小buffer源码条件下AB/BA都不能完整结束，不能把另bank先TL称收益；实际四源DEFAULT前缀可达性unknown。

一次PTE-safe被动RTL新前缀4151row、两sole4KiB：双NormalTL VALID、双fresh安全READ、comp/DS coVALID均0。12PTE内部→DDR56、DDR→firstR32、DDR→consume88，后继birth1；208held均DDTW，payload/PTW无held。没有合法ABBA，未扫相位。源码残余关进一步只在空复位/两private512buffer/正常S1/完整DSpriority条件模型域排四因果：queue≤6<16，spill恢复≤8<准备52。这不是真实MIG theorem。

MIG4.2绑定显示已accepted payload读不能让后来PTE读越过，QoS/RD_PRI_REG只读写方向优先；私有DS优先不能当MIG免费接口。short-mn-a1软件只启动两个512buffer源，未启动3buffer源；V固定16但A target2^32在软件观察后STOP，候选可改变A工作量，所以原窗口不能直接证明固定双方收益。firstVALID合流早于TL gate，后来的coVALID不是重选机会。

PTE准备gate只能暂停尚未发downstream firstVALID的payload。局部等待/填空档被earlyPause及a≤g覆盖；R-only依赖式min(a,h)≤g未闭合W/B。完整RWB手算ADMIT/WAIT方向会反转，但具体O-envelope-derived闭式max/add统一覆盖窄域；并非完整F1已发表覆盖。literal current-full-AW两job入口被原W bank排他排除；4bit reservation证书只证明顺序，非AW接受。后续AW源码纠正：upstream默认两槽AW spill，coupled第二AR依赖源侧AW0，不依赖后一级input AW0或另一raw WLAST。

另一次AW自然入口新被动记录确实得到两源accepted coupled/nondeburst、source第二AR提前及B2/A0前驱；B2/A0持续1695拍，A读P活429拍，B物理载荷头L全无效。因此固定冷前缀停止、不补预热/重采，future-AW全族仍开放。完成3190/4145模型周期是原基线，不是候选收益。初次误把super_last当final burst分析已纠正，没有重采。

## A—H、CO1、RCS1、BR1的最新snapshot

A在途多层会合已有Neighborhood-aware直接前作；B/D简单ATC push、descriptor预翻译有披露；C连续segment已有DirectSegments/CoLT，保留完成驱动局部撤销；E共享reply口本身未证HOL，当前已有N；F sketch过近似不是权限；G TFID不替正式domain。H基础new-page credit应作普通强基线。保留的是任务后继/真实buffer释放义务的H/D/G和C局部撤销卡，尚无性能/整体FLOW论文贡献接受。

CO1区别最后翻译用途退休与真实task/buffer退休，可信登记+用途保护+release-path准入；普通OC1-min逐事件表达同动作/escape，配对1228+16E+C_common无独有必要逻辑资源。4任务64B/七key是未运行纸面图；公平token错误、非法相邻DAG均修正并留原稿。关闭仅本版，无性能运行。

RCS1随后是不同规则：完整walk后最多保全剩余完整翻译集合，化简E_q过滤refs；18key可产生RCS换p3、minrefs/pin换a、SJF换b的真实条件模型选择差异。4条件×8固定规则32episode，RCS max/sum=921/3491、923/3484、921/3491、907/3411；同一SJF=920/3478、923/3484、920/3478、900/3408。P局部提前不代表最后任务/双方收益。每episode51copy，R/W各3264B、408R/W、51B、4consumer/release、quiet闭合；这是条件服务模型，不是RTL/MIG/板测。派生32行CSV本轮已实际读，未重算116750原始事件。

BR1是当前最新：normal fill victim按owned义务至下一真实release的传播代价选择。固定DAG/单受影响节点、资源顺序和后续residency不变时StaticCP `F0+max(0,d-slack)`精确覆盖；动态service/order族unknown。新19copy/17key图只有4个相关resident、12outside-H，未有后续fill且同延迟时普通Hpin零影响tie；未给优选反转和完整净gain，0性能条件/模型。名义R/W各1216B、51PTE/408B和128B冷DC是合同算术，非完成实测。

BR1最新直接源码勘误非常重要：生产fullbuffer证书仅normal、页对齐、正好4096B且容量≥4096；64B没有permit。旧RCS1 engine给64B的例外为额外条件能力，不能再称保留native guard。旧数据/报告/engine保留，32结果仅适用该旧条件engine，未修后重跑或取得native性能；旧性能关闭不恢复。

## 关键实验与合同/运行/验收导航

以下统一基址为Dell绝对路径 `/home/xvhaocheng/work/iommu-platform/out/`。每项列出的文件是报告明确导航；除上方6个额外read记录，其余只定位而未在本单元直接读取out原件。父单元如核存在性/全文，应另记。

| 阶段 | 合同/进入 | 实际运行/原始证据 | 验收/分析 |
|---|---|---|---|
| stage113—114来源 | stage113-zero-infinity-admission-20261013/byte-and-resource-contract.json；stage114-c113-native-evidence-20261013/source/config-derived-byte-contract.json | 对本研究无新工作流运行，source/原作者PDF仅来源 | 各目录root-result-acceptance.json |
| stage117三个结果 | stage117-three-results-20261013/common/fixture.json | cpu/run.json；dma/results/run.json；dma/results/delivery.json（同目录） | root/critical-review.json；dma/results/artifact-verification.json |
| stage117四案G0 | stage117-plan-revision-g0-20261013/root/gate-acceptance.json；revised-gates.json | model/results/comparison.json，逐事件/snapshot由README导航 | root-result-acceptance.json |
| stage117协议适配 | stage117-packet-handoff-gate-20261013/adaptation-scope-card.json | stage117-packet-protocol-adapter-20261013/implementation/complete-execution-ledger.json（报告给完整运行导航） | root-result-acceptance.json（同protocol目录） |
| stage117真实cache | stage117-real-cache-handoff-20261013/next-goal-contract.json仅后继；实际进入报告章节 | implementation/costs-and-overlap.json；三运行原件见README | checker/delivery.json；root-result-acceptance.json |
| stage117六案 | stage117-delivery-g0-20261013/comparison-contract.json | implementation/results/{cpu,single,parallel}_{eager,batch}/run.json及CPU/cache/memory CSV；interface_probe/ | checker/summary.json；root-result-acceptance.json；root/goal-complete-receipt.json |
| PTE-safe前缀 | pte-safe-stage-causality-20261014/README.md（报告内冻结供给/服务合同） | 新4151row、12PTE表及失败构建由README定位，未在此猜CSV文件名 | root-result-acceptance.json；root/delivery-verification.json |
| AW被动 | aw-natural-entry-observation-20261014/README.md（原供给/反向还原合同） | model/prefix.csv；model/run-result.json（回执全文read） | root-result-acceptance.json |
| CO1 | completion-obligation-design-20261014/root/full-cost-comparison.json；next-goal-contract.json为后继 | 未性能运行；root设计、control/min-rule.md为分析 | root-result-acceptance.json |
| RCS1 | rcs1-completion-set-20261014/README.md（冻结条件导航） | model/results-table.csv（全文read，派生表） | root/event-verification.json；root-result-acceptance.json（全文read） |
| BR1最新 | blocking-replacement-20261014/root/projection-contract.md；root/mechanism-card.md | 未运行；witness/legal-chain.md为合同；root/fullbuffer-platform-correction.md为源码勘误 | root-result-acceptance.json、next-goal-contract.json（全文read） |

## 给旧私人wiki/会议稿的具体修订点

- 不应以“最新CO1”截止；补RCS32条件模型负结果和BR1入口停止及64B勘误。
- 不应把R107的24layer/12joint写为原生计算/collective执行；它是CPU字节代理。
- 不应把NVMe模板、CPUoffload日志、原论文规模及不同版本测试拼成一次关联原生运行。
- 不应说“平台只能串行/全cache flush必须全DMA停”；有限repair接口、一般tracked并发接口、WT fence.i三者不同。
- stage117有多轮来源转向、适配实现与修订后的G0，不是从AI失败直接跳CO1。早期功能满足不构成并发性能否定；后续四案/六案已测模型结果分别支持有限裁决。
- 六案普通即时发布确有消费重叠，不再沿前一真实cache关“未观察消费重叠”概括所有阶段；71周期局部差额不能全归翻译。
- R2应关闭本版完整逻辑成本扩展，不能沿旧“成本未核”继续描述；1329等组件数不是面积。
- F1/PTE/AW的源码条件、数学手推、一次被动RTL记录与真正候选收益实验分开。MIG accepted read不支持免费DS读重排；private模型能力不能当真实硬件。
- 当前待办是新的明确合法动作/完整资源寿命或有符号动态service差异，不是自动重跑旧图、补预热、扩大压力；首篇方法论文未完成。大学报备案题目本单元未代选。

未读边界：未阅读全文各out source/原作PDF、全部事件CSV、二进制构建输入、原生GPU/NVMe日志和现板身份；不声称独立复验安全/性能。没有读取私人会话、凭据或无关文件，没有远端写入。
