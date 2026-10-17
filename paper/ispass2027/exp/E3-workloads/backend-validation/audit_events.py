#!/usr/bin/env python3
"""Reconstruct byte and burst evidence independently from accepted AXI events."""
import argparse
import collections
import csv
import hashlib
import io
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[4]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--summary', type=Path, required=True)
    args = parser.parse_args()
    data = args.summary.read_bytes()
    summary = json.loads(data)
    assert summary['status']=='passed' and summary['case_count']==13
    raw = ROOT/summary['raw_event_log']
    raw_bytes = raw.read_bytes()
    events = [json.loads(line) for line in raw_bytes.decode().splitlines() if line.startswith('{')]
    grouped = collections.defaultdict(list)
    for event in events:
        if 'case' in event:
            grouped[event['case']].append(event)
    rows = []
    for case in summary['actual_backend_cases']:
        observed = grouped[case['case']]
        counts = collections.Counter(event['event'] for event in observed)
        assert counts['descriptor']==case['submitted']==counts['response']==case['completed']
        assert counts['AR']==case['ar']==sum(e['last'] for e in observed if e['event']=='R')
        assert counts['AW']==case['aw']==counts['B']
        read_bytes = 8*counts['R']
        write_bytes = sum(e['strb'].bit_count() for e in observed if e['event']=='W')
        assert read_bytes==case['read_bytes'] and write_bytes==case['write_bytes']
        reads, writes = collections.defaultdict(dict), collections.defaultdict(dict)
        for event in observed:
            if event['event'] not in {'R','W'}:
                continue
            index = event['descriptor']
            descriptor = case['descriptors'][index]
            write = event['event']=='W'
            base = descriptor['dst' if write else 'src']
            target = writes[index] if write else reads[index]
            for lane in range(8):
                if write and not (event['strb'] & (1<<lane)):
                    continue
                offset = event['address']+lane-base
                assert 0<=offset<descriptor['length']
                assert offset not in target, 'byte transferred twice'
                target[offset] = (event['data']>>(8*lane))&255
        for index, descriptor in enumerate(case['descriptors']):
            assert len(reads[index])==len(writes[index])==descriptor['length']
            assert reads[index]==writes[index], 'actual accepted R and W byte contents differ'
            ar = [e for e in observed if e['event']=='AR' and e['descriptor']==index]
            aw = [e for e in observed if e['event']=='AW' and e['descriptor']==index]
            assert len(ar)==descriptor['ar'] and len(aw)==descriptor['aw']
            assert sum((e['len']+1)*(1<<e['size']) for e in ar)==descriptor['read_bytes']
            assert sum((e['len']+1)*(1<<e['size']) for e in aw)==descriptor['write_bytes']
        rows.append({'case':case['case'],'descriptors':counts['descriptor'],'responses':counts['response'],
                     'ar':counts['AR'],'aw':counts['AW'],'rlast':case['rlast'],'b':counts['B'],
                     'read_bytes':read_bytes,'write_bytes':write_bytes,
                     'r_slverr':sum(bool(e['resp']) for e in observed if e['event']=='R'),
                     'b_slverr':sum(bool(e['resp']) for e in observed if e['event']=='B')})
    receipt = {'status':'passed','scope':'independent reconstruction from actual accepted AXI event bytes; no DUT rerun',
               'cases':len(rows),'accepted_descriptors':sum(row['descriptors'] for row in rows),
               'raw_events_sha256':hashlib.sha256(raw_bytes).hexdigest(),
               'summary_sha256':hashlib.sha256(data).hexdigest(),
               'source_summary':str(args.summary.resolve()),'case_counts':rows,
               'zero_length_probe':'excluded from legal-domain acceptance; preserved failed run02 remains linked in source summary'}
    destination = args.summary.parent
    (destination/'event-audit.json').write_text(json.dumps(receipt,indent=2,sort_keys=True)+'\n')
    output = io.StringIO(newline='')
    writer = csv.DictWriter(output,list(rows[0]),lineterminator='\n')
    writer.writeheader(); writer.writerows(rows)
    (destination/'case-counts.csv').write_text(output.getvalue())
    print(json.dumps({'status':'passed','cases':len(rows),'accepted_descriptors':receipt['accepted_descriptors']}))


if __name__=='__main__':
    main()
