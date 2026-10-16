import csv, json
from collections import defaultdict

METRICS = 'iommu-platform/out/paper-council-20261005/metrics.csv'
RECORDS = 'iommu-platform/out/paper-council-20261005/records-full.jsonl'

NUM_COLS = [
    'a_pages','v_gap_cycles','transfers','warmup','v_workload_bytes',
    'v_window_cycles','a_completed_jobs_window','v_actual_gap_cycles',
    'raw_v_lookup','raw_v_hit','raw_v_miss','raw_a_lookup','raw_a_hit',
    'raw_a_miss','raw_v_fast','raw_a_fast','raw_v_fill','raw_a_fill',
    'raw_replace_pscid','raw_inval_cycles'
]

rows = []
with open(METRICS, newline='') as f:
    for r in csv.DictReader(f):
        for c in NUM_COLS:
            if r.get(c) not in (None, ''):
                r[c] = int(r[c])
        r['v_nongap_cycles'] = r['v_window_cycles'] - r['v_actual_gap_cycles']
        r['a_jobs_per_cycle'] = r['a_completed_jobs_window'] / r['v_window_cycles']
        r['a_MBps'] = r['a_completed_jobs_window'] * 4096 / (r['v_window_cycles'] * 20e-9) / 1e6
        r['v_total_reqs'] = r['raw_v_lookup'] + r['raw_v_fast']
        r['v_fast_fraction'] = r['raw_v_fast'] / r['v_total_reqs'] if r['v_total_reqs'] else None
        rows.append(r)

by_id = {r['id']: r for r in rows}

def show(ids):
    print('id,mode,a_pages,v_gap,v_window,a_window,a_jobs_per_cycle,v_nongap,raw_v_lookup,raw_v_fast,raw_v_miss,raw_v_fill,raw_a_lookup,raw_a_miss')
    for i in ids:
        r = by_id[i]
        print(','.join(str(x) for x in [
            r['id'], r['mode'], r['a_pages'], r['v_gap_cycles'],
            r['v_window_cycles'], r['a_completed_jobs_window'],
            round(r['a_jobs_per_cycle'], 9), r['v_nongap_cycles'],
            r['raw_v_lookup'], r['raw_v_fast'], r['raw_v_miss'], r['raw_v_fill'],
            r['raw_a_lookup'], r['raw_a_miss']
        ]))

# PN A8 g0 vs g1000，3 blocks
show([
    'explore-b1-g0-a8-mpn', 'explore-b1-g1000-a8-mpn',
    'explore-b2-g0-a8-mpn', 'explore-b2-g1000-a8-mpn',
    'explore-b3-g0-a8-mpn', 'explore-b3-g1000-a8-mpn',
])

# N vs PN A8 g1000
show([
    'explore-b1-g1000-a8-mn', 'explore-b1-g1000-a8-mpn',
    'explore-b2-g1000-a8-mn', 'explore-b2-g1000-a8-mpn',
    'explore-b3-g1000-a8-mn', 'explore-b3-g1000-a8-mpn',
])

# A8 g4000 四模式
show([
    'explore-b1-g4000-a8-ms', 'explore-b1-g4000-a8-mn',
    'explore-b1-g4000-a8-mp', 'explore-b1-g4000-a8-mpn',
])

# 从 records-full 校验 window[3] 与 whole-round A completed
def check_records():
    print('id,window3,agg_window_match,agg_completed,agg_submitted,agg_inflight_end')
    for line in open(RECORDS):
        rec = json.loads(line)
        case = rec['case']
        result = rec['record']['result']
        p4m = result.get('p4m') or {}
        win = p4m.get('window') or []
        exp = result.get('experiment') or {}
        agg = exp.get('aggressor')
        if agg is None:
            continue
        window3 = win[3] if len(win) > 3 else None
        print(','.join(str(x) for x in [
            case['id'], window3, window3 == by_id[case['id']]['a_completed_jobs_window'],
            agg.get('completed'), agg.get('submitted'), agg.get('in_flight_at_end')
        ]))

check_records()
