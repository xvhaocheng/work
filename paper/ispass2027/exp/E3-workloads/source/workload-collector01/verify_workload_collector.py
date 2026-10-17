#!/usr/bin/env python3
"""Actual collector entry through explicit internal synthetic mode and fake UART."""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import sys
import types
import collect_workload as collect
import verify_workload_records as fixtures
HERE=Path(__file__).resolve().parent


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--CLI-report',type=Path,required=True)
    p.add_argument('--p0-backend',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);inputs=a.output/'inputs';inputs.mkdir()
    cli=json.loads(a.CLI_report.read_text());templates={}
    for x in cli['cases']:
        if x['expected_success'] and not x['record']['input']['caps_only']:
            i=x['record']['input'];key=i['victim_shape'],i['victim_size'],i['aggressor_shape'],i['aggressor_size'],i['aggressor_geometry']['ring_pages']
            templates[key]=x['record']
    fpga=json.loads((HERE/'artifacts/full-e3-ds0-build01/build-manifest.json').read_text())
    fpga.update(status='passed',fixture_only=True,artifacts={'fixture.bit':'fixture-bit-only'})
    software=json.loads((HERE/'artifacts/full-e3-software01/build-manifest.json').read_text());software['fixture_only']=True
    loaded={'status':'boot_identity_verified','fixture_only':True,'kernel_release':collect.RELEASE,'boot_id':fixtures.BOOT,'bit_sha256':'fixture-bit-only'}
    for name,d in [('fpga',fpga),('software',software),('load',loaded)]:
        (inputs/(name+'.json')).write_text(json.dumps(d)+'\n')
    fixtures.KERNEL=collect.RELEASE
    state={'next':0,'calls':0,'closed':False,'fail_at':None}
    class Console:
        def __init__(self,port,output):
            assert port=='/NO-REAL-UART-ABI5-FIXTURE';self.output=output;output.mkdir()
        def command(self,text,timeout):
            assert timeout==120;index=state['next'];state['next']+=1;state['calls']+=1
            point=collect.plan()[index];assert text==collect.command(point)
            ap=point['aggressor_pages'] or (collect.model.geometry(point['aggressor_shape'],point['aggressor_size'])['mapped_pages'] if point['aggressor_shape'] else 0)
            key=point['victim_shape'],point['victim_size'],point['aggressor_shape'],point['aggressor_size'],ap
            item=fixtures.fixture(templates[key]);item['sequence']=index+1;item['record']['result']['observer_sequence']=index+1
            raw=fixtures.output(item)
            if state['fail_at']==index:raw=raw.replace('TAINT_AFTER=0','TAINT_AFTER=16384')
            path=self.output/f'command-{index+1:03}.json';value={'stdout':raw,'exit_code':0,'raw_path':str(path.resolve())}
            path.write_text(json.dumps(value)+'\n');return value
        def close(self):state['closed']=True
    sys.modules['uart']=types.SimpleNamespace(Console=Console,DEFAULT_PORT='/NO-REAL-UART-ABI5-FIXTURE')
    def run(path,max_cases=150,resume=None,test=True):
        argv=['collect_workload.py','--output',str(path),'--board-tools',str(inputs),'--load-manifest',str(inputs/'load.json'),
              '--fpga-manifest',str(inputs/'fpga.json'),'--software-manifest',str(inputs/'software.json'),
              '--p0-backend',str(a.p0_backend),'--long-backend',str(HERE/'test-results/long-backend04-summary.json'),
              '--packet-backend',str(HERE/'test-results/packet-backend01-summary.json'),'--max-cases',str(max_cases)]
        if resume:argv+=['--resume-run',str(resume)]
        sys.argv=argv;collect.main(_test_fixture=test)
    checks=[]
    run(a.output/'pilot',30)
    m=json.loads((a.output/'pilot/manifest.json').read_text());assert m['status']=='partial_limit' and m['recorded_cases']==30
    assert m['fixture_only'] and m['evidence_level']=='synthetic_protocol_fixture' and state['calls']==30 and state['closed']
    hashes={p:hashlib.sha256(p.read_bytes()).hexdigest() for p in (a.output/'pilot').rglob('*') if p.is_file()}
    checks.append('30-point full candidate block, explicitly synthetic partial output')
    state.update(next=30,calls=0,closed=False)
    run(a.output/'continued',resume=a.output/'pilot')
    m=json.loads((a.output/'continued/manifest.json').read_text());rows=json.loads((a.output/'continued/records.json').read_text())
    assert m['status']=='passed' and m['recorded_cases']==150 and m['recovered_cases']==30 and state['calls']==120 and state['closed']
    assert len(rows)==150 and all(x['fixture_only'] and x['evidence_level']=='synthetic_protocol_fixture' for x in rows)
    assert set(Counter((x['family'],x['victim_shape'],x['victim_size'],x['condition']) for x in rows).values())=={5}
    assert all(hashlib.sha256(p.read_bytes()).hexdigest()==h for p,h in hashes.items())
    checks.append('resume reads immutable successful prefix, executes only120 remaining, 30cells balanced n5')
    state.update(next=0,calls=0,closed=False,fail_at=2)
    try:run(a.output/'taint-failure')
    except ValueError:pass
    else:raise AssertionError('taint accepted')
    m=json.loads((a.output/'taint-failure/manifest.json').read_text());assert m['status']=='failed' and m['recorded_cases']==2 and state['calls']==3 and state['closed']
    checks.append('taint16384 causes first-error stop, successful2 prefix and failing raw retained, no retry')
    state.update(next=0,calls=0,closed=False,fail_at=None)
    try:run(a.output/'failed-resume',resume=a.output/'taint-failure')
    except ValueError:pass
    else:raise AssertionError('failed run resumed')
    assert state['calls']==0;checks.append('failed source/boot run cannot silently resume or replace bad point')
    try:run(a.output/'publication-rejected',test=False)
    except ValueError:pass
    else:raise AssertionError('fixture accepted by production CLI')
    assert not (a.output/'publication-rejected').exists() and state['calls']==0
    checks.append('default production entry rejects bound fixtures before UART/output creation')
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,'checks':checks,
            'collector_sha256':hashlib.sha256((HERE/'collect_workload.py').read_bytes()).hexdigest(),
            'parser_sha256':hashlib.sha256((HERE/'workload_records.py').read_bytes()).hexdigest()}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'output':str(a.output)}))

if __name__=='__main__':main()
