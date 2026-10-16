#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Review-scoped summariser for the frozen method analyzer output.

    summarize_method.py RESULTS_JSON --output NEWDIR [--method-dir DIR]

Reads only the frozen results.json written by scripts/research2_results.py and
writes new tables into a new, non-existing directory.  It never re-runs or
retries the board, never uses the network or a shell, and never rewrites its
input.

Scope: grouping and report tables only.  Board-side archiving / RTL checks are
out of scope.  The metrics.v_mib_s / a_mib_s versus record.raw P4M time/bytes
cross-check is deliberately left to the Codex verification step and is not
re-implemented here.

Raw 0xE25 is permission_closed_cycles; it is counted in the armed billing
interval, never an actual ADMIT-blocked or stall period, and it is not a
V-window wait ratio.  This script never renames it.
"""

from __future__ import annotations

import argparse
import csv
import json
import statistics
import sys
from pathlib import Path

EXPECTED_CASES = 462
EXPECTED_COMPARISONS = 88
EXPECTED_PHASES = {"boundary": 40, "calibration": 89, "formal": 192,
                   "holdout": 120, "reference": 12, "short": 9}
A_GAIN_THRESHOLD = 0.05
P5_METHOD = "p5"
DEFAULT_RESULTS = "out/method-results-review-20261004/offline-v2/results.json"

POLICY_COUNTER_FIELDS = (
    "complete_active_windows", "v_done_events", "v_done_bytes", "short_windows",
    "shortfall_bytes", "permission_closed_cycles", "total_charge",
    "total_replenish", "a_issue_events", "a_done_events", "charged_cycles",
    "a_lookup_hit_events", "a_lookup_miss_events", "a_ptw_return_events",
    "a_owner_cycles", "a_select_events", "a_lookup_events",
    "a_lookup_miss_events_dup", "a_req_await_cycles", "window_boundaries",
    "partial_window_boundaries", "feedback_increase_steps",
    "feedback_decrease_steps", "mode6_gap_loads", "mode7_stale_charges",
    "armed_cycles", "final_balance_signed", "b_min", "b_max", "b_final",
    "longest_vactive_no_done_cycles",
)

RUN_TSV_COLUMNS = (
    "id", "phase", "method", "target", "block", "a_pages", "reverse",
    "candidate_id", "selection_matched", "status",
    "window_cycles", "common_window_cycles", "billing_window_cycles",
    "v_mib_s", "a_mib_s", "v_done", "a_done",
    "v_done_events", "a_done_events", "complete_active_windows",
    "short_windows", "deficit_fraction", "shortfall_bytes",
    "permission_closed_cycles", "permission_closed_over_armed_cycles",
    "total_charge", "total_replenish", "charged_cycles", "armed_cycles",
    "final_balance_signed", "b_min", "b_max", "b_final",
    "longest_vactive_no_done_cycles", "feedback_increase_steps",
    "feedback_decrease_steps", "a_owner_cycles", "unknown_fields",
)

MISSING_TRACKED = tuple(
    name for name in RUN_TSV_COLUMNS
    if name not in ("id", "phase", "method", "target", "block", "a_pages",
                    "reverse", "candidate_id", "selection_matched", "status",
                    "unknown_fields"))

_ANY = object()


class SummaryError(Exception):
    """Unusable input or a failed frozen-summary assertion."""


def _num(value):
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        return None
    return value


def _metrics(case):
    metrics = case.get("metrics")
    return metrics if isinstance(metrics, dict) else {}


def metric_value(case, name):
    return _num(_metrics(case).get(name))


def _policy(case):
    policy = _metrics(case).get("policy")
    return policy if isinstance(policy, dict) else {}


def policy_value(case, name):
    counters = _policy(case).get("counters")
    value = _num(counters.get(name)) if isinstance(counters, dict) else None
    if value is None and name == "final_balance_signed":
        value = _num(_policy(case).get("final_balance_signed"))
    return value


def policy_histogram(case):
    hist = _policy(case).get("histogram")
    if isinstance(hist, list) and all(
            isinstance(item, int) and not isinstance(item, bool) for item in hist):
        return hist
    return None


def _ratio(numerator, denominator):
    if numerator is None or denominator is None or denominator <= 0:
        return None
    return numerator / float(denominator)


def deficit_fraction(case):
    return _ratio(policy_value(case, "short_windows"),
                  policy_value(case, "complete_active_windows"))


def permission_closed_over_armed_cycles(case):
    return _ratio(policy_value(case, "permission_closed_cycles"),
                  policy_value(case, "armed_cycles"))


def _run_row(case):
    row = {
        "id": case.get("id"),
        "phase": case.get("phase"),
        "method": case.get("method"),
        "target": case.get("target"),
        "block": case.get("block"),
        "a_pages": case.get("a_pages"),
        "reverse": case.get("reverse"),
        "candidate_id": case.get("candidate_id"),
        "selection_matched": case.get("selection_matched"),
        "status": case.get("status"),
        "window_cycles": metric_value(case, "window_cycles"),
        "common_window_cycles": metric_value(case, "common_window_cycles"),
        "billing_window_cycles": metric_value(case, "billing_window_cycles"),
        "v_mib_s": metric_value(case, "v_mib_s"),
        "a_mib_s": metric_value(case, "a_mib_s"),
        "v_done": metric_value(case, "v_done"),
        "a_done": metric_value(case, "a_done"),
        "v_done_events": policy_value(case, "v_done_events"),
        "a_done_events": policy_value(case, "a_done_events"),
        "complete_active_windows": policy_value(case, "complete_active_windows"),
        "short_windows": policy_value(case, "short_windows"),
        "deficit_fraction": deficit_fraction(case),
        "shortfall_bytes": policy_value(case, "shortfall_bytes"),
        "permission_closed_cycles": policy_value(case, "permission_closed_cycles"),
        "permission_closed_over_armed_cycles": permission_closed_over_armed_cycles(case),
        "total_charge": policy_value(case, "total_charge"),
        "total_replenish": policy_value(case, "total_replenish"),
        "charged_cycles": policy_value(case, "charged_cycles"),
        "armed_cycles": policy_value(case, "armed_cycles"),
        "final_balance_signed": policy_value(case, "final_balance_signed"),
        "b_min": policy_value(case, "b_min"),
        "b_max": policy_value(case, "b_max"),
        "b_final": policy_value(case, "b_final"),
        "longest_vactive_no_done_cycles": policy_value(case, "longest_vactive_no_done_cycles"),
        "feedback_increase_steps": policy_value(case, "feedback_increase_steps"),
        "feedback_decrease_steps": policy_value(case, "feedback_decrease_steps"),
        "a_owner_cycles": policy_value(case, "a_owner_cycles"),
    }
    row["unknown_fields"] = ";".join(
        name for name in MISSING_TRACKED if row.get(name) is None)
    return row


def _sort_token(value):
    if value is None:
        return (0, 0.0, "")
    if isinstance(value, bool):
        return (1, float(value), "")
    if isinstance(value, (int, float)):
        return (1, float(value), "")
    return (2, 0.0, str(value))


def _case_sort_key(case):
    return tuple(_sort_token(case.get(name)) for name in
                 ("phase", "target", "method", "a_pages", "reverse", "id"))


def _group_key(row):
    candidate_id = row["candidate_id"] if row["phase"] == "calibration" else None
    return (row["phase"], row["target"], row["method"], row["a_pages"],
            row["reverse"], candidate_id)


def _stats(values):
    present = [value for value in values if value is not None]
    if not present:
        return {"n": 0, "median": None, "min": None, "max": None}
    return {"n": len(present), "median": statistics.median(present),
            "min": min(present), "max": max(present)}


def _build_groups(rows):
    buckets = {}
    for row in rows:
        buckets.setdefault(_group_key(row), []).append(row)
    groups = []
    for key in sorted(buckets, key=lambda item: tuple(_sort_token(part) for part in item)):
        members = buckets[key]
        phase, target, method, a_pages, reverse, candidate_id = key
        groups.append({
            "phase": phase, "target": target, "method": method,
            "a_pages": a_pages, "reverse": reverse,
            "candidate_id": candidate_id,
            "n": len(members),
            "candidate_ids": sorted(
                {member["candidate_id"] for member in members if member["candidate_id"]},
                key=str),
            "v_mib_s": _stats([member["v_mib_s"] for member in members]),
            "a_mib_s_same_window": _stats([member["a_mib_s"] for member in members]),
            "common_window_cycles": _stats([member["common_window_cycles"] for member in members]),
            "complete_active_windows": _stats([member["complete_active_windows"] for member in members]),
            "short_windows": _stats([member["short_windows"] for member in members]),
            "deficit_fraction": _stats([member["deficit_fraction"] for member in members]),
            "permission_closed_cycles": _stats([member["permission_closed_cycles"] for member in members]),
            "permission_closed_over_armed_cycles": _stats([
                member["permission_closed_over_armed_cycles"] for member in members]),
            "longest_vactive_no_done_cycles": _stats([
                member["longest_vactive_no_done_cycles"] for member in members]),
            "b_min": _stats([member["b_min"] for member in members]),
            "b_max": _stats([member["b_max"] for member in members]),
            "b_final": _stats([member["b_final"] for member in members]),
            "final_balance_signed": _stats([member["final_balance_signed"] for member in members]),
        })
    return groups


def _collect_unknown(rows):
    counts = {name: 0 for name in MISSING_TRACKED}
    ids = {name: [] for name in MISSING_TRACKED}
    for row in rows:
        for name in MISSING_TRACKED:
            if row.get(name) is None:
                counts[name] += 1
                ids[name].append(row["id"])
    return counts, ids


def _windows_min(values):
    if not isinstance(values, list):
        return None
    clean = [value for value in values
             if isinstance(value, int) and not isinstance(value, bool)]
    return min(clean) if clean else None


def _bool_and(*values):
    if any(value is None for value in values):
        return None
    return all(bool(value) for value in values)


def _p5_row(row):
    gain = _num(row.get("a_gain_fraction"))
    threshold = _num(row.get("a_engineering_threshold"))
    if threshold is None:
        threshold = A_GAIN_THRESHOLD
    matched = row.get("protection_matched")
    matched = bool(matched) if isinstance(matched, bool) else None
    above = None if gain is None else (gain >= threshold)
    ci = row.get("difference_a_mib_s")
    ci_out = None
    if isinstance(ci, dict):
        low = _num(ci.get("low"))
        high = _num(ci.get("high"))
        ci_out = {
            "present": True,
            "mean_difference": _num(ci.get("mean_difference")),
            "low": low, "high": high, "n": ci.get("n"),
            "iterations": ci.get("iterations"), "seed": ci.get("seed"),
            "crosses_zero": None if (low is None or high is None) else (low <= 0.0 <= high),
            "entirely_positive": None if low is None else (low > 0.0),
            "entirely_negative": None if high is None else (high < 0.0),
        }
    return {
        "group": row.get("group"),
        "baseline": row.get("baseline"),
        "target": row.get("target"),
        "a_pages": row.get("a_pages"),
        "reverse": row.get("reverse"),
        "paired_rounds": row.get("paired_rounds"),
        "v_median_method": _num(row.get("v_median_method")),
        "v_median_baseline": _num(row.get("v_median_baseline")),
        "v_relative_diff": _num(row.get("v_relative_diff")),
        "v_within_5pct": row.get("v_within_5pct"),
        "target_met_method": row.get("target_met_method"),
        "target_met_baseline": row.get("target_met_baseline"),
        "both_target_met": _bool_and(row.get("target_met_method"), row.get("target_met_baseline")),
        "deficit_fraction_method": _num(row.get("deficit_fraction_method")),
        "deficit_fraction_baseline": _num(row.get("deficit_fraction_baseline")),
        "deficit_fraction_diff": _num(row.get("deficit_fraction_diff")),
        "deficit_within_10pct": row.get("deficit_within_10pct"),
        "full_windows_method_min": _windows_min(row.get("full_windows_method")),
        "full_windows_baseline_min": _windows_min(row.get("full_windows_baseline")),
        "meaningful_full_windows": row.get("meaningful_full_windows"),
        "protection_matched": matched,
        "a_gain_fraction": gain,
        "a_engineering_threshold": threshold,
        "a_gain_above_threshold": above,
        "engineering_pass_matched_and_gain": (
            None if (matched is None or above is None) else (matched and above)),
        "ci": ci_out,
        "gain_claim": row.get("gain_claim"),
        "calibration_selected_matched": row.get("calibration_selected_matched"),
    }


def _p5_sort_key(entry):
    return tuple(_sort_token(entry.get(name))
                 for name in ("target", "a_pages", "reverse", "baseline"))


def _build_comparisons(comparisons):
    raw = comparisons if isinstance(comparisons, list) else []
    table = {"formal": [], "holdout": [], "boundary": [], "other": []}
    for row in raw:
        if not isinstance(row, dict) or row.get("method") != P5_METHOD:
            continue
        table.get(row.get("group"), table["other"]).append(_p5_row(row))
    for name in table:
        table[name].sort(key=_p5_sort_key)
    return {
        "source": "results.json.comparisons",
        "observed_count": len(raw),
        "expected_count": EXPECTED_COMPARISONS,
        "count_match": len(raw) == EXPECTED_COMPARISONS,
        "raw_comparisons": raw,
        "p5_table": table,
        "p5_table_note": (
            "rows are exactly the p5 rows already present in the frozen comparisons; nothing is "
            "re-selected and no parameter is changed.  engineering_pass_matched_and_gain is "
            "protection_matched and a_gain_fraction >= threshold, kept separate from the CI "
            "crossing zero.  An unmatched protection is never a valid gain claim."),
    }


def _load_json(path):
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError as exc:
        raise SummaryError("results.json is unreadable: %s" % exc)
    try:
        doc = json.loads(text)
    except ValueError as exc:
        raise SummaryError("results.json is not valid JSON: %s" % exc)
    if not isinstance(doc, dict):
        raise SummaryError("results.json is not a JSON object")
    return doc


def _verify(doc):
    summary = doc.get("summary")
    cases = doc.get("cases")
    comparisons = doc.get("comparisons")
    problems = []
    if not isinstance(summary, dict):
        problems.append("results.json has no summary object")
    if not isinstance(cases, list):
        problems.append("results.json has no cases list")
    if not isinstance(comparisons, list):
        problems.append("results.json has no comparisons list")
    if problems:
        raise SummaryError("; ".join(problems))
    if summary.get("cases") != EXPECTED_CASES:
        problems.append("frozen summary cases=%r, expected %d"
                        % (summary.get("cases"), EXPECTED_CASES))
    if summary.get("ok") != EXPECTED_CASES:
        problems.append("frozen summary ok=%r, expected %d"
                        % (summary.get("ok"), EXPECTED_CASES))
    if summary.get("failed") != 0:
        problems.append("frozen summary failed=%r, expected 0" % (summary.get("failed"),))
    if summary.get("missing") != 0:
        problems.append("frozen summary missing=%r, expected 0" % (summary.get("missing"),))
    if summary.get("failed_ids"):
        problems.append("frozen summary lists failed ids: %r" % (summary.get("failed_ids"),))
    if summary.get("missing_ids"):
        problems.append("frozen summary lists missing ids: %r" % (summary.get("missing_ids"),))
    if len(cases) != EXPECTED_CASES:
        problems.append("results.json carries %d case records, expected %d"
                        % (len(cases), EXPECTED_CASES))
    ids = [case.get("id") if isinstance(case, dict) else None for case in cases]
    if any(not isinstance(case_id, str) or not case_id for case_id in ids):
        problems.append("at least one case has a missing or non-string id")
    elif len(set(ids)) != len(ids):
        problems.append("case ids are not unique")
    bad = sorted(str(case.get("id")) for case in cases
                 if isinstance(case, dict) and case.get("status") != "ok")
    if bad:
        problems.append("non-ok case statuses: %s" % ", ".join(bad))
    bad_metrics = sorted(str(case.get("id")) for case in cases
                         if not isinstance(case, dict)
                         or not isinstance(case.get("metrics"), dict))
    if bad_metrics:
        problems.append("cases without a metrics dict: %s" % ", ".join(bad_metrics))
    phase_counts = {}
    for case in cases:
        if isinstance(case, dict):
            phase = case.get("phase")
            phase_counts[phase] = phase_counts.get(phase, 0) + 1
    if phase_counts != EXPECTED_PHASES:
        problems.append("phase counts %r, expected %r" % (phase_counts, EXPECTED_PHASES))
    if len(comparisons) != EXPECTED_COMPARISONS:
        problems.append("comparisons carries %d rows, expected %d"
                        % (len(comparisons), EXPECTED_COMPARISONS))
    if problems:
        raise SummaryError("frozen assertions failed: " + "; ".join(problems))
    return summary


def _write_json(path, document):
    path.write_text(json.dumps(document, indent=2, ensure_ascii=False) + "\n",
                    encoding="utf-8")


def _tsv_cell(value):
    if value is None:
        return ""
    if isinstance(value, bool):
        return "true" if value else "false"
    return str(value)


def _write_runs_tsv(path, rows):
    lines = ["\t".join(RUN_TSV_COLUMNS)]
    for row in rows:
        lines.append("\t".join(_tsv_cell(row.get(column)) for column in RUN_TSV_COLUMNS))
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def _write_policy_counters(path, cases):
    header = ["id"] + list(POLICY_COUNTER_FIELDS) + [
        "histogram_sum", "common_window_cycles", "billing_window_cycles"]
    with path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(header)
        for case in cases:
            histogram = policy_histogram(case)
            row = [case.get("id")]
            row.extend(policy_value(case, name) for name in POLICY_COUNTER_FIELDS)
            row.append(sum(histogram) if histogram else None)
            row.append(metric_value(case, "common_window_cycles"))
            row.append(metric_value(case, "billing_window_cycles"))
            writer.writerow(row)


def _write_policy_histogram(path, cases):
    with path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle)
        writer.writerow(["id", "bucket", "count"])
        for case in cases:
            histogram = policy_histogram(case)
            if not histogram:
                continue
            for bucket, count in enumerate(histogram):
                writer.writerow([case.get("id"), bucket, count])


def _md_cell(value):
    if value is None:
        return "unknown"
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, float):
        return "%.6g" % value
    return str(value)


def _filter(groups, phase, target=_ANY, a_pages=_ANY, reverse=_ANY):
    return [group for group in groups
            if group["phase"] == phase
            and (target is _ANY or group["target"] == target)
            and (a_pages is _ANY or group["a_pages"] == a_pages)
            and (reverse is _ANY or group["reverse"] == reverse)]


def _distinct(groups, phase, field):
    return sorted({group[field] for group in groups if group["phase"] == phase},
                  key=_sort_token)


def render_markdown(results_path, output_dir, verification, groups, comparisons, unknown_counts):
    lines = []
    add = lines.append

    def table(header, rows):
        lines.append("| " + " | ".join(str(item) for item in header) + " |")
        lines.append("|" + "|".join("---" for _ in header) + "|")
        for row in rows:
            lines.append("| " + " | ".join(_md_cell(item) for item in row) + " |")

    def emit(title, group_list):
        add(title)
        add("")
        table(header, group_rows(group_list))
        add("")

    def group_rows(group_list):
        out = []
        for group in group_list:
            v = group["v_mib_s"]
            a = group["a_mib_s_same_window"]
            full = group["complete_active_windows"]
            deficit = group["deficit_fraction"]
            closed = group["permission_closed_over_armed_cycles"]
            out.append([group["phase"], group["target"], group["method"],
                        group["a_pages"], group["reverse"], group["candidate_id"],
                        group["n"], v["median"], v["min"], v["max"],
                        a["median"], a["min"], a["max"], full["median"],
                        deficit["median"], deficit["min"], deficit["max"],
                        closed["median"]])
        return out

    header = ["phase", "target", "method", "a_pages", "reverse", "候选", "n",
              "V中位", "V最小", "V最大", "A中位", "A最小", "A最大", "完整窗中位",
              "不足比例中位", "不足比例最小", "不足比例最大", "许可关闭/armed中位"]

    add("# method 结果汇总（仅数字表格，不构成任何硬件结论）")
    add("")
    add("- 输入: %s" % results_path)
    add("- 输出: %s" % output_dir)
    add("- 生成: out/method-results-review-20261004/summarize_method.py（只读输入；不重跑、不联网、不 shell）")
    add("")
    asserts = verification["asserts"]
    add("## 1 冻结校验")
    add("")
    add("- cases=%s ok=%s failed=%s missing=%s unique_ids=%s case_records=%s" % (
        asserts["summary_cases"], asserts["summary_ok"], asserts["summary_failed"],
        asserts["summary_missing"], asserts["unique_ids"], asserts["case_records"]))
    add("- comparisons=%s/%s phase_counts_match=%s" % (
        verification["comparisons_observed"], verification["comparisons_expected"],
        verification["phase_counts_match"]))
    add("")
    table(["phase", "n"], sorted(verification["phase_counts"].items(),
                                 key=lambda item: _sort_token(item[0])))
    add("")
    add("## 2 条件分组统计（phase/target/method/a_pages/reverse）")
    add("")
    add("组内为 run 级中位/最小/最大；A 为同一硬件共同窗 MiB/s；"
        "不足比例=short_windows/complete_active_windows；"
        "许可关闭比=permission_closed_cycles/armed_cycles（0xE25 按 armed 区间计费，"
        "不是实际 stall，也不是 V 窗口等待比例）。"
        "calibration 组按候选分开，逐候选证据见 runs.tsv。")
    add("")
    for target in _distinct(groups, "formal", "target"):
        emit("### 2.1 formal target=%s" % _md_cell(target),
             _filter(groups, "formal", target=target))
    for target in _distinct(groups, "holdout", "target"):
        for a_pages in _distinct(groups, "holdout", "a_pages"):
            for reverse in _distinct(groups, "holdout", "reverse"):
                block = _filter(groups, "holdout", target=target, a_pages=a_pages, reverse=reverse)
                if block:
                    emit("### 2.2 holdout target=%s a_pages=%s reverse=%s"
                         % (_md_cell(target), _md_cell(a_pages), _md_cell(reverse)), block)
    for a_pages in _distinct(groups, "boundary", "a_pages"):
        emit("### 2.3 boundary a_pages=%s" % _md_cell(a_pages),
             _filter(groups, "boundary", a_pages=a_pages))
    emit("### 2.4 calibration", _filter(groups, "calibration"))
    emit("### 2.5 short", _filter(groups, "short"))
    emit("### 2.6 reference", _filter(groups, "reference"))

    add("## 3 p5 公平比较（已有比较表，按 group 分开；不做跨 phase 合并）")
    add("")
    add("工程通过 = protection_matched 且 A 增益>=5%；CI 是否跨 0 单独一列，二者不合并，也不作显著性宣称。")
    add("")
    p5_header = ["target", "a_pages", "reverse", "baseline", "配对数", "V方法", "V基线",
                 "V相对差", "V<=5%", "达标方法", "达标基线", "不足比例方法", "不足比例基线",
                 "不足差", "不足差<=10pt", "完整窗最小方法", "完整窗最小基线", "保护匹配",
                 "A增益", ">=5%", "工程通过", "CI低", "CI高", "CI跨0", "校准matched"]
    for index, group_name in enumerate(("formal", "holdout", "boundary"), start=1):
        add("### 3.%d p5 vs baseline: %s" % (index, group_name))
        add("")
        table_rows = []
        for row in comparisons["p5_table"].get(group_name) or []:
            ci = row.get("ci") or {}
            table_rows.append([
                row["target"], row["a_pages"], row["reverse"], row["baseline"],
                row["paired_rounds"], row["v_median_method"], row["v_median_baseline"],
                row["v_relative_diff"], row["v_within_5pct"], row["target_met_method"],
                row["target_met_baseline"], row["deficit_fraction_method"],
                row["deficit_fraction_baseline"], row["deficit_fraction_diff"],
                row["deficit_within_10pct"], row["full_windows_method_min"],
                row["full_windows_baseline_min"], row["protection_matched"],
                row["a_gain_fraction"], row["a_gain_above_threshold"],
                row["engineering_pass_matched_and_gain"], ci.get("low"), ci.get("high"),
                ci.get("crosses_zero"), row.get("calibration_selected_matched")])
        table(p5_header, table_rows)
        add("")

    add("## 4 缺失数据（unknown，绝不当作 0）")
    add("")
    table(["字段", "缺失 run 数"],
          sorted(((name, count) for name, count in unknown_counts.items() if count),
                 key=lambda item: -item[1]))
    add("")
    add("缺失字段在 runs.tsv 为空白、在 JSON 为 null；逐 run 缺失清单见 verification.json。")
    add("")
    add("## 5 限制")
    add("")
    add("- metrics.v_mib_s / a_mib_s 与原始 record.raw 的 P4M 时间/字节一致性 = 待 Codex 关键验证，本脚本不做。")
    add("- 0xE25 = permission_closed_cycles，来自 armed 计费区间；不是实际 ADMIT 阻塞或 stall 等待周期，也不是 V 窗口等待比例。")
    add("- 未做跨启动/跨 phase 合并；未合并旧 E2；不做板端归档或 RTL 验证。")
    add("- 逐 run 明细见 runs.tsv / policy_counters.csv / policy_histogram.csv / comparisons.json。")
    return "\n".join(lines) + "\n"


def summarize(results_path, output_dir, method_dir=None):
    results_path = Path(results_path)
    output = Path(output_dir)
    if not results_path.is_file():
        raise SummaryError("results.json not found: %s" % results_path)
    if output.exists():
        raise SummaryError("output directory already exists: %s" % output)
    input_dir = results_path.resolve().parent
    output_resolved = output.resolve()
    if output_resolved == input_dir or input_dir in output_resolved.parents:
        raise SummaryError("output directory must not be the input directory or a subdirectory of it")
    if method_dir:
        method_root = Path(method_dir).resolve()
        if output_resolved == method_root or method_root in output_resolved.parents:
            raise SummaryError("output directory must not be the method directory or a subdirectory of it")

    doc = _load_json(results_path)
    summary = _verify(doc)
    cases = sorted((case for case in doc["cases"] if isinstance(case, dict)), key=_case_sort_key)
    rows = [_run_row(case) for case in cases]
    groups = _build_groups(rows)
    unknown_counts, unknown_ids = _collect_unknown(rows)
    comparisons = _build_comparisons(doc.get("comparisons"))

    phase_counts = {}
    for case in cases:
        phase = case.get("phase")
        phase_counts[phase] = phase_counts.get(phase, 0) + 1

    verification = {
        "results_json": str(results_path),
        "output_dir": str(output),
        "method_dir": str(method_dir) if method_dir else None,
        "generated_by": "out/method-results-review-20261004/summarize_method.py",
        "asserts": {
            "summary_cases": summary.get("cases"),
            "summary_ok": summary.get("ok"),
            "summary_failed": summary.get("failed"),
            "summary_missing": summary.get("missing"),
            "summary_failed_ids": summary.get("failed_ids"),
            "summary_missing_ids": summary.get("missing_ids"),
            "case_records": len(cases),
            "unique_ids": len({case.get("id") for case in cases}),
            "expected": {"cases": EXPECTED_CASES, "ok": EXPECTED_CASES,
                         "failed": 0, "missing": 0},
        },
        "phase_counts": phase_counts,
        "expected_phase_counts": EXPECTED_PHASES,
        "phase_counts_match": phase_counts == EXPECTED_PHASES,
        "comparisons_observed": comparisons["observed_count"],
        "comparisons_expected": EXPECTED_COMPARISONS,
        "comparisons_match": comparisons["count_match"],
        "stopped_ids": summary.get("stopped"),
        "unmatched_selections": summary.get("unmatched_selections"),
        "unknown_counts": unknown_counts,
        "unknown_ids": unknown_ids,
        "deferred_to_codex": [
            "metrics.v_mib_s / a_mib_s versus the original record.raw P4M time/bytes consistency",
        ],
        "notes": [
            "this summariser only groups the already-frozen evidence; it never re-runs, retries, builds or selects",
            "no cross-launch, no cross-phase and no old-E2 pooling",
            "raw 0xE25 is permission_closed_cycles, never an actual ADMIT-blocked or stall period",
            "a missing field is unknown (empty in TSV, null in JSON), never 0",
            "an unmatched protection is never a valid A-gain claim",
        ],
    }

    groups_doc = {
        "group_by": ("phase/target/method/a_pages/reverse/candidate_id "
                     "(candidate_id only for calibration; None otherwise)"),
        "n_groups": len(groups),
        "groups": groups,
        "phase_counts": phase_counts,
        "deficit_fraction_note": "deficit_fraction = short_windows / complete_active_windows (per run)",
        "permission_closed_note": (
            "permission_closed_over_armed_cycles = permission_closed_cycles / armed_cycles; "
            "permission_closed_cycles is measured in the armed billing interval and common_window_cycles "
            "is a different interval, so they are never divided; the ratio is not an actual "
            "ADMIT-blocked/stall period and not a V-window wait ratio"),
        "pooling_note": (
            "no cross-launch, no cross-phase and no old-E2 pooling; calibration groups are split by "
            "candidate_id and per-run candidate evidence is in runs.tsv"),
    }

    try:
        output.mkdir()
    except OSError as exc:
        raise SummaryError("cannot create output directory %s: %s" % (output, exc))
    _write_runs_tsv(output / "runs.tsv", rows)
    _write_policy_counters(output / "policy_counters.csv", cases)
    _write_policy_histogram(output / "policy_histogram.csv", cases)
    _write_json(output / "groups.json", groups_doc)
    _write_json(output / "comparisons.json", comparisons)
    _write_json(output / "verification.json", verification)
    (output / "summary.zh.md").write_text(
        render_markdown(results_path, output, verification, groups, comparisons, unknown_counts),
        encoding="utf-8")
    return verification


def main(argv=None):
    parser = argparse.ArgumentParser(
        prog="summarize_method.py",
        description="Group the frozen results.json into review tables (read-only).")
    parser.add_argument("results_json", nargs="?", default=DEFAULT_RESULTS)
    parser.add_argument("--output", required=True, metavar="NEWDIR")
    parser.add_argument("--method-dir", default=None,
                        help="original method run path, recorded only and never read")
    args = parser.parse_args(argv)
    try:
        verification = summarize(args.results_json, args.output, args.method_dir)
    except SummaryError as exc:
        print("summarize_method: %s" % exc, file=sys.stderr)
        return 2
    except OSError as exc:
        print("summarize_method: output failed: %s" % exc, file=sys.stderr)
        return 4
    asserts = verification["asserts"]
    print("summarize_method: cases=%s ok=%s comparisons=%s out=%s" % (
        asserts["summary_cases"], asserts["summary_ok"],
        verification["comparisons_observed"], args.output), file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
