from pathlib import Path
import shutil,json,hashlib
B=Path(__file__).resolve().parents[1];P=B.parents[1];W=P.parent
report=P/'docs/mig-pte-debt-observation-20261015.md'
q=B/'pre-final-sync';q.mkdir(exist_ok=False)
for f in [W/'CURRENT_STATUS.md',W/'HANDOFF.md',W/'研究计划.md',P/'docs/research-execution-plan-20261011.md',P/'docs/validation.md']:
 t=q/f.relative_to(W);t.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(f,t)
shutil.copy2(B/'README.md',q/'pack-README.md')
report.write_text('''# MIG首条读债务观测：第6次构建时序失败，按用户边界暂停

2026年10月15日。实际goal1792023314，无额外token预算。本轮目标尚未完成：没有真实MIG记录，债务值为未知，不是零。原IOMMU方法论文路线及R2、CO1、RCS1、BR1、SDH1、LR1等具体关闭结论保持；本轮停止不代表论文完成。

## 稳定结果与停止原因

第6次完整构建已结束，Make退出0，BIT/MCS已生成，但**最终时序未通过，禁止加载**。未做失败后的物理优化、重布线或第7次构建；0次新BIT加载、0次工作负载尝试。采集器在用户收紧边界之后未改动。交付同步后，按用户明确授权将现有goal设为paused，工具状态见[暂停回执](../out/mig-pte-debt-observation-20261015/root/goal-pause.json)。

| 最终布线检查 | 实际结果 |
|---|---:|
| WNS／TNS | −0.391ns／−54.142ns |
| setup违例端点 | 598 |
| WHS／hold违例端点 | ＋0.049ns／0 |
| 最小脉宽余量／违例端点 | ＋0.062ns／0 |
| 完成布线／可布线网络 | 298336／298336 |
| 路由错误 | 0 |

[最终报告](../out/mig-pte-debt-observation-20261015/final-build6/reports/ariane_xilinx_timing_summary_routed.rpt:159)与[路径摘录](../out/mig-pte-debt-observation-20261015/root/sixth-final-critical-paths.json)表明两个时钟域均有setup违例：200MHz最差路径从MIG命令队列到观测器live held寄存器使能，−0.391ns，其中连线4.396ns／逻辑0.772ns；50MHz CPU issue→LSU路径最差−0.334ns。不能将失败仅归结为一条观测逻辑路径，也不能从独立综合通过推出整个平台可用。

完整[构建日志](../out/mig-pte-debt-observation-20261015/root/build-attempt-6.log)、BIT、MCS、routed DCP及报告已保留在[final-build6](../out/mig-pte-debt-observation-20261015/final-build6/DO_NOT_LOAD.txt)，[逐文件SHA256](../out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json)可导航。Make退出0只说明产物生成，不是时序验收；BIT与MCS均未加载或烧写。

## 本条记录原本会改变什么决定

- **有效正债务：**保留“该自然片段中，PTE接受时确有前序载荷读未排完”的机会前提。已接受的共同读债务不能记成候选的收益；本结果不自动准入原型或更多实验。
- **有效零债务：**排除所选首个PTE片段的“接受之后仍被前序载荷读债务挡住”解释。它不排除接受前等待、写竞争或其他片段；不换时相、不筛后续正例。
- **本轮实际结果：**因最终时序失败，没有执行冻结采集，正／零均未获得；不能用工具失败裁研究假设。

真实选择点在共享AR首次发布并承诺之前。固定普通current-VALID DS优先只能选择当时已有效、尚未承诺的offer；无法收回已进入MIG的读债务。这条AXI记录不能单独证明P在L被选中时已经可供选择，也不能证明普通方法不足。AXI返回等待还不等于DDR内部始终未服务P。旧12.7%全部归因不属于本轮目标。

## 已保留的可复用实现与成本

原A8/gap0 PN、V64、warmup256、mode3、正常对齐4096B COPY合同保持；单PTW、16项IoTLB、buffer[512,512,3,3]和DS50/50未改。生产源码未修改，所有插桩在私有快照。known-empty从实际MIG reset起连续跟踪，ARM保留carry；首次合格PTE选择不依赖债务；同沿Rfire先扣，fullID／occurrence／源job generation关联，RVALID等待与无返回区间分开。

本轮为新观测工具实施了功能检查：native120条检查、异步读回29条、解码器5组及source owner复位／复用检查通过。它们不是120个性能条件，也不是板上证据。独立UI5ns综合WNS＋1.020ns仅是OOC结果。早期实现及失败／停止构建均留存，未重跑旧科学实验或矩阵。

观测器采用1536条Rfire＋此前held长度的无损日志；源owner保存PTW64／payload96近期记录；冻结完成收据在RAM中保存。最终raw布局4608个u64、36864B；原功能队列与buffer未扩大。字段、读口、记录、两UI读回、跨时钟通知及源出口均见[全部观测费用](../out/mig-pte-debt-observation-20261015/root/observation-cost.json)。

失败实现的整平台路由资源为178668 LUT、148876 FF、69 RAMB36＋3 RAMB18、27 DSP。三个具名观测模块合计20868 LUT、21096 FF、20 RAMB36＋1 RAMB18；此小计不含全部导出／集成逻辑及全局物理复制，不能当净增总成本。资源报告是失败实现的工具结果，不是通过时序的硬件面积。实际Fmax、CPU采集成本、DDR物理字节、应用收益和方法净收益仍未测／null。

root与两个新增独占席使用6.1-sol／ultra、fork none、Codex-only；没有DeepSeek／Claude／Astra、上下文手动覆盖、旧席或关闭分支恢复。未为本次建立审查goal，当前仍是原观测goal。

## 剩余工作与增量成本

剩余的是两个域的完整时序闭合，以及之后实际BIT／reset／clock／kernel身份与原生功能验收、一次冻结采集、owner／逐拍债务校验和基于记录的裁决。当前没有执行这些后续步骤，也没有启动第7次构建。

若以后另行恢复，工程增量暂估**0.5—1个工作日**，包含问题定位／必要实现修正及至少一次完整构建；构建暂估40—60分钟（本次第6次实耗约38分钟），板端身份、一次采集及校验暂估15—30分钟。这是未执行的工程估计，不保证下一次通过。当前不自动开始任何修正。

后续提出新机制任务之前，必须同时说清：**哪里还可控制；一个具名可部署普通程序为何不足；新信息或执行动作改变什么约束。** 目前后两项未成立，因此不提出新机制任务、不改名继续优先级筛选、不自动进入GPU／AI工作流或新性能矩阵。暂停的是取证执行，不是关闭共享服务问题或改变论文定位。

[稳定根结果](../out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[收紧后的执行合同](../out/mig-pte-debt-observation-20261015/root/execution-contract.json)／[原始观测合同](../out/shared-service-control-boundary-20261015/native/observation.md)／[新交付核对](../out/mig-pte-debt-observation-20261015/root/delivery-verification.json)。
''')
cur=W/'CURRENT_STATUS.md';s=cur.read_text();suffix=s[s.index('- 独立Linux贡献'):]
(B/'root/independent-status-suffix.sha256').write_text(hashlib.sha256(suffix.encode()).hexdigest()+'\n')
cur.write_text('''# 当前状态 MIG观测停止，按用户指令暂停goal

2026年10月15日。实际goal1792023314未完成，无额外token预算；交付同步后实际调用paused，回执见下方。IOMMU方法论文主线保持，论文未完成。

- 第6次完整构建结束，Make退出0但最终时序失败：WNS−0.391ns，598个setup违例；hold／脉宽无违例，全部网络完成布线。200MHz观测路径与50MHz CPU路径均未收敛，不能加载。BIT／MCS／routed DCP与完整报告已封存。
- 0次新BIT加载、0次工作负载尝试、没有真实MIG记录。债务未知，不是零，也不是研究负结果。没有第7次构建、失败后物理优化、采集器扩展或重构；用户收紧后的源码SHA保持不变。
- 新观测工具120条native检查、29条异步读回、5组解码器及source owner检查通过；只证明工具功能参考，非板上证据。生产源码及原工作／服务／队列／buffer／walker保持，旧科学实验和矩阵未重跑。
- 正债务只保留所选片段的已接受读返回债务前提；零债务只排除该片段接受后的这条解释。两者都不补齐旧12.7%归因，不自动准入机制。普通DS优先只控制未承诺且已有效的offer，本次未证明它不足。
- 执行停止。剩余：50／200MHz完整时序、实际加载身份和功能验收、一次冻结采集及记录裁决。若未来恢复，工程暂估0.5—1工作日（含至少一次40—60分钟构建及15—30分钟板端工作），未经执行、不能保证通过。下一机制须先明确可控点、具名普通不足及新动作，不自动续新goal或筛同类候选。

[本轮报告](iommu-platform/docs/mig-pte-debt-observation-20261015.md)／[根结果](iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[实际暂停回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)／[失败产物](iommu-platform/out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json)。LR1及全部具名关闭结论保留；root与新增席6.1-sol／ultra、Codex-only，无旧席恢复或上下文手动覆盖。

'''+suffix)
(W/'研究计划.md').write_text('''# 共享IOMMU总计划：先证明剩余因果约束

2026年10月15日。论文仍以多DMA共享IOMMU方法为主线。LR1及R2、CO1、RCS1、BR1、SDH1等具体关闭结论保留；不自动改变论文定位。8/9月平台、E1/E4/batch、E2/E3及method88/M0/stage117验证资产继续复用。

路线保持：**固定问题与普通能力边界 → 剩余因果见证 → 解除约束的动作 → 有界microbenchmark → 一个GPU／AI真实共享路径 → RTL／FPGA关键验证。** 不能以关闭候选数、局部提前或同新接口下的控制等价代替方法贡献；准确发表、独立普通程序、候选等价实现和离线上界分别裁决。

## 当前执行决定

实际观测goal1792023314未完成，按用户收紧边界停止并暂停。第6次构建Make0但最终WNS−0.391ns／598个setup违例，不加载、不采集；没有第7次构建或失败后物理优化。全部源码、失败位流、检查点、报告和工具验证已保存。债务结果未知，不是有效零债务，也不否定研究问题。

本轮原问题是原A8/gap0 PN首个合格PTE接受时的前序载荷剩余R拍及其消解，不是补齐旧12.7%归因。有效正结果只保留该片段机会；有效零结果只排除该片段接受后的前序读债务解释。不能筛正债务、换时相或加压力。已接受共同债务不能记成候选收益，AXI返回等待也不能全部记成DDR尚未服务。

## 后续门槛与停止边界

真实可控点在共享AR首次发布并承诺之前：普通current-VALID DS优先可选择当时合法有效的offer，不能收回已进入MIG的读。当前没有证据证明具名普通在这个点不足，也没有已成立的新安全执行动作，因此**不提出新机制任务**。

只有能说明“具体可达状态／约束使具名普通付出什么代价；新信息或动作怎样解除它”，并计入全部费用和完整任务后果，才进入一个有界机制。单条排队现象不自动准入更多实验；两个NormalTL队头不是通用门。

若未来重新授权恢复取证，先核工程投入：双域时序闭合及一次真实身份／采集校验，暂估0.5—1工作日；不自动构建第7版、不新增审查goal、不恢复旧席或关闭分支、不重跑旧矩阵。当前没有待自动执行的下一机制／工作流／FPGA任务。功能工具通过与完整实现失败分别留存；方法空间仍未被本轮裁决。

[本轮报告](iommu-platform/docs/mig-pte-debt-observation-20261015.md)／[稳定结果](iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[实际goal状态回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)／[局部计划](iommu-platform/docs/research-execution-plan-20261011.md)。原立题窗口累计，本轮停止不是论文完成。
''')
(P/'docs/research-execution-plan-20261011.md').write_text('''# 当前局部计划：MIG取证在第6次构建门槛停止

2026年10月15日。实际goal1792023314未完成；按用户明确条件停止并暂停，不标complete。原方法路线与既有关闭结论保持。

1. 已完成私有被动观测器、真实MIG／源owner链、v2读回与解码工具。native120检查、CDC29检查、解码器5组、source owner复位复用检查通过；这些不是板测或性能结果。
2. 第6次完整构建结束：Make0，最终WNS−0.391ns／598 setup违例。UI200MHz与CPU50MHz均有负余量；hold／脉宽及布线完整性通过。失败BIT／MCS／DCP已封存，禁止加载。
3. 当前0次加载、0次冻结工作尝试、无真实MIG记录。未知不能写成零债务；工具失败不能裁研究假设。没有第7次构建、失败后物理优化或用户收紧后的采集器改动。
4. 当前只交付与同步、实际暂停，不继续修正或新增审查goal。若未来恢复，剩余为双域时序→实际BIT／reset／clock／kernel／功能验收→仅一次原A8/gap0/V64/warmup256正常4096B COPY→首个合格PTE的owner／逐拍债务与消解校验。选择与K正负无关；零结果接受有限范围，不换场景寻找正例。
5. 增量工程暂估0.5—1工作日，含至少一次40—60分钟构建及15—30分钟板端身份／记录处理，未执行且不保证通过。当前没有任何自动恢复授权。
6. 正债务仅保留当前片段的已接受读返回债务机会；零债务仅排除该片段接受后的解释。普通DS优先仍可控制未承诺且已有效的offer，其不足未被本轮证明。只有明确“哪里可控制／具名普通为何不足／新动作改变什么”并闭合成本及完整任务方向，才提出下一机制任务；不自动性能矩阵、GPU／AI工作流或新原型。

[报告](mig-pte-debt-observation-20261015.md)／[原观测合同](../out/shared-service-control-boundary-20261015/native/observation.md)／[收紧执行合同](../out/mig-pte-debt-observation-20261015/root/execution-contract.json)／[根结果](../out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[实际暂停回执](../out/mig-pte-debt-observation-20261015/root/goal-pause.json)。根及必要新席继续6.1-sol／ultra、Codex-only、原生上下文；旧实验、旧席及关闭分支不恢复。论文未完成。
''')
entry='''# 2026年10月15日 MIG观测第6次最终时序失败，按用户指令暂停

actualgoal1792023314未完成，无额外token预算。第6次完整构建Make0，但最终WNS−0.391ns／TNS−54.142ns／598 setup违例；WHS＋0.049ns，脉宽余量＋0.062ns，两者0违例；298336网络全布线、0路由错误。UI200MHz观测held使能与CPU50MHz issue→LSU均未收敛。BIT/MCS成功生成不等于通过时序，未加载或烧写。

用户收紧后采集器源码SHA不变，没有第7次构建、失败后物理优化或继续重构。0次新加载／冻结工作尝试／真实MIG记录；债务未知不是零，研究假设未被工具失败裁决。新native120检查、CDC29检查、解码器5组及source owner复位复用检查通过；独立综合UI5ns＋1.020ns仅为OOC。生产源码和单PTW／16IoTLB／原buffer／队列／服务参数未改；旧实验矩阵、关闭分支、旧席未恢复，Codex-only／6.1-sol／ultra。

最终路由整平台178668LUT／148876FF／69RAMB36＋3RAMB18；三个具名观测模块小计20868LUT／21096FF／20RAMB36＋1RAMB18，不含全部集成和全局物理复制，非净增成本或通过时序的面积。实际Fmax／CPU／应用／方法净收益null。失败原件、源码、检查点、日志、工具和前五次实施诊断全部留存。

正债务只保留所选自然片段的接受后读返回债务前提；零债务只排除该片段的该解释。均不补旧12.7%归因、不自动准入机制或实验；AXI返回等待不等于DDR内部一直未服务。真实可控点在首次AR承诺之前，普通currentVALID DS优先能选已有效offer；目前未证明它不足或新动作收益。下一机制必须先明确可控点／具名普通不足／新动作，不继续改名筛同类候选。

保存、状态／交接／总局部计划／验证同步后实际暂停goal，不标complete。剩余为双域时序和实际加载身份／功能验收、一次冻结采集及记录裁决；未来如恢复，工程暂估0.5—1工作日（含至少一次40—60分钟构建及15—30分钟板端工作），未执行、不能保证通过、不自动开始。论文未完成。

LINKS

以下历史原文保留。

'''
links_h='[报告](iommu-platform/docs/mig-pte-debt-observation-20261015.md)／[稳定结果](iommu-platform/out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[失败产物和SHA](iommu-platform/out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json)／[实际暂停回执](iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json)。'
links_v='[报告](mig-pte-debt-observation-20261015.md)／[稳定结果](../out/mig-pte-debt-observation-20261015/root-result-acceptance.json)／[失败产物和SHA](../out/mig-pte-debt-observation-20261015/root/final-build6-manifest.json)／[实际暂停回执](../out/mig-pte-debt-observation-20261015/root/goal-pause.json)。'
for f,links in [(W/'HANDOFF.md',links_h),(P/'docs/validation.md',links_v)]:f.write_text(entry.replace('LINKS',links)+f.read_text())
(B/'README.md').write_text('''# MIG首条债务观测：第6次时序未通过，停止并暂停

实际goal1792023314未完成；按用户指令在交付同步后调用paused，不标complete。第6次Make0但最终WNS−0.391ns／598 setup违例；0次新BIT加载、0次工作负载、无真实MIG记录。债务未知，不能写成零。没有第7次构建、失败后物理优化或收紧边界后的采集器改动。

- [完整报告](../../docs/mig-pte-debt-observation-20261015.md)、[稳定根结果](root-result-acceptance.json)、[实际暂停回执](root/goal-pause.json)、[交付核对](root/delivery-verification.json)。
- [失败BIT／MCS／DCP与报告](final-build6/DO_NOT_LOAD.txt)、[逐文件SHA](root/final-build6-manifest.json)、[最终关键路径](root/sixth-final-critical-paths.json)、[第6次完整日志](root/build-attempt-6.log)。禁止加载或烧写失败位流。
- [收紧后的执行合同](root/execution-contract.json)、[判定边界](root/decision-boundary.json)、[继承原观测合同](../shared-service-control-boundary-20261015/native/observation.md)。只选首个合格PTE，不筛正债务，原A8/gap0/V64/warmup256正常4096B COPY保持，实际未执行。
- [源码冻结与构建输入](root/build-inputs-attempt-6.json)、[私有插桩补丁](root/full-private-instrumentation.patch)、[实际参数](root/actual-parameter-check.json)。生产源码未改，旧分支与旧席未恢复。
- [观测器与字段](native/schema.json)、[源owner](capture/source-schema.json)、[源原文](capture/source-evidence.json)、[当前功能接受范围](root/current-native-acceptance.json)、[全部工具费用](root/observation-cost.json)。native120／CDC29／解码器5组及source owner通过，不是板测；OOC通过未替代完整时序。
- [原工具字节归档](root/original-benchmark-copy.json)、[一次运行脚本（未执行）](capture/run_one_observation.sh)、[只读回工具](capture/mig_capture_read.c)、[记录校验器](capture/validate_capture.py)。
- [实施修正与范围](root/observer-implementation-refinement.md)、[前三至五次诊断原件](before-routefix/)、[codec前原件](root/before-compressed-log/)、[任务前五页](before/)、[最终同步前五页](pre-final-sync/)。原通用关联版本、未完成构建和所有日志均保留，不冒作验收。

旧close-platform.tcl的optimize入口本轮禁用；verify-sixth.tcl也因已取得决定性时序失败而未运行。没有加载、采集或增加验证矩阵。未来新机制任务须先给出真实可控点、具名普通不足及改变约束的新动作。当前停止取证，不自动改论文定位；论文未完成。
''')
p=B/'root/current-native-acceptance.json';d=json.loads(p.read_text());d['final_full_timing']={'accepted':False,'WNS_ns':-.391,'setup_failing_endpoints':598,'actual_workloads':0,'source':'final-build6/reports/ariane_xilinx_timing_summary_routed.rpt'};p.write_text(json.dumps(d,indent=2)+'\n')
p=B/'native/README.md';p.write_text(p.read_text()+'\n最终第6次完整时序未通过：WNS−0.391ns／598 setup违例。未加载或采集，按用户收紧边界停止并暂停；OOC通过不代表可运行。\n')
print('report and five state/plan/handoff/validation destinations synchronized; actual pause call remains pending')
