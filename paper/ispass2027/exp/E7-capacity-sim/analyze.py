#!/usr/bin/env python3
"""Derive capacity ratios without promoting simulation to board evidence."""
import csv
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
CONDITIONS = ['Solo', 'A1', 'A8', 'A64']


def analyze():
    records = {}
    input_paths = sorted(HERE.glob('results-*.csv'))
    for path in input_paths:
        with path.open() as source:
            for row in csv.DictReader(source):
                key = (int(row['capacity']), row['condition'])
                if row['checks_passed'] != 'True':
                    raise ValueError('Failed case: ' + str(key))
                if row['base_id'] != 'e7-f5-ddr16-mmio-v1':
                    raise ValueError('Mixed simulation base: ' + str(key))
                if key in records:
                    raise ValueError('Duplicate capacity case: ' + str(key))
                records[key] = row
    rows = []
    for key, original in sorted(records.items(), key=lambda item: (item[0][0], CONDITIONS.index(item[0][1]))):
        capacity, condition = key
        row = dict(original)
        solo = records.get((capacity, 'Solo'))
        a1 = records.get((capacity, 'A1'))
        row['v_vs_solo'] = int(row['v_cycles']) / int(solo['v_cycles']) if solo else ''
        row['v_vs_a1'] = int(row['v_cycles']) / int(a1['v_cycles']) if a1 else ''
        rows.append(row)
    if rows:
        with (HERE / 'results.csv').open('w') as target:
            writer = csv.DictWriter(target, fieldnames=list(rows[0]), lineterminator='\n')
            writer.writeheader(); writer.writerows(rows)
    baseline = {r['condition']: r for r in rows if int(r['capacity']) == 16}
    calibrated = all(c in baseline for c in ['Solo', 'A1', 'A8'])
    trend = calibrated and int(baseline['Solo']['v_cycles']) < int(baseline['A1']['v_cycles']) < int(baseline['A8']['v_cycles'])
    zero_miss = calibrated and all(int(baseline[c][m]) == 0 for c in ['Solo', 'A1'] for m in ['v_walks', 'a_walks'])
    board = dict(solo=56.53868, a1=107.09009, a8=118.22441)
    calibration = dict(status='passed_trend_only' if trend and zero_miss else 'failed' if calibrated else 'incomplete',
        declared_acceptance='16-entry Solo < A1 < A8 and post-warm Solo/A1 zero walks; no DDR tuning to match ratios',
        primary_clock='first V hardware ISSUE through last V DONE, cycles',
        board_ds0_ratio_a1_solo=board['a1']/board['solo'],
        board_ds0_ratio_a8_a1=board['a8']/board['a1'],
        simulation_ratio_a1_solo=float(baseline['A1']['v_vs_solo']) if calibrated else None,
        simulation_ratio_a8_a1=float(baseline['A8']['v_vs_a1']) if calibrated else None,
        generalization='One deterministic configuration each; no replication CI and no numerical board validation',
        missing_cases=[f'{n}/{c}' for n in [16, 32, 64] for c in CONDITIONS if (n, c) not in records])
    (HERE / 'calibration.json').write_text(json.dumps(calibration, indent=2) + '\n')
    lines = ['# E7 capacity simulation results', '',
        'Real frozen iDMA/IOMMU RTL; FIFO behavioral DDR: 16-cycle head startup, then 8 B/cycle.',
        'No FPGA, CPU, Linux, DS injection, invalidations, partition, or early-hit bypass.',
        'V: one source/destination pair. A1/A8/A64: 1/8/64 pairs (2/16/128 distinct translations).',
        'Ordinary MMIO submits one active COPY per lane; this differs from the board batch frontend.',
        'Both footprints are warmed; V executes 4096 x 4096 B. A keeps submitting through V host completion and drains its last offered COPY.',
        'Each completed COPY checks every source/destination byte, 512 R and W beats, and two B responses.', '',
        'Calibration: `' + calibration['status'] + '`; see [calibration.json](calibration.json) for ratios and missing cases.', '',
        '| Entries | Condition | V cycles | V/Solo | V/A1 | V walks | A walks | A jobs incl. tail |',
        '|---:|---|---:|---:|---:|---:|---:|---:|']
    for row in rows:
        lines.append('| {capacity} | {condition} | {v_cycles} | {v_vs_solo:.5f} | {v_vs_a1:.5f} | {v_walks} | {a_walks} | {a_jobs} |'.format(**{
            **row, 'v_vs_solo': float(row['v_vs_solo']) if row['v_vs_solo'] else float('nan'),
            'v_vs_a1': float(row['v_vs_a1']) if row['v_vs_a1'] else float('nan')}))
    lines += ['', 'No confidence interval is inferred from a single deterministic run. Capacity removes residency pressure only when the entire footprint fits.',
        'A zero-walk A1 co-run can still be slower than Solo because payload traffic shares resources. This matrix alone does not separate memory contention from arbitration or head-of-line blocking.',
        'If A8 becomes zero-walk at 32/64 entries, its remaining timing difference from A1 cannot be called pure translation cost.', '',
        'Reproduce: `python3 prepare.py`; use `run.py --help` to build a new named attempt, then `python3 analyze.py`.',
        'Do not reuse existing run directories. All failed attempts and original manifests are retained.',
        'Actual inputs and commands: [manifests](manifests/), [runs](runs/), [test overlay](test-overlay.patch).',
        'Generated C++, binaries, and objects remain local under ignored `build/`; frozen core RTL is the already-versioned closure referenced by the manifests.']
    (HERE / 'results.md').write_text('\n'.join(lines) + '\n')
    print(json.dumps(calibration))


if __name__ == '__main__':
    analyze()
