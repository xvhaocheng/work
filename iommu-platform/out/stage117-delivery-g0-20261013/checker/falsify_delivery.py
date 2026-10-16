#!/usr/bin/env python3
"""Bounded controls unique to new CPU copy, batch polling and raw metrics.

No simulator invocation; no earlier cache/protocol/microtest battery replay.
"""
import copy,json,time
from verify_delivery import *

def exercise():
    f,pmap,spans,initial,expected,tables=oracle();root=HERE.parent/'implementation/results';d=root/'cpu_eager';raw,ops,ports=check_cpu_handshake(d);rows,by=load_events(d/'events.csv');tests=[]
    def negative(name,function,scope,mutation):
        begin=time.monotonic()
        try:function()
        except Rejected as e:tests.append(dict(name=name,passed=True,expected='rejected',actual='rejected',reason=str(e),scope=scope,mutation=mutation,wall_seconds=time.monotonic()-begin))
        else:tests.append(dict(name=name,passed=False,expected='rejected',actual='accepted',scope=scope,mutation=mutation,wall_seconds=time.monotonic()-begin))
    altered=copy.deepcopy(ops)
    for o in altered:
        if o['phase']=='cpu_copy' and not o['we']:o['phase']='removed_actual_source_response'
    negative('CPU_hostknown_payload_without_actual_source_responses',lambda:check_cpu_copy('cpu_eager',altered,pmap,spans,initial,by),'new CPU source→store byte dependency','Keep actual destination stores but remove source response roles from the CPU copy evidence.')
    altered=copy.deepcopy(ops);o=next(o for o in altered if o['phase']=='cpu_copy' and o['we']);lane=next(l for l in range(8) if o['be']&(1<<l));o['wdata']^=1<<(8*lane)
    negative('CPU_store_byte_not_from_actual_returned_word',lambda:check_cpu_copy('cpu_eager',altered,pmap,spans,initial,by),'new CPU source→store byte dependency','Flip one selected destination store byte while actual source response remains unchanged.')
    altered=copy.deepcopy(ops);o=next(o for o in altered if o['phase']=='cpu_copy' and o['we'] and o['span']==3 and o['be'].bit_count()>1);lane=next(l for l in range(8) if o['be']&(1<<l));o['be']&=~(1<<lane)
    negative('CPU_missing_original_span3_strobe',lambda:check_cpu_copy('cpu_eager',altered,pmap,spans,initial,by),'new exact maskedstore/fourspan coverage','Clear one real span3 destination byte enable while retaining every other actual operation.')
    d=root/'parallel_batch';rows,by=load_events(d/'events.csv');raw2,ops2,ports2=check_cpu_handshake(d);cache=check_axi(d/'cache_axi.csv');control,fences=check_control(d,by,cache)
    first_begin=by['poll_round_begin'][0];first_end=by['poll_round_end'][0];target=next(r for r in rows if first_begin['index']<r['index']<first_end['index'] and r['event']=='mmio_begin' and r['engine']==1 and r['address']==0x30);remove={target['index']};remove.update(r['index'] for r in rows if target['index']<r['index']<first_end['index'] and r['engine']==1 and r['span']==target['span'] and r['event'] in ('mmio_ack','mmio_return','done_poll_return'))
    mutated=[];mutby=collections.defaultdict(list)
    for row in rows:
        if row['index'] in remove:continue
        r=dict(row,index=len(mutated));mutated.append(r);mutby[r['event']].append(r)
    def missing_poll():
        proto=check_protocol(mutated,mutby,'parallel_batch',pmap,spans,initial,tables)
        return check_prefix_scheduler('parallel_batch',mutby,ops2,fences,proto,pmap,spans)
    negative('batch_skips_active_engine_public_DONE_poll',missing_poll,'new batch ordinary allactive poll control',f'Remove busy DONE transaction of engine1 in first round ({len(remove)} events) while keeping later actual completions.')
    rows,by=load_events(root/'cpu_eager/events.csv')
    negative('batch_wrongly_accepts_early_completed_prefix',lambda:check_protocol(rows,by,'cpu_batch',pmap,spans,initial,tables),'new batch allcopycomplete publication contract','Relabel actual CPU eager trace as batch, retaining its early incomplete batch publication.')
    r=json.loads((HERE/'single_batch-independent-verification.json').read_text());run=json.loads((root/'single_batch/run.json').read_text());changed=copy.deepcopy(run);changed['used_publish_vector'][0]+=1
    negative('used_metric_claim_not_raw_metadata_release',lambda:check_summary_fields(changed,r['metrics']),'new primary metric raw reconstruction','Add one cycle to claimed first used vector without changing actual physical metadata B/release signals.')
    begin=time.monotonic();changed=copy.deepcopy(run);changed['pass']=False
    try:
        mutant=HERE/'mutants/PASS-independent';mutant.mkdir(parents=True,exist_ok=True)
        for p in (root/'single_batch').iterdir():
            if p.name=='run.json':continue
            q=mutant/p.name
            if not q.exists():q.symlink_to(p.resolve())
        (mutant/'run.json').write_text(json.dumps(changed,indent=2)+'\n')
        report=verify_case('single_batch',mutant);require(report['passed'],'PASS-independent full verifier failed')
        tests.append(dict(name='executor_PASS_flag_independence',passed=True,expected='accepted',actual='accepted',scope='full new verifier over actual unchanged raw records',mutation='Set executor run.json pass=false in isolated checker-owned symlink fixture; full verifier still accepts independently valid raw evidence.',wall_seconds=time.monotonic()-begin,full_verifier_exercised=True))
    except Exception as e:tests.append(dict(name='executor_PASS_flag_independence',passed=False,expected='accepted',actual='rejected',reason=str(e)))
    return dict(schema='independent-stage117-delivery-bounded-falsification-v1',passed=all(t['passed'] for t in tests),negative_controls=6,positive_controls=1,controls=tests,old_cache_microtests_replayed=0,old_protocol17_replayed=0,simulator_calls=0,source_file_sha256=sha(Path(__file__)),verification_source_sha256=sha(HERE/'verify_delivery.py'))
if __name__=='__main__':
    r=exercise();(HERE/'bounded-falsification.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(dict(passed=r['passed'],negative_controls=r['negative_controls'],positive_controls=r['positive_controls'],results=[dict(name=t['name'],passed=t['passed'],reason=t.get('reason')) for t in r['controls']])));raise SystemExit(int(not r['passed']))
