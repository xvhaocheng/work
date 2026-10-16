#!/usr/bin/env python3
"""New static contract/width checks only. Never invokes RTL, tests, or builds."""
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
c = json.loads((HERE / "baseline-contract.json").read_text())
m = json.loads((HERE / "source-manifest.json").read_text())
root = Path(m["workspace_root"])

for s in m["files"]:
    assert hashlib.sha256((root / s["path"]).read_bytes()).hexdigest() == s["sha256"], s["path"]
fields = set(c["cost_field_catalog"])
assert len(c["baselines"]) == 6
for b in c["baselines"]:
    assert set(b["cost"]) == fields, b["id"]
    assert set(b["invariants"]) == {"W", "R", "VALID", "same_id", "resource", "lifetime"}
    for unknown in ("lut", "ff", "bram", "area", "timing_fmax", "energy"):
        assert b["cost"][unknown] is None, (b["id"], unknown)

# All widths below are the explicitly read test type parameters, not synthesis.
ar_fields = 64 + 8 + 3 + 2 + 1 + 4 + 3 + 4 + 4 + 1
ext = 24 + 1 + 20 + 2 + 1 + 1 + 1
local_ar = 2 + ar_fields + ext
comp_ar = 4 + ar_fields
aw_raw = 4 + ar_fields + 6 + ext
aw_physical = 4 + ar_fields + 6
local_r = 2 + 64 + 2 + 1 + 1
assert (local_ar, comp_ar, aw_raw, aw_physical, local_r) == (146, 98, 154, 104, 70)
assert 256 > 32 // 2 == 16
assert 2 * 3 * 8 == 48
assert (512 + 512 + 3 + 3) * 8 == 8240
assert 32 * 64 // 8 == 256
assert 32 * (8 + 1 + 1) == 320
assert 2 * 256 * 64 // 8 == 4096
assert 2 * 256 * (local_r - 64) == 3072
assert 16 * local_ar == 2336
assert 16 * (64 + 8 + 3) == 1200
assert 2 * 8 + 4 * 3 + 2 * 4 == 36
assert 2 * 8 * 2 + 4 * 3 + 2 * 4 == 52
assert 8 * aw_raw == 1232
assert 8 * 3 == 24
assert c["execution"]["new_RTL_tests"] == 0
assert c["execution"]["old_tests_rerun"] == 0
assert not c["decisions"]["new_performance_admission"]

result = {
    "kind": "new_static_contract_and_arithmetic_only_not_RTL_performance_experiment",
    "status": "PASS",
    "baseline_count": len(c["baselines"]),
    "cost_fields_each": len(fields),
    "source_hashes_checked": len(m["files"]),
    "L2_length_condition": {"burst_beats": 256, "TC_capacity": 16, "holds": True},
    "actual_four_DMA_RTL_runs": 0,
    "ordinary_success_mapping": "hand proof in evidence.md; not verified by this arithmetic script",
    "general_exception_proof": "unknown",
}
(HERE / "static-check.json").write_text(json.dumps(result, indent=2) + "\n")
print(json.dumps(result))
