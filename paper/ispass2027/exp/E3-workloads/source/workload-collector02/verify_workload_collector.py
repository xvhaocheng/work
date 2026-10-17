#!/usr/bin/env python3
"""Actual collector entry through explicit internal synthetic mode and fake UART."""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import shutil
import sys
import types
import collect_workload as collect
import verify_workload_records as fixtures
HERE=Path(__file__).resolve().parent


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--CLI-report',type=Path,required=True)
    p.add_argument('--p0-backend',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    a.output.mkdir(parents=True,exist_ok=False);inputs=a.output/'inputs';inputs.mkdir()
    sources=a.output/'sources';sources.mkdir()
    for name in ['collect_workload.py','workload_records.py','verify_workload_collector.py']:
        shutil.copyfile(HERE/name,sources/name)
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
    state={'next':0,'calls':0,'closed':False,'fail_at':None,'command_at':None,'jump_at':None}
    class Console:
        def __init__(self,port,output):
            assert port=='/NO-REAL-UART-ABI5-FIXTURE';self.output=output;output.mkdir()
        def command(self,text,timeout):
            assert timeout==120;index=state['next'];state['next']+=1;state['calls']+=1
            point=collect.plan()[index];assert text==collect.command(point)
            ap=point['aggressor_pages'] or (collect.model.geometry(point['aggressor_shape'],point['aggressor_size'])['mapped_pages'] if point['aggressor_shape'] else 0)
            key=point['victim_shape'],point['victim_size'],point['aggressor_shape'],point['aggressor_size'],ap
            seq=index+1+(state['jump_at']==index)
            item=fixtures.fixture(templates[key]);item['sequence']=seq;item['record']['result']['observer_sequence']=seq
            raw=fixtures.output(item)
            if state['fail_at']==index:raw=raw.replace('TAINT_AFTER=0','TAINT_AFTER=16384')
            path=self.output/f'command-{index+1:03}.json';value={'stdout':raw,'exit_code':0,'raw_path':str(path.resolve()),
                'command':text+(' --forged' if state['command_at']==index else '')}
            path.write_text(json.dumps(value)+'\n');return value
        def close(self):state['closed']=True
    sys.modules['uart']=types.SimpleNamespace(Console=Console,DEFAULT_PORT='/NO-REAL-UART-ABI5-FIXTURE')
    def run(path,max_cases=150,resume=None,test=True,input_dir=inputs):
        argv=['collect_workload.py','--output',str(path),'--board-tools',str(input_dir),'--load-manifest',str(input_dir/'load.json'),
              '--fpga-manifest',str(input_dir/'fpga.json'),'--software-manifest',str(input_dir/'software.json'),
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
    pilot=a.output/'pilot'
    for field in collect.FROZEN_RESUME_FIELDS:
        changed=a.output/('changed-'+field);changed.mkdir()
        old=json.loads((pilot/'manifest.json').read_text());value=old[field]
        if isinstance(value,bool):old[field]=not value
        elif isinstance(value,int):old[field]=value+1
        elif isinstance(value,list):old[field]=[]
        elif isinstance(value,dict):old[field]={}
        else:old[field]=str(value)+'-changed'
        (changed/'manifest.json').write_text(json.dumps(old));shutil.copyfile(pilot/'records.json',changed/'records.json')
        state.update(next=30,calls=0,closed=False)
        try:run(a.output/('rejected-'+field),resume=changed)
        except ValueError:pass
        else:raise AssertionError('changed resume identity accepted: '+field)
        assert state['calls']==0;checks.append('resume rejects changed '+field+' before UART')
    for kind in ('command','prefix-sequence','cached-record'):
        changed=a.output/('raw-change-'+kind);changed.mkdir()
        old=json.loads((pilot/'manifest.json').read_text());rows=json.loads((pilot/'records.json').read_text())
        if kind=='cached-record':rows[1]['record']['result']['victim']['completed_bytes']+=1
        else:
            raw=json.loads(Path(rows[1]['raw_command_path']).read_text())
            if kind=='command':raw['command']+=' --forged'
            else:raw['stdout']=raw['stdout'].replace('PRE_SEQ=1','PRE_SEQ=2').replace('"observer_sequence": 2','"observer_sequence": 3').replace('seq=2 ','seq=3 ')
            path=changed/'raw.json';path.write_text(json.dumps(raw));rows[1]['raw_command_path']=str(path.resolve())
        (changed/'manifest.json').write_text(json.dumps(old));(changed/'records.json').write_text(json.dumps(rows))
        state.update(next=30,calls=0,closed=False)
        try:run(a.output/('raw-rejected-'+kind),resume=changed)
        except ValueError:pass
        else:raise AssertionError('changed prefix accepted: '+kind)
        assert state['calls']==0;checks.append('resume rejects '+kind+' without UART')
    for kind,index in [('command',0),('live-sequence',1),('resume-live-gap',30)]:
        state.update(next=30 if index==30 else 0,calls=0,closed=False,command_at=0 if kind=='command' else None,
                     jump_at=index if kind!='command' else None)
        target=a.output/('live-rejected-'+kind)
        try:run(target,resume=pilot if index==30 else None)
        except ValueError:pass
        else:raise AssertionError('live tamper accepted: '+kind)
        failed=json.loads((target/'manifest.json').read_text())
        assert failed['status']=='failed' and failed['recorded_cases']==index and state['calls']==(1 if index==30 else index+1) and state['closed']
        checks.append('actual collector rejects '+kind+' and retains failing raw, no retry')
    state.update(next=0,calls=0,closed=False,command_at=None,jump_at=None)
    for name,marker in [('nested-flag',{'metadata':[{'fixture_only':True}]}),
                        ('nested-evidence',{'metadata':{'origin':{'evidence_level':'synthetic_protocol_fixture'}}}),
                        ('fixtures-container',{'metadata':{'fixtures':[{'name':'test-only'}]}})]:
        location=a.output/name;location.mkdir()
        for k in ['load','fpga','software']:
            d=json.loads((inputs/(k+'.json')).read_text());d.pop('fixture_only',None)
            if k=='software':d.update(marker)
            (location/(k+'.json')).write_text(json.dumps(d))
        target=a.output/(name+'-rejected')
        try:run(target,test=False,input_dir=location)
        except ValueError:pass
        else:raise AssertionError('nested synthetic marker accepted: '+name)
        assert state['calls']==0 and not target.exists();checks.append('production entry rejects '+name+' before UART/output')
    report={'status':'passed','fixture_only':True,'new_board_measurements':0,'checks':checks,
            'collector_sha256':hashlib.sha256((HERE/'collect_workload.py').read_bytes()).hexdigest(),
            'parser_sha256':hashlib.sha256((HERE/'workload_records.py').read_bytes()).hexdigest()}
    (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':'passed','checks':len(checks),'output':str(a.output)}))

if __name__=='__main__':main()
