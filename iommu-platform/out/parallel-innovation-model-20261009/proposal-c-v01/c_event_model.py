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
