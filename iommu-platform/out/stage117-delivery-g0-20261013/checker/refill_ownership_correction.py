#!/usr/bin/env python3
"""Evaluate only new refill-owner predicate on six original raw records.

This does not run the full verifier, simulator, old probe or six original
falsification controls. Prior immutable acceptance reports retain their original
meaning; this correction supplies the missing exact native owner evidence.
"""
import copy,json,time
from verify_delivery import *

def verify_correction():
    started=time.monotonic();f,pmap,spans,initial,expected,tables=oracle();proofs={};evaluation=[]
    target=None
    for case in CASES:
        d=HERE.parent/'implementation/results'/case
        _,by=load_events(d/'events.csv');_,ops,_=check_cpu_handshake(d);cache=check_axi(d/'cache_axi.csv')
        protocol=dict(ready=sole(by,'common_ready'),quiet=sole(by,'global_quiet'),pe=packet_events(by,pmap))
        begin=time.monotonic();proof=check_refill_ownership(cache,ops,protocol,pmap,spans,tables);assert proof['passed'];proofs[case]=proof
        evaluation.append(dict(case=case,passed=True,only_new_refill_ownership_predicate=True,new_predicate_wall_seconds=time.monotonic()-begin,runtime_AR=proof['runtime_refill_count'],postquiet_AR=proof['postquiet_refill_count'],global_diagnostic_label_mismatches=proof['global_label_mismatch_count']))
        if case=='parallel_eager':target=(cache,ops,protocol)
    cache,ops,protocol=target
    candidates=[r for r in cache['requests'] if r['channel']=='AR' and protocol['ready']['cycle']<=r['cycle']<protocol['quiet']['cycle']]
    req=next(r for r in candidates if r['address'] in {PA+a for a in pmap[1]['destination_closure']})
    peer_address=min(PA+a for a in pmap[3]['destination_closure'])
    assert peer_address%16==0
    cache_mutated=dict(cache);cache_mutated['requests']=[dict(r) if r is req else r for r in cache['requests']]
    mutated=next(r for r in cache_mutated['requests'] if r is not req and r['channel']=='AR' and r['cycle']==req['cycle'])
    mutated['address']=peer_address
    begin=time.monotonic()
    try:check_refill_ownership(cache_mutated,ops,protocol,pmap,spans,tables)
    except Rejected as exc:
        require(str(exc)=='cache AR lacks unique matching actual pending native port1 load','focused mutant unexpectedly rejected by another condition')
        control=dict(name='globally_allowed_peer_refill_not_own_pending_load',passed=True,expected='rejected',actual='rejected',reason=str(exc),source_case='parallel_eager',original_AR_cycle=req['cycle'],original_own_AR_address=req['address'],mutant_peer_AR_address=peer_address,mutation='Change one actual packet1 consumer cache AR address to packet3 permitted destination cacheline, keeping native granted load and timing unchanged.',peer_global_range_would_have_accepted=True,actual_old_gate_globally_allowed=True,wall_seconds=time.monotonic()-begin)
    else:raise AssertionError('peer AR without own pending load was accepted')
    return dict(schema='independent-stage117-delivery-refill-owner-correction-v1',passed=True,problem='Previous global allowed byte union expanded every destination closure byte as if a line start and lacked pending-load owner binding.',correction='Use exact byte sets and bind AR to unique actual pending port1 request address/line and native op role/packet/span. Consumer publish/acquire, sourceAreturn and postquiet table rules checked.',six_new_predicate_evaluations=evaluation,focused_new_negative_control=control,only_new_predicate_rechecked=True,original_six_controls_rerun=False,probe_rerun=False,full_verifier_rerun=False,simulator_calls=0,verification_source_sha256=sha(HERE/'verify_delivery.py'),correction_script_sha256=sha(Path(__file__)),wall_seconds=time.monotonic()-started,case_proofs=proofs)
if __name__=='__main__':
    report=verify_correction();(HERE/'refill-ownership-correction.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(passed=report['passed'],sixcases=report['six_new_predicate_evaluations'],control=report['focused_new_negative_control'],simulator_calls=0)))
