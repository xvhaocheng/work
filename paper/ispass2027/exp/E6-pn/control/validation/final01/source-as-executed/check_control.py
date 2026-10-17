#!/usr/bin/env python3
"""Finite standalone actual PN control DUT; external quiet inputs are fixtures."""
import argparse
import datetime
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import xml.etree.ElementTree as ET

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
RTL = 'corev_apu/fpga/src/dma_pn_control.sv'


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6', type=Path, required=True)
    parser.add_argument('--contract', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    cva = args.cva6.resolve()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    frozen = out / 'source-as-executed'
    frozen.mkdir()
    commands, checks = [], []
    report = {'schema': 'ispass2027.e6_pn.control.host.v1', 'status': 'running',
              'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
              'fixture_only': True, 'synthetic': True, 'source_board_cases': 0,
              'hardware_access': False, 'commands': commands, 'checks': checks,
              'sources': [], 'summaries': {},
              'scope': 'standalone control only; external facts/counters synthetic; no integration'}

    def run(argv, label):
        result = subprocess.run([str(x) for x in argv], text=True, capture_output=True)
        saved = {'argv': [str(x) for x in argv], 'exit_code': result.returncode,
                 'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(saved)
        (out / (label + '.json')).write_text(json.dumps(saved, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError(f'{label}: exit {result.returncode}; original source and output retained')
        return result.stdout

    def check(label, condition):
        if not condition:
            raise AssertionError(label)
        checks.append(label)

    try:
        run(['verilator', '--version'], 'verilator-version')
        report['cva6_head_at_execution'] = run(['git', '-C', cva, 'rev-parse', 'HEAD'], 'cva6-head').strip()
        report['cva6_status_at_execution'] = run(['git', '-C', cva, 'status', '--short'], 'cva6-status')
        for source, name in [(cva / RTL, 'dma_pn_control.sv'),
                             (HERE / 'check_control.cpp', 'check_control.cpp'),
                             (Path(__file__).resolve(), 'check_control.py'),
                             (args.contract.resolve(), 'runtime-contract-v1.md')]:
            raw = source.read_bytes()
            (frozen / name).write_bytes(raw)
            report['sources'].append({'origin': str(source), 'snapshot': str(frozen / name),
                                      'sha256': digest(raw), 'bytes': len(raw)})

        profiles = [('default-disabled', 0, 0, []),
                    ('disabled-history', 0, 1, ['-GHistoryControlEnable=1']),
                    ('enabled-isolated', 1, 0, ['-GEnable=1']),
                    ('enabled-history', 1, 1, ['-GEnable=1', '-GHistoryControlEnable=1'])]
        for name, enable, history, parameters in profiles:
            model = out / ('obj-' + name)
            run(['verilator', '--cc', '--exe', '--build', '-j', '2', '--assert',
                 '--public-flat-rw', '--stats', '-Wall', '-Wno-fatal',
                 '--top-module', 'dma_pn_control', '--Mdir', model, *parameters,
                 frozen / 'dma_pn_control.sv', frozen / 'check_control.cpp'], name + '-build')
            text = run([model / 'Vdma_pn_control', str(enable), str(history)], name + '-execution')
            rows = [json.loads(line) for line in text.splitlines()]
            check(name + ' explicitly synthetic results',
                  all(x['fixture_only'] is True and x['source_board_cases'] == 0 for x in rows))
            summary = rows[-1]
            check(name + ' actual DUT finite pass', summary.get('summary') == 'passed' and
                  summary['enable'] == enable and summary['history'] == history and summary['cases'] == 11)
            report['summaries'][name] = summary
            # Independent elaboration evidence: disabled generate has no state.
            xml = out / (name + '.xml')
            run(['verilator', '--xml-only', '--xml-output', xml, '-Wall', '-Wno-fatal',
                 '--top-module', 'dma_pn_control', '--Mdir', out / ('xml-' + name),
                 *parameters, frozen / 'dma_pn_control.sv'], name + '-elaboration')
            tree = ET.parse(xml)
            variables = tree.findall('.//module/var') + tree.findall('.//begin/var')
            state = [x for x in variables if x.get('name') in ('mode_word_q', 'last_rejected_q')]
            if enable:
                check(name + ' only declared control state present',
                      sorted(x.get('name') for x in state) == ['last_rejected_q', 'mode_word_q'])
            else:
                check(name + ' disabled generate has no state', len(state) == 0 and
                      not tree.findall('.//always'))
            report.setdefault('elaboration_state', {})[name] = [x.attrib for x in state]
        check('snapshots unchanged during validation',
              all(digest(Path(item['snapshot']).read_bytes()) == item['sha256'] for item in report['sources']))
        check('actual component still equals compiled bytes',
              (cva / RTL).read_bytes() == (frozen / 'dma_pn_control.sv').read_bytes())
        report['status'] = 'passed'
        report['limits'] = ['no top, wrapper, old observer, frontend, IOMMU or helper changed',
                            'quiet/armed/busy/certificate/ARM inputs are interface stimuli only',
                            'no complete-system quiet or combinational-loop proof',
                            'four nominal state bits when enabled; no index/counter state',
                            'finite standalone Verilator evidence, no synthesis/timing/board']
        print(json.dumps({'status': report['status'], 'checks': len(checks),
                          'summaries': report['summaries'], 'source_board_cases': 0}))
    except Exception as error:
        report['status'] = 'failed'
        report['error'] = str(error)
        raise
    finally:
        report['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
        report['check_count'] = len(checks)
        (out / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
