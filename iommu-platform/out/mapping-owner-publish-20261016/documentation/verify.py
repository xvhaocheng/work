from pathlib import Path
import json,hashlib,re,urllib.parse
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/mapping-owner-publish-20261016';D=R/'documentation'
def inf(b):return {'bytes':len(b),'sha256':hashlib.sha256(b).hexdigest()}
def load(p):return json.loads(p.read_text())
def dump(p,x):p.write_text(json.dumps(x,ensure_ascii=False,indent=2)+'\n')
checks={}; manifest=load(D/'before-write-manifest.json'); paths=[x['path'] for x in load(D/'changed-files.json')['files']]
old={x['path']:(D/'before-write'/x['path']).read_bytes() for x in manifest['files']}
new={p:(W/p).read_bytes() for p in paths}
for x in manifest['files']: checks['snapshot_'+x['path']]=inf(old[x['path']])=={k:x[k] for k in ['bytes','sha256']}
for p in ['AGENTS.md','iommu-platform/AGENTS.md']: checks['unchanged_'+p]=(W/p).read_bytes()==old[p]
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']: checks['full_old_suffix_'+p]=new[p].endswith(old[p])
for p,start,end in [('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'),('CURRENT_STATUS.md','## 独立工程事项',None),('研究计划.md','## I：',None),('研究导航.md','## 已有成果怎样复用',None)]:
 def block(b):
  s=b.decode();s=s[s.index(start):];return s[:s.index(end)] if end else s
 checks['protected_'+p+'_'+start]=block(old[p])==block(new[p])
frozen=load(D/'frozen-before.json')
for p,v in frozen.items():checks['frozen_'+p]=inf((R/p).read_bytes())==v
accepted=load(R/'root/root-result-acceptance.json')['frozen_files']
checks['18_accepted_frozen_files']=len(accepted)==18 and all(inf((R/x['path']).read_bytes())=={k:x[k] for k in ['bytes','sha256']} for x in accepted)
# Record-only administrative scope: inspect links in prose, skipping fenced code and remote targets.
def links(text):
 text=re.sub(r'```.*?```','',text,flags=re.S)
 return set(re.findall(r'\[[^\]\n]*\]\(([^)\n]+)\)',text))
def local(p,t):
 t=t.strip().strip('<>');t=t.split('#',1)[0];t=re.sub(r':\d+(?::\d+)?$','',t)
 if not t or re.match(r'^[a-zA-Z][\w+.-]*:',t): return None
 return ((W/p).parent/urllib.parse.unquote(t)).exists()
# verification.json is an intended artifact; create it before reference existence checks.
dump(D/'verification.json',{'status':'checking'})
baseline=[]; after=[];current=[]
for p,b in old.items():
 if not p.endswith('.md'):continue
 for t in links(b.decode()):
  if local(p,t) is False:baseline.append({'file':p,'target':t,'exists':False})
for p,b in new.items():
 for t in links(b.decode()):
  exists=local(p,t)
  if exists is False:after.append({'file':p,'target':t,'exists':False})
  # histories only new prefix; all other current prose is current.
 current_text=b[:-len(old[p])].decode() if p in ['HANDOFF.md','iommu-platform/docs/validation.md'] else b.decode()
 for t in links(current_text):
  exists=local(p,t)
  if exists is not None:current.append({'file':p,'target':t,'exists':exists})
pairs=lambda xs:{(x['file'],x['target']) for x in xs}
checks['14_preexisting_broken_pairs_retained']=len(pairs(baseline))==14 and pairs(baseline)==pairs(after)
checks['all_current_new_local_links_exist']=all(x['exists'] for x in current)
# No stale next task is left in current canonical prose; full history suffixes intentionally remain.
checks['gate_in_current_plan_nav']=all('MAP-OWNER-INTEGRATION-CONTRIBUTION-GATE-1' in new[p].decode() for p in ['CURRENT_STATUS.md','研究计划.md','研究导航.md'])
checks['no_stale_unexecuted_protocol_in_current']=all('MAP-OWNER-RECALL-PUBLISH-1已指定、未执行' not in new[p].decode() for p in ['CURRENT_STATUS.md','研究计划.md','研究导航.md'])
for x in load(D/'changed-files.json')['files']:checks['written_'+x['path']]=inf((W/x['path']).read_bytes())==x['after']
json_paths=list(R.rglob('*.json'));json_errors=[]
for p in json_paths:
 try:load(p)
 except Exception as e:json_errors.append({'path':str(p.relative_to(R)),'error':str(e)})
checks['all_run_JSON_parse']=not json_errors
report={'passed':all(checks.values()),'checks':checks,'check_count':len(checks),'new_current_local_links':sorted(current,key=lambda x:(x['file'],x['target'])),'preexisting_broken_local_links':sorted(baseline,key=lambda x:(x['file'],x['target'])),'preexisting_broken_count':len(pairs(baseline)),'badpair_handling':'14 unique historical document,target pairs retained; no historical repairs','frozen_snapshot_count':len(frozen),'accepted_frozen_file_count':len(accepted),'all_run_JSON_count':len(json_paths),'json_errors':json_errors,'initial_to_before_write_drift':manifest['initial_to_before_write_drift'],'final_hashes':{p:inf(b) for p,b in new.items()},'actual_model':'unknown','actual_effort':'unknown','execution':'administrative document/hash/link/JSON checks only; no model/native/build/device actions','no_commit_push_by_this_seat':True}
dump(D/'verification.json',report)
print(json.dumps({'passed':report['passed'],'checks':len(checks),'failed':[k for k,v in checks.items() if not v],'historical_bad_pairs':len(pairs(baseline)),'new_or_current_local_links':len(current),'frozen_snapshot_count':len(frozen),'JSON_checked':len(json_paths)},ensure_ascii=False))
