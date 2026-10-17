#!/usr/bin/env python3
"""Offline analysis of validated pathv3 records; never opens the board.

ABI 1 batch has one shared, 4096-byte chunk. Reject invented small-chunk
records instead of interpreting them as E3 measurements. A new ABI needs an
explicit adapter after the change request is approved and implemented.
"""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import statistics

DS0_BIT = 'e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6'
CLOCK_HZ = 50_000_000


def require(value, message):
    if not value:
        raise ValueError(message)


def summarize(records, manifest, repeats=None, require_subset=False):
    require(manifest['status'] == 'passed', 'source collection did not pass')
    require(manifest['bit_sha256'] == DS0_BIT, 'source is not the fixed DS0 BIT')
    require(manifest['kernel_release'] == '6.5.0-rc2-ispass2027-pathv3', 'kernel mismatch')
    require(manifest['recorded_cases'] == len(records), 'manifest count mismatch')
    require(bool(records), 'no records')
    groups, seen = defaultdict(list), set()
    for item in records:
        key = item['boot_id'], item['sequence']
        require(key not in seen, 'duplicate boot/observer sequence')
        seen.add(key)
        record = item['record']
        require(record['schema'] == 'idma.batch.v1', 'unsupported measurement schema')
        require(record['success'] and record['errno'] == 0, 'CLI failure')
        p, r = record['input'], record['result']
        require(p['abi_version'] == r['abi_version'] == 1, 'unsupported ABI')
        require(p['chunk_bytes'] == 4096,
                'ABI 1 batch requires 4096 bytes; small-chunk data cannot come from pathv3')
        require(p['flags'] == 0 and not any(p['reserved']), 'unsupported ABI flags')
        require(p['device'] == '/dev/idma0', 'victim device mismatch')
        require(r['status'] == 0 and all(r[k] == 1 for k in
                ('hpm_configured', 'measurement_started', 'measurement_complete')),
                'incomplete measurement')
        n = p['victim_transfers']
        require(n > 0, 'empty task')
        for role in ('victim', 'aggressor'):
            lane = r[role]
            if lane is not None:
                require(lane['data_errors'] == lane['quarantined'] == 0,
                        'data error or quarantine')
                require(lane['submitted'] == lane['completed'], 'undrained lane')
                require(lane['completed_bytes'] == lane['completed'] * p['chunk_bytes'],
                        'byte accounting mismatch')
                require(lane['iotlb_miss'] >= 0, 'invalid miss count')
        v, a = r['victim'], r['aggressor']
        require(v['completed'] == n, 'victim count mismatch')
        require((a is None) == (p['aggressor_id'] is None), 'aggressor presence mismatch')
        if a is not None:
            require(p['aggressor_id'] == 1 and a['completed'] > 0, 'aggressor mismatch')
        t = item['observer']['time']
        require(t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1,
                'invalid hardware window')
        require(t['issued'] == t['done'] == n and t['cycles'] > 0, 'hardware task mismatch')
        require(t['cycles'] == t['h'] + t['gap'] and t['gap'] == n - 1,
                'hardware time accounting mismatch')
        require(t['v_did'] == r['victim_did'] and t['a_did'] ==
                (r['aggressor_did'] if a is not None else 0xffffffff), 'observer DID mismatch')
        condition = 'Solo' if a is None else 'A' + str(p['aggressor_pages'])
        require(item['condition'] == condition, 'condition metadata mismatch')
        require(item['ds'] == '0/0', 'DS metadata mismatch')
        config = (p['chunk_bytes'], p['victim_pages'], n, p['warmup_transfers'])
        groups[config, condition].append({
            'hardware_ms': t['cycles'] / CLOCK_HZ * 1000,
            'V_MiB_s': v['completed_bytes'] / 1048576 / (t['cycles'] / CLOCK_HZ),
            'V_miss': v['iotlb_miss'],
            'A_miss': a['iotlb_miss'] if a is not None else None,
            'boot_id': item['boot_id'],
        })
    if require_subset:
        expected = {(size, condition) for size in (64, 1024, 4096)
                    for condition in ('Solo', 'A1', 'A8')}
        require({(config[0], condition) for config, condition in groups} == expected,
                'requested E3 subset is absent: need 64/1024/4096 x Solo/A1/A8')
        require(all(config[1:] == (1, 4096, 256) for config, _ in groups),
                'subset victim geometry differs from the plan')
        repeats = 5
    if repeats is not None:
        require(all(len(items) == repeats for items in groups.values()), 'repeat count mismatch')
    rows = []
    for (config, condition), items in sorted(groups.items()):
        def median(metric):
            values = [x[metric] for x in items if x[metric] is not None]
            return statistics.median(values) if values else None
        refs = []
        for reference in ('Solo', 'A1'):
            require((config, reference) in groups, 'missing matched ' + reference)
            refs.append(statistics.median(x['hardware_ms'] for x in groups[config, reference]))
        row = dict(zip(('chunk_bytes', 'victim_pages', 'transfers', 'warmup'), config))
        row.update(condition=condition, n=len(items), boot_ids=sorted({x['boot_id'] for x in items}))
        row.update({metric: median(metric) for metric in ('hardware_ms', 'V_MiB_s', 'V_miss', 'A_miss')})
        row.update(hardware_ms_min=min(x['hardware_ms'] for x in items),
                   hardware_ms_max=max(x['hardware_ms'] for x in items),
                   slowdown_vs_Solo_x=row['hardware_ms'] / refs[0],
                   slowdown_vs_A1_x=row['hardware_ms'] / refs[1])
        rows.append(row)
    return {'status': 'offline_analysis_only', 'new_board_measurements': 0,
            'clock_hz': CLOCK_HZ, 'samples': len(records), 'rows': rows,
            'counter_window': 'V/A HPM software brackets, non-atomic; not exact hardware-window misses',
            'slowdown': 'ratio of group median hardware times for equal victim work',
            'scope': 'existing ABI 1 batch records; collector correctness checks remain required'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--records', type=Path, required=True)
    p.add_argument('--manifest', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True, help='new JSON file; never overwritten')
    p.add_argument('--expected-repeats', type=int)
    p.add_argument('--require-subset', action='store_true')
    a = p.parse_args()
    require(a.expected_repeats is None or a.expected_repeats > 0, 'repeats must be positive')
    result = summarize(json.loads(a.records.read_text()), json.loads(a.manifest.read_text()),
                       a.expected_repeats, a.require_subset)
    result['sources'] = [{'path': str(path.resolve()),
                          'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}
                         for path in (a.records, a.manifest)]
    with a.output.open('x') as f:
        json.dump(result, f, indent=2)
        f.write('\n')


if __name__ == '__main__':
    main()
