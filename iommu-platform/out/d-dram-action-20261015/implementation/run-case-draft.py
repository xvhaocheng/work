#!/usr/bin/env python3
"""Root-reviewed preparation-only runner; writing this file does not execute cases."""
import argparse,hashlib,json,os,subprocess,time
from pathlib import Path
p=Path(__file__).resolve().parent
ap=argparse.ArgumentParser();ap.add_argument('case',choices=['p_row0','p_row1','pn_row0','pn_row1']);args=ap.parse_args()
def sha(f):return hashlib.sha256(Path(f).read_bytes()).hexdigest()
manifest=json.loads((p/'prebuild-bindings.json').read_text())
for f,h in manifest.items():
 if sha(f)!=h:raise RuntimeError('input hash changed '+f)
build=json.loads((p/'build-receipt.json').read_text());assert build['exit_code']==0 and build['inputs_unchanged']
binary=p/'preparation-projection';assert sha(binary)==build['binary_sha256']
contract=json.loads((p.parent/'root/preparation-projection-contract.json').read_text());case=next(c for c in contract['cases'] if c['id']==args.case)
out=p/'runs'/args.case;out.mkdir(parents=True,exist_ok=False)
config=p.parents[1]/'d-dram-boundary-20261015/implementation/boundary-config.yaml'
env=os.environ.copy();env.update(PROJECTION_CONFIG=str(config),PROJECTION_OUTPUT=str(out),PROJECTION_HISTORY=args.case.split('_')[1])
cmd=[str(binary),'+mode='+case['mode'],'+full-batch=1','+transfers=64','+a-target=64','+a-pages=8','+warmup=256','+v-gap=0','+cpu-enable=0','+cpu-writes=0','+ds-priority=0','+ptw-latency=31','+read-admit=none','+owner-only=0','+start-delay=0','+v-start-delay=0','+a-start-delay=0','+full-watchdog=200000']
receipt={'case':args.case,'argv':cmd,'provider_environment':{k:env[k] for k in ('PROJECTION_CONFIG','PROJECTION_OUTPUT','PROJECTION_HISTORY')},'timeout_seconds':180,'max_rtl_ticks':7900480,'full_task_executed':False,'binary_sha256':sha(binary),'contract_sha256':sha(p.parent/'root/preparation-projection-contract.json'),'start_unix':time.time(),'model_actual':'unknown'}
(out/'run-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');start=time.monotonic()
try:
 with (out/'stdout.log').open('wb') as stdout,(out/'stderr.log').open('wb') as stderr:result=subprocess.run(cmd,env=env,stdout=stdout,stderr=stderr,timeout=180)
 receipt['exit_code']=result.returncode
except subprocess.TimeoutExpired:receipt['exit_code']=124;receipt['timeout']=True
receipt['duration_seconds']=time.monotonic()-start;receipt['end_unix']=time.time();(out/'run-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2));raise SystemExit(receipt['exit_code'])
