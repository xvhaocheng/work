#!/usr/bin/env python3
"""Focused actual-entry resume check: self-consistent cached/raw skipped sequence."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
import types
import collect_workload as collect
from workload_records import backend_evidence,parse
HERE=Path(__file__).resolve().parent


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--pilot',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
    old=json.loads((a.pilot/'manifest.json').read_text());rows=json.loads((a.pilot/'records.json').read_text())
    changed=a.output/'changed';changed.mkdir();raw=json.loads(Path(rows[1]['raw_command_path']).read_text())
    raw['stdout']=raw['stdout'].replace('PRE_SEQ=1','PRE_SEQ=2').replace('"observer_sequence": 2','"observer_sequence": 3').replace('seq=2 ','seq=3 ')
    sources=old['sources'];backend=backend_evidence(*(sources[x]['path'] for x in ['p0_backend','long_backend','packet_backend']))
    item=parse(raw['stdout'],old['kernel_release'],backend)
    for k in ['record','observer','path','histogram','sequence','boot_id']:rows[1][k]=item[k]
    target=changed/'raw.json';target.write_text(json.dumps(raw));rows[1]['raw_command_path']=str(target.resolve())
    (changed/'records.json').write_text(json.dumps(rows));(changed/'manifest.json').write_text(json.dumps(old))
    calls=[]
    class Console:
        def __init__(self,*args):calls.append(args);raise AssertionError('UART reached')
    sys.modules['uart']=types.SimpleNamespace(Console=Console,DEFAULT_PORT='/NO-REAL-UART-GAP-FIXTURE')
    sys.argv=['collect_workload.py','--output',str(a.output/'rejected'),'--resume-run',str(changed),'--board-tools',str(a.output)]
    for name,key in [('load-manifest','load'),('fpga-manifest','fpga'),('software-manifest','software'),
                     ('p0-backend','p0_backend'),('long-backend','long_backend'),('packet-backend','packet_backend')]:
        sys.argv+=['--'+name,sources[key]['path']]
    try:collect.main(_test_fixture=True)
    except ValueError as e:
        assert 'observer sequence gap' in str(e),str(e)
        rejection=str(e)
    else:raise AssertionError('self-consistent skipped sequence accepted')
    assert not calls
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,
            'check':'cached and raw independently valid and equal, but prefix1->3 rejected before UART',
            'rejection':rejection,'collector_sha256':hashlib.sha256((HERE/'collect_workload.py').read_bytes()).hexdigest()}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
