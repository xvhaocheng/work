#!/usr/bin/env python3
"""Exactly two protocol cases, once each using the same newly built binary."""
import hashlib,json,pathlib,subprocess,time
HERE=pathlib.Path(__file__).resolve().parent
binary=HERE/'obj/Vnonblock_actual_dma_system_top'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
ledger=[]
for name in ('single_FIFO_ahead','parallel_FIFO_ahead'):
    out=HERE/'results'/name
    assert not out.exists(),f'Refusing implicit rerun/overwrite: {out}'
    out.mkdir()
    args=[str(binary),name,str(out)];started=time.time();mono=time.monotonic()
    with (HERE/'raw'/f'{name}.log').open('w') as f:p=subprocess.run(args,stdout=f,stderr=subprocess.STDOUT)
    ledger.append({'case':name,'argv':args,'start_unix_seconds':started,'elapsed_wall_seconds':time.monotonic()-mono,'returncode':p.returncode,'binary_sha256':sha(binary)})
    (HERE/'execution-ledger.json').write_text(json.dumps({'kind':'new protocol integration only','case_count':len(ledger),'old_matrix_or_solo_runs':0,'runs':ledger},indent=2)+'\n')
    print(json.dumps(ledger[-1]),flush=True)
    if p.returncode:raise SystemExit(p.returncode)
