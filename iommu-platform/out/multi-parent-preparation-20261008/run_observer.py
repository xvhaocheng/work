#!/usr/bin/env python3
"""Two diagnostics of a NEW unresolved causal question; no acceptance re-run."""
import json,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent
def execute(name,argv):
    (OUT/(name+'-command.json')).write_text(json.dumps(argv,indent=2)+'\n')
    start=time.monotonic()
    with (OUT/(name+'.log')).open('w') as f:
        r=subprocess.run(argv,cwd=ROOT,stdout=f,stderr=subprocess.STDOUT)
    value={'exit_code':r.returncode,'seconds':round(time.monotonic()-start,3),'reused':False}
    (OUT/(name+'-result.json')).write_text(json.dumps(value,indent=2)+'\n')
    print(name,json.dumps(value),flush=True)
    return r.returncode
if execute('build-observer',json.loads((OUT/'build-observer-command.json').read_text())):
    raise SystemExit('Build failed.')
for mode,reuse in [('observer-lazy',1),('observer-prepare0',2)]:
    name=f'{mode}-v-32768'
    execute(name,[str(OUT/'obj-observer/Vnonblock_actual_dma_system_top'),'+mode=s','+natural=1',
        '+natural-bytes=32768','+natural-first=v',f'+natural-trace={OUT/(name+".csv")}',
        '+ptw-latency=31','+warmup=0','+read-admit=none',f'+reuse={reuse}','+prepare-order=0'])
