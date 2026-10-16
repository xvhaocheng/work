#!/usr/bin/env python3
"""Codex executes one build and eight fresh DUT runs; preserves actual failures."""
import hashlib
import json
import subprocess
import time
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
OUT=Path(__file__).resolve().parent

def save(name, value):
    (OUT/name).write_text(json.dumps(value,indent=2)+'\n')

def audit():
    old=json.loads((ROOT/'out/original-window-20261008/production-after.json').read_text())
    new={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in old}
    assert new==old, 'Production source changed since section 37'
    return new

def execute(name, argv):
    save(name+'-command.json',argv)
    start=time.monotonic()
    with (OUT/(name+'.log')).open('w') as f:
        result=subprocess.run(argv,cwd=ROOT,stdout=f,stderr=subprocess.STDOUT)
    value={'exit_code':result.returncode,'seconds':round(time.monotonic()-start,3),'reused':False}
    save(name+'-result.json',value)
    print(name, json.dumps(value),flush=True)
    return result.returncode

save('production-before.json',audit())
argv=json.loads((OUT/'build-command.json').read_text())
if execute('build',argv):
    raise SystemExit('Build failed; no DUT runs attempted.')
exe=str(OUT/'obj-multi/Vnonblock_actual_dma_system_top')
for mode,reuse,order in [('lazy',1,0),('prepare0',2,0),('prepare1',2,1),('prepare2',2,2)]:
    for first in ['v','a']:
        name=f'{mode}-{first}-32768'
        execute(name,[exe,'+mode=s','+natural=1','+natural-bytes=32768',
            f'+natural-first={first}',f'+natural-trace={OUT/(name+".csv")}',
            '+ptw-latency=31','+warmup=0','+read-admit=none',f'+reuse={reuse}',f'+prepare-order={order}'])
save('production-after.json',audit())
save('source-hashes.json',{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in OUT.iterdir()
    if p.suffix in ['.sv','.cpp','.py']})
