## 2026-10-13 stage117 来源支持交付 G0 六案技术接受

CPU/单DMA/双DMA×即时/整批发布六案同新二进制各一次通过；原1639B/1675B、四span、真实cache/consumer、所有权和物理metadata退休闭合。三包used向量：CPU即时[114,392,2051]，CPU整批[1625,1625,1625]，单DMA即时[1209,1804,1804]，单DMA整批[1455,1455,1455]，双DMA即时[1138,1913,1913]，双DMA整批[1895,1895,1895]（共同就绪后模型周期）。CPU即时最早前两包，单DMA整批最早第三包；不存在按全部资源的统一赢家。

普通即时控制自然出现实际consumer/peer后端重叠。双DMA即时首包早71、后两包晚109且资源/前缀metadata不同；全部DMA翻译12/命中11，单/双DDT流量80/144B。没有具体可达可避免翻译损失的因果证据，不进G1；全CPU/ALU/软件/应用/FPGA成本null。停止自主扩矩阵，保留普通适配资产；下一范围是有新增原始事实的单个工作流，GPU/AI优先，先核实际共享路径和普通强对照，不恢复关闭分支、不加压、不重跑。

新探针1次＋六案6次；两成功构建均在运行前，最终320项实际编译依赖；独立7负＋1正控制（实际10次应用）通过。root发现refill允许范围/owner绑定缺口后，只对原六trace补验新增谓词与1负控制：16356次AR全部绑定实际pending native load、本包/span及精确字节闭包；没有重复仿真、旧控制/探针或全verifier。最终审计/释放36804周期单列，生产driver/ABI/RTL和板未变。

root当前实际gpt-6.1-sol/ultra，最终原文/复制/cache/协议/独立checker与研究裁决亲核已接受，模型配置阻塞已解除。此前medium运行与blocked记录原样归档，不声称历史全树一直ultra；两个新席实际Sol/ultra，原生有效上下文475000。技术及五文档交付已完成，实际goal已complete，工具记录646,662tokens、55分12秒；无额外token预算，论文未完成。。没有重跑实验/控制/构建，没有额外预算；论文未完成，原两周窗口累计。

[本轮报告](iommu-platform/docs/stage117-delivery-g0-20261013.md)给出原始证据、成本、稳定技术裁决与下一范围。以下原样保留此前完整历史；其中“本轮complete”各指其历史关口。

## 2026-10-13 stage117真实缓存交接接受

本轮完整交付核验后，实际goal已complete：566,137 tokens，38分36秒，无额外token预算；论文未完成。

stage117真实缓存交接功能已接受：实际WT缓存/controller、共享CPU/DMA物理backing及metadata闭合；3项微测试、两个同二进制新案例各一次通过，独立8项篡改拒绝＋1项PASS独立性控制通过。包1取得/发布时peer活跃，真正消费重叠未观察到，不加压或调序重跑。每包缓存交接258模型周期，重填18480B与不缓存表读取12288B分开计；完整CPU/应用/FPGA成本null。无新G0/收益/创新声明，论文未完成。

下一步固定来源支持交付目标与合法CPU/单DMA/普通多DMA强对照，完整计成本后一次G0；不重复缓存关口、不恢复关闭分支，原两周窗口继续累计。

关键边界：实际端口驱动、controller输入，未执行完整CPU指令；peer-active缓存交接/发布与实际消费分别裁决。CPU真实初始化、每行清空、WT B退休、最终扫描后release_quiet均亲核。生产driver/ABI/RTL未改，三运行各一次，6失败/2成功构建及隔离工具成本保留。root与两席全Sol/ultra。完整交付核验后才实际goal complete。

[报告](iommu-platform/docs/stage117-real-cache-handoff-20261013.md)提供原始证据、成本、实际goal回执和交付导航。以下完整保留前轮历史，不修改旧有限G0裁决。

## 2026-10-13 stage117逐包协议模型功能接受

本轮完整交付检查通过后，实际goal已complete：工具记录673,693 tokens、30分05秒，无额外token预算；论文未完成。

stage117逐包协议模型已实现并接受：17项协议测试、两个同二进制新增RTL案例各一次通过；独立原JSON/总线/实际访问核验及14项篡改拒绝、1项独立性控制通过。包1/2消费时peer实际后端仍活跃，本包R/W/B已退休。原1639B/1675B、四span及布局不变，生产driver/ABI/RTL未改，旧实验未重跑。MODEL_UNCACHED_ASSUMPTION，实际CVA6缓存、应用时延、生产持久隔离未验证，未知成本null，无新G0/收益/创新声明，论文未完成。下一步是原工作负载下最小真实CVA6缓存交接见证，闭合alias、metadata和cache成本后再冻结一次G0普通对照；原两周窗口继续累计。

root实际Sol/ultra、实现Astra/ultra、checker Sol/ultra；直接关键源码审查及范围修正、实际编译依赖160项、产物214项已核。宿主耗时仅执行成本，软件/cache/application RTL时延null。首次单元编译警告和包装字段解析失败均保留；一次RTL构建、两次案例、零重试。

[报告](iommu-platform/docs/stage117-packet-protocol-adapter-20261013.md)。完整成本、原始记录、实际goal回执和交付核验从报告导航。以下完整保留前轮记录，当前功能接受不改变旧有限G0裁决。

## 2026-10-13 stage117逐包交接可达性：需要具名有界软件适配

真实登记同源有限goal，无额外预算；root核DPDK完成/发布/归还，新Astra/ultra独占席核平台完成/cache/所有权，root最终接受。source支持及时poll/通知完成前缀，持续submit不poll会耗尽DMA ring；host mbuf释放是A侧复用，不等于B guest消费/归还。没有到达率/期限或新性能数值。

tracked driver双DMA poll/refill与DEFAULT精确span已存在，但公开pattern/研究ioctl无逐包输入、完整包join、CPU acquire/publish/consume/return，现CPU校验只在双lane drain之后。R107有限allidle不代表平台只能串行；本轮只引用其历史范围，不恢复或复跑。

新增决定事实：f9冻结源实际WT_DCACHE、128bit即16B行；fence.i清I/D cache、等待CPU写buffer/MSHR，无all-DMA-idle输入，所以“全cache flush→全DMA必须静止”不成立。原地址/容量闭包跨group交集空，header/payload共享行起点8336/8608/9040；CPU不可peer-active全池扫guard，metadata/alias还须绑定。NO_ERROR_HANDLING把error固定0，DONE不单独证明字节成功；FQ限频日志/observer守恒不是包关联错误收据。受控完整字节success-path保留，全生产异常恢复不移成所有G0前置，也不据此强制新增RTL。

root选择下一方向为具名软件packet ownership/cache glue：完整span join、缓存交接、按前缀used发布、原cookie消费后B归还、源读退休与B回收分开、保留驻留leaf到最终checked release，不确定状态retain/quarantine。本轮不实现，不冻结新G0/后继goal；适配是普通基础能力，不作创新或收益声明。供给/poll、全cache/refill、consumer、源持有/校验和释放成本未知null。旧1161/1601有限G0接受不改，不从440差额推新逐包性能。

13/14关键tracked↔f9源相同，top模式不同（f9 NonblockMode3/E3Observer0、tracked0/1；DMA0/1缓冲512/3beat）。当前板身份未新核，历史不冒充current。root实际Sol/ultra有效475000，新平台席Astra/ultra有效258400，未手改上下文/模型历史；无DeepSeek/Claude、生产修改、实验/构建/探板/部署、另选源或旧席恢复。新静态产物核验与所有文档同步后才实际complete，论文未完成。

[本轮报告](iommu-platform/docs/stage117-packet-handoff-gate-20261013.md)／[稳定裁决](iommu-platform/out/stage117-packet-handoff-gate-20261013/root-result-acceptance.json)／[适配范围](iommu-platform/out/stage117-packet-handoff-gate-20261013/adaptation-scope-card.json)／[证据导航](iommu-platform/out/stage117-packet-handoff-gate-20261013/README.md)。

本轮已在root稳定接受、全部计划/状态/交接/验证同步与246项交付检查通过后，由实际update_goal返回complete；工具记录308,316 tokens／29分13秒，无额外token预算，论文未完成。[实际完成回执](iommu-platform/out/stage117-packet-handoff-gate-20261013/root/goal-completion.json)／[交付核验](iommu-platform/out/stage117-packet-handoff-gate-20261013/root/delivery-verification.json)。

以下保留原阶段完整记录；上轮有限G0关闭范围及原关口规则不改，本节不把需要新增软件合同当作已实现的逐包能力。

## 2026-10-13 stage117计划修订与G0：四案已接受

用户采纳整体/局部复核后，真实登记本轮goal，无额外token预算。当前规则改为来源/接口→G0有界普通性能判别→G1→最小原型→FPGA关键验证；不要求实测残余后重复同题。前轮三结果只验功能/局部成本，不能关闭未测并发性能；Dell是主机支持端，平台仍为Genesys2/CVA6＋四iDMA＋IOMMU＋Linux。两周为原批次整体立题窗口，成本按阶段补齐，未知null。

一次同源亲核保留多通道/目的预留/前缀发布；全局quiet删去原生逐包消费重叠，native流式投影不适用，只准有限phase硬件就绪/排空。保持原三帧1639B/1675B记录、4 exact spans、同owner、DID10/11及相同2MiB驻留映射。真实DEFAULT一引擎一公开在途job，双方使用捕获后prepare下一笔、匹配公开DONE后触发；不以FIFO8伪造四笔预排。

四案同一binary各跑一次：solo0/solo1全部排空861/1051；single两组就绪855/1155、全部1161；parallel1426/1596、全部1601模型周期，有1333周期实际重叠。普通single以一活动引擎覆盖这批parallel完成向量，结束有限性能主张，不进G1；440周期差额混合供给/冷DDT/数据/翻译，未作独占归因。组0完成时peer503B只是包3分段，完整包0包/0B；组1完成时peer2包/125B。solo不发布used，完整案quiet后按原cookie消费/归还；应用时延、CVA6基线、板端成本未知。

两个新Astra/ultra独占席，root亲核与接受；2成功构建、4成功运行、2离线检查，第二次仅修进度口径，无RTL重跑。112输入/73产物哈希、原始事件/快照/成本可导航；旧资产和独立贡献保留。无外部模型、生产RTL/driver/ABI改动、板测/部署、加压/另选源或关闭分支恢复。交付与同步核验通过后才实际complete，论文未完成。

下一步是需求/投影范围决定；继续同源须有新增原始事实支持普通控制外的性能条件，且现平台能保留决定性就绪/消费依赖，再冻结一次G0。当前没有后继实验goal，不自动做暖态/换序/压力搜索或完整vhost移植。

[报告](iommu-platform/docs/stage117-plan-revision-g0-20261013.md)／[稳定接受](iommu-platform/out/stage117-plan-revision-g0-20261013/root-result-acceptance.json)／[当前规则](iommu-platform/out/stage117-plan-revision-g0-20261013/revised-gates.json)／[证据导航](iommu-platform/out/stage117-plan-revision-g0-20261013/README.md)。

工具实际update_goal已在root接受、全部文档同步与110项交付核验通过后返回complete；记录354,222 tokens／30分15秒，无额外token预算，论文未完成。[实际完成回执](iommu-platform/out/stage117-plan-revision-g0-20261013/root/goal-completion.json)／[交付核验](iommu-platform/out/stage117-plan-revision-g0-20261013/root/delivery-verification.json)。

以下原阶段记录逐字保留；“普通功能满足→G0不准入”和“必须先有残余才G0”属于当时规则/功能范围，当前执行由上面的修订与实际四案结果取代，不能再当作未测性能裁决。

## 2026-10-13 stage117续行：三个实际结果已接受

同一C117R3已实现并实跑：有限packet/descriptor/lifetime适配、Dell直接CPU基线、真实iDMA＋启用翻译IOMMU的单DMA RTL基线。共同generated功能输入为60/65/1514B三帧、1639B有效帧/1675B含header接收record、4 exact spans；不是native trace或自然到达。CPU真实返回B原cookie的view并显式归还，16项定向检查通过；源退休前保持、退休后复用不损B。CPU最终有限生命周期29,830ns，交付phase5128ns、observer24,344ns、记录间隙358ns；输入物化/编译/进程启动等另列，单次值不作性能推断。

单DMA RTL四笔全部真实接受/完成，6 AW/B、WSTRB1639B，原DDR快照独立核验通过；非等值Sv39/2MiB叶驻留，12次lookup为1初次miss＋11hit，配置至quiet1227周期。CPU墙钟与RTL周期不作速度比；真实cache、Linux映射/释放、物理大页保留、native与板端成本仍未测。普通CPU/单DMA已功能满足这批输入，没有建立可避免共享翻译残余，G0/G1不准入；此轮投影收束，保留适配，新增实际普通方法之外损失才重新判别，不自动加压/扩多DMA/另选源。

root亲核接受三项实质结果；Astra/ultra两个新独占执行席，无DeepSeek/Claude。普通CPU与UBSan通过；初次ASan反复DEADLYSIGNAL被终止，原因未定，后续有界handler-off address＋undefined检查通过，失败记录保留。生产driver/RTL/ABI未改、未部署/上板，源码/仿真/本机结果分别陈述；论文未完成。实际goal已complete，[完成回执](iommu-platform/out/stage117-three-results-20261013/root/goal-completion.json)。

[报告](iommu-platform/docs/stage117-three-results-20261013.md)／[三项验收](iommu-platform/out/stage117-three-results-20261013/root/critical-review.json)／[CPU原始运行](iommu-platform/out/stage117-three-results-20261013/cpu/run.json)／[DMA完整交付](iommu-platform/out/stage117-three-results-20261013/dma/results/delivery.json)。

以下原阶段文字保留为历史，“适配未实现/下一基线inactive”描述的是本轮以前状态。

## 2026-10-13 stage117-r3：C117R3来源条件准入

只冻结DPDK vhost v25.11本地VM→VM任务；guest预发布目的buffer与used-ID/cookie回收形成新增必要交付依据。允许CPU/同owner单DMA直接A→B完整强对照，不强制两次host staging，不把必要搬运等同DMA不可替代。原生共同启用IOMMU和性能未知；有限FPGA可用现非batch DEFAULT精确地址/长度，需明确的有限软件glue与全局quiet/cache交接，尚未实现，不主张逐槽并发或native运行。

来源准入下一次有界普通基线；G0/G1、新机制与收益均未准入。所有无复制/页重映射/直接放置、CPU、单DMA、大页/预映射/驻留/批量/布局/稳定池与真实全成本保留。只有普通残余实证成立才G0；适配越界或普通已覆盖时停止本投影并作范围/平台决定，不自动stage117-r4。原历史、关闭卡和独立贡献保留，旧文档已before快照。

实际仅新原始源码与交付检查，无实现/构建/板测/旧重跑。Astra独立goal已在root最终接受后实际complete，原始回执在stage117-r3证据目录goal-completion.json；父stage115未主动改写，先前paused与后来工具null分列、原因unknown。论文未完成。

[报告](iommu-platform/docs/stage117-r3-astra-20261013.md)／[证据导航](iommu-platform/out/stage117-r3-astra-20261013/README.md)／[下一inactive合同](iommu-platform/out/stage117-r3-astra-20261013/next-goal-contract.json)／[交付核验](iommu-platform/out/stage117-r3-astra-20261013/delivery-verification.json)。

以下保留原阶段记录；其中“下一stage117-r3未执行”等为当时历史状态。

## stage116 路线裁决已交付（2026年10月13日JST）

## 2026-10-13 stage118转向与stage117-r2规则修订/重做

用户要求困难时重訂stage117规则并重做。C117现FPGA需要载荷接口及图像consumer适配，stage118给出needs_named_adaptation并条件归档该投影；不启动stage119适配。新规则先核任务必须搬运及原地/不复制强对照，再核现平台接口。仅新选C117R2 DPDK DMA复制转发；源码/API允许同owner有效独占mbuf原地改MAC后TX，未建立任务必须独立copy需求，因此本卡不准入基线/G0/G1。对照未运行，不声称速度收益或普遍无copy需求。

9份exact-commit DPDK原始文件、关键原文、完整字节/资源/未知成本、普通对照与stable接受已保存；旧研究/独立贡献原样保留。实际stage115 paused未修改，两项任务均未注册goal。唯一stage117-r3必要搬运优先来源合同未执行；论文未完成。[报告](iommu-platform/docs/stage117-r2-platform-first-20261013.md)／[证据导航](iommu-platform/out/stage117-r2-platform-first-20261013/README.md)／[交付核验](iommu-platform/out/stage117-r2-platform-first-20261013/delivery-verification.json)。


## stage117 — 2026-10-13 单一Holoscan双目来源预筛交付

C117条件保留：源码任务/owner/持续MR及GPU数值消费结构成立，actual common translated path与source-faithful现FPGA写入/consumer/预算缺项，未准入基线或G0/G1；不作科学阴性。22份exact-commit源码、20处关键原文、全部普通对照/成本项目已保存。MR地址空间与host IOVA、allocation lifetime与slot freshness、4K代码默认与1080文档分列。实际stage115 paused未修改，本轮未注册goal。唯一stage118静态接口判别合同未启动；本轮不代表论文完成。

[报告](iommu-platform/docs/stage117-holoscan-stereo-prescreen-20261013.md)／[证据与接受](iommu-platform/out/stage117-holoscan-stereo-prescreen-20261013/README.md)／[交付验证](iommu-platform/out/stage117-holoscan-stereo-prescreen-20261013/delivery-verification.json)。


用户明确选择“先做路线裁决”及“分开来源与基线判别”，随后要求Implement the proposed plan。C113原生入口仍无可接受同run包；现有有限接口的数值消费者/逐槽handoff与自然peer未成立，DDR固定池还删去native NVMe请求生命周期，当前不批准投影。条件封存C113，未知不作科学阴性；唯一冻结stage117未关闭来源的单一预筛合同，未执行或选择第二workflow。

来源关保留真实task/owner/dependency/bytes、实际共享翻译路径/自然peer、合法接口、支持预算与全部普通控制/成本项目；完整数值、成功完成、资源和成本由来源通过后的另一次有界普通判别取得。具体可达可避免残余成立后才登记正式G0，G0支持后才G1。stage115原完整包目标未降低或标complete，实际仍paused；stage116未注册为goal，不重复已知接口拒绝。

亲核20组源证据及当前tracked driver：有限模块在已验repair snapshot，tracked idma.c有pair/composition实验入口，不能将有限模块的整组idle限制推及所有并发能力。CPUAdam的host Torch/PyBind源码不证明已有CVA6数值runtime。位置猜测失败已纠正并入账，无旧模块恢复/新实现。零新agent/外部模型/源搜索/driver或RTL改动/build/board/部署/旧实验与校验器重跑，关闭卡保持；原记录字节及父goal完整身份保留。论文未完成。

[完整报告](iommu-platform/docs/stage116-c113-route-decision-20261013.md)／[稳定裁决](iommu-platform/out/stage116-c113-route-decision-20261013/root-result-acceptance.json)／[三个关口](iommu-platform/out/stage116-c113-route-decision-20261013/gate-contract.json)／[唯一下一合同](iommu-platform/out/stage116-c113-route-decision-20261013/next-goal-contract.json)／[交付核验](iommu-platform/out/stage116-c113-route-decision-20261013/validation/delivery-check.json)。

以下旧stage115 paused/blocked及早期关口条目保留为历史；当前执行路线以上述stage116合同为准，原stage115实际目标仍paused未完成。

## stage115 实际状态观察为 paused（2026年10月13日JST）

用户询问当前工作和卡点时，实际get_goal返回paused，完整目标/身份与无额外预算保持，覆盖当前页的旧blocked状态。本席本轮未调用update_goal暂停，状态变更主体不可读，不归因或伪造恢复。研究执行停止，仅同步状态记录。原生NVMe包仍未取得，C113条件保留/FPGA不准入、关闭范围与既有证据不变；目标和论文未完成。前面的纯状态复述不计研究进展。

[实际状态观察](iommu-platform/out/stage115-c113-native-bundle-20261013/paused-observation-20261013/actual-goal.json)。

## stage115 再次实际登记 blocked（2026年10月13日JST）

最新新来源补查已收束后，连续三个用户继续请求仍未提供关联原生包或范围调整；当前一级资料入口亦无新增。旧审计不计入这三次继续请求。实际工具从未被声称恢复active；本轮再次update_goal标blocked，完整目标/身份/无额外预算保留，未complete、未另建goal。停止目标工作，等待具体新输入或范围澄清。无查源、实现、实验、构建、部署或新席；C113条件保留、FPGA不准入及关闭范围不变，论文未完成。

[三次继续请求审计](iommu-platform/out/stage115-c113-native-bundle-20261013/renewed-block-20261013/renewed-blocking-audit.json)／[最新实际回执](iommu-platform/out/stage115-c113-native-bundle-20261013/renewed-block-20261013/goal-blocked-receipt.json)。

## stage115 新原始来源补查，实际 goal 仍 blocked（2026年10月13日JST）

按用户继续指令保留完整获取目标，定向检索后只核一个新来源：DeepSpeed Discussion7447。作者报告NVMe参数换入的尺寸/数量断言，贴出的optimizer device仍CPU；版本只引用0.17.2源分支，未有实际版本/启动/解析配置、物理IOMMU拓扑、完整完成或普通成本。拒绝为C113 NVMe CPU-Adam目标包；不把GRPO等另立候选，不把名义缓冲字节当分配成功或搬运字节，不当科学阴性。原HTML/hash、六组root原文、配置与两种case边界已保存；未执行作者修改。

实际goal仍blocked，完整objective未变；此前实际create_goal被未完成目标阻挡，本轮没有再调用或假称active/complete。一个新入口查完即收束，无新agent/外部模型/部署/构建/板测/源码改动/旧重跑；原C113条件保留、当前FPGA不准入和各关闭范围保持。获取目标和论文未完成。

[本轮报告与原证据](iommu-platform/out/stage115-c113-native-bundle-20261013/source-continuation-20261013/README.md)／[稳定入口裁决](iommu-platform/out/stage115-c113-native-bundle-20261013/source-continuation-20261013/entry-result.json)／[本轮记录核验](iommu-platform/out/stage115-c113-native-bundle-20261013/source-continuation-20261013/delivery-check.json)。

## stage115 同一 goal 第三轮：实际 blocked（2026年10月13日JST）

连续第三轮仍未取得同一既有原生 NVMe 运行证据包，也未收到资料路径/链接或新的匹配一级入口。已核官方模板和作者 CPU offload 日志无法闭合实际 NVMe 运行、共同翻译单元/自然 peer、完整完成与普通成本；没有可独立推进的授权工作。达到 goal 工具的连续三轮门槛，实际 update_goal 已标 blocked，原完整 objective、创建身份和无额外预算均保留；未 complete、未 paused、未另建或启动后继 goal。

目标工作停止，保留关联原生证据或用户明确调整范围的输入条件；用户恢复时重新开始阻塞审计。当前 C113 条件保留/FPGA 不准入、各关闭分支不变，未知不作科学阴性。仅同步状态与交接记录，无新增席/外部模型/查源/部署/实现/构建/板测/实验。证据获取目标和论文均未完成。

[第三轮观察](iommu-platform/out/stage115-c113-native-bundle-20261013/turn-3/intake-observation.json)／[三轮审计](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocking-audit.json)／[实际 blocked 回执](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocked-receipt.json)／[保留原目标](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-registration.json)／[记录同步核验](iommu-platform/out/stage115-c113-native-bundle-20261013/validation/blocked-status-check.json)。

## stage115同goal第二轮：无新原生资料，仍active（2026年10月13日JST）

用户再次要求按计划继续；实际get_goal核同一stage115仍active，完整objective未变，无新goal或额外预算。未收到nativebundle路径/链接，工作区与platform out仅一级新相关目录名检查无匹配，不重扫旧source/日志、不重复已结束原入口检索，不作全局无资料断言。缺项为同一已执行NVMe运行的version/launcher/config、physicaltranslatedpath/commonunit/naturalpeer、完整字节/结果/完成/释放及ordinary成本；已查CPU日志与NVMe模板不能拼接。无新agent/build/test/设备/部署/生产改变；C113旧裁决和关闭范围保持。

同一外部缺项在本stage115 goal连续第2turn阻止有意义进展；未达到3turn规则，所以保持active，不complete/paused/提前blocked。若下轮仍同缺项且无独立进展，实际update_goal标blocked；新原始资料到来则继续同goal，不用重复审读维持active。本轮只记真实输入/状态，不作为研究贡献或论文完成。

[实际本轮观察](iommu-platform/out/stage115-c113-native-bundle-20261013/turn-2/intake-observation.json)／[连续审计](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocking-audit.json)／[保留原goal](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-registration.json)。

## stage115 active：同一C113原生包获取未完成（2026年10月13日JST）

真实goal已登记，无额外预算，目标保持“取得并核关联原生NVMe运行包”；当前未取得，active，不complete/paused/提前blocked。本轮新核官方旧DeepSpeedExampleslinkedcommit：NVMe模板存在，默认launch另一配置且无关联rawrun。独占新Sol/ultra/fork-none席核SC22作者AE中的zero-infinity对照，原归档两log均CPUoffload0.5.7/pathnull/oneGPU，拒为C113NVMe包；NVMe模板不能倒灌CPUrun。root亲核原命令/版本/config及原ZenodoMD5/ZIPsize/log成员bytes。两个新入口有界结束，不重复旧查源；没有第二workflow或新部署/实现/build/board/workloadtest/旧重跑。C113旧源合同/完整合法普通对照和成本保留，不作科学阴性。

已异步请求已有同run证据包路径/链接，保存时未答。仍缺实际run/config/path/IOMMU/commonunit/naturalpeer/shapebudget/fullbytesresults/DONEquietrelease/ordinary成本残余；无新input不重写卡/另建小goal填空。当前stage115缺项审计第1goalturn，三turnblocked门槛未满足；同一缺项连续3turn且无独立有意义进展时必须实际工具blocked。此保存是partial intake交接，不是goal完整交付或论文完成。

[入口报告](iommu-platform/docs/stage115-c113-native-bundle-20261013.md)／[原证据导航](iommu-platform/out/stage115-c113-native-bundle-20261013/README.md)／[稳定入口结果](iommu-platform/out/stage115-c113-native-bundle-20261013/intake-result.json)／[active实际快照](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-checkpoint-active.json)／[原归档核验](iommu-platform/out/stage115-c113-native-bundle-20261013/validation/original-archive-check.json)／[同goal继续合同](iommu-platform/out/stage115-c113-native-bundle-20261013/continuation-contract.json)／[首次缺项审计](iommu-platform/out/stage115-c113-native-bundle-20261013/goal-blocking-audit.json)。

## stage114 C113同源原生证据关（2026年10月13日JST）

来源合同条件保留并增强自然NVMe容量需求，当前FPGA翻译实验不准入。一个工作流，原论文Table1/Figure5c单节点0.5T/16参与V100/MP1/h18432/124层/seq1024/batch8/NVMe参数与optimizer；标称p/m/v约6e12B超过CPU1.5TB，仅是nominal容量界，不是exactP/H/filebytes或固定v0.16.4运行。fixedcode官方测试的worldsize1/twoLinear2048/tmpdir/B4/p1/q8/pipelinefalse、wholeparameter分组和稳定有界256MiB预期AIO补齐；源码6step调用含checkpoint恢复重做，不当实际6更新或完整成功byte。engine默认CPUAdam工厂已root亲核，step标量/p-m-v三文件界正确。

精确v0.16.4查询唯一issue3024实际CPUoffload，拒为NVMe日志；精确sha actions0条不当从未跑CI。原论文/SC21/附录各run边界原样保留，不跨版本/跨图拼记录。仍缺关联nativebundle闭合path/FS/block/PCI/IOMMU/domain/commonunit、naturalphysicalpeer、N/B/p/H/page、数值/byte/DONE/quiet/释放和ordinary完整成本残余。普通wholeCPU驻留只在published0.5T容量界被排除；合法tile/pool驻留、大页、premap/persistentmapping、布局/批量/单设备/CPU路径和全成本保持。公开材料核查有界收束；无新原始记录不重复同查、不造压力/期限、不实现新系统/FPGA/driver，不自动第二workflow/G0/G1。

Codex-only，root＋一个必要新gpt-6.1-sol/ultra/fork-none独占publishedboundary席完成；root26原文组并视觉PDF4/10/14，子席18组/12hash入口，失败GET/403原记录保留。零DeepSeek/Claude/Astra、旧席恢复、context手动覆盖、build/workloadtest/board/部署/生产改动或旧已验实验重跑；R107/C112与原历史/独立维护保持。真实goal无额外预算；完整状态/交接/验证同步与交付核验后已由工具标complete，本轮不是论文完成。

[报告](iommu-platform/docs/stage114-c113-native-evidence-20261013.md)／[稳定裁决](iommu-platform/out/stage114-c113-native-evidence-20261013/root-result-acceptance.json)／[证据导航](iommu-platform/out/stage114-c113-native-evidence-20261013/README.md)／[实际账本](iommu-platform/out/stage114-c113-native-evidence-20261013/execution-ledger.json)／[交付检查](iommu-platform/out/stage114-c113-native-evidence-20261013/validation/delivery-check.json)／[实际完成](iommu-platform/out/stage114-c113-native-evidence-20261013/goal-completion.json)／[inactive下一合同](iommu-platform/out/stage114-c113-native-evidence-20261013/next-goal-contract.json)。

## stage113单一来源／可达性裁决（2026年10月13日JST）

C113 DeepSpeed ZeRO-Infinity NVMe tiled CPU-Adam来源合同条件保留，当前FPGA实验不准入；整体仍阶段一立题，无G0/G1、新规则或论文完成。固定v0.16.4/e2dc3eeb的新事实是CPU更新p/m/v真实回写依赖与app pinned池/driver request map-unmap生命周期不同；两个句柄不代表两个physical DID，数据更新不强制remap。root直接核任务/所有权/依赖、符号完整字节、合法大页/预映射/驻留/批量/布局/稳定scratch/单DMA/CPU强对照和全成本。实际path/FS/PCI/domain/共享peer、N_i/B/p/H/数值/完成及ordinary外残余未知；不当科学阴性。

唯一新Sol/ultra/fork-none子席独占AIO→Linux NVMe边界并完成，57原文组/版本snapshot保留；root亲核41组及实际FPGA四iDMA mux→IOMMU路径。原有限接口串行且只CPU字节检查，不冒充Adam/NVMe运行。源wait数量不等于成功byte；普通持久mapping有合法driver接口但应用pin不是这个接口，成本不得免费。当前投影不给准入，source合同仅条件保留。零DeepSeek/Claude/Astra、实现/build/RTL/workload test/board或旧重跑；R107/C112等关闭不变，原历史和独立维护未改。

下一限定同源既有原生配置/运行证据：先path→设备/domain/自然peer，再形状/预算/全byte数值与ordinary残余。无新资料不实现adapter/driver或G0，不造peer/压力，不自动追加第二源。实际goal无额外预算；完整交付与状态同步后已由工具标complete，回执见下。root实际thread已读但runtime model/effort/window字段未可读，不以旧记录冒充本席验收。

[完整报告](iommu-platform/docs/stage113-zero-infinity-admission-20261013.md)／[稳定裁决](iommu-platform/out/stage113-zero-infinity-admission-20261013/root-result-acceptance.json)／[原文导航](iommu-platform/out/stage113-zero-infinity-admission-20261013/README.md)／[实际账本](iommu-platform/out/stage113-zero-infinity-admission-20261013/execution-ledger.json)／[交付核验](iommu-platform/out/stage113-zero-infinity-admission-20261013/validation/delivery-check.json)／[goal真实回执](iommu-platform/out/stage113-zero-infinity-admission-20261013/goal-completion.json)／[inactive下一合同](iommu-platform/out/stage113-zero-infinity-admission-20261013/next-goal-contract.json)。

# 2026年10月13日 新主会话交接与启动提示词

用户明确换新主会话继续，保持gpt-6.1-sol/ultra。已保存独立交接包、完整启动词和未登记的stage113有界来源选择/可达性合同，承接stage112归档后的phase1路线；区分来源核查goal与尚未成立的实验/G0入口。新席先说明问题/证据/进入与结束条件，实际create_goal不加预算并执行，交付齐备才complete。旧R107/C112及更早关闭范围、已验数据/校验器/构建不重跑或恢复。原三旧席仍未真正关闭，不声明已释放。本主会话只保存交接，没有新研究goal或实验，也没有创建新主会话。

[交接入口](iommu-platform/out/new-main-handoff-20261013/README.md)／[启动提示词](iommu-platform/out/new-main-handoff-20261013/START_PROMPT.txt)／[新席有界合同](iommu-platform/out/new-main-handoff-20261013/next-goal-contract.json)。下面已有阶段历史原样保留。

# 2026年10月13日 stage112新增消费需求／普通边界裁决

stage112新增需求裁决完成：归档C112当前FPGA翻译缺口投影，R107继续归档，无G0/G1。固定vLLM TP2每层attention输出/MLP输出共24次RowParallel归并、GraphAllReduce持久buffer已亲核；同两序列decode形状每rank活动12,288B/两prefix24,576B，授权普通布局可放既有backing slack原leaf，未执行且完整M未知。原板24worker-layer/12joint只是全恢复后CPU字节检查，新的partial输出/collective完成与实际共享requester未成立。保留合法大页/premap/residency/batch/singleDMA及完整准备、计算、归并、释放成本；wire/compute/arrival/deadline和新translation残余未知，不填0。不扩场景/压力、不重跑旧回放/旧校验器，不做实现/build/板动作。实际goal由工具标complete，无额外预算；本轮交付齐备，论文仍立题。

用户补充顺序已核：旧r109_artifact_identity/r111_build_staging/r111_ordinary_interface_plan原始FINAL分别09:07:32/12:13:23/12:10:57UTC，历史任务完成且无后续dispatch，12关键产物保留。当前三席仍pending_init，协作close不可用；archive不当作close，零interrupt/archive/关闭/释放断言，无占用原因猜测或配置修改。root已完成关键任务，本轮无需换主会话；真需独占席时以保存交接进入新主会话，继续Sol/ultra。

[报告](iommu-platform/docs/post-r107-demand-gate-20261013.md)／[裁决](iommu-platform/out/post-r107-demand-gate-20261013/root-result-acceptance.json)／[源码与原数据](iommu-platform/out/post-r107-demand-gate-20261013/critical/root-source-review.json)／[字节合同](iommu-platform/out/post-r107-demand-gate-20261013/source/finite-consumer-byte-contract.json)／[旧席原证据](iommu-platform/out/post-r107-demand-gate-20261013/validation/agent-lifecycle-review.json)／[新主会话入口](iommu-platform/out/post-r107-demand-gate-20261013/new-primary-handoff.md)／[下一合同](iommu-platform/out/post-r107-demand-gate-20261013/next-goal-contract.json)。

下面历史内容原样保留。

# 2026年10月13日 stage111 R107普通实板恢复／全成本裁决

stage111普通恢复／全成本关实际交付齐备，R107本有限卡归档；原goal已由实际工具标complete，无额外预算。r1真实boot370a9dd6…/S0，4×4MiB、8真实2MiB叶；priorout＋restore各1,769,472B/432tasks、144whole batches、24worker-layer/12joint、全部raw/字节/quiet/checked release通过。cold先前换出8miss/8S1walk，resident恢复1728hit/0miss/0walk；不把REFILL或未匹配layout差全部归翻译。kernel whole nominal21.960s，恢复45.192ms、条件Dell40.557–49.914ms；实验全字节/seed/output与ordinary成本分列，SI/精确peak未知。原pre-NEXT EBUSY是root配置idle判定错误，旧source/guard/raw保留；最小一文件修复29项冻结RTL/C回归，第二干净构建59.542s后完整一次硬件pilot通过。无旧stage106/108重跑、新RTL/G0/G1。下一回phase1新显式需求与新增原始事实，不自动扩场景或恢复关闭分支；方法论文仍立题。

[报告](iommu-platform/docs/r107-ordinary-pilot-20261012.md)／[根裁决](iommu-platform/out/r107-ordinary-pilot-20261012/root-result-acceptance.json)／[原始接受](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/validation/actual-finite-pilot-validation.json)／[全成本](iommu-platform/out/r107-ordinary-pilot-20261012/repair-1/costs-and-boundaries.json)／[下一合同](iommu-platform/out/r107-ordinary-pilot-20261012/next-goal-contract.json)。

下面历史内容原样保留。

## stage111原goal已由工具标blocked：等待最终新SD批准

同一新SD批准缺失连续第三个goal turn出现，独立接口/构建/备份/真实dry-run/实际执行适配均已备齐，当前没有可继续的独立授权工作。自动goal继续消息不是SD许可。本次只核当前产物/备份实际bytes/hash与缺失批准/newboot/timer/RUN记录，payload ade1bd4a…、backup21ba2506…未变，实际SD/重启/DMA均0；不重跑旧已验项目、扩场景或替换较小目标。

已调用实际update_goal(status=blocked)成功，原objective完整保留，不是complete或科学否决。明确批准部署后恢复这个同一goal，核newboot、计时、单次普通全成本及稳定R107决定；不另登记较小goal或G0。goal累计token/time为工具原值，详见receipt；并非native context当前占用。

[实际blocked回执](iommu-platform/out/r107-ordinary-pilot-20261012/goal-blocked-receipt.json)／[三轮阻塞审计](iommu-platform/out/r107-ordinary-pilot-20261012/approval-blocking-audit.json)／[具体待批准方案](iommu-platform/out/r107-ordinary-pilot-20261012/deployment-proposal.json)。旧记录原样留在下方，仅当前入口状态更新。

## stage111继续：实际执行入口已备齐，SD批准仍待回复

完成新kernel实际boot gate、CAPS原始记录解析、独立Dell计时上下界、单次RUN与超时不重跑guard、完整8个原始数据文件的有界回传。21项离线接口/拒绝检查通过；计时算法2正14反通过。root亲读数学和实际源，平均率相对Dell CLOCK_MONOTONIC，未来频率稳定／SI准确度不由synthetic或CPU:TIME比例证明。实际样本为空、actual频率界null、新boot和DMA未验实。没有新UART/网络/SD/FPGA/DMA动作，没有重复旧回放或构建。

先前具体SD许可尚未收到，自动goal继续消息不作为批准；同一goal仍active，没有缩小目标或标complete。原部署方案/原问题保留，不再重复询问。所有剩余实际部署、newboot、计时、单次ordinary、全成本与R107稳定裁决继续受原合同约束。

[执行入口](iommu-platform/out/r107-ordinary-pilot-20261012/design/postapproval-execution-entry.md)／[root界限](iommu-platform/out/r107-ordinary-pilot-20261012/validation/root-execution-adapter-acceptance.json)／[数学与原始离线数据](iommu-platform/out/r107-ordinary-pilot-20261012/validation/timer-bounds-analysis.md)。下方既有记录保留。

## stage111 R107普通接口、干净构建与部署前提（2026-10-12，goal active）

真实goal已登记，无额外预算。仍阶段一立题；R107条件保留，必要additive普通DMA API四池/实际leaf/缓存/checked IOFENCE/NEXT-DONE/全字节/层proxy/安全释放接口已实现并由root亲核。prepare checkedfence失败保留、不重试释放；原错误及修正保留。一次干净kernel/loader/OpenSBI59.066s，release6.5.0-rc2-r107-ordinary-stage111、payload29,363,656B/ade1bd4a…；源码/config/Image/notes哈希已接受。静态RV64及host校验1正26反、失败交付模拟通过，均非FPGA恢复证据。

完整原32MiB分区备份在Dell，SHA21ba2506…两端一致；新payload板端SHA与existing updater实际dry-run通过，下载/hash/预检Dell229.864s。保留原读/传输超时、binary POST拒绝、base64恢复和raw解析修正；不把漏字段猜成成功。181旧主产物/内核输入未改。当前旧Linux运行、UART释放，task HTTP与专用地址保留供最终授权后使用；新SD写入/重启/R107 DMA/G0/G1均0。

最终新SD写入许可已询问，尚未收到，不标goal complete。批准后只同实例一次完整ordinary采证，再稳定裁决；raw计数与板端nominal、独立Dell计时分列。普通覆盖只结束R107卡并回阶段一新合同；技术失败不作科学阴性，不扩场景/加压力/恢复关闭分支。全Codex，无DeepSeek/Claude，新RTL0。论文未完成。

[报告](iommu-platform/docs/r107-ordinary-pilot-20261012.md)／[部署前接受](iommu-platform/out/r107-ordinary-pilot-20261012/predeployment-acceptance.json)／[最终部署方案](iommu-platform/out/r107-ordinary-pilot-20261012/deployment-proposal.json)／[实际账本](iommu-platform/out/r107-ordinary-pilot-20261012/execution-ledger.json)。下方旧记录原样保留。

## stage110 R107平台身份恢复与普通恢复进入关（2026-10-12）

stage110完成JTAG／指定bit／新boot与S0有界核验，R107条件保留。CPU DMI默认IDCODE1被误当FPGA；B接口实xc7k325t/0x43651093。用户批准载入冻结优化f9fa9434…，第一次mask verify通过但SD块CRC失败，第二次同文件不并行readback后启动成功；CRC物理原因未知。新boot 7fd964e5-563f-458d-9617-c890f737f068、config/DTB/notes一致、taint0，observerV9/ABI1/config0xA00=S0/DID10、idle1；选择器还原，未执行DMA。

一次干净内核构建56.886秒，81122项源冻结、2078/2079链接对象与四关键驱动逐字同；临时version元数据32B造成完整Image/ELF未匹配，一次metadata恢复未保留成功中间对象且递归覆盖，停止，不声称完整旧Image来源闭合。下一走新源码冻结ordinary执行内核/接口并匹配其真实boot，避免旧横幅循环。尚无普通分配/map/恢复/全成本或translation残余，不进G0/G1。

相关clock/IO/CDC按真实路径处置，允许有限功能采证条件，绝对秒/吞吐需独立计时校准，不能用50/25MHz声明或2:1比例冒充实测。保留同实例144或合法96笔/1769472B、合法大页/4KiB/premap/residency/batch/singleDMA；不重复stage106/108、扩场景或加压力。133旧stage109文件及46内核输入不变，生产代码/config/原历史保留；两次授权易失载入、无Flash/SD镜像写入，Linux运行、UART释放。全Codex root＋两独占Sol/ultra，零DeepSeek/Claude/Astra。

[报告](iommu-platform/docs/r107-platform-identity-recovery-20261012.md)／[稳定裁决](iommu-platform/out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)／[下一合同](iommu-platform/out/r107-platform-identity-recovery-20261012/next-goal-contract.json)／[账本](iommu-platform/out/r107-platform-identity-recovery-20261012/execution-ledger.json)。仅本关口交付后complete；论文未完成。下方原条目保留为历史。

## stage109 R107执行前提裁决（2026-10-12）

本轮真实goal登记且无额外预算；结论条件保留R107合同资产，actual execution前提未全过，无ordinary恢复／新G0／G1，论文未完成。当前Dell UART实际登录成功，riscv64/cpn3-clean、boot db0664f0-2d67-4b0d-91fb-c67d7557abe6、taint0、四idma；live DTB及kernel notes与本地构建一致，eth0 192.168.77.2/24 UP/LOWER_UP。当前bit/runtime mode/source freeze仍未验实，不能用历史boot填入。

静态候选仅route-opt f9fa9434…，24 overlay对齐，原内部50MHz时序+0.022/+0.051ns、失败0；原manifest f15f8a72…为−0.219ns/1失败，拒混用。IO/CDC与6 clock warnings未完成。JTAG双接口wildcard选择错误已解，精确主目标1MHz只读状态却为Unknown_Device/0x00000001，原因未知；用户确认开机亮灯。没有以此推断bit损坏或自动重刷。

普通adapter范围/必要性已核，保留原完整144或合并96笔/1,769,472B以及强普通大页/稳定池/预映射/驻留/批量/singleDID；没有实现、池分配/map或恢复性能实验。仅补当前bit/S0、精确kernel freeze、相关时序处置等新事实后做同实例采证；loss成立才G0。无新输入重复静态工作到此停止，归档条件与整体阶段一后续路线已登记，不恢复关闭场景。全Codex，129旧输入不变；两个用户授权临时设备ACL、系统凭据库/非秘密引用，密码无项目日志。现有Linux保持运行、UART已释放。

[报告](iommu-platform/docs/r107-execution-gate-20261012.md)／[稳定裁决](iommu-platform/out/r107-execution-gate-20261012/root-result-acceptance.json)／[本次live](iommu-platform/out/r107-execution-gate-20261012/identity/current-live-evidence.json)／[下一合同](iommu-platform/out/r107-execution-gate-20261012/next-goal-contract.json)／[账本](iommu-platform/out/r107-execution-gate-20261012/execution-ledger.json)。原CURRENT_STATUS/五文档快照在out/r107-execution-gate-20261012/before，stage108和stage106原始输入均未改。下列条目保留原样，旧“当前/下一”只指当轮。

## 2026-10-12 stage108 R107有限恢复合同与有界立题裁决

实际create_goal登记执行，无额外token预算；按stage107 next-goal-contract完成一份有限合同、普通准备/成本及稳定裁决。决定：条件保留R107有限合同资产，无新G0/G1。具体实例和source-level控制点已成立，缺实例/不可达归档条件未触发；actual普通准备与残余翻译损失仍未知，不能据CPU回放推进方法，也不能声称科学阴性。

完整OPT-125m官方配置固定revision27dcfa74d334bc871f3234de431e71c6eeba5dd6；L12/Htotal12、TP2/Hshard6、D64、B16、fp16。研究选定双序列共享完整前缀＋两不同尾槽，3唯一槽、ref2/1/1、COW0；槽8而live3/保守预留5，无容量强迫。原source类/形状函数＋CPU代理仅一次实际核验：144完整恢复、1,769,472B，既往swapout另计同量；24worker-layer和源保持/未使用槽guard/完整padding通过。六个池原始数据保存，不是native GPU/trace/FPGA完成。

663,552B是代理checkpoint标记字节；root亲核worker最后token于get_len()-1重送与attention等待后更新再读，Sequence长度不证明native KV-valid/必读量。源refcount、同stream与layer-event已做安全，logical free不撤整池映射；双向同期swap禁止。同可信owner允许授权单DMA四池，连续run96笔保留所有完整字节与层屏障。

四池逻辑9,437,184B。普通4×完整4MiB/order10或8×2MiB/order9，均16MiB backing/8目标叶；当前内核order>MAX_ORDER才拒，旧10MiB/order12限制不套本例。4KiB回退、驻留、预映射、批量及单DMA都保留。actual allocation/map/leaf/cache/fence/全成本与liveImage时序未测；IOTLB16/8叶不证明零miss。Linux helper缺普通manifest/stablepool/layer-event glue，硬件实际preissue地址/长度/NEXT/DONE控制已有；未实现adapter/RTL。

root亲核20源及原始旧summary边界，独立raw/源/保护643项通过；平台200项一次通过；73本轮冻结旧输入不变，stage106八案不重跑。旧校验器通用断言/hash实际复用，旧ready0和six-tensor专用events/output不套新KV。来源两web open失败后实际一次revision查找＋一次固定config下载；只新增此primary，不取权重/trace。代理字节scope标签修订未重跑CPU，初次验收恢复保存明确说明非事前快照。

下一合同未激活：实际镜像/时序身份与明确ordinary adapter范围先成立，才做同实例一次普通prepare/full completion采证；无新输入或普通覆盖就归档，不扩场景/压力或猜计算。具体translation残余成立才新G0，G0支持后G1。root＋两独占Sol/ultra席，fork none；无DeepSeek/Claude/Astra/DUT/build/GPU/板连接/真实分配/map/生产改动。新主会话及先前核验子席500k/475k已验实，旧窗口历史不改。本轮结束不代表论文完成。

[报告](iommu-platform/docs/r107-bounded-decision-20261012.md)／[稳定裁决](iommu-platform/out/r107-bounded-decision-20261012/root-result-acceptance.json)／[有限合同](iommu-platform/out/r107-bounded-decision-20261012/instance/finite-contract.md)／[原始数据](iommu-platform/out/r107-bounded-decision-20261012/instance/raw-manifest.json)／[普通成本](iommu-platform/out/r107-bounded-decision-20261012/platform/cost-inventory.json)／[账本](iommu-platform/out/r107-bounded-decision-20261012/execution-ledger.json)／[下一合同](iommu-platform/out/r107-bounded-decision-20261012/next-goal-contract.json)。

## 2026-10-12 Linux修复与贡献资格只读整理

按用户问“本地Linux修了什么、能否贡献”，核xvhaocheng提交与差异：3b47包含IOVA lookup错误PAGE_MASK/真实叶偏移修复和CVA6缓存workaround两独立项；affb包含HPM索引宏参数缺括号修复；031为短copy错源校验、9ad为Solo零页验证兼容、585为可选fullcopy统计损坏拒绝。优先上游候选为IOVA和HPM宏，面向malejo97/linux旧驱动需再核当前分支，不能直接声称Linux主线bug。CVA6 FENCE.I Dcache副作用不作为通用DMA方案；实验内部bug归类保持。

DeepSeek一次flash/high/default393216、26904 provider tokens，完整答复存档；worker未收到iommu-bits.h宏diff只读到HPMsession新增，Codex补核原header diff并纠正分类。主线当前RISC-V driver使用generic_pt接口、未有旧lookup函数；仅确认实现不同，不作全主线无bug证明。官方patch提交流程要求面向维护者/邮件review，不等同GitHubPR即成为主线贡献者。无新代码/测试/commit/投稿/发送。[裁决](iommu-platform/out/upstream-iommu-fixes-20261012/linux-fix-acceptance.json)／[完整提交证据](iommu-platform/out/upstream-iommu-fixes-20261012/linux-fix-evidence.txt)／[HPM宏补核](iommu-platform/out/upstream-iommu-fixes-20261012/linux-hpm-macro-fix-evidence.txt)。研究stage107不变。

## 2026-10-12 上游PR对当前平台的只读适用性审查

用户已网页创建PTW PR51／DS PR52，GitHub核实均xvhaocheng、mc/dti-ats、1提交/5文件且open，并已附加本chat；此前403仅是connector创建限制，原提交历史保留。用户进一步要求判断其他PR是否需修到平台，本轮只分析未实施。

定向取PR42/43/45–49当前body及实际RTLdiff，DeepSeek一次flash/high/default393216，341383 provider tokens、无截断，完整证据/答复落out。Codex直接核TC、MRIF handler、CQ、DDTP regmap、banked counters及Linux/MSI配置，核AIA v1.0 7.1.3.2与IOMMU v1.0 4.1。默认源码mode0/MRIF能力开；非阻塞mode1..3覆写basic-only；当前Linux MSI页表填M3而非MRIF，不能由源码推出liveBIT。

接受：47忽略PR/PW及CQH提前、49仅翻译state等待且缺桥级全局排序，需要平台合同适配作为可靠性待办；当前实验先quiesce且Linux不置PR/PW，不能说已有实验受此破坏。46MRIF缺AW size/len/WSTRB等过滤，48IE=0抑制notice违反AIA当前强制语义，实际使用MRIF前修；当前Linux基本翻译不触发。45旧main即时W竞态不能直接映射当前descriptor/W FIFO，需延迟/连续MRIF写小回归再定。42仅内部缓存reserved删除优化，worker“必然破坏Linux ABI”错误已拒，存储DC64B/DW7检查可保持；43GIPC功能扩展当前无需，未据旧PRbody宣称当下规范仍未ratify。45/46/48堆叠diff需拆语义，不能逐个套整个补丁。

无RTL/Linux/配置/PR改动、新lint/仿真/FPGA/板测或研究G0/G1。[稳定裁决](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-acceptance.json)／[原始PRdiff](iommu-platform/out/upstream-iommu-fixes-20261012/platform-pr-evidence.json)／[完整worker答复](iommu-platform/out/upstream-iommu-fixes-20261012/deepseek-platform-pr-review.json)。

## 2026-10-12 用户DS／PTW修复上游贡献：fork已推送，PR创建权限受限

用户明确截图两修复由自己修改并要求提交到合适地方。连接身份实际xvhaocheng、已有fork xvhaocheng/riscv-iommu；上游zero-day-labs/riscv-iommu默认main为旧四源DS/不同PTW，准确受影响实现位于mc/dti-ats，当前c80bba7。独立clone/worktree各仅应用DS或PTW原修复；原研究树及57eb562边界修复不打入本次PR。主仓库邀请bug贡献，无额外本仓库CONTRIBUTING/AGENTS。

DeepSeek一次flash/high/default393216、90235 provider tokens、无截断，移植现有测试入口及删除PTW wrapper研究专用e3_level口；Codex关键源码核、应用原修复和测试、实际纯上游before/after。DS旧FQ场景AW1/W0/B0 watchdog失败，修复4/4；PTW旧合法4KiB读写cause13/15失败，修复7/7（两阶段15AR/15R），测试/日志完整保存。依赖按上游gitlink本地clone为独立子模块，不改原vendor。

author/committer均xvhaocheng <xvhaocheng@gmail.com>，DS `7eb751798b1005a40945751b46607989b6bf27e9`／fix/ds-write-owner，PTW `078572baf4855303bcfd8f1931611fa5017a3f83`／fix/ptw-leaf-level。SSH真实认证并已推送两个新分支到用户fork。调用GitHub connector向上游创建DS PR返回403 Resource not accessible by integration，未创建任何PR；未再重复同权限调用PTW。本机无GH_TOKEN/GITHUB_TOKEN或标准gh/hub/netrc/credential文件，gh未安装。两个完整PR正文/预填compare链接存submission.json；需要用户在网页完成Create pull request或修复连接权限后继续，不能称已成为上游贡献者/已合并。无自动发issue、无改权限或其他发布。[完整提交状态与链接](iommu-platform/out/upstream-iommu-fixes-20261012/submission.json)。

## 2026-10-12 IOMMU AXI 4KiB边界修复并提交

用户提供朋友wiki后要求检查当前代码，有则修复commit。当前InclAxiBC=1；Codex直接核rv_iommu_axi_bc与TC错误路径及Arm IHI0022H A3.4.1/A3.4.2，确认FIXED用exclusive-end误拒页尾，INCR/WRAP按size-1半长度算且WRAP两3bit指数相加回绕。修复按aligned beat window的inclusive-end、17bit完整burst长度计算，WRAP用宽mask且拒非beat对齐。IOMMU仓库提交 `57eb5621802ef9ed8d77c684b9a03205c6630391`，author/committer xvhaocheng；只含checker与sim/axi_bc四个测试/说明文件，其余已有dirty代码原样留存。

DeepSeek两次flash/high/默认393216，完整答复保存在验收目录，provider合计108757 tokens；worker起草测试及一次runner定向修订，Codex主导RTL/关键核查、审查应用及实际执行。Verilator4.038：旧版816064检查112514失败；初修152失败定位WRAP指数回绕，最终816064/816064通过，五定向全过，旧定向fixed-tail及incr-cross-half失败。size0旧定向在本工具通过，不沿用worker“必然拒绝”推断。测试按逐拍地址oracle，覆盖所有页内offset/size0..7及选定八LEN，非全AXI协议/全LEN或完整DMA验证。手工lint首次-G类型参数入口报错，后真实64bit wrapper构建通过，vendor告警保留可见。

无新FPGA完整构建／时序／上板／性能结果；CVA6与平台子模块指针及sources.lock未改，若后续锁定构建需另登记新组件链。研究stage107与R107下一合同不变。[验收及完整证据](iommu-platform/out/iommu-axi-bc-fix-20261012/acceptance.json)；原旧版、初修失败、最终build/run与两份DeepSeek答复均保留。

## 2026-10-12 Codex树模型／上下文执行规则调整（不更换研究阶段）

按用户要求将主会话和后续Codex子席默认设为gpt-6.1-sol／ultra，保留推理强度；设计、分析和审查也由Sol承担。Astra仅用于具体难机制冲突、架构取舍、根因或已证实Sol限制的有界任务，调用前说明、完成后把原始证据与裁决交回Sol，不设例行复审。全局与工作区AGENTS已持久化任务上下文、完整本地产物和可导航交接规则；DeepSeek／Claude原约定与配置保留，本研究仍Codex-only。

移除model_context_window、model_auto_compact_token_limit、model_catalog_json三个旧覆盖，旧目录文件保留为非活动历史。支持的config接口重新载入并核默认值；root线程设置应用成功，当前实际也是Sol／ultra。完整存档树18席（root＋17直属席），历史15Sol／3Astra，最近强度均ultra；14旧席未加载，3个v2子席拒绝直接app-server输入，未强行修改数据库／日志、恢复或重跑它们。两现存研究席收到不启动新turn的规则通知。当前turn设置接口另要求未启用step_model_switching；未启用该特性，root实际已符合模型要求。

500,000原生默认／475,000有效窗口没有通过本轮验收：当前原生目录context_window=272,000、max_context_window=872,000，最新token_count有效258,400。移除覆盖不等于扩大运行窗口，未手工改缓存或用API最大窗口冒充Codex默认，也未重启会话。模型选择与规则已接受，目标窗口及全旧树热切换明确保留限制。

17项配置／保护核查通过：六个研究／计划输入、原有DeepSeek／Claude配置与说明、全部18份rollout既有前缀保持；旧当前状态、交接与验证说明先快照。研究仍stage107／R107条件保留，下一关口仍是有限KV恢复任务／所有权／依赖合同及普通准备路径；无新goal、研究实验、构建或平台变更。本批root＋一个独占只读Sol／ultra清单席，fork none，无Astra、DeepSeek或Claude调用。

[稳定验收](iommu-platform/out/codex-model-context-policy-20261012/root-result-acceptance.json)／[运行窗口证据](iommu-platform/out/codex-model-context-policy-20261012/runtime-context-evidence.json)／[完整树](iommu-platform/out/codex-model-context-policy-20261012/tree/tree-inventory.json)／[配置与历史保护核查](iommu-platform/out/codex-model-context-policy-20261012/preservation-verification.json)／[接口结果与执行账本](iommu-platform/out/codex-model-context-policy-20261012/execution-ledger.json)／[执行前快照](iommu-platform/out/codex-model-context-policy-20261012/before-manifest.json)。

## 2026-10-12 stage107 平台大页／合并约束与AI恢复依赖

当前第一阶段，完成stage106普通G0后的约束复核和具体重设计；R107条件保留，无新G0/G1。主平台仍Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux，GPU／AI仅优先，服务器不替代实验平台。

现存CMA=n、MAX_ORDER10下整槽10MiB FORCE_CONTIGUOUS需order12，generic路线超限；普通四槽各五个独立2MiB extent可映连续IOVA，不要求整槽物理连续。二十叶静态见证保留原十二个整张量descriptor和18,895,872B；对齐／RAM几何／全字节／全叶owner／不alias通过。全部PA未分配，40MiB块容量、78MiB含空隙示例跨度与原48MiB dense模型backing区别明确；SG成功不保证大叶。实际allocation、map/cache/drain成本未测。

固定PagedAttention v1和vLLM v0.1.0两primary、十四提取文件：整池预分配、logical free/refcount不撤映射；恢复完整唯一KV和layer use有源，现有顺序/event承担安全。src_to_dst在当前issue前可见；同一期不得双向swap，单序列默认重算、保守预留启发式不证明容量强迫。GPU head分片不证明当前两DID同共享路径；源码需求、可控制位置、普通失效、新颖性及收益分别裁决。

root直接核24项关键源／配置，指出mode0 buffer3与私有512差、sourceRO和Linux helper READ|WRITE＋自动映射别名差、CVA6 FENCE.I交接与实际所有者／单DMA授权成本。平台27源和90段精确引文、AI41行号、静态布局一次通过；AI语法错误和root行号记录超界各一次纠正均记账。49旧输入、stage106私有修复与八案原始结果不变，生产未合入。全Codex，无新DUT/build/GPU/板连接/真实分配或生产修改。

下一仅落实R107一份有限任务／所有权／生命周期合同及普通准备路径，明确原生来源和研究回放、完整集合／消费者／自然结束、实际资源和全部成本；保留大页／预映射／批量／驻留／授权合并。缺具体实例或合法控制点就归档，不增加第三场景或猜计算；有可检验残余翻译损失才登记最小G0，随后才进G1。

[报告](iommu-platform/docs/ai-platform-constraints-20261012.md)／[root裁决](iommu-platform/out/ai-platform-constraints-20261012/root-result-acceptance.json)／[R107卡](iommu-platform/out/ai-platform-constraints-20261012/critical/redesign-card.md)／[普通准备合同](iommu-platform/out/ai-platform-constraints-20261012/critical/ordinary-preparation-contract.md)／[下一goal](iommu-platform/out/ai-platform-constraints-20261012/next-goal-contract.json)／[账本](iommu-platform/out/ai-platform-constraints-20261012/execution-ledger.json)。

## 2026-10-12 stage106 AI独立必需任务的普通G0与工程纠错

固定FlexLLMGen两rank层0／层6的不同MLP对象，各六张量9,447,936B；冷目标共同ready0为抽取的提前准备，非原GPU自然同时运行或共享IOMMU证明。真实两个iDMA RTL路径接受完整对象，可信单iDMA可合法准备两包；不强迫可选中转，不把流当DID。

driver reset DONE0误设按实际源改为1；七个旧整张量配置失败，经专用观测定位写creditC8低三位q0／overflow1被当空。私有wrap仅两处W非空谓词包含高位，原AW门控／宽度／容量／时序不变。修正后原八案全部真实数据、权限、NEXT/DONE、字节／B／TL及排空通过；全程C≤8、terminal0，六案满计数下降通过。MRIF／生产合入／实板不作通用验收。原失败保留，不能算普通方法科学阴性或创新收益；4KiB分段只一独跑功能验收，不扩矩阵。

合法2MiB并发完成2,377,027，4KiB为5,365,690（−55.6995%），双包18,895,872B对应单64bit R口下界2,361,984，makespan剩余上界0.63285%。2MiB普通单DMA先完成角色0为1,188,896，两包2,377,298，仅晚271；首包时另一包尚无完成张量，取舍未隐藏。模型DS50/50、DDR全读latency1及直接cfg保留；共享差额不全部归因PTW，表项host初始化／板端分配/cache/供给成本明确未测。

当前整体第一阶段D4—5结束，本版all-ready连续冷准备不进G1。下一关口定向核实际大页／完整准备成本与任务合并／隔离约束，优先有来源AI页块生命周期或依赖；只有具体翻译相关可避免损失超过准确普通对照与成本才设计新规则，不重跑已验矩阵或加压力。六次私有构建、20DUT：10完成、10拒绝／诊断失败；原whole和分段各五记录负例均拒，八案suite一次。全Codex，零生产改动／GPU推理／FPGA构建／板测；前五文档、旧源／脏树／原始数据保留。

[报告](iommu-platform/docs/ai-mandatory-work-g0-results-20261012.md)／[根裁决](iommu-platform/out/ai-mandatory-work-g0-20261012/root-result-acceptance.json)／[八案验收](iommu-platform/out/ai-mandatory-work-g0-20261012/validator/credit-fixed-suite-acceptance.json)／[账本](iommu-platform/out/ai-mandatory-work-g0-20261012/execution-ledger.json)。

## 2026-10-12 stage105 GPU／AI任务到现有FPGA的适配功能关

本轮goal接受固定FlexLLMGen源OPT125m一层MLP六个FP16权重张量：9,447,936B有效对象、2,309张量内4KiB分段。来源第一跳是映射文件到中转的CPU拷贝，非已观察SSD DMA；整张量填满后才能拷出，源码队列/CUDA同步保留为来源语义。默认四worker的8GiB中转不能免费移植。新增同提交opt_config/utils/timer三文件，不猜计算delay或应用期限。

生产四iDMA共享IOMMU路径、原始寄存器与Linux任意张量接口缺口、全局FENCE.I/cache交接已亲核。私有诊断以DID10填充代理、DID11拷贝代理执行有限准备合同：不是GPU／SSD，也不把CUDA流叫设备。行为DDR模型不含实际CPU/cache/MIG或板端时钟；没有新增生产RTL／ABI或板端应用接口。

新独立Verilator构建一次、冻结106项输入；实际两跳4,618笔和驻留源一跳2,309笔均自然完成。根直接读原始issue/done、整张量依赖及全六张量数据，独立记录验收通过；提前拷出、缺done、重复done、改数据四变异均被拒，原始记录未改。R/W/TL计数来自运行时观测，非独立逐拍重放。种子串行，quiet周期不是AI或FPGA性能比较。载荷预留48MiB，页表212,992B、DDT4KiB；实际板端内存分配、CPU供给/cache交接成本与时序未验。

根裁决：功能资产可复用，受控搬运G0技术可执行；但当前源已在DMA可访问DDR，普通直接一跳完成同一对象，不能靠强制中转支撑AI双设备保护收益。当前可选中转分支不进入方法性能G0，无普通失效或G1，不扩矩阵。下一固定有来源、独立用途且双方均须完成的AI搬运任务，再核平台合法投影、映射和完整成本；成立后才登记普通基线G0，不要求完整GPU推理。整体仍阶段一，方法论文未完成。

[报告](iommu-platform/docs/gpu-ai-fpga-fit-20261012.md)／[稳定裁决](iommu-platform/out/gpu-ai-fpga-fit-20261012/root-result-acceptance.json)／[任务合同](iommu-platform/out/gpu-ai-fpga-fit-20261012/transfer-contract.json)／[普通规则和入口](iommu-platform/out/gpu-ai-fpga-fit-20261012/g0-entry-contract.json)／[账本](iommu-platform/out/gpu-ai-fpga-fit-20261012/execution-ledger.json)。本批全Codex，三个新增席gpt-6.1-sol/ultra；无DeepSeek／Claude、GPU推理、FPGA构建或板测。执行服务器不变成实验平台，旧源／原始数据／脏树及五文档执行前快照保留。

## 2026-10-12 stage104实验平台选型纠偏

用户指出本机只是服务器，不是实验平台。root承认此前用本机现成GPU/NVMe便于补共享路径证据，错误地替代了既定FPGA适配关口。GPU/AI优先不意味着换平台。现有Genesys2/CVA6＋四iDMA＋RISC-V IOMMU＋Linux仍为主实验平台；仿真辅助，关键验证回板。

原stage104九primary、本机只读硬件事实及AI源码生命周期保留为参考，不证明FPGA场景入口。撤销本机安装/下载OPT6.7B/跑disk100与CPU100作为下一主线动作；原root裁决和pre-G0合同逐字快照后标记历史失效。当前恢复到GPU/AI来源与既有平台能力/控制/预算的适配判断：明确真实执行、回放和未支持部分，计完整成本和普通对照，不猜GPU时序或把iDMA叫GPU。没有可执行AI平台合同、性能G0或G1；未安装、下载、运行AI或切换硬件，所以原实验数据和生产实现未受影响。

[纠偏裁决](iommu-platform/out/platform-scope-correction-20261012/scope-correction.json)／[带纠偏说明的报告](iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)／[原当前状态](iommu-platform/out/platform-scope-correction-20261012/before/CURRENT_STATUS.md)。仅Codex，无新agent/外部模型、构建或实验。

## 2026-10-12 GPU／AI优先的实际部署与前置采集合同 stage104

用户要求设置goal继续，GPU／AI仅优先不排他。root实际只读取证确认T4004GB、NVMe和NIC同dmar1，各DMA-FQ；Intel iGPU在dmar0；所选私有文件目录实际由该NVMe承载。弥补stage103具体部署共同路径缺证，但不等于已经观察cache/PTW争用。实际Linux6.8.0-60与v6.8上游参考分开，同版NVIDIA公开代码与loaded proprietary二进制分开；ATS未知，nonOpen不满足所核HMM前提，P2P限制不否定CPUstaged pinnedcopy。

两必要复用Codex席独占GPUhost路径、AI任务，根亲核源码／原文／控制／成本及根条件设计；九新primary全部归档。选定固定作者FlexLLMGen源OPT6.7B finite seed：batch1、请求prompt32、完整2token，两功能profile disk100/CPU100，保留source warmup/init/排空，明确缩尺不是论文性能复现。128GBCPU可驻全权重和自然pagecache是合法强对照；不把disk标签当NVMe流量。四relay pin为8*GB条件量，实际GB/config/峰值待测；不造deadline/后台或为了SSD限制RAM。

稳定裁决进入前置功能／事件采集准备，实际应用pending，未冻结性能矩阵或接受G1。下一批需固定隔离软件、真权重/tokenizer/config和实际输入、核资源并添加最小私有任务/字节/ownership/时钟/完成记录；两配置各一次功能采集后，以真实NVMe与GPU DMA重叠及普通残余决定性能G0。无NVMe/CPU驻留覆盖则关闭该seed的稳态SSD收益假设，保留AI功能资产，不扩大模型维持压力。内核观测不可访问如实记录，一次PermissionError恢复未改权限。

本批全Codex，无DeepSeek/Claude，零安装、模型下载、AI/CUDA/性能模型/构建/实板或生产变更。worker两次静态验收、root最终来源/文档验收只验材料；旧stage98—103/源/原始数据/脏树保留。整体仍阶段一，方法论文未完成。

[报告](iommu-platform/docs/gpu-ai-deployment-gate-20261012.md)／[根裁决](iommu-platform/out/gpu-ai-deployment-gate-20261012/root-result-acceptance.json)／[下批合同](iommu-platform/out/gpu-ai-deployment-gate-20261012/pre-g0-contract.json)／[实际硬件](iommu-platform/out/gpu-ai-deployment-gate-20261012/local/hardware-inventory.json)／[AI卡](iommu-platform/out/gpu-ai-deployment-gate-20261012/ai_workflow/demand-card.json)／[原文阅读覆盖](iommu-platform/out/gpu-ai-deployment-gate-20261012/root-source-read-coverage.json)／[账本](iommu-platform/out/gpu-ai-deployment-gate-20261012/execution-ledger.json)。

## 2026-10-12 新真实多DMA两场景预筛结束 stage103

用户选择重新筛真实并发场景并批准计划，创建goal执行；允许调整旧A/B入口，仍保留共享IOMMU主线。Codex根亲核关键原文/源/控制/成本，两复用Codex席独占网络存储与周期采集；各三项新primary到限，总六项，未调用DeepSeek/Claude，零构建/模型/应用运行/FPGA/生产改动。

网络侧固定SPDKv24.09/iperf3.17.1：真实独立程序、PCIe请求/回调补发、buffer复用、有限退出/排空已核。共跑是研究组合；缺实际NIC/SSD translated mode/Requester/domain/共同remapping-unit与buffer/arrival/完成成本。group不证明cache/PTW共享、随机LBA不等于新IOVA、cpl含错误不能全算成功；elapsed排空前而print排空后，固定窗需单独成功事件。F&S远程读链不作独立后台，严格映射失效不免费套静态模型。

采集侧官方SDI入口＋固定YAVTA＋2025.1baseDTS：正常10DQBUF只后3允许保存，buffer数量/stride/bytesused及实际周期需返回记录；QBUF/消费/重排/停流边界已核。DTS默认SMMUdisabled/GEM连接注释，未与历史demo运行image匹配；wikirevision未知，保存工具全文快照而非冒称原HTML。独立有限文件传输未确定。DDR环不必放R2 96KiB SPM，不以名义大小作否决。

根裁决接受两张来源卡但两项未过可执行G0，无选定场景、矩阵或G1；需求/路径未闭合不是科学阴性。保留两个明确条件假设及反例，普通方法适用与真实成本待测。没有deadline也可完成/有效进度，研究组合可明示；结束理由是实际共同路径/完整事件和适配闭合不足。两材料失败恢复均入账：网络旧路径404、wiki202空体/REST404后官方工具全文。旧stage98—102/源/数据/脏树保留，不能把方法论文写成完成。

下一关口仅在选定部署实例提供共享路径和独立任务buffer/arrival/有效completion/mapping事实后重开；不继续同类检索凑第三个场景，不扫描背景强度或恢复旧gate。

[报告](iommu-platform/docs/real-dma-scenario-screen-20261012.md)／[根裁决](iommu-platform/out/real-dma-scenario-screen-20261012/root-result-acceptance.json)／[网络卡](iommu-platform/out/real-dma-scenario-screen-20261012/network_storage/demand-card.json)／[采集卡](iommu-platform/out/real-dma-scenario-screen-20261012/periodic_capture/demand-card.json)／[根亲核](iommu-platform/out/real-dma-scenario-screen-20261012/root-critical-source-review.json)／[根假设](iommu-platform/out/real-dma-scenario-screen-20261012/root-hypotheses.json)。

## 2026-10-12 B真实后台需求有界补证结束 stage102

用户批准优先真实后台需求方案并要求执行，创建/goal后全程Codex根完成。仅新增固定HERO应用README与suite testset两项直接材料，SHA256/Git blob对应既有固定commit；没有agents、DeepSeek/Claude、新构建、模型、旧validator或实板。

README提供构建及计算线程/DMA/offload选项，suite列四子测试而未给共跑合同；未读取调度器，不能推出一律串行。既有mm-large第351—405行明确逻辑device0/1是一物理加速器、同时使用不安全；A/B/C属于一个矩阵结果，不能转作独立背景。child timeout是测试限时且单位未知，不作应用deadline。原SP单工作流也未提供独立后台合同。

需求卡逐项来源事实/实验设定/未知：独立源后台身份、到达、有限工作、自然结束、目标和共享路径均未建立；本轮取证结束，需求未决，不算科学阴性。保留原8MiB受控微基准及stage100/101裁决；没有deadline不是否定理由，共同启动等研究设定可合法明示。

未登记实际计算时序构建或G1。下一关口需具体独立任务及需求/记录新事实，随后才规划同时间基测量；不重读同材料、扩大背景或凑新应用。方法论文未完成，源与旧数据保持。

[报告](iommu-platform/docs/workflow-stream-background-demand-20261012.md)／[需求卡](iommu-platform/out/workflow-stream-background-demand-20261012/background-demand-card.json)／[根裁决](iommu-platform/out/workflow-stream-background-demand-20261012/root-result-acceptance.json)／[来源清单](iommu-platform/out/workflow-stream-background-demand-20261012/source-manifest.json)／[核证](iommu-platform/out/workflow-stream-background-demand-20261012/source-evidence.json)／[账本](iommu-platform/out/workflow-stream-background-demand-20261012/execution-ledger.json)。

## 2026-10-12 真实B平台条件复核及本轮立题结束 stage101

用户要求继续goal、仅Codex模型不限。根保留关键原文/架构/路线判断，两个必要席分别核固定平台事实和原始工作流界限；没有新构建/模型/旧validator/FPGA。官方14文件、两PDF、两固定树元数据及实际检索/哈希落盘；八个关键旧输入和16份新原始材料一致。stage100源/数据/裁决保持。

新勘误：原SP PDF第8页Fig5题头明确16KiB/four pages，旧预筛“没有块字节数”遗漏；原SP kernel/time仍缺，现HERO六槽不是原SP复现。官方每cluster8RV32/16×32bit单端口bank/128KiB，当前单CVA6配置50MHz不等于八核时序；unchanged multiplier每拍ready+结果注册，R2六步只是控制选择。R2总34209792grant理论下界不能当实测速度。

既有四案255次后续计算交接均一周期握手，真实前驱均提前就绪，无输入/输出槽气泡。4k差严格6006=5988首命令+18累计tile时长；startup62=45+17，末C尾段同5023。4k BG q7内结束、C0尚未ready。startup让BG_final提前1191但首issue/done延后13549；源应用没有独立BG生命周期/首笔期限，原有限受控负载合法且保留，不能冒称原生trace或补无来源deadline。

根裁决：新事实支持资源能力/普通流水可能性，不支持真实更快应用吞吐或新的G0具体失效；按计划结束本轮两工作流有限立题筛选，无G1/R3/方法原型，方法论文未完成。重开必须有实际同钟计算/搬运、独立BG需求和落关键路径的普通未覆盖损失，不自动扩背景/扫强度/换题凑候选。原八PE或领域没有被否定。

2026-09-18具体E3旧routed报告20ns路径slack−6.470ns，未匹配至已验收boardimage，不撤销E3局部因果或断言FPGA整体不可用；仅不借此宣称新B50MHz时序闭合。全部Codex，未调用DeepSeek/Claude。

[报告](iommu-platform/docs/workflow-stream-platform-review-20261012.md)／[根裁决](iommu-platform/out/workflow-stream-platform-review-20261012/root-result-acceptance.json)／[平台事实](iommu-platform/out/workflow-stream-platform-review-20261012/platform-source-facts.json)／[界限](iommu-platform/out/workflow-stream-platform-review-20261012/workflow-bounds-review.json)／[重开条件](iommu-platform/out/workflow-stream-platform-review-20261012/reopen-conditions.json)／[账本](iommu-platform/out/workflow-stream-platform-review-20261012/execution-ledger.json)。

## 2026-10-12 B同钟适配与固定R2 G0归档 stage100

用户要求按计划继续设置goal，全Codex、本轮模型不限。落实预先冻结的R2平台投影：256² uint32源合同、六槽96KiB、单总读写端口/一周期响应、实际CVA6乘法器，计算、iDMA/IOMMU/行为DDR同clock；供给和全部尾段计时。性能288×16KiB、256tile、16,777,216MAC；固定后台2048×4KiB，不扩大压力。原八PE/OpenMP/CVA6软件及FPGA时延没有复现。

四案模型和独立验收各一次：solo4k V100813737；shared4k V100819743/BG3132054；普通首块准备V100813799/BG3130863；shared2m V100806133/BG1288361。三个shared后台在V完成时全部8MiB完成。普通准备恢复5944/6006周期（98.97%），余差62，后台还提前1191；不独占归PTW，不用未匹配2m独跑证明共享收益。归档固定R2差额需要新门控的分支，无G1/留出/方法原型，整体方法论文未完成。

全65536输出、槽代次A/C0—7及B0—127、DAG/AXI/TL/quiet独立接受；完整逐MAC/RAM靠已核实际driver，稠密微迹独立逐拍回放并拒绝六负例。SPM端点强制R/B返回暂停和越界无别名通过；SYSTEM DDR B反压未覆盖。4KiB授权992KiB/2MiB6MiB；96KiB实际RAM和单端口/乘法器明示，未综合/上板。

适配首轮暴露原TC可超配置四AW额度至八、3位W计数归零挂起。根直接核源码/生成状态，私有同预算额度修正供全部四案共用，真实16KiB copy保持；原生产未改，不把工程bug当科学阴性或新方法。实际SYSTEM Verilator5调用（2成功3失败），三次诊断C++增量编译；SYSTEM模型9次（4故障采证＋稠密1＋完整4），端点构建/运行各1；独立核验5次。完整实际账本和失败日志保留。

107构建输入匹配final exe；补当前诊断include/工具hash但不追认历史缺失。136保护检查/130路径零不符，旧数据/源码/工作树及stage99文档原样快照保留。全Codex，无DeepSeek/Claude；必要席位分同钟计算、接口闭包、独立验收，根承担机制/成本/因果裁决与文档收束。

后续只在新独立来源支持可达残余损失时重设计；不自动扩搜索或恢复旧候选。当前整体阶段一D4—5已结束该分支，论文主贡献未立。

[本轮报告](iommu-platform/docs/workflow-stream-g0-results-20261012.md)／[根裁决](iommu-platform/out/workflow-stream-g0-20261012/root-result-acceptance.json)／[独立验收](iommu-platform/out/workflow-stream-g0-20261012/validator/full-acceptance-summary.json)／[冻结](iommu-platform/out/workflow-stream-g0-20261012/source-freeze-final.json)／[账本](iommu-platform/out/workflow-stream-g0-20261012/execution-ledger.json)。

## 2026-10-12 B应用合同与计时缺口 stage99

用户要求设下一步goal并继续原计划，全Codex。B采证取得PULP官方HERO固定commit内核，256² uint32、16行条带、A/B/C各双缓冲共96KiB，A16+B256+C16=288个16KiB搬运，256tile、16,777,216源码乘加。不是原2018 SP复现；active copy-based，SVM关闭。

根直接核完整原函数／API与SYSTEM/CVA6边界，私有程序仅加两观察块、malloc和同步memcpy shim；删除观察块逐字还原原函数。实际功能编译／运行各1次；全部65,536输出零错。独立核验1次重建1,390事件、完整内积参考、每copy issue/done/wait和六槽代次／最终释放；三个内存损坏例拒绝。512新增屏障与同步CPU拷贝明确，host_ns不冒称targetcycles。另有2个RISC-V编译接口检查和历史binary版本探测，不算应用时延。

根裁决：来源／功能／依赖关通过，缺口收敛为同clock计算、96KiB有限端口存储及真实DMA/供给预算；现SYSTEM为synthetic CPU且无SPM。忠实8PE复现和明确单计算器模型的成本分别登记，后者合法但不是现成平台／原8PE时延。当前不进入B性能G0、G1或方法原型，不科学否定B。A期限假设阴性、stage97 PN分支和所有历史证据保持。

两个新增gpt-6.1-sol/ultra席加既有同配置独立验收席；全Codex，无DeepSeek/Claude。原测试／生产RTL／Linux／ABI未改，无新Verilator、DMA性能模型、FPGA构建或上板。136哈希检查覆盖130不同路径零不符；旧状态和被改文档原样快照。方法论文总体目标尚未完成。

[报告](iommu-platform/docs/workflow-stream-contract-20261012.md)／[根裁决](iommu-platform/out/workflow-stream-contract-20261012/root-result-acceptance.json)／[独立验收](iommu-platform/out/workflow-stream-contract-20261012/validator/acceptance.json)／[缺口登记](iommu-platform/out/workflow-stream-contract-20261012/timing-gap-register.json)／[执行账本](iommu-platform/out/workflow-stream-contract-20261012/execution-ledger.json)。

## §98（2026-10-11发起／2026-10-12收束）全景计划执行至有界预筛终点

用户确认全景计划并要求设定/goal、全部Codex、新执行席gpt-6.1-sol/ultra。已建立目标，保留用户的方法论文优先和立题通过后现有FPGA验证选择；默认跨任务DeepSeek分工只在本轮被明确例外覆盖。Root亲自研究判断、核原文和实际控制链、设计合同、审查完整私有驱动与独立校验器并执行编译/模型。三个新席分别做历史收束、双缓冲来源/成本绘图、实际接口/驱动；历史席后续做独立burst/任务核验。未调用DeepSeek/Claude。

总计划重写为目标、阶段与关口；stage97 M0已有资产和G0已结束状态分开。保留E2/E3有限因果、P4M60份53在线＋7离线跨段、method88固定A8阴性及旧PN实板边界。没有重跑原矩阵或清理现有工作树；原计划/状态/入口快照保存。B原文支持双缓冲I/O与计算重叠，但没有本平台固定kernel/compute/slot释放和scratchpad预算；缺证延后，不制造A1/A8交替应用阶段。

A选行预取：640B×480、前3实际初始化、477固定期限、DID11独立8MiB/2048COPY自然完成，P全速gap0、CPU关闭、无DS优先/新门控、静态S1。只编译私有C++链接既有SYSTEM模型，副本新增一个pre-edge测量hook；原测试C++、RTL、ABI、Linux未动。93项目输入元数据支持复用，109项受保护哈希前后匹配；非项目Verilator工具元数据变化披露，不冒称历史构建签名。

登记两案均技术与独立验收：4KiB／2MiB迟到0/477，max释放到done3192／2175、min余量280／1297；同0..1656250周期后台完成938／2048笔，A完整done3309615／1401779，最终quiet3309632／1656267。每路全部真实完成、AR/AW精确覆盖、三向字节与AW/B守恒；真实quiet事件3/0/0，源/保护区实际CHECK。七项内存篡改均拒绝。大页最低授权/保留容量6MiB对432KiB，额外5849088B；前三行初始化1919／1629、准备各65周期；reset和未测真实软件/映射/硬件成本另披露。2MiB后台提前自然结束，曲线后段不能当同竞争强度。

根裁决：普通4KiB P已覆盖本期限，归档该“需新增保护”的失效假设；B仍是证据未决。没有通过立题的新规则，不进入消融、留出或FPGA。关闭有限选题不等于主线无空间，不据已有大页收益称创新、不自动转表征论文、不扩大压力搜索。重开须新的来源明确需求/可达未覆盖事实，B先补固定应用合同、事件段和资源预算。

实际新增私有C++编译1、DMA模型进程2、离线两案核验（核验器4进程，7种负测2轮均拒绝）、绘图1；无新Verilator展开/生产修改/FPGA构建/板测。初次同文件delete/add编辑被工具拒绝且无部分变更，之后单次整文件写入成功；仅必要编译/运行/产物验收，不修大夹具。

[本轮报告](iommu-platform/docs/workflow-row-g0-results-20261011.md)／[总计划](研究计划.md)／[具体计划](iommu-platform/docs/research-execution-plan-20261011.md)／[根裁决](iommu-platform/out/research-execution-20261011/root-result-acceptance.json)／[执行账本](iommu-platform/out/research-execution-20261011/execution-ledger.json)／[前状态快照](iommu-platform/out/research-execution-20261011/before/CURRENT_STATUS.md)。

## §97（2026-10-10）M0完成并结束本轮P与PN差额分支

已按用户确认计划实现完整批次测量，保持SYSTEM拓扑，新增默认开启的CPU测试使能。共同t0在双方配置后首次NEXT_ID前；双路真实issue64/done64，独立watchdog，载荷读写各262144字节和128个B响应，最终quiet与数据一致。A/V两种先结束顺序、首发128周期计费、CPU读源开关、旧N回归、watchdog预期失败、14损坏记录拒绝与9参数拒绝通过。10配置2560个事件时间戳与CSV逐项匹配，离线请求/返回和walk区间配对全部通过。

根裁决：A8 FIFO P/PN的T_V为86594/90283，差3689周期（4.26%）；DS优先后双方V均76738、A均76164、T_return均76745。93个DS请求的AR到返回累计延迟P/PN相差3689，干预后均为2976。A1四模式全同。这项差额在当前行为DDR合同中已由普通服务规则消除，结束以此支持新增门控的分支，不进G1、不扩矩阵、不推广到实板或整个共享翻译主问题。N/S工作量不同的差异单独保留，不自动开新题。

实际：仅测试C++/SV/运行器/校验器及离线脚本；两种Verilator构建，16次仿真进程含1次预期失败，10性能配置含1项M0复用；生产RTL/Linux、FPGA完整构建、板测、Claude、Codex子席均无新增。DeepSeek四次提供实现/修订、校验器、离线脚本及文稿；Codex实质设计、关键核源与裁决、审查并修复提案遗漏、应用与执行。离线绘图首次因scripts/platform.py遮蔽标准库失败，改模块方式运行通过，未重跑仿真。

[报告](iommu-platform/docs/workflow-g0-m0-results-20261010.md)／[根裁决](iommu-platform/out/workflow-g0-m0-20261010/root-result-acceptance.json)／[原始结果](iommu-platform/out/workflow-g0-m0-20261010/results.json)／[执行账本](iommu-platform/out/workflow-g0-m0-20261010/execution-ledger.json)／[调用账本](iommu-platform/out/workflow-g0-m0-20261010/worker-ledger.json)。旧当前页原样归档，新当前页同步此前G0/M0文档增订与本轮实际执行。

## 2026-10-10 全景路线审查完成 stage95保持

用户明确允许推翻当前主线，以有价值的独立贡献为核心、不必绑定Chiplet。裁决：原始Remote PTW及多DMA扩展合理；后期机制条件不断扩张、实际问题未固定，最新计划仅部分纠偏。建议G0先检验具体普通方法的有用失效，不默认逐copy状态准入；现有first V issue到last V done并STOP A的模型不直接满足共同就绪64+64，接口尚未适配。原研究计划与生产源码未改，旧候选/实验不自动恢复。

Codex直接核原学位论文、关键源码及前作，并对42个输入文件中的E3四对、E2十条、method88二十四条、PN六条原始记录作数值抽核；保留局部因果、阴性与模型/实板边界。DeepSeek提供规划历史导航、执行证据及报告首稿/修订，Codex排除旧P4M状态、错误对照/百分比/时态与来源链接后保存。两份旧审查正文相同且与旧根记录的阅读范围描述不一致，已如实说明，不冒称首次回原文。

无新性能模型实验、RTL/Linux修改、构建、板测、Claude或新增子agent。入口：[完整报告](iommu-platform/docs/research-panorama-audit-20261010.md)、[根裁决](iommu-platform/out/research-panorama-audit-20261010/root-result-acceptance.json)、[原始抽核](iommu-platform/out/research-panorama-audit-20261010/raw-spotcheck.json)、[执行与调用账本](iommu-platform/out/research-panorama-audit-20261010/worker-ledger.json)。审查前状态已原样归档；本次不递增实验进度编号。

## §96（2026-10-10）研究计划执行前增订

用户要求吸收三项审查后制定全面计划。已在原[研究计划.md](研究计划.md)局部增订，保留核心目的、四类依据、研究地图及已有结果范围。G0改为先提出普通方法的具体失效假设，允许一次有界基线判别；缺口获支持后G1才选择信息/动作，不能先把观察状态与门控当答案。无候选只结束本轮/分支，不否定整个共享翻译主问题。

Codex亲自设计M0共同就绪与完整工作量合同：V/A各issue64/done64、正常无STOP A、首次NEXT_ID前共同t0到各自硬件final_done；显式gap0、首笔等待、双方先结束顺序、后台尾段、独立watchdog与协议收尾均纳入。新M0尚未实现或执行。V当前/A历史/共享状态分别登记来源、时效和可影响请求，现有ADMIT只能放下一copy。更正PolicyEnable分区参数与旧反馈门控的混淆：实际测试端口常量才说明门控未接入。CPU随V活动启停会改变A尾段，机制比较保持SYSTEM拓扑，必要时最小测试开关隔离背景。

计划补齐阶段交付与推进/停止条件、独立普通对照和候选等价实现的区别、完整成本、数据契约、准确前作与消融、冻结留出、最小RTL/板端及论文证据顺序。10工作日和六机制组加两留出为建议上限，不是必须跑满。第一份待交付仍为一页失效卡及对照登记，不声称已发现可避免缺口或新机制收益。

本次DeepSeek定向证据、文稿、定向修订3次deepseek-flash/high，默认393216、均stop未截断，provider tokens257744；Codex实质机制/实验设计、关键源码核对、提案审定、保存与文档检查。旧方案残留的预选门控、强制copy ID与G0进入条件已由根裁决纠正。新性能模型/RTL仿真/生产修改/构建/上板/Claude为0，无新增Codex子席；保留此前独立审查与stage95历史。

[根接受](iommu-platform/out/research-plan-comprehensive-20261010-root/root-result-acceptance.json)／[完整回复与调用账本](iommu-platform/out/research-plan-comprehensive-20261010-root/worker-ledger.json)／[文档检查](iommu-platform/out/research-plan-comprehensive-20261010-root/execution-ledger.json)／[stage95原样归档](iommu-platform/docs/status-stage95-before-comprehensive-plan-20261010-root.md)。

## 2026-10-10 研究计划审查

用户要求审查根目录研究计划.md。本轮结论为总体合理、需三项小澄清：G0允许先用已有基线做有界失效判别再锁定新动作；逐copy状态须定义属于V当前/A历史/共享队列及其可控制的next-copy时刻；现有模型V完成后STOP A且以first V issue计时，不能直接当双方各64全部完成的评价，G1应明确完整完成与共同就绪时间验收。仅审查，不修改计划原文和源码。Codex亲自审查机制/源码/实验口径，DeepSeek一次接口事实提取；无新子席/模型/RTL/构建/上板/Claude。

[审查与证据](iommu-platform/out/research-plan-review-20261010/root-result-acceptance.json)。当前阶段仍95，不把审查编号当实验进展。

## §95（2026-10-10）依据方法 地图 全局审查和核心目的统一计划

用户要求建立研究计划。Codex亲自把主干固定为Remote PTW自身代价→跨DMA共享翻译干扰→改善前台真实完成并保留后台有效进度、计完整成本的方法贡献。先用既有固定目的COPY判别具体翻译放行与实际完成失配；拟定DID10/11各64次4KiB，A源/目的各A8环，具体地址/预热/初态/接口执行前登记。Sobel477项合同仅保留后续应用候选，阶段反馈须真实轨迹与具体错误决策才激活；旧机制不自动重启。

计划四关：G0失效卡和独立对照登记，G1一正一反，G2准确前作/因果/成本与留出，G3立题裁决。10工作日、六机制组加两留出均为建议投入上限，不要求凑满；可信可达差异假设即可进入最小模型，不要求先证明收益。成功允许同V要求下A更多、同A进度下V更快或同服务下成本更低；机制新颖性和性能收益分别裁决。

关键核查：原目标与全局审查、地图范围；直接读旧p1–p6反馈、composition quiet/clear与PolicyEnable默认关闭、完成相对GAP。旧策略源码存在不等于当前profile已接入。复用既有前作核查，不声称本轮重新全面读论文；DeepSeek建议的额外例行证据审计及并列Sobel测量未采纳。无新性能模型/RTL仿真/生产修改/构建/上板/Claude，无新增Codex子席。

已保存根目录[研究计划.md](研究计划.md)，更新README/AGENTS及旧详细计划入口；stage94原样归档。DeepSeek证据、文稿、定向修订3次deepseek-flash/high，默认393216、均stop且未截断，provider tokens164102；Codex实质设计、关键核源、审稿和保存。[根接受](iommu-platform/out/research-main-plan-20261010-root/root-result-acceptance.json)／[完整回复及账本](iommu-platform/out/research-main-plan-20261010-root/worker-ledger.json)。本轮计划完成，第一份失效卡与对照登记为后续任务，未写成已完成实验。

## §94文档补充（2026-10-10）保存用户研究方法

用户要求将四张截图总结作为自己的研究方法保存到文件夹。已写入根目录[研究方法.md](研究方法.md)，包含七步流程、AI辅助边界、工作区应用补充、核心文献表和候选卡模板；README、AGENTS和当前状态加入入口。DeepSeek起草，Codex核对并保存，1次deepseek-flash/high调用，默认393216、stop、未截断，provider tokens6789；[完整回复和账本](iommu-platform/out/research-method-document-20261010-root/worker-ledger.json)。本次为方法文档保存。

## §94补充（2026-10-10）本对话的固定工作流与贡献筛选假设

用户要求固定谁搬什么/何时需结果/真实约束、找简单方法缺口、形成可推翻主张。Codex选择DASH §2.2行预取作为固定目的COPY的需求实例：640×480/30fps、640B行、4槽，前台DID10与本研究新增独立后台DID11共享IOMMU。地址和期限全部提前可知；只限制目的载荷覆盖，不禁止合法预翻译或预读，预读暂存公平计价。初始化、最终消费与整帧FPS分开。

根判断：工作流已具体化，贡献句只获筛选假设资格；强简单方法的实际残余缺口仍未测。旧PN相对P慢12.85%不能迁移成新工作流空间；合法2MiB源＋4KiB目的仅需两个S1叶映射，可能消除候选假设。对照规则包括固定保护/额度、DASH式期限入口控制与整任务保护，未声称三个行调度器已实现。维持不进入新硬件方法。

执行：DeepSeek证据、脚本与定向修订共3次，Codex亲自研究设计、原文/源码核验、代码审查和执行；另1个关键Codex席。477任务合同10项内部检查，生成/有效记录接受/过早写入拒绝/逾期拒绝4调用符合预期。人造一周期完成记录只测试检查器；新性能模型/生产修改/构建/上板/Claude为0。provider tokens158436，完整回复与账本独立保存，不混入同期收敛任务。

[报告](iommu-platform/docs/workflow-framing-20261010-root.md)／[根接受](iommu-platform/out/workflow-framing-20261010-root/root-result-acceptance.json)／[执行账本](iommu-platform/out/workflow-framing-20261010-root/execution-ledger.json)／[worker账本](iommu-platform/out/workflow-framing-20261010-root/worker-ledger.json)。保存时保留同期stage94主计划，本补充为其固定目的COPY下的一个需求实例，不覆盖另一任务的裁决或执行记录。

## §94（2026-10-10）依据截图方法与研究地图做计划和收敛

用户要求做计划、做收敛。本轮选定五篇核心前作，区分作者覆盖、未测范围与本项目假设；主线只给“原固定目的 COPY 下翻译放行与完成进度失配”一次便宜的判别机会，备选为有真实阶段证据的反馈失配，关闭其他方向自动扩展。不是宣布新机制成立。主线若没有具体失效卡与独立强对照差异就不入模型，不以解释完 PN 板上12.7%为强制工程目标。

直接源码核实旧p1–p5的V进度反馈、p6 gap反馈，以及模式配置的quiet/clear成本；核合成CPU完成驱动供给和模型DS返回优先的语义。独立席倾向先B，root因A已有现象而仅给一次短关口，接受其全部反馈/切换成本要求；不存在以模型投票认定创新。计划最多六组决定性场景加两组留出，先固定独立策略与校准规则，不用分案例事后最优冒充在线控制。最多两次针对新证据的设计修订，整体建议十工作日上限，可提前停止。

入口：[研究收敛计划](/home/xvhaocheng/work/iommu-platform/docs/research-convergence-plan-20261010.md)、[root裁决](/home/xvhaocheng/work/iommu-platform/out/research-convergence-20261010/root-result-acceptance.json)。本轮新模型/RTL/Linux/编译/上板/Claude均0；Codex root负责实质设计和关键核查，DeepSeek一次证据整理，一个Codex关键审查席。计划后续执行未启动，stage93含10月10日补充已原样归档。

## §93（2026-10-10补充）本次路线审查的证据与范围

Codex本次定向审查9月8日至stage92的目标、E2/E3/method88、PN及近期机制；更早毕业论文只依已有总览，不宣称本次重读其原稿。一个关键席核近期前作否定边界。结论与同题stage93一致：保留问题域和平台，纠正为局部机制不断补条件的推进方式；暂停保持。六份既有PN原始record复算窗口退化12.6744%至12.7922%，不是新板测。新模型、RTL仿真、RTL/Linux修改、构建、板测与Claude均0。

本次DeepSeek为历史提取和定向勘误，两次deepseek-flash/high、默认393216、stop、未截断，合计306200 provider tokens。首稿的P4M旧状态、在途加入旧未实现状态及HPM范围泛化不采纳；回复与勘误原样保留。本次worker-ledger.json与另一次同题审查的worker-record.json分别记账，不混算。保存时发现当前页已由并行记录更新为stage93，保留该裁决，追加本补充；stage92归档恢复为本次开始实际读到的stage92内容。

[补充报告](iommu-platform/docs/research-route-audit-20261010-supplement.md)／[本次接受](iommu-platform/out/route-audit-20261009/root-result-acceptance.json)／[原始复算](iommu-platform/out/route-audit-20261009/pn-raw-spotcheck.json)／[本次账本](iommu-platform/out/route-audit-20261009/worker-ledger.json)。本次记录不覆盖另一次更早论文审查的独立来源与范围。

## §93（2026-10-09）从原毕业论文开始的路线审查

用户要求审查是否走偏，并明确从 P/PN 更早的最初课题开始。本轮核原毕业论文的远程 page-walk 评估目标、9 月 8–9 日研究计划、E1/E4 与硬件连续供给、DS/E3、E2、固定 A8 method88、P/PN 和后期机制。判断：原方向合理，早期测量与供给修正必要；后期论文贡献未稳定而问题/合同持续扩展，需纠偏。当前具体候选不追加 RTL，保留平台与原始干扰问题；建议恢复时做一次有限论文可行性收敛，而非自动恢复机制探索。

同时修正筛选标准：单例被复现不等于整条路线无价值；含候选完整规则的通用控制不自动成为前作；每场景事后最优不是一套在线方法。固定 A8 p5、L-M/L-W 等负结果仍保留其实际范围，不因此重启。关键评估由 Codex root 亲自完成，DeepSeek 两次整理历史证据，另一个 Codex 席审查关键否决标准。新模型/RTL/Linux/构建/上板/Claude 均 0；没有新的性能验证或录用/分区承诺。

入口：[完整中文审查](/home/xvhaocheng/work/iommu-platform/docs/research-route-audit-20261009.md)、[root 裁决](/home/xvhaocheng/work/iommu-platform/out/route-audit-20261009/root-route-verdict.json)、[调用账本](/home/xvhaocheng/work/iommu-platform/out/route-audit-20261009/worker-record.json)。stage92 原状态已原样归档，当前机制暂停不变。

## §92（stage 92，2026-10-09）有界判断后暂停

本轮完成有界创新判断：得到具体复现边界，但没有新的已支持独立贡献。L-M 静态 {X,Y}/{Y} 主例由普通 early-bind+pin 从原始起点复现，L-W 的 root+middle+leafY=3 次 PTE 由有界 checkpoint 取消加一条虚拟前缀 partial-translation cache 复现；两例的 PTE 节省均不能证明独立算法贡献。按本轮开始时说明的理解，因没有新的独立贡献证据，保存记录后暂停探索；这不是整个领域无法推进。实际执行：模型/RTL/Linux/构建/上板/Claude 均 0，仅定向源码/原始文献阅读、手推和 1 关键 Codex 席；Codex root 设计/审核/保存，DeepSeek 仅起草中文记录，未执行命令。

新交接稿见 [mechanism-design-handoff-20261009-round05.md](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round05.md)；唯一裁决见 [root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round05/root-result-acceptance.json)。暂停后仅在真实原始轨迹/完整付费成本出现不可避免差异，或用户明确恢复时，才重审受影响声明；不自动启动模型/RTL/编译/上板。

## §91 记录（2026-10-09）

本轮把主线收敛为 L-M：把可互换的 COPY 端点当作活翻译选择，源填充时必须保住已接纳、未绑定父任务的最大同时输出分配容量；若没有保秩的普通 victim，就走完整权限检查、把该源 READ 请求经正常权限检查得到的地址翻译结果（PTE/翻译响应）直接返回原请求的 no-fill 路径，不返回源 payload，源工作不跳过。重叠集合 {X,Y}/{Y} 说明逐任务命中计数会漏掉秩 2→1 的容量损失，因此准入用完整 FREE 图匹配，标量 credit 不够；BIND 后保护转移到固定单元素；普通输出绑定后直到 WRITE 翻译被稳定的物理请求头实际接收前必须保持，FREE→COPY 时清除是错的。备选 L-W 是公共前缀内、下一 PTE AR 首次 VALID 前的集合目标叶子选择；取消只作组件与对照。字段小计 319bit/40B 与加 160bit 索引后的 479bit/60B 是包含关系，319+160=479，479 已包含 319，二者不能再次相加，均非净面积。本轮 0 模型/RTL/Linux/构建/上板/Claude，root 亲设、2 关键 Codex 席；M1/原始可达性必须允许普通控制立刻合法早 BIND/提前 WRITE 准备，不能拖延对照，真实源完成、头容量背压或晚语义约束须从原始轨迹产生，若早绑定等低成本控制消除全部重叠就停单独 matching 准入收益；DeepSeek 起草中文，Codex root 审核保存。

入口：[round04交接稿](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round04.md)，[root验收](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round04/root-result-acceptance.json)。

## §90 继续机制设计：正常源 READ 后绑定输出（2026-10-09）

接续 round02，主推 L-F：先预留空闲输出容量，做真实正常源 READ，再在首次 AW 前按事件维护 resident/free 索引绑定目的。新 pool-parent 合同必须保持各级 VALID 稳定与正常 READ/WRITE 权限。目的在源阶段真正释放的 ownership-change 是新判别题；精确 victim/pin 复现源自驱逐小例，source prepare+晚分配可能复现主例。等价实现不自动证明前作覆盖，hot-first/额度/软件 generation 搬迁不单独算创新。

- 交付：[round03中文稿](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round03.md)；权威：[验收裁决](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round03/root-result-acceptance.json)。
- 先并行 F1/F5/F6，后按证据决定下游 F2..F4；不建快照注入器；不迫使 source prepare 做重复 walk，允许 PA 续用与 source-prepare/WRITE 合法重叠。
- 首版接口范围：新 pool-parent datatype/API 与 register/completion 记录；source-only phase + 真实 READ 回调 + 保留 payload + 延迟 write-child 生成；IOTLB 功能事件 sidecar；内核 owned-pool 注册、恰好一次 release 校验/cache sync 与所选 slot completion。
- 停止条件随各 F 项 stop 执行；F1 未取得合法 ownership-change 正例则不进入后续收益主张。

实际完成仅设计/手推/定向原文核查与保存：0 模型/RTL/Linux/构建/上板/Claude。O98，3355bit420B与旧同信任3587bit449B只差29B字段；非净面积。Codex亲自设计和关键核查，1关键设计席独立推演，DeepSeek两次成功证据/文稿调用，Codex审核保存；实测 provider tokens 合计148316。

## §89 机制设计与下游交接（2026-10-09）

本轮仅机制设计与可执行交接，实现/复现交下游；入口 [机制设计交接稿](/home/xvhaocheng/work/iommu-platform/docs/mechanism-design-handoff-20261009-round02.md)，权威 [root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round02/root-result-acceptance.json)。主推 L：在首次 backend VALID 前按当前翻译局部性选择可互换目的槽；COPY_TO_POOL 接口本身不新，已被 io_uring provided buffers、DAMN、IOTLB wall 覆盖。备选 R：COPY 关系压缩 IOTLB，字段子集 876/1312 bit，但 generic 任意关系字典精确复现，不作独立压缩算法主张；fixed-4KiB paired specialization 可能同等降低两侧结构。低优先级 T：原生续步一拍。下一批只按 root 7 项 L1-L4/R1-R2/T0 的输入/验证/推进/停止标准准备最小模型与成本表；不预置普遍优越证明阻止最小模型。执行 0 模型/0 RTL/0 Linux/0 构建/0 上板；数字为手算/字段子集，非实测 cycles；不宣称首创/二区/模型通过；CoLT 设计章节已核（4.1-4.2），HPCA2014 全文核验待完成。

## §88：三方向机制设计与条件式路线选择（2026-10-09）

### 归属

- 设计/核查：Codex root 及两名关键设计席亲自设计/核查。
- 证据/起草：DeepSeek 源码证据整理和文档起草。
- 审核/保存：Codex 审核保存。

### 边界

- 0 模型、0 RTL/Linux 改动、0 构建、0 上板；旧 29 次 COPY 仅引用，不算本轮验证。
- 未测 LUT/Fmax/实际周期/Linux CPU 成本；手算均为归一化服务时间或显式假设。
- 未取全文：Marching Page Walks、NB-Walker 论文全文、ASPLOS 2016 True IOMMU Protection；SDM 本轮未重新读完评估。

### 已接受结论

- A 为条件主线，C 为条件备选；B 被完整普通 walker 复现，停止独立路线。A 候选与普通同合同 cursor 都为 10/14/24；范围正确 whole-source 为 8/14/26 或 8/15/25；没有独有系统收益证据。
- 最终主例纠正了初稿 CPU 两页 compute/store 的非法重叠；初稿 8/14/25 作废。
- 推荐只代表研究判别顺序；若完整新合同可由简单 cursor 实现，采用最简实现，不把这种实现等价误当发表覆盖证据。

### 下一批

1. A：先核同信任/权限合同与完整成本，对照可信 single-use DMA 仅进度、静态 reader 端点分段 cursor、具体可执行 whole-source、逐页源完成+预装门/定向撤销、rIOMMU 多 ring、额外源页双缓冲/bounce-copy。
2. A：过门后接 M1/M2，复用现有事件模型，先成功路径范围+真实返回。
3. C：先给普通 dynamic legalizer+相同反馈 bounded pinning 等强控制无法复现的手算点，否则不启动 M8/M9。
4. B：不再建模，只作两 context 普通 walker 强对照。

### 入口

- 权威：[root-result-acceptance.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-result-acceptance.json)、[root-range-complement-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-range-complement-design.json)、[root-cost-and-gates.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/root-cost-and-gates.json)
- 细节：[walk-critical.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/walk-critical.json)、[endpoint-design.json](/home/xvhaocheng/work/iommu-platform/out/higher-model-mechanism-20261009/round01/endpoint-design.json)
- 源码入口：[idma_reg64_frontend.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:323)、[idma_axi_transport_layer.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_axi_transport_layer.sv:170)、[idma_backend.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/iDMA/src/idma_backend.sv:519)、[rv_iommu_cq_handler.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_cq_handler.sv:330)、[rv_iommu_ptw.sv](/home/xvhaocheng/work/iommu-platform/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_ptw.sv:90)
- stage87：[status-stage87-20261009.md](/home/xvhaocheng/work/iommu-platform/docs/status-stage87-20261009.md)

## 2026-10-09：提示词改为同工作区接手版本

用户明确高级模型仍在 /home/xvhaocheng/work 工作。本轮复用已审核研究要求，补充当前状态、研究地图、关键原文/源码/真实RTL对照的绝对路径及阅读顺序、成果保存位置。入口：[prompt-workspace.txt](iommu-platform/out/higher-model-prompt-20261009/prompt-workspace.txt)。Codex核实23个材料路径并完成目录适配；未再次调用DeepSeek，无新机制裁决、源码改动、实验或构建。旧自含版提示词和全部既有记录保留。

## 2026-10-09：交给高级模型的机制设计提示词

用户计划让更高级模型提出方案与机制。本轮交付自含的中文提示词，明确平台、P/N 定义、已被强对照复现的候选、实际接口成本，并要求三个不同机制方向、原始工作负载起点的公平对照、最小正反例与条件式创新裁决。允许重新定义路线，不把历史负结果扩展成所有场景结论。只完成提示词准备，§87 研究结论不变，无新模型/RTL/构建/上板。DeepSeek 起草，Codex 指定研究要求并核查背景后应用。提示词与完整调用记录见 [prompt.txt](iommu-platform/out/higher-model-prompt-20261009/prompt.txt)、[worker-record.json](iommu-platform/out/higher-model-prompt-20261009/worker-record.json)。

## §87：共享存储对照与许可组合设计（2026-10-09）

- 阶段：§87 贡献筛选完成一轮；采纳普通同池缓存为架构对照，正向提出许可组合调度；实现门未过。
- 结论：统一载荷 9→6 只对重复 payload baseline；576/384 bits 非 LUT/面积；token/port/spill 未计价。elastic p5/R11 对 p3/R13 同 trueW16，但 samepool 复现；手推非 cycles。
- 许可组合：A/C 单项 0，合并 + 实际 W free3；B/D 两项 free2；maxoccupancy+partner 已完整复现所选组合手例；考虑阶段的完成优先也完整复现所选单项手例；不报领先/论文档次。
- 边界：native 仅 2 bank lifecycle；4-entry prepare + owned result/access_ok + Wowner/heldVALID 都计价；已 offered TL/AW 不改。prepare error==0 非 access_ok。32W/spills 非 free P5 cells；P5/P6 手例，caps3 非 ΣQ。
- 实际 DUT：仍引用 §76–78 六 build/29 copy PASS，不重跑；new model/RTL/Linux/build/board 0。
- 下一 gate：从 original workload 起点比较同接口全许可预准备/WRITE lookahead、maxoccupancy、单伙伴延续、stage-aware multicast/group；无差异不 RTL，不扩全矩阵。
- 证据：[root-result-acceptance.json](iommu-platform/out/innovation-unified-cost-20261009/root-result-acceptance.json)、[root-elastic-prefix-design.json](iommu-platform/out/innovation-unified-cost-20261009/root-elastic-prefix-design.json)、[root-permission-coalition-design.json](iommu-platform/out/innovation-unified-cost-20261009/root-permission-coalition-design.json)、[c-permission-scheduling-boundary.json](iommu-platform/out/innovation-unified-cost-20261009/c-permission-scheduling-boundary.json)、[worker-record.json](iommu-platform/out/innovation-unified-cost-20261009/worker-record.json)。
- 归属：DeepSeek 源码/成本证据 + 文档起草与定向修订；Codex root 机制/关键 primary/source 审查及实际应用；Claude 0。

## §86：强对照、晚许可复用与统一存储候选（2026-10-09）

- 稳定接受：stage86关闭所选所有目的端仅有部分写许可性能试验，保留安全准入能力；两child强普通对照复现所选性能点；另一项物理历史对照复现晚许可前缀读次数。因此这两项所选结果不能作为独有性能收益。
- 实际范围：无新模型/RTL/Linux/构建/上板；源码与论文直接检查加手推；最近DUT29PASS仅引用。
- 关键边界：单child H3 29/33对31/33只是小手例差；同等两child强普通为27/33；其余相邻条件持平或变慢。实际支持8在途，不得人为削弱强对照。WRITE记录数不等于总物理存储，READ阶段字段可回收。
- 物理历史对照：更简单的IOMMU后物理前缀历史具体对照复现相同读次数，故不为相同能力新增raw-adopt/prepare；该对照不是已发表覆盖证明。
- 正向设计：root-unified-store-design.json。未知许可历史与正常DMA共用真实载荷池，权限成功后原子引用转移，保持真实原READ/W/B/error合同。
- 成本待证：手例6载荷槽对独立history3加两个FIFO3共9槽，仅载荷数；3槽64位仅省192bit，token/元数据/端口成本可能抵消，无LUT/面积测量。必须完整同预算加准确最近前作才可称贡献，当前不入RTL。
- 前作：SDM PACT2023 [SDM论文](https://jaewoong.org/pubs/pact23-sdm.pdf) 已重叠permission/data且评估假定推测成功；SpecTLB/InvisiSpec/SafeSpec覆盖推测/缓存复用原语，不从搜索未命中宣称首创。
- 下一决定问题：统一存储相对简单共享物理cache/token的完整成本及独立许可认领差异；不再扩手例矩阵。若简单统一post-IOMMU缓存合同与成本相同，采用该架构并重设计附加规则。
- 证据：[接受](iommu-platform/out/innovation-native-cost-20261009/root-result-acceptance.json)、[统一存储](iommu-platform/out/innovation-native-cost-20261009/root-unified-store-design.json)、[物理历史](iommu-platform/out/innovation-native-cost-20261009/root-physical-history-control.json)、[关键前作](iommu-platform/out/innovation-native-cost-20261009/root-primary-review.json)、[源码](iommu-platform/out/innovation-native-cost-20261009/root-source-review.json)、[账本](iommu-platform/out/innovation-native-cost-20261009/worker-record.json)。
- 归属：DeepSeek源码证据与本次文档proposal；Codex亲自机制/关键源核/审查与实际应用；Claude调用0。

## §85：工作流程改进与准入边界（2026-10-09）

> Stage85接受已完成：能力合同保留，性能重设计；未启动RTL/build。

- 机制正向：绝对连续WRITE前沿Fi与独占Qi联合约束 e<=min(Fi+Qi)；已有积压计入Qi；最大安全段不等于最快。
- 四控制hand推导非真实cycles：长读ACK21/23 AR1；强split20/22 AR2；one-full sponsor22/24 AR1；all-full23/25 AR1；共同R8/W16、每flow3AWB、null0、PTW2。
- 许可：actual prepResp handshake进入已计费owned slot，trusted TC可查且旧区域保留至真B；READY0 TL VALID不等于已取得许可；普通one-sponsor/缓存复用允许。
- 决策：撤销长读必快/全面领先；不得用弱split或全目的fullprep制造优势。
- scope：无新model/RTL/Linux/build/上板；stage84模型仅引用不重跑；无新production RTL。
- 最新actualDUT：§76–78，6成功build步骤、29完整copy PASS；true late join通过，但五组强普通许可齐的起点共享均更快。
- DUT：[最近真实DUT接受](iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)
- 历史归档：[状态历史全文](iommu-platform/docs/status-history-through-stage84-20261009.md)
- 流程：AGENTS.md六规则已执行；稳定接受后一次普通文档；旧765行原样归档；无未经测量的节省。
- 归属：root设计/关键primary/source核/完整控制接受；DeepSeek流程证据与文档proposal；Codex实际应用；Claude0。
- 证据：[本轮接受](iommu-platform/out/innovation-frontier-boundary-20261009/root-result-acceptance.json)；[准入设计](iommu-platform/out/innovation-frontier-boundary-20261009/root-maximal-frontier-design.json)；[完整成本核对](iommu-platform/out/innovation-frontier-boundary-20261009/root-hand-accounting-review.json)；[前作卡](iommu-platform/out/innovation-frontier-boundary-20261009/stage85-c-exact-prior.json)；[关键原文与源码](iommu-platform/out/innovation-frontier-boundary-20261009/root-primary-review.json)；[调用账本](iommu-platform/out/innovation-frontier-boundary-20261009/worker-record.json)。

## 下一判别题

- 只判实际AR成本/许可可用时序+完整强对照的一个正反例；差异成立且接口闭合才RTL。

## §84：部分目的许可与真实配置边界（2026-10-09）

- 正向能力：首次VALID后不可撤的长源读，在冻结K个消费者且各自取得成功源读许可、连续目的写前缀Ci、真实私有返回容量Qi时，若每个消费者Ui=N-Ci≤Qi，可在首个ARVALID前整包准入；未授权尾段只进各自私有缓冲，待各自下一页写权限到达后写出。该规则把“读数据可接收进私有缓冲”与“写可落目的”拆开，旧sponsor/split合约要求V赞助完整长源返回或等待消费者写前沿。
- 几何与coupler条件：source256字与dest253+3只在decouple_rw=1且deburst=0下成立；decouple_rw=0会按读写共同边界把source切短，是现成普通shared/split对照来源，准入前可以主动分段。当前production RAW_COUPLING_AVAIL=0，不能称旧firstRcoupler已启用；若新使能coupler，一个source firstR只提供一个AW release credit，两个AW需要显式实际release设计，不能仅凭任务几何记录闭合。
- 所有消费者仅有部分目的写许可：在完全驻留、独立授权child service下可去掉旧full-WRITE sponsor；这是条件推导和手例，未跑sharedRTL，也未给新共享图。具体剩余题是所有消费者仅有部分目的写许可共享准入容量、独立prefix drain/真实所有接口，以及同等强共享分片/整段提前准备的总成本。
- 执行证据：Python有限状态6图190 states；唯一故意Ui>Qi case deadlock(r8,w5,pfalse)，其余5完成；oracle/edge/witness检查exit0。分图状态计数见结果文件，不在前缀重复。
- B独立卡：正例C5/6、Qi3已手推ACK22/24，payload新增0仅使用手例现有私有槽；旧groups barrier不能自动归因该特例，因为两侧prefix child count都是2。普遍heterogeneous C2/6、Qi6/2反例说明independent drain必要，
- 接口与成本：真实接口包括native access_ok只在完整授权后置位、pre-mux adapter先于所有公共W锁、NO_ERROR_HANDLING需显式failed job完成、按W-before-AW归属真实W/B；聚合SLVERR不够。C识别四流存储字段小计2835位，其中跨原始AW的340位包含任务几何与当前父起点；这仍是未闭合的字段估算，额外control、广播/准入/失效/错误/时序另计。
- 公平对照：候选与最佳shared split使用同样N个物理源字；候选节省启动准备依赖/一个sourceAR，不保证更少总PTE、数据流量、更早完整ACK或速度。
- 创新条件与下一步：该读写权限容量规则相对旧sponsor/split合约增加具体所有消费者仅有部分目的写许可unsplit-read能力，保留为机制候选；若精确prior hardware同成本提供同规则则为复现。下一步只指向所有消费者仅有部分目的写许可共享容量、准确原实现检索、完整成本；同一shared source下先比较实际common-return reservation control，不转为增加全面回归。本轮无新RTL/Linux/build/BIT/board；调用与token台账统一见worker记录。

链接：
- [接受](iommu-platform/out/innovation-permission-frontier-20261009/root-result-acceptance.json)
- [共享设计](iommu-platform/out/innovation-permission-frontier-20261009/root-shared-frontier-design.json)
- [B独立关键](iommu-platform/out/innovation-permission-frontier-20261009/b-shared-frontier-critical.json)
- [worker记录](iommu-platform/out/innovation-permission-frontier-20261009/worker-record.json)

## §83 当前：先能力/强对照/接口，保留统一欠账候选（2026-10-09）

- 用户“改进推进方式后，继续推进”。根AGENTS已固化一页贡献卡＋提前主线可达性＋复用校验器、不搭大模型/扩矩阵、每轮保留正向设计产物。root亲自提出等待链优先级及替代的retained WRITE-range组件/反例/债界；两个native关键席分别B有限投机承诺、C完整return-envelope准备。DS只证据，未执行/修改；没有新增Claude授权或调用。
- root核2014 US20140281081A1 blocked-path priority inheritance与MASK Golden/Silver，归档generic等待链优先级独立创新。直接重核CutForward2022 Rules1–7及MMU-aware DMA2018 §IV-C；进一步核Arm LTI IssueE2026 B1/B4/B6，before-ordering translation/lookaside、多事务区域复用和完成寿命已有，不能把准备接口/PA保留/B合并当新本体。
- B正向：首次physical AWVALID前L≤r+Q，r是真实专属连续ready前缀，Q3。source-starved当前child q≤3，q与整job/accepted source AR未返回长度分别计。patch先至少1真W再可escape，q严格降；每child≤3转换/≤6null/≤4AW，不是全job常数/无条件time界。parent描述符N不能送入任何W-owner mux；所有child/patch LAST在mux前，null不pop真数据、不推进logical byte、不提前user成功B；旧physicalB结算后才能复用metadata。
- 两完整N12/Q3手例（非运行cycles）：候选normal A18/2AW vsCF3 A22/4AW、B34/C49同；gap候选114/13/108、3AAW/3null vs所选resident117/11/107、3AAW/0null，sumACK235同。root证resident-only三槽完成12拍且在peer释放前至少4AW，但必要B复核找到同一个future-blind adaptive streaming普通：normal A17/2AAW，gap114/11/108、3AAW/0null，匹配/胜两例。保存原手例与新反例；下一保留immutable AW后才突发gap的统一欠账能力，不能据原两例报独有性能。
- B随后给完整突发branch：A0..5先到、A6..11到100..105，peerB ready10；普通AW9在7已固定、真W8/9/10后q6，A107/B110；候选firstAW6 LAST9/B10、B W11..13/ACK14、A后AW6在103/W104..109/ACK110。两边A2+B1 physical AW/B、sourceAR2、realR/W15、null0。root看实际卡并重算ready/LAST/B，是手推非模型。候选A慢3、peer早96，未证明全部ordinary不能匹配或总ACK最优；保留对每个未来缺口的债务保证。B额外只修stage82 `/hand_predictions/accounting`无peer收费一句，旧/新原文均保存在澄清卡，其余预测/结果不变。
- root独立held-R反例：只保留旧AW1..6能repair5/6，但accepted source AR24的下一child7..12仍可能fresh PTE排在自身满FIFO heldR之后；须准备完整accepted source-return envelope目的WRITE覆盖，或AR首次VALID前按证书边界切段收费。C native full prep＋one nonfull primer＋first-data AW、final true-envelope WLAST释放lease、全R/B后permission退休可闭合此类义务；同资源普通pretranslation＋既有coupler＋同lease逐动作相同，不进调度创新RTL。
- C字段302bit/flow×4＋global39＝1247，仅寄存草案；IOVA tuple140/flow若能真pin已有descriptor才687，现未证。失效/代次wrap/error/route/spill/timing完整成本未闭；现production非四小FIFO，DMA0/1 nonblock512，其余3。root直接核top请求/响应类型、TL arb/spill/三路响应、TC AW优先和bank read gate、平台premux：需新增明确prepare origin/response及可信range-checked PA child路径，不能用bank/ATS/debug语义偷换、不直接删read gate。
- 下一只核上述uniform debt capability的完整最小接口/同成本普通与准确successful-replay实现；对立场景是AW长度已固定才source停供。翻译主线另要实际cold walk/共享返回服务反馈；全译址h0固定sourcegap不能证明IOMMU因果收益。有可达能力/完整成本差异才定向RTL，不重跑§82/旧DUT。
- DS首dispatch代理不可达/无usage，原失败保存；同授权task一次reviewed网络恢复exit0。7定向文件实际prompt97813仍偏大，后续按关键源片段缩输入、不得降thinking/output budget。成功flash/high/max393216、stop未截断，completion33094含reasoning24409、provider total130907。root工具执行与source/primary接受；无新模型/checker/DUT/RTL/Linux/build/BIT/Mac/SD/JTAG/board。

[§83接受](iommu-platform/out/innovation-mechanism-focus-20261009/root-result-acceptance.json)／[root能力/接口合同](iommu-platform/out/innovation-mechanism-focus-20261009/root-capability-and-interface-contract.json)／[root设计与反例](iommu-platform/out/innovation-mechanism-focus-20261009/root-positive-design.json)／[B](iommu-platform/out/innovation-mechanism-focus-20261009/b-positive-design.json)／[C](iommu-platform/out/innovation-mechanism-focus-20261009/c-mainline-positive-design.json)／[实际worker记录](iommu-platform/out/innovation-mechanism-focus-20261009/worker-record.json)。下方为历史。

## §82 当前：结构成本核闭合＋18组有界补写模型（2026-10-09）

- 本轮延续§81的A/B，root＋两个独立Codex关键席做机制设计与边界；三个实际DeepSeek工作调用做接口证据、普通模型/校验器/测试提案与一次限定修正，后者不能执行命令。root核原文/源码/完整提案代码、应用并执行验收；未固定维持六席，无Claude。
- A保存Talluri/Hill ASPLOS1994原PDF、Unified-TP ICCD2020原PDF、Rocket固定c1dda5827d214cf04a4d0917c1b2915e36603d52源码。complete-subblock独立PPN/属性/valid早已有；独立普通two-slot validated-PTE控制header43+2×61+LRU1=166b（同observer168），types用成功PTE.v/r/x，不漏superpage mode；nested完整NORMAL172。path的g只祖先证据，非叶权限；cofill不能借global叶跨PSCID。专用S-only102→164新增62b/行，resume/fill/比较/mux/失效/在途状态均另计。根直接核PTW395–417、pkg315–330、IOTLB58–71/106–137、固定Rocket152–244和原文§3，决定停止这版独有存储贡献；不是证明综合逻辑相同。
- B根加入强普通N−Q流式prefix＋resident末Q尾，而不是全程Q分片：N256/Q3只两笔A AW。Arm NI-700 TRM §2.5.6明示W缺口时自动生成zero data/strobes余拍、实际B上行后reset master，read例sink余R；比旧专利dummy段更准确。MMUaware2018 §IV-C615–718根直接核父DMA ID/metadata、inflight drain/原序reissue/最终成功，不能把它说成只整jobabort。正常成功exact source-retaining suffix控制的最新完整实现仍未核，公开未查到不证明首次。
- root先写模型合同与独立手推oracle，再派DS。N24/Q3、WREADY1、单R/W每周期、oneAW/cycle、同h0preprepared，B3已在prehistory -3/-2/-1读取，只有B_release非None才有Bjob；source输入固定、无真实MIG/IOTLB/翻译反馈。原sourceR保留，synthetic null不pop真FIFO、不推进原logical W；原null physical B内部收，suffix全resident/oldB已回才发patch，最后真B才one userACK。normal/global权限稳定、noalias/Normal DDR等都仅有限合同，实际NO_ERROR_HANDLING不提供完整fault功能。
- 实际18比较及事件checker全部通过，18结果与事先root手推逐字一致，5unittest通过（含提前用户B/错误nullLAST负例）。nopeer long/C A25/AW1 vsO28/AW2；continuouslatepeer C25/28 vsO29/26；late gap C107/28 vsO107/26；short C31/28 vsO31/26；repeated C145/28 vsO145/26。所有A真R/真W24，B存在真R/真W3，physical B=AW，conversion多3null。O在conversion三例完整A相同、B更早2，traffic更少；C常态控制工作少一AW/B，但不是全场景优势。动态工作差p*qbar*w−(1−p)*c仅工作项，不能推出实际latency/hitmiss；前作和真面积成本尚不闭合，不送RTL。
- 保留v01原稿/执行TypeError，root看出正常nonconversion B被checker误判内部响应；DS一次限定修正重复cycle、absorption、CONVERT真FIFO/head与nonzero验收退出，root完整review后执行v02。raw/代码/错log/修正/实际trace/source SHA/ledger保存，不把脚本错误当RTL故障。
- root独立提出后继strict真实剩余量：patch首data resident可stream；至少一个真实W完成后才准再escape，q严格降，初Q3最多3conversion/6null、1+Q=4个A AW/B。单patch descriptor复用要等旧physicalB结算；仍需heldVALID、stable epoch/nooverlap、error merge与公平服务，数学拍数上界不是墙钟保证。B席独立质疑闭合。未实现/模型化此后继；下一有限判别加入普通prefix+逐尾resident单拍（同最多4AW、0null）、dynamic tail-stream和多个晚peer，不能靠只比always-tailguard声称独立创新。
- 主线仍多DMA共享IOMMU地址翻译干扰：若改善只来自通用AXI调度且无真实翻译/数据干扰因果，不能当主线贡献。无新生产RTL/Linux/DUT/build/BIT/Mac/SD/JTAG/板卡；最新真实DUT§76–78。三DS实际flash/high/max393216/stop未截断，总252450provider tokens，不声称节省。

[§82根接受](iommu-platform/out/innovation-recourse-cost-20261009/root-result-acceptance.json)／[实际ledger](iommu-platform/out/innovation-recourse-cost-20261009/execution-ledger.json)／[完整事件](iommu-platform/out/innovation-recourse-cost-20261009/all-traces.json)／[A完整成本](iommu-platform/out/innovation-recourse-cost-20261009/a-full-structure-cost.json)／[B推导](iommu-platform/out/innovation-recourse-cost-20261009/b-online-recourse-design.json)／[root后继](iommu-platform/out/innovation-recourse-cost-20261009/root-monotone-recourse-next.json)／[真实worker](iommu-platform/out/innovation-recourse-cost-20261009/worker-record.json)。下方为历史。

## §81 当前：按用户要求扩六独立方向，完成首轮贡献筛选（2026-10-09）

- 根AGENTS最新组织规则已记录。实际四native席含root：A原consumer席核翻译结构、B原delivery席核W义务、C原C2席核remap、root亲自D接口；两个DeepSeek外部proposal worker分别E/F，各一次限定修订。不是六Codex自主agent，不固定永久席数；无Claude。root创新设计不能全部交DS的分工继续。
- A初稿stage-shaped共享tag行：S-only把GSCID/content2S闲置字段换成第二任意叶或最低层pointer，172b/行仅resident字段，不是全LUT优势。根直接核tag/global/stage/52b content及PTW续走入口缺失。Unified-TP ICCD2020原文§III-A/B核到统一leaf/path、动态驻留；原完整a,a,b,a,b,c,d,e,f candidate10/greedy16，但固定PATH+MRU10、首叶+bypass9，同packed format two-hit状态映射精确相同。原稿保留，addendum撤独有读数/算法；只保留狭窄双证据共享行的完整format/ports/失效/region分配成本差未知，未送模型。
- B新合法postcommit动作：per-DMA出口shim必须在shared W mux之前，正常INCR、稳定源/映射、无目标重叠、当前无held真实W、patch/父B状态与真实尾payload容量预留后，原AW剩余尚未firstVALID尾拍发zero-WSTRB至原LAST。原source R不取消/重读；实际尾W之后用新patch AW写，旧B内部退休、patch B后才给原user B。若只在post-IOMMU端补零，上游mux owner仍锁，例不成立。root D列cut/grant、byte债、不可撤W/B、per-ID响应归并与真正完成合同；不把IOFENCE当job fence/future patch授权。
- B完整同6拍shared-pool强对照可消held-R环，longgap已承诺状态仍A/B104/108，candidate104/7，付3null W/额外AW/B。A源本来ready的反例loan5/9优于candidate10/7。根追加从descriptor开始的adaptive resident-prefix，104/6、同A AW2且少3null：不能用共同旧AW前缀人为保护整体创新。下一只核normal-operation transparent partial replay是否已有/真实continuous与罕见gap的在线完整成本，暂无独立创新/实测/分区结论。
- 原作边界：Arm IHI0022H早已允许去strobes仍送足尾拍；iDMA poison WVALID仍需真实mask-data，平台NO_ERROR_HANDLING；MMU-aware DMA ICCD2018有parent-aware burst metadata、failed drain/reissue与最终success，不可误说只会终止job。root下载并亲读US8332564B2 dummy remainder+真实B/abort-done段，缺payload独立物理排空在abort工程早已有；该段没明确zero-WSTRB与成功suffix继续，留准确未知，不凭未见证明首创。
- C独立设计controlled leaf BBM：旧R接收不发布授权、保留合法祖先、新make后重读leaf。具体一项partial-cache+同scope/代/通知/端口控制同t19，t39全root重启仅弱对照；此单活跃版本停止RTL。root直接核20260222 Table2 AV=1 leaf、PR/PW只processed旧请求到GOP，CQ362仍PR/PW TODO；新映射与旧页回收双完成/范围drain本身已有普通/VT-d近邻。D接口可由replay ledger表达，只作B安全成本组件，不重复列创新。
- E首次CBPE是普通work-conserving per-PTE reservation，nested15而非4、单FSM无checkpoint不能切owner；一次修订仍把phase-index2/3bit当恢复存储，漏PPN/GPA/权限，根不接受该表为lemma，固定链§45普通下界不重跑。F初稿extent demotion有同输入普通mark-dead；原union8无16容量压力，修订stage-specific aliases与普通map同动作。修订“不同DID不竞争IOTLB”错误：不同context不能因同PA复用，但仍竞争同16物理行，DID不在其tag且不自动等于PSCID/GSCID。无真实phase/layout trace，F具体机制均停止，不以新名字继续。
- 四DS实际calls均deepseek-flash/high/max393216/stop/未截断，provider总357381，完整task/raw/answer/usage/修订留存；E修订是prose并非JSON，原文照存。Codex亲自机制设计、关键来源/RTL/完整对照接受和资料保存，DS独立提案/证据/聚焦修订，不称其运行了实验。
- 本轮新math model/DUT/test/production RTL/Linux/build/BIT/Mac/JTAG/SD/上板均0；只研究产物与状态文档更新，旧脏改动/失败/历史不删，最新实测仍§76–78。下一优先B原文/完整能力成本门，A只并行核狭窄表示成本，明确正差才小模型/RTL；其余具体版本停止，不维持六个实现任务。

[§81六方向接受与下一门](iommu-platform/out/innovation-six-directions-20261009/screen-verdict.json)／[并行合同](iommu-platform/out/innovation-six-directions-20261009/six-direction-contract.json)／[B](iommu-platform/out/innovation-six-directions-20261009/b-multi-route-obligations.json)／[root D](iommu-platform/out/innovation-six-directions-20261009/d-obligation-interface.json)／[A补正](iommu-platform/out/innovation-six-directions-20261009/a-prior-addendum.json)／[C](iommu-platform/out/innovation-six-directions-20261009/c-invalidation-progress.json)／[E/F根纠正](iommu-platform/out/innovation-six-directions-20261009/root-e-f-review.json)／[worker账本](iommu-platform/out/innovation-six-directions-20261009/worker-record.json)。下方为历史。

## §80 当前：完整比较关闭三版独有贡献门（2026-10-09）

- 用户问三个agent够否、希望提效并继续。已用三个机制席：root亲自C设计/合同/critical review，原C席独立A成本，原B席独立B联合DDR；额外必要C2席核DARTS作者原实现。DeepSeek普通小模型/强控制与各一次修订；被完整复现的A/B及时停止，不为保持并发继续造夹具。
- A最强共享环从逻辑R留payload到真实W消费，独立许可后完整前缀一次加入；具体O-ring同P/RV/RA/WV/WA、同端口/授权/容量复现。B改变旧源门为完整短返回预留、新AR-only准入，热/冷手算7/8及9/10；root核旧compact当前W连续cover约束，本例旧DUT不可达。普通W-head guard不追B身份仍逐拍复现，删除baton无损，本版不执行模型或RTL。
- DARTS官方mirror固定ed79140（2025-06-03）及直接perfmodel/memalloc/coherency源核：默认transfer/立即解锁compute时长、one_from_free单最高priority、已有ref-protected消费释放。零size metadata漏setup、同consumer metadata寿命与双额度自然映射失配是确切边界；2023/2025最终全文缺口保留，非已核论文artifact tag；派生marginal_unlock_proxy不冒充StarPU实跑，也不参与独有收益裁决。
- C root合同：16desc/8组、单source walk、source LRU1/2、明确paid热目标、单完整copy及consumer，dispatch整片预留/ACK才回收；六scene×serial/各方相同单successor overlap。DeepSeek四Python初稿在test import出现SyntaxError，root核漏LRU hit touch，一次限定修订；12unittest exit0（矩阵memoize）及72基础完整比较exit0。
- root发现B2正结果可能只是warm-first、L1可能是fixed tile，定义普通真实setup计费与固定同首源key cohort k1/2/3，第二worker与上项修订实际重叠。初稿忘global remaining，48请求仅12valid/36invalid，旧all_ok=True漏数量；root源核/实跑后一次限定patch修正，source v02和失败JSON留存。最终accepted-c-v03强runner48/0invalid，各数量、warm重用、完整终端门通过；核心四文件与已12test/72run版本字节相同，不重跑旧矩阵。最终120种比较、12测试不是RTL/FPGA。
- 12个候选scene/profile都有具体普通对照精确同完整ACK列表、walk和makespan：warm-density覆盖8项，slow-consumer由group-first/k2/k3覆盖，L1由serial-k2/overlap-k1覆盖。不是一个固定k或万能在线选择器全域等价。L10八组candidate61.875/127/11比group-first56.75/107/9差；B2candidate/warm/k1/2/3均76.625/148/13。cap2实际LRU修正candidate54.375/97/8、普通k1 54/96/8；旧FIFO数字不接受。
- 因完整具体控制解释全部本轮正向条件，三版本轮不能当独有性能/机制算法，均不送RTL。下一候选必须先有简单规则不能合法完成的新动作或实付成本差，重点独立许可/不可撤W/B/多route的真实义务；动态选择tile也须比便宜selector、计公开反馈/扫描/通知。不把toy1/2 source槽直接等价当前默认16-entry combined PLRU，不预支真实多DMA应用。
- 4flash/high calls都max393216/stop/不截断，真实provider总325650；worker只提代码/patch，没有执行。Codex原始机制、关键源读、检查诊断、应用/执行/最终接受；无Claude、新DUT/build/综合、production/Linux/BIT/Mac/JTAG/SD/board。既有脏改动和历史数据保留，最新DUT仍§76–78。

[根接受与下一门](iommu-platform/out/parallel-innovation-model-20261009/root-result-acceptance.json)／[120对照判定](iommu-platform/out/parallel-innovation-model-20261009/c-comparison-verdict.json)／[DARTS原源边界](iommu-platform/out/parallel-innovation-model-20261009/darts-critical-verdict.md)／[A](iommu-platform/out/parallel-innovation-model-20261009/a-codex-cost-rule.md)／[B](iommu-platform/out/parallel-innovation-model-20261009/b-codex-rule.md)／[源SHA与版本](iommu-platform/out/parallel-innovation-model-20261009/accepted-source-manifest.json)／[实际worker](iommu-platform/out/parallel-innovation-model-20261009/worker-record.json)。下方为历史。

## §79 当前：三方向并行贡献筛选（2026-10-09）

- 用户确认先筛能力/精确前作/具体简单对照，再同时评估2–3机制方向，以推导、反例/简模淘汰，有差异才RTL。根AGENTS已记录；本轮根Codex设计A、两关键席独立设计B/C，三个DeepSeek证据任务并行。
- A：晚到共享读保留条件架构成本问题；root实读single-skid/single logical R，手构紧密返回时V可比solo晚，零leader-tax不能由0新payload推出。J/H计数须包含全部可用history与任何地址的数据重取，不能只数source-R。
- B：具体deadline/write机会草案与普通next-owner同两结果槽同轨迹；r32/a48/b8/x64/y8均112/120，固定W序/序无关译址成本等限制的两任务交换证明根核。本版本不RTL，真实共享DDR改变walk和W drain的联合反馈仍是可重开问题。
- C：sealed消费者按一/两缺页key形成桶前缀，different payload只共享译址setup；计划按真实WRITE完成/consumer ACK保留B4，选择包含容量停顿的闭包密度，带年龄逃生。原稿提前借未来ACK容量被root拦住，v01保存、正式稿一次限定修正：candidate ready24/26/27/38、ACK25/27/28/39；group-first ready11/35/37/38、ACK12/36/38/39。平均ACK31.25→29.75，手算4.8%，makespan同39，条件L>4；hot-page在四组完全追平。八组15desc（加Hi={wi,ui}）手构例截至t28 candidate3 ACK、安全hot-page≤1，非全8均值/总时间胜出。
- 根和C席核DARTS2025出版商引言/章节片段：少量transfer解锁many tasks已有；open失败，未拿全文算法/代码，不能据摘要说已覆盖/未覆盖所有译址合同。下一仅核DARTS实际聚合成本、shared translation vs shared payload、WRITE/B与consumer ACK寿命；再同信息/完整成本小模型和真实多DMA应用，给普通tile/prepare/overlap机会，有增量才原型。最新§78线性单job夹具不能直接当consumer/continuous引擎。
- DS3实际flash/high/max393216 calls全stop/未截断，provider总406691；完整任务/raw JSON/答案保存。DS没有执行/浏览/改文件，Codex做设计、源/原文核和接受。C修订是纸面合同纠正，非模型/DUT失败。本轮无新math model、DUT/build、生产RTL/Linux/BIT/Mac/JTAG/SD/上板动作；旧实验/脏改动均保留，最新实测仍§76–78。

[§79根筛选与下一门](iommu-platform/out/parallel-innovation-screen-20261009/screen-verdict.json)／[并行合同](iommu-platform/out/parallel-innovation-screen-20261009/screen-contract.json)／[A设计](iommu-platform/out/parallel-innovation-screen-20261009/a-codex-design.json)／[B设计](iommu-platform/out/parallel-innovation-screen-20261009/b-codex-design.md)／[C正式稿](iommu-platform/out/parallel-innovation-screen-20261009/c-codex-design.md)／[worker记录](iommu-platform/out/parallel-innovation-screen-20261009/worker-record.json)。下方为历史。

## §76–78 当前：真实在途加入通过，强普通更快（2026-10-09）

- §76一SV/7PASS、§77一SV+一C++增量/8PASS、§78三SV/14PASS：合计6成功build步骤、29真实完整copy PASS；不是整个研究历史总量。actual audit76/audit77_78 exit0，六source8+exe快照、R/W/CSV/LOG核过，旧记录不删。
- 模型修复：RR旧指针在idle探测时前进，interval64隐藏catch-up窗口；改实际R handshake提交，held VALID及同ID顺序保留。旧PASS仍支持字节/协议检查，旧interval>0零窗与公平服务结论须限定旧模型；未宣称interval0全部逐字回归。
- §77三项实际4周期宽窗口；§78已真正绑定，同沿req11、退short/禁A fallback、留旧V E/LAST、新W共同网格/peer门。5次actual join（E64/63/256），R257=N256+J1，0新payload，局部1functional FF/9observer FF，未综合。
- 主结果：join V19462/A19465、R257；弱普通 V23492/A23499、R320（平均低17.1607%）；强普通许可已齐起点共享 V19402/A19411、R256，反而更快。其余4组强普通也胜；因此这组17%不能称独立性能算法。许可迟到run13/14均无join/startup，V23244/A23246/R320，CSV/指标逐字相同。
- 关闭新join的§78run02与§77run04 CSV/旧指标回归一致；§76与§75三组pure-observer对照亦逐字同。脚本初稿错误及实际诊断保留，修审计没重跑RTL。
- 下一创新门：真实late提交/晚授权＋热/冷翻译/连续作业，同信息/总资源比较普通pending merge、等待窗、分片和有限history；首拍已交付后8B skid可能仍保留值，J1必须补普通保留响应复用对照（源码推导，未实现/测）。连续作业/epoch、remap/fault及held-physical-R绑定沿尚未测。
- Codex亲自设计、复杂诊断/关键源核/实际执行接受；DeepSeek纯观测/C++验收/普通audit提案及限定修订，6flash/high calls实际provider tokens836285，无Claude。out-only，无production/Linux/BIT/板卡/SD动作。

[§76–78根接受](iommu-platform/out/shared-read-inflight-join-20261009/codex-result-acceptance.md)／[§78账本](iommu-platform/out/shared-read-inflight-join-20261009/execution-ledger.json)／[下一创新门](iommu-platform/out/shared-read-inflight-join-20261009/codex-next-novelty-gate.md)。下方状态为历史。

## §75 当前：短补读夹具通过，更宽加入门待实测（2026-10-09）

- out-only四build成功、29DUT执行：27完整copy PASS＋run17/21两旧route-not-bound终端拒绝；不是watchdog。修复E=N（无共享尾段）立即绑定独立后run22/23 PASS，R512；7非法CLI另算。
- actual audit75 exit0：27份CSV AR/rfirst/rlast和真实R握手counter、W各256、logical r/W各2048、四source8+exe快照SHA核对。两修复回归＋上一阶段对照CSV逐字一致，旧metrics相同（SOURCE只四新driver字段）。CSV不含逐拍pr；末个raw父长度不是总作业长度，F1末A父255/整A256分开。
- 已实现普通A[0,J)短补读＋正常fallback，V不等A；18次成功short条件严格 rV=rA=rawWV=rawWA=J<E 窗口均0（J8时V已46）。没有实现在途加入，不能由0否定更宽窗口。
- 主线下一门槛：own许可/PA/属性相同、双方logicalR同T、旧W承诺≤T且payload已齐时，rawW可落后；原子绑定要退旧short状态、禁A fallback、保留旧V E/LAST和新W网格/peer门。宽门是待实现合同，具体错误接法死锁反例已手推，非现版DUT故障。
- 补读后合流/速度追赶/有限buffer在经典Patching/piggyback/HMSM已有；如果新门同完整成本不增加前作/强对照能力，就不能称创新；有真实增量才能形成机制贡献。源码新增1 functional FF/134 observer FF/0 payload，未综合；无production/Linux/BIT/board/SD动作。
- Codex亲自创新设计、关键源核/诊断/执行/接受；DeepSeek驱动和验收脚本提案/限定修订，四次flash/high调用实际provider tokens合计532557；无Claude。本段为最新状态，下方夜间状态保留为历史。

[§75根接受](iommu-platform/out/shared-read-short-prefix-20261009/codex-source-acceptance.md)／[机制合同](iommu-platform/out/shared-read-short-prefix-20261009/codex-mechanism-verdict.md)／[验收账本](iommu-platform/out/shared-read-short-prefix-20261009/execution-ledger.json)。

## §70–74 夜间任务完成（2026-10-09 08:00 JST）

- §70：22/22完整copy PASS；带宽受限时共享整体耗时降低约19%–39%，但部分条件V变慢。普通multicast解释少读收益，不能单独称独立算法。
- §71：50次执行40完整copy PASS＋10actual-cutover严格拒绝；两在线规则首批22项选择相同，无新增优势。odd-E/B候选环是关键席手推，定向RTL未复现，不能称DUT故障。
- §72：48次执行40PASS＋8初版真实watchdog；根因是独立A suffix继续依赖已完成V的READ有效项，改为ownA授权后最终24/24PASS，16旧成功CSV/指标逐字回归。4旧/4新版本快照、全ledger已审定。
- §73：30/30PASS；V闭合自身数据债/独立继续不必等A全授权；共享仍核双方真实许可和PA/attrs。计数器改为实际AR握手，4定向CSV/指标回归一致。长预算V慢约53.4%，不是性能创新。
- §74：20/20真实完整copy PASS；最终3条件确认A AR在rawV W闭合前接受，但只省12/14/12cycles（约0.13%），普通流水线解释。最终20份CSV/6build与旧新source/exe快照全部审定，3次早AR是真实握手；8项CSV/原指标逐字回归。
- 最后五阶段合计170DUT执行：152完整copy PASS、8旧watchdog、10严格拒绝；非整个夜间总量，非统计重复/上板。没有production/Linux/BIT/JTAG/SD/Mac动作。
- 创新下一判别：先观察A能否在旧V physicalR仍在途时追到J<E，再验证未来R安全加入，理论N+E→N+J。此机制尚未实现或DUT测试；如果强普通late-read-merge同成本重现，就不能称创新；如果在独立许可/不可撤R/W/B/小buffer下给出额外真实收益，才可成为贡献。
- 已按约定持续探索到08:00 JST并结束本轮；新研究动作等待下一次继续指令。DeepSeek提案/普通验收脚本；Codex亲自机制设计、复杂诊断、源核、执行/接受；无Claude。

[晨报](iommu-platform/docs/research-morning-20261009.md)／[§70](iommu-platform/out/shared-read-memory-service-20261009/codex-result-acceptance.md)／[§71](iommu-platform/out/shared-read-online-choice-20261009/codex-result-acceptance.md)／[§72](iommu-platform/out/shared-read-deferred-binding-20261009/codex-result-acceptance.md)／[§73](iommu-platform/out/shared-read-permission-independent-20261009/codex-result-acceptance.md)／[§74接受](iommu-platform/out/shared-read-overlap-closure-20261009/codex-result-acceptance.md)。

## §64–69 当前状态（2026-10-09 夜间，约06:47 JST）

- §64普通ordered-prefix额度：15copy PASS、全部终点metrics与§63相同，仅5个CSV提早发AW，不能据此主张独立性能算法。
- §65：38copy PASS，奇数E的既有promise保护与短W闭包修复；§66：47copy PASS＋6旧夹具真实死锁保留，Max2 issued-group由相等改≤后六项全完成。普通两B流水线解释压力约19%的改善。
- §67：30copy PASS＋2actual要求拒绝留存，6Max1逐字回归；同两B下minimum-J对burst末端仍有约1%–4%的配置收益，但未据此确认独立创新。
- §68正向新候选：先闭合不可撤W数据债，再让旧V tail与A prefix同时在途，单skid/privateB3不加payload；18copy PASS，13实测AR-before-oldRLAST；E256三项属于普通双独立读、zero sharedsuffix。
- §69强普通并行独立读：15copy PASS，4候选CSV/metrics逐字回归。E160共享R416 vs独立R512少18.75%，但V4550 vs3883慢17.2%；压力V9824 vs7800慢25.9%、A9833相同。因此当前不能把并行补读或少读流量当独立性能创新。下一检验带宽压力与明确保护V进度的在线共享规则，并核经典/最新前作。
- 本轮§65–69合计156新执行：148完整copy PASS、6真实旧死锁＋2actual要求拒绝，全部保留；每个PASS的实际R/W拍数与完整bytes已核，source/可执行快照及SHA ledger已保存。行为DDR真实RTL仿真，非上板/综合。
- 夜间目标到08:00JST仍active；无production/Linux/BIT/JTAG/SD/Mac/board动作。Codex亲自设计、诊断、执行和接受；DeepSeek独立提案/条件审核，原提案错误及修订保留；无Claude。

[§65接受](iommu-platform/out/shared-read-join-boundary-20261009/codex-result-acceptance.md)／[§66接受](iommu-platform/out/shared-read-write-pipeline-20261009/codex-result-acceptance.md)／[§67接受](iommu-platform/out/shared-read-join-pipeline-20261009/codex-result-acceptance.md)／[§68接受](iommu-platform/out/shared-read-parallel-repair-20261009/codex-result-acceptance.md)／[§69接受](iommu-platform/out/shared-read-independent-repair-control-20261009/codex-result-acceptance.md)。各目录execution-ledger.json和critical稿。

## §62–63（2026-10-09 夜间）：经典容量额度解释当前收益

§62三种真实Q/FIFO构建、37完整copy PASS；源读256/各目标W256，Q2回归完整metrics/CSV同。§63标准end≤peer实际W+Q准入一构建/15完整copy PASS，11对物理CSV逐字相同，包含prefetchOFF、PTE10000、RRoff和写背压。因此大Q相对resident的收益不能作为独立peer算法。根直接读CTC2009作者主文§3/4；下一§64用已承诺raw W序做普通容量预约、独立关键席审核，不据改名认创新。

详[§62接受](iommu-platform/out/shared-read-quantum-credit-20261009/codex-result-acceptance.md)／[§63接受](iommu-platform/out/shared-read-credit-envelope-20261009/codex-result-acceptance.md)，各ledger/9文件快照。out-only，无production/Linux/BIT/板卡动作；DeepSeek审计草案，Codex个人设计、关键源核、执行及接受，无Claude；夜间目标08:00JST继续。

## §60–61（2026-10-09 夜间）：简单resident准入追平peer规则

两个阶段各10完整copy PASS，各10CSV独立R/W计数都256，原0/1三对回归逐字同。F1：旧groups6098/6097→peer5596/5604→普通payload齐备5596/5596；F128 peer5164/5163 vs普通5178/5177，仅差0.27%。因此原barrier约8.7%不能称peer独立新算法。下一§62以同预算Q2/B3、Q8/B9、Q32/B33比较；尚在构建。详 [根判断](iommu-platform/out/shared-read-resident-admission-20261009/codex-result-acceptance.md)、各execution-ledger、accepted快照。out-only，无production/Linux/BIT/板卡动作；DeepSeek普通夹具，Codex设计/源核/实际执行接受，无Claude。

## §59（2026-10-09 夜间）：V独立早启动后接力，末拍反例修复

27次新执行26完整copy PASS＋F255真实停滞1次留存；最终build03 8/8 PASS，26CSV独立拍数核对。F1/K2真实接力V/A5616/5624，平均比同预取共同切分低7.83%，但源R298 vs256且V比独立串行慢19.31%；不称普遍创新。真实环是已授权末拍等待未来AW coverage、未来AW等旧B、旧W又被peer缺末拍的W头挡。仅full-WR/q2 phase3末拍可private停车，原logicalRLAST不代替最后W/B完成。详见 [根接受](iommu-platform/out/shared-read-early-sponsor-20261009/codex-result-acceptance.md) 与critical-early-sponsor-review.md、execution-ledger.json。下一检验同256源拍的字节进度W准入强简单挑战；out-only，无production/BIT/board动作。DeepSeek普通C++提案，Codex设计/诊断/实际验证/关键源审，无Claude。

## §58（2026-10-09 夜间）：强普通未来 WRITE 预取完成

一份SV完整构建＋一份C++增量，46次真实完整copy PASS；46CSV独立source AR声明/实际R、两目标各256实际W核对。激活真实下一WRITE页槽，完整WRITE上下文和正常TL许可，固定job页key跨原B保留；common W仍共同截到首F。F3/16接力平均更低，但F63以上多数预取更好，B64/背压F1差仅0.51%；不称普遍创新或等综合面积。根接受、全ledger与accepted版本见 [codex-result-acceptance.md](iommu-platform/out/shared-read-early-write-prefetch-20261009/codex-result-acceptance.md) 与同目录execution-ledger.json。下一核V独立早启动后的安全接力，同预取独立对照。out-only，无production/Linux/BIT/board动作；DeepSeek普通夹具提案，Codex机制设计/实际执行/接受，无Claude。

## §52–54 夜间研究（2026-10-09）

§52早恢复保留cohort8份真实copy PASS，A收益伴随V代价，普通规则；§53有限FIFO先读1build/12copy PASS，12CSV独立计数通过，普通q256与长读同轨迹，早恢复改善仅约0.6%。结果分别在out/shared-read-rejoin-20261009和out/shared-read-lookahead-20261009/codex-result-acceptance.md及execution-ledger.json。

§54接入正常跨页WRITE权限前沿：out/shared-read-permission-frontier-20261009/codex-design-before-run.md三强对照合同；critical-parent-progress.md直接源核首WRITE B不依赖whole source RLAST。RTL初build成功，跨页C++夹具printf修订中，尚未执行跨页DUT。mode3本版本改为普通独立V后A，与§52含义不同；不支持外部pause，不改生产/BIT/板卡。夜间目标到08:00 JST仍活动，根亲自机制设计、DeepSeek普通夹具，无Claude。

## §51 当前状态（2026-10-09，普通分片与两轮暂停判别）

- out-only真实RTL两build成功、15新执行：14完整copy PASS，其中最终build02六项都执行两轮pause；run09第二次quiet失败留存。根修：暂停不能挡住已first-VALID短W需要的R，singleton新AR需整段已有W覆盖。
- 普通mode2/q256与长读mode1四对CSV SHA逐字相同：无pause5657/5650；一次pause5161/19848；两次5161/29793；两次+B64/背压13277/28956。均同keep-q和四recipe。因为普通同成本同效，长读/q2的34%不能作为新算法贡献。
- sourceR无pause256、有pause504，目标W各256；14CSV独立计数核对。只给A可暂停、V继续的窄合同；logical quiet≠physical/backend idle。实际DMA为NO_ERROR_HANDLING，TC SLVERR不等于job可恢复fault处理。
- 夜间目标仍活动，到08:00 JST。接着核提前resume避免不必要退组/重读，以及独立context迟到/失败授权与source R交错的真正约束；不编BIT、不上板、不改生产Linux/RTL。
- [根验收](iommu-platform/out/shared-read-frontier-20261009/codex-result-acceptance.md)／[义务边界](iommu-platform/out/shared-read-frontier-20261009/critical-obligation-boundary.md)／[账本](iommu-platform/out/shared-read-frontier-20261009/execution-ledger.json)。DeepSeek driver提案，Codex设计/诊断/执行验收，无Claude。

# 论文平台交接

## §50（2026-10-09）：共享长读接真实 iDMA/IOMMU，完整拷贝通过

用户继续授权推进创新。根 Codex 实现 out-only q_write_splitter/shared_pretl_read，复用§43真实组件，独立 READ source recipe＋WRITE destination recipe；两 DMA AxID均0，mux逻辑ID0/4，samebank bit3，不能套此前误推的ID6。WR row来自实际rawAW/context、真实TL成功，复用于同页q-offset并由TC发真实写；没有借source.w授权target。CONF129/cache0010两模式同用且补wrapper属性链；首版每reset各一2KiB immutable source copy、目标分离、静态许可。共享R串行逐logical parent返回，一拍skid固定mask；q-W在全局mux前，首VALID不撤，收齐childB再返原B。pause清A已承诺q的R/W/B才退出，V沿长AR继续，A私有parent/cursor留待补读，quiet不是backend_idle。

实际27新run中，最终build06接受12（14–21、24–27），先前版本7PASS（03–07、10–11）；6失败（01 ID guard误写6的root错误、02未来WRITEwalk与held长R环、08/09 earlyresume A长AW抢V环、12/13完成byte但observer漏samecycle AW/W）；22/23参数+ptw-lat不识别、actual31，退出0也不作为latency0证据。run02前源留before-write-recipe/、08/09前源留before-resume-fence/；12/13 cpp留multi_test-before-counter-fix.cpp。6次SV构建尝试1parsefail/5成功；两cpp增量重链接成功。所有失败日志与raw CSV保留。根直接核并修4recipe门及VlogicalRLAST前A新AW恢复fence；已出现AW/W/R不撤。DeepSeek页表/普通driver/observer、文档提案，根审读实际补丁、应用与唯一执行；一个必要native关键review席核并发/权限。4flash/high/max393216返回成功未截断，provider tokens711997，无Claude。

lat31/no-pause V/A5657/5650 vs q2读8579/8572，mean低34.1%，AR1 vs128，R同256、两个W各256、各128child；lat100为6692/6685 vs18377/18370。两lat参数均全DDR读的模型代价，非MIG/PTW单独干预。pause10000/cut64B：candidate AR2 vs7、R504同，V3438 vs3571，A13450 vs13496；closedV/A15635/15628不能用作唯一弱基线。B64/周期READY背压earlyresume V3763/A4293 vs3802/4332，pin3181实际detach3254；同压力长pause A两方13712相同。每条件一次确定性运行，各真DMA完整reservedID DONE、byte untouched、源/目标geometry及virtualW队列严格校验；根额外核12份CSV的W256/256、ARlen总和及R504/256闭合。最终observer版本run20起，run14–19旧observer无背压严格检查已过，RTL同build06。

本轮直接复核DMA-Latte v3 broadcast/grouping、Multicast AXI XBAR W acquire/commit；共享读、一般死锁解法本身有前作。34%显著依赖读请求模型成本，独立pause下V实际只低1～4%，不能直接当独立创新/分区承诺。下一按同信息/完整缓存和暂停债务强化ordinary可变分片/有限先读，再扩多页sourceREAD/destinationWRITE成功前沿及写序恢复。如果强普通同效同成本就不能立新算法；若存在前作未覆盖控制且确有系统成本/进度增量，则可论证微架构贡献。局部声明状态审计已写；48B FIFO+8Bskid只是三个payload位置，公共R管线、TC FIFO、metadata/控制/组合逻辑与面积另计。未做综合、生产整合、连续job/fault/invalidation/ATS/device通用支持。本阶段生产RTL/Linux/tests/BIT/JTAG/board/SD动作0。最新actualcopy为§50。

补充合同：目前只验A一次pause/resume且V保持推进；退出后suffix W扩成长段，第二次pause不能沿用q债务界，所有成员同时暂停也未验。连续job/重复pause/多页需在下一门扩展，不能把本次单次reset结果当长期scheduler已完工。

[报告](iommu-platform/docs/shared-read-rtl-progress-20261009.md)／[根级接受与下一门](iommu-platform/out/shared-read-rtl-20261009/codex-result-acceptance.md)／[全部执行/版本/SHA](iommu-platform/out/shared-read-rtl-20261009/execution-ledger.json)／[源码/复现入口](iommu-platform/out/shared-read-rtl-20261009/README.md)／[关键审核](iommu-platform/out/shared-read-rtl-20261009/critical-write-recipe-review.md)。不要重跑过时初始化integrate.py，它会覆写后续修复；直接最终源＋build-argv.json构建，不复跑已通过旧stage/BIT。下文历史保留。

## §49（2026-10-09）：从复现的问题出发，推进共享长读取的独立暂停

用户要求创新、允许复现以发现新问题，不能继续只做负结果筛选。Codex根亲自设计shared source/independent logical job的first-VALID义务控制，两名必要研究席innovation_rules_40与innovation_map_40提出W粒度/ingress coverage及预算放弃共享译址的备选，全文分别在本阶段out的rules-design.md/map-design.md，均已完成。根核真实关键源和DMA-Latte当前v3、Torrent、AXI multicast、跨域RFC2887/SC24与Arm AXI属性；共享read/透明grouping/fork/slow退组+repair是前作，不借不同术语认首创。备选手算不是平台结果，当前只推进shared-read主线。

DeepSeek返回接口证据、初版事件模型、一次完整修订、局部fix与文档草稿，root审读实际代码并执行；5次flash/high/default max393216成功、不截断，provider tokens共527959。worker没有执行/写文件。root修正局部fix漏掉已EXIT/closed/independent resume的else分支；未把模型共识当验收，无Claude。

4次记录的模型执行：run01 293场景/3569checks、598失败；run02 306/6835、143失败；run03 310/7540 exit0；run04 4/104 exit0，后两次合计314/7644接受。run02 child已写JSON/log后ledger包装误把对象当列表，丢失start/elapsed/process-code变量；未重跑恢复记录，ledger明示result reported exit1。原code_run01/code_run02、raw响应、失败data/log都保留。run04实际VlongAW firstoffer4/resume请求6/旧义务drainEXIT10，open与强tile都完整恢复，两种AWREADY/RREADY互动不是toy。

N16/B3/q2/P3/resume200：open物理R28/AR2/AW6，强tile R28/AR4/AW6，最终都214 event steps；closed R16/AR1/AW16、V等resume。no-pause open/closed/tile同48steps、open/closed AR1 vs tile8；independent35steps但一次承诺longW，不满足指定P的有限pause，不能当同保证速度对照。N256早P2只节约512→510源beats，不能宣传普遍减半。permanent pause的open/tile V已完成，stalled标志只是driver等未resume的A，不能说V死锁。

根接受的是受限可行性/AR数量差，非RTL、平台cycle、PTW下降或面积。qgroup<=B、每流至多一个physical group、先coverage再冻mask可推A旧R<=q/W<=q，但未逐场景专项tail界或全输入形式证明；B3两流48B+skid8B=56B payload，Pythonout/history是oracle；完整metadata另计。默认FIFO512beat/4KiB每流可停2KiB，小buffer负例非默认bug。B与WLAST在模型同事件，真实delayed-B未验；permission/属性静态前提非真实IOMMU验收。

后续一个out-only真实adapter，不复编旧stage/BIT。根新核：当前CONF1 cache=0，合并read不能直接套。实际生成reg_top CONF[9:6]支持cache（hjson旧入口无该字段）；新原型和强对照同CONF129/cache0010且核下游attrs，device/nonmodifiable回退。现axi_burst_splitter只单beat，q>1写侧children及Bjoin需明确实现，读侧保长。保持raw logical parents，限prepared page/权限span与256beat物理AR，先单页2KiB、再合法32KiB段；source shared PA需显式映射且每context各自许可。计所有recipe/state、AR/AW/B、R/W、TL/PTW、V/A/makespan。若同完整成本真实RTL有系统增量、前作不覆盖控制，就可立微架构贡献；当前不承诺速度/分区。RTL实现/构建尚未开始。

[正式报告](iommu-platform/docs/shared-read-independent-scheduling-20261009.md)／[根接受](iommu-platform/out/innovation-rescue-20261009/codex-result-acceptance.md)／[数据摘要](iommu-platform/out/innovation-rescue-20261009/accepted-model-summary.json)／[执行账本](iommu-platform/out/innovation-rescue-20261009/execution-ledger.json)／[RTL合同](iommu-platform/out/innovation-rescue-20261009/codex-rtl-admission-contract.md)／[前作边界](iommu-platform/out/innovation-rescue-20261009/codex-cross-field-boundary.md)。本轮生产RTL/Linux/tests/BIT/JTAG/board/SD均无改动/动作，actual DUT build/run为0，最新真实完整copy仍§43；根AGENTS写入用户复现→发现问题→创新要求，新增文件仅out与文档。

## §48（2026-10-08研究，2026-10-09交接）：真实成功窗口与普通策略模型

root Codex亲自设计有限cache合同、核critical producer/helper/legalizer/backend/IOTLB、三项primary段落，审查DeepSeek完整脚本和schema修订，应用并执行唯一math model、亲读实际JSON接受。复用innovation_rules_40/innovation_map_40两席分别关键源码/普通等价与最接近前作设计，两席均完成全文保存。DeepSeek接口证据、普通脚本实现/一处schema修订/报告草稿四次flash/high/max393216，均stop且未截断，总193410 provider tokens（87127+34451+10219+61613）；Claude无调用，worker无shell/write执行。

模型1run exit0，0.01556497160345316s；3trace×E4/8/16=9组，每组四普通策略共36records。每流固定N8/source4KiB aligned/目的aligned、两2KiB bursts每页D,S,D,S，两个job共享source/context、destination独立；每trace64requests、24unique keys。gap8/4/1是逻辑页序列差，不是data retirement或cycle。LRU/ordinary remaining-use/private-dest+shared-source/oracle miss矩阵分别：
gap8 E4 32/30/32/29，E8 32/26/32/25，E16全24；
gap4 E4 32/30/32/26，E8 30/24/26/24，E16全24；
gap1三E全24。
小N2/E2和3的独立memo DP各6==oracle6，非全域最优验证。每miss×3PTE仅无PWC三层Sv39模型，不是DUT实测。oracle拥有futureglobal序，ordinary只用登记各flow规则/位置；private-dest槽计入E但非完整面积/端口公平。首次proposal重复trace字典key在执行前退回修订，trace_name/request_trace分开；此前未run/无旧JSON，worker提及既有JSON是未适用假设，不伪称重跑。

具体源码结论：producer pa是raw accept，done只在parent R last推进；helper prepared在request accept置位，valid在matching normal successful response写83bit结果，raw live不等helper len255资格。CONF1 source3072/dst0x10000/len4096生成1KiB、2KiB、1KiB，独立有限pause_readsA=1/pauseA=0能让raw前两parent驻留、prepare跳过首短父而先fill后父；max-success-page错误忽略未译page0。是paper module-interface witness，未DUT/旧CLI执行；原source aligned32KiB全full、strictINCR、prefixeligible、无faultflush、单slow限制下可以条件性成功前缀，快hit本身不推翻。普通从raw done..pa而非helper eligible扫描既有Depth4 success/seq可形成保守前缀、不需要新的hole bitmap；短父如果要求R前及时更新需扩大合法result资格/新增TC materialized证书，两方同计。source窗口不能免费外推write-direction或IOMMU job/epoch/context可见。

三primary已由root直接读：Pichai2014固定15p原稿§7.2/8.2含TLB-aware producer集合/共享PTE历史重组、hit LRUdepth；LocalityDescriptor2018已知range指导任务聚集/共享soft-pin/容量约束预取lead；Orchestrated2023固定6p作者副本新TB放置+partition/sharing，非当前规则live-stream throttle，远端字节未重新确认。3PDF hash/版本导航保存。以上与ordinary tile barrier/finite window+refcount构造覆盖泛原则，具体contract未写不等于无人做。

接受决策：registered job Remaining与entry取交/OR可被同信息ordinary interval/refcount完全复现；模型中ordinary在E8/gap4达首次24下界，E4 paired甚至LRU同效，停止纯remaining-use算法命名。保留工程反馈/窗口分界；无完整成本或独有收益。下一root候选只核TL materialization frontier控制超前、保留数据重叠、held/committed W必要有限继续。普通tile/lagcap、same-materialization finite-window、translate-first/lookahead/RR同信息/完整资源都允许；如果同decision/cost则停机制，如果具体组织有actualwholecopy或完整成本增量才可论微架构贡献。先构造当前W/window合同下真实区分trace，未通过不启动新DUT/BIT。N8/E16本模型全24，旧§43不同contexts，不可当positive baseline；需另定存在容量重读的规模。

[报告](iommu-platform/docs/translation-sharing-window-20261008.md)／[根设计](iommu-platform/out/translation-sharing-window-20261008/codex-window-proof-and-design.md)／[实际接受](iommu-platform/out/translation-sharing-window-20261008/codex-result-acceptance.md)／[执行账本](iommu-platform/out/translation-sharing-window-20261008/execution-ledger.json)／[数据](iommu-platform/out/translation-sharing-window-20261008/cache-model-result.json)／[下一判别](iommu-platform/out/translation-sharing-window-20261008/codex-next-decision.md)／[阶段账本](iommu-platform/out/translation-sharing-window-20261008/stage-evidence.json)。本轮DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD动作均0，最新actual完整copy仍§43，不重跑旧模型/DUT。11critical source与§47 hash一致只作provenance，不宣称全仓干净；没有速度/面积/期刊分区承诺。worker raw、两席全文与originals都保留。

## §47（2026-10-08）：登记任务的剩余翻译用途与强普通对照

root Codex亲自设计registered-job remaining-use候选与两槽信息见证，直接核frontend/legalizer/TC/banked/TL/最新§43 helper及2007/2018/2023/2025原文；复用innovation_rules_40/innovation_map_40两席作关键必要性/前作质疑，两席均完成且全文保存。DeepSeek整理接口证据并起草报告，root审查、编辑与落盘；两次flash/high/max393216未截断，共88876 provider tokens（57771+31105），Claude未调用。worker未执行命令/写文件。

合并physical fee与logical beneficiaries是不同账本；Li2015已有late merger更新排队score/累计age，2008 shared-hit/client history与UCP/SHiP/Hawkeye各有具体字段。停止把这些普通枝改名为创新。descriptor/setup送IOMMU、预装/保护、completed translation与consumer-use区别分别已有2023、2025、2007直接前作；MMU-aware DMA2018每burst只在请求抵达时需翻译，不能强迫ordinary整job锁或把早于data完成消费译址说成本轮新发现。

保留一个候选：有限已登记copy jobs的真实translation materialization反馈，形成多任务/两方向尚未译址后缀及pending用途并集，辅助shared-cache准入/回收；未知future job按普通策略。不hard-pin全部live，也不改变权限/失效语义。frontend已有src/dst/len但legalizer advance不是译址成功事件，新job/context/epoch/window接口全部计费。native TC每方向FIFO、单bank单选中pending支持限定单job INCR/fixed bank下保守页游标；bank是原AXI ID位不是DID/dir，2M/1G须完整覆盖范围。最新out§43 helper允许慢prepare与快reuse并存，prepared≠valid，未证eligible/pause形成前缀；下一读producer valid/seq/head链，需连续完成边界或计费holes，不免费用max返回序号代替前缀。

232bit两方向游标、另加端页464bit、128bit普通用途bitmap只是D4/E16/W29局部载荷手算；双方仍需context/epoch/pending/接口逻辑，不是面积或同成本结果。两槽信息见证仅说明已知用途可以改变victim，同信息ordinary也达到同miss数，非copy性能/新算法证据。

工作负载门：独立单调source/dst且无后续共享页复用，普通last-page cache/shared-entry pointer可能已消除全部容量重读，不能省compulsory walks。下一用两DMA共享只读source、独立destination、进度错开的32KiB copy；规则席给合法TL页表/握手字典，未run。先证真实额外非compulsory PTW，再允许同任务信息/总容量的global replacement、job-aware interval/protection union、bitmap、microTLB或shared pointer/ref保护、动态容量/prefetch/bounded release比较；若ordinary消差异或data路径遮蔽收益则停止纯替换性能贡献。

接受门是可靠新反馈/状态组织或具体决策的实际性能/完整成本增量及正确性，不要求数学击败所有可想象程序。当前candidate未实现、未运行、无收益/分区结论。下一按[单一判别](iommu-platform/out/translation-sharing-qos-20261008/codex-next-decision.md)推进，不重跑旧数学/§43，不先编译已有机制BIT。本轮math/DUT/build/production RTL/Linux/tests/BIT/JTAG/board/SD均0，最新完整copy仍§43。IOMMU原有dirty状态不是本轮diff；11关键源hash、6原作文件SHA一致保存只是provenance检查。

[报告](iommu-platform/docs/translation-sharing-qos-20261008.md)／[根设计](iommu-platform/out/translation-sharing-qos-20261008/codex-candidate-design.md)／[两席规则](iommu-platform/out/translation-sharing-qos-20261008/rules-critical-agent.md)／[前作审核](iommu-platform/out/translation-sharing-qos-20261008/prior-critical-agent.md)／[原文](iommu-platform/out/translation-sharing-qos-20261008/root-primary-browser-evidence.json)／[阶段账本](iommu-platform/out/translation-sharing-qos-20261008/stage-evidence.json)。worker raw/全文、原作和条件边界全部保留。

## §46（2026-10-08）：共享真实性判别与 requester 历史边界

root Codex亲自构造合法共享页表例与 IOTLB 接口不可区分证明，直接核 IOTLB/PTW/TL/fast 模块、最新 §43 fixture、既有 S3 及官方规范/2008 专利原文；复用 innovation_rules_40/innovation_map_40 两席作关键设计质疑。DeepSeek整理接口证据、补正和报告草稿，root保存全文、编辑并接受。两次 flash/high/max393216 均未截断，共263080 provider tokens；Claude未调用，DeepSeek无命令/写文件执行。

现有 IOTLB tags/content/PLRU 与 PTW walker progress 分属独立存储，N只接 hit、非第二 miss coalescer；没有统一 slot 竞争。最新 out/dual-write-prefix-20261008/multi_test.cpp:913–948 为 V/A PSCID10/11、S-root0x20000/0x30000、G-stage Bare，旧结果不能改称 nested/shared 实测。root纸面让 DID11 引用 PSCID10/同 root：P0/P1共享 root PTE0x20000 与中间0x40400，但 leaf0x50000/0x50008 和最终 SPA 不同。NB-Walker作者固定8837da7dda5591837877d20d4ad51b042c988aad源码已有共享fetch/每请求Info/有限MSHR/PTW预约；普通组合不立独立机制。原文/源码18文件SHA核对一致；官方RISC-V规范为补充，不冒称NB论文公平性定理或Go结构的RTL面积。

具体工程边界：IOTLB无DID tag、lookup不限fillmask，PartitionLocalHistory按resident way更新节点。A合法alias hit可使下一V cold fill从evictP0改为evictP1；只是纸面victim变化，非周期/PN慢因果。旧tests/composition_iotlb_isolated_test.cpp:570–576有意要求A-mask/V-way touch更新protected node7，因此旧合同与requester硬隔离不同，不称旧测试失败。main A normal alias与main A忙时fast V hit可在IOTLB边界给同key/touch/mask，却应有不同requester行为；需实际hit class区分，main fill mask会误伤fast V。普通sideband/filter是局部工程接口，不给端到端性能保证。US7415575B1 Fig7/claim6直接覆盖shared hit与requesting-client局部历史，基本机制不能称独立创新。

初次DeepSeek证据中Fixture-A的Vro/Vcold混列已补正：0x50010/0x200000D3是只读0x10002000→0x80000000；Vcold是0x50018/0x200008DF、0x10003000→0x80002000。另核fast来源dc_q.ta.pscid与req_q.did、accepted source；保留原始响应，补正单独链接，不覆写历史伪造原文正确。

下一保持共享IOMMU整体性能主线：同合法共享空间、同总状态/信息，纳入true-requester filter、resident-way与普通共享/容量准入强对照，寻找普通不可达的性能—成本点；不自动添加硬requester noninterference。必要时只复用TL bench做out定向例，覆盖main A alias/fast V来源；未决定生产patch，未实现或执行。本轮新math/DUT/build、production RTL/Linux/tests、BIT/JTAG/board/SD均0，不重跑§45/§43；最新实际完整copy仍§43。IOMMU非干净树，当前status与关键文件hash保存在账本，不当本轮diff。

[报告](iommu-platform/docs/translation-sharing-progress-20261008.md)／[共享可达性](iommu-platform/out/translation-sharing-progress-20261008/codex-sharing-reachability.md)／[根级历史证明](iommu-platform/out/translation-sharing-progress-20261008/codex-requester-history-boundary.md)／[完整worker补正](iommu-platform/out/translation-sharing-progress-20261008/deepseek-source-corrigendum.md)／[账本](iommu-platform/out/translation-sharing-progress-20261008/stage-evidence.json)／[下一判别](iommu-platform/out/translation-sharing-progress-20261008/codex-next-decision.md)。两席完整审核、两次worker raw/全文与原文源码均保留。

## §45（2026-10-08）：固定链容量—服务枚举收尾，普通方法达到读取下界

root Codex亲自设计双有限请求15步合同与max-gap iff证明、审查实际脚本和三篇primary；复用innovation_rules_40/innovation_map_40两席关键质疑。DeepSeek起草数学Python、一处搜索统计修订及报告；root保存全文、应用代码、执行唯一数学运行和结果接受。三次flash/high/max393216调用均未截断，共106589 provider tokens；Claude未调用。DeepSeek无命令/写文件执行。

执行1 math run，0.7781777381896973s、exit0、stderr空；5族×86B×15q=6450配置，每族1290。另125个代表路径/特例求解重叠在同一执行，不作为额外配置。Dijkstra可达性与手工max-gap全一致、mismatch0，所有可完成最小工作30。root读取JSON核字段/数量和实际路径；成功early exit计settled而非全reachable，未探索节点不称不可达；无解确实heap耗尽。没有重跑数学或旧DUT。

q3/B77普通G结果保存＋单S提前让路：每请求3/1/3/1/3/1/3、双方30读无重读；q4/B36 semantic根保留4/4/4/3同效；q3/B36不能跨0→4缺口，无完成。JSON cycle是每流一笔后reset，两席各三笔完整G后reset反例只手工，不伪称在JSON。q2专用七checkpoint/3bit tag给80bit普通上界，只纸面核未进五族run；mixed81/generic82不是q2通用下界。B只附加停车帧，C/active/路由/terminal等同计，q非cycle，30限no sharing/coalescing/PWC与各固定15步。

接受“存在可完成策略 iff q≥相邻可保存点最大gap”，非任意公平策略保证；固定链普通单调checkpoint达到下界，因此该曲线/早yield/phase编码停止作为独立创新。Barr2010 partial/path、跨层/reload-aware缓存与Bhargava2008 NTLB提供具体字段普通对应；不说它们已公布77bit/暂停保证。Hong–Kung一般hold/drop/recompute不能直接覆盖nested bit/quantum/fault全frontier。没有全编码信息下界或面积/周期/持续到达结论。

下一主线保持多DMA共享IOMMU性能干扰。只查真实共享PTE/partial mapping复用与progress保护的资源竞争，先验证实际key/context/地址/权限，然后同总容量与退休信息比较dynamic checkpoint、cache、pin与普通分槽；未知路径/持续到达更难不自动产生创新。如果普通能消差异则停，不增加阈值扫描/DUT。生产无当前暂停接口，不把纸面可切换walker当既有板瓶颈。

[报告](iommu-platform/docs/translation-progress-frontier-20261008.md)／[root证明](iommu-platform/out/translation-progress-frontier-20261008/codex-proof-and-decision.md)／[实际验收](iommu-platform/out/translation-progress-frontier-20261008/codex-result-acceptance.md)／[数据](iommu-platform/out/translation-progress-frontier-20261008/frontier_report.json)／[执行账本](iommu-platform/out/translation-progress-frontier-20261008/execution-ledger.json)／[完整阶段账本](iommu-platform/out/translation-progress-frontier-20261008/stage-evidence.json)／[下一判别](iommu-platform/out/translation-progress-frontier-20261008/codex-next-decision.md)。本轮DUT/build/run、production RTL/Linux/tests、BIT/JTAG/板卡/SD均0；最新真实copy仍§43。下一轮不重复本模型，raw响应、原文、先前证据都保留。

## §44（2026-10-08）：三席新约束探索与续走成本边界

用户问需要几个agent继续创新探索。本轮实际root＋两席复用native Codex（innovation_map_40/innovation_rules_40），分别设计与核平台、查最强primary、构造强普通反例；DeepSeek负责接口证据/报告草稿。两席各完成初始约束审查与nested continuation逐字段审查，交换fault和partial-restart反例；root直接读关键TC/TL/PTW/CQ、package/soc、CVA6祖先及Rocket2022固定source，作最终审定。未调用Claude。

正常TC AWAIT_RESP恒ready，物理AW/W等待在结果保存之后，因此worker从TL/RR看到的ready结构不能证明normal慢subscriber可达。root提出无关scope失效取消/重试，具体普通范围比较器与US20220414016A1覆盖，停止立新。nested冷4K路径15 PTE；每PTE tracking/token/next-level再仲裁在2009公开US8140781B2已有，phase union大块表示又被Rocket2022 bf5742b9e353a80f501d43130218d3a444ec4b30 aux父/leafPPN复用＋r_pte物理页号＋原request索引重建覆盖。香山2026-09-28固定source只证明S/G分解/返回，不误称其整实现82bit。FQ暂时存储满可背压，OF/MF/disabled是消费丢记录；普通FIFO/credit/去重同效，未测自然fault串。

两席给出I/S/F27-state恢复式与普通帧上界82bit；C/valid/token/active/在途/terminal另计，77只是在固定F4K、无重读、同完整IOTLB输出与合法地址族下的条件数据下界，82不是最小编码/面积。root补强partial G restart：保留S语义parent/leaf最大36bit，重启当前G子walk而非whole walk，一次pause最多重做3次G读。但重复正常G-root→暂停→重起可永远无S进展；持续进度需G+S最多4笔语义quantum（final G最多3）或有限暂停/足够连续服务。不能把一次handoff的一笔不可撤尾部偷换成无限抢占下有界完成，也不能用名义PAddr56虚构实际DDR有整个地址空间。

裁决：四条当前规则不立独立机制，不新建DUT。下一单一问题是固定到达/持续进度/同cache自由度/完整总资源合同后的容量—重复PTE工作—让路完整frontier；当前没有新锐界、新最优算法或未覆盖ordinary的新规则。如果结果只是选择已有checkpoint/restart/PWC点或一般livelock事实，就不能称独立创新。先纸面可达/必要性判别，不先阈值扫描或Linux/BIT。

[正式报告](iommu-platform/docs/translation-continuation-frontier-20261008.md)／[root设计](iommu-platform/out/innovation-frontier-reset-20261008/codex-critical-design.md)／[字段与反例](iommu-platform/out/innovation-frontier-reset-20261008/continuation-critical-agent.md)／[primary逐字段对照](iommu-platform/out/innovation-frontier-reset-20261008/continuation-prior-agent.md)／[下一判别](iommu-platform/out/innovation-frontier-reset-20261008/codex-next-decision.md)／[账本](iommu-platform/out/innovation-frontier-reset-20261008/stage-evidence.json)。本轮新DUT build/run/数学运行/生产RTL/Linux/BIT/JTAG/板卡/SD均0，最新真实copy仍§43。两个DeepSeek成功调用均flash/high/max393216/未截断，共118259 provider tokens，全文JSON/MD保留；DeepSeek无命令或写文件执行。Codex三席实质设计/核源/审定，root实际执行保存和文档更新。

## §43（2026-10-08）：双bank写序判别完成，停止顺序冲突分支

1新build（5.950741s）＋1新run（0.485378s），两个32KiB完整copy PASS。34笔external physical AW逐位置/owner/seq与virtual首次offer一致；head lag64周期、owner16/seq64重叠、max(vr−pr)=1，终态vw=pw=vr=pr=34。A request8315→sealed9469→quiet11321，latency3006；AW4→4、children3→7、parents5→5，A只完成7KiB义务而非整job；V quiet17KiB、pause-end32KiB。TL66/hit32/miss34/PTW102。不同暂停起点不得与§42作周期优劣因果比较，不重跑旧四项。

root直接核source child逐byte external-accepted目的前缀、demux首次offer锁bank/virtual WLAST退、bank FIFO及内部normal spill保序；native独立审计全文已修正TC COMPLETE是内部spill握手，可早于外部AW接受，root已复核。条件归纳P是V前缀且pr≤vr≤pw≤vw；合法headlag只有旧TC/output payload待服务，不等于AW倒置。限静态DEFAULT、正常固定映射、空payload reset；不是生产一般定理、内存公平/周期界或全系统无死锁。

研究停止：双写序冲突在当前原型不可达，停止该新机制枝。root进一步构造普通owner-prefix＋source剩余字节记账；纯byte-debt准入归为普通对照。AXI-REALM冻结W-last token gate使external AW前整W已供给；worker初版漏读此门及Normal触发当拍，已定向勘误并根核原源。下一只查真实翻译额外约束/锐界，若同信息同资源普通方法同保证就不能立独立创新；不先build阈值扫描。

[本轮报告](iommu-platform/docs/dual-write-order-boundary-20261008.md)／[账本](iommu-platform/out/dual-write-prefix-20261008/stage-evidence.json)／[原始事件](iommu-platform/out/dual-write-prefix-20261008/order-evidence.json)／[根级验收](iommu-platform/out/dual-write-prefix-20261008/codex-result-acceptance.md)／[准入等价](iommu-platform/out/dual-write-prefix-20261008/codex-admission-equivalence.md)／[下一判别](iommu-platform/out/dual-write-prefix-20261008/codex-next-decision.md)。完整模型响应均本地保留；DeepSeek证据/文稿，root Codex设计/核源/实际执行/验收，native只读无执行；Claude未用。改动仅out观测和研究记录，原tests/production/Linux/BIT/board/SD无动作。

## §42（2026-10-08）

- 归属：Codex 负责 RTL 设计/关键评审/构建运行/最终验收；DeepSeek 提供 harness/证据/草案；native audit 只读源码、未执行；无 Claude。
- 计数：new_builds=1，new_dut_runs=4，complete_copy_pass=4，mode2_pause_quiet=2，mode1_pause_quiet=0，old_dut_reruns=0，BIT/board/SD=0。
- 不变量：普通强对照 V 先 ≥15360 R/≥15368 W；A 先仅 A 7168/7168；7 个 NEW 1 KiB A child、无 NEW AW、parents 4→4；Aoffers 4→4 不是新 true push。
- A4 前驱：有序 AW/W 回溯 V 17 KiB；CSV2635/5733、CSV2638/6616。
- 成本：功能 ≥65 bit；diag counter/header 未综合。
- 覆盖：V 先 held1、A 先 held0；其他 cases source audit only。
- 下一步：同 codex-next-decision；若只剩单一有序队列守恒，按 principal innovation 停分支。
- 不重复跑四项或重新编译此版；新问题以 [下一判别](iommu-platform/out/committed-write-drain-20261008/codex-next-decision.md)为准。
- [报告](iommu-platform/docs/committed-write-drain-boundary-20261008.md)／[真实账本](iommu-platform/out/committed-write-drain-20261008/stage-evidence.json)／[根级推导](iommu-platform/out/committed-write-drain-20261008/codex-result-acceptance.md)。

## §41（2026-10-08）

- 先读 [codex-critical-acceptance.md](iommu-platform/out/fragment-drain-boundary-20261008/codex-critical-acceptance.md)、[codex-model-contract.md](iommu-platform/out/fragment-drain-boundary-20261008/codex-model-contract.md)、[geometry-oracle-result.json](iommu-platform/out/fragment-drain-boundary-20261008/geometry-oracle-result.json)、[geometry_oracle.py](iommu-platform/out/fragment-drain-boundary-20261008/geometry_oracle.py)、[rules-proof-agent.md](iommu-platform/out/fragment-drain-boundary-20261008/rules-proof-agent.md)、[map-boundary-agent.md](iommu-platform/out/fragment-drain-boundary-20261008/map-boundary-agent.md)、[release-composition-prior-agent.md](iommu-platform/out/fragment-drain-boundary-20261008/release-composition-prior-agent.md)、[codex-next-decision.md](iommu-platform/out/fragment-drain-boundary-20261008/codex-next-decision.md)。
- 不重复运行 math，除非根另有明确新任务；本轮只有一次 math run，exit0，7585/3805/11390 有限分类、mismatch0、两组重复覆盖。
- 把真实义务界对齐到指定释放点 g：首次 offer、mux owner push、held AR/AW、已承诺 W、两个 W 前缀、译址/服务独立性。
- 用 beat/prefix 里程碑替换整 transaction 图，避免误判正常流式进度为环。
- 强对照必须在同 B、同 payload、同 metadata、同释放点下比较：内部 nonuniform splitter、短 child 完整 R/W 预约、软件 tiling、仅停 source 有限 destination 续行、AXI-REALM 透明 child+Wbuffer、AMD PG313 选择性停 source。
- 根新增审定：§39 `compact_pretl_owner_fragment.sv:87–123` 的两层匹配 physical-accepted W prefix 门已覆盖当前 child 准入，不得再用“未来目的 AW 未准备”纸面反例当新 bug；实际待验假设是预算暂停时已有较远 W-owner 债务需更多 source child/同 job raw parent 接收。
- `multi_actual_dma_top.sv:610–622` raw AR 移出共享 mux、perflow credit 捕获、保留 parent 能力，限 out 原型，不能反推生产常规 mux 具备同能力。
- [下一判别](iommu-platform/out/fragment-drain-boundary-20261008/codex-next-decision.md)已落盘；受限单 flow fixedpoint 只是普通守恒，32KiB 无新增 cuts 到 job 末、未来 cut 3KiB 到 3KiB 仅为纸面例，须保持独立目的译址/服务前提、未证 DUT 可达、前沿不是新增 byte/cycles。
- 两名研究席的两轮审核均已完成，全文已保存，根已直接核关键原文、实际源码及两份最终分析；这不等于硬件验收。
- 不新增 DUT build/run、BIT、板卡、SD、production、Linux 或 tests 改动；full responses 留 out；文献仅定向补证未并入 123，不声称全域 novelty 或论文 gap。

归属：DeepSeek整理证据、提出脚本与文档草案；Codex设计、核源、实际执行并审定；未调用Claude。

[正式报告](iommu-platform/docs/fragment-drain-cost-boundary-20261008.md) · [两席最终证明](iommu-platform/out/fragment-drain-boundary-20261008/release-closure-proof-agent.md) · [执行账本](iommu-platform/out/fragment-drain-boundary-20261008/stage-evidence.json)

## §40 交接插入：翻译寿命边界与创新门槛

- 范围：根 Codex `codex-platform-gate` 审定，第一席 `innovation_rules_40`、第二席 map 均已完成；本轮完成候选设计与筛选，没有确立独立机制，不作论文等级或分区承诺。
- 结论：因为当前平台没有“普通译址结果必须等 DMA 数据完成才释放、并以该释放反馈阻塞 IOFENCE”的可达路径，源码可达性门槛否决当前 LTI-style 寿命反馈；Arm LTI §6.2.1 独立完成义务与 §6.2.2 多事务寿命分清后也不能直接套用。
- 已定边界：PR/PW 硬件 TODO 不是创新；Linux 默认 IOFENCE 设置 opcode/func 与 dword1=0，普通同步调用 PR/PW=0；真实 InclATS=0；CQH 早推进不等 generic completion；§39 静态 6 PASS 不验证 live invalidation。
- 强进度控制：普通 fault/error+合法 discard 必须列为强对照，保持 copy 成功是性能额外合同，不混安全或成功工作量；当前 fault 排空未验。
- 本轮的常规规则、PT 着色、共享计账、walker 续表转存具体提案均未排除强前作／普通方法，停止将这些提案作为独立机制；NB-Walker 新作者 artifact 已取得代码但论文全文仍未得、未复现实验。
- 保留窄问题与下一最小判别：错位 burst 尾 buffer/最小分片成本界；以 [codex-next-decision.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-next-decision.md) 为准：固定映射、long copy 内可控让路、小缓冲下最小合法前沿/分片成本界。temporary translation lock 只属 ShareMMU 适用性反例，不是生产新加锁，也不是当前 IOTINVAL 语义。
- 主报告没有排除强普通 new rule，因此不能认定独立机制；待证明锐界只作分析候选。下一判别未 run，不假造实验/算法/证明。
- 报告：[iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md](iommu-platform/docs/translation-lifetime-and-innovation-gate-20261008.md)；根审定：[iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md](iommu-platform/out/translation-lifetime-boundary-20261008/codex-platform-gate.md)；两席：[innovation-rules-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-rules-agent.md)、[innovation-map-agent.md](iommu-platform/out/translation-lifetime-boundary-20261008/innovation-map-agent.md)；证据：[source-evidence.json](iommu-platform/out/translation-lifetime-boundary-20261008/source-evidence.json)、[deepseek-source-evidence.md](iommu-platform/out/translation-lifetime-boundary-20261008/deepseek-source-evidence.md)。
- 归属：DeepSeek 整理证据和起草文档；Codex 根与 2 研究席作创新推演、关键源/原文核查、最终审定；未使用 Claude。

## 2026-10-08（§39）：普通已准备源片物理AR旁路已完成

- 接用户“推进”，Codex独立实现out/prepared-source-bypass-20261008六源副本；parent缓存导出同generation正常可读PA，child首次offer固定direct位；完整AR复用，不新增payload；ID0/4→1/5保留位路由、R还原。单全局源child使TC源AR与direct互斥，实际full属性保持／互斥／R身份和子／父RLAST检查无错误。范围仅正常静态+reuse2，不是任意组合／fault／epoch／失效通用证明。
- 实际1新SV+C++构建exit0（5.871s）、6新运行6PASS；原旧DUT未重跑。off V/A CSV与§38 bypass-aw-first逐字节同；[账本](iommu-platform/out/prepared-source-bypass-20261008/stage-evidence.json)有真实argv／源码和binary哈希／raw。build原有UNOPTFLAT类型仍在，未综合或核FPGA时序。
- V先off23380/26122均24751，cache17704/20246均18975，wait18146/20984均19565；旧post17485/20163均18824。原gap5927只剩151，缩小97.4523368%；post相对最好普通低0.7957839%（分母18975）。cache A先20279/17737均19008，交换后各+33，非镜像复制；off/wait镜像。
- cache direct62/fallback2，TC写AWAIT_RESP期间实际directAR25(V先)/16(A先)，wait64/0和24/24。六项TL66／lookup66／hit32／miss34／DDTW2／PTW102／FAST0／源AR64／AW34／每流4096R-W，fullbytes／guards／quiet；原父32／child64，原window高水位4/4终端0/0，normal cache32父填入，valid/slow/hit/offer0。TC入口var=fallback，不能强制64。
- [根直接审计](iommu-platform/out/prepared-source-bypass-20261008/codex-raw-audit.json)核全部source地址／len／低ID位与direct事件，held与route排空；[轨迹比较](iommu-platform/out/prepared-source-bypass-20261008/timeline-comparison.json)显示首AR/R普通早897、后续相对差-897..1402。两次V父2/5未准备好走TC，但没有证明它们解释全部151，不能称带宽或固定流水瓶颈。
- 声明功能3317+1281=4598 vs旧post4602，只增1位route，其余observer新增128+149另计；无完整公共成本配平／LUT／面积。76productionSVhash前后同，无生产tests/Linux/BIT/SD/上板。原生产C0修复仍是out副本，不要误报已修生产或旧PN原因。
- 创新判断：[报告](iommu-platform/docs/prepared-source-bypass-boundary-20261008.md)和[Codex下一规格](iommu-platform/out/prepared-source-bypass-20261008/codex-next-boundary.md)。因为Arm LTI1.1/1.2.2/1.2.3明确有提前译址／lookaside／cachedintegration且普通几乎追平，位置／普通旁路／原23.95%不立独立机制。下一先核平台真实结果完成寿命、未准备目的W、IOFENCE/软件的可达依赖；LTI6.2已有完成与共享寿命规范，不能拿规范条款当新发现。如果普通按子片译址／预约／重译同性质同成本，就不能称独立创新；只有具体新约束下前作未覆盖的独有进度/成本才立题。若平台没有该约束，不人为添加以制造胜利。
- DeepSeek离线汇总／报告草案，Codex架构、关键源和官方前作核查、实际执行及最终审定；所有调用完整返回本地保存。首脚本漏在MD显示mirrorFalse、修订示意patch不能直接应用（git apply --check exit128），这只是汇总工具提案问题，不是DUT失败；原响应和失败patch保留。
- 有界恢复已完成：DeepSeek返回完整`summarize_revised.py`，Codex读取关键检查后执行exit0，最终`analysis.json/md`六项evidence_ok、build成功且cache镜像差异显式列出；数学／V-A数值与根raw审计一致，无再次DUT。3次调用均flash／思考／默认预算／完整未截断，182478 provider tokens见[model-usage.json](iommu-platform/out/prepared-source-bypass-20261008/model-usage.json)。复现汇总用revised，原脚本和示意patch是历史提案，不覆盖最终analysis。

## 2026-10-08（§38）：多父准备边界 handoff

- 状态：5 构建／18 运行／18 PASS；8 次普通模式、4 次读优先、2 次观察运行、4 次命中直通；仅 out 副本，未改 production/tests/Linux/BIT/SD/board。首次汇总解析失败（VAR32 与 64、header0x）是解析／汇总问题，不是 DUT 失败；summary.py 初稿保留，summary_revised.py 经 Codex 字面 mdir 修正后 8 项全部接受。`analysis.md` 只覆盖最初 8 项；根审计覆盖全部 18 项；新增 10 项是 DUT 运行，不是另加根审计。
- 证据：[codex-raw-audit.json](iommu-platform/out/multi-parent-preparation-20261008/codex-raw-audit.json)、[analysis.md](iommu-platform/out/multi-parent-preparation-20261008/analysis.md)、[报告](iommu-platform/docs/multi-parent-preparation-boundary-20261008.md)、[下一对照规格](iommu-platform/out/multi-parent-preparation-20261008/codex-next-control.md)、[前作核查](iommu-platform/out/multi-parent-preparation-20261008/primary-source-review.md)。
- 结果：已完成最强普通缓存命中直通 AW 优先 V 先启动 23380/26122 mean24751；旧 post window4 V 先启动 17485/20163 mean18824；降低 23.946507%，绝对 5927；不是 27% 独有规则证明，也不是普通 TC 旁路。prepare0/1/2 均值均 25869.5 但不是 3 个独立性能点；读优先非统一最好，缓存命中直通 read 24992 比 24751 差。
- 新颖性：Arm LTI 已定义 translation-before-ordering／lookaside（1.1／1.2.2）、同 page+context share 与 completion deadline 6.2.2；仅缓存读绕过 TC 或背景未命中不能称为独立创新。下一最强普通对照未实现；下一优先检验普通物理AR旁路。
- 因果见证：源 V0 缓存填充 1019；写 REQUEST 2421→写 RESPONSE 2861 同一 pending 持续 440 周期等待写；源命中接受 2868 在写完成之后，不能称 2868 时 TC 仍忙写。计数为 reset／drain 重叠，不是延迟分解，不与 flow completion 求和。
- 下一步：实现已准备源片物理 AR 在 TC 写未命中期间通过，full permission／lifetime／BC／error／routing，same original window／saved metadata／payload；若普通完整 TC 旁路同进度+成本，则位置／组合规则不能称为独立创新；若联合依赖前沿准入／保留能在同预算下展示独特进度／成本，则能称为独立机制。无 production 改动。

## 2026-10-08 §37：同原窗口残差6.7%，物理单父槽存在可达进度缺口
- 用户“持续推进”。Codex亲自设计out/original-window-20261008准入：两flow各4bit计数，在原iDMA AR进入既有mux前消耗credit，原parent最终RLAST返回DMA才退；同时接受/退还保持，localcredit非ready依赖，完整stalled AR断言。显式C++ parent_window_i=1/2/4/8，物理父深度另外1/2/4，不改AW/W/R/B/payload/原tests。窗口8两序CSV等§35post，有实际输入/highwater/末live0佐证。
- 初build后按8/1/2顺序，window2-v watchdog停止首次run.py；没有重跑前四成功。继续只跑缺少的2-a/4-v/4-a。父depth2新增build试window2/4，depth4新增build试window4。容量组3build14run：8PASS、6watchdog，无未知/缺失。六停滞来自window2/4-depth1及window4-depth2两序，保存全部raw/argv/results，不算完成均值。
- 同窗口1 post delta22282/25697均23989.5，旧普通提前25714，低6.706463%；post窗口8均18810，原差距缩小5179.5，但不作全部因果分摊。窗口2/depth2 delta18595/21707均20151；窗口4/depth4 delta17485/20163均18824。A先各逐路对换非随机重复。
- 冻结见证window2-v: paV=doneV=2/vaV4，paA1/doneA0/vaA2，inputID4，双W头V/seq2/got128，TC COMPLETE等待A的physicalAR。V缺下一父，A槽不释放因为W头V，TC不能越过A。12状态16384..196608固定，DDR读队列空且无有效PTW。普通多父槽解除本例；§34原轨迹单槽同效保留但不能推广进度。是Codex原型组合缺口，不是原生产源码bug或旧PN解释。
- Codex又实现普通背景owner资格，仅prepare_eligible接现有i_pre.candidate_valid/candidate_owner；不改父有效判定/缓存寿命，已形成offer不撤销。新build2run两PASS，准备32/填32/复用64/TL66，末slow/hit/valid/offer0。delta24085/27341均25713，只比旧early快1cycle，未追平post-window1（剩1723.5/6.702835%）。TL入口VAR64是前置child，父32另从POSTTL-STATS核。
- 全阶段4SV+C++build exit0约6.00/5.73/5.69/5.73s，16freshDUT=10完整PASS+6watchdog；旧6refs只读。容量final363条件、timing48条件绿，collection_valid仅正确采集并非全DUT通过。DeepSeek3high/defaultmax393216/stop/nontruncated完整汇总，共775277 provider tokens；Codex审定纠正跨窗口混平均和误要求post-window1=oldearly，完整初稿/raw保留，修summary不重DUT。
- poststate3568+342*(depth-1)+入口8=3576/3918/4602bits，普通提前及owner资格3518，观测另计，未综合/非总预算。正常成功均TL66/hit32/miss34/PTW102/DDTW2/FAST0/物理AR64/AW34/每流4096R-W/完整bytes/guards/NEXT-DONE/quiet，原FIFO48B，无新Rpayload。故页表流量不解释本差异。
- 本轮核PULP axi_xbar W-order/spill环、2026PreemptiveVC端点DMA RW耦合和2025US20250190351A1 MMIO预装译址，Arm LTI已知同页共享/排序前译址限制继续。因为普通加槽解停滞，不能把加槽/credit/原型等待环本身当新；若同窗口同总预算普通同效，联合规则不能称独立创新。须有新约束下独有进度/成本并核最强前作才立贡献。
- 下一实现更强普通pre多原父header可见范围/多父普通prepare，优先匹配窗口4和post-depth4 metadata预算，不能再用pre1current父替代。Codex核响应type185中normal非MRIF83、MRIF102；predepth4声明3828，8cache*(83+valid)=672，subtotal4500 vs post4602，仅102给控制/保持。是预算边界未实现，不省VALID稳定/结果全字段/选中父寿命/失效错误退路来凑数。76生产SV hash同，无原tests/production/Linux/BIT/SD/上板，生产C0仍未应用。
- [完整判断](iommu-platform/docs/original-window-progress-boundary-20261008.md)／[raw入口与执行分类](iommu-platform/out/original-window-20261008/README.md)／[actual源码/二进制hash账本](iommu-platform/out/original-window-20261008/stage-evidence.json)。下一从普通多父准备预算对照继续，不重新扫描或重跑本轮，不先BIT。

## 2026-10-08 §36：普通父复用／当前父提前准备完成，六原父可见范围差异已证
- 用户“持续推进”。Codex独立实现out/parent-translation-reuse-20261008/parent_translation_reuse.sv接真实TC/TL，真实pre父上下文/seq关联，成功完整response缓存后复用第二1KiB，完整TL context相同、PA真实spaddr+offset、页内/normal/父有效检查，无payload/W调度变动。关键接口无AXI ID，不能从TL单独认父；父metadata由i_pre真实寄存状态输入。
- 首SVplusargs模式开关未生效，生成obj-reuse代码常量enable1；首4reuse0/reuse1-{v,a}都exit0/PASS但排除受控对照。Codex初归因独立context不准确，读C++ Tb(new TopT)/生成代码后撤回，未追工具消除原因。改显式顶层input+C++解析+reuse=0/1，obj-reuse-port模型4新bound0/1两序全PASS。关闭CSV字节等§35pre；首次SV/command/log在.initial保留。
- Codex再实现early_parent_translation.sv，+reuse=2、背景准备只有TC无VALID且无slow/hit/offer时开始；offer寄存保持到TL接收，先形成的正式TC请求优先。复用既有slow请求寄存器，slow_prepare分响应归属，prepared标志/seq防重复，仅每flow当前已接受父，normal静态映射，真实prepare响应自己消费，正式child缓存返回。obj-early两序2PASS，prepared32/filled32/reused64/forwarded66，终止slow/hit/valid/offer0。
- 总3新SV+C++build exit0，约5.943/5.944/6.247sec；10新DUT均exit0/PASS，4excluded首批+6有效（4关闭/复用+2提前），4旧§35 pre/post引用不重跑。无额外RTLtest重复，解析修正只读raw。
- V先真实issue→done：关闭24162/27436均25799；复用24084/27340均25712低0.33722%；提前24086/27342均25714低0.32947%；旧post17471/20149均18810，残差6902/6904。A先镜像，非随机多次统计。两普通TL调用66/hit32，对关闭98/hit64，均miss34/DDTW2/PTW102/physical AR64/AW34、每流4096R-W/bytes/guards/NEXT→DONE/quiet正确，不能把miss/walk少或全时差归重复TL。
- 真实旧post/S CSV第一次physical R2404之前，六原VAR cycle68/69/70/71/2367/2374且len255、first-flow地址连续0..0x2800，六父不可能完成。是整路径原父在途下界≥6，非某内部队列高水位6；pre入口每flow1原父。普通early仅当前父，没有同未来可见范围，不能因它低效声称普通lookahead无效，也不归全部残差于队列。
- 声明req118/resp185，普通helper1012+pre2502=3514bits，earlyhelper1016+pre=3518，post3568；extra观察128/160另计，未综合/非总面积，公共TC/cuts/逻辑可用capacity需配平。输入invalidations原harness硬0，虽有清除/禁再填入条件，不作fault/epoch/软件可见fault通用保证。仍同bank两真DMA/CONF1/各32KiB/src0/dst3072/48B FIFO/全部行为DDR读31，非四DMA/MIG/PN修复。
- 本轮直接核官方LTI IHI0089A §6.2.2同4KiB同context翻译共享及完成寿命、1.1排序条件前译址、4.2SPEC不能直接用；父复用/提前/单加队列本身不能立新。下一普通同原父可见范围：复用TC/header队列或未接受但稳定VALID下一父的无副作用准备，并计映射保存/权限失效/全部metadata。该下一版未实现，不重跑此六项或先BIT。
- DeepSeek3次接口取证、汇总草案和有界revision，全high/max393216/stop/nontruncated，usage259312 provider tokens。汇总首14条件失败是absolute offset/parent-local及reused字段未认，回传修好；Codex关键最终review发现跨启动只取先发起者平均，直接修为固定V先V/A，778最终绿，并raw重算均值/镜像。不是所有PASS就接受研究结论。
- 76生产SV哈希同，无原tests/production/Linux/BIT/SD/板卡动作；旧§31生产C0修正未应用。独立创新条件保持：同预算同可见范围普通同效→位置规则不能称独立创新；新约束下独有进度/成本并核前作→可建立机制贡献。§33动态负结论保持。
- [报告](iommu-platform/docs/parent-translation-reuse-boundary-20261008.md)／[raw与有效/排除/旧参考分类](iommu-platform/out/parent-translation-reuse-20261008/README.md)／[关键数学与成本](iommu-platform/out/parent-translation-reuse-20261008/codex-metric-audit.json)／[6原父见证](iommu-platform/out/parent-translation-reuse-20261008/lookahead-witness.json)／[执行与hash](iommu-platform/out/parent-translation-reuse-20261008/stage-evidence.json)。下一从可见范围/总成本公平普通对照开始，不重扫仓库、不重复构建本阶段模型。

## 2026-10-08 §35：前置／后置固定分片完成，普通父翻译复用为下一门槛
- 最新用户“继续”，措辞偏好已写AGENTS：用如果/因为给具体创新条件，不用“尚不能称为独立创新”。Codex亲自设计out前置模块，来自§34固定单父，保留max128拍/原parent256拍/双W owner-seq/原ID与最终RLAST/一child/无新R payload。
- pre在两个真实DMA的AR仲裁之前每流留1原父，child经过TL；post沿用§34译后父，每流8项virtual关联及原TC队列保留。AR cast tr2→tr4后显式owner ID，类型148/98bit，全module保存extended AR，但公共flat harness有扩展字段子集限制。原AW/W/B mux不改，不声称等总资源/单一变量。
- 1新SV+C++构建6.669sec，3仅C++重链接0.792/0.890/0.781sec，均exit0；8新运行S/N×pre/post×V/A先，全exit0/PASS（约0.40–0.61sec）。使用旧post模型archive但新binary，旧SV不重编。最终汇总902条件过，post/S两CSV等§34单父；N四CSV等对应S。
- CONF1/src页偏移0/dst3072/各32KiB、同bank/真实DMA/总FIFO48B，无CPU/诊断逆序/AW交错/预热/DS优先；全部行为DDR读延迟31，非仅PTW/MIG。均32iDMA原父、64physical child、34AW、每流4096R/W、bytes/源/guards/NEXT→DONE/quiet通过。
- TL前98后66，hit64/32，miss均34、DDTW2/PTW102，实际页表读取未减少。V先真实issue64/69，pre done24226/27505、post17535/20218；delta24162/27436→17471/20149，均值25799→18810低27.0902%，双方改善，A先确定性镜像。末DDR W另列24221/27500→17530/20213，不混为DONE；整个6691/7287差不能全归32hit，early translation/AW反馈/入口容量有别。
- Codex补核N window源码303–306要求跨bank，四N FAST0因此未触发。原先预计其作有效hit fastpath对照的判断已撤回，不因同S说快路径不能替代。下一直接试普通父翻译复用，若不足再普通提前准备同总成本。
- 元数据声明前2502/post3568bits，差1066、统计160另计、未综合，不含公共TC/cuts，logical ingress不同。新增pre_parent_ar UNOPTFLAT警告路径已读，局部flow_ready纯寄存state；运行收敛不等lint/综合/完整组合无环验收。未实现异常/失效/epoch/通用公平性。
- 已核官方RISC-V IOMMU IOATC/ATC/ATS，ICCD2018 DMA burst元数据/retry，2025LLC对walk处理；复用/提前准备已有前史，不把本位置速差作首创。条件：普通复用＋提前准备同成本同效→当前规则不能称独立创新；新约束下独有决定/进度/成本且核清前作→可建贡献。动态最长前缀旧负结论保持，不承诺二区。
- DeepSeek3成功high/max393216/stop/nontruncated，观测补丁+汇总草稿+修订，实际provider usage332487。初汇总exit1/24条件失败是两流总var误作单流及other未认，回传实际诊断修正只解析raw无DUT重复；原回复保存。Codex关键审查及八CSV child对真实AR地址/ID/len/offset逐项对齐。76生产SV哈希同，无生产/originaltests/Linux/BIT/SD/板卡动作。
- [报告](iommu-platform/docs/fragment-placement-boundary-20261008.md)／[raw与命令](iommu-platform/out/fragment-placement-20261008/README.md)／[关键审定](iommu-platform/out/fragment-placement-20261008/codex-review.md)／[阶段清点](iommu-platform/out/fragment-placement-20261008/stage-evidence.json)。下一从普通父翻译复用原型开始，不重扫仓库、不重复八项，不先BIT。

## 2026-10-08 §34：父槽容量判别，固定强基线1槽与8槽轨迹相同
- 用户“继续”，先核强固定所需请求保存容量，不重复§33。Codex在out/parent-capacity-20261008独立参数化physical parent深度1/2/4/8；parent AR/offset/returned按新模数，virtual source association与virtual/physicalAW环仍8，原iDMA/TC容量未改。按flow bit2选择，SameBank bit3不变，一次一物理子片、原RLAST恢复、无新payload FIFO。
- 3新SV+C++构建均exit0（6.31/6.32/6.33sec），6新运行1/2/4各V先/A先均exit0/PASS；depth8两份旧§33固定结果复制为显式reused参照，保留原argv和来源，无新8槽构建／运行。480汇总条件通过，6项新CSV逐字节与对应8槽相同；父32/子64、逻辑AR32/AW34、每流4096R/W、完整bytes/guards、NEXT保留ID-DONE/quiet均通过。
- 参数31下所有深度V先末DDR W17530/20213、A先对换，整体20213；这是reset-relative数据端点不是DONE或稳态吞吐。当前1槽无可观测损失，但旧8槽高水位未记录，不作=1推断、通用进度或四DMA/MIG结论。natural驱动强制reset(false)/policy off，仅S，非旧PN解释。
- 元数据98+64+9=171bit每物理父，2流×删除7项少2394；声明5962→3568bits，另观测160；未综合／排除公共FIFO/cuts与保留队列，不宣称整板FF/LUT或算法节省。强固定比原型估计更省，动态主创新立项仍不通过。
- Codex直接核AMD2009 US7480784B2 Fig10：已有IOMMU write-miss/PTW/P2P等待环与独立VC，不能称首次发现IOMMU此类环或开专线为新。非同式双W-prefix覆盖证明，不作专利法律结论。
- 下一Codex受控规格：同CONF1、原父256拍、AW几何、两侧owner信息与48B FIFO／1父槽，比较preTL固定128拍与postTL固定128拍；若差异成立再加普通提前翻译准备同总成本对照。尚未实现／运行，TL请求减少不等于miss/walk少；不直接用§32 CONF0对照或声称独立创新／二区。
- 76生产SV SHA同；无原tests/生产/Linux/BIT/SD/板卡动作；沿用§31 out额度修正，生产C0未应用。DeepSeek3次接口与汇总脚本草案／修订（high/max393216、stop/nontruncated，usage总793120 provider tokens），Codex个人设计、核源/原文、实际执行和最终审定。首稿argv list AttributeError、修订Markdown counts KeyError后修复；Codex额外检查真实同ID父子打印序；没有重跑DUT。
- [判别报告](iommu-platform/docs/parent-capacity-boundary-20261008.md)／[关键审定](iommu-platform/out/parent-capacity-20261008/codex-review.md)／[raw与汇总](iommu-platform/out/parent-capacity-20261008/README.md)／[阶段账本](iommu-platform/out/parent-capacity-20261008/stage-evidence.json)。下一任务从该规格起，不重扫仓库／重复六次运行。

## 2026-10-08 §33：推进到独立创新立项判断，动态规则不作当前主贡献
- 用户“推进到能判断独立创新”。Codex亲自实现out/posttl-independent-20261008/posttl_owner_fragment.sv；系统副本接在完整completion AR译址后，真实virtual source AR offset与物理parent关联，虚/实AW独立owner/seq环和实际virtualW拍进度，原ID/parent RLAST恢复；两流各8父槽、每ID最老优先，可暂停父服务另一ID，一次一物理child，无R payload新增。
- 固定max128beat与动态最长连续双W prepared前缀共用槽/元数据/owner/返回逻辑，同一个newbinary。全completion AR struct保留属性；本例CONF1/cache0/INCR/size3/父256拍，长Non-modifiable允许拆；错误、短不可拆、exclusive、越界等fatal，非production fault/epoch处理。8槽不是最小性或通用进度定理。
- 1实际SV+C++构建exit0、6调用6PASS：31延迟参数固定/动态各V先/A先、100仅V先一对。两路各32KiB/src0/dst3072，均32逻辑AR/34AW；固定64物理源AR/动态55（9父合并）；各流4096R/Wbeat/完整数据、guard、NEXT→DONE、quiet。验收脚本516项通过，真实command/result JSON、每ID父子连续覆盖、两数据端口和片序均核。
- 31参数全体最后DDR W20213/20766，动态晚553，V先V17530→18005/A20213→20766；A先镜像。100全体31805/31737早68，但V27604→27734晚130、A31805→31737早68，两末W平均晚31。firstVAR全68，first→lastW只是数据观察窗，非DONE或稳态吞吐；ptw-latency参数实际作用全部行为DDR读，非只干预walk。请求少14.0625%不是TLB miss或字节带宽收益。
- 配置FIFO两路48B仅payload；模块声明非统计5962bits＋观测160bits（含一些范围检查状态），未综合且不含公共TC/cuts。固定独立实现可裁剪多项扫描，不宣称动态同面积/更省。原76生产SV哈希与§32同，无原tests/production/Linux/BIT/SD/板卡修改；S/DS50/50/串行DDR、无CPU/batch/warm/逆序/AW交错，沿用§31 out C0额度修正，生产C0未改；非四流/MIG/旧PN根因。
- 原文核AXI-REALM透明分片/RLAST、Arm AXI长Non-modifiable规则、Preemptive VC DMA依赖环及TI2025可终止adaptive接口。前作没被证明完全同式；立项否决理由是强固定已覆盖逻辑请求保留、小缓冲完成和本例进度，动态剩余差异收益不稳定、总成本/通用约束未证，不能把较差性能当论文已存在的证明。保留固定为强基线，后续必须给强固定无法兼顾的总资源/进度必要条件或具体新机制，不自动做BIT。
- DeepSeek三次high/max393216、stop/非truncated，provider总658044 tokens；取证/普通验收稿与修订，Codex设计/核源/原文/执行审定。PDF换页符使LF行号与bridge splitlines不同，Codex初次误报引用错误已撤回；验收初次SyntaxError为Codex误截内嵌Markdown围栏，补回完整稿后应用DS字段/分组修订。原答和修订均保留。
- [判定报告](iommu-platform/docs/posttl-independent-verdict-20261008.md)／[关键审定](iommu-platform/out/posttl-independent-20261008/codex-review.md)／[六项验收](iommu-platform/out/posttl-independent-20261008/analysis.json)／[阶段清点](iommu-platform/out/posttl-independent-20261008/stage-evidence.json)。下一不重复这六项或旧172；先把独立差异与总成本条件讲清楚。

## 2026-10-08 §32：普通准确源读门的小缓冲实证
- 用户“继续”。不重跑§31；DeepSeek接口证据与普通准备脚本，Codex亲自设计/实现out/ordinary_copy_owner_gate.sv与owner_actual_dma_top.sv。旧SourceReadGate开关只在latepool且固定4KiB/Depth8/len255，当前Mode0原本惰性；新out Mode0用同SourceReadGateEnable参数选择独立新门，不能称旧开关直接生效。
- 新门逐流仅AR VALID/READY门控，正常AW/W/B/R直接透传；真实normal descriptor＋8槽AW ring宽seq，virtual/physical Whead同owner和相同完成序，physical completion AW已准备，源AR/当前virtualAW偏移/len/size匹配才首次offer，运行held_q保持至握手并手写断言。两流各一个normal static-valid64bit INCR/constant inputID0，不含通用fault/epoch保证。
- 1次新SV+C++build-owner成功，另一次只在§31obj-full-buffer重链接同CONF0 CPP，成功。DeepSeek准备脚本已执行一次，manifest是准备snapshot；Codex后加观察printf，不推进时钟，coupled-metric.patch/最终hash保留。models/owner/relink.conf0.json只作初次build用，没有先canonical再重复RTL构建；optional CONF1命令都未执行。
- 4新运行4PASS：owner-{v,a}-32768 BUF3和full-{v,a}-32768 BUF512，均CONF0/src0/dst3072/两流各32KiB。各路32R/32AW/4096 R-W拍，总64AR/64AW；完整源/目标/guard/真实NEXT-DONE/quiet通过，无payload R反压，other/DS反压208。owner grants64、wait55046是每拍阻止流数累计，不是平均占有。两gate CSV128次virtualAR核当前W owner、got0、偏移和len127匹配。
- reset-relative最后DDR W两启动顺序都owner27894/full25146，差2748；首virtual AR1032/68，首DDR源AR2880/1934。不是独立copy时间窗/平均V-A吞吐，不把全部差归翻译；gate正常反馈改变AW出现顺序。48/8192B只是两路iDMA数据FIFO，gate metadata/公共FIFO/cuts不计，未综合LUT/BRAM面积。
- 创新裁定：普通准确源读门已解除CONF0真实环，不推荐将解除停滞作新贡献。下一先做固定后译址1KiB＋owner选择，保留大logical parent和原RLAST，与动态I同信息/slots/成本比较；先核actual AR flags可拆范围、部分AW由不同parent片拼成的byte offset、同ID oldest、必要owner被同流头填槽/上游FIFO挡住的问题。2parent槽非定理。I/固定post-TL scheduler均未实现；固定同效同成本就停动态主贡献。
- 几何仅复用§31：CONF1大缓冲32AR+34AW=66虚拟Ax，本CONF0=128，不能直接当PTE/miss/收益。重核AXI-REALM原文III-A1，split元数据/RLAST及不支持属性已有，不能把位置/可调拆分称首创；网页未提IOMMU不是无人做证据。
- Codex纠正worker的“只看owner”以及reset分支误读：old latepool:301 if(!rst)，:327-328 release更新在reset，:348才else，运行仅job_fire清零，不是有效runtime sticky。当前新门held在else、旧module未例化/未修改；未推出所有旧运行无效。原mux w_fifo存switch index，port0/1的bit0是stream，IDbit2也stream，BankIdBit3是bank分类。
- 76生产SVhash同，无生产/tests/Linux/BIT/SD/JTAG/板卡改；当前测试重分类两stream同bank，非生产四路512/3混合配置/MIG/旧PN退化。DeepSeek两成功high/thinking/max393216完整返回343532 provider tokens，接口/脚本草案；Codex关键设计RTL、直接核源、执行验收及裁定。无本轮Claude/新增Codex子agent。
- [完整审定](iommu-platform/docs/write-owner-baseline-20261008.md)／[入口](iommu-platform/out/write-owner-baseline-20261008/README.md)／[stage](iommu-platform/out/write-owner-baseline-20261008/stage-evidence.json)／[逐项raw](iommu-platform/out/write-owner-baseline-20261008/runs.json)。下一接此out和后译址PoC约束，不重复本轮4项或旧失败项。

## 2026-10-08 §31：自然同 bank 写序与真实等待环
- 用户“好”，接§30关闭诊断强制逆读/交错。新out独立副本，两个真实iDMA同bank，S/CONF1，源页首/目的偏移3072，2KiB/32KiB，V先/A先；无CPU/预热/batch/read-admit/owner/DS优先。删TEST_COMPLETION_GATE，实际生产banked_ctl；DDR仍单序列行为模型。
- Codex先保存C0源码风险假设；两长任务实际第8个bank AW使3bit外部q=0/overflow1，W被关闭。DDR只观察7 AW，第8项还在下游管线。仅out复制TC加配置容量准入4项，之后无overflow；原生产源仍未修正，工程缺陷不称创新。
- 修正后两长任务仍停。V先例physical AR5657为源80002800/2048B，对应目的81003400..3bff，前1KiB在A首写之前、后1KiB在其后。V下一目的AW5649早已准备；6098 V前写结束，6100开始尾部反压，Whead=A90100c00 got0。失败队列快照确认A根页表读30000在V R尾后；不进一步凭快照指定A哪项逻辑事务。
- 普通CONF0共同边界短读两顺序仍停在错误owner整段R；普通Buffer512两顺序均PASS，各DMA R/W4096拍，34AW/32AR总，完整数据/未写区/排空通过。最后DDR W24411只是轨迹终点，未测新机制吞吐/面积。两路FIFO3=48B、512=8192B，非全部存储。
- 3次SV+C++构建＋2次C++重链接全部exit0；13运行6PASS/7watchdog，其中1次加软件队列快照无RTL变化，CSV与原长V例相同；修正前后短例两CSV也逐字同。failed completed/verified均false。完整[runs](iommu-platform/out/natural-write-order-20261008/runs.json)/[stage](iommu-platform/out/natural-write-order-20261008/stage-evidence.json)。76生产SV前后hash同，无生产/tests/Linux/BIT/SD/JTAG/板卡改变。
- 与生产边界：测试port0/1以BankIdBit3同bank，非生产port0/2 DID10/12；生产同bank512/3，而本轮3/3或512/512。仅两个活跃真DMA，长错位CONF1超出当前冻结4KiB aligned原型，不拿新环解释旧PN。
- Codex独立I修订为源译址后按具体W序选择child边界＋跨ID parent/owner服务选择；原RID/拍数/RLAST与VALID保持。只分片单parent可能仍卡，2parent槽不保证足够；元数据/上游队头/epoch尚未解。未实现，不承诺创新/二区。下一先比较固定后译址1KiB＋同owner调度、共同分段＋准确W-owner门、精确尾部预约/per-ID队列和普通大缓冲，另核页表优先/返回交错能否消环；同效同成本就停动态机制主贡献。
- 补核TI EDMA3原索引段落，独立源/目的分段及TR流水已有；PDF直连失败，不称全篇到手。AXI-REALM split/RLAST及2026 VC已有，不把普通拆分/owner选择包装首创。
- [审定与具体下一步](iommu-platform/docs/natural-write-order-boundary-20261008.md)／[资料入口](iommu-platform/out/natural-write-order-20261008/README.md)。DeepSeek两次driver/观测草案与聚焦修订，high/thinking/max393216完整stop共249657 provider tokens；Codex设计关键假设/机制、直接核源与前作、修正副本、实际构建运行验收。无本轮Claude/新增Codex子agent。下一接此out/报告，不重复本轮13项或§30。

## 2026-10-08 §30：候选 I 的真实几何门槛
- 用户“推进”。Codex先保存独立规格 I：接收原源读后，在源译址成功且尚未发物理 AR 时按合法目的可推进范围选片，恢复原 RLAST。未实现；不能称透明拆分/动态粒度首创。
- DeepSeek首轮取证/草案及聚焦修订，完整 task/answer/raw 保留；两成功 deepseek-flash high/thinking 未截断，总 provider tokens 570972。另一次路径错误被本地预检拒绝，无模型答复。Codex没有增加新子agent或调用Claude。
- 复用 §23 obj-samebank-private 的 RTL archive，独立 out 驱动 C++ 编译1次 exit0；冷手动 V2048B src0/dst3072，CONF0/1两次各exit0/PASS。源R1024+1024 vs2048，目的W1024+1024，数据R/W256，完整字节/源/未写头尾/排空检查通过。
- 两项 W2 physical AW1456，first AR1899，first R1931。目的第二段准备早443/475周期；I在此退化普通长读。末R2217/2186差31，只是普通模式源返回终点差，不是 I 加速或全copy吞吐。
- 旧复用模型 PoolMode0/Buffer512/SameBank1/ReverseReadOrder1，仅启动V；诊断参数不能用于证明自然多DMA。下一实验须使用生产对应bank/ID及自然供给，关闭强制逆读/AW交错，先找部分可推进与整段可推进两种状态；若只在源AR前停滞，单独 I 也不能宣称修复。
- 新核2026-07-01 Preemptive VCs原文；AXI-REALM透明split/RLAST已有。CN114780466B原披露已检到，CN122309411A原文未获，不采二手覆盖结论。工作继续沿共享翻译性能/进度资源主线，创新未成立。
- [机制、结果、文献与淘汰条件](iommu-platform/docs/post-translation-fragment-witness-20261008.md)／[运行与来源](iommu-platform/out/adaptive-fragment-witness-20261008/README.md)。仅新增out/docs及交接；无生产/tests源、SV/Linux/BIT/SD/JTAG/实板变化。

## 2026-10-08 创新反证：两项翻译返回优先对照完成，新机制仍待证

- 用户强调“需要创新”。专用通道/VC、固定页表优先、朴素动态burst降为强旧方法对照；[原文与规则审定](iommu-platform/docs/translation-lane-and-burst-rule-20261008.md)／[报告§29](iommu-platform/docs/composition-history-isolation-20261007.md#29-翻译返回对照与新准入规则反证)。
- root核四DMA mux→IOMMU comp与DS独立发起端口→单DDR服务，DS有独立返回级及50/50延迟器；payload信用池不与PTE共享，单AXI不等于MIG固定FIFO。实际test仅两真DMA，CPU背景流/debug闲置，DMAcfg直连。
- 无重编复用§26binary（sha963cdc5…），仅现成+ds-priority改1。P88989/PN91547，新两项exit0/PASS，V/A各64/DS93/DMA AR256/FAST0/62，暖场旧对照相同。DDR AR→firstR均值/max32、A PTW-active13489相同后PN仍慢2.875%；非端到端翻译相同或实板效果。CPU读221/253是闭环背景流，不能用账本推单因果或带宽饱和。[完整运行](iommu-platform/out/translation-lane-check-20261008/README.md)。
- 本地新增VDC2011、Arm DMA350两PDF/TXT，网页核TaPaSCo/H2RC2019（本地下载503），再核AMD/MASK、CTC/TAPA。动态burst、多引擎读写环、完整读预约＋W数据齐备提交都有前作。
- Codex独立G/H：目的可消费前缀驱动源读边界，并探索仅具体跨copy依赖存在时的最小进展预留；目前未证明区别强普通方法的有效决定/资源保证，未实现。worker以共享PTE/payload信用构造反例与现平台不符，未采。下一先给同资源真实见证，普通切短＋直通同效就停，不先生产RTL/BIT，也不借CONF0旧PN结果证明新候选。
- [Codex审定](iommu-platform/out/translation-lane-check-20261008/codex-review.md)／[阶段清点](iommu-platform/out/translation-lane-check-20261008/stage-evidence.json)。DeepSeek两次thinking/high/max393216/完整stop共340752 tokens；Codex亲自设计、原源/原文核查、实验执行和裁决。新源码/编译/Linux/BIT/板卡0；2新仿真PASS，无Claude/新Codexagent。原日志和全文答案保持。


## 2026-10-08 继续创新探索：准备寿命与共享W承诺，候选未实现

- 用户要求“继续探索创新”。新取得五份公开原文：LTI IHI0089A2020、Intel DSA341204-006US rev3.0 June2025、MetaSys TACO2022、TAPA TRETS2023、XDMA arXiv2025；各PDF→TXT exit0、来源/hash保存。既有Walk Scheduling ISCA2018 §IV重新核。[研究规格](iommu-platform/docs/dependency-contract-innovation-20261008.md)／[报告§28](iommu-platform/docs/composition-history-isolation-20261007.md#28-继续创新筛选依赖信息翻译寿命与写承诺)。
- Codex先独立写E接口信息/F首读首写准备草案，直接核源码/原文后收窄：test实际从iDMA内部导出job/src/dst/len，不是普通AXI桥固有copy合同；LTI支持ordering前请求翻译，SPEC结果不能作事务地址，LC不依赖另一Stall响应且多事务使用须在全球可观察后完成。当前TC地址握手后的COMPLETE不是这套协议。iDMA内部有copy语义，socdma的failed-PFN去重不等于payload依赖。
- 普通准确per-segment byte credit可表达D，普通整对预约/留mate槽/ready队列可处理条件pin反例；不能以“普通能表达”直接断言已有发表同款或否定所有组合创新。宽泛metadata/双端协作/纯译址预取已有强前作。下一窄问题为准备期可丢弃缓存/提示，W首次VALID前复核两端及进度资源，再建立首片段真实在途映射合同；不是已实现新算法。
- 下一只复用真实iDMA/CONF0，比较当前W-head、普通联合准备ready队列、可丢弃预取复核、W完整后AW；一端冷热＋准备后被替换两形状，计全状态/payload/重做/持有寿命及V/A综合指标。同效同成本或重做吞噬收益就停复杂机制；不要求所有负载赢，不预承诺二区。旧D独立分段小判别未执行、优先级后移；不先BIT。
- DeepSeek两次成功high/thinking/max393216，stop/未截断，共366895 provider tokens；另一次错误文件路径在模型前失败无usage。修订仍有credit泄漏/守恒和prefix间隙等错误，Codex按关键并发合同亲自纠正，原答复不覆写。[完整审定与记录](iommu-platform/out/dependency-contract-innovation-20261008/README.md)。
- 本轮生产/test/Linux新代码、测试/仿真/构建/BIT/JTAG/SD/板卡动作均0，最新实际仍§26四次两DMA仿真。没有真实四DMA/新面积或收益证据，无Claude/新Codex子agent。

## 2026-10-08 原论文驱动创新筛选：分段错配的条件候选，未实验

- 用户要求“在阅读论文的情况下探索创新”。本轮对照14篇原论文关键机制，复用既有全文；新增公开iDMA2024、CTC2009、AlphaServer1996三PDF及TXT，[来源/hash](iommu-platform/out/paper-driven-innovation-20261008/papers/sources.json)。[最终审定](iommu-platform/docs/paper-driven-innovation-20261008.md)／[报告§27](iommu-platform/docs/composition-history-isolation-20261007.md#27-原论文驱动创新筛选分段错配与部分返回预约)。
- Codex亲自提出并审定A预翻译后承诺W顺序、B实际阻塞链调度、C按完成头寿命记credit；宽泛方案各有强先行/普通对照，暂不实现。D为源R2048对应目的W1024+1024时，可靠可排空前缀直通、后缀按1024独占预约，保持原逻辑R数据/RLAST。仅条件容量规格，非最低容量定理、面积/性能/创新证明。
- 关键源码：64-bit iDMA虚拟合法化边界2048；frontend conf bit0→decouple_rw；Linux419/test1225,1715写CONF0，当前R/W共同分段。候选示例需CONF bit0=1、src页首/dst偏移3072/len2048，因此不能解释旧CONF0 PN退化。RAWCouplingAvail是独立编译参数。
- 已纠正worker过强判断：无时间进度BFS不能否决性能；已有AR顺序不是不可知未来；MASK token不是completed-head occupancy；合法R反压不保证让PTE越过；p减少须先增加预约，不能无条件收缩；WREADY瞬时值不是独立排空合同。最强C若同样按段预约后缀，D机制等价降级。
- 下一最小判别：复用真实iDMA抓独立R/W分段、证明前缀不会依赖本R完成/后续PTW才排空，比较F整预约、S共同分段短读、C准确普通直通池、D。普通C同决定同资源/成本则停机制贡献，S同吞吐更简单则停长读优势；通过才考虑四真实DMA/综合。当前未执行此判别。
- 本轮生产/test/Linux修改、测试/仿真/构建/BIT/SD/板卡动作均0；最新真实结果仍§26四次RTL仿真。DeepSeek三次high/thinking/max393216完整stop、无截断共448607 provider tokens，仅证据/提案；Codex核原文、独立架构设计与裁决、执行公开资料下载及文档更新。无Claude/新Codex子agent。[完整任务/答复/审计](iommu-platform/out/paper-driven-innovation-20261008/README.md)。

## 2026-10-08 持续推进：翻译完成后的等待定位与四DMA受限容量模型

- 用户问复杂方法与强普通方法同效是否说明其他瓶颈。区分：§25两法CSV相同先说明放行决定等价；PN vs P虽有FAST但总耗时不改善，是另一待诊问题。Codex直接核真实W mux/入口demux/源读准备门，设计内存读准备延迟干预，未把等效实现当瓶颈证据。
- 复用§25 ordinary-virtual SVobj，仅改`tests/nonblock_actual_dma_test.cpp`；默认serial保留旧时序，overlap仅measurement_phase从AR接受计延迟，单独请求首次R仍32cycle，不改返回顺序/数据口速率/VALID保持。每模式warm行逐字一致。原旧二进制冻结为legacy-binary，obj同名二进制现重链接；旧§25日志/CSV保留。[补丁](iommu-platform/out/read-service-boundary-20261008/test-source.patch)／[审定](iommu-platform/out/read-service-boundary-20261008/codex-review.md)。
- 四实际RTL仿真PASS：正常P92109/PN92374；overlap P85339/PN85984，V/A均64、PN FAST62。两法都改善，PN仍慢265/645；串行准备延迟影响绝对性能但不是PN不足的唯一原因。serial完整服务差empty+241/setup−31/transfer−8/bp+63＝265；overlap+443/+128/+64/+10＝645。CPU polling随窗口改变，不能说全部DDR工作量相同，账本差非单模块因果/板上带宽证据。
- DeepSeek轨迹脚本经Codex直接审查后执行：P/PN各V128/A128次capture→arfire→DDR按源/地址/len完全匹配；全为2/3cycles。62 FAST→capture前/后burst各31次1051/1052cycle，→DDR1056/1057。FIFO初始pending0重建无负值、空区间总量4351/4592精确匹配新计数。首FAST2483，Afill2933，A末R3518，V AW等待解除3524，Vcapture3534。flags17的bit0是跨bank W所有权阻塞。**定位后续准备/写顺序约束，尚未通过只改该条件的安全干预独立量化265cycle差。** [轨迹分析](iommu-platform/out/read-service-boundary-20261008/trace-analysis.json)。
- 四生产DMA IDs0/4/8/12，两bank0/2与1/3直接核；完整copy保证和大FIFO仅前两路。已有测试仅两真实iDMA、其余mux端口置零，不把四路实例化当四路N已验证。纠正DeepSeek“TC只有bank无DID”和“same-bank必fault12”两推断；原AR被接收后直到RLAST才退demux读ID，不能直接丢已翻译AR再无状态重放。[接口与审定](iommu-platform/out/four-dma-boundary-20261008/codex-review.md)。
- Codex定义四源/单job/两burst合同，固定W顺序0a,0b,2a,2b,1a,1b,3a,3b，36同源保序读组合；DeepSeek起草BFS脚本，Codex审核实际转换并执行。九策略×36＝324**抽象分析**/34680累计状态，非RTL性能实验。盲目共享H2..4均30/36组合可达停滞，H5为18/36，H6为0；普通每bank2槽共4与普通当前W-bank预留2槽都是0/36。成功路径与全路径进度分开统计。合同把准确R→W派发原子化/假定AW可备，没有实际TC额外FIFO、FAST/PTW/周期/失效/未来job，不是普遍容量定理。[结果](iommu-platform/out/four-dma-boundary-20261008/results.json)／[见证](iommu-platform/out/four-dma-boundary-20261008/traces.json)。
- 创新判断：共享槽普通预留已有等价办法，复杂扫描也无独立决定；不宣布新机制/Q2。下一沿性能/成本主线，先查跨bank AW/W锁与源读供给的具体合法反例、最小安全干预和强普通队列/准备门能否同解；真实四DMA可达性仍待验证，不自动全面扩RTL/BIT/172。
- 本轮1次C++成功编译重链接、0SV重建、4运行4PASS；76生产SV前后同hash，仅1个测试C++变化，无生产/Linux/BIT/SD/板卡动作。[运行](iommu-platform/out/read-service-boundary-20261008/runs.json)／[清点](iommu-platform/out/read-service-boundary-20261008/stage-evidence.json)。DeepSeek三次接口证据/两脚本提案385750 provider tokens，high/thinking/max393216完整返回；Codex亲自设计、诊断、原源审查、执行验收。无Claude/新Codex子agent。


## 2026-10-08 继续：源读准入新条件与最强普通对照，独立创新未成立

- 用户继续推进创新。Codex独立设计：匹配物理AW已准备，且其所有物理前序写的payload已通过virtual WLAST进入TC时，允许当前virtual W头的源读，不再等待physical W头也相同。另设更强普通对照：virtual W头＋准确matching prepared AW，扫描只用于独立fault16见证，不参与该版功能准入。仅tests四源；[关键审定](iommu-platform/out/payload-frontier-poc-20261007/codex-review.md)／[阶段报告](iommu-platform/out/payload-frontier-poc-20261007/README.md)／[差分](iommu-platform/out/payload-frontier-poc-20261007/test-source.patch)。无Claude/新Codex子agent。
- 先两版正常PN校准92374，额外状态0。再加公共DDR夹具：completion 256beat写接受248beat后有限WREADY暂停64cycle，每段一次；三最后模型各P/PN，共6运行。PN严格102686、扫描/普通96267；P101731、扫描/普通95692，V/A每窗64、PN FAST62、写stall16384及写拍65536相同。PN耗时少6.2511%，但最强普通PN仍慢P575cycle/0.6009%，不证明PN领先或板端MIG因果。CPU polling工作量随窗口耗时改变；不能说全DDR请求相同。
- 扫描与普通P、PN两对已记录CSV逐字相同，额外窗口grant96，E2逐tick fault检查包含fault16没有触发。只证明此矩阵；输入demux的W所有权锁和准备门给出条件性解释，未证明普遍AW保序、异常/取消/失效、多job或四DMA进度。virtual WLAST不是DDR写完/B完成。正常iDMA DEFAULT完成等最后B，但job_fire只是frontend→job FIFO接受，不能普遍断言硬件禁止提前排job。
- 统计wait=每周期符合额外准入的源数量之和，含grant那一拍，不是独立请求或纯阻塞周期；raw含warm/排空。Codex纠正DeepSeek修订改变wait定义及pre-edge差分偏一，最终在posedge之后做开闭快照，对应pre-edge事件(open,close]。首次夹具函数名event误写，编译失败后改trace_event。5成功SV构建＋1失败，8仿真全部PASS（2早期代次校准＋6最后模型），不称最后模型默认路径已另回归。[全部运行](iommu-platform/out/payload-frontier-poc-20261007/runs.json)／[清点](iommu-platform/out/payload-frontier-poc-20261007/stage-evidence.json)。字节/排空、read标签fault0/queued0、capture=issued1284、E2接收=回放328704拍。76编译生产SV hash前后同；无生产/Linux/BIT/SD/板卡动作。
- 已核AXI-REALM DATE2024 III-A写数据准备门及2026 AXI VC预印本中的DMA R→W依赖原文；通用ready/消除等待环不是首创依据。完整扫描降工程对照，不继续加复杂度或以6.25%包装独立创新。
- 下一仍原性能/成本主线：先将真实四DMA IDs0/4/8/12、两bank（0/2、1/3）、copy epoch/burst、共享TC头顺序写成明确约束，找普通准确门/共享选择＋旁路/每DMA队列/全响应预约在同容量、同信息下无法同时保住提前翻译和进度的合法反例或非平凡成本结论。此阶段未实现四DMA；简单扩展普通队列不算新贡献，未认证创新或二区水平，不自动做BIT/172。
- DeepSeek三次观测接口/指标草案共378225 provider tokens，high/max393216、完整返回，raw/task/answer保存；DeepSeek没有执行命令。Codex亲自创新设计、关键源/原论文审定、RTL/夹具与测量纠正、编译仿真及最终验收。[使用量](iommu-platform/out/payload-frontier-poc-20261007/model-usage.json)。

## 2026-10-07 继续探索：可排空读段与更小缓冲，E2未实现

- 用户“继续探索”：Codex亲自提出E1共享整job区及同bank队头反例，再收窄E2：精确供给当前write burst的源读可小FIFO流式；其他提前读须在共享AR仲裁选择前预约完整burst返回空间。[独立稿](iommu-platform/out/completion-reserve-exploration-20261007/codex-independent-design.md)／[最终审定](iommu-platform/out/completion-reserve-exploration-20261007/codex-review.md)／[报告§20](iommu-platform/docs/composition-history-isolation-20261007.md#20-继续探索按可排空读段预约保留成本预研候选)。
- E1共享/预约已有CTC/Arm等强前作，不作为主创新；E2只保留成本假设：当前单源burst2048B，候选共享2048+两小FIFO48=2096B vs现两整copy8192B，理论payload少74.41%，未证明LUT/BRAM、速度、最低容量或全状态进度。普通pool+准确owner门可能同式，必须作强对照。单DMA身份不能代替同job不同source/write burst匹配。
- 源码事实：4DMA DID10/12 bank0、11/13 bank1，fullbuffer保证只DID10/11；TC AR FIFO不能越队头；在FIFO头新增job拒绝可能制造等待环，但此轮未复现原RTL/板端bug。现bank guard放同bank非真正owner，是覆盖范围缺口，不能凭推演说已坏。RAWcoupling bypass允许同copy AW/AR重叠，串行job不排除这个状态。
- 既有cold-walk-PN root1295返回32cycles，256beat数据1327→1582，下一层PTE1408→1653等245；普通Q下可有这类等待，CPU亦贡献，不称违规或全差DMA因果。CSV按win_open写，原始spot-check与编码已直接核。
- DeepSeek两次完整high/max393216总509490 provider tokens，整理源/事件与独立反方；Codex核原RTL/原文，纠正其job串行误读、pre-arb与post-VALID反压混淆、in-flight counter当字节credit、私有证书当pool等过度判断。完整raw/task/answer与audit保留，未把模型一致当证据。
- 当前E2未实现/未认证创新。**下一仅接口/状态成本判别**，通过再做2DMA最小PoC，共比私有fullbuffer、普通pool全响应预约、普通pool＋准确owner门、walk/在途，同时看V/A吞吐和局部综合成本。性能主线保持；非所有原语首创亦可能有增量微架构贡献，但须真实证据。无生产/Linux/tests修改、编译、新仿真、BIT、上板或SD动作；实际最新仍上一轮8/8，不自动扩4DMA/板测。

## 2026-10-07 普通准入/翻译后额度对照：8/8，旧C停止首选推荐

- 用户“推进”及澄清C是否没人做/实测差：本轮仍未实现C，二者均不能断言。Codex先独立定义普通owner准入，再因源重遍历补同预热cold walk P/PN，[规格](iommu-platform/out/owner-baseline-20261007/codex-test-spec.md)／[最终审定](iommu-platform/out/owner-baseline-20261007/codex-review.md)／[报告§19](iommu-platform/docs/composition-history-isolation-20261007.md#19-普通准入强对照88通过停止旧c首选推荐)。
- 仅tests actual_dma_top/system_top增加owner_only输入及真实fullbuffer许可掩码，actual_dma_test.cpp增加+owner-only和+read-admit-measure-only、测量阶段与JSON字段。实际DMA4KiB缓冲不改；warm均为原buffer，warm摘要与同mode旧结果逐字一致。owner模式测量后至STOP/quiet保持，额度measure-only只在warm结束后启用，旧默认行为保留。[实际补丁](iommu-platform/out/owner-baseline-20261007/actual-test-changes.patch)。仅一次成功SV测试构建（首elaboration input默认值不支持已改为CPP显式驱动）、随后C++重编，生产SVhash前后同，无生产RTL/Linux、BIT或板卡/SD动作。
- 8/8最终数据/quiet/代理排空/字节账本/CSV完成验收；默认hot PN93487/cold94427精确。owner hot P92898/PN92857，cold P93778/PN93797，FAST0，仍比原P慢。cold已有整walk额度256在同buffer warm下P92334/PN91727，FAST64（V62/A2），V fill1，A/V各64；PN比P快0.657%，A吞吐+0.662%，非新C收益。[验收](iommu-platform/out/owner-baseline-20261007/acceptance.json)／[全部命令](iommu-platform/out/owner-baseline-20261007/runs.json)。
- owner cold先目的PTE0x50010/fill再源PTE0x50000/fill，V fill从1增2；原源先行及翻译后walk均没有源重遍历。源touch顺序影响本bank PLRU，新目的替换源；不是此前跨bank历史隔离bug再现。局部源码/静态失效约束与事件保留在source-rewalk-evidence.json/审定，未加板端观测器。
- 当前停止旧C首选创新推荐，普通walk已在此case保留并发及减少干扰；不等于C全部状态无创新或算法效果失败。下一候选先找到普通walk/在途额度、两端普通准备在同资源/真实约束下失效的合法反例或同效果成本差，当前尚缺；不自动扩矩阵/BIT/172。不以PN略胜P直接确认论文创新。
- DeepSeek接口与两CPP草案3次high/393216、未截断151450+60735+58361=270546 provider tokens；原任务/答复/JSON和初次入口失败保留。Codex规格、关键SV/架构/HOL/缓存机制判断、文献核查、应用/构建/仿真验收。无Claude或新Codex子agent。

## 2026-10-07 C可达性小判别完成：5/5，候选仍未成立

- 用户“继续”后，Codex先独立定义[新冷目的假设与否证条件](iommu-platform/out/c-readiness-check-20261007/codex-hypotheses.md)，亲自核TC AW-first、输入owner、N窗口与实际服务事件；DeepSeek只起草指定夹具/离线解析及一次定向修订。[报告§18](iommu-platform/docs/composition-history-isolation-20261007.md#18-c可达性小判别一般源先行成立独立创新仍未成立)／[Codex最终审定](iommu-platform/out/c-readiness-check-20261007/codex-review.md)。
- 唯一实际代码改动为tests/nonblock_actual_dma_test.cpp新目的PTE/初值、+v-cold-dst开关、测量地址、JSON与必要字节检查，warm不变。复用既有policy/system SV模型，仅C++编译/链接一次；默认hot93487精确复现。新cold四case FIFO P92334/PN94427、DS优先干预P80252/PN79911，A/V各64；全部数据/quiet、代理排空/债务及trace对应通过，[5/5验收](iommu-platform/out/c-readiness-check-20261007/acceptance.json)。未实现C，没有生产RTL/Linux、BIT构建或板卡/SD操作。
- 一般源热/目的冷源先行成立，首V源AR16/26早于唯一目的叶读/fill；但capture非FAST且P同样如此，严格同job FAST冷目的路径未证。PN在V cold-walk时A FAST源读1226/1232先于V根PTE1263，A被V输入W owner挡住；FIFO根PTE服务P32/PN577，DS优先干预PN回32。仅此模型等待链，非旧板端完整根因。DS干预是在模型已接收队列中改变R返回，不能当可直接部署的MIG强基线。
- C不升级主方法，也不称完全否证。下一仅核额外prepare信息能否在同owner/PTW关系、同容量/合法在途下超出普通owner排空优先＋非owner有界额度；无真实反例或普通等效更省就停C主贡献推荐。不得因新夹具自动恢复B、扩矩阵或BIT/172；研究保持多DMA共享IOMMU性能主线。
- DeepSeek三份完整high/393216、未截断285238+74532+133029=492799 provider tokens；完整任务/原JSON/答案、两个格式失败patch、原C++/binary、新日志CSV/命令均在out/c-readiness-check-20261007。Codex修正diff字符串格式与bank诊断名字、应用/执行、关键机制与创新判断；无Claude/新Codex子agent。旧证据未删。

## 2026-10-07 调度与微架构候选：Codex亲自设计／分析（非实验）

- 用户最新明确“设计到创新 和分析这种。codex不能交给deepseek，codex也要做”，根AGENTS已记录：Codex须亲自给方案/推演并核关键证据，DeepSeek只能补充、不能替代；不禁止其独立提案。[报告§17](iommu-platform/docs/composition-history-isolation-20261007.md#17-调度与微架构继续探索codex-独立设计候选仍待判别)。
- Codex在阅读本轮DeepSeek答案之前写成[独立规格](iommu-platform/out/scheduling-microarchitecture-20261007/codex-proposal.md)，随后作[最终审定](iommu-platform/out/scheduling-microarchitecture-20261007/codex-review.md)。C为目的翻译/可执行状态控制有界源数据提前量；D为可执行等待者控制尚未VALID的下一读写段长度。D因普通下游压力方法可等价的风险降为配套/普通对照；C也可能等价于两端预翻译＋普通在途额度，尚无新颖性/性能结论。
- 关键接口：TC AW-first已存在；单TC在物理AW等待中不能默认另有同bank源译址；新增translation-only不能借AW保留W所有权。N window限另一bank，准备槽需要事务标签、完整属性与在途；输入owner与输出mux owner分开。legalizer已有共同边界，buffer逐byte lane无现成占用输出。IOFENCE PR/PW仍TODO，首判限定静态映射正常copy，不称通用进度/失效证明。
- Codex定向核Criticality2013、SMS2012、DASH、MMU-awareDMA2018§IV-C、AXI-REALM2025、Arm DMA-350§5.8.1；ABW2015只机构摘要，NB-Walker正文仍缺。[原源定位](iommu-platform/out/scheduling-microarchitecture-20261007/source-audit.md)。非穷尽检索，不把job元数据/两队列/自适应burst当首次。
- 两DeepSeek调用在用户最新强调前派发，high/393216、完整stop，共413463 provider tokens；完整任务/JSON/答复和[audit](iommu-platform/out/scheduling-microarchitecture-20261007/audit.json)保留。其frontier/ticket/pair/P-N/debt草案存在老请求≠关键、等待自身响应、PTE抵data债务、N状态和job键等问题，未采纳为实现；细节见审定，不直接按原稿写RTL。无新增模型调用/Claude/Codex子agent。
- 本輪仅读源码、研究原源、独立设计与记录，生产RTL/Linux未改、无构建/新仿真/上板/JTAG/SD/网传操作；已有21/21B和板测均为此前结果。下一定向查C合法状态可达与强普通等价；有差异才一次测试侧短概念验证，不扩172/矩阵/BIT，不预设收益或另起大框架。既有脏文件和历史证据保留。

## 2026-10-07 B最小差异判别：21/21通过，普通对照仍更强

- 用户接受“B先最小差异验证再决定RTL”。[报告§16](iommu-platform/docs/composition-history-isolation-20261007.md#16-b最小差异验证普通对照仍更强)／[验收](iommu-platform/out/pte-progress-poc-20261007/acceptance.json)／[比较](iommu-platform/out/pte-progress-poc-20261007/comparison.json)／[关键审定](iommu-platform/out/pte-progress-poc-20261007/codex-review.md)。仅tests新增completion-AR单entry准入代理和运行参数/事件，所有buffer策略同容量；none精确复现FIFO P91878/PN93480及DS优先P79847/PN79484。同PolicyEnable=1模型运行S/N关分区，补齐同条件N；未另编N模型。
- 一次Verilator并行构建、21次确定性短仿真（A8/gap0/V64/warm256/lat31）。同buffer P91874/N151947/PN93487，入口确认92917、逐PTE92362、period64/256/1024为92917/92927/92227、整walk91091、在途额度92362；各窗口V/A完成64。B比PN耗时−1.203%、A吞吐+1.218%，仍比P慢0.531%；页表优先buffer PN79491/B79486只有5周期差。B未超过强普通对照，当前停止主方法推荐；不是所有场景的不可创新证明。
- 真实bank响应握手＋is_read绑定FAST读标签，非FAST/写响应覆盖、AR捕获消费；不能用旧B清新标签。首次VALID前预留额度和债务，阶段切换不撤VALID，返回R不补阶段额度，重置拍禁止受限非豁免新offer。物理AW已承诺/可见同bank宽松排空豁免。代理在TC交出物理AR后，可能新增HOL，非生产完整PREPARED协议；本轮exemptoffers=0，未覆盖复杂豁免、异常、失效或通用活性。
- 21/21原测试PASS及最终归约通过；原逐8字节页校验/quiet和新AXI稳定性监测保留，代理终点捕获=发出、buffer空、债务0。20份相对周期CSV事件计数与窗口一致；逐PTE两组各155完整阶段、38/14实际收费阶段均<=256拍。脚本首次错用绝对周期、LEN列及对照说明，保留失败产物并修订；Codex最终另纠正period查找字段、reset限制范围与实际报告。不因汇总问题重新仿真。
- 证据目录含任务/4份完整DeepSeek回复与元数据、before快照、最终applied.patch/source hashes、21份日志/20份trace、measurements/comparison/acceptance；不要混用first-*失败归约或初版proposal.patch。DeepSeek思考/high默认393216完整stop共441009 provider tokens；它只作者提案，Codex设计/审核、必要修补、实际执行及验收。无Claude/新Codex子agent，生产RTL/Linux/BIT/板卡/SD未改或操作。
- 下一仅保留“具体完成依赖与普通页表优先失效的真实反例”作为研究问题，性能主线不转安全。先明确真实地址路径/强前作缺口再提出机制，不自动展开矩阵/172或FPGA构建。其他已有脏改动、历史结果均保留。

## 2026-10-07 两条具体规则创新查验：不立项，保留B差异

- 用户要求检查是否创新。[报告§15](iommu-platform/docs/composition-history-isolation-20261007.md#15-两条规则的创新查验一条工程对照一条未验证差异)／[Codex关键审定](iommu-platform/out/rule-novelty-check-20261007/codex-novelty-review.md)。A最后一级+AxLEN!=0 N门控判工程对照，不作为主方法；B共享PTE入口确认+按PTE阶段重新计量有明确差异，已核全文未见同式，但增量价值未证，不直接改生产RTL/BIT。不是证明全世界已有逐项相同，也不是因counter/FSM可表达而否决。
- 新原源AXI-REALM2025完整稿§III-A-1/2/3（前身DATE24）与ABU ECRTS2019 §3存于`out/rule-novelty-check-20261007/papers/`。入口budget、burst拆分、write-data-ready均有前史，周期预算不等于PTE阶段补额。MASK §5.3深度用于L2绕行，§5.4为PTE/data优先及配额；Scheduling PTW工作量不等于DMA AxLEN；AuRORA动态rate、MMU-aware DMA元数据/重试近邻保持。
- DeepSeek初版“p4已覆盖B”错误：Codex直接核`dma_research_policy.sv:325–327,340–345,353–354`，p4按返回事件收费、按固定窗口补额。初版旧板测/固定delayer混写、PLRU未执行等旧事实未采用；新隔离和模型数据仍依§3/8/9。定向修订已撤回主要错误，表内“Q>beats进展”号颠倒，最终要核`beats>Q`。原稿与诊断保留、不再循环调用。
- B可有状态选择区别：PTE仍未进展、used=Q且旧数据返回时，周期budget或单纯在途上限可再准入而B继续阻止；只是规则差异，未证明当前256-beat任务可达/性能更好。下一项只核同等协议/接口/资源下强普通方法的选择和价值差异；若继续小模型概念验证先短条件、最终补N，不直接展开矩阵/172或长构建。
- 补核NB-Walker Zenodo v2/AE README/树24项/只读setup，工具配置无PDF，未核完整MMU正文；不能宣布排除最新前作。未克隆或运行该artifact。
- DeepSeek两次思考/high完整stop，97565+133564=231129 provider tokens、最大393216；作者为证据及修订，不能执行。Codex直接核关键源/论文、下载保存两篇公开PDF及整理审定。无Claude/新Codex子agent、生产变更、编译/新仿真/上板/板卡操作。目录内任务、原响应、修订与source证据齐全。

## 2026-10-07 双方独立提出具体规则：候选交付，创新未验收

- 用户要求创建新规则，并明确Codex和DeepSeek都参与提出方案。[报告§14](iommu-platform/docs/composition-history-isolation-20261007.md#14-codex与deepseek分别提出具体规则尚未通过创新预审)；完整草案/任务/原响应在`iommu-platform/out/new-rule-20261007/`，使用时以[Codex关键审定](iommu-platform/out/new-rule-20261007/codex-review.md)为准，原稿保留含未采纳表述。
- DeepSeek规则：S1最后一级MEM_ACCESS/PROC_PTE期间，拒绝另一bank的AxLEN!=0读FAST，走现有REJECT普通回退。不是保存翻译/窗口结束自动N重试；2拍也拦，阶段尚未验证合法叶。需要顶层请求长度字段与正确main_lvl接口；现有guard保留不等于新增等待环已排除。作为工程对照，原创未证。
- Codex规则：准备译址和物理offer拆开，当前PTE跨共同入口握手后才能新offer；每PTE阶段有限旁路beat预算，真实PTE进展才重新计量、旧债务保留，已承诺AW的排空读须单列豁免。规则/元数据/同状态不同选择已给，边界/例外未成为生产完整协议，不可直接照初稿实现。
- **纠正Codex初稿：** AR已接受不等于整读不可抢占；C++519–524可调整跨ID R且既有DS优先已移除模型退化，不能拿2763/2766作为MASK必然失败反例。旧offer/在途读跨阶段，入口顺序不保证MIG服务顺序；walk结束普通分支可能超过Q，撤回无条件全局在途量<=Q。两案尚无强普通方案无法等价实现的证据。
- 原源MASK §5.3–5.4核walk-depth、PTE优先及数据quota；新核AuRORA MICRO2023 §3.4.3的动态request rate，不能把相位/动态限流/控制位置自身作创新。NB-Walker全文仍缺；ASAP应为MICRO2019 Prefetched Address Translation，不与ASPLOS2017 Translation-Triggered Prefetching混写。
- 后续若继续先只在现有真实DMA模型一个gap0条件比较普通PTE优先、固定credit与两条候选，Codex方案消融两个条件，后续性能判断补N同条件；接口不适用/普通方法同等/只关闭N或人工强延迟有效即停止。没有新实现、编译、仿真/板测/JTAG/SD/reboot/172操作。
- DeepSeek-flash thinking/high本轮1次，prompt196289、completion21938、total218227 provider tokens（reasoning18845）、最大393216、stop/未截断；不能执行或修改文件。Codex先保存自提规则后读取其答复，双方共享事实与候选轴，不称完全盲审；Codex实际执行保存、关键源/原论文核查及最终审定。无Claude、新Codex子agent。

## 2026-10-07 四法前作质疑：创新预审不通过，撤回优先推荐

用户明确指出§12各方法均有前作。Codex直接复核MASK §5.4、Scheduling PTW引言/相关工作、RISC-V非叶缓存以及本地BIS与DWS §VII-D（850–864行）。[§13审定](iommu-platform/docs/composition-history-isolation-20261007.md#13-用户指出均有前作撤回创新路线优先推荐)：撤回B“首选”/A优先作为创新路线的推荐；四法是工程入口/待核问题，尚无可立项新点。DWS组合无显著叠加收益也限制泛泛“组合不单调”的贡献，不等于本平台具体链条已有逐项相同前作。

后续先明确真实场景下最强普通方案的具体代价、新方案改变哪一步以及可证伪预测，再做最低成本概念验证；不是要求预审前已有完整硬件正收益，也不凭未取得全文宣布领域缺口。允许扩大PN之外搜索；不为维持§12而改名/堆实验。历史成果与工程证据保留。此轮无新增DeepSeek/Claude/子agent、源码/实验/构建/板卡操作；此前的优先级均为质疑前阶段。

## 2026-10-07 PN其他解法：四个入口、两条有条件候选

- 用户要求探索其他解决方法；[报告§12](iommu-platform/docs/composition-history-isolation-20261007.md#12-其他解法筛选请求作用完成依赖与页表访问削减)保留角色/依赖协调N（优先）、帮助当前owner排空的服务（需先证明新选择机会）；动态burst、小nonleaf/PTEcache或独立BRAM通路为普通对照。没有创新或收益验收，不宣称PN已胜N，仍缺N同条件。
- Codex直接核实际源码：AW本就优先，4KiB/足够缓冲的真实job证书可豁免owner读准入；单owner在输入侧，不是下游所有权的完整描述。N拒绝候选会进入REJECT等待原慢路，不能仅修改accept就声称策略完整。物理COMPLETE暂缓与虚拟AR请求方延迟是不同点；已VALID的请求/响应必须保持。仅改变burst、保留4KiBjob并不自动取消证书，job改2KiB才不满足长度。
- 对既有P/PN轨迹作频数归约：93次DS DDR读，两个非叶PTE地址各31次、四个叶项8/8/8/7；62/93可为小缓存提供工程动机，不是周期节省比例。归约/原文件位置及审定在[review.json](iommu-platform/out/pn-alternatives-20261007/review.json)。没有新实验。
- 复核MASK §5.4、Scheduling PTW、BIS与RISC-V规范的先行限制。arXiv2607.01430由仅摘要推进到原文§III关键机制：封装NoC流类别的链路让出，不能移植成外部AXI撤销VALID/W抢占；不先重构NoC。下一步先在现成真实DMA测试补角色/owner事件关联，找合法选择机会，再对照§9普通PTW感知准入与B；该判别仍未实施，不自动长编译或重跑172。
- DeepSeek两次deepseek-flash/high/393216完整返回，合计312813 provider tokens；[任务与完整答复](iommu-platform/out/pn-alternatives-20261007/)保存。Codex架构审定、实际源/原论文核查及机械归约；首稿虚拟AR/物理出口、时延留痕、换地址分流等混淆未采纳，修订的过严工程/发表判据也纠正。无Claude调用、生产RTL/Linux改动、编译、新仿真/板测/板卡操作；当前isolated BIT/Linux运行状态未由本轮重新检查。

## 2026-10-07 第三项瓶颈解法与“PN胜P/N”创新条件

- 用户提出前两项用工作集调整解决，问第三项共享数据/AW-W依赖的已有办法，随后问PN若领先是否算创新。[审定§11](iommu-platform/docs/composition-history-isolation-20261007.md#11-第三项瓶颈的现成解法与-pn-创新验收条件)。PN领先只是性能证据；允许旧构件组合的小论文，需明确新协调、真实未覆盖约束或有边界的可复用因果结论，并对照适用普通解法。当前没有改工作集、补N对照或确认创新。
- Codex直接核：iDMA coupler已实现首拍R→AW信用，actual RAW_COUPLING_AVAIL=0，无软件启用路径；前端max_llen=0只支持完整单拍deburst，不能承诺当前软件设置32/64拍；banked w_owner_q/w_unfilled_q是单owner+计数而非owner FIFO，现有guard避免R/W返回环，不能单删。AW优先本来已有。页表优先目前仅模型干预；四类办法均为普通基线，选适用项而非大规模全测试。
- 新核Arm AXI4写顺序及不可W交错；不误说协议强制单一在途。2026 arXiv2607.01430 AXI NoC读写流类别分离仅摘要核查，非当前IOMMU已适用方案，未全文核验或实施NoC重构。
- DeepSeek一次完整返回deepseek-flash/high/393216，50280 provider tokens；[完整任务/答复](iommu-platform/out/pn-third-bottleneck-20261007/)。Codex修正worker对owner队列、软件短burst和gap百分比的混淆，关键判断不靠模型共识。没有Claude调用、生产改动、构建、仿真/板卡操作。§9下一最小仿真判别仍待实施。

## 2026-10-07 PN组合收益相关研究：原文对照完成

- 用户问“有相关研究吗”；定向复用旧地图，DeepSeek筛6篇，Codex浏览原始作者/出版资料并核关键段落。[报告§10](iommu-platform/docs/composition-history-isolation-20261007.md#10-pn组合收益与下游干扰的相关研究)、[任务与完整worker回复](iommu-platform/out/pn-related-work-20261007/)。首次受限代理连接失败、无答复；一次授权网络调用成功，deepseek-flash/high/393216，87282 provider tokens、未截断。没有Claude调用。
- MASK §5.4数据与翻译干扰、Golden/Silver/Normal内存队列；bypass含义与本平台N不同。DWS §VII-D/Fig.11原文明确MASK+DWS可能不比单用显著增益，本地作者PDF文本831–864行可核（网页全文超时，检索摘要及作者页可用）。不能写成DWS复现本平台P+N退化。DAC2023 §V简单分区执行时间+14.3%，完整协调方案−12.5%，nw命中率收益不转性能，原因不同于本平台当前同miss的退化。
- Scheduling PTW ISCA2018服务顺序/随机调度负例；Bermuda RTCSA2025真实SMMUv2 DMA干扰、未完整实验缓解方法。NB-Walker MICRO2026议程与作者artifact已核，仍未获全文，不能以non-blocking题名判断N相同/不同。没有证明独创或全领域缺口。
- 本轮只文献证据，不新实验/构建/生产修改/板卡操作；§9现有真实DMA仿真准入判别仍为下一步，未自动实施。P/PN模型没有N同条件，仍不能宣布PN与N持平或保证组合领先。

## 2026-10-07 PN板端小对照完成：18/18，供给节奏改变退化幅度

- 用户“好”授权承接上一阶段板端小对照；[报告§9](iommu-platform/docs/composition-history-isolation-20261007.md#9-同一-bit-与启动下的-v-供给间隔板测)、[验收](iommu-platform/out/pn-board-path-20261007/board-acceptance.json)、[最终归约](iommu-platform/out/pn-board-path-20261007/reduction.json)。现有V9无页表AR/R或PTW独立计数，接口不开放DDR优先/突发长度，因此选择V供给gap扰动，不能称作板端DDR服务优先干预。
- 冻结18例：保留原plan的A8/P2/PN3/gap0/1000/4000/block1–3顺序、warmup256、4KiB拷贝和原校验，只将V测量拷贝4096改64。新`mac_pn_gap_probe.py`复用原case_argv/validator/metrics，未改生产源码。03:52:54–03:55:34 UTC，约2分40秒；run_id `c7bdaec923b3815c`，18/18、failed0、exit0，数据错误/恢复状态/taint均0，72条新增正常dmesg记录、无新增告警。原始目录完整回传`out/pn-board-path-20261007/mac-records/pn-gap64-20261007-formal01/`。
- 同一isolated BIT/boot `d61e7517-2b15-4702-86f2-3f338ddd5345`，内核仍cpn3-clean，FPGA构建/JTAG/SD写入/reboot均0，UART未打开。Linux保持运行。Mac入口`tools/composition-isolated-20261007/mac_pn_gap_probe.py`，沿用`--transport ssh --board-host 192.168.77.2 --credential-script scripts/run_research2_mac.py --ssh-helper tools/composition-clean-20261006/mac_update_reboot.py --expected-kernel 6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty --through-phase explore --out results/pn-gap64-20261007-formal01 --run`。该out已完成，勿重跑覆盖。
- 数值：gap0 P/PN W均值109839.3/123956.7周期，PN V耗时+12.85%、A有效速度−11.39%；gap1000为171739.7/178143.7，+3.73%/−3.85%；gap4000为327404.7/327658.3，+0.08%/+0.03%。A速度先逐样本按A_done*50e6/W计算再平均；W是周期、包括gap。gap0双方A完成64、miss/fill32相同，但不能外推地址/顺序一致。N快速请求64→110–111→5–8，大gap近零差额不能当N同强度生效或修复。
- 下一步：不重复gap矩阵；既有真实DMA仿真中判别“提前翻译与物理数据读准入分离”，A PTW活跃时暂缓新V数据读，保持AXI合法握手及进度，gap0比较P/PN数据正确、V完成时间、A有效速度及N触发。候选尚未实施，生产RTL未改；普通优先/准入不是自动创新，不据此直接大规模构建/重跑172。
- DeepSeek四次完整调用提供接口证据、包装器、报告及定向修订（315273 provider tokens，thinking/high、默认最大预算）。Codex实际执行及关键验收；定向修订引入周期误标毫秒和A速度百分比错误，未采纳，最终数值直接按原始记录核算，原回复/错误/审定均保留。无Claude追加调用。

## 2026-10-07 PN数据路径：真实DMA短段与DDR因果干预完成

- 用户“好”授权继续§7最小复现；[报告§8](iommu-platform/docs/composition-history-isolation-20261007.md#8-真实-dma-小段复现与-ddr-因果对照)、[四组归约](iommu-platform/out/pn-data-path-20261007/comparison.json)、[验收](iommu-platform/out/pn-data-path-20261007/acceptance.json)。已完成；下方“完整DMA尚未执行”为历史阶段。
- 仅修改测试：`tests/test_nonblock_actual_dma.py`增p/pn与独立obj-policy-system；`tests/nonblock_actual_dma_test.cpp`增硬件(open,close]窗口、JSON/CSV及窗口内DS优先模型干预；`tests/nonblock_actual_dma_system_top.sv`只增仿真探针。无生产RTL/Linux变更，无FPGA编译/板测/重载/写卡。10份关键源与isolated BIT快照仍相同。初次探针漏了demux unnamed genblk1层，修正后构建通过；非DUT缺陷。
- 同一PolicyEnable=1模型、DS50/50、plain DDR额外延迟31、A8/gap0/V64/warm256/合成CPU：四组PASS，各A/V64；P/PN同32normal miss、31A fill。基线W91878/93480（+1.744%）；模型DS优先W79847/79484（−0.455%）。PN fast分别62/14，P0；AW所有分类为跨bank W未结束。
- 两基线完整窗口93个DDR DS读地址顺序逐项相同；延迟总和10206/15711，差5505与PTW active差一致。PN事件中V数据读2763先于A叶子PTE2766，后者334周期才返回；P叶子45周期，V数据在填充之后进入。模型干预保同ID顺序、VALID反压稳定，只在窗口应用；不是生产MIG修复/新创新。
- 所有日志/完整CSV/patch/DeepSeek完整JSON保存在out/pn-data-path-20261007。首次建模后直接运行同binary；两次基线记录W一致，后续仅重编C++观测驱动。未全面回归，不新增无关验证。
- 下一步：板端对应页表/数据服务顺序的小反事实。模型含动态合成CPU，非CVA6/Linux/MIG时序等价，未定量解释板上12.74%。不自动增加占LUT的历史诊断线或重跑172，不把相关计数相加。
- DeepSeek负责接口patch和证据报告草案（两次完整未截断）；Codex负责关键窗口/观测、模型干预和实际执行/最终审定；本轮未追加Claude。Mac/板卡仍为前次isolated状态。

## 2026-10-07 PN算法三方审核：尚无确定bug，等待来源进一步收窄

- 用户明确授权当次DeepSeek/Codex/Claude审核实际PN算法；[完整审定与模型回复索引](iommu-platform/out/pn-algorithm-audit-20261007/audit-summary.json)，[报告§7](iommu-platform/docs/composition-history-isolation-20261007.md#7-pn-算法源码审核与定向时序检查)。DeepSeek两次完整回复；Claude首轮思考用满无文字、收窄恢复有文字但截断，补齐demux证据后的聚焦交叉审定完整返回。没有把模型一致当作验证。
- 10份关键RTL与实际isolated BIT构建副本一致；实际AXI demux、单元测试vendor和BIT副本一致。Codex核查发现DeepSeek把编译参数NonblockEnable误当P/PN差异、遗漏已有fast反压测试；DeepSeek修订撤回。Claude的owner过时序列要求跨bank AW在旧W未结束时接受，实际demux 175–177明确禁止、输入SpillAw=0；Codex核直连、aw_sel及W LAST定义后否证，Claude撤回。
- 只新增out目录审计C++驱动，复用21份RTL/header输入未变更的已编译TL模型；52个late-PTE/hot-cold V相位case全部通过，A最多晚1周期、hot V fast早4–8周期、cold V一相位晚1周期，无重复/丢响应或fill。仅单对TL固定内存，不证明完整DMA/DDR上界。首个链接命令误带本机不存在的verilated_threads.o，按既有模型链接项纠正后编译及执行成功。
- 复算已有3组P/PN：W均值7035178→7931659.67，差896481.67；硬件phase和both_pending同增，refill均4095、poll均0；共享IOMMU入口AW反压6796732→7660220，增863488。这些计数重叠，只支持定位，不能相加或推出因果。[归约](iommu-platform/out/pn-algorithm-audit-20261007/window-decomposition.json)。
- 当前结论：没有确认能解释板上12.74%退化的具体算法/实现bug，不保证无bug，不宣称必然架构效应。下一最小工作是复用tests/nonblock_actual_dma*真实iDMA/banked/TL，先接入当前CLI未开放的policy P/PN并对齐板上DS固定50/50，取得少量合法AW/AR/W/PTE/R事件及入口AW停顿原因。当前尚未执行完整路径复现，不自动扩成大矩阵或新调试硬件。
- 此轮无生产RTL/Linux改动，无FPGA编译、新板测、重载或写卡；现有板上BIT和Linux保持此前状态。Codex负责关键审查和实际仿真/数据执行，DeepSeek与Claude提供证据及交叉意见。

## 2026-10-07 替换历史隔离完成：22/22，PN剩余退化仍在

- [结果报告](iommu-platform/docs/composition-history-isolation-20261007.md)；[原始结果与验收](iommu-platform/out/composition-isolated-20261007/board-acceptance.json)。下方“实施中”为本轮早期快照，已由本条更新。
- 生产改动仅IOTLB与TL wrapper：固定ways0/1、2..15隔离命中替换历史，按resident way识别命中归属；full-mask S/N保留旧行为。Linux/SD不变。默认IOTLB25834检查、TL回归、新增真实RTL隔离9232检查通过。仅完成本轮定向22，未重跑172。
- 原生构建exit0但WNS -0.219ns一项失败；沿用一次post-route优化后WNS +0.022ns、WHS +0.051ns，各时序/布线gate通过。整板LUT156186、FF127714、Slice50556/50950；完整板级IO/CDC签核未完成。交付使用`out/composition-isolated-20261007/route-opt/ariane_xilinx.bit`。
- Mac当前BIT `tools/composition-isolated-20261007/ariane_xilinx.bit`，定向入口`tools/composition-isolated-20261007/mac_composition_history.py`，复用`tools/composition-clean-20261006/mac_update_reboot.py`。内核仍`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`，boot_id `d61e7517-2b15-4702-86f2-3f338ddd5345`。JTAG一次，SD零写入；Linux仍从SD启动，Ethernet用于控制及采集；UART启动时短读后关闭。
- Mac原始目录`results/composition-isolated-20261007-boot01`及`results/composition-isolated-20261007-formal01`已完整回传到本轮`mac-records/`。short4+explore18全部通过；22项原validator含meta通过、boot一致、无新内核告警。Linux保持运行。
- A8/gap0三组：旧P/PN A miss5117/6582，新2048/2048；P耗时约7.03M、PN约7.93M周期，新PN仍慢12.67%～12.79%。P、PN绝对耗时约降21%，S/N变化≤0.28%。前后跨BIT/boot，n3仅描述性；不能仅以miss次数差解释剩余退化，也不能认为同miss数即同miss页/顺序/PTW代价。
- 下一最小证据：合法实际DMA/TL事件顺序与等待账，解释约0.90M周期差额；先排查本实现快速通路额外周期、仲裁/响应缓冲，再区分miss对象/次序/代价及下游竞争。尚未执行该项，不自动重开大矩阵或机制实现。论文主线仍为保护关键DMA并保留后台吞吐，需真实场景、强普通基线与明确增量，隔离修复/剩余差额均不直接算创新。
- DeepSeek起草运行/分析脚本、测试和报告；Codex主导关键RTL、修正测试设计判断、应用并实际执行全部构建/板测与验收。完整调用与修订保存在本轮out目录。

## 2026-10-07 替换历史隔离实施中（尚无新 BIT／板测结果）

- 用户要求隔离共享替换历史后比较结果；性能论文方向继续，PLRU隔离仅作工程归因。固定2/14分区，按命中resident way更新所属分区决策节点，S/N full-mask保持原行为；仅生产IOTLB及TL wrapper两文件改变，不改Linux/SD。
- 实施目录：[execution-state.json](iommu-platform/out/composition-isolated-20261007/execution-state.json)，含生产差异、源文件备份、DeepSeek完整提案、22项冻结计划、旧clean基线分析。原IOTLB25834检查、TL回归及新增隔离9232检查通过。新增测试使用真实shared/isolated RTL实例；不把合成初态当作完整DMA可达性。
- 新BIT构建已启动，Vivado2023.1，沿用composition profile和clean的24文件overlay；日志`out/composition-isolated-20261007/fpga-build.log`。最终时序尚未判定。编译期间待机，不重复测试/扫描/编译；必要时沿用一次post-route脚本，禁止放松约束。
- Mac SSH确认原cpn3-clean内核、boot_id `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`、taint0。构建/时序通过后用现成`mac_update_reboot.py`仅重载新BIT，随后`mac_composition_history.py`执行short4+explore18，共22。保留原用例参数/ID/顺序、失败停止，不重跑172。
- clean基线18项通过原validator；A8 gap0，PN比P慢11.22%～11.64%，A miss5117→6582，V miss0，A同窗完成4096。前后比较跨BIT/boot，需S/N及A1对照；未宣称因果成立。
- DeepSeek起草运行/分析/定向测试，Codex设计和审查生产RTL、发现并要求修订测试刺激、应用并实际执行检查。最终板测后还需更新本条、CURRENT_STATUS及平台validation，回传全部原始结果。

## 2026-10-07 中文老师汇报稿完成

用户指定日语稿并要求另写中文版，已保存 [research-progress-presentation-zh-20261007.md](iommu-platform/docs/research-progress-presentation-zh-20261007.md)。保留10页、480秒参考时间、逐页要点/口头稿/结论/证据以及完整附录；口头稿去空白1966字符，未实测时长。35个本地链接均有效，原日语稿全部链接目标保留，并增加日语版对照入口；日语稿本身未改。

结合当前追问，在对应页明确：10月6日clean172是清理历史诊断逻辑后复跑同一矩阵；约12%的P→PN退化来自10月5日两处已知功能缺陷修复后的旧数据，不能宣称全部工程因素排除；共享PLRU四例是合成初态下的局部RTL证据，生产替换逻辑未改、板上退化归因未证。第10页与问答列出先整理最新已有数据，再做合法请求序列及隔离历史对照的计划，不表示该计划已执行。

DeepSeek两次完整返回，负责中文初稿与定向语言修订，共84759 provider tokens，thinking/high及默认393216预算。Codex核对日语稿、关键范围与数字，纠正修订引入的“请求供给”误译成“供电”，并将一个重复匹配限定到目标bullet后应用；原始答复和修订均保存在 [teacher-report目录](iommu-platform/out/teacher-report-20261007/)，[文稿检查记录](iommu-platform/out/teacher-report-20261007/chinese-document-review.json)。无新实验、仿真、性能重算、源码/RTL/Linux修改、编译或板卡操作；未生成PPT/PDF、未创建外部Page或发送老师。

## 2026-10-07 衔接上次老师汇报的日语讲稿完成

用户要求查看上次向老师报告的内容，接续整理本次可报告结果，并明确选择“延续上次格式，整理日语讲稿”。以现有`research-progress-presentation-ja-20261002.md`为基线，保存新稿 [research-progress-presentation-ja-20261007.md](iommu-platform/docs/research-progress-presentation-ja-20261007.md)，旧稿未改。10页/480秒规划、每页幻灯片要点＋日语口头稿＋结论与证据链接，正文口头稿2267字符；附上次待办完成表、实验批次区分、问答与索引。没有声称实际发言时间已测量，也未生成PPT/PDF或发送老师。

叙事直接回答旧稿待办：旧84不是84/84（80成功/2失败/2未运行，Solo修复后独立11项）；后续E2强化64给出有限访问条件下的替换→首miss链，W7/W8各正向5次探测中位696/1483周期；fullstats21显示同req4/miss2群的占有约1408/1101/841周期；method88的p5在V保护匹配的两启动×两方向四格均不敌强基线。随后讲同BIT的S/N/P/PN，旧10月5日A8/Vgap0下P→PN窗口增加11.58%～12.22%，与共享PLRU四例局部仿真的已证事实及未证板上归因分开；下一步为合法请求序列与历史隔离反事实，不宣布已找到创新。

Codex直接核上次稿、E2/fullstats/method88原报告、旧P/PN复算说明及局部RTL/新BIT验收，修正草稿的W定义、100ms观测窗不等于A连续忙100ms、两启动非四启动、局部RTL仿真非实板、百分比区间非从一个百分比变到另一个百分比等；压缩主体、去掉内部字段/中文直译。新clean172只作功能结果，LUT减少及Ethernet/SD/JTAG属于实验基盘，旧内核锁死根因保留在附录/注释。

DeepSeek三次完整调用完成证据提取、日语草稿和定向语言/事实修订，共196292 provider tokens（high/393216预算），完整答复在 [本轮目录](iommu-platform/out/teacher-report-20261007/)。按write-page技能尊重既有仓库目的地与稿件格式，未新建外部Page。Codex完成关键审查/最终编辑与保存；[文档记录](iommu-platform/out/teacher-report-20261007/document-review.json)显示10页、34个本地链接均有效。没有新研究实验、源代码改动、编译、性能重算、Mac/板卡操作或对外发送。

## 2026-10-06 23:38 JST 新裁剪 BIT 冻结172项全部通过

用户明确要求编译后上板实验并确认FPGA已连接。已有裁剪BIT和cpn3-clean正在运行，直接复用同一boot `05f32544-9e7d-4a2b-bc56-9b5dcabb8138`；入场uname正确、taint0、composition_trace=N。沿用Mac `tools/composition-v1-20261004/mac_composition.py --run --transport ssh --through-phase explore`，SSH helper显式使用`tools/composition-clean-20261006/mac_update_reboot.py`，expected-kernel为`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`。独占UART `/dev/cu.usbserial-AV0KRCHT`仅被动采集，没有发送字符。

运行14:10:36–14:38:16 UTC完成，runner exit0；planned/attempted/completed=172、failed=0，short16/noise12/explore144。逐项现成检查problems全空、exit/framed_exit均0、前后taint0、boot_id不变；V及参与的A数据错误0、restore_status0、measurement_complete1，40项solo无A；已有dmesg/UART日志未见新BUG/Oops/WARNING/soft lockup/Bad page/panic。[板测验收](iommu-platform/out/composition-clean-20261006-board/board-acceptance.json)、[执行状态](iommu-platform/out/composition-clean-20261006-board/execution-state.json)。本轮不是性能比较或创新证明；前次旧系统更新软锁死未作根因诊断。

Mac完整原始目录`results/composition-clean-20261006-formal01`及旁边UART日志/exit JSON已全部回传 [mac-records](iommu-platform/out/composition-clean-20261006-board/mac-records/)，包括全部172份raw/parsed record、meta、metrics、stderr、SSH原始帧、manifest/plan/boot、阶段dmesg。未新增哈希/回读/预跑，未改变冻结矩阵、阈值或脚本，未重编/写SD/重载BIT。实验结束Linux保持运行，UART及SSH临时占用由程序关闭。

DeepSeek一次成功恢复调用（deepseek-flash，thinking/high，393216预算，完整39012 tokens）提供现成runner/V9接口及结果字段证据；首次受限网络调用失败已记录。Codex直接核关键入口、执行实验、回传及核对已有结果。worker建议的可选日志省略/hash核对未采用，按用户要求原始日志全部回传、不追加hash。无需更改composition脚本：其bank magic与observer V9 magic不同；实际全172进一步确认当前配套Linux/CLI能运行。全部worker答复保存在本轮目录。

## 2026-10-06 裁剪版完整构建及 Ethernet 更新上板完成

用户要求编译期间Codex待机、取消额外分析/回读/重复验证，全速推进。本轮沿用已有24文件overlay和构建器，完整FPGA构建一次、配套软件增量构建一次。base工具exit0但WNS−0.131ns/setup失败2，未加载该BIT；直接复用ptwfix的post-route Tcl，仅改输入/输出路径，单次物理优化exit0，WNS+0.169ns/WHS+0.038ns、setup/hold/pulse失败均0、route errors0且全部布通。最终整板LUT156215（相对旧165535减少9320）、FF127713、Slice50138/50950；不等于新增完整IO/CDC签核。[本轮验收](iommu-platform/out/composition-clean-20261006/acceptance.json)、[执行状态](iommu-platform/out/composition-clean-20261006/execution-state.json)。用户要求待机后未应用/执行TL新探针草稿或增加实验。

实际交付BIT为`iommu-platform/out/composition-clean-20261006/route-opt/ariane_xilinx.bit`，Mac为`tools/composition-clean-20261006/ariane_xilinx.bit`；同Mac目录有`fw_payload.bin`、`kernel-release`、当前`mac_update_reboot.py`。Linux为`6.5.0-rc2-cpn3-clean-00093-gc0f5e13ea81f-dirty`，payload29363656B。新V9 BIT配套新驱动，不使用旧net4trace作为它的实验软件。

第一次复用SSH更新脚本在60分钟超时，尚未调用JTAG；SSH随后banner超时。独占UART实际读到旧内核PID150 `wc`在`xas_start`/`filemap_get_entry`/`shmem_file_read_iter`软锁死，taint含B/L。按`platform-fetch-update`顺序，`wc -c`位于下载后、SD updater之前；据此恢复旧已验ptwfix BIT一次（JTAGexit0，UART启动至login），没有把此次内核软锁死归因于新BIT，也没有确定内存损坏根因。[首次现场](iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot01/)、[恢复](iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-rescue01/)。

恢复后第二次更新于13:29:20–14:03:54 UTC完成，exit0、34分34秒。实际下载29363656B，`platform-sd-update` PASS写33554432B至`/dev/mmcblk0p1`（4190776B零填充），sync exit0；JTAG1MHz加载新BIT exit0，SSH实际返回cpn3-clean，boot_id从`b6223553-9ad7-46b4-83a1-6de248ab2c97`变为`05f32544-9e7d-4a2b-bc56-9b5dcabb8138`。[成功原始结果](iommu-platform/out/composition-clean-20261006/mac-records/composition-clean-20261006-boot02/result.json)、同目录SSH/Wget/JTAG输出及旁边UART日志均已回传。SD常驻；累计本轮JTAG2次（旧恢复1、新加载1）、确认SD写入1次，实验0项。Linux保持运行、UART/临时HTTP已释放。

没有新增哈希/写后回读或重跑旧172；新BIT未取得172项板测结论，旧172/172验收不迁移。现有工作流继续使用Ethernet和JTAG，首次旧内核锁死根因仍开放，不宣称恢复等于根治。DeepSeek提供既有流程证据/提案，Codex审关键边界、实际编译/上板及验收；编译等待不并行扩展分析。

## 2026-10-06 历史诊断硬件裁剪与 P/PN 四例判别

用户先授权推进 B1 最小因果判别，随后提醒 P/PN 历史工程问题，并明确“历史废弃 debug 观测”是占 LUT 的诊断连线/逻辑。历史四例规格确实已在本文件 2026-10-05 第二次六席五轮阶段记载，此前未执行；本轮沿用而非另造实验。清理不含历史数据、BIT、日志或文献。

### 硬件清理与接口

- 移除 `ariane_xilinx.sv` 中 `IOMMU_HANG_DIAG` USER4 全部跨层 AXI/AMO/LRSC/xbar 采样和计数器、`ariane_peripherals_xilinx.sv` 诊断端口/连线；删除 `hang_snapshot.sv`、专用 XDC、仅服务它的 CDC testbench；`run.tcl` 不再接受诊断宏/约束。
- 当前 composition(mode3、legacy_e3=false) 的 `dma_path_observer` 此前仍实例化，现通过常量 PathEnable=0 裁掉。其他旧 E3/research/nonblock 观测器在 composition 已禁用，不为删除名字而破坏仍引用它们的历史 profile。保留 P4M/基础计数、composition 128 words/首尾 IOTLB 快照和方法本身。
- 仅 Research+P4M、无 Path/E3/Controlled 的裁剪组合报告 observer magic V9(`0x49444d414f425309`)；原组合 magic 不变。Linux `idma_observation_prepare` 接受 V9 的基础/P4M，强制不发 ARM bit26；仍拒绝 V9 的 E3/controlled/queue-pair。旧研究专用入口仍只接受 V8。UAPI/计数布局、DMA/AXI功能路径未改。
- **部署顺序：先构建/更新新 Linux，再加载裁剪 BIT。** 新驱动支持旧 V8；旧驱动不接受 V9。当前只交叉编译了 `idma.o`，未生成新 Image/payload、未更新 SD。历史 reader/decoder 仍可读旧 BIT/日志，并标明已退役硬件。
- [变更与验收](iommu-platform/out/debug-cleanup-20261006/acceptance.json)、[本轮实际生产补丁](iommu-platform/out/debug-cleanup-20261006/production-cleanup.patch)、[24文件 overlay](iommu-platform/out/debug-cleanup-20261006/overlay.json)、源码 hashes/before 副本均已保存。新 overlay 从 ptwfix 清单去掉两项已删 USER4 文件并加入 dma_observer；未来完整构建沿用 `scripts/platform.py --workspace-overlay ... --fpga-stage <新out目录>`，不要直接用旧诊断 overlay 或恢复旧 IOMMU 版本。

### 实际检查与资源

Linux `drivers/misc/idma.o` 交叉编译 exit0；提取实际 prepare/arm 函数的 host 测试7场景通过；P4M Verilator 四配置35+2+36+36=109场景通过；V9 observer+真实composition control联动10场景通过，已删 path 模块不在该联动编译源列表。未运行全板172项、未操作Mac/UART/JTAG/SD。

Vivado2023.1 同 part `xc7k325tffg900-2`、同端口/参数的 observer 独立综合：before PathEnable1 **14420 LUT/9199 FF**，after PathEnable0 **5561 LUT/3222 FF**，差 **8859 LUT/5977 FF**；before 层级中旧 path 子模块9969 LUT。报告见 [before](iommu-platform/out/debug-cleanup-20261006/observer-before-utilization.rpt)/[after](iommu-platform/out/debug-cleanup-20261006/observer-after-utilization.rpt)。这是 out-of-context 模块对照，**不是最终整板资源差，也未包括 USER4 删除收益**。首次受限环境许可证失败记录保留；沿用本文件既有完整许可证地址并获网络执行后两次综合 exit0。未重编整板、未验新BIT时序。

### P/PN 局部因果判别

复用 `tests/test_composition_iotlb.py --history-probe`；只改原测试三文件，实际生产 IOTLB/TL SHA与本轮前相同。通过正常 fill+单 way mask 合成16项有效驻留初态；真实RTL对照测试侧分区局部 PLRU shadow。影子只推进到第一次 victim 分歧，再查询指定一个逻辑页，不继续强行重放整个轨迹。

| 用例 | 实际RTL / shadow替换way | 指定后继页 |
|---|---|---|
| V有效touch后A fill | 8 / 2 | 原way8页 miss / hit |
| V lookup但touch=0 | 2 / 2 | 不查询 |
| V touch与A fill同拍 | 2 / 2（用旧tree） | 拍后tree为0x8b / 0x80 |
| 正例同前缀、改后继页 | 8 / 2 | 原way2页 hit / miss |

4例474次求值0失败，默认回归25834次求值0失败；[验收](iommu-platform/out/b1-causal-20261006/acceptance.json)、[摘要](iommu-platform/out/b1-causal-20261006/iotlb-probe/summary.json)、[143条事件](iommu-platform/out/b1-causal-20261006/iotlb-probe/events.jsonl)。证明共享替换历史能跨容量分区影响受害项，但不证明当前合法TL/DMA轨迹可达，更未解释板上11.58%～12.22%退化；收益方向不是单调的。**普通工程解释得到局部证据，创新仍未确立。** 下一研究缺口是完整请求语义下的首次分歧，不能把四例当新论文机制或直接改生产PLRU。

分工：DeepSeek 完成 harness/依赖证据、测试提案及一次探针修订；Codex 直接核关键源、设计/修改 RTL-Linux 接口、应用并纠正补丁、执行测试/综合并验收。原答复在两个 out 目录保留；初稿补丁格式、no-successor标记、touch残留及 Sv39 VPN 取值已纠正，不把 worker 提议当已执行结果。

## 2026-10-06 推进空间有界审查：旧B1的具体判别与新近邻

[novelty-gate §8](iommu-platform/docs/novelty-gate-20261006.md#8-用户再问推进空间保留一次因果判别不升级创新承诺)保留一次低成本因果核查，不批准新控制器或扩大板测。根复算A8/gap0的P→PN：同2/14份额、V miss0，A miss5117→6582（+28.63%），W+11.58%～12.22%，A完成均4096；W与同窗A速率不是独立证据。加性交互A8随gap符号变化仅为回溯描述，A1/gap0第一block实际为负，纠正worker手算。当前IOTLB与TL源码匹配ptwfix快照；mask选择共用PLRU树、正常/fast触摸共用历史，作为普通解释候选，不宣称根因或新bug。

根核composition_plan提取器与dma_observer，window[3]按A lane done事件累加，没有被V transfers截断。拒绝worker“语义正确后只缺独立boot”“六cell实为八cell”等计划错误，未进行全库扫描或重测。下一步是确认现有仿真能否取得复现该现象的短事件序列，定位首次A替换差异；必要时用独立替换历史作诊断反事实。若普通PLRU/缺陷解释完，工程收尾；若剩具体因果规律，再查强前作并作概念验证。不能把剩余残差、交互统计或复现等同创新。

补入官方DAAT-MCS/ECRTS2026全文，核其有限配置测量＋容忍度选择及明确不主张新原语/预测模型的范围；这既说明系统方法可以成为贡献，也压缩简单自动配置路线。核CVA6-VMRT的PLRU约束，不把mask或私有树当新点。主库SP-IMPact DOI误写`.2`已在catalog/Bib/INDEX更为官方`.5`，历史worker原稿保留；主库123项不重计。新PDF与SHA/网页/复算保存在novelty-gate out的room-*，新增全文不等于全部精读/复现。

DeepSeek一次deepseek-flash、thinking/high、默认393216、完整215658 provider tokens，整理既有证据与算式；Codex执行、关键来源审查及最终裁定。无Claude/新Codex子agent、生产代码修改、编译、仿真、新板测、SD或Mac操作。C2不通过、method88阴性和172功能验收原义保持。

## 2026-10-06 用户要求明确判断是否已有：当前C2描述不通过创新立项

根直接复核ICCD2018 §IV-C、本地idma_channel_coupler、Intel DSA §4.3–4.4、Arm DTI §4.3.6，补入[innovation-deeper §7](iommu-platform/docs/innovation-deeper-20261006.md#7-用户追问这些是否早已有过能否算创新)。裁定基础办法大多已有，“依赖信息改善准入”尚未具体化为有效增量，停止把当前表述当值得投入RTL的机制路线。前作对象/协议不同，未声称精确完整等价；也不因找不到一篇完全同名论文就认定创新。此前“空间”只为待证可能，不能给用户乐观立项暗示。合理非顶级论文允许旧构件组合或新因果结论，不设全场景击败一切方案的门槛，但必须明确真实问题、具体差异和证据。无新DeepSeek/Claude/子agent；本次属于Codex关键证据核查与最终接受决定。无生产修改、构建、新实验或板卡操作；后续不自动重做限速或增加协议锁。网页记录保存在innovation-deeper out下novelty-verdict-sources.json；GitHub master耦合器URL失败，采用实际本地源码证据，没有把失败页当来源。

## 2026-10-06 实际早发写配置与更弱普通准入条件

用户继续问还有没有空间进一步。完成[审定](iommu-platform/docs/innovation-deeper-20261006.md)：根核顶层RAW_COUPLING_AVAIL=0、frontend decouple_aw=0在无coupler时无效、backend AW直通、transport W依赖R，以及serial/banked每bank AW优先，闭合“允许目的物理AW先于源翻译”的源码前缀。未仿真或证明完整系统死锁。CQ IOFENCE PR/PW为TODO；IOTLB失效不等于全局暂停翻译。官方RISC-V规范也不要求增加那个锁，不能人为加约束后据此宣布领域空白。

进一步由Codex提出限定模型充分条件：按实际接收域O+R≤B；物理AW发出前其全部源读已物理发出或数据已有，不再依赖被更新禁止的新翻译/准入。在公平内存、完成通道可服务、无其他网络环、更新只等已发物理请求且允许其合法完成时，可排除所述两类更新等待环；不必先取得两端翻译才发读。这是普通信用/耦合的强对照，不是新颖性验收。单chunk时序代数只说明可重叠，不宣称超过多槽流水预翻译。更新若等整个job、发生fault/其他环则证明不适用。

补核Intel DSA 341204-006US Rev3(2025-06) §3.12–3.13/4.3–4.4/8.2.4，已有双端地址ATS、故障阻塞或部分完成、buffer与在途配额。仅作为近邻，不能直接移植到当前无重放的iDMA或声称证明相同AXI环。C2不升主线；下一真正判据是实际合法场景中普通信用＋匹配coupler＋小表流水预翻译仍保守，而少量新依赖信息能改变准入的例子。尚未找到并核验这个差异，不因继续讨论就算创新。

DeepSeek2次deepseek-flash enabled/high、393216、无截断，共308551 provider tokens；根执行与关键审查。第一稿把decouple_rw=1当必要条件、同拍虚拟推进当同时物理翻译已纠正；第二稿仍把控制器1/2槽当K配对槽的表格未接受。配对可以来自descriptor或顺序，不必新AXItag/锁两个TLBway；取消不要求保留失效项。完整输入/答复、根纠正、10份关键源码窗口/哈希和网页记录在[out记录](iommu-platform/out/innovation-deeper-20261006/README.md)。没有Claude/新Codex子agent、生产修改、测试/仿真/构建/上板/写卡；172与method88保持。cqh提前推进观察未核到regmap，不升级成bug/论文贡献或自动修复任务。

## 2026-10-06 创新再推进：artifact 证据与两端 DMA 完成条件

用户要求“可以再进一步吗”。完成[后续审定](iommu-platform/docs/innovation-followup-20261006.md)，没有把追加模型意见当创新证明。此前503的官方ShareMMU v3必要源码本次206成功，定向提取20文件，没有整包构建／运行。`NetworkInterrupts.cc`实际替换为最大age−5×shareCounter，非论文有限LRU rank公式，且shareCounter非唯一设备数；上轮C1数值例仅对应论文算法。完整DMA/LCAccDevice/消息/Ruby回调链已定向核，源接口至少一端scratchpad、非双外存，所审回失效路径主要是事件／延迟建模，不能外推整个论文错误。

Codex证明普通逐读返回空间预留O+R≤B可在所述服务与排空条件下排除原C2读返回循环；不必全job buffer。当前RTL已AW-before-AR，不能再把写优先立新点。新增条件环是U等已发目的写B，B等W数据，W等源读翻译M，M被U暂停；读信用不解决，但当前iDMA端到端可达性和合法更新作用域尚未核实，当前平台也无假定的global revoke lock。普通逐块两端预翻译＋有效性/取消部分预留是强基线；只分块或换名不新。保留同等缓冲/翻译预留/撤销语义下的并发—资源边界问题，尚未确认机制或启动实现。

DeepSeek3次deepseek-flash enabled/high、requested/effective393216、无截断、stop，共374901 provider tokens，提取证据并窄审；Codex直接核关键论文、规范、原始源码并作推演/裁定。完整输入/原答复/纠正/20文件SHA256/网页证据见[记录](iommu-platform/out/innovation-followup-20261006/README.md)。撤回worker参数顺序疑点、单串行边即环、同/不同VPN作为新信息反事实等；不把单copy推断为只可能一项源/目的/context miss。没有Claude/新Codex子agent、生产改动、新测试/仿真/构建/板卡操作。172和method88原义不变。下一最小证据是实际双端DMA合法序列与普通成对预翻译资源账本，不自动启动昂贵编译。

## 2026-10-06 进一步聚焦创新：具体参数边界与完成性假设

用户强调创新。完成[定向审定](iommu-platform/docs/innovation-focus-20261006.md)，拓展上一轮 A/B，淘汰一般 metadata replay 和宽泛生命周期成本策略的直接创新主张，保留 C1 历史共享与近期收益分离、C2 有限缓冲 DMA 的翻译暂停／排空契约。C2 是优先做语义反证的候选，不是已批准的论文方向；C1 也尚未证明超出加权 AA-LRU/aging/SHARP 的增量。增量论文允许组合旧构件，不设置概念验证之前先取得完整收益的循环门槛。

根直接核 ShareMMU Algorithm1/§IV-C7、SHARP §4.1、MMU-aware DMA 2018 §IV-C、Arm DTI IHI0088G §4.3.6，以及当前 IOTLB 标签/翻译完成链。修正模型的 miss=fault 移植、fan-out乘延迟、DID标签简化、旧guard2当最终修复、AA-LRU平局当严格优势等。有限缓冲依赖环只在具体锁/等待/读写条件下成立，不是 ShareMMU 实现 bug；当前单copy完整buffer可能排除它。

继续读到 artifact 官方元数据：概念 DOI 21543532 当前指向22681559，v3 ZIP132141444字节；只取得约2MiB目录，定位DMAController/ProtectionMemobj等文件。两官方源码区间读取均503，尚无实现锁范围证据、没有下载完整归档/执行仿真。后续应先补这个关键语义；不要只因目录中存在DMA就假定与我们memory-to-memory有限FIFO等价。

DeepSeek4次deepseek-flash enabled/high、默认393216、均无截断，合计499021 provider tokens；Codex执行、关键审阅和最终裁定。[完整输入/答复/来源/纠正](iommu-platform/out/innovation-focus-20261006/README.md)。本轮仅研究资料与交接改动，没有新平台实验/构建/板卡操作，也没有Claude或Codex子agent。旧172与method88结果保持。

## 2026-10-06 实现前创新预审与用户要求继续收窄

用户要求先确认触及研究边界再进入工作，并随后要求继续。完成[创新审查](iommu-platform/docs/novelty-gate-20261006.md)：A页感知提交/B配置选择的宽泛版本均未通过机制实现预审。根直接核TCWS页VTA/LRU调度、CCWS/DAWS源端与预测控制、MASK各动作区别、DWSwalker份额对TLB容量影响，以及NeuMMU的DMA限流/并行度取舍、mNPUsim的突发/计算重叠/共享资源模型。补核两份作者PDF，复用旧全文。最新论文Dead on Arrival仍按预印本，CounterPoint非在线控制。未以没有找到同名方案证明创新。

收窄两张卡：A1局部性与消费者等待（普通EDF/slack+限流可能足够、真实轨迹/限制未知）；B1固定保护P与命中先服务N的组合边界（旧PN/N现象仅线索，须区别一般容量/服务耦合、PLRU实现问题和完成驱动供给，尚无根因）。A1暂缓实现，B1只保留低成本旧证据归因候选，不升级论文主线。进入概念验证前须具体场景、强前作假设不足及可推翻预测；完整收益/留出验证在后，不设置先有实验才准实验的循环门槛。不强制固定被机制改变的PTW负载/miss/实际吞吐。

DeepSeek4次deepseek-flash enabled/high、max393216、无截断，合计552503 provider tokens；原输入/完整返回、根纠正和来源见[out/novelty-gate记录](iommu-platform/out/novelty-gate-20261006/README.md)。Codex实际执行、直接核关键原文/旧policy并作接受决定。已修正first-paper-options的MASK会议信息和旧A优先安排，platform-feasibility强调工程可行性不放行创新。无RTL/Linux/生产脚本改动、编译、新实验、上板或写卡；没有Claude/新Codex子agent。下方“先软件限速曲线”等旧建议受最新预审约束，不自动执行；172与method88结果原义保留。

## 2026-10-06 用户指出限速已做：修正平台判断

上一轮仅审composition172，遗漏P4M及method88，造成从头建立限速基线的错误暗示。P4M pilot21/21含gap0/500/1000/2000；formal60原24在线＋1离线追认，35续跑未见完成证据（不自动恢复旧任务）。method88七方法88/88完成：fixedgap/p1字节/p2请求/p3 miss/p4事件加权/p5实占/p6仅V反馈；p5四组匹配最强基线A增益−5.69/−8.24/−5.15/−4.44%，旧固定A8主线阴性保留。最新172 A-gap=0、当前profile不启用旧policy仍成立，仅限此版本。修订platform-feasibility和first-paper-options：先复用历史证据/强基线、说明新页特征的实质差异，只补缺口，不能换名重做或为p5重新挑条件。DeepSeek一次38695 tokens完整返回，Codex核pilot run、formal audit/statistics及method88两报告；记录在out/platform-feasibility-20261006/throttle-history*。无新实验/测试/构建/板卡操作。

## 2026-10-06 CVA6/IOMMU/iDMA实际可行性审查

[平台审定](iommu-platform/docs/platform-feasibility-20261006.md)和[调用/修正记录](iommu-platform/out/platform-feasibility-20261006/README.md)已保存。A先软件开放后台gap及静态页数，保留冻结172契约；真逐任务控制需新profile接ADMIT，当前role=0禁用，不能称现成。B可离线/批次边界先行；quiet+clear成本必须计入。当前final LUT81.22%/Slice99.60%、WNS0.031ns/WHS0.019ns；五关键RTL与构建快照一致。旧数据三组PN/N差异复算；172仅既有验收，无A控制/新规则未见验证。DeepSeek两个完整证据稿共236718 tokens，Codex直接核源码/构建和计量、保留架构决策。未改RTL/Linux/实验源码、未运行测试/构建/上板/写卡。用户本轮只要求可行性判断，后续建议先做软件基线而不自动启动全面实现。

## 2026-10-06 用户追问两个候选能否突破研究边界

[首篇方案§6](iommu-platform/docs/first-paper-options-20261006.md#6-用户追问这两点能否突破已有研究边界)补入创新条件。A位于软件提交与共享翻译服务交界，B位于观测与配置决策交界；主题交叉本身不新。IOTLB Wall/MASK/Bermuda/KPart及HELM/CounterPoint/Coz已限制宽泛页预算、联合资源管理、组合反噬和指标选策略。可检验路线是正常条件下强普通规则失败，新增信息/约束或方法纠正，未见条件仍有净用途；B作为A证据与解释，非预设两篇。用户要突破但不要求顶刊；不承诺新意或发表。Codex负责本次关键决策，复用已有DeepSeek工作，无新委派/代码/实验。

## 2026-10-06 用户明确博一、无需顶刊：首篇论文选题缩小

- [首篇方案](iommu-platform/docs/first-paper-options-20261006.md)优先A页面工作集感知DMA提交控制，备选B配置适用边界与选择规则；C替换状态耦合留作工程诊断，独立事件账本不立题。原四候选长期保留，排序根据用户新目标调整，不代表已证新颖性。投稿门槛已异步询问尚无回复，暂按正式同行评审、范围可控组织，不指定分区/发表承诺。
- DeepSeek两个独立范围：小候选提取、Bermuda/IOTLB Wall/SP-IMPact/rIOMMU前作边界。MCP均success、deepseek-flash enabled/high、393216、stop不截断，共292480 provider tokens；[原答复/审定](iommu-platform/out/first-paper-options-20261006/README.md)。Codex执行检索和关键原文核读，决定可行规模/实验口径；无Claude或新Codex子agent。
- 未采用worker固定总miss、延迟除后台字节、先做PLRU四例的强制顺序，以及失败自动转测量论文。公平比较固定工作/输入/资源，miss为结果；采用耗时—后台吞吐曲线，记提交前等待和积压。原规格旧“未上板”不覆盖后续功能验收；gap不是在途上限，512拍缓冲不是descriptor深度。
- 近期页数仅为代理，必须超越字节限速、大页、回收复用、ring/批量/队列调整等强对照；代理需要识别实际翻译上下文、源和目的页，不能将DID当通用IOTLB tag。下一步是接口卡和基线权衡图，不先造完整控制器。建议约两周立题冲刺是计划估计；本轮未执行新代码、仿真、编译、板测、轨迹采集或全文下载。

## 2026-10-06 按用户选定句执行：四候选前作—场景—约束—验证矩阵

- 交付[比较文档](iommu-platform/docs/translation-comparison-20261006.md)，含总表、各候选强前作/适用层、应用证据等级、窄问题、同条件基线、成本与反证，及下一阶段产物。①优先立题核查，③先作为支撑；②条件保留但不实施机制；④宽泛双视图/多表示暂缓。不是论文创新或性能验收。
- DeepSeek 两组分别处理①③与②④，deepseek-flash thinking/high，requested/effective393216，stop不截断，CLI均exit0，总715497 provider tokens。Codex实际核关键来源、纠正基线与方法；完整答复/输入/用量见[本轮记录](iommu-platform/out/translation-comparison-20261006/README.md)。未调用Claude或新建Codex子agent。
- 关键纠正：依赖可由运行时廉价提供，需证明普通调度不够，而非要求运行时不能取得；不同翻译层的Hydra/IDYLL/ShareMMU不能直接拼基线；维护成本是结果，不预设相同；trace必须容许请求时序反馈，依赖启发式不自动最优；④按总资源预算比，不固定相同条目数/页大小；worker未收到VIO/VBI等不等于库里缺文献。
- 补核ShadowUpdate机构摘要/作者页与HELM出版社MIM/artifact入口，明确限制②③宽泛主张。出版社全文/Zenodo网页访问未成功，CATS可得相关讲稿不等于正式全文，SPMC仍题录；本轮0新增PDF，主库和此前增补计数不改。下一交付为一条真实应用地址/同步路径卡，再据可用性做①+③闭环收益空间；未开始这些实验，无源码/RTL/Linux、构建或板卡操作，172/172原义保持。

## 2026-10-06 再问是否需补文献：有界覆盖与收敛

[gap-audit-03](iommu-platform/out/shared-translation-literature-20261006/gap-audit-03/README.md)核VIO(OSDI2025)、BaM(ASPLOS2023)、GPUrdma(ROSS2016)、2026修订GPU通信综述、Binoculars(Security2022)及GPUDirect官方文档；四篇PDF/文本取得，GPUrdma旧作者链接重定向HTML，未保存伪PDF。只核题录/摘要及必要关键段，非全部精读。主123项/上一20项不重建；未复现作者实验。DeepSeek覆盖审计一次86896 tokens、high/393216、stop不截断，Codex执行检索/下载/核文档与VIO §3并作决策。关键新限制：GPUDirect所述路径要求1:1映射/直通；NIC-GPU应用存在不能证明共享IOMMU翻译争用。VIO是提交前探查缓冲区/处理缺页的软件近邻。候选及缺口文档已同步。否定worker“必须先有轨迹才准建前作矩阵”等过强门槛；当前开始矩阵，定向补全文、应用路径、语义、测量证据。无RTL/Linux、构建、仿真或板卡操作。

## 2026-10-06 用户询问文献遗漏：有界审计与补存

- 交付[缺口说明](iommu-platform/docs/shared-translation-gaps-20261006.md)和[20项清单/16项PDF](iommu-platform/out/shared-translation-literature-20261006/gap-audit-02/README.md)。主123项库不重建；新4项无正式全文是CATS/Trail/SPMC/Make Every Batch Count，与原19项已收缺全文分开。5项关键段经根审阅，其余不可称全部精读。
- 直接新近邻包含PFM、CounterPoint、SPMC、UVM故障合并、Trail、Dead on Arrival、Virtuoso；经典增补13项。PFM保留VA→PA、在§IV-C重映射PA到硬件地址，依赖离线映射/MDT/重排缓冲，不能因层级不同自动判新；CounterPoint检查带噪观测与微架构模型，不代表最优在线控制。已在四候选文档补反证边界。
- DeepSeek两次目录审计/三篇证据及草稿共212971 provider tokens，无截断、high/393216。Codex执行联网、现成下载器、5项关键原文核验及审定。初次委派错把ECMDQ视作候选4的映射错误、ENQCMD混淆、第二稿不准确行号/过强推论已在ROOT_REVIEW纠正；原答复保留，不整体认证原卡行号。ATLAS文件名HPCA16是届次，不是2016。作者UVM页面SSD错误摘要未采纳。
- DVM/Direct Segments/rIOMMU/MMU-aware DMA原库已有全文；UCP/KPart/GPS/WIC旧地图已有，不重复下载。下一步优先对照PFM/CounterPoint/Criticality/BIS/Coz/ASM/RMM并取SPMC全文；旧19项优先ShadowUpdate/Clover/LATPC/NB-Walker/CATO/HELM。仅文献与记录变化，没有实验、构建或板卡操作。

## 2026-10-06 用户询问可能创新点：四项条件候选

- [候选文档](iommu-platform/docs/translation-opportunities-20261006.md)将宽泛思路收紧为：跨设备消费者依赖指导翻译服务（优先验证）、映射生命周期与共享副本范围、有限观测下的因果可判别性、不同设备翻译表示共存（高风险）。普通batch/页排序归入强基线，不拆成多个创新；只看命中率/按关键性/多命令队列不独立立题。
- DeepSeek基于现有报告/18席记录一次聚合A–F反证；实际294288 tokens、thinking/high、393216预算、stop且未截断。原任务/回复/用量在out/translation-opportunities-20261006；Codex直接重看2018 SIMT-aware PTW调度、MASK、CLAP+migration、ShareMMU、IDYLL，核RISC-V规范允许命令乱序而fence覆盖此前命令。新核2024 ECMDQ原作者邮件已经提出ATS HOL和无关失效并行；CATO作者搜索索引说明是CXL分层内存争用卸载，直接页404未获全文，不能当完整协议证据。
- 第一项必须超过2018同指令分组/SJF、DWS、运行时普通优先级及一般依赖调度；先做同真实轨迹理想收益上限是建议，尚未执行。第二项须真实高复用+高更新同时存在；第三项须证明限定观测下不同动作确有必要且额外观测廉价；第四项不能假定页级翻译免费做页内张量转置。无发表概率或新颖性保证，无源码/硬件变化或新实验，172/172保持原义。

## 2026-10-06 继续收集并深化共享地址翻译线（第二批完成）

用户要求继续大量收集、拓宽加深，未要求硬件操作。完成[深化综述](iommu-platform/docs/shared-translation-deepening-20261006.md)，更新旧专题地图与原目录。新计数123独立工作（104全文/105版本，19题录/摘要）；从72增加51项，另2项旧题录升级全文。新增七组45版本证据卡；42份公开下载、3份复用既有IMPICA/OASIS/vAttention全文。基线72项与原稿保留在expansion-01/baseline。

- [增补记录](iommu-platform/out/shared-translation-literature-20261006/expansion-01/README.md)含13次DeepSeek实际调用（全部393216/high、stop、无截断，实际累计2839782 provider tokens），不代表费用估算。DeepSeek提取/草拟，Codex联网检索、下载提取、审阅应用与关键判定；未调用Claude或新建Codex子agent。
- 新增重点为IOMMU2007/2010经典、DIPTA/Utopia/VBI/DPT/FS-HPT/EMT结构线、CLAP/SoftWalker、Hydra/FPR、Soar/Alto/PACT、RDMA ODP/TeRM/ODRP与CXL NDP。原G11伪造SoftWalker被cuPTW引用，经EfixG11与原文核查改为机制对比；调度优先级同步纠正。CLAP已经评估迁移扩展、Hydra是页表级sharer、EMT不面向IOMMU DMA，不能据遗漏宣称新意。
- Codex收紧最终问题表的否定条件：强前作/简单基线已解决、真实负载缺少对应组合、或理想策略也无额外收益，才削弱新课题必要性；开销大本身不是停止理由。明确设备私有ATC与共享主机翻译服务。文档新增七脉络、六组对照、20篇必读；仍未接受任何论文创新或性能收益主张。
- 目录生成exit0，123条CSV/BibTeX、105PDF、版本/证据卡归并、781本地跳转与20条必读核过。导航检查初次仅因检查自己的输出文件发生在创建前而报缺失，后直接核文件并保留初次收据。无论文复现或代码测试。元数据DOI仅声称被实际来源核到的部分；CDFD/LIBRA作者PDF404，HELM摘要，SagePTE公开日期2026-11-04，仍19项非全文。
- 当前根关键检查24项（本轮增12项），不等于104篇全文逐字审核。源码、RTL/Linux、硬件与172/172原功能验收均未变化。下一步按真实应用/协议语义/强适用对手查证具体问题，不据综述自动转安全、启动编译或实验。

## 2026-10-06 共享地址翻译专题：72项文献、经典—前沿—瓶颈地图

- 用户在质疑“多设备共享翻译是否已成熟”后，明确要求大量收集经典和最新前沿并掌握领域瓶颈。本次转为专题证据收集，没有继续启动新的模型创新委员会或硬件实验。
- [专题地图](iommu-platform/docs/shared-translation-literature-20261006.md)覆盖6条脉络和6类问题关系；[文献库](iommu-platform/out/shared-translation-literature-20261006/README.md)含72项独立工作、59项全文／60版本及13项题录／摘要。STAR预印本与正式稿归并；出版状态为67已发表、3已接收待开会、2预印本。CSV、BibTeX、JSON与PDF均可导航。
- DeepSeek（deepseek-flash，thinking/high，预算393216）完成8组60版本全文证据卡，另给范围清单与目录工具。全部12次实际调用答复完整，包含目录草案精简及一次报错修复；原始答复和用量见audit.json。Codex直接检索来源、核12项关键原文、纠正版本与元数据、审阅应用工具并验收。本专题未调用Claude或新增Codex子agent。
- 关键判断：基础共享、借容量、队列感知转发、动态放置和定向失效均有强前作；应查具体应用中共享／局部性／均衡／更新／服务保证的冲突，不能把某论文未覆盖当全领域空白。HDPAT排除迁移，WaferTrans存在性元数据与完整PTE语义不同；F&S有严格保护及尾延迟，MGvm和Trans-FW已有自适应，均已据原文收紧说法。详见ROOT_REVIEW.md。
- 最终目录生成exit0；72条CSV、72条BibTeX、60份PDF、全文卡覆盖与版本唯一性通过，403个本地链接无缺失。该检查不等于论文复现。尚待补全文优先项：ShadowUpdate、CLAP、LATPC、Clover、NB-Walker、CATO、LAMP。未修改生产代码／RTL／Linux，未做新仿真、构建、上板或SD操作，既有172/172验收不变。

## 2026-10-06 六方向三模型18席完成：26项候选与36份正式答复

- 用户要求Codex、Claude、DeepSeek各6席，每模型每方向一席，先学习共同地图再寻找创新并交叉审核。已完成18份独立R1和18份R2；每席审其他两模型同方向提案并跨方向查漏。不是沿用旧六席五轮，也不是18种新机制。完整[记录索引](iommu-platform/out/innovation-council18-20261005/README.md)、[审计清单](iommu-platform/out/innovation-council18-20261005/audit-manifest.json)、[根审定](iommu-platform/docs/innovation-council18-20261006.md)。DeepSeek D1另整理26项原候选台账，不增加席位或轮次；台账保留后来纠正的原主张，最终去留看根审定。
- Claude原6份R1到16384上限，均作聚焦恢复/补答；A4–A6随后HTTP400，经一次分类诊断确认账户余额问题。用户要求保持18席并回复“充值了”后，剩余提案及全部Claude R2成功。32768任务覆盖曾被尝试，但回到16384也失败，不能归因于扩大预算；临时覆盖已移除、全局未改。49份逐席API尝试记录+1份诊断，其中31份有provider用量（研究30、台账1），失败记录保留。所有成功DeepSeek维持enabled/high及393216，Claude成功调用沿用adaptive/high/16384；Codex用量未返回，不估算费用。
- 新的决定性近邻：DVM已有严格验证下并行预读；IDYLL原文允许hash误报目录；Quest/ShadowKV已有KV选择/粒度/回取分析；FPT/Griffin已有按页在途追踪/选择性排空。Codex定向核原文并纠正审核中的non-ATS迟到请求、量纲、重叠时间、有限容量与硬界、扩大失效对照等错误。ShadowUpdate只到摘要、Intel wait仅定向条款，其他阅读范围在各席来源表。Tiara主要周期模型/推导吞吐/分析基线已修正进地图，原地图快照保留。
- **尚未验收任何论文创新。** 淘汰具体重叠机制和错误定律，保留可判别问题；不能由缺证据推全领域无价值，也不以当前FPGA不支持GPU/ATS淘汰现实问题。下一步建议三份证据卡：D3/A6控制命令与第三方进度；C4搬运批次与真实消费者；C5观测信息是否及时。每项先查真实应用、适用强对手和停止条件，再决定机制/平台。A5先对照直接近邻，C1/C2/C6等保留明确证据缺口。后续审计和轨迹采集尚未执行。
- Codex六席可浏览，外部API席只有给定文件；部分Codex R2能回应较早API R2，未做完全对称再回审，不能据此评模型优劣或称18方共识。DeepSeek/Claude提出和交叉质疑，Codex直接核关键证据并作最终取舍。无RTL/Linux修改、新实验/仿真/构建/上板/写卡；旧172/172功能验收保留，论文收益未证。

## 2026-10-05 经典与前沿研究地图完成：先摸清问题，再选题

- 用户最新要求是“先把这一块地图摸透，经典的和现在前沿的，这个领域卡在哪里”。据此把发散候选暂存，完成 [研究地图](iommu-platform/docs/research-landscape-20261005.md)：六条研究线、演变脉络、六个跨论文瓶颈、36篇代表论文与强基线、项目位置和未覆盖范围。此前“动态零散内存优先”不再生效；不据地图自动转安全或启动实现。
- 原始资料为36篇独立论文的38份全文版本、4官方文档，另4项局部网页来源。地图阶段6次成功 DeepSeek 调用（deepseek-flash，预算393216，未截断），加此前已完成但未作为证据采用的3份发散提案；完整答复/任务/用量在 [证据包](iommu-platform/out/research-landscape-20261005/README.md) 与 audit.json。OASIS提取文本55个NUL导致一次本地输入拒绝，清理副本后一次恢复成功；原件保留。没有新Claude或Codex子代理调用。
- Codex直接点验KPart后台缓存与带宽、Page Walk Stealing资源对照、HDPAT排除迁移/失效、ShareMMU权限跟踪与写回、IDYLL实物剖析/模拟方案、ObservUVM采样机制及单GPU范围、vAttention2025、Revelator2026v2、NDS/BypassD平台与RISC-V队列规范。主图采用审定边界，原worker错误保留并有纠正记录，不把模型共识当证明。
- 新认识是机制已有大量先行工作，困难集中在粒度、共享利用率/可预测性、动态更新成本、信息可见性、布局/依赖与物理扩展；这是综合判断，未证明全领域空白或不可能定理。CXL完整放置/一致性、编译器布局/稀疏格式、RDMA注册/撤销及更广实时界仍需专项文献覆盖。
- 下一步按少量真实应用的需求和条件，查最近方法依赖了什么、适用强对手还能做到什么，再决定具体研究命题及必要实验。本轮仅文献和文档，无源码修改、新实验/测试/构建/板测/写卡；172/172功能验收保持，不增加性能或创新结论。

## 2026-10-05 跳出当前 P/N 的开放创新搜索

- 用户指出“都是普通方案，怎么找创新，别被束缚住”。根 Codex 将论文探索优先于旧四用例诊断；保留现有工程/证据，不把已经投入的平台当选题边界。允许跨 DMA、运行时、内存管理与应用接口。
- 实际两次 DeepSeek 独立调用，deepseek-flash，thinking/high，requested/effective 均 393216，未截断，API 总用量分别 21976/24027；任务与完整 JSON/正文在 `iommu-platform/out/innovation-search-20261005/`。本次未调用 Claude 或 Codex 子代理，不是新一轮六席五轮。
- 根直接核查 rIOMMU、ShareMMU、BypassD、DX100、SPARTA 及相关查表调度文献。排除普通队列分离/标签/缓存提示被当作创新；查阅 composition 规格第41行与官方规范，纠正两份提案把实验配置 quiet 泛化为所有规范失效必须全局静默的错误。E3 时间差也不证明有“过期翻译”。原回复保留，修正在[审定文件第7节](iommu-platform/docs/performance-council-decision-20261005.md#7-开放创新搜索问题先于当前实现)。
- 优先候选为零散动态映射内存上的稀疏访存加速：DX100 §3.6 明确大页及应用生命周期一次载入翻译的前提，可据此核查有实际需求的不同条件。普通非阻塞MMU/合并/预翻译/重排及大页内存池均必须作为强对手。其他发散问题为跨设备布局视图和取消后细粒度资源回收；都未确立空白、收益或论文价值。
- 下一步先交应用证据、最近工作差异表与最小性能空间评估；旧四用例作为独立工程待办，不自动先执行。没有生产代码、实验、测试、构建、上板、SD操作，172/172既有功能验收不变。DeepSeek起草候选，Codex核原始证据并作研究裁决。

## 2026-10-05 第二次六席五轮：性能主线与四用例下一交付

- 用户要求“再一次探讨”，并明确回复“沿用六席五轮，授权 Claude 接收这些材料”。本次Codex2、DeepSeek3、Claude1实际完成5轮/30份正式答复，独立提案→交叉质疑→判别设计→论文/执行性审查→最终计划；根协调不计额外席。[记录索引](iommu-platform/out/performance-council-20261005/README.md) · [30份答复/22份API返回清单](iommu-platform/out/performance-council-20261005/audit-manifest.json)。Claude R1空正文经一次缩小范围恢复，R3结尾截断经一次聚焦续答完成；原始回复保留，不增加轮数。本次Claude授权随本轮议题完成，不自动续调。
- [最终审定](iommu-platform/docs/performance-council-decision-20261005.md)：继续有限在途DMA的翻译保护/命中绕过与实际进度问题，H1共享替换历史先作低成本诊断，H2具体等待链按信息缺口续查。已有E2/E3、172功能验收、PN/N条件性反例保留；没有已验证论文创新。KPart原文已明显重叠宽泛“容量保护增加后台压力”的论点，实际应用轨迹、共享PLRU精确近邻、Page Walk Stealing全文仍有缺口。
- 关键源码新增约束：mask不隔离PLRU祖先更新；accepted fast确会touch，正常双bank生命周期响应接收局部界为1–2拍；mux按burst释放、batch为完成后gap，没有已证跨流job锁或周期级开环。根直接点验关键源码、测试边界和KPart原文，并机械复算A8/A16反例；总miss、fast数、整窗1:1均不直接定位根因。
- 下一项交付已具体化：复用composition-IOTLB骨架，在测试层启用显式touch并加入分区更新shadow，运行隔拍touch、touch=0、同拍fill、反向后继页四例；预期8/2与后继miss/hit等，[C1规格](iommu-platform/out/performance-council-20261005/round5/C1.md)。真实RTL、shadow模型、合成初态、尚未验平台/性能必须分列。旧测试TouchEnable=0；coupled默认深度3、可显式512但无真实iDMA/XBAR/DS仲裁/DDR，不原样当板上证明。后续按未知选wrapper或最小实物观察，不要求先建全板仿真。
- 本次只完成讨论和规格，新增定向测试、模型执行、生产代码、BIT编译、板测与SD操作均未发生。DeepSeek提供数据/机制审查及执行计划草案，Claude提出并修订竞争解释，Codex直接核查关键证据、执行工具并作最终验收。原稿残留的“V不变即无收益”“尾段即伪差”等不被审定计划采纳；主指标为V窗口与A同窗有效率。

## 2026-10-05 用户质疑突然转向；性能主线优先级纠正

- 用户追问“既然当前性能的现有成果为什么突然转向？”根Codex重审后承认：先前将P/N/PN等具体提案未证新颖性，扩大成接口研究应优先于性能研究，缺少证据和路线比较。已有E3机制证据、N局部收益、PN退化及gap效应仍是性能研究的直接基础。
- [最新计划](iommu-platform/docs/paper-council-decision-20261005.md)已修订为继续多DMA共享IOMMU性能干扰与缓解主线：先明确PN退化/gap效应的竞争解释，再选最小可区分对照；安全复用接口候选降为备选。具体论文方法仍待证明，不默认将PN组合当创新，也不默认重编BIT或重跑172。
- 五轮原始意见、30份答复和调用记录均保留。本次是根Codex对关键研究优先级的直接纠正，不是新一轮共识；未调用DeepSeek、Claude或子agent，未新增分析实验、模型、构建、板测或SD操作。下方“接口首选”是先前决策历史。

## 2026-10-05 六席五轮论文讨论完成；只交付研究计划

- 用户指定两个Codex、三个DeepSeek、一个Claude席位，五轮交叉讨论，并要求不局限既有线路、以论文发表潜力为主。已实际完成独立提案、交叉质疑、回应重构、模拟拒稿和最终计划，每轮六份、合计30份完整答复。[全记录](iommu-platform/out/paper-council-20261005/README.md) · [机器清单](iommu-platform/out/paper-council-20261005/audit-manifest.json)。根Codex只协调/执行/验收，不算第七席；外部API席位没有命令和写文件能力。
- 决定：尚无正式论文主线过门；优先交付一页真实接口合同，审查IOMMU、互连与软件如何支持DMA内存安全复用，再按非平凡差异决定小模型。备选为现有172的回溯诊断，不能称独立预注册确认，不以拟合/残差直接判根因。[最终决定与顺序](iommu-platform/docs/paper-council-decision-20261005.md)。D1偏向先分析旧数据，其余最终答复优先接口探索；保留分歧，不以投票证明创新。
- 直接核源码纠正了私有512拍缓冲、四次AXI地址服务、fast accept必为合格hit、A8源/目标双足迹等事实。规范/原始文献核查确认官方参考模型已有PR/PW同步hook；B响应不能未经平台证明等同全局可见，同一IOFENCE不能按局部撤销缩小义务。本地PR/PW TODO未据此升级为已发现漏洞；172也未覆盖动态撤销安全。
- DeepSeek D1编写的只读分析由根Codex审查并实际执行exit0；计划草案主要由D3提供，根Codex修订了信息不足、强基线、路线停止条件和后续授权边界。没有新探针、小模型、仿真、构建、板测、写卡或生产源码修改；已有172/172验收保留。
- Anthropic初次外发被自动审批拒绝后暂停；用户随后明确“授权本次5轮发送这些材料”才执行。Claude R1触及输出上限，用一次聚焦续答完成；C1 R2服务容量错误恢复同席位，均不增加轮数。原始回复和恢复记录保留。此次Claude授权随五轮议题结束，后续按既有DeepSeek默认/Codex必要关键审查分工，不自动新增Claude调用。

## 2026-10-05 composition新BIT纯SSH实验第53项停止，UART接回后进行JTAG恢复

- 用户确认03:06时序通过的route-opt BIT，沿用net3/原SD；实验启动boot_id为`29d710b2-4949-402d-8055-8734cbaa5220`。冻结172一次执行，short16/noise12/explore24完成，共52；第53项`explore-b1-g1000-a8-mpn`失败，119未运行。不是172完成或收益结论，不自动重跑/续跑。[原始汇总](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/summary.json) · [结果记录](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/RESULTS.md)。
- 失败case CLI timeout参数15秒，SSH命令300秒超时；未得到JSON/exit/boot.after/taint.after，`boot id changed: None`是读取缺失，不能推断重启。Mac en11/192.168.77.1仍active、100Mbps全双工，ping2/0、新建TCP22连接5秒超时。
- 用户接回UART后，原端口`/dev/cu.usbserial-AV0KRCHT`/115200无占用；被动3秒、换行后8秒、身份命令8秒累计0字节。重新接线不能找回失联之前的输出。[故障记录](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-01/uart-fault/identity-result.json)。
- Codex直接核对驱动：batch启动在关中断后读NEXT_ID；poll/drain/capture/restore的timeout依赖MMIO返回。硬件访问不返回是候选，未证实为本次原因；没有UART故障前日志、CPU PC或RTL现场状态，不能写成已确认死锁。实际交付内核`CONFIG_MAGIC_SYSRQ`未开启，本轮不改内核或RTL。
- 现场已保存后，复用既有SerialConsole/OpenFPGALoader/认证模块，仅JTAG重载同一BIT；exit0，net3/SSH恢复，boot_id `476dcf54-3dfb-441f-b95c-3440589736f5`、taint0、carrier1，UART启动日志15838字节。累计JTAG2、SD0、科学尝试53；恢复不重跑实验。[恢复结果](iommu-platform/out/composition-board-net3-20261005/mac-records/results/composition-routeopt-net3-20261005-03-rescue/result.json)及启动日志已完整回传，状态以[execution-state.json](iommu-platform/out/composition-board-net3-20261005/execution-state.json)为准。
- 当前开发实验建议保留UART记录，日常传输/操作继续SSH；纯SSH科学runner不会自动采集UART。DeepSeek核对部分结果、定位源码并起草记录；Codex检查关键源码、执行现场诊断与JTAG恢复。

## 2026-10-05 既有拷贝与 FPGA 上板流程已核对

用户要求先掌握现成流程，不再造轮子。已只读复制 Mac 指定部署源码/说明到 Dell 独立 `iommu-platform/out/deployment-workflow-review-20261005/mac-snapshot/`，由 DeepSeek 梳理调用与证据并定向修订；Codex 直接核对写卡身份/分区写入、JTAG 选择和调用、UART 占用、实际 Mac 交付位置，补 [当前入口地图](MAC_FPGA_HANDOFF.md)，旧首次上板正文保留为历史。完整 worker JSON/Markdown 与存在性证据在该独立目录，不把未修订原稿当最终结论。

既有链条为 Dell 编译交付→Mac 现成 fetch/SSH/SCP→需要时写 SD→先 UART 采集再指定 BIT 一次 JTAG→释放临时串口后 attach 已有 runner→完整 raw 回 Dell。写卡脚本与通用 FPGA 入口默认历史版本；新版 release 用 `write_research_sd.py --release software-update-20261004`，只写第一分区且已完成，本次不再写。四模式 `mac_composition.py` 只 attach，旧 nonblock/method88 入口有版本固定限制。默认 1MHz 有历史恢复记录，不擅调快/自动重试。最新速度偏好覆盖旧额外 hash/readback/独立验收清单，已有身份/退出码/首错停止保留。

Mac 本次只读检查 `scripts/mac_update_reboot.py`、`artifacts/software/software-update-20261004/delivery/mac_update_reboot.py`、`tools/composition-v1-20261004/ariane_xilinx.bit`、`tools/composition-v1-20261004/kernel-release` 均不存在；这仅表示这些位置尚未交付，不判断 Dell 构建状态或所有 Mac BIT。新版 helper 在 Dell 交付中可单独复制，无需新造脚本。本次没有写卡、加载 BIT、打开 UART、启动/重启、测试或重查构建，cpn2 板端 SD/网口仍待实际启用。

## 2026-10-05 SSH Mac 写新版 Linux 到 SD：已完成并安全推出

用户授权直接 SSH Mac 工作区 `/Users/jojo/Desktop/在mac链接pfga(codex)` 写入已插入的 SD，并明确要求极致部署速度、复用现成脚本、取消额外下载哈希/写后回读/测试。Codex 直接复用 `scripts/write_research_sd.py`，仅追加 `software-update-20261004` 版本选项；没有新造写卡工具，也没有调用 DeepSeek。本次执行不改变此前软件实现的作者分工。

新版 payload 已传入 Mac `artifacts/software/software-update-20261004/delivery/fw_payload.bin`（29363656 字节，内核 `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty`）。现有脚本核对目标卡和原始启动分区后，写入 `/dev/rdisk5s1` 的 32 MiB 分区并零填充，完成 fsync/sync、安全推出；退出码 0，最终状态 `written_not_readback_verified`。其他分区未写入。日志实际输出卸载成功、`Disk /dev/disk5 ejected` 和成功状态；没有新增下载哈希、写后回读或额外测试。

Mac 原脚本结果目录 `results/sd/software-update-20261004-ba7hduy9`；启动日志和回执 `results/sd-install/software-update-20261005T003732`，`MAC_HANDOFF.md` 已更新。Dell 已保存[原始回执](iommu-platform/out/sd-install-20261005/receipt.json)、[终端日志](iommu-platform/out/sd-install-20261005/terminal.log)和退出码。SD 已可拔下插回 FPGA；本次未加载 BIT、启动 Linux 或验证板端 SD/网口。原任务继续管理 FPGA 构建，当前构建状态没有在本次重查；后续用其确认的 BIT 启动新版软件，再接通以太网更新和 JTAG 重载流程。

## 2026-10-04 Claude 模型已配置，密钥等待本地隐藏输入

用户进一步要求配置密钥并选择最适合的模型。Codex查阅Anthropic当前官方模型、科学/复杂推理选型、价格和effort文档，选择 `claude-opus-5-5` 为此用途的起始模型：自适应思考、high，日常生成上限16,384 token，HTTP等待600秒；标准输入/输出每百万token $4/$20。并非已证明最适合本论文，后续仍看实际议题效果。官方来源及选型记录见 [使用说明](tools/claude-advisor/README.md) 与 preparation/model-selection.json。本机非敏感 config.json 已写入，仍受 gitignore 忽略；当前未检测到配置路径下的密钥文件，不读取密钥内容。

DeepSeek起草 `configure_key.py`、effort支持和离线测试，Codex直接核查实际源码、应用提案并执行。初测87项有一处StringIO测试错误；定向修订98项通过后，Codex补查发现超时设置函数遗漏表前配置，实际probe确认prefix/deepseek丢失，但未运行真实全局写入。再次限定修正后最终目录113项exit0，含表前/表后/中文/嵌套表保留、CRLF、拒绝多行伪表头、密钥权限、隐藏输入、短请求与失败/部分完成状态。完整模型返回、补丁、诊断及 [最终验收](tools/claude-advisor/preparation/key-setup-acceptance.json) 保存在preparation目录；不把临时假响应当Claude实测。

最后一步必须用户本人在Dell真实终端运行 `/usr/bin/python3 /home/xvhaocheng/work/tools/claude-advisor/configure_key.py`，在隐藏输入提示粘贴key，不发聊天。程序仅发一次无研究材料的512-token上限短检查，成功后写工作区外 `/home/xvhaocheng/.config/claude-advisor/anthropic.key`（0600/目录0700），更新配置并注册无密钥MCP入口，持久tool等待660/startup20并经CLI复核；不确定的同名入口保留不覆盖。检查失败不覆盖旧key；保存后配置/注册失败如实报告key_saved/keyconfigured/registered分开状态。含三连引号的全局TOML自动超时编辑保守拒绝，待后续核查，不盲改。工具可见性需重新加载/新聊天，不能承诺当前列表已刷新。

本聊天尚未输入真实密钥、读取真实凭证、请求Claude或修改全局Codex配置；实际后续连接以 preparation/connection-status.json 和真实程序输出为准。仅用户明确点名的当次议题允许研究调用，现有Codex/DeepSeek分工保持。无RTL/Linux/构建/板卡改动。

## 2026-10-04 SD常驻更新：软件阶段交付完成（无RTL/真实写卡）

用户从可行性讨论转为明确实施，要求先改软件，上一版 RTL 继续由原任务编译。本轮只修改 Linux/OpenSBI、板端工具、Mac 外部重启工具和软件构建/打包，不干预 FPGA 工程或现有构建。

软件实际完成：CONFIG_SPI_XILINX/MMC/MMC_BLOCK/MMC_SPI=y；新增匹配 framing_top 寄存器的 GPL lowRISC Ethernet 驱动，64位包缓冲访问、NAPI、MDIO/phylib 实链路、无 TX 完成中断时的轮询/定时器、1G全双工限定。Codex直接审查实际 RTL 后修正 RX保留FCS的处理（软件CRC检查并移除）、PHY广告速度、MDIO释放与网口关闭的IRQ/定时器竞争。SPI/ETH DTS interrupts改为单单元2/3。SD更新器确认实际/dev与sysfs设备号、父卡、512B逻辑扇区、32MiB/LBA2048/GPT首项type与unique GUID/CRC/边界和重叠，拒绝挂载或锁冲突；镜像先完整读入内存，写分区及零填充尾部，fsync/关闭成功才PASS，无自动重启。OpenSBI只调用标准fdt_reset_init，当前DT无复位设备，不虚构硬件寄存器。

最终内核 `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty`，Linux/OpenSBI构建exit0，payload 29363656字节，SHA256 `c457dceb483aeb0f33f59a32c8b01236932d437565c7d983d5134840e99fbc53`。SD工具23/23、rootfs13/13、网络/下载11/11、Mac重启15/15、实际axis_gmii_rx/rgmii_lfsr仿真3/3，共65项通过。检查当前gzip initramfs确实在Image、当前Image确实在fw_payload，14个rootfs工具与当前源产物一致，最终配置全部为y；驱动checkpatch 0错误/0警告。完整worker原回复、修订补丁与真实日志保存在out/software-update-20261004，未以worker建议当实际测试。

[交付包](iommu-platform/out/software-update-20261004/software-update-20261004.tar.gz) 26553224字节，SHA256 `9bc33872f7e3395d0801da308dd6ca6b6b2182784e775c74a004256cac4b47a4`，13个包成员全部逐字节核验。[操作说明](iommu-platform/docs/software-update-20261004.md)；[软件状态](iommu-platform/out/software-update-20261004/software-update-state.json)；[嵌入/测试验收](iommu-platform/out/software-update-20261004/embedded-artifact-acceptance.json)；[压缩包核验](iommu-platform/out/software-update-20261004/package-acceptance.json)。旧cpn1标准产物已在previous/保存，原composition-v1-delivery未覆盖。

当前普通reboot仍依赖未加入的CPU可控硬件复位路径；当前Mac helper通过UART sync确认、记录旧boot_id、JTAG重载同一显式BIT，再确认目标uname和不同boot_id。默认dry-run，真实执行需指定BIT/串口/目标内核/新输出目录；正确处理无换行提示、旧提示残留和从旧内核升级，失败不重试。首次仍由用户人工写新payload；随后需要Mac真实SD枚举、PHY/ping/传输SHA、写同一已知可启动镜像与重启闭环。静态IP不跨启动持久化（可由AI经UART重配），纯网口控制认证未适配，单分区更新无A/B回退。未连接板卡、未写SD、未JTAG加载或reboot；板端结果均待验，不宣称已经实现完全无人值守。DeepSeek起草工具、测试、文档及定向修订；Codex负责关键驱动/边界修正、直接源码审查、应用执行和最终验收。

## 2026-10-04 Claude 官方 API 顾问入口准备（仅用户点名时调用）

用户希望用 Anthropic 官方 API，在自己主动声明时，参与计划、创造力、论文主线、研究方向及与 Codex/DeepSeek 的交叉讨论、审计。不是默认规划/审核者，也不替换主对话模型；授权范围限当次议题。该约定已写入根 AGENTS.md。

已准备 [tools/claude-advisor/README.md](tools/claude-advisor/README.md)、Python 标准库 CLI/stdio MCP 桥、无密钥配置模板、默认禁用的 MCP 注册模板与离线测试。调用须传入 user_request，缺失时在配置/密钥读取及网络请求前拒绝；该字段只作追溯，不能证明授权。显式文件限定项目范围，拒绝越界/隐藏/凭证类路径；密钥来自环境变量或工作区外私有文件，不写入项目。保留模型、用量、停止原因、回复与触发请求；无自动重试，无自主执行或浏览。

DeepSeek Flash 思考模式起草及一次定向修订，完整返回保存在 preparation/deepseek-response.json 与 deepseek-revision.json，均 finish_reason=stop、truncated=false（total_tokens 50869 / 78046）；Codex 直接读源码、应用并执行。最终目录 52 项离线测试 exit0，CLI --status exit0 且 configured=false/call_ready=false；stdio 子进程完成初始化/列工具/本地状态/无授权引用拒绝/后续 ping，exit0。原始日志及摘要见 [准备验收](tools/claude-advisor/preparation/acceptance.json)。测试为本地假响应，不是 Claude 连通验收。

尚未选择 Claude 模型、输出/思考预算，未配置密钥、未注册或启用 MCP、未改全局设置、Claude 真实请求为 0。下一步用户选定模型并在本地配置密钥后，再在明确授权范围内做一次小规模连通检查及后续工具注册。非流式长回答限制尚未验证。未改 RTL/Linux、未构建/上板，不改变现有研究结论或其他聊天的执行状态。

## 2026-10-04 SD常驻、板端更新工作流可行性核查（仅讨论）

用户考虑保留SD启动，由Mac把fw_payload.bin传入运行中的FPGA Linux，写GPT第一项的32MiB原始启动分区、同步后再重启，询问是否即可避免日常插拔并交AI运作。本轮不是实施指令，未改驱动/构建/写卡/重启。Digilent官方Genesys2手册24–25页确认配置后FPGA获SD总线，支持SPI/native与块读写；源码KINTEX7已包含SPI和网口，SPI IRQ接irq_sources[1]、DTS有spi@20000000/mmc-spi-slot。实际out/linux/.config有MMC/MMC_BLOCK/MMC_SPI=y，但CONFIG_SPI_XILINX未开启；先补驱动及板上枚举/读写验证，不能把硬件可读写当现有Linux已可更新。

Codex直接核对bootrom/src/main.c与gpt.c：读GPT第一项first_lba到last_lba整段，保持现有分区布局和镜像格式可沿用读取逻辑；不是BootROM固定32MiB常数。DTS网口compatible为lowrisc-eth，本轮在drivers/net/ethernet中未找到匹配串，不能简单视为开启ETHOC即可。OpenSBI ariane platform_ops未配置system_reset，普通Linux reboot未证实；可优先沿用Mac/JTAG加载后的启动路径，另验证复位。总体可行目标为正常更新不拔卡：先传输（串口可先行）+SD读写+远程重启三关，再做网口优化。覆盖单一启动镜像不具备已验证的坏镜像自动回退，不能承诺任何故障都无需人工。DeepSeek整理源码证据，Codex直接复核关键链路；完整返回/tmp/iommu-sd-workflow-deepseek-20261004.json，核查见docs/validation.md。

## 2026-10-04 22:38–22:43 JST 当前状态只读核查：BIT布线中断

用户询问当前步骤、状态、问题与下一步。DeepSeek整理状态证据，Codex直接核对主机进程、实际日志、产物及两端聊天快照：Linux/OpenSBI software-build-state.json为complete/exit0；补充composition-tl-test-fixed.log为PASS，旧“测试激励修订中”已过期。FPGA日志最后更新20:43:36，停在Phase4.3 Global Iteration2，末尾overlaps41；impl_1有placed/physopt检查点及route begin，无route end或BIT，交付目录也无BIT。主机ps未见Vivado/make，启动标记PID2993231/2993275均不存在；仅存Python状态监视器不能证明构建仍在运行，fpga-build-state.json的running已过期。中断原因与退出码未知，不能把中间WNS−1.377ns或Route35-447拥塞警告当最终失败原因。

Mac“上板”聊天的当前快照也确认Vivado已不在、无新BIT，正在诊断且暂不加载FPGA；Dell“benchmark实现”聊天已idle。本轮仅核查与同步文档，未启动重建、写SD或加载板卡，也不改另一任务管理的build-state。下一步由现有执行聊天查明中断、恢复必要构建并验收最终时序与BIT/MCS；随后完成交付、一次SD更新/JTAG加载，同boot执行16短验收+12 S/S+144探索并回传离线核查。来源与原始路径见docs/validation.md本日状态核查条目。

## 2026-10-04 普通非阻塞原型的关键审查

DeepSeek定向整理真实请求生命周期。Codex直接核对trans_ctl、tl_wrap、top类型及driver preflight，否决same-DID复用A上下文的草案：当前A/V为DID11/10；原AR FIFO只露队头，保留A又把年轻V结果按tag送回“正确队头”不能闭合。单lookup端口不必然禁止等待内存期间时分复用；只读probe不能冒充实际服务或替换更新；旧feature mask并不自动拒绝额外位。已形成[修正研究约束](iommu-platform/docs/research-n-prototype-spec-20261004.md)，尚缺可实施的完整header/slot提取、own-context、结果顺序/失效契约；无新RTL/driver/构建/板测，不为了这版无效草案要求用户写SD。下一批先闭合这些接口，小基线原型与完整性能工程分阶段。普通hit-under-miss不是基本创新；P仅指同每类总容量静态预留，不能偷加端口/服务机会。


## 2026-10-04 method88完成、FPGA存活确认与文献审查

- method88 已正式完成：88/88 host_exit0，原始已回传。正式报告 `iommu-platform/out/method88-execution-20261004/RESULTS.md`；文献矩阵 `iommu-platform/docs/research-literature-review-20261004.md`。
- 两个启动分别报告：boot 1 `93406a68-fc8a-4040-9dc6-ae4e4a670d36`（复用既有启动，有旧 462 历史）；boot 2 `236b6316-cca3-49be-b52b-48f3963fa515`（新启动）。kernel `6.5.0-rc2-00093-gc0f5e13ea81f`。4 short + 84 long。
- 结论：p5 对最强匹配简单对照四格全负（-5.68952% / -8.23689% / -5.15142% / -4.43680%），跨方向 False。固定 A8 主创新路线结束，p5 降为已测试候选/基线；不继续为 p5 挑条件找有利条件。
- E2 已完成 64，不是尚无条目替换直接证据：Linux `00093`/role1 新版 64 = 短 4、边界 30、前缀 30，64 clean/exit0；128 target 中 62 个替换后 firstmiss、66 保持 hit，无未决。W7 正反各 5 次保留两 target，W8 正反各 5 次均替换；128 target 非 128 独立试验。证据 `iommu-platform/out/e2-results-review-20261003/RESULTS.md`；只限有限布局/实现范围，不能推持续吞吐归因。新 88 不是重做 E2/E3。
- Bermuda 是 SMMUv2 ZCU104 实验；SMMUv3 cache 论述属于 discussion，不能写成已实测验证 SMMUv3 显著吸收干扰。缓解措施只提议，未完整评估。
- 新 primary 是 Memory Latency Distribution-Driven Regulation，ECRTS2023，不是 RTSS2022；latency CDF 反馈限流已存在，仅把平均吞吐反馈改成 CDF 不自动新颖；IOMMU vs DRAM 差别须具体规则证据。
- 不要写成：窄幅反转、actual cost 独有收益、每 copy miss 固定、0xE2E 作真实服务分母、p5 漏计 DDT/PDT/PTW、E2 尚无条目替换直接证据、SMMUv3 已实测显著吸收干扰。
- 路径：Dell `iommu-platform/out/mac-method88-20261004-ssh-01/20261004-method88-ssh-01/`；分析 `iommu-platform/out/method88-analysis-20261004-01/`；辅助 `iommu-platform/out/method88-aux-20261004-01/`；Mac 归档 `results/experiments/20261004-method88-ssh-01/`。
- UART 已 live 确认 uname `00093` + bootid second，无需重启；串口已关闭。本轮无 Linux/RTL 新增改动、无新构建、无 SD 更新。
- 下一研究问题与终止判据见文献文档第 4 节；当前没有 N/P 实现和足够 novelty 差异，不能称已选好新方法或创新成立。先做标准 N 接口/资源/失效/顺序规格与关键 RTL 可行性对照；若仅常规优化无增量则不编大工程。读现有源码不是多余 hash 回读。用户授权继续实现不授权无科学价值工程。

## 2026-10-04 02:42 JST SSH Mac 自主执行 method88 已启动

用户授权自主实验、分析、计划和实现，直到需要人工写入SD，且要求取消额外回读/验收。已通过SSH传送现有交付包，使用Mac `tools/method88-venv`，外层 `scripts/mac_method88_attach.py` 接入既有role2 boot `93406a68-fc8a-4040-9dc6-ae4e4a670d36`，首批44无需BIT重载；第二批44由原runner同BIT新启动，冻结88参数和计划字节保持不变。部署修订在首样本前保存，报告区分第一批既有启动/第二批新启动，既往462活动是限制。Mac结果 `results/experiments/20261004-method88-ssh-01`，主机日志同名前缀 `.host.log`，启动进程43849；结果未完成，无科学结论。没有Linux/BIT构建或SD操作。新版离线分析器仅增加部署修订记录及启动文字，原匹配/两不同boot/错误门槛不改。DeepSeek起草接入脚本和报告修订，Codex审阅后执行。

## 2026-10-04 method88 主机侧交接：脚本/交付包与 Mac 执行

- 目标：在固定 A8、target=170 下，公平比较 p5（实占计费）相对简单对照 p1 字节、p2 请求、p3 miss、p4 事件加权是否还有额外收益；同时复核 p5 vs p6 的方向/阶段混杂与反转复现。不是全目标扫参，不以双模型一致替代证据。
- 冻结计划：88 项=4 短+84 长，每启动 44 项，2 次 fresh boot（同一 role2 BIT），seed 1380792376，canonical plan SHA256 `6ea6c4158a19c324359ece6e1458adfa450472cfb3b09168d13550c383f783dc`；源 selection raw SHA256 `d046a33572e7ff49d314e89419ce9574fbf0770d11675242243c2c2a5b12baa2`，参数不改。
- 主机文件：`scripts/mac_method88.py`、`scripts/research_method88.py`、`scripts/package_method88.py`、`tests/test_package_method88.py` 等；交付路径为 `iommu-platform/out/method88-20261004/method88-20261004.tar.gz`，打包验收 `iommu-platform/out/method88-20261004/acceptance.json`，主机验收 `iommu-platform/out/method88-preparation-20261004/host-acceptance.json`。交付包已生成，5,404,254 字节，SHA256 `d3dc7940993b37e8b05f6984f3c7275e41976f70d13197a209b5b47980c66d66`；板上仍为 0/88。
- Mac 下一步：解包 `method88-20261004.tar.gz`，`cd method88-20261004/delivery`，先预检，再正式运行到必须尚不存在的 `../../results/method88-20261004-01`，完整保留并回传输出目录。离线分析可选，可在回传后由 Dell 执行到新的 `../../results/method88-20261004-01-analysis`，不重新 freeze，也不构成 Mac 采集前置。
- 判据引用 `iommu-platform/docs/research-method-next-20261004.md`：公平匹配规则、强基线只在 p1..p4 中按保护匹配且 A 均值最高挑选、预定对照全部公开；同方向两次 fresh boot 均匹配且 p5 相对强简单基线 A 增益 >=5% 才作为窄条件信号；跨方向结论必须两个方向皆成立；无匹配可行对照为证据不足；p5 vs p6 反转单列；科学阴性按预定清单继续，技术错误首错停、不自动重跑/覆盖。
- 本轮实际主机证据：新计划/分析器 38 passed；新 Mac 26 passed（mock IPC 88 且仅运行一次，2 次同 role 2 fresh boot；原始 record 严格校验由 38 项覆盖）；复用旧核心 97 passed；打包 5 passed（package+plan 合并 43 passed）。总计 166 个不同主机测试。10041 份原始文件哈希未变，记录在 `iommu-platform/out/method88-preparation-20261004/raw-input-integrity.json`。无实际 FPGA、无 Mac 预检、无板端结果。
- 边界：不改 Linux/驱动/RTL/BIT，不重写 SD，不编译；未向 Mac 传输交付包、未运行板端 CLI/板端实验。沿用现有 00093 与 role2 BIT。DeepSeek 起草脚本/测试/文档；Codex 关键来源复核、实际执行与主机验收。
- 用户最新要求：后续省去额外解包与独立运行验收，只做必要脚本检查和出现错误时的排查；不再追加本轮检查。


## 2026-10-04 JST 两BIT上板采集完成：E2 64项＋method 462项

Linux00093，两段共526项原始记录已保存，主机续采集进程退出0，COMPLETE.json及两个stage-done.json完整保留。完整目录iommu-platform/out/mac-research-two-bit-20261003/，含e2/、method/、完整UART、计划、版本、退出状态、原runner文件及主机适配器；首次tar-z采集停止现场initial-transfer-failure/独立保留，第1项只恢复采集未重跑。Linux仍运行，UART已释放。用户最新要求仅保留并回传原始数据，不再做结果分析；不新增统计/机制结论、离线分析或独立验收。既有自动产物原样保留，正常完成不代表科学结论已验证。回传内容核对见DELL_RECEIPT.md；下方方法未完成是历史状态。

## 2026-10-03 两BIT上板进度：E2 64/64已回传，方法阶段运行中

Mac使用共用Linux00093与role1 BIT，64项E2均执行保存、逐项交付validate_record及整段保存哈希核对通过，stage-done cases64。完整原始JSON/归档/metadata/退出码/dmesg/taint/argv/计划/boot/flash位于iommu-platform/out/mac-research-two-bit-20261003/e2/，回传内容核对一致；原始UART前缀及DELIVERY来源一起保留。第一项执行exit0后因BusyBox tar不支持-z停止采集，原现场保留；只调整tar -cf后gzip -c，取回原第1项未重跑，再执行其余63项。方法role2 BIT已切换、462项正在运行，尚未完成；不宣称全526项完成。记录只是板上执行/逐项校验，机制及性能结论待离线分析。

## 2026-10-03 fullstats v1首次上板：21/21在线完成

Mac使用新fullstats配套BIT＋Linux6.5.0-rc2-00092-g5853e425caf8，boot_id=9a14f4f9-514e-4d18-93f2-b987b2e3bce6，执行一次platform-research cost-fullstats /tmp/research-fullstats-01。3短＋18主共21/21原runner在线通过，CLI实参含--require-fullstats；final_exit0、FULLSTATS_EXIT0、collection_failed0。完整JSON/stderr/退出码/dmesg/taint/argv/计划/UART/启动与版本来源回传out/mac-research-fullstats-20261003。未新增独立离线语义验收或统计分析，待Dell后续分析；不宣称方法M有效。旧E2未重跑，不跨版本拼接；没有重跑/新增试次/代码修改/编译。Linux保持运行，UART已释放。本轮Codex执行既有交付工具，未调用DeepSeek。旧0/21未上板条目保留为历史状态。

## 2026-10-02 Mac Solo修复版00091：11/11在线完成

Mac已加载现有研究BIT并从新SD启动Linux6.5.0-rc2-00091-g9ad05d00cca7，boot_id=db14364a-c046-4e6a-8ae7-f4dea007724a。运行一次platform-research cost-followup /tmp/research-followup-01：短Solo/A8gap0各1＋主Solo/A8gap0/A8gap2000各3，共11/11原runner通过，final_exit0、host0、collection_failed0。Solo未再被EINVAL拒绝。完整原始数据与UART/版本来源回传out/mac-research-followup-20261002；没有新增独立验收或性能分析，仍需后续离线核查。独立新软件同启动对照，旧84项80成功2失败2未运行记录不改，不跨版本拼接；E2未重跑。初次SPI停顿后1MHz重载同一BIT恢复，未编译/改驱动RTL/扩矩阵。Linux保持运行，UART已释放。本轮Codex执行既有交付工具，未调用DeepSeek。

## 2026-10-02 Research v1继续：可运行项已跑完，累计80通过/2失败/2未运行

用户授权后80项能跑的先跑。原runner临时副本仅改选取范围，复用当前Linux00090/BIT，不重跑前4项、不改驱动/RTL/参数。ordinal5..66共62项通过；67主Solo再次Invalid argument/exit1（JSON errno22/stage0/raw_captured0/phase提交完成0），停止保留。另两次同参数Solo ordinal78/83未运行；其余15项双路主试次随后全部通过exit0。新增77通过＋1失败＋2未运行，合并首次记录为80通过＋2失败＋2未运行；E2短3/主60和cost非Solo短2/主15均完成。仅CLI及原runner检查，不冒称独立语义验收或84/84。完整两段原始数据、UART、临时runner回传out/mac-research-cont-20261002，首次证据不覆盖。Linux保持运行，UART已释放；未根因诊断/修复/性能分析，原始hrtimer警告保留。本轮Codex执行机械选取及已有工具，未调用DeepSeek。

## 2026-10-02 Research v1 上板：第4项失败，停止未重跑

配套新BIT/SD启动Linux00090后，仅执行一次platform-research all /tmp/research-v1-01。short-e2-w0/w1/w16共3项CLI exit0；第4项short-cost-solo返回idma_research: run: Invalid argument（CLI exit1）。runner final_exit1、cases_done3/84、collection_failed0；4项尝试、80项未运行。未独立验收JSON语义，根因未诊断，未修改/重建代码或重跑。Linux保持运行，UART已释放。完整原始目录、UART、版本来源已回传iommu-platform/out/mac-research-v1-20261002/，传输内容已核对；README记录边界。按用户要求直接使用已有工具，未调用DeepSeek。旧未上板0/84状态由此更新，不改变其他研究计划。

## 2026-10-02 正式五轮研究设计审查：唯一M仍是有条件候选

用户指定五轮结构已实际执行：R1双方独立提案（隔离本轮答案、哈希冻结，但已有历史知识仍影响）；R2交换攻击；R3砍到1案；R4双方各模拟体系结构/OS/安全审稿；R5只解决R4问题。旧路线两轮预审在`out/route-cross-review-20261002/`另计，正式五轮在`iommu-platform/out/research-five-rounds-20261002/`。五次DeepSeek均deepseek-flash、思考开启/高、默认最大预算、finish_reason=stop、truncated=false；全回答、本轮提示、Codex独立提案/质疑/裁决与文献摘记留存。不是六位真实独立审稿人；不得把未采纳原文当已批准规格。

- 最终执行入口：[paper-logic.md 五轮最终裁决](iommu-platform/docs/paper-logic.md#2026-10-02-五轮交叉审查最终裁决)，具体逐项关闭表在DeepSeek R5，最终覆核差异在`round5-codex-final.txt`。Codex核查关键frontend/transaction controller及backend完成脉冲边界，未全面扫描仓库。
- R1有Codex C1实占反馈/C2合法描述符重排/C3服务时间通道；DeepSeek D1分阶段归因/D2翻译合并/D3通道加相位防护/D4反馈信用。C1+D4合M，D1留诊断；C2缺真实语义trace、D2前提与接口成本不成立、C3/D3先行技术/真实可见性/防护不足，均不做主线。普通N/P及旧R不再自动实施；保留历史，不称被M击败。
- 先行技术新增直接IOMMU按翻译请求token的US20250181518A1、AXI-REALM按周期补给/按beat计费、MISE服务率反馈；最终Codex补核SafeSU §III参考成本/CCS归属/时间quota，因此计时间本身也不新。L11补核发生在DS R5生成期间，不假称DS已回应。M只剩真实控制占用相对强计数/固定类型权重/简单反馈的额外价值假设，尚无实验。文献检索非穷尽，不保证可发表，也不提前判只能表征论文。
- 修正M草案：A owner且REQ_TRANS/AWAIT_RESP每拍单计；COMPLETE出口背压等只诊断；有符号余额，末拍先扣再旧b补给再更新下窗b，欠账保留；首copy同准入门。V可信batch生命周期，零完成不作缺席；旁侧硬件块接真实DMA完成脉冲，不用翻译完成替代有效工作。删除强制放行、免债饱和、silent fallback；故障停新任务排空留档、既有VALID不撤；软件清状态只在排空后。源归属/成功完成/CDC/完整排空/定点位宽与地址映射仍未验证。
- 两条并行：E2 V预热→A有限访问→排空→V首次探测及条目覆盖链；主W0/1/4/7/8/16各10工程候选、A112次64B、前缀30事前选配。M自然4KiB成本诊断检验字节/请求/miss/固定类型权重之外的实占差异，含同版Solo、固定模式参考与负对照。E2不能量化普通数据竞争，阳性不是M前置；共享被动观测包、单板重建初态分开执行。
- 先规格/小RTL与主机验证，后构建及时序资源审核，再板短验收与诊断，之后才决定完整M/基线与独立验证。尽量E2与诊断一次部署，但不为省烧录提前实现所有方法、不承诺一次部署覆盖全部研究。当前无可直接启动的新实验命令。
- 评测：同BIT同仪器同作动点、各计费单位训练归一化、等调参预算；增加固定类型权重请求基线。匹配V固定任务时间及短时未达分布，主比A同V硬件窗完成bytes/time，带入/带出和A尾部单列。run/paired-block独立、boot分层，不把窗当重复。校准后再冻结E/目标/容差/MDE/n/调参次数；未预注册不跑确认性测试。无差异显著证据不等于等价；确实无实用增量则撤回M方法主张，证据不足保留未决，不自动切换论文目标。
- 本轮计划/文档而已：没改RTL/驱动/runner，没构建/仿真/板测；没有重跑P4M分析或修改原始数据。原60完成与E3机制结论仍有效。DeepSeek作者提案/批评/重构/报告，Codex作者独立提案/关键质疑/科学裁决、实际工具核查与文件应用。

## 2026-10-02 Codex／DeepSeek两轮交叉审查与最终裁决

用户要求相互提问质疑整体路线及创新，未要求本轮实施或编译。DeepSeek两轮均deepseek-flash、thinking enabled/high、393216预算、finish_reason=stop、truncated=false；usage总数分别123934/64175，完整响应、Codex第二轮问题及核查补充在`iommu-platform/out/route-cross-review-20261002/`。没有伪造讨论或用双方同意代替验证。DeepSeek提质疑/修订草案，Codex定向读关键RTL、统计口径及原始文献后裁决。

- 保留P4M60已完成、E3有限服务阻塞证据、E2待证和方法论文目标。固定gap三点不能证明翻译解释所有损失或所有节流均差；PTW忙占比/重叠只能定位，不能直接当可回收耗时。
- 文献新增直接IOMMU技术先行披露WO2010117360A1（按设备pending队列避免其他设备阻塞），连同HPCA2017 hit-under-miss、Valsan非阻塞资源干扰、MASK fill控制、DWS walker分区、RTCSA2025表征，说明普通N/P不能当新意；不保证检索穷尽或论文可发表。R仅剩离线联合资源依赖与合法顺序、选择保护位置/容量的研究假设，规则尚未形成。
- 采纳DeepSeek关于避免循环门槛的反问：允许小型实际RTL功能原型验证可行性；完整FPGA性能工程须再过方法门槛。否决其仍残留的“10次阴性否容量”“并发1就一格足够”“非V路径保护必无效”等推断；N一场成功只否定该场景需要P，N失败不能自动归于出口。A-only在P下减速是需报告的合法预留成本，不要求N/P全轨迹一致。
- E2主矩阵W0/1/4/7/8/16各10次保留工程候选，A112次64B；前缀1/8/16共30次降事前选配，不边看阳性边加。阴性继续记录不自动停。真旁路快照与有归属的覆盖→驻留改变→首访结果链；统一T、完全排空、PLRU与真实上下文、源目实际顺序，记录同拍语义。拟观测包附Solo/A1g0/A8g0/1000/2000各3次4KiB诊断候选，与原正式60分开；执行前冻结参数，不是新方法正式评价。
- 当前板源码绑定`ariane_peripherals.sv:768`→`ariane_soc_pkg.sv:115`=16项，已核；observer队列以tr_fire握手记时，非首次valid观测，且快照不是完整IOTLB状态。N需跨控制器/TL/完成路径的逐请求身份与原始头配对，FIFO现有头不等于可直接乱序读。补核Arm AXI握手稳定要求：已提出未受理的A地址不能擅自替换为V，不承诺预留消除共同出口阻塞。
- 开发并行：S上被动观测包（E2＋4KiB诊断）与E3生命周期规格/小原型。共用小记录头、各模块版本扩展；E2不等N；单板顺序各包交付，不为省一次上板强行合并未知架构。之后同版本自身S/gap/N/必要P-R、匹配V收益的节流对照及独立验证，旧60不被重新当新方法独立证据。
- 本轮只改计划/交接，未改实验代码、未构建/仿真/板测、未重跑统计或改原始数据。执行以`docs/paper-logic.md`最新“交叉审查裁决”和`docs/experiment-plan.md`入口为准；完整讨论原文含未采纳提议。没有自动转为表征论文，也没有要求用户提前放弃方法目标。

## 2026-10-02 完整60份分析与两条并行实验计划

- 用户要求“35轮完成，分析后再完善并行的两个实验”。依据已回传证据，实际为原25＋保留6＋新boot29。60份每gap20、身份53在线＋7离线追认；沿用原审计，未将缺失CLI退出码补写为0，未重跑任何试次。保留三个采集段及pre31/post29，后者各gap为10/10/11及10/10/9。
- DeepSeek起草只读分析程序并两轮定向修订；Codex审阅程序、计时／流量契约和关键原始记录，实际执行。`iommu-platform/out/p4m-complete-review-20261002/analyze.py` 及 `--self-test` 均退出0，60完整性、参数、计时对应、流量守恒等检查必需失败0／警告0／缺项0；191输入SHA未变。自检覆盖重复／缺例、错误周期或字节、文件清单额外／放错位置、伪造conservation=true等。完整JSON、CSV、报告、模型输出和修订过程均在同目录，原数据不改。
- 全60 V中位MiB/s为108.8134916／150.6025798／171.3890800，A同窗口为108.8134916／75.3012899／57.0878518。V范围分别108.720942–110.156634、150.366839–150.731973、171.208434–171.508421。整轮A miss/S1中位5140/5140、3086/3086、2401/2401，不能当作硬件窗口计数或直接解释为每请求miss率降低。
- 主要同采集段配对：gap1000 n20，V吞吐+38.4233%、耗时−27.7578%、A速率−30.7884%；gap2000 n18，分别+57.4856%、−36.5021%、−47.5539%。同pre/post段的gap2000敏感性n19（含b09）接近；b11 g2000/g0跨boot单列。描述统计保留全部60，不因配对条件而删除原数据。重启前后V中位差约−0.041/−0.030/−0.060%，仅描述而非等价检验。
- **重要新样本：** `mac-p4m-remaining29-20261002/collection/unpacked/formal-b15-g0.json`，新boot seq13：V整轮miss/S1=2/2、其余59份0/0；A miss5141、V吞吐110.156634（gap0最快）、开窗A在途ID16（通常6）、a_to_v248040ns、irq_span548840ns、done_to_read794615cycle≈15.8923ms。Codex核对startup和batch_time与该份原始dmesg精确一致。现有计数不能定位miss在硬件窗内／外或相位，无条目轨迹；不推断替换、不推断miss加速或启动／迟读因果，不剔除该份。
- 决策：P4M-5这轮固定gap基线完成收尾，不再扩gap；总体方法论文路线保留。后续评价目标为相近V改善下保留更多A。旧60是参考，新硬件须自身同配置串行／gap对照，不能跨BIT直接归因。
- E2候选单包主矩阵W0/1/4/7/8/16，各有A组112次64B搬运、每条件10次工程候选；新增A8仅1/8/16次有限搬运前缀（各10），共90候选测量＋短验收。主／前缀统一等时窗口、真实准备／排空、probe前两目标项同时快照及条目跨槽生命周期和逐请求归属。原布局／顺序复核延后独立验证。不是对b15内部历史的精确重现，ID16不直接等于16次。
- E3改善：先合理普通非阻塞N，验证A长查表时合法V hit前进；必要才同容量静态预留P（非创新）。完整AXI头与译后地址配对、AW/W排空／排序／权限／fault／失效及结果寿命须先定规格，不能只改TL。Codex重新核对trans_ctl单状态及FIFO头字段来源、TL单请求ready。三筛选场景与后续4KiB4096/256整批轮询性能对照详见 `docs/paper-logic.md` 最新节。
- 两支线开发／主机验证并行，单板顺序，每支线一次准备齐；共用被动记录接口但不把未验证N/P塞进E2包。下一步共用记录格式与运行清单、E2观测／runner与E3生命周期规格并行；本轮未进行新增实验RTL/驱动实施、完整构建或上板。新计划在paper-logic.md，experiment-plan.md已同步入口，validation.md已记录实际验证。

## 2026-10-02 P4M补缺完成：29/29通过，60份计划结果已齐（跨启动）

证据已完整回传 `iommu-platform/out/mac-p4m-remaining29-20261002/` 并与Mac逐文件内容核对一致。实际只执行flat32..60共29次，无重跑/额外短验收，原runner检查全部通过：exit0、collection_failed0、taint0；29份JSON全部success=true、errno0、双方data_errors0，V完成4096、A完成数等于提交数。

之前未登记的6份flat26..31已离线复核通过（原始CLI退出码在输入中缺失，不改称在线通过）。原25＋6＋本次29＝60份，每gap20，合计53在线＋7离线追认。用户断电后原BIT/SD恢复Linux00089，新boot_id=e6d264eb-e2a2-43e8-8136-8a7e10363e56；这是跨启动补缺，不能当作同次启动60次在线通过，性能分析须保留分段。旧reported_running/35未跑仅为历史状态。

完整JSON/UART/板端归档/版本来源/审计/脚本均保留，初次串口打开失败未发送DMA。Linux保持运行，UART已释放；未修改或重建RTL/内核、未启动E2。详细边界见结果目录 `README.md` 和 `DELL_UPDATE.md`。DeepSeek起草脚本，Codex审核修正并实际执行和回传。

## 2026-09-20 P4M formal60 recovery：采集修复已应用，resume35 prepared_not_run

原始在线运行仍为 24 在线通过、25 attempted、35 unrun、runner exit5；第 25 次 formal-b09-g2000 因 `startup_line_count before=36 after=36 want=37` 停止，判定为 runner 记录计数假设失效，不是 DMA 失败或新硬件根因。第 25 次作为离线复核追认保留，原始 run.json 与 flags 不改写，不伪称 25 在线通过。

Codex 已应用 `iommu-platform/tests/board-p4m-formal60.sh` 新 resume35 模式，并复制交付为 `iommu-platform/out/p4m-formal60-recovery-20260920/delivery/platform-p4m-formal60`。resume35 只跑 flat26..60：先 b09g0、b09g1000，然后 b10..20 原顺序；分配 12/12/11，合并已有 8/8/9 可成 20/20/20。Preflight 要求当前 Linux exact 6.5.0-rc2-00089-gefd9461d6459、path_observe Y、taint0、现 dmesg maxseq37 完整唯一 startup+batchtime 且 a_begin_ns=3600085892000，原板端 /tmp/p4m-formal60-215406 存在，并保留第 25 次的 JSON、原始 dmesg 和 params.txt；若重启、额外试验、seq 改变、证据缺失等不通过则停止人工复查，不能用改门限/重新跑 formal60 绕开。板端继续运行 Linux，使用已有 00089/P4M v7，无需 SD 更新或重启或 BIT 载入，无内核/RTL 修改或构建。

Dell 无接板，本次 35 尚未运行，状态 prepared_not_run。Mac 操作员需按既有传输方式将新单文件放到板端 /tmp/platform-p4m-formal60，并在串口完整保存 stdout/stderr：`sh /tmp/platform-p4m-formal60 resume35 /tmp/p4m-formal60-resume-01`（新输出 dir，不能覆盖旧 dir，不用 tee 管道掩盖退出码，退出立即 `echo $?`）；完整保存 35 JSON/err/dmesg/argv/matrix/params/taint/resume35-preflight/串口和 exit，完成后只读归档回 Dell 再合并审计。脚本 35 完成不等于论文 60 正式已审结。既有 Mac 采集器的自动上传/参数适配未实现，不声称脚本已传到 Mac/FPGA。

主机与离线验证已由 Codex 执行/接受：tests/test_board_p4m_start.py 41 项、tests/test_p4m_formal60_recovery.py 16 项、tests/test_p4m_formal60_resume.py 15 项（含真实 seq37 归档 BusyBox 预检查）通过；shell syntax 0；加强版 offline_revalidate.py 25/25 all_passed=true，global_metadata 通过，PLAN25 前缀通过，taint0，CLI exit0，capture complete，JSON success/errno0，data_errors/quarantined0；证据 tree 和原输入 hash 未变，Codex 机械核对全部 14 份原始交付 SHA 一致，25 份 before-dmesg 错误文件皆空。

首 24 样本（8 完整 blocks）保持 balanced；V MiB/s 中位数 108.8275045/150.6503266/171.4545531，同硬件窗口 A MiB/s 中位数 108.8275045/75.3251633/57.0900898，A 整轮 miss/PTW 中位数 5140/3086/2401，V miss/PTW 0。相对 gap0 配对 V 硬件周期比中位数 0.72242199/0.63492757，反比推算吞吐提升 38.42%/57.50%；同窗口 A 速率配对比中位数 0.69211626/0.52448158，下降 30.79%/47.55%。这些仅描述节流取舍，不宣布最终效果/显著性/最优 gap 或 IOMMU 机制根因；整轮 HPM 不能当窗口计数；50 MHz 为既有假定时钟。详见 [恢复报告](iommu-platform/docs/p4m-formal60-recovery.md) 与 [续跑交付说明](iommu-platform/out/p4m-formal60-recovery-20260920/delivery/README.md)。

## 2026-09-20 P4M formal60：24 通过，第 25 次日志计数检查停止（未完成）

证据目录：`/home/xvhaocheng/work/iommu-platform/out/mac-p4m-formal60-20260920-215406-run-cicggil4`。14 份原始证据已回传并逐字节核对一致，含 25 个 JSON、完整 UART、板端目录压缩包（207 项）、冻结 PLAN、实际 runner、版本来源、RESULTS.md 与 FAILURE_HANDOFF.md。故障原因由 Codex 核定，DeepSeek 起草交接；没有另跑独立验收。

计划 gap0/1000/2000 各 20，共 60，4096 transfers / warmup256 / 4KiB / V0-A1，20 块交错。复用同次启动的 Linux00089 与 P4M v7 BIT，无重启/重载。实际 24 次通过（每 gap 8），第 25 次 formal-b09-g2000 CLI exit0、JSON success=true、双方 data_errors/quarantined0、taint0，但 runner exit5；35 次未运行，无自动重跑。

错误原文：`P4M: case=formal-b09-g2000 verify_failed=startup_line_count before=36 after=36 want=37`。归档前快照 seq1..36，后快照 seq2..37，各36条：旧seq1被 dmesg 环形缓冲覆盖，新seq37存在。继承的记录检查要求总数+1，因而停止；并非发现 DMA 内容校验错误。新 formal60 矩阵未改该检查函数。保留第25样本但不计通过；RESULTS.md gap2000 n9含此样本，不能当成9个合格结果，也不能宣称完成60次。

下一步候选仅修脚本按新 seq 定位、保留其余检查，再由用户决定剩余计划；本轮未修 runner、未补跑，不需要因这处脚本缺陷重建 BIT/Linux。FPGA Linux 保持运行，主机串口采集已关闭。旧 gap500 反例及所有历史证据保留。下面旧记录里的“尚未上板/正式参数未冻结”是历史状态，不覆盖本条。

## 2026-09-20 P4M start-control 00089：12/12 板上完成并回传

结果目录：`out/mac-p4m-start-20260920-211940-run-46nvmwgi`。Mac加载复用P4M v7 BIT，从更新SD启动6.5.0-rc2-00089-gefd9461d6459，执行一次 `platform-p4m-start all /tmp/p4m-start-01`。4短64/8＋8长4096/256、BPPBBPPB全部通过交付runner：exit0、cases_done12、taint0、collection_failed0，参与DMA data_errors/quarantined0。未重跑、未扩展正式矩阵。

8长B/P的V硬件MiB/s中位数108.81305/108.85767；硬件cycles中位7352059/7349046。4对P/B硬件cycles比值0.999413682/0.999699263/0.999995512/0.999888800。8长A→V为162960–165280ns、irq_span为315320–317040ns；P4M开窗A硬件ID均6。全部长样本V miss/S1PTW0/0、A整轮5140/5140，S2PTW0。n=4仅描述性接近，不作统计等价或IRQ根因结论；启动软件a_id与硬件开窗ID不同域，不相减。

短A1启动跨度较大（A→V778800ns、irq_span1387240ns），相关dmesg有hrtimer13155120ns警告，全部保留，不能据此确诊因果。候选下一步gap0/1000/2000，重复数/顺序尚未冻结，旧gap500反例保留；无自动追加试次。

12项交付文件已回传并逐字节读回一致，含原始JSON、完整UART、板端全目录归档（board-evidence.tar.gz.b64，105项含根目录）、交付runner、JTAG与SD版本来源、RESULTS.md。BusyBox tar不支持-z，先前打包失败1，后改只读普通tar成功；非实验失败，无试次重跑。串口已退出并关闭。run.json为上传时快照，后续完成状态见Mac回执/本交接。

按用户偏好未做下载哈希/SD回读或新增独立主机验收/HPM校准；保留交付runner内置检查。DeepSeek整理与修订报告；Codex实际执行、核对原始关键字段、复算并回传。

## 2026-09-20 P4M pilot-21 复核与启动协议控制计划（文档/分析）

- 状态：P4M-4 pilot-21 已完成，21/21 功能通过；P4M-2 legacy batch 与 P4M gap0 显式比较已做但结论未定。本轮只做分析、文档和计划，不改代码、不跑板、不构建，已有原始报告不改。
- 证据：全量 [RESULTS.md](iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/RESULTS.md)、[records.json](iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/records.json)、[observations-unique.log](iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/observations-unique.log)、[console.raw.log](iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/console.raw.log)、[run.json](iommu-platform/out/mac-p4m-pilot-20260920-202441-run-9_mfua8v/run.json)；复核稿 [p4m-pilot-results-review.md](iommu-platform/docs/p4m-pilot-results-review.md)。
- 关键判断：第三 P4M gap0 更快不能只用全局 ID 或 poll tail 解释；opening A inflight ID 为 4/4/1448，第三样本开窗前已完成更多 A copy，三个窗口 A done 均 4096；RTL/驱动支持启动/预窗口历史不同。残余算术仍归为硬件窗口与 poll tail 之外的其他开销；CPU 中断/抢占是怀疑方向，未证根因。
- 计划：先做 legacy batch 与 P4M 共享的最小 driver batch start 协议控制（A/V 先配置，只有两个 trigger 在短非睡眠临界区，立即恢复 IRQ/抢占），再按验证后 4 短 sanity + 8 次平衡 B/P/P/B×2 比较推进；新 8 次不替换旧 21，不冻结 n=3 的正式参数，保留 0/500/1000/2000 候选与 gap500 反例。
- 分工：DeepSeek 证据草案；Codex 关键源码/原始记录抽查与决策。

## 2026-09-20 P4M 21 次预试完成（Mac 回传）

目录：`out/mac-p4m-pilot-20260920-202441-run-9_mfua8v`。Linux00088 / P4M v7，同一次既有启动；4096 transfers、warmup256、4KiB、V0/A1，按既定3block交错顺序完成7条件各3次。21/21全部exit0、success/status0、V4096/4096、data_errors/quarantined/taint0、collection_failed0，无重试或额外试次，未启动正式采样。

完整原始JSON、UART、逐次观测、runner、版本与启动来源已回传，远端逐文件回读一致。RESULTS.md含7条件中位数/范围、21次明细、配对比值；EXECUTION.md说明采集口径。observer seq5..25为本轮，seq1..4为历史short。run.json是核心回传时快照，完成回传与交接以本条及Mac回执为准。

V硬件窗口推算MiB/s中位数：Solo283.508、A1gap0 149.346、batchA8 103.284、P4MA8gap0 103.243、gap500 98.075、gap1000 145.379、gap2000 171.394。A同窗口MiB/s中位数：P4MA8gap0 103.243、gap500 98.051、gap1000 72.689、gap2000 57.101。整轮A miss/S1PTW不是同窗口计数。

三次P4MA8gap0/batchA8硬件cycles配对比值1.00078/1.00054/0.93578；第三次gap0 Vmiss/S1PTW2/2、A整轮5141/5141、cycles7248503，不同于前两次；保留，不删不补跑，不确定根因或宣布等价。gap1000/2000可作吞吐取舍候选；gap500非单调点若研究整条曲线应保留。n=3仅描述性预试，正式参数与重复数仍待确认。

按用户偏好未做独立验收/主机回归或新增HPM校准；下载哈希和SD回读未核验。DeepSeek起草runner/报告，Codex审阅、实际执行、关键指标复算和回传。串口已关闭。

## 2026-09-20 P4M 板上短验收（功能通过，服务器复核通过）

- 执行：`platform-p4m short /tmp/p4m-v7-01`，Solo/A1gap0/A8gap0/A8gap1000，64 jobs、warmup 8、4 KiB；板卡 exit 0、final taint=0、collection_failed=0。
- 证据：[console.raw.log](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/console.raw.log)、[records.json](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/records.json)、[run.json](iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/run.json)。
- 复核：[p4m-short-results-review.md](iommu-platform/docs/p4m-short-results-review.md)、[review.json](iommu-platform/out/p4m-short-review-20260920/review.json)；修订后复核结果 正常复核 exit 0，自测 PASS，原始输入哈希未变。
- 结论边界：短验收只做功能验收，n=1；P4M-2 legacy `--batch` 与 `--p4m` gap0 显式对照未做；P4M-4 预试为下一步，计划 3×7=21 trials，正式 gap 点与重复数未冻结。原始 profile/manifest 中 board not_run 保持历史快照，不因陈旧 SD/hash 标记强制重烧。
- 分工：DeepSeek 起草复核/文档，Codex 审阅原始记录、UART、driver/observer 并做科学决策与实际执行。

## 2026-09-20 P4M 四组交付短实验已完成并回传

用户本次明确指定 platform-p4m short /tmp/p4m-v7-01，Mac 在配套 P4M BIT / Linux00088 上顺序运行 Solo、A1 gap0、A8 gap0、A8 gap1000；每组64 transfers、warmup8、4KiB。4组板端exit0、JSON success=true/status0、V完成64/64、活动DMA data_errors/quarantined0；最终taint0/collection_failed0/host exit0。无重跑/加测，未新增独立主机验收脚本或回归。已退出root关闭串口。

完整原始4份JSON、UART、版本、板端runner及结果说明已回传 /home/xvhaocheng/work/iommu-platform/out/mac-p4m-short-20260920-180713-run-l7cin11d/，7文件远端回读一致，本地有upload-receipt.json。实时printk破碎但完整dmesg回显和原始字节均保留。归档run为上传前快照，已回传以本节和回执为准。

V硬件窗口44279/83938/120679/85276周期；同窗口A完成0/64/64/32项，不得与整轮A完成4961/1496/61混用。V miss/S1 PTW均0，A整轮miss/S1 PTW为0/112/72；HPM与共同V硬件窗口口径不同。gap1000只是工程点，各组n=1，正式gap点/重复数仍未冻结，不声称正式方法收益，不自动扩矩阵。下一步按P4M实施卡确定预试范围。下载哈希/SD回读及时钟CDC边界仍保留。

DeepSeek整理指标及口径；Codex核对原始字段、执行上板和回传。补充指标表若生成将作为同目录独立文件追加，原始结果不改。

## 2026-09-20 P4M / Linux00088 已启动，尚未执行实验

Mac 已将 p4m-baseline-20260920 payload 写入登记 SD 第一分区（备份 update-v6_09g7q，未写第二分区），加载配套 migration-4dma-p4m BIT，并实际启动到 Buildroot 登录界面；串口显示 Linux 6.5.0-rc2-00088-g9f8da9e7a6a0。按用户快速流程未做额外验收、哈希或 SD 回读，未跑 platform-p4m short，实验次数为0，不声明 P4M 板卡实验通过。现有交付只提供4组short，正式参数未冻结；等用户明确实验范围，不自行扩矩阵。

本次启动完整 UART、delivery、SD/JTAG记录及run已回传 out/mac-p4m-boot-20260920-175705-run-rsn6mvks（相对iommu-platform），6文件远端回读一致，Mac实验目录有upload-receipt.json。串口已关闭，板卡停在登录界面。本轮操作复用脚本、由Codex执行，没有新增DeepSeek调用；用户重申DeepSeek无回应优先有目的重试，保持最大预算，避免为省DeepSeek token额外消耗Codex，仍不无限重试。

## 2026-09-20 P4M-1 主机验证与软件/FPGA 构建通过，待上板

P4M-1 主机实现与主机验证完成：14 组主机/RTL 检查通过（[validation.json](iommu-platform/out/p4m-baseline-20260920/validation.json) 的实际证据），包括实际 frontend 与 observer 联合仿真；完整软件构建 exit 0（[build-status.json](iommu-platform/out/p4m-baseline-20260920/build-status.json)）。FPGA 完整构建通过 exit 0；配套交付包已生成并校验（[delivery README](iommu-platform/out/p4m-baseline-20260920/delivery/README.md)）。

新构建：内核 `6.5.0-rc2-00088-g9f8da9e7a6a0`，CVA6 `e822bc9d8adf246a5d098585fb229744bafd9cc1`，Linux `9f8da9e7a6a08fb19134146241018eaff0eee124`，IOMMU 未改 `137f5e5fb1f2cc65a1346359226b16247c3f8445`；软件构建根 `1cdeb61`，FPGA 构建 `2fbf7ae`，两者差异仅为集成测试，不涉及综合源码。

最新实际板上版本仍为 E3 v6/Linux00087；P4M profile `migration-4dma-p4m`/observer v7 未上板，Dell 本机无板。P4M-1 实现与主机验证完成，P4M-2/P4M-3 板卡验收待做，正式 gap 点和重复数未冻结。

下一步先做板上短验收 `platform-p4m short /tmp/p4m-v7-01`：Solo/A1gap0/A8gap0/A8gap1000，64 jobs、warmup 8、4 KiB；gap1000 仅工程点，不是正式 gap 点。通过后预试同版本 gap0 并选定正式小 gap 集；暂不做全矩阵或复杂硬件方法。脚本向 UART 打印原始输出，由主机侧捕获保存。

## 2026-09-18 E3 v6 上板及服务器复核完成，下一步进入 P4M

E3 v6 / Linux00087 已完成 3 对短验收 + 8 对正式实验；服务器独立复核 exit 0、9 组检查通过。11 对 / 22 轮、418 条唯一 UART、114 条相同重复去重，11 份完整 CLI JSON 与 UART 对象逐字段一致，原始输入哈希未变；8 项程序检查（正常对照 1、故障注入 7）均符合预期。原始回传在 [mac-e3-controlled](iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/)，[复核证据](iommu-platform/out/e3-controlled-review-20260918/server-review.json)。run.json 的 pending_upload 是上传前快照；现已在 Dell，未宣称验证 upload receipt。

核心结果：4 对 cold 的 D128−D0，A 剩余服务和 V 剩余等待均增加 127/129/129/130 周期，V 自身翻译恒为 4 周期；14 个 cold 轮次 V 被选中均比 A 翻译响应晚 9 周期。身份、目标 A 的 PTE、前缀计数和完整插入身份配对一致，V 首查命中；D0/D0 的剩余等待差为 −2/0。结合串行控制器/AR FIFO 代码，支持本实现、此配置下 A miss 引起 V hit 等待的机制。样本仅 4 对，预触发耗时并非全部相同；不宣称完整 E3 矩阵完成、所有旧 A8 损失已解释或已有方法收益。[结果分析](iommu-platform/docs/e3-controlled-results-review.md)。

整体方向保留，当前优先推进 P4M：共同 V 硬件窗口内的 A 完成量与边界在途计数，以及 A 完整拷贝结束后的硬件固定间隔基线。先验证 gap0/短正确性，再预试、冻结少量参数并画 V slowdown—A 完成速率曲线；具体间隔和重复数待预试，不自动扩旧 E3 矩阵。E2 按证据触发，P5 在规则冻结后评价。[下一步实施卡](iommu-platform/docs/e3-next-method-plan.md)。本次只做分析程序和文档；DeepSeek 提供程序/文档草案，Codex 核对关键证据与 RTL、做科学决策并实际执行复核。

## 2026-09-18 E3 v6 / Linux00087 实板完成并回传

Mac 已按固定 3 短 + 8 正式对完成 22 轮，板端及离线执行/字段/守恒验收均通过；22 个 sequence、418 条完整观测，data_errors/quarantine/taint 均为 0。新 BIT migration-4dma-e3-controlled，实读 observer magic 0x49444d414f425306、launcher 0x49444d414c4e4301、STAT0x501079fc；Linux 6.5.0-rc2-00087-g2de370cd344e。无重试/补采/扩矩阵/重编，已退出 root 并关闭串口，printk 未改。

完整 16 文件已回传并逐文件远端回读比对：/home/xvhaocheng/work/iommu-platform/out/mac-e3-controlled-20260918-164635-run-rk48bhte/。含原始 short/main JSON、全部 UART、验收/摘要、版本 delivery/run、host-validation、E3_CONTROLLED_RESULTS.md 及验收脚本/测试。归档 run/summary 保留上传前状态，回传完成以本节和 Mac upload-receipt.json 为准。主机回归 111 项、25 子测试通过；实时 printk 有破碎字符，完整 dmesg 回显完好，按阶段 seq 取 114/304 条观测，原始字节与所有样本保留。

正式 cold 12/12 触发、hot 0/4，V miss/S1 PTW 全为 0；cold A 每轮 miss/S1 PTW 均为 2。4 对 cold 互补的 V 剩余等待 D128−D0 为 +127/+129/+129/+130 周期，自身翻译均为 4 周期；正式 2 对 D0/D0 的 V 剩余等待差为 −2/0。跨轮目标/PTE/前史计数/插入身份一致且无截断，但触发前等待/服务仍有波动，仅互补对6两项前置时间完全相同。有限证据支持延迟传播，不凭接近128即宣布纯D因果已证明；64B诊断不计算吞吐、不与旧A8混算。下一步服务器独立复核身份/前缀及原始证据，不自动扩实验或改RTL。

按用户要求跳过 Mac 下载哈希和 SD 回读；交付 SHA 是来源声明，非 Mac 实测。时序收敛/CDC/外部IO审查边界保留。DeepSeek 起草验收、收集脚本及测试修订；Codex 审查、应用、运行验证/上板/回传并完成原始证据解释。每轮完整结果默认回传 Dell 的持续规则继续有效。

## 2026-09-18 E3 v5 复核与 v6 受控实验（构建交付完成）

v5 服务器独立复核 PASS：9472 项检查、0 失败，26 子轮、494 条唯一观测，输入哈希未变。执行/覆盖达成，因果目标未达成：A8 目标 V 首次 miss 16/16，7/8 对目标身份不同；第 5 对身份/前缀相同、A 剩余 +128、V 剩余 +134，但预热不同且插入截断，仍不是合格因果配对。详见 [结果复核](iommu-platform/docs/e3-queue-results-review.md)。

下一步已实施独立 64 B 受控有限实验：每轮全局 S1 IOTLB 失效、hot/cold A、V prime/probe；先保留两路任务，硬件在 A 读被选中后释放 V，CPU 保留完成轮询。固定 3 短 + 8 正式对，不扩旧矩阵。方案见 [E3_CONTROLLED](iommu-platform/E3_CONTROLLED.md)。

组件已固定：CVA6 2b04936d、Linux 2de370cd344e、IOMMU 137f5e5；顶层硬件/内核锁定提交 57b3bcf。RTL 定向及旧模式回归、IOMMU 清空接口主机测试、相关内核对象交叉编译已通过；新驱动 19 场景故障模型、板端脚本、新旧 CLI 与镜像打包检查均通过。完整软件与 FPGA 构建均退出 0、manifest passed，内核 00087；BIT/MCS 非空，产物哈希、测试源码与硬件导出源码、实际 rootfs 内新程序均已核对。FPGA 于 UTC 06:24:37 完成，成功日志仅保存、未审查时序报告。[交付说明](iommu-platform/out/e3-controlled-20260918/delivery/README.md)，[构建与哈希状态](iommu-platform/out/e3-controlled-20260918/build-status.json)。新 v6 尚未上板；下一步配套更新 BIT 与 SD 软件，只跑 3 短 + 8 正式对，完整回传 JSON/UART 后再判断因果条件。

Codex 负责原始结果分析、架构/计划、DMA 关键修正、最终审查与执行；DeepSeek 提供证据整理及实现/测试/文档草案。用户已长期授权向配置的 DeepSeek 服务发送任务必要项目资料；不含密钥和无关个人信息。

> 本文件保留各阶段工作与证据；段落中的“最新”“下一步”按当时日期理解。当前状态速览见 [CURRENT_STATUS.md](CURRENT_STATUS.md)。

## 2026-09-18 E3 v5 / Linux00086 实板完成并回传

Mac 已完成 3 对短验收 + 10 对正式实验，26 子轮全部通过，26 个 sequence、494 条去重观测，data_errors/quarantine/taint 均为 0。新 BIT 为 migration-4dma-e3-queue，实读 observer magic 0x49444d414f425305，STAT0x501079fc，Linux 6.5.0-rc2-00086-gd2cd32d78918。未补采或重编；printk 已恢复，串口已关闭。

完整证据：/home/xvhaocheng/work/iommu-platform/out/mac-e3-queue-20260918-130545-run-d01lc9wp/，含原始 short/main JSON、验收/摘要、完整 console.raw.log、版本 run.json、host-validation.json 及 E3_QUEUE_RESULTS.md。归档 run/summary 保留上传前状态快照，回传完成以本节及 Mac upload-receipt.json 为准。正式 A8 16/16 触发、A1 0/4；A8 目标 V 首次 miss 16/16、插入记录截断 9/16、实际触发时旧队头为 A 9/16。中位吞吐 A8 109.29 MiB/s、A1 149.51 MiB/s；前史及插入差异仍在，不证明纯 D128 效应或 HOL。所有不可比样本保留，不扩大矩阵。

用户持续授权：以后每轮结果必须默认回传 Dell 并更新交接，不以仅 Mac 本地保存视为交付完成。此授权不扩大硬件改动或重跑范围。DeepSeek 起草辅助脚本/文档，Codex 审查、修正语义并执行测试、上板及回传。

## 2026-09-18 E3 v5 实施、完整构建和交付完成（最新）

用户要求分析已完成实验、复审计划、实施并编译。00085 的9对/18子轮/342条观测已与UART独立核对，原数据未改；[分析](iommu-platform/docs/e3-diag-results-review.md)。正式A8八轮都触发，预热数量/末页相同，但A预热miss512→312/318，第1对目标/首查不同；旧8ms轮次差未稳定复现，仍不能证明D128/HOL。

已实施v5真实V已接收未选中计数，覆盖A读占AR头时的后续V；新增候选门槛25..31、前史78..95、前两条插入身份46..63。带软件诊断的新D0/D128为diagnostic2/ioctl0x0b，要求v5 BIT。DS50/50和整批轮询保持。[新协议](iommu-platform/E3_QUEUE_PAIR.md)仅3短+10正式（2 A8 D0/D0、6 A8 D0/D128、2 A1控制），不补采；先看覆盖和可比性，不自动进入方法评估。

主机RTL/真实controller联测、驱动76控制+3缓存+96小chunk、E3 CLI49/旧CLI69/rootfs5通过。软件和FPGA完整构建均退出0/manifest passed，BIT/MCS非空，清单哈希及关键导出源码核对通过；内核00086，FPGA结束UTC 00:56:45。构建起点0119909，CVA6 147c4d4a、IOMMU 137f5e5、Linux d2cd32d78918，lock/gitlink一致；后续仅文档提交。首次许可证失败已归档，完整地址/允许联网环境重试成功。成功日志未读，未审查时序报告。

交付[README](iommu-platform/out/e3-followup-20260918/delivery/README.md)，证据out/e3-followup-20260918/{validation,build-status}.json。BIT SHA256 `8f90865ef1fb4f747e7a1f16c3a4992adf4499c56356e0d8d1679b8962d7342e`；payload `13dc664895f570925b6ae59f11cedbfefcb3de331c0d9d6518557b7ce4a7ea36`。标准manifest仅列BIT，MCS非空及哈希另记delivery.json。新版未上板，下一步同时更新BIT和SD软件，再按新协议有限采样；字段78..85是窗口内查询前史，86..93从ARM累计，不能直接与窗口stage entered比较。原时钟/CDC/IO边界保留。

DeepSeek整理证据并给出CLI/桩/CLI测试提案；Codex独立复核原始数据与关键源码、负责RTL/驱动、应用审查提案及执行验证/编译。旧数据、产物及既有脏文档保留，无本机板卡操作。

## 2026-09-18 00085 D0/D0 实板诊断完成（Mac回传）

复用原E3 BIT，STAT0x501079fc，Linux00085启动。3短+6正式配对共18子轮执行/数据/观测检查通过，data_errors/quarantine/taint0，每轮256字段及19条观测完整；固定V0/A1，正式顺序A8,A8,A1,A8,A8,A1，没有补采。证据在 iommu-platform/out/mac-e3-diag-short-20260918-081411-run-1z6a407q/ 和 iommu-platform/out/mac-e3-diag-main-20260918-081411-run-1z6a407q/（相对work），各有records/assessment/summary.json，summary包含完整UART。用户明确批准后已回传。

本轮正式12轮V预热256/256，A提交/完成/排空256/255/1；A8末页均7、下一页0。A8预热miss/S1 PTW首轮512，第二轮312（第1对318），工作量/末页相同但前史状态不同。A8八轮均触发、正式V miss均2，A miss多数5142（第1对次轮5133）；A1四轮无触发且正式miss0。第3对A1第二轮软件A返回到V开始间隔16.9236ms，不能当精确硬件发射差或指认具体中断。硬件MiB/s中位A8两轮109.10/109.07，A1两轮149.36/149.56。

旧约8ms轮次差未在本诊断组稳定复现，但诊断有扰动，不称修复或D128/HOL因果完成。先独立复核预热HPM/软件启动区间，再决定最小状态/触发覆盖改动；本轮无RTL/驱动改动、重编或扩矩阵。已恢复printk、退出root、关闭串口。原哈希/SD回读跳过及时钟/CDC/IO边界保留。Mac详细报告E3_DIAG_RESULTS.md；DeepSeek整理证据表，Codex核对原始代码/JSON并执行测试与板卡操作。

## 2026-09-18 E3 D0/D0 诊断软件构建完成（最新）

用户“Dell可用DeepSeek，继续工作”后，承接已有DeepSeek代码地图和9/17结果，实施软件前史诊断。新增独立ioctl0x0a/能力位及 `idma_e3_pair --diagnostic 1`，固定D0/D0、同映射两轮，记录实际预热数/排空/末偏移、预热HPM及ARM/A/V提交软件区间；旧D0/D128保持。Linux `1b268b89200c`，构建顶层 `c5c4c43`，lock/gitlink一致，内核00085。仅测试去重在构建后修改；无编译源码变更。原文档脏改动保留。

驱动64控制+3缓存+96小chunk、E3 CLI39、旧CLI69、rootfs5通过；软件完整构建退出0/manifest passed，产物哈希一致，成功日志未读。交付 [README](iommu-platform/out/e3-diag-20260918/delivery/README.md)，实验逻辑/字段/命令 [E3_DIAGNOSTIC](iommu-platform/E3_DIAGNOSTIC.md)，证据 `out/e3-diag-20260918/{validation,build-status}.json`。payload `1e4a49b08bfcbcbeb1e86eb17fb0836d373d5950d61b274b885d443f7c458f62`。

新版未上板。只需更新SD软件，复用原E3 BIT `a99ce956…`，无RTL修改/FPGA重编。下一步3短验收+6对诊断（A8,A8,A1,A8,A8,A1；4096/warm256），采集新schema `idma.e3_diag.v1`、各轮19条观测/256字段/UART/退出/taint。无触发不补采、真错误停。先解释前史；软件时间区间不等于硬件发射，诊断有扰动；AR队头触发覆盖问题仍待硬件修订，不宣称因果完成。原时钟/CDC/IO边界保留。

## 2026-09-18 DeepSeek 仓库接手梳理

已由 DeepSeek Flash 思考模式梳理，Codex 对照关键 RTL/驱动/CLI、既有 E3 复核 JSON 及实际版本核查。后续优先读[当前代码地图与状态](iommu-platform/docs/repository-map-2026-09-18.md)。顶层 f8a0374；21 个组件 HEAD/父级已提交 gitlink 与 lock 一致，组件已跟踪文件无修改（未检查未跟踪文件/alternates，不等于完整 make check）。当前仍为 E3 排队定位成立、配对因果未成立；旧 README/AGENTS“尚未上板”已过时。原有文档修改保留，本次仅加接手记录，无实现修改、构建、测试或上板；下一步仍是 D0/D0 前史诊断及触发覆盖修正。

接手时点提醒：上述地图早于本文件顶部的 E3 v5 实施，地图中的“当前没有 D0/D0 接口”、旧触发条件和版本指针均属历史状态；当前操作读 [E3_QUEUE_PAIR](iommu-platform/E3_QUEUE_PAIR.md)，源码行号按需重新定位。本次由 DeepSeek 对照说明、Codex 核对历史和最新协议，仅补此提醒，未重跑构建、测试或上板。

## 2026-09-17 E3服务器独立复核与下一步（最新）

15条配对JSON、30子轮、570条观测与UART逐项一致，正确性/退出/taint/窗口/AXI/path/E3/实际延迟检查通过，原文件未改。证据 `iommu-platform/out/mac-e3-main-20260917-222837-run-p0lbvbv7/server-review.{py,json}`；[详细分析](iommu-platform/docs/e3-results-review.md)。

新定位：正式第一轮A1→A8，V读排队18→596.78周期、写4→263.62；选中后请求/自身服务/出队始终1/4/2。V写AW→WLAST 1111.31→1439.79，WLAST→B约18不变。内部增量落在选中前，不能直接称HOL/纯PTW或将累计时间相加。

正式A8双触发0/10，因果验收未完成。第8/9对双轮均未触发仍第二轮快约8ms，轮次状态变化不是D128效果。静态代码确认触发只看AR FIFO头为V，A读在服务期间仍占头，漏掉V在其后的场景；不能据此断定首轮0触发唯一原因。10个实际触发A均为写，插入数均1（无完整身份）；正式5个V首查hit的跨配对探索性残余等待D0中位282、D128中位408，差126周期，仅线索。

下一步先修对照条件：少量D0/D0软件诊断记录预热/启动前史（现CLI不支持，需软件改动，现BIT可用），并补足目标V排队识别及触发门槛诊断（需RTL/新BIT）；必要时有限请求/统一硬件启动，之后恢复D0/D128。不要扩样本代替可比性，不直接进入方法评估。本轮仅复核/文档，无RTL/软件修改、构建或上板。

## 2026-09-17 E3实板采样完成：执行通过，配对因果条件未满足

新migration-4dma-e3 BIT/Linux00084启动，STAT0x501079fc。按交付完成3短+12正式配对（30子轮），每轮256字段及19条观测齐全；数据错误/quarantine/taint0，任务/AXI/path/E3守恒和触发延迟精度检查通过。没有补采、扩矩阵或修改RTL。

关键限制：正式10对A8第一轮触发0/10、第二轮8/10，两轮均触发0/10；首轮D0/128已交替。第一轮全V miss0/A miss6160；第二轮8次V2、2次V0，A miss5133/5136。硬件中位第一/二轮155.245/146.567ms（103.06/109.16MiB/s），不可当128周期因果效果。2对A1四轮无触发。短A8虽双触发但身份/前史/首次命中不同且有插入，亦非干净对照。先独立复核双轮状态及触发门槛，不以加样本替代问题解释。主机检查通过只说明测量正确性，不证明HOL。

证据：iommu-platform/out/mac-e3-short-20260917-222837-run-p0lbvbv7/与iommu-platform/out/mac-e3-main-20260917-222837-run-p0lbvbv7/，各含records/assessment/summary.json（summary含完整UART，路径相对work）；Mac报告E3_STAGE_RESULTS.md。短测一次hrtimer延迟13439920ns保留。printk已恢复、退出root/关闭串口。原哈希/SD回读跳过及时钟/CDC/IO边界保留。

## 2026-09-16 E3实现及软硬件构建完成（最新）

已实现v4内部四段、首次查询、有界A/V及COMP写记录、单次A PTW响应D0/128，同一映射内分别预热/排空/核验两轮；DS50/50及批量轮询保持。源码构建起点bea8f57、CVA6 44068498、IOMMU1519510、Linux94a519eebf07，lock/gitlink一致；内核00084。新RTL/旧回归、驱动56场景（含3缓存）+96小chunk、E3 CLI23及rootfs验证通过，证据`iommu-platform/out/e3-service-20260916/validation.json`，仅模型验证。

软件及FPGA均退出0/manifest passed，BIT/MCS非空，FPGA结束UTC 09:27:41。交付[README](iommu-platform/out/e3-service-20260916/delivery/README.md)，入口[E3_SERVICE](iommu-platform/E3_SERVICE.md)。BIT SHA256 a99ce9565b82449ca5eb0a8afc18c8b79cdf9e23a5a4e9197aafae02b5df98d4，payload c9b1ac46d6c9ef623f2a814477fb92d7ab62441eff97278cd3376f254d621a33。成功日志只保存，未审查时序；旧结果/产物保留，原时钟/CDC/IO边界不变。

尚未上板。下一步用配套BIT和SD payload做3项配对短验收（Solo/A1/A8，各两轮），然后10对A8+2对A1、首轮D交替。每命令一个新schema JSON含两轮256字段，按两条seq分别收齐各19条旧观测，完整UART保留；无触发、首次V缺失、顺序变化不补采，真实错误停止。比较触发后V剩余等待并分报完整排队/自身服务，不能只看整批耗时或宣称HOL。

## 2026-09-16 v3结果服务器独立复核与E3计划（最新）

33条JSON及627条观测与回传UART逐项一致，退出/taint/正确性/排空/身份/窗口/AXI/路径守恒通过；原始结果未改。证据`iommu-platform/out/mac-path-main-20260916-164633-run-vvguros8/server-review.{py,json}`；[结果分析](iommu-platform/docs/path-results-review.md)。正式V miss全0；A8较A1硬件+47.75961ms/+44.57%，V内部读25→601.04/写11→268.68周期，入口1/出口0不变，写后段1130.37→1455.66。历史50/50吞吐差≤0.133%，不是严格开关开销测量。E4分段定位目标完成，仍未证明纯PTW或HOL。

下一步[E3因果实验卡](iommu-platform/docs/e3-shared-service-plan.md)：同一请求细分排队/自身翻译/出队，锁存首次IOTLB查询；V已排队时选择性对一笔A PTW响应D=0/128周期，一窗口一次，同映射成对会话，10对A8+2对A1控制；AW/WLAST/B解释既有写后段增量。全部为待实现设计，需新RTL/软件/定向验证及一套新BIT，尚未启动。不能用当前命令假装已具备控制，也不扩页数/重做旧矩阵。

本轮仅复核和文档，无RTL/软件修改、构建或新上板。保留轮询及成功日志仅保存的要求。A8软件窗口174.046ms与硬件154.920ms分报，done_to_read中位18.673ms；短测hrtimer日志和原HPM/哈希/SD回读/时钟CDC边界保留。

## 2026-09-16 v3分段路径实板完成（Mac回传）

配套migration-4dma-path / Linux00083启动，STAT0x501079fc、path_observe=Y。固定V0/A1，3短验收+Solo/A1/A8各10次正式全部通过；33组19条观测完整且守恒通过，data_errors/quarantine/taint0。硬件吞吐中位283.32/149.31/103.28 MiB/s，与上次50/50偏差均<0.14%；正式V miss全0，A8 A miss/S1 PTW全6160。V stage1读/写均值由A1的25/11升至A8的601/269周期，写stage3约1130→1456；不能称纯PTW或证明HOL，累计段时间不能相加。短测有一次hrtimer延迟日志，软件/硬件窗口分报。

证据：iommu-platform/out/mac-path-short-20260916-164633-run-vvguros8/（3次）及out/mac-path-main-20260916-164633-run-vvguros8/（30次），各含records/assessment/summary.json，summary保存完整UART；两份assessment passed。Mac已恢复printk、退出root/关闭串口。没有重新编译/扩矩阵，下载哈希及SD回读按用户要求跳过；时钟/CDC/IO原待办保留。

## 2026-09-16 分段路径观测构建交付完成（最新）

用户“继续”授权执行后续机制计划第一步。已实现v3被动观测：按源DMA/读写/同ID顺序匹配，记录入口→TR、TR→COMP首次有效、COMP反压、COMP地址→RLAST/B四段；A跨窗口首尾单列，计数/ID/溢出错误拒绝有效结果。Linux批量默认要求v3，旧三行加16条path日志，全部在排空/验证后读取；CPU整批轮询及DS50/50保持。入口[E4_PATH](iommu-platform/E4_PATH.md)。

构建源码：顶层a2e8475、CVA6 a1572926、Linux45bf76b15e36，lock/gitlink同步。真实RTL17场景+缩位计数/area/sum溢出、旧observer28+6、frontend联测4、驱动47场景+96小chunk通过，测试仅事件/MMIO模型。软件及FPGA均已退出0/manifest passed，BIT/MCS非空；内核00083。BIT SHA256 62f822d19851da0309b1a7e085197661caa72f6366fccf34d721fa418b941c29，payload 031025c1fb1551ab0e82610027f76106e7b2efe3052b45cda62ea39dc0173c6a。profile migration-4dma-path；日志out/e4-path-20260916/{software,fpga}.log，按要求成功日志未读、未审查时序报告。交付[README](iommu-platform/out/e4-path-20260916/delivery/README.md)，含BIT、SD payload、版本/验证/构建清单和SHA256SUMS。新版未上板，无新性能结论。

下一步固定V=DMA0/A=DMA1，仅8项Solo两次及64项A8无预热短验收，随后Solo/A1/A8各10次4096项/warm256；每次同seq三条batch+16条path。原时钟/CDC/IO待办保留；旧结果/产物未删，不自动进入E2/E3控制或扩矩阵。

## 2026-09-16 后续机制计划已整理（最新）

用户要求制定接着执行的计划，见 [后续执行计划](iommu-platform/docs/next-mechanism-plan.md)。当前只规划，未改RTL/软件、编译或上板。顺序：DS50/50补入口/内部/出口/后段被动分段观测→新增字段必要短验收及固定角色Solo/A1/A8各10次→按证据选择E3内部阻塞、E4出口/数据因果对照或必要E2状态探测→一项方法及直接基线。E2非必经前置，V miss=2不自动触发扩展。

先复用现有observer/Linux读出；需要按设备/读写/AXI ID及顺序匹配、处理A跨窗口在途，不能将并行累计等待相加作整批耗时。只先构建一套新DS50/50；按用户要求日志只保存、有报错才检查。方法阶段门槛为可重复等待位置+区分性干预+可控点及A代价口径，不要求先证明HOL或完成所有分支。

## 2026-09-16 DS50回切服务器独立复核（最新）

30条JSON/observer与回传UART逐项一致，退出/taint/数据/排空/身份/计数/窗口/AXI守恒通过。证据 out/mac-batch-ds50-20260916-135544-run-eawziww2/server-review.{py,json}；[结果解释](iommu-platform/docs/ds50-retest-results-review.md)。A8硬件中位154.99→118.19→154.88ms，A8较A1额外耗时44.71%→10.50%→44.72%；三组回切吞吐与首次中位差均<0.16%。回切A8的8/10为V miss0/A miss6160；第14、16项V2/A5141（硬件时间中位145.05ms），全保留。

回切复现目标完成，DS敏感性证据增强；一次三阶段切换未固定跨启动映射/状态，仍不能称纯PTW代价或HOL证明。下一步方向为IOMMU翻译前/内部与翻译后数据等待的定位；未实施新观测、未构建/上板，无需再把本轮复测列为待办。

## 2026-09-16 50/50 BIT回切复测完成（最新）

已有 migration-4dma-batch（DS50/50）BIT回切成功，STAT0x501079fc，Linux00082正常。固定V=DMA0/A=DMA1，Solo/A1/A8各10次，30/30通过；硬件中位283.33/149.51/103.31MiB/s，软件中位282.76/147.94/93.25MiB/s。V miss/S1 PTW Solo/A1中位0，A8中位0（少数样本可2），A miss/PTW中位6160。A8恢复到约103MiB/s，与DS0/0的135.38MiB/s区分明确。数据错误/quarantine/taint0，observer检查通过。原始结果已回传out/mac-batch-ds50-20260916-135544-run-eawziww2/，Mac报告DS50_RETEST_RESULTS.md；无重新编译/刷卡，下载哈希/SD回读按用户要求跳过。

## 2026-09-16 DS0结果服务器独立复核（最新）

DS0的60条JSON/observer已逐项与回传UART对照，退出/taint/正确性/身份/计数/窗口/AXI守恒通过，原数据未改。证据 out/mac-batch-ds0-20260916-132332-run-ebgxhirr/server-review.{py,json}；[结果解释](iommu-platform/docs/ds0-batch-results-review.md)。相比历史同角色50/50，Solo/A1基本不变，A8～A64吞吐+31.14%～35.63%；A8相对A1额外硬件耗时44.71%→10.50%，支持DS敏感性，未证明HOL或纯翻译代价。

比较边界：未同期回切50/50；DS0 A8～A64共40项V miss均2，A8的A miss6160→5141，映射/内部状态及HPM边界未严格匹配。建议下一项使用已有50/50 BIT回切，只复测Solo/A1/A8固定角色各10次，无需重编；尚未执行。本次仅复核/记录，无新RTL/软件/构建/上板。

## 2026-09-16 DS0/0实板正式对照完成（最新）

baseline-4dma-batch加载成功，STAT0x501079fc、Linux00082；固定V=DMA0/A=DMA1，六条件Solo/A1/A8/A16/A32/A64各10次，4096项/warmup256，60/60通过。按用户要求不短校准、不交换角色。数据错误/quarantine/taint0，observer计数/覆盖/窗口/AXI守恒通过。硬件中位MiB/s 283.17/149.59/135.38/131.61/129.98/128.64。

较此前同角色DS50/50，Solo/A1基本不变，A8～A64提升约31%～36%；旧数据为历史对照，非同期交错采样。新版A8～A64共40项V miss/S1 PTW均2，Solo/A1均0；A miss/PTW中位0/5141/6940/7501/8331，S2全0。不能直接把提升折算单次PTW代价或证明HOL。原样保留全部样本，无补采。

证据out/mac-batch-ds0-20260916-132332-run-ebgxhirr/，含原JSON/observer、统计及完整UART；Mac报告DS0_BATCH_RESULTS.md。未修改RTL/Linux或重编；原哈希/SD回读跳过、HPM/时钟/CDC/IO签核边界保留。已恢复printk并退出root，当前保持DS0/0配置。

## 2026-09-16 DS0/0批量对照FPGA构建完成（最新）

按下一步计划新增 `baseline-4dma-batch` 配置，仅将 DS input/output 从50/50改为0/0，批量RTL、Linux和软件保持不变。FPGA构建退出0，manifest `out/manifests/fpga-baseline-4dma-batch.json` 为passed；日志 `iommu-platform/out/e4-ds0-batch-20260916-fpga.log` 仅保存未审查。BIT `out/baseline-4dma-batch/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`，SHA256 `c6555d14245935820224a2cae2d835a65bade81fe993d5f8d626ec0ff2724050`；MCS同目录，SHA256 `b4dbe765ba25ae5d043b9782c488adb32b6402b2f2b07f1ccfe5540992461f7e`。尚未上板或采样，待用同一批量矩阵比较50/50与0/0。

## 2026-09-16 batch结果服务器独立复核（最新）

用户询问是否达预期。104条（短14+主60+扩展30）原JSON/observer逐项与回传summary内完整UART一致，正确性/退出/taint/任务/窗口/AXI守恒通过；原数据未改。证据 out/mac-batch-main-20260916-112954-run-9sa69rhc/server-review.{py,json}，解释见 [batch结果复核](iommu-platform/docs/batch-results-review.md)。

本阶段自主供给、持续重叠、硬件慢化目标达成，机制未定：正式批次gap恒4095周期，共跑在途重叠约99.7%。V=DMA0的Solo/A1/A8硬件中位56.56/107.10/154.99ms，A8较A1耗时+44.7%/吞吐-30.9%；A1无miss已明显变慢，不能把全部损失归为翻译。90性能样本88个V miss0，主42/扩展28各2，完整保留。A16/32/64为后续固定角色扩展，未同期重采基线。仅复核及更新文档，无构建/新上板；下一步机制区分，不自动实施E2/E3。

## 2026-09-16 A16/A32/A64固定角色实板扩展完成（最新）

用户明确不短校准、不交换角色；固定V=DMA0/A=DMA1，4096项/warmup256，各10次轮换条件，30/30通过。数据错误/quarantine/taint0，30组observer窗口/计数/覆盖/AXI守恒通过。硬件MiB/s中位98.27/95.95/94.84，A miss/S1 PTW中位7246/7743/7976；第28项A16的V miss/PTW2，其余29项V0，完整保留未补采。S2全0。

证据out/mac-batch-ext-20260916-114036-run-8pjv_djc/，包含原始JSON、观测、统计、完整UART；Mac报告BATCH_EXT_RESULTS.md。未重编或刷卡，旧Solo/A1/A8未重采，仅历史对照；不能单凭趋势归因为HOL/纯翻译。新增页数未独立短校准、未反向角色验收，原HPM/时钟/CDC/IO待办保留。

## 2026-09-16 batch实板正式矩阵完成（最新）

Mac新BIT/Linux00082完成14短校准+60正式样本，74/74通过；正式4096项/warmup256、Solo/A1/A8、双角色每条件10次轮换顺序。数据错误/quarantine/taint均0，observer计数/覆盖/H+gap/AXI守恒通过。硬件中位吞吐约283/149/103 MiB/s，双路在途重叠99.69%～99.79%。第42项DMA1+A8的V miss/PTW2，其余59项V0；A8 miss/PTW中位6160，第42项5141，完整保留无补采。不能归为纯翻译或HOL；硬件窗口与软件/HPM窗口分开统计。

原始JSON、观测、统计和完整UART已回传 out/mac-batch-main-20260916-112954-run-9sa69rhc/；短校准 out/mac-batch-short-20260916-111626-run-n8mg7wsz/。Mac报告BATCH_MAIN_RESULTS.md，RTL/Linux未改、未重编，下载哈希/SD回读按用户要求跳过。原时钟/CDC/IO与完整HPM语义待办保留，不自动启动E2/E3。

## 2026-09-16 硬件批量版构建交付完成（最新）

用户已批准改RTL，并明确编译节约token：日志只保存，有报错才读取，不逐项审查成功日志/时序报告。实现入口 [E4_BATCH.md](iommu-platform/E4_BATCH.md)。硬件每项4KiB完成后自主接续并在页环回绕，CPU轮询整批；原软件模式保留。新CLI `--batch` / ioctl0x08 / JSON `idma.batch.v1`，强制observer v2，H+硬件gap窗口冻结于末项硬件完成，done_to_read另计；先A后V，A有限上限并在CPU发现V完成后STOP/排空。旧P/R不能套用。

真实frontend批量11+旧模式对照12、observer28+2溢出、双frontend/observer联测4、驱动41+96小chunk及CLI回归通过。软件与最终FPGA构建均退出0，manifest为passed，BIT/MCS非空。Linux `b7979a1a06ac` / 00082，软件构建顶层 `e47b48e`。发现重复相同批次需清旧计数后主动中止首轮FPGA（130），修正回归通过；CVA6 `7ed43840cab2`、FPGA构建顶层 `3198fef`，独立profile `migration-4dma-batch`，软件未因RTL修正重编。 文档提交 `7b321a2`，各源码提交均保留在本地。

交付 [README](iommu-platform/out/e4-batch-20260916/delivery/README.md)，含新BIT、SD用fw_payload.bin、说明、版本/构建清单及SHA256SUMS；证据/日志 `iommu-platform/out/e4-batch-20260916/`、build-status.json。首轮导出在out/migration-4dma-batch-aborted-before-repeat-fix保留。新版未上板；下一步Mac同时更新BIT/SD软件，先重复8项Solo、换角色、多页及8/64项A1/A8校准，再决定正式采样。旧E1/E4/用户文档保持，时钟/CDC/IO待办保留；本轮仅确认构建退出及产物，不审计成功日志/时序报告、不宣称新增时序签核。

## 2026-09-16 E4服务器独立复核与下一步（最新）

已复核Mac回传105条原JSON及86组观测，身份/任务数/正确性/HPR与AXI守恒通过；原数据未改，证据见 `iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/server-review.json`。详细 [E4结果复核](iommu-platform/docs/e4-results-review.md)。60主样本H约56ms、全部V miss0；按同角色均值差，新增时间99.08%～99.92%在P。60样本（其中40共跑）both全0，V/A地址等待也为0；开关配对中位最大绝对变化0.5051%。不把P全称CPU，不宣称HOL或IOMMU无干扰。

建议下一步保留轮询，先设计两路独立有限批量供给/软件轮询批次完成，校准真实请求重叠后再定位硬件干扰。仅建议，未实施/构建；用户未要求本轮直接改供给。现有HPR假设逐任务DONE读取，自主重发不能直接复用该有效性判据，需一起设计窗口。完整UART仍在Mac，本次核查JSON及回传配对，下载哈希/SD回读未做的边界保留。无需重复本轮105试次，不自动启动E2/E3。

下一步问答已进一步限定为“硬件接续有限任务、软件轮询批次”，仍每路最多一个任务、4KiB拷贝、V固定页/A1或A8回绕、A覆盖V窗口后安全排空；先8/64项校准再主矩阵。原n_trans非DEFAULT循环逐页递增且无回绕，不能直接当新版拷贝接口。新硬件窗口按任务发出/实际完成定义，旧逐次DONE的P/R分开保留。详见E4结果复核文末，仅方案细化，尚无新代码/构建。

## 2026-09-16 E4新版实板验收完成（最新）

Mac以新observe BIT和Linux00081完成105/105：9短测、36开关开销、60主样本。86组time/axi_v/axi_a检查通过，data_errors/quarantine=0、taint0，开关配对中位变化最大0.506%，低于预定1%门槛。软硬件未再修改。

主样本每条件10次：V H约56ms基本不变，P从Solo约346–349ms增至A1约767–769ms/A8约804–805ms，R约185–191ms；60主样本AXI both均0。支持优先核查供给/完成读取路径，不把P全部当CPU、不把H全部当IOMMU，不宣称HOL。下一步供给方式改动需另行明确，不自动实施E2/E3。

证据：`iommu-platform/out/mac-e4-stage-20260916-094001-run-kzi_s0zc/` 内105条原JSON、86观测组、逐项判定及版本边界。完整UART在Mac results/serial/20260916-094001-console-x8xsc_f0/console.raw.log，详细说明E4_STAGE_RESULTS.md。首轮直接printk缺字，从内核dmesg恢复完整观测；后续临时loglevel4并逐次dmesg读回，结尾恢复7 4 1 7、observe=N、taint0，串口已退出。用户要求未做下载哈希/SD回读，期望哈希不能称Mac实测。旧E1与本轮不合并，原时钟/CDC/IO待办保留。

## 2026-09-16 保留轮询、E4观测构建交付完成（最新）

用户明确“暂时保留轮询,继续”，已实施Linux可读被动DMA观测，保留热循环、供给/缓存/排空和原ABI。入口 [E4_OBSERVER.md](iommu-platform/E4_OBSERVER.md)。CVA6 `540544ce`、Linux `853e60b234c3`，构建顶层 `228aed2`，文档及补充测试提交 `e53aaf9`，独立profile `migration-4dma-observe`。24+2个真实观测场景、12个frontend前后对照任务、驱动原19+新增11场景及96小chunk组合通过，软件/FPGA完整构建均退出0。

默认关闭，由 `/sys/module/idma/parameters/observe` 开启并检查硬件标识；H/P/R和AXI在末个DONE读取时冻结，排空/校验后输出同seq的time/axi_v/axi_a三条记录。P/R含控制MMIO等待，不能全称CPU时间；缺硬件/无效观测不能伪PASS。

交付 [README](iommu-platform/out/e4-observer-20260916/delivery/README.md)，内核00081；BIT SHA256 `41fa23d0aee476b603111df192c612f3d40e844067211564d2b8b175a60b9b90`，payload `d31fc2db4f620704563c8e4d75a50ce4a62e9e50b0a6e047b4f14cbc073d4ba6`。布线WNS0.177ns/WHS0.027ns、累计违例0，Slice96.55%；原时钟/CDC/IO待办保留。初始许可证网络失败及一次缺RISCV的手动重试保留，最终manifest记录同导出源码续建成功。

**新版尚未上板。** 下一步Mac下载/核验新BIT与payload、按已有SD/JTAG流程更新，先观测关闭短功能试次→开启短Solo/角色互换/共跑及8/64任务计数校准→36条开关开销→60条E4主样本。旧BIT/软件/E1数据及用户研究文档改动保留；无需重建已通过版本，不扩充E1或启动E2/E3。

## 2026-09-16 E4 最小观测方案细化（最新）

用户“继续”后已把后续收敛成 [E4供给/完成观察实验卡](iommu-platform/docs/e4-supply-timing-plan.md)。优先Linux可读的被动硬件汇总，沿用Mac下载/SD/串口，不把未验证的ILA读取流程作为前置。用同一硬件时钟记录任务分配ID→完成、完成→对应DONE读取、DONE读取→下次提交，补mux两侧请求/返回/等待及重叠观测；不能把外设DONE读取时刻当CPU精确发现时刻。首末自动门控、A跨预热边界在途状态持续维护、ID/溢出/窗口恒等式校准等边界已写明。

源码另确认DMA frontend处于WAIT时等本任务完成，8项FIFO不等于当前已能排队提交8项；本轮观测保持原供给方式。拟36个开关观测对照后60条Solo/A1/A8双角色主样本；新BIT/软件与原产物分开，新增产物须验收。当前只完成方案和代码落点核对，未改RTL/Linux、未构建/上板、未启动E2/E3。下一项实施是观测模块及对应边界测试，不扩充原E1参数矩阵。

## 2026-09-16 E1 服务器复核与后续建议（最新）

已读取Mac E1的协议/assessment及全部144条原始JSON，独立核对成功/完成数/校验/隔离状态，80主样本条件与时间一致。复核见 [E1结果与下一步](iommu-platform/docs/e1-results-review.md)，统计保存在原回传目录的server-review.json，原记录未改。主样本V miss全0；A1也全0但软件耗时已为Solo的1.728倍，A8/32为1.787～1.795倍且A miss8190。A8/32相对A1只额外约3.5%～3.9%；V backend active各组均约270万周期、基本不变。不能把约79%慢化归为纯翻译损失或HOL。

下一步建议优先既定E4供给/计时核查：少量Solo/A1/A8，加最小软件汇总和板上请求到达/完成/重叠观测，先区分CPU供给/发现延迟与共享硬件等待，再按证据安排E2/E3。硬件观测尚未导出、需要新BIT；E2仍需同映射生命周期分阶段接口。建议未实施，本轮无代码修改/构建/新上板。保留E1已完成的范围，不重跑旧smoke或默认补全所有E0项目。

## 2026-09-16 Mac 当前双路 E1 已完成（最新）

缓存同步内核00080与SC.W修复BIT已在Mac实板通过两次新启动29组功能试次、单DID32组和双DID12组归属检查；其后18个探索性能样本独立保留。本次用户确认仅做当前双路E1，不含E2/E3。

本轮144/144通过：16项有限HPM/冷热/进位回绕检查，48个预热/窗口敏感性样本，80个主样本（双角色，Solo/A1/A8/A32各10次）。data_errors/quarantine=0、taint0，脚本退出0。A1页软件观测耗时约Solo的1.73倍，A8/32页约1.79倍；敏感性归一化变化最大0.427%。未重建或刷卡，Mac串口已退出。

证据：`iommu-platform/out/mac-e1-stage-20260916-032650-run-rbo67pvd/` 的records.json、assessment.json、summary.json（内含采样前协议）。Mac完整UART在results/serial/20260916-032650-console-jz4u_u_b/console.raw.log，说明E1_STAGE_RESULTS.md/HPM_REMAINING.md。

范围严格限定软件计时E1、DS50/50。完整E0/P2仍未签核：S2正向、溢出IRQ、空窗口开销和硬件请求重叠缺接口，DDTW相邻事件漏计及时钟/CDC/IO待办保留；PTW字段不是独立页表访存量。无纯硬件慢化/HOL结论。下载哈希/SD回读按用户明确要求未做，不将期望哈希写成实测值。


更新：2026-09-16。项目交接摘要；以用户新要求和实际文件状态为准，不代表完整聊天历史。

## 2026-09-16 DMA 缓存同步修正版（最新，优先于下文历史状态）

用户回传 `DMA_DIAGNOSTIC.md`：诊断版新启动首次 warmup32 通过，随后完整 smoke 首组 warmup0 失败，其余13组未运行，Linux正常/taint0。首错 offset912，dst=eb（恰为初始化值 ~14），FENCE.I 后=14，src始终14；支持 CPU 缓存旧视图，但尚未确认唯一根因。两次 JSON、首错和摘要已回传 `out/dma-smoke-debug-20260916/mac-20260916-*/`，完整串口仍在Mac。watchdog/遇错停止已在板上工作。

本轮仅补当前 CVA6 平台 DMA 开始前和安全排空后的缓存同步，均在测量窗口外；保持真实错误EILSEQ，不改ABI/RTL。另确定并修复 `iova_to_phys` 将IOVA页号错误拼入PA的问题，旧 `*_pte_pa` 异常大地址不代表硬件实际翻译。Linux `3b47ab1645e412095b601e3be68b6347b9fad213`，构建顶层 `76de9ee73656cc90bdae5331be7020b5f905e205`。19个调度/诊断/缓存场景、96小chunk组合、16软件查询场景通过；移除任一同步回归能检出，旧查询16场景12失败/修正后0失败。完整软件构建及交付校验通过，修正版尚未上板。已有用户文档脏改动保留。

最新入口：[DMA 缓存同步修正](iommu-platform/DMA_CACHE_SYNC.md)，交付 `iommu-platform/out/dma-cache-sync-20260916/delivery/`，内核 `6.5.0-rc2-00080-g3b47ab1645e4`，payload SHA256 `6f52ad85c4d2fc87e97301d98f1394216b843cd8be601b2965eca429caf74053`。需更新SD软件并回读，沿用SC.W修复BIT（df7c240…），旧交付保留。下一步新版首次warmup32→完整14组smoke；通过后另一新启动直接完整smoke。任何失败即停，不进入性能矩阵。功能通过后重新校准HPM/开销，不混用旧版性能样本；时钟/CDC/IO待办保留。

## 2026-09-16 DMA smoke 失败（历史诊断阶段）

用户提供 `DMA_SMOKE.md`：SC.W 修复 BIT 已上板，Linux 登录/proc 正常、taint=0，但 DMA smoke 未通过。原脚本依赖缺失的 BusyBox timeout，退出127且未发DMA；Mac临时兼容脚本首例 DMA0 单页/4096chunk/8次/warmup0 通过，下一例 warmup32 出现 EILSEQ/data_errors1。其余12例未运行，双路/HPM端到端校准未完成。输入摘要已归档 `iommu-platform/out/dma-smoke-debug-20260916/`；原始串口/JSON仍在Mac，尚未收到。

诊断与交付入口：[DMA smoke 最小修正](iommu-platform/DMA_SMOKE_FIX.md)。两组同时改变预热和调用顺序，不能直接归因预热。静态查到 Linux 默认 coherent 与无外部DMA失效的WT缓存契约缺口；物理页复用后CPU读旧值是候选根因，尚未由实板首错值确认。最小驱动诊断 Linux `53d3113dee74761515fc08b6a4f984d41227c50f` 已提交：仅失败/排空后保存首错、当前CVA6 FENCE.I刷新后重读、记录IOVA/PA/软件页表PA，仍保留EILSEQ；不改ABI/正常测量/RTL，不把探针当最终一致性修复。主机原12+4诊断场景、96小chunk组合与驱动目标编译通过；timeout依赖修正和诊断软件交付结果以专门文档为准。此前用户的研究/计划文档脏改动保留，不代为提交。

本轮软件已构建交付：`iommu-platform/out/dma-smoke-debug-20260916/delivery/`，构建顶层 `38774213f7cb8f3ea4240e6e127e363ae5f6be02`，Linux版本 `6.5.0-rc2-00079-g53d3113dee74`。独立 `platform-timeout` 已编译进rootfs，避免依赖缺失applet，smoke失败也收集dmesg；9项watchdog、5项rootfs检查通过，产物SHA256全部通过。新payload哈希 `58ba4d4ae9601ba8fad6064e217dabef51f897ac5461ca07fcc77eb9232836bb`。此次要更新SD软件，继续使用现有SC.W修复BIT；旧交付 `out/experiment-v1/` 保留。下一步由Mac按既有SD流程更新并单次复现DMA0，保存首错整行；新诊断尚未上板，数据问题未宣布修复。详见专门文档，不重建RTL或继续性能矩阵。

## 2026-09-16 Mac 实板异常及修复（最新，优先于下文准备阶段历史）

Mac 已完成 SD 更新/回读与 1 MHz JTAG 配置，Linux 进入用户空间后 proc/dentry 警告、generic_permission Oops，登录失败。问题和完整日志已同步 `iommu-platform/out/board-bringup-20260915-mac/`。实板诊断复现高半部 SC.W 失败误报成功，1000/1000；WT AXI 适配器在等待 B 响应时清掉字节偏移。CVA6 已修为保持偏移，提交 `7f086b73f19d8e4d48ed74a671e8315f827d2ce0`；真实生产模块回归旧版 54 场景/6 失败，修复版 54/0。

用户已明确授权 RTL 修复、回归及重建新 BIT。新构建独立使用 `migration-4dma-scfix`（仍 DS 50/50、50 MHz、四 DMA），旧构建不覆盖；构建状态见 `iommu-platform/out/board-bringup-20260915-mac/fpga-build-status.json`，详见 [SC.W 修复与验证](iommu-platform/SC_WORD_FIX.md)。Linux 软件未改、SD 不需因这处 RTL 修复重写。新 BIT 上板、登录/smoke/HPM 尚未验证；Mac 板卡暂处 CPU 调试暂停状态。原时钟/CDC/IO 约束待办保留。初次调试寄存器缓存读取问题和需重新启动的边界在 Mac 的 LINUX_BOOT_FIX.md 及服务器证据目录中已记录。

本次上板准备：用户使用 M1 Pro MacBook，代码/编译在实验室服务器；明确不想同时使用另一个 Windows 电脑。搜索后更新建议为 Mac 原生 openFPGALoader（项目明确支持 Genesys 2，Homebrew 有 Apple Silicon 包）下载服务器 `.bit`、SD 启动 Linux；需 Vivado 界面时再试 Parallels + Windows 11 ARM，已有其他 Digilent 板卡成功案例，当前用户组合未实测。Windows 实体机仅后备，非已接受方案。已定向核对 Boot ROM 的 GPT/原始分区读取及现有 JTAG/OpenOCD 入口；无 SD 启动完整 Linux 尚未验证，未安装本地工具、制卡、重建或上板。证据与边界见 [上板电脑与启动分工](iommu-platform/docs/board-host-setup.md)。

## 先读这些

- Mac 本地任务独立交接：[MAC_FPGA_HANDOFF.md](MAC_FPGA_HANDOFF.md)。包含单 Mac 下载方案、本地检查顺序、服务器 BIT 绝对路径及 2026-09-15 实算 SHA256；仅生成交接，尚未操作本地 Mac 或板卡。

- 当前开发目录：`/home/xvhaocheng/work/iommu-platform`。旧工程 `/home/xvhaocheng/riscv-iommu-demo-refact` 和 `platform-archives/` 用作历史/备份，默认不在其中开发。
- 研究重点是多 DMA 共享 IOMMU 的地址翻译干扰。容量影响、HOL、仲裁/互连竞争和软件供给影响是待区分假设，不能把吞吐变化直接写成机制证据。
- **最新范围：用户要求“只做必要的软件修改”，避免软件臃肿。已仅修小 chunk 错源校验、补现有回归/smoke；不自动扩展限速、日志字段、HPM 槽、运行器或新模式。2026-09-15 已编译并更新交付镜像，包含本次修复。**
- 用户已授权执行新实验软件并清理旧 benchmark。新版 `idma_experiment`、Linux 测量与 per-DID HPM 会话、rootfs/板上 smoke 已实现，主机检查和完整软件构建通过。旧四个程序及旧 ioctl 已退役，历史和旧产物保留。
- **FPGA 是实验主线，仿真按需辅助，不是开始实验的前置门槛。当前主机没有连接 FPGA，用户尚未准备 SD 卡。** 新版尚未上板运行/HPM 校准；SD 启动方式继续保留，实际卡布局和加载文件需准备介质时核实。
- 本轮未改 RTL、未重建 FPGA。已有 migration-4dma BIT/MCS、HPM RTL 定向测试和迁移阶段离线恢复结果保留；时钟/CDC/IO 约束审查仍待完成。
- 源码面板已配置只显示顶层、CVA6、IOMMU、Linux；没有删组件或 `.git`，仍是顶层加 21 个组件。不要自动执行此前讨论过的裁剪或 Git 合并。
- 2026-09-15 按用户要求执行 `make software JOBS=12`，退出 0，已更新 `out/experiment-v1/` 及版本/SHA256 清单；日志仅保存，未逐项审查，未重跑既有测试或 FPGA 构建。旧交付保留在 `out/experiment-v1-before-20260915T140327Z/`。下一步是设备启动、smoke 和 E0 校准。

## 按任务找文件

平台相对路径均相对于 `iommu-platform/`，按需定向读取，不默认扫描 Linux、`.git`、`out/` 或恢复副本。

| 任务 | 入口 |
|---|---|
| 使用、构建、Git | [README](iommu-platform/README.md)、[构建说明](iommu-platform/docs/build.md) |
| 本轮实现、参数、字段边界、测试命令 | [实验入口 v1](iommu-platform/docs/experiment-v1.md) |
| v1 实现复审与建议改动 | [复审记录](iommu-platform/docs/experiment-v1-review.md)：小 chunk 软件问题已修、HPM RTL 问题待修；其余能力仅按所选实验需要实施 |
| 组会讲解 access pattern 和实验逻辑 | [组会讲解稿](iommu-platform/docs/experiment-v1-presentation.md)：地址序列、工作集/拷贝量、时序、对照、一分钟口头稿 |
| 最新构建证据、历史 FPGA 报告、待办 | [验证记录](iommu-platform/docs/validation.md) |
| 新版软件交付 | [交付说明](iommu-platform/out/experiment-v1/README.md)、[版本/SHA256](iommu-platform/out/experiment-v1/delivery.json) |
| 旧 benchmark 为什么退役 | [核查](iommu-platform/docs/benchmark-audit.md)，描述的是替换前版本 |
| E0～E4 假设/对照/判据 | [实验总设计](iommu-platform/docs/experiment-plan.md) |
| 投稿导向的问题、缓和措施与场景推演 | [论文推演草案](iommu-platform/docs/paper-scenarios-mitigation.md)：结果分支、创新性复核、措施前提/代价、公平对照和先行工作 |
| 整篇论文的最新逻辑与优先级 | [论文逻辑](iommu-platform/docs/paper-logic.md)：缓存保护何时足够、翻译中/翻译后等待、最小措施、双方代价与投稿证据门槛 |
| 面向尽快投稿的独立审查 | [审查记录](iommu-platform/docs/research-review-2026-09-09.md)：贡献收敛、W/闭环供给混杂、预热/统计单位、最小因果链与停止条件；建议尚未替代原计划 |
| E1 候选负载 | [双路设计 v1](iommu-platform/docs/experiment-design-v1.md)，正式参数尚未收敛 |
| 请求边界、FIFO/PTW 观测候选 | [观测分工](iommu-platform/docs/experiment-instrumentation.md)，额外 RTL 观测未接入 |
| P0～P5 完整实施路线 | [FPGA 实施计划](iommu-platform/docs/experiment-implementation-plan.md) |
| 论文方向、旧研究背景 | [研究总览](研究总览_2026-09-08.md)，注意其中迁移前路径与状态 |
| 个人补丁来历 | [补丁说明](iommu-platform/docs/patches.md)，provenance 补丁已应用，勿重复 apply |
| ABI、DMA 测量调度 | `linux/include/uapi/linux/idma.h`、`linux/drivers/misc/idma.c` |
| per-DID HPM 所有权/配置/恢复 | `linux/drivers/iommu/riscv/iommu-hpm.c`、`linux/include/linux/riscv_iommu_hpm.h`、同目录 `iommu-sysfs.c` |
| 当前唯一实验前端 | `bench/idma_experiment.c` |
| 构建、rootfs、板上 smoke | `scripts/platform.py`、`scripts/rootfs.py`、`tests/board-smoke.sh` |
| HPM RTL 与既有仿真 | `cva6/corev_apu/riscv-iommu/rtl/software_interface/rv_iommu_hpm.sv`；`tests/hpm_test_top.sv`、`tests/hpm_test.cpp` |
| 四 DMA/DID/PID/地址窗口 | `cva6/corev_apu/fpga/src/ariane_peripherals_xilinx.sv` |
| IOMMU 容量、DS、FPGA 顶层 | `cva6/corev_apu/tb/ariane_soc_pkg.sv`、`cva6/corev_apu/fpga/src/ariane_xilinx.sv` |
| iDMA 与 active cycles | `cva6/corev_apu/iDMA/`，由 CVA6 跟踪，没有独立 Git |

## 已确认的实验范围

后续讨论已修正目标：**争取方法论文，因果表征作为方法设计的前半段，不提前把终点限定为表征论文。** 本次重审 E0～E4/P0～P5 与 v1，文档补入 P4M 方法原型/直接基线/双方代价/冻结规则后验证，并明确 E2 阶段执行依赖、P2 分层校准；具体方法尚未选定。用户随后要求限制软件增量；预热/映射记录、A 间隔、UT_REQ 分母、运行记录器及新执行模式均为按需候选，不是自动待办。

[复审](iommu-platform/docs/experiment-v1-review.md)发现两项可复现问题：① 旧模式仅将 DID 混入高位，小 chunk（1/2/4 字节）短试次漏检错设备源；已改为每字节带唯一 DMA 实例编号。新增 96 个正常/错源组合在旧代码检出 36 项失败，修复后全部通过，原 12 个调度场景也通过；驱动目标交叉编译通过。smoke 每路增加 8 页中前 4 页各拷贝一次、校验其余未触及页，无新接口。② HPM 对全局 valid 取沿，连续 UT_REQ→DDTW 可丢后者；生产 HPM 单元复现得 0/预期 1，有低 valid 间隔的对照正常。RTL 尚未修，当前六槽未选择 DDTW，不能据此说六槽全部失效。S1/S2 与 IOTLB miss 同处产生，不是独立 PTW 访存量或占用时间。原缺陷证据 [out/review-v1-20260909](iommu-platform/out/review-v1-20260909/README.md)仍对应修复前版本；修复验证见[验证记录](iommu-platform/docs/validation.md)。没有新板上数据；交付镜像已于 2026-09-15 更新。

用户在独立审查任务中确认优先“尽快形成一篇扎实投稿”。[本轮审查](iommu-platform/docs/research-review-2026-09-09.md)建议首篇只打通一条能改变设计选择的因果链，不先实现所有机制/措施；保留 E0～E4 既定判据和 FPGA 主线。新增明确点：W 同时改变物理数据工作集与闭环到达率，事后匹配 A 吞吐可评措施代价但不自动识别纯翻译效应；需落实 A 预热相位和独立试次统计。补查 TPDS 2017 的软件管理 IOTLB 非阻塞先例，进一步限制泛称 Hit-under-Miss 创新；其机制并非当前硬件 PTW 的等价实现。上述均是审查建议，无新颖性确认、源码修改或新实验数据，用户尚未选定具体新主张。

用户将本任务作为投稿导向的并行思考支线，推演问题、緩和措置和シナリオ，并要求实质创新。最新[论文逻辑](iommu-platform/docs/paper-logic.md)收敛为“缓存保护何时足够，何时还需干预共享服务”，沿用 E0～E4，优先区分替换、翻译中等待与翻译后出口等待，再选择最小必要措施。性能反转和等待预算仅保留为可选分支，不是必须出现的结果或已选实现。当前**没有已确认的创新点**：RTCSA 2025 已测 DMA/IOMMU 干扰，MASK/DWS 已有翻译感知反馈与组合控制，本次补查 STFM/ATLAS 也排除了“改为时间计费即创新”。下一步收敛少量因果实验卡及措施适用条件，在相近 A 进度下评价 V；仅更新研究文档和交接，未改代码、构建或产生实验数据。

已补充微基准定位：机制与限定范围的预测可主要用微基准；同点重复、冻结规则后测试未见参数、真实应用外推是三层证据。真实 workload 不是当前研究前置条件；声明应用收益时再补对应证据。额度选择须先预测后验证，穷举最佳点仅作离线参照。默认 V 两个映射页/纯循环可能只呈容量阶跃，q 分区与额外时序控制尚未实现。

已按用户要求将面向教授的简短讲稿改为[日语 Wiki 条目](iommu-platform/docs/experiment-v1-report-ja.md)，涵盖实验计划、v1 设计与实现状态。Notion 连续返回授权重试，未能读取或写入用户所指当前页面；页面同步仍待完成。本次仅更新文档风格，未改源码、重建或产生板上数据。

用户希望组会能讲清 access pattern 和实验逻辑。已增加上表讲解稿并定向核对当前驱动：V 固定页对、A 顺序循环；每路一个软件未确认完成描述符；预热按 V 次数，切换时仅 V 回到偏移 0，A 相位继续。该次补充仅改说明，没有新构建/板上数据；实际地址与完整翻译上下文目前不在 JSON 中，不能宣称跨试次物理轨迹已固定。

完整 E0 校准、E1 性能、E2 翻译状态、E3 阻塞、E4 路径/软件对照已提前规划；不是等 v1 结果出来再临时设计机制实验。当前仅完成 E0 准备/E1 的最小软件入口，不代表整套实验或 P2 板上校准已完成。

新版只支持 4 KiB 基础页、顺序循环、Solo/双路；V 完成固定工作量就结束自身测量窗口，A 在预热和测量期间由同一 CPU 持续轮询补交，之后停止并排空。默认 V 源/目标各 1 页，A 源/目标各 8 页，chunk 4096，V transfers 4096、warmup 256。这些是试跑默认值，不是正式采样矩阵。

- 主时间为首个 V 被测提交前到最后完成被软件观察到，含 CPU/MMIO/调度开销，不含 A 尾部排空。
- `dma_active_cycles` 是 backend 非 idle 累加，不是 IOMMU 专属等待。
- HPM V/A 各三槽，精确 DID 过滤、独占会话、保存恢复；`hpm_configured` 只表示配置回读成功，板上事件归属尚未校准。快照非原子，边界外 A 事件可能计入。
- 补交间隙是软件观察完成到下一次提交触发读完成，含提交 MMIO；不等于硬件空闲时间。软件 in-flight 不证明请求重叠。
- 失败会停止补交、排空；无法确认安全时保留映射/缓冲并隔离设备至重启，禁止继续实验。最终数据校验不证明每个重复描述符都实际正确搬运。
- 新 ABI 查询/运行是 0x06/0x07；旧 0x01～0x05 返回 ENOTTY。旧 Solo/Pair/Multi/Once 当前均已删除，不能继续写“旧 independent 已实现所以可直接使用”。
- 新程序没有 random、大页、BARE、四路独立流量、硬件请求重叠/阻塞观测或自动 slowdown。E2/E3 所需控制与额外观测未实现；Page Batching、IOTLB 预留、Hit-under-Miss 等仍是候选缓解方法。

旧论文是 Accelerator-side IOMMU 的 Remote Page Walk 性能评估；英文稿在 `/home/xvhaocheng/thesis/2026_thesis_eng2/`。FPGA 模拟远端访问代价，不代表真实多 Chiplet/PCIe P2P 系统已搭好。

## 当前版本与本轮验证

下表为最小软件修复后的本地提交；Linux、顶层 gitlink 与 lock 一致，CVA6/IOMMU 未改。已有日语报告、研究草案及复审文档保留，部分文档尚未提交；交付镜像已按下表源码于 2026-09-15 更新，构建清单如实记录顶层文档 dirty 状态。

| 仓库 | 分支 | 提交 |
|---|---|---|
| 顶层 | `codex/idma-experiment-v1` | `0bb9851` |
| Linux | `codex/idma-experiment-v1` | `031e91dbadc115dafac957067135002b74035a10` |
| CVA6 | `platform` | `7f64251542f9eaf2e80dced55d87c9d6e6a2ab79` |
| IOMMU | `platform` | `da8a8011f2c52fb1e0c5fd8ae9b9757a5f0ffbb5` |
| OpenSBI | `platform` | `e8d21bfdc2dd753314627968e496c3a1188f6dfb` |

本轮起点顶层 `151c9a7ef2f2aea707c8f587b9b1906691a65430`、Linux `2ef3f4b82afd3d2b039d6c67f214e50f27f10198`，均干净。完整软件从顶层 `ffe55a40175937122b3d6be92a6c005099f16dfd` 构建，起止干净；结束提交仅更新文档，不改变被测源码。所有提交仅保存在本地，没有推送。

已通过：

- CLI 解析/ABI/错误传播 65 组。
- 动态抽取真实 DMA 调度函数的 12 场景，含独立结束、循环补交、预热隔离、超时/中断排空和 quarantine；三类临时注入回归被检出。
- 真实 HPM 控制逻辑 25 组，含六槽配置故障、状态恢复、互斥和进位；检出并修复两个索引宏参数未括住导致 Aggressor PTW 读错寄存器的问题。
- rootfs 白名单/去重/设备节点 4 项；内核目标交叉编译、相关样式和 diff 检查。
- `make check`：21 个组件干净，所有指针/lock 一致，无 alternates。
- `make software JOBS=12`：新用户程序、rootfs、Linux Image、DTB、lloader、OpenSBI payload 全流程通过。DTC 原有 interrupt provider 缺少 `#address-cells` 警告仍在。
- 交付目录全部 SHA256 校验通过；原 BIT/MCS 哈希未变。

证据：`out/manifests/software-migration-4dma.json`、`out-experiment-software.log`、`out/experiment-v1/{delivery.json,SHA256SUMS}`。主机 harness 不验证真实 DMA 映射/MMIO 时序、RTL 事件准确性或板上压力。

## 产物和历史验证

`out/experiment-v1/` 集中保存新版 `fw_payload.bin`、Image、platform.dtb、rootfs.cpio、lloader、实验程序、smoke、lock、manifest、日志和 SHA256。它是软件产物集合，**不是已确认布局的 SD 整盘镜像**；不能假设所有文件都要拷入卡。

覆盖软件前已备份到 `out/pre-experiment-v1-20260908T124529Z/`（含 archive.json）；旧 out/bench 可执行文件移入其中 legacy-benchmarks。旧 `out/migration-4dma/delivery.json` 是历史软硬件组合，其软件哈希不对应新版；当前组合看新 delivery.json。未删除 FPGA 报告/产物或旧源码历史。

硬件为 Genesys 2/XC7K325T、RV64 CVA6 50 MHz、timer 25 MHz；4 DMA，DID 10～13、PID 20～23，IOTLB 16，DDTC/PDTC 各 8，HPM 31。

- `migration-4dma`：DS 50/50，BIT/MCS 已构建，沿用作明确标注的校准候选。
- `baseline-4dma`：DS 0/0，配置已准备，未构建/上板验证。正式样本不能混用两个配置。
- 构建器只支持四路拓扑，改 DMA 数量不能只改 JSON。
- 历史 HPM RTL 测试覆盖 pulse、held-valid、idle、DID filter、inhibit；相同测试检出旧 RTL 10/预期 1 的错误。这不是完整 IOMMU 系统仿真。
- FPGA WNS 0.177 ns、WHS 0.038 ns、WPWS 0.062 ns，总负余量 0；仅表示现有约束满足。仍有 6 项 methodology Critical Warning（TIMING-4/6/7/27）及外部 IO delay 缺失，详见验证记录。
- FPGA 实际导出源码 2,332 个跟踪 blob 与锁定提交一致，见 `out/migration-4dma/staged-source-audit.json`。旧 manifest 的顶层结束状态已单独说明，不据此否认实际源码审计。
- BIT/MCS 和 routed timing/methodology 报告在 `out/migration-4dma/fpga-src/cva6/corev_apu/fpga/work-fpga/`。MCS 步骤会重建 Vivado 项目，别去已消失的 `ariane.runs/impl_1` 找报告。

## 构建、备份与工作视图

常用：`make check`、`make software JOBS=12`、`make hpm-test`、`make fpga PROFILE=migration-4dma`、`make bundle`。不要为熟悉仓库重跑完整构建；`make check` 会写 out/manifests，并非严格只读。

本机工具：Vivado 2023.1（`/tools/Xilinx/Vivado/2023.1/bin/vivado`）、riscv64-linux-gnu GCC 11.4、riscv64-unknown-elf GCC 10.2、Verilator 4.038、Python 3.10。RISCV 默认 `/home/xvhaocheng/RISCV/riscv_tools`。许可证完整地址 `2100@opus4.apal.cs.waseda.ac.jp` 曾直连验证；短主机名 opus4 曾解析失败。没有新增 SSH 转发或 systemd 服务。

组件修改须先提交，再依次更新父 gitlink 和 `sources.lock.json` 并提交；IOMMU 的链路是 IOMMU → CVA6 → 顶层。构建检查拒绝脏组件或不匹配指针。FPGA 导出目录已存在时拒绝覆盖，需按构建说明保留旧结果。

来源：CVA6 `zero-day-labs/cva6 feat/iommu-refact` 锚点 `1f992d80`；IOMMU `zero-day-labs/riscv-iommu mc/dti-ats` 锚点 `c80bba7`，不是当前平台版本。保留接口修复、AW 优先大传输修复、HPM edge 修复、四 DMA 地址译码及 OpenSBI timer 修复；不要切回锚点覆盖个人更改。

没有个人远端，直接 clone --recursive 不保证取到本地提交。`out/source-bundles/` 和 `platform-archives/restore-check` 已验证的是迁移阶段版本，**尚不含本轮实验修改**；要迁移新版先导出新 bundle。本轮没有重复完整离线恢复或覆盖旧 bundles。

历史备份 `platform-archives/legacy-20260908` 保留 22 个旧仓库历史/diff/未跟踪文件/产物。`platform-archives/migration-tools` 是一次性脚本。均未删除。

工作视图由外层 `.vscode/settings.json` 和 `iommu-platform.code-workspace` 控制，关闭自动展开并限制扫描，隐藏 OpenSBI/小依赖/恢复副本。尚未收到用户 Reload Window 后界面确认。组件裁剪和“缩减成 5 个 Git”只讨论过，用户澄清为界面需求后没有执行，也不是待办。apb_timer 在实际硬件中、tb 内有 SoC 包、vendor/modified 有在用 RTL；不能整目录删除。RV64 未实例化的两个宽度转换 IP 属于 Debug Module，不是 iDMA。

## 后续接手

软件交付已于 2026-09-15 更新，包含小 chunk 修复及新增 smoke。设备准备后使用 `out/experiment-v1/`，准备启动和日志通道，跑 platform-smoke（四路无预热/有预热 Solo、多页部分覆盖，加 DMA0/1 双路角色互换），随后做 E0：设备/HPM 归属、窗口与预热敏感性、真实压力校准。warmup=0 不自动等于 cold，命令成功不等于机制证据。独立时钟约束审查仍未完成。

E2/E3 已有设计但无状态/请求控制实现。IOTLB 按上下文/页号等匹配，不能由 DID 不同断言条目独立；AW/AR FIFO 等待不能仅凭入口 stall，PTW active 未接父层；DS delayer 汇合多类数据结构请求，不是 PTW 专属干预。上游 vsim/类式 VIP 可按需参考，当前只验证 HPM Verilator 流程。

需要历史决策原文时定向查源任务 **整理论文工作文件夹**：任务 ID `01a0806d-e7d2-7092-964c-0edd0919d17e`，host ID `remote-ssh-discovered:dell`。不要默认重读完整聊天。本轮未创建新对话。

## 2026-09-16 SC 修复构建完成（最新结果）

新 BIT/MCS 构建已通过，退出 0；完成 UTC：2026-09-15T15:56:47.509135+00:00。

BIT：`/home/xvhaocheng/work/iommu-platform/out/migration-4dma-scfix/fpga-src/cva6/corev_apu/fpga/work-fpga/ariane_xilinx.bit`；11443722 bytes；SHA256 `df7c2400555bba8a95c4ab2b31526fa7958e669dd4c3bfd216140d01de22d5e3`。

正式 routed timing：WNS 0.177 ns、WHS 0.010 ns、WPWS 0.062 ns，TNS/THS/TPWS 均为 0。已设置的时序约束满足，但 6 项时钟 Critical Warning 及 IO/CDC 审查待办保留。

源码与 54 场景回归已通过，旧 BIT 未覆盖。新 BIT 尚未上板；Linux 登录和 DMA smoke/HPM 尚未验收，下一步用新 BIT 重新配置板卡，SD 不改。

## 2026-09-20 P4M 启动控制软件：软件构建通过，板上未跑

本轮实现 P4M 启动控制软件（驱动 batch start 控制、测试、runner、finalizer）与配套文档；不改 RTL、不重建 FPGA、不上板。

- 版本：内核 `6.5.0-rc2-00089-gefd9461d6459`，Linux `efd9461d6459049f59c9ea5466f3703e45413bb3`，顶层平台提交 `a08426f46720b646ae0fa052a0a27136a347b69f`，CVA6 `e822bc9d`、IOMMU `137f5e5`。新内核是独立软件基线，禁止与 `00088` 混算正式对照。
- 构建：`make software PROFILE=migration-4dma-p4m JOBS=12` 退出 0（2026-09-20 12:12–12:13 UTC）；沿用既有 BIT `7af1ccbcb36306d5439eeab9b39d8fdfa266f70ebfedd7c0a4f0dd1be6825d0e`（无新 FPGA 编译），新 payload SHA256 `fc8e8a73d3e71e127d7d995518f668382391ef0920418d16ccb66d716543291d`；交付位置 `out/p4m-start-control-20260920/delivery`，产物哈希、rootfs 内的新脚本及复用 BIT 来源均已核验。
- 主机检查 6 套件全 0：batch-start 10 场景、scheduler（含失败保留与排空）、controlled、CLI、rootfs、board-p4m-start runner 41 例（含主机编译 CLI 与 BusyBox ash/awk）。以上均为主机检查，非板级证据；本轮无板上数据。
- 板卡流程（未执行）：命令 `platform-p4m-start all /tmp/p4m-start-01`，输出目录须不存在；先 4 短试次全过，再 8 次比较试次（4 对，顺序 `B P P B B P P B`）；UART 与 stdout/stderr 由 Mac 侧捕获，Dell 未接板。协议见 [P4M 启动控制协议](iommu-platform/P4M_START_CONTROL.md)，验证见 [validation.md](iommu-platform/docs/validation.md)，事实与日志见 [out](iommu-platform/out/p4m-start-control-20260920/)。
- 动机：pilot-21 第三次 gap0 开窗 A ID 1448（前两次 4/4），启动相位/预窗口历史混淆；旧 n=3 与本次 4 对均只作描述，正式参数未冻结。见 [p4m-pilot-results-review.md](iommu-platform/docs/p4m-pilot-results-review.md)。

## 2026-09-30 组会汇报资料

- 用户明天组会计划 8～10 分钟，上次汇报止于 A/V 共跑变慢；本次材料衔接两种假设、E3 受控机制证据、P4M 固定等待取舍与当前阶段。
- 交付：[PPT/PDF、讲稿与问答、证据索引](iommu-platform/out/group-meeting-20261001/README.md)，8 页、计划 540 秒；PPT 备注含讲稿与来源。
- 原始 E3 配对数组复算 ΔA/ΔV 为 127/129/129/130，V 自身翻译均 4 周期；P4M 前 24 的三组同窗口速率 min/median/max 复算与既有统计一致；记录在 numeric-verification.json。8 页 PPT 容器、备注、形状边界、8 页 PDF 与实际版面已检查，记录在 materials-validation.json。
- 未新增实验、未改实验源码、未构建内核/FPGA；不把材料准备记为剩余 35 次已完成。E2 未定，E3 结论范围受限，当前仍 P4M-5 正式取舍采样阶段。
- DeepSeek 起草资料和生成程序；Codex 做关键科学审阅、原始数字复算、实际生成及版面校正。

## 2026-09-30 两次组会汇报边界与日语讲解

- 上次组会：用户已确认实际汇报到「A 和 V 共跑变慢」这一事实；具体汇报日期未知。不要把本次刚刚整理的机制证据追溯为上次已讲；上次是否讲了两假设、E3、P4M 等，不能从本次记录反推。
- 本次组会准备：按 8 分钟纯文字/表格、日语口述准备。拟从两个假设讲到 E3 有限机制证据，再概述 P4M 缓解的吞吐取舍与下一步。此边界仅是准备范围，尚无实际汇报完成确认。
- 本次刚刚讲清：A/V 剩余计时与因果链；第一组 A 272→399、V 281→408、各 +127，V 自身 4→4；其余配对 +129/+129/+130，V 自身仍 4；两个剩余量均从捕获所选 A 页表响应的干预起点计时：A 到翻译响应返回，V 到被控制器选中；V 自身另从翻译请求接受到翻译响应返回。V 是后来首次查翻译缓存并实际命中，不能描述为排队期间已经完成缓存查询；控制器 IDLE 才选新请求、AWAIT_RESP 保持当前请求、COMPLETE 等译后地址接收才回 IDLE，因此等 A 时不选 V 不是持续计算而是占处理状态；干预 + 阶段计时 + 代码共同支持当前实现/配置/到达方式下 A miss 让随后 hit 的 V 等待。样本局限：N=4、单次启动、64B、三对触发前时序不同、D0/D0 波动 -2/0；不能归因全部旧 A8 减速或所有 IOMMU 如此。
- E2 缓存替换：未证实，也未排除。
- P4M formal60 状态：目前 24 在线通过 + 第 25 离线认可 = 25 可用；35 未运行。按 gap0 / gap1000 / gap2000 的顺序，前 24 均衡样本为 8/8/8，V 中位 108.8275 / 150.6503 / 171.4546 MiB/s，A 同窗口 108.8275 / 75.3252 / 57.0901 MiB/s。此状态不视为 formal60 完成，也不作为最优参数结论。
- 实验结果本轮无更新。
- 后续组会衔接：应从本次最终实际讲到处衔接，需用户日后反馈确认，不能先标完成。实验下一步延续既有 resume35 计划；上板前校验原环境衔接，不重新跑已有样本。
- 本次只做翻译与记录：无代码修改、编译、板测。用户只要聊天中文/日语与表格，不新制 PPT/PDF。
- 链接：参见 [docs/e3-controlled-results-review.md](iommu-platform/docs/e3-controlled-results-review.md) 与 [docs/p4m-formal60-recovery.md](iommu-platform/docs/p4m-formal60-recovery.md)。
- 注意：不要假设用户把原计划中的所有内容都实际讲了。
- 分工：DeepSeek 起草日语讲稿与交接；Codex 核对关键结论、修订时序措辞并实际更新文档。日语草稿中擅自提出的泛化追试未采纳，下一步仍以既有 P4M 计划为准。


## 2026-09-30 改善措施与创新点：Codex / DeepSeek 两轮讨论

- 用户要求讨论最重要的改善与论文创新。结论：尚无已确认创新；固定 gap 是基线，首选评估有限资源下 hit 与 pending miss 的服务解耦，等待反馈准入为备选。候选未实施、未编译、未板测。
- 定向核查 HPCA2017 已有 hit-under-miss，RTCSA2025 已有 IOMMU 干扰表征，MASK/DWS 已有翻译资源管理；不能以平台迁移、泛化限速或普通非阻塞直接声明新颖性。
- 下一步：既定 formal60 续 35；并行准备与合理通用有限非阻塞对照的最小设计卡，先说明不同规则和可测成本/效果；差异明确后才原型。评价双方取舍、资源和频率、排序/失效/异常/排空/公平进度。没有用户 V/A 目标则画曲线，不编门槛。
- 不采用未经论证的 DID 不同即可越序、epoch 自动正确、只计状态位同成本等假设；不因四对固定128干预结果相近而推翻已有有限E3机制。当前只支持D0/D128，独立出口反压与任意延迟幅度不是现成功能，不重新展开所有E2/E3/E4作前置。
- 已确认 E3 与 formal60 状态不变：24 在线通过＋第25离线认可、35未运行。本次汇报仍仅准备，未收到实际汇报完成确认。
- 详细结论与原始论文链接：[论文主线最新讨论](iommu-platform/docs/paper-logic.md#2026-09-30-方法路线讨论改善候选与创新门槛)。完整 DeepSeek 两轮 JSON 及 Codex 反馈保存在 `iommu-platform/out/method-design-discussion-20260930/`。DeepSeek 提方案与反驳；Codex 核查文献/代码、决策及文档更新。

## 2026-09-30 创新继续讨论：普通非阻塞／必要容量保护

- 又完成两轮 DeepSeek 讨论及原始论文定向核查。下一步先形成合理有限非阻塞对照规格，再比较同容量静态保护；动态借用暂不立项。当前未实现普通非阻塞，也未测得其槽耗尽，不写成新发现。
- 候选进一步收窄为按请求生命周期与阻塞传播关系选择必要保护位置/容量；尚无具体新规则及创新证明。普通非阻塞若已解决目标等待，停止额外保护立项，不人为造弱基线。静态离线设计不要求在线识别。
- 补充先行工作 Valsan/Yun/Farshchi 非阻塞缓存隔离论文已有MSHR耗尽挡hit及并行度控制，因此配额/预留不新；其CPU数据缓存与本IOMMU区别需保留。成本按各类槽同总容量且报告全部逻辑/频率，不能N对N+k或只算状态位。
- 当前没有新源码/编译/仿真/板测；formal60冻结续35仍待执行，E3有限结论不变。详细讨论见 `iommu-platform/docs/paper-logic.md` 最新节；完整第三/四轮JSON和Codex反馈在 `iommu-platform/out/method-design-discussion-20260930/`。

## 2026-09-30 N/P 可否决设计卡（第五/六轮讨论）

- 已形成设计筛选卡：N是合理有限非阻塞，P是同底座同各类容量的朴素静态预留，第一版只改资源分配，不给P另加lookup优先级。P不是新算法；未来必要保护位置/容量选择规则另称候选R，仍未形成或验证创新。
- 结果存储采用何时分配尚待共同评估，不能强令全部长miss早占结果槽而制造弱N。当前译后AW/AR头直接取FIFO头、地址取单响应；并发必须跟踪完整头与对应结果。
- 三场景：A长查表+V合法命中；合理N可到达的真实容量压力/区分全局与A支路反压；V活跃/空闲下A代价。V空闲用独立固定公共窗口或有限A任务计时，不能使用不存在的V窗口；活跃时保持现P4M窗口契约。
- 请求到达与ready握手分开计时，连续valid时按实际握手区分请求，计接收前等待，不把准入等待藏掉。正确性逐分支契约及定向RTL环境仍待完成，当前不是可直接编码的最终规格。
- formal60仍24在线+25th离线=25可用，35未跑。无代码改造/编译/新测试/板测。卡已追加`iommu-platform/docs/paper-logic.md`；完整worker输出、审阅版JSON在`iommu-platform/out/method-design-discussion-20260930/`。DeepSeek起草/修订，Codex检查关键代码和契约、保存记录。

## 2026-10-02 E2 并行建议与续跑状态更新

- 用户明确报告剩余 35 次正在跑：记为 reported_running，新增完成数未知，尚无本轮新增原始数据审计；不再沿用“35 未启动”，也不宣称 60 次完成。
- Codex 建议并行开展最小 E2 机制验证，与 E3／改善设计互补而不互设前置条件。E2 检查 A 停止排空后 V 的翻译状态是否受影响；E3 现有证据检查 A 服务时 V 命中请求仍等待，两者可并存。
- 最小方案沿用 experiment-plan.md:57 的顺序与三类对照：V 预热确认命中→A 有限访问→DMA／翻译／查表请求完全排空→V 目标首次探测；等时无 A、小重复集合、更大集合，集合大小按独立翻译条目计。准备不得顺带失效 V，多地址探测必要时逐地址重建初态。参数和观测能力需在实施时定向确认，不假定现成完整 runner。
- 首次 miss／PTW 增加仅支持翻译状态变化；称为 IOTLB 替换需具体条目、替换／失效及上下文证据。阳性需评估对目标工作量的重要性和干预收益再决定是否增加容量保护；阴性只限已测条件。现有 N/P 仍是未实现的服务阻塞改善讨论，不因 E2 未完成而停下。
- 本轮回答并行是否值得，未执行 E2 代码改造、构建、仿真或板测。DeepSeek 提供判据评议，Codex 直接核对 E2 原计划并作范围决定、写入交接。完整输出：`iommu-platform/out/e2-parallel-decision-20261002/deepseek-assessment.json`（deepseek-flash，未截断）。

## 2026-10-02 E2 减少重复上板：单次部署方案

- 用户指出上板费时，要求一次准备更完善。将上一轮最小验证扩为同一 BIT＋软件包支持完整有限矩阵，运行时调整条件；一次启动自动短验收、冻结配置、主矩阵和事前选定的小复核，真错停并保留检查点、不自动重跑。没有在本轮开始实现或构建。
- 主矩阵候选 A 源／目的各 W=0/1/4/7/8/16 页，V 两个独立映射，合计 2/4/10/16/18/34；不由 IOTLB16 预言精确替换阈值。每条件 10 次与各有 A 组相同的 112 次有限拷贝为工程候选，需验证可达性／观测容量并冻结；少量顺序或布局复核事前固定，避免结果驱动扩样。
- Codex 直接核对 IOTLB 更新／PLRU 和现有 observer：更新按 replace_en 落位，既有 PTW 快照不是完整条目状态。方案须补目标条目跨槽生命周期、实际覆盖／失效提交及请求归属，以及 V 探测前源／目的同时快照和逐 lookup 顺序。两个探测可能互相影响；仅 miss/PTW 不作容量证据。观测应被动且验证新增资源／时序影响。
- 不每次重启整板；统一真实请求准备并记录实际 tag/valid/PLRU／上下文，不虚称 flush 重置所有内部状态。无 A 等时对照优先采用短验收按预设规则确定的共同阶段窗口 T，主矩阵前冻结；实现不到则必须显式匹配，不能只称等时。初态不达标／超窗／溢出属于执行失败，正常首次 miss 或无替换须保留。
- 范围是 E2 观测 RTL＋驱动/CLI＋runner/汇总；不捆绑未定型非阻塞／预留改善电路或完整 E4。必要主机检查／构建在交付前，预计运行时间待实测，当前均未执行。DeepSeek 两轮起草修订，Codex 作科学和实现边界决定、直接核查关键源码并保存审阅稿；详见 `iommu-platform/docs/experiment-plan.md` 末节及 `iommu-platform/out/e2-single-deployment-20261002/`。35 次 P4M 仍沿用用户报告 running，本轮没有新增结果审计。


## 2026-10-02 同包诊断实现进展（构建中，未交付）

用户授权先冻结下一步实验，再实现。已冻结短验收 6＋E2 60＋自然占用 18＝84 次，见 `iommu-platform/docs/experiment-plan.md` 末节；不是完整 M 改善电路。DeepSeek 起草观测器/CLI/runner/分析/文档，Codex 主导低层驱动、关键时序与科学判据审查、应用及执行。

组件：Linux `a8d28cd44f35`（新研究 ioctl、全局串行化、捕获后释放映射），IOMMU `238a39e`（被动实际 q/n/replace/owner tap），CVA6 `2ccb6401`（observer v8 与可综合记录存储），顶层 `60b2d44`。完整软件已退出 0，内核 `6.5.0-rc2-00090-ga8d28cd44f35`；payload `out/opensbi/platform/fpga/ariane/firmware/fw_payload.bin`，SHA256 `9eb5bc200cc69b8b0cf16d82b835775f71e1e5676bec66a8d0cfc3ff7e1bdadc`。rootfs 中 idma_research、platform-research、固定 TSV 已逐字节核对。

主机观测器 162＋2 项仿真、实际 IOTLB/旧版功能对照、legacy P4M 与 TC/batch-start 回归、runner 和 rootfs 检查通过；CLI 56 项通过。发现并纠正 CLI 误用翻译占用作吞吐分母、测试误拒默认 A8 gap1000、矛盾 first-hit 替身等。FPGA 初次沙箱 DNS/许可证失败；沙箱外许可证可达。后续综合拒绝多字写 RAM，Codex 将事件/样本 RAM 改为每拍一条宽记录，快照用寄存器；164 项仿真重过。失败源码构建保留在 `out/migration-4dma-research-v1/fpga-src-failed-ram`，新冻结源码完整构建正在执行。

离线分析已起草但未验收：39 项首次测试 7 失败；另发现 cost 状态全零误判、替换后恢复的错误归因、全局采集错误未影响退出码等，已送 DeepSeek 修订。全部原始响应和日志在 `out/next-experiment-20261002/`。不得将脚本存在、两模型意见一致或仿真通过写成 E2 阳性或改善有效。Dell 无 FPGA；当前没有新板测。后续以本节之后的最终交付条目为准。


## 2026-10-02 最终交付：84 次 E2／自然占用诊断实现与完整编译完成

本条取代上文“构建中、未交付”状态。当前仅完成实现及主机／构建验收，本包实际板测 0/84；P4M60 和旧 E3 有限机制结论不变，E2 和 M 创新仍未证明。

- 计划：短 6＋E2 60＋自然占用 18；同一次部署，顺序执行，遇错停止、不重跑／跳过／自动续跑。参数和解释在 `iommu-platform/docs/experiment-plan.md` 末节及 `docs/experiment-instrumentation.md` 最新节。
- 代码版本：顶层 `6e4ec6577e3e45fc1820b2432805b6bd01428e97`，CVA6 `8c1e08b3`，IOMMU `238a39e`，Linux `a8d28cd44f35`；内核 `6.5.0-rc2-00090-ga8d28cd44f35`。源码已分别提交并锁定，已有无关脏文档保留。
- FPGA 最终 `fpga-fit-build.exit=0`，软件 `software-build.exit=0`。BIT `7e7024fc00a722b2ed9a78ab63223af70a54bcca423407643385e6c729e0bc62`（11443722 字节）；MCS `dd55c3557d95ee7f66590a9ba64ee529a778ac1fcc0cbdebd192c5a5a966f854`（31472748 字节）；payload `9eb5bc200cc69b8b0cf16d82b835775f71e1e5676bec66a8d0cfc3ff7e1bdadc`（29363656 字节）。软件构建早于最终纯 FPGA 配置调整，Linux/OpenSBI/设备树/软件均未变化；两份原始构建 manifest 及 source lock 分别保留，未伪造同一构建来源。
- 已解决构建问题：沙箱许可证网络、记录 RAM 多字写入不适合综合、布局容量不足。事件/样本改为一条宽记录单端口 LUT RAM；新 observer 约 9117 LUT，旧 E3 详细追踪约 24564 LUT，因此仅本次 profile `legacy_e3_observer=false`，默认其他 profile 保留；P4M/path、新研究记录及 DMA/IOMMU 功能保留。旧 E3 模式不用于本包；旧 BIT 保留。失败 stage 和日志未删。最终正常编译日志只保存，未扩展为全面时钟约束审查。
- 验收：新 observer 164 检查，实际 IOTLB 与旧版功能 703 对照、0 失败；P4M 常规/小计数器/research 配置 29＋2＋29 场景；CLI 56、分析 46、runner 33、rootfs 13 项通过，既有 TC 与 batch-start 回归通过。离线分析修正了吞吐分母、错误状态判断、替换后恢复的错误归因、相同重写槽号、异常样本混入统计等，保留失败记录、分离短验收与主试次及每次运行，控制器占用和搬运总时间分别报告。主机替身测试不是板测。
- 交付：`iommu-platform/out/next-experiment-20261002/delivery/`；压缩包 `research-v1-20261002.tar.gz`；完整步骤 `delivery/RUN_FIRST.txt`，产物版本 `DELIVERY.json`，文件校验 `SHA256SUMS`。包含配套 BIT/MCS/payload、runner、固定 TSV、便携离线分析与原始构建来源。
- 下一步：Mac 按原流程载入配套 BIT＋payload，确认 Linux00090，再执行 `platform-research all /tmp/research-v1-01`。all 已含短验收，不要先额外重跑；回传完整目录，不能只回传 UART 摘要。Dell 用 `python3 analysis/research_results.py all INPUT --output NEWDIR` 分析（从交付目录执行）；中途停止也保留全部数据并按 all 列出缺项。
- 分工：DeepSeek 起草观测、采集/分析/runner/文档及专用配置；Codex 作实验与架构决定、低层驱动、关键资源/判据修正、审查和全部实际工具执行。没有声称 DeepSeek 自己运行命令或测试。


## 2026-10-02 日语发表资料：约8分钟 Markdown 完成

- 用户本次只要求写日语内容到 Markdown，后续自行制作 PPT。已交付 [research-progress-presentation-ja-20261002.md](iommu-platform/docs/research-progress-presentation-ja-20261002.md)，10页时间合计8分钟；每页包含可贴PPT的短句／表格、独立讲稿、核心结论。附术语、剩余时间数值例、60份分段与异常说明、84次计划及10项答疑。
- 拟讲述边界：研究目的与共跑事实→E2/E3两假设→64B的E3有限干预证据→连续4KiB的固定等待完整60份取舍→尚未确认创新的候选问题→新84次观测包已实现并编译、未上板。不能沿用旧25可用／35待跑，也不能说60份均V零miss或同一启动60次在线通过。
- 本轮直接核对既有E1/E3报告、完整60份RESULTS及最新交付记录。E3表保持127/129/129/130与自身4周期；P4M用108.813/150.603/171.389及同窗A108.813/75.301/57.088，整轮miss/PTW不与速度窗口混算。
- 没有新实验、构建、PPT/PDF或统计复算。只读来源与既有源码／结果保持原状。仅记录材料准备完成，用户实际发表到哪一页尚未确认；上一次用户确认的实际汇报仍止于A/V共跑变慢。
- 分工：首先委托DeepSeek阅读指定报告并起草日语稿；调用最终报“DeepSeek 返回格式异常。”，没有有效稿件，usage等元数据未提供，不能判断故障原因或编造消耗。原返回保存在 `iommu-platform/out/presentation-ja-20261002/deepseek-response.json`。Codex因本次委托不可用接手完成原稿与核对，不将文稿记为DeepSeek起草。


## 2026-10-02 Research结果后的两轮讨论：Solo修复可合并下一次软件交付

- 用户请求评议80成功/2失败/2未运行的影响、与DeepSeek讨论下一步、能否合并修复；重申编译不反复看正常日志。完成诊断/数据初审和两轮DeepSeek评议，尚未开始修代码、编译或板测。证据目录 `iommu-platform/out/research-board-review-20261002/`，两轮原文deepseek-round1/round2.json均完整未截断。
- Solo根因：部署版研究适配层把a_pages0传成legacy aggressor_pages0；research_validate和p4m_validate通过，但idma_run_session入口idma_validate拒零。失败在mutex、映射、observer、DMA前；短与主失败均errno22/stage0/raw0/提交完成0。E2 W0独立路径、双路cost正页数不受此EINVAL影响。idma.c与部署a8d28cd44f35相同。
- 本轮开始时已有未提交修复 `linux/drivers/misc/idma_research.h`：Solo旧接口占位页数设1但保留NO_AGGRESSOR和研究a_pages0。本轮没有改此草案，也不声称原作者或已构建部署。后续须真实driver链回归确认短64/8和主4096/256可进入执行、无A分配/提交、非法参数仍拒绝、nonSolo1/8/16不变。仅此修复不需要更换BIT。
- 既有分析器三段逐项3+62+15=80ok，2failed、另2missing；全局exit1/STOPPED/子集非原84前缀记录不变。额外核对临时runner仅改选取范围，选取是原冻结计划精确子集、实际为各自前缀、82已尝试无重复。709份输入哈希未变，未伪造合并目录。见cross-segment-summary.json。
- E2初步主结果：W0/1/4/7各10次源目的两目标保持命中；W8/16各10次两目标A提交覆盖、截止不在、首探测miss。不推广精确普适阈值。21份附加警告来自脚本时点错误：dst先探测并回填，src后探测看到dst；用dst早时点筛更新却比较src晚时点live_other。例b01-w8：5030007→事件18回填5030411→src5030426。各自时点重放/三快照/first-hit硬检查通过，21份全部逐例解释。仍须修分析代码、回归并原数据重算，不重跑E2采集，也不直接删告警。
- cost 14份lane_active=2是bit1 A在close时冻结：close清cost_active使lane记录停止更新；驱动随后停止/排空/quiet再capture，各份A提交=完成、无quarantine。不能叫capture仍活动，也不无条件忽略所有活跃告警。逐例依据在advisory-cross-check.json，关键代码在critical-excerpts.txt。
- cost相同req4/miss2抽样组A8gap0 main中位1406/1408/1408周期，gap2000为835/837/842，仅有继续探索的线索。每16个完整copy固定抽样有相位偏向，A8gap1000每轮127条全miss0却窗口miss3073/3073/3072，不能用抽样估总体分布。gap本身改变竞争与时序，不能据此称M有效或最优。
- 对DeepSeek的裁决：采纳全部Solo对照缺失并非无关紧要、修脚本须原数据重算、新段同版对照；第一轮不能比较任何双路变化的说法过宽，第二轮收窄。再纠正第二轮固定A计数的措辞：固定的是V工作量、A工作集等设置，A在共同窗口完成量会随gap改变。已有同段gap取舍描述可用，缺Solo时不计算相对Solo slowdown。
- 下一步：先修离线两处口径、针对性回归并重算；审查Solo最小修复与真实驱动链，合入同一新软件镜像一次上板。先短Solo验收，再同BIT同软件同段交错Solo/A8gap0/A8gap2000；工程重复可沿用各3次，具体新协议和ID在实施前冻结。若要全部5个双路条件的slowdown，须同段覆盖5种，不用新Solo与旧80配对。原2失败永久保留，新样本不追写原84全过；不因bug重跑80全套，完整M与采样RTL暂不自动实施。
- 编译规则：仅正常日志落盘等退出，实际报错才查看，不反复轮询日志、不重复完整检查。本轮没有启动编译。Linux运行/UART释放沿用回传状态，本轮未接板。


## 2026-10-02 用户询问镜像裁剪与JTAG替代SD启动

- 用户明确慢的是FPGA从SD卡加载，不是Mac写卡，询问能否串口/JTAG传镜像。本轮只做窄范围源码/产物/已有UART分析；未删组件、改配置/SD、编译或接板。DeepSeek起草评议，Codex核对关键启动路径、当前JTAG连接依据和实物日志，原文在 `iommu-platform/out/image-size-review-20261002/deepseek-assessment.json`，未截断。
- 体积：payload29,363,656B=28.003MiB，Image24,130,560B=23.013MiB；rootfs.cpio6,899,712B，内核实际压缩输入initramfs_inc_data3,415,302B=3.257MiB。rootfs已gzip、内核DEBUG_INFO_NONE，不能建议再次gzip/删debug来虚报收益。rootfs无.ko，=m的显卡等不直接构成当前Image大头。
- 关键：原UART分区0的first LBA0x800、last0x107ff，65536扇区=32MiB；bootrom gpt.c的size参数未用，按last-first+1整分区复制。现日志65个加载进度点与整分区读取相符。只缩小payload不改读取范围，读取量仍32MiB。当前payload最低57351扇区（DeepSeek多算成57352，已纠正）；不直接照此改卡，应按产物大小留余量、正确GPT与数据验证。当前最多约4MiB多读，去掉仅减少约12.5%读取字节，非保证全启动时间缩短12.5%。
- 慢速路径还包括SPI软件FIFO轮询、64B分批与CPU逐字节CRC；并非普通电脑SD接口速度，未做实际瓶颈计时。约5MiBImage外开销主要含OpenSBI/lloader的2MiB布局与DTB对齐，不擅自改地址/删除填充。
- 可裁剪候选：当前DTS不用的USB/SATA/PCI/其他SoC驱动、无关NIC驱动/文件系统；旧E3两个程序约1MiB原始体积，可按实验profile另包，但压缩与2MiB对齐后实际payload收益需构建测。DeepSeek误把当前idma_research也列作旧工具，Codex明确否决；idma_experiment服务P4M/验收、platform-timeout服务watchdog，不直接删除。基础库/BusyBox/串口/SD路径/IOMMU/DMA/HPM和潜在网络更新功能先保留。裁剪会形成新内核版本，后续实验须用同版对照。
- JTAG：CVA6 README:259说明Genesys2板载FTDI第二通路接CPU调试；ariane.cfg channel0、1MHz；genesys-2.xdc与顶层dmi_jtag存在。可探索加载payload到DDR启动，区别于Vivado烧BIT；OpenOCD官方load_image支持写目标内存：https://openocd.org/doc/html/General-Commands.html 。未实测host连接/DDR初始化/复位/缓存/跳转/速度，不能保证比SD快，也不能在正在运行Linux内存中直接覆盖后当有效启动。
- UART：当前boot初始化115200，按8N1传28.003MiB理论42.482分钟，不含协议开销；bootrom没有串口收镜像流程。串口适合小脚本/命令，不作为整镜像加速首选。网络/SSH将来可另验，当前不假设Ethernet及Linux写SD已通（配置SPI_XILINX未开需核查），不自动扩展部署工程。
- 建议先同步优化镜像与SD读取范围；JTAG作可减少插拔卡的开发路线，先一次小规模连接/读写/启动计时验证再选。现处建议/证据阶段，后续照用户编译规则只等结果、报错才读日志。

## 2026-10-02 Mac 交接、Solo修复和独立11项对照已交付待上板

用户再次明确 FPGA 在 Mac，本机 Dell 不接板。Mac任务「上板」ID `01a0a548-bdae-7610-9af6-9709ddf26b5c`，host `local`；可列出该任务但当前没有发送工具，未伪称已发送。说明在 `iommu-platform/docs/mac-followup-20261002.md`，请 Mac 从 Dell 读取。当前可确认无实验运行、记录USB/JTAG/OpenOCD状态；JTAG完整加载/启动/速度需维护窗口验证，不操作运行中Linux。软件包按原SD流程部署，同研究BIT；保持32MiB分区读取问题为单独优化项。

Dell接受工作树已有Solo适配修复，将零A页数兼容为旧公共validator要求的1，同时NO_AGGRESSOR仍在，research输入a_pages仍0。直接审核实际run_session：Solo无A实例获取/stream准备/提交；不改旧公共校验，不改DMA/cache/时序。Linux提交 `9ad05d00cca761e8b480a994a34196b4d595c0cf`，顶层实现提交 `2b02d4f`。DeepSeek起草分析、mode、回归和说明；两轮草案中的时点/计数混用、错误测试、UAPI手抄及补丁上下文问题由Codex驳回，定向修订后机械应用并补正上下文，实际运行验证。完整往返保存在 `iommu-platform/out/research-followup-20261002/deepseek-*.json`，未声称模型自己运行测试或编辑文件。

分析器按目标自身probe时点/live_self核对E2；保留两probe交叉视图、原事件重放、分类及兼容输出字段。cost C2C定义为关窗锁存信息，不能当capture时busy；最终排空/错误/隔离硬检查保留。不要求whole-arm A issue==done，因为自然carry-out可在关窗后排空。原三段重新分析到新目录：3/62/15个ok，全局分别failed/failed/incomplete、退出1保持；35条已定位未知项归零，成功/失败逐case状态不变，709份输入SHA未变，82个attempted ID无重复。见 `reanalysis-confirmation.json`。不是84/84完成，旧失败/未运行及原UART/dmesg保留。

`cost-followup`选择原冻结计划的ordinal 4,5,67,69,72,74,77,78,79,82,83；完整84行TSV字节不变。2短Solo/A8g0，主Solo/A8g0/A8g2000各3，保留原block内相对顺序。新软件单独采集段，4096/256/4KiB，短64/8；任何错误停，不自动重跑。用于Solo修复验收和局部同版比较，不是原84续跑，也不完成五条件全量slowdown；原E2不用重跑，M及采样RTL不启动。

主机40项计划/runner、63项分析、1项真实4函数提取+stub+旧行变异均通过；sh syntax及定向diff检查通过。新软件构建退出0，正常编译日志未读取；内核 `6.5.0-rc2-00091-g9ad05d00cca7`，rootfs内runner/plan/CLI与当前来源逐字节一致。FPGA未重建，沿用BIT `7e7024fc00a722b2ed9a78ab63223af70a54bcca423407643385e6c729e0bc62`。payload 29,363,656B，SHA256 `4046cd117052e5988a618cebbfaf63f8d0cb686a1babc5d3dc01f7647fc889c7`。无裁剪，所以不宣称SD启动提速。

交付包：`iommu-platform/out/research-followup-20261002/research-followup-software-20261002.tar.gz`，10,569,665B，SHA256 `2ec2c0b68ac185ba5cfe0c5fabe4490162dbaebf7290da040ab1997b0ff2c942`。包内DELIVERY.json/RUN_FIRST.txt/SHA256SUMS、软件manifest、复用硬件manifest与来源锁均保留。Mac尚未收到/新版板测0/11；下一步Mac核对包、更新SD payload、确认00091后执行 `platform-research cost-followup /tmp/research-followup-01`，回传完整目录与UART。Dell重算后再决定方法M是否值得实现。既有完整时钟约束审查仍为独立待办。

## 2026-10-02 00091 cost-followup正式回传：11/11离线通过及路线裁决

用户“实验结束”，从CURRENT_STATUS定位到 `iommu-platform/out/mac-research-followup-20261002/`，无需重扫仓库。按已有cost-followup模式对 collection/unpacked 运行当前analyzer，退出0/statusok/11成功/0失败缺项；结果写入 `out/research-followup-review-20261002/analysis/`，原目录不改。独立用原11份JSON的raw[0xc19]、p4m.window[3]及victim.completed_bytes重新算硬件时间与V/A同窗速率，并核对errno/status/stage、最终提交完成/数据错误及全部Solo A零活动；与分析器一致。分析器105输入文件哈希不变，外围含UART/manifest/回执的完整115文件SHA也全部不变。见direct-check.json、acceptance.json、RESULTS.md。

新内核00091、boot_id db14364a-c046-4e6a-8ae7-f4dea007724a。初始SPI停顿经同BIT1MHz重新加载恢复，这是试验前启动恢复，不是重跑实验；未定位该停顿根因。UART在short-cost-a8g0阶段保留hrtimer interrupt took 12921440ns；未将taint0说成无warning，不删除/剔除任何样本。记录的Linux保持运行/UART释放来自Mac回传，本轮Dell不接板。

主9次各条件3次（短2另列）：Solo耗时56.44032ms [56.43188,56.49138]、V283.485281MiB/s；g0耗时146.982ms [146.85438,146.99186]、V/A108.856867；g2000耗时93.30072ms [93.27222,93.35676]、V171.488494/A57.114570。相对Solo中位耗时比2.604202/1.653086，不是同步配对；g2000对g0 V速率+57.5358%、耗时−36.5223%、A同窗速率−47.5324%。原00090非Solo不拼接；旧84仍80成功2失败2未运行，新11独立完成。

所有主V硬件窗口hit16384/miss0、软件HPM miss/PTW0/0，仍有共跑变慢；表明该条件V损失无需额外V miss，不足以量化IOMMU和互连/数据竞争各自贡献。g0 A软件HPM5140/5140对硬件窗口miss5119；g2000软件HPM2406/2402/2408（PTW相等）对硬件窗口2388/2388/2390。软件HPM含MMIO边界，不称精确硬件同窗PTW。

固定抽样每run req4/miss2组：g0占用中位1406/1405/1408，n各255；g2000 832/836/832，n各85。每16次固定相位偏向保留，不能把拷贝样本当独立run或总体。另做窄范围离线边界证明：同4096B/4req/2miss输入的固定线性权重只能产生相同估值，不能精确还原两组抽样中位，见count-only-boundary.json；没有测试/否定所有类型权重、简单V反馈，更未证明M控制价值。

DeepSeek第一调用因UART单个NUL输入拒绝；另存UTF8/空字节转义副本后第二次因共享1M上下文拒绝，不能称网络问题。按技术限制缩小到全11统计、3份关键原始和既有paper-logic后成功，预算保持393216/思考high。完整回复deepseek-bounded-review.json（未截断），草案DEEPSEEK_REPORT_DRAFT.md。Codex纠正105分析输入与115完整回传的范围、旧E2证据不得被本次无E2否定、训练内拟合不是路线判据以及“无独立样本”的过度表述，形成RESULTS.md。

路线保持但测量关卡完善：优先审查实际计数覆盖的事件类型；下一候选观测在共同窗口内对完整A搬运做全量分组计数、占用总和与范围，避免只抽每16次，明确窗口边界/溢出/资源预算后冻结协议，尽量同包部署。消除偏相位后仍有可用于计费的差异才推进小范围M原型，与类型加权计数、相同外层控制计费消融及简单V反馈公平比较；证据不足保持未决，不从gap效果推M有效，不从训练拟合推M无效。不追加大规模重复/扫gap/重跑旧E2。此轮只分析和计划，无新代码、构建、仿真或板测。


## 2026-10-02 全量分组统计 v1：实现与构建交付完成（新板测0/21）

用户最新“实现”承接上一轮全量统计计划。Codex冻结完成沿和窗口接口，DeepSeek起草统计器、采集/分析/runner及测试文档；Codex直接审查关键源码，增加低层capture guard、应用并验收提案。分析器首次MCP本地连接错误，经命令入口恢复；硬件独立测试入口及CLI寄存器映射/runner遗漏在交付前修复。不能把模型自述当执行证据。

实现：`dma_research_observer.sv`追加只读聚合器；提交严格同C14 eligible，使用完成沿_n计数，清零优先，关闭沿完成计入，carry-in/out排除。D00 `IDMAAGG1`；六组0..4=req4/miss0..4，组5保留其他组合；每组count、五项sum、main/complete/duration的min/max，溢出/单copy非法关系sticky拒绝。保持原抽样、IOMMU控制、DMA供给/gap不变。没有M缓解策略，没有DDT/PDT/PTW完整类型细分。

软件：原UAPI/raw4096/JSON schema保留；新CLI `--require-fullstats`只限cost；缺新magic必须失败，有块时即使未指定flag也校验。驱动拒绝坏magic/版本/布局/错误/总数并仍回传raw。CLI以128位整数、分析器以Python整数检查守恒及正确的A窗口寄存器C1B/C1D/C1F/C23，防止溢出或误用V值。新增 `cost-fullstats` 从不变84行计划选全部21个cost：3短64/8，六条件各3主4096/256，4KiB。每例实际argv带flag且有记录；首错停止。分析报告分别列全量与同组抽样的均值、范围，不推导总体中位，不将组内搬运当板级重复。

验证证据 `iommu-platform/out/research-fullstats-20261002/`：统计器189＋21，观察器261＋2，P4M双路293、gap17，CLI77，计划/runner49，分析86，Solo/capture实际驱动函数各1套，shell syntax0。无测试跳过。rootfs内CLI/runner/TSV与实际产物匹配，84行计划字节不变。FPGA暂存观察器源码与已提交CVA6文件一致；两个构建组件锁与profile一致。

提交：CVA6 `0dc5e333`，Linux `5853e425caf8`，平台硬件锁`de34dbc`、软件/分析实现`3f060f3`，交付文档`3e25ce7`。构建时保留无关脏文件；未合并或清除旧仓库/日志。FPGA在de34dbc开始，期间平台软件提交至3f060f3；固定组件和profile不变，来源记录保留两端。Linux `6.5.0-rc2-00092-g5853e425caf8`。

完整软件构建exit0。FPGA第一次受限沙箱无法连接许可证服务器，失败日志与暂存保留；获网络权限后完整构建exit0，BIT/MCS非空。正常构建日志只落盘，未巡查；不新增时钟约束审查或板测时序结论。

交付 `iommu-platform/out/research-fullstats-20261002/research-fullstats-20261002.tar.gz`，24,405,743字节，SHA256 `9080fe9e95b25756b3718dff88469d7738bfcc13729fb9f220a572b8c2a7146f`，归档成员逐字节核对。BIT SHA `be463eeb2b72b85df47477ea4982c4ed5289bca2829015c2d25bc6a62c9c95a7`；payload SHA `637fff6f897c2dc117a31290e4244b370a74941570a18daa35aca0b590e0ea16`。包内DELIVERY.json/SHA256SUMS有完整文件摘要和测试/构建来源，MCS一并提供。Mac说明见 `iommu-platform/docs/research-fullstats-mac.md`，包内 `MAC_HANDOFF.md`、`RUN_FIRST.txt`。

下一步仅由Mac在新启动用同包新BIT＋payload执行 `platform-research cost-fullstats /tmp/research-fullstats-01`（目录须新建），自动3短＋18主；保留所有样本、警告、taint和完整UART，遇错停止，不自动重跑。新板测当前0/21，Dell不接板，也未自动发送消息或替换Mac文件。旧E2/旧84不重跑，旧11结果保留；不跨版本拼接。新结果用于排除固定抽样偏向后再判断M原型是否值得做，不是M收益或创新已成立。

## 2026-10-03 fullstats21回传分析与一次部署成套实验计划

Mac新BIT/Linux00092的3短+18主已在线通过；Dell现有research_results.py cost-fullstats复算exit0、21/21 ok，无failed/missing/not_run。独立从21原JSON raw C/D/sample及P4M window重算守恒、范围和V/A速率；完整回传198文件SHA256未变。证据：iommu-platform/out/research-fullstats-review-20261003/{acceptance.json,direct-check.json,condition-statistics.json,analysis/results.json,RESULTS.md}。没有重跑、构建或新RTL/软件修改。

主条件各3次中位V/A MiB/s：Solo283.050/无A，A1g0 149.548/149.548，A8g0 108.907/108.907，A16g0 100.994/100.994，A8g1000 150.764/75.382，A8g2000 171.375/57.079。RESULTS主表为三次均值，不能混写为中位数。全量req4/miss2每run均值约g0 1406–1410、g1000 1100–1103、g2000 840–842周期；旧固定16抽样在g1000只取g0，漏掉1536/2047份g2。采样总体偏差不能因抽中组均值接近而忽略。A16前两次V hit16382/miss2，第三次16384/0，组分布也变化，全部保留。分组只含窗口内完整copy，窗口总计还含边界事件，不强求组miss和等于窗口miss。short-a8有hrtimer13207760ns，taint0；原回传引用交付哈希，没有fresh下载哈希/SD回读，不能补造。

DeepSeek两次成功整理/修订报告计划，完整回复deepseek-response.json与deepseek-revision-response.json，均未截断。Codex核查原始证据及原论文控制合同，修正草稿将“此次无新板测”误写为结果未上板、组与窗计数强等同、训练拟合替代控制评价、科学阴性即停、无授权措辞等问题。报告与研究计划已审定，不以模型同意代替证据。

用户连续强调“结合整体路线、创新”“一次上板多做有用实验”。审定计划docs/research-next-single-deployment-20261003.md：现象→E2/E3局部机制→P4M取舍→全量成本证据→下一关M公平控制评价。SafeSU/AXI-REALM已有时间成本/周期预算观念，M新意只保留有条件的额外控制价值，不宣称创新成立。A1主占用20周期但明显共跑减速是窄信号限制，须作为反例条件。

下一次目标同BIT+payload运行时模式，成套准备主方法对比、三个保护档、A1/A8/A16、计费依据消融/滞后对照、板前预声明独立布局/顺序/相位、开销稳定性。非全笛卡尔积；精确总表与重复/阈值/预计耗时在实施及交付前冻结。校准候选和选择算法板前固定，验证不反调；数据/硬件错误停止，方法无收益照预定矩阵收阴性。连续逐周期扣费与下一copy边界放行不能混为完成后记账。新的控制器接口必须验收，但不重启已完成测量或重跑旧E2/21。此轮只完成分析和计划，尚无新控制实现、构建或板测。下次从控制规格和有限同包矩阵继续，不重新全面扫描仓库。

## 2026-10-03 E2 同步纳入下一次同包部署

用户问“下一次上板是否可以E2同步推进”，已明确纳入计划，见iommu-platform/docs/research-next-single-deployment-20261003.md第7节。同期准备、同BIT+payload、阶段间排空和重建初态、顺序运行。E2新增X1 W7/8另一布局或顺序边界复核，X2 W8/16有限访问前缀（1/8/16候选，实际覆盖与序号关联须明确），X3复用用例采集V首次探测等待/翻译/完成代价；自然方法共跑关联V miss/PTW和必要驻留证据。旧60不整套重跑，新版少量对照有必要。具体参数/重复/总时长和接口交付前冻结。

DeepSeek扩展草案保存在out/research-fullstats-review-20261003/deepseek-e2-extension.json，未截断。Codex直接读dma_research_observer.sv的阶段准入/quiet/截止守卫及更新事件接口：现有阶段戳不是完整探测计时。纠正草稿过推：计划统一112不证明底层--a-transfers不支持其他值；驻留不变不充分证明服务阻塞；100ms截止窗不是性能；不能把E2 A-only时V非活跃的M开关当完整反馈评价；首访总时间差仍有内存状态混杂；E2探索布局不得充当M未观察验证集。本轮仅计划文档，无RTL/驱动/脚本实现、构建或板测。

## 2026-10-03 用户澄清部署成本：SD镜像优先，E2/M不强求同步

用户原话：如果不能同时就不要勉强同步，麻烦在Linux重新编译后SD插到Mac拷贝再插回FPGA；只重编BIT可经JTAG传输、不必动SD。此前“一次上板”不得再解释为所有实验必须同BIT/同时完成。已修改docs/research-next-single-deployment-20261003.md：先审查/冻结共同软件接口，必要Linux/payload集中交付；后续兼容BIT独立构建和JTAG更新，按流程重新启动/准备初态。当前00092兼容未来模式未核查，不承诺无需软件改动或热切保留运行态。

E2扩展项目保留独立工作包，能并行准备则并行，不能就顺序或分BIT，不让E2阻塞M。方法与直接性能基线仍优先同方法BIT；不同BIT/boot保留版本，不能伪配对。无需为合并E2/M额外扩大硬件。此次Codex仅落实用户对部署约束的明确更正，未调用DeepSeek重复讨论，未修改实现/测试/构建或执行板测。

## 2026-10-03 用户接受两BIT共用Linux，Mac端AI轮流执行

用户进一步明确可以一次上板准备两个BIT，轮流执行，交给AI而不人工制作SD卡。已将交付目标更新为共用一份Linux＋E2/M两份兼容BIT＋Mac端顺序执行流程。优先复用现有SD镜像；若当前00092缺驱动/工具支持，必须板前核查集中解决，不假装兼容已证。Mac流程含JTAG加载、重新启动、版本/身份核对、各自短验收与实验、收集结果确认后才换第二BIT；BIT/boot/目录分开，技术错误停止、不自动重跑，不擦写SD。当前只是交付要求，尚未实现自动脚本或新BIT；Dell未连接FPGA。此次按用户明确偏好直接更新计划，没有新增DeepSeek讨论或构建。


## 2026-10-03 两BIT共用软件实现中（非交付）

用户授权设计后实现。合同：[research-two-bit-v2.md](iommu-platform/docs/research-two-bit-v2.md)，共用新Linux＋role1 E2/role2方法两BIT；E2 64项、方法462项，Mac逐项取回后才切BIT，无自动重跑。当前00092不具备新ioctl，集中更换一次软件，后续BIT切换不动SD。

已写入真实源码：UAPI0x0f/0x10、frontend ADMIT/反向、D80计时及F能力区、E策略区、driver新旧接口、CLI，两profile和builder参数。DeepSeek起草，Codex审查低层时序/错误恢复，应用并执行。主机通过：观察器312+2、计时116+2、frontend6、CLI61/旧CLI77、driver配置/能力/反向覆盖、新旧部分回归；Linux idma.o编译0。控制器参考仿真诊断未见随机参考失配，但正式定向测试仍在修订期望；不得写全套通过。Mac/分析器草案仍修订。完整构建尚未开始，未打包、未上板。

草案、原始DeepSeek回复与检查日志位于 `iommu-platform/out/two-bit-20261003/`。继续需完成待返回修订/连接仿真、原调度回归，提交相关组件更新gitlink+lock后完整构建一份software及两FPGAprofile，再交付Mac。正常构建日志只保存；实际错误才查。不清除无关用户改动。


## 2026-10-03 两 BIT v2：主机验收完成、长编译等待结果

用户授权设计后直接实现，集中更新一次软件后通过JTAG轮流加载两个BIT。共用软件00093完整构建通过，E2计时/能力、方法控制、准入排空、反向覆盖、CLI/计划、Mac保存后换BIT与分析器验证通过。具体测试和限制见 `iommu-platform/docs/two-bit-build-acceptance-20261003.md`。E2 64＋方法462清单冻结，未板测。

初次两BIT在Vivado展开因default_nettype none下输入net未显式声明失败；原失败构建目录及日志保留。修复仅input logic→input wire logic，单模块Vivado及控制器回归通过；CVA6 `a0497a8d`，平台 `10fd189ee705`，两BIT已重新开始完整构建。软件仍使用平台 `481bd47fd9d6` 生成的Linux `c0f5e13ea81f`/00093，原锁文件与实际软件哈希已核对；rootfs中idma_research与构建二进制一致。正常构建日志不巡查；最终BIT/时序/打包结果待补。

DeepSeek负责草案和有界修订，Codex审查实际接口、RTL及错误后应用和执行。审查纠正：策略直方图总和是E20窗口数、Mac cd失败须阻止后续分号命令、提前停止无selection时仍可分析、新版分析入口是research2_results.py。


### 2026-10-03 v2 最终构建复核发现并修正方法组合反馈

E2完整构建退出0，最终WNS 0.177ns/WHS 0.028ns/WPWS 0.062ns、失败端点0、组合回路0。方法初版虽退出0生成BIT，但报告WNS -0.322ns/TNS -59.515ns、598建立时间失败端点和3组合回路；拒绝交付。旧fullstats报告回路0（旧WNS -0.944ns，原约束审查仍未完成），因此未将新回路当成历史警告忽略。

实际方法回路报告 `out/two-bit-20261003/method-loops.rpt` 指向策略同拍错误→frontend终止/观察→错误统计。Codex改为当拍allow仍关闭，abort仅显式ABORT或已锁存错误驱动；正常无错计费及放行规则不变。CVA6 `475f3e19`，方法构建平台 `9a172d2`；E2仍使用平台 `10fd189` / CVA6 `a0497a8d` 的已通过产物（role1不实例化策略模块），软件保持00093。

DeepSeek起草测试修订；Codex执行3744＋窄位宽618＋关闭模式5检查及真实DMA A端口配置的10连接场景，均通过。Vivado真实frontend＋policy小范围综合回路0，证据 `out/two-bit-20261003/registered-abort-synth/loops.rpt`。方法完整构建重新开始，日志只保存。旧方法构建保存在 `out/migration-4dma-research-method-v2/fpga-src-before-registered-abort/`；旧网表物理优化尝试因确认须修源码而停止（143），不能交付其产物。原文件、日志均保留。


### 2026-10-03 方法版继续收敛时序：直接解码研究ARM

registered-abort修复版完整构建退出0，组合回路归零，但最终WNS -3.979ns/TNS -17075.918ns、9454个setup失败，因此拒绝交付。旧产物保存在 `out/migration-4dma-research-method-v2/fpga-src-before-direct-arm/`；manifest在 `out/two-bit-20261003/method-registered-abort-negative-timing.manifest.json`。关键路径证据 `out/two-bit-20261003/method-direct-arm-timing-evidence.txt`。

Codex直接检查路径与源码：research_arm_o原来由phase_n==ARMED推导，引入统计RAM读回→poll_id→phase_n→fullstats清零的无关长路径。改为仅在原先已接受ARM命令分支同拍置位，默认0，不加流水延迟或改变ABI/计费。DeepSeek起草定向测试；Codex应用执行P4M35＋窄位宽2＋研究35、旧观察器28＋6场景通过。CVA6 `a1b08934391833aed21e6ad63b545634040808ac`，平台 `7430772309dbed9453b505eb97f43c2a3f54f141`；方法完整重编已开始（direct-arm.build.log），尚未完成或交付。保留已通过的软件00093/E2，不重编它们。DeepSeek正在补打包时序门槛和版本交接；后续仍需最终时序/loop实际报告验收及打包。


### 2026-10-03 直接ARM版方法构建完成，DDR布线物理优化中

平台7430772/CVA6a1b08934完整构建退出0，组合回路0。WNS -0.150ns/TNS -0.179ns、仅2个setup失败，hold/pulse均0；实际两条最差路径在DDR200MHz写缓冲，第一条1级LUT、92.965%路径延迟为布线。未交付。基于当前routed.dcp执行phys_opt_design AggressiveExplore + route_design Explore，独立输出 `out/two-bit-20261003/method-direct-arm-postroute/`，真实输入DCP/原manifest/Tcl/原timing哈希已写input.json，session46536尚未退出。无RTL/时钟/实验参数变化。

DeepSeek起草additive优化manifest与打包支持，Codex审查并指出常量、JSON Path序列化、重新核对输入哈希的问题，修订进行中。原3构建manifest和负时序原始BIT不得覆盖；最终必须记录优化输入/实际退出码/报告与最终BIT哈希、时序门槛通过后才能打包。正常日志不巡查。


## 2026-10-03 两BIT v2最终交付完成（替代前述编译中状态）

用户授权设计后实现，落实共用Linux＋两个兼容BIT，避免实验切换重复处理SD。软件00093、E2 BIT与方法BIT完整生成；E2最终WNS +0.177ns/WHS +0.028ns/WPWS +0.062ns，方法经独立物理优化后 +0.048/+0.028/+0.062ns，三类失败端点/总负余量/组合回路均0。Vivado优化session46536实际退出0，检查点/Tcl/原manifest/原timing真实哈希保存在input.json，执行结果在execution.json，优化输出manifest已实际校验。原负时序基础BIT和所有失败构建均保留，不得用于上板或覆盖其manifest。旧IO/clock/CDC整体审查未完成，不宣称全平台约束签署。

最终role1 BIT SHA256 `319b415e0a717565fa11df7bf23c1c9cc6ac8e283cdbaea791e795c9b2924d40`；role2 `31e18ee96e6edd47d444ed3cf9f4279cfa1ba494b53497d37183a3d7d0c33651`；payload `af9f703b6d00420598282a983201361983b17ed731bc245284b67689dcd9e748`，rootfs中CLI与交付CLI一致。软件平台481bd47，E2平台10fd189，方法平台7430772/CVA6a1b08934；交付工具提交 `0a570deda89636631462bb188d39c058409df364`。

交付包 `iommu-platform/out/research-two-bit-20261003/research-two-bit-20261003.tar.gz`，21,734,376B，SHA256 `95fbddda5488230e7bca03dcd0890883f3a9cef5201a06f6b1bec03242eb045f`。解压后为delivery/，30个文件；全SHA、角色BIT、CLI、Mac依赖、E2 64与方法462冻结清单均通过实际解包核验，证据同目录acceptance.json/package-smoke.json。首次核验脚本误将包根当作delivery根，修正核验路径后通过；包未改动。

主机回归明细在docs/two-bit-build-acceptance-20261003.md；补充启动脉冲P4M35＋2＋研究35、旧观察器28＋6、打包时序20及优化溯源12通过。所有新增测试/脚本已提交；已有无关用户文档/板端脚本改动未纳入该提交。正常构建日志只保存，无重复巡视；必要最终时序检查发现问题后修正源码/物理优化直到门槛通过。

Mac交接 `iommu-platform/docs/research-two-bit-mac-20261003.md` 与包内同名文档。当前SD00092缺新接口，需要集中更新一次payload00093；后续Mac工具经JTAG依次role1→保存并哈希核验→role2，重启核对kernel/CLI/caps/boot_id；技术失败即停、不自动重跑、所有样本留存。当前新板测0/526，Dell未连接FPGA、未实际执行串口/JTAG或发送给Mac；方法收益与创新未得到新证据。下一步是Mac运行包内流程并完整回传，不重开设计或重扫仓库。

DeepSeek起草实现、测试、脚本和文档，以及有限诊断/修订草案；Codex完成关键低层审查/决策，执行工具应用补丁、实测验证、构建和产物接受。


## 2026-10-03 用户确认新版64项E2已有结果，等待Dell回传路径

用户要求先分析E2线结果与下一步，并明确指新版64项。定向检查当前交接及out一级目录，仅定位旧mac-research-v1/cont20261002，尚未找到新64项原始目录；已用异步问题询问回传路径/Mac回传说明，待回复。不把旧63项当新64，不凭缺文件判实验未完成。DeepSeek background证据梳理保存在out/e2-results-review-20261003/deepseek-background.json与background-evidence.md，非新数据报告；后续须实际读新原始结果/复算/哈希核对再给结论与下一步。本轮无RTL/驱动修改、编译或板端操作。


## 2026-10-03 新版E2 64项结果分析、离线验收与下一步裁决

用户补充回传路径`iommu-platform/out/mac-research-two-bit-20261003/e2/`，此前“结果待定位”已解决。只处理新版E2，未读取正在运行的方法462项结果，未改硬件/驱动、编译或操作板卡。读取现有状态/合同后定向检查关键观测器、计时块及驱动路径，未全面扫描仓库。

冻结research2_results.py实际退出0、64ok/0failed/0missing；本次e2_audit_aggregate.py最终64clean/exit0。64份归档SHA及成员、板端record与顶层record、元数据/参数/exit/taint/uname一致；64逐项dmesg before/after无新增，stderr空。分析前后1226份输入SHA完全相同。首次BusyBox tar -z采集失败后只重取原结果，第1项没有重跑；同一boot，有采集暂停，不称独立重启重复或不间断采集。

本批达到E2局部机制与首次再用代价的阶段门槛：W7正反各5次保持V两目标；W8正反各5次均有A阶段DID11更新覆盖V条目、探测前消失、A停止排空后V首miss的完整证据链。前缀W8/W16的N1保持、N8/N16替换，实际页覆盖按A阶段提交更新页并集逐次核对，不把最终快照当完整历史，不把W16N8说成访问16对页。所有64次更新重放复现deadline/freeze快照，共62目标替换后首miss、66保持命中；无probe诱发丢失或未决类别。128目标非128独立试验。

W7正反V整次4KiB探测中位696周期，W8正/反1483/1486；D89翻译服务占用中位20与822/821，全部64 D88首次接纳延迟5周期。50MHz，约13.92与29.66/29.72µs。2目标首查与完整4次lookup分开；不能把总时间减翻译占用称纯排队、单次探测称稳态吞吐、固定5M周期A窗口称替换延迟。

裁决：E2本阶段收尾，暂无新增上板或软件/BIT更改。主线待方法462完整回传后按冻结规则比较V保护程度相近时A进度、开销、稳定性及独立验证。E2为缓存替换证据，E3为共享服务阻塞证据，均不能解释全部共跑损失；E2现象不单独构成创新，是否加入缓存保护须结合方法结果决定，不自动叠加新硬件。精确前缀门槛/跨布局泛化仅在论文主张确需时再设计，不是当前待执行实验。

报告和证据：[RESULTS.md](iommu-platform/out/e2-results-review-20261003/RESULTS.md)，同目录offline-v2、audit-final、accepted-summary.json、integrity-result.json。DeepSeek前缀草案存在寄存器误读、A/V映射误读及无效tag推断未flush等错误，均经原始值/代码复核排除；两轮汇总草案存在字段接口错误，Codex限定修正并实际执行通过。早期草案和失败输出全部保留，不用于正式结论。DeepSeek负责证据整理和脚本/报告草案，Codex负责关键证据审查、执行、修订与最终决策。


## 2026-10-03 23:45 role2三次定时核对及E2下一步说明

用户授权在5、10、15分钟后确认role2方法462项，再分析和制定计划。检查起点23:30:23；实际检查程序按计划时间读取Dell回传目录和现有状态/交接，不接触板端。

| 检查点 | 实际时间 | Dell回传情况 |
|---|---|---|
| 5分钟 | 23:35:23 | 无method目录、无role2 record、无stage-done |
| 10分钟 | 23:40:23 | 无method目录、无role2 record、无stage-done |
| 15分钟 | 23:45:23 | 无method目录、无role2 record、无stage-done |

目标为`iommu-platform/out/mac-research-two-bit-20261003/method/`；最新同批目录仍仅E2，三次之间CURRENT_STATUS/HANDOFF未变化。最后一次轻量检查out一级相关Mac回传目录，未定位另一份新方法数据。应用list_threads只读查询未返回信息，约6分钟后取消，不据此推断Mac运行状态。

结论：只能确认role2尚未回传到已检查的Dell位置，不能认定板端未完成或失败。本轮没有分析方法收益，也没有重跑、调参、编译、Linux/BIT更改或发送Mac指令。三次检查已结束，没有无限期监视。

收到完整method目录（原始记录、计划、选择结果、启动/结束元数据、日志及SHA）后，再按冻结规则校验完整性、分阶段汇总、公平保护下比较A进度、开销/稳定性和独立验证。DeepSeek负责必要证据整理与分析草案，Codex核对关键证据和制定最终计划；本轮纯机械检查未调用DeepSeek。

E2下一步澄清：先完成本批机制证据归档；若方法结果/论文主张确需保护缓存，再设计共享缓存与少量V预留位置的对照。预留属于常规基线，不能直接称创新。比较应保持A实际完成次数与页覆盖一致，避免将A少干活误判为缓存保护收益。当前不追加E2上板或硬件实现。


## 2026-10-04 role2新增三次定时核对：15/30/45分钟

用户重新授权于日本时间00:22:53、00:37:53、00:52:53检查role2方法462项回传。本轮起点00:07:53，开始时Dell仍仅E2；检查记录保存`iommu-platform/out/role2-status-checks-20261004-0007/`。完整method数据到齐后按冻结规则离线分析并制定计划；未齐不推断板端完成状态，不重跑或改变板端实验。下方23:45为上一轮三次检查结果。


## 2026-10-04 00:52 role2新一轮15/30/45分钟检查结束

用户授权新一轮三次确认，从00:07:53起计；检查已完成，检查进程退出0。

| 间隔 | 实际检查时间 | Dell已见情况 |
|---|---|---|
| 15分钟 | 00:22:53 | 同批目录仍仅E2；无method结果、无完成标记 |
| 30分钟 | 00:37:53 | 同上，无新的相关Mac回传目录 |
| 45分钟 | 00:52:53 | 同上，状态与交接内容未更新 |

目标路径：`/home/xvhaocheng/work/iommu-platform/out/mac-research-two-bit-20261003/method/`。每次仅读取同批目录、相关Mac回传目录一级列表、CURRENT_STATUS头部与HANDOFF尾部。原始三次快照为同目录check-15min.json、check-30min.json、check-45min.json；request.json记录本轮授权时点与three_checks_finished状态。

结论：截至最后检查，Dell未收到role2方法462项的结果。不能据此判定Mac板端未完成、失败或仍在运行。本轮未直接连接Mac/FPGA，不启动实验、重跑、调参、编译或修改Linux/BIT，也未作方法收益分析。

下一步需要Mac侧实际进度或完整method回传目录；若已有结果存于别处，需提供路径。收到数据后继承冻结清单和选择规则，先验证完整性/版本/失败记录，再比较相近V保护下A的进度、开销/稳定性和独立验证，最后决定整体路线是否调整。没有新结果前保留现有计划；E2局部机制阶段验收结论不变。

本轮是机械定时核对，未调用DeepSeek。完整结果到齐后由DeepSeek整理分析草案，Codex核验关键证据并决定下一步。本轮三次检查已结束，没有设置后续无限监视。

## 2026-10-04 方法462结果分析与下一步裁决


### 已完成且已接受

- 462/462 summary ok，0 failed/missing；462 条 case records 全部 unique。证据：[机器证据](iommu-platform/out/method-results-review-20261004/accepted-evidence.json)。
- 10041 回传文件 before/after_existing 均 10041，changed_or_removed 空，added 空，unchanged true。证据：[机器证据](iommu-platform/out/method-results-review-20261004/accepted-evidence.json)。
- raw audit：462 cases，errors 空；stderr 非空 0。证据：[机器证据](iommu-platform/out/method-results-review-20261004/accepted-evidence.json)。
- 指定内核 warning 仅 `m-short-unlimited-a8` hrtimer 13930720 ns；其余 added dmesg 为正常观察消息，不自动当 warning。证据：[机器证据](iommu-platform/out/method-results-review-20261004/accepted-evidence.json)。
- Codex 已实际执行冻结分析器 462/462、汇总脚本 exit0、独立全部 462 归档 SHA/成员/板端 record/元数据参数/exit0/taint0/uname 核查，以及 C19、C12/P4Mwindow[3] 吞吐复算，全部一致；p5E26==E32；stderr 全空。
- 环境：Linux `00093-gc0f5e13ea81f` role2；单一 boot `93406a68-fc8a-4040-9dc6-ae4e4a670d36`；收集结束 2026-10-04 00:50:01 JST；此次非板测重跑。
- 原始 E2 报告已接受 64 项不重复合并/改写；原始 README 只采集禁分析已被本对话明确“继续分析并制定下一步计划”覆盖，原始 README 不改。

### 关键决策

- 功能全部成功不等于创新成立。
- 正式 p5 vs fixedgap t150/t170 公平匹配 A 增益 +15.289%/+24.211%；正式 t170 vs p1/p2 为 -6.464%/-6.110% 匹配反例。
- 反向 p5 相对 p6（目标170） +40.672% 且匹配，但正式 -3.791%；方向与采集阶段混杂，不能归因方向。
- 反向 p5 相对 p4（目标120） +12.878%、t150 +5.247%（CI 跨 0）正例保留；反向 p5 相对 p1/p2（目标170） 为 +0.963%/+2.697%，未达 5%。
- 反向 fixedgap 三个目标均不满足公平匹配，不能说复现正式 fixedgap 收益。
- 边界 A1/A16 p5 V 中位 149.581/147.257，未达 150；A1 差小也不事后放宽。
- 参考 solo 283.099、A1 149.507、A8 108.904、A16 100.930 MiB/s，来自当前同版。
- 更准成本测量不等于控制更好；同外层反馈可补偿稳定负载简单代理误差，是待验证解释，不是本数据证明根因。
- E25 许可关闭不等于实际 stall；E2E 只按 ctl_did==A，不筛状态，不是实际 cost；p5 E32 才 actual；E39 armed 与 C19 共同窗口口径不同；V 完整活动窗不足不是硬延迟 SLA；无旧 BIT 对照故未测零开销；holdout 未参与调参但不是独立重启。

### 下一步交接

- 计划文件：[88项复核计划](iommu-platform/docs/research-method-next-20261004.md)。
- 报告文件：[结果报告](iommu-platform/out/method-results-review-20261004/RESULTS.md)。
- 计划为固定 A8 target170 的 88 项复核：7 方法 × 2 方向 × 6 轮，2 启动；每启动 3 轮正式 42 项 + 2 项短测，共 44 项；合计 88 项。
- runner 未实现；manifest 未冻结；未启动；未编译。实现前先核对现有接口和版本，确有缺口再审查。
- 本次审定计划判据：同方向两次启动均匹配且 p5 相对“当前最有竞争力的简单对照”A 增益 >= 5%，才作为窄条件工程信号；2 方向皆成立才谈跨方向。不存在匹配可行对照时，结论为证据不足，不是成功。完整执行顺序/seed/hash/manifest 仍待实现验收后冻结；不宣称可执行包已交付。
- 若 88 无稳定额外收益，结束当前固定 A8 实占主创新路线，p5 作为已测试候选/基线；先设计可证伪问题，再改硬件。技术错停保留，不自动重跑/删除，科学阴性继续预定清单。

DeepSeek 起草证据整理、统计程序和文档；Codex 核对关键来源、实际执行验证并作最终决策。


## 2026-10-04 N 跨设备原型实现中的交接

- method88 的阴性决策不变；N 是合理的有限命中绕过对照，不是已确认创新。
- 已实现跨 DID 两银行请求生命周期标记、实际 DDTC/IOTLB 命中路径、单响应口仲裁和失效回退；请求总容量与缓存容量保持原值。FPGA 顶层接线正在验证，MSI 使用扩展设备上下文格式。
- 已实际通过真实翻译模块的初版定向仿真及原 IOTLB 703 次行为对照。追加用例、完整数据通路联合测试尚在处理，不能宣称全原型通过。
- 尚无新 FPGA 构建或 N 板测。下一步完成联合验证、编译同银行布局的关闭/开启绕过两 BIT，再通过 SSH Mac/JTAG 沿用 Linux00093 做 P4M 工程试验。旧 COST/E3 翻译计数不用于 N 结论。
- 用户授权循环执行、分析、实现和更新文档，直到实际需要人工更新 SD；不在规格完成处停止。证据与 DeepSeek 草案保存 `iommu-platform/out/n-prototype-20261004/`。


### N 原型验证更新（2026-10-04）

- 真实翻译模块追加测试通过，控制器的身份、顺序、写数据先到、错误响应和反压测试通过。联合实际控制器+翻译模块测试通过：跨 DID 命中先行、关闭绕过对照、完整 4 KiB 读写、权限拒绝、同 ID 并发请求顺序及最终排空。原缓存行为 703 次对照通过。
- 测试环境使用 Verilator 4.038；不支持的供应商 SVA 通过 ASSERTS_OFF 关闭，保留新控制器的立即错误检查。这是明确的仿真范围，不是全形式/协议验证。
- DeepSeek 起草测试、构建快照和只读计数模块；Codex审查核心RTL、生命周期/窗口/屏障接口，应用草案并实际执行测试。
- 新只读银行记录共同搬运窗口内的快速/普通接收数，不沿用旧 COST 语义。用户程序固定访问 DMA0 观察寄存器并恢复索引，已完成 RISC-V 静态编译；板上 /dev/mem 可用性与计数器还未验收。当前00093内核配置支持 /dev/mem，准备沿用现有 SD。
- 尚无新 FPGA BIT 或 N 板测。工程筛选冻结为三个硬件版本、每版 15 项，参数见 docs/nonblock-pilot-plan-20261004.md；正常编译日志仅保存，不巡查。

### 2026-10-04 N 编译启动

计数模块 Mode1/Mode2 实际 PASS，联合数据通路更换 512 个不重复字的数据后实际 PASS。日志：`out/n-prototype-20261004/monitor-fixed.log`、`joint-unique-data.log`。两个新 profile 已使用显式十文件工作树快照启动完整 FPGA 编译，日志分别为 `fpga-serial.log`、`fpga-hit.log`；尚无构建或板测成功声明。原版 15 项工程对照准备在编译期间经 SSH Mac 运行，Linux00093 与 SD 沿用。

### 2026-10-04 N 原版对照 15/15 完成

`out/n-prototype-20261004/nonblock-pilot-20261004-original-03/summary.json` 为 complete、15 completed、0 failed、无 unexpected_warnings；真实 P4M record 的 V/A data_errors=0、status=0、taint=0。Linux00093，boot `161dcc93-c6fe-4344-845c-f32f925cf183`。原版 BIT JTAG 成功，但原 original-02 启动在 Saving random seed 附近超时；一次 Ctrl-C 后出现 getty，经实际登录确认新启动。original-03 只复用该已写入、未执行任何实验的启动，15 项均首次执行，不再次 JTAG。具体卡住子步骤未确认，不能断言熵不足；此启动恢复是工程限制，后续版本须记录并采用一致规则。两新 BIT 仍编译中，无 N 板上性能结果。

### 2026-10-04 关闭绕过版构建完成并启动板测

Profile migration-4dma-n-serial 完整编译 exit0，BIT/MCS 非空，显式快照来自十个选定 RTL 文件。最终 routed summary：WNS +0.177 ns、WHS +0.047 ns、WPWS +0.062 ns，三类失败端点0；未约束内部端点/组合回路0。原有IO延时缺项仍存在，不能称整个IO/CDC约束审查完成。已经SSH Mac/JTAG启动 banked-serial 15 项，首次验证RO计数器；尚无该版板测结果。开启版继续编译。

### 2026-10-04 银行串行版启动与上传故障

serial-01 成功 JTAG、Linux四DMA注册，300秒等待在随机种子初始化附近超时，无样本。一次启动恢复后 serial-02 实读 Linux00093、boot b58776e2-8aa2-4643-bb92-b929215bf75e、taint0、caps成功、/dev/mem存在；失败发生在415KiB只读工具的base64 heredoc上传，默认串口write_timeout=0.1秒且全速连续发1024字符行造成Write timeout。无case启动，无实验重跑。失败原始目录已回Dell；DeepSeek正在起草限速/写超时修正。guest未结束heredoc已单独中断恢复，属于工具上传恢复，不能写成第二次随机种子干预。原版15/15结论、串行版时序验收不变，N开启版仍编译。

### 2026-10-04 开启绕过版构建完成

Profile migration-4dma-n-hit 完整编译 exit0，BIT/MCS 非空；routed summary WNS +0.177 ns、WHS +0.045 ns、WPWS +0.062 ns，建立/保持/脉宽失败端点0；no_clock/未约束内部端点/组合回路0。验收元数据 `out/n-prototype-20261004/hit-build-acceptance.json`。两版都使用显式十文件快照，旧IO延时约束审查仍未完成。串行版工具上传已按115200限速且提高写超时，正复用 b58776e2 启动继续首次试验；开启版待串口释放后上板。

### 2026-10-04 N 首次工程验收未通过

串行版serial-03仅短Solo/A1退出0且数据正确，短A8未正常返回；剩余12项未开始。RO窗口cycles与P4M相同，但所有实际接收计数0，不能作为机制合格证据。已确认源代码931-933信号局部于gen_dma，而1381-1382在gen_iommu产生implicit wire；Vivado日志明确undeclared/undriven。Codex已把fire/accept/did/type移到模块作用域并为IOMMU-disabled补零，尚未重新编译。此观测接线错误不自动解释A8停滞；DeepSeek在起草必要多页压力复现和完整top共享AXI/PTW证据，根因仍待确认。host runner PID46254已停止，不自动重跑样本；原始日志保留。两BIT虽然完整构建和约束时序通过，功能未验收，暂不能用于正式结果；Hit未上板。接下来修复后编译v2再JTAG，不需要SD更新。

## 2026-10-04 N停滞后的恢复与排除记录

短A8没有退出后，保存原始目录并停止对应Mac runner，JTAG恢复已知可用原版BIT。实际UART确认kernel00093、boot956fa8d0-0bf3-4104-a8b1-2af736624a01、taint0，记录out/n-prototype-20261004/restored-boot.json；没有重跑任何样本。

Codex短仿真检查了“跨银行AW重排直接堵住W”的候选解释：当前axi_demux_simple在前一银行W未完成时阻止切换AW银行，两个模式的512写beat/两个B均完成，aw-order-repro.log保留。该候选不成立；不能据此排除读写数据依赖/共享DDR响应停滞。正在由DeepSeek起草有限读写缓冲＋共享返回路径测试和接收计数非零guard，Codex继续核心诊断。计数作用域接线修复尚未重编。

## 2026-10-04 v1当前进展

- 已修复计数generate作用域接线；Codex补读写依赖约束，S/N使用同一规则，旧控制器默认行为不变。
- DeepSeek起草读写依赖模型、脚本与诊断；Codex连接真实模块接口、审查/修改关键RTL并执行。共享返回模型3种发起顺序36场景、原控制器12对照、原有联合/控制器测试、计数非零guard全部通过。模型尚未包括实际DMA后端、DDR以及同一任务两段的全部重叠。
- 两个v1 BIT完整构建正在运行；旧v0失败构建保存在out/n-prototype-20261004/failed-v0-builds/，原逻辑路径映射见RELOCATION.json，不覆盖失败证据。
- FPGA已恢复原版Linux00093，boot956fa8d0-0bf3-4104-a8b1-2af736624a01，taint0。无新正式样本、无旧样本重跑，无SD更新。编译/必要最终时序完成后JTAG继续两版各15项；原版15项已完成不重跑。
- 根因边界、科学影响和原始路径见iommu-platform/docs/nonblock-dependency-diagnosis-20261004.md；不能称N有性能收益或新颖性。


### N v1后续分支草案（构建等待期间；未启动新实验）

DeepSeek已依据实际RTL修订后续分支，完整输出在 `iommu-platform/out/n-prototype-20261004/deepseek-next-pilot-branches-fix.json`。Codex确认fast实际接收可由raw fast>0证明，Solo fast0正常，不是命中率。科学阴性不默认重跑；有收益才考虑交替顺序S/N新鲜启动配对。新增诊断接口只是草案，窗口起止在途余额与真实排队读守卫计数定义尚待架构审查，未实现。见 `next-pilot-branches-review.json`。当前仍只执行既定两版v1各15项，无SD更新要求。


### N v1两版完整编译结束

S/N均exit0，BIT各11,443,722字节、MCS各31,472,748字节；最终WNS均+0.177ns，WHS S+0.014/N+0.035ns，建立/保持/脉宽失败0，组合回路、无时钟、未约束内部端点0。旧IO/clock/CDC全面审查仍未完成。验收记录 `serial-v1-build-acceptance.json` 与 `hit-v1-build-acceptance.json`，板测待验收。S v1 BIT已传Mac，正常编译日志未巡查。DeepSeek新增两突发重叠模型；补丁两次git hunk对接失败，Codex按唯一原文区间重建统一diff应用，未另改实现语义。正在运行新增模型，未重跑已有默认36场景。


### N v1轮询交错重叠功能拒绝，未上板

DeepSeek起草rr合法AXI交错刺激，Codex审查应用并实际运行：banked8/12共跑场景停滞，Solo4通过；legacy12/12通过。首错V第二段R排在共享返回队首，V cfifo3满，V第二段AW未接收；A当前W owner需要后方R或PTW响应，形成有限模型等待环。v1读守卫仅按当前W所属银行放行，未约束同银行读提前到下一未准备写突发。模型不覆盖完整真实fabric，仍不等于旧板端A8根因最终确认。两版v1虽编译/时序通过但功能不接受，保持Mac原版Linux；BIT已传但未加载。后续由Codex审查读写资源约束后修复；无SD需求，原始日志保留。


### N v2选择整份缓冲并开始两版编译

Codex核查batch_cfg_ok nbytes4096且src/dst页对齐、COUNT→WAIT等trans_complete，选择每参与DMA整份4KiB缓冲，避免更限制一般读写比例的预算规则。已改FPGA .BUFFER_DEPTH：N mode非0且i<2用512，否则3；S/N同配置、额外数据8144B、MetaFIFO11→520。既有bank守卫和TL容量保留，仅BIT变更，无Linux/reader/ABI或SD变更。DeepSeek起草depth选项/规格，Codex修正两次失败的注释diff上下文并应用提案、执行模型，banked/legacy depth512 rr各12/12通过；原depth3 rr失败原始日志保留，未重跑。模型仅代理DMA/DDR，v1待拒绝构建已整体移至 rr-rejected-v1-builds 并保存manifest/迁移映射。当前两v2 FPGA完整构建运行，最终时序/面积与各15板测待完成；正常编译日志不巡查。创新与收益未建立，整缓冲是已知基线。


### N v2完整编译失败：深FIFO资源超限，继续存储实现修复

两版均exit1，Place 30-487/30-99布局失败；combined LUT S207069/N207226 >203800，total LUT S237003/N237306，控制集11563；FF S203821/N203949在407600容量内，但打包受控集和LUT约束。按用户偏好只在失败后读取相关错误尾部，未巡查正常日志。当前模型512深度已通过，物理实现未接受、未上板。Codex正在审查深缓冲RAM映射与复位/握手等价性，保留4KiB工作量边界，不缩减容量碰运气；Linux/SD继续沿用。


### N v3局部RAM修复开始（待验证）

v2两版布局失败，未产出接受BIT。v3保留DMA0/1各4KiB数据缓冲，复用production common_cells fifo_v3既有AsyncDpRam分支；新增默认关闭FORCE_FPGA_RAM，仅idma_stream_fifo Depth512且type_t8位时启用，其他FIFO/全局FPGA_EN不改变。复位和flush只需保持有效队列语义；无效存储内容不要求相同。Codex负责关键RTL，DeepSeek正在起草实际模块RAM/FF对照加独立数据队列验证；测试、资源、时序、上板尚未验收。失败构建移至out/n-prototype-20261004/placement-failed-v2-builds，无Linux/SD变更。


### N v3实际FIFO存储验证通过，两版BIT编译中

DeepSeek起草与修订实际production模块验证；Codex审查、应用并执行。功能9/9通过，454278检查/13150周期，覆盖512满队列、拒绝写入、排空、多轮回绕、并行读写、输出背压、占用时flush/复位后重用、旧3深度路径；有独立软件队列数据判定。首轮strict检查因Verilator4.038删除gen_asic_queue名称误报，未执行功能；后单独执行功能通过，Codex用实际生成mem_q/RAM数组更正名称匹配并确认结构，未重复跑功能。证据out/n-prototype-20261004/fifo-ram-v3-acceptance.json及fifo-ram-v3-functional.log。无效数据存储可不同（记录1次差异），仅有效数据须一致。本验证不是完整DMA/DDR集成证明。两版完整v3编译并行运行，不巡查正常日志，尚未上板。


### N v3两版产物完成，开始Mac板测准备

N初始完整构建exit0；S初始exit1，但impl_1 write_bitstream成功，随后open_run崩溃（工具segfault）。Codex复用原impl_1产物，完成Makefile同样复制和MCS步骤（exit0），没有重新综合；早期恢复命令的环境/板卡路径失败日志保留。两版BIT各11443722B、MCS31472748B。最终已布线报告WNS S+0.177/N+0.145ns，WHS+0.031/+0.038ns，WPWS均+0.062ns，建立/保持/脉宽失败、无时钟、组合回路、未约束内部端点均0；旧外部IO/clock/CDC全面审查仍未完成。资源LUT S173518/N173667，寄存器137822/137952，LUT memory均5639，物理超限已解决。验收serial-v3-build-acceptance.json、hit-v3-build-acceptance.json。两BIT传Mac，无SD/Linux变更；仅原版15项已完成，S/N v3各15项准备运行。


### N v3 S版15项板测技术通过，N版开始

Mac S fresh boot1c4d6878-a1ea-41a5-b472-e8fdcfbad136，Linux00093；15/15completed，退出码均0、problems空、taint_after均0、unexpected_warnings空。短A8与长A8/A16均完成。原始目录out/n-prototype-20261004/nonblock-pilot-20261004-serial-v3-01/（summary.json、每项JSON/UART/helper、完整console/dmesg保留）。不是普遍无死锁或新方法收益证明。按冻结流程继续banked-hit-v3 BIT同15项，不重跑原版或S；N完成后共同分析。


### N v3 N版15/15通过，但未观察到任何快速路径接收

N版15项completed/exit0/problems空/taint0/unexpected_warnings空，已回传out/n-prototype-20261004/nonblock-pilot-20261004-hit-v3-01/。既有原版15+S15+N15共45项汇总完成，analysis-v3-pilot/RESULTS.md与nonblock-pilot-analysis.json，分析器exit0 complete=True quality_issues0。关键限制：全部N样本fastV/A=0，modeword2、normal/accepted非零，因此本轮未验到实际快速接收；不能把S/N吞吐差归因为绕过收益，也不能仅由0断定硬件坏或不存在候选机会。DeepSeek正整理原始证据、源码门控和最小BIT-only诊断草案；Codex直接审查TL PTW机会窗、DC/leaf权限等门控后决策。原Linux00093持续可用，无SD变更，原始数据不重跑、不剔除。


### N v4静态MSI兼容性修正与被动诊断，编译开始

只读设备配置证据已回传 `iommu-platform/out/n-prototype-20261004/nonblock-live-dc-20261004/`，解析摘要 `v4-live-context-evidence.json`。DID10/11 msiptp.mode=1，flat MSI被实际平台保留；v3守卫mode0因此排除了这类配置。此证据不代表缓存逐次状态、候选机会或唯一根因。DDTPmode2被支持，不应与MSI mode混淆。Linux默认页表_PAGE_BASE包含USER位，不能保留无证据的U位缺失推断。

Codex直接核查TL/PTW匹配宽度并实现v4：普通输入地址和缓存S1物理页都不在MSI范围时允许flat mode1，保留原权限/稳定/4KiB/Sv39/Bare守卫；MSI范围读写均保守回退，保留模式回退。被动diagnostic位/计数另占index16..31，不借用旧error15。TL与depth512交错模型exit0；DeepSeek新增真实hit模块MSI测试由Codex应用执行，结果pass_cases7/reject10/accepts8/responses8（含背压），touch均等于实际接收。

两版v4 BIT完整编译启动，旧v3stage迁移到v3-accepted-builds并保留清单/映射，正常日志不巡查。诊断软件和observer新bank验证由DeepSeek修订中。无Linux/SD更新；编译验收后继续S/N各15新样本，不复跑旧样本。


### v4工具与主机检查完成，等待BIT

DeepSeek修订诊断读取工具、observer测试、分析器及规格；Codex审查并应用，diag validator21/21、analyzer12/12、observerMode1/2均实际通过，静态RV64 reader编译通过。Verilator4.038只在临时测试副本移除已显式连线输入默认值，生产RTL未改；初始失败日志保留。分析器初次10项一失败（可选core_fast_sum字段未校验），DeepSeek有界修正后12项通过。一次完整DeepSeek输出保存超出命令参数长度，从本线程自身失败工具记录恢复已返回内容，未重新调用DeepSeek；后续分块保存。验收v4-host-acceptance.json，v3中文复核analysis-v3-pilot/RESULTS_REVIEW.md。Mac已收到新runner/reader，两BIT仍在编译，无SD需求。


### v4 S编译验收并开始板测

S完整构建exit0，最终WNS+0.124/WHS+0.046/WPWS+0.062ns，建立/保持/脉宽失败、no_clock、未约束内部端点、组合回路均0。LUT174446/FF138414、LUT memory5639，BIT11443722B/MCS31472748B，验收serial-v4-build-acceptance.json；旧IO约束/CDC全面审查仍未完成。Mac已传S v4 BIT并开始经JTAG加载，按原15项计划加diagnostics，输出新目录nonblock-pilot-20261004-serial-v4-01。N编译仍运行。没有重跑旧样本或更新Linux/SD。


### v4 N编译验收完成，等待S板测结束

N完整构建exit0，最终WNS+0.177/WHS+0.039/WPWS+0.062ns，建立/保持/脉宽失败、no_clock、未约束内部端点、组合回路均0。LUT176448/FF141197，BIT11443722B/MCS31472748B，hit-v4-build-acceptance.json。两版BIT均已传Mac；S v4运行中，N待切换。不检查正常编译日志，旧IO/clock/CDC全面审查未完成，不称全面时序签核。无Linux/SD更新。


### v4 S 15/15通过，N开始

S run472f842634e87ede，实际板卡Linux00093/boot a3ad4197-c397-4022-be40-2f89ea98b5f7，15/15completed，exit0/problems空/taint0/unexpected_warnings空。原始目录out/n-prototype-20261004/nonblock-pilot-20261004-serial-v4-01/已回传Dell，第二诊断bank有效，mode1 fast均0；longA8/A16记录到基础PTW活动，probe/fast0。S尚不能代替N的路径触发证据。N v4已按同一15项+diagnostics流程开始，结束后统一分析；无旧样本重跑及Linux/SD更新。


### v4 S/N 30/30完成；没有候选，进入v5受限入口资格

N runc7cfe91d1faec458、boot191c68af-774c-42be-a0a0-3ddd6065f8a1、Linux00093，15/15completed/exit0/problems空/taint0/unexpected_warnings空；S同样15/15。全部raw回传，analysis-v4-pilot汇总exit0 completeTrue quality_issues0。N15项enable seenbit15均出现，candidate bit2从未出现，probe/DC/LEAF/fast全0；A8部分seen8003，A16seen8001。限定为本冻结工作量未提供可探测候选，不是零缓存命中/全部排队根因证明。实际guard控制新AR从bank FIFO进入REQ_TRANS，另一W owner未填完时阻止其选取；demux又锁住跨bank AW。源码与单任务完整缓冲边界已由Codex直接核查，DeepSeek证据在deepseek-v4-candidate-evidence.json。

v5实现中：新增idma_fullbuffer_certificate，采样frontend job FIFO接收与backend job完成，只有容量>=4096且当前唯一完整对齐4KiB DEFAULT copy授新权限；重叠/无任务完成异常关闭权限直到reset。经dma_core_wrap→peripherals→IOMMU bank mask传递，仅对应DID10/11的AR可越过旧守卫；非资格/其他DID保留旧规则，AW优先/既有VALID不撤回。COUNT→WAIT前端实际串行单任务是依赖论证边界，不外推一般AXI/流水多任务无死锁。关键RTL由Codex实现，DeepSeek起草真实模块资格与联合测试，v5尚未验证/编译。overlay新增3路径（iDMA Bender/新模块/wrapper），旧overlay-v4保留。Benz iDMA论文P4已讨论整份传输缓冲，不能作为新颖性；该文不证明本平台入口修正的安全性。无Linux/SD需求。


### v5 首批验证与 v4 报告落盘（继续执行）

DeepSeek修订测试由Codex应用；真实资格模块8类边界实际exit0，coupled RR/depth512/overlap资格开启12/12通过，旧守卫对照12/12通过。独立banked_ctl测试从旧首项出现completion AW地址/ID不匹配，2032 checks失败；原始 `out/n-prototype-20261004/v5-banked-ctl-failure.log`，尚未判定生产RTL根因。已交DeepSeek检查测试接口及补充联合模型的实际候选/探测/接纳观测，Codex保留诊断与验收。尚未启动v5 FPGA编译。v4报告完成重点口径修正并保存 `analysis-v4-pilot/RESULTS_REVIEW.md`：S/N不同BIT物理布局、旧original基线复用、同窗等字节数不证明利用率。无新板测、无Linux/SD更新。


### v5 主机验收通过，两版 FPGA 编译已启动

控制器旧首错具有复位后首拍phantom completion形态；保留旧对象与原始失败，添加DeepSeek首错定位提案后干净对象重建11/11 exit0，包括资格DID10放行与DID12阻挡。未证明旧模型/缓存异常具体来源，不改生产RTL来掩盖错误。新增真实nonblock_fire/busy观测后proxy12/12通过，Mode1fire全0、Mode2 A8(l1/l31)各fire78/busy234；统计覆盖scenario warmup+任务+drain，不能混同P4M硬件窗口。资格模型耗时94003/127222，相对旧守卫76451/109751未改善。v5-host-acceptance.json保存实际验收范围；v5两套完整FPGA构建启动，日志fpga-serial-v5.log/fpga-hit-v5.log仅失败时读。v4成功构建迁入v4-accepted-builds，RELOCATION.json与manifest保留。Linux00093/reader/SD继续不变，待构建实际通过后S/N各15新板测，原版不重放。


### v5 首次 FPGA 编译：作用域错误，修订中

两套完整构建初次exit1，Vivado Synth8-36：ariane_peripherals_xilinx.sv:1395 fullbuffer_jobs未声明。Codex新增vector在gen_dma内部声明(:920)，IOMMU实例位于外部作用域。关键原始报错在fpga-serial-v5.log/fpga-hit-v5.log，未有可接受v5 BIT，未上板。Codex直接审查确认作用域原因，已交DeepSeek起草共享声明+非DMA置零最小语法修订。资格规则/既有主机验证范围不变。修正后计划复用已有失败源码阶段继续make，保留初次失败记录并明确恢复构建，避免再次全量Git阶段拷贝；最终源修正与恢复验收另记，不能将初次构建改写为exit0。无Linux/SD操作。


### v5 scope恢复构建进行中

DeepSeek共享声明/非DMA置零最小patch由Codex应用并检查，`v5-shared-scope-fix.patch`留存。两套已失败fpga-src仅替换这一个修正源码，首次build-inputs/failed manifest各自另存，现build-inputs overlay该文件记录更新并附scope_recovery说明。首次直接make环境缺RISCV立即exit2，报错保存fpga-*-v5-recovery-env-failed.log；改用平台原ENV（不输出local配置）后两套实际make恢复编译已启动，日志fpga-*-v5-scope-recovery.log。复用已生成IP，不再全量拷贝源码阶段。资格模块/协议规则无再变更，无重复主机测试、无板测/SD操作。


### v5 FPGA源码清单遗漏：补齐中

scope恢复两版exit1，真实主错Synth8-439：idma_fullbuffer_certificate未出现在FPGA生成add_sources.tcl；Bender.yml已添加，但cva6/Makefile显式src列表未添加。之后DEFAULT解析/外围条件错误暂视为可能连带，实际idma_pkg.sv已有DEFAULT=2b00且旧前端使用，不修改枚举。Codex直接检查Makefile与实际清单，DeepSeek起草最小清单修订；将同步overlay新增Makefile路径并仅更新既有阶段该文件，保留scope恢复失败日志。无可接受v5 BIT/无板测，Linux/SD未动。


### v5 FPGA源清单修正应用，两版再次恢复编译

DeepSeek只追加cva6/Makefile的certificate源条目，Codex实际审查应用；`v5-fpga-source-list-fix.patch`与原Makefile保留。overlay新增cva6/Makefile，总18路径，旧v5-before-source-list JSON保留。两套既有阶段仅同步修正Makefile，build-inputs记录source_list_recovery且新增该源条目的provenance。实际make fpga重新生成add_sources，两版恢复构建已启动，日志fpga-serial-v5-source-recovery.log/fpga-hit-v5-source-recovery.log。前序scope错误/环境缺失/缺模块记录均保留；DEFAULT未改，尚未证明连带解析错误是否已消失。无主机协议逻辑再改、无v5板测、无Linux/SD操作。


### 用户收窄停止条件（当前v5构建仍在运行）

用户要求：“这轮实验完成后,分析结果后暂时停止”。从此覆盖此前持续循环至SD的要求：完成当前v5 S/N两套冻结板测与结果分析、必要交接更新后停止，不进入下一轮新方案/代码/构建。当前尚待两套恢复构建返回；无v5板测结果。


### v5 两版恢复构建完成，S板测启动

最终source-recovery构建S/N均exit0。原先scope/module清单问题修正后实际整机编译通过，DEFAULT枚举不需更改。最终报告一次验收：S WNS+.177/WHS+.035ns，N WNS+.177/WHS+.038ns，WPWS均+.062ns；setup/hold/pulse失败0，no_clock/internal unconstrained/loops0。S/N LUT174413/176476，FF138384/141170，LUTmemory均5639；BIT/MCS非空。旧外部IO延迟/时钟CDC审查仍未完成，不称全面时序签核。实际验收保存serial-v5-build-acceptance.json/hit-v5-build-acceptance.json及专门*-v5-recovery manifest；平台首次失败manifest保留，不把首次失败改写为成功。

S v5 BIT已传Mac，S冻结15项新板测已通过SSH启动，沿用v4脚本/reader、诊断ABI、Linux00093；N下一步在S结束后执行。无Linux/SD更新。按最新用户要求，两套结果分析/文档更新完成后停止，不启动下一轮。


### v5 S15完成，N15执行中；昨夜进展报告落盘

S v5 actual runner exit0，summary completed15/failed0/statuscomplete/unexpected_warnings空，run c76ccc3fac1ff68a、finished_at2026-10-04T06:55:56Z；boot d103c6f7-b74a-487b-adac-20b46e0ebbd2，previousNv4 191c68af-774c-42be-a0a0-3ddd6065f8a1，Linux00093/boottaint0。整目录nonblock-pilot-20261004-serial-v5-01已回传Dell，不重跑。N BIT传输实际exit0后、S runner退出释放UART后，N v5冻结15项SSH/JTAG已启动，目录nonblock-pilot-20261004-hit-v5-01。N完成后统一离线汇总再分析并停止，无下一轮代码。

用户询问昨夜以来做了什么，DeepSeek证据整理和草案由Codex审阅、修正v3候选记录范围/v0不合格计数/启动停止vs数据错误/复用原版次数/4KiB搬运与计时窗口/真实N验收时间；报告docs/research-overnight-progress-20261004.md已落盘并提供聊天表格。报告先为快照（75已完成原版/v3/v4板测，v5尚待），后补当前S完成/N进行中，不把旧基线重复计数。


### 2026-10-04 v5 S/N30/30完成、分析验收后按用户要求暂停

v5 S/N两版各15/15完成，runner退出0，全部原始数据已回传Dell；既有分析器退出0、complete=True、quality_issues=0。无新增数据错误/内核警告，逐项taint_after=0。长组每条件3次；N的快速处理计数A8每轮6144/6146/6146、A16每轮8193，Solo/A1及S版均0。N/S速度中位变化A8+4.15%、A16−2.69%；N相对原版A8−14.52%、A16−22.09%。A8三轮104.263/92.987/93.048 MiB/s的波动全部保留。机制已实际触发，但尚不能宣布性能改善成功；不同BIT/新启动、原版结构与缓冲差异限制因果归因，n=3仅描述。

N run e7497b8802cb9e58，finished_at2026-10-04T07:10:06Z（JST16:10:06）；boot c2bd10b5-26e7-4de6-ac4f-d3a04c14498c，Linux6.5.0-rc2-00093-gc0f5e13ea81f。S run c76ccc3fac1ff68a，finished_at06:55:56Z（JST15:55:56），boot d103c6f7-b74a-487b-adac-20b46e0ebbd2。两runner及原始SCP均实际exit0；N退出后UART释放。原版06:07完成的15项只复用，不重跑；累计original15+v3两版30+v4两版30+v5两版30=105完整功能通过，v0部分/启动停止、v1/v2模型不计入。

DeepSeek起草结果与昨夜进展说明并做有界修订；Codex直接核对原始A8/A16 record.json计时窗口、helper.json计数及实际工具返回，审阅限定结论后落盘。[昨夜进展](iommu-platform/docs/research-overnight-progress-20261004.md)、[v5复核报告](iommu-platform/out/n-prototype-20261004/analysis-v5-pilot/RESULTS_REVIEW.md)、[离线汇总](iommu-platform/out/n-prototype-20261004/analysis-v5-pilot/RESULTS.md)、v5-board-analysis-acceptance.json。旧HPM/miss/PTW不作本分bank结构的有效指标；fast是实际接收请求数，不是命中率。完整缓冲/命中先服务为已知原则，本轮不宣称新方法贡献。

初次scope/source-list/ENV失败保留，最终S/N恢复构建exit0；WNS均+.177ns、WHS+.035/+.038ns，建立/保持/脉宽失败0。旧IO/CDC全面复核仍未完成。构建验收时间不等于板测完成时间；build-state/N board字段已按真实完成事实更新。Linux/SD无需操作；按最新用户要求本轮结果分析与交接后暂停，不开展下一轮方案、代码、编译或实验。

昨夜总进展说明已补全前半夜method88（02:42 JST启动、88/88完成；四格强基线均负）与后续N原型105完整通过项，两条线分开解释；不将193项目总数写成同条件重复次数。方法M的详细既有报告在iommu-platform/out/method88-execution-20261004/RESULTS.md；当前仅补文档，未重复原始分析或上板实验。首次补充调用因不存在的analysis/RESULTS.md路径被桥接器发送前拒绝，定位实际report.zh.txt后仅作一次有目的修复，返回草案已审阅。

Mac最终MAC_HANDOFF.md已实际更新，昨夜两线总进展说明与v5两份MD已复制到Mac（命令均exit0）；无需解包或回读。当前v5板测、结果分析、Dell/Mac文档交接均完成，已按用户要求暂停。


## 2026-10-04 论文主线五轮复审启动（用户解除暂停）

用户授权Codex/DeepSeek五轮交叉、必要时新路线、实现并完成BIT与Linux编译，愿意更新SD。前四轮独立/交叉攻击/重构/审稿已完成并保留双方原文于iommu-platform/out/paper-route-review-20261004/。文献核查否定把分区/命中绕行/固定限流本身当创新；主候选收敛为同BIT容量保护×命中先服务的有界可行性验证，P主基线2/14、后备失效路线本批不实施。第5轮正在关闭接口/静止切换/计数/公平性问题；尚无本批代码或编译产物，不称已实现。旧v5结果和method88否定结论保留，不重跑。


## 2026-10-04 composition-v1：五轮完成、软件与测试完成，BIT恢复中

五轮独立/交叉/重构/审稿/收敛已完成，双方原文保存在 `iommu-platform/out/paper-route-review-20261004/`。本批方案已冻结（docs/composition-v1-spec-20261004.md），RTL/驱动/CLI 与 Mac 入口实现、组合仿真与 CLI 合同测试、Linux 全量构建均已完成；本补充只记实质变更与证据，不重述全过程。本次用户已授权 Linux/SD 修改，与上一次“旧暂停”记录明确区分。

实质变更与证据：①新 profile 内四模式共用同一 BIT（与旧版 BIT 区分），4 模式 0=S/1=N/2=P/3=PN，16 条 IOTLB 中 P 固定 low2 保护 DID10、其余 14，保护归属取自稳定 main DID（docs/composition-v1-spec-20261004.md:2–3）；②单 BIT 运行时切换：新 128×u64 raw bank（D20–D9F，index=relative−D20），仅 offset2 可写，写同周期原子配置并清 IOTLB，offset3 为 64 位递增 generation（饱和拒绝不回绕），offset4 last_status、offset5 live quiet 全静止谓词（spec:5–13、38–39）；③新驱动 ABI 独立 ioctl 0x11，旧 0x0d 结构、cache 与 drain 规则不变（spec:40–44）；④V 侧 gap 复用每设备 gap 硬件，只读 gap-active 记录真实等待，不把名义值当服务延迟（spec:24–25、31；五轮 review:31、47）；⑤raw128 覆盖正常/首次 lookup、fast accepts、填充事件与 OPEN/CLOSE 16 条快照（spec:20–37）；⑥CLI `idma_composition` 输出单行 JSON（inputs/P4M/raw128/状态/ABI），Mac 入口冻结 172 用例、固定顺序且不自动重跑（spec:44–47）。

状态与边界：Linux 全量构建 complete、exit_code 0，内核 6.5.0-rc2-cpn1-00093-gc0f5e13ea81f-dirty（out/paper-route-review-20261004/software-build-state.json:17,25，产物含 fw_payload.bin 29363656 B、rootfs.cpio、idma_composition）。FPGA 侧首次构建在布线期间原进程停止、原因未知，现从同一 source 的 physopt DCP 在新独立后台 state path `out/paper-route-review-20261004/recovery-v1/recovery-state.json` 恢复，尚未完成，本文只标“恢复中”，构建产物/时序由 Codex 完成后机械补充（out/paper-route-review-20261004/recovery-v1/REVIEW.md:1）。Mac 侧 tools/composition-v1-20261004 的 README/SPEC/RESEARCH_REVIEW/DELIVERY 已实际 SCP 成功；未写 SD、未加载新 BIT、未跑 172，全系统上板仍待。旧负结果保留：method88 强对照为负（前文），N v5 仅证可触发（A8 三轮 104.263/92.987/93.048）但未建立稳定收益（N/S A8 中位 +4.15%、A16 中位 −2.69%；N 相对原版 A8 −14.52%、A16 −22.09%），创新未成立（五轮 review:76）。

DeepSeek 起草文档、测试及工具；Codex负责关键RTL/驱动、审查、实际执行和构建验收。


### 2026-10-05 用户询问布线耗时：资源与观测开销审查

恢复编译仍运行。日志明确拥堵，00:03 JST冲突数下降至666；不能保证完成时刻/时序通过。新Placed LUT89.80%、Slice50949/50950；新增composition控制观测模块Synthesized5754LUT/4171FF。四mode复用同BIT、旧512拍缓冲未本次扩容。资源阶段不同，未唯一归因或在跑时改RTL；后续若失败优先审查仪器开销与布局。详细证据见 [布线与仪器开销](iommu-platform/docs/composition-routing-cost-20261005.md)。DeepSeek提取、Codex核查。

### 2026-10-05 composition-v1转交另一对话；SD已就绪，未上板

用户确认配套SD已写入并插到FPGA，并授权编译通过后直接上板；随后要求后续交给另一对话。本对话停止后续上板推进，独立后台编译不停止。最后00:44 JST实查恢复PID3064145/3064187在跑，已运行约1h56m，最新Phase4.3冲突194，无routed DCP/BIT/MCS。当前无自动上板任务，接手需读实时状态、一次必要最终验收、传BIT到Mac、加载并按既有runner跑16+12+144=172；全部原始结果回Dell、技术错误停止、不自动重跑、保持Linux。操作/路径/约束汇总见 [直接接手说明](iommu-platform/docs/composition-handoff-20261005.md)。

Mac实际写卡记录results/sd/update-6p5rn8l6/status.json为written_not_readback_verified、ejected=true；release composition-v1-20261004，配套内核cpn1-00093，用户已插板，不需要再写卡。另一个cpn2-net1软件任务不替换此版本。串口最后检查空闲、结果目录不存在。本轮172尚未开始。DeepSeek启动包装器初稿已返回但未接受/安装，精简调用被用户打断；保存未接受提案供需要时参考，不能当已部署自动流程。

### 2026-10-05 01:39 JST：恢复布线结束但失败，未上板

用户再次询问进度，只读实查显示恢复进程运行约2h51m、Vivado日志01:39:28更新；Phase9 Verification failed、Route35-2明确Design is not legally routed、31处node overlaps；Route35-77 Router completed with failures。route_design本身正常返回不代表布线合法；已写routed DCP147646933B，但没有BIT/MCS，进程当时仍保存报告，Tcl最终结果尚未出现。未修改源码、未另起编译、未上板；另一对话接手须读最终state/原始报告处理失败，不能放松门槛使用失败DCP。已同步直接接手说明。此次为Codex必要最终错误核查，没有新增DeepSeek调用。

最终回执01:40:09 JST：recovery-state.json已failed/stopped、exit1、timed_out=false、gates_passed0；recovery-tcl.log明确Negative timing slack WNS=-3.268/WHS+0.042ns。失败布线DCP最终147646979B，无BIT/MCS。本轮失败已确立，不再等待编译结束；另一对话接手资源/布线及实际时序失败诊断后重新构建，不能仅继续原等待或上板。配套SD保留，无需自动改为cpn2或再写卡。没有新增RTL修改/构建/上板。

### 2026-10-05 新版 Linux 已在旧 BIT 启动；以太网测试等待 Mac 网卡识别

用户最新范围仅为 Linux 加载/以太网更新，不运行实验；172 项未启动。复用 Mac 已通过板测的 `tools/nonblock-pilot-20261004/banked-hit-v5.bit`，现有 loader 1 MHz 加载一次成功，SRAM done1。实际 uname 为 `6.5.0-rc2-cpn2-net1-00093-gc0f5e13ea81f-dirty`，boot_id `9d3166a2-1d07-48dc-add5-07d40bae2d28`；这覆盖此前 cpn1 写卡快照，以实际板端证据为准。Xilinx SPI/MMC 驱动识别 `/dev/mmcblk0`、p1/p2，lowRISC eth0 和 RTL8211E PHY 已绑定；`platform-net-up` 192.168.77.2/24 实际 exit0，carrier0。

Mac 用户重新插入 Ethernet USB-A 后，IOUSB 新增扩展坞、鼠标等，但仍无 USB Ethernet 设备；以太网硬件列表无新增网口，等待将网卡换到已识别的鼠标 USB 口。当前未通过 Ethernet 下载 payload、未在板端写 SD、未再次重载 BIT；无实验数据，Linux 保持运行，UART 已释放。初次采集 180 秒超时是观察窗口不足，后续继续采集已证明启动成功；不要为此重复加载。实际 root 登录需密码响应，已复用 `scripts/run_research2_mac.py` 现有登录流程，无口令披露；交付 `mac_update_reboot.py` 的空密码假设不能直接用于该镜像。Homebrew loader 用绝对路径或补 PATH，下一次启动采集使用 600 秒窗口。

新 composition 恢复编译已 failed/exit1/gates0，无 BIT/MCS；实际 timing WNS−3.268ns/WHS+0.042ns，setup失败端点258，route_status 有51条资源冲突线路（31处冲突节点）。未另起编译、未加载失败产物。当前软件加载测试继续用旧已通过 BIT，不自动修 RTL 或启动172项。

完整原始 JTAG/串口/板端输出已回 Dell：[板端实际身份](iommu-platform/out/composition-board-execution-20261005/ethernet-update-20261005-oldbit-02/board-state-existing-login.json)、[当前执行状态](iommu-platform/out/composition-board-execution-20261005/execution-state.json)。DeepSeek 提供执行准备与网络步骤草案，Codex 直接审查、复用现有工具执行并核对实际结果；本次 USB 重插核对和状态记录未新增 DeepSeek 调用。

## 2026-10-05 composition编译失败修复：裁剪旧观测，独立完整构建已启动

用户要求修改未通过编译的RTL并保持代码简洁，随后询问RTL/Linux分工。Codex直接核查失败criticalpath为IOTLB tags_q→旧research event_ram，完成仅composition profile旧research与nonblock观测器裁剪；保留本轮两缓存快照、128字接口、通用/path/P4M观测及全部方法逻辑。旧bank读0、不假造capability；无Linux/ABI/CLI/计划改动，SD不需为此次修复重做。RTL捕获精确硬件时刻，Linux配置/读取/校验，Mac/Dell保存长日志与统计报告。

DeepSeek提出联合测试和独立输出目录/启动包装，Codex审查应用并实际执行：10项联合仿真PASS、4项入口检查PASS（源码边界另确认）；不外推真实AXI/SoC/板测。2026-10-05 02:06 JST独立完整构建已启动，launcher PID3110218，路径`iommu-platform/out/composition-trim-20261005/`、`build-state.json`、`build.log`、`fpga-src/`。尚未宣称编译通过或新资源数字，结束后一次验收实际布线、最终时序与非空BIT/MCS；正常日志不巡查。原失败构建保留，不重复启动当前构建。

另一对话负责旧N v5 BIT下cpn2-net1 Linux加载及网络更新；本次修复不碰UART/JTAG/SD，不自动启动172项。后续接手先看本次state，不以旧recovery失败或历史running判断新编译。[修改及证据](iommu-platform/docs/composition-trim-20261005.md)。

### 2026-10-05 百兆适配进行中（未宣布网络加载通过）

Mac换到鼠标USB口后实际识别USB 10_100 LAN，VID:PID0bda:8152，AppleUserECM/en11；管理员配置192.168.77.1/24成功。双方仍无链路，当前cpn2-net1驱动仅广告1000全双工并拒绝100，用户确认没有千兆网口。DeepSeek提取现有RTL速度检测/MII节拍证据，Codex补读rgmii_soc/eth_mac_1g确认接口传递；只修改Linux驱动，开放100/1000全双工，RTL未改。小模块lowrisc_net100（独立driver名lowrisc-eth100-bootstrap）已编译，31944B，vermagic匹配板端net1；已传Mac，尚未传板/加载。首次模块构建缺modules_prepare已据原错误修正，实际最终exit0。

复用现有软件构建器生成独立out/software-update-net100-20261005/delivery，exit0；目标内核6.5.0-rc2-cpn2-net2-00093-gc0f5e13ea81f-dirty，payload29363656B已传Mac。当前Linux串口随后不回应，换行/XON/中断均无返回，因此一次纠正性重载相同旧BIT（JTAG rc0），UART恢复到BootROM正常SD复制，正在等Linux启动。尚无板端模块加载、Ethernet下载/SD写入/新镜像启动结果，实验0项。原始阶段结果保留，网络验收必须以真实传输/写入/重新启动为准。Mac管理员凭据未保存到项目或交付/日志，不发送DeepSeek。

## 2026-10-05 实际工作区清理：method88代码退役，生成物移出源码

用户指出工作区无用代码过多，要求简化。DeepSeek按当前构建/rootfs/composition/Npilot/E2/网络入口审查依赖，Codex直接核查关键调用及反向引用后归档：method88五个脚本与三个专用测试共4,796行；两目录测试生成代码、三份根目录.orig/.rej残留及退役模块缓存一并移出当前开发目录。归档保持工作区相对路径，不加shim；[可恢复路径清单](platform-archives/retired-code-20261005/manifest.json)。需要复现时移回原路径，归档本身不是独立可执行包，共享模块保留在平台。

仅将composition控制器/nonblock观测器两测试默认构建目录从tests改为out，不改测试逻辑。实际加载composition计划/串口类、E2离线/rootfs模块均成功，活动源码无method88引用，两个测试Python语法与差异格式检查通过；不为此次归档重跑RTL仿真/全构建。当前独立FPGA stage、原始数据、Linux/RTL/22源码组件未改；不碰另一对话板卡。[实际检查](iommu-platform/out/composition-trim-20261005/workspace-cleanup-checks.json)。

2026-10-05 裁剪后同阶段综合资源已确认：LUT172284→152609（−19675/11.42%，84.54%→74.88%），寄存器136576→124728（−11848/8.68%），BRAM48/DSP27不变。两份均Synthesized，不与旧placed183014混算；最终布线/时序与BIT仍待验收。DeepSeek提取计算、Codex核对原报告，[比较及来源](iommu-platform/docs/composition-routing-cost-20261005.md)。

## 2026-10-05 用户要求继续：一次布线后物理优化已启动

裁剪完整构建02:49 JST正常exit0，work-fpga下BIT/MCS非空但WNS−0.380ns、setup失败30，故base-acceptance.json accepted=false，禁止按成功版本上板。全部273890线路合法布通、route errors0，hold+0.052/pulse+0.062均失败0。构建收尾Makefile已将报告/DCP从ariane.runs移至work-fpga；初次DeepSeek输入因此本地拒绝，定向摘录报告后恢复成功。最坏50MHz路径：nonblock hit FSM→quiet/config读出→观测控制，50级/route80.571%；前10违规均同域同起点，非eth_rxck。

Codex决定先不改RTL/约束/频率/ABI，从最终routed DCP一次post-route phys_opt Explore，若产生未布通线则一次route Explore。DeepSeek整理证据并起草Tcl，Codex核实AMD2023.1支持、审查并加完全布线及有限数值门槛，实际Tcl解析原失败报告/loops/route/非有限数字拒绝检查通过。优化PID3230656独立启动，目录`iommu-platform/out/composition-trim-20261005/route-opt/`、execution-state.json、optimize.tcl；未重跑综合/未加生产RTL或Python框架/未动Linux或SD/板卡。最终需工具exit0且result门槛全过：setup/hold/pulse失败0、真实非负slack、loops0、完全布通error0、native BIT DRC与BIT/MCS非空。正常日志不巡查，结束时一次验收；失败保留报告，不无限复跑。原IO/CDC全面审查仍非本次闭合范围。

### 2026-10-05 百兆联网与板端SD更新已完成，等待新镜像启动确认

已通过32KB UART小模块引导100Mbps全双工，insmod/旧driver解绑/新driver绑定/net-up均exit0，carrier1；Mac en11 active/100baseTX/full-duplex。模块执行程序最终exit1来自额外dmesg命令的UART输入溢出，原始日志显示ttyS0 input overrun及未闭合命令的`>`提示；后续实际SSH握手/认证/执行证明Linux未死机。不要把不同串口数量归因于本次丢字，也不要为此继续重载BIT。

直接复用Mac系统SSH与既有板端登录响应，通过Ethernet执行`platform-fetch-update http://192.168.77.1:8000/fw_payload.bin --write`，实际全部29363656B下载完成，工具PASS写33554432B到/dev/mmcblk0p1（image29363656B+padding4190776B）、同步完成，SSHexit0。总执行约24.3分钟，包含传输与写卡；不是百兆线速，不宣布性能优化完成。未哈希/回读或实验。第一次SSH未取得控制tty而空认证失败，SD未动，失败现场保留；修正控制tty后成功。

最新目标net2内核6.5.0-rc2-cpn2-net2-00093-gc0f5e13ea81f-dirty。同步成功后只重载相同旧banked-hit-v5.bit一次（JTAGexit0），当前BootROM从SD启动；最终uname/新boot_id尚待。末次UART发送以4B/10ms分段限速防止已实证输入溢出。本轮SD一直留在FPGA上；Linux开机自动静态IP尚未补，当前每次启动仍需现有UART net-up，不声明已能永久拔UART。新composition BIT原编译失败、172项未跑、未改RTL。

证据：[实际网传写卡回执](iommu-platform/out/composition-board-execution-20261005/ethernet-update-20261005-net100-update-02/update-state.json)，完整SSH/Wget/板端更新输出同目录ssh-update.raw.log；小模块引导和纠正性恢复原始目录均已回Dell，最终启动目录待结果。DeepSeek整理RTL百兆证据、提出串口引导片段并修订；Codex负责驱动速度修改、关键解绑/设备归属审查、实际JTAG/UART/SSH与板端更新执行。

### 2026-10-05 最终：Linux百兆网口更新闭环通过（实验0项）

实际网传29363656B、板端32MiB第一分区写入及同步exit0；同旧N v5 BIT JTAG再加载后，内核cpn2-net2、boot_id f7e78b10-b4a5-44b5-9114-10d0aa21251a，net-up/carrier1，Linux保持运行、UART释放，SD全程未拔。只改Linux速度约束，未改RTL/BootROM或重编BIT，无额外hash/回读/实验。自动开机IP和完全拔UART尚待；实际传输+写卡约24.3分钟，速度未优化。原新composition编译失败、172未启动。详细结果和各阶段失败边界见 [完整结果](iommu-platform/out/composition-board-execution-20261005/LINUX_ETHERNET_STATUS.md)。DeepSeek整理证据/引导片段，Codex修改关键驱动并执行最终验收。

## 2026-10-05 03:06 JST composition优化版最终验收通过，构建阶段完成

单次post-route物理优化02:59:41→03:06:49 JST（7分8秒）工具exit0；Codex已直接核对最终原报告：WNS+0.058ns/WHS+0.052ns/WPWS+0.062ns、setup/hold/pulse失败0；全部273893线路布通、routing errors0；组合/锁存环0。native bitgen与cfgmem完成、DRC0 errors/227 warnings。[最终验收](iommu-platform/out/composition-trim-20261005/acceptance.json) accepted=true，scope限现有约束下构建/时序/布线，不等于板测或完整IO/CDC签核。未再改RTL/50MHz/XDC/ABI，不重跑综合、Linux/SD或主机仿真；之前10场景/4入口PASS保持。

交接BIT唯一使用`/home/xvhaocheng/work/iommu-platform/out/composition-trim-20261005/route-opt/ariane_xilinx.bit`（11443722B）；同目录MCS31472748B、优化DCP133778206B。原base虽工具exit0/有BIT但accepted=false，不能加载。最终routed LUT163673/203800=80.31%、Register133604、occupiedSlice50670/50950=99.45%，仍密集，不与此前synth74.88%混算。原报告和检查点仍在原stage/work-fpga。

当前另一对话负责Linux/网络加载验收；本任务未上传Mac、未碰UART/JTAG/SD或启动172项。Mac可按现有流程交接新BIT，冻结172规格不变；新BIT板测仍待板端对话按当前范围安排。DeepSeek整理证据、起草Tcl/交接，Codex负责关键RTL/诊断/实际执行及验收。[最终修复说明](iommu-platform/docs/composition-trim-20261005.md)，MAC_FPGA_HANDOFF已更新，原始数据和工作区清理归档保留。

### 2026-10-05 net3

- 软件构建 exit0，内核 `6.5.0-rc2-cpn2-net3-00093-gc0f5e13ea81f-dirty`；builder 只做软件增量，不改 RTL/BootROM。

- rootfs：`etc/network/interfaces` 静态 `192.168.77.2/24` 无 gateway，`etc/dhcpcd.conf` 加 `denyinterfaces eth0`，复用 `S40network`；无新增服务。

- 操作说明统一进 `docs/software-update-20261004.md`（追加 net3 小节），不新增其它 README。

- 路径：Mac helper `scripts/mac_update_reboot.py`；交付 `artifacts/software/software-update-autonet-20261005/delivery/`；BIT `tools/nonblock-pilot-20261004/banked-hit-v5.bit`；输出 `results/ethernet-workflow-20261005-03`（每次新目录，不覆盖）。

- 现状：net3 已实板通过（`通过（写卡、同步、JTAG重载、开机自动联网及SSH身份验收exit0）` / `1a955cce-bcdd-4f0a-9012-6e0c7eda8322`）；net2 此前已整链路通过（约 24.34 分钟）；科学实验 0，不改 RTL/BootROM。

主操作说明：[net3固化入口](iommu-platform/docs/software-update-20261004.md)；实板结果：[入口原始记录](iommu-platform/out/ethernet-workflow-20261005/ethernet-workflow-20261005-03/result.json)。DeepSeek起草，Codex审查修正并实际执行；入口exit0、科学实验0项，Linux保持运行。


## 2026-10-05 新 BIT 上板实验重新授权与准备

用户最新要求等待重新编译的新BIT完成后用新BIT上板，重新授权既定172项（16短＋12噪声＋144探索），覆盖此前本轮仅Linux测试的限制。只读实查Dell无Vivado，最新已接受的composition route-opt为03:06产物；另一个新编译位置尚未确定，已发文本澄清，未加载BIT或启动实验。Mac三个依赖脚本均在、UART存在且无占用。后续复用SSH mac_update_reboot无payload重载，再一次mac_composition --through-phase explore，expected-kernel为已运行cpn2-net3；不重编Linux/写SD，不先短跑再重复全量。

DeepSeek核对现成入口/计划/输出/串口协议；完整提案见`iommu-platform/out/composition-board-net3-20261005/deepseek-preflight.json`。Codex直接核对驱动和RTL，P4M供数来自保留的dma_observer，raw[33]来自真实composition W_FAST_N计数；不因旧observer裁剪修改验收。原始时序摘要与最终路由状态实际对应WNS+0.058、WHS+0.052、WPWS+0.062、273893线路全部布通/错误0。DeepSeek末尾关于任意新BIT都能在ABI门前阻止错误归属的推论不能成立：--abi仅软件字段，BIT来源必须由实际加载日志绑定，未将该推论当作验收。

当前状态见`iommu-platform/out/composition-board-net3-20261005/execution-state.json`，本请求SD写卡0、JTAG0、实验0。


## HANDOFF（2026-10-05 追加）

- 接手时先确认：板卡 `192.168.77.2`、Mac `en11=192.168.77.1`、`uname -r` 为 net3、`boot_id` 是否变化；日常不接 UART，JTAG 保留。
- 需要重载 BIT：用上面 `mac_update_reboot.py --run --transport ssh ...`，不带 `--payload`；这表示只 JTAG 重载、不写 SD。Linux/网口完全失效可能需人工 SD 恢复，不要承诺全自动恢复。
- 需要跑实验：用上面 `mac_composition.py --run --transport ssh ... --through-phase explore ...`；沿用冻结 172，不新增实验框架。本轮 out 已用 `results/composition-routeopt-net3-20261005-01`；下次用新时间戳 fresh dir。
- 回传：沿用 Mac tar over SSH 到 Dell `out/composition-board-net3-20261005`；不做 payload hash、SD 回读、再短跑全量、完整重编。
- 失败：技术失败保留后停，不自动重跑；科学阴性不是技术失败。
- 沿用现有Mac登录脚本内存AST读取，ControlMaster auto/ControlPersist60，关闭自身连接并保留原始帧。
- 责任：DeepSeek 起草 SSH/tests/docs；Codex 负责关键退出竞态、架构决定、源码审查、工具执行与实板验收。

当前172已实际进入探索；前28项技术通过。精确入口及来源见`iommu-platform/docs/composition-v1-mac-20261004.md`，现场与证据见`iommu-platform/out/composition-board-net3-20261005/execution-state.json`。


## 2026-10-05 完整172修复任务进行中：N卡死定位、net4trace正在网口更新

用户最新授权“完成完整的实验，代码有错误就改正、编译、上板”，覆盖前轮停止后“不自动重跑”的历史范围。原172仅52通过/1失败/119未运行，保持原始结果；新鲜启动PN单项也失败。随后同BIT同gap1000/A8对照S通过、N超时，P/PN未继续，UART证据见 `iommu-platform/out/composition-fix-20261005/compare01.tar`。不能从缺失boot_id推断重启。

Codex直接审查关键RTL，未接受DeepSeek原报告中两个未经合法握手证明的死锁推论；当前没有功能RTL修复。DeepSeek起草既有仿真扩展与可选阶段日志，Codex修正补丁上下文、审查应用并执行：N长仿真V4096/A4096通过；持续A版本V4096/A8189通过（7110130cycles、17fast接受），仍是简化DMA/DDR模型，未复现真实板端。Linux新增默认false的composition_trace，仅打印进程上下文阶段；中断关闭区不打印、不改MMIO/IRQ顺序/ABI。net4trace增量构建exit0，payload29363656B；批启动现有10个场景检查通过。

同一route-opt BIT重载后net3恢复，boot_id89e50509-ca3e-4788-9850-c546b295bde0、taint0/carrier1。当前Mac `results/composition-fix-20261005-net4trace-update/` 正运行既有更新入口：网传、仅SD32MiB p1写入/同步、同BITJTAG启动、SSH核对新kernel。预计版本 `6.5.0-rc2-cpn2-net4trace-00093-gc0f5e13ea81f-dirty`。更新完成后开启trace，仅复现失败N项，UART定位最后阶段；关trace后新鲜正式运行172。若需RTL修复，重用原stage全部20项overlay清单，最终时序/布线通过再部署。状态与证据在 `iommu-platform/out/composition-fix-20261005/execution-state.json`；并未宣称任务完成。


## 2026-10-05：完整172项修复仍在定位（当前阶段）

用户授权修正错误、编译上板并完成完整172项。原正式轮保留52通过/1失败/119未运行；所有诊断项独立记录，不补入原轮。net4trace已压缩网传、成功解压后只写SD 32MiB p1并同步一次，沿用原accepted route-opt BIT。N64通过、N1024/4096卡死；eth0关闭后仍复现。UART最后stage=pair-enter，不能据此断定具体阻塞指令。最新同BIT恢复boot_id640fbad1-3f22-4794-8243-01fe23c2b68c，kernel精确net4trace。

用户提示旧IOMMU修复：7fbb2b9 AW先于AR防大传输死锁、6cc8ed3双DMA MMU侧带；Codex核对当前与accepted构建快照，修改均保留，实际wrapper/macros/parent extended type路径也仍在。症状相似不等于同根因，相关证据在out/composition-fix-20261005/history-*.patch及deepseek-history-review.md。

DeepSeek起草真实iDMA联合诊断框架及依赖修订，Codex审查并修正test-only配置主机AW/W独立握手、补编译器点名的两模块依赖，未改功能RTL。真实dma_core_wrap/512深RAM buffer/certificate/AXImux/双bank/TL：S64、N64通过；N4096/PTWlat100通过（7828985 cycles、37156请求/响应、fast586全bank0）。该框架warm串行、尚无真实系统xbar/CPU/MIG，不代表板端故障消失。正在由DeepSeek起草有界sharedDDR系统诊断补丁，Codex负责关键连接、验证及最终诊断。尚无功能RTL修复或新FPGA构建；仍需修复后fresh完整172。详细事实与日志入口见iommu-platform/out/composition-fix-20261005/execution-state.json。


## 2026-10-05 13:29 JST：卡死现场观测准备，诊断 BIT 编译中

- 原正式172仍为52通过/1失败/119未运行；根因未确认，无功能RTL修复。本阶段不写SD、不重载原BIT；net4trace及rescue03启动状态沿用。
- 真实iDMA＋共享DDR系统模型加入与Linux一致的并发预热后，N64/N4096均通过；warm V256/A257，实际重叠605624cycles，N4096 fast bank0=3242/bank1=1。模型仍缺真实CPU/MIG/板级时钟转换，不能替代板测。证据：out/composition-fix-20261005/system-dma-overlap-*.log。
- Codex添加仅IOMMU_HANG_DIAG=1启用的USER4只读快照：1024位CPU/COMP/页表端口/DDR/配置总线与DMA状态，1088位含头帧；不回馈功能逻辑。跨时钟request/ack与冻结数据实际XSim测试通过（snapshot-cdc-sim/run-local-socket.log）。
- 诊断构建独立于accepted BIT，目录out/composition-hangdiag-20261005；最初PATH缺Vivado在综合前退出，随后复用已准备stage继续同一次make，没有重新staging。综合中发现XDC不支持if：工作源改为run.tcl按诊断开关加载独立hang_snapshot.xdc；当前诊断stage将同样约束展开到既有ariane.xdc，无功能RTL变化、不重启综合。必须在实现时确认约束实际生效，再验收最终时序/布线，当前尚无可上板新BIT。
- Vivado经XVC枚举未成功；openFPGALoader原生XVC客户端识别成功。Codex定位自定义读取协议需拆分TAP entry/data/exit/idle；DeepSeek整理定向修订，Codex应用并实际验证ID=0x43651093（低28位符合设备）、1088位BYPASS回环逐位通过。scripts/read_hang_snapshot.py可用；尚未验收USER4实际帧。XVC服务已停止释放JTAG。
- 下一步复用Mac现成helper加载通过时序的诊断BIT，先正常快照，再运行单项N4096抓卡死现场；根据证据修功能错误，诊断关闭后编译最终BIT并fresh完整172。详细机器状态及证据在out/composition-fix-20261005/execution-state.json与out/composition-hangdiag-20261005/execution-state.json。DeepSeek负责测试/读取/解码辅助提案，Codex负责关键RTL、诊断、工具执行和实测验收。


## 2026-10-05 14:20 JST：诊断BIT上板成功，N4096卡死硬件快照已回传

只读诊断BIT完整构建exit0；WNS+0.177/WHS+0.034/WPWS+0.062ns，278583线路全通、错误0。Mac复用既有helper，JTAG一次exit0、SD写0；boot_id2458d16b-87e3-4cab-af96-427a01671f9f，实际net4trace内核。原正式172仍52通过/1失败/119未运行，不与诊断合并。

正常USER4三帧验证通过；诊断N4096再次卡死，runner退出1、UART最后pair-enter。卡死快照seq5/6/7可信：时钟前进、CPU/DDR AR及RLAST计数均停滞，COMP WVALID1/WREADY0，DDR入口AWVALID1/AWREADY0（80038fa8）、WVALID0/WREADY1；PTWactive0。CPU最后已接纳AR80003750只表示总线地址，不等同程序计数器。没有断定具体堵塞指令或根因。

Codex直接核查DDR入口后实际串联axi_riscv_atomics_wrap（MAX_WRITE_TXNS1）→clock_converter→MIG，而既有系统仿真缺原子层/CPU写。下一步有界补入并观察握手依赖，未作功能RTL修复。DeepSeek整理原子层状态证据与测试接口，Codex负责关键诊断。板卡保持卡死、XVC服务/tunnel在，UART已释放；重新上板前必须先停止XVC。

证据：iommu-platform/out/composition-hangdiag-20261005/{acceptance.json,n4096-hang-snapshot.json,n4096-hang-decoded.json,mac-records/diagnostic-board.tar,execution-state.json}。早期boot01调用错误未触发JTAG/SD；成功记录boot02，诊断记录n01。


### 2026-10-05 14:34 JST：真实DDR原子层接入模型后仍未复现

Codex已在测试顶层按实际64位数据/6位ID/MAX_WRITE_TXNS1接入原始axi_riscv_atomics（90个端口逐字段连接），默认旧模型仍可用，独立obj-system-atomics。N4096/PTWlat100通过：12857702cycles、CPU读完成31004、TL请求/响应37168、fast bank0=6368/bank1=1；预热重叠696529cycles。原始证据out/composition-fix-20261005/system-atomics-n4096.log，不能据此推断板端已修复。

DeepSeek已交原子层源映射（deepseek-atomics-map.md）。其CPU写测试第一提案未应用：Codex发现W-first把AWVALID依赖W握手，会自行死锁；并且64次流量过早结束，响应捕获需覆盖最后请求同拍。已发定向修订，要求固定拍独立延迟/持续覆盖并发窗口/完整B+R及内存分数板。该缺陷属于未应用的测试提案，不是已确认的生产根因。生产功能RTL未改，正式172未重跑，板卡继续保留卡死现场。


## 2026-10-05 14:47 JST：CPU读写/原子并发模型通过，第二版只读快照构建中

DeepSeek第二轮修订的测试提案经Codex直接审查后应用（仅归一化一处补丁上下文尾空格，git apply --recount --unidiff-zero）。新增合法AW/W独立固定延迟、持续普通写/AMO、独立读写看门狗、按ID捕获、AMO旧值与内存终值分数板；生产RTL未修。N4096/PTWlat100通过：13576802cycles，CPU普通写与AMO各23039完成/无缺失、读24063完成，三路错误0，fast bank0=5632/bank1=1，AMO终值d00006665431d988。仍未复现板端，原日志system-cpu-writes-n4096.log；不用重复旧模型。

Codex因此增加v2被动快照，frame仍1088位，版本字段2；words0..23与v1相同，24..27改为atomic后/clockconverter前dram（SoC时钟），28AMOS状态/计数，29DDR xbar Wowner队列/握手，30当前DDR AW元数据和LRSC状态/内部W握手，31LRSC保存的AW地址。权威是ariane_xilinx.sv新增赋值；以空闲/有效状态分别解释。核心CDC握手和宽度不改，只新增FrameVersion参数。Vivado极小跨层读取综合验证通过，无功能通路反馈。v1源码保存在out/composition-hangdiag2-20261005/v1-*，旧stage/BIT与现场均保留。

新独立构建out/composition-hangdiag2-20261005/fpga-src，exec session72780，IOMMU_HANG_DIAG1，同23文件overlay，PATH显式Vivado2023.1。此时正在IP阶段，未验收BIT。DeepSeek session55945正在更新reader/decoder兼容两版，尚未应用。Mac恢复脚本已准备在该目录mac-rescue-prepared.py并py_compile通过，尚未执行：复用原helper，对已卡死的板明确记录before来自上次确认boot2458d16b，并跳过无法执行的sync（本次只RAM诊断，SD无写）；新BIT必须验收后才能传/加载。XVC session52203仍存活，加载前须先发送换行停止释放JTAG；UART已释放。正式172仍52通过/1失败/119未运行，根因未确认。


### 2026-10-05 14:54 JST：v2读取/解码工具就绪

DeepSeek工具提案已由Codex应用并直接核对位布局：reader增加--expected-version 1/2，decoder保留真实v1兼容并解释v2原子层/写归属字段。6项定向离线检查通过，含真实v1重放、v2合成位字段、空队列归属无效、篡改拒绝及未知版本拒绝；日志out/composition-hangdiag2-20261005/tools-v2-test.log。v2合成帧不是板测证据，真实v2读取仍待本次BIT完成并上板。构建session72780仍在运行，未重复构建；恢复与单项N4096复现命令已准备，未动卡死板卡。


### 2026-10-05 15:24 JST：v2基础布线完成但时序拒收，单次布线后优化中

v2基础构建275176/275176线路布通、routing errors0，但最终WNS−0.123ns/建立失败9，WHS+0.030ns、WPWS+0.062ns；未上板。base-acceptance.json与原始三报告已保存。Codex复用先前已验收的composition-trim route-opt/optimize.tcl，仅更换输入/输出路径，读取稳定副本base-routed.dcp；session47053正在单次phys_opt Explore，只有必要才一次route Explore，保留全部验收门槛。无RTL/频率/约束更改，不重新综合。原生构建session72780可能继续生成未通过BIT及导出，禁止把它当可上板产物；只等待优化版验收。板卡仍保留v1卡死现场、XVC52203占JTAG，SD未重写。


### 2026-10-05 15:29 JST：v2单次优化验收通过，已传Mac并开始恢复上板

route-opt单次物理优化exit0：WNS+0.015ns/WHS+0.030ns/WPWS+0.062ns，三类时序失败0、275177/275177线路布通、routing errors0、组合/锁存环0/no_clock0、原生BIT DRC错误0；BIT11443722B、MCS已生成。acceptance.json、route-opt/result.json及reports为权威；基础构建虽exit0，其WNS−0.123ns版本仍拒收，未上板。仅上传优化BIT到Mac tools/composition-hangdiag2-20261005/ariane_xilinx.bit，上传exit0，无哈希/回读。旧XVC52203已正常退出释放JTAG，恢复runner session17438已启动（mac-rescue-prepared.py，额外sys.exit(rc)透传真实退出码），新结果results/composition-hangdiag2-20261005-boot01，Linux启动待确认。之后重启XVC、用reader --expected-version2取正常帧，再复用准备好的单项N4096 runner（mac-case-prepared.py、fresh n01），非正式172，无SD写。


### 2026-10-05 15:50 JST：v2实板卡死定位到DS写归属，已应用针对性RTL修正，尚未编译修复BIT

v2上板JTAG成功一次，但Codex临时rescue_identity包装器未接受timeout参数，导致初次boot runner17438在JTAG完成后异常exit1。已修正参数透传，continuation84790只继续同次启动身份验证，明确跳过再次JTAG；最终exit0，net4trace、boot5f3c68bd-a175-4d0f-8b68-dee2d824fbd4。三正常帧v2有效。单项N4096 session94371复现pair-enter卡死并exit1，全部Mac记录回传mac-records/diagnostic-board.tar；无SD写，正式172不动。XVC70884仍持有JTAG，UART已释放。

三卡死帧5/6/7均可信，读计数无进展、时钟增量约12M。DDR xbar Wowner FIFO非空、count4、head3=DS；内部Wvalidmask5=CPU0+COMP2，readymask8=仅DS3，而DS外部WVALID0；AMOS全feedthrough/w_cnt1，LRSC W_FORWARD/R_IDLE、savedAW82b0a000；atomic后DDR接口空闲且AW/W/AR ready。可确认队头等待DS写数据，但快照没直接捕获DS内部生产者ID，不单凭地址断定FQ。

Codex核对当前与已构建stage的rv_iommu_ds_if.sv逐字节相同，发现AWID映射CQ0/FQ1/MRIF2/MSI3/PQ4，与原W数组实际索引PQ0/CQ1/FQ2/MRIF3/MSI4错位，并有重复case3。历史63f790a接入PQ时引入，与此前7fbb2b9/6cc8ed3修复不是同一处。已在该唯一RTL文件纠正W数组/重复case，并接入所有权FIFO空满保护；原源码保存在rv_iommu_ds_if.before.sv。修复文件/24项overlay和补丁在out/composition-dsfix-20261005，当前build_started=false，等有界旧失败→新通过测试后再编译。DeepSeek来源梳理已返回并留完整证据（243723tokens）；其中旧W索引解释一处不准确，Codex直接修正于deepseek-ds-source-review.json；测试提案session11176仍待返回。生产PQ的ID/长度等独立旧问题先记录，当前ATS/PRI disabled/PQvalid0，不扩大本轮实验修复。


### 2026-10-05 15:57 JST：修复BIT与针对性仿真并行

为缩短等待，Codex直接审查唯一RTL补丁后已启动修复版独立构建session64970，out/composition-dsfix-20261005/fpga-src，24文件overlay/IOMMU_HANG_DIAG1（保留v2诊断，无Linux/SD修改）。DeepSeek测试初稿144111tokens/truncated=false，已读源码并应用tests/iommu_ds_if_write_tb.sv和tests/test_iommu_ds_if_write.py；两次执行均在命令入口报Invalid option --binary，实际本机Verilator4.038，因此没有旧版失败/新版通过的RTL结果，原失败日志保留test-before.log/test-after.log。已交回DeepSeek session72530定向适配既有--cc --exe --build/C++驱动，并补空队列提前W、满队列背压及精确完成计数/strb检查；未降低worker预算，未安装工具。代码接线静态错误已确认，实板是否修好仍待验收，不重复旧完整模型。decoder仅更正一条v1/v2 header说明文字，不改解码逻辑、不重跑已通过的6项检查。


### 2026-10-05 16:03 JST：已取得旧失败→新通过的实际DS接口回归，修复BIT综合中

DeepSeek测试修订69067tokens/truncated=false已由Codex逐块审查应用。Codex把提前W用例改为CQ0以实际覆盖FIFO复位/陈旧选择0；旧Verilator另报TIMESCALEMOD和缺sc_time_stamp，已机械删除不再使用的timescale、复用已有测试时间戳接口解决，原入口/链接失败日志保留。未更改生产RTL。实际最终命令：旧源码scenario1编译成功、运行watchdog失败，AW握手1/W拍0/B0（进程-6，runner1）；修复源码四场景全PASS/runner0，FQ4拍、五源并发9burst21拍、CQ提前W4拍、队列满12单拍。场景4第10个AW后出现背压，放开W后剩余2AW完成，精确计数/数据/strb/WLAST/B归属/清空均通过。权威targeted-test-acceptance.json、test-before-final.log、test-after-final.log。

已逐字节确认当前已验证DS源码与运行中fpga-src副本一致。构建session64970继续、无重启；尚无可上板修复BIT或板端修复结论。实际FPGA配置InclATS0（ariane_soc_pkg110..124），PQvalid在rv_iommu_top禁用分支绑0；PQ生成器独立ID/长度问题仍不在本轮功能修复范围。实验门槛：新BIT最终时序/布线/原生DRC通过后，停止旧XVC70884并恢复上板，先N4096诊断，成功后再执行冻结172。


## 2026-10-05 16:29 JST：DS 修复 BIT 基础布线完成，继续时序优化

基础结果 WNS −0.428 ns、33 个建立端点失败；WHS +0.040 ns、WPWS +0.062 ns，保持/脉宽失败均 0。275494/275494 线路布通、错误 0，未接受该 BIT、未上板。完整 routed DCP 与报告保存在 `iommu-platform/out/composition-dsfix-20261005/base-routed.dcp` 和 `base-acceptance-reports/`，结论在 `base-acceptance.json`。复用既有 `route-opt/optimize.tcl` 启动一次布线后 Explore 优化（工具会话 52309），保持 RTL、时钟、约束及验收门槛；不重新综合。原生构建会话 64970 当时仍在收尾。四项针对性回归通过结论不变，板端仍为旧 v2 卡死现场，SD 写入 0、新 BIT 加载 0。已复用现有 runner 准备新的单 N4096 与完整 172 入口，均未执行；全量入口关闭 composition_trace，并明确同一启动先有诊断案例。


## 2026-10-05 16:37 JST：修复 BIT 最终验收通过，开始 Mac 上板

`iommu-platform/out/composition-dsfix-20261005/route-opt/` 单次物理优化 exit0；最终 WNS +0.048 ns、WHS +0.040 ns、WPWS +0.062 ns，三类时序失败均0，275498/275498线路布通/错误0，组合环/锁存环/no_clock均0，原生BIT DRC错误0。BIT 11443722 B、MCS31472748 B。`acceptance.json`及原始报告保留；基础BIT仍因−0.428ns拒绝。实际上传接受的route-opt BIT至Mac `tools/composition-dsfix-20261005/ariane_xilinx.bit`，无额外哈希/回读。旧XVC会话70884 stdin已关闭，改为核对Mac PID69456完整参数后SIGINT停止，旧SSH exit255释放JTAG。复用已准备修正签名的rescue脚本启动上板（会话5350，Mac结果`results/composition-dsfix-20261005-boot01`），此时未声明JTAG/启动完成、N4096/172尚未跑、SD写0。


## 2026-10-05 16:46 JST：DS 修复实板 N4096 通过，完整 172 已启动

新BIT JTAG成功、Linux/SSH启动验收exit0，boot_id `7651a76f-6f96-4bd6-8e67-090391a54af1`，net4trace内核一致。单例 `explore-b1-g1000-a8-mn` 完成exit0、1/1通过、无problems、taint0；原始CLI `success=true/errno=0/result.status=0/restore_status=0`，UART已越过原pair-enter阻塞点到窗口完成/排空/恢复。原记录回传 `iommu-platform/out/composition-dsfix-20261005/mac-records/boot-and-n4096.tar`；正常启动及案例后三帧JTAG快照保留。N4096不计入172。复用原runner，在同一验证启动、明确记录已有N4096诊断后，关闭composition_trace，完整冻结172在Mac `results/composition-dsfix-20261005-formal01` 启动（会话94223）；未重启、未重写SD。新XVC会话85467保留只读快照通道。全量结果仍待，不宣称172完成或研究收益。


## 2026-10-05 17:04 JST：DS修复后PN不再全板卡死，暴露PTW合法页误报

修复DS版完整172会话94223 exit1，在原第53项 `explore-b1-g1000-a8-mpn` 停止：52通过/1失败/119未跑。完整原始数据在 `iommu-platform/out/composition-dsfix-20261005/mac-records/formal01-stopped.tar` 及解包目录。CLI EIO/errno5，experiment.status=-5，observer_status=0x4000000004；V4096完成、两路data_errors0，DID11出现10条F15写页故障，iotval为0x3fffe0b000..e000。Linux能返回记录和dmesg，未卡死，UART已释放。不能把data_errors0等同全功能正确。

Codex核对实际构建PTW与工作区相同，发现 `rv_iommu_ptw.sv` STAGE_1 的LVL2后使用独立if(LVL1)/else，合法LVL3和LVL2均被误置ERROR但仍update=1；STAGE_2_INTERMED也误拒LVL3。blame均来自cc0a4844初始代码。TL在PT状态先收ptw_error再允许IOTLB命中覆盖；N probe会屏蔽主命中，解释潜在暴露时序，但仍待定向仿真/板测验证。Codex已修正3处条件（S1改else-if、两处仅非法层级才ERROR），保留非法PTE/权限/对齐检查；补丁和旧源码在 `iommu-platform/out/composition-ptwfix-20261005/`。新25文件overlay构建会话63098运行，测试草案DeepSeek会话27168，故障取证DeepSeek会话94907。部署门为真实旧失败→新通过测试、最终时序/布线/原生BIT DRC，均未提前判过。

DeepSeek流程草案已返回(deepseek-flash，79477 tokens，未截断)，保留dsfix目录JSON/MD，尚未应用，因为完整172未通过。板端仍运行已验收DS版boot7651a76f-6f96-4bd6-8e67-090391a54af1，JTAG XVC85467，后续换BIT需先退出；本次累计JTAG1、SD写0。


## 2026-10-05 17:17 JST：PTW专项旧失败→新通过完成

DeepSeek起草3个PTW测试文件并给定向加强补丁，Codex审查、应用、校正AXI检查仅看握手（等待中的ARVALID合法），实际执行：旧源码合法4KiB读/写均失败，分别error13/15、update1、AR/R=3、pending0；修复源码7场景全通过，合法4KiB/2MiB/1GiB无错误，V0读正确error13/D0写正确error15且不update，两阶段4KiB为15个AR/15个R、无pending/晚到错误。额外16空闲周期观察捕捉旧版先update后ERROR，保留错误路径。既有composition_tl_test也PASS。验收与日志在 `iommu-platform/out/composition-ptwfix-20261005/targeted-test-acceptance.json`。初稿模型编译成功；加强补丁git apply因hunk位置/上下文格式拒绝后，误触发一次初稿旧失败/新通过，已明确标初稿并保存initial日志；随后以逐hunk唯一精确上下文机械应用，再执行加强后的最终验收，未伪称初稿为加强版。

新BIT会话63098继续综合，stage中的PTW字节与修复源一致，不重复构建。DeepSeek全部已完成：PTW测试135863 tokens、修订71764 tokens、PN证据245817 tokens，均deepseek-flash/未截断，完整JSON/MD保留。证据worker中“观测错误使校验提前跳过”及“raw6和P4M状态可推测先后”两条未采信，Codex复核记录在dsfix/deepseek-pn-fault-evidence-review.json；实际driver在观测finish之前执行条件数据比对，失败记录不足以判定是否经过它，composition raw6为自己的FSM。生产修复由Codex完成，测试草案由DeepSeek，实际验证由Codex。Mac新版本的正常sync/JTAG启动、单PN和完整172复用脚本已准备但未运行；当前板卡/旧记录保持。


## 2026-10-05：两处 IOMMU 修复完成，172 项全部通过

- 两处历史源码 bug 非本轮新引入：DS 写源排列错出自 `63f790a`（2025-04-02）；PTW 合法叶误置 ERROR 在 `cc0a4844`（2025-01-20）已有。
- DS-only 版 N4096 通过，但完整 172 第 53 项 PN 报 F15，52/1/119 且无全局卡死，该 DS-only 失败 boot 与 PTW 修复新 boot 不同；PTW 修复旧 4K 读写错误 cause 13/15 → 新 7 场景 pass，既有 TL pass。新 BIT 基础时序失败 WNS -0.384，复用已布线检查点一次 Explore 后 WNS/WHS/WPWS +0.031/+0.019/+0.062、native DRC 0，不降频不改约束。
- PTW 修复 BIT 重新 JTAG 后产生新 boot `2695d848-8589-491e-bf7f-1de7af5ea9cb`；仅新 BIT 的 PN 单项诊断与完整 172 共享该新 boot，完整 172 已全部通过（short16/noise12/explore144），不是重用旧 52 项；records 已回传、UART/JTAG 释放、Linux 保持运行。关键 source 与构建副本一致，SD 本轮 0 写/JTAG 1 次；内核 net4trace。现行 BIT 相对平台 `out/composition-ptwfix-20261005/route-opt/ariane_xilinx.bit`，Mac `tools/composition-ptwfix-20261005/ariane_xilinx.bit`。
- 固定流程见 [composition-v1-mac-20261004.md](iommu-platform/docs/composition-v1-mac-20261004.md)，验收见 [board-acceptance.json](iommu-platform/out/composition-ptwfix-20261005/board-acceptance.json)。`WORKFLOW.md` 已写入 Mac `tools/composition-ptwfix-20261005/WORKFLOW.md`，平台 docs 与根 `MAC_FPGA_HANDOFF.md` 均已应用并更新 172 最终结论。功能验收不等于性能收益或完整 IO/CDC 签核。


## 2026-10-07 推进：共享返回缓冲原型与下一预约边界

- 用户最新“推进”：Codex亲自设计/实现测试专用池，DeepSeek接口证据与普通C++、结果草案；不把创新分析全交给worker。成果从文档变为真实RTL＋完整拷贝，生产与板卡均无动作。
- 新模块tests/nonblock_completion_pool.sv；真实DMA各BufferDepth3，Mode1两个完整响应池槽，Mode2一个共享槽＋精确写段流式，Mode3抑制当前写属主后续段抢池。预约在翻译入口前，一DMA只一未消费完AR。私有端回放避开全局R锁住；AW首次选择须从gen_mux.mst_aw_chan配对w_fifo_push，不能与寄存器后mux_mst_req混配。
- 修正后13唯一完整场景通过：V/A各64，字节/静默/响应守恒；旧热PN93487 FAST62校准。两槽4144B PN103348 FAST31；一槽2096B PN112448 FAST0；改分配125677 FAST0更慢，冷结果见[表](iommu-platform/out/e2-interface-poc-20261007/codex-review.md)。仅数据容量降低74.41%，无面积、性能创新或上板结论；原强walk P92334/PN91727另保留。
- 主线判断：改池分配不能单独恢复N；关键约束是入口预约与一未消费AR绑定。旧首V第二DDR AR+26早于首Rlast+334，一槽+1234/+330，改分配+1238/+334，证明流水明显改变，未做全损失因果分解。下一元数据可先排队，容量在每bank完成AR进入i_out_axi_mux锁定选择前预约；保持私有回放，按具体请求/段约束许可，防止job位授权未预约AR。先FAST与正确进度，再同容量/队列普通精确依赖池与额度对照，等价则不包装新规则。此接口候选未实施。
- 5成功SV构建、1C++诊断重编；初lint关键字/漏gen_mux和首测fault260为Codex新原型错误，修正后通过，原失败日志保留；不要说一次全过或旧生产bug。现5个测试文件变化（4旧含启动器依赖＋1新），76编译生产SV哈希不变，Linux/BIT/SD/板卡无操作；两DMA正常对齐4KiB、单执行job、合成CPU/行为DDR边界，未验证4DMA/错误/取消/任意背压/MIG/面积。
- 完整[审定](iommu-platform/out/e2-interface-poc-20261007/codex-review.md)、[原始9项](iommu-platform/out/e2-interface-poc-20261007/initial-runs.json)、[后续4项](iommu-platform/out/e2-interface-poc-20261007/reserved-runs.json)、[测试补丁](iommu-platform/out/e2-interface-poc-20261007/test-source.patch)、[验收](iommu-platform/out/e2-interface-poc-20261007/acceptance.json)；原脚本已补新模块依赖，不新造运行器、不自动BIT构建。研究候选E2与旧替换残留E2实验编号不同。

最终记录核对已执行退出0：13项原始日志与6类JSON一致、窗口/响应及排空通过，76生产源码哈希无改动/缺失；[验收](iommu-platform/out/e2-interface-poc-20261007/acceptance.json)与[比较](iommu-platform/out/e2-interface-poc-20261007/comparison.json)已落盘。6次DeepSeek均正常未截断，488769 provider tokens（含推理）见model-usage.json；未采用首版过度验证框架。


## 2026-10-07：完成阶段准入与强普通对照，共享池不再作主贡献

- Codex独立实现测试副本完成AR门＋每DMA8项元数据排队、Mode4单槽、Mode5普通两槽、Mode6精确紧邻下一W段流式，最后Mode7同写前缀但无POOL；只修改tests，旧生产controller默认路径不变。原PN热93487精确校准；4/5/6热PN123804/102746/99432，Mode7热P/PN92109/92374、冷92549/92814。V/A窗口均64完整4KiB、字节/响应/终端守恒PASS。48B是活跃DMA payload，不含共同TC FIFO/metadata/control，无综合面积或实板结论；共享池的独立价值被普通无池对照否定。
- Codex发现completion-stage入队让老RA代理每bank一位fast_pending被后续翻译响应覆盖。修perbank8个成功READ标签FIFO，真实READ翻译握手push、对应AR捕获pop，写/错误不入队；标签fault逐拍0/终端queued0。只重编与重跑Mode7 buffer/walk八项，旧13模式buffer本身不依赖tag，因此保留其bytes/cycles/TL FAST，FAST-read计数标unreliable。新热/冷read FAST62/64，Mode7周期精确保持；walk无fast_gated_offers/budget_stall，不能宣称额度生效。全部29次完整执行原始保存，最终矩阵21＝原13＋修标签8；旧Mode7八条分类/额度解释撤销，非29独立有效配置。
- 实际验收脚本exit0，21项六/七类JSON与日志一致、终端池/元数据/标签排空、容量/收发守恒；76生产SV unchanged、missing0。[acceptance.json](iommu-platform/out/completion-reserve-late-20261007/acceptance.json)。六SV模型构建均exit0（0/4/5/6/7/7tagfix）＋1lint。未重复无影响模型，不改生产RTL/Linux/BIT，不写SD或接板。
- 下一主线先核翻译前/后的W顺序不一致是否真实合法可达，普通准确前缀门在多DMA/异步翻译中的界限；若普通方法同资源等价则按工程优化处理。需具体新约束/最小安全回退差异才作为论文候选，不能说48B世界最少、PN已经胜P或新无死锁定理。不自动增加4DMA全矩阵/完整BIT。
- [关键审定/表/前作](iommu-platform/out/completion-reserve-late-20261007/codex-review.md)；[最终21项](iommu-platform/out/completion-reserve-late-20261007/runs.json)；[全部29执行](iommu-platform/out/completion-reserve-late-20261007/runs-all-executions.json)；[比较](iommu-platform/out/completion-reserve-late-20261007/comparison.json)；[补丁](iommu-platform/out/completion-reserve-late-20261007/test-source.patch)。研究候选E2不是旧停止攻击者的替换残留E2实验。DeepSeek四次C++/独立质疑/验收草稿，Codex创新设计、关键源码核验、并发实现/标签诊断、实际执行和最终审定；未调用Claude/新Codex子agent。


## 2026-10-07：二区目标推进，确认多DMA共享bank的完成AR队头边界

- 最新关键结论：[报告§23](iommu-platform/docs/composition-history-isolation-20261007.md#23-二区目标推进共享bank的真实停滞边界与下一机制规格)／[Codex审定与新规格](iommu-platform/out/write-order-boundary-20261007/codex-review.md)。创新仍未成立，二区是目标，不保证分区。
- 排除AW翻转/无停车槽假设：输入demux锁W，TC内部SpillAr1实际双槽；第二AW延迟64/4096cycle都能完整copy。4096的future0是合法覆盖缺失，原C++错判已修，byte/fault/drain检查保留。
- 真实2DMA测试别名到bank0（原ID0/4；生产BankIdBit2的DMA0/2类似共享拓扑）、有限A首AR延后64cycle：Mode7无池48B卡在首次暖场。最新samebank-causal-s：TC状态4、spill3、queue_ids1024=0x400（V0/V1在spill、A0在当前AR FIFO头）、外部AR头V、DDR W头A got0、四AW已备、comp AR累计0、sharedR/PTW/TL空，V watchdog200008cycle。三次复现均原始exit1，非生产/板卡bug或带宽结论。
- 同背压夹具S/warmup2/V64：双bank48B PASS129548/V64-A63；同bank普通8192B PASS127659/64-63；同bank普通源端准确写准备门48B PASS131464/64-64。成功完整字节与排空；不同实际窗口工作量/暖场A，不做净性能排名，P/N未跑。源门是普通强基线，修复不算新算法。
- 新候选尚未实现：每bank非当前义务完成AR只暂存完整头；当前可排空源读跨ID挑选/直接通路不需空槽，同ID有序。先PoC两头＋bypass、H1/2及普通perDMA双槽。候选(K−1)L容量界待证明、不是定理；普通shared-CAM＋bypass同信息/容量等价则不能包装创新。须在捕获原完成AR时附READ类别，不能跨ID重排后仍用原bank标签FIFO；quiet须含头池、四DMA需完整stream ID。后续四DMA两bank/PN/cost与强普通方法，不自动BIT。
- tests仅5源：nonblock_banked_top、actual_dma_top、system_top、completion_pool_late、actual_dma_test.cpp；原clonebankCtl本轮未变。alias时物理W身份从真实AW firstoffer ID与W LAST维护，不能用共用muxport代替DMA；完成AR观测/准入按源DMA映射。普通源门首测代理标签还按ID2 pop，修实际BankIdBit后PASS，属Codex新夹具配置错误。最新清洁诊断在compAR0之前复现停滞，排除该标签错为因。
- [阶段清点](iommu-platform/out/write-order-boundary-20261007/stage-evidence.json)：10成功SV构建、2C++重编，13调用7PASS/3watchdog/1覆盖失败/1标签失败/1CLI拒绝。76生产SV hash unchanged/missing0，无Linux/BIT/SD/板卡操作；before/after、test-source.patch、build/run commands/log/results及DeepSeek完整任务/返回保留。最后只加诊断/显示bank位，successful源门行为逻辑已编译运行。
- DeepSeek两次source证据/验收草案300918 provider tokens，Codex亲自创新设计/关键源/并发实现/诊断/执行与审定；无Claude/新Codex子agent。第一份AW翻转推断缺因果链未采；第二份可选tap写错full OR/层次未采，Codex用实际spill和ID观测。用户要求编译期间待机已遵守，不反复读构建日志。

## 2026-10-07 请求头暂存PoC、强共享三槽对照完成

- 测试专用HeaderParkMode1/shared头＋optional direct、Mode2普通每源固定FIFO已实现。核心并发由Codex设计实现；非生产修改。
- 同bank公共首AR有限延迟64、S/warmup2/V64：H1+direct watchdog；H2/no-direct（Mode1，不是Mode2）watchdog；H2+direct PASS127092，park262/direct2/dispatch264、full_bypass2、HW2；普通perDMA2四槽PASS127147/HW3；补最强已跑普通shared3/no-direct PASS127147/HW3。通过窗均V64/A63，终止排空。
- 正常分bank无夹具warm256：候选/普通每DMA2槽均P92109/PN92374，V/A各64、PN FAST/readFAST62/62，与旧Mode7精确一致。没有新的PN收益；55周期不作速度结论。
- 强容量比较为2+direct vs普通shared3，只有一个头槽差异；不称50%面积或LUT下降。payload48B仍排除公共TC/元数据/控制，新头序号与选择状态未综合。
- FAST标签按源FIFO，Codex已核TL response时ar_data仍为当前读头；新same-ID顺序/VALID稳定/捕获派发守恒/终止排空检查。默认Mode0源级旧路径保留，未单独跑默认等价。
- 7SV构建成功、9调用7PASS+2预期watchdog；两失败无终止标签JSON，七成功标签fault/queued0。76生产SV哈希不变，tests6源快照和差分保留，无Linux/BIT/SD/板卡操作。
- 创新审定：此实现等价普通shared选择队列+同样旁路，停止独立机制创新推荐。通用H=(K−1)L、进度/容量下界未证明。下一先识别真实4DMA/2bank新约束及强普通方法遗漏的保证，不直接把普通组件推广包装为二区。
- [报告/边界](iommu-platform/out/header-park-poc-20261007/README.md)／[清点](iommu-platform/out/header-park-poc-20261007/stage-evidence.json)／[9调用](iommu-platform/out/header-park-poc-20261007/runs.json)。DeepSeek FAST-tag补丁和证据/文档草案，Codex架构、核心实现、关键审查和实际执行验收。3调用205006provider tokens(high/393216/无截断)，完整答复/错误/修订留档，无Claude/新Codexagent。


## 2026-10-09 夜间 §55–57（04:30 JST 接续）

§55 2build/8完整copy PASS，真实R cross-ID交错2–3次，强普通相同提前准备只差33cycles；§56 2build/9完整copy PASS＋旧F1实测watchdog死锁保留。原firstVALID W需求实际R覆盖后即可清future-R membership而不等B；共同R/W切分也解环且F1整体更快，不能把小AR差立新性能机制。详细验收分别在out/shared-read-frontier-closure-20261009和out/shared-read-data-obligation-20261009/codex-result-acceptance.md，后者execution-ledger.json实际pw/pwlast逐拍计数核对9CSV。§54最终16PASS，以其最终验收为准，不以早期记录数量为准。

§57根设计尝试补读接力：A下一WRITE真实成功→V已firstVALID W数据债闭合→真实sink旧physical R→A补读前缀仅A/尾段V+A。未运行，先证heldR及oldW-head两层环，公平强普通/整体耗时。08:00 JST目标活动，不做production/BIT/board/Linux/SD动作。DeepSeek夹具/前作摘录证据，Codex设计/源核/执行验收，无Claude。


## 2026-10-09 §57完整仿真与§58强前作对照（04:50 JST）

§57 out/shared-read-sponsor-handover-20261009/codex-result-acceptance.md/ledger/snapshot accepted-build02：1SVbuild+1C++incremental，28完整copy PASS、run02 requireactual拒绝未触发。root补读接力先闭合V W数据债/偶数J/rawW==J，再sink真R到RLAST，A prefix裁J/joint peer实际W门。F1接力长R平均5906 vs关闭6312，V慢/A快；普通K16接力5741 vs共同6153.5但R320vs256。PTE1000接力平均更差，不能无条件接力或称已证独立新算法。新增11功能位+9observer，无payload，未综合。§58已copy开始强普通nextWR translation prefetch，用真实AW的fullWRITE context/两固定页槽/actualTL，不伪造nativeAW，普通commonR/W仍截nativeA第一页F（防错位环），将Aoffset0|8aligned2056..4088扩展；未运行。到08:00目标持续。
# 2026-10-12 Linux IOVA／HPM独立上游贡献

用户要求两个补丁核对并向真实上游提PR。核实生产上游malejo97/linux的master无旧驱动文件，wip/riscv-iommu/d371fb0f仍有两缺陷；更上层tjeznach/linux的新riscv_iommu.next宏已加括号，当前主线采用不同页表实现，不宣称是新主线bug。用户实际fork为xvhaocheng/linux_iommudirver（parent tjeznach/linux）。

独立稀疏worktree隔离原修复：IOVA 8e3a7406fadb单文件，HPM 77805904b3c0单文件；均一个提交从相同原上游基线出发，author xvhaocheng <xvhaocheng@gmail.com>，已SSH推送fix/iova-leaf-offset及fix/hpm-macro-arguments，无force。排除CVA6 DMA workaround、实验HPM API及生产dirty内容，未加DCO签署。IOVA主机16项旧12失败新0；实际宏提取编译32项旧14失败新0；checkpatch禁用signoff检查后两项0error/0warning。无完整内核构建、新板测或研究阶段变化。

实际向malejo97/linux创建PR返回403 Resource not accessible by integration，因此未产生PR编号，不重复相同权限失败。正文和两个预填真实上游创建入口：[submission.md](iommu-platform/out/upstream-linux-fixes-20261012/submission.md)。用户需网页点击创建或提供可用连接。DeepSeek给初稿；其普通页不变结论及后续错误模块/宏名修订被拒，Codex直接核源、隔离用户修复、完成小宏测试/执行/推送；修订完整答复留deepseek-revision.json。未调用Claude或Codex子agent。

