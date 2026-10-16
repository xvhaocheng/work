#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''geometry_oracle.py -- section 41 pure static geometry oracle.

Single-file standard-library computation of the finite static geometry problem
defined by:
  out/fragment-drain-boundary-20261008/codex-model-contract.md
  out/fragment-drain-boundary-20261008/rules-proof-agent.md

Scope string used in every output section: pure_static_geometry_not_rtl

Guarantees:
* Python 3 standard library only (bisect, json, sys, traceback, itertools).
* Writes exactly one JSON object to stdout; writes no file, runs no shell, uses
  no network, reads no other project file, modifies nothing.
* Every number is pure static geometry over byte offsets. Nothing here models
  or measures a DUT, RTL, TL/PTW traffic, real independent-stop capacity,
  latency, cycles, area or end-to-end progress, and no such claim is made.
* No artificial budget: both finite enumeration groups run in full (7585 and
  3805 cases as specified by the contract).
* Exit code 0 only if greedy and brute agree on every enumerated case and every
  contract check passes; otherwise 1. Computed values are reported as found and
  are never adjusted to make a check pass.
'''

import bisect
import json
import sys
import traceback
from itertools import combinations

MODEL_SCOPE = 'pure_static_geometry_not_rtl'
CONTRACT_SOURCE = 'out/fragment-drain-boundary-20261008/codex-model-contract.md'
PROOF_SOURCE = 'out/fragment-drain-boundary-20261008/rules-proof-agent.md'
MAX_MISMATCH_RECORDS = 20
EXPECTED_GROUP_1_CASES = 7585
EXPECTED_GROUP_2_CASES = 3805


# --------------------------------------------------------------------------
# definitional geometry (shared predicate only, no scheduling decision)
# --------------------------------------------------------------------------

def window_hi(q, B, N):
    '''Inclusive right end of I(q) = [q, min(q+B, N)].'''
    hi = q + B
    return hi if hi < N else N


def supported_by(q, B, N, pts_sorted):
    '''Definitional geometric support.

    q is supported by endpoint set pts_sorted iff some p in pts_sorted satisfies
    q <= p <= min(q+B, N). Shared by greedy, by the independent brute oracle and
    by the old control model; it contains no scheduling decision.
    '''
    if not pts_sorted:
        return False
    i = bisect.bisect_left(pts_sorted, q)
    if i >= len(pts_sorted):
        return False
    return pts_sorted[i] <= window_hi(q, B, N)


def safe_dest_points(D_sorted, B, N, R_sorted):
    '''Sorted destination endpoints that are geometrically supported by R.'''
    return [q for q in D_sorted if supported_by(q, B, N, R_sorted)]


def closure_chain(safe_sorted, H, N):
    '''Contract closure().

    Walk the sorted supported checkpoints from 0; keep pushing the reachable
    prefix forward while the next safe point is within H of the last reachable
    point; stop at the first unsafe gap. Returns (max_reachable, chain).
    '''
    if not safe_sorted or safe_sorted[0] != 0:
        raise ValueError('closure precondition violated: 0 is not a safe checkpoint')
    reach = 0
    chain = [0]
    for q in safe_sorted:
        if q <= 0:
            continue
        if q - reach <= H:
            reach = q
            chain.append(q)
        else:
            break
    if reach > N:
        raise ValueError('closure produced a reach beyond N')
    return reach, chain


def closure_reach(safe_sorted, H, N):
    return closure_chain(safe_sorted, H, N)[0]


def dag_reach_is_full(safe_sorted, H, N):
    '''Independent feasibility test used by the brute oracle.

    Build the checkpoint DAG (edge u -> v iff 0 < v-u <= H) and ask whether N is
    reachable from 0. Written as an explicit reachability scan that does not use
    the greedy walk of closure(), so brute does not copy the greedy decision.
    '''
    reachable = [0]
    for q in safe_sorted:
        if q == 0:
            continue
        for r in reachable:
            if 0 < q - r <= H:
                reachable.append(q)
                break
    return N in reachable


def powerset(seq):
    items = list(seq)
    for r in range(len(items) + 1):
        for combo in combinations(items, r):
            yield list(combo)


# --------------------------------------------------------------------------
# contract greedy
# --------------------------------------------------------------------------

def greedy_solve(A_sorted, R0_sorted, D_sorted, B, H, N):
    '''Plain greedy of the contract (line 19).

    Repeat: close the free supported prefix; if it reaches N stop; otherwise add
    the right-most legal cut of the support window of the LARGEST SUPPORTABLE
    destination checkpoint inside (x, x+H]. The largest D point is never assumed
    to be supportable. A chosen cut that already exists, or a closure that does
    not advance, is reported as implementation_error instead of being repaired.
    '''
    cuts = []
    iterations = []
    previous_reach = None
    guard = len(A_sorted) + 2
    for step in range(guard):
        R_cur = sorted(set(R0_sorted) | set(cuts))
        safe = safe_dest_points(D_sorted, B, N, R_cur)
        reach, chain = closure_chain(safe, H, N)
        if previous_reach is not None and reach <= previous_reach:
            return {
                'status': 'implementation_error',
                'reason': 'closure did not advance after adding a cut',
                'cost': None,
                'cuts': sorted(cuts),
                'reach': reach,
                'previous_reach': previous_reach,
                'chain': chain,
                'iterations': iterations,
            }
        if reach == N:
            return {
                'status': 'feasible',
                'cost': len(cuts),
                'cuts': sorted(cuts),
                'reach': reach,
                'chain': chain,
                'iterations': iterations,
            }
        upper = reach + H
        if upper > N:
            upper = N
        cands = [q for q in D_sorted
                 if reach < q <= upper and supported_by(q, B, N, A_sorted)]
        if not cands:
            return {
                'status': 'infeasible',
                'reason': 'no supportable destination checkpoint inside (reach, reach+H]',
                'cost': None,
                'cuts': sorted(cuts),
                'reach': reach,
                'chain': chain,
                'iterations': iterations,
            }
        q_star = cands[-1]
        lo = q_star
        hi = window_hi(q_star, B, N)
        i = bisect.bisect_right(A_sorted, hi) - 1
        if i < 0 or A_sorted[i] < lo:
            return {
                'status': 'implementation_error',
                'reason': 'q* supportable but no legal cut inside I(q*)',
                'cost': None,
                'q_star': q_star,
                'window': [lo, hi],
                'cuts': sorted(cuts),
                'chain': chain,
                'iterations': iterations,
            }
        c_star = A_sorted[i]
        if c_star in R_cur:
            return {
                'status': 'implementation_error',
                'reason': 'chosen cut already present although q* was not freely reachable',
                'cost': None,
                'q_star': q_star,
                'cut': c_star,
                'cuts': sorted(cuts),
                'chain': chain,
                'iterations': iterations,
            }
        iterations.append({
            'step': step,
            'reach_before': reach,
            'q_star': q_star,
            'window': [lo, hi],
            'cut': c_star,
            'chain_before': chain,
        })
        cuts.append(c_star)
        previous_reach = reach
    return {
        'status': 'implementation_error',
        'reason': 'iteration guard exhausted',
        'cost': None,
        'cuts': sorted(cuts),
        'chain': [],
        'iterations': iterations,
    }


# --------------------------------------------------------------------------
# independent all-subsets brute oracle
# --------------------------------------------------------------------------

def brute_solve(A_sorted, R0_sorted, D_sorted, B, H, N):
    '''Exact independent oracle (contract line 23).

    Enumerate every subset of the legal new cut set A minus R0 in order of
    increasing size; decide feasibility with the independent checkpoint-DAG
    reachability scan; return the first feasible subset, which is a minimum.
    Never calls greedy and never reuses a greedy decision.
    '''
    mandatory = set(R0_sorted)
    pool = [a for a in A_sorted if a not in mandatory]
    for k in range(0, len(pool) + 1):
        for combo in combinations(pool, k):
            R_cur = sorted(mandatory.union(combo))
            safe = safe_dest_points(D_sorted, B, N, R_cur)
            if not safe or safe[0] != 0:
                continue
            if dag_reach_is_full(safe, H, N):
                return {'status': 'feasible', 'cost': k, 'cuts': sorted(combo)}
    return {'status': 'infeasible', 'cost': None, 'cuts': []}


# --------------------------------------------------------------------------
# old per-node-cost control model (kept only to show it is wrong for B > 0)
# --------------------------------------------------------------------------

def naive_node_cost_min(A_sorted, R0_sorted, D_sorted, B, H, N):
    '''Old control model (contract line 25), not a baseline.

    Every destination endpoint costs 0 if a mandatory R0 endpoint supports it,
    1 if any legal A endpoint supports it, and is otherwise not selectable. The
    minimum cost path over the gap <= H DAG is returned. It does not remember the
    support range of already chosen cuts, so it may over-count.
    '''
    node_cost = {}
    for q in D_sorted:
        if supported_by(q, B, N, R0_sorted):
            node_cost[q] = 0
        elif supported_by(q, B, N, A_sorted):
            node_cost[q] = 1
    if 0 not in node_cost or N not in node_cost:
        return {'status': 'infeasible', 'cost': None, 'node_cost': node_cost}
    usable = sorted(node_cost)
    best = {0: node_cost[0]}
    for q in usable:
        if q == 0:
            continue
        chosen = None
        for u in usable:
            if u >= q:
                break
            if 0 < q - u <= H and best.get(u) is not None:
                cand = best[u] + node_cost[q]
                if chosen is None or cand < chosen:
                    chosen = cand
        best[q] = chosen
    if best.get(N) is None:
        return {'status': 'infeasible', 'cost': None, 'node_cost': node_cost}
    return {'status': 'feasible', 'cost': best[N], 'node_cost': node_cost}


# --------------------------------------------------------------------------
# per-case evaluation
# --------------------------------------------------------------------------

def evaluate_case(A, R0, D, B, H, N, counters):
    '''Run greedy + brute + control model on one case and classify the result.'''
    A_sorted = sorted(set(A))
    R0_sorted = sorted(set(R0))
    D_sorted = sorted(set(D))

    g = greedy_solve(A_sorted, R0_sorted, D_sorted, B, H, N)
    b = brute_solve(A_sorted, R0_sorted, D_sorted, B, H, N)
    n = naive_node_cost_min(A_sorted, R0_sorted, D_sorted, B, H, N)

    if g['status'] == 'implementation_error':
        counters['greedy_implementation_error'] += 1
    if b['status'] == 'feasible':
        R_brute = sorted(set(R0_sorted) | set(b['cuts']))
        safe_brute = safe_dest_points(D_sorted, B, N, R_brute)
        if closure_reach(safe_brute, H, N) != N:
            counters['reach_definition_disagreement'] += 1
    if (g['status'] == 'feasible' and b['status'] == 'feasible'
            and g['cost'] < b['cost']):
        counters['greedy_below_brute'] += 1
    if (b['status'] == 'feasible' and n['cost'] is not None
            and n['cost'] < b['cost']):
        counters['naive_below_brute'] += 1

    greedy_cost = g.get('cost')
    brute_cost = b.get('cost')
    record = {
        'N': N,
        'B': B,
        'H': H,
        'A': A_sorted,
        'R0': R0_sorted,
        'D': D_sorted,
        'greedy_status': g['status'],
        'greedy_cost': greedy_cost,
        'greedy_cuts': g.get('cuts', []),
        'greedy_reach': g.get('reach'),
        'brute_status': b['status'],
        'brute_cost': brute_cost,
        'brute_cuts': b.get('cuts', []),
        'naive_status': n['status'],
        'naive_cost': n.get('cost'),
        'greedy_brute_mismatch': greedy_cost != brute_cost,
        'naive_differs_from_brute': n.get('cost') != brute_cost,
        'greedy_implementation_error': g['status'] == 'implementation_error',
    }
    if g['status'] == 'implementation_error':
        record['greedy_error_reason'] = g.get('reason')
    return record


# --------------------------------------------------------------------------
# finite enumeration groups
# --------------------------------------------------------------------------

def group_1_cases():
    '''Contract line 29.

    N=1..5, A = all integer offsets 0..N, R0 and D each any interior subset with
    0/N mandatory, B=0..N-1, H=1..N. Theoretical total 7585.
    '''
    for N in range(1, 6):
        A = list(range(0, N + 1))
        interior = list(range(1, N))
        for R0_int in powerset(interior):
            R0 = [0, N] + list(R0_int)
            for D_int in powerset(interior):
                D = [0, N] + list(D_int)
                for B in range(0, N):
                    for H in range(1, N + 1):
                        yield (N, A, R0, D, B, H)


def group_2_cases():
    '''Contract line 30.

    N=1..4, A any subset of 0..N containing 0/N, R0 any subset of the interior of
    A, D any subset of the full grid interior, B=0..N-1, H=1..N. Total 3805.
    '''
    for N in range(1, 5):
        grid = list(range(1, N))
        for A_int in powerset(grid):
            A = [0, N] + list(A_int)
            for R0_int in powerset(A_int):
                R0 = [0, N] + list(R0_int)
                for D_int in powerset(grid):
                    D = [0, N] + list(D_int)
                    for B in range(0, N):
                        for H in range(1, N + 1):
                            yield (N, A, R0, D, B, H)


def run_enumeration_group(group_name, description, case_iter, counters):
    cases = 0
    feasible_greedy = 0
    infeasible_greedy = 0
    feasible_brute = 0
    infeasible_brute = 0
    greedy_brute_mismatch = 0
    mismatch_records = []
    naive_differs_total = 0
    first_naive_diff = None
    for N, A, R0, D, B, H in case_iter:
        cases += 1
        rec = evaluate_case(A, R0, D, B, H, N, counters)
        if rec['greedy_status'] == 'feasible':
            feasible_greedy += 1
        elif rec['greedy_status'] == 'infeasible':
            infeasible_greedy += 1
        if rec['brute_status'] == 'feasible':
            feasible_brute += 1
        else:
            infeasible_brute += 1
        if rec['greedy_brute_mismatch']:
            greedy_brute_mismatch += 1
            if len(mismatch_records) < MAX_MISMATCH_RECORDS:
                mismatch_records.append(rec)
        if rec['naive_differs_from_brute']:
            naive_differs_total += 1
            if first_naive_diff is None:
                first_naive_diff = rec
    return {
        'group': group_name,
        'description': description,
        'cases': cases,
        'feasible_greedy': feasible_greedy,
        'infeasible_greedy': infeasible_greedy,
        'feasible_brute': feasible_brute,
        'infeasible_brute': infeasible_brute,
        'greedy_brute_mismatch': greedy_brute_mismatch,
        'greedy_brute_mismatch_records_first_20': mismatch_records,
        'naive_vs_brute_differing_cases': naive_differs_total,
        'first_naive_diff_case': first_naive_diff,
    }


# --------------------------------------------------------------------------
# named witnesses
# --------------------------------------------------------------------------

def describe_case(label, N, A, R0, D, B, H, counters):
    A_sorted = sorted(set(A))
    R0_sorted = sorted(set(R0))
    D_sorted = sorted(set(D))
    g = greedy_solve(A_sorted, R0_sorted, D_sorted, B, H, N)
    b = brute_solve(A_sorted, R0_sorted, D_sorted, B, H, N)
    n = naive_node_cost_min(A_sorted, R0_sorted, D_sorted, B, H, N)
    if g['status'] == 'implementation_error':
        counters['greedy_implementation_error'] += 1
    record = {
        'label': label,
        'N': N,
        'B': B,
        'H': H,
        'A': A_sorted,
        'R0': R0_sorted,
        'D': D_sorted,
        'r0_source_burst_count': len(R0_sorted) - 1,
        'dest_burst_count': len(D_sorted) - 1,
        'mandatory_supported_dest_points': [
            q for q in D_sorted if supported_by(q, B, N, R0_sorted)
        ],
        'greedy': {
            'status': g['status'],
            'cost': g.get('cost'),
            'cuts': g.get('cuts', []),
            'reach': g.get('reach'),
            'checkpoint_chain': g.get('chain', []),
            'reason': g.get('reason'),
            'iterations': g.get('iterations', []),
        },
        'brute': {
            'status': b['status'],
            'cost': b.get('cost'),
            'cuts': b.get('cuts', []),
        },
        'naive_node_cost_control': {
            'status': n['status'],
            'cost': n.get('cost'),
            'node_cost_by_dest_endpoint': {
                str(k): v for k, v in sorted(n.get('node_cost', {}).items())
            },
        },
        'greedy_cost_equals_brute_cost': g.get('cost') == b.get('cost'),
    }
    if g['status'] == 'feasible':
        record['greedy_cut_support'] = [
            {
                'cut': c,
                'dest_checkpoints_supported': [
                    q for q in D_sorted if c - B <= q <= c
                ],
            }
            for c in g['cuts']
        ]
    return record


def run_named_witnesses(counters, group1_first_naive_diff, group2_first_naive_diff):
    N10 = 10
    A10 = list(range(0, 11))
    main = describe_case(
        'contract 4.1: one new cut supports two required stops (B=2)',
        N10, A10, [0, 10], [0, 4, 6, 10], 2, 4, counters)
    b0 = describe_case(
        'contract 4.1 B=0 control: same input, per-node cost happens to be exact',
        N10, A10, [0, 10], [0, 4, 6, 10], 0, 4, counters)
    infeasible = []
    for B in (0, 1, 2, 5, 10):
        infeasible.append(describe_case(
            'contract 3 infeasible witness: D={0,5,10}, H=4, any B',
            N10, A10, [0, 10], [0, 5, 10], B, 4, counters))
    return {
        'main_counterexample_N10_B2': main,
        'b0_control_N10_B0': b0,
        'infeasible_N10_D_0_5_10_H4_any_B': infeasible,
        'first_minimal_naive_counterexample_group_1': group1_first_naive_diff,
        'first_minimal_naive_counterexample_group_2': group2_first_naive_diff,
        'first_naive_diff_record_format': 'compact per-case record from the enumeration '
                                          'harness (state only, no support map)',
    }


# --------------------------------------------------------------------------
# byte-scale representative static examples
# --------------------------------------------------------------------------

def byte_scale_inputs():
    N = 32768
    R0 = list(range(0, N + 1, 2048))
    D = [0] + [1024 + 2048 * k for k in range(16)] + [N]
    A = list(range(0, N + 1, 8))
    return N, A, R0, D


def run_byte_examples():
    N, A, R0, D = byte_scale_inputs()
    A_sorted = sorted(A)
    R0_sorted = sorted(R0)
    D_sorted = sorted(D)
    records = []
    for B in (0, 24, 1024):
        for H in (2048, 4096, 32768):
            g = greedy_solve(A_sorted, R0_sorted, D_sorted, B, H, N)
            chain = g.get('chain', [])
            gaps = [chain[i + 1] - chain[i] for i in range(len(chain) - 1)]
            records.append({
                'scope': MODEL_SCOPE,
                'B': B,
                'H': H,
                'status': g['status'],
                'new_cuts': g.get('cost'),
                'cut_positions': g.get('cuts', []),
                'safe_stop_count': len(chain),
                'safe_stop_chain': chain,
                'worst_safe_gap': max(gaps) if gaps else None,
                'source_physical_requests': (len(R0_sorted) - 1) + (g.get('cost') or 0),
                'destination_bursts': len(D_sorted) - 1,
                'closure_reach': g.get('reach'),
                'greedy_reason_if_not_feasible': g.get('reason'),
            })
    return records


# --------------------------------------------------------------------------
# software tiling static counts (contract line 33)
# --------------------------------------------------------------------------

def run_software_tiling(byte_examples):
    N, A, R0, D = byte_scale_inputs()
    tiling = [0, 3072]
    for _ in range(7):
        tiling.append(tiling[-1] + 4096)
    tiling.append(N)
    segments = []
    source_total = 0
    dest_total = 0
    for i in range(len(tiling) - 1):
        lo = tiling[i]
        hi = tiling[i + 1]
        src_pts = sorted(set([lo, hi]) | set(p for p in R0 if lo < p < hi))
        dst_pts = sorted(set([lo, hi]) | set(p for p in D if lo < p < hi))
        src_intervals = len(src_pts) - 1
        dst_intervals = len(dst_pts) - 1
        source_total += src_intervals
        dest_total += dst_intervals
        segments.append({
            'subjob_index': i + 1,
            'byte_range': [lo, hi],
            'size_bytes': hi - lo,
            'source_interior_endpoints': [p for p in src_pts if lo < p < hi],
            'source_effective_intervals': src_intervals,
            'dest_interior_endpoints': [p for p in dst_pts if lo < p < hi],
            'dest_effective_intervals_aw': dst_intervals,
        })
    internal = tiling[1:-1]
    greedy_cuts_b0_h4096 = []
    for rec in byte_examples:
        if rec['B'] == 0 and rec['H'] == 4096:
            greedy_cuts_b0_h4096 = list(rec['cut_positions'])
            break
    return {
        'scope': MODEL_SCOPE,
        'N': N,
        'tiling_endpoints': tiling,
        'tiling_segment_sizes': [tiling[i + 1] - tiling[i] for i in range(len(tiling) - 1)],
        'subjob_count': len(tiling) - 1,
        'segments': segments,
        'source_effective_intervals_total': source_total,
        'dest_effective_intervals_total_aw': dest_total,
        'expected_source_intervals': 24,
        'expected_dest_intervals_aw': 17,
        'expected_subjobs': 9,
        'internal_tiling_points': internal,
        'greedy_cuts_B0_H4096': greedy_cuts_b0_h4096,
        'internal_tiling_points_equal_greedy_cuts_B0_H4096': internal == greedy_cuts_b0_h4096,
        'nominal_fifo_per_dma_bytes': 24,
        'nominal_fifo_two_paths_bytes': 48,
        'fifo_note': 'nominal per-DMA and summed FIFO sizes are static configuration facts '
                     'only; the model B is an assumed static stop capacity and is not '
                     'identified with this FIFO (contract line 34)',
        'ideal_count_note': 'source/dest interval counts are ideal static legalizer '
                            'segmentation counts, not measured requests, TL/PTW traffic, '
                            'job overhead, timing or area (contract line 33)',
    }


# --------------------------------------------------------------------------
# contract checks
# --------------------------------------------------------------------------

def build_contract_checks(group1, group2, witnesses, byte_examples, tiling, counters):
    checks = []

    def add(name, expected, actual, note):
        checks.append({
            'check': name,
            'expected': expected,
            'actual': actual,
            'ok': expected == actual,
            'note': note,
        })

    add('group_1_case_count', EXPECTED_GROUP_1_CASES, group1['cases'],
        'contract line 29: theoretical total 7585')
    add('group_2_case_count', EXPECTED_GROUP_2_CASES, group2['cases'],
        'contract line 30: theoretical total 3805')
    add('group_1_greedy_brute_mismatches', 0, group1['greedy_brute_mismatch'],
        'contract lines 29/35: greedy cardinality must equal the independent oracle')
    add('group_2_greedy_brute_mismatches', 0, group2['greedy_brute_mismatch'],
        'contract lines 30/35: greedy cardinality must equal the independent oracle')
    add('greedy_implementation_errors', 0, counters['greedy_implementation_error'],
        'contract line 19: an existing cut or a non-advancing closure must be reported '
        'as an implementation error, never silently repaired')

    main = witnesses['main_counterexample_N10_B2']
    add('witness_main_greedy_cost', 1, main['greedy']['cost'],
        'contract lines 11/96: N=10, R0={0,10}, D={0,4,6,10}, A=0..10, B=2, H=4 -> 1 cut')
    add('witness_main_brute_cost', 1, main['brute']['cost'],
        'the independent oracle must also find 1')
    add('witness_main_naive_node_cost', 2, main['naive_node_cost_control']['cost'],
        'contract line 11: the old per-node-cost model charges 2')
    add('witness_main_greedy_cuts', [6], main['greedy']['cuts'],
        'the single cut 6 supports q=4 and q=6')

    b0 = witnesses['b0_control_N10_B0']
    add('witness_b0_control_greedy_cost', 2, b0['greedy']['cost'], 'contract line 11')
    add('witness_b0_control_brute_cost', 2, b0['brute']['cost'], 'contract line 11')
    add('witness_b0_control_naive_node_cost', 2, b0['naive_node_cost_control']['cost'],
        'B=0 control: the old per-node model happens to be exact')
    add('witness_b0_control_greedy_cuts', [4, 6], b0['greedy']['cuts'],
        'B=0 needs the two separate cuts 4 and 6')

    infeasible_witnesses = witnesses['infeasible_N10_D_0_5_10_H4_any_B']
    add('witness_infeasible_greedy_all_B', True,
        all(w['greedy']['status'] == 'infeasible' for w in infeasible_witnesses),
        'contract line 31: N=10, D={0,5,10}, H=4 is infeasible for every B')
    add('witness_infeasible_brute_all_B', True,
        all(w['brute']['status'] == 'infeasible' for w in infeasible_witnesses),
        'the independent oracle agrees for every tested B')

    add('software_tiling_subjobs', 9, tiling['subjob_count'],
        'contract line 33: 3 KiB + seven 4 KiB + 1 KiB = 9 subjobs')
    add('software_tiling_source_intervals', 24, tiling['source_effective_intervals_total'],
        'contract line 33: ideal static source interval count 24')
    add('software_tiling_dest_intervals_aw', 17, tiling['dest_effective_intervals_total_aw'],
        'contract line 33: ideal static destination interval / AW count 17')
    add('software_tiling_internal_points_equal_greedy_cuts_B0_H4096', True,
        tiling['internal_tiling_points_equal_greedy_cuts_B0_H4096'],
        'contract line 155: the tiling internal points are exactly the 8 greedy cuts '
        'for B=0, H=4096')

    b0h4096 = None
    for rec in byte_examples:
        if rec['B'] == 0 and rec['H'] == 4096:
            b0h4096 = rec
            break
    if b0h4096 is None:
        add('byte_example_B0_H4096_present', True, False, 'internal consistency')
    else:
        add('byte_example_B0_H4096_source_physical_requests', 24,
            b0h4096['source_physical_requests'],
            'contract line 32: 16 original source bursts + 8 new cuts')
        add('byte_example_B0_H4096_destination_bursts', 17,
            b0h4096['destination_bursts'],
            'contract line 32: 17 destination bursts')
        add('byte_example_B0_H4096_source_matches_tiling_source',
            tiling['source_effective_intervals_total'],
            b0h4096['source_physical_requests'],
            'cross-check between contract items 4 and 5')
        add('byte_example_B0_H4096_dest_matches_tiling_dest',
            tiling['dest_effective_intervals_total_aw'],
            b0h4096['destination_bursts'],
            'cross-check between contract items 4 and 5')

    add('reach_definition_disagreements', 0,
        counters['reach_definition_disagreement'],
        'the independent checkpoint-DAG reachability used by brute and the definitional '
        'closure must agree on every brute witness')
    add('naive_below_brute_violations', 0, counters['naive_below_brute'],
        'the old per-node-cost model can only over-count, never beat the oracle')
    add('greedy_below_brute_violations', 0, counters['greedy_below_brute'],
        'greedy must never beat the exact minimum')

    return checks


def build_hand_derived_expectations(byte_examples):
    '''Hand-derived predictions of the drafting worker (contract lines 32 and 155).

    Informational only: these are not contract-mandated constants and they never
    influence the exit code.
    '''
    predictions = {
        (0, 2048): (16, 18, 2048, 32),
        (0, 4096): (8, 10, 4096, 24),
        (0, 32768): (0, 2, 32768, 16),
        (24, 2048): (16, 18, 2048, 32),
        (24, 4096): (8, 10, 4096, 24),
        (24, 32768): (0, 2, 32768, 16),
        (1024, 2048): (0, 18, 2048, 16),
        (1024, 4096): (0, 18, 2048, 16),
        (1024, 32768): (0, 18, 2048, 16),
    }
    out = []
    for rec in byte_examples:
        key = (rec['B'], rec['H'])
        expected = predictions.get(key)
        if expected is None:
            continue
        got = (rec['new_cuts'], rec['safe_stop_count'], rec['worst_safe_gap'],
               rec['source_physical_requests'])
        out.append({
            'B': rec['B'],
            'H': rec['H'],
            'predicted_new_cuts': expected[0],
            'actual_new_cuts': got[0],
            'predicted_safe_stop_count': expected[1],
            'actual_safe_stop_count': got[1],
            'predicted_worst_safe_gap': expected[2],
            'actual_worst_safe_gap': got[2],
            'predicted_source_physical_requests': expected[3],
            'actual_source_physical_requests': got[3],
            'ok': expected == got,
        })
    return out


# --------------------------------------------------------------------------
# main
# --------------------------------------------------------------------------

def main():
    counters = {
        'greedy_implementation_error': 0,
        'reach_definition_disagreement': 0,
        'naive_below_brute': 0,
        'greedy_below_brute': 0,
    }

    group1 = run_enumeration_group(
        'group_1',
        'contract line 29: N=1..5, A = all integer offsets 0..N, R0 and D each any '
        'interior subset of 0..N with 0/N mandatory, B=0..N-1, H=1..N',
        group_1_cases(), counters)
    group2 = run_enumeration_group(
        'group_2',
        'contract line 30: N=1..4, A any subset of 0..N containing 0/N, R0 any subset '
        'of the interior of A, D any subset of the full grid interior, B=0..N-1, H=1..N',
        group_2_cases(), counters)

    witnesses = run_named_witnesses(
        counters,
        group1['first_naive_diff_case'],
        group2['first_naive_diff_case'])
    byte_examples = run_byte_examples()
    tiling = run_software_tiling(byte_examples)
    checks = build_contract_checks(group1, group2, witnesses, byte_examples, tiling, counters)
    hand_expectations = build_hand_derived_expectations(byte_examples)

    total_cases = group1['cases'] + group2['cases']
    total_mismatch = group1['greedy_brute_mismatch'] + group2['greedy_brute_mismatch']
    failed_checks = [c['check'] for c in checks if not c['ok']]
    greedy_brute_all_match = (group1['greedy_brute_mismatch'] == 0
                              and group2['greedy_brute_mismatch'] == 0)
    exit_code = 0 if (greedy_brute_all_match and not failed_checks) else 1

    result = {
        'model_scope': MODEL_SCOPE,
        'script': 'out/fragment-drain-boundary-20261008/geometry_oracle.py',
        'contract_source': CONTRACT_SOURCE,
        'supporting_notes_source': PROOF_SOURCE,
        'run_mode': 'single-shot static computation, executed and redirected once by Codex',
        'scope_disclaimers': [
            'pure static geometry over byte offsets; not a DUT, not RTL, not a simulation run',
            'no real request lookahead, no multiple W-head ordering, no shared spill parking '
            'and no translation blocking are modelled (contract line 13)',
            'no real independent-stop capacity is claimed; B=24/1024 are assumed static '
            'capacities only (contract line 32)',
            'no TL/PTW request counts, timing, cycles, area, CPU supply cost or macrocopy cost '
            'are computed or claimed',
            'H only bounds the distance between selected destination checkpoints; it is not a '
            'cycle budget or a DDR/TC/translation budget (contract line 50)',
            'the two enumeration groups are two deliberately different input covers, not '
            'independent DUT repetitions (contract line 30)',
            'source 24 / AW 17 and the 9-subjob tiling are ideal static legalizer segmentation '
            'counts, not measured request counts (contract line 33)',
            'geometric feasibility here does not imply that real RTL can stop at those byte '
            'offsets (contract lines 5-13 and 134-145)',
        ],
        'model_definition': {
            'scope': MODEL_SCOPE,
            'coordinates': 'byte offsets inside one byte-preserving copy [0, N]; source and '
                           'destination physical addresses are not modelled',
            'N': 'positive integer length in byte units',
            'R0': 'mandatory source endpoints, contains 0 and N; never modified or deleted',
            'D': 'destination completion endpoints, contains 0 and N',
            'A': 'legal new source cut endpoints, contains 0 and N and covers R0',
            'C': 'new cuts, subset of A minus R0, each new cut costs exactly 1',
            'B': 'assumed static independent stop capacity in the same byte units; no real '
                 'independent-stop capacity is claimed (contract line 34)',
            'H': 'upper bound on the distance between consecutive selected destination '
                 'checkpoints only; no backpressure or pre-accepted lookahead is modelled '
                 '(contract lines 9 and 50)',
            'support': 'q in D is supported by endpoint set R iff some p in R satisfies '
                       'q <= p <= min(q+B, N)',
            'closure': 'walk sorted supported checkpoints from 0 while the next is within H '
                       'of the last reachable one; return the largest reachable endpoint',
            'greedy': 'close the free prefix; if it reaches N stop; otherwise pick the '
                      'largest SUPPORTABLE destination checkpoint q* inside (x, x+H] and add '
                      'the right-most legal cut of I(q*) = [q*, min(q*+B, N)] '
                      '(contract line 19)',
            'brute': 'independent oracle: enumerate all subsets of A minus R0 by increasing '
                     'size and test feasibility with an independent checkpoint-DAG '
                     'reachability scan; never calls greedy and never reuses a greedy '
                     'decision (contract line 23)',
            'naive_node_cost': 'old control model only: node cost 0 if a mandatory R0 endpoint '
                               'supports the checkpoint, else 1 if any legal A endpoint '
                               'supports it; shortest path over the gap<=H DAG; it does not '
                               'carry the support range of already chosen cuts and may '
                               'over-count (contract line 25)',
        },
        'no_artificial_budget': {
            'enumeration_truncated': False,
            'time_limit_applied': False,
            'case_cap_applied': False,
            'max_default_mode': True,
            'note': 'both finite groups are enumerated in full; no deliberately reduced '
                    'budget is used',
        },
        'case_counts': {
            'group_1': {
                'group': group1['group'],
                'description': group1['description'],
                'cases': group1['cases'],
                'expected_cases': EXPECTED_GROUP_1_CASES,
                'case_count_matches_expected': group1['cases'] == EXPECTED_GROUP_1_CASES,
                'feasible_greedy': group1['feasible_greedy'],
                'infeasible_greedy': group1['infeasible_greedy'],
                'feasible_brute': group1['feasible_brute'],
                'infeasible_brute': group1['infeasible_brute'],
                'greedy_brute_mismatch': group1['greedy_brute_mismatch'],
                'naive_vs_brute_differing_cases': group1['naive_vs_brute_differing_cases'],
            },
            'group_2': {
                'group': group2['group'],
                'description': group2['description'],
                'cases': group2['cases'],
                'expected_cases': EXPECTED_GROUP_2_CASES,
                'case_count_matches_expected': group2['cases'] == EXPECTED_GROUP_2_CASES,
                'feasible_greedy': group2['feasible_greedy'],
                'infeasible_greedy': group2['infeasible_greedy'],
                'feasible_brute': group2['feasible_brute'],
                'infeasible_brute': group2['infeasible_brute'],
                'greedy_brute_mismatch': group2['greedy_brute_mismatch'],
                'naive_vs_brute_differing_cases': group2['naive_vs_brute_differing_cases'],
            },
            'combined': {
                'cases': total_cases,
                'feasible_greedy': group1['feasible_greedy'] + group2['feasible_greedy'],
                'infeasible_greedy': group1['infeasible_greedy'] + group2['infeasible_greedy'],
                'feasible_brute': group1['feasible_brute'] + group2['feasible_brute'],
                'infeasible_brute': group1['infeasible_brute'] + group2['infeasible_brute'],
                'greedy_brute_mismatch': total_mismatch,
            },
            'note': 'groups 1 and 2 are two deliberately different input covers, not '
                    'independent DUT repetitions (contract line 30)',
        },
        'mismatch_detail': {
            'total_greedy_brute_mismatches': total_mismatch,
            'max_records_kept_per_group': MAX_MISMATCH_RECORDS,
            'group_1_first_20': group1['greedy_brute_mismatch_records_first_20'],
            'group_2_first_20': group2['greedy_brute_mismatch_records_first_20'],
            'exit_rule': 'greedy/brute cost equal in every case -> exit 0; any difference -> '
                         'exit 1 (contract line 35)',
        },
        'selected_witnesses': witnesses,
        'byte_scale_representative_examples': {
            'scope': MODEL_SCOPE,
            'contract_reference': 'contract lines 32 and 155',
            'N': 32768,
            'A': 'all 8-byte aligned offsets in [0, 32768] (legal new cut grid)',
            'R0': '0, 2048, 4096, ..., 32768 (17 endpoints, 16 source bursts)',
            'D': '0, 1024, 3072, ..., 31744, 32768 (18 endpoints, 17 destination bursts)',
            'method': 'plain greedy only; the independent all-subsets oracle is not run at '
                      'this size (contract line 32)',
            'records': byte_examples,
            'notes': [
                'source_physical_requests = 16 + number_of_new_cuts; destination_bursts = 17',
                'B=0/24/1024 and H=2048/4096/32768 are nine pure static parameter points; no '
                'real independent-stop capacity, cycle budget or drain time is claimed',
            ],
        },
        'software_tiling_static_counts': tiling,
        'contract_checks': checks,
        'structural_invariants': {
            'greedy_implementation_error_count': counters['greedy_implementation_error'],
            'reach_definition_disagreement_count': counters['reach_definition_disagreement'],
            'naive_below_brute_count': counters['naive_below_brute'],
            'greedy_below_brute_count': counters['greedy_below_brute'],
            'note': 'counts expected to be 0; a non-zero value means an internal '
                    'inconsistency or a violation of the contract model and is reported '
                    'instead of being repaired',
        },
        'hand_derived_expectations': {
            'note': 'hand-derived predictions of the drafting worker for the byte-scale '
                    'examples (contract lines 32 and 155); informational only, never '
                    'exit-code relevant',
            'checks': hand_expectations,
        },
        'summary': {
            'scope': MODEL_SCOPE,
            'greedy_brute_all_cases_match': greedy_brute_all_match,
            'greedy_brute_total_mismatches': total_mismatch,
            'contract_checks_all_ok': not failed_checks,
            'failed_checks': failed_checks,
            'exit_code': exit_code,
            'exit_code_rule': '0 only if greedy and brute agree on every enumerated case AND '
                              'all contract checks pass; otherwise 1. Computed values are '
                              'reported as found and never patched (contract line 35).',
            'no_dut_tl_ptw_time_area_claim': True,
        },
    }

    json.dump(result, sys.stdout, indent=2)
    sys.stdout.write('\n')
    return exit_code


if __name__ == '__main__':
    try:
        code = main()
    except Exception:
        json.dump({
            'model_scope': MODEL_SCOPE,
            'script': 'out/fragment-drain-boundary-20261008/geometry_oracle.py',
            'fatal_error': 'unhandled exception; no partial geometry result is claimed',
            'traceback': traceback.format_exc(),
            'exit_code': 1,
        }, sys.stdout, indent=2)
        sys.stdout.write('\n')
        sys.exit(1)
    sys.exit(code)
