#!/usr/bin/env python3
"""Synthetic local import checks; never dispatch hardware or modify data/."""
import argparse
import collections
import copy
import csv
import io
import json
from pathlib import Path
import subprocess
import statistics
import sys
import tempfile

sys.dont_write_bytecode = True
import import_board_runs as importer

HERE = Path(__file__).resolve().parent


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(importer.json_text(value))


def fixture(directory, phase, seeds):
    _, conditions, ds, blocks = importer.PHASES[phase]
    directory.mkdir(parents=True)
    boot = '00000000-0000-0000-0000-' + ('000000000025' if phase == 'ds25-all' else '000000000100')
    bit = ('a' if phase == 'ds25-all' else 'b')*64
    load = {'status': 'boot_identity_verified', 'kernel_release': 'fixture-kernel',
            'boot_id': boot, 'bit_sha256': bit, 'program_file_sha256_verified': True,
            'configuration_readback_verified': False}
    save(directory/'load.json', load)
    plan = [(b, c) for b in blocks for c in conditions[b % 5:]+conditions[:b % 5]]
    records = []
    for index, (block, condition) in enumerate(plan):
        item = copy.deepcopy(seeds[condition])
        item.update(boot_id=boot, sequence=100+index, case=index+1, block=block, condition=condition, ds=ds)
        path = directory/'uart'/f'command-{index:03d}.json'
        item['raw_command_path'] = str(path)
        cli = json.dumps(item['record'], separators=(',', ':'))
        text = f"KERNEL=fixture-kernel\nBOOT={boot}\nTAINT=0\nPRE_SEQ={item['sequence']-1}\n{cli}\nCASE_EXIT=0\nTAINT_AFTER=0\n"
        save(path, {'exit_code': 0, 'stdout': text, 'command': 'SYNTHETIC local import test', 'raw_path': str(path)})
        records.append(item)
    groups = {}
    for condition in conditions:
        items = [item for item in records if item['condition'] == condition]
        metrics = {}
        for metric in items[0]['metrics']:
            values = [item['metrics'][metric] for item in items if item['metrics'][metric] is not None]
            metrics[metric] = {'median': statistics.median(values) if values else None}
        groups[condition] = {'n': len(items), 'metrics': metrics}
    save(directory/'records.json', records)
    save(directory/'review.json', {'count': len(records), 'sample_count_complete': True, 'groups': groups})
    save(directory/'manifest.json', {'phase': phase, 'load_manifest': str(directory/'load.json'),
        'load_manifest_sha256': importer.sha_bytes((directory/'load.json').read_bytes()),
        'kernel_release': 'fixture-kernel', 'bit_sha256': bit, 'status': 'passed',
        'recorded_cases': len(records), 'planned_cases': plan, 'synthetic_fixture': True})


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seed-records', type=Path, required=True,
                        help='Existing accepted five-condition records, used only to seed clearly synthetic cases')
    args = parser.parse_args()
    seeds = {}
    for item in json.loads(args.seed_records.read_text()):
        seeds.setdefault(item['condition'], item)
    assert set(seeds) == {'Solo', 'A1', 'A8', 'A16', 'A64'}
    checks = []
    with tempfile.TemporaryDirectory(dir=HERE, prefix='.import-test-') as name:
        root = Path(name)
        runs = [root/phase for phase in importer.DS_SWEEP]
        for run, phase in zip(runs, importer.DS_SWEEP):
            fixture(run, phase, seeds)
        output = root/'packet'
        files, config, summary = importer.prepare(runs, output, 'synthetic-ds-sweep')
        assert summary['ds_sweep']['complete'] and summary['ds_sweep']['accepted_points'] == 100
        assert summary['formal100']['accepted_points'] == 0 and summary['fixture_only']
        checks.append('two synthetic n50 phases become a separate complete n100 sweep; formal100 remains zero')
        rows = list(csv.DictReader(io.StringIO(files['board_results.csv'])))
        assert len(rows) == len(config['rows']) and len({r['row_id'] for r in rows}) == len(rows)
        assert {r['experiment'] for r in rows} == {'board_ds_sweep'}
        assert all(r['raw_path'].startswith('paper/ispass2027/') for r in rows)
        samples = [r for r in rows if r['statistic'] == 'sample' and r['metric'] == 'hardware_cycles']
        assert len(samples) == 100 and all(r['n'] == '1' for r in samples)
        assert len({r['boot_id'] for r in samples}) == 2
        checks.append('CSV retains unique scalar IDs, n1 trial rows, source pointers and two actual fixture boot labels')
        again, _, _ = importer.prepare(runs, output, 'synthetic-ds-sweep')
        assert files == again
        checks.append('packet and CSV are byte-for-byte deterministic')
        argv = [sys.executable, str(HERE/'import_board_runs.py'), '--source-root', str(importer.ROOT),
                '--name', 'synthetic-ds-sweep', '--output', str(output)]
        for run in runs:
            argv.extend(['--run', str(run)])
        for extra in [[], ['--check']]:
            subprocess.run(argv+extra, check=True, capture_output=True, text=True)
        assert (output/'board_results.csv').read_text() == files['board_results.csv']
        checks.append('standalone CLI writes only task packet and verifies an identical rebuild')
        bad_path = runs[0]/'uart/command-002.json'
        bad = json.loads(bad_path.read_text())
        bad['exit_code'] = 1
        save(bad_path, bad)
        failed_files, _, failed = importer.prepare(runs, root/'failed-packet', 'synthetic-failed')
        assert not failed['ds_sweep']['complete'] and failed['ds_sweep']['accepted_points'] == 99
        assert failed['ds_sweep']['failed_or_unretained_points'] == 1
        failed_rows = list(csv.DictReader(io.StringIO(failed_files['board_results.csv'])))
        assert any(r['value_status'] == 'failed_observation' for r in failed_rows)
        checks.append('failed command remains visible and reduces accepted count; no replacement or formal100 promotion')
        try:
            importer.prepare(runs, HERE.parents[1]/'data/not-permitted', 'synthetic-rejected')
        except ValueError:
            pass
        else:
            raise AssertionError('output outside E1b task accepted')
        checks.append('shared data output is rejected before writes')
    report = {'status': 'passed', 'scope': 'synthetic host import only; no board data or shared table changed',
              'checks_passed': len(checks), 'checks': checks}
    save(HERE/'host-import-validation.json', report)
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
