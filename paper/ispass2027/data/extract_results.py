#!/usr/bin/env python3
"""Build source-bound ISPASS tables without changing historical evidence.

Only the Python standard library is used. source_locator is an RFC 6901 JSON
pointer, or a JSON expression with explicit source operands for derived ratios.
"""
from __future__ import annotations

import argparse
import collections
import csv
import hashlib
import io
import json
import math
from pathlib import Path
import re
import statistics

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
OUT = ROOT / "iommu-platform/out"
FIELDS = ["row_id", "experiment", "condition", "metric", "statistic", "value",
          "unit", "n", "bitstream_id", "kernel_id", "raw_path", "source_locator",
          "evidence_level", "value_status", "identity_status", "boot_id", "sample_set"]
STATS = ("min", "median", "max", "q1", "q3", "iqr")


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def encoded(value):
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True) + "\n"


def pointer(*parts):
    return "/" + "/".join(str(p).replace("~", "~0").replace("/", "~1") for p in parts)


def locate(obj, ptr):
    if ptr == "":
        return obj
    assert ptr.startswith("/"), ptr
    for part in ptr[1:].split("/"):
        key = part.replace("~1", "/").replace("~0", "~")
        obj = obj[int(key)] if isinstance(obj, list) else obj[key]
    return obj


def slug(text):
    return re.sub(r"[^a-zA-Z0-9_.-]+", "-", str(text)).strip("-")


def unit(metric):
    low = metric.lower()
    if "mib_s" in low:
        return "MiB/s"
    if "percent" in low or "pct" in low:
        return "%"
    if "ratio" in low or "slowdown" in low or "over_m0" in low:
        return "ratio"
    if low.endswith("_ms") or "_ms_" in low:
        return "ms"
    if low.endswith("_ns") or "_ns_" in low:
        return "ns"
    if "bytes" in low:
        return "bytes"
    if low == "cq_entry_cycles":
        return "command_simulation_steps"
    if low in {"t_a", "t_b", "t_end", "a_final_flush_end", "cq_total_waiting"}:
        return "simulation_steps"
    if "cycle" in low or low in {"w", "a_remaining", "v_remaining", "v_wait", "v_own"}:
        return "cycles"
    return "count"


def measurement_window(experiment, metric, condition):
    """Keep the window explicit even when one condition supplies many metrics."""
    window = None
    if experiment in {"e1", "batch_main", "batch_ext", "batch_ds0", "batch_ds50_retest", "path"}:
        if metric.endswith(("iotlb_miss", "s1_ptw", "s2_ptw", "active_cycles")):
            window = "non_atomic_HPM_software_snapshot"
        elif metric in {"software_ms", "victim_elapsed_ms", "paired_slowdown"}:
            window = "software_task"
        elif "done_to_read" in metric:
            window = "hardware_done_to_software_read"
        elif "refill_gap" in metric:
            window = "software_completion_to_resubmission"
    if experiment == "e2":
        if metric.startswith("p1_"):
            window = "A_legacy_phase_counter"
        elif metric.startswith("p2_"):
            window = "V_probe_phase_counter"
    if experiment == "p4m":
        if metric.endswith("whole_miss"):
            window = "non_atomic_HPM_software_snapshot"
        elif metric == "a_whole_completed":
            window = "whole_A_including_tail_after_V"
        elif metric == "done_to_read_ns":
            window = "hardware_done_to_software_read"
        elif metric in {"irq_span_ns", "a_to_v_ns"}:
            window = "software_startup"
    if window:
        condition = re.sub(r"window=[^;]*", "window="+window, condition)
    return condition


class Extraction:
    def __init__(self):
        self.cache = {}
        self.sources = {}
        self.board = []
        self.sim = []
        self.checks = []

    def source(self, path, kind="review_json", expected=None):
        path = Path(path)
        if not path.is_absolute():
            path = ROOT / path
        path = path.resolve()
        rel = path.relative_to(ROOT).as_posix()
        assert path.is_file(), rel
        current = digest(path)
        record = self.sources.setdefault(rel, {"path": rel, "sha256": current,
                                              "bytes": path.stat().st_size,
                                              "kind": kind})
        assert record["sha256"] == current, f"Source changed during extraction: {rel}"
        if expected:
            assert current == expected, f"Historical source hash mismatch: {rel}"
            record["historical_expected_sha256"] = expected
            record["historical_hash_match"] = True
        return rel

    def load(self, path, kind="review_json", expected=None):
        resolved = Path(path)
        if not resolved.is_absolute():
            resolved = ROOT / resolved
        rel = resolved.resolve().relative_to(ROOT).as_posix()
        if rel in self.cache:
            if expected:
                assert self.sources[rel]["sha256"] == expected, f"Historical source hash mismatch: {rel}"
                self.sources[rel]["historical_expected_sha256"] = expected
                self.sources[rel]["historical_hash_match"] = True
            return rel, self.cache[rel]
        rel = self.source(path, kind, expected)
        if rel not in self.cache:
            self.cache[rel] = json.loads((ROOT / rel).read_text())
        return rel, self.cache[rel]

    def row(self, experiment, condition, metric, statistic, path, ptr, n=1,
            identity=None, row_key=None, status=None, table="board", units=None,
            sample_set="formal", boot="unknown"):
        identity = identity or {}
        rel, data = self.load(path)
        val = locate(data, ptr)
        assert val is None or (isinstance(val, (int, float)) and not isinstance(val, bool)), (rel, ptr, val)
        rid = ".".join(map(slug, [experiment, row_key or condition, metric, statistic]))
        target = self.board if table == "board" else self.sim
        target.append(dict(zip(FIELDS, [rid, experiment, measurement_window(experiment, metric, condition), metric, statistic,
            "unknown" if val is None else val, units or unit(metric), n,
            identity.get("bitstream_id", "unknown"), identity.get("kernel_id", "unknown"),
            rel, ptr, "fpga_board" if table == "board" else "rtl_simulation_behavioral_ddr",
            status or ("unknown" if val is None else "observed"),
            identity.get("identity_status", "loaded_hash_unknown"), boot, sample_set])))
        return target[-1]

    def compare(self, name, actual, expected):
        okay = actual == expected or (isinstance(actual, (int, float)) and
            isinstance(expected, (int, float)) and math.isclose(actual, expected, rel_tol=1e-12, abs_tol=1e-10))
        assert okay, (name, actual, expected)
        self.checks.append({"check": name, "passed": True})

    def aggregate_row(self, experiment, condition, metric, statistic, path, ptrs,
                      identity, row_key, sample_set="formal", boot="unknown"):
        rel, data = self.load(path)
        vals = [locate(data, p) for p in ptrs]
        assert all(isinstance(v, (int, float)) and not isinstance(v, bool) for v in vals)
        value = {"min": min, "median": statistics.median, "max": max}[statistic](vals)
        row = self.row(experiment, condition, metric, statistic, rel, ptrs[0], len(ptrs),
                       identity, row_key, sample_set=sample_set, boot=boot)
        row["value"] = value
        row["value_status"] = "derived"
        row["source_locator"] = json.dumps({"op": statistic, "sources": [
            {"raw_path": rel, "pointer": p} for p in ptrs]}, sort_keys=True)

    def metadata(self, directory, filename="summary.json"):
        rel, data = self.load(OUT / directory / filename, "historical_run_metadata")
        run = data.get("run", data)
        bit = run.get("bit", {})
        sha = bit.get("expected_sha256") or bit.get("sha256")
        kernel = run.get("observed_linux_version") or run.get("linux_version")
        if not kernel:
            observed = re.findall(r"Linux version (\S+)", data.get("raw_serial", ""))
            kernel = observed[-1] if observed else "unknown"
        return {"bitstream_id": "expected_sha256:" + sha if sha else "unknown",
                "kernel_id": kernel, "identity_status": "loaded_hash_unknown",
                "metadata_path": rel}

    def hash_companions(self, directory, expected):
        for name, sha in expected.items():
            self.source(OUT / directory / name, "historical_raw_or_assessment", sha)


def add_e1(x):
    directory = "mac-e1-stage-20260916-032650-run-rbo67pvd"
    path, review = x.load(OUT / directory / "server-review.json")
    x.hash_companions(directory, review["source_sha256"])
    identity = x.metadata(directory)
    _, raw = x.load(OUT / directory / "records.json", "historical_raw_json")
    x.compare("E1 records including E0/sensitivity", len(raw), 144)
    formal = raw[64:]
    x.compare("E1 formal samples", len(formal), 80)
    for i, g in enumerate(review["groups"]):
        dev, pages = g["device"], g["aggressor_pages"]
        mode = "Solo" if pages == 0 else f"A{pages}"
        cond = f"role=V{dev}-A{1-dev};ds=50/50;mode={mode};window=software_task;boot=unknown"
        key = f"v{dev}-{mode}"
        subset = [r for r in formal if r["input"]["device"] == f"/dev/idma{dev}" and
                  (0 if r["input"]["aggressor_id"] is None else r["input"]["aggressor_pages"]) == pages]
        x.compare(f"E1 {key} n", len(subset), g["n"])
        x.compare(f"E1 {key} raw median elapsed", statistics.median(r["result"]["victim_elapsed_ns"]/1e6 for r in subset), g["victim_elapsed_ms_median"])
        for metric in ("victim_elapsed_ms_median", "paired_slowdown_median"):
            x.row("e1", cond, metric.removesuffix("_median"), "median", path,
                  pointer("groups", i, metric), g["n"], identity, key)
        for metric, stats in g["victim"].items():
            for stat in ("min", "median", "max"):
                vals = [r["result"]["victim"][metric] for r in subset]
                calc = {"min": min, "median": statistics.median, "max": max}[stat](vals)
                x.compare(f"E1 {key} raw {metric}/{stat}", calc, stats[stat])
                x.row("e1", cond, "victim_"+metric, stat, path,
                      pointer("groups", i, "victim", metric, stat), g["n"], identity, key)
        if pages:
            indices = [j for j, r in enumerate(raw) if j >= 64 and
                       r["input"]["device"] == f"/dev/idma{dev}" and
                       r["input"]["aggressor_id"] is not None and r["input"]["aggressor_pages"] == pages]
            for metric in ("iotlb_miss", "s1_ptw", "completed", "active_cycles"):
                ptrs = [pointer(j, "result", "aggressor", metric) for j in indices]
                for stat in ("min", "median", "max"):
                    x.aggregate_row("e1", cond, "aggressor_"+metric, stat,
                                    OUT / directory / "records.json", ptrs, identity, key)


def add_batch(x):
    configs = [
        ("batch_main", "mac-batch-main-20260916-112954-run-9sa69rhc", "main", 60),
        ("batch_ext", "mac-batch-ext-20260916-114036-run-8pjv_djc", "ext", 30),
        ("batch_ds0", "mac-batch-ds0-20260916-132332-run-ebgxhirr", "ds0", 60),
        ("batch_ds50_retest", "mac-batch-ds50-20260916-135544-run-eawziww2", "ds50", 30)]
    for experiment, directory, label, count in configs:
        review_dir = configs[0][1] if label == "ext" else directory
        path, data = x.load(OUT / review_dir / "server-review.json")
        run = data["runs"][label]
        assert run["passed"]
        x.hash_companions(directory, run["source_sha256"])
        identity = x.metadata(directory)
        _, records = x.load(OUT / directory / "records.json", "historical_raw_json")
        x.compare(experiment+" count", len(records), count)
        ds = "0/0" if label == "ds0" else "50/50"
        for i, g in enumerate(run["groups"]):
            dev = int(g["device"][-1]); mode = g["mode"]
            key = f"v{dev}-{mode}"
            cond = f"role=V{dev}-A{1-dev};ds={ds};mode={mode};window=hardware_V_batch;boot=unknown"
            subset = [r for r in records if r["record"]["input"]["device"] == g["device"] and
                      ("Solo" if r["record"]["result"]["aggressor"] is None else "A"+str(r["record"]["input"]["aggressor_pages"])) == mode]
            x.compare(experiment+key+" n", len(subset), g["samples"])
            x.compare(experiment+key+" raw hardware median", statistics.median(r["observer"]["time"]["cycles"]/50000 for r in subset), g["median"]["hardware_ms"])
            x.compare(experiment+key+" raw overlap median", statistics.median(r["observer"]["axi_v"]["both"]*100/r["observer"]["time"]["cycles"] for r in subset), g["median"]["overlap_percent"])
            for stat in ("min", "median", "max"):
                for metric in g[stat]:
                    # Absence of A is not a measured zero A counter.
                    if mode == "Solo" and metric.startswith("aggressor_"):
                        continue
                    x.row(experiment, cond, metric, stat, path,
                          pointer("runs", label, "groups", i, stat, metric), g["samples"], identity, key)


def add_path(x):
    directory = "mac-path-main-20260916-164633-run-vvguros8"
    path, data = x.load(OUT / directory / "server-review.json")
    run = data["runs"]["main"]
    assert run["passed"]
    x.hash_companions(directory, run["source_sha256"])
    identity = x.metadata(directory)
    for mode, metrics in run["groups"].items():
        cond = f"role=V0-A1;ds=50/50;mode={mode};window=hardware_V_batch;boot=unknown"
        for metric, stats in metrics.items():
            if not (metric.endswith("_mean_cycles") or metric in {
                    "hardware_ms", "hardware_MiB_s", "software_ms", "done_to_read_ms",
                    "overlap_percent", "v_iotlb_miss", "a_iotlb_miss", "v_s1_ptw", "a_s1_ptw"}):
                continue
            if mode == "Solo" and metric.startswith("a_"):
                continue
            for stat in ("min", "median", "max"):
                x.row("path", cond, metric, stat, path,
                      pointer("runs", "main", "groups", mode, metric, stat), 10, identity, mode)


def add_e3(x):
    path, data = x.load(OUT / "e3-controlled-review-20260918/server-review.json")
    assert data["exit_code"] == 0
    for record in data["inputs"].values():
        x.source(record["path"], "historical_E3_input", record["sha256"])
    identity = x.metadata("mac-e3-controlled-20260918-164635-run-rk48bhte", "run.json")
    for i, pair in enumerate(data["pairs"]):
        key = f"{pair['phase']}-case{pair['case']}"
        cond = f"role=V0-A1;ds=50/50;case={pair['case']};prep={pair['expected']['prep']};pair={pair['baseline']['kind']};window=controlled64B;boot=unknown"
        sample = "formal" if pair["phase"] == "main" else "short_acceptance"
        if pair["eligibility"]["triggered_rounds"] == 2:
            if pair["baseline"]["kind"] == "d128_minus_d0":
                high = next(r for r in pair["rounds"] if r["delay"] == 128)
                low = next(r for r in pair["rounds"] if r["delay"] == 0)
            else:
                low, high = pair["rounds"]
            for metric in ("A_remaining", "V_remaining", "V_wait", "V_own"):
                x.compare(f"E3 {key} paired {metric}", high["readings"][metric]-low["readings"][metric], pair["baseline"][metric])
            x.compare(f"E3 {key} paired complete hardware cycles", high["v_hw_cycles"]-low["v_hw_cycles"], pair["baseline"]["v_hw_cycles_delta"])
        for metric in ("A_remaining", "V_remaining", "V_wait", "V_own", "v_hw_cycles_delta"):
            val = pair["baseline"][metric]
            x.row("e3v6", cond, "delta_"+metric, "paired_difference", path,
                  pointer("pairs", i, "baseline", metric), 1, identity, key,
                  status="no_trigger" if val is None else "observed", sample_set=sample, units="cycles")
        for j, rnd in enumerate(pair["rounds"]):
            rk = key+f"-round{j}"
            rc = cond+f";round={j};delay_cfg={rnd['delay']}"
            for metric in ("trigger", "actual_added_cycles", "v_hw_cycles"):
                x.row("e3v6", rc, metric, "sample", path,
                      pointer("pairs", i, "rounds", j, metric), 1, identity, rk,
                      sample_set=sample)
            for metric in ("A_remaining", "V_remaining", "V_wait", "V_own",
                           "pre_wait_e4_minus_e69", "pre_a_service_e4_minus_e39"):
                if metric in rnd["readings"]:
                    x.row("e3v6", rc, metric, "sample", path,
                          pointer("pairs", i, "rounds", j, "readings", metric), 1,
                          identity, rk, sample_set=sample, units="cycles")


def add_e2(x):
    path, data = x.load(OUT / "e2-results-review-20261003/accepted-summary.json")
    x.compare("E2 accepted64 clean", data["cases_clean"], 64)
    b = data["board"]
    identity = {"bitstream_id": "declared_sha256:"+b["bit_sha256"][0],
                "kernel_id": b["kernel_release"][0], "identity_status": "loaded_hash_unknown"}
    boot = b["boot_id"][0]
    _, extract = x.load(OUT / "e2-results-review-20261003/verified-extract.json")
    for run in extract["runs"]:
        folder = OUT / "mac-research-two-bit-20261003/e2/runs" / run["id"]
        x.source(folder / "record.sha256", "historical_record_hash_receipt")
        original_sha = (folder / "record.sha256").read_text().split()[0]
        _, raw = x.load(folder / "record.json", "historical_raw_E2", original_sha)
        x.compare("E2 "+run["id"]+" probe stats match raw", raw["phase_stats"][2], run["probe_phase"])
    for name, group in data["groups"].items():
        cond = f"role=V0-A1;group={name};window=first_reuse_after_A;boot={boot}"
        sample = "short_acceptance" if name.startswith("short/") else "formal"
        for metric, stats in group["metrics"].items():
            for stat in ("min", "median", "max"):
                x.row("e2", cond, metric, stat, path,
                      pointer("groups", name, "metrics", metric, stat), stats["count"],
                      identity, name, sample_set=sample, boot=boot)
        for role, facts in group["first_probe_hit"].items():
            for metric in ("hits", "targets"):
                x.row("e2", cond+";target="+role, "first_probe_"+metric, "count", path,
                      pointer("groups", name, "first_probe_hit", role, metric), group["runs"],
                      identity, name+"-"+role, sample_set=sample, boot=boot)
        for classification in group["classification_counts"]:
            x.row("e2", cond+";observation_unit=translation_target;targets_per_trial=2",
                  classification, "target_count", path,
                  pointer("groups", name, "classification_counts", classification), group["runs"],
                  identity, name, sample_set=sample, boot=boot)
        x.compare(f"E2 {name} classification target counts cover two targets per trial",
                  sum(group["classification_counts"].values()), group["runs"]*2)


P4_METRICS = ("v_cycles", "hardware_window_ns", "v_mib_s", "a_window_done_jobs",
              "a_window_bytes", "a_window_mib_s", "a_flowboundary_jobs", "a_issue_window",
              "a_done_window", "v_whole_miss", "a_whole_miss", "a_whole_completed",
              "irq_span_ns", "a_to_v_ns", "done_to_read_ns")


def add_p4m(x):
    summary_path, data = x.load(OUT / "p4m-complete-review-20261002/summary.json")
    trial_path, trials = x.load(OUT / "p4m-complete-review-20261002/trials.json")
    x.compare("P4M trials60", len(trials), 60)
    identity = {"bitstream_id": "unknown", "kernel_id": "unknown",
                "identity_status": "loaded_hash_unknown"}
    kernels = {re.sub(r"^kernel_release=", "", s) for v in data["segment_metadata"].values()
               for s in v["params.txt"] if s.startswith("kernel_release=")}
    if len(kernels) == 1:
        identity["kernel_id"] = next(iter(kernels))
    new_boot_ids = {s.removeprefix("boot_id=") for s in data["segment_metadata"]["remaining29"]["params.txt"] if s.startswith("boot_id=")}
    assert len(new_boot_ids) == 1
    new_boot = next(iter(new_boot_ids))
    for i, trial in enumerate(trials):
        x.source(trial["json_path"], "historical_raw_P4M", trial["json_sha256"])
        boot = new_boot if trial["power_segment"] == "post_powercycle_newboot" else "unknown_pre_powercycle"
        cond = f"role=V0-A1;ds=50/50;gap={trial['gap']};acq={trial['acquisition_segment']};power={trial['power_segment']};block={trial['block']};window=common_V;boot={boot}"
        for metric in P4_METRICS:
            x.row("p4m", cond, metric, "sample", trial_path, pointer(i, "derived", metric),
                  1, identity, trial["case_id"], sample_set=trial["acquisition_segment"], boot=boot)
    # Summaries are separated by acquisition segment. We deliberately do not
    # import all60 or aggregate across the two boots.
    for acq, block in data["descriptive"]["by_acquisition_segment"].items():
        boot = new_boot if acq == "acq29_newboot" else "unknown_pre_powercycle"
        for gap, metrics in block["by_gap"].items():
            cond = f"role=V0-A1;ds=50/50;gap={gap};acq={acq};window=common_V;boot={boot}"
            key = f"{acq}-gap{gap}"
            for metric in P4_METRICS:
                stats = metrics[metric]
                vals = [t["derived"][metric] for t in trials if t["acquisition_segment"] == acq and t["gap"] == int(gap)]
                x.compare(f"P4M {key} {metric} median", statistics.median(vals), stats["median"])
                ordered = sorted(vals)
                quartiles = []
                for probability in (0.25, 0.75):
                    position = (len(ordered)-1)*probability
                    low, high = math.floor(position), math.ceil(position)
                    quartiles.append(ordered[low]*(1-(position-low))+ordered[high]*(position-low))
                computed = {"min": min(vals), "max": max(vals), "q1": quartiles[0],
                            "q3": quartiles[1], "iqr": quartiles[1]-quartiles[0]}
                for stat, value in computed.items():
                    x.compare(f"P4M {key} {metric} {stat}", value, stats[stat])
                for stat in STATS:
                    x.row("p4m", cond, metric, stat, summary_path,
                          pointer("descriptive", "by_acquisition_segment", acq, "by_gap", gap, metric, stat),
                          stats["n"], identity, key, sample_set=acq, boot=boot)
    # Keep every paired comparison separately, including cross-segment pairs.
    for i, pair in enumerate(data["pairs"]["rows"]):
        cond = f"role=V0-A1;gap={pair['gap']};block={pair['block']};reference={pair['gap0_case']};acq={pair['gap0_acquisition_segment']}->{pair['case_acquisition_segment']};power={pair['gap0_power_segment']}->{pair['case_power_segment']};window=common_V"
        for metric in ("v_time_decrease_pct", "v_rate_improvement_pct", "a_rate_loss_pct"):
            x.row("p4m", cond, metric, "paired_difference_percent", summary_path,
                  pointer("pairs", "rows", i, metric), 1, identity, pair["case"],
                  sample_set="paired_same_acq" if pair["same_acquisition_segment"] else "paired_cross_acq")


def add_composition(x):
    path, data = x.load(OUT / "composition-isolated-20261007/comparison.json")
    for i, inp in enumerate(data["inputs"]):
        side = "clean_history" if i == 0 else "isolated_history"
        for j, case in enumerate(inp["cases"]):
            directory = ROOT / "iommu-platform" / inp["run_dir"] / "cases" / case["id"]
            _, meta = x.load(directory / "meta.json", "historical_case_metadata")
            _, raw = x.load(directory / "record.json", "historical_raw_composition")
            words = raw["result"]["raw"]
            for metric, index in {"v_lookup": 12, "v_hit": 13, "v_miss": 14,
                                  "a_lookup": 15, "a_hit": 16, "a_miss": 17,
                                  "v_fast": 21, "a_fill": 27}.items():
                x.compare(f"composition {side} {case['id']} raw {metric}", words[index], case[metric])
            x.compare(f"composition {side} {case['id']} raw W", raw["result"]["p4m"]["window"][1], case["W"])
            x.compare(f"composition {side} {case['id']} raw A common-window completion", raw["result"]["p4m"]["window"][3], case["a_completed_window"])
            identity = {"kernel_id": meta["uname_r"], "bitstream_id": "unknown",
                        "identity_status": "loaded_hash_unknown"}
            boot = case["boot_id"]
            cond = f"role=V0-A1;side={side};mode={case['mode_name'].upper()};A_pages={case['a_pages']};block={case['block']};Vgap=0;window=common_V;boot={boot}"
            for metric in ("W", "a_completed_window", "a_fill", "a_hit", "a_lookup", "a_miss",
                           "v_fast", "v_hit", "v_lookup", "v_miss"):
                x.row("composition", cond, metric, "sample", path,
                      pointer("inputs", i, "cases", j, metric), 1, identity,
                      side+"-"+case["id"], boot=boot, sample_set=side)
        for j, pair in enumerate(inp["p_to_pn"]):
            cond = f"role=V0-A1;side={side};A_pages={pair['a_pages']};block={pair['block']};comparison=PN_over_P;window=common_V;boot={inp['boot_ids'][0]}"
            x.compare(f"composition {side} block{pair['block']} a{pair['a_pages']} ratio",
                      (pair["w_pn"]/pair["w_p"]-1)*100, pair["w_pct_change"])
            for metric in ("w_pct_change", "a_miss_diff"):
                x.row("composition", cond, metric, "paired_difference", path,
                      pointer("inputs", i, "p_to_pn", j, metric), 1, identity,
                      f"{side}-b{pair['block']}-a{pair['a_pages']}", boot=inp["boot_ids"][0], sample_set=side)


def add_invalidation(x):
    base = OUT / "invalidation-interference-20261017"
    _, validation = x.load(base / "analysis/validation.json")
    assert validation["status"] == "passed" and validation["accepted_cases"] == 28
    _, selected = x.load(base / "analysis/selected-inputs.json")
    x.source(base / "build/build03/build-manifest.json", "simulation_build_manifest")
    x.source(base / "frozen/source-manifest.json", "simulation_source_manifest")
    identity = {"bitstream_id": "not_applicable", "kernel_id": "not_executed",
                "identity_status": "frozen_source_manifest"}
    all_summaries = {}
    for record in selected["summaries"]:
        path, data = x.load(record["path"], "historical_raw_simulation_summary", record["sha256"])
        all_summaries[data["config"]["case_id"]] = (path, data)
    for key, (path, data) in sorted(all_summaries.items()):
        cfg = data["config"]
        assert data["checks"]["passed"]
        cond = f"domain={cfg['domain']};B_pages={cfg['b_pages']};mode={cfg['mode']};map_policy={cfg['map_policy']};A_jobs={cfg['a_jobs']};B_jobs={cfg['b_jobs']};window=complete_tasks"
        sample = "main24" if cfg["map_policy"] == "full-api" else "sensitivity4"
        for metric in data["metrics"]:
            x.row("invalidation_sim", cond, metric, "sample", path,
                  pointer("metrics", metric), 1, identity, key, table="sim", sample_set=sample,
                  boot="not_applicable")
        baseline_path, baseline = all_summaries[cfg["baseline_case_id"]]
        row = x.row("invalidation_sim", cond, "B_time_over_M0", "normalized_ratio", path,
                    pointer("metrics", "T_B"), 1, identity, key, table="sim",
                    sample_set=sample, boot="not_applicable", units="ratio")
        row["value"] = data["metrics"]["T_B"] / baseline["metrics"]["T_B"]
        row["value_status"] = "derived"
        row["source_locator"] = json.dumps({"op": "divide", "numerator": {"raw_path": path, "pointer": "/metrics/T_B"},
                                          "denominator": {"raw_path": baseline_path, "pointer": "/metrics/T_B"}}, sort_keys=True)


def add_incremental(x):
    config = HERE / "incremental_sources.json"
    if not config.exists():
        return
    _, data = x.load(config, "incremental_import_configuration")
    assert data["schema_version"] == 1
    for source in data.get("source_files", []):
        x.source(source["raw_path"], "incremental_import_source", source["sha256"])
    for spec in data["rows"]:
        needed = set(FIELDS)-{"value"}
        assert set(spec) == needed, ("Incremental fields", set(spec)^needed)
        assert spec["evidence_level"] == "fpga_board"
        path, obj = x.load(spec["raw_path"], "incremental_board_raw")
        row = dict(spec)
        row["raw_path"] = path
        value = locate(obj, spec["source_locator"])
        assert value is None or (isinstance(value, (int, float)) and not isinstance(value, bool))
        row["value"] = "unknown" if value is None else value
        if value is None:
            assert row["value_status"] in {"unknown", "no_trigger", "not_applicable"}
        x.board.append(row)


def csv_text(rows):
    output = io.StringIO(newline="")
    writer = csv.DictWriter(output, FIELDS, lineterminator="\n")
    writer.writeheader()
    writer.writerows(sorted(rows, key=lambda row: row["row_id"]))
    return output.getvalue()


def build():
    x = Extraction()
    for fn in (add_e1, add_batch, add_path, add_e3, add_e2, add_p4m, add_composition,
               add_invalidation, add_incremental):
        fn(x)
    x.compare("No blank numeric cells", all(r["value"] != "" for r in x.board+x.sim), True)
    x.compare("Historical E3 untriggered causal fields remain unknown",
              sum(r["experiment"] == "e3v6" and r["value"] == "unknown" and r["value_status"] == "no_trigger" for r in x.board), 16)
    x.compare("CQ residence area unit is command times simulation steps",
              {r["unit"] for r in x.sim if r["metric"] == "cq_entry_cycles"}, {"command_simulation_steps"})
    x.compare("Simulation completion times retain simulation steps",
              {r["unit"] for r in x.sim if r["metric"] in {"T_A", "T_B", "T_end", "A_final_flush_end", "cq_total_waiting"}}, {"simulation_steps"})
    x.compare("Board and simulation evidence levels are separate",
              all(r["evidence_level"] == "fpga_board" for r in x.board) and all(r["evidence_level"] == "rtl_simulation_behavioral_ddr" for r in x.sim), True)
    x.compare("Composition retains two distinct boots",
              len({r["boot_id"] for r in x.board if r["experiment"] == "composition"}), 2)
    x.compare("P4M samples retain three acquisition segments",
              len({r["sample_set"] for r in x.board if r["experiment"] == "p4m" and r["statistic"] == "sample"}), 3)
    for table in (x.board, x.sim):
        ids = [r["row_id"] for r in table]
        assert len(ids) == len(set(ids)), "Duplicate row_id"
        for row in table:
            if row["source_locator"].startswith("{"):
                expr = json.loads(row["source_locator"])
                if expr["op"] == "divide":
                    vals = [locate(x.cache[expr[k]["raw_path"]], expr[k]["pointer"]) for k in ("numerator", "denominator")]
                    actual = vals[0]/vals[1]
                else:
                    vals = [locate(x.cache[r["raw_path"]], r["pointer"]) for r in expr["sources"]]
                    actual = {"min": min, "median": statistics.median, "max": max}[expr["op"]](vals)
                x.compare(row["row_id"]+" source expression", actual, row["value"])
            else:
                v = locate(x.cache[row["raw_path"]], row["source_locator"])
                x.compare(row["row_id"]+" source pointer", "unknown" if v is None else v, row["value"])
    outputs = {"board_results.csv": csv_text(x.board), "invalidation_sim_results.csv": csv_text(x.sim)}
    key_groups = {
        "E1_software_slowdown": [r for r in x.board if r["experiment"] == "e1" and r["metric"] in {"paired_slowdown", "victim_elapsed_ms", "victim_iotlb_miss"} and r["statistic"] == "median"],
        "batch_hardware_and_overlap": [r for r in x.board if r["experiment"].startswith("batch_") and r["metric"] in {"hardware_ms", "hardware_MiB_s", "overlap_percent"} and r["statistic"] == "median"],
        "path_request_stage_means": [r for r in x.board if r["experiment"] == "path" and r["metric"].startswith("v_") and r["metric"].endswith("_mean_cycles") and r["statistic"] == "median"],
        "E3_controlled_pairs": [r for r in x.board if r["experiment"] == "e3v6" and r["statistic"] == "paired_difference"],
        "E2_boundary": [r for r in x.board if r["experiment"] == "e2" and "boundary/w" in r["condition"] and r["metric"] in {"d8d_v_miss", "d89_v_main_cycles"} and r["statistic"] == "median"],
        "P4M_separate_acquisition_segments": [r for r in x.board if r["experiment"] == "p4m" and r["statistic"] == "median" and r["metric"] in {"v_cycles", "v_mib_s", "a_window_mib_s", "a_window_done_jobs"}],
        "composition_separate_histories_and_boots": [r for r in x.board if r["experiment"] == "composition" and r["metric"] in {"W", "a_miss", "v_miss", "w_pct_change", "a_miss_diff"}],
        "invalidation_simulation_only": [r for r in x.sim if r["metric"] in {"T_A", "T_B", "A_final_flush_end", "B_time_over_M0", "B_invalidation_attributed_misses"}]
    }
    outputs["key_rows.json"] = encoded({"schema_version": 1, "groups": {
        group: [{k: r[k] for k in ("row_id", "condition", "metric", "statistic", "value", "unit", "n", "value_status")} for r in sorted(rows, key=lambda row: row["row_id"])]
        for group, rows in key_groups.items()}})
    # Large incremental imports can reuse a source thousands of times. Read
    # each JSON once, then recheck every source before publishing the manifest.
    for source in x.sources.values():
        assert digest(ROOT / source["path"]) == source["sha256"], f"Source changed during extraction: {source['path']}"
    manifest = {"schema_version": 1, "generator": "paper/ispass2027/data/extract_results.py",
                "historical_sources_are_read_only": True, "source_count": len(x.sources),
                "sources": [x.sources[k] for k in sorted(x.sources)]}
    outputs["source_manifest.json"] = encoded(manifest)
    validation = {"schema_version": 1, "status": "passed",
                  "scope": "Deterministic extraction and source-bound calculations; no hardware rerun or renewed experimental acceptance",
                  "board_rows": len(x.board), "simulation_rows": len(x.sim),
                  "board_experiments": dict(sorted(collections.Counter(r["experiment"] for r in x.board).items())),
                  "null_rows_by_status": dict(sorted(collections.Counter(r["value_status"] for r in x.board if r["value"] == "unknown").items())),
                  "source_count": len(x.sources), "source_pointer_and_calculation_checks": len(x.checks),
                  "all_historical_expected_hashes_match": True,
                  "output_sha256": {k: hashlib.sha256(v.encode()).hexdigest() for k, v in outputs.items()},
                  "generator_sha256": digest(Path(__file__)),
                  "checks": x.checks}
    outputs["validation.json"] = encoded(validation)
    return outputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="Compare deterministic reconstruction without writing files")
    args = parser.parse_args()
    outputs = build()
    if args.check:
        for name, content in outputs.items():
            assert (HERE / name).read_bytes() == content.encode(), f"Rebuild differs: {name}"
        saved = json.loads((HERE / "source_manifest.json").read_text())
        for record in saved["sources"]:
            assert digest(ROOT / record["path"]) == record["sha256"], record["path"]
    else:
        for name, content in outputs.items():
            (HERE / name).write_text(content)
    print(json.dumps({"status": "passed", "mode": "check" if args.check else "build",
                      "outputs": list(outputs)}, ensure_ascii=False))


if __name__ == "__main__":
    main()
