from pathlib import Path
import json, hashlib, datetime, re
W=Path('/home/xvhaocheng/work'); R=W/'iommu-platform/out/d-persistent-source-20261015'; D=R/'documentation'
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
h=lambda b:hashlib.sha256(b).hexdigest()
before={p:(D/'before-write'/p).read_bytes() for p in paths}
manifest=json.loads((D/'before-write-manifest.json').read_text())
after={p:(W/p).read_text() for p in paths}
# Assertions use full byte segments, not paraphrases.
assert (W/'HANDOFF.md').read_bytes().endswith(before['HANDOFF.md'])
assert (W/'iommu-platform/docs/validation.md').read_bytes().endswith(before['iommu-platform/docs/validation.md'])
old=before['CURRENT_STATUS.md']; new=(W/'CURRENT_STATUS.md').read_bytes()
branch=old.split('## 各分支最新进展'.encode())[1].split('## 硬件与执行状态'.encode())[0]
engineering=old.split('## 独立工程事项'.encode())[1]
assert branch in new and new.endswith(engineering)
# Current canonical local links + newly prepended historical sections and new README only.
checks=[]
for p in paths+['iommu-platform/out/d-persistent-source-20261015/README.md']:
 text=(W/p).read_text()
 if p in ('HANDOFF.md','iommu-platform/docs/validation.md'):text=text[:-len(before[p].decode())]
 for m in re.finditer(r'\[[^\]]*\]\(([^)]+)\)',text):
  target=m.group(1).strip('<>')
  if target.startswith(('http:','https:','app:','codex:','#')):continue
  target=target.split('#')[0]
  q=Path(target) if target.startswith('/') else (W/p).parent/target
  checks.append({'document':p,'target':target,'exists':q.exists() or q.resolve()==(D/'receipt.json').resolve()})
assert all(c['exists'] for c in checks), [c for c in checks if not c['exists']]
receipt={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'canonical entry synchronization only; no new research/model/build/simulation by documentation worker','requested_model':'gpt-6.1-sol','requested_reasoning':'medium','actual_model_metadata':'unknown','files':[{**m,'after_sha256':h((W/m['path']).read_bytes())} for m in manifest], 'created_files':[{'path':str((R/'README.md').relative_to(W)),'before_sha256':None,'after_sha256':h((R/'README.md').read_bytes())}], 'protected_assertions':{'CURRENT_STATUS_A5_A2_branch_bytes':True,'CURRENT_STATUS_independent_engineering_suffix_bytes':True,'HANDOFF_entire_old_suffix_bytes':True,'validation_entire_old_suffix_bytes':True,'plan_retained_assets_and_hardware_suffix':after['研究计划.md'].split('## I：')[1]==before['研究计划.md'].decode().split('## I：')[1],'navigation_assets_code_and_history_suffix':after['研究导航.md'].split('## 已有成果')[1]==before['研究导航.md'].decode().split('## 已有成果')[1],'MIG_pause_preserved':all('paused' in after[p] or '暂停' in after[p] for p in paths),'no_old_method_restoration':not (W/'研究方法.md').exists()},'link_scope':'full three current canonical entries + new HANDOFF/validation prefixes + new run README; historical old suffix links not rescanned','local_link_count':len(checks),'local_links':checks,'actual_changes':{'CURRENT_STATUS.md':'replace D summary and execution paragraph; protected branch and engineering bytes retained','研究计划.md':'update current result, day18–19 step and next D action; other direction/asset/hardware text retained','研究导航.md':'update D row and next gate only','HANDOFF.md':'one new prefix, full old bytes as suffix','iommu-platform/docs/validation.md':'one new prefix, full old bytes as suffix','README.md':'new navigation to accepted source/decision/raw logs/full hash ledgers/next contract'}}
assert all(receipt['protected_assertions'].values())
(D/'receipt.json').write_text(json.dumps(receipt,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'files':len(manifest),'local_links':len(checks),'protected':receipt['protected_assertions'],'concurrent_changes':[m['path'] for m in manifest if m['changed_since_run_start']]},ensure_ascii=False,indent=2))
