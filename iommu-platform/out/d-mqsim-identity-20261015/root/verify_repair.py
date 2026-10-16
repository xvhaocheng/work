#!/usr/bin/env python3
"""Independent cross-layer accounting of captured fixed-case logs. Never runs MQSim."""
from pathlib import Path
import json,hashlib,collections,copy,datetime,re
R=Path(__file__).resolve().parent.parent
B=R/'implementation/repaired'
OLD=R.parent/'d-persistent-source-20261015/offline'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
# Reuse the frozen independent native message/GC accountant, without invoking main.
ref=R.parent/'d-persistent-source-20261015/root/verify_boundary.py'
ns={'__file__':str(ref),'__name__':'reference_accounting'}
exec(compile(ref.read_text(),str(ref),'exec'),ns)
native_check=ns['check_rows']
NO_PPA=2**64-1

def verify(rows,n,trace):
    host={};sqes={};users={};children={};events=collections.defaultdict(lambda:collections.defaultdict(list))
    locks=set();barrier=set();overfull=set();active_programs={};epoch=collections.Counter();used_pages=set()
    counters=collections.Counter();boundaries=[];last=-1
    for i,e in enumerate(rows):
        k=e['event'];t=e['time_ns'];assert t>=last,('time',i);last=t;counters[k]+=1
        assert k!='identity_error',e
        if k=='identity_host_generated':
            uid=e['host_uid'];assert uid==len(host)+1 and uid<=n
            a=trace[uid-1];assert (e['arrival_ns'],e['lba'],e['sectors'],e['type'],e['flow'])==(a[0],a[2],a[3],1,0)
            host[uid]=e
        elif k=='identity_sqe_bind':
            uid=e['host_uid'];assert uid in host and uid not in sqes and host[uid]['host_ptr']==e['host_ptr'];sqes[uid]=e
        elif k=='identity_user_bind':
            uid=e['user_uid'];assert uid==e['host_uid'] and uid in sqes and uid not in users
            assert (e['sqe_ptr'],e['cid'])==(sqes[uid]['sqe_ptr'],sqes[uid]['cid'])
            assert (e['lba'],e['sectors'],e['stream'])==(host[uid]['lba'],host[uid]['sectors'],0);users[uid]=e
        elif k=='identity_child_created':
            kid=e['child_uid'];assert kid==len(children)+1;children[kid]=e
            if e['user_uid']:
                uid=e['user_uid'];assert uid in users and (e['source'],e['type'],e['stream'],e['creation_lpa'],e['creation_ppa'])==(0,1,0,host[uid]['lba']//16,NO_PPA)
        elif k=='identity_lpa_lock':
            key=e['stream'],e['lpa'];assert key not in locks;locks.add(key)
        elif k=='identity_lpa_unlock':
            key=e['stream'],e['lpa'];assert key in locks;locks.remove(key)
        elif k=='identity_barrier_enter':
            kid=e['child_uid'];assert kid not in barrier;assert (e['stream'],e['creation_lpa']) in locks;barrier.add(kid)
        elif k in ['identity_barrier_write_resume','identity_barrier_read_resume']:
            kid=e['child_uid'];assert kid in barrier and k=='identity_barrier_write_resume';barrier.remove(kid)
            assert (e['stream'],e['creation_lpa']) not in locks
        elif k=='identity_overfull_wait_enter':
            kid=e['child_uid'];assert kid not in overfull;overfull.add(kid)
        elif k=='identity_overfull_wait_exit':
            kid=e['child_uid'];assert kid in overfull;overfull.remove(kid)
        elif k=='identity_user_allocate':
            kid=e['child_uid'];assert kid in children and e['user_uid']==children[kid]['user_uid']>0
            assert not e['locked'] and (e['stream'],e['lpa']) not in locks
            assert e['new_ppa']!=NO_PPA and 0<=e['new_ppa']<1024
            block=e['new_ppa']//16;key=block,epoch[block],e['new_ppa']%16
            assert key not in used_pages;used_pages.add(key)
            assert kid not in active_programs;active_programs[kid]=(block,e['new_ppa'])
        elif k=='identity_gc_candidate':
            block=e['block'];assert e['ongoing_user_program_count']==0
            assert not any(b==block for b,p in active_programs.values()),('gc while user program pending',block)
        elif k=='gc_end':epoch[e['block']]+=1
        elif k=='identity_phy_broadcast' and e['user_uid']:
            kid=e['child_uid'];assert kid in active_programs;active_programs.pop(kid)
        elif k=='identity_host_complete':
            uid=e['host_uid'];assert e['cqe_user_uid']==uid and uid in sqes and e['cid']==sqes[uid]['cid']
        elif k=='host_boundary_after_complete_event':boundaries.append(e)
        if k.startswith('identity_'):
            if 'child_uid' in e:events['c',e['child_uid']][k].append(i)
            if 'user_uid' in e and e['user_uid']:events['u',e['user_uid']][k].append(i)
            if 'host_uid' in e:events['h',e['host_uid']][k].append(i)
        if k=='identity_phy_issue' and e['user_uid']:assert e['child_uid'] not in overfull and e['child_uid'] not in barrier
    assert len(host)==len(sqes)==len(users)==n
    owned={};required=['identity_child_created','identity_user_allocate','identity_tsu_submit','identity_phy_issue','identity_phy_broadcast','identity_cache_child_serviced','identity_phy_delete']
    for kid,c in children.items():
        if not c['user_uid']:continue
        uid=c['user_uid'];assert uid not in owned;owned[uid]=kid;ce=events['c',kid]
        assert all(len(ce[k])==1 for k in required),(uid,kid,{k:len(ce[k]) for k in required})
        seq=[ce[k][0] for k in required[:-1]];assert seq==sorted(seq)
        assert ce['identity_phy_delete'][0]>ce['identity_phy_broadcast'][0]
        ue=events['u',uid];he=events['h',uid]
        assert all(len(ue[k])==1 for k in ['identity_cache_owner_complete','identity_cqe_send'])
        assert len(he['identity_host_complete'])==1
        assert ce['identity_cache_child_serviced'][0]<ue['identity_cache_owner_complete'][0]<ue['identity_cqe_send'][0]<he['identity_host_complete'][0]
        assert not any('delete' in k and k!='identity_phy_delete' for k in ce),('premature delete',uid)
    assert len(owned)==n and not locks and not barrier and not overfull and not active_programs
    assert counters['identity_mapping_wait_enter']==0 and not any(c['source']==1 for c in children.values())
    # Only host write children should be attached to user requests in this contract.
    native=[e for e in rows if not e['event'].startswith('identity_')]
    account=native_check(native,n);assert account['completion_gate']=='PASS' and len(boundaries)==1
    return {'expected':n,'host_complete':n,'unique_user_write_children':len(owned),'each_user_allocation_submit_issue_phy_remove_cqe_once':True,'barrier_enter_resume':counters['identity_barrier_enter'],'overfull_enter_exit':counters['identity_overfull_wait_enter'],'gc_candidates':counters['identity_gc_candidate'],'gc_operations':account['gc_operations_started'],'gc_at_complete_host_pcie_boundary':boundaries[0]['gc_ongoing_protected_operations'],'boundary_time_model_ns':boundaries[0]['time_ns'],'no_locked_allocation_or_live_program_gc':True,'async_mapping_wait_or_CACHE':0,'native_accounting':account,'all_event_records':len(rows),'target1777_child':owned.get(1777),'event_counts':dict(counters)}

def main():
    totals=json.loads((B/'runs/status.json').read_text());assert totals['actual_simulation_runs']==4 and not totals['stopped_on_process_failure']
    cases=[];rows2048=None;trace2048=None;hashes=0
    for n in [512,2048,4096,8192]:
        d=B/'runs'/str(n);m=json.loads((d/'pre-run-manifest.json').read_text());res=json.loads((d/'result.json').read_text())
        assert res['returncode']==0 and not res['timed_out']
        for path,h in m['hashes'].items():assert sha(Path(path))==h,path;hashes+=1
        for f in ['ssdconfig.xml','workload.xml']:assert (d/f).read_bytes()==(OLD/'runs'/str(n)/'instrumented'/f).read_bytes()
        rows=[json.loads(s) for s in (d/'stderr.log').read_text().splitlines()];trace=[list(map(int,s.split())) for s in (OLD/f'inputs/write-{n}.trace').read_text().splitlines()]
        c=verify(rows,n,trace);c['raw_log_sha256']=sha(d/'stderr.log');c['input_sha256']=sha(OLD/f'inputs/write-{n}.trace')
        if n==512:
            assert [e for e in rows if not e['event'].startswith('identity_')]==[json.loads(s) for s in (OLD/'runs/512/instrumented/stderr.log').read_text().splitlines()]
            assert (d/'workload_scenario_1.xml').read_bytes()==(OLD/'runs/512/instrumented/workload_scenario_1.xml').read_bytes();c['negative_control_old_events_XML_exact']=True
        if n==2048:rows2048=rows;trace2048=trace
        assert not re.search(r'\b(?:ERROR|Inconsistency|Illegal operation|Unexpected situation)\b',(d/'stdout.log').read_text())
        cases.append(c)
    mutations=[]
    mut=copy.deepcopy(rows2048);i=next(i for i,e in enumerate(mut) if e['event']=='identity_phy_broadcast' and e['user_uid']==1777);mut.pop(i)
    mut2=copy.deepcopy(rows2048);next(e for e in mut2 if e['event']=='identity_user_allocate')['locked']=True
    mut3=copy.deepcopy(rows2048);next(e for e in mut3 if e['event']=='identity_gc_candidate')['ongoing_user_program_count']=1
    mut4=copy.deepcopy(rows2048);j=next(i for i,e in enumerate(mut4) if e['event']=='identity_tsu_submit' and e['user_uid']==1777);mut4.insert(j+1,copy.deepcopy(mut4[j]))
    for name,m in [('fake_completion_without_own_phy',mut),('allocate_while_locked',mut2),('gc_with_user_program_pending',mut3),('double_submit_same_child',mut4)]:
        try:verify(m,2048,trace2048)
        except AssertionError:mutations.append({'name':name,'rejected':True})
        else:raise AssertionError('negative mutation accepted '+name)
    out={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'status':'PASS_FIXED_FOUR_WRITE_ONLY_CASES','verifier_sha256':sha(Path(__file__)),'mode':'independent read-only captured-log accounting, zero new simulations','checked_pre_run_hashes':hashes,'cases':cases,'negative_mutations':mutations,'residual_gc_witnesses':0,'gc_claim_scope':'four exact cacheoff write histories in private semantic variant; zero residual GC at their complete host+PCIe boundaries; not all workloads/SSDs','unsupported':['generic mixed reads/writes/partial/cache/multichip','actual payload byte integrity','IOMMU clear/warm persistence or P/PN performance','native-device timing or paper novelty']}
    (R/'root/repair-verification.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n')
    print(json.dumps({'status':out['status'],'checked_pre_run_hashes':hashes,'cases':[(c['expected'],c['host_complete'],c['barrier_enter_resume'],c['gc_at_complete_host_pcie_boundary']) for c in cases],'negative_mutations':len(mutations)},ensure_ascii=False))
if __name__=='__main__':main()
