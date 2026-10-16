from pathlib import Path
import hashlib, json, re, urllib.parse, datetime
W=Path('/home/xvhaocheng/work')
R=W/'iommu-platform/out/d-dram-action-20261015'
D=R/'documentation'
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
def sha(b): return hashlib.sha256(b).hexdigest()
def dump(p,v): p.write_text(json.dumps(v,ensure_ascii=False,indent=2)+'\n')
# Read immediately before mutation; preserve complete bytes, never reconstruct protected text.
before={p:(W/p).read_bytes() for p in paths}
assert not (D/'before/manifest.json').exists(), 'refuse to overwrite baseline'
for p,b in before.items():
    dest=D/'before'/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(b)
manifest={p:{'sha256':sha(b),'bytes':len(b)} for p,b in before.items()}
dump(D/'before/manifest.json',manifest)
protected_before={p:sha((W/p).read_bytes()) for p in ['AGENTS.md','iommu-platform/AGENTS.md']}
dump(D/'before/instruction-hashes.json',protected_before)
base='iommu-platform/out/d-dram-action-20261015/'
def chain(prefix=base):
    return '／'.join(f'[{label}]({prefix}{p})' for label,p in [('本轮入口','README.md'),('root裁决','root/decision-analysis.md'),('稳定接受','root/root-result-acceptance.json'),('下一最小合同','root/next-minimal-contract.json')])
conclusion='四例P/PN×row0/row1的完整COPY准备投影与独立核账通过，各完成V256/A257；同模式两前史在pre_t0与strong drain的有效DRAM状态、公开clock及next refresh相同，仅过期历史不同。准备过程六类轨迹不同，不能称同O_phys/O_host或整个RTL/IOTLB等价，也没有P/PN完整收益结论。局部停止这对前史的“准备后DRAM新增观测”版本，不准入完整P/PN动作比较。'
nextq='下一仅D-BOUNDARY-SOURCE-2，尚未执行：先从真实来源确认正常task/phase边界，分清一次试验初始化与每任务准备，核quiet/config/clear、完整费用、持久task/buffer及最早信息；比较同信息public history/layout预测器与一个具体可达的新关系。Linux可请求warm不授权降低固定M0 warm256追正例；若没有真实边界或剩余缺口，明确裁决D优先级，不无休止扫状态，不自动转I或A。'
execution='本轮4次新prepare-only run，0次完整64/64测量、旧实验重跑、生产Linux/RTL/ABI修改或硬件操作；私有host构建4次尝试、3次成功链接、1次失败，0次DRAM库或RTL构建。原MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究Codex-only；文档请求gpt-6.1-sol/medium，实际模型/强度元数据unknown。'
# Current status: preserve the complete A5/A2 block and independent engineering suffix.
s=before['CURRENT_STATUS.md'].decode()
s=s.replace(s.splitlines()[0],'# 当前状态：完整准备使选定DRAM前史对收敛，下一核真实任务边界',1)
start=s.index('- DRAM持续状态来源关已过：');end=s.index('\n- 旧MQSim',start)
s=s[:start]+'- '+conclusion+' '+nextq+' '+chain()+s[end:]
start=s.index('## 硬件与执行状态\n\n')+len('## 硬件与执行状态\n\n');end=s.index('## 独立工程事项',start)
s=s[:start]+execution+'\n\n'+s[end:]
after={'CURRENT_STATUS.md':s.encode()}
# Plan: short global summary; detailed current decision lives only in D section.
s=before['研究计划.md'].decode()
s=s.replace('DRAM来源关已过，下一核完整准备、write完成域及有限动作准入，固定M0不扩大','完整准备使选定DRAM前史对收敛，下一核真实task/phase边界，固定M0不扩大',1)
start=s.index('当前实际结果：');end=s.index('\n\n## D：',start)
s=s[:start]+'当前实际结果：四例完整准备投影通过，但同模式两前史在准备后有效DRAM状态相同，因此局部停止该前史对的观察器版本，不进入完整P/PN动作比较。下一仅核真实task/phase边界及剩余信息缺口，D主线、I轻量预筛与原立题窗口保持；细节见D节和[root裁决]('+base+'root/decision-analysis.md)。'+s[end:]
start=s.index('持续服务来源关保留SSD/DRAM来源事实；');end=s.index('\n\n## I：',start)
newd='持续服务来源关与旧MQSim局部停止保持：四冻结输入私有修复后完整完成，四个完整排空边界活动GC均0；停止本版cache-off有限覆写残留扩测，不否定整个D。[MQSim旧轮](iommu-platform/out/d-mqsim-identity-20261015/README.md)。上一DRAM轮只证明普通读前史在严格排空/idle后保留开行/cap，以及冲突warm、cap关行、全rank刷新可改写或洗去状态；保留[来源关证据](iommu-platform/out/d-dram-boundary-20261015/README.md)。\n\n'+conclusion+' 这是保留固定warm256、gap0、64/64批配置和原guard、在测量t0前退出的私有来源投影；B以真实WR+16数据末推导，末端另等WR+34恢复，不是库写回调或原生MIG/AXI完成保证。本轮4次prepare-only，完整64/64测量为0；准备耗时不作完整P/PN偏好证据。'+chain()+'\n\n'+nextq+' 只有真实边界和普通信息未解释的缺口获来源支持，才先冻结正常输入、两种合法动作、全部完成/guard/时序及同成本wait/replay对照，再谈完整动作实验；新增观测采集/传输/存储/CPU/切换费和应用完整生命周期偏好仍未知。常驻池、合法映射/准备、固定动作、完整公开历史、队深/poll和全费用等待/试选仍须同等供给；本轮不建collector或扩大固定M0。'
s=s[:start]+newd+s[end:];after['研究计划.md']=s.encode()
# Navigation: change only current-research prefix; assets and code map remain literal bytes.
s=before['研究导航.md'].decode()
oldstart=s.index('| D：');oldend=s.index('\n| I：',oldstart)
s=s[:oldstart]+'| D：有限观测→完整控制选择 | 主线；四例完整准备通过、同模式前史对有效DRAM状态收敛，局部停止该观察器版；下一核真实task/phase边界；MQSim/CS1局部停止保持 | '+chain()+' |'+s[oldend:]
start=s.index('DRAM持续状态来源关已过：');end=s.index('\n\n## 已有成果怎样复用',start)
s=s[:start]+conclusion+' '+nextq+' 上一[DRAM来源关](iommu-platform/out/d-dram-boundary-20261015/README.md)和[MQSim局部停止](iommu-platform/out/d-mqsim-identity-20261015/README.md)保留为资产；D主线、I轻量not_ready、A5/A2与10月15—28日窗口保持。'+s[end:]
after['研究导航.md']=s.encode()
# Historical logs: prepend only; preserve original bytes as exact suffix.
def record(prefix):
    return '# 2026年10月15日 D完整准备接受、局部停止与真实边界下一关\n\n方法：ARS-Codex有界来源投影与确定性核账；本次仅同步root稳定接受，不启动新调研或实验。'+conclusion+'\n\n'+execution+' D主线、I轻量not_ready、A5/A2、MQSim/CS1局部停止及10月15—28日窗口保持。\n\n'+nextq+'\n\n'+chain(prefix)+'／[独立核账]('+prefix+'verification/README.md)／[执行账本]('+prefix+'root/execution-ledger.json)／[文档保护回执]('+prefix+'documentation/receipt.json)。以下旧全文作为原字节suffix保留；旧记录中的“尚未执行”和工作席构建时快照按当时阶段阅读，不回填历史。\n\n'
after['HANDOFF.md']=record(base).encode()+before['HANDOFF.md']
after['iommu-platform/docs/validation.md']=record('../out/d-dram-action-20261015/').encode()+before['iommu-platform/docs/validation.md']
readme='''# D完整COPY准备：选定前史对局部停止

2026-10-15，D-DRAM-ACTION-CONTRACT-1。ARS-Codex有界来源投影与确定性正确性验证；共享研究Codex-only。文档请求gpt-6.1-sol/medium，实际model/effort元数据unknown。

'''+conclusion+'''

实际4次新prepare-only run，0次完整64/64测量，0旧重跑、生产修改或硬件操作；host构建4次尝试/3次成功/1次失败，0 DRAM库/RTL构建。固定M0 warm256/gap0/guards未放宽。B是私有WR+16推导数据末，另保留至WR+34恢复；不是原生完成保证。D主线、I轻量not_ready、A5/A2与10月15—28日窗口保持；MIG1792023314 paused、WNS−0.391ns/598 setup失败/0新load，实际债务unknown。

'''+nextq+'''

## 裁决与冻结合同

- [root完整裁决](root/decision-analysis.md)、[稳定接受](root/root-result-acceptance.json)、[下一最小合同](root/next-minimal-contract.json)。下一合同尚未执行，本轮前史对不进入完整P/PN动作比较。
- [事前冻结投影合同](root/preparation-projection-contract.json)、[原COPY准备核查](preparation/findings.md)、[准备合同](preparation/preparation-contract.json)。
- [写完成源级卡](completion/completion-card.md)、[桥接合同](completion/bridge-contract.json)、[root实现审查](root/implementation-review.json)。
- [有效状态比较](root/effective-state-comparison.json)、[phase摘录](root/phase-excerpts.json)、[分析更正](root/analysis-corrections.json)。仅DRAM服务域等价，不扩为整个RTL/IOTLB等价；准备六类轨迹不同。

## 私有实现与四例原始证据

[实现交付说明](implementation/README.md)是工作席的构建时快照，其中“本席未运行”“unknown”记录当时状态，已绑定在运行前输入中，不回写它；后续实际运行与接受以[root执行账本](root/execution-ledger.json)、[稳定接受](root/root-result-acceptance.json)和[核账报告](verification/README.md)为准。[私有C++夹具](implementation/preparation-harness.cpp)、[完整私有差异](implementation/private-source.patch)、[桥接实现](implementation/ramulator-bridge.cpp)、[构建回执](implementation/build-receipt.json)保留。

| 案例 | 实际运行回执 | 原始事件 | 官方命令 | pre_t0状态 | strong状态 | 原始输出 |
| --- | --- | --- | --- | --- | --- | --- |
'''
for c in ['p_row0','p_row1','pn_row0','pn_row1']:
    pref='implementation/runs/'+c+'/'
    readme+='| '+c+' | '+' | '.join('['+lab+']('+pref+f+')' for lab,f in [('receipt','run-receipt.json'),('CSV','bridge-events.csv'),('trace','official-trace.ch0'),('pre_t0','phase-pre_t0_after_configs.yaml'),('strong','phase-strong_final_drain.yaml'),('stdout','stdout.log')])+' |\n'
readme+='''
## 独立核账与执行来源

- [核账入口](verification/README.md)、[checker](verification/verify.py)、[正式报告](verification/report.json)、[实际执行回执](verification/execution-receipt.json)。四例已通过，5类负变异各在4例小状态被拒绝；查看结果无需重扫CSV。
- [raw绑定与六类轨迹首差](verification/evidence-bindings.json)、[历史有效性与源码](verification/history-and-source.json)、[服务数闭合](verification/service-bijections.json)。
- [root实际执行账](root/execution-ledger.json)、[64项运行前输入](root/run-inputs.json)、[文档保护与链接回执](documentation/receipt.json)、[文档核验说明](documentation/README.md)。
- [前轮DRAM来源关](../d-dram-boundary-20261015/README.md)、[MQSim局部停止](../d-mqsim-identity-20261015/README.md)、[唯一总计划](../../../研究计划.md)、[当前状态](../../../CURRENT_STATUS.md)。前轮来源保留不等于本轮完整准备后仍保留差异；历史未执行字段按当时记录阅读。
'''
# Compare latest files with immediate prewrite bytes: never clobber concurrent changes.
for p,b in before.items(): assert (W/p).read_bytes()==b, 'concurrent modification: '+p
for p,b in after.items(): (W/p).write_bytes(b)
assert not (R/'README.md').exists();(R/'README.md').write_text(readme)
# Receipt creation and checks are completed by the separate bounded checker.
dump(D/'sync-manifest.json',{'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'before':manifest,'after':{p:{'sha256':sha(b),'bytes':len(b)} for p,b in after.items()},'actual_model_effort':'unknown','requested_model_effort':'gpt-6.1-sol/medium','instructions_before':protected_before})
print(json.dumps({'written':list(after)+[str(R/'README.md')],'baseline_bytes':sum(len(b) for b in before.values())},ensure_ascii=False))
