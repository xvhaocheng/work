#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Read-only summarizer for one already captured P4M formal60 UART log.

Not an acceptance or regression framework: it reads one log, writes one new
report directory and never runs, re-runs or modifies an experiment.  Only
anchored ``P4M: ...`` markers emitted by scripts/linux_p4m_formal60.sh are
parsed, so a leading login banner / typed command echo and a trailing base64
archive blob in the raw UART stream are ignored.
"""

import argparse
import json
import math
import os
import re
import statistics
import sys

TRIAL_RE = re.compile(r"^formal-b(\d+)-g(\d+)$")
TRANSFERS = 4096                       # frozen formal60 shape (PLAN.json)
WARMUP = 256
MIB = 1024.0 * 1024.0

ROW_FIELDS = (
    ("cli_success", "success"),
    ("result_status", "result.status"),
    ("hpm_configured", "result.hpm_configured"),
    ("measurement_started", "result.measurement_started"),
    ("measurement_complete", "result.measurement_complete"),
    ("victim_did", "result.victim_did"),
    ("aggressor_did", "result.aggressor_did"),
    ("victim_submitted", "result.victim.submitted"),
    ("victim_completed", "result.victim.completed"),
    ("victim_elapsed_ns", "result.victim_elapsed_ns"),
    ("victim_data_errors", "result.victim.data_errors"),
    ("victim_quarantined", "result.victim.quarantined"),
    ("victim_iotlb_miss", "result.victim.iotlb_miss"),
    ("victim_s1_ptw", "result.victim.s1_ptw"),
    ("victim_s2_ptw", "result.victim.s2_ptw"),
    ("aggressor_submitted", "result.aggressor.submitted"),
    ("aggressor_completed", "result.aggressor.completed"),
    ("aggressor_completed_bytes", "result.aggressor.completed_bytes"),
    ("aggressor_data_errors", "result.aggressor.data_errors"),
    ("aggressor_quarantined", "result.aggressor.quarantined"),
    ("aggressor_iotlb_miss", "result.aggressor.iotlb_miss"),
    ("aggressor_s1_ptw", "result.aggressor.s1_ptw"),
    ("aggressor_s2_ptw", "result.aggressor.s2_ptw"),
    ("sequence", "p4m.sequence"),
    ("v_cycles", "p4m.cycles"),
    ("window_ns", "p4m.hardware_window_ns"),
    ("a_window_jobs", "p4m.a_completed_jobs_window"),
    ("a_window_bytes", "p4m.a_progress_window_bytes"),
    ("a_window_rate_bps", "p4m.a_completed_bytes_per_second"),
    ("a_window_jobs_per_second", "p4m.a_completed_jobs_per_second"),
    ("a_started_before_window_bytes", "p4m.a_started_before_window_bytes"),
    ("observer_status", "p4m.observer_status"),
    ("flags_bits", "p4m.flags"),
    ("has_a", "p4m.has_a"),
    ("closed", "p4m.closed"),
    ("conservation", "p4m.conservation"),
    ("strict_preopening_inflight", "p4m.strict_preopening_inflight"),
    ("task_error_mask", "p4m.task_error_mask"),
    ("v_issued", "p4m.v_issued"),
    ("v_done", "p4m.v_done"),
    ("platform_hz", "p4m.platform_hz"),
    ("cycle_ns", "p4m.cycle_ns"),
)

# Hardware observer A domain, kept separate from the software startup domain.
HW_A_FIELDS = (
    ("issue_window", "p4m.a_issue_window"),
    ("done_window", "p4m.a_done_window"),
    ("inflight_at_issue", "p4m.a_inflight_at_issue"),
    ("inflight_at_done", "p4m.a_inflight_at_done"),
    ("started_before_window_completed", "p4m.a_started_before_window_completed"),
    ("opening_inflight_id", "p4m.a_opening_inflight_id"),
    ("closing_inflight_id", "p4m.a_closing_inflight_id"),
    ("first_done_id", "p4m.a_first_done_id"),
    ("last_done_id", "p4m.a_last_done_id"),
)

STARTUP_FIELDS = (
    ("seq", "seq"), ("method", "method"), ("v_did", "v_did"), ("a_did", "a_did"),
    ("has_a", "has_a"), ("a_triggered", "a_triggered"), ("v_triggered", "v_triggered"),
    ("a_id", "a_id"), ("v_id", "v_id"), ("a_begin_ns", "a_begin_ns"),
    ("a_end_ns", "a_end_ns"), ("v_begin_ns", "v_begin_ns"), ("v_end_ns", "v_end_ns"),
    ("a_to_v_ns", "a_to_v_ns"), ("irq_span_ns", "irq_span_ns"), ("error", "error"),
)


def as_int(text):
    """Decimal or 0x-prefixed integer, else None (mirrors the runner int_of)."""
    if text is None:
        return None
    s = str(text).strip()
    neg = s.startswith("-")
    if neg:
        s = s[1:]
    if s[:2].lower() == "0x":
        try:
            val = int(s, 16)
        except ValueError:
            return None
    elif s.isdigit():
        val = int(s, 10)
    else:
        return None
    return -val if neg else val


def parse_kv(line):
    """Collect key=value tokens whose key is a simple identifier."""
    out = {}
    for token in line.split():
        key, sep, val = token.partition("=")
        if sep and re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", key):
            out[key] = val
    return out


def dget(obj, path):
    cur = obj
    for key in path.split("."):
        if not isinstance(cur, dict) or key not in cur:
            return None
        cur = cur[key]
    return cur


def _is_number(value):
    """True only for real finite ints/floats; excludes bools and non-finite."""
    return (isinstance(value, (int, float)) and not isinstance(value, bool)
            and math.isfinite(float(value)))


def safe_ratio(num, den):
    """num/den as float; None unless both are finite numbers and den > 0."""
    if not _is_number(num) or not _is_number(den) or den <= 0:
        return None
    return num / den


def stats(values):
    """n/n_missing/min/max/median/mean/sampleSD/CV; only mathematically
    invalid entries (missing, non-numeric, non-finite) are ignored and counted
    in n_missing; no outlier filtering."""
    vals = [float(v) for v in values if _is_number(v)]
    out = {"n": len(vals), "n_missing": len(values) - len(vals),
           "min": None, "max": None, "median": None,
           "mean": None, "sd": None, "cv": None}
    if not vals:
        return out
    out["min"], out["max"] = min(vals), max(vals)
    out["median"] = statistics.median(vals)
    out["mean"] = sum(vals) / len(vals)
    if len(vals) >= 2:
        out["sd"] = statistics.stdev(vals)               # sample SD (n-1)
        if out["mean"] != 0:
            out["cv"] = out["sd"] / out["mean"]
    return out


def harvest_dmesg(line, part_time, seen_lines):
    """Historical dmesg repeats: dedup exact repeated lines, then group by seq.

    Different ``part=time`` records that share one seq are all retained as
    variants and the seq is flagged as conflicting; the first variant supplies
    the primary fields, so a later record is never a silent overwrite."""
    if line in seen_lines:
        return
    seen_lines.add(line)
    if "idma_batch_obs " not in line or "part=time" not in line:
        return
    fields = parse_kv(line)
    seq = as_int(fields.get("seq"))
    if seq is None:
        return
    fields["_raw"] = line
    entry = part_time.get(seq)
    if entry is None:
        part_time[seq] = {"fields": fields, "variants": [line], "conflict": False}
        return
    if line not in entry["variants"]:
        entry["variants"].append(line)
    if len(entry["variants"]) > 1:
        entry["conflict"] = True


def parse_log(text):
    """Walk the log once, following anchored markers and stdout/dmesg sections."""
    order, info = [], {}
    part_time, seen_lines = {}, set()
    outcome = {"final_marker_seen": False, "final_exit_rc": None,
               "host_exit_rc": None,
               "collection_failed": None, "complete_marker_seen": False,
               "cases_done": None, "cases_total": None, "mode": None}
    section, cur = None, None

    def slot(tid):
        if tid not in info:
            info[tid] = {"id": tid, "begin_line": None, "condition": None,
                         "kind": None, "mode": None, "stdout": [],
                         "stdout_begin_line": None, "stdout_end_line": None,
                         "exit": None, "startup": None, "case_end": None,
                         "failed": None, "dmesg_lines": 0}
            order.append(tid)
        return info[tid]

    for lineno, rawline in enumerate(text.splitlines(), 1):
        line = rawline.rstrip("\r")

        if section == "stdout":
            if re.match(r"P4M: case_stdout_end\b", line):
                section = None
                if cur is not None:
                    cur["stdout_end_line"] = lineno
            elif cur is not None:
                cur["stdout"].append(line)
            continue
        if section == "dmesg":
            if re.match(r"P4M: dmesg\.raw_end\b", line):
                section = None
            elif cur is not None:
                cur["dmesg_lines"] += 1
                harvest_dmesg(line, part_time, seen_lines)
            continue

        match = re.match(r"P4M: case_stdout_begin id=(\S+)", line)
        if match:
            cur = slot(match.group(1))
            cur["stdout_begin_line"] = lineno
            section = "stdout"
            continue
        match = re.match(r"P4M: dmesg\.raw_begin id=(\S+)", line)
        if match:
            cur = slot(match.group(1))
            section = "dmesg"
            continue
        match = re.match(r"P4M: case_begin id=(\S+) (.*)$", line)
        if match:
            cur = slot(match.group(1))
            kv = parse_kv(match.group(2))
            cur["begin_line"], cur["condition"] = lineno, kv.get("condition")
            cur["kind"], cur["mode"] = kv.get("kind"), kv.get("mode")
            continue
        match = re.match(r"P4M: case_startup id=(\S+) (.*)$", line)
        if match:
            cur = slot(match.group(1))
            cur["startup"] = parse_kv(match.group(2))
            continue
        match = re.match(r"P4M: case_exit id=(\S+) (.*)$", line)
        if match:
            cur = slot(match.group(1))
            cur["exit"] = as_int(parse_kv(match.group(2)).get("exit"))
            continue
        match = re.match(r"P4M: case_end id=(\S+) (.*)$", line)
        if match:
            cur = slot(match.group(1))
            cur["case_end"] = as_int(parse_kv(match.group(2)).get("exit"))
            continue
        match = re.match(r"P4M: case_failed id=(\S+) (.*)$", line)
        if match:
            cur = slot(match.group(1))
            cur["failed"] = as_int(parse_kv(match.group(2)).get("exit"))
            continue
        match = re.match(r"P4M: final_exit=(\S+) collection_failed=(\S+) mode=(\S+) "
                         r"cases_done=(\S+) total=(\S+)(?:\s+output=.*)?\s*$", line)
        if match:
            outcome["final_marker_seen"] = True
            outcome["final_exit_rc"] = as_int(match.group(1))
            outcome["collection_failed"] = match.group(2) != "0"
            outcome["mode"] = match.group(3)
            outcome["cases_done"] = as_int(match.group(4))
            outcome["cases_total"] = as_int(match.group(5))
            continue
        match = re.match(r"FORMAL60_HOST_EXIT=(\d+)\s*$", line)
        if match:
            outcome["host_exit_rc"] = int(match.group(1))
            continue
        if re.match(r"P4M: complete ok mode=", line):
            outcome["complete_marker_seen"] = True

    return order, info, part_time, outcome


def build_entry(index, trial, part_time, log_path):
    """Return (records.json entry, trial_summary.json row) for one trial."""
    rid = trial["id"]
    match = TRIAL_RE.match(rid)
    block = int(match.group(1)) if match else None
    gap = int(match.group(2)) if match else None

    raw_stdout = "\n".join(trial["stdout"])
    cli, parse_error = None, None
    if raw_stdout.strip():
        try:
            cli = json.loads(raw_stdout)               # ints keep full precision
        except ValueError:
            parse_error = "json_parse_error"
        if cli is not None and not isinstance(cli, dict):
            cli, parse_error = None, "json_not_object"

    capture = "complete"
    if trial["stdout_begin_line"] is None:
        capture = "missing"
    elif trial["stdout_end_line"] is None or cli is None:
        capture = "partial"

    record = {
        "index": index, "trial_id": rid, "block": block, "gap": gap,
        "condition": None if gap is None else "gap%d" % gap,
        "capture": capture,
        "case_exit": trial["exit"], "case_end": trial["case_end"],
        "case_failed": trial["failed"],
        "raw_source": {"log": log_path, "case_begin_line": trial["begin_line"],
                       "stdout_begin_line": trial["stdout_begin_line"],
                       "stdout_end_line": trial["stdout_end_line"]},
        "cli_json": cli,
    }
    if capture != "complete":
        record["cli_json_error"] = parse_error or (
            "no_stdout_block" if capture == "missing" else "stdout_block_incomplete")
        record["raw_stdout"] = raw_stdout

    startup = trial["startup"] or {}
    window_ns = dget(cli, "p4m.hardware_window_ns")
    window_negative = (isinstance(window_ns, (int, float))
                       and not isinstance(window_ns, bool) and window_ns < 0)
    window_s = (float(window_ns) / 1e9
                if isinstance(window_ns, (int, float))
                and not isinstance(window_ns, bool) and window_ns > 0 else None)
    victim_bytes = dget(cli, "result.victim.completed_bytes")

    row = {
        "index": index, "trial_id": rid, "block": block, "gap": gap,
        "condition": None if gap is None else "gap%d" % gap,
        "capture": capture, "case_begin_condition": trial["condition"],
        "case_exit": trial["exit"], "case_end": trial["case_end"],
        "case_failed": trial["failed"], "completed": trial["case_end"] == 0,
        "victim_completed_bytes": victim_bytes,
        "v_rate_bps": safe_ratio(victim_bytes, window_s),
        "a_whole_hpm_active_cycles": dget(cli, "result.aggressor.active_cycles"),
    }
    for key, path in ROW_FIELDS:
        row[key] = dget(cli, path)
    row["hw_a_domain"] = {k: dget(cli, p) for k, p in HW_A_FIELDS}
    row["startup_domain"] = {
        k: (startup.get(s) if k == "method" else as_int(startup.get(s)))
        for k, s in STARTUP_FIELDS}
    if row["a_window_rate_bps"] is None:
        row["a_window_rate_bps"] = safe_ratio(row["a_window_bytes"], window_s)

    seq = row["startup_domain"]["seq"]
    if seq is None:
        seq = row["sequence"]
    part_entry = part_time.get(seq) if seq is not None else None
    part = part_entry["fields"] if part_entry is not None else None
    row["batch_time"] = None
    row["batch_time_variants"] = part_entry["variants"] if part_entry else []
    row["batch_time_conflict"] = bool(part_entry and part_entry["conflict"])
    if part is not None:
        row["batch_time"] = {
            "seq": as_int(part.get("seq")), "valid": as_int(part.get("valid")),
            "status": as_int(part.get("status")), "v_did": as_int(part.get("v_did")),
            "a_did": as_int(part.get("a_did")),
            "issued": as_int(part.get("issued")), "done": as_int(part.get("done")),
            "cycles": as_int(part.get("cycles")),
            "batch_polled": as_int(part.get("batch_polled")),
            "done_to_read_cycles": as_int(part.get("done_to_read")),
            "raw": part.get("_raw"),
        }

    flags = []
    if capture != "complete":
        flags.append("capture_" + capture)
    if trial["exit"] not in (None, 0):
        flags.append("case_exit_%s" % trial["exit"])
    if trial["failed"] is not None:
        flags.append("case_failed_%s" % trial["failed"])
    if trial["case_end"] != 0:
        flags.append("no_case_end")
    if row["cli_success"] is not True:
        flags.append("cli_success_not_true")
    for side in ("victim", "aggressor"):
        for field in ("data_errors", "quarantined"):
            value = row["%s_%s" % (side, field)]
            if isinstance(value, (int, float)) and not isinstance(value, bool) and value:
                flags.append("%s_%s_%s" % (side, field, value))
    if window_negative:
        flags.append("window_ns_negative")
    if window_ns == 0:
        flags.append("window_ns_zero")
    if row["v_rate_bps"] is None:
        flags.append("v_rate_undefined")
    if row["a_window_rate_bps"] is None:
        flags.append("a_window_rate_undefined")
    if row["a_whole_hpm_active_cycles"] is None:
        flags.append("a_whole_hpm_missing")
    if not startup:
        flags.append("startup_missing")
    if row["startup_domain"]["seq"] is not None and row["sequence"] is not None \
            and row["startup_domain"]["seq"] != row["sequence"]:
        flags.append("startup_seq_vs_p4m_sequence_mismatch")
    if part is None:
        flags.append("part_time_missing")
    else:
        if row["batch_time_conflict"]:
            flags.append("part_time_seq_conflict")
        if as_int(part.get("valid")) != 1:
            flags.append("part_time_valid_not_1")
        if as_int(part.get("status")) != 4:
            flags.append("part_time_status_not_4")
        if as_int(part.get("issued")) != as_int(part.get("done")):
            flags.append("part_time_issued_done_mismatch")
    if row["task_error_mask"] not in (None, 0):
        flags.append("task_error_mask_nonzero")
    if row["conservation"] is False:
        flags.append("conservation_false")
    if row["closed"] is False:
        flags.append("not_closed")
    if row["v_issued"] != row["v_done"]:
        flags.append("v_issued_done_mismatch")
    if dget(cli, "input.victim_transfers") not in (None, TRANSFERS) or \
            dget(cli, "input.warmup_transfers") not in (None, WARMUP):
        flags.append("shape_not_4096_256")
    if gap is not None and dget(cli, "input.a_gap_cycles") not in (None, gap):
        flags.append("gap_vs_input_mismatch")
    row["flags"] = flags
    return record, row


def build_summary(log_path, rows, outcome, part_time_count):
    conditions = {}
    for gap in sorted({r["gap"] for r in rows if r["gap"] is not None}):
        sel = [r for r in rows if r["gap"] == gap]
        conditions["gap%d" % gap] = {
            "n_trials": len(sel),
            "n_capture_complete": sum(1 for r in sel if r["capture"] == "complete"),
            "n_case_end_ok": sum(1 for r in sel if r["case_end"] == 0),
            "n_cli_success_true": sum(1 for r in sel if r["cli_success"] is True),
            "n_with_flags": sum(1 for r in sel if r["flags"]),
            "metrics": {
                "v_cycles": stats([r["v_cycles"] for r in sel]),
                "window_ns": stats([r["window_ns"] for r in sel]),
                "v_rate_bps": stats([r["v_rate_bps"] for r in sel]),
                "a_window_rate_bps": stats([r["a_window_rate_bps"] for r in sel]),
                "a_window_jobs": stats([r["a_window_jobs"] for r in sel]),
                "a_whole_hpm_active_cycles": stats(
                    [r["a_whole_hpm_active_cycles"] for r in sel]),
                "v_iotlb_miss": stats([r["victim_iotlb_miss"] for r in sel]),
                "v_s1_ptw": stats([r["victim_s1_ptw"] for r in sel]),
                "v_s2_ptw": stats([r["victim_s2_ptw"] for r in sel]),
                "a_iotlb_miss": stats([r["aggressor_iotlb_miss"] for r in sel]),
                "a_s1_ptw": stats([r["aggressor_s1_ptw"] for r in sel]),
                "a_s2_ptw": stats([r["aggressor_s2_ptw"] for r in sel]),
            },
        }

    by_block = {}
    for r in rows:
        if r["block"] is not None and r["gap"] is not None:
            by_block.setdefault(r["block"], {})[r["gap"]] = r
    ratio_rows = []
    for block in sorted(by_block):
        base = by_block[block].get(0)
        for gap in sorted(g for g in by_block[block] if g):
            r = by_block[block][gap]
            ratio_rows.append({
                "block": block, "gap": gap, "condition": "gap%d" % gap,
                "baseline_present": base is not None,
                "v_cycles_ratio": safe_ratio(
                    r["v_cycles"], base["v_cycles"] if base else None),
                "a_window_rate_ratio": safe_ratio(
                    r["a_window_rate_bps"], base["a_window_rate_bps"] if base else None),
            })
    ratio_ranges = {}
    for gap in sorted({x["gap"] for x in ratio_rows}):
        sel = [x for x in ratio_rows if x["gap"] == gap]
        ratio_ranges["gap%d" % gap] = {
            "n_blocks": len(sel),
            "v_cycles_ratio": stats([x["v_cycles_ratio"] for x in sel]),
            "a_window_rate_ratio": stats([x["a_window_rate_ratio"] for x in sel]),
            "null_reason": "missing or zero gap0 baseline in the same block",
        }

    return {
        "log": log_path,
        "generated_by": "scripts/collect_p4m_formal60.py",
        "scope": "read-only summarizer; no rerun, no experiment change, no sample dropped; "
                 "kernel-read values assume a fixed 50 MHz reference and this is not independent acceptance",
        "expected_shape": {"transfers": TRANSFERS, "warmup": WARMUP,
                           "chunk_bytes": 4096, "aggressor_pages": 8},
        "outcome": outcome,
        "part_time_seqs_deduped": part_time_count,
        "counts": {
            "trials": len(rows),
            "capture_complete": sum(1 for r in rows if r["capture"] == "complete"),
            "capture_partial": sum(1 for r in rows if r["capture"] == "partial"),
            "capture_missing": sum(1 for r in rows if r["capture"] == "missing"),
            "case_end_ok": sum(1 for r in rows if r["case_end"] == 0),
            "cli_success_true": sum(1 for r in rows if r["cli_success"] is True),
            "with_flags": sum(1 for r in rows if r["flags"]),
            "scope_max_trials": 60,
            "trials_not_run": max(0, 60 - len(rows)),
        },
        "conditions": conditions,
        "ratios": {"by_block": ratio_rows, "per_condition": ratio_ranges},
        "notes": [
            "v_rate_bps = result.victim.completed_bytes / (p4m.hardware_window_ns / 1e9) = "
            "bytes/(ns/1e9), not bytes/ns (16 MiB for the formal60 shape)",
            "a_window_rate_bps is the A completion rate inside the P4M observer window; "
            "not AXI/bus throughput",
            "a_whole_hpm_active_cycles = result.aggressor.active_cycles (whole run, separate "
            "from the observer window)",
            "whole-run HPM counters for both sides: result.victim.* and result.aggressor.* "
            "(iotlb_miss, s1_ptw, s2_ptw)",
            "startup_domain is the software irq-pair domain; hw_a_domain is the hardware "
            "observer domain; not interchangeable",
            "batch_time is the dmesg idma_batch_obs part=time record for the trial seq; "
            "done_to_read_cycles is that line's done_to_read counter; batch_time.cycles is "
            "the same line's hardware cycles (compare with p4m.cycles)",
            "part=time records sharing one seq but differing are kept as batch_time_variants "
            "and flagged (part_time_seq_conflict); the first is shown, never a silent overwrite",
            "null means missing capture or mathematically undefined (window <= 0, zero baseline, "
            "absent field); only mathematically invalid values are ignored and n_missing counts them",
            "objective stats are descriptive only: no outlier filtering, no significance test, "
            "no rerun, no experiment control",
            "records.json is a list of untouched CLI JSON objects (parsed dicts only); captures.json "
            "keeps every trial wrapper including partial/failed with raw_stdout and line references",
            "trial count is capped at 60; unrun/uncaptured trials are reported as remaining, "
            "never counted as completed",
        ],
    }


def fmt(value, digits=3):
    if value is None:
        return "-"
    if isinstance(value, float):
        return ("%%.%df" % digits) % value
    return str(value)


def scaled(stat, factor):
    out = dict(stat)
    for key in ("min", "max", "median", "mean", "sd"):
        out[key] = None if stat[key] is None else stat[key] * factor
    return out


def range_cell(stat, digits=3):
    return "%s [%s .. %s]" % (fmt(stat["median"], digits),
                              fmt(stat["min"], digits),
                              fmt(stat["max"], digits))


def write_results_md(path, log_path, rows, summary, outcome, part_time_count):
    lines = []
    add = lines.append
    add("# P4M formal60 采集汇总")
    add("")
    add("仅对已采集日志做只读后处理：未重跑任何 trial，未修改实验条件，未删除或事后筛选任何样本。")
    add("")
    add("- 输入日志：`%s`" % log_path)
    add("- 结束标记：`P4M: final_exit=` %s；runner 最终退出码 = %s；"
        "host rc（`FORMAL60_HOST_EXIT`）= %s；collection_failed = %s；mode = %s"
        % ("出现" if outcome["final_marker_seen"] else "未出现",
           fmt(outcome["final_exit_rc"]), fmt(outcome["host_exit_rc"]),
           fmt(outcome["collection_failed"]), fmt(outcome["mode"])))
    add("- complete 标记：%s；cases_done/total = %s/%s"
        % ("出现" if outcome["complete_marker_seen"] else "未出现",
           fmt(outcome["cases_done"]), fmt(outcome["cases_total"])))
    add("- 解析 trial = %d（上限 60）；未运行/未采集剩余 = %d；dmesg part=time 去重后 seq = %d"
        % (len(rows), max(0, 60 - len(rows)), part_time_count))
    add("- 本报告不包含 call hash 校验，也不声称通过独立验收。")
    add("")
    add("## 1. 三条件汇总")
    add("")
    add("| 条件 | n | capture=complete | case_end=0 | 有标记 | V cycles 中位[min..max] | "
        "V MiB/s 中位[min..max] | A窗口 MiB/s 中位[min..max] | A整程 HPM cycles 中位 | "
        "A整程 iotlb_miss 中位 | A整程 s1_ptw 中位 | V cycles CV(分数) | A窗口速率 CV(分数) |")
    add("| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |")
    for cond in sorted(summary["conditions"]):
        c = summary["conditions"][cond]
        m = c["metrics"]
        v_rate = scaled(m["v_rate_bps"], 1.0 / MIB)
        a_rate = scaled(m["a_window_rate_bps"], 1.0 / MIB)
        add("| %s | %d | %d | %d | %d | %s | %s | %s | %s | %s | %s | %s | %s |" % (
            cond, c["n_trials"], c["n_capture_complete"], c["n_case_end_ok"],
            c["n_with_flags"],
            range_cell(m["v_cycles"], 0),
            range_cell(v_rate, 2),
            range_cell(a_rate, 2),
            fmt(m["a_whole_hpm_active_cycles"]["median"], 0),
            fmt(m["a_iotlb_miss"]["median"], 0),
            fmt(m["a_s1_ptw"]["median"], 0),
            fmt(m["v_cycles"]["cv"]), fmt(m["a_window_rate_bps"]["cv"])))
    add("")
    add("## 2. trial 明细（全部样本，无删除；上限 60）")
    add("")
    add("| # | trial | block | gap | seq | case_end | V cycles | V iotlb_miss | V s1_ptw | "
        "V s2_ptw | window ns | V MiB/s | A jobs | A bytes | A MiB/s | A iotlb_miss | "
        "A s1_ptw | A s2_ptw | startup a_to_v ns | startup irq_span ns | hwA opening id | "
        "part_time done_to_read | flags |")
    add("| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | "
        "--- | --- | --- | --- | --- | --- | --- | --- | --- | --- |")
    for r in rows:
        batch = r["batch_time"] or {}
        add("| %d | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | "
            "%s | %s | %s | %s | %s | %s | %s | %s |" % (
                r["index"] + 1, r["trial_id"], fmt(r["block"]), fmt(r["gap"]),
                fmt(r["sequence"]), fmt(r["case_end"]), fmt(r["v_cycles"], 0),
                fmt(r["victim_iotlb_miss"]), fmt(r["victim_s1_ptw"]),
                fmt(r["victim_s2_ptw"]), fmt(r["window_ns"], 0),
                fmt(None if r["v_rate_bps"] is None else r["v_rate_bps"] / MIB, 2),
                fmt(r["a_window_jobs"]), fmt(r["a_window_bytes"]),
                fmt(None if r["a_window_rate_bps"] is None
                    else r["a_window_rate_bps"] / MIB, 2),
                fmt(r["aggressor_iotlb_miss"]), fmt(r["aggressor_s1_ptw"]),
                fmt(r["aggressor_s2_ptw"]),
                fmt(r["startup_domain"]["a_to_v_ns"]),
                fmt(r["startup_domain"]["irq_span_ns"]),
                fmt(r["hw_a_domain"]["opening_inflight_id"]),
                fmt(batch.get("done_to_read_cycles")),
                ", ".join(r["flags"]) or "-"))
    add("")
    add("## 3. 块内比值（同一 block 内 gap/gap0）")
    add("")
    add("| 条件 | block 数 | V cycles 比值 min..max (中位) | "
        "A窗口速率比值 min..max (中位) | 空值原因 |")
    add("| --- | --- | --- | --- | --- |")
    for cond in sorted(summary["ratios"]["per_condition"]):
        rr = summary["ratios"]["per_condition"][cond]
        add("| %s | %d | %s .. %s (%s) | %s .. %s (%s) | %s |" % (
            cond, rr["n_blocks"],
            fmt(rr["v_cycles_ratio"]["min"]), fmt(rr["v_cycles_ratio"]["max"]),
            fmt(rr["v_cycles_ratio"]["median"]),
            fmt(rr["a_window_rate_ratio"]["min"]), fmt(rr["a_window_rate_ratio"]["max"]),
            fmt(rr["a_window_rate_ratio"]["median"]),
            rr["null_reason"]))
    add("")
    add("## 4. 说明")
    add("")
    add("- 以上为描述性统计（中位/最小/最大/均值/样本标准差/CV），不做显著性检验，不做因果结论。")
    add("- CV 以分数记录（= sd/mean）；表格表头标注 CV(分数)，转百分比时乘 100。")
    add("- 所有有效样本均保留并列出；未剔除离群点，未做任何事后筛选或重跑。")
    add("- 数学上无定义的字段（缺采集、分母为 0、窗口 <= 0）记为 null，表中显示为 `-`，原因见 flags；"
        "仅忽略数学上无定义的值，n_missing 记录被忽略数。")
    add("- `capture=partial/missing` 表示该 trial 的 CLI JSON 未完整捕获；原始行区间见 captures.json 的 "
        "raw_source，原文见 raw_stdout；records.json 只保留成功解析为 dict 的 CLI JSON。")
    add("- trial 上限 60；未运行/未采集的剩余 trial 按剩余数列出，不记为已完成。")
    add("- 软件（startup/irq-pair）A 域与硬件观察窗 A 域是两个分开的域：`startup_domain.a_id` 与 "
        "`hw_a_domain.opening_inflight_id` 不做等价比较。")
    add("- V 速率 = 16 MiB（4096 x 4096 B）/ (`p4m.hardware_window_ns` / 1e9)，即 bytes/(ns/1e9)，"
        "不是 bytes/ns；A 窗口速率是 P4M 观察窗内完成速率，不是 AXI/总线吞吐。")
    add("- A 整程 HPM 取 `result.aggressor.active_cycles`、`iotlb_miss`、`s1_ptw`、`s2_ptw`（整程，与窗口无关）。")
    add("- `batch_time.done_to_read_cycles` 取 `part=time` 行的 `done_to_read` 计数；`batch_time.cycles` 是"
        "同一行的硬件 cycles，可与 `p4m.cycles` 对照。")
    add("- 内核相关读数基于固定 50 MHz 假设；本报告只是只读后处理，不构成独立验收。")
    add("")
    with open(path, "w", encoding="utf-8") as handle:
        handle.write("\n".join(lines))


def write_json(path, obj):
    with open(path, "w", encoding="utf-8") as handle:
        json.dump(obj, handle, ensure_ascii=False, indent=2)
        handle.write("\n")


def main(argv=None):
    parser = argparse.ArgumentParser(
        description="Summarize one captured P4M formal60 UART log (read-only).")
    parser.add_argument("--log", required=True,
                        help="captured UART log containing anchored P4M markers")
    parser.add_argument("--out", required=True,
                        help="new output directory; must not already exist")
    args = parser.parse_args(argv)

    try:
        with open(args.log, "r", encoding="utf-8", errors="replace") as handle:
            text = handle.read()
    except OSError as exc:
        sys.stderr.write("collect_p4m_formal60: cannot read --log: %s\n" % exc)
        return 2

    try:
        os.makedirs(args.out)          # fail if it already exists
    except FileExistsError:
        sys.stderr.write("collect_p4m_formal60: --out already exists: %s\n" % args.out)
        return 2
    except OSError as exc:
        sys.stderr.write("collect_p4m_formal60: cannot create --out: %s\n" % exc)
        return 2

    order, info, part_time, outcome = parse_log(text)

    records, rows = [], []
    for trial_id in order:
        if not TRIAL_RE.match(trial_id):        # only formal-bNN-gGAP blocks
            continue
        record, row = build_entry(len(records), info[trial_id], part_time, args.log)
        records.append(record)
        rows.append(row)

    summary = build_summary(args.log, rows, outcome, len(part_time))
    write_json(os.path.join(args.out, "captures.json"), records)
    write_json(os.path.join(args.out, "records.json"),
               [r["cli_json"] for r in records if isinstance(r["cli_json"], dict)])
    write_json(os.path.join(args.out, "trial_summary.json"), rows)
    write_json(os.path.join(args.out, "summary.json"), summary)
    write_results_md(os.path.join(args.out, "RESULTS.md"),
                     args.log, rows, summary, outcome, len(part_time))
    return 0


if __name__ == "__main__":
    sys.exit(main())
