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
    'short': ('mac-batch-short-20260916-111626-run-n8mg7wsz', 14),
    'main': ('mac-batch-main-20260916-112954-run-9sa69rhc', 60),
    'ext': ('mac-batch-ext-20260916-114036-run-8pjv_djc', 30),
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
    assert re.search(r'(?m)^BATCH_TAINT_AFTER=0\s*$', serial)
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
output = Path(__file__).with_suffix('.json')
output.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed': True, 'cases': sum(x['cases'] for x in report['runs'].values()),
                  'evidence': str(output)},ensure_ascii=False))
