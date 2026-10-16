"""Six policies for c-model-contract.json.

All policies use the same common executor from c_event_model.
"""
from __future__ import annotations

import itertools
from typing import Dict, List, Set, Tuple, Any, Optional

from c_event_model import (
    Fixture,
    STATUS_OK,
    build_case,
    simulate,
)


def _remaining_set(case: Fixture, order: List[int]) -> Set[int]:
    return set(range(len(case.descs))) - set(order)


def _group_remaining_keys(case: Fixture, remaining: Set[int], g: int) -> Set[str]:
    return {case.descs[d].key for d in remaining if case.descs[d].group == g}


def _order_by_pages(case: Fixture, desc_ids: List[int], page_order: List[str]) -> List[int]:
    result: List[int] = []
    for key in page_order:
        part = [d for d in desc_ids if case.descs[d].key == key]
        part.sort(key=lambda d: (case.descs[d].group, case.descs[d].pos))
        result.extend(part)
    for d in desc_ids:
        if d not in result:
            result.append(d)
    return result


def _incremental_safe_batch(
    case: Fixture,
    prefix: List[int],
    batch: List[int],
    overlap: bool,
) -> List[int]:
    """Apply the contract's incoming-slot reserve using the common executor.

    An addition that completes its own group may occupy the last slot.
    An addition that does not complete its group must leave one free slot
    at the time of its actual dispatch, counting in-flight/live inputs.
    """
    safe: List[int] = []
    for did in batch:
        trial = prefix + safe + [did]
        res = simulate(case, trial, overlap=overlap, drain=False)
        if res["status"] != STATUS_OK:
            break
        g = case.descs[did].group
        if res["ready"][g] is not None:
            safe.append(did)
            continue
        used_before = res.get("used_before_dispatch", {}).get(did)
        if used_before is None:
            used_before = res.get("used", 0)
        if used_before + 1 <= case.B - 1:
            safe.append(did)
        else:
            break
    return safe


def _partial_group_fallback(
    case: Fixture,
    prefix: List[int],
    remaining: Set[int],
    overlap: bool,
) -> List[int]:
    """Prefer a page that closes the oldest partial group, then any feasible group."""
    if not remaining:
        return []
    prefix_set = set(prefix)
    res_prefix = simulate(case, prefix, overlap=overlap, drain=False)
    if res_prefix["status"] != STATUS_OK:
        return []

    partial: List[Tuple[int, List[int]]] = []
    for g in range(len(case.groups)):
        if res_prefix["ready"][g] is not None:
            continue
        has_prefix = any(case.descs[d].group == g for d in prefix_set)
        if not has_prefix:
            continue
        rem = [d for d in remaining if case.descs[d].group == g]
        if rem:
            partial.append((g, rem))

    for g, rem in sorted(partial, key=lambda x: x[0]):
        rem_sorted = sorted(
            rem,
            key=lambda d: (case.descs[d].key, case.descs[d].group, case.descs[d].pos),
        )
        res = simulate(case, prefix + rem_sorted, overlap=overlap, drain=False)
        if res["status"] == STATUS_OK:
            return rem_sorted

    groups_with_rem = sorted({case.descs[d].group for d in remaining})
    for g in groups_with_rem:
        rem = sorted(
            [d for d in remaining if case.descs[d].group == g],
            key=lambda d: (case.descs[d].key, case.descs[d].pos),
        )
        res = simulate(case, prefix + rem, overlap=overlap, drain=False)
        if res["status"] == STATUS_OK:
            return rem

    return []


def _select_page_batch(
    case: Fixture,
    order: List[int],
    remaining: Set[int],
    selector: str,
) -> List[int]:
    if not remaining:
        return []
    if selector == "group_first":
        groups_with_rem = sorted({case.descs[d].group for d in remaining})
        g = groups_with_rem[0]
        g_rem = [d for d in remaining if case.descs[d].group == g]
        g_rem.sort(key=lambda d: case.descs[d].pos)
        key = case.descs[g_rem[0]].key
    elif selector == "hot_page":
        counts: Dict[str, int] = {}
        first: Dict[str, Tuple[int, int]] = {}
        for d in remaining:
            k = case.descs[d].key
            counts[k] = counts.get(k, 0) + 1
            pos = (case.descs[d].group, case.descs[d].pos)
            if k not in first or pos < first[k]:
                first[k] = pos
        key = sorted(
            counts.keys(),
            key=lambda k: (-counts[k], first[k][0], first[k][1]),
        )[0]
    else:
        raise ValueError(f"unknown selector {selector}")

    batch = [d for d in remaining if case.descs[d].key == key]
    batch.sort(key=lambda d: (case.descs[d].group, case.descs[d].pos))
    return batch


def _plan_page_policy(case: Fixture, overlap: bool, selector: str) -> List[int]:
    order: List[int] = []
    remaining = set(range(len(case.descs)))

    while remaining:
        batch = _select_page_batch(case, order, remaining, selector)
        if not batch:
            batch = [min(remaining)]

        safe = _incremental_safe_batch(case, order, batch, overlap)

        # If the selected page-batch was truncated by the reserve rule,
        # prefer a page that closes the oldest partial group before moving on.
        if len(safe) < len(batch):
            extra = _partial_group_fallback(
                case, order + safe, remaining - set(safe), overlap
            )
            if extra:
                safe = safe + extra

        if not safe:
            safe = _partial_group_fallback(case, order, remaining, overlap)

        if not safe:
            for did in sorted(remaining):
                res = simulate(case, order + [did], overlap=overlap, drain=False)
                if res["status"] == STATUS_OK:
                    safe = [did]
                    break

        if not safe:
            safe = [min(remaining)]

        order.extend(safe)
        for d in safe:
            remaining.remove(d)

    return order


def policy_group_first(case: Fixture | str, overlap: bool = False) -> List[int]:
    if isinstance(case, str):
        case = build_case(case)
    return _plan_page_policy(case, overlap, "group_first")


def policy_hot_page(case: Fixture | str, overlap: bool = False) -> List[int]:
    if isinstance(case, str):
        case = build_case(case)
    return _plan_page_policy(case, overlap, "hot_page")


def _candidate_rank(
    case: Fixture,
    res_prefix: Dict[str, Any],
    candidate_tuple: Tuple[str, List[int], Set[str], List[int], Dict[str, Any]],
    mode: str,
) -> Tuple[Any, ...]:
    _mode, P, union, cand, res = candidate_tuple
    newly_ready = sum(
        1
        for g in P
        if res["ready"][g] is not None and res_prefix["ready"][g] is None
    )
    oldest = min(P) if P else 0

    if mode == "max_closure":
        cold = len([k for k in union if k not in res_prefix["cache"]])
        return (-newly_ready, cold, len(cand), oldest, tuple(cand))

    if mode == "setup_density":
        cold = len([k for k in union if k not in res_prefix["cache"]])
        cost = cold * case.L + len(cand) * case.d
        score = newly_ready / cost if cost > 0 else 0.0
        return (-score, oldest, cold, tuple(cand))

    if mode == "closure_ack_density":
        duration = res["last_copy_time"] - res_prefix["last_copy_time"]
        if duration <= 0:
            duration = 1e-9
        score = newly_ready / duration
        extra_walks = (
            res["metrics"]["source_walks"] - res_prefix["metrics"]["source_walks"]
        )
        return (-score, oldest, extra_walks, tuple(cand))

    return (0,)


def _iter_union_candidates(
    case: Fixture,
    prefix: List[int],
    remaining: Set[int],
    union: Set[str],
    overlap: bool,
):
    groups_with_rem = [
        g for g in range(len(case.groups))
        if any(case.descs[d].group == g for d in remaining)
    ]
    eligible = [
        g for g in groups_with_rem
        if _group_remaining_keys(case, remaining, g).issubset(union)
    ]
    eligible.sort()

    for k in range(1, len(eligible) + 1):
        P = eligible[:k]
        desc_ids = [d for d in remaining if case.descs[d].group in P]
        if not desc_ids:
            continue
        for po in itertools.permutations(sorted(union)):
            cand = _order_by_pages(case, desc_ids, list(po))
            res = simulate(case, prefix + cand, overlap=overlap, drain=False)
            if res["status"] != STATUS_OK:
                continue
            yield P, cand, res


def _plan_union_policy(case: Fixture, overlap: bool, mode: str) -> List[int]:
    order: List[int] = []
    remaining = set(range(len(case.descs)))

    while remaining:
        res_prefix = simulate(case, order, overlap=overlap, drain=False)
        if res_prefix["status"] != STATUS_OK:
            return order

        candidates: List[Tuple[str, List[int], Set[str], List[int], Dict[str, Any]]] = []

        if mode in ("max_closure", "setup_density"):
            keys = sorted({case.descs[d].key for d in remaining})
            for size in (1, 2):
                for union_tuple in itertools.combinations(keys, size):
                    union = set(union_tuple)
                    for P, cand, res in _iter_union_candidates(
                        case, order, remaining, union, overlap
                    ):
                        candidates.append((mode, P, union, cand, res))

        elif mode == "closure_ack_density":
            buckets: Dict[frozenset, List[int]] = {}
            groups_with_rem = [
                g for g in range(len(case.groups))
                if any(case.descs[d].group == g for d in remaining)
            ]
            for g in groups_with_rem:
                S = frozenset(_group_remaining_keys(case, remaining, g))
                if len(S) <= 2:
                    buckets.setdefault(S, []).append(g)

            for S, gs in buckets.items():
                gs_sorted = sorted(gs)
                for k in range(1, len(gs_sorted) + 1):
                    P = gs_sorted[:k]
                    desc_ids = [d for d in remaining if case.descs[d].group in P]
                    if not desc_ids:
                        continue
                    for po in itertools.permutations(sorted(S)):
                        cand = _order_by_pages(case, desc_ids, list(po))
                        res = simulate(case, order + cand, overlap=overlap, drain=False)
                        if res["status"] != STATUS_OK:
                            continue
                        candidates.append((mode, P, set(S), cand, res))

        else:
            raise ValueError(f"unknown union mode {mode}")

        if not candidates:
            did = min(remaining)
            order.append(did)
            remaining.remove(did)
            continue

        best = min(candidates, key=lambda c: _candidate_rank(case, res_prefix, c, mode))
        _m, _P, _union, cand, _res = best
        cand = [d for d in cand if d in remaining]
        if not cand:
            did = min(remaining)
            order.append(did)
            remaining.remove(did)
            continue

        order.extend(cand)
        for d in cand:
            remaining.remove(d)

    return order


def policy_closure_ack_density(case: Fixture | str, overlap: bool = False) -> List[int]:
    if isinstance(case, str):
        case = build_case(case)
    return _plan_union_policy(case, overlap, "closure_ack_density")


def policy_max_closure(case: Fixture | str, overlap: bool = False) -> List[int]:
    if isinstance(case, str):
        case = build_case(case)
    return _plan_union_policy(case, overlap, "max_closure")


def policy_setup_density(case: Fixture | str, overlap: bool = False) -> List[int]:
    if isinstance(case, str):
        case = build_case(case)
    return _plan_union_policy(case, overlap, "setup_density")


def policy_marginal_unlock_proxy(case: Fixture | str, overlap: bool = False) -> List[int]:
    """DARTS-inspired proxy only.

    This is not a faithful StarPU/DARTS implementation. It only maps the idea
    "setup/transfer cost divided by immediately unlocked consumer work" onto the
    same authority/cache/copy/ACK resource contract.
    """
    if isinstance(case, str):
        case = build_case(case)

    order: List[int] = []
    remaining = set(range(len(case.descs)))

    while remaining:
        res_prefix = simulate(case, order, overlap=overlap, drain=False)
        if res_prefix["status"] != STATUS_OK:
            return order

        best = None  # (rank, cand)

        # One-key batches.
        keys = sorted({case.descs[d].key for d in remaining})
        for K in keys:
            cand = [d for d in remaining if case.descs[d].key == K]
            cand.sort(key=lambda d: (case.descs[d].group, case.descs[d].pos))
            if not cand:
                continue
            res = simulate(case, order + cand, overlap=overlap, drain=False)
            if res["status"] != STATUS_OK:
                continue
            unlocked_groups = [
                g for g in range(len(case.groups))
                if res["ready"][g] is not None and res_prefix["ready"][g] is None
            ]
            work = sum(case.consumer for _g in unlocked_groups)
            if work <= 0:
                continue
            cost = (0.0 if K in res_prefix["cache"] else case.L) + len(cand) * case.d
            score = cost / work
            oldest = min(case.descs[d].group for d in cand)
            rank = (score, oldest, tuple(cand))
            if best is None or rank < best[0]:
                best = (rank, cand)

        if best is not None:
            cand = best[1]
            order.extend(cand)
            for d in cand:
                remaining.remove(d)
            continue

        # Fallback: oldest feasible one-group two-key closure.
        groups_with_rem = sorted({case.descs[d].group for d in remaining})
        found = False
        for g in groups_with_rem:
            rem = [d for d in remaining if case.descs[d].group == g]
            if len({case.descs[d].key for d in rem}) > 2:
                continue
            cand = sorted(rem, key=lambda d: (case.descs[d].key, case.descs[d].pos))
            res = simulate(case, order + cand, overlap=overlap, drain=False)
            if res["status"] == STATUS_OK:
                order.extend(cand)
                for d in cand:
                    remaining.remove(d)
                found = True
                break
        if found:
            continue

        did = min(remaining)
        order.append(did)
        remaining.remove(did)

    return order
