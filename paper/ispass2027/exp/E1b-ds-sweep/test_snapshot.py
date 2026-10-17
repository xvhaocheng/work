#!/usr/bin/env python3
"""Check live-prefix immutability and replay historical data without hardware."""
import argparse
import copy
import json
from pathlib import Path
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import import_board_runs as importer
import snapshot_partial as snapshot
from test_import import fixture, save

HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline-root', type=Path, default=Path('/home/xvhaocheng/work'))
    args = parser.parse_args()
    baseline = args.baseline_root/'paper/ispass2027/board/runs/baseline100-20261017-01'
    seeds = {}
    for item in json.loads((baseline/'ds0-middle/records.json').read_text()):
        seeds.setdefault(item['condition'], item)
    checks = []
    with tempfile.TemporaryDirectory(dir=HERE, prefix='.snapshot-test-') as directory:
        root = Path(directory)
        source = root/'live-fixture'
        fixture(source, 'ds25-all', seeds)
        original_manifest = json.loads((source/'manifest.json').read_text())
        original_records = json.loads((source/'records.json').read_text())
        original_manifest.update(status='running')
        original_manifest.pop('recorded_cases')
        save(source/'manifest.json', original_manifest)
        save(source/'records.json', original_records[:5])
        (source/'review.json').unlink()
        frozen = snapshot.freeze_run(source, root/'frozen')
        files, _, report = importer.prepare([frozen], root/'packet', 'synthetic-snapshot')
        assert report['ds_sweep']['accepted_points'] == 5 and not report['ds_sweep']['complete']
        assert report['runs'][0]['manifest_status'] == 'running'
        assert all(g['n_trials'] == 1 for g in report['groups'])
        saved = json.loads((frozen/'manifest.json').read_text())
        assert saved['recorded_cases'] == 5 and saved['derived_snapshot']['observed_original_recorded_cases'] is None
        assert json.loads((frozen/'as-observed/manifest.json').read_text()) == original_manifest
        assert json.loads((frozen/'as-observed/records.json').read_text()) == original_records[:5]
        checks.append('running five-point prefix has actual n1 per condition, complete=false and preserved original count omission/status')
        for a, b in zip(original_records[:5], json.loads((frozen/'records.json').read_text())):
            a, b = copy.deepcopy(a), copy.deepcopy(b)
            a.pop('raw_command_path'); b.pop('raw_command_path')
            assert a == b
        checks.append('normalization leaves every measured value, condition, DS and boot unchanged; original documents remain separate')
        save(source/'records.json', original_records[:10])
        original_manifest.update(status='passed', recorded_cases=50)
        save(source/'manifest.json', original_manifest)
        again, _, _ = importer.prepare([frozen], root/'packet', 'synthetic-snapshot')
        assert files == again
        checks.append('source collector growth and manifest closure cannot change an existing checkpoint or its input hashes')
        actual = root/'historical-reference'
        argv = [sys.executable, str(HERE/'snapshot_partial.py'), '--output', str(actual), '--name', 'historical100-replay']
        for phase in importer.FORMAL:
            argv.extend(['--run', str(baseline/phase)])
        process = subprocess.run(argv, capture_output=True, text=True)
        assert process.returncode == 0, process.stdout+process.stderr
        paper = json.loads((actual/'paper-data.json').read_text())
        analysis = json.loads((actual/'analysis/ds-sweep-summary.json').read_text())
        assert paper['accepted_trials_including_explicit_references'] == 100 and not paper['complete']
        assert paper['historical_ds0_ds50_included'] and not paper['figures']
        assert all(g['hardware_ms']['n'] == (10 if g['ds_cycles_each_direction'] in {0, 50} else 0)
                   for g in analysis['condition_statistics'])
        assert len(analysis['paired_extra_slowdown']) == 60
        checks.append('all 100 actual historical trials replay through local snapshots and reused partial analysis with true n10, 60 real pairs and zero new DS points')
        argv = [sys.executable, str(HERE/'plot_ds_sweep.py'), '--allow-partial', '--check',
                '--import-summary', str(actual/'import/summary.json'), '--long-table', str(actual/'analysis-input.csv'),
                '--data-dir', str(actual/'analysis'), '--figures-dir', str(actual/'figures')]
        process = subprocess.run(argv, capture_output=True, text=True)
        assert process.returncode == 0, process.stdout+process.stderr
        checks.append('reused partial analysis is byte-deterministic and withholds four-point figures')
    receipt = {'status': 'passed', 'scope': 'synthetic live-prefix checks plus actual historical100 arithmetic replay; no FPGA/JTAG/UART',
               'checks_passed': len(checks), 'checks': checks,
               'temporary_snapshot_outputs_removed': True, 'new_ds25_ds100_trials_measured': 0}
    save(HERE/'host-snapshot-validation.json', receipt)
    print(json.dumps(receipt, indent=2))


if __name__ == '__main__':
    main()
