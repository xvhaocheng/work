#!/usr/bin/env python3
"""Offline matched-block fit of excess victim cycles to observed aggressor walks.

Rebuild from the canonical long CSV, without opening hardware or raw UART logs.
Primary fit: DeltaT = Nwalk * (t0 + k * DS), OLS in cycles, no extra intercept.
Example: python interference_model.py --levels 3 --levels-status confirmed
Dependencies: numpy and matplotlib. No packages are downloaded by this script.
"""
from __future__ import annotations

import argparse
import copy
import csv
import hashlib
import io
import json
import sys
from collections import Counter, defaultdict
from itertools import product
from pathlib import Path

import numpy as np

sys.dont_write_bytecode = True
PAPER = Path(__file__).resolve().parents[2]
LEVELS = (0, 25, 50, 100)
MODES = ("Solo", "A1", "A8", "A16", "A64")
FOOTPRINTS = MODES[2:]
METRICS = ("hardware_cycles", "aggressor_s1_ptw", "victim_s1_ptw")
SEED = 20261018
BOOTSTRAPS = 10000
CLOCK_HZ = 50_000_000
CYCLE_NS = 20.0
# Identity contract for this fixed 200-trial campaign, from accepted load/build
# receipts. These are four intentionally distinct DS BITs, with one Linux ABI.
EXPECTED_BIT_SHA256 = {
    0: "e9b6554f2b3a64a19f910c672781ad38b7c91879b560d382506aca5ec5df1da6",
    25: "48896ebf59f65df339a369fba30e1b5b0c77d2b1e49916f921bdc91c09da10fb",
    50: "580b34014914a1bee8541ed1e23b36a164bfc0fea3a2f691f56870b44dc35e77",
    100: "11f0f2d6bce196707df98262d8f21cf361634d5599c273a75bbc33622a3be9e1",
}
EXPECTED_KERNEL = "6.5.0-rc2-ispass2027-pathv3"


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def require(ok, message):
    if not ok:
        raise ValueError(message)


def conditions(text):
    entries = [x.split("=", 1) for x in text.split(";")]
    require(all(len(x) == 2 for x in entries), "malformed condition")
    require(len({x[0] for x in entries}) == len(entries), "duplicate condition key")
    return dict(entries)


def extract(rows):
    """Validate sample rows and form 200 trials / 40 complete matched blocks."""
    ids = set()
    trials = {}
    relevant = 0
    for line, row in enumerate(rows, 2):
        require(row["row_id"] not in ids, "duplicate row_id: " + row["row_id"])
        ids.add(row["row_id"])
        if row["experiment"] not in {"board_baseline100", "board_ds_sweep"}:
            continue
        if row["statistic"] != "sample" or row["metric"] not in METRICS:
            continue
        relevant += 1
        c = conditions(row["condition"])
        ds_parts = c["ds"].split("/")
        require(len(ds_parts) == 2 and ds_parts[0] == ds_parts[1], "asymmetric DS")
        ds = int(ds_parts[0])
        mode, block, boot = c["mode"], int(c["block"]), row["boot_id"]
        require(ds in LEVELS and mode in MODES and 0 <= block < 10, "unexpected cell")
        require(c["role"] == "V0-A1" and c["boot"] == boot, "role/boot disagreement")
        require(c["attempt_status"] == "accepted" and c["campaign_status"] == "passed"
                and c["phase_complete"] == "1", "non-accepted/incomplete input")
        require(row["n"] == "1" and row["evidence_level"] == "fpga_board", "not a trial scalar")
        require(boot not in {"unknown", ""} and not boot.startswith("mixed:"), "unknown/mixed boot")
        require(row["bitstream_id"].startswith("program_file_sha256:"), "missing program-file identity")
        require(row["bitstream_id"] == "program_file_sha256:" + EXPECTED_BIT_SHA256[ds], "DS/BIT identity contract mismatch")
        require(row["identity_status"] == "program_file_hash_boot_verified_no_readback", "identity contract")
        metric = row["metric"]
        expected_unit = "cycles" if metric == "hardware_cycles" else "count"
        expected_window = "hardware_V_batch" if metric == "hardware_cycles" else "non_atomic_HPM_software_snapshot"
        require(row["unit"] == expected_unit and c["window"] == expected_window, "metric unit/window mismatch")
        expected_suffix = {"hardware_cycles": "/observer/time/cycles",
                           "aggressor_s1_ptw": "/record/result/aggressor/s1_ptw",
                           "victim_s1_ptw": "/record/result/victim/s1_ptw"}[metric]
        na = metric == "aggressor_s1_ptw" and mode == "Solo"
        if na:
            require(row["value_status"] == "not_applicable" and row["value"] == "unknown", "Solo walk not N/A")
            value = None
        else:
            require(row["value_status"] == "observed", "metric is not an observed sample")
            value = int(row["value"])
            require(value >= 0 and row["source_locator"].endswith(expected_suffix), "invalid value/raw pointer")
            require(row["raw_path"].endswith("/records.json"), "unexpected raw source")
        key = ds, boot, block, mode
        base = (row["bitstream_id"], row["kernel_id"], c["run"], c["phase"])
        trial = trials.setdefault(key, {"ds": ds, "boot_id": boot, "block": block,
                                       "footprint": mode, "base": base, "metrics": {}})
        require(trial["base"] == base, "mixed base within trial")
        require(metric not in trial["metrics"], "duplicate trial metric")
        trial["metrics"][metric] = {"value": value, "row_id": row["row_id"], "csv_line": line,
                                     "raw_path": row["raw_path"], "pointer": row["source_locator"]}
    require(relevant == 600 and len(trials) == 200, "require 200 trials and 600 selected scalar rows")
    by_ds_block = defaultdict(set)
    ds_boots = defaultdict(set)
    for (ds, boot, block, mode), t in trials.items():
        require(set(t["metrics"]) == set(METRICS), "missing trial metric")
        require(t["metrics"]["hardware_cycles"]["value"] > 0, "zero hardware time")
        time_source = t["metrics"]["hardware_cycles"]
        for metric in ["victim_s1_ptw"] + ([] if mode == "Solo" else ["aggressor_s1_ptw"]):
            source = t["metrics"][metric]
            require(source["raw_path"] == time_source["raw_path"] and
                    source["pointer"].split("/")[1] == time_source["pointer"].split("/")[1],
                    "counter/time raw trial source mismatch")
        if mode == "A1":
            require(t["metrics"]["aggressor_s1_ptw"]["value"] == 0, "A1 has walks: revise model definition")
        if mode in FOOTPRINTS:
            require(t["metrics"]["aggressor_s1_ptw"]["value"] > 0, "zero aggressor walks")
        by_ds_block[ds, block].add(boot)
        ds_boots[ds].add(boot)
    require(set(by_ds_block) == {(ds, b) for ds in LEVELS for b in range(10)}, "missing DS/block")
    require(all(len(x) == 1 for x in by_ds_block.values()), "cross-boot block")
    require({ds: len(bs) for ds, bs in ds_boots.items()} == {0: 1, 25: 1, 50: 2, 100: 1}, "recorded boot design changed")
    for ds in LEVELS:
        require(len({t['base'][:2] for t in trials.values() if t['ds'] == ds}) == 1, "mixed BIT/kernel within DS")
    require({t["base"][1] for t in trials.values()} == {EXPECTED_KERNEL}, "cross-DS unified kernel identity mismatch")
    blocks, pairs = [], []
    for ds, block in sorted(by_ds_block):
        boot = next(iter(by_ds_block[ds, block]))
        cell = [trials.get((ds, boot, block, mode)) for mode in MODES]
        require(all(x is not None for x in cell), "missing full block")
        require(len({x["base"] for x in cell}) == 1, "mixed base within paired block")
        a1 = cell[1]
        a1metric = a1["metrics"]["hardware_cycles"]
        block_id = f"DS{ds}:{boot}:block{block}"
        blocks.append({"block_id": block_id, "ds": ds, "boot_id": boot, "block": block})
        for t in cell[2:]:
            tv = t["metrics"]["hardware_cycles"]
            nw = t["metrics"]["aggressor_s1_ptw"]
            delta = tv["value"] - a1metric["value"]
            require(delta > 0, "nonpositive excess time")
            pairs.append({"sample_id": block_id + ":" + t["footprint"], "block_id": block_id,
                          "ds": ds, "boot_id": boot, "block": block, "footprint": t["footprint"],
                          "bitstream_id": t["base"][0], "kernel_id": t["base"][1], "run_id": t["base"][2],
                          "tv_cycles": tv["value"], "a1_cycles": a1metric["value"], "delta_cycles": delta,
                          "nwalk": nw["value"], "cycles_per_walk": delta / nw["value"],
                          **{prefix + k: v for prefix, metric in [("tv_", tv), ("a1_", a1metric), ("nwalk_", nw)]
                             for k, v in metric.items() if k != "value"}})
    counts = Counter((b["ds"], b["boot_id"]) for b in blocks)
    require(sorted(counts.values()) == [5, 5, 10, 10, 10], "unexpected bootstrap strata")
    return list(trials.values()), blocks, pairs


def arrays(samples):
    return {k: np.asarray([s[k] for s in samples], dtype=float)
            for k in ["ds", "nwalk", "delta_cycles", "a1_cycles", "tv_cycles"]}


def delay_axis(ds, step_corrected=False):
    return ds + 2 * (ds > 0) if step_corrected else ds


def coefficients(a, normalized=False, step_corrected=False):
    n, ds, y = a["nwalk"], delay_axis(a["ds"], step_corrected), a["delta_cycles"]
    x = np.column_stack([np.ones_like(ds), ds]) if normalized else np.column_stack([n, n * ds])
    target = y / n if normalized else y
    require(np.linalg.matrix_rank(x) == 2, "rank deficient fit")
    return np.linalg.lstsq(x, target, rcond=None)[0]


def errors(actual, predicted):
    residual = actual - predicted
    denominator = np.sum((actual - actual.mean()) ** 2)
    return {"n": len(actual), "centered_r2": float(1 - np.sum(residual ** 2) / denominator) if denominator > 0 else None,
            "rmse": float(np.sqrt(np.mean(residual ** 2))), "mae": float(np.mean(abs(residual))),
            "max_absolute_error": float(max(abs(residual))), "mean_residual": float(np.mean(residual)),
            "min_residual": float(min(residual)), "max_residual": float(max(residual)),
            "relative_rmse": float(np.sqrt(np.mean((residual / actual) ** 2))),
            "relative_mae": float(np.mean(abs(residual / actual))),
            "max_relative_error": float(max(abs(residual / actual)))}


def fit(samples, normalized=False, step_corrected=False):
    a = arrays(samples)
    beta = coefficients(a, normalized, step_corrected)
    prediction = a["nwalk"] * (beta[0] + beta[1] * delay_axis(a["ds"], step_corrected))
    return {"n": len(samples), "t0_cycles_per_walk": float(beta[0]), "k_cycles_per_walk_per_ds": float(beta[1]),
            "delta_time_errors": errors(a["delta_cycles"], prediction),
            "full_victim_time_errors": errors(a["tv_cycles"], a["a1_cycles"] + prediction)}, prediction


def resampling_indices(blocks, count=BOOTSTRAPS):
    """One shared draw of whole blocks for every footprint and both estimators."""
    rng = np.random.default_rng(SEED)
    strata = defaultdict(list)
    for i, b in enumerate(blocks):
        strata[b["ds"], b["boot_id"]].append(i)
    return np.concatenate([rng.choice(indices, size=(count, len(indices)), replace=True)
                           for _, indices in sorted(strata.items())], axis=1)


def bootstrap(pairs, blocks, indices, normalized=False, step_corrected=False):
    out = {}
    require([p["footprint"] for p in pairs] == list(FOOTPRINTS) * len(blocks), "block/footprint array alignment")
    a = arrays(pairs)
    for model in (*FOOTPRINTS, "pooled"):
        columns = list(range(3)) if model == "pooled" else [FOOTPRINTS.index(model)]
        n = a["nwalk"].reshape(-1, 3)[indices][:, :, columns].reshape(len(indices), -1)
        ds = delay_axis(a["ds"], step_corrected).reshape(-1, 3)[indices][:, :, columns].reshape(len(indices), -1)
        y = a["delta_cycles"].reshape(-1, 3)[indices][:, :, columns].reshape(len(indices), -1)
        w = np.ones_like(n) if normalized else n * n
        target = y / n
        s0, s1, s2 = w.sum(1), (w * ds).sum(1), (w * ds * ds).sum(1)
        z0, z1 = (w * target).sum(1), (w * ds * target).sum(1)
        determinant = s0 * s2 - s1 * s1
        require(bool(np.all(determinant > 0)), "rank deficient bootstrap")
        out[model] = np.column_stack([(s2 * z0 - s1 * z1) / determinant,
                                       (s0 * z1 - s1 * z0) / determinant])
    return out


def ci(x):
    lo, hi = np.quantile(x, [0.025, 0.975], method="linear", axis=0)
    return [float(lo), float(hi)] if np.ndim(lo) == 0 else np.stack([lo, hi], axis=-1).tolist()


def model_results(pairs, draws, levels, status, normalized=False, step_corrected=False):
    result = {}
    for name in (*FOOTPRINTS, "pooled"):
        samples = pairs if name == "pooled" else [p for p in pairs if p["footprint"] == name]
        f, _ = fit(samples, normalized, step_corrected)
        f["coefficient_ci95"] = {"t0": ci(draws[name][:, 0]), "k": ci(draws[name][:, 1])}
        f["apparent_eta"] = {"value": f["k_cycles_per_walk_per_ds"] / (2 * levels),
                    "ci95": ci(draws[name][:, 1] / (2 * levels)), "levels": levels,
                    "levels_status": status, "formula": "k/(2*L)",
                    "interpretation": "apparent conditional slope ratio with software-window walks; not a measured blocking fraction/probability"}
        result[name] = f
    return result


def holdouts(pairs, normalized=False, step_corrected=False):
    """Exclude a complete cell or DS level, never a single repeated trial."""
    folds, detailed, summaries = [], [], {}
    definitions = []
    for footprint in FOOTPRINTS:
        for ds in LEVELS:
            definitions.append(("per_footprint_cell", footprint, ds))
    for footprint in FOOTPRINTS:
        for ds in LEVELS:
            definitions.append(("pooled_cell", footprint, ds))
    for ds in LEVELS:
        definitions.append(("pooled_ds", "all", ds))
    for family, footprint, ds in definitions:
        universe = [p for p in pairs if family != "per_footprint_cell" or p["footprint"] == footprint]
        test = [p for p in universe if p["ds"] == ds and (family == "pooled_ds" or p["footprint"] == footprint)]
        test_ids = {p["sample_id"] for p in test}
        train = [p for p in universe if p["sample_id"] not in test_ids]
        require(len(test) == (30 if family == "pooled_ds" else 10), "wrong holdout size")
        if family == "pooled_ds":
            require(not any(p["ds"] == ds for p in train), "DS holdout leakage")
        elif family == "pooled_cell":
            require(len({(p["ds"], p["footprint"]) for p in train}) == 11, "cell holdout leakage")
        else:
            require(len(train) == 30 and len({p["ds"] for p in train}) == 3, "footprint holdout leakage")
        model, _ = fit(train, normalized, step_corrected)
        beta = model["t0_cycles_per_walk"], model["k_cycles_per_walk_per_ds"]
        a = arrays(test)
        pred = a["nwalk"] * (beta[0] + beta[1] * delay_axis(a["ds"], step_corrected))
        fold_id = f"{family}:{footprint}:DS{ds}"
        folds.append({"fold_id": fold_id, "family": family, "held_out_ds": ds, "held_out_footprint": footprint,
                      "n_train": len(train), "n_test": len(test), "t0": beta[0], "k": beta[1],
                      "actual_delta_median": float(np.median(a["delta_cycles"])), "prediction_delta_median": float(np.median(pred)),
                      "actual_delta_mean": float(np.mean(a["delta_cycles"])), "prediction_delta_mean": float(np.mean(pred)),
                      "delta_time_errors": errors(a["delta_cycles"], pred),
                      "full_victim_time_errors": errors(a["tv_cycles"], a["a1_cycles"] + pred),
                      "training_sample_ids": [p["sample_id"] for p in train],
                      "test_sample_ids": [p["sample_id"] for p in test]})
        for p, prediction in zip(test, pred):
            detailed.append({"fold_id": fold_id, "family": family, "sample_id": p["sample_id"],
                             "footprint": p["footprint"], "ds": ds, "n_train": len(train),
                             "t0": beta[0], "k": beta[1], "delta_cycles": p["delta_cycles"],
                             "a1_cycles": p["a1_cycles"], "tv_cycles": p["tv_cycles"],
                             "prediction_cycles": float(prediction), "residual_cycles": p["delta_cycles"] - float(prediction)})
    for family in ["per_footprint_cell", "pooled_cell", "pooled_ds"]:
        s = [x for x in detailed if x["family"] == family]
        actual = np.asarray([x["delta_cycles"] for x in s]);pred = np.asarray([x["prediction_cycles"] for x in s])
        tv = np.asarray([x["tv_cycles"] for x in s]);a1 = np.asarray([x["a1_cycles"] for x in s])
        summaries[family] = {"delta_time_errors": errors(actual, pred), "full_victim_time_errors": errors(tv, a1 + pred)}
    return folds, detailed, summaries


def cell_summaries(pairs, models):
    rows = []
    for footprint in FOOTPRINTS:
        for ds in LEVELS:
            s = [p for p in pairs if p["footprint"] == footprint and p["ds"] == ds]
            a = arrays(s)
            row = {"footprint": footprint, "ds": ds, "n": len(s)}
            for k in ["delta_cycles", "tv_cycles", "a1_cycles", "nwalk"]:
                row[k + "_median"] = float(np.median(a[k]))
                row[k + "_q1"], row[k + "_q3"] = np.quantile(a[k], [.25, .75]).tolist()
            for name in [footprint, "pooled"]:
                model = models[name]
                prefix = "footprint" if name == footprint else "pooled"
                row[prefix + "_prediction_median"] = float(np.median(a["nwalk"] * (model["t0_cycles_per_walk"] + model["k_cycles_per_walk_per_ds"] * ds)))
            rows.append(row)
    return rows


def extrapolate(pairs, models, draws, step_corrected=False):
    rows = []
    for footprint in FOOTPRINTS:
        a = arrays([p for p in pairs if p["footprint"] == footprint and p["ds"] == 0])
        kinds = ["physical_memory_increment"] + (["diagnostic_ds_configuration"] if step_corrected else [])
        interpretations = ["total_extra_per_page_table_access", "extra_in_each_direction"]
        for kind, interpretation, ns, name in product(kinds, interpretations, [50, 100, 200], [footprint, "pooled"]):
            ds = ns / CYCLE_NS / (2 if interpretation == "total_extra_per_page_table_access" else 1)
            m = models[name]
            effective_ds = ds + (2 if kind == "diagnostic_ds_configuration" and ds > 0 else 0)
            cost = m["t0_cycles_per_walk"] + m["k_cycles_per_walk_per_ds"] * effective_ds
            cost_ci = ci(draws[name][:, 0] + draws[name][:, 1] * effective_ds)
            increment = a["nwalk"] * m["k_cycles_per_walk_per_ds"] * effective_ds
            rows.append({"projection_kind": kind, "footprint": footprint, "model": name, "interpretation": interpretation,
                         "input_extra_ns": ns, "total_access_extra_ns": ds * 2 * CYCLE_NS,
                         "ds_each_direction": ds, "model_delay_axis": effective_ds,
                         "fixed_nwalk_source": "observed DS0 per-block walk counts",
                         "nwalk_median": float(np.median(a["nwalk"])), "effective_cycles_per_walk": cost,
                         "effective_cycles_per_walk_ci95_lo": cost_ci[0], "effective_cycles_per_walk_ci95_hi": cost_ci[1],
                         "additional_delta_cycles_median_vs_model_DS0": float(np.median(increment)),
                         "model_DS0_delta_cycles_median": float(np.median(a["nwalk"] * m["t0_cycles_per_walk"])),
                         "observed_DS0_delta_cycles_median": float(np.median(a["delta_cycles"])),
                         "delta_cycles_median": float(np.median(a["nwalk"] * cost)),
                         "tv_cycles_median": float(np.median(a["a1_cycles"] + a["nwalk"] * cost)),
                         "observed_DS0_anchored_delta_cycles_median": float(np.median(a["delta_cycles"] + increment)),
                         "observed_DS0_anchored_tv_cycles_median": float(np.median(a["tv_cycles"] + increment)),
                         "conditional_projection_only": True})
    return rows


def write_csv(path, rows):
    buf = io.StringIO(newline="")
    writer = csv.DictWriter(buf, fieldnames=list(rows[0]), lineterminator="\n")
    writer.writeheader();writer.writerows(rows)
    path.write_text(buf.getvalue())


def plots(output, pairs, models, draws):
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.lines import Line2D
    plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 8, "axes.titlesize": 8,
                         "axes.labelsize": 8, "xtick.labelsize": 8, "ytick.labelsize": 8,
                         "legend.fontsize": 8, "axes.formatter.use_mathtext": False,
                         "axes.formatter.useoffset": False, "pdf.fonttype": 42, "ps.fonttype": 42,
                         "savefig.dpi": 300, "axes.linewidth": .7})
    colors = {"A8": "#0072B2", "A16": "#D55E00", "A64": "#009E73"}
    markers = {"A8": "o", "A16": "s", "A64": "^"}
    styles = {"A8": "-", "A16": "--", "A64": "-."}
    fig, axes = plt.subplots(3, 1, figsize=(3.5, 5.3), sharex=True)
    for ax, footprint in zip(axes, FOOTPRINTS):
        measured, predicted, pooled, lows, highs = [], [], [], [], []
        for ds in LEVELS:
            s = [p for p in pairs if p["footprint"] == footprint and p["ds"] == ds]
            a = arrays(s)
            ax.scatter(ds + np.linspace(-1.3, 1.3, len(s)), a["delta_cycles"] / 50000,
                       marker=markers[footprint], s=14, facecolors="none", edgecolors=colors[footprint], linewidths=.7, zorder=3)
            measured.append(float(np.median(a["delta_cycles"])) / 50000)
            for name, target in [(footprint, predicted), ("pooled", pooled)]:
                m = models[name]
                target.append(float(np.median(a["nwalk"] * (m["t0_cycles_per_walk"] + m["k_cycles_per_walk_per_ds"] * ds))) / 50000)
            interval = ci(np.median(a["nwalk"]) * (draws[footprint][:, 0] + draws[footprint][:, 1] * ds) / 50000)
            lows.append(interval[0]);highs.append(interval[1])
        ax.plot(LEVELS, predicted, color=colors[footprint], linestyle=styles[footprint], linewidth=1.1)
        ax.plot(LEVELS, pooled, color=".25", linestyle=":", linewidth=1)
        ax.fill_between(LEVELS, lows, highs, color=colors[footprint], alpha=.2)
        ax.set_title(footprint + " (40 matched pairs)", loc="left", pad=3)
        ax.grid(axis="y", color=".87", linewidth=.5)
        ax.ticklabel_format(axis="y", style="plain", useOffset=False)
        ax.set_xlim(-4, 104)
    axes[1].set_ylabel("Extra victim time vs A1 (ms)")
    axes[-1].set_xlabel("DS (cycles in each direction)")
    axes[-1].set_xticks(LEVELS)
    legend = [Line2D([], [], marker="o", markerfacecolor="none", color=".25", linestyle="none", markersize=4, label="Measured trials"),
              Line2D([], [], color=".25", label="Footprint fit"),
              Line2D([], [], color=".25", linestyle=":", label="Pooled fit")]
    fig.legend(handles=legend, loc="upper center", ncol=2, frameon=False, bbox_to_anchor=(.52, 1.005), columnspacing=1)
    fig.subplots_adjust(left=.18, right=.98, top=.91, bottom=.1, hspace=.35)
    stems = ["interference-model-measured-predicted", "interference-model-per-walk"]
    directory = output / "figures";directory.mkdir(exist_ok=True)
    for suffix in ["pdf", "png"]:
        fig.savefig(directory / (stems[0] + "." + suffix), metadata={"CreationDate": None, "ModDate": None} if suffix == "pdf" else None)
    plt.close(fig)
    fig, ax = plt.subplots(figsize=(3.5, 3.1))
    for footprint in FOOTPRINTS:
        s = [p for p in pairs if p["footprint"] == footprint]
        a = arrays(s);model = models[footprint]
        ax.scatter(a["ds"] + np.tile(np.linspace(-1.3, 1.3, 10), 4), a["delta_cycles"] / a["nwalk"],
                   marker=markers[footprint], s=15, facecolors="none", edgecolors=colors[footprint], linewidths=.7)
        x = np.asarray(LEVELS)
        ax.plot(x, model["t0_cycles_per_walk"] + model["k_cycles_per_walk_per_ds"] * x,
                color=colors[footprint], linestyle=styles[footprint], label=footprint, linewidth=1.1)
    model = models["pooled"]
    ax.plot(LEVELS, model["t0_cycles_per_walk"] + model["k_cycles_per_walk_per_ds"] * np.asarray(LEVELS), color="black", linestyle=":", label="Pooled", linewidth=1.2)
    ax.set(xlabel="DS (cycles in each direction)", ylabel="Extra victim cycles / observed A walk", xlim=(-4, 104), xticks=LEVELS)
    ax.grid(axis="y", color=".87", linewidth=.5)
    ax.ticklabel_format(axis="y", style="plain", useOffset=False)
    ax.legend(loc="upper left", frameon=False, ncol=2, columnspacing=.8, handlelength=2.1)
    fig.subplots_adjust(left=.19, right=.98, top=.97, bottom=.18)
    for suffix in ["pdf", "png"]:
        fig.savefig(directory / (stems[1] + "." + suffix), metadata={"CreationDate": None, "ModDate": None} if suffix == "pdf" else None)
    plt.close(fig)
    return {"files": [f"figures/{stem}.{ext}" for stem in stems for ext in ["pdf", "png"]],
            "matplotlib_version": matplotlib.__version__,
            "width_inches": 3.5, "font_size_points": 8, "scientific_mathtext": False,
            "all_120_matched_pairs_shown": True, "horizontal_jitter_ds_cycles": 1.3,
            "figure1": "Observed trial DeltaT with footprint-specific and pooled cycles-OLS predictions at observed DS/Nwalk; connecting lines guide the eye. Shading is parameter-only conditional 95% bootstrap CI, too narrow to resolve at some points; not prediction intervals.",
            "figure2": "Observed DeltaT/Nwalk; lines are cycles-domain fits, not normalized-OLS fits. Different markers/line styles retain grayscale distinction.",
            "limitations": "Plots use empirical non-atomic HPM counts; fitted cost is not a direct isolated PTW latency."}


def self_test(rows):
    trials, blocks, pairs = extract(rows)
    fake = [{**p, "delta_cycles": p["nwalk"] * (91 + 4.5 * p["ds"]),
             "tv_cycles": p["a1_cycles"] + p["nwalk"] * (91 + 4.5 * p["ds"])} for p in pairs]
    for normalized in [False, True]:
        b = coefficients(arrays(fake), normalized)
        require(np.allclose(b, [91, 4.5], rtol=1e-12, atol=1e-10), "known OLS test")
    corrected_fake = [{**p, "delta_cycles": p["nwalk"] * (91 + 4.5 * (p["ds"] + 2 * (p["ds"] > 0))),
                       "tv_cycles": p["a1_cycles"] + p["nwalk"] * (91 + 4.5 * (p["ds"] + 2 * (p["ds"] > 0)))} for p in pairs]
    require(np.allclose(coefficients(arrays(corrected_fake), step_corrected=True), [91, 4.5], atol=1e-10), "known corrected OLS test")
    idx = resampling_indices(blocks, 100)
    require(np.array_equal(idx, resampling_indices(blocks, 100)), "seed reproducibility")
    for draws in [bootstrap(fake, blocks, idx), bootstrap(fake, blocks, idx, True)]:
        require(all(np.allclose(x, [91, 4.5], atol=1e-10) for x in draws.values()), "known bootstrap fit")
    selected = next(i for i, r in enumerate(rows) if r["experiment"] == "board_baseline100" and r["metric"] == "hardware_cycles" and r["statistic"] == "sample")
    fixtures = []
    fixtures.append(("duplicate row ID", rows + [rows[selected]]))
    fixtures.append(("missing sample", rows[:selected] + rows[selected + 1:]))
    for label, field, value in [("mixed base", "bitstream_id", "program_file_sha256:" + "f" * 64),
                                ("mixed boot", "boot_id", "fixture-other-boot"),
                                ("summary mistaken for trial", "n", "10")]:
        changed = copy.deepcopy(rows);changed[selected][field] = value;fixtures.append((label, changed))
    for label, field, value in [("whole-DS wrong BIT", "bitstream_id", "program_file_sha256:" + EXPECTED_BIT_SHA256[100]),
                                ("whole-DS different kernel", "kernel_id", "fixture-other-kernel")]:
        changed = copy.deepcopy(rows)
        for row in changed:
            if row["experiment"] in {"board_baseline100", "board_ds_sweep"} and "ds=25/25;" in row["condition"]:
                row[field] = value
        fixtures.append((label, changed))
    for label, fixture in fixtures:
        try:
            extract(fixture)
        except ValueError:
            continue
        raise AssertionError("did not reject " + label)
    folds, detail, _ = holdouts(pairs)
    require(len(folds) == 28 and len(detail) == 360, "whole-cell / whole-DS holdout count")
    require(all(not (set(f["training_sample_ids"]) & set(f["test_sample_ids"])) for f in folds), "sample leakage")
    require(50 / CYCLE_NS / 2 == 1.25 and 200 / CYCLE_NS / 2 == 5, "total-access delay conversion")
    fake_models = {name: {"t0_cycles_per_walk": 91, "k_cycles_per_walk_per_ds": 4.5} for name in [*FOOTPRINTS, "pooled"]}
    fake_draws = {name: np.tile([91, 4.5], (100, 1)) for name in fake_models}
    for row in extrapolate(pairs, fake_models, fake_draws, step_corrected=True):
        step = 2 if row["projection_kind"] == "diagnostic_ds_configuration" else 0
        require(row["model_delay_axis"] == row["ds_each_direction"] + step, "physical vs diagnostic projection separation")
        expected = row["nwalk_median"] * 4.5 * (row["ds_each_direction"] + step)
        require(abs(row["additional_delta_cycles_median_vs_model_DS0"] - expected) < 1e-8, "marginal increment arithmetic")
    return {"status": "passed", "checks": ["known exact cycles, normalized and positive-DS step OLS", "known block bootstrap and fixed seed",
             "duplicate ID rejection", "missing sample rejection", "mixed base rejection", "mixed boot rejection",
             "whole-DS wrong BIT rejection", "whole-DS different kernel rejection",
             "summary-is-not-trial rejection", "28 whole-cell/DS folds and 360 held-out predictions", "50/100/200ns total-access conversion",
             "physical marginal increment excludes diagnostic FSM step"]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input-csv", type=Path, default=PAPER / "data/board_results.csv")
    parser.add_argument("--output-dir", type=Path, default=PAPER / "analysis")
    parser.add_argument("--levels", type=int, default=3)
    parser.add_argument("--levels-status", choices=["pending", "confirmed"], default="pending")
    parser.add_argument("--self-test", action="store_true")
    args = parser.parse_args()
    require(args.levels > 0, "levels must be positive")
    with args.input_csv.open(newline="") as f:
        rows = list(csv.DictReader(f))
    validation = self_test(rows) if args.self_test else {"status": "not_run", "command_flag": "--self-test"}
    trials, blocks, pairs = extract(rows)
    indices = resampling_indices(blocks)
    draws = bootstrap(pairs, blocks, indices)
    sensitivity_draws = bootstrap(pairs, blocks, indices, True)
    corrected_draws = bootstrap(pairs, blocks, indices, step_corrected=True)
    primary = model_results(pairs, draws, args.levels, args.levels_status)
    sensitivity = model_results(pairs, sensitivity_draws, args.levels, args.levels_status, True)
    corrected = model_results(pairs, corrected_draws, args.levels, args.levels_status, step_corrected=True)
    variants = [("cycles_ols", primary, draws, False, False),
                ("per_walk_normalized_ols", sensitivity, sensitivity_draws, True, False),
                ("positive_ds_step_corrected", corrected, corrected_draws, False, True)]
    folds, detailed, holdout_summary = [], [], {}
    for estimator, _, _, normalized, step in variants:
        vf, vd, vs = holdouts(pairs, normalized, step)
        for row in vf + vd:
            row["estimator"] = estimator
            row["fold_id"] = estimator + ":" + row["fold_id"]
        folds.extend(vf);detailed.extend(vd);holdout_summary[estimator] = vs
    for p in pairs:
        for estimator, models, _, _, step in variants:
            for name in [p["footprint"], "pooled"]:
                model = models[name]
                ds_axis = p["ds"] + (2 if step and p["ds"] > 0 else 0)
                prediction = p["nwalk"] * (model["t0_cycles_per_walk"] + model["k_cycles_per_walk_per_ds"] * ds_axis)
                prefix = estimator + ("_footprint" if name == p["footprint"] else "_pooled")
                p[prefix + "_prediction_cycles"] = prediction
                p[prefix + "_residual_cycles"] = p["delta_cycles"] - prediction
    out = args.output_dir;out.mkdir(exist_ok=True, parents=True)
    write_csv(out / "interference-model-samples.csv", pairs)
    trial_rows = []
    for t in sorted(trials, key=lambda t: (t["ds"], t["block"], MODES.index(t["footprint"]))):
        trial_rows.append({"ds": t["ds"], "boot_id": t["boot_id"], "block": t["block"], "condition": t["footprint"],
                           "bitstream_id": t["base"][0], "kernel_id": t["base"][1], "run_id": t["base"][2],
                           **{m: t["metrics"][m]["value"] for m in METRICS},
                           **{m + "_row_id": t["metrics"][m]["row_id"] for m in METRICS}})
    write_csv(out / "interference-model-trials.csv", trial_rows)
    cells = cell_summaries(pairs, primary)
    write_csv(out / "interference-model-cells.csv", cells)
    write_csv(out / "interference-model-loo-samples.csv", detailed)
    loo_rows = []
    for fold in folds:
        row = {k: v for k, v in fold.items() if k not in {"delta_time_errors", "full_victim_time_errors", "training_sample_ids", "test_sample_ids"}}
        row.update({scope + "_" + k: v for scope in ["delta_time_errors", "full_victim_time_errors"] for k, v in fold[scope].items()})
        loo_rows.append(row)
    write_csv(out / "interference-model-loo.csv", loo_rows)
    projections = []
    for estimator, models, mapping, _, step in variants:
        for row in extrapolate(pairs, models, mapping, step):
            row["estimator"] = estimator
            projections.append(row)
    write_csv(out / "interference-model-projections.csv", projections)
    figures = plots(out, pairs, primary, draws)
    strata = Counter((b["ds"], b["boot_id"]) for b in blocks)
    result = {"schema": "interference-model-v1", "generator": "analysis/scripts/interference_model.py",
              "python_version": sys.version.split()[0], "numpy_version": np.__version__,
              "generator_sha256": digest(__file__), "input_csv": str(args.input_csv.resolve()), "input_csv_sha256": digest(args.input_csv),
              "input_scalar_rows": len(rows), "selected_scalar_rows": 600, "trials": 200, "blocks": 40, "matched_pairs": 120,
              "pairs_per_footprint": 40, "clock_hz": CLOCK_HZ, "cycle_ns": CYCLE_NS,
              "identity_contract": {"expected_bit_sha256_by_ds": EXPECTED_BIT_SHA256, "expected_unified_kernel": EXPECTED_KERNEL,
                  "source": "accepted baseline100 and DS-sweep load/build receipts; program-file identity, not FPGA configuration readback",
                  "scope": "fixed recorded 200-trial dataset; distinct intentional DS BITs must not be substituted"},
              "model": "DeltaT = Nwalk * (t0 + k*DS); X=[Nwalk,Nwalk*DS]; no additional intercept",
              "response": "matched hardware_cycles(Ak)-hardware_cycles(A1), same DS/boot/block/BIT/kernel/run",
              "nwalk": {"metric": "aggressor_s1_ptw", "unit": "count", "raw_pointer_pattern": "/i/record/result/aggressor/s1_ptw",
                        "window": "non_atomic_HPM_software_snapshot: before A start; after V completion, CPU discovery and STOP A, before drain",
                        "hardware_time_pointer_pattern": "/i/observer/time/cycles",
                        "hardware_time_window": "hardware_V_batch", "A1_walks_all_zero": True,
                        "victim_walk_count_distribution": dict(sorted(Counter(t['metrics']['victim_s1_ptw']['value'] for t in trials).items()))},
              "primary_estimator": "unweighted OLS on cycle-domain residuals, all 120 measured pairs retained",
              "sensitivity_estimator": "unweighted OLS of DeltaT/Nwalk on [1,DS]; equivalent cycle residual weights 1/Nwalk**2",
              "relative_error_definition": "absolute(actual-predicted)/actual; delta and full-TV denominators are distinct; relative values are fractions",
              "r2_definition": "centered 1-SSE/sum((actual-mean(actual))**2), not uncentered no-intercept R2",
              "bootstrap": {"count": BOOTSTRAPS, "seed": SEED, "rng": "numpy.random.default_rng/PCG64",
                            "quantile": "Type 7 / numpy linear percentile", "unit": "complete DS/boot/block with all three Ak and shared A1",
                            "strata": [{"ds": ds, "boot_id": boot, "n_blocks": n} for (ds, boot), n in sorted(strata.items())],
                            "scope": "conditional on recorded boots; not independent-boot confidence intervals",
                            "joint_coefficient_order": [model + ':' + coefficient for model in [*FOOTPRINTS, 'pooled'] for coefficient in ['t0', 'k']],
                            "joint_coefficient_covariance": {estimator: np.cov(np.concatenate([mapping[model] for model in [*FOOTPRINTS, 'pooled']], axis=1), rowvar=False).tolist()
                                                             for estimator, _, mapping, _, _ in variants},
                            "shared_indices_sha256": hashlib.sha256(indices.astype('<i8').tobytes()).hexdigest()},
              "levels": {"value": args.levels, "status": args.levels_status, "eta_formula": "k/(2*L); not inferred from k"},
              "primary": primary, "per_walk_normalized_sensitivity": sensitivity,
              "positive_ds_step_sensitivity": {"formula": "DeltaT=Nwalk*(t0+k*(DS+2*I(DS>0)))",
                  "scope": "source-supported first-VALID release for measured DS25/50/100: Idle load d, Valid countdown, Ready; d=1 has a special case and is not measured here. DS0 bypass. Conditional on no extra READY wait / starting from Idle.",
                  "models": corrected},
              "leave_out": {"estimators": [v[0] for v in variants], "folds": folds, "summary": holdout_summary,
                            "prediction_scope": "conditional validation using held-out measured Nsw; reconstructed full TV also uses held-out matched A1. These inputs are known after the run, not blind advance prediction or Nwalk extrapolation. DeltaT errors are primary.",
                            "pooled_cell_scope": "11 training cells; other footprints at held-out DS remain in training and share A1; not unseen-DS validation",
                            "pooled_ds_scope": "all three footprints and all ten replicates at one DS are withheld together"},
              "cells": cells, "projections": projections, "figures": figures, "validation": validation,
              "limitations": ["Observed HPM walks and victim hardware time use different snapshot windows; an empirical conditional fit, not isolated PTW latency.",
                              "Nwalk is an observed, endogenous covariate; it changes with DS and interference. Predictions condition on it, not an independent predictor of future Nwalk.",
                              "HPM includes A work outside the exact V hardware window. Apparent eta=k/(2L) is not an experimentally isolated blocking fraction, probability, or overlap efficiency.",
                              "Each DS uses its recorded BIT/boot configuration; within-boot repeats do not establish a boot-population CI.",
                              "High in-sample R2 does not establish mechanism, generalization to unmeasured workload or pure PTW causality.",
                              "50/100/200ns TOTAL added time per page-table access maps to DS=1.25/2.5/5 each direction at 50MHz. The per-direction interpretation maps to 2.5/5/10 and is exported separately.",
                              "Projection holds observed DS0 Nwalk fixed and retains measured DS0 A1; new delays can change walks, arbitration and baseline timing.",
                              "Physical memory projections use only the marginal k*roundtrip_ns/40, with no diagnostic FSM step, including when k comes from the corrected model. Model-DS0 and observed-DS0 anchored total DeltaT/TV are exported separately.",
                              "Diagnostic configuration projections are separate from physical memory increments. Fractional DS values are conditional model evaluations, not implemented stream_delay configurations; the positive-DS step is only mechanically verified at measured d>1 values."]}
    numeric_files = ['samples', 'trials', 'cells', 'loo-samples', 'loo', 'projections']
    result["output_sha256"] = {f'interference-model-{name}.csv': digest(out/f'interference-model-{name}.csv') for name in numeric_files}
    result["output_sha256"].update({p: digest(out/p) for p in figures["files"]})
    (out / "interference-model-results.json").write_text(json.dumps(result, indent=2, ensure_ascii=False, allow_nan=False) + "\n")
    print(json.dumps({"status": "complete", "trials": 200, "pairs": 120, "bootstrap": BOOTSTRAPS, "levels": result["levels"],
                      "primary": {k: {x: v[x] for x in ['t0_cycles_per_walk', 'k_cycles_per_walk_per_ds']} for k, v in primary.items()},
                      "output": str(out.resolve())}, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
