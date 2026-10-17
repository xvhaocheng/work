#!/usr/bin/env python3
"""Reuse existing translation-logic suites unchanged; redirect only IOTLB and output."""
from pathlib import Path
import importlib.util,subprocess,types,hashlib,json,time,concurrent.futures,contextlib,io
ROOT=Path(__file__).resolve().parents[3];OUT=Path(__file__).resolve().parent.parent

def one(v,suite):
 path=ROOT/'tests'/f'test_{suite}.py';spec=importlib.util.spec_from_file_location(f'{suite}_v{v}',path);mod=importlib.util.module_from_spec(spec);spec.loader.exec_module(mod)
 run=OUT/'regression'/f'v{v}'/suite;run.mkdir(parents=True,exist_ok=True);mod.OUT=run
 variant=OUT/'variants'/f'v{v}'/'overlay/cva6/corev_apu/riscv-iommu/rtl/translation_logic/rv_iommu_iotlb.sv'
 old_require=mod.require
 mod.require=lambda p:variant if str(p).endswith('/translation_logic/rv_iommu_iotlb.sv') else old_require(p)
 record={'variant':v,'suite':suite,'script':str(path),'script_sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'commands':[],'started':time.time()}
 def recorded(cmd,**kw):
  if '--build' in cmd:cmd=cmd[:1]+['-j','2']+cmd[1:]
  entry={'argv':cmd};record['commands'].append(entry)
  entry['input_files']=[{'path':p,'sha256':hashlib.sha256(Path(p).read_bytes()).hexdigest()} for p in cmd if Path(p).is_file() and (p.endswith('.sv')or p.endswith('.cpp'))]
  if '--build' not in cmd:
   with (run/'stdout.log').open('w')as f,(run/'stderr.log').open('w')as err:q=subprocess.run(cmd,stdout=f,stderr=err,timeout=120)
  else:q=subprocess.run(cmd,**kw,timeout=900)
  entry['exit_code']=q.returncode;return q
 mod.subprocess=types.SimpleNamespace(run=recorded,STDOUT=subprocess.STDOUT)
 record['exit_code']=mod.main();record['finished']=time.time()
 if (run/'stdout.log').exists():record['summary']=[s for s in (run/'stdout.log').read_text().splitlines()if 'PASS' in s or 'FAIL' in s][-10:]
 (run/'receipt.json').write_text(json.dumps(record,indent=2)+'\n');return record
with concurrent.futures.ThreadPoolExecutor(max_workers=3)as ex:
 rs=list(ex.map(lambda x:one(*x),[(v,s) for v in range(3)for s in ['nonblock_tl','composition_tl']]))
(OUT/'regression/integration-results.json').write_text(json.dumps(rs,indent=2)+'\n')
for r in rs:print(r['variant'],r['suite'],r['exit_code'],r.get('summary'))
raise SystemExit(0 if all(r['exit_code']==0 for r in rs)else 1)
