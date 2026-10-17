#!/usr/bin/env python3
"""Synthetic protocol fixtures only: shared spill, rejection, 45-case scheduling/resume.

No fixture is a hardware result. UART is replaced in sys.modules before main;
the real backend-evidence loader is replaced by an explicit fixture-only stub.
"""
import argparse
from collections import Counter
import copy
import json
from pathlib import Path
import re
import shutil
import sys
import types

import collect_p0
from p0_records import parse, validate

BURSTS = {64: 1, 1024: 1, 4096: 2}  # Synthetic fixture configuration, not backend proof.
KERNEL = '6.5.0-rc2-p0-PROTOCOL-FIXTURE'
BOOT = '00000000-0000-0000-0000-000000000001'


def fixture(template, size=64, condition='Solo', closing_spill=False):
    record = copy.deepcopy(template)
    p, r = record['input'], record['result']
    has_a = condition != 'Solo'
    p.update(victim_chunk_bytes=size, aggressor_id=1 if has_a else None,
             aggressor_pages=int(condition[1:]) if has_a else 1)
    r.update(aggressor_did=11 if has_a else None)
    r['victim']['completed_bytes'] = 4096*size
    r['aggressor'] = copy.deepcopy(r['victim']) if has_a else None
    if has_a:
        r['aggressor'].update(submitted=32, completed=32, completed_bytes=32*4096)
    n = 4096*BURSTS[size]
    private_a, leave_a = (5 if closing_spill else 4) if has_a else 0, 4 if has_a else 0
    obs = {'time': {'valid': 1, 'status': 4, 'v_did': 10, 'a_did': 11 if has_a else 0xffffffff,
                    'cycles': 100000, 'h': 95905, 'gap': 4095, 'hmax': 100, 'gapmax': 1,
                    'issued': 4096, 'done': 4096, 'batch_polled': 1, 'done_to_read': 0},
           'axi_v': {'neither': 0, 'v_only': 50000 if has_a else 100000, 'a_only': 0,
                     'both': 50000 if has_a else 0, 'ar': n, 'aw': n, 'rlast': n, 'b': n,
                     'ar_wait': 0, 'aw_wait': 0, 'start': 0, 'end': 0},
           'axi_a': {'ar': private_a, 'aw': private_a, 'rlast': 0, 'b': 0,
                     'ar_wait': 0, 'aw_wait': 0, 'start': 0, 'end': (private_a << 16)+private_a,
                     'shared_ar': n+leave_a, 'shared_aw': n+leave_a,
                     'shared_ar_wait': 0, 'shared_aw_wait': 0}}
    paths = {}
    for role in 'va':
        for channel in 'rw':
            for stage in range(4):
                entered = n if role == 'v' else (private_a if stage == 0 else leave_a)
                full = n if role == 'v' else (leave_a if stage < 3 else 0)
                end = entered-full
                paths[role+channel+str(stage)] = {'valid': 1, 'entered': entered, 'full': full,
                    'carry': 0, 'sum': full, 'max': 1 if full else 0,
                    'start': 0, 'end': end, 'area': full+end}
    return {'record': record, 'observer': obs, 'path': paths, 'sequence': 1, 'boot_id': BOOT}


def output(item, sequence):
    lines = ['KERNEL='+KERNEL, 'BOOT='+BOOT, 'TAINT=0', 'PRE_SEQ='+str(sequence-1),
             json.dumps(item['record']), 'CASE_EXIT=0']
    for part, values in item['observer'].items():
        lines.append(f'idma_batch_obs seq={sequence} part={part} '+
                     ' '.join(f'{k}={v}' for k, v in values.items()))
    for key, values in item['path'].items():
        lines.append(f'idma_path_obs seq={sequence} role={key[0]} channel={key[1]} stage={key[2]} '+
                     ' '.join(f'{k}={v}' for k, v in values.items()))
    return '\n'.join(lines+['TAINT_AFTER=0'])+'\n'


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--CLI-report', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    template = json.loads(a.CLI_report.read_text())['cases'][0]['record']
    checks = []
    for size in BURSTS:
        for condition in ('Solo', 'A1', 'A8'):
            validate(fixture(template, size, condition), BURSTS)
            checks.append({'check': f'fixture V{size} {condition}', 'passed': True})
    closing = fixture(template, 64, 'A8', closing_spill=True)
    metrics = validate(closing, BURSTS)
    assert metrics['shared_minus_private_ar'] == metrics['shared_minus_private_aw'] == -1
    parse(output(closing, 7), KERNEL, BURSTS)
    checks.append({'check': 'legal closing spill=1; shared PATH0 leave passes, old private sum differs', 'passed': True})
    for name, mutate, message in (
        ('shared leave missing', lambda x: x['observer']['axi_a'].__setitem__('shared_ar', 4096), 'shared/PATH0'),
        ('wrong small-packet AXI count', lambda x: x['observer']['axi_v'].__setitem__('ar', 8192), 'victim AXI'),
        ('missing PATH', lambda x: x['path'].pop('vr0'), '16 matched PATH'),
        ('bad PATH stock', lambda x: x['path']['ar0'].__setitem__('end', 0), 'PATH stock/flow'),
        ('bad private stock', lambda x: x['observer']['axi_a'].__setitem__('end', 0), 'private AXI stock/flow'),
        ('wrong bytes', lambda x: x['record']['result']['victim'].__setitem__('completed_bytes', 4096*4096), 'byte accounting'),
        ('DMA quarantine', lambda x: x['record']['result']['victim'].__setitem__('quarantined', 1), 'data error/quarantine')):
        row = copy.deepcopy(closing); mutate(row)
        try: validate(row, BURSTS)
        except ValueError as error:
            assert message in str(error), str(error)
            checks.append({'check': name, 'passed': True, 'rejection': str(error)})
        else: raise AssertionError(name+' was accepted')

    # Exercise actual collector entry through fake UART. No real Console import.
    inputs = a.output / 'inputs'; inputs.mkdir()
    bit = 'fixture-only-bit'
    for name, value in {'load': {'status': 'boot_identity_verified', 'kernel_release': KERNEL,
                                'boot_id': BOOT, 'bit_sha256': bit},
                        'fpga': {'status': 'passed', 'direct_ds_wire': True,
                                 'parameters': {'clock_hz': 50000000, 'ds_cycles': [0, 0]},
                                 'artifacts': {'fixture.bit': bit}},
                        'software': {'status': 'passed', 'kernel_release': KERNEL,
                                     'kernel_critical_sources': {'include/uapi/linux/idma_p0.h': 'fixture'}},
                        'backend': {'status': 'fixture_only', 'fixture_only': True}}.items():
        (inputs / (name+'.json')).write_text(json.dumps(value))
    collect_p0.backend_evidence = lambda _: BURSTS
    state = {'start': 0, 'calls': 0, 'fail_at': None, 'closed': False}

    class Console:
        def __init__(self, port, output_path):
            assert port == '/NO-REAL-UART-PROTOCOL-FIXTURE'
            self.path = output_path; self.path.mkdir()

        def command(self, text, timeout):
            assert timeout == 120
            index = state['start']+state['calls']; state['calls'] += 1
            point = collect_p0.plan()[index]
            assert f"--victim-chunk {point['victim_chunk_bytes']} " in text
            row = fixture(template, point['victim_chunk_bytes'], point['condition'], True)
            if state['fail_at'] == index:
                row['record']['result']['victim']['data_errors'] = 1
            path = self.path / f'command-{index+1:03}.json'
            raw = {'stdout': output(row, index+1), 'exit_code': 0, 'raw_path': str(path.resolve())}
            path.write_text(json.dumps(raw)); return raw

        def close(self): state['closed'] = True

    sys.modules['uart'] = types.SimpleNamespace(Console=Console, DEFAULT_PORT='/NO-REAL-UART-PROTOCOL-FIXTURE')

    def collect(target, resume=None):
        argv = ['collect_p0.py', '--output', str(target), '--expected-kernel', KERNEL,
                '--board-tools', str(inputs), '--load-manifest', str(inputs/'load.json'),
                '--fpga-manifest', str(inputs/'fpga.json'), '--software-manifest', str(inputs/'software.json'),
                '--backend-evidence', str(inputs/'backend.json')]
        if resume: argv += ['--resume-run', str(resume)]
        sys.argv = argv; collect_p0.main()

    collect(a.output / 'complete-fixture')
    records = json.loads((a.output/'complete-fixture/records.json').read_text())
    assert len(records) == state['calls'] == 45 and state['closed']
    assert set(Counter((r['victim_chunk_bytes'], r['condition']) for r in records).values()) == {5}
    checks.append({'check': 'actual collector fake UART executes exactly 45 balanced trials', 'passed': True})
    state.update(start=0, calls=0, fail_at=2, closed=False)
    try: collect(a.output/'failure-fixture')
    except ValueError: pass
    else: raise AssertionError('failed data was accepted')
    failed = json.loads((a.output/'failure-fixture/manifest.json').read_text())
    assert failed['status'] == 'failed' and failed['recorded_cases'] == 2 and state['calls'] == 3 and state['closed']
    checks.append({'check': 'actual collector stops after first error, preserves successful prefix, closes without retry', 'passed': True})
    prefix = a.output/'old-parser-prefix-fixture'; (prefix/'uart').mkdir(parents=True)
    old = json.loads((a.output/'complete-fixture/manifest.json').read_text())
    old.update(status='failed', recorded_cases=1, error='synthetic old private/shared equality rejection')
    (prefix/'manifest.json').write_text(json.dumps(old))
    for path in sorted((a.output/'complete-fixture/uart').glob('command-*.json'))[:2]:
        shutil.copyfile(path, prefix/'uart'/path.name)
    state.update(start=2, calls=0, fail_at=None, closed=False)
    collect(a.output/'resume-fixture', prefix)
    resumed = json.loads((a.output/'resume-fixture/manifest.json').read_text())
    assert resumed['status'] == 'passed' and resumed['recorded_cases'] == 45 and resumed['recovered_cases'] == 2
    assert state['calls'] == 43 and state['closed']
    checks.append({'check': 'raw recovery of valid closing-spill prefix skips two complete cases, executes only 43 remaining', 'passed': True})
    report = {'status': 'passed', 'scope': 'synthetic protocol fixtures and fake UART only',
              'fixture_only': True, 'new_board_measurements': 0, 'checks': checks}
    (a.output/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({'status': 'passed', 'checks': len(checks), 'report': str((a.output/'report.json').resolve())}))


if __name__ == '__main__':
    main()
