#!/usr/bin/env python3
"""Independently check figure numbers using frozen paper.csv and exact ratios.

This deliberately does not import the plotting script or reuse its summary
parser, floating-point formula, or quantile implementation. It is an offline
numerical check, not a rerun of the collector or a new board experiment.
"""

import argparse
import csv
import hashlib
import json
import math
import re
import statistics
from fractions import Fraction
from pathlib import Path


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def rows(path):
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--figures", type=Path, default=Path(__file__).parent)
    args = parser.parse_args()
    source, figures = args.input.resolve(), args.figures.resolve()
    paper = source / "paper.csv"
    if sha256(paper) != "81f9c197a8cd0d821273fb7498ad7a246faa5009f947b743c32a254a379d68af":
        raise ValueError("Frozen paper.csv SHA256 mismatch")
    manifest = json.loads((figures / "manifest.json").read_text())
    for filename, expected in manifest["artifacts"].items():
        if sha256(figures / filename) != expected["sha256"]:
            raise ValueError(f"Figure artifact changed: {filename}")
    cycles = {}
    for row in rows(paper):
        if row["metric"] != "hardware_cycles" or row["statistic"] != "sample":
            continue
        fields = dict(field.split("=", 1) for field in row["condition"].split(";"))
        key = (row["boot_id"], int(fields["block"]), int(fields["V_bytes"]), fields["mode"])
        if key in cycles:
            raise ValueError("Duplicate paper.csv trial")
        case = int(re.search(r"\.case(\d+)\.hardware_cycles\.sample$", row["row_id"])[1])
        cycles[key] = (Fraction(row["value"]), case)
    if len(cycles) != 45 or len({key[0] for key in cycles}) != 1:
        raise ValueError("Expected 45 individual cycle observations in one boot")
    boot = next(iter(cycles))[0]
    exact, differences, maximum_error = {}, {}, 0.0

    def check(actual, expected):
        nonlocal maximum_error
        error = abs(float(actual) - float(expected))
        maximum_error = max(maximum_error, error)
        if not math.isclose(float(actual), float(expected), rel_tol=1e-12, abs_tol=1e-10):
            raise ValueError(f"Numerical mismatch: {actual} vs {expected}")

    pairs = rows(figures / "paired-samples.csv")
    if len(pairs) != 15:
        raise ValueError("Expected 15 same-size pairs")
    for pair in pairs:
        size, block = int(pair["victim_chunk_bytes"]), int(pair["block"])
        a8, case8 = cycles[(boot, block, size, "A8")]
        a1, case1 = cycles[(boot, block, size, "A1")]
        check(pair["A8_case"], case8)
        check(pair["A1_case"], case1)
        check(pair["A8_hardware_cycles"], a8)
        check(pair["A1_hardware_cycles"], a1)
        ratio = a8 / a1
        check(pair["time_ratio_A8_over_A1"], ratio)
        exact[(size, block, "time_increase_percent")] = 100 * (a8 - a1) / a1
        exact[(size, block, "throughput_loss_percent")] = 100 * (a8 - a1) / a8
        for metric in ("time_increase_percent", "throughput_loss_percent"):
            check(pair[metric], exact[(size, block, metric)])
    contrasts = rows(figures / "size-contrasts.csv")
    if len(contrasts) != 10:
        raise ValueError("Expected 10 within-block size contrasts")
    for contrast in contrasts:
        size, block = int(contrast["victim_chunk_bytes"]), int(contrast["block"])
        for target, base in (("time_increase_difference_pp", "time_increase_percent"),
                             ("throughput_loss_difference_pp", "throughput_loss_percent")):
            differences[(size, block, target)] = exact[(size, block, base)] - exact[(4096, block, base)]
            check(contrast[target], differences[(size, block, target)])
    checked = []
    for row in rows(figures / "descriptive-statistics.csv"):
        size, metric = int(row["victim_chunk_bytes"]), row["metric"]
        store = differences if metric.endswith("_pp") else exact
        values = [store[(size, block, metric)] for block in range(5)]
        q1, _, q3 = statistics.quantiles(values, n=4, method="inclusive")
        result = dict(n=len(values), min=min(values), q1=q1,
                      median=statistics.median(values), q3=q3, max=max(values), iqr=q3 - q1)
        for name, value in result.items():
            check(row[name], value)
        checked.append(dict(victim_chunk_bytes=size, metric=metric,
                            **{name: float(value) for name, value in result.items()}))
    if len(checked) != 10 or len(manifest["trial_keys"]) != 45:
        raise ValueError("Statistics or trial-key count differs")
    report = dict(status="VERIFIED_OFFLINE_NUMBERS", source_board_cases=45,
                  additional_board_cases=0, source_paper_sha256=sha256(paper),
                  figure_manifest_sha256=sha256(figures / "manifest.json"),
                  verifier_sha256=sha256(Path(__file__)),
                  method="paper.csv sample cycles; exact Fraction ratios/differences; statistics.quantiles inclusive (Type7)",
                  pairs_checked=len(pairs), within_block_contrasts_checked=len(contrasts),
                  statistics_checked=len(checked), max_absolute_numeric_difference=maximum_error,
                  absolute_tolerance=1e-10, relative_tolerance=1e-12, statistics=checked,
                  scope="Independent offline formulas only; Root's raw-source audit and hardware runs were not repeated")
    (figures / "numerical-verification.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: value for key, value in report.items() if key != "statistics"}))


if __name__ == "__main__":
    main()
