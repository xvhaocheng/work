#!/usr/bin/env python3
"""Verify current deliverables and preservation; do not execute scientific cases."""
from pathlib import Path
from urllib.parse import unquote
import hashlib
import json
import re
import subprocess

OUT = Path(__file__).resolve().parents[1]
PLATFORM = OUT.parents[1]
WORK = PLATFORM.parent
RESULT = OUT / "root/delivery-verification.json"
RESULT.write_text(json.dumps({"passed": False, "status": "checking"}) + "\n")
checks = []
sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
read = lambda name: json.loads((OUT / name).read_text())


def check(name, passed, details=None):
    checks.append({"name": name, "passed": bool(passed), "details": details})


def fingerprints(name, entries, base):
    errors = []
    for entry in entries:
        path = Path(entry["path"])
        if not path.is_absolute():
            path = base / path
        if not path.is_file() or sha(path) != entry["sha256"]:
            errors.append(str(path))
    check(name, not errors, {"files": len(entries), "failures": errors})


bad_json = []
json_paths = [path for path in OUT.rglob("*.json")
              if "before" not in path.relative_to(OUT).parts and path != RESULT]
for path in json_paths:
    try:
        json.loads(path.read_text())
    except (ValueError, UnicodeError) as error:
        bad_json.append({"path": str(path), "error": str(error)})
check("new_JSON_artifacts", not bad_json, {"count": len(json_paths), "errors": bad_json})
before = read("before/source-manifest.json")
fingerprints("before_snapshots_preserved", before, OUT / "before")
protected = [entry for entry in before if entry["path"] in {
    "AGENTS.md", "iommu-platform/AGENTS.md", "iommu-platform/sources.lock.json",
    "iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json"}]
fingerprints("rules_lock_and_MIG_pause_receipt_unchanged", protected, WORK)
fingerprints("root_critical_original_sources_unchanged", read("inputs/root-critical-evidence.json"), WORK)
recent = read("inputs/recent-contracts.json")
fingerprints("recent_accepted_original_sources_unchanged", [
    {"path": entry["absolute_path"], "sha256": entry["sha256"]}
    for entry in recent["sources"].values()], WORK)

old_h = (OUT / "before/HANDOFF.md").read_bytes()
new_h = (WORK / "HANDOFF.md").read_bytes()
old_v = (OUT / "before/iommu-platform/docs/validation.md").read_bytes()
new_v = (PLATFORM / "docs/validation.md").read_bytes()
check("historical_handoff_original_bytes_retained", new_h.endswith(old_h))
check("historical_validation_original_bytes_retained", new_v.startswith(old_v))
marker = "- 独立Linux贡献：".encode()
old_s = (OUT / "before/CURRENT_STATUS.md").read_bytes()
new_s = (WORK / "CURRENT_STATUS.md").read_bytes()
check("unrelated_status_tail_unchanged", old_s[old_s.index(marker):] == new_s[new_s.index(marker):])

contract_sha = (OUT / "cases/frozen-contract.sha256").read_text().strip()
check("frozen_case_contract_unchanged", sha(OUT / "cases/frozen-contract.json") == contract_sha)
pred = read("method/predictions.json")
ledger = read("validation/semantic-execution-ledger.json")
check("predictions_and_executor_unchanged_since_execution",
      ledger["contract_sha256"] == contract_sha and
      ledger["analysis_predictions_sha256"] == sha(OUT / "method/predictions.json") and
      ledger["executor_sha256"] == sha(OUT / "validation/semantic_executor.py"))
check("all_four_complete_time_results_remain_unknown",
      pred["all_full_time_predictions_unknown"] and not pred["timed_execution_admitted"] and
      len(pred["cases"]) == 4 and all(x["full_completion_time_prediction"] is None for x in pred["cases"]))
summary = read("validation/semantic-runs/summary.json")
events = read("validation/source-event-verification.json")
check("single_bounded_eight_source_runs_and_independent_raw_acceptance",
      ledger["exit_code"] == 0 and summary["executed_runs"] == summary["requested_runs"] == 8 and
      summary["pass"] and events["passed"] and events["verified_complete_source_episodes"] == 8,
      {"source_episodes": 8, "raw_events": sum(x["raw_records"] for x in events["cases"])})
fingerprints("raw_source_events_unchanged_after_independent_verification", [
    {"path": str(OUT / "validation/semantic-runs" / item["case"] / item["policy"] / "events.jsonl"),
     "sha256": item["event_log_sha256"]} for item in events["cases"]], WORK)
check("no_timing_bus_or_native_claims_in_execution_results", all(
    item["timing"] is None and item["CPU_time"] is None and
    item["AR_R_AW_W_B_counts"] is None and not item["native_DDR_or_RTL_validation"] and
    not item["performance_prediction_accepted"] for item in summary["results"]))
accept = read("root-result-acceptance.json")
check("current_acceptance_matches_frozen_scope", not accept["novelty"]["accepted_as_independent_paper_contribution"] and
      accept["actual_execution"]["complete_source_semantic_episodes"] == 8 and
      accept["actual_execution"]["timed_or_RTL_episodes"] == 0 and
      not read("cases/frozen-contract.json")["old_four_timing_predictions_accepted_by_this_contract"])
corrections = read("root/method-scope-corrections.json")
fingerprints("scope_correction_current_versions_recorded", [
    {"path": str(OUT / name), "sha256": digest}
    for name, digest in corrections["current_artifact_hashes"].items()], WORK)

prose = {path: path.read_text() for path in OUT.rglob("*.md")
         if "before" not in path.relative_to(OUT).parts}
for path in [WORK / "研究计划.md", WORK / "CURRENT_STATUS.md",
             PLATFORM / "docs/control-position-method-20261015.md",
             PLATFORM / "docs/research-execution-plan-20261011.md"]:
    prose[path] = path.read_text()
prose[WORK / "HANDOFF.md"] = new_h[:-len(old_h)].decode()
prose[PLATFORM / "docs/validation.md"] = new_v[len(old_v):].decode()
broken, local_count = [], 0
for source, text in prose.items():
    for target in re.findall(r"\[[^\]]*\]\(([^\n)]+)\)", text):
        target = target.strip()
        target = target[1:target.index(">")] if target.startswith("<") else target.split(' "', 1)[0]
        if re.match(r"^[a-zA-Z][a-zA-Z0-9+.-]*://", target) or target.startswith("#"):
            continue
        target = re.sub(r":\d+(?:[-–]\d+)?$", "", unquote(target.split("#", 1)[0]))
        path = Path(target)
        if not path.is_absolute():
            path = source.parent / path
        local_count += 1
        if not path.exists():
            broken.append({"source": str(source), "target": target})
check("new_navigation_links", not broken,
      {"documents": len(prose), "local_links": local_count, "broken": broken})
status = subprocess.run(["git", "--no-optional-locks", "status", "--short"],
                        cwd=PLATFORM, text=True, capture_output=True, check=True).stdout
(OUT / "root/platform-git-status-after.txt").write_text(status)
prior = set((OUT / "before/platform-git-status.txt").read_text().splitlines())
after = set(status.splitlines())
check("prior_dirty_entries_retained_and_only_new_report_entry", not (prior - after) and
      all(line[3:] == "docs/control-position-method-20261015.md" for line in after - prior),
      {"added": sorted(after - prior), "removed": sorted(prior - after)})
result = {"schema": "A1-method-delivery-verification-v1", "date": "2026-10-15",
          "passed": all(c["passed"] for c in checks), "checks": checks,
          "scope": "交付/原始输入保留与实际结果一致性；未重跑source执行器、旧validator、时延/RTL/板测",
          "source_semantics_accepted": True, "timing_or_native_accepted": False}
RESULT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n")
print(json.dumps({"passed": result["passed"], "checks": len(checks),
                  "failed": [c for c in checks if not c["passed"]]}, ensure_ascii=False))
raise SystemExit(0 if result["passed"] else 1)
