# 共享 IOMMU 研究具体执行计划

2026-10-11发起；2026-10-12收束。本文件承接[总计划](../../研究计划.md)。执行由当前 `/goal` 管理；日期是实际记录时间，不把预计十工作日写成已完成十天。

## stage111普通接口／唯一构建／部署批准关（当前入口，2026-10-12）

真实goal已登记并active，无额外预算。必要普通接口和一次干净构建、静态RV64／host校验、完整原分区备份、板端payload hash与updater dry-run已接受；尚未收到最终新SD写入许可，没有新kernel实际boot或R107 DMA恢复。goal不标complete，普通全成本／翻译残余未测，无新G0/G1。仅同一有限合同、合法大页/4KiB/预映射/驻留/批量/单DMA；不重复stage106/108、新RTL或扩场景。

[当前报告](r107-ordinary-pilot-20261012.md)／[部署前接受](../out/r107-ordinary-pilot-20261012/predeployment-acceptance.json)／[最终部署方案](../out/r107-ordinary-pilot-20261012/deployment-proposal.json)／[原执行依据](../out/r107-platform-identity-recovery-20261012/next-goal-contract.json)。批准后新boot／计时前提通过才同实例一次ordinary采证；具体可避免翻译残余才最小G0，普通覆盖则只结束R107卡并回阶段一新合同，技术失败不作科学阴性。

## stage110平台身份恢复裁决（历史入口，2026-10-12）

stage110完成JTAG／指定bit／新boot与S0有界核验，R107条件保留。CPU DMI默认IDCODE1被误当FPGA；B接口实xc7k325t/0x43651093。用户批准载入冻结优化f9fa9434…，第一次mask verify通过但SD块CRC失败，第二次同文件不并行readback后启动成功；CRC物理原因未知。新boot 7fd964e5-563f-458d-9617-c890f737f068、config/DTB/notes一致、taint0，observerV9/ABI1/config0xA00=S0/DID10、idle1；选择器还原，未执行DMA。

一次干净内核构建56.886秒，81122项源冻结、2078/2079链接对象与四关键驱动逐字同；临时version元数据32B造成完整Image/ELF未匹配，一次metadata恢复未保留成功中间对象且递归覆盖，停止，不声称完整旧Image来源闭合。下一走新源码冻结ordinary执行内核/接口并匹配其真实boot，避免旧横幅循环。尚无普通分配/map/恢复/全成本或translation残余，不进G0/G1。

相关clock/IO/CDC按真实路径处置，允许有限功能采证条件，绝对秒/吞吐需独立计时校准，不能用50/25MHz声明或2:1比例冒充实测。保留同实例144或合法96笔/1769472B、合法大页/4KiB/premap/residency/batch/singleDMA；不重复stage106/108、扩场景或加压力。133旧stage109文件及46内核输入不变，生产代码/config/原历史保留；两次授权易失载入、无Flash/SD镜像写入，Linux运行、UART释放。全Codex root＋两独占Sol/ultra，零DeepSeek/Claude/Astra。

当前下一关口仅新source-frozen ordinary内核/接口与同实例一次完整成本；新部署先有具体可审产物，未登记后继goal。归档条件与阶段一后续路线继续按本次合同。

[报告](r107-platform-identity-recovery-20261012.md)／[稳定裁决](../out/r107-platform-identity-recovery-20261012/root-result-acceptance.json)／[下一合同](../out/r107-platform-identity-recovery-20261012/next-goal-contract.json)。

## stage109执行前提裁决（历史入口，2026-10-12）

真实goal已登记，无额外token预算。R107条件保留有限合同；当前Dell实际Linux访问、DTB/内核notes构建身份、四idma及eth0已核，但载入bit、runtime S0与dirty kernel source/config freeze仍未闭合。JTAG精确目标可开，实际Unknown_Device/IDCODE1，原因未知。可复用静态候选为route-opt f9fa9434…，内部50MHz时序+0.022/+0.051；原manifest f15f8a72…−0.219/1失败不能混用，IO/CDC缺口分列。

ordinary adapter必要范围已核，未实现或运行恢复。只补具体新身份/冻结/时序事实，齐备才同实例一次完整准备与完成采证；保持大页/预映射/驻留/批量/同owner单DID合并与全部成本。无普通未覆盖翻译loss，故无G0/G1。不可达或普通覆盖则归档卡，回整体阶段一新需求合同，不自动恢复旧场景/加压力/重跑stage106或108。129旧输入不变，全Codex；当前Linux保持运行，UART释放。

[报告](r107-execution-gate-20261012.md)／[稳定裁决](../out/r107-execution-gate-20261012/root-result-acceptance.json)／[下一合同](../out/r107-execution-gate-20261012/next-goal-contract.json)／[账本](../out/r107-execution-gate-20261012/execution-ledger.json)。本关口完成不代表方法论文完成。以下stage108及更早内容为历史记录。

## stage108有限KV合同与有界裁决（历史入口）

R107条件保留合同资产，没有新G0/G1。完整OPT125m形状与source方法一次CPU核验闭合3唯一槽、144完整恢复/1,769,472B、同量此前swapout和24worker-layer；这不是原生trace/GPU/FPGA。663,552B只proxy标记，nativeuse与时序未验。真实硬件preissue控制已有，Linux普通manifest/stablepool/layer-event glue尚未实现；实现缺口不作科学失败。

普通四完整4MiB/order10或八2MiB/order9、16MiB backing/8目标叶在源码大小上合法；保留4KiB/premap/residency/batch及同owner singleDMA/coalescing96笔。actual分配/leaf/cache/fence/完整成本及ordinary残余翻译loss仍未知，8<16不证明零miss。root643项、平台200项、73旧输入保护通过，旧八案不重跑；零新DUT/build/GPU/board/allocation/map/生产修改，全Codex。

下一仅有前提的同实例普通prepare/full completion采证合同，尚未登记后继goal。先核actual镜像/模式/时序身份与必要ordinary adapter范围，再保存全成本/真实叶/缓存/完成/排空/残余归因；没有这些新输入或普通覆盖就归档R107，不重复源/CPU回放，不加场景/压力/猜计算。具体可避免翻译loss成立才最小G0，G0支持后最多两G1方向。本轮完成不是论文完成。

[报告](r107-bounded-decision-20261012.md)／[稳定裁决](../out/r107-bounded-decision-20261012/root-result-acceptance.json)／[合同和数据](../out/r107-bounded-decision-20261012/instance/finite-contract.md)／[普通成本](../out/r107-bounded-decision-20261012/platform/cost-inventory.json)／[下一合同](../out/r107-bounded-decision-20261012/next-goal-contract.json)／[账本](../out/r107-bounded-decision-20261012/execution-ledger.json)。stage107原文与输入保留在本轮before及原ai-platform-constraints目录；其源级事实未改。

以下stage106及更早条目为历史记录，其中“当前／下一”指当轮；执行状态以本节与CURRENT_STATUS为准。

## stage106已完成普通G0与下一约束关口

固定FlexLLMGen两rank层0／层6的不同MLP对象，各六张量9,447,936B；冷目标共同ready0为抽取的提前准备，非原GPU自然同时运行或共享IOMMU证明。真实两个iDMA RTL路径接受完整对象，可信单iDMA可合法准备两包；不强迫可选中转，不把流当DID。

driver reset DONE0误设按实际源改为1；七个旧整张量配置失败，经专用观测定位写creditC8低三位q0／overflow1被当空。私有wrap仅两处W非空谓词包含高位，原AW门控／宽度／容量／时序不变。修正后原八案全部真实数据、权限、NEXT/DONE、字节／B／TL及排空通过；全程C≤8、terminal0，六案满计数下降通过。MRIF／生产合入／实板不作通用验收。原失败保留，不能算普通方法科学阴性或创新收益；4KiB分段只一独跑功能验收，不扩矩阵。

合法2MiB并发完成2,377,027，4KiB为5,365,690（−55.6995%），双包18,895,872B对应单64bit R口下界2,361,984，makespan剩余上界0.63285%。2MiB普通单DMA先完成角色0为1,188,896，两包2,377,298，仅晚271；首包时另一包尚无完成张量，取舍未隐藏。模型DS50/50、DDR全读latency1及直接cfg保留；共享差额不全部归因PTW，表项host初始化／板端分配/cache/供给成本明确未测。

当前整体第一阶段D4—5结束，本版all-ready连续冷准备不进G1。下一关口定向核实际大页／完整准备成本与任务合并／隔离约束，优先有来源AI页块生命周期或依赖；只有具体翻译相关可避免损失超过准确普通对照与成本才设计新规则，不重跑已验矩阵或加压力。六次私有构建、20DUT：10完成、10拒绝／诊断失败；原whole和分段各五记录负例均拒，八案suite一次。全Codex，零生产改动／GPU推理／FPGA构建／板测；前五文档、旧源／脏树／原始数据保留。

[报告](ai-mandatory-work-g0-results-20261012.md)／[根裁决](../out/ai-mandatory-work-g0-20261012/root-result-acceptance.json)／[八案验收](../out/ai-mandatory-work-g0-20261012/validator/credit-fixed-suite-acceptance.json)／[账本](../out/ai-mandatory-work-g0-20261012/execution-ledger.json)。

## stage98已结束批次

本批已完成D1状态收束、D2—3两工作流预筛和A的有界D4—5判别，按停止条件提前归档；这不是已执行十个工作日。stage97 已完成的 M0/G0 不重跑：完整64＋64夹具保留，P/PN差额支持新门控的理由关闭。

| 产物 | 当前状态 | 证据入口 |
|---|---|---|
| 当前总计划与全景结论 | 已同步 | [总计划](../../研究计划.md)、[全景审查](research-panorama-audit-20261011.md) |
| 历史保留/关闭主张 | 已接受与落盘 | [历史证据](../out/research-execution-20261011/history-evidence.json) |
| A 行预取需求卡 | G0验收；普通P已满足登记期限，本假设归档 | [需求卡](workflow-row-prefilter-20261011.md)、[实际结果](workflow-row-g0-results-20261011.md) |
| B 双缓冲流处理需求卡 | 已预筛；缺实际计算／slot释放证据而延后 | [需求卡](workflow-stream-prefilter-20261011.md) |
| 普通策略运行登记 | 冻结两项，均完成且独立验收 | [登记](../out/research-execution-20261011/row-baseline-register.json)、[根裁决](../out/research-execution-20261011/root-result-acceptance.json) |
| 新规则／留出／原型 | 立题未通过，按关口未进入 | A只关闭本例需求；B不是实验阴性；总主线保留 |

所有本批执行证据保存在 `out/research-execution-20261011/`。初始源哈希及计划/状态快照已保留；后续实验在独立产物目录进行，保护已有验收源与原始数据。

## stage99后续应用采证（2026-10-12）

新的`/goal`已执行B最小应用采证。取得官方HERO固定256²内核，六槽96KiB、288次16KiB搬运及256tile；私有主机功能运行一次，65,536值和1,390事件独立验收通过。此源与原SP不同，原active路径copy-based、SVM关闭；六槽合同取代把四槽泛化模板直接当B实际参数。见[本轮报告](workflow-stream-contract-20261012.md)与[根裁决](../out/workflow-stream-contract-20261012/root-result-acceptance.json)。

本轮来源／功能／依赖关完成；剩余是同clock计算＋有限端口存储、实际DMA／供给及普通失效数据。两条平台途径及新增资源已登记，不能以host_ns或猜delay跳过；没有新B性能实验，不宣称普通失败或B阴性。落实一个明确且计成本的平台合同才冻结G0基线／相邻判别；成本过大则结束本选题轮次，不自动建设原8PE环境或扩搜索。

## stage100同clock适配及有界G0（2026-10-12）

后续goal已落实预先冻结R2：256²、六槽96KiB、单乘法器/单总读写端口、一周期响应，与真实iDMA/IOMMU/行为DDR同钟；实际配置、供给、计算和排空计时。四案完整288搬运/256tile及65,536输出、全槽多代次独立通过，稠密微迹和端点R/B反压/越界另验；生产ABI未改。

匹配4KiB独跑/共享V100813737/100819743；普通首块准备V100813799，恢复5944/6006周期、余差62，后台完整完成还提前1191。2MiB邻例V100806133，没有匹配独跑不作共享归因。固定8MiB后台自然完成，不扩强度。AW容量挂起的私有正确性修正四案共用，未变成候选方法。

当前整体阶段一D4—5分支已结束：普通策略覆盖主要差额，无保留G1规则，不进入留出/原型/FPGA。本结论限定R2，并不否定原八PE/更快合法架构/共享领域；后续须有新独立需求或平台事实支持残余损失再开具体重设计。两周是立题投入估计，不能写成方法论文已经完成。见[最新报告](workflow-stream-g0-results-20261012.md)、[稳定裁决](../out/workflow-stream-g0-20261012/root-result-acceptance.json)、[成本](../out/workflow-stream-g0-20261012/cost-boundaries.json)和[实际账本](../out/workflow-stream-g0-20261012/execution-ledger.json)。

## stage101真实平台条件复核与本轮终点（2026-10-12）

新的goal已核原SP16KiB图题（旧块大小未知勘误）、固定HERO八核/16bank/128KiB与当前单CVA6、实际生产MUL流水和旧报告时序限制。没有取得当前原应用实测吞吐或独立后台自然合同，不按核数缩放R2、不自动改流水R3。

既有四案后续255交接均无供给气泡，4k BG q7结束；6006/62差精确为5988+18/45+17。startup虽使后台全完成提前1191，首服务却推迟13549；不能给其新增无来源期限。新facts只支持条件工程可能性，未支持新G0失效或G1。

当前裁决为结束本轮两工作流有限立题筛选，无留出/方法原型/FPGA。重开需实际共同时间基计算/搬运、独立BG到达/结束/目标和强普通策略未覆盖的关键路径等待；原有限受控背景与所有阴性/因果证据保留。不是整个领域失败或论文目标完成。[最新报告](workflow-stream-platform-review-20261012.md)／[稳定裁决](../out/workflow-stream-platform-review-20261012/root-result-acceptance.json)／[开启条件](../out/workflow-stream-platform-review-20261012/reopen-conditions.json)。本批只有原文/源码/已有记录推导，无新build/model/validator/实板。

## stage102真实后台需求补证终点（2026-10-12）

用户选择优先后台真实需求，批准两材料/一天上限方案并执行。两项固定官方材料为应用README和suite testset：前者构建/线程/DMA选项、后者四子测试登记，均未给所选独立背景合同。既有内核明示device0/1属同一物理加速器，同时使用不安全；不能把逻辑设备号、同一矩阵A/B/C或测试列表当独立应用背景。未核调度器，不推所有测试串行。

独立背景身份/到达/有限工作/自然结束/目标/共享路径仍未决，补证到限结束，无新时序构建或G1。原8MiB研究背景和所有阶段结论保持；没有deadline非否定条件，实验设定须区别原生来源。下一次重开需具体任务合同或记录的新事实，再规划实际同时间基测量，不重复补查旧事实、不扩压力凑候选。[报告](workflow-stream-background-demand-20261012.md)／[需求卡](../out/workflow-stream-background-demand-20261012/background-demand-card.json)／[稳定裁决](../out/workflow-stream-background-demand-20261012/root-result-acceptance.json)。全Codex，零新实验或生产修改。

## stage103用户批准的新真实场景预筛（2026-10-12）

本轮明确调整旧A/B搜索范围为网络/独立存储、周期采集/独立后台两项；两Codex席各至多三材料，总投入两工作日上限，根选至多一项。实际六材料到限、两来源卡接受；iperf/SPDK和SDI/YAVTA的具体任务/生命周期正向事实保留，但实际同运行实例共享翻译路径及完整事件/完成/成本未闭合，两项均未过可执行G0，无登记/模型/G1，不追加第三场景。

无deadline非否定，研究组合可明确登记；不同设备或VFIO group不证明共同cache/PTW，默认DTS不替代实际image。根直接核协议/command/DMA边界、真实buffer返回语义及发布前控制能力；DDR环不要求进入R2 SPM。两条件失效假设待实际数据，普通大页、稳定buffer/布局/准备、优先与相关反馈均计成本。具体实例路径和任务事件新事实到位后才重开，不扫有利强度、不恢复旧gate。[报告](real-dma-scenario-screen-20261012.md)／[选择](../out/real-dma-scenario-screen-20261012/selection.json)／[根裁决](../out/real-dma-scenario-screen-20261012/root-result-acceptance.json)。当前仍阶段一，新预筛结束，方法论文未完成。

## stage104 GPU／AI优先与实验平台纠偏（2026-10-12）

用户明确本机是服务器而非实验平台。stage104来源和只读T400／NVMe／dmar1资料留作参考；原本机OPT6.7B两配置前置采集安排撤销，历史合同保留。执行平台仍为现有CVA6＋四iDMA＋RISC-V IOMMU＋Linux／Genesys2，仿真按需辅助，关键验证回现有FPGA。GPU／AI优先而非排他，不意味换平台或直接复现商用GPU。

下一步骤是能力与场景对应：利用已取得的AI任务／依赖／缓冲／映射生命周期与控制资料，核哪些在当前实际RTL和存储/计算预算中成立，哪些属于明确负载回放，哪些需要未实现硬件或不可见信息；先计适配成本、合法普通策略和一正一反条件，选择一个适配可达的工作流。未知计算速度不猜delay，不要求把完整大模型搬进FPGA，不把参考服务器采集当FPGA创新证据。当前无可执行AI平台合同或G1。

没有安装、模型下载、AI运行或实验平台切换。旧源／产物／结论保留，纠偏本身不作科学阴性。[有效报告](gpu-ai-deployment-gate-20261012.md)／[纠偏裁决](../out/platform-scope-correction-20261012/scope-correction.json)。

## 两张卡与唯一选择

每张卡要说明数据工作量、逻辑就绪、数据消费、缓冲寿命、实际映射/权限、真实目标、最强简单对照、可控位置、失效假设、相邻反例、可改善上界、最低适配/转换成本和证据缺口。

A沿已有Sobel合同做预筛：640×480、30fps、1B像素、4×640B行槽、前3行初始化与最后消费单列，477行稳态任务。每行释放/期限用精确整数；不能把禁止提前覆盖扩大成禁止提前翻译/预读。大页分配、缓冲和额外暂存公平计价。背景是本研究追加的独立DID11 copy，不冒称DASH已评价该组合。

B的最初预筛采用MMU-aware DMA原文的regular-strided模板；stage99已用HERO明确内核及六槽依赖补齐应用来源，不冒称原SP复现。论文的intensity sweep仍不代表自然阶段；stage100已用明确R2计成本同clock投影补齐本轮计时，不等于原八PE持续时间或拼合有利阶段。

选择规则：有需求来源且存在可检验普通失效假设的一项进入G0；两项均满足时优先适配更小者。原文/原始轨迹支持不足属于证据未决；普通实际满足需求才属于该失效主张被否定。两项均未得到可测题时结束本批预筛并记录最小缺证，不并行建设两个模型。

## G0最小执行与后续关口

1. 冻结一套任务、映射、初态、CPU/内存背景、完整工作量、尾段规则、资源和计时；不改变逻辑ready以藏控制等待。
2. 选相关的固定普通策略、已有反馈和强简单策略；先判断无需保护是否已满足实际需求。大页/提前准备可用且合法时允许，并报告真实代价。
3. 只做具体失效与相邻对照。若要报告共享slowdown才补匹配独跑；若要翻译归因才补同载荷低翻译压力或已核可执行服务干预。内部模型队列调度只作诊断，不当免费硬件基线。
4. G0确认缺口后，G1才比较最多两条新规则的一正一反。应用提供的标记各方同享；新信息不是创新的必要条件。
5. G2按剩余问题做消融、完整成本和两组冻结留出；训练与留出分开。只因新事实或失败定向修订，不重跑未变已通过检查。
6. G3交付贡献卡、核心图、成本表与推进/重设计/归档裁决。通过才定义一个最小原型和现有FPGA关键验证。

## stage105 AI来源到现有FPGA的功能适配与G0裁决（2026-10-12）

本轮goal完成固定作者源、平台能力、完整任务合同和实际DMA功能关。选择源码支持OPT125m一层MLP六FP16张量9,447,936B，2,309张量内4KiB分段；整张量fill完成后copy才能ready。原首跳为CPU memcopy、默认pin relay共8GiB，不能叫SSD DMA或免费移植；仅准备阶段，无猜测compute/deadline。原接口缺任意张量提交，私有有限调度和观测不改生产ABI。

一次新独立构建冻结106项；两跳4,618笔及驻留源一跳2,309笔完整真实硬件完成、数据、依赖和quiet通过。独立记录验收另核，三类四种错误记录拒绝；不是新增DUT负载。全串行功能种子不支持并发收益结论，行为DDR/directcfg不代表CPU/cache/MIG或实板性能。源、中转、目的合计48MiB、页表212,992B及DDT4KiB明示，板端分配和软件供给成本未验。

功能资产接受，受控搬运G0技术可执行；当前DDR驻留源的一跳合法完成相同对象，因此当前可选中转分支不进入方法性能G0，无普通失效/G1，不自动执行四案矩阵。普通按张量流水、事件反馈、全包准备、驻留直接拷贝及合法大页/整张量提交规则保留。下一固定来源支持、双方各有独立用途的必需AI搬运及平台投影，再冻结有界G0；不把整套商用GPU迁移到FPGA当入口条件。

[报告](gpu-ai-fpga-fit-20261012.md)／[稳定裁决](../out/gpu-ai-fpga-fit-20261012/root-result-acceptance.json)／[任务合同](../out/gpu-ai-fpga-fit-20261012/transfer-contract.json)／[普通规则与入口](../out/gpu-ai-fpga-fit-20261012/g0-entry-contract.json)／[账本](../out/gpu-ai-fpga-fit-20261012/execution-ledger.json)。三个新增Codex席gpt-6.1-sol/ultra，无DeepSeek／Claude；无GPU推理、FPGA构建、板测或生产变更，旧资产和脏树保留。整体仍第一阶段，当前适配goal完成，方法论文未完成。

## 实验正确性与成本

沿用M0的硬件issue/done、真实字节/B/quiet守恒思想，按实际工作流替换64＋64参数。新行任务必须核实际写入不早于slot释放、数据在真实done后正确，且各任务恰好一次。共同起点、必要初始化、准备和排空均声明。后台固定有限任务完成不截断，共同观察窗内的后台进度另报告。

补测只针对当前输入：权限、VALID稳定、同ID次序、背压、写段与完成归属、最终进度。无生产ABI改动的测试适配不能称新机制。切模式需quiet/clear，计排空、软件配置、冷缓存与重热。净收益扣除观测、路由、存储、端口、控制、准备、切换与物理分配成本。

stage98终点为A有限假设归档、B缺证登记；stage99补应用合同，stage100补同clockR2并完成固定G0分支归档。stage106已完成AI独立任务的普通G0，本版冷连续准备前提不进G1；下一轮先核实际映射／所有权／生命周期约束及完整成本，有具体未覆盖损失才设计新规则。

本次执行席均为Codex；用户最新允许模型不限，本批复用已有`gpt-6.1-sol`、`ultra`席，不调用DeepSeek或Claude。实际新编译、仿真、板测和检查次数由执行账本记录，不在未发生时预填。
