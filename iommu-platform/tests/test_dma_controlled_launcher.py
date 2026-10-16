#!/usr/bin/env python3
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/e3-controlled-20260918/dma-controlled-launcher'
out.mkdir(parents=True, exist_ok=True)

fpga = root / 'cva6/corev_apu/fpga/src'
files = [
    fpga / 'dma_controlled_launcher.sv',
    root / 'tests/dma_controlled_launcher_top.sv',
    root / 'tests/dma_controlled_launcher_test.cpp',
]

cmd = [
    'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
    '--top-module', 'dma_controlled_launcher_top',
    '--Mdir', str(out),
    *map(str, files),
]

log = out / 'build.log'
with log.open('w') as f:
    r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
if r.returncode:
    print(log.read_text())
    raise SystemExit(r.returncode)

subprocess.run([str(out / 'Vdma_controlled_launcher_top')], check=True)
