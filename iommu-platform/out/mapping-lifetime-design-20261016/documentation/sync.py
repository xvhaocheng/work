from pathlib import Path
import hashlib,json,re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-lifetime-design-20261016';D=R/'documentation'
if (D/'before-write-manifest.json').exists():raise SystemExit('One-shot sync already executed; preserve original before-write bytes.')
contract=json.loads((R/'root/canonical-sync-contract.json').read_text());accept=json.loads((R/'root/root-result-acceptance.json').read_text())
names=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md'];agents=['AGENTS.md','iommu-platform/AGENTS.md']
# Actual live reread immediately before mutation, preserving independently updated sections.
before={n:(W/n).read_bytes() for n in names+agents};sha=lambda b:hashlib.sha256(b).hexdigest()
stable={x['path']:sha((R/x['path']).read_bytes()) for x in accept['frozen_files']}
stable['root/root-result-acceptance.json']=sha((R/'root/root-result-acceptance.json').read_bytes())
anchor=contract['agents_insertion_anchor'].encode();insertion=contract['agents_insert_exact'].encode()
assert before['AGENTS.md'].count(anchor)==1 and insertion not in before['AGENTS.md']
agents_after=before['AGENTS.md'].replace(anchor,insertion+anchor,1)
for n,b in before.items():
 p=D/'before-write'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b)
(D/'before-write-manifest.json').write_text(json.dumps({n:{'bytes':len(b),'sha256':sha(b)} for n,b in before.items()},ensure_ascii=False,indent=2)+'\n')
def span(b,s,e=None):
 i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
base='iommu-platform/out/mapping-lifetime-design-20261016'
links=f'[本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[稳定接受]({base}/root/root-result-acceptance.json)／[下一合同]({base}/root/next-minimal-contract.json)'
decision='本轮最值得保留V1：源page_pool持有物理页、B持可撤销映射关联、一次TX持短时token，三种寿命分开，保留正常recycle并在真正源退出前撤图。'
boundary='一般owner通知已有dma-buf/DMA_PMD近邻覆盖，同付费接口下相对O3策略增量为0；具体page_pool/NAPI/FIFO整合的完整成本或部署差异仍待局部判别。未证创新或净赢，未选论文主线。'
next_text='下一OWNER-BINDING-INTEGRATION-DELTA-1已指定、尚未执行，仅列V1与合理等价adapter的字段、调用、锁及退出责任差项；不自动开发驱动或模型。'
resource='相同有效工作与服务下，降低CPU费用或不可回收内存可作主要贡献，无需再次确认抽象授权/资源价值。实际部署事实未知只按条件处理。源池本来持有、B真正延后释放、copy目标新增和撤销隔离Q须按物理并集去重，映射不等于多一份payload，原帧早归还不等于总RAM下降。'
keep='B1具体残余/成本证据触发、I not_ready、D既有版本边界、A5/A2与独立工程结果保持；MIG暂停及10月15—28日窗口不重启，不自动升位其他分支。'
execution='本轮50项来源hash、38个唯一路径、58项行界检查通过，只核来源/定位；新增3个固定commit源码HTTP文件，旧PDF与源码复用。0新功能/性能模型、0旧阈值账重跑、0原生应用/内核运行、0构建、0生产Linux/RTL/ABI编辑、0设备动作；设计事件序列非实测。'
hardware='MIG1792023314继续paused，build6 WNS −0.391ns、598项setup失败、0新加载采集，实际债务unknown。'
config='共享研究Codex-only，未用DeepSeek/Claude；请求正向设计Astra/xhigh、资源/近邻源核Sol/high、文档Sol/medium，实际独立模型/强度元数据unknown。'

current=f'''# 当前状态：保留源owner与B关联分离，下一核局部整合差项

2026年10月16日（JST）。**{decision}** {boundary} [研究计划](研究计划.md)是唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- {next_text} {links}。
- 普通page ref、pool-aware pp_ref和pool对象寿命不同；B长期普通ref可能使页离开源pool。拟议V1空闲cache不加page/pp pin，完成先还token再还frame，避免源退出等待自己；持久map仍需per-use sync。三逻辑FRAME FIFO槽可换binding handle仅为设计，未编译。[资源源码核查]({base}/resource/findings.md)／[owner合同]({base}/root/owner-binding-contract.md)。
- {resource} 具体无法兼得的价值取舍才携方案与证据询问。[资源卡]({base}/resource/resource-card.json)。
- 强对照允许普通O3同owner接口/FIFO表示、合理分组/预准备/range适配的dynamic dma-buf及owner-bound RFC适配；不能靠弱pin对照、一般通知或接口缺项宣称贡献。[近邻核查]({base}/prior/findings.md)／[正向版本]({base}/design/proposal.md)。{keep}

'''.encode()+span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态')+f'''## 硬件与执行状态

{execution} [来源验证]({base}/root/source-verification.json)／[执行账]({base}/root/execution-ledger.json)。{hardware} {config}

'''.encode()+span(before[names[0]],'## 独立工程事项')

plan=f'''# 共享IOMMU研究总计划

2026年10月16日（JST）。**{decision}** {boundary} {next_text} 10月15—28日窗口和既有授权/暂停状态保持。

本文件是唯一总计划；[研究导航](研究导航.md)定位成果和代码，[当前状态](CURRENT_STATUS.md)记录实际执行。分支合同不替代全局安排。[ARS-Codex方法入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)按阶段使用，来源/条件预测不当测量结论，不默认全pipeline。

## 分阶段安排

| 窗口 | 交付 | 推进条件 |
| --- | --- | --- |
| 10月15—17日：固定问题 | V1源owner/B关联/TX使用分离设计与来源核查完成；下一局部整合差项表 | 相同source布局、权限、target和上下文；普通对照同接口/表示，不重复旧阈值 |
| 10月18—19日：首个决定性判别 | 每方案字段/调用/锁/owner责任表，仅定位真正差异 | 处理首miss、token-before-frame、source/target关闭、sync或Q；不把原语数当周期 |
| 10月20—24日：通过后扩大 | 只对剩余具体差异决定有界实现/验证需求 | 不因全部成本未知自动开发；无差异收为普通正确集成，不关闭思想族 |
| 10月25—28日：研究裁决 | 贡献论证、前作差异、完整资源与服务结果、后继合同 | 相同有效工作/服务下CPU或不可回收内存可作主要价值，未证不写论文完成 |

本轮接受的是有界设计与源码证据，当前不确认独立论文贡献或更低完整成本。下一只做V1与具名等价适配的局部差项，不等待抽象授权/CPU内存论文价值确认。{links}。

## V1：保留源回收的跨设备映射关联

固定官方v6.12普通page-backed mlx5e native XDP单页链，首版order0；复用上轮FIB/UDP/TX证据，不拼成原生运行。普通page_ref、pp_ref与pool对象引用不同，B空闲cache跨consumer持普通ref可能使页离开A pool。V1不让空闲cache加page/pp pin，将可撤映射绑定真实source owner；source退出须在release_cnt、清pp_info与put之前关关联/撤图。[资源核查]({base}/resource/findings.md)。

每次TX单独持binding token，完成先还token再还frame，持久map仍需每次sync。三逻辑FRAME FIFO槽中的dma_addr可替换handle只是表示草图，同样给普通O3；未编译、未证明零额外metadata。source/target并发关闭要唯一unmapper和配置寿命保证；同步不适合时明确Q保留页、metadata、worker与inflight责任，不能先丢owner再撤图。[owner/binding合同]({base}/root/owner-binding-contract.md)。

一般通知/反向关联已有dynamic dma-buf和DMA_PMD，策略增量对同接口O3为0。公平dma-buf adapter允许预export/attach、pool/span分组、自有range cache、控制面prepare、hot hit和miss defer/copy；不能强迫逐包export或每页一对象。dma-buf map的might_sleep/reservation与当前NAPI/原DMA API GFP_ATOMIC差异是可研究部署关口，不证明V1已经更优；owner通知也不自动完成DMA drain/硬件撤图。[近邻实质比较]({base}/prior/findings.md)。

{resource} 固定外部到包、路由、consumer和服务合同，各策略分别导出真实recycle/allocation与再用，不强套同一PFN轨迹。metadata/PTE、IOVA、带宽和完成结果另列；普通pin造成的替代分配可能出现在另一页，copy目标池和Q都不能漏账。[资源卡]({base}/resource/resource-card.json)。

{next_text} [后继合同]({base}/root/next-minimal-contract.json)限定source join/release、B miss/hit、TX接受/失败/CQ、source/target关闭竞争、sync/Q以及聚合adapter的局部可执行差项。只有具体完整整合成本降低或同合同部署范围差异成立，才推进独立贡献；若相同接口/表示后等价，保留普通正确集成，不重复阈值账或因缺成本自动上系统。

## 既有问题与分支边界

路由后copy/短map/持久接纳一般问题保留；上一累计阈值按定义为普通O3，策略新意为0，补同例数字不恢复新意，也不说明所有实现完整成本相同。前轮说明CPU小计与来源身份仅作历史证据入口，不改写数字或原报告。[前轮接纳账](iommu-platform/out/mapping-admission-20261016/README.md)。旧整页协议不执行、候选2不展开；{keep} [B1保留入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)。

'''.encode()+span(before[names[1]],'## I：')

nav=f'''# 共享IOMMU研究导航

2026年10月16日（JST）。{decision}{boundary} 下一OWNER-BINDING-INTEGRATION-DELTA-1已指定、未执行。先读[当前状态](CURRENT_STATUS.md)，按[唯一总计划](研究计划.md)执行；背景查[HANDOFF](HANDOFF.md)，方法查[ARS-Codex入口](/home/xvhaocheng/.codex/skills/academic-research-suite/SKILL.md)。

## 当前研究与下一关口

| 版本/方向 | 当前安排 | 入口 |
| --- | --- | --- |
| V1源owner/B关联/TX token | 保留recycle的具体整合假设；尚无独立贡献或净收益结果 | [本轮入口]({base}/README.md)／[root裁决]({base}/root/decision-analysis.md)／[owner合同]({base}/root/owner-binding-contract.md) |
| 下一局部整合差项 | 字段/调用/锁/退出责任表；未执行、不自动开发 | [下一合同]({base}/root/next-minimal-contract.json) |
| page_pool与资源归因 | page_ref/pp_ref/pool寿命区分，S/P/C/Q物理并集与逐策略分配反馈 | [资源报告]({base}/resource/findings.md)／[机器卡]({base}/resource/resource-card.json) |
| 近邻与普通强对照 | 一般通知已有，O3同接口策略增量0；dma-buf合理分组/预准备/非睡眠接入等差项待核 | [近邻方法]({base}/prior/findings.md)／[比较卡]({base}/prior/comparison-card.json) |
| V2与其他表示选择 | copy早返仅作资源强对照，FIFO handle是未编译草图，不另开系统 | [正向提案]({base}/design/proposal.md) |
| 旧费用账/整页协议 | 历史保留，旧阈值不重跑、旧整页合同不恢复 | [前轮账](iommu-platform/out/mapping-admission-20261016/README.md)／[整页历史](iommu-platform/out/mapping-organization-source-20261016/README.md) |
| B1 / I / D / A5A2 | 既有证据触发/未就绪/版本边界和工程资产保持，不自动升位 | [B1入口](iommu-platform/out/b1-stripe-cost-20261016/README.md)／[六族历史比较](iommu-platform/docs/independent-research-directions-20261015.md) |

相同工作/服务下CPU或不可回收内存改善可作主要贡献，无需再次确认抽象价值；实际部署未知保留条件事实。50hash/38路径/58行界只核来源定位；新增3固定commit HTTP源码，0模型/旧阈值/原生/构建/生产/设备动作。MIG暂停与原窗口保持。[稳定接受]({base}/root/root-result-acceptance.json)／[执行账]({base}/root/execution-ledger.json)。

'''.encode()+span(before[names[2]],'## 已有成果怎样复用')

def record(prefix):
 def lk(t,p):return f'[{t}]({prefix}{p})'
 return f'''# 2026年10月16日 V1源owner/B关联/TX使用分离：有界设计接受

方法：ARS-Codex有界机制发展与定向源码/前作核查，本次仅同步root稳定接受。{decision}{boundary} {next_text}

普通page ref、pp_ref与pool寿命分开；真实source退出须先于release_cnt/清pp_info/put。持久TX先还token再归frame且仍需per-use sync。一般通知已有，合理dma-buf聚合/预准备/缓存与普通RFC适配均是强对照；FIFO三槽换handle仅设计，不报已编译或零成本。

{resource} 逐策略真实分配替代固定PFN账，不把copy早返或映射数量直接称省RAM。{execution} {hardware} {config} {keep}

根AGENTS只在指定锚点插入精确授权/资源价值/版本判别政策节，模型规则和其余原字节不改；平台AGENTS全文保护。写前快照、四保护块、两完整旧suffix与冻结产物保护见回执；以下历史全文保留，不回填旧记录。

{lk('本轮入口','README.md')}／{lk('root裁决','root/decision-analysis.md')}／{lk('稳定接受','root/root-result-acceptance.json')}／{lk('下一合同','root/next-minimal-contract.json')}／{lk('owner合同','root/owner-binding-contract.md')}／{lk('执行账','root/execution-ledger.json')}／{lk('文档验证','documentation/verification.json')}。

'''.encode()
outputs={names[0]:current,names[1]:plan,names[2]:nav,names[3]:record(base+'/')+before[names[3]],names[4]:record('../out/mapping-lifetime-design-20261016/')+before[names[4]],'AGENTS.md':agents_after}
for n,b in outputs.items():
 assert (W/n).read_bytes()==before[n],f'Concurrent change: {n}'
 (W/n).write_bytes(b)
readme='''# V1：源owner、B可撤映射关联与一次TX使用分离

2026-10-16 JST。最值得保留V1的具体page_pool/NAPI/FIFO整合假设；普通O3同付费接口策略增量为0，一般owner通知已有dma-buf/DMA_PMD近邻。未证独立创新、更低完整成本或净赢，未选论文主线。

- [root裁决](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[owner/binding合同](root/owner-binding-contract.md)／[下一局部差项合同](root/next-minimal-contract.json)
- [正向设计](design/proposal.md)／[候选卡](design/candidate-card.json)／[设计来源manifest](design/source-manifest.json)
- [资源与生命周期核查](resource/findings.md)／[资源卡](resource/resource-card.json)／[资源来源manifest](resource/source-manifest.json)
- [近邻接口/方法](prior/findings.md)／[比较卡](prior/comparison-card.json)／[近邻来源manifest](prior/source-manifest.json)／[新源码取得回执](prior/download-receipts.json)
- [50hash/38路径/58行界核验](root/source-verification.json)／[实际执行账](root/execution-ledger.json)／[本轮精确同步合同](root/canonical-sync-contract.json)
- [文档更新账](documentation/update-ledger.json)／[保护和链接核验](documentation/verification.json)／[即时写前字节/hash](documentation/before-write-manifest.json)／[变更清单](documentation/changed-files.json)
- [前轮有限费用账及数字](../mapping-admission-20261016/README.md)／[旧整页协议历史](../mapping-organization-source-20261016/README.md)／[B1证据入口](../b1-stripe-cost-20261016/README.md)

V1空闲cache不加普通page/pp pin，短时TX token先于frame归还，真正source退出前撤B图；持久map仍需每次sync。FIFO handle替换、条件复用正序和反例均为设计，未编译运行。相同工作/服务下CPU或不可回收内存改善可作主要价值，无需抽象再确认；实际部署未知只作条件事实。

下一OWNER-BINDING-INTEGRATION-DELTA-1仅局部字段/调用/锁/owner责任差项，已指定未执行，不自动开发。相同接口普通O3、合理分组/预准备/range适配dma-buf及owner-bound RFC均允许相同表示，独立贡献须有具体整合成本或部署差异。

本轮3个新固定commit源码HTTP，旧PDF与源码复用；0功能/性能模型、旧阈值账重跑、原生/构建/生产/设备动作。50hash/38路径/58行界只核来源定位。Codex-only，未用DeepSeek/Claude；实际独立模型/强度unknown。B1/I/D/A5A2、MIG暂停和10月15—28日窗口保持。最终交付状态以root单独保存的收尾回执为准。
'''
(R/'README.md').write_text(readme)
config_record={'requested_model':'gpt-6.1-sol','requested_reasoning_effort':'medium','actual_model':'unknown','actual_reasoning_effort':'unknown'}
changed=[{'path':n,'operation':'modified','before_bytes':len(before[n]),'before_sha256':sha(before[n]),'after_bytes':len(b),'after_sha256':sha(b)} for n,b in outputs.items()]
changed.append({'path':str((R/'README.md').relative_to(W)),'operation':'created','after_bytes':len(readme.encode()),'after_sha256':sha(readme.encode())})
changed_manifest={'canonical_and_readme':changed,'documentation_created':['sync.py','update-ledger.json','verification.json','before-write-manifest.json','changed-files.json']+[str(Path('before-write')/n) for n in before],'note':'Only allowed canonical documents, exact root AGENTS insertion, new run README and documentation deliverables; production and frozen outputs not edited.'}
(D/'changed-files.json').write_text(json.dumps(changed_manifest,ensure_ascii=False,indent=2)+'\n')
update={'task':'mapping-lifetime-design-20261016 final canonical sync','date':'2026-10-16 JST','configuration':config_record,'method':'single final sync after stable root acceptance, immediate live reread, exact independent/history byte preservation','summary':['retain V1 source owner/B binding/TX token separation as concrete hypothesis, no paper-mainline or benefit claim','ordinary same-interface O3 strategy increment zero; general notification prior art and concrete integration delta separated','CPU/unreclaimable-memory value principle accepted; deployment unknown is conditional fact, not pending abstract authorization','next OWNER-BINDING-INTEGRATION-DELTA-1 specified_not_executed; no automatic development','root AGENTS exact policy insertion only; model rules preserved'],'changes':'changed-files.json','before':'before-write-manifest.json','verification':'verification.json','execution':{'document_checks_only':True,'new_models_or_experiments_or_builds':0,'network_calls':0,'frozen_source_verification_rerun':False}}
(D/'update-ledger.json').write_text(json.dumps(update,ensure_ascii=False,indent=2)+'\n')
# Materialize final receipt pathname before validating links to it.
(D/'verification.json').write_text('{}\n')
checks={
 'CURRENT_A5_A2_exact':span(outputs[names[0]],'## 各分支最新进展','## 硬件与执行状态')==span(before[names[0]],'## 各分支最新进展','## 硬件与执行状态'),
 'CURRENT_independent_exact':span(outputs[names[0]],'## 独立工程事项')==span(before[names[0]],'## 独立工程事项'),
 'plan_I_and_later_exact':span(outputs[names[1]],'## I：')==span(before[names[1]],'## I：'),
 'navigation_assets_and_later_exact':span(outputs[names[2]],'## 已有成果怎样复用')==span(before[names[2]],'## 已有成果怎样复用'),
 **{f'complete_old_suffix_{n}':outputs[n].endswith(before[n]) for n in names[3:]},
 'root_AGENTS_exact_insertion_only':(W/'AGENTS.md').read_bytes()==agents_after and agents_after.replace(insertion,b'',1)==before['AGENTS.md'],
 'platform_AGENTS_full_exact':(W/'iommu-platform/AGENTS.md').read_bytes()==before['iommu-platform/AGENTS.md'],
 'root_AGENTS_model_rules_exact':span(agents_after,'## 按任务选择模型与推理强度','## 研究方向不默认沿用A')==span(before['AGENTS.md'],'## 按任务选择模型与推理强度','## 研究方向不默认沿用A'),
 **{f'before_bytes_and_hash_{n}':(D/'before-write'/n).read_bytes()==before[n] and sha((D/'before-write'/n).read_bytes())==sha(before[n]) for n in before},
 **{f'written_bytes_{n}':(W/n).read_bytes()==b for n,b in outputs.items()},
 **{f'frozen_unchanged_{n}':sha((R/n).read_bytes())==h for n,h in stable.items()},
 'README_saved_bytes_hash':(R/'README.md').read_bytes()==readme.encode(),
}
pattern=re.compile(r'\[[^\]\n]*\]\(([^)\n]+)\)')
def targets(s):return {t.split('#',1)[0].strip('<>') for t in pattern.findall(s) if not re.match(r'^[A-Za-z][A-Za-z0-9+.-]*:',t) and not t.startswith('#')}
def exists(n,t):
 t=re.sub(r':\d+(?::\d+)?$','',t);return (Path(t) if t.startswith('/') else (W/n).parent/t).exists()
new_links=[];old_bad=[]
for n,b in outputs.items():
 old_targets=targets(before[n].decode())
 for t in sorted(targets(b.decode())):
  e={'file':n,'target':t,'exists':exists(n,t)}
  if t not in old_targets:new_links.append(e)
  elif not e['exists']:old_bad.append(e)
rn=str((R/'README.md').relative_to(W))
for t in sorted(targets(readme)):new_links.append({'file':rn,'target':t,'exists':exists(rn,t)})
checks['new_local_links_all_exist']=all(e['exists'] for e in new_links)
checks['historical_broken_targets_still_14']=len(old_bad)==14
verification={'task':'mapping-lifetime-design-20261016 canonical delivery','configuration':config_record,'checks':checks,'check_count':len(checks),'passed':all(checks.values()),'before_write':'before-write-manifest.json','changes':'changed-files.json','after':{n:{'bytes':len(b),'sha256':sha(b)} for n,b in outputs.items()},'README':{'path':rn,'bytes':len(readme.encode()),'sha256':sha(readme.encode())},'AGENTS_exact_allowed_insertion':{'anchor':contract['agents_insertion_anchor'],'insert_exact':contract['agents_insert_exact'],'all_other_bytes_preserved':checks['root_AGENTS_exact_insertion_only']},'frozen_output_hashes':stable,'new_local_links':new_links,'new_local_link_count':len(new_links),'preexisting_broken_local_links':old_bad,'preexisting_broken_count':len(old_bad),'link_check_method':'unique(document,target), strip fragment, normalize :line[:column]; all README links new; pre-existing invalid targets retained in untouched history, no claim they were valid before archive','execution':'document-byte/hash/link checks only; no source50/range58 rerun, models, old fee script, build, production or device action'}
(D/'verification.json').write_text(json.dumps(verification,ensure_ascii=False,indent=2)+'\n')
assert verification['passed'],json.dumps({k:v for k,v in checks.items() if not v})
print(json.dumps({'passed':True,'checks':len(checks),'new_links':len(new_links),'preexisting_broken':len(old_bad)},ensure_ascii=False))
