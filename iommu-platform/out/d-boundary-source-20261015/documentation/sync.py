from pathlib import Path
import hashlib, json, re

W = Path('/home/xvhaocheng/work')
R = W / 'iommu-platform/out/d-boundary-source-20261015'
D = R / 'documentation'
if (D/'before-manifest.json').exists():
    raise SystemExit('One-shot sync already executed; preserve original before bytes. Use verify.py for read-only checks.')
names = ['CURRENT_STATUS.md', '研究计划.md', '研究导航.md', 'HANDOFF.md', 'iommu-platform/docs/validation.md']
# Read all targets immediately before mutation; these exact bytes are the merge base.
before = {n: (W/n).read_bytes() for n in names}
agents = {n: (W/n).read_bytes() for n in ['AGENTS.md','iommu-platform/AGENTS.md']}
sha = lambda b: hashlib.sha256(b).hexdigest()
for n,b in {**before, **agents}.items():
    p=D/'before'/n; p.parent.mkdir(parents=True,exist_ok=True); p.write_bytes(b)
(D/'before-manifest.json').write_text(json.dumps({n:{'bytes':len(b),'sha256':sha(b)} for n,b in {**before,**agents}.items()},ensure_ascii=False,indent=2)+'\n')

base='iommu-platform/out/d-boundary-source-20261015'
links=f'[本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json)／[下一最小合同]({base}/root/next-minimal-contract.json)'
next_text='下一仅RESEARCH-REPRIORITIZE-1，尚未执行：把本轮新事实带回已核六族，按具名真实问题选择2–3个最强版本，做有限正向设计与投入重排；不预设A/I/D或名单，不重复全面调研，不自动实现。本轮不进入新增adapter/sensor或模型扩测；后继验证按获选版本冻结具体合同，原有授权与暂停状态保持。'
facts='本机composition每次ioctl重新alloc/init/map/warm，结束capture→restore S/DID10→unmap/free；E3/SVA未补出目标持久控制路径。stock SPDK普通callback保留task/buffer并立即补交，warm只清统计且不断供，terminal drain释放task/buffer/qpair，均不等于保池joint quiet→config→resume。硬件成功配置要求整体quiet，并清IOTLB valid与PLRU；这不是DDR reset。'
decision='D“上一窗口有限观测→下一任务P/PN选择”的当前版本降为待新证据触发的候选，停止自动新增adapter/sensor、状态扫测和完整动作扩测；暂未选替代主线。'
status=f'''# 当前状态：D当前选策版本降优先级，下一重排研究投入

2026年10月15日。**{decision}** I保持轻量not_ready，A5/A2及独立工程成果保留；10月15—28日立题窗口保持，论文尚未完成。[研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)是当前地图。

## 全局安排与下一证据

- {decision}这是本轮有界投入裁决，不是D整个思想族被证明无价值。{next_text} {links}
- {facts} 可新增普通keepalive adapter的设计未实现、费用未知，接口本身不算创新；带owner身份的PTE等待—payload服务联合关系，跨边界的新信息与完整收益仍未证。[硬件边界]({base}/root/hardware-boundary.md)／[两种设计比较]({base}/root/mechanism-comparison.md)。
- 前轮四例完整准备及所选前史对收敛、MQSim/CS1具体版本的局部停止均保留，不重跑、不外推整个D。[四例结果](iommu-platform/out/d-dram-action-20261015/README.md)／[MQSim旧轮](iommu-platform/out/d-mqsim-identity-20261015/README.md)。
- I有条件strict DMA后端，FQ因能力缺项回退strict；当前保存config/DTS没有ixgbe原生链，实际部署/live预算/联合trace仍缺，not_ready。保留精确失效、合法合批、持久映射与分domain对照；D降优先级不自动提升I或退回A。[I后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)。
- 入口统一和12份计划物理归档已完成，原字节/hash/行号与迁移前坏链记录保留。[归档索引](iommu-platform/docs/history/plans/README.md)／[首批交付](iommu-platform/out/research-plan-implementation-20261015/README.md)。

'''
s=before[names[0]].decode(); protected=s[s.index('## 各分支最新进展'):s.index('## 硬件与执行状态')]; independent=s[s.index('## 独立工程事项'):]
status+=protected+f'''## 硬件与执行状态

本轮SOURCE_ONLY：0构建、0模型/源程序实验、0旧实验重跑、0设备/硬件操作、0生产Linux/RTL/ABI修改、0联网；[前轮四例结果](iommu-platform/out/d-dram-action-20261015/README.md)原样保留。MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。共享研究Codex-only；文档请求gpt-6.1-sol/medium，实际模型/强度元数据unknown。

'''+independent

plan=f'''# 共享IOMMU研究总计划

2026年10月15日，按本轮稳定接受：**{decision}** I维持轻量not_ready，A5/A2及其他资产保留。立题窗口仍为10月15—28日，不重新起算。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支局部合同不能自行替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按当前阶段选择工作流；来源/条件预测不当测量结论，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 既有六族、D来源与局部停止证据保留；下一做RESEARCH-REPRIORITIZE-1有限正向设计 | 按具名真实问题选2–3最强版本，不按旧排名或投入选方向 |
| 10月18—19日：首个决定性判别 | 随重排结果冻结一个能改变投入判断的最小证据合同 | 来源、普通强对照、信息时机、完整费用和终点明确后另行决定执行 |
| 10月20—24日：通过后扩大 | 对获选版本按冻结规则验证未见任务/状态 | 取得新证据并通过相应准入；不自动扩D模型或实现adapter/sensor |
| 10月25—28日：研究裁决 | 贡献论证、前作差异、证据矩阵、下一最小合同 | 明确推进、重设计或降低优先级；缺证和模型成功都不写成论文完成 |

本轮真实边界核查已完成，现有native/SPDK路径未提供目标保池quiet→config→resume边界，跨边界关系的额外决策价值与费用仍缺，因此降低D当前版本的投入优先级。下一仅做候选正向设计与投入重排，未选替代主线；I轻量not_ready与原立题窗口保持。{links}。

## D：保留有限观测选策idea，等待有意义的新证据

真实问题仍是共享翻译与payload反馈下，额外关系信息是否改变完整任务控制选择；本轮只降低“上一窗口→下一任务P/PN”当前版本的优先级，不否定整个D。{facts} 具体来源、初始化/每任务准备及最早信息见[native卡]({base}/native/boundary-card.json)与[持久来源卡]({base}/persistent/episode-source-card.md)。新keepalive adapter可以设计，但未实现、费用未知，保池暂停接口本身不构成创新。

同信息普通强对照获得完整合法public history、布局/映射、任务结构及已有计数；正常重复来源成立时纳入同费用完整试选。root比较了它与带owner身份的PTE等待—payload服务联合关系；后者是否在合法边界后仍增加信息、改变完整P/PN结果并覆盖全部费用，本轮均未证明。[机制比较]({base}/root/mechanism-comparison.md)分别列出能力、成本和可推翻条件，不能以“普通程序可想象做到”宣称整体被覆盖。

再进入条件：有具体正常边界，或来源绑定、假设冻结且动作合法的研究投影；有public history/layout及同费用试选尚未解释的具体跨边界关系；信息及时可得并计入adapter、观测、config/clear/warm与完整生命周期费用。不要求建模前先有原生反号或joint trace，也不能用任意隐状态代替来源。

固定M0的64/64、gap0、warm256与guards不变；四例完整准备只支持选定前史对在准备后有效DRAM状态收敛，未证明所有状态或整个RTL/IOTLB等价，也未取得完整P/PN收益。CS1/MQSim各具体版本的局部停止保持，历史来源与实验结果经[前轮入口](iommu-platform/out/d-dram-action-20261015/README.md)、[CS1](iommu-platform/out/d-service-model-20261015/README.md)及[MQSim](iommu-platform/out/d-mqsim-identity-20261015/README.md)定位，不重跑或扩大固定M0。

{next_text} [六族独立比较](iommu-platform/docs/independent-research-directions-20261015.md)保留已核阅读边界，不重复全面调研；本轮投入裁决与再进入条件以[稳定接受]({base}/root/root-result-acceptance.json)为准。

'''+before[names[1]].decode().split('## I：',1)[1].join(['## I：',''])
# Preserve I and every later section as an exact suffix.
plan=plan[:plan.index('## I：')]+before[names[1]].decode()[before[names[1]].decode().index('## I：'):]
nav=f'''# 共享IOMMU研究导航

2026年10月15日。D当前下一任务选策版本降优先级，暂未选替代主线；下一有限正向设计与投入重排。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；阶段背景查[HANDOFF](HANDOFF.md)，判定方法查[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)。按当前阶段选择工作流，来源/条件预测不当测量结论，不默认全pipeline。

## 当前研究与下一关口

| 方向 | 当前安排 | 入口 |
| --- | --- | --- |
| D：有限观测→完整控制选择 | 当前“上一窗口→下一任务P/PN”版本降为待新证据候选；停止自动adapter/sensor、状态扫测及完整动作扩测 | [本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json) |
| 下一研究决定 | RESEARCH-REPRIORITIZE-1尚未执行：基于真实问题选2–3最强版本正向设计与投入重排；不预设方向或实现 | [下一最小合同]({base}/root/next-minimal-contract.json)／[六族比较](iommu-platform/docs/independent-research-directions-20261015.md) |
| I：失效粒度/live DMA/回收 | 轻量not_ready；strict存在/FQ回退，固定profile无ixgbe原生链，部署/live预算仍缺；不自动提升 | [后端门](iommu-platform/out/d-source-gate-20261015/i-backend/findings.md)／[预筛](iommu-platform/out/research-plan-implementation-20261015/i/prescreen.md) |
| A：完成域与等待范围 | A5/A2资产保留，分支进展不改写全局排序，不自动恢复A优先 | [A5设计](iommu-platform/docs/a5-wait-scope-contract-20261015.md)／[接受](iommu-platform/out/a5-wait-scope-contract-20261015/root-result-acceptance.json)／[局部合同](iommu-platform/docs/research-execution-plan-20261011.md) |
| B1 / Q / G及更早候选 | 按新事实保留/重审，不自动恢复关闭实验 | [完整比较证据](iommu-platform/out/independent-directions-20261015/README.md) |

本轮来源表明native每次重新准备与释放，stock SPDK连续补交/统计warm/terminal drain也未给出保池联合quiet→config→resume；硬件配置清IOTLB valid/PLRU，不是DDR reset。公开history/layout/同费用试选与owner绑定PTE—payload关系的比较见[机制卡]({base}/root/mechanism-comparison.md)；跨边界新增信息与完整收益仍未证。当前投入裁决不等于D整个思想族无价值；I、A5/A2及10月15—28日窗口保持。前轮[四例完整准备](iommu-platform/out/d-dram-action-20261015/README.md)、[DRAM来源](iommu-platform/out/d-dram-boundary-20261015/README.md)与[MQSim局部停止](iommu-platform/out/d-mqsim-identity-20261015/README.md)保留。

'''+before[names[2]].decode()[before[names[2]].decode().index('## 已有成果怎样复用'):]

def record(prefix):
    def lk(label,rel): return f'[{label}]({prefix}{rel})'
    return f'''# 2026年10月15日 D真实边界来源接受与投入重排

方法：ARS-Codex有界来源核查与机制比较；本次仅同步root稳定接受。{decision}这是累计来源/费用缺口下的有界投入裁决，不是整个D被证明无价值。{facts}

同信息public history/layout/付费试选须与owner绑定PTE—payload关系公平比较；跨边界新增信息、完整收益和新增adapter费用仍未证，普通保池暂停接口本身不算创新。{next_text} I轻量not_ready、A5/A2与10月15—28日窗口保留。

本轮SOURCE_ONLY：0构建、0模型/源程序实验、0旧重跑、0设备/硬件操作、0生产修改、0联网。MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载采集、实际债务unknown。共享研究Codex-only；文档请求gpt-6.1-sol/medium，实际模型/强度unknown。五文件写前完整字节/hash、指定四块、两历史全文suffix及双AGENTS保护见文档回执；以下历史全文原字节保留，不回填旧记录。

{lk('本轮入口','README.md')}／{lk('root裁决','root/decision-analysis.md')}／{lk('稳定接受','root/root-result-acceptance.json')}／{lk('下一合同','root/next-minimal-contract.json')}／{lk('执行账','root/execution-ledger.json')}／{lk('文档回执','documentation/receipt.json')}。

'''
outputs={names[0]:status.encode(),names[1]:plan.encode(),names[2]:nav.encode(),names[3]:record(base+'/').encode()+before[names[3]],names[4]:record('../out/d-boundary-source-20261015/').encode()+before[names[4]]}
for n,b in outputs.items():
    assert (W/n).read_bytes()==before[n], f'Concurrent change: {n}'
    (W/n).write_bytes(b)
readme='''# D-BOUNDARY-SOURCE-2：边界来源与投入裁决

D“上一窗口有限观测→下一任务P/PN”当前版本降优先级，保留为待新证据候选；暂未选替代主线。下一仅RESEARCH-REPRIORITIZE-1有界正向设计与投入重排，尚未执行。

- [root决定](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[下一最小合同](root/next-minimal-contract.json)
- [实际硬件边界](root/hardware-boundary.md)／[正向机制比较](root/mechanism-comparison.md)
- [native findings](native/findings.md)／[边界机器卡](native/boundary-card.json)／[精确原文](native/source-evidence.md)／[原文manifest](native/source-manifest.json)
- [持久来源findings](persistent/findings.md)／[episode来源卡](persistent/episode-source-card.md)／[精确原文](persistent/source-evidence.md)／[原文manifest](persistent/source-manifest.json)
- [硬件原文manifest](root/hardware-source-manifest.json)／[关键源码审查](root/critical-source-review.json)／[执行账](root/execution-ledger.json)
- [前轮四例完整准备](../d-dram-action-20261015/README.md)／[文档保护与链接回执](documentation/receipt.json)／[写前完整字节/hash](documentation/before-manifest.json)

本轮SOURCE_ONLY：0构建、0模型/源程序实验、0旧重跑、0设备/硬件操作、0生产修改、0联网。I轻量not_ready、A5/A2、10月15—28日窗口与MIG暂停保持。共享研究Codex-only；文档请求gpt-6.1-sol/medium，实际模型/强度unknown。
'''
(R/'README.md').write_text(readme)

def span(b,start,end=None):
    x=b.index(start.encode()); return b[x:b.index(end.encode(),x)] if end else b[x:]
checks={
 'current_A5_A2_block':span(outputs[names[0]],'## 各分支最新进展','## 硬件与执行状态')==span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态'),
 'current_independent_suffix':outputs[names[0]].endswith(span(before[names[0]],'## 独立工程事项')),
 'plan_I_and_later_suffix':outputs[names[1]].endswith(span(before[names[1]],'## I：')),
 'navigation_assets_and_later_suffix':outputs[names[2]].endswith(span(before[names[2]],'## 已有成果怎样复用')),
 'handoff_full_original_suffix':outputs[names[3]].endswith(before[names[3]]),
 'validation_full_original_suffix':outputs[names[4]].endswith(before[names[4]]),
 **{f'unchanged_{n}':(W/n).read_bytes()==b for n,b in agents.items()},
 **{f'before_bytes_{n}':(D/'before'/n).read_bytes()==b for n,b in before.items()},
 **{f'written_bytes_{n}':(W/n).read_bytes()==b for n,b in outputs.items()},
}
pattern=re.compile(r'\[[^\]\n]*\]\(([^)\n]+)\)')
def local_links(text):
    return [x.split('#',1)[0].strip('<>') for x in pattern.findall(text) if not re.match(r'^[A-Za-z][A-Za-z0-9+.-]*:',x) and not x.startswith('#')]
new_links=[]; old_bad=[]
for n,b in outputs.items():
    old=set(local_links(before[n].decode()))
    for target in sorted(set(local_links(b.decode()))):
        p=Path(target) if target.startswith('/') else (W/n).parent/target
        valid=p.exists()
        if target not in old: new_links.append({'file':n,'target':target,'exists':valid})
        elif not valid: old_bad.append({'file':n,'target':target})
for target in sorted(set(local_links(readme))):
    new_links.append({'file':str((R/'README.md').relative_to(W)),'target':target,'exists':(R/target).exists() or target=='documentation/receipt.json'})
# The receipt is created below, then its existence is checked separately.
for item in new_links:
    if item['target'].endswith('documentation/receipt.json'): item['exists']=True
checks['all_new_local_links_exist']=all(x['exists'] for x in new_links)
receipt={'task':'D-BOUNDARY-SOURCE-2 documentation only','configuration':{'requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown'},'before_manifest':'before-manifest.json','checks':checks,'passed':all(checks.values()),'check_count':len(checks),'new_local_link_count':len(new_links),'new_local_links':new_links,'preexisting_broken_local_links':old_bad,'preexisting_broken_count':len(old_bad),'executions':{'builds':0,'models_or_source_program_experiments':0,'old_reruns':0,'device_hardware':0,'production_edits':0,'network':0},'after':{n:{'bytes':len(b),'sha256':sha(b)} for n,b in outputs.items()}}
(D/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
assert (D/'receipt.json').exists()
assert receipt['passed'], json.dumps(checks)
print(json.dumps({'passed':receipt['passed'],'checks':len(checks),'new_links':len(new_links),'preexisting_broken':len(old_bad)},ensure_ascii=False))
