#!/usr/bin/env python3
"""Plot only the archived invalidation CSV; never build or run an experiment.

Run from any directory with a Python interpreter that provides matplotlib:
  python paper/carrv/scripts/plot_invalidation.py

The default inputs are the 24 principal cases plus four map-flush sensitivity
controls published on 2026-10-17. Output PDFs have a fixed 3.35-inch page width;
PNG previews have the same dimensions at 300 dpi. All text is 8 pt.
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
SOURCE_RELATIVE = Path(
    "iommu-platform/out/invalidation-interference-20261017/analysis/results.csv"
)
DOMAINS = ("same", "different")
FOOTPRINTS = (4, 8)
MODES = ("m0", "m1", "m2", "m3", "m4-16", "m4-64")
PLOTTED_MODES = MODES[1:]
MODE_LABELS = ("Broad", "PSCID", "Address", "Defer 16", "Defer 64")
BLUE, ORANGE, GREEN = "#0072B2", "#E69F00", "#009E73"
DERIVED_FIELDS = (
    "source_csv", "source_csv_line", "source_csv_sha256",
    "B_baseline_complete_steps", "B_completion_over_M0_recomputed",
    "B_completion_increase_pct", "A_broad_reference_case_id",
    "A_broad_complete_steps", "A_completion_over_broad",
    "A_completion_increase_over_broad_pct", "cq_total_commands",
    "neighbor_plot_role", "initiator_plot_role",
)


def case_id(domain: str, pages: int, mode: str, policy: str = "full-api") -> str:
    return f"{domain}-b{pages}-{mode}-{policy}"


def derive(source: Path) -> tuple[list[str], list[dict[str, str]]]:
    """Retain every source column and add explicit, independently computed ratios."""
    with source.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        source_fields = reader.fieldnames
        rows = list(reader)
    if not source_fields:
        raise ValueError("Missing CSV header")
    by_id = {row["case_id"]: row for row in rows}
    expected = {
        case_id(domain, pages, mode)
        for domain in DOMAINS for pages in FOOTPRINTS for mode in MODES
    } | {
        case_id(domain, pages, "m1", "unmap-only")
        for domain in DOMAINS for pages in FOOTPRINTS
    }
    if set(by_id) != expected or len(rows) != len(expected):
        raise ValueError("Input must contain the unique 24 principal + 4 control cases")
    source_sha = hashlib.sha256(source.read_bytes()).hexdigest()
    try:
        source_label = str(source.relative_to(WORK_ROOT))
    except ValueError:
        source_label = str(source)
    for line, row in enumerate(rows, start=2):
        domain, pages, mode = row["domain"], int(row["B_pages"]), row["mode"]
        if (int(row["A_jobs"]), int(row["B_jobs"]), int(row["copy_bytes"])) != (64, 256, 4096):
            raise ValueError(f"{row['case_id']}: workload differs from documented matrix")
        baseline = by_id[case_id(domain, pages, "m0")]
        broad = by_id[case_id(domain, pages, "m1")]
        if row["baseline_case_id"] != baseline["case_id"]:
            raise ValueError(f"{row['case_id']}: baseline changes domain/footprint")
        b_ratio = int(row["B_complete_steps"]) / int(baseline["B_complete_steps"])
        if not math.isclose(b_ratio, float(row["B_time_over_M0"]), rel_tol=1e-12):
            raise ValueError(f"{row['case_id']}: archived B ratio disagrees with raw times")
        if int(row["B_walks"]) - int(baseline["B_walks"]) != int(row["extra_B_walks_vs_M0"]):
            raise ValueError(f"{row['case_id']}: archived extra-walk difference is inconsistent")
        a_ratio = int(row["A_complete_with_final_flush_steps"]) / int(broad["A_complete_with_final_flush_steps"])
        expected_inval = (
            0 if mode == "m0" else
            128 if row["map_policy"] == "unmap-only" else
            136 if mode == "m4-16" else
            130 if mode == "m4-64" else 256
        )
        if (int(row["cq_iotinval"]), int(row["cq_iofence"])) != (expected_inval, 2 * expected_inval):
            raise ValueError(f"{row['case_id']}: command counts differ from documented policy")
        role = ("normalization-reference" if mode == "m0" else
                "unmap-only-control" if row["map_policy"] == "unmap-only" else
                "full-api-bar")
        row.update({
            "source_csv": source_label,
            "source_csv_line": str(line),
            "source_csv_sha256": source_sha,
            "B_baseline_complete_steps": baseline["B_complete_steps"],
            "B_completion_over_M0_recomputed": format(b_ratio, ".12g"),
            "B_completion_increase_pct": format(100 * (b_ratio - 1), ".12g"),
            "A_broad_reference_case_id": broad["case_id"],
            "A_broad_complete_steps": broad["A_complete_with_final_flush_steps"],
            "A_completion_over_broad": format(a_ratio, ".12g"),
            "A_completion_increase_over_broad_pct": format(100 * (a_ratio - 1), ".12g"),
            "cq_total_commands": str(int(row["cq_iotinval"]) + int(row["cq_iofence"])),
            "neighbor_plot_role": role,
            "initiator_plot_role": "unplotted-persistent-reference" if mode == "m0" else role,
        })
    return list(source_fields) + list(DERIVED_FIELDS), rows


def plot(rows: list[dict[str, str]], output: Path, plt, patches, lines) -> None:
    """Use identical horizontal-bar layouts and policy order for the two figures."""
    plt.rcParams.update({
        "font.family": "sans-serif",
        "font.sans-serif": ["DejaVu Sans"],
        "font.size": 8,
        "axes.titlesize": 8,
        "axes.labelsize": 8,
        "xtick.labelsize": 8,
        "ytick.labelsize": 8,
        "legend.fontsize": 8,
        "figure.facecolor": "white",
        "axes.facecolor": "white",
        "savefig.facecolor": "white",
        "savefig.transparent": False,
        "axes.spines.top": False,
        "axes.spines.right": False,
        "axes.spines.left": False,
        "axes.linewidth": 0.5,
        "pdf.fonttype": 42,
        "ps.fonttype": 42,
        "figure.dpi": 300,
        "savefig.dpi": 300,
    })
    by_key = {
        (row["domain"], int(row["B_pages"]), row["mode"], row["map_policy"]): row
        for row in rows
    }
    specs = (
        ("invalidation_neighbor", "B_completion_increase_pct", (-5, 21), (0, 10, 20),
         0, "B completion increase over M0 (%)"),
        ("invalidation_initiator", "A_completion_over_broad", (0, 1.22), (0, 0.5, 1),
         1, "A completion / broad (M1)\n(final flush included)"),
    )
    legend = [
        patches.Patch(facecolor=BLUE, edgecolor="none", label="4-page B"),
        patches.Patch(facecolor=ORANGE, edgecolor="white", hatch="////", label="8-page B"),
        lines.Line2D([], [], color=GREEN, marker="D", markerfacecolor="white",
                     linestyle="none", markersize=4, label="Unmap flush only"),
    ]
    for stem, metric, limits, ticks, reference, xlabel in specs:
        fig, axes = plt.subplots(1, 2, figsize=(3.35, 2.65), sharey=True)
        # Fixed margins preserve the exact final page width (no bbox_inches=tight).
        fig.subplots_adjust(left=0.235, right=0.98, bottom=0.24, top=0.77, wspace=0.20)
        fig.legend(handles=legend[:2], loc="upper center", bbox_to_anchor=(0.56, 1.005),
                   ncol=2, frameon=False, columnspacing=1.3, handlelength=1.1,
                   handletextpad=0.5, borderaxespad=0)
        fig.legend(handles=legend[2:], loc="upper center", bbox_to_anchor=(0.56, 0.935),
                   frameon=False, handlelength=1.1, handletextpad=0.5, borderaxespad=0)
        for ax, domain in zip(axes, DOMAINS):
            ax.set_title("Same domain" if domain == "same" else "Different domains", pad=5)
            for pages, color, offset, hatch in (
                (4, BLUE, -0.17, None), (8, ORANGE, 0.17, "////")
            ):
                positions = [index + offset for index in range(len(PLOTTED_MODES))]
                values = [float(by_key[domain, pages, mode, "full-api"][metric])
                          for mode in PLOTTED_MODES]
                ax.barh(positions, values, height=0.29, color=color, edgecolor="#ffffff",
                        linewidth=0.3, hatch=hatch, zorder=2)
                # Controls align vertically with the matching footprint's broad bar.
                control = by_key[domain, pages, "m1", "unmap-only"]
                ax.plot(float(control[metric]), offset, marker="D", markersize=4,
                        markerfacecolor="white", markeredgecolor=GREEN,
                        markeredgewidth=1.0, linestyle="none", zorder=5)
            ax.axvline(reference, color="0.30", linewidth=0.7, linestyle=(0, (3, 2)), zorder=3)
            ax.set_xlim(limits)
            ax.set_xticks(ticks)
            ax.set_yticks(range(len(MODE_LABELS)), MODE_LABELS)
            ax.set_ylim(len(MODE_LABELS) - 0.55, -0.55)
            ax.tick_params(axis="y", length=0, pad=4)
            ax.tick_params(axis="x", width=0.5, length=2.5, pad=3)
            ax.grid(False)
        axes[1].tick_params(axis="y", labelleft=False)
        fig.supxlabel(xlabel, x=0.58, y=0.035, fontsize=8)
        fig.savefig(output / f"{stem}.pdf", metadata={
            "Creator": "plot_invalidation.py; matplotlib",
            "Title": stem.replace("_", " "),
            "CreationDate": None, "ModDate": None,
        })
        fig.savefig(output / f"{stem}.png", dpi=300)
        plt.close(fig)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, default=WORK_ROOT / SOURCE_RELATIVE)
    parser.add_argument("--figures", type=Path, default=PAPER_ROOT / "figures")
    parser.add_argument("--derived", type=Path, default=PAPER_ROOT / "data/invalidation-derived.csv")
    args = parser.parse_args()
    fields, rows = derive(args.input.resolve())
    args.figures.mkdir(parents=True, exist_ok=True)
    args.derived.parent.mkdir(parents=True, exist_ok=True)
    with args.derived.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    # Matplotlib's font cache stays under the paper directory and is removed.
    with tempfile.TemporaryDirectory(prefix=".invalidation-mpl-", dir=PAPER_ROOT) as cache:
        os.environ["MPLCONFIGDIR"] = cache
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
        from matplotlib import lines, patches
        plot(rows, args.figures, plt, patches, lines)
    print(f"Read {len(rows)} archived cases; wrote {args.derived} and four figure files.")


if __name__ == "__main__":
    main()
