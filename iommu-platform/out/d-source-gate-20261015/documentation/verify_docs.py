from pathlib import Path
import hashlib,json,re,datetime
R=Path('/home/xvhaocheng/work');run=R/'iommu-platform/out/d-source-gate-20261015';D=run/'documentation'
paths=['研究计划.md','研究导航.md','CURRENT_STATUS.md','HANDOFF.md','iommu-platform/docs/validation.md','iommu-platform/out/d-source-gate-20261015/README.md']
sha=lambda b:hashlib.sha256(b).hexdigest()
changes=[];checks={};links=[]
for n in paths:
 p=R/n;after=p.read_bytes();bp=D/'before'/n;before=bp.read_bytes() if bp.exists() else None
 changes.append({'path':n,'before_sha256':sha(before) if before is not None else None,'after_sha256':sha(after),'before_bytes':len(before) if before is not None else None,'after_bytes':len(after)})
 if n in ['HANDOFF.md','iommu-platform/docs/validation.md']:
  assert after.endswith(before);new=after[:-len(before)];checks[n+'_old_bytes_preserved']=True
 else:new=after
 for target in re.findall(r'\]\(([^)]+)\)',new.decode()):
  if '://' in target or target.startswith('#'):continue
  t=target.split('#')[0];q=(p.parent/t).resolve();exists=q.exists()
  preexisting=before is not None and target in re.findall(r'\]\(([^)]+)\)',before.decode())
  links.append({'from':n,'target':target,'exists':exists,'preexisting_broken':not exists and preexisting,'pending_root_generation':not exists and q in [run/'root/delivery-verification.json',run/'root/execution-ledger.json',D/'receipt.json',D/'checks.json',D/'changes.json']})
# Existing A5/A2 links and the independent engineering tail must survive unchanged.
status=(R/'CURRENT_STATUS.md').read_text();old=(D/'before/CURRENT_STATUS.md').read_text()
checks['independent_engineering_tail_exact']=status[status.index('## 独立工程事项'):]==old[old.index('## 独立工程事项'):]
checks['branch_A5_A2_section_exact']=status[status.index('## 各分支最新进展'):status.index('## 硬件与执行状态')]==old[old.index('## 各分支最新进展'):old.index('## 硬件与执行状态')]
for n in ['研究计划.md','研究导航.md','CURRENT_STATUS.md']:
 a=set(re.findall(r'\]\(([^)]+)\)',(D/'before'/n).read_text()));b=set(re.findall(r'\]\(([^)]+)\)',(R/n).read_text()))
 required={x for x in a if any(y in x.lower() for y in ['a5','a2'])};checks[n+'_A5_A2_links_preserved']=required<=b
checks['local_links']=links
checks['preexisting_broken_links']=[x for x in links if x['preexisting_broken']]
checks['unexplained_missing_links']=[x for x in links if not x['exists'] and not x['pending_root_generation'] and not x['preexisting_broken']]
checks['preservation_checks_passed']=all(v is True for k,v in checks.items() if k not in ['local_links','unexplained_missing_links','preexisting_broken_links'])
(D/'changes.json').write_text(json.dumps({'files':changes},ensure_ascii=False,indent=2)+'\n')
(D/'checks.json').write_text(json.dumps(checks,ensure_ascii=False,indent=2)+'\n')
receipt={'time_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'six owned documentation files only; documentation artifacts below own run','status':'pass' if checks['preservation_checks_passed'] and not checks['unexplained_missing_links'] else 'fail','history_byte_preservation':True,'branch_and_independent_preservation':True,'hashes':'changes.json','checks':'checks.json','before':'before-manifest.json','requested_model':'gpt-6.1-sol','requested_reasoning_effort':'medium','actual_model':'unknown','actual_reasoning_effort':'unknown','new_models_experiments_builds_board_operations':0,'goal_scope':'新建本轮来源判别goal；旧MIG及其他既有goal未改','pending_root_files':sorted({x['target'] for x in links if x['pending_root_generation'] and 'root/' in x['target']}),'preexisting_broken_links':checks['preexisting_broken_links'],'missing_target_scope':'旧链接目标当前缺失，删除来源未核；不是本轮新引入，未恢复或新建','correction':'goal-correction.json','semantics':'documentation integrity only; not system/performance validation'}
(D/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(receipt,ensure_ascii=False,indent=2));print('Unexplained missing links:',checks['unexplained_missing_links'])
