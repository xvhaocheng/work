#!/usr/bin/env python3
"""Static identity/index checks only. Does not execute a build or analyzer."""
from __future__ import annotations

import ast
import csv
import hashlib
import json
from datetime import datetime, timezone
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]


def digest(p):
    h=hashlib.sha256()
    with p.open("rb") as f:
        for b in iter(lambda:f.read(1024*1024),b""):
            h.update(b)
    return h.hexdigest()


def main():
    checks=[]
    def check(name, condition, detail=None):
        checks.append({"name":name,"passed":bool(condition),"detail":detail})
    for p in [HERE/"collect.py",Path(__file__)]:
        ast.parse(p.read_text(), filename=str(p))
        check("python_syntax:"+p.name,True)
    index=json.loads((HERE/"index.json").read_text())
    inventory=json.loads((HERE/"evidence-files.json").read_text())["files"]
    core=json.loads((HERE/"core-build-sources.json").read_text())["builds"]
    rows=index["records"]
    check("record_ids_unique", len({r["experiment_id"] for r in rows})==len(rows),len(rows))
    check("record_count",index["record_count"]==len(rows))
    required={"experiment_id","date","evidence_layer","report","output_dir","input_commits","source_manifests","overlay","config","known_extension_flags","baseline_classification","reconstruction_status","feature_mapping","new_commits","unknowns"}
    check("record_schema",all(required<=r.keys() for r in rows))
    check("reports_and_outputs_exist",all((ROOT/r["report"]["path"]).is_file() and (ROOT/r["output_dir"]).is_dir() for r in rows))
    check("no_retroactive_stock_certification",all(r["stock_baseline_completed"] is False for r in rows))
    check("profile_inventory_separate",sum(r["experiment_id"].startswith("PROFILE-") for r in rows)==17)
    check("first_pte_successful_cases",sum(r["experiment_id"].startswith("FIRST-PTE-") and r["evidence_layer"]=="finite_RTL_behavioral_memory_run" for r in rows)==6)
    check("discovery_case_count",sum(r["experiment_id"].startswith("DISCOVERY-") for r in rows)==8)
    check("plru_primary_case_count",sum(r["experiment_id"].startswith("PLRU-") for r in rows)==18)
    check("plru_attempt_metadata_count",sum(len(r.get("execution_metadata",[])) for r in rows if r["experiment_id"].startswith("PLRU-"))==24)
    errors=[]
    for item in inventory:
        p=ROOT/item["path"]
        if not item["exists"] or not p.is_file() or digest(p)!=item["sha256"]:
            errors.append(item["path"])
        if item.get("snapshot"):
            s=ROOT/item["snapshot"]
            if not s.is_file() or digest(s)!=item["sha256"]:
                errors.append("snapshot:"+item["snapshot"])
    check("original_metadata_and_snapshots_match",not errors,{"files":len(inventory),"errors":errors})
    bypath={x["path"]:x for x in inventory}
    refs=[]
    for r in rows:refs += [r["report"]]+r["source_manifests"]+r["overlay"]
    check("row_references_match_inventory",all(x["path"] in bypath and x["sha256"]==bypath[x["path"]]["sha256"] for x in refs))
    source_errors=[]
    source_count=0
    distinct=set()
    passed_build_counts={}
    for name,b in core.items():
        sources=b["explicit_sources"]
        source_count += len(sources)
        if b["metadata"].get("state")=="passed":
            passed_build_counts[name]={"entries":len(sources),"available_blobs":sum(bool(s.get("source_blob")) for s in sources)}
        for s in sources:
            blob=s.get("source_blob")
            if blob:
                p=ROOT/blob
                distinct.add(blob)
                if not p.is_file() or digest(p)!=s["sha256"]:source_errors.append(blob)
            elif s["recorded_bytes_match_at_index"]:source_errors.append("missing_blob:"+s["path"])
        if b["counts"]["explicit_sv"]+b["counts"]["cpp"]!=len(sources):source_errors.append("count:"+name)
    check("explicit_source_blobs_match_recorded_hashes",not source_errors,{"entries":source_count,"distinct_blobs":len(distinct),"errors":source_errors})
    check("passed_runtime_build_explicit_sources_available",all(x["entries"]==x["available_blobs"]==86 for name,x in passed_build_counts.items() if "/runner/" not in name and "/lint" not in name),passed_build_counts)
    with (HERE/"index.csv").open(newline="") as f:csv_rows=list(csv.DictReader(f))
    check("index_csv_matches_json_ids",[x["experiment_id"] for x in csv_rows]==[x["experiment_id"] for x in rows])
    with (HERE/"core-build-sources.csv").open(newline="") as f:csv_sources=list(csv.DictReader(f))
    check("source_csv_count",len(csv_sources)==source_count)
    feature_maps={}
    for c,m in index.get("organized_component_manifests",{}).items():
        j=json.loads((ROOT/m["path"]).read_text())
        feature_maps[c]={s["feature_id"]:s["commit"] for s in j["steps"]}
    linked=[n for r in rows for n in r["new_commits"]]
    check("new_commits_match_component_manifest_feature_ids",all(feature_maps.get(n["component"],{}).get(n["feature_id"])==n["commit"] and n["historical_source_equivalence"]=="not_asserted" for n in linked),len(linked))
    check("all_declared_feature_associations_resolved",all(len(r["feature_mapping"])==len(r["new_commits"]) for r in rows))
    lock_recovery=json.loads((HERE/"historical-lock-recovery.json").read_text())["records"]
    check("historical_git_source_locks_match_manifest_hashes",all(x["matches_manifest"] and digest(ROOT/x["snapshot"])==x["sha256"]==x["expected_sha256"] for x in lock_recovery),len(lock_recovery))
    result={"schema":"history-static-verification-v1","checked_utc":datetime.now(timezone.utc).isoformat(),"status":"passed" if all(x["passed"] for x in checks) else "failed","checks":checks,"index_sha256":digest(HERE/"index.json"),"core_sources_sha256":digest(HERE/"core-build-sources.json"),"source_coverage_limit":"Explicitly listed source bytes only; implicit includes/environment/toolchain closure is not certified. Historical failed preparation source revisions missing from listed paths remain hash-only.","scientific_execution":{"new_experiments":0,"new_builds":0,"new_data_reductions":0,"board_operations":0}}
    (HERE/"verification.json").write_text(json.dumps(result,ensure_ascii=False,indent=2)+"\n")
    print(json.dumps({"status":result["status"],"checks":len(checks),"failed":[c["name"] for c in checks if not c["passed"]],"records":len(rows),"explicit_source_entries":source_count,"distinct_source_blobs":len(distinct)},ensure_ascii=False))
    if result["status"]!="passed":raise SystemExit(1)


if __name__=="__main__":main()
