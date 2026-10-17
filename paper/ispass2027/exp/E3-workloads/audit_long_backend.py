#!/usr/bin/env python3
"""Independently recount recorded AXI handshakes and per-cycle burst inventory."""
import argparse
from collections import Counter,defaultdict
import hashlib
import json
from pathlib import Path


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--run',required=True,type=Path);p.add_argument('--output',required=True,type=Path);a=p.parse_args()
    raw=a.run/'events.raw.jsonl';manifest=a.run/'manifest.json'
    m=json.loads(manifest.read_text());assert m['status']=='passed'
    cases=defaultdict(list)
    for number,line in enumerate(raw.read_text().splitlines(),1):
        if line.startswith('{'):
            x=json.loads(line)
            if 'case' in x:cases[x['case']].append(dict(x,raw_line=number))
    checks=[]
    for name,events in cases.items():
        stock=[0,0];peak=[0,0];counts=Counter();readbytes=writebytes=0
        arbytes=awbytes=0;rlast=0;bycycle=defaultdict(list)
        for x in events:
            if 'cycle' in x:bycycle[x['cycle']].append(x)
        for cycle,xs in sorted(bycycle.items()):
            for x in xs:
                e=x['event'];counts[e]+=1
                if e=='AR':stock[0]+=1;arbytes+=(x['len']+1)*(1<<x['size'])
                if e=='AW':stock[1]+=1;awbytes+=(x['len']+1)*(1<<x['size'])
                if e=='R':
                    readbytes+=8
                    if x['last']:stock[0]-=1;rlast+=1
                if e=='B':stock[1]-=1
                if e=='W':writebytes+=x['strb'].bit_count()
            assert min(stock)>=0
            peak=[max(x,y) for x,y in zip(peak,stock)]
        summary=next(x for x in events if x['event']=='case_summary')
        capacity=next(x for x in events if x['event']=='path_capacity')
        assert stock==[0,0] and counts['AR']==rlast==summary['ar']==summary['rlast']
        assert counts['AW']==counts['B']==summary['aw']==summary['b']
        assert arbytes==readbytes==summary['read_bytes']
        assert awbytes==writebytes==summary['write_bytes']
        assert peak==[capacity['peak_read_bursts'],capacity['peak_write_bursts']]
        assert capacity['depth16_read_full']==counts['AR'] and capacity['depth16_write_full']==counts['AW']
        assert not capacity['depth16_read_error'] and not capacity['depth16_write_error']
        assert capacity['depth4_read_error']==int(peak[0]>4) and capacity['depth4_write_error']==int(peak[1]>4)
        checks.append({'case':name,'peak_read':peak[0],'peak_write':peak[1],'AR':counts['AR'],'AW':counts['AW'],
                       'read_bytes':readbytes,'write_bytes':writebytes,'final_stock':stock,
                       'source_capacity_line':capacity['raw_line'],'source_summary_line':summary['raw_line']})
    assert len(checks)==4
    digest=lambda x:hashlib.sha256(x.read_bytes()).hexdigest()
    value={'status':'passed','scope':'raw accepted AXI beats and per-cycle inventory recount; not full IOMMU proof',
           'new_board_measurements':0,'script_sha256':digest(Path(__file__)),'checks':checks,
           'sources':{str(x.resolve()):digest(x) for x in (raw,manifest)}}
    with a.output.open('x') as stream:json.dump(value,stream,indent=2);stream.write('\n')
    print(json.dumps({'status':'passed','cases':len(checks),'output':str(a.output.resolve())}))


if __name__=='__main__':main()
