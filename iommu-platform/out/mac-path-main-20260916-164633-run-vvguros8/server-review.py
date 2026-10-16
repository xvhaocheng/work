#!/usr/bin/env python3
"""Independent review of returned v3 records against embedded UART; no board run."""
from pathlib import Path
import collections
import hashlib
import json
import math
import re
import statistics as st

ROOT = Path(__file__).resolve().parents[2]
RUN = '20260916-164633-run-vvguros8'
report = {'scope': 'Read returned records and UART; no source change, build or board action', 'runs': {}}
FIELDS = ('valid', 'entered', 'full', 'carry', 'sum', 'max', 'start', 'end', 'area')

def nums(line):
    return {k: int(v, 16) if v.startswith('0x') else int(v)
            for k, v in re.findall(r'(\w+)=(0x[0-9a-fA-F]+|\d+)', line)}

def summary(values):
    return {'median': st.median(values), 'min': min(values), 'max': max(values)}

def mode(item):
    r = item['record']
    return 'Solo' if r['result']['aggressor'] is None else 'A' + str(r['input']['aggressor_pages'])

for phase, count in [('short', 3), ('main', 30)]:
    folder = ROOT / 'out' / f'mac-path-{phase}-{RUN}'
    records = json.loads((folder / 'records.json').read_text())
    returned = json.loads((folder / 'summary.json').read_text())
    serial = returned['raw_serial']
    block = re.search(r'^PATH_' + phase.upper() + r'_BEGIN\s*$(.*?)^PATH_' +
                      phase.upper() + r'_EXIT=0\s*$', serial, re.M | re.S).group(1)
    raw_records = [json.loads(line) for line in block.splitlines()
                   if line.startswith('{"schema":"idma.batch.v1"')]
    assert len(records) == len(raw_records) == count
    assert raw_records == [x['record'] for x in records]
    assert re.findall(r'^BATCH_CASE_EXIT=(\d+)\s*$', block, re.M) == ['0'] * count
    assert re.search(r'^PATH_TAINT_BEFORE=0\s*$', block, re.M)
    assert re.search(r'^PATH_TAINT_AFTER=0\s*$', block, re.M)
    assert '6.5.0-rc2-00083-g45bf76b15e36' in serial
    assert re.search(r'^FINAL_PATH=Y\s*$', serial, re.M)
    obs, paths = collections.defaultdict(dict), collections.defaultdict(dict)
    for seq, part, line in re.findall(r'idma_batch_obs seq=(\d+) part=(\w+) ([^\r\n]+)', serial):
        values = nums(line)
        prior = obs[int(seq)].get(part)
        assert prior is None or prior == values
        obs[int(seq)][part] = values
    for seq, role, channel, stage, line in re.findall(
            r'idma_path_obs seq=(\d+) role=([va]) channel=([rw]) stage=(\d+) ([^\r\n]+)', serial):
        key = (role, channel, int(stage)); values = nums(line)
        prior = paths[int(seq)].get(key)
        assert prior is None or prior == values
        paths[int(seq)][key] = values
    assert [x['case'] for x in records] == list(range(1, count + 1))
    assert [x['sequence'] for x in records] == (list(range(1, 4)) if phase == 'short' else list(range(4, 34)))
    groups = collections.defaultdict(lambda: collections.defaultdict(list))
    cases = []
    for x in records:
        r=x['record']; i=r['input']; res=r['result']; o=x['observer']; seq=x['sequence']
        t=o['time']; v=o['axi_v']; a=o['axi_a']; n=i['victim_transfers']; label=mode(x)
        assert r['success'] and r['errno'] == res['status'] == 0
        assert res['hpm_configured'] == res['measurement_started'] == res['measurement_complete'] == 1
        assert i['device'] == '/dev/idma0' and i['aggressor_id'] in (None, 1)
        assert i['victim_pages'] == 1 and i['chunk_bytes'] == 4096
        assert res['victim_did'] == 10 and res['aggressor_did'] == (None if label == 'Solo' else 11)
        assert obs[seq] == o and set(o) == {'time', 'axi_v', 'axi_a'}
        assert t['status'] == 4 and t['valid'] == t['batch_polled'] == 1
        assert t['issued'] == t['done'] == n and t['cycles'] == t['h'] + t['gap']
        assert t['gap'] == n - 1
        assert t['v_did'] == 10 and t['a_did'] == (0xffffffff if label == 'Solo' else 11)
        assert sum(v[k] for k in ('neither', 'v_only', 'a_only', 'both')) == t['cycles']
        assert v['ar'] == v['aw'] == v['rlast'] == v['b'] == 2*n and v['start'] == v['end'] == 0
        for q in (v, a):
            assert (q['start'] >> 16) + q['ar'] == q['rlast'] + (q['end'] >> 16)
            assert (q['start'] & 65535) + q['aw'] == q['b'] + (q['end'] & 65535)
        assert a['shared_ar'] == v['ar'] + a['ar'] and a['shared_aw'] == v['aw'] + a['aw']
        assert res['victim']['submitted'] == res['victim']['completed'] == n
        assert res['victim']['completed_bytes'] == n * 4096
        for q in (res['victim'], res['aggressor']):
            if q is not None:
                assert q['submitted'] == q['completed'] and q['data_errors'] == q['quarantined'] == 0
        pmap = {(p['role'],p['channel'],p['stage']):p for p in x['path']}
        expected_keys = {(role,ch,stage) for role in 'va' for ch in 'rw' for stage in range(4)}
        assert set(pmap) == set(paths[seq]) == expected_keys and len(x['path']) == 16
        for (role,ch,stage), p in pmap.items():
            assert {k:p[k] for k in FIELDS} == paths[seq][role,ch,stage]
            assert p['valid'] == 1 and p['start'] + p['entered'] == p['full'] + p['carry'] + p['end']
            assert p['carry'] <= p['start'] and p['full'] <= p['entered']
            assert p['max'] <= p['sum'] <= p['full'] * p['max']
            assert p['area'] >= p['sum']
            if stage < 3:
                assert p['full'] + p['carry'] == pmap[role,ch,stage+1]['entered']
            mean = p['sum']/p['full'] if p['full'] else None
            assert mean == p['mean_cycles']
            if role == 'v':
                assert p['full'] == p['entered'] == 2*n
                assert p['carry'] == p['start'] == p['end'] == 0 and p['area'] == p['sum']
            elif label == 'Solo':
                assert all(p[k] == 0 for k in FIELDS if k != 'valid')
            prefix = f'{role}_{ch}_{stage}'
            for field in ('full','carry','start','end','sum','max','area'):
                groups[label][prefix+'_'+field].append(p[field])
            if mean is not None:
                groups[label][prefix+'_mean_cycles'].append(mean)
        if label == 'Solo':
            assert v['both'] == v['a_only'] == a['ar'] == a['aw'] == 0
        else:
            assert v['both'] > 0 and a['ar'] > 0 and a['aw'] > 0
        assert math.isclose(x['overlap_fraction'], v['both']/t['cycles'], rel_tol=1e-12, abs_tol=1e-12)
        metrics={'hardware_ms':t['cycles']/50000, 'hardware_MiB_s':n*4096/1048576/(t['cycles']/50e6),
                 'software_ms':res['victim_elapsed_ns']/1e6,'done_to_read_ms':t['done_to_read']/50000,
                 'gap_cycles':t['gap'],'overlap_percent':100*v['both']/t['cycles'],
                 'a_ar':a['ar'],'a_aw':a['aw'],'a_completed_including_tail':res['aggressor']['completed'] if res['aggressor'] else 0}
        for role,name in [('v','victim'),('a','aggressor')]:
            for event in ('iotlb_miss','s1_ptw','s2_ptw'):
                metrics[role+'_'+event]=res[name][event] if res[name] else 0
        for k,value in metrics.items(): groups[label][k].append(value)
        cases.append({'case':x['case'],'sequence':seq,'mode':label,**metrics})
        if phase == 'main':
            assert n == 4096 and i['warmup_transfers'] == 256 and i['run_timeout_ms'] == 60000
        else:
            assert n == (8 if x['case'] < 3 else 64) and i['warmup_transfers'] == 0
    if phase == 'main':
        for start in range(0,30,3): assert {mode(x) for x in records[start:start+3]} == {'Solo','A1','A8'}
        assert all(len(m['hardware_ms']) == 10 for m in groups.values())
    stats={label:{metric:summary(values) for metric,values in metrics.items()} for label,metrics in groups.items()}
    report['runs'][phase]={'passed':True,'cases':count,'raw_json_batch_and_16_path_match':True,
        'exit_and_taint_zero':True,'source_sha256':{name:hashlib.sha256((folder/name).read_bytes()).hexdigest()
            for name in ('records.json','summary.json','assessment.json')},'groups':stats,'per_case':cases,
        'kernel_hrtimer_messages':sorted(set(re.findall(r'^\[\s*[\d.]+\] hrtimer:.*$',serial,re.M))),
        'artifact_identity_limits':{'download_hash_verified':returned['run']['bit']['hash_verified'],
                                    'sd_readback_verified':returned['run']['sd_readback_verified']}}
current=report['runs']['main']['groups']
old=json.loads((ROOT/'out/mac-batch-ds50-20260916-135544-run-eawziww2/server-review.json').read_text())['runs']['ds50']
for name,digest in old['source_sha256'].items():
    assert hashlib.sha256((ROOT/'out'/old['directory']/name).read_bytes()).hexdigest() == digest
old={g['mode']:g['median'] for g in old['groups']}
report['throughput_change_vs_previous_ds50_percent']={label:(current[label]['hardware_MiB_s']['median']/old[label]['hardware_MiB_s']-1)*100 for label in current}
report['A8_vs_A1']={'extra_hardware_ms':current['A8']['hardware_ms']['median']-current['A1']['hardware_ms']['median'],
    'extra_hardware_percent':100*(current['A8']['hardware_ms']['median']/current['A1']['hardware_ms']['median']-1),
    'v_stage_mean_delta_cycles':{f'{ch}_{stage}':current['A8'][f'v_{ch}_{stage}_mean_cycles']['median']-
                               current['A1'][f'v_{ch}_{stage}_mean_cycles']['median'] for ch in 'rw' for stage in range(4)}}
report['limits']=['Per-stage cumulative times overlap across requests/channels; do not sum as batch elapsed or causal shares.',
    'HPM uses non-atomic software windows. V miss=0 is not a direct per-request hit trace.',
    'Historical performance comparison is not a randomized observer-on/off equivalence test.',
    'Stage 1 includes queue/service/issue; write stage 3 includes W supply and response; neither identifies pure PTW/DDR cost.',
    'No new compilation, board intervention, or complete HPM/timing/CDC/IO signoff.']
output=Path(__file__).with_suffix('.json')
output.write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps({'passed':True,'cases':33,'observer_rows':33*19,'evidence':str(output),
                  'A8_vs_A1':report['A8_vs_A1'],'throughput_change_percent':report['throughput_change_vs_previous_ds50_percent']},ensure_ascii=False,indent=2))
