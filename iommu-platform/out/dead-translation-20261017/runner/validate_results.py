#!/usr/bin/env python3
"""Independently integrate raw occupancy/victim traces and check final counters."""
import csv, gzip, hashlib, json
from pathlib import Path
TASK=Path(__file__).resolve().parent.parent
ROOT=TASK/'runs/final-28'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rows(path):
    packed=path.with_suffix(path.suffix+'.gz')
    actual=packed if packed.exists() else path
    opener=gzip.open if actual==packed else open
    with opener(actual,'rt',newline='') as f:yield from csv.DictReader(f)
checks=[]
for p in sorted(ROOT.glob('*/summary.json')):
    d=json.loads(p.read_text());m=d['metrics'];c=d['config'];assert d['checks']['passed'] and d['failure'] is None
    start=d['timing']['t0'];stop=start+m['dead_sample_cycles']
    assert m['dead_sample_cycles']==max(m['A_final_flush_end'],m['T_B']+1)
    previous=start;mask=0;valid=0 if c['b_jobs']==0 else (1<<c['b_pages'])-1
    dead_sum=valid_sum=peak=dead_victims=live_victims=empty=0
    for r in rows(p.parent/'dead-trace.csv'):
        t=int(r['cycle'])
        if t<start:continue
        if t>=stop:break
        dead_sum+=mask.bit_count()*(t-previous);valid_sum+=valid.bit_count()*(t-previous)
        previous=t;mask=int(r['dead_mask']);valid=int(r['valid_mask']);peak=max(peak,mask.bit_count())
        assert mask&~valid==0
        if r['event']=='replacement':
            v=int(r['victim_way']);assert int(r['victim_dead'])==((mask>>v)&1)
            assert int(r['victim_valid'])==((valid>>v)&1)
            if not int(r['victim_valid']):empty+=1
            elif int(r['victim_dead']):dead_victims+=1
            else:live_victims+=1
    dead_sum+=mask.bit_count()*(stop-previous);valid_sum+=valid.bit_count()*(stop-previous)
    assert dead_sum==m['dead_slot_cycles'],(p,'dead integral',dead_sum,m['dead_slot_cycles'])
    assert valid_sum==m['dead_valid_cycles'],(p,'valid integral',valid_sum,m['dead_valid_cycles'])
    assert peak==m['dead_peak'] and dead_victims==m['dead_replacements'] and live_victims==m['live_replacements'] and empty==m['empty_fills']
    assert dead_victims+live_victims+empty==m['A_walks']+m['B_walks']
    assert sum(m[k] for k in ['A_wall_host','A_wall_flush','A_wall_copy_pte','A_wall_copy_payload_wait','A_wall_copy_other'])==m['A_final_flush_end']
    assert m['comp_read_requests']==m['payload_read_completed'] and m['comp_write_requests']==m['payload_write_completed']
    latency=list(rows(p.parent/'command-latency.csv'))
    assert len(latency)==m['command_latency_count']==m['cq_iotinval']
    for r in latency:
        assert int(r['first_fence_complete'])-int(r['submit'])==int(r['submit_to_fence'])
        assert sum(int(r[k]) for k in ['cq_fetch_register','cq_decode','cq_fence','other'])==int(r['submit_to_fence'])
    checks.append({'case':c['case_id'],'summary_sha256':sha(p),'occupancy_cycle_integral':True,'victim_flags':True,'wall_partition':True,'latency_partition':True,'cohort_conservation':True})
assert len(checks)==28
out=TASK/'analysis/final/validation.json'
out.write_text(json.dumps({'state':'passed','cases':checks,'driver_sha256':sha(Path(__file__))},indent=2)+'\n')
print('PASS 28/28: independent occupancy integrals, victim flags, wall/command partitions, payload cohorts')
