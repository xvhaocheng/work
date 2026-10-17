#!/usr/bin/env python3
"""Replot the delivered mainline/dead-entry results without running experiments.

The CSV's 16 matrix rows provide the figure; 12 diagnostic rows are retained in
the derived CSV but never mixed with the mainline comparison. Same/different
domain results must match the displayed metrics before they are coalesced.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import math
import os
from pathlib import Path
import tempfile


PAPER_ROOT = Path(__file__).resolve().parents[1]
WORK_ROOT = PAPER_ROOT.parents[1]
SOURCE_RELATIVE = Path("iommu-platform/out/dead-translation-20261017/analysis/final/results.csv")
DOMAINS = ("same", "different")
FOOTPRINTS = (4, 8)
MODES = ("m0", "ml-strict", "ml-fq", "oracle")
PLOTTED_MODES = MODES[1:]
BLUE, ORANGE, GREEN = "#0072B2", "#E69F00", "#009E73"
EXTRA_FIELDS = (
    "source_csv", "source_csv_line", "source_csv_sha256", "B_baseline_steps",
    "B_over_M0_recomputed", "B_increase_over_M0_pct", "dead_mean_slot_pct",
    "dead_peak_slot_pct", "oracle_benefit_over_FQ_pct", "fq_remaining_capacity",
    "work_completed_before_timeout", "plot_role",
)


def case_id(domain: str, pages: int, mode: str) -> str:
    return f"{domain}-b{pages}-{mode}"


def derive(source: Path) -> tuple[list[str], list[dict[str, str]]]:
    with source.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        original_fields = reader.fieldnames
        rows = list(reader)
    if not original_fields:
        raise ValueError("Missing CSV header")
    matrix = [row for row in rows if row["group"] == "matrix"]
    by_id = {row["case_id"]: row for row in matrix}
    expected = {case_id(domain, pages, mode) for domain in DOMAINS
                for pages in FOOTPRINTS for mode in MODES}
    if len(rows) != 28 or len(matrix) != 16 or set(by_id) != expected:
        raise ValueError("Expected the delivered 16 matrix + 12 diagnostic rows")
    source_sha = hashlib.sha256(source.read_bytes()).hexdigest()
    try:
        label = str(source.relative_to(WORK_ROOT))
    except ValueError:
        label = str(source)
    for line, row in enumerate(rows, 2):
        row.update({name: "" for name in EXTRA_FIELDS})
        row.update(source_csv=label, source_csv_line=str(line), source_csv_sha256=source_sha)
        if row["group"] != "matrix":
            row["plot_role"] = "diagnostic-unplotted"
            continue
        if row["checks_passed"] != "True":
            raise ValueError(f"{row['case_id']}: original checks did not pass")
        if (int(row["a_jobs"]), int(row["b_jobs"])) != (64, 256):
            raise ValueError(f"{row['case_id']}: different workload")
        domain, pages = row["domain"], int(row["b_pages"])
        baseline = by_id[case_id(domain, pages, "m0")]
        if row["baseline_case_id"] != baseline["case_id"]:
            raise ValueError(f"{row['case_id']}: baseline changes domain/footprint")
        b_ratio = int(row["T_B"]) / int(baseline["T_B"])
        dead_mean = int(row["dead_slot_cycles"]) / (16 * int(row["dead_sample_cycles"]))
        dead_peak = int(row["dead_peak"]) / 16
        for calculated, original, name in (
            (b_ratio, float(row["T_B_over_m0"]), "B normalization"),
            (dead_mean, float(row["dead_average_slot_fraction"]), "mean dead occupancy"),
            (dead_peak, float(row["dead_peak_slot_fraction"]), "peak dead occupancy"),
        ):
            if not math.isclose(calculated, original, rel_tol=1e-12, abs_tol=1e-15):
                raise ValueError(f"{row['case_id']}: inconsistent {name}")
        oracle_benefit = ""
        if row["mode"] == "oracle":
            fq = by_id[case_id(domain, pages, "ml-fq")]
            oracle_benefit = format(100 * (int(fq["T_B"]) - int(row["T_B"])) / int(fq["T_B"]), ".12g")
        if row["mode"] in ("ml-fq", "oracle"):
            if not (int(row["fq_peak"]) < int(row["fq_capacity"]) and
                    int(row["T_end"]) < int(row["fq_timeout_cycles"]) < int(row["fq_drain_end"])):
                raise ValueError(f"{row['case_id']}: FQ window differs from short-workload condition")
            if (int(row["fq_flushes"]), int(row["fq_timer_flushes"]), int(row["fq_full_flushes"])) != (1, 1, 0):
                raise ValueError(f"{row['case_id']}: unexpected FQ trigger counts")
        row.update({
            "B_baseline_steps": baseline["T_B"],
            "B_over_M0_recomputed": format(b_ratio, ".12g"),
            "B_increase_over_M0_pct": format(100 * (b_ratio - 1), ".12g"),
            "dead_mean_slot_pct": format(100 * dead_mean, ".12g"),
            "dead_peak_slot_pct": format(100 * dead_peak, ".12g"),
            "oracle_benefit_over_FQ_pct": oracle_benefit,
            "fq_remaining_capacity": str(int(row["fq_capacity"]) - int(row["fq_peak"])),
            "work_completed_before_timeout": str(int(row["T_end"]) < int(row["fq_timeout_cycles"])),
            "plot_role": "normalization-reference" if row["mode"] == "m0" else
                         "same-domain-representative" if domain == "same" else
                         "identical-different-domain-check",
        })
    # Coalesce only metrics that are actually identical; command tag observations
    # such as invalidated-way counts differ legitimately across domain setups.
    equal_fields = (
        "T_B", "T_A", "A_final_flush_end", "T_end", "dead_slot_cycles",
        "dead_sample_cycles", "dead_peak", "B_walks", "fq_drain_end",
        "fq_peak", "fq_capacity", "fq_flushes", "fq_timer_flushes", "fq_full_flushes",
    )
    for pages in FOOTPRINTS:
        for mode in MODES:
            same, different = (by_id[case_id(domain, pages, mode)] for domain in DOMAINS)
            if any(same[field] != different[field] for field in equal_fields):
                raise ValueError(f"{pages}/{mode}: domain results cannot be coalesced")
    return list(original_fields) + list(EXTRA_FIELDS), rows


def plot(rows: list[dict[str, str]], figures: Path, plt, patches, lines) -> None:
    plt.rcParams.update({
        "font.family": "sans-serif", "font.sans-serif": ["DejaVu Sans"],
        "font.size": 8, "axes.titlesize": 8, "axes.labelsize": 8,
        "xtick.labelsize": 8, "ytick.labelsize": 8, "legend.fontsize": 8,
        "figure.facecolor": "white", "axes.facecolor": "white",
        "savefig.facecolor": "white", "savefig.transparent": False,
        "axes.spines.top": False, "axes.spines.right": False,
        "axes.spines.left": False, "axes.linewidth": 0.5,
        "pdf.fonttype": 42, "ps.fonttype": 42, "figure.dpi": 300, "savefig.dpi": 300,
    })
    by_key = {(int(row["b_pages"]), row["mode"]): row for row in rows
              if row["group"] == "matrix" and row["domain"] == "same"}
    fig, axes = plt.subplots(1, 2, figsize=(3.35, 2.7), sharey=True)
    fig.subplots_adjust(left=0.235, right=0.98, bottom=0.27, top=0.70, wspace=0.28)
    fig.text(0.56, 0.985, "Same / different domains coincide", ha="center", va="top", fontsize=8)
    handles = [patches.Patch(facecolor=BLUE, edgecolor="none", label="4-page B"),
               patches.Patch(facecolor=ORANGE, edgecolor="white", hatch="////", label="8-page B")]
    fig.legend(handles=handles, loc="upper center", bbox_to_anchor=(0.56, 0.925),
               ncol=2, frameon=False, handlelength=1.1, columnspacing=1.3,
               handletextpad=0.5, borderaxespad=0)
    peak_handle = lines.Line2D([], [], color=GREEN, marker="D", markerfacecolor="white",
                              linestyle="none", markersize=4, label="Peak dead slots (b)")
    fig.legend(handles=[peak_handle], loc="upper center", bbox_to_anchor=(0.56, 0.855),
               frameon=False, handlelength=1.1, handletextpad=0.5, borderaxespad=0)
    for ax, metric in zip(axes, ("B_increase_over_M0_pct", "dead_mean_slot_pct")):
        for pages, color, offset, hatch in ((4, BLUE, -0.17, None), (8, ORANGE, 0.17, "////")):
            positions = [index + offset for index in range(3)]
            values = [float(by_key[pages, mode][metric]) for mode in PLOTTED_MODES]
            ax.barh(positions, values, height=0.29, color=color, edgecolor="white",
                    linewidth=0.3, hatch=hatch, zorder=2)
            # Zero-width bars and the tiny oracle mean remain explicitly visible.
            for position, value in zip(positions, values):
                if value < 0.01:
                    ax.plot(value, position, marker="o", markersize=3,
                            markerfacecolor=color, markeredgecolor=color,
                            linestyle="none", zorder=4, clip_on=False)
            if metric == "dead_mean_slot_pct":
                ax.plot([float(by_key[pages, mode]["dead_peak_slot_pct"]) for mode in PLOTTED_MODES],
                        positions, linestyle="none", marker="D", markersize=4,
                        markerfacecolor="white", markeredgecolor=GREEN,
                        markeredgewidth=1, zorder=5)
        ax.set_yticks(range(3), ("ML-strict", "ML-FQ", "Oracle"))
        ax.set_ylim(2.55, -0.55)
        ax.tick_params(axis="y", length=0, pad=4)
        ax.tick_params(axis="x", width=0.5, length=2.5, pad=3)
        ax.grid(False)
    axes[0].set_title("(a) B completion", pad=6)
    axes[0].set_xlim(-0.06, 3.1)
    axes[0].set_xticks((0, 1, 2, 3))
    axes[0].set_xlabel("Increase over M0 (%)", labelpad=5)
    axes[1].set_title("(b) Dead slots", pad=6)
    axes[1].set_xlim(-1, 56)
    axes[1].set_xticks((0, 25, 50))
    axes[1].set_xlabel("Mean / peak (%)", labelpad=5)
    axes[1].tick_params(axis="y", labelleft=False)
    fig.text(0.56, 0.035, "Work completes before the 10 ms FQ timer", ha="center", va="bottom", fontsize=8)
    fig.savefig(figures / "mainline_baseline.pdf", metadata={
        "Creator": "plot_mainline.py; matplotlib", "Title": "Mainline baseline and dead occupancy",
        "CreationDate": None, "ModDate": None,
    })
    fig.savefig(figures / "mainline_baseline.png", dpi=300)
    plt.close(fig)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, default=WORK_ROOT / SOURCE_RELATIVE)
    parser.add_argument("--figures", type=Path, default=PAPER_ROOT / "figures")
    parser.add_argument("--derived", type=Path, default=PAPER_ROOT / "data/mainline-derived.csv")
    args = parser.parse_args()
    fields, rows = derive(args.input.resolve())
    args.figures.mkdir(parents=True, exist_ok=True)
    args.derived.parent.mkdir(parents=True, exist_ok=True)
    with args.derived.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    with tempfile.TemporaryDirectory(prefix=".mainline-mpl-", dir=PAPER_ROOT) as cache:
        os.environ["MPLCONFIGDIR"] = cache
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
        from matplotlib import lines, patches
        plot(rows, args.figures, plt, patches, lines)
    print(f"Read {len(rows)} archived rows (16 matrix); wrote {args.derived} and two figure files.")


if __name__ == "__main__":
    main()
