#!/usr/bin/env python3
"""Read the 100 saved trials; generate new offline tables without changing inputs.

Python standard library only. Quantiles use Type 7 linear interpolation. The CI
is the percentile interval for the trial median, with 10,000 bootstrap samples.
For DS50 pooled summaries, resample within each observed boot (5 + 5 trials).
"""

import argparse
from bisect import bisect_right
import csv
import hashlib
import io
import json
import math
from pathlib import Path
from statistics_common import summarize as summarize_values


ROOT = Path(__file__).resolve().parents[4]
BASE = Path("paper/ispass2027/board/runs/baseline100-20261017-01")
OUT = Path("paper/ispass2027/analysis")
PHASES = ("ds0-middle", "ds50-first", "ds50-last")
CONDITIONS = ("Solo", "A1", "A8", "A16", "A64")
SEED = 20261017
RESAMPLES = 10000
CLOCK_HZ = 50_000_000
BURST_BYTES = 2048  # Fixed-source 64-bit AXI, 256 beats; see report source audit.
SOURCE_AUDIT = OUT / "inputs/baseline100-source-audit.json"
UNITS = {
    "hardware_ms": "ms", "victim_MiB_s": "MiB/s",
    "aggressor_response_bytes": "bytes", "aggressor_MiB_s_proxy": "MiB/s",
    "total_MiB_s_proxy": "MiB/s", "total_over_solo_percent": "%",
    "victim_over_solo_percent": "%", "victim_over_half_solo_percent": "%",
    "time_over_solo": "ratio", "time_beyond_two_solo_ms": "ms",
    "aggressor_whole_batch_bytes": "bytes",
    "aggressor_stage3_full_response_bytes": "bytes",
    "aggressor_payload_lower_bytes": "bytes", "aggressor_payload_upper_bytes": "bytes",
    "total_MiB_s_payload_lower": "MiB/s", "total_MiB_s_payload_upper": "MiB/s",
    "total_over_solo_lower_percent": "%", "total_over_solo_upper_percent": "%",
    "victim_iotlb_miss_software_window": "count",
    "aggressor_iotlb_miss_software_window": "count",
    "aggregate_loss_vs_solo_percent": "%",
    "victim_shortfall_vs_half_solo_percent": "%",
}
for channel in "rw":
    for stage in range(4):
        UNITS[f"v{channel}{stage}_trial_mean_cycles"] = "cycles"


def pointer_lines(text):
    """Map JSON pointers to exact original value-start lines, including objects."""
    decoder = json.JSONDecoder()
    newlines = [i for i, char in enumerate(text) if char == "\n"]
    locations = {}

    def skip(pos):
        while pos < len(text) and text[pos].isspace():
            pos += 1
        return pos

    def walk(pos, pointer):
        pos = skip(pos)
        locations[pointer] = bisect_right(newlines, pos) + 1
        if text[pos] == "{":
            pos = skip(pos + 1)
            while text[pos] != "}":
                key, pos = decoder.raw_decode(text, pos)
                pos = skip(pos)
                assert text[pos] == ":"
                token = key.replace("~", "~0").replace("/", "~1")
                pos = skip(walk(pos + 1, pointer + "/" + token))
                if text[pos] == ",":
                    pos = skip(pos + 1)
                else:
                    assert text[pos] == "}"
            return pos + 1
        if text[pos] == "[":
            pos, index = skip(pos + 1), 0
            while text[pos] != "]":
                pos = skip(walk(pos, pointer + "/" + str(index)))
                index += 1
                if text[pos] == ",":
                    pos = skip(pos + 1)
                else:
                    assert text[pos] == "]"
            return pos + 1
        _, end = decoder.raw_decode(text, pos)
        return end

    assert skip(walk(0, "")) == len(text)
    return locations


def load_trials():
    inputs = {}

    def load(path):
        data = (ROOT / path).read_bytes()
        inputs[str(path)] = hashlib.sha256(data).hexdigest()
        return json.loads(data)

    source = load(Path("paper/ispass2027/board/packages/baseline100-20261017/source.json"))
    audit = load(SOURCE_AUDIT)
    assert all(f["commit"] == source["actual_commits"][f["repository"]] for f in audit["files"])
    table_path = Path("paper/ispass2027/data/board_results.csv")
    table_data = (ROOT / table_path).read_bytes()
    inputs[str(table_path)] = hashlib.sha256(table_data).hexdigest()
    scalar_sources = {}
    for row in csv.DictReader(io.StringIO(table_data.decode())):
        if row["row_id"].startswith("board_baseline100.") and row["statistic"] == "sample":
            scalar_sources[row["raw_path"], row["source_locator"]] = row
    trials = []
    manifests = {}
    for phase in PHASES:
        path = BASE / phase / "records.json"
        records = load(path)
        locations = pointer_lines((ROOT / path).read_text())
        manifest = manifests[phase] = load(BASE / phase / "manifest.json")
        loaded = load(BASE / (phase + "-load") / "load.json")
        assert loaded["status"] == "boot_identity_verified"
        assert loaded["bit_sha256"] == manifest["bit_sha256"]
        assert loaded["kernel_release"] == manifest["kernel_release"] == "6.5.0-rc2-ispass2027-pathv3"
        expected = 50 if phase == "ds0-middle" else 25
        assert manifest["status"] == "passed" and len(records) == expected
        assert len({r["boot_id"] for r in records}) == 1
        assert all(r["boot_id"] == loaded["boot_id"] for r in records)
        blocks = range(5, 10) if phase == "ds50-last" else range(expected // 5)
        assert {(r["block"], r["condition"]) for r in records} == {
            (block, condition) for block in blocks for condition in CONDITIONS}
        solos = {r["block"]: (i, r) for i, r in enumerate(records) if r["condition"] == "Solo"}
        for index, r in enumerate(records):
            result = r["record"]["result"]
            arguments = r["record"]["input"]
            time = r["observer"]["time"]
            v, a = r["observer"]["axi_v"], r["observer"]["axi_a"]
            assert r["ds"] == ("0/0" if phase == "ds0-middle" else "50/50")
            assert arguments["chunk_bytes"] == 4096 and arguments["victim_transfers"] == 4096
            assert arguments["warmup_transfers"] == 256
            assert result["status"] == 0 and r["record"]["success"]
            assert time["valid"] == 1 and time["issued"] == time["done"] == 4096
            assert time["cycles"] == time["h"] + time["gap"] and time["gap"] == 4095
            assert v["ar"] == v["aw"] == v["rlast"] == v["b"] == 8192
            assert v["start"] == v["end"] == 0
            assert result["victim"]["completed_bytes"] == 16 * 2**20
            for lane in (result["victim"], result["aggressor"]):
                if lane is not None:
                    assert lane["data_errors"] == lane["quarantined"] == 0
                    assert lane["submitted"] == lane["completed"]
            for lane in (v, a):
                assert (lane["start"] >> 16) + lane["ar"] == lane["rlast"] + (lane["end"] >> 16)
                assert (lane["start"] & 65535) + lane["aw"] == lane["b"] + (lane["end"] & 65535)
            is_solo = r["condition"] == "Solo"
            assert (result["aggressor"] is None) == is_solo
            if not is_solo:
                # These are observed properties, not invented completed descriptors.
                assert a["ar"] == a["aw"] == a["rlast"] == a["b"] == 8192
                assert a["start"] == a["end"] == (2 << 16) + 2
                assert result["aggressor"]["completed"] > 4096
            seconds = time["cycles"] / CLOCK_HZ
            victim_rate = result["victim"]["completed_bytes"] / 2**20 / seconds
            a_bytes = None if is_solo else a["b"] * BURST_BYTES
            total_rate = victim_rate if is_solo else victim_rate + a_bytes / 2**20 / seconds
            solo_index, solo = solos[r["block"]]
            solo_seconds = solo["observer"]["time"]["cycles"] / CLOCK_HZ
            solo_rate = solo["record"]["result"]["victim"]["completed_bytes"] / 2**20 / solo_seconds
            metrics = {
                "hardware_ms": time["cycles"] / 50000, "victim_MiB_s": victim_rate,
                "aggressor_response_bytes": a_bytes,
                "aggressor_MiB_s_proxy": None if is_solo else a_bytes / 2**20 / seconds,
                "total_MiB_s_proxy": total_rate,
                "total_over_solo_percent": total_rate / solo_rate * 100,
                "victim_over_solo_percent": victim_rate / solo_rate * 100,
                "victim_over_half_solo_percent": None if is_solo else victim_rate / (solo_rate / 2) * 100,
                "time_over_solo": seconds / solo_seconds,
                "time_beyond_two_solo_ms": None if is_solo else (seconds - 2 * solo_seconds) * 1000,
                "aggressor_whole_batch_bytes": None if is_solo else result["aggressor"]["completed_bytes"],
                "aggressor_stage3_full_response_bytes": None if is_solo else r["path"]["aw3"]["full"] * BURST_BYTES,
                # One 4KiB task at a time: outstanding W-byte/B-credit stock
                # at either boundary is in [0, 4096]. This is interface traffic,
                # conditional on legal AXI, not a physical DDR timing guarantee.
                "aggressor_payload_lower_bytes": None if is_solo else a_bytes - 4096,
                "aggressor_payload_upper_bytes": None if is_solo else a_bytes + 4096,
                "total_MiB_s_payload_lower": total_rate if is_solo else total_rate - 4096 / 2**20 / seconds,
                "total_MiB_s_payload_upper": total_rate if is_solo else total_rate + 4096 / 2**20 / seconds,
                "total_over_solo_lower_percent": (total_rate if is_solo else total_rate - 4096 / 2**20 / seconds) / solo_rate * 100,
                "total_over_solo_upper_percent": (total_rate if is_solo else total_rate + 4096 / 2**20 / seconds) / solo_rate * 100,
                "victim_iotlb_miss_software_window": result["victim"]["iotlb_miss"],
                "aggressor_iotlb_miss_software_window": None if is_solo else result["aggressor"]["iotlb_miss"],
                "aggregate_loss_vs_solo_percent": 100 - total_rate / solo_rate * 100,
                "victim_shortfall_vs_half_solo_percent": None if is_solo else 100 - victim_rate / (solo_rate / 2) * 100,
            }
            for channel in "rw":
                for stage in range(4):
                    key = f"v{channel}{stage}"
                    p = r["path"][key]
                    assert p["valid"] == 1 and p["entered"] == p["full"] == 8192
                    assert p["start"] == p["carry"] == p["end"] == 0
                    assert p["area"] == p["sum"]
                    value = p["sum"] / p["full"]
                    assert value == r["metrics"][f"path_{key}_mean_cycles"]
                    metrics[f"{key}_trial_mean_cycles"] = value
            assert math.isclose(metrics["hardware_ms"], r["metrics"]["hardware_ms"], abs_tol=1e-12)
            assert math.isclose(victim_rate, r["metrics"]["hardware_MiB_s"], abs_tol=1e-12)
            for table_metric, calculated in {
                "hardware_ms": metrics["hardware_ms"], "hardware_MiB_s": victim_rate,
                **{f"path_v{ch}{s}_mean_cycles": metrics[f"v{ch}{s}_trial_mean_cycles"]
                   for ch in "rw" for s in range(4)},
            }.items():
                row = scalar_sources[str(path), f"/{index}/metrics/{table_metric}"]
                assert math.isclose(float(row["value"]), calculated, rel_tol=1e-13), row["row_id"]
            trials.append({
                "phase": phase, "ds": r["ds"], "condition": r["condition"],
                "boot_id": r["boot_id"], "block": r["block"], "case": r["case"],
                "raw_path": str(path), "source_locator": f"/{index}",
                "sample_id": f"baseline100-extra.{phase}.case{r['case']:04d}",
                "raw_record_line": locations[f"/{index}"],
                "paired_solo_locator": f"/{solo_index}", "hardware_cycles": time["cycles"],
                "a_rlast": a["rlast"], "a_b": a["b"], "a_start": a["start"], "a_end": a["end"],
                "a_write_stage3_full": r["path"]["aw3"]["full"],
                "a_write_stage3_carry": r["path"]["aw3"]["carry"],
                **metrics,
                "source_operands": {
                    operand: dict(raw_path=str(path), pointer=f"/{idx}" + operand,
                                  line=locations[f"/{idx}" + operand],
                                  board_row_id=scalar_sources.get((str(path), f"/{idx}" + operand), {}).get("row_id"))
                    for idx, operand in [(index, p) for p in (
                        "/observer/time/cycles", "/observer/axi_a/b", "/observer/axi_a/rlast",
                        "/observer/axi_a/start", "/observer/axi_a/end",
                        "/record/result/victim/completed_bytes", "/record/result/victim/iotlb_miss",
                        "/record/result/aggressor" if is_solo else "/record/result/aggressor/completed_bytes",
                        *(() if is_solo else ("/record/result/aggressor/iotlb_miss", "/record/result/aggressor/completed")),
                        *(f"/path/v{ch}{s}/{field}" for ch in "rw" for s in range(4) for field in ("sum", "full")),
                        "/path/aw3/full")]
                },
                "paired_solo_source": dict(raw_path=str(path), pointer=f"/{solo_index}/observer/time/cycles",
                                           line=locations[f"/{solo_index}/observer/time/cycles"]),
            })
    assert len(trials) == 100 and len({t["boot_id"] for t in trials}) == 3
    assert manifests["ds50-first"]["bit_sha256"] == manifests["ds50-last"]["bit_sha256"]
    return trials, inputs, manifests, source


def summarize(trials):
    rows = []
    for scope in (*PHASES, "ds50-pooled"):
        for condition in CONDITIONS:
            selected = [t for t in trials if t["condition"] == condition and (
                t["phase"] == scope if scope != "ds50-pooled" else t["ds"] == "50/50")]
            strata = [t["boot_id"] for t in selected]
            group_seed = int.from_bytes(hashlib.sha256(f"{SEED}:{scope}:{condition}".encode()).digest()[:8], "big")
            for metric, unit in UNITS.items():
                values = [t[metric] for t in selected]
                known = [v for v in values if v is not None]
                assert not known or len(known) == len(values)
                if known:
                    summary = summarize_values(known, seed=group_seed, n_resamples=RESAMPLES, strata=strata)
                    numbers = {key: summary[key] for key in ("median", "q1", "q3", "iqr")}
                    numbers.update(minimum=summary["min"], maximum=summary["max"],
                                   ci95_low=summary["ci_low"], ci95_high=summary["ci_high"])
                else:
                    numbers = dict.fromkeys(("median", "q1", "q3", "iqr", "minimum", "maximum", "ci95_low", "ci95_high"))
                rows.append(dict(scope=scope, ds=selected[0]["ds"], condition=condition,
                                 result_id=f"baseline100-extra.{scope}.{condition}.{metric}",
                                 boot_ids=";".join(sorted({t["boot_id"] for t in selected})),
                                 n_trials=len(selected), n_values=len(known), metric=metric, unit=unit,
                                 **numbers, ci_target="trial_median", resamples=RESAMPLES,
                                 bootstrap="within_observed_boot", seed=SEED, group_seed=group_seed,
                                 contributing_sample_ids=[t["sample_id"] for t in selected],
                                 formula_id=metric,
                                 status="not_applicable" if not known else "observed_or_derived"))
    return rows


def tables(stats):
    index = {(r["scope"], r["condition"], r["metric"]): r for r in stats}
    throughput, paths, pooling = [], [], []
    for scope in (*PHASES, "ds50-pooled"):
        for condition in CONDITIONS:
            def med(metric):
                return index[scope, condition, metric]["median"]
            throughput.append(dict(scope=scope, ds=index[scope, condition, "hardware_ms"]["ds"],
                                   condition=condition, n=index[scope, condition, "hardware_ms"]["n_trials"],
                                   **{m: med(m) for m in list(UNITS)[:12]}))
            for channel in "rw":
                paths.append(dict(scope=scope, condition=condition, channel=channel,
                                  **{f"stage{s}_median_cycles": med(f"v{channel}{s}_trial_mean_cycles") for s in range(4)}))
    for condition in CONDITIONS:
        first = index["ds50-first", condition, "hardware_ms"]["median"]
        last = index["ds50-last", condition, "hardware_ms"]["median"]
        pooling.append(dict(condition=condition, first_median_ms=first, last_median_ms=last,
                            last_minus_first_ms=last-first, last_over_first_change_percent=(last/first-1)*100))
    return throughput, paths, pooling


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="Recompute and compare outputs without writing anything")
    parser.add_argument("--self-test", action="store_true", help="Exercise input-validation failures in a temporary copy")
    args = parser.parse_args()
    if args.self_test:
        self_test()
        return
    trials, inputs, manifests, source = load_trials()
    stats = summarize(trials)
    throughput, paths, pooling = tables(stats)
    formulas = metric_formulas()
    assert set(formulas) == set(UNITS)
    script = Path(__file__).resolve()
    outputs = {
        "baseline100-extra.json": json.dumps({
            "schema": "baseline100-extra-v2", "analysis_date": "2026-10-17", "seed": SEED,
            "resamples": RESAMPLES, "clock_hz": CLOCK_HZ, "burst_bytes": BURST_BYTES,
            "inputs_sha256": inputs,
            "generator_sha256": hashlib.sha256(script.read_bytes()).hexdigest(),
            "statistics_library_sha256": hashlib.sha256(script.with_name("statistics_common.py").read_bytes()).hexdigest(),
            "n_trials": len(trials), "n_groups": 20, "formulas": formulas,
            "source_audit": str(SOURCE_AUDIT), "actual_commits": source["actual_commits"],
            "bootstrap_scope": "trial medians, fixed boot strata; conditional on the observed boots",
            "samples": trials, "statistics": stats, "throughput": throughput,
            "path": paths, "boot_comparison": pooling,
        }, indent=2, allow_nan=False) + "\n",
    }
    # The narrative and machine-readable artifact share exactly the same rows.
    outputs["baseline100-extra.md"] = render_report(stats, throughput, paths, pooling, manifests, source)
    for relative, digest in inputs.items():
        assert hashlib.sha256((ROOT / relative).read_bytes()).hexdigest() == digest, relative
    if not args.check:
        (ROOT / OUT).mkdir(parents=True, exist_ok=True)
    for name, text in outputs.items():
        path = ROOT / OUT / name
        if args.check:
            assert path.read_bytes() == text.encode(), f"Reproduction mismatch: {path}"
        else:
            path.write_bytes(text.encode())
    print(f"{'VERIFIED' if args.check else 'GENERATED'}: 100 trials, 20 groups, {len(stats)} metric summaries; input bytes unchanged.")


def self_test():
    """Ensure malformed counts, incomplete paths and identity mismatches fail."""
    import tempfile
    from copy import deepcopy
    global ROOT
    original_root = ROOT
    _, inputs, _, _ = load_trials()
    records_path = BASE / "ds0-middle/records.json"
    load_path = BASE / "ds0-middle-load/load.json"
    records = json.loads((ROOT / records_path).read_text())
    loaded = json.loads((ROOT / load_path).read_text())
    shared_index = next(i for i, item in enumerate(records) if item["condition"] == "A1")
    tests = []
    changed = deepcopy(records)
    changed[shared_index]["observer"]["axi_a"]["b"] -= 1
    tests.append(("A response conservation", records_path, changed))
    changed = deepcopy(records)
    changed[0]["path"]["vr1"]["carry"] = 1
    tests.append(("V complete-stage coverage", records_path, changed))
    tests.append(("trial count", records_path, records[:-1]))
    tests.append(("BIT declaration identity", load_path, {**loaded, "bit_sha256": "0" * 64}))
    try:
        with tempfile.TemporaryDirectory(prefix="baseline100-extra-test-") as directory:
            ROOT = Path(directory)
            for relative in inputs:
                target = ROOT / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes((original_root / relative).read_bytes())
            for name, relative, altered in tests:
                target = ROOT / relative
                original = target.read_bytes()
                target.write_text(json.dumps(altered, indent=2) + "\n")
                try:
                    load_trials()
                except AssertionError:
                    pass
                else:
                    raise RuntimeError(f"Invalid input was accepted: {name}")
                finally:
                    target.write_bytes(original)
    finally:
        ROOT = original_root
    assert all(hashlib.sha256((ROOT / relative).read_bytes()).hexdigest() == digest
               for relative, digest in inputs.items())
    assert pointer_lines('{\n  "x": [\n    2,\n    {"y": "a\\nb"}\n  ]\n}\n')["/x/1/y"] == 4
    print("SELF-TEST PASS: 4 malformed-input cases rejected; escaped-JSON source-line mapping checked; original inputs unchanged.")


def metric_formulas():
    """Symbols point to exact operands saved for every contributing sample."""
    formulas = {
        "hardware_ms": "T=observer.time.cycles / 50000000 seconds; result=1000*T",
        "victim_MiB_s": "V=record.result.victim.completed_bytes; result=V/2**20/T",
        "aggressor_response_bytes": "C=2048*observer.axi_a.b; response-accounted COPY bytes, not full-window COPY count",
        "aggressor_MiB_s_proxy": "C/2**20/T",
        "total_MiB_s_proxy": "(V+C)/2**20/T; Solo uses C=0",
        "total_over_solo_percent": "100*total_MiB_s_proxy / paired_same_phase_same_block_Solo.victim_MiB_s",
        "victim_over_solo_percent": "100*victim_MiB_s / paired_same_phase_same_block_Solo.victim_MiB_s",
        "victim_over_half_solo_percent": "2*victim_over_solo_percent",
        "time_over_solo": "T/paired_same_phase_same_block_Solo.T",
        "time_beyond_two_solo_ms": "1000*(T-2*paired_same_phase_same_block_Solo.T)",
        "aggressor_whole_batch_bytes": "record.result.aggressor.completed_bytes; whole batch including pre/post window, never divided by T",
        "aggressor_stage3_full_response_bytes": "2048*path.aw3.full; only stage-3 endpoints are within window",
        "aggressor_payload_lower_bytes": "C-4096; S=C+D_end-D_start, 0<=D<=one_4096_byte_task",
        "aggressor_payload_upper_bytes": "C+4096; legal AXI and the fixed aligned COPY configuration assumed",
        "total_MiB_s_payload_lower": "(V+C-4096)/2**20/T; Solo uses V/2**20/T",
        "total_MiB_s_payload_upper": "(V+C+4096)/2**20/T; Solo uses V/2**20/T",
        "total_over_solo_lower_percent": "100*total_MiB_s_payload_lower/paired_Solo.victim_MiB_s",
        "total_over_solo_upper_percent": "100*total_MiB_s_payload_upper/paired_Solo.victim_MiB_s",
        "victim_iotlb_miss_software_window": "record.result.victim.iotlb_miss; non_atomic_HPM_software_window",
        "aggressor_iotlb_miss_software_window": "record.result.aggressor.iotlb_miss; non_atomic_HPM_software_window",
        "aggregate_loss_vs_solo_percent": "100-total_over_solo_percent; negative means observed aggregate exceeds Solo",
        "victim_shortfall_vs_half_solo_percent": "100-victim_over_half_solo_percent; negative means V exceeds half Solo",
    }
    for channel in "rw":
        for stage in range(4):
            formulas[f"v{channel}{stage}_trial_mean_cycles"] = f"path.v{channel}{stage}.sum / path.v{channel}{stage}.full; summarize across trials, not requests"
    return formulas


def render_report(stats, throughput, paths, pooling, manifests, source):
    index = {(r["scope"], r["condition"], r["metric"]): r for r in stats}
    labels = {"ds0-middle": "DS0", "ds50-first": "DS50 前 boot",
              "ds50-last": "DS50 后 boot", "ds50-pooled": "DS50 合并"}

    def med(scope, condition, metric):
        return index[scope, condition, metric]["median"]

    def fmt(value, digits=3):
        return "—" if value is None else f"{value:.{digits}f}"

    def table(headers, rows):
        return "\n".join(["| " + " | ".join(headers) + " |",
                          "| " + " | ".join("---" for _ in headers) + " |",
                          *("| " + " | ".join(map(str, row)) + " |" for row in rows)])

    a1_share = med("ds0-middle", "A1", "victim_over_solo_percent")
    a1_total = med("ds0-middle", "A1", "total_MiB_s_proxy")
    a1_total_ratio = med("ds0-middle", "A1", "total_over_solo_percent")
    a1_vs_half = med("ds0-middle", "A1", "victim_over_half_solo_percent")
    ds0_iqr = [index["ds0-middle", c, "hardware_ms"]["iqr"] for c in CONDITIONS]
    ds50_iqr = [index["ds50-pooled", c, "hardware_ms"]["iqr"] for c in CONDITIONS]
    lines = [
        "# 统一基线100次：合计吞吐、路径等待与离散度", "",
        "## Material Passport", "",
        "Origin Skill: academic-research-suite / experiment-agent；Mode: validate；Date: 2026-10-17（JST）；"
        "Verification Status: VERIFIED（仅离线重算，不表示重跑硬件）；Version Label: baseline100_extra_v2。", "",
        "仅使用本轮100个已接受试次；G1、旧板测、仿真均未混入。DS0 是原始直连硬件的论文主结果，"
        "DS50 是额外50/50延迟敏感性；两者均沿用本轮固定的原版加必要修复、批量供给与被动观测配置。", "",
        "100是完整矩阵的试次数，不是每条件的n：每DS×条件实际n=10。"
        "DS0来自一个boot；DS50前、后boot每条件各n=5，另显式报告合并n=10。"
        "[机器可读结果](baseline100-extra.json)的`statistics`含每格的稳定`result_id`、"
        "`contributing_sample_ids`与`formula_id`；`samples`保存逐操作数原始文件、JSON pointer、精确行号及已有标量行ID。"
        "所有统计在逐试次派生量上计算；不得用标量表行数作为n。", "",
        "硬件时间与绝对吞吐沿用采集器声明的50MHz时钟；原始语义标注calibration=unverified，"
        "本次离线分析没有追加物理时钟校准。", "",
        "## 1. 双方合计吞吐", "",
        f"80个共跑试次的 A 在 V 硬件窗口内均完成8192个写响应，按固定2048字节突发记账为16 MiB；"
        f"严格同窗 W 有效字节只能给出16 MiB ±4096字节的边界区间，不能把 A 整批排空后的字节数当成同窗量。"
        f"DS0 A1 的 V 保留 Solo 的{a1_share:.3f}%，比理想50%高{a1_share-50:.3f}个百分点，"
        f"相对半份带宽高{a1_vs_half-100:.3f}%，因此没有超出平分的额外损失；"
        f"合计约{a1_total:.3f} MiB/s、为同区组Solo的{a1_total_ratio:.3f}%，是接近平分且总量略增，并非严格守恒。"
        f"DS0 A8/A16/A64合计为Solo的"
        f"{med('ds0-middle','A8','total_over_solo_percent'):.3f}%/"
        f"{med('ds0-middle','A16','total_over_solo_percent'):.3f}%/"
        f"{med('ds0-middle','A64','total_over_solo_percent'):.3f}%，此时才出现低于Solo的总吞吐损失；"
        "DS50的相应损失更大，但不解释为纯PTW成本。", "",
        "以下均为逐试次计算后的中位数，COPY字节只记一次，不把读流量与写流量再相加；"
        "A与合计列是按响应完成量折算的近似吞吐（proxy）。"
        "“合计/Solo”先与同phase、同block的Solo配对，再取中位，故可能与两列中位数相除略有差异；"
        "“边界区间”是该配对比值下界/上界各自的中位数，不是置信区间。", "",
        "A1的16 MiB完成记账与V相同，所以双方在该记账口径下各占共跑总量约一半。"
        "这支持“基本平分”的有限描述；没有逐beat数据来证明物理DDR带宽公平分配。"
        "A8/A16/A64的相对Solo合计损失分别为"
        f"DS0 {med('ds0-middle','A8','aggregate_loss_vs_solo_percent'):.3f}%/"
        f"{med('ds0-middle','A16','aggregate_loss_vs_solo_percent'):.3f}%/"
        f"{med('ds0-middle','A64','aggregate_loss_vs_solo_percent'):.3f}%，"
        f"DS50 {med('ds50-pooled','A8','aggregate_loss_vs_solo_percent'):.3f}%/"
        f"{med('ds50-pooled','A16','aggregate_loss_vs_solo_percent'):.3f}%/"
        f"{med('ds50-pooled','A64','aggregate_loss_vs_solo_percent'):.3f}%。", "",
        table(["配置", "条件", "n", "A响应记账 MiB", "V MiB/s", "A MiB/s ≈", "合计 MiB/s ≈", "合计/Solo %", "边界区间 %"],
              [[labels[r["scope"]], r["condition"], r["n"],
                fmt(None if r["aggressor_response_bytes"] is None else r["aggressor_response_bytes"] / 2**20, 6),
                fmt(r["victim_MiB_s"]), fmt(r["aggressor_MiB_s_proxy"]), fmt(r["total_MiB_s_proxy"]),
                fmt(r["total_over_solo_percent"]),
                f"[{med(r['scope'],r['condition'],'total_over_solo_lower_percent'):.3f}, "
                f"{med(r['scope'],r['condition'],'total_over_solo_upper_percent'):.3f}]"] for r in throughput]), "",
        f"DS0 A1 配对时间比的中位数为{med('ds0-middle','A1','time_over_solo'):.6f}×；"
        f"相对2×Solo的配对时间差中位为{med('ds0-middle','A1','time_beyond_two_solo_ms'):.5f} ms。"
        "总吞吐略高于Solo只说明本配置下完成记账的观测关系，现有数据不足以区分具体共享资源的利用率变化，"
        "也不能据此证明每条路径的带宽上限。", "",
        "## 2. V 的 IOMMU 路径等待", "",
        f"DS0读路径内部阶段的试次均值中位数为Solo/A1=25、A8="
        f"{med('ds0-middle','A8','vr1_trial_mean_cycles'):.3f}、A16="
        f"{med('ds0-middle','A16','vr1_trial_mean_cycles'):.3f}、A64="
        f"{med('ds0-middle','A64','vr1_trial_mean_cycles'):.3f}周期，表明原始直连配置也有内部等待增长。"
        f"DS50合并的A8为{med('ds50-pooled','A8','vr1_trial_mean_cycles'):.3f}周期，"
        "因此既有25→601周期结论是延迟敏感性结果，不能替代DS0的25→143周期。"
        "两种DS下入口均为1周期、出口地址反压均为0，A1主要增加后段时延；"
        "这些是每笔突发的阶段延迟，读写及事务之间重叠，不能相加解释整批慢化。", "",
        "每个 V 阶段每试次均有8192个完整阶段样本（start=carry=end=0），"
        "先算sum/full，再跨试次取中位。单位为50MHz时钟周期；"
        "下表也保留DS50两boot各n=5的独立结果。", "",
        table(["配置", "条件", "读入口", "读内部", "读出口地址反压", "读后段", "写入口", "写内部", "写出口地址反压", "写后段"],
              [[labels[scope], c,
                *[fmt(med(scope,c,f"v{ch}{s}_trial_mean_cycles")) for ch in "rw" for s in range(4)]]
               for scope in (*PHASES,"ds50-pooled") for c in CONDITIONS]), "",
        "四阶段分别为：私有DMA地址首次VALID→IOMMU入口地址握手；"
        "该握手→IOMMU出口地址首次VALID；出口首次VALID→地址握手；"
        "出口地址握手→读RLAST/写B返回。内部阶段包括IOMMU与其内部缓冲/排队，"
        "不是纯页表遍历服务时间；后段包含下游数据/响应服务。", "",
        "## 3. 离散度与95% bootstrap置信区间", "",
        f"DS0五条件硬件时间IQR为{min(ds0_iqr):.5f}–{max(ds0_iqr):.5f} ms，"
        f"DS50合并为{min(ds50_iqr):.5f}–{max(ds50_iqr):.5f} ms；"
        "全部样本保留，包括DS50前boot的A64最小值。"
        f"DS50后boot各条件时间中位数比前boot高{min(r['last_over_first_change_percent'] for r in pooling):.3f}%–"
        f"{max(r['last_over_first_change_percent'] for r in pooling):.3f}%，两boot的方向一致但存在小幅偏移，"
        "所以同时报告分boot结果与等权合并结果。"
        "置信区间描述已观察boot条件下试次中位数的不确定性，不能代表跨重启总体、硬件配置等价性或系统性测量误差。", "",
        "### V 硬件时间（ms）", "",
    ]
    for metric in ("hardware_ms", "victim_MiB_s"):
        if metric == "victim_MiB_s":
            lines += ["### V 吞吐（MiB/s）", ""]
        rows = [r for r in stats if r["metric"] == metric]
        lines += [table(["配置", "条件", "n", "中位数", "IQR", "最小值", "最大值", "中位数95% CI"],
                        [[labels[r["scope"]],r["condition"],r["n_trials"],
                          *[fmt(r[k],5) for k in ("median","iqr","minimum","maximum")],
                          f"[{r['ci95_low']:.5f}, {r['ci95_high']:.5f}]"] for r in rows]), ""]
    lines += [
        "### DS50 合并依据与方法", "",
        "前后两个boot记录相同BIT哈希、相同pathv3 kernel release，实验包声明共用一个payload；"
        "源码与负载参数相同、每条件各5次，沿用同一轮转方案；本分析不追加加载验证或倒填SD字节证明。"
        "合并是对这两个已观察boot的等权描述，不以“不显著”作为等价证明。"
        "下表完整显示偏移；两boot以外的温度、运行顺序等混杂因素未被排除。", "",
        table(["条件", "前boot中位 ms", "后boot中位 ms", "后−前 ms", "相对变化 %"],
              [[r["condition"],fmt(r["first_median_ms"],5),fmt(r["last_median_ms"],5),
                fmt(r["last_minus_first_ms"],5),fmt(r["last_over_first_change_percent"],6)] for r in pooling]), "",
        table(["采集段", "boot_id", "每条件n", "BIT声明SHA256"],
              [[labels[phase],index[phase,"Solo","hardware_ms"]["boot_ids"],
                index[phase,"Solo","hardware_ms"]["n_trials"],manifests[phase]["bit_sha256"]] for phase in PHASES]), "",
        f"IQR=Q3−Q1，采用Type 7线性插值。每个分组/指标做{RESAMPLES}次有放回重采样，"
        f"固定基础种子{SEED}，分组种子为SHA256(基础种子:scope:condition)的前8字节大端整数；"
        "同组各指标共用重采样索引。分boot组各抽原样本数，DS50合并组每次在前/后boot内各抽5次再合并10次，"
        "取bootstrap中位数分布的2.5%和97.5%分位；不对两个boot再做总体重启重采样。", "",
        "相对Solo的指标按完整区组配对后重采样；区间均为逐指标pointwise percentile CI，不是多指标同时区间。"
        "n=5时区间常覆盖完整观察范围，这是小样本离散bootstrap的正常现象；"
        "未声称正态性、跨试次独立性或已消除串行相关，区间条件于组内试次可交换这一近似。"
        "路径、A记账量、合计吞吐、配对比值及HPM计数的中位数/Q1/Q3/IQR/min/max/95% CI"
        "全部在[JSON的statistics数组](baseline100-extra.json)，不只保留上表两项。", "",
        "## 口径不确定性与可确认边界", "",
        "### A 的跨窗口事务", "",
        "1. 窗口从V首个任务issue之后开始，包含第4096个任务最终完成边沿，"
        "软件poll的done_to_read不在窗内；`record.result.aggressor.completed_bytes`是A启动至最后排空的整批完成量，"
        "包含V窗口前后的工作，不能除以V时间。本分析只采用相同observer序列的`axi_a.b`，"
        "读RLAST用于交叉核对；80个共跑样本均为ar=aw=rlast=b=8192，"
        "A开/关窗的已接受未响应读写突发各为2，V均为0。", "",
        "A不是预定4096次后停下的有限批：驱动给A设定的目标为`IDMA_EXPERIMENT_MAX_TRANSFERS=2**32`，"
        "V完成后由软件STOP并排空；本轮A整批完成4105–5623次，全部大于4096。"
        "observer还在整个V活动窗口检查A的batch_active，异常会使记录失效。"
        "这支持全窗保持自主批量供给，但不保证每周期都有可服务请求或资源饱和，"
        "也不据此声称全窗公平。开闭边界均有2个未响应写burst，不能用AW/B守恒把W字节误差化为零。", "",
        "2. 固定配置为4KiB COPY、64位AXI、最长256 beat且不deburst，故每COPY分为两个2048字节突发；"
        "`C_A=2048×B_A=16777216 bytes`是按响应完成的名义COPY字节记账。"
        "跨窗前已发起而窗内响应的突发计入C，窗内发起但窗后响应的不计入；"
        "它不是“整个COPY从发起到完成都落在窗口内”的字节数。", "",
        "3. 严格窗口内DMA私有接口的W握手有效字节未记录，不能精确恢复。"
        "对合法AXI，设D(t)为边界前已W握手但尚未取得B记账的字节，"
        "则`S_A=C_A+D_end−D_start`；实际frontend每DMA只允许一个4KiB任务在途，"
        "故`0≤D≤4096`，从而`C_A−4096≤S_A≤C_A+4096`（"
        f"[{16*2**20-4096}, {16*2**20+4096}] bytes）。"
        f"A字节/吞吐边界相对误差至多{4096/(16*2**20)*100:.7f}%，"
        "合计吞吐的相对误差至多一半；W可早于AW，不能仅凭AW在途数再缩小界限。", "",
        "4. `path.aw3.full`只保证出口AW→B这一阶段两端在窗内；"
        "开窗时在内部阶段的事务也可随后成为aw3.full，不能将它当作整个写事务或COPY的同窗完成量。"
        "边界公式衡量私有AXI接口传输，不是物理DDR写入的时间；observer未保存每个W beat/WSTRB或每个BRESP。"
        "因此上述±4096仅是合法AXI及固定长度条件下的窗口边界误差，"
        "不约束未观测的错误响应、物理落地或每次重复COPY的逐次正确性；"
        "已有status=0、排空与末态data_errors=0均保留，不能据此杜撰逐次校验。", "",
        "### 路径、HPM与因果限制", "",
        "V路径阶段均完整覆盖，A跨窗不污染V的sum/full中位数。HPM来自非原子软件窗口，"
        "DS0 A8/A16/A64的V miss中位2仍保留在JSON，不能据此定位其发生于V硬件窗口；"
        "DS50−DS0也不等于纯PTW费用。总吞吐低于Solo是描述性损失，"
        "当前计数不足以把它全部归到IOMMU内部某一阶段或某个具体带宽瓶颈。", "",
        "## 原始证据与实际源码", "",
        "输入为三段`records.json`、对应`manifest.json`和`*-load/load.json`、只读标量长表与"
        "[固定执行身份](../board/packages/baseline100-20261017/source.json)；"
        "另保存[固定源码摘录](inputs/baseline100-source-audit.json)，其12个源码文件按当时commit读取。"
        "逐试次JSON提供raw_path、JSON pointer、精确原始行与同block Solo pointer。"
        "源码口径按实际执行commit读取，不以当前研究checkout替代：", "",
        f"- CVA6 `{source['actual_commits']['cva6']}`，下列路径均相对该仓库："
        "`corev_apu/fpga/src/dma_observer.sv:53–74,148–204`为共享活动窗口、开闭沿和AXI累计；"
        "`corev_apu/fpga/src/dma_path_observer.sv:27–44,62–85,132–142`为完整阶段/跨窗计数；"
        "`corev_apu/fpga/src/ariane_peripherals_xilinx.sv:912–946`为私有接口RLAST/B与路径实际接线。",
        "- 同一CVA6 commit的`corev_apu/fpga/src/ariane_xilinx.sv:158–159`定义64位data；"
        "`corev_apu/iDMA/src/idma_pkg.sv:107–110`与`corev_apu/iDMA/src/idma_legalizer.sv:75–85,143–178,235–267,335–359`"
        "推得每burst2048字节；`corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv:96–102,332–363`"
        "限定4KiB且COUNT→WAIT直至完成才发下一任务。",
        "- 单任务完成链为同一CVA6 commit的`corev_apu/iDMA/src/systems/cva6_reg/dma_core_wrap.sv:180–181,283–286`"
        "→`corev_apu/iDMA/src/idma_backend.sv:519–532,643–648`"
        "→`corev_apu/iDMA/src/idma_axi_transport_layer.sv:447–455`，"
        "即最后burst的B触发任务完成、NO_ERROR_HANDLING不检查BRESP；"
        "transport同文件:354–360的WVALID不要求AW已握手。",
        f"- Linux `{source['actual_commits']['linux']}` 的`drivers/misc/idma.c:341–346`写CONF=0；"
        "同文件:556–565,590–615,963–998给出A先启动、STOP/drain后才读取整批completed_bytes。"
        "同文件:941–943与`include/uapi/linux/idma.h:9`给A设置2**32目标。"
        "同文件:952–970把非原子HPM快照与软件窗口绑定；原始record.semantics也明示整批与软件窗口。", "",
        "例如可只读核查：", "", "```sh",
        f"git -C iommu-platform/cva6 show {source['actual_commits']['cva6']}:corev_apu/iDMA/src/frontends/register_64bit/idma_reg64_frontend.sv",
        f"git -C iommu-platform/linux show {source['actual_commits']['linux']}:drivers/misc/idma.c",
        "```", "",
        "该CVA6 commit中的`corev_apu/iDMA`是普通Git tree（非独立gitlink），"
        "以上iDMA文件均直接从该固定CVA6 commit读取。", "",
        "## 统计验证与复现", "",
        "统计谬误检查覆盖11/11：分boot报告检查聚合掩盖（Simpson）；单位固定为试次，不作个体请求生态推断；"
        "Berkson选择偏差与survivorship：100条全保留，但范围仍限于这一个平台、固定负载与已观察boot；"
        "collider：未按结果或后果变量作额外调整；base-rate：不涉及诊断概率；"
        "regression-to-mean：未按极端结果选组重测；look-elsewhere：不筛选显著指标；"
        "forking-paths：固定全部条件、指标、分位算法和种子，"
        "明确这是事后描述分析；不把关联解释为已定位因果，也不反推因果方向。"
        "小样本、试次可交换性及跨boot推广仍为CAUTION，不将CI当等价检验。", "",
        "```sh", "python3 paper/ispass2027/analysis/scripts/baseline_extra.py",
        "python3 paper/ispass2027/analysis/scripts/baseline_extra.py --check",
        "python3 paper/ispass2027/analysis/scripts/baseline_extra.py --self-test", "```", "",
        "第一条只生成本分析目录的文件；第二条只读重算并逐字节比较全部输出。"
        "每次都比较读取前/后的输入SHA256，任何输入或输出不一致即失败；未运行采集器、构建、仿真或板测。", "",
        "逐试次重算已交叉核对长表1000个hardware/path标量；负向测试只改临时副本，"
        "确认A响应不守恒、V阶段不完整、试次数不足与BIT声明不符均被拒绝，并检查转义JSON的原始行映射。", "",
        "| 输出 | 用途 |", "| --- | --- |",
        "| [完整JSON](baseline100-extra.json) | samples、statistics、throughput、path、boot_comparison、formulas及输入SHA256 |",
        "| [计数器源码摘录](inputs/baseline100-source-audit.json) | 实际commit、原文件行号、摘录与完整源文件SHA256 |", "",
        "稳定结果ID为`baseline100-extra.<scope>.<condition>.<metric>`；scope取`ds0-middle`、"
        "`ds50-first`、`ds50-last`或`ds50-pooled`。例如DS0 A8读内部中位数位于"
        "`baseline100-extra.ds0-middle.A8.vr1_trial_mean_cycles`的`median`；同一对象给出IQR、min/max和CI，"
        "按contributing_sample_ids追到每一个`/path/vr1/{sum,full}`的原始行。"
        "吞吐表metric对应`aggressor_response_bytes`、`victim_MiB_s`、`aggressor_MiB_s_proxy`、"
        "`total_MiB_s_proxy`及`total_over_solo_percent`；所有单元均由JSON中同名formula_id计算。", "",
    ]
    return "\n".join(lines)


if __name__ == "__main__":
    main()
