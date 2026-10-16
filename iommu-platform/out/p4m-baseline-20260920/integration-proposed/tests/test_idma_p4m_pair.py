#!/usr/bin/env python3
"""Build and run the bounded P4M frontend/observer integration test.

This is an integration-wiring and edge-accounting check against the real
frontend and the real observer.  The synthetic backends are not an AXI timing
model; no FPGA, datapath throughput or continuous-performance claim is made.
Verilator build output is kept on disk only when the build fails.
"""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
cva6 = root / 'cva6'
out = root / 'out/p4m-baseline-20260920/pair'
out.mkdir(parents=True, exist_ok=True)

base = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
prim = cva6 / 'core_apu/register_interface/vendor/lowrisc_opentitan/src'
files = [
    cva6 / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
    cva6 / 'vendor/pulp-platform/axi/src/axi_pkg.sv',
    cva6 / 'corev_apu/iDMA/src/idma_pkg.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
    prim / 'prim_subreg_arb.sv', prim / 'prim_subreg.sv', prim / 'prim_subreg_ext.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
    cva6 / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv',
    cva6 / base,
    cva6 / 'corev_apu/fpga/src/dma_observer.sv',
    root / 'tests/idma_p4m_pair_top.sv',
    root / 'tests/idma_p4m_pair_test.cpp',
]

cmd = ['verilator', '--cc', '--exe', '--build', '-Wno-fatal',
       '--top-module', 'idma_p4m_pair_top',
       '--Mdir', str(out),
       '-I' + str(cva6 / 'vendor/pulp-platform/common_cells/include'),
       '-I' + str(cva6 / 'corev_apu/iDMA/src/include')] + list(map(str, files))

build = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
if build.returncode:
    (out / 'build.log').write_text(build.stdout)
    raise SystemExit('verilator build failed; inspect ' + str(out / 'build.log'))

subprocess.run([str(out / 'Vidma_p4m_pair_top')], check=True)
