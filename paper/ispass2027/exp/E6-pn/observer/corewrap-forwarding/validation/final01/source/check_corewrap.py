#!/usr/bin/env python3
"""Focused real fullwrap tap/gap forwarding; immutable independent8aac frontend gold."""
import argparse
import datetime
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import resource
import shutil
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
BASE = '8aac3fcd345af7686a55af1867783fd6b91a0069'
BACKEND = '9befa577b153856990cb6c9c18bae1f5332e0685'
WRAP = 'corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv'
FRONT = 'corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv'
CERT = 'corev_apu/iDMA/src/idma_fullbuffer_certificate.sv'
EXTRA = ['vendor/pulp-platform/common_cells/src/popcount.sv',
         'corev_apu/iDMA/src/frontends/register_64bit_2d/idma_reg64_2d_frontend_reg_pkg.sv',
         'corev_apu/iDMA/src/frontends/register_64bit_2d/idma_reg64_2d_frontend_reg_top.sv',
         'corev_apu/iDMA/src/frontends/register_64bit_2d/idma_reg64_2d_frontend.sv',
         'corev_apu/iDMA/src/midends/idma_nd_midend.sv']


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--backend-evidence', type=Path,
                        default=Path('/home/xvhaocheng/work-e6-pn-backend/paper/ispass2027/exp/E6-pn/backend-validation'))
    parser.add_argument('--deps', type=Path,
                        default=Path('/home/xvhaocheng/work/iommu-platform/out/ispass2027-20261017-ds0-01/src/cva6'))
    args = parser.parse_args()
    repo, out, deps = args.cva6.resolve(), args.output.resolve(), args.deps.resolve()
    out.mkdir(parents=True, exist_ok=False)
    frozen = out / 'source'
    frozen.mkdir()
    report = {'schema': 'ispass2027.e6_pn.corewrap_forwarding.host.v1', 'status': 'preparing',
              'fixture_only': True, 'synthetic': True, 'source_board_cases': 0,
              'hardware_access': False, 'full_build': False, 'commands': [], 'checks': [],
              'summaries': {}, 'inputs': {}, 'parameters': {'AxiIdWidth': 2, 'JobFifoDepth': 8,
              'NumAxInFlight': 8, 'BufferDepth': 3, 'NBufferDepth': 512, 'FPGA_EN': 0},
              'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat()}

    def save():
        (out / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')

    def check(name, condition):
        if not condition:
            raise AssertionError(name)
        report['checks'].append(name)

    def execute(argv, label, timeout=900):
        record = {'argv': [str(x) for x in argv], 'log': label + '.log'}
        report['commands'].append(record)
        save()
        with (out / record['log']).open('w') as log:
            result = subprocess.run(record['argv'], stdout=log, stderr=subprocess.STDOUT, timeout=timeout,
                                    preexec_fn=lambda: resource.setrlimit(resource.RLIMIT_CORE, (0, 0)))
        record.update(exit_code=result.returncode, sha256=sha(out / record['log']))
        save()
        if result.returncode:
            raise RuntimeError(label + ' failed; original log/source retained')

    def blob(ref, path):
        return subprocess.check_output(['git', '-C', str(repo), 'show', ref + ':' + path])

    try:
        report['execution_HEAD'] = subprocess.check_output(['git', '-C', str(repo), 'rev-parse', 'HEAD'], text=True).strip()
        report['execution_diff'] = subprocess.check_output(['git', '-C', str(repo), 'diff'], text=True)
        donor = args.backend_evidence.resolve() / 'runs/run09'
        donor_mf = json.loads((donor / 'manifest.json').read_text())
        check('accepted donor run09 passed', donor_mf['status'] == 'passed')
        donor_runner = donor / 'source/run_backend.py'
        check('actual donor runner identity', sha(donor_runner) == donor_mf['inputs']['source/run_backend.py'])
        shutil.copyfile(donor_runner, frozen / 'donor_run_backend.py')
        spec = importlib.util.spec_from_file_location('frozen_donor', frozen / 'donor_run_backend.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        verilator = donor_mf['commands'][0]['argv'][0]
        report['verilator'] = {'path': verilator, 'version': subprocess.check_output([verilator, '--version'], text=True).strip()}
        report['donor'] = {'manifest': str(donor / 'manifest.json'), 'sha256': sha(donor / 'manifest.json'),
                           'runner_sha256': sha(donor_runner)}
        baseline_path = deps.parents[1] / 'build-manifest.json'
        baseline = json.loads(baseline_path.read_text())
        check('actual baseline selected8aac', baseline['selected_cva6'] == BASE)
        report['baseline_manifest'] = {'path': str(baseline_path), 'sha256': sha(baseline_path)}
        for rel in module.SOURCES + module.HEADERS + EXTRA + ['core/include/cv64a6_imafdc_sv39_config_pkg.sv']:
            source, dest = deps / rel, frozen / 'deps' / rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            check('baseline dependency ' + rel, sha(source) == baseline['sources']['cva6/' + rel])
            shutil.copyfile(source, dest)
        for rel, name in [(WRAP, 'wrap.candidate.sv'), (FRONT, 'frontend.candidate.sv'), (CERT, 'certificate.candidate.sv')]:
            shutil.copyfile(repo / rel, frozen / name)
        original = blob(BASE, WRAP)
        (frozen / 'wrap.gold.original.sv').write_bytes(original)
        gold = original.replace(b'dma_core_wrap', b'dma_core_wrap_gold')
        gold, count = re.subn(rb'\bidma_reg64_frontend\b', b'idma_reg64_frontend_gold', gold)
        check('gold wrapper binds original namespace frontend', count == 1)
        (frozen / 'wrap.gold.renamed.sv').write_bytes(gold)
        original_front = blob(BASE, FRONT)
        (frozen / 'frontend.gold.original.sv').write_bytes(original_front)
        check('baseline frontend equals original8aac blob', original_front == (frozen / 'deps' / FRONT).read_bytes())
        front_gold, count = re.subn(rb'\bidma_reg64_frontend\b', b'idma_reg64_frontend_gold', original_front)
        check('gold frontend only module/end namespace changed', count == 2)
        (frozen / 'frontend.gold.renamed.sv').write_bytes(front_gold)
        check('certificate unchanged from accepted backend', (repo / CERT).read_bytes() == blob(BACKEND, CERT))
        current, backend = (repo / WRAP).read_bytes(), blob(BACKEND, WRAP)
        marker = b'  logic pn_select_n;'
        check('all backend/certificate/intf tail bytes unchanged', current.split(marker)[1] == backend.split(marker)[1])
        check('entire outer interface bytes unchanged', current.split(b'module dma_core_wrap_intf')[1] == backend.split(b'module dma_core_wrap_intf')[1])
        for name in ('corewrap_top.sv', 'check_corewrap.cpp', 'check_corewrap.py'):
            shutil.copyfile(HERE / name, frozen / name)
        (frozen / 'ariane_config_stub.sv').write_bytes((donor / 'source/ariane_config_stub.sv').read_bytes())
        report['inputs'] = {str(p.relative_to(out)): sha(p) for p in frozen.rglob('*') if p.is_file()}
        common = [frozen / 'ariane_config_stub.sv', *[frozen / 'deps' / rel for rel in module.SOURCES if rel != FRONT],
                  *[frozen / 'deps' / rel for rel in EXTRA], frozen / 'frontend.candidate.sv',
                  frozen / 'frontend.gold.renamed.sv', frozen / 'certificate.candidate.sv',
                  frozen / 'wrap.candidate.sv', frozen / 'wrap.gold.renamed.sv', frozen / 'corewrap_top.sv']
        includes = ['-I' + str(frozen / 'deps' / x) for x in (module.CC + '/include', module.AXI + '/include',
                    module.IOMMU + '/include', module.IDMA + '/src/include', module.REG + '/include')]
        profiles = [('default', 0, 0, 0, 0, 2), ('tap-gap-S', 0, 1, 1, 0, 6),
                    ('tap-gap-N', 1, 1, 1, 0, 7), ('twoD-idle', 1, 1, 1, 1, 0)]
        for name, enable, taps, gaps, two_d, count in profiles:
            obj = out / ('obj-' + name)
            execute([verilator, '--cc', '--exe', '--build', '--assert', '--stats', '-j', '2', '-Wno-fatal',
                     '+define+ASSERTS_OFF', '--top-module', 'pn_backend_top', '--Mdir', obj,
                     f'-GEnable={enable}', f'-GTaps={taps}', f'-GGaps={gaps}', f'-GTwoD={two_d}',
                     '-CFLAGS', f'-std=c++17 -O1 -DPN_ENABLE={enable} -DTAP_ENABLE={taps} -DGAP_ENABLE={gaps} -DTWO_D={two_d}',
                     *includes, *common, frozen / 'check_corewrap.cpp'], name + '-build')
            execute([obj / 'Vpn_backend_top'], name + '-run', timeout=120)
            rows = [json.loads(x) for x in (out / (name + '-run.log')).read_text().splitlines() if x.startswith('{')]
            check(name + ' all raw JSON explicitly fixtures', all(x['fixture_only'] is True and x['source_board_cases'] == 0 for x in rows))
            check(name + ' actual finite passed', rows[-1]['event'] == ('twoD_idle_passed' if two_d else 'all_passed'))
            cases = [x for x in rows if x['event'] == 'case_summary']
            check(name + ' finite case count', len(cases) == count)
            report['summaries'][name] = cases if not two_d else [rows[-1]]
            report.setdefault('binary_sha256', {})[name] = sha(obj / 'Vpn_backend_top')
        check('frozen input bytes unchanged', all(sha(out / p) == digest for p, digest in report['inputs'].items()))
        check('candidate still matches compiled source', all((repo / rel).read_bytes() == (frozen / name).read_bytes()
              for rel, name in [(WRAP, 'wrap.candidate.sv'), (FRONT, 'frontend.candidate.sv'), (CERT, 'certificate.candidate.sv')]))
        report['status'] = 'passed'
    except Exception as error:
        report.update(status='failed', error=repr(error))
    report['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    report['check_count'] = len(report['checks'])
    save()
    print(json.dumps({'status': report['status'], 'checks': report['check_count'], 'error': report.get('error'), 'source_board_cases': 0}))
    return 0 if report['status'] == 'passed' else 1


if __name__ == '__main__':
    raise SystemExit(main())
