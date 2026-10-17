#!/usr/bin/env python3
"""Finite actual tap units, candidate on/default vs immutable original bytegold."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
BASE = '8aac3fcd345af7686a55af1867783fd6b91a0069'
GAP = 'dd1a97e890e3c9f587416891b3f006336f939619'
OBS = 'corev_apu/fpga/src/dma_observer.sv'
FRONT = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cva6', type=Path, required=True)
    p.add_argument('--gap-evidence', type=Path, required=True,
                   help='accepted E6-pn/gap-validation directory with frozen dependencies')
    p.add_argument('--output', type=Path, required=True)
    args = p.parse_args()
    cva = args.cva6.resolve()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    saved = out / 'source-as-executed'
    saved.mkdir()
    commands = []
    checks = []
    report = {'schema': 'ispass2027.e6_pn.fact_taps.gold.v1', 'status': 'running',
              'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'fixture_only': True, 'source_board_cases': 0, 'hardware_access': False,
              'scope': 'actual old observer/PATH and frontend only; synthetic event/backend inputs',
              'base_observer': BASE, 'gold_frontend': GAP,
              'commands': commands, 'checks': checks, 'sources': []}

    def run(argv, label):
        result = subprocess.run([str(x) for x in argv], text=True, capture_output=True)
        entry = {'argv': [str(x) for x in argv], 'exit_code': result.returncode,
                 'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(entry)
        (out / (label + '.json')).write_text(json.dumps(entry, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError(f'{label} failed: exit {result.returncode}; original output/source retained')
        return result.stdout

    def check(label, condition):
        if not condition:
            raise AssertionError(label)
        checks.append(label)

    def store(name, raw, origin):
        dest = saved / name
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(raw)
        report['sources'].append({'path': str(dest), 'sha256': sha(raw),
                                  'bytes': len(raw), 'origin': origin})
        return dest

    try:
        run(['verilator', '--version'], 'verilator-version')
        report['cva6_head_at_execution'] = run(['git', '-C', cva, 'rev-parse', 'HEAD'], 'cva6-head').strip()
        report['cva6_status_at_execution'] = run(['git', '-C', cva, 'status', '--short'], 'cva6-status')
        originals = {}
        candidates = {}
        for label, commit, path, module in [('observer', BASE, OBS, 'dma_observer'),
                                            ('frontend', GAP, FRONT, 'idma_reg64_frontend')]:
            raw = subprocess.check_output(['git', '-C', str(cva), 'show', commit + ':' + path])
            originals[label] = store(label + '.gold.original.sv', raw, commit + ':' + path)
            pattern = rb'\bmodule\s+' + module.encode() + rb'\b'
            renamed, count = re.subn(pattern, b'module ' + module.encode() + b'_gold', raw)
            if label == 'frontend':
                renamed = renamed.replace(b'endmodule : idma_reg64_frontend',
                                           b'endmodule : idma_reg64_frontend_gold')
            check(label + ' gold only module-name adaptation', count == 1)
            store(label + '.gold.renamed.sv', renamed, 'only module/end-label rename of preceding original')
            candidates[label] = store(label + '.candidate.sv', (cva / path).read_bytes(), str(cva / path))
        for name in ('frontend_tap_top.sv', 'frontend_tap_test.cpp',
                     'observer_tap_top.sv', 'observer_tap_test.cpp', 'check_taps.py'):
            store(name, (HERE / name).read_bytes(), str(HERE / name))

        deps_manifest_path = args.gap_evidence.resolve() / 'source/dependencies.json'
        deps_manifest = json.loads(deps_manifest_path.read_text())
        check('accepted gap dependency manifest passed', deps_manifest['status'] == 'passed')
        store('gap-dependencies.original.json', deps_manifest_path.read_bytes(), str(deps_manifest_path))
        dep_files = {}
        for item in deps_manifest['files']:
            original = args.gap_evidence.resolve() / item['snapshot']
            raw = original.read_bytes()
            check('frozen dependency ' + original.name, sha(raw) == item['sha256'])
            suffix = item['snapshot'].removeprefix('source/deps/')
            dep_files[suffix] = store('deps/' + suffix, raw, str(original))
            # CVA-owned files are also tied directly to the selected baseline blob.
            fixed = subprocess.run(['git', '-C', str(cva), 'show', BASE + ':' + suffix], capture_output=True)
            if not fixed.returncode:
                check('dependency equals fixed8aac Git blob ' + suffix, fixed.stdout == raw)
        path = 'corev_apu/fpga/src/dma_path_observer.sv'
        raw = subprocess.check_output(['git', '-C', str(cva), 'show', BASE + ':' + path])
        path_rtl = store('dma_path_observer.original.sv', raw, BASE + ':' + path)
        deps = saved / 'deps'
        prim = deps / 'corev_apu/register_interface/vendor/lowrisc_opentitan/src'
        front_files = [deps / 'vendor/pulp-platform/common_cells/include/common_cells/registers.svh',
                       deps / 'vendor/pulp-platform/axi/src/axi_pkg.sv',
                       deps / 'corev_apu/iDMA/src/idma_pkg.sv',
                       deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_pkg.sv',
                       prim / 'prim_subreg_arb.sv', prim / 'prim_subreg.sv', prim / 'prim_subreg_ext.sv',
                       deps / 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend_reg_top.sv',
                       deps / 'corev_apu/iDMA/src/frontends/idma_transfer_id_gen.sv',
                       candidates['frontend'], saved / 'frontend.gold.renamed.sv',
                       saved / 'frontend_tap_top.sv', saved / 'frontend_tap_test.cpp']
        groups = {
            'frontend': front_files,
            'observer': [path_rtl, candidates['observer'], saved / 'observer.gold.renamed.sv',
                         saved / 'observer_tap_top.sv', saved / 'observer_tap_test.cpp']}
        summaries = {}
        for label, files in groups.items():
            top = label + '_tap_top'
            obj = out / ('obj-' + label)
            argv = ['verilator', '--cc', '--exe', '--build', '-j', '2', '--assert',
                    '--public-flat-rw', '-Wno-fatal', '--top-module', top, '--Mdir', obj,
                    '-I' + str(deps / 'vendor/pulp-platform/common_cells/include'),
                    '-I' + str(deps / 'corev_apu/iDMA/src/include'), *files]
            run(argv, label + '-build')
            text = run([obj / ('V' + top)], label + '-execution')
            rows = [json.loads(line) for line in text.splitlines()]
            check(label + ' all actual results fixture tagged',
                  all(x['fixture_only'] is True and x['source_board_cases'] == 0 for x in rows))
            summary = rows[-1]
            check(label + ' actual bytegold finite pass', summary.get('summary') == 'passed')
            summaries[label] = summary
        check('source-as-executed bytes unchanged',
              all(sha(Path(x['path']).read_bytes()) == x['sha256'] for x in report['sources']))
        check('candidate actual component bytes still match test snapshot',
              (cva / FRONT).read_bytes() == candidates['frontend'].read_bytes() and
              (cva / OBS).read_bytes() == candidates['observer'].read_bytes())
        report['summaries'] = summaries
        report['status'] = 'passed'
        report['limits'] = ['finite differential unit evidence, not formal equivalence or timing',
                            'backend, AXI and observer events explicitly synthetic; no whole stack',
                            'old observer has no Linux seq register; event ID progression/read windows compared',
                            'target pulse on final count does not replace original ID/error validity gate',
                            'no corewrap/top/control/helper/S15 change, no synthesis or board']
        print(json.dumps({'status': report['status'], 'source_board_cases': 0,
                          'checks': len(checks), 'summaries': summaries}))
    except Exception as error:
        report['status'] = 'failed'
        report['error'] = str(error)
        raise
    finally:
        report['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        (out / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
