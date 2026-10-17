#!/usr/bin/env python3
"""Finite Verilator checks of the production frontend; synthetic backend completion.

Only the frontend is under test. This does not model AXI bursting, data memory,
IOMMU behavior, CPU, or timing closure and never opens a board device.
"""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
BASE = '8aac3fcd345af7686a55af1867783fd6b91a0069'
FRONTEND = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6', required=True, type=Path)
    p.add_argument('--deps', type=Path, default=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6'))
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    a.output = a.output.resolve()
    golden = subprocess.check_output(['git', '-C', str(a.cva6), 'show', BASE + ':' + FRONTEND], text=True)
    (a.output / 'gold.sv').write_text(golden.replace('module idma_reg64_frontend #(', 'module idma_reg64_frontend_gold #(')
        .replace('endmodule : idma_reg64_frontend', 'endmodule : idma_reg64_frontend_gold'))
    deps = a.deps.resolve()
    prim = deps / 'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
    files = [deps / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
             deps / 'vendor/pulp-platform/axi/src/axi_pkg.sv', deps / 'corev_apu/iDMA/src/idma_pkg.sv',
             deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
             prim / 'prim_subreg_arb.sv', prim / 'prim_subreg.sv', prim / 'prim_subreg_ext.sv',
             deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
             deps / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv',
             a.cva6.resolve() / FRONTEND, a.output / 'gold.sv', HERE / 'source/p0_frontend_top.sv',
             HERE / 'source/p0_frontend_test.cpp']
    cmd = ['verilator', '--cc', '--exe', '--build', '-Wno-fatal', '--top-module', 'p0_frontend_top',
           '--Mdir', str(a.output / 'obj'), '-I' + str(deps / 'vendor/pulp-platform/common_cells/include'),
           '-I' + str(deps / 'corev_apu/iDMA/src/include'), *map(str, files)]
    report = {'status': 'running', 'scope': 'finite actual frontend; synthetic completion only',
              'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'base_cva6': BASE, 'cva6_worktree': str(a.cva6.resolve()),
              'inputs': {str(x): hashlib.sha256(x.read_bytes()).hexdigest() for x in files},
              'commands': []}
    report_path = a.output / 'report.json'

    def save():
        report_path.write_text(json.dumps(report, indent=2) + '\n')

    save()
    with (a.output / 'build.log').open('w') as f:
        r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, timeout=180)
    report['commands'].append({'argv': cmd, 'exit_code': r.returncode})
    if r.returncode == 0:
        cmd = [str(a.output / 'obj/Vp0_frontend_top')]
        r = subprocess.run(cmd, text=True, capture_output=True, timeout=60)
        report['commands'].append({'argv': cmd, 'exit_code': r.returncode,
                                   'stdout': r.stdout, 'stderr': r.stderr})
        (a.output / 'test.log').write_text(r.stdout + r.stderr)
    report.update(status='passed' if r.returncode == 0 else 'failed',
                  finished_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
    save()
    print(json.dumps({'status': report['status'], 'report': str(report_path)}))
    if (a.output / 'test.log').exists():
        print((a.output / 'test.log').read_text(), end='')
    return r.returncode


if __name__ == '__main__':
    raise SystemExit(main())
