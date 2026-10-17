#!/usr/bin/env python3
"""Focused source/numerical/failure checks; never access the board or install data.

Fixture curves are rendered only in memory and are explicitly synthetic. The
saved receipt contains actual historical DS0/DS50 numerical checks, test names,
and provenance; it never presents synthetic DS25/DS100 values as measurements.
"""
from __future__ import annotations

import argparse
import copy
import csv
from fractions import Fraction
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
import plot_ds_sweep as sweep

BASELINE = sweep.PAPER / "data/board_imports/baseline100-20261017-01/summary.json"
LONG_TABLE = sweep.PAPER / "data/board_results.csv"
checks = []


def require(name, condition):
    if not condition:
        raise AssertionError(name)
    checks.append({"check": name, "passed": True})


def rejected(name, action, text):
    try:
        action()
    except ValueError as error:
        require(name, text in str(error))
    else:
        raise AssertionError(name + ": unexpectedly accepted")


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(sweep.encoded(value))


def fixture(destination, phase):
    original = sweep.PAPER / "board/runs/baseline100-20261017-01/ds0-middle"
    destination.mkdir()
    for name in ("load.json",):
        load = json.loads((original.parent / "ds0-middle-load" / name).read_text())
        write(destination / name, load)
    manifest = json.loads((original / "manifest.json").read_text())
    records = json.loads((original / "records.json").read_text())
    for i, record in enumerate(records):
        command_path = destination / "uart" / f"command-case{i+1:04d}.json"
        command = json.loads(Path(record["raw_command_path"]).read_text())
        write(command_path, command)
        record.update(ds=sweep.importer.PHASES[phase][2], raw_command_path=str(command_path))
    manifest.update(phase=phase, load_manifest=str(destination / "load.json"), synthetic_fixture=True,
                    load_manifest_sha256=sweep.sha(destination / "load.json"))
    write(destination / "records.json", records)
    write(destination / "manifest.json", manifest)
    shutil.copyfile(original / "review.json", destination / "review.json")
    return records


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=sweep.PAPER / "analysis/ds-sweep-validation.json")
    args = parser.parse_args()
    production_before = sweep.sha(sweep.importer.OFFICIAL)
    trials, failures, runs, sources = sweep.load_trials([BASELINE], LONG_TABLE)
    require("All 100 historical raw trials join original UART evidence and exact long-table scalar IDs", len(trials) == 100 and not failures and all(r["phase_complete"] for r in runs))
    report = sweep.summarize(trials, failures, runs)
    require("Historical two-DS input is explicitly partial with exactly 100 missing DS25/DS100 points", not report["complete"] and len(report["missing_points"]) == 100 and {m["ds_cycles_each_direction"] for m in report["missing_points"]} == {25, 100})
    require("DS50 pooled first/last blocks preserve ten within-boot pairs per aggressor", all(g["n"] == 10 for g in report["extra_slowdown_statistics"] if g["ds_cycles_each_direction"] == 50) and len({p["boot_id"] for p in report["paired_extra_slowdown"] if p["ds_cycles_each_direction"] == 50}) == 2)
    numerical = []
    for ds, condition in ((0, "A8"), (50, "A64")):
        raw_values = {}
        for run in runs:
            if run["ds_cycles_each_direction"] == ds:
                records = json.loads((sweep.ROOT / run["run_path"] / "records.json").read_text())
                raw_values.update({(r["block"], r["condition"]): r["observer"]["time"]["cycles"] for r in records})
        pairs = [{"block": b, "numerator_cycles": raw_values[(b, condition)], "denominator_cycles": raw_values[(b, "A1")]} for b in range(10)]
        exact = [Fraction(p["numerator_cycles"], p["denominator_cycles"]) - 1 for p in pairs]
        ordered = sorted(exact)
        # Independent n=10 hand formulas: Type 7 h=2.25 and 6.75.
        expected = {"median": float((ordered[4] + ordered[5]) / 2),
                    "q1": float((3 * ordered[2] + ordered[3]) / 4),
                    "q3": float((ordered[6] + 3 * ordered[7]) / 4)}
        observed = next(g for g in report["extra_slowdown_statistics"] if g["ds_cycles_each_direction"] == ds and g["condition"] == condition)
        require(f"DS{ds} {condition} paired ratio median/Q1/Q3 agree with exact rational arithmetic and independent n10 quartile formulas", all(sweep.close(observed[k], value) for k, value in expected.items()))
        nums, dens = sorted(p["numerator_cycles"] for p in pairs), sorted(p["denominator_cycles"] for p in pairs)
        ratio_of_medians = (nums[4] + nums[5]) / (dens[4] + dens[5]) - 1
        require(f"DS{ds} {condition} estimator stays the median of block ratios rather than ratio of condition medians", not sweep.close(observed["median"], ratio_of_medians))
        numerical.append({"ds_cycles_each_direction": ds, "condition": condition, "pairs": pairs,
                          "exact_ratio_strings": [str(v) for v in exact], "expected": expected,
                          "observed": {k: observed[k] for k in expected}, "ratio_of_condition_medians": ratio_of_medians})
    missing_one = sweep.summarize(trials[1:], failures, runs)
    require("A missing raw trial is counted, never imputed", not missing_one["complete"] and len(missing_one["missing_points"]) == 101)
    rejected("Duplicate DS/block/condition is rejected rather than selected", lambda: sweep.summarize(trials + [trials[0]], failures, runs), "Duplicate DS/block/condition")
    failed = {"ds_cycles_each_direction": 0, "block": 0, "condition": "Solo", "errors": [{"problem": "synthetic failure retention test"}]}
    failed_report = sweep.summarize(trials, [failed], runs)
    require("A failure remains visible and prevents campaign completeness", failed_report["failed_attempts"] == [failed] and any("failed/unretained" in i for i in failed_report["completeness_issues"]))
    cross_boot = copy.deepcopy(trials)
    next(t for t in cross_boot if t["ds_cycles_each_direction"] == 0 and t["block"] == 0 and t["condition"] == "A8")["boot_id"] = "synthetic-different-boot"
    rejected("Pairing across different boots is rejected", lambda: sweep.summarize(cross_boot, failures, runs), "pairing crosses boots")
    synthetic_trials, synthetic_runs = copy.deepcopy(trials), copy.deepcopy(runs)
    for ds in (25, 100):
        for trial in trials:
            if trial["ds_cycles_each_direction"] == 0:
                item = copy.deepcopy(trial)
                item["ds_cycles_each_direction"] = ds
                synthetic_trials.append(item)
        item = copy.deepcopy(next(r for r in runs if r["ds_cycles_each_direction"] == 0))
        item.update(ds_cycles_each_direction=ds, run_id="synthetic-render-test-ds" + str(ds))
        synthetic_runs.append(item)
    synthetic = sweep.summarize(synthetic_trials, [], synthetic_runs)
    require("Complete synthetic render fixture has 200 trials and 120 paired ratios", synthetic["complete"] and len(synthetic["paired_extra_slowdown"]) == 120)
    rendered, version = sweep.figures(synthetic)
    rerendered, _ = sweep.figures(synthetic)
    require("All nine in-memory PDF/PNG/SVG outputs are nonempty and byte-deterministic", len(rendered) == 9 and all(len(v) > 1000 and v == rerendered[k] for k, v in rendered.items()))
    with tempfile.TemporaryDirectory(prefix="ds-sweep-validation-", dir=sweep.PAPER / "data") as temp:
        work = Path(temp)
        output = work / "strict-output"
        command = [sys.executable, str(Path(sweep.__file__)), "--import-summary", str(BASELINE), "--data-dir", str(output), "--figures-dir", str(work / "figures")]
        strict = subprocess.run(command, text=True, capture_output=True)
        require("Strict CLI exits cleanly on incomplete campaign without any output directory", strict.returncode == 1 and "Incomplete DS sweep" in strict.stderr and not output.exists())
        partial = subprocess.run(command + ["--allow-partial"], text=True, capture_output=True)
        partial_data = json.loads((output / "ds-sweep-summary.json").read_text()) if partial.returncode == 0 else {}
        require("Explicit partial CLI retains 100 raw trials and withholds all four-point figures", partial.returncode == 0 and partial_data.get("status") == "partial" and partial_data.get("accepted_trials") == 100 and partial_data.get("figure_files") == [] and not (work / "figures").exists())
        checked = subprocess.run(command + ["--allow-partial", "--check"], text=True, capture_output=True)
        require("Partial report is byte-deterministic under --check", checked.returncode == 0)
        csv_rows = sweep.table_rows(LONG_TABLE)
        rid = trials[0]["long_table_row_ids"]["hardware_ms"]
        selected = [dict(row) for row in csv_rows.values()]
        def write_csv(path, rows):
            with path.open("w", newline="") as stream:
                writer = csv.DictWriter(stream, fieldnames=list(rows[0]))
                writer.writeheader()
                writer.writerows(rows)
        bad_csv = work / "bad-metric.csv"
        next(row for row in selected if row["row_id"] == rid)["value"] = "999.0"
        write_csv(bad_csv, selected)
        rejected("CSV value disagreement with raw cycles/50MHz is rejected", lambda: sweep.load_trials([BASELINE], bad_csv), "differs from cycles/50MHz")
        duplicate_csv = work / "duplicate-row.csv"
        write_csv(duplicate_csv, [dict(csv_rows[rid]), dict(csv_rows[rid])])
        rejected("Duplicate long-table stable IDs are rejected", lambda: sweep.table_rows(duplicate_csv), "Duplicate long-table row_id")
        rejected("Supplying one source run twice is rejected", lambda: sweep.load_trials([BASELINE, BASELINE], LONG_TABLE), "Run supplied more than once")
        phase_dirs = []
        for phase in sweep.importer.DS_SWEEP:
            destination = work / phase
            fixture(destination, phase)
            phase_dirs.append(destination)
        _, config, imported = sweep.importer.prepare(phase_dirs, work / "import", "synthetic-ds-sweep-check")
        require("Importer accepts both new DS contracts as 100-point separate ds_sweep series", imported["fixture_only"] and imported["ds_sweep"]["complete"] and imported["ds_sweep"]["accepted_points"] == 100 and imported["formal100"]["accepted_points"] == 0 and all(r["series"] == "ds_sweep" for r in imported["runs"]))
        require("New DS rows have their own stable experiment/row namespace", all(r["experiment"] == "board_ds_sweep" and r["row_id"].startswith("board_ds_sweep.synthetic-ds-sweep-check.") for r in config["rows"]))
        broken = json.loads((phase_dirs[0] / "records.json").read_text())
        broken.pop()
        write(phase_dirs[0] / "records.json", broken)
        _, _, incomplete = sweep.importer.prepare(phase_dirs, work / "incomplete-import", "synthetic-ds-sweep-check")
        require("Importer retains the unretained original command and does not replace a missing DS trial", not incomplete["ds_sweep"]["complete"] and incomplete["ds_sweep"]["accepted_points"] == 99 and incomplete["ds_sweep"]["failed_or_unretained_points"] == 1 and any(not a["retained_by_collector"] for r in incomplete["runs"] for a in r["attempts"]))
    require("Production incremental_sources.json remains byte-for-byte unchanged", sweep.sha(sweep.importer.OFFICIAL) == production_before)
    receipt = {"schema_version": 1, "status": "passed", "scope": "Focused DS analysis/import checks; source arithmetic replay, no board execution or production-data installation",
               "checks_passed": len(checks), "checks": checks, "numerical_checks": numerical,
               "source_sha256": sources, "script_sha256": {sweep.relative(sweep.__file__): sweep.sha(sweep.__file__), sweep.relative(__file__): sweep.sha(__file__), sweep.relative(sweep.importer.__file__): sweep.sha(sweep.importer.__file__)},
               "matplotlib_version": version, "production_incremental_sha256": production_before,
               "synthetic_figures": "Nine outputs rendered only in memory; no synthetic curves saved or represented as measurements", "temporary_fixtures_removed": True}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(sweep.encoded(receipt))
    print(json.dumps({"status": "passed", "checks_passed": len(checks), "receipt": str(args.output)}))


if __name__ == "__main__":
    main()
