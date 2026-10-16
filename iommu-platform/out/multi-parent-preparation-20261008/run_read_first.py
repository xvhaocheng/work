#!/usr/bin/env python3
"""Fresh bounded ordinary priority counterfactual; preserve original eight runs."""
import hashlib,json,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
def execute(name,argv):
    (OUT/(name+'-command.json')).write_text(json.dumps(argv,indent=2)+'\n')
    start=time.monotonic()
    with (OUT/(name+'.log')).open('w') as f:
        r=subprocess.run(argv,cwd=ROOT,stdout=f,stderr=subprocess.STDOUT)
    record={'exit_code':r.returncode,'seconds':round(time.monotonic()-start,3),'reused':False}
    (OUT/(name+'-result.json')).write_text(json.dumps(record,indent=2)+'\n')
    print(name,json.dumps(record),flush=True)
    return r.returncode
if execute('build-read-first',json.loads((OUT/'build-read-first-command.json').read_text())):
    raise SystemExit('Build failed.')
for mode,reuse in [('read-first-lazy',1),('read-first-prepare0',2)]:
    for first in ['v','a']:
        name=f'{mode}-{first}-32768'
        execute(name,[str(OUT/'obj-read-first/Vnonblock_actual_dma_system_top'),'+mode=s','+natural=1',
            '+natural-bytes=32768',f'+natural-first={first}',f'+natural-trace={OUT/(name+".csv")}',
            '+ptw-latency=31','+warmup=0','+read-admit=none',f'+reuse={reuse}','+prepare-order=0'])
old=json.loads((OUT/'production-before.json').read_text())
new={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in old}
assert old==new
(OUT/'production-after-read-first.json').write_text(json.dumps(new,indent=2)+'\n')
