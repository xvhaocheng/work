from pathlib import Path
import hashlib,json,re,urllib.parse,collections,datetime
W=Path('/home/xvhaocheng/work');R=W/'iommu-platform/out/d-dram-action-20261015';D=R/'documentation'
paths=['CURRENT_STATUS.md','研究计划.md','研究导航.md','HANDOFF.md','iommu-platform/docs/validation.md']
def sha(b):return hashlib.sha256(b).hexdigest()
def links(b):
    # Inline Markdown local destination existence only; external URLs and fragment semantics are out of scope.
    found=[]
    for m in re.finditer(rb'\[[^\]\n]*\]\(([^)\n]+)\)',b):
        raw=m.group(1).decode().strip()
        if raw.startswith('<') and raw.endswith('>'):raw=raw[1:-1]
        if re.match(r'^[A-Za-z][A-Za-z0-9+.-]*:',raw) or raw.startswith('#'):continue
        target=urllib.parse.unquote(raw.split('#',1)[0].split('?',1)[0])
        if target:found.append(target)
    return collections.Counter(found)
def exists(p,target):
    q=Path(re.sub(r':\d+(?:-\d+)?$','',target))
    if not q.is_absolute():q=(W/p).parent/q
    return q.exists()
before={p:(D/'before'/p).read_bytes() for p in paths};after={p:(W/p).read_bytes() for p in paths}
checks=[]
def check(name,ok,**detail):checks.append({'name':name,'pass':bool(ok),**detail})
manifest=json.loads((D/'before/manifest.json').read_text())
for p,b in before.items():check('full_baseline:'+p,sha(b)==manifest[p]['sha256'],sha256=sha(b),bytes=len(b))
blocks=[('CURRENT_STATUS.md','## 各分支最新进展','## 硬件与执行状态'),('CURRENT_STATUS.md','## 独立工程事项',None),('研究计划.md','## I：',None),('研究导航.md','## 已有成果怎样复用',None)]
protected=[]
for p,start,end in blocks:
    def extract(b):
        a=b.index(start.encode());z=b.index(end.encode(),a) if end else len(b);return b[a:z]
    a,z=extract(before[p]),extract(after[p]);entry={'path':p,'start':start,'end':end,'bytes':len(a),'before_sha256':sha(a),'after_sha256':sha(z)};protected.append(entry);check('protected:'+p+':'+start,a==z,**entry)
for p in ['HANDOFF.md','iommu-platform/docs/validation.md']:
    b=before[p];a=after[p];check('original_exact_suffix:'+p,a.endswith(b),bytes=len(b),sha256=sha(b),prepended_bytes=len(a)-len(b))
for p,h in json.loads((D/'before/instruction-hashes.json').read_text()).items():check('instructions_untouched:'+p,sha((W/p).read_bytes())==h,sha256=h)
baseline_bad=[];new_bad=[];all_bad=[];counts={};newlinks=0
for p in paths+['iommu-platform/out/d-dram-action-20261015/README.md','iommu-platform/out/d-dram-action-20261015/documentation/README.md']:
    a=(W/p).read_bytes();old=links(before[p]) if p in before else collections.Counter();now=links(a);added=now-old
    counts[p]={'all_local_link_occurrences':sum(now.values()),'distinct_local_targets':len(now),'introduced_local_link_occurrences':sum(added.values())}
    newlinks+=sum(added.values())
    for target,n in old.items():
        if not exists(p,target):baseline_bad.append({'path':p,'target':target,'occurrences':n})
    for target,n in now.items():
        if not exists(p,target):all_bad.append({'path':p,'target':target,'occurrences':n})
    for target,n in added.items():
        if not exists(p,target):new_bad.append({'path':p,'target':target,'occurrences':n})
check('introduced_local_links_exist',not new_bad,introduced_link_occurrences=newlinks,new_bad_target_count=len(new_bad))
# Necessary current conclusions/guards have direct presence checks, without claiming semantic proof.
checks_text={
 'current_title_and_decision':('CURRENT_STATUS.md',['完整准备使选定DRAM前史对收敛','D-BOUNDARY-SOURCE-2','尚未执行','0次完整64/64测量']),
 'plan_current_boundary':('研究计划.md',['D-BOUNDARY-SOURCE-2','Linux可请求warm不授权降低固定M0 warm256','同成本wait/replay']),
 'navigation_current_chain':('研究导航.md',['d-dram-action-20261015/root/decision-analysis.md','D-BOUNDARY-SOURCE-2']),
 'readme_snapshot_time':('iommu-platform/out/d-dram-action-20261015/README.md',['构建时快照','后续实际运行与接受','0次完整64/64测量','WR+34'])}
for name,(p,needles) in checks_text.items():
    s=(W/p).read_text();check(name,all(n in s for n in needles),needles=needles)
report={'scope':'deterministic document byte protection, inline local destination existence, and bounded current-entry presence checks only; no model/build/old experiment/raw checker run','utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'suggested_configuration':'gpt-6.1-sol/medium','requested_configuration':'gpt-6.1-sol/medium','actual_model_effort':'unknown','check_count':len(checks),'passed_check_count':sum(c['pass'] for c in checks),'failed_check_count':sum(not c['pass'] for c in checks),'checks':checks,'protected_blocks':protected,'local_link_counts':counts,'introduced_local_link_occurrences':newlinks,'introduced_broken_links':new_bad,'baseline_existing_broken_links':baseline_bad,'all_current_broken_links':all_bad,'new_readme_sha256':sha((R/'README.md').read_bytes()),'production_sources_modified_by_this_task':False,'AGENTS_modified_by_this_task':False,'raw_files_rehashed':False,'stable_acceptance_modified_by_this_task':False}
(D/'receipt.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({k:report[k] for k in ['check_count','passed_check_count','failed_check_count','introduced_local_link_occurrences','introduced_broken_links']},ensure_ascii=False))
print('baseline_existing_broken_targets='+str(len(baseline_bad)))
if report['failed_check_count']:raise SystemExit(1)
