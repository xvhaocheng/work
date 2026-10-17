#!/usr/bin/env python3
"""Check historical identity sidecars without executing builds or experiments.

The original collector and verification receipt remain untouched. This checker
reads cited files, resolves JSON pointers and checks Git objects where available.
It does not certify an unrecorded source closure or reproduce historical results.
"""
from __future__ import annotations

import ast
import csv
import hashlib
import json
import re
import subprocess
from datetime import datetime, timezone
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
DATASETS = (
    "historical-identities",
    "invalidation-identities",
    "supplemental-identities",
)


def digest(path):
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def pointer(document, value):
    if value == "":
        return document
    if not isinstance(value, str) or not value.startswith("/"):
        raise ValueError(f"not an RFC 6901 pointer: {value!r}")
    for part in value[1:].split("/"):
        part = part.replace("~1", "/").replace("~0", "~")
        document = document[int(part)] if isinstance(document, list) else document[part]
    return document


def walk(value, location=""):
    if isinstance(value, dict):
        yield location, value
        for key, child in value.items():
            yield from walk(child, location + "/" + str(key))
    elif isinstance(value, list):
        for number, child in enumerate(value):
            yield from walk(child, location + "/" + str(number))


def main():
    csv.field_size_limit(16 * 1024 * 1024)
    checks = []
    errors = []
    file_hashes = {}
    documents = {}
    datasets = {}
    pointers = 0
    absent_generated_artifacts = []
    invalid_commits = []

    def check(name, passed, detail=None):
        checks.append({"name": name, "passed": bool(passed), "detail": detail})

    def read_json(path):
        if path not in documents:
            documents[path] = json.loads(path.read_text())
        return documents[path]

    for script in HERE.glob("*identities.py"):
        ast.parse(script.read_text(), filename=str(script))
    check("identity_python_syntax", True)

    for name in DATASETS:
        document = read_json(HERE / (name + ".json"))
        datasets[name] = document
        records = document["records"]
        ids = [r["experiment_id"] for r in records]
        check(name + ":unique_ids", len(ids) == len(set(ids)), len(ids))
        with (HERE / (name + ".csv")).open(newline="") as stream:
            csv_ids = [r["experiment_id"] for r in csv.DictReader(stream)]
        expected_csv_ids = ids + [r["experiment_id"] for r in document.get("preparation_entries", [])]
        check(name + ":csv_ids", expected_csv_ids == csv_ids)
        for location, obj in walk(document):
            if "checkout_branch" in obj and (isinstance(obj.get("repository"), str) or isinstance(obj.get("path"), str)):
                for key in ("checkout_commit", "selected_commit", "selected_source_commit"):
                    if obj.get(key) is not None and not re.fullmatch(r"[0-9a-f]{40}", str(obj[key])):
                        invalid_commits.append({"dataset": name, "location": location + "/" + key,
                                                "value": obj[key]})
            if not isinstance(obj.get("path"), str) or not isinstance(obj.get("sha256"), str):
                continue
            # Git object citations use repository/commit/git_path rather than a
            # workspace path; only actual file references are processed here.
            if not re.fullmatch(r"[0-9a-f]{64}", obj["sha256"]):
                errors.append({"dataset": name, "location": location, "error": "invalid SHA256"})
                continue
            path = Path(obj["path"])
            path = path if path.is_absolute() else ROOT / path
            try:
                path.resolve().relative_to(ROOT)
                if not path.exists() and "local generated artifact" in obj.get("retention", ""):
                    absent_generated_artifacts.append(obj["path"])
                    continue
                if path not in file_hashes:
                    file_hashes[path] = digest(path)
                if file_hashes[path] != obj["sha256"]:
                    raise ValueError("file bytes differ from cited SHA256")
                if "json_pointer" in obj and obj["json_pointer"] is not None:
                    pointer(read_json(path), obj["json_pointer"])
                    pointers += 1
                line_numbers = obj.get("line_anchors", []) + ([obj["line"]] if obj.get("line") is not None else [])
                if line_numbers:
                    total_lines = len(path.read_text().splitlines())
                    if any(not isinstance(line, int) or line < 1 or line > total_lines for line in line_numbers):
                        raise ValueError("line citation outside file")
            except (OSError, ValueError, KeyError, IndexError, TypeError) as exc:
                errors.append({"dataset": name, "location": location, "path": obj["path"], "error": str(exc)})
    check("cited_file_hashes_and_json_pointers", not errors,
          {"distinct_files": len(file_hashes), "json_pointers": pointers,
           "absent_generated_artifacts_with_recorded_hash_only": absent_generated_artifacts, "errors": errors})
    check("checkout_and_selected_commits_are_full_or_null", not invalid_commits, invalid_commits)

    original = read_json(HERE / "index.json")
    check("all_79_original_index_records_covered",
          [r["experiment_id"] for r in original["records"]]
          == [r["experiment_id"] for r in datasets["historical-identities"]["records"]])
    check("28_invalidation_measurement_records",
          len(datasets["invalidation-identities"]["records"]) == 28)
    missing_field_reasons = []
    for record in datasets["historical-identities"]["records"]:
        for repo in record["repositories"]:
            for key in ("checkout_branch", "checkout_commit", "selected_ref", "selected_commit", "dirty"):
                if repo[key] is None and not repo["known_unknowns"].get(key):
                    missing_field_reasons.append((record["experiment_id"], repo["path"], key))
                elif repo[key] is not None and not repo["field_evidence"].get(key):
                    missing_field_reasons.append((record["experiment_id"], repo["path"], key + ":evidence"))
    check("historical_fields_have_evidence_or_unknown_reason", not missing_field_reasons, missing_field_reasons)

    # Recheck immutable metadata and saved explicit source bytes. This only reads
    # the old inventory; it does not rerun collect.py or overwrite its receipt.
    original_errors = []
    original_path_drift = []
    inventory = read_json(HERE / "evidence-files.json")["files"]
    for item in inventory:
        snapshot = item.get("snapshot")
        snapshot_matches = bool(snapshot and digest(ROOT / snapshot) == item["sha256"])
        if snapshot and not snapshot_matches:
            original_errors.append(snapshot)
        actual = digest(ROOT / item["path"]) if (ROOT / item["path"]).is_file() else None
        if actual != item["sha256"]:
            original_path_drift.append({"path": item["path"], "historical_sha256": item["sha256"],
                                        "current_sha256": actual, "preserved_snapshot": snapshot,
                                        "snapshot_matches": snapshot_matches})
            if not snapshot_matches:
                original_errors.append(item["path"])
    check("original_236_metadata_bytes_preserved", not original_errors,
          {"files": len(inventory), "current_path_drift_with_preserved_historical_bytes": original_path_drift,
           "errors": original_errors})
    core = read_json(HERE / "core-build-sources.json")["builds"]
    blobs = {}
    for build in core.values():
        for source in build["explicit_sources"]:
            if source.get("source_blob"):
                blobs[source["source_blob"]] = source["sha256"]
    source_errors = [path for path, expected in blobs.items() if digest(ROOT / path) != expected]
    check("original_explicit_source_blobs_unchanged", not source_errors,
          {"builds": len(core), "entries": sum(len(b["explicit_sources"]) for b in core.values()),
           "distinct_blobs": len(blobs), "errors": source_errors})

    # Directly verify the new connection between a recorded export commit and
    # the frozen input bytes; a current branch alias is not involved.
    inv = ROOT / "iommu-platform/out/invalidation-interference-20261017"
    manifest = read_json(inv / "build/build03/build-manifest.json")
    frozen = read_json(inv / "frozen/source-manifest.json")
    frozen_errors = []
    repos = {
        "cva6": ROOT / "iommu-platform/cva6",
        "iommu": ROOT / "iommu-platform/cva6/corev_apu/riscv-iommu",
        "register_interface": ROOT / "iommu-platform/cva6/corev_apu/register_interface",
        "axi_slice": ROOT / "iommu-platform/cva6/corev_apu/fpga/src/axi_slice",
    }
    prefixes = {name: str(repo.relative_to(ROOT / "iommu-platform")) + "/" for name, repo in repos.items()}
    for item in frozen["files"]:
        try:
            file_path = inv / "frozen" / item["logical_path"]
            if digest(file_path) != item["sha256"]:
                raise ValueError("frozen file hash mismatch")
            name = next(k for k in repos if frozen[k] == item["commit"])
            git_path = item["logical_path"].removeprefix(prefixes[name])
            actual_blob = subprocess.check_output(["git", "-C", str(repos[name]), "rev-parse",
                                                   item["commit"] + ":" + git_path], stderr=subprocess.PIPE, text=True).strip()
            if actual_blob != item["git_blob"]:
                raise ValueError("blob is not at the cited commit/path")
            data = subprocess.check_output(["git", "-C", str(repos[name]),
                                            "cat-file", "blob", item["git_blob"]], stderr=subprocess.PIPE)
            if hashlib.sha256(data).hexdigest() != item["sha256"]:
                raise ValueError("Git source blob hash mismatch")
        except (OSError, ValueError, StopIteration, subprocess.CalledProcessError) as exc:
            frozen_errors.append({"path": item["logical_path"], "error": str(exc)})
    check("frozen_bytes_equal_recorded_git_blobs", not frozen_errors,
          {"files": len(frozen["files"]), "errors": frozen_errors})
    input_errors = []
    for item in manifest["sources"] + [{"path": manifest["cpp"]["path"], "sha256": manifest["cpp"]["sha256"]}]:
        if digest(Path(item["path"])) != item["sha256"]:
            input_errors.append(item["path"])
    check("invalidation_build03_explicit_inputs", not input_errors,
          {"sources": len(manifest["sources"]), "cpp": 1, "errors": input_errors})
    run_errors = []
    cases = 0
    measured_ids = set()
    for name in ("matrix01", "sensitivity01"):
        campaign = read_json(inv / "runs" / name / "campaign.json")
        if campaign["build_manifest_sha256"] != digest(inv / "build/build03/build-manifest.json"):
            run_errors.append(name + ":build_manifest")
        if campaign["binary_sha256"] != manifest["executable"]["sha256"]:
            run_errors.append(name + ":binary")
        cases += len(campaign["cases"])
        measured_ids.update("invalidation-interference-20261017/" + name + "/" + c["config"]["case_id"]
                            for c in campaign["cases"])
    invalidation = datasets["invalidation-identities"]
    mapped = invalidation["records"]
    if measured_ids != {r["experiment_id"] for r in mapped}:
        run_errors.append("sidecar IDs differ from measured campaign IDs")
    for record in mapped:
        if record["build_inputs"]["build_id"] != "build03" or not record["is_measurement"]:
            run_errors.append(record["experiment_id"] + ":build_or_role")
        if record["repositories"]["context_id"] not in invalidation["repository_contexts"]:
            run_errors.append(record["experiment_id"] + ":unresolved_context")
    check("28_cases_use_recorded_build03", not run_errors and cases == 28,
          {"cases": cases, "errors": run_errors})

    result = {
        "schema": "historical-identity-static-verification-v1",
        "checked_utc": datetime.now(timezone.utc).isoformat(),
        "status": "passed" if all(c["passed"] for c in checks) else "failed",
        "checks": checks,
        "datasets": {name: {"path": str((HERE / (name + ".json")).relative_to(ROOT)),
                             "sha256": digest(HERE / (name + ".json")),
                             "records": len(document["records"])} for name, document in datasets.items()},
        "limits": ["Unknown historical branches and missing dirty/include/environment closures remain unknown.",
                   "Git object or saved source equality does not prove a historical build/run on an organized branch.",
                   "No compiler, simulation, analyzer, synthesis or board command is invoked."],
        "scientific_execution": {"new_experiments": 0, "new_builds": 0, "new_data_reductions": 0, "board_operations": 0},
    }
    (HERE / "identity-verification.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n")
    print(json.dumps({"status": result["status"], "checks": len(checks),
                      "records": {name: len(d["records"]) for name, d in datasets.items()},
                      "failed": [c["name"] for c in checks if not c["passed"]]}, ensure_ascii=False))
    if result["status"] != "passed":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
