#!/usr/bin/env python3
"""Independently replay raw victim snapshots and check the frozen run matrix."""
import csv
import gzip
import hashlib
import json
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def event_hash(path):
    digest = hashlib.sha256()
    opener = gzip.open if path.suffix == ".gz" else open
    with opener(path, "rb") as stream:
        while block := stream.read(1024 * 1024):
            digest.update(block)
    return digest.hexdigest()


def raw_victims(attempt, variant):
    resident = {}
    evictions = Counter()
    fills = 0
    for line in (attempt / "stdout.log").read_text().splitlines():
        if not line.startswith("IOTLB_VICTIM,"):
            continue
        f = [x.strip() for x in line.split(",")]
        assert len(f) == 16 and int(f[1]) == int(variant[1])
        vpn, way, old_vpn = int(f[6], 16), int(f[7]), int(f[8], 16)
        old_valid, valid, eligible = int(f[9]), int(f[10], 16), int(f[11], 16)
        tree, active, ranks = int(f[12], 16), int(f[14], 16), int(f[15], 16)
        assert eligible & (1 << way), (attempt, "ineligible victim", way)
        assert valid == sum(1 << i for i in resident), (attempt, "pre-edge occupancy")
        assert bool(old_valid) == (way in resident)
        if old_valid:
            assert old_vpn == resident[way], (attempt, "evicted VPN")
            assert f[13] == "valid_evict"
            evictions[way] += 1
        else:
            assert f[13] == "invalid_fill"
        candidates = [i for i in range(16) if eligible & (1 << i)]
        if variant == "v0":
            node, lower, upper = 0, 0, 16
            while upper - lower > 1:
                middle = (lower + upper) // 2
                left = [i for i in candidates if lower <= i < middle]
                right = [i for i in candidates if middle <= i < upper]
                select_right = bool(right) and (not left or bool(tree & (1 << node)))
                if select_right:
                    node, lower = 2 * node + 2, middle
                else:
                    node, upper = 2 * node + 1, middle
            expected = lower
        elif variant == "v1":
            assert eligible in (3, 65532)
            if eligible == 3:
                expected = (active >> 13) & 1
            else:
                def descend(leaves, bit):
                    if len(leaves) == 1:
                        return leaves[0]
                    split = len(leaves) // 2
                    return (descend(leaves[split:], bit + split)
                            if active & (1 << bit)
                            else descend(leaves[:split], bit + 1))
                expected = descend(candidates, 0)
        else:
            rank = [(ranks >> (4 * i)) & 15 for i in range(16)]
            assert sorted(rank) == list(range(16)), (attempt, "LRU rank permutation")
            empty = [i for i in candidates if i not in resident]
            expected = empty[0] if empty else min(candidates, key=rank.__getitem__)
        assert way == expected, (attempt, "victim/state mismatch", way, expected)
        resident[way] = vpn
        fills += 1
    return {"fills": fills, "valid_evictions": sum(evictions.values()),
            "eviction_ways": dict(evictions), "final_occupancy": len(resident)}


def main():
    with (HERE / "analysis/comparison.csv").open(newline="") as stream:
        rows = list(csv.DictReader(stream))
    expected = {(v, m, r) for v in ("v0", "v1", "v2")
                for m in ("p", "pn") for r in (1, 8, 16)}
    assert len(rows) == 18
    assert {(r["variant"], r["mode"], int(r["ring"])) for r in rows} == expected
    checked = []
    for row in rows:
        variant, mode, ring = row["variant"], row["mode"], int(row["ring"])
        attempts = sorted((HERE / "runs" / f"{variant}-{mode}-r{ring}").glob("attempt*"))
        assert len(attempts) == (2 if ring == 8 else 1)
        repeat_hashes = []
        for attempt in attempts:
            receipt = json.loads((attempt / "receipt.json").read_text())
            result = json.loads((attempt / "result.json").read_text())
            assert receipt["exit_code"] == 0 and receipt["state"] == "completed"
            assert result["T_return"] == int(row["T_return"])
            assert result["mode"] == mode and result["ring"] == ring
            with (attempt / "parents.csv").open(newline="") as stream:
                parents = list(csv.DictReader(stream))
            assert len(parents) == 128
            assert {(int(p["lane"]), int(p["j"])) for p in parents} == {
                (lane, j) for lane in (0, 1) for j in range(64)}
            for parent in parents:
                assert int(parent["rd_bytes"]) == int(parent["wr_bytes"]) == 4096
                assert int(parent["b"]) == 2 and int(parent["err"]) == 0
                assert int(parent["done"]) >= int(parent["last_b"])
            replay = raw_victims(attempt, variant)
            assert replay["fills"] == int(row["walks_total"])
            assert replay["valid_evictions"] == int(row["valid_evictions"])
            events = attempt / "events.csv.gz"
            digest = event_hash(events)
            assert digest == receipt["uncompressed_events_sha256"]
            raw = attempt / "events.csv"
            if raw.exists():
                assert sha(raw) == digest
            hashes = (sha(attempt / "result.json"), sha(attempt / "parents.csv"), digest)
            repeat_hashes.append(hashes)
            if variant == "v0":
                old = HERE.parent / "problem-discovery-run-20261017/runs" / f"base-{mode}-r{ring}" / "attempt01"
                assert sha(old / "result.json") == hashes[0]
                assert sha(old / "parents.csv") == hashes[1]
                old_events = old / "events.csv.gz"
                assert event_hash(old_events if old_events.exists() else old / "events.csv") == digest
            checked.append({"attempt": str(attempt.relative_to(HERE)), **replay})
        assert len(set(repeat_hashes)) == 1, (variant, mode, ring, "repeat changed")
    output = {"status": "passed", "primary_configurations": len(rows),
              "attempts": len(checked), "raw_victim_replay": checked,
              "baseline_reproduction": "All six V0 result JSON, parents and uncompressed event bytes match 10-17 exactly",
              "repeat_scope": "R8 P and PN, all three variants: result/parent/event bytes identical",
              "script_sha256": sha(Path(__file__))}
    (HERE / "verification.json").write_text(json.dumps(output, indent=2) + "\n")
    print(json.dumps({k: v for k, v in output.items() if k != "raw_victim_replay"}, indent=2))


if __name__ == "__main__":
    main()
