#!/usr/bin/env python3
"""Independent burst/task-level verifier for the frozen row workflow.

Usage:
  python3 verify-row-results.py --self-test
  python3 verify-row-results.py /path/to/prefix --output verification.json

Input: PREFIX.json and PREFIX.csv, plus the sibling baseline register by default.
No DMA experiment is launched. CSV byte evidence is accepted R_LAST beat totals
and W_LAST accumulated WSTRB totals. Per-beat data, VALID stability, source
immutability and padding contents remain actual driver-CHECK evidence.
"""
from __future__ import annotations

import argparse
import copy
import csv
import hashlib
import json
import sys
from collections import deque
from pathlib import Path


ROW_BYTES, ROW_COUNT, INIT_ROWS, BG_COUNT = 640, 480, 3, 2048
ROW_SRC, ROW_DST, BG_SRC, BG_DST = 0x80000000, 0x80200000, 0x90000000, 0x90100000
FIELDS = ["absolute_cycle", "phase", "phase_cycle", "event", "channel", "job", "id", "address", "value"]
EVENTS = {"dma_issue", "dma_done", "payload_ar", "payload_aw", "payload_first_w",
          "payload_r_last", "payload_w_last", "payload_b", "ds_ar", "ds_r_last",
          "consumer_release", "consumer_deadline_ready", "consumer_deadline_miss",
          "initialization_quiet", "final_quiet", "guard_checks_pass"}


class Rejected(ValueError):
    pass


def require(ok, message):
    if not ok:
        raise Rejected(message)


def release(row):
    return ((row - INIT_ROWS) * 31250 + 8) // 9


def deadline(row):
    return ((row - INIT_ROWS + 1) * 31250) // 9


def integer(value, label):
    require(isinstance(value, int) and not isinstance(value, bool), f"{label}: expected integer")
    return value


def same(obj, key, expected, label):
    require(key in obj, f"{label}: missing {key}")
    actual = obj[key]
    if isinstance(expected, int) and not isinstance(expected, bool):
        integer(actual, f"{label}.{key}")
    elif isinstance(expected, bool):
        require(isinstance(actual, bool), f"{label}.{key}: expected boolean")
    require(actual == expected, f"{label}.{key}: {actual!r} != reconstructed {expected!r}")


def read_trace(path):
    with path.open(newline="") as stream:
        reader = csv.DictReader(stream)
        require(reader.fieldnames == FIELDS, f"CSV header must be exactly {FIELDS}")
        records = []
        for line, row in enumerate(reader, 2):
            require(None not in row and all(row[x] is not None for x in FIELDS), f"CSV line {line}: malformed row")
            for field in FIELDS:
                if field not in ("phase", "event"):
                    try:
                        row[field] = int(row[field], 10)
                    except ValueError as error:
                        raise Rejected(f"CSV line {line}: invalid {field}") from error
            row["line"] = line
            records.append(row)
        return records


def new_job(channel, job):
    src = ROW_SRC + job * ROW_BYTES if channel == 0 else BG_SRC + (job % 16) * 4096
    dst = ROW_DST + (job % 4) * ROW_BYTES if channel == 0 else BG_DST + (job % 16) * 4096
    return {"job": job, "src_pa": src, "dst_pa": dst, "bytes": ROW_BYTES if channel == 0 else 4096,
            "issue": None, "done": None, "first_w": None, "ar": 0, "aw": 0,
            "rd_bytes": 0, "r_bytes": 0, "wr_bytes": 0, "b": 0,
            "read_intervals": [], "write_intervals": []}


def coverage(intervals, base, size, label):
    cursor = base
    for start, end in sorted(intervals):
        require(start == cursor, f"{label}: gap/overlap at {cursor:#x}, next interval {start:#x}..{end:#x}")
        require(end > start, f"{label}: empty interval")
        cursor = end
    require(cursor == base + size, f"{label}: range does not cover the complete COPY")


def check_register(register):
    same(register, "schema", "row-g0-register-v1", "register")
    contract = register.get("contract", {})
    expected = {"frame_bytes": 307200, "init_rows": 3, "steady_rows": 477, "row_bytes": 640,
                "slots": 4, "release": "ceil(j*31250/9), j=0..476",
                "deadline": "floor((j+1)*31250/9)", "cpu_enable": False,
                "background_pages_per_ring": 16, "background_copies": 2048,
                "background_bytes": 8388608, "normal_stop_a": False,
                "static_s1": True, "s2": False, "ptw_latency": 31}
    for key, value in expected.items():
        same(contract, key, value, "registered contract")
    require(contract.get("policy") == "P fixed, A unthrottled, no DS priority, no new gate",
            "register policy differs from frozen ordinary-P contract")


def verify(result, events, register):
    check_register(register)
    same(result, "schema", "actual-dma-row-workflow-v1", "JSON")
    same(result, "cpu_enable", 0, "JSON")
    same(result, "ds_priority", False, "JSON")
    same(result, "quiet", True, "JSON")
    require(any(x.get("mode") == result.get("mode") and x.get("mapping") == result.get("mapping")
                for x in register.get("cases", [])), "mode/mapping is not a registered case")
    require(events, "empty trace")

    # Phase origins are independently inferred from all CSV absolute/relative
    # timestamps. JSON origin scalars are only compared after reconstruction.
    origins = {}
    previous_cycle = -1
    steady_seen = False
    for e in events:
        require(e["event"] in EVENTS, f"unknown CSV event {e['event']!r}")
        require(e["phase"] in ("initialization", "steady"), "unknown phase")
        for field in FIELDS:
            if field not in ("phase", "event"):
                integer(e[field], f"CSV {field}")
                require(e[field] >= 0, f"negative CSV {field}")
        require(e["absolute_cycle"] >= previous_cycle, "CSV timestamps run backwards")
        require(e["id"] <= 255, "CSV AXI/debug ID exceeds recorded hardware width")
        previous_cycle = e["absolute_cycle"]
        if e["phase"] == "steady":
            steady_seen = True
        else:
            require(not steady_seen, "initialization event after steady phase")
        origin = e["absolute_cycle"] - e["phase_cycle"]
        require(origin >= 0, "negative inferred phase origin")
        require(e["phase"] not in origins or origins[e["phase"]] == origin,
                "CSV phase timestamps do not share one origin")
        origins[e["phase"]] = origin
    require(set(origins) == {"initialization", "steady"}, "both CSV phases are required")
    init_start, t0 = origins["initialization"], origins["steady"]
    require(t0 > init_start, "steady origin must follow initialization")

    jobs = [[new_job(0, j) for j in range(ROW_COUNT)], [new_job(1, j) for j in range(BG_COUNT)]]
    active = [None, None]
    issued, completed = [0, 0], [0, 0]
    reads, writes, responses = deque(), deque(), deque()
    owner = [None] * 4
    releases, deadlines = {}, {}
    blocked = failures = ds_requests = ds_bytes = 0
    initial_quiet = final_quiet = guard_pass = None
    warnings = []

    def payload_job(e):
        c, j = e["channel"], e["job"]
        require(c in (0, 1), "payload event has non-DMA channel")
        require(0 <= j < len(jobs[c]), "payload event index outside finite workload")
        require(active[c] == j, f"channel {c} job {j}: payload outside active COPY")
        require(e["phase"] == ("initialization" if c == 0 and j < 3 else "steady"), "COPY phase mismatch")
        return jobs[c][j]

    def matches(burst, e):
        require((burst["channel"], burst["job"], burst["id"], burst["address"]) ==
                (e["channel"], e["job"], e["id"], e["address"]), "burst LAST owner/ID/address mismatch")

    for e in events:
        event, now, c, j = e["event"], e["absolute_cycle"], e["channel"], e["job"]
        require(final_quiet is None or event == "guard_checks_pass", "transaction/event follows terminal quiet")
        if event == "dma_issue":
            require(c in (0, 1) and j == issued[c] and j < len(jobs[c]), "ISSUE duplicated, skipped or beyond target")
            require(active[c] is None, "second same-channel ISSUE before DONE")
            r = jobs[c][j]
            ready = init_start if c == 0 and j < 3 else t0 + release(j) if c == 0 else t0 if j == 0 else jobs[1][j-1]["done"]
            require(ready is not None and now >= ready, "ISSUE before fixed logical ready/release")
            require(e["phase"] == ("initialization" if c == 0 and j < 3 else "steady"), "ISSUE phase mismatch")
            r["issue"] = now
            active[c] = j
            issued[c] += 1
        elif event in ("payload_ar", "payload_aw"):
            r = payload_job(e)
            require(((e["id"] >> 4) & 3) == 2 and ((e["id"] >> 2) & 1) == c, "payload AXI ID master/bank mismatch")
            require(0 <= e["value"] <= 255 and e["address"] % 8 == 0, "invalid AXI burst LEN/alignment")
            size = (e["value"] + 1) * 8
            write = event == "payload_aw"
            base = r["dst_pa"] if write else r["src_pa"]
            require(base <= e["address"] and e["address"] + size <= base + r["bytes"], "burst outside fixed COPY range")
            require(e["address"] // 4096 == (e["address"] + size - 1) // 4096, "burst crosses 4KiB")
            burst = {"channel": c, "job": j, "id": e["id"], "address": e["address"],
                     "beats": e["value"] + 1, "cycle": now}
            r["write_intervals" if write else "read_intervals"].append((e["address"], e["address"] + size))
            r["aw" if write else "ar"] += 1
            if write:
                writes.append(burst)
            else:
                r["rd_bytes"] += size
                reads.append(burst)
        elif event == "payload_first_w":
            r = payload_job(e)
            require(writes, "first W without accepted AW")
            matches(writes[0], e)
            require(r["first_w"] is None and 0 < e["value"] <= 255, "duplicate/missing-strobe first W")
            require(now >= r["issue"] and now >= writes[0]["cycle"], "first W precedes ISSUE/AW")
            if c == 0:
                require(j < 3 or now >= t0 + release(j), "slot overwritten before fixed release")
                old = owner[j % 4]
                require(old is None or (jobs[0][old]["done"] is not None and jobs[0][old]["done"] <= now),
                        "slot overwritten before previous row DONE")
                owner[j % 4] = j
            r["first_w"] = now
        elif event == "payload_w_last":
            r = payload_job(e)
            require(writes, "W_LAST without AW")
            burst = writes.popleft()
            matches(burst, e)
            require(e["value"] == burst["beats"] * 8, "W_LAST actual strobed-byte total differs from AW size")
            require(now >= burst["cycle"] + burst["beats"] - 1, "W_LAST cannot contain stated beat count at this timestamp")
            require(r["first_w"] is not None and r["first_w"] <= now, "W_LAST precedes/misses first accepted W")
            r["wr_bytes"] += e["value"]
            burst["last_cycle"] = now
            responses.append(burst)
        elif event == "payload_b":
            r = payload_job(e)
            require(responses, "B without completed W burst")
            burst = responses.popleft()
            # B has no address on AXI; the driver records address/value zero.
            require((burst["channel"], burst["job"], burst["id"]) == (c, j, e["id"]), "B FIFO owner/ID mismatch")
            require(e["address"] == 0 and e["value"] == 0 and now >= burst["last_cycle"], "invalid B record")
            r["b"] += 1
        elif event == "payload_r_last":
            r = payload_job(e)
            require(reads, "R_LAST without accepted AR")
            burst = reads.popleft()
            matches(burst, e)
            require(e["value"] == burst["beats"], "R_LAST actual accepted beat count differs from AR")
            require(now >= burst["cycle"] + burst["beats"] - 1, "R_LAST cannot contain stated beat count at this timestamp")
            r["r_bytes"] += e["value"] * 8
        elif event == "ds_ar":
            require(c == 3 and j == 0 and ((e["id"] >> 4) & 3) == 3, "DS AR master/owner mismatch")
            require(0 <= e["value"] <= 255 and e["address"] % 8 == 0, "invalid DS burst")
            reads.append({"channel": c, "job": j, "id": e["id"], "address": e["address"],
                          "beats": e["value"] + 1, "cycle": now})
            ds_requests += 1
            ds_bytes += (e["value"] + 1) * 8
        elif event == "ds_r_last":
            require(reads, "DS R_LAST without AR")
            burst = reads.popleft()
            matches(burst, e)
            require(c == 3 and e["value"] == burst["beats"] - 1, "DS R_LAST LEN differs from AR")
            require(now >= burst["cycle"] + burst["beats"] - 1, "DS R_LAST timestamp invalid")
        elif event == "dma_done":
            r = payload_job(e)
            require(r["done"] is None and r["first_w"] is not None, "duplicate DONE or missing first W")
            require(now >= r["first_w"] >= r["issue"], "COPY completion timestamps inconsistent")
            require(all(not (b["channel"] == c and b["job"] == j) for q in (reads, writes, responses) for b in q),
                    "DONE before this COPY's bursts/responses drained")
            require(r["rd_bytes"] == r["r_bytes"] == r["wr_bytes"] == r["bytes"], "DONE byte conservation failed")
            require(r["ar"] > 0 and r["aw"] > 0 and r["aw"] == r["b"], "DONE AW/B conservation failed")
            coverage(r["read_intervals"], r["src_pa"], r["bytes"], f"channel {c} COPY {j} AR")
            coverage(r["write_intervals"], r["dst_pa"], r["bytes"], f"channel {c} COPY {j} AW")
            r["done"] = now
            completed[c] += 1
            active[c] = None
        elif event == "consumer_release":
            require(c == 0 and 3 <= j < ROW_COUNT and e["phase"] == "steady", "release owner/phase invalid")
            require(j not in releases and now == t0 + release(j), "release duplicated or fixed schedule changed")
            require(e["id"] == 0 and e["address"] == j % 4, "release slot record invalid")
            old = owner[j % 4]
            require(e["value"] == (0 if old is None else old + 1), "release records wrong old slot owner")
            if old is not None and jobs[0][old]["done"] is None:
                blocked += 1
            releases[j] = now
        elif event in ("consumer_deadline_ready", "consumer_deadline_miss"):
            require(c == 0 and 3 <= j < ROW_COUNT and e["phase"] == "steady", "deadline owner/phase invalid")
            require(j not in deadlines and now == t0 + deadline(j), "deadline duplicated or fixed schedule changed")
            r = jobs[0][j]
            readable = r["done"] is not None and r["done"] <= now and owner[j % 4] == j
            require((event == "consumer_deadline_ready") == readable, "deadline readiness contradicts actual DONE/slot owner")
            failures += not readable
            deadlines[j] = now
        elif event in ("initialization_quiet", "final_quiet"):
            require((c, j, e["id"], e["address"], e["value"]) == (0, 0, 3, 0, 0), "quiet must sample actual idle bits/in-flight/model queue count")
            require(not reads and not writes and not responses and active == [None, None], "quiet with observer bursts/COPY still pending")
            if event == "initialization_quiet":
                require(initial_quiet is None and e["phase"] == "initialization" and completed == [3, 0], "initial quiet duplicated or premature")
                initial_quiet = now
            else:
                require(final_quiet is None and e["phase"] == "steady" and completed == [ROW_COUNT, BG_COUNT], "final quiet duplicated or premature")
                require(len(releases) == len(deadlines) == 477, "final quiet before all fixed consumer observations")
                final_quiet = now
        elif event == "guard_checks_pass":
            require(guard_pass is None and final_quiet is not None and now >= final_quiet, "guard CHECK marker missing terminal quiet or duplicated")
            require((c, j, e["id"], e["address"], e["value"]) == (0, 0, 0, 0, 0), "guard CHECK marker fields invalid")
            guard_pass = now

    require(issued == completed == [ROW_COUNT, BG_COUNT], "incomplete 480+2048 finite COPY workload")
    require(not reads and not writes and not responses and active == [None, None], "terminal observer queues not drained")
    require(final_quiet is not None, "missing final_quiet raw debug/queue event; JSON quiet=true is insufficient")
    require(set(releases) == set(deadlines) == set(range(3, ROW_COUNT)), "missing fixed row release/deadline events")
    if initial_quiet is None:
        initial_quiet = integer(result.get("init_end_absolute"), "JSON.init_end_absolute")
        require(jobs[0][2]["done"] <= initial_quiet <= t0, "reported init/preparation boundary out of range")
        warnings.append("No initialization_quiet event: total preparation is independent, but initialization/preparation split uses JSON boundary.")
    if guard_pass is None:
        warnings.append("No guard_checks_pass marker: source immutability/data/padding acceptance requires actual driver PASS log and source review.")

    misses = sum(r["done"] > t0 + deadline(r["job"]) for r in jobs[0][3:])
    late = [max(0, r["done"] - t0 - deadline(r["job"])) for r in jobs[0][3:]]
    expected_scalars = {"init_start_absolute": init_start, "init_end_absolute": initial_quiet,
                        "initialization_cycles": initial_quiet - init_start,
                        "prepared_t0_absolute": t0, "preparation_cycles": t0 - initial_quiet,
                        "quiet_cycle": final_quiet - t0, "deadline_misses": misses,
                        "consumer_blocked_at_release": blocked, "consumer_deadline_failures": failures,
                        "consumer_deadline_events": 477, "v_count": 480, "v_steady_count": 477, "a_count": 2048}
    for key, value in expected_scalars.items():
        same(result, key, value, "JSON")
    for channel, key in ((0, "rows"), (1, "background")):
        require(isinstance(result.get(key), list) and len(result[key]) == len(jobs[channel]), f"JSON.{key}: wrong number of COPY records")
        for j, (raw, r) in enumerate(zip(result[key], jobs[channel])):
            phase = "initialization" if channel == 0 and j < 3 else "steady"
            origin = init_start if phase == "initialization" else t0
            ready = init_start if phase == "initialization" else t0 + release(j) if channel == 0 else t0 if j == 0 else jobs[1][j-1]["done"]
            expect = {"job": j, "phase": phase, "ready_absolute": ready, "ready_cycle": ready-origin,
                      "issue_absolute": r["issue"], "issue_cycle": r["issue"]-origin,
                      "first_w_absolute": r["first_w"], "payload_write_start_cycle": r["first_w"]-origin,
                      "done_absolute": r["done"], "done_cycle": r["done"]-origin}
            for field in ("src_pa", "dst_pa", "bytes", "ar", "aw", "rd_bytes", "r_bytes", "wr_bytes", "b"):
                expect[field] = r[field]
            if channel == 0 and j >= 3:
                expect.update(release_cycle=release(j), deadline_cycle=deadline(j),
                              release_absolute=t0+release(j), deadline_absolute=t0+deadline(j))
            for field, value in expect.items():
                same(raw, field, value, f"JSON.{key}[{j}]")

    window_end = t0 + deadline(ROW_COUNT - 1)
    bg_window = [r for r in jobs[1] if t0 <= r["done"] <= window_end]
    totals = [{k: sum(r[k] for r in lane) for k in ("ar", "aw", "rd_bytes", "r_bytes", "wr_bytes", "b")} for lane in jobs]
    return {"schema": "independent-row-verification-v1", "status": "accepted_burst_and_task_level",
            "case": {"mode": result["mode"], "mapping": result["mapping"]},
            "fixed_schedule": {"rows": 477, "release": "ceil((row-3)*31250/9)",
                               "deadline": "floor((row-2)*31250/9)", "last_deadline_cycle": deadline(479)},
            "metrics": {"deadline_misses": misses, "max_lateness_cycles": max(late),
                        "max_release_to_done_cycles": max(r["done"]-t0-release(r["job"]) for r in jobs[0][3:]),
                        "minimum_deadline_slack_cycles": min(t0+deadline(r["job"])-r["done"] for r in jobs[0][3:]),
                        "late_rows": [r["job"] for r in jobs[0][3:] if r["done"] > t0+deadline(r["job"])],
                        "consumer_deadline_failures": failures, "consumer_blocked_at_release": blocked,
                        "background_fixed_window": {"start_absolute": t0, "end_absolute": window_end,
                                                    "inclusion": "t0 <= hardware DONE <= final fixed row deadline",
                                                    "completed_copies": len(bg_window),
                                                    "completed_effective_bytes": sum(r["bytes"] for r in bg_window)},
                        "initialization_cycles": initial_quiet-init_start,
                        "descriptor_preparation_cycles": t0-initial_quiet,
                        "total_before_steady_cycles": t0-init_start,
                        "V_last_done_cycle": jobs[0][-1]["done"]-t0,
                        "A_last_done_cycle": jobs[1][-1]["done"]-t0,
                        "T_data_cycle": max(jobs[0][-1]["done"], jobs[1][-1]["done"])-t0,
                        "T_return_cycle": final_quiet-t0},
            "conservation": {"issued": issued, "completed": completed, "foreground": totals[0],
                             "background": totals[1], "DS_AR_requests": ds_requests, "DS_AR_bytes": ds_bytes,
                             "observer_read_write_B_queues_empty": True,
                             "raw_quiet_event_sample": {"dma_job_quiet": 3, "in_flight": 0, "model_nonempty_queues": 0}},
            "physical_reservation_bytes": 442368 if result["mapping"] == "4k" else 6291456,
            "preparation_cost_boundary": "Clock costs start at init_start. Earlier model memory/PTE construction is unmeasured; physical reservation is authorization capacity, not host allocation or synthesized area.",
            "driver_guard_marker_present": guard_pass is not None, "warnings": warnings,
            "independent_scope": ["Fixed schedule and release/issue/first-W/DONE times",
                                  "Exact AR/AW range coverage without gaps or overlaps",
                                  "Accepted burst R-beat and W-strobe totals, global head-serial R FIFO and W/B FIFO",
                                  "Finite 480+2048 counts, raw terminal idle sample, complete event-queue drain",
                                  "Same fixed-window background completion and real task-end/preparation costs"],
            "driver_dependent_scope": ["Every beat data/response/LAST correctness and held VALID stability",
                                       "Full source immutability, destination padding and final data contents",
                                       "CPU-off/model/policy configuration and debug signal interpretation",
                                       "Actual executable/source provenance; source/binary identities must be recorded by root"],
            "not_claimed": ["Independent per-beat data replay", "FPGA timing or synthesized resource cost",
                            "Actual Sobel computation", "Application-level or statistical performance generality"]}


def synthetic_fixture():
    """In-memory physically serialized trace; no hardware/model is run."""
    init_start = 10
    events, lanes = [], [[None] * ROW_COUNT, [None] * BG_COUNT]
    t0 = 1000

    def emit(cycle, event, c=0, j=0, ident=0, address=0, value=0, phase="steady"):
        events.append(dict(absolute_cycle=cycle, phase=phase, phase_cycle=cycle-(init_start if phase == "initialization" else t0),
                           event=event, channel=c, job=j, id=ident, address=address, value=value))

    def intervals(base, size):
        while size:
            part = min(size, 2048, 4096-base % 4096)
            yield base, part
            base, size = base + part, size - part

    def job(c, j, cursor):
        r = new_job(c, j)
        phase = "initialization" if c == 0 and j < 3 else "steady"
        origin = init_start if phase == "initialization" else t0
        ready = init_start if phase == "initialization" else t0+release(j) if c == 0 else t0 if j == 0 else lanes[1][j-1]["done_absolute"]
        ident = 32 + c * 4
        issue, first = cursor, None
        emit(cursor, "dma_issue", c, j, phase=phase)
        # Separate exact source/destination segmentation, as the real DMA can
        # use a different source and destination boundary for one COPY.
        ar_count = aw_count = 0
        for addr, size in intervals(r["src_pa"], r["bytes"]):
            cursor += 1
            emit(cursor, "payload_ar", c, j, ident, addr, size//8-1, phase)
            cursor += size//8 + 2
            emit(cursor, "payload_r_last", c, j, ident, addr, size//8, phase)
            ar_count += 1
        for addr, size in intervals(r["dst_pa"], r["bytes"]):
            cursor += 1
            emit(cursor, "payload_aw", c, j, ident, addr, size//8-1, phase)
            if first is None:
                first = cursor + 1
                emit(first, "payload_first_w", c, j, ident, addr, 255, phase)
            cursor += size//8
            emit(cursor, "payload_w_last", c, j, ident, addr, size, phase)
            cursor += 1
            emit(cursor, "payload_b", c, j, ident, phase=phase)
            aw_count += 1
        cursor += 1
        emit(cursor, "dma_done", c, j, phase=phase)
        record = {"job": j, "phase": phase, "src_pa": r["src_pa"], "dst_pa": r["dst_pa"], "bytes": r["bytes"],
                  "ready_cycle": ready-origin, "issue_cycle": issue-origin, "payload_write_start_cycle": first-origin,
                  "done_cycle": cursor-origin, "ar": ar_count, "aw": aw_count, "rd_bytes": r["bytes"],
                  "r_bytes": r["bytes"], "wr_bytes": r["bytes"], "b": aw_count,
                  "ready_absolute": ready, "issue_absolute": issue, "first_w_absolute": first, "done_absolute": cursor}
        if c == 0 and j >= 3:
            record.update(release_cycle=release(j), deadline_cycle=deadline(j), release_absolute=t0+release(j), deadline_absolute=t0+deadline(j))
        lanes[c][j] = record
        return cursor + 1

    cursor = init_start + 1
    for j in range(3):
        cursor = job(0, j, cursor)
    init_end = cursor
    emit(init_end, "initialization_quiet", ident=3, phase="initialization")
    cursor = t0 + 1
    for j in range(3, ROW_COUNT):
        cursor = max(cursor, t0+release(j)+1)
        cursor = job(1, j-3, cursor)
        cursor = job(0, j, cursor)
    for j in range(477, BG_COUNT):
        cursor = job(1, j, cursor)
    terminal = max(cursor, t0+deadline(479)+1)
    emit(terminal, "final_quiet", ident=3)
    emit(terminal, "guard_checks_pass")
    first_w = [e for e in events if e["event"] == "payload_first_w" and e["channel"] == 0]
    for j in range(3, ROW_COUNT):
        old_events = [e for e in first_w if e["job"] % 4 == j % 4 and e["absolute_cycle"] < t0+release(j)]
        old = max(old_events, key=lambda e: e["absolute_cycle"])["job"] if old_events else None
        emit(t0+release(j), "consumer_release", j=j, address=j % 4, value=0 if old is None else old+1)
        emit(t0+deadline(j), "consumer_deadline_ready", j=j)
    priority = {"consumer_release": 0, "dma_done": 6, "dma_issue": 7,
                "consumer_deadline_ready": 8, "final_quiet": 9, "guard_checks_pass": 10}
    events.sort(key=lambda e: (e["absolute_cycle"], priority.get(e["event"], 3)))
    result = {"schema": "actual-dma-row-workflow-v1", "mode": "p", "mapping": "4k", "cpu_enable": 0,
              "ds_priority": False, "init_start_absolute": init_start, "init_end_absolute": init_end,
              "initialization_cycles": init_end-init_start, "prepared_t0_absolute": t0,
              "preparation_cycles": t0-init_end, "quiet_cycle": terminal-t0, "deadline_misses": 0,
              "consumer_blocked_at_release": 0, "consumer_deadline_failures": 0, "consumer_deadline_events": 477,
              "v_count": 480, "v_steady_count": 477, "a_count": 2048, "quiet": True,
              "rows": lanes[0], "background": lanes[1]}
    return result, events


def self_test(register):
    result, events = synthetic_fixture()
    verify(result, events, register)
    mutations = []
    def reject(label, mutation):
        altered_result, altered_events = copy.deepcopy(result), copy.deepcopy(events)
        mutation(altered_result, altered_events)
        try:
            verify(altered_result, altered_events, register)
        except Rejected as error:
            mutations.append({"mutation": label, "rejected": True, "reason": str(error)})
        else:
            raise Rejected(f"negative self-test wrongly accepted {label}")
    reject("JSON deadline +1", lambda r, e: r["rows"][3].__setitem__("deadline_cycle", deadline(3)+1))
    reject("JSON finite count -1", lambda r, e: r.__setitem__("a_count", BG_COUNT-1))
    reject("JSON completed byte -1", lambda r, e: r["background"][0].__setitem__("wr_bytes", 4095))
    reject("CSV W_LAST strobed bytes -1", lambda r, e: next(x for x in e if x["event"] == "payload_w_last").__setitem__("value", 639))
    reject("CSV R_LAST accepted beats -1", lambda r, e: next(x for x in e if x["event"] == "payload_r_last").__setitem__("value", 79))
    reject("CSV raw quiet removed", lambda r, e: e.__setitem__(slice(None), [x for x in e if x["event"] != "final_quiet"]))
    reject("CSV fixed deadline timestamp +1", lambda r, e: next(x for x in e if x["event"] == "consumer_deadline_ready").__setitem__("absolute_cycle", t0_for(r)+deadline(3)+1))
    return {"synthetic_positive": "accepted", "negative_mutations": mutations,
            "scope": "in-memory validator checks only; no DMA/model experiment"}


def t0_for(result):
    return result["prepared_t0_absolute"]


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("prefix", nargs="?", type=Path, help="prefix for .json and .csv")
    parser.add_argument("--register", type=Path, default=Path(__file__).with_name("row-baseline-register.json"))
    parser.add_argument("--output", type=Path, help="write verification JSON (stdout otherwise)")
    parser.add_argument("--self-test", action="store_true", help="also exercise valid synthetic data and rejected mutations")
    args = parser.parse_args()
    if args.prefix is None and not args.self_test:
        parser.error("provide a PREFIX or --self-test")
    try:
        register = json.loads(args.register.read_text())
        test = self_test(register) if args.self_test else None
        if args.prefix is not None:
            prefix = str(args.prefix)
            json_path, csv_path = Path(prefix+".json"), Path(prefix+".csv")
            report = verify(json.loads(json_path.read_text()), read_trace(csv_path), register)
            report["inputs"] = {str(p.resolve()): {"sha256": sha256(p)} for p in (json_path, csv_path, args.register)}
            report["verifier_sha256"] = sha256(Path(__file__))
            if test is not None:
                report["self_test"] = test
        else:
            report = {"schema": "independent-row-verifier-self-test-v1", "status": "passed", "self_test": test}
        rendered = json.dumps(report, ensure_ascii=False, indent=2)+"\n"
        if args.output:
            args.output.write_text(rendered)
        else:
            sys.stdout.write(rendered)
        return 0
    except (Rejected, OSError, ValueError, KeyError, TypeError) as error:
        print(json.dumps({"schema": "independent-row-verification-v1", "status": "rejected",
                          "error": str(error)}, ensure_ascii=False), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
