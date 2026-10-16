#!/usr/bin/env python3
"""Fault checks on the real host main, with explicitly CPU-only fake devices."""
from pathlib import Path
import os, subprocess, json
root = Path(__file__).resolve().parent.parent
os.chdir(root)
subprocess.run(['gcc', '-O2', '-Wall', '-Wextra', '-Wno-unused-parameter',
 '-Isource/hero-tools/sw/libhero/include', '-Isource/hero-tools/apps/omp/common',
 'source/hero-tools/apps/omp/basic/axpy/axpy_main.c', 'tests/axpy_host_backend.c',
 '-lm', '-o', 'evidence/axpy-host-cpu-harness'], check=True)
cases = [
 ('normal', ['32768','1','1','0'], 0, 'hot consumer verified', 'maps=2 targets=2 syncs=8 unmaps=2'),
 ('normal', ['512','0.75','1','0'], 0, 'hot consumer verified', 'maps=2 targets=2 syncs=8 unmaps=2'),
 ('corrupt', ['512','1','1','0'], 1, 'cold mismatch at 511', 'maps=2 targets=1 syncs=4 unmaps=2'),
 ('map-fail', ['512','1','1','0'], 1, 'map y', 'maps=2 targets=0 syncs=0 unmaps=1'),
 ('unsupported', ['512','1','1','0'], 1, 'map x', 'maps=1 targets=0 syncs=0 unmaps=0'),
 ('cache-fail', ['512','1','1','0'], 1, 'cache handoff to device', 'maps=2 targets=0 syncs=1 unmaps=2'),
 ('unmap-fail', ['512','1','1','0'], 1, 'unmap y', 'maps=2 targets=2 syncs=8 unmaps=2'),
 ('normal', ['16','1','1','0'], 1, 'usage:', ''),
 ('normal', ['512','1','0','0'], 1, 'usage:', ''),
]
rows=[]
for i,(mode,args,code,message,counts) in enumerate(cases):
 env = os.environ | {'A2_TEST_MODE':mode}
 r=subprocess.run(['evidence/axpy-host-cpu-harness',*args],env=env,text=True,capture_output=True)
 output=r.stdout+r.stderr
 ok = r.returncode==code and message in output and counts in output
 rows.append({'mode':mode,'arguments':args,'returncode':r.returncode,'passed':ok,'output':output})
 print(f'{i+1}: {mode} {args} -> {r.returncode}; passed={ok}')
Path('evidence/host-lifecycle-results.json').write_text(json.dumps({'scope':'CPU semantic fault harness only, no actual kernel/target/DMA/hardware execution','cases':rows},indent=2)+'\n')
if not all(x['passed'] for x in rows): raise SystemExit(1)
