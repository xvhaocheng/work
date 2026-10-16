#!/usr/bin/env python3
"""Read-only independent accounting of MQ-BOUNDARY-1 captured records; no simulation."""
from pathlib import Path
import json,hashlib,copy,datetime,re,xml.etree.ElementTree as ET
R=Path(__file__).resolve().parent.parent
B=R/'offline'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def check_rows(rows,n):
    gen=served=pending=starts=ends=0;closed=False;keys=set();boundaries=[];last=-1;exit_state=None;exit_event=None
    for line,e in enumerate(rows,1):
        t=e['time_ns'];assert t>=last,('time order',line);last=t
        assert not e.get('observer_invalid',False),('native invalid',line)
        kind=e['event']
        if kind=='trace_generation_event':
            gen+=1;assert e['generated']==gen
        elif kind=='host_cqe_consume_returned':
            served+=1;assert e['serviced']==served
        elif kind=='generation_done':
            assert gen==n and not closed;closed=True
        elif kind=='pcie_deliver':pending+=1
        elif kind=='pcie_consumed_after_followup_schedule':
            pending-=1;assert pending>=0;assert pending==e['toward_host']+e['toward_device']
        elif kind in ('gc_start','gc_end'):
            key=tuple(e[x] for x in ('channel','chip','die','plane','block'))
            if kind=='gc_start':assert key not in keys;keys.add(key);starts+=1
            else:assert key in keys;keys.remove(key);ends+=1
        elif kind=='host_boundary_after_complete_event':
            assert closed and gen==served==n and pending==0
            assert all(e[x]==0 for x in ['waiting','active','toward_host','toward_device'])
            boundaries.append({'line':line,**e})
        elif kind=='engine_exit_state':exit_state=e
        elif kind=='engine_exit':exit_event=e
        else:assert kind=='host_payload_arrived_no_data_model',kind
        if 'pcie_pending' in e:assert e['pcie_pending']==pending,('link accounting',line)
        if 'generated' in e:assert e['generated']==gen and e['serviced']==served and e['target']==(n if gen or served else 0),('flow accounting',line)
        if 'generation_done' in e:assert e['generation_done']==closed
        if 'gc_ongoing_protected_operations' in e:assert e['gc_ongoing_protected_operations']==len(keys),('gc accounting',line)
        if 'gc_starts' in e:assert e['gc_starts']==starts and e['gc_ends']==ends
    assert gen==n and closed and exit_state and exit_event
    assert exit_event['event_count_remaining']==0 and not exit_event['explicit_stop']
    assert pending==0 and not keys and starts==ends
    assert len(boundaries)==(1 if served==n else 0)
    assert exit_state['active']==n-served and exit_state['waiting']==0
    return {'generated':gen,'host_cqes':served,'missing':n-served,'gc_operations_started':starts,'gc_operations_finished':ends,'pcie_messages_delivered':sum(e['event']=='pcie_deliver' for e in rows),'record_count':len(rows),'boundaries':boundaries,'last_time_ns':last,'engine_empty':True,'explicit_stop':False,'gc_operations_at_exit':0,'completion_gate':'PASS' if served==n else 'FAIL_INCOMPLETE_HOST_COMMANDS','residual_gc_witness':bool(boundaries and boundaries[0]['gc_ongoing_protected_operations']>0)}
def main():
    ledger=json.loads((B/'runs/execution-summary.json').read_text());assert ledger['executed_runs']==8 and not ledger['stopped_early']
    cases=[];allrows={};hashed=0
    for run in ledger['runs']:
        d=B/'runs'/str(run['length'])/run['variant'];pre=json.loads((d/'pre-run-manifest.json').read_text())
        assert digest(d/'pre-run-manifest.json')==run['pre_run_manifest_sha256']
        for z in [pre['binary'],pre['contract'],pre['runner'],pre['config'],pre['workload_xml'],pre['trace'],*pre['sources'],*run['output_xml'],run['stdout'],run['stderr']]:
            assert digest(Path(z['path']))==z['sha256'],z['path'];hashed+=1
        assert run['execution_ok'] and run['exit_code']==0
    for n in [512,2048,4096,8192]:
        d=B/'runs'/str(n);p=d/'instrumented/stderr.log';rows=[json.loads(s) for s in p.read_text().splitlines()];allrows[n]=rows
        result=check_rows(rows,n)
        a=next((d/'original').glob('*scenario*.xml'));b=next((d/'instrumented').glob('*scenario*.xml'));assert a.read_bytes()==b.read_bytes()
        doc=ET.parse(a);reported=int(doc.findtext('.//Request_Count'));assert reported==n
        for variant in ['original','instrumented']:
            counts=re.search(r'total requests generated: (\d+) total requests serviced:(\d+)',(d/variant/'stdout.log').read_text())
            assert counts and tuple(map(int,counts.groups()))==(n,result['host_cqes'])
        result['xml_request_count_semantics']='generated, not serviced (source IO_Flow_Base.cpp:533-546)'
        result.update(expected=n,original_observer_xml_byte_equal=True,result_xml_sha256=digest(a),raw_log_sha256=digest(p),artifact_status='accepted captured trace; not acceptance of complete-task witness')
        cases.append(result)
    mutations=[]
    forged=copy.deepcopy(allrows[512]);next(e for e in forged if e['event']=='host_boundary_after_complete_event')['gc_ongoing_protected_operations']=1
    missing=copy.deepcopy(allrows[2048]);j=next(i for i,e in enumerate(missing) if e['event']=='pcie_consumed_after_followup_schedule');missing.pop(j)
    early=copy.deepcopy(allrows[2048]);e=copy.deepcopy(next(e for e in allrows[512] if e['event']=='host_boundary_after_complete_event'));e['time_ns']=early[0]['time_ns'];early.insert(1,e)
    for name,rows,n in [('forged_gc_witness',forged,512),('removed_pcie_completion',missing,2048),('forged_early_boundary',early,2048)]:
        try:check_rows(rows,n)
        except AssertionError:mutations.append({'name':name,'rejected':True})
        else:raise AssertionError('mutation accepted: '+name)
    report={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'method':'independent read-only accounting; does not reexecute generator/simulator','checker_sha256':digest(Path(__file__)),'artifact_hash_checks':hashed,'cases':cases,'negative_mutations':mutations,'structural_accounting':'PASS','original_instrumented_xml_equivalence':'4/4 exact bytes','complete_host_boundaries':sum(bool(x['boundaries']) for x in cases),'complete_overwrite_boundaries':sum(bool(x['boundaries']) for x in cases if x['expected']>512),'residual_gc_witnesses':sum(x['residual_gc_witness'] for x in cases),'research_verdict':'NOT_ESTABLISHED: three overwrite cases incomplete, not a negative result on residual-GC possibility','statistical_scope':'deterministic fixed cases; no population inference or statistical significance claim','new_simulator_runs_by_this_checker':0}
    (R/'root/boundary-verification.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ['structural_accounting','artifact_hash_checks','complete_host_boundaries','complete_overwrite_boundaries','residual_gc_witnesses','research_verdict']},ensure_ascii=False))
if __name__=='__main__':main()
