#!/usr/bin/env python3
from pathlib import Path
import json
B=Path(__file__).resolve().parents[1]; P=B.parents[1]; W=P.parent
A=json.loads((B/'root-result-acceptance.json').read_text())
A['retained_model_configuration']={'actual_iDMA_paths':2,'BufferDepth':512,'DataWidth':64,'NumAxInFlight':8,'CPU_enable':False,'policy_enable':0,'nonblock_enable':False,'DS_priority':False,'read_admit':0,'warmup':0,'DDR_ptw_latency_setting':1,'DDR_latency_setting_covers_all_reads_not_only_PTW':True,'DS_fixed_delay_input':50,'DS_fixed_delay_output':50,'no_model_timing_change_across_cases':True,'independent_native_GPU_or_board_service_time':False}
(B/'root-result-acceptance.json').write_text(json.dumps(A,ensure_ascii=False,indent=2)+'\n')
C=json.loads((B/'credit-fixed-result-analysis.json').read_text())
q=lambda v:'—' if v<0 else f'{v:,}'
rows='\n'.join(f"| {r['case']} | {r['useful_bytes']:,} | {q(r['role0_hardware_done'])} | {q(r['role1_hardware_done'])} | {r['natural_quiet']:,} |" for r in C['core'])
costs=[]
for r in C['costs']:
 if not r['case'].endswith(('parallel','single')): continue
 costs.append(f"| {r['case']} | {r['pte_pages']} | {r['metadata_reserved_bytes']:,} | {r['pte_init_write64_count']+r['ddt_init_write64_count']:,} | {r['metadata_ar']:,} / {r['metadata_ar_bytes']:,} | {r['cfg_reads']:,} / {r['cfg_read_cycles']:,} |")
report=f'''# AI独立权重准备的普通基线结果与立题裁决

本轮完成了两份有独立用途的冷权重包合同、现有平台私有功能投影及八组普通G0对照。合法2MiB映射把双路完整完成从5,365,690降至2,377,027周期；后者距模型载荷返回下界仅0.63%。普通单DMA顺序搬运可让第一包在1,188,896周期完成，两包总完成只比并发晚271周期。**这版共同就绪、连续冷准备合同没有支持新增翻译门控的具体缺口，结束该立题前提，不进入G1。**

整体仍在第一阶段“问题与方法立题”，当前完成D4—5普通方法判别；没有进入新规则、留出、方法原型或实板方法验证。GPU／AI仅场景优先，主实验平台仍为Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux；执行服务器承担构建与仿真。[稳定根裁决](../out/ai-mandatory-work-g0-20261012/root-result-acceptance.json)。

## 来源任务与合法投影

固定作者FlexLLMGen提交`004ffef82b46e8dc8685c55d0cdda650bdaf1269`的两rank OPT125m切分支持rank0层0、rank1层6的不同MLP对象。每对象含fc1/fc2权重与bias及final layer norm权重/bias，六个FP16张量、9,447,936B有效数据；双包为18,895,872B。所选CPU-home策略和冷目标使两份目标对象各有用途；GPU驻留是合法相邻初态，不能把CPU-home说成容量强迫。[作者分布式源码](https://github.com/FMInference/FlexLLMGen/blob/004ffef82b46e8dc8685c55d0cdda650bdaf1269/flexllmgen/dist_flex_opt.py)／[需求卡及源码行证据](../out/ai-mandatory-work-g0-20261012/distributed-source/demand-card.md)。

模型抽取两包的提前准备，所有已提供任务逻辑ready固定为0。原分布式循环有激活收发关系，这不是原应用自然同时发起的轨迹；原两rank GPU也不能证明共享IOMMU。两个现有iDMA RTL路径在模型中向各自DDR目的区准备不同对象，是共享翻译硬件上的搬运阶段投影，不执行GPU计算。数据为可区分角色/张量/word的确定性载荷，非训练权重。源支持两包都必须形成，不要求两个DMA引擎不可替代；可信单iDMA有权访问四个buffer，作为强普通对照。[选择及边界](../out/ai-mandatory-work-g0-20261012/selection.json)。

另一项权重＋先前请求KV来源卡保留，未选入模型；其逻辑流不冒称独立DID，不构成应用阴性。此次没有第三场景搜索。

## 冻结的普通规则与测量

八案为4KiB/2MiB各匹配两种独跑、双路并发及单DMA顺序。每角色按六个完整张量FIFO、实际NEXT_ID/DONE反馈补下一笔；没有新门控、参数搜索、背景加压或猜测期限。单DMA先角色0后角色1，角色1的ready仍为0，控制等待显式保留；它改变requester/权限配置，不能作为身份匹配的因果干预。各映射独跑仍保留双上下文预算，便于与并发匹配。[原登记](../out/ai-mandatory-work-g0-20261012/g0-register.json)／[私有工程纠错后登记](../out/ai-mandatory-work-g0-20261012/credit-fixed-g0-register.json)。

保留原DDR/Tb队列、容量与握手，真实DS路径50/50固定延迟；`ptw_latency=1`对行为DDR全部读取生效，不是只改变PTW。CPU／P策略／NB／DS优先／ADMIT／warmup均关闭。配置供给从t0开始计入逐任务等待；natural quiet与轮询通知、随后64周期的静默校验分别报告。表项构建为t0前host诊断初始化，登记写入量与预留，未假装测得板端设置时间。所有案例共享相同模型设置，不据此宣称原生AI或实板耗时。

## 完成普通基线前修正的工程错误

初次driver把reset DONE初值当0，实际为1（NEXT为2），早期尝试拒绝后仅修driver初值。随后整张量八案中一案完成、七案真实进度看门狗失败，这些不属于普通策略的科学阴性。

失败专用观测显示：bank实际写授权计数C=8，低三位q=0、高位overflow=1；原WREADY/WVALID只用q非零，误把满当空，导致W无法排出，载荷R阻塞并挡住后续PTW读。私有wrap副本仅把这两处谓词改为`q非零 || overflow`，AW满门控、计数宽度、端口、FIFO和模型时序不变。八案恢复完整完成；运行中C始终不超过8、终点全零，六案实际命中满边界并降回7。原生产源码未改，MRIF/其他配置不作通用验收。[原失败](../out/ai-mandatory-work-g0-20261012/diagnostic-counter/4k-solo0/run.log)／[关键源码证明](../out/ai-mandatory-work-g0-20261012/critical/credit-boundary-review.md)／[两行私有patch](../out/ai-mandatory-work-g0-20261012/critical/credit-boundary-private.patch)。

内部AW/WLAST守恒来自直接源码证明与实际完整计数边界观测，未独立逐拍重放内部信号。外部DDR守恒、VALID背压、完整数据及真实DONE另验。该修正是既有授权的空/满纠错，不是新翻译调度方法；修正前后不能算候选性能收益。4KiB安全分段备选仅运行一组独跑：2,309描述符、六张量完整功能通过，保留成本资产；未扩为另一八案矩阵。[备选功能验收](../out/ai-mandatory-work-g0-20261012/validator/segmented-minimal-acceptance.json)。

## 八案核心结果

以下周期均为私有模型从共同ready0起算。独跑只完成一包，不能计成双包结果。

| 案例 | 有效字节 | 角色0硬件完成 | 角色1硬件完成 | 自然排空 |
|---|---:|---:|---:|---:|
{rows}

两种并发在首角色完整完成时，另一角色已有9,446,400B由完成的张量贡献，终点两包全部完成。单DMA首包完成时另一包尚无完成张量，不隐藏这个进度取舍。并发/匹配独跑约为4KiB的2.078倍、2MiB的1.999倍；这是共享载荷、翻译及供给合计影响，不能全部归因PTW。

4KiB→2MiB并发全完成减少2,988,663周期（55.6995%），同期元数据读取从221,792B降为448B，支持映射／翻译元数据敏感性。两包有效R载荷共2,361,984个64bit beat，单R口每周期至多一个，给出乐观下界2,361,984周期；2MiB并发仅多15,043周期。即便免费移除所有其余开销，全完成改善上界也只有0.63285%；这不是单角色等待或GPU全应用的上界。[核心CSV](../out/ai-mandatory-work-g0-20261012/credit-fixed-core-results.csv)／[逐任务时序](../out/ai-mandatory-work-g0-20261012/credit-fixed-job-timing.csv)／[推导](../out/ai-mandatory-work-g0-20261012/payload-bound.json)。

## 成本与验收边界

四个10MiB映射槽为40MiB，同一实际模型载荷backing预留48MiB，包含8MiB未映射guard；角色包4KiB补齐extent为9,457,664B，空洞不算有效数据。源为读权限、目的为读写，无可执行载荷；独立DID/PSCID/root及单requester重新授权均由真实DDT/PTE snapshot核验。

| 完整双包案 | PTE页数 | PTE＋DDT预留B | 初始化64bit写次数 | 元数据AR数 / 字节 | cfg读次数 / 周期 |
|---|---:|---:|---:|---:|---:|
{chr(10).join(costs)}

每个双包案另有50次cfg写、250周期；读轮询与DMA运行重叠，不能把cfg周期再加到总完成上制造重复成本。模型直接cfg不是板端CVA6/Linux执行成本。实际Linux连续对齐分配、pin/cache交接、应用提交接口、MIG服务和FPGA时钟／时序尚未闭合，因此不作计完整板端设置后的净性能主张；也不假装大页在实际应用中必然可用。[成本CSV](../out/ai-mandatory-work-g0-20261012/credit-fixed-costs.csv)。

冻结旧独立验收器对修正后的八案一次整套验收通过：四完整双包案各12任务，四独跑各6任务；检查真实issue/DONE、NEXT/DONE、完整载荷/空洞/未活动目标、raw DDT/PTE权限身份、AR/AW/B和TL序列以及quiet。per-beat R/W总量、完整48MiB guard及源保持由运行时检查承担，非独立逐拍重放；完整TL payload未暴露。原whole五负例及分段五负例均拒绝，原记录hash保留。[整套验收](../out/ai-mandatory-work-g0-20261012/validator/credit-fixed-suite-acceptance.json)。

实际共六次独立构建、20次DUT执行：10次完成（修正后八案、旧并发一案、分段独跑一案），10次拒绝或诊断失败。所有失败、私有输入冻结和原始产物保留；没有新增生产ABI、FPGA构建/板测或GPU推理，全部Codex。[执行账本](../out/ai-mandatory-work-g0-20261012/execution-ledger.json)。

## 下一关口

关闭本版all-ready连续冷准备支持新门控的前提，保留任务／模型／普通策略和工程修正资产。不能据此否定AI细粒度映射、动态生命周期或整个共享翻译领域，也不把工程纠错包装成独立方法贡献。

下一轮先过“实际约束与具体重设计”关：定向核现有平台2MiB连续对齐映射及完整准备成本、合法任务合并与真实设备所有权／隔离。优先AI页块生命周期或任务依赖，但只有准确来源提供本轮缺少的约束与用途才继续；不人为禁大页、禁普通合并或编造期限。至多两项决定问题，不重跑八案或扩大压力。若约束使普通方法不可用且有可达、可避免的翻译相关有效路径损失，才设计最多两条新规则；否则归档该前提，不直接开始RTL方法原型。
'''
reportpath=P/'docs/ai-mandatory-work-g0-results-20261012.md';assert not reportpath.exists();reportpath.write_text(report)
current='''# 当前状态 stage106 2026年10月12日 AI独立任务普通G0完成

- 整体第一阶段“问题与方法立题”，当前D4—5普通判别完成；本版共同就绪、连续冷权重准备前提结束，无G1候选。GPU／AI仅优先，主实验平台仍为Genesys2／CVA6＋四iDMA＋RISC-V IOMMU＋Linux；服务器做构建／仿真。[总计划](研究计划.md)。
- 固定作者源两rank不同MLP各六张量、9,447,936B；双包18,895,872B。原八组普通对照全部完整验收。共同ready0是抽取的提前准备，不是原GPU自然并发或共享IOMMU证明。[本轮报告](iommu-platform/docs/ai-mandatory-work-g0-results-20261012.md)。
- 合法2MiB并发5,365,690→2,377,027周期（−55.6995%），距模型载荷R下界仅0.63285%；普通单DMA首包1,188,896、双方2,377,298周期，进度取舍显式计入。没有支持新翻译门控的具体缺口；不作PTW全部归因或完整板端净收益主张。[稳定裁决](iommu-platform/out/ai-mandatory-work-g0-20261012/root-result-acceptance.json)。
- 真实写credit满8被低三位误当空的错误已定位，仅私有RTL两处纠错，恢复整张量基线；未改生产源。八案计数边界／数据／权限／完成／排空通过；MRIF／实板不作通用验收。4KiB分段一案功能资产保留，未扩矩阵。[关键审查](iommu-platform/out/ai-mandatory-work-g0-20261012/critical/credit-boundary-review.md)。
- 下一关口：定向核实际大页分配／准备成本、任务合并与设备隔离约束，优先有来源AI页块生命周期／依赖；只有普通失效和具体翻译相关损失成立才进G1，不禁合法普通方法或扩压力。尚无Linux应用adapter、板端cache/MIG／时序验收。
- 六次私有构建、20次DUT（10完成、10拒绝／诊断失败），八案整套独立通过；全Codex，无DeepSeek／Claude、GPU推理、FPGA构建／板测。旧源、脏树、原始结果及前五文档快照保留。[账本](iommu-platform/out/ai-mandatory-work-g0-20261012/execution-ledger.json)／[执行前状态](iommu-platform/out/ai-mandatory-work-g0-20261012/before/CURRENT_STATUS.md)。
'''
link='[报告](iommu-platform/docs/ai-mandatory-work-g0-results-20261012.md)／[根裁决](iommu-platform/out/ai-mandatory-work-g0-20261012/root-result-acceptance.json)／[八案验收](iommu-platform/out/ai-mandatory-work-g0-20261012/validator/credit-fixed-suite-acceptance.json)／[账本](iommu-platform/out/ai-mandatory-work-g0-20261012/execution-ledger.json)'
section='''## 2026-10-12 stage106 AI独立必需任务的普通G0与工程纠错

固定FlexLLMGen两rank层0／层6的不同MLP对象，各六张量9,447,936B；冷目标共同ready0为抽取的提前准备，非原GPU自然同时运行或共享IOMMU证明。真实两个iDMA RTL路径接受完整对象，可信单iDMA可合法准备两包；不强迫可选中转，不把流当DID。

driver reset DONE0误设按实际源改为1；七个旧整张量配置失败，经专用观测定位写creditC8低三位q0／overflow1被当空。私有wrap仅两处W非空谓词包含高位，原AW门控／宽度／容量／时序不变。修正后原八案全部真实数据、权限、NEXT/DONE、字节／B／TL及排空通过；全程C≤8、terminal0，六案满计数下降通过。MRIF／生产合入／实板不作通用验收。原失败保留，不能算普通方法科学阴性或创新收益；4KiB分段只一独跑功能验收，不扩矩阵。

合法2MiB并发完成2,377,027，4KiB为5,365,690（−55.6995%），双包18,895,872B对应单64bit R口下界2,361,984，makespan剩余上界0.63285%。2MiB普通单DMA先完成角色0为1,188,896，两包2,377,298，仅晚271；首包时另一包尚无完成张量，取舍未隐藏。模型DS50/50、DDR全读latency1及直接cfg保留；共享差额不全部归因PTW，表项host初始化／板端分配/cache/供给成本明确未测。

当前整体第一阶段D4—5结束，本版all-ready连续冷准备不进G1。下一关口定向核实际大页／完整准备成本与任务合并／隔离约束，优先有来源AI页块生命周期或依赖；只有具体翻译相关可避免损失超过准确普通对照与成本才设计新规则，不重跑已验矩阵或加压力。六次私有构建、20DUT：10完成、10拒绝／诊断失败；原whole和分段各五记录负例均拒，八案suite一次。全Codex，零生产改动／GPU推理／FPGA构建／板测；前五文档、旧源／脏树／原始数据保留。

'''+link+'。\n\n'
old=(W/'CURRENT_STATUS.md').read_bytes();assert old==(B/'before/CURRENT_STATUS.md').read_bytes();(W/'CURRENT_STATUS.md').write_text(current)
for path,snapshot,prefix in [(W/'HANDOFF.md',B/'before/HANDOFF.md',section),(P/'docs/validation.md',B/'before/validation.md',section.replace('## 2026-10-12','## 2026年10月12日').replace('iommu-platform/docs/','').replace('iommu-platform/out/','../out/'))]:
 old=path.read_bytes();assert old==snapshot.read_bytes(),path;path.write_bytes(prefix.encode()+old)
plan=W/'研究计划.md';s=plan.read_text();assert plan.read_bytes()==(B/'before/研究计划.md').read_bytes()
start=s.index('当前stage105完成');end=s.index('\n详细合同、运行登记',start)
s=s[:start]+'''当前stage106已完成AI独立冷权重准备的有界普通G0：固定作者两rank各六张量、双包18,895,872B，私有真实iDMA/IOMMU整张量八案完整验收。两处私有写credit空/满纠错恢复普通基线，生产源不变；合法2MiB并发2,377,027周期，4KiB为5,365,690，距模型载荷R下界仅0.63285%。普通单DMA第一包1,188,896、双包2,377,298，进度取舍明示。本版共同就绪／连续冷准备前提结束，无G1；不是原生GPU或完整板端净收益。[本轮裁决与成本](iommu-platform/docs/ai-mandatory-work-g0-results-20261012.md)。

下一关口定向核现有平台实际2MiB分配和完整准备成本、合法任务合并与设备所有权／隔离；优先来源明确的AI页块生命周期／任务依赖。只有准确普通对照未覆盖的可达翻译相关有效路径损失成立，才比较最多两条新规则；不能人为禁大页／合并、猜期限或扩大压力。保留已验资产，实板方法验证仍在立题通过后，不恢复服务器GPU主线。
'''+s[end:];plan.write_text(s)
concrete=P/'docs/research-execution-plan-20261011.md';s=concrete.read_text();assert concrete.read_bytes()==(B/'before/research-execution-plan-20261011.md').read_bytes();pos=s.index('## stage98')
execsection=section.replace('## 2026-10-12 stage106 AI独立必需任务的普通G0与工程纠错','## stage106已完成普通G0与下一约束关口').replace('iommu-platform/docs/','').replace('iommu-platform/out/','../out/')
s=s[:pos]+execsection+s[pos:]
old='stage105已完成AI来源平台功能投影；当前只在新的必需独立任务合同达到关口时登记有界G0，不以可选中转或恢复旧机制替代具体缺口。'
new='stage106已完成AI独立任务的普通G0，本版冷连续准备前提不进G1；下一轮先核实际映射／所有权／生命周期约束及完整成本，有具体未覆盖损失才设计新规则。'
assert old in s;s=s.replace(old,new);concrete.write_text(s)
print('one shared-doc batch written: report + five state/plan documents')
