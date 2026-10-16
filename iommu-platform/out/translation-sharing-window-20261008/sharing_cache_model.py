#!/usr/bin/env python3
"""
sharing_cache_model.py

有限逻辑译址请求 trace 的缓存/步调模型。
只统计 miss 与 PTE 读次数；不是 DUT、cycle、速度或完整 copy 实验模型。
"""

import argparse
import json
import sys
from bisect import bisect_right
from collections import OrderedDict
from functools import lru_cache

INF = 10**9
FLOWS = ('A', 'V')
PAGES = 8
E_VALUES = (4, 8, 16)


def key_for(flow, kind, page):
    """合同规定的 cache key：
    - 源输入共享：('S', page)
    - 目的输出独立：('D', flow, page)
    """
    if kind == 'S':
        return ('S', page)
    if kind == 'D':
        return ('D', flow, page)
    raise ValueError(f"unknown kind: {kind}")


def make_flow_requests(flow, pages):
    """每 page 发 D,S,D,S（两 2KiB bursts）。"""
    reqs = []
    for p in pages:
        reqs.append((flow, 'D', p))
        reqs.append((flow, 'S', p))
        reqs.append((flow, 'D', p))
        reqs.append((flow, 'S', p))
    return reqs


def trace_to_keys(seq):
    return [key_for(f, k, p) for f, k, p in seq]


def check_flow_order(seq):
    """检查每个 flow 的 page 顺序为 0..7，每 page 为 D,S,D,S。"""
    for flow in FLOWS:
        flow_reqs = [(f, k, p) for f, k, p in seq if f == flow]
        expected_pages = list(range(PAGES))
        pages_seen = []
        i = 0
        while i < len(flow_reqs):
            f, k, p = flow_reqs[i]
            if k != 'D':
                return False, f"flow {flow} request {i} is {k}, expected D"
            if i + 3 >= len(flow_reqs):
                return False, f"flow {flow} truncated at {i}"
            if flow_reqs[i + 1][1] != 'S' or flow_reqs[i + 1][2] != p:
                return False, f"flow {flow} page {p} expected S after D"
            if flow_reqs[i + 2][1] != 'D' or flow_reqs[i + 2][2] != p:
                return False, f"flow {flow} page {p} expected second D"
            if flow_reqs[i + 3][1] != 'S' or flow_reqs[i + 3][2] != p:
                return False, f"flow {flow} page {p} expected second S"
            pages_seen.append(p)
            i += 4
        if pages_seen != expected_pages:
            return False, f"flow {flow} page order {pages_seen} != {expected_pages}"
    return True, "ok"


def compute_peak_source_gap(seq):
    """peak_source_gap 定义为任一时刻 |A 已完成 page 数 - V 已完成 page 数| 的最大值。
    合同给出的 lead 为：trace1=8，trace2=4，trace3=1。
    """
    completed = {f: 0 for f in FLOWS}
    count = {f: 0 for f in FLOWS}
    peak = 0
    for f, k, p in seq:
        count[f] += 1
        if count[f] % 4 == 0:
            completed[f] += 1
            gap = abs(completed['A'] - completed['V'])
            if gap > peak:
                peak = gap
    return peak


def build_traces():
    """三种 trace schedule。"""
    traces = {}

    # 1) A 整 job 后 V
    seq = []
    seq += make_flow_requests('A', list(range(PAGES)))
    seq += make_flow_requests('V', list(range(PAGES)))
    traces['A_full_then_V'] = seq

    # 2) A 先 4 page，再 V/A 各 1 page 交替，A 结束后补 V
    #    A: 0,1,2,3,4,5,6,7
    #    V: 0,1,2,3,4,5,6,7
    #    交替段: V0,A4, V1,A5, V2,A6, V3,A7
    seq = []
    seq += make_flow_requests('A', [0, 1, 2, 3])
    for i in range(4):
        seq += make_flow_requests('V', [i])
        seq += make_flow_requests('A', [i + 4])
    seq += make_flow_requests('V', [4, 5, 6, 7])
    traces['A_first4_then_VA_alt_then_V'] = seq

    # 3) A/V 每 page 配对：A0,V0,A1,V1,...
    seq = []
    for i in range(PAGES):
        seq += make_flow_requests('A', [i])
        seq += make_flow_requests('V', [i])
    traces['A_V_paired_per_page'] = seq

    return traces


def run_lru(seq, capacity):
    """普通全局 LRU，正常插入。"""
    cache = OrderedDict()
    misses = 0
    perflow = {f: 0 for f in FLOWS}
    for f, kind, page in seq:
        k = key_for(f, kind, page)
        if k in cache:
            cache.move_to_end(k)
        else:
            misses += 1
            perflow[f] += 1
            if len(cache) >= capacity:
                cache.popitem(last=False)
            cache[k] = None
            cache.move_to_end(k)
    return misses, perflow


def build_flow_seqs(seq):
    """每个 flow 自身的 key 序列，用于 registered-use 的 per-flow remaining 计数。"""
    flow_seqs = {f: [] for f in FLOWS}
    for f, kind, page in seq:
        flow_seqs[f].append(key_for(f, kind, page))
    return flow_seqs


def remaining_uses(k, current_flow, pos, flow_seqs):
    """在不读取未来 global next-use 排序的前提下，用已登记每 flow 规则/当前 index
    计算 key k 在当前请求之后的剩余 lookup 次数。
    """
    total = 0
    for f in FLOWS:
        start = pos[f]
        if f == current_flow:
            start += 1  # 当前请求已消耗
        if start < len(flow_seqs[f]):
            total += flow_seqs[f][start:].count(k)
    return total


def run_registered_use(seq, capacity):
    """registered-use dead-first/LRU：
    - 已登记每 flow 规则/当前 position 推导剩余用途；
    - incoming 无剩余用途则不填充；
    - cache full 先选无剩余用途者，再 LRU。
    """
    flow_seqs = build_flow_seqs(seq)
    cache = OrderedDict()
    misses = 0
    perflow = {f: 0 for f in FLOWS}
    pos = {f: 0 for f in FLOWS}

    for f, kind, page in seq:
        k = key_for(f, kind, page)
        rem_after = remaining_uses(k, f, pos, flow_seqs)

        if k in cache:
            cache.move_to_end(k)
        else:
            misses += 1
            perflow[f] += 1
            if rem_after > 0:
                if len(cache) >= capacity:
                    dead = []
                    for ck in cache:
                        if remaining_uses(ck, f, pos, flow_seqs) == 0:
                            dead.append(ck)
                    if dead:
                        victim = dead[0]  # OrderedDict 迭代为 LRU -> MRU
                    else:
                        victim = next(iter(cache))
                    del cache[victim]
                cache[k] = None
                cache.move_to_end(k)
        pos[f] += 1

    return misses, perflow


def run_private_dest_shared_src(seq, capacity):
    """2 个 private destination last-page slot + (E-2) 个 shared source LRU slot。
    所有 physical mappings 总数 E。
    D(flow,page) 只进对应 flow 的 private slot；S(page) 进 shared LRU。
    """
    assert capacity >= 4, "E 至少为 4，保证 E-2 >= 2"
    shared_cap = capacity - 2
    private = {f: None for f in FLOWS}
    shared = OrderedDict()
    misses = 0
    perflow = {f: 0 for f in FLOWS}

    for f, kind, page in seq:
        if kind == 'D':
            k = ('D', f, page)
            if private[f] == k:
                pass
            else:
                misses += 1
                perflow[f] += 1
                private[f] = k
        else:  # 'S'
            k = ('S', page)
            if k in shared:
                shared.move_to_end(k)
            else:
                misses += 1
                perflow[f] += 1
                if len(shared) >= shared_cap:
                    shared.popitem(last=False)
                shared[k] = None
                shared.move_to_end(k)

    return misses, perflow


def run_belady_optional(seq, capacity):
    """full-trace offline Belady with optional admission。
    可读取完整未来 trace，因此只作为额外信息 lower-bound，不作为同信息 candidate。
    """
    keys = trace_to_keys(seq)
    occ = {}
    for i, k in enumerate(keys):
        occ.setdefault(k, []).append(i)

    def next_use(k, i):
        lst = occ.get(k, [])
        p = bisect_right(lst, i)
        if p < len(lst):
            return lst[p]
        return INF

    cache = set()
    misses = 0
    perflow = {f: 0 for f in FLOWS}

    for i, (f, kind, page) in enumerate(seq):
        k = keys[i]
        if k in cache:
            continue

        misses += 1
        perflow[f] += 1
        nu = next_use(k, i)

        if nu == INF:
            # incoming 无未来用途，optional admission 选择 bypass
            continue

        if len(cache) < capacity:
            cache.add(k)
        else:
            victim = None
            victim_nu = -1
            for ck in cache:
                cnu = next_use(ck, i)
                if cnu > victim_nu:
                    victim_nu = cnu
                    victim = ck
            if nu > victim_nu:
                # incoming 比 victim 还远，bypass 更优
                continue
            cache.remove(victim)
            cache.add(k)

    return misses, perflow


def build_small_n2_trace():
    """独立小 N=2 trace，用于 DP 验证 offline Belady。"""
    seq = []
    seq += make_flow_requests('A', [0])
    seq += make_flow_requests('V', [0])
    seq += make_flow_requests('A', [1])
    seq += make_flow_requests('V', [1])
    return seq


def dp_optimal_misses(seq, capacity):
    """独立 memo DP，枚举合法 evict/bypass，求最小 miss 数。"""
    keys = trace_to_keys(seq)

    @lru_cache(maxsize=None)
    def dp(i, cache_tuple):
        if i == len(keys):
            return 0

        k = keys[i]
        cache = set(cache_tuple)

        if k in cache:
            return dp(i + 1, tuple(sorted(cache)))

        # miss: 先考虑 bypass
        best = 1 + dp(i + 1, tuple(sorted(cache)))

        if len(cache) < capacity:
            new_cache = cache | {k}
            best = min(best, 1 + dp(i + 1, tuple(sorted(new_cache))))
        else:
            for victim in cache:
                new_cache = (cache - {victim}) | {k}
                best = min(best, 1 + dp(i + 1, tuple(sorted(new_cache))))

        return best

    return dp(0, tuple())


def main():
    parser = argparse.ArgumentParser(
        description="Sharing cache model: miss/PTE only, no DUT/cycle claims."
    )
    parser.add_argument('--output', required=True, help='JSON output path')
    args = parser.parse_args()

    traces = build_traces()

    trace_meta = {}
    for name, seq in traces.items():
        ok, msg = check_flow_order(seq)
        assert ok, f"flow order check failed for {name}: {msg}"

        keys = trace_to_keys(seq)
        unique = len(set(keys))
        assert unique == 24, f"{name}: unique_keys {unique} != 24"

        peak = compute_peak_source_gap(seq)
        trace_meta[name] = {
            'seq': seq,
            'unique_keys': unique,
            'request_count': len(seq),
            'peak_source_gap': peak,
            'trace_strings': [f"{f}:{k}{p}" for f, k, p in seq],
            'flow_order_check': {f: True for f in FLOWS},
        }

    cases = []

    for name, meta in trace_meta.items():
        seq = meta['seq']
        for E in E_VALUES:
            oracle_misses, oracle_perflow = run_belady_optional(seq, E)

            assert oracle_misses >= meta['unique_keys'], (
                f"{name} E={E}: oracle {oracle_misses} < unique {meta['unique_keys']}"
            )

            policies = [
                ('LRU', run_lru(seq, E)),
                ('registered_use_dead_first_no_alloc', run_registered_use(seq, E)),
                ('private_dest_lastpage_shared_src_lru', run_private_dest_shared_src(seq, E)),
                ('offline_belady_optional_admission', (oracle_misses, oracle_perflow)),
            ]

            for pol_name, (misses, perflow) in policies:
                if pol_name != 'offline_belady_optional_admission':
                    assert misses >= oracle_misses, (
                        f"{name} E={E} {pol_name}: misses {misses} < oracle {oracle_misses}"
                    )

                assert sum(perflow.values()) == misses, (
                    f"{name} E={E} {pol_name}: perflow sum {sum(perflow.values())} != misses {misses}"
                )
                hits = len(seq) - misses
                rec = {
                    'trace_name': name,
                    'capacity': E,
                    'policy': pol_name,
                    'is_oracle_extra_info': pol_name == 'offline_belady_optional_admission',
                    'request_count': len(seq),
                    'unique_keys': meta['unique_keys'],
                    'compulsory_bound': meta['unique_keys'],
                    'misses': misses,
                    'hits': hits,
                    'perflow_misses': perflow,
                    'PTE_reads': 3 * misses,
                    'peak_source_gap': meta['peak_source_gap'],
                    'flow_order_check': meta['flow_order_check'],
                    'request_trace': meta['trace_strings'],
                }
                cases.append(rec)

    assert len(cases) == 36, f"cases {len(cases)} != 36"
    small_seq = build_small_n2_trace()
    small_keys = trace_to_keys(small_seq)
    small_unique = len(set(small_keys))

    verification = {}
    for E in (2, 3):
        greedy_misses, greedy_perflow = run_belady_optional(small_seq, E)
        dp_misses = dp_optimal_misses(small_seq, E)

        assert greedy_misses == dp_misses, (
            f"small N2 E={E}: greedy {greedy_misses} != DP {dp_misses}"
        )
        assert greedy_misses >= small_unique, (
            f"small N2 E={E}: greedy {greedy_misses} < unique {small_unique}"
        )

        verification[f'E{E}'] = {
            'capacity': E,
            'request_count': len(small_seq),
            'unique_keys': small_unique,
            'compulsory_bound': small_unique,
            'greedy_belady_misses': greedy_misses,
            'dp_optimal_misses': dp_misses,
            'equal': greedy_misses == dp_misses,
            'perflow_misses': greedy_perflow,
            'PTE_reads': 3 * greedy_misses,
            'request_trace': [f"{f}:{k}{p}" for f, k, p in small_seq],
        }

    out = {
        'meta': {
            'model': 'sharing_cache_model.py',
            'note': '有限逻辑译址请求 trace 的 miss/PTE 模型；不是 DUT/cycle/速度模型。',
            'flows': list(FLOWS),
            'pages_per_flow': PAGES,
            'bursts_per_page': 2,
            'requests_per_page': 4,
            'pte_per_cold_leaf': 3,
            'capacities': list(E_VALUES),
            'unique_mappings_total': 24,
        },
        'cases': cases,
        'verification': verification,
    }

    with open(args.output, 'w', encoding='utf-8') as f:
        json.dump(out, f, indent=2, ensure_ascii=False)

    print(f"wrote {args.output}", file=sys.stderr)


if __name__ == '__main__':
    main()
