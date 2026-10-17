#!/usr/bin/env python3
"""Summarize existing PATH aggregates without inventing request distributions."""
import argparse
from collections import defaultdict
import csv
import hashlib
import json
from pathlib import Path
import statistics


def summarize(paths):
    rows, seen, inputs = [], set(), []
    for path in paths:
        records = json.loads(path.read_text())
        manifest_path = path.parent / 'manifest.json'
        manifest = json.loads(manifest_path.read_text())
        configuration = {key: manifest.get(key) for key in
                         ['bit_sha256', 'kernel_release', 'selected_source']}
        inputs.append({'path': str(path.resolve()), 'trials': len(records),
                       'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
                       'manifest_path': str(manifest_path.resolve()),
                       'manifest_sha256': hashlib.sha256(manifest_path.read_bytes()).hexdigest(),
                       'configuration': configuration})
        for trial in records:
            identity = trial['boot_id'], trial['sequence']
            if identity in seen:
                raise ValueError('duplicate boot/sequence: ' + str(identity))
            seen.add(identity)
            config_id = hashlib.sha256(json.dumps(
                {'configuration': configuration, 'input': trial['record']['input'],
                 'schema': trial['record']['schema']}, sort_keys=True).encode()).hexdigest()
            expected = {role + ch + str(stage) for role in 'va' for ch in 'rw'
                        for stage in range(4)}
            if set(trial['path']) != expected:
                raise ValueError('incomplete PATH set')
            for key, values in sorted(trial['path'].items()):
                full, total, maximum = values['full'], values['sum'], values['max']
                if (values['valid'] != 1 or full < 0 or
                    not maximum <= total <= full * maximum or
                    values['start'] + values['entered'] !=
                        full + values['carry'] + values['end']):
                    raise ValueError('invalid PATH data')
                rows.append({'records_path': str(path.resolve()),
                             'raw_command_path': trial['raw_command_path'],
                             'boot_id': trial['boot_id'], 'sequence': trial['sequence'],
                             'case': trial['case'], 'condition': trial['condition'],
                             'ds': trial['ds'], 'lane_stage': key, 'full': full,
                             'configuration_id': config_id,
                             'sum_cycles': total, 'mean_cycles': total / full if full else None,
                             'observed_max_cycles': maximum if full else None,
                             'carry': values['carry'], 'opening': values['start'],
                             'closing': values['end']})
    groups = defaultdict(list)
    for row in rows:
        groups[row['ds'], row['condition'], row['lane_stage'], row['configuration_id']].append(row)
    summary = []
    for (ds, condition, lane_stage, config_id), group in sorted(groups.items()):
        valid = [row for row in group if row['full']]
        full = sum(row['full'] for row in group)
        total = sum(row['sum_cycles'] for row in group)
        summary.append({'ds': ds, 'condition': condition, 'lane_stage': lane_stage,
                        'configuration_id': config_id,
                        'n_trials': len(group), 'n_nonempty_trials': len(valid),
                        'boot_ids': sorted({row['boot_id'] for row in group}),
                        'full_total': full, 'sum_cycles_total': total,
                        'pooled_mean_cycles': total / full if full else None,
                        'median_trial_mean_cycles': statistics.median(
                            row['mean_cycles'] for row in valid) if valid else None,
                        'largest_observed_stage_cycles': max(
                            row['observed_max_cycles'] for row in valid) if valid else None,
                        'request_p50_cycles': None, 'request_p99_cycles': None,
                        'quantile_status': 'unavailable_from_aggregates'})
    return {'schema': 'ispass2027.path_aggregates.v1', 'inputs': inputs,
            'trial_count': len(seen), 'stage_row_count': len(rows),
            'clock_hz': 50000000, 'summary': summary,
            'scope': 'Per-stage full-window transaction aggregates; no end-to-end request CDF or quantiles'}, rows


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--records', nargs='+', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    report, rows = summarize(args.records)
    args.output.mkdir(parents=True, exist_ok=False)
    (args.output / 'summary.json').write_text(json.dumps(report, indent=2) + '\n')
    with (args.output / 'trial-stages.csv').open('w', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]) if rows else [], lineterminator='\n')
        writer.writeheader()
        writer.writerows(rows)
    lines = ['# Existing baseline100 PATH aggregates', '',
             'Offline reuse only; no new hardware run. Cycles at 50 MHz. Each value below is a stage statistic; request p50/p99 remain unavailable.', '',
             '| DS | Condition | V read stage1 median of trial means (cycles) | Largest observed stage1 (cycles) | Trials | Boots |',
             '| --- | --- | ---: | ---: | ---: | ---: |']
    for row in report['summary']:
        if row['lane_stage'] == 'vr1':
            lines.append(f"| {row['ds']} | {row['condition']} | {row['median_trial_mean_cycles']:.6f} | "
                         f"{row['largest_observed_stage_cycles']} | {row['n_trials']} | {len(row['boot_ids'])} |")
    lines += ['', f"Validated {report['trial_count']} existing trials / {len(rows)} stage rows / {len(report['summary'])} groups. Full=0 is null in JSON and blank in CSV; no request quantiles are derived.",
              '', 'Inputs and hashes are retained in summary.json; each stage row retains boot/sequence and the original raw command path. DS50 spans two boots; DS0 one.']
    (args.output / 'summary.md').write_text('\n'.join(lines) + '\n')
    print(json.dumps({'trials': report['trial_count'], 'stage_rows': len(rows),
                      'groups': len(report['summary'])}))


if __name__ == '__main__':
    main()
