# 10月5—9日机制单元A：最终全文审阅简报

审阅2026-10-14本地只读快照；远端源均在`/home/xvhaocheng/work/`。**本单元182/182分配块全部真实完整展示阅读，57批全部完成，未读0、截断0；涉及42个来源文件，其中本单元HANDOFF分配59/59块全部读完。** 不宣称读遍所有源文件：部分长文按分配切块，另一B单元负责互补范围；完整逐块路径和起止行见`ledger-oct_05_09_a.json`。

本轮没有直接打开原始out回执/CSV/LOG或实验源码，以下实测值来自已读当时HANDOFF、专题报告和validation记载，并非重新验证或原始证据复算。70/87/107/120等数字及“剩余/下一批”仅保留在文末分批过程记录，均是历史，不是最终覆盖状态。本单元没有远端文件修改、实验、构建、仿真或板测；统一Dell归档由根任务负责。

## 10月5日：P/N/PN工程实现与路线纠偏

P为固定2/14缓存份额，N为PTW忙时另一bank合格hit旁路，四模式同一IOMMU/同一BIT运行时切换；四模式不代表四个IOMMU。既有512拍DMA0/1缓冲非本轮新加，四DMA中只有前两路有完整copy保证/大缓冲。

composition-v1恢复构建最后31 node overlaps、WNS−3.268ns，拒收，未上板；不能把route_design命令成功或DCP存在当合法布线。原critical path为IOTLB tags_q到旧research observer event_ram。实验观测持续叠加，四同级观测/组合控制综合LUT33390、FF25217，不等于全部可删或方法硬件开销。composition profile裁剪旧research事件记录与旧N诊断，保留通用/path/P4M及双快照、128字段、方法。10联合scenario及4入口检查过（非实际AXI/SoC），新base工具exit0但WNS−0.380拒收；单次route-opt 03:06 JST WNS+0.058/WHS+0.052/WPWS+0.062、273893/273893布通、routing errors0，accepted BIT。最终routed LUT163673、Slice99.45%，全IO/CDC仍未签核。根HANDOFF3029—3102；专题`iommu-platform/docs/composition-handoff-20261005.md`、`composition-routing-cost-20261005.md`、`composition-trim-20261005.md`已全文读。

网络工程从Mac百兆0bda:8152/en11、旧驱动只支持1000，改Linux驱动100/1000并增量net2/net3。net3真实SD写入/同步/JTAG/开机联网/SSH身份通过，科学实验0；BIT和Linux发布不可混版本。`/home/xvhaocheng/work/iommu-platform/out/ethernet-workflow-20261005/ethernet-workflow-20261005-03/result.json`。

冻结172最初52通过/1失败/119未运行，N板端卡死；模型N4096/持续A、并发warm和CPU写AMO皆过，缺MIG/板级转换，未复现。只读USER4快照v1/v2对CPU/COMP/DS/DDR/Wowner做观测。v2正常三帧后N4096再现；卡死帧5/6/7显示Wowner FIFO count4/headDS、只DS ready但DS WVALID0，外部DDR/atomic可接受，队头等DS数据。不能仅从地址断言FQ。Codex源核：AWID CQ0/FQ1/MRIF2/MSI3/PQ4，而原W数组PQ0/CQ1/FQ2/MRIF3/MSI4错位且重复case3，63f790a接PQ引入。唯一DS RTL纠正数组及空满保护。旧源码scenario1 AW1/W0/B0 watchdog；新四场景均通过（FQ4拍、并发9burst21拍、提前CQ4拍、满队列12单拍）。该工程bug和后来的PN性能退化是不同问题。证据目录`/home/xvhaocheng/work/iommu-platform/out/composition-dsfix-20261005/targeted-test-acceptance.json`、`test-before-final.log`、`test-after-final.log`；v2原始现场`out/composition-hangdiag2-20261005/mac-records/diagnostic-board.tar`。根HANDOFF3116—3206已读；修复BIT后续正式172完成由B单元后续块合并。

六席五轮30答复只交研究计划，初接口首选随后用户质疑转向，根10/5纠正继续性能主线，接口降备选。模型投票非创新证据。开放搜索零散动态映射后又按用户请求先研究地图（36独立论文38全文版本/4官方文档），候选暂存，不自动转安全或实现。根HANDOFF1712—1749。不能让早期“接口优先”“动态零散内存优先”覆盖后来的纠正。

## 10月6日：文献、首篇缩题与预审负结果

专题共享翻译72独立工作、59全文/60版本、13题录摘要，Codex核12关键原文、DeepSeek整理卡；全文取得≠通读全部或复现实验。补VIO/BaM/GPUrdma/GPUDirect，GPUDirect1:1直通限制意味着NIC-GPU存在不能证明共享IOMMU翻译竞争。六瓶颈地图是综合，不是领域空白证明。

用户博一不要求顶刊，首篇A页工作集感知提交、B配置边界选择一度优先，但最新预审两者均未放行实现；必须复用已有P4M/method88，不能“从零建立限速基线”或挑有利条件救p5。新A要区别字节、请求、miss、加权事件、实际占用、V反馈；gap为完成后闭环等待，不是在途深度，512拍buffer不等descriptor队列。首篇文档119行已读，后续是否有续篇由B单元合并。`first-paper-options-20261006.md`顶部最新修订与下方历史优先级不能混用。

C1共享/存储已有ShareMMU等前史；实际ShareMMU artifact替换max age−5shareCounter而非论文有限LRU公式，接口至少一端scratchpad非双外存。普通逐读预约O+R≤B能在服务/排空条件下解除原C2返回环；AW-before-AR已有。新U→B→W→M→U环只是拟议条件、iDMA真实可达和合法更新范围未核，不存在假定global revoke lock。C2宽泛依赖准入未具体增量，不入RTL。A1局部性消费者等待暂缓，B1 PN组合边界仅低成本归因候选。根HANDOFF1593—1703。

clean BIT完整构建base−0.131ns拒收，单次post-route+0.169ns/+0.038ns、三失败0，LUT156215。首次更新旧内核wc在shmem读取软锁死，未调用新BIT JTAG，不能归罪新BIT，根因仍开放；恢复旧ptwfix后第二次34m34s更新成功，新cpn3-clean boot身份，JTAG本轮2/SD1/实验0，新BIT不能继承旧172。根HANDOFF1548—1559。

## 10月7日：历史隔离、板端供给干预和强普通准入

PLRU fixed2/14按resident way更新所属树，S/N fullmask不变；生产IOTLB/TL两文件改，原25834检查、新9232和TL回归过（合成初态不等DMA可达）。旧clean PN退化、A miss5117→6582只是线索；isolated结果由B单元补。

PN算法审计没有确认解释12.74%板退化的bug，不能说无bug或必然架构效应。Claude提出跨bank AW老owner例与demux175—177禁止条件冲突撤回；DeepSeek误把编译NonblockEnable作P/PN差异撤回。单TL52相位均过不等完整DMA/DDR。旧窗口AW反压+863488与总W+896481.67重叠定位不能相加归因。

同isolated BIT/boot18项gap64板测03:52:54—03:55:34UTC，A8/P2/PN3/gap0/1000/4000/block1—3、warm256、V64，18/18/no taint。gap0 P/PN W109839.3/123956.7（V+12.85%、A−11.39%）；gap1000 +3.73%/−3.85%；gap4000 +0.08%/+0.03%，FAST降5—8且A完成315—316，不能说N同强度修复退化。gap增V绝对W也增，不能扣gap冒充独立服务延迟。硬件无PTW独立计数，支持供给敏感性，未证MIG FIFO或全板因果。合同`out/pn-board-path-20261007/reviewed-plan.json`；run`mac-records/pn-gap64-20261007-formal01/`；验收`board-acceptance.json`、`reduction.json`。

逐PTE B额度PoC相对PN−1.203%，仍慢P0.531%，普通周期/wholewalk更好，普通在途同效，DS优先B只再省5周期；这些专题fragment327—420已读，同条件默认S/N/P/PN105588/152042/91878/93480补N数据，未实板。C源热目的冷5短仿真一般源先行成立但非同job FAST严格预测。PN中A FAST提前DDR使V根PTE服务32→577，优先返回干预回32，只是行为DDR队列干预，非可实施MIG仲裁/旧板12.7%全差。C未实现，不能说C测差。

普通owner8/8屏蔽FAST虽少退化但新增本bank源重walk；ordinary wholewalk256保FAST64、冷PN91727<P92334（0.657%），所以旧C首选停止。此是准入位置改变touch/PLRU的顺序效应，非跨bank共享历史旧bug。合同/运行/验收在`out/owner-baseline-20261007/codex-test-spec.md`、`runs.json`、`acceptance.json`；本单元未打开raw。

完成预约Mode4单槽/5普通两槽/6精确下一W流式/7无POOL，无池Mode7热P/PN92109/92374、冷92549/92814覆盖共享池主价值。代理fast_pending被后续翻译覆盖，新每bank8 READtag修；29执行只有21最终矩阵＝原13+修标签8，旧计数unreliable，周期/bytes仍可保留。48B仅payload，不含TC/FIFO元数据或面积。`out/completion-reserve-late-20261007/{acceptance.json,runs.json,runs-all-executions.json,comparison.json,codex-review.md}`。

同bank无池48B真实2DMA warm卡住，TC/AR头A，外部AR头V，WheadA无数据，sharedR/PTW空；三watchdog非生产板bug。双bank48B、同bank8192B、普通源prepared写门48B皆过但A完成量不同不能净排名。候选完成AR头暂存+bypass尚未实现、(K−1)L非定理。`out/write-order-boundary-20261007/{stage-evidence.json,codex-review.md}`。

payload-frontier复杂扫描与普通准确virtual-W/preparedAW两对CSV逐字同，PN约6.25%改善不是独立规则，仍慢P0.6009%。virtual WLAST不等DDR/B完成。tests副本有8PASS，生产hash同；scan降普通工程。`out/payload-frontier-poc-20261007/{runs.json,stage-evidence.json,codex-review.md}`。

## 10月8日：从写依赖到译后分片，收益被强普通逐步消解

读服务串行/接受后重叠4仿真均过，P/PN92109/92374→85339/85984，PN差仍在；计数empty/setup/beat/backpressure互斥账本不等模块因果。四源36读交错九策略324抽象分析34680累计states非四DMA RTL；普通W-bank预留2头已无停滞。

CONF0源目的共同边界，旧PN合同不能用CONF1源2048/目的1024+1024错位解释。RAWCouplingAvail编译参数与conf不同。新同bank自然CONF1长任务发现C0配置4项/计数3bit第8AW回绕关闭W，out修capacity门但production未修；修后R尾/Wowner/PTW排队环仍停，ordinary8192B buffer解。13run6PASS7watchdog，不丢失败，非旧PN根因。`out/natural-write-order-20261008/{runs.json,stage-evidence.json,README.md}`。

译后parent/owner固定max128 vs动态最长prepared W前缀：6PASS，动态物理AR64→55但延迟31反而末W+553，延迟100平均仍晚31，不主推动态。FIFO48B/5962声明位未综合，非四DMA/MIG。前/后置8PASS约27.09%差伴入口/metadata不同，N四项FAST0因为同bank不触发，撤回有效N对照。原父翻译复用随后降低TLlookup但并非PTE减，不能把全部速差归32hit。

原窗口配平16run10PASS6watchdog，单父window2/4卡、ordinary多父槽解；同window1后置残差6.706%，不是加槽新机制。普通multi-parent准备18PASS最强均24751 vs post18824（23.95%），但仍源片进繁忙TC。真ordinary prepared物理AR bypass6PASS均18975，原gap5927→151，97.45%被消解，剩0.7958%，故不能以位置/旧23.95%创新。声明4598 vs4602非等总面积；静态6PASS不验证失效。专题历史685—775已读；证据`out/{original-window-20261008,multi-parent-preparation-20261008,prepared-source-bypass-20261008}/`各stage-evidence和root audit。

LTI-style译址寿命反馈当前平台无普通译址结果等DMA完成释放并阻IOFENCE路径，PR/PW TODO、Linux普通PR/PW=0/InclATS0，CQH早推进不等generic completion；不能为创新添加不存在锁。fault+合法discard为强进度控制，成功copy为额外性能合同，当前fault排空未验。

有限排空§42 1build4PASS，普通到达字节下界：V先至少15360R/15368W，A先7168各，7new child无new AW。4115/5436周期不同起点不可比较因果。§43双bank34/34虚offer到物理AW全序一致、max(vr−pr)=1；headlag旧payload退休而非AW倒置，停止冲突分支。只静态CONF1/DEFAULT单job，不是生产一般AW定理或硬实时界。`out/committed-write-drain-20261008/{stage-evidence.json,codex-result-acceptance.md}`；`out/dual-write-prefix-20261008/{order-evidence.json,codex-result-acceptance.md,stage-evidence.json}`。

shared translation可达性手例同PSCID/root共享上级而叶不同，旧§43实际PSCID10/11/Bare不能称nested/shared实测。N只是hit不是第二miss coalescer，IOTLB/PTW独立无统一槽争用。requester history专利已有，alias纸面victim变化不等周期/PN归因。remaining-use有限cache9组36普通records，ordinary E8/gap4达24 compulsory，停止纯remaining算法命名；noPWC miss×3为Sv39数学模型非DUT。

## 10月9日：共享读真实完成→普通覆盖→新合同设计

§50独立src READ/dst WRITE recipe接真实iDMA/TL/sharedR+qW，27run最后12接受、早7PASS、6失败、2参数不适用不当lat0。lat31模型V/A5657/5650 vs q2 8579/8572约34.1%主要少AR请求模型代价，非MIG/PTW独立干预；pause V只低1—4%。single-reset一次pause/resume、静态许可不能推广连续job/全部暂停；raw留`out/shared-read-rtl-20261009/{execution-ledger.json,codex-result-acceptance.md,README.md}`。

后继§52/53早恢复、lookahead普通同轨迹，§57 28PASS接力V慢A快、PTE1000更差，§59 27run26PASS+F255末拍停滞后private末拍修，只特定fullWR/q2，logical RLAST不能代W/B。§62三Q37PASS与§63ordinary end≤peerW+Q15PASS（11CSV同）覆盖大Q“peer算法”。§70—74共170执行152PASS/8旧watchdog/10strict reject：§70 bandwidth模型整体19—39%但V部分慢，multicast解释；§72 A suffix错依赖已完成V READ修ownA；§73 independent V代价长budget慢53.4%；§74早A AR只12—14cycles~0.13%，普通pipeline。各编号目录见HANDOFF1007—1079与3297—3299，全为out RTL模型，非板测。

§76—78共29完整copyPASS（6成功build），修模型RR从idle探测转实际R握手提交，旧interval64零窗/公平服务结论收窄。真实在途join5次E64/63/256、R257 vs弱320，约17.16%优势被强ordinary许可已齐起点共享R256、V19402/A19411比join19462/19465更快。晚许可无join两CSV同。连续epoch/fault/remap/held物理R绑定未测。`out/shared-read-inflight-join-20261009/{codex-result-acceptance.md,execution-ledger.json,codex-next-novelty-gate.md}`。

§80三创新版本被完整普通覆盖：shared-ring同信息/端口/授权/容量O-ring同，Whead guard复现B且旧DUT该例不可达，C120比较每scene/profile具体ordinary同ACK/walk/makespan（不是一个全域在线万能selector）。12 tests/120Python比较非RTL/FPGA，DARTS核源码不是StarPU实跑，LRU/globalremaining脚本修正保留。`out/parallel-innovation-model-20261009/{root-result-acceptance.json,c-comparison-verdict.json,accepted-source-manifest.json}`。

§82结构成本A complete-subblock/PTE two-slot已有，166b普通vs168observer、nested172，停止独有存储贡献不是综合逻辑等价；B18有限Python模型全部事先oracle一致，candidate常态少一AW，但conversion ordinary A同/B早2且无null少流量，成本/前作未闭不送RTL。monotone suffix后继未实现。`out/innovation-recourse-cost-20261009/{root-result-acceptance.json,execution-ledger.json,all-traces.json,a-full-structure-cost.json}`。

§84 N−Ci≤Qi允许所有consumer仅部分destWR整包unsplit-read条件规则，Python6图190states、Ui>Qi唯一deadlock，非sharedRTL；coupler当前关闭，新启firstR只一AW credit不能默认两AW。§86强two-child普通27/33胜单child H3 29/33，物理prefix history同读次数，所选性能点关闭；统一载荷池6槽vs9仅省192bit payload，端口metadata可能抵消非面积。`out/innovation-permission-frontier-20261009/`、`innovation-native-cost-20261009/`根accept/design。

round01 A条件主线/C备选/B普通walker覆盖；A与同合同cursor均10/14/24，correct whole-source8/14/26或8/15/25，无独有系统收益；旧CPU overlap8/14/25非法作废。round03 L-F预留输出、真实正常源READ再按resident/free绑定目的；F1 ownership-change无合法正例不进收益，无模型/RTL/Linux/build。O98与3355bit vs3587bit只差29B字段非净面积。10/9§93从原毕业论文路线审查：原问题合理，测量/供给修正必要；后期贡献不稳而合同持续扩展需纠偏，暂停具体候选，普通复现单例不等全路线无价；等价实现不自动发表前作覆盖。`docs/mechanism-design-handoff-20261009-round03.md`、`out/higher-model-mechanism-20261009/round03/root-result-acceptance.json`；`docs/research-route-audit-20261009.md`和`out/route-audit-20261009/root-route-verdict.json`仅通过本单元HANDOFF引用，专题未读。

## 给私人wiki/会议稿的明确纠正

1. 不能只讲PLRU/PN和CO1两端：10/5真实DS写归属bug修复、诊断现场、观测资源与route-opt、Linux网络更新是独立工程链；方法成本和仪器成本须分开。
2. 10/5原17252/1/119与后来clean/isolated验收区别版本、boot。同一个“172/172”不能迁移新BIT。
3. 10/7板gap收窄退化不是加速或N同强度修好；无板端PTW计数，模型DS优先因果不能改写成MIG根因。
4. C没有实作/测差，普通wholewalk已覆盖单case目的准备效果。shared pool约74%理论payload、后置27%/23.95%、sharedread34%/join17%均须列强普通覆盖或模型请求成本边界，不能当论文贡献。
5. 两真实DMA/四源抽象分析与四DMA生产、RTL行为DDR与实板区别；CONF1错位新合同不解释旧CONF0 PN。
6. 10/6A/B首篇建议与10/9新合同设计都是历史条件候选，后续否定修订优先；不能给“已有方法论文/明确创新”。
7. 当前研究可保留平台、干扰问题、因果局部证据，负结果收窄具体主张而非领域无价值。成功copy与安全fault/discard不是同合同；可达性和完整费用缺口不能靠额外弱化普通对照补。

## 分批过程记录：前期尚未读范围（已在后续全部完成）

本单元第20—56批共112块未展示读取，包含first-paper后续/高阶机制专题、innovation深审、round02—04、literature/full native design、status85—91与validation重复记录等，全部ledger full_read=false。其他阅读单元负责互补片段，本文件不代称全文。所有原始CSV/LOG、源码、完整论文及out回执本单元未抽核，不算原始证据已读；仅已读报告给出可追踪绝对目录。没有运行任何实验、构建、板测或改远端文件。

## 续读补充（第20—34批，2026-10-14）

累计真实阅读全文87/182分配块，next_batch=35；上方70/182及剩112是前一轮历史快照，以ledger/continuation最新数字为准。新增17块无截断。包括高阶机制round01两片、innovation-deeper/followup全文、round02一片/round03一片/round04全文、natural-write全文、novelty-gate前136行、paper-driven全文、parent reuse全文、platform feasibility全文、posttl verdict全文、landscape两片、permission release全文、日语讲稿尾部。其余专题仍未读完，未新增raw抽核。

新增关键纠正与精细证据：

- 10/9 round01 A源回收区间补集：未来F=[I,end)、公开读义务U，首次publicVALID前从F移U，只有完整成功真实R才删U；回收必须无未来/未退休读、无未知别名并有真实CPU/cache所有权。原18 obligation字段2985bit≈374B是字段子集而非净面积，软件可能O(P)，不能宣称全系统O(F+K)。若trusted descriptor已保证不重读，普通真实源进度可能足够，不能新增威胁模型制造性能贡献。证书FIFO满只丢通知/sticky overflow不丢权限账、不阻R，软件未确认页不得复用；page query冻结ARM/PUBLISH但已公开AXI继续。错误冻结证书合法排空，NO_ERROR_HANDLING未验恢复。
- 同文C静态bridge-last7→6 miss/260→240被普通目的分组精确覆盖，e4早但e1—3晚44，非Pareto；K2不是整机2entry。动态split/merge+costed residency无独有正例；九组M1—M9未来计划全部未执行。普通同合同cursor复现A24不自动宣布已发表前作覆盖整个新合同，但不能再声称独有调度收益。
- 10/6 innovation-deeper明确actualRAWcoupler关闭允许物理AW先源翻译，CONF0共同推进不排除下游先后；只属于源码可允许前缀非仿真/死锁。ordinary O+R≤B加AW前全部所需源读已发/已有数据可在公平服务、U只等物理请求等限定条件排除环；不必严格同时先取得两端翻译，Ts+max(Td,Lr)+Lw为普通重叠手算非实测。更新等整个descriptor、fault、额外反压环不适用。若完整差异只靠不存在global lock或不给对照流水，停止C2。
- 10/9 round02 R generic relation dictionary精确复现编码不意味着整体完整成本一致；CoLT4.1—4.2定向原文已核而HPCA2014全文未得。T native一拍续步无长空窗，不为不存在gap建模型。所有closed-form三概率case非workload仿真。
- round03 F1合法原始warm/ownership轨迹必须A真COPY完成→consumer/cache sync→Z真实release在V源READ阶段；source自驱逐X/Y小例被精确victim/pin覆盖，非独有点。F1/F5/F6先行，F2—4按差异，6组首批S1—6只是计划。与ordinary source-prepare+晚allocate比较不准迫使其重复walk。完整索引和新合同复制到ordinary allocator是候选等价实现，非已发表覆盖证据；独立已有准备/pin/布局/容量对照完整复现收益才关闭该收益。
- round04 L-M匹配容量r(H)与Hall是标准数学，{X,Y}/{Y}驱X使两个local h≥1但rank2→1，小例证明scalar不足而非独有速度。guard319bit/若新resident160则479，不能319+479再相加；都是字段子集。BIND需全部合法FREE图可完成后偏热，绑定未译WRITE义务到响应进稳定物理头才退，不能FREE→COPY就清。no-fill必须完整普通RW/A-D/stage/fault检查不免费许可。L-W公共前缀晚叶选择30+δ vs40手算被cancel+PWC匹配PTE次数，取消仅组件；0模型/RTL/Linux/build/board。最新stage91候选并不构成已通过后来的stage92/93路线暂停。
- parent reuse10新run中首4plusargs开关未生效排除受控比较，只6有效新对照。关闭pre CSV与旧同；复用/提前25712/25714 vs25799仅0.337%/0.329%，PTW102未减；后置在首R前已接6raw父，而pre每流只1，physical父槽各1不等同可见范围。字段3514/3518/3568仅声明非等总面积。背景失效输入0、fault可见性生产未处理。
- platform feasibility10/6仍写P4M60未发现后35续跑，此为当日边界；不能用它覆盖后来的实际formal60完整状态。ptwfix172是同boot三block不是三启动，A长batch2^32在V后STOP，不能当固定4096A完整job；PN/N A8 gap4000 V−3.21%～−2.93%同时A rate−31.09%～−30.90%，不能只报V好。quiet/clear是实验接口非规范必须全局静默。
- permission release同池6 vs独立9字收益来自重复基线；candidate和ordinary均6，3×64bit仅192bit字段。弹性前缀11reads vs13、coalition3数据 vs B/D2均手推，被samepool/阶段完成优先或occupancy+partner continuation覆盖；授权本身不释放数据、必须真实W退休，现接口2许可生命周期，四prepare须队列费用。
- 日语10/7既有汇报可核出处尾284—353：旧84=80成功/2失败/2未执行，cost11同boot补充不是续84；E2 64项中的128为页延次数非独立128试验；method88两启动第一复用/n3非显著性；clean172只功能，旧10/5P/PN非clean性能；N v5 S/N各15，A8约+4.2%、A16约−2.7%非普适。这个稿具体遗漏后续isolated板gap、共享池/分片/旁路/shared-read普通覆盖、stage91/92—93停止链，应更新10/19材料。

尚余95块（第35—56批）；所有远端source保持只读，未写Dell docs/research-history，统一落库由根负责。

## 续读补充2（第35—42批）

累计107/182真实全文块，next43，余75。新增中文讲稿1—285（旧计划不能当10/14现状）、shared-read独立调度35行全文、deepening47—114、literature1—94、status through84分配20块。新增报告/历史重复以ledger为准，未抽核out raw。

重点新增：
- §49 event模型314场景/7644check非RTL，run01 598/run02 143fail来自Flow.resumed遗漏等worker实现、保留原稿。B3二FIFO48B+skid8B=56payload非完整面积；model B在WLAST同事件退无delayedB。N16/q2/P3/resume200候选与ordinary tile都214eventsteps，candidate少AR非实测速；independent35steps不满足同指定pause合同不能弱比较。有限tail≤2q需一个已first-offer Wgroup、B≥q、有限公平服务，非固定walltime。
- §51 two-pause RTL15新执行14PASS/run09旧第二quiet失败后修；ordinary mode2/q256与longmode1四对CSV SHA完全同（nopause5657/5650、once5161/19848、twice5161/29793、twice+B64背压13277/28956），同cost同效直接否定以§50 q2弱基线34%为新算法。义务保持即pause不能挡已firstVALID短W需要R，singletonAR要整段Wcoverage。
- §64 ordinary ordered-prefix15PASS末metrics同63，仅5CSV AW早非性能；§65—69合156真实RTL执行148PASS6旧死锁2actual拒绝。Max2 issued-group相等改≤，普通two-B pipeline解释~19%压力改善；minimum-J约1—4%未确立创新。parallel E160 sharedR416vsindependent512少18.75%，V4550vs3883慢17.2%；压力V9824vs7800慢25.9%、A9833同，不能少流量=更快。
- §75 short-prefix29exec27PASS+2route-not-bound拒绝，E=N修独立R512；18short成功strictJ<E窗口0只否定该窄门，不能否定后宽join。源1functionalFF/134observer/0payload非面积。
- §79 C closure4小例ACK31.25→29.75但ordinary hotpage追平，15desc到t28 candidate3ACKvsordinary1仅早期纸面差异，被§80后来完整ordinary覆盖。B兑现被same-slot nextWowner lookahead复现。§81 A cached evidence candidate10reads被PATH-MRU10/leaf+bypass9覆盖；B nulltail104/7被原起点ordinary adaptiveprefix104/6少null，next只cost/前作；C BBMpartialcache复现19而39弱rootrestart。
- §7/8/9 timeline新补具体完整仿真因果：two真实iDMA/DS50-50 A8 V64，FIFO P91878/PN93480（+1.744%），DS返回优先P79847/PN79484（−.455%），93DS地址序同，额外等5505=PTWactive增，VFAST数据先Aleaf；只模型局部因果，不全解释板12.74。
- isolated22/22真实板：最终WNS+.022/WHS+.051/LUT156186，一次JTAG无Linux/SD更新，bootd61e...；A8/gap0P/PN A miss均2048（旧5117/6582），绝对P时间−21.44~21.82%、PN−20.69~20.74%，新PN仍+12.67~12.79%；S/N跨版≤.28%。跨BIT/boot描述性不能把全剩余归因。这个后续结果使10/7中文/日文稿“尚未隔离”应更新为已做隔离、额外miss消除仍性能退化。
- clean功能172最终10/6 23:38JST同boot27m41s172PASS，不是10/6构建启动时已有172，也不是新研究样本或性能验证。module14420→5561LUT与整板165535→156215不能混。
- 文献库最新123独立104全文/105版本19metadata，72是初轮而非10/6最终总数；NB artifact10/8源码已得但论文未得。literature六脉络/问题关系非领域空白证明，不能重复将全文下载计全文本次阅读/复现实验。

## 续读补充3（第43—44批）

实际120/182，next45，余62。新status85—88/90—91全文与through84旧史7块。stage85绝对WRITE连续前沿Fi、privateQi的e≤min(Fi+Qi)为条件安全最大段，不等最快；真实prepResp入owned slot才许用，READY0的TL VALID非权限。四手推controls长21/23、strongsplit20/22、onesponsor22/24、allfull23/25，均R8/W16/3AWB每flow/ownprep6，未实测；ARcost3 long47split48、cost1 long45split44是解释目标非性能。stage86 WRITE记录可回收READ字段不等总存储。stage90成本3355 vs3587差29B另计至少1024B软件generation及1MiBDDR池，不是总优势。stage91 no-fill返回是翻译响应不是payload。root落库前以ledger120为实际统计，不将旧段107/87/70合加。

## 最终续读与覆盖验收（第45—56批全部完成）

**最终182/182完整阅读，42个来源文件的分配范围，728822 UTF-8字节／408364 Unicode字符，57批全部，未读0／截断0。** 这是分配块全文覆盖；与B单元互补后才是长来源全文覆盖。无本轮raw/源码直接核验。逐块清单字段source/start_line/end_line/chunk_id/full_read保持完整。

末批补充此前简报未展开的独立结果及边界：

1. 固定nested链§45：唯一数学graph6450配置（5族×86B预算×15q），mismatch0，可完成总work30。q=PTE最多连续服务数、B=单suspended frame bit，非cycle/面积/全状态；q3/B77普通semantic_chunks路径3/1/3/1/3/1/3、q4/B36普通semanticrestart4/4/4/3均零重读，q3/B36无解。30仅两个各15步/noPWC/no跨请求共享，普通checkpoint+主动让路已经达下界，停止固定链调度创新。存在完成策略不等任意公平策略皆完成，q2/80bit额外纸构未进枚举/非全架构最小编码。合同`/home/xvhaocheng/work/iommu-platform/out/translation-progress-frontier-20261008/codex-model-contract.md`，运行`frontier_report.json`与`execution-ledger.json`，验收`codex-result-acceptance.md`；报告`docs/translation-progress-frontier-20261008.md`已全文读。
2. §41 geometry math11390分类（7585+3805有重复）greedy/brute0非11390DUT，B>0一cut可支持多个stop点，H只safe目标点距不是预算关后的工作/时限。软件3KiB+7×4KiB+1KiB/9subjob/source24/AW17/8cut仅静态未跑。`out/fragment-drain-boundary-20261008/{geometry_oracle.py,geometry-oracle-result.json,stage-evidence.json}`。
3. §83反向history追加纸账：ordinary A107/B110、candidate A110/B14，共3AWB/sourceAR2/realRW15/null0，candidate A慢3/peer早96是条件取舍非实验cycle/全ordinary最优；原两N12/Q3 normal/gap被futureblind adaptive普通同时匹配。wholechild动态q上界≤Q与wholejob累加、null≤Q(Q+1)/2、AW≤1+Q均工作界非walltime。ordinary pretranslation+nativecoupler+oneprimer逐动作等价，停止两scene调度试验。接口nonclaiming prepare/真实physPA-route/全source目的覆盖是显式新费用，不能fake LAST闭合。`out/innovation-mechanism-focus-20261009/{root-result-acceptance.json,root-capability-and-interface-contract.json,b-positive-design.json,c-mainline-positive-design.json}`。
4. §60—61两个阶段各10completePASS、每流RW256，peer5596/5604被普通resident5596/5596，F128只0.27%差；旧barrier8.7%不能peer独有。`out/shared-read-resident-admission-20261009/codex-result-acceptance.md`。
5. §34 physicalparent1/2/4六RTL对照6PASS且CSV同旧8槽，各末W20213，virtual/AW关联仍8；少2394声明bit非综合/全队列缩减/通用进度。`out/parent-capacity-20261008/{summary.json,stage-evidence.json}`。
6. §32 ordinary准确源gate48B vs8192B四completePASS，CONF0各源AR32/AW32（总128Ax）而旧CONF1总66；gate放行需virtual/physical Wowner/seq同、completionAW成功、偏移长度size真实匹配，并firstVALID held。W末27894vs25146差2748非DONEwindow/纯译址/速度因果。计挡55046是每cycle被挡流数量和可超run长度，不是实际等待。`docs/write-owner-baseline-20261008.md`已全文；`out/write-owner-baseline-20261008/{runs.json,stage-evidence.json,gate-admission-audit.json}`。后继强fixedpost结果由§33记，解除环本身普通已做到。
7. 10/7早期共享pool候选E2（不是旧容量替换E2）13唯一完整scene通过；payload8192→2096B仅配置，singlepool热PN112448较93487慢20.28%且FAST62→0、改分配125677更慢；ordinary2slot103348/FAST31。5SV成功+1CPP诊断，初lint/genmux/AW前后观察fault260为原型错误已修。`out/e2-interface-poc-20261007/{initial-runs.json,reserved-runs.json,acceptance.json,comparison.json,codex-review.md}`。
8. 四翻译长期候选比较`docs/translation-comparison-20261006.md`1—109中MASK一行旧误venue ISCA与后续novelty-gate已更正ASPLOS2018矛盾，按后续关键原文修订处理，不将旧候选排序当前。闭环方法要求固定程序输入而非固定每请求发出时刻，miss/供给/吞吐为可变中介；依赖理想启发式非数学oracle；额外观测净决策收益非分类准确，global最小观测消融未证。

最终应引用的关键链：工程卡死有实板现场与DS接口oldfail/newpass；PLRU局部合成因果与isolated实板miss反事实分开；PN数据返回竞争在行为RTL模型有因果干预、实板尚未全归因；小buffer/conf1写依赖为扩展合同有RTL停滞与普通解；post位置和shared-read弱对照优势随后被强ordinary大幅/精确覆盖；stage79—91仅手推/小Python的多数独有收益关闭或未证，stage93路线暂停保留研究问题不宣称领域无价。所有原始日志/CSV无需逐字读目标已明确，本单元并未抽核这些原始证据，汇总须保留此边界。
