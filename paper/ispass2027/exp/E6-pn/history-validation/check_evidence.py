#!/usr/bin/env python3
"""Verify frozen history-control finite evidence; does not run a simulation."""
import argparse,csv,gzip,hashlib,json,re,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent
checks=0
def require(ok,message):
 global checks
 checks+=1
 if not ok:raise AssertionError(message)
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--iommu-root',type=Path,required=True);args=ap.parse_args();repo=args.iommu_root.resolve()
 s=json.loads((HERE/'summary.json').read_text());commit=s['component_commit'];base=s['component_parent']
 for folder in sorted((HERE/'evidence').iterdir()):
  m=json.loads((folder/'manifest.json').read_text())
  for item in m['inputs']:
   original=Path(item['path']);source=next(p for p in original.parents if p.name=='source');f=folder/'source'/original.relative_to(source)
   require(sha(f.read_bytes())==item['sha256'],str(f))
  for n in ['build.log','run.log','cases.csv','cycle-trace.csv']:
   if n not in m:continue
   compressed=folder/(n+'.gz');raw=gzip.decompress(compressed.read_bytes()) if compressed.exists() else (folder/n).read_bytes()
   require(sha(raw)==m[n]['sha256'],str(folder/n))
 for item in s['source_commit_binding']:
  original=subprocess.check_output(['git','show',commit+':'+item['path']],cwd=repo)
  require(sha(original)==item['sha256'] and original==(HERE/'evidence'/item['execution']/'source'/Path(item['path']).name).read_bytes(),'commit/source '+item['path'])
 changed=subprocess.check_output(['git','diff','--name-only',base,commit],cwd=repo,text=True).splitlines()
 require(changed==s['changed_production_files'],'two-file scope')
 def raw(ref,name):return subprocess.check_output(['git','show',ref+':rtl/translation_logic/'+name+'.sv'],cwd=repo)
 def rename(data,name,replacement):
  data,n=re.subn(rb'(?m)^module '+name.encode()+rb'\b',b'module '+replacement.encode(),data,count=1);require(n==1,'module transform');return data
 for ref,target in [('85b157ebe71a623e326e51fff7b7126fe77e8eb9','gold'),('d2a813d67bcfd0295d8934c3cc027d24d3195d63','old')]:
  name='rv_iommu_iotlb';gold=rename(raw(ref,name),name,name+'_'+target)
  require(gold==(HERE/'evidence/history-iotlb03/source'/(name+'_'+target+'.sv')).read_bytes(),'IOTLB original '+target)
 name='rv_iommu_iotlb';require(rename(raw(base,name),name,name+'_old')==(HERE/'evidence/history-n03/source'/(name+'_old.sv')).read_bytes(),'old N IOTLB')
 for v in ['iso','shared']:
  name='rv_iommu_tl_wrap';gold=rename(raw(base,name),name,name+'_old_'+v)
  gold,n=re.subn(rb'\brv_iommu_iotlb\s*#\(',b'rv_iommu_iotlb_old #(',gold);require(n==1,'old TL cache bind')
  if v=='shared':
   require(gold.count(b'.PartitionLocalHistory(PolicyEnable)')==1,'shared existing parameter');gold=gold.replace(b'.PartitionLocalHistory(PolicyEnable)',b".PartitionLocalHistory(1'b0)")
  require(gold==(HERE/'evidence/history-n03/source'/(name+'_old_'+v+'.sv')).read_bytes(),'old TL original '+v)
 def rows(run):
  out=list(csv.DictReader(gzip.decompress((HERE/'evidence'/run/'cycle-trace.csv.gz').read_bytes()).decode().splitlines()));require(all(None not in r and None not in r.values() for r in out),'CSV shape '+run);return out
 xs=rows('history-iotlb03');require(len(xs)==8994,'IOTLB eval count')
 for r in xs:
  if r['rst']!='1':continue
  require(r['disabled_history_equal']=='1','compile-disabled history differs')
  require(r['shared_equal']=='1','fixed runtime shared differs')
  if r['clear']=='0':require(r['history_equal']=='1','selected old local/shared differs')
  if r['update']=='1' and r['clear']=='0' and int(r['mask'],16) in [3,65532]:require(not(int(r['victim'],16)&~int(r['mask'],16)),'fill escaped partition')
 ns=rows('history-n03');require(len(ns)==476,'TL cycle count')
 require(sum(int(r['n_accept']) for r in ns)==2,'two actual N accepts')
 for h in [0,1]:require(sum(int(r['n_accept']) for r in ns if int(r['history_isolate'])==h)==1,'accept in both internal histories')
 for r in ns:
  require(r['touch']==r['n_accept'],'only actual accept touches')
  if r['probe']=='1' and r['n_accept']=='0' and r['fill']=='0':require(r['no_probe_touch']=='1','probe altered PLRU')
 shared0=[r for r in ns if r['scenario']=='6'];require(shared0 and all(r['n_enable']=='0' and r['n_accept']=='0' for r in shared0),'P_shared N0 inactive fast path')
 start=next(i for i,r in enumerate(shared0) if r['valid']=='1');require(all(r['history_isolate']=='0' for r in shared0[start:]),'P_shared applied configuration through request/drain')
 es=json.loads((HERE/'elaboration-risk.json').read_text());require(es['production_sequential_state_added']==0,'module state')
 for item in es['generated_excerpts']:
  for a in item['assignments']:require(('history_isolate_i' in a['assignment'])==(item['scope']=='enabled_runtime'),'generated predicate')
 print('PASS immutable history evidence checks='+str(checks)+'; board_cases=0')
if __name__=='__main__':main()
