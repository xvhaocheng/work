#!/usr/bin/env python3
"""Independent source-shape, byte and real-DMA-record acceptance.

This does not import the contract builder or run a DUT. A host logical reference
is never accepted as hardware execution. All output is confined to critical/ or
an explicitly selected acceptance path; originals are read-only.
"""
import argparse
import csv
import hashlib
import json
import math
import shutil
import struct
from pathlib import Path


class Rejection(ValueError):
    pass


def require(condition, message):
    if not condition:
        raise Rejection(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def independent_specs():
    # Source-reviewed opt_config.py:17-31,65-69; flex_opt.py:477-505.
    h = 768
    return [("fc1.weight", (4*h, h)), ("fc1.bias", (4*h,)),
            ("fc2.weight", (h, 4*h)), ("fc2.bias", (h,)),
            ("final_layer_norm.weight", (h,)),
            ("final_layer_norm.bias", (h,))]


def align(n):
    return ((n + 4095) // 4096) * 4096


def read_commands(path):
    lines = path.read_text().splitlines()
    require(bool(lines), "empty command file")
    keys = lines[0].split()
    require(keys == ["id", "channel", "src_iova", "dst_iova", "bytes",
                     "dep", "tensor", "chunk"], "unexpected TSV header")
    rows = []
    for lineno, line in enumerate(lines[1:], 2):
        fields = line.split()
        require(len(fields) == len(keys), f"TSV line {lineno} field count")
        rows.append(dict(zip(keys, map(int, fields))))
    return rows


def independent_contract(root):
    contract = json.loads((root / "transfer-contract.json").read_text())
    require(contract["source_commit"] == "004ffef82b46e8dc8685c55d0cdda650bdaf1269",
            "wrong source snapshot")
    require(contract["source_model"] == "opt-125m", "wrong source model")
    require(contract["application_timing"] is None and
            contract["compute_delay"] is None and contract["deadlines"] is None,
            "unsourced application timing")
    require(contract["performance_G0_accepted"] is False and
            contract["G1_candidate"] is None, "functional contract overstated")
    tensors, chunks, offset = [], [], 0
    for ti, (name, shape) in enumerate(independent_specs()):
        n = math.prod(shape) * 2
        tensors.append({"name": name, "shape": list(shape), "bytes": n,
                        "offset": offset})
        for local in range(0, n, 4096):
            chunks.append({"index": len(chunks), "tensor": ti,
                           "offset": offset + local, "bytes": min(4096, n-local)})
        offset += align(n)
    require(len(contract["tensors"]) == 6, "not a complete six-tensor package")
    for actual, expected in zip(contract["tensors"], tensors):
        for key in expected:
            require(actual[key] == expected[key], f"tensor {expected['name']} {key}")
        require(actual["dtype"] == "FP16 bit representation", "dtype changed")
    require(contract["chunks"] == chunks, "missing/extra/reordered chunk coverage")
    total = sum(t["bytes"] for t in tensors)
    require(total == 9447936 and len(chunks) == 2309, "independent source count")
    require(contract["useful_bytes"] == total, "padding counted as useful")
    require(contract["payload_bytes_including_holes"] == offset,
            "padded payload extent")
    require(contract["two_hop_logical_copy_bytes"] == 2*total and
            contract["two_hop_DDR_read_plus_write_bytes"] == 4*total and
            contract["direct_logical_copy_bytes"] == total and
            contract["direct_DDR_read_plus_write_bytes"] == 2*total,
            "hop/data traffic accounting")
    source, relay, dest = 0x40000000, 0x50000000, 0x60000000
    expected_rows, expected_direct, groups = [], [], {}
    for ti in range(6):
        subset = [c for c in chunks if c["tensor"] == ti]
        fill_ids = [2*c["index"] for c in subset]
        groups[f"tensor_{ti}_fill_complete"] = fill_ids
        for channel in (0, 1):
            for c in subset:
                expected_rows.append({"id": 2*c["index"]+channel,
                    "channel": channel,
                    "src_iova": (source if channel == 0 else relay)+c["offset"],
                    "dst_iova": (relay if channel == 0 else dest)+c["offset"],
                    "bytes": c["bytes"], "dep": -1 if channel == 0 else fill_ids[-1],
                    "tensor": ti, "chunk": c["index"]})
    for c in chunks:
        expected_direct.append({"id": c["index"], "channel": 1,
            "src_iova": source+c["offset"], "dst_iova": dest+c["offset"],
            "bytes": c["bytes"], "dep": -1, "tensor": c["tensor"],
            "chunk": c["index"]})
    require(read_commands(root / "two-hop-commands.tsv") == expected_rows,
            "two-hop TSV differs from independently reconstructed DAG")
    require(read_commands(root / "resident-direct-commands.tsv") == expected_direct,
            "resident-direct TSV differs from full same-package copy")
    require(contract["barrier_groups"] == groups, "whole-tensor fill group changed")
    for jobs, rows in ((contract["jobs"], expected_rows),
                       (contract["resident_direct_jobs"], expected_direct)):
        require(len(jobs) == len(rows), "JSON job coverage count")
        for job, row in zip(jobs, rows):
            renamed = dict(row, src=row["src_iova"], dst=row["dst_iova"])
            for key in ("id", "channel", "src", "dst", "bytes", "tensor", "chunk"):
                require(job[key] == renamed[key], f"JSON job {row['id']} {key}")
            require(job["did"] == (10 if row["channel"] == 0 else 11),
                    "requester/DID changed")
            require(job["dependencies"] == ([] if row["dep"] < 0 else [row["dep"]]),
                    "JSON direct dependency changed")
            if row["dep"] >= 0:
                require(job["dependency_group"] == f"tensor_{row['tensor']}_fill_complete",
                        "JSON tensor barrier omitted")
    maps = [{"iova": iova, "pa": pa, "reserved_bytes": 16*1024*1024}
            for iova, pa in ((source, 0x84000000), (relay, 0x85000000),
                             (dest, 0x86000000))]
    require(contract["mappings"] == maps and
            contract["reserved_payload_bytes"] == 48*1024*1024,
            "physical or authorization footprint changed")
    payload = (root / "payload.bin").read_bytes()
    require(len(payload) == offset, "source payload extent")
    previous_end = 0
    for ti, tensor in enumerate(tensors):
        lo, n = tensor["offset"], tensor["bytes"]
        require(all(b == 0xA5 for b in payload[previous_end:lo]), "source hole modified")
        for wi in range(n//8):
            require(struct.unpack_from("<Q", payload, lo+wi*8)[0] == ((ti+1)<<56)|wi,
                    f"source fixture mismatch tensor {ti} word {wi}")
        previous_end = lo+n
    require(all(b == 0xA5 for b in payload[previous_end:offset]), "source tail hole")
    return contract, tensors, chunks, expected_rows, payload


def static_report(root):
    contract, tensors, chunks, rows, payload = independent_contract(root)
    useful = b"".join(payload[t["offset"]:t["offset"]+t["bytes"]] for t in tensors)
    return {"schema": "independent-ai-transfer-static-acceptance-v1", "accepted": True,
        "classification": "source-shape/command/fixture contract only; no DUT execution",
        "source_model": "opt-125m", "tensor_count": len(tensors),
        "chunks_per_hop": len(chunks), "two_hop_jobs": len(rows),
        "useful_bytes": len(useful), "source_payload_bytes_including_holes": len(payload),
        "source_useful_sha256": sha(useful), "source_payload_sha256": sha(payload),
        "full_tensor_barriers": True, "resident_direct_same_package": True,
        "RTL_function_accepted": False, "performance_G0_accepted": False,
        "AI_throughput_accepted": False,
        "inputs": {name: sha((root/name).read_bytes()) for name in
            ("transfer-contract.json", "two-hop-commands.tsv", "resident-direct-commands.tsv", "payload.bin")}}


EVENT_COLUMNS = ["cycle", "event", "job", "channel", "hw_id", "src_iova",
                 "dst_iova", "bytes", "dep", "tensor", "chunk", "value", "aux"]


def read_events(path):
    with path.open(newline="") as stream:
        reader = csv.DictReader(stream)
        require(reader.fieldnames == EVENT_COLUMNS, "unexpected hardware CSV schema")
        result = []
        for row in reader:
            require(None not in row and all(v is not None for v in row.values()),
                    "truncated hardware CSV row")
            result.append({key: value if key == "event" else int(value)
                           for key, value in row.items()})
    return result


def validate_hardware_events(rows, events):
    """Derive whole-tensor readiness from raw DUT pulses, never summary times."""
    expected = {row["id"]: row for row in rows}
    per_job = {jid: {} for jid in expected}
    per_kind = ("ready", "config_start", "next_id", "issue", "done", "done_reg")
    for event in events:
        require(event["cycle"] >= 0, "negative event time")
        if event["event"] not in per_kind:
            continue
        jid, kind = event["job"], event["event"]
        require(jid in expected, f"unknown job {jid} hardware event")
        require(kind not in per_job[jid], f"duplicate hardware {kind} job {jid}")
        row = expected[jid]
        for field in ("channel", "src_iova", "dst_iova", "bytes", "dep", "tensor", "chunk"):
            require(event[field] == row[field], f"job {jid} event {field} differs from command")
        per_job[jid][kind] = event
    for jid, record in per_job.items():
        require(set(record) == set(per_kind),
                f"job {jid} missing event {sorted(set(per_kind)-set(record))}")
    tensor_barriers = {}
    for tensor in range(6):
        fills = [row["id"] for row in rows if row["channel"] == 0 and row["tensor"] == tensor]
        tensor_barriers[tensor] = max((per_job[j]["done"]["cycle"] for j in fills), default=0)
    ids = [set(), set()]
    counts = [{"issue": 0, "done": 0, "bytes": 0},
              {"issue": 0, "done": 0, "bytes": 0}]
    for jid, record in per_job.items():
        row = expected[jid]
        ch = row["channel"]
        required_ready = 0 if row["dep"] == -1 else tensor_barriers[row["tensor"]]
        require(record["ready"]["cycle"] == required_ready,
                f"whole-tensor logical-ready violation job {jid}")
        require(record["issue"]["cycle"] >= required_ready,
                f"copy precedes same-tensor late fill DONE job {jid}")
        require(record["config_start"]["cycle"] >= required_ready,
                f"copy config precedes full tensor fill job {jid}")
        require(record["config_start"]["cycle"] <= record["issue"]["cycle"]
                <= record["done"]["cycle"] <= record["done_reg"]["cycle"],
                f"hardware/config completion order job {jid}")
        require(record["config_start"]["cycle"] <= record["next_id"]["cycle"]
                <= record["done_reg"]["cycle"], f"trigger order job {jid}")
        hwid = record["next_id"]["hw_id"]
        require(hwid > 0 and hwid not in ids[ch], f"duplicate/invalid NEXT_ID channel {ch}")
        ids[ch].add(hwid)
        require(record["done_reg"]["hw_id"] == hwid, f"DONE register ID mismatch job {jid}")
        for kind in ("issue", "done"):
            eid = record[kind]["hw_id"]
            require(eid == hwid or (eid == -1 and record[kind]["cycle"] <=
                                   record["next_id"]["cycle"]),
                    f"pulse/NEXT_ID binding mismatch job {jid}")
            counts[ch][kind] += 1
        counts[ch]["bytes"] += row["bytes"]
    # The adapter's seed is strictly one active job. At equal edges process a
    # completion before the next issue, matching the DUT scoreboard boundary.
    active = [None, None]
    edge_events = sorted((e for e in events if e["event"] in ("issue", "done")),
                         key=lambda e: (e["cycle"], 0 if e["event"] == "done" else 1))
    for event in edge_events:
        ch, jid = event["channel"], event["job"]
        if event["event"] == "issue":
            require(active[ch] is None, f"same-channel overlapping active jobs {ch}")
            active[ch] = jid
        else:
            require(active[ch] == jid, f"DONE without matching active ISSUE job {jid}")
            active[ch] = None
    require(active == [None, None], "outstanding job at hardware trace end")
    final_done = max(record["done"]["cycle"] for record in per_job.values())
    quiet = [event for event in events if event["event"] == "quiet"]
    require(len(quiet) == 1 and quiet[0]["cycle"] >= final_done,
            "missing/early/duplicate hardware quiet event")
    return per_job, counts, tensor_barriers, quiet[0]["cycle"]


def validate_output_bytes(tensors, source, target):
    require(len(target) == len(source), "hardware output extent differs from source-layout extent")
    source_useful, target_useful, tensor_results = [], [], []
    source_padding, target_padding, previous_end = [], [], 0
    for tensor in tensors:
        lo, n = tensor["offset"], tensor["bytes"]
        source_padding.append(source[previous_end:lo])
        target_padding.append(target[previous_end:lo])
        before, after = source[lo:lo+n], target[lo:lo+n]
        require(len(after) == n and after == before,
                f"raw output bytes differ in complete tensor {tensor['name']}")
        source_useful.append(before)
        target_useful.append(after)
        tensor_results.append({"name": tensor["name"], "bytes": n,
                               "source_sha256": sha(before), "target_sha256": sha(after)})
        previous_end = lo+n
    source_padding.append(source[previous_end:])
    target_padding.append(target[previous_end:])
    source_useful, target_useful = b"".join(source_useful), b"".join(target_useful)
    source_padding, target_padding = b"".join(source_padding), b"".join(target_padding)
    require(len(source_useful) == len(target_useful) == 9447936,
            "padding counted as hardware useful progress")
    return {"useful_bytes": len(source_useful),
            "source_useful_sha256": sha(source_useful),
            "target_useful_sha256": sha(target_useful),
            "source_padding_bytes": len(source_padding),
            "target_padding_bytes": len(target_padding),
            "source_padding_sha256": sha(source_padding),
            "target_padding_sha256": sha(target_padding),
            "tensors": tensor_results}, target_padding


STAT_FIELDS = ["issue", "done", "ar", "aw", "r", "w", "b", "ar_bytes",
               "r_bytes", "aw_bytes", "w_bytes", "errors", "tl_accept", "tl_resp"]


def stat_check(stat, nbytes, nissue, where):
    require(set(stat) == set(STAT_FIELDS), f"{where} stat schema changed")
    require(all(type(stat[k]) is int and stat[k] >= 0 for k in STAT_FIELDS),
            f"{where} nonnumeric/negative counters")
    require(stat["issue"] == stat["done"] == nissue, f"{where} hardware completion counts")
    require(stat["ar_bytes"] == stat["r_bytes"] == stat["aw_bytes"]
            == stat["w_bytes"] == nbytes, f"{where} AR/R/AW/W payload conservation")
    require(stat["r"]*8 == nbytes and stat["w"]*8 == nbytes,
            f"{where} 64bit beat/strobe counts")
    require(stat["aw"] == stat["b"] and stat["errors"] == 0,
            f"{where} AW/B completion/errors")
    require(stat["tl_accept"] == stat["tl_resp"], f"{where} TL requests/responses")
    if nbytes:
        require(stat["ar"] > 0 and stat["aw"] > 0 and stat["tl_accept"] > 0,
                f"{where} missing real DMA/TL activity")
    else:
        require(all(stat[k] == 0 for k in STAT_FIELDS), f"{where} inactive requester did work")


def validate_record_objects(root, run, context, summary, events, target):
    contract, tensors, chunks, two_rows, source = context
    require(summary["schema"] == "ai-fpga-transfer-functional-v1", "wrong hardware summary schema")
    require(summary["scope"] == "functional_serial_seed_only" and summary["pass"] is True,
            "not a completed bounded RTL functional seed")
    require(summary["mode"] in ("two-hop", "resident-direct"), "unknown real-DMA mode")
    two_hop = summary["mode"] == "two-hop"
    rows = two_rows if two_hop else read_commands(root / "resident-direct-commands.tsv")
    require(summary["useful_bytes"] == 9447936 and summary["payload_bytes_including_holes"] == len(source),
            "hardware summary source/padding counts")
    require(summary["cpu_enable"] == summary["policy_enable"] == summary["nonblock_enable"] == 0,
            "functional seed configuration changed")
    require(summary["model_only_metadata"] is True and summary["ptw_latency"] == 1,
            "frozen diagnostic model configuration changed")
    require(type(summary["common_t0_absolute_cycle"]) is int and summary["common_t0_absolute_cycle"] >= 0,
            "missing common timebase")
    allowed_events = set(("ready", "config_start", "next_id", "issue", "done", "done_reg",
                          "common_start", "barrier_ready", "quiet", "ddr_ar", "ddr_aw", "ddr_b"))
    require(all(e["event"] in allowed_events for e in events), "unknown hardware event kind")
    common = [e for e in events if e["event"] == "common_start"]
    require(len(common) == 1 and common[0]["cycle"] == 0 and common[0]["value"] == len(source)
            and common[0]["aux"] == 9447936, "common-start metadata")
    raw, counts, barriers, quiet_cycle = validate_hardware_events(rows, events)
    quiet_event = next(e for e in events if e["event"] == "quiet")
    require(quiet_event["value"] == 3 and quiet_event["aux"] == 0,
            "raw quiet DMA/TL flags")
    require(summary["quiet_cycle"] == quiet_cycle, "summary/raw quiet timestamp differs")
    expected = {row["id"]: row for row in rows}
    require(len(summary["jobs"]) == len(rows), "summary job list missing/extra")
    summary_ids = [j["id"] for j in summary["jobs"]]
    require(summary_ids == [r["id"] for r in rows], "summary job identity/order differs from input")
    summaries = {j["id"]: j for j in summary["jobs"]}
    raw_bus = {jid: {kind: [] for kind in ("ddr_ar", "ddr_aw", "ddr_b")} for jid in expected}
    for event in events:
        if event["event"] not in ("ddr_ar", "ddr_aw", "ddr_b"):
            continue
        jid = event["job"]
        require(jid in expected, "bus header belongs to unknown job")
        for key in ("channel", "src_iova", "dst_iova", "bytes", "dep", "tensor", "chunk"):
            require(event[key] == expected[jid][key], f"job {jid} bus identity {key}")
        require(raw[jid]["issue"]["cycle"] <= event["cycle"] <= raw[jid]["done"]["cycle"],
                f"job {jid} bus transaction outside real issue/DONE")
        raw_bus[jid][event["event"]].append(event)
    accumulated = [{key: 0 for key in STAT_FIELDS} for _ in range(2)]
    last_done = [0, 0]
    last_global_done = 0
    for row in rows:
        jid, ch = row["id"], row["channel"]
        job, record, bus = summaries[jid], raw[jid], summaries[jid]["bus"]
        for key, value in row.items():
            require(job[key] == value, f"job {jid} summary command {key}")
        require(job["did"] == 10+ch, "summary requester identity changed")
        for skey, kind in (("ready", "ready"), ("config_start", "config_start"),
                           ("issue", "issue"), ("done", "done"),
                           ("next_cycle", "next_id"), ("done_reg_cycle", "done_reg")):
            require(job[skey] == record[kind]["cycle"], f"job {jid} summary/raw {skey}")
        require(job["next_id"] == record["next_id"]["hw_id"] == record["next_id"]["value"]
                and job["done_id"] == record["done_reg"]["hw_id"] == record["done_reg"]["value"]
                == job["next_id"], f"job {jid} NEXT/DONE reserved-ID binding")
        require(job["channel_ready"] == last_done[ch], f"job {jid} channel capacity ready")
        require(job["config_start"] >= last_global_done and job["issue"] >= last_done[ch],
                "serial function seed contains overlapping job submission")
        last_done[ch], last_global_done = job["done"], job["done"]
        stat_check(bus, row["bytes"], 1, f"job {jid}")
        for kind, stat_name, base_iova, base_pa, offset_field in (
                ("ddr_ar", "ar", row["src_iova"], None, "src_iova"),
                ("ddr_aw", "aw", row["dst_iova"], None, "dst_iova")):
            region = base_iova & ~((256*1024*1024)-1)
            pa_base = {0x40000000: 0x84000000, 0x50000000: 0x85000000,
                       0x60000000: 0x86000000}[region]
            first = pa_base+(base_iova-region)
            cursor = first
            headers = sorted(raw_bus[jid][kind], key=lambda e: e["aux"])
            require(len(headers) == bus[stat_name], f"job {jid} raw {kind} count")
            for header in headers:
                require(header["aux"] == cursor and header["value"] > 0 and header["value"]%8 == 0,
                        f"job {jid} raw {kind} has gap/overlap/alignment error")
                cursor += header["value"]
            require(cursor == first+row["bytes"], f"job {jid} raw {kind} bytes incomplete")
        replies = raw_bus[jid]["ddr_b"]
        require(len(replies) == bus["b"] and all(e["value"] == 0 for e in replies),
                f"job {jid} raw B responses/errors")
        require(all(((e["aux"]>>4)&3) == 2 and ((e["aux"]>>2)&1) == ch for e in replies),
                f"job {jid} raw B requester ID")
        for key in STAT_FIELDS:
            accumulated[ch][key] += bus[key]
    require(len(summary["channels"]) == 2, "requester count changed")
    for ch in range(2):
        channel = summary["channels"][ch]
        stat_check(channel, counts[ch]["bytes"], counts[ch]["issue"], f"requester {ch}")
        require(channel == accumulated[ch], f"requester {ch} aggregate != all raw-owned jobs")
    barriers_raw = [e for e in events if e["event"] == "barrier_ready"]
    require(len(barriers_raw) == (6 if two_hop else 0), "raw tensor barrier count")
    require(len(summary["tensors"]) == 6, "summary missing full tensor")
    for ti, tensor in enumerate(tensors):
        actual = summary["tensors"][ti]
        for key in ("name", "offset", "bytes"):
            require(actual[key] == tensor[key], f"tensor {ti} summary {key}")
        fill_rows = [r for r in rows if r["channel"] == 0 and r["tensor"] == ti]
        require(actual["tensor"] == ti and actual["expected_fill_count"]
                == actual["completed_fill_count"] == len(fill_rows)
                and actual["expected_fill_bytes"] == actual["completed_fill_bytes"]
                == sum(r["bytes"] for r in fill_rows), f"tensor {ti} fill conservation")
        require(actual["barrier_cycle"] == (barriers[ti] if two_hop else -1),
                f"tensor {ti} summary/raw complete-fill barrier")
        if two_hop:
            points = [e for e in barriers_raw if e["tensor"] == ti]
            require(len(points) == 1 and points[0]["cycle"] == barriers[ti]
                    and points[0]["value"] == len(fill_rows)
                    and points[0]["aux"] == tensor["bytes"], f"tensor {ti} raw barrier byte/count")
    all_ddr = summary["all_ddr"]
    require(set(all_ddr) == set(STAT_FIELDS) and all(type(v) is int and v >= 0 for v in all_ddr.values()),
            "all-DDR counter schema/negative")
    require(all_ddr["ar_bytes"] == all_ddr["r_bytes"] == all_ddr["r"]*8
            and all_ddr["aw_bytes"] == all_ddr["w_bytes"] == all_ddr["w"]*8
            and all_ddr["aw"] == all_ddr["b"] and all_ddr["errors"] == 0,
            "global DDR request/response/payload conservation or error")
    for key in ("aw", "w", "b", "aw_bytes", "w_bytes"):
        require(all_ddr[key] == sum(s[key] for s in accumulated), f"unexpected global DDR {key}")
    for key in ("ar", "r", "ar_bytes", "r_bytes"):
        require(all_ddr[key] >= sum(s[key] for s in accumulated), f"global DDR {key} lost payload")
    terminal_expected = {"dma_job_quiet": 3, "tl_in_flight": 0,
        "cpu_quiet": 1, "cpu_wr_quiet": 1, "cpu_issued": 0, "cpu_completed": 0,
        "memory_r_queue": 0, "memory_aw_queue": 0, "memory_b_queue": 0,
        "observer_r_queue": 0, "observer_aw_queue": 0, "observer_b_queue": 0, "ddr_valid": 0}
    require(summary["terminal"] == terminal_expected, "both requester/TL/DDR queues not finally drained")
    require(summary["launch_errors"] == 0 and summary["stability_samples"] > 0,
            "launch error or missing protocol stability sampling")
    for key in ("source_match", "relay_match", "destination_match", "padding_preserved"):
        require(summary[key] is True, f"driver runtime integrity assertion {key} failed")
    mapping = summary["mapping"]
    require(mapping["diagnostic_metadata_only"] is True and mapping["page_bytes"] == 4096
            and mapping["region_bytes"] == 16*1024*1024 and mapping["payload_reserved_bytes"] == 48*1024*1024,
            "diagnostic mapping/authorization footprint changed")
    require(mapping["pte_pages"] == 52 and mapping["pte_reserved_bytes"] == 52*4096
            and mapping["ddt_reserved_bytes"] == 4096
            and mapping["metadata_reserved_bytes"] == 53*4096,
            "PTE/DDT cost not itemized at actual diagnostic footprint")
    byte_result, target_padding = validate_output_bytes(tensors, source, target)
    require(all(b == 0xD9 for b in target_padding), "target padding modified/counting out-of-range writes")
    return {"schema": "independent-ai-transfer-RTL-function-acceptance-v1", "accepted": True,
        "classification": "actual-iDMA RTL serial functional seed; no performance/AI application result",
        "mode": summary["mode"], "jobs": len(rows), "tensors": 6,
        "raw_hardware_issue_done_unique": True, "whole_tensor_ready_from_raw_DONE": two_hop,
        "raw_DDR_headers_complete_and_owned": True,
        "bus_check_boundary": "raw AR/AW/B headers independently checked; R/W/TL handshake totals checked from runtime observer counters, no independent per-beat replay",
        "full_source_relay_region_check_boundary": "driver runtime assertions; raw source payload and final target six-tensor/holes independently checked",
        "channels": summary["channels"], "metadata_read_bytes": all_ddr["ar_bytes"]-sum(s["ar_bytes"] for s in accumulated),
        "complete_natural_drain": True, "quiet_cycle_for_event_order_only": quiet_cycle,
        "bytes": byte_result, "mapping": mapping,
        "performance_G0_accepted": False, "AI_throughput_accepted": False,
        "G1_candidate_accepted": False,
        "inputs": {str(run/name): sha((run/name).read_bytes()) for name in
                   ("events.csv", "summary.json", "output.bin")}}


def hardware_report(root, run):
    return validate_record_objects(root, run, independent_contract(root),
        json.loads((run/"summary.json").read_text()), read_events(run/"events.csv"),
        (run/"output.bin").read_bytes())


def write_events(path, events):
    with path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=EVENT_COLUMNS)
        writer.writeheader()
        writer.writerows(events)


def negative_report(root, run, directory):
    context = independent_contract(root)
    summary = json.loads((run/"summary.json").read_text())
    events, target = read_events(run/"events.csv"), (run/"output.bin").read_bytes()
    baseline = validate_record_objects(root, run, context, summary, events, target)
    require(summary["mode"] == "two-hop", "whole-tensor barrier negatives require two-hop originals")
    original_hashes = {name: sha((run/name).read_bytes()) for name in ("events.csv", "summary.json", "output.bin")}
    cases = []
    # Three fault classes. Completion integrity tests both missing and duplicate
    # pulses because their rejection obligations differ.
    for name in ("early_tensor_copy", "missing_hardware_done", "duplicate_hardware_done", "changed_useful_byte"):
        folder = directory/name
        folder.mkdir(parents=True, exist_ok=True)
        changed_events = [dict(e) for e in events]
        changed_target = target
        if name == "early_tensor_copy":
            own_fill_done = next(e["cycle"] for e in events if e["event"] == "done" and e["job"] == 0)
            changed = {"ready": 0, "config_start": 1, "issue": 2,
                       "next_id": 3, "done": 4, "done_reg": 5}
            for event in changed_events:
                if event["job"] == 1 and event["event"] in changed:
                    event["cycle"] = own_fill_done+changed[event["event"]]
        elif name == "missing_hardware_done":
            changed_events = [e for e in changed_events if not (e["event"] == "done" and e["job"] == 0)]
        elif name == "duplicate_hardware_done":
            changed_events.append(dict(next(e for e in events if e["event"] == "done" and e["job"] == 0)))
        else:
            changed_target = bytearray(target)
            changed_target[context[1][3]["offset"]] ^= 1
            changed_target = bytes(changed_target)
        write_events(folder/"events.csv", changed_events)
        shutil.copyfile(run/"summary.json", folder/"summary.json")
        (folder/"output.bin").write_bytes(changed_target)
        try:
            validate_record_objects(root, folder, context, summary, changed_events, changed_target)
        except (Rejection, KeyError, ValueError, OSError) as exc:
            cases.append({"name": name, "rejected": True, "reason": str(exc),
                          "modified_records": str(folder)})
        else:
            cases.append({"name": name, "rejected": False, "reason": "checker accepted damaged records"})
    require({name: sha((run/name).read_bytes()) for name in original_hashes} == original_hashes,
            "negative-case generator modified original records")
    return {"schema": "independent-ai-transfer-negative-acceptance-v1",
        "accepted": all(case["rejected"] for case in cases), "fault_classes": 3,
        "fault_variants": len(cases), "cases": cases, "baseline_mode": baseline["mode"],
        "originals_unchanged": True, "original_hashes": original_hashes,
        "scope": "record/byte corruption rejection only; no new DUT execution"}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, default=Path(__file__).resolve().parent.parent)
    ap.add_argument("--static", action="store_true")
    ap.add_argument("--run", type=Path)
    ap.add_argument("--negative-cases", action="store_true")
    ap.add_argument("--report", type=Path, required=True)
    args = ap.parse_args()
    try:
        require(args.static != bool(args.run), "select exactly --static or --run")
        require(not args.negative_cases or args.run, "negative cases require actual RTL records")
        if args.static:
            report = static_report(args.root)
        elif args.negative_cases:
            report = negative_report(args.root, args.run, args.report.parent/"negative-records")
        else:
            report = hardware_report(args.root, args.run)
    except (Rejection, KeyError, ValueError, OSError) as exc:
        report = {"accepted": False, "reason": str(exc)}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2)+"\n")
    print(json.dumps({k: report[k] for k in ("accepted", "classification", "reason") if k in report}))
    return 0 if report["accepted"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
