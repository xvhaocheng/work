#!/usr/bin/env python3
"""Read-only independent ledger verification; never executes a model."""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys
import yaml

HERE = Path(__file__).resolve().parent
BASE = HERE.parent
CASES = ('cold_short', 'history_idle', 'history_independent_warm',
         'history_conflict_warm', 'history_cap_warm', 'cold_refresh',
         'history_refresh', 'history_independent_replay')
QUEUES = ('read', 'write', 'active', 'priority', 'pending')
STATES = {0: 'Opened', 1: 'Closed', 2: 'PowerUp', 3: 'N/A', 4: 'Refreshing'}

class Invalid(Exception):
    pass

def require(ok, message):
    if not ok:
        raise Invalid(message)

def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def mapping(a):
    return [0, (a >> 13) & 1, (a >> 14) & 3, (a >> 16) & 3,
            (a >> 18) & 65535, (a >> 6) & 127]

def trace_read(p):
    result = []
    for line in p.read_text().splitlines():
        if not line.strip():
            continue
        v = [x.strip() for x in line.split(',')]
        require(len(v) == 8, f'malformed official trace: {line}')
        result.append([int(v[0]), v[1], [int(x) for x in v[2:]]])
    return result

def callbacks(doc):
    return [r for r in doc['records'] if r['kind'] == 'callback']

def normalize(obj):
    if isinstance(obj, dict):
        return {k: normalize(v) for k, v in obj.items()
                if k not in ('case', 'outputpath', 'output_path')}
    if isinstance(obj, list):
        return [normalize(x) for x in obj]
    return obj

def check(doc, case, official, private=True):
    cid = case['id']
    require(doc.get('valid') is True, 'driver did not declare valid')
    require(doc.get('case') == cid, 'case mismatch')
    require(doc.get('instrumented') is private, 'instrumentation flag mismatch')
    require(doc.get('contract') == 'D-DRAM-BOUNDARY-1', 'contract mismatch')
    require(doc.get('clock_epoch') == 'cold-zero', 'non-cold epoch')
    planned = sorted(case['history'] + [case['target']], key=lambda x: x['send_after_tick'])
    records = doc['records']
    require(records[0]['kind'] == 'cold_initial' and records[0]['tick'] == 0, 'missing cold initial')
    require(records[-1]['kind'] == 'final' and records[-1]['tick'] == case['run_until_tick'], 'missing final')
    sends, done, read_issues = {}, {}, []
    banks = {(0, rank, bg, bank): {'state': 1, 'rows': [], 'cap': 0}
             for rank in range(2) for bg in range(4) for bank in range(4)}
    # Independent maintenance FIFO: request_type distinguishes REF and cap PRE.
    maintenance, refreshes, active_ref, cap_events = [], [], {}, []
    actual, post_ticks, full_count, previous_tick = [], [], 0, 0
    pretarget, last_history, last_warm = None, None, None
    nrefi = nrfc = names = None
    ref_ready_gaps = []

    def snapshot(r):
        nonlocal full_count, pretarget, nrefi, nrfc, names
        s, t = r['state'], r['tick']
        for key in ('dram', 'controller_0', 'refresh_0'):
            require(key in s, f'{t}: missing {key}')
            require(s[key]['clock'] == t, f'{t}: component clock {key}')
        c, d, ref = s['controller_0'], s['dram'], s['refresh_0']
        require(ref['next_refresh'] > ref['clock'], f'{t}: refresh already due')
        if nrefi is not None:
            require(ref['nREFI'] == nrefi and ref['next_refresh'] == (t // nrefi + 1)*nrefi,
                    f'{t}: inconsistent periodic refresh deadline')
        require(c['write'] == 0, f'{t}: unexpected writes')
        require(d['future_count'] == len(active_ref), f'{t}: future count differs from REF ledger')
        require(d['refreshing_banks'] == sum(b['state'] == 4 for b in banks.values()),
                f'{t}: refreshing count differs from reconstructed banks')
        drain = len(sends) == len(done) and all(c[k] == 0 for k in QUEUES) and not active_ref and not d['refreshing_banks']
        require(r['drained'] is drain, f'{t}: false drained certificate')
        if r['kind'].startswith('before_') or r['kind'] in ('cold_initial', 'final'):
            require(drain, f'{t}: frozen boundary not drained')
        full = 'nodes' in d
        if r.get('full_state') is True:
            require(full, f'{t}: full_state missing nodes')
        if r['kind'] != 'post_tick':
            require(full, f'{t}: boundary missing full snapshot')
        if not full:
            return
        full_count += 1
        require(d['read_latency'] == 20, f'{t}: source read latency differs')
        nrfc, nrefi, names = d['nRFC'], d['nREFI'], d['command_names']
        require(nrfc > 1 and nrefi > 0, 'invalid refresh timings')
        nodes = {tuple(n['path']): n for n in d['nodes']}
        require(len(nodes) == len(d['nodes']), f'{t}: duplicate node paths')
        banknodes = {p: n for p, n in nodes.items() if n['level'] == 3}
        require(set(banknodes) == set(banks), f'{t}: expected exactly 32 banks')
        cap = s['cap_0']
        require(cap['cap'] == 4 and len(cap['column_accesses']) == 32, 'cap organization mismatch')
        for p, b in banks.items():
            n = banknodes[p]
            require(n['state'] == b['state'], f'{t}: bank {p} state inconsistent')
            require(sorted((x['row'], x['state']) for x in n['rows']) == [(row, 0) for row in b['rows']],
                    f'{t}: bank {p} row map inconsistent')
            flat = p[3] + 4*p[2] + 16*p[1]
            require(cap['column_accesses'][flat] == b['cap'], f'{t}: bank {p} cap inconsistent')
        fa = sorted((tuple(f['address_vector']), f['clock'], names[f['command']]) for f in d['future_actions'])
        expected = sorted((tuple(v['address_vector']), v['issue'] + nrfc-1, 'REFab') for v in active_ref.values())
        require(fa == expected, f'{t}: future action content mismatch')
        for q in QUEUES:
            require(len(c[q+'_entries']) == c[q], f'{t}: {q} size/detail mismatch')
        ordinary = [x for q in ('read', 'active', 'pending') for x in c[q+'_entries']]
        require(sorted(x['id'] for x in ordinary) == sorted(set(sends)-set(done)), f'{t}: queued request ID ledger mismatch')
        for x in ordinary:
            require(x['type'] == 0 and x['address'] == sends[x['id']]['address'], f'{t}: nonread or changed request')
            require(x['address_vector'] == mapping(x['address']), f'{t}: queued mapping mismatch')
        require(c['priority'] == sum(not x.get('issued') for x in maintenance), f'{t}: maintenance queue ledger mismatch')
        if r['kind'] == 'before_target':
            b0 = banks[(0, 0, 0, 0)]
            pretarget = {'tick': t, 'bank0': [STATES[b0['state']], list(b0['rows']), b0['cap']],
                         'all_ready_deadlines_mature': all(v <= t for n in nodes.values() for v in n['command_ready_clock']),
                         'maximum_ready_deadline': max(v for n in nodes.values() for v in n['command_ready_clock']),
                         'drained': drain, 'next_refresh': ref['next_refresh'],
                         'effective_banks': [{'path':list(p),'state':STATES[b['state']], 'rows':list(b['rows']), 'cap':b['cap']} for p,b in banks.items()]}
        if r['kind'] == 'post_tick':
            for rr in refreshes:
                if rr.get('end') == t:
                    ai = names.index('ACT')
                    pathdeadlines = [n['command_ready_clock'][ai] for p,n in nodes.items() if p[:2] == tuple(rr['address_vector'][:2]) or p == (0,)]
                    deadline = max(pathdeadlines)
                    require(deadline == rr['issue'] + nrfc, f'{t}: REF ACT deadline mismatch')
                    ref_ready_gaps.append({'rank': rr['rank'], 'end': t, 'act_ready_deadline': deadline, 'gap': deadline-t})

    for r in records:
        t, kind = r['tick'], r['kind']
        require(t >= previous_tick and 0 <= t <= case['run_until_tick'], 'record time/order mismatch')
        previous_tick = t
        if kind == 'send':
            uid = r['id']
            require(uid == len(sends)+1 and uid <= len(planned), 'send duplicate/unplanned ID')
            p = planned[uid-1]
            require(r['accepted'] is True and t == p['send_after_tick'] == r['send_after_tick'] and
                    r['address'] == p['physical_address'] and r['role'] == p['role'], 'send differs from frozen input')
            sends[uid] = r
        elif kind == 'callback':
            uid = r['id']
            require(uid in sends and uid not in done, 'duplicate/unaccepted callback')
            send = sends[uid]
            require(all(r[k] == send[k] for k in ('address','role','send_after_tick')), 'callback identity changed')
            require(r['address_vector'] == mapping(r['address']), 'callback mapping differs from public bits')
            require(t == r['depart'] and r['arrive'] == send['tick'], 'callback arrive/depart clock mismatch')
            if private:
                match = [e for e in read_issues if e['address_vector'] == r['address_vector'] and e['clock']+20 == t and not e.get('used')]
                require(len(match) == 1, 'callback lacks unique actual RD + read_latency20')
                match[0]['used'] = True
            done[uid] = r
            if r['role'] == 'history': last_history = t
            if r['role'] == 'warm': last_warm = t
        elif kind == 'post_tick':
            post_ticks.append(t)
            if private:
                events = r['events']
                # Row policy enqueues cap PRE before the same tick's actual RD.
                cap_sends = [e for e in events if e['kind']=='priority_send' and e['request_type']==4]
                expected_cap = []
                for e in events:
                    require(e['clock'] == t, f'{t}: nested event clock mismatch')
                    ek = e['kind']; a = e['address_vector']; rank = a[1]
                    if ek == 'priority_send':
                        require(e['accepted'] is True, f'{t}: maintenance enqueue rejected')
                        require(e['request_type'] in (2,4), 'unknown maintenance request type')
                        maintenance.append(dict(e, issued=False))
                    elif ek == 'issued':
                        cmd = e['command']; actual.append([t, cmd, a])
                        require(cmd in ('ACT','PRE','PREA','RD','REFab'), f'unexpected command {cmd}')
                        p = tuple(a[:4]); affected = [b for key,b in banks.items() if key[:2] == tuple(a[:2])]
                        if cmd == 'ACT':
                            require(banks[p]['state'] == 1, 'ACT on nonclosed bank')
                            banks[p].update(state=0, rows=[a[4]])
                        elif cmd == 'RD':
                            b = banks[p]
                            require(b['state'] == 0 and b['rows'] == [a[4]], 'RD without matching open row')
                            b['cap'] += 1
                            if b['cap'] == 4:
                                expected_cap.append(a); b['cap'] = 0
                            read_issues.append(dict(e))
                        else:
                            if cmd == 'PRE':
                                affected = [banks[p]]
                            if cmd in ('PRE','REFab'):
                                typ = 4 if cmd == 'PRE' else 2
                                # Conflict PRE is part of a read, not priority maintenance.
                                matches = [m for m in maintenance if not m['issued'] and m['request_type']==typ and
                                           (m['address_vector']==a if typ==4 else m['address_vector'][:2]==a[:2])]
                                if matches: matches[0]['issued'] = True
                                elif cmd == 'REFab': raise Invalid('REF issue without accepted enqueue')
                            if cmd == 'REFab':
                                require(rank not in active_ref and all(b['state']==1 for b in affected), 'REF on unclosed/inflight rank')
                                rr = {'rank': rank, 'issue': t, 'address_vector': a}
                                refreshes.append(rr); active_ref[rank] = rr
                            for b in affected:
                                b['cap'] = 0; b['state'] = 4 if cmd=='REFab' else 1
                                if cmd != 'REFab': b['rows'] = []
                    elif ek == 'REFab_end':
                        require(rank in active_ref, 'REF end without issue/duplicate')
                        rr = active_ref.pop(rank)
                        require(rr['address_vector'] == a and t == rr['issue']+nrfc-1, 'REF end address/deadline mismatch')
                        rr['end'] = t
                        for p,b in banks.items():
                            if p[:2] == tuple(a[:2]): b['state'] = 1
                    else: raise Invalid(f'unknown audit event {ek}')
                require(sorted(e['address_vector'] for e in cap_sends) == sorted(expected_cap), f'{t}: cap PRE enqueue missing/extra')
                cap_events.extend({'tick':t,'address_vector':a} for a in expected_cap)
                snapshot(r)
        elif 'state' in r:
            if private: snapshot(r)
        else:
            raise Invalid(f'unknown record kind {kind}')
    require(post_ticks == list(range(1, case['run_until_tick']+1)), 'missing/duplicate post_tick')
    require(len(sends) == len(done) == len(planned) == doc['accepted'], 'input completion cardinality mismatch')
    require({int(k):v for k,v in doc['completion_counts'].items()} == {k:1 for k in sends}, 'completion_counts mismatch')
    if private:
        require(actual == official, 'official TraceRecorder differs from actual issue ledger')
        require(len(read_issues) == len(done) and all(e.get('used') for e in read_issues), 'extra unmatched RD')
        require(not active_ref and all(m['issued'] for m in maintenance), 'unfinished maintenance')
        ref_enqueue = [m for m in maintenance if m['request_type']==2]
        due = case['run_until_tick']//nrefi
        require(len(ref_enqueue) == len(refreshes) == 2*due, 'missing all-rank refresh burst')
        for generation in range(1,due+1):
            burst = [m for m in ref_enqueue if m['clock']==generation*nrefi]
            require(sorted(m['address_vector'][1] for m in burst)==[0,1], 'missing second-rank refresh enqueue')
        require(all('end' in x for x in refreshes), 'missing second-rank REF end')
    target = next(x for x in done.values() if x['role']=='target')
    return {'status':'passed', 'target_latency_ticks':target['tick']-target['send_after_tick'],
            'pre_target':pretarget, 'last_history_callback':last_history, 'last_warm_callback':last_warm,
            'full_snapshots_checked':full_count, 'refreshes':refreshes, 'refresh_ready_gaps':ref_ready_gaps,
            'cap_pre_enqueues':cap_events, 'accepted':len(sends), 'callbacks':len(done), 'official_commands':len(official)}

def mutations(docs, case_by_id, traces):
    tests = []
    def run(label, cid, mutate):
        d = copy.deepcopy(docs[cid]); mutate(d)
        try: check(d, case_by_id[cid], traces[cid])
        except (Invalid, KeyError, TypeError, ValueError) as ex:
            tests.append({'mutation':label,'rejected':True,'reason':str(ex)})
        else: tests.append({'mutation':label,'rejected':False})
    def duplicate(d):
        i = next(i for i,r in enumerate(d['records']) if r['kind']=='callback')
        d['records'].insert(i,copy.deepcopy(d['records'][i]))
    def missing_end(d):
        for r in d['records']:
            if 'events' in r:
                r['events'] = [e for e in r['events'] if not (e['kind']=='REFab_end' and e['address_vector'][1]==1)]
    def missing_cap(d):
        for r in d['records']:
            if 'events' in r:
                r['events'] = [e for e in r['events'] if not (e['kind']=='priority_send' and e['request_type']==4)]
    def false_drain(d):
        r = next(r for r in d['records'] if r['kind']=='post_tick' and r['state']['controller_0']['pending']>0)
        r['drained'] = True
    run('duplicate callback','history_idle',duplicate)
    run('missing second rank REF end','history_refresh',missing_end)
    run('missing cap PRE enqueue','history_cap_warm',missing_cap)
    run('false drained with pending','history_idle',false_drain)
    return tests

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--runs', type=Path, required=True)
    ap.add_argument('--report', type=Path, default=HERE/'report.json')
    args = ap.parse_args()
    contract_path = BASE/'root/dram-contract.json'
    prediction_path = BASE/'root/pre-run-predictions.json'
    manifest = json.loads((HERE/'frozen-inputs.json').read_text())
    for relative, sha in manifest.items():
        require(digest(BASE/relative)==sha, f'frozen input changed: {relative}')
    contract = json.loads(contract_path.read_text()); pred = json.loads(prediction_path.read_text())
    require(tuple(c['id'] for c in contract['cases']) == CASES, 'fixed case list changed')
    report = {'status':'pending', 'scope':'finite physical-read ledger only; no policy/novelty/O_host conclusion',
              'frozen_inputs':manifest, 'cases':{}, 'pending':[], 'errors':[]}
    docs, traces, paths = {}, {}, {}
    # Discover only named run outputs under caller-specified directory.
    for p in sorted(args.runs.glob('**/events.yaml')) if args.runs.exists() else []:
        try:
            d = yaml.load(p.read_text(), Loader=yaml.CSafeLoader); key = d.get('case')
            if key not in CASES: raise Invalid(f'unknown run case {key}')
            name = key if d.get('instrumented') else key+'__original'
            require(name not in docs, f'duplicate run {name}')
            docs[name] = d; paths[name] = p
            traces[name] = trace_read(p.parent/'official-trace.ch0')
        except Exception as e: report['errors'].append(f'{p}: {e}')
    case_by_id = {c['id']:c for c in contract['cases']}
    for key in CASES + ('history_cap_warm__original',):
        if key not in docs:
            report['pending'].append(key); continue
        cid = key.removesuffix('__original')
        try:
            result = check(docs[key],case_by_id[cid],traces[key],private=key==cid)
            result['source_prediction_comparison'] = {'latency_matches': result['target_latency_ticks']==pred['target_latency_ticks'][cid],
                 'bank0_matches': None if key!=cid else result['pre_target']['bank0']==pred['expected_pre_target_bank0_row_cap'][cid]}
            result['input_hashes'] = {'events.yaml':digest(paths[key]),'official-trace.ch0':digest(paths[key].parent/'official-trace.ch0')}
            report['cases'][key] = result
        except Exception as e:
            report['cases'][key] = {'status':'failed','error':str(e)}; report['errors'].append(f'{key}: {e}')
    if all(x in docs for x in ('history_cap_warm','history_cap_warm__original')):
        equal = callbacks(docs['history_cap_warm'])==callbacks(docs['history_cap_warm__original']) and traces['history_cap_warm']==traces['history_cap_warm__original']
        report['original_passivity_control_equal'] = equal
        if not equal: report['errors'].append('original/private callbacks or official trace differ')
    if all(x in docs for x in ('history_independent_warm','history_independent_replay')):
        equal = normalize(docs['history_independent_warm'])==normalize(docs['history_independent_replay']) and traces['history_independent_warm']==traces['history_independent_replay']
        report['same_physical_trace_full_replay_equal'] = equal
        if not equal: report['errors'].append('replay records differ')
    if not report['pending'] and not report['errors']:
        report['negative_mutations'] = mutations(docs,case_by_id,traces)
        if not all(t['rejected'] for t in report['negative_mutations']): report['errors'].append('a mutation was accepted')
        # Effective washout uses state and all ready deadlines; raw history arrays need not disappear.
        report['washout_pairs'] = {}
        for warm,cold in (('history_cap_warm','cold_short'),('history_refresh','cold_refresh')):
            a,b = report['cases'][warm],report['cases'][cold]
            report['washout_pairs'][warm] = {'control':cold,'effective_all_banks_equal':a['pre_target']['effective_banks']==b['pre_target']['effective_banks'],
                 'effective_bank0_equal':a['pre_target']['bank0']==b['pre_target']['bank0'],
                 'both_all_ready_deadlines_mature':a['pre_target']['all_ready_deadlines_mature'] and b['pre_target']['all_ready_deadlines_mature'],
                 'target_latency_equal':a['target_latency_ticks']==b['target_latency_ticks']}
    report['status'] = 'failed' if report['errors'] else 'pending' if report['pending'] else 'passed'
    args.report.resolve().relative_to(HERE)
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'status':report['status'],'report':str(args.report),'pending':report['pending'],'errors':report['errors']}))
    return 1 if report['status']=='failed' else 0 if report['status']=='passed' else 2

if __name__ == '__main__':
    sys.exit(main())
