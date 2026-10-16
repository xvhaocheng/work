from pathlib import Path
import json,hashlib,re
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-lifetime-design-20261016';D=R/'documentation';P=D/'before-scope-revision'
if P.exists():raise SystemExit('Scope revision already executed; preserve its original predecessors.')
sha=lambda b:hashlib.sha256(b).hexdigest()
names=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md','AGENTS.md',str((R/'README.md').relative_to(W))]
for n in names:
 p=P/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((W/n).read_bytes())
for n in ['verification.json','update-ledger.json','changed-files.json']:(P/n).write_bytes((D/n).read_bytes())
replacements={
 'CURRENT_STATUS.md':[(
 '强对照允许普通O3同owner接口/FIFO表示、合理分组/预准备/range适配的dynamic dma-buf及owner-bound RFC适配；不能靠弱pin对照、一般通知或接口缺项宣称贡献。',
 '普通O3获得同owner接口/FIFO表示后完全等价只判策略增量0，不要求V1击败共享候选接口的O3才认可架构价值。接口/整合差异比较stock路径与合理分组/预准备/range适配的dynamic dma-buf、owner-bound RFC；只有这些普通适配也具相同完整能力/成本，才收当前版本为普通集成。')],
 '研究计划.md':[(
 '只有具体完整整合成本降低或同合同部署范围差异成立，才推进独立贡献；若相同接口/表示后等价，保留普通正确集成，不重复阈值账或因缺成本自动上系统。',
 'O3共享候选接口后的等价只判策略增量0，不要求V1击败它才认可接口/架构。架构比较stock路径及具名dma-buf/RFC普通适配的完整整合成本或同合同部署范围；若这些普通适配也具相同完整能力和成本，才收当前具体版本为普通集成。不重复阈值账或因缺成本自动上系统。')],
 '研究导航.md':[(
 '一般通知已有，O3同接口策略增量0；dma-buf合理分组/预准备/非睡眠接入等差项待核',
 'O3共享接口等价只判策略增量0；架构差项比较stock及具名dma-buf/RFC适配，不要求胜过共享接口O3')],
 str((R/'README.md').relative_to(W)):[(
 '相同接口普通O3、合理分组/预准备/range适配dma-buf及owner-bound RFC均允许相同表示，独立贡献须有具体整合成本或部署差异。',
 '同付费接口O3完全等价只判策略增量0，不要求V1击败共享候选接口的O3才认可接口/架构价值。架构差项比较stock路径和合理分组/预准备/range适配dma-buf、owner-bound RFC；只有这些普通适配也具相同完整能力与成本，才收当前具体整合为普通方案。')]
}
changes=[]
for n,pairs in replacements.items():
 p=W/n;before=p.read_bytes();s=before.decode()
 for old,new in pairs:
  assert s.count(old)==1,(n,old);s=s.replace(old,new,1)
 after=s.encode();assert p.read_bytes()==before;p.write_bytes(after)
 changes.append({'path':n,'before_sha256':sha(before),'after_sha256':sha(after),'exact_replacements':[{'old':a,'new':b} for a,b in pairs]})
accept=json.loads((R/'root/root-result-acceptance.json').read_text())
v=json.loads((D/'verification.json').read_text());old_frozen=v['frozen_output_hashes']
new_frozen={x['path']:sha((R/x['path']).read_bytes()) for x in accept['frozen_files']}
new_frozen['root/root-result-acceptance.json']=sha((R/'root/root-result-acceptance.json').read_bytes())
for x in accept['frozen_files']:assert new_frozen[x['path']]==x['sha256'],x['path']
def span(b,s,e=None):
 i=b.index(s.encode());return b[i:b.index(e.encode(),i)] if e else b[i:]
for n,s,e in [('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'),('CURRENT_STATUS.md','## 独立工程事项',None),('研究计划.md','## I：',None),('研究导航.md','## 已有成果怎样复用',None)]:
 assert span((W/n).read_bytes(),s,e)==span((D/'before-write'/n).read_bytes(),s,e),n
for n in ['HANDOFF.md','iommu-platform/docs/validation.md']:
 assert (W/n).read_bytes().endswith((D/'before-write'/n).read_bytes())
 assert (W/n).read_bytes()==(P/n).read_bytes()
assert (W/'AGENTS.md').read_bytes()==(P/'AGENTS.md').read_bytes()
c=json.loads((R/'root/canonical-sync-contract.json').read_text());insert=c['agents_insert_exact'].encode()
assert (W/'AGENTS.md').read_bytes().replace(insert,b'',1)==(D/'before-write/AGENTS.md').read_bytes()
assert (W/'iommu-platform/AGENTS.md').read_bytes()==(D/'before-write/iommu-platform/AGENTS.md').read_bytes()
for n in v['after']:
 b=(W/n).read_bytes();v['after'][n]={'bytes':len(b),'sha256':sha(b)}
b=(R/'README.md').read_bytes();v['README']['bytes']=len(b);v['README']['sha256']=sha(b)
v['frozen_output_hashes']=new_frozen
for n,h in new_frozen.items():v['checks']['frozen_unchanged_'+n]=sha((R/n).read_bytes())==h
for e in v['new_local_links']:
 t=re.sub(r':\d+(?::\d+)?$','',e['target']);p=Path(t) if t.startswith('/') else (W/e['file']).parent/t;e['exists']=p.exists();assert e['exists']
for e in v['preexisting_broken_local_links']:
 t=re.sub(r':\d+(?::\d+)?$','',e['target']);p=Path(t) if t.startswith('/') else (W/e['file']).parent/t;assert not p.exists()
assert len(v['preexisting_broken_local_links'])==14
v['checks']['scope_revision_original_protection_and_exact_AGENTS_insertion']=True
v['checks']['scope_revision_refrozen_artifacts_match_root_acceptance']=True
v['checks']['scope_revision_final_bytes_hashes_links_verified']=True
v['check_count']=len(v['checks']);v['passed']=all(v['checks'].values());v['scope_correction']='scope-revision.json';assert v['passed']
(D/'verification.json').write_text(json.dumps(v,ensure_ascii=False,indent=2)+'\n')
ch=json.loads((D/'changed-files.json').read_text())
for e in ch['canonical_and_readme']:
 b=(W/e['path']).read_bytes();e['after_bytes']=len(b);e['after_sha256']=sha(b)
ch['documentation_created']+=['scope-revision.py','scope-revision.json','before-scope-revision/ predecessors and original documentation receipts']
ch['bounded_correction']='scope-revision.json; initial before-write and first receipts preserved'
(D/'changed-files.json').write_text(json.dumps(ch,ensure_ascii=False,indent=2)+'\n')
u=json.loads((D/'update-ledger.json').read_text());u['bounded_post_review_correction']={'reason':'specific acceptance wording error, not new research','record':'scope-revision.json','changed_canonical':['CURRENT_STATUS.md','研究计划.md','研究导航.md'],'README_changed':True,'HANDOFF_validation_AGENTS_unchanged':True};(D/'update-ledger.json').write_text(json.dumps(u,ensure_ascii=False,indent=2)+'\n')
record={'reason':'root final_scope_clarification: same-interface O3 equivalence only limits strategy; architecture compared against stock and named ordinary adapters','changes':changes,'unchanged_reviewed':['HANDOFF.md new prefix','validation.md new prefix','AGENTS.md exact insertion'],'initial_before_write_preserved':True,'pre_revision_snapshots':'before-scope-revision/','root_refreeze_changed_hashes':{n:{'previous':old_frozen.get(n),'refrozen':h} for n,h in new_frozen.items() if old_frozen.get(n)!=h},'source_model_or_data_changed':False,'new_research_or_experiment_runs':0,'verification':'verification.json'}
(D/'scope-revision.json').write_text(json.dumps(record,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed':v['passed'],'checks':v['check_count'],'changed_documents':len(changes),'new_links':v['new_local_link_count'],'preexisting_broken':14}))
