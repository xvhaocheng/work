#!/usr/bin/env python3
"""Compile and execute the research observer contract (E2 + cost)."""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
for small in (False, True):
    out = ROOT / 'out' / 'research-observer-20261002' / ('rtl-small' if small else 'rtl')
    out.mkdir(parents=True, exist_ok=True)
    args = ['verilator', '--cc', '--exe', '--build', '-Wall', '-Wno-fatal',
            '--top-module', 'dma_research_observer', '--Mdir', str(out),
            '-GT_CYCLES=64',
            str(ROOT / 'cva6/corev_apu/fpga/src/dma_research_observer.sv'),
            str(ROOT / 'tests/research_observer_test.cpp')]
    if small:
        args += ['-GCounterWidth=8']
    with (out / 'build.log').open('w') as log:
        result = subprocess.run(args, stdout=log, stderr=subprocess.STDOUT)
        if result.returncode != 0:
            tail = (out / 'build.log').read_text()[-4000:]
            print(f"BUILD FAILED ({'small' if small else 'normal'}) tail:\n{tail}")
            sys.exit(1)
    result = subprocess.run([str(out / 'Vdma_research_observer')] + (['small'] if small else []),
                            capture_output=True, text=True)
    if result.returncode != 0:
        print(f"RUN FAILED ({'small' if small else 'normal'}):\n{result.stdout}\n{result.stderr}")
        sys.exit(1)
    print(result.stdout.strip())
print("PASS research observer scenarios (no board claim)")
