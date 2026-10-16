#!/usr/bin/env python3
"""Ordinary hit/background concurrency: two TC priority controls, four fresh runs."""
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
for tc,mdir in [('aw-first','obj-bypass-aw'),('read-first','obj-bypass-read')]:
    if execute(f'build-bypass-{tc}',json.loads((OUT/f'build-bypass-{tc}-command.json').read_text())):
        raise SystemExit('Build failed.')
    for first in ['v','a']:
        name=f'bypass-{tc}-{first}-32768'
        execute(name,[str(OUT/mdir/'Vnonblock_actual_dma_system_top'),'+mode=s','+natural=1',
            '+natural-bytes=32768',f'+natural-first={first}',f'+natural-trace={OUT/(name+".csv")}',
            '+ptw-latency=31','+warmup=0','+read-admit=none','+reuse=2','+prepare-order=0'])
