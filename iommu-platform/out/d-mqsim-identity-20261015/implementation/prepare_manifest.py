from pathlib import Path
import hashlib,json,difflib
p=Path(__file__).resolve().parent;old=p.parents[1]/'d-persistent-source-20261015/offline/instrumented';new=p/'source'
def sha(f):return hashlib.sha256(f.read_bytes()).hexdigest()
def files(r):return {str(f.relative_to(r)):f for f in r.rglob('*') if f.is_file() and 'build' not in f.relative_to(r).parts and f.relative_to(r)!=Path('MQSim')}
a,b=files(old),files(new);patch=[]
for f in sorted(a.keys()|b.keys()):
 if f in a and f in b and sha(a[f])==sha(b[f]):continue
 patch.extend(difflib.unified_diff(a[f].read_text().splitlines(True) if f in a else [],b[f].read_text().splitlines(True) if f in b else [],fromfile='a/'+f if f in a else '/dev/null',tofile='b/'+f if f in b else '/dev/null'))
(p/'identity-observer.patch').write_text(''.join(patch))
manifest={'old_source':{f:sha(v) for f,v in sorted(a.items())},'new_source':{f:sha(v) for f,v in sorted(b.items())},'scripts_patch':{f.name:sha(f) for f in p.iterdir() if f.is_file() and f.suffix in ('.py','.patch')},'contract':sha(p.parent/'root/identity-contract.json'),'actual_simulation_runs':0}
assert manifest['old_source']==json.loads((p/'build1-pre-build-manifest.json').read_text())['old_source']
(p/'pre-build-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
