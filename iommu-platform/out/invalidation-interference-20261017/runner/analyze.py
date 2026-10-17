#!/usr/bin/env python3
"""Validate existing results and draw the fixed invalidation comparison.

This script never builds or runs a simulation. It requires real summary/trace
files and does not synthesize missing results. Extra walk counts are arithmetic
differences from M0, separate from the RTL invalidation-attribution counter.
"""
from __future__ import annotations

import argparse
import csv
from dataclasses import dataclass
from datetime import datetime, timezone
import gzip
import hashlib
import json
import math
from pathlib import Path
import tempfile
from typing import Any, Iterator

MODES = ("m0", "m1", "m2", "m3", "m4-16", "m4-64")
DOMAINS = ("same", "different")
FOOTPRINTS = (4, 8)
CHECKS = ("passed", "data", "quiet", "commands", "warm_resident", "warm_no_extra_walks")
FIXED_CONFIG = {"a_jobs": 64, "b_jobs": 256, "bytes_per_copy": 4096,
                "ddr_first_cycles": 16, "ddr_beat_cycles": 1}
COUNT_METRICS = (
    "A_hits", "A_misses", "A_walks", "A_invalidation_attributed_misses",
    "A_cold_misses", "A_replacement_misses", "A_ptw_memory_bytes",
    "B_hits", "B_misses", "B_walks", "B_invalidation_attributed_misses",
    "B_cold_misses", "B_replacement_misses", "B_ptw_memory_bytes",
    "cq_iotinval", "cq_iofence", "cq_decode_count", "cq_submit_commands",
    "cq_fence_completed", "cq_doorbells", "cq_peak", "cq_entry_cycles",
    "cq_rounds", "cq_total_waiting", "map_regions",
    "unmap_regions", "A_completed", "B_completed",
    "inval_post_edge_verified_events", "inval_post_edge_verified_ways",
)
TRACE_COLUMNS = {"cycle", "event", "lane", "job", "address", "value", "aux", "era"}
TRACE_COLUMN_ORDER = tuple(sorted(TRACE_COLUMNS))


class Invalid(ValueError):
    pass


def digest(path: Path) -> str:
    value = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            value.update(block)
    return value.hexdigest()


def integer(value: Any, label: str) -> int:
    if isinstance(value, bool):
        raise Invalid(f"{label}: boolean is not an integer counter")
    if isinstance(value, int):
        result = value
    elif isinstance(value, str):
        try:
            result = int(value, 0)
        except ValueError:
            try:
                result = int(value, 10)
            except ValueError as exc:
                raise Invalid(f"{label}: invalid integer {value!r}") from exc
    else:
        raise Invalid(f"{label}: expected integer, got {value!r}")
    if result < 0:
        raise Invalid(f"{label}: negative counter {result}")
    return result


def time_value(value: Any, label: str) -> float | int:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise Invalid(f"{label}: missing/non-numeric time {value!r}")
    if not math.isfinite(value) or value < 0:
        raise Invalid(f"{label}: time must be finite and nonnegative")
    return value


@dataclass
class Case:
    path: Path
    data: dict
    config: dict
    metrics: dict
    trace: dict | None = None

    @property
    def case_id(self) -> str:
        return self.config["case_id"]

    @property
    def key(self) -> tuple[str, int, str, str]:
        c = self.config
        return c["domain"], c["b_pages"], c["mode"], c["map_policy"]


def expected_counts(config: dict) -> dict[str, int]:
    """Frozen source-derived counts; never fit these to observed results."""
    mode, policy = config["mode"], config["map_policy"]
    if mode == "m0":
        regions = rounds = 0
    else:
        regions = 2 * FIXED_CONFIG["a_jobs"]
        map_rounds = regions if policy == "full-api" else 0
        batch = int(mode.split("-")[1]) if mode.startswith("m4-") else 1
        unmap_rounds = (regions + batch - 1) // batch
        rounds = map_rounds + unmap_rounds
    return {"map_regions": regions, "unmap_regions": regions,
            "cq_rounds": rounds, "cq_iotinval": rounds,
            "cq_iofence": 2 * rounds, "cq_doorbells": 2 * rounds,
            "cq_fence_completed": 2 * rounds,
            "inval_post_edge_verified_events": rounds,
            "cq_decode_count": 3 * rounds, "cq_submit_commands": 3 * rounds}


def load_case(path: Path) -> Case:
    try:
        data = json.loads(path.read_text())
    except (OSError, ValueError) as exc:
        raise Invalid(f"{path}: cannot read summary: {exc}") from exc
    if not isinstance(data, dict) or data.get("schema_version") != 1:
        raise Invalid(f"{path}: expected schema_version=1")
    if not isinstance(data.get("config"), dict) or not isinstance(data.get("metrics"), dict):
        raise Invalid(f"{path}: config and metrics objects are required")
    case = Case(path, data, dict(data["config"]), dict(data["metrics"]))
    c, m = case.config, case.metrics
    if c.get("domain") not in DOMAINS or c.get("b_pages") not in FOOTPRINTS:
        raise Invalid(f"{path}: invalid domain/footprint")
    if c.get("mode") not in MODES or c.get("map_policy") not in ("full-api", "unmap-only"):
        raise Invalid(f"{path}: invalid mode/map_policy")
    expected_id = f"{c['domain']}-b{c['b_pages']}-{c['mode']}-{c['map_policy']}"
    if c.get("case_id") != expected_id:
        raise Invalid(f"{path}: case_id must be {expected_id!r}")
    for key, value in FIXED_CONFIG.items():
        if isinstance(c.get(key), bool) or c.get(key) != value:
            raise Invalid(f"{case.case_id}: {key} must remain {value}, got {c.get(key)!r}")
    if c["map_policy"] == "unmap-only" and c["mode"] != "m1":
        raise Invalid(f"{case.case_id}: optional unmap-only sensitivity is M1 only")
    if "failure" not in data or data["failure"] is not None or ("exit_code" in data and data["exit_code"] != 0):
        raise Invalid(f"{case.case_id}: runner failure: {data.get('failure')!r}")
    checks = data.get("checks", {})
    if not isinstance(checks, dict):
        raise Invalid(f"{case.case_id}: checks object is required")
    for key in CHECKS:
        if checks.get(key) is not True:
            raise Invalid(f"{case.case_id}: check {key!r} is missing or false")
    failed_extra = [key for key, value in checks.items() if value is False]
    if failed_extra:
        raise Invalid(f"{case.case_id}: additional failed checks {failed_extra}")
    for key in COUNT_METRICS:
        if key not in m:
            raise Invalid(f"{case.case_id}: missing real metric {key}")
        m[key] = integer(m[key], f"{case.case_id}.{key}")
    for key in ("T_A", "T_B", "T_end", "A_final_flush_end"):
        m[key] = time_value(m.get(key), f"{case.case_id}.{key}")
    if m["T_B"] <= 0:
        raise Invalid(f"{case.case_id}: B completion time must be positive")
    if m["A_final_flush_end"] < m["T_A"] or m["T_end"] < max(m["A_final_flush_end"], m["T_B"]):
        raise Invalid(f"{case.case_id}: complete time excludes a final copy/flush")
    if m["A_completed"] != c["a_jobs"] or m["B_completed"] != c["b_jobs"]:
        raise Invalid(f"{case.case_id}: incomplete A/B work")
    for lane in ("A", "B"):
        if m[f"{lane}_misses"] != sum(m[f"{lane}_{kind}"] for kind in ("invalidation_attributed_misses", "cold_misses", "replacement_misses")):
            raise Invalid(f"{case.case_id}: {lane} miss attribution does not partition misses")
    for key, value in expected_counts(c).items():
        if m[key] != value:
            raise Invalid(f"{case.case_id}: real {key}={m[key]}, frozen expectation={value}; report mismatch instead of adjusting expectation")
    if c["mode"] == "m0" and (m["A_invalidation_attributed_misses"] or m["B_invalidation_attributed_misses"]):
        raise Invalid(f"{case.case_id}: no-invalidation M0 has invalidation-attributed misses")
    return case


def iter_trace(path: Path) -> Iterator[dict]:
    opener = gzip.open if path.suffix == ".gz" else open
    with opener(path, "rt", newline="") as f:
        if ".jsonl" in path.suffixes:
            for number, line in enumerate(f, 1):
                if not line.strip():
                    continue
                try:
                    item = json.loads(line)
                except ValueError as exc:
                    raise Invalid(f"{path}:{number}: invalid JSON event") from exc
                if not isinstance(item, dict) or not TRACE_COLUMNS <= item.keys():
                    raise Invalid(f"{path}:{number}: missing shared trace columns")
                yield item
        else:
            reader = csv.DictReader(f)
            if not TRACE_COLUMNS <= set(reader.fieldnames or []):
                raise Invalid(f"{path}: missing shared trace columns including era")
            yield from reader


def verify_trace(case: Case, path: Path) -> dict:
    counts = {"map_regions": 0, "unmap_regions": 0, "cq_iotinval": 0,
              "cq_iofence": 0, "cq_decode_count": 0, "cq_submit_commands": 0,
              "cq_doorbells": 0, "cq_rounds": 0, "cq_fence_completed": 0}
    counts["inval_post_edge_verified_events"] = 0
    counts["inval_post_edge_verified_ways"] = 0
    lane_counts = {lane: {key: 0 for key in ("hits", "misses", "walks", "invalidation_attributed_misses", "cold_misses", "replacement_misses", "ptw_memory_bytes")} for lane in ("A", "B")}
    launches = {lane: set() for lane in ("A", "B")}
    completions = {lane: {} for lane in ("A", "B")}
    measure_begin = []
    measure_end = []
    a_final = []
    round_waits = []
    trace_hash = hashlib.sha256()
    submitted_inval = submitted_fences = posted_sum = waiting = measured = 0
    previous_cycle = -1
    previous_round = None
    sequence = (1, 2, 2)
    for event in iter_trace(path):
        normalized = [str(event[key]) for key in TRACE_COLUMN_ORDER]
        trace_hash.update(json.dumps(normalized, separators=(",", ":")).encode()+b"\n")
        if event["era"] != "measure":
            continue
        measured += 1
        cycle = integer(event["cycle"], f"{path}.cycle")
        if cycle < previous_cycle:
            raise Invalid(f"{case.case_id}: measurement events are not ordered by cycle")
        previous_cycle = cycle
        name = event["event"]
        if name in ("fault", "cq_error", "failure"):
            raise Invalid(f"{case.case_id}: raw trace contains {name}")
        if name in ("lookup", "walk", "ptw_read", "copy_launch", "copy_done"):
            lane_id = integer(event["lane"], f"{case.case_id}.{name}.lane")
            if lane_id not in (0, 1):
                raise Invalid(f"{case.case_id}: invalid raw lane {lane_id}")
            lane = "B" if lane_id == 0 else "A"
            if name == "lookup":
                hit = integer(event["value"], f"{case.case_id}.lookup.hit")
                cause = integer(event["aux"], f"{case.case_id}.lookup.cause") >> 20
                if hit not in (0, 1) or (hit == 1 and cause != 0) or (hit == 0 and cause not in (1, 2, 3)):
                    raise Invalid(f"{case.case_id}: invalid raw lookup/cause encoding")
                lane_counts[lane]["hits" if hit else "misses"] += 1
                if not hit:
                    lane_counts[lane][{1: "invalidation_attributed_misses", 2: "cold_misses", 3: "replacement_misses"}[cause]] += 1
            elif name == "walk":
                lane_counts[lane]["walks"] += 1
            elif name == "ptw_read":
                value = integer(event["value"], f"{case.case_id}.ptw_read.bytes")
                if value != 8:
                    raise Invalid(f"{case.case_id}: PTW read is not one 64-bit PTE")
                lane_counts[lane]["ptw_memory_bytes"] += value
            else:
                job = integer(event["job"], f"{case.case_id}.{name}.job")
                if name == "copy_launch":
                    if job in launches[lane]:
                        raise Invalid(f"{case.case_id}: duplicate {lane} COPY launch {job}")
                    launches[lane].add(job)
                else:
                    if job in completions[lane] or job not in launches[lane]:
                        raise Invalid(f"{case.case_id}: duplicate/unlaunched {lane} COPY completion {job}")
                    completions[lane][job] = cycle
        elif name == "measure_begin":
            measure_begin.append(cycle)
        elif name == "measure_end":
            measure_end.append(cycle)
        elif name == "a_final_flush_complete":
            a_final.append((cycle, integer(event["value"], f"{case.case_id}.a_final.duration")))
        elif name in ("map_region", "unmap_region"):
            if str(event["lane"]) not in ("1", "A", "a"):
                raise Invalid(f"{case.case_id}: measured region operation is not lane A")
            if integer(event["aux"], f"{case.case_id}.region_bytes") != FIXED_CONFIG["bytes_per_copy"]:
                raise Invalid(f"{case.case_id}: measured region is not 4KiB")
            counts["map_regions" if name == "map_region" else "unmap_regions"] += 1
        elif name in ("cq_decode", "cq_submit"):
            opcode = integer(event["value"], f"{case.case_id}.{name}.opcode")
            if integer(event["aux"], f"{case.case_id}.{name}.func3") != 0 or opcode not in (1, 2):
                raise Invalid(f"{case.case_id}: unexpected real CQ opcode/function")
            key = "cq_decode_count" if name == "cq_decode" else "cq_submit_commands"
            if opcode != sequence[counts[key] % 3]:
                raise Invalid(f"{case.case_id}: {name} sequence is not INVAL,FENCE,FENCE")
            counts[key] += 1
            if name == "cq_decode":
                counts["cq_iotinval" if opcode == 1 else "cq_iofence"] += 1
            elif opcode == 1:
                submitted_inval += 1
            else:
                submitted_fences += 1
        elif name == "cq_doorbell":
            posted = integer(event["aux"], f"{case.case_id}.doorbell.count")
            if posted != (2 if counts["cq_doorbells"] % 2 == 0 else 1):
                raise Invalid(f"{case.case_id}: real doorbells are not alternating 2 then1 entries")
            counts["cq_doorbells"] += 1
            posted_sum += posted
        elif name == "cq_round_complete":
            group = integer(event["value"], f"{case.case_id}.round")
            if (previous_round is None and group != 0) or (previous_round is not None and group != previous_round + 1):
                raise Invalid(f"{case.case_id}: duplicate/skipped CQ round completion")
            previous_round = group
            counts["cq_rounds"] += 1
            elapsed = integer(event["aux"], f"{case.case_id}.round.waiting")
            round_waits.append(elapsed)
            waiting += elapsed
        elif name == "cq_fence_complete":
            if integer(event["value"], f"{case.case_id}.fence.AV") != 0:
                raise Invalid(f"{case.case_id}: measurement contains a marker fence")
            counts["cq_fence_completed"] += 1
        elif name == "inval_post_edge_verified":
            mask = integer(event["value"], f"{case.case_id}.inval_post_edge.mask")
            ways = integer(event["aux"], f"{case.case_id}.inval_post_edge.ways")
            if mask >= 1 << 16 or ways != mask.bit_count():
                raise Invalid(f"{case.case_id}: post-edge verified way mask/count differs")
            counts["inval_post_edge_verified_events"] += 1
            counts["inval_post_edge_verified_ways"] += ways
    if measured == 0:
        raise Invalid(f"{case.case_id}: trace has no measurement-era events")
    expected = expected_counts(case.config)
    for key, value in expected.items():
        if counts[key] != value or counts[key] != case.metrics[key]:
            raise Invalid(f"{case.case_id}: raw {key}={counts[key]}, summary={case.metrics[key]}, frozen expectation={value}")
    if submitted_inval != counts["cq_iotinval"] or submitted_fences != counts["cq_iofence"] or posted_sum != counts["cq_submit_commands"]:
        raise Invalid(f"{case.case_id}: CQ submit/doorbell/decode conservation failed")
    if waiting != case.metrics["cq_total_waiting"]:
        raise Invalid(f"{case.case_id}: raw CQ round waiting sum differs from summary")
    if counts["inval_post_edge_verified_ways"] != case.metrics["inval_post_edge_verified_ways"]:
        raise Invalid(f"{case.case_id}: raw post-edge verified ways differs from summary")
    for lane, values in lane_counts.items():
        for key, value in values.items():
            if value != case.metrics[f"{lane}_{key}"]:
                raise Invalid(f"{case.case_id}: raw {lane}_{key}={value} differs from summary")
        expected_jobs = set(range(case.config[f"{lane.lower()}_jobs"]))
        if launches[lane] != expected_jobs or set(completions[lane]) != expected_jobs:
            raise Invalid(f"{case.case_id}: raw {lane} COPY launch/completion coverage differs")
    if len(measure_begin) != 1 or len(measure_end) != 1 or len(a_final) != 1:
        raise Invalid(f"{case.case_id}: missing/duplicate measurement or final-flush boundary")
    t0 = measure_begin[0]
    timing = case.data.get("timing", {})
    if timing.get("t0") != t0 or timing.get("end_cycle") != measure_end[0]:
        raise Invalid(f"{case.case_id}: raw measurement boundaries differ from summary")
    for lane in ("A", "B"):
        if max(completions[lane].values())-t0 != case.metrics[f"T_{lane}"]:
            raise Invalid(f"{case.case_id}: raw {lane} last COPYDONE time differs from summary")
    if a_final[0][0]-t0 != case.metrics["A_final_flush_end"] or a_final[0][1] != case.metrics["A_final_flush_end"]:
        raise Invalid(f"{case.case_id}: raw A final flush completion time differs from summary")
    if max(a_final[0][0], max(completions["B"].values()))-t0 != case.metrics["T_end"]:
        raise Invalid(f"{case.case_id}: raw complete time differs from summary")
    return {"path": str(path.resolve()), "sha256": digest(path),
            "shared_columns_stream_sha256": trace_hash.hexdigest(),
            "measure_events": measured, "counters": counts,
            "cq_waiting_sum": waiting, "posted_entry_sum": posted_sum,
            "cq_waiting_mean": waiting/len(round_waits) if round_waits else None,
            "cq_waiting_min": min(round_waits) if round_waits else None,
            "cq_waiting_max": max(round_waits) if round_waits else None,
            "lane_counters": lane_counts}


def coverage(cases: list[Case]) -> None:
    expected = {(domain, pages, mode, "full-api") for domain in DOMAINS
                for pages in FOOTPRINTS for mode in MODES}
    found = [case.key for case in cases]
    if len(set(found)) != len(found) or len({x.case_id for x in cases}) != len(cases):
        raise Invalid("Duplicate case/configuration; select explicit accepted attempts, do not silently choose the latest")
    main = {key for key in found if key[3] == "full-api"}
    if main != expected:
        raise Invalid(f"Main24 coverage mismatch; missing={sorted(expected-main)}, extra={sorted(main-expected)}")
    optional = {key for key in found if key[3] == "unmap-only"}
    expected_optional = {(domain, pages, "m1", "unmap-only") for domain in DOMAINS for pages in FOOTPRINTS}
    if optional and optional != expected_optional:
        raise Invalid(f"Optional unmap-only sensitivity must contain all4 groups, missing={sorted(expected_optional-optional)}")


def result_rows(cases: list[Case]) -> list[dict]:
    by_id = {x.case_id: x for x in cases}
    rows = []
    for case in sorted(cases, key=lambda x: (DOMAINS.index(x.key[0]), x.key[1], x.key[3], MODES.index(x.key[2]))):
        c, m = case.config, case.metrics
        expected_baseline = f"{c['domain']}-b{c['b_pages']}-m0-full-api"
        baseline_id = c.get("baseline_case_id")
        if c["map_policy"] == "unmap-only" and baseline_id != expected_baseline:
            raise Invalid(f"{case.case_id}: optional off sensitivity requires explicit baseline_case_id={expected_baseline}")
        if baseline_id not in (None, "", expected_baseline):
            raise Invalid(f"{case.case_id}: baseline changes domain/footprint/workload")
        base = by_id[expected_baseline]
        for key in FIXED_CONFIG:
            if c[key] != base.config[key]:
                raise Invalid(f"{case.case_id}: baseline {key} differs")
        rows.append({
            "case_id": case.case_id, "domain": c["domain"], "B_pages": c["b_pages"],
            "mode": c["mode"], "map_policy": c["map_policy"], "A_jobs": c["a_jobs"],
            "B_jobs": c["b_jobs"], "copy_bytes": c["bytes_per_copy"],
            "A_last_COPYDONE_steps": m["T_A"], "A_complete_with_final_flush_steps": m["A_final_flush_end"],
            "B_complete_steps": m["T_B"], "both_complete_steps": m["T_end"],
            "A_plus_B_complete_steps": m["A_final_flush_end"] + m["T_B"],
            "baseline_case_id": expected_baseline, "B_time_over_M0": m["T_B"] / base.metrics["T_B"],
            "B_walks": m["B_walks"], "M0_B_walks": base.metrics["B_walks"],
            "extra_B_walks_vs_M0": m["B_walks"] - base.metrics["B_walks"],
            "RTL_invalidation_attributed_B_misses": m["B_invalidation_attributed_misses"],
            "B_hits": m["B_hits"], "B_misses": m["B_misses"],
            "A_hits": m["A_hits"], "A_misses": m["A_misses"], "A_walks": m["A_walks"],
            "RTL_invalidation_attributed_A_misses": m["A_invalidation_attributed_misses"],
            **{f"{lane}_{key}": m[f"{lane}_{key}"] for lane in ("A", "B") for key in ("cold_misses", "replacement_misses", "ptw_memory_bytes")},
            **{key: m[key] for key in ("map_regions", "unmap_regions", "cq_rounds", "cq_iotinval", "cq_iofence", "cq_decode_count", "cq_submit_commands", "cq_fence_completed", "cq_doorbells", "cq_peak", "cq_total_waiting", "cq_entry_cycles", "inval_post_edge_verified_events", "inval_post_edge_verified_ways")},
            **{f"cq_round_waiting_{key}": case.trace[f"cq_waiting_{key}"] for key in ("mean", "min", "max")},
            "baseline_scope_note": "M0 persistent A takes identical input/code path with map flush enabled or disabled" if c["map_policy"] == "unmap-only" else "same domain, footprint and fixed workload M0",
            "summary_path": str(case.path.resolve()), "summary_sha256": digest(case.path),
        })
    return rows


def plot(rows: list[dict], output: Path) -> None:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    plt.rcParams.update({"font.size": 10, "svg.fonttype": "none"})
    fig, axes = plt.subplots(1, 2, figsize=(12, 4.4), sharey=True, constrained_layout=True)
    width = 0.34
    colors = {4: "#2878B5", 8: "#E69F00"}
    labels = ["M0\npersistent", "M1\nwide", "M2\nPSCID", "M3\naddress", "M4\nN=16", "M4\nN=64"]
    for ax, domain in zip(axes, DOMAINS):
        for pages, offset in [(4, -width/2), (8, width/2)]:
            series = [next(r for r in rows if (r["domain"], r["B_pages"], r["mode"], r["map_policy"]) == (domain, pages, mode, "full-api")) for mode in MODES]
            values = [r["B_time_over_M0"] for r in series]
            positions = [i + offset for i in range(len(MODES))]
            bars = ax.bar(positions, values, width, color=colors[pages], label=f"B: {pages} pages")
            for bar, value in zip(bars, values):
                ax.annotate(f"{value:.3g}", (bar.get_x()+bar.get_width()/2, value), xytext=(0, 3), textcoords="offset points", ha="center", va="bottom", fontsize=8)
            sensitivity = [r for r in rows if r["domain"] == domain and r["B_pages"] == pages and r["map_policy"] == "unmap-only"]
            if sensitivity:
                ax.scatter([1+offset], [sensitivity[0]["B_time_over_M0"]], marker="D", facecolors="white", edgecolors=colors[pages], linewidths=1.5, zorder=5, label=f"M1 unmap-only: {pages} pages")
        ax.axhline(1, color="#666666", linestyle="--", linewidth=0.8)
        ax.set_xticks(range(len(MODES)), labels)
        ax.set_title("Same domain" if domain == "same" else "Separate domains")
        ax.set_xlabel("Invalidation policy")
        ax.set_ylim(bottom=0)
        ax.grid(axis="y", alpha=0.2)
        ax.set_axisbelow(True)
    axes[0].set_ylabel("B complete time / corresponding M0")
    handles, names = axes[0].get_legend_handles_labels()
    fig.legend(handles, names, loc="upper center", bbox_to_anchor=(0.5, 1.10), ncol=2)
    fig.savefig(output/"B_completion_normalized.svg", bbox_inches="tight")
    fig.savefig(output/"B_completion_normalized.png", dpi=200, bbox_inches="tight")
    plt.close(fig)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--runs-dir", type=Path, help="Explicit directory of real case summary/trace files")
    source.add_argument("--summary", type=Path, nargs="+", help="Explicit real summary.json files; duplicates fail")
    parser.add_argument("--output", type=Path, default=Path(__file__).resolve().parents[1]/"analysis")
    parser.add_argument("--validation-only", action="store_true", help="Validate real inputs without writing CSV/figures")
    args = parser.parse_args()
    paths = sorted(args.runs_dir.rglob("summary.json")) if args.runs_dir else args.summary
    args.output.mkdir(parents=True, exist_ok=True)
    receipt = {"schema_version": 1, "analysis_script_sha256": digest(Path(__file__)),
               "checked_utc": datetime.now(timezone.utc).isoformat(),
               "scope": "Existing real summaries and raw measurement-era traces; no simulation/build execution",
               "inputs": [], "traces": {}, "errors": [],
               "outputs_current": False,
               "extra_walk_definition": "B_walks - corresponding M0 B_walks; not automatically invalidation-caused",
               "attribution_definition": "Separate RTL resident-key invalidation followed by first lookup miss counter",
               "cq_entry_cycles_definition": "Sum of actual (CQT-CQH) modulo ring capacity each measured cycle; queue residence, not all FSM execution time",
               "cq_round_waiting_definition": "From PTE update/start_flush through the second CQH target response, including two doorbells and both waits",
               "trace_selection": "For each format prefer published .gz when present, otherwise plain; receipt hashes refer to the exact selected files",
               "time_unit": "RTL simulation clock steps in the recorded behavioral DDR environment"}
    cases = []
    if not paths:
        receipt["errors"].append("No real summary.json inputs; no results were fabricated")
    for path in paths:
        try:
            receipt["inputs"].append({"path": str(path.resolve()), "sha256": digest(path)})
            case = load_case(path)
            traces = []
            for name in ("events.csv", "events.jsonl"):
                compressed = path.parent/f"{name}.gz"
                traces.append(compressed if compressed.is_file() else path.parent/name)
            if not all(p.is_file() for p in traces):
                raise Invalid(f"{case.case_id}: both real CSV and JSONL traces are required")
            trace_results = [verify_trace(case, p) for p in traces]
            if trace_results[0]["shared_columns_stream_sha256"] != trace_results[1]["shared_columns_stream_sha256"]:
                raise Invalid(f"{case.case_id}: CSV and JSONL shared-column event streams differ")
            case.trace = trace_results[0]
            receipt["traces"][case.case_id] = trace_results
            cases.append(case)
        except (Invalid, OSError, EOFError, KeyError, ValueError, TypeError) as exc:
            receipt["errors"].append(f"{path}: {exc}")
    rows = []
    try:
        coverage(cases)
        rows = result_rows(cases)
    except (Invalid, KeyError) as exc:
        receipt["errors"].append(str(exc))
    receipt["accepted_cases"] = len(cases)
    receipt["main_cases"] = sum(c.config["map_policy"] == "full-api" for c in cases)
    receipt["optional_unmap_only_cases"] = sum(c.config["map_policy"] == "unmap-only" for c in cases)
    receipt["status"] = "failed" if receipt["errors"] else "passed"
    if not receipt["errors"] and not args.validation_only:
        try:
            with tempfile.TemporaryDirectory(prefix=".render-", dir=args.output) as temporary:
                staging = Path(temporary)
                plot(rows, staging)
                with (staging/"results.csv").open("w", newline="") as f:
                    writer = csv.DictWriter(f, fieldnames=list(rows[0]))
                    writer.writeheader()
                    writer.writerows(rows)
                for name in ("results.csv", "B_completion_normalized.svg", "B_completion_normalized.png"):
                    (staging/name).replace(args.output/name)
            receipt["outputs"] = [{"path": str((args.output/name).resolve()), "sha256": digest(args.output/name)} for name in ("results.csv", "B_completion_normalized.svg", "B_completion_normalized.png")]
            receipt["outputs_current"] = True
        except Exception as exc:
            receipt["status"] = "failed"
            receipt["errors"].append(f"Output generation failed: {exc}")
    (args.output/"validation.json").write_text(json.dumps(receipt, ensure_ascii=False, indent=2)+"\n")
    print(json.dumps({"status": receipt["status"], "cases": len(cases), "errors": receipt["errors"]}, ensure_ascii=False))
    return 0 if receipt["status"] == "passed" else 1


if __name__ == "__main__":
    raise SystemExit(main())
