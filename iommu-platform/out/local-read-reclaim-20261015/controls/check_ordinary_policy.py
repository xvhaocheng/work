"""New finite control/safety checks for the three independent LR1 policies."""

import ast
import hashlib
import itertools
import json
from datetime import datetime, timezone
from pathlib import Path
from types import MappingProxyType

import ordinary_policy as ordinary


HERE = Path(__file__).resolve().parent
POLICIES = (ordinary.whole_copy, ordinary.phased, ordinary.targeted_drain)
QUALIFICATION = ("readonly", "nonoverlap", "registry_complete", "alias_safe")
ACK_TRUE = QUALIFICATION + (
    "all_sources_sealed",
    "all_readers_complete",
    "authorization_revoked",
    "cache_synced",
    "invalidation_committed",
    "target_ordering_proven",
    "request_active",
)


def ready():
    state = dict.fromkeys(ordinary.BOOL_FIELDS, True)
    state.update(request_epoch=7, current_epoch=7, old_obligations=0, read_error=False)
    return state


def successful_release_allowed(state, name):
    return (
        all(state[field] for field in ACK_TRUE)
        and not state["read_error"]
        and state["old_obligations"] == 0
        and state["request_epoch"] == state["current_epoch"]
        and (name != "whole_copy" or state["whole_copy_complete"])
    )


def check():
    tree = ast.parse((HERE / "ordinary_policy.py").read_text())
    imported = []
    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            imported.extend(alias.name for alias in node.names)
        elif isinstance(node, ast.ImportFrom):
            imported.append(node.module)
    assert imported == ["collections.abc"], imported
    for node in tree.body:
        if isinstance(node, ast.FunctionDef) and node.name in {p.__name__ for p in POLICIES}:
            calls = {
                call.func.id
                for call in ast.walk(node)
                if isinstance(call, ast.Call) and isinstance(call.func, ast.Name)
            }
            assert not calls.intersection({p.__name__ for p in POLICIES}), (node.name, calls)

    exhaust_calls = 0
    action_counts = {p.__name__: dict.fromkeys(sorted(ordinary.ACTIONS), 0) for p in POLICIES}
    epoch_cases = ((0, 0), (7, 7), (7, 8))
    for flags in itertools.product((False, True), repeat=len(ordinary.BOOL_FIELDS)):
        template = dict(zip(ordinary.BOOL_FIELDS, flags))
        for request_epoch, current_epoch in epoch_cases:
            for old in (0, 1, 4):
                state = dict(template, request_epoch=request_epoch, current_epoch=current_epoch, old_obligations=old)
                frozen = MappingProxyType(state)
                for policy in POLICIES:
                    action = policy(frozen)
                    assert action in ordinary.ACTIONS
                    action_counts[policy.__name__][action] += 1
                    exhaust_calls += 1
                    if action == "ack":
                        assert successful_release_allowed(state, policy.__name__), (policy.__name__, state)
                    if action == "error":
                        assert state["read_error"] and state["all_sources_sealed"] and old == 0
                        assert state["request_active"] and request_epoch == current_epoch

    directed = []
    for policy in POLICIES:
        state = ready()
        assert policy(MappingProxyType(state)) == "ack"
        before = dict(state)
        state["global_quiet"] = False
        state["destination_write_complete"] = False
        state["private_oracle"] = False
        assert policy(MappingProxyType(state)) == "ack"
        assert {key: state[key] for key in before} == before
        directed.append({"policy": policy.__name__, "case": "extra_unrelated_fields_do_not_gate_ack", "result": "pass"})

        for name in ordinary.BOOL_FIELDS:
            for malformed in (0, 1, None, "true", [], {}):
                bad = ready()
                bad[name] = malformed
                assert policy(bad) == "wait", (policy.__name__, name, malformed)
        for name in ordinary.INT_FIELDS:
            for malformed in (False, True, -1, 0.0, None, "0"):
                bad = ready()
                bad[name] = malformed
                assert policy(bad) == "wait", (policy.__name__, name, malformed)
        for name in ordinary.SNAPSHOT_FIELDS:
            bad = ready()
            del bad[name]
            assert policy(bad) == "wait", (policy.__name__, name)
        for malformed in (None, (), [], True):
            assert policy(malformed) == "wait"
        directed.append({"policy": policy.__name__, "case": "strict_literal_types_missing_fields_and_nonmapping_fail_closed", "result": "pass"})

        error = ready()
        error.update(read_error=True, whole_copy_complete=False, all_readers_complete=False, all_sources_sealed=False, old_obligations=2)
        sequence = [policy(error)]
        assert sequence[-1] == "seal"
        error["all_sources_sealed"] = True
        sequence.append(policy(error))
        assert sequence[-1] == "wait"
        error["old_obligations"] = 0
        sequence.append(policy(error))
        assert sequence[-1] == "error" and "ack" not in sequence
        error["current_epoch"] += 1
        assert policy(error) == "wait"
        directed.append({"policy": policy.__name__, "case": "error_seal_drain_quarantine_and_stale_epoch", "sequence": sequence, "result": "pass"})

        state = ready()
        state.update(all_sources_sealed=False, all_readers_complete=False, old_obligations=2,
                     authorization_revoked=False, cache_synced=False, invalidation_committed=False,
                     target_ordering_proven=False, whole_copy_complete=False)
        sequence = [policy(state)]
        assert sequence[-1] == ("wait" if policy.__name__ == "whole_copy" else "seal")
        if policy.__name__ == "whole_copy":
            state["whole_copy_complete"] = True
            sequence.append(policy(state))
            assert sequence[-1] == "seal"
        state["all_sources_sealed"] = True
        sequence.append(policy(state))
        assert sequence[-1] == "wait"
        state["all_readers_complete"] = True
        assert policy(state) == "wait"
        state["old_obligations"] = 0
        for action, field in (("revoke", "authorization_revoked"), ("sync", "cache_synced"),
                              ("invalidate", "invalidation_committed"), ("order", "target_ordering_proven")):
            actual = policy(state)
            assert actual == action, (policy.__name__, actual, action)
            sequence.append(actual)
            state[field] = True
        sequence.append(policy(state))
        assert sequence[-1] == "ack"
        directed.append({"policy": policy.__name__, "case": "success_action_sequence_with_partial_reader_and_old_obligation", "sequence": sequence, "result": "pass"})

    for policy in (ordinary.phased, ordinary.targeted_drain):
        state = ready()
        state["whole_copy_complete"] = False
        assert policy(state) == "ack"
    state = ready()
    state["whole_copy_complete"] = False
    assert ordinary.whole_copy(state) == "wait"

    file_hashes = {
        name: hashlib.sha256((HERE / name).read_bytes()).hexdigest()
        for name in ("ordinary_policy.py", "check_ordinary_policy.py")
    }
    return {
        "schema": "lr1-ordinary-new-checks-v1",
        "completed_utc": datetime.now(timezone.utc).isoformat(),
        "result": "pass",
        "policy_files_sha256": file_hashes,
        "snapshot_fields": list(ordinary.SNAPSHOT_FIELDS),
        "exhaustive_snapshots": exhaust_calls // len(POLICIES),
        "exhaustive_policy_calls": exhaust_calls,
        "action_counts": action_counts,
        "directed_checks": directed,
        "scope": "New untimed ordinary control/type/safety checks only; no candidate/oracle imports or old experiment rerun",
        "limitations": [
            "Does not verify native RTL, PTE/cache/CQ behavior, pipeline accounting or genuine DMA real-R receipts",
            "Does not measure source reuse latency, destination completion, area, Fmax, CPU or application benefit",
            "Physical page release is an adapter operation allowed only on ack; error leaves it quarantined",
        ],
    }


if __name__ == "__main__":
    report = check()
    (HERE / "policy-checks.json").write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n")
    print(json.dumps(report, indent=2, ensure_ascii=False))
