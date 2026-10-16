#!/usr/bin/env python3
"""Compile and execute the production passive observer, including overflow variants."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
for small in (False, True):
    out = ROOT/'out/e3-followup-20260918'/('rtl-small' if small else 'rtl')
    out.mkdir(parents=True, exist_ok=True)
    args = ['verilator', '--cc', '--exe', '--build', '-Wall', '-Wno-fatal',
            '--top-module', 'dma_observer', '--Mdir', str(out),
            str(ROOT/'cva6/corev_apu/fpga/src/dma_observer.sv'),
            str(ROOT/'tests/dma_observer_test.cpp')]
    if small:
        args += ['-GCounterWidth=8', '-GOutstandingWidth=3']
    with (out/'build.log').open('w') as log:
        subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, check=True)
    subprocess.run([str(out/'Vdma_observer')] + (['small'] if small else []), check=True)
