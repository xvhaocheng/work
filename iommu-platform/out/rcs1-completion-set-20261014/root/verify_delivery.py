#!/usr/bin/env python3
"""New delivery integrity only. Never executes model or old validators."""
import hashlib
import json
import pathlib
import re

O = pathlib.Path(__file__).resolve().parents[1]
W = O.parents[2]
checks = []

def read(p):
    return json.loads(p.read_text())

def sha_bytes(b):
    return hashlib.sha256(b).hexdigest()

def ck(n, b):
    checks.append({"check":n,"pass":bool(b)})
    if not b:
        raise AssertionError(n)

a = read(O / "root-result-acceptance.json")
ck("stable close with positive action preserved, paper remains open", a["decision"] == "close_frozen_RCS1_complete_performance_hypothesis" and a["positive_design"]["legal_choice_difference"] and not a["paper_complete"])
ck("scope unchanged / no old replay or branch restoration", all(a["constraints"][k] == 0 for k in ["old_experiment_validator_trace_build_replay","closed_branches_restored","old_seats_restored","external_models","extra_warmup_search_pressure_capacity_walkers"]))
v = read(O / "root/event-verification.json")
ck("already completed new raw checks without repeating them", v["status"] == "passed" and v["passed"] == 308 and v["failed"] == 0 and v["totals"]["events"] == 116750)
gate = read(O / "root/entry-gate.json")
for f in gate["files"]:
    ck("frozen interface/control " + f["path"], sha_bytes((O / f["path"]).read_bytes()) == f["sha256"])
for f in read(O / "root/accepted-model-artifacts.json")["files"]:
    ck("accepted actual model file " + f["path"], sha_bytes((O / f["path"]).read_bytes()) == f["sha256"])
source = read(O / "root/source-evidence.json")
for f in source["files"]:
    path = pathlib.Path(f["path"])
    lines = path.read_text().splitlines()
    ck("source SHA " + path.name, sha_bytes(path.read_bytes()) == f["sha256"])
    for e in f["excerpts"]:
        expected = "\n".join(f'{i}: {lines[i-1]}' for i in range(e["start"],e["end"]+1))
        ck(f'excerpt {path.name}:{e["start"]}', expected == e["text"])
ck("sources12 / excerpts33", len(source["files"])==12 and sum(len(f["excerpts"]) for f in source["files"])==33)
for c in ["C1","C2","C3","C4"]:
    for pname, alias in [("RCS1","RCS1"),("SJF","SJF")]:
        r = read(O / f'model/{c}-{pname}.result.json')
        m = a["metrics"][c][alias]
        ck(c+pname+" stable result values", m["max"] == r["model_ticks"]["completion_max"] and m["sum"] == r["model_ticks"]["completion_sum"] and m["vector"] == [r["tasks"][s]["completion"] for s in a["metrics"]["vector_roles"]] and m["PTE_AR"] == r["actual_counts"]["PTE_AR"])
ck("all unmeasured physical costs null", all(a["costs"][k] is None for k in ["total_state_bits","area","Fmax","physical_CPU_cycles","net_hardware_gain","GPU_AI_application_gain"]))
root_runtime = read(O / "root/runtime-model-context.json")
ck("root actual Sol ultra native475000", root_runtime["latest_turn"]["model"] == "gpt-6.1-sol" and root_runtime["latest_turn"]["effort"] == "ultra" and root_runtime["latest_window"]["model_context_window"] == 475000 and not root_runtime["overrides_changed"])
runtime = read(O / "root/worker-runtime-model-context.json")
ck("two new exclusive actual Sol ultra native475000", len(runtime["agents"])==2 and all(g["latest_turn"]["model"] == "gpt-6.1-sol" and g["latest_turn"]["effort"] == "ultra" and g["latest_window"]["model_context_window"]==475000 for g in runtime["agents"]))
sync = read(O / "root/state-sync.json")
ck("five pages synced before complete", sync["before_actual_complete"] and len(sync["files"])==5)
for f in sync["files"]:
    p = W / f["path"]
    data = p.read_bytes()
    ck("synced current hash " + f["path"], sha_bytes(data) == f["after_sha256"])
    suffix = data[f["prefix_bytes"]:]
    ck("original history byte-preserved " + f["path"], len(suffix)==f["preserved_suffix_bytes"] and sha_bytes(suffix)==f["preserved_suffix_sha256"])
    ck("archive hash " + f["path"], sha_bytes(pathlib.Path(f["archive"]).read_bytes()) == f["before_sync_sha256"])
    ck("new status clearly contains rule and full close " + f["path"], "RCS1" in data[:f["prefix_bytes"]].decode() and "关闭" in data[:f["prefix_bytes"]].decode())
for p in [O / "README.md", O / "root/prior-and-closed-boundaries.md", O / "root/ownership-and-full-cost.md", W / "iommu-platform/docs/rcs1-completion-set-20261014.md"]:
    for uri in re.findall(r'\]\(([^)]+)\)',p.read_text()):
        if uri.startswith("https:"):
            continue
        ck("new local navigation " + str(p.name)+":"+uri, (p.parent/uri).exists())
nextc = read(O / "next-goal-contract.json")
ck("successor not automatically started or paper-complete", not nextc["automatically_started"] and not nextc["paper_complete"])
out = {"status":"passed","before_actual_complete":True,"kind":"new delivery/state/source/hash checks; no model or previously passed event-check execution","passed":len(checks),"failed":0,"checks":checks}
(O / "root/delivery-verification.json").write_text(json.dumps(out,indent=2)+"\n")
print(json.dumps({k:v for k,v in out.items() if k != "checks"}))
