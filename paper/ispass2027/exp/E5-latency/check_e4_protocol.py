#!/usr/bin/env python3
"""One finite, explicitly internal fixture compatibility check of frozen E4 protocol."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
from types import SimpleNamespace

import analyze_hist as analysis
import hist_records

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--e4-dir', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    a = parser.parse_args()
    out = a.out.resolve()
    if not out.is_relative_to(HERE) or out.exists():
        parser.error('output must be a new E5 task-local directory')
    out.mkdir()
    e4 = a.e4_dir.resolve()
    spec = importlib.util.spec_from_file_location('e4_fixture_protocol', e4 / 'collect_pages.py')
    protocol = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(protocol)
    source = e4 / 'pages-collector-fixtures.json'
    fixtures = json.loads(source.read_text())
    assert fixtures['fixture_only'] is True
    for path in (source, e4 / 'collect_pages.py', e4 / 'idma_pages.c', Path(__file__), HERE / 'analyze_hist.py', HERE / 'hist_records.py'):
        shutil.copyfile(path, out / path.name)
    cases, first = [], None
    for name, condition in [('leaf4k', 'leaf4k'), ('leaf2m', 'leaf2m'), ('closing_spill', 'leaf4k')]:
        raw = fixtures[name]
        assert raw['fixture_only'] is True
        kernel = re.findall(r'^KERNEL=(.*)$', raw['stdout'], re.M)[0]
        item = protocol.parse(raw['stdout'], kernel, condition, hist_records)
        item.update(case=1 if condition == 'leaf4k' else 2, block=1, condition=condition,
                    schema='ispass2027.e4_pagesize.record.v1', fixture_only=True,
                    raw_command_path='command.json')
        try:
            analysis.rebind_trial(raw, item, kernel)
            raise AssertionError('public fixture guard did not reject')
        except ValueError as error:
            diagnostic = str(error)
        rebound = analysis.rebind_trial(raw, item, kernel, _internal_fixture=True)
        rows = analysis.trial_intervals(rebound, rebound['histogram'], {'mapping_page_bytes': item['mapping_page_bytes']},
                                        _internal_fixture=True)
        assert len(rows) == 16 and all(r['fixture_only'] and r['source_board_cases'] == 0 for r in rows)
        cases.append({'case': name, 'status': 'passed', 'public_rejection': diagnostic,
                      'fixture_only': True, 'source_board_cases': 0, 'stage_rows': 16})
        if name == 'leaf4k': first = raw, item, kernel
    raw, item, kernel = first
    packet = out / 'internal-packet'
    packet.mkdir()
    for name, value in [('command.json', raw), ('records.json', [item])]:
        (packet / name).write_text(json.dumps(value, indent=2) + '\n')
    load = {'status': 'boot_identity_verified', 'fixture_only': True, 'kernel_release': kernel,
            'boot_id': item['boot_id'], 'bit_sha256': 'a' * 64, 'source_board_cases': 0}
    load_path = packet / 'load.json'
    load_path.write_text(json.dumps(load, indent=2) + '\n')
    manifest = {'schema': 'ispass2027.e4_pagesize.run.v1', 'status': 'running', 'fixture_only': True,
                'source_board_cases': 0, 'derived_snapshot': {'scope': 'internal protocol fixture only'},
                'planned_cases': protocol.plan(), 'recorded_cases': 1, 'kernel_release': kernel,
                'boot_id': item['boot_id'], 'bit_sha256': 'a' * 64, 'load_manifest': 'load.json',
                'load_manifest_sha256': sha(load_path)}
    manifest_path = packet / 'manifest.json'
    manifest_path.write_text(json.dumps(manifest, indent=2) + '\n')
    args = SimpleNamespace(records=packet / 'records.json', manifest=manifest_path, source_root=packet,
                           output=out / 'internal-analysis', group_by=['mapping_page_bytes'], allow_partial=True)
    analysis.run_packet(args, _internal_fixture=True)
    report = json.loads((args.output / 'summary.json').read_text())
    assert report['fixture_only'] and report['source_board_cases'] == 0 and not report['complete'] and report['n_trials'] == 1
    receipt = {'status': 'passed', 'scope': 'actual frozen E4 parser/CLI protocol; synthetic fixtures only; no hardware',
               'fixture_only': True, 'source_board_cases': 0, 'cases': cases,
               'sources': {p.name: sha(p) for p in out.iterdir() if p.is_file()},
               'end_to_end': {'n_internal_fixture_trials': 1, 'stage_rows': 16, 'bin_rows': 512,
                              'source_board_cases': 0, 'complete': False}}
    (out / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps(receipt, indent=2))


if __name__ == '__main__':
    main()
