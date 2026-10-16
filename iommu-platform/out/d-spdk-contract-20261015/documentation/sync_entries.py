from pathlib import Path
import hashlib,json,re,datetime
root=Path('/home/xvhaocheng/work'); run=root/'iommu-platform/out/d-spdk-contract-20261015'; doc=run/'documentation'
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
# Read exact current bytes immediately before constructing and applying this update.
before={p:(root/p).read_bytes() for p in paths}
sha=lambda b:hashlib.sha256(b).hexdigest()
for p,b in before.items():
 s=doc/'before-write'/p; s.parent.mkdir(parents=True,exist_ok=True); s.write_bytes(b)
out={p:b.decode() for p,b in before.items()}
def replace(p,old,new):
 assert out[p].count(old)==1,(p,old[:100]); out[p]=out[p].replace(old,new,1)
p='CURRENT_STATUS.md'
replace(p,'# 当前状态：D来源门裁决与条件重设计','# 当前状态：D来源链与无计时语义原型')
s=out[p]; start=s.index('- D本轮'); end=s.index('\n- I有条件',start)
out[p]=s[:start]+'''- D已接受SPDK映射/事件来源链及无计时闭环语义原型：8种callback次序、6个负变异拒绝、2个错误案例通过；均为synthetic，实际/联合trace各0，无计时、原生或P/PN收益。固定M0旧来源门不通过不变。新条件服务性能模型仍NOT_ADMITTED；下一步冻结provider/资源/方向/动作、O0与付费Z、全费用/完整终点、跨quiet持续性及留出。来源绑定的显式研究投影不以原PC/SSD原生P/PN、真实联合trace或预测反号为普遍建模前提；部署主张与校准仍须相应实证。[本轮入口与接受](iommu-platform/out/d-spdk-contract-20261015/README.md)。'''+s[end:]
replace(p,'本轮仅源码/旧记录核查与文档落盘，0新模型/实验回放/编译/Linux或RTL或ABI修改/FPGA或板操作。','本轮新增1个无计时合成语义原型；实现席执行生成/验证，root只读重消费已登记合成数据，未重跑生成器。0旧实验重跑/原生DMA/编译/Linux或RTL或ABI修改/collector/FPGA或板操作。')
p='研究计划.md'
replace(p,'D先固定同实例来源/合法动作/状态持续性及相邻相反条件预测；满足接口后用小模型判别，固定M0本轮不扩大','D来源链与无计时语义原型已接受；下一关冻结显式服务投影、动作/观测/费用/持续性与留出，固定M0本轮不扩大')
start=out[p].index('当前实际结果：'); end=out[p].index('\n\n## D：',start)
out[p]=out[p][:start]+'''当前实际结果：固定M0旧来源门不通过，六份COPY日志及停止扩大/调gap/collector的裁决不变。本轮取得SPDK映射与事件实现链，完成无计时合成闭环原型：8种callback次序、6个负变异拒绝、2个错误案例通过；root只读重消费已登记数据，未重跑生成器。实际/联合trace各0，无计时、原生或P/PN收益。**新条件服务性能模型仍NOT_ADMITTED**；下一步先冻结一份服务投影合同，而非改旧M0。D主线、I轻量not_ready、A5保留及10月15—28日窗口不变。[本轮完整交付与稳定接受](iommu-platform/out/d-spdk-contract-20261015/README.md)。'''+out[p][end:]
replace(p,'首轮只比较P（分区）与PN（分区＋合格命中旁路）。','固定M0首轮只比较P（分区）与PN（分区＋合格命中旁路）；该旧合同保留，不能冒充新的NVMe服务投影。')
old='**来源绑定的条件预测可以进入小模型，不要求建模前已测出完整反号。** 准入材料须固定真实事件/调用链、状态来源与身份、共同目标/O0/合法动作及相邻相反预测，并保证模型可忠实表达因果边。本轮缺的是这些来源/接口/状态绑定，故未启动模型；不得用先验保证收益挡住研究。'
new='**来源绑定的显式研究投影可以进入条件小模型，不要求原PC/SSD已原生支持P/PN，也不要求先有真实联合trace或实测反号。** 来源语义S已接受；服务性能模型M须冻结provider及参数域、研究资源/方向/动作、完整O0与付费Z、全部费用、完整终点、跨边界状态持续性和留出后才准入。实际部署、服务校准和原生效果N仍需对应真实证据，不能由合成结果补齐。当前只完成S的无计时原型，M仍NOT_ADMITTED；缺服务合同，不能用先验保证收益挡住研究。'
replace(p,old,new)
start=out[p].index('下一最小材料是一份'); end=out[p].index('\n\n## I：',start)
out[p]=out[p][:start]+'''下一最小材料是一份**源—服务研究投影合同**：保留同task/buffer完成→补发规则，明确两个owner、NVMe read的device→host payload方向、资源/映射、provider状态转移及参数域、拟议P/PN动作、安全许可、决策前O0/付费Z可见时刻、跨quiet/clear/warm保留与重置、双方同成功工作量的完整终点及配置/预热/观测/切换/排空/释放费用，事前冻结留出与可推翻条件。每个动作必须重算完成→补发反馈，不能把一方固定到达表当反事实。[投影设计](iommu-platform/out/d-spdk-contract-20261015/design/proposal.md)／[root决定](iommu-platform/out/d-spdk-contract-20261015/root/decision-analysis.md)。

普通对照包括常驻池、合法大映射/准备、固定P/PN、完整O0规则、队深/poll和含全部扰动费用的试选；付费Z同样交给普通规则。stock QD变化同时改变task池与翻译footprint；固定池admission/pacing须明确实现并计费，不能假装stock已有。SPDK汇总elapsed在最终drain之前采样，io_completed含错误，均不能直接充当完整成功目标。若O0足够、持续性被clear/warm消除、Z太迟/太贵或普通方案同成本复现，只停止相应增量。未准入前不扩大固定M0、不调gap、不加collector或改guard，不运行覆盖历史结果的包装脚本。'''+out[p][end:]
p='研究导航.md'
s=out[p]; start=s.index('| D：有限观测'); end=s.index('\n| I：',start)
out[p]=s[:start]+'''| D：有限观测→完整控制选择 | 主线；来源链/无计时合成原型已接受（8次序/6拒绝/2错误），性能M仍NOT_ADMITTED；固定M0旧门不变 | [本轮入口](iommu-platform/out/d-spdk-contract-20261015/README.md) · [映射](iommu-platform/out/d-spdk-contract-20261015/mapping/findings.md) · [事件](iommu-platform/out/d-spdk-contract-20261015/events/findings.md) · [服务投影](iommu-platform/out/d-spdk-contract-20261015/design/proposal.md) · [原型/验证](iommu-platform/out/d-spdk-contract-20261015/model/README.md) · [稳定接受](iommu-platform/out/d-spdk-contract-20261015/root/root-result-acceptance.json) |'''+s[end:]
replace(p,'本轮机制校准、SPDK完成驱动供给的最强条件版本、YAVTA尾三帧边界及下一最小联合材料见[本轮入口](iommu-platform/out/d-source-gate-20261015/README.md)。两张卡不是已取得的联合部署/自然反号；不声称性能或新意已证。','下一关固定provider/资源/方向/动作、O0/付费Z、全费用/完整终点、跨边界持续性及留出；实际/联合trace各0。条件研究投影不以原PC/SSD原生P/PN、真实联合trace或预测反号为普遍门槛；部署/校准仍需相应实证。[旧机制门与两卡](iommu-platform/out/d-source-gate-20261015/README.md)保留；没有新性能或新意证明。')
handoff='''# 2026年10月15日 D-SPDK来源链与无计时闭环语义原型

D主线/I轻量/A5保留及10月15—28日窗口不变。root稳定接受源码链和clockless语义，不接受性能模型或原生效果。SPDK同task/buffer完成后补发、IOVA/VFIO/PRP实现链及事件schema已核；LBA不是host IOVA，IO/PRP/CPU hugepage不能证明实际IOMMU leaf。实际trace与联合trace各0；SC/SCT判错误而非phase，io_completed包含错误，perf elapsed在最终drain之前采样。

冻结S合同后新增无计时合成原型，N4/Q2仅为验证常量，8种接口允许callback次序、6个拒绝变异、2个错误案例通过；root独立只读重消费已登记数据，未重跑生成器。每个动作须重新演化完成→补发，不用固定到达代替反事实；当前不含服务/地址/计时，不证明全部次序在原生硬件可达，更不证明P/PN收益。

新服务性能模型M仍NOT_ADMITTED，下一步冻结provider/参数域、资源/方向/动作、安全许可、完整O0与付费Z、全费用/完整成功终点、跨quiet持续性及留出。源绑定的显式研究投影不要求原PC/SSD已有原生P/PN、真实联合trace或先测反号作为普遍建模前提；实际部署主张、校准与原生效果仍须相应真实证据。普通对照含常驻/大映射/准备、QD/poll、完整O0与付费试选；stock QD改变池大小及翻译footprint，固定池admission/pacing须显式实现并计费。

旧M0来源门及六日志不变；0旧实验重跑/构建/原生DMA/Linux或RTL或UAPI改动/collector/FPGA或板操作。MIG1792023314 paused、build6旧时序失败与债务unknown保持，A5/A2和独立工程事项按写前现状保留。Codex-only：证据/设计/实现及文档请求Sol high/xhigh/medium，实际运行元数据unknown；不把请求当实际证明。旧个人研究方法由用户删除，不恢复；ARS-Codex按需使用。

[完整入口](iommu-platform/out/d-spdk-contract-20261015/README.md)／[稳定接受](iommu-platform/out/d-spdk-contract-20261015/root/root-result-acceptance.json)／[root决策](iommu-platform/out/d-spdk-contract-20261015/root/decision-analysis.md)／[执行账本](iommu-platform/out/d-spdk-contract-20261015/root/execution-ledger.json)／[文档回执](iommu-platform/out/d-spdk-contract-20261015/documentation/receipt.json)。以下旧交接逐字节保留；历史方法/门槛措辞是当时记录。

'''
validation='''# 2026年10月15日 D-SPDK来源链与无计时原型核验

接受S来源/回调语义，M性能仍NOT_ADMITTED，N原生未运行。映射链与实际事件schema已取得，实际/联合trace各0；不推断真实leaf/共享部署。SC/SCT错误分类、完成数含错误及最终drain前elapsed边界已核。stock QD改变task池/翻译footprint，固定池admission/pacing是需实现和计费的普通扩展。

实现席运行一次新clockless合成原型生成/验证及一次独立CLI样例；root仅只读重消费已登记数据，生成器未重跑。8种callback次序、6个负变异拒绝、2个错误案例通过，N4/Q2是验证常量；结果仅为源语义，不是服务计时、NVMe硬件可达性、性能或新意PASS。条件服务模型须先冻结provider/资源/方向/动作、O0/付费Z、全费用/完整终点、跨边界持续性与留出；真实joint trace、原PC/SSD原生P/PN和预测反号不是所有显式研究投影的前置条件，实际部署/校准仍须相应实证。

0旧实验重跑/构建/原生DMA/Linux或RTL或UAPI修改/collector/FPGA或板操作；固定M0旧门、A5/A2及MIG暂停/旧时序失败/债务unknown保持。Codex-only，请求Sol high/xhigh/medium，实际运行元数据unknown。文档核验只查本地引用、前后hash、并发合并、A5/A2及独立事项原字节、交接/验证旧后缀，不作系统性能验证。

[完整入口](../out/d-spdk-contract-20261015/README.md)／[稳定接受](../out/d-spdk-contract-20261015/root/root-result-acceptance.json)／[原型结果](../out/d-spdk-contract-20261015/model/verification.json)／[root只读回放](../out/d-spdk-contract-20261015/root/semantic-replay-check.json)／[执行账本](../out/d-spdk-contract-20261015/root/execution-ledger.json)／[文档回执](../out/d-spdk-contract-20261015/documentation/receipt.json)。以下旧验证逐字节保留；不改写历史方法或门槛措辞。

'''
out['HANDOFF.md']=handoff+out['HANDOFF.md']; out['iommu-platform/docs/validation.md']=validation+out['iommu-platform/docs/validation.md']
readme='''# D：SPDK来源合同与无计时语义原型

2026-10-15。**来源链及无计时闭环语义已接受；服务性能模型仍NOT_ADMITTED。** 全局D主线/I轻量、A5保留及10月15—28日窗口不变，固定M0旧门不变。

## Material Passport

ARS-Codex有界来源核查与experiment-agent/plan。固定SPDK v24.09及其DPDK gitlink，Linux v6.12只作参考实现；来源版本/hash见下列索引。交付范围是映射/事件源码证据、显式研究投影合同及synthetic clockless原型，不是原生测量。Codex-only：证据席请求Sol/high、设计席Sol/xhigh、实现/文档席Sol/medium；实际运行元数据unknown，请求不作运行证明。未恢复用户删除的个人研究方法。

## 证据入口

| 内容 | 文件 |
| --- | --- |
| 映射/IOVA/VFIO/PRP链、QD与固定池边界 | [映射结论](mapping/findings.md) · [来源索引](mapping/sources.json) |
| SUBMIT/COMPLETE字段、成功与drain边界 | [事件结论](events/findings.md) · [来源索引](events/sources.json) |
| S/M/N准入、研究资源/动作/观测/全费用 | [投影设计](design/proposal.md) · [结构化评估](design/assessment.json) |
| 冻结源语义合同与原型 | [合同](root/semantic-prototype-contract.json) · [原型说明](model/README.md) · [验证结果](model/verification.json) · [执行回执](model/run-receipt.json) |
| root判断、原件核查及稳定接受 | [决定分析](root/decision-analysis.md) · [关键源核对](root/critical-source-check.json) · [稳定接受](root/root-result-acceptance.json) |
| 只读重消费、执行与文档完整性 | [root回放](root/semantic-replay-check.json) · [执行账本](root/execution-ledger.json) · [输入清单](root/input-manifest.json) · [文档回执](documentation/receipt.json) |

N4/Q2仅验证常量，8种接口允许callback次序、6个负变异拒绝、2个错误案例通过；不证明全部次序原生可达。root只读重消费已登记synthetic事件，未重跑生成器。实际/联合trace各0，无服务计时/原生/P-PN收益。SC/SCT定义错误，io_completed含错误；perf elapsed在最终drain前采样。stock QD改task池及翻译footprint，固定池admission/pacing须明确实现并计费。

下一关：冻结一个source-to-service投影的provider/参数域、资源/方向/动作、完整O0与付费Z、全费用/完整成功终点、跨quiet持续性、普通强对照和留出。来源绑定显式研究模型不以原PC/SSD原生P/PN、真实joint trace或预测反号为普遍前置条件；部署主张与校准仍需相应真实证据。每个动作独立演化完成→补发反馈。

本阶段0旧实验重跑/构建/原生DMA/Linux或RTL或UAPI修改/collector/FPGA或板操作。MIG暂停、旧时序失败与债务unknown保持。当前安排见[总计划](../../../研究计划.md)，历史M0来源门见[上一阶段](../d-source-gate-20261015/README.md)。
'''
# Assert no concurrent change occurred since the exact pre-write read.
for p in paths: assert (root/p).read_bytes()==before[p],f'concurrent update: {p}'
for p,text in out.items(): (root/p).write_text(text)
(run/'README.md').write_text(readme)
files={}
for p in paths:
 b=before[p]; a=(root/p).read_bytes(); stage=(run/'before'/p).read_bytes(); files[p]={'before_sha256':sha(b),'after_sha256':sha(a),'stage_start_sha256':sha(stage),'stage_start_differs_from_prewrite':stage!=b,'before_snapshot':'before-write/'+p,'before_bytes':len(b),'after_bytes':len(a)}
 if p in ['HANDOFF.md','iommu-platform/docs/validation.md']: files[p]['old_suffix_byte_identical']=a.endswith(b)
# CURRENT_STATUS preserved branch/tail and exact MIG sentence.
a=before['CURRENT_STATUS.md'].decode(); b=out['CURRENT_STATUS.md']
branch=a[a.index('## 各分支最新进展'):a.index('## 硬件与执行状态')]
tail=a[a.index('## 独立工程事项'):]
mig='原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。'
checks={'current_branch_bytes_preserved':branch in b,'independent_tail_bytes_preserved':tail in b,'current_MIG_sentence_bytes_preserved':mig in a and mig in b,'handoff_suffix_preserved':(root/'HANDOFF.md').read_bytes().endswith(before['HANDOFF.md']),'validation_suffix_preserved':(root/'iommu-platform/docs/validation.md').read_bytes().endswith(before['iommu-platform/docs/validation.md'])}
# Check new/changed entry links, excluding preserved historical suffixes.
missing=[]; count=0
texts={p:out[p] for p in ['CURRENT_STATUS.md','研究计划.md','研究导航.md']}; texts['HANDOFF.md']=handoff; texts['iommu-platform/docs/validation.md']=validation; texts['iommu-platform/out/d-spdk-contract-20261015/README.md']=readme
for p,t in texts.items():
 for dest in re.findall(r'\]\(([^)]+)\)',t):
  if '://' in dest or dest.startswith('#'):continue
  dest=dest.split('#')[0]; loc=(root/p).parent/dest; count+=1
  if not loc.exists() and not dest.endswith('documentation/receipt.json'): missing.append({'file':p,'target':dest})
checks['new_current_links_exist']=not missing
assert all(checks.values()),(checks,missing)
receipt={'schema':'d-spdk-documentation-receipt-v1','utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'D-current documentation only; root accepted research conclusions reused, no substantive research rerun','requested':{'model':'gpt-6.1-sol','reasoning':'medium'},'actual_runtime_metadata':'unknown','files':files,'new_readme_sha256':sha(readme.encode()),'checks':checks,'local_links_checked':count,'missing_links':missing,'concurrency_merge':{'read_immediately_before_write':True,'rechecked_before_apply':True,'changed_since_stage_start':[p for p,v in files.items() if v['stage_start_differs_from_prewrite']],'policy':'current pre-write bytes used; A5/A2 and independent tail preserved; historical handoff/validation exact suffix; root-plan method update retained'},'historical_files_modified':False,'old_method_restored':False,'research_model_or_generator_runs_by_documentation_seat':0,'final_targeted_integrity_check_owner':'root'}
(doc/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'files':list(files),'checks':checks,'local_links_checked':count,'concurrent_paths':receipt['concurrency_merge']['changed_since_stage_start']},ensure_ascii=False))
