#!/usr/bin/env python3
"""Check immutable N finite evidence against original manifest bytes/commits."""
import argparse,csv,gzip,hashlib,json,re,subprocess
from pathlib import Path

HERE=Path(__file__).resolve().parent
DEFAULT_REPO=Path('/home/xvhaocheng/work-e6-pn-iommu')
checks=0
def require(ok,message):
 global checks
 checks+=1
 if not ok:raise AssertionError(message)
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--iommu-root',type=Path,default=DEFAULT_REPO);args=ap.parse_args();repo=args.iommu_root.resolve()
 summary=json.loads((HERE/'summary.json').read_text())
 for folder in sorted((HERE/'evidence').iterdir()):
  path=folder/'manifest.json'
  if not path.exists():continue
  m=json.loads(path.read_text())
  for item in m['inputs']:
   original=Path(item['path']);suffix=original.relative_to(original.parents[next(i for i,p in enumerate(original.parents) if p.name=='source')])
   require(sha((folder/'source'/suffix).read_bytes())==item['sha256'],str(original))
  for name in ['build.log','run.log','cycle-trace.csv','cases.csv']:
   if name not in m:continue
   compressed=folder/(name+'.gz');raw=gzip.decompress(compressed.read_bytes()) if compressed.exists() else (folder/name).read_bytes()
   require(sha(raw)==m[name]['sha256'],str(folder/name))
 for b in summary['source_commit_binding']:
  raw=subprocess.check_output(['git','show',summary['component_commit']+':'+b['path']],cwd=repo)
  frozen=HERE/'evidence'/b['execution']/'source'/Path(b['path']).name
  require(raw==frozen.read_bytes() and sha(raw)==b['sha256'],b['path'])
 for name in ['ddtc','iotlb','tl_wrap']:
  stem='rv_iommu_'+name;raw=subprocess.check_output(['git','show',summary['iommu_baseline']+':rtl/translation_logic/'+stem+'.sv'],cwd=repo)
  raw,n=re.subn(rb'(?m)^module '+stem.encode()+rb'\b',b'module '+stem.encode()+b'_gold',raw,count=1);require(n==1,'gold declaration')
  if name=='tl_wrap':
   for child in ['ddtc','iotlb']:
    raw,n=re.subn(rb'\brv_iommu_'+child.encode()+rb'\s*#\(',b'rv_iommu_'+child.encode()+b'_gold #(',raw);require(n==1,'gold instance')
  require(raw==(HERE/'evidence/n-tl08/source'/(stem+'_gold.sv')).read_bytes(),'raw gold '+name)
 rows=list(csv.DictReader(gzip.decompress((HERE/'evidence/n-tl08/cycle-trace.csv.gz').read_bytes()).decode().splitlines()))
 require(len(rows)==summary['actual_runs']['tl']['cycles'],'cycle count')
 require(all(None not in r and None not in r.values() for r in rows),'CSV shape')
 require(sum(int(r['n_accept']) for r in rows)==4,'actual accepts')
 for r in rows:
  require(r['touch']==r['n_accept'],'only accept touches')
  if r['probe']=='1' and r['n_accept']=='0' and r['fill']=='0':require(r['no_probe_touch']=='1','probe modified PLRU')
 for dtf in [0,1]:
  xs=[r for r in rows if r['phase']=='N-MSI-DTF'+str(dtf) and r['rst']=='1']
  require(all(r[k]=='0' for r in xs for k in ['fill','ptw_ar_fire','msi_init_ar','msi_fill','mrif','n_accept']),'MSI side effect')
  results=[r for r in xs if r['resp_valid']=='1' and r['resp_ready']=='1']
  require(len(results)==1 and results[0]['error']=='1' and results[0]['code']=='260','MSI response')
  fq=[r for r in xs if r['fq_valid']=='1'];require(bool(fq)==(dtf==0),'DTF FQ presence')
  require(all(r['fq_cause']=='260' for r in fq),'FQ cause')
 for i in range(13):
  xs=[r for r in rows if r['phase']=='N-real-exclusion' and int(r['scenario'])==i]
  require(xs and all(r['n_accept']=='0' for r in xs),'exclusion '+str(i))
 print('PASS immutable N evidence checks='+str(checks)+'; board_cases=0')
if __name__=='__main__':main()
