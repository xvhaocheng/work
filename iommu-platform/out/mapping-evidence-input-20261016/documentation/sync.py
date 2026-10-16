from pathlib import Path
from datetime import datetime, timezone
import json, hashlib, re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-evidence-input-20261016';D=R/'documentation'
def h(b):return hashlib.sha256(b).hexdigest()
def meta(b):return {'bytes':len(b),'sha256':h(b)}
def js(p,o):p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(o,ensure_ascii=False,indent=2)+'\n')
targets=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md',str((R/'README.md').relative_to(W))];agents=['AGENTS.md','iommu-platform/AGENTS.md']
before={p:(W/p).read_bytes() if (W/p).exists() else None for p in targets+agents}
assert before[targets[-1]] is None
for p,b in before.items():
 if b is not None:
  q=D/'before-write'/p;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(b)
accept=json.loads((R/'root/root-result-acceptance.json').read_text());assert len(accept['frozen_files'])==39
assert all(meta((R/x['path']).read_bytes())=={'bytes':x['bytes'],'sha256':x['sha256']} for x in accept['frozen_files'])
frozen={str(p.relative_to(R)):h(p.read_bytes()) for part in ['root','pmd','paper-artifacts','iotlb-wall'] for p in (R/part).rglob('*') if p.is_file()}
prior=W/'iommu-platform/out/mapping-reuse-opportunity-20261016';oldrun={str(p.relative_to(W)):h(p.read_bytes()) for p in prior.rglob('*') if p.is_file()}
js(D/'before-write-manifest.json',{'utc':datetime.now(timezone.utc).isoformat(),'files':{p:meta(b) if b is not None else {'exists':False} for p,b in before.items()},'accepted_frozen_count':39,'frozen_files':frozen,'previous_run_hashes':oldrun,'method':'complete immediate originals including concurrent GitHub status; README absence explicitly recorded'})
P='iommu-platform/out/mapping-evidence-input-20261016/'
def links(p):return f'[本轮入口]({p}README.md)／[裁决]({p}root/decision-analysis.md)／[接受]({p}root/root-result-acceptance.json)／[输入判定]({p}root/input-verdicts.json)／[后继gate]({p}root/next-minimal-contract.json)'
done='MAP-EVIDENCE-INPUT-1已完成有界输入验收，实际取得公开数据前缀及3份完整小文本，只作其他身份参考。当前合格匹配输入0，原生再用、fullCPU与netgain仍为null；匹配输入0不等再用或收益0。'
scope='Zenodo7727193整包791,012,771 bytes，本轮仅HTTP206取前8,388,608 bytes；读到70个tar header，最后1项不完整，按固定规则选3个完整小文本。未验整包MD5/gzip，余包未审，不能据样本断言整包无事件。论文主要Linux5.15/iPerf TCP；所读CPU分类CSV与DAT汇总不具当前source/B map/CQ/fullCPU关系。'
gate='后继MAP-INPUT-READY-GATE为conditional_trigger_not_queued：这批入口验收结束，不自动重复相同搜索、合同或旧阈值账。仅在具体新record/config、另一身份或设计差项，或具名member及决定问题出现时触发有界后继；不等一般授权/保证痛点，不自动采集、开发、全包下载或板测。'
hyp='V1-N/K低完整实现成本整合假设保留：固定B/config、K预留lazy sparse关联、满额O0、无独立idle eviction，借真实frame/全部片段寿命整合。普通R/dma-buf slot/O3同享接口按各自覆盖范围判断，本轮未补完整实现成本差异，不新增novelty/netgain或共享干扰主张，未选论文主线。'
other='DMA_PMD cover仅作者聚合；DAMN是Linux4.7/ConnectX-4参考；DMA shadowing是ASPLOS2016、Linux3.19、被测Intel Fortville。两作者对应完整条目所查未给专属code/raw入口，不推全球不存在；lore HTTP200实际challenge，正文未知。'
execfact='20项worker来源hash/size及4项完整作者条目href检查通过，仅属provenance。保存8项HTTP响应含2份challenge；0原生运行、artifact执行、host新probe、模型/旧账、构建、生产Linux/RTL/ABI/collector或设备动作。'
runtime='Codex-only；两研究席请求Sol/high、文档Sol/medium，实际独立metadata unknown，未声称切换root。'
boundary='B1/I/D/A5/A2及独立工程保持；MIG1792023314继续paused，WNS −0.391ns、598项setup失败、0新加载、实际债务unknown；10月15—28日窗口不重启。'
resource='CPU、物理S/P/C/Q并集与策略分配反馈、metadata/PTE、IOVA、带宽及有效完成分别报告；stock无idle B pin，不作天然payload RAM节省主张。'
ordinary='[普通R比较](iommu-platform/out/mapping-integration-delta-20261016/owner/integration-analysis.md)／[dma-buf slot比较](iommu-platform/out/mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)'
cur=before['CURRENT_STATUS.md'].decode();branch=cur[cur.index('## 各分支最新进展'):cur.index('## 硬件与执行状态')];independent=cur[cur.index('## 独立工程事项'):];assert 'GitHub 同步' in independent
current=f'''# 当前状态：现成输入验收完成，后继仅条件触发

2026年10月16日（JST）。**{done}** 保留V1-N/K低完整成本整合假设，未证独立创新或净收益，未选论文主线。[研究计划](研究计划.md)为唯一总计划，[研究导航](研究导航.md)定位资产。

## 全局安排与下一证据

- {scope} {links(P)}。
- {gate}
- {hyp} {ordinary}。{resource} [前轮来源/观测合同](iommu-platform/out/mapping-reuse-opportunity-20261016/README.md)保留，不重复字段表。

{branch}## 硬件与执行状态

{execfact} [执行账]({P}root/execution-ledger.json)。{runtime} {boundary} 上轮[只读host身份](iommu-platform/out/mapping-reuse-opportunity-20261016/root/host-readonly-identity.json)未重测。

{independent}'''
# Minimal changes only inside the current plan prefix; established evidence text retained.
plan=before['研究计划.md'].decode();prefix,suffix=plan.split('## I：',1)
paras=prefix.split('\n\n');paras[1]=f'2026年10月16日（JST）。**{done}** {gate} 原10月15—28日窗口及暂停状态保持，未选论文主线。';prefix='\n\n'.join(paras)
prefix=prefix.replace('下一转已有匹配输入定位，不用更多相同合同代替数据','本批明确入口验收已完成；当前无排队后继')
prefix=prefix.replace('已有原始记录的身份与完整性验收，合格L0先粗筛','公开前缀/3完整样本已验收为其他身份参考；native统计null')
prefix=prefix.replace('依匹配输入评估正常重复和可归属完整费用','仅具体新记录/配置或设计问题触发后继')
start=prefix.index('[本轮入口]');end=prefix.index('\n\n## 来源与最小证据入口',start)
prefix=prefix[:start]+links(P)+'。本批仅其他身份参考；'+scope+'\n\n'+prefix[end+2:]
prefix=prefix.replace('## 来源与最小证据入口','## 前轮来源与最小证据入口（保留合同）',1)
old='下一MAP-EVIDENCE-INPUT-1已指定、未执行：只定向定位并验收已有匹配原始输入及构建/设备配置，先核身份再算合格L0。无匹配输入保留条件界与精确缺项，不重复同合同/原语表/旧阈值，不自动开发collector、驱动、模型或板测，也不等一般授权或用户保证痛点。'
prefix=prefix.replace(old,gate)
plan=prefix+'## I：'+suffix
# Navigation preserves existing rows except completed/next status and adds actual data entry.
nav=before['研究导航.md'].decode();prefix,suffix=nav.split('## 已有成果怎样复用',1)
prefix=prefix.replace('MAP-REUSE-OPPORTUNITY-1已完成；下一MAP-EVIDENCE-INPUT-1指定未执行，转已有匹配原始输入验收。','MAP-EVIDENCE-INPUT-1已完成；取得真实公开前缀/3完整样本，仅作其他身份参考。native统计null；后继gate仅条件触发、不排队。')
prefix=prefix.replace('| MAP-EVIDENCE-INPUT-1 | 已指定未执行，只验收已有匹配输入，不自动collector/驱动/板测 | [下一合同](iommu-platform/out/mapping-reuse-opportunity-20261016/root/next-minimal-contract.json) |',f'| MAP-EVIDENCE-INPUT-1 / 后继gate | 本批验收完成，匹配输入0非再用0；conditional_trigger_not_queued | [本轮入口]({P}README.md)／[输入判定]({P}root/input-verdicts.json)／[触发gate]({P}root/next-minimal-contract.json) |\n| 实际公开数据 | 仅8MiB prefix/70 header末项partial/3完整小样本，未验整包MD5，余包unknown | [样本manifest]({P}iotlb-wall/sample-manifest.json)／[prefix回执]({P}iotlb-wall/archive-prefix-receipt.json)／[PMD验收]({P}pmd/findings.md)／[论文入口验收]({P}paper-artifacts/findings.md) |')
start=prefix.index('本轮新增3份固定commit trace header HTTP');end=prefix.index('\n\n',start)
prefix=prefix[:start]+execfact+' '+links(P)+'。原窗口与MIG暂停保持，未选论文主线。'+prefix[end:]
nav=prefix+'## 已有成果怎样复用'+suffix
def hist(p):return f'''# 2026年10月16日 MAP-EVIDENCE-INPUT-1：公开输入有界验收完成

{done} {scope}

{other} {gate}

{hyp} {execfact} {runtime} {boundary}

{links(p)}／[文档验收]({p}documentation/verification.json)。旧全文以下按字节不变后缀保留，双AGENTS、现行保护块与14个历史坏链不改。

'''
readme=f'''# 现成输入验收：真实公开数据仅作具名参考

2026-10-16 JST。**{done}** {scope}

{other}

{hyp} 相对[普通R](../mapping-integration-delta-20261016/owner/integration-analysis.md)和[dma-buf slot/聚合适配](../mapping-integration-delta-20261016/dmabuf/adapter-analysis.md)的完整成本，本轮未新增证据。{resource}

{gate}

| 交付 | 入口 |
| --- | --- |
| root报告与判定 | [主报告](root/decision-analysis.md)／[稳定接受](root/root-result-acceptance.json)／[输入判定](root/input-verdicts.json)／[后继gate](root/next-minimal-contract.json) |
| 3份完整样本 | [首CSV](iotlb-wall/samples/first-csv.csv)／[CPU CSV](iotlb-wall/samples/first-cpu-csv.csv)／[DAT](iotlb-wall/samples/first-dat.dat)／[原member/偏移/尺寸/hash](iotlb-wall/sample-manifest.json) |
| 部分归档与来源 | [prefix回执](iotlb-wall/archive-prefix-receipt.json)／[70 header清单](iotlb-wall/archive-prefix-inventory.json)／[8MiB部分字节](iotlb-wall/raw-data-prefix.tar.gz.part)／[Zenodo元数据](iotlb-wall/zenodo-record.json)／[论文原文](iotlb-wall/pmc-article.txt) |
| 两席结果 | [PMD/RFC验收](pmd/findings.md)／[PMD输入卡](pmd/input-card.json)／[论文入口验收](paper-artifacts/findings.md)／[论文输入卡](paper-artifacts/input-card.json) |
| 执行与完整性 | [执行账](root/execution-ledger.json)／[来源核验](root/source-verification.json)／[同步合同](root/canonical-sync-contract.json) |
| 文档回执 | [即时完整快照](documentation/before-write-manifest.json)／[更新账](documentation/update-ledger.json)／[变更清单](documentation/changed-files.json)／[验收](documentation/verification.json) |
| 前轮资产 | [来源/观测合同](../mapping-reuse-opportunity-20261016/README.md)／[整合比较](../mapping-integration-delta-20261016/README.md)／[寿命设计](../mapping-lifetime-design-20261016/README.md)／[接纳账](../mapping-admission-20261016/README.md) |

{execfact} {runtime} {boundary}

未运行归档代码、补下载整包或重做来源研究。14个既存历史document-target坏链照录未修。
'''
new={'CURRENT_STATUS.md':current.encode(),'研究计划.md':plan.encode(),'研究导航.md':nav.encode(),'HANDOFF.md':hist(P).encode()+before['HANDOFF.md'],'iommu-platform/docs/validation.md':hist('../out/mapping-evidence-input-20261016/').encode()+before['iommu-platform/docs/validation.md'],targets[-1]:readme.encode()}
assert all(((W/p).read_bytes() if (W/p).exists() else None)==b for p,b in before.items()),'concurrent update: re-read/merge required'
for p,b in new.items():(W/p).write_bytes(b)
checks={}
for p,b in before.items():checks['snapshot_'+p]=(D/'before-write'/p).read_bytes()==b if b is not None else not (D/'before-write'/p).exists()
for p in agents:checks['unchanged_'+p]=(W/p).read_bytes()==before[p]
checks['CURRENT_branch_exact']=branch.encode() in new['CURRENT_STATUS.md'];checks['CURRENT_independent_GitHub_exact']=new['CURRENT_STATUS.md'].endswith(independent.encode())
for p,mark in [('研究计划.md','## I：'),('研究导航.md','## 已有成果怎样复用')]:checks['protected_suffix_'+p]=new[p].endswith(before[p][before[p].index(mark.encode()):])
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']:checks['old_full_suffix_'+p]=new[p].endswith(before[p])
checks['39_accepted_frozen_files_match']=all(meta((R/x['path']).read_bytes())=={'bytes':x['bytes'],'sha256':x['sha256']} for x in accept['frozen_files'])
for p,s in frozen.items():checks['frozen_'+p]=h((R/p).read_bytes())==s
checks['previous_run_all_unchanged']=all(h((W/p).read_bytes())==s for p,s in oldrun.items())
for p,b in new.items():checks['written_'+p]=(W/p).read_bytes()==b
v0=json.loads((prior/'documentation/verification.json').read_text());bad=v0['preexisting_broken_local_links'];checks['14_badpairs_remain']=len(bad)==14 and all(not ((W/x['file']).parent/x['target']).exists() for x in bad)
changes={p:{'before':meta(before[p]) if before[p] is not None else None,'after':meta(b),'mode':'prepend_full_original_suffix' if p in ['HANDOFF.md','iommu-platform/docs/validation.md'] else 'minimal_current_prefix_edits' if p in ['研究计划.md','研究导航.md'] else 'allowed_current_sections' if before[p] is not None else 'create'} for p,b in new.items()}
js(D/'changed-files.json',{'files':changes,'documentation_directory':str(D.relative_to(W)),'AGENTS_changed':False,'production_changed':False})
js(D/'update-ledger.json',{'date':'2026-10-16','completed':'MAP-EVIDENCE-INPUT-1','next':'MAP-INPUT-READY-GATE conditional_trigger_not_queued','configuration':{'requested_model':'gpt-6.1-sol','requested_reasoning':'medium','actual_model':'unknown','actual_reasoning':'unknown'},'authorship_execution':'Codex-only documentation seat; no DeepSeek/Claude','files':list(changes),'concurrency':'immediate originals re-read; unchanged-at-write guard; concurrent GitHub CURRENT suffix retained byte-for-byte','execution':'document bytes/hash/link checks only; no research, download, source-check rerun, model, old account, build or device action'})
js(D/'verification.json',{})
texts={p:b.decode() for p,b in new.items()};texts['HANDOFF.md']=hist(P);texts['iommu-platform/docs/validation.md']=hist('../out/mapping-evidence-input-20261016/');ls=[]
for p,t in texts.items():
 for dest in sorted(set(re.findall(r'\[[^\]]*\]\(([^)]+)\)',t))):
  if '://' in dest or dest.startswith('#'):continue
  path=re.sub(r':\d+(?::\d+)?$','',dest.strip('<>').split('#')[0]);ls.append({'file':p,'target':dest,'exists':((W/p).parent/path).exists()})
checks['all_current_and_new_links_exist']=all(x['exists'] for x in ls)
assert all(checks.values()),[k for k,v in checks.items() if not v]
js(D/'verification.json',{'passed':True,'checks':checks,'check_count':len(checks),'new_local_links':ls,'new_local_link_count':len(ls),'preexisting_broken_local_links':bad,'preexisting_broken_count':14,'badpair_handling':'same prior unique document-target pairs retained; full historical bytes untouched','final_hashes':{p:meta(b) for p,b in new.items()},'accepted_frozen_files':39,'all_research_files_protected':len(frozen),'previous_run_files_protected':len(oldrun),'before_write_manifest':'before-write-manifest.json','execution':'administrative document checks only; provenance checks not rerun','actual_model':'unknown','actual_reasoning':'unknown'})
print(json.dumps({'passed':True,'checks':len(checks),'links':len(ls),'accepted_frozen':39,'all_research_files':len(frozen),'CURRENT_lines':len(current.splitlines()),'files':list(changes)},ensure_ascii=False))
