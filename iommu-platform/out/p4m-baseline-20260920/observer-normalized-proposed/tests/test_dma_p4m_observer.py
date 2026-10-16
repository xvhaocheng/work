#!/usr/bin/env python3
"""Compile and execute the P4M observer contract (P4MEnable=1, PathEnable=0)."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
for small in (False, True):
    out = ROOT/'out/p4m-20260920'/('rtl-small' if small else 'rtl')
    out.mkdir(parents=True, exist_ok=True)
    args = ['verilator', '--cc', '--exe', '--build', '-Wall', '-Wno-fatal',
            '--top-module', 'dma_observer', '--Mdir', str(out),
            '-GP4MEnable=1',
            str(ROOT/'cva6/corev_apu/fpga/src/dma_observer.sv'),
            str(ROOT/'tests/dma_p4m_observer_test.cpp')]
    if small:
        args += ['-GCounterWidth=8', '-GOutstandingWidth=3']
    with (out/'build.log').open('w') as log:
        subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, check=True)
    subprocess.run([str(out/'Vdma_observer')] + (['small'] if small else []), check=True)
