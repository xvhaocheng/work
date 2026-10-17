#!/usr/bin/env python3
"""Finite actual P0 frontend comparison; synthetic completion, no AXI/board."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

BASE = '6c5c666d45979d3a03fb98cff89ba7ad095f6125'
FRONTEND = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--deps', type=Path, default=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6'))
    args = parser.parse_args()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    args.cva6, args.deps = args.cva6.resolve(), args.deps.resolve()
    original = subprocess.check_output(['git', 'show', BASE + ':' + FRONTEND], cwd=args.cva6)
    (out / 'gold.as-frozen.sv').write_bytes(original)
    (out / 'gold.sv').write_bytes(original.replace(b'module idma_reg64_frontend #(', b'module idma_reg64_frontend_gold #(')
                                .replace(b'endmodule : idma_reg64_frontend', b'endmodule : idma_reg64_frontend_gold'))
    for source, name in [(args.cva6 / FRONTEND, 'candidate.sv'), (HERE / 'gap_frontend_top.sv', 'gap_frontend_top.sv'),
                         (HERE / 'gap_frontend_test.cpp', 'gap_frontend_test.cpp'), (Path(__file__), 'run_frontend.as-executed.py')]:
        shutil.copyfile(source, out / name)
    deps = args.deps
    primitive = deps / 'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
    files = [deps / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
             deps / 'vendor/pulp-platform/axi/src/axi_pkg.sv', deps / 'corev_apu/iDMA/src/idma_pkg.sv',
             deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
             primitive / 'prim_subreg_arb.sv', primitive / 'prim_subreg.sv', primitive / 'prim_subreg_ext.sv',
             deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
             deps / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv', out / 'candidate.sv', out / 'gold.sv',
             out / 'gap_frontend_top.sv', out / 'gap_frontend_test.cpp']
    headers = [deps / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
               deps / 'corev_apu/iDMA/src/include/idma/typedef.svh']
    cmd = ['verilator', '--cc', '--exe', '--build', '-Wno-fatal', '--top-module', 'gap_frontend_top',
           '--Mdir', str(out / 'obj'), '-I' + str(deps / 'vendor/pulp-platform/common_cells/include'),
           '-I' + str(deps / 'corev_apu/iDMA/src/include'), *map(str, files)]
    report = {'schema': 'ispass2027.e6.g.frontend.validation.v1', 'status': 'running',
              'scope': 'three real frontend instances: exact frozen P0, candidate default-disabled and candidate enabled; synthetic completion only',
              'not_verified': ['actual backend/AXI/data', 'observer/Linux integration', 'FPGA implementation', 'board execution'],
              'base_cva6': BASE, 'candidate_repository': str(args.cva6),
              'candidate_head': subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=args.cva6, text=True).strip(),
              'candidate_branch': subprocess.check_output(['git', 'branch', '--show-current'], cwd=args.cva6, text=True).strip(),
              'dirty_diff': subprocess.check_output(['git', 'diff', '--', FRONTEND], cwd=args.cva6, text=True),
              'start': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'input_sha256': {str(p): hashlib.sha256(p.read_bytes()).hexdigest() for p in files + headers},
              'limits': {'compile_seconds': 180, 'simulation_seconds': 20}, 'commands': []}
    path = out / 'manifest.json'
    def save():
        path.write_text(json.dumps(report, indent=2) + '\n')
    save()
    try:
        for argv, timeout, log in [(cmd, 180, 'compile.log'), ([str(out / 'obj/Vgap_frontend_top')], 20, 'simulation.log')]:
            with (out / log).open('w') as stream:
                result = subprocess.run(argv, stdout=stream, stderr=subprocess.STDOUT, timeout=timeout)
            report['commands'].append({'argv': argv, 'timeout_seconds': timeout, 'exit_code': result.returncode, 'log': log})
            save()
            if result.returncode:
                raise RuntimeError(f'{log}: exit {result.returncode}')
        report['status'] = 'passed'
    except Exception as error:
        report.update(status='failed', error=str(error))
    report['end'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    save()
    print(json.dumps({'status': report['status'], 'manifest': str(path)}))
    if (out / 'simulation.log').exists():
        print((out / 'simulation.log').read_text(), end='')
    return 0 if report['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
