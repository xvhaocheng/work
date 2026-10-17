#!/usr/bin/env python3
"""Report all fixed scenes; compare B time, without choosing a winning scene."""
from __future__ import annotations

import argparse
import csv
import json
import math
from pathlib import Path
import sys

sys.dont_write_bytecode = True
from execute import cases
from identity import sha256, utc

HERE = Path(__file__).resolve().parent
TASK = HERE.parent
MODES = ("m0", "ml-strict", "ml-fq", "oracle")
DEAD_FIELDS = ("dead_slot_cycles", "dead_sample_cycles", "dead_peak", "dead_valid_cycles",
               "dead_replacements", "live_replacements", "empty_fills")
WALL_FIELDS = ("T_A", "A_final_flush_end", "A_wall_host", "A_wall_flush", "A_wall_copy_pte",
               "A_wall_copy_payload_wait", "A_wall_copy_other", "A_flush_cq_fetch", "A_flush_cq_decode",
               "A_flush_cq_fence", "A_flush_other", "command_latency_count", "command_latency_sum",
               "command_latency_min", "command_latency_max", "prpw_fences", "prpw_wait_cycles",
               "comp_read_requests", "comp_write_requests", "payload_read_completed", "payload_write_completed")


def write_json(path: Path, value: dict) -> None:
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False, allow_nan=False) + "\n")


def write_csv(path: Path, rows: list[dict]) -> None:
    fields = list(dict.fromkeys(key for row in rows for key in row))
    with path.open("x", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)


def ratio(a: float, b: float) -> float | None:
    return a / b if b else None


def command_sequence_check(path: Path, config: dict, metrics: dict) -> dict:
    """Bind measured CQ rounds to unmaps or the FQ timer, never to map installs."""
    import gzip
    import hashlib
    source = path if path.exists() else path.with_suffix(path.suffix + ".gz")
    opener = gzip.open if source.suffix == ".gz" else open
    counts = {}
    unmaps = {}
    bound_unmaps = set()
    timer_index = None
    last_unmap_index = -1
    round_indices = []
    violations = []
    with opener(source, "rt", newline="") as stream:
        for index, event in enumerate(csv.DictReader(stream)):
            if event["era"] != "measure":
                continue
            name = event["event"]
            counts[name] = counts.get(name, 0) + 1
            if name == "unmap_region":
                key = (event["job"], event["address"])
                if key in unmaps:
                    violations.append("duplicate unmap identity")
                unmaps[key] = index
                last_unmap_index = index
            elif name == "fq_timer_expire":
                timer_index = index
            elif name == "cq_round_begin":
                round_indices.append(index)
                if config["mode"] == "ml-strict":
                    key = (event["job"], event["address"])
                    if key not in unmaps or unmaps[key] >= index or key in bound_unmaps:
                        violations.append("strict CQ round is not bound to a unique prior matching unmap")
                    bound_unmaps.add(key)
                elif config["mode"] in ("ml-fq", "oracle"):
                    if timer_index is None or timer_index >= index:
                        violations.append("FQ CQ round is not after the recorded timer expiry")
    mode = config["mode"]
    expected_rounds = 0 if mode == "m0" else 128 if mode == "ml-strict" else 1
    expected_regions = 0 if mode == "m0" else 128
    expected_counts = dict(map_region=expected_regions, unmap_region=expected_regions,
                           cq_round_begin=expected_rounds, cq_round_complete=expected_rounds,
                           cq_submit=2 * expected_rounds, cq_decode=2 * expected_rounds,
                           cq_doorbell=2 * expected_rounds, cq_fence_complete=expected_rounds)
    for name, expected in expected_counts.items():
        if counts.get(name, 0) != expected:
            violations.append(f"event count {name}: {counts.get(name, 0)} != {expected}")
    expected_metrics = dict(cq_iotinval=expected_rounds, cq_iofence=expected_rounds,
                            cq_submit_commands=2 * expected_rounds, cq_doorbells=2 * expected_rounds,
                            cq_rounds=expected_rounds, cq_fence_completed=expected_rounds,
                            prpw_fences=expected_rounds)
    for name, expected in expected_metrics.items():
        if metrics.get(name) != expected:
            violations.append(f"summary count {name}: {metrics.get(name)} != {expected}")
    if mode == "ml-strict" and bound_unmaps != set(unmaps):
        violations.append("not every strict unmap has exactly one CQ round")
    if mode in ("ml-fq", "oracle"):
        if not round_indices or min(round_indices) <= last_unmap_index:
            violations.append("FQ CQ round occurred before the last unmap")
        for name, expected in {"fq_enqueue": 128, "fq_release": 128, "fq_timer_expire": 1}.items():
            if counts.get(name, 0) != expected:
                violations.append(f"FQ event count {name}: {counts.get(name, 0)} != {expected}")
        for name, expected in {"fq_reclaimed": 128, "fq_timer_flushes": 1, "fq_full_flushes": 0}.items():
            if metrics.get(name) != expected:
                violations.append(f"FQ summary count {name}: {metrics.get(name)} != {expected}")
    for requested, completed in (("comp_read_requests", "payload_read_completed"),
                                 ("comp_write_requests", "payload_write_completed")):
        if not isinstance(metrics.get(requested), (int, float)) or metrics.get(requested) != metrics.get(completed):
            violations.append("payload cohort requests/completions differ: " + requested)
    if source.suffix == ".gz":
        raw_hash = hashlib.sha256()
        with gzip.open(source, "rb") as stream:
            for block in iter(lambda: stream.read(1024 * 1024), b""):
                raw_hash.update(block)
        original_hash = raw_hash.hexdigest()
    else:
        original_hash = sha256(source)
    return dict(passed=not violations, case_id=config["case_id"], trace=str(source), trace_sha256=sha256(source),
                trace_raw_sha256=original_hash,
                expected_rounds=expected_rounds, observed_rounds=len(round_indices),
                strict_rounds_bound_to_unmaps=len(bound_unmaps), no_map_phase_flush=not violations,
                relevant_event_counts={name: counts.get(name, 0) for name in expected_counts},
                violations=violations)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--campaign", type=Path, required=True, action="append",
                        help="Campaign directory or campaign.json; multiple distinct campaigns are accepted")
    parser.add_argument("--precheck", type=Path, action="append", default=[],
                        help="Record excluded precheck campaign(s) without using their cases in results")
    parser.add_argument("--output", type=Path, required=True, help="New directory under task/analysis")
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to(TASK / "analysis") or output == TASK / "analysis" or output.exists():
        parser.error("--output must be a new directory inside task/analysis")
    selected = []
    rows = []
    seen = set()
    command_sequences = []
    prechecks = []
    precheck_paths = set()
    for value in args.precheck:
        path = (value / "campaign.json" if value.is_dir() else value).resolve(strict=True)
        if not path.is_relative_to(TASK / "runs") or path in precheck_paths:
            parser.error("precheck must be a distinct campaign under this task/runs")
        precheck_paths.add(path)
        data = json.loads(path.read_text())
        prechecks.append(dict(path=str(path), sha256=sha256(path), excluded_from_all_results=True,
                              complete=data.get("complete"), failure=data.get("failure"),
                              binary_sha256=data.get("binary_sha256"),
                              build_input_set_sha256=data.get("build_input_set_sha256"),
                              cases=[dict(case_id=item["config"]["case_id"], passed=item.get("passed"),
                                          returncode=item.get("returncode")) for item in data.get("cases", [])]))
    allowed = {config["case_id"]: config for config in cases("all")}
    for value in args.campaign:
        manifest_path = (value / "campaign.json" if value.is_dir() else value).resolve(strict=True)
        if not manifest_path.is_relative_to(TASK / "runs"):
            parser.error("campaign must belong to this task/runs")
        if manifest_path in precheck_paths:
            parser.error("precheck campaign cannot also be a result campaign")
        campaign = json.loads(manifest_path.read_text())
        if not campaign.get("complete"):
            parser.error("incomplete campaign: " + str(manifest_path))
        if len(campaign["cases"]) != len(campaign["expected_cases"]):
            parser.error("campaign does not contain all its planned cases")
        build_path = Path(campaign["build_manifest"])
        if sha256(build_path) != campaign["build_manifest_sha256"]:
            parser.error("build manifest identity changed since run")
        for item in campaign["cases"]:
            config = item["config"]
            case_id = config["case_id"]
            if case_id in seen or case_id not in allowed or config != allowed[case_id]:
                parser.error("duplicate or nonfixed case: " + case_id)
            seen.add(case_id)
            summary_path = Path(item["output"]) / "summary.json"
            if not item.get("passed") or item.get("returncode") != 0 or sha256(summary_path) != item.get("summary_sha256"):
                parser.error("failed run or summary identity mismatch: " + case_id)
            summary = json.loads(summary_path.read_text())
            if not summary.get("checks", {}).get("passed") or summary.get("failure"):
                parser.error("summary checks failed: " + case_id)
            for key in ("case_id", "domain", "b_pages", "mode", "a_jobs", "b_jobs"):
                if summary.get("config", {}).get(key) != config[key]:
                    parser.error("summary config mismatch: " + case_id + ":" + key)
            metrics = summary["metrics"]
            for key in ("T_A", "A_final_flush_end", *DEAD_FIELDS):
                if not isinstance(metrics.get(key), (int, float)) or not math.isfinite(metrics[key]) or metrics[key] < 0:
                    parser.error("missing/invalid metric " + key + ": " + case_id)
            if not config["a_alone"] and (not isinstance(metrics.get("T_B"), (int, float)) or not math.isfinite(metrics["T_B"]) or metrics["T_B"] <= 0):
                parser.error("missing/invalid B completion: " + case_id)
            if (metrics["dead_peak"] > 16 or metrics["dead_sample_cycles"] <= 0
                    or metrics["dead_slot_cycles"] > 16 * metrics["dead_sample_cycles"]
                    or metrics["dead_slot_cycles"] > metrics["dead_valid_cycles"]):
                parser.error("inconsistent dead-entry occupancy: " + case_id)
            row = dict(config)
            if config["group"] == "matrix":
                sequence = command_sequence_check(summary_path.with_name("events.csv"), config, metrics)
                if sequence["trace_raw_sha256"] != item.get("artifacts", {}).get("events.csv", {}).get("sha256"):
                    parser.error("raw event trace identity mismatch: " + case_id)
                command_sequences.append(sequence)
                if not sequence["passed"]:
                    parser.error("map/CQ sequence verification failed: " + case_id + ": " + "; ".join(sequence["violations"]))
            row.update({key: value for key, value in metrics.items()
                        if value is None or isinstance(value, (str, int, float, bool))})
            row["dead_replacements_total"] = metrics.get("dead_replacements_total")
            row.update(dead_average_slots=ratio(metrics["dead_slot_cycles"], metrics["dead_sample_cycles"]),
                       dead_average_slot_fraction=ratio(metrics["dead_slot_cycles"], 16 * metrics["dead_sample_cycles"]),
                       dead_peak_slot_fraction=metrics["dead_peak"] / 16,
                       dead_valid_entry_fraction=ratio(metrics["dead_slot_cycles"], metrics["dead_valid_cycles"]),
                       summary_path=str(summary_path), summary_sha256=sha256(summary_path),
                       binary_sha256=campaign["binary_sha256"], build_input_set_sha256=campaign["build_input_set_sha256"],
                       checks_passed=True)
            rows.append(row)
            selected.append(dict(case_id=case_id, path=str(summary_path), sha256=sha256(summary_path),
                                 campaign=str(manifest_path), campaign_sha256=sha256(manifest_path)))
    required = {config["case_id"] for config in cases("matrix")}
    if not required.issubset(seen):
        parser.error("all 16 primary configurations are required; missing: " + ", ".join(sorted(required - seen)))
    build_inputs = {row["build_input_set_sha256"] for row in rows}
    binaries = {row["binary_sha256"] for row in rows}
    if len(build_inputs) != 1 or len(binaries) != 1:
        parser.error("all cases must use exactly one frozen input set and binary")
    by_case = {row["case_id"]: row for row in rows}
    comparisons = []
    for domain in ("same", "different"):
        for pages in (4, 8):
            data = {mode: by_case[f"{domain}-b{pages}-{mode}"] for mode in MODES}
            baseline = data["m0"]["T_B"]
            for mode, row in data.items():
                row["T_B_over_m0"] = row["T_B"] / baseline
            strict = data["ml-strict"]["T_B"]
            fq = data["ml-fq"]["T_B"]
            oracle = data["oracle"]["T_B"]
            # Integer-cycle arithmetic keeps the inclusive 3% thresholds exact.
            slower = fq * 100 >= strict * 103
            better = oracle * 100 <= fq * 97
            comparisons.append(dict(domain=domain, b_pages=pages,
                                    T_B_m0=baseline, T_B_strict=strict, T_B_fq=fq, T_B_oracle=oracle,
                                    fq_slowdown_vs_strict=fq / strict - 1,
                                    oracle_improvement_vs_fq=1 - oracle / fq,
                                    fq_at_least_3pct_slower=slower,
                                    oracle_at_least_3pct_better=better, passed=slower and better))
    output.mkdir(parents=True)
    rows.sort(key=lambda row: list(allowed).index(row["case_id"]))
    write_csv(output / "results.csv", rows)
    write_csv(output / "comparisons.csv", comparisons)
    diagnostics = []
    for domain in ("same", "different"):
        for mode in ("m1", "m2", "m3"):
            alone = by_case.get(f"a-alone-{domain}-b8-{mode}")
            concurrent = by_case.get(f"concurrent-{domain}-b8-{mode}")
            if alone is None and concurrent is None:
                continue
            if alone is None or concurrent is None:
                parser.error("diagnostic pair is incomplete: " + domain + ":" + mode)
            item = dict(domain=domain, mode=mode, b_pages=8,
                        A_DMA_slowdown=concurrent["T_A"] / alone["T_A"] - 1,
                        A_API_slowdown=concurrent["A_final_flush_end"] / alone["A_final_flush_end"] - 1,
                        concurrent_T_B=concurrent["T_B"])
            for label, row in (("a_alone", alone), ("concurrent", concurrent)):
                for field in WALL_FIELDS:
                    item[label + "_" + field] = row.get(field)
                count = row.get("command_latency_count")
                total = row.get("command_latency_sum")
                item[label + "_command_latency_mean"] = ratio(total, count) if total is not None and count is not None else None
                # Preserve the runner's wall partition, and expose its sum for
                # review instead of silently assigning any residual to a stage.
                parts = [row.get(field) for field in ("A_wall_host", "A_wall_flush", "A_wall_copy_pte",
                                                      "A_wall_copy_payload_wait", "A_wall_copy_other")]
                item[label + "_A_wall_partition_sum"] = sum(parts) if all(value is not None for value in parts) else None
            diagnostics.append(item)
    if diagnostics:
        write_csv(output / "diagnostics.csv", diagnostics)
    gate = dict(schema="dead-translation-gate-v1", generated_utc=utc(),
                observed_primary_cases=16, observed_diagnostic_cases=len(rows) - 16,
                complete_diagnostic_pairs=len(diagnostics),
                map_flush_sequence_checks=command_sequences,
                repetitions_per_configuration=1, statistical_uncertainty_estimated=False,
                thresholds=dict(fq_slowdown_vs_strict=0.03, oracle_improvement_vs_fq=0.03,
                                inclusive=True, required="both thresholds in the same scene"),
                scenes=comparisons, any_scene_passed=any(row["passed"] for row in comparisons),
                passed_scene_count=sum(row["passed"] for row in comparisons),
                all_scenes_passed=all(row["passed"] for row in comparisons),
                interpretation="Scene-specific deterministic observations, with all four scenes retained; no winner selection or paper judgement.",
                optional_missing_dead_replacements_total=[row["case_id"] for row in rows if row["dead_replacements_total"] is None],
                metrics_note="T_A: final DMA; A_final_flush_end: final API unmap completion; fq_drain_end: separate delayed timer drain. Dead occupancy sampling excludes the later timer drain.",
                selected_inputs=selected,
                prechecks_excluded=prechecks,
                analysis_driver=dict(path=str(Path(__file__).resolve()), sha256=sha256(Path(__file__).resolve())))
    write_json(output / "gate.json", gate)
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    figure, axes = plt.subplots(1, 2, figsize=(9.6, 4.6), constrained_layout=True, sharey=True)
    width = 0.34
    plotted_modes = ("ml-strict", "ml-fq", "oracle")
    for domain, axis, title in zip(("same", "different"), axes, ("Same domain", "Different domains")):
        for index, (pages, color) in enumerate(zip((4, 8), ("#3478b0", "#d77a2c"))):
            values = [by_case[f"{domain}-b{pages}-{mode}"]["T_B_over_m0"] for mode in plotted_modes]
            positions = [group + (index - 0.5) * width for group in range(3)]
            bars = axis.bar(positions, values, width, color=color, label=f"B: {pages} pages")
            axis.bar_label(bars, labels=[f"{value:.3f}" for value in values], fontsize=8, padding=3)
        axis.axhline(1, color="#343a40", linestyle="--", linewidth=0.8, label="M0 baseline")
        axis.set_xticks(range(3), ["ML-strict", "ML-FQ", "ORACLE"])
        axis.set_title(title)
        axis.set_ylim(0, max(row.get("T_B_over_m0", 0) for row in rows) * 1.24)
        axis.grid(axis="y", alpha=0.2)
        axis.set_axisbelow(True)
        axis.legend(fontsize=8, loc="upper left")
    axes[0].set_ylabel("B completion time / M0 completion time")
    figure.suptitle("A: 64 copies; B: 256 copies; 4096 bytes per copy")
    figure.savefig(output / "B_completion_normalized.png", dpi=180)
    figure.savefig(output / "B_completion_normalized.pdf")
    plt.close(figure)
    print(json.dumps(dict(cases=len(rows), scenes=4, passed_scenes=gate["passed_scene_count"],
                          any_scene_passed=gate["any_scene_passed"], output=str(output))))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
