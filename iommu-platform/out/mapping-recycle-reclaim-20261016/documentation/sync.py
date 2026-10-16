from pathlib import Path
from datetime import datetime, timezone
import json,hashlib,re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-recycle-reclaim-20261016';D=R/'documentation'
def h(b):return hashlib.sha256(b).hexdigest()
def meta(b):return {'bytes':len(b),'sha256':h(b)}
def js(p,o):p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(o,ensure_ascii=False,indent=2)+'\n')
targets=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md',str((R/'README.md').relative_to(W))];agents=['AGENTS.md','iommu-platform/AGENTS.md']
before={p:(W/p).read_bytes() if (W/p).exists() else None for p in targets+agents};assert before[targets[-1]] is None
for p,b in before.items():
 if b is not None:
  q=D/'before-write'/p;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(b)
a=json.loads((R/'root/root-result-acceptance.json').read_text());assert len(a['frozen_files'])==15
assert all(meta((R/x['path']).read_bytes())=={'bytes':x['bytes'],'sha256':x['sha256']} for x in a['frozen_files'])
frozen={str(p.relative_to(R)):h(p.read_bytes()) for part in ['root','source-cut','closest-reclaim'] for p in (R/part).rglob('*') if p.is_file()}
previous=W/'iommu-platform/out/mapping-evidence-input-20261016';oldrun={str(p.relative_to(W)):h(p.read_bytes()) for p in previous.rglob('*') if p.is_file()}
js(D/'before-write-manifest.json',{'utc':datetime.now(timezone.utc).isoformat(),'files':{p:meta(b) if b is not None else {'exists':False} for p,b in before.items()},'frozen_files':frozen,'accepted_frozen_count':15,'previous_run_hashes':oldrun,'method':'complete immediate original bytes; current GitHub synchronized facts retained; README absent before write'})
P='iommu-platform/out/mapping-recycle-reclaim-20261016/'
def links(p):return f'[本轮入口]({p}README.md)／[裁决]({p}root/decision-analysis.md)／[假设卡]({p}root/hypothesis-card.json)／[接受]({p}root/root-result-acceptance.json)／[下一合同]({p}root/next-minimal-contract.json)'
done='MAP-RECYCLE-RECLAIM-1已完成，选中owner-assisted private alloc.cache-only扩展：有限K关联有压力时，由源allocation owner显式取出合格冷闲页，撤B图后放回源cache。首版不含ring；这是新设计差项，不是重复数据核查，尚未实现或测得净赢。'
conditions='旧V1-N/K固定B/config、K预留lazy sparse、满额O0且无idle eviction；本扩展能在冷页不再自然return/alloc时周转B关联。须owner实际服务、取页阻止再分配、唯一撤图与必要CPU metadata退休后才能返K；无返槽时限保证，O0仍保服务。return-only不能保证清沉cache冷项，外扫pp_ref==1也不能判idle。'
cost='不增独立逐TX/逐alloc use token，不等零热成本：miss合并pressure请求、owner唤醒/pump、扫描/lookup、unmap/sync、metadata退休、重建/churn及供给影响均计。'
napi='root新核driver schedule入口、pool.napi及positive-budget零RXwork仍到post的路径，可投影新control hook；budget0绕过。poll在RCU读侧，不能等自身grace；pending/失唤醒/关闭寿命协议尚未实现，不能当成已有可靠pump。'
ordinary='O0原本已有B-only卸图保留A；DMA_PMD已有安全idle隔离与分配器协同原理。普通R/slot/O3获得同接口可复现能力，不等已发表网络实现或已测完整成本相同；保留低完整实现成本整合假设，不认安全cut/token原则或策略独占创新，未选论文主线。'
resource='回收的是B关联库存，不等payload RAM减少，源pool的S仍在。CPU、真实S/P/C/Q并集及分配反馈、metadata/PTE、IOVA、临时取页/控制存储、带宽与完成分报；不套同一O0 PFN轨迹，原生数值仍null。'
nexttext='下一MAP-OWNER-RECALL-PUBLISH-1已指定、未执行：仅核请求/pending→owner→ack/退休的发布、失唤醒及pool/B关闭寿命，给源码绑定的手工正反序列。不自动driver/collector/RTL、状态或性能模型、数据扫描、旧账或板测，不等一般授权/价值或保证痛点。'
execution='12项worker来源hash/size和16项root读段行界通过，仅provenance；本轮手工条件序列不是运行结果。0下载、0新模型/原生/旧账/构建、0host设备probe、生产driver/RTL/ABI编辑或硬件动作。'
runtime='Codex-only；source-cut请求Sol/xhigh、closest Sol/high、文档Sol/medium，实际独立metadata unknown，未声称root切换。'
boundary='B1/I/D/A5/A2和并发GitHub/Linux/RTL独立事项保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。'
comp='[具名普通R](iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md)／[dma-buf slot比较](iommu-platform/out/mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)'
cur=before['CURRENT_STATUS.md'].decode();branch=cur[cur.index('## 各分支最新进展'):cur.index('## 硬件与执行状态')];independent=cur[cur.index('## 独立工程事项'):];assert 'GitHub 同步' in independent
current=f'''# 当前状态：owner冷cache关联回收扩展，下一核发布与关闭

2026年10月16日（JST）。**{done}** 保留低完整实现成本整合假设，未证独占创新或净收益。[研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- {conditions} {links(P)}。
- {napi} {nexttext}
- {cost} {ordinary} {comp}。{resource} [前轮输入验收](iommu-platform/out/mapping-evidence-input-20261016/README.md)已结束，本轮不重复。

{branch}## 硬件与执行状态

{execution} [执行账]({P}root/execution-ledger.json)。{runtime} {boundary}

{independent}'''
# Plan: replace only current paragraphs/table cells and add a concise new design section.
plan=before['研究计划.md'].decode();prefix,suffix=plan.split('## I：',1);pars=prefix.split('\n\n');pars[1]=f'2026年10月16日（JST）。**{done}** {nexttext} 原10月15—28日窗口和既有暂停保持，未选论文主线。';prefix='\n\n'.join(pars)
prefix=prefix.replace('本批明确入口验收已完成；当前无排队后继','输入验收已结束；具体cache-only设计差项触发本轮')
prefix=prefix.replace('公开前缀/3完整样本已验收为其他身份参考；native统计null','owner显式回收设计接受；下一有界发布/关闭协议')
prefix=prefix.replace('证明几何/调用来源/epoch；L0不升级accept/CQ/CPU','请求/ack/lifetime及失唤醒、自等待手工正反序列')
prefix=prefix.replace('仅具体新记录/配置或设计问题触发后继','仅协议结果及具体成本问题触发后继扩大')
oldgate='后继MAP-INPUT-READY-GATE为conditional_trigger_not_queued：这批入口验收结束，不自动重复相同搜索、合同或旧阈值账。仅在具体新record/config、另一身份或设计差项，或具名member及决定问题出现时触发有界后继；不等一般授权/保证痛点，不自动采集、开发、全包下载或板测。'
prefix=prefix.replace(oldgate,'前轮数据入口gate保持条件触发，不重查同批入口；本轮由具体设计差项触发。'+nexttext)
start=prefix.index('[本轮入口]');end=prefix.index('\n\n## 前轮来源',start)
prefix=prefix[:start]+f'''{links(P)}。前轮[公开输入验收](iommu-platform/out/mapping-evidence-input-20261016/README.md)及其partial样本边界保留；不是新原生数据。

## 当前owner-assisted cache-only容量扩展

{conditions} {cost} {napi} 正常隔离idle页不另要求B全局DMA drain，真正target关闭才需禁新及真实drain。{ordinary} {resource} [安全cut]({P}source-cut/findings.md)／[最接近工作]({P}closest-reclaim/findings.md)。

'''+prefix[end+2:]
prefix=prefix.replace('## V1-N/K与既有研究边界','## 旧V1-N/K与既有研究边界（历史基线）',1);plan=prefix+'## I：'+suffix
# Navigation: keep evidence rows, add current design rows and relabel the completed data gate.
nav=before['研究导航.md'].decode();prefix,suffix=nav.split('## 已有成果怎样复用',1)
prefix=prefix.replace('MAP-EVIDENCE-INPUT-1已完成；取得真实公开前缀/3完整样本，仅作其他身份参考。native统计null；后继gate仅条件触发、不排队。','MAP-RECYCLE-RECLAIM-1已完成，保留owner-assisted private-cache回收扩展；下一发布/ack/lifetime合同已指定未执行，非重复数据核查。native统计仍null。')
marker='| --- | --- | --- |\n';rows=f'''| 当前cache-only扩展 | owner显式取private alloc.cache闲页撤B后放回；返K须必要metadata退休，非payload RAM收益 | [本轮入口]({P}README.md)／[假设卡]({P}root/hypothesis-card.json)／[安全cut]({P}source-cut/findings.md)／[前作比较]({P}closest-reclaim/findings.md) |
| MAP-OWNER-RECALL-PUBLISH-1 | 已指定未执行；只核请求/ack/寿命、失唤醒和RCU自等待，不自动实现 | [下一合同]({P}root/next-minimal-contract.json) |
''';prefix=prefix.replace(marker,marker+rows,1)
prefix=prefix.replace('| MAP-EVIDENCE-INPUT-1 / 后继gate |','| 前轮MAP-EVIDENCE-INPUT-1 / 数据gate |',1)
prefix=prefix.replace('[本轮入口](iommu-platform/out/mapping-reuse-opportunity-20261016/README.md)','[前轮来源](iommu-platform/out/mapping-reuse-opportunity-20261016/README.md)')
prefix=prefix.replace('[本轮入口](iommu-platform/out/mapping-evidence-input-20261016/README.md)','[前轮输入验收](iommu-platform/out/mapping-evidence-input-20261016/README.md)')
start=prefix.index('20项worker来源hash/size');end=prefix.index('\n\n',start);prefix=prefix[:start]+execution+' '+links(P)+'。原窗口及MIG暂停保持，未选论文主线。'+prefix[end:];nav=prefix+'## 已有成果怎样复用'+suffix
def history(p):return f'''# 2026年10月16日 MAP-RECYCLE-RECLAIM-1：owner冷cache关联回收扩展

{done} {conditions}

{cost} {napi} 正常已隔离idle页无需额外B全局drain，真正target关闭另须真实DMA drain。{ordinary} {resource}

{nexttext} {execution} {runtime} {boundary}

{links(p)}／[文档验收]({p}documentation/verification.json)。以下完整旧字节后缀、现行保护块、双AGENTS与14个历史坏链均保留。

'''
readme=f'''# 源owner显式回收冷cache的B关联：有界设计接受

2026-10-16 JST。**{done}** {conditions}

{cost} {napi}

{ordinary} 详见本轮[最接近工作比较](closest-reclaim/findings.md)、前轮[具名普通R](../mapping-integration-delta-20261016/owner/integration-analysis.md)与[dma-buf slot/聚合适配](../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)。正常隔离idle页不额外要求全B排空；target关闭才须禁新及真实DMA drain。

{resource}

{nexttext}

| 内容 | 入口 |
| --- | --- |
| root裁决 | [主报告](root/decision-analysis.md)／[假设卡](root/hypothesis-card.json)／[稳定接受](root/root-result-acceptance.json)／[下一合同](root/next-minimal-contract.json) |
| source-cut席 | [来源与安全cut](source-cut/findings.md)／[cut卡](source-cut/cut-card.json)／[来源manifest](source-cut/source-manifest.json) |
| closest席 | [普通方案/前作](closest-reclaim/findings.md)／[比较卡](closest-reclaim/comparison-card.json)／[来源manifest](closest-reclaim/source-manifest.json) |
| 本轮执行 | [执行账](root/execution-ledger.json)／[provenance核验](root/source-verification.json)／[同步合同](root/canonical-sync-contract.json) |
| 文档回执 | [即时完整快照](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[变更清单](documentation/changed-files.json)／[验收](documentation/verification.json) |
| 历史基线 | [输入验收已结束](../mapping-evidence-input-20261016/README.md)／[正常再用/观测合同](../mapping-reuse-opportunity-20261016/README.md)／[旧V1整合](../mapping-integration-delta-20261016/README.md)／[寿命设计](../mapping-lifetime-design-20261016/README.md) |

{execution} {runtime} {boundary}

文档席只同步冻结接受，无研究重跑、源码/模型/驱动/设备动作。14个既存历史document-target坏链照录未修。
'''
new={'CURRENT_STATUS.md':current.encode(),'研究计划.md':plan.encode(),'研究导航.md':nav.encode(),'HANDOFF.md':history(P).encode()+before['HANDOFF.md'],'iommu-platform/docs/validation.md':history('../out/mapping-recycle-reclaim-20261016/').encode()+before['iommu-platform/docs/validation.md'],targets[-1]:readme.encode()}
assert all(((W/p).read_bytes() if (W/p).exists() else None)==b for p,b in before.items()),'concurrent update: reread/merge required'
for p,b in new.items():(W/p).write_bytes(b)
checks={}
for p,b in before.items():checks['snapshot_'+p]=(D/'before-write'/p).read_bytes()==b if b is not None else not (D/'before-write'/p).exists()
for p in agents:checks['unchanged_'+p]=(W/p).read_bytes()==before[p]
checks['CURRENT_branches_exact']=branch.encode() in new['CURRENT_STATUS.md'];checks['CURRENT_independent_GitHub_exact']=new['CURRENT_STATUS.md'].endswith(independent.encode())
for p,mark in [('研究计划.md','## I：'),('研究导航.md','## 已有成果怎样复用')]:checks['protected_suffix_'+p]=new[p].endswith(before[p][before[p].index(mark.encode()):])
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']:checks['old_full_suffix_'+p]=new[p].endswith(before[p])
checks['15_accepted_frozen_match']=all(meta((R/x['path']).read_bytes())=={'bytes':x['bytes'],'sha256':x['sha256']} for x in a['frozen_files'])
for p,s in frozen.items():checks['frozen_'+p]=h((R/p).read_bytes())==s
checks['previous_run_unchanged']=all(h((W/p).read_bytes())==s for p,s in oldrun.items())
for p,b in new.items():checks['written_'+p]=(W/p).read_bytes()==b
prior=json.loads((previous/'documentation/verification.json').read_text());bad=prior['preexisting_broken_local_links'];checks['14_badpairs_unchanged']=len(bad)==14 and all(not ((W/x['file']).parent/x['target']).exists() for x in bad)
changes={p:{'before':meta(before[p]) if before[p] is not None else None,'after':meta(b),'mode':'prepend_full_original_suffix' if p in ['HANDOFF.md','iommu-platform/docs/validation.md'] else 'targeted_current_prefix_edits' if p in ['研究计划.md','研究导航.md'] else 'allowed_current_sections' if before[p] is not None else 'create'} for p,b in new.items()}
js(D/'changed-files.json',{'files':changes,'documentation_directory':str(D.relative_to(W)),'AGENTS_changed':False,'production_changed':False})
js(D/'update-ledger.json',{'date':'2026-10-16','completed':'MAP-RECYCLE-RECLAIM-1','next':'MAP-OWNER-RECALL-PUBLISH-1 specified_not_executed','configuration':{'requested_model':'gpt-6.1-sol','requested_reasoning':'medium','actual_model':'unknown','actual_reasoning':'unknown'},'authorship_execution':'Codex-only documentation seat; no DeepSeek/Claude','files':list(changes),'concurrency':'complete immediate reads plus unchanged-at-write guard; latest GitHub/upload/standing-sync independent suffix retained byte-for-byte','execution':'document byte/hash/link checks only; no new research, download, provenance-source rerun, old account, build or device actions','top_level_git_sync':'root coordinates final reviewed batch; documentation seat does not mutate shared index'})
js(D/'verification.json',{});texts={p:b.decode() for p,b in new.items()};texts['HANDOFF.md']=history(P);texts['iommu-platform/docs/validation.md']=history('../out/mapping-recycle-reclaim-20261016/');ls=[]
for p,t in texts.items():
 for dest in sorted(set(re.findall(r'\[[^\]]*\]\(([^)]+)\)',t))):
  if '://' in dest or dest.startswith('#'):continue
  path=re.sub(r':\d+(?::\d+)?$','',dest.strip('<>').split('#')[0]);ls.append({'file':p,'target':dest,'exists':((W/p).parent/path).exists()})
checks['all_current_and_new_links_exist']=all(x['exists'] for x in ls)
assert all(checks.values()),[k for k,v in checks.items() if not v]
js(D/'verification.json',{'passed':True,'checks':checks,'check_count':len(checks),'new_local_links':ls,'new_local_link_count':len(ls),'preexisting_broken_local_links':bad,'preexisting_broken_count':14,'badpair_handling':'same prior 14 unique(document,target); histories untouched; not repaired','final_hashes':{p:meta(b) for p,b in new.items()},'accepted_frozen_file_count':15,'all_research_files_protected':len(frozen),'previous_run_files_protected':len(oldrun),'before_write_manifest':'before-write-manifest.json','execution':'administrative document checks only; source provenance/range checks not rerun','actual_model':'unknown','actual_reasoning':'unknown'})
print(json.dumps({'passed':True,'checks':len(checks),'links':len(ls),'accepted_frozen':15,'all_research_files':len(frozen),'CURRENT_lines':len(current.splitlines()),'files':list(changes)},ensure_ascii=False))
