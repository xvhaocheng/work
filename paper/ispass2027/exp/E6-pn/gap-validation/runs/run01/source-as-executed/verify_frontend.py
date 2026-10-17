#!/usr/bin/env python3
"""Bounded actual PN-gap frontend/default/gold tests; synthetic completion only."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
BASE = '8aac3fcd345af7686a55af1867783fd6b91a0069'
FRONTEND = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'


def sha(raw): return hashlib.sha256(raw).hexdigest()
def now(): return datetime.datetime.now(datetime.timezone.utc).isoformat()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6', type=Path, required=True)
    p.add_argument('--deps', type=Path, default=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6'))
    p.add_argument('--output', type=Path, required=True); args = p.parse_args()
    output = args.output.resolve(); output.mkdir(parents=True, exist_ok=False)
    snapshot = output/'source-as-executed'; snapshot.mkdir()
    original = subprocess.check_output(['git', '-C', str(args.cva6), 'show', BASE+':'+FRONTEND])
    (snapshot/'frontend.gold.original.sv').write_bytes(original)
    (snapshot/'frontend.gold.renamed.sv').write_bytes(original.replace(b'module idma_reg64_frontend #(', b'module idma_reg64_frontend_gold #(').replace(b'endmodule : idma_reg64_frontend', b'endmodule : idma_reg64_frontend_gold'))
    for name, path in [('frontend.candidate.sv', args.cva6/FRONTEND), ('pn_gap_top.sv', HERE/'source/pn_gap_top.sv'), ('pn_gap_test.cpp', HERE/'source/pn_gap_test.cpp'), ('verify_frontend.py', Path(__file__))]:
        (snapshot/name).write_bytes(path.read_bytes())
    deps = args.deps.resolve(); prim = deps/'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
    files = [deps/'vendor/pulp-platform/axi/src/axi_pkg.sv', deps/'corev_apu/iDMA/src/idma_pkg.sv',
        deps/'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
        prim/'prim_subreg_arb.sv', prim/'prim_subreg.sv', prim/'prim_subreg_ext.sv',
        deps/'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
        deps/'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv', snapshot/'frontend.candidate.sv',
        snapshot/'frontend.gold.renamed.sv', snapshot/'pn_gap_top.sv', snapshot/'pn_gap_test.cpp']
    includes = [deps/'vendor/pulp-platform/common_cells/include/common_cells/registers.svh', deps/'corev_apu/iDMA/src/include/idma/typedef.svh']
    command = ['verilator', '--cc', '--exe', '--build', '--assert', '-Wno-fatal', '--top-module', 'pn_gap_top',
        '--Mdir', str(output/'obj'), '-I'+str(deps/'vendor/pulp-platform/common_cells/include'),
        '-I'+str(deps/'corev_apu/iDMA/src/include'), *map(str, files)]
    report = dict(schema='ispass2027.pn_gap.frontend_test.v1', status='running', fixture_only=True,
        evidence_level='finite_actual_frontend_synthetic_completion', source_board_cases=0, started_utc=now(),
        scope='actual frontend only; not backend, AXI, IOMMU, whole-path equivalence, timing or board proof',
        base_cva6=BASE, cva6_worktree=str(args.cva6.resolve()), source_sha256={str(path):sha(path.read_bytes()) for path in files+includes+[snapshot/'verify_frontend.py']},
        commands=[])
    def save(): (output/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    save()
    try:
        with (output/'build.log').open('w') as stream:
            r = subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT, timeout=240)
        report['commands'].append(dict(argv=command, exit_code=r.returncode))
        if r.returncode == 0:
            command = [str(output/'obj/Vpn_gap_top')]
            r = subprocess.run(command, text=True, capture_output=True, cwd=output, timeout=60)
            (output/'test.log').write_text(r.stdout+r.stderr)
            report['commands'].append(dict(argv=command, exit_code=r.returncode, stdout=r.stdout, stderr=r.stderr))
        report['status'] = 'passed' if r.returncode == 0 else 'failed'
    except Exception as error:
        report.update(status='failed', error=repr(error)); raise
    finally:
        report['finished_utc'] = now(); save()
    print(json.dumps(dict(status=report['status'], report=str(output/'report.json'))))
    return r.returncode


if __name__ == '__main__': raise SystemExit(main())
