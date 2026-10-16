#!/usr/bin/env python3
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
cva6 = root / 'cva6'
out = root / 'out/e3-controlled-20260918/frontend-controlled-pair'
out.mkdir(parents=True, exist_ok=True)

prim = cva6 / 'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
files = [
    cva6 / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
    cva6 / 'vendor/pulp-platform/axi/src/axi_pkg.sv',
    cva6 / 'corev_apu/iDMA/src/idma_pkg.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
    prim / 'prim_subreg_arb.sv',
    prim / 'prim_subreg.sv',
    prim / 'prim_subreg_ext.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv',
    cva6 / 'corev_apu/fpga/src/dma_controlled_launcher.sv',
    root / 'tests/idma_controlled_pair_top.sv',
    root / 'tests/idma_controlled_pair_test.cpp',
]

cmd = [
    'verilator', '--cc', '--exe', '--build', '-Wno-fatal',
    '-I' + str(cva6 / 'vendor/pulp-platform/common_cells/include'),
    '-I' + str(cva6 / 'corev_apu/iDMA/src/include'),
    '--top-module', 'idma_controlled_pair_top',
    '--Mdir', str(out),
    *map(str, files),
]

log = out / 'build.log'
with log.open('w') as f:
    r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, text=True)
if r.returncode:
    print(log.read_text())
    raise SystemExit(r.returncode)

subprocess.run([str(out / 'Vidma_controlled_pair_top')], check=True)
