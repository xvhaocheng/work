#!/usr/bin/env python3
"""Plot existing PLRU-ablation CSV results; never invoke an experiment.

Run from any directory. Outputs stay under paper/carrv. The source CSV is
read-only; the derived CSV preserves every original row and its line number.
"""

from __future__ import annotations

import csv
import hashlib
import os
from pathlib import Path

PAPER = Path(__file__).resolve().parents[1]
WORK = PAPER.parents[1]
SOURCE = WORK / "iommu-platform/out/plru-ablation-20261017/analysis/comparison.csv"
DERIVED = PAPER / "data/plru-derived.csv"
FIGURES = PAPER / "figures"
os.environ.setdefault("MPLCONFIGDIR", str(PAPER / ".cache/matplotlib"))

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def read_results() -> tuple[list[dict[str, str]], dict[tuple[str, str, int], dict[str, str]]]:
    with SOURCE.open(newline="") as stream:
        rows = list(csv.DictReader(stream))
    indexed = {(r["variant"], r["mode"], int(r["ring"])): r for r in rows}
    expected = {(v, m, r) for v in ("v0", "v1", "v2") for m in ("p", "pn") for r in (1, 8, 16)}
    if len(rows) != 18 or set(indexed) != expected:
        raise ValueError("Expected precisely the 18 existing primary configurations")
    for variant in ("v0", "v1", "v2"):
        for ring in (1, 8, 16):
            p, pn = (indexed[variant, mode, ring] for mode in ("p", "pn"))
            if any(p[field] != pn[field] for field in ("walks_total", "walks_A", "walks_V")):
                raise ValueError("The common P/PN walk panel requires matching walk counts")
    return rows, indexed


def write_derived(rows: list[dict[str, str]], indexed: dict) -> None:
    digest = hashlib.sha256(SOURCE.read_bytes()).hexdigest()
    additional = ["baseline_T_return", "return_reduction_steps", "return_reduction_pct",
                  "source_file", "source_line", "source_sha256"]
    DERIVED.parent.mkdir(parents=True, exist_ok=True)
    with DERIVED.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]) + additional, lineterminator="\n")
        writer.writeheader()
        for line, row in enumerate(rows, start=2):
            baseline = int(indexed["v0", row["mode"], int(row["ring"])]["T_return"])
            reduction = baseline - int(row["T_return"])
            writer.writerow({**row, "baseline_T_return": baseline,
                             "return_reduction_steps": reduction,
                             "return_reduction_pct": f"{100 * reduction / baseline:.8f}",
                             "source_file": SOURCE.relative_to(WORK).as_posix(),
                             "source_line": line, "source_sha256": digest})


def plot(indexed: dict) -> None:
    matplotlib.rcParams.update({
        "font.family": "sans-serif",
        "font.sans-serif": ["DejaVu Sans"],
        "font.size": 8,
        "axes.titlesize": 8,
        "axes.labelsize": 8,
        "xtick.labelsize": 8,
        "ytick.labelsize": 8,
        "legend.fontsize": 8,
        "axes.spines.top": False,
        "axes.spines.right": False,
        "axes.linewidth": 0.6,
        "pdf.fonttype": 42,
        "ps.fonttype": 42,
        "figure.facecolor": "white",
        "axes.facecolor": "white",
        "savefig.facecolor": "white",
        "savefig.dpi": 300,
    })
    variants = ("v0", "v1", "v2")
    colors = ("#0072B2", "#E69F00", "#009E73")
    fig, (walk_ax, time_ax) = plt.subplots(2, 1, figsize=(3.35, 4.10))
    fig.subplots_adjust(left=0.20, right=0.98, bottom=0.14, top=0.89, hspace=0.76)
    width = 0.22

    rings = (1, 8, 16)
    x = np.arange(len(rings))
    for offset, variant, color in zip((-width, 0, width), variants, colors):
        values = [int(indexed[variant, "p", ring]["walks_total"]) for ring in rings]
        bars = walk_ax.bar(x + offset, values, width=width, label=variant.upper(), color=color)
        walk_ax.bar_label(bars, padding=2, fontsize=8)
    walk_ax.set(xticks=x, xticklabels=[str(r) for r in rings], ylim=(0, 80),
                yticks=(0, 20, 40, 60, 80), ylabel="Total PTW walks (count)",
                xlabel="A destination ring, R (pages)")
    walk_ax.set_title("(a) Walk counts, identical for P and PN", loc="left", pad=7)

    groups = (("p", 8), ("pn", 8), ("p", 16), ("pn", 16))
    x = np.arange(len(groups))
    for offset, variant, color in zip((-width, 0, width), variants, colors):
        values = [int(indexed[variant, mode, ring]["T_return"]) / 1000 for mode, ring in groups]
        time_ax.bar(x + offset, values, width=width, color=color)
    time_ax.set(xticks=x, xticklabels=[f"{mode.upper()}\nR{ring}" for mode, ring in groups],
                ylim=(0, 150), yticks=(0, 50, 100, 150),
                ylabel="RTL clock steps (×10³)", xlabel="Organization and destination ring")
    time_ax.set_title("(b) Complete return time", loc="left", pad=7)
    for i, mode in enumerate(("p", "pn")):
        baseline = int(indexed["v0", mode, 8]["T_return"])
        lru = int(indexed["v2", mode, 8]["T_return"])
        reduction = 100 * (baseline - lru) / baseline
        # Text sits above each whole R8 group; a leader points to its V2 bar.
        time_ax.annotate(f"−{reduction:.2f}%", xy=(i + width, lru / 1000),
                         xytext=(i, baseline / 1000 + 13), ha="center", va="bottom",
                         fontsize=8, arrowprops={"arrowstyle": "-", "lw": 0.6, "color": "#444444"})
    for ax in (walk_ax, time_ax):
        ax.tick_params(width=0.6, length=3, pad=3)
        ax.set_axisbelow(True)
        ax.grid(axis="y", color="#DDDDDD", linewidth=0.4)
    fig.legend(*walk_ax.get_legend_handles_labels(), loc="upper center", ncol=3,
               frameon=False, bbox_to_anchor=(0.58, 0.995), handlelength=1.1, columnspacing=1.4)
    FIGURES.mkdir(parents=True, exist_ok=True)
    # Do not use bbox_inches='tight': preserve the exact 3.35-inch column width.
    fig.savefig(FIGURES / "plru_ablation.pdf", metadata={"CreationDate": None, "ModDate": None})
    fig.savefig(FIGURES / "plru_ablation.png", dpi=300)
    plt.close(fig)


def main() -> None:
    rows, indexed = read_results()
    write_derived(rows, indexed)
    plot(indexed)
    print(f"Read {len(rows)} existing CSV rows; wrote {DERIVED.relative_to(PAPER)} and PLRU PDF/PNG.")


if __name__ == "__main__":
    main()
