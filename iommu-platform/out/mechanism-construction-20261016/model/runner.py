#!/usr/bin/env python3
"""Frozen finite event model. Times are integer suffix boundaries; prefix is negative.
Simulation and verification deliberately use separate state reconstruction paths.
No installed dependencies, random search, hardware access, or native timing claims.
"""
import argparse
import copy
import csv
import hashlib
import json
from pathlib import Path
import platform
import sys
import time

HERE = Path(__file__).resolve().parent
CONTRACT = HERE.parent / 'design/model-contract.json'
FROZEN_SHA = '8b59b91b9ab73e0480400ed25a196c55b300e18aa893e30ccc339591a6bd7541'
OWNERS = ['V0', 'A0', 'V1', 'A1']


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def dump(path, obj):
    path.write_text(json.dumps(obj, ensure_ascii=False, indent=2) + '\n')


def programs(c):
    return ([('O_CURRENT', {}) , ('M_TICKET', {}), ('O_SAME_TICKET', {})]
            + [('O_GUARD_' + str(g), {'guard': g}) for g in c['policies'][2]['G_values']]
            + [('O_SPLIT_128_PACED_a' + str(a), {'a_split': a})
               for a in next(x for x in c['policies'] if x['id'] == 'O_SPLIT_128_PACED')['a_split_values']])


def header(job, kind, beats, offset):
    # Logical original parent identity and ranges survive read-only splitting.
    return dict(parent=job, direction=kind, beats=beats, byte_offset=offset,
                parent_bytes=4096, bytes_per_beat=8, attributes='normal-aligned-fullbuffer',
                permission='valid', error='OKAY')


def simulate(c, case, policy, options):
    p = dict(c['parameters'], **case['changes'])
    events = []

    def emit(t, kind, ident, **kw):
        events.append(dict(t=t, kind=kind, id=ident, **kw))

    # Full useful work, explicitly charged common prefix, not a free suffix state.
    for i, (job, offset) in enumerate([('V', 0), ('V', 2048), ('A', 0)]):
        t = -768 + i * 256
        ident = 'prefix' + str(i)
        h = header(job, 'R', 256, offset)
        emit(t, 'offer', ident, channel='AR', **h)
        emit(t, 'AR', ident, **h)
        emit(t, 'data', ident, start=t, end=t + 256, **h)
        emit(t + 256, 'R_done', ident, **h)
    split = 'a_split' in options
    payloads = ([('L0', 128, 2048), ('L1', 128, 3072)] if split
                else [('L', 256, 2048)])
    first_done = None
    l_done = None
    offered_payloads = set()
    accepted = set()
    reads = []
    p_done = {}
    writes_done = {}
    bs = []
    active = None
    owner_index = 0
    gate = False
    ticket_closed = False
    ticket_consumed = False
    missed = False
    ticket_policy = policy in ('M_TICKET', 'O_SAME_TICKET')
    for t in range(0, 20000):
        # All retiring transactions and B controls are processed before admission.
        if active is not None and active['end'] == t:
            a = active
            active = None
            if a['direction'] == 'R':
                emit(t, 'R_done', a['id'], **a['h'])
                if a['id'].startswith('P'):
                    p_done[a['id']] = t
                else:
                    if a['id'] in ('L', 'L1'):
                        l_done = t
                    if a['id'] == 'L0':
                        first_done = t
            elif a['direction'] == 'W':
                emit(t, 'W_done', a['id'], **a['h'])
                writes_done[a['id']] = t
                bs.append((t + p['b_response'], a['id']))
            else:
                emit(t, 'setup_done', a['id'])
        for bt, ident in bs:
            if bt == t:
                emit(t, 'B', ident, response='OKAY')
        if ticket_policy and case['cold'] and t == p['intent_arrival']:
            emit(t, 'intent', 'walk0', marker='P1')
            if offered_payloads or p['L_already_held']:
                missed = True
                emit(t, 'ticket_missed', 'walk0', reason='payload_already_offered')
            else:
                gate = True
                ticket_closed = True
                emit(t, 'ticket_close', 'walk0')
        if gate and t >= p['intent_arrival'] + p['H']:
            gate = False
            emit(t, 'ticket_timeout', 'walk0')
        # Data-dependent DS visibility; accepting a later P never overtakes FIFO.
        ds = []
        if case['cold']:
            if t >= p['d'] and 'P1' not in accepted:
                ds.append('P1')
            if 'P1' in p_done and t >= p_done['P1'] + p['tau1'] and 'P2' not in accepted:
                ds.append('P2')
            if 'P2' in p_done and t >= p_done['P2'] + p['tau2'] and 'P3' not in accepted:
                ds.append('P3')
        # For split, piece1 is not published until piece0 RLAST. No future DS knowledge.
        payload = None
        for ident, beats, offset in payloads:
            if ident in accepted:
                continue
            if ident == 'L1' and first_done is None:
                break
            if not gate and t >= options.get('guard', 0):
                payload = (ident, beats, offset)
            break
        # At most one new AR per boundary. This workload has <=3 consecutive DS,
        # so existing DS-first/F4 fairness never demands its fourth-DS escape.
        if ds:
            ident = ds[0]
            h = dict(parent='walk0', direction='R', beats=1, byte_offset=(int(ident[1])-1)*8,
                     parent_bytes=24, bytes_per_beat=8, attributes='PTE', permission='valid', error='OKAY')
            emit(t, 'offer', ident, channel='AR', **h)
            emit(t, 'AR', ident, **h)
            accepted.add(ident)
            reads.append((ident, h))
            if ident == 'P1' and gate:
                gate = False
                ticket_consumed = True
                emit(t, 'ticket_release', 'walk0', matched='P1')
        elif payload is not None:
            ident, beats, offset = payload
            h = header('A', 'R', beats, offset)
            emit(t, 'offer', ident, channel='AR', **h)
            offered_payloads.add(ident)
            emit(t, 'AR', ident, **h)
            accepted.add(ident)
            reads.append((ident, h))
        if active is None:
            # The explicitly assumed split overhead occupies this same serial resource.
            if split and first_done == t and options['a_split']:
                end = t + options['a_split']
                emit(t, 'setup', 'split_setup', start=t, end=end,
                     assumption='exposed_serial_extra_split_delay')
                active = dict(direction='setup', id='split_setup', end=end)
            elif reads:
                ident, h = reads.pop(0)
                emit(t, 'data', ident, start=t, end=t+h['beats'], **h)
                active = dict(direction='R', id=ident, end=t+h['beats'], h=h)
            elif owner_index < 4:
                ident = OWNERS[owner_index]
                ready = ((not case['cold'] and t >= p['hot_write_ready']) or
                         (case['cold'] and 'P3' in p_done and t >= p_done['P3'] + p['tau_final']))
                if ident.startswith('A'):
                    ready = ready and l_done is not None
                if ready:
                    h = header(ident[0], 'W', 256, int(ident[1])*2048)
                    emit(t, 'AW', ident, **h)
                    emit(t, 'data', ident, start=t, end=t+256, **h)
                    active = dict(direction='W', id=ident, end=t+256, h=h)
                    owner_index += 1
        if (len(writes_done) == 4 and active is None and not reads and not gate
                and all(bt <= t for bt, _ in bs) and l_done is not None
                and (not case['cold'] or len(p_done) == 3)):
            emit(t, 'quiet', 'all')
            break
    else:
        raise RuntimeError('finite model did not drain')
    events.sort(key=lambda e: e['t'])  # stable: same-boundary causal ordering retained
    r = dict(case=case['id'], policy=policy, options=options, parameters=p,
             L_R_complete=l_done, P_R_complete=[p_done[x] for x in ('P1','P2','P3') if x in p_done],
             V_complete=max(bt for bt, x in bs if x.startswith('V')),
             A_complete=max(bt for bt, x in bs if x.startswith('A')),
             quiet=t, full_quiet=t+768, missed_window=missed,
             ticket_closed=ticket_closed, ticket_consumed=ticket_consumed,
             full_AR=sum(e['kind']=='AR' for e in events),
             full_AW=sum(e['kind']=='AW' for e in events),
             full_B=sum(e['kind']=='B' for e in events),
             payload_R_beats=sum(e.get('beats',0) for e in events if e['kind']=='R_done' and e['parent'] in ('A','V')),
             PTE_R_beats=sum(e.get('beats',0) for e in events if e['kind']=='R_done' and e['parent']=='walk0'),
             W_beats=sum(e.get('beats',0) for e in events if e['kind']=='W_done'))
    return r, events


def verify_trace(c, case, r, ev):
    """Reconsume recorded events only, independently reconstruct every obligation.
    This is not a second call to the scheduler or a comparison to its private state.
    """
    errors = []
    def need(ok, why):
        if not ok:
            errors.append(why)
    p = dict(c['parameters'], **case['changes'])
    need(all(type(e['t']) is int for e in ev), 'noninteger event boundary')
    need([e['t'] for e in ev] == sorted(e['t'] for e in ev), 'event time regresses')
    offers, ars, aws, rd, wd, bdone = {}, {}, {}, {}, {}, {}
    fifo = []
    services = []
    setups = []
    gate = False
    gate_terminal = False
    intent = False
    missed = False
    ar_times = set()
    started_r = set()
    started_w = set()
    guard = r['options'].get('guard',0)
    split = 'a_split' in r['options']
    read_h = {k:[] for k in ('V','A','walk0')}
    write_h = {k:[] for k in ('V','A')}
    current_data = None
    for e in ev:
        t, k, ident = e['t'], e['kind'], e['id']
        if k == 'intent':
            need(case['cold'] and t == p['intent_arrival'] and e['marker']=='P1', 'bad intent')
            intent = True
        elif k == 'ticket_close':
            need(intent and not any(i.startswith('L') for i in offers), 'cancelled held offer at close')
            gate = True
        elif k == 'ticket_missed':
            need(intent and (any(i.startswith('L') for i in offers) or p['L_already_held']), 'false missed window')
            missed = True
        elif k == 'ticket_release':
            need(gate and ident=='walk0' and e['matched']=='P1' and 'P1' in ars and ars['P1']['t']==t,
                 'ticket discharged without tagged P1 acceptance')
            gate = False
            gate_terminal = True
        elif k == 'ticket_timeout':
            need(gate and t == p['intent_arrival']+p['H'], 'incorrect timeout')
            gate = False
            gate_terminal = True
        elif k == 'offer':
            need(ident not in offers, 'duplicate offer')
            if ident.startswith('L'):
                need(not gate and t>=guard, 'new payload offer during gate/guard')
                if ident=='L1':
                    need('L0' in rd and t>=rd['L0']['t'], 'split piece published before RLAST')
            offers[ident] = e
        elif k == 'AR':
            need(t not in ar_times, 'more than one AR per boundary')
            ar_times.add(t)
            need(ident in offers and ident not in ars, 'AR missing/duplicate offer')
            if ident in offers:
                need({x:y for x,y in e.items() if x!='kind'} == {x:y for x,y in offers[ident].items() if x not in ('kind','channel')}, 'offer fields changed')
            need(1<=e['beats']<=256, 'illegal burst length')
            if ident=='P1':
                need(case['cold'] and t>=p['d'], 'P1 too early/hot PTE')
            if ident=='P2':
                need('P1' in rd and t>=rd['P1']['t']+p['tau1'], 'P2 dependency violated')
            if ident=='P3':
                need('P2' in rd and t>=rd['P2']['t']+p['tau2'], 'P3 dependency violated')
            ars[ident] = e
            fifo.append(ident)
        elif k == 'AW':
            need(ident not in aws and len(aws)<4 and ident==OWNERS[len(aws)], 'write owner order violated')
            if case['cold']:
                need('P3' in rd and t>=rd['P3']['t']+p['tau_final'], 'cold AW before translation')
            else:
                need(t>=p['hot_write_ready'], 'hot AW before ready')
            if ident.startswith('A'):
                need(('L1' if split else 'L') in rd and 'V0' in wd, 'A write before source/owner')
            if aws:
                need(OWNERS[len(aws)-1] in wd, 'next owner before prior WLAST')
            aws[ident] = e
        elif k in ('data','setup'):
            need(type(e['start']) is int and type(e['end']) is int and e['start']==t and e['end']>t, 'bad service boundaries')
            if services:
                need(services[-1]['end']<=t, 'shared data resource overlap')
            services.append(e)
            if k=='setup':
                setups.append(e)
                need(split and 'L0' in rd and t==rd['L0']['t'] and e['end']-t==r['options']['a_split'], 'invalid exposed split setup')
            else:
                need(e['end']-t==e['beats'], 'service cost not one unit per beat')
                if e['direction']=='R':
                    need(fifo and fifo[0]==ident, 'accepted read FIFO violated')
                    if fifo:
                        fifo.pop(0)
                    need(ident not in started_r, 'read replay')
                    started_r.add(ident)
                    need(ident in ars and e['start']>=ars[ident]['t'], 'R before AR')
                    if ident in ars:
                        need(all(e.get(k)==ars[ident].get(k) for k in ('parent','direction','beats','byte_offset','parent_bytes','permission','attributes','error')), 'R identity/range changed')
                else:
                    need(not fifo, 'write bypassed accepted read FIFO')
                    need(ident in aws and ident not in started_w, 'W before AW/replay')
                    started_w.add(ident)
                current_data=e
        elif k in ('R_done','W_done'):
            need(current_data is not None and current_data['id']==ident and current_data['end']==t, 'response has no matching service/RLAST')
            if current_data is not None:
                need(all(e.get(x)==current_data.get(x) for x in ('parent','beats','byte_offset','error')), 'return identity/error/range mismatch')
            current_data=None
            if k=='R_done':
                need(ident not in rd, 'duplicate RLAST')
                rd[ident]=e
                read_h[e['parent']].append(e)
            else:
                need(ident not in wd, 'duplicate WLAST')
                wd[ident]=e
                write_h[e['parent']].append(e)
        elif k=='B':
            need(ident in wd and ident not in bdone and t==wd[ident]['t']+p['b_response'], 'missing W/bad B latency/duplicate B')
            need(e['response']=='OKAY', 'unexpected error completion')
            bdone[ident]=e
    need(not fifo and not gate and current_data is None, 'quiet leaves read/ticket/data obligation')
    need(set(offers)==set(ars)==set(rd)==started_r, 'held/read obligations not drained')
    need(set(aws)==set(wd)==set(bdone)==set(OWNERS), 'write/B obligations not drained')
    need(len(setups)==(1 if split and r['options']['a_split'] else 0), 'split setup charge absent/extra')
    for job in ('V','A'):
        for hh, direction in ((read_h[job],'R'),(write_h[job],'W')):
            cursor=0
            for x in sorted(hh,key=lambda e:e['byte_offset']):
                need(x['byte_offset']==cursor and x['parent_bytes']==4096 and x['permission']=='valid' and x['error']=='OKAY', 'parent coverage/permission/error violation '+job+direction)
                cursor+=8*x['beats']
            need(cursor==4096, 'parent useful bytes not conserved '+job+direction)
    expected_ptes=3 if case['cold'] else 0
    need(sum(x['beats'] for x in read_h['walk0'])==expected_ptes, 'PTE work not conserved')
    totals={'payload_R_beats':sum(x['beats'] for job in ('V','A') for x in read_h[job]),
            'PTE_R_beats':sum(x['beats'] for x in read_h['walk0']),
            'W_beats':sum(x['beats'] for hh in write_h.values() for x in hh),
            'full_AR':len(ars),'full_AW':len(aws),'full_B':len(bdone)}
    for field,val in totals.items():
        need(r[field]==val, 'summary/trace disagreement '+field)
    need(len(ars)==4+expected_ptes+int(split), 'wrong total AR')
    need(ev[-1]['kind']=='quiet', 'no final quiet event')
    if bdone:
        quiet=max(x['t'] for x in bdone.values())
        need(r['quiet']==quiet and ev[-1]['t']==quiet and r['full_quiet']==quiet+768, 'quiet/full-start mismatch')
        for job in ('V','A'):
            need(r[job+'_complete']==max(x['t'] for ident,x in bdone.items() if ident.startswith(job)), 'job completion mismatch')
    key='L1' if split else 'L'
    need(key in rd and r['L_R_complete']==rd[key]['t'], 'L completion mismatch')
    need(r['P_R_complete']==[rd[k]['t'] for k in ('P1','P2','P3') if k in rd], 'PTE completion summary mismatch')
    need(r['missed_window']==missed, 'missed-window mismatch')
    return errors


def verify_all(c, results, traces):
    checks=[]
    for case in c['frozen_cases']:
        rows={x['policy']:x for x in results if x['case']==case['id']}
        for policy,r in rows.items():
            errors=verify_trace(c,case,r,traces[case['id']][policy])
            checks.append(dict(case=case['id'],policy=policy,passed=not errors,errors=errors))
        expected_errors=[]
        for policy, exp in case['expected'].items():
            if isinstance(exp,dict):
                for key,val in exp.items():
                    if rows[policy].get(key)!=val:
                        expected_errors.append(f'{policy}.{key}: got {rows[policy].get(key)!r}, expected {val!r}')
        gain=rows['O_CURRENT']['quiet']-rows['M_TICKET']['quiet']
        for k in ('complete_gain','candidate_gain'):
            if k in case['expected'] and gain!=case['expected'][k]:
                expected_errors.append(k+' mismatch')
        if 'A_read_penalty' in case['expected']:
            if rows['M_TICKET']['L_R_complete']-rows['O_CURRENT']['L_R_complete']!=case['expected']['A_read_penalty']:
                expected_errors.append('A_read_penalty mismatch')
        if case['expected'].get('missed_window') and not rows['M_TICKET']['missed_window']:
            expected_errors.append('expected missed_window')
        if case['expected'].get('M_TICKET_equals_O_CURRENT'):
            # Interface observation bookkeeping can differ; actual service/offer events must match.
            actual=lambda pol:[e for e in traces[case['id']][pol] if not e['kind'].startswith('ticket_') and e['kind']!='intent']
            if actual('M_TICKET')!=actual('O_CURRENT'):
                expected_errors.append('late-intent service differs')
        if traces[case['id']]['M_TICKET']!=traces[case['id']]['O_SAME_TICKET']:
            expected_errors.append('same paid interface not event-for-event equal')
        checks.append(dict(case=case['id'],check='frozen_expected_and_interface_equivalence',passed=not expected_errors,errors=expected_errors))
    split=next(x for x in c['policies'] if x['id']=='O_SPLIT_128_PACED')
    es=[]
    for cid,field in [('positive_cold','main_zero_cost_expected'),('paired_hot','hot_zero_cost_expected')]:
        row=next(x for x in results if x['case']==cid and x['policy']=='O_SPLIT_128_PACED_a0')
        for key,value in split[field].items():
            if row[key]!=value:es.append(cid+'.'+key+' mismatch')
    cold={x['policy']:x for x in results if x['case']=='positive_cold'}
    hot={x['policy']:x for x in results if x['case']=='paired_hot'}
    for a in split['a_split_values']:
        if cold['O_SPLIT_128_PACED_a'+str(a)]['quiet']!=1290+a:es.append('cold split cost formula')
        if hot['O_SPLIT_128_PACED_a'+str(a)]['quiet']!=1282+a:es.append('hot split cost formula')
    checks.append(dict(check='split_frozen_expected_and_cost',passed=not es,errors=es))
    # Meaningful negative checks: corrupt serialized outputs, then reconsume them.
    case=c['frozen_cases'][0]
    row=cold['M_TICKET']
    original=traces[case['id']]['M_TICKET']
    mutations={}
    x=copy.deepcopy(original); x=[e for e in x if not(e['kind']=='B' and e['id']=='A1')]; mutations['missing_final_B']=x
    x=copy.deepcopy(original); next(e for e in x if e['kind']=='data' and e['id']=='L')['start']=2; mutations['resource_overlap']=x
    x=copy.deepcopy(original); next(e for e in x if e['kind']=='AR' and e['id']=='P2')['t']=3; mutations['PTE_dependency']=x
    x=copy.deepcopy(original); next(e for e in x if e['kind']=='AW' and e['id']=='A0')['id']='A1'; mutations['owner_order']=x
    x=copy.deepcopy(original); next(e for e in x if e['kind']=='R_done' and e['id']=='L')['beats']=255; mutations['useful_bytes']=x
    x=copy.deepcopy(original); next(e for e in x if e['kind']=='ticket_release')['matched']='P2'; mutations['unmatched_ticket']=x
    for name,ev in mutations.items():
        errs=verify_trace(c,case,row,ev)
        checks.append(dict(check='negative_checker_'+name,passed=bool(errs),detected_errors=errs))
    return dict(schema='independent-output-verification-v1',passed=all(x['passed'] for x in checks),checks=checks,
                scope='serialized outputs reconsumed for conservation, dependencies, FIFO, integer boundaries, nonoverlap, parent/error identity, B drain, frozen expected values and exact same-interface trace equality',
                protocol_not_executed=['timeout with late/missing target','downstream held/backpressure stability','reset/epoch mismatch','errors/cancellation','multiwalk fairness escape'],
                # Shared prefix is explicit and observable, hidden cold state is absent from public projection.
                public_initial_history='Identical three prefix reads; no public WVALID at suffix boundary zero. Native/full-system indistinguishability is not proved.')


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--verify-only',action='store_true')
    args=parser.parse_args()
    if digest(CONTRACT)!=FROZEN_SHA:
        raise SystemExit('Frozen model contract hash changed; stop and ask root, do not tune.')
    c=json.loads(CONTRACT.read_text())
    if args.verify_only:
        r=json.loads((HERE/'results.json').read_text())['runs']
        traces=json.loads((HERE/'traces.json').read_text())
        ver=verify_all(c,r,traces)
        dump(HERE/'verification.json',ver)
        print(json.dumps({'verification_passed':ver['passed'],'checks':len(ver['checks'])}))
        return 0 if ver['passed'] else 1
    if (HERE/'execution-receipt.json').exists():
        raise SystemExit('Existing receipt: refuse unchanged batch rerun. Preserve outputs; root-approved concrete amendment required.')
    # Save immutable inputs before simulation begins, including the complete source contract.
    dump(HERE/'frozen-inputs.json',dict(contract_sha256=FROZEN_SHA,contract=c,
        source_contract_sha256=digest(HERE.parent/'source/minimal-source-contract.md'),
        split_note_sha256=digest(HERE.parent/'source/split-control-note.md'),
        programs=[{'id':x,'options':y} for x,y in programs(c)]))
    begun=time.time()
    runs=[]; traces={}
    receipt=dict(schema='execution-receipt-v1',contract_sha256=FROZEN_SHA,
                 runner_sha256=digest(Path(__file__)),frozen_inputs_sha256=digest(HERE/'frozen-inputs.json'),
                 command='python3 model/runner.py',implementation_author='Codex',execution='Codex tool shell execution',
                 requested_model='gpt-6.1-sol',requested_effort='medium',actual_model='unknown',actual_effort='unknown',
                 python=platform.python_version(),started_unix=begun,main_batch_executions=1,expected_simulations=70,
                 frozen_case_count=5,parameter_search=False,failures_or_repairs=[],native_timing=False,
                 production_rtl_changes=0,builds=0,hardware_access=0)
    dump(HERE/'execution-receipt.json',dict(receipt,status='running'))
    for case in c['frozen_cases']:
        traces[case['id']]={}
        for pol,options in programs(c):
            row,ev=simulate(c,case,pol,options)
            runs.append(row);traces[case['id']][pol]=ev
    dump(HERE/'results.json',dict(schema='finite-conditional-model-results-v1',contract_sha256=FROZEN_SHA,runs=runs,
         unexecuted_controls=[{'id':'O_PREP_EARLY','status':'manual_conditional_optimistic_construction','illustrative_full_quiet':2053},
                              {'id':'O_SEGMENT','status':'manual_conditional_optimistic_construction_not_sweep','illustrative_full_quiet':2053}],
         unknown_costs=['native area/Fmax','CPU/integration','source-prefix reachability','actual split overhead'],
         resource_accounting={'original_payload_bytes':8192,'additional_payload_bytes':0,'physical_retention':'unchanged conditional input','CPU':'unknown','a_split':'assumed exposed serial delay, not measured hardware cost'}))
    dump(HERE/'traces.json',traces)
    with (HERE/'traces.csv').open('w',newline='') as f:
        w=csv.writer(f);w.writerow(['case','policy','time','event','id','details_json'])
        for case,pp in traces.items():
            for pol,ev in pp.items():
                for e in ev:w.writerow([case,pol,e['t'],e['kind'],e['id'],json.dumps({k:v for k,v in e.items() if k not in ('t','kind','id')},sort_keys=True)])
    # Verify the actual serialized artifacts, not the in-memory scheduler state.
    saved=json.loads((HERE/'results.json').read_text())['runs']
    saved_traces=json.loads((HERE/'traces.json').read_text())
    ver=verify_all(c,saved,saved_traces)
    dump(HERE/'verification.json',ver)
    receipt.update(status='passed' if ver['passed'] else 'failed_preserved',finished_unix=time.time(),
                   simulations_executed=len(runs),verification_checks=len(ver['checks']),verification_passed=ver['passed'],
                   artifact_sha256={x:digest(HERE/x) for x in ['frozen-inputs.json','results.json','traces.json','traces.csv','verification.json']})
    dump(HERE/'execution-receipt.json',receipt)
    print(json.dumps({'status':receipt['status'],'simulations':len(runs),'checks':len(ver['checks']),
                      'errors':[x for x in ver['checks'] if not x['passed']]},ensure_ascii=False))
    return 0 if ver['passed'] else 1

if __name__=='__main__':
    sys.exit(main())
