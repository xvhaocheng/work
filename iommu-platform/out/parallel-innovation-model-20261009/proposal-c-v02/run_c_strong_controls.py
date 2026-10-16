"""run_c_strong_controls.py -- 48 comparisons for c-strong-control-contract.json.

Six existing fixtures x two existing profiles x four ordinary controls
(warm_density, fixed_cohort k=1,2,3) = 48 full comparisons.  No new fixtures, no
RTL, no sweeps, no observers, no application traces, and no candidate claim.
The runner records order / full ready + ACK / mean / makespan / walks / buffer,
verifies every call finished all groups legally, and prints the warm-density
cap1 warm-reuse acceptance probes and a module-integrity (no-monkeypatch) check.
"""
from __future__ import annotations

import json
from typing import Any, Callable, Dict, List, Tuple

import c_event_model
import c_policies
import c_strong_controls
from c_event_model import STATUS_OK, build_case, simulate
from c_strong_controls import (
    InfeasibleComparator,
    policy_fixed_cohort_tile,
    policy_warm_density,
    warm_density_candidates,
)

FIXTURE_IDS: List[str] = [
    "four_L10",
    "eight_L10",
    "four_L1",
    "eight_cap2",
    "eight_slow_consumer",
    "eight_small_pool",
]

PROFILES: List[Tuple[str, bool]] = [("serial", False), ("overlap", True)]

CONTROLS: List[Tuple[str, Dict[str, Any], Callable[..., List[int]]]] = [
    ("warm_density", {}, policy_warm_density),
    ("fixed_cohort_k1", {"k": 1}, policy_fixed_cohort_tile),
    ("fixed_cohort_k2", {"k": 2}, policy_fixed_cohort_tile),
    ("fixed_cohort_k3", {"k": 3}, policy_fixed_cohort_tile),
]

# cap1 decision points where the complete plan is otherwise tied: the cached page
# must be consumed before it is re-walked.
WARM_REUSE_PROBES: List[Tuple[str, List[int], str]] = [
    (
        "four_L10",
        [1, 2],
        "cap1 serial, cache={Z}: two Y/Z pair groups tie except for the warm Z",
    ),
    (
        "eight_small_pool",
        [0, 1, 2],
        "cap1 B=2 serial, cache={Z}: two Y/Z pair groups tie except for the warm Z",
    ),
]


def run_one(
    case,
    policy_name: str,
    pfunc: Callable[..., List[int]],
    pkwargs: Dict[str, Any],
    profile: str,
    overlap: bool,
) -> Tuple[Dict[str, Any], bool]:
    record: Dict[str, Any] = {
        "fixture": case.id,
        "policy": policy_name,
        "profile": profile,
    }
    record.update(pkwargs)
    try:
        order = pfunc(case, overlap=overlap, **pkwargs)
    except InfeasibleComparator as exc:
        # Contract line 7: report an invalid comparator with the exact reason,
        # never a win/loss.
        record.update({"status": "invalid_comparator", "reason": str(exc)})
        return record, True

    res = simulate(case, order, overlap=overlap, drain=True)
    metrics = res["metrics"]
    record.update(
        {
            "status": res["status"],
            "order": list(order),
            "ready": res["ready"],
            "ack": res["ack"],
            "mean_ack": metrics["mean_ack"],
            "makespan": metrics["makespan"],
            "source_walks": metrics["source_walks"],
            "peak_buffer": metrics["peak_buffer"],
            "capacity_wait": metrics["capacity_wait"],
            "total_copies": metrics["total_copies"],
            "complete_unique_order": sorted(order) == list(range(len(case.descs))),
            "all_groups_acked": all(a is not None for a in res["ack"]),
            "buffer_within_B": metrics["peak_buffer"] <= case.B,
        }
    )
    return record, False


def _comparison_table(runs: List[Dict[str, Any]]) -> Dict[str, Any]:
    table: Dict[str, Any] = {}
    for r in runs:
        table.setdefault(r["fixture"], {}).setdefault(r["profile"], {})[r["policy"]] = {
            "makespan": r["makespan"],
            "mean_ack": r["mean_ack"],
            "source_walks": r["source_walks"],
            "peak_buffer": r["peak_buffer"],
            "capacity_wait": r["capacity_wait"],
        }
    return table


def _whole_run_check(runs: List[Dict[str, Any]]) -> Dict[str, Any]:
    failures = [
        {
            "fixture": r["fixture"],
            "policy": r["policy"],
            "profile": r["profile"],
            "status": r["status"],
        }
        for r in runs
        if r["status"] != STATUS_OK
        or not r["complete_unique_order"]
        or not r["all_groups_acked"]
        or not r["buffer_within_B"]
    ]
    return {
        "runs_checked": len(runs),
        "expected_runs": len(FIXTURE_IDS) * len(PROFILES) * len(CONTROLS),
        "all_ok": not failures,
        "failures": failures,
    }


def _warm_reuse_probe(fixture_id: str, prefix: List[int], label: str) -> Dict[str, Any]:
    case = build_case(fixture_id)
    res_prefix = simulate(case, prefix, overlap=False, drain=False)
    remaining = set(range(len(case.descs))) - set(prefix)
    candidates = warm_density_candidates(case, prefix, remaining, False)
    warm_page = res_prefix["cache"][-1] if res_prefix["cache"] else None
    detail: Dict[str, Any] = {
        "fixture": fixture_id,
        "label": label,
        "prefix": list(prefix),
        "cap": case.cap,
        "B": case.B,
        "cached_page": warm_page,
        "candidate_count": len(candidates),
    }
    if not candidates:
        detail["pass"] = False
        detail["reason"] = "no capacity-feasible union candidate at this decision point"
        return detail

    top = min(candidates, key=lambda c: c.rank)
    tied = [
        c
        for c in candidates
        if c.completed_groups == top.completed_groups and len(c.cand) == len(top.cand)
    ]
    tied_walks = sorted({c.extra_walks for c in tied})
    checks = {
        "cap_is_one": case.cap == 1,
        "prefix_cache_is_warm": warm_page is not None,
        "top_starts_with_cached_page": (
            warm_page is not None and case.descs[top.cand[0]].key == warm_page
        ),
        "otherwise_tied_candidates_exist": len(tied) > 1,
        "top_has_min_extra_walks_among_tied": (
            top.extra_walks == min(c.extra_walks for c in tied)
        ),
        "tied_alternative_rewalks_cached_page": (
            len(tied_walks) > 1 and max(tied_walks) > top.extra_walks
        ),
        "top_candidate_executable": top.res["status"] == STATUS_OK,
    }
    detail.update(
        {
            "top_order": list(top.cand),
            "top_completed_groups": top.completed_groups,
            "top_extra_walks": top.extra_walks,
            "tied_candidate_count": len(tied),
            "tied_extra_walk_values": tied_walks,
            "checks": checks,
            "pass": all(checks.values()),
        }
    )
    return detail


def _module_integrity() -> Dict[str, Any]:
    """Confirm the runner/control import path did not wrap or replace the shared
    executor (no monkeypatching), and that the walk counter used by warm_density
    exists on the current c_event_model."""
    probe = simulate(build_case("four_L10"), [0], overlap=False, drain=False)
    return {
        "c_policies_simulate_is_c_event_model": c_policies.simulate is c_event_model.simulate,
        "c_policies_build_case_is_c_event_model": c_policies.build_case is c_event_model.build_case,
        "c_strong_controls_simulate_is_c_event_model": (
            c_strong_controls.simulate is c_event_model.simulate
        ),
        "status_ok_is_shared": STATUS_OK == c_event_model.STATUS_OK,
        "source_walks_metric_present": "source_walks" in probe["metrics"],
    }


def acceptance(out: Dict[str, Any]) -> Dict[str, Any]:
    return {
        "whole_run": _whole_run_check(out["runs"]),
        "warm_reuse_probes": [_warm_reuse_probe(*p) for p in WARM_REUSE_PROBES],
        "module_integrity": _module_integrity(),
        "invalid_comparators": len(out["invalid"]),
    }


def run_all() -> Dict[str, Any]:
    out: Dict[str, Any] = {
        "contract": "c-strong-control-contract.json",
        "fixtures": FIXTURE_IDS,
        "profiles": [p for p, _ in PROFILES],
        "controls": [c for c, _, _ in CONTROLS],
        "runs": [],
        "invalid": [],
    }
    for fid in FIXTURE_IDS:
        case = build_case(fid)
        for policy_name, pkwargs, pfunc in CONTROLS:
            for profile, overlap in PROFILES:
                record, invalid = run_one(
                    case, policy_name, pfunc, pkwargs, profile, overlap
                )
                (out["invalid"] if invalid else out["runs"]).append(record)
    out["comparison_table"] = _comparison_table(out["runs"])
    out["acceptance"] = acceptance(out)
    return out


def main() -> None:
    print(json.dumps(run_all(), indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
