#!/usr/bin/env python3
"""Recheck returned batch records against UART and compute hardware-window stats."""
import collections
import hashlib
import json
from pathlib import Path
import re
import statistics

ROOT = Path(__file__).resolve().parents[2]
RUNS = {
    'ds50': ('mac-batch-ds50-20260916-135544-run-eawziww2', 30),
}
report = {'scope': 'Returned batch records and embedded UART; no rebuild or new board run', 'runs': {}}
for label, (directory, expected) in RUNS.items():
    folder = ROOT / 'out' / directory
    records = json.loads((folder / 'records.json').read_text())
    serial = json.loads((folder / 'summary.json').read_text())['raw_serial']
    raw_records = [json.loads(line) for line in serial.splitlines()
                   if line.startswith('{"schema":"idma.batch.v1"')]
    assert len(records) == len(raw_records) == expected
    assert raw_records == [r['record'] for r in records]
    assert re.findall(r'(?m)^BATCH_CASE_EXIT=(\d+)\s*$', serial) == ['0'] * expected
    assert re.search(r'(?m)^BATCH_DS50_TAINT_AFTER=0\s*$', serial)
    assert re.search(r'(?m)^BATCH_' + label.upper() + r'_EXIT=0\s*$', serial)
    observations = collections.defaultdict(dict)
    for seq, part, fields in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\r\n]+)', serial):
        values = {k: int(v, 16) if v.startswith('0x') else int(v)
                  for k, v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', fields)}
        old = observations[int(seq)].get(part)
        assert old is None or old == values
        observations[int(seq)][part] = values
    assert len(observations) == expected
    groups = collections.defaultdict(list)
    nonzero_v = []
    for item, seq in zip(records, sorted(observations)):
        r = item['record']; i = r['input']; result = r['result']; o = item['observer']
        t = o['time']; v = o['axi_v']; a = o['axi_a']; n = i['victim_transfers']
        assert observations[seq] == o
        assert r['success'] and r['errno'] == result['status'] == 0
        assert result['measurement_started'] == result['measurement_complete'] == result['hpm_configured'] == 1
        assert result['victim']['submitted'] == result['victim']['completed'] == n
        assert result['victim']['completed_bytes'] == n * i['chunk_bytes']
        for lane in [result['victim'], result['aggressor']]:
            if lane:
                assert lane['data_errors'] == lane['quarantined'] == 0
                assert lane['submitted'] == lane['completed']
        assert t['valid'] == 1 and t['status'] == 4 and t['batch_polled'] == 1
        assert t['issued'] == t['done'] == n and t['cycles'] == t['h'] + t['gap']
        assert t['v_did'] == result['victim_did']
        assert t['a_did'] == (result['aggressor_did'] if result['aggressor'] else 0xffffffff)
        assert sum(v[k] for k in ['neither', 'v_only', 'a_only', 'both']) == t['cycles']
        assert v['ar'] == v['aw'] == v['rlast'] == v['b'] == 2*n and v['end'] == 0
        for lane in [v, a]:
            # Production observer packs reads in high 16 bits, writes in low 16 bits.
            assert (lane['start'] >> 16) + lane['ar'] == lane['rlast'] + (lane['end'] >> 16)
            assert (lane['start'] & 65535) + lane['aw'] == lane['b'] + (lane['end'] & 65535)
        assert a['shared_ar'] == v['ar'] + a['ar'] and a['shared_aw'] == v['aw'] + a['aw']
        if result['aggressor']:
            assert a['ar'] > 0 and a['aw'] > 0 and v['both'] > 0
        else:
            assert v['both'] == v['a_only'] == a['ar'] == a['aw'] == 0
        mode = 'Solo' if result['aggressor'] is None else 'A' + str(i['aggressor_pages'])
        groups[(i['device'], mode)].append(item)
        if result['victim']['iotlb_miss']:
            nonzero_v.append({'case': item['case'], 'mode': mode, 'device': i['device'],
                              'miss': result['victim']['iotlb_miss'], 's1_ptw': result['victim']['s1_ptw']})
    stats = []
    if label != 'short':
        assert all(len(g) == 10 for g in groups.values())
        assert all(x['record']['input']['victim_transfers'] == 4096 and
                   x['record']['input']['warmup_transfers'] == 256 and
                   x['record']['input']['victim_pages'] == 1 for x in records)
        for (device, mode), items in sorted(groups.items()):
            metrics = collections.defaultdict(list)
            for x in items:
                t=x['observer']['time']; v=x['observer']['axi_v']; r=x['record']['result']
                metrics['hardware_MiB_s'].append(r['victim']['completed_bytes']/1048576/(t['cycles']/50e6))
                metrics['hardware_ms'].append(t['cycles']/50000)
                metrics['h_ms'].append(t['h']/50000)
                metrics['gap_ms'].append(t['gap']/50000)
                metrics['overlap_percent'].append(v['both']*100/t['cycles'])
                metrics['done_to_read_ms'].append(t['done_to_read']/50000)
                metrics['software_ms'].append(r['victim_elapsed_ns']/1e6)
                for lane in ['victim','aggressor']:
                    for counter in ['iotlb_miss','s1_ptw','s2_ptw']:
                        metrics[lane+'_'+counter].append(r[lane][counter] if r[lane] else 0)
            stats.append({'device':device,'mode':mode,'samples':len(items),
                          'median':{k:statistics.median(v) for k,v in metrics.items()},
                          'min':{k:min(v) for k,v in metrics.items()},
                          'max':{k:max(v) for k,v in metrics.items()}})
    report['runs'][label] = {'directory':directory,'passed':True,'cases':expected,
        'raw_json_and_observer_match':True,'uart_exit_and_taint_zero':True,
        'source_sha256':{n:hashlib.sha256((folder/n).read_bytes()).hexdigest()
                         for n in ['records.json','summary.json','assessment.json']},
        'groups':stats,'nonzero_v_miss':nonzero_v}
assert '6.5.0-rc2-00082-gb7979a1a06ac' in serial
latest = {g['mode']: g for g in report['runs']['ds50']['groups']}
assert set(latest) == {'Solo', 'A1', 'A8'}
assert all(g['device'] == '/dev/idma0' for g in latest.values())
assert all(r['record']['input']['aggressor_id'] in [None, 1] and
           r['record']['input']['chunk_bytes'] == 4096 for r in records)
baseline_groups = {}
for key, filename, run in [
    ('first_ds50', 'mac-batch-main-20260916-112954-run-9sa69rhc', 'main'),
    ('ds0', 'mac-batch-ds0-20260916-132332-run-ebgxhirr', 'ds0')]:
    previous = json.loads((ROOT/'out'/filename/'server-review.json').read_text())['runs'][run]
    for name, digest in previous['source_sha256'].items():
        assert hashlib.sha256((ROOT/'out'/previous['directory']/name).read_bytes()).hexdigest() == digest
    baseline_groups[key] = {g['mode']: g for g in previous['groups'] if g['device'] == '/dev/idma0'}
report['three_stage_comparison'] = []
for mode in ['Solo', 'A1', 'A8']:
    values = {key: groups[mode]['median'] for key, groups in baseline_groups.items()}
    values['retest_ds50'] = latest[mode]['median']
    report['three_stage_comparison'].append({'mode': mode, **values,
        'retest_vs_first_throughput_change_percent':
            (values['retest_ds50']['hardware_MiB_s']/values['first_ds50']['hardware_MiB_s']-1)*100})
report['A8_vs_A1_extra_hardware_time_percent'] = {
    key: (groups['A8']['median']['hardware_ms']/groups['A1']['median']['hardware_ms']-1)*100
    for key, groups in {**baseline_groups, 'retest_ds50':latest}.items()}
miss_groups = collections.defaultdict(list)
for r in records:
    result = r['record']['result']
    if result['aggressor'] and r['record']['input']['aggressor_pages'] == 8:
        miss_groups[(result['victim']['iotlb_miss'], result['aggressor']['iotlb_miss'])].append(r)
report['retest_A8_miss_patterns'] = [
    {'victim_miss': v, 'aggressor_miss': a, 'cases': [r['case'] for r in items],
     'hardware_ms_median': statistics.median(r['observer']['time']['cycles']/50000 for r in items)}
    for (v,a), items in sorted(miss_groups.items())]
report['limits'] = ['One DS50 -> DS0 -> DS50 sequence; mappings and internal state not fixed across boots.',
    'DS affects multiple IOMMU structure requests; HPM windows are non-atomic; no HOL or pure-PTW-cost proof.',
    'All samples retained, including two retest A8 samples with V miss=2 and A miss=5141.']
output = Path(__file__).with_suffix('.json')
output.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed': True, 'cases': sum(x['cases'] for x in report['runs'].values()),
                  'evidence': str(output)},ensure_ascii=False))
