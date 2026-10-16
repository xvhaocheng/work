#!/usr/bin/env python3
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
out = root / 'out/e3-controlled-20260918/dma-observer-finite'
out.mkdir(parents=True, exist_ok=True)

fpga = root / 'cva6/corev_apu/fpga/src'
base = [
    fpga / 'dma_path_observer.sv',
    fpga / 'dma_observer.sv',
    root / 'tests/dma_observer_finite_test.cpp',
]

for controlled in (1, 0):
    name = 'controlled' if controlled else 'legacy'
    dest = out / name
    dest.mkdir(exist_ok=True)
    cmd = [
        'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
        '--top-module', 'dma_observer',
        '--Mdir', str(dest),
        '-GPathEnable=1', '-GE3Enable=1', f'-GControlledEnable={controlled}',
        '-CFLAGS', '-D' + ('CONTROLLED' if controlled else 'LEGACY'),
        *map(str, base),
    ]
    log = dest / 'build.log'
    with log.open('w') as f:
        r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
    if r.returncode:
        print(log.read_text())
        raise SystemExit(r.returncode)
    subprocess.run([str(dest / 'Vdma_observer')], check=True)
