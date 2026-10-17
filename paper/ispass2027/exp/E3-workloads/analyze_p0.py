#!/usr/bin/env python3
"""Analyze a complete real P0 collection without modifying source/raw logs."""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import statistics

from p0_records import backend_evidence, require, validate


def summarize(records, manifest, bursts):
    require(manifest['schema'] == 'ispass2027-p0-collection-v1' and manifest['status'] == 'passed'
            and manifest['ds'] == '0/0' and manifest['clock_hz'] == 50000000,
            'requires passed P0 DS0 collection')
    require(manifest['recorded_cases'] == len(records) == 45, 'P0 requires all 45 actual cases')
    require({x['case'] for x in records} == set(range(1, 46)), 'P0 case identifiers incomplete/duplicated')
    groups, seen = defaultdict(list), set()
    for item in records:
        key = item['boot_id'], item['sequence']
        require(key not in seen and item['boot_id'] == manifest['boot_id'], 'P0 duplicate sequence/boot mismatch')
        seen.add(key)
        metrics = validate(item, bursts)  # Do not trust cached derived metrics.
        p = item['record']['input']
        condition = 'Solo' if p['aggressor_id'] is None else 'A'+str(p['aggressor_pages'])
        require(item['condition'] == condition and item['victim_chunk_bytes'] == p['victim_chunk_bytes']
                and item['ds'] == '0/0', 'P0 metadata mismatch')
        require(manifest['planned_cases'][item['case']-1] ==
                {k: item[k] for k in ('block', 'victim_chunk_bytes', 'condition')}, 'P0 planned order mismatch')
        groups[p['victim_chunk_bytes'], condition].append({
            'case': item['case'], 'block': item['block'], 'boot_id': item['boot_id'],
            'sequence': item['sequence'], 'raw_command_path': item['raw_command_path'], **metrics})
    expected = {(size, condition) for size in (64, 1024, 4096) for condition in ('Solo', 'A1', 'A8')}
    require(set(groups) == expected and all(len(x) == 5 for x in groups.values()), 'P0 nine cells x five required')
    rows = []
    for (size, condition), samples in sorted(groups.items()):
        median = {k: statistics.median(x[k] for x in samples if x[k] is not None)
                  if any(x[k] is not None for x in samples) else None
                  for k in ('hardware_ms', 'hardware_MiB_s', 'victim_iotlb_miss', 'aggressor_iotlb_miss')}
        refs = {ref: statistics.median(x['hardware_ms'] for x in groups[size, ref]) for ref in ('Solo', 'A1')}
        rows.append({'victim_chunk_bytes': size, 'aggressor_chunk_bytes': 4096, 'condition': condition,
                     'n': 5, **median, 'hardware_ms_min': min(x['hardware_ms'] for x in samples),
                     'hardware_ms_max': max(x['hardware_ms'] for x in samples),
                     'slowdown_vs_Solo_x': median['hardware_ms']/refs['Solo'],
                     'slowdown_vs_A1_x': median['hardware_ms']/refs['A1'], 'samples': samples})
    return {'status': 'analysis_of_complete_P0', 'source_board_cases': 45, 'clock_hz': 50000000,
            'rows': rows, 'boot_id': manifest['boot_id'], 'bit_sha256': manifest['bit_sha256'],
            'kernel_release': manifest['kernel_release'],
            'counter_window': 'non-atomic HPM software bracket; A bytes/tasks include startup and final drain',
            'interpretation': 'within-size ratios of median times; n=5, no significance or pure-translation claim',
            'shared_boundary': 'shared AR/AW matched to PATH0 leave; private difference is signed diagnostic only'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--records', required=True, type=Path)
    p.add_argument('--manifest', required=True, type=Path)
    p.add_argument('--backend-evidence', required=True, type=Path)
    p.add_argument('--output', required=True, type=Path)
    a = p.parse_args()
    manifest = json.loads(a.manifest.read_text())
    digest = hashlib.sha256(a.backend_evidence.read_bytes()).hexdigest()
    require(manifest['sources']['backend']['sha256'] == digest, 'P0 backend evidence identity changed')
    result = summarize(json.loads(a.records.read_text()), manifest, backend_evidence(a.backend_evidence))
    result['sources'] = {name: {'path': str(path.resolve()), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}
                         for name, path in [('records', a.records), ('manifest', a.manifest),
                                            ('backend', a.backend_evidence)]}
    with a.output.open('x') as f:
        json.dump(result, f, indent=2); f.write('\n')


if __name__ == '__main__':
    main()
