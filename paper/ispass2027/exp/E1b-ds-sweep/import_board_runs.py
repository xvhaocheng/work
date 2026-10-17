#!/usr/bin/env python3
"""Prepare a deterministic, source-bound import of explicitly named board runs.

This module never opens UART, executes collector code, or changes run artifacts.
Output is confined to exp/E1b-ds-sweep; shared data tables are never written.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
import math
from pathlib import Path
import re
import statistics
import sys

sys.dont_write_bytecode = True
import table_schema as tables

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
PHASES = {
    "g1": (3, ("Solo", "A1", "A8"), "50/50", tuple(range(3))),
    "ds50-first": (5, ("Solo", "A1", "A8", "A16", "A64"), "50/50", tuple(range(5))),
    "ds0-middle": (10, ("Solo", "A1", "A8", "A16", "A64"), "0/0", tuple(range(10))),
    "ds50-last": (5, ("Solo", "A1", "A8", "A16", "A64"), "50/50", tuple(range(5, 10))),
    "ds50-all": (10, ("Solo", "A1", "A8", "A16", "A64"), "50/50", tuple(range(10))),
    "ds0-all": (10, ("Solo", "A1", "A8", "A16", "A64"), "0/0", tuple(range(10))),
    "ds25-all": (10, ("Solo", "A1", "A8", "A16", "A64"), "25/25", tuple(range(10))),
    "ds100-all": (10, ("Solo", "A1", "A8", "A16", "A64"), "100/100", tuple(range(10))),
}
FORMAL = ("ds50-first", "ds0-middle", "ds50-last")
FORMAL_AB = ("ds50-all", "ds0-all")
DS_SWEEP = ("ds25-all", "ds100-all")
MISSING = object()


def json_text(value):
    return json.dumps(value, ensure_ascii=False, indent=2, sort_keys=True, allow_nan=False)+"\n"


def sha_bytes(value):
    return hashlib.sha256(value).hexdigest()


def relative(path):
    return Path(path).resolve().relative_to(ROOT).as_posix()


def number(value):
    return isinstance(value, (int, float)) and not isinstance(value, bool) and math.isfinite(value)


def optional(obj, pointer):
    try:
        return tables.locate(obj, pointer)
    except (KeyError, IndexError, TypeError, ValueError):
        return MISSING


def batch_documents(text):
    """Decode complete CLI objects even when printk prefixes the same line."""
    decoder = json.JSONDecoder()
    documents = []
    for match in re.finditer(r'\{\s*"schema"\s*:\s*"idma\.batch\.v1"', text):
        document, _ = decoder.raw_decode(text, match.start())
        documents.append(document)
    return documents


def run_reference(value, run_dir):
    path = Path(value)
    if path.is_absolute():
        return path.resolve()
    if path.parts[:1] == ("paper",) or path.parts[:1] == ("iommu-platform",):
        return (ROOT/path).resolve()
    return (run_dir/path).resolve()


class Inputs:
    def __init__(self):
        self.files = {}
        self.missing = []

    def file(self, path, required=True, expected=None):
        path = Path(path).resolve()
        rel = relative(path)
        if not path.is_file():
            if required:
                raise ValueError("Required input missing: "+rel)
            self.missing.append(rel)
            return None
        data = path.read_bytes()
        digest = sha_bytes(data)
        if expected and digest != expected:
            raise ValueError("Input hash disagrees with recorded receipt: "+rel)
        self.files[rel] = {"raw_path": rel, "sha256": digest}
        return data

    def load(self, path, required=True, expected=None):
        data = self.file(path, required, expected)
        return json.loads(data) if data is not None else None


def recovered_manifest_chain(reader, manifest, run_dir):
    chain, seen = [], set()
    while manifest.get("recovered_from"):
        path = run_reference(manifest["recovered_from"], run_dir)
        if path in seen:
            raise ValueError("Cycle in recovered_from manifest chain")
        seen.add(path)
        expected = (manifest.get("recovered_manifest_sha256") or manifest.get("recovered_from_sha256") or
                    manifest.get("recovered_from_manifest_sha256"))
        if not expected:
            raise ValueError("Recovered source manifest needs its recorded SHA256")
        manifest = reader.load(path, expected=expected)
        chain.append({"raw_path": relative(path), "sha256": expected,
                      "status": manifest.get("status", "unknown"), "error": manifest.get("error")})
        run_dir = path.parent
    return chain


def load_evidence_chain(reader, loaded, load_path):
    """Keep existing program/authentication/SD receipt references, not credentials."""
    evidence, seen = [], set()
    def follow(document, parent):
        for key in ("resumed_from", "authentication_receipt", "sd_evidence", "raw_receipt"):
            if not document.get(key):
                continue
            path = run_reference(document[key], parent)
            if path in seen:
                continue
            seen.add(path)
            expected = document.get("original_receipt_sha256") if key == "resumed_from" else None
            child = reader.load(path, expected=expected)
            evidence.append({"role": key, "raw_path": relative(path),
                             "sha256": reader.files[relative(path)]["sha256"],
                             "status": child.get("status", "unknown"), "error": child.get("error")})
            if key == "resumed_from":
                for name in ("program.log", "program.tcl"):
                    raw = path.parent/name
                    if raw.is_file():
                        reader.file(raw)
                        evidence.append({"role": name, **reader.files[relative(raw)]})
            follow(child, path.parent)
    if loaded:
        follow(loaded, load_path.parent)
    return evidence


def scalar(value, source_path, ptr, *, status=None, operation=None, inputs=None):
    exists = value is not MISSING
    if not exists:
        value = None
    if value is not None and not number(value):
        raise ValueError(f"Non-numeric scalar at {source_path}#{ptr}: {value!r}")
    return {"value": value,
            "value_status": status or ("unknown" if value is None else "observed"),
            "source": {"raw_path": source_path, "source_locator": ptr, "exists": exists},
            "operation": operation, "inputs": inputs or []}


def sample_metrics(record, source_path, index, expect_path):
    base = tables.pointer(index)
    values = {}

    def direct(name, ptr, unit=None, absent_a=False):
        value = optional(record, ptr)
        actual = base+ptr
        status = None
        if absent_a and optional(record, "/record/result/aggressor") is None:
            value, actual, status = None, base+"/record/result/aggressor", "not_applicable"
        values[name] = scalar(value, source_path, actual, status=status)
        values[name]["unit"] = unit or tables.unit(name)

    def computed(name, operands, calculation, operation, unit):
        fields = [optional(record, ptr) for ptr in operands]
        valid = all(number(v) for v in fields)
        value = calculation(*fields) if valid else None
        if value is not None and not math.isfinite(value):
            value = None
        supplied = optional(record, "/metrics/"+name)
        if number(supplied) and number(value) and not math.isclose(supplied, value, rel_tol=1e-12, abs_tol=1e-10):
            raise ValueError(f"Collector metric disagrees with original fields: {source_path}{base}/metrics/{name}")
        refs = [{"raw_path": source_path, "source_locator": base+ptr,
                 "exists": field is not MISSING} for ptr, field in zip(operands, fields)]
        values[name] = scalar(value, source_path, base+"/metrics/"+name,
                              status="derived" if value is not None else "unknown",
                              operation=operation, inputs=refs)
        # A supplied metric is a direct scalar locator only after recomputation.
        values[name]["source"]["exists"] = supplied is not MISSING
        values[name]["source"]["matches_value"] = number(supplied) and supplied == value
        values[name]["unit"] = unit

    computed("hardware_ms", ["/observer/time/cycles"], lambda cycles: cycles/50000,
             "cycles / 50000; collector's recorded 50MHz assumption", "ms")
    computed("hardware_MiB_s", ["/record/result/victim/completed_bytes", "/observer/time/cycles"],
             lambda size, cycles: size/1048576/(cycles/50e6) if cycles else None,
             "completed_bytes / 2**20 / (hardware_cycles / 50000000)", "MiB/s")
    computed("software_ms", ["/record/result/victim_elapsed_ns"], lambda ns: ns/1e6,
             "victim_elapsed_ns / 1000000", "ms")
    computed("overlap_percent", ["/observer/axi_v/both", "/observer/time/cycles"],
             lambda both, cycles: both*100/cycles if cycles else None,
             "axi_v.both * 100 / hardware_cycles", "%")
    for name, field in {"hardware_cycles": "cycles", "h_cycles": "h", "gap_cycles": "gap",
                        "done_to_read_cycles": "done_to_read"}.items():
        direct(name, "/observer/time/"+field, "cycles")
    for field in ("issued", "done"):
        direct("victim_window_"+field, "/observer/time/"+field)
    for lane in ("victim", "aggressor"):
        for field in ("submitted", "completed", "completed_bytes", "iotlb_miss", "s1_ptw", "s2_ptw"):
            direct(lane+"_"+field, "/record/result/"+lane+"/"+field, absent_a=lane == "aggressor")
    for lane, label in (("axi_v", "v"), ("axi_a", "a")):
        for field in ("ar", "aw", "rlast", "b", "ar_wait", "aw_wait", "start", "end"):
            units = "cycles" if field.endswith("_wait") else "packed_read16_write16" if field in {"start", "end"} else "count"
            direct(label+"_"+field, "/observer/"+lane+"/"+field, units)
    for field in ("neither", "v_only", "a_only", "both"):
        direct("occupancy_"+field+"_cycles", "/observer/axi_v/"+field, "cycles")
    for field in ("shared_ar", "shared_aw", "shared_ar_wait", "shared_aw_wait"):
        direct(field, "/observer/axi_a/"+field, "cycles" if field.endswith("_wait") else "count")
    if expect_path or record.get("path"):
        for role in "va":
            for channel in "rw":
                for stage in range(4):
                    key = role+channel+str(stage)
                    operands = ["/path/"+key+"/sum", "/path/"+key+"/full"]
                    name = "path_"+key+"_mean_cycles"
                    full = optional(record, operands[1])
                    if full == 0:
                        values[name] = scalar(None, source_path, base+operands[1], status="not_applicable",
                                              operation="sum/full undefined when full=0")
                        values[name]["unit"] = "cycles"
                    else:
                        computed(name, operands, lambda total, full: total/full if full else None,
                                 "path.sum / path.full", "cycles")
                    direct("path_"+key+"_max_cycles", "/path/"+key+"/max", "cycles")
                    if full == 0:
                        values["path_"+key+"_max_cycles"]["value"] = None
                        values["path_"+key+"_max_cycles"]["value_status"] = "not_applicable"
                        values["path_"+key+"_max_cycles"]["source"]["matches_value"] = False
    return values


def validate_record(record, phase, condition, expected_ds):
    """Bounded checks on saved parse outputs; not a new hardware certification."""
    errors = []
    wanted = {"/record/success": True, "/record/errno": 0, "/record/result/status": 0,
              "/record/input/device": "/dev/idma0", "/record/input/victim_pages": 1,
              "/record/input/chunk_bytes": 4096, "/record/input/victim_transfers": 4096,
              "/record/input/warmup_transfers": 256, "/record/result/victim/completed": 4096,
              "/record/result/victim/submitted": 4096, "/record/result/victim/completed_bytes": 4096*4096,
              "/observer/time/valid": 1, "/observer/time/status": 4,
              "/observer/time/issued": 4096, "/observer/time/done": 4096,
              "/observer/time/batch_polled": 1}
    for ptr, expected in wanted.items():
        if optional(record, ptr) != expected:
            errors.append({"field": ptr, "expected": expected, "observed": None if optional(record, ptr) is MISSING else optional(record, ptr)})
    if not isinstance(record.get("sequence"), int) or isinstance(record.get("sequence"), bool) or record["sequence"] <= 0:
        errors.append({"field": "/sequence", "problem": "missing or invalid observer sequence"})
    expected_aggressor = None if condition == "Solo" else 1
    if optional(record, "/record/input/aggressor_id") != expected_aggressor:
        errors.append({"field": "/record/input/aggressor_id", "expected": expected_aggressor,
                       "problem": "does not match fixed V0/A1 contract"})
    if record.get("ds") != expected_ds:
        errors.append({"field": "/ds", "expected": expected_ds, "observed": record.get("ds")})
    actual_mode = "Solo" if optional(record, "/record/result/aggressor") is None else "A"+str(optional(record, "/record/input/aggressor_pages"))
    if condition != actual_mode:
        errors.append({"field": "/condition", "expected": actual_mode, "observed": condition})
    for lane in ("victim", "aggressor"):
        obj = optional(record, "/record/result/"+lane)
        if lane == "aggressor" and obj is None:
            continue
        if not isinstance(obj, dict) or obj.get("data_errors") != 0 or obj.get("quarantined") != 0 or obj.get("submitted") != obj.get("completed"):
            errors.append({"field": "/record/result/"+lane, "problem": "data, drain or missing lane"})
    if phase != "g1" and set(record.get("path", {})) != {r+c+str(s) for r in "va" for c in "rw" for s in range(4)}:
        errors.append({"field": "/path", "problem": "16 path entries required for baseline series"})
    return errors


def load_run(reader, run_dir):
    run_dir = Path(run_dir).resolve()
    manifest_path = run_dir / "manifest.json"
    manifest = reader.load(manifest_path)
    phase = manifest.get("phase")
    if phase not in PHASES:
        raise ValueError("Not a collector run manifest: "+relative(manifest_path))
    repeat, conditions, ds, blocks = PHASES[phase]
    run_id = tables.slug(run_dir.name)+"-"+sha_bytes(relative(run_dir).encode())[:8]
    series = "g1" if phase == "g1" else "ds_sweep" if phase in DS_SWEEP else "baseline100"
    load_path = run_dir / "load.json"
    if not load_path.is_file() and manifest.get("load_manifest"):
        load_path = Path(manifest["load_manifest"])
        if not load_path.is_absolute():
            load_path = ROOT / load_path
    loaded = reader.load(load_path, required=False)
    load_matches = loaded is not None and reader.files[relative(load_path)]["sha256"] == manifest.get("load_manifest_sha256")
    kernel = manifest.get("kernel_release") or "unknown"
    bit = manifest.get("bit_sha256") or "unknown"
    identity_bound = bool(load_matches and loaded.get("status") == "boot_identity_verified" and
                          loaded.get("kernel_release") == kernel and loaded.get("bit_sha256") == bit)
    readback = loaded.get("configuration_readback_verified") if loaded else None
    program_hash = loaded.get("program_file_sha256_verified") if loaded else None
    identity_status = ("program_file_hash_boot_verified_no_readback" if identity_bound and program_hash and not readback
                       else "configuration_readback_verified" if identity_bound and readback
                       else "program_file_or_load_binding_unknown")
    records = reader.load(run_dir / "records.json", required=False) or []
    review = reader.load(run_dir / "review.json", required=False)
    if not isinstance(records, list):
        raise ValueError("records.json must be a list: "+relative(run_dir))
    recovery = {"recovered_cases": manifest.get("recovered_cases", 0),
                "recovered_raw_commands": [], "recovered_from": None,
                "recovered_from_sha256": None}
    recovery["manifest_chain"] = recovered_manifest_chain(reader, manifest, run_dir)
    if recovery["manifest_chain"]:
        prior = recovery["manifest_chain"][0]
        recovery.update(recovered_from=prior["raw_path"], recovered_from_sha256=prior["sha256"],
                        prior_manifest_status=prior["status"], prior_manifest_error=prior["error"])
    recovered_paths = {run_reference(p, run_dir) for p in manifest.get("recovered_raw_commands", [])}
    recovery["recovered_raw_commands"] = sorted(relative(p) for p in recovered_paths)
    commands = []
    local_paths = {p.resolve() for p in (run_dir / "uart").glob("command-*.json")}
    for path in sorted(local_paths | recovered_paths):
        command = reader.load(path)
        text = command.get("stdout", "").replace("\r", "")
        decoding_error = None
        try:
            cli = batch_documents(text)
        except json.JSONDecodeError as error:
            cli, decoding_error = [], str(error)
        pre = re.findall(r"^PRE_SEQ=(\d+)$", text, re.M)
        boot = re.findall(r"^BOOT=([^\n]+)$", text, re.M)
        commands.append({"path": relative(path), "object": command, "cli": cli, "matched": False,
                         "recovered": path in recovered_paths, "cli_decoding_error": decoding_error,
                         "expected_sequence": int(pre[0])+1 if len(pre) == 1 else None,
                         "boot_id": boot[0] if len(boot) == 1 else None})
    run = {"run_id": run_id, "run_path": relative(run_dir), "series": series, "phase": phase,
           "manifest_status": manifest.get("status", "unknown"), "manifest_error": manifest.get("error"),
           "planned_cases": manifest.get("planned_cases", []), "expected_repetitions_per_condition": repeat,
           "expected_points": repeat*len(conditions), "ds": ds,
           "identity": {"kernel_id": kernel, "bit_sha256": bit,
                        "load_path": relative(load_path), "load_hash_matches_manifest": load_matches,
                        "binding_verified": identity_bound, "identity_status": identity_status,
                        "configuration_readback_verified": readback,
                        "program_file_sha256_verified": program_hash,
                        "load_evidence": load_evidence_chain(reader, loaded, load_path),
                        "boot_id": loaded.get("boot_id", "unknown") if loaded else "unknown"},
           "review": review, "review_path": relative(run_dir / "review.json"),
           "recovery": recovery,
           "fixture_only": bool(manifest.get("synthetic_fixture")), "issues": [], "attempts": []}
    expected_plan = {(b, c) for b in blocks for c in conditions}
    actual_plan = [tuple(p) for p in run["planned_cases"]]
    if set(actual_plan) != expected_plan or len(actual_plan) != len(expected_plan):
        run["issues"].append("planned_cases do not match fixed phase contract")
    if manifest.get("recorded_cases") != len(records):
        run["issues"].append("manifest recorded_cases differs from records.json length")
    for index, record in enumerate(records):
        condition = record.get("condition", "unknown")
        errors = validate_record(record, phase, condition, ds)
        block = record.get("block")
        if (block, condition) not in expected_plan:
            errors.append({"field": "/block,/condition", "problem": "outside phase plan"})
        if record.get("boot_id") != run["identity"]["boot_id"]:
            errors.append({"field": "/boot_id", "problem": "differs from load identity"})
        declared_command = relative(run_reference(record["raw_command_path"], run_dir)) if record.get("raw_command_path") else None
        matching = [c for c in commands if not c["matched"] and (not declared_command or c["path"] == declared_command) and c["cli"] == [record.get("record")] and
                    c["expected_sequence"] == record.get("sequence") and c["boot_id"] == record.get("boot_id")]
        if matching:
            matching[0]["matched"] = True
            if matching[0]["object"].get("exit_code") != 0:
                errors.append({"field": "/raw_command_path", "problem": "original command exit status is not zero",
                               "observed": matching[0]["object"].get("exit_code")})
        elif declared_command:
            errors.append({"field": "/raw_command_path", "observed": declared_command,
                           "problem": "declared original command is absent, not declared for recovery, already used, or mismatches CLI/sequence/boot"})
        try:
            metrics = sample_metrics(record, relative(run_dir / "records.json"), index, phase != "g1")
        except ValueError as exc:
            errors.append({"field": "/metrics", "problem": str(exc)})
            # Retain bad raw values and the error without minting accepted stats.
            metrics = {name: scalar(optional(record, "/metrics/"+name), relative(run_dir / "records.json"), tables.pointer(index, "metrics", name))
                       for name in ("hardware_ms", "hardware_MiB_s", "hardware_cycles", "victim_iotlb_miss", "aggressor_iotlb_miss")}
            for name, item in metrics.items():
                item["unit"] = tables.unit(name)
        run["attempts"].append({"attempt_id": f"case{index+1:04d}", "case": record.get("case", index+1),
            "record_index": index, "condition": condition, "block": block,
            "boot_id": record.get("boot_id") or "unknown_in_"+run_id,
            "accepted": not errors, "errors": errors, "retained_by_collector": True,
            "raw_record_path": relative(run_dir / "records.json"), "raw_record_locator": tables.pointer(index),
            "command_path": matching[0]["path"] if matching else None,
            "recovered_original_command": bool(matching and matching[0]["recovered"]),
            "command_binding": "original_CLI_JSON_equal_and_fresh_sequence_and_boot" if matching else "raw_command_unavailable",
            "metrics": metrics})
    for command in commands:
        if command["matched"]:
            continue
        # Collector aborts on the first unretained attempt, but never invent a
        # block when more than one unassociated command makes order ambiguous.
        unmatched = sum(not c["matched"] for c in commands)
        point = actual_plan[len(records)] if unmatched == 1 and len(records) < len(actual_plan) else (None, "unknown")
        suffix = sha_bytes(command["path"].encode())[:8]
        metrics = {name: scalar(MISSING, command["path"], "/stdout", status="unknown")
                   for name in ("hardware_ms", "hardware_MiB_s", "hardware_cycles", "victim_completed", "aggressor_completed", "victim_iotlb_miss", "aggressor_iotlb_miss")}
        for name, value in metrics.items():
            value["unit"] = tables.unit(name)
        run["attempts"].append({"attempt_id": "unretained-"+suffix,
            "case": len(records)+1 if unmatched == 1 else None,
            "record_index": None, "condition": point[1], "block": point[0],
            "boot_id": run["identity"]["boot_id"], "accepted": False,
            "retained_by_collector": False, "errors": [{"problem": "raw UART command not retained in records.json; no performance acceptance inferred", "manifest_error": run["manifest_error"]}],
            "command_path": command["path"], "command_exit_code": command["object"].get("exit_code"),
            "command_error": command["object"].get("error"), "raw_record_path": None,
            "cli_decoding_error": command["cli_decoding_error"], "recovered_original_command": command["recovered"],
            "raw_record_locator": None, "command_binding": "unretained_raw_command", "metrics": metrics})
    boots = sorted({a["boot_id"] for a in run["attempts"] if a["retained_by_collector"]})
    run["observed_boot_ids"] = [b for b in boots if not b.startswith("unknown")]
    recovered_matched = sum(a["recovered_original_command"] for a in run["attempts"] if a["retained_by_collector"])
    recovery["matched_recovered_cases"] = recovered_matched
    if recovery["recovered_cases"] != recovered_matched:
        run["issues"].append("declared recovered_cases differs from matched original commands")
    if len(boots) > 1:
        run["issues"].append("more than one boot inside one collector phase")
    if review is None:
        run["issues"].append("review.json missing; retained samples and failure evidence remain visible")
    else:
        if review.get("count") != len(records):
            run["issues"].append("review count differs from original records")
        if not review.get("sample_count_complete"):
            run["issues"].append("collector review sample_count_complete is false")
        for condition, group in review.get("groups", {}).items():
            trials = [a for a in run["attempts"] if a["retained_by_collector"] and a["condition"] == condition]
            if group.get("n") != len(trials):
                run["issues"].append("review n mismatch: "+condition)
            for metric in ("hardware_ms", "hardware_MiB_s", "victim_iotlb_miss", "aggressor_iotlb_miss"):
                vals = [a["metrics"][metric]["value"] for a in trials if metric in a["metrics"] and number(a["metrics"][metric]["value"])]
                observed = group.get("metrics", {}).get(metric, {}).get("median")
                expected = statistics.median(vals) if vals else None
                if observed != expected and not (number(observed) and number(expected) and math.isclose(observed, expected, rel_tol=1e-12, abs_tol=1e-10)):
                    run["issues"].append("review median mismatch with original records: "+condition+"/"+metric)
    run["accepted_points"] = sum(a["accepted"] for a in run["attempts"])
    run["attempted_points_with_evidence"] = len(run["attempts"])
    run["failed_or_unretained_points"] = sum(not a["accepted"] for a in run["attempts"])
    run["unattempted_plan_points"] = max(0, len(actual_plan)-len(run["attempts"]))
    run["phase_complete"] = bool(run["manifest_status"] == "passed" and not run["issues"] and
        identity_bound and program_hash and len(boots) == 1 and run["accepted_points"] == run["expected_points"] and
        run["failed_or_unretained_points"] == 0 and run["attempted_points_with_evidence"] == run["expected_points"] and
        len({(a["block"], a["condition"]) for a in run["attempts"] if a["accepted"]}) == run["expected_points"] and
        (phase != "g1" or bool(review and review.get("passed"))))
    return run


def aggregate(samples, *, group_id, series, ds, condition, scope, phases):
    boots = sorted({a["boot_id"] for run, a in samples})
    fields = sorted({metric for run, sample in samples for metric in sample["metrics"]})
    metrics = {}
    for field in fields:
        contributors = [(run, a, a["metrics"][field]) for run, a in samples if field in a["metrics"]]
        values = [v["value"] for _, _, v in contributors if number(v["value"])]
        metrics[field] = {"n": len(values), "n_trials": len(samples),
                          "n_unknown_or_not_applicable": len(samples)-len(values),
                          "min": min(values) if values else None,
                          "median": statistics.median(values) if values else None,
                          "max": max(values) if values else None,
                          "unit": contributors[0][2]["unit"],
                          "contributors": [{"run_id": run["run_id"], "attempt_id": a["attempt_id"],
                                            "source": v["source"], "operation": v["operation"], "inputs": v["inputs"]}
                                           for run, a, v in contributors if number(v["value"])]}
    return {"group_id": group_id, "series": series, "ds": ds, "condition": condition,
            "aggregation": scope, "phases": phases, "boot_ids": boots,
            "observed_boot_count": len([b for b in boots if not b.startswith("unknown")]),
            "independent_restart_repetitions": None, "n_trials": len(samples),
            "bit_ids": sorted({run["identity"]["bit_sha256"] for run, _ in samples}),
            "kernel_ids": sorted({run["identity"]["kernel_id"] for run, _ in samples}),
            "source_run_status": {run["run_id"]:run["manifest_status"] for run, _ in samples},
            "source_phase_complete": {run["run_id"]:run["phase_complete"] for run, _ in samples},
            "software_payload_equivalence": "not_inferred_from_kernel_release",
            "metrics": metrics}


def window(metric):
    if metric == "software_ms":
        return "software_task"
    if metric.startswith("done_to_read"):
        return "hardware_done_to_software_read"
    if metric.endswith(("iotlb_miss", "s1_ptw", "s2_ptw")):
        return "non_atomic_HPM_software_snapshot"
    if metric.startswith(("victim_completed", "aggressor_completed", "victim_submitted", "aggressor_submitted")):
        return "whole_lane_including_post_V_drain"
    return "hardware_V_batch"


def row_spec(row_id, series, condition, metric, statistic, unit, n, path, ptr,
             *, boot, sample_set, identity, value_status):
    return dict(zip((f for f in tables.FIELDS if f != "value"), [
        row_id, "board_"+series, condition, metric, statistic, unit, n,
        "program_file_sha256:"+identity["bit_sha256"] if identity["bit_sha256"] != "unknown" else "unknown",
        identity["kernel_id"], path, ptr, "fpga_board", value_status,
        identity.get("identity_status", "explicit_multi_boot_aggregation"), boot, sample_set]))


def prepare(run_dirs, output, name, software_evidence=()):
    output = Path(output).resolve()
    output.relative_to(ROOT / "paper/ispass2027/exp/E1b-ds-sweep")
    reader = Inputs()
    evidence = []
    for path in sorted(map(Path, software_evidence), key=str):
        reader.load(path)
        evidence.append({"raw_path": relative(path), "sha256": reader.files[relative(path)]["sha256"],
                         "binding": "evidence_pointer_only; payload equality and actual SD boot bytes not inferred"})
    runs = [load_run(reader, p) for p in sorted({Path(p).resolve() for p in run_dirs}, key=lambda p: relative(p))]
    samples = [(run, a) for run in runs for a in run["attempts"] if a["accepted"]]
    groups = []
    strata = collections.defaultdict(list)
    for run, a in samples:
        strata[(run["run_id"], run["phase"], a["boot_id"], a["condition"])].append((run, a))
    for (run_id, phase, boot, condition), items in sorted(strata.items()):
        groups.append(aggregate(items, group_id="stratum-"+tables.slug(run_id+"-"+condition),
            series=items[0][0]["series"], ds=items[0][0]["ds"], condition=condition,
            scope="within_one_run_phase_and_boot", phases=[phase]))
    formal_runs = [r for r in runs if r["series"] == "baseline100"]
    observed_phases = {r["phase"] for r in formal_runs}
    formal_phases = FORMAL_AB if observed_phases.intersection(FORMAL_AB) else FORMAL
    layout_issues = (["Mixed AB and ABA phases cannot form one formal100 campaign"]
                     if observed_phases.intersection(FORMAL_AB) and observed_phases.intersection(FORMAL) else [])
    by_phase = {phase: [r for r in runs if r["phase"] == phase] for phase in formal_phases}
    duplicate_points = []
    point_keys = collections.Counter((r["phase"], a["block"], a["condition"]) for r, a in samples if r["series"] == "baseline100")
    for point, count in sorted(point_keys.items()):
        if count > 1:
            duplicate_points.append({"phase": point[0], "block": point[1], "condition": point[2], "count": count})
    pool_issues = []
    first, last = by_phase.get("ds50-first", []), by_phase.get("ds50-last", [])
    if len(first) == len(last) == 1:
        a, b = first[0], last[0]
        if a["identity"]["bit_sha256"] == b["identity"]["bit_sha256"] != "unknown" and a["identity"]["kernel_id"] == b["identity"]["kernel_id"] != "unknown" and not duplicate_points:
            for condition in PHASES["ds50-first"][1]:
                items = [(run, trial) for run, trial in samples if run["phase"] in {"ds50-first", "ds50-last"} and trial["condition"] == condition]
                if items:
                    groups.append(aggregate(items, group_id="ds50-explicit-pooled-"+condition,
                        series="baseline100", ds="50/50", condition=condition,
                        scope="pooled_first_and_last_with_explicit_boots", phases=["ds50-first", "ds50-last"]))
        else:
            pool_issues.append("DS50 pooling blocked: unknown/different BIT or kernel release, or duplicate phase/block/condition")
    elif first or last:
        pool_issues.append("DS50 pooling unavailable: exactly one first and one last run required")
    formal_boots = sorted({b for r in formal_runs for b in r["observed_boot_ids"]})
    formal = {"planned_points": 100, "phase_expected_points": {p: PHASES[p][0]*len(PHASES[p][1]) for p in formal_phases},
              "phase_run_counts": {p: len(by_phase[p]) for p in formal_phases},
              "accepted_points": sum(r["accepted_points"] for r in formal_runs),
              "attempted_points_with_evidence": sum(r["attempted_points_with_evidence"] for r in formal_runs),
              "failed_or_unretained_points": sum(r["failed_or_unretained_points"] for r in formal_runs),
              "boot_ids": formal_boots, "observed_boot_count": len(formal_boots),
              "independent_restart_repetitions": None, "duplicate_points": duplicate_points,
              "pooling_issues": pool_issues,
              "complete": all(len(by_phase[p]) == 1 and by_phase[p][0]["phase_complete"] for p in formal_phases) and not duplicate_points and not pool_issues and not layout_issues}
    if formal_phases == FORMAL_AB:
        formal.update(layout="AB_50_50", layout_issues=layout_issues)
    summary = {"schema_version": 1, "name": name, "fixture_only": any(r["fixture_only"] for r in runs),
               "software_evidence": evidence, "software_payload_equivalence": "not_inferred",
               "missing_inputs": sorted(set(reader.missing)), "runs": runs, "groups": groups,
               "formal100": formal, "g1": {"series_is_separate": True,
                   "run_ids": [r["run_id"] for r in runs if r["phase"] == "g1"],
                   "accepted_points": sum(r["accepted_points"] for r in runs if r["phase"] == "g1"),
                   "all_named_runs_passed": all(r["phase_complete"] for r in runs if r["phase"] == "g1") if any(r["phase"] == "g1" for r in runs) else None}}
    sweep_runs = [r for r in runs if r["series"] == "ds_sweep"]
    if sweep_runs:
        sweep_by_phase = {p: [r for r in sweep_runs if r["phase"] == p] for p in DS_SWEEP}
        sweep_points = collections.Counter((r["ds"], a["block"], a["condition"])
            for r in sweep_runs for a in r["attempts"] if a["accepted"])
        sweep_duplicates = [{"ds": ds, "block": block, "condition": condition, "count": count}
            for (ds, block, condition), count in sorted(sweep_points.items()) if count > 1]
        summary["ds_sweep"] = {
            "series_is_separate_from_formal100": True, "planned_points": 100,
            "phase_run_counts": {p: len(sweep_by_phase[p]) for p in DS_SWEEP},
            "accepted_points": sum(r["accepted_points"] for r in sweep_runs),
            "attempted_points_with_evidence": sum(r["attempted_points_with_evidence"] for r in sweep_runs),
            "failed_or_unretained_points": sum(r["failed_or_unretained_points"] for r in sweep_runs),
            "duplicate_points": sweep_duplicates,
            "complete": all(len(sweep_by_phase[p]) == 1 and sweep_by_phase[p][0]["phase_complete"]
                            for p in DS_SWEEP) and not sweep_duplicates}
    summary_path = relative(output / "summary.json")
    rows = []
    for ri, run in enumerate(runs):
        for ai, attempt in enumerate(run["attempts"]):
            status = "accepted" if attempt["accepted"] else "failed_or_unretained"
            for metric, value in sorted(attempt["metrics"].items()):
                source = value["source"]
                direct = source["exists"] and value["value_status"] != "not_applicable" and (not value["operation"] or source.get("matches_value"))
                path = source["raw_path"] if direct else summary_path
                ptr = source["source_locator"] if direct else tables.pointer("runs", ri, "attempts", ai, "metrics", metric, "value")
                condition = f"role=V0-A1;ds={run['ds']};mode={attempt['condition']};phase={run['phase']};run={run['run_id']};block={attempt['block']};attempt_status={status};campaign_status={run['manifest_status']};phase_complete={int(run['phase_complete'])};window={window(metric)};boot={attempt['boot_id']}"
                value_status = value["value_status"]
                if not attempt["accepted"] and value["value"] is not None:
                    value_status = "failed_observation"
                rows.append(row_spec(f"board_{run['series']}.{tables.slug(name)}.{run['run_id']}.{attempt['attempt_id']}.{metric}.sample",
                    run["series"], condition, metric, "sample", value["unit"], 1, path, ptr,
                    boot=attempt["boot_id"], sample_set=run["phase"]+":"+status,
                    identity=run["identity"], value_status=value_status))
    for gi, group in enumerate(groups):
        identity = {"bit_sha256": group["bit_ids"][0] if len(group["bit_ids"]) == 1 else "unknown",
                    "kernel_id": group["kernel_ids"][0] if len(group["kernel_ids"]) == 1 else "unknown",
                    "identity_status": "explicit_multi_boot_aggregation" if len(group["boot_ids"]) > 1 else "see_run_load_identity"}
        boot = group["boot_ids"][0] if len(group["boot_ids"]) == 1 else "mixed:"+",".join(group["boot_ids"])
        for metric, stats in sorted(group["metrics"].items()):
            source_status = ",".join(sorted(set(group["source_run_status"].values())))
            qualified = int(all(group["source_phase_complete"].values()))
            condition = f"role=V0-A1;ds={group['ds']};mode={group['condition']};phases={','.join(group['phases'])};aggregation={group['aggregation']};campaign_status={source_status};source_phases_complete={qualified};trials_observed={group['n_trials']};metric_missing={stats['n_unknown_or_not_applicable']};observed_boot_count={group['observed_boot_count']};window={window(metric)};boot={boot}"
            for statistic in ("min", "median", "max"):
                rows.append(row_spec(f"board_{group['series']}.{tables.slug(name)}.{group['group_id']}.{metric}.{statistic}",
                    group["series"], condition, metric, statistic, stats["unit"], stats["n"], summary_path,
                    tables.pointer("groups", gi, "metrics", metric, statistic), boot=boot,
                    sample_set=group["aggregation"], identity=identity,
                    value_status="derived" if stats[statistic] is not None else "unknown"))
    ids = [r["row_id"] for r in rows]
    if len(ids) != len(set(ids)):
        raise ValueError("Duplicate scalar row_id in import packet")
    summary_text = json_text(summary)
    for source in reader.files.values():
        if sha_bytes((ROOT/source["raw_path"]).read_bytes()) != source["sha256"]:
            raise ValueError("Input changed while preparing import: "+source["raw_path"])
    source_files = [reader.files[p] for p in sorted(reader.files)]+[{"raw_path": summary_path, "sha256": sha_bytes(summary_text.encode())}]
    config = {"schema_version": 1, "rows": sorted(rows, key=lambda r: r["row_id"]),
              "source_files": source_files, "import_name": name, "fixture_only": summary["fixture_only"]}
    csv_rows = []
    cache = {summary_path: summary}
    for spec in config["rows"]:
        path = spec["raw_path"]
        if path not in cache:
            cache[path] = json.loads((ROOT / path).read_text())
        value = tables.locate(cache[path], spec["source_locator"])
        if value is not None and not number(value):
            raise ValueError("Non-scalar value in long table: " + spec["row_id"])
        csv_rows.append(dict(spec, value="unknown" if value is None else value))
    files = {"summary.json": summary_text, "incremental_sources.json": json_text(config),
             "board_results.csv": tables.csv_text(csv_rows)}
    manifest = {"schema_version": 1, "generator_sha256": sha_bytes(Path(__file__).read_bytes()),
                "name": name, "fixture_only": summary["fixture_only"], "input_files": source_files[:-1],
                "missing_inputs": summary["missing_inputs"], "scalar_rows": len(rows),
                "formal100": formal, "g1": summary["g1"],
                "output_sha256": {p:sha_bytes(s.encode()) for p,s in files.items()}}
    if "ds_sweep" in summary:
        manifest["ds_sweep"] = summary["ds_sweep"]
    files["manifest.json"] = json_text(manifest)
    return files, config, summary


def main():
    global ROOT
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-root", type=Path, default=ROOT, help="Root containing all named raw evidence and the task output directory")
    parser.add_argument("--run", action="append", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True, help="New snapshot directory under paper/ispass2027/exp/E1b-ds-sweep")
    parser.add_argument("--name", required=True, help="Stable namespace for row IDs, e.g. baseline100-20261017")
    parser.add_argument("--software-evidence", action="append", type=Path, default=[])
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    ROOT = args.source_root.resolve()
    files, config, summary = prepare(args.run, args.output, args.name, args.software_evidence)
    if args.check or args.output.exists():
        if not args.check:
            raise ValueError("Snapshot output already exists; use --check to verify it without overwriting")
        for name, content in files.items():
            if (args.output/name).read_bytes() != content.encode():
                raise ValueError("Deterministic rebuild differs: "+name)
    else:
        args.output.mkdir(parents=True, exist_ok=False)
        for name, content in files.items():
            (args.output/name).write_text(content)
    result = {"status": "passed", "mode": "check" if args.check else "prepare",
        "installed": False, "scalar_rows": len(config["rows"]),
        "formal100_complete": summary["formal100"]["complete"],
        "formal100_accepted_points": summary["formal100"]["accepted_points"],
        "g1_accepted_points": summary["g1"]["accepted_points"],
        "fixture_only": summary["fixture_only"]}
    if "ds_sweep" in summary:
        result.update(ds_sweep_complete=summary["ds_sweep"]["complete"],
                      ds_sweep_accepted_points=summary["ds_sweep"]["accepted_points"])
    print(json.dumps(result, ensure_ascii=False))


if __name__ == "__main__":
    main()
