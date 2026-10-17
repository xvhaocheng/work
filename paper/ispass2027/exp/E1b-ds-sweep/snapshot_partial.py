#!/usr/bin/env python3
"""Freeze observed collector prefixes and publish actual partial paper data."""
import argparse
import copy
import csv
import datetime
import json
from pathlib import Path
import subprocess
import sys

sys.dont_write_bytecode = True
import import_board_runs as importer
import table_schema

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]


def rel(path):
    return Path(path).resolve().relative_to(ROOT).as_posix()


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(importer.json_text(value))


def read_observed(path):
    data = Path(path).read_bytes()
    try:
        return data, json.loads(data)
    except json.JSONDecodeError as error:
        raise ValueError('Collector file is between writes; no paper data published: ' + str(path)) from error


def freeze_run(source, target):
    """Preserve original bytes, normalize only counts and evidence locations."""
    source, target = source.resolve(), target.resolve()
    manifest_bytes, manifest = read_observed(source/'manifest.json')
    records_bytes, records = read_observed(source/'records.json')
    if not records:
        raise ValueError('No accepted collector prefix yet: ' + str(source))
    if manifest['phase'] not in importer.PHASES:
        raise ValueError('Unsupported collector phase')
    plan = [list(p) for p in manifest['planned_cases']]
    actual = [[item['block'], item['condition']] for item in records]
    if actual != plan[:len(records)] or len(records) > len(plan):
        raise ValueError('Observed records are not the exact planned prefix')
    loaded_path = importer.run_reference(manifest['load_manifest'], source)
    load_bytes, loaded = read_observed(loaded_path)
    if importer.sha_bytes(load_bytes) != manifest['load_manifest_sha256']:
        raise ValueError('Existing load receipt hash mismatch')
    if any(loaded.get(key) for key in ('resumed_from', 'authentication_receipt', 'sd_evidence', 'raw_receipt')):
        raise ValueError('Linked load receipt requires a full immutable evidence snapshot; no links dropped')
    command_data, replacements = [], {}
    for index, item in enumerate(records):
        original = importer.run_reference(item['raw_command_path'], source)
        data, command = read_observed(original)
        # A prefix already retained by collect.py must still bind its original
        # command; never publish a candidate or an in-flight command as a sample.
        if command.get('exit_code') != 0 or importer.batch_documents(command.get('stdout', '')) != [item['record']]:
            raise ValueError('Observed retained record does not bind a successful original command')
        copied = target/'uart'/f'command-case{index+1:04d}.json'
        replacements[original] = rel(copied)
        command_data.append((copied, data))
    normalized = copy.deepcopy(records)
    for item in normalized:
        original = importer.run_reference(item['raw_command_path'], source)
        item['raw_command_path'] = replacements[original]
    derived = copy.deepcopy(manifest)
    derived.update(recorded_cases=len(records), load_manifest=rel(target/'load.json'))
    if manifest.get('recovered_raw_commands'):
        derived['recovered_raw_commands'] = [replacements[importer.run_reference(p, source)]
                                             for p in manifest['recovered_raw_commands']]
    # Existing failed recovery sources are immutable. Keep their actual paths;
    # do not relabel their failure history or forge a new successful attempt.
    if manifest.get('recovered_from'):
        recovered = importer.run_reference(manifest['recovered_from'], source)
        recovered.relative_to(ROOT)
        derived['recovered_from'] = rel(recovered)
    target.mkdir(parents=True, exist_ok=False)
    observed = target/'as-observed'
    observed.mkdir()
    originals = [('manifest.json', manifest_bytes), ('records.json', records_bytes)]
    for name, data in originals:
        (observed/name).write_bytes(data)
    derived['snapshot_inputs'] = [{'raw_path': rel(observed/name), 'sha256': importer.sha_bytes(data)}
                                  for name, data in originals]
    derived['derived_snapshot'] = {
        'source_run': str(source), 'captured_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
        'original_manifest': rel(observed/'manifest.json'), 'original_records': rel(observed/'records.json'),
        'changed_fields': ['manifest.recorded_cases := observed prefix length (running collector updates only at close)',
                           'manifest.load_manifest := copied receipt; original receipt bytes/hash retained',
                           'records[*].raw_command_path := copied original command evidence',
                           'recovery evidence locations normalized where present; counts/status retained'],
        'unchanged': 'all sample values, trial order, condition, DS, boot, kernel, BIT, source commits and actual campaign status',
        'observed_trials': len(records), 'planned_trials': len(plan),
        'observed_original_recorded_cases': manifest.get('recorded_cases'),
        'source_status': manifest['status']}
    (target/'load.json').write_bytes(load_bytes)
    for path, data in command_data:
        path.parent.mkdir(exist_ok=True)
        path.write_bytes(data)
    if (source/'review.json').is_file():
        review_bytes, review = read_observed(source/'review.json')
        if review.get('count') == len(records):
            (target/'review.json').write_bytes(review_bytes)
    save(target/'records.json', normalized)
    save(target/'manifest.json', derived)
    return target


def combine_tables(paths):
    rows, seen = [], set()
    for path in paths:
        with path.open(newline='') as stream:
            for row in csv.DictReader(stream):
                if row['row_id'] in seen:
                    raise ValueError('Duplicate table row ID across checkpoint inputs: ' + row['row_id'])
                seen.add(row['row_id'])
                rows.append(row)
    return table_schema.csv_text(rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', action='append', type=Path, required=True,
                        help='Actual collector run; all currently retained samples are frozen, with no selection')
    parser.add_argument('--output', type=Path, required=True, help='New immutable checkpoint under this E1b directory')
    parser.add_argument('--name', required=True, help='Stable namespace for this checkpoint')
    parser.add_argument('--reference-packet', action='append', type=Path, default=[],
                        help='Existing immutable import packet, e.g. the one-time historical DS0/DS50 snapshot')
    args = parser.parse_args()
    output = args.output.resolve()
    output.relative_to(HERE)
    if output.exists():
        raise ValueError('Checkpoint already exists; choose a new name without overwriting')
    if len({path.resolve() for path in args.run}) != len(args.run):
        raise ValueError('Repeated source run')
    output.mkdir(parents=True)
    runs = [freeze_run(path, output/'raw'/f'{index+1:02d}-{path.name}')
            for index, path in enumerate(args.run)]
    packet = output/'import'
    files, _, summary = importer.prepare(runs, packet, args.name)
    packet.mkdir()
    for name, content in files.items():
        (packet/name).write_text(content)
    if summary['fixture_only']:
        raise ValueError('Synthetic fixture cannot be published as actual partial paper data')
    references = [path.resolve() for path in args.reference_packet]
    table_path = output/'analysis-input.csv'
    table_path.write_text(combine_tables([p/'board_results.csv' for p in references]+[packet/'board_results.csv']))
    command = [sys.executable, str(HERE/'plot_ds_sweep.py'), '--allow-partial',
               '--long-table', str(table_path), '--data-dir', str(output/'analysis'),
               '--figures-dir', str(output/'figures')]
    for path in references+[packet]:
        command.extend(['--import-summary', str(path/'summary.json')])
    subprocess.run(command, check=True)
    analysis = json.loads((output/'analysis/ds-sweep-summary.json').read_text())
    with table_path.open(newline='') as stream:
        accepted_rows = [row for row in csv.DictReader(stream)
                         if row['statistic'] == 'sample' and row['sample_set'].endswith(':accepted')]
    (output/'accepted-board-results.csv').write_text(table_schema.csv_text(accepted_rows))
    paper_data = {'status': analysis['status'], 'complete': analysis['complete'],
                  'accepted_trials_including_explicit_references': analysis['accepted_trials'],
                  'four_ds_target_trials': 200,
                  'historical_ds0_ds50_included': any(run['ds_cycles_each_direction'] in {0, 50}
                                                       for run in analysis['source_runs']),
                  'reference_packets': [rel(path) for path in references],
                  'new_ds_matrix': summary.get('ds_sweep'),
                  'trial_table': rel(output/'accepted-board-results.csv'),
                  'full_import_table_with_failure_flags': rel(packet/'board_results.csv'),
                  'statistics': rel(output/'analysis/ds-sweep-summary.json'),
                  'figures': analysis['figure_files'],
                  'scope': 'actual accepted samples and explicit failures/missing points; no imputation or new board operation'}
    save(output/'paper-data.json', paper_data)
    print(json.dumps(paper_data, ensure_ascii=False))


if __name__ == '__main__':
    main()
