#!/usr/bin/env python3
"""Compile only the actual standalone DUT; finite synthetic host simulation."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
BASE = '8aac3fcd345af7686a55af1867783fd6b91a0069'
DUT = 'corev_apu/fpga/src/dma_pn_observer.sv'


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cva6-worktree', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    root = args.cva6_worktree.resolve()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    source = root / DUT
    tb = HERE / 'check_observer.cpp'
    commands = []
    checks = []
    receipt = {'schema': 'ispass2027.e6_pn.passive_observer.dut.v1',
               'fixture_only': True, 'source_board_cases': 0, 'status': 'running',
               'hardware_access': False, 'full_build': False,
               'simulation': 'actual standalone Verilator DUT, synthetic event traces',
               'commands': commands, 'checks': checks,
               'source_sha256': {str(x): sha(x) for x in (source, tb, Path(__file__).resolve())},
               'cva6_base': BASE, 'endpoint_injection': {
                   'method': '--public-flat-rw actual packed state mutation, no test RTL ports',
                   'NumPorts': 4, 'packed_state_bits': 342,
                   'bit_offsets': {'a_age': 278, 'u_age': 150, 'n10': 54, 'n11': 22},
                   'scope': '6 endpoint scenarios; finite ordinary traces use absolute cycle timestamps'}}

    def run(argv, name, cwd=None):
        result = subprocess.run([str(x) for x in argv], cwd=cwd, text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        item = {'argv': [str(x) for x in argv], 'cwd': str(cwd) if cwd else None,
                'exit_code': result.returncode, 'stdout': result.stdout, 'stderr': result.stderr}
        commands.append(item)
        (out / (name + '.json')).write_text(json.dumps(item, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError(f'{name}: exit {result.returncode}; raw output retained')
        return result.stdout

    def check(name, condition):
        if not condition:
            raise AssertionError(name)
        checks.append(name)

    try:
        run(['git', 'merge-base', '--is-ancestor', BASE, 'HEAD'], 'base-ancestor', root)
        receipt['cva6_head_at_test'] = run(['git', 'rev-parse', 'HEAD'], 'cva6-head', root).strip()
        receipt['cva6_status_at_test'] = run(['git', 'status', '--short'], 'cva6-status', root)
        run(['verilator', '--version'], 'verilator-version')
        summaries = {}
        for enabled in (True, False):
            label = 'enabled' if enabled else 'default-disabled'
            obj = out / label
            argv = ['verilator', '--cc', '--exe', '--build', '-j', '2', '-Wall',
                    '--public-flat-rw', '--top-module', 'dma_pn_observer', '--Mdir', obj]
            if enabled:
                argv += ["-GEnable=1'b1", '-CFLAGS', '-std=c++14']
            else:
                # Constant-disabled inputs are deliberately unused. No other
                # warning or production logic is suppressed.
                argv += ['-Wno-UNUSED', '-CFLAGS', '-std=c++14 -DPN_DISABLED']
            argv += [source, tb]
            run(argv, label + '-build')
            text = run([obj / 'Vdma_pn_observer'], label + '-execution')
            rows = [json.loads(line) for line in text.splitlines()]
            check(label + ' every record explicitly fixture-only',
                  all(x.get('fixture_only') is True and x.get('source_board_cases') == 0 for x in rows))
            summary = rows[-1]
            check(label + ' actual DUT scoreboard passed', summary.get('summary') == 'passed')
            summaries[label] = summary
            if enabled:
                cases = [x for x in rows if 'scenario' in x]
                check('27 unique actual DUT finite scenarios', len(cases) == 27 and
                      len({x['scenario'] for x in cases}) == 27)
                receipt['finite_scenarios'] = cases
                check('per-cycle raw event trace retained', any('event_cycle' in x for x in rows))
            else:
                header = (obj / 'Vdma_pn_observer.h').read_text()
                check('default Enable0 elaborates no observer state', 'state_q' not in header)
        check('source bytes unchanged through actual simulation',
              all(sha(path) == digest for path, digest in receipt['source_sha256'].items()))
        receipt['summaries'] = summaries
        receipt['status'] = 'passed'
        receipt['limits'] = [
            'standalone DUT only; ARM/STOP/allquiet/target/Naccept inputs driven by unit harness',
            'not connected to system top/wrap/MMIO or original observer',
            'no synthesis, FPGA timing, full platform build or board run',
            'endpoint injection does not represent executing 2**64 or 2**32 real cycles',
            '4x64 age/latest +2x32 N +22 flags/roles =342 RTL state bits at NumPorts4']
        print(json.dumps({'status': 'passed', 'source_board_cases': 0,
                          'checks': len(checks), 'DUT_summaries': summaries}))
    except Exception as error:
        receipt['status'] = 'failed'
        receipt['error'] = str(error)
        raise
    finally:
        (out / 'validation.json').write_text(json.dumps(receipt, indent=2) + '\n')


if __name__ == '__main__':
    main()
