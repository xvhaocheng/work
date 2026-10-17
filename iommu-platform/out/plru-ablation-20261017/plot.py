#!/usr/bin/env python3
"""Render the measured completion-time comparison; never starts simulations."""
import csv
import hashlib
import json
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    root = Path(__file__).resolve().parent
    source = root / "analysis/comparison.csv"
    with source.open(newline="") as stream:
        rows = list(csv.DictReader(stream))
    indexed = {}
    for row in rows:
        key = (row["variant"].lower(), row["mode"].lower(), int(row["ring"]))
        if key in indexed:
            raise ValueError(f"Duplicate primary measurement: {key}")
        indexed[key] = int(row["T_return"])
    expected = {(v, m, r) for v in ("v0", "v1", "v2")
                for m in ("p", "pn") for r in (1, 8, 16)}
    if set(indexed) != expected:
        raise ValueError(f"Matrix mismatch: {set(indexed) ^ expected}")
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 10,
                         "svg.fonttype": "none", "axes.spines.top": False,
                         "axes.spines.right": False})
    fig, axes = plt.subplots(1, 2, figsize=(10.0, 4.1), sharey=True)
    positions = np.arange(3)
    width = .24
    variants = (("v0", "V0: original masked PLRU", "#667085"),
                ("v1", "V1: compacted-mask PLRU", "#2878B5"),
                ("v2", "V2: exact LRU", "#D97706"))
    for axis, mode in zip(axes, ("p", "pn")):
        for offset, (variant, label, color) in enumerate(variants):
            values = [indexed[(variant, mode, ring)] for ring in (1, 8, 16)]
            bars = axis.bar(positions + (offset - 1) * width,
                            np.array(values) / 1000, width=width,
                            label=label, color=color, zorder=3)
            for bar, value in zip(bars, values):
                axis.text(bar.get_x() + bar.get_width()/2,
                          bar.get_height() + .65, f"{value/1000:.1f}",
                          ha="center", va="bottom", fontsize=8)
        axis.set_xticks(positions, ("R1", "R8", "R16"))
        axis.set_xlabel("A destination reuse span (pages)")
        axis.set_title(mode.upper())
        axis.grid(axis="y", alpha=.2, zorder=0)
        axis.set_axisbelow(True)
    axes[0].set_ylabel("Full completion time (1,000 simulation clock steps)")
    axes[0].set_ylim(0, max(indexed.values()) / 1000 * 1.14)
    handles, labels = axes[0].get_legend_handles_labels()
    fig.legend(handles, labels, loc="upper center", ncol=3,
               frameon=False, bbox_to_anchor=(.5, 1.025), fontsize=9)
    fig.tight_layout(rect=(0, .02, 1, .94))
    target = root / "figures"
    target.mkdir(exist_ok=True)
    for extension in ("png", "svg"):
        fig.savefig(target / f"completion-time.{extension}", dpi=200,
                    bbox_inches="tight")
    plt.close(fig)
    metadata = {"input": str(source.relative_to(root)),
                "input_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
                "measurements": len(indexed), "unit": "RTL simulation clock steps",
                "matplotlib_version": matplotlib.__version__, "numpy_version": np.__version__,
                "script_sha256": hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                "scope": "Behavioral DDR model; not measured FPGA or Linux application time"}
    (target / "plot-metadata.json").write_text(json.dumps(metadata, indent=2) + "\n")


if __name__ == "__main__":
    main()
