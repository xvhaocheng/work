#!/usr/bin/env python3
"""Lightweight, read-only statistics for the P4M formal60 recovery run.

Inputs (read-only, produced by the existing read-only summarizer):
    <run-dir>/trial_summary.json   (list of per-trial dicts; required)

Output:
    <run-dir>/statistics.json

Hard rules encoded here:
  * Primary set = the FIRST 24 trials only (8 balanced blocks x 3 conditions).
  * The 25th trial (formal-b09-g2000) is reported strictly separately and is
    never merged into the primary per-condition statistics.
  * Descriptive min/median/max only; no significance test, no bootstrap, no
    power claim, no outlier removal, no rerun, no relabel.

Usage:
    python3 scripts/statistics.py <run-dir>
"""
from __future__ import annotations

import json
import os
import sys
from statistics import median

MIB = 1024 * 1024
PRIMARY_N = 24  # frozen: first 24 trials = 8 balanced blocks (gap0/1000/2000 x 8)
CONDITIONS = ("gap0", "gap1000", "gap2000")
RATIO_CONDITIONS = ("gap1000", "gap2000")  # each paired to gap0 within the same block


def load_json(path):
    with open(path, "r", encoding="utf-8") as fh:
        return json.load(fh)


def mm(values):
    """min / median / max over mathematically defined values only."""
    vals = sorted(v for v in values if v is not None)
    if not vals:
        return {"n": 0, "n_missing": len(values), "min": None, "median": None, "max": None}
    return {
        "n": len(vals),
        "n_missing": len(values) - len(vals),
        "min": vals[0],
        "median": median(vals),
        "max": vals[-1],
    }


def col(trials, key):
    return [t.get(key) for t in trials]


def per_mib(trials, key):
    return [None if v is None else v / MIB for v in col(trials, key)]


def startup_col(trials, key):
    out = []
    for t in trials:
        sd = t.get("startup_domain") or {}
        out.append(sd.get(key))
    return out


def per_condition(trials):
    out = {}
    for cond in CONDITIONS:
        sel = [t for t in trials if t.get("condition") == cond]
        out[cond] = {
            "n_trials": len(sel),
            "v_cycles": mm(col(sel, "v_cycles")),
            "window_ns": mm(col(sel, "window_ns")),
            "v_mib_s": mm(per_mib(sel, "v_rate_bps")),
            "a_window_bytes": mm(col(sel, "a_window_bytes")),
            "a_window_mib_s": mm(per_mib(sel, "a_window_rate_bps")),
            "a_whole_hpm_active_cycles": mm(col(sel, "a_whole_hpm_active_cycles")),
            "a_whole_iotlb_miss": mm(col(sel, "aggressor_iotlb_miss")),
            "a_whole_s1_ptw": mm(col(sel, "aggressor_s1_ptw")),
            "a_whole_s2_ptw": mm(col(sel, "aggressor_s2_ptw")),
            "v_whole_iotlb_miss": mm(col(sel, "victim_iotlb_miss")),
            "v_whole_s1_ptw": mm(col(sel, "victim_s1_ptw")),
            "v_whole_s2_ptw": mm(col(sel, "victim_s2_ptw")),
            "startup_a_to_v_ns": mm(startup_col(sel, "a_to_v_ns")),
            "startup_irq_span_ns": mm(startup_col(sel, "irq_span_ns")),
        }
    return out


def paired_ratios(trials):
    blocks = {}
    for t in trials:
        blocks.setdefault(t.get("block"), {})[t.get("condition")] = t
    rows = []
    for b in sorted(k for k in blocks if k is not None):
        per = blocks[b]
        g0 = per.get("gap0")
        for cond in RATIO_CONDITIONS:
            cur = per.get(cond)
            row = {"block": b, "condition": cond, "baseline_present": bool(g0 and cur)}
            if g0 and cur:
                row["v_cycles_ratio"] = cur["v_cycles"] / g0["v_cycles"]
                row["a_window_rate_ratio"] = (
                    cur["a_window_rate_bps"] / g0["a_window_rate_bps"]
                )
            rows.append(row)
    return rows


def tradeoff(ratios):
    out = {}
    for cond in RATIO_CONDITIONS:
        v = [r["v_cycles_ratio"] for r in ratios
             if r["condition"] == cond and "v_cycles_ratio" in r]
        a = [r["a_window_rate_ratio"] for r in ratios
             if r["condition"] == cond and "a_window_rate_ratio" in r]
        if not v or not a:
            out[cond] = None
            continue
        v_med = median(v)
        a_med = median(a)
        out[cond] = {
            "n_paired_blocks": len(v),
            "v_cycles_ratio_median": v_med,
            "a_window_rate_ratio_median": a_med,
            "v_rate_improvement_pct": (1.0 / v_med - 1.0) * 100.0,
            "a_window_rate_loss_pct": (1.0 - a_med) * 100.0,
            "note": "same-block paired medians; not ratios of unpaired medians",
        }
    return out


def main(argv):
    if len(argv) != 2:
        print("usage: statistics.py <run-dir>", file=sys.stderr)
        return 2
    run_dir = argv[1]
    trials = load_json(os.path.join(run_dir, "trial_summary.json"))
    primary = trials[:PRIMARY_N]
    provisional = trials[PRIMARY_N:]
    ratios = paired_ratios(primary)

    result = {
        "schema": "p4m.formal60.statistics.v1",
        "run_dir": os.path.abspath(run_dir),
        "input": "trial_summary.json",
        "primary_n": len(primary),
        "provisional": [
            {
                "trial_id": t.get("trial_id"),
                "condition": t.get("condition"),
                "block": t.get("block"),
                "capture": t.get("capture"),
                "flags": t.get("flags"),
            }
            for t in provisional
        ],
        "primary_per_condition": per_condition(primary),
        "provisional_per_condition": per_condition(provisional),
        "paired_ratios": ratios,
        "tradeoff": tradeoff(ratios),
        "notes": [
            "v_mib_s = v_rate_bps / 2^20 = 16 MiB / window_s",
            "a_window_mib_s = a_window_rate_bps / 2^20 = a_window_bytes / window_s",
            "a_window progress is completed A jobs * chunk inside the window, not bus bytes",
            "descriptive only: no significance test, no bootstrap, no power claim, no exclusion",
            "primary = first 24 trials (8 balanced blocks); the 25th trial is reported separately",
        ],
    }

    out_path = os.path.join(run_dir, "statistics.json")
    with open(out_path, "w", encoding="utf-8") as fh:
        json.dump(result, fh, indent=2, ensure_ascii=False)
        fh.write("\n")
    print(out_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
