#!/usr/bin/env python3
"""Bounded new-cache falsification controls on copied fresh traces only.

Does not execute RTL, microtests, old protocol tests, or the old test matrix.
Temporary synthetic corruptions never enter implementation raw-failure custody.
"""
import csv
import datetime
import json
from pathlib import Path
import shutil
import tempfile
import time
import verify_cache as checker

HERE=Path(__file__).resolve().parent
SOURCE=HERE.parent/'implementation/results/parallel_FIFO_ahead'

def read_csv(path):
    with path.open() as h:
        reader=csv.DictReader(h);return reader.fieldnames,list(reader)

def put_csv(path,names,rows):
    with path.open('w') as h:
        writer=csv.DictWriter(h,fieldnames=names);writer.writeheader();writer.writerows(rows)

def mutate_csv(path,name,predicate,change):
    p=path/name;names,rows=read_csv(p)
    row=next(r for r in rows if predicate(r));change(row);put_csv(p,names,rows)

def wrong_CPU_response(path):
    mutate_csv(path,'cpu_ports.csv',lambda r:r['phase']=='consumer' and r['rvalid']=='1',
               lambda r:r.update(rdata=str(int(r['rdata']) ^ (1 << (8*next(i for i in range(8) if int(r['be'])&(1<<i)))))))

def pending_write_ack(path):
    mutate_csv(path,'cache_control.csv',lambda r:r['ack']=='1',lambda r:r.update(wb_empty='0'))

def missing_Icache_index(path):
    mutate_csv(path,'cache_control.csv',lambda r:r['phase']=='packet_acquire' and r['i_flush_en']=='1' and r['i_flush_idx']=='128',
               lambda r:r.update(i_flush_idx='127'))

def refill_other_owner(path):
    mutate_csv(path,'cache_axi.csv',lambda r:r['channel']=='AR' and r['valid']=='1' and r['ready']=='1'
               and r['phase']=='prepublish_validation' and r['packet']=='1',lambda r:r.update(packet='2'))

def broken_shared_alias(path):
    mutate_csv(path,'cache_axi.csv',lambda r:r['channel']=='R' and r['valid']=='1' and r['ready']=='1',
               lambda r:r.update(data=str(int(r['data']) ^ 1)))

def marker_before_actual_metadata_B(path):
    _,ops=checker.check_cpu_handshake(path);cache=checker.check_axi(path/'cache_axi.csv')
    op=next(o for o in ops if o['phase']=='source_retirement_metadata' and o['packet']==1)
    retire=next(r['request']['retire_cycle'] for r in cache['writes']
                if r['address']==checker.META+48 and r['cycle']>op['grant_cycle'])
    target=op['grant_cycle']+1
    assert target<retire
    names,rows=read_csv(path/'events.csv');event=next(r for r in rows if r['event']=='A_reused' and r['span']=='0')
    rows.remove(event);event['cycle']=str(target)
    at=next(i for i,r in enumerate(rows) if int(r['cycle'])>target)
    rows.insert(at,event);put_csv(path/'events.csv',names,rows)

def missing_actual_pool_initialization_grant(path):
    mutate_csv(path,'cpu_ports.csv',lambda r:r['phase']=='initialization' and r['req']=='1' and r['gnt']=='1'
               and checker.PA<=int(r['address'])<checker.META,lambda r:r.update(gnt='0'))

def missing_postscan_release(path):
    names,rows=read_csv(path/'events.csv')
    rows=[r for r in rows if r['event']!='release_quiet'];put_csv(path/'events.csv',names,rows)

def dishonest_executor_flags(path):
    p=path/'run.json';d=json.loads(p.read_text());d['pass']=False;d['quarantined']=True
    d['actual_cache']={'everything':987654321};d['counts']={'everything':987654321}
    p.write_text(json.dumps(d)+'\n')

TESTS=[
 ('wrong_actual_CPU_consumer_response',wrong_CPU_response,'actual cache CPU response differs'),
 ('flush_ack_with_actual_pending_WT_buffer',pending_write_ack,'flush ack before writebuffer/MSHR drain'),
 ('incomplete_actual_Icache_invalidation_sweep',missing_Icache_index,'complete256-index invalidation sweep'),
 ('actual_refill_bound_to_peer_packet',refill_other_owner,'refill crossed own destination capacity16B closure'),
 ('cache_R_not_from_same_actual_DDR',broken_shared_alias,'physical alias reads mismatch'),
 ('source_retirement_marker_before_actual_metadata_B',marker_before_actual_metadata_B,'physical metadata marker claimed before actual WT B retirement'),
 ('missing_actual_CPU_pool_initialization_grant',missing_actual_pool_initialization_grant,'CPU request changed before grant'),
 ('missing_postscan_actual_release_quiet',missing_postscan_release,'exactly one release_quiet'),
]

def main():
    reports=[];ledger=[]
    for name,mutation,expected in TESTS:
        started=datetime.datetime.now(datetime.timezone.utc).isoformat();start=time.monotonic()
        with tempfile.TemporaryDirectory(prefix='synthetic-cache-tamper-',dir=HERE) as tmp:
            copy=Path(tmp)/'case';shutil.copytree(SOURCE,copy);mutation(copy)
            try:
                checker.verify_case('parallel_FIFO_ahead',copy)
            except checker.Rejected as exc:
                reason=str(exc);passed=expected in reason
            else:reason='tampered case accepted';passed=False
        reports.append(dict(name=name,passed=passed,expected_rejection=expected,actual_rejection=reason))
        ledger.append(dict(purpose='Synthetic copied-new-trace cache-specific falsification',
            name=name,started_UTC=started,elapsed_seconds=time.monotonic()-start,
            checker_evaluations=1,mutation_source=__file__,passed=passed))
    start=time.monotonic();started=datetime.datetime.now(datetime.timezone.utc).isoformat()
    with tempfile.TemporaryDirectory(prefix='executor-flag-independence-',dir=HERE) as tmp:
        copy=Path(tmp)/'case';shutil.copytree(SOURCE,copy);dishonest_executor_flags(copy)
        report=checker.verify_case('parallel_FIFO_ahead',copy)
    reports.append(dict(name='acceptance_independent_of_executor_PASS_and_cost_counters',passed=report['passed'],
        evidence='Only run.json flags/counters changed; actual CPU/cache/controller/shared-DDR evidence independently accepted.'))
    ledger.append(dict(purpose='Executor flag and counter independence control on copied new trace',
        started_UTC=started,elapsed_seconds=time.monotonic()-start,checker_evaluations=1,passed=report['passed']))
    result=dict(schema='independent-real-cache-checker-selftests-v1',passed=all(r['passed'] for r in reports),
        count=len(reports),tests=reports,scope='Eight critical new-cache mutations plus one executor-flag control; copied new parallel trace only.',
        no_RTL_binary_runs=True,old_protocol_battery_replays=0)
    (HERE/'checker-selftests.json').write_text(json.dumps(result,indent=2)+'\n')
    lp=HERE/'execution-ledger.json';d=json.loads(lp.read_text());d['entries'].extend(ledger)
    lp.write_text(json.dumps(d,indent=2)+'\n');print(json.dumps(result,indent=2))
    return int(not result['passed'])

if __name__=='__main__':raise SystemExit(main())
