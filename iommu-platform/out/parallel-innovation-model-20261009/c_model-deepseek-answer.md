Below are four complete Python blocks. They implement the contract in `c-model-contract.json` as an abstract finite scheduling model. I did **not** run them; the included `unittest` suite is the meaningful verification the root should execute. The DARTS C code is used only as an explicit proxy boundary, not as a claim of faithful DARTS reproduction.

Assumptions/limits:
- Descriptor IDs are global flattening of group declarations in order.
- `ready`/`ack` arrays are returned in fixture group order. The contract’s YZ-first hand text appears to list `[G1,G2,G3,X]`; tests therefore compare by group name where the order is ambiguous.
- Source cache is a small LRU keyed by exact source key. `cap` is the number of cached source translations.
- Buffer release happens only at consumer ACK. A full buffer with no scheduled ACK and no in-flight copy that can complete a group is reported `infeasible`.
- Overlap is exactly one immediate-successor source-walk lookahead, shared by all policies.
- No RTL/board/software-build scope is added.

Acceptance commands after writing the files:
```bash
python3 -m unittest -v test_c_model.py
python3 run_c_model.py > c_model_runs.json
```

```python c_event_model.py
"""Abstract finite scheduling model for c-model-contract.json.

No hardware/RTL/board claim. All times are abstract service units.
"""
from __future__ import annotations

from dataclasses import dataclass, field
from typing import Dict, List, Tuple, Optional, Set, Any
import json

STATUS_OK = "ok"
STATUS_INFEASIBLE = "infeasible"
STATUS_INVALID = "invalid"


@dataclass(frozen=True)
class Descriptor:
    id: int
    group: int
    group_name: str
    pos: int
    key: str


@dataclass
class Fixture:
    id: str
    groups: List[Tuple[str, List[str]]]
    L: float
    d: float
    consumer: float
    B: int
    cap: int
    name: str = ""
    descs: List[Descriptor] = field(init=False, default_factory=list)
    group_desc_ids: Dict[int, List[int]] = field(init=False, default_factory=dict)

    def __post_init__(self) -> None:
        self.descs = []
        for gi, (gname, keys) in enumerate(self.groups):
            for pos, key in enumerate(keys):
                self.descs.append(Descriptor(len(self.descs), gi, gname, pos, key))
        self.group_desc_ids = {
            g: [d.id for d in self.descs if d.group == g]
            for g in range(len(self.groups))
        }


FIXTURE_SPECS: Dict[str, Dict[str, Any]] = {
    "four_L10": {
        "id": "four_L10",
        "groups": [
            ["X", ["X"]],
            ["G1", ["Y", "Z"]],
            ["G2", ["Y", "Z"]],
            ["G3", ["Y", "Z"]],
        ],
        "L": 10,
        "d": 1,
        "consumer": 1,
        "B": 4,
        "cap": 1,
    },
    "eight_L10": {
        "id": "eight_L10",
        "base": "four_L10",
        "add": [
            ["H1", ["W", "U1"]],
            ["H2", ["W", "U2"]],
            ["H3", ["W", "U3"]],
            ["H4", ["W", "U4"]],
        ],
    },
    "four_L1": {
        "id": "four_L1",
        "base": "four_L10",
        "L": 1,
    },
    "eight_cap2": {
        "id": "eight_cap2",
        "base": "eight_L10",
        "cap": 2,
    },
    "eight_slow_consumer": {
        "id": "eight_slow_consumer",
        "base": "eight_L10",
        "consumer": 20,
    },
    "eight_small_pool": {
        "id": "eight_small_pool",
        "base": "eight_L10",
        "B": 2,
    },
}


def _resolve_spec(fid: str) -> Dict[str, Any]:
    spec = FIXTURE_SPECS[fid]
    if "base" in spec:
        base = _resolve_spec(spec["base"])
        out = dict(base)
        out["id"] = fid
        if "add" in spec:
            out["groups"] = list(base["groups"]) + list(spec["add"])
        for k in ("L", "d", "consumer", "B", "cap"):
            if k in spec:
                out[k] = spec[k]
        return out
    return dict(spec)


def build_case(fixture_id: str) -> Fixture:
    spec = _resolve_spec(fixture_id)
    return Fixture(
        id=spec["id"],
        groups=[(g[0], list(g[1])) for g in spec["groups"]],
        L=float(spec["L"]),
        d=float(spec["d"]),
        consumer=float(spec["consumer"]),
        B=int(spec["B"]),
        cap=int(spec["cap"]),
    )


def simulate(
    case: Fixture | str,
    ordered_descriptor_ids: List[int],
    overlap: bool = False,
    drain: bool = False,
) -> Dict[str, Any]:
    """Common executor for serial and one-step-lookahead overlap profiles.

    Returns a dict with status, times, cache, completed ids, ready/ack arrays,
    metrics and trace. Prefix replay is allowed because the workload is tiny.
    """
    if isinstance(case, str):
        case = build_case(case)

    descs = case.descs
    n = len(descs)
    order = list(ordered_descriptor_ids)

    if len(set(order)) != len(order):
        return _invalid_result("duplicate descriptor", case, order, overlap)
    for did in order:
        if not (0 <= did < n):
            return _invalid_result("descriptor id out of range", case, order, overlap)

    time = 0.0
    used = 0
    peak = 0
    capacity_wait = 0.0
    cache: List[str] = []
    walks = 0
    trace: List[Dict[str, Any]] = []

    copy_start: Dict[int, float] = {}
    copy_end: Dict[int, float] = {}
    used_before_dispatch: Dict[int, int] = {}

    completed: Set[int] = set()
    dispatched: Set[int] = set()

    group_ready: List[Optional[float]] = [None] * len(case.groups)
    group_ack: List[Optional[float]] = [None] * len(case.groups)
    group_consumer_start: List[Optional[float]] = [None] * len(case.groups)

    scheduled_acks: List[Tuple[float, int]] = []
    consumer_end = 0.0

    current_copy: Optional[Tuple[int, float]] = None
    next_walk: Optional[Tuple[int, str, float]] = None

    def insert_cache(key: str, t: float) -> None:
        nonlocal cache
        if key in cache:
            cache.remove(key)
        cache.append(key)
        if len(cache) > case.cap:
            evict = cache.pop(0)
            trace.append({"t": t, "event": "cache_evict", "key": evict})

    def process_events_until(t: float) -> None:
        nonlocal time, used, current_copy, next_walk, consumer_end, scheduled_acks
        while True:
            candidates: List[float] = []
            if current_copy is not None:
                candidates.append(current_copy[1])
            if next_walk is not None:
                candidates.append(next_walk[2])
            for at, _g in scheduled_acks:
                candidates.append(at)
            if not candidates:
                break
            et = min(candidates)
            if et > t:
                break
            time = et

            # Walk completion.
            if next_walk is not None and next_walk[2] == et:
                did, key, _ = next_walk
                next_walk = None
                insert_cache(key, et)
                trace.append({"t": et, "event": "walk_end", "descriptor": did, "key": key})

            # Copy completion.
            if current_copy is not None and current_copy[1] == et:
                did, _ = current_copy
                current_copy = None
                completed.add(did)
                d = descs[did]
                trace.append({"t": et, "event": "copy_end", "descriptor": did, "group": d.group})
                g = d.group
                if group_ready[g] is None:
                    if all(x in completed for x in case.group_desc_ids[g]):
                        group_ready[g] = et
                        start = max(et, consumer_end)
                        group_consumer_start[g] = start
                        ack_t = start + case.consumer
                        consumer_end = ack_t
                        scheduled_acks.append((ack_t, g))
                        trace.append({
                            "t": et,
                            "event": "group_ready",
                            "group": g,
                            "ack_time": ack_t,
                        })

            # ACKs.
            remaining_acks: List[Tuple[float, int]] = []
            for at, g in scheduled_acks:
                if at == et:
                    group_ack[g] = et
                    cnt = sum(1 for x in case.group_desc_ids[g] if x in dispatched)
                    used -= cnt
                    trace.append({
                        "t": et,
                        "event": "ack",
                        "group": g,
                        "released": cnt,
                    })
                else:
                    remaining_acks.append((at, g))
            scheduled_acks = remaining_acks

        if t > time:
            time = t

    def build_result(status: str) -> Dict[str, Any]:
        last_copy_time = max(copy_end.values()) if copy_end else 0.0
        ready_list = list(group_ready)
        ack_list = list(group_ack)
        ack_vals = [x for x in ack_list if x is not None]
        mean_ack = sum(ack_vals) / len(ack_vals) if ack_vals else None
        if all(x is not None for x in ack_list):
            makespan = max(ack_list)  # type: ignore[arg-type]
        else:
            makespan = last_copy_time
        metrics = {
            "mean_ack": mean_ack,
            "makespan": makespan,
            "last_copy_time": last_copy_time,
            "total_copies": len(completed),
            "source_walks": walks,
            "peak_buffer": peak,
            "capacity_wait": capacity_wait,
            "completion_curve": sorted(
                (group_ready[g], g)
                for g in range(len(case.groups))
                if group_ready[g] is not None
            ),
            "ready": ready_list,
            "ack": ack_list,
        }
        return {
            "status": status,
            "last_copy_time": last_copy_time,
            "cache": list(cache),
            "completed_ids": sorted(completed),
            "ready": ready_list,
            "ack": ack_list,
            "metrics": metrics,
            "trace": trace,
            "order": order,
            "overlap": overlap,
            "used": used,
            "used_before_dispatch": dict(used_before_dispatch),
            "copy_start": dict(copy_start),
            "copy_end": dict(copy_end),
            "time": time,
            "scheduled_acks": list(scheduled_acks),
        }

    if not overlap:
        for did in order:
            d = descs[did]
            key = d.key

            if key not in cache:
                walks += 1
                trace.append({
                    "t": time,
                    "event": "walk_start",
                    "descriptor": did,
                    "key": key,
                })
                walk_end = time + case.L
                process_events_until(walk_end)
                insert_cache(key, time)
                trace.append({
                    "t": time,
                    "event": "walk_end",
                    "descriptor": did,
                    "key": key,
                })

            if used >= case.B:
                wait_start = time
                while used >= case.B:
                    next_t: Optional[float] = None
                    if scheduled_acks:
                        next_t = min(at for at, _g in scheduled_acks)
                    if current_copy is not None:
                        if next_t is None or current_copy[1] < next_t:
                            next_t = current_copy[1]
                    if next_t is None:
                        return build_result(STATUS_INFEASIBLE)
                    process_events_until(next_t)
                capacity_wait += time - wait_start

            used_before_dispatch[did] = used
            dispatched.add(did)
            used += 1
            peak = max(peak, used)
            copy_start[did] = time
            copy_end[did] = time + case.d
            trace.append({
                "t": time,
                "event": "copy_start",
                "descriptor": did,
                "group": d.group,
            })
            current_copy = (did, copy_end[did])
            process_events_until(copy_end[did])

        if current_copy is not None:
            process_events_until(current_copy[1])
    else:
        idx = 0
        while idx < len(order):
            did = order[idx]
            d = descs[did]
            key = d.key

            if key not in cache:
                if next_walk is None or next_walk[0] != did:
                    walks += 1
                    trace.append({
                        "t": time,
                        "event": "walk_start",
                        "descriptor": did,
                        "key": key,
                    })
                    next_walk = (did, key, time + case.L)
                process_events_until(next_walk[2])

            if current_copy is not None:
                process_events_until(current_copy[1])

            if used >= case.B:
                wait_start = time
                while used >= case.B:
                    next_t: Optional[float] = None
                    if scheduled_acks:
                        next_t = min(at for at, _g in scheduled_acks)
                    if current_copy is not None:
                        if next_t is None or current_copy[1] < next_t:
                            next_t = current_copy[1]
                    if next_walk is not None:
                        if next_t is None or next_walk[2] < next_t:
                            next_t = next_walk[2]
                    if next_t is None:
                        return build_result(STATUS_INFEASIBLE)
                    process_events_until(next_t)
                capacity_wait += time - wait_start

            used_before_dispatch[did] = used
            dispatched.add(did)
            used += 1
            peak = max(peak, used)
            copy_start[did] = time
            copy_end[did] = time + case.d
            trace.append({
                "t": time,
                "event": "copy_start",
                "descriptor": did,
                "group": d.group,
            })
            current_copy = (did, copy_end[did])

            if idx + 1 < len(order):
                nid = order[idx + 1]
                nkey = descs[nid].key
                if nkey not in cache:
                    walks += 1
                    trace.append({
                        "t": time,
                        "event": "walk_start",
                        "descriptor": nid,
                        "key": nkey,
                    })
                    next_walk = (nid, nkey, time + case.L)
                else:
                    next_walk = None
            else:
                next_walk = None

            idx += 1

        if current_copy is not None:
            process_events_until(current_copy[1])

    if drain:
        while True:
            candidates: List[float] = []
            if current_copy is not None:
                candidates.append(current_copy[1])
            if next_walk is not None:
                candidates.append(next_walk[2])
            for at, _g in scheduled_acks:
                candidates.append(at)
            if not candidates:
                break
            process_events_until(min(candidates))

    return build_result(STATUS_OK)


def _invalid_result(
    reason: str,
    case: Fixture,
    order: List[int],
    overlap: bool,
) -> Dict[str, Any]:
    return {
        "status": STATUS_INVALID,
        "reason": reason,
        "last_copy_time": 0.0,
        "cache": [],
        "completed_ids": [],
        "ready": [None] * len(case.groups),
        "ack": [None] * len(case.groups),
        "metrics": {
            "mean_ack": None,
            "makespan": 0.0,
            "last_copy_time": 0.0,
            "total_copies": 0,
            "source_walks": 0,
            "peak_buffer": 0,
            "capacity_wait": 0.0,
            "completion_curve": [],
            "ready": [None] * len(case.groups),
            "ack": [None] * len(case.groups),
        },
        "trace": [],
        "order": order,
        "overlap": overlap,
        "used": 0,
        "used_before_dispatch": {},
        "copy_start": {},
        "copy_end": {},
        "time": 0.0,
        "scheduled_acks": [],
    }
```

```python c_policies.py
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
```

```python run_c_model.py
"""Run all six fixtures x two profiles x six policies."""
from __future__ import annotations

import json
from typing import Dict, List, Any

from c_event_model import build_case, simulate
from c_policies import (
    policy_group_first,
    policy_hot_page,
    policy_closure_ack_density,
    policy_max_closure,
    policy_setup_density,
    policy_marginal_unlock_proxy,
)

FIXTURE_IDS = [
    "four_L10",
    "eight_L10",
    "four_L1",
    "eight_cap2",
    "eight_slow_consumer",
    "eight_small_pool",
]

PROFILES = [
    ("serial", False),
    ("overlap", True),
]

POLICIES = [
    ("group_first", policy_group_first),
    ("hot_page", policy_hot_page),
    ("closure_ack_density", policy_closure_ack_density),
    ("max_closure", policy_max_closure),
    ("setup_density", policy_setup_density),
    ("marginal_unlock_proxy", policy_marginal_unlock_proxy),
]


def run_all() -> Dict[str, Any]:
    out: Dict[str, Any] = {"runs": [], "hand": {}}

    for fid in FIXTURE_IDS:
        case = build_case(fid)
        for pname, pfunc in POLICIES:
            for profile, overlap in PROFILES:
                order = pfunc(case, overlap=overlap)
                res = simulate(case, order, overlap=overlap, drain=True)
                out["runs"].append({
                    "fixture": fid,
                    "policy": pname,
                    "profile": profile,
                    "status": res["status"],
                    "order": order,
                    "ready": res["ready"],
                    "ack": res["ack"],
                    "metrics": res["metrics"],
                })

    case4 = build_case("four_L10")
    hand_orders = {
        "x_first": [0, 1, 3, 5, 2, 4, 6],
        "yz_first": [1, 3, 5, 2, 4, 6, 0],
    }
    for label, order in hand_orders.items():
        res = simulate(case4, order, overlap=False, drain=True)
        out["hand"][label] = {
            "order": order,
            "status": res["status"],
            "ready": res["ready"],
            "ack": res["ack"],
            "metrics": res["metrics"],
        }

    return out


def main() -> None:
    print(json.dumps(run_all(), indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
```

```python test_c_model.py
"""Meaningful unittest checks for the c-model contract."""
from __future__ import annotations

import unittest

from c_event_model import build_case, simulate, STATUS_OK, STATUS_INFEASIBLE, STATUS_INVALID
from c_policies import (
    policy_group_first,
    policy_hot_page,
    policy_closure_ack_density,
    policy_max_closure,
    policy_setup_density,
    policy_marginal_unlock_proxy,
    FIXTURE_IDS if hasattr(__import__("c_policies"), "FIXTURE_IDS") else [],
)

# c_policies does not export FIXTURE_IDS; import from run_c_model instead.
from run_c_model import FIXTURE_IDS, POLICIES, PROFILES


class TestHandSerial(unittest.TestCase):
    def test_four_L10_x_first_hand_timing(self):
        case = build_case("four_L10")
        order = [0, 1, 3, 5, 2, 4, 6]  # X, Y1, Y2, Y3, Z1, Z2, Z3
        res = simulate(case, order, overlap=False, drain=True)
        self.assertEqual(res["status"], STATUS_OK)
        self.assertEqual(res["ready"], [11, 35, 37, 38])
        self.assertEqual(res["ack"], [12, 36, 38, 39])
        self.assertEqual(res["metrics"]["source_walks"], 3)
        self.assertEqual(res["metrics"]["total_copies"], 7)
        self.assertLessEqual(res["metrics"]["peak_buffer"], 4)
        self.assertEqual(res["metrics"]["makespan"], 39)

    def test_four_L10_yz_first_hand_timing(self):
        case = build_case("four_L10")
        order = [1, 3, 5, 2, 4, 6, 0]  # Y1, Y2, Y3, Z1, Z2, Z3, X
        res = simulate(case, order, overlap=False, drain=True)
        self.assertEqual(res["status"], STATUS_OK)
        by_name = {case.groups[g][0]: res["ready"][g] for g in range(4)}
        ack_by_name = {case.groups[g][0]: res["ack"][g] for g in range(4)}
        self.assertEqual(by_name, {"X": 38, "G1": 24, "G2": 26, "G3": 27})
        self.assertEqual(ack_by_name, {"X": 39, "G1": 25, "G2": 27, "G3": 28})
        self.assertEqual(res["metrics"]["makespan"], 39)
        self.assertEqual(res["metrics"]["source_walks"], 3)
        self.assertEqual(res["metrics"]["total_copies"], 7)
        self.assertLessEqual(res["metrics"]["peak_buffer"], 4)

    def test_closure_ack_density_matches_yz_first_on_four_serial(self):
        case = build_case("four_L10")
        order_closure = policy_closure_ack_density(case, overlap=False)
        res_closure = simulate(case, order_closure, overlap=False, drain=True)
        res_yz = simulate(case, [1, 3, 5, 2, 4, 6, 0], overlap=False, drain=True)
        self.assertEqual(res_closure["status"], STATUS_OK)
        self.assertEqual(res_closure["ready"], res_yz["ready"])
        self.assertEqual(res_closure["ack"], res_yz["ack"])

    def test_eight_prefix_hand_timing(self):
        case = build_case("eight_L10")
        # H1..H4 are groups 4..7. W1=7, W2=9, W3=11, U1=8.
        res = simulate(case, [7, 9, 11, 8], overlap=False, drain=True)
        self.assertEqual(res["status"], STATUS_OK)
        self.assertEqual(res["ready"][4], 24)
        self.assertEqual(res["ack"][4], 25)

        # Y1=1, Y2=3, Y3=5, Z1=2, Z2=4, Z3=6.
        res2 = simulate(case, [1, 3, 5, 2, 4, 6], overlap=False, drain=True)
        self.assertEqual(res2["status"], STATUS_OK)
        self.assertEqual(res2["ack"][1], 25)
        self.assertEqual(res2["ack"][2], 27)
        self.assertEqual(res2["ack"][3], 28)


class TestNegativeAndInvariants(unittest.TestCase):
    def test_reject_full_pool_unfinished_head_prefix(self):
        case = build_case("eight_L10")
        # W1,W2,W3,W4 then U1: after W4 the pool is full with no pending ACK.
        res = simulate(case, [7, 9, 11, 13, 8], overlap=False, drain=False)
        self.assertEqual(res["status"], STATUS_INFEASIBLE)

    def test_detect_duplicate_descriptor(self):
        case = build_case("four_L10")
        res = simulate(case, [0, 0], overlap=False, drain=False)
        self.assertEqual(res["status"], STATUS_INVALID)

    def test_no_future_ack_borrowing(self):
        case = build_case("four_L10")
        res = simulate(case, [0, 1, 3, 5, 2, 4, 6], overlap=False, drain=True)
        # Z2 is descriptor 4. It cannot dispatch before G1 ACK at 36.
        self.assertGreaterEqual(res["copy_start"][4], res["ack"][1])
        self.assertEqual(res["ack"][1], 36)

    def test_same_overlap_opportunity_for_controls(self):
        case = build_case("four_L10")
        order = [1, 3, 5, 2, 4, 6, 0]
        serial = simulate(case, order, overlap=False, drain=True)
        overlap = simulate(case, order, overlap=True, drain=True)
        self.assertEqual(serial["status"], STATUS_OK)
        self.assertEqual(overlap["status"], STATUS_OK)
        self.assertLessEqual(overlap["last_copy_time"], serial["last_copy_time"])

    def test_all_fixtures_profiles_policies_are_legal_and_complete(self):
        for fid in FIXTURE_IDS:
            case = build_case(fid)
            for pname, pfunc in POLICIES:
                for profile, overlap in PROFILES:
                    order = pfunc(case, overlap=overlap)
                    res = simulate(case, order, overlap=overlap, drain=True)
                    ctx = f"{fid}/{pname}/{profile}"
                    self.assertEqual(res["status"], STATUS_OK, ctx)
                    self.assertEqual(
                        len(res["completed_ids"]),
                        len(case.descs),
                        f"{ctx}: not all descriptors copied once",
                    )
                    self.assertEqual(
                        sorted(res["completed_ids"]),
                        list(range(len(case.descs))),
                        f"{ctx}: duplicate or missing descriptor",
                    )
                    self.assertTrue(
                        all(x is not None for x in res["ready"]),
                        f"{ctx}: some group not ready",
                    )
                    self.assertTrue(
                        all(x is not None for x in res["ack"]),
                        f"{ctx}: some group not ACKed",
                    )
                    self.assertLessEqual(
                        res["metrics"]["peak_buffer"],
                        case.B,
                        f"{ctx}: peak buffer exceeds B",
                    )
                    for g, ids in case.group_desc_ids.items():
                        self.assertLessEqual(
                            len(ids),
                            case.B,
                            f"{ctx}: group {g} exceeds B",
                        )
                    # No release before ACK: every ACK time must not precede ready time.
                    for g in range(len(case.groups)):
                        self.assertGreaterEqual(
                            res["ack"][g],
                            res["ready"][g],
                            f"{ctx}: ACK before ready for group {g}",
                        )


if __name__ == "__main__":
    unittest.main()
```