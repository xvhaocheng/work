"""Trial-level statistics; no dependency on scalar-table row counts.

Quartiles use Type 7 linear interpolation. Bootstrap intervals are percentile
intervals conditional on the observed boots, not population-of-boots inference.
"""
from __future__ import annotations

import math
import random
from collections import defaultdict
from statistics import median

SEED = 20261017
N_RESAMPLES = 10000


def _values(values):
    values = [float(x) for x in values]
    if any(not math.isfinite(x) for x in values):
        raise ValueError("statistics require finite observed values")
    return values


def quantile(values, p):
    """Type 7 quantile, including median for p=0.5."""
    values = sorted(_values(values))
    if not 0 <= p <= 1:
        raise ValueError("quantile probability outside [0,1]")
    if not values:
        return None
    h = (len(values) - 1) * p
    lo, hi = math.floor(h), math.ceil(h)
    return values[lo] + (values[hi] - values[lo]) * (h - lo)


def _groups(values, strata):
    if strata is None:
        return [values]
    if len(strata) != len(values):
        raise ValueError("strata length must equal sample count")
    groups = defaultdict(list)
    for value, stratum in zip(values, strata):
        groups[str(stratum)].append(value)
    return [groups[k] for k in sorted(groups)]


def _draw(groups, rng):
    return [group[rng.randrange(len(group))] for group in groups for _ in group]


def bootstrap_median_ci(values, *, seed=SEED, n_resamples=N_RESAMPLES,
                        strata=None, confidence=0.95):
    values = _values(values)
    groups = _groups(values, strata)
    if n_resamples < 1 or not 0 < confidence < 1:
        raise ValueError("invalid bootstrap parameters")
    if len(values) < 2:
        return None, None
    rng = random.Random(seed)
    draws = [median(_draw(groups, rng)) for _ in range(n_resamples)]
    alpha = (1 - confidence) / 2
    return quantile(draws, alpha), quantile(draws, 1 - alpha)


def summarize(values, *, seed=SEED, n_resamples=N_RESAMPLES, strata=None,
              confidence=0.95):
    values = _values(values)
    _groups(values, strata)
    q1, q3 = quantile(values, .25), quantile(values, .75)
    low, high = bootstrap_median_ci(values, seed=seed, n_resamples=n_resamples,
                                   strata=strata, confidence=confidence)
    return dict(n=len(values), median=median(values) if values else None,
                q1=q1, q3=q3, iqr=q3-q1 if values else None,
                min=min(values) if values else None, max=max(values) if values else None,
                ci_low=low, ci_high=high, confidence=confidence,
                n_resamples=n_resamples, seed=seed,
                ci_method=("not_estimable_n_lt_2" if len(values) < 2 else
                           "percentile_bootstrap_fixed_strata" if strata is not None else
                           "percentile_bootstrap"))


def relative_slowdown(shared, baseline, *, paired=False, seed=SEED,
                      n_resamples=N_RESAMPLES, shared_strata=None,
                      baseline_strata=None):
    """Elapsed ratio minus one; paired=True is median of trialwise ratios.

    The default is ratio of medians with independent resampling within each
    observed stratum. Callers must enforce matching base_id and workload first.
    """
    shared, baseline = _values(shared), _values(baseline)
    if not shared or not baseline:
        return dict(value=None, ci_low=None, ci_high=None, n_shared=len(shared),
                    n_baseline=len(baseline), method="missing_samples")
    if min(baseline) <= 0:
        raise ValueError("baseline elapsed must be positive")
    if paired:
        if len(shared) != len(baseline):
            raise ValueError("paired slowdown requires equal matched trial counts")
        if shared_strata is not None and baseline_strata is not None and list(shared_strata) != list(baseline_strata):
            raise ValueError("paired strata must match")
        ratios = [s/b-1 for s,b in zip(shared,baseline)]
        out = summarize(ratios, seed=seed, n_resamples=n_resamples, strata=shared_strata)
        return dict(value=out['median'], ci_low=out['ci_low'], ci_high=out['ci_high'],
                    n_shared=len(shared), n_baseline=len(baseline), method="median_paired_elapsed_ratios")
    sg, bg = _groups(shared, shared_strata), _groups(baseline, baseline_strata)
    rng = random.Random(seed)
    draws = [median(_draw(sg,rng))/median(_draw(bg,rng))-1 for _ in range(n_resamples)]
    low, high = (quantile(draws,.025), quantile(draws,.975)) if min(len(shared),len(baseline)) > 1 else (None,None)
    return dict(value=median(shared)/median(baseline)-1, ci_low=low, ci_high=high,
                n_shared=len(shared), n_baseline=len(baseline), method="ratio_of_elapsed_medians_minus_one")
