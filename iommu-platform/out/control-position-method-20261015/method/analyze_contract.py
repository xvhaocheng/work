#!/usr/bin/env python3
"""A1 source-contract checker and symbolic completion analysis.

No source event index is a cycle. Missing protocol/service bindings produce UNKNOWN.
"""
from pathlib import Path
import argparse
import hashlib
import json


def graph(nodes):
    table = {n["id"]: n for n in nodes}
    if len(table) != len(nodes):
        raise ValueError("duplicate event identity")
    pending, order, done = list(table), [], set()
    for n in nodes:
        if not set(n["predecessors"]) <= table.keys():
            raise ValueError("missing generating predecessor: " + n["id"])
    while pending:
        ready = [name for name in pending if set(table[name]["predecessors"]) <= done]
        if not ready:
            raise ValueError("dependency cycle: unresolved ownership/resource progress")
        for name in ready:
            pending.remove(name)
            done.add(name)
            order.append(name)
    ancestors = {}
    for name in order:
        ancestors[name] = set(table[name]["predecessors"])
        for parent in table[name]["predecessors"]:
            ancestors[name].update(ancestors[parent])
    return table, order, ancestors


def analyze(case):
    table, order, ancestors = graph(case["nodes"])
    seen = set()
    for name in case["prefix"]:
        if name in seen or not set(table[name]["predecessors"]) <= seen:
            raise ValueError("illegal common source prefix: " + name)
        seen.add(name)
    enabled_publications = [name for name in order if name not in seen and
                            table[name]["kind"] == "publish" and
                            set(table[name]["predecessors"]) <= seen]
    terminal = ancestors["task_complete"] | {"task_complete"}
    if terminal != set(table):
        raise ValueError("task completion omits required events: " + str(set(table) - terminal))

    def require_before(left, right):
        if left not in ancestors[right]:
            raise ValueError("missing ownership/completion edge: " + left + " -> " + right)

    for left, right in [("K01", "pub_C0"), ("K11", "wait_C0"),
                        ("done_C0", "pub_C1"), ("done_C1", "free_six"),
                        ("free_six", "map_out"), ("map_out_visibility", "host_compare"),
                        ("host_compare", "free_host"), ("free_host", "task_complete"),
                        ("pub_A1", "K00"), ("pub_B01", "K00"), ("pub_C0", "K10")]:
        require_before(left, right)
    if case["retain_B"]:
        for name in ("pub_B10", "done_B10", "pub_B11", "done_B11", "wait_B10", "wait_B11"):
            if name in table:
                raise ValueError("retention must not fabricate eliminated job/wait: " + name)
        require_before("wait_B00", "retain_check_B0")
        require_before("wait_B01", "retain_check_B1")
        require_before("retain_check_B0", "K10")
        require_before("retain_check_B1", "K11")
    else:
        require_before("K00", "pub_B10")
        require_before("K01", "pub_B11")
        require_before("pub_B10", "K01")
        require_before("pub_B11", "K10")

    jobs = [n for n in case["nodes"] if n["kind"] == "publish"]
    job_ids = {n["details"]["job_id"] for n in jobs}
    completions = {n["details"]["job_id"] for n in case["nodes"] if n["kind"] == "dma_complete"}
    waits = {n["details"]["job_id"] for n in case["nodes"]
             if n["kind"] == "wait" and "job_id" in n["details"]}
    if not job_ids == completions == waits:
        raise ValueError("publish/complete/real-wait identity mismatch")
    copied = sum(n["details"]["bytes"] for n in jobs)
    if len(jobs) != case["expected_jobs"] or copied != case["expected_dma_copied_bytes"]:
        raise ValueError("frozen useful work mismatch")

    committed = [n for n in case["prefix"] if table[n]["kind"] == "publish"]
    local_action = "UNKNOWN"
    certificate = {}
    if "pub_A1" in committed and "pub_B00" not in committed:
        local_action = "late_position_cannot_insert_B00_before_committed_A1"
        certificate = {"immutable_prefix": committed, "would_require": "withdrawing_or_reordering_source_publication"}
    elif {"pub_A1", "pub_B00"} <= set(enabled_publications):
        local_action = "ordinary_source_head_selection_suffices_for_the_local_order_choice"
        certificate = {"ready": enabled_publications,
                       "nonalias": [table[x]["details"]["slot"] for x in ["pub_A1", "pub_B00"]],
                       "independent": "pub_A1" not in ancestors["pub_B00"] and
                                      "pub_B00" not in ancestors["pub_A1"],
                       "legal_orders": [["pub_A0", "pub_A1", "pub_B00"],
                                        ["pub_A0", "pub_B00", "pub_A1"]]}
    if case.get("prefix_until"):
        event = case["prefix_until"]
        source_before = ancestors[event] | {event}
        remaining_publications = [n for n in order if table[n]["kind"] == "publish" and n not in source_before]
        local_action = "late_position_has_no_remaining_A_B_publication_choice"
        certificate = {"cut": event, "all_prior_A_B_publications_in_source_prefix": all(
            n["id"] in source_before for n in jobs if n["details"]["host_array"] in ["A", "B"]),
                       "remaining_publications": remaining_publications,
                       "C0_completion_mandatory_for_C1": "done_C0" in ancestors["pub_C1"],
                       "C0_must_have_completed_at_cut": "done_C0" in ancestors[event]}

    finish_equations = [{"event": name, "max_predecessor_finishes": table[name]["predecessors"],
                         "plus_duration_symbol": "d_" + name}
                        for name in order]
    return {
        "case": case["id"], "source_contract_checked": True,
        "node_count": len(table), "source_complete_ancestor_count": len(terminal),
        "enabled_unpublished_after_initial_short_prefix": enabled_publications,
        "initial_short_prefix_scope": "before prefix_until expansion; not the H2 intervention state",
        "local_source_order_decision": local_action, "action_certificate": certificate,
        "cut_enabled_publications": None if case.get("prefix_until") else enabled_publications,
        "cut_state_scope": "H2 requires actual generated prefix through K11; static ancestors alone do not determine C0 retirement or its ready set" if case.get("prefix_until") else "fixed initial prefix",
        "copy_jobs": len(jobs), "logical_copied_bytes": copied,
        "logical_external_read_bytes": sum(n["details"]["bytes"] for n in jobs if n["details"]["direction"] == "h2d"),
        "logical_external_write_bytes": sum(n["details"]["bytes"] for n in jobs if n["details"]["direction"] == "d2h"),
        "completion_goals": {"A_transport": ["done_A0", "done_A1"],
                             "B_transport": [n["id"] for n in case["nodes"] if n["kind"] == "dma_complete" and n["details"]["host_array"] == "B"],
                             "C_output": ["done_C0", "done_C1"],
                             "consumer": "host_compare", "release_and_task": "task_complete"},
        "symbolic_finish_constraints": finish_equations,
        "timing_semantics": "causal constraints only; exact completion requires actual resource/service/race guards",
        "full_completion_time_prediction": None,
        "full_completion_deployment_decision": "UNKNOWN_missing_shared_IOMMU_service_ownership_and_software_cost_binding",
        "predicted_source_semantic_properties": case["frozen_prediction"],
    }


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--contract", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise SystemExit("refusing to overwrite a completed analysis; inspect prior result")
    raw = args.contract.read_bytes()
    contract = json.loads(raw)
    analyses = [analyze(case) for case in contract["cases"]]
    result = {"schema": "iommu-A1-contract-analysis-v1", "contract_sha256": hashlib.sha256(raw).hexdigest(),
              "analysis_level": contract["execution_level"], "source_analysis_accepted": True,
              "cases": analyses, "timed_execution_admitted": False,
              "reason": contract["performance_gate"]["reason"],
              "all_full_time_predictions_unknown": all(x["full_completion_time_prediction"] is None for x in analyses),
              "new_native_or_RTL_episodes": 0,
              "old_four_timing_predictions_accepted": False}
    args.output.write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n")
    print(json.dumps({"source_analysis_accepted": True, "cases": len(analyses),
                      "timed_execution_admitted": False, "output": str(args.output)}))


if __name__ == "__main__":
    main()
