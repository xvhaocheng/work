## Deliverable

Two complete blocks only (plus bounded assumptions and acceptance commands). Nothing was executed; every number below is a hand-derivation from the supplied executor, explicitly flagged as such.

---

### Block 1 — `c_strong_controls.py` (new file, standalone, no monkeypatching)

```python
"""c_strong_controls.py -- two strong ORDINARY controls (c-strong-control-contract.json).

Standalone module.  It does not modify c_event_model.py or c_policies.py and does
not monkeypatch any existing policy; it only imports the common executor and the
stable helpers whose signatures the pending LRU / test-import / proxy-cost
revision leaves unchanged.

  policy_warm_density(case, overlap=False) -> List[int]
      Contract line 5.  Candidate set, lawful capacity gate, payload work and
      overlap profile are exactly the existing setup_density enumeration
      (one/two-source-key unions x age prefixes x page orders, via
      c_policies._iter_union_candidates).  Only the score differs:
          completed_groups / (L * ACTUAL extra source walks measured between the
                              common prefix probe and the candidate probe
                              + d * descriptors in the candidate)
      Capacity wait and ACK timing are NOT scored.  Ties: older group, fewer
      extra walks, then lexicographic descriptor order.

  policy_fixed_cohort_tile(case, overlap=False, k=1) -> List[int]
      Contract line 6.  k in {1,2,3} is a predetermined static parameter, fixed
      for the whole run.  At each cohort boundary the oldest unfinished group g0
      opens a cohort containing the age prefix of up to k still-unfinished groups
      whose FIRST declared source page equals g0's first declared source page
      (complete static source domain).  ALL remaining copies of the cohort are
      issued before the next cohort opens.  Inside a cohort the page order is:
      currently cached page if it still has remaining copies, else largest
      remaining fanout, ties oldest group then descriptor declaration.  Batches
      go through the existing common-executor safe incoming-slot helper with
      in-cohort partial-group fallback.  k limits the cohort, never the number of
      fragments, and no outside group is ever mixed in.

Both functions return a complete unique descriptor order, or raise
InfeasibleComparator with an exact reason (contract line 7) instead of reporting
a bogus comparator result.
"""
from __future__ import annotations

import itertools
from typing import Any, Dict, List, NamedTuple, Sequence, Set, Tuple

from c_event_model import STATUS_OK, build_case
from c_policies import (
    Fixture,                     # contract line 4: Fixture/simulate via c_policies
    simulate,
    _incremental_safe_batch,     # existing safe incoming-slot helper
    _iter_union_candidates,      # existing setup_density candidate enumeration
    _partial_group_fallback,     # existing partial-group fallback
)

__all__ = [
    "InfeasibleComparator",
    "WarmCandidate",
    "warm_density_candidates",
    "policy_warm_density",
    "policy_fixed_cohort_tile",
]


class InfeasibleComparator(RuntimeError):
    """No legal continuation exists for this comparator.

    Contract line 7: report an invalid comparator with an exact reason instead of
    counting a candidate win.  The runner converts this into an explicit
    "invalid_comparator" record.
    """


class WarmCandidate(NamedTuple):
    """One capacity-feasible warm_density candidate."""

    rank: Tuple[Any, ...]
    groups: List[int]
    cand: List[int]
    res: Dict[str, Any]
    extra_walks: int
    completed_groups: int


def _coerce_case(case: "Fixture | str") -> Fixture:
    return build_case(case) if isinstance(case, str) else case


def _newly_ready(
    res: Dict[str, Any], res_prefix: Dict[str, Any], groups: Sequence[int]
) -> int:
    """Groups that become ready inside the candidate, relative to the common prefix."""
    return sum(
        1
        for g in groups
        if res["ready"][g] is not None and res_prefix["ready"][g] is None
    )


# ---------------------------------------------------------------------------
# warm_density (contract line 5)
# ---------------------------------------------------------------------------

def warm_density_candidates(
    case: "Fixture | str",
    order: List[int],
    remaining: Set[int],
    overlap: bool = False,
) -> List[WarmCandidate]:
    """All one/two-source-key union + age-prefix + page-order candidates under the
    warm-density rule, probed on the same common prefix `order`.

    The candidate set is exactly the existing setup_density set; only the ranking
    differs.  The denominator uses the ACTUAL extra source walks reported by the
    common executor, so warm hits, evictions and in-candidate re-walks caused by
    the LRU cache are all accounted for as they really happen (no static
    cold-key estimate, no ACK wait).
    """
    case = _coerce_case(case)
    if not remaining:
        return []
    res_prefix = simulate(case, order, overlap=overlap, drain=False)
    if res_prefix["status"] != STATUS_OK:
        raise InfeasibleComparator(
            "warm_density prefix is not executable "
            f"(status={res_prefix['status']}, prefix_len={len(order)})"
        )
    walks_prefix = res_prefix["metrics"]["source_walks"]
    keys = sorted({case.descs[d].key for d in remaining})
    candidates: List[WarmCandidate] = []
    for size in (1, 2):
        for union_tuple in itertools.combinations(keys, size):
            union = set(union_tuple)
            for groups, cand, res in _iter_union_candidates(
                case, order, remaining, union, overlap
            ):
                if res["status"] != STATUS_OK or not cand:
                    continue
                extra_walks = res["metrics"]["source_walks"] - walks_prefix
                completed = _newly_ready(res, res_prefix, groups)
                cost = case.L * extra_walks + case.d * len(cand)
                score = (completed / cost) if cost > 0 else 0.0
                rank = (-score, min(groups) if groups else 0, extra_walks, tuple(cand))
                candidates.append(
                    WarmCandidate(
                        rank=rank,
                        groups=list(groups),
                        cand=list(cand),
                        res=res,
                        extra_walks=extra_walks,
                        completed_groups=completed,
                    )
                )
    return candidates


def _plan_warm_density(case: Fixture, overlap: bool) -> List[int]:
    order: List[int] = []
    remaining: Set[int] = set(range(len(case.descs)))
    while remaining:
        candidates = warm_density_candidates(case, order, remaining, overlap)
        block: List[int] = []
        if candidates:
            best = min(candidates, key=lambda c: c.rank)
            block = [d for d in best.cand if d in remaining]
        if not block:
            # No one/two-key-union candidate is capacity-feasible here: ordinary
            # fallback = oldest remaining descriptor, validated by the same
            # common executor (never a silent partial order).
            block = [min(remaining)]
        if not set(block) <= remaining:
            raise InfeasibleComparator(
                f"warm_density candidate left the remaining set (block={block})"
            )
        trial = order + block
        res = simulate(case, trial, overlap=overlap, drain=False)
        if res["status"] != STATUS_OK:
            raise InfeasibleComparator(
                "warm_density has no legal extension "
                f"(status={res['status']}, block={block}, prefix_len={len(order)})"
            )
        order = trial
        remaining -= set(block)
    return order


def policy_warm_density(case: "Fixture | str", overlap: bool = False) -> List[int]:
    """Ordinary source-miss accounting control, warm-first (contract line 5)."""
    return _plan_warm_density(_coerce_case(case), overlap)


# ---------------------------------------------------------------------------
# fixed_cohort_tile (contract line 6)
# ---------------------------------------------------------------------------

def _first_declared_key(case: Fixture, group: int) -> str:
    """FIRST declared source page of a group inside the complete static domain."""
    return case.groups[group][1][0]


def _unfinished_groups(case: Fixture, remaining: Set[int]) -> List[int]:
    return [
        g
        for g in range(len(case.groups))
        if any(case.descs[d].group == g for d in remaining)
    ]


def _cohort_groups(case: Fixture, remaining: Set[int], k: int) -> List[int]:
    """Oldest unfinished group plus the age prefix of up to k unfinished groups
    that share its first declared source page (g0 is always included)."""
    unfinished = _unfinished_groups(case, remaining)
    if not unfinished:
        return []
    g0 = unfinished[0]
    anchor = _first_declared_key(case, g0)
    cohort: List[int] = []
    for g in unfinished:
        if len(cohort) >= k:
            break
        if _first_declared_key(case, g) == anchor:
            cohort.append(g)
    return cohort


def _choose_cohort_page(
    case: Fixture, cohort_remaining: Set[int], cache: Sequence[str]
) -> str:
    """Currently cached page if it still has remaining copies, else largest
    remaining fanout, ties oldest group then descriptor declaration."""
    warm = set(cache)
    fanout: Dict[str, int] = {}
    oldest: Dict[str, Tuple[int, int]] = {}
    for did in sorted(cohort_remaining):  # deterministic declaration walk
        key = case.descs[did].key
        rank = (case.descs[did].group, case.descs[did].pos)
        fanout[key] = fanout.get(key, 0) + 1
        if key not in oldest or rank < oldest[key]:
            oldest[key] = rank
    return min(
        fanout,
        key=lambda key: (
            0 if key in warm else 1,
            -fanout[key],
            oldest[key][0],
            oldest[key][1],
        ),
    )


def _plan_fixed_cohort_tile(case: Fixture, overlap: bool, k: int) -> List[int]:
    if k < 1:
        raise ValueError("fixed_cohort_tile requires k >= 1")
    order: List[int] = []
    remaining: Set[int] = set(range(len(case.descs)))
    while remaining:
        cohort = _cohort_groups(case, remaining, k)
        cohort_remaining = {d for d in remaining if case.descs[d].group in cohort}
        if not cohort_remaining:
            raise InfeasibleComparator(
                f"empty cohort selection (cohort={cohort}, remaining={len(remaining)})"
            )
        while cohort_remaining:
            res_prefix = simulate(case, order, overlap=overlap, drain=False)
            if res_prefix["status"] != STATUS_OK:
                raise InfeasibleComparator(
                    "cohort prefix not executable "
                    f"(status={res_prefix['status']}, cohort={cohort}, "
                    f"prefix_len={len(order)})"
                )
            page = _choose_cohort_page(case, cohort_remaining, res_prefix["cache"])
            batch = sorted(
                (d for d in cohort_remaining if case.descs[d].key == page),
                key=lambda d: (case.descs[d].group, case.descs[d].pos),
            )
            block = _incremental_safe_batch(case, order, batch, overlap)
            if len(block) < len(batch):
                fallback = _partial_group_fallback(
                    case, order + block, cohort_remaining - set(block), overlap
                )
                if fallback:
                    block = block + fallback
            if not block:
                block = _partial_group_fallback(case, order, cohort_remaining, overlap)
            if not block:
                # Last resort: single oldest descriptor still inside this cohort.
                for did in sorted(
                    cohort_remaining,
                    key=lambda d: (case.descs[d].group, case.descs[d].pos),
                ):
                    trial = simulate(
                        case, order + [did], overlap=overlap, drain=False
                    )
                    if trial["status"] == STATUS_OK:
                        block = [did]
                        break
            if not block:
                raise InfeasibleComparator(
                    "no legal in-cohort continuation "
                    f"(cohort={cohort}, cohort_remaining={sorted(cohort_remaining)}, "
                    f"prefix_len={len(order)})"
                )
            if not set(block) <= cohort_remaining:
                raise InfeasibleComparator(
                    f"helper returned descriptors outside the cohort (block={block})"
                )
            order = order + block
            cohort_remaining -= set(block)
    return order


def policy_fixed_cohort_tile(
    case: "Fixture | str", overlap: bool = False, k: int = 1
) -> List[int]:
    """Fixed static cohort-tiling control (contract line 6).  k in {1,2,3}."""
    return _plan_fixed_cohort_tile(_coerce_case(case), overlap, k)
```

---

### Block 2 — `run_c_strong_controls.py` (new file; 48 comparisons)

```python
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
```

---

## Bounded assumptions (each is a place where the contract leaves a choice)

1. **Import surface (contract line 4).** `Fixture`/`simulate` are imported from `c_policies`; `STATUS_OK`/`build_case` from `c_event_model`; three helpers from `c_policies`: `_incremental_safe_batch`, `_partial_group_fallback`, `_iter_union_candidates`. Per the task note the pending revision changes only LRU / test imports / proxy cost, so these signatures hold. If any is renamed the module fails loudly at import — it never silently degrades.
2. **"Currently hot source page" = a page present in the executor's source cache** (LRU list returned by the common prefix probe). For the five `cap==1` fixtures this is unambiguous; for `eight_cap2` (cap=2) any warm key beats any cold key, and among two warm keys the larger remaining fanout wins (the contract fixes no warm-vs-warm rule).
3. **`warm_density` "completed_groups" = groups newly ready inside the candidate relative to the common prefix probe**, matching the existing `setup_density`/`max_closure` definition; ties are `(-score, oldest group, extra walks, lexicographic candidate)` exactly as line 5 lists them.
4. **`warm_density` measures walks with `metrics["source_walks"]` deltas** (`ACTUAL`), so its results depend on the pending LRU/walk-count fix; **do not compare against the pre-fix 72-run draft `c-first-run.json`**.
5. **"FIRST declared source page" = the group's first declared descriptor key in the complete static domain** (`case.groups[g][1][0]`), not "first still-remaining key". Because a cohort is always finished completely, a group never spans cohorts, so in practice the two coincide.
6. **Capacity statements.** Nothing is forced resident: `_incremental_safe_batch` is used unchanged, so a non-completing addition must leave one free incoming slot and a group-completing addition may take the last slot; the in-cohort fallback is restricted to `cohort_remaining`, so it can only close the cohort's own groups. If no legal continuation exists the policy raises `InfeasibleComparator` with the cohort/prefix context, and the runner files it under `invalid` (never as a win).
7. **Scope:** 6 fixtures × 2 profiles × 4 controls = 48 comparisons; no new fixtures, RTL, sweeps, observers or traces; the three `k` values are predetermined static parameters, so a per-case best `k` is an optimistic tuned SIMPLE comparator, not an online adaptive result.

Hand-derived expectations (NOT executed; for root's cross-check only): on `four_L10` serial, `warm_density` should commit to the full Y/Z closure first and then X, i.e. reproduce the contract's `YZ-first` anchor order `[1,3,5,2,4,6,0]` with `ready [24,26,27,38]`, `ack [25,27,28,39]`, `makespan 39`, `walks 3` (contract line 132); on `four_L1` it should pick `X` first (`L` cheapens the extra walk), matching the `X-first` anchor.

Flagged for Codex (not resolved here): whether "completed_groups" should ever be read as absolute completed groups rather than newly-ready groups, and whether per-step myopia of `warm_density` is acceptable as the intended "strong ordinary" comparator. No candidate advantage is claimed anywhere in this work.

---

## Acceptance commands (root to execute; none run here)

```bash
cd out/parallel-innovation-model-20261009/proposal-c-v01   # dir holding c_event_model.py / c_policies.py

# 0. imports + no monkeypatch of the shared executor
python -c "import c_strong_controls, c_policies, c_event_model as e; \
print(c_policies.simulate is e.simulate, c_strong_controls.simulate is e.simulate)"

# 1. both controls on the two hand-anchored fixtures (order only)
python -c "from c_strong_controls import policy_warm_density as w, policy_fixed_cohort_tile as t; \
print('four_L10 warm', w('four_L10', overlap=False)); \
print('four_L1  warm', w('four_L1',  overlap=False)); \
print('four_L10 k1..3', [t('four_L10', k=k) for k in (1,2,3)]); \
print('small_pool k3', t('eight_small_pool', k=3))"

# 2. hand anchor: warm_density must equal the contract's YZ-first hand order
python -c "from c_event_model import build_case, simulate; from c_strong_controls import policy_warm_density as w; \
c=build_case('four_L10'); o=w(c, overlap=False); r=simulate(c,o,overlap=False,drain=True); \
print(o, r['ready'], r['ack'], r['metrics']['makespan'], r['metrics']['source_walks'], r['status'])"

# 3. the 48-comparison run
python run_c_strong_controls.py > c-strong-controls-run.json

# 4. inspect the recorded gates
python - <<'PY'
import json
d = json.load(open("c-strong-controls-run.json"))
print(d["acceptance"]["whole_run"])                    # expect all_ok true, runs_checked 48
print(d["acceptance"]["invalid_comparators"])          # expect 0
print(json.dumps(d["acceptance"]["module_integrity"], indent=2))
print(json.dumps(d["acceptance"]["warm_reuse_probes"], indent=2))  # expect both pass true
print(json.dumps(d["comparison_table"]["four_L10"], indent=2))
print(json.dumps(d["comparison_table"]["eight_small_pool"], indent=2))
PY
```

Pass criteria the runner encodes: every one of the 48 runs has `status == "ok"`, `complete_unique_order`, `all_groups_acked`, `peak_buffer <= B`; both warm-reuse probes report `pass: true` (cap1, warm page consumed before it is re-walked while the plans are otherwise tied on completed groups and descriptor count); `invalid_comparators == 0`.