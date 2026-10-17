#!/usr/bin/env python3
"""Actual old observer + PN capture/control with fixed old bytegold; host only."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
PARENT = 'ad56c8a835724370d151ac5789b6c7cf1ba52a1a'
GOLD = 'e36194cbff516a1ee803ddc2fa97cd1043403f09'
SRC = 'corev_apu/fpga/src/'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    cva, out = args.cva6.resolve(), args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    frozen = out / 'source-as-executed'
    frozen.mkdir()
    commands, checks = [], []
    report = {'schema': 'ispass2027.e6_pn.observer_embedding.host.v1', 'status': 'running',
              'fixture_only': True, 'synthetic': True, 'source_board_cases': 0,
              'hardware_access': False, 'scope': 'actual three modules and old PATH; external event/quiet fixtures',
              'commands': commands, 'checks': checks, 'sources': [], 'summaries': {},
              'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat()}

    def run(argv, label):
        result = subprocess.run([str(x) for x in argv], capture_output=True, text=True)
        record = {'argv': [str(x) for x in argv], 'exit_code': result.returncode,
                  'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(record)
        (out / (label + '.json')).write_text(json.dumps(record, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError(f'{label}: exit {result.returncode}; original source/output retained')
        return result.stdout

    def check(name, condition):
        if not condition:
            raise AssertionError(name)
        checks.append(name)

    def save(name, raw, origin):
        (frozen / name).write_bytes(raw)
        report['sources'].append({'snapshot': name, 'origin': origin, 'sha256': sha(raw), 'bytes': len(raw)})

    def blob(ref, path):
        return subprocess.check_output(['git', '-C', str(cva), 'show', ref + ':' + path])

    try:
        run(['verilator', '--version'], 'verilator-version')
        report['component_HEAD_at_execution'] = run(['git', '-C', cva, 'rev-parse', 'HEAD'], 'cva6-head').strip()
        report['component_status_at_execution'] = run(['git', '-C', cva, 'status', '--short'], 'cva6-status')
        report['production_diff_at_execution'] = run(['git', '-C', cva, 'diff', PARENT, '--', SRC + 'dma_observer.sv'], 'production-diff')
        for name in ('dma_observer.sv', 'dma_pn_observer.sv', 'dma_pn_control.sv', 'dma_path_observer.sv'):
            raw = (cva / SRC / name).read_bytes()
            save(name, raw, str(cva / SRC / name))
            if name != 'dma_observer.sv':
                check(name + ' unchanged from accepted parent', raw == blob(PARENT, SRC + name))
        original = blob(GOLD, SRC + 'dma_observer.sv')
        save('observer.gold.original.sv', original, GOLD + ':' + SRC + 'dma_observer.sv')
        check('gold module name occurs once', original.count(b'module dma_observer #(') == 1)
        save('observer.gold.renamed.sv', original.replace(b'module dma_observer #(', b'module dma_observer_gold #('),
             'only module name adaptation of preceding fixed blob')
        for name in ('embedding_top.sv', 'check_embedding.cpp', 'check_embedding.py'):
            save(name, (HERE / name).read_bytes(), str(HERE / name))
        profiles = [('default-PNoff-PATHon', 0, 0, 0, 1), ('PNoff-taps-PATHoff', 0, 0, 1, 0),
                    ('PNon-isolated-tapsoff', 1, 0, 0, 1), ('PNon-history-taps', 1, 1, 1, 1),
                    ('PNon-history-PATHoff', 1, 1, 0, 0)]
        for name, pn, history, taps, path in profiles:
            obj = out / ('obj-' + name)
            parameters = [f'-GPn={pn}', f'-GHistory={history}', f'-GTaps={taps}', f'-GPathOn={path}']
            run(['verilator', '--cc', '--exe', '--build', '-j', '2', '--assert', '--public-flat-rw',
                 '--stats', '-Wall', '-Wno-fatal', '--top-module', 'embedding_top', '--Mdir', obj,
                 *parameters, frozen / 'dma_path_observer.sv', frozen / 'dma_pn_observer.sv',
                 frozen / 'dma_pn_control.sv', frozen / 'dma_observer.sv',
                 frozen / 'observer.gold.renamed.sv', frozen / 'embedding_top.sv', frozen / 'check_embedding.cpp'], name + '-build')
            text = run([obj / 'Vembedding_top', str(pn), str(history), str(taps), str(path)], name + '-execution')
            rows = [json.loads(line) for line in text.splitlines()]
            check(name + ' results explicitly fixture-only',
                  all(x['fixture_only'] is True and x['source_board_cases'] == 0 for x in rows))
            check(name + ' real DUT gold finite passed', rows[-1].get('summary') == 'passed' and rows[-1]['cases'] == 15)
            report['summaries'][name] = rows[-1]
        check('frozen sources unchanged', all(sha((frozen / x['snapshot']).read_bytes()) == x['sha256'] for x in report['sources']))
        check('actual component still equals tested bytes', all((cva / SRC / name).read_bytes() == (frozen / name).read_bytes()
              for name in ('dma_observer.sv', 'dma_pn_observer.sv', 'dma_pn_control.sv', 'dma_path_observer.sv')))
        report['status'] = 'passed'
        report['limits'] = ['external quiet/cert/N/STOP/AXI are synthetic; no frontend/backend/IOMMU integration',
                            'old software seq is not a hardware register; no kernel/CLI execution claimed',
                            'private quiet covers old stocks/tracker/AR/AW only; held W/R/B needs external coverage',
                            'policy clear observed only as a module output; actual IOMMU flush not connected',
                            'finite standalone unit evidence; no synthesis, timing or board']
        print(json.dumps({'status': report['status'], 'checks': len(checks), 'summaries': report['summaries'], 'source_board_cases': 0}))
    except Exception as error:
        report['status'], report['error'] = 'failed', str(error)
        raise
    finally:
        report['check_count'] = len(checks)
        report['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        (out / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
