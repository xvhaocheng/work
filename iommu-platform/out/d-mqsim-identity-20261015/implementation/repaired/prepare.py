from pathlib import Path
import hashlib,json,difflib
from datetime import datetime,timezone
p=Path(__file__).resolve().parent;old=p.parent/'source';new=p/'source';study=p.parents[1];offline=study.parent/'d-persistent-source-20261015/offline'
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
def files(r):return {str(f.relative_to(r)):f for f in r.rglob('*') if f.is_file() and 'build' not in f.relative_to(r).parts and f.relative_to(r)!=Path('MQSim')}
a,b=files(old),files(new);patch=[]
for f in sorted(a.keys()|b.keys()):
 if f in a and f in b and sha(a[f])==sha(b[f]):continue
 patch.extend(difflib.unified_diff(a[f].read_text().splitlines(True) if f in a else [],b[f].read_text().splitlines(True) if f in b else [],fromfile='a/'+f,tofile='b/'+f))
(p/'repair.patch').write_text(''.join(patch))
contracts=[study/'root/repair-contract.json',study/'root/identity-contract.json',study/'design/repair-contract.md']
inputs=[]
for n in (512,2048,4096,8192):inputs += [offline/'runs'/str(n)/'instrumented/ssdconfig.xml',offline/'runs'/str(n)/'instrumented/workload.xml',offline/'inputs'/('write-'+str(n)+'.trace')]
m={'utc':datetime.now(timezone.utc).isoformat(),'baseline_source':{f:sha(v) for f,v in sorted(a.items())},'new_source':{f:sha(v) for f,v in sorted(b.items())},'scripts_patch':{str(f):sha(f) for f in p.iterdir() if f.is_file() and f.suffix in ('.py','.patch')},'contracts':{str(f):sha(f) for f in contracts},'inputs':{str(f):sha(f) for f in inputs},'actual_simulation_runs':0,'build_sequence_number':5}
(p/'pre-build-manifest.json').write_text(json.dumps(m,indent=2)+'\n')
