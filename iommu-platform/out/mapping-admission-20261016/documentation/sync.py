from pathlib import Path
import hashlib,json,re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-admission-20261016';D=R/'documentation'
if (D/'before-write-manifest.json').exists():raise SystemExit('One-shot sync already executed; preserve before-write originals.')
names=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'];agents=['AGENTS.md','iommu-platform/AGENTS.md']
# Reread current bytes immediately before writing; never merge from root's start snapshot.
before={n:(W/n).read_bytes() for n in names+agents};sha=lambda b:hashlib.sha256(b).hexdigest()
roots=['decision-analysis.md','next-minimal-contract.json','root-result-acceptance.json','source-and-ledger-verification.json','execution-ledger.json']
stable={n:sha((R/'root'/n).read_bytes()) for n in roots}
exception=json.loads((R/'root/root-result-acceptance.json').read_text())['AGENTS_allowed_exception']
old=exception['old'].encode();new=exception['new'].encode();assert before['AGENTS.md'].count(old)==1
agents_after=before['AGENTS.md'].replace(old,new)
for n,b in before.items():
 p=D/'before-write'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b)
(D/'before-write-manifest.json').write_text(json.dumps({n:{'bytes':len(b),'sha256':sha(b)} for n,b in before.items()},ensure_ascii=False,indent=2)+'\n')
def span(b,s,e=None):
 i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
base='iommu-platform/out/mapping-admission-20261016'
links=f'[本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json)／[下一合同]({base}/root/next-minimal-contract.json)'
decision='用户收窄后的路由映射接纳来源与有限费用判别已完成；一般copy/短map/持久接纳问题保留，当前累计超额版本归普通O3，不认领独立新控制机制。暂未选论文主线。'
contract='本轮已冻结T分析合同：相关专用范围按规定权限持续授权；S仅条件分支。实际部署/设备信任及论文主要价值未确认，不能将分析合同写成部署已经选定。'
next_text='下一仅ADMISSION-FULL-COST-RESIDUAL-1，由具体输入或设计变化触发、尚未执行；核O3之外的可部署能力或完整成本残余，不重复同账，不自动开发完整系统/性能模型。'
source='官方v6.12独立快照支持A page_pool常驻、B额外map/TX/unmap与本机UDP条件链；pp_ref_count须按新版本处理，B map先于最终SQ接受。原DMA_PMD整PMD RW不等于READ-only，后者须计普通适配成本；没有把旧本地Linux、RFC和v6.12拼成原生移植。'
ledger='3个PMD源区、1/1/8再用、顺序回源与全TX接受均为条件投影，非实测。O0/O1/O2/O3/离线小计40/30/29/27/25只是说明CPU单位；K、h及源大页/关联/退役等未测差项不能设0，27不是净赢。当前版本按定义就是O3，未定义或独立测试另一个controller，不据此否定整个idea。'
keep='旧MAPPING-LEASE-PROTOCOL-1不按旧合同执行，历史保留；候选2暂不展开。B1待具体残余/成本证据，I not_ready，D既有版本不升位，A5/A2等资产与10月15—28日窗口保持。'
execution='本轮保存32个v6.12文件及1份RFC镜像，共33项HTTP；有限账脚本实际执行3次，root独立标量核算1次。57项来源hash/57路径与独立标量核算通过，只核来源/费用账，非驱动/IOMMU正确性或性能。0性能/cycle模型、0原生应用/驱动运行、0构建、0旧实验重跑、0生产编辑、0设备操作。'
hardware='MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。'
config='共享研究Codex-only，未用DeepSeek/Claude；请求DAMN/RFC及v6.12源核Sol/high、费用账与文档Sol/medium，实际独立模型/强度元数据unknown。'

current=f'''# 当前状态：映射接纳有限判别完成，待完整费用残余

2026年10月16日（JST）。**{decision}** {contract} [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产；论文尚未完成。

## 全局安排与下一证据

- {next_text} {links}。
- {source} DAMN已有设备/权限池、读取点复制及huge/dense分析；dense变体不能free、shrinker未实装，不把已有方法或实现缺项直接当新意。[v6.12链]({base}/v612-source/findings.md)／[DAMN核读]({base}/damn/findings.md)。
- {ledger} [事件与费用账]({base}/ledger/README.md)／[实际结果]({base}/ledger/result.json)。在线只用当时可知信息，未来再用r为离线下界输入；CPU、物理字节、带宽与完成约束分别报告。
- {keep} B1结题不等于预热足够或全族无价值，不重复卡。[B1保留入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[旧整页提案历史](iommu-platform/out/mapping-organization-source-20261016/README.md)。

'''.encode()+span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态')+f'''## 硬件与执行状态

{execution} [来源与费用账核验]({base}/root/source-and-ledger-verification.json)／[执行账]({base}/root/execution-ledger.json)。{hardware} {config}

'''.encode()+span(before[names[0]],'## 独立工程事项')

plan=f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{decision}** {contract} {next_text} 10月15—28日窗口不重新起算，既有授权与暂停状态保持。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支局部合同不替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按阶段使用；来源/条件预测不当测量结论，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 用户收窄、v6.12链、DAMN与有限费用账完成；累计版本归普通O3 | 不重复同阈值账，保持来源投影/测量、T/S和部署身份分开 |
| 10月18—19日：首个决定性判别 | 仅随ADMISSION-FULL-COST-RESIDUAL-1具体输入或设计变化冻结一个后继 | 完整差项或正常约束能带来O3之外的能力/成本残余，未知项不填0 |
| 10月20—24日：通过后扩大 | 对具名残余给正向实现、同信息普通强对照与完整费用正反范围 | 不自动开发池/系统/性能模型；若主张IOMMU或consumer收益，需访问/服务证据 |
| 10月25—28日：研究裁决 | 贡献论证、前作差异、证据矩阵与后续最小合同 | 无差异可止于普通部署建议，不把单阈值/手工公式称方法论文 |

本轮有限判别已完成，源区再用可使普通累计规则不同于首次建图或同包长固定copy选择；未测全费用差项可能吃掉说明差额。一般接纳问题保留，未认领新算法或净收益；旧整页转交后继不执行，下一仅由具体变化触发。{links}。

## 路由后source extent×generation×domain×rights的组织

{source} stock O0源RX池+B逐次map须保留正确批处理、IOVA缓存；O1目标持久紧凑池+copy、O2首次使用持久建图、O3同信息普通三选均计真实适配。O1新copy与原frame回调owner须唯一；O3短map需要合法bypass，不能沿用pool-window no-op unmap冒充撤权。源区退一般分配器或信任变化前必须清B alias。[官方v6.12来源]({base}/v612-source/findings.md)／[生命周期卡]({base}/v612-source/lifecycle-card.json)。

{contract} 原RFC RW若直接使用须满足T_RW；READ-only T要计普通权限适配。PASS不强制撤A映射，packet id不是源物理generation；别名不是两份物理内存，较早回源池也不等于总内存减少。S不能混用本轮T费用表。

DAMN已有device/rights分配、常驻池、读取点复制、零复制IP转发回DMA API及huge/dense翻译footprint分析；其native XDP读取适配不能免费假定。shrinker未实现、dense变体不能free既限制可部署对照，也不自动产生新意。此前shadowing/Mosaic/Ingens/fbufs已核方法复用，本轮未证明全领域没有类似admission规则。[DAMN实质比较]({base}/damn/findings.md)。

{ledger} F包含建立和最终撤销CPU费用，接纳时预记不代表撤销已发生；目标池K只有实际共享生命周期时可抵消，O3查询h及源PMD取得/保留/集成不得免费。O3在本例胜O2的未测额外CPU差项须小于2个说明单位，胜O1须小于3；这不是周期、内存或吞吐收益。[冻结分析合同]({base}/root/finite-ledger-contract.md)／[账本]({base}/ledger/README.md)。

{next_text} [后继合同]({base}/root/next-minimal-contract.json)允许测量或假设冻结来源投影，不要求先有原生胜例。若声称共享IOMMU竞争/consumer完成收益，需(domain,IOVA leaf)访问与服务关系，不能从映射数推miss。若无O3之外差异，可保留普通部署建议，不否定更一般idea。

## 保留候选与既有边界

{keep} B1单条带已结题、未证普通预热足够或B1全族无价值，具体再进入合同保持。[B1入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[B1再进入](iommu-platform/out/b1-stripe-cost-20261016/root/next-minimal-contract.json)。不执行[历史整页后继](iommu-platform/out/mapping-organization-source-20261016/root/next-minimal-contract.json)，不自动回退旧方向或恢复关闭实验。

'''.encode()+span(before[names[1]],'## I：')

nav=f'''# 共享IOMMU研究导航

2026年10月16日（JST）。{decision} T本轮分析合同已冻结，S仅条件；实际部署与论文主要价值未确认。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；背景查[HANDOFF](HANDOFF.md)，方法查[ARS-Codex入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)。

## 当前研究与下一关口

| 版本/方向 | 当前安排 | 入口 |
| --- | --- | --- |
| 路由后映射接纳 | 官方v6.12/DAMN/有限费用判别完成；一般三选问题保留，累计版本即普通O3，无独立controller实验 | [本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md) |
| 下一完整费用残余 | ADMISSION-FULL-COST-RESIDUAL-1仅具体输入/设计变化触发，未执行，不重复同账 | [下一合同]({base}/root/next-minimal-contract.json) |
| 官方v6.12来源 | 独立源码快照，pp_ref_count/B map先于SQ接受/条件本地UDP；非生产树更新或运行身份 | [来源报告]({base}/v612-source/findings.md)／[manifest]({base}/v612-source/source-manifest.json) |
| DAMN与RFC权限 | DAMN已有huge/dense但不能free、shrinker未实现；原RFC PMD RW不等READ-only | [方法核读]({base}/damn/findings.md)／[比较卡]({base}/damn/comparison-card.json) |
| 有限条件费用账 | 1/1/8再用、3PMD、全TX接受为投影；40/30/29/27/离线25为说明CPU单位，K/h等未测 | [账与限制]({base}/ledger/README.md)／[结果]({base}/ledger/result.json) |
| 旧整页协议/候选2 | MAPPING-LEASE-PROTOCOL-1不执行、历史保留；候选2暂不展开 | [前轮历史](iommu-platform/out/mapping-organization-source-20261016/README.md) |
| B1 / I / D / A5A2 | B1具体证据触发，I not_ready，D既有边界，分支资产保留 | [B1入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md) |

57sourcehash/57路径与独立标量核算通过，非协议正确性/性能证明。33项HTTP保存，有限账脚本实际3次+root标量1次；0性能模型/原生运行/构建/旧重跑/生产编辑/设备操作。MIG暂停、原有授权和10月15—28日窗口保持。[稳定接受]({base}/root/root-result-acceptance.json)／[执行账]({base}/root/execution-ledger.json)。

'''.encode()+span(before[names[2]],'## 已有成果怎样复用')

def record(prefix):
 def lk(t,p):return f'[{t}]({prefix}{p})'
 return f'''# 2026年10月16日 路由后映射接纳：普通O3与完整费用边界

方法：ARS-Codex定向事实核查、实质方法比较和有限参数判别；本次同步root稳定接受，不开fullpipeline。{decision} {contract} {next_text}

{source} DAMN已有huge/dense分析但dense不能free、shrinker未实现。{ledger} 不把按定义的同规则当两种独立控制器实验；source extent×generation×domain×rights问题仍保留。

{execution} {hardware} {config} {keep} 根AGENTS仅按稳定接受指定单句，把过时“D是当前安排”改为当前安排以总计划为准；其余原字节与平台AGENTS全文保留。五目标写前字节/hash、四保护块和两整历史suffix见回执；以下历史全文原样保留，不回填旧记录。

{lk('本轮入口','README.md')}／{lk('root裁决','root/decision-analysis.md')}／{lk('稳定接受','root/root-result-acceptance.json')}／{lk('下一合同','root/next-minimal-contract.json')}／{lk('有限账','ledger/README.md')}／{lk('执行账','root/execution-ledger.json')}／{lk('文档回执','documentation/doc-change-manifest.json')}。

'''.encode()
outputs={names[0]:current,names[1]:plan,names[2]:nav,names[3]:record(base+'/')+before[names[3]],names[4]:record('../out/mapping-admission-20261016/')+before[names[4]],'AGENTS.md':agents_after}
for n,b in outputs.items():
 assert (W/n).read_bytes()==before[n],f'Concurrent change: {n}'
 (W/n).write_bytes(b)
readme='''# 路由后映射接纳／复制：来源与有限费用判别

2026-10-16 JST。用户收窄后的官方v6.12链、DAMN与有限费用判别已完成。T为本轮冻结分析合同，S条件分支；实际部署/信任与论文主要价值未确认。一般source extent×generation×domain×rights的copy/短map/持久接纳问题保留，具体累计超额版本即普通O3，不认领独立新控制机制，未选论文主线。

- [用户收窄合同](root/steering-contract.md)／[root裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[下一ADMISSION-FULL-COST-RESIDUAL-1](root/next-minimal-contract.json)
- [官方v6.12来源](v612-source/findings.md)／[生命周期卡](v612-source/lifecycle-card.json)／[源码manifest](v612-source/source-manifest.json)／[取得manifest](v612-source/download-manifest.json)
- [DAMN实质核读](damn/findings.md)／[比较卡](damn/comparison-card.json)／[来源manifest](damn/source-manifest.json)／[RFC镜像比较](damn/mirror-comparison.json)
- [冻结有限账合同](root/finite-ledger-contract.md)／[账与边界](ledger/README.md)／[输入事件](ledger/events.json)／[费用脚本](ledger/fee_ledger.py)／[结果](ledger/result.json)
- [57sourcehash及独立标量核验](root/source-and-ledger-verification.json)／[实际执行账](root/execution-ledger.json)
- [文档保护/精确AGENTS例外/链接回执](documentation/doc-change-manifest.json)／[完整写前字节/hash](documentation/before-write-manifest.json)／[旧整页提案历史](../mapping-organization-source-20261016/README.md)

3PMD、源区1/1/8再用和全部TX接受为条件投影，非真实布局/流量。40/30/29/27/离线25是说明CPU小计，K/h等完整差项未知，不能设0宣称净赢；没有另一个独立controller或等价实验。官方v6.12 pp_ref_count与map先于SQ接受已核；原RFC整PMD RW不等READ-only。DAMN已有huge/dense但不能free、shrinker未实装，缺项不自动构成新意。

下一仅由具体输入或设计变化触发，未执行，不重复同账、不完整系统或性能模型。旧MAPPING-LEASE-PROTOCOL-1不执行、历史保留；候选2不展开。B1、I/D/A5A2、MIG暂停及原窗口保持。

本轮33项HTTP保存、费用脚本3次、root独立标量1次，0性能/cycle模型、原生运行、构建、旧重跑、生产编辑和设备操作。Codex-only，未用DeepSeek/Claude，实际模型/强度unknown。最终交付状态以root单独保存的收尾回执为准。
'''
(R/'README.md').write_text(readme)
checks={
 'current_A5_A2_exact':span(outputs[names[0]],'## 各分支最新进展','## 硬件与执行状态')==span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态'),
 'current_independent_exact':span(outputs[names[0]],'## 独立工程事项')==span(before[names[0]],'## 独立工程事项'),
 'plan_I_and_later_exact':span(outputs[names[1]],'## I：')==span(before[names[1]],'## I：'),
 'navigation_assets_and_later_exact':span(outputs[names[2]],'## 已有成果怎样复用')==span(before[names[2]],'## 已有成果怎样复用'),
 **{f'full_old_suffix_{n}':outputs[n].endswith(before[n]) for n in names[3:]},
 'root_AGENTS_exact_authorized_sentence':(W/'AGENTS.md').read_bytes()==agents_after and agents_after.replace(exception['new'].encode(),exception['old'].encode())==before['AGENTS.md'],
 'platform_AGENTS_full_exact':(W/'iommu-platform/AGENTS.md').read_bytes()==before['iommu-platform/AGENTS.md'],
 **{f'before_bytes_and_hash_{n}':(D/'before-write'/n).read_bytes()==before[n] for n in names+agents},
 **{f'written_bytes_{n}':(W/n).read_bytes()==b for n,b in outputs.items()},
 **{f'stable_root_{n}':sha((R/'root'/n).read_bytes())==h for n,h in stable.items()},
}
pattern=re.compile(r'\[[^\]\n]*\]\(([^)\n]+)\)')
def targets(s):return {t.split('#',1)[0].strip('<>') for t in pattern.findall(s) if not re.match(r'^[A-Za-z][A-Za-z0-9+.-]*:',t) and not t.startswith('#')}
def exists(n,t):
 t=re.sub(r':\d+(?::\d+)?$','',t);return (Path(t) if t.startswith('/') else (W/n).parent/t).exists()
new=[];old_bad=[]
for n,b in outputs.items():
 old=targets(before[n].decode())
 for t in sorted(targets(b.decode())):
  e={'file':n,'target':t,'exists':exists(n,t)}
  if t not in old:new.append(e)
  elif not e['exists']:old_bad.append(e)
rn=str((R/'README.md').relative_to(W))
for t in sorted(targets(readme)):new.append({'file':rn,'target':t,'exists':exists(rn,t)})
rp=D/'doc-change-manifest.json';rp.write_text('{}\n')
for e in new:e['exists']=exists(e['file'],e['target'])
checks['all_new_local_links_exist']=all(e['exists'] for e in new)
r={'task':'mapping-admission-20261016 documentation delivery','local_date':'2026-10-16 JST','configuration':{'requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown'},'checks':checks,'check_count':len(checks),'passed':all(checks.values()),'before_write_manifest':'before-write-manifest.json','after':{n:{'bytes':len(b),'sha256':sha(b)} for n,b in outputs.items()},'final_README':{'path':rn,'bytes':len(readme.encode()),'sha256':sha(readme.encode())},'stable_root_input_hashes':stable,'AGENTS_exact_allowed_exception':exception,'link_check_method':'unique (document,target) pairs; fragment stripped; :line[:column] normalized; newly introduced target per document/all new README links; history retained','new_local_links':new,'new_local_link_count':len(new),'preexisting_broken_local_links':old_bad,'preexisting_broken_count':len(old_bad),'execution':{'documentation_checks_only':True,'new_models_or_experiments_or_builds':0,'network':0,'root_source57_or_ledger_recheck':False,'run_execution':'see root ledger:33HTTP,3fee-script runs,1root scalar; no performance/native/production operations'}}
rp.write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n');assert r['passed'],json.dumps({k:v for k,v in checks.items() if not v})
print(json.dumps({'checks':len(checks),'passed':r['passed'],'new_links':len(new),'preexisting_broken':len(old_bad)}))
