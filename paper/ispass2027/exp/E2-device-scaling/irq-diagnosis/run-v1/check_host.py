#!/usr/bin/env python3
"""Protocol fixtures only; never open UART or execute rendered commands."""
import argparse
import copy
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--historical-n2', type=Path, required=True)
    parser.add_argument('--report', type=Path, required=True)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location('e2_probe_run', HERE/'prepare_run.py')
    run = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(run)
    source = json.loads(args.historical_n2.read_text())
    historical = run.original_collector().parse(source['stdout'], '6.5.0-rc2-ispass2027-full-e3-v1', 'N2_A1')
    boot = '00000000-0000-0000-0000-000000000001'
    plan = run.make_plan(boot, review_template=True)
    checks = []

    def check(name, condition):
        if not condition:
            raise AssertionError(name)
        checks.append({'name': name, 'result': 'passed'})

    def reject(name, function, error_text):
        try:
            function()
        except (AssertionError, ValueError) as error:
            if error_text not in str(error):
                raise AssertionError((name, str(error))) from error
            checks.append({'name': name, 'result': 'rejected', 'error': str(error)})
        else:
            raise AssertionError('unexpected acceptance: ' + name)

    check('exactly two ordered case windows', [s['condition'] for s in plan['steps'] if 'original_cli' in s] == ['N2_A1', 'N4_A8'])
    check('no automatic continuation/retry/CTRL-C/reset', not plan['automatic_continuation'] and not plan['automatic_retry_or_ctrl_c_or_reset'])
    check('preflight has no DMA command', 'idma_scaling' not in plan['steps'][0]['command'])
    for step in [s for s in plan['steps'] if 'original_cli' in s]:
        command, cli = step['command'], step['original_cli']
        enable = command.index(f'printf "1\\n" > {run.CONTROL}')
        invoke = command.index(cli)
        freeze = command.index(f'printf "0\\n" > {run.CONTROL}')
        check(step['condition']+' enable/CLI/freeze/CASE_EXIT order',
              command.index('PROBE_BEFORE_END') < enable < invoke < freeze < command.index('CASE_EXIT='))
        check(step['condition']+' no active snapshot read', f'cat {run.SNAPSHOT}' not in command[enable:freeze])
        check(step['condition']+' original budgets', command.count(cli) == 1 and '--timeout-ms 60000' in cli and step['host_timeout_seconds'] == 180)
    check('UART command plus wrapper below target 4096-byte line limit', max(len(s['command'].encode()) for s in plan['steps'])+200 < 4096)
    for step in plan['steps'] + plan['recovery_proposals_not_automatic']:
        subprocess.run(['sh', '-n'], input=('( '+step['command']+' )\n').encode(), check=True, capture_output=True)
    check('all target command bodies pass host POSIX shell syntax only', True)
    reject('review template rejected as real trial plan', lambda: run.validate_cli(plan, source, 'N2_A1'), 'review template')

    # These are explicit synthetic headers/command bindings around one saved old
    # passed N2 output. They test host gates, never a new Image or probe runtime.
    plan['review_template'] = False
    raw = copy.deepcopy(source)
    raw['command'] = next(s['command'] for s in plan['steps'] if s['name'] == 'N2_A1_window')
    raw['stdout'] = raw['stdout'].replace('KERNEL=6.5.0-rc2-ispass2027-full-e3-v1', 'KERNEL='+run.KERNEL)
    raw['stdout'] = raw['stdout'].replace('BOOT='+historical['boot_id'], 'BOOT='+boot)
    result = run.validate_cli(plan, raw, 'N2_A1')
    check('synthetic header fixture preserves original successful CLI gates', result['status'] == 'original_cli_gates_passed')
    reject('host timeout stops without next phase', lambda: run.validate_cli(plan, dict(raw, exit_code=None), 'N2_A1'), 'host timeout/error')
    reject('nonzero host result stops', lambda: run.validate_cli(plan, dict(raw, exit_code=1), 'N2_A1'), 'host timeout/error')
    reject('nonzero CLI result still stops', lambda: run.validate_cli(plan, dict(raw, stdout=raw['stdout'].replace('CASE_EXIT=0', 'CASE_EXIT=124')), 'N2_A1'), 'CLI exit')
    reject('changed actual command stops', lambda: run.validate_cli(plan, dict(raw, command=raw['command']+'; true'), 'N2_A1'), 'actual target command differs')
    reject('taint still rejects', lambda: run.validate_cli(plan, dict(raw, stdout=raw['stdout'].replace('TAINT_AFTER=0', 'TAINT_AFTER=16384')), 'N2_A1'), 'taint')
    reject('wrong boot still rejects', lambda: run.validate_cli(dict(plan, boot_id='10000000-0000-0000-0000-000000000001'), raw, 'N2_A1'), 'boot changed')

    def mutate_record(field, value):
        changed = copy.deepcopy(raw)
        match = re.search(r'\{\s*"schema"\s*:\s*"idma\.multi_batch\.v1"', changed['stdout'])
        record, consumed = json.JSONDecoder().raw_decode(changed['stdout'][match.start():])
        record['result'][field] = value
        changed['stdout'] = changed['stdout'][:match.start()] + json.dumps(record) + changed['stdout'][match.start()+consumed:]
        return changed

    reject('STOP mask still rejects', lambda: run.validate_cli(plan, mutate_record('stopped_mask', 1), 'N2_A1'), 'complete lifecycle: stopped_mask')
    reject('drain mask still rejects', lambda: run.validate_cli(plan, mutate_record('drained_mask', 1), 'N2_A1'), 'complete lifecycle: drained_mask')
    bad_stats = copy.deepcopy(historical['record']['result']['stats'])
    bad_stats[0]['data_errors'] = 1
    reject('data errors still reject', lambda: run.validate_cli(plan, mutate_record('stats', bad_stats), 'N2_A1'), 'data/quiet')
    n4 = dict(raw, command=next(s['command'] for s in plan['steps'] if s['name'] == 'N4_A8_window'))
    reject('N4 requires prior N2 evidence', lambda: run.validate_cli(plan, n4, 'N4_A8'), 'previously accepted N2')
    report = {'schema': 'e2-timer-probe-run-host-checks-v1', 'status': 'passed', 'checks_total': len(checks),
              'checks': checks, 'script_sha256': run.sha(HERE/'prepare_run.py'),
              'historical_N2_source': str(args.historical_n2.resolve()), 'historical_N2_source_sha256': run.sha(args.historical_n2),
              'synthetic_header_changes': ['kernel release', 'boot ID', 'rendered command binding'],
              'synthetic_tests_only': True, 'new_probe_runtime_data': False, 'board_operations': 0,
              'limits': 'No UART, target command, Image runtime, control/per-open lifecycle, snapshot/proc semantics or new performance measurements executed.'}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'status': 'passed', 'checks': len(checks), 'board_operations': 0}))


if __name__ == '__main__':
    main()
