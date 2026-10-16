from pathlib import Path
import json,hashlib,re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/b1-stripe-cost-20261016';D=R/'documentation'
if (D/'before-write-manifest.json').exists():raise SystemExit('One-shot sync already executed; do not overwrite before-write originals.')
names=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'];agents=['AGENTS.md','iommu-platform/AGENTS.md']
# Reread all five live targets immediately before final write, retaining independent blocks.
before={n:(W/n).read_bytes() for n in names+agents};sha=lambda b:hashlib.sha256(b).hexdigest()
roots=['decision-analysis.md','residual-and-capacity.md','root-result-acceptance.json','next-minimal-contract.json','source-verification.json','execution-ledger.json']
root_before={n:sha((R/'root'/n).read_bytes()) for n in roots}
for n,b in before.items():
 p=D/'before-write'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b)
(D/'before-write-manifest.json').write_text(json.dumps({n:{'bytes':len(b),'sha256':sha(b)} for n,b in before.items()},ensure_ascii=False,indent=2)+'\n')
def span(b,s,e=None):
 i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
base='iommu-platform/out/b1-stripe-cost-20261016'
links=f'[本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json)／[下一合同]({base}/root/next-minimal-contract.json)'
decision='B1-STRIPE-COST-1单条带证据任务已完成；本B1-D实例转待具体残余/成本证据，本轮不进入新模型或RTL。暂不选论文主线。'
limit='未证明普通预热已足够，也未证明B1全族无价值。'
next_text='下一仅B1-REENTRY-WITNESS-1，输入/设计触发、尚未执行：给出普通方案后的具体残余及完整后果，或同scope组织的可核完整成本/部署能力差异。接受测量或假设冻结的来源投影，不强制native；这是证据触发条件，既有授权与暂停状态保持。不重复卡、不自动六族重排或回退A/I/D。'
source='正常来源是copy-based device1，SVM device0为if(0)；B0/B2复用槽0，t0计算隐式barrier后才释放。4KiB外部腿共同投影A1∪B1∪B2紧上界为14≤16；页集合不是同时驻留需求，也不保证PLRU保留。源码未给普通预热后强制驱逐或完整关键残余。'
cost='同scope紧凑ATC与O-PAQ可共享span/context表头。N=4/5时one-shot主表示为343/390+E+W+G位；pin为238/241+E+W+G位，均须另计同类X项，pin还占已有688/860逻辑位的way资源。这是参考编码，不是净面积、最小性、时钟或性能结果。'
keep='I仍not_ready，D旧下一任务版降优先级、D-live未升位；A5/A2及Q/G/C2等资产和具体边界保留。10月15—28日窗口不重算。'
execution='本轮仅定向读源和几何/表示算术：root 37项源hash/行界（24个不同路径）、14项scalar与4类offset检查通过；source席4096 offset算术不是模型或运行工作负载。0联网、0新模型/源程序实验、0构建、0旧实验重跑、0生产Linux/RTL/ABI编辑、0硬件操作。'
hardware='MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。'
config='共享研究Codex-only，未用DeepSeek/Claude；请求source Sol/high、storage Sol/xhigh、文档Sol/medium，实际独立模型/强度元数据unknown。'

current=f'''# 当前状态：B1单条带卡完成，待具体残余或成本证据

2026年10月16日（JST）。**{decision}** {limit} {keep} [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产；论文尚未完成。

## 全局安排与下一证据

- {decision}{next_text} {links}。
- {source} [单条带来源]({base}/stripe-source/findings.md)／[root残余与容量]({base}/root/residual-and-capacity.md)。
- {cost} 当前debug为48bit单份返回，TC仅单结果，backend AT固定untranslated；准备结果要有合法采纳/权限/有效性路径，缺接口本身不算创新。[存储/端口卡]({base}/storage-cost/cost-card.md)。
- {keep} 既有D边界、旧轨迹分析与各版本局部停止仍按各自范围保存，不重跑或拼成新收益证据。[前轮正向设计与裁决](iommu-platform/out/research-reprioritize-20261015/README.md)／[计划归档](iommu-platform/docs/history/plans/README.md)。

'''.encode()+span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态')+f'''## 硬件与执行状态

{execution} [来源与算术核验]({base}/root/source-verification.json)／[执行账]({base}/root/execution-ledger.json)。{hardware} {config}

'''.encode()+span(before[names[0]],'## 独立工程事项')

plan=f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{decision}** {limit} I仍not_ready，D旧下一任务版降优先级、D-live不升位，A5/A2及其余资产保留。立题窗口仍为10月15—28日，不重新起算。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支局部合同不替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按阶段使用；来源/条件预测不当测量结论，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 正向设计重排与B1单条带卡完成，当前实例待具体残余/成本证据 | 仅按B1-REENTRY-WITNESS-1的实际输入/设计差异触发，不重复未变卡 |
| 10月18—19日：首个决定性判别 | 若触发，冻结一个能改变判断的具体后继；否则记录未推进与准确缺项 | 测量或假设冻结来源投影均可，不强制native，不自动开模型 |
| 10月20—24日：通过后扩大 | 依获选具体合同验证完整目标与费用 | 普通对照、owner/端口/失效/释放边界明确，原有授权与暂停状态保持 |
| 10月25—28日：研究裁决 | 贡献论证、前作差异、证据矩阵与后续最小合同 | 明确推进、重设计或降优先级；不以证据卡或模型成功声称论文完成 |

所选B1单条带证据投入已结题，但普通预热后的残余与完整成本优势均未证明；不能据此判定预热足够或整个B1无价值。下一仅保留输入/设计触发合同，尚未执行；不选论文主线，不重复卡或自动六族重排。{links}。

## B1：本实例待具体残余或完整成本证据

{source} 目标侧b与host malloc、leaf/context及窗口时长仍unknown，不能把关闭的SVM分支写成已启用共享IOMMU运行。当前原生job ID只32bit，没有免费未来多页结果表。[单条带来源]({base}/stripe-source/findings.md)／[容量与普通方法]({base}/root/residual-and-capacity.md)。

{cost} 准备返回写、采纳读、release与失效可共用bank但会增加仲裁；debug48bit返回、TC单结果、untranslated backend都不能免费变成合法future adopt。last-use、epoch、route及物理实现费用须明确或保留unknown。[逐字段与端口卡]({base}/storage-cost/cost-card.md)。

允许普通debug预热、PTE cache/可行大页、正确收费且早释放的O-PIN、同scope O-PAQ/紧凑ATC及合法结果缓存位置。ICCD2018已有进度限定纯翻译预取，ATS已有结果保存/后续访问，DATE2025已有PTE LLC；可发展的新增贡献须是正常span上的具体残余，或合法交接组织带来的完整部署/成本差异。最简实现可以是O-PAQ，不要求击败同接口所有想象程序，也不把缺adopt当新意。

{next_text} [B1-REENTRY-WITNESS-1]({base}/root/next-minimal-contract.json)列出目标侧地址/leaf/context、合法请求/预热/消费顺序、完整后果，或owner/端口/失效/last-use接法与全费用。不得任意添压力流、缩小普通容量或限制合法回收制造差异。

## 其他候选：保留具体版本边界

I-SET32仍缺合法稀疏批/live预算/命令服务证据，D旧下一任务版的quiet/config/clear缺口与降优先级保持。D-live有合法普通补交位置，但前轮P/PN两条COPY轨迹未给当前PTE保护正例，不能升级成反事实无效证明或native收益。A5/A2与Q/G/C2等准确结果保留，不默认A或自动回退。[前轮root裁决](iommu-platform/out/research-reprioritize-20261015/root/decision-analysis.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md)。

'''.encode()+span(before[names[1]],'## I：')

nav=f'''# 共享IOMMU研究导航

2026年10月16日（JST）。{decision}{limit} 下一仅B1-REENTRY-WITNESS-1输入/设计触发，尚未执行。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；背景查[HANDOFF](HANDOFF.md)，方法查[ARS-Codex入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)，不默认全pipeline。

## 当前研究与下一关口

| 版本/方向 | 当前安排 | 入口 |
| --- | --- | --- |
| B1-STRIPE-COST-1 | 单条带来源/残余/存储卡完成；当前B1-D实例待具体残余或成本证据，不进入新模型/RTL | [本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md) |
| 下一：B1-REENTRY-WITNESS-1 | 输入/设计触发、未执行；接受测量或冻结来源投影，不强制native，不设新许可流程 | [下一合同]({base}/root/next-minimal-contract.json) |
| B1来源与普通对照 | copy-based device1；4KiB外部腿页集合≤14≤16，非驻留/PLRU保证；未证预热后关键残余 | [来源卡]({base}/stripe-source/findings.md)／[残余与容量]({base}/root/residual-and-capacity.md) |
| B1成本组织 | O-PAQ/同scope ATC共享压缩；逻辑位数非净面积，合法adopt和各X费用仍须核 | [存储/端口卡]({base}/storage-cost/cost-card.md)／[成本机器卡]({base}/storage-cost/cost-card.json) |
| I-SET32 / D既有版本 | I not_ready；D旧下一任务版降优先级，D-live未升位 | [前轮裁决](iommu-platform/out/research-reprioritize-20261015/root/decision-analysis.md) |
| Q / A / G / C2及其他版本 | 具体资产/边界保留，不重复卡、自动六族重排或回退 | [前轮设计](iommu-platform/out/research-reprioritize-20261015/README.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md) |

本轮37项源hash/行界、14项scalar与4类offset核验通过；source席4096 offset检查仅是算术。0联网、新模型/实验、构建、旧重跑、生产编辑与硬件操作。I/A5/A2、原有授权、MIG暂停与10月15—28日窗口保持。[稳定接受]({base}/root/root-result-acceptance.json)／[源与算术核验]({base}/root/source-verification.json)。

'''.encode()+span(before[names[2]],'## 已有成果怎样复用')

def record(prefix):
 def lk(t,p):return f'[{t}]({prefix}{p})'
 return f'''# 2026年10月16日 B1单条带成本卡结题与再进入条件

方法：ARS-Codex有界源码、协议与成本推导；本次同步root稳定接受，不开fullpipeline。{decision}{limit} {source}

{cost} 当前debug48bit、TC单结果、backend AT固定untranslated，合法采纳仍需完整绑定/有效性；缺接口本身不是新意。{next_text} {keep}

{execution} {hardware} {config} 五目标写前完整字节/hash、四保护块、两整历史suffix与双AGENTS检查见文档回执；以下旧全文原样保留，不回填历史。

{lk('本轮入口','README.md')}／{lk('root裁决','root/decision-analysis.md')}／{lk('稳定接受','root/root-result-acceptance.json')}／{lk('下一合同','root/next-minimal-contract.json')}／{lk('成本卡','storage-cost/cost-card.md')}／{lk('执行账','root/execution-ledger.json')}／{lk('文档回执','documentation/receipt.json')}。

'''.encode()
outputs={names[0]:current,names[1]:plan,names[2]:nav,names[3]:record(base+'/')+before[names[3]],names[4]:record('../out/b1-stripe-cost-20261016/')+before[names[4]]}
for n,b in outputs.items():
 assert (W/n).read_bytes()==before[n],f'Concurrent change: {n}'
 (W/n).write_bytes(b)
readme='''# B1-STRIPE-COST-1：单条带来源与完整成本卡

2026-10-16 JST。所选单条带证据任务完成；本B1-D实例待具体残余/成本证据，本轮不进入新模型或RTL，暂不选论文主线。未证明普通预热已足够，也未证明B1全族无价值。

- [root决定](root/decision-analysis.md)／[残余与容量](root/residual-and-capacity.md)／[稳定接受](root/root-result-acceptance.json)／[下一触发合同](root/next-minimal-contract.json)
- [单条带来源](stripe-source/findings.md)／[机器卡及4096 offset算术](stripe-source/stripe-card.json)／[源manifest](stripe-source/source-manifest.json)
- [存储/端口/生命周期卡](storage-cost/cost-card.md)／[成本机器卡](storage-cost/cost-card.json)／[成本源manifest](storage-cost/source-manifest.json)
- [root关键原文manifest](root/root-source-manifest.json)／[37源与14 scalar/4 offset核验](root/source-verification.json)／[实际执行账](root/execution-ledger.json)
- [文档保护与链接回执](documentation/receipt.json)／[完整写前字节/hash](documentation/before-write-manifest.json)／[前轮正向设计](../research-reprioritize-20261015/README.md)

copy-based device1启用，SVM if(0)；B0/B2槽0由t0隐式barrier释放。4KiB外部腿共同投影A1∪B1∪B2紧上界14≤16，不代表同时驻留或PLRU保证。当前debug48bit、TC单结果、backend untranslated，缺合法adopt不等于新意。one-shot和同scope紧凑ATC/O-PAQ可共享header；参考逻辑位编码不是净面积、最小性或收益。

下一仅B1-REENTRY-WITNESS-1：输入/设计触发、未执行；接受测量或假设冻结的来源投影，不强制native，不新增许可流程。不重复卡，不自动六族重排或回退A/I/D。I not_ready、D既有版本边界、A5/A2、MIG暂停及10月15—28日窗口保留。

本轮为源码/表示/几何算术，0联网、新模型/实验、构建、旧重跑、生产编辑与硬件操作。请求source Sol/high、storage Sol/xhigh、文档Sol/medium；实际模型/强度元数据unknown，未用DeepSeek/Claude。root稍后收尾delivery-verification及goal receipt，本页不提前声称已完成收尾核验。
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
 **{f'stable_root_{n}':sha((R/'root'/n).read_bytes())==h for n,h in root_before.items()},
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
rp=D/'receipt.json';rp.write_text('{}\n')
for e in new:e['exists']=exists(e['file'],e['target'])
checks['all_new_local_links_exist']=all(e['exists'] for e in new)
r={'task':'B1-STRIPE-COST-1 documentation delivery','local_date':'2026-10-16 JST','configuration':{'requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown'},'checks':checks,'check_count':len(checks),'passed':all(checks.values()),'before_write_manifest':'before-write-manifest.json','after':{n:{'bytes':len(b),'sha256':sha(b)} for n,b in outputs.items()},'final_README':{'path':rn,'bytes':len(readme.encode()),'sha256':sha(readme.encode())},'stable_root_input_hashes':root_before,'link_check_method':'unique (document,target) pairs; fragments stripped; :line[:column] normalized; newly introduced target per document and all new README links; historical suffix retained','new_local_links':new,'new_local_link_count':len(new),'preexisting_broken_local_links':old_bad,'preexisting_broken_count':len(old_bad),'execution':{'documentation_checks_only':True,'models_or_experiments_or_builds':0,'network':0,'root_source37_or_scalar_recheck':False}}
rp.write_text(json.dumps(r,ensure_ascii=False,indent=2)+'\n');assert r['passed'],json.dumps({k:v for k,v in checks.items() if not v})
print(json.dumps({'checks':len(checks),'passed':r['passed'],'new_links':len(new),'preexisting_broken':len(old_bad)}))
