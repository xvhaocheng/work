#!/usr/bin/env python3
"""Run only the two pre-frozen complete episodes, retaining every result."""
from pathlib import Path
import hashlib,json,resource,subprocess,time
P=Path(__file__).resolve().parent
assert hashlib.sha256((P/'contract.json').read_bytes()).hexdigest()==(P/'contract.sha256').read_text().strip()
ledger=P/'run-ledger.json'
assert not ledger.exists(),'no automatic replay of previously run episodes'
resource.setrlimit(resource.RLIMIT_CORE,(0,0))
exe=P/'obj/Vnonblock_actual_dma_system_top'
base=[str(exe),'+mode=n','+system=1','+a-pages=2','+v-gap=0','+transfers=2','+warmup=0','+ptw-latency=31','+a-target=2','+full-batch=1','+cpu-enable=0','+start-delay=0','+v-start-delay=0','+a-start-delay=0','+launch-order=av','+full-watchdog=200000']
rs=[]
for policy in [0,1]:
 label=['rr','ds-first'][policy];log=P/(label+'.log');trace=P/(label+'.csv')
 cmd=base+['+ar-ds-first='+str(policy),'+event-trace='+str(trace)]
 start=time.monotonic()
 with log.open('w') as f:r=subprocess.run(cmd,cwd=P,stdout=f,stderr=subprocess.STDOUT,timeout=60)
 row={'policy':label,'command':cmd,'exit_code':r.returncode,'elapsed_seconds':time.monotonic()-start,'log':str(log),'trace':str(trace)}
 rs.append(row);ledger.write_text(json.dumps(rs,indent=2)+'\n');print(json.dumps(row))
 if r.returncode:break
raise SystemExit(0 if all(r['exit_code']==0 for r in rs) and len(rs)==2 else 1)
