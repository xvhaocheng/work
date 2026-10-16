from pathlib import Path
import hashlib, json, re

W=Path('/home/xvhaocheng/work')
R=W/'iommu-platform/out/research-reprioritize-20261015'
D=R/'documentation'
if (D/'before-write-manifest.json').exists():
    raise SystemExit('One-shot document sync already executed; preserve before-write originals.')
names=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
agents=['AGENTS.md','iommu-platform/AGENTS.md']
# Immediate pre-write reread of all five targets; protected/current independent facts merge below.
before={n:(W/n).read_bytes() for n in names+agents}
sha=lambda b:hashlib.sha256(b).hexdigest()
roots=['decision-analysis.md','root-result-acceptance.json','next-minimal-contract.json','source-verification.json']
root_before={n:sha((R/'root'/n).read_bytes()) for n in roots}
for n,b in before.items():
    p=D/'before-write'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b)
(D/'before-write-manifest.json').write_text(json.dumps({n:{'bytes':len(b),'sha256':sha(b)} for n,b in before.items()},ensure_ascii=False,indent=2)+'\n')
def span(b,s,e=None):
    i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
base='iommu-platform/out/research-reprioritize-20261015'
links=f'[本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json)／[下一最小合同]({base}/root/next-minimal-contract.json)'
selected='本轮不选替代论文主线；下一笔有限证据投入选B1-STRIPE-COST-1，尚未执行。'
task='仅核HERO mm-large一条正常未来B条带：允许普通预热/PTE cache后还剩什么，以及一次性结果的真实存储、端口、归属与释放成本。它是下一证据任务，不是B1论文最优结论，不自动恢复B1旧实验。'
other='I仍为轻量not_ready，D旧“上一窗口→下一任务P/PN”版降优先级；D-live固定模式合法补交版不升位。A5/A2与Q/A/G/C2的具体资产和边界保留，不自动回退任何方向。'
trace='本轮复用P、PN旧COPY轨迹各作一次完整只读提取，共4,331,570事件行，各256次合法补交；仅首个窗口在软件最早返回时有旧V PTE pending 60模型tick，到下一A首物理/首payload时均为0。该结果未给“延后新A保护当前旧PTE”正例，不是反事实延后实验、native结果或整个walk完成证明。'
execution='本轮有2条旧轨迹分析及Arcus、z-READ局部方法定向取得，不能记为0联网或0分析；0新机制/模型/应用源程序实验、0构建、0旧实验重跑、0生产Linux/RTL/ABI编辑、0设备/硬件操作。'
config='共享研究Codex-only，未用DeepSeek/Claude；请求配置为开放设计Astra/xhigh、I机制Sol/xhigh、普通提取/文档Sol/medium，实际模型/强度独立元数据unknown。第三设计席2次创建失败后由root接手，不记成已运行席。'
hardware='MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。'

current=f'''# 当前状态：下一证据投入B1单条带成本卡，未选论文主线

2026年10月16日（JST，10月15日启动）。**{selected}** {task} 10月15—28日立题窗口不重算，论文尚未完成。[研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- {selected}{task} 先复用源码和已核方法，缺映射/时间输入明确保留unknown，不先建大模型、collector或采硬件；后继由此卡结果决定，既有授权与暂停状态保持。{links}。
- 已发展B1-D一次性翻译结果晚交接、I-SET32精确稀疏页集失效、D-live合法补交控制三个正向版本。{other} [版本与普通对照]({base}/root/decision-analysis.md)。
- {trace} [提取方法与实际执行]({base}/refill-window/brief.md)／[结果]({base}/refill-window/analysis.json)。
- B1 future地址早于SPM释放可知，但真实对象仅32-bit job ID，未来条带尚未提交，无免费多页结果表；默认16KiB条带的真实页粒度/对齐未知。预热后的完整残余和片上成本仍待下一卡回答。[B1/Q提案]({base}/prepare-resource/proposal.md)。I合法稀疏批/live预算仍缺，strict串行调用不能免费合批、物理离散不等于IOVA离散。[I提案]({base}/invalidation/proposal.md)。
- 既有D边界/四例准备/CS1/MQSim具体版本的局部停止及12份计划归档保持，不重跑、不外推全族。[D前轮](iommu-platform/out/d-boundary-source-20261015/README.md)／[归档索引](iommu-platform/docs/history/plans/README.md)。

'''.encode()+span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态')+f'''## 硬件与执行状态

{execution} 源hash/行界81项检查通过，来源范围与历史轨迹有身份限制。[来源核验]({base}/root/source-verification.json)／[执行账]({base}/root/execution-ledger.json)。{hardware} {config}

'''.encode()+span(before[names[0]],'## 独立工程事项')

plan=f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{selected}** {task} I维持轻量not_ready，D旧下一任务选策版降优先级，D-live不升位；A5/A2及其余资产保留。立题窗口仍为10月15—28日，不重新起算。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支局部合同不替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按当前阶段使用，来源/条件预测不当测量结论，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | 三个正向版本与来源裁决已完成；下一B1单条带残余/存储卡，尚未执行 | 正常源码窗口、允许普通方案和实际存储/端口有明确出处，未知输入保留 |
| 10月18—19日：首个决定性判别 | 按单条带卡结果决定推进一项具体实现/模型、重设计或结束该次投入 | 存在有来源残余及具名普通对照后仍成立的能力/成本点；不自动开大模型 |
| 10月20—24日：通过后扩大 | 仅随获选具体合同冻结目标、成本与未见输入 | 所需准入证据成立后安排后继，原有授权与暂停状态保持 |
| 10月25—28日：研究裁决 | 贡献论证、前作差异、证据矩阵和后续最小合同 | 明确推进、重设计或降优先级；不以证据卡或模型成功声称论文完成 |

本轮有限正向设计和两条旧轨迹分析已完成，尚不足以给论文方向作净收益或成功率排名。下一只选择B1-STRIPE-COST-1这笔有限证据投入，I与D均不自动升位；后继由卡的结果决定，不默认再开六族总评。{links}。

## B1：下一笔单条带残余与成本证据

HERO mm-large正常计算当前B条带时，更远future地址可知但SPM仍被占用；B1-D拟把有限一次性翻译结果保留至真实release/接收许可。真实job仅32-bit ID，尚未提交的future stripe没有现成结果表；默认16KiB，真实页粒度/对齐未确立，4KiB下4/5页只可作为双方共享的显式投影。

下一B1-STRIPE-COST-1只做一条正常future stripe的来源卡、普通预热后残余卡，以及pending/ready/fault额度、context/permission/epoch/PPN、物理存储/端口、owner与last-use释放表。允许现有debug预热、PTE cache/可行大页、合法提前unpin的O-PIN、O-PAQ-D及可行ATC/正常物理缓存位置；O-PAQ-D可以是最简候选实现，不虚构免费池或强迫它击败同接口普通程序。[下一合同]({base}/root/next-minimal-contract.json)／[B1/Q提案]({base}/prepare-resource/proposal.md)。

ICCD2018进度限定纯预取、ATS/ATC和DATE2025 PTE LLC已缩小宽泛主张；新增价值必须落在普通方法后的真实残余、具体完整成本或部署能力。当前未证明残余、面积/时钟或完整任务收益；未知映射/时间不填造。若普通方案足够或不能形成有来源成本问题，就结束这次证据投入，不自动恢复旧B1实验。

## D与其他候选：保留边界，不自动升位

{trace} D-live的成功callback→下一代submit前确有普通合法位置，但bounded ready队列/延后是新增适配，Arcus已覆盖通用供给解耦、整形及反馈；仍需同信息强对照后的及时PTE—payload关系增量。旧下一任务P/PN版的跨quiet/config/clear缺口与降优先级保持，不能把新补交位置当旧边界已补齐。[root设计]({base}/root/consumer-and-refill-design.md)／[两轨迹提取]({base}/refill-window/brief.md)。

I-SET32有明确leaf/完成/保留合同，但IDYLL与局部核读z-READ已覆盖稀疏offset/bitmap合批的重要普通构件；合法同context稀疏批、live预算与命令前端费用仍缺，维持not_ready。Q-S具名checkpoint已表达所提pause动作，A/G缺未见抽取/正常动态来源后果，C2缺正常有限返回见证与分析增量；各自问题保留，不否定全族、不默认A。[本轮裁决]({base}/root/decision-analysis.md)复用[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md)，未改写历史报告。

'''.encode()+span(before[names[1]],'## I：')

nav=f'''# 共享IOMMU研究导航

2026年10月16日（JST）。{selected}{task} 先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；阶段背景查[HANDOFF](HANDOFF.md)，方法查[ARS-Codex入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)，不默认全pipeline。

## 当前研究与下一关口

| 版本/方向 | 当前安排 | 入口 |
| --- | --- | --- |
| 下一证据：B1-STRIPE-COST-1 | HERO未来条带普通预热后残余/真实存储与端口卡；尚未执行，不等于论文主线或恢复旧B1实验 | [下一合同]({base}/root/next-minimal-contract.json)／[本轮入口]({base}/README.md) |
| B1-D：准备结果晚交接 | 保留接口/成本候选；job ID不提供免费结果池，真实粒度/对齐未知 | [提案]({base}/prepare-resource/proposal.md)／[源证据]({base}/prepare-resource/evidence-map.md) |
| I-SET32：稀疏精确失效 | not_ready；正常合法稀疏批/live预算/命令服务仍缺，物理离散不等于IOVA离散 | [提案]({base}/invalidation/proposal.md)／[候选卡]({base}/invalidation/candidate-card.json) |
| D-live：固定mode合法补交 | 新位置成立；两条旧COPY轨迹未给当前PTE保护正例，不升位 | [root设计]({base}/root/consumer-and-refill-design.md)／[实际提取]({base}/refill-window/brief.md) |
| D旧下一任务选策版 | 降为待新证据候选；旧quiet/config/clear边界缺口保持 | [前轮裁决](iommu-platform/out/d-boundary-source-20261015/root/decision-analysis.md) |
| Q / A / G / C2及其他版本 | 具体资产/边界保留，不自动回退、恢复旧实验或认定全族无价值 | [本轮root裁决]({base}/root/decision-analysis.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md) |

{trace} 81项源hash/行界检查通过；Arcus与z-READ仅定向局部核读，不冒称全论文覆盖。{execution} I/A5/A2、原有授权与10月15—28日窗口保持。[稳定接受]({base}/root/root-result-acceptance.json)／[来源核验]({base}/root/source-verification.json)。

'''.encode()+span(before[names[2]],'## 已有成果怎样复用')

def record(prefix):
    def lk(t,p):return f'[{t}]({prefix}{p})'
    return f'''# 2026年10月16日 正向设计与下一笔证据投入裁决

10月15日启动、16日JST交付。方法：ARS-Codex有界问题发展、实质前作比较与证据规划；仅同步root稳定接受，不启动fullpipeline。{selected}{task} {other}

三个正向版本为B1-D、I-SET32、D-live，Q-S与A/G/C2边界也经审视。{trace} 源hash/行界81项通过；Arcus与z-READ为定向局部方法取得，保留覆盖限制，未改写既有阅读/失败记录。

{execution} {hardware} {config} 10月15—28日窗口与既有授权保持；下一合同未执行不表示用户原授权被撤销。五目标写前完整字节/hash、四保护块、两整历史suffix与双AGENTS检查见回执；以下旧全文保留，不回填历史。

{lk('本轮入口','README.md')}／{lk('root裁决','root/decision-analysis.md')}／{lk('稳定接受','root/root-result-acceptance.json')}／{lk('下一合同','root/next-minimal-contract.json')}／{lk('提取执行','refill-window/brief.md')}／{lk('执行账','root/execution-ledger.json')}／{lk('文档回执','documentation/receipt.json')}。

'''.encode()
outputs={names[0]:current,names[1]:plan,names[2]:nav,names[3]:record(base+'/')+before[names[3]],names[4]:record('../out/research-reprioritize-20261015/')+before[names[4]]}
for n,b in outputs.items():
    assert (W/n).read_bytes()==before[n],f'Concurrent change: {n}'
    (W/n).write_bytes(b)
readme='''# RESEARCH-REPRIORITIZE-1：正向设计与下一证据投入

2026-10-15启动、2026-10-16 JST交付。本轮不选替代论文主线；下一笔有限证据投入选B1-STRIPE-COST-1，尚未执行。这是HERO正常未来条带的普通预热后残余、真实存储/端口成本卡，不是B1论文最优或恢复旧实验的结论。

- [root裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[下一最小合同](root/next-minimal-contract.json)
- [B1/Q提案](prepare-resource/proposal.md)／[候选卡](prepare-resource/candidate-card.json)／[证据地图](prepare-resource/evidence-map.md)／[源manifest](prepare-resource/source-manifest.json)
- [I-SET32提案](invalidation/proposal.md)／[候选卡](invalidation/candidate-card.json)／[源manifest](invalidation/source-manifest.json)
- [root A/G与D-live设计](root/consumer-and-refill-design.md)／[两条旧轨迹提取与执行](refill-window/brief.md)／[结果](refill-window/analysis.json)／[提取执行回执](refill-window/execution-receipt.json)
- [Arcus局部取得范围](root/sources/arcus-retrieval.json)／[z-READ局部取得范围](root/sources/zread2024-root-retrieval.json)
- [root源manifest](root/root-source-manifest.json)／[81项源核验](root/source-verification.json)／[执行账](root/execution-ledger.json)
- [文档回执](documentation/receipt.json)／[写前完整字节/hash](documentation/before-write-manifest.json)／[前轮D边界](../d-boundary-source-20261015/README.md)

三个正向版本为B1-D、I-SET32、D-live。I维持not_ready，D旧下一任务版降优先级；D-live两条旧COPY轨迹未给当前PTE保护正例，不升位。Q/A/G/C2具体资产与边界保留，尚未选择论文主线。

本轮有P/PN各一次只读轨迹提取（共4,331,570事件行、各256次合法补交）及Arcus/z-READ局部定向方法取得，不能记为0分析/0联网。0新机制/模型/应用源程序实验、构建、旧实验重跑、生产编辑和硬件操作。10月15—28日窗口、A5/A2及MIG暂停保持；实际模型/强度元数据unknown，未使用DeepSeek/Claude。
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
def targets(s):
    return {t.split('#',1)[0].strip('<>') for t in pattern.findall(s) if not re.match(r'^[A-Za-z][A-Za-z0-9+.-]*:',t) and not t.startswith('#')}
def exists(n,t):
    t=re.sub(r':\d+(?::\d+)?$','',t);return (Path(t) if t.startswith('/') else (W/n).parent/t).exists()
new=[];old_bad=[]
for n,b in outputs.items():
    old=targets(before[n].decode())
    for t in sorted(targets(b.decode())):
        entry={'file':n,'target':t,'exists':exists(n,t)}
        if t not in old:new.append(entry)
        elif not entry['exists']:old_bad.append(entry)
rn=str((R/'README.md').relative_to(W))
for t in sorted(targets(readme)):new.append({'file':rn,'target':t,'exists':exists(rn,t)})
# Create receipt first, then resolve its self-link against the actual filesystem.
receipt_path=D/'receipt.json';receipt_path.write_text('{}\n')
for item in new:item['exists']=exists(item['file'],item['target'])
checks['all_new_local_links_exist']=all(x['exists'] for x in new)
receipt={'task':'RESEARCH-REPRIORITIZE-1 documentation delivery','local_date':'2026-10-16 JST','configuration':{'requested_model':'gpt-6.1-sol','requested_effort':'medium','actual_model':'unknown','actual_effort':'unknown'},'checks':checks,'check_count':len(checks),'passed':all(checks.values()),'before_write_manifest':'before-write-manifest.json','after':{n:{'bytes':len(b),'sha256':sha(b)} for n,b in outputs.items()},'final_README':{'path':rn,'bytes':len(readme.encode()),'sha256':sha(readme.encode())},'stable_root_input_hashes':root_before,'link_check_method':'unique (document,target) pairs; fragments stripped, :line[:column] normalized; newly introduced target per document, all new README links; historical suffix retained','new_local_links':new,'new_local_link_count':len(new),'preexisting_broken_local_links':old_bad,'preexisting_broken_count':len(old_bad),'execution':{'documentation_checks_only':True,'models_or_experiments_or_builds':0,'network':0,'source81_recheck':False,'run_execution':'see stable root acceptance: two existing trace analyses and targeted method acquisitions, not zero networking/analysis'}}
receipt_path.write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
assert receipt['passed'],json.dumps({k:v for k,v in checks.items() if not v})
print(json.dumps({'checks':len(checks),'passed':receipt['passed'],'new_links':len(new),'preexisting_broken':len(old_bad)},ensure_ascii=False))
