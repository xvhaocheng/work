from pathlib import Path
import hashlib,json,re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-organization-source-20261016';D=R/'documentation'
if (D/'before-write-manifest.json').exists():raise SystemExit('One-shot sync already executed; preserve before-write originals.')
names=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'];agents=['AGENTS.md','iommu-platform/AGENTS.md']
# Immediate reread of all live targets; merge independent facts by exact protected byte spans.
before={n:(W/n).read_bytes() for n in names+agents};sha=lambda b:hashlib.sha256(b).hexdigest()
roots=['decision-analysis.md','next-minimal-contract.json','root-result-acceptance.json','source-verification.json','execution-ledger.json']
stable={n:sha((R/'root'/n).read_bytes()) for n in roots}
for n,b in before.items():
 p=D/'before-write'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b)
(D/'before-write-manifest.json').write_text(json.dumps({n:{'bytes':len(b),'sha256':sha(b)} for n,b in before.items()},ensure_ascii=False,indent=2)+'\n')
def span(b,s,e=None):
 i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
base='iommu-platform/out/mapping-organization-source-20261016'
links=f'[本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json)／[下一合同]({base}/root/next-minimal-contract.json)'
decision='用户DMA_PMD＋XDP新提案触发buffer/映射组织的有限立题；优先候选1的有界整页转交协议判别，候选2保留为条件分叉，暂未选论文主线。'
next_text='下一仅MAPPING-LEASE-PROTOCOL-1，已指定、尚未执行：钩子表、状态/owner协议、最小正向合法序列与cache回流/提前逐对象撤权反例。先核可达性，不启动性能模型或生产实现。'
facts='固定本地mlx5e native XDP单page路径已核分配→路由→B DMA→完成→归还链；这是源码来源，不是已运行的DMA_PMD移植、同4KiB混包或同PMD混合最终依赖。软件flush、B CQE、最后引用、pool cache归还与整PMD释放是不同门。'
contracts='固定逐对象物理撤权、有界整批lease、原始对象隔离且专用shadow常驻三种合同分别分析；固定逐buffer revoke不能偷偷拖到最后CQE。作者合同与主要价值选择仍未回答，不阻塞这次有限分析，也不替用户选权重。'
design='整页B授权期间不得让子块回A接收未知新数据；须核route后publish前钩子、RX在途、cache回流、B接受前缀/失败和唯一归还owner。搬少数对象须在外部发布前，scrub不代替整页授权。'
keep='B1仍待具体残余/成本证据，不重复单条带卡；I not_ready，D旧下一任务版降优先级、D-live未升位，A5/A2及其他资产保留。10月15—28日窗口不重算。'
execution='本轮定向来源核查、原文方法核读与条件设计，保存HTTP下载12项；51项来源hash/行界检查涉及49路径且全过，只证明来源绑定，不证明协议正确、新意或收益。0性能模型、0原生应用运行、0构建、0旧实验重跑、0生产Linux/RTL/ABI编辑、0设备/硬件操作。'
hardware='MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。'
config='共享研究Codex-only，未用DeepSeek/Claude；请求RFC/XDP源核Sol/high、正向设计Astra/xhigh、文档Sol/medium，实际独立模型/强度元数据unknown。'

current=f'''# 当前状态：映射组织有限立题，下一核整页转交协议

2026年10月16日（JST）。**{decision}** {next_text} [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产；论文尚未完成。

## 全局安排与下一证据

- {next_text} 新切入点来自用户主动提案，不是B1缺证后的自动fallback。{links}。
- {facts} {contracts} [RFC核查]({base}/dma-pmd/findings.md)／[XDP生命周期]({base}/xdp-source/findings.md)。
- {design} Mosaic已描述迁走异域页面；普通同接口seal/copy/scrub/map等价目前只是概念同动作推理，未编译运行。须分别核协议/接口能力与策略增量，不把搬少数、分池或阈值选择直接称新意。[正向设计与强对照]({base}/design/proposal.md)。
- {keep} B1单条带卡完成但未证普通预热足够或B1全族无价值，后继仍按具体证据触发。[B1接受](iommu-platform/out/b1-stripe-cost-20261016/root/root-result-acceptance.json)／[B1再进入](iommu-platform/out/b1-stripe-cost-20261016/root/next-minimal-contract.json)。

'''.encode()+span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态')+f'''## 硬件与执行状态

{execution} [来源验证]({base}/root/source-verification.json)／[执行账]({base}/root/execution-ledger.json)。{hardware} {config}

'''.encode()+span(before[names[0]],'## 独立工程事项')

plan=f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{decision}** {next_text} 这不是B1缺证后的自动fallback；10月15—28日窗口不重新起算，原有授权与暂停状态保持。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支局部合同不替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按阶段使用；来源/条件预测不当测量结论，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 用户新提案来源/合同核查完成；下一整页转交钩子、owner协议与最小正例/反例 | 明确真实钩子、拟新增付费接口、物理布局投影与各合同，不预宣称协议可行 |
| 10月18—19日：首个决定性判别 | 逐合同推进、重设计或缩小协议范围 | RX在途、cache回流、最后引用、提前CQE/失败和撤权截止均有明确处理 |
| 10月20—24日：通过后扩大 | 先冻结一个来源投影、同信息普通对照及完整费用，再决定是否值得单个有界模型 | 不自动进入模型/RTL/板测；有限供给、延迟、内存与授权合同相同 |
| 10月25—28日：研究裁决 | 贡献论证、前作差异、证据矩阵与后续最小合同 | 不以条件协议、来源hash或模型一致声称已证新意、净收益或论文完成 |

本轮来源支持继续做候选1的有限协议判别，尚未执行后继；候选2仅在来源与合同触发时提高优先级。作者合同/主要价值未定，分开分析即可，不阻塞此轮有限工作，也不选论文主线。{links}。

## 新候选：晚路由下的buffer与映射组织

{facts} DMA_PMD已含domain追踪、cap、idle-only shrinker及partial/MRU控制；不能把大页、持久池、回收或权限分组认作新增。固定XDP一包一page，跨order0页同PMD必须明确为RFC＋本地源码的待适配投影，不能写成原生混包/部署事实。[RFC卡]({base}/dma-pmd/contract-card.json)／[XDP链]({base}/xdp-source/lifecycle-card.json)。

{contracts} Linux strict规定实际unmap同步，不自动要求page_pool逐包unmap，不能因此宣布普通page_pool违约。候选1只在允许有界整批lease等相应合同下探索整页转交；固定对象撤权更早时须拆细、全撤或明确该协议不适用。

{design} 需要显式保存目的引用与每帧状态，不能免费重跑有副作用BPF或借用per-CPU临时redirect状态；若RX页只能整队撤下，就计入全队供给影响。限定batch等待、在途帧、替代页与总内存预算，不能丢包或无限积压制造收益。[下一协议合同]({base}/root/next-minimal-contract.json)。

DMA shadowing已有专用映射池/复制，Mosaic已有连续性与异域页面搬迁，Ingens已有利用率/压缩，fbufs已有按路径缓存映射与引用归还。普通同信息seal＋搬迁＋scrub＋map构造的同动作等价仅为概念推理，不是已编译API或实测胜者；策略等价不能单独证明接口已发表或整个思想族无价值。若新协议在owner/权限/归还门间兑现新的合法转换能力或完整成本差异，才推进系统/方法贡献；名称、换场景或单阈值不足。[root实质比较]({base}/root/decision-analysis.md)／[设计提案]({base}/design/proposal.md)。

候选2关心按最终释放依赖组织局部退役；PASS最后dataref和REDIRECT TX CQE/return已找到，但同处PMD未证，pool cache也可能继续持有。无消费者/回收服务上界不声称有限回收期限。先保持条件分叉，不同时另建回收模型。

## 保留候选与既有边界

{keep} B1-STRIPE-COST-1已结题；未证普通预热后完整残余/优势，也未证明预热足够或B1无价值。其下一B1-REENTRY-WITNESS-1仅由具体残余或同scope完整成本/能力差异触发，不重复卡。[B1入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[再进入合同](iommu-platform/out/b1-stripe-cost-20261016/root/next-minimal-contract.json)。既有六族资产继续复用，不自动回退旧A/I/D或恢复旧实验。

'''.encode()+span(before[names[1]],'## I：')

nav=f'''# 共享IOMMU研究导航

2026年10月16日（JST）。{decision} 下一MAPPING-LEASE-PROTOCOL-1已指定、尚未执行。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；背景查[HANDOFF](HANDOFF.md)，方法查[ARS-Codex入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)，不默认全pipeline。

## 当前研究与下一关口

| 版本/方向 | 当前安排 | 入口 |
| --- | --- | --- |
| 候选1：晚路由整页转交 | 用户提案触发的有限立题；下一核钩子/owner协议/正例与反例，未执行，不选论文主线 | [本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[下一合同]({base}/root/next-minimal-contract.json) |
| DMA_PMD＋XDP来源 | RFC版本与固定mlx5e单page源码分离；非原生移植/同4KiB混包/同PMD依赖实测 | [RFC]({base}/dma-pmd/findings.md)／[XDP]({base}/xdp-source/findings.md) |
| 合同与普通对照 | 三合同分开；严格逐buffer revoke不拖至最后CQE；Mosaic搬异域页已知，普通seal等价仅概念 | [设计提案]({base}/design/proposal.md)／[候选卡]({base}/design/candidate-card.json) |
| 候选2：回收依赖组织 | 条件分叉；同PMD混合依赖未证，cache持有须收费，无有限期限保证 | [root条件边界]({base}/root/decision-analysis.md) |
| B1实例 | 待具体残余/成本证据，不重复卡；未证预热足够或全族无价值 | [B1接受](iommu-platform/out/b1-stripe-cost-20261016/root/root-result-acceptance.json)／[再进入](iommu-platform/out/b1-stripe-cost-20261016/root/next-minimal-contract.json) |
| I / D / A5A2及其他版本 | I not_ready，D旧版降优先级/live未升位；具体资产/边界保留 | [前轮重排](iommu-platform/out/research-reprioritize-20261015/README.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md) |

51项来源hash/行界、49路径全过，不是协议正确性/新意证明。本轮有定向联网与方法核读，0性能模型/原生应用/构建/旧重跑/生产编辑/设备操作。作者合同及价值问题仍待回答但不阻塞分合同分析；原有授权、MIG暂停和10月15—28日窗口保持。[稳定接受]({base}/root/root-result-acceptance.json)／[执行账]({base}/root/execution-ledger.json)。

'''.encode()+span(before[names[2]],'## 已有成果怎样复用')

def record(prefix):
 def lk(t,p):return f'[{t}]({prefix}{p})'
 return f'''# 2026年10月16日 用户映射组织提案：来源接受与协议下一关

方法：ARS-Codex定向事实核查、问题细化与机制比较；本次仅同步root稳定接受，不开fullpipeline。{decision} {next_text} 不是B1缺证后的自动fallback。

{facts} {contracts} Mosaic已描述搬异域页面；普通同接口seal/copy/scrub/map等价是概念同动作推理，未编译运行。{design} 后继尚未执行，不能称协议已验证或新意/收益已成立。

{execution} {hardware} {config} {keep} 写前五目标完整字节/hash、四保护块、两整历史suffix、双AGENTS及稳定root输入保护见文档回执；以下历史全文原样保留，不回填旧记录。

{lk('本轮入口','README.md')}／{lk('root裁决','root/decision-analysis.md')}／{lk('稳定接受','root/root-result-acceptance.json')}／{lk('下一合同','root/next-minimal-contract.json')}／{lk('XDP链','xdp-source/findings.md')}／{lk('执行账','root/execution-ledger.json')}／{lk('文档回执','documentation/doc-change-manifest.json')}。

'''.encode()
outputs={names[0]:current,names[1]:plan,names[2]:nav,names[3]:record(base+'/')+before[names[3]],names[4]:record('../out/mapping-organization-source-20261016/')+before[names[4]]}
for n,b in outputs.items():
 assert (W/n).read_bytes()==before[n],f'Concurrent change: {n}'
 (W/n).write_bytes(b)
readme='''# DMA_PMD＋XDP映射组织：来源与有限协议判别

2026-10-16 JST。用户新提案触发buffer/映射组织有限立题；候选1有界整页转交协议优先后继，候选2条件分叉，未定论文主线。不是B1缺证后的自动fallback。

- [用户原提案](root/user-proposal.txt)／[root裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[下一MAPPING-LEASE-PROTOCOL-1](root/next-minimal-contract.json)
- [DMA_PMD核查](dma-pmd/findings.md)／[合同卡](dma-pmd/contract-card.json)／[原文manifest](dma-pmd/source-manifest.json)／[取得回执](dma-pmd/download-receipts.json)
- [固定mlx5e XDP链](xdp-source/findings.md)／[生命周期卡](xdp-source/lifecycle-card.json)／[源码manifest](xdp-source/source-manifest.json)
- [正向设计与前作](design/proposal.md)／[候选卡](design/candidate-card.json)／[设计来源manifest](design/source-manifest.json)
- [root来源范围与51项核验](root/source-verification.json)／[实际执行账](root/execution-ledger.json)／[shadowing取得范围](root/sources/shadowing-retrieval.json)／[fbufs取得范围](root/sources/fbufs-retrieval.json)
- [文档保护与链接回执](documentation/doc-change-manifest.json)／[完整写前字节/hash](documentation/before-write-manifest.json)／[B1保留证据](../b1-stripe-cost-20261016/README.md)

下一合同已指定、尚未执行，仅钩子表、owner状态/协议及最小合法序列与cache回流/提前逐对象撤权反例；不是协议正确性验收。固定源码一包一page，未证同4KiB混包、同PMD混合依赖或DMA_PMD原生移植。strict逐buffer撤权不能延至最后CQE，三种授权合同分别分析；作者合同/价值选择待回答不阻塞有限分析。

Mosaic已有异域页面搬迁；普通同接口seal等价目前仅概念，不是已运行对照。新意须落在可核合法转换能力或完整成本差异。51项来源hash/行界、49路径全过仅为来源绑定。保存HTTP下载12项，0性能模型/原生应用/构建/旧重跑/生产编辑/设备操作。

B1待具体残余/成本证据，I not_ready、D既有边界、A5/A2、MIG暂停与10月15—28日窗口保持。共享研究Codex-only，未用DeepSeek/Claude；实际模型/强度unknown。最终交付状态以root单独保存的收尾回执为准。
'''
(R/'README.md').write_text(readme)
checks={
 'current_A5_A2_exact':span(outputs[names[0]],'## 各分支最新进展','## 硬件与执行状态')==span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态'),
 'current_independent_exact':span(outputs[names[0]],'## 独立工程事项')==span(before[names[0]],'## 独立工程事项'),
 'plan_I_and_later_exact':span(outputs[names[1]],'## I：')==span(before[names[1]],'## I：'),
 'navigation_assets_and_later_exact':span(outputs[names[2]],'## 已有成果怎样复用')==span(before[names[2]],'## 已有成果怎样复用'),
 **{f'full_old_suffix_{n}':outputs[n].endswith(before[n]) for n in names[3:]},
 **{f'AGENTS_unchanged_{n}':(W/n).read_bytes()==before[n] for n in agents},
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
r={'task':'mapping-organization-source-20261016 documentation delivery','local_date':'2026-10-16 JST','configuration':{'requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown'},'checks':checks,'check_count':len(checks),'passed':all(checks.values()),'before_write_manifest':'before-write-manifest.json','after':{n:{'bytes':len(b),'sha256':sha(b)} for n,b in outputs.items()},'final_README':{'path':rn,'bytes':len(readme.encode()),'sha256':sha(readme.encode())},'stable_root_input_hashes':stable,'link_check_method':'unique (document,target) pairs; fragment stripped; :line[:column] normalized; newly introduced target per document/all new README links; history retained','new_local_links':new,'new_local_link_count':len(new),'preexisting_broken_local_links':old_bad,'preexisting_broken_count':len(old_bad),'execution':{'documentation_checks_only':True,'new_models_or_experiments_or_builds':0,'network':0,'root_source51_recheck':False,'run_execution':'see stable root ledger: targeted network acquisition and scoped source/design work; no performance/native/production operations'}}
rp.write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n');assert r['passed'],json.dumps({k:v for k,v in checks.items() if not v})
print(json.dumps({'checks':len(checks),'passed':r['passed'],'new_links':len(new),'preexisting_broken':len(old_bad)}))
