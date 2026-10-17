#!/usr/bin/env python3
"""Replay HPM-window and path-observer arithmetic for the accepted 200 trials.

This is an observer audit, not a fit.  Event-count bounds refer to increments
actually registered by HPM, not an independently calibrated count of all walks.
No narrow boundary correction is inferred from extra A copies or elapsed time.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import math
from collections import Counter, defaultdict
from pathlib import Path
from statistics import median


DS_LEVELS = (0, 25, 50, 100)
CONDITIONS = ("Solo", "A1", "A8", "A16", "A64")
STAGES = tuple(f"v{channel}{stage}" for channel in "rw" for stage in range(4))
BURSTS_PER_CHANNEL = 8192
CLOCK_NS = 20  # Configuration-based conversion, not an independent clock test.
EXPERIMENTS = {"board_baseline100", "board_ds_sweep"}
CSV_REL = "paper/ispass2027/data/board_results.csv"


def require(ok, message):
    if not ok:
        raise ValueError(message)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def summary(values):
    values = [value for value in values if value is not None]
    return {"n": len(values), "median": median(values),
            "min": min(values), "max": max(values)} if values else None


def validate_record(record, label):
    result = record["record"]["result"]
    obs = record["observer"]
    require(record["record"]["success"] and result["status"] == 0, f"{label}: failed trial")
    require(result["measurement_complete"] == 1, f"{label}: incomplete measurement")
    require(obs["time"]["issued"] == obs["time"]["done"] == 4096,
            f"{label}: incorrect task count")
    for channel in ("ar", "aw", "rlast", "b"):
        require(obs["axi_v"][channel] == BURSTS_PER_CHANNEL,
                f"{label}: V {channel} count mismatch")
    for name in STAGES:
        stage = record["path"][name]
        require(stage["valid"] == 1 and stage["full"] == stage["entered"] == BURSTS_PER_CHANNEL,
                f"{label}: {name} is not a complete burst-stage sample")
        require(stage["start"] == stage["carry"] == stage["end"] == 0,
                f"{label}: {name} has crossing stages")
        require(stage["area"] == stage["sum"], f"{label}: {name} area/sum mismatch")
        require(math.isclose(record["metrics"][f"path_{name}_mean_cycles"],
                             stage["sum"] / stage["full"], rel_tol=0, abs_tol=1e-10),
                f"{label}: {name} derived mean mismatch")


def load_samples(source_root):
    csv_path = source_root / CSV_REL
    indexed = {}
    raw_paths = set()
    with csv_path.open(newline="") as handle:
        for line, row in enumerate(csv.DictReader(handle), 2):
            if (row["experiment"] not in EXPERIMENTS or row["statistic"] != "sample"
                    or not row["raw_path"].endswith("/records.json")):
                continue
            index = int(row["source_locator"].split("/")[1])
            key = (row["raw_path"], index, row["metric"])
            require(key not in indexed, f"duplicate source row: {key}")
            indexed[key] = {**row, "csv_line": line}
            raw_paths.add(row["raw_path"])
    inputs = [{"path": CSV_REL, "sha256": digest(csv_path)}]
    samples = []
    for raw_path in sorted(raw_paths):
        path = source_root / raw_path
        records = json.loads(path.read_text())
        inputs.append({"path": raw_path, "sha256": digest(path), "trials": len(records)})
        for index, record in enumerate(records):
            label = f"{raw_path}:/{index}"
            validate_record(record, label)
            r, o = record["record"]["result"], record["observer"]
            refs = {}
            for metric in ("hardware_cycles", "software_ms", "victim_iotlb_miss", "victim_s1_ptw",
                           "aggressor_iotlb_miss", "aggressor_s1_ptw", "path_vr1_mean_cycles",
                           "path_vw1_mean_cycles", "aggressor_completed"):
                source = indexed.get((raw_path, index, metric))
                value = record["metrics"].get(metric)
                if value is None and metric == "aggressor_completed":
                    value = (r["aggressor"] or {}).get("completed")
                if value is None:
                    continue
                require(source is not None, f"{label}: missing CSV {metric}")
                require(math.isclose(float(source["value"]), value, rel_tol=1e-12, abs_tol=1e-9),
                        f"{label}: CSV/raw mismatch for {metric}")
                refs[metric] = {key: source[key] for key in
                                ("row_id", "raw_path", "source_locator", "csv_line", "value")}
            hw_ref = refs["hardware_cycles"]
            a = r["aggressor"]
            a_s1 = a["s1_ptw"] if a else None
            v_s1 = r["victim"]["s1_ptw"]
            # Each individual HPM reading brackets H.  The fields do not identify
            # which of those increments occurred outside H; never infer ±1/±2.
            sample = {
                "sample_id": hw_ref["row_id"].removesuffix(".hardware_cycles.sample"),
                "ds": int(record["ds"].split("/")[0]), "condition": record["condition"],
                "phase": path.parent.name, "block": record["block"], "boot_id": record["boot_id"],
                "hardware_cycles": o["time"]["cycles"], "software_elapsed_ns": r["victim_elapsed_ns"],
                "snapshot_before_ns": r["snapshot_before_ns"], "snapshot_after_ns": r["snapshot_after_ns"],
                "done_to_read_cycles": o["time"]["done_to_read"], "drain_ns": r["drain_ns"],
                "v_sw_iotlb_miss": r["victim"]["iotlb_miss"], "v_sw_s1_ptw": v_s1,
                "a_sw_iotlb_miss": a["iotlb_miss"] if a else None, "a_sw_s1_ptw": a_s1,
                "v_counted_s1_in_h_lower": 0, "v_counted_s1_in_h_upper": v_s1,
                "a_counted_s1_in_h_lower": 0 if a else None, "a_counted_s1_in_h_upper": a_s1,
                "a_whole_completed_copies": a["completed"] if a else None,
                "a_window_ar": o["axi_a"]["ar"], "a_window_aw": o["axi_a"]["aw"],
                "a_window_rlast": o["axi_a"]["rlast"], "a_window_b": o["axi_a"]["b"],
                "path_vr1_mean_cycles": record["path"]["vr1"]["sum"] / BURSTS_PER_CHANNEL,
                "path_vw1_mean_cycles": record["path"]["vw1"]["sum"] / BURSTS_PER_CHANNEL,
                "path_vr1_sum_cycles": record["path"]["vr1"]["sum"],
                "path_vw1_sum_cycles": record["path"]["vw1"]["sum"],
                "path_vr0_mean_cycles": record["path"]["vr0"]["sum"] / BURSTS_PER_CHANNEL,
                "path_vr2_mean_cycles": record["path"]["vr2"]["sum"] / BURSTS_PER_CHANNEL,
                "path_vw0_mean_cycles": record["path"]["vw0"]["sum"] / BURSTS_PER_CHANNEL,
                "path_vw2_mean_cycles": record["path"]["vw2"]["sum"] / BURSTS_PER_CHANNEL,
                "raw_path": raw_path, "raw_pointer": f"/{index}", "source_refs": refs,
                "raw_field_pointers": {
                    key: f"/{index}/{pointer}" for key, pointer in {
                        "snapshot_before_ns": "record/result/snapshot_before_ns",
                        "snapshot_after_ns": "record/result/snapshot_after_ns",
                        "drain_ns": "record/result/drain_ns", "done_to_read_cycles": "observer/time/done_to_read",
                        "path_vr1": "path/vr1", "path_vw1": "path/vw1",
                    }.items()
                },
            }
            sample["software_minus_hardware_ns_at_configured_clock"] = (
                sample["software_elapsed_ns"] - CLOCK_NS * sample["hardware_cycles"])
            sample["hpm_overhang_ns_at_configured_clock_lower"] = sample[
                "software_minus_hardware_ns_at_configured_clock"]
            sample["hpm_overhang_ns_at_configured_clock_upper"] = (
                sample["software_minus_hardware_ns_at_configured_clock"]
                + sample["snapshot_before_ns"] + sample["snapshot_after_ns"])
            require(sample["hpm_overhang_ns_at_configured_clock_lower"] >= 0,
                    f"{label}: clock/window comparison failed")
            sample["a_walk_sensitivity_not_error_bound"] = {
                str(offset): max(0, a_s1 + offset) for offset in (-2, -1, 0, 1, 2)
            } if a else None
            samples.append(sample)
    counts = Counter((s["ds"], s["condition"]) for s in samples)
    require(counts == Counter({(ds, c): 10 for ds in DS_LEVELS for c in CONDITIONS}),
            "expected exactly 4 DS × 5 conditions × 10 trials")
    require(len({s["sample_id"] for s in samples}) == 200, "duplicate sample identities")
    return samples, inputs


def add_pairs(samples):
    lookup = {}
    for sample in samples:
        key = (sample["raw_path"], sample["boot_id"], sample["block"], sample["condition"])
        require(key not in lookup, f"ambiguous same-block pair: {key}")
        lookup[key] = sample
    for sample in samples:
        a1_key = (sample["raw_path"], sample["boot_id"], sample["block"], "A1")
        require(a1_key in lookup, f"missing A1 in same phase/boot/block: {a1_key}")
        a1 = lookup[a1_key]
        sample["a1_sample_id"] = a1["sample_id"]
        sample["a1_raw_pointer"] = a1["raw_pointer"]
        sample["a1_source_refs"] = {k: a1["source_refs"][k] for k in
                                    ("hardware_cycles", "path_vr1_mean_cycles", "path_vw1_mean_cycles")}
        for name in ("hardware_cycles", "path_vr1_mean_cycles", "path_vw1_mean_cycles",
                     "path_vr1_sum_cycles", "path_vw1_sum_cycles"):
            sample[f"delta_{name}_vs_a1"] = sample[name] - a1[name]
        dt = sample["delta_hardware_cycles_vs_a1"]
        for channel in ("r", "w"):
            sample[f"{channel}1_delta_sum_over_delta_t"] = (
                sample[f"delta_path_v{channel}1_sum_cycles_vs_a1"] / dt
                if sample["condition"] in ("A8", "A16", "A64") and dt > 0 else None)
        sample["rw1_delta_sum_over_delta_t"] = (
            sample["r1_delta_sum_over_delta_t"] + sample["w1_delta_sum_over_delta_t"]
            if sample["r1_delta_sum_over_delta_t"] is not None else None)


def aggregate(samples):
    groups = []
    keys = [key for key, value in samples[0].items()
            if isinstance(value, (int, float)) and key not in ("ds", "block")]
    for key in ("a_sw_s1_ptw", "a_sw_iotlb_miss", "a_whole_completed_copies",
                "a_counted_s1_in_h_lower", "a_counted_s1_in_h_upper", "r1_delta_sum_over_delta_t",
                "w1_delta_sum_over_delta_t", "rw1_delta_sum_over_delta_t"):
        if key not in keys:
            keys.append(key)
    for ds in DS_LEVELS:
        for condition in CONDITIONS:
            selected = [s for s in samples if s["ds"] == ds and s["condition"] == condition]
            groups.append({"ds": ds, "condition": condition, "n": len(selected),
                           "boot_ids": sorted({s["boot_id"] for s in selected}),
                           "sample_ids": [s["sample_id"] for s in selected],
                           "v_sw_miss_frequency": dict(Counter(s["v_sw_iotlb_miss"] for s in selected)),
                           "statistics": {key: summary(s.get(key) for s in selected) for key in keys}})
    return groups


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-root", type=Path, default=Path(__file__).resolve().parents[4])
    parser.add_argument("--output-dir", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    samples, inputs = load_samples(args.source_root.resolve())
    add_pairs(samples)
    groups = aggregate(samples)
    same_counts = all(s["v_sw_iotlb_miss"] == s["v_sw_s1_ptw"] and
                      s["a_sw_iotlb_miss"] == s["a_sw_s1_ptw"] for s in samples)
    require(same_counts, "miss/S1 read values differ; review non-atomic snapshot interpretation")
    # Exercise a scientifically material rejection: pairing across missing phase
    # controls is forbidden even if an A1 in another boot/block happens to exist.
    negative = [s.copy() for s in samples if not (s["raw_path"] == samples[0]["raw_path"]
                and s["block"] == samples[0]["block"] and s["condition"] == "A1")]
    try:
        add_pairs(negative)
    except ValueError as error:
        require("missing A1" in str(error), "unexpected negative-check failure")
    else:
        raise ValueError("missing same-block A1 was silently accepted")
    document = {
        "schema": "ispass-interference-model-observers-v1", "inputs": inputs,
        "generator": {"path": "paper/ispass2027/analysis/scripts/interference_model_observers.py",
                      "sha256": digest(Path(__file__))},
        "scope": "accepted 200 trials only; offline arithmetic, no fit or new experiment",
        "configuration": {"clock_ns": CLOCK_NS, "copies": 4096,
                          "burst_bytes": 2048, "bursts_per_channel": BURSTS_PER_CHANNEL,
                          "address_bursts_read_plus_write": 2 * BURSTS_PER_CHANNEL},
        "definitions": {
            "strict_count_bounds": "0 <= counted S1 HPM increments inside V hardware window <= software-bracket S1 counter difference; counter semantics/monotonicity and ordered accesses assumed",
            "true_walks": "A count of all actual walks needs event-count calibration; the bounds above are on recorded increments, not missed events",
            "overhang_duration": "configured-clock conversion only; [Tsw-20*Thw, Tsw-20*Thw+snapshot_before+snapshot_after]; per-counter sample timestamps are absent",
            "sensitivity": "Nsw + [-2,-1,0,1,2] is an assumption sensitivity, never a measured error bound",
            "no_narrow_bound": "No narrow bound inferred from whole A copies, assumed hot tail, or post-translation boundary counters; complete upstream cross-boundary state is not audited",
            "path": "each trial sum/full of ingress-address handshake to first output VALID; queueing plus translation/buffering latency, not pure wait or PTW",
            "paired": "Ak minus A1 within exact raw file, boot and block, then group medians; DS50 pools two observed boots for description",
            "nonadditivity": "8192*delta(mean) is summed overlapping burst residence; it is not elapsed time or an additive causal partition",
            "independence": "path and HPM are distinct observer mechanisms on the same execution/boot, not independent experiments or per-request hit classifications",
        },
        "validation": {"accepted_trials": len(samples), "group_count": len(groups),
                       "csv_raw_agreement": True, "all_v_stages_full_8192_no_crossing": True,
                       "all_v_stage_area_equals_sum": True, "all_miss_s1_read_values_equal": same_counts,
                       "missing_same_block_a1_rejected": True},
        "groups": groups, "samples": samples,
    }
    args.output_dir.mkdir(parents=True, exist_ok=True)
    json_path = args.output_dir / "interference-model-observers.json"
    json_path.write_text(json.dumps(document, indent=2, ensure_ascii=False) + "\n")
    csv_path = args.output_dir / "interference-model-observers.csv"
    with csv_path.open("w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(samples[0]), lineterminator="\n")
        writer.writeheader()
        for sample in samples:
            writer.writerow({k: json.dumps(v, sort_keys=True, separators=(",", ":"))
                             if isinstance(v, dict) else v for k, v in sample.items()})
    print(json.dumps({"accepted_trials": len(samples), "groups": len(groups),
                      "validation": document["validation"],
                      "outputs": [{"path": str(p), "sha256": digest(p), "bytes": p.stat().st_size}
                                  for p in (json_path, csv_path)]}, indent=2))


if __name__ == "__main__":
    main()
