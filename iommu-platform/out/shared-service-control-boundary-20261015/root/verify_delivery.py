#!/usr/bin/env python3
"""Check only this new delivery, source quotations, and preserved originals.

No model, HDL, old checker, trace parser, experiment, or build is executed.
"""
from pathlib import Path
import hashlib
import json
import re


PACK = Path(__file__).resolve().parents[1]
PLATFORM = PACK.parents[1]
WORK = PLATFORM.parent
checks = []
cache = {}


def raw(path):
    path = Path(path)
    if path not in cache:
        cache[path] = path.read_bytes()
    return cache[path]


def digest(path):
    return hashlib.sha256(raw(path)).hexdigest()


def obj(path):
    return json.loads(raw(path))


def check(name, condition):
    checks.append({"check": name, "passed": bool(condition)})


for path in sorted(PACK.rglob("*.json")):
    if path.name == "delivery-verification.json":
        continue
    try:
        obj(path)
        check("JSON " + str(path.relative_to(PACK)), True)
    except (ValueError, OSError):
        check("JSON " + str(path.relative_to(PACK)), False)

before = obj(PACK / "before/manifest.json")
check("six exact before archives", len(before["files"]) == 6)
for row in before["files"]:
    check("archive " + Path(row["path"]).name,
          digest(row["archive"]) == row["sha256"])
for row in obj(PACK / "before/protected-artifacts.json")["files"]:
    check("old raw preserved " + row["path"],
          digest(row["path"]) == row["sha256"])

for row in obj(PACK / "root/critical-source-evidence.json")["excerpts"]:
    data = raw(row["path"])
    physical = data.decode().split("\n")
    text = "\n".join(physical[row["start_line"] - 1:row["end_line"]])
    check("root original hash " + row["id"],
          digest(row["path"]) == row["sha256"])
    check("root LF physical quotation " + row["id"],
          text == row["text"] and
          hashlib.sha256(text.encode()).hexdigest() == row["excerpt_sha256"])

for side in ("native", "controls"):
    for row in obj(PACK / side / "evidence.json")["sources"]:
        physical = raw(row["path"]).decode().split("\n")
        check(side + " original hash " + row["id"],
              digest(row["path"]) == row["sha256"])
        for index, quote in enumerate(row.get("excerpts", [])):
            start, end = quote["start"], quote["end"]
            plain = "\n".join(physical[start - 1:end])
            numbered = "\n".join(f"{start + i}: {line}"
                                  for i, line in enumerate(physical[start - 1:end]))
            expected = quote.get("original", quote.get("text"))
            check(f"{side} quotation {row['id']} {index}",
                  expected == (plain if "original" in quote else numbered))
        for index, tokens in enumerate(row.get("path_tokens", [])):
            line = physical[tokens["line"] - 1]
            check(f"selected source tokens {row['id']} {index}",
                  hashlib.sha256(line.encode()).hexdigest() ==
                  tokens["full_original_line_sha256"] and
                  all(token in line.split() for token in tokens["original_tokens"]))

evidence = {row["id"]: row for row in
            obj(PACK / "root/critical-source-evidence.json")["excerpts"]}
check("Intel correct paragraph meaning",
      "Root-Complex has finished fetching" in evidence["intel_drained_definition"]["text"] and
      "page-selective" in evidence["intel_page_domain_scope"]["text"] and
      "domain-selective" in evidence["intel_page_domain_scope"]["text"])
check("internal handoff plus downstream owners",
      "axi_comp_resp.ar_ready" in evidence["tc_internal_handoff"]["text"] and
      ".SpillAr        (1'b1)" in evidence["tc_following_spills"]["text"])
check("old first R event is handshake",
      "if (f_ddr_r &&" in evidence["legacy_first_R_is_handshake"]["text"])
diagnosis = obj(PACK / "errata/line-index-diagnosis.json")
check("index error established on unchanged original",
      diagnosis["source_hash_equals_old_recorded_hash"] and
      all(x["matches_splitlines"] and not x["matches_LF_lines"]
          for x in diagnosis["ranges"]))

native = obj(PACK / "native/findings.json")
check("two BITs not falsely equated", native["binding"]["same_bytes"] is False)
check("old aggregate does not supply target debt",
      native["observer"]["debt_capability"] is False)
check("observation not executed", native["execution"]["tests"] == 0 and
      native["execution"]["board_operations"] == 0)
classification = obj(PACK / "controls/control-classification.json")
classes = {row["id"]: row for row in classification["classes"]}
check("ordinary integration not deployed",
      classes["C2"]["can_legally_integrate_at_owner"] and
      not classes["C2"]["implemented"] and
      not classes["C2"]["timing_accepted"])
check("equivalent programs not claimed published",
      not classes["C5"]["independent_published_complete_baseline"] and
      not classes["C6"]["independent_published_complete_baseline"] and
      not classes["C6"]["native_adapter_cost_identity_proven"])
cost = obj(PACK / "root/full-cost.json")
check("all control and observation cost rows retained",
      cost["control_cost"]["categories"] == 40 and
      len(cost["observation_cost"]) == 10 and
      not cost["cost_directory_is_complete_numeric_cost_closure"] and
      all(value is None for value in cost["metrics"].values()))
next_decision = obj(PACK / "next-decision-contract.json")
check("single frozen observation and no inherited dual-head gate",
      not next_decision["generic_two_NormalTL_head_requirement"] and
      next_decision["frozen_request"]["a_pages"] == 8 and
      next_decision["frozen_request"]["v_gap_cycles"] == 0 and
      next_decision["frozen_request"]["V_transfers"] == 64 and
      not next_decision["frozen_request"]["is_full_fixed_both_task_performance_test"])
acceptance = obj(PACK / "root-result-acceptance.json")
check("no new prototype performance or paper-complete claim",
      not acceptance["current_prototype_gate"] and
      not acceptance["current_performance_gate"] and
      not acceptance["paper_complete"])
check("no create or update goal",
      acceptance["goal_operations"]["create"] == 0 and
      acceptance["goal_operations"]["update"] == 0)
goal = obj(PLATFORM / "out/local-read-reclaim-20261015/root/goal-completion.json")["goal"]
check("actual historic goal remains complete",
      goal["status"] == "complete" and goal["createdAt"] == 1791990424 and
      obj(PACK / "root/goal-state-after.json")["result"]["goal"] is None)

sync = obj(PACK / "root/state-sync.json")
check("five documents synchronized", len(sync["documents"]) == 5)
for row in sync["documents"] + [sync["LR1_report_prefix_only"]]:
    check("public synchronized " + row["path"], digest(row["path"]) == row["sha256"])
for current, archive in ((WORK / "HANDOFF.md", PACK / "before/HANDOFF.md"),
                         (PLATFORM / "docs/validation.md", PACK / "before/validation.md"),
                         (PLATFORM / "docs/local-read-reclaim-20261015.md",
                          PACK / "before/local-read-reclaim-20261015.md")):
    check("historical body preserved " + current.name,
          raw(current).endswith(raw(archive)))
marker = b"- \xe7\x8b\xac\xe7\xab\x8bLinux\xe8\xb4\xa1\xe7\x8c\xae"
old_current = raw(PACK / "before/CURRENT_STATUS.md")
check("independent current-status suffix preserved",
      raw(WORK / "CURRENT_STATUS.md").endswith(old_current[old_current.index(marker):]))

markdown = [PLATFORM / "docs/shared-service-control-boundary-20261015.md"]
markdown += [path for path in PACK.rglob("*.md")
             if "before" not in path.relative_to(PACK).parts and
             not path.name.endswith("-original.md")]
for path in markdown:
    prose = re.sub(r"```[\s\S]*?```", "", raw(path).decode())
    for target in re.findall(r"\[[^\]\n]+\]\(([^)\n]+)\)", prose):
        target = target.strip().strip("<>")
        if target.startswith(("https://", "http://", "#")):
            continue
        target = re.sub(r":\d+$", "", target.split("#", 1)[0])
        dest = Path(target) if target.startswith("/") else path.parent / target
        dest = dest.resolve()
        if dest == PACK / "root/delivery-verification.json":
            continue  # This invocation's output is created below.
        check("new Markdown target " + str(path.relative_to(PLATFORM)) + " " + target,
              dest.exists())

failed = [row for row in checks if not row["passed"]]
result = {"schema": "new-delivery-only-verification-v1",
          "status": "passed" if not failed else "failed",
          "checks_count": len(checks), "failed_count": len(failed),
          "scope": "new artifacts/quotes/semantic source boundaries/public sync/original preservation",
          "scientific_experiments_or_old_checkers_executed": 0,
          "checks": checks}
(PACK / "root/delivery-verification.json").write_text(
    json.dumps(result, ensure_ascii=False, indent=2) + "\n")
print(json.dumps({key: result[key] for key in
                  ("status", "checks_count", "failed_count")}, ensure_ascii=False))
if failed:
    print(json.dumps(failed[:12], ensure_ascii=False, indent=2))
raise SystemExit(1 if failed else 0)
