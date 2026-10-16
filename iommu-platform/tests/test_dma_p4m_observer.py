#!/usr/bin/env python3
"""Compile and execute the P4M observer contract (P4MEnable=1, PathEnable=0/1)."""
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
FPGA = ROOT/'cva6/corev_apu/fpga/src'
for variant in ('rtl', 'rtl-small', 'research', 'research-path'):
    small = variant == 'rtl-small'
    research = variant in ('research', 'research-path')
    with_path = variant == 'research-path'
    out = ROOT/'out/p4m-20260920'/variant
    out.mkdir(parents=True, exist_ok=True)
    args = ['verilator', '--cc', '--exe', '--build', '-Wall', '-Wno-fatal',
            '--top-module', 'dma_observer', '--Mdir', str(out),
            '-GP4MEnable=1',
            str(FPGA/'dma_observer.sv')]
    if with_path:
        args += ['-GPathEnable=1', str(FPGA/'dma_path_observer.sv')]
    args.append(str(ROOT/'tests/dma_p4m_observer_test.cpp'))
    if small:
        args += ['-GCounterWidth=8', '-GOutstandingWidth=3']
    if research:
        args += ['-GResearchEnable=1', '-GE3Enable=0', '-GControlledEnable=0']
    with (out/'build.log').open('w') as log:
        subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, check=True)
    run_arg = 'small' if small else ('' if variant == 'rtl' else variant)
    subprocess.run([str(out/'Vdma_observer')] + ([run_arg] if run_arg else []),
                   check=True)
