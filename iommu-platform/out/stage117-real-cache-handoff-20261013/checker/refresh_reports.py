#!/usr/bin/env python3
"""Targeted reporting revision on unchanged already-accepted original traces.

Reconstructs new labels/costs/stage projections; does not replay the full
protocol acceptance suite, run RTL, or repeat the existing mutation controls.
"""
import collections
import datetime
import json
import time
from pathlib import Path
import verify_cache as v

HERE=Path(__file__).resolve().parent

def refresh(case):
    path=HERE/f'{case}-independent-verification.json';old_hash=v.sha(path)
    report=json.loads(path.read_text());directory=HERE.parent/'implementation/results'/case
    assert report['passed']
    for name,digest in report['raw_artifact_sha256'].items():
        assert v.sha(directory/name)==digest, f'raw input changed: {case}/{name}'
    _,by=v.load_events(directory/'events.csv');raw,ops=v.check_cpu_handshake(directory)
    cache=v.check_axi(directory/'cache_axi.csv');control=v.cycle_trace(directory/'cache_control.csv')
    _,pmap,spans,_,_,_=v.oracle();pe={pid:{kind:next(r for r in by[kind] if r['span']==pid-1)
          for kind in ('cache_acquired','used_publish','A_read_retired','A_reused')}
          for pid in pmap}
    ready=v.sole(by,'common_ready');sev={s['id']:{kind:next(r for r in by[kind] if r['span']==s['id'])
          for kind in ('backend_accepted','backend_response','done_register_matched','issue','front_accepted')}
          for s in spans}
    stage=[];span_engine={sid:e for e,ids in v.CASES[case].items() for sid in ids}
    for pid in pmap:
        own=span_engine[next(s['id'] for s in spans if s['packet']==pid)]
        for s in spans:
            if s['packet']==pid or span_engine[s['id']]==own:continue
            a=sev[s['id']]['backend_accepted']['cycle'];b=sev[s['id']]['backend_response']['cycle']
            for kind in ('cache_acquired','used_publish'):
                cycle=pe[pid][kind]['cycle']
                if a<=cycle<b:stage.append(dict(packet=pid,stage=kind,cycle=cycle,
                    active_peer_packet=s['packet'],active_peer_span=s['id'],backend_accepted=a,backend_response=b))
    report['earlier_acquire_or_used_publication_peer_witnesses']=stage
    costs=report['costs'];costs.pop('cache_refill_bus_bytes',None)
    ars=[r for r in cache['requests'] if r['channel']=='AR']
    cached=[r for r in ars if r['size']==3 and r['len']==1]
    uncached=[r for r in ars if r['size']==3 and r['len']==0]
    costs.update(cache_cached16B_refill_AR=len(cached),cache_uncached8B_AR=len(uncached),
                 cache_cached16B_refill_bus_bytes=sum(r['beats']*8 for r in cached),
                 cache_uncached8B_read_bus_bytes=sum(r['beats']*8 for r in uncached),
                 cache_total_read_bus_bytes=len(cache['reads'])*8)
    costs['actual_CPU_grant_to_response_cycles_by_phase']={phase:sum(o['response_cycle']-o['grant_cycle'] for o in ops if o['phase']==phase)
                 for phase in sorted({o['phase'] for o in ops})}
    costs['actual_cache_AR_to_RLAST_cycles_by_phase']={phase:sum(r['retire_cycle']-r['cycle'] for r in ars if r['phase']==phase)
                 for phase in sorted({r['phase'] for r in ars})}
    supply=[]
    for e,ids in v.CASES[case].items():
        for old,new in zip(ids,ids[1:]):
            prev=sev[old];succ=sev[new]
            supply.append(dict(engine=e,completed_span=old,successor_span=new,
                previous_backend_response=prev['backend_response']['cycle'],
                previous_public_DONE_matched=prev['done_register_matched']['cycle'],
                successor_issue=succ['issue']['cycle'],successor_frontend_accepted=succ['front_accepted']['cycle'],
                backend_response_to_public_DONE_cycles=prev['done_register_matched']['cycle']-prev['backend_response']['cycle'],
                public_DONE_to_successor_issue_cycles=succ['issue']['cycle']-prev['done_register_matched']['cycle']))
    costs['measured_public_done_successor_supply']=supply
    for pid,p in pmap.items():
        own=[o for o in ops if o['packet']==pid and o['phase']=='source_reuse'];first=min(o['grant_cycle'] for o in own)
        writes=[b for b in cache['writes'] if b['request']['cycle']>=ready['cycle'] and
            any(b['address']+lane-v.PA in p['source_bytes'] for lane in range(8) if b['strb']&(1<<lane))]
        d=costs['actual_CPU_cache_port']['packets'][str(pid)]
        d.update(source_reuse_CPU_stores=len(own),first_actual_source_store_grant=first,
            last_actual_source_store_grant=max(o['grant_cycle'] for o in own),
            source_hold_until_first_actual_overwrite_cycles=first-pe[pid]['A_read_retired']['cycle'],
            source_reuse_drain_actual_common_clock_cycles=sum(r['phase']=='source_reuse_drain' and r['packet']==pid for r in control),
            last_actual_source_store_B_retirement=max(b['request']['retire_cycle'] for b in writes))
    report['targeted_reporting_revision']=dict(prior_accepted_report_sha256=old_hash,
        raw_input_sha256_unchanged=True,reason='Split cached refills/uncached tables, acquire/publication vs actual consumer peer overlap, and actual source/supply costs.',
        full_protocol_suite_reexecuted=False,RTL_binary_executions=0)
    path.write_text(json.dumps(report,indent=2)+'\n')
    return dict(case=case,cached16B_AR=len(cached),uncached8B_AR=len(uncached),
        cached16B_bus_bytes=costs['cache_cached16B_refill_bus_bytes'],uncached_bus_bytes=costs['cache_uncached8B_read_bus_bytes'],
        earlier_stage_peer_witnesses=stage,actual_consumer_peer_overlap=report['real_backend_active_peer_during_actual_cache_consumer'])

def main():
    started=datetime.datetime.now(datetime.timezone.utc).isoformat();t=time.monotonic()
    findings=[refresh(case) for case in v.CASES]
    lp=HERE/'execution-ledger.json';ledger=json.loads(lp.read_text())
    ledger['entries'][0]['checker_evaluations']=1;ledger['entries'][1]['checker_evaluations']=2
    ledger['entries'].append(dict(purpose='Authorized targeted reporting revision against unchanged accepted raw SHA',
        command=['python3',str(Path(__file__).resolve())],started_UTC=started,
        elapsed_seconds=time.monotonic()-t,checker_full_acceptance_evaluations=0,RTL_binary_executions=0,
        raw_parsing_for_changed_cost_labels=True,findings=findings))
    lp.write_text(json.dumps(ledger,indent=2)+'\n');print(json.dumps(findings,indent=2))

if __name__=='__main__':main()
