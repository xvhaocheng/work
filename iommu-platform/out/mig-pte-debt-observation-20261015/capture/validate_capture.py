#!/usr/bin/env python3
"""Validate this private new capture; no workload, old replay or selection search."""
import argparse
import hashlib
import json
from pathlib import Path
import struct

MASK30 = (1 << 30) - 1


def bits(n, low, width):
    return (n >> low) & ((1 << width) - 1)


def validate(words, original, identity):
    errors = []

    def require(test, text):
        if not test:
            errors.append(text)

    require(len(words) == 4608, "dump is not exactly4608 words")
    if len(words) != 4608:
        return {"valid": False, "errors": errors, "decision": "unknown"}
    w, s = words, words[0xE00:]
    for key in ("timing_accepted", "bit_identity_verified", "reset_to_first_AR_epoch_verified",
                "exact_source_build_verified", "source_ID_mapping_verified"):
        require(identity.get(key) is True, f"identity evidence not accepted: {key}")
    sha = identity.get("loaded_bit_sha256")
    require(isinstance(sha, str) and len(sha) == 64 and all(c in "0123456789abcdefABCDEF" for c in sha),
            "loaded BIT byte identity missing")
    require(original.get("schema") == "idma-composition-v1" and original.get("success") is True
            and original.get("run_attempted") is True, "original benchmark failed or wrong schema")
    expected = {"device": "/dev/idma0", "mode": 3, "protected_did": 10,
                "victim_gap_cycles": 0, "a_pages": 8, "aggressor_id": 1,
                "victim_pages": 1, "chunk_bytes": 4096, "victim_transfers": 64,
                "warmup_transfers": 256, "run_timeout_ms": 15000}
    for key, val in expected.items():
        require(original.get("request", {}).get(key) == val, f"frozen request mismatch: {key}")
    result = original.get("result") or {}
    require(result.get("status") == 0 and result.get("restore_status") == 0
            and result.get("raw_captured") == 1, "composition status/restore/raw failure")
    require(len(result.get("raw", [])) == 128, "missing complete original raw128 words")
    require(len(result.get("p4m", {}).get("observation", [])) == 35
            and len(result.get("p4m", {}).get("window", [])) == 16, "missing original P4M/raw35+16 words")
    exp = result.get("experiment", {})
    require(exp.get("measurement_started") == 1 and exp.get("measurement_complete") == 1
            and exp.get("status") == 0, "original measured window incomplete")
    for role in ("victim", "aggressor"):
        stats = exp.get(role) or {}
        require(stats.get("data_errors") == 0 and stats.get("quarantined") == 0,
                f"{role} data or quarantine failure")
        require(stats.get("submitted") == stats.get("completed"),
                f"{role} original task completion accounting failed")
        completed = stats.get("completed")
        require(isinstance(completed, int) and completed >= 0 and
                stats.get("completed_bytes") == completed * 4096,
                f"{role} complete4096B byte accounting failed")
    require(exp.get("victim", {}).get("completed") == 64, "victim64 incomplete")
    require(w[0] == 0x4D49474445425402 and s[0] == 0x4D49474F574E0001, "observer magic mismatch")
    status = w[1]
    require(w[2] == 0, f"native observer errors0x{w[2]:x}")
    require((s[1] >> 32) == 0, f"source observer errors0x{s[1] >> 32:x}")
    require(status & 0x4F == 0x4F, "native tracking/arm/window/frozen coverage incomplete")
    require(w[5] <= w[6] < w[7] <= w[8], "native epoch/open/close/freeze ordering invalid")
    require(s[3] <= s[4] < s[5], "source ARM/open/close ordering invalid")
    require(s[25] == (544 << 32) | 32 and s[26] == (96 << 48) | (5 << 32) | (64 << 16) | 8,
            "source layout differs from accepted schema")
    require(w[26] == (1536 << 32) | 32, "native layout differs from accepted schema")
    require(w[28] == 0x5246495245480001, "unknown Rfire/held codec")
    selected = bool(status & 16)
    report = {"schema": "mig-observation-validation-v1", "valid": False, "errors": errors,
              "UI_window": [w[6], w[7]], "source_window": [s[4], s[5]],
              "clock_domains_subtracted": False, "eligible_UI_count": w[11],
              "source_window_PTW_count": s[9], "source_window_normal_PTW_count": s[10],
              "selected": selected, "kernel_matches_historical": identity.get("expected_kernel") == identity.get("prior_kernel"),
              "performance_comparison": "not authorized by one episode",
              "original_in_flight_at_end_semantics": "window-end snapshot before background STOP/drain, not final quiet",
              "original_window_end_in_flight": {role: exp.get(role, {}).get("in_flight_at_end") for role in ("victim", "aggressor")}}
    if not selected:
        require(w[11] == 0, "eligible PTE exists but no selection was recorded")
        report.update(valid=not errors, decision="noeligiblePTE" if not errors else "unknown")
        return report
    require(w[11] >= 1, "selected without complete-window opportunity count")
    require(status & 0x3E0 == 0x3E0, "selected episode or first/last returns incomplete")
    require(s[1] & 1, "source recent rings not frozen by selected toggle")
    require(s[6] > s[4], "source ring freeze did not follow source open")

    def ptw_record(rec):
        c, m, t = rec[2], rec[5], rec[7]
        return {"source_tick": rec[0], "occurrence": rec[1], "request_generation": bits(c, 0, 32),
                "did": bits(c, 32, 24), "ttype": bits(c, 56, 6), "bank": bits(c, 62, 1),
                "normal": bool(c >> 63), "address": rec[3], "iova": rec[4],
                "pscid": bits(m, 0, 20), "gscid": bits(m, 20, 16), "stage": bits(m, 36, 2),
                "main_level": bits(m, 38, 2), "s1_level": bits(m, 40, 2),
                "implicit": bits(m, 42, 1), "en1": bits(m, 43, 1), "en2": bits(m, 44, 1),
                "debug": bits(m, 45, 1), "pid_valid": bits(m, 46, 1),
                "len": bits(m, 47, 8), "size": bits(m, 55, 3), "burst": bits(m, 58, 2),
                "id": 48 + bits(m, 60, 4), "descriptor_id": rec[6],
                "descriptor_generation": bits(t, 0, 32), "lane": bits(t, 32, 2),
                "active": bits(t, 34, 1), "unique_DID_lane": bits(t, 35, 1), "DID_matches": bits(t, 36, 3)}

    def comp_record(rec):
        t = rec[4]
        lane = bits(t, 49, 2)
        return {"source_tick": rec[0], "occurrence": rec[1], "address": rec[2],
                "descriptor_id": rec[3], "descriptor_generation": bits(t, 0, 32),
                "id": 32 + bits(t, 32, 4), "len": bits(t, 36, 8), "size": bits(t, 44, 3),
                "burst": bits(t, 47, 2), "lane": lane, "active": bits(t, 51, 1),
                "observed_DID_valid": bits(t, 52, 1), "did": s[13 + lane]}

    pcount, ccount = bits(s[11], 6, 7), bits(s[11], 23, 8)
    ppointer, cpointer = bits(s[11], 0, 6), bits(s[11], 16, 7)
    require(pcount <= 64 and ccount <= 96 and cpointer < 96, "source ring occupancy invalid")
    ps = [ptw_record(s[32+i*8:40+i*8]) for i in range(64 if pcount == 64 else pcount)]
    cs = [comp_record(s[544+i*5:549+i*5]) for i in range(96 if ccount == 96 else ccount)]
    require(pcount == 64 or ppointer == pcount, "partial PTW ring pointer invalid")
    require(ccount == 96 or cpointer == ccount, "partial comp ring pointer invalid")

    def join(target, records, name):
        matches = [r for r in records if r["id"] == target["id"] and r["occurrence"] == target["occurrence"]]
        require(len(matches) == 1, f"{name} source owner missing or nonunique (ID/occurrence)")
        if len(matches) != 1:
            return None
        r = matches[0]
        require(all((r[k] & MASK30 if k == "address" else r[k]) == target[k]
                    for k in ("address", "len", "size", "burst")), f"{name} endpoint tuple mismatch")
        require(r["descriptor_id"] > 0 and r["descriptor_generation"] > 0 and r["active"] == 1,
                f"{name} lacks active actual descriptor identity")
        require(r["lane"] in (0, 1) and r["did"] == 10 + r["lane"], f"{name} actual DID/lane mismatch")
        return r

    ptuple = w[13]
    p = {"ordinal": bits(w[12], 0, 32), "occurrence": bits(w[12], 32, 32),
         "id": bits(ptuple, 30, 6), "address": bits(ptuple, 0, 30),
         "len": bits(ptuple, 36, 8), "size": bits(ptuple, 44, 3), "burst": bits(ptuple, 47, 2)}
    require(p["id"] == 48 and p["len"] == 0 and p["size"] == 3 and p["burst"] == 1,
            "selected candidate is not native PTW single64bit INCR")
    owner = join(p, ps, "P")
    if owner:
        require(owner["normal"] and owner["ttype"] in (2, 3) and not owner["debug"]
                and not owner["implicit"] and owner["request_generation"] > 0,
                "selected PTW does not belong to a normal explicit AR/AW translation")
        require(owner["unique_DID_lane"] and owner["DID_matches"] == 1 and
                owner["bank"] == owner["lane"] % 2, "P normal TL bank/actual descriptor association invalid")
    require(w[6] <= w[14] < w[7] and w[14] <= w[15] <= w[16] <= w[17], "P/UI window/return ordering invalid")
    require(w[19] & 511 == 0 and bits(w[19], 16, 9) == p["len"] + 1, "P beat debt not fully retired")

    events = []
    require(w[23] <= 1536, "R event count overflow")
    for i in range(min(w[23], 1536)):
        tick, e = w[512+2*i:514+2*i]
        events.append({"tick": tick, "id": bits(e, 0, 6), "last": bits(e, 6, 1),
                       "resp": bits(e, 7, 2), "valid": bits(e, 9, 1), "ready": bits(e, 10, 1),
                       "matched": bits(e, 11, 1), "Pedge": bits(e, 12, 1), "held_before": bits(e, 13, 19), "ordinal": bits(e, 32, 32)})
    require(all(e["valid"] and e["ready"] and e["matched"] and e["resp"] == 0 for e in events), "invalid/error/unmatched R event")
    require(all(events[i]["tick"] < events[i+1]["tick"] for i in range(len(events)-1)), "R timeline nonmonotonic")
    require(all(e["tick"] >= w[14] and e["Pedge"] == (e["tick"] == w[14]) for e in events), "P same-edge R ordering/log mismatch")
    for index,e in enumerate(events):
        start = e["tick"] - e["held_before"]
        require(start >= w[14] and (index == 0 or start > events[index-1]["tick"]),
                "held interval precedes P or overlaps previous fire")
        require(not e["Pedge"] or e["held_before"] == 0, "same-P-edge fire has unclipped held history")
    pevents = [e for e in events if e["ordinal"] == p["ordinal"]]
    require(bool(pevents) and pevents[0]["tick"] - pevents[0]["held_before"] == w[15], "P firstVALID absent/mismatched in R log")
    pfires = [e for e in pevents if e["ready"]]
    require(len(pfires) == p["len"]+1 and pfires[0]["tick"] == w[16] and pfires[-1]["tick"] == w[17]
            and pfires[-1]["last"], "P beat/fire/LAST conservation failed")
    require(sum(e["held_before"] for e in pevents) == w[18], "P held intervals mismatch")
    require(all(e["id"] == p["id"] for e in pevents), "P RID differs from selected P")
    rows, payload = [], []
    for slot in range(32):
        rec = w[32+slot*8:40+slot*8]
        f = rec[0]
        if not f & 1:
            continue
        r = {"ordinal": bits(rec[1], 0, 32), "occurrence": bits(rec[1], 32, 32),
             "id": bits(f, 8, 6), "address": rec[2], "len": bits(f, 14, 8),
             "size": bits(f, 22, 3), "burst": bits(f, 25, 2), "k": bits(f, 27, 9),
             "remaining": bits(f, 36, 9), "completed": bits(f, 1, 1),
             "tAR": rec[3], "tLastRfire": rec[4], "held_cycles": rec[5]}
        require(r["ordinal"] < p["ordinal"] and r["tAR"] <= w[14] and r["k"] <= r["len"]+1,
                "predecessor ordinal/time/beat bound failed")
        all_fires = [e for e in events if e["ordinal"] == r["ordinal"]]
        require(all(e["id"] == r["id"] for e in all_fires), "predecessor RID differs from row")
        fires = [e for e in all_fires if e["ready"] and e["tick"] > w[14]]
        require(len(fires) == r["k"] == rec[6] and r["completed"] and r["remaining"] == 0,
                f"predecessor{r['ordinal']} postP beat conservation failed")
        if fires:
            require(fires[-1]["last"] and fires[-1]["tick"] == r["tLastRfire"], "predecessor final fire mismatch")
        else:
            require(r["k"] == 0 and r["tLastRfire"] == w[14], "zero-debt same-edge predecessor mismatch")
        if 32 <= r["id"] < 48:
            r["classification"] = "DMA payload"
            r["source_owner"] = join(r, cs, f"payload{r['ordinal']}")
            payload.append(r)
        elif r["id"] < 16:
            r["classification"] = "CPU"
        elif r["id"] < 32:
            r["classification"] = "debug"
        else:
            r["classification"] = {48: "PTW", 49: "DDTW", 50: "PDTW", 51: "CQ",
                                   52: "MSIPTW", 53: "MRIF"}.get(r["id"], "unclassified DS")
            require(r["classification"] != "unclassified DS", "unclassified predecessor DS source")
        rows.append(r)
    require(len(rows) == w[20] and sum(r["k"] for r in rows) == w[21], "predecessor summary mismatch")
    end = max([w[17]]+[r["tLastRfire"] for r in rows])
    require(sum(e["held_before"] for e in events) == w[24], "held R cycles mismatch")
    require(end-w[14]+1-len(events)-sum(e["held_before"] for e in events) == w[25], "noVALID gaps mismatch")
    known_ordinals = {p["ordinal"]} | {r["ordinal"] for r in rows}
    require(all(e["ordinal"] in known_ordinals for e in events), "Rfire with no selected/predecessor occurrence")
    report["Rfire_records"] = events
    report["held_intervals"] = [{"firstVALID_tick":e["tick"]-e["held_before"],
                                  "last_held_tick":e["tick"]-1,"fire_tick":e["tick"],
                                  "ordinal":e["ordinal"],"id":e["id"]}
                                 for e in events if e["held_before"]]
    witness = [r for r in payload if r["k"] > 0 and r["tLastRfire"] < w[15]]
    report.update(P=p, P_owner=owner, P_UI_times={"accepted": w[14], "firstVALID": w[15],
                                               "firstfire": w[16], "lastfire": w[17]},
                  predecessors=rows, payload_debt=sum(r["k"] for r in payload),
                  total_predecessor_debt=w[21], witness_ordinals=[r["ordinal"] for r in witness],
                  valid=not errors)
    report["decision"] = "unknown" if errors else "reachable_payload_R_debt" if witness else "selected_episode_no_preceding_payload_resolution_witness"
    return report


def main():
    a = argparse.ArgumentParser()
    a.add_argument("--dump", type=Path, required=True)
    a.add_argument("--original", type=Path, required=True)
    a.add_argument("--identity", type=Path, required=True)
    a.add_argument("--receipt-dir", type=Path, help="defaults to original stdout directory")
    a.add_argument("--out", type=Path, required=True)
    args = a.parse_args()
    raw = args.dump.read_bytes()
    if len(raw) % 8:
        raise SystemExit("dump byte length is not a u64 multiple")
    original_raw = args.original.read_bytes()
    identity = json.loads(args.identity.read_text())
    result = validate(list(struct.unpack("<"+"Q"*(len(raw)//8), raw)), json.loads(original_raw), identity)
    receipt_dir = args.receipt_dir or args.original.parent
    receipt_errors = []
    def receipt_text(name):
        try:
            return (receipt_dir / name).read_text().strip()
        except OSError:
            receipt_errors.append(f"missing receipt: {name}")
            return ""
    launch = dict(line.split("=",1) for line in receipt_text("launcher.receipt").splitlines() if "=" in line)
    for key in ("abi_exit", "probe_exit", "workload_exit", "readback_exit"):
        if launch.get(key) != "0":
            receipt_errors.append(f"launcher incomplete/failure: {key}")
    if launch.get("workload_attempted") != "1":
        receipt_errors.append("launcher did not record exactly one attempted workload")
    boot_before, boot_after = receipt_text("boot.before.txt"), receipt_text("boot.after.txt")
    if not boot_before or boot_before != boot_after:
        receipt_errors.append("boot changed during observation or boot identity missing")
    before_taint, after_taint = receipt_text("tainted.before.txt"), receipt_text("tainted.after.txt")
    if not before_taint or before_taint != after_taint:
        receipt_errors.append("kernel taint changed or taint receipt missing")
    kernel = receipt_text("kernel.release.txt")
    if not identity.get("expected_kernel") or kernel != identity["expected_kernel"]:
        receipt_errors.append("actual kernel differs from accepted new kernel identity")
    loaded_sha = receipt_text("loaded-bit.sha256")
    if loaded_sha != identity.get("loaded_bit_sha256"):
        receipt_errors.append("launcher loaded BIT byte identity differs from accepted identity")
    tools = receipt_text("tools.sha256").splitlines()
    if len(tools) != 2 or any(len(line.split()[0]) != 64 for line in tools):
        receipt_errors.append("tool SHA256 receipts absent or malformed")
    result["program_sha256_receipts"] = tools
    try:
        readback = json.loads(receipt_text("readback.receipt.json"))
    except json.JSONDecodeError:
        readback = {}
        receipt_errors.append("readback receipt is not JSON")
    if (readback.get("schema") != "mig-readback-receipt-v2" or readback.get("words") != 4608
            or readback.get("bytes") != len(raw) or readback.get("build_tag") != "4d444f3130313502"):
        receipt_errors.append("readback identity/size receipt mismatch")
    if readback.get("final_composition_quiet") != 1 or readback.get("frontend_backend_busy") != [0,0,0,0]:
        receipt_errors.append("final composition/frontend quiet not demonstrated")
    result["final_composition_quiet"] = readback.get("final_composition_quiet")
    result["final_frontend_backend_busy"] = readback.get("frontend_backend_busy")
    result["kernel"] = kernel
    result["kernel_matches_historical"] = kernel == identity.get("prior_kernel")
    result["boot_id"] = boot_before
    result["receipt_directory"] = str(receipt_dir.resolve())
    result["errors"].extend(receipt_errors)
    if receipt_errors:
        result["valid"] = False
        result["decision"] = "unknown"
    result["dump_sha256"] = hashlib.sha256(raw).hexdigest()
    result["original_exact_stdout_sha256"] = hashlib.sha256(original_raw).hexdigest()
    result["input_paths"] = {"dump": str(args.dump.resolve()), "original": str(args.original.resolve()),
                             "identity": str(args.identity.resolve())}
    args.out.write_text(json.dumps(result, indent=2)+"\n")
    print(json.dumps({"valid": result["valid"], "decision": result["decision"], "errors": result["errors"]}))
    raise SystemExit(0 if result["valid"] else 1)


if __name__ == "__main__":
    main()
