#!/usr/bin/env python3
'''Stage 84 permission-frontier reachability checker.

This is a small finite-state enumerator for the six contract cases only.
It does not model a production DUT or claim system-level progress.
'''

from __future__ import annotations

import argparse
import json
import sys
from collections import deque, defaultdict

POLICIES = ('frontier', 'full_prepare', 'split_source')
GEOMETRIES = ((8, 5, 3), (9, 5, 3))
EVENT_ORDER = {'PTE': 0, 'R': 1, 'W': 2}

HYPOTHESES = [
    'One source AR envelope of aligned fixed-width words; ordered R for this AR; no additional unreserved AR and no interleaved unrelated stream sharing the Q return slots.',
    'The initial WRITE prefix 0..C-1 and the source READ itself are already validated, retained and usable independently of the second destination translation.',
    'Destination permission is a continuous logical prefix covering every word before C; physical prepared-prefix W is an explicit abstract hypothesis, not an RTL or DUT fault claim.',
    'Q is counted actual exclusive return-slot capacity after excluding other descriptors, skid/metadata conflicts and unrelated source data; no same-event W-pop-to-R borrowing.',
    'The return service is a legal worst-case shared serialized service: a new PTE response cannot bypass an already selected AR RLAST; p becomes true only on an actual PTE grant, never by assumption.',
    'Scope excludes AXI AW owner simulation, child/debt scheduling, timeline/performance, faults/remap, unaligned masks, multiple AR or interleaved IDs.',
    'A nonterminal deadlock in this graph is a model artifact of the stated hypotheses and is not a production DUT fault.',
]


def state_key(s):
    r, w, p = s
    return (r, w, 1 if p else 0)


def state_dict(s):
    r, w, p = s
    return {'r': r, 'w': w, 'p': bool(p)}


def is_terminal(s, N):
    r, w, p = s
    return r == N and w == N and bool(p)


def legal_successors(s, N, C, Q, policy):
    r, w, p = s
    out = []
    if not p:
        if policy == 'full_prepare':
            pte_allowed = (r == 0)
        elif policy == 'split_source':
            pte_allowed = (r == C)
        elif policy == 'frontier':
            pte_allowed = (r == N)
        else:
            raise ValueError(f'unknown policy {policy!r}')
        if pte_allowed:
            out.append(('PTE', (r, w, True)))
    if r < N and (r - w) < Q:
        if policy == 'frontier':
            r_allowed = True
        elif policy == 'full_prepare':
            r_allowed = bool(p)
        elif policy == 'split_source':
            r_allowed = (r < C) or bool(p)
        else:
            raise ValueError(f'unknown policy {policy!r}')
        if r_allowed:
            out.append(('R', (r + 1, w, p)))
    if w < r and (w < C or bool(p)):
        out.append(('W', (r, w + 1, p)))
    out.sort(key=lambda item: (EVENT_ORDER[item[0]], state_key(item[1])))
    return out


def state_violations(s, N, C, Q):
    r, w, p = s
    bad = []
    if not (0 <= w <= r <= N):
        bad.append('0<=w<=r<=N violated')
    if not (0 <= r - w <= Q):
        bad.append('0<=r-w<=Q violated')
    if not p and w > C:
        bad.append('p=false implies w<=C violated')
    return bad


def edge_violations(fr, event, to, N, C, Q, policy):
    r, w, p = fr
    r2, w2, p2 = to
    bad = []
    if fr == to:
        bad.append('idle self-edge')
    if event == 'R':
        if not (r2 == r + 1 and w2 == w and p2 == p):
            bad.append('R edge did not change exactly r by +1')
    elif event == 'W':
        if not (r2 == r and w2 == w + 1 and p2 == p):
            bad.append('W edge did not change exactly w by +1')
    elif event == 'PTE':
        if not (r2 == r and w2 == w and p is False and p2 is True):
            bad.append('PTE edge did not set p exactly once')
    else:
        bad.append(f'unknown event {event!r}')
    bad.extend(state_violations(to, N, C, Q))
    if (event, to) not in legal_successors(fr, N, C, Q, policy):
        bad.append('edge is not a legal successor under transition rules')
    return bad


def state_record(s, succ, N):
    r, w, p = s
    return {
        'state': state_dict(s),
        'occupancy': r - w,
        'terminal': is_terminal(s, N),
        'nonterminal_deadlock': (not is_terminal(s, N) and not succ.get(s)),
        'successors': [
            {'event': event, 'to': state_dict(to)}
            for event, to in succ.get(s, [])
        ],
    }


def edge_record(fr, event, to):
    return {
        'from': state_dict(fr),
        'event': event,
        'to': state_dict(to),
        'delta': {
            'r': to[0] - fr[0],
            'w': to[1] - fr[1],
            'p': int(to[2]) - int(fr[2]),
        },
    }


def enumerate_case(policy, N, C, Q):
    initial = (0, 0, False)
    states = {initial}
    edges = []
    queue = deque([initial])
    while queue:
        s = queue.popleft()
        for event, to in legal_successors(s, N, C, Q, policy):
            edges.append((s, event, to))
            if to not in states:
                states.add(to)
                queue.append(to)
    sorted_states = sorted(states, key=state_key)
    sorted_edges = sorted(
        edges,
        key=lambda e: (state_key(e[0]), EVENT_ORDER[e[1]], state_key(e[2])),
    )
    if len(sorted_edges) != len(set(sorted_edges)):
        return None, ['duplicate edges generated']
    errors = []
    for s in sorted_states:
        for msg in state_violations(s, N, C, Q):
            errors.append(f'state {state_dict(s)}: {msg}')
    for fr, event, to in sorted_edges:
        for msg in edge_violations(fr, event, to, N, C, Q, policy):
            errors.append(f'edge {state_dict(fr)} --{event}--> {state_dict(to)}: {msg}')
    succ = defaultdict(list)
    for fr, event, to in sorted_edges:
        succ[fr].append((event, to))
    edge_set = set(sorted_edges)
    for s in sorted_states:
        for event, to in legal_successors(s, N, C, Q, policy):
            if (s, event, to) not in edge_set:
                errors.append(f'missing legal edge {state_dict(s)} --{event}--> {state_dict(to)}')
    terminals = [s for s in sorted_states if is_terminal(s, N)]
    deadlocks = [
        s for s in sorted_states
        if not is_terminal(s, N) and not succ.get(s)
    ]
    for s in terminals:
        if succ.get(s):
            errors.append(f'terminal state {state_dict(s)} has outgoing edges')
    for s in deadlocks:
        if succ.get(s):
            errors.append(f'deadlock state {state_dict(s)} has outgoing edges')
    if errors:
        return None, errors
    parent = {initial: (None, None)}
    dist = {initial: 0}
    q = deque([initial])
    while q:
        s = q.popleft()
        for event, to in succ.get(s, []):
            if to not in parent:
                parent[to] = (s, event)
                dist[to] = dist[s] + 1
                q.append(to)
    for s in sorted_states:
        if s not in dist:
            errors.append(f'reachable state {state_dict(s)} has no BFS distance')
    if errors:
        return None, errors
    candidates = terminals if terminals else deadlocks
    if not candidates:
        return None, ['no terminal and no nonterminal deadlock']
    kind = 'terminal' if terminals else 'deadlock'
    target = min(candidates, key=lambda s: (dist[s], state_key(s)))
    path = []
    cur = target
    while parent[cur][0] is not None:
        prev, event = parent[cur]
        path.append((prev, event, cur))
        cur = prev
    path.reverse()
    final_r, final_w, final_p = target
    case_id = f'{policy}_N{N}'
    counts = {
        'reachable_state_count': len(sorted_states),
        'legal_edge_count': len(sorted_edges),
        'terminal_state_count': len(terminals),
        'nonterminal_deadlock_count': len(deadlocks),
        'witness_kind': kind,
        'witness_final_real_R_count': final_r,
        'witness_final_true_W_count': final_w,
        'witness_second_write_permission_received': bool(final_p),
        'contract_complete_expected_real_R_count': N,
        'contract_complete_expected_true_W_count': N,
        'read_AR_count': 2 if policy == 'split_source' else 1,
        'write_region_preparations': 2,
        'upfront_region_preparations': 2 if policy == 'full_prepare' else 1,
        'startup_region_preparations_before_source_launch': 2 if policy == 'full_prepare' else 1,
        'witness_completed_write_region_preparations': 1 + (1 if final_p else 0),
        'witness_completed_upfront_region_preparations': 1 + (1 if (policy == 'full_prepare' and final_p) else 0),
        'contract_complete_terminal_exists': len(terminals) > 0,
    }
    result = {
        'case_id': case_id,
        'policy': policy,
        'geometry': {'N': N, 'C': C, 'Q': Q},
        'state_encoding': '(r,w,p)',
        'initial_state': state_dict(initial),
        'explicit_conditioned_hypotheses': HYPOTHESES,
        'reachable_states': [state_record(s, succ, N) for s in sorted_states],
        'legal_edges': [edge_record(fr, event, to) for fr, event, to in sorted_edges],
        'terminal_states': [state_dict(s) for s in terminals],
        'nonterminal_deadlocks': [
            {
                'state': state_dict(s),
                'occupancy': s[0] - s[1],
                'reason': 'nonterminal state has no legal progressing R/W/PTE event under the stated hypothesis',
                'note': 'Model deadlock under hypothesis; not a production DUT fault.',
            }
            for s in deadlocks
        ],
        'shortest_witness': {
            'kind': kind,
            'target': state_dict(target),
            'edge_count': dist[target],
            'events': [
                {
                    'from': state_dict(fr),
                    'event': event,
                    'to': state_dict(to),
                }
                for fr, event, to in path
            ],
            'final_real_R_count': final_r,
            'final_true_W_count': final_w,
            'second_write_permission_received': bool(final_p),
        },
        'counts': counts,
    }
    return result, []


def build_all():
    cases = []
    all_errors = []
    for policy in POLICIES:
        for N, C, Q in GEOMETRIES:
            result, errors = enumerate_case(policy, N, C, Q)
            if errors:
                all_errors.extend([f'{policy}_N{N}: {e}' for e in errors])
            else:
                cases.append(result)
    if all_errors:
        for e in all_errors:
            print(e, file=sys.stderr)
        return None
    return {
        'stage': 84,
        'component': 'permission_frontier_reachability',
        'cases': cases,
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description='Stage 84 permission-frontier reachability checker')
    parser.add_argument('--output', required=True, help='output JSON path')
    args = parser.parse_args(argv)
    result = build_all()
    if result is None:
        return 1
    with open(args.output, 'w', encoding='utf-8') as f:
        json.dump(result, f, indent=2, sort_keys=False)
        f.write(chr(10))
    for case in result['cases']:
        c = case['counts']
        w = case['shortest_witness']
        case_id = case['case_id']
        states = c['reachable_state_count']
        edges = c['legal_edge_count']
        terminals = c['terminal_state_count']
        deadlocks = c['nonterminal_deadlock_count']
        kind = w['kind']
        length = w['edge_count']
        real_r = w['final_real_R_count']
        real_w = w['final_true_W_count']
        ar_count = c['read_AR_count']
        prep = c['write_region_preparations']
        upfront = c['upfront_region_preparations']
        completed_prep = c['witness_completed_write_region_preparations']
        print(f'{case_id}: states={states} edges={edges} terminal={terminals} deadlocks={deadlocks} witness={kind} len={length} realR={real_r} realW={real_w} AR={ar_count} prep={prep} upfront={upfront} completed_prep={completed_prep}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
