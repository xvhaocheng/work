#!/usr/bin/env python3
"""Production observer v3: AXI matching, timing, boundaries and invalidation."""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/e3-followup-20260918/path-rtl'
out.mkdir(parents=True, exist_ok=True)
files = [root / 'cva6/corev_apu/fpga/src' / name for name in
         ('dma_path_observer.sv', 'dma_observer.sv')]
cmd = ['verilator', '--cc', '--exe', '--build', '-Wno-fatal',
       '--top-module', 'dma_observer', '-GPathEnable=1', '--Mdir', str(out),
       *map(str, files), str(root / 'tests/dma_path_test.cpp')]
with (out / 'build.log').open('w') as log:
    subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, check=True)
subprocess.run([str(out / 'Vdma_observer')], check=True)
small = out.parent / 'path-small'
small.mkdir(exist_ok=True)
cmd = ['verilator', '--cc', '--exe', '--build', '-Wno-fatal',
       '--top-module', 'path_stage_test_top', '--Mdir', str(small), str(files[0]),
       str(root / 'tests/path_stage_test_top.sv'),
       str(root / 'tests/dma_path_stage_test.cpp')]
with (small / 'build.log').open('w') as log:
    subprocess.run(cmd, stdout=log, stderr=subprocess.STDOUT, check=True)
subprocess.run([str(small / 'Vpath_stage_test_top')], check=True)
