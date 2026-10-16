#!/usr/bin/env python3
"""Independently verify new raw source events and mathematical output bytes."""
from pathlib import Path
from collections import Counter
import hashlib
import json
import struct

OUT = Path(__file__).resolve().parents[1]
contract = json.loads((OUT / "cases/frozen-contract.json").read_text())
expected_sha = (OUT / "cases/frozen-contract.sha256").read_text().strip()
sha = lambda data: hashlib.sha256(data).hexdigest()
assert sha((OUT / "cases/frozen-contract.json").read_bytes()) == expected_sha
summary = json.loads((OUT / "validation/semantic-runs/summary.json").read_text())
assert summary["executed_runs"] == summary["requested_runs"] == 8
assert summary["input_contract_sha256"] == expected_sha
assert summary["executor_sha256"] == sha((OUT / "validation/semantic_executor.py").read_bytes())
pack = lambda words: struct.pack("<" + str(len(words)) + "I", *words)
expected_arrays = {
    "A": pack(list(range(1024))),
    "B": pack([2 if row == column else 0 for row in range(32) for column in range(32)]),
    "C": pack([2 * value for value in range(1024)]),
}
verified = []
for case in contract["cases"]:
    h2_prefixes = []
    for policy in contract["policies"]:
        run = OUT / "validation/semantic-runs" / case["id"] / policy
        rows = [json.loads(line) for line in (run / "events.jsonl").read_text().splitlines()]
        result = json.loads((run / "result.json").read_text())
        assert result["pass"] and result["input_contract_sha256"] == expected_sha
        nodes = {n["id"]: n for n in case["nodes"]}
        done, begins, published, finished, waited = set(), {}, [], [], []
        copy_bytes = Counter()
        allocations = set()
        owners = {name: set() for name in ["a0", "a1", "b0", "b1", "c0", "c1"]}
        event_order = []
        pending_begin = None
        at_K11 = None
        for row in rows:
            assert row["case"] == case["id"] and row["policy"] == policy
            event = row["event"]
            if event == "event_begin":
                name = row["node_id"]
                assert pending_begin is None and name not in done
                assert set(nodes[name]["predecessors"]) <= done
                assert row["predecessors"] == nodes[name]["predecessors"]
                assert name in row["ready"]
                begins[name] = row
                pending_begin = name
            elif event == "event_complete":
                name = row["node_id"]
                assert pending_begin == name and name not in done
                pending_begin = None
                detail, kind, receipt = nodes[name]["details"], nodes[name]["kind"], row["result"]
                if kind == "publish":
                    job = detail["job_id"]
                    assert job not in published and receipt["job_id"] == job
                    published.append(job)
                elif kind == "dma_complete":
                    job = detail["job_id"]
                    assert job == next(j for j in published if j not in finished)
                    assert receipt["copied_bytes"] == 2048
                    assert receipt["direction"] == detail["direction"]
                    payload = expected_arrays[detail["host_array"]][detail["stripe"] * 2048:(detail["stripe"] + 1) * 2048]
                    assert receipt["data_sha256"] == sha(payload)
                    finished.append(job)
                    copy_bytes[detail["direction"]] += 2048
                elif kind == "wait" and "job_id" in detail:
                    assert detail["job_id"] in finished and detail["job_id"] not in waited
                    waited.append(detail["job_id"])
                done.add(name)
                event_order.append(name)
                if name == "K11":
                    at_K11 = {"published": list(published), "completed": list(finished),
                               "C0_unretired": "C0" in published and "C0" not in finished,
                               "prefix_nodes": list(event_order)}
            elif event == "slot_owner":
                slot, owner = row["slot"], row["owner"]
                operation = row["operation"]
                if operation == "acquire":
                    assert owner not in owners[slot]
                    if owner == "allocation":
                        assert slot not in allocations
                        allocations.add(slot)
                    else:
                        assert "allocation" in owners[slot]
                    owners[slot].add(owner)
                elif operation == "release":
                    assert owner in owners[slot]
                    owners[slot].remove(owner)
                elif operation == "free":
                    assert owner == "allocation" and owners[slot] == {"allocation"}
                    owners[slot].clear()
                    allocations.remove(slot)
                elif operation in ["read", "write"]:
                    if owner == "retention_check":
                        assert case["retain_B"] and any(x.startswith("resident:B0") for x in owners[slot])
                    else:
                        assert owner in owners[slot]
                else:
                    raise AssertionError("unknown owner transition")
        assert pending_begin is None and done == set(nodes)
        assert len(published) == len(finished) == len(waited) == case["expected_jobs"]
        assert sum(copy_bytes.values()) == case["expected_dma_copied_bytes"]
        assert copy_bytes["h2d"] == case["expected_external_read_bytes"]
        assert copy_bytes["d2h"] == case["expected_external_write_bytes"]
        assert not allocations and all(not value for value in owners.values())
        assert (run / "output-c.bin").read_bytes() == expected_arrays["C"]
        assert result["output_C_sha256"] == sha(expected_arrays["C"])
        assert result["consumer_elements"] == 1024 and result["kernel_MACs"] == 32768
        assert event_order[:len(case["prefix"])] == case["prefix"]
        if case["id"] == "P":
            assert published[:2] == (["A0", "A1"] if policy == "source_A_first" else ["A0", "B00"])
        if case["id"] == "N":
            assert published[:2] == ["A0", "A1"]
        if case["id"] == "H1":
            assert "B10" not in published and "B11" not in published
            assert result["retained_B_checks"] == ["b0", "b1"]
        if case["id"] == "H2":
            assert at_K11 is not None
            assert result["H2_C0_debt_at_K11"] == at_K11["C0_unretired"]
            assert all(j in at_K11["published"] for j in ["A0", "A1", "B00", "B01", "B10", "B11"])
            h2_prefixes.append(at_K11["prefix_nodes"])
        tail = ["done_C0", "wait_C0", "pub_C1", "done_C1", "wait_C1", "parallel_join",
                "free_six", "kernel_return", "map_out", "map_out_visibility", "host_compare", "free_host", "task_complete"]
        assert all(event_order.index(a) < event_order.index(b) for a, b in zip(tail, tail[1:]))
        assert result["timing"] is None and not result["performance_prediction_accepted"]
        assert not result["native_DDR_or_RTL_validation"] and result["AR_R_AW_W_B_counts"] is None
        verified.append({"case": case["id"], "policy": policy, "source_events": len(done),
                         "jobs": len(published), "copied_bytes": sum(copy_bytes.values()),
                         "publication_order": published, "output_sha256": sha(expected_arrays["C"]),
                         "raw_records": len(rows), "owner_and_full_consumer_checked": True,
                         "C0_unretired_at_K11_in_this_untimed_linearization": at_K11["C0_unretired"],
                         "event_log_sha256": sha((run / "events.jsonl").read_bytes())})
    if case["id"] == "H2":
        assert h2_prefixes[0] == h2_prefixes[1]

output = {"schema": "A1-independent-source-event-verification-v1", "passed": True,
          "verified_complete_source_episodes": len(verified), "cases": verified,
          "contract_sha256": expected_sha, "performance_or_native_episodes": 0,
          "original_four_timing_predictions_accepted": False,
          "meaning": "source DAG/bytes/owners/consumer only; atomic event order and unfinished C0 are not measured latency or AXI debt"}
(OUT / "validation/source-event-verification.json").write_text(json.dumps(output, ensure_ascii=False, indent=2) + "\n")
print(json.dumps({"passed": True, "complete_source_episodes": len(verified),
                  "raw_records": sum(x["raw_records"] for x in verified), "timed_or_native_episodes": 0}))
