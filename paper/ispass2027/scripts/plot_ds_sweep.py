#!/usr/bin/env python3
"""Rebuild E1b DS sensitivity summaries and publication figures from board data.

Supply the historical baseline100 import summary and the DS25/DS100 import
summary with repeated --import-summary. No board access or CSV edits occur.
The default requires the full 4 DS x 5 conditions x 10 blocks. --allow-partial
explicitly reports an incomplete campaign and never draws a four-point curve.
Only the Python standard library and matplotlib are required.
"""
from __future__ import annotations

import argparse
import collections
import csv
import hashlib
import io
import json
import math
import os
from pathlib import Path
import statistics
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
PAPER = HERE.parent
ROOT = PAPER.parents[1]
sys.path.insert(0, str(PAPER / "data"))
import import_board_runs as importer

DS_VALUES = (0, 25, 50, 100)
CONDITIONS = ("Solo", "A1", "A8", "A16", "A64")
AGGRESSORS = ("A8", "A16", "A64")
BLOCKS = tuple(range(10))
METRICS = ("hardware_cycles", "hardware_ms", "hardware_MiB_s", "victim_completed_bytes")
QUANTILE_METHOD = "Type 7: sorted x, h=(n-1)*p; linear interpolation between floor(h) and ceil(h)"


def relative(path):
    return Path(path).resolve().relative_to(ROOT).as_posix()


def output_reference(path):
    try:
        return relative(path)
    except ValueError:
        return str(Path(path).resolve())


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def encoded(value):
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True, allow_nan=False) + "\n"


def close(a, b):
    return importer.number(a) and importer.number(b) and math.isclose(a, b, rel_tol=1e-12, abs_tol=1e-10)


def quantile(values, p):
    ordered = sorted(values)
    if not ordered:
        raise ValueError("Cannot compute a quantile from zero observations")
    h = (len(ordered) - 1) * p
    lo, hi = math.floor(h), math.ceil(h)
    return ordered[lo] + (ordered[hi] - ordered[lo]) * (h - lo)


def describe(values):
    if not values:
        return {"n": 0, "median": None, "q1": None, "q3": None, "iqr": None}
    q1, q3 = quantile(values, .25), quantile(values, .75)
    return {"n": len(values), "median": statistics.median(values), "q1": q1, "q3": q3, "iqr": q3 - q1}


def source_document(path, sources, expected=None):
    path = Path(path).resolve()
    digest = sha(path)
    if expected is not None and digest != expected:
        raise ValueError("Source hash mismatch: " + relative(path))
    sources[relative(path)] = digest
    return json.loads(path.read_text())


def table_rows(path):
    with Path(path).open(newline="") as stream:
        rows = list(csv.DictReader(stream))
    indexed = {}
    for row in rows:
        if row["row_id"] in indexed:
            raise ValueError("Duplicate long-table row_id: " + row["row_id"])
        indexed[row["row_id"]] = row
    return indexed


def load_trials(summary_paths, long_table):
    """Revalidate original run inputs, then join exact sample IDs to the CSV."""
    sources, trials, failures, runs, seen_runs, documents = {}, [], [], [], set(), {}
    csv_rows = table_rows(long_table)
    sources[relative(long_table)] = sha(long_table)
    for summary_path in sorted(map(lambda p: Path(p).resolve(), summary_paths)):
        summary = source_document(summary_path, sources)
        documents[relative(summary_path)] = summary
        if summary.get("schema_version") != 1 or summary.get("fixture_only"):
            raise ValueError("Expected a schema-1, non-fixture board import: " + relative(summary_path))
        manifest_path = summary_path.parent / "manifest.json"
        manifest = source_document(manifest_path, sources)
        if manifest["output_sha256"][summary_path.name] != sources[relative(summary_path)]:
            raise ValueError("Import manifest does not bind summary bytes: " + relative(summary_path))
        for source in manifest["input_files"]:
            path = ROOT / source["raw_path"]
            digest = sources.get(source["raw_path"]) or sha(path)
            if digest != source["sha256"]:
                raise ValueError("Import input hash mismatch: " + source["raw_path"])
            sources[source["raw_path"]] = digest
        for ri, saved in enumerate(summary["runs"]):
            if saved["phase"] == "g1":
                continue
            if saved["run_id"] in seen_runs:
                raise ValueError("Run supplied more than once: " + saved["run_id"])
            seen_runs.add(saved["run_id"])
            fresh = importer.load_run(importer.Inputs(), ROOT / saved["run_path"])
            if fresh != saved:
                raise ValueError("Saved import differs from current original run validation: " + saved["run_path"])
            left, right = map(int, fresh["ds"].split("/"))
            if left != right or left not in DS_VALUES:
                raise ValueError("Outside the fixed DS0/25/50/100 contract: " + fresh["ds"])
            identity = fresh["identity"]
            blocks = importer.PHASES[fresh["phase"]][3]
            rotated_plan = [[b, c] for b in blocks for c in CONDITIONS[b % len(CONDITIONS):] + CONDITIONS[:b % len(CONDITIONS)]]
            if fresh["planned_cases"] != rotated_plan:
                raise ValueError("Source phase plan is not the fixed rotating condition order: " + fresh["run_path"])
            actual_order = [[a["block"], a["condition"]] for a in fresh["attempts"] if a["retained_by_collector"]]
            if actual_order != rotated_plan[:len(actual_order)]:
                raise ValueError("Retained raw trial order differs from the rotating phase plan: " + fresh["run_path"])
            runs.append({"run_id": fresh["run_id"], "run_path": fresh["run_path"], "phase": fresh["phase"],
                         "ds_cycles_each_direction": left, "manifest_status": fresh["manifest_status"],
                         "phase_complete": fresh["phase_complete"], "issues": fresh["issues"],
                         "expected_points": fresh["expected_points"], "accepted_points": fresh["accepted_points"],
                         "failed_or_unretained_points": fresh["failed_or_unretained_points"],
                         "unattempted_plan_points": fresh["unattempted_plan_points"], "identity": identity,
                         "condition_order_verified": True, "import_summary": relative(summary_path)})
            raw = json.loads((ROOT / fresh["run_path"] / "records.json").read_text())
            documents[relative(ROOT / fresh["run_path"] / "records.json")] = raw
            for ai, attempt in enumerate(fresh["attempts"]):
                base = {"ds_cycles_each_direction": left, "block": attempt["block"],
                        "condition": attempt["condition"], "run_id": fresh["run_id"],
                        "attempt_id": attempt["attempt_id"], "boot_id": attempt["boot_id"],
                        "import_summary": relative(summary_path),
                        "import_locator": importer.tables.pointer("runs", ri, "attempts", ai),
                        "raw_path": attempt["raw_record_path"], "raw_locator": attempt["raw_record_locator"],
                        "command_path": attempt["command_path"], "accepted": attempt["accepted"]}
                if not attempt["accepted"]:
                    failures.append({**base, "errors": attempt["errors"],
                                     "retained_by_collector": attempt["retained_by_collector"]})
                    continue
                if attempt["command_binding"] != "original_CLI_JSON_equal_and_fresh_sequence_and_boot":
                    raise ValueError("Accepted trial lacks an original command binding: " + fresh["run_id"] + "/" + attempt["attempt_id"])
                record = raw[attempt["record_index"]]
                cycles = record["observer"]["time"]["cycles"]
                size = record["record"]["result"]["victim"]["completed_bytes"]
                if not importer.number(cycles) or cycles <= 0 or size != 4096 * 4096:
                    raise ValueError("Invalid hardware cycles or completed bytes in raw trial")
                values = {"hardware_cycles": cycles, "hardware_ms": cycles / 50000,
                          "hardware_MiB_s": size / 1048576 / (cycles / 50e6), "victim_completed_bytes": size}
                row_ids = {}
                for metric, value in values.items():
                    rid = f"board_{fresh['series']}.{importer.tables.slug(summary['name'])}.{fresh['run_id']}.{attempt['attempt_id']}.{metric}.sample"
                    row = csv_rows.get(rid)
                    if row is None:
                        raise ValueError("Raw trial has no long-table sample row: " + rid)
                    fields = dict(part.split("=", 1) for part in row["condition"].split(";") if "=" in part)
                    expected = {"ds": fresh["ds"], "mode": attempt["condition"], "block": str(attempt["block"]),
                                "run": fresh["run_id"], "attempt_status": "accepted", "phase": fresh["phase"],
                                "campaign_status": fresh["manifest_status"], "phase_complete": str(int(fresh["phase_complete"]))}
                    if (any(fields.get(k) != v for k, v in expected.items()) or row["statistic"] != "sample" or
                            row["metric"] != metric or row["n"] != "1" or row["boot_id"] != attempt["boot_id"] or
                            row["bitstream_id"] != "program_file_sha256:" + identity["bit_sha256"] or
                            row["kernel_id"] != identity["kernel_id"] or row["evidence_level"] != "fpga_board" or
                            row["experiment"] != "board_" + fresh["series"] or row["sample_set"] != fresh["phase"] + ":accepted" or
                            row["unit"] != attempt["metrics"][metric]["unit"] or
                            row["value_status"] != attempt["metrics"][metric]["value_status"] or
                            row["identity_status"] != identity["identity_status"]):
                        raise ValueError("Long-table metadata differs from raw run: " + rid)
                    if not close(float(row["value"]), value) or not close(attempt["metrics"][metric]["value"], value):
                        raise ValueError("Long-table/import metric differs from cycles/50MHz: " + rid)
                    source_path = ROOT / row["raw_path"]
                    if relative(source_path) not in sources:
                        raise ValueError("Long-table locator is outside verified import sources: " + rid)
                    if relative(source_path) not in documents:
                        documents[relative(source_path)] = json.loads(source_path.read_text())
                    source = documents[relative(source_path)]
                    if not close(importer.tables.locate(source, row["source_locator"]), value):
                        raise ValueError("Long-table source locator differs from value: " + rid)
                    row_ids[metric] = rid
                trials.append({**base, **values, "long_table_row_ids": row_ids,
                               "bit_sha256": identity["bit_sha256"], "kernel_release": identity["kernel_id"]})
    return sorted(trials, key=lambda t: (t["ds_cycles_each_direction"], t["block"], CONDITIONS.index(t["condition"]))), failures, runs, sources


def summarize(trials, failures, runs):
    wanted = {(ds, block, condition) for ds in DS_VALUES for block in BLOCKS for condition in CONDITIONS}
    indexed = {}
    for trial in trials:
        key = (trial["ds_cycles_each_direction"], trial["block"], trial["condition"])
        if key not in wanted:
            raise ValueError("Trial outside fixed DS/block/condition contract: " + str(key))
        if key in indexed:
            raise ValueError("Duplicate DS/block/condition; no sample selection is permitted: " + str(key))
        indexed[key] = trial
    missing = [{"ds_cycles_each_direction": ds, "block": b, "condition": c} for ds, b, c in sorted(wanted - indexed.keys())]
    issues = []
    if missing:
        issues.append(f"{len(missing)} of 200 required DS/block/condition points are missing")
    if failures:
        issues.append(f"{len(failures)} failed/unretained attempts are retained in failed_attempts")
    if any(not run["phase_complete"] for run in runs):
        issues.append("One or more source phases are incomplete or failed")
    kernels = {r["identity"]["kernel_id"] for r in runs}
    if len(kernels) != 1 or "unknown" in kernels:
        issues.append("Source phases do not have one known kernel release")
    for ds in DS_VALUES:
        subset = [r for r in runs if r["ds_cycles_each_direction"] == ds]
        bits = {r["identity"]["bit_sha256"] for r in subset}
        if subset and (len(bits) != 1 or "unknown" in bits):
            issues.append(f"DS{ds} source phases have different or unknown BIT identities")
        if any(not r["identity"]["binding_verified"] or not r["identity"]["program_file_sha256_verified"] for r in subset):
            issues.append(f"DS{ds} source phase load identity is not bound")
    groups, paired, extra = [], [], []
    for ds in DS_VALUES:
        for condition in CONDITIONS:
            members = [indexed[(ds, b, condition)] for b in BLOCKS if (ds, b, condition) in indexed]
            groups.append({"ds_cycles_each_direction": ds, "condition": condition,
                           "hardware_ms": describe([t["hardware_ms"] for t in members]),
                           "hardware_MiB_s": describe([t["hardware_MiB_s"] for t in members]),
                           "sample_row_ids": {m: [t["long_table_row_ids"][m] for t in members] for m in METRICS}})
        for condition in AGGRESSORS:
            ratios = []
            for block in BLOCKS:
                numerator, denominator = indexed.get((ds, block, condition)), indexed.get((ds, block, "A1"))
                if numerator is None or denominator is None:
                    continue
                if numerator["boot_id"] != denominator["boot_id"]:
                    raise ValueError(f"Within-block pairing crosses boots: DS{ds} block {block}")
                value = numerator["hardware_cycles"] / denominator["hardware_cycles"] - 1
                ratios.append(value)
                paired.append({"ds_cycles_each_direction": ds, "block": block, "condition": condition,
                               "boot_id": numerator["boot_id"], "value": value,
                               "formula": "T_V(condition,block)/T_V(A1,block)-1; common 50MHz cancels",
                               "numerator_row_id": numerator["long_table_row_ids"]["hardware_cycles"],
                               "denominator_row_id": denominator["long_table_row_ids"]["hardware_cycles"],
                               "numerator_raw": {"raw_path": numerator["raw_path"], "pointer": numerator["raw_locator"] + "/observer/time/cycles"},
                               "denominator_raw": {"raw_path": denominator["raw_path"], "pointer": denominator["raw_locator"] + "/observer/time/cycles"}})
            extra.append({"ds_cycles_each_direction": ds, "condition": condition, **describe(ratios)})
    return {"schema_version": 1, "complete": not issues, "status": "complete" if not issues else "partial",
            "required_trials": 200, "accepted_trials": len(trials), "failed_attempts": failures,
            "missing_points": missing, "completeness_issues": issues, "source_runs": runs,
            "quantiles": QUANTILE_METHOD, "hardware_clock_hz_assumption": 50000000,
            "estimators": {"hardware_ms": "median of per-trial cycles/50000",
                           "hardware_MiB_s": "median of per-trial completed_bytes/2**20/(cycles/50000000)",
                           "extra_slowdown_vs_A1": "median of 10 within-DS, within-block T_V(Ak)/T_V(A1)-1 values; Ak=A8,A16,A64",
                           "error_bars": "Q1 to Q3 (IQR range), descriptive; not confidence intervals"},
            "limitations": ["DS0 is a direct wire; positive DS values are added input/output delays in cycles each direction.",
                            "Added DS sensitivity is not an estimate of isolated PTW cost.",
                            "Repeated trials within a boot are not independent board restarts; DS50 spans its actual two boots.",
                            "Matching kernel releases do not alone prove payload-byte equality; retain package/load evidence.",
                            "No significance tests or population-level uncertainty claims are made."],
            "condition_statistics": groups, "extra_slowdown_statistics": extra,
            "trials": trials, "paired_extra_slowdown": paired}


def markdown(report, data_dir, figures_dir):
    def link(path):
        return os.path.relpath(path, data_dir)
    def stat(cell):
        return "missing" if cell["n"] == 0 else f"{cell['median']:.6f} [{cell['q1']:.6f}, {cell['q3']:.6f}]"
    lines = ["# E1b DS sweep", "", "## Material Passport", "",
             "- Origin Skill: academic-research-suite / experiment-agent", "- Origin Mode: validate",
             "- Verification Status: ANALYZED (source arithmetic replay; no new board rerun by this script)",
             "- Version Label: ds_sweep_v1", "", f"Campaign status: **{report['status']}**; accepted {report['accepted_trials']}/200 trials.", "",
             "DS is the added delay in cycles **in each input/output direction**; DS0 is a direct wire.",
             "All values below are median [Q1, Q3] from actual per-trial values. Quantiles use " + QUANTILE_METHOD + ".",
             "IQR error bars are descriptive ranges, not confidence intervals. No significance claim is made.", "",
             "Extra slowdown pairs Ak and A1 in the same DS and block, computes T_V(Ak)/T_V(A1)-1 per block, then takes the median and quartiles of those ratios.",
             "DS50 blocks 0–4 come from the first boot and 5–9 from the last boot; each pair stays within its boot.", "",
             "| DS (cycles each direction) | Condition | n | Hardware time (ms) | Throughput (MiB/s) |", "| --- | --- | ---: | ---: | ---: |"]
    for group in report["condition_statistics"]:
        lines.append(f"| {group['ds_cycles_each_direction']} | {group['condition']} | {group['hardware_ms']['n']} | {stat(group['hardware_ms'])} | {stat(group['hardware_MiB_s'])} |")
    lines += ["", "| DS | Condition / A1 | Pairs | Extra slowdown (ratio) |", "| --- | --- | ---: | ---: |"]
    for group in report["extra_slowdown_statistics"]:
        lines.append(f"| {group['ds_cycles_each_direction']} | {group['condition']} | {group['n']} | {stat(group)} |")
    lines += ["", "## Sources and limits", "", "Per-trial raw JSON pointers, exact long-table row IDs, pair operands, and input SHA256 values are in [ds-sweep-summary.json](ds-sweep-summary.json).", ""]
    for run in report["source_runs"]:
        lines.append(f"- DS{run['ds_cycles_each_direction']} {run['phase']}: [{run['run_id']}]({link(ROOT / run['run_path'] / 'manifest.json')}); [import summary]({link(ROOT / run['import_summary'])}); boot `{run['identity']['boot_id']}`.")
    lines += ["", *["- " + text for text in report["limitations"]], ""]
    if report["complete"]:
        lines += ["## Figures", ""]
        for name in ("ds-sweep-hardware-time", "ds-sweep-throughput", "ds-sweep-extra-slowdown"):
            lines.append(f"- {name}: [PDF]({link(figures_dir / (name + '.pdf'))}), [PNG]({link(figures_dir / (name + '.png'))}), [SVG]({link(figures_dir / (name + '.svg'))}).")
    else:
        lines += ["**Partial report: figures are withheld. No missing DS point is interpolated or fabricated.**", "", *["- " + issue for issue in report["completeness_issues"]]]
    lines += ["", "## Statistical interpretation coverage", "", "11/11 fallacy categories considered within this descriptive analysis:", "", "| Category | Scope disposition |", "| --- | --- |",
              "| Simpson's paradox | DS and conditions remain separate; DS50 pooled boot identities are explicit. Boot-level trend equivalence is not claimed. |",
              "| Ecological fallacy | Only this board's measured trial/block summaries are described. |",
              "| Berkson's paradox | Fixed 200-point design; no outcome-based sample selection. |",
              "| Collider bias | No regression or post-outcome covariate adjustment. |",
              "| Base-rate neglect | No diagnostic/classification rates. |",
              "| Regression to the mean | No extreme-outcome selection or pre/post improvement claim. |",
              "| Survivorship bias | Failures and missing points are explicit; any failure prevents a complete curve. |",
              "| Look-elsewhere effect | All fixed DS/conditions reported; no significance testing. |",
              "| Garden of forking paths | Fixed estimators and Type-7 quartiles recorded; this is descriptive analysis, not a preregistered significance test. |",
              "| Correlation versus causation | Added-delay sensitivity only; not isolated PTW attribution. |",
              "| Reverse causality | No directional population claim. |", ""]
    return "\n".join(lines)


def figures(report):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 9, "pdf.fonttype": 42,
                         "svg.fonttype": "none", "svg.hashsalt": "ispass2027-ds-sweep-v1"})
    outputs = {}
    cases = (("ds-sweep-hardware-time", "hardware_ms", "Victim hardware time (ms)", CONDITIONS),
             ("ds-sweep-throughput", "hardware_MiB_s", "Victim throughput (MiB/s)", CONDITIONS),
             ("ds-sweep-extra-slowdown", "extra", "Victim extra slowdown vs A1 (%)", AGGRESSORS))
    colors = {"Solo": "#666666", "A1": "#0072B2", "A8": "#D55E00", "A16": "#009E73", "A64": "#CC79A7"}
    markers = {"Solo": "s", "A1": "o", "A8": "^", "A16": "D", "A64": "v"}
    for filename, metric, ylabel, conditions in cases:
        fig, ax = plt.subplots(figsize=(5.4, 3.45), layout="constrained")
        for condition in conditions:
            lookup = {g["ds_cycles_each_direction"]: (g if metric == "extra" else g[metric])
                      for g in report["extra_slowdown_statistics" if metric == "extra" else "condition_statistics"] if g["condition"] == condition}
            values = [lookup[ds] for ds in DS_VALUES]
            scale = 100 if metric == "extra" else 1
            medians = [v["median"] * scale for v in values]
            errors = [[(v["median"] - v["q1"]) * scale for v in values],
                      [(v["q3"] - v["median"]) * scale for v in values]]
            ax.errorbar(DS_VALUES, medians, yerr=errors, color=colors[condition], marker=markers[condition],
                        label=condition, capsize=3, linewidth=1.35, markersize=4.2)
        ax.set_xticks(DS_VALUES, ["0\nwire", "25", "50", "100"])
        ax.set_xlabel("Added DS delay (cycles in each input/output direction)")
        ax.set_ylabel(ylabel)
        ax.grid(axis="y", alpha=.22)
        ax.legend(frameon=False, ncol=len(conditions), fontsize=8, loc="best")
        if metric == "extra":
            ax.axhline(0, color="black", linewidth=.6, alpha=.5)
        for extension in ("pdf", "png", "svg"):
            buffer = io.BytesIO()
            metadata = {"CreationDate": None, "ModDate": None} if extension == "pdf" else {"Date": None} if extension == "svg" else {}
            fig.savefig(buffer, format=extension, dpi=220, metadata=metadata)
            outputs[filename + "." + extension] = buffer.getvalue()
        plt.close(fig)
    return outputs, matplotlib.__version__


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--import-summary", action="append", required=True, type=Path)
    parser.add_argument("--long-table", type=Path, default=PAPER / "data/board_results.csv")
    parser.add_argument("--data-dir", type=Path, default=PAPER / "data")
    parser.add_argument("--figures-dir", type=Path, default=PAPER / "analysis/figures")
    parser.add_argument("--allow-partial", action="store_true", help="Write explicitly partial reports; withhold all figures")
    parser.add_argument("--check", action="store_true", help="Rebuild in memory and compare all output bytes")
    args = parser.parse_args()
    try:
        trials, failures, runs, sources = load_trials(args.import_summary, args.long_table)
        report = summarize(trials, failures, runs)
        if not report["complete"] and not args.allow_partial:
            raise ValueError("Incomplete DS sweep; no outputs written: " + "; ".join(report["completeness_issues"]) + ". Use --allow-partial only for an explicitly partial report.")
        figure_outputs, version = figures(report) if report["complete"] else ({}, None)
        report.update(generator={"path": relative(__file__), "sha256": sha(__file__), "matplotlib_version": version},
                      long_table=relative(args.long_table), source_sha256=dict(sorted(sources.items())),
                      figure_files=[output_reference(args.figures_dir / name) for name in sorted(figure_outputs)])
        data_outputs = {"ds-sweep-summary.json": encoded(report).encode(),
                        "ds-sweep-summary.md": markdown(report, args.data_dir, args.figures_dir).encode()}
        for path, content in [(args.data_dir / n, b) for n, b in data_outputs.items()] + [(args.figures_dir / n, b) for n, b in figure_outputs.items()]:
            if args.check:
                if not path.is_file() or path.read_bytes() != content:
                    raise ValueError("Deterministic output differs or is missing: " + str(path))
            else:
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(content)
        print(json.dumps({"status": report["status"], "mode": "check" if args.check else "write",
                          "accepted_trials": len(trials), "paired_ratios": len(report["paired_extra_slowdown"]),
                          "figure_files": len(figure_outputs)}, sort_keys=True))
    except (ValueError, KeyError, TypeError, OSError, ImportError) as error:
        print(json.dumps({"status": "failed", "error": str(error)}, sort_keys=True), file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
