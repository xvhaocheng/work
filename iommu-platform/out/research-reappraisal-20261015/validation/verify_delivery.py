#!/usr/bin/env python3
"""Check this delivery and preserved inputs without rerunning scientific cases."""
from pathlib import Path
from urllib.parse import unquote
import hashlib
import json
import re
import subprocess

OUT = Path(__file__).resolve().parents[1]
PLATFORM = OUT.parents[1]
WORK = PLATFORM.parent
RESULT = OUT / "validation/delivery-verification.json"
RESULT.write_text(json.dumps({"passed": False, "status": "checking"}) + "\n")
checks = []


def check(name, passed, details=None):
    checks.append({"name": name, "passed": bool(passed), "details": details})


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read(name):
    return json.loads((OUT / name).read_text())


def fingerprint_set(name, entries, base):
    failures = []
    for item in entries:
        path = Path(item["path"])
        if not path.is_absolute():
            path = base / path
        if not path.is_file() or sha(path) != item["sha256"]:
            failures.append(str(path))
    check(name, not failures, {"count": len(entries), "failures": failures})


json_paths = [p for p in OUT.rglob("*.json")
              if "before" not in p.relative_to(OUT).parts and p != RESULT]
json_failures = []
for path in json_paths:
    try:
        json.loads(path.read_text())
    except (ValueError, UnicodeError) as error:
        json_failures.append({"path": str(path), "error": str(error)})
check("new_JSON_artifacts_parse", not json_failures,
      {"count": len(json_paths), "failures": json_failures})

before = read("before/source-manifest.json")
fingerprint_set("before_snapshots_match_initial_fingerprints", before, OUT / "before")
protected = [x for x in before if x["path"] in {
    "AGENTS.md", "iommu-platform/AGENTS.md", "iommu-platform/sources.lock.json",
    "iommu-platform/out/mig-pte-debt-observation-20261015/root/goal-pause.json"}]
fingerprint_set("rules_lock_and_original_pause_receipt_unchanged", protected, WORK)

old_handoff = (OUT / "before/HANDOFF.md").read_bytes()
new_handoff = (WORK / "HANDOFF.md").read_bytes()
old_validation = (OUT / "before/iommu-platform/docs/validation.md").read_bytes()
new_validation = (PLATFORM / "docs/validation.md").read_bytes()
check("handoff_original_bytes_preserved", new_handoff.endswith(old_handoff))
check("validation_original_bytes_preserved", new_validation.startswith(old_validation))
tail_marker = "- 独立Linux贡献：".encode()
old_status = (OUT / "before/CURRENT_STATUS.md").read_bytes()
new_status = (WORK / "CURRENT_STATUS.md").read_bytes()
check("unrelated_CURRENT_STATUS_tail_preserved",
      tail_marker in old_status and tail_marker in new_status and
      old_status[old_status.index(tail_marker):] == new_status[new_status.index(tail_marker):])

a = read("literature/a-cards.json")["cards"]
b = read("literature/b-cards.json")["cards"]
check("seven_named_core_cards", {c["id"] for c in a + b} == {
    "bermuda2025", "mask2018", "walk_stealing2021", "orchestrated2023",
    "socdma2018", "accelerator_translation2017", "riscvsva2025"})
papers = []
for cards, base, prefix in [(a, PLATFORM, False), (b, WORK, True)]:
    for card in cards:
        for kind in ["pdf", "txt"]:
            papers.append({"path": str(base / card["local_" + kind]),
                           "sha256": card[("sha256_" + kind) if prefix else (kind + "_sha256")]})
fingerprint_set("fourteen_original_core_paper_files_unchanged", papers, WORK)
history = read("history/asset-ledger.json")
check("sixteen_history_assets", len(history["assets"]) == 16)
fingerprint_set("history_cited_sources_unchanged",
                history["cited_source_fingerprints"]["files"], WORK)
adjacent = read("analysis/adjacent/sources.json")
fingerprint_set("three_added_primary_manuscripts", [
    {"path": str(OUT / ("analysis/adjacent/" + item["id"] + ".pdf")),
     "sha256": item["sha256"]} for item in adjacent], WORK)
fingerprint_set("root_critical_source_excerpts_unchanged", read("analysis/source-evidence.json"), WORK)
fingerprint_set("original_compilation_inputs_unchanged_including_test_sources",
                read("validation/model/production-input-manifest.json"), WORK)

contract_sha = (OUT / "validation/model/contract.sha256").read_text().strip()
check("frozen_pilot_contract_fingerprint", contract_sha ==
      sha(OUT / "validation/model/contract.json") ==
      read("root-result-acceptance.json")["new_pilot"]["contract_sha256"])
pilot = read("validation/model/pilot-verification.json")
run_ledger = read("validation/model/run-ledger.json")
check("two_complete_new_episodes_recorded", pilot["passed"] and
      len(run_ledger) == 2 and all(x["exit_code"] == 0 for x in run_ledger))
check("raw_complete_trace_fingerprints_match_recorded_verification", all(
    sha(Path(run["trace"])) == verified["trace_sha256"]
    for run, verified in zip(run_ledger, pilot["episodes"])))
check("pilot_not_overclaimed", pilot["coverage_decision"] ==
      "stop_this_fixed_pilot_not_covered_no_sweep" and
      not pilot["nonempty_fresh_competition_covered"] and
      not pilot["held_late_DS_covered"] and
      pilot["four_task_level_predictions_status"] == "not_executed_not_accepted")
predictions = read("analysis/prospective-validation.json")
check("four_task_predictions_not_reported_as_passed", len(predictions["cases"]) == 4 and
      not predictions["gate_passed"] and all(not x["executed"] for x in predictions["cases"]))

component = read("validation/model/component-results.json")
check("four_component_expected_outcomes_recorded", component["passed"] and
      len(component["cases"]) == 4 and
      [x["exit_code"] for x in component["cases"]] == [0, 0, -6, -6])
check("component_expected_failures_have_exact_diagnostics", all(
    text in Path(case["log"]).read_text() for case, text in zip(component["cases"][2:], [
        "changed a previously offered held full packet", "ar-ds-first must be 0"])))
check("compiled_helper_unchanged", sha(OUT / "validation/model/research_ar_selector.sv") ==
      component["helper_sha256"])
fingerprint_set("first_component_attempt_preserved", [
    {"path": entry["preserved"], "sha256": entry["sha256"]}
    for entry in read("validation/model/component-attempt1-preservation.json")], WORK)
fingerprint_set("component_adapter_and_attempt_fingerprints",
                read("validation/model/component-execution-ledger.json")["artifacts"], WORK)

# Validate only new prose and the additions to historical logs.
prose = {p: p.read_text() for p in OUT.rglob("*.md")
         if "before" not in p.relative_to(OUT).parts}
for path in [WORK / "研究计划.md", WORK / "CURRENT_STATUS.md",
             PLATFORM / "docs/research-reappraisal-20261015.md",
             PLATFORM / "docs/research-execution-plan-20261011.md"]:
    prose[path] = path.read_text()
prose[WORK / "HANDOFF.md"] = new_handoff[:-len(old_handoff)].decode()
prose[PLATFORM / "docs/validation.md"] = new_validation[len(old_validation):].decode()
broken = []
local_links = 0
for source, body in prose.items():
    for target in re.findall(r"\[[^\]]*\]\(([^\n)]+)\)", body):
        target = target.strip()
        if target.startswith("<"):
            target = target[1:target.index(">")]
        else:
            target = target.split(' "', 1)[0]
        if re.match(r"^[a-zA-Z][a-zA-Z0-9+.-]*://", target) or target.startswith("#"):
            continue
        target = unquote(target.split("#", 1)[0])
        target = re.sub(r":\d+(?:[-–]\d+)?$", "", target)
        candidate = Path(target)
        if not candidate.is_absolute():
            candidate = source.parent / candidate
        local_links += 1
        if not candidate.exists():
            broken.append({"source": str(source), "target": target})
check("new_local_navigation_links", not broken,
      {"documents": len(prose), "links": local_links, "broken": broken})

git_result = subprocess.run(["git", "--no-optional-locks", "status", "--short"],
                            cwd=PLATFORM, capture_output=True, text=True, check=True)
(OUT / "validation/platform-git-status-after.txt").write_text(git_result.stdout)
prior = set((OUT / "before/platform-git-status.txt").read_text().splitlines())
after = set(git_result.stdout.splitlines())
added, removed = sorted(after - prior), sorted(prior - after)
allowed = {"docs/research-reappraisal-20261015.md"}
check("git_status_preserves_prior_dirty_entries", not removed and all(
    line[3:] in allowed for line in added), {"added": added, "removed": removed})

result = {
    "schema": "research-reappraisal-delivery-verification-v1",
    "date": "2026-10-15",
    "scope": "文件、链接、原始输入保留与实际结果一致性；没有重跑模型/组件/旧validator或板测",
    "passed": all(x["passed"] for x in checks),
    "checks": checks,
    "scientific_status": "A方法继续发展/完整验证入口重设计；独立新意与四项任务级预测未接受；B1备选重设计",
}
RESULT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n")
print(json.dumps({"passed": result["passed"], "checks": len(checks),
                  "failed": [x for x in checks if not x["passed"]]}, ensure_ascii=False))
raise SystemExit(0 if result["passed"] else 1)
