"""Execute the one new model and each window/order exactly once."""
import json
import subprocess
import time
from pathlib import Path

OUT = Path(__file__).resolve().parent
ROOT = OUT.parents[1]

def execute(name, argv):
    (OUT / (name + '-command.json')).write_text(json.dumps(argv, indent=2) + '\n')
    start = time.monotonic()
    with (OUT / (name + '.log')).open('w') as log:
        p = subprocess.run(argv, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
    result = dict(exit_code=p.returncode, seconds=time.monotonic()-start, reused=False)
    (OUT / (name + '-result.json')).write_text(json.dumps(result, indent=2) + '\n')
    print(name, result, flush=True)
    if p.returncode:
        raise SystemExit(p.returncode)

execute('build', json.loads((OUT / 'build-command.json').read_text()))
binary = str(OUT / 'obj-window/Vnonblock_actual_dma_system_top')
for window in [8, 1, 2, 4]:
    for first in ['v', 'a']:
        name = f'window{window}-{first}-32768'
        execute(name, [binary, '+mode=s', '+natural=1', '+natural-bytes=32768',
            '+natural-first='+first, '+natural-trace='+str(OUT / (name+'.csv')),
            '+ptw-latency=31', '+warmup=0', '+read-admit=none',
            '+parent-window='+str(window)])
