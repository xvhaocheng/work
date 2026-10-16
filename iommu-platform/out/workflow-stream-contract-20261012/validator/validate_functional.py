#!/usr/bin/env python3
"""Validate the fixed HERO host functional run without rerunning its driver.

python3 validate_functional.py RUN_DIR --negative-tests --output acceptance.json --ledger execution.json
The 20-field trace proves host program/slot ordering only. CLOCK_MONOTONIC
timestamps are never interpreted as target cycles or asynchronous DMA timing.
"""
from __future__ import annotations

import argparse
import copy
import csv
import datetime as dt
import hashlib
import json
import re
import shlex
import struct
import sys
import time
from pathlib import Path

COMMIT = "c8481a930a15a385d4f58f519bed83d2e75e2a73"
SOURCE_SHA = "8b16b51169bb31b7eb290eb5f309ddfbba25fa94e3fae3b327baadf491360186"
ADAPTER_SHA = "4bb74f87e0b868841d2f1b718a2705f29ddf188d6006029387b75f7bfff00264"
W, SH, NS, SIZE, VALUES = 256, 16, 16, 16384, 65536
FIELDS = ["event_id", "host_ns", "thread_id", "event", "role", "matrix", "stripe", "tile",
          "linear_tile", "slot", "generation", "byte_offset", "size_bytes", "job_id", "a_slot",
          "a_generation", "b_slot", "b_generation", "c_slot", "c_generation"]
TEXT = {"event", "role", "matrix"}
KINDS = {"slot_alloc", "copy_issue", "copy_done", "wait", "compute_start", "compute_done",
         "slot_free", "ref_check", "app_complete"}


class Rejected(ValueError):
    pass


def need(ok, message):
    if not ok:
        raise Rejected(message)


def equal(obj, key, expected, label):
    need(key in obj, f"{label}: missing {key}")
    actual = obj[key]
    if isinstance(expected, bool):
        need(type(actual) is bool, f"{label}.{key}: expected boolean")
    elif isinstance(expected, int):
        need(type(actual) is int, f"{label}.{key}: expected integer")
    need(actual == expected, f"{label}.{key}: {actual!r} != {expected!r}")


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def source_checks(batch, run_dir):
    original = batch / "source-evidence/hero/openmp-examples/mm-large/mm-large.c"
    adapter = batch / "proposed/hero-mm-functional.c"
    manifest_path = batch / "proposed/functional-source-manifest.json"
    interface_path = batch / "proposed/functional-build-argv.json"
    provenance_path = batch / "functional-execution/run-provenance.json"
    need(digest(original) == SOURCE_SHA, "pinned upstream source hash changed")
    need(digest(adapter) == ADAPTER_SHA, "reviewed host adapter hash changed")
    manifest = json.loads(manifest_path.read_text())
    equal(manifest, "source_commit", COMMIT, "source manifest")
    equal(manifest, "source_sha256", SOURCE_SHA, "source manifest")
    original_function = "".join(original.read_text().splitlines(keepends=True)[43:276])
    equal(manifest, "original_function_sha256", hashlib.sha256(original_function.encode()).hexdigest(), "source manifest")
    adapted = adapter.read_text().split("/* BEGIN extracted actual upstream function: only trace blocks added below. */", 1)[1]
    adapted = adapted.split("/* END extracted upstream function. */", 1)[0].strip("\n") + "\n"
    for kind in ("start", "done"):
        block = ('\n        #pragma omp single\n        {\n'
                 f'          trace_compute("compute_{kind}", s, t, a_ptrs[!a_idx],\n'
                 '                        b_ptrs[!b_idx], c_ptrs[c_idx]);\n        }\n')
        need(adapted.count(block) == 1, f"missing/extra declared compute_{kind} observation block")
        adapted = adapted.replace(block, "", 1)
    need(adapted == original_function, "original function changed beyond the two declared observation blocks")
    interface = json.loads(interface_path.read_text())
    need(interface["trace_interface"]["columns"] == FIELDS, "fixed 20-field interface changed")
    provenance = json.loads(provenance_path.read_text())
    equal(provenance, "functional_run_count", 1, "run provenance")
    equal(provenance, "target_DMA_runs", 0, "run provenance")
    provenance_files = {Path(x["path"]).resolve(): x["sha256"] for x in provenance["files"]}
    for path in (adapter, run_dir / "trace.csv", run_dir / "output.bin", run_dir / "result.json",
                 batch / "functional-execution/hero-mm-functional"):
        need(path.resolve() in provenance_files and digest(path) == provenance_files[path.resolve()],
             f"actual run provenance hash mismatch: {path.name}")
    return {"source_commit": COMMIT, "source_sha256": SOURCE_SHA, "adapter_sha256": ADAPTER_SHA,
            "original_function_equal_after_removing_two_observation_blocks": True,
            "original_function_lines": [44, 276], "input_initialization_lines": [320, 326],
            "host_reference_lines": [335, 346], "trace_columns": FIELDS,
            "run_provenance_sha256": digest(provenance_path)}


def read_events(path):
    with path.open(newline="") as stream:
        rows = csv.DictReader(stream)
        need(rows.fieldnames == FIELDS, "CSV header is not the fixed 20-field interface")
        events = []
        for line, row in enumerate(rows, 2):
            need(None not in row and all(row[x] is not None for x in FIELDS), f"malformed CSV line {line}")
            for field in set(FIELDS) - TEXT:
                try:
                    row[field] = int(row[field], 10)
                except ValueError as error:
                    raise Rejected(f"invalid numeric field at CSV line {line}: {field}") from error
            events.append(row)
        return events


def full_reference():
    # Reconstruct the pinned initialization and compute a genuine full dot
    # product for every C[i,j] with B[j,k], independently of the C driver.
    a = [list(range(i*W, (i+1)*W)) for i in range(W)]
    b = [[2 if j == k else 0 for k in range(W)] for j in range(W)]
    reference = [sum(a[i][k] * b[j][k] for k in range(W)) & 0xFFFFFFFF
                 for i in range(W) for j in range(W)]
    need(reference == [(2*i) & 0xFFFFFFFF for i in range(VALUES)], "full reference contradicts pinned 2A initialization")
    return reference


def event_fields(event, role, **values):
    expected = {x: -1 for x in FIELDS if x not in TEXT | {"event_id", "host_ns", "thread_id"}}
    expected.update(event=event, role=role, matrix="-", size_bytes=0)
    expected.update(values)
    return expected


def fields_match(event, expected):
    for key, value in expected.items():
        equal(event, key, value, f"event {event['event_id']}")


def verify_data(output, reference):
    need(len(output) == VALUES*4, "output.bin is not exactly 65536 little-endian uint32 values")
    values = struct.unpack(f"<{VALUES}I", output)
    for index, (got, expected) in enumerate(zip(values, reference)):
        need(got == expected, f"output[{index}]={got}, independently computed C=A*B^T requires {expected}")
    return sum(values)


def verify_events(events):
    slots, jobs, inputs, outputs, compute_done = {}, {}, {}, {}, {}
    copy_count = {"A": 0, "B": 0, "C": 0}
    allocation_order = [(matrix, slot) for matrix in "ABC" for slot in range(2)]
    freed = []
    active_copy = {}
    active_compute = None
    next_compute = 0
    ref_check = app_complete = None
    previous_ns = -1

    def ready_input(matrix, index):
        need((matrix, index) in inputs, f"missing {matrix} input {index}")
        job = jobs[inputs[matrix, index]]
        need(job["done"] is not None and job["wait"] is not None, f"compute before {matrix}{index} copy DONE/wait")
        return job

    def prior_output_done(stripe, require_wait=False):
        need(stripe in outputs, f"C slot reuse before output {stripe} was issued")
        job = jobs[outputs[stripe]]
        need(job["done"] is not None, f"C slot reused before output {stripe} COPY DONE")
        need(not require_wait or job["wait"] is not None, f"C slot reused before output {stripe} original wait")

    for ordinal, e in enumerate(events, 1):
        equal(e, "event_id", ordinal, "CSV")
        need(type(e["host_ns"]) is int and e["host_ns"] >= previous_ns >= -1, "host observation timestamps run backwards")
        previous_ns = e["host_ns"]
        need(e["event"] in KINDS and 0 <= e["thread_id"] < 8, "unknown event or out-of-team thread")
        need(app_complete is None, "event follows complete application")
        kind, matrix, slot, index = e["event"], e["matrix"], e["slot"], e["stripe"]
        if kind == "slot_alloc":
            need(not jobs and len(slots) < 6 and (matrix, slot) == allocation_order[len(slots)], "six-slot allocation order/count changed")
            fields_match(e, event_fields(kind, "allocator", matrix=matrix, slot=slot, size_bytes=SIZE))
            need(e["thread_id"] == 0, "allocation is not original host thread")
            slots[matrix, slot] = {"generation": -1, "active": True}
        elif kind == "copy_issue":
            need(len(slots) == 6 and matrix in copy_count and (matrix, slot) in slots and slots[matrix, slot]["active"], "copy has no allocated live slot")
            controller = "ABC".index(matrix)
            need(e["thread_id"] == controller and controller not in active_copy, "copy manager/CPU synchronous-call owner invalid")
            sequence = copy_count[matrix]
            need(sequence < (256 if matrix == "B" else 16), "copy beyond finite target")
            expected_index = sequence % 16 if matrix == "B" else sequence
            generation = sequence // 2
            need(slot == sequence % 2 and index == expected_index, "copy slot/stripe order changed")
            if matrix in "AB":
                if sequence >= 2:
                    old_tile = (sequence-2)*16+15 if matrix == "A" else sequence-2
                    need(old_tile in compute_done, f"{matrix} slot overwritten before previous owner's final compute DONE (tile {old_tile})")
                need(slots[matrix, slot]["generation"] == generation-1, "input slot generation skipped/repeated")
                slots[matrix, slot]["generation"] = generation
            else:
                need(sequence*16+15 in compute_done, "C output begins before all 16 row-stripe tiles complete")
                need(slots[matrix, slot]["generation"] == generation, "C output uses wrong slot generation")
                if sequence == 15:
                    prior_output_done(14, require_wait=True)
            ident = e["job_id"]
            need(ident == len(jobs)+1, "copy job IDs must be unique, positive, contiguous")
            expected = event_fields(kind, f"controller_{matrix}", matrix=matrix, stripe=expected_index,
                                    tile=expected_index if matrix == "B" else -1,
                                    linear_tile=sequence if matrix == "B" else -1,
                                    slot=slot, generation=generation, byte_offset=expected_index*SIZE,
                                    size_bytes=SIZE, job_id=ident)
            fields_match(e, expected)
            jobs[ident] = {"expected": expected, "thread": controller, "issue": ordinal, "done": None, "wait": None}
            (outputs if matrix == "C" else inputs)[sequence if matrix == "C" else (matrix, sequence)] = ident
            active_copy[controller] = ident
            copy_count[matrix] += 1
        elif kind in ("copy_done", "wait"):
            ident = e["job_id"]
            need(ident in jobs, "COPY DONE/wait references unknown job")
            job = jobs[ident]
            expected = dict(job["expected"], event=kind)
            fields_match(e, expected)
            need(e["thread_id"] == job["thread"], "COPY DONE/wait belongs to wrong manager thread")
            if kind == "copy_done":
                need(job["done"] is None and active_copy.get(job["thread"]) == ident, "duplicate/out-of-call COPY DONE")
                job["done"] = ordinal
                del active_copy[job["thread"]]
            else:
                need(job["done"] is not None and job["wait"] is None, "wait before COPY DONE or duplicate wait")
                job["wait"] = ordinal
        elif kind in ("compute_start", "compute_done"):
            q = e["linear_tile"]
            s, t = divmod(q, NS)
            need(0 <= q < 256, "tile outside fixed 16x16 grid")
            expected = event_fields(kind, "compute_observer", matrix="C", stripe=s, tile=t, linear_tile=q,
                                    slot=s % 2, generation=s // 2, byte_offset=(s*SH*W+t*SH)*4,
                                    size_bytes=SH*SH*4, a_slot=s % 2, a_generation=s // 2,
                                    b_slot=q % 2, b_generation=q // 2, c_slot=s % 2, c_generation=s // 2)
            fields_match(e, expected)
            if kind == "compute_start":
                need(active_compute is None and q == next_compute, "compute overlap, tile skipped, or duplicate tile")
                ready_input("A", s)
                ready_input("B", q)
                need(slots["A", s % 2]["generation"] == s // 2 and slots["B", q % 2]["generation"] == q // 2,
                     "compute reads a replaced A/B generation")
                # Original manager calls prefetch next input before the barrier.
                if q < 255:
                    need(("B", q+1) in inputs and jobs[inputs["B", q+1]]["done"] is not None,
                         "missing source-equivalent next-B synchronous prefetch")
                if t == 0:
                    if s < 15:
                        need(("A", s+1) in inputs and jobs[inputs["A", s+1]]["done"] is not None,
                             "missing source-equivalent next-A synchronous prefetch")
                    if s > 0:
                        prior_output_done(s-1)
                    if s >= 2:
                        prior_output_done(s-2, require_wait=True)
                    need(slots["C", s % 2]["generation"] == s // 2-1, "C generation reused/skipped")
                    slots["C", s % 2]["generation"] = s // 2
                need(slots["C", s % 2]["generation"] == s // 2, "C tile writes wrong row generation")
                active_compute = q
            else:
                need(active_compute == q and q not in compute_done, "compute DONE without matching START")
                compute_done[q] = ordinal
                active_compute = None
                next_compute += 1
        elif kind == "slot_free":
            need(copy_count == {"A": 16, "B": 256, "C": 16} and len(compute_done) == 256 and
                 active_compute is None and not active_copy and all(j["done"] is not None and j["wait"] is not None for j in jobs.values()),
                 "slot freed before complete computation/copy/wait drain")
            need(len(freed) < 6 and (matrix, slot) == allocation_order[len(freed)] and slots[matrix, slot]["active"], "slot free order/count invalid")
            fields_match(e, event_fields(kind, "allocator", matrix=matrix, slot=slot,
                                        generation=slots[matrix, slot]["generation"], size_bytes=SIZE))
            need(e["thread_id"] == 0, "slot freed by wrong host thread")
            slots[matrix, slot]["active"] = False
            freed.append((matrix, slot))
        elif kind in ("ref_check", "app_complete"):
            fields_match(e, event_fields(kind, "application", matrix="C", size_bytes=VALUES*4))
            need(e["thread_id"] == 0 and len(freed) == 6, "application completes before all six slots freed")
            if kind == "ref_check":
                need(ref_check is None, "duplicate reference marker")
                ref_check = ordinal
            else:
                need(ref_check is not None, "application complete without reference marker")
                app_complete = ordinal

    need(copy_count == {"A": 16, "B": 256, "C": 16} and len(jobs) == 288 and len(compute_done) == 256,
         "incomplete fixed copy/tile workload")
    need(active_compute is None and not active_copy and all(j["done"] is not None and j["wait"] is not None for j in jobs.values()),
         "copy/compute/wait not fully drained")
    need(len(freed) == 6 and not any(s["active"] for s in slots.values()) and ref_check is not None and app_complete == len(events),
         "missing complete application/slot release")
    return {"event_count": len(events), "A_input_copies": 16, "B_input_copies": 256, "C_output_copies": 16,
            "copy_jobs": 288, "copy_done_events": 288, "wait_events": 288, "bytes_per_copy": SIZE,
            "total_copied_bytes": 288*SIZE, "compute_tile_pairs": 256, "tile_rows": 16, "tile_columns": 16,
            "tile_inner_width": 256, "source_determined_uint32_MAC_steps": 16777216,
            "slot_count": 6, "peak_live_slot_bytes": 6*SIZE, "final_slot_generations": {"A": 7, "B": 127, "C": 7},
            "host_ns_order": "monotonic observation only; no target duration inferred",
            "all_jobs_done_and_waited": True, "all_six_slots_freed": True, "app_complete_event": app_complete}


def verify_result(result, output_sum, counts):
    expected = {"execution_mode": "HOST_SYNCHRONOUS_COPY_FUNCTIONAL", "source_commit": COMMIT,
                "width": 256, "height": 256, "stripe_height": 16, "requested_threads": 8,
                "observed_team_threads": 8, "slots": 6, "slot_bytes": SIZE, "A_jobs": 16, "B_jobs": 256,
                "C_jobs": 16, "jobs": 288, "compute_starts": 256, "compute_dones": 256,
                "uint32_multiply_add_steps": 16777216, "extra_single_barriers": 512,
                "all_jobs_completed_and_waited": True, "host_reference_and_2a_mismatches": 0,
                "output_values": VALUES, "output_format": "uint32_little_endian", "output_sum_u64": output_sum,
                "reference_check_pass": True, "timing_claim": "NONE: HOST timestamps are observation order only"}
    for key, value in expected.items():
        equal(result, key, value, "result.json")
    need(counts["copy_jobs"] == result["jobs"], "self-reported jobs differ from raw events")


def negative_checks(output, reference, events):
    records = []
    def rejected(label, check):
        try:
            check()
        except Rejected as error:
            records.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise Rejected(f"negative check accepted {label}")
    altered = bytearray(output)
    altered[0] ^= 1
    rejected("one output value changed", lambda: verify_data(altered, reference))
    removed = copy.deepcopy(events)
    del removed[next(i for i, e in enumerate(removed) if e["event"] == "copy_done")]
    for ordinal, event in enumerate(removed, 1):
        event["event_id"] = ordinal
    rejected("one COPY DONE removed with event IDs renumbered", lambda: verify_events(removed))
    early = copy.deepcopy(events)
    moved = early.pop(next(i for i, e in enumerate(early) if e["event"] == "copy_issue" and e["matrix"] == "B" and e["linear_tile"] == 2))
    start = next(i for i, e in enumerate(early) if e["event"] == "compute_start" and e["linear_tile"] == 0)
    moved["host_ns"] = early[start]["host_ns"]
    early.insert(start+1, moved)
    for ordinal, event in enumerate(early, 1):
        event["event_id"] = ordinal
    rejected("B slot overwritten before old tile DONE, IDs/time kept monotonic", lambda: verify_events(early))
    return records


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("run_dir", type=Path)
    parser.add_argument("--negative-tests", action="store_true")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--ledger", type=Path)
    args = parser.parse_args()
    batch = Path(__file__).resolve().parent.parent
    started = dt.datetime.now(dt.timezone.utc).isoformat()
    clock_start = time.monotonic()
    ledger = {"schema": "functional-validator-execution-v1", "author": "Codex",
              "requested_model": "gpt-6.1-sol", "requested_reasoning_effort": "ultra",
              "command": shlex.join(["python3", *sys.argv]), "runtime_executable": sys.executable,
              "cwd": str(Path.cwd()), "started_utc": started, "validator_processes": 1,
              "driver_runs": 0, "DMA_runs": 0, "negative_tests_requested": args.negative_tests}
    exit_code = 1
    try:
        source = source_checks(batch, args.run_dir)
        input_paths = [args.run_dir / name for name in ("trace.csv", "output.bin", "result.json")]
        input_hashes = {str(p.resolve()): digest(p) for p in input_paths}
        output = (args.run_dir / "output.bin").read_bytes()
        events = read_events(args.run_dir / "trace.csv")
        result = json.loads((args.run_dir / "result.json").read_text())
        reference = full_reference()
        output_sum = verify_data(output, reference)
        counts = verify_events(events)
        verify_result(result, output_sum, counts)
        negatives = negative_checks(output, reference, events) if args.negative_tests else []
        need(all(digest(Path(p)) == value for p, value in input_hashes.items()), "input artifacts changed during validation")
        report = {"schema": "hero-mm-independent-functional-acceptance-v1", "status": "accepted_functional_contract_only",
                  "source": source, "raw_inputs_sha256": input_hashes, "verifier_sha256": digest(Path(__file__)),
                  "output": {"values_checked": VALUES, "bytes": VALUES*4, "format": "uint32_little_endian",
                             "reference": "independent full C[i,j]=sum_k A[i,k]*B[j,k] modulo 2^32",
                             "default_exact_crosscheck": "2*linear_index, indices 0..65535", "sum_u64": output_sum,
                             "mismatches": 0}, "trace": counts, "negative_tests": negatives,
                  "independent_scope": ["All output values against full independent arithmetic reference",
                                        "Every copy issue/DONE/wait, offset, bytes, thread role and finite count",
                                        "A reuse after all 16 tiles, B reload/reuse per tile, C output after 16 tiles and safe generation reuse",
                                        "Six live allocations, all slots freed, monotonic total event order and complete final drain",
                                        "Pinned actual function unchanged except two declared observation blocks; actual source/run hashes"],
                  "limits": ["Host malloc and synchronous CPU memcpy; no target asynchronous DMA/AXI retirement proof",
                             "host_ns supplies observation order only, no target/FPGA timing or G0 deadline result",
                             "512 added barriers change scheduling; no performance inference",
                             "Default B=2I cannot numerically distinguish transpose vs nontranspose; pinned source indices preserve A*B^T",
                             "MAC count is retained source-loop work, not a hardware retired-instruction counter",
                             "Physical slot nonalias follows live malloc/source contract; CSV contains role/slot IDs, not pointer addresses",
                             "Eight-thread team is supported by pinned driver runtime guard and marker, not every worker's per-MAC trace"],
                  "scientific_decision": "functional application contract only; no new G0 timing or research negative conclusion"}
        ledger.update(positive_validation="passed", negative_rejections=negatives, input_artifacts_unchanged=True)
        exit_code = 0
    except (Rejected, OSError, ValueError, KeyError, TypeError, IndexError) as error:
        report = {"schema": "hero-mm-independent-functional-acceptance-v1", "status": "rejected", "error": str(error)}
        ledger.update(positive_validation="rejected", error=str(error))
    ledger.update(exit_code=exit_code, finished_utc=dt.datetime.now(dt.timezone.utc).isoformat(),
                  elapsed_seconds=time.monotonic()-clock_start)
    rendered = json.dumps(report, ensure_ascii=False, indent=2)+"\n"
    if args.output:
        args.output.write_text(rendered)
        ledger["acceptance_path"] = str(args.output.resolve())
        ledger["acceptance_sha256"] = digest(args.output)
    else:
        sys.stdout.write(rendered)
    if args.ledger:
        args.ledger.write_text(json.dumps(ledger, ensure_ascii=False, indent=2)+"\n")
    if exit_code:
        print(report["error"], file=sys.stderr)
    return exit_code


if __name__ == "__main__":
    raise SystemExit(main())
