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
