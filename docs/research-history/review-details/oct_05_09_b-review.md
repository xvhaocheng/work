# 10月5—9日 b 单元审阅（进行中）

证据读法：ledger 中 full_read 指该分配块实际完整进入模型并读完；不表示全文source已完整读完（a单元拥有交错块）。未运行实验或修改远端。每个裸 out 路径须冠 /home/xvhaocheng/work/iommu-platform/。当前未额外SSH核原始日志，以下为原工作记录所报结果及定位。

## 10月5日：先排工程功能故障，再谈组合性能

HANDOFF 1750—1758、3125—3257详细记录：首次新composition冻结172于第53项停止，52通过/1失败/119未运行；失联没有JSON/boot.after，不能凭boot changed None断言重启。USER4硬件快照显示COMP W阻塞、DDR AW阻塞、PTW空闲，只定位不是根因。真实DDR原子层模型N4096仍通过，不能说实板修复。随后两处历史IOMMU源码问题定位与修复：DS写源排列（63f790a，2025-04-02），PTW合法叶误置ERROR（cc0a4844，2025-01-20，S1 LVL1独立if/else错、S2合法LVL3拒绝）。DS-only N4096成功但172 PN第53条F15，仍52/1/119；合法4K旧错误13/15→新7场景全过，TL过。PTW修复BIT新boot后fresh172全过，不合并旧52项；报告 /home/xvhaocheng/work/iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json。功能通过不等于PN收益。

同日工作流工程：联网更新32MiB SD第一分区、JTAG重载闭环。method88代码归档4,796行，非删除原数据；原有83? 此处仅明确manifest而不编造组件数。综合172284→152609 LUT比较仅Synthesized同阶段；base WNS−0.380需一次post-route优化，不混placed报告。

## 10月6日：清理观测成本、局部PLRU因果与广泛立题

HANDOFF 1538—1711：历史USER4挂起诊断和composition path observer裁剪，V9兼容Linux先部署。模块OOC before14420/9199→after5561/3222，少8859 LUT/5977FF，不是整板差，未含USER4。clean新BIT/cpn3-clean同boot172全部通过、原日志回传，不能用旧ptwfix172代表新BIT。

B1四例为正常fill+单way mask合成驻留、actual RTL vs局部分区PLRU shadow：V touch/Afill victim8/2，touch0 2/2，同拍用旧tree2/2，改后继页收益反向；474次求值+25834默认回归。证明共享替换历史跨容量分区影响victim，尚非合法DMA可达或板上退化归因。

研究立题扩大：18席六方向R1/R2共36正式答复/26原候选，根没验收任何创新；123独立文献、104全文/105版本、19摘要，但根关键审核24不等于104精读。纯metadata replay、生命周期、双视图、动态供给不能因新命名成立；C1历史vs收益、C2缓冲DMA暂停/排空保留假设，ShareMMU锁范围源码取得失败。既有method88 p5强对照4组A增益均负，用户纠正“限速未做”错误后明确不重做。

## 10月7日：隔离修复、模型因果链与共享池尝试

composition-history-isolation分配块1—126、206—326、421—515、607—684、776—814已读（其他块在a）。生产分区localPLRU修改、22板测通过；旧A miss5117/6582→新2048/2048，双方W绝对低约21%，PN仍慢12.67%—12.79%（P约7.035M/PN7.932M）。cross BIT/boot n3描述性，不把miss数量相同当地址/时序/PTW成本相同。报告 /home/xvhaocheng/work/iommu-platform/docs/composition-history-isolation-20261007.md §1—9；验收 out/composition-isolated-20261007/board-acceptance.json。18同boot供给间隔短段板测：gap0 PN V+12.85%/A速度−11.39%；gap1000 +3.73%；gap4000 +0.08%无一致方向，不是修复。

局部TL52场景fixed A叶PTE返回时刻，A最多晚1拍、hot V快4—8拍，无漏重复，不能把局部拍数当板级上界。真实2iDMA＋behavior DDR＋synthetic CPU四模型P/PN91878/93480(+1.744%)；仅跨ID DS优先79847/79484(−0.455%)。PN V数据2763先于A PTE2766，PTE334拍vsP45拍；93 DS地址顺序同，服务差5505等PTWactive差。证明本模型可达服务反馈链，不定量解释板12.74%、非MIG部署新方法。报告同§8；out/pn-data-path-20261007/{comparison.json,trace-evidence.json,acceptance.json,p-full-events.csv,pn-full-events.csv}。

Codex逐PTE进展额度与DeepSeek最后级多拍FAST拒绝提案原误判已纠正：AR接受不意味着所有跨ID R不可抢占；p4按事件收费不是按进展补额。21短仿真同条件P91874/N151947/PN93487/逐PTE92362/普通在途92362/整walk91091；普通更强，停止B主方法推荐（非领域全否）。out/pte-progress-poc-20261007/{acceptance.json,comparison.json,codex-review.md}。

冷目的5/5：P92334/PN94427，DS优先80252/79911；一般热源冷目的先行成立，但FAST同job路径未证，不升级C。共享响应池13完整场景：8192B private→2096B payload少74.41%，PN112448更慢、FAST0；调整125677更慢。预约太早/每DMA一未消费AR压流水。完成阶段预约修后FAST恢复但普通无POOL W-prefix48B P92109/PN92374最强，停止共享池贡献；payload排除公共TC/metadata，无面积。最终21矩阵=13旧+修后8，实际29运行，旧FAST标签覆盖错误纠正分类。out/e2-interface-poc-20261007/{acceptance.json,initial-runs.json,reserved-runs.json}，out/completion-reserve-late-20261007/{acceptance.json,runs.json,runs-all-executions.json}。

同bank合法AR延迟64小例：V0/V1挡A0、TC两个AR spill满、PTW闲、WheadA，真实RTLwatchdog；不是板bug或饱和。48B普通准确源准备门也解除，8192B完整FIFO成功。请求头park两槽＋直通127092 vs普通shared3 127147，只一头差55拍不报性能；正常PN同92374。普通选择队列+旁路表达等效，停止创新；(K−1)L未证。out/write-order-boundary-20261007/{runs.json,stage-evidence.json,codex-review.md}；out/header-park-poc-20261007/{runs.json,stage-evidence.json,README.md}。

## 10月8日：自然错位反例与普通准备逐步追平

共享DDR读延迟overlap干预P92109/PN92374→85339/85984，仍慢265/645；fast→source capture2/3拍，关键受AW/W关系；并非全部PN差额因果。4DMA这里只BFS九策略×36=324抽象组合，盲共享H2—4 30/36停滞，普通每bank2槽或当前W-bank预约2槽0/36；非实4DMA性能/定理。out/read-service-boundary-20261008/{runs.json,trace-analysis.json}、out/four-dma-boundary-20261008/{results.json,traces.json,codex-review.md}。

natural32KiB/src0/dst3072真实AW错位使一个2KiB R跨A前后V W段，V尾R堵A PTE；生产C0写容量错误仅out修正、未应用production。普通准确源门CONF0/BUF3两启动全通过vs512，不把解除环作创新。postTL固定1KiB vs动态最长前缀6PASS，动态physical AR64→55但31延迟末W晚553，100延迟整体早68、均值晚31，未稳定独有收益；固定同效解除，关闭动态主贡献。固定父槽1/2/4 vs8 CSV逐字同，减少2394字段bit但非面积。前/后置位置8PASS真实issue→DONE均值25799→18810低27.09%，但TLmiss34/PTW102同；可见容量不等。普通父复用/当前父early只改善0.33%，post首R前已接受六原父，不能据此否定普通lookahead。普通已准备source PA旁路6PASS后V/A均值18975 vspost18824仅0.7958%，原23.95%差缩97.45%；无完整成本、不能说所有151拍原因已归因。合同/运行/验收入口分别out/write-owner-baseline-20261008、posttl-independent-20261008、parent-capacity-20261008、fragment-placement-20261008、parent-translation-reuse-20261008、prepared-source-bypass-20261008的README/stage-evidence/codex-review（旁路为codex-raw-audit.json/analysis.json）。

nested continuation只有分析：15PTE冷nested、77条件数据下界/82普通帧上界不是面积全编码最优。固定链一次数学6450配置/mismatch0，可完成全30read；q≥可保存相邻最大gap iff存在策略，普通checkpoint达下界，停止此独立贡献。7585/3805/11390片几何分类也是数学，不DUT。remaining-use共享cache模型36records E8 gap4 LRU30→ordinary24，但same-info interval/refcount同规则，停止独立原则，非copy速度。

## 10月9日：共享读原型与不断加强普通对照

stage49事件模型初run01/02失败598/143，最终314场景7644checks接受，N16候选/强tile同R28/AW6/214steps，仅AR2/4；非RTLcycles，CONF129 cache属性前提要同对照。

stage51真实adapter两build15run14PASS一quiet失败；普通q256与longread四对CSV同，关闭长读34%贡献。stage58普通WRITE lookahead46PASS，多数F63以上比接力好；stage60/61普通resident准入追平peer8.7%。stage65—69合计156run148PASS/6旧真实死锁/2要求拒绝；Max2 B流水线解释约19%改善；并行补读E160少18.75%读但V慢17.2%，压力慢25.9%，不能拿省流量当性能创新。stage75短补读29run27PASS/2旧route拒绝，严格加入窗口0不否定更宽合同。实际latest stage76—78六build29copyPASS（由相关HANDOFF引用，此b单元尚未直接read对应a块），五强普通许可齐起点共享更快，true late join功能成立仍无创新。

stage79—92主要手推/原文/接口筛选，无新模型/RTL/Linux/build/board。等待链优先、deadline next-owner、closed density grouping、stage形状shared tags、leaf BBM、按许可coalition、longread最大safe-frontier均被具体ordinary覆盖或缺完整可达性/成本。共享行172bit、319/479bit、1228? 1228是10月14别处，不在本单元。L-M overlapping {X,Y}/{Y}最大匹配容量秩，但ordinary合法early-bind+pin复现；L-W集合叶选择3PTE被checkpoint取消+partial cache复现。stage92 bounded judgment后暂停（不全领域否定）；out/higher-model-mechanism-20261009/round05/root-result-acceptance.json；docs/mechanism-design-handoff-20261009-round05.md（待专题块全文）。

## 对旧会议草稿必须纠正

不能还写“只有平台、没有机制证据”；但也不能把clean172、PLRU隔离、功能PoC当论文创新。必须记录10月5两历史功能bug、10月7PLRU工程隔离与剩余退化；模型1.744%与板12.74%分开、DS优先为模型干预。必须加入失败/关闭链：p5 method88、逐PTEB、共享pool、park、动态fragment、固定链checkpoint、remaining-use、共享读省流量与earlybind普通覆盖；不能用不同时期结果合并共同分母。7—9月草稿遗漏这整个强基线与否定演进，单CO1结论也不足概括10月。

## 最终覆盖与后续专题补充

全部分配186块全文实际读完，共401128字符，39个source（ledger实际计算）；没有截断块、未读块0。source全文是否读完必须与a单元联合清单判断，不能以本单元块读完宣布整个对应source读完。HANDOFF63块，MAC1，专题/status61，validation61。validation与status历史重复记录仍逐块读，未据摘要跳过；后续否定/修订保留。

专题原件新增清楚边界：
- innovation-focus-20261006.md顶部明确后来innovation-followup已取得之前503的ShareMMU必要源码，artifact替换计分与论文伪码不同、原读环可由普通返回空间预留解除。因此本报告此前“锁范围源码未取得”只适用于当时阶段，不是最新总判断；请与a对应followup合并。
- research-progress-presentation-ja-20261007.md分配1—283与zh286—356已读。旧老师稿第7页使用10月5旧composition11.58%—12.22%；第10页仍计划隔离PLRU。用于10月19材料必须改为后来isolated22/22、额外miss差消失/剩12.67—12.79%、18gap小板测和2DMA模型反馈。不能继续说“隔离未实施”。
- §37原请求窗口公平对照后27.09%缩至6.706%；窗口2/4配depth1存在6watchdog，普通加slots解除，窗口8原轨迹1slot同效不是通用保证。§38普通future-parent缓存hit直通仍进共享TC，只有§39物理旁路真绕忙TC，不能混称。
- §70—74是170run（152完整PASS、8旧watchdog、10实际cutover拒绝），非整夜总数。§72独立A suffix错误依赖已退休V授权导致8死锁修复；原型故障不论文贡献。§76—78 RR指针旧idle前进藏窗口，修按真实R握手进，5truejoin；强普通startup五组更快，17%对弱法不能独有宣称。
- §80Python120完整调度比较，每候选完整ACK曲线至少被一个具体普通control复现，但这不是一套在线万能selector；§82Python18模型事件仅h0静态已准备，不证明IOMMU因果；§84Python6图190state，Ui>Qi一故意死锁不DUT，Ui≤Qi为普通安全容量。
- status-stage93-20261010.md正文标题/日期为10月9stage93，10月10附补充。指出问题由原远端翻译自身成本扩为多DMA干扰合理；后期贡献目标没固定、合同不断扩，不能把每个新toy反例当自然应用。建议有期限固定贡献、2—3可执行对照和正反例而未启动；不是自动选新方法。
- validation.md 1341—1346仍写第一次paper-council“优先接口”，同一paper-council-decision.md已因用户异议修订“性能继续、接口备选”。属未同步的历史摘要；采用最新决定但保留原分歧，不写全体原本一致。

## 本轮直接SSH证据核对（无重跑）

已实际SSH确认并全文读取两份board acceptance：
1. `/home/xvhaocheng/work/iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json`：172 planned/attempted/completed、failed/unrun0、boot2695...、sameboot PN诊断true、source DS/PTW matchtrue、IO/CDC false、performance claimfalse。**dmesg_prefix_preserved=false**，虽new_fault_or_kernel_alerts为空，不能报告前缀保存检查已通过；这是验收原JSON中的边界。
2. `/home/xvhaocheng/work/iommu-platform/out/composition-isolated-20261007/board-acceptance.json`：22/0failed、bootd61...、cross_BIT_boot=true、WNS0.022/WHS0.051、kernel新警告空、end_starts_with_before=true。完整JSON的18 crossBIT百分比已读。

另实际确认存在并抽读顶层关键数值（不是全文JSON或原CSV）：
- `/home/xvhaocheng/work/iommu-platform/out/prepared-source-bypass-20261008/codex-raw-audit.json`：1build/6run/6PASS、均18975/18824、151残差、97.45233676%、cache镜像false/A先19008、未综合。
- `/home/xvhaocheng/work/iommu-platform/out/original-window-20261008/stage-evidence.json`：4build/16run/10PASS/6watchdog、all_dut_runs_completed=false，capacity14/timing2。
- `/home/xvhaocheng/work/iommu-platform/out/shared-read-inflight-join-20261009/execution-ledger.json`：stage78、boardfalse、14运行exit0、5truejoin run01/04/06/10/11；disabled与latepermission CSV字节同。其run/build完整条目未展开。

原始record/log/CSV此次仅沿引用定位，未逐项核验所有原始数值或source/binary hash。此前文档的“raw审计通过”是历史root记录，不是本轮重验；无实验、构建、仿真或板动作。
